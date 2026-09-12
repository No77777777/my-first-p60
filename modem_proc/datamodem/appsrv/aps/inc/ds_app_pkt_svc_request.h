
#ifndef DS_APP_PKT_SVC_REQUEST_H
#define DS_APP_PKT_SVC_REQUEST_H

#include "ds_app_pkt_svc_api.h"
#include "ds_ASTimer.h"
#include "ds_ASBuffer.h"
#include "dssocket.h"

void ds_app_pkt_svc_net_cb(
                           sint15            nethandle,
                           dss_iface_id_type iface_id,
                           sint15            err,
                           void*             net_cb_user_data
                           );
void ds_app_pkt_svc_sock_cb(
                            sint15 nethandle,
                            sint15 sockfd,
                            uint32 event_mask,
                            void*  sock_cb_user_data
                            );

void ds_app_pkt_svc_pdn_retry_timer_cb(uint32 user_data);
void ds_app_pkt_svc_periodic_timer_cb(uint32 user_data);




class ds_app_pkt_svc_request
{
public:
  ds_app_pkt_svc_request(
                         uint32                            client_request_id,
                         ds_app_pkt_svc_protocol_type      client_protocol,
                         dss_net_policy_info_type&         client_dss_policy_info,
                         ds_app_pkt_svc_addr_info&         client_src_addr_info,
                         ds_app_pkt_svc_addr_info&         client_dst_addr_info,
                         ds_app_pkt_svc_pdn_retry_options& client_pdn_retry_option,
                         ds_app_pkt_svc_status_cb_fcn      client_status_cb_fcn
                         );
  virtual ~ds_app_pkt_svc_request();

  void process_request();
  void stop_request();

  uint32 get_dsnet_handle_id()
  { return dsnet_handle_id; }
  uint32 get_sockfd()
  { return sockfd; }

  void set_error(uint32 err_type);

  /* Returns true if request had a hard failure or if one-time request is complete */
  bool remove_request()
  { return is_complete || is_failed; }


  void dsnet_event_hdlr(const dss_iface_id_type iface, const sint15 error);
  virtual void socket_event_hdlr(const uint32 event_mask) = 0;

  void handle_pdn_retry_timeout();
  virtual void handle_periodic_timeout();

  virtual void handle_pdn_down_soft_failure() = 0;
  

protected:
  virtual void socket_init() = 0;
  virtual void socket_communicate() = 0;
  bool socket_init(ip_addr_enum_type ip_type, ds_app_pkt_svc_addr_info& bind_addr_info);
  void socket_close();

private:
  void socket_start();
  void dsnet_connect();

public:
  const uint32 request_id;

protected:
  Appsrv::Utils::ASTimer pdn_retry_timer;
  Appsrv::Utils::ASTimer periodic_timer;
  uint32  pdn_retry_count;

  bool dsnet_is_up;
  bool is_complete;
  bool is_failed;

  /* Client config */
  dss_net_policy_info_type         dss_policy_info;
  ds_app_pkt_svc_protocol_type     protocol;
  ds_app_pkt_svc_addr_info         src_addr_info;
  ds_app_pkt_svc_addr_info         dst_addr_info;
  ds_app_pkt_svc_pdn_retry_options pdn_retry_option;
  ds_app_pkt_svc_status_cb_fcn     status_cb_fcn;


  /* DSNet */
  enum ds_app_pkt_svc_dsnet_status
  {
    DS_APP_PKT_SVC_NET_STATUS_NOHANDLE,
    DS_APP_PKT_SVC_NET_STATUS_DOWN_IN_PROGRESS,
    DS_APP_PKT_SVC_NET_STATUS_DOWN,
    DS_APP_PKT_SVC_NET_STATUS_UP_IN_PROGRESS,
    DS_APP_PKT_SVC_NET_STATUS_UP
  };

  sint15                      dsnet_handle_id;
  dss_iface_id_type           iface_id;
  ds_app_pkt_svc_dsnet_status dsnet_status;

  /* Socket */
  sint15                      sockfd;
};


class ds_app_pkt_svc_request_udp_send : public ds_app_pkt_svc_request
{
public:
  ds_app_pkt_svc_request_udp_send(
                                  uint32                           request_id,
                                  dss_net_policy_info_type&        dss_policy,
                                  ds_app_pkt_svc_send_config_type& send_config,
                                  ds_app_pkt_svc_status_cb_fcn     status_cb_fcn
                                  );
  virtual ~ds_app_pkt_svc_request_udp_send();

  virtual void socket_event_hdlr(const uint32 event_mask);
  virtual void handle_periodic_timeout();
  virtual void handle_pdn_down_soft_failure();


private:
  virtual void socket_init();
  virtual void socket_communicate();

  bool     is_periodic_timer_running;
  uint32   periodic_interval; //in ms
  ASBuffer send_content;
};



class ds_app_pkt_svc_request_udp_receive : public ds_app_pkt_svc_request
{
public:
  ds_app_pkt_svc_request_udp_receive(
                                     uint32                              request_id,
                                     dss_net_policy_info_type&           dss_policy,
                                     ds_app_pkt_svc_receive_config_type& receive_config,
                                     ds_app_pkt_svc_receive_cb_fcn       receive_cb_fcn,
                                     ds_app_pkt_svc_status_cb_fcn        status_cb_fcn
                                     );
  virtual ~ds_app_pkt_svc_request_udp_receive();

  virtual void socket_event_hdlr(const uint32 event_mask);
  virtual void handle_pdn_down_soft_failure();

private:
  virtual void socket_init();
  virtual void socket_communicate();
  void socket_receive();

  ds_app_pkt_svc_receive_cb_fcn      receive_cb_fcn;
};





#endif /* DS_APP_PKT_SVC_REQUEST_H */
