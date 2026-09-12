#ifndef _TM_LPP_TX_MGR_H
#define _TM_LPP_TX_MGR_H
/*===========================================================================
  
                            TM_LPP_TX_MGR.H  
                   
DESCRIPTION

  
  Copyright (c) 2011 - 2012 Qualcomm Technologies Incorporated.
  Qualcomm Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2015 Qualcomm Atheros, Inc.
  Qualcomm Atheros Confidential and Proprietary. All Rights Reserved. 
  Copyright (c) 2016 - 2018 Qualcomm Technologies, Inc.
  Qualcomm Technologies Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2020 Qualcomm Technologies, Inc. 
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/lpp_common/inc/tm_lpp_tx_mgr.h#2 $
  $DateTime: 2020/09/04 13:58:08 $
  $Author: pwbldsvc $

  when        who     what, where, why
  --------    ---     ----------------------------------------------------------
  03/06/20   jv      Added function to check if asn1 mem ctxt reset is allowed
  04/11/18   sj      Fix bad DBH Punc in LPPe
  10/19/17   rk      Added option in tx item to inform ACK request is needed.
  08/31/17   py      Remove sub id from tm_lpp_tx_mgr_handle_encode_transaction()
  03/22/17   rk      Remove tx that protocols considers to be delete in the end of handling the
                     incoming message.
  02/17/17   skm     BLE and Barometer support for LPPE Phase 2
  01/23/17   kpn     Remove Sub id Tx packet structures
  11/16/16   gk      Velocity flags not correctly stored
  11/04/16   kpn     Add sub id to tm_lpp_tx_mgr_handle_encode_transaction()
  07/28/16   skm     2Timer re-arc for LPPe includes CR's(985835,1034212,1034213,1034216,1034219) 
  03/29/16   kpn     Use of subscription ID while handling NAS DL message for LPP CP
  02/12/15   gk      Ack processing incorrect for seq #0
  10/27/14   rk      Added LPP RSTD >24 measurement processing support
  03/05/14   sj      Fixed LPP CP Fuzz crash  
  11/03/11   gk      Initial Checkin
===========================================================================*/

#include "tm_asn1_common.h"
#include "tm_umts_common_utils.h"
#include "tm_lpp_asn1.h"
#include "tm_lppe_asn1_enc.h"
#include "tm_lppe_asn1_dec.h"
#include "tm_auxtech_iface.h"

typedef enum
{
  TM_LPP_SERVER_ACK_NONE,
  TM_LPP_SERVER_ACK_EXPLICIT,
  TM_LPP_SERVER_ACK_IMPLICIT
}tm_lpp_ack_e_type;


#define TM_LPP_TX_MGR_CLIENT_MAX 2
#define TM_LPP_TX_MGR_CLIENT_TYPE_NULL  0
#define TM_LPP_TX_MGR_CLIENT_TYPE_SUPL  1
#define TM_LPP_TX_MGR_CLIENT_TYPE_CP    2
#define TM_LPP_TX_MGR_TRANSACTIONS_MAX 10

/* Maximum number of concurrent LPP RLI transaction to be supported */
#define MAX_CONCUR_LPP_RLI_TRANSACTIONS 10
/*Maximum number of concurrent LPP EarlyTimer transactions supported 
  inside a session*/
#define MAX_CONCUR_LPP_EARLY_TIMER_TRANSACTIONS 2
/* Maximum number of GNSS Auxillary timers needed. */
#define MAX_LPP_GNSS_AUXI_TIMERS 2


/* LPP has received from the Engine valid */
typedef uint32 tm_lpp_req_type;
#define TM_LPP_AD_REQUESTED                          0x00000001 /* LPP CP has requested Assitance Data from the engine */
#define TM_LPP_AD_RECVD                              0x00000002 /* LPP CP has received Assistance Data from the engine */
#define TM_LPP_OTDOA_DELIVERED                       0x00000004 /* OTDOA assistance data delivered */
#define TM_LPP_MAY_RET_ADD_INFO                      0x00000008
#define TM_LPP_MAY_REQ_ADD_OTDOA_AD                  0x00000010 /* UE may request additional OTDOA AD from the server */
#define TM_LPP_REQ_LOC_INFO_RCVD                     0x00000020 /* LPP CP has received IE Req Loc Info from E-SMLC */
/* Location Information IE from SMLC contains req for */
#define TM_LPP_REQ_LOC_INFO_POS_EST_RCVD             0x00000040 /* Position Estimate from UE (i.e. MSB) */
#define TM_LPP_REQ_LOC_INFO_GNSS_MEAS_RCVD           0x00000080 /* Measurements, i.e. position computed by E-SMLC, MSA*/
#define TM_LPP_REQ_LOC_INFO_OTDOA_RCVD               0x00000100 /* OTDOA measurements */
#define TM_LPP_REQ_LOC_INFO_ECID_RCVD                0x00000200 /* ECID measurements */
#define TM_LPP_REQ_LOC_INFO_VEL_REQUESTED            0x00000400 /* Velocity requested */
#define TM_LPP_MAY_REQ_ADD_GNSS_AD                   0x00000800 /* UE may request additional GNSS AD from the server */

