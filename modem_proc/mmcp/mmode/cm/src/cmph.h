#ifndef CMPH_H
#define CMPH_H
/*===========================================================================

         C A L L   M A N A G E R   P H   H E A D E R   F I L E

DESCRIPTION
  This header file contains all the definitions necessary to interface
  with CMPH.C

  CM phone object. The phone object is responsible for:
  1. Processing clients' phone commands.
  2. Processing LL replies for clients' phone commands.
  3. Processing LL phone related notifications.
  4. Notifying the client list of phone events.

Copyright (c) 1998 - 2014 by Qualcomm Technologies INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

===========================================================================*/



/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/mmcp.mpss/7.9.0/mmode/cm/src/cmph.h#4 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/31/14   xs      hVOLTE SSAC support
11/13/13   xs      Add control for VOLTE SR LTE failure
10/07/13   jvo     Added flag is_rat_bal to cmph_s_type
09/23/13   cl      CM support CM_PH_SIM_AVAILABLE_EVENT to AT-COP
07/22/13   fj      Add support for DSDX L+G
08/13/13   cl      CM supports explicit PS_DETACH when capability from PM changed
07/15/13   ss      FR1860 - T/G on DSDS
06/19/13   gm      Send pref to SD-ALL stacks before ONLINE/PWRUP ind
06/18/13   gm      FR 2617 - OMADM IMS and ADMIN PDN Disable Requirement
05/16/13   fj      Avoid duplicate preferece forced to SD after ATCOP PS
                   detach
04/11/13   xs      FRAT support
01/25/13   gm      FR 3059 - ONLINE optimization - skip duplicate subscription
01/07/13   qf      FR 2564 - World mode support for DSDA
01/07/12   xs      Added PS supplemetary service support
08/07/12   gm      RF init status synch and handling oprt modes
06/22/12   xs      Hold subscription unavailable and ok_to_proceed to GSDI
                   until get DS session termination confirmation
03/01/12   fj      Added support for DRX.
05/14/12   cl      Allow emergency call with originated Voice Type before
                   PRL_INIT
04/24/12   sg      Add required changes to support TDS in Automatic mode
03/23/12   gm      DIME integration fix: TMC deprecation and others.
03/19/12   ak/gm   Fix ETM to wait for detach
03/06/12   ak      Deprecate is_data_switch_over_call_success from the CM code
02/19/12   ak      Move cm_gwl_avoidance_timers to CMPH
11/30/11   jh      Added feature for dynamic rat acq order change
02/09/12   cl      Update the pref_reas when forcing HDR stacks
01/17/12   aj/cl   SC SVLTE SO 33 suspend changes
01/06/12   gm      Data synchronization changes for freefloating platform
01/05/12   qf      Block GET_NET_LIST request by CM, if mode_pref contains
                   3GPP2
12/21/11   sg      Move nv items to cm_v.h file
12/20/11   sg      Move nv items from cm.h file to this file
11/18/11   sg      Added new function cmph_is_valid_network_type_on_ss()
10/05/11   xs      Shorten BSR to force LTE scan after HDR srv lost
08/17/11   xs      Added new mode pref CDMA_HDR_GSM
07/24/11   aj      SC SVLTE 1.0 merge to mainline
07/05/11   rn      Terminate Get Net only when reqd and abort seach at LL too
06/14/11   gm      Thermal emergency fixes - resuming HDR, LPM->Online
05/18/11   rn      WCDMA Channel locking feature
05/03/11   sv      Remove AMPS feature flag
04/21/11   sv      Correct Lint Errors
04/12/11   fj      Add new field in cmph_s_type for PLMN blocking.
04/08/11   sv      Add DRS bit param for Voice calls
03/24/11   cl      FUSION2 MMSS-Arbitration centralization rework
03/22/11   cl      Add GW-Throttle Mechanism during HDR/LTE Idle for FUSION2
03/15/11   gm      Separate features for CFM interface & therm emerg
03/15/11   cl      Allow MDM to grant permission to MSM when permission is
                   pending
03/13/11   cl      Remove duplicated cmph_msm_grant_scan_perm_if_pending
03/09/11   cl      Add support for SVLTE2 MMSS arbitration
03/03/11   gm      Added separate queue processing for M2M indications
02/28/11   gm      Support for thermal emergency feature
02/28/11   rk      Adding support for LTE UE mode preference
02/25/11   sn      Support for graceful pdp deactivation before detach.
02/17/11   cl      Add support FEATURE_MMODE_REMOVE_1X for FUSION-MDM
02/14/11   rm      LTE RAT-B changes
02/02/11   xs      Wait for DS notification for PWROFF/LPM oprt mode change
01/27/11   rk      Copying LTE band pref in priority queue for data call
01/19/11   ak      Added support for periodic GWL scans in DO only coverage.
12/23/10   sv      Integrate DSDS feature
10/20/10   xs      Added new fields rsrp_delta and rsrq_delta to cmph_s_typ
10/05/10   rk      Merging 1XCSFB pre-registration code
09/22/10   cl      Extend MC_MAX_REPLY to 30 seconds
09/15/10   rm      Release-7 changes
08/26/10   gm      Removed calls to cmnv_read_wait from frequently called APIs
07/27/10   xs      Remove the subscription buffering
06/22/10   rm      Handle EF_RAT chnage when Subs available comes before
                   ONLINE during power-on
06/15/10   aj      support for IRAT measurement support
05/13/10   xs      Rename cmph_pref_nochg_e enum type
05/06/10   xs      Don't update NV when NO_CHANGE specified
03/03/10   rn      Move to Auto with MM17 reject with new NV
02/15/10   cl      Added support for blocking certain pages during emergency
                   callback mode
01/19/10   aj      Added VOIP support for emergency call
01/12/10   fj      Removed tab.
01/06/09   fj      Added LTE band pref mapping functions.
12/08/09   fj      Added lte_band_pref/lte_band_capability in necessary APIs.
10/15/09   ks      Sending SS event as response to client against
                   sys_sel_pref cmd when network mode change from auto to
                   manual or vice versa
12/22/08   fj      Enabled some APIs for LTE.
08/07/09   am      Adding functions to set/get GW and CDMA subscription from
                   card status and card intialization completed status.
04/23/09   rm      Updating correct mode pref of phone when data_avaialable
                   request comes. Added misc_intersect_mode_pref
04/04/09   ks      Added FEATURE_CM_DISABLE_CALL_TYPE feature support
03/12/09   rm      Removing cmph_otasp_update_mode
03/09/09   rm      Updating correct mode pref of phone when data_avaialable
                   request comes. Added cmph_get_srv_type.
03/01/09   sv      Lint cleanup.
01/19/09   sv      Updated cmph_term_pending_get_networks_req to accept SS
12/08/08   fj      Added mode_capability and band_capability in cmph_s_type;
                   Updated function cmph_get_mode_band_capability();
12/25/08   sv      Updated the cmph_is_valid_network_selection_mode_pref
                   to validate WLAN network selection mode preference.
12/11/08   am      Changes for  mode retained after power cycle.
10/21/08   ks      Update default threshold value for sir and pathloss and
                   add masks for RSSI event
09/18/08   rm      RAT Balancning enhancement- Block user from changing mode
                   pref only if ENS flag is ON and UIM_USIM_RAT file is
                   present - Adding new field 'is_uim_usim_rat_present '
09/10/08   ak      Fixed scenario of data call drop during GSM scan
09/04/08   dm/sv   Implemented WPS support for WLAN
07/30/08   ak      Fix for the HICPS scenario.
06/11/08   fj      Added API cmph_get_mode_band_capability,
                   added API cmutil_map_sd_band_pref_to_cm_band_pref.
06/11/08   sv      Exposing cmph_info_alloc for power collapse buffered
                   events notifications.
06/6/08    rn      Added fn for deleting ps activity from prio queue
06/06/08   sv      Removing unneccesary snd.h
05/08/08   sv      Added support for detecting the supported msm
                   and idenfity if modem is available.
                   Featurized snd.h under CDMA
04/16/08   vk      changed subscription behaviour to buffer the subscription
                   command if the mode pref does not contain the technology,
                   but will allow all commands to go through if anyone
                   subscription goes through also checks buffers and sends
                   subscription command when mode pref changes
12/16/07   ks      Added cm_ph_cmd_signal_strength_delta
12/13/07   sk/ak   Added DCM.
12/12/07   rm      Adding RAT-balancing feature support
11/16/07   rn      Added support for indicating clients of phone event in use
                   state when there is MT call coming in while MO call end
                   cnf is still expected
06/22/07   sk      DDTM rewrite.
06/05/07   pk      Added support to read mob_cai_rev from NV
04/03/07   sk      Added cmph_cmd_client_release()
03/09/07   sk      RAM usage reduction.
02/26/07   ka      Changes to support Voip deregistration.
11/17/06   pk      Added support for HYBR_GW and UMTS -> 1X handover cont...
11/07/06   pk      Added support for HYBR_GW and UMTS -> 1X handover
08/29/06   sk      Added WLAN Adhoc support.
08/11/06   pk      Added support for UOOS
06/05/06   ka/ic   Ignoring LPM_OK report received from MMOC when CM is not
                   waiting on it.
04/06/06   sk      Added cmph_map_sd_ss_type_to_cm_ss_type()
04/01/06   ka      Changes for emerg serv category during orig, aborting
                   manual search and returning current band with srv ind.
03/10/06   pk      Added support for high PSIST
03/09/06   pk      Added support for POWER COLLAPSE
01/06/06   ka      Externing cmph_get_gprs_anite_gcf_ptr().
12/14/05   ic      Changed featurization for cmph_otasp_update_mode() so it
                   links for JCDMA builds
12/13/05   ic      Restored cmph_otasp_update_mode()
12/11/05   ic      Lint cleanup
11/10/05   pk      Added support for privacy_pref, and is_privacy in the
                   phone object
11/08/05   sk      Separated CM reports and commands.
11/01/05   ka      Helper functions to check if nam change is pending
10/25/05   pk      Fixed RVCT compiler warnings and added mapping functions
10/20/05   pk      Cleaned RVCT compiler warnings and added mapping functions
10/18/05   pk      Added cmutil_map_sd_hybr_pref_to_cm_hybr_pref()
10/13/05   ic      Mainlined FEATURE_HDR_HICPS20 and FEATURE_HDR_HICPS30
09/14/05   pk      Extern definition for cmutil_map_sd_hybr_pref_to_cm_hybr_pref
09/12/05   pk      Added cmph_pd_session_start(), cmph_pd_session_end()
08/15/05   pk      Added support for priority Queues for origination
                   parameters
08/15/05   pk      Extern definition for cmph_get_is_hdr_kicked_ptr()
08/11/05   ka      Adding CMPH_MAX_VAL_ACM
06/24/05   ic      Adding CCBS feature (merged in P4 change list 204600)
06/06/05   pk      Added access overload class information to the cmph_s_type
05/11/05   ic      Moved cmph_net_list_copy() to cm.h/.c
05/04/05   jqi     Fixed for hybr data switch over.
04/08/05   pk      Removed cmph_get_cur_hybr_status(void),
                   cmss_is_stack1_operational is the correct function
02/17/05   pk      Added cmutil_map_cm_band_pref_to_sd_band_pref()
02/01/05   dk      Added WLAN Support.
01/24/05   ic      For consistency, renamed cmph_is_ph_in_emerg_cb_mode() to
                   cmph_is_in_emergency_cb()
11/11/04   sj      Rearranged featurization for DDTM_CNTL.
09/17/04   ic      Merged in FEATURE_ALS changes from Saber 4.2 release
                   (MSMSHARED_CM.02.00.19.00.ALS.04)
09/10/04   jqi     Any system preference change or operation mode change
                   should terminate a pending get network request.
09/03/04   ic      Merged in FEATURE_ALS changes from Saber 4.2 release
                   (MSMSHARED_CM.02.00.19.00.ALS.03)
09/03/04   dk      Added is_ps_srv_req_sent flag.
08/30/04   sj      Made orig_mode as an array. Added get/update orig mode
                   funcs
08/27/04   jqi     Added ps dormant optimization.
08/20/04   sj      Added support for PRL Preference.
08/12/04   ws      Remove cm20 support.
08/04/04   ic      Removed cmph_stop_mode_req() as it became obsolete
07/21/04   sj      Added support for umts to 1x handover.
07/12/04   ic      Added support for FEATURE_ALS
07/09/04   sj      Created fn. for checking if pref_term is temporary.
06/30/04   dk      Added cmph_is_waiting_for_ps_sig_down().
06/25/04   dk      Added cmph_set_ps_sig_down_ind_wait_flag() and
                   cmph_set_ps_sig_down_wait_time().
06/22/04   ic      Comments clean up
06/10/04   dk      Added cmph_call_end_req function.
05/14/04   ka      Fixed Lint errors.
04/13/04   ka      Moved cmph_unforce_mode_on_the_fly out of CDMA
                   featurization
04/08/04   ws      Merged changes from MSMSHARED_CM.01.00.32.00.03
03/17/04   sj      Added new DDTM parameters to phone object.
02/25/04   ic      Changes to make the code pass unit test cases and at the
                   same time make calls in Saber build
02/06/04   ws      Initial jaguar/mainline merge.
01/21/04   RI      Modified cmph_force_pref_on_the_fly() to include service
                   domain preference parameter.
01/13/04   ka      Merged changes from MSM6200
10/28/03   ws      Removed uim pref slot support.
10/03/03   ws      Added get function for ps data orig prefs.
10/01/03   ws      Merged changes from GW Targets
09/23/03   ws      Merged fix when SIM fails do not end the emergency call.
09/18/03   ws      Added support for dual slots.
09/18/03   RI      Added signaling_conn_rel_req field to cmph_s_type.
09/16/03   vt      Added preferred slot handling.
09/12/03   prk     Added ps_service_req_uptime field to cmph_s_type.
09/03/03   jds     Correct bug when SIM fails (or is removed) during an
                   emergency call (do NOT end the call).
08/28/03   ws      Added access functions for subscription information.
08/22/03   vt      Mainlined packet state handling.
04/28/03   AT      Lint cleanup.
04/11/03   prk     Removed references to FEATURE_AOC.
02/28/03   ws      Featurized cmph_send_stop_mode_req for GW
02/27/03   ws      Fixes for MO SDB timeout problems.
02/25/03   RI      Added orig_srv_domain_pref in cmph_s_type for PS attach/
                   detach on demand.
                   Modified the parameters in cmph_send_service_req().
02/06/03   vt      Replaced answer_duration with answer_uptime in
                   cmph_s_type structure.
02/06/03   prk     Added valid_card in cm_ph_s_type.
02/04/03   ws      Modified cmph_otasp_update_mode to not read from NV.
01/21/03   ic      For consistency, replaced answer_uptime with
                   answer_duration in cmph_s_type structure.
01/21/03   ws      Updated copyright information for 2003
11/22/02   PRK     Added support for processing user preferred PLMN list.
11/19/02   vt      Added changing RTRE configuration.
11/15/02   vt      Properly featurized the packet state.
11/14/02   vt      Merged changes from GW mainline.
                   [YR]Added test control type handling.
                   [CS]Added cmph_send_service_req defintion.
09/10/02   ws      Added initial cm20 backward compatibility.
07/22/02   PRK     Added available_networks to cmph_s_type.
04/09/02   sh      Modified cmph_mo_sms_end and added
                   cmph_select_dbm_mode_pref
04/02/02   sh      added first_time_camp_on_1x
03/08/02   hxw     Modified cmph_mo_sms_end() to support MSBased gpsOne.
03/21/02   sh      Added orig start reason in CMPH_ON_COLLOC_AND_NEW_HYBR_OFF
03/03/02   sh      Used pref_reas instread of orig_mode in some hybrid related
                   macros.
02/20/02   sh      Added data session handoff support for hybrid operation
02/06/02   sh      Added hybrid 3.0 support
01/25/02   sj      Added support for Tiered services - user zone feature.
01/19/02   az      CR 17624, Added a parameter in cmph_force_mode_on_the_fly()
12/11/01   sh      Added preliminary hybrid operation NV item support
12/08/01   sh      Added preliminary hybrid operation support
05/04/02   PRK     Added prst_srv_domain_pref, prst_network_sel_mode_pref,
                   network_sel_mode_pref, prst_acq_order_pref and
                   acq_order_pref fields to cmph_s_type.
04/22/02   AT      Added cm_ph_aoc_no_funds_available and cm_ph_aoc_no_funds
                   function prototypes.
01/04/02   PRK     Replaced FEATURE_PLMN with FEATURE_NEW_PLMN.
                   Replaced terminology of rat and service type with system
                   mode and service domain respectively.
                   Added is_subscription_available flag to cmph_s_type.
10/18/01   PRK     Added changes for WCDMA/GSM mode.
                     Added support for PLMN selection/reselection
                      (FEATURE_PLMN).
                     Re-added Serving System group and moved srv_state,
                      roam_status,
                     rssi from phone group to serving system group
                      (FEATURE_NEWSS).
10/17/01   AT      Added support for Advice of Charge (AOC)
10/15/01   RI      Added support for Supplementary Services ( GSM/WCDMA ).
01/18/02   ic      Merged MSM_CM.05.03.33 on top of tip CCS CM
                   Diffed MSM_CM.05.03.32 with MSM_CM.05.03.33 and only the
                   changes between those two were merged with CCS CM
12/13/01   ic      Merged MSM_CM.05.03.32 onto SVD CM tip.
                   CCS CM code started from CM VU MSM_CM.05.03.25 + WCDMA/GSM
                   stuff from SIMBA team merged on top of it.
                   We diff'ed MSM_CM.05.03.25 with MSM_CM.05.03.32
                   (CM VU in N*2165) that we needed to merge with and changes
                   between those two VUs were merged onto CCS CM tip.
10/29/01   sh      Added MSM5500 release 2.0 hybrid operation support
10/23/01   VT/IC   Baseline for CCS
08/21/01   ic      Main-lined FEATURE_NEWCM
08/23/01   HD      Added RSSI reporting for GSM/WCDMA.
08/21/01   ic      Main-lined FEATURE_NEWCM
04/27/01   HD      Added changes for WCDMA/GSM mode.
04/13/01   SH      Added Preliminary HDR support
03/27/01   RC      Added SD 2.0 trimode support.
02/16/01   SH      Added is_end_ack_pending flag
02/06/01   RC      Added support for System Determination 2.0.
01/23/01   ych     Merged T53 and JCDMA features.
12/03/99   VT      Merged TriMode Code.
05/31/99   SH      Added CM 2.0 features
01/07/99   RC      Changes per ARM compiler.
12/30/98   RC      Changes per 4th week of I&T.
12/08/98   RC      Changes per second week of I&T.
11/24/98   RC      Initial release.

===========================================================================*/


/**--------------------------------------------------------------------------
** Includes
** --------------------------------------------------------------------------
*/

#include "mmcp_variation.h"
#include "comdef.h"    /* Definition for basic types and macros */
#include "sys.h"        /* system wide common types */
#include "sys_v.h"
#include "cm.h"        /* External interface to cm.c */
#include "cm_v.h"
#include "cmi.h"       /* Internal interface to cm.c */
#include "cmcall.h"    /* Interface to CM call object */
#include "nv.h"        /* Interface to NV services */
#include "subs_prio.h" /* Interface to MCS services*/
#include "mmoc_v.h"
#include "cmwll.h"
#include "cm_i.h"       /* Internal interface to cm.c */
#include "cmxcall.h"
#include "mcfg_refresh.h"
#include "cmclnup.h"
#ifdef FEATURE_DISABLED_HABANERO
#include "qrarb.h"
#endif
#include "mcfg_nv_api.h"
#include "gts_api.h"
#include "cmefs.h"

/**--------------------------------------------------------------------------
** Defines
** --------------------------------------------------------------------------
*/


#define CMPH_INVALID_NUM_OF_SIMS  (int)(-1)
#define CMPH_INVALID_NUM_OF_SESSION_TYPES  (int)(-1)

#define CMPH_DEFAULT_RSSI_DELTA     5 /* Default RSSI delta change threshold
                                      ** for SIGNAL STRENGTH reporting
                                      */

#define CMPH_DEFAULT_ECIO_DELTA     2 /* Default ECIO delta change threshold
                                      ** for SIGNAL STRENGTH reporting
                                      */

#define CMPH_DEFAULT_IO_DELTA       5 /* Default IO delta change threshold
                                      ** for SIGNAL STRENGTH reporting
                                      */

#define CMPH_DEFAULT_SIR_DELTA      3 /* Default SIR delta change threshold
                                      ** for SIGNAL STRENGTH reporting
                                      */

#define CMPH_DEFAULT_PATHLOSS_DELTA 5 /* Default PATHLOSS delta change threshold
                                      ** for SIGNAL STRENGTH reporting
                                      */

#define CMPH_DEFAULT_CQI_DELTA      1 /* Default CQI delta change threshold
                                      ** for SIGNAL STRENGTH reporting
                                      */

#define CMPH_DEFAULT_RSRP_DELTA     5  /* Default RSRP delta change threshold
                                       ** for SIGNAL STRENGTH reporting
                                       */

#define CMPH_DEFAULT_RSRQ_DELTA     5  /* Default RSRQ delta change threshold
                                       ** for SIGNAL STRENGTH reporting
                                       */

#define CMPH_HOLD_MODE_PREF_TIME    4 /* seconds */
/* At end of call, delay unforcing the mode preference for
** this long.
**
** NOTE that we delay unforcing the mode preference in order to
** avoid loading MC with pref sys changes in a situation where
** the user is pressing SEND, END, SEND, END, ... */


#define CM_MMGSDI_ABORT_TIMER_VALUE_SECS    5 /* seconds */

#define CM_MMGSDI_ABORT_TIMER_VALUE_MILLI_SECS    5000 /* milli seconds */

#define CMPH_GW_PKT_SIGNALING_DOWN_WAIT_TIME  5  /* seconds */
#define CMPH_GW_PKT_WAIT_TIME_NONE           (dword) (0)

/* At the end of last Packet call for GSM/WCDMA, we would
** wait for 5 sec. This allows network to teardown signaling
** during this time. By introducing wait, we will avoid the
** race condition when a PDP CONTEXT ACTIVE request is sent
** while network is tearing down the PS signaling.
*/

#define CM_PH_ORIG_MODE_UPTIME_NONE (dword) (-1)
/* This is a special value indicating that the call/activity is still
** in progress
*/

#define CMPH_MAX_VAL_ACM      0xFFFFFF
/* ACM value should not exceed 0xFFFFFF (Spec 31.102, 4.2.9)
** Since SIM has only 3 bytes of storage allocated to this.
*/

#define CMPH_MAX_MC_REPLY_TIME          30    /* seconds */
/* Upper bound on how long a client phone-command waits for a reply
** from LL. After such time the command considered failed due to
** communication problems with lower layers
**
** Increased REPLY_TIME from 2 to 4 to account for bigger delay in lower
** layers confirming with LPM_OK
*/

#define CMPH_MAX_IPAPP_DEREG_TIME       5
/* Max Time allowed by APP to report dereg.
**
** If in service, bringing up PS call and deregistering
** should not take more than 5 secs
**
** If not in service, ipapp will imeediately return DEREG.
*/

#define CMPH_MAX_DS_TRAFFIC_DOWN_TIME       5
/* Max Time allowed by DS to tear down the traffic.
*/

#define CMPH_MAX_DS_TRAFFIC_DOWN_TIME_REFRESH  60
/* Max Time allowed by DS to tear down the traffic 
   due to SIM refresh.
*/

#define CMPH_CMREG_ABORT_CC_CNF_TIME          5
/* Max time CM is allowed to wait for ABORT_CC_CNF */

#define CM_PERMISSIONS 0777
/* Added for Auto Register Feature */

#define CMPH_MAX_UPTIME 0xffffffff
/* Added for operating mode hold FR */

#define CMPH_WAIT_OPRT_MODE_TIME 5
/* CM will buffer operating mode online for this much time */

#define CMPH_WAIT_SUBSC_READY_TIME 5
/* CM will buffer sub-ready for this much time */

#define CMPH_WAIT_DEACTIVATE_SUBS_TIME 500
/* CM will buffer deactivate subs for this much time */

#define CMPH_MAX_CS_REDIAL_COUNT_FOR_EMERG                0xFF
/*Replicates unlimited attempts on CS without falling back to PS for emergency calls */

#define CM_PERIODIC_MANUAL_SEARCH_DEFAULT_TIMER 86400
/*Default timer for periodic internal manual search*/
#define CM_PERIODIC_MANUAL_SEARCH_PWRUP_TIMER 900
/*Timer for internal manual search at power up*/
#define CM_PERIODIC_MANUAL_SEARCH_REDIAL_TIMER 900
/*Redial timer for periodic internal manual search*/

#define CM_UI_NOT_READY_TIMER     60

#define CM_MAN_SRCH_DEFAULT_TIMER 5
#define CM_MAN_SRCH_CALL_TIMER    150
#define CM_MAN_SRCH_MAX_RETRIES   30
#define CM_MAN_SRCH_CALL_END_HOLD_TIMER 2
#define CM_MAN_SRCH_MAX_TIMER    180

#define CM_INQUISITIVE_SEARCH_FAST_RETRY_TIMER 3
#define CM_INQUISITIVE_SEARCH_NUM_ATTEMPTS_MAX 5
#define CM_INQUISITIVE_SEARCH_MAX_TIME 300

#define CM_QRARB_RESTART_ID_INVALID    0xFF

#define CMPH_PS_CALL_WAIT_SRV_CNF_HOLD_ORIG_UPTIME                3 /* seconds */
/* Time to hold PS call when other SUB is in WAIT_SRV_CNF state */

#define CM_MT_CS_PAGE_REJECT_DEFAULT_DURATION 7

#define CM_GPP_WIFI_CELLULAR_SCAN_INVALID_TIMER 0xFFFF

#define CM_GPP_WIFI_CELLULAR_SCAN_DEFAULT_MIN_TIMER 5
#define CM_GPP_WIFI_CELLULAR_SCAN_DEFAULT_MAX_TIMER 20

#define VOPS_MASK_NONE          0
#define VOPS_RAT_LTE            0
#define VOPS_MASK_LTE           BM(VOPS_RAT_LTE)

/**--------------------------------------------------------------------------
** Enums
** --------------------------------------------------------------------------
*/

/* Enumeration of preferred system changed state when the hybrid operation
   is enabled.
*/
typedef enum cmph_pref_ss_state_e
{

  CMPH_PREF_SS_STATE_NONE=-1,    /* FOR INTERNAL USE OF CM ONLY! */

  CMPH_PREF_SS_STATE_NULL,            /* Null state */
  CMPH_PREF_SS_STATE_WAIT_FOR_1X_SRV, /* Wait for 1x to acquire service */
  CMPH_PREF_SS_STATE_HDR_ONLY,        /* send pref sys changed command
                                         to HDR only */
  CMPH_PREF_SS_STATE_1X_HDR,          /* pref sys has been sent to
                                         MC and HDRMC and hybrid opertion
                                         is on */

  CMPH_PREF_SS_STATE_MAX         /* FOR INTERNAL USE OF CM ONLY! */

} cmph_pref_ss_state_e_type;

