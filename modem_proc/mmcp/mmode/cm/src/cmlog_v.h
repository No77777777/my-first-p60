#ifndef CM_LOG_V_H
#define CM_LOG_V_H
/**
  @file cmlog_v.h

  @brief This module contains functions for logging CM packets.
*/
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                            CM Logging Facilities

GENERAL DESCRIPTION
   This module contains functions for logging CM packets.

Copyright (c) 2002  - 2010 by Qualcomm Technologies Incorporated. All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/mmcp.mpss/7.9.0/mmode/cm/src/cmlog_v.h#1 $

when       who      what, where, why
--------   ---      ---------------------------------------------------------
10/21/13   jvo      Modified cmlog_event_ssac_timer_s_type to match unaligned
                    qxdm event payload
08/06/13   xs       Add function cmlog_csfb_svlte_switch() to log switch between
                    SVLTE and CSFB
04/12/12   fj       Separate call_orig and call_connect log structure for DSDS
                    and non-DSDS.
05/22/12   ab       Reverted back the changes for WLAN as
                    QXDM tool is not able to parse the CM SS event
01/06/12   gm       Data synchronization changes for freefloating platform
12/13/11   sg       Log packet updates TDS
07/24/11   aj       SC SVLTE 1.0 merge to mainline
04/29/11   rm       Adding req/resp packet for CDMA info
04/21/11   sv       Correct Lint Errors
04/13/11   rm       Moving LOG_CM_CALL_EVENT_C to cmlog.h
12/23/10   sv       Integrate DSDS feature
07/12/10   mj       LTE log packet updates
11/16/09   ak       Rectified command value definitions.
11/04/09   ak       Added support for Ecall commands.
07/01/09   fj       Changes due to LTE merge.
05/27/09   aj       Adding support for updating clients with cdma local time
05/27/09   aj       Adding support for passing otasp status to clients
05/15/09   sg       Update SS EVENT log packet with new SS events added in CM
02/02/09   ks       Adding support for QDSP 6 based compiler
01/29/09   fj       Remove FEATURE_HDR_HYBRID for call events
03/17/08   am       Added two diag commands to support
                    commands for CM_CALL_CMD_MODIFY_REQ / RSP
01/03/08   ka       Adding handset based plus code dialing feature.
04/15/07   rp       Removing FEATURE_MMGPS
11/17/06   jqi      Added support for HDR active protocol revision information.
11/09/06   rp       Added support for SS req/response packet
                    CM_GET_CELL_SERVICE_IND.
04/19/06   jqi      Added diag subsystem command(select system preference
                    command2) to process 64-bit band preference.
03/10/06   pk       Added support for IMSI and MCC for IS 95 systems. Changed
                    the changed fields of the ss log packet to 64 bits
01/05/06   pk       Modified the SS log packet structure. Replaced constants
                    with hard coded values, repalced boolean with uint8
12/10/05   ic       Lint cleanup
11/21/05   ic       Send EVENT_CM_OPERATIONAL_MODE on transition from
                    SYS_OPRT_MODE_OFFLINE_CDMA to SYS_OPRT_MODE_RESET.
                    Fix the functionality that postpones sending of
                    EVENT_CM_OPERATIONAL_MODE for SYS_OPRT_MODE_OFFLINE_CDMA
                    until after TASK_OFFLINE_SIG from TMC comes (CR73050).
10/27/05   pk       Added support for SS log packet and its data structures
06/21/05   ic       Renamed DIAG_MAX_GSM_DIGITS to CM_DIAG_MAX_GSM_DIGITS
06/10/05   sk       Moved user defined data types from cmlog.c to here.
01/24/05   ic       Hardcoded array sizes in declarations of phone and call
                    event log packets
01/14/05   ic       Added comments about contents of num field in
                    LOG_CM_CALL_EVENT_C_type declaration
01/06/05   ic       Mainlined FEATURE_CM_CALL_EVENT_LOGGING and
                    FEATURE_CM_PH_EVENT_LOGGING.
01/05/05   ic       Log packet version for call and phone event packets
12/20/04   ic       In LOG_CM_PH_EVENT_C_type and LOG_CM_CALL_EVENT_C_type
                    declarations changed all int to int32
12/19/04   ic       Added support for sending LOG_CM_CALL_EVENT_C under
                    FEATURE_CM_CALL_EVENT_LOGGING
12/10/04   ic       Refinments for CR: 48569 - HDR-KDDI: Request for new EVENTs
                    for DDTM mode (LOG_CM_PH_EVENT_C)
12/06/04   ic       Include log.h instead of log_dmss.h to correct RVCT
                    compiler error on 6250 ("could not find log_dmss.h")
12/05/04   ic       Put definition of LOG_CM_PH_EVENT_C_type under
                    FEATURE_CM_PH_EVENT_LOGGING
12/05/04   ic       Added support for sending LOG_CM_PH_EVENT_C
08/10/04   ic       Removed cmlog_get_operating_mode() and cmlog_system_mode()
                    as they became obsolete.
03/18/04   ka       Prototype for cmlog_set_cm_offline_signal_recvd ()
02/06/04   ws       Initial jaguar/mainline merge.
05/21/03   vt       Added new call events.
03/21/03   ATM      Added event logging for svc_cnf and nw_list_cnf
02/12/03   atm      Added pref logging and moved subsys command from UMTS to
                    CM.
01/21/03   ws       Updated copyright information for 2003
04/26/02   atm      Initial version.
===========================================================================*/

#include "mmcp_variation.h"
#include "cm.h"
#include "comdef.h"
#include "sys.h"
#include "sys_plmn_selection.h"
#include "cmcall.h"
#include "log.h"
#include "cmsoa.h"

#define CM_LOG_PACKET_CONTENTS_NOT_DEFINED   0
/**<
  Filler for log packet fields when there is
  nothing meaningful to write into them
*/

#define CM_LOG_PH_EVENT_LOG_PACKET_VERSION   6
/**<
  Version number for LOG_CM_PH_EVENT_C_type
  New fields are to be added at the end of the structure only
  (DIAG and tools requirement).
  Whenever a new field is added to the structure, the value of
  the constant below needs to be increased by 1 and
  DIAG and tools group need to be notified about the changes.
*/

#define CM_LOG_CALL_EVENT_LOG_PACKET_VERSION 3
/**<
  Version number for LOG_CM_CALL_EVENT_C_type
  New fields are to be added at the end of the structure only
  (DIAG and tools requirement).
  Whenever a new field is added to the structure, the value of
  the constant below needs to be increased by 1 and
  DIAG and tools group need to be notified about the changes.
*/

#define CM_LOG_SS_EVENT_LOG_PACKET_VERSION 8
/**<
  Version number for LOG_CM_SS_EVENT_C_type
  New fields are to be added at the end of the structure only
  (DIAG and tools requirement).
  Whenever a new field is added to the structure, the value of
  the constant below needs to be increased by 1 and
  DIAG and tools group need to be notified about the changes.
*/

#define CM_LOG_SS_EVENT_MSIM_LOG_PACKET_VERSION 1

#define CM_LOG_STATS_EVENT_LOG_PACKET_VERSION 2
/*===========================================================================
                                     Datatypes
===========================================================================*/

/**
  Data structure used to log CM phone event SUBS_INFO.
  We log current PH event along with what's in cm_subs_info_s_type
*/
LOG_RECORD_DEFINE(LOG_CM_SUBS_EVENT_C)

byte                  subscription_id;
/**< Active Subscription ID assigned to this subscription
*/

int32                 mode_pref;
/**<
  Switch to this mode preference:
  - CM_MODE_PREF_GSM_ONLY
  - CM_MODE_PREF_WCDMA_ONLY
  - CM_MODE_PREF_GSM_WCDMA_ONLY
  - CM_MODE_PREF_CDMA_ONLY
  - CM_MODE_PREF_HDR_ONLY
  - CM_MODE_PREF_CDMA_HDR_ONLY
  - CM_MODE_PREF_PERSISTENT
  - CM_MODE_PREF_NO_CHANGE
  Only the above modes are allowed.
*/

int32                 pref_term;
/**<
  Indicate the current mode preference term -
  one-call, power-cycle, etc.
*/

int32                 acq_order_pref;
/**< gw acquisition order preference
*/

int32                 srv_domain_pref;
/**<
  Switch to this service domain (e.g. Circuit Switch)
  Set to CM_SRV_DOMAIN_PREF_NO_CHANGE if a service
  domain is not being changed.
*/

int64                 band_pref;
/**< Switch to this band preference
*/

int32                 roam_pref;
/**< Indicate the current roam preference
*/

int32                 hybr_pref;
/**< Indicate the hybr preference setting
*/

int32                 network_sel_mode_pref;
/**<
  Defines whether the network should be selected automatically
  or manually
*/

PACKED struct PACKED_POST
{
  byte                 identity[3];
} plmn;
/**< If network selection is set to manual, this specifies plmn id
*/

int32                 prl_pref;
/**< The current prl preferred setting
*/

uint8                 sim_id[50];
/**< SIM id of the card to which this subscription belongs
*/

PACKED struct PACKED_POST
{
  uint32                data_len;

  uint8                 data_ptr[32];
} app_id;
/**< Application ID (AID) assocuated with this subscription
*/

uint16                             session_type;
/**< UIM session type on which this subscription is currently provisioned
*/

boolean                           is_operational;
/**< Flag indicating if this subscription is in operation right now
*/

boolean                           is_priority_subs;
/**< Flag indicating if this subscription is the priority subscription
*/

boolean                           is_default_voice;
/**< Flag indicating if this subscription is the default voice subs
*/

boolean                           is_default_data;
/**< Flag indicating if this subscription is the default data subs
*/


LOG_RECORD_END


/**
  Data structure used to log CM phone events.
  We log current PH event along with what's in cm_ph_info_s_type
*/
LOG_RECORD_DEFINE(LOG_CM_PH_EVENT_C)

uint8                  version;
/**< Version of the packet (CM_LOG_PH_EVENT_LOG_PACKET_VERSION)
*/

int32                  ph_event;
/**< PH event, as defined by cm_ph_event_e_type
*/

boolean                is_in_use;
/**< Indicate whether phone is currently in use
*/

int32                  oprt_mode;
/**< Indicate current operating mode (sys_oprt_mode_e_type)
*/

int32                  test_control_type;
/**< Indicate current test control type (cm_test_control_type_e_type)
*/

int32                  cdma_lock_mode;
/**<
  Indicate whether phone is CDMA locked until power cycle
  (cm_cdma_lock_mode_e_type)
*/

int32                  answer_voice;
/**<
  Indicate whether phone should answer incoming
  voice calls as voice, fax or modem. CDMA only (cm_answer_voice_e_type)
*/

int32                  mode_pref;
/**< Indicate current mode preference (cm_mode_pref_e_type)
*/

int32                  pref_term;
/**<
  Indicate the current preference term - one-call, power-cycle, etc.
  (cm_pref_term_e_type)
*/

int32                  acq_order_pref;
/**<
  The acquisition order preferred by the client. GSM/WCDMA only.
  (cm_gw_acq_order_pref_e_type)
*/

int32                  srv_domain_pref;
/**<
  The service domain preferred by the client.
  This field is set after a call to cm_ph_cmd_sys_sel_pref_new.
  GSM/WCDMA only (cm_srv_domain_pref_e_type).
*/

int64                  band_pref;
/**<
  Indicate current band preference (cm_band_pref_e_type)
  for CDMA, GSM, WCDMA, systems
*/

int32                  roam_pref;
/**< Indicate current roam preference (cm_roam_pref_e_type)
*/

int32                  nam_sel;
/**< Indicate the NAM selection. CDMA only (cm_nam_e_type)
*/

int32                  curr_nam;
/**<
  Indicate the current NAM -
  needed when NAM selection is set to AUTO-NAM. CDMA only
  (cm_nam_e_type)
*/

int32                  packet_state;
/**< Indicate current packet state. CDMA only (cm_packet_state_e_type)
*/

int32                  hybr_pref;
/**< Indicate hybrid preference (cm_hybr_pref_e_type)
*/

int32                  network_sel_mode_pref;
/**<
  The network selection mode selected by the client. GSM/WCDMA only
  (cm_network_sel_mode_pref_e_type)
*/

PACKED struct PACKED_POST
{
  byte                 identity[3];
} plmn;
/**<
  The identifier of the PLMN to be selected manually. GSM/WCDMA only
  (sys_plmn_id_s_type)
*/

PACKED struct PACKED_POST
{
  byte                 length;
  /**< The number of PLMNs in the list below
  */

  PACKED struct PACKED_POST
  {

    PACKED struct PACKED_POST
    {
      byte             identity[3];
    } plmn;
    /**< (sys_plmn_id_s_type)
    */

    uint16             access_tech;
    /**< sys_access_tech_e_type
    */

  } info[ 50 ];
  /**<
    (sys_user_pref_plmn_list_info_s_type)
    Hardcoded to the value of SYS_USER_PPLMN_LIST_MAX_LENGTH
    at the time log packet was declared.
    This is intentional as otherwise changes to the constant would
    change byte ordering in the log packet.
  */

} user_pref_networks;
/**<
  The user preferred network list. GSM/WCDMA only
  (sys_user_pref_plmn_list_s_type)
*/

PACKED struct PACKED_POST
{

  uint32               length;
  /**< The number of PLMNs in the list
  */

  PACKED struct PACKED_POST
  {

    PACKED struct PACKED_POST
    {
      byte             identity[3];
    } plmn;
    /**< The ID of the PLMN (sys_plmn_id_s_type)
    */

    int32              rat;
    /**< The radio access technology of the PLMN (sys_radio_access_tech_e_type)
    */

    int32              plmn_service_capability;
    /**<
      Indicates the type of service domain the PLMN can provide
      (sys_srv_capability_e_type)
    */

    boolean            plmn_forbidden;
    /**< Indicates if the PLMN is forbidden
    */

    int32              list_category;
    /**< Indicates the type of PLMN (sys_detailed_plmn_list_category_e_type)
    */

    int32              signal_quality;
    /**< The signal quality of the PLMN (sys_signal_quality_e_type)
    */

    int32              signal_strength;
    /**< The signal strength of the PLMN (sys_signal_strength_type)
    */

  } info[ 40 ];
  /**<
    (sys_detailed_plmn_list_info_s_type)
    Hardcoded to the value of SYS_PLMN_LIST_MAX_LENGTH
    at the time log packet was declared.
    This is intentional as otherwise changes to the constant would
    change byte ordering in the log packet.
  */

} available_networks;
/**< Available networks. GSM/WCDMA only (sys_detailed_plmn_list_s_type)
*/

boolean                is_cdma_subscription_available;
/**<
  Indicate whether the cdma subscription/provisioning information
  is available for consumption.
*/

boolean                is_gwl_subscription_available;
/**<
  Indicate whether the GSM/WCDMA/LTE subscription/provisioning
  information is available for consumption.
*/

int32                  rtre_config;
/**<
  Current RTRE configuration - RUIM, NV, RUIM fallback NV. CDMA only
  (cm_rtre_config_e_type)
*/

int32                  rtre_control;
/**< Current RTRE control - RUIM or NV. CDMA only (cm_rtre_control_e_type)
*/

int32                  orig_srv_domain_pref;
/**<
  Service domain preference for PS attach/detach commands.
  This is different from the phone service domain preference
  (cm_srv_domain_pref_e_type)
*/

int32                  ddtm_pref;
/**< Data Dedicated Tranmission mode (DDTM) preference (cm_ddtm_pref_e_type).
*/

boolean                cur_ddtm_status;
/**< ddtm status
*/


int32                  ddtm_act_mask;
/**< DDTM action mask settings (sys_ddtm_act_mask_e_type).
*/

uint16                 ddtm_num_srv_opt;
/**< Number of service options specified in below list.
*/

uint16                 ddtm_srv_opt_list[ 10 ];
/**<
  List of service options whose pages have to be ignored provided
  ddtm_pref & ddtm_act_mask allows it (sys_srv_opt_type).
  Hardcoded to the value of SYS_DDTM_MAX_SO_LIST_SIZE
  at the time log packet was declared.
  This is intentional as otherwise changes to the constant would
  change byte ordering in the log packet.
*/

int32                  cphs_allowed;
/**<
  Whether Common PCN Handset Specifications procedures are allowed
  FEATURE_ALS (cm_cphs_proc_e_type)
*/

int32                  als_allowed;
/**<
  Whether ALS procedures are allowed.
  FEATURE_ALS (cm_als_proc_e_type)
*/

int32                  line;
/**<
  Currently selected line (all MO voice calls will use it)
  FEATURE_ALS (cm_als_line_e_type)
*/

int32                  line_switching;
/**<
  Whether the user is allowed to switch between lines
  FEATURE_ALS (cm_als_line_switching_e_type)
*/

int32                  requesting_client_id;
/**< Requesting client (cm_client_id_type)
*/

PACKED struct PACKED_POST
{

  uint8                type;
  /**< Message type
  */

  uint32               payload_len;
  /**< Message payload length in bits max is SYS_MAX_IS2000_MSG_SIZE * 8
  */

  byte                 payload[ 256 ];
  /**< Message payload
  */
} cdma_capability;
/**<
  CDMA capability in the form of IS2000 Status Message
  (sys_is2000_msg_s_type)
  FEATURE_UMTS_1X_HANDOVER_1XMSM || FEATURE_UMTS_1X_HANDOVER_UMTSMSM
  otherwise filled with 0s.
  Hardcoded to the value of SYS_MAX_IS2000_MSG_SIZE
  at the time log packet was declared.
  This is intentional as otherwise changes to the constant would
  change byte ordering in the log packet.
*/

int32                  msm_id;
/**<
  Identifies the MSM which generated this event (sys_msm_id_e_type).
  FEATURE_UMTS_1X_HANDOVER_1XMSM || FEATURE_UMTS_1X_HANDOVER_UMTSMSM
  otherwise filled with 0s
*/


int32                  orig_ddtm_act_mask;
/**< DDTM action mask settings (sys_ddtm_act_mask_e_type)
*/

