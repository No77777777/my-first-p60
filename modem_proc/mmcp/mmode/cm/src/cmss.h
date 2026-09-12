#ifndef CMSS_H
#define CMSS_H
/*===========================================================================

C A L L   M A N A G E R   S E R V I N G   S Y S T E M   H E A D E R   F I L E

DESCRIPTION
  This header file contains all the definitions necessary to interface
  with CMSS.C

Copyright (c) 1998 - 2013 by Qualcomm Technologies INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/mmcp.mpss/7.9.0/mmode/cm/src/cmss.h#3 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/08/14   fj        Add support for DSDX L+G
07/15/13   ss      FR1860 - T/G on DSDS
03/26/13   fj        Hold no srv or limited srv for the dedicated time based on RAT.
10/09/12   skk     Implemented fetch based mechanism for 3GPP RSSI
09/18/12   qf      Reset SSAC backoff timer if G/W full service is acquired
09/17/12   skk     Do not send internal service lost or limited service during uptime.
04/18/12   gm      Simplified API to retrieve SS info for all tech.
04/02/12   gm      We actually print country code, not MCC.
03/15/12   vk      Added domain selection info CMSS.
03/13/12   vk      Removal of feature flags added for CELL_DCH feature.
02/20/12   vk      Added feature to get DCH cell info.
11/16/11   jh      Replace cmcall CTXT_TRANSFER event to cmss event
08/12/11   vk      Throtling time Random delay range changed from (0-54) to (0-15)
07/24/11   aj      SC SVLTE 1.0 merge to mainline
05/18/11   cl      Added support for GW throttle filtering based on regions.
04/14/11   xs      Added mew API cmss_inform_target_rat()
02/17/11   cl      Add support FEATURE_MMODE_REMOVE_1X for FUSION-MDM
01/26/11   rm      CMI-4: Inclusion of sd_i.h
01/19/11   fj      Add idm_timer_value in cmss_s_type,
                   add default value for EFS IDM timer item.
12/23/10   sv      Integrate DSDS feature
12/14/10   xs      Added support for call throttle parameter configurable in EFS for JCDMA
11/11/10   rm      Adding WLAN security info interface
10/04/10   cl      Adding support for uplink/downlink lte cell info indication
10/01/10   ak      Merged the Fusion code from Dev branch.
08/18/10   fj      Add cmss_update_lte_cell_info().
06/28/10   ak      Added EOOS support
06/18/10   aj      Add support to throttle HDR systems based on subnet id
03/17/10   fj      Added support for LTE RSSI indication handling.
02/01/10   aj      Add support for DCTM 4.0
01/05/10   pm      Removing UOOS
12/22/08   fj      Enabled some APIs for LTE.
08/25/09   rn       MM reject 17 makes data card move to Automatic mode
06/10/09   ks      Added default values of camped cell info parameters
05/27/09   aj      Adding support for updating clients with cdma local time
03/01/09   sv      Lint cleanup.
11/12/08   ks      Fixing Compilation error
10/21/08   ks      Update default threshold value for sir and pathloss and
                   add masks for RSSI event
09/17/08   ks      When OOS in CELL_FACH, UE needs to follow UOOS scanning
                   pattern to re-acquire service
09/16/08   pk      Added support for WLAN UOoS
09/05/08   sv      Memory Reduction for ULC
09/04/08   ks      Remove info for Neighbors Cell IDs
07/18/08   ks      Added new info for Current Operator's Cell ID and
                   Neighbors Cell IDs
06/11/08   sv      Exposing cmss_send_ss_info for Power Collapse
                   buffered events notifications.
02/15/08   ks      Adding SIR, Pathloss Implementation
01/24/08   ks      Display of UE signal strength indication
01/03/08   ka      Adding handset based plus code dialing feature.
12/13/07   sk/ak   Added DCM.
12/07/07   ra      Added support for IMS VCC in HOMER files
08/12/07   jq      Fixed DCTM timeline issue under JCDMA feature.
06/08/07   cl      Fix CR#: 119770 - cmss_orig_thr_tbl_entry_info_put should
                   pass parameter by pointer
05/15/07   rp      Adding support for ECIO for GW
04/15/07   rp      Added last_system_reported
03/30/07   jqi     Changed the DCTM timeline to 0,0,180 under JCDMA feature.
03/09/07   sk      RAM usage reduction.
11/16/06   pk      Lint Fixes
11/11/06   pk      Added support for HYBR_GW and UMTS -> 1X handover cont...
11/09/06   pk      Added support for HYBR_GW and UMTS -> 1X handover cont...
04/06/06   sk      Removed the following fields from cmss_s_type:
                   - hybr_hdr_bcmcs_no_srv_uptime
                   - ss_main_bcmcs_no_srv_uptime
                   - ss_main_bcmcs_srv_status
                   - hybr_hdr_bcmcs_srv_status
03/10/06   pk      Changed the return type of cmss_get_ss_wlan_info_change to
                   uint64
01/06/06   ka      Adding Release 5 changes including emerg call list,
                   traffic flow template for PDP modify and protocol config
                   options for some of the GW PS call reports.
01/06/06   ic      Lint cleanup
11/08/05   sk      Separated CM reports and commands.
07/26/05   pk      DCTM modifications, changed the backoff time line from
                   1,2,4,8 to 0,0,1,2,8,15. Ignoring NID for DCTM, a system
                   is uniquely identified by SID,PZID.
07/11/05   pk      Modified cmss_check_if_orig_thr_true() added packet_zone
                   as a additional parameter
06/06/05   pk      Added structures and function definitions for Data Throttle
                   Manager API's
05/19/05   ka      Merging support to inform clients about call ciphering.
02/01/05   dk      Added WLAN Support.
01/10/05   dk      Added cmss_sys_id_match().
10/22/04   jqi     Removed redundent RSSI type from cmss_report_rssi().
10/19/04   jqi     Sent the data available request upon the ps call orig
                   when the data is suspended.
09/03/04   ka      Added service throttling for HDR
08/27/04   jqi     Added ps dormant optimization.
03/02/04   ic      Added BCMCS support (FEATURE_BCMCS)
02/10/04   jq      Removed cmss_proc_call_connected prototype due to recursive
                   including of cmcall.h
02/06/04   ws      Initial jaguar/mainline merge.
10/21/03   ws      Fixed featurization of cmss_timer_proc
11/19/03   ic      Comment clean up.
11/04/03   ic      Moved cmss_IDM_timer_start_check() to forward declarations
                   section in the .c file
10/24/03   ic      Added cmss_IDM_timer_start_check()
10/22/03   ic/jai  Fixes in IDM code
09/22/03   vt      Generated IDM event on voice call connect.
07/10/03   jqi     Added Extended PRL support
                   Removed FEATURE_IS683C_PRL
02/18/03   vt      Fixed problems to build for HDR.
01/21/03   ws      Updated copyright information for 2003
05/05/02   vt      Fixed compile problem with new mm cm.h
10/18/01   PRK     Added changes for WCDMA/GSM mode.
                     Re-added Serving System group and moved srv_state, roam_status,
                     rssi from phone group to serving system group (FEATURE_NEWSS).
01/18/02   ic      Merged MSM_CM.05.03.33 on top of tip CCS CM
                   Diffed MSM_CM.05.03.32 with MSM_CM.05.03.33 and only the changes
                   between those two were merged with CCS CM
12/13/01   ic      Merged MSM_CM.05.03.32 onto SVD CM tip.
                   CCS CM code started from CM VU MSM_CM.05.03.25 + WCDMA/GSM
                   stuff from SIMBA team merged on top of it.
                   We diff'ed MSM_CM.05.03.25 with MSM_CM.05.03.32 (CM VU in N*2165)
                   that we needed to merge with and changes between those two VUs
                   were merged onto CCS CM tip.
10/23/01   VT/IC   Baseline for CCS
09/09/99   SH      Added p_rev and is_registered to support data application
05/31/99   SH      Added CM 2.0 fetures
01/07/99   RC      Changes per ARM compiler.
11/24/98   RC      Initial release.

===========================================================================*/

/**--------------------------------------------------------------------------
** Includes
** --------------------------------------------------------------------------
*/
#include "queue.h"

#include "mmcp_variation.h"
#include "comdef.h"    /* Definition for basic types and macros */
#include "cm.h"        /* External interface to cm.c */
#include "cm_v.h"
#include "cmi.h"       /* Internal interface to cm.c */

#include "cmnv.h"      /* Includes NV services */

#include "sd.h"
#include "sd_v.h"
#include "sd_i.h"

#if (defined(FEATURE_GSM) && defined(FEATURE_GSM_RR_CELL_CALLBACK))
#include "sys.h"
#include "sys_v.h"
#ifdef FEATURE_CM_NEW_RR_CELL_INFO_INTERFACE
#include "geran_eng_mode_read_api.h"
#else
#include "rr_cb_if.h"
#endif /*FEATURE_CM_NEW_RR_CELL_INFO_INTERFACE */
#endif
#include "lte_rrc_ext_msg.h"

#ifdef FEATURE_HDR_HANDOFF
#define CMSS_IDLE_DIGITAL_MODE_DELAY_TIME  (CMPH_HOLD_MODE_PREF_TIME + 1)
/* At end of call, not report idle digital mode change event
** for this amount of time */


#define CMSS_IDLE_DIGITAL_MODE_HUP_DELAY_TIME  60
/* Handup delay timer
*/
#endif /* FEATURE_HDR_HANDOFF */

#define CMSS_MAX_SUBNET_MASK_LEN               128
/* Max/default value of HDR subnet mask length
*/
#ifdef FEATURE_CALL_THROTTLE

#define CMSS_HARD_FAIL_THROTTLE_TIME           3600
/* Default value of hard failure throttle time.
** This value is used if NV_HARD_FAIL_THR_TIME_I is not set
*/
#define CMSS_ORIG_THR_TABLE_MAX_ENTRIES 20
/* The size of the throttle table
*/
#else
#define CMSS_ORIG_THR_TABLE_MAX_ENTRIES 1
/* The size of the throttle table when feature is not enabled.
** To reduce RAM usage.
*/
#endif

#define CMSS_NO_SRV_UPTIME_NONE             (dword) (-1)

#define CMSS_INFORM_SRV_CHANGE_INTER_NONE   (dword) (-1)

#define CMSS_LTE_DO_IRAT_DURATION     3   /* seconds */
/* Max duration to receive overhead message from the time Auto
** activation indication is received for LTE to Do redirection */

#define CMSS_ORIG_THR_MAX_TIMER_INDEX_DFT     7
/* Orig throttle timer slots
*/

#define CMSS_ORIG_THR_RND_INT            1.667
/* Randomizing factor for the orig throttle time
*/

#define ROUNDOFF(value, to)\
((value) % (to)) >= ((to)/2) ?  (((value) / (to)) + 1) : ((value)/(to))


#define CMSS_SID_MCC_TBL_MAX             NV_SID_TO_MCC_TBL_LEN
/* Max Sid to MCC table size
*/

#define CMSS_INVALID_CELL_ID_INFO        0xFFFFFFFF
/* value for Invalid cell_id info
*/