/* Enumeration of phone's system selection preference when NO_CHANGE
   is specified
*/
typedef enum cmph_pref_nochg_e
{
  CMPH_PREF_NOCHG_MIN = -1,   /* FOR INTERNAL USE ONLY! */
  CMPH_PREF_NOCHG_MODE_PREF,
  CMPH_PREF_NOCHG_ACQ_ORDER_PREF,
  CMPH_PREF_NOCHG_BAND_PREF,
  CMPH_PREF_NOCHG_LTE_BAND_PREF,
  CMPH_PREF_NOCHG_TDS_BAND_PREF,
  CMPH_PREF_NOCHG_PRL_PREF,
  CMPH_PREF_NOCHG_ROAM_PREF,
  CMPH_PREF_NOCHG_HYBR_PREF,
  CMPH_PREF_NOCHG_SRV_DOMAIN_PREF,
  CMPH_PREF_NOCHG_NETWORK_SEL_MODE_PREF,
  CMPH_PREF_NOCHG_UE_USAGE_SETTING,
  CMPH_PREF_NOCHG_DDTM_SO_LST_ACT,
  CMPH_PREF_NOCHG_VOICE_DOMAIN_PREF,
  CMPH_PREF_NOCHG_MAX = 32 /* FOR INTERNAL USE ONLY! */
} cmph_pref_nochg_e_type;

#ifdef FEATURE_TDSCDMA
/* Enumeration for CMCC Special Test Setting Modes
*/
typedef enum cmph_spl_test_settings_e
{

  CMPH_SPL_SET_CONF_NONE = -1,     /* FOR INTERNAL USE ONLY! */
  /* Invalid Configuration Value */

  CMPH_SPL_SET_TEST_CONF,
  /*Software will by default configure all required NV settings for CMCC Test
  Any TDS specific NV configured by the user (OEM/QMC) will take effect
  OEMs can use this setting for � testing on specific boxes, change
  capabilities/configurations etc.*/


  CMPH_SPL_SET_LAB_CONF,
  /* Software will by default configure all required NV settings for CMCC Lab
   Any TDS specific NV configuration by the user (OEM/QMC) will NOT take effect � ignored
   OEMs should use this setting for �CMCC Lab Testing� */


  CMPH_SPL_SET_FIELD_DPLYMT_CONF,
  /* Software will by default configure all required NV settings for CMCC Field
   Any TDS specific NV configuration by the user (OEM/QMC) will NOT take effect � ignored
   OEMs should use this setting for �CMCC Field Testing/Deployment/Mass Production� */

  CMPH_SPL_SET_MAX_CONF

} cmph_spl_test_settings_e;
#endif

typedef enum cmph_subs_mcfg_state_e
{
  CMPH_MCFG_STATE_NONE,

  CMPH_MCFG_STATE_WAIT_FOR_DEACT,
  /* CM sent standby pref to MMOC and waiting for confirmation */

  CMPH_MCFG_STATE_NOTIFY_PENDING,
  /* CM received REFRESH confirmation from MMOC, but yet to notify MCFG */

  CMPH_MCFG_STATE_WAIT_FOR_NV_REFRESH,
  /* CM notified to MCFG for deact cnf and waiting for NV refresh from MCFG */

  CMPH_MCFG_STATE_WAIT_FOR_ACT,
  /* NV refresh done, CM is waiting for Sub to be activated from MCFG */

  CMPH_MCFG_STATE_MAX
} cmph_subs_mcfg_state_e_type;

/* Enumeration indicating the status during DDS switch for DSDS L+G  */
typedef enum cmph_dds_status_e
{

  CMPH_DDS_STATUS_NONE,
  CMPH_DDS_STATUS_PS_DETACH_PENDING,
  CMPH_DDS_STATUS_REMOVE_DATA_OBJ,
  CMPH_DDS_STATUS_MAX

} cmph_dds_status_e_type;

typedef enum cmph_sub_block_reason_e
{
  CMPH_BLOCK_REASON_NONE,

  CMPH_BLOCK_REASON_ILLEGAL_ME,
  /* NAS sent ILLEGAL ME, cause #06 on this subscription */
  
  CMPH_BLOCK_REASON_ILLEGAL_ME_DUE_TO_OTHER_SUB,
  /* CM blocked this subscription as NAS sent ILLEGAL ME, cause #06 on other subscription */
  
  CMPH_BLOCK_REASON_MAX
}cmph_sub_block_reason_e_type;

#define CMPH_VSID_PRIMARY   0x11C05000
#define CMPH_VSID_SECONDARY 0x11DC5000

/**--------------------------------------------------------------------------
** Datatypes
** --------------------------------------------------------------------------
*/

#define MAX_STACK_PER_SUB      3

/* This holds information regarding emergency call customization
 * Need to declare as PACKED since this structure is also stored in
 * CM-NV
 */
typedef PACKED struct PACKED_POST
{

  uint8                            version;

  boolean                          is_ecbm_required;
  /* Indicate whether UE should enter ECBM after emergency call connect.
  ** If TRUE , UE should remain in emergency mode after MO emergency call gets
  ** connected on any RAT. If FALSE, ECBM applies only to calls connected over CDMA.
  ** Default value is FALSE.
  */

  boolean                          hold_concurrent_voicecall_during_emergency;
  /* Indicate what should be done to voice calls in progress at the time emergency
  ** call is initiated. If TRUE, place calls on HOLD. If FALSE, release calls.
  ** Default value is FALSE. */


  boolean                          is_alternate_redial_algorithm;
  /* Indicate if alternate silent redial algorithm should be enabled.
  ** If this is disabled, CM will use standard redial behavior of disabling
  ** LTE after cs_fallback timer expires without data call setup, IMS/NAS hard
  ** failures etc..
  */

  boolean                          is_ral_stored_in_efs;
  /* indicates if 1. recently Acqed list has to be stored in EFS across
  ** operating modes, and power cycles. 2. band level granular system records
  ** for 3gpp based systems records have to be used for recently acqed lists.
  */

  boolean                          is_wlan_e911_call_supp;
  /* indicates if VzW WLAN calling is supported. Default value would be FALSE
  */

  uint16                           twwan_timer;
  /* Value for duration of Twwan-911 scan timer in seconds
  ** default value 40 seconds
  */

  uint8                           reserved[4];

} cmph_custom_emerg_call_info_s_type;



/* This holds information regarding mid call srvccc timer expiry
 * configuaration
 */
typedef PACKED struct PACKED_POST
{

  uint8                            version;

  uint32                           ims_cache_expiry_duration;
  /* IMS cache expiry timer */

  uint8                            mid_call_support;
  /* mid call feature support */

  boolean                          srvcc_sync_support;
  /* srvcc sync support */

  uint8                           reserved[1];
  /* Reserved Fields */


} cmph_mid_call_srvcc_info_s_type;

/* This holds information regarding custom wifi
 * configuarations
 */
typedef PACKED struct PACKED_POST
{

  uint8                   version;

  boolean                 is_wifi_only_mode_for_voice;
  /* if set to TRUE, indicates support of voice calls in wifi only mode */

  boolean                 is_e911_over_wifi;
  /* if set to TRUE indicates support of emergency calls over wifi and the
          corresponding silent redial requirements support.*/

  uint8                   wifi_cs_scan_timer;
  /* This field indicates the time for which call would be attempted
     on CS domain. If the call does not get an ok_to_orig in the above
     mentioned time, call would be re-attempted on wifi. Default value
     for this timer is 5sec */
  boolean                 wifi_e911_wwan_full_srv;
  /* this feature will trigger the scan sequence of
       LTE -> full srv -> limited srv - >WLAN
       */
  boolean                 volte_special_pref_setting;
  /* Handle requirement of VoLTE UE behavior when VoLTE is not preferred */
  uint8                           reserved[5];
  /* Reserved Fields */

} cmph_custom_vowifi_config_s_type;


/* This holds information if call end event is to be sent to clients
** in case of a AWIM CDMA call collision
*/
typedef PACKED struct PACKED_POST
{

  uint8                            version;

  uint8                            is_AWIM;
  /* Send call end event for MO call to clients */

  uint8                            reserved[2];
  /* Reserved Fields */


} cmph_cdma_call_collision_s_type;


/* Structure to store custom silent redial timer value in CM
*/
typedef PACKED struct PACKED_POST
{

  uint8                            version;

  boolean                          is_custom_voice_sr_timer_enabled;
  /* Boolean value to indicate if the custom voice call silent
  ** redial timer value is enabled
  */

  uint16                           voice_sr_timer_value;
  /* Value of the silent redial timer */

  uint8                            reserved[2];
  /* Reserved Fields */


} cmph_custom_voice_sr_timer_config_s_type;


/* This structure is used to indicate if stack suspend
** operation is pending and the current stack suspend status per stack
*/
typedef struct
{
  uint8                          num_requests;
  /**< Count is incremented whenever a request is sent to MMOC and
  ** decremented with each response
  */
  boolean                        is_suspend;
  /**< indicates current suspend status of a stack */

  boolean                        susp_due_to_emerg;
  /**< Being suspended due to emergency */

} cm_stack_suspend_info_s_type ;

/* This holds the information about get networks enabled for auto mode
 * Need to declare as PACKED since this structure is also stored in
 * CM-NV
 */

typedef struct
{  
    boolean                full_srv_added;
	sys_sys_mode_e_type			   mode;
	
	sys_lte_band_mask_e_type  lte_band;
	 

} cm_lte_full_srv_added_e_type;

typedef PACKED struct PACKED_POST
{

  uint8 version ;

  uint16 get_net_auto_mode;
  /* Indicate whether manual plmn selection is enabled
  ** during auto mode or not.
  */
  uint16 reserved;
} cmph_efs_get_net_auto_mode;

/* Store SS to subs/stack id mapping */
typedef struct cm_ph_stack_subs_info_s
{
  int                      stack_id;

} cm_ph_stack_subs_info_s_type;

/* EFS/NV csg mode support in auto mode*/
typedef enum
{
  /** @cond
  */
  CSG_SUPPORT_NONE = 0,
  /** No support for CSG in auto mode
  */

  CSG_MANUAL_AND_APP_SUPPORT = 1,
  /**< Manual and APP CSG support in auto mode. */

  CSG_MANUAL_SUPPORT = 2,
  /**< Manual CSG support in auto mode */

  CSG_APP_SUPPORT = 3,
  /**< CSG App support in auto mode */

  /** @cond
  */
  CSG_SUPPORT_MAX   /* FOR INTERNAL USE OF CM ONLY! */
  /** @endcond
  */

} cmph_csg_support_auto_mode_e_type;


/* This holds the information about CSG support enabled for auto mode
 * Need to declare as PACKED since this structure is also stored in
 * CM-NV
 */


typedef PACKED struct PACKED_POST
{

  uint8 version ;

  uint8 csg_global_mode_support;
  /* Indicate whether csg support is enabled or not for auto mode
   ** 0 -Disable CSG support for auto mode
   ** 1 - Enable support for manual CSG and CSG app
   ** 2 - Enable support for manual CSG
   ** 3 - Enable support for CSG App
   */
  uint8 disable_bsr_global_csg;
  /*  0 - Bsr is enabled
  **   1- Bsr is disabled during global csg support
  */

  uint16 reserved;
} cmph_efs_get_global_mode_csg;


/* Used to enable CS to PS(LTE) redial for E911 calls
 */

typedef PACKED struct PACKED_POST
{

  uint8 version ;

  boolean redial_on_lte;

  uint8                            cs_favoured_retry_count;
  /* applicable only for NW rejects reasons for which
    immeidate PS redial is not performed */

  uint8                            reserved[2];

} cmph_emerg_lte_redial;

/* This will be used to skip scanning of GWT rats when VOLTE call in place
   during LTE RLF
 */

typedef PACKED struct PACKED_POST
{

  uint8 version ;

  boolean skip_gwt_scan;

  uint8                            reserved[3];

} cmph_skip_gw_scan_during_rlf;

/* Dbg structure - store the info from initialization phase which is hard to capture in
** F3.
*/
typedef struct cmph_device_dbg_info
{

  boolean                mcfg_refresh;
  /* set to true if mcfg refresh is triggered */

  sys_overall_feature_t  init_feature_mode;

} cmph_device_dbg_info_s_type;

/* Dbg structure - store the info from initialization phase which is hard to capture in
** F3.
*/
typedef struct cmph_sub_dbg_info
{

  sd_ss_hybr_pref_e_type init_hybr_pref;
  /* initial hybr pref at power up */

  sys_ue_mode_e_type     init_ue_mode;
  /* initial ue mode at power up */

  sys_subs_feature_t     init_subs_feature_mode;
  /* power up sub feature mode */

} cmph_sub_dbg_info_s_type;

typedef enum
{
  CM_MAN_SRCH_STATE_NONE = -1,
  CM_MAN_SRCH_STATE_BUFFERED_CALL,
  CM_MAN_SRCH_STATE_BUFFERED_TIMER,
  CM_MAN_SRCH_STATE_MAX

} cm_man_srch_state_e_type;

/*******************************************************************************
     INCOMING_PAGE_F           INCOMING_PAGE_RSP               SUSPEND_F                   INCOMING_F
NONE------------------->DS_RSP-------------------->SUSPEND_RSP-------------->INCOMING_F----------------->NONE


*******************************************************************************/
#ifdef FEATURE_MMODE_SC_SVLTE
typedef enum
{
  CM_AWAITING_NONE,        /* when no 1x MT data page pending at CM/DS*/
  CM_AWAITING_DS_RSP,      /* when page has been indicated to DS and awaiting response*/
  CM_AWAITING_SUSPEND_RSP, /* when DS accepted page and CM suspending other stacks and waiting response*/
  CM_AWAITING_INCOMING_F   /* CM responded to 1x page with proceed and waiting for incoming call*/
} cm_awaiting_mt_page_rsp_e_type;
#endif

/* Structure to store Data prirority per sub */

typedef struct emerg_call_data_s
{

  cm_ph_state_e_type               ph_state;
  /* The current operating mode of the phone.
  ** Used to indicate emergency call back mode
  */

  cm_ecbm_exit_reason_e_type       ecbm_exit_reason;
  /* Indicates the reason for ph_state transition from EMERG_CB to NORMAL
  ** It can be due to origination of another emergency call or any other user trigger
  */

  sys_sys_mode_e_type              ecbm_rat;
  /* Indicates the RAT on which UE connects the emergency call
  */

  boolean                          is_ecbm_required;
  /* Indicate whether UE should enter ECBM after emergency call connect.
  ** If TRUE , UE should remain in emergency mode after MO emergency call gets
  ** connected on any RAT. If FALSE, ECBM applies only to calls connected over CDMA.
  ** Default value is FALSE.
  */

  boolean                          hold_concurrent_voicecall_during_emergency;
  /* Indicate what should be done to voice calls in progress at the time emergency
  ** call is initiated. If TRUE, place calls on HOLD. If FALSE, release calls.
  ** Default value is FALSE. */

  boolean                          is_alternate_redial_algorithm;
  /* Indicate if alternate silent redial algorithm should be enabled.
  ** If this is disabled, CM will use standard redial behavior of disabling
  ** LTE after cs_fallback timer expires without data call setup, IMS/NAS hard
  ** failures etc..
  */

  boolean                          is_wlan_e911_call_supp;
  /* indicates if WLAN E911 calling is supported.
  */

  uint16                           twwan_timer;
  /* Value for duration of Twwan-911 scan timer in seconds.
  ** after this timer expiry, CM needs to check if WLAN call si supported
  ** and if no WWAN system was found then route the call to WLAN
  */

  boolean                          is_allow_sms_in_ecbm_enabled;
  /* Support for allowing SMS in ECBM for Sprint */

  boolean                          redial_emerg_on_lte;
  /* used to enable CS to PS redial for
     e911 calls upon during NO SRV/LL rejects */

  uint8                            cs_favoured_redial_counter ;
  /* Specifies the Maximum attempt to be made on CS for e911 call
    before fallback to PS. Set to CMPH_MAX_CS_REDIAL_COUNT_FOR_EMERG
    if cs_favoured_retry_counter is not set from NV. */

#if defined (FEATURE_IP_CALL) && defined (FEATURE_CM_LTE)
  boolean                          wifi_e911_support;
  /* if set to TRUE indicates support of emergency calls on wifi */

  boolean                          emcall_to_voicecall;
  /* if set to TRUE indicates that emergency call should be
  ** converted to normal voice call and attempt it over wifi */

  boolean                            wifi_e911_wwan_full_srv;
  /* this feature will trigger the scan sequence of
     LTE -> full srv -> limited srv - >WLAN
     */
#endif

  boolean                          is_emergency_mode;

  boolean                        is_ral_stored_in_efs;
  /* indicates if 1. recently Acqed list has to be stored in EFS across
    ** operating modes, and power cycles. 2. band level granular system records
    ** for 3gpp based systems records have to be used for recently acqed lists.
  */

  boolean                         emerg_call_rej_in_acq_hold;
  
  boolean                   ecbm_rat_change_to_wlan;
  /* if set to TRUE indicates ECBM RAT changed to WLAN */

} emerg_call_data_s_type;

typedef struct sds_ipcall_volte_pref_s
{

#ifdef FEATURE_VOIP_E911_SUPPORT
  dword                            voip_cancel_retry_uptime;
  /* If this timer is running when 911 call is originated,the call will be
  ** attempted over VOIP. This timer is started when a 911 call is cancelled
  ** by the user while the origination has been sent to VOIP.
  */

  uint8                            voip_cancel_retry_period;
  /* Time period to use to set up the voip_cancel_retry_uptime timer
  ** Initialized from NV_VOIP_CANCEL_RETRY_TIMER_I.
  */

  uint8                            cs_to_voip_fallback_period;
  /* Time period to use to set up the cs_to_voip_fallback_uptime timer. This
  ** timer is maintained in the call object. When a 911 call is originated
  ** and voip is available , this timer is started. When the timer expires
  ** and the call has not yet succeeded over CS networks, the call is
  ** attempted over voip. Initialized from NV_CS_TO_VOIP_FALLBACK_TIMER_I
  */
#endif

  sys_proc_type_e_type             proc_type_req_sent_by_cm;
  /* update last proc_type sent during VOLTE call progress by CM */

  boolean                         ims_proc_type_sent_to_nas;
  /* update if last proc_type is informed to NAS */

  boolean                          cdma_srvcc_enabled;
  /* 1xSRVCC enabled from UE */

} sds_ipcall_volte_pref_s_type;

#ifdef FEATURE_HICPS_STACK_SYNC_UP
typedef struct hicps_pref_s
{

  boolean                          hicps_gwl_avoid_timer_active;
  /* Indicates whether the PS service request timer
  ** is active. */

  dword                            hicps_gwl_avoid_uptime;
  /* Indicate the uptime when a PS attach request is
  ** expected to be completed. */

  boolean                          is_hdr_activity;

} hicps_pref_s_type;
#endif

typedef struct ddtm_pref_s
{

  cm_ddtm_pref_e_type              ddtm_pref;
  /* data dedicated transmission mode preference */

  boolean                          cur_ddtm_status;
  /* Last DDTM status sent to 1xCP */

  boolean                          ddtm_sent_volte_stack2;
  /* Whether DDTM is sent during volte call on stack 2 to disable SMS and voice paging */

  sys_ddtm_act_mask_e_type         ddtm_act_mask;

  sys_ddtm_act_mask_e_type         cur_ddtm_act_mask;
  /* Last DDTM activity mask send to 1xCP.
  */

  cm_ddtm_so_list_act_e_type       ddtm_so_list_act;
  /* Specifies what to do ( add, replace, delete ) with the user
      ** specified SO list in the following parameters.
    */

  uint16                           ddtm_num_srv_opt;
  /* Number of SOs specified in list;
  */

  sys_srv_opt_type                 ddtm_srv_opt_list[SYS_DDTM_MAX_SO_LIST_SIZE];
  /* List of SOs for which 1x pages have to ignored while in
  ** HDR connected state. if srv_opt_list[] contains CAI_SO_NULL
  ** & ddtm_pref is ON, ignore all pages.
  */

  uint16                           cur_ddtm_num_srv_opt;
  /* Last DDTM ignore SO list count sent to 1xCP.
  */

  sys_srv_opt_type                 cur_ddtm_srv_opt_list[SYS_DDTM_MAX_SO_LIST_SIZE];
  /* Last DDTM ignore SO list sent to 1xCP.
  */

  uint16                           ddtm_num_allow_srv_opt;
  /* Number of SOs specified in allowed list;
  */

  sys_srv_opt_type                 ddtm_allow_srv_opt_list[SYS_DDTM_MAX_ALLOW_SO_LIST_SIZE];
  /* DDTM allow SO list sent to 1xCP.
  */

  uint16                           ddtm_num_blocked_srv_opt;
  /* Number service options blocked by DS during T_max_scan_time
  ** to allow LTE scan
  */

  sys_srv_opt_type                 ddtm_blocked_srv_opt_list[SYS_DDTM_MAX_SO_LIST_SIZE];
  /* List of service options being blocked by DS
  */

  boolean                          is_cdma_tx_disabled;
  /*
  ** Tells is CDMA Tx has been disabled using DDTM.
  */


} ddtm_pref_s_type;

typedef struct cm_ef_rat_s
{
  byte                       ens_value;

  boolean                    is_rat_bal;
  /* is_rat_bal: 'is RAT balancing on'
  ** is_rat_bal == (ens_value && rat balancing subfeature)
  ** Replaces FEATURE_UICC_RAT_INDICATOR_SUPPORT feature
  ** Determines whether we will perform RAT balancing
  */

  boolean                    is_uim_usim_rat_present;

  boolean                    is_mode_change_due_to_ef_rat;
  /* Flag to check if the mode pref chnage is due to
  ** EF_RAT refresh.
  */
} cm_ef_rat_s_type;

typedef struct sub_3gpp2_pref_s
{

  cm_cdma_lock_mode_e_type         cdma_lock_mode;
  /* Indicate whether phone is CMDA locked until power cycle */

  uint8                            cdma_hold_srv_timer;

#ifdef FEATURE_MMODE_SC_SVLTE
  cm_awaiting_mt_page_rsp_e_type   mt_1x_data_await_stage;
  /* Indicates CM wait stage upon receiving INCOMING_PAGE from 1xcp
  */
#endif // FEATURE_MMODE_SC_SVLTE

  boolean                          is_1x_mt_call_pending;
  /* Indicates if 1x MT call is active without any cmcall obj allocated for it.
  ** The flag is set when incoming page for voice call is received.
  ** The flag is reset:
  ** 1. when 1xCP reports CM_INCOMING_PAGE_FAIL_F.  (1XCP sends the report when call id
  **   is not allocated in CM but call fails in 1x internally.
  ** 2. when 1x reports CM_EXIT_TC_F.  (Sent if the failure happens after allocating call id and
  **   before incoming call notification).
  ** 3. when 1x reports CM_INCOMING_CALL_F.  (Sent when call id is allocated
  **   and NW sends AWIM OTA to 1x
  */

#ifdef FEATURE_CM_SS_MEAS
  #error code not present
#endif /*FEATURE_CM_SS_MEAS */

  cm_privacy_pref_e_type           privacy_pref;
  /* The privacy setting for calls, all calls share the privacy setting
  */
  boolean                          is_privacy;
  /* The state of the current pruvacy, this could be different than the
  ** privacy_setting
  */

  boolean                          is_spc_zero;
  /* Indicate whether the SPC is all zeros (i.e., "000000") */

  boolean                          is_otasp_commit_pending;
  /* After a succesfull OTASP commit nam change has to take place.
  ** This change is delayed till the Traffic channel gets torn down.
  ** CM_EXIT_TC_F gets sent on traffic channel getting torn down.
  **
  ** TRUE  - successfull OTASP commit occurred, send nam change after
  **         traffic is torn down.
  ** FALSE - No OTASP commit occurred so no need to send nam change on
  **         traffic channel being torn down.
  */

#if defined(FEATURE_UMTS_1X_HANDOVER_UMTSMSM) || \
      defined(FEATURE_UMTS_1X_HANDOVER_1XMSM)
  #error code not present
#endif

  boolean                          is_awim_mo_call_end_event;
  /* Flag to check if call end event is to be sent for MO calls when AWIM
  ** call collision happens */

  boolean                          lte_do_redir;

  cm_answer_voice_e_type           answer_voice;
  /* Indicate whether phone should answer incoming calls as
  ** voice, modem, or fax */

  dword                            answer_uptime;
  /* Indicate uptime for temporary answer-voice settings - i.e. the
  ** uptime when the a temporary answer-voice setting is expired and
  ** phone should restore the normal CM_ANSWER_VOICE_AS_VOICE
  ** setting */

  boolean                          is_hold_user_act;
  /* Indicates whether CM should process user action or not */

#ifdef FEATURE_HICPS_STACK_SYNC_UP
  hicps_pref_s_type                hicps_pref;
#endif

  ddtm_pref_s_type                 sub_ddtm_pref;

  cmxcall_ps_call_optimize_s_type  ps_call_optimized;

  boolean                          voice_drs_bit;
  /* DRS Bit value to be used for CDMA Voice calls */

  sd_ss_hybr_pref_e_type           int_hybr_pref;

} sub_3gpp2_pref_s_type;

typedef struct
{
  boolean        use_ue_based_sups;
  /* indicate if UE is configured as UE based SUPS will overide network based SUPS*/
  boolean        is_enabled;
  /* indicate if UE based sups is enabled or not */
  boolean        sync_ue_based_sups_with_cs;
  /*Enable syncing ue based sups with CS feature*/
}cm_ue_based_sups_config_s_type;

typedef PACKED struct PACKED_POST
{
  uint8  version;
  /* version */
  cm_ue_based_sups_config_s_type    config[CMEFS_UE_BASED_SUPS_ARRAY_SIZE];
  /*For configuring UE based supplementary mode*/
}cm_ue_based_sups_s_type;

typedef struct
{
  uint8     ue_based_sups_mode[CMEFS_UE_BASED_SUPS_ARRAY_SIZE];
  /*For configuring UE based supplementary mode*/
}cm_ue_based_sups_mode_s_type;