int64                  lte_band_pref;
/**< Indicate current LTE band preference (cm_band_pref_e_type)
*/

byte                   asubs_id;

boolean                is_hybr_gw_subscription_available;
/**<
  Indicate whether the GSM/WCDMA subscription/provisioning information
  is available for consumption
*/

boolean                is_hybr_gw3_subscription_available;
/**<
  Indicate whether the GSM/WCDMA subscription/provisioning information
  is available for consumption
*/

byte                   standby_pref;
/**< Standby preference of the Phone
*/

byte                   active_subs;
/**<
  The active subscription in Single Standby mode.
  Only valid when standby_pref is SYS_MODEM_DS_PREF_SINGLE_STANDBY
*/

byte                   default_voice_subs;
/**< Default voice subscription
*/

byte                   default_data_subs;
/**< Default data subscription
*/

byte                   priority_subs;
/**< Priority subscription
*/

int64                  tds_band_pref;
/**<
  Indicate current TDS band preference (cm_band_pref_e_type) Used
  with CM_BAND_PREF_TDS_* values.
*/
int64                  lte_band_pref_65_128;
/**< Indicate current LTE band preference (cm_band_pref_e_type)
*/
int64                  lte_band_pref_129_192;
/**< Indicate current LTE band preference (cm_band_pref_e_type)
*/
int64                  lte_band_pref_193_256;
/**< Indicate current LTE band preference (cm_band_pref_e_type)
*/

LOG_RECORD_END

LOG_RECORD_DEFINE(LOG_CM_SS_MSIM_EVENT_C)
uint8                     version;
/**< Version of the packet (CM_LOG_SS_MSIM_EVENT_LOG_PACKET_VERSION)
    */
int32                      ss_event;
/**< SS event, as defined by cm_ss_event_e_type
*/

int32                     asubs_id;

uint8                     number_of_stacks ;

PACKED struct PACKED_POST
{
  uint64                                  changed_fields ;
  uint64                                  changed_fields2 ;
  uint64                                  signal_strength_changed_fields ;
  boolean                                 is_operational ;

  int32                                   srv_status;
  /**<
    The service status (SERVICE/NO SERVICE) of the system.
    Mask used is CM_SS_SRV_STATUS_MASK.
  */
  int32                                   true_srv_status ;
  /**< CM_SS_EVT_TRUE_SRV_STATUS_MASK               changed_fields
  */

  int32                                   srv_domain;
  /**<
    The service domain registered on the system.
    Mask used is CM_SS_SRV_DOMAIN_MASK.
  */

  int32                                   srv_capability;
  /**<
    The system's service capability.
    Mask used is CM_SS_SRV_CAPABILITY_MASK.
  */
  int32                                   sys_mode;
  /**<
    The system's mode.
    Mask used is CM_SS_SYS_MODE_MASK.
  */
  PACKED union PACKED_POST
  {

    PACKED struct PACKED_POST
    {
      uint8                  bit_err_rate;
      /**< Bit Error Rate / RX QUAL in GSM
      */

      int32                  sim_state;
      /**<
        Current state of the SIM.
        Mask used is CM_SS_SIM_STATE_MASK.
        sys_sim_state_e_type
      */

      uint8                plmn_forbidden;
      /**<
        Indicates whether the PLMN is forbidden.
        Mask used is CM_SS_PLMN_FORBIDDEN_MASK.
      */

      PACKED struct PACKED_POST
      {
        /*
          Flags that indicate whether the particular mm information is
          available.
        */
        uint8              plmn_avail;
        uint8              full_name_avail;
        uint8              short_name_avail;
        uint8              univ_time_and_time_zone_avail;
        uint8              time_zone_avail;
        uint8              lsa_identity_avail;
        uint8              daylight_saving_adj_avail;

        PACKED struct PACKED_POST
        {
          byte               identity[3];
        } plmn;

        PACKED struct PACKED_POST
        {
          uint8              length;
          /**< Number of characters in the full name
          */

          int32              coding_scheme;
          /**<
            The coding scheme used to encode the full name of the network
            sys_network_name_coding_scheme_type_e_type
          */

          uint8            add_country_initials;
          /**<
            Flag that indicates whether MS should add the country initials
            to the full name of the network
          */

          uint8              name[255];
          /**< The full name of the network
          */

          /**<
            name.
            Hardcoded to the value of SYS_MAX_FULL_NAME_CHARS
            at the time log packet was declared.
            This is intentional as otherwise changes to the constant would
            change byte ordering in the log packet.
          */

        } full_name;

        PACKED struct PACKED_POST
        {

          uint8              length;
          /**< Number of characters in the full name
          */

          int32              coding_scheme;
          /**<
            The coding scheme used to encode the full name of the network
            sys_network_name_coding_scheme_type_e_type
          */

          uint8            add_country_initials;
          /**<
            Flag that indicates whether MS should add the country initials
            to the full name of the network
          */

          uint8              name[255];
          /**< The full name of the network
          */

          /**<
            name.
            Hardcoded to the value of SYS_MAX_SHORT_NAME_CHARS
            at the time log packet was declared.
            This is intentional as otherwise changes to the constant would
            change byte ordering in the log packet.
          */

        } short_name;

        PACKED struct PACKED_POST
        {

          uint8             year;
          uint8             month;
          uint8             day;
          uint8             hour;
          uint8             minute;
          uint8             second;
          int               time_zone;
          /**< sys_time_zone_type
          */

        } univ_time_and_time_zone;

        int                  time_zone;
        /**< sys_time_zone_type
        */
        PACKED struct PACKED_POST
        {
          int32              type;
          /**< sys_lsa_identity_type_e_type
          */

          uint8              identity[3];

        } lsa_identity;

        int32                daylight_saving_adj;
        /**< sys_daylight_saving_adj_e_type
        */

      } mm_information;
      /**<
        Mobility management information, i.e. network name, date,
        time and time zone of registered network
      */

    } gw_info;
    /**< GSM/WCDMA SS information
    */

    PACKED struct PACKED_POST
    {


      int32                  band_class;
      /**<
        cellular or PCS.
        sys_band_class_e_type
      */

      int32                  block_or_system;
      /**<
        PCS frequency block: A to F Cellular System A or B.
        sys_blksys_e_type
      */

      uint16                 reg_zone;
      /**< 12 bit registration zone
      */

      uint16                 packet_zone;
      /**<
        8 bit packet zone, 0xffff means PACKET ZONE NONE.
        Mask used is CM_SS_PACKET_ZONE_MASK.
      */

      uint8                  bs_p_rev;
      /**<
        8 bit base station p_rev.
        Mask used is CM_SS_BS_P_REV_MASK.
      */

      uint8                  p_rev_in_use;
      /**<
        8 bit p_rev in use the lessor of BS p_rev and mobile p_rev.
        Mask used is CM_SS_P_REV_IN_USE_MASK.
      */

      uint8                  is_registered;
      /**< Is mobile registered on the current system
      */

      uint8                  ccs_supported;
      /**< Is concurrent services supported
      */

      int32                  uz_id;
      /**<
        User zone Identification of the selected user zone,
        == CM_INVALID_UZID, if no user zone selected. FEATURE_USERZONE.
        Mask used is CM_SS_UZ_CHANGED_MASK.
      */

      PACKED struct PACKED_POST
      {
        byte                 uz_name_encoding;

        byte                 uz_name_n_octets;

        byte                 uz_name[32];
        /**<
          uz_name.
          Hardcoded to the value of SYS_PUZL_MAX_UZ_NAME_LEN
          at the time log packet was declared.
          This is intentional as otherwise changes to the constant would
          change byte ordering in the log packet.
        */

      } uz_name;
      /**<
        User zone name of the selected user zone.
        Valid only if uz_id != CM_INVALID_UZID. FEATURE_USERZONE.
        Mask used is CM_SS_UZ_CHANGED_MASK.
      */

      byte                   srch_win_n;
      /**<
        Neighbour set window search size.
        Mask used is CM_SS_BASE_STATION_PARMS_CHGD_MASK.
      */

      int32                  base_lat;
      /**<
        Base station latitude[0.25sec and already sign extended].
        Mask used is CM_SS_BASE_STATION_PARMS_CHGD_MASK.
        sys_base_lat_type
      */

      int32                  base_long;
      /**<
        Base station latitude[0.25sec and already sign extended].
        Mask used is CM_SS_BASE_STATION_PARMS_CHGD_MASK.
        sys_base_long_type
      */

      word                   base_id;
      /**<
        Base station Identification number.
        Mask used is CM_SS_BASE_STATION_PARMS_CHGD_MASK.
        sys_base_id_type
      */

      PACKED struct
      {
        int32                  sys_mode;
        /**< Indicates the system from which the time was obtained (sys_sys_mode_e_type)
        */
        PACKED union
        {
          PACKED struct
          {
            uint8                lp_sec;
            int8                 daylt_savings;
            int16                ltm_offset;
          } cdma_time;

          PACKED struct
          {
            uint8                lp_sec;
            int16                ltm_offset;
          } hdr_time;

        } time;

      } time_info;
      /**<
        Time obtained from sync channel / sector parameter msg
        Mask used is CM_SS_CDMA_TIME_CHGD_MASK.
      */

    } cdma_info;
    /**< CDMA SS information
    */
  } mode_info ;


  int32                      roam_status;
  /**<
    Indicate current roaming status.
    Mask used is CM_SS_ROAM_STATUS_MASK.
  */
  PACKED struct PACKED_POST
  {
    int32                    id_type;

    PACKED union PACKED_POST
    {
      PACKED struct PACKED_POST
      {
        uint16               sid;
        /**< The system ID
        */

        uint16               nid;
        /**< The network ID
        */

      } is95;
      /**< IS 95 sys id
      */

      byte                   is856[16];
      /**< HDR sys id
      */

      PACKED struct PACKED_POST
      {
        byte                 identity[3];

      } plmn;
      /**< UMTS (WCDMA/GSM) system ID
      */

      PACKED struct PACKED_POST
      {
        uint8                len;
        /**< Length of the SSID, if == 0, then SSID = broadcast SSID
        */
        char                 ssid[32];
        /**< SSID of the wlan system
        */

        /**<
          ssid.
          Hardcoded to the value of SYS_WLAN_SSID_MAX_SIZE
          at the time log packet was declared.
          This is intentional as otherwise changes to the constant would
          change byte ordering in the log packet.
        */

      } ssid;
      /**< WLAN system Id
      */

      PACKED struct PACKED_POST
      {
        PACKED struct PACKED_POST
        {
          byte               identity[3];
        } plmn;

        word                 lac;
        /**< LAC
        */
      } plmn_lac;
      /**< UMTS (WCDMA/GSM) PLMN and LAC ID
      */
    } id;

  } sys_id;

  uint8                    is_sys_forbidden;
  /**<
    Indicates whether the System is forbidden.
    Mask used is CM_SS_SYS_FORBIDDEN_MASK.
  */

  uint16                  rssi;
  /**<
    RSSI in positive dBm i.e. 75 means -75dBm. The range for
    - AMPS:     -110 to -89 dBm
    - 800 CDMA:  -105 to -90 dBm
    - 1900 CDMA: -108 to -93 dBm
    - GSM/WCDMA: -105 to -60 dBm
  */

  int16                     ecio;
  int                       io;
  byte                      sinr;
  /**<
    ECIO in negative 0.5 dBm i.e. 63 means -31.5dBm.
    Io field is valid only when in HDR mode only operation
    SINR range: from 0 to 8, Level 8 represents highest SINR
    SINR applicable only when sys_mode = HDR.
  */

  int16                  pathloss;
  /**< Pathloss is applicable only in Connected mode (otherwise 0xFF)
  */

  int16                  sir;
  /**<
    SIR applicable and valid (otherwise 0xFF) only in WCDMA
    connected mode when DPCCH is active.
  */

  int16                      rssi2;
  /**< The -ve RSSI value, coming from RRC layer
  */

  int16                      rscp;
  /**< The range of RSCP would be from -28 to -121 dBm
  */

  boolean                  ecio_for_display;
  /**< TRUE if UE is in Data call
  */

  int32                      psist;
  /**<
    Indicate psist of the system.
    Mask used is CM_SS_PSIST_CHGD_MASK.
    cm_psist_e_type
  */

  int32                       active_band;
  /**<
    Clients need to check for CM_API_EMERGENCY_NUM_LIST before
    using this structure.
  */

  uint16                      active_channel;
  /**< Report active channel of the current serving system.
  */
  PACKED struct PACKED_POST
  {

    uint8                  ss_thr_status;
    /**< The throttle status of the current ss
    */

    dword                    thr_uptime;
    /**< The throttling time period
    */

    unsigned int             idx;
    /**<
      The index in the throttle_time table.
      Used internally by CM to keep track of the next throttle time
      increment
    */

    int32                    call_status;
    /**<
      The call end reason
      cm_ss_orig_thr_reasons_e_type
    */

  } thr_status;

  PACKED struct PACKED_POST
  {
    uint32                   cell_id;
    /**< Cell_id
    */

  } cell_info;

  PACKED struct PACKED_POST
  {
    int32                       hs_ind;
    /**< High speed service indication (Over WCDMA)
    */

    int32                       dtm_supp;
    /**< Dual Transfer mode indication (Over GSM)
    */

    int32                       egprs_supp;
    /**< EGPRS indication
    */

    int32                       hs_call_status;
    /**<
      Clients need to check for CM_API_CELL_SUPP_SERVICES
      and CM_API_CELL_SUPP_SERVICES_HS_CALL before
      using this structure
    */

  } cell_srv_ind;

  PACKED struct PACKED_POST
  {
    int32                   reject_srv_domain;
    /**< Srv domain on which registration is rejected
    */

    uint8                   reject_cause;
    /**< Registration Reject Cause
    */

  } reg_reject_info;

  int32   cipher_domain;
  /**<
    Can be used only if CM_CIPHERING_IND_SUPP is defined.
    Ciphering on CS or PS domain or both.
    sys_srv_domain_e_type
  */

  int32                    prev_idle_digital_mode;
  /**<
    Previous idle digital mode. FEATURE_HDR_HANDOFF.
    Mask used is CM_SS_IDLE_DIGITAL_MODE_MASK.
    sys_sys_mode_e_type
  */

  int32                    cur_idle_digital_mode;
  /**<
    Current idle digital mode. FEATURE_HDR_HANDOFF.
    Mask used is CM_SS_IDLE_DIGITAL_MODE_MASK.
    sys_sys_mode_e_type
  */

  uint8                               bcmcs_srv_supported ;
  /**< CM_SS_EVT_BCMCS_SRV_SUPPORTED_MASK changed_fields
  */

  int32                               bcmcs_srv_status ;


  PACKED union PACKED_POST
  {
    PACKED struct PACKED_POST
    {
      word                             lac ;

      uint8                            rac_or_mme_code ;

      PACKED struct PACKED_POST
      {
        uint32                         csg_id ;

        uint8                          hybrid_cell ;
      } csg_info ;

    } gsm_mode_info ;

    PACKED struct PACKED_POST
    {

      uint16                          cdma_reg_prd ;
      /**< NO MASK
      */

      boolean                         is_colloc ;
      /**< NO MASK
      */

      uint16                        frame_err_rate ;
      /**< CM_SS_EVT_FER_MASK changed_fields
      */

    } cdma_mode_info ;

    PACKED struct PACKED_POST
    {
      int32                           hdr_active_prot ;

      uint8                           subnet_mask_len ;

      int32                           hdr_ln_status  ;

      uint16                          hdr_packet_err_rate ;

      int32                           hdr_personality  ;

      uint8                           is_colloc ;

      uint8                           hdr_hybrid ;

    } hdr_mode_info ;

    PACKED struct PACKED_POST
    {
      word                           lac ;

      uint8                           rac_or_mme_code ;

      PACKED struct PACKED_POST
      {
        uint32                         csg_id ;

        uint8                          hybrid_cell ;
      } csg_info ;
    } wcdma_mode_info ;

    PACKED struct PACKED_POST
    {
      int16                             rsrp ;
      int8                              rsrq ;
      word                              lac ;
      word                              tac ;
      uint8                             rac_or_mme_code ;

      PACKED struct PACKED_POST
      {
        uint8                      embms_coverage_status ;

        int16                          trace_id ;
      } embms_info ;


      PACKED struct PACKED_POST
      {
        uint32                         csg_id ;

        uint8                          hybrid_cell ;
      } csg_info ;

      uint8                           voice_support_on_lte ;
    } lte_mode_info ;

    PACKED struct PACKED_POST
    {
      word                           lac ;

      uint8                           rac_or_mme_code ;

      PACKED struct PACKED_POST
      {
        uint32                         csg_id ;

        uint8                          hybrid_cell ;
      } csg_info ;
    } tds_mode_info ;
  } rat_mode_info ;

} stack_info[2];


uint64                              changed_fields_subs;

uint8                               is_csfb_call_active ;

dword                               curr_uptime ;

uint8                               bsr_in_progress ;

int32                               eutra_detect_status ;

int                                 requesting_client_id ;

int32                               msm_id ;

PACKED struct PACKED_POST
{
  int32                     reg_status;
  /**< IP app registration status
  */

  int                       cause;
  /**< Cause given when app registration fails or deregisters
  */

  PACKED struct PACKED_POST
  {
    boolean                  present;
    /**<
      TRUE   - Warning is present
      FALSE  - Warning is not present
    */

    int                       cause;
    /**< Warning code - SIP RFC 3261, sect 20.43
    */

    char                      hostname[20];
    /**<
      Null terminated host name that
      generates the warning
    */

    char                      description[20];
    /**< Null terminated Warning text.
    */

  } warn_info;
  /**< Warning information given by ipapp
  */

  uint64                      call_type_mask[CMIPAPP_CAP_ARR_MAX];
  /**< Array of Call tpyes that ipapp can service
  */

  int32                       sys_mode[CMIPAPP_CAP_ARR_MAX];
  /**<
    System modes app can support for calls.
    Can be used only if CM_API_SMS_OVER_IP is defined.
    corresponding to the call_type (one to one)
  */

} ipapp_info ;