#define CDMA_TIME_PTR( xx_info,asubs_id ) (&(xx_info->sub_prop[asubs_id]->stack_info[MM_STACK_0].gen_info->mode_info.cdma_info.time_info))
/* Pointer to cdma time info structure obtained from structure of type
** cm_mm_ss_info_s_type
*/

#define SS_CDMA_TIME_PTR( ss_ptr,asubs_id ) (&(ss_ptr->info.sub_prop[asubs_id]->stack_info[MM_STACK_0].gen_info->mode_info.cdma_info.time_info))
/* Pointer to cdma time info structure obtained from SS object pointer
*/

#define CMSS_INVALID_LAC_INFO            0xFFFF
/* value for Invalid LAC info
*/
#define CMSS_INVALID_RAC_OR_MME_CODE_INFO            0xFF
   /* value for Invalid LAC info
   */
#define CMSS_INVALID_TAC_INFO            0xFFFE
   /* value for Invalid LAC info
   */

#define CMSS_INVALID_PSC_INFO            0xFFFF
/* value for Invalid PSC info
*/

#define CMSS_INVALID_UARFCN_DL_INFO      0xFFFF
/* value for Invalid UARFCN_DL info
*/

#define CMSS_INVALID_UARFCN_UL_INFO      0xFFFF
/* value for Invalid UARFCN_UL info
*/

#define CMSS_INVALID_EARFCN_DL_INFO      0xFFFFFFFF;
/* value for invalid Downlink Frequency
*/

#define CMSS_INVALID_EARFCN_UL_INFO      0xFFFFFFFF;
/* value for invalid Uplink Frequency
*/

#define CMSS_IDM_HANDDOWN_TIME_DEFAULT   (dword)(15);
/* "Handdown_timer" time out time, set to 15 seconds.
*/


/* Bit mask to indicate DCH CELL_INFO_REQ is sent to RRC and waiting for response */
#define CMSS_IS_DCH_CELL_INFO_REQ          BM(0)

/* Bit mask to indicate RRC provided DCH CELL INFO */
#define CMSS_IS_DCH_CELL_INFO_RES          BM(1)

/* Max duration to hold 1x service reported to clients */
#ifdef FEATURE_CUST_1
#define CMSS_CDMA_HOLD_SRV_TIMER             3 /* seconds */
#else
#define CMSS_CDMA_HOLD_SRV_TIMER             0
#endif

/**--------------------------------------------------------------------------
** Enums
** --------------------------------------------------------------------------
*/

/* Enumeration of serving system info update action
*/
typedef enum cmss_update_act_e
{

  CMSS_INFO_UPDATE_ACT_NONE=-1,            /* FOR INTERNAL USE OF CM ONLY! */

  CMSS_INFO_UPDATE_N_TO_D,
  CMSS_INFO_UPDATE_A_TO_D,
  CMSS_INFO_UPDATE_N_TO_A,
  CMSS_INFO_UPDATE_D_TO_A,

  CMSS_INFO_UPDATE_A_TO_A,
  CMSS_INFO_UPDATE_D_TO_D,

  CMSS_INFO_UPDATE_A_TO_N,
  CMSS_INFO_UPDATE_D_TO_N,

  CMSS_INFO_UPDATE_ACT_MAX                 /* FOR INTERNAL USE OF CM ONLY! */

} cmss_update_act_e_type;


/**--------------------------------------------------------------------------
** Datatypes
** --------------------------------------------------------------------------
*/

/* Internal SRV information of all the stacks
*/

typedef struct cmss_true_stack_info
{
  sd_si_info_s_type   *si_info[MAX_STACKS];

} cmss_true_stack_info_s_type;

typedef struct cmss_true_info_s
{
  cmss_true_stack_info_s_type *sub_info[MAX_SIMS];

} cmss_true_info_s_type;


/* Service indication uptimer type
*/
typedef struct cmss_srv_ind_uptimer_s
{
  int   no_srv_uptimer;

  int   srv_uptimer;

} cmss_srv_ind_uptimer_s_type;

/* Service indication uptimers of all the rats
*/
typedef struct cmss_rat_srv_ind_uptimers_s
{
  boolean                       is_valid;
  cmss_srv_ind_uptimer_s_type   cdma_uptimers;
  cmss_srv_ind_uptimer_s_type   hdr_uptimers;
  cmss_srv_ind_uptimer_s_type   gsm_uptimers;
  cmss_srv_ind_uptimer_s_type   wcdma_uptimers;
  cmss_srv_ind_uptimer_s_type   tds_uptimers;
  cmss_srv_ind_uptimer_s_type   lte_uptimers;

} cmss_rat_srv_ind_uptimers_s_type;

/* Throttle table entry type
*/

typedef struct cmss_orig_thr_table_entry_s
{

  cm_ss_orig_thr_call_status_s_type     orig_call_info;
  /* The call information
  */

  dword                                 thr_uptime;
  /* The time until throttling is in effect
  */

  unsigned int                          idx;
  /* The index in the throttle_time table
  ** Used internally by CM to keep track of the next throttle time
  ** increment
  */
} cmss_orig_thr_table_entry_s_type;


typedef struct
{
  q_link_type               link;
  /* Link element to link clients into a client-list */
  /* this element has to be the first in this structure
  ** because the address of link is also used as
  ** the "self pointer" (to save memory space by eliminating
  ** self_ptr in q_link_type) to access other element in this
  ** data structure */

  cmss_orig_thr_table_entry_s_type cmss_orig_thr_tbl;
} cmss_orig_thr_table_q_type;

typedef struct cmss_orig_thr_table_s
{
  unsigned int   num_valid_entries;
  /* Number of valid entries in the table
  */

  q_type          cmss_orig_thr_q;
  /* The origination throttle q
   */

} cmss_orig_thr_table_s_type;

typedef enum
{
  CM_SMREG_PS_SIG_REL_REQ_CAUSE_MIN,
  CM_SMREG_UE_INITIATED_DORMANCY,
  CM_SMREG_DUAL_STANDBY_CS_MO_CALL,
  CM_SMREG_DUAL_TRIPLE_STANDBY_CHANGE,
  CM_SMREG_PS_SIG_REL_REQ_CAUSE_MAX
} cm_smreg_ps_signalling_rel_req_cause_T;


/* ------------------------------------------------------------------------ */
typedef struct cm_mm_ss_gpp_signal_info_s
{

  int16                      pathloss;
  /**< Pathloss is applicable only in connected mode (otherwise 0xFF). */

  int16                      sir;
  /**< SIR is applicable and valid (otherwise 0xFF) only in WCDMA
       connected mode when the Dedicated Physical Control Channel
       (DPCCH) is active.  */

  int16                      rssi2;
  /**< RSSI -ve value, coming from the Radio Resource Control (RRC) layer. */

  int16                      rscp;
  /**< Range of Received Signal Code Power (RSCP) is
       from -28 to -121 dBm. */

  int16                      gw_ecio;
  /**< Ec/Io for WCDMA only. Ec/Io is measured in -0.5 dB. WCDMA L1 sends
       positive values in 0.5dB increments to RRC. For GSM, it is defaulted
       to CMSS_ECIO_VALUE_INVALID_SIGNAL. Clients must check for
       CM_API_GW_ECIO to use this field. */

  cm_cqi_info_s_type      cqi_info;
  /**< CQI information. Applicable for WCDMA only. */

  int16                      rsrp;
  /**< Current Reference Signal Received Power (RSRP) in dBm as measured
       by L1. The range is -44 to -140 dBm. */

  int8                       rsrq;
  /**< Current Reference Signal Receive Quality (RSRQ) as measured
       by L1. Quantities are in dB. The range is -20 to -3 dB. */

  sys_lac_type               lac;
  /**< LAC information for GSM. */

  sys_lac_type               tac;
  /**< TAC information for LTE. */

  uint8                      rac_or_mme_code;
  /**< RAC or Mobility Management Entity (MME) code for GW or LTE. */

} cm_mm_ss_gpp_signal_info_s_type;

typedef struct cm_mm_ss_info_3gpp_stack_info_s
{

  sys_srv_status_e_type      true_srv_status;
  /**< True service status (LIMITED/LIMITED_REGIONAL) of the
       system. Mask used is CM_SS_TRUE_SRV_STATUS_MASK. */

  sys_srv_domain_e_type      srv_domain;
  /**< Service domain registered on the system. Mask used is
       CM_SS_SRV_DOMAIN_MASK. */

  sys_srv_domain_e_type      srv_capability;
  /**< System's service capability. Mask used is
       CM_SS_SRV_CAPABILITY_MASK. */

  boolean                    is_sys_prl_match;
  /**< Indicates if the system is in PRL. Mask used is
       CM_SS_SYS_PRL_MATCH_MASK. */

  boolean                    is_sys_forbidden;
  /**< Indicates whether the system is forbidden. Mask used is
       CM_SS_SYS_FORBIDDEN_MASK. */

  cm_mm_ss_gpp_signal_info_s_type       gpp_signal_info;
  /** <Indicates 3GPP system signal info */

  boolean                    ecio_for_display;
  /**< TRUE if User Equipment (UE) is in data call. */

  boolean                    ps_data_suspend;
  /**< Indicates whether the current PS data service has been suspended. Mask
       used is CM_SS_PS_DATA_SUSPEND_MASK.  */

  cm_cell_srv_ind_s_type      cell_srv_ind;
  /**< Structure to hold information about services in the current cell. The
       information is tied to CM_SS_SRV_IND_MASK. Clients must check for
       CM_API_CELL_SUPP_SERVICES before using this structure. */

  cm_reg_reject_info_s_type reg_reject_info;
  /**< Registration rejection information.  */

  sys_srv_domain_e_type      cipher_domain;
  /**< Ciphering on CS or PS domain or both. Can be used only if
       CM_CIPHERING_IND_SUPP is defined. */

  sys_cell_info_s_type      cell_info;
  /**< CELL_INFO of the selected system. Mask used is
       CM_SS_CELL_INFO_MASK. */

  sys_lte_rrc_data_inact_timer_info_s_type lte_rrc_data_inact_timer_info;
   /**< Inactivity timer Info Event */

} cm_mm_ss_info_3gpp_stack_info_s_type;

typedef struct cm_mm_ss_common_signal_info_s
{

  uint16                     rssi;
  /**< RSSI in positive dBm. 75 means -75 dBm. The ranges are as follows:
       - AMPS:      -110 to -89 dBm.
       - 800 CDMA:  -105 to -90 dBm.
       - 1900 CDMA: -108 to -93 dBm.
       - GSM/WCDMA: -105 to -60 dBm.
       - TD-SCDMA:  -120 to -25 dBm. */

  int16                      ecio;
  /**< Ec/Io in negative 0.5 dBm. 63 means -31.5 dBm. */

  int                        io;
  /**< IO field is valid only when in HDR mode-only operation. */

  byte                       sinr;
  /**< Signal-to-Interface plus Noise Ratio (SINR)  SINR is applicable
       when sys_mode = HDR or LTE

       For HDR SINR: range is from 0 to 8,where Level 8 represents
                    the highest SINR.
       For LTE SINR: 10xdB values which are from -200 to 300
                    are mapped to range 0 - 250. -20 db maps to 0,
                    -19.8 db maps to 1, -19.6 maps to 2 ....
                    30 db maps to 250 */

} cm_mm_ss_common_signal_info_s_type;