typedef struct
{
  boolean                          is_hvolte;
  /*hvolte feature control*/
  boolean                          is_ssac_hvolte;
  /* SSAC domain selection control.control if need to consider SSAC barring factor in domain selection */
  boolean                          volte_ssac_sr_control;
  /*SSAC silent redial control*/
  boolean                          volte_acb_sr_control;
  /* ACB silent redial control */
  uint16                           t_ssac;
  /* When UE moves from non-VA to VA, the device shall start a hysteresis timer tssac_hys.
  ** domain selection will not notify IMS voice preference until the expiry of tssac_hys.
  ** value 0 means timer is stopped
  */
  cm_ue_based_cw_s_type            ue_based_cw;
  /*config UE based call waiting feature*/

  boolean                          volte_sr_control;
  /* VOLTE Silent redial control flag for LTE level failure handling
  ** TRUE - disable LTE failure handling
  ** FALSE - enable LTE failure handling
  */

  boolean                 volte_special_pref_setting;
  /* Handle requirement of VoLTE UE behavior when VoLTE is not preferred */

  uint8                         lte_soft_fail_retry_count;
  /* This flag is read from NV lte_softfailure_count and it will not change during call.*/

  boolean                       e911_prioritize_ps;
    /* Enable/disable CM to route emergency calls over VOLTE as long as
    ** LTE network has e911 capability regardless of csfb availability
    */
    
  boolean                       is_volte_on;
  /* Indicates if AP has enabled VOLTE from UI or IMS has permenent permenent registration failure for 
  ** which volte can be recovered until power cycle or LPM-online.
  ** When is_volte_on = false for non DDS LTE only sub, LTE will be removed from rat capability.
  */
  boolean                       e911_pref_over_ps_during_wlan;
    /* Prefer PS for emergency call when the current voice domain is wifi
    */

  boolean                       is_380_emerg_attach_supp;
    /* Indicates, if limited camping(emergency attach) is allowed on PLMN where 380 is received
    */

  boolean                       is_volte_on_pending;
  /* When Volte is turned on and there is a PM policy change, this flag is set
  ** If stack 2 is in full service and needs to be turned off due to this capability change,
  ** notify IMS to do de-registration.
  */

  boolean                       is_ps_detach_comp_pending;
  /* This flag is set when PS detach trigger is sent to IMS due to stack 2 turning off.
  ** When stack 2 reports no service, PS detach complete needs to be sent to IMS
  ** so that IMS retry timer is stopped and will trigger re-registration once LTE service
  ** is reported on main stack.
  */
  cm_ue_based_sups_config_s_type            ue_based_sups[CMEFS_UE_BASED_SUPS_ARRAY_USED];
  /**UE based supplementary service configuration items */

  uint8     sups_mode[CMEFS_UE_BASED_SUPS_ARRAY_USED];
  /** For configuring UE based supplementary mode **/
} volte_sub_info_s_type;

typedef struct ps_pref_s
{

  cm_packet_state_e_type           packet_state;
  /* Indicate Packet State */

  boolean                          ps_service_req_timer_active;
  /* Indicates whether the PS service request timer
  ** is active. */

  dword                            ps_service_req_uptime;
  /* Indicate the uptime when a PS attach request is
  ** expected to be completed. */

  cm_ps_data_orig_prefs_e_type     ps_data_orig_pref;
  /* Indicates the preferences for ps data originations */

  dword                            ps_signaling_down_wait_time;

  boolean                          is_ps_srv_req_sent;
  /* ps_service_req_sent flag is used to decide if we need to start the
  ** PS_SIG_DOWN timer after PS service is acquired.
  */

  sys_data_priority_e_type         priority_type;
  /* DEPRECATED, was used to set DS priority */

  cm_lte_full_srv_added_e_type        e911_lte_full_srv_added;
  /* Indicates that SD has requested FULL LTE service during an E911 call even though
  ** LTE is not part of the idle capability
  */

  boolean                          is_tau_cap_update_enabled;
  /* Indicates if the TAU capability update feature is enabled
  ** If the feature is enabled, some capability updates will be notified to NAS with a TAU
  ** and some will be notified to NAS with a PS detach.
  ** If the feature is disabled, all capability updates will be notified to NAS with PS detach
  */

  boolean                          is_cap_update_required;
  /* Indicates if a capability update needs to be notfied to NAS due to a
  ** mode/band pref change. CM will either tell NAS to do a PS detach or
  ** a TAU depending on the flag is_tau_cap_update_required
  */

  boolean                          is_tau_cap_update_required;
  /* Indicates if a TAU needs to be sent to NAS for a capability update instead of
  ** a PS detach
  */

  boolean                          is_policy_chg_cap_update;
  /* Indicates if a capability update is due to a policy change, this is used
  ** to send a special detach type to NAS
  */

  boolean                          is_user_ps_detach_required;
  /* Indicates if PS detach MUST be performed due to a user action such as a srv domain
  ** preference change (PS_DETACH, CS_ONLY, etc). When this flag is set a PS detach
  ** will be sent to NAS (TAU will never be used).
  */

  boolean                          dereg_is_sent;
  /* Indicate if cm has notified IMS to do dereg for PS detach
  */

} ps_pref_s_type;

typedef struct csg_pref_s
{

  sys_csg_support_e_type           csg_support;
  /*Available CSG support */

  cmph_efs_get_global_mode_csg     csg_auto_mode;
  /* CSG global mode support */
} csg_pref_s_type;

typedef PACKED struct PACKED_POST
{

  uint8 version;

  boolean  is_cs_mt_page_concurrency_allowed;
  /* Indicate if CS MT page is allowed while there is a VoWLAN call on the
  ** other sub
  */

  boolean  is_ims_mt_page_concurrency_allowed;
  /* Indicate if IMS MT page is allowed while there is a VoWLAN call on the
  ** other sub
  */

  uint8 reserved[1];

}cmph_allow_mt_page_concurrency_s_type;

typedef struct
{
  boolean  is_cs_mt_page_concurrency_allowed;
  /* Indicate if CS MT page is allowed while there is a VoWLAN call on the
  ** other sub
  */

  boolean  is_ims_mt_page_concurrency_allowed;
  /* Indicate if IMS MT page is allowed while there is a VoWLAN call on the
  ** other sub
  */

}cmph_mt_page_concurrency_config_s_type;

typedef struct cmph_sub_info_s
{

  boolean                          is_in_use;
  /* Indicate whether phone is currently in use */

  boolean                          is_in_use_reported;
  /* Indicate whether phone is in use as per client's knowledge*/

  sys_subs_feature_t               subs_feature_mode;
  /* sub level feature mode for each sub */

  mmgsdi_slot_id_enum_type         valid_card;
  /* Indicates the SIM card 1 slot if valid (i.e. inserted and no errors) */

  mmgsdi_slot_id_enum_type         slot_id;
  /* Indicates the slot corresponding to this subscription */

  uint32                           rat_disabled_mask;
  /* Bit mask for disabled rats due to domain pref change */

  boolean                          is_3gpp2_subs_avail;
  /* Indicate whether the cdma subscription/provisioning information
  ** is available for consumption.                              */

  boolean                          is_3gpp_subs_avail;
  /* Indicate whether the GSM/WCDMA subscription/provisioning information
  ** is available for consumption.                              */

  boolean                          is_3gpp_subs_from_card_sent;

  boolean                          is_3gpp2_subs_from_card_sent;

  cm_rtre_config_e_type            rtre_config;
  /* Current RTRE configuration - RUIM, NV, RUIM fallback NV */

  cm_rtre_control_e_type           rtre_control;
  /* Current RTRE control - RUIM or NV */

  cm_pref_s_type                   pref_info;
  /* mode, band etc preferences of this stack */

  cm_pref_s_type                   prst_pref_info[NV_MAX_NAMS];

  boolean                          is_sys_sel_pref_rsp_pending;
  /* Flag = TRUE to indiacate that client is waiting for response
  ** after changing network sel mode pref
  */

  uint32                           hplmn_timer;
  /* the hplmn timer returned from NAS */

  cm_lte_disable_cause_e_type      lte_disable_cause;
  /* Reason for LTE disable */

  boolean                          is_prl_load_complete;
  /* Indicates if prl init is successful. */

  emerg_call_data_s_type           emerg_call_prop;
  /* Phone properties associated with emergency call and ECBM operations.*/

  sds_ipcall_volte_pref_s_type     ip_pref;

  sub_3gpp2_pref_s_type            sub_3gpp2_pref;

#ifdef CM_GW_SUPPORTED
  boolean                          is_gsm_amr_call_config_on;
  boolean                          is_umts_call_vt_codec_list_on;
#endif

  cm_operator_e_type               operator_name;

  /* Indicates WCDMA channel locking status
  ** Non-zero means lock to WCDMA channel
  */
  uint16                           wcdma_freq_lock;

  cm_acq_pri_order_pref_s_type     rat_acq_order_pref;
  /* Indicates rat acquisition priority order
  */

  boolean                          skip_gw_scan_during_rlf;
  /* flag to check if GWT scan should be skipped during RLF
     when VOLTE call is active */


  cm_lte_ue_mode_pref_e_type       lte_ue_mode_pref;
  /* Current UE mode of operation preference in LTE network */

#if defined (FEATURE_IP_CALL) && defined (FEATURE_CM_LTE)

  uint32                           ims_cache_expiry_duration;
  /* Maintains expiry timer for IMS cache message */

  boolean                          mid_call_support;


  boolean                          voice_in_wifi_only;
  /* if set to TRUE indicates that voice call are supported in WIFI only mode
     and the coressponding silent redial requirements are also supported*/

  uint8                            wifi_cs_scan_timer;
  /* This field indicates the time for which call would be attempted
     on CS domain. If the call does not get an ok_to_orig in the above
     mentioned time, call would be re-attempted on wifi. Default value
     for this timer is 5sec */

  cm_mt_cs_page_reject_param_s_type mt_cs_page_reject_param;

  dword                            mt_cs_page_reject_uptime;
  /* Indicates expected time till which time MT CS Page needs to be rejected
      due to ongoing MT IMS Call*/

#endif

  sys_ims_stack_enable_status_e_type active_ims_stack;

  boolean                          is_cdma_less_device;
  /* config item to tell if this is CDMA less device */

  volte_sub_info_s_type            volte_sub_info;
  /* volte info stored per subscription */

  ps_pref_s_type                   ps_pref;

  sys_sys_mode_mask_e_type         subs_capability;
  /* system mask to be sent to MCS based on subscription*/

  cm_disable_call_type             disable_call_type_mask;
  /* The bit mask for disabling the call types. The call type mask is
  ** defined in CM.H CM_CALL_TYPE_VOICE_MASK ...
  */

  boolean                          is_user_resel_buffered;
  /* Below boolean will be used when user reselection
  ** couldnot be forced due to hgh priority activity
  */

  boolean                          is_shorten_bsr;
  /* Flag to indicate when to force LTE scan. It is set when HDR srv lost
  ** reset when shorten_bsr_timer expired
  */

  dword                            shorten_bsr_timer;
  /* Timer to keep LTE scan non preempted
  */

  sys_plmn_list_status_e_type      available_networks_list_cnf;
  /* Gives the status of plmn list request (success/abort).
  */

  cm_ef_rat_s_type                 ef_rat;

#ifdef CM_GW_SUPPORTED
  cm_ph_charge_data_type           phone_charge;
  /* Phone charge details of this stack */

  sys_user_pref_plmn_list_s_type   user_pref_networks;
  /* The user preferred network list */

  sys_found_plmn_list_u_type          plmn_list;
  /* The available network list */

  boolean                          is_net_lists_present;
  /* This flag indicates if network lists info is available
  ** for clients.
  */

  cm_ccbs_indx_store_s_type        ccbs_indx_store[CM_MAX_CCBS_REQ];
  /* Stores list of ccbs indexes that user is waiting for a recall
  ** from the network
  */


#endif  /* (FEATURE_GSM) || (FEATURE_WCDMA)) */

#ifdef FEATURE_CM_LTE
  cm_plmn_blocking_info_s_type     plmn_blocking_info;
  /* Buffer the PLMN blocking command, and send the info to NAS in the
  ** next service request.
  */
  sys_aplmn_list_s_type            avoid_plmn_list_info;
#endif

  boolean                          is_pref_change_pending;
  /* flag to indicate whether the last pref change is pending. */

  boolean                     is_pref_chg_pending_stk2_volte;
  /* flag to indicate whether there is pref change pending due to volte call on stack 2 */

  cm_band_pref_e_type         pending_pref_old_band_pref;

  sys_lte_band_mask_e_type    pending_pref_old_lte_band_pref;

  cm_band_pref_e_type         pending_pref_old_tds_band_pref;

  sys_mcc_type                     gwl_mcc;
  /*Stores MCC of Primary GWL Subscription*/

  sys_mnc_type                     gwl_mnc;
  /*Stores MNC of Primary GWL Subscription*/

  sys_plmn_id_s_type               gwl_imsi_plmn;
  /* Store IMSI PLMN of GW subscription*/

  boolean                          self_activation_support;
  /* Configitem to tell if self activation is supported, and UE shall not switch to SRLTE
  mode till activation  of CSIM is completed */

  byte                             accolc;
  /* The access overload class information
  */

  /* Indicates whether the stacks are in suspend
  */
  cm_stack_suspend_info_s_type     ss_susp_info[MM_STACK_ALL];

  boolean                          is_mode_pref_none;
  /*Indicates whether mode_pref is NONE due to DAM/DS */

  boolean                          is_limited_srv_net_sel_mode_selected;
  /*Indicates whether user is forcing preferences for the limited service */

  cmph_sub_dbg_info_s_type         sub_dbg_info;

  cmph_subs_mcfg_state_e_type      mcfg_state;

  boolean  clear_mplmn;
  /* Clears MPLMN*/

  /* PS data activity ongoing (e.g. MMS) */
  boolean is_data_activity_ongoing;

    boolean force_data_activity_pref_ongoing;
  /* data activity pref are being forced */

  /* reject any PS activity */
  boolean reject_ps_activity;
  
  cmph_efs_get_net_auto_mode       get_net_auto_mode_enabled;
  /* Get Network in Automatic mode enable/disable */

  dword                            subsc_avail_send_time;
  /* Indicates expected time to send buffered subscription available command*/

  boolean is_enable_volte_stack2;
  /* Config item to tell if to stay on srlte in home and volte on stack 2 is enabled */

  cmph_sub_block_reason_e_type      block_reason;
  /* Indicates that subscription is blocked due to mentioned reason */

    sys_ehplmn_plmn_list_s_type      ehplmn_list;

  boolean                          is_sp_special_priority_mode_mms;
  /* Indicates if special TRM priority mapping is being used on this sub due to MMS */

  boolean                          is_sp_special_priority_mode_wifi;
  /* Indicates if special TRM priority mapping is being used on this sub due to Wifi calls */

  boolean                          ps_detach_in_progress;
  /* Indicates if ps detach is pending, set only during DDS switch on new DDS
   ** this is used not to allow data calls as they would be teared down*/

  boolean                          ip_call_end_rsp_pend;
  /* Flag to indicate if we need to lower TRM priority immediately after call end*/


  boolean                         modem_arfcn_list_info;
  /* Controls sending modem frequency info reports to AP
  */
   boolean                         user_ss_pref_in_full_rat;
  /* Flag that indicates whether User SS Pref are being enforced in FULL RAT */

  boolean                         refresh_vote_ok;
  /* Indicates if need to vote ok for mmgsdi refresh
  ** even when there is call
  */

  boolean                                 is_msim_esr_enabled_for_volte;
  /* Indicates if ESR is needed for LTE in VoLTE mode sharing RF with other RAT in msim */

  boolean                         sync_ue_based_cw_with_cs;
  /* Enable syncing ue based cw with CS feature
  */

  boolean                       immediate_sups_sync_with_cs_req;
    /* For immediate sups sync to CS network through CSFB */
  boolean                       is_ra_cap_update_fail_buffered;
  /* Indicates RA capability update timer expired but subsequent detach is buffered due to ongoing activity */

  csg_pref_s_type                  csg_pref;

  boolean                          skip_ps_sig_down_timer;
  /* Indicates if PS sig down timer should be skipped post dds switch */

  uint16                    gpp_wifi_cellular_scan_timer;
  /* This field indicates the time for which emergency call would be attempted
     on Cellular domain when the UE is attached over a WLAN access.When the 
     emerg-non3gpp timer expires, the UE shall consider that it has failed to 
     use 3GPP access to setup the emergency call and call would be re-attempted 
     over the available WLAN access.*/
     
  cm_srv_domain_pref_e_type user_srv_domain_pref_setting;
  /*Value set is NV by user*/
} cmph_sub_info_s_type;

typedef struct oprt_mode_data_s
{

  sys_oprt_mode_e_type             oprt_mode;
  /* Indicate current operating mode - online, offline, LPM, etc. */

  cm_rf_init_s_type       rf_init_info;
  /*  indicates Rf init status and error code. */

  boolean                          is_gps_pgi_init_complete;
  /* GPS complete status regardless of success or failed */

  dword                            oprt_mode_send_time;

  boolean                          is_pending_online;
  /*  Indicates that there is a pending online transition. */

  boolean                          is_oprt_mode_change_ongoing;
  /* Flag to indicate oprt_mode_change is ongoing */

  sys_oprt_mode_e_type             true_oprt_mode;
  /* Reflects true oprt_mode
     For Ex. it will be set to online, even before cnf is received */

  boolean                          is_wait_on_data_end;
  /* Indicates whether waiting for datacall end */

  nv_stat_enum_type                nv_ftm_status;
  /*below will help to control NV item status from the testcase */

  boolean                          moved_from_ftm;
  /**< this stores of UE moved from FTM to other oprt mode
   **  but not to online
   **  this is used , to reset NV-FTM when UE moves to online */

  boolean                          moved_from_lpm_to_online;
    /* Indicates whether UE moved from LPM to ONLINE */

  boolean                          is_online_once;
  /* Set to false at power up and set to TRUE when operation mode changes to online */

  gts_TimeType lpm_time;
  /* Timestamp when operation mode changes to LPM */

  gts_TimeType online_time;
  /* Timestamp when operation mode changes to online */

  cm_emerg_pending_info_s_type emerg_pending_info;
  /* Indicates if e911 call is pending with ONLINE mode change */

} oprt_mode_data_s_type;

typedef struct ph_trm_cap_s
{

  boolean                          is_svdo_supported;
  /* Indicates whether SVDO is supported
  */

  boolean                          is_svlte_supported;
  /* Indicates whether SVLTE is supported
  */

} ph_trm_cap_s_type;

typedef struct mode_band_cap_s
{

  sys_sys_mode_mask_e_type         mode_capability;

  sys_band_mask_e_type             band_capability;

  sys_lte_band_mask_e_type         lte_band_capability;

  sys_band_mask_e_type             tds_band_capability;

} mode_band_cap_s_type;

typedef struct line_switch_pref_s
{

  cm_cphs_proc_e_type              cphs_allowed;
  /*
  ** FEATURE_ALS
  ** Whether Common PCN Handset Specifications procedures are allowed
  */

  cm_als_proc_e_type               als_allowed;
  /*
  ** FEATURE_ALS
  ** Whether ALS procedures are allowed.
  ** cphs_allowed must be CM_CPHS_PROC_ALLOWED for als_allowed to
  ** be CM_ALS_PROC_ALLOWED
  */

  cm_als_line_e_type               line;
  /*
  ** FEATURE_ALS
  ** Currently selected line (all MO voice calls will use it)
  */

  cm_als_line_switching_e_type     line_switching;
  /*
  ** FEATURE_ALS
  ** Whether the user is allowed to switch between lines
  */

} line_switch_pref_s_type;

typedef struct sub_signal_strength_s
{

  uint8                            rssi_delta;
  /* RSSI change exceeds the delta threshold will be reported,
  ** default is 5dBm */

  uint8                            ecio_delta;
  /* ECIO change exceeds the delta threshold will be reported,
  ** default is 2dBm */

  uint8                            io_delta;
  /* IO  change exceeds the delta threshold will be reported,
  ** default is 5 dBm */

  uint8                            sir_delta;
  /* SIR  change exceeds the delta threshold will be reported,
  ** default is 3 dBm */

  uint8                            pathloss_delta;
  /* PATHLOSS  change exceeds the delta threshold will be reported,
  ** default is 5 dBm */

  uint8                            cqi_delta;
  /* CQI  change exceeds the delta threshold will be reported,
  ** default is 1 dBm */


  uint8                            rsrp_delta;
  /* RSRP  change exceeds the delta threshold will be reported,
  ** default is 5 dBm */

  uint8                            rsrq_delta;
  /* RSRQ  change exceeds the delta threshold will be reported,
  ** default is 5 dBm */


} sub_signal_strength_s_type;

typedef struct ph_msim_prop_s
{

  sys_modem_dual_standby_pref_e_type      standby_pref;
  /* Standby preference of the Phone */

  sys_modem_dual_standby_pref_e_type      internal_standby_pref;

  sys_modem_dual_standby_pref_e_type      new_standby_pref;
  /* Similar to internal_standby_pref, but updated as soon as the final standby_pref is
  ** calculated in standby_pref_proc_int
  ** Exposed to NAS to avoid a race condition, should not be used internally in MMODE
  */

  sys_modem_dual_standby_pref_e_type      user_standby_pref;
  /* Standby preference of the Phone as set by user/NV */

  boolean                                 is_bootup_no_standby;
  /* Boolean which indicates whether or not the NV 6876 dual_standby_pref
  * was set to NO_STANDBY at boot up. If true, any new dual_standby_pref
  * commands that change the standby pref will not update NV.
  */


  uint8                                   active_subs;
  /* The active subscriptions
  ** only valid when standby_pref is SYS_MODEM_DS_PREF_SINGLE_STANDBY or DUAL_STANDBY
  */

  sys_modem_as_id_e_type                  default_voice_subs;
  /* type of the system to avoid */

  sys_modem_as_id_e_type                  curr_voice_subs;
  /* subs which is currently preferred for voice */

  sys_modem_as_id_e_type                  default_data_subs;
  /* type of the system to avoid */

  sys_modem_as_id_e_type                  new_default_data_subs;
  /* new DDS sub, used for dual LTE when getting new IMS cap in msc */

  sys_modem_as_id_e_type                  priority_subs;
  /* type of the system to avoid */

  sys_modem_as_id_e_type                  curr_priority_subs;
  /* type of the system to avoid */

  cmph_dds_status_e_type                  dds_status;
  /* status during the DDS switch for DSDS L+G */

  boolean                          dds_switch_pending;
  /* set to TRUE when user PS detach is triggered on DDS sub */

  boolean                                 is_dds_change_on_svlte_sub;
  /* Indicates DDS is getting changed to SVLTE SUB */

  boolean                                 is_standby_pref_buffered;
  /* set to TRUE when standby_pref change is buffered*/

  cm_common_ds_pref_s_type                prst_dual_standby_pref;

  boolean                                 is_mmoc_standby_pref_not_req;
  /* Flag indicating whether MMOC should not receive standby pref chg */

  cm_dds_cause_e_type                     dds_switch_type;
  /*DDS Switch Type*/

  boolean                                 is_dds_switch_type_temp;
    /* Flag indicating whether the current DDS switch being processed
    ** is following a temporary switch (for MMS or
    ** other temporary reason)or a temporary DDS switch */

  boolean                                 is_dual_lte_dds_switch;
    /* For DDS switch in dual-LTE config with no max cap change, 
    ** both subs should start with acq db scan. */

  boolean                                 is_non_dds_in_fallback;
    /* If the non-DDS is in fallback state, send service request to NAS
    ** indicating the same so that NAS will start PLMN selection with
    ** the highest priority RAT in the RAT priority list instead of the
    ** last camped RAT (RPLMN). */

  boolean                                 is_msim_esr_enabled;
  /* Indicates if ESR is needed for LTE sharing RF with other RAT in msim */

  uint8                                   abort_sub_bitmask;

  uint16                                  mode_disabled_in_TSTS;
  /* modes to be diabled from multimode sub in
  ** case we move from DSDS to TSTS and vice versa */

  boolean                                 is_dual_lte;
  /* indicate if the configuration is dual lte with dual data and dual ims support */

  sys_modem_as_id_e_type                  sub_on_hold;

  boolean                                 online_hold_acq;
  /* Indicate if sub_on_hold is due to mmoc online transaction */

  cmph_mt_page_concurrency_config_s_type  mt_page_concurrency_config;
  /* Indicates if MT page should be allowed while there is a VoWLAN call on the
  ** other sub
  */
} ph_msim_prop_s_type;

typedef struct cmph_dev_prop_s
{

  cm_init_mask                     init_mask;
  /* Indicate whether this object was properly initialized */

  oprt_mode_data_s_type            ph_oprt_mode;
  /* All the data related to the operating mode of the device. */


  cm_nam_e_type                    nam_sel;
  /* Indicate the NAM selection */

  cm_nam_e_type                    curr_nam;
  /* Indicate the current NAM -
  ** needed when NAM selection is set to AUTO-NAM */

  sys_overall_feature_t            feature_mode;
  /* Indicate the feature mode configuration: svlte/sglte/multi-sim
  */

  ph_msim_prop_s_type              msim_prop;
  /* MSIM related properties of the device such as standby preference, DDS */

  boolean                          is_event_process;
  /* This flag controls if more details PH info
  ** should be copied or not. This flag restricts that client
  ** can request more PH info only while in the PH_EVENT callback
  ** processing.
  */

  cm_test_control_type_e_type      test_control_type;
  /* Indicate what type of test control test is */

  cm_reply_s_type                  reply;
  /* CM reply object to handles the details that are associated
  ** with waiting for a reply from MC */

  cmph_device_dbg_info_s_type      device_dbg_info;

  boolean                          wd_switch_on;
  /* Wireless disable switch info */

  boolean                          is_buffer_sys_sel_pref;
  /* Indicates if sys_sel_pref command can be buffered based on the
  ** value read from NV */

  sp_subscription_call_state_info  call_state_info;
  /* Call state on each sub
  */

  boolean                          qmss_enabled;

  boolean                          is_tau_needed;

#ifdef FEATURE_CRAT_MSIM_TEST_FRAMEWORK
  boolean                          is_lte_resume_needed;
  boolean                          is_lte_suspend_cnf_received;
#endif

  ph_trm_cap_s_type                trm_cap;

  mode_band_cap_s_type             hw_capability;
  /* Hardware capability of the device. */

  line_switch_pref_s_type          als_pref;

  sub_signal_strength_s_type       rssi_delta_pref;

  sys_drx_cn_coefficient_s1_e_type drx_coefficient;
  /* The DRX value NAS cnf for GET_DRX_REQ
  */

  boolean                          set_drx_result;
  /* The result NAS cnf for SET_DRX_REQ
  */

  boolean                          lu_reject_auto_enabled;

  mm_sub_stk_id_s_type             mm_id_for_get_networks;
  /* store the mm_id performing get network */

  sys_periodic_frequency_scan_info_s_type *pf_scan_info;

  sys_incremental_results_s_type   incremental_results;

  uint16                           max_search_time;

  uint32                           internal_manual_search_timer;

  uint64                           internal_manual_search_timer_uptime;

  cm_man_srch_state_e_type         man_srch_state;

  uint64                           man_srch_timer_uptime;

  /*To keep track of back to back calls while MPLMN scan is ongoing on other SUB*/
  uint64              man_srch_max_timer_uptime;

  uint8                            man_srch_retries;

  /* Stores QRARB restart id received during QLINK error recovery
  */
  uint8                            qrarb_restart_id;

  byte                             itc_aux_val_to_use;
  /* Indicates ITC Aux value to use
  */

  boolean                          ui_ready_status;
  /* indicates UI status if it is up to accept MT calls
  */
  uint64       ui_not_ready_timer;

  boolean                          mm_nv_conn_mode_manual_search;
  /* NAS conn mode during manual search efs*/

  uint8                            ps_signaling_max_wait_timer;
  /* PS signaling max wait timer*/

  uint8                            ps_call_orig_hold_timer;
  /* Maximum time to hold PS call orig if other sub is waiting for srv cnf */

  uint8 ims_dereg_subs;
  /* Bitmask of subs which need IMS dereg */

  uint8 is_new_pm_subs_cap_checked;
  /* Indicates CM has queried PM for subs with cap change
  ** which need to be IMS deregistered */

  sys_disable_emerg_call_end_no_sim_mcc_list_s_type disable_emerg_call_end_no_sim_list;
  /* List of MCCs for which emerg call should not be ended without redial when
   ** received end cause #5 from NAS */

  boolean      is_emerg_call_rej_no_sim;
  /*Indicates if emerg call should be ended without redial when received end cause #5 from NAS*/

  boolean is_msim_csg_enable;
  /* is csg supported in msim */
} cmph_dev_prop_s_type;
/* Phone type - holds all the information that is associated
** with a phone.
*/
typedef struct cmph_s
{
  cmph_dev_prop_s_type             device_prop;
  /* cmph device level info */

  cmph_sub_info_s_type            *sub_info[MAX_SIMS];
  /* cmph info stored per subscription */

} cmph_s_type;

