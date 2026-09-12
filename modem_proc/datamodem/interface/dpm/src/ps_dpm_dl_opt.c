/*===========================================================================

                     P S _ D L _ O P T I M I Z E D _ H D L R . C
DESCRIPTION


EXTERNALIZED FUNCTIONS


Copyright (c) 2011-2017 QUALCOMM Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/
/*===========================================================================

                            EDIT HISTORY FOR FILE
  $Header: //components/rel/data.mpss/3.5..2.1/interface/dpm/src/ps_dpm_dl_opt.c#2 $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
11/09/14    yl     IP data path re-architecture
11/01/13    rk     Removed feature mobileap.
09/30/13    bvd    DPM and IPA filtering support
09/27/13    wc     Support dynamic port configuration
07/26/13    cx     Change the ps_icmp6_get_dest_mac_addr arguments due to memcpy replacement
07/25/13    ms     Route packet to next iface for IPsec iface
07/15/13    mp     Deprecating memcpy and memmove.
05/30/13    wc     Non-MBIM packet filtering
01/29/13    vb     Fix to handle the GRE packet processing in
                   NAT optimized path.
12/20/12    at     Fix to make the IPv6 fragments go through legacy path
11/27/12    at     RNDIS DL path and CS fixes.
08/21/12    tw	   Deny ports that are reserved for RSVP or BIP
07/11/12    pgm    Added RNDIS Agg mode support.
07/05/12    cp     Fix to get the opt_filter_ptr from NAT iface in
                   MobileAP call.
05/07/12    mp     Fix to send TCP Reset for exhausted NAT entry.
03/25/12    sb     Added support for TCP_UDP protocol in optimized filters.
03/22/12    pgm    Check for valid um base iface ptr in ps_dl_phys_opt_rx_pkt.
02/29/12    am     Added Mobile AP V6 support.
02/29/12    am     Ship all packets to A5 if USB is not up.
02/13/12    pgm    Compiler warning fixes.
02/13/12    sb     Added iface statistics in optimized path
02/06/12    sb     Added router fragmentation support for optimized path
02/01/12    am     Added range checks for optimized filters.
02/01/12    cy     Added packet filter support
01/09/12    pgm    KW error and Compiler warning fixes
11/15/11    pgm    Fixed Bug for fragmented pkts in ps_dl_phys_opt_rx_pkt
11/11/11    pgm    Disabled MBIM header addition as it is in done in A2.
11/10/11    am     ICMP6 packets also need to be sent up.
11/01/11    am     Added NAT support.
09/22/11    am     Created module.
===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"

#include "ps_dpm_dl_opt.h"

#include "amssassert.h"
#include "msg.h"
#include "err.h"

#include "ps_dpm_dl_opt_hdlri.h"
#include "ps_dpm_opt_defs.h"
#include "ps_ifacei.h"
#include "ps_phys_linki.h"
#include "ps_crit_sect.h"
#include "ps_logging.h"
#include "ps_meta_info.h"
#include "ps_ifacei_utils.h"
#include "ps_arp.h"
#include "ds_rmnet_xport.h"
#include "ps_lan_llci.h"
#include "ds_rmnet_utils.h"
#include "ps_dpmi.h"

#include <stringl/stringl.h>

#include "ps_in.h"
#include "ps_icmp6_nd.h"

#include "ps_metai_info.h"
#include "ps_handle_mgr.h"
#include "ps_pkt_info_utils.h"
#include "ps_sys_fast_dormancy.h"
#include "ps_logging_defs.h"

#ifdef FEATURE_DATA_REFLECTIVE_QOS
#include "ps_ifacei.h"
#include "ps_flow.h"
#endif /* FEATURE_DATA_REFLECTIVE_QOS */

#ifdef FEATURE_DATA_PS_464XLAT
#include "ps_dpm_clat.h"
#include "ps_clat.h"
#endif /* FEATURE_DATA_PS_464XLAT */

/*===========================================================================

                             LOCAL DATA DEFINITIONS

===========================================================================*/
typedef enum
{
  PS_DPM_DL_OPT_NO_PATH      = 0,
  PS_DPM_DL_OPT_OPT_PATH     = 1,
  PS_DPM_DL_OPT_LEGACY_PATH  = 2
} ps_dpm_dl_opt_path_type;

#define PS_OPTIMIZED_IP_DL_V4_FRAG_CHECK(val)                              \
    (( (val) & ps_htons(PS_DPM_OPT_V4_FRAG_MASK) ) != 0)

extern ps_dl_opt_filter_spec_type * global_dl_opt_fltr_arr;

extern ps_crit_sect_type         ps_dpm_crit_section;

extern ps_dpm_cont_err_pkt_cb_info ps_dpm_cont_err_pkt_info;


#ifdef TEST_FRAMEWORK
#error code not present
#endif

/*===========================================================================

                        INTERNAL FUNCTION DEFINITIONS

===========================================================================*/
INLINE boolean ps_dl_opti_process_filters
(
  ps_dl_opt_filter_spec_type * filter_ptr,
  uint16 src_port,
  uint16 dst_port,
  uint16 protocol,
  uint16                     * filter_result,
  uint16 max_filter_entries
)
{
  uint8                    filter_cnt;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(NULL == filter_result)
  {
    DS_UTILS_ASSERT(0);
    return FALSE;
  }

  if (NULL != filter_ptr)
  {
    for (filter_cnt = 0;
         filter_cnt < max_filter_entries && filter_ptr->fi_handle != 0;
         filter_cnt++, filter_ptr++)
    {
      if ((protocol == filter_ptr->protocol) &&
          ((dst_port >= filter_ptr->dst_port_start &&
            dst_port <= filter_ptr->dst_port_end) ||
          (src_port >= filter_ptr->src_port_start &&
            src_port <= filter_ptr->src_port_end)))
      {
        *filter_result = (uint16)filter_ptr->filter_result;
        return TRUE;
      }
    }
  }

  return FALSE;
} /* ps_dl_opti_process_filters() */

