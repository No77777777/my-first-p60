#ifndef _TM_LPP_CP_H
#define _TM_LPP_CP_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                         TM LPP Utility

GENERAL DESCRIPTION
  This file contains TM's LPP protocol utilities, which convert decoded LPP
  messages for the consumption of TmCore.

  Copyright (c) 2012 Qualcomm Technologies Incorporated.
  Qualcomm Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2015 Qualcomm Atheros, Inc.
  Qualcomm Atheros Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2015 - 2017, 2019 Qualcomm Technologies, Inc. All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/lpp_cp/inc/tm_lpp_cp.h#2 $
  $DateTime: 2020/07/27 23:14:18 $
  $Author: pwbldsvc $

  when        who     what, where, why
  --------    ---     ----------------------------------------------------------
  07/13/20   skm     Ensure we reg/dereg with TX MGR by reading correct Sub's NV for nDDS AGPS session.
  09/27/19    mj      No-Ship support
  02/22/19    mj      Location Privacy Feature
  05/08/17    rn      Added support for Early Response 2-Timer feature.
  02/17/17    skm     BLE and Barometer support for LPPE Phase 2
  01/23/17    kpn     Remove AS ID from Tx packet structures
  03/29/16    kpn     Use of subscription ID while handling NAS DL message for LPP CP
  03/14/15    rh      Added support for MultiSIM NV
  10/27/14    rk      Added LPP RSTD >24 measurement processing support
  09/02/14    gk      cache the tx_mgr_init as a static
  05/04/13    gk      Wait for wishlist resp before sending Req AD
  05/14/13    rk      Removed duplicate gnss time structure. Instead use from tm_common.h
  03/26/13    rk      Resolved Ref Time IE not sent in CP LPP MSB Call flow issue.
===========================================================================*/

#include "gnss_lpp_ecid.h"
#include "mgp_api.h"               /* cgps_CellInfoCachedStruct */
#include "tm_prtl_iface.h"         /* include lte_ml1_ext_api.h for lte_ml1_gps_mrl_cell_meas_update_t */
#include "tm_lpp_tx_mgr.h"         /* tm_lpp_provide_cap_s_type */
#include "sm_api.h"                /* sm_InternalAssistDataStatusStructType */
#include "tm_umts_common_utils.h"  /* for gad_ellip_alt_unc_ellip_type */


/* Min assistance data needed by the engine for UEBased and UEAssited GNSS */
#define LPP_CP_UE_BASED_GPS_MIN_AD          ( C_GNSS_ASSIST_NAVMODL     | \
                                              C_GNSS_ASSIST_REFLOC      | \
                                              C_GNSS_ASSIST_REFTIME     )

#define LPP_CP_UE_BASED_GLO_MIN_AD          ( C_GNSS_ASSIST_NAVMODL     | \
                                              C_GNSS_ASSIST_REFLOC      | \
                                              C_GNSS_ASSIST_TIME_MODELS | \
                                              C_GNSS_ASSIST_AUXI_INFO   )


#define LPP_CP_UE_ASSISTED_GPS_MIN_AD       ( C_GNSS_ASSIST_ACQ         | \
                                              C_GNSS_ASSIST_REFTIME     )

#define LPP_CP_UE_ASSISTED_GLO_MIN_AD       ( C_GNSS_ASSIST_ACQ         | \
                                              C_GNSS_ASSIST_AUXI_INFO   | \
                                              C_GNSS_ASSIST_TIME_MODELS)


/* Max Session Timeout for multi-periodic reporting */
#define C_LPP_CP_TM_MULTI_REPT_MAX_SESSION_TIMEOUT (900) /* 900 seconds: test case on ULTS expects minimum of 900 secs of reporting*/

#define C_LPP_CP_AGNSS_SESS_GPS     (1 << GNSS_ID_Bitmap_gnss_ids_gps)
#define C_LPP_CP_AGNSS_SESS_SBAS    (1 << GNSS_ID_Bitmap_gnss_ids_sbas)
#define C_LPP_CP_AGNSS_SESS_QZSS    (1 << GNSS_ID_Bitmap_gnss_ids_qzss)
#define C_LPP_CP_AGNSS_SESS_GALILEO (1 << GNSS_ID_Bitmap_gnss_ids_galileo)
#define C_LPP_CP_AGNSS_SESS_GLONASS (1 << GNSS_ID_Bitmap_gnss_ids_glonass)
typedef uint32 tm_lpp_cp_session_agps_gnss_id;