/* Structure to store the HICPS GWL avoidance parameters locally. */
typedef struct
{
  uint32  cmph_gwl_avoid_enable;      /* Valid values: 0 & 1 */
  /* Default value 1     */
  uint32  cmph_gwl_avoid_timer_value; /* Default value 180s  */
  uint32  cmph_gwl_avoid_exten_value; /* Default value 60s   */
} cm_gwl_avoidance_timers_s_type;


/**--------------------------------------------------------------------------
** Macros
** --------------------------------------------------------------------------
*/

/* Macro to return a pointer to the persistent mode preference
** of the current NAM.
**
** NOTE! Phone object must have already been initialized with
** cmph_init().
*/

/* Macro to return a pointer to the mode preference of requested asubs_id */
//#define IS_VALID_PH_SUB_PTR(asubs_id) \
//                   (cmutil_is_as_id_valid(asubs_id) && cmph_ptr()->sub_info[(asubs_id)] != NULL )

cm_disable_call_type CMPH_SS_DISABLE_CALL_TYPE_MASK(
  sys_modem_as_id_e_type asubs_id,
  const cmph_s_type *ph_ptr
);

cm_pref_term_e_type CMPH_SS_PREF_TERM(
  sys_modem_as_id_e_type asubs_id,
  const cmph_s_type *ph_ptr
);

#define CMPH_SS_NV_CONTEXT( asubs_id, ph_ptr )\
    (cmutil_is_as_id_valid(asubs_id) ? (uint16)asubs_id: 0)

#ifdef CM_GW_SUPPORTED
cm_ph_charge_data_type CMPH_SS_PHONE_CHARGE(
  sys_modem_as_id_e_type asubs_id,
  const cmph_s_type *ph_ptr
);
#endif

#define ACTIVE_MASK_TO_AS_ID( asubs_id_mask ) \
                         ( (asubs_id_mask == SYS_MODEM_AS_ID_3_MASK)? \
                                SYS_MODEM_AS_ID_3:\
         ( (asubs_id_mask != SYS_MODEM_AS_ID_2_MASK)? \
                                        SYS_MODEM_AS_ID_1: \
                                        SYS_MODEM_AS_ID_2 \
                     ))

/* Macro to return a pointer to the hybr preference of requested ss */
cm_hybr_pref_e_type CMPH_SS_HYBR_PREF(
  sys_modem_as_id_e_type asubs_id,
  const cmph_s_type *ph_ptr
);

/* Macro to return a pointer to the persistent hybr preference
** of the current NAM.
*/
#define CMPH_SET_PRST_HYBR_PREF( asubs_id, ph_ptr, val ) \
          if (cmutil_is_as_id_valid(asubs_id)) {\
            ph_ptr->sub_info[asubs_id]->prst_pref_info[ ph_ptr->device_prop.curr_nam ].hybr_pref = val; }


#define CMPH_PRST_SS_HYBR_PREF( asubs_id, ph_ptr ) \
                ( cmutil_is_as_id_valid(asubs_id) ?\
                  ph_ptr->sub_info[asubs_id]->prst_pref_info[ ph_ptr->device_prop.curr_nam ].hybr_pref:\
ph_ptr->sub_info[SYS_MODEM_AS_ID_1]->prst_pref_info[ ph_ptr->device_prop.curr_nam ].hybr_pref\
                )

/* Macro to return a pointer to the mode preference of requested ss */
cm_mode_pref_e_type CMPH_SS_MODE_PREF(
  sys_modem_as_id_e_type asubs_id,
  const cmph_s_type *ph_ptr
);

/* Macro to return a pointer to the mode preference of requested ss */
cm_mode_pref_e_type CMPH_SS_USER_MODE_PREF(
  sys_modem_as_id_e_type asubs_id,
  const cmph_s_type *ph_ptr
);

cm_mode_pref_e_type CMPH_PRST_SS_MODE_PREF(
  sys_modem_as_id_e_type asubs_id,
  const cmph_s_type *ph_ptr
);

cm_band_pref_e_type  CMPH_PRST_SS_BAND_PREF(
  sys_modem_as_id_e_type asubs_id,
  const cmph_s_type *ph_ptr
);

sys_lte_band_mask_e_type  CMPH_PRST_SS_LTE_BAND_PREF(
  sys_modem_as_id_e_type asubs_id,
  const cmph_s_type *ph_ptr
);

cm_band_pref_e_type  CMPH_PRST_SS_TDS_BAND_PREF(
  sys_modem_as_id_e_type asubs_id,
  const cmph_s_type *ph_ptr
);

/* Macro to return a pointer to the persistent mode preference
** of the current NAM.
*/
#define CMPH_SET_PRST_MODE_PREF( asubs_id, ph_ptr, val ) \
            if (cmutil_is_as_id_valid(asubs_id)) {\
                        ph_ptr->sub_info[asubs_id]->prst_pref_info[ ph_ptr->device_prop.curr_nam ].mode_pref = val; }

#define CMPH_PRST_USER_NETWORK_SEL_MODE_PREF( asubs_id, ph_ptr ) \
                ( cmutil_is_as_id_valid(asubs_id)?\
                  ph_ptr->sub_info[asubs_id]->prst_pref_info[ ph_ptr->device_prop.curr_nam ].user_net_sel_mode:\
ph_ptr->sub_info[SYS_MODEM_AS_ID_1]->prst_pref_info[ ph_ptr->device_prop.curr_nam ].user_net_sel_mode\
                )


/* Macro to return a pointer to the band preference of requested ss */
cm_band_pref_e_type CMPH_SS_BAND_PREF(
  sys_modem_as_id_e_type asubs_id,
  const cmph_s_type *ph_ptr
);

#define CMPH_SS_LTE_BAND_PREF( asubs_id, ph_ptr ) cmph_get_ss_lte_band_pref( asubs_id, ph_ptr )

/* Macro to return a pointer to the mode preference of requested ss */
cm_band_pref_e_type CMPH_SS_TDS_BAND_PREF(
  sys_modem_as_id_e_type asubs_id,
  const cmph_s_type *ph_ptr
);

/* Macro to return a pointer to the persistent band preference
** of the current NAM.
*/
#define CMPH_SET_PRST_BAND_PREF( asubs_id, ph_ptr, val ) \
          if (cmutil_is_as_id_valid(asubs_id) ) {\
                        ph_ptr->sub_info[asubs_id]->prst_pref_info[ ph_ptr->device_prop.curr_nam ].band_pref = val; }

#define CMPH_SET_PRST_LTE_BAND_PREF( asubs_id, ph_ptr, val ) \
          if (cmutil_is_as_id_valid(asubs_id) ) {\
                        ph_ptr->sub_info[asubs_id]->prst_pref_info[ ph_ptr->device_prop.curr_nam ].lte_band_pref = val; }

#define CMPH_SET_PRST_TDS_BAND_PREF( asubs_id, ph_ptr, val ) \
          if (cmutil_is_as_id_valid(asubs_id) ) {\
                        ph_ptr->sub_info[asubs_id]->prst_pref_info[ ph_ptr->device_prop.curr_nam ].tds_band_pref = val; }

/* Macro to return the voice domain preference of requested asubs_id */
sys_ue_usage_setting_e_type CMPH_SS_UE_USAGE_SETTING(
  sys_modem_as_id_e_type asubs_id,
  const cmph_s_type *ph_ptr
);

/* Macro to return the voice domain preference of requested asubs_id */
sys_voice_domain_pref_e_type CMPH_SS_VOICE_DOMAIN_PREF(
  sys_modem_as_id_e_type asubs_id,
  const cmph_s_type *ph_ptr
);

/* Macro to return a pointer to the voice domain preference of requested asubs_id */
#define CMPH_SS_VOICE_DOMAIN_PREF_PTR( asubs_id, ph_ptr ) \
              (cmutil_is_as_id_valid(asubs_id)?\
                  &(ph_ptr->sub_info[asubs_id]->pref_info.voice_domain_pref):\
                   &(ph_ptr->sub_info[SYS_MODEM_AS_ID_1]->pref_info.voice_domain_pref)\
                     )

#define CMPH_PRST_VOICE_DOMAIN_PREF( asubs_id, ph_ptr ) \
              (cmutil_is_as_id_valid(asubs_id)?\
               ph_ptr->sub_info[asubs_id]->prst_pref_info[ ph_ptr->device_prop.curr_nam ].voice_domain_pref:\
               ph_ptr->sub_info[SYS_MODEM_AS_ID_1]->prst_pref_info[ ph_ptr->device_prop.curr_nam ].voice_domain_pref\
               )

/* Macro to return a pointer to the persistent VOICE DOMAIN PREF of the current NAM.
*/
#define CMPH_PRST_VOICE_DOMAIN_PREF_PTR( asubs_id, ph_ptr ) \
              (cmutil_is_as_id_valid(asubs_id)?\
                  &(ph_ptr->sub_info[asubs_id]->prst_pref_info[ ph_ptr->device_prop.curr_nam ].voice_domain_pref):\
                  &(ph_ptr->sub_info[SYS_MODEM_AS_ID_1]->prst_pref_info[ ph_ptr->device_prop.curr_nam ].voice_domain_pref)\
               )

/* Macro to return a pointer to the persistent VOICE DOMAIN PREF of the requested NAM.
*/
#define CMPH_PRST_VOICE_DOMAIN_PREF_NAM_PTR( nam, asubs_id, ph_ptr ) \
              (cmutil_is_as_id_valid(asubs_id)?\
                  &(ph_ptr->sub_info[asubs_id]->prst_pref_info[ nam ].voice_domain_pref):\
                   &(ph_ptr->sub_info[SYS_MODEM_AS_ID_1]->prst_pref_info[ nam ].voice_domain_pref)\
                     )

cm_prl_pref_e_type CMPH_SS_PRL_PREF(
  sys_modem_as_id_e_type asubs_id,
  const cmph_s_type *ph_ptr
);

/* Macro to return a pointer to the persistent prl preference
** of the current NAM.
*/
#define CMPH_SET_PRST_PRL_PREF( asubs_id, ph_ptr, val ) \
      if (cmutil_is_as_id_valid(asubs_id)) {\
        ph_ptr->sub_info[asubs_id]->prst_pref_info[ ph_ptr->device_prop.curr_nam ].prl_pref = val; }


#define CMPH_PRST_SS_PRL_PREF( asubs_id, ph_ptr ) \
               (cmutil_is_as_id_valid(asubs_id)?\
                ph_ptr->sub_info[asubs_id]->prst_pref_info[ ph_ptr->device_prop.curr_nam ].prl_pref:\
              ph_ptr->sub_info[SYS_MODEM_AS_ID_1]->prst_pref_info[ ph_ptr->device_prop.curr_nam ].prl_pref\
               )

/* Macro to return a pointer to the mode preference of requested ss */
cm_roam_pref_e_type CMPH_SS_ROAM_PREF(
  sys_modem_as_id_e_type asubs_id,
  const cmph_s_type *ph_ptr
);

/* Macro to return a pointer to the persistent roam preference
** of the current NAM.
*/
#define CMPH_SET_PRST_ROAM_PREF( asubs_id, ph_ptr, val ) \
          if (cmutil_is_as_id_valid(asubs_id) ) {\
            ph_ptr->sub_info[asubs_id]->prst_pref_info[ ph_ptr->device_prop.curr_nam ].roam_pref = val; }


#define CMPH_PRST_SS_ROAM_PREF( asubs_id, ph_ptr ) \
                 (cmutil_is_as_id_valid(asubs_id) ?\
                  ph_ptr->sub_info[asubs_id]->prst_pref_info[ ph_ptr->device_prop.curr_nam ].roam_pref:\
                  ph_ptr->sub_info[SYS_MODEM_AS_ID_1]->prst_pref_info[ ph_ptr->device_prop.curr_nam ].roam_pref\
               )

/* Macro to return a pointer to the mode preference of requested asubs_id */
cm_gw_acq_order_pref_e_type CMPH_SS_ACQ_ORDER_PREF(
  sys_modem_as_id_e_type asubs_id,
  const cmph_s_type *ph_ptr
);

sys_plmn_id_s_type CMPH_SS_PLMN_PREF(
  sys_modem_as_id_e_type asubs_id,
  const cmph_s_type *ph_ptr
);

/* Macro to return a pointer to the persistent acquisition order preference
** of the current NAM.
*/

#define CMPH_SET_PRST_ACQ_ORDER_PREF( asubs_id, ph_ptr, val ) \
          if (cmutil_is_as_id_valid(asubs_id)) {\
            ph_ptr->sub_info[asubs_id]->prst_pref_info[ ph_ptr->device_prop.curr_nam ].acq_order_pref = val; }


#define CMPH_PRST_SS_ACQ_ORDER_PREF( asubs_id, ph_ptr ) \
            (cmutil_is_as_id_valid(asubs_id)?\
                ph_ptr->sub_info[asubs_id]->prst_pref_info[ ph_ptr->device_prop.curr_nam ].acq_order_pref:\
              ph_ptr->sub_info[SYS_MODEM_AS_ID_1]->prst_pref_info[ ph_ptr->device_prop.curr_nam ].acq_order_pref\
              )

/* Macro to return a pointer to the mode preference of requested ss */
cm_network_sel_mode_pref_e_type CMPH_SS_NETWORK_SEL_MODE_PREF(
  sys_modem_as_id_e_type asubs_id,
  const cmph_s_type *ph_ptr
);

/* Macro to return a pointer to the persistent network selection mode preference
** of the current NAM.
*/
#define CMPH_SET_PRST_NETWORK_SEL_MODE_PREF( asubs_id, ph_ptr, val ) \
        if (cmutil_is_as_id_valid(asubs_id)) {\
          ph_ptr->sub_info[asubs_id]->prst_pref_info[ ph_ptr->device_prop.curr_nam ].network_sel_mode_pref = val; }

#define CMPH_PRST_SS_NETWORK_SEL_MODE_PREF( asubs_id, ph_ptr ) \
          (cmutil_is_as_id_valid(asubs_id) ?\
             ph_ptr->sub_info[asubs_id]->prst_pref_info[ ph_ptr->device_prop.curr_nam ].network_sel_mode_pref:\
             ph_ptr->sub_info[SYS_MODEM_AS_ID_1]->prst_pref_info[ ph_ptr->device_prop.curr_nam ].network_sel_mode_pref\
             )

/* Macro to return a pointer to the mode preference of requested asubs_id */
cm_srv_domain_pref_e_type CMPH_SS_SRV_DOMAIN_PREF(
  sys_modem_as_id_e_type asubs_id,
  const cmph_s_type *ph_ptr
);

/* Macro to return a pointer to the persistent service domain preference
** of the current NAM.
*/
#define CMPH_SET_PRST_SRV_DOMAIN_PREF( asubs_id, ph_ptr, val ) \
          if (cmutil_is_as_id_valid(asubs_id)) {\
            ph_ptr->sub_info[asubs_id]->prst_pref_info[ ph_ptr->device_prop.curr_nam ].srv_domain_pref = val; }

#define CMPH_PRST_SS_SRV_DOMAIN_PREF( asubs_id, ph_ptr ) \
          (cmutil_is_as_id_valid(asubs_id)? \
             ph_ptr->sub_info[asubs_id]->prst_pref_info[ ph_ptr->device_prop.curr_nam ].srv_domain_pref:\
             ph_ptr->sub_info[SYS_MODEM_AS_ID_1]->prst_pref_info[ ph_ptr->device_prop.curr_nam ].srv_domain_pref\
           )

#define cmph_map_sd_mode_pref_to_sys_mode_mask(x) sys_map_sd_mode_pref_to_sys_mode_mask(x)

/**--------------------------------------------------------------------------
** Functions - common
** --------------------------------------------------------------------------
*/

/*===========================================================================

FUNCTION is_contain_3GPP2_mode_pref

DESCRIPTION
 Utility function which checks if the given mode preference contains any 3GPP2
 mode preferences.

DEPENDENCIES
 cmph_init has been called.

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
boolean is_contain_3GPP2_mode_pref(
  cm_mode_pref_e_type  mode_pref
);

sys_overall_feature_t  cmph_get_feature_mode (void);

/*===========================================================================

FUNCTION  cmph_get_subs_feature_mode

DESCRIPTION
 Get the current feature mode configuration stored in phone object.

DEPENDENCIES
 None

RETURN VALUE
  feature_mode

SIDE EFFECTS
  None

===========================================================================*/
sys_subs_feature_t  cmph_get_subs_feature_mode
(
  sys_modem_as_id_e_type  asubs_id
);
boolean  cmph_is_feature_mode_1x_sxlte (void);
boolean  cmph_is_feature_mode_msim(void);

boolean cmph_is_subs_feature_mode_svlte (
  sys_modem_as_id_e_type          asubs_id
);

boolean cmph_is_subs_feature_mode_srlte (
  sys_modem_as_id_e_type          asubs_id
);
boolean cmph_is_subs_feature_mode_1x_sxlte (
  sys_modem_as_id_e_type          asubs_id
);

boolean cmph_is_subs_feature_mode_normal (
  sys_modem_as_id_e_type          asubs_id
);

/*===========================================================================

FUNCTION  cmph_determine_stk_per_rat

DESCRIPTION
 The function determines cm_ss base on asubs_id and rat.

DEPENDENCIES
 none

RETURN VALUE
  multimode_stack_e_type.

SIDE EFFECTS
  none

===========================================================================*/
multimode_stack_e_type  cmph_determine_stk_per_rat (
  sys_modem_as_id_e_type  asubs_id,
  sys_sys_mode_e_type     rat
);

multimode_stack_e_type  cmph_determine_ss_per_domain (
  sys_modem_as_id_e_type  asubs_id,
  sys_srv_domain_e_type     domain
);
sys_modem_as_id_e_type   cmph_determine_as_id_to_ll_per_domain (
  sys_modem_as_id_e_type  asubs_id,
  sys_srv_domain_e_type     domain
);
sys_modem_as_id_e_type cmph_get_as_id_from_ll_ind (
  sys_modem_as_id_e_type  asubs_id
);

/*==========================================================================

FUNCTION cmph_is_subsc_cmd_proccessing_in_progress

DESCRIPTION
  Indicates if there is ongoing GWL subscription command processing in progress

RETURN VALUE
  TRUE: if there is ongoing GWL subscription command processing in progress
  FALSE: if there is no ongoing GWL subscription command processing

DEPENDENCIES
  None

==========================================================================*/
boolean  cmph_is_subsc_cmd_proccessing_in_progress( void );

/*===========================================================================

FUNCTION cmph_rat_disabled_mask_contain

DESCRIPTION
  Check if the rat disabled mask contains the specified rat.

DEPENDENCIES
  Phone object must have already been initialized with cmph_init().

RETURN VALUE
 TRUE/FALSE

SIDE EFFECTS
  none

===========================================================================*/
boolean  cmph_rat_disabled_mask_contain(
  sys_sys_mode_e_type    sys_mode,
  sys_modem_as_id_e_type as_id
);

/*===========================================================================

FUNCTION cmph_rat_disabled_mask_add

DESCRIPTION
  Add the rat to the rat disabled mask.

DEPENDENCIES
  Phone object must have already been initialized with cmph_init().

RETURN VALUE
 none

SIDE EFFECTS
  none

===========================================================================*/
void  cmph_rat_disabled_mask_add(
  sys_sys_mode_e_type     sys_mode,
  sys_modem_as_id_e_type  as_id
);

/*===========================================================================

FUNCTION cmph_rat_disabled_mask_delete

DESCRIPTION
  Delete the rat from the rat disabled mask.

DEPENDENCIES
  Phone object must have already been initialized with cmph_init().

RETURN VALUE
 none

SIDE EFFECTS
  none

===========================================================================*/
void  cmph_rat_disabled_mask_delete(
  sys_sys_mode_e_type    sys_mode,
  sys_modem_as_id_e_type as_id
);

/*==========================================================================

FUNCTION cmph_get_received_card_init_completed_evt

DESCRIPTION
  The subcription handling in CM start listening the MMGSDI events only
  after receiving MMGSDI_CARD_INIT_COMPLETED_EVT from MMGSDI.
  As CM receives MMGSDI_CARD_INIT_COMPLETED_EVT it
  update the static variable cmph_received_card_init_completed_evt
  to TRUE. On some events (like refresh start) CM need to update the
  static variable cmph_received_card_init_completed_evt  to FALSE. By
  this function we can know what is the current status of
  cmph_received_card_init_completed_evt.

RETURN VALUE
  TRUE: if the present status of card intilization completed is TRUE
  FALSE: if the present status of card intilization completed is FALSE

DEPENDENCIES
  None

==========================================================================*/
boolean  cmph_get_received_card_init_completed_evt(sys_modem_as_id_e_type asubs_id );

boolean*  cmph_get_addr_received_card_init_completed_evt( sys_modem_as_id_e_type asubs_id );

/*==========================================================================

FUNCTION cmph_set_received_card_init_completed_evt

DESCRIPTION
  The subcription handling in CM start listening the MMGSDI events only
  after receiving MMGSDI_CARD_INIT_COMPLETED_EVT from MMGSDI.
  As CM receives MMGSDI_CARD_INIT_COMPLETED_EVT it
  update the static variable cmph_received_card_init_completed_evt
  to TRUE. On some events (like refresh start) CM need to update the
  static variable cmph_received_card_init_completed_evt  to FALSE.
  This function set the value of cmph_received_card_init_completed_evt
  as per the parameter passed.

RETURN VALUE
  NONE

DEPENDENCIES
  None

==========================================================================*/
void  cmph_set_received_card_init_completed_evt(

  boolean val,
  /* this value is used to update the currect status if
  ** card initilization completed event
  */
  sys_modem_as_id_e_type asubs_id
);

/*==========================================================================

FUNCTION cmph_get_gw_subs_from_card_sent

DESCRIPTION
  If GW subscription available is sent to CM module from subscription module
  cmph_gw_subs_from_card_sent is set to TRUE. If not,
  cmph_gw_subs_from_card_sent is set to FALSE. This function returns
  the value of cmph_gw_subs_from_card_sent.

RETURN VALUE
  TRUE: if GW subscription sent from card.
  FALSE: if GW subscription is not sent from card.

DEPENDENCIES
  None

==========================================================================*/
boolean  cmph_get_gw_subs_from_card_sent( sys_modem_as_id_e_type asubs_id);

/*==========================================================================

FUNCTION cmph_set_gw_subs_from_card_sent

DESCRIPTION
  If GW subscription available is sent to CM module from subscription module
  cmph_gw_subs_from_card_sent is set to TRUE. If not,
  cmph_gw_subs_from_card_sent is set to FALSE.
  This function set the value of cmph_gw_subs_from_card_sent
  as per the parameter passed.

RETURN VALUE
  NONE

DEPENDENCIES
  None

===========================================================================*/
void  cmph_set_gw_subs_from_card_sent(

  sys_modem_as_id_e_type asubs_id,

  boolean val
  /* Used to update the value of cmph_gw_subs_from_card_sent
  */
);

/*===========================================================================

FUNCTION  cmph_set_lte_do_redir

DESCRIPTION
 Set the flag cmph_set_lte_do_redir to true.

DEPENDENCIES


RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmph_set_lte_do_redir (sys_modem_as_id_e_type asubs_id);

/*===========================================================================

FUNCTION  cmph_reset_lte_do_redir

DESCRIPTION
 Reset the flag cmph_reset_lte_do_redir to false.

DEPENDENCIES


RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmph_reset_lte_do_redir(sys_modem_as_id_e_type asubs_id);

/*==========================================================================

FUNCTION cmph_get_cdma_subs_from_card_sent

DESCRIPTION
  If CDMA subscription available is sent to CM module from subscription
  module cmph_cdma_subs_from_card_sent is set to TRUE. If not,
  cmph_cdma_subs_from_card_sent is set to FALSE. This function returns
  the value of cmph_cdma_subs_from_card_sent.

RETURN VALUE
  TRUE: if CDMA subscription sent from card.
  FALSE: if CDMA subscription is not sent from card.

DEPENDENCIES
  None

==========================================================================*/
boolean  cmph_get_cdma_subs_from_card_sent( sys_modem_as_id_e_type asubs_id);

/*==========================================================================

FUNCTION cmph_set_cdma_subs_from_card_sent

DESCRIPTION
  If CDMA subscription available is sent to CM module from subscription
  module cmph_gw_subs_from_card_sent is set to TRUE. If not,
  cmph_gw_subs_from_card_sent is set to FALSE.
  This function set the value of cmph_cdma_subs_from_card_sent
  as per the parameter passed.

RETURN VALUE
  NONE

DEPENDENCIES
  None

==========================================================================*/
void  cmph_set_cdma_subs_from_card_sent(

  sys_modem_as_id_e_type asubs_id,

  boolean val
  /* Used to update the value of cmph_cdma_subs_from_card_sent
  */

);



/*===========================================================================

FUNCTION cmph_dem_get_apps_pwr_status

DESCRIPTION
  Return the current apps power state

DEPENDENCIES
  none

RETURN VALUE
  current apps power status

SIDE EFFECTS
  none

===========================================================================*/

extern cm_apps_pwr_status_e_type cmph_dem_get_apps_pwr_status(void);

/*===========================================================================

FUNCTION cmph_remove_other_subs_act_priority_end

DESCRIPTION
  Remove the priority queue with activity priority = priority END from the other subs.

DEPENDENCIES
  none

RETURN VALUE
  TRUE: Call command requeued.
  False: Proceed with call origination.

SIDE EFFECTS
  none

===========================================================================*/
void    cmph_remove_other_subs_act_priority_end(
  sys_modem_as_id_e_type         asubs_id,

  boolean                        is_force
  /* Indicate if forcing top pref is desired or not */

);

/*===========================================================================

FUNCTION cmph_remove_and_force_orig_mode_with_priority_end

DESCRIPTION
  Process the origination parameters queue, and remove the specified orig
  modes with priority END and unforce the orig mode on ss.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void    cmph_remove_and_force_orig_mode_with_priority_end(
  mm_sub_stk_id_s_type            mm_id,

  boolean                 is_force
  /* Indicate ithat forcing mode_pref is desired or not
  */
);