boolean ps_dpm_dl_opti_rx_pkt_internal
(
  ps_iface_type            * v4_iface_ptr,
  ps_iface_type            * v6_iface_ptr,
  dsm_item_type           ** pkt_ref_ptr,
  ps_rx_meta_info_type    ** rx_mi_ptr,
  boolean                    is_legacy_proc_needed,
  ps_dpm_bearer_info_type  * bearer_ptr
)
{
  uint8  pkt_buf[PS_DPM_OPT_EXP_HEADER_LEN_DL + 1] ALIGN(4); //__attribute__((aligned(4)));
  ps_dpm_rm_info_type              * dpm_rm_info_ptr = NULL;
  ps_iface_type                    * um_base_iface_ptr = NULL;
  ps_iface_type                    * bridge_iface_ptr = NULL;
  ps_dl_opt_filter_spec_type       * filter_ptr;
  llc_frm_packed_mode_zero_type    * eth_hdr_ptr;
  int32                              ret_val_num = 0;
  boolean                            ret_status = TRUE;
  uint16                             total_len = 0;
  uint16                             dst_port;
  uint16                             src_port;
  uint16                             buf_len;
  uint16                             ip_hdr_len;
  uint8                              version;
  uint8                              protocol;
  uint8                              ip_hdr_first_byte;
  ps_dpm_dl_opt_path_type            path_type = PS_DPM_DL_OPT_NO_PATH;
  sint15                             ps_errno;
  errno_enum_type                    pkt_info_errno;
  ip_pkt_info_type                 * ip_pkt_info_ptr = NULL;
  boolean                            return_value = FALSE;
  uint16                             filter_result = 0xFFFF;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);
  PS_ENTER_CRIT_SECTION(&ps_dpm_crit_section);

  do
  {
    /*-----------------------------------------------------------------------
      Peek in to the first byte to determine version of the IP packet and
      process the packet appropriately
    -----------------------------------------------------------------------*/
    (void) dsm_extract(*pkt_ref_ptr, 0, &ip_hdr_first_byte, 1);

    version = ip_hdr_first_byte & PS_DPM_OPT_IP_VERSION_MASK;

    if (version == PS_DPM_OPT_V4_VERSION)                   /* IPv4 packet */
    {
      um_base_iface_ptr = v4_iface_ptr;

      /*---------------------------------------------------------------------
        Intentionally duplicated below two checks for IPv4 and IPv6 cases so
        that bytes are extracted only if Um is bridged. This optimizes code
        path for modem embedded calls without any additional overhead for
        bridged path
      ---------------------------------------------------------------------*/

      /*---------------------------------------------------------------------
        Drop the packet if Um iface is invalid
      ---------------------------------------------------------------------*/
      if (!PS_IFACE_IS_VALID(um_base_iface_ptr))
      {
        LOG_MSG_ERROR_1("ps_dpm_dl_opti_rx_pkt_internal(): "
                        "Invalid Um iface 0x%p, dropping packet", um_base_iface_ptr);
        PS_DPM_DL_INC_STATS(bearer_ptr, stats.dropped.pkts.invalid_iface, 1);
        path_type = PS_DPM_DL_OPT_NO_PATH;
        break;
      }

      /*---------------------------------------------------------------------
      Fetch IHL and validate it
      ---------------------------------------------------------------------*/
      ip_hdr_len = (ip_hdr_first_byte & PS_DPM_OPT_IP_HDR_LEN_MASK) << 2;
      if( ip_hdr_len  < PS_DPM_OPT_V4_HDR_LEN )
      {
        PS_DPM_DL_INC_STATS(bearer_ptr, stats.dropped.pkts.no_bytes, 1);      
        path_type = PS_DPM_DL_OPT_NO_PATH;
        break;
      }

      /*---------------------------------------------------------------------
       extract IP header and port information from  the IP packet
      ---------------------------------------------------------------------*/

      buf_len = dsm_extract(*pkt_ref_ptr,
                            0,
                            pkt_buf,
                            ip_hdr_len + PS_DPM_OPT_PORT_INFO_LEN);
      if ((ip_hdr_len + PS_DPM_OPT_PORT_INFO_LEN) > buf_len)
      {
        PS_DPM_DL_INC_STATS(bearer_ptr, stats.legacy_path.pkts.no_bytes, 1); 
        path_type = PS_DPM_DL_OPT_LEGACY_PATH;
        break;
      }

      protocol  = *(pkt_buf + PS_DPM_OPT_V4_PROT_FIELD_OFFSET);
      total_len =
        ps_htons(*((uint16 *)(pkt_buf + PS_DPM_OPT_V4_TOTAL_LEN_OFFSET)));
      /*---------------------------------------------------------------------
        Take legacy path for all IPv4 fragments
      ---------------------------------------------------------------------*/
      if (PS_OPTIMIZED_IP_DL_V4_FRAG_CHECK
          (
            *((uint16 *)(pkt_buf + PS_DPM_OPT_V4_FRAG_OFFSET)))
          )
      {
        PS_DPM_DL_INC_STATS(bearer_ptr,stats.legacy_path.pkts.frag_pkt, 1);
        PS_DPM_DL_INC_STATS(bearer_ptr,stats.legacy_path.bytes.frag_pkt, total_len);
        path_type = PS_DPM_DL_OPT_LEGACY_PATH;
        break;
      }
    }
    else if (version == PS_DPM_OPT_V6_VERSION)              /* IPv6 packet */
    {
#ifdef FEATURE_DATA_REFLECTIVE_QOS
      ip_vsn = IP_V6;
#endif /* FEATURE_DATA_REFLECTIVE_QOS */
      um_base_iface_ptr = v6_iface_ptr;

      /*---------------------------------------------------------------------
        Intentionally duplicated below two checks for IPv4 and IPv6 cases so
        that bytes are extracted only if Um is bridged. This optimizes code
        path for modem embedded calls without any additional overhead for
        bridged path
      ---------------------------------------------------------------------*/

      /*---------------------------------------------------------------------
        Drop the packet if Um iface is invalid
      ---------------------------------------------------------------------*/
      if (!PS_IFACE_IS_VALID(um_base_iface_ptr))
      {
        LOG_MSG_ERROR_1("ps_dpm_dl_opti_rx_pkt_internal(): "
                        "Invalid Um iface 0x%p , dropping packet", um_base_iface_ptr);
        PS_DPM_DL_INC_STATS(bearer_ptr,stats.dropped.pkts.invalid_iface, 1);
        path_type = PS_DPM_DL_OPT_NO_PATH;
        break;
      }

      /*---------------------------------------------------------------------
        Extract IP header and port information from the IP packet
      ---------------------------------------------------------------------*/
      buf_len = dsm_extract(*pkt_ref_ptr,
                            0,
                            pkt_buf,
                            PS_DPM_OPT_V6_HDR_LEN + PS_DPM_OPT_PORT_INFO_LEN);
      if(buf_len < PS_DPM_OPT_V6_HDR_LEN)
      {
        PS_DPM_DL_INC_STATS(bearer_ptr,stats.dropped.pkts.no_bytes, 1);
        path_type = PS_DPM_DL_OPT_NO_PATH;
        break;
      }

      if ((PS_DPM_OPT_V6_HDR_LEN + PS_DPM_OPT_PORT_INFO_LEN) > buf_len)
      {
        PS_DPM_DL_INC_STATS(bearer_ptr,stats.legacy_path.pkts.no_bytes, 1);
        path_type = PS_DPM_DL_OPT_LEGACY_PATH;
        break;
      }

      /*---------------------------------------------------------------------
        Explicit frag check is not needed for IPv6 as all non TCP/UDP packets
        are sent in legacy path
      ---------------------------------------------------------------------*/

      protocol  = *(pkt_buf + PS_DPM_OPT_V6_NXT_HDR_OFFSET);
      total_len =
        ps_htons(*((uint16 *)(pkt_buf + PS_DPM_OPT_V6_TOTAL_LEN_OFFSET)));
      total_len += PS_DPM_OPT_V6_HDR_LEN;
      ip_hdr_len = PS_DPM_OPT_V6_HDR_LEN;

      /*---------------------------------------------------------------------
        Take legacy path for IP packets destined to linklocal address
      ---------------------------------------------------------------------*/
      if (PS_IN6_IS_PREFIX_LINKLOCAL
          (
            *(uint32 *) (pkt_buf + PS_DPM_OPT_V6_DEST_ADDR_OFFSET))
          )
      {   
        PS_DPM_DL_INC_STATS(bearer_ptr,stats.legacy_path.pkts.link_local, 1);
        PS_DPM_DL_INC_STATS(bearer_ptr,stats.legacy_path.bytes.link_local, total_len);      
        path_type = PS_DPM_DL_OPT_LEGACY_PATH;
        break;
      }
    }
    else
    {
      LOG_MSG_ERROR_INT_1("ps_dpm_dl_opti_rx_pkt_internal(): Invalid IP version %d, dropping packet",
                      version);
      PS_DPM_DL_INC_STATS(bearer_ptr,stats.dropped.pkts.invalid_ip_version, 1);
      PS_DPM_DL_INC_STATS(bearer_ptr,stats.dropped.bytes.invalid_ip_version, total_len);                      
      path_type = PS_DPM_DL_OPT_NO_PATH;
      break;
    }

    /*-----------------------------------------------------------------------
      Ship all packets other than TCP and UDP to legacy path
    -----------------------------------------------------------------------*/
    if (protocol != PS_IPPROTO_TCP && protocol != PS_IPPROTO_UDP)
    {
      PS_DPM_DL_INC_STATS(bearer_ptr,stats.legacy_path.pkts.non_tcp_udp, 1);
      PS_DPM_DL_INC_STATS(bearer_ptr,stats.legacy_path.bytes.non_tcp_udp, total_len);     
      path_type = PS_DPM_DL_OPT_LEGACY_PATH;
      break;
    }

    /*-----------------------------------------------------------------------
      Extract source and destination ports and run optimized socket filters
      to determine if packet needs to be sent in legacy path
    -----------------------------------------------------------------------*/
    src_port = ps_ntohs(*((uint16 *)(pkt_buf + ip_hdr_len)));
    dst_port = ps_ntohs(*((uint16 *)(pkt_buf + ip_hdr_len + 2)));

    filter_ptr = PS_IFACEI_GET_OPT_FILTER_BASE(um_base_iface_ptr);

    if (ps_dl_opti_process_filters(filter_ptr,
                                   src_port,
                                   dst_port,
                                   protocol,
                                   &filter_result,
                                   PS_IFACE_MAX_OPT_FILTER_ENTRIES))
    {
      PS_DPM_DL_INC_STATS(bearer_ptr,stats.legacy_path.pkts.opt_filter_matched, 1);
      PS_DPM_DL_INC_STATS(bearer_ptr,stats.legacy_path.bytes.opt_filter_matched, total_len);         
      path_type = PS_DPM_DL_OPT_LEGACY_PATH;
      break;
    }

    if (ps_dl_opti_process_filters(global_dl_opt_fltr_arr,
                                   src_port,
                                   dst_port,
                                   protocol,
                                   &filter_result,
                                   MAX_GLOBAL_OPT_FILTER_ENTRIES))
    {
      PS_DPM_DL_INC_STATS(bearer_ptr,stats.legacy_path.pkts.opt_filter_matched, 1);
      PS_DPM_DL_INC_STATS(bearer_ptr,stats.legacy_path.bytes.opt_filter_matched, total_len);        
      path_type = PS_DPM_DL_OPT_LEGACY_PATH;
      break;
    }
    

    /*---------------------------------------------------------------------
      Take legacy path if Um iface is not bridged
    ---------------------------------------------------------------------*/
    bridge_iface_ptr = PS_IFACEI_GET_OPT_RM_IFACE(um_base_iface_ptr);
    
    if (!PS_IFACE_IS_VALID(bridge_iface_ptr))
    {
      PS_DPM_DL_INC_STATS(bearer_ptr,stats.legacy_path.pkts.rm_iface_null, 1);
      PS_DPM_DL_INC_STATS(bearer_ptr,stats.legacy_path.bytes.rm_iface_null, total_len);       
      path_type = PS_DPM_DL_OPT_LEGACY_PATH;
      break;
    }    

    /*-----------------------------------------------------------------------
      Increment Rx stats on Um iface
    -----------------------------------------------------------------------*/
    um_base_iface_ptr->iface_i_stats.pkts_rx++;
    um_base_iface_ptr->iface_i_stats.bytes_rx += total_len;
    
    /*-----------------------------------------------------------------------
       DPL will not log the packet if ADPL is enabled (PDN sharing with SW path)
    -----------------------------------------------------------------------*/
    if (FALSE == PS_DPM_HW_IS_DPL_ENABLED())
    {
      DPL_LOG_NETWORK_RX_PACKET(um_base_iface_ptr,
                                *pkt_ref_ptr,
                                DPL_IID_NETPROT_IP);
    }
    /*-----------------------------------------------------------------------
      Increment Tx stats on Rm iface
    -----------------------------------------------------------------------*/
    bridge_iface_ptr->iface_i_stats.pkts_tx++;
    bridge_iface_ptr->iface_i_stats.bytes_tx += total_len;

    /*-----------------------------------------------------------------------
      Log packet on the Rm iface
    -----------------------------------------------------------------------*/
    if (FALSE == PS_DPM_HW_IS_DPL_ENABLED())
    {
      DPL_LOG_NETWORK_TX_PACKET(bridge_iface_ptr,
                                *pkt_ref_ptr,
                                DPL_IID_NETPROT_IP);
    }
    /*-----------------------------------------------------------------------
      Execute MBIM filters for each packet if installed
    -----------------------------------------------------------------------*/
    if (PS_IFACEI_NUM_FILTERS(bridge_iface_ptr, IP_FLTR_CLIENT_MBIM) > 0)
    {
      if (PS_IFACE_IPFLTR_NOMATCH ==
            ps_iface_ipfltr_mbim_execute(bridge_iface_ptr,
                                         IP_FLTR_CLIENT_MBIM,
                                         *pkt_ref_ptr))
      {
        LOG_MSG_DATA_PATH_INFO_0("MBIM Filtering applied, Filter didnt match");

        /*-------------------------------------------------------------------
          powersave_restrictive == TRUE :
           Whitelist filter, Send the packet further up if filter match else
           drop the packet
        -------------------------------------------------------------------*/
        if (bridge_iface_ptr->powersave_restrictive == TRUE)
        {
          LOG_MSG_INFO1_0("ps_dpm_dl_opti_rx_pkt_internal(): Dropping packet "
                          "as powersave restrictive flag is true and "
                          "MBIM filters didnt match"); 
          PS_DPM_DL_INC_STATS(bearer_ptr,stats.dropped.pkts.mbim_filter, 1);
          PS_DPM_DL_INC_STATS(bearer_ptr,stats.dropped.bytes.mbim_filter, total_len);                         
          path_type = PS_DPM_DL_OPT_NO_PATH;
          return_value   = TRUE;
          break;
        }
      }
      else  //filter matched.
      {
        LOG_MSG_DATA_PATH_INFO_0("MBIM Filtering applied, Filter matched");

        /*-------------------------------------------------------------------
          powersave_restrictive == FALSE :
           Blacklist filter, Drop the packet if filter match else send up the
           packet further up in data path
        -------------------------------------------------------------------*/
        if (bridge_iface_ptr->powersave_restrictive == FALSE)
        {
          LOG_MSG_INFO1_0("ps_dpm_dl_opti_rx_pkt_internal(): Dropping packet "
                          "as powersave restrictive flag is false and "
                          "MBIM filters matched");
          PS_DPM_DL_INC_STATS(bearer_ptr,stats.dropped.pkts.powersave_filter, 1);
          PS_DPM_DL_INC_STATS(bearer_ptr,stats.dropped.bytes.powersave_filter, total_len);                                     
          path_type = PS_DPM_DL_OPT_NO_PATH;
          return_value   = TRUE;
          break;
        }
      }
    }

   /*-----------------------------------------------------------------------
      Use tx_cmd_info to determine if Rm is in Ethernet mode

      For IP mode, tx cmd info is actually the DPM info itself where as for
      Ethernet mode, it is LAN LLC instance. Since addresses are not in the
      range of 0 - LAN_LLE_MAX, if tx_cmd_info is above LAN_LLE_MAX, it can
      be inferred that Rm is in Ethernet mode

      If Rm is in Ethernet mode, get DPM info using Um iface and add
      Ethernet header to the IP packet
    -----------------------------------------------------------------------*/
    dpm_rm_info_ptr = ps_dpm_get_dpm_rm_info_by_iface(bridge_iface_ptr);
    if (NULL == dpm_rm_info_ptr)
    {
      PS_DPM_DL_INC_STATS(bearer_ptr,stats.dropped.pkts.dpm_rm_null, 1);
      PS_DPM_DL_INC_STATS(bearer_ptr,stats.dropped.bytes.dpm_rm_null, total_len);       
      path_type = PS_DPM_DL_OPT_NO_PATH;
      break;
    }
    
    if (RMNET_ETHERNET_MODE == dpm_rm_info_ptr->link_prot)
    {
      eth_hdr_ptr = (version == PS_DPM_OPT_V4_VERSION)
                      ? dpm_rm_info_ptr->v4_eth_hdr_ptr
                      : dpm_rm_info_ptr->v6_eth_hdr_ptr;

      /*---------------------------------------------------------------------
        Pushdown Ethernet hdr
      ---------------------------------------------------------------------*/
      ret_val_num = dsm_pushdown(pkt_ref_ptr,
                             eth_hdr_ptr,
                             sizeof(llc_frm_packed_mode_zero_type),
                             DSM_DS_SMALL_ITEM_POOL);

      if (ret_val_num != sizeof(llc_frm_packed_mode_zero_type))
      {
        LOG_MSG_ERROR_INT_0("ps_dpm_dl_opti_rx_pkt_internal(): "
                        "Couldn't push down eth hdr, dropping packet");
        PS_DPM_DL_INC_STATS(bearer_ptr,stats.dropped.pkts.ether_pkt_err, 1);
        PS_DPM_DL_INC_STATS(bearer_ptr,stats.dropped.bytes.ether_pkt_err, total_len);                           
        path_type = PS_DPM_DL_OPT_NO_PATH;
        break;
      }
    }

    /* ------------------------------------------------------------------------
      Execute powersave filters on incoming packet. If there is a match send
      it for upperlayers. Exit powersave mode if auto exit mode is enabled.

      If there is no match, drop the paket and send an indication for 
      fast dormancy algorithm to kick in.
    -------------------------------------------------------------------------*/
    if( PS_IFACE_GET_POWERSAVE_FILTERING_MODE(bridge_iface_ptr) )
    {
      if( ps_rx_pkt_info_generate(pkt_ref_ptr, rx_mi_ptr, &pkt_info_errno) == -1 )
      {
        DPL_LOG_NETWORK_TX_DROPPED_PACKET
         (bridge_iface_ptr,*pkt_ref_ptr,DPL_IID_NETPROT_IP)
         
        dsm_free_packet(pkt_ref_ptr);
        PS_DPM_DL_INC_STATS(bearer_ptr,stats.dropped.pkts.pkt_info_err, 1);
        PS_DPM_DL_INC_STATS(bearer_ptr,stats.dropped.bytes.pkt_info_err, total_len);
        um_base_iface_ptr->iface_i_stats.pkts_dropped_rx++;
        bridge_iface_ptr->iface_i_stats.pkts_dropped_tx++;

        LOG_MSG_ERROR_INT_0("ps_dpm_dl_opti_rx_pkt_internal():Not able to generate"
                        " rx_mi_ptr info, dropping packet");
        PS_RX_META_INFO_FREE(rx_mi_ptr);

        PS_LEAVE_CRIT_SECTION(&ps_dpm_crit_section);
        PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);
        return FALSE;
      }

      ip_pkt_info_ptr = &PS_RX_META_GET_PKT_INFO(*rx_mi_ptr);
      if( PS_IFACE_IPFLTR_NOMATCH == 
            ps_iface_ipfltr_execute( bridge_iface_ptr, 
                                     IP_FLTR_CLIENT_POWERSAVE, 
                                     ip_pkt_info_ptr))
      {
        //call the drop packet indication to run the fast dormancy delay timer algorithm
        um_base_iface_ptr->iface_i_stats.pkts_dropped_rx++;
        bridge_iface_ptr->iface_i_stats.pkts_dropped_tx++;
        
        if(!ps_iface_is_first_powersave_packet_drop_reported())
        {
          ps_iface_set_first_powersave_packet_drop_reported(TRUE);
          ps_sys_fast_dormancy_pkt_dropped_ind(bridge_iface_ptr);
        }
        
        LOG_MSG_ERROR_INT_0("ps_dpm_dl_opti_rx_pkt_internal():Powersave filters didn't match,"
                        "dropping packet");

        PS_DPM_DL_INC_STATS(bearer_ptr,stats.dropped.pkts.powersave_filter, 1);
        PS_DPM_DL_INC_STATS(bearer_ptr,stats.dropped.bytes.powersave_filter, total_len);        
        DPL_LOG_NETWORK_TX_DROPPED_PACKET
          (bridge_iface_ptr,*pkt_ref_ptr,DPL_IID_NETPROT_IP)
        dsm_free_packet(pkt_ref_ptr);
        ret_status = FALSE;
      }
      else
      { 
        PS_DPM_DL_INC_STATS(bearer_ptr,stats.total_dl_opt_path_pkts, 1);
        PS_DPM_DL_INC_STATS(bearer_ptr,stats.total_dl_opt_path_bytes, total_len);        
        #ifndef TEST_FRAMEWORK
          dsm_enqueue(dpm_rm_info_ptr->sio_tx_wm_ptr, pkt_ref_ptr);
        #else
          #error code not present
#endif /* TEST_FRAMEWORK */
        
        if(PS_IFACE_GET_POWERSAVE_FILTERING_AUTO_EXIT_MODE(bridge_iface_ptr))
        {
          if( -1 == ps_iface_set_powersave_filtering_mode( bridge_iface_ptr,
                                                           FALSE,
                                                           &ps_errno) )
          {
            
            LOG_MSG_ERROR_INT_0("ps_dpm_dl_opti_rx_pkt_internal:" 
              "powersave filtering mode change failed ");
          }
        }
      }
      PS_RX_META_INFO_FREE(rx_mi_ptr);
    }
    else
    {
      PS_DPM_DL_INC_STATS(bearer_ptr,stats.total_dl_opt_path_pkts, 1);
      PS_DPM_DL_INC_STATS(bearer_ptr,stats.total_dl_opt_path_bytes, total_len);      
      PS_DPMI_INC_CONT_VALID_PKT_CNT( bearer_ptr , ps_dpm_cont_err_pkt_info ); 
      #ifndef TEST_FRAMEWORK
        dsm_enqueue(dpm_rm_info_ptr->sio_tx_wm_ptr, pkt_ref_ptr);
      #else
        #error code not present
#endif /* TEST_FRAMEWORK */
    }

    PS_RX_META_INFO_FREE(rx_mi_ptr);

    PS_LEAVE_CRIT_SECTION(&ps_dpm_crit_section);
    PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

    return ret_status;
  } while (0);

  /*-------------------------------------------------------------------------
    Leave CS before tx_cmd to avoid possible deadlock issues
  -------------------------------------------------------------------------*/
  PS_LEAVE_CRIT_SECTION(&ps_dpm_crit_section);  
  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);
  
  /*-------------------------------------------------------------------------
    Process the packet based on 'path_type'

    If 'path_type' is LEGACY, call ps_iface_input. Otherwise, free the packet
  -------------------------------------------------------------------------*/
  if (PS_DPM_DL_OPT_LEGACY_PATH == path_type)
  {
    if (NULL == *rx_mi_ptr)
    {
      PS_RX_META_INFO_GET(*rx_mi_ptr);
      if (NULL == *rx_mi_ptr)
      {
        LOG_MSG_ERROR_INT_0("ps_dpm_dl_opti_rx_pkt_internal() Failed to "
                        "get rx meta info buf,dropping packet");
        PS_DPM_DL_INC_STATS(bearer_ptr,stats.dropped.pkts.pkt_info_err, 1);
        PS_DPM_DL_INC_STATS(bearer_ptr,stats.dropped.bytes.pkt_info_err, total_len); 
        um_base_iface_ptr->iface_i_stats.pkts_dropped_rx++;
        if (NULL != bridge_iface_ptr)
        {
          bridge_iface_ptr->iface_i_stats.pkts_dropped_tx++;  
        }         
        DPL_LOG_NETWORK_RX_DROPPED_PACKET
          (um_base_iface_ptr,*pkt_ref_ptr,DPL_IID_NETPROT_IP);
        dsm_free_packet(pkt_ref_ptr);
        return FALSE;
      }        
    } 
    PS_DPMI_INC_CONT_VALID_PKT_CNT( bearer_ptr , ps_dpm_cont_err_pkt_info ); 

    LOG_MSG_DATA_PATH_INFO_1("ps_dpm_dl_opti_rx_pkt_internal() filter_result %d",
                              filter_result);

    /*---------------------------------------------------------------------
      Checksum is offloaded to IPA hardware so that protocols dont
      have to recalculate checksum value.
    ---------------------------------------------------------------------*/ 
    ip_pkt_info_ptr = &PS_RX_META_GET_PKT_INFO(*rx_mi_ptr); 
      
    PS_DPM_HW_SET_CKSUM_OFFLOAD_VALUES(ip_pkt_info_ptr, *pkt_ref_ptr);
    
    LOG_MSG_DATA_PATH_INFO_1("ps_dpm_dl_opti_rx_pkt_internal() filter_result %d",
                              filter_result);
    
    PS_RX_META_SET_FILTER_RESULT
    (
      *rx_mi_ptr,
      IP_FLTR_CLIENT_SOCKETS,
      filter_result
    );
      
    PS_RX_META_SET_SOCKET_FI_SOURCE
    (
      *rx_mi_ptr, 
      PS_IFACE_IPFLTR_DL_SOCKET_RESULT_SOURCE_DPM
    );

    if (TRUE == is_legacy_proc_needed)
    {  
      PS_DPM_DL_INC_STATS(bearer_ptr,stats.total_dl_legacy_path_pkts, 1);
      PS_DPM_DL_INC_STATS(bearer_ptr,stats.total_dl_legacy_path_pkts, total_len);    
#ifdef TEST_FRAMEWORK
      #error code not present
#endif
      (void) ps_iface_input_internal(um_base_iface_ptr,
                                     pkt_ref_ptr,
                                     *rx_mi_ptr,
                                     NULL);
      return TRUE;
    }
    else
    {
      return FALSE;
    }
  }
  else
  {
    if (PS_IFACE_IS_VALID(um_base_iface_ptr))
    {
      DPL_LOG_NETWORK_RX_DROPPED_PACKET
        (um_base_iface_ptr,*pkt_ref_ptr,DPL_IID_NETPROT_IP);
      um_base_iface_ptr->iface_i_stats.pkts_dropped_rx++;
    }
    if (NULL != bridge_iface_ptr)
    {
      bridge_iface_ptr->iface_i_stats.pkts_dropped_tx++;  
    } 
          
    PS_DPMI_INC_CONT_PKT_DROP_CNT( bearer_ptr , ps_dpm_cont_err_pkt_info );
    dsm_free_packet(pkt_ref_ptr);
    PS_RX_META_INFO_FREE(rx_mi_ptr);
    return return_value;
  }
} /* ps_dpm_dl_opti_rx_pkt_internal() */