typedef struct
{
   uint32    q_gps_session_timeout_value_msecs;
   uint32    q_hor_acc_meters;
} tm_lpp_cp_session_qos_type;

/* LPP CP RLI timer ids. RLI timer ids are created for each incoming LPP RLI transaction. This timer
*  is used to send a response (i.e. LPP PLI) for each LPP RLI. In LPP concurrent transaction
*  scenario more than one RLI timer ids are used from the below definitions.
*/
static uint32 const LPP_CP_RLI_TIMER_IDS[MAX_CONCUR_LPP_RLI_TRANSACTIONS] = {
  (TM_LPP_CP_TIMER_ID_RANGE),
  (TM_LPP_CP_TIMER_ID_RANGE + 1),
  (TM_LPP_CP_TIMER_ID_RANGE + 2),
  (TM_LPP_CP_TIMER_ID_RANGE + 3),
  (TM_LPP_CP_TIMER_ID_RANGE + 4),
  (TM_LPP_CP_TIMER_ID_RANGE + 5),
  (TM_LPP_CP_TIMER_ID_RANGE + 6),
  (TM_LPP_CP_TIMER_ID_RANGE + 7),
  (TM_LPP_CP_TIMER_ID_RANGE + 8),
  (TM_LPP_CP_TIMER_ID_RANGE + 9)
};

/* LPP CP Timers . Todo : clean-up later */
#define LPP_CP_NAS_SESS_TIMER  (TM_LPP_CP_TIMER_ID_RANGE + 12)
#define LPP_CP_AD_TIMER_ID     (TM_LPP_CP_TIMER_ID_RANGE + 14)
#define LPP_CP_OTDOA_MUL_MEAS_TIMER_ID     (TM_LPP_CP_TIMER_ID_RANGE + 15)

/* GNSS_Auxillary timer is needed when certain technologies are requested combinedly. Below are
*  the technology combination needs GNSS_Auxillary timer :-
*
*  1. The request consist of A-GNSS technology along with "OTDOA and/or ECID and/or DBH" type to
*     guard the GNSS engine
*  2. The request is OTDOA + ECID type.
*/
static uint32 const LPP_CP_GNSS_AUXI_TIMER_ID[MAX_LPP_GNSS_AUXI_TIMERS] = {
  (TM_LPP_CP_TIMER_ID_RANGE + 16),
  (TM_LPP_CP_TIMER_ID_RANGE + 17)
};

/* LPP rel-12 supports 2-Timer callflow. LPP RLI's Early Response Timer ID's are created for 
   Individual RLI's with EarlyTimer Qos request .Current implementation supports maximum 2 RLI's 
   with EarlyTimer Qos per CP session. Applicable to "GNSS+OTDOA" or "OTDOA Only" and "MSB" RLI's */
static uint32 const LPP_CP_EARLY_RESPONSE_TIMER_IDS[MAX_CONCUR_LPP_EARLY_TIMER_TRANSACTIONS] = {
  (TM_LPP_CP_TIMER_ID_RANGE + 18),
  (TM_LPP_CP_TIMER_ID_RANGE + 19)
};

typedef enum
{
  /* No Session Started Yet */
  LPP_CP_NONE_S,
  /* Session Started, but no requested yet */
  LPP_CP_STARTED_S         = 0x0001,
  /* Waiting for MGP & other Auxillary tech engine (like OTDOA, WIFI, ECID etc) to produce
     measurements and/or position */
  LPP_CP_GNSS_AUXI_RESP_S   = 0x0002,
  /* Substate to waiting for MGP produce measurements and/or position */
  LPP_CP_GNSS_RESP_SUBS     = 0x0004,
  /* Substate to waiting for OTDOA engine to produce measurements */
  LPP_CP_OTDOA_RESP_SUBS    = 0x0008,
  /* Substate to waiting for LOC_LTE_ECID to produce ECID measurements */
  LPP_CP_ECID_RESP_SUBS     = 0x0010,
  /* Substate to waiting for WIFI (LOWI-Server) produce measurements */
  LPP_CP_WLAN_AP_RESP_SUBS  = 0x0020,
  /* Substate to waiting for best position to be available */
  LPP_CP_BEST_POS_RESP_SUBS  = 0x0040,
  /* Substate to waiting for BTLE measurements */
  LPP_CP_SRN_RESP_SUBS = 0x0080,
  /* Substate to waiting for BARO measurements */
  LPP_CP_BARO_RESP_SUBS = 0x0100,
  /* Substate to wait for Early Response */
  LPP_CP_EARLY_RESP_SUBS = 0x0200,
  /* Waiting for AD to be sent by the SMLC */
  LPP_CP_TIMER_AD_REQ_S     = 0x0400,
  /* When more than 24 OTDOA measurements are available to send, enter this state to
     process/send correctly.*/
  LPP_CP_OTDOA_GRT_24_MEAS_S  = 0x0800
} tm_lpp_cp_state_types;