typedef struct cm_mm_ss_info_gen_stack_info_s
{

  uint64                     changed_fields;
  /**< Bitmask indicating which of the following serving system fields
       were changed since the last notification. Refer to the define
       CM_SS_*_MASK. */

  uint64                     changed_fields2;
  /**< Bitmask indicating which of the following serving system fields
       were changed since the last notification. Refer to the define
       CM_SS_*_MASK.This field is extension to changes_fields */

  sys_srv_status_e_type      srv_status;
  /**< Service status (SERVICE/NO SERVICE) of the system. Mask used is
       CM_SS_SRV_STATUS_MASK. */

  sys_srv_status_e_type      prev_srv_status;
  /**< Previous Service status (SERVICE/NO SERVICE) of the system. */

  sys_sys_mode_e_type        sys_mode;
  /**< System's mode. Mask used is CM_SS_SYS_MODE_MASK. */

  sys_srv_domain_e_type      srv_domain;
  /**< Service domain registered on the system. Mask used is
       CM_SS_SRV_DOMAIN_MASK. */

  cm_ss_info_u_type          mode_info;
  /**< Mode-specific Serving System information. */

  sys_roam_status_e_type     roam_status;
  /**< Current roaming status. Mask used is
       CM_SS_ROAM_STATUS_MASK. */

  sys_sys_id_s_type          sys_id;
  /**< ID of the selected system. Mask used is CM_SS_SYS_ID_MASK. */

  cm_mm_ss_common_signal_info_s_type   common_sig_info;
  /** <Indicates common system signal info */

  sys_band_class_e_type       active_band;
  /**< Clients must check for CM_API_EMERGENCY_NUM_LIST before
       using this structure. */

  sys_channel_num_type        active_channel;
  /**< Report the active channel of the current serving system. */

  boolean                    is_stack_operational;
  /**< Operating status for a subscription.\ Decided by selected dual
       standby preference. */

  sys_lte_cs_capability_e_type lte_cs_capability;

  boolean                                  is_mm_info_backed_up_during_switch;
  /* Indicates whether LTE mminfo is backed up into prev mminfo during SRLTE -><- CSFB switches */


} cm_mm_ss_info_gen_stack_info_s_type;

typedef struct cm_mm_ss_info_3gpp2_stack_info_s
{

  cm_ss_orig_thr_info_s_type gpp2_thr_status;
  /**< DCTM status; indicates the current throttle status of the
       system. */

  uint16             gpp2_ota_mcc;
  /* CM_SS_EVT_XX_MCC_MASK                  changed_fields2*/

  boolean                  is_3gpp2_custom_home;
  /* Indicates if the current 3GPP2 system roam indicator maps to custom_home
   */

  /* FEATURE_BCMCS
     These fields specify whether lower layer
     supports broadcast/multicast as per
     High Rate Broadcast-Multicast Packet data Air
     Interface Specification C.S0054 and
     BCMCS Framework Draft Document 0.1.3 XP0019.
  */

  boolean                    ss_bcmcs_srv_supported;
  /**< Specifies whether stack is configured to support BCMCS service. Mask
       used is CM_SS_XX_BCMCS_SRV_SUPPORTED_MASK. */

  sys_bcmcs_srv_status_e_type ss_bcmcs_srv_status;
  /**< Specifies whether BCMCS service is available on stack. Mask used is
       CM_SS_XX_BCMCS_SRV_STATUS_MASK. */

} cm_mm_ss_info_3gpp2_stack_info_s_type;

typedef struct cm_mm_ss_sds_info
{

  boolean                     voice_support_on_lte;
  /**< Indicates the voice support status on LTE. */

  boolean                        lte_ims_voice_avail;
  /**< Indicate the IMS voice support status on LTE */

  sys_ims_lte_emerg_supp_e_type  lte_ims_emerg_avail;
  /**< Indicate the IMS voice support status on LTE if UE has
  ** acquired LTE service */

  sys_cm_domain_sel_domain_e_type  voice_domain;
  /**< Indicates selected voice domain on LTE */

  sys_cm_domain_sel_domain_e_type  sms_domain;
  /**< Indicates selected sms domain on LTE */

  sys_lte_cs_capability_e_type lte_cs_capability;

} cm_mm_ss_sds_info_s_type;

typedef struct cm_mm_ss_info_gen_sub_info_s
{

  uint64                     signal_strength_changed_fields;
  /**< Bitmask indicating which of the following signal strength fields
       were changed since the last notification. Refer to the define
       CM_SS_*_MASK. */

  cm_psist_e_type            psist;
  /**< PSIST of the system. */

  boolean                    is_csfb_call_active;
  /**< Indicates whether service is acquired because of CSFB call.
       Mask used is CM_SS_CSFB_CALL_ACTIVE_MASK.  */

  dword                      curr_uptime;
  /**< Current up time. */

  sys_roam_status_e_type     def_roam_ind;
  /**< Default roaming indicator from the PRL. */

  sys_eutra_status_enum_type eutra_detect_status;
  /**< Indicates the status of the EUTRA detection. */

  cm_ipapp_info_s_type       ipapp_info;
  /**< Registration information of the application. Clients must check for
       CM_API_VIDEOSHARE before using this structure type. */

  cm_gw_gan_info_s_type       gan_info;
  /**< GAN controller information. */

  sys_sys_mode_e_type         irat_ctxt_target_rat;
  /**< Indicates the target rat when 3GPP IRAT context transfer happens */

  sys_reg_domain_e_type    available_plmn_type;
  /** < Indicates plmn type in CAMP only mode (applicable for 3GPP RATs only) */

  cm_mm_ss_sds_info_s_type     dom_sel_info;
  /** < Domain selection related info */

  sys_cell_broadcast_info_s_type   cell_bc_info;
  /**< Cell broadcast capability info of main stack. */

  cm_emerg_num_list_s_type                emerg_num_list ;
  /*NO MASK*/

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  **                     eMBMS INFO
  **- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  sys_embms_info_s_type          embms_info;
  /** < Indicates the eMBMS info status (only available on LTE) */

  sys_domain_access_bar_s_type  cell_access_info;
  /**< Cell access information */

  cm_client_id_type          requesting_client_id;
  /**< Requesting client. */

  uint16                   sys_sel_pref_req_id;
  /**< System Selection Preferences Request Id */

} cm_mm_ss_info_gen_sub_info_s_type;

typedef struct cm_mm_ss_cdma_signal_info_s
{

  uint16                      cdma_reg_prd;
  /**< Indicates the registration period after the CDMA system is acquired
  ** When CDMA registration period is not valid (-1) is sent
  */

  uint16                      frame_err_rate;
  /**< Value range from 1 to 800. 0xFFFF is an invalid value. */


  uint16                          height;
  /*CM_SS_EVT_POSITION_INFO_MASK      changed_fields2 */

  uint8                           horiz_uncertainty;
  /*CM_SS_EVT_POSITION_INFO_MASK          changed_fields2 */

  uint8                           vert_uncertainty;
  /*CM_SS_EVT_POSITION_INFO_MASK          changed_fields2 */

} cm_mm_ss_cdma_signal_info_s_type;

typedef struct cm_mm_ss_hdr_signal_info_s
{

  sys_hdr_location_notification_status_e_type hdr_ln_status;
  /**< HDR LN status. */

  uint16                    hdr_packet_err_rate;
  /**< Value range from 1 to 800. 0xFFFF is an invalid value. */

  sys_personality_e_type     hdr_personality;
  /**< HDR personality information. Mask used is CM_SS_HDR_PERSONALITY_MASK. */

  sys_base_lat_type      latitude;
  /* CM_SS_EVT_HDR_BASE_STATION_PARMS_MASK     changed_fields2 */

  sys_base_long_type         longitude;
  /* CM_SS_EVT_HDR_BASE_STATION_PARMS_MASK     changed_fields2*/

  uint8              system_type;
  /* CM_SS_EVT_HDR_SYS_TYPE_MASK         changed_fields2*/

} cm_mm_ss_hdr_signal_info_s_type;

typedef struct cm_mm_ss_hpcd_info_s
{

  cm_country_code_type        current_mcc;
  /**< Carries the country code of the current serving country. Associated
       with CM_SS_SRV_STATUS_MASK. Clients must check for CM_API_PLUS_DIALING
       before using this structure. */

  cm_country_code_type        home_mcc;
  /**< Carries the home country code. Associated with
       CM_SS_SRV_STATUS_MASK. Clients must check for
       CM_API_PLUS_DIALING before using this structure. */

  cm_hs_based_plus_dial_e_type  plus_dial_setting;
  /**< Carries the NV setting corresponding to plus dialing. Associated
       with CM_SS_SRV_STATUS_MASK. Clients must check for CM_API_PLUS_DIALING
       before using this structure. */

} cm_mm_ss_hpcd_info_s_type;

typedef struct cm_mm_ss_hdr_gen_info_s
{

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  **                     BCMCS INFO
  **- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  sys_active_prot_e_type     hdr_active_prot;
  /**< HDR active protocol revision information. Mask used is
       CM_SS_HDR_ACTIVE_PROT_MASK. Must define CM_API_HDR_ACTIVE_PROT before
       accessing this field. */

  cm_ss_meas_resp_s_type      meas_resp;
  /**< Indicates the response to the measurement request made by the
       connection manager. */

  uint8                       subnet_mask_len;
  /* The subnet mask length of the currently acquired HDR system if any
  */

  sys_plmn_mcc_list             prl_hdr_mcc;
  /* CM_SS_EVT_PRL_HDR_MCC_MASK                changed_fields_subs
     HDR MCC list from eqprl
  */

} cm_mm_ss_hdr_gen_info_s_type;

typedef struct cm_mm_ss_info_3gpp2_sub_info_s
{

  cm_mm_ss_cdma_signal_info_s_type    cdma_signal_info;

  cm_mm_ss_hdr_signal_info_s_type     hdr_signal_info;

  cm_mm_ss_hdr_gen_info_s_type        hdr_gen_info;

  cm_mm_ss_hpcd_info_s_type   hpcd_info;

  boolean                   is_colloc;
  /**< Indicates whether the system is colocated. Mask used is
       CM_SS_SYS_ID_MASK. */

  uint16                     geo_sys_idx;
  /**< The system table index referencing the beginning of the geo in which
  ** the current serving system is present.
  ** When sys idx is not known 0xFFFF is used
  */

  sys_sys_mode_e_type        prev_idle_digital_mode;
  /**< Previous idle digital mode (FEATURE_HDR_HANDOFF). Mask used is
       CM_SS_IDLE_DIGITAL_MODE_MASK. */

  sys_sys_mode_e_type        cur_idle_digital_mode;
  /**< Current idle digital mode (FEATURE_HDR_HANDOFF). Mask used is
       CM_SS_IDLE_DIGITAL_MODE_MASK. */

  boolean                   bsr_in_progress;
  /**< Indicates if BSR is in progress or not. */

  sys_simultaneous_mode_e_type simultaneous_mode;
  /**< Simultaneous; used to differentiate between WX and non-WX modes. Mask
       used is CM_SS_SRV_IND_MASK. */

  sys_plmn_mcc_list             prl_3gpp2_mcc;
  /* CM_SS_EVT_PRL_3GPP2_MCC_MASK                changed_fields_subs
     3GPP2 MCC list from eqprl
  */

  sys_sys_mode_e_type         target_rat;
  /**< Indicate the target RAT during DO to LTE reselection */

} cm_mm_ss_info_3gpp2_sub_info_s_type;