uint16                    current_mcc ;

uint16                    home_mcc ;

int32                     plus_dial_setting ;

PACKED struct PACKED_POST
{
  int32                    gan_state;
  /**<
    Indicate GAN state.
    Mask used is CM_SS_GAN_STATE_MASK.
    sys_gw_gan_state_e_type
  */
  int32                    gan_conn_status;
  /**<
    Indicate GAN connection status.
    Mask used is CM_SS_GAN_CONN_STATUS_MASK.
    sys_gw_gan_conn_status_e_type
  */
  int32                    gan_sig_chan_status;
  /**<
    Indicate GAN signal channel status.
    Mask used is CM_SS_GAN_SIG_CHAN_STATUS_MASK.
    sys_gw_gan_sig_chan_status_e_type
  */
  PACKED struct PACKED_POST
  {
    boolean               is_mode_display_on;

    uint8                 gan_usz_icon_indicator;

    uint8                 gan_ms_location_status;

    uint8                 usz_str[34];

  } gan_mode_display;
  /**<
    Indicate GAN mode display - whether GERAN or WLAN.
    Mask used is CM_SS_GAN_MODE_DISPLAY_MASK.
    sys_gw_gan_mode_display_s_type
  */
  PACKED struct PACKED_POST
  {
    uint8                 gan_error_category;

    uint8                 gan_error_codepoint;

  } gan_error;
  /**<
    Indicate GAN errors.
    Mask used is CM_SS_GAN_ERROR_PROMPT_MASK.
    sys_gw_gan_error_s_type
  */
} gan_info;


int32                     target_rat ;

int32                     irat_ctxt_target_rat ;

int32                     gwl_irat_asubs_id ;

int32                     voice_domain ;

int32                     sms_domain ;

int32                     div_duplex ;

int32                     def_roam_ind ;

uint8                     ps_data_suspend ;

uint16                    sys_sel_pref_req_id ;


LOG_RECORD_END
/**
  Data structure used to log CM SS events.
  We log current SS event along with what's in cm_ss_info_s_type.
*/
LOG_RECORD_DEFINE(LOG_CM_SS_EVENT_C)

uint8                      version;
/**< Version of the packet (CM_LOG_SS_EVENT_LOG_PACKET_VERSION)
*/

int32                      ss_event;
/**< SS event, as defined by cm_ss_event_e_type
*/

uint64                     changed_fields;
/**<
  Bit mask indicating which of the following serving system fields
  were changed since last notification. Refer to the #defines
  (CM_SS_*_MASK) above.
*/

int32                      srv_status;
/**<
  The service status (SERVICE/NO SERVICE) of the system.
  Mask used is CM_SS_SRV_STATUS_MASK.
*/

int32                      srv_domain;
/**<
  The service domain registered on the system.
  Mask used is CM_SS_SRV_DOMAIN_MASK.
*/

int32                      srv_capability;
/**<
  The system's service capability.
  Mask used is CM_SS_SRV_CAPABILITY_MASK.
*/

int32                      sys_mode;
/**<
  The system's mode.
  Mask used is CM_SS_SYS_MODE_MASK.
*/

int32                      roam_status;
/**<
  Indicate current roaming status.
  Mask used is CM_SS_ROAM_STATUS_MASK.
*/

PACKED struct PACKED_POST
{
  int32                    id_type;

  PACKED union PACKED_POST
  {
    PACKED struct PACKED_POST
    {
      uint16               sid;
      /**< The system ID
      */

      uint16               nid;
      /**< The network ID
      */

    } is95;
    /**< IS 95 sys id
    */

    byte                   is856[16];
    /**< HDR sys id
    */

    PACKED struct PACKED_POST
    {
      byte                 identity[3];

    } plmn;
    /**< UMTS (WCDMA/GSM) system ID
    */

    PACKED struct PACKED_POST
    {
      uint8                len;
      /**< Length of the SSID, if == 0, then SSID = broadcast SSID
      */
      char                 ssid[32];
      /**< SSID of the wlan system
      */

      /**<
        ssid.
        Hardcoded to the value of SYS_WLAN_SSID_MAX_SIZE
        at the time log packet was declared.
        This is intentional as otherwise changes to the constant would
        change byte ordering in the log packet.
      */

    } ssid;
    /**< WLAN system Id
    */

    PACKED struct PACKED_POST
    {
      PACKED struct PACKED_POST
      {
        byte               identity[3];
      } plmn;

      word                 lac;
      /**< LAC
      */
    } plmn_lac;
    /**< UMTS (WCDMA/GSM) PLMN and LAC ID
    */
  } id;

} sys_id;
/**< Type for system ID
*/

uint8                    is_sys_forbidden;
/**<
  Indicates whether the System is forbidden.
  Mask used is CM_SS_SYS_FORBIDDEN_MASK.
*/

uint8                    is_colloc;
/**<
  Indicates whether the system is collocated.
  Mask used is CM_SS_SYS_ID_MASK.
*/

uint16                     rssi;
/**<
  RSSI in positive dBm i.e. 75 means -75dBm. The range for
  - AMPS:      -110 to -89 dBm
  - 800 CDMA:  -105 to -90 dBm
  - 1900 CDMA: -108 to -93 dBm
  - GSM/WCDMA: -105 to -60 dBm
*/

int16                      ecio;
int                        io;
byte                       sinr;
/**<
  ECIO in negative 0.5 dBm i.e. 63 means -31.5dBm.
  Io field is valid only when in HDR mode only operation
  SINR range: from 0 to 8, Level 8 represents highest SINR
  SINR applicable only when sys_mode = HDR.
*/

uint8                    ps_data_suspend;
/**<
  Indicates that the current PS data service has been suspended.
  Mask used is CM_SS_PS_DATA_SUSPEND_MASK.
*/

uint8                    hdr_hybrid;
/**< System is hybrid or not. FEATURE_HDR_HYBRID
*/

int32                      hdr_srv_status;
/**<
  Indicate current service state - FEATURE_HDR_HYBRID.
  Mask used is CM_SS_HDR_SRV_STATUS_MASK.
  sys_srv_status_e_type
*/

int32                      hdr_roam_status;
/**<
  Indicate current hybrid hdr roaming status - FEATURE_HDR_HYBRID.
  Mask used is CM_SS_HDR_ROAM_STATUS_MASK.
  sys_roam_status_e_type
*/

uint16                     hdr_rssi;
/**<
  HDR RSSI in positive dBm i.e. 75 means -75dBm - FEATURE_HDR_HYBRID.
  The range is -105 to -90 dBm
*/

int16                      hdr_ecio;
int                        hdr_io;
byte                       hdr_sinr;
/**<
  HDR Ec/Io and Io values (used when mobile is in
  hybrid operation).
  Ec/Io range: from 5 ( -31.5 dB ) to 512 ( 0 dB )
  Io    range: from -106 dBm to -21 dBm
  SINR range: from 0 to 8, Level 8 represents highest SINR
  (FEATURE_HDR)
*/

int32                      prev_idle_digital_mode;
/**<
  Previous idle digital mode. FEATURE_HDR_HANDOFF.
  Mask used is CM_SS_IDLE_DIGITAL_MODE_MASK.
  sys_sys_mode_e_type
*/

int32                      cur_idle_digital_mode;
/**<
  Current idle digital mode. FEATURE_HDR_HANDOFF.
  Mask used is CM_SS_IDLE_DIGITAL_MODE_MASK.
  sys_sys_mode_e_type
*/

PACKED union PACKED_POST
{

  PACKED struct PACKED_POST
  {
    uint8                  bit_err_rate;
    /**< Bit Error Rate / RX QUAL in GSM
    */

    int32                  sim_state;
    /**<
      Current state of the SIM.
      Mask used is CM_SS_SIM_STATE_MASK.
      sys_sim_state_e_type
    */

    uint8                plmn_forbidden;
    /**<
      Indicates whether the PLMN is forbidden.
      Mask used is CM_SS_PLMN_FORBIDDEN_MASK.
    */

    PACKED struct PACKED_POST
    {
      /*
        Flags that indicate whether the particular mm information is
        available.
      */
      uint8              plmn_avail;
      uint8              full_name_avail;
      uint8              short_name_avail;
      uint8              univ_time_and_time_zone_avail;
      uint8              time_zone_avail;
      uint8              lsa_identity_avail;
      uint8              daylight_saving_adj_avail;

      PACKED struct PACKED_POST
      {
        byte               identity[3];
      } plmn;

      PACKED struct PACKED_POST
      {
        uint8              length;
        /**< Number of characters in the full name
        */

        int32              coding_scheme;
        /**<
          The coding scheme used to encode the full name of the network.
          sys_network_name_coding_scheme_type_e_type
        */

        uint8            add_country_initials;
        /**<
          Flag that indicates whether MS should add the country initials
          to the full name of the network
        */

        uint8              name[255];
        /**< The full name of the network
        */

        /**<
          name.
          Hardcoded to the value of SYS_MAX_FULL_NAME_CHARS
          at the time log packet was declared.
          This is intentional as otherwise changes to the constant would
          change byte ordering in the log packet.
        */

      } full_name;

      PACKED struct PACKED_POST
      {

        uint8              length;
        /**< Number of characters in the full name
        */

        int32              coding_scheme;
        /**<
          The coding scheme used to encode the full name of the network.
          sys_network_name_coding_scheme_type_e_type
        */

        uint8            add_country_initials;
        /**<
          Flag that indicates whether MS should add the country initials
          to the full name of the network
        */

        uint8              name[255];
        /**< The full name of the network
        */

        /**<
          name.
          Hardcoded to the value of SYS_MAX_SHORT_NAME_CHARS
          at the time log packet was declared.
          This is intentional as otherwise changes to the constant would
          change byte ordering in the log packet.
        */

      } short_name;

      PACKED struct PACKED_POST
      {

        uint8             year;
        uint8             month;
        uint8             day;
        uint8             hour;
        uint8             minute;
        uint8             second;
        int               time_zone;
        /**< sys_time_zone_type
        */

      } univ_time_and_time_zone;

      int                  time_zone;
      /**< sys_time_zone_type
      */
      PACKED struct PACKED_POST
      {
        int32              type;
        /**< sys_lsa_identity_type_e_type
        */

        uint8              identity[3];

      } lsa_identity;

      int32                daylight_saving_adj;
      /**< sys_daylight_saving_adj_e_type
      */

    } mm_information;

    /**<
      Mobility management information, i.e. network name, date,
      time and time zone of registered network
    */

  } gw_info;

  /**< GSM/WCDMA SS information
  */

  PACKED struct PACKED_POST
  {


    int32                  band_class;
    /**<
      cellular or PCS.
      sys_band_class_e_type
    */

    int32                  block_or_system;
    /**<
      PCS frequency block: A to F Cellular System A or B.
      sys_blksys_e_type
    */

    uint16                 reg_zone;
    /**< 12 bit registration zone
    */

    uint16                 packet_zone;
    /**<
      8 bit packet zone, 0xffff means PACKET ZONE NONE.
      Mask used is CM_SS_PACKET_ZONE_MASK.
    */

    uint8                  bs_p_rev;
    /**<
      8 bit base station p_rev.
      Mask used is CM_SS_BS_P_REV_MASK.
    */

    uint8                  p_rev_in_use;
    /**<
      8 bit p_rev in use the lessor of BS p_rev and mobile p_rev.
      Mask used is CM_SS_P_REV_IN_USE_MASK.
    */

    uint8                  is_registered;
    /**< Is mobile registered on the current system
    */

    uint8                  ccs_supported;
    /**< Is concurrent services supported
    */

    int32                  uz_id;
    /**<
      User zone Identification of the selected user zone,
      == CM_INVALID_UZID, if no user zone selected. FEATURE_USERZONE.
      Mask used is CM_SS_UZ_CHANGED_MASK.
    */

    PACKED struct PACKED_POST
    {
      byte                 uz_name_encoding;

      byte                 uz_name_n_octets;

      byte                 uz_name[32];


      /**<
        uz_name.
        Hardcoded to the value of SYS_PUZL_MAX_UZ_NAME_LEN
        at the time log packet was declared.
        This is intentional as otherwise changes to the constant would
        change byte ordering in the log packet.
      */

    } uz_name;
    /**<
      User zone name of the selected user zone.
      Valid only if uz_id != CM_INVALID_UZID. FEATURE_USERZONE.
      Mask used is CM_SS_UZ_CHANGED_MASK.
    */

    byte                   srch_win_n;
    /**<
      Neighbour set window search size.
      Mask used is CM_SS_BASE_STATION_PARMS_CHGD_MASK.
    */

    int32                  base_lat;
    /**<
      Base station latitude[0.25sec and already sign extended].
      Mask used is CM_SS_BASE_STATION_PARMS_CHGD_MASK.
      sys_base_lat_type
    */

    int32                  base_long;
    /**<
      Base station latitude[0.25sec and already sign extended].
      Mask used is CM_SS_BASE_STATION_PARMS_CHGD_MASK.
      sys_base_long_type
    */

    word                   base_id;
    /**<
      Base station Identification number.
      Mask used is CM_SS_BASE_STATION_PARMS_CHGD_MASK.
      sys_base_id_type
    */

    PACKED struct
    {
      int32                  sys_mode;
      /**< Indicates the system from which the time was obtained (sys_sys_mode_e_type)
      */
      PACKED union
      {
        PACKED struct
        {
          uint8                lp_sec;
          int8                 daylt_savings;
          int16                ltm_offset;
        } cdma_time;

        PACKED struct
        {
          uint8                lp_sec;
          int16                ltm_offset;
        } hdr_time;

      } time;

    } time_info;
    /**<
      Time obtained from sync channel / sector parameter msg
      Mask used is CM_SS_CDMA_TIME_CHGD_MASK.
    */

  } cdma_info;
  /**< CDMA SS information
  */
} mode_info;
/**< Mode specific serving system information
*/


/*
  FEATURE_BCMCS.
  These fields specify whether lower layer
  supports broadcast/multicast as per
  High Rate Broadcast-Multicast Packet data Air
  Interface Specification C.S0054 and
  BCMCS Framework Draft Document 0.1.3 XP0019
*/

uint8                    hybr_hdr_bcmcs_srv_supported;
/**<
  Specify whether HDR is configured to support BCMCS service.
  Mask used is CM_SS_HYBR_HDR_BCMCS_SRV_SUPPORTED_MASK.
*/

uint8                    ss_main_bcmcs_srv_supported;
/**<
  Specify whether MAIN is configured to support BCMCS service.
  Mask used is CM_SS_MAIN_BCMCS_SRV_SUPPORTED_MASK.
*/

int                        requesting_client_id;
/**< Requesting client
*/

int32                      msm_id;
/**<
  Identifies the MSM which generated this event.
  sys_msm_id_e_type
*/

/*
  Clients check CM_API_WLAN for WLAN support for following
  wlan_rssi, wlan_srv_status, wlan_bss_info, wlan_stats
*/
uint16                     wlan_rssi;

int32                      wlan_srv_status;
/**<
  Indicate current WLAN service state.
  Mask used is CM_SS_WLAN_SRV_STATUS_MASK.
  sys_srv_status_e_type
*/

PACKED struct PACKED_POST
{

  word                     chan;
  /**<
    Channel on which this BSS provides service.
    sys_chan_type
  */

  int32                    band;
  /**<
    Band in which this BSS provides service.
    sys_band_class_e_type
  */

  int32                    bss_type;
  /**<
    Type of BSS - Infrastructure or independent.
    sys_wlan_bss_e_type
  */

  uint64                   bssid;
  /**<
    BSS Id.
    sys_wlan_bssid_type
  */

  PACKED struct PACKED_POST
  {
    uint8                  len;
    /**< Length of the SSID, if == 0, then SSID = broadcast SSID
    */

    char                   ssid[32];
    /**< SSID of the wlan system
    */

    /**<
      ssid.
      Hardcoded to the value of SYS_WLAN_SSID_MAX_SIZE
      at the time log packet was declared.
      This is intentional as otherwise changes to the constant would
      change byte ordering in the log packet.
    */
  } ssid;
  /**< SS Id
  */

  uint16                   rssi;
  /**< Measured RSSI of the BSS
  */

  int32                    tech;
  /**<
    WLAN technology in use.
    sys_tech_e_type
  */
} wlan_bss_info;
/**< 802.11 (WLAN) System Information
*/

PACKED struct PACKED_POST
{
  int32                    current_xmit_rate;
  /**<
    Xmit rate of the last packet successfully transmitted.
    sys_wlan_rate_e_type
  */

  uint32                   total_tx_bytes;
  /**< Number of bytes transmitted over the WLAN interface
  */

  uint32                   total_rx_bytes;
  /**< Number of bytes received over the WLAN interface
  */

} wlan_stats;
/**< WLAN stats such as Current Transmit Rate, TX/RX bytes
*/

int32   cipher_domain;
/**<
  Can be used only if CM_CIPHERING_IND_SUPP is defined.
  Ciphering on CS or PS domain or both.
  sys_srv_domain_e_type
*/

PACKED struct PACKED_POST
{

  uint8                  ss_thr_status;
  /**< The throttle status of the current ss
  */

  dword                    thr_uptime;
  /**< The throttling time period
  */

  unsigned int             idx;
  /**<
    The index in the throttle_time table.
    Used internally by CM to keep track of the next throttle time
    increment
  */

  int32                    call_status;
  /**<
    The call end reason.
    cm_ss_orig_thr_reasons_e_type
  */

} main_thr_status;

/**<
  The DCTM status, indicates the current throttle status of the MAIN
  system
*/

PACKED struct PACKED_POST
{
  uint8                  ss_thr_status;
  /**< The throttle status of the current ss
  */

  dword                    thr_uptime;
  /**< The throttling time period
  */

  unsigned int             idx;
  /**<
    The index in the throttle_time table.
    Used internally by CM to keep track of the next throttle time
    increment
  */

  int32                    call_status;
  /**<
    The call end reason.
    cm_ss_orig_thr_reasons_e_type
  */

}  hdr_thr_status;
/**<
  The DCTM status, indicates the current throttle status of the HDR
  system
*/