typedef enum
{
  C_LPP_CP_SESSION_NONE,
  C_LPP_CP_SESSION_NI, /* network-initiated */
  C_LPP_CP_SESSION_UI, /* UE-initiated immediate */
} tm_lpp_cp_callflow_kind_enum_type;


typedef struct
{
   /* See TM_LPP_xxx in tm_lpp_common_utils.h */
   tm_lpp_req_type           q_lpp_flags;
   tm_lpp_error_type        z_lpp_err;
   boolean                   b_status_check_needed;
   uint32                    q_lpp_ecid_req_loc;
   gnss_lpp_ECID_ProvideLocationInformation   z_lpp_ecid_data;
   SmGnssMeasReportStructType                 z_gnss_meas_report;

   /* Hold Auxiliary info AD until AcqAssist is available to process & post the
    * steering info to MGP */
   GNSS_AuxiliaryInformation  *p_gnss_auxi_info_cache;
   /* Store AcqAssist AD until GPS UTCModel or Glonass TimeModel AD is available */
   GNSS_AcquisitionAssistance  z_gnss_acqassist_cache;
} tm_lpp_cp_data_type;

/*
 *  This structure contains all "per-session" LPP CP data with TM-Core.
 *  It is cleared at the end of each session.
 */
typedef struct
{
  struct
  {
    unsigned dbhPresent : 1;
  }m;
   /*  +
    *  PDAPI ID
    */
   pdsm_client_id_type                           q_pdapi_client_id;

   /*  + (tm_common.h)
    *  TM session ID/Handle
    */
   tm_sess_handle_type                           tm_active_session_handle;

   /*  +
    *  This is a copy of the Start Session Request parameters that
    *  were used to start the session, assuming it is an MO session.
    *  This field will be left blank if this is an MT session.
    */
   prtl_start_sess_req_param_s_type              z_start_sess_req_param;

   /*  +
    *  Copy of the loc_info_req_type sent by the SMLC, one of
    *  LPP_locationMeasurementsRequired, MeasurementsPreferred, and
    *  EstimateRequired, EstimatePreferred.
    */
   LPP_LocationInfoType                          u_orig_loc_info_req_type;

   /* #############  OK  #################
    *  Session QoS information
    */
   tm_lpp_cp_session_qos_type                    z_session_qos;

   /*  (tm_umts_common_utils.h)
    *  GPS Assistance Data is encoded into Gsm0931 formatting and
    *  stored in here for use later.
    */
   GpsAssisData_Gsm0931                          z_GpsAssistData;

   GpsAssisData_ganss_Gsm0931                    z_GloAssistData; /* Glonass assist. data wishlist */

   /* buffer to combine Glonass eph. data with the freq. number in aux. info assistance */
   prot_glo_eph_buffer_s_type                    z_prot_glo_eph_buffer[N_GLO_SV];

   /* buffer to combine Glonass Acq. Assistance data with the freq. number in aux. info assistance */
   prot_glo_acqast_buffer_s_type                 z_prot_glo_acqasst_buffer;

   /* Cached (pending) position
      Used when a position is reported to pdapi prior to completion of the
      network call flow
    */
   tm_umts_common_pos_for_net_s_type           z_pending_pos_to_network;

   /* Store the actual fix report structure. Todo : Clean-up other intermediate conversion &
      temporary fix structure. */
   sm_FixInfoStructType *pz_last_fix_rpt;

   /*  (tm_umts_common_utils.h)
    *  Assistance Data is stored here
    */
   tm_network_assist_s_type                      z_network_assist;

   gnss_time_info                             z_set_assist_pos_time_stamp;  // ayt_todo - can this combine with above z_msa_pos_time_stamp ?
                                                                            // ayt_todo - this captures the REF TIME from Prov AD

   /*  +
    *  NI vs. UI
    */
   tm_lpp_cp_callflow_kind_enum_type           e_callflow_kind;

   /*  +
    *  UE-BASED vs UE-ASSISTED
    */
   lpp_pos_mode_type         e_agps_mode; /* Todo : remove later */

   /*  +
    *  Bit map containing a bit mask of GNSS methods requested by the SMLC
    */
   uint8                                         u_needed_gnss_method_bmap;
   /*
    *  This defines the various callflow states for the GSM-CP protocol
    *  sub-module.
    */

   /* LPP CP state machine */
   tm_lpp_cp_state_types  e_cf_state;

   /*
    *  This is a bit mask describing the assistance data pieces that
    *  have been received this session.  It is used in MTMA mode to
    *  determine what wishlist should be requested from the server.
    *  The bit mask is defined by the #defines in sm_api.h which all
    *  start with C_GPS_ASSIS_.
    */
   uint32                                        q_agps_data_rcvd;
   uint32                                        q_aglo_data_rcvd;

   tm_lpp_cp_data_type                           z_lpp_cp_data;

   /*
     *  This is a bit mask describing which almanac and ephemeris data
     *  has been received.  It is used to provide a list of non-existent
     *  or unhealthy SVs to MGP.
     */
   uint32                                        q_alm_sv_list;
   uint32                                        q_eph_sv_list;

   /*
    *  Set to TRUE when a PDAPI fix is reported from MGP.  This is
    *  used later in the callflow to determine if an existing fix can be
    *  returned to the network OR if the callflow state machine must
    *  wait for one to be reported.
    */
   boolean                                       u_pdapi_fix_reported;

   /*
    *  Set to TRUE when an MT callflow requests additional assistance
    *  data (ie. the wishlist) from the network.
    */
   boolean                                       u_wishlist_requested;

   /* Cache the original wish list */
   sm_InternalAssistDataStatusStructType         z_OrigWishList;     // ayt_todo - check if logic is applicable

   /*
    *  Set to TRUE when a session is started with TM-Core.
    */
   boolean                                       u_tm_core_session_started;

   /*  ayt_todo
    *  Set to TRUE when ref location is to be acquired BY FORCE
   boolean                                       u_force_ref_loc;
    */

   /*  +
    *  Set to TRUE when a network session is in progress
    */
   boolean                                       u_network_session_on;

   /* LM has been given the true qos_timer */
   boolean                                       qos_timer_started;

   /*
    *  Set to TRUE when accuracy reset timer expires
    */
   boolean                                       u_accuracy_reset_timer_expired;


   /* ayt_todo
    *  The u_preemption_flag is set when a GSM preemption event is
    *  received.  It is never cleared until the end of the session.  This
    *  flag can be used to alter logic which is dependent on whether or
    *  not there has been at least one preemption in the current session.
   boolean                                       u_preemption_flag;
    */

   /* For Early Response 2-Timer used to indicate if MSB Early Fix timer */
   /* has expired.                                                       */
   boolean                                       v_msb_early_resp_expired;

   /*----------------------------------------------*/
   /* The following is used for Periodic Reporting */
   /*----------------------------------------------*/
   /* from Req Loc Info Msg: when Common IE for periodic reporting is present */
   boolean                                       u_periodic_multi_report;

   /* from Req Loc Info Msg: total number of reports in a multiple-reporting periodic session */
   uint16                                        w_cp_periodic_reports_total_num;

   /* Number of remaining reports in a multi-reporting periodic session.
      Initialized to w_cp_periodic_reports_total_num.
    */
   uint16                                        w_cp_periodic_reports_remaining;

   /* Count how many fix or no-fix reports before the 1st or since last reporting */
   uint16                                        w_lm_report_count;

   /* */
   uint32                                        q_cp_periodic_reporting_interval_sec;

   /* Meas Repsonse time in sec. */
   uint16                                        w_cp_reporting_interval_long;

   /* pd comm begin sent to PDAPI*/
   boolean                                       b_comm_begin_sent;
   /* pd comm begin sent to PDAPI*/
   boolean                                       b_comm_end_sent;

   /*
    * Store the transaction for a AD req sent to SMLC. This will be used for
    * substitute ACK and also to time out pending transactions
    */
   tm_lpp_transaction_info_s_type *p_req_ad_tx;
   /* boolean value tracking if OTDOA AD has been requested in this session */
   boolean                                       b_otdoa_ad_requested;
   /* bit mask tracking if GNSS AD has been requested in this session */
   uint32                                        q_gps_ad_requested;
   /* bit mask tracking if GNSS AD has been requested in this session */
   uint32                                        q_glo_ad_requested;

   /* if AD wishlist request is pending from SM to MGP */
   boolean                                       b_ad_wishlist_req_pending;
   /* if AD request is pending to be sent to server */
   boolean                                       b_req_ad_pending;
   /* RLI technology mask currently accepted and processed by UE */
   rli_tx_tech_mask m_RliTech;
   /* Total RLI timer ids are used */
   uint16 u_RliTimerIdNdx;
   /* Total GNSS Auxillary timer ids are used */
   uint16 u_GnssAuxiTimerIdNdx;
   /* Early timer ids*/
   uint16 u_EarlyTimerIdNdx;
   /* Count of simultaneous earlyRsp Transactions*/
   uint8  s_NumEarlyTxn;
   /* Total number of concurrent transaction open */
   uint8  b_NumConcurrentTx;
   tm_lpp_e_pending_dbh_pos_to_network_s_type  z_DbhPosInfo;  /* DBH position valid when (dbhPresent == 1) */
   /* No need to send any more location notifications for failure because
     one was already sent */
   boolean v_send_no_more_location_notifications;
} tm_lpp_cp_session_status_s_type;


