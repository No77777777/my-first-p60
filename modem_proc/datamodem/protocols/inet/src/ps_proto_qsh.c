/*!
  @file
  ps_proto_qsh.c

  @brief
  QSH implementation for Data Protocols.
*/

/*==============================================================================

  Copyright (c) 2019 QUALCOMM Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

==============================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.


when       who         what, where, why
--------   ---         ------------------------------------------------------------- 
01/09/19   rj         Initial release
==============================================================================*/

/*==============================================================================

                           INCLUDE FILES

==============================================================================*/
#include "datamodem_variation.h"
#include "customer.h"
#include "comdef.h" 

#ifdef FEATURE_QSH_MDUMP

#include "ps_proto_qsh.h"
#include "ds_Utils_DebugMsg.h"
#include "ps_stat_tcp.h"
#include "ps_stat_udp.h"
#include "queue.h"
#include "ps_socki_defs.h"

#include "dhcp.h"
#include "dhcpi.h"
#include "ps_clat_sm.h"
#include "ps_icmp6_nd.h"
#include "ps_ip_fraghdl.h"
#include "ps_ip6i_sm.h"
#include "ps_pppi.h"
#include "ps_pppi_auth.h"

/*==============================================================================

                         DEFINITIONS

==============================================================================*/


/*==============================================================================

                         EXTERNAL Variables

==============================================================================*/
extern ps_stat_tcp_g_s_type tcp_stats;
extern ps_stat_udp_g_s_type udp_stats;
extern q_type ps_socket_send_q;
extern struct dss_socket_config_s sock_config_cb;

/*==============================================================================

                                FUNCTIONS

==============================================================================*/

/*===========================================================================

FUNCTION ps_utils_qsh_cb

DESCRIPTION
  Processes the QSH request .
  Currently we handle only QSH_ACTION_MDUMP.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
  
===========================================================================*/
QSH_MDUMP_FN_ATTR void  ps_proto_qsh_cb()
{
    ps_proto_qsh_mdump_collect();
  
}

/*===========================================================================
FUNCTION ps_utils_qsh_mdump_collect
  
DESCRIPTION
  This function collects the mdump for Data Interface module.

DEPENDENCIES
  None


RETURN VALUE
  None


SIDE EFFECTS
  None

===========================================================================*/ 
QSH_MDUMP_FN_ATTR void ps_proto_qsh_mdump_collect()
{

  dhcp_am_mp_qsh_mdump_collect();// dhcp_am_mp_head handle
  dhcp_server_ps_iface_qsh_mdump_collect(); //dhcp_static_fltr_handle
  dhcp6_server_ps_iface_qsh_mdump_collect(); //dhcp6_static_fltr_handle
  ps_clat_sm_qsh_mdump_collect(); //clat_config_info & rm_global_fltr_handle
  ps_icmp_nd_qsh_mdump_collect(); //nd_config_items
  /* ipv6_priv_ext_enabled, priv_ext_lifetimes, 
     ip6_include_rdnss_opt, ip6_static_fltr_handle, 
     ip6_static_ra_fltr_handle, dssps_ipv6_enabled_setting_flag, 
     dhcp6_duid_counter, icmp6_mdm_iid_fltr_enable*/
  ps_ip6_sm_qsh_mdump_collect(); 
  ps_ip_fraghdl_qsh_mdump_collect(); //rcvd_fragments_num & ip_fraghdl_array
  ps_ppp_qsh_mdump_collect(); //ppp_def_dev_const
  ps_pppi_qsh_mdump_collect(); //ppp_info
  ps_ppp_auth_qsh_mdump_collect(); //ppp_mmgsdi_auth_config  
  
  /* Capturing Global variable directly */
  qsh_mdump_collect_high(&tcp_stats, sizeof(ps_stat_tcp_g_s_type));
  qsh_mdump_collect_high(&udp_stats, sizeof(ps_stat_udp_g_s_type));
  qsh_mdump_collect_high(&ps_socket_send_q, sizeof(q_type));
  qsh_mdump_collect_high(&sock_config_cb, sizeof(struct dss_socket_config_s));
}

#endif /* FEATURE_QSH_MDUMP */