typedef enum
{
  TM_LPP_MESG_NONE,
  TM_LPP_MESG_REQ_CAP,
  TM_LPP_MESG_PROV_CAP,
  TM_LPP_MESG_REQ_AD,
  TM_LPP_MESG_PROV_AD,
  TM_LPP_MESG_REQ_LOC_INFO,
  TM_LPP_MESG_PROV_LOC_INFO,
  TM_LPP_MESG_ABORT,
  TM_LPP_MESG_ERROR,
  TM_LPP_MESG_LAST = TM_LPP_MESG_ERROR
}tm_lpp_message_e_type;

typedef enum
{
  TM_LPP_ENCODE_TX,
  TM_LPP_DECODE_TX
} tm_lpp_handle_tx_e_type;

typedef enum
{
  TM_LPP_GET_CLIENT_CAP,
  TM_LPP_GET_CLIENT_AD_LIST,
  TM_LPP_PROV_CLIENT_AD,
  TM_LPP_REQ_CLIENT_LOC_INFO,
  TM_LPP_PROV_LOC_INFO,
  TM_LPP_REQ_CLIENT_CAP ,
  TM_LPP_GET_ERROR_INFO,
  TM_LPP_GET_ABORT_INFO,
  TM_LPP_HANDLE_ERROR,
  TM_LPP_HANDLE_ABORT
} tm_lpp_tx_mgr_client_cmd_e_type;

typedef enum
{
  TM_LPP_TRANS_ID_INIT_SERVER,
  TM_LPP_TRANS_ID_INIT_SET
} tm_lpp_trans_id_e_type;

/* Structure for PD COMM transaction id info */
typedef struct 
{
   struct 
   {
     unsigned u_trans_id_valid : 1;   /* if transaction id is valid. server can send a tx wih no txid.*/
   } m;
  uint8  u_transaction_id;            /* valid value is 0-255 */
  tm_lpp_trans_id_e_type u_initiator; /* if the id is initiated by set or server */
} tm_lpp_transaction_id_s_type;



tm_lpp_transaction_id_s_type z_lpp_transaction_id;

typedef struct 
{
  uint8 u_pos_mode_supported;
} tm_lpp_agnss_cap_s_type;


typedef struct 
{
  uint8 u_dbh_supported;
  uint8 u_wifi_supported;
  uint8 u_bt_supported;
  uint8 u_baro_supported;
} tm_lppe_cap_s_type;

/* flags for Provide Capability */
typedef uint32 tm_lpp_prov_cap_types;
#define LPP_PROV_AGNSS_CAP  0x0001
#define LPP_PROV_OTDOA_CAP  0x0002
#define LPP_PROV_ECID_CAP   0x0004
#define LPP_PROV_EPDU_CAP   0x0008
#define LPP_PROV_HA3DPOS_CAP   0x0100
#define LPP_PROV_WLAN_AP_CAP   0x0200
#define LPP_PROV_SRN_CAP       0x0400
#define LPP_PROV_BARO_CAP      0x0800
#define LPP_PROV_REL15_IE_CAP     0x1000

typedef struct 
{
  tm_lpp_prov_cap_types q_flags; /* Provide capabilities flag */
  tm_lpp_agnss_cap_s_type z_agnss_capabilities;
  boolean u_otdoa_supported;
  boolean u_glonass_supported;
  boolean u_ecid_supported;
} tm_lpp_provide_cap_s_type;

#define LPP_PROV_CAP_VALID 0x0001
#define LPP_CELL_INFO_VALID 0x0004
#define LPP_PHYS_CELL_ID_VALID 0x0008
#define LPP_PRM_DATA_VALID 0x0020
#define LPP_POS_DATA_VALID 0x0040
#define LPP_ERROR_DATA_VALID 0x0080
#define LPP_REQ_LOC_TYPE_VALID 0x0100
#define LPP_ENC_PRM_DATA 0x0200
#define LPP_ENC_OTDOA_DATA 0x0400
#define LPP_ENC_POS_DATA 0x0800
#define LPP_ENC_PROV_LOC_PERIODIC_RPT        0x1000  /* 1st to n-1 periodic report */
#define LPP_ENC_PROV_LOC_PERIODIC_RPT_FINAL  0x2000  /* last periodic report       */
#define LPP_ENC_ECID_DATA 0x4000
#define LPP_ECID_DATA_VALID 0x8000