/* LPP Control Plane Protocol Info */
typedef struct {
  tm_lpp_error_type                     z_lpp_err;   /* Used for LPP Error Message */
  uint8    u_transaction_id;            /*  Tx Id being processed                */
  boolean  u_is_duplicate;              /*  TRUE - Found a duplicate LPP message */
  boolean  u_ack_requested;             /*  TRUE - ACK requested                 */
  boolean  u_last_rcvd_seq_num_valid;   /*  Is the Last Received Sequence Number valid ?  */
  uint8    u_last_rcvd_seq_num;         /*  Last Received Sequence Number : 0 .. 255      */
  uint8    u_last_out_seq_num;          /*  Last Outgoing Sequence Number : 0..255        */
  boolean  u_last_out_seq_num_valid;    /*  Is the Last Outgoing Sequence Number valid ?  */

  boolean b_wait_for_ack;
} tm_lpp_cp_info_s_type;

extern tm_lpp_provide_cap_s_type   z_lpp_cp_provide_cap;  /* tm_lpp_provide_cap_s_type in tm_lpp_tx_mgr.h */
extern boolean                     b_ota_reset_flag;      /* TRUE if an OTA reset is rcvd, indicating test mode */

/*===========================================================================

FUNCTION tm_lpp_cp_init

DESCRIPTION
  This function initializes LPP Control Plane Protocol Module.

DEPENDENCIES: none.

RETURN VALUE: none.

SIDE EFFECTS: none.

===========================================================================*/
void tm_lpp_cp_init( void );