typedef struct cm_mm_ss_info_sub_level_info_s
{

  cm_mm_ss_info_gen_sub_info_s_type     gen_sub_info; // need for each sub

  cm_mm_ss_info_3gpp2_sub_info_s_type   gpp2_sub_info; // need for each sub

} cm_mm_ss_info_sub_level_info_s_type;

typedef struct cm_mm_ss_info_stack_level_info_s
{
//for stack 0, we need all 3 all the time
  cm_mm_ss_info_3gpp_stack_info_s_type  *gpp_info; //need 1 if gw avail, 2 if 1x and gw avail, 1 if 1x avail, 1 if none

  cm_mm_ss_info_gen_stack_info_s_type   *gen_info; //need 1 if gw avail, 3 if 1x and gw avail, 2 if 1x avail, 1 if none

  cm_mm_ss_info_3gpp2_stack_info_s_type  *gpp2_info; //need 2 if 1x sub avail, 1 if none

} cm_mm_ss_info_stack_level_info_s_type;

typedef struct cm_mm_ss_info_sub_prop_s
{

  cm_mm_ss_info_sub_level_info_s_type    sub_info;

  cm_mm_ss_info_stack_level_info_s_type  stack_info[3];

} cm_mm_ss_info_sub_prop_s_type;

typedef struct cm_mm_ss_info_dev_prop_s
{

  sys_msm_id_e_type          msm_id;
  /**< Identifies the MSM that generated this event. */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  **                     CSG (femto) information
  **- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  sys_csg_info_s_type      csg_info;

} cm_mm_ss_info_dev_prop_s_type;

/** @addtogroup SERVSYSOBJ
    @{ */

/** @brief Type to hold serving system information. When notified of serving
    system events, clients are given a reference to such a structure.
*/
typedef struct cm_mm_ss_info_s
{

  cm_mm_ss_info_sub_prop_s_type          *sub_prop[MAX_SIMS];

  cm_mm_ss_info_dev_prop_s_type           device_prop;

} cm_mm_ss_info_s_type;

typedef struct cmss_3gpp_signal_info_s
{

  uint8                       bit_err_rate;
  /* Bit Error Rate / RX QUAL in GSM
  */

  uint16                      gw_rssi;
  /* Indicate the GSM/WCDMA RSSI value. Only valide for GW mode.
  ** Updated when there is RSSI report from GW stack.
  */

  int16                       gw_ecio;
  /* Indicate the WCDMA ecio value. Only valide for WCDMA mode.
  ** Updated when there is ECIO report from GW stack.
  */

  int16                       gw_pathloss;
  /* Pathloss is applicable only in Connected mode (otherwise 0xFF)
  */
  byte                        gw_cqi;
  /*< Indicate teh WCDMA CQI value. Only valide for WCDMA mode.
  **  Updated when there is CQI report from GW stack.
  */

  uint16                      gw_cqi_num_samples;
  /*< Number of average CQI samples.
  */

  int16                       gw_sir;
  /* SIR applicable and valid (otherwise 0xFF) only in WCDMA
  ** connected mode when DPCCH is active
  */


  int16                       gw_rscp;
  /* Signal strength in RSSI, change for WCDMA only (otherwise 0xFF)
  */

} cmss_3gpp_signal_info_s_type;

typedef struct cmss_misc_3gpp_stack_info_s
{

  sys_div_duplex_e_type         div_duplex;
  /* Indicate the division duplex type - None,FDD, or TDD
  **  This info is only for LTE
  */

  cmss_3gpp_signal_info_s_type  gpp_signal_info;
  /*  Indicates 3GPP system signal info
  */

  boolean                       gw_ecio_for_display;
  /* TRUE if UE is in a data call, else FALSE
  */

  byte                          cs_reg_reject_cause;
  /* Registration Reject Cause for CS
  ** Default = 0
  */

  byte                         ps_reg_reject_cause;
  /* Registration Reject Cause for PS
  ** Default = 0
  */

  boolean                      is_connected_mode_oos;
  /* Flag to store the current OOS status updated when
  ** RRC send CM_CONN_MODE_SERVICE_STATUS_IND to CM
  ** Flag = TRUE means Currently in OOS mode
  ** Flag = FALSE means FACH OOS state is left
  */

  sys_cell_info_s_type         gw_cell_info;
  /*  Indicates the cell_info value. Only valid for GW mode
  */

} cmss_misc_3gpp_stack_info_s_type;

typedef struct cmss_misc_gen_stack_info_s
{

  dword                       srv_inform_uptime;
  /*  Indicates the time interval before clients are informed about
  **  change in system parameters when service is present
  */

  sd_sys_usable_e_type        ss_usable;
  /* Indicates if the system is usable or not.
  */

  boolean                     is_pref_srv_acq;
  /* Indicate whether phone is currently attempting to acquire a more
  ** preferred service. NOTE! This field is undefined when sys_mode =
  ** CM_SYS_MODE_NO_SRV
  */


  dword                       no_srv_uptime;
  /* Indicate the uptime where the service state should be set to
  ** CM_SRV_STATE_NO_SRV
  */

  sys_sys_mode_e_type         last_system_reported;
  /*
  **  Stores the last sys_mode reported to clients.
  **  This variable shouldnt be assigned NO_SRV (except in cmss_reset).
  **  If NO_SRV is reported, this variable holds the system
  **  that was last succesfully acquired before NO_SRV was reported.
  **  Helps keep DS in sync.
  */

} cmss_misc_gen_stack_info_s_type;

typedef struct cmss_misc_gen_sub_info_s
{

  boolean                     is_stable_in_svc;
  /* Indicate if the UE is currently in service on stack 0 and not scanning for it.
  */

  sys_daylt_savings_e_type     daylt_savings;
  /* Day light savings for current CDMA
  ** system. Used for HPCD
  */

#if (defined(FEATURE_GSM) || defined(FEATURE_WCDMA))
  boolean                     is_ps_data_pending_for_srv_cnf;
  /* Indicate whether ps data is set to pending due to cmregprx
  ** watiting for srv cnf when srv req type is user plmn RAT selection
  */
#endif /* FEATURE_PS_DORMANT_PWR_SAVE */

  boolean                     is_redir_or_ho;
  /* Indicate whether redirection or handoff indication is currently
  ** ON
  */

  uint32                       hard_fail_thr_period;
  /* Throttle time period for a hard failure (such as EHRPD auth failure)
  ** Default = 1 hour = CMSS_HARD_FAIL_THROTTLE_TIME */

  boolean         ue_init_ps_data_suspend;
  /* Flag to indicate if the current PS data suspend is initiated by CM
  ** due to voice call in DSDS (or) by NAS in OOS.
  */

  cm_smreg_ps_signalling_rel_req_cause_T    ue_init_ps_data_suspend_cause;
  /* cause due to which it is suspended
  */

  byte                       dch_cell_info_flgs;
  /* CMSS_IS_DCH_CELL_INFO_REQ: If set, Request for Cell_Id is sent to RRC in DCH.
  ** CMSS_IS_DCH_CELL_INFO_RES: If set, "CM_SS_EVENT_SRV_CHANGED" is sent to clients with flag
  ** CM_SS_CELLID_DCH_MASK set to TRUE.
  */

  dword                      main_buffer_srv_uptime;

#if (defined(FEATURE_GSM) || defined(FEATURE_WCDMA))
  cm_ps_data_stt_e_type       ps_data_stt;
  /* Indicate the state when phone is attempting to resume data.
  */

  dword                       no_ps_data_uptime;
  /* Indicate the uptime when data resume is time out
  */
#endif /* FEATURE_PS_DORMANT_PWR_SAVE */
  boolean is_skip_no_srv_buffering_on_rat_remv;
/*If this NV is enabled, no_srv will not be buffered*/

} cmss_misc_gen_sub_info_s_type;


typedef struct cmss_misc_3gpp2_sub_info_s
{

  int8                        ltm_offset;
  /* LTM offset for current CDMA system.
  ** COPY TEXT FROM SPEC. IT IS A 2s compliment
  ** value. It is the signed version of the value obtained from
  ** the CDMA sync channel message. Used for HPCD
  */

  dword                      cdma_ps_call_bsr_uptime;

  dword                      cdma_hold_srv_uptime;
  /* the current uptime for 1x hold service timer */

  boolean                     is_colloc;
  /* Indicate whether a collocated system is available
  */

  boolean                     is_hdr_session_open;
  /**<
  ** Stores whether a HDR session is open or not. A dormant
  ** session is considered as an open session.
  */

  dword                        idm_timer_value;
  /* The value for IDM timer.
  */

#if (defined (FEATURE_HDR_HYBRID))

  boolean                     hdr_is_redir_or_ho;
  /* Indicate whether redirection or handoff indication is currently
  ** ON
  */

#endif /* FEATURE_HDR_HYBRID */

  uint8                       subnet_mask_len;
  /* The subnet mask length of the currently acquired HDR system if any
  */

  cm_country_code_type        current_hdr_mcc;
  /**< Carries the country code of the current serving country. Associated
        with CM_SS_SRV_STATUS_MASK. */

  /* the current uptime for 1x hold service timer */
  /* Timer for buffering the is_operational flag on Hybrid 2 stack when UE
  ** mode switches from SRLTE to CSFB.
  */
  uint8                      hold_stack2_oper_flag_timer_value;
  dword                      hold_stack2_oper_flag_uptime;

} cmss_misc_3gpp2_sub_info_s_type;

/* cmss internal misc structure for stack.
*/
typedef struct cmss_misc_stack_info_s
{

  cmss_misc_3gpp_stack_info_s_type *gpp_info; //need 1 if gw avail, 2 if 1x and gw avail, 1 if 1x avail, 1 if none

  cmss_misc_gen_stack_info_s_type *gen_info; //need 1 if gw avail, 3 if 1x and gw avail, 2 if 1x avail, 1 if none

} cmss_misc_stack_info_s_type;

/* cmss internal misc structure for sub.
*/
typedef struct cmss_misc_sub_info_s
{

  cmss_misc_gen_sub_info_s_type gen_sub_info;

  cmss_misc_3gpp2_sub_info_s_type gpp2_sub_info;

} cmss_misc_sub_info_s_type;

/* cmss internal misc structure for sub and stacks.
*/
typedef struct cmss_misc_sub_prop_s
{

  cmss_misc_sub_info_s_type    sub_info;

  cmss_misc_stack_info_s_type  stack_info[MAX_STACKS];

} cmss_misc_sub_prop_s_type;

/* CM internal misc information.
*/
typedef struct cmss_misc_info_s
{

  cmss_misc_sub_prop_s_type   *sub_prop[MAX_SIMS];

} cmss_misc_info_s_type;