PACKED struct PACKED_POST
{
  int32                    id_type;

  PACKED union PACKED_POST
  {
    PACKED struct PACKED_POST
    {
      uint16               sid;
      /**< The system ID
      */

      uint16               nid;
      /**< The network ID
      */

    } is95;
    /**< IS 95 sys id
    */

    byte                   is856[16];
    /**< HDR sys id
    */

    PACKED struct PACKED_POST
    {
      byte                 identity[3];

    } plmn;
    /**< UMTS (WCDMA/GSM) system ID
    */

    PACKED struct PACKED_POST
    {
      uint8                len;
      /**< Length of the SSID, if == 0, then SSID = broadcast SSID
      */
      char                 ssid[32];
      /**< SSID of the wlan system
      */

      /**<
        ssid.
        Hardcoded to the value of SYS_WLAN_SSID_MAX_SIZE
        at the time log packet was declared.
        This is intentional as otherwise changes to the constant would
        change byte ordering in the log packet.
      */

    } ssid;
    /**< WLAN system Id
    */

    PACKED struct PACKED_POST
    {
      PACKED struct PACKED_POST
      {
        byte               identity[3];
      } plmn;

      word                 lac;
      /**< LAC
      */
    } plmn_lac;
    /**< UMTS (WCDMA/GSM) PLMN and LAC ID
    */
  } id;
} hdr_sys_id;
/**<
  The ID of the HDR sub system.
  Mask used is CM_SS_HDR_SYS_ID_MASK.
*/

dword                     curr_uptime;
/**< Current uptime
*/

int32                     psist;
/**<
  Indicate psist of the system.
  Mask used is CM_SS_PSIST_CHGD_MASK.
  cm_psist_e_type
*/

int32                     hdr_active_prot;
/**<
  Indicate HDR active protocol revision value.
  Mask used is CM_SS_HDR_ACTIVE_PROT_MASK.
  sys_active_prot_e_type
*/

/* Below fields added for version = 5 and upwards */

int32                     true_srv_status;
/**<
  The true service status (LIMITED/LIMITED_REGIONAL) of the system.
  Mask used is CM_SS_TRUE_SRV_STATUS_MASK.
*/

uint64                    signal_strength_changed_fields;
/**<
  Bit mask indicating which of the following signal strength fields
  were changed since last notification. Refer to the #defines
  (CM_SS_*_MASK) above.
*/

int16                    pathloss;
/**< Pathloss is applicable only in Connected mode (otherwise 0xFF)
*/

int16                    sir;
/**<
  SIR applicable and valid (otherwise 0xFF) only in WCDMA
  connected mode when DPCCH is active.
*/

int16                     rssi2;
/**< The -ve RSSI value, coming from RRC layer
*/

int16                     rscp;
/**< The range of RSCP would be from -28 to -121 dBm
*/

boolean                   ecio_for_display;
/**< TRUE if UE is in Data call
*/

int16                     gw_ecio;
/**<
  ECIO for WCDMA only.
  Ec/Io is measured in -0.5 dB.
  What WCDMA L1 sends to RRC is in 0.5dB (positive value).
  For GSM it is defaulted to CMSS_ECIO_VALUE_INVALID_SIGNAL.
  Clients should check for CM_API_GW_ECIO to use this field
*/

PACKED struct PACKED_POST
{
  uint32                 cell_id;
  /**< Cell_id
  */

} cell_info;

PACKED struct PACKED_POST
{
  int32                   reject_srv_domain;
  /**< Srv domain on which registration is rejected
  */

  uint8                   reject_cause;
  /**< Registration Reject Cause
  */

} reg_reject_info;

PACKED struct PACKED_POST
{
  int32                    gan_state;
  /**<
    Indicate GAN state.
    Mask used is CM_SS_GAN_STATE_MASK.
    sys_gw_gan_state_e_type
  */
  int32                    gan_conn_status;
  /**<
    Indicate GAN connection status.
    Mask used is CM_SS_GAN_CONN_STATUS_MASK.
    sys_gw_gan_conn_status_e_type
  */
  int32                    gan_sig_chan_status;
  /**<
    Indicate GAN signal channel status.
    Mask used is CM_SS_GAN_SIG_CHAN_STATUS_MASK.
    sys_gw_gan_sig_chan_status_e_type
  */
  PACKED struct PACKED_POST
  {
    boolean               is_mode_display_on;

    uint8                 gan_usz_icon_indicator;

    uint8                 gan_ms_location_status;

    uint8                 usz_str[34];

  } gan_mode_display;
  /**<
    Indicate GAN mode display - whether GERAN or WLAN.
    Mask used is CM_SS_GAN_MODE_DISPLAY_MASK.
    sys_gw_gan_mode_display_s_type
  */
  PACKED struct PACKED_POST
  {
    uint8                 gan_error_category;

    uint8                 gan_error_codepoint;

  } gan_error;
  /**<
    Indicate GAN errors.
    Mask used is CM_SS_GAN_ERROR_PROMPT_MASK.
    sys_gw_gan_error_s_type
  */
} gan_info;

/* Below fields added for version = 6 and upwards */

PACKED struct PACKED_POST
{
  int32                       hs_ind;
  /**< High speed service indication (Over WCDMA)
  */

  int32                       dtm_supp;
  /**< Dual Transfer mode indication (Over GSM)
  */

  int32                       egprs_supp;
  /**< EGPRS indication
  */

  int32                       hs_call_status;
  /**<
    Clients need to check for CM_API_CELL_SUPP_SERVICES
    and CM_API_CELL_SUPP_SERVICES_HS_CALL before
    using this structure
  */

} cell_srv_ind;



int32                      hybr_hdr_bcmcs_srv_status;
/**<
  Specify whether BCMCS service is available on HDR.
  Mask used is CM_SS_HYBR_HDR_BCMCS_SRV_STATUS_MASK.
*/

int32                     ss_main_bcmcs_srv_status;
/**<
 Specify whether BCMCS service is available on MAIN.
  Mask used is CM_SS_MAIN_BCMCS_SRV_STATUS_MASK.
*/


PACKED struct PACKED_POST
{
  int32                     reg_status;
  /**< IP app registration status
  */

  int                       cause;
  /**< Cause given when app registration fails or deregisters
  */

  PACKED struct PACKED_POST
  {
    boolean                  present;
    /**<
      TRUE   - Warning is present
      FALSE  - Warning is not present
    */

    int                       cause;
    /**< Warning code - SIP RFC 3261, sect 20.43
    */

    char                      hostname[20];
    /**<
      Null terminated host name that
      generates the warning
    */

    char                      description[20];
    /**< Null terminated Warning text
    */

  } warn_info;
  /**< Warning information given by ipapp
  */

  uint64                      call_type_mask[CMIPAPP_CAP_ARR_MAX];
  /**< Array of call types that ipapp can service
  */

  int32                       sys_mode[CMIPAPP_CAP_ARR_MAX];
  /**<
    System modes app can support for corresponding call_type.
    Can be used only if CM_API_SMS_OVER_IP is defined.
  */

} ipapp_info;


int32                       active_band;
/**<
  Clients need to check for CM_API_EMERGENCY_NUM_LIST before
  using this structure.
*/

uint16                      active_channel;
/**< Report active channel of the current serving system
*/

int32                       hybrid_active_band;
/**< Report active band of hybrid system
*/

uint16                      hybrid_active_channel;
/**< Report active channel of the current serving system
*/

int32                       hdr_ln_status;
/**< HDR LN status
*/

PACKED struct PACKED_POST
{
  boolean                    present;
  /**<
    Present flag if this structure
    is populated
  */

  uint16                     sid;
  /**<
    Sid that is in conflict with
    countries
  */

  uint8                      country_code_len;
  /**< Length of country_code_list[]
  */

  uint16                     country_code_list [5];
  /**<
    Array of mobile country codes that conflict
    with given sid
  */

} country_list;


uint16                      current_mcc;
/**<
  Clients need to check for CM_API_PLUS_DIALING before
  using this structure. Associated with CM_SS_SRV_STATUS_MASK.
  Carries country code of current serving country.
*/


uint16                      home_mcc;
/**<
  Clients need to check for CM_API_PLUS_DIALING before
  using this structure. Associated with CM_SS_SRV_STATUS_MASK.
  Carries home country code.
*/


int32                       plus_dial_setting;
/**<
  Clients need to check for CM_API_PLUS_DIALING before
  using this structure. Associated with CM_SS_SRV_STATUS_MASK.
  Carries NV setting corresp to plus dial
*/

int32                       def_roam_ind;
/**< Default roaming indicator from PRL
*/


/* Below fields added for version = 7 and upwards */

byte                        asubs_id;
/**<
  Subscription ID corresponding to the Main GW service.
  Mask used is CM_SS_SUBS_ID_MASK.
  This is only valid when the sys_mode is GSM/WCDMA.
*/

boolean                    is_main_operational;
/**<
  Operating status for HYBR_GW subscription. Decided by selected dual
  standby preference
*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
**                     HYBRID GW SS INFO
**- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

uint64                     hybr_gw_changed_fields;
/**<
  Bit mask indicating which of the following serving system fields
  were changed since last notification. Refer to the #defines
  (CM_SS_*_MASK) above.
*/

boolean                    is_hybr_gw_operational;
/**<
  Operating status for HYBR_GW subscription. Decided by selected dual
  standby preference
*/

byte                       gw_asubs_id;
/**<
  Subscription ID corresponding to the Hybrid GW service.
  Mask used is CM_SS_GW_SUBS_ID_MASK.
  This is only valid when the sys_mode is GSM/WCDMA.
*/

int32                     gw_active_band;
/**<
  Clients need to check for CM_API_EMERGENCY_NUM_LIST before
  using this structure.
*/

int16                     gw_active_channel;
/**< Report active channel of the current serving system
*/

int32                     gw_srv_status;
/**<
  The service status (SERVICE/NO SERVICE) of the system.
  Mask used is CM_SS_GW_SRV_STATUS_MASK.
*/

int32                     true_gw_srv_status;
/**<
  The true service status (LIMITED/LIMITED_REGIONAL) of the system.
  Mask used is CM_SS_TRUE_GW_SRV_STATUS_MASK.
*/

int32                     gw_srv_domain;
/**<
  The service domain registered on the system.
  Mask used is CM_SS_SRV_DOMAIN_MASK.
*/

int32                     gw_srv_capability;
/**<
  The system's service capability.
  Mask used is CM_SS_SRV_CAPABILITY_MASK.
*/

int32                     gw_sys_mode;
/**<
  The system's mode.
  Mask used is CM_SS_SYS_MODE_MASK.
*/

int32                     gw_roam_status;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
**                                       HYBRID3 GW SS INFO
**- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

uint64                                       hybr_gw3_changed_fields;
/**<
  Bit mask indicating which of the following serving system fields
  were changed since last notification. Refer to the #defines
  (CM_SS_*_MASK) above.
*/

boolean                                      is_hybr_gw3_operational;
/**<
  Operating status for HYBR_GW subscription. Decided by selected dual
  standby preference
*/

byte                                             gw3_asubs_id;
/**<
  Subscription ID corresponding to the Hybrid GW service.
  Mask used is CM_SS_GW_SUBS_ID_MASK.
  This is only valid when the sys_mode is GSM/WCDMA.
*/

int32                                       gw3_active_band;
/**<
  Clients need to check for CM_API_EMERGENCY_NUM_LIST before
  using this structure.
*/

int16                                       gw3_active_channel;
/**< Report active channel of the current serving system.
*/

int32                                       gw3_srv_status;
/**<
  The service status (SERVICE/NO SERVICE) of the system.
  Mask used is CM_SS_GW_SRV_STATUS_MASK.
*/

int32                                       true_gw3_srv_status;
/**<
  The true service status (LIMITED/LIMITED_REGIONAL) of the system.
  Mask used is CM_SS_TRUE_GW_SRV_STATUS_MASK.
*/

int32                                       gw3_srv_domain;
/**<
  The service domain registered on the system.
  Mask used is CM_SS_SRV_DOMAIN_MASK.
*/

int32                                       gw3_srv_capability;
/**<
  The system's service capability.
  Mask used is CM_SS_SRV_CAPABILITY_MASK.
*/

int32                                       gw3_sys_mode;
/**<
  The system's mode.
  Mask used is CM_SS_SYS_MODE_MASK.
*/

int32                                       gw3_roam_status;
/**<
  Indicate current roaming status.
  Mask used is CM_SS_ROAM_STATUS_MASK.
*/

PACKED struct PACKED_POST
{
  int32                    id_type;

  PACKED union PACKED_POST
  {
    PACKED struct PACKED_POST
    {
      uint16               sid;
      /**< The system ID
      */

      uint16               nid;
      /**< The network ID
      */

    } is95;
    /**< IS 95 sys id
    */

    byte                   is856[16];
    /**< HDR sys id
    */

    PACKED struct PACKED_POST
    {
      byte                 identity[3];

    } plmn;
    /**< UMTS (WCDMA/GSM) system ID
    */

    PACKED struct PACKED_POST
    {
      uint8                len;
      /**< Length of the SSID, if == 0, then SSID = broadcast SSID
      */
      char                 ssid[32];
      /**< SSID of the wlan system
      */

      /**<
        ssid.
        Hardcoded to the value of SYS_WLAN_SSID_MAX_SIZE
        at the time log packet was declared.
        This is intentional as otherwise changes to the constant would
        change byte ordering in the log packet.
      */

    } ssid;
    /**< WLAN system Id
    */

    PACKED struct PACKED_POST
    {
      PACKED struct PACKED_POST
      {
        byte               identity[3];
      } plmn;

      word                 lac;
      /**< LAC
      */
    } plmn_lac;
    /**< UMTS (WCDMA/GSM) PLMN and LAC ID
    */
  } id;

} gw3_sys_id;
/**<
  The ID of the selected system.
  Mask used is CM_SS_SYS_ID_MASK.
*/

PACKED struct PACKED_POST
{
  int32                    id_type;

  PACKED union PACKED_POST
  {
    PACKED struct PACKED_POST
    {
      uint16               sid;
      /**< The system ID
      */

      uint16               nid;
      /**< The network ID
      */

    } is95;
    /**< IS 95 sys id
    */

    byte                   is856[16];
    /**< HDR sys id
    */

    PACKED struct PACKED_POST
    {
      byte                 identity[3];

    } plmn;
    /**< UMTS (WCDMA/GSM) system ID
    */

    PACKED struct PACKED_POST
    {
      uint8                len;
      /**< Length of the SSID, if == 0, then SSID = broadcast SSID
      */
      char                 ssid[32];
      /**< SSID of the wlan system
      */

      /**<
        ssid.
        Hardcoded to the value of SYS_WLAN_SSID_MAX_SIZE
        at the time log packet was declared.
        This is intentional as otherwise changes to the constant would
        change byte ordering in the log packet.
      */

    } ssid;
    /**< WLAN system Id
    */

    PACKED struct PACKED_POST
    {
      PACKED struct PACKED_POST
      {
        byte               identity[3];
      } plmn;

      word                 lac;
      /**< LAC
      */
    } plmn_lac;
    /**< UMTS (WCDMA/GSM) PLMN and LAC ID
    */
  } id;

} gw_sys_id;
/**<
  The ID of the selected system.
  Mask used is CM_SS_SYS_ID_MASK.
*/

boolean                    gw_is_sys_prl_match;
/**
  Indicates whether the System is in PRL.
  Mask used is CM_SS_SYS_PRL_MATCH_MASK.
*/

boolean                    gw_is_sys_forbidden;
/**<
  Indicates whether the System is forbidden.
  Mask used is CM_SS_SYS_FORBIDDEN_MASK.
*/

uint16                     gw_rssi;
/**<
  RSSI in positive dBm i.e. 75 means -75dBm. The range for
  - AMPS:      -110 to -89 dBm
  - 800 CDMA:  -105 to -90 dBm
  - 1900 CDMA: -108 to -93 dBm
  - GSM/WCDMA: -105 to -60 dBm
*/

int                        gw_io;
byte                       gw_sinr;
/**<
  ECIO in negative 0.5 dBm i.e. 63 means -31.5dBm.
  Io field is valid only when in HDR mode only operation.
  SINR range: from 0 to 8, Level 8 represents highest SINR.
  SINR applicable only when sys_mode = HDR.
*/

int16                      gw_pathloss;
/**< Pathloss is applicable only in Connected mode (otherwise 0xFF)
*/

int16                      gw_sir;
/**<
  SIR applicable and valid (otherwise 0xFF) only in WCDMA
  connected mode when DPCCH is active
*/

int16                      gw_rssi2;
/**< The -ve RSSI value, coming from RRC layer
*/

int16                      gw_rscp;
/**< Signal strength in RSSI, change for WCDMA only (otherwise 0xFF)
*/

boolean                    gw_ecio_for_display;
/**< TRUE if UE is in a data call, else FALSE
*/

boolean                                      gw3_is_sys_prl_match;
/**<
  Indicates whether the System is in PRL.
  Mask used is CM_SS_SYS_PRL_MATCH_MASK.
*/

boolean                                    gw3_is_sys_forbidden;
/**<
  Indicates whether the System is forbidden.
  Mask used is CM_SS_SYS_FORBIDDEN_MASK.
*/

uint16                                         gw3_rssi;
/**<
  RSSI in positive dBm i.e. 75 means -75dBm. The range for
  - AMPS:           -110 to -89 dBm
  - 800 CDMA:   -105 to -90 dBm
  - 1900 CDMA: -108 to -93 dBm
  - GSM/WCDMA: -105 to -60 dBm
*/

int                                              gw3_io;
byte                                             gw3_sinr;
/**<
  ECIO in negative 0.5 dBm i.e. 63 means -31.5dBm.
  Io field is valid only when in HDR mode only operation.
  SINR range: from 0 to 8, Level 8 represents highest SINR.
  SINR applicable only when sys_mode = HDR.
*/

int16                                        gw3_pathloss;
/**< Pathloss is applicable only in Connected mode (otherwise 0xFF)
*/