/*===========================================================================

FUNCTION tm_lpp_cp_error_cb

DESCRIPTION
  This function is the LPP Control Plane Protocol error callback to handle
  LPP Reliable Transport errors for transmition failures, etc.

DEPENDENCIES: none.

RETURN VALUE: none.

SIDE EFFECTS: none.

===========================================================================*/
void tm_lpp_cp_error_cb(
                         int32  error,      /* error number defined above */
                         uint32 nas_id,     /* NAS Routing ID for associated outgoing LPP packet */
                         uint32 trans_id,   /* Transaction ID for associated outgoing LPP packet */
                         uint8  seq_num     /* Sequence Number for associated outgoing LPP packet */
                        );


/********************************************************************
*
* tm_lpp_cp_lte_cell_db_update
*
* Function description:
*   Processing cell db update from MGP for LTE or OOS.
*
* Parameters:
*
* Return:
*  None
*
*********************************************************************
*/
void tm_lpp_cp_lte_cell_db_update( const cgps_CellInfoCachedStruct *cell_db_data_ptr );


/********************************************************************
 *
 * tm_lpp_cp_pa_proc
 *
 * Function description:
 *  Processes PA events from TM Core.
 *
 * Parameters:
 *
 * Return:
 *  None.
 *
 *********************************************************************
 */