/* Serving System type - holds all the information that is associated
** with a Serving System.
*/
typedef struct cmss_s
{

  cm_init_mask                init_mask;
  /* Indicate whether this object was properly initialized
  */

  uint8                       count;
  /* sub count
  */

  uint8                       ss_stk_bm_pending_deact[MAX_SIMS];

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  /* Private Serving System Information */

  cmss_misc_info_s_type       misc_info;
  /* does not contains the actual serving system information from sd.
  */

  cmss_orig_thr_table_s_type  *cmss_orig_thr_tbl_ptr;
  /* Origination throttle table pointer
  ** Copies system parameters when CM_SS_EVENT_SRV_CHANGED
  ** event is sent to clients about main srv being available.
  ** Used to compare with system parameters after main_srv_inform_uptime
  ** expires. Used for both MAIN and HDR
  */

  cm_mm_ss_info_s_type        info;
  /* cm internal serving system info - it is not true serving system info from sd.
  ** It contain the information to be sent to clients if ss event is triggerred.
  */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  /* Public Serving System Information */

  cm_mm_msim_ss_info_s_type   new_srv_avl_info[MAX_SIMS];
  /* Contains serving system information that is shared
  ** with the clients.
  */

} cmss_s_type;

/* User sid to country mapping
*/
typedef struct cmss_sid_mcc_assoc_s
{

  cm_sid_type           sid;
  /* sid of country */

  cm_country_code_type  country_code;
  /* Country code corresponding to above sid */
} cmss_sid_mcc_assoc_s_type;


/* Table that holds user generated
** sid to mcc mapping
*/
typedef struct cmss_sid_mcc_assoc_tbl
{

  uint8                        sid_mcc_tbl_len;
  /* Size of sid to mcc table length */

  cmss_sid_mcc_assoc_s_type    sid_mcc_tbl [CMSS_SID_MCC_TBL_MAX];
  /* Sid to mcc table */

} cmss_sid_mcc_assoc_tbl_s_type ;

#ifdef CM_GW_SUPPORTED
typedef struct cmss_gwl_signal_status_s
{

  cm_signal_status_type   signal_status_g;
  /* Latest RSSI info of GSM on main stack */

  cm_signal_status_type   signal_status_w;
  /* Latest RSSI info of WCDMA on main/hybr stack */

#ifdef FEATURE_CM_LTE
  lte_cphy_rssi_ind_s       signal_status_lte;
  /* Latest RSSI info of LTE on main/hybr stack */
#endif

#ifdef FEATURE_TDSCDMA
  tds_rrc_scell_signal_status_ind_type signal_status_tds;
  /* Latest RSSI info of TDS on main stack */

#endif

} cmss_gwl_signal_status_s_type;
#endif



/**--------------------------------------------------------------------------
** Functions
** --------------------------------------------------------------------------
*/


/*===========================================================================

FUNCTION cmss_ptr

DESCRIPTION
  Return a pointer to the one and only SS object.

  The serving system object is responsible for:
  1. Processing MC serving system related notifications.
  2. Notifying the client list of serving system events.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern cmss_s_type  *cmss_ptr
(
  void
);

/*===========================================================================

FUNCTION cmss_true_info_ptr

DESCRIPTION
  Return a pointer to current true serving system info

DEPENDENCIES
  none

RETURN VALUE
  cmss_true_info_s_type

SIDE EFFECTS
  none

===========================================================================*/
extern cmss_true_info_s_type *cmss_true_info_ptr(void);

/*===========================================================================

FUNCTION cmss_true_stack_info_ptr

DESCRIPTION
  Return a pointer for the specified stack to current true serving system info

DEPENDENCIES
  none

RETURN VALUE
  sd_si_info_s_type

SIDE EFFECTS
  none

===========================================================================*/
sd_si_info_s_type  *cmss_true_stack_info_ptr(
  mm_sub_stk_id_s_type   mm_id
);

/*===========================================================================

FUNCTION cmss_true_srv_info_mem_free

DESCRIPTION
  free memory of true srv info

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/
extern void cmss_true_srv_info_mem_free(void);

/*===========================================================================

FUNCTION cmss_intl_srv_info_mem_free

DESCRIPTION
  free memory of cmss srv stack info

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/
extern void cmss_mm_ss_info_stack_mem_free(
  mm_sub_stk_id_s_type  mm_id
);

/*===========================================================================

FUNCTION cmss_mm_ss_info_sub_mem_free

DESCRIPTION
  free memory of cmss srv sub info

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/
extern void cmss_mm_ss_info_sub_mem_free(
  sys_modem_as_id_e_type asubs_id
);

/*===========================================================================

FUNCTION cmss_misc_info_stack_mem_free

DESCRIPTION
  free memory of misc srv info


DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/
extern void cmss_misc_info_stack_mem_free(
  mm_sub_stk_id_s_type  mm_id
);

/*===========================================================================

FUNCTION cmss_misc_info_sub_mem_free

DESCRIPTION
  free memory of cmss misc sub info

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/
extern void cmss_misc_info_sub_mem_free(
  sys_modem_as_id_e_type asubs_id
);

/*===========================================================================

FUNCTION cmss_init

DESCRIPTION
  Initializes the serving system object.

  This function must be called before the serving system object
  is being used, in any way, place, or form.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmss_init
(
  void
);

/*===========================================================================

FUNCTION cmss_update_stack_upon_subsc_chg

DESCRIPTION
  Allocate stack based on new subscription availability
  Mark stack as pending for deallocation if the active stack is no longer active

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmss_update_stack_upon_subsc_chg(
  sys_modem_as_id_e_type     asubs_id,
  uint8                      new_num_stack
);

/*===========================================================================

FUNCTION cmss_dealloc_stack_upon_subsc_chg_done

DESCRIPTION
  Deallocate inactive stack upon receiving subscription done report from MMOC
  This function deallocate the memory for cmss stacks that are no longer needed
  based on current subcription availability

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmss_dealloc_stack_upon_subsc_chg_done(

  sys_modem_as_id_e_type     asubs_id,

  uint8                      stk_active_bitmask
);

/*===========================================================================

FUNCTION cmss_generic_rpt_proc

DESCRIPTION
  Process Generic reports in CMSS


DEPENDENCIES
  Serving System object must have already been initialized with
  cmss_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmss_generic_rpt_proc(

  const cm_hdr_type   *rpt_ptr
  /* Pointer to generic reports */
);


#if (defined(FEATURE_MMODE_CDMA_800) || defined(FEATURE_MMODE_CDMA_1900))

/*===========================================================================

FUNCTION cmss_mc_rpt_proc

DESCRIPTION
  Process MC reports


DEPENDENCIES
  Serving system object must have already been initialized with
  cmss_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmss_mc_rpt_proc
(
  const cm_hdr_type   *rpt_ptr
  /* Pointer to MC report */
);
#endif /* (defined(FEATURE_MMODE_CDMA_800) || defined(FEATURE_MMODE_CDMA_1900)) */


#if (defined(CM_GW_SUPPORTED) || defined(FEATURE_CM_LTE))

/*===========================================================================

FUNCTION cmss_rpt_proc

DESCRIPTION
  Process reports from the lower layers.


DEPENDENCIES
  Serving system object must have already been initialized with
  cmss_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmss_rpt_proc
(
  const cm_hdr_type   *rpt_ptr
  /* Pointer to LL report */
);
#endif /* (defined(FEATURE_WCDMA) || defined(FEATURE_GSM) || defined(FEATURE_CM_LTE)) */

/*===========================================================================
FUNCTION cmss_gwl_get_signal_status_per_sub

DESCRIPTION
  This function fetches the RSSI of the given mode from cmss_gwl_signal_status.

DEPENDENCIES


RETURNS
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmss_gwl_get_signal_status_per_sub(
  sys_sys_mode_e_type     sys_mode,

  void                   *signal_status_ptr,

  sys_modem_as_id_e_type  asubs_id
);

#ifdef FEATURE_TDSCDMA
/*===========================================================================

FUNCTION cmss_msgr_rpt_proc

DESCRIPTION
  Process reports from the TDCMDA lower layers.

DEPENDENCIES
  Serving System object must have already been initialized with
  cmss_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmss_msgr_tds_rpt_proc(

  const void          *rpt_ptr
  /* Pointer to the report sent by Message Router */
);

#endif

#ifdef FEATURE_CM_LTE
/*===========================================================================

FUNCTION cmss_msgr_rpt_proc

DESCRIPTION
  Process reports from the LTE lower layers.

DEPENDENCIES
  Serving System object must have already been initialized with
  cmss_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmss_msgr_rpt_proc(

  const void          *rpt_ptr,
  /* Pointer to the report sent by Message Router */

  sys_modem_as_id_e_type asubs_id
);


/*===========================================================================

FUNCTION cmss_update_lte_cell_info

DESCRIPTION
  This function receives cell info from LTE RRC to be sent out to the clients.
  SRV change event sent to client only if change in cell_id

DEPENDENCIES
  Serving System object must have already been initialized with
  cmss_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmss_update_lte_cell_info
(
  const lte_rrc_service_ind_s           *rpt_ptr,
  /* ptr to message received */

  sys_modem_as_id_e_type asubs_id
);
#ifdef FEATURE_DISABLED_HABANERO
/*===========================================================================

FUNCTION cmss_trigger_inact_timer_info_evt

DESCRIPTION
   Notify client when data inactivity timer expires or RRC connection 
   is released.


DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmss_trigger_inact_timer_info_evt
(
const lte_rrc_data_inactivity_info_ind_s           *rpt_ptr,
  /* ptr to message received */
sys_modem_as_id_e_type asubs_id
);
#endif
/*===========================================================================

FUNCTION cmss_read_emerg_barring_params

DESCRIPTION
  This function receives emergency ac barring info from LTE RRC to be sent out to the clients.

DEPENDENCIES
  Serving System object must have already been initialized with
  cmss_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/

void cmss_read_emerg_barring_params
(
  lte_rrc_barring_update_ind_s   *barring_update_ind,
  sys_modem_as_id_e_type asubs_id
);

/*===========================================================================

FUNCTION cmss_update_lte_emerg_access_status

DESCRIPTION
  This function receives SIB2 emergency access barred status from LTE RRC.
  SRV change event sent to client only if svc_status is AVAILABLE and there's
  change in emergency access bar status.

DEPENDENCIES
  Serving System object must have already been initialized with
  cmss_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmss_update_lte_emerg_access_status
(
  const lte_rrc_service_ind_s           *rpt_ptr
  /* ptr to message received */
);

#endif

/*===========================================================================
FUNCTION  cmss_update_cdma_srv_lte_do_redir

DESCRIPTION
 Reset the flag cmss_reset_main_buffer_srv_uptime to NONE.

DEPENDENCIES


RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmss_update_cdma_srv_lte_do_redir (
  sys_modem_as_id_e_type  asubs_id
);


/*===========================================================================

FUNCTION cmss_get_ss_cmd_asubs_id

DESCRIPTION
  Get asubs_id from cm ss command ptr.

DEPENDENCIES
  None.

RETURN VALUE
  sys_modem_as_id_e_type.

SIDE EFFECTS
  None.
===========================================================================*/
sys_modem_as_id_e_type cmss_get_ss_cmd_asubs_id (
  const  cm_ss_cmd_s_type  *ss_cmd_ptr
);

