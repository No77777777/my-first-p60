#ifndef PS_DNSI_DEFS_H
#define PS_DNSI_DEFS_H
/*===========================================================================

                       P S  _  D N S I  _  D E F S  .  H

DESCRIPTION

  The Data Services DNS module internal Header File. Contains declarations
  of enums, variables and control blocks used by the DNS subsystem.

EXTERNALIZED FUNCTIONS
  None.

INITIALIZATIONS AND SEQUENCING REQUIREMENTS
  None.

Copyright (c) 2007-2011 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary.
===========================================================================*/

/*===========================================================================

                            INCLUDE FILES FOR MODULE

===========================================================================*/
#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"


#include "dssdns.h"
#include "dss_dns_priv.h"
#include "queue.h"
#include "ps_svc.h"
#include "dssocket.h"  // Needed for defining struct ps_sockaddr_storage
#include "err.h"
#include "msg.h"
#include "amssassert.h"

#include "ps_dnsi_query_defs.h"

/*===========================================================================

                               MACRO DECLARATIONS

===========================================================================*/
/* isalpha/isalnum equivalents */
#define ps_isalpha(ch) ( (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') )

#define ps_isalnum(a) ( ps_isalpha(a) || (a >= '0' && a <= '9') )

#ifdef FEATURE_Q_NO_SELF_QPTR
#define Q_DELETE( qptr, linkptr )        q_delete( (qptr), (linkptr) );
#else
#define Q_DELETE( qptr, linkptr )        q_delete( (linkptr) );
#endif /* FEATURE_Q_NO_SELF_QPTR */

/*===========================================================================

                            INTERNAL DATA DECLARATIONS

===========================================================================*/
#define PS_DNSI_PS_TIMER                2

typedef int32 ps_dnsi_resolver_handle_type;
#define PS_DNSI_RESOLVER_INVALID_HANDLE  (-1)

/* Define for DNS API control block indicating service parameter was not
   provided in the API call */
#define PS_DNSI_SERVICE_NOT_SPECIFIED 0

/*-------------------------------------------------------------------------
  State of the API control block
-------------------------------------------------------------------------*/
typedef enum
{
  PS_DNSI_API_CB_STATE_INVALID = 0,
  PS_DNSI_API_CB_STATE_OPEN    = 1,
  PS_DNSI_API_CB_STATE_FREE    = 2
} ps_dnsi_api_state_enum_type;


/*-------------------------------------------------------------------------
  Suffixes for IPv4 and IPv6 PTR queries.
-------------------------------------------------------------------------*/
#define DNS_INADDR_ARPA_STR     "in-addr.arpa."
#define DNS_IN6ADDR_ARPA_STR    "ip6.arpa."

/*---------------------------------------------------------------------------
  DNS Critical section
---------------------------------------------------------------------------*/
/*
#ifdef FEATURE_DATA_PS_L4
static rex_crit_sect_type dns_crit_section;
#endif
*/

/*-------------------------------------------------------------------------
  Hostname query struct.
  Contains an array of query type, query_class and one query name.
  Used by getaddrinfo as well as getipnodebyname.
-------------------------------------------------------------------------*/
typedef struct ps_dnsi_query_struct_type
{
  ps_dnsi_query_type_enum_type   query_type;
  ps_dnsi_query_class_enum_type  query_class;
  char                           query_data[DSS_DNS_MAX_DOMAIN_NAME_LEN];
} ps_dnsi_query_struct_type;

/*-------------------------------------------------------------------------
  Pending query struct..
-------------------------------------------------------------------------*/
typedef struct ps_dnsi_pending_query_struct_type
{
  q_link_type                    link;
  dss_dns_api_query_enum_type    curr_query_mask;
  ps_dnsi_query_class_enum_type  query_class;
  char                           query_data[DSS_DNS_MAX_DOMAIN_NAME_LEN];
} ps_dnsi_pending_query_struct_type;

/*---------------------------------------------------------------------------
Data structure for the result control block.
  handle            - Query handle
result_mask   - available results mask
api_cb_ptr       - API control block
---------------------------------------------------------------------------*/
typedef struct ps_dnsi_result_cb_s_type
{
  q_link_type                        link;
  dss_dns_query_handle_type          handle;
  dss_dns_result_type_enum_type      result_mask;
  q_type                             answer_q;
}ps_dnsi_result_cb_type;

typedef struct ps_dnsi_legacy_api_client_info
{
  boolean                            is_present;
  uint16                             port_num;
  uint16                             sock_type;
  uint16                             transport_proto;
  dss_dns_api_type_enum_type         api_type;
}ps_dnsi_legacy_api_client_info;