typedef struct
{
  uint32    q_fix_time_gps_msec; /* GPS time: ms in week */
  uint32    q_fix_time_utc_msec; /* time adjusted for leap seconds */
  uint16    w_GpsWeek;           /* fix week number in GPS time */
  uint32    q_pos_sources;       /* indicate GPS/GLONASS etc */

  float     f_ClockBias;         /* Receiver's clock bias (m) */
  float     f_ClockBiasUncMs;    /* Receiver's clock uncertainty (m) */
  gad_ellip_alt_unc_ellip_type z_pos; /* all data except lat/long/alt are used from this struct
                                         below data w_Alt, q_Lat and l_Long are used for alt/lat/long */
  boolean   vel_valid;
  float     f_VelEnu[3];         /* East, North, Up velocity (m/s) */
  float     f_Vunc3dMps;
  float     f_velVertUnc;

  /* GAD9 values, but LPP needs them in different format than what RRLP uses, hence saving it here */
  uint16    w_Alt;
  uint32    q_Lat;
  int32     l_Long;

  uint32    q_GloTodMs; /* Glonass time of day (milliseconds). 0..86399999.
                           Valid only when q_pos_sources is Glonass only (C_POS_SOURCE_GLO) */
} tm_lpp_pending_pos_to_network_s_type;

typedef struct
{
  uint16    w_gps_week; /* GPS week number*/
  uint32    q_fix_time_gps_msec; /* GPS time: ms in week */
  uint32    q_fix_time_utc_msec; /* time adjusted for leap seconds */
  sm_NavPosFlags z_SmNavSolutionFlags;

  /* values sent by ALE in sm_GnssFixRptStructType type */
  DBL    d_PosLla[3];           /* Lat, Long, Alt */
  uint8  u_conf;                /* Conf level for error ellipse. Currently, this is 38% */
  FLT    f_ErrorEllipse[3];     /* loc_uncrtnty_ang (degree), loc_uncrtnty_a (m), loc_uncrtnty_p (m) */
  FLT    f_PuncVertMeters;
} tm_lpp_e_pending_dbh_pos_to_network_s_type;


#define LPP_ERROR_NO_ERR                     0x00
#define LPP_ERROR_TYPE_LPP_ERROR             0x01
#define LPP_ERROR_TYPE_PROV_LOC_COMMON_ERROR 0x02
#define LPP_ERROR_TYPE_PROV_LOC_AGNSS_ERROR  0x04
#define LPP_ERROR_TYPE_PROV_LOC_OTDOA_ERROR  0x08
#define LPP_ERROR_TYPE_PROV_LOC_ECID_ERROR   0x10
#define LPP_ERROR_TYPE_PROV_LOC_EPDU_ERROR   0x20
#define LPP_ERROR_TYPE_LPP_ABORT             0x40
#define LPP_ERROR_TYPE_IGNORE_LPP_TRANS      0x80 /* Ignore/drop the LPP transaction item */

typedef struct
{
  struct {
    unsigned s_AgnssReqCap : 1; /* AGNSS Capability is requested in LPP ReqCap */
    unsigned s_OtdoaReqCap : 1; /* OTDOA Capability is requested in LPP ReqCap */
    unsigned s_EcidReqCap : 1; /* ECID Capability is requested in LPP ReqCap */
    unsigned s_EpduReqCap : 1; /* EPDU Capability is requested in LPP ReqCap */
    unsigned s_WlanApReqCap : 1; /* WLAN_AP Capability is requested in LPPe ReqCap */
    unsigned s_Ha3dposReqCap : 1; /* HA3DPOS Capability is requested in LPPe ReqCap */
    unsigned s_SrnBleReqCap : 1;  /* SRN BLE Capability is requested in LPPe ReqCap */
    unsigned s_SensorBaroReqCap : 1;  /* Sensor Baro Capability is requested in LPPe ReqCap */
    unsigned s_Rel15WlanApReqCap : 1;  /* Rel-13/15 WLAN Capability is requested in LPP ReqCap */
    unsigned s_Rel15SrnBleReqCap : 1;  /* Rel-13/15 BTLE Capability is requested in LPP ReqCap */
    unsigned s_Rel15SensorBaroReqCap : 1; /* Rel-13/15 Sensor Capability is requested in LPP ReqCap */
    unsigned s_Rel15TbsReqCap : 1;  /* Rel-13/15 TBS capability is requested in LPP ReqCap */
  }m;
} tm_lpp_req_cap_s_type;

typedef uint32 rli_wlan_ap_meas_mask;
#define C_LPP_WLAN_AP_MEAS_RSSI             0x01
#define C_LPP_WLAN_AP_MEAS_NONSERVING       0x02
#define C_LPP_WLAN_AP_MEAS_HISTORIC         0x04

typedef uint32 lpp_pos_mode_type;
#define  C_LPP_AGPS_MODE_UNDEF       0x00
#define  C_LPP_AGPS_MODE_UE_BASED    0x01
#define  C_LPP_AGPS_MODE_UE_ASSISTED 0x02 /* A-GNSS UEA positioning mode*/
#define  C_LPP_AGPS_NO_POSITION      0x04
#define  C_LPP_AGPS_MODE_STANDALONE  0x08
#define  C_LPP_MODE_UE_OTDOA         0x10 /* OTDOA positiong mode. For OTDOA only pos mode this bit is set & rest are zero. */
#define  C_LPP_MODE_UE_ECID          0x20