int16                                        gw3_sir;
/**<
  SIR applicable and valid (otherwise 0xFF) only in WCDMA
  connected mode when DPCCH is active
*/

int16                                        gw3_rssi2;
/**< The -ve RSSI value, coming from RRC layer
*/

int16                                        gw3_rscp;
/**< Signal strength in RSSI, change for WCDMA only (otherwise 0xFF)
*/

boolean                                      gw3_ecio_for_display;
/**< TRUE if UE is in a data call, else FALSE
*/

PACKED struct PACKED_POST
{
  uint32                 cell_id;
  /**< Cell_id
  */

} gw3_cell_info;
/**<
  The CELL_INFO of the selected system.
  Mask used is CM_SS_CELL_INFO_MASK.
*/

PACKED struct PACKED_POST
{
  uint32                 cell_id;
  /**< Cell_id
  */

} gw_cell_info;
/**<
  The CELL_INFO of the selected system.
  Mask used is CM_SS_CELL_INFO_MASK.
*/

PACKED union PACKED_POST
{

  PACKED struct PACKED_POST
  {
    uint8                  bit_err_rate;
    /**< Bit Error Rate / RX QUAL in GSM
    */

    int32                  sim_state;
    /**<
      Current state of the SIM.
      Mask used is CM_SS_SIM_STATE_MASK.
      sys_sim_state_e_type
    */

    uint8                plmn_forbidden;
    /**<
      Indicates whether the PLMN is forbidden.
      Mask used is CM_SS_PLMN_FORBIDDEN_MASK.
    */

    PACKED struct PACKED_POST
    {
      /*
        Flags that indicate whether the particular mm information is
        available.
      */
      uint8              plmn_avail;
      uint8              full_name_avail;
      uint8              short_name_avail;
      uint8              univ_time_and_time_zone_avail;
      uint8              time_zone_avail;
      uint8              lsa_identity_avail;
      uint8              daylight_saving_adj_avail;

      PACKED struct PACKED_POST
      {
        byte               identity[3];
      } plmn;

      PACKED struct PACKED_POST
      {
        uint8              length;
        /**< Number of characters in the full name
        */

        int32              coding_scheme;
        /**<
          The coding scheme used to encode the full name of the network.
          sys_network_name_coding_scheme_type_e_type
        */

        uint8            add_country_initials;
        /**<
          Flag that indicates whether MS should add the country initials
          to the full name of the network
        */

        uint8              name[255];
        /**< The full name of the network
        */

        /**<
          name.
          Hardcoded to the value of SYS_MAX_FULL_NAME_CHARS
          at the time log packet was declared.
          This is intentional as otherwise changes to the constant would
          change byte ordering in the log packet.
        */

      } full_name;

      PACKED struct PACKED_POST
      {

        uint8              length;
        /**< Number of characters in the full name
        */

        int32              coding_scheme;
        /**<
          The coding scheme used to encode the full name of the network.
          sys_network_name_coding_scheme_type_e_type
        */

        uint8            add_country_initials;
        /**<
          Flag that indicates whether MS should add the country initials
          to the full name of the network
        */

        uint8              name[255];
        /**< The full name of the network
        */

        /**<
          name.
          Hardcoded to the value of SYS_MAX_SHORT_NAME_CHARS
          at the time log packet was declared.
          This is intentional as otherwise changes to the constant would
          change byte ordering in the log packet.
        */

      } short_name;

      PACKED struct PACKED_POST
      {

        uint8             year;
        uint8             month;
        uint8             day;
        uint8             hour;
        uint8             minute;
        uint8             second;
        int               time_zone;
        /**< sys_time_zone_type
        */

      } univ_time_and_time_zone;

      int                  time_zone;
      /**< sys_time_zone_type
      */
      PACKED struct PACKED_POST
      {
        int32              type;
        /**< sys_lsa_identity_type_e_type
        */

        uint8              identity[3];

      } lsa_identity;

      int32                daylight_saving_adj;
      /**< sys_daylight_saving_adj_e_type
      */

    } mm_information;

    /**<
      Mobility management information, i.e. network name, date,
      time and time zone of registered network
    */

  } gw_info;

  /**< GSM/WCDMA SS information
  */

  PACKED struct PACKED_POST
  {


    int32                  band_class;
    /**<
      cellular or PCS.
      sys_band_class_e_type
    */

    int32                  block_or_system;
    /**<
      PCS frequency block: A to F Cellular System A or B.
      sys_blksys_e_type
    */

    uint16                 reg_zone;
    /**< 12 bit registration zone
    */

    uint16                 packet_zone;
    /**<
      8 bit packet zone, 0xffff means PACKET ZONE NONE.
      Mask used is CM_SS_PACKET_ZONE_MASK.
    */

    uint8                  bs_p_rev;
    /**<
      8 bit base station p_rev.
      Mask used is CM_SS_BS_P_REV_MASK.
    */

    uint8                  p_rev_in_use;
    /**<
      8 bit p_rev in use the lessor of BS p_rev and mobile p_rev.
      Mask used is CM_SS_P_REV_IN_USE_MASK.
    */

    uint8                  is_registered;
    /**< Is mobile registered on the current system
    */

    uint8                  ccs_supported;
    /**< Is concurrent services supported
    */

    int32                  uz_id;
    /**<
      User zone Identification of the selected user zone,
      == CM_INVALID_UZID, if no user zone selected. FEATURE_USERZONE.
      Mask used is CM_SS_UZ_CHANGED_MASK.
    */

    PACKED struct PACKED_POST
    {
      byte                 uz_name_encoding;

      byte                 uz_name_n_octets;

      byte                 uz_name[32];


      /**<
        uz_name.
        Hardcoded to the value of SYS_PUZL_MAX_UZ_NAME_LEN
        at the time log packet was declared.
        This is intentional as otherwise changes to the constant would
        change byte ordering in the log packet.
      */

    } uz_name;
    /**<
      User zone name of the selected user zone.
      Valid only if uz_id != CM_INVALID_UZID. FEATURE_USERZONE.
      Mask used is CM_SS_UZ_CHANGED_MASK.
    */

    byte                   srch_win_n;
    /**<
      Neighbour set window search size.
      Mask used is CM_SS_BASE_STATION_PARMS_CHGD_MASK.
    */

    int32                  base_lat;
    /**<
      Base station latitude[0.25sec and already sign extended].
      Mask used is CM_SS_BASE_STATION_PARMS_CHGD_MASK.
      sys_base_lat_type
    */

    int32                  base_long;
    /**<
      Base station latitude[0.25sec and already sign extended].
      Mask used is CM_SS_BASE_STATION_PARMS_CHGD_MASK.
      sys_base_long_type
    */

    word                   base_id;
    /**<
      Base station Identification number.
      Mask used is CM_SS_BASE_STATION_PARMS_CHGD_MASK.
      sys_base_id_type
    */

    PACKED struct
    {
      int32                  sys_mode;
      /**< Indicates the system from which the time was obtained (sys_sys_mode_e_type)
      */
      PACKED union
      {
        PACKED struct
        {
          uint8                lp_sec;
          int8                 daylt_savings;
          int16                ltm_offset;
        } cdma_time;

        PACKED struct
        {
          uint8                lp_sec;
          int16                ltm_offset;
        } hdr_time;

      } time;

    } time_info;
    /**<
      Time obtained from sync channel / sector parameter msg.
      Mask used is CM_SS_CDMA_TIME_CHGD_MASK.
    */

  } cdma_info;
  /**< CDMA SS information
  */
} gw_mode_info;

PACKED union PACKED_POST
{

  PACKED struct PACKED_POST
  {
    uint8                                bit_err_rate;
    /**< Bit Error Rate / RX QUAL in GSM
    */

    int32                                sim_state;
    /**<
      Current state of the SIM.
      Mask used is CM_SS_SIM_STATE_MASK.
      sys_sim_state_e_type
    */

    uint8                            plmn_forbidden;
    /**<
      Indicates whether the PLMN is forbidden.
      Mask used is CM_SS_PLMN_FORBIDDEN_MASK.
    */

    PACKED struct PACKED_POST
    {
      /*
        Flags that indicate whether the particular mm information is
        available
      */
      uint8                        plmn_avail;
      uint8                        full_name_avail;
      uint8                        short_name_avail;
      uint8                        univ_time_and_time_zone_avail;
      uint8                        time_zone_avail;
      uint8                        lsa_identity_avail;
      uint8                        daylight_saving_adj_avail;

      PACKED struct PACKED_POST
      {
        byte                             identity[3];
      } plmn;

      PACKED struct PACKED_POST
      {
        uint8                        length;
        /**< Number of characters in the full name
        */

        int32                        coding_scheme;
        /**<
          The coding scheme used to encode the full name of the network.
          sys_network_name_coding_scheme_type_e_type
        */

        uint8                    add_country_initials;
        /**<
          Flag that indicates whether MS should add the country initials
          to the full name of the network
        */

        uint8                        name[255];
        /**< The full name of the network
        */

        /**<
          name.
          Hardcoded to the value of SYS_MAX_FULL_NAME_CHARS
          at the time log packet was declared.
          This is intentional as otherwise changes to the constant would
          change byte ordering in the log packet.
        */

      } full_name;

      PACKED struct PACKED_POST
      {

        uint8                        length;
        /**< Number of characters in the full name
        */

        int32                        coding_scheme;
        /**<
          The coding scheme used to encode the full name of the network.
          sys_network_name_coding_scheme_type_e_type
        */

        uint8                    add_country_initials;
        /**<
          Flag that indicates whether MS should add the country initials
          to the full name of the network
        */

        uint8                        name[255];
        /**< The full name of the network
        */

        /**<
          name.
          Hardcoded to the value of SYS_MAX_SHORT_NAME_CHARS
          at the time log packet was declared.
          This is intentional as otherwise changes to the constant would
          change byte ordering in the log packet.
        */

      } short_name;

      PACKED struct PACKED_POST
      {

        uint8                       year;
        uint8                       month;
        uint8                       day;
        uint8                       hour;
        uint8                       minute;
        uint8                       second;
        int                             time_zone;
        /**< sys_time_zone_type
        */

      } univ_time_and_time_zone;

      int                                  time_zone;
      /**< sys_time_zone_type
      */
      PACKED struct PACKED_POST
      {
        int32                        type;
        /**< sys_lsa_identity_type_e_type
        */

        uint8                        identity[3];

      } lsa_identity;

      int32                            daylight_saving_adj;
      /**< sys_daylight_saving_adj_e_type
      */

    } mm_information;

    /**<
      Mobility management information, i.e. network name, date,
      time and time zone of registered network
    */

  } gw_info;
  /**< GSM/WCDMA SS information
  */

  PACKED struct PACKED_POST
  {


    int32                                band_class;
    /**<
      cellular or PCS.
      sys_band_class_e_type
    */

    int32                                block_or_system;
    /**<
      PCS frequency block: A to F Cellular System A or B.
      sys_blksys_e_type
    */

    uint16                               reg_zone;
    /**< 12 bit registration zone
    */

    uint16                               packet_zone;
    /**<
      8 bit packet zone, 0xffff means PACKET ZONE NONE.
      Mask used is CM_SS_PACKET_ZONE_MASK.
    */

    uint8                                bs_p_rev;
    /**<
      8 bit base station p_rev.
      Mask used is CM_SS_BS_P_REV_MASK.
    */

    uint8                                p_rev_in_use;
    /**<
      8 bit p_rev in use the lessor of BS p_rev and mobile p_rev.
      Mask used is CM_SS_P_REV_IN_USE_MASK.
    */

    uint8                                is_registered;
    /**< Is mobile registered on the current system
    */

    uint8                                ccs_supported;
    /**< Is concurrent services supported
    */

    int32                                uz_id;
    /**<
      User zone Identification of the selected user zone,
      == CM_INVALID_UZID, if no user zone selected. FEATURE_USERZONE.
      Mask used is CM_SS_UZ_CHANGED_MASK.
    */

    PACKED struct PACKED_POST
    {
      byte                                 uz_name_encoding;

      byte                                 uz_name_n_octets;

      byte                                 uz_name[32];


      /**<
        uz_name.
        Hardcoded to the value of SYS_PUZL_MAX_UZ_NAME_LEN
        at the time log packet was declared.
        This is intentional as otherwise changes to the constant would
        change byte ordering in the log packet.
      */

    } uz_name;
    /**<
      User zone name of the selected user zone.
      Valid only if uz_id != CM_INVALID_UZID. FEATURE_USERZONE.
      Mask used is CM_SS_UZ_CHANGED_MASK.
    */

    byte                                     srch_win_n;
    /**<
      Neighbour set window search size.
      Mask used is CM_SS_BASE_STATION_PARMS_CHGD_MASK.
    */

    int32                                base_lat;
    /**<
      Base station latitude[0.25sec and already sign extended].
      Mask used is CM_SS_BASE_STATION_PARMS_CHGD_MASK.
      sys_base_lat_type
    */

    int32                                base_long;
    /**<
      Base station latitude[0.25sec and already sign extended].
      Mask used is CM_SS_BASE_STATION_PARMS_CHGD_MASK.
      sys_base_long_type
    */

    word                                     base_id;
    /**<
      Base station Identification number.
      Mask used is CM_SS_BASE_STATION_PARMS_CHGD_MASK.
      sys_base_id_type
    */

    PACKED struct
    {
      int32                                sys_mode;
      /**< Indicates the system from which the time was obtained (sys_sys_mode_e_type)
      */
      PACKED union
      {
        PACKED struct
        {
          uint8                            lp_sec;
          int8                                 daylt_savings;
          int16                            ltm_offset;
        } cdma_time;

        PACKED struct
        {
          uint8                            lp_sec;
          int16                            ltm_offset;
        } hdr_time;

      } time;

    } time_info;
    /**<
      Time obtained from sync channel / sector parameter msg.
      Mask used is CM_SS_CDMA_TIME_CHGD_MASK.
    */

  } cdma_info;
  /**< CDMA SS information
  */
} gw3_mode_info;
/**< Mode specific serving system information
*/

PACKED struct PACKED_POST
{
  int32                       hs_ind;
  /**< High speed service indication (Over WCDMA)
  */

  int32                       dtm_supp;
  /**< Dual Transfer mode indication (Over GSM)
  */

  int32                       egprs_supp;
  /**< EGPRS indication
  */

  int32                       hs_call_status;
  /**<
    Clients need to check for CM_API_CELL_SUPP_SERVICES
    and CM_API_CELL_SUPP_SERVICES_HS_CALL before
    using this structure.
  */

} gw_cell_srv_ind;

PACKED struct PACKED_POST
{
  int32                       hs_ind;
  /**< High speed service indication (Over WCDMA)
  */

  int32                       dtm_supp;
  /**< Dual Transfer mode indication (Over GSM)
  */

  int32                       egprs_supp;
  /**< EGPRS indication
  */

  int32                       hs_call_status;
  /**<
    Clients need to check for CM_API_CELL_SUPP_SERVICES
    and CM_API_CELL_SUPP_SERVICES_HS_CALL before
    using this structure.
  */

} gw3_cell_srv_ind;
/**<
  Clients need to check for CM_API_CELL_SUPP_SERVICES before
  using this structure.
  Structure to hold information about services in current cell.
  Information tied to CM_SS_SRV_IND_MASK.
*/

boolean                    gw_ps_data_suspend;
/**<
  Indicates that the current PS data service has been suspended.
  Mask used is CM_SS_PS_DATA_SUSPEND_MASK.
*/

boolean                    gw3_ps_data_suspend;
/**<
  Indicates that the current PS data service has been suspended.
  Mask used is CM_SS_PS_DATA_SUSPEND_MASK.
*/

PACKED struct PACKED_POST
{
  int32                   reject_srv_domain;
  /**< Srv domain on which registration is rejected
  */

  uint8                   reject_cause;
  /**< Registration Reject Cause
  */

} gw_reg_reject_info;
/**< Registration reject information
*/

PACKED struct PACKED_POST
{
  int32                   reject_srv_domain;
  /**< Srv domain on which registration is rejected
  */

  uint8                   reject_cause;
  /**< Registration Reject Cause
  */

} gw3_reg_reject_info;
/**< Registration reject information
*/

int32                     gw_cipher_domain;
/**<
  Can be used only if CM_CIPHERING_IND_SUPP is defined.
  Ciphering on CS or PS domain or both.
*/

int32                     gw3_cipher_domain;
/**<
  Can be used only if CM_CIPHERING_IND_SUPP is defined.
  Ciphering on CS or PS domain or both.
*/

LOG_RECORD_END

LOG_RECORD_DEFINE(LOG_CM_STATS_EVENT_C)
uint8                    version;
/* Version of the packet (CM_LOG_STATS_EVENT_LOG_PACKET_VERSION)
*/
int8  stats_event;

int8  type;   /**< Module responsible for generating a
                             report. */

int8   asubs_id;  /*Active Subscription*/