#ifdef FEATURE_DATA_PS_464XLAT
static boolean ps_dpm_dl_opti_clat_rx_pkt_internal
(
  ps_iface_type            * v4_iface_ptr,
  ps_iface_type            * v6_iface_ptr,
  dsm_item_type           ** pkt_ref_ptr,
  ps_rx_meta_info_type    ** rx_mi_ptr,
  boolean                    is_legacy_proc_needed,
  ps_dpm_bearer_info_type  * bearer_ptr
)
{

#ifndef FEATURE_DATA_LLVM_7x
  uint8  pkt_buf[PS_DPM_OPT_EXP_HEADER_LEN_DL + 1] ALIGN(8); //__attribute__((aligned(4)));
#else
  uint8  pkt_buf[PS_DPM_OPT_EXP_HEADER_LEN_DL + 1] ALIGN(8); // Need review
#endif /* FEATURE_DATA_LLVM_7x */

  ps_dpm_rm_info_type              * dpm_rm_info_ptr = NULL;
  ps_iface_type                    * um_base_iface_ptr = NULL;
  ps_iface_type                    * bridge_iface_ptr = NULL;
  ps_dl_opt_filter_spec_type       * filter_ptr;
  llc_frm_packed_mode_zero_type    * eth_hdr_ptr;
  int32                              ret_val;
  boolean                            ret_status = TRUE;
  uint16                             total_len = 0;
  uint16                             dst_port;
  uint16                             src_port;
  uint16                             buf_len;
  uint16                             ip_hdr_len;
  uint8                              version;
  uint8                              protocol;
  uint8                              ip_hdr_first_byte;
  ps_dpm_dl_opt_path_type            path_type = PS_DPM_DL_OPT_NO_PATH;
  sint15                             ps_errno;
  errno_enum_type                    pkt_info_errno;
  ip_pkt_info_type                 * ip_pkt_info_ptr = NULL;
  ps_dpm_um_info_type              * dpm_um_info_ptr = NULL;
  struct ps_in6_addr                 src_v6_addr;
  struct ps_in6_addr                 dst_v6_addr;
  ps_dpm_clat_pdn_cntxt_info_type  * clat_pdn_cntxt = NULL;
  int16                              ds_errno = DS_ENOERR;
  uint16                             filter_result = 0xFFFF;
  boolean                            is_pkt_trnsltd = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);
  PS_ENTER_CRIT_SECTION(&ps_dpm_crit_section);

  do
  {
    /*-----------------------------------------------------------------------
      Peek in to the first byte to determine version of the IP packet and
      process the packet appropriately
    -----------------------------------------------------------------------*/
    (void) dsm_extract(*pkt_ref_ptr, 0, &ip_hdr_first_byte, 1);

    version = ip_hdr_first_byte & PS_DPM_OPT_IP_VERSION_MASK;

    um_base_iface_ptr = v6_iface_ptr;

    if (!PS_IFACE_IS_VALID(um_base_iface_ptr))
    {
      PS_DPM_DL_INC_STATS(bearer_ptr, stats.dropped.pkts.invalid_iface, 1);
      LOG_MSG_ERROR_INT_0("ps_dpm_dl_opti_clat_rx_pkt_internal : Invalid iface"
                      " dropping packet");
      path_type = PS_DPM_DL_OPT_NO_PATH;
      break;
    }
      
    /*-------------------------------------------------------------------
      Handle V4 packet that got translated by IPA from V6
    -------------------------------------------------------------------*/           
    if (version == PS_DPM_OPT_V4_VERSION)
    {
      clat_pdn_cntxt =  
        (ps_dpm_clat_pdn_cntxt_info_type *)um_base_iface_ptr->clat_pdn_cntxt;  

      if (NULL == clat_pdn_cntxt)
      {
        LOG_MSG_ERROR_INT_0("ps_dpm_dl_opti_clat_rx_pkt_internal : "
                        "Invalid clat pdn cntxt, dropping packet"); 
        PS_DPM_DL_INC_STATS(bearer_ptr,stats.dropped.pkts.clat_pdn_null, 1);
        PS_DPM_DL_INC_STATS(bearer_ptr,stats.dropped.bytes.clat_pdn_null, total_len);        
        path_type = PS_DPM_DL_OPT_NO_PATH;
        break;
      }          

      if( ((ip_hdr_first_byte & PS_DPM_OPT_IP_HDR_LEN_MASK) << 2) <
                    PS_DPM_OPT_V4_HDR_LEN )
      {
        PS_DPM_DL_INC_STATS(bearer_ptr, stats.legacy_path.pkts.no_bytes, 1);
        path_type = PS_DPM_DL_OPT_NO_PATH;
        break;
      }
      /*-------------------------------------------------------------------
        Set um base iface to clat iface as v4 packet cannot be sent
        on v6 iface
      -------------------------------------------------------------------*/           
      um_base_iface_ptr = clat_pdn_cntxt->clat_iface_ptr;
      if (!PS_IFACE_IS_VALID(um_base_iface_ptr))
      {
        LOG_MSG_ERROR_INT_0("ps_dpm_dl_opti_rx_pkt_internal: CLAT IFACE null, dropping packet");  
        PS_DPM_DL_INC_STATS(bearer_ptr,stats.dropped.pkts.invalid_iface, 1);
        PS_DPM_DL_INC_STATS(bearer_ptr,stats.dropped.bytes.invalid_iface, total_len);        
        path_type = PS_DPM_DL_OPT_NO_PATH;
        break;
      }
    }

    if (version == PS_DPM_OPT_V6_VERSION)  /* IPv6 packet */
    {
      /*---------------------------------------------------------------------
        Extract IP header and port information from the IP packet
      ---------------------------------------------------------------------*/
      buf_len = dsm_extract(*pkt_ref_ptr,
                            0,
                            pkt_buf,
                            PS_DPM_OPT_V6_HDR_LEN + PS_DPM_OPT_PORT_INFO_LEN);
                            
      if(buf_len < PS_DPM_OPT_V6_HDR_LEN)
      {
        PS_DPM_DL_INC_STATS(bearer_ptr, stats.legacy_path.pkts.no_bytes, 1);
        path_type = PS_DPM_DL_OPT_NO_PATH;
        break;
      }

      if ((PS_DPM_OPT_V6_HDR_LEN + PS_DPM_OPT_PORT_INFO_LEN) > buf_len)
      {
        PS_DPM_DL_INC_STATS(bearer_ptr, stats.legacy_path.pkts.no_bytes, 1);      
        path_type = PS_DPM_DL_OPT_LEGACY_PATH;
        break;
      }

      /*---------------------------------------------------------------------
        Store total pkt length to update stats later
      ---------------------------------------------------------------------*/
      total_len = 
            ps_htons(*((uint16 *)(pkt_buf + PS_DPM_OPT_V6_TOTAL_LEN_OFFSET)));
      total_len += PS_DPM_OPT_V6_HDR_LEN;
      

      /*---------------------------------------------------------------------
        Take legacy path for IP packets destined to linklocal address
      ---------------------------------------------------------------------*/
      if (PS_IN6_IS_PREFIX_LINKLOCAL
          (
            *(uint32 *) (pkt_buf + PS_DPM_OPT_V6_DEST_ADDR_OFFSET))
          )
      {
        PS_DPM_DL_INC_STATS(bearer_ptr,stats.legacy_path.pkts.link_local, 1);
        PS_DPM_DL_INC_STATS(bearer_ptr,stats.legacy_path.bytes.link_local, total_len);        
        path_type = PS_DPM_DL_OPT_LEGACY_PATH;
        break;
      }      

      /*---------------------------------------------------------------------
         1. check the CLAT capability on base_um_iface_ptr 
         2. Get dpm_um_info_ptr from base um iface ptr 
         3. Get dpm_rm_info_ptr from dpm_um_info_ptr
         4. If src, dst addr matches, do the translation.
         5. If translation is successful then bridge iface will be v4 iface ptr.
         6. Set um base iface to clat v4 iface as v4 packet cannote be sent
            on v6 iface
         7. If translation is not successful the bridge iface will be v6 iface ptr
      ---------------------------------------------------------------------*/                                
      /*---------------------------------------------------------------------
        1. Check the CLAT capability on base_um_iface_ptr 
      ---------------------------------------------------------------------*/                            
      if (PS_IFACE_GET_CAPABILITY(um_base_iface_ptr, 
                                  PS_IFACE_CAPABILITY_CLAT_CAPABLE))
      {      
        /*-------------------------------------------------------------------
          2.Get dpm_um_info_ptr from base um iface ptr 
        -------------------------------------------------------------------*/        
        dpm_um_info_ptr = ps_dpmi_get_dpm_um_info(um_base_iface_ptr);
        
        /*-------------------------------------------------------------------
          3.Get dpm_rm_info_ptr from dpm_um_info_ptr
        -------------------------------------------------------------------*/          
        if (NULL != dpm_um_info_ptr)
        {
          dpm_rm_info_ptr = dpm_um_info_ptr->rm_info_ptr;
        }
        
        src_v6_addr = *((struct ps_in6_addr *)(pkt_buf + PS_DPM_OPT_V6_SRC_ADDR_OFFSET));
        dst_v6_addr = *((struct ps_in6_addr *)(pkt_buf + PS_DPM_OPT_V6_DEST_ADDR_OFFSET));

        /*-------------------------------------------------------------------
          4.If src, dst addr matches, do the translation.
        -------------------------------------------------------------------*/      
        clat_pdn_cntxt =  
          (ps_dpm_clat_pdn_cntxt_info_type *)um_base_iface_ptr->clat_pdn_cntxt;  
          
        if (NULL == clat_pdn_cntxt)
        {
          LOG_MSG_ERROR_INT_0("ps_dpm_dl_opti_clat_rx_pkt_internal : "
                          "Invalid clat pdn cntxt, dropping packet");
          PS_DPM_DL_INC_STATS(bearer_ptr,stats.dropped.pkts.clat_pdn_null, 1);
          PS_DPM_DL_INC_STATS(bearer_ptr,stats.dropped.bytes.clat_pdn_null, total_len);                            
          path_type = PS_DPM_DL_OPT_NO_PATH;
          break;
        }          
          
        if (IN6_ARE_PREFIX_EQUAL(&(clat_pdn_cntxt->v6_dev_addr),
                                 &dst_v6_addr,
                                 clat_pdn_cntxt->v6_dev_addr_prefix_len) &&
           (IN6_ARE_PREFIX_EQUAL(&(clat_pdn_cntxt->v6_plat_prefix),                      
                                  &src_v6_addr,
                                  clat_pdn_cntxt->v6_plat_prefix_len)))
        {
          /*-------------------------------------------------------------------
            Increment Rx stats on Um iface
          -------------------------------------------------------------------*/
          um_base_iface_ptr->iface_i_stats.pkts_rx++;
          um_base_iface_ptr->iface_i_stats.bytes_rx += total_len;
              
          /*-------------------------------------------------------------------
            Log the packet on the Um iface before translating
          -------------------------------------------------------------------*/
          if( FALSE == dpm_um_info_ptr->is_bridged_with_hw )
          {          
            DPL_LOG_NETWORK_RX_PACKET(um_base_iface_ptr,
                                      *pkt_ref_ptr,
                                      DPL_IID_NETPROT_IP);
          }
        
          if( -1 == ps_clat_ip6_to_ip(pkt_ref_ptr,
                                      um_base_iface_ptr,
                                      clat_pdn_cntxt->client_pdn_handle))
          {          
            LOG_MSG_ERROR_INT_0("ps_dpm_dl_opti_clat_rx_pkt_internal : "
                            "IP6 to IP conversion failed. Not dropping pkt");          
            ds_errno = DS_EINVAL; /* To make sure we dont drop the pkt since protcols 
                                     would have already dropped.*/            
            PS_DPM_DL_INC_STATS(bearer_ptr,stats.dropped.pkts.clat_translate_failure, 1);
            PS_DPM_DL_INC_STATS(bearer_ptr,stats.dropped.bytes.clat_translate_failure, total_len);            
            path_type = PS_DPM_DL_OPT_NO_PATH;
            break;
          }
 
          /*-------------------------------------------------------------------
            5. If translation is successful then bridge iface will be 
               v4 iface ptr for tethered call
          -------------------------------------------------------------------*/ 
          if (NULL != dpm_rm_info_ptr)
          {
            bridge_iface_ptr = dpm_rm_info_ptr->v4_iface_ptr;
          }

          /*-------------------------------------------------------------------
            6. Set um base iface to clat iface as v4 packet cannoted be sent
               on v6 iface
          -------------------------------------------------------------------*/           
          um_base_iface_ptr = clat_pdn_cntxt->clat_iface_ptr;
          if (!PS_IFACE_IS_VALID(um_base_iface_ptr))
          {
            LOG_MSG_ERROR_INT_0("ps_dpm_dl_opti_rx_pkt_internal: CLAT IFACE null"
                            " dropping packet");
            PS_DPM_DL_INC_STATS(bearer_ptr,stats.dropped.pkts.invalid_iface, 1);
            PS_DPM_DL_INC_STATS(bearer_ptr,stats.dropped.bytes.invalid_iface, total_len);                               
            path_type = PS_DPM_DL_OPT_NO_PATH;
            break;
          }
          
          /*-------------------------------------------------------------------
            Update version & is_pkt_trnsltd to indicate that the pkt got
            translated
          -------------------------------------------------------------------*/           
          version = PS_DPM_OPT_V4_VERSION;
          is_pkt_trnsltd = TRUE;
        }
        
        /*-------------------------------------------------------------------
          7. If translation is not successful the bridge iface will be 
             v6 iface ptr 
        -------------------------------------------------------------------*/       
        else
        {
          if (NULL != dpm_rm_info_ptr)
          {
            bridge_iface_ptr = dpm_rm_info_ptr->v6_iface_ptr;
          }
        }
      }

      if (version == PS_DPM_OPT_V6_VERSION)  /* IPv6 packet didnt get translated */
      {  
        /*---------------------------------------------------------------------
          Explicit frag check is not needed for IPv6 as all non TCP/UDP packets
          are sent in legacy path
        ---------------------------------------------------------------------*/
        protocol  = *(pkt_buf + PS_DPM_OPT_V6_NXT_HDR_OFFSET);
        ip_hdr_len = PS_DPM_OPT_V6_HDR_LEN;
      }
    }

    if (version == PS_DPM_OPT_V4_VERSION)  /* Translated V6 to V4 packet */
    {
      /*---------------------------------------------------------------------
        Fetch IHL and use it to extract IP header and port information from
        the IP packet
      ---------------------------------------------------------------------*/
      buf_len = dsm_extract(*pkt_ref_ptr,
                            0,
                            pkt_buf,
                            PS_DPM_OPT_MAX_V4_HDR_LEN + PS_DPM_OPT_PORT_INFO_LEN);
      if ((PS_DPM_OPT_MAX_V4_HDR_LEN + PS_DPM_OPT_PORT_INFO_LEN) > buf_len)
      {
        PS_DPM_DL_INC_STATS(bearer_ptr, stats.legacy_path.pkts.no_bytes, 1);        
        path_type = PS_DPM_DL_OPT_LEGACY_PATH;
        break;
      }  

      /*---------------------------------------------------------------------
        Take legacy path for all IPv4 fragments
      ---------------------------------------------------------------------*/
      if (PS_OPTIMIZED_IP_DL_V4_FRAG_CHECK
          (
            *((uint16 *)(pkt_buf + PS_DPM_OPT_V4_FRAG_OFFSET)))
          )
      {
        PS_DPM_DL_INC_STATS(bearer_ptr,stats.legacy_path.pkts.frag_pkt, 1);
        PS_DPM_DL_INC_STATS(bearer_ptr,stats.legacy_path.bytes.frag_pkt, total_len);           
        path_type = PS_DPM_DL_OPT_LEGACY_PATH;
        break;
      }

      protocol  = *(pkt_buf + PS_DPM_OPT_V4_PROT_FIELD_OFFSET);
      total_len =
        ps_htons(*((uint16 *)(pkt_buf + PS_DPM_OPT_V4_TOTAL_LEN_OFFSET)));
      ip_hdr_len = (*pkt_buf & PS_DPM_OPT_IP_HDR_LEN_MASK) << 2;
    }
    
    if(version != PS_DPM_OPT_V4_VERSION && version != PS_DPM_OPT_V6_VERSION)
    {
      LOG_MSG_ERROR_INT_1("ps_dpm_dl_opti_clat_rx_pkt_internal(): Invalid IP version %d"
                      " dropping packet",version);
      PS_DPM_DL_INC_STATS(bearer_ptr,stats.dropped.pkts.invalid_ip_version, 1);
      PS_DPM_DL_INC_STATS(bearer_ptr,stats.dropped.bytes.invalid_ip_version, total_len);                       
      path_type = PS_DPM_DL_OPT_NO_PATH;
      break;
    }
    
    /*-----------------------------------------------------------------------
      Ship all packets other than TCP and UDP to legacy path
    -----------------------------------------------------------------------*/
    if (protocol != PS_IPPROTO_TCP && protocol != PS_IPPROTO_UDP)
    {
      PS_DPM_DL_INC_STATS(bearer_ptr,stats.legacy_path.pkts.non_tcp_udp, 1);
      PS_DPM_DL_INC_STATS(bearer_ptr,stats.legacy_path.bytes.non_tcp_udp, total_len);       
      path_type = PS_DPM_DL_OPT_LEGACY_PATH;
      break;
    }

    /*-----------------------------------------------------------------------
      Extract source and destination ports and run optimized socket filters
      to determine if packet needs to be sent in legacy path
    -----------------------------------------------------------------------*/
    src_port = ps_ntohs(*((uint16 *)(pkt_buf + ip_hdr_len)));
    dst_port = ps_ntohs(*((uint16 *)(pkt_buf + ip_hdr_len + 2)));

    filter_ptr = PS_IFACEI_GET_OPT_FILTER_BASE(um_base_iface_ptr);

    if (ps_dl_opti_process_filters(filter_ptr,
                                   src_port,
                                   dst_port,
                                   protocol,
                                   &filter_result,
                                   PS_IFACE_MAX_OPT_FILTER_ENTRIES))
    {
      PS_DPM_DL_INC_STATS(bearer_ptr,stats.legacy_path.pkts.opt_filter_matched, 1);
      PS_DPM_DL_INC_STATS(bearer_ptr,stats.legacy_path.bytes.opt_filter_matched, total_len);         
      path_type = PS_DPM_DL_OPT_LEGACY_PATH;
      break;
    }

    if (ps_dl_opti_process_filters(global_dl_opt_fltr_arr,
                                   src_port,
                                   dst_port,
                                   protocol,
                                   &filter_result,
                                   MAX_GLOBAL_OPT_FILTER_ENTRIES))
    {
      PS_DPM_DL_INC_STATS(bearer_ptr,stats.legacy_path.pkts.opt_filter_matched, 1);
      PS_DPM_DL_INC_STATS(bearer_ptr,stats.legacy_path.bytes.opt_filter_matched, total_len);        
      path_type = PS_DPM_DL_OPT_LEGACY_PATH;
      break;
    }

    /*-----------------------------------------------------------------------
      Take legacy path if Um iface is not bridged
    -----------------------------------------------------------------------*/
    if (NULL == bridge_iface_ptr)
    {
      bridge_iface_ptr = PS_IFACEI_GET_OPT_RM_IFACE(um_base_iface_ptr);
    }

    if (!PS_IFACE_IS_VALID(bridge_iface_ptr))
    {
      PS_DPM_DL_INC_STATS(bearer_ptr,stats.legacy_path.pkts.rm_iface_null, 1);
      PS_DPM_DL_INC_STATS(bearer_ptr,stats.legacy_path.bytes.rm_iface_null, total_len);        
      path_type = PS_DPM_DL_OPT_LEGACY_PATH;
      break;
    }

    if (is_pkt_trnsltd)
    {
      /*----------------------------------------------------------------------- 
        If the pkt got translated then it is already logged. Hence this is NA.
        Increment Rx stats on Um iface
      -----------------------------------------------------------------------*/
      um_base_iface_ptr->iface_i_stats.pkts_rx++;
      um_base_iface_ptr->iface_i_stats.bytes_rx += total_len;

      /*-----------------------------------------------------------------------
        Log the packet on the Um iface
      -----------------------------------------------------------------------*/
      if (FALSE == PS_DPM_HW_IS_DPL_ENABLED())
      {
      
        DPL_LOG_NETWORK_RX_PACKET(um_base_iface_ptr,
                                  *pkt_ref_ptr,
                                  DPL_IID_NETPROT_IP);
      }
    }

    /*-----------------------------------------------------------------------
      Increment Tx stats on Rm iface
    -----------------------------------------------------------------------*/
    bridge_iface_ptr->iface_i_stats.pkts_tx++;
    bridge_iface_ptr->iface_i_stats.bytes_tx += total_len;

    /*-----------------------------------------------------------------------
      Log packet on the Rm iface
    -----------------------------------------------------------------------*/
    if (FALSE == PS_DPM_HW_IS_DPL_ENABLED())
    {
      DPL_LOG_NETWORK_TX_PACKET(bridge_iface_ptr,
                                *pkt_ref_ptr,
                                DPL_IID_NETPROT_IP);
    }

    /*-----------------------------------------------------------------------
      Use tx_cmd_info to determine if Rm is in Ethernet mode

      For IP mode, tx cmd info is actually the DPM info itself where as for
      Ethernet mode, it is LAN LLC instance. Since addresses are not in the
      range of 0 - LAN_LLE_MAX, if tx_cmd_info is above LAN_LLE_MAX, it can
      be inferred that Rm is in Ethernet mode

      If Rm is in Ethernet mode, get DPM info using Um iface and add
      Ethernet header to the IP packet
    -----------------------------------------------------------------------*/
    dpm_rm_info_ptr = ps_dpm_get_dpm_rm_info_by_iface(bridge_iface_ptr);
    if (NULL == dpm_rm_info_ptr)
    {
      LOG_MSG_INFO1_0("ps_dpm_dl_opti_clat_rx_pkt_internal(): RM call down");
      PS_DPM_DL_INC_STATS(bearer_ptr,stats.legacy_path.pkts.rm_iface_null, 1);
      PS_DPM_DL_INC_STATS(bearer_ptr,stats.legacy_path.bytes.rm_iface_null, total_len);       
      path_type = PS_DPM_DL_OPT_NO_PATH;
      break;
    }
    
    if (RMNET_ETHERNET_MODE == dpm_rm_info_ptr->link_prot)
    {
      eth_hdr_ptr = (version == PS_DPM_OPT_V4_VERSION)
                      ? dpm_rm_info_ptr->v4_eth_hdr_ptr
                      : dpm_rm_info_ptr->v6_eth_hdr_ptr;

      /*---------------------------------------------------------------------
        Pushdown Ethernet hdr
      ---------------------------------------------------------------------*/
      ret_val = dsm_pushdown(pkt_ref_ptr,
                             eth_hdr_ptr,
                             sizeof(llc_frm_packed_mode_zero_type),
                             DSM_DS_SMALL_ITEM_POOL);

      if (ret_val != sizeof(llc_frm_packed_mode_zero_type))
      {
        LOG_MSG_ERROR_INT_0("ps_dpm_dl_opti_clat_rx_pkt_internal(): "
                        "Couldn't push down eth hdr"); 
        PS_DPM_DL_INC_STATS(bearer_ptr,stats.dropped.pkts.ether_pkt_err, 1);
        PS_DPM_DL_INC_STATS(bearer_ptr,stats.dropped.bytes.ether_pkt_err, total_len);        
        path_type = PS_DPM_DL_OPT_NO_PATH;
        break;
      }
    }

    
    /*-----------------------------------------------------------------------
      Execute MBIM filters for each packet if installed   
    -----------------------------------------------------------------------*/    
    if (PS_IFACEI_NUM_FILTERS(bridge_iface_ptr, IP_FLTR_CLIENT_MBIM) > 0)
    {    
      if (PS_IFACE_IPFLTR_NOMATCH == ps_iface_ipfltr_mbim_execute(bridge_iface_ptr, 
                                                                 IP_FLTR_CLIENT_MBIM,
                                                                 *pkt_ref_ptr))
      {
        LOG_MSG_DATA_PATH_INFO_0("MBIM Filtering applied, Filter didnt match");
        /*-----------------------------------------------------------------------
          powersave_restrictive == TRUE : 
           Whitelist filter, Send the packet further up if filter match else 
           drop the packet    
        -----------------------------------------------------------------------*/    
        if (bridge_iface_ptr->powersave_restrictive == TRUE)
        { 
          PS_LEAVE_CRIT_SECTION(&ps_dpm_crit_section);
          PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);
          LOG_MSG_INFO1_0("Dropping packet as powersave restrictive flag is true"
                          " and mbim filters didnt match");      
          DPL_LOG_NETWORK_TX_DROPPED_PACKET
            (bridge_iface_ptr,*pkt_ref_ptr,DPL_IID_NETPROT_IP);
            
          PS_DPM_DL_INC_STATS(bearer_ptr,stats.dropped.pkts.mbim_filter, 1);
          PS_DPM_DL_INC_STATS(bearer_ptr,stats.dropped.bytes.mbim_filter, total_len); 
          um_base_iface_ptr->iface_i_stats.pkts_dropped_rx++;
          bridge_iface_ptr->iface_i_stats.pkts_dropped_tx++;          
          dsm_free_packet(pkt_ref_ptr);
          return TRUE;
        }
      }
      else  //filter matched.
      {
        LOG_MSG_DATA_PATH_INFO_0("MBIM Filtering applied, Filter matched");
        /*-----------------------------------------------------------------------
          powersave_restrictive == FALSE : 
           Blacklist filter, Drop the packet if filter match else send up the 
           packet further up in data path       
        -----------------------------------------------------------------------*/    
        if (bridge_iface_ptr->powersave_restrictive == FALSE)
        {
          PS_LEAVE_CRIT_SECTION(&ps_dpm_crit_section);
          PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);
          LOG_MSG_INFO1_0("Dropping packet as powersave restrictive flag is false"
                          " and mbim filters matched");  
          DPL_LOG_NETWORK_TX_DROPPED_PACKET
            (bridge_iface_ptr,*pkt_ref_ptr,DPL_IID_NETPROT_IP);
            
          um_base_iface_ptr->iface_i_stats.pkts_dropped_rx++;
          bridge_iface_ptr->iface_i_stats.pkts_dropped_tx++;            
          PS_DPM_DL_INC_STATS(bearer_ptr,stats.dropped.pkts.powersave_filter, 1);
          PS_DPM_DL_INC_STATS(bearer_ptr,stats.dropped.bytes.powersave_filter, total_len);           
          dsm_free_packet(pkt_ref_ptr);
          return TRUE;
        }  
      }    
    }

    /* ------------------------------------------------------------------------
      Execute powersave filters on incoming packet. If there is a match send
      it for upperlayers. Exit powersave mode if auto exit mode is enabled.

      If there is no match, drop the paket and send an indication for 
      fast dormancy algorithm to kick in.
    -------------------------------------------------------------------------*/
    if( PS_IFACE_GET_POWERSAVE_FILTERING_MODE(bridge_iface_ptr))
    {
      if( ps_rx_pkt_info_generate(pkt_ref_ptr, rx_mi_ptr, &pkt_info_errno) == -1 )
      {
        DPL_LOG_NETWORK_TX_DROPPED_PACKET
          (bridge_iface_ptr,*pkt_ref_ptr,DPL_IID_NETPROT_IP);
        dsm_free_packet(pkt_ref_ptr);
        
        PS_DPM_DL_INC_STATS(bearer_ptr,stats.dropped.pkts.pkt_info_err, 1);
        PS_DPM_DL_INC_STATS(bearer_ptr,stats.dropped.bytes.pkt_info_err, total_len); 
        
        um_base_iface_ptr->iface_i_stats.pkts_dropped_rx++;
        bridge_iface_ptr->iface_i_stats.pkts_dropped_tx++;

        LOG_MSG_ERROR_INT_0("ps_dpm_dl_opti_clat_rx_pkt_internal():Not able to generate"
                        " rx_mi_ptr info,dropping packet");
        PS_RX_META_INFO_FREE(rx_mi_ptr);

        PS_LEAVE_CRIT_SECTION(&ps_dpm_crit_section);
        PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);
        return FALSE;
      }

      ip_pkt_info_ptr = &PS_RX_META_GET_PKT_INFO(*rx_mi_ptr); 
      if( PS_IFACE_IPFLTR_NOMATCH == 
            ps_iface_ipfltr_execute( bridge_iface_ptr, 
                                     IP_FLTR_CLIENT_POWERSAVE, 
                                     ip_pkt_info_ptr))
      {
        //call the drop packet indication to run the fast dormancy delay timer algorithm
        um_base_iface_ptr->iface_i_stats.pkts_dropped_rx++;
        bridge_iface_ptr->iface_i_stats.pkts_dropped_tx++;
        
        if(!ps_iface_is_first_powersave_packet_drop_reported())
        {
          ps_iface_set_first_powersave_packet_drop_reported(TRUE);
          ps_sys_fast_dormancy_pkt_dropped_ind(bridge_iface_ptr);
        }
        
        LOG_MSG_ERROR_INT_0("ps_dpm_dl_opti_clat_rx_pkt_internal():Powersave filter "
                        "didn't match, dropping packet");

        DPL_LOG_NETWORK_TX_DROPPED_PACKET
          (bridge_iface_ptr,*pkt_ref_ptr,DPL_IID_NETPROT_IP);
          
        PS_DPM_DL_INC_STATS(bearer_ptr,stats.dropped.pkts.powersave_filter, 1);
        PS_DPM_DL_INC_STATS(bearer_ptr,stats.dropped.bytes.powersave_filter, total_len);           
        dsm_free_packet(pkt_ref_ptr);
        ret_status = FALSE;
      }
      else
      {
        PS_DPM_DL_INC_STATS(bearer_ptr,stats.total_dl_opt_path_pkts, 1);
        PS_DPM_DL_INC_STATS(bearer_ptr,stats.total_dl_opt_path_bytes, total_len);         
        #ifndef TEST_FRAMEWORK
          sio_transmit(dpm_rm_info_ptr->sio_stream_id, *pkt_ref_ptr);      
        #else
          #error code not present
#endif /* TEST_FRAMEWORK */
        
        if(PS_IFACE_GET_POWERSAVE_FILTERING_AUTO_EXIT_MODE(bridge_iface_ptr))
        {
          if( -1 == ps_iface_set_powersave_filtering_mode( bridge_iface_ptr,
                                                           FALSE,
                                                           &ps_errno) )
          {
            
            LOG_MSG_ERROR_INT_0("ps_dpm_dl_opti_clat_rx_pkt_internal:" 
              "powersave filtering mode change failed ");
          }
        }
      }
      PS_RX_META_INFO_FREE(rx_mi_ptr);
    }
    else
    {
      PS_DPMI_INC_CONT_VALID_PKT_CNT( bearer_ptr , ps_dpm_cont_err_pkt_info ); 
      PS_DPM_DL_INC_STATS(bearer_ptr,stats.total_dl_opt_path_pkts, 1);
      PS_DPM_DL_INC_STATS(bearer_ptr,stats.total_dl_opt_path_bytes, total_len);      
      #ifndef TEST_FRAMEWORK
        sio_transmit(dpm_rm_info_ptr->sio_stream_id, *pkt_ref_ptr);
      #else
        #error code not present
#endif /* TEST_FRAMEWORK */  
    }

    PS_RX_META_INFO_FREE(rx_mi_ptr);
    
    PS_LEAVE_CRIT_SECTION(&ps_dpm_crit_section);
    PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

    //MSR TODO The meta info architecture is such that once one function
    //passes on meta info to another function, that function becomes
    //responsible for freeing meta info. So, Rx meta should be freed here,
    //not in phys_link_input. Same applies for ps_dl_iface_opt_rx_pkt

    return ret_status;
  } while (0);

  /*-------------------------------------------------------------------------
    Leave CS before tx_cmd to avoid possible deadlock issues
  -------------------------------------------------------------------------*/
  PS_LEAVE_CRIT_SECTION(&ps_dpm_crit_section);  
  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);
  

  /*-------------------------------------------------------------------------
    Process the packet based on 'path_type'

    If 'path_type' is LEGACY, call ps_iface_input. Otherwise, free the packet
  -------------------------------------------------------------------------*/
  if (PS_DPM_DL_OPT_LEGACY_PATH == path_type)
  {
    if (NULL == *rx_mi_ptr)
    {
      PS_RX_META_INFO_GET(*rx_mi_ptr);
      if (NULL == *rx_mi_ptr)
      {
        LOG_MSG_ERROR_INT_0("ps_dpm_dl_opti_rx_pkt_internal: "
                        "Failed to get rx meta info buf, dropping packet");
        DPL_LOG_NETWORK_RX_DROPPED_PACKET
          (um_base_iface_ptr,*pkt_ref_ptr,DPL_IID_NETPROT_IP);
        um_base_iface_ptr->iface_i_stats.pkts_dropped_rx++;
        if (NULL != bridge_iface_ptr)
        {
          bridge_iface_ptr->iface_i_stats.pkts_dropped_tx++;  
        }          
        PS_DPM_DL_INC_STATS(bearer_ptr,stats.dropped.pkts.pkt_info_err, 1);
        PS_DPM_DL_INC_STATS(bearer_ptr,stats.dropped.bytes.pkt_info_err, total_len);           
        dsm_free_packet(pkt_ref_ptr);
        return FALSE;
      }        
    } 
    PS_DPMI_INC_CONT_VALID_PKT_CNT( bearer_ptr , ps_dpm_cont_err_pkt_info ); 
    /*---------------------------------------------------------------------
      Checksum is offloaded to IPA hardware so that protocols dont
      have to recalculate checksum value.
    ---------------------------------------------------------------------*/ 
    ip_pkt_info_ptr = &PS_RX_META_GET_PKT_INFO(*rx_mi_ptr); 
      
    PS_DPM_HW_SET_CKSUM_OFFLOAD_VALUES(ip_pkt_info_ptr, *pkt_ref_ptr);   
    
    PS_RX_META_SET_FILTER_RESULT
    (
      *rx_mi_ptr,
      IP_FLTR_CLIENT_SOCKETS,
      filter_result
    );
     
    PS_RX_META_SET_SOCKET_FI_SOURCE
    (
      *rx_mi_ptr, 
      PS_IFACE_IPFLTR_DL_SOCKET_RESULT_SOURCE_DPM
    );

    if (TRUE == is_legacy_proc_needed)
    {
      PS_DPM_DL_INC_STATS(bearer_ptr,stats.total_dl_legacy_path_pkts, 1);
      PS_DPM_DL_INC_STATS(bearer_ptr,stats.total_dl_legacy_path_pkts, total_len);   
#ifdef TEST_FRAMEWORK
      #error code not present
#endif
      (void) ps_iface_input_internal(um_base_iface_ptr,
                                     pkt_ref_ptr,
                                     *rx_mi_ptr,
                                     NULL);
      return TRUE;
    }
    else
    {
      return FALSE;
    }
  }
  else
  {
    PS_DPMI_INC_CONT_PKT_DROP_CNT( bearer_ptr , ps_dpm_cont_err_pkt_info ); 
    if (DS_EINVAL != ds_errno)
    {  
      if (PS_IFACE_IS_VALID(um_base_iface_ptr))
      {
        DPL_LOG_NETWORK_RX_DROPPED_PACKET
         (um_base_iface_ptr,*pkt_ref_ptr,DPL_IID_NETPROT_IP);
        um_base_iface_ptr->iface_i_stats.pkts_dropped_rx++;
      }
      if (NULL != bridge_iface_ptr)
      {
        bridge_iface_ptr->iface_i_stats.pkts_dropped_tx++;  
      }
      dsm_free_packet(pkt_ref_ptr);
    }
    return FALSE;
  }
} /* ps_dpm_dl_opti_clat_rx_pkt_internal() */
#endif /* FEATURE_DATA_PS_464XLAT */
/*===========================================================================

                        EXTERNAL FUNCTION DEFINITIONS

===========================================================================*/
boolean ps_dl_phys_opt_rx_pkt
(
  const ps_phys_link_type  * this_phys_link_ptr,
  dsm_item_type           ** pkt_ref_ptr,
  ps_rx_meta_info_type     * rx_mi_ptr,
  ps_dpm_bearer_info_type  * bearer_ptr
)
{
  ps_iface_type * v4_iface_ptr = NULL;
  ps_iface_type * v6_iface_ptr = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------
    To avoid race conditions here we are caching v4 and v6 iface ptr's 
    after validating phys link under critical section.
    if corresponding iface is not valid it will be dropped 
    anyway in further processing.
    ---------------------------------------------------------------------*/	
  PS_ENTER_CRIT_SECTION(&ps_dpm_crit_section);
  if(PS_PHYS_LINK_IS_VALID(this_phys_link_ptr))
  {
    v4_iface_ptr = this_phys_link_ptr->rx_cmd_info.ip_proto_handle.v4_iface_ptr;
    v6_iface_ptr = this_phys_link_ptr->rx_cmd_info.ip_proto_handle.v6_iface_ptr;
  }
  else
  {
    PS_LEAVE_CRIT_SECTION(&ps_dpm_crit_section);
    return FALSE;
  }
  PS_LEAVE_CRIT_SECTION(&ps_dpm_crit_section);
  
#ifdef FEATURE_DATA_PS_464XLAT
  if (PS_IFACE_GET_CAPABILITY(v4_iface_ptr, 
                               PS_IFACE_CAPABILITY_CLAT_CAPABLE) ||
      PS_IFACE_GET_CAPABILITY(v6_iface_ptr, 
                               PS_IFACE_CAPABILITY_CLAT_CAPABLE))
  { 
    return ps_dpm_dl_opti_clat_rx_pkt_internal
           (
             v4_iface_ptr,
             v6_iface_ptr,
             pkt_ref_ptr,
             &rx_mi_ptr,
             TRUE,
             bearer_ptr
           );
  }
  else
#endif /* FEATURE_DATA_PS_464XLAT */
  {
    return ps_dpm_dl_opti_rx_pkt_internal
           (
             v4_iface_ptr,
             v6_iface_ptr,
             pkt_ref_ptr,
             &rx_mi_ptr,
             TRUE,
             bearer_ptr
           );
  }  
} /* ps_dl_phys_opt_rx_pkt() */