/*
 * Technology mask of the request location information sent by the server.
 */
typedef uint32 rli_tx_tech_mask;
#define  C_LPP_TECH_GNSS_MSB         0x01
#define  C_LPP_TECH_GNSS_MSA         0x02
#define  C_LPP_TECH_OTDOA_MSA        0x04
#define  C_LPP_TECH_ECID_MEAS        0x08
#define  C_LPP_TECH_HA3DPOS          0x10
#define  C_LPP_TECH_WLAN_AP_MEAS     0x20
#define  C_LPP_TECH_BARO_MEAS        0x40
#define  C_LPP_TECH_SRN_MEAS         0x80

/*
 * Special callflows mask of the request location information sent by the server.
 */
typedef uint8 rli_tx_lpp_ext;
#define C_LPP_EXT_UNDEF              0x0
#define C_LPP_EXT_EARLY_RESP         0x1

typedef struct
{
  rli_tx_tech_mask  m_tech;
  uint32            q_response_time_msec;         /* UE's LPP PLI reponse time in milli seconds */
  uint32            u_early_response_time_msec;   /* UE's LPP PLI Early response time in milli seconds*/
  uint8             s_gnss_method;     /* Store server choice of gnss_Methods (GPS, Glonass) type from
                                                      LPP ReqLocInfo. Uses C_POS_SOURCE_XXX bit positions to
                                                      set/unset gnss methods */
  /* GNSS or Auxillary timer. Used only when GNSS and/or OTODA engine is involved */
  os_TimerMsgType*    pz_gnss_auxi_timer;
  os_TimerMsgType*    pz_rli_timer;               /* RLI timer. Upon RLI timeout sent the LPP PLI */
  os_TimerMsgType*    pz_earlyRsp_fetch_timer;    /* EarlyFixTimer. Upon time out fetch and send LPP PLI if Meas/POS available*/
  sm_loc_lte_otdoa_meas_info_type *p_otoda_data;  /* Pointer to main OTDOA data struct contain RSTD measurements.
                                                   If this pointer is not NULL, then OTDOA data is VALID. */
  rli_tx_lpp_ext           s_lpp_ext;             /*Bitmap to store special LPP callflow type. ex. earlytimer*/
} tm_lpp_rli_type;

/* LPP technology errors */
typedef struct
{
  uint16 w_ErrType;    /* Use LPP_ERROR_TYPE_XXX macro to set/clear the error types */
  uint8 u_CommonErrCode; /* LPP PLI Common error. u_ErrType == LPP_ERROR_TYPE_PROV_LOC_COMMON_ERROR */
  uint8 u_GnssErrCode;   /* LPP PLI GNSS error. u_ErrType == LPP_ERROR_TYPE_PROV_LOC_AGNSS_ERROR */
  uint8 u_OtdoaErrCode;  /* LPP PLI OTDOA error. u_ErrType ==  LPP_ERROR_TYPE_PROV_LOC_OTDOA_ERROR */
  uint8 u_EcidErrCode;   /* LPP PLI ECID error. u_ErrType == LPP_ERROR_TYPE_PROV_LOC_ECID_ERROR */
  uint8 u_ErrorErrCode;   /* LPP Error error. u_ErrType == LPP_ERROR_TYPE_LPP_ERROR */
  uint8 u_AbortErrCode;   /* LPP Abort error. u_ErrType == LPP_ERROR_TYPE_LPP_ABORT */
  uint8 u_EpduErrCode;   /* LPP PLI EPDU error. u_ErrType == LPP_ERROR_TYPE_PROV_LOC_EPDU_ERROR */
} tm_lpp_error_type;

