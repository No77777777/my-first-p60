#ifndef DS_APP_PKT_SVC_API_H
#define DS_APP_PKT_SVC_API_H

#include "ps_in.h"
#include "ps_sys.h"

#define DS_APP_PKT_SVC_ERROR 0xFFFFFFFF /* uint32 max value */
#define DS_APP_PKT_SVC_APN_NAME_MAX_LEN 128

/*======================= Create request errors =======================*/
/* No Errors */
#define DS_APP_PKT_SVC_ERROR_NONE           0
/* Invalid or NULL param given */
#define DS_APP_PKT_SVC_ERROR_INVALID_PARAM -1
/* Memory allocation failure */
#define DS_APP_PKT_SVC_ERROR_MALLOC        -2


/*========================= Status codes for callback =========================*/
#define DS_APP_PKT_SVC_REQUEST_COMPLETE          1

#define DS_APP_PKT_SVC_ERROR_DSNET               -100
#define DS_APP_PKT_SVC_ERROR_SOCKET              -101
#define DS_APP_PKT_SVC_ERROR_POLICY_INFO         -102
// #define DS_APP_PKT_SVC_ERROR_IP_ADDR_INFO         -103


typedef enum
{
  DS_APP_PKT_SVC_PROTOCOL_UDP
  // , DS_APP_PKT_SVC_PROTOCOL_TCP // TODO: future work
} ds_app_pkt_svc_protocol_type;



typedef enum
{
  DS_APP_PKT_SVC_RAT_EPC_ANY,       //3GPP, 3GPP2, IWLAN
  DS_APP_PKT_SVC_RAT_WWAN_ANY,      //3GPP, 3GPP2
  DS_APP_PKT_SVC_RAT_3GPP_ANY,      //3GPP
  DS_APP_PKT_SVC_RAT_3GPP2_ANY,     //3GPP2
  DS_APP_PKT_SVC_RAT_WLAN_LB,       //WLAN local breakout
  DS_APP_PKT_SVC_RAT_IWLAN,         //IWLAN 
  DS_APP_PKT_SVC_RAT_MAX
} ds_app_pkt_svc_rat_type;



typedef struct
{
  ds_app_pkt_svc_protocol_type  protocol;                                    // UDP or TCP
  ds_app_pkt_svc_rat_type       rat;                                         // any, wwan, wlan, iwlan
  char                          apn_name[DS_APP_PKT_SVC_APN_NAME_MAX_LEN+1]; // if empty, use default
  ps_sys_subscription_enum_type subs_id;                   
} ds_app_pkt_svc_nw_policy_type;


/*both IP address and port should be big endian*/
typedef struct
{
  ps_ip_addr_type ip_addr; // IP type MUST be either IPV4_ADDR or IPV6_ADDR
  uint16          port;
} ds_app_pkt_svc_addr_info;



typedef struct
{
  boolean enable_default_retry;
  uint32  retry_interval; // in ms
  uint16  max_retry_count;
} ds_app_pkt_svc_pdn_retry_options;



typedef struct
{
  ds_app_pkt_svc_nw_policy_type     policy_info;
  ds_app_pkt_svc_addr_info          src_addr_info;
  ds_app_pkt_svc_addr_info          dst_addr_info;     // Required
  ds_app_pkt_svc_pdn_retry_options  pdn_retry_option;
  uint32                            periodic_interval; // in ms
  const uint8*                      content_ptr;
  uint32                            content_len;
} ds_app_pkt_svc_send_config_type;



typedef struct
{
  ds_app_pkt_svc_nw_policy_type     policy_info;
  ds_app_pkt_svc_addr_info          src_addr_info; // Required
  ds_app_pkt_svc_addr_info          dst_addr_info; // Port required
  ds_app_pkt_svc_pdn_retry_options  pdn_retry_option;
} ds_app_pkt_svc_receive_config_type;



typedef struct
{
  const uint8* content_ptr;
  uint32       content_size;
} ds_app_pkt_svc_receive_info;



typedef void (*ds_app_pkt_svc_status_cb_fcn)(
                                             uint32  request_id,
                                             sint15  error,
                                             void*   error_info
                                             );

typedef void (*ds_app_pkt_svc_receive_cb_fcn)(
                                              uint32                       request_id,
                                              ds_app_pkt_svc_receive_info* receive_info
                                              );




#ifdef __cplusplus
extern "C"
{
#endif

uint32 ds_app_pkt_svc_send_start(
                                 ds_app_pkt_svc_send_config_type* send_config_info,
                                 ds_app_pkt_svc_status_cb_fcn     status_cb_fcn,
                                 sint15*                          aps_errno
                                 );



uint32 ds_app_pkt_svc_receive_start(
                                    ds_app_pkt_svc_receive_config_type* receive_config_info,
                                    ds_app_pkt_svc_receive_cb_fcn       receive_cb_fcn,
                                    ds_app_pkt_svc_status_cb_fcn        status_cb_fcn,
                                    sint15*                             aps_errno
                                    );


// OR
void ds_app_pkt_svc_stop_request(uint32 request_id);

#ifdef __cplusplus
}
#endif

#endif /* DS_APP_PKT_SVC_API_H */