/*===========================================================================

FUNCTION cmss_is_stack1_operational

DESCRIPTION
  This function should be called to determine if hybrid is ON.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
boolean    cmss_is_stack1_operational(
  sys_modem_as_id_e_type   asubs_id
);

/*===========================================================================

FUNCTION cmss_is_stack2_operational

DESCRIPTION
  This function should be called to determine if hybrid is ON.

DEPENDENCIES
  None.

RETURN VALUE
  Boolean.

SIDE EFFECTS
  None.
===========================================================================*/
boolean    cmss_is_stack2_operational(
  sys_modem_as_id_e_type   asubs_id
);

/*===========================================================================

FUNCTION cmss_is_main_operational

DESCRIPTION
  This function should be called to determine if MAIN is active

DEPENDENCIES
  None.

RETURN VALUE
  Boolean.

SIDE EFFECTS
  None.
===========================================================================*/
boolean    cmss_is_main_operational(
  sys_modem_as_id_e_type   asubs_id
);

/*===========================================================================

FUNCTION cmss_is_operational

DESCRIPTION
  This function should be called to determine if a stack is operational.

DEPENDENCIES
  None.

RETURN VALUE
  Boolean.

SIDE EFFECTS
  None.
===========================================================================*/
boolean    cmss_is_operational(
  mm_sub_stk_id_s_type  mm_id
);

/*===========================================================================

FUNCTION cmss_get_info_sub_prop

DESCRIPTION
  get cmss info sub property pointer

DEPENDENCIES
  none

RETURN VALUE
  cm_mm_ss_info_sub_prop_s_type

SIDE EFFECTS
  none

===========================================================================*/
cm_mm_ss_info_sub_prop_s_type *cmss_get_info_sub_prop(
  sys_modem_as_id_e_type  asubs_id
);

/*===========================================================================

FUNCTION cmss_get_misc_sub_prop_ptr

DESCRIPTION
  get cmss internal info sub property pointer

DEPENDENCIES
  none

RETURN VALUE
  cmss_misc_sub_prop_s_type

SIDE EFFECTS
  none

===========================================================================*/
cmss_misc_sub_prop_s_type *cmss_get_misc_sub_prop_ptr(
  sys_modem_as_id_e_type  asubs_id
);

/*===========================================================================

FUNCTION cmss_map_stack_id_to_client_stk

DESCRIPTION
  map stack id to client stack id, mainly for cm_mm_msim_ss_info_s_type

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
uint8 cmss_map_stack_id_to_client_stk(
  multimode_stack_e_type stack_id
);

/*===========================================================================

FUNCTION cmss_event

DESCRIPTION
  Notify the client list of a specified serving system event.

DEPENDENCIES
  Serving system object must have already been initialized with
  cmss_init().

RETURN VALUE
  none

SIDE EFFECTS
  Changes Serving System object
===========================================================================*/
extern void cmss_event(

  mm_sub_stk_id_s_type  mm_id,

  cm_ss_event_e_type    ss_event
  /* notify client list of this serving system event */
);

/*===========================================================================

FUNCTION cmss_mapping_info

DESCRIPTION
  Copy the current serving system state information into a specified buffer.

DEPENDENCIES
  Serving System object must have already been initialized with
  cmss_init().

RETURN VALUE
  TRUE if serving system information is valid, FALSE otherwise.

  Note that serving system information only becomes valid after cmss_init()
  returns.

SIDE EFFECTS
  none

===========================================================================*/
extern void cmss_mapping_info(

  cm_mm_ss_info_s_type    *ss_info_ptr,
  /* Copy serving system state info into this buffer */

  sys_modem_as_id_e_type        req_as_id,

  cm_mm_msim_ss_info_s_type    *new_info_ptr
  /* Base ss_info that gets copied into ss_info_ptr. Can be either
  ** cmss.info OR cmss.srv_avail_info
  */
);

/*===========================================================================

FUNCTION cmss_copy_rssi_info

DESCRIPTION
  Copy the current serving system state information related to RSSI into a specified buffer.

DEPENDENCIES
  Serving System object must have already been initialized with
  cmss_init().

RETURN VALUE
  TRUE if serving system information is valid, FALSE otherwise.

  Note that serving system information only becomes valid after cmss_init()
  returns.

SIDE EFFECTS
  none

===========================================================================*/
extern void cmss_copy_rssi_info(

  cm_mm_ss_info_s_type    *ss_info_ptr,
  /* Copy serving system state info into this buffer */

  sys_modem_as_id_e_type        req_as_id,

  cm_mm_msim_ss_info_s_type    *new_info_ptr
  /* Base ss_info that gets copied into ss_info_ptr. Can be either
  ** cmss.info OR cmss.srv_avail_info
  */
);

/*===========================================================================

FUNCTION cmss_sd_rpt_proc

DESCRIPTION
  Process reports from System Determination.


DEPENDENCIES
  Serving system object must have already been initialized with
  cmss_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmss_sd_rpt_proc
(
  const cm_hdr_type   *rpt_ptr
  /* Pointer to SD report */
);



/*===========================================================================

FUNCTION cmss_client_cmd_proc

DESCRIPTION
  Process clients' ss commands

DEPENDENCIES
  SS object must have already been initialized with
  cmss_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmss_client_cmd_proc
(
  cm_ss_cmd_s_type  *ss_cmd_ptr
  /* pointer to a CM command */
);



/*===========================================================================

FUNCTION cmss_timer_proc

DESCRIPTION
  Process timer events.

DEPENDENCIES
  Serving system object must have already been initialized with
  cmss_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmss_timer_proc
(
  cm_timer_event_type    timer_event,
  /* Indicate specific timer event.
  **
  ** Note that currently timer event is not being used */
  /*lint -esym(715,timer_event) */
  sys_modem_as_id_e_type   asubs_id
);

/*===========================================================================

FUNCTION cmss_stack_2_timer_proc

DESCRIPTION
  Process timer events.

DEPENDENCIES
  Serving system object must have already been initialized with
  cmss_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmss_stack_2_timer_proc
(
  cm_timer_event_type    timer_event,
  /* Indicate specific timer event.
  **
  ** Note that currently timer event is not being used
  */
  sys_modem_as_id_e_type  asubs_id
);


#if (defined(FEATURE_MMODE_CDMA_800) || defined(FEATURE_MMODE_CDMA_1900))

/*===========================================================================
FUNCTION cmss_rssi_timer_handle

DESCRIPTION
  This function check the rssi. If RSSI change exceeds the delta threshold,
  CM reports rssi change event to registered clients.

DEPENDENCIES
  Serving System object must be initialized

RETURNS
  none
SIDE EFFECTS
  none

===========================================================================*/
extern void cmss_rssi_timer_handle
(
  void
);
#endif /* FEATURE_MMODE_CDMA_800, FEATURE_MMODE_CDMA_1900 */


#ifdef CM_GW_SUPPORTED

/*===========================================================================
FUNCTION cmss_gwl_rssi_timer_handle

DESCRIPTION
  This function check the rssi. If RSSI change exceeds the delta threshold,
  CM reports rssi change event to registered clients.

DEPENDENCIES
  Serving System object must be initialized

RETURNS
  none
SIDE EFFECTS
  none

===========================================================================*/
extern void cmss_gwl_rssi_timer_handle
(
  void
);
#endif /* CM_GW_SUPPORTED */

/*===========================================================================
FUNCTION cmss_send_idle_digital_mode_event

DESCRIPTION
  This function sends the idle digital mode event

DEPENDENCIES
  Serving System object must be initialized

RETURNS
  None

SIDE EFFECTS
  none

===========================================================================*/
void cmss_send_idle_digital_mode_event
(
  sys_modem_as_id_e_type  asubs_id
);




/*===========================================================================

FUNCTION cmss_map_sd_mode_to_cm_mode

DESCRIPTION
  Map SD mode to CM system mode

DEPENDENCIES
  None

RETURN VALUE
  CM system mode

SIDE EFFECTS
  None

===========================================================================*/
extern sys_sys_mode_e_type   cmss_map_sd_mode_to_cm_mode(

  sd_mode_e_type        sd_mode
  /* cm system mode.
  */
);


/*===========================================================================

FUNCTION cmss_proc_call_ended

DESCRIPTION
  This function should be called to process the call end event from
  CMCALL.

DEPENDENCIES
  cmssidm_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern  void                           cmss_proc_call_ended
(
  sys_modem_as_id_e_type  asubs_id
);


/*===========================================================================

FUNCTION cmss_is_cdma_svd_supported

DESCRIPTION
  This function should be called to check if SVD is supported on 1x.

DEPENDENCIES
  cmss_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern  boolean                           cmss_is_cdma_svd_supported
(
  sys_modem_as_id_e_type  asubs_id
);


/*===========================================================================

FUNCTION cmss_get_service_status_per_stack

DESCRIPTION
  Copies quick service info into output buffer for given stack.

DEPENDENCIES
  cmss_init()

RETURN VALUE
  None

SIDE EFFECTS
  none

===========================================================================*/
void cmss_get_service_status_per_stack(

  mm_sub_stk_id_s_type  mm_id,
  cm_service_status_s_type *srv_info
  /* Buffer to hold service info for the given stack */
);

/*===========================================================================

===========================================================================*/

void cmss_get_camped_srv_info(

  mm_sub_stk_id_s_type mm_id,
  sys_srv_status_e_type *srv_status

);


/*===========================================================================

FUNCTION cmss_send_mm_ps_data_avail_req

DESCRIPTION
  Send the ps data available request to MM

DEPENDENCIES

RETURN VALUE
  Return TRUE if the command is successfully send. Otherwise FALSE.

SIDE EFFECTS
  none

===========================================================================*/
extern  boolean                   cmss_send_mm_data_avail
(
  sys_modem_as_id_e_type  asubs_id
);


/*===========================================================================

FUNCTION cmss_send_mm_ims_reg_info

DESCRIPTION
  Send the IMS registration Info to MM

DEPENDENCIES

RETURN VALUE
  Return TRUE if the command is successfully send. Otherwise FALSE.

SIDE EFFECTS
  none

===========================================================================*/
boolean cmss_send_mm_ims_reg_info(

  boolean                 is_ims_voice_avail,
  /* Indicates whether IMS voice is avaialble or not*/

  sys_modem_as_id_e_type  asubs_id
);

/*===========================================================================

FUNCTION cmss_send_ims_emerg_capability

DESCRIPTION
  Send the IMS emergency capability to LTE RRC

DEPENDENCIES

RETURN VALUE


SIDE EFFECTS
  none

===========================================================================*/

void cmss_send_ims_emerg_capability(
  boolean                 ims_emg_supp,
  sys_modem_as_id_e_type  asubs_id
);

/*===========================================================================

FUNCTION cmss_process_data_suspend_rpt

DESCRIPTION
  Update the Serving system object per the new data suspend state.

  If data suspend state is changed, Inform CM clients of corresponding event.

DEPENDENCIES
  Serving system object must have already been initialized with
  cmss_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void  cmss_process_data_suspend_rpt(

  boolean             data_suspend,
  /* New data suspend State.
  */
  mm_sub_stk_id_s_type  mm_id
);

/*===========================================================================

FUNCTION cmss_sys_id_match

DESCRIPTION
  Verfies if two system identities are equal or not.

DEPENDENCIES

RETURN VALUE
  Return TRUE if identities are equal. Otherwise FALSE.

SIDE EFFECTS
  none

===========================================================================*/
extern boolean cmss_sys_id_match(

  sys_sys_id_s_type     sys_id_1,

  sys_sys_id_s_type     sys_id_2
);