typedef struct
{
  uint32 q_flags; /* validity flags for the data below (e.g. LPP_POS_DATA_VALID) */
  /* Flags to indicate the kind and different type of server request. (e.g
     TM_LPP_REQ_LOC_INFO_RCVD etc) */
  tm_lpp_req_type m_req_flags;
  tm_lpp_req_cap_s_type                 z_lpp_req_cap;
  tm_lpp_provide_cap_s_type *p_lpp_prov_cap_type;

  /* gps assistance data. data is copied from a static buffer. no dynamic memory allocation */
  sm_InternalAssistDataStatusStructType *p_AssistData;
  boolean b_otdoa_needed;
  uint16 w_phys_cell_id;
  cgps_CellInfoCachedStruct z_cached_cell_info;

  /* GNSS measurement report. data is copied from a static buffer. no dynamic memory allocation  */
  SmGnssMeasReportStructType *p_gnss_meas_report;

  tm_lpp_pending_pos_to_network_s_type  z_pos_info;

  /* HSLP requires the following measurements from the UE */
  rli_wlan_ap_meas_mask q_wifi_ap_meas_to_send;

#ifdef FEATURE_GNSS_LPPE
  /* Pointer to WLAN AP Measurements. If this pointer is not NULL, then WiFi data is VALID.
     data is copied from a static buffer. no dynamic memory allocation */
  tm_lppe_WifiInfoListType *p_wifi_ap_meas;
  tm_lppe_SrnInfoListType  *pz_srn_ap_meas;
  tm_lppe_sensorInfoType   *pz_sensor_meas;
#endif

  /* Pointer to High Accuracy 3d Position. If this pointer is not NULL, then the position data is
     VALID. data is copied from a static buffer. no dynamic memory allocation */
  tm_lpp_e_pending_dbh_pos_to_network_s_type *p_dbh_pos_info;

  tm_pdu_buf_type  z_pdu_buf;                   /* ASN.1 encoded LPP_Message */
  tm_pdu_buf_type  z_epdu_buf;    /* ASN.1 encoded LPP EPDU Message */

  /* If epdu is not present or could not be decoded, p_epdu_ptr will be NULL, Non Null otherwise */
  LPP_Message *p_pdu_ptr;                       /* OSYS memory for decoded LPP_Message */
#ifdef FEATURE_GNSS_LPPE
  tm_encode_OMA_LPPe_MessageExtension   *p_encode_epdu_ptr; /* OSYS memory for decoded LPPe EPDU */
  tm_decode_OMA_LPPe_MessageExtension   *p_decode_epdu_ptr; /* OSYS memory for decoded LPPe EPDU */
#endif


  /***************** Pointers below used only for LPP UP!!! *****************/
  GNSS_AuxiliaryInformation *p_gnss_auxi_info;        /* Hold Auxiliary info AD until AcqAssist is available to process
                                                         & post the steering info to MGP */
  GNSS_AcquisitionAssistance z_gnss_acqassist;  /* Store AcqAssist AD until GPS UTCModel or Glonass TimeModel AD
                                                         is available */
  /***************** Pointers below used only for LPP UP!!! *****************/
  /* Data copied from a static buffer. not dynamic memory allocation */
  gnss_lpp_ECID_ProvideLocationInformation *p_ecid_data;

  tm_lpp_rli_type z_rli;
  tm_lpp_error_type z_lpp_err;

} tm_lpp_data_type;

#define TM_LPP_SESSION_TYPE_UP (0)
#define TM_LPP_SESSION_TYPE_CP (1)

typedef enum
{
  TM_LPP_ACK_RSP_NONE,
  TM_LPP_ACK_RSP_EXPLICIT,
  TM_LPP_ACK_RSP_IMPLICIT
} tm_lpp_ack_rsp_type;

typedef struct
{
  struct {
    /* Mark this transaction for deletion from Tx manager */
    uint64 s_delete_tx : 1; /* 1 - Delete this tx from tx manager in the end */
    /* For LPP CP use only */
    uint64 s_no_ack : 1; /* 1 - Do not request for ACK in the outgoing LPP message */
    uint64 : 62;  /* Padding to align with 8 byte boundary */
  }m;

  uint8 u_client_handle;
  boolean b_in_use; /* transaction info in use*/
  tm_lpp_transaction_id_s_type z_current_trans_id; /* transaction id */
  boolean b_new_tx;           /* Set to false if a duplicate transaction is received from server.
                                 No new transaction is created for the duplicate transaction */
  tm_lpp_message_e_type        e_messages_id; /* messages type. */
  tm_lpp_message_e_type        e_next_messages_id; /* messages type. */
  uint8                        u_lpp_session_type;/* TM_LPP_SESSION_TYPE_UP (CP) */
  void                *p_session_info_ptr; /* SUPL session pointer */
  boolean             b_abnomal_transaction; /* if this transaction has abnormal/error condition */
  boolean             b_end_sent; /* if end is sent for this transaction */
  boolean             b_unsolicited; /* if this is an unsolicited transaction and no resp is reqd */
  boolean             b_pending_ue_tx; 
  /* this value is set to TRUE if we have to still send a response to server  */
  boolean             b_pending_server_tx; 
  /* this value is set to TRUE if we have to still recv response from server  */
  uint8               u_pos_mode; /* Positioning mode for the session */
  tm_lpp_data_type    z_lpp_data_type; /* lpp_data */

  /* for LPP CP use only */  
  uint8                        u_sequence_num;              /* incoming sequence number */
  uint8                        u_out_sequence_num;          /* outgoing sequence number */
  uint8                        u_wait_ack;                  /* bit 0 - Wait server ACK */
  uint32                       nas_session_id;              /* NAS ID */
  boolean                      b_in_sequence_num_valid;    /* if incoming seq # is valid */
  boolean                      b_out_sequence_num_valid;    /* if outgoing seq # is valid */
  uint32                       q_timestamp;                 /* Timestamp of the transaction */
} tm_lpp_transaction_info_s_type;
                  