void tm_lpp_cp_pa_proc( prtl_pa_event_s_type *p_pa_event  );

/*===========================================================================
FUNCTION tm_lpp_cp_wait_for_ack

DESCRIPTION
  This function updates the flag in TM LPP CP INFO on ACK is received

PARAMETERS:

DEPENDENCIES: none.

RETURN VALUE: none.

SIDE EFFECTS: none.

===========================================================================*/
void tm_lpp_cp_wait_for_ack( boolean u_ack_rcvd );

/********************************************************************
 *
 * tm_lpp_cp_get_nxt_out_seq_no
 *
 * Function description:
 *   This function gets the next outgoing sequence number for a LPP Message.
 *
 * Parameters:
 *
 * Return:
 *  None.
 *
 *********************************************************************
 */
void tm_lpp_cp_get_nxt_out_seq_no( tm_lpp_transaction_info_s_type *p_lpp_tx_info_type );

/*===========================================================================

FUNCTION tm_lpp_cp_handle_nofix

DESCRIPTION handles TmCore provided no-fix info: only used for multiple
  periodic reporting.

DEPENDENCIES
  None.

RETURN VALUE
  none.

SIDE EFFECTS
  None.

===========================================================================*/
void tm_lpp_cp_handle_nofix( void );

/*===========================================================================

FUNCTION tm_lpp_cp_handle_lr_data

DESCRIPTION handles TmCore provided LR data.

DEPENDENCIES
  None.

RETURN VALUE
  none.

SIDE EFFECTS
  None.

===========================================================================*/
void tm_lpp_cp_handle_lr_data( sm_FixInfoStructType *lr_data_ptr, boolean u_final );


/*===========================================================================

FUNCTION tm_lpp_cp_handle_prm_data

DESCRIPTION handles TmCore provided PRM data.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tm_lpp_cp_handle_prm_data( sm_GpsMeasRptStructType const * const p_prm_data );

/*===========================================================================
FUNCTION tm_lpp_cp_handle_otdoa_grt_24_meas_timer_expiry

DESCRIPTION
  This function handles Multi-OTDOA Timer expiry. Remove the transaction for the
  AD req from tx manager. If no more pending transactions, start a
  NAS_SESS_TIMER for 5 seconds.

PARAMETERS:

DEPENDENCIES: none.

RETURN VALUE: none.

SIDE EFFECTS: none.

===========================================================================*/

void tm_lpp_cp_handle_otdoa_grt_24_meas_timer_expiry();

/*===========================================================================
FUNCTION tm_lpp_cp_handle_ad_timer_expiry

DESCRIPTION
  This function handles AD Timer expiry. Remove the transaction for the
  AD req from tx manager. If no more pending transactions, start a
  NAS_SESS_TIMER for 5 seconds.

PARAMETERS:

DEPENDENCIES: none.

RETURN VALUE: none.

SIDE EFFECTS: none.

===========================================================================*/

void tm_lpp_cp_handle_ad_timer_expiry( void );

/********************************************************************
 *
 * tm_lpp_cp_handle_nas_sess_timer_expiry
 *
 * Function description:
 *   This is LPP Control Plane Event handler which handles timer expiry
 *   for NAS session timer.
 *
 * Parameters:
 *
 * Return:
 *  None.
 *
 *********************************************************************
 */

void tm_lpp_cp_handle_nas_sess_timer_expiry( void );
/********************************************************************
 *
 * tm_lpp_cp_events
 *
 * Function description:
 *   This is LPP Control Plane Event handler which serves as the
 *   entry point of processing incoming LPP Control Plane Messages.
 *
 * Parameters:
 *
 * Return:
 *  None.
 *
 *********************************************************************
 */
void tm_lpp_cp_events( prtl_lpp_cp_event_s_type *lpp_cp_event_info );