/*===========================================================================

FUNCTION cmss_is856_sys_id_match

DESCRIPTION
  Verifies if two IS856 system identities are equal or not.
  The subnet mask to be used is obtained from cmss_ptr()

DEPENDENCIES

  subnet mask length shonld be updated correctly in cmss_ptr()

RETURN VALUE
  Return TRUE if subnet ids are equal. Otherwise FALSE.

SIDE EFFECTS
  none

===========================================================================*/
boolean cmss_is856_sys_id_match(

  sys_sys_id_s_type     sys_id_1,

  sys_sys_id_s_type     sys_id_2
);

/*===========================================================================

FUNCTION cmss_rel_cipher_ind

DESCRIPTION
  This function removes the ciphering indication for a particular domain if
  it is active and informs clients.

DEPENDENCIES
  Serving System object must have already been initialized with
  cmss_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmss_rel_cipher_ind
(
  sys_srv_domain_e_type cipher_domain,
  /* Domain of ciphering to be removed
  */

  sys_modem_as_id_e_type  asubs_id
);

#ifdef  FEATURE_CALL_THROTTLE

/*===========================================================================

FUNCTION cmss_check_if_orig_thr_true

DESCRIPTION
  This function checks is the origination needs to be throttled

DEPENDENCIES
  cmss_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

extern boolean cmss_check_if_orig_thr_true
(
  sys_sys_id_s_type   sys_id,
  /* The sys id of the system, SID,NID
  */

  sys_band_class_e_type band_class,
  /* The band class information
  */

  cmss_orig_thr_table_s_type *thr_table_ptr,
  /* Origination throttle table pointer
  */
  cm_call_type_e_type         call_type,
  /* Call type - VOICE, SMS, E911, etc
  */

  uint16                      packet_zone
  /* The packet zone id
  */

);


/*===========================================================================

FUNCTION cmss_orig_throttle_update_table_with_call_status

DESCRIPTION
  This function should be called to update the origination throttle table
  with the call status

DEPENDENCIES
  cmss_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

extern boolean cmss_orig_thr_update_tbl_entry_with_call_status
(
  cmss_orig_thr_table_s_type *throttle_table_ptr,
  /* Pointer to the origination throttle table
  */
  cm_ss_orig_thr_call_status_s_type *throttle_call_status,
  /* Information about the current call status
  */
  sys_modem_as_id_e_type  asubs_id
);

/*===========================================================================

FUNCTION cmss_check_if_orig_thr_and_add_entry

DESCRIPTION
  This function checks if the call origination needs to be throttled, and
  if it needs to be throttled then adds a entry in the origination throttle
  table

DEPENDENCIES
  cmss_init() should have been called previously.

RETURN VALUE
  True: If the call is throttled and the entry made in the table
  False: Otherwise

SIDE EFFECTS
  None.
===========================================================================*/
boolean cmss_check_if_orig_thr_and_add_entry(

  cm_call_type_e_type   call_type,
  /* Call type - VOICE, SMS, E911, etc
  */

  cm_call_end_e_type    end_status,
  /* Call end status
  */
  boolean               is_hdr,
  /* Is the HDR instance throttled
  */
  sys_modem_as_id_e_type  asubs_id
);
#endif  /* FEATURE_CALL_THROTTLE */



/*===========================================================================

FUNCTION cmss_orig_thr_table_reset

DESCRIPTION
  Reset all the cmss_orig_thr_table information


DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmss_orig_thr_table_reset(void);


#ifdef  FEATURE_CALL_THROTTLE
/*===========================================================================

FUNCTION cmss_misc_set_ss_info_thr_status

DESCRIPTION
  This function sets the ss_thr_status of the ss object

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

void cmss_misc_set_ss_info_thr_status(
  sys_modem_as_id_e_type  asubs_id
);

#endif  /* FEATURE_CALL_THROTTLE */


/*===========================================================================

FUNCTION cmss_map_reg_emergtype_to_cm_emergtype

DESCRIPTION
  Emergency type bit masks from REG get mapped to CM type bit masks

DEPENDENCIES
  cmss_init() should have been called previously.

RETURN VALUE
  CM bit masks

SIDE EFFECTS
  None.

===========================================================================*/
uint32 cmss_map_reg_emergtype_to_cm_emergtype (

  byte reg_emerg_num_type
  /* Emergency number type
  */
);

/*===========================================================================

FUNCTION cmss_send_ss_info

DESCRIPTION
  This function sends the serving system information to the requesting
  client.

DEPENDENCIES
  SS object must have already been initialized with
  cmss_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmss_send_ss_info(

  const cm_ss_cmd_s_type     *ss_cmd_ptr,
  /* The pointer to a serving system command */

  cm_ss_event_e_type    event_type
  /* The type of event to be sent to client */
);

/*===========================================================================

FUNCTION cmss_proc_full_srv_lost

DESCRIPTION
   Process the service indication from SD/RRC.


DEPENDENCIES
  Serving System object must have already been initialized with
  cmss_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern  void  cmss_proc_full_srv_lost
(
  mm_sub_stk_id_s_type   mm_id
);

/*===========================================================================

FUNCTION cmss_inform_target_rat

DESCRIPTION
   Notify client of target RAT when doing eHRPD to LTE reselection


DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/

void cmss_inform_target_rat
(
  sys_sys_mode_e_type     target_rat,

  sys_modem_as_id_e_type  asubs_id
);

/*===========================================================================
FUNCTION cmss_update_ps_dormant_entry_in_queue

DESCRIPTION
  Update the priority queue as below.

  Pump phone  preferences to top of the queue and change the PS call
  preferences to below phone preferences in following scenario.
  (To do alternate scanning on GW+1X).
    - Data is suspended
    - Serive is not full service.
    - PS call preferences are on top of the queue

  Pump ps call preferences to top of the queue in following scenario
  (To resume dormant data call).
    - Data is suspended
    - UE is already having full service on GW.
    - PS call preferences are below phone preferences.
    - Data available request is pending

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmss_update_ps_dormant_entry_in_queue
(
  boolean change_priority,

  sys_modem_as_id_e_type  asubs_id
);



/*===========================================================================

FUNCTION cmss_clear_reg_reject_info

DESCRIPTION
   Clear the registration reject info in ss ptr


DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/

void cmss_clear_reg_reject_info
(
  sys_plmn_service_state_s_type  *srv_info_ptr,
  mm_sub_stk_id_s_type            mm_id
);


/*===========================================================================

FUNCTION cmss_inform_ctxt_transfer_event

DESCRIPTION
   Notify client of target RAT when doing IRAT CONTEXT TRANSFER


DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmss_inform_ctxt_transfer_event
(
  sys_radio_access_tech_e_type       target_rat,
  sys_modem_as_id_e_type             asubs_id
  /* The ASUBS_ID on which the context transfer happens. */
);

/*===========================================================================

FUNCTION cmss_is_3gpp_in_srv

DESCRIPTION
   Check if G/W/L/TDS/GWL/GW is in service

DEPENDENCIES
  none

RETURN VALUE
  TRUE if in service
  FASLE otherwise

SIDE EFFECTS
  none

===========================================================================*/
boolean cmss_is_3gpp_in_srv
(
  sys_sys_mode_e_type  mode,
  /* Mode to check against */

  boolean              check_only_full_srv
  /* Indicate if check only full srv */
);

/*===========================================================================

FUNCTION cmss_reset_cell_info_upon_hybr_gw_change

DESCRIPTION
  Reset gw cell info id when Hybr2 stack is removed(hybr gw not operational)
  Reset main cell info when hybr gw becomes operational

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
void cmss_reset_cell_info_upon_stack2_change
(
  boolean  is_stack2_operational,

  sys_modem_as_id_e_type              asubs_id
);

/*===========================================================================

FUNCTION cmss_reset_gwl_signal_status

DESCRIPTION
   Resets the static data of CMSS maintaining the
   gwl signal status


DEPENDENCIES
  Serving System object must have already been initialized with
  cmss_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/

extern void cmss_reset_gwl_signal_status(void);

/*===========================================================================

===========================================================================*/

sys_modem_stack_id_e_type cmss_map_stack_to_ssevt_idx(
  multimode_stack_e_type  stk_id
);

/*===========================================================================

FUNCTION cmss_send_mcc_msg

DESCRIPTION
  This functions sends a messages to generic report queues updating the latest
  MCC that has been detected. This MCC information would be used in prediction
  algorithm.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmss_send_mcc_msg(
  const sd_mcc_rpt_s_type *sd_rpt,
  mm_sub_stk_id_s_type   mm_id
);

/*===========================================================================

FUNCTION cmss_nv_refresh

DESCRIPTION
  Reading NV items in serving system object.


DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmss_nv_refresh(

  uint8             refresh_subs_mask
  /* Subs mask whose NVs need to be refreshed
  */

);

/*===========================================================================

FUNCTION cmss_get_gw_sim_state_per_subs

DESCRIPTION
  Get the sim state for the corresponding sub.

DEPENDENCIES
  Serving system object must have already been initialized with
  cmss_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern sys_sim_state_e_type cmss_get_gw_sim_state_per_subs(

  sys_modem_as_id_e_type asubs_id

);

/*===========================================================================

FUNCTION cmss_get_no_srv_uptime

DESCRIPTION
  Get the no_srv_uptime for the corresponding stack.

DEPENDENCIES
  Serving system object must have already been initialized with
  cmss_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
dword cmss_get_no_srv_uptime(
  mm_sub_stk_id_s_type  mm_id
  /* sub and stack id */
);

/*===========================================================================

FUNCTION cmss_reset_sys_id_info

DESCRIPTION
  clears the plmn id information in cmss structure

DEPENDENCIES
  Serving system object must have already been initialized with
  cmss_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/

void cmss_reset_sys_id_info(void);

/*===========================================================================

FUNCTION cmss_get_true_sys_mode

DESCRIPTION
  Gets true sys mode taking buffered service indications into consideration

DEPENDENCIES
  Serving system object must have already been initialized with
  cmss_init().

RETURN VALUE
  actual sys mode of that stack

SIDE EFFECTS
  none

===========================================================================*/
extern sys_sys_mode_e_type cmss_get_true_sys_mode(
  mm_sub_stk_id_s_type mm_id
);

/*===========================================================================

FUNCTION cmph_evaluate_effect_srv_domain_send_ss_event

DESCRIPTION
  send SS Event to clients if effective service domain is updated

DEPENDENCIES
  None

RETURN VALUE
  boolean

SIDE EFFECTS
  None.

===========================================================================*/
extern void cmss_evaluate_effect_srv_domain_send_ss_event(
  sys_modem_as_id_e_type  asubs_id
);

/*===========================================================================

FUNCTION cmss_remove_srv_domain_comp

DESCRIPTION
   Remvoe a srv domain component from srv_domain_val

DEPENDENCIES
  Serving System object must have already been initialized with
  cmss_init().

RETURN VALUE
  New srv domain type

SIDE EFFECTS
  none

===========================================================================*/