typedef struct tm_lpp_transaction_list_s
{
  tm_lpp_transaction_info_s_type     elem;
  struct tm_lpp_transaction_list_s  *next;  /* next tm_lpp_transaction_list_s_type in the transaction list */
} tm_lpp_transaction_list_s_type;

typedef struct tm_lpp_transaction_list_summary_s
{
  uint8                           num_trans;
  tm_lpp_transaction_list_s_type  *head;
} tm_lpp_transaction_list_summary_s_type;


/* Dynamic linked list of outstanding LPP transaction for a given LPP Session */  
extern tm_lpp_transaction_list_summary_s_type   tm_lpp_tx_summary;
/* client call back function that can be called by the LPP transaction manager once LPP transaction      
     mananger is done with this transaction 
*/
typedef boolean (tm_lpp_tx_mgr_client_cb_func) (

    tm_lpp_tx_mgr_client_cmd_e_type  e_cmd_type,

    tm_lpp_transaction_info_s_type *p_lpp_tx_type
);

typedef struct
{
  uint8 client_id; /*A unique client id*/  
  /* callback fn ptr */
     /**< callback notification function */
   tm_lpp_tx_mgr_client_cb_func *client_cb_func; 
    
} tm_lpp_tx_mgr_client_type;

typedef struct
{
  boolean b_in_use;
  tm_lpp_tx_mgr_client_type client;
  
} tm_lpp_tx_mgr_client_list_type;

/* Number of LPP clients. */
extern tm_lpp_tx_mgr_client_list_type tm_lpp_tx_mgr_client_list[TM_LPP_TX_MGR_CLIENT_MAX];


/*===========================================================================
  tm_lpp_tx_mgr_init
 
  Description:
    LPP function to init the transaction manager 
  
  Parameters
 
  Returns 
    none
===========================================================================*/
void tm_lpp_tx_mgr_init(uint8 u_client_id);


/*===========================================================================
  tm_lpp_decode message

  Description:

     This function is called by the LPP transaction manager to decode LPP transaction

  Parameters: 

     *p_payload - payload of the LPP transaction
      *decode_status_ptr - status of the decoding

  Return value:                                 
     pointer to the decoded message                            
                                 
=============================================================================*/
void *  tm_lpp_decode_message (const tm_pdu_buf_type *p_payload, 
                                /**< encoded data */
                                int *decode_status_ptr
);

/*===========================================================================
  tm_lpp_tx_mgr_handle_encode_transaction

  Description:

     This function is called by the LPP clients to process a LPP transaction

  Parameters: 
      client_handle - handle to the client
      message_type - message to be encoded
      p_lpp_tx_id  - Look-up transaction id. Used to retrieve the tx item from table if provided.
      p_lpp_tx_type - Pointer to the transaction item. If its provided it should be of valid type.
      So use it directly instead of again calling the handle entry function & copy over the data.
      lpp session type - UP or CP

  Return value: none
=============================================================================*/
boolean tm_lpp_tx_mgr_handle_encode_transaction ( 
      uint8 u_client_handle,
      tm_lpp_message_e_type z_message_type,
      tm_lpp_transaction_id_s_type *p_lpp_tx_id,
      tm_lpp_transaction_info_s_type **p_lpp_tx_type,
      uint8                         lpp_session_type
);

/*===========================================================================
  tm_lpp_tx_mgr_handle_decode_transaction

  Description:

     This function is called by the LPP clients to process a LPP transaction

  Parameters: 
      client_handle - handle to the client
      p_payload - lpp payload to be decoded
      p_lpp_tx_type - pointer to the transaction table
      lpp session type - UP or CP      
      tm_lpp_error_type - Pointer to LPP Error found during decode

  Return value: none
=============================================================================*/
 boolean tm_lpp_tx_mgr_handle_decode_transaction ( 
      uint8 u_client_handle,
      const tm_pdu_buf_type *p_payload,
      tm_lpp_transaction_info_s_type **p_lpp_tx_type,
      uint8                         lpp_session_type,
      tm_lpp_error_type              *p_lpp_error
);


/*===========================================================================
  tm_lpp_tx_mgr_client_register

  Description:

     This function is used by clients to register with LPP transaction manager

  Parameters: 

  *p_client: Pointer to client structure
  *handle: Pointer to client handle which will be filled by this function

  Return value: 
    boolean: SUCCESS or FAIL
=============================================================================*/
boolean tm_lpp_tx_mgr_client_register(const tm_lpp_tx_mgr_client_type * p_client, uint8 *handle);


/*===========================================================================
  tm_lpp_tx_mgr_client_deregister

  Description:

     This function is used by clients to deregister to Geofencing Services.

  Parameters: 

    uint8 client_id: A unique client identifier.

  Return value: 
    boolean: SUCCESS or FAIL
=============================================================================*/
boolean tm_lpp_tx_mgr_client_deregister(uint8 client_handle);