boolean ps_dl_iface_opt_rx_pkt
(
  ps_iface_type            * this_iface_ptr,
  dsm_item_type           ** pkt_ref_ptr,
  ps_rx_meta_info_type    ** rx_mi_ptr
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#ifdef FEATURE_DATA_PS_464XLAT
  if (PS_IFACE_GET_CAPABILITY(this_iface_ptr, 
                              PS_IFACE_CAPABILITY_CLAT_CAPABLE))
  {            
    /*---------------------------------------------------------------------
      Here is_legacy == TRUE because for clat mode, we want
      convereted packet to end up on clat iface rather than current um iface
      Note : ps_dl_iface_opt_rx_pkt is called by ps_iface_input and ps_iface_input
      itself belong to legacy path and hence is_legacy is false for 
      non-clat scenario
    ---------------------------------------------------------------------*/                   
    return ps_dpm_dl_opti_clat_rx_pkt_internal(this_iface_ptr,
                                               this_iface_ptr,
                                               pkt_ref_ptr,
                                               rx_mi_ptr,
                                               TRUE,
                                               NULL);
  }
  else
#endif /* FEATURE_DATA_PS_464XLAT */
  {
    return ps_dpm_dl_opti_rx_pkt_internal(this_iface_ptr,
                                          this_iface_ptr,
                                          pkt_ref_ptr,
                                          rx_mi_ptr,
                                          FALSE,
                                          NULL);
  }  
} /* ps_dl_iface_opt_rx_pkt() */