sys_srv_domain_e_type cmph_get_effective_srv_domain(
  sys_modem_as_id_e_type  as_id,

  /*
   subs id information
  */

  sys_srv_domain_e_type srv_domain

  /* registered service domain
  */

);

/*===========================================================================
FUNCTION  cmss_reset_cdma_hold_srv_uptime

DESCRIPTION
 Reset cdma_hold_srv_uptime to NONE and update CDMA service.

DEPENDENCIES


RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmss_reset_cdma_hold_srv_uptime(sys_modem_as_id_e_type  asubs_id);

extern boolean cmss_get_is_stack1_full_srv_lost(sys_modem_as_id_e_type  asubs_id);
extern boolean cmss_get_is_stack2_full_srv_lost(sys_modem_as_id_e_type  asubs_id);

/*===========================================================================

FUNCTION cmss_start_stack2_oper_flag_timer

DESCRIPTION
  Start the timer to buffer the value of is_operational flag on the outgoing
  CM SS Events.

DEPENDENCIES
  Serving system object must have already been initialized with
  cmss_init().

RETURN VALUE
  actual sys mode of that stack

SIDE EFFECTS
  none

===========================================================================*/
extern void cmss_start_stack2_oper_flag_timer(
  sys_modem_as_id_e_type  asubs_id
);

/*===========================================================================

FUNCTION cmss_lte_srv_on_sub

DESCRIPTION
  Determine if LTE is in service for the sub.

DEPENDENCIES
  Serving system object must have already been initialized with
  cmss_init().

RETURN VALUE
  Boolean

SIDE EFFECTS
  none

===========================================================================*/
boolean  cmss_lte_srv_on_sub(
  mm_sub_stk_id_s_type  *mm_id_ptr
);

/*===========================================================================

FUNCTION cmss_resume_data

DESCRIPTION
  Resume data if it is suspended on the given sub.

DEPENDENCIES
  Serving system object must have already been initialized with
  cmss_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void  cmss_resume_data(
  boolean reset_flag,
  sys_modem_as_id_e_type asubs_id
);

/*===========================================================================

FUNCTION cmss_process_suspend_cause

DESCRIPTION
  Resume data if it is suspended due to standby change. .

DEPENDENCIES
  Serving system object must have already been initialized with
  cmss_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void  cmss_process_suspend_cause(void);

/*===========================================================================

FUNCTION cmss_get_true_srv_lte_stk

DESCRIPTION
  Returns the sub/stk id which has true LTE service.

DEPENDENCIES
  none

RETURN VALUE
  multimode_stack_e_type
  MM_STACK_NONE - if the sub has no LTE

SIDE EFFECTS
  none

===========================================================================*/
multimode_stack_e_type  cmss_get_true_srv_lte_stk(
  sys_modem_as_id_e_type  asubs_id
);

/*===========================================================================

FUNCTION cmss_get_info_stk_ptr

DESCRIPTION
  get cmss info stack property pointer

DEPENDENCIES
  none

RETURN VALUE
  cm_mm_ss_info_gen_stack_info_s_type

SIDE EFFECTS
  none

===========================================================================*/
cm_mm_ss_info_gen_stack_info_s_type *cmss_get_info_stk_ptr(
  sys_modem_as_id_e_type  asubs_id,
  multimode_stack_e_type  stk_id
);

/*===========================================================================

FUNCTION cmss_get_misc_gen_stk_ptr

DESCRIPTION
  get cmss misc info stack pointer

DEPENDENCIES
  none

RETURN VALUE
  cmss_misc_gen_stack_info_s_type

SIDE EFFECTS
  none

===========================================================================*/
cmss_misc_gen_stack_info_s_type *cmss_get_misc_gen_stk_ptr(
  sys_modem_as_id_e_type  asubs_id,
  multimode_stack_e_type  stk_id
);

/*===========================================================================

FUNCTION cmss_get_info_sys_mode
DESCRIPTION
  Determine sys mode for cmss stack.

DEPENDENCIES
  Serving system object must have already been initialized with
  cmss_init().

RETURN VALUE
  sys_sys_mode_e_type

SIDE EFFECTS
  none

===========================================================================*/
sys_sys_mode_e_type  cmss_get_info_sys_mode(
  sys_modem_as_id_e_type  asubs_id,
  multimode_stack_e_type  stk_id
);

/*===========================================================================
FUNCTION cmss_report_raw_3gpp_service

DESCRIPTION
  This function report original 3gpp service status from NAS to IMS without delay

DEPENDENCIES

RETURNS
  none

SIDE EFFECTS
  none

===========================================================================*/

extern void cmss_report_raw_3gpp_service
(
  sys_srv_status_e_type      srv_status,
  sys_srv_domain_e_type      srv_domain,
  sys_radio_access_tech_e_type sys_mode,
  sys_modem_as_id_e_type     as_id,
  sys_plmn_id_s_type           plmn
);
/*===========================================================================
FUNCTION cmss_get_info_srv_status
DESCRIPTION
  Determine srv status for cmss stack.

DEPENDENCIES
  Serving system object must have already been initialized with
  cmss_init().

RETURN VALUE
  sys_srv_status_e_type

SIDE EFFECTS
  none

===========================================================================*/
sys_srv_status_e_type  cmss_get_info_srv_status(
  sys_modem_as_id_e_type  asubs_id,
  multimode_stack_e_type  stk_id
);

/*===========================================================================

FUNCTION cmss_get_info_srv_domain
DESCRIPTION
  Determine srv domain for cmss stack.

DEPENDENCIES
  Serving system object must have already been initialized with
  cmss_init().

RETURN VALUE
  sys_srv_domain_e_type

SIDE EFFECTS
  none

===========================================================================*/
sys_srv_domain_e_type  cmss_get_info_srv_domain(
  sys_modem_as_id_e_type  asubs_id,
  multimode_stack_e_type  stk_id
);

/*===========================================================================
  FUNCTION cmss_is_ss_in_full_srv

  DESCRIPTION
    Tells if given ss is in full service or not.

  DEPENDENCIES

  RETURN VALUE
    true if in full service
    false if not

  SIDE EFFECTS
    none
===========================================================================*/
extern boolean cmss_is_ss_in_full_srv(mm_sub_stk_id_s_type mm_id);

/*===========================================================================
  FUNCTION cmss_is_ss_in_lmtd_srv

  DESCRIPTION
    Tells if given ss is in limited service or not.

  DEPENDENCIES

  RETURN VALUE
    true if in full service
    false if not

  SIDE EFFECTS
    none
===========================================================================*/
extern boolean cmss_is_ss_in_lmtd_srv(mm_sub_stk_id_s_type mm_id);

/*===========================================================================
FUNCTION CMSS_CONVERT_RRC_PLMN_ID_TO_SYS_PLMN_ID

DESCRIPTION
  This function converts from the RRC PLMN ID format (1 BCD digit per byte)
  to the NAS PLMN_id_T format (2 BCD digits per byte).

DEPENDENCIES
  None

RETURN VALUE
  PLMN ID converted to PLMN_id_T format (3 byte array - 2 BCD digits/byte)

SIDE EFFECTS
  None
===========================================================================*/
extern sys_plmn_id_s_type cmss_convert_rrc_plmn_id_to_sys_plmn_id(
  lte_rrc_plmn_s plmn_id  /* PLMN id provided by LTE RRC */
);

/*===========================================================================

FUNCTION cmss_send_ims_reg_capability

DESCRIPTION
  Send the IMS registration capability to LTE L1 for the specified call_type

DEPENDENCIES

RETURN VALUE


SIDE EFFECTS
  none

===========================================================================*/

void cmss_send_ims_reg_capability(
  cm_call_type_mask_type  call_type_mask, 
  /* Indicate the call type received from IMS */
  boolean                 ims_reg_supp,
  /* Indicates if IMS is registered for above call_type */
  sys_modem_as_id_e_type  asubs_id
  /* Sub id on which IMS registration status is recieved*/
);
/*===========================================================================

FUNCTION cmss_notify_ds_cs_domain_send_ss_event

DESCRIPTION
  Send SS Event to clients - effective service domain is CS only.

DEPENDENCIES
  None

RETURN VALUE
  boolean

SIDE EFFECTS
  None.

===========================================================================*/
void cmss_notify_ds_cs_domain_send_ss_event(
  sys_modem_as_id_e_type  asubs_id
);

/*===========================================================================

FUNCTION cmss_is_cs_srv_available

DESCRIPTION
  Check if STACK 0 has CS service.
  If so, return back sys mode and roam status for the passed sub

DEPENDENCIES
  None
  
RETURN VALUE
  boolean - CS service is available or not

SIDE EFFECTS
  none

===========================================================================*/
extern boolean cmss_is_cs_srv_available
(
  sys_modem_as_id_e_type    as_id,
   /* as id for which srv info is required */
  cm_srv_state_info_s_type *srv_info
   /* output parameter: contains srv params */

);

/*===========================================================================
@FUNCTION cmss_send_ims_handover_ready_ind

@DESCRIPTION
Will update IMS that LTE service is acquired with emergency capability and 
IMS can start the handover procedure from WLAN to LTE. 

@DEPENDENCIES

None

@RETURNS
  None

@SIDE_EFFECT

None

===========================================================================*/
void cmss_send_ims_handover_ready_ind
(
 
  sys_modem_as_id_e_type  as_id,
  /* Handover ready indication to be sent per sub. */

  sys_sys_mode_e_type         system,
  /**< Currently only support SYS_SYS_MODE_LTE **/ 

  sys_srv_status_e_type      srv_status
  /* Service status of the UE */

);


/*===========================================================================

FUNCTION cmss_srlte_csfb_mm_info_backup

DESCRIPTION
  reset mm info on stack0 and take backup of stack2's mm info on to stack0's prev mm info

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/
extern void cmss_srlte_csfb_mm_info_backup(
  sys_modem_as_id_e_type  asubs_id
);

/*===========================================================================

FUNCTION cmss_csfb_srlte_mm_info_backup

DESCRIPTION
  reset mm info on stack2 and take backup of stack0's mm info on to stack2's prev mm info

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/
extern void cmss_csfb_srlte_mm_info_backup(
  sys_modem_as_id_e_type  asubs_id
);
/*===========================================================================

FUNCTION cmss_report_stop_mode

DESCRIPTION
  Informs Stop mode by changing srv domain through msg rtr

DEPENDENCIES

RETURN VALUE


SIDE EFFECTS
  none

===========================================================================*/
void cmss_report_stop_mode
(
  sys_modem_as_id_e_type  asubs_id
);

/*=====================================================================

  FUNCTION cmss_is_cdma_time_chgd

  DESCRIPTION
    Tells if the time info of CDMA/HDR is changed

  DEPENDENCIES

  RETURN VALUE
    true if changed
    false if not

  SIDE EFFECTS
    none
===================================================================*/
boolean cmss_is_cdma_time_chgd(const sys_time_info_s_type *old_time,
const sys_time_info_s_type *new_time);

/*===========================================================================

FUNCTION cmss_get_true_plmn_id

DESCRIPTION
  Get the true plmn.

RETURN VALUE
  sys_plmn_id_s_type

===========================================================================*/
sys_plmn_id_s_type  cmss_get_true_plmn_id(
  mm_sub_stk_id_s_type mm_id
);
#endif /* CMSS_H */