/*===========================================================================
  tm_lpp_tx_mgr_client_deregister

  Description:

     This function is used by clients to deregister to Geofencing Services.

  Parameters: 

    uint8 client_id: A unique client identifier.
    uint8* error_coe: Error code in case of error will be returned in this param.

  Return value: 
    boolean: SUCCESS or FAIL
=============================================================================*/
tm_lpp_tx_mgr_client_type *tm_lpp_tx_mgr_client_get_client_ptr(uint8 client_handle);


/*=========================================================================
  tm_lpp_encode_ack

  Description:
    LPP function to encode Explicit LPP ACK message.
    
  Returns:
    TRUE if the encoding passed, FALSE otherwise    
==========================================================================*/
boolean tm_lpp_encode_ack (uint8 u_seq_number, tm_pdu_buf_type  *p_pdu_buf, boolean end_flag);

/*===========================================================================
  tm_lpp_tx_mgr_find_seq_number
 
  Description: 
    LPP function to find the transaction with matching sequence number.
  
  Parameters
     u_directionn - 0 - incoming ; 1 - outgoing
     u_seq_number - sequence number to search for a match
    **p_lpp_tx_type- pointer to the transaction table with a match
 
  Returns
    TRUE if a match, FALSE otherwise
=========================================================================*/
boolean tm_lpp_tx_mgr_find_seq_number( 
  uint8                           u_direction,
  uint8                           u_seq_number,
  tm_lpp_transaction_info_s_type **p_lpp_tx_type
);

/*===========================================================================
  tm_lpp_tx_mgr_remove_entry_by_clientid
 
  Description: 
    Remove all transactions associated with the client id.
 
  Parameters:
   u_client_id - client id.
   
  Returns: 
   None
=============================================================================*/
void tm_lpp_tx_mgr_remove_entry_by_clientid( uint8 const u_client_id );

/*===========================================================================
  tm_lpp_tx_mgr_remove_entry
 
  Description: 
    LPP function to remove one or all transactions associated with a transaction id.
 
  Parameters:
   *p_lpp_tx_type- pointer to the transaction to be removed
   
  Returns: 
   None
=============================================================================*/
void tm_lpp_tx_mgr_remove_entry(tm_lpp_transaction_info_s_type **p_lpp_tx_to_remove);

/*===========================================================================
  tm_lpp_tx_mgr_get_num_tx
 
  Description: 
    LPP function to get number of outstanding transactions associated with a client 
  Parameters:
  u_client_handle - Client Handle
                                 
  Returns: 
   uint8 - number of transaction entries 
=============================================================================*/

uint8 tm_lpp_tx_mgr_get_num_tx( uint8 u_client_handle );

/*===========================================================================
  tm_lpp_tx_mgr_insert_entry
 
  Description: 
    LPP function to insert a transaction in transaction 
          manager table
 
  Parameters:
   *p_lpp_tx_id  - pointer to transaction id type
   *p_lpp_tx_type- pointer to the transaction table
    client_handle - the client_handle to which this transaction belongs to

  Returns: 
    Returns the TRUE or FALSE. 
=============================================================================*/
boolean  tm_lpp_tx_mgr_insert_entry( const tm_lpp_transaction_id_s_type *p_lpp_tx_id,
                                     tm_lpp_transaction_info_s_type **p_lpp_tx_type,
                                     uint8 u_client_handle );

/*===========================================================================
  tm_lpp_tx_mgr_get_tx_by_rli_timer
 
  Description: 
    Function to find the pending transaction item which has matching RLI timer id.
 
  Parameters:
   *p_timer_param  - pointer to timer information
   client_handle - the client_handle to which this transaction belongs to

  Returns: 
    Return the transaction entry for the matching timer id in the transaction table.
    Otherwise return NULL.
=============================================================================*/
tm_lpp_transaction_info_s_type* tm_lpp_tx_mgr_get_tx_by_rli_timer( os_TimerExpiryType const *const p_timer_param,
                                                                    uint8 const u_client_handle );

/*===========================================================================
  tm_lpp_tx_mgr_get_tx_by_gnss_auxi_timer

  Description:
    Function to find the pending transaction item which has matching GNSS timer id.

  Parameters:
   *p_timer_param  - pointer to timer information
   client_handle - the client_handle to which this transaction belongs to

  Returns:
    Return the transaction entry for the matching timer id in the transaction table.
    Otherwise return NULL.
=============================================================================*/
tm_lpp_transaction_info_s_type* tm_lpp_tx_mgr_get_tx_by_gnss_auxi_timer( os_TimerExpiryType const *const p_timer_param,
                                                                    uint8 const u_client_handle );

/*===========================================================================
  tm_lpp_tx_mgr_get_tx_by_tech

  Description:
    Function to find the 'Request Location Info' transactions & return the matching transaction item
    which has request technology in the RLI teck mask. Note the requested technology could be same
    or part of RLI tech mask.

  Parameters:
    tech_mask - Requested technology (like PRM, LR, OTDOA, WLAN-AP, DBH etc)
    u_client_handle - Client handle

  Returns:
    Return the transaction item for the matching technology in the transaction table.
    Otherwise return NULL.
=============================================================================*/
tm_lpp_transaction_info_s_type* tm_lpp_tx_mgr_get_tx_by_tech( rli_tx_tech_mask const tech_mask,
                                                              uint8 const u_client_handle );