PACKED union PACKED_POST
{
  PACKED struct PACKED_POST
  {
    uint16                         changed_bitmask;
    /**< Indicates changed information. */
    byte                           attach_type;
    /**< Attached type. */
    byte                           cs_reject_cause;
    /**< Mobility Management (MM) reject cause code. */
    byte                           ps_reject_cause;
    /**< GPRS MM (GMM) reject cause code. */

    PACKED struct PACKED_POST
    {
      byte        tmsi [4];        /**< TMSI value. */
      byte        ptmsi[4];       /**< PTMSI value. */
      byte        gprs_attach_status;          /**< Attached or detached. */
      PACKED struct PACKED_POST
      {
        byte   identity[3];
      } rplmn; /**< PLMN info */
    } reg_nw_info;
    /**< Registered Network information. */

    PACKED struct PACKED_POST
    {
      dword     T3212_value;
      /**< Periodic location update timer value (T3212). */

      dword     T3312_value;
      /**< Periodic Routing Area Update (RAU) timer value (T3312). */
    } periodic_timers;
    /**< Periodic timer */

    PACKED struct PACKED_POST
    {
      PACKED struct PACKED_POST
      {
        boolean valid;
        /**< Whether a PDP address is present. */
        byte    pdp_type_org;
        /**< PDP type organization. */
        byte    pdp_type_num;
        /**< PDP type number. */
        byte    pdp_addr_len;
        /**< PDP address length. */
        byte    address[20];
        /**< PDP address. */
      } pdp_address; /**< Active PDP context: IP address. */

      PACKED struct PACKED_POST
      {
        boolean valid;
        /**< Whether QoS information is present. */
        byte    delay_class;
        /**< Delay class. */
        byte    reliability_class;
        /**< Reliability class. */
        byte    peak_throughput;
        /**< Peak throughput. */
        byte    precedence_class;
        /**< Precedence class. */
        byte    mean_throughput;
        /**< Mean throughput. */
        byte    traffic_class;
        /**< Traffic class. */
        byte    deliver_order;
        /**< Delivery order. */
        byte    deliver_err_sdu;
        /**< Delivery of an erroneous Service Data Unit. */
        byte    max_sdu_size;
        /**< Maximum Service Data Unit size. */
        byte    max_bit_rare_uplink;
        /**< Maximum uplink bitrate. */
        byte    max_bit_rare_downlink;
        /**< Maximum downlink bitrate. */
        byte    residual_ber;
        /**< Residual Bit Error Rate. */
        byte    sdu_err_ratio;
        /**< Service Data Unit error ratio. */
        byte    tranfer_delay;
        /**< Transfer delay. */
        byte    traffic_priority;
        /**< Traffic priority. */
        byte    guarantee_ber_uplink;
        /**< Guaranteed uplink Bit Error Rate. */
        byte    guarantee_ber_dwnlink;
        /**< Guaranteed downlink Bit Error Rate. */
        boolean sig_ind;
        /**< Signal indication. */
        byte    src_stat_desc;
        /**< Source statistics descriptor. */
        byte    ext_max_bit_rate_downlink;
        /**< Extended maximum downlink bitrate. */
        byte    ext_gtd_bit_rate_downlink;
        /**< Extended guaranteed downlink bitrate. */
        byte    ext_max_bit_rate_uplink;
        /**< Extended maximum uplink bitrate. */
        byte    ext_gtd_bit_rate_uplink;
        /**< Extended guaranteed uplink bitrate. */
        byte   ext2_max_bit_rate_downlink;
        /**< Extended 2 maximum downlink bitrate. */
        byte   ext2_gtd_bit_rate_downlink;
        /**< Extended 2 guaranteed downlink bitrate. */
        byte   ext2_max_bit_rate_uplink;
        /**< Extended 2 maximum uplink bitrate. */
        byte   ext2_gtd_bit_rate_uplink;
        /**< Extended 2 guaranteed uplink bitrate. */
      } qos;  /**< Active PDP context: QoS. */

      PACKED struct PACKED_POST
      {
        boolean valid;
        /**< Whether APN information is present. */
        word    apn_addr_len;
        /**< APN address length. */
        byte    address[MAX_APN_ADR_LEN];
        /**< APN address. */
      } apn;         /**< Active PDP context: APN. */

    } ps_call_info;
    /**< Active PS call information. */
    int32                  sys_mode;
    /**< RAT on which nas reported statistics */

    /**<emm current state*/
    byte               emm_state;
    /**<emm connection state*/
    byte               emm_connection_state;

  } nas_info;

  PACKED struct PACKED_POST
  {
    uint64                     changed_bitmask;
    /**< Indicates the updated values in GSM. */

    uint32                     timing_advance_value;
    /**< Time offset; changes with the mobile distance from the base station. */

    PACKED struct PACKED_POST
    {
      uint8                     num_of_items;
      /**< Number of NMRs. Set to zero if NMR information is invalid or there are
        no neighbors to report. */

      PACKED struct PACKED_POST
      {
        uint16                   arfcn;
        /**< Absolute Radio Frequency Channel Number. */

        uint8                    bsic;
        /**< Base Station Identity Code. */

        uint8                    rxlev;
        /**< Receive level. */

        PACKED struct PACKED_POST
        {
          uint32              cell_id;
          /**< Cell ID. */

          /* range 0..65535, cell id not available: 0xffffffff
             Data in sys_mode_ncell_id_info_s_type is invalid when cell_id invalid*/

          uint16              lac;
          /**< Location Area Code. */       /* range 0..65535 */

          PACKED struct PACKED_POST
          {
            byte                                identity[3];
            /**< Public Land Mobile Network ID. */
          } plmn;
          /**< PLMN ID. */
        } ncell_id;
        /**< Neighboring cell ID. */

        int32                        C1;
        /*C1 as defined in 45.008 6.4, default: 0*/

        int32                        C2;
        /*C1 as defined in 45.008 6.4, default: 0*/

        int32                        C31;
        /*C31 as defined in 45.008 10.1.2, default: 0*/

        int32                        C32;
        /*C32 as defined in 45.008 10.1.2, default: 0*/
      } nmr[SYS_ENG_MODE_MAX_NMR_ITEMS];
      /**< NMR array. */

      uint64                      time_stamp;
      /**< Indicates when information was collected. */
    } nmr_info;
    /**< Network Measurement Report engine mode. */

    uint32                     rx_qual;
    /**< Reception quality. */  /*0xffffffff - Invalid*/

    int16                   tx_pwr_in_use[SYS_ENG_MODE_TX_PWR_NUM_TIMESLOTS];
    /**< Transmission power in use. */                      /*0 - Invalid*/

    PACKED struct PACKED_POST
    {
      uint32                   cell_id;
      /**< Cell ID. Range is 0 to 65535; cell ID not available = 0xffffffff
           (all data in eng_mode_scell_info_t is invalid when cell_id is invalid). */

      uint16             lac;
      /**< Location Area Code. */                      /* range 0..65535 */

      PACKED struct PACKED_POST
      {
        byte                identity[3];
        /**< Public Land Mobile Network ID. */
      } plmn;


      uint16                   arfcn;
      /**< Absolute Radio Frequency Channel Number. */     /*range 0..1023 */

      uint8                    bsic;
      /**< Base Station Identity Code. */             /*range 0…63, see 24.008 10.5.1.3*/

      uint8            cell_info_cause;
      /**< Cause code of the serving cell. */

      uint16                   rac;
      /**< RAC (not used). */        /*range 0..255, invalid=0xffff (i.e. GPRS not supported)*/

    } scell_info;

    PACKED struct PACKED_POST
    {
      int32                   C1;
      /**< C1 as defined in 45.008 6.4, default: 0. */

      int32                   C2;
      /**< C2 as defined in 45.008 6.4, default: 0. */

      uint16                  rx_lev;
      /**< RX_LEV range is 0 to 63. See 3GPP TS 45.008 Section 8.1.4; default = 0. */

      uint16                  rx_qual_sub;
      /**< Reception quality in dedicated mode. The range is 0 to 7; 0xffff is
           invalid. */

      uint8           channel_type;
      /**< Indicates channel rx_lev is measured on. */

    } scell_measurements;

    PACKED struct PACKED_POST
    {
      uint8   ul_chan_mode; /**< Uplink channel mode.  */

      uint8   dl_chan_mode; /**< Downlink channel mode: \n
                                    0 -- Full rate.\n
                                    1 -- Half rate.\n
                                    2 -- Enhanced full rate.\n
                                    3 -- AMR full rate.\n
                                    4 -- AMR half rate.\n
                                    0xFF -- Invalid mode. */

      uint8   ul_acs[4];    /**< Uplink Active Code Set; AMR coding group. */

      uint8   dl_acs[4];    /**< 0 -- 4.75 kbit/s.\n
                                   1 -- 5.15 kbit/s.\n
                                   2 -- 5.90 kbit/s.\n
                                   3 -- 6.70 kbit/s.\n
                                   4 -- 7.40 kbit/s.\n
                                   5 -- 7.95 kbit/s.\n
                                   6 -- 10.2 kbit/s.\n
                                   7 -- 12.2 kbit/s.\n
                                   0xFF -- Undefined.  */

      boolean dl_dtx;       /**< 1 = On, 0 = Off; default = 0. */

      uint16  dl_c_over_i;  /**< Range 0..63, 0xFFFF -- Invalid. */

    } amr_info;

    uint8                      dtx_ul_status;
    /**< Discontinuous Transmission status. */    /*0 - Off, 1 - On, 0xff - Invalid*/

    PACKED struct PACKED_POST
    {
      uint8 mac_mode;       /**< As specified in 3GPP TS 4.060 chapter 11.2.7.2.\n
                                   0 -- Dynamic Allocation.\n
                                   1 -- Extended Dynamic Allocation.\n
                                   0xff -- Invalid. */

      uint8  tbf_mode;      /**< 1 -- GPRS.\n
                                   2 -- EGPRS.\n
                                   0xff -- Invalid. */

    } tbf_info;
    /**< Dynamic information about the currently active EGPRS TBF. */

    PACKED struct PACKED_POST
    {
      uint8 nc_mode;
      /**< Network Control mode; relates to how the mobile sends measurements and
         performs reselection:
            - 0 -- NC1 mode.
            - 1 -- NC2 mode.
            - 0xff -- Invalid. */

      uint8 nmo;
      /**< Network Mode of Operation:
           - 0 -- NMO I.
           - 1 -- NMO II.
           - 2 -- NMO III.
           - 0xff -- Invalid. */

    } nw_params;
    /**< Network parameters, such as NMO, which apply to an operator. */

    uint8                      gprs_ul_cs;
    /**< GPRS coding scheme used in Upload GPRS TBFs. */      /* Range 0-3, 0xff - invalid */

    uint8                      gprs_dl_cs;
    /**< GPRS coding scheme used in Download GPRS TBFs. */    /* Range 0-3, 0xff - invalid */

    uint8                      egprs_ul_mcs;
    /**< EGPRS coding scheme used in Upload EGPRS TBFs. */    /* Range 0-8, 0xff - invalid */

    uint8                      egprs_dl_mcs;
    /**< EGPRS coding scheme used in DL EGPRS TBFs. */        /* Range 0-8 , 0xff - invalid */

    PACKED struct PACKED_POST
    {

      uint8 lq_meas_mode;
      /**< Link Quality measurement mode. */
      /* spec 4.60 chap 11.2.7.2 ref for detail values 0 to 3. 0xff = invalid */

      uint8 bep_period;
      /**< Bit Error Probability measured during EGPRS TBF. */
      /* spec ref 4.60 cha 12.24.1 and 5.08  chap 10.2.3.2.1 0xff = invalid */

    } edge_meas;
    /**< Quality measurements taken while an EGPRS TBF is active. */

    PACKED struct PACKED_POST
    {

      uint8     pbcch_present;
      /**< Presence of PBCCH in cell:
         - 0 -- No.
         - 1 -- Yes.
         - 0xff -- Invalid. */

      uint8     gprs_rxlev_access_min; /*0..63, 0xff - invalid, see 45.008*/
      uint8     gprs_ms_txpwr_max_cch; /*0..31, 0xff - invalid, see 45.005, 45.008*/

    } scell_config;
    /**< Serving cell information that describes the configuration data of the
       cell. */

    PACKED struct PACKED_POST
    {
      uint8    hopping_status;
      /**< Hopping status:
         - 0 -- No.
         - 1 -- Yes.
         - 0xff -- Invalid. */
    } hopping_status;
    /*ENG_MODE_HOPPING_STATUS*/

    uint16 g_bcch;
    /**< Channel number. ARFCN. Range - 0 to 1023 */

    uint32 g_ta;
    /**< Timing Advance. Range - 0 to 63 */

    PACKED struct PACKED_POST
    {
      uint8 timeslot; /*timeslot number 0..7, 0xff - invalid*/
    } current_timeslot;
    /*ENG_MODE_CURRENT_L1_TIMESLOT*/

  } gsm_info;

  PACKED struct PACKED_POST
  {
    uint64 changed_bitmask;
    /**< Bitmask. */

    PACKED struct PACKED_POST
    {

      PACKED struct PACKED_POST
      {

        uint16 uarfcn;
        /**< Serving cell DL UARFCN. */

        uint16 psc;
        /**< Serving cell primary scrambling code. */

        PACKED struct PACKED_POST
        {

          byte                identity[3];
          /**< Public Land Mobile Network ID. */

        } plmn_id;
        /**< PLMN ID. */

        uint16 lac_id;
        /**< Indicates the LAC. */

        uint16  rac_id;
        /**< Indicates the RAC. */

        uint32 ura;
        /**< UTRAN Registration Area (URA). */

        uint32 cell_id;
        /**< Indicates the service cell ID. */

        uint8 nw_operation_mode;
        /**< Network operation mode. */

      } wcdma_serving_cell_info;
      /**< Serving system cell information. */

      PACKED struct PACKED_POST
      {

        uint16 psc;
        /**< Serving cell Primary Scrambling Code. */

        int16 rscp;
        /**< Received Signal Code Power. */

        int16 ecio;
        /**< Ec/Io. */

        int16 squal;
        /*S_qual */

        int16 srxlev;
        /*S_rxlev */

      } wcdma_serving_cell_power_info;
      /**< Power range of the serving cell. */

      PACKED struct PACKED_POST
      {

        uint16 uarfcn;
        /**< DL UARFCN. */

        uint16 psc;
        /**< Primary Scrambling Code. */

        PACKED struct PACKED_POST
        {

          byte                  identity[3];
          /**< Public Land Mobile Network ID. */

        } plmn_id;
        /**< PLMN ID. */

        uint16 lac_id;
        /**< Indicates the LAC. */

        uint16  rac_id;
        /**< Indicates the RAC. */

        uint32 cell_id;
        /**< Indicates the network cell. */

      } wcdma_active_set_reference_rl_info;
      /**< Active set reference Radio Link information. */

      PACKED struct PACKED_POST
      {

        uint8 num_rls;
        /**< Number of Radio Links in the active set. */

        uint16 psc[SYS_WCDMA_MAX_ASET_CELLS];
        /**< Primary Scrambling Codes. */

        uint32 cell_id[SYS_WCDMA_MAX_ASET_CELLS];
        /**< Cell ID. */

        int16  rscp[SYS_WCDMA_MAX_ASET_CELLS];
        /**< Received Signal Code Power. */

        int16 ecio[SYS_WCDMA_MAX_ASET_CELLS];
        /**< Ec/Io. */

        /* spreading factor of the channel */
        uint8 sf;

        /* Physical channel type FDPCH/DPCH */
        uint8 phy_chan_type;

        /* Slot format for the channel */
        uint8 slot_format;

        /*indicates CM On or not*/
        boolean is_compressed_mode_on;

      } wcdma_active_set_info;
      /**< Active set information. */

      PACKED struct PACKED_POST
      {

        uint8 num_of_freqs;
        /**< Number of frequencies. */

        PACKED struct PACKED_POST
        {

          uint16 uarfcn;
          /**< UARFCN. */

          uint8 num_of_cells;
          /**< Number of cells in the frequency. */

          PACKED struct PACKED_POST
          {
            uint16 psc;         /**< Primary Scrambling Code. */
            int16 rscp;         /**< Received Signal Code Power. */
            int16 ecio;         /**< Ec/Io. */
            int16 squal;   /*S_qual */
            int16 srxlev;    /*S_rxlev */
            int16 rank;  /* Rank of the cell */
            uint8 set;  /* Set of the cell */

          } cell[SYS_WCDMA_MAX_MONITORED_CELLS_PER_FREQ];
          /**< Cells information. */

        } freq[SYS_WCDMA_MAX_MONITORED_FREQ];
        /**< Frequency information. */

        uint8 num_gsm_cells;
        /**< Number of GSM cells. */  /* Range 0-WCDMA_MAX_MONITORED_GSM_CELLS */

        PACKED struct PACKED_POST
        {
          /*ARFCN*/
          uint16 arfcn;    /**< ARFCB; range 0-1023. */
          /*BSIC-NCC*/
          uint8 bsic_ncc;  /**< BSIC-NCC; range 0-7, 0XFF is invalid. */
          /*BSIC-BCC*/
          uint8 bsic_bcc;  /**< BSIC-BCC; range 0-7, 0XFF is invalid. */
          /*RSSI*/
          int16 rssi;    /**< RSSI. */
          /*BCCH*/
          uint16 bcch;     /**< BCCH */

          int16 rank;  /* Rank of the cell */

        } gsm_cell[SYS_WCDMA_MAX_MONITORED_GSM_CELLS];
        /**< GSM cell information. */

        uint8 num_lte_cells;
        /**< Number of LTE cells. */  /* Range 0- SYS_WCDMA_MAX_MONITORED_LTE_CELLS */

        PACKED struct PACKED_POST
        {

          /*EARFCN*/
          uint32 earfcn;

          /* The physical cell id of the detected cell */
          uint16 cell_id;

          /* Max RSRP comb.value combined across Tx-Rx pairs. In linear scale. */
          int16 rsrp;

          /* Max RSRQ value combined across Tx-Rx pairs. In linear scale. */
          int16 rsrq;

          /* LTE cell rxlev */
          int16  s_rxlev;

          /* check if a cell is LTE */
          boolean is_tdd_freq;

        } lte_cell[SYS_WCDMA_MAX_MONITORED_LTE_CELLS];


      } wcdma_monitored_set_info;
      /**< Monitored set information. */

      uint8                wcdma_rrc_state;
      /**< Radio Resource Control state type. */

      uint16 gw_dl_bler;
      /**< Block Error Ratio. Range - 0 to 63. */

      int32 w_agc;
      /* rx agc, only WCDMA need AGC, GSM can use rssi */
      /**< Automatic Gain Control. Range - 0 to -106 dB */

      int32 w_txagc;
      /**< Automatic Gain Control. Range - 0 to -106 dB */


    } wcdma_event_info;

  } wcdma_info;

  PACKED struct PACKED_POST
  {
    uint32                                 changed_bitmask;
    /**< Bitmask of the changed field. */

    PACKED struct PACKED_POST
    {
      uint8    cdma_prot_state;
      /**< Protocol state. */
      uint8               band_class;
      /**< Band class supported. */
      uint8  slotted_mode;
      /**< Slotted mode or not. */
      uint8     pch_rate;
      /**< Paging Channel rate. */
      uint16              active_so;
      /**< Active Service Option. */

      PACKED struct PACKED_POST
      {
        dword           min1;  /**< 24 bits. */
        dword           min2;  /**< 10 bits. */
      } min;
      /**< Mobile Identification Number of current state. */

      int8              ms_pref_sci;
      /**< Mobile Station preferred SCI (0-7). */
      uint8               qpch_status;
      /**< Quick Paging Channel (QPCH) ON (1) or OFF (0). */ /* 1X L1 */
      uint8           num_active_pilots;
      /**< Number of active pilots in traffic. */
    } state_info;
    /**< CDMA state information. */

    PACKED struct PACKED_POST
    {
      int16       rx_pwr;     /**< Rx instantaneous power in dBm. */
      int16       tx_pwr;     /**< Tx instantaneous power in dBm.*/
      uint16      pch_loss_count; /**< Paging Channel loss count. */
      uint16      tch_loss_count; /**< Traffic Channel loss count. */
      uint16      idle_ho_count;  /**< Idle hand off count. */
      uint16      acc_req_count;  /**< Number of access channel requests. */
      uint16      acc_rsp_count;  /**< Number of access channel responses. */
      uint16      failed_acc_att_count;  /**< Unsuccessful access attempts. */
      uint16      t41_expiry_count; /**< T41 timer expiration count. */
    } meas_info;
    /**< Channel measurement information. */

    PACKED struct PACKED_POST
    {
      PACKED struct PACKED_POST
      {
        uint16 pn;    /**< Pseudo-Noise. */
        uint16 ecio;    /**< Ec/Io. */
      } pilot_info[SYS_CDMA_MAX_ASET_PILOTS];
      /**< CDMA active set type list. */

    } active_set_info;
    /**< Active set information. */

    PACKED struct PACKED_POST
    {
      uint16         pilot_pn_offset;
      /**< Pilot Pseudo-Noise offset. */
      uint16         channel;
      /**< Channel. */
      uint8          pagech;
      /**< Paging Channel. */
      uint16         base_id;
      /**< Base station ID */
      uint16         sid;
      /**< System ID. */
      uint16         nid;
      /**< Network ID. */
      uint8          packet_zone_id;
      /**< Packet zone ID. */
      uint8          p_rev;
      /**< Protocol revision. */
      uint8          min_p_rev;
      /**< Mobile Identification Number revision. */
      int8           nw_pref_sci;
      /**< Network preferred SCI (0-7). */
    } serv_sys_info;
    /**< Serving system information. */

    PACKED struct PACKED_POST
    {
      uint16         fsch0_mux;
      /**< Forward Supplemental Channel MUX. */

      uint8          fsch0_rc;
      /**< Forward Supplemental Channel Radio Configuration. */

      boolean        fsch0_turbo_supported;
      /**< Whether turbo is supported. */

      uint16         rsch0_mux;
      /**< Reverse Supplemental Channel MUX. */

      uint8          rsch0_rc;
      /**< Reverse Supplemental Channel Radio Configuration. */

      boolean        rsch0_turbo_supported;
      /**< Reverse Supplemental Channel turbo. */

      boolean        fsch0_active;
      /**< Forward Supplemental Channel active. */

      uint8          fsch0_duration;
      /**< Forward Supplemental Channel duration. */

      uint8          fsch0_sccl_index;
      /**< Sample correlate, choose largest index. */

      uint8          fsch0_num_pilots;
      /**< Number of pilots. */

      boolean        rsch0_active;
      /**< Reverse Supplemental Channel active. */

      uint8          rsch0_duration;
      /**< Reverse Supplemental Channel duration. */

      uint8          scrm_seq_num;
      /**< Sequence number. */

    } channel_config_info;
    /**< Channel configuration information. */

    PACKED struct PACKED_POST
    {
      uint16         orig_acc_att_count;
      /**<  Origination access attempts count. */

      uint16         page_rsp_acc_att_count;
      /**< Page response access attempts count. */

    } call_stat;
    /**< Originations/terminations statistics. */

    PACKED struct PACKED_POST
    {

      uint16         powerdown_reg_count;
      /**< Power down registration count. */

      uint16         timer_reg_count;
      /**< Timer-based registration count. */

      uint16         parameter_reg_count;
      /**< Parameter-based registration count. */

      uint16         powerup_reg_count;
      /**< Power up registration count. */

      uint16         ordered_reg_count;
      /**< Ordered registration count. */

      uint16         zone_reg_count;
      /**< Zone-based registration count. */

      uint16         distance_reg_count;
      /**< Distance-based registration count. */

    } reg_stat;
    /**< Registration information. */

    int32 cdma_1x_rx0_agc;
    /**< Automatic Gain Control. Range - 0 to -106 dB */

    int32 cdma_1x_rx1_agc;
    /**< Automatic Gain Control. Range - 0 to -106 dB */

  } cdma_info;

  PACKED struct PACKED_POST
  {
    uint32                    changed_bitmask;
    /**< Indicates the fields that have changed. */

    PACKED struct PACKED_POST
    {
      uint8    band;
      /**< Band class: 0, 1, etc. */

      uint16     chan_num;
      /**< CDMA channel number. */

    } channel;
    /**< Channel: band and frequency information. */

    uint8 hdr_prot_state;
    /**< HDR protocol state: inactive, initialized, idle, or connected. */

    int16   c2i;
    /**< SINR of the serving sector in dB. */

    int32   rx_dbm;
    /**< Rx power in dBm. */

    int32   tx_dbm;
    /**< Tx power in dBm. */

    int16   ecio;
    /**< Ec/Io of reference sector in dB. */

    uint32  uati_24;
    /**< Three least significant bits of the Unique Access Terminal
       Identifier (UATI). */

    uint8   color_code;
    /**< Color code for the UATI. */

    uint8   subnet_mask;
    /**< Subnet mask for the UATI. */

    uint16  mac_index;
    /**< Current MAC index. */

    int32 cdma_evdo_rx0_agc;
    /**< Automatic Gain Control. Range - 0 to -106 dB */

    int32 cdma_evdo_rx1_agc;
    /**< Automatic Gain Control. Range - 0 to -106 dB */

  } hdr_info;

  PACKED struct PACKED_POST
  {
    uint32                        changed_bitmask;
    /**< Bitmask. */

    PACKED struct PACKED_POST
    {

      uint8    pref_sys;
      /**< Preferred system. */

      int32      mode_pref;
      /**< Mode. */

      uint64     band_cap;
      /**< PCS/Cellular/AWS supported band capability. */

    } ph_info;
    /**< Phone information. */

    PACKED struct PACKED_POST
    {

      PACKED struct PACKED_POST
      {

        uint8         id_type;
        /**< Type of the system ID. */

        PACKED union PACKED_POST
        {

          PACKED struct PACKED_POST
          {
            uint16     sid;
            /**< System ID. */

            uint16     nid;
            /**< Network ID. */

            uint16     mcc;
            /**< Mobile County Code. */

            byte     imsi_11_12;
            /**< IMSI_11_12. */

          } is95;
          /**< IS-95 system ID. */

          byte                 is856[16];
          /**< IS-856 system ID. */

          PACKED struct PACKED_POST
          {

            byte                  identity[3];
            /**< Public Land Mobile Network ID. */

          } plmn;
          /**< UMTS (WCDMA/GSM) system ID. */

          PACKED struct PACKED_POST
          {

            uint8                len;
            /**< Length of the SSID; if == 0, then SSID = broadcast SSID.  */

            char                 ssid[SYS_WLAN_SSID_MAX_SIZE];
            /**< SSID of the WLAN system. */

          } ssid;
          /**< WLAN system ID. */

          PACKED struct PACKED_POST
          {
            PACKED struct PACKED_POST
            {

              byte                  identity[3];
              /**< Public Land Mobile Network ID. */

            } plmn;  /**< Public Land Mobile Network. */

            uint16     lac;   /**< Location Area Code. */

          } plmn_lac;
          /**< UMTS (WCDMA/GSM) PLMN and LAC ID. */

        } id;
        /**< The system identifier. */

      } sel_sys;
      /**< Selected system. */

      uint8    roam_ind;
      /**< Roaming indication. */

    } sys_sel_info;
    /**< System selection information. */

    uint16                          prl_ver;
    /**< PRL version. */

    uint16                        send_key_orig_count;
    /**< Number of times the SEND key was pressed. */

  } cm_info;

  PACKED struct PACKED_POST
  {
    uint32                   changed_bitmask;
    /**< Bitmask indicating which fields changed. */

    PACKED struct PACKED_POST
    {

      uint32 hdr_fwd_data_rate;
      /**< HDR forward link data rate in bytes/sec. */

      uint32 hdr_rev_data_rate;
      /**< HDR reverse link data rate in bytes/sec. */

    } ds_hdr_link_info;
    /**< Forward and reverse link data. */

  } ds_info;

  PACKED struct PACKED_POST
  {
    uint32                    changed_bitmask;
    /**< Bitmask indicating which fields changed. */

    PACKED struct PACKED_POST
    {
      boolean          is_service_available;
      /**< flag to indicate if there is service. */

      int8    sys_mode;
      /**< system mode. Valid only is
         is_service_available is true. */

      PACKED struct PACKED_POST
      {
        boolean   valid;
        /**< When FALSE none of the fields in the structure are valid */

        boolean   idle;
        /**< Flag to indicate whether the UE is in idle mode.
           If TRUE the UE is in idle mode all idle sub-structs
           will be considered valid. If FALSE all idle mode sub-structs
           should be considered invalid */

        PACKED struct PACKED_POST
        {
          uint32    earfcn;
          /**< Earfcn of serving cell*/

          uint16    serving_cell_id;
          /**< LTE Serving cell id */

          uint8   num_lte_cells;
          /**< number of LTE Serving cells */

          PACKED struct PACKED_POST
          {
            uint16    pci;
            /**< Physical cell ID */

            int16     rsrq;
            /**< The current Reference Signal Receive Quality as measured by L1.
               The quantities are in dB.  Range: -20.0 ..-3.0 dB. x10 */

            int16     rsrp;
            /**< The current Reference Signal Receive Power in dBm x10 as measured by L1.
               Range: -44..-140 */

            int16     rssi;
            /**< The current Received Signal Strength Indication as measured by L1.
               Values are in dBm x10 with Range: -120.0 .. 0  */

            PACKED struct PACKED_POST
            {
              int16   srxlev;
              /**< Srxlev */

            } idle;

          } cell[SYS_LTE_NGBR_IND_NUM_CELLS];
          /**< The lte cell paramters */

          PACKED struct PACKED_POST
          {
            uint8   cell_resel_priority;
            /**< Priority for serving freq */

            uint8   s_non_intra_search;
            /**< S non intra search threshold to control non-intra freq searches */

            uint8   thresh_serving_low;
            /**< threshServingLow */

            uint8   s_intra_search;
            /**< Threshold current cell meas must fall below to consider
               intra freq for reselection. */

          } idle;
          /**< Idle state parameters. */

        } lte_intra;
        /**< LTE intra-frquency parameters */

        PACKED struct PACKED_POST
        {
          uint8      num_freqs;
          /**< Number of freq. */

          PACKED struct PACKED_POST
          {
            uint32    earfcn;
            /**< Earfcn of serving cell*/

            uint8     num_lte_cells;
            /**< The number of LTE cells */

            PACKED struct PACKED_POST
            {
              uint16    pci;
              /**< Physical cell ID */

              int16   rsrq;
              /**< The current Reference Signal Receive Quality as measured by L1.
                 The quantities are in dB.  Range: -20.0 ..-3.0 dB. x10 */

              int16   rsrp;
              /**< The current Reference Signal Receive Power in dBm x10 as measured by L1.
                 Range: -44..-140 */

              int16   rssi;
              /**< The current Received Signal Strength Indication as measured by L1.
                 Values are in dBm x10 with Range: -120.0 .. 0  */

              PACKED struct PACKED_POST
              {
                int16   srxlev;
                /**< Srxlev */

              } idle;

            } cell[SYS_LTE_NGBR_IND_NUM_CELLS];
            /**< The lte cell paramters */

            PACKED struct PACKED_POST
            {
              uint8    threshX_low;
              /**< Srxlev-value of an evaluated cell must be greater than these values to
                 be considered for reselection */

              uint8    threshX_high;
              /**< Srxlev-value of an evaluated cell must be lesser than these values to
                 be considered for reselection */

              uint8    cell_resel_priority;
              /**< Cell reselection priority */

            } idle;
            /**< Per-freq idle param */

          } freqs[SYS_LTE_NGBR_IND_NUM_FREQS];
          /**< Frequency info */

        } lte_inter;
        /**< LTE inter-frquency parameters */

        PACKED struct PACKED_POST
        {
          uint8      num_freq_groups;
          /**< The number of GSM frequency groups and the size of the freq_group
             array */

          PACKED struct PACKED_POST
          {
            uint8    num_gsm_arfcn;
            /**< the number of gsm arfcns indicated and the
               size of the gsm array */

            PACKED struct PACKED_POST
            {
              uint16    arfcn;
              /**< GSM frequency being reported */

              boolean   band_1900;
              /**< Band indicator for the GSM ARFCN, only valid if arfcn is in the
                 overlapping region. If TRUE and the cell is in the overlapping region
                 it is on the 1900 band, if FALSE it is on the 1800 band */

              boolean   cell_id_valid;
              /**< Flag indicating whether the BSIC ID is valid */

              uint8   bsic_id;
              /**< BSIC ID */

              int16   rssi;
              /**< Measured RSSI value in Db x10 */

              PACKED struct PACKED_POST
              {
                int16   srxlev;
                /**< Srxlev */
              } idle;

            } arfcn[SYS_LTE_NGBR_IND_NUM_CELLS];
            /**< The gsm cell paramters */

            PACKED struct PACKED_POST
            {
              uint8   cell_resel_priority;
              /**< The priority of this frequency group */

              uint8   thresh_gsm_high;
              /**< Reselection threshold for high priority layers.
                */

              uint8   thresh_gsm_low;
              /**< Reselection threshold for low priority layers.
                */

              uint8   ncc_permitted;
              /**<  Bitmask specifying whether a neighbour with a particular
                Network Color Code is to be reported.  Bit "n" set to 1 means
                that a neighbour with NCC "n" should be included in the report.
                This flag is synonymous with a blacklist in other RATs
               */

            } idle;
            /**< Per-group idle parameters */

          } freq_group[SYS_LTE_NGBR_IND_NUM_FREQS];
          /**< GSM freq group info. */

        } gsm;
        /**< GSM cell information */

        PACKED struct PACKED_POST
        {
          uint8    num_wcdma_freqs;
          /**< The number of WCDMA frequencies, and the size of the freq array */

          PACKED struct PACKED_POST
          {
            uint16     uarfcn;
            /**< wcdma Layer frequency */

            uint8      num_wcdma_cells;
            /**< The number of wcdma cells indicated and the
            size of the wcdma array */

            PACKED struct PACKED_POST
            {
              uint16         psc;
              /**< Scrambling code*/

              int16        cpich_rscp;
              /**< This is the absolute power level of the CPICH as received by the
                 UEin dBm x10 Defined in 25.304*/

              int16        cpich_ecno;
              /**< CPICH Ec/No is the ratio of the received energy per PN chip
                 for the CPICH to the total received power spectral density at
                 the UE antenna connector. in dB x10 .Defined in 25.304 */

              PACKED struct PACKED_POST
              {
                int16        srxlev;
                /**< Srxlev */
              } idle;

            } cell[SYS_LTE_NGBR_IND_NUM_CELLS];
            /**< The wcdma cell paramters */

            PACKED struct PACKED_POST
            {
              uint8   cell_resel_priority;
              /**< Cell reselection priority 0..7 */

              uint16    thresh_Xhigh;
              /**< Reselection threshold. Upper limit */

              uint16    thresh_Xlow;
              /**< Reselection threshold. Lower limit */

            } idle;
            /**< The wcdma cell freq idle paramters */

          } freq [SYS_LTE_NGBR_IND_NUM_FREQS];
          /**< WCDMA neighbor cell freq info. */

        } wcdma;
        /**< WCDMA cell information */

      } lte_nbr_info;
      /**< LTE Signal and Nbr Information */

    } lte_meas_info;
    /**< LTE RAT measurement info. */
  } lte_info;
} prm;

