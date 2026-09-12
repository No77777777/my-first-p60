#ifndef TRM_H
#define TRM_H
typedef uint32                    trm_request_tag_t;


typedef enum
{           
  TRM_1X,
  TRM_1X_SECONDARY,

  TRM_HDR,
  TRM_HDR_SECONDARY,

  TRM_GPS,

  TRM_UMTS,
  TRM_UMTS_SECONDARY,

  TRM_GSM1,

  TRM_GSM2,

  TRM_WCDMA,

  TRM_TDSCDMA,
  TRM_TDSCDMA_SECONDARY,

  TRM_UMTS_CA,
  TRM_UMTS_CA_SECONDARY,

  TRM_LTE,
  TRM_LTE_SECONDARY,
  TRM_LTE_CA,

  TRM_LTE_CA_SECONDARY,


  TRM_CM,

  TRM_GSM3,

  TRM_GSM_SECONDARY,


  TRM_RF,

  /* Dummy client for L+G DSDS */
  TRM_IRAT,

  /* WLAN client */
  TRM_WLAN,

  TRM_GPRS1,

  TRM_GPRS2,

  TRM_LTE_CA1,    

  TRM_LTE_CA1_SECONDARY,

  TRM_GSM1_SECONDARY,

  TRM_GSM2_SECONDARY,

  TRM_GSM3_SECONDARY,

  /* For internal bound-checking and array-sizing */
  TRM_LAST_CLIENT = TRM_GSM3_SECONDARY,

  /* Should always be TRM_LAST_CLIENT + 1 */
  TRM_MAX_CLIENTS,

  TRM_NO_CLIENT    = 0xFF
}
trm_client_enum_t;

typedef union
{

} trm_async_callback_data;

typedef enum
{
  TRM_AYSNC_DR_INFO_REQ       = 1,

  TRM_AYSNC_CONN_HOP_INFO_REQ = 2,

  /* optional async callback
      WLAN subscribes to this to know if unlock was due to conflicting IRAT event */
  TRM_ASYNC_IRAT_EVENT_INFO = 4,

  TRM_AYSNC_MAX_INFO_REQ

} trm_async_optional_event_bitmask;


typedef enum
{
  TRM_CONNECTED_MODE_HOP_ASYNC_INFO,

  TRM_REQUEST_AND_NOTIFY_ASYNC_INFO,

  TRM_REQUEST_AND_NOTIFY_ENH_ASYNC_INFO,

  TRM_MODIFY_CHAIN_STATE_ASYNC_INFO,

  TRM_UNLOCK_CHAIN_ASYNC_INFO,

  TRM_DUAL_RECEIVE_INFO,

  /* async callback to WLAN for IRAT event release info */
  TRM_IRAT_EVENT_ASYNC_INFO,

  TRM_ASYNC_EVENT_MAX,

}trm_async_callback_type_enum_t;

typedef uint32                    trm_request_tag_t;


typedef struct
{
  trm_client_enum_t               client;
  trm_async_callback_type_enum_t  cb_type;
  trm_async_callback_data         data;
  trm_request_tag_t               tag;
} trm_async_callback_information_type;




#endif /* TRM_H */