/*===========================================================================
  tm_lpp_tx_mgr_get_rli_tx_item

  Description:
    Function to find the list of open 'Request Location Info' transaction item & return those to
    calling function.

  Parameters:
    pz_rli_txitem [OUT] - Point to list of RLI transaction item.
    cpu_total_rli_txitem[OUT] - Total number of RLI transaction item found & returned by this
                                function.
    u_client_handle[IN] - Client handle

  Returns:
    Return TRUE if atleast one RLI transaction item is present, otherwise FALSE.
=============================================================================*/
boolean tm_lpp_tx_mgr_get_rli_tx_item( tm_lpp_transaction_info_s_type *pz_rli_txitem[MAX_CONCUR_LPP_RLI_TRANSACTIONS],
                                       uint8 *const cpu_total_rli_txitem,
                                       uint8 const u_client_handle );

/*===========================================================================
  tm_lpp_tx_mgr_get_client_type
 
  Description:
    LPP function to get the client structure given a client handle
  
  Parameters
    u_client_index - Index of the client in the transaction manager client list table.
 
  Returns 
    Returns the pointer to the client type
===========================================================================*/
tm_lpp_tx_mgr_client_type *tm_lpp_tx_mgr_get_client_type(uint8 u_client_index);

/*===========================================================================
  tm_lpp_tx_mgr_get_tx_by_early_response

  Description:
    Function to find the 'Request Location Info' transactions & return the matching transaction item
    which has early response in the RLI.

  Parameters:
   *p_timer_param  - pointer to timer information
   client_handle - the client_handle to which this transaction belongs to
 
  Returns:
    Return the transaction item for which early response is needed.
    Otherwise return NULL.
=============================================================================*/
tm_lpp_transaction_info_s_type* tm_lpp_tx_mgr_get_tx_by_early_timer(os_TimerExpiryType const *const p_timer_param,
                                                                uint8 const u_client_handle);

/*===========================================================================
  tm_lpp_tx_mgr_remove_lpp_ext_rli

  Description:
    Function to find the 'Request Location Info' transactions  with EarlyResponse
    & return clear the flag

  Parameters:
    pz_lpp_tx_item:  Pointer to RLI Tx item to be removed from TX manager queue
    lpp_ext       :  Type of LPP extension call flow specific RLI to remove.
  Returns:

=============================================================================*/
void tm_lpp_tx_mgr_remove_lpp_ext_rli( tm_lpp_transaction_info_s_type const * const pz_lpp_tx_item,
                                             const uint8 lpp_ext);

/*===========================================================================
FUNCTION tm_lpp_tx_mgr_add_early_resp_tx

DESCRIPTION
  This function
  1)Duplicates the original RLI tech carrying EarlyReponse Qos in TX MGR.
  2)Copies required fields into new TX item
  3)Unchecks the EarlyResponse LPP_EXT flag in original TX item.


PARAMETERS:
  pz_lpp_tx_item : Pointer to Original LPP Tx item, which will be used to
                   duplicate the transaction
DEPENDENCIES: none.

RETURN VALUE:   p_lpp_tx_rli   : Pointer to duplicate LPP Tx item which will be used to
                send Early Response.
SIDE EFFECTS: none.

===========================================================================*/
tm_lpp_transaction_info_s_type* tm_lpp_tx_mgr_add_early_resp_tx( tm_lpp_transaction_info_s_type* pz_lpp_tx_item );

/*===========================================================================
  tm_lpp_tx_mgr_get_tx_item_by_clientid

  Description:
    Function to find the list of transaction item associated to a particular client & return those
    to calling function.

  Parameters:
    pz_txitem [OUT] - Point to list of transaction item.
    cpu_total_txitem[OUT] - Total number of transaction item found & returned by this function.
    u_client_handle[IN] - Client handle

  Returns:
    Return TRUE if atleast one RLI transaction item is present, otherwise FALSE.
=============================================================================*/
boolean tm_lpp_tx_mgr_get_tx_item_by_clientid( tm_lpp_transaction_info_s_type *pz_TxItems[TM_LPP_TX_MGR_TRANSACTIONS_MAX],
                                               uint8 *const cpu_TotalTxItems, uint8 const cu_ClientHandle );

/*===========================================================================
  tm_lpp_tx_mgr_is_osys_ctxt_mem_reset_allowed

  Description:
    Function to find if the asn1 mem context rest is allowed based on the no of transactions pending at tx manager.

  Parameters:
   void

  Returns:
    Return TRUE if no transactions pending , otherwise FALSE.
=============================================================================*/


boolean tm_lpp_tx_mgr_is_osys_ctxt_mem_reset_allowed( void );


#endif