/*
******************************************************************************
* tm_lpp_cp_handle_otdoa_data
*
* Function description:
*
*   Handle OTDOA data
*
* Parameters: None.
*
* Return value: None.
*
******************************************************************************
*/
void tm_lpp_cp_handle_otdoa_data( sm_loc_lte_otdoa_meas_info_type const *p_OtdoaData );


/*
******************************************************************************
* tm_lpp_cp_handle_ecid_data
*
* Function description:
*
*   Handle OTDOA data
*
* Parameters: None.
*
* Return value: None.
*
******************************************************************************
*/
void tm_lpp_cp_handle_ecid_data( gnss_lpp_ECID_ProvideLocationInformation *p_otdoa_data );

/*===========================================================================
FUNCTION tm_lpp_cp_process_ack

DESCRIPTION
  This function process Server Ack (implicit or explicit).

PARAMETERS:

DEPENDENCIES: none.

RETURN VALUE: none.

SIDE EFFECTS: none.

===========================================================================*/
boolean tm_lpp_cp_process_ack( LPP_Message                     *p_pdu_decoded_ptr,
                               tm_lpp_transaction_info_s_type **p_lpp_tx_type,
                               tm_lpp_error_type               *p_lpp_error,
                               boolean                         *p_early_exit );


/*===========================================================================

FUNCTION tm_lpp_cp_inform_tm_core_stop_session

DESCRIPTION
  This function informs the TM core that the current LPP CP session should
  terminate.

DEPENDENCIES: none.

RETURN VALUE: none.

SIDE EFFECTS: none.

===========================================================================*/
void tm_lpp_cp_inform_tm_core_stop_session( tm_sess_stop_reason_e_type stop_reason );


/*
 ******************************************************************************
 * tm_lpp_cp_send_lpp_abort
 *
 * Function description:
 *  Sends LPP ABORT for all outstanding transactions
 *
 * Parameters:
 *  vc_req_ack - Indicate if the LPP Abort should request for ACK or not.
 *
 * Return value:
 *      TRUE - if successfully sent, FALSE otherwise
 ******************************************************************************
*/
boolean tm_lpp_cp_send_lpp_abort( boolean const vc_req_ack );


/********************************************************************
 *
 * tm_lpp_cp_emergency_exit
 *
 * Function description:
 *   This function handles LPP Control Plane periodic session after an emergency
 *   call ends
 *
 * Parameters:
 *
 * Return:
 *  None.
 *
 *********************************************************************
 */
void tm_lpp_cp_emergency_exit( void );

/********************************************************************
 *
 * tm_lpp_cp_send_req_ad
 *
 * Function description:
 *   This function sends a request for AD.
 *
 * Parameters:
 *
 * Return:
 *  None.
 *
 *********************************************************************
 */
void tm_lpp_cp_send_req_ad( void );

/********************************************************************
 *
 * tm_lpp_cp_config_change_proc
 *
 * Function description:
 * This function processes change to the LPP COnfig value via QMI/NV
 *
 * Parameters:
 *
 * Return:
 *  None.
 *
 *********************************************************************
 */
void tm_lpp_cp_config_change_proc(pdsm_pa_lpp_config_info_type lpp_config_info);


void tm_lpp_cp_handle_best_pos( sm_FixReportStructType const *const cpz_pos_report );

void tm_lpp_cp_handle_rli_timer_expiry( os_TimerExpiryType const *const p_timer_param );

void tm_lpp_cp_handle_gnss_auxi_timer_expiry( os_TimerExpiryType const *const p_timer_param );

void tm_lpp_cp_handle_lm_timeout( tm_sess_stop_reason_e_type const cv_StopReason,
                                  pdsm_pd_end_e_type const ce_PdEndEvent );

void lpp_cp_handle_early_response_timer_expiry( tm_lpp_transaction_info_s_type *pz_lpp_tx_item );

extern boolean tm_lpp_cp_get_as_id(sys_modem_as_id_e_type *p_as_id);


/********************************************************************
 *
 * tm_lpp_cp_refresh_efs_nv
 *
 * Function description:
 * This function handles the refresh of EFS NV items.
 *
 * Parameters:
 *  v_hwRead : TRUE, Read NV from Hardware
 *             FALSE, Read NV from Cache
 *
 * Return:
 *  None.
 *
 *********************************************************************
 */
void tm_lpp_cp_refresh_efs_nv(boolean v_hwRead );

#endif