/*---------------------------------------------------------------------------
  Data structure for the API layer control block.
  curr_query_mask - Current (Running) Query mask which need to be resolved
  res_handle        - Resolver handle for this API instance.
  api_type          - Type of API (ADDRINFO, NAMINFO)
  state             - State of API control block
  query_index       - Query index for multiple iteration case. Currently
                      used by ADDRINFO
  current_query           - A query struct, which is populated with the query
                      type, query class .
  port_num          - Port number (Valid only for ADDRINFO API)
  sock_type         - Socket type (Valid only for ADDRINFO API). Stored in
                      host byte order.
  transport_proto   - Transport protocol (Valid only for ADDRINFO API)
  addr_family       - Address family (Valid only for ADDRINFO API)
  flags             - Options associated with the API
                      for ADDRINFO this is cast to DSS_DNS_AI_FLAGS_ENUM_TYPE
                      for NAMEINFO this is cast to DSS_DNS_NI_FLAGS_ENUM_TYPE
  response_received - A flag that indicates if any successful response was
                      received, and success should be forwarded to the
                      application.
  result_cb_ptr   - result control block pointer
  pending_query_q - Store pending DNS queries 
----------------------------------------------------------------------------*/
typedef struct ps_dnsi_api_cb_s_type
{
  dss_dns_session_mgr_handle_type    session_handle;
  dss_dns_api_query_enum_type        curr_query_mask;
  dss_dns_result_type_enum_type      curr_result_mask; 
  ps_dnsi_resolver_handle_type       res_handle;
  ps_dnsi_api_state_enum_type        state;
  ps_dnsi_query_struct_type          current_query;
  q_type                             pending_query_q;
  ps_dnsi_legacy_api_client_info     legacy_api_info;
  ps_dnsi_result_cb_type           * result_cb_ptr;
  dss_dns_query_hints_info           hints_info;
  // This flag is used to determine whether the query Failed or not.
  // The QUERY_FAILED must be 0 in order to prevent data corruption.
  // Take care when modifying this code (lock protcection may be required)
  enum dns_query_status {QUERY_FAILED=0, QUERY_IN_PROGRESS} query_status;
} ps_dnsi_api_cb_type;


/*---------------------------------------------------------------------------
  Session control block
  session_handle        - Session handle identifying a session control block.
  cback_f_ptr           - Application callback function pointer.
  user_data_ptr         - Application callback function user data.
  net_policy            - Network policy to be used to send the DNS query.
  is_net_policy_set     - If the application sets the iface_id, routeable
                          iface mode or network policy config param, this 
                          value would be set to true.
  use_cache_results     - Specifies if the resolver can use results from the
                          the DNS cache or not.
  add_to_cache          - Specifies if the results of the name resolution
                          should be added to the cache or not.
  session_deleted       - Identifies if the delete session API is already 
                          processed.
  is_recursion_desired  - If set to true it will request the DNS to do recursion.
  linear_backoff_retry_parameters_info
                        - Parameters that define the behevior of the for linear 
                          backoff retry mechanism.
  client_specified_servers_behavior
                       - When set to USE_LAST the DNS servers configured
                         by the user for this session are used only after
                         trying the iface layer (technology specific e.g
                         UMTS) DNS servers when sending a DNS query.
                         When set to USE_FIRST the DNS servers configured
                         by the user for this session are used before
                         trying the iface layer DNS servers.
                         When set to USE_EXCLUSIVELY the iface layer DNS 
                         primary and secondary servers are not used. Only
                         the session’s DNS servers queue is used.
  dns_ipv4_servers_q    - IPV4 DNS servers queue configured for this session.
  dns_ipv6_servers_q    - IPV6 DNS servers queue configured for this session.
---------------------------------------------------------------------------*/
typedef struct ps_dnsi_session_mgr_cb_s_type
{
   dss_dns_session_mgr_handle_type             session_handle;
   dss_dns_cback_f_type                        cback_f_ptr;
   dss_dns_cback_f_type_ex                     cback_f_ptr_ext;
   void                                      * user_data_ptr;
   dss_net_policy_info_type                    net_policy;
   boolean                                     is_net_policy_set;
   boolean                                     use_cache_results;
   boolean                                     add_to_cache;
   boolean                                     session_deleted;
   boolean                                     is_recursion_desired;
   dss_dns_linear_backoff_retry_info_type      linear_backoff_retry_parameters_info;
   ps_dnsi_api_cb_type                       * api_cb_ptr;
   dss_dns_client_specified_servers_enum_type  client_specified_servers_behavior;
   q_type                                      dns_ipv4_servers_q;
   q_type                                      dns_ipv6_servers_q;
   dss_dns_exp_retry_info_type                 exp_timer_params_info;
   q_type                                      passive_results_q;
} ps_dnsi_session_mgr_cb_type;

/*---------------------------------------------------------------------------
Data structure for node in dns_ipv4/6_servers_q
---------------------------------------------------------------------------*/
typedef struct ps_dnsi_addr_node_s_type
{
  q_link_type                               link;
  struct ps_sockaddr_storage                dns_serv_addr;
} ps_dnsi_addr_node_type;

/*---------------------------------------------------------------------------
  Hostent API control block
  session_handle       - Session handle identifying a session control block.
  cback_f_ptr          - Application callback function pointer.
  user_data_ptr        - Application callback function user data.
  iface_id             - Iface id configuration parameter.
  routeable_iface_mode - Identifies if the lookup needs to be performed when
                         mobile is connected to an external device
  use_cache_results    - Specifies if the resolver can use results from the
                         the DNS cache or not.
  add_to_cache         - Specifies if the results of the name resolution
                         should be added to the cache or not.
---------------------------------------------------------------------------*/
typedef struct dss_dnsi_hostent_cb_s_type
{
  dss_dns_session_mgr_handle_type    session_handle;
  dss_dns_query_handle_type          query_handle;
  dss_dns_api_type_enum_type         api_type;
  uint16                             addr_family;
  char                               query_data_ptr[DSS_DNS_MAX_DOMAIN_NAME_LEN];
  struct dss_hostent               * ret_hostent_ptr;
  dss_dns_hostent_cback_f_type       cback_f_ptr;
  void                             * user_data_ptr;
} dss_dnsi_hostent_cb_type;



#endif  /* PS_DNSI_DEFS_H */