/*===========================================================================

FUNCTION cmph_remove_orig_mode_with_priority_end

DESCRIPTION
  Process the origination parameters queue, and remove the specified orig
  modes with priority END

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern boolean cmph_remove_orig_mode_with_priority_end(

  mm_sub_stk_id_s_type  mm_id
  /* The ss on which to process the orig update
  */
);
/*===========================================================================

FUNCTION cmph_remove_orig_mode_data_call_below_ph

DESCRIPTION
  Process the origination parameters queue, and removes the preferences which are below PH and corresponding call
  is not active.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

extern void cmph_remove_orig_mode_data_call_below_ph(

  mm_sub_stk_id_s_type mm_id
);

/*===========================================================================

FUNCTION cmph_remove_orig_mode_with_act_id

DESCRIPTION
  Process the origination parameters queue, and remove the specified act id entity

DEPENDENCIES
  None

RETURN VALUE
  command deleted or not

SIDE EFFECTS
  None

===========================================================================*/
boolean cmph_remove_orig_mode_with_act_id(

  mm_sub_stk_id_s_type     mm_id,
  /* The ss on which to process the orig update
  */
  cm_act_id_type            act_id,
  /* The unique id of the activity */
  boolean *top_ptr_changed
);

/*===========================================================================

FUNCTION cmph_remove_act_id_and_force_orig_mode

DESCRIPTION
  Process the origination parameters queue, and remove the specified act if
  and unforce the orig mode on ss.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void    cmph_remove_act_id_and_force_orig_mode(

  mm_sub_stk_id_s_type            mm_id,

  cm_act_id_type            act_id,
  /* The unique id of the activity */
  boolean                 is_emerg
  /* Indicate if remove priority_end is due to emergency all origination
  ** on the other sub
  */
);

/*===========================================================================

FUNCTION cmph_remove_orig_mode_with_no_call_obj

DESCRIPTION
  Process the origination parameters queue, and remove the entry not belonging to that act id

DEPENDENCIES
  None

RETURN VALUE
  command deleted or not

SIDE EFFECTS
  None

===========================================================================*/
void cmph_remove_orig_mode_with_no_call_obj(
  mm_sub_stk_id_s_type    mm_id,
  cm_act_type_e_type act_type,
  cm_act_priority_e_type prio_type
);


/*===========================================================================

FUNCTION cmph_ptr

DESCRIPTION
  Return a pointer to the one and only phone object.

  The phone object is responsible for:
  1. Processing clients' phone commands.
  2. Processing MC replies for clients' phone commands.
  3. Processing MC phone related notifications.
  4. Notifying the client list of phone events.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern cmph_s_type  *cmph_ptr( void );

/*===========================================================================

FUNCTION cmph_get_volte_sub_info_ptr

DESCRIPTION
  Helper function to get a pointer to cmph_volte_ptr->volte_sub_info[] for a given sub_id.
  Checks that sub_id is in range of cmph_volte_ptr->volte_sub_info[] array.

DEPENDENCIES
  none

RETURN VALUE
  volte_sub_info_s_type* config

SIDE EFFECTS
  none

===========================================================================*/
volte_sub_info_s_type *cmph_get_volte_sub_info_ptr(
  sys_modem_as_id_e_type sub_id
);

/*===========================================================================


FUNCTION cm_mmgsdi_ssn_tbl_update_uicc_for_id

DESCRIPTION

  Update UICC for session entry matching given session id.

DEPENDENCIES
  This function is to be used only with the session based MMGSDI.

RETURN VALUE
  CM_MMGSDI_SUCCESS if successful
  CM_MMGSDI_UNSUPPORTED: Function is unsupported on this target
  CM_MMGSDI_NOTFOUND: Session ID for requested session type was not found

SIDE EFFECTS
  none

===========================================================================*/


extern cm_mmgsdi_error_e_type cm_mmgsdi_ssn_tbl_update_uicc_for_id
(
  mmgsdi_session_id_type,
  const uint8 *,
  uint32
);

/*===========================================================================

FUNCTION cmph_init

DESCRIPTION
  Initializing the phone object.

  This function must be called before the phone object
  is being used, in any way, place, or form.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmph_init( void );

/*===========================================================================

FUNCTION cmph_post_init_proc

DESCRIPTION
  Perform procedure post init of all entities of CM

  This function must be called before the phone object
  is being used, in any way, place, or form.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/

extern void cmph_post_init_proc(void);

/*===========================================================================

FUNCTION cmph_update_subscription_to_mmode

DESCRIPTION
  Refer to function definition.

===========================================================================*/

void cmph_update_subscription_to_mmode( mmoc_subsc_chg_e_type chg_type);

/*===========================================================================

FUNCTION cmph_get_mode_band_capability

DESCRIPTION
  Request for all the available modes and bands supported by the current
  target.

DEPENDENCIES
  None

RETURNS


SIDE_EFFECT
  None

===========================================================================*/
void cmph_get_mode_band_capability(

  sys_sys_mode_mask_e_type     *mode_capability,
  /* Mode capability supported by the current target */

  sys_band_mask_e_type         *band_capability,
  /* Band capability supported by the current target */

  sys_lte_band_mask_e_type     *lte_band_capability,
  /* LTE band capability supported by the current target */

  sys_band_mask_e_type         *tds_band_capability
  /* TD-SCDMA band capability supported by the current target */

);

/**===========================================================================

FUNCTION cmph_get_comm_mode_band_capab_with_pm

DESCRIPTION
  Request for all the available modes and bands intersected with cmph and policyman's

DEPENDENCIES
  None

RETURNS


SIDE_EFFECT
  None

===========================================================================*/
void cmph_get_comm_mode_band_capab_with_pm(

  sd_ss_mode_pref_e_type         *comm_mode,
  /* Mode capability supported by the current target */

  sd_ss_band_pref_e_type         *comm_band,
  /* Band capability supported by the current target */

  sys_lte_band_mask_e_type       *comm_lte_band,
  /* LTE band capability supported by the current target */

  sd_ss_band_pref_e_type         *comm_tds_band,
  /* TD-SCDMA band capability supported by the current target */

  sys_modem_as_id_e_type       as_id
  /* ASID for which the capabilties are required */

);

/*===========================================================================

FUNCTION cmph_map_call_type_to_sd_orig_mode

DESCRIPTION
  Map CM call-type type to SD origination mode type.

DEPENDENCIES
  none

RETURN VALUE
  The SD origination mode that corresponds to the input CM call-type.

SIDE EFFECTS
  none

===========================================================================*/
extern sd_ss_orig_mode_e_type   cmph_map_cm_call_type_to_sd_orig_mode(

  cm_call_type_e_type         call_type,
  /* Call type to map */

  sys_modem_as_id_e_type      asubs_id

);

/*===========================================================================

FUNCTION cmph_is_in_emergency_cb

DESCRIPTION
  Check if the phone is in emergency call back mode.
  valid.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if the phone is in emergency call back mode.
  FALSE otherwise.

SIDE EFFECTS
  None

===========================================================================*/
extern boolean cmph_is_in_emergency_cb(void);

/*===========================================================================

FUNCTION cmph_get_sub_in_emergency_cb

DESCRIPTION
  Return the sub in emergency call back mode.

DEPENDENCIES
  None

RETURN VALUE
  sys_modem_as_id_e_type

SIDE EFFECTS
  None

===========================================================================*/
extern sys_modem_as_id_e_type cmph_get_sub_in_emergency_cb(void);

/*===========================================================================

FUNCTION cmph_call_start

DESCRIPTION
  Does necessary phone's call start activities.

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmph_call_start(

  cmcall_s_type    *call_ptr
  /* Pointer to a call object */
);

/*===========================================================================

FUNCTION cmph_call_end_process

DESCRIPTION
  Does necessary phone's call end activities.

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmph_call_end_process
(

  const cmcall_s_type    *call_ptr
  /* Pointer to a call object */
);

/*===========================================================================

FUNCTION cmph_call_end

DESCRIPTION
  Does necessary phone's call end activities.

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmph_call_end(
  const cmcall_s_type    *call_ptr
  /* Pointer to a call object */
);

/*===========================================================================

FUNCTION cmph_pd_session_start

DESCRIPTION
  Does necessary phone's pd session start activities.

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmph_pd_session_start( void );


/*===========================================================================

FUNCTION cmph_pd_session_end

DESCRIPTION
  Does necessary phone's pd session end activities.

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmph_pd_session_end(
  sys_modem_as_id_e_type  asubs_id
);



/*===========================================================================

FUNCTION cmph_check_and_unforce_orig_mode

DESCRIPTION
  Process the origination parameters queue, and unforce the orig modes if
  required

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/


extern void cmph_check_and_unforce_orig_mode(

  mm_sub_stk_id_s_type mm_id,
  /* The ss on which to process the orig update
  */

  boolean is_forced_del
  /*Skip checks and blindly delete orig mode with priority=END
  */

);


/*===========================================================================

FUNCTION cmph_remove_orig_mode

DESCRIPTION
  Remove the orig modes specified

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/


extern void cmph_remove_orig_mode(
  mm_sub_stk_id_s_type mm_id,
  /* The ss on which to process the orig update
  */

  cm_act_type_e_type act_type,
  /* The activity type to check
  */

  cm_act_priority_e_type prio_type
  /* The priority type to check
  */
);

/*===========================================================================

FUNCTION cmph_notify_mmoc_no_sim

DESCRIPTION
  After an emergency call has ended, we must notify MMOC the
  SIM is missing or failed.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmph_notify_mmoc_no_sim (

  sys_modem_as_id_e_type asubs_id
  /* sub on which the emergency call has ended */
);

/*===========================================================================

FUNCTION cmph_info_get

DESCRIPTION
  Copy the current phone state information into a specified buffer.

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  TRUE if phone information is valid, FALSE otherwise.

  Note that phone information only becomes valid after cmph_init()
  returns.

SIDE EFFECTS
  none

===========================================================================*/
extern boolean cmph_info_get(

  cm_ph_info_s_type       *ph_info_ptr,
  /* Copy phone state info into this buffer */

  sys_modem_as_id_e_type   asubs_id,

  cm_ph_event_e_type       ph_event
  /* notify client list of this phone event */
);



/*===========================================================================

FUNCTION cmph_client_cmd_proc

DESCRIPTION
  Process clients' phone commands.

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmph_client_cmd_proc(

  cm_ph_cmd_s_type  *ph_cmd_ptr
  /* pointer to a CM command */
);

/*===========================================================================

FUNCTION cmph_client_cmd_mmll_reply

DESCRIPTION
  Process MC replies to clients phone commands.

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmph_client_cmd_mmll_reply(

  const cm_ph_cmd_s_type     *ph_cmd_ptr,
  /* Pointer to a CM command */

  cm_ph_cmd_err_e_type    cmd_err
  /* Indicate phone command error */
);

/*===========================================================================

FUNCTION cmph_client_cmd_forward_mmll

DESCRIPTION
  Forward a client phone commands to mmoc/ll.

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  TRUE if CM needs to wait for a replay from LL before continuing
  processing this command, FALSE otherwise.

SIDE EFFECTS
  none

===========================================================================*/
extern boolean cmph_client_cmd_forward_mmll(

  cm_ph_cmd_s_type      *ph_cmd_ptr
  /* Pointer to a CM command */

);
/*===========================================================================

FUNCTION cmph_sd_rpt_proc

DESCRIPTION
  Process System Determination reports


DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmph_sd_rpt_proc(

  const cm_hdr_type   *rpt_ptr
  /* Pointer to SD reports */
);


/*===========================================================================

FUNCTION cmph_generic_rpt_proc

DESCRIPTION
  Process Generic reports


DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmph_generic_rpt_proc(

  const cm_hdr_type   *rpt_ptr
  /* Pointer to generic reports */
);



/*===========================================================================

FUNCTION cmph_timer_proc

DESCRIPTION
  Process timer events.

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmph_timer_proc(

  cm_timer_event_type    timer_event
  /* Indicate specific timer event */
);


/*===========================================================================

FUNCTION cmph_rf_init_complete_event_proc

===========================================================================*/
void cmph_rf_init_complete_event_proc(
const rf_init_comp_msg_type *rfm_ind
);

#ifdef FEATURE_CGPS
/*===========================================================================

FUNCTION cmph_gps_init_complete_event_proc

DESCRIPTION
  When RFM init and GPS init are both complete, move to ONLINE state.

INPUT
  None

===========================================================================*/
void cmph_gps_init_complete_event_proc(void);
#endif

/*===========================================================================

FUNCTION cmph_get_curr_nam

DESCRIPTION
  Get the current NAM.

DEPENDENCIES
  Phone object must be initialized.

RETURNS
  The current NAM. If phone was not yet initialized, return CM_NAM_NONE.

SIDE EFFECTS
  None

===========================================================================*/
extern cm_nam_e_type cmph_get_curr_nam( void );



/*===========================================================================

FUNCTION cmph_autonam_is_enabled

DESCRIPTION
  Check whether auto-NAM is currently enabled.

DEPENDENCIES
  Phone object must be initialized

RETURNS
  TRUE if auto NAM is enabled. FALSE otherwise.

SIDE EFFECTS
  None

===========================================================================*/
extern boolean cmph_autonam_is_enabled( void );



/*===========================================================================
FUNCTION CMPH_OFFLINE

DESCRIPTION
  This function performs offline processing for the phone object

DEPENDENCIES
  Phone object must be initialized

RETURNS
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmph_offline (void);



/*===========================================================================

FUNCTION cmph_orig_mode_reset

DESCRIPTION
  Reset the origination mode to none.
  Exit emergency callback mode.

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void cmph_orig_mode_reset(

  boolean                  force_top
  /* Force the top preferences */
);



/**--------------------------------------------------------------------------
** Functions - cdma
** --------------------------------------------------------------------------
*/

/*===========================================================================

FUNCTION cmph_is_valid_mode_pref

DESCRIPTION
  Check whether a given mode preference is valid.

DEPENDENCIES
  none

RETURN VALUE
  TRUE if mode preference is valid, FALSE otherwise.

SIDE EFFECTS
  none

===========================================================================*/
extern boolean cmph_is_valid_mode_pref(

  cm_mode_pref_e_type     mode_pref,
  /* mode preference to check */

  sys_modem_as_id_e_type  asubs_id
);

/*===========================================================================

FUNCTION cmph_is_valid_mode_pref_on_ss

DESCRIPTION
  Check whether a given mode preference is valid on a particular SS

DEPENDENCIES
  none

RETURN VALUE
  TRUE if mode preference is valid, FALSE otherwise.

SIDE EFFECTS
  none

===========================================================================*/
extern boolean cmph_is_valid_call_mode_pref_on_ss(

  sys_modem_as_id_e_type           asubs_id,
  /* SS on which to validate the mode pref */

  cm_mode_pref_e_type    mode_pref,
  /* mode preference to check */

  const cm_cmd_type    *cmd_ptr
);

/*===========================================================================

FUNCTION cmph_unforce_mode_on_the_fly

DESCRIPTION
  Unforce the mode preference on the fly

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  A ballpark estimation for the number of seconds it takes the phone to
  reacquire service. If mode is not forced return 0.

SIDE EFFECTS
  none

===========================================================================*/
extern dword cmph_unforce_mode_on_the_fly(

  sys_modem_as_id_e_type   asubs_id
);



#if defined(FEATURE_MMODE_CDMA_800) || defined(FEATURE_MMODE_CDMA_1900)


/*===========================================================================

FUNCTION cmph_mc_rpt_proc

DESCRIPTION
  Process MC reports (i.e. notifications of phone activity or
  replies to clients phone commands).


DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmph_mc_rpt_proc(

  const cm_hdr_type   *rpt_ptr
  /* Pointer to MC report */
);



/*===========================================================================

FUNCTION cmph_mo_sms_end

DESCRIPTION
  Does necessary phone's MO SMS end activities.

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmph_mo_sms_end(

  sys_modem_as_id_e_type asubs_id,

  dword          duration,
  /* when to unforce the mode */

  cm_act_id_type act_id
  /* The id of the activity, used to access the orig parameter Q */
);



/*===========================================================================

FUNCTION cmph_send_nam_change_to_mc

DESCRIPTION
  Forward a MC_NAM_CHANGED_F command to MC.

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  None

SIDE EFFECTS
  none

===========================================================================*/
extern void cmph_send_nam_change_to_mc(

  sys_modem_as_id_e_type asubs_id,

  cm_nam_e_type  nam
  /* The current NAM */
);



/*===========================================================================
FUNCTION cmph_get_user_mode_pref

DESCRIPTION
  This function get the user mode preference

DEPENDENCIES
  Phone object must be initialized

RETURNS
  persistent mode preference

SIDE EFFECTS
  none

===========================================================================*/
cm_mode_pref_e_type cmph_get_user_mode_pref (
  sys_modem_as_id_e_type asubs_id
);


#ifdef FEATURE_HDR_HYBRID
/*===========================================================================

FUNCTION CMPH_GET_CONFIG_HYBR_STATUS

DESCRIPTION
  This function get the config hybrid status
DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  TRUE- hybrid operation is enabled

SIDE EFFECTS
  none

===========================================================================*/
boolean cmph_get_config_hybr_status(sys_modem_as_id_e_type asubs_id);

/*===========================================================================

FUNCTION cmph_select_dbm_mode_pref

DESCRIPTION
  Select a mode preference suitable for data burst message transmission

DEPENDENCIES
  none

RETURN VALUE
  mode_pref: mode reference used for dbm

SIDE EFFECTS
  none

===========================================================================*/
cm_mode_pref_e_type cmph_select_dbm_mode_pref(void);
#endif /* FEATURE_HDR_HYBRID */
#endif /* FEATURE_MMODE_CDMA_800 || FEATURE_MMODE_CDMA_1900 */

/*===========================================================================

FUNCTION cmph_update_ddtm_status

DESCRIPTION
  Send a DDTM status to MC

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  None

SIDE EFFECTS
  none

===========================================================================*/
void  cmph_update_ddtm_status
(
  sys_modem_as_id_e_type      asubs_id
  /* Pointer to a phone object */
);


#ifdef FEATURE_DEDICATED_C2K_MODE
/*===========================================================================

FUNCTION cmph_send_hdrmc_session_close_cmd

DESCRIPTION
  Send an end command to either MC or HDRMC task

DEPENDENCIES
  Call object must have already been initialized with
  cmph_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmph_send_hdrmc_session_close_cmd( void );
#endif


/**--------------------------------------------------------------------------
** Functions - gsm/wcdma
** --------------------------------------------------------------------------
*/

#if (defined CM_GW_SUPPORTED || defined FEATURE_CM_LTE)
/*===========================================================================

FUNCTION cmph_rpt_proc

DESCRIPTION
  Process Lower Layer reports (i.e. notifications of phone activity or
  replies to clients phone commands).


DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmph_rpt_proc(

  const cm_hdr_type   *rpt_ptr
  /* Pointer to LL report */
);


/*===========================================================================

FUNCTION cm_aoc_update_acm

DESCRIPTION
  This function is called when the ACM needs to be updated.  This can
  occur when the CCM is updated, when a call starts or ends, or when
  the acm timer has expired.

DEPENDENCIES
  The call structure pointed to within call_ptr must be initialized.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
void cmph_update_acm(

  sys_modem_as_id_e_type  asubs_id
  /* sub whose acm needs to be updated */

);


/*===========================================================================

FUNCTION cm_ph_increment_segments

DESCRIPTION
  This function is called by Data Services when a segment is transferred.
  If the call in question is being charged, it increments the number of
  segments and checks if the signal should be sent alerting CM of a charge
  unit.

DEPENDENCIES
  The call structure pointed to within call_ptr must be initialized.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
void cm_ph_increment_segments(cmcall_s_type *call_ptr, uint32 segments);


/*===========================================================================

FUNCTION cmph_aoc_no_funds_available

DESCRIPTION
  This function is called to determine if funds are still availble
  when a call is originated.

DEPENDENCIES
  The call structure pointed to within call_ptr must be initialized.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
boolean cmph_aoc_no_funds_available(

  sys_modem_as_id_e_type    asubs_id
  /* Determine funds availability for this SS */

);

/*===========================================================================

FUNCTION cmph_aoc_no_funds

DESCRIPTION
  This function is called when we determine the phone is out of funds.
  It sends the warning if it needs it and sets the drop_calls variable.

DEPENDENCIES
  The call structure pointed to within call_ptr must be initialized.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
void cmph_aoc_no_funds(

  sys_modem_as_id_e_type   asubs_id
  /* sub on which funds are not available */

);

#endif /* defined(FEATURE_GSM) || defined (FEATURE_WCDMA) || defined(FEATURE_CM_LTE) */


/*===========================================================================^M

FUNCTION cmph_update_orig_mode_uptime

DESCRIPTION
  This function increases the orig mode uptime of ph by CMPH_HOLD_MODE_PREF_TIME
  plus clock uptime.

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmph_update_orig_mode_uptime (

  mm_sub_stk_id_s_type mm_id,

  cm_act_id_type act_id
);


/*===========================================================================^M

FUNCTION cmph_is_cdma_subscription_available

DESCRIPTION
  This function returns a boolean indicating whether cdma subscription is available.

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  boolean

SIDE EFFECTS
  none

===========================================================================*/
boolean cmph_is_cdma_subscription_available (

  sys_modem_as_id_e_type     asubs_id
);

/*===========================================================================

FUNCTION cmph_get_cdma_subscription_source

DESCRIPTION
  This function will return where cdma is currently getting it's subscription or none if there
  is no subscription available.

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  source of cdma subscription

SIDE EFFECTS
  none

===========================================================================*/
cm_cc_subscription_source_e_type cmph_get_cdma_subscription_source (
  sys_modem_as_id_e_type asubs_id
);


/*===========================================================================

FUNCTION cmph_get_gw_subscription_source

DESCRIPTION
  This function will return where gw is currently getting it's subscription or none if there
  is no subscription available.

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  source of gw subscription

SIDE EFFECTS
  none

===========================================================================*/
cm_cc_subscription_source_e_type cmph_get_gw_subscription_source
(
  sys_modem_as_id_e_type asubs_id
);

/*===========================================================================

FUNCTION cmph_activate_ps_service_req_timer

DESCRIPTION
  This function activates the PS service request timer.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void cmph_activate_ps_service_req_timer(
  sys_modem_as_id_e_type  as_id
  /* sub on which to activate ps service req timer */
);


#ifdef FEATURE_HICPS_STACK_SYNC_UP
/*===========================================================================

FUNCTION cmph_activate_hicps_gwl_avoid_timer

DESCRIPTION
  This function activates the GW avoidance timer.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void cmph_activate_hicps_gwl_avoid_timer( sys_modem_as_id_e_type );

/*===========================================================================

FUNCTION cmph_deactivate_hicps_gwl_avoid_timer

DESCRIPTION
  This function deactivates the GW avoidance timer.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void cmph_deactivate_hicps_gwl_avoid_timer( sys_modem_as_id_e_type );
#endif

/*===========================================================================

FUNCTION cmph_call_end_req

DESCRIPTION
  Does necessary phone's call end req activities. It is applicable
  for GSM/WCDMA only.

  Currently this function just sends the CMPH_EVENT_IN_USE_STATE if
  the overall call state is IDLE.

  This function should be called from cmwcall_end_req while sending
  CM_CALL_EVENT_END_REQ to its clients.


DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmph_call_end_req
(
  const cmcall_s_type    *call_ptr
  /* Pointer to a call object */
);

/*===========================================================================

FUNCTION cmph_set_ps_sig_down_wait_time

DESCRIPTION

   This function sets the is_ps_signaling_down_wait_time
   with the value passed in the argument.

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmph_set_ps_sig_down_wait_time(

  dword          time,
  /* wait time */

  sys_modem_as_id_e_type   asubs_id
  /* SS on which the PS signalling is down */
);

/*===========================================================================

FUNCTION cmph_is_waiting_for_ps_sig_down

DESCRIPTION

   This function return TRUE if CM is waiting for
   the PS Signaling down indication from lower layers.

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
 TRUE  : If CM is waiting for the PS Sig Down Ind from Lower Layers.
 FALSE : If CM is not waiting for the PS Sig Down Ind from Lower Layers.

SIDE EFFECTS
  none

===========================================================================*/
extern boolean cmph_is_waiting_for_ps_sig_down(

  sys_modem_as_id_e_type    asubs_id
  /* Stack */
);


/*===========================================================================

FUNCTION cmph_als_reset

DESCRIPTION

  ALS custom object was reset due to PH receiving subscription available,
  not available or changed event, so reset ALS settings in PH object.

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmph_als_reset(void);

/*===========================================================================

FUNCTION cmph_als_supported

DESCRIPTION

  ALS is supported when:

  - CPHS is allowed
  - ALS is allowed
  - ALS custom object has all custom logic functions registered

DEPENDENCIES

  Phone object must have already been initialized with
  cmph_init().

  ALS custom logic object must have already been initialized with
  cmals_init().


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern boolean cmph_als_supported(void);


/*===========================================================================

FUNCTION cmph_is_valid_network_type_for_ss

DESCRIPTION
  Check whether a specified combination of ss and network type is
  supported by the current target.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if the specified combination of ss and network type is supported.
  FALSE otherwise.

SIDE EFFECTS
  None

===========================================================================*/
extern boolean cmph_is_valid_network_type_on_ss(

  sys_modem_as_id_e_type    asubs_id
);

/*===========================================================================

FUNCTION cmph_term_pending_get_networks_req

DESCRIPTION

  This function should be called when the phone preferences have changed.

DEPENDENCIES

  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern boolean cmph_term_pending_get_networks_req(
   sys_modem_as_id_e_type    asubs_id,
   boolean                   end_pf_scan,
   boolean                   force_policy
);

/*===========================================================================

FUNCTION cmph_terminate_periodic_get_networks_req_only

DESCRIPTION
  This function will terminate an ongoing periodic manual search.
  If the periodic search is on hold while another manual search is ongoing
  (CSG/internal), this will only reset the periodic search info.
  If the periodic search is the only existing search, then it will be
  terminated.

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  Boolean indicating whether the periodic search was cleaned up

SIDE EFFECTS
  None

===========================================================================*/
boolean cmph_terminate_periodic_get_networks_req_only(
  sys_modem_as_id_e_type    asubs_id
);

/*===========================================================================

FUNCTION cmph_term_pending_get_networks_req_client_only

DESCRIPTION

  This function should be called when the phone preferences have changed.

DEPENDENCIES

  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void cmph_term_pending_get_networks_req_client_only
(
  mm_sub_stk_id_s_type              mm_id
  /* Indicates the SS-Instance ( SS_MAIN or SS_HDR) based on which
  ** the corresponding getNetworks requests are terminated.
  */
);

/*===========================================================================

FUNCTION cmph_add_ccbs_entry

DESCRIPTION

  ccbs index and called party get stored in phone object

DEPENDENCIES

  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  TRUE  - ccbs index could be added with success
  FALSE - ccbs index could not be added

SIDE EFFECTS
  ccbs_indx_store gets changed

===========================================================================*/
extern boolean cmph_add_ccbs_entry (

  sys_modem_as_id_e_type                       asubs_id,
  /* SS on which the SUPS rpt is recieved */

  byte                               ccbs_index,
  /* ccbs index to be added */

  cm_called_party_bcd_no_s_type      called_party
  /* Called party number */

);