LOG_RECORD_END

/*lint -e717 */             /* DIAG uses a do ... while (0) construct. */
/*lint -e754 */             /* Local structure member for not referenced */
/*lint -e778 */             /* Re reference NONE+1, which evaluates to 0 */
/*===========================================================================
                        TYPE DEFINITIONS FOR EVENTS
===========================================================================*/
#ifdef CM_GW_SUPPORTED
/* Constants used for processing USSD messages */
#define DEFAULT_ALPHABET             0x00
#define ALPHABET_MASK                0xF0
#define USS_DEF_ALPHABET_LANG_UNSPEC 0x0F
#endif /*CM_GW_SUPPORTED */


#ifdef CM_GW_SUPPORTED
#define CM_LOG_DATA_OFFSET 12 /**< Offset in log packet to write USSD indication type */
#define CM_RESPONSE_NOTIFY_DATA_LENGTH_OFFSET 13 /**< Offset to write data length in log packet */
#define CM_RESPONSE_NOTIFY_DATA_OFFSET 14 /**< Offset to write actual data in log packet */
#define CM_SIZE_RESPONSE_NOTIFY_HEADER 2 /**< Size reqd for indication type and data len in log packet */
#endif /* CM_GW_SUPPORTED */


#define CMLOG_MAX_EVENT_PAYLOAD 8

/*===========================================================================
                         DATA DECLARATIONS
===========================================================================*/