/*===========================================================================

FUNCTION cmph_remove_ccbs_entry

DESCRIPTION

  Remove ccbs index from phone object entry.

DEPENDENCIES

  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE

  TRUE  - ccbs index remove oepration succeeded.
  FALSE - ccbs index remove operation failed.

SIDE EFFECTS
  ccbs_indx_store gets changed

===========================================================================*/
extern boolean cmph_remove_ccbs_entry (

  sys_modem_as_id_e_type            asubs_id,
  /* stack on which to remove the call */

  byte                              ccbs_index
  /* Remove ccbs entry from phone object */
);


/*===========================================================================

FUNCTION cmph_clear_ccbs_entries

DESCRIPTION

  Clears current list of ccbs indexes in phone object

DEPENDENCIES

  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE

  None

SIDE EFFECTS
  ccbs_indx_store gets changed

===========================================================================*/
extern void cmph_clear_ccbs_entries(sys_modem_as_id_e_type asubs_id);


/*===========================================================================

FUNCTION cmph_cmd_client_release

DESCRIPTION
  Inform CM that a client was released so that CM can inform asynchronously
  free the client object.

  This is mostly important when a client release happens in the client
  callback function that is called during an event, or immediately after it
  (in case of multiprocessor builds).

DEPENDENCIES
  cm_init_before_task_start() must have already been called.

RETURN VALUE
  none

SIDE EFFECTS

===========================================================================*/
extern void cmph_cmd_client_release(

  cm_client_s_type        *client_ptr
  /**< Client object that should be released */

);

/*===========================================================================

FUNCTION cmph_update_orig_param

DESCRIPTION
  Updates the origination parameters in the priority queue.
  Currently only CM_ACT_UPDATE_REAS_END is supported

DEPENDENCIES
 None

RETURN VALUE
  True if the act_id object is found in the queue and updated
  False otherwise

SIDE EFFECTS
  None.

===========================================================================*/

extern boolean cmph_update_orig_param(

  cm_act_id_type            act_id,
  /* The id of the activity whose orig_params are to be updated
  */
  mm_sub_stk_id_s_type      mm_id,
  /* The ss on which the activity was going on
  */
  cm_act_update_reas_e_type act_update_reas
  /* The reason for updating the activity
  */
);

/*===========================================================================

FUNCTION cmph_update_orig_para_incoming_call

DESCRIPTION
  Updates the origination parameters, for the incoming call.

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init(), the orig queues must be initialized.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern boolean cmph_update_orig_param_incoming_call
(
  const cmcall_s_type *call_ptr
  /* Pointer to the call object
  */
);

/*===========================================================================

FUNCTION cmph_insert_orig_mode

DESCRIPTION
  Inserts the origination parameters in the priority queue.

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
 TRUE If the top pointer changed
 FALSE Otherwise

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean cmph_insert_orig_mode(

  mm_sub_stk_id_s_type              mm_id,
  /* SS to use for origination, if hybrid operation is changed,
  ** then ss= SD_SS_MAX.
  */

  sd_ss_orig_mode_e_type    orig_mode,
  /* Origination Mode */

  cm_act_type_e_type    act_type,
  /* Type of CM activity */

  cm_pref_term_e_type       force_term,
  /* Term for which to force the preferences - one-call, etc. */

  cm_mode_pref_e_type       mode_pref,
  /* Mode preference which to force */

  cm_band_pref_e_type       band_pref,
  /* Band preference which to force */

  sys_lte_band_mask_e_type  lte_band_pref,
  /* LTE band preference which to force */

  cm_band_pref_e_type       tds_band_pref,
  /* TDS band preference which to force */

  cm_prl_pref_e_type        prl_pref,
  /* PRL preference which to force */

  cm_roam_pref_e_type       roam_pref,
  /* Roam preference which to force */

  cm_hybr_pref_e_type       hybr_pref,
  /* hybr preference which to force */

  sd_ss_hybr_pref_e_type    int_hybr_pref,
  /* Hybrid preferences used internally by CM */

  cm_srv_domain_pref_e_type srv_domain_pref,
  /* service domain preference to force */

  cm_act_id_type            act_id,
  /* The uniques id of the activity that is forcing the mode */

  cm_act_update_reas_e_type act_update_reas,
  /* Reason for the update */

  uint16                    req_id,
  /* Sys sel pref request id */

  sys_csg_id_type           csg_id,
  /**< CSG identifier */

  sys_radio_access_tech_e_type csg_rat
  /**< RAT specified for CSG */
);

/*===========================================================================

FUNCTION cmph_delete_orig_param

DESCRIPTION
  Deletes the origination parameters in the priority queue.
  Currently only CM_ACT_UPDATE_REAS_END is supported

DEPENDENCIES
 None

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/

void cmph_delete_orig_param(

  cm_act_id_type            act_id,
  /* The id of the activity whose orig_params are to be updated
  */
  mm_sub_stk_id_s_type      mm_id
  /* The ss on which the activity was going on
  */
);

/*===========================================================================

FUNCTION cmph_get_last_pref_sent

DESCRIPTION
  Gets a pointer to the last pref sent to SD

DEPENDENCIES


RETURN VALUE
 Pointer to the last origination pref sent to SD

SIDE EFFECTS
  None.

===========================================================================*/
extern cm_act_orig_s_type *cmph_get_last_pref_sent(

  mm_sub_stk_id_s_type   mm_id
  /* SS to get the previous orig preferences */

);

/*===========================================================================

FUNCTION cmph_is_change_in_gwl_cap

DESCRIPTION
verifies changes in GWL capabilities.

DEPENDENCIES
  Phone object must have already been initialized with cmph_init().

RETURN VALUE
 TRUE/FALSE

SIDE EFFECTS
  none

===========================================================================*/

extern boolean cmph_is_change_in_gwl_cap(cm_ph_cmd_info_s_type      *cmd_info_ptr);

/*===========================================================================

FUNCTION cmph_get_is_kicked_hybr_ptr

DESCRIPTION
  Returns the is_kicked_hdr value

DEPENDENCIES
  none

RETURN VALUE
  Returns the is_kicked_hdr value

SIDE EFFECTS
  None

===========================================================================*/
extern boolean *cmph_get_is_kicked_hybr_ptr(void);

/*===========================================================================

FUNCTION cmph_hdr_kick_pending

DESCRIPTION
  Returns if hdr kick is pending

DEPENDENCIES
  none

RETURN VALUE
  Returns the is_kicked_hdr value

SIDE EFFECTS
  None

===========================================================================*/

extern boolean *cmph_hdr_kick_pending( void );

/*===========================================================================

FUNCTION cmph_ddtm_disable_cdma_tx

DESCRIPTION
  Send cmd to CDMA to disable TX.

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  None

SIDE EFFECTS
  none

===========================================================================*/
extern void cmph_ddtm_disable_cdma_tx( sys_modem_as_id_e_type asubs_id );

/*===========================================================================

@FUNCTION cmph_send_ddtm_status

@DESCRIPTION
  Send a DDTM status to 1xCP.

@DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

@RETURNS
  None

@SIDE_EFFECT
  Updates the cur_ddtm_* fields of the phone object to keep track of the DDTM
  settings sent down to 1xCP.

===========================================================================*/
extern void cmph_send_ddtm_status
(
  sys_modem_as_id_e_type    asubs_id,
  boolean                     ddtm_status,
  /**< DDTM status to send to MC
  */

  sys_ddtm_act_mask_e_type    ddtm_act_mask,
  /**< DDTM mask to send to MC
  */

  sys_srv_opt_type const      *ddtm_srv_opt_list,
  /**< Service options to ignore if ignore SO bit is set in ddtm_act_mask.
  */

  uint16                      ddtm_num_srv_opt
  /**< No. of service option lists to ignore.
  */

);


/*===========================================================================

FUNCTION cmph_ip_rpt_proc

DESCRIPTION
  Process IP specific reports

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmph_ip_rpt_proc(

  const cm_hdr_type   *rpt_ptr
  /* Pointer to SD reports */
);

/*===========================================================================

FUNCTION cmph_info_alloc

DESCRIPTION
 Allocate a phone info buffer.

DEPENDENCIES
  none

RETURN VALUE
  Pointer to allocated phone info buffer.

SIDE EFFECTS
  none

===========================================================================*/
extern cm_ph_info_s_type  *cmph_info_alloc( void );

cm_pref_s_type           *cm_pref_ptr (void);

/*===========================================================================

FUNCTION cmph_get_srv_type_ext

DESCRIPTION
  Returns srv_type for the particular call_type that
  has been given as input.
  If such an object does not exist returns NONE.

DEPENDENCIES
  CM must be initialized

RETURN VALUE
  srv_type of teh call, else NONE
  from_call will set to TRUE if call of call_type is found otherwise FALSE.

SIDE EFFECTS
  None

===========================================================================*/
extern cm_srv_type_e_type cmph_get_srv_type_ext( cm_call_type_e_type call_type, boolean *from_call);

/*===========================================================================

FUNCTION cmph_get_srv_type

DESCRIPTION
  Returns srv_type for the particular call_type that
  has been given as input.
  If such an object does not exist returns NONE.

DEPENDENCIES
  CM must be initialized

RETURN VALUE
  srv_type of teh call, else NONE

SIDE EFFECTS
  None

===========================================================================*/
extern cm_srv_type_e_type cmph_get_srv_type( cm_call_type_e_type );

/*===========================================================================

FUNCTION cmph_kick_hybr_2_if_pending

DESCRIPTION
  Kick Hybrid 2 stack if a kick is pending with the initial intended values.

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  none

SIDE EFFECTS
  None

===========================================================================*/
void cmph_kick_hybr_2_if_pending(

  sys_modem_as_id_e_type asubs_id,

  boolean         force_top
);

/*===========================================================================

FUNCTION cmph_get_is_kicked_hybr_2_ptr

DESCRIPTION
  Returns the is_kicked_hybr_2 value

DEPENDENCIES
  none

RETURN VALUE
  Returns the is_kicked_hybr_2 value

SIDE EFFECTS
  None

===========================================================================*/
extern boolean *cmph_get_is_kicked_hybr_2_ptr(void);


/*===========================================================================

FUNCTION cmph_delete_update_orig_mode

DESCRIPTION
  Delete the origination parameters from the priority queue.

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE

SIDE EFFECTS
  None.

===========================================================================*/
extern void cmph_delete_update_orig_mode(

  mm_sub_stk_id_s_type              mm_id,
  /* SS to use for origination, if hybrid operation is changed,
  ** then ss= CM_SS_MAX.
  */

  cm_act_id_type            act_id,
  /* The unique id of the activity */

  boolean                   force_ph_pref_if_top
  /* If the top ptr is the phone ptr force the phone preferences */

);

/*===========================================================================

FUNCTION cmph_update_act_id_priority_queue_obj_with_1_call_plus

DESCRIPTION
  Updates the priority queue object corresponding to act_id passed
  sets
  1. act_update_reas asact_end
  2. pref_term as call_1_plus.
  3. orig_mode_uptime is set to curr_uptime + 4 seconds
  4. act_priority to act_end


DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE

SIDE EFFECTS
  None.

===========================================================================*/
extern void cmph_update_act_id_priority_queue_obj_with_1_call_plus(

  mm_sub_stk_id_s_type              mm_id,
  /* SS to use for origination, if hybrid operation is changed,
  ** then ss= CM_SS_MAX.
  */

  cm_act_id_type            act_id
  /* The unique id of the activity */

);


/*===========================================================================

FUNCTION cmph_enter_emergency_cb

DESCRIPTION

  Enter emergency callback mode and notify clients

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void cmph_enter_emergency_cb(

  mm_sub_stk_id_s_type             mm_id,
  /* ss which connects the emerg call */

  boolean                  is_evt_reqd
  /* Do we need to generate a event */
);

/*===========================================================================

FUNCTION cmph_exit_emergency_cb

DESCRIPTION

  Exit emergency callback mode and notify clients

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void cmph_exit_emergency_cb(

  boolean        lte_lmtd_handling,

  boolean        is_force_pref,
  /*should force preferences*/
  boolean        wlan_calling
);

/*===========================================================================

FUNCTION cmph_is_gw_subsc_avail

DESCRIPTION
  This function returns whether the corresponding sub have valid GW subscription.


DEPENDENCIES
  Phone object must have already been initialized with cmph_init().

RETURN VALUE
  boolean

SIDE EFFECTS
  none

===========================================================================*/
extern boolean cmph_is_gw_subsc_avail(sys_modem_as_id_e_type as_id);

/*===========================================================================

FUNCTION cmph_force_orig_mode_on_the_stack

DESCRIPTION
  Unforce the origination mode on the fly

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  cmph_force_orig_mode_on_the_stack(

  mm_sub_stk_id_s_type      mm_id
  /* The ss on which the orig_mode is to be unforced */
);

/*===========================================================================

FUNCTION  cmph_is_msim

DESCRIPTION
 Check if the device mode is type of multiple sim verse single sim.

DEPENDENCIES
 cmph_init has been called.

RETURN VALUE
  TRUE if device mode is type of multiple sim verse single sim.
  FALSE otherwise.

SIDE EFFECTS
  none

===========================================================================*/
boolean cmph_is_msim (void);

/*===========================================================================

FUNCTION  cmph_is_svlte_any

DESCRIPTION
 Check if sub feature mode is svlte

DEPENDENCIES
 cmph_init has been called.

RETURN VALUE
  TRUE if sub feature mode is SVLTE
  FALSE otherwise.

SIDE EFFECTS
  none

===========================================================================*/
boolean cmph_is_svlte_any (void);

/*===========================================================================

FUNCTION  cmph_is_sxlte_any

DESCRIPTION
 Check if the feature mode says that device is SVLTE or SGLTE capable

DEPENDENCIES
 cmph_init has been called.

RETURN VALUE
  TRUE if feature mode is SVLTE or SGLTE
  FALSE otherwise.

SIDE EFFECTS
  none

===========================================================================*/
boolean cmph_is_sxlte_any (void);

/*===========================================================================

FUNCTION  cmph_is_sxlte

DESCRIPTION
 Check if the feature mode says that device is SVLTE or SGLTE capable

DEPENDENCIES
 cmph_init has been called.

RETURN VALUE
  TRUE if feature mode is SVLTE or SGLTE
  FALSE otherwise.

SIDE EFFECTS
  none

===========================================================================*/
boolean cmph_is_sxlte (void);

/*===========================================================================

FUNCTION  cmph_is_ssim

DESCRIPTION
 Check if the device mode is type of single_sim.

DEPENDENCIES
 cmph_init has been called.

RETURN VALUE
  TRUE if device mode is type of single_sim.
  FALSE otherwise.

SIDE EFFECTS
  none

===========================================================================*/
boolean cmph_is_ssim (void);

/*===========================================================================

FUNCTION  cmph_is_dsds

DESCRIPTION
 Check if the device mode is type of dual_sim_dual_standby.

DEPENDENCIES
 cmph_init has been called.

RETURN VALUE
  TRUE if device mode is type of dual_sim_dual_standby.
  FALSE otherwise.

SIDE EFFECTS
  none

===========================================================================*/
boolean cmph_is_dsds (void);

/*===========================================================================

FUNCTION  cmph_is_dsda

DESCRIPTION
 Check if the device mode is type of dual_sim_dual_active.

DEPENDENCIES
 cmph_init has been called.

RETURN VALUE
  TRUE if device mode is type of dual_sim_dual_active.
  FALSE otherwise.

SIDE EFFECTS
  none

===========================================================================*/
boolean cmph_is_dsda (void);

/*===========================================================================

FUNCTION  cmph_is_tsts

DESCRIPTION
 Check if the device mode is type of tsts.

DEPENDENCIES
 cmph_init has been called.

RETURN VALUE
  TRUE if device mode is type of tsts.
  FALSE otherwise.

SIDE EFFECTS
  none

===========================================================================*/
boolean cmph_is_tsts (void);

/*===========================================================================
  This function gives configuration about if ESR needs to be done in MSIM
  scenarios.
===========================================================================*/
boolean cmph_is_msim_standby_esr_enabled( void );

/*===========================================================================
  This function gives configuration about if ESR needs to be done in MSIM VoLTE
  scenarios.
===========================================================================*/
boolean cmph_is_msim_standby_volte_esr_enabled( sys_modem_as_id_e_type     asubs_id );

#if defined FEATURE_MMODE_TRIPLE_SIM
/*===========================================================================
FUNCTION cmph_update_curr_priority_sub

DESCRIPTION
  This function should be called when the current priority sub is deactivated so that
  sub having service should be updated with the current priority sub.

DEPENDENCIES
  cmpref2_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void                           cmph_update_curr_priority_sub
(
  void
);
#endif

/*===========================================================================

FUNCTION cmph_sp_subs_info_update

DESCRIPTION
  Updates subs info & priority parameters to MCS

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void cmph_sp_subs_info_update(void);

/*===========================================================================

FUNCTION cmph_get_first_sub_with_lte_cap

DESCRIPTION
  Returns the sub which has LTE in max cap.  For now, only 1 sub could support LTE.

DEPENDENCIES
  None

RETURN VALUE
  sys_modem_as_id_e_type

SIDE EFFECTS
  none

===========================================================================*/
sys_modem_as_id_e_type cmph_get_first_sub_with_lte_cap(void);

/*===========================================================================

FUNCTION cmph_is_ims_capable_on_sub

DESCRIPTION
  Returns whether the sub has IMS capability.

DEPENDENCIES
  None

RETURN VALUE
  boolean

SIDE EFFECTS
  none

===========================================================================*/
boolean cmph_is_ims_capable_on_sub(

  sys_modem_as_id_e_type  asubs_id
);

/*===========================================================================

FUNCTION cmph_is_dual_lte

DESCRIPTION
  Determines whether UE is in L+L (dual-LTE) capable mode.

DEPENDENCIES
  None

RETURN VALUE
  TRUE  - UE is in dual-LTE mode
  FALSE - UE is not in dual-LTE mode

SIDE EFFECTS
  none

===========================================================================*/
boolean cmph_is_dual_lte(void);

/*===========================================================================

FUNCTION cmph_is_lte_capable_on_sub

DESCRIPTION
  Check if the LTE is allowed on the specified sub per max allowed capability.

DEPENDENCIES
  None

RETURN VALUE
  boolean

SIDE EFFECTS
  None.

===========================================================================*/
boolean cmph_is_lte_capable_on_sub(
  sys_modem_as_id_e_type  asubs_id
);

/*===========================================================================

FUNCTION  cmph_reset_sys_sel_pref_rsp_pending

DESCRIPTION
 Reset the flag is_sys_sel_pref_rsp_pending to false.

DEPENDENCIES


RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmph_reset_sys_sel_pref_rsp_pending (sys_modem_as_id_e_type asubs_id);

/*===========================================================================

FUNCTION cmph_update_vsid_info

DESCRIPTION
  Updates subs info & priority parameters to clients,NAS & MCS

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/


void cmph_update_vsid_info(void);

/*===========================================================================


FUNCTION  cmph_reset_sys_sel_pref_rsp_pending2

DESCRIPTION
 Set the flag is_sys_sel_pref_rsp_pending2 to false.

DEPENDENCIES


RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmph_reset_sys_sel_pref_rsp_pending2 (void);


/*===========================================================================
FUNCTION cmph_send_oprt_mode_online_to_mmoc

DESCRIPTION
    This function sends operating mode online to mmoc if it is buffered.

DEPENDENCIES

RETURN VALUE
   Void

SIDE EFFECTS
  None
===========================================================================*/

extern void cmph_send_oprt_mode_online_to_mmoc
(
  void
);


#ifdef FEATURE_CM_LTE
/*===========================================================================

FUNCTION cmph_update_lte_ue_mode_of_operation

DESCRIPTION
  Updates UE mode of operation taking input
  from phone pointer (domain preference )and domain selection
  pointer (UE usage setting)

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmph_update_lte_ue_mode_of_operation(
  sys_modem_as_id_e_type   sub_id
);
/*===========================================================================

FUNCTION cmph_get_lte_ue_mode_of_operation

DESCRIPTION
  To get lte ue mode pref

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/

cm_lte_ue_mode_pref_e_type cmph_get_lte_ue_mode_of_operation(
  sys_modem_as_id_e_type   sub_id
);
#endif



/*===========================================================================

FUNCTION cmph_change_volte_call_priority

DESCRIPTION
  Move existing VOLTE call obj above/below PH  in MAIN/HYBR2 . Currently this
  is down during e911 orig and ECBM Enter/Exit.
  1) Move it down phone object during e911 call orig[Flag set to false]
  2)Move it up when e911 call has ended and currently not in ECBM[Flag set to True]

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmph_change_volte_call_priority(
  boolean                 volte_priority,
  /* Flag set to alter prioriy */

  sys_modem_as_id_e_type  as_id
);


#if (defined CM_GW_SUPPORTED && defined FEATURE_GRACEFUL_PDP_DEACTIVATION_B4_DETACH)
/*===========================================================================

FUNCTION cmph_get_pdp_deactivate_before_detach_supported_ptr

DESCRIPTION
  Return a pointer to gprs anite gcf flag

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  None

===========================================================================*/
extern boolean *cmph_get_pdp_deactivate_before_detach_supported_ptr(void);
#endif

/*===========================================================================
FUNCTION   cmph_getNVImsi

DESCRIPTION
  This function reads NV info.

DEPENDENCIES
  Currently usable only for Thin UI builds

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/

void cmph_getNVImsi (
  byte                   *imsi_p,
  sys_modem_as_id_e_type  asid
);

/* Auto Register */
/*===========================================================================
FUNCTION   cm_device_model_init

DESCRIPTION
  This function reads EFS info required for Auto Register Message.

DEPENDENCIES
  Currently usable only for Thin UI builds

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void cmph_device_model_init
(
  void
);

/*===========================================================================
FUNCTION cmph_set_apss_sw_version_efs

DESCRIPTION
    This function writes the SW Version received from APSS to EFS

DEPENDENCIES

RETURN VALUE
   Void

SIDE EFFECTS
  None
===========================================================================*/

void cmph_set_apss_sw_version_efs
(
  void
);



/*===========================================================================

FUNCTION cmph_cmd_evaluate_is_cap_update_required

DESCRIPTION
  This function compares the old & new mode/band prefs, and determines if a capability
  update needs to be notified to NAS through a PS detach or a TAU

DEPENDENCIES
  Phone object must have already been initialized with cmph_init().

RETURN VALUE
  TRUE if a capability update needs to be notified to NAS through a PS detach or a TAU
  FALSE otherwise
  NOTE: the pointer is_tau_cap_update_required will be updated to indicate if a TAU is
        needed instead of a PS detach

SIDE EFFECTS
  none

===========================================================================*/
boolean cmph_cmd_evaluate_is_cap_update_required(
  sys_modem_as_id_e_type      as_id,
  cm_mode_pref_e_type         old_cm_mode_pref,
  cm_band_pref_e_type         old_cm_band_pref,
  sys_lte_band_mask_e_type    old_cm_lte_band_pref,
  cm_band_pref_e_type         old_cm_tds_band_pref,
  cm_mode_pref_e_type         new_cm_mode_pref,
  cm_band_pref_e_type         new_cm_band_pref,
  sys_lte_band_mask_e_type    new_cm_lte_band_pref,
  cm_band_pref_e_type         new_cm_tds_band_pref,
  boolean                    *is_tau_cap_update_required,
  boolean                     is_pm_restrict_needed
);

/*===========================================================================

FUNCTION cmph_force_pref_on_the_fly_stack

DESCRIPTION
  Force the system selection preference on the fly on the sub+stack


DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
dword cmph_force_pref_on_the_fly_stack(

  mm_sub_stk_id_s_type            mm_id,
  /* Sub+Stack combo */

  sd_ss_pref_reas_e_type           force_reason,
  /* Reason for forcing the SS-preference */

  cm_act_type_e_type               act_type,
  /* Call type for which to force the preferences */

  cm_pref_s_type                   *pref_info_ptr,
  /* New Preferences being pushed on to stack */

  cm_activate_code_e_type          otasp_act_code,
  /* OTASP call activation code */

  cm_act_id_type                   act_id,
  /* The uniques id of the activity that is forcing the mode */

  cm_act_update_reas_e_type        act_update_reas,
  /* Reason for the update */

  boolean                          force_top_pref,
  /* Force the top ptr even if the top didn't change */

  const cm_acq_pri_order_pref_s_type   *rat_acq_order_pref_ptr,
  /* Rat acquisition priority order */

  uint16                           req_id
  /* Sys sel pref req id */

);

/*===========================================================================

FUNCTION cmph_is_new_policy_apply

DESCRIPTION
  Only apply the new policy if both stacks are idle.

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
boolean                          cmph_is_new_policy_apply(

  sys_modem_as_id_e_type              asubs_id
);

/*===========================================================================

FUNCTION cmph_send_oprt_mode

DESCRIPTION
  Informs change in operating mode.
  (DSDS/DSDA)

DEPENDENCIES

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void                          cmph_send_oprt_mode(

	   sys_oprt_mode_e_type              oprt_mode,
	   sys_modem_as_id_e_type              asubs_id
);

/*===========================================================================

FUNCTION cmph_send_sim_available_ph_event

DESCRIPTION
  This function sends the event CM_PH_EVENT_SIM_AVAILABLE


DEPENDENCIES

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmph_send_sim_available_ph_event( void );

/*===========================================================================

FUNCTION cmph_send_reg_csg_selection_update_cmd

DESCRIPTION
  This function sends the CM_CSG_SELECT_CONFIG_UPDATE_IND to REG


DEPENDENCIES

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/

void cmph_send_reg_csg_selection_update_cmd(ui_base_msgr_msg_csg_select_config_cmd_msg_type *);

/*===========================================================================

FUNCTION cmph_process_search_trigger_ind

DESCRIPTION
  This function sends the MM_CM_CSG_SEARCH_TRIGGER_IND to CSG app upon recieving CM_CSG_SEARCH_TRIGGER_IND from REG


DEPENDENCIES

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/

void cmph_process_search_trigger_ind( const cm_csg_search_trigger_ind_s_type *);

/*===========================================================================

    FUNCTION cmph_get_sxlte_sub

   DESCRIPTION
      This function returns which sub is sxlte


    DEPENDENCIES

   RETURN VALUE
      sys_modem_as_id_e_type

    SIDE EFFECTS
      none

    ===========================================================================*/
sys_modem_as_id_e_type cmph_get_sxlte_sub(void);

/*===========================================================================

FUNCTION  cmph_is_oprting_in_1xsrlte_mode

DESCRIPTION
 Returns TRUE if UE is operating in 1XSRLTE-SVLTE mode .

DEPENDENCIES


RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
boolean cmph_is_oprting_in_1xsrlte_mode(sys_modem_as_id_e_type         asubs_id);

/*===========================================================================
FUNCTION  cmph_is_oprting_in_csfb_pref_srlte

DESCRIPTION
 Returns TRUE if UE is operating in 1XSRLTE-SVLTE mode .

DEPENDENCIES
 cmmsc_init should already be called


RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
boolean cmph_is_oprting_in_csfb_pref_srlte(
  sys_modem_as_id_e_type         asubs_id
);

/*===========================================================================
FUNCTION  cmph_is_oprting_in_csfb_pref

DESCRIPTION
Returns TRUE if UE is operating in csfb preferred mode .



DEPENDENCIES
 cmmsc_init should already be called


RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
boolean cmph_is_oprting_in_csfb_pref(
  sys_modem_as_id_e_type         asubs_id
);

/*===========================================================================
FUNCTION  cmph_is_oprting_in_csfb_pref_csfb_sub_mode

DESCRIPTION
Returns TRUE, if UE is operating in CSFB sub mode of operation when UE mode is CSFB_PREF

DEPENDENCIES
 cmmsc_init should already be called


RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
boolean cmph_is_oprting_in_csfb_pref_csfb_sub_mode(
  sys_modem_as_id_e_type         asubs_id
);

/*===========================================================================

FUNCTION cmph_update_suspend_status

DESCRIPTION
  take action whenever a new suspend cmd has been sent to MMOC or response
  has been recieved from MMOC.
  update suspend status whenver response is obtained.
  increment or decrement num_request based on request/response sent/recieved.

DEPENDENCIES
  Ph object must have already been initialized with
  cmph_init().

RETURN VALUE
  None

SIDE EFFECTS
  none

===========================================================================*/
extern void cmph_update_suspend_status(

  mm_sub_stk_id_s_type            mm_id,
  /* indicates the stack that is being updated
  */

  boolean                 is_request,
  /* indicate if a request has been sent (TRUE) or response recieved (FALSE)
  */

  boolean                 suspend_status
  /* valid only if is_request = FALSE (response). It is the new value of
  ** stack suspend status
  */
);

/*===========================================================================

FUNCTION cmph_ss_susp_due_to_emerg

DESCRIPTION
  Check iff stack is supended due to emergency

DEPENDENCIES
  Ph object must have already been initialized with
  cmph_init().

RETURN VALUE
  TRUE if stack is suspended, FALSE otherwise

SIDE EFFECTS
  none

===========================================================================*/
boolean cmph_ss_susp_due_to_emerg(

  mm_sub_stk_id_s_type            mm_id
  /* Stack to be checked */
);

/*===========================================================================

FUNCTION cmph_get_ss_mode_pref

DESCRIPTION
  Returns the mode_pref of corresponding ss

DEPENDENCIES
  Ph object must have already been initialized with
  cmph_init().

RETURN VALUE
  mode_pref of ss, which is passed as argument

SIDE EFFECTS
  none

===========================================================================*/
cm_mode_pref_e_type cmph_get_ss_mode_pref(
  sys_modem_as_id_e_type  as_id
);

/*===========================================================================

FUNCTION  cmph_delete_and_unforce_activity

DESCRIPTION
 This function checks for the given activityand unforces it without any timer logic.

DEPENDENCIES
 none

RETURN VALUE
  ss

SIDE EFFECTS
  none

===========================================================================*/
void cmph_delete_and_unforce_activity(mm_sub_stk_id_s_type mm_id, cm_act_id_type  act_id);


/*===========================================================================

FUNCTION cmph_is_emerg_mode_on

DESCRIPTION
  This function returns the emergency mode of the corresponding subscription.

DEPENDENCIES
  TRUE if UE is in emergency mode
  FALSE if UE is not in emergency mode

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern boolean cmph_is_emerg_mode_on(
  sys_modem_as_id_e_type subs_id
  /* subscription id */
);

/*===========================================================================

FUNCTION cmph_update_emerg_status_info

DESCRIPTION
  This function updates the emergency mode for the corresponding subscription.

DEPENDENCIES
  None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmph_update_emerg_status_info(
  sys_modem_as_id_e_type subs_id,
  /* subscription id */

  boolean          emerg_mode
  /* emergency mode status */
);

/*===========================================================================

FUNCTION cmph_update_send_emerg_status_ind

DESCRIPTION
  This function updates the emergency mode in ph_ptr and sends the emergency
  mode status through the message router interface to clients - LTE RRC.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmph_update_send_emerg_status_ind(
  cm_emerg_mode_status_e_type status,
  /* emergency mode status */

  sys_modem_as_id_e_type     subs_id,
  /* Subscription on which the call was originated */

  sys_sys_mode_e_type sys_mode
  /* Sys mode on which the call was originated */

);

/*===========================================================================

FUNCTION cmph_add_mode_in_DSDX

DESCRIPTION
  To add the modes as dictated by NV-mode_disabled_in_TSTS to the mode_pref of
  multimode sub.

DEPENDENCIES
  Ph object must have already been initialized with
  cmph_init().
  standby pref should change from triple standby to dual/single standby

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmph_add_mode_in_DSDX(

  void
);

/*===========================================================================

FUNCTION cmph_remove_mode_in_TSTS

DESCRIPTION
   To remove the modes as dictated by NV-mode_disabled_in_TSTS from the mode_pref of
   multimode sub.

DEPENDENCIES
  Ph object must have already been initialized with
  cmph_init().
  standby pref should change from single/dual standby to triple standby

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmph_remove_mode_in_TSTS(
  uint8                   size,
  /* size of array */

  prot_subsc_chg_e_type  *sub_chg
  /* sub_chg array */
);

/*===========================================================================

FUNCTION cmph_remove_orig_mode_with_priority_end_per_subs

DESCRIPTION
  Remove the priority queue with activity priority = priority END from the sub.

DEPENDENCIES
  none

RETURN VALUE
 None.

SIDE EFFECTS
  none

===========================================================================*/
void    cmph_remove_orig_mode_with_priority_end_per_subs(
  sys_modem_as_id_e_type         asubs_id
);

/*===========================================================================

FUNCTION cmph_event_per_subs

DESCRIPTION
  Notify the client list of a specified phone event.

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmph_event_per_subs(

  cm_ph_event_e_type      ph_event,
  /* notify client list of this phone event */

  sys_modem_as_id_e_type            asubs_id
);
/*===========================================================================
FUNCTION cmph_nv_refresh

DESCRIPTION
  Refresh all CM PH NV items for the subs specified in the refresh_subs_mask

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmph_nv_refresh(
  uint8             refresh_subs_mask,
  /* Subs mask whose NVs need to be refreshed
  */
  boolean           is_pwr_on
  /* Subs mask whose NVs need to be refreshed
  */
);

/*===========================================================================

FUNCTION cmds_is_wait_on_subsc_cmd

DESCRIPTION
  sends subsc change command notification to DS
  wait until DS tear down the data traffic before continuing with operation

DEPENDENCIES
  none

RETURN VALUE
  TRUE - hold command until get confirmation from DS or time out
  FALSE - continue Proceed the command

SIDE EFFECTS
  none

===========================================================================*/

extern boolean  cmds_is_wait_on_subsc_cmd
(
  uint8      inactive_subs_id_bm
  /* Bitmask of subs_ids impacted */
);

/*===========================================================================

FUNCTION cmds_is_wait_on_oprt_mode

DESCRIPTION
  sends operation mode change information to DS
  If mode change is to LPM or poweroff, wait until DS tear down the data traffic
  before continuing with operation mode change processing

DEPENDENCIES
  none

RETURN VALUE
  TRUE - hold command until get confirmation from DS or time out
  FALSE - continue Proceed the command

SIDE EFFECTS
  none

===========================================================================*/
extern boolean  cmds_is_wait_on_oprt_mode
(
  sys_oprt_mode_e_type      oprt_mode
  /* New operating mode */
);

/*===========================================================================

FUNCTION cmph_send_subscription_not_available_internal()

DESCRIPTION
  Utility function to put the subscription not available command on to CM
  command queue. End all the


DEPENDENCIES


RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

========================================================================== */
void                               cmph_send_subscription_not_available_internal(

  sys_modem_as_id_e_type          asubs_id,

  cm_subscription_status_e_type   cdma_subs_status,
  /* should cdma subscription status be changed */

  cm_subscription_status_e_type   gw_subs_status,
  /* should gw subscription status be changed */

  cm_subscription_not_avail_cause_e_type sub_not_avail_cause
  /* Cause for subscription not available */

);

#ifdef FEATURE_CM_LTE
/*===========================================================================

FUNCTION cmph_reg_send_unblock_plmn_req

DESCRIPTION
    This function sendsPLMN unblocking to reg


DEPENDENCIES
  Phone object must have already been initialized with cmph_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmph_reg_send_unblock_plmn_req(

  sys_plmn_id_s_type                plmn,
  /* The id of the system to be selected */

  sys_modem_as_id_e_type            cmd_subs,
  /* The subscription on which the command is being issued.
  */

  sys_block_plmn_cause_e_type       unblock_cause
  /* The reason for unblcoking the PLMN
    */
);

/*===========================================================================

FUNCTION cmph_reg_send_unblock_plmn_force_pref_req

DESCRIPTION
    This function sendsPLMN unblocking to reg


DEPENDENCIES
  Phone object must have already been initialized with cmph_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmph_reg_send_unblock_plmn_force_pref_req(

  sys_plmn_id_s_type                plmn,
  /* The id of the system to be selected */

  sys_modem_as_id_e_type            cmd_subs,
  /* The subscription on which the command is being issued.
  */

  sys_block_plmn_cause_e_type       unblock_cause
  /* The reason for unblcoking the PLMN
    */
);

#endif

/*===========================================================================

FUNCTION cmph_send_trm_priority_req

DESCRIPTION
  Send TRM priority request(HIGH/LOW) to LTE RCC for multisim configuration
  (DSDS/DSDA)

DEPENDENCIES

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void  cmph_send_trm_priority_req(
  lte_trm_priority_e      trm_priority,
  mm_sub_stk_id_s_type    mm_id,
  boolean                 client_is_cm
);


/*===========================================================================

FUNCTION cmph_check_and_reduce_trm_priority

DESCRIPTION
    Check whether CM should LOWER the TRM or not based on ip_call_end_rsp_pend

DEPENDENCIES

RETURN VALUE
  True or FALSE

SIDE EFFECTS
  none

===========================================================================*/

void cmph_check_and_reduce_trm_priority(sys_modem_as_id_e_type asubs_id);


/*===========================================================================

FUNCTION cmph_send_trm_priority_req_proc_type

DESCRIPTION
  Send TRM priority request (based on proc_type) to LTE RCC for multisim configuration
  (DSDS/DSDA)

DEPENDENCIES

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmph_send_trm_priority_req_proc_type(
  sys_proc_type_e_type    proc_type,
  sys_modem_as_id_e_type  as_id,
  multimode_stack_e_type  stk,
  boolean                 client_is_cm
);

/*===========================================================================

FUNCTION cmph_send_plmn_list_to_lrc

DESCRIPTION
  Informs LTE RRC on which plmns to avoid.
  (DSDS/DSDA)

DEPENDENCIES

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmph_send_plmn_list_to_lrrc(
    sys_modem_as_id_e_type             asubs_id
);

/*===========================================================================

FUNCTION cmph_send_plmn_clear_list_ind_to_lrrc

DESCRIPTION
  Informs LTE RRC to clear the avoid plmn list.
  (DSDS/DSDA)

DEPENDENCIES

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/

void cmph_send_plmn_clear_list_ind_to_lrrc(
    sys_modem_as_id_e_type asubs_id
);

/*===========================================================================

FUNCTION cmph_add_avoid_plmn_to_list

DESCRIPTION
This function checks for duplicates in avoid plmn list. If no duplicates,
calls sys_add_avoid_plmn_to_list() to add the current camped plmn into the 
avoid plmn list.

DEPENDENCIES

RETURN VALUE
  TRUE if plmn added
  FALSE if plmn is not added

SIDE EFFECTS
  none

===========================================================================*/

boolean cmph_add_avoid_plmn_to_list(
     sys_modem_as_id_e_type asubs_id,
     sys_plmn_id_s_type  plmn_to_avoid
);

/*===========================================================================

FUNCTION cm_avoid_list_init

DESCRIPTION
  Initialize PLMN list to avoid.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  none

===========================================================================*/
void cm_avoid_list_init( sys_modem_as_id_e_type asubs_id);


/*===========================================================================

FUNCTION cmph_set_ssac_hyst_timer_req

DESCRIPTION
  UPdates value of t_ssac in CMPH context for volte hysteresis
  updates the NV as well
  If there is soem timer running currently it will continue to
  run with old value itself.

DEPENDENCIES

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmph_set_ssac_hyst_timer_req(
  cm_mm_set_ssac_hyst_req_s_type set_ssac_hyst_timer_req,
  sys_modem_as_id_e_type asubs_id
);


/*===========================================================================

FUNCTION cmph_send_get_ssac_hyst_timer_rsp

DESCRIPTION
  Sends the current value of the t_ssac hysteresis timer value to
  CM clients via MSGR interface MM_CM_GET_SSAC_HYST_TIMER_RSP.

DEPENDENCIES

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmph_send_get_ssac_hyst_timer_rsp( sys_modem_as_id_e_type asubs_id );

/*===========================================================================

FUNCTION cmph_check_and_send_standby_pref_change

DESCRIPTION
  If standby pref got buffered due to voice call,
  update pref change now.

DEPENDENCIES

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmph_check_and_send_standby_pref_change( void );

/*===========================================================================

FUNCTION cmph_get_acq_pri_order_per_sub

DESCRIPTION
  get the acq priority order by asubs_id

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  acq priority order

SIDE EFFECTS
  none

===========================================================================*/
cm_acq_pri_order_pref_s_type *cmph_get_acq_pri_order_per_sub(
  sys_modem_as_id_e_type  asubs_id
);

/*===========================================================================
FUNCTION cmph_calc_vsid_for_call_per_subs_id

DESCRIPTION
  This function calculates the VSID for the given call, per call serving system.

DEPENDENCIES


RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
sys_vsid_type cmph_calc_vsid_for_call_per_subs_id(sys_modem_as_id_e_type asubs_id);

/*===========================================================================

FUNCTION cm_ph_get_default_vsid

DESCRIPTION
  To get the default VSID.

DEPENDENCIES
  CM task should have been initialized

RETURN VALUE


SIDE EFFECTS
  None

===========================================================================*/
sys_vsid_type cm_ph_get_default_vsid( void );


/*===========================================================================

FUNCTION cmph_dds_update_srv_domain

DESCRIPTION
  Update phone pointer srv domain based on asubs_id

DEPENDENCIES

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmph_dds_update_srv_domain(
  sys_modem_as_id_e_type        asubs_id,
  cm_srv_domain_pref_e_type     new_srv_domain
);

/*===========================================================================

FUNCTION cmph_dds_update_cmd_mode_pref

DESCRIPTION
  Update phone pointer mode pref based on asubs_id

DEPENDENCIES

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmph_dds_update_cmd_mode_pref(
  sys_modem_as_id_e_type        asubs_id,
  cm_mode_pref_e_type           new_mode_pref
);

/*===========================================================================

FUNCTION cmph_get_sub_in_emg_cb

DESCRIPTION
  Get the sub which is in emergency cb mode.

DEPENDENCIES
  None.

RETURN VALUE
  sys_modem_as_id_e_type.

SIDE EFFECTS
  None.

===========================================================================*/
sys_modem_as_id_e_type  cmph_get_sub_in_emg_cb(void);

/*===========================================================================

FUNCTION cmph_is_lte_capable_on_any_sub

DESCRIPTION
  Check if the LTE is allowed on any sub per max allowed capability.

DEPENDENCIES
  None

RETURN VALUE
  boolean

SIDE EFFECTS
  None.

===========================================================================*/
boolean cmph_is_lte_capable_on_any_sub(void);


/*===========================================================================

FUNCTION cmph_get_ss_lte_band_pref

DESCRIPTION
return the LTE band mask on the sub.

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
none

===========================================================================*/
sys_lte_band_mask_e_type   cmph_get_ss_lte_band_pref(

  sys_modem_as_id_e_type  asubs_id,

  cmph_s_type            *ph_ptr
);

/*===========================================================================

FUNCTION cmph_send_high_prio_activity_ind

DESCRIPTION
  Sends an indication to REG to detach locally, when a high priority activity is triggered
  while CM is waiting for PS_DETACH_CNF from NAS

DEPENDENCIES

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmph_send_high_prio_activity_ind (

  /* detach required or not */
  boolean         local_detach
);


/*===========================================================================

FUNCTION cmph_is_buffer_sys_sel_pref_activity

DESCRIPTION
  To check if there is an LPM related cleanup activity or if the MM layer
  is busy in an MT page.

DEPENDENCIES
  TRUE if there is such activity
  FALSE if otherwise

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern boolean cmph_is_buffer_sys_sel_pref_activity(
  cm_ph_cmd_info_s_type *cmd_info_ptr
);

/*===========================================================================

FUNCTION cmph_is_cap_update_during_ext_audio_call

DESCRIPTION
  There is ongoing third party call and sys sel pref can result in PS detach or TAU

  For cases where sys sel pref buffering is enabled and is required , we 
  should buffer for this case.
  IF however, buffering is not needed and below condition is TRUE, rejection
  logic needs to be end of sys sel pref checking - as we evaluate capability update
  and it should be done on finally resolved mode/band prefs.

DEPENDENCIES
  none

RETURN VALUE
  TRUE if there above condition is met
  FALSE otherwise

SIDE EFFECTS
  none

===========================================================================*/
extern boolean cmph_is_cap_update_during_ext_audio_call(
  cm_ph_cmd_info_s_type *cmd_info_ptr
);

/*===========================================================================

FUNCTION cmph_enter_emergency_cb_wlan

DESCRIPTION

  Enter emergency callback mode for WLAN  and notify clients

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void cmph_enter_emergency_cb_wlan(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================

FUNCTION cmph_set_t_wwan_911_req

DESCRIPTION
  UPdates value of t wwan 911 timer.
  New values will apply for the calls originated after setting the timer.

DEPENDENCIES

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmph_set_t_wwan_911_req( uint16 t_wwan_911_val, sys_modem_as_id_e_type asubs_id);

/*===========================================================================

FUNCTION cmph_send_get_t_wwan_911_rsp

DESCRIPTION
  Sends the current value of the t_wwan_911 timer to clients.

DEPENDENCIES

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmph_send_get_t_wwan_911_rsp( sys_modem_as_id_e_type asubs_id );

/*===========================================================================

FUNCTION cmph_get_sub_ptr

DESCRIPTION
  Helper function to get sub info for a given sub_id.

DEPENDENCIES
  none

RETURN VALUE
  cmph_sub_info_s_type* cmph_sub_info

SIDE EFFECTS
  none

===========================================================================*/
cmph_sub_info_s_type *cmph_get_sub_ptr(
  sys_modem_as_id_e_type sub_id
);

/*===========================================================================

FUNCTION cmph_get_net_sub_ptr

DESCRIPTION
  Helper function to get the sub info ptr that will store the manual search
  info for a given sub_id.
  For DSDA manual search info is stored in the corresponding sub's structure.
  For DSDS manual search info is always stored in the primary sub's structure.

DEPENDENCIES
  None

RETURN VALUE
  cmph_get_sub_ptr

SIDE EFFECTS
  None

===========================================================================*/
cmph_sub_info_s_type *cmph_get_net_sub_ptr(
  sys_modem_as_id_e_type asubs_id
);


#ifdef FEATURE_MODEM_CONFIG_REFRESH
/*===========================================================================

FUNCTION cm_mcfg_refresh_cb

DESCRIPTION
  This is a callback function used by MCFG for NV/EFS refresh purpose
  This function puts the received refresh info into CM command queue

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean cm_mcfg_refresh_cb(mcfg_refresh_info_s_type *p_info);
#endif

/*===========================================================================

FUNCTION cmph_get_ph_sub_avail_mask

DESCRIPTION
  Get the sub avail mask for the specified sub.

DEPENDENCIES
  None

RETURN VALUE
  uint8

SIDE EFFECTS
  None

===========================================================================*/
uint8  cmph_get_ph_sub_avail_mask (
  uint8  index
);

/*===========================================================================

FUNCTION cmph_get_ph_sub_avail_info

DESCRIPTION
  Get the array of phone subscription availability mask.

DEPENDENCIES
  cmph initialization is done.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void cmph_get_ph_sub_avail_info(
  uint8                  size,
  uint8                 *sub_avail
);

/*===========================================================================

FUNCTION cmph_get_ph_sub_pref

DESCRIPTION
  Populate the phone sub preference information for subscription transaction.

DEPENDENCIES
  cmph initialization is done.

RETURN VALUE
  uint8

SIDE EFFECTS
  None

===========================================================================*/
uint8  cmph_get_ph_sub_pref(
  uint8                  size,
  cm_pref_s_type       **sub_pref_ptr,
  prot_subsc_chg_e_type  *sub_chg
);

/*===========================================================================

FUNCTION cmph_get_top_sub_pref

DESCRIPTION
  Get the top priority preference for subscription transaction.

DEPENDENCIES
  cmph initialization is done.

RETURN VALUE
  uint8

SIDE EFFECTS
  None

===========================================================================*/
uint8  cmph_get_top_sub_pref(
  uint8                  size,
  cm_pref_s_type       **sub_pref_ptr
);

/*===========================================================================

FUNCTION cmph_map_sub_stk_to_struct

DESCRIPTION
  Map sub id and stack id to id in structure.

DEPENDENCIES
  None.

RETURN VALUE
  mm_sub_stk_id_s_type

SIDE EFFECTS
  None

===========================================================================*/
mm_sub_stk_id_s_type  cmph_map_sub_stk_to_id(
  sys_modem_as_id_e_type  as_id,
  multimode_stack_e_type  stk_id
);

/*===========================================================================

FUNCTION cmph_orig_mode_reset_on_ss

DESCRIPTION
  Reset the origination mode to none.

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void            cmph_orig_mode_reset_on_ss(

  mm_sub_stk_id_s_type    mm_id,
  /* The ss on which to reset
  */

  boolean         force_top,
  /* Force the top preferences
  */

  uint16         req_id
  /* sys sel pref related request Id
  */

);

/*===========================================================================

FUNCTION cmph_orig_mode_insert_new

DESCRIPTION
  Insert phone object

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void            cmph_orig_mode_insert_new(

  mm_sub_stk_id_s_type    mm_id,
  /* The ss on which to reset
  */

  uint16         req_id
  /* sys sel pref related request Id
  */

);

/*===========================================================================

FUNCTION cmph_get_network_type

DESCRIPTION
  Return phone object network type.

DEPENDENCIES
  None

RETURN VALUE
  network_type

SIDE EFFECTS
  None

===========================================================================*/
cm_mode_pref_e_type  cmph_get_network_type(
  sys_modem_as_id_e_type    mm_id
);

/*===========================================================================

FUNCTION cmph_get_is_in_use

DESCRIPTION
  Return is_in_use value.

DEPENDENCIES
  None

RETURN VALUE
  boolean

SIDE EFFECTS
  None

===========================================================================*/
boolean  cmph_get_is_in_use(
  sys_modem_as_id_e_type    asubs_id
);

/*===========================================================================

FUNCTION cmph_get_is_in_use_reported

DESCRIPTION
  Return is_in_use_reported value.

DEPENDENCIES
  None

RETURN VALUE
  boolean

SIDE EFFECTS
  None

===========================================================================*/
boolean  cmph_get_is_in_use_reported(
  sys_modem_as_id_e_type    asubs_id
);

/*===========================================================================

FUNCTION cmph_set_is_in_use

DESCRIPTION
  Set network type.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void cmph_set_is_in_use(
  sys_modem_as_id_e_type    asubs_id,
  boolean                   is_in_use
);

/*===========================================================================

FUNCTION cmph_set_is_in_use_reported

DESCRIPTION
  Set is_in_use_reported.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void cmph_set_is_in_use_reported(
  sys_modem_as_id_e_type    asubs_id,
  boolean                   is_in_use_reported
);

/*===========================================================================

FUNCTION cmph_get_other_sub

DESCRIPTION
  Return the other sub.

DEPENDENCIES
  None

RETURN VALUE
  sys_modem_as_id_e_type

SIDE EFFECTS
  None

===========================================================================*/
sys_modem_as_id_e_type  cmph_get_other_sub(
  sys_modem_as_id_e_type    asubs_id
);

/*===========================================================================

FUNCTION cmph_get_ens_value

DESCRIPTION
  Return ens_value.

DEPENDENCIES
  None

RETURN VALUE
  byte

SIDE EFFECTS
  None

===========================================================================*/
byte cmph_get_ens_value (
  sys_modem_as_id_e_type  asubs_id
);

/*===========================================================================

FUNCTION cmph_update_ens_value

DESCRIPTION
  Update ens_value.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void cmph_update_ens_value(
  sys_modem_as_id_e_type  asubs_id,
  byte                    ens_value
);

/*===========================================================================

FUNCTION cmph_is_rat_bal_enabled

DESCRIPTION
  Return if rat balance is enabled.

DEPENDENCIES
  None

RETURN VALUE
  boolean

SIDE EFFECTS
  None

===========================================================================*/
boolean cmph_is_rat_bal_enabled (
  sys_modem_as_id_e_type  asubs_id
);

/*===========================================================================

FUNCTION cmph_update_is_rat_bal_enabled

DESCRIPTION
  Update is_rat_bal.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void cmph_update_is_rat_bal_enabled(
  sys_modem_as_id_e_type  asubs_id,
  boolean                 is_rat_bal
);

/*===========================================================================

FUNCTION cmph_is_mode_change_ef_rat

DESCRIPTION
  Return the value of is_mode_change_due_to_ef_rat.

DEPENDENCIES
  None

RETURN VALUE
  boolean

SIDE EFFECTS
  None

===========================================================================*/
boolean cmph_is_mode_change_ef_rat (
  sys_modem_as_id_e_type  asubs_id
);

/*===========================================================================

FUNCTION cmph_update_mode_change_ef_rat

DESCRIPTION
  Update is_mode_change_due_to_ef_rat.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void cmph_update_mode_change_ef_rat(
  sys_modem_as_id_e_type  asubs_id,
  boolean                 is_mode_change_due_to_ef_rat
);
/*===========================================================================

FUNCTION cmph_is_user_ss_pref_in_full_rat

DESCRIPTION
  Return the value of is_user_ss_pref_in_full_rat

DEPENDENCIES
  None

RETURN VALUE
  boolean

SIDE EFFECTS
  None

===========================================================================*/
boolean cmph_is_user_ss_pref_in_full_rat (
  sys_modem_as_id_e_type  asubs_id
);
/*===========================================================================

FUNCTION cmph_update_user_ss_pref_in_full_rat

DESCRIPTION
  Update user_ss_pref_in_full_rat

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void cmph_update_user_ss_pref_in_full_rat(
  sys_modem_as_id_e_type  asubs_id,
  boolean                 user_ss_pref_full_rat
);

/*===========================================================================

FUNCTION cmph_is_uim_usim_rat_present

DESCRIPTION
  Return the value of is_uim_usim_rat_present.

DEPENDENCIES
  None

RETURN VALUE
  boolean

SIDE EFFECTS
  None

===========================================================================*/
boolean  cmph_is_uim_usim_rat_present(
  sys_modem_as_id_e_type  asubs_id
);