/** Type for CM test client.
*/
typedef struct cmlog_client_sub_info_s
{

  sys_sys_mode_e_type              sys_mode;
  /**< Indicates current system mode */

  cm_mode_pref_e_type              mode_pref;
  /**< Indicates current mode preference */

  cm_gw_acq_order_pref_e_type      acq_order_pref;
  /**< The acquisition order preferred by the client */

  cm_srv_domain_pref_e_type        srv_domain_pref;
  /**<
    The service domain preferred by the client.
    This field is set after a call to cm_ph_cmd_sys_sel_pref_new.
  */

  cm_band_pref_e_type              band_pref;
  /**< Indicates current band preference, CDMA, GW, WLAN systems */

  cm_roam_pref_e_type              roam_pref;
  /**< Indicates current roam preference */

  cm_hybr_pref_e_type              hybr_pref;
  /**< Indicates hybrid preference */

  cm_network_sel_mode_pref_e_type  network_sel_mode_pref;
  /**< The network selection mode selected by the client */

  sys_srv_status_e_type            srv_status;
  /**< Indicates current service status */

  cm_cell_srv_ind_s_type           cell_srv_ind;
  /**< Indicates the CELL SERVICE Type */

  cm_band_pref_e_type              lte_band_pref;
  /**< Indicates current band preference LTE systems */

  cm_band_pref_e_type              tds_band_pref;
  /**< Indicates current band preference TD-SCDMA systems */

  /* CDMA specific info */
  sys_channel_num_type             active_channel;
  sys_band_class_e_type            band_class;
  uint16                           packet_zone;
  sys_roam_status_e_type           roam_status;
  sys_base_lat_type                latitude;
  sys_base_long_type               longitude;
  cm_country_code_type             mcc;

} cmlog_client_sub_info_s_type;

typedef struct cmlog_client_s
{

  cm_client_id_type                client;

  sys_oprt_mode_e_type             oprt_mode;
  /**< Indicates current operating mode */

  boolean                          offline_signal_recvd;
  /**< Indicates if CM received TASK_OFFLINE_SIG signal from TMC */

  cmlog_client_sub_info_s_type     sub_info[MAX_SIMS];
  /**< Indicates current system sub level info */

} cmlog_client_s_type;


typedef struct cmlog_call_orig_dsds_s
{
  uint8 asubs_id;
  uint8 call_id;
  uint8 call_type;
  uint8 srv_type;
} cmlog_call_orig_dsds_s_type;

typedef struct cmlog_call_orig_s
{
  uint8 call_id;
  uint8 call_type;
  uint8 srv_type;
} cmlog_call_orig_s_type;

typedef struct cmlog_call_orig_2_s
{
  uint8 call_id;
  uint8 call_type;
  uint8 srv_type;
  uint8 client_id;
  uint8 subs_id;
} cmlog_call_orig_2_s_type;

typedef struct cmlog_call_connect_dsds_s
{
  uint8 asubs_id;
  uint8 call_id;
  uint8 call_type;
  uint8 sys_mode;
} cmlog_call_connect_dsds_s_type;

typedef struct cmlog_call_connect_s
{
  uint8 call_id;
  uint8 call_type;
  uint8 sys_mode;
} cmlog_call_connect_s_type;

typedef struct cmlog_call_connect_2_s
{
  uint8 call_id;
  uint8 call_type;
  uint8 sys_mode;
  uint8 client_id;
  uint8 subs_id;
} cmlog_call_connect_2_s_type;

typedef struct cmlog_call_end_s
{
  uint8 num_calls;
  uint8 call_id;
} cmlog_call_end_s_type;

typedef struct cmlog_call_end_2_s
{
  uint8 num_calls;
  uint8 call_id;
  uint8 client_id;
  uint8 subs_id;
} cmlog_call_end_2_s_type;

typedef struct cmlog_call_client_id_s
{
  uint8 client_id;
  uint8 reserved1;
  uint8 reserved2;
} cmlog_call_client_id_s_type;

typedef struct cmlog_srv_confirmed_s
{
  uint8                         asubs_id;
  sys_plmn_service_state_s_type srv_info;
} cmlog_srv_confirmed_s_type;

/** Holds call_state, call_type and call_id for allocated call_id's
*/
typedef struct cmlog_call_state_s
{

  cm_call_id_type            call_id;
  /**< call id */

  cm_call_state_e_type       call_state;
  /**< Call state - idle, orig, etc. */

  cm_call_type_e_type        call_type;
  /**< Call state - idle, orig, etc. */

  boolean                    is_ps_call_dormant;
  /**<
    Gives status of ps call in converation.
    Only applicable to ps calls.
    FALSE - PS call is active.
    TRUE  - PS call is dormant.
  */

  sys_modem_as_id_e_type     asubs_id;

} cmlog_call_state_s_type;

/** Type for logging ssac call events
*/
typedef struct
{
  uint8    ssac_result;
  /**<
    Indicate the ssac result - accept or reject the call.
    - 0 - REJECT.
    - 1 - ACCEPT.
  */

  uint8    mmtel_type;
  /**<
    mmtel_type indicating voice/video over LTE.
    - 0 - NONE
    - 1 - VOICE
    - 2 - VIDEO
    - 3 - VOICE_VIDEO
  */

  uint8    ssac_param_barring_factor;
  /**< Barring factor for ssac_call_type */

  uint16    ssac_param_barring_time;
  /**< Barring time for ssac_call_type */
} cmlog_event_ssac_call_s_type;

/** Type for logging ssac timer events
*/
typedef struct
{
  uint8   timer_action;
  /**<
    Indicate to start/stop/cancel the ssac backoff timer.
    - TIMER_NONE
    - TIMER_START
    - TIMER_STOP
    - TIMER_CANCEL
  */

  uint8   mmtel_type;
  /**<
    mmtel_type indicating voice/video over LTE.
    - 0 - NONE
    - 1 - VOICE
    - 2 - VIDEO
    - 3 - VOICE_VIDEO
  */

  uint8   backoff_timer_byte_0;
  uint8   backoff_timer_byte_1;
  uint8   backoff_timer_byte_2;
  uint8   backoff_timer_byte_3;
  /**<
    Indicates the backoff timer value. Only applicable to start timer.
    For timer_action other than TIMER_START, the value will be 0.
    Note: These bytes are specified individually to force packing
    this struct to 6 bytes. To copy a uint32 to this location,
    use memscpy.
  */

} cmlog_event_ssac_timer_s_type;


/*=========================================================================*/


/*-------- cmlog_init --------*/
/**
@brief
  This function should be called at phone startup.
  It registers the subsystem command handlers with diag.

@return
  None
*/
void cmlog_init ( void );

#if (defined(FEATURE_LTE_TO_1X) && defined(FEATURE_MMODE_SC_SVLTE)) || defined(FEATURE_MMODE_DYNAMIC_SV_OPERATION_SWITCH)
/*-------- cmlog_csfb_svlte_switch --------*/
/**
@brief
  This function logs the dynamic switch event between CSFB and SVLTE

@param[in] dyn_sw   dynamic switch action type

@return
  None
*/
void cmlog_csfb_svlte_switch(
  cmsoa_sv_switch_action_e_type dyn_sw,
  /*dynamic switch action type*/
  sys_modem_as_id_e_type asubs_id
  /*Subscription ID*/
);
#endif

/*-------- cmlog_enter_emergency_cb --------*/
/**
@brief
  This function should be called when CM enters emergency callback state.
  It will send an event packet to the DM.

@return
  None
*/
void cmlog_enter_emergency_cb (
  sys_modem_as_id_e_type asubs_id
  /*Subscription ID*/
);


/*-------- cmlog_exit_emergency_cb --------*/
/**
@brief
  This function should be called when CM exits emergency callback state.
  It will send an event packet to the DM.

@return
  None
*/
void cmlog_exit_emergency_cb (
  sys_modem_as_id_e_type asubs_id
  /*Subscription ID*/
);


/*-------- cmlog_call_state --------*/
/**
@brief
  This function should be called when call state info changes.
  It will send an event packet to the DM.

@param[in] ss   system selection

@return
  None

@sideeffects
  Local Copy of the call state is updated
*/
void cmlog_call_state (
  sys_modem_as_id_e_type  asubs_id
);

#ifdef FEATURE_MMODE_DUAL_SIM
/*-------- cmlog_get_system_mode_per_subs --------*/
/**
@brief
  Gets the system mode of the specified sub

@param[in] asubs_id   asubs_id to get system mode of

@return
  byte -- the system mode
*/
byte cmlog_get_system_mode_per_subs(
  sys_modem_as_id_e_type      asubs_id
);

/*-------- cmlog_get_call_state_per_subs --------*/
/**
@brief
  Gets the call state of the specified sub

@param[in] asubs_id   asubs_id to get call state of

@return
  byte -- the call state
*/
byte cmlog_get_call_state_per_subs (

  sys_modem_as_id_e_type   asubs_id

);
#endif /*FEATURE_MMODE_DUAL_SIM*/

/*-------- cmlog_plmn_list_event_per_subs --------*/
/**
@brief
  This function sends a diag event when the PLMN NETWORK LIST arrives.

@param[in] plmn_list_ptr   Pointer to a structure containing the list of available PLMNs.
@param[in] ss              system selection

@return
  None
*/
void  cmlog_plmn_list_event_per_subs
(
  const sys_detailed_plmn_list_s_type *plmn_list_ptr,
  /* Pointer to a structure containing the list of available PLMNs.
  */

  sys_modem_as_id_e_type          as_id
);

/*-------- cmlog_plmn_list_event --------*/
/**
@brief
  This function sends a diag event when the PLMN NETWORK LIST arrives.

@param[in] plmn_list_ptr   Pointer to a structure containing the list of available PLMNs.

@return
  None
*/
void  cmlog_plmn_list_event
(
  sys_detailed_plmn_list_s_type *plmn_list_ptr,
  /* Pointer to a structure containing the list of available PLMNs.
  */
  sys_modem_as_id_e_type          asubs_id
  /* Subscription ID
  */
);

/*-------- cmlog_get_call_state_list --------*/
/**
@brief
  Returns pointer to call_state_list static data. This data needs to be used
  in cmdiag module, so the global datum is converted to static and provided
  is through an API.

@return
  cmlog_call_state_s_type* -- Pointer to type cmlog_call_state_s_type
*/
cmlog_call_state_s_type *cmlog_get_call_state_list( void );

/*-------- cmlog_get_diag_client --------*/
/**
@brief
  Returns pointer to diag client global static data defined in cmlog.c

@return
  cmlog_client_s_type* -- Pointer to type cmlog_client_s_type
*/
cmlog_client_s_type *cmlog_get_diag_client( void );



/*-------- cmlog_service_cnf_event --------*/
/**
@brief
  This function sends a diag event when a SERVICE CONFIRMATION arrives.

@param[in] srv_info_ptr   Pointer to the service state info received in the report.
@param[in] ss             Stack on which the service confirmation is recieved

@return
  None
*/
void  cmlog_service_cnf_event
(
  sys_plmn_service_state_s_type   *srv_info_ptr,
  /* Pointer to the service state info received in the report.
  */

  sys_modem_as_id_e_type           as_id
  /* Sub on which the service confirmation is recieved
  */
);

#if defined FEATURE_HDR_HYBRID && defined FEATURE_GPSONE
/*-------- cmlog_block_hdr_orig_during_gps --------*/
/**
@brief
  This function logs the block HDR orig during GPS event

@return
  None
*/
void cmlog_block_hdr_orig_during_gps(
  sys_modem_as_id_e_type           asubs_id
  /* Subscription ID */
);

/*-------- cmlog_allow_hdr_orig_during_gps --------*/
/**
@brief
  This function logs the allow HDR orig during GPS event

@return
  None
*/
void cmlog_allow_hdr_orig_during_gps(
  sys_modem_as_id_e_type           asubs_id
  /* Subscription ID */
);

#endif // defined FEATURE_HDR_HYBRID && defined FEATURE_GPSONE

/*-------- cmlog_call_orig_start --------*/
/**
@brief
  This function logs the call orig start event

@param[in] call_ptr   pointer for the call object

@return
  None
*/
void cmlog_call_orig_start(
  const cmcall_s_type  *call_ptr
  /*  pointer for the call object */
);


/*-------- cmlog_call_orig_redial --------*/
/**
@brief
  This function logs the call orig redial event

@param[in] end_status       the call end status
@param[in] redial_act       the redial action
@param[in] sr_left          number of silent redial left
@param[in] sr_max_redial    maximun number of silent redial attempts

@return
  None
*/
void cmlog_call_orig_redial(
  cm_call_end_e_type    end_status,
  /* the call end status */
  cmcall_act_e_type     redial_act,
  /* the redial action */
  dword                 sr_left,
  /* number of silent redial left */
  unsigned int          sr_max_redial,
  /* maximun number of silent redial attempts */
  sys_modem_as_id_e_type asubs_id
  /* Subscription ID */
);


/*-------- cmlog_call_orig_send_hdr_orig --------*/
/**
@brief
  This function logs the orig sent to HDR event

@return
  None
*/
void cmlog_call_orig_send_hdr_orig(
  sys_modem_as_id_e_type asubs_id
  /* Subscription ID */
);


/*-------- cmlog_call_orig_send_mc_orig --------*/
/**
@brief
  This function logs the orig sent to MC event

@return
  None
*/
void cmlog_call_orig_send_mc_orig(
  sys_modem_as_id_e_type asubs_id
  /* Subscription ID */
);


/*-------- cmlog_call_orig_end --------*/
/**
@brief
  This function logs the call orig end event

@param[in] end_status   call end status

@return
  None
*/
void cmlog_call_orig_end(
  cm_call_end_e_type         end_status,
  /* call end status */
  sys_modem_as_id_e_type     asubs_id
  /*Subscription ID*/
);


/*-------- cmlog_call_orig_connected --------*/
/**
@brief
  This function logs the call orig connected event

@param[in] call_type   origination call type
@param[in] srv_type    origination srv type
@param[in] srv_opt     origination srv opt

@return
  None
*/
void cmlog_call_orig_connected(
  cm_call_type_e_type         call_type,
  /* origination call type */
  cm_srv_type_e_type          srv_type,
  /* origination srv type */
  word                        srv_opt,
  /* origination srv opt */
  sys_modem_as_id_e_type      asubs_id
  /* Subscription ID */
);

/*-------- cmlog_set_cm_offline_signal_recvd --------*/
/**
@brief
  CM calls this after receiving TASK_OFFLINE_SIG signal from TMC.
  This happens some time after cm_ph_cmd_oprt_mode() with
  SYS_OPRT_MODE_OFFLINE_CDMA had been processed by CM.
  Call on log object oprt mode event handler to force sending of DIAG
  EVENT_CM_OPERATIONAL_MODE event now that we got TASK_OFFLINE_SIG
  from TMC.

@return
  None
*/
void  cmlog_set_cm_offline_signal_recvd(void);

#ifdef CM_GW_SUPPORTED

/*-------- cmlog_cm_send_call_information_log --------*/
/**
@brief
  This function sends log packets containing call info.

@return
  None
*/
void cmlog_cm_send_call_information_log(void);

#endif /* CM_GW_SUPPORTED */

/*-------- cmlog_ssac_call_event_handler --------*/
/**
@brief
  This function logs the ssac call event when voice/video over LTE call is made.

@param[in] ssac_call_result   ssac result - accept or reject the call
@param[in] mmtel_type         voice/video over LTE
@param[in] ssac_info          containing cm ssac info, ie. ssac params, uptime, etc

@return
  None
*/
void cmlog_ssac_call_event_handler(

  boolean                     ssac_call_result,
  /* ssac result - accept or reject the call */

  cmcall_ssac_mmtel_e_type    mmtel_type,
  /* voice/video over LTE */

  cmcall_ssac_sub_info_s_type          *ssac_info,
  /* containing cm ssac info, ie. ssac params, uptime, etc */

  sys_modem_as_id_e_type               asubs_id
  /* Subscription ID */

);

/*-------- cmlog_ssac_timer_event_handler --------*/
/**
@brief
  This function logs the ssac timer event such as start/stop/reset ssac timer.

@param[in] timer_action   start/stop/cancel timer
@param[in] mmtel_type     voice/video over LTE
@param[in] timer          backoff timer if timer_action == start.  0 for all the other timer_action values

@return
  None
*/
void cmlog_ssac_timer_event_handler(

  cmcall_ssac_timer_action_e_type    timer_action,
  /* start/stop/cancel timer */

  cmcall_ssac_mmtel_e_type           mmtel_type,
  /* voice/video over LTE */

  uint32                             timer,
  /* backoff timer if timer_action == start.  0 for all the other timer_action values */

  sys_modem_as_id_e_type             asubs_id
  /* Subscription ID */

);

/*-------- cmlog_call_event_report_payload --------*/
/**
@brief
  Generate DIAG events corresponding to given call event

@param[in] call_event      the call event
@param[in] call_info_ptr   call state information

@return
  None
*/
void cmlog_call_event_report_payload(

  cm_call_event_e_type          call_event,

  const cm_mm_call_info_s_type  *call_info_ptr
);

/*===========================================================================
FUNCTION  cmlog_msim_info

DESCRIPTION
   This function logs any changes in MSIM information

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void cmlog_msim_info(

  sys_modem_dual_standby_pref_e_type  user_standby_pref,

  sys_modem_dual_standby_pref_e_type  standby_pref,

  uint8                               active_subs,

  sys_modem_as_id_e_type              default_data_subs,

  sys_modem_as_id_e_type              priority_subs,

  cm_dds_cause_e_type                 dds_reason,

  sys_modem_device_mode_e_type        device_mode

);

/*===========================================================================
FUNCTION  cmlog_plmn_block_req

DESCRIPTION
   This function logs the Event in case the Domain Selection based PLMN Blocking
   for Voice Centric Devices gets triggered

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void cmlog_plmn_block_req(
  uint32         lte_disable_duration_nv,
  /* Value of lte_disable_duration as read from NV */
  uint32         plmn_block_interval,
  /* actual interval for which plmn is blocked */
  dword          t3402_value,
  /* Value of T3402*/
  sys_modem_as_id_e_type     asubs_id
  /* Subscription ID */

);

#endif /* CM_LOG_V_H */