/*===========================================================================

FUNCTION cmph_update_uim_usim_rat_present

DESCRIPTION
  Update is_uim_usim_rat_present.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void cmph_update_uim_usim_rat_present(
  sys_modem_as_id_e_type  asubs_id,
  boolean                 is_uim_usim_rat_present
);

#if defined(CM_GW_SUPPORTED) || defined (FEATURE_CM_LTE)
/*===========================================================================

FUNCTION cmph_is_mt_page_in_prog

DESCRIPTION
  Check if there is a mt page in progress for the device.

DEPENDENCIES
  None

RETURN VALUE
  boolean

SIDE EFFECTS
  None

===========================================================================*/
boolean cmph_is_mt_page_in_prog(void);

/*===========================================================================

FUNCTION cmph_is_mt_page_in_prog_per_sub

DESCRIPTION
  Check if there is a mt page in progress for the sub.

DEPENDENCIES
  None

RETURN VALUE
  boolean

SIDE EFFECTS
  None

===========================================================================*/
boolean cmph_is_mt_page_in_prog_per_sub(sys_modem_as_id_e_type asubs_id);
#endif

/*==========================================================================

FUNCTION cmph_adjust_hybr_pref_for_pending_pref

DESCRIPTION
  Function that adjust's the hybr_pref oc ommand pointer as per switching values

RETURN VALUE
  None

DEPENDENCIES
  None
===========================================================================*/
void cmph_adjust_hybr_pref_for_pending_pref(cm_hybr_pref_e_type hybr_pref);

/*==========================================================================

FUNCTION cmph_is_get_networks_ongoing

DESCRIPTION
  Function returns TRUE if there is any ongoing get_networks present on provided as_id

RETURN VALUE
  TRUE/FALSE

DEPENDENCIES
  None
===========================================================================*/
boolean cmph_is_get_networks_ongoing(sys_modem_as_id_e_type as_id);

/*==========================================================================

FUNCTION cmph_is_standby_chg_on_refresh

DESCRIPTION
  During subscription becoming unavailable due to SIM REFRESH, calculate if
  standby pref change is needed, based on the REFRESH MODE and REFRESH source.

RETURN VALUE
  boolean Flag indicating whether standby pref has to be changed on REFRESH

DEPENDENCIES
  None
===========================================================================*/
boolean cmph_is_standby_chg_on_refresh
(
  mmgsdi_refresh_mode_enum_type  mode,

  mmgsdi_refresh_source_enum_type source


); /* cmph_is_standby_chg_on_refresh() */

/*==========================================================================

FUNCTION cmph_update_user_mode_pref

DESCRIPTION
  Function that update the's the user mode pref on receiving command sys_sel_pref from user or read from NV during ph_nv_reset.

RETURN VALUE
  None

DEPENDENCIES
  None
===========================================================================*/

void cmph_update_user_mode_pref(
  sys_modem_as_id_e_type asubs_id,
  cm_mode_pref_e_type mode_pref
);

/*==========================================================================

FUNCTION cmph_get_intersected_mode_pref_from_efrat_user_mode_pref

DESCRIPTION
  Function that retrieves a intersection of mode pref from EF_RAT and user mode pref settings.

RETURN VALUE
  None

DEPENDENCIES
  None
===========================================================================*/

cm_mode_pref_e_type cmph_get_intersected_mode_pref_from_efrat_user_mode_pref(cm_mode_pref_e_type user_pref,cm_mode_pref_e_type ef_mode_pref);

/*==========================================================================
FUNCTION cmph_find_standby_pref

DESCRIPTION
  Function that finds new standby pref based on active subs
  provided

RETURN VALUE
  None

DEPENDENCIES
  None
===========================================================================*/
sys_modem_dual_standby_pref_e_type cmph_find_standby_pref(cm_ph_cmd_info_s_type cmd_info_ptr, uint8 *new_sub_mask);

/*==========================================================================

FUNCTION cm_qrarb_client_cb

DESCRIPTION
  Function callback used for error notification by QRARB module

RETURN VALUE
  None

DEPENDENCIES
  None
===========================================================================*/
#ifdef FEATURE_DISABLED_HABANERO
extern boolean cm_qrarb_client_cb(

qrarb_cb_info_s qrarb_cb_info
);
#else
extern boolean cm_qrarb_client_cb(void);
#endif


/*==========================================================================

FUNCTION cmph_check_send_qrarb_restart_comp_ind

DESCRIPTION
  Function to send recovery complete complete indication to QRARB

RETURN VALUE
  None

DEPENDENCIES
  None
===========================================================================*/
extern void  cmph_check_send_qrarb_restart_comp_ind(void);

/*==========================================================================

FUNCTION cmph_is_sxlte_hybr2_stack_allowed

DESCRIPTION
  Function returns TRUE if there can be second stack for SXLTE/CH

RETURN VALUE
  TRUE/FALSE

DEPENDENCIES
  None
===========================================================================*/
boolean cmph_is_sxlte_second_stack_allowed_on_mode(sys_modem_as_id_e_type as_id, cm_mode_pref_e_type mode);

/*===========================================================================

FUNCTION cmph_reset_network_pref

DESCRIPTION
  Set preference for get network.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void cmph_reset_network_pref(
    sys_modem_as_id_e_type    asubs_id,
    boolean                   clear_pf_scan_info
);

boolean IS_VALID_PH_SUB_PTR(sys_modem_as_id_e_type          asubs_id);


/*==========================================================================

FUNCTION cmph_is_mcfg_refresh_ongoing

DESCRIPTION
  Checks if there is any MCFG refresh activity happening on given asubs

RETURN VALUE
  boolean Flag indicating whether refresh is ongoing or not

DEPENDENCIES
  None
===========================================================================*/
boolean cmph_is_mcfg_refresh_ongoing(sys_modem_as_id_e_type asubs_id);

/*==========================================================================

FUNCTION cmph_get_subs_with_mcfg_refresh_ongoing

DESCRIPTION
  Checks if there is any MCFG refresh activity happening on any subs

RETURN VALUE
  boolean Flag indicating whether refresh is ongoing or not

DEPENDENCIES
  None
===========================================================================*/
uint8 cmph_get_subs_with_mcfg_refresh_ongoing(void);

/*==========================================================================

FUNCTION cmph_set_subs_mcfg_state

DESCRIPTION
  sets MCFG state for given subs

RETURN VALUE
  none

DEPENDENCIES
  None
===========================================================================*/
void cmph_set_subs_mcfg_state(sys_modem_as_id_e_type asubs_id, cmph_subs_mcfg_state_e_type state);

/*==========================================================================

FUNCTION cmph_get_subs_mcfg_state

DESCRIPTION
  sets MCFG state for given subs

RETURN VALUE
  none

DEPENDENCIES
  None
===========================================================================*/
cmph_subs_mcfg_state_e_type cmph_get_subs_mcfg_state(sys_modem_as_id_e_type asubs_id);

/*==========================================================================

FUNCTION cmph_update_active_subs_for_mcfg_refresh

DESCRIPTION
  sets MCFG state for given subs

RETURN VALUE
  none

DEPENDENCIES
  None
===========================================================================*/
uint8 cmph_remove_mcfg_refresh_subs(uint8 active_subs);

/*==========================================================================

FUNCTION cmph_get_active_subs

DESCRIPTION
  returns active_subs of ph_ptr

RETURN VALUE
  none

DEPENDENCIES
  None
===========================================================================*/
uint8 cmph_get_active_subs(void);
/*===========================================================================

FUNCTION cmph_send_msgr_qmi_cache_reset

DESCRIPTION
 Notify QMI to reset IMS pref chg ind cache upon voice domain pref change to CS only and VOLTE off

DEPENDENCIES

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmph_send_msgr_qmi_cache_reset(sys_modem_as_id_e_type as_id);

/*==========================================================================

FUNCTION cmph_is_mms_ongoing

DESCRIPTION
  get if MMS is ongoing on specified sub

RETURN VALUE
  TRUE if MMS ongoing
  FALSE otherwise

DEPENDENCIES
  None
===========================================================================*/
boolean cmph_is_mms_ongoing(
  sys_modem_as_id_e_type asubs_id
);

/*==========================================================================

FUNCTION cmph_get_oprt_mode

DESCRIPTION
  returns the device oprt_mode

RETURN VALUE
  none

DEPENDENCIES
  None

===========================================================================*/
extern sys_oprt_mode_e_type cmph_get_oprt_mode( void );

/*==========================================================================

FUNCTION cmph_get_enable_volte_stack2

DESCRIPTION
  returns the is_enable_volte_stack2

RETURN VALUE
  none

DEPENDENCIES
  None

===========================================================================*/
extern boolean cmph_get_enable_volte_stack2(
  sys_modem_as_id_e_type asubs_id
);

/*===========================================================================

FUNCTION cmph_force_pref_on_the_fly_by_stk_id

DESCRIPTION
  Force preference on the fly by stk_id.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmph_force_pref_on_the_fly_by_stk_id (
  sys_modem_as_id_e_type   asubs_id,
  multimode_stack_e_type   stk,
  cm_mode_pref_e_type      emerg_mode_pref
);

/*===========================================================================

FUNCTION cmph_force_pref_on_the_fly_by_call_type

DESCRIPTION
  Force preference on the fly by call type.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmph_force_pref_on_the_fly_by_call_type (
    cm_call_type_e_type      call_type,
    cm_srv_type_e_type       srv_type,
    cm_mode_pref_e_type      emerg_mode_pref,
    sys_modem_as_id_e_type   asubs_id,
    cm_act_id_type           act_id
);

/*==========================================================================

FUNCTION cmph_ims_dereg_is_sent

DESCRIPTION
  Update ims_dereg_is_sent flag.

RETURN VALUE
  None

DEPENDENCIES
  None

===========================================================================*/
void cmph_ims_dereg_is_sent(
  sys_modem_as_id_e_type  asubs_id,
  boolean                 dereg_is_sent
);

/*==========================================================================

FUNCTION cmph_send_detach_comp

DESCRIPTION
  Send CM_PS_DETACH_BY_MMODE_COMPLETE so that IMS can perform re-reg later.

RETURN VALUE
  None

DEPENDENCIES
  None

===========================================================================*/
void cmph_send_detach_comp(
  sys_modem_as_id_e_type  asubs_id
);

/*===========================================================================
FUNCTION cmph_send_subs_info

DESCRIPTION
  Send the subscription info to CM Clients

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmph_send_subs_info(

     sys_modem_as_id_e_type        asubs_id,
     /* Stack whose subscription Information needs to be sent */

     const cm_client_s_type       *client_ptr
     /* Client who requested this subscription information
      ** If NULL, sent to all registered clients
     */

);

/*===========================================================================

FUNCTION cmph2_check_device_init_status

DESCRIPTION
  This function checks the current RF/GPS init status and returns the proper 
  error value.

RETURN VALUE
  cm_ph_cmd_err_e_type 

===========================================================================*/
cm_ph_cmd_err_e_type cmph2_check_device_init_status(void);

/*===========================================================================

FUNCTION cmph2_proc_device_init_status

DESCRIPTION
  Process RF/gps initialization status update.

RETURN VALUE
  None 

===========================================================================*/
void cmph2_proc_device_init_status(  
  const rf_init_comp_msg_type  *rfm_ind,
  boolean                       oprt_mode_chg
);

/*===========================================================================

FUNCTION cmph2_update_device_init_status

DESCRIPTION
  Update rf/gps init status to cm clients.

RETURN VALUE
  None 

===========================================================================*/
void cmph2_update_device_init_status(  
  cm_ph_info_s_type  *ph_info_ptr
);


/*===========================================================================

FUNCTION cmph_is_device_blocked

DESCRIPTION
  This function returns subscription which is blocked by network due to reason 6

DEPENDENCIES

RETURN VALUE
  sys_modem_as_id_e_type

SIDE EFFECTS
  none

===========================================================================*/

boolean cmph_is_device_blocked(void);

/*===========================================================================

FUNCTION cmph_get_blocked_sub

DESCRIPTION
  This function returns subscription which is blocked by network due to reason 6

DEPENDENCIES

RETURN VALUE
  sys_modem_as_id_e_type

SIDE EFFECTS
  none

===========================================================================*/

sys_modem_as_id_e_type cmph_get_blocked_sub(void);

/*==========================================================================

FUNCTION cmph_get_subs_block_reason

DESCRIPTION
  sets MCFG state for given subs

RETURN VALUE
  none

DEPENDENCIES
  None
===========================================================================*/
cmph_sub_block_reason_e_type cmph_get_subs_block_reason(sys_modem_as_id_e_type asubs_id);

#ifdef FEATURE_MMODE_DUAL_SIM
/*===========================================================================

FUNCTION cmph_process_me_blocked_status_ind

DESCRIPTION
  This function process ME blocked status ind from NAS

DEPENDENCIES

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/

void cmph_process_me_blocked_status_ind( const cm_me_blocked_status_ind_s_type  *me_blocked_status_ind_ptr);
#endif

/*==========================================================================

FUNCTION cmph_get_volte_state

DESCRIPTION
  Return the value for is_volte_on for the specified sub.

RETURN VALUE
  Boolean

DEPENDENCIES
  None

===========================================================================*/
boolean cmph_get_volte_state(
  sys_modem_as_id_e_type  asubs_id
);


/*===========================================================================

FUNCTION cmph_is_periodic_srch_active_in_ll

DESCRIPTION
  This function will determine if there is actual periodic search active in lower layer and CM is 
  waiting for list cnf. 
  If there is a periodic search in CM but timer for mplmn search is still running, the function should 
  return FALSE.
  If there are any other MPLMN search such as one-timer search, CSG search or mmode internal search
  ongoing, the function should return FALSE.

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  Boolean

SIDE EFFECTS
  None

===========================================================================*/
boolean cmph_is_periodic_srch_active_in_ll(
  sys_modem_as_id_e_type    asubs_id
);


/*===========================================================================

FUNCTION cmph_is_high_prio_man_srch

DESCRIPTION
  Determine if the MPLMN search is high priority search.  
  MPLMN search is high priority if it is one-time search.
  MPLMN search is high prority if priority is high for periodic search.

DEPENDENCIES
  None

RETURN VALUE
  Boolean
  
SIDE EFFECTS
  none

===========================================================================*/
boolean cmph_is_high_prio_man_srch(
  sys_periodic_frequency_scan_info_s_type  *pf_scan_info
);

/*===========================================================================

@FUNCTION cmph_ef_rat_and_rat_bal_check

@DESCRIPTION
  TRUE: 
    When rat bal is enabled:
    - if single sim or dual LTE. 
    - if primary subs is Multimode and Active

  FALSE:
  - Otherwise.

@DEPENDENCIES
  None

@SIDE EFFECTS
  None

===========================================================================*/
boolean cmph_is_rat_bal_allowed(
  sys_modem_as_id_e_type  asubs_id
);

/*===========================================================================

@FUNCTION cmph_update_sp_special_priority_mode

@DESCRIPTION
  Indicate to TRM layer if special TRM mapping is needed for MMS or Wifi call activity

  Start Conditions:
  - For MMS/data activity on non DDS sub, start the special mapping on the DDS sub
  - For MO/MT wifi call, start the special mapping on the call sub

  End Condition:
  - For MMS/data activity end on the non DDS sub, end the special mapping on the DDS sub
  - For DDS switch, end the special mapping on the DDS sub
  - For MO/MT wifi call end, end the special mapping on the call sub
  - For MO/MT wifi call handover to LTE, end the special mapping on the call sub

@DEPENDENCIES
  None

@SIDE EFFECTS
  None

===========================================================================*/
void cmph_update_sp_special_priority_mode(
  boolean is_mms_activity,
    /* Indicates if current activity is MMS or Wifi related */

  boolean is_start,
    /* Indicates if the special mapping should be started or stopped */

  sys_modem_as_id_e_type  asubs_id
);

/*===========================================================================

@FUNCTION cmph_send_sp_special_priority_mode

@DESCRIPTION
  Sends the special TRM mapping request to TRM

@DEPENDENCIES
  None

@SIDE EFFECTS
  None

===========================================================================*/
void cmph_send_sp_special_priority_mode(
  boolean                 is_start,
    /* Indicates if the special mapping should be started or stopped */

  sys_modem_as_id_e_type  asubs_id
);

/*==========================================================================

FUNCTION cmph_read_ehplmn_list

DESCRIPTION
  Function that reads EHPLMN list and 

RETURN VALUE
  boolean  Flag indicating whether EHPLMN read
           from the SIM successfully (TRUE) or unsuccessfully(FALSE).

DEPENDENCIES
  None
===========================================================================*/
boolean cmph_read_ehplmn_list(sys_modem_as_id_e_type asubs_id);

/*==========================================================================

FUNCTION cmph_clear_ehplmn_list

DESCRIPTION
  Function that reads EHPLMN list and 

RETURN VALUE
  boolean  Flag indicating whether EHPLMN read
           from the SIM successfully (TRUE) or unsuccessfully(FALSE).

DEPENDENCIES
  None
===========================================================================*/
void cmph_clear_ehplmn_list(sys_modem_as_id_e_type asubs_id);

/*==========================================================================

FUNCTION cmph_search_ehplmn_list

DESCRIPTION
  Function that reads EHPLMN list and 

RETURN VALUE
  boolean  Flag indicating whether EHPLMN read
           from the SIM successfully (TRUE) or unsuccessfully(FALSE).

DEPENDENCIES
  None
===========================================================================*/
boolean cmph_search_ehplmn_list
(
  sys_modem_as_id_e_type asubs_id,

  sys_plmn_id_s_type plmn_id
);

/*===========================================================================

FUNCTION cmph_check_if_clnup_on_non1x_sub

DESCRIPTION
  If the device is currently in Online operating mode and has subscription
  available, cleanup operation might be needed.

DEPENDENCIES
  none

RETURN VALUE
  TRUE: Clean up might be needed.
  FALSE: Cleanup operation wont be needed.

SIDE EFFECTS
  none

===========================================================================*/
boolean                       cmph_check_if_clnup_on_non1x_sub(
  cm_subscription_status_e_type                 gwl_sub_not_avail_status,
  cm_subscription_status_e_type                 cdma_sub_not_avail_status,   
  sys_modem_as_id_e_type                        asubs_id
);


#ifdef FEATURE_CM_LTE
/*===========================================================================

FUNCTION cmph_reg_send_block_plmn_reset_req

DESCRIPTION
  This function sends PLMN block req to NAS with type RESET and cause
  VOICE_NOT_AVAILABLE. NAS will remove all backoff forbidden PLMNs that
  were added with cause VOICE_NOT_AVAILABLE.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void cmph_reg_send_block_plmn_reset_req(

  sys_modem_as_id_e_type            cmd_subs,

  sys_block_plmn_cause_e_type       cause
);
#endif

/*===========================================================================

@FUNCTION cmph_is_trm_in_fallback_state

@DESCRIPTION
  Queries TRM to get the current fallback state for the non-DDS sub.
  Returns TRUE if TRM indicates a valid fallback state.

===========================================================================*/
boolean cmph_is_trm_in_fallback_state(void);

/*===========================================================================

FUNCTION cmph_reset_emerg_call_rej_no_sim

DESCRIPTION
  This function resets emerg call rej no sim.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/

void cmph_reset_emerg_call_rej_no_sim(void);

/*===========================================================================
FUNCTION cmph_map_slot_id_to_asubs_id

DESCRIPTION
  Maps MMGSDI slot id to asubs id.
  
===========================================================================*/
sys_modem_as_id_e_type  cmph_map_slot_id_to_asubs_id(
  mmgsdi_slot_id_enum_type  slot_id
);

/*==========================================================================

FUNCTION cmph_resume_ps_on_non_dds

DESCRIPTION
  Resume PS on non DDS sub if it is needed

RETURN VALUE
  none

DEPENDENCIES
  None

===========================================================================*/
void cmph_resume_ps_on_non_dds(void);
/*===========================================================================

FUNCTION cmph_is_subscription_available

DESCRIPTION
  Checks if subscription is avail or not
  
===========================================================================*/
boolean cmph_is_subscription_available( uint8 new_sub_mask);

/*===========================================================================

FUNCTION cmph_current_mcc_match_with_efs

DESCRIPTION
  Checks the current MCC against MCC list present in the EFS, if current MCC matches with any one 
   of the MCC in the EFS then return TRUE if not return False.
   
DEPENDENCIES
 None

RETURN VALUE
  Boolean.

SIDE EFFECTS
  None.

===========================================================================*/
boolean cmph_current_mcc_match_with_efs (

  sys_mcc_type       current_mcc
  /* Current camped MCCs*/
);


/*===========================================================================

FUNCTION  cmph_is_e911_wlan_to_LTE_handover_enabled

DESCRIPTION
 Read the EFS "e911_wlan_to_lte_handover" and check if the handover from WLAN to LTE is enabled.
 If EFS is set then return the value set in the EFS.

 IF EFS not set, return FALSE.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean cmph_is_e911_wlan_to_LTE_handover_enabled (sys_modem_as_id_e_type asubs_id);



/*===========================================================================

FUNCTION cmph_handle_ra_cap_update_fail_ind

DESCRIPTION
  Sends user_ps_detach_req whenever ra_cap_update_fail_ind is received from NAS
   
DEPENDENCIES
 None

RETURN VALUE
  void.

SIDE EFFECTS
  None.

===========================================================================*/
extern void cmph_handle_ra_cap_update_fail_ind
(
  sys_modem_as_id_e_type asubs_id
);

/*===========================================================================

FUNCTION cmph_check_and_update_sim_change_status

DESCRIPTION
  Checks' and updates the SIM change status.

DEPENDENCIES
  None

RETURN VALUE
  Boolean.

SIDE EFFECTS
  None.

===========================================================================*/
boolean cmph_check_and_update_sim_change_status(
  sys_modem_as_id_e_type asubs_id
);
/*===========================================================================

FUNCTION cmph_prioritize_lte_scan_req

DESCRIPTION

If UE is currently OOS force preferences on main stack to trigger one immediate service request.
If UE has 1x service on main stack , force preferences on stack2 to trigger one immediate BSR.

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void cmph_prioritize_lte_scan_req(sys_modem_as_id_e_type asubs_id);
/*===========================================================================

FUNCTION  cmph_read_gpp_wifi_cellular_scan_timer

DESCRIPTION
 Read the cellular wifi scan timer from nv efs.

DEPENDENCIES


RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmph_read_wifi_cellular_scan_timer (sys_modem_as_id_e_type asubs_id);
/*===========================================================================

FUNCTION  cmph_update_gpp_wifi_cellular_scan_timer

DESCRIPTION
 Update the cellular wifi scan timer to ph ptr on the fly.

DEPENDENCIES


RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/

void cmph_update_gpp_wifi_cellular_scan_timer ( uint16 timer, sys_modem_as_id_e_type asubs_id);

/*===========================================================================

FUNCTION cmph_msim_mt_page_concurrency_init

DESCRIPTION
  Configure if MT page concurrency is allowed.

RETURN VALUE
  None

===========================================================================*/
void  cmph_msim_mt_page_concurrency_init(void);

/*===========================================================================

FUNCTION cmph_msim_unblock_ims_reg_fail_on_non_dds

DESCRIPTION
  Unblock the PLMN blocked for IMS REG Failure

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void cmph_msim_unblock_ims_reg_fail_on_non_dds(
      sys_modem_as_id_e_type asubs_id,
	  sys_voice_domain_pref_e_type old_domain_pref,
	  sys_voice_domain_pref_e_type new_domain_pref
);

/*===========================================================================

FUNCTION cmph_err_fatal_if_ps_srv_domain_on_multiple_subs

DESCRIPTION
  ERROR FATAL if PS is part of service domain preference of multiple subs.

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmph_err_fatal_if_ps_srv_domain_on_multiple_subs(void);


/*===========================================================================

FUNCTION cmph_init_emerg_pending_info

DESCRIPTION
  Initialize emergency pending info.

RETURN VALUE
  none
  
===========================================================================*/
void  cmph_init_emerg_pending_info(void);

/*===========================================================================

FUNCTION cmph_force_emerg_pending

DESCRIPTION
  Process online mode with pending E911 call

RETURN VALUE
  None

===========================================================================*/
void cmph_force_emerg_pending(void);

/*===========================================================================

FUNCTION cmph_is_emerg_pending

DESCRIPTION
  Return if there is a  pending e911 call

RETURN VALUE
  TRUE if in E911 pending state else FALSE
  
===========================================================================*/
boolean cmph_is_emerg_pending (void);

/*===========================================================================

FUNCTION cmph_delete_emerg_pending_obj

DESCRIPTION
  Delete the online pending emergency call orig params.

RETURN VALUE
  none
  
===========================================================================*/
void cmph_delete_emerg_pending_obj(void);

/*===========================================================================

FUNCTION cmph_update_orig_param_emerg_pending

DESCRIPTION
  Process emerg pending state upon updating orig params.

RETURN VALUE
  None
  
===========================================================================*/
void cmph_update_orig_param_emerg_pending(void);

/*===========================================================================

FUNCTION cmph_exit_emerg_pending_st_chk

DESCRIPTION
  Exit pending emergency state if UE changes oprt mode to non ONLINE, or pending state is
  OFF.

RETURN VALUE
  TRUE if this is an online request to exit emegency pending state.  
  
===========================================================================*/
boolean cmph_exit_emerg_pending_st_chk(
  cm_ph_cmd_s_type       *ph_cmd_ptr
);

/*===========================================================================

FUNCTION cmph_exit_emerg_pending_st

DESCRIPTION
  Exit online emergency pending state.

RETURN VALUE
  none
  
===========================================================================*/
void cmph_exit_emerg_pending_st(void);

/*===========================================================================

FUNCTION cmph_oprt_mode_emerg_pending_check

DESCRIPTION
  Check parameter errors for operating mode setting command

RETURN VALUE
  CM_PH_CMD_ERR_NONE if no parameter related errors,
  otherwise specific cm_call_cmd_err_e_type.
  
===========================================================================*/
cm_ph_cmd_err_e_type  cmph_oprt_mode_emerg_pending_check(
  cm_ph_cmd_s_type     *ph_cmd_ptr
);

/*===========================================================================

FUNCTION cmph_oprt_mode_is_dup_check

DESCRIPTION
  Check if oprt_mode command is duplicate

RETURN VALUE
  TRUE if this is a duplicate oprt_mode command.
  
===========================================================================*/
boolean cmph_oprt_mode_is_dup_check(
  cm_ph_cmd_s_type     *ph_cmd_ptr
);

/*===========================================================================

FUNCTION cmph_update_emerg_pending

DESCRIPTION
  Set state for online emergency pending.

RETURN VALUE
  none
  
===========================================================================*/
void  cmph_update_emerg_pending (
  cm_emerg_pending_info_s_type  info
);

/*===========================================================================

FUNCTION cmph_get_emerg_pending_info

DESCRIPTION
  Get the online emergency pending information.

RETURN VALUE
  none
  
===========================================================================*/
cm_emerg_pending_info_s_type  cmph_get_emerg_pending_info(void);

/*===========================================================================

FUNCTION cmph_skip_orig_proc

DESCRIPTION
  Check if orig proc is to skipped for the call id.

RETURN VALUE
  Boolean
  
===========================================================================*/
boolean cmph_skip_orig_proc(
  cmcall_s_type  *call_ptr
);


#endif /* CMPH_H */
