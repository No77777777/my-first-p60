/*=========================================================================*/
/*!
  @file
  ps_iface_io.c

  @brief
  This file abstracts the data path functionality of the PS iface.

  @see
  ps_iface.c

  Copyright (c) 2008-2017 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
*/
/*=========================================================================*/

/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/netiface/src/ps_iface_io.c#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2017-09-20 js  Added support for RFC2460 Regulator Mandatory Requirement
  2014-11-09 yl  IP data path re-architecture
  2013-07-24 ash Data Path Debug F3's & Routing support for IPsec + Fragmentation.
  2013-02-04 ssh Empty IPv6 frags into temp queue to avoid reassembly loop
  2013-01-18 tw  Check if packet should go over alternate iface
  2013-01-09 ssh Deliver pending frags for ip input as well
  2012-07-12 tw  Multi PDN over 1 RMNET   
  2012-04-17 su  Added fragment local check to ps_iface_input_internal
  2012-03-09 pgm Added check to drop data packets on flow if capability is set
  2011-11-01 am  Mainlining opt DL path for NAT support too.
  2011-09-22 am  Added optimized DL MBIM path in ps_iface_input().
  2011-09-14 sm  Added TCP ACK PRIO NV item check for dynamic enablement
                 of ACK tagging.
  2011-08-11 sm  FEATURE_DATA_PS_TCP_ACK_PRIO, tagged tcp ack packet as
                 DSM_HIGHEST.
  2010-05-13 pp  Avoid multiple packet info generations during scenarios
                 involving Logical IFACE.
                 Avoid double filtering during Frag Handle timeout scenario
                 [ps_iface_frag_tx_cmd].
  2010-04-20 am  Invalidate pkt_info after physical iface tx in
                 logical_default_tx_cmd(). Check for pkt_info validity
                 before dequeueing frags in ps_iface_tx_cmd().
  2009-07-27 sp  Fixed IP Fragmentation issues.
  2008-07-09 hm  Created module.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"


#include "ds_Utils_DebugMsg.h"
#include "ps_iface.h"
#include "ps_ifacei.h"
#include "ps_crit_sect.h"
#include "dsm.h"
#include "ps_pkt_info_utils.h"
#include "ps_ip_fraghdl.h"
#include "ps_stat_iface.h"
#include "ps_stat_flow.h"
#include "ps_stat_phys_link.h"
#include "ps_phys_linki.h"
#include "ps_stat_phys_link.h"
#include "ps_ifacei_utils.h"
#include "ps_flow.h"
#include "ps_flowi_utils.h"
#include "ps_ip4.h"
#include "ps_ip_fraghdl.h"
#include "ps_metai_info.h"
#include "ps_utils.h"
#include "ps_dpm.h"

#ifdef FEATURE_DATA_PS_IPV6
#include "ps_ip6.h"
#include "ps_icmp6_msg.h"
#include "ps_icmp6.h"
#endif

#include "ps_logging.h"
#include "ps_logging_defs.h"

#include "ps_dpm_dl_opt.h"
#include "ps_dpm.h"
#include "ps_icmp.h"

static int ps_iface_frag_tx_cmd
(
  void                 * device_ptr,
  dsm_item_type       ** dsm_item_ptr,
  ps_tx_meta_info_type * meta_info_ptr
);
/*===========================================================================

                             INTERNAL FUNCTIONS

===========================================================================*/

/*===========================================================================
FUNCTION PS_IFACE_INPUT_INTERNAL()

DESCRIPTION
   Interface client gives the protocol stack a packet to transmit.

PARAMETERS
  this_iface_ptr:    ptr to interface control block on which to operate on.
  pkt_ref_ptr:       ref to dsm item ptr received
  rx_meta_info_ptr:  ptr to RX meta meta information
  tx_meta_info_ptr:  ptr to TX meta meta information

RETURN VALUE
  0: data was transfered
 -1: data was not transferred, but memory was freed

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
int ps_iface_input_internal
(
  ps_iface_type         *this_iface_ptr,
  dsm_item_type        **pkt_ref_ptr,
  ps_rx_meta_info_type  * rx_meta_info_ptr,
  ps_tx_meta_info_type  * tx_meta_info_ptr
)
{
  uint64                   packet_len_in_bytes = 0;
  errno_enum_type          ps_errno;
  int                      retval = 0;
  uint16                   filter_result;

  ps_iface_type          * bridge_iface_ptr;
  ps_iface_type          * next_iface_ptr;
  ps_tx_meta_info_type   * tmp_meta_info_ptr = NULL;
  void                   * fraghdl = NULL;
  dsm_item_type          * frag_ptr;
  ps_iface_type          * if_ptr = NULL;
  ip_pkt_info_type       * meta_pkt_info_ptr = NULL;

  boolean                  execute_iface_filter = TRUE;
  boolean                  execute_group_filter = TRUE;
  ps_icmp6_msg_type        icmp6;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_DATA_PATH_INFO_2("ps_iface_input_internal(): iface 0x%x:%d",
                           this_iface_ptr->name, this_iface_ptr->instance);

  /*-------------------------------------------------------------------------
    Increment stats
  -------------------------------------------------------------------------*/
  IFACE_INC_INSTANCE_STATS(this_iface_ptr, pkts_rx, 1);

  packet_len_in_bytes = dsm_length_packet(*pkt_ref_ptr);

  IFACE_INC_INSTANCE_STATS(this_iface_ptr, bytes_rx, packet_len_in_bytes);

  bridge_iface_ptr = PS_IFACEI_GET_BRIDGE_IFACE(this_iface_ptr);

  /*-------------------------------------------------------------------------
    If filter result is set from Hardware
      1. Filter result is less than max iface indexes. It means filter
         result points to index of iface. If filter result matches iface index 
         then call ip4_input or ip6_input else retrieve iface from index and 
         call rx_proc_f_ptr    
      2. Filter result is equal to PS_IFACE_IPFLTR_RESULT_DL_HW_UNKNOWN. This 
         would be set for example for iid filtering where even though packet
         is diverted to modem, full filtering still needs to be done to 
         determine if packet belongs to modem embedded stack.  Execute only 
         iface filters. Do not execute group filters.          
      3. Filter result is equal to iface name. It means filter result was set
         by client calling "add_by_group" and wanted to run the filter on 
         every iface matching the iface name. Send the packet to protocols 
         and call ip4_input or ip6_input 
      4. Filter result is equal to PS_IFACE_IPFLTR_RESULT_DL_PROTOCOL. This
         is set by Protocol to indicate packet belongs to protocol stack. Send 
         the packet to protocols and call ip4_input or ip6_input.  
      5. Retrieve back if ptr and call rx_proc_f_ptr on that iface. If 
         back_iface_ptr is not valid then execute both iface and group filters.
         This step has been introduced only for safety reason just in case
         filter result from Hardware is not correct
         
    If filter result is set from DPM
      6. Filter result can be less than max iface indexes. It means filter
         result points to index of iface and full iface filtering needs to 
         be done if filter index matches current iface. If filter result matches 
         iface index then run all filters on iface to determine if filter matches. 
         If it doesnt then retrieve iface from filter index and call rx_proc_f_ptr.    
      7. Filter result is equal to iface name. It means filter result was set
         by client calling "add_by_group" so only run group filters if
         iface name matches current iface. Execute only group filters and 
         skip iface filters  
      8. Filter result is equal to PS_IFACE_IPFLTR_RESULT_DL_PROTOCOL. This
         is set by Protocol to indicate packet belongs to protocol stack.
         Send the packet to protocols and call ip4_input or ip6_input  

    Note :
      1. Iface name cannot be used for protocol filters because those filters
         are global filters and it needs to be ran on physical iface (3gpp/3gpp2). 
      2. Order of if checks should be maintain as above numerical value 
         assigned. Iface index would be most used followed by hardware
         unknown (iid filtering) followed by iface name and DL protocols         
  -------------------------------------------------------------------------*/  
  if (NULL != rx_meta_info_ptr)
  {
    if (PS_IFACE_IPFLTR_DL_SOCKET_RESULT_SOURCE_HW == 
        PS_RX_META_GET_SOCKET_FI_SOURCE(rx_meta_info_ptr))
    {
      filter_result = 
        PS_RX_META_GET_FILTER_RESULT(rx_meta_info_ptr, IP_FLTR_CLIENT_SOCKETS);

      LOG_MSG_DATA_PATH_INFO_1("ps_iface_input_internal() HW filter_result %d", 
                                filter_result); 

      /*-------------------------------------------------------------------------
        Step 1.
      -------------------------------------------------------------------------*/        
      if (filter_result < MAX_SYSTEM_IFACES)
      {
        if (filter_result == PS_IFACE_GET_IFACE_INDEX(this_iface_ptr))
        {
          /*-----------------------------------------------------------------------
            Send this pkt to Protocols (ip4_input or ip6_input)
          -----------------------------------------------------------------------*/
          return PS_IFACE_GET_INPUT_F_PTR(this_iface_ptr)(this_iface_ptr,
                                                          *pkt_ref_ptr,
                                                          rx_meta_info_ptr);        
        }
        else
        {
          if_ptr = this_iface_ptr->iface_private.back_iface_ptr;
          if (PS_IFACE_IS_VALID(if_ptr) && 
              NULL != if_ptr->iface_private.rx_proc_f_ptr)
          {
            return if_ptr->iface_private.rx_proc_f_ptr( if_ptr,
                                                        pkt_ref_ptr,
                                                        rx_meta_info_ptr);
          }
          else
          {
            LOG_MSG_ERROR_INT_1("Invalid iface filter result %d ", filter_result);      
          }
        }
      }
      /*-------------------------------------------------------------------------
        Step 2.     
      -------------------------------------------------------------------------*/       
      else if (filter_result == PS_IFACE_IPFLTR_RESULT_DL_HW_UNKNOWN)
      {
        execute_iface_filter = TRUE;
        execute_group_filter = FALSE;
      }      
      /*-------------------------------------------------------------------------
        Step 3 and Step 4.   
      -------------------------------------------------------------------------*/         
      else if (filter_result == PS_IFACE_GET_NAME(this_iface_ptr) ||
              filter_result == PS_IFACE_IPFLTR_RESULT_DL_PROTOCOL)
      {
        /*-----------------------------------------------------------------------
          Send this pkt to Protocols (ip4_input or ip6_input)
        -----------------------------------------------------------------------*/
        return PS_IFACE_GET_INPUT_F_PTR(this_iface_ptr)(this_iface_ptr,
                                                        *pkt_ref_ptr,
                                                        rx_meta_info_ptr);      
      }
      /*-------------------------------------------------------------------------
        Step 5.          
      -------------------------------------------------------------------------*/        
      else
      {
        if_ptr = this_iface_ptr->iface_private.back_iface_ptr;
        if (PS_IFACE_IS_VALID(if_ptr) && 
            if_ptr->iface_private.rx_proc_f_ptr)
        {
          return if_ptr->iface_private.rx_proc_f_ptr( if_ptr,
                                                      pkt_ref_ptr,
                                                      rx_meta_info_ptr);
        }
        else
        {
          /*-----------------------------------------------------------------------
            If iface back ptr is NULL. For example epc iface back ptr is null
            then filtering module needs to execute all filters.
          -----------------------------------------------------------------------*/
          execute_iface_filter = TRUE;
          execute_group_filter = TRUE;         
        }      
      }
    }     
    else if (PS_IFACE_IPFLTR_DL_SOCKET_RESULT_SOURCE_DPM == 
             PS_RX_META_GET_SOCKET_FI_SOURCE(rx_meta_info_ptr))  
    {
      /*-------------------------------------------------------------------------
          Log the packet on the Rx side
      -------------------------------------------------------------------------*/
      DPL_LOG_NETWORK_RX_PACKET(this_iface_ptr, *pkt_ref_ptr, DPL_IID_NETPROT_IP);
    
      filter_result = 
        PS_RX_META_GET_FILTER_RESULT(rx_meta_info_ptr, IP_FLTR_CLIENT_SOCKETS); 

      LOG_MSG_DATA_PATH_INFO_1("ps_iface_input_internal() DPM filter_result %d",
                                filter_result); 

      /*-------------------------------------------------------------------------
        Step 6.
      -------------------------------------------------------------------------*/            
      if (filter_result < MAX_SYSTEM_IFACES)
      {
        if (filter_result != PS_IFACE_GET_IFACE_INDEX(this_iface_ptr))
        {
          /*-----------------------------------------------------------------------
            Send this pkt to the iface matching the index.
          -----------------------------------------------------------------------*/    
          if_ptr = global_iface_ptr_array[filter_result];
          if (PS_IFACE_IS_VALID(if_ptr) && 
              NULL != if_ptr->iface_private.rx_proc_f_ptr)
          {
            return if_ptr->iface_private.rx_proc_f_ptr( if_ptr,
                                                        pkt_ref_ptr,
                                                        rx_meta_info_ptr);
          }                                                            
        }
        else
        {
          execute_iface_filter = TRUE;
          execute_group_filter = FALSE;
        }
      }
      /*-------------------------------------------------------------------------
        Step 7. 
      -------------------------------------------------------------------------*/         
      else if (filter_result == PS_IFACE_GET_NAME(this_iface_ptr))
      {
        execute_iface_filter = FALSE;
        execute_group_filter = TRUE;   
      }     
      /*-------------------------------------------------------------------------
        Step 8.   
      -------------------------------------------------------------------------*/          
      else if (filter_result == PS_IFACE_IPFLTR_RESULT_DL_PROTOCOL)
      {
        /*-----------------------------------------------------------------------
          Send this pkt to Protocols (ip4_input or ip6_input)
        -----------------------------------------------------------------------*/
        return PS_IFACE_GET_INPUT_F_PTR(this_iface_ptr)(this_iface_ptr,
                                                        *pkt_ref_ptr,
                                                        rx_meta_info_ptr);      
      }    
    }
  }
  else
  {
    /*-------------------------------------------------------------------------
        Log the packet on the Rx side
    -------------------------------------------------------------------------*/
    DPL_LOG_NETWORK_RX_PACKET(this_iface_ptr, *pkt_ref_ptr, DPL_IID_NETPROT_IP);
  }

  LOG_MSG_DATA_PATH_INFO_4("ps_iface_input_internal(): Regular filtering, iface "
                           "0x%x:%d rx meta info 0x%x filtering source %d",
                           this_iface_ptr->name, 
                           this_iface_ptr->instance, 
                           rx_meta_info_ptr,
                           (NULL != rx_meta_info_ptr) ? 
                           PS_RX_META_GET_SOCKET_FI_SOURCE(rx_meta_info_ptr): 0);
  /*-------------------------------------------------------------------------
    At this stage atleast execute_iface_filter or execute_group_filter is
    true 
  -------------------------------------------------------------------------*/  
  /*-------------------------------------------------------------------------
    Generate Rx meta info. It will be used to filter the packet and also by
    TCP/IP stack
  -------------------------------------------------------------------------*/
  if( rx_meta_info_ptr == NULL ||
    PS_RX_META_GET_PKT_INFO(rx_meta_info_ptr).is_pkt_info_valid == FALSE )
  {
    LOG_MSG_DATA_PATH_INFO_0("ps_iface_input_internal: generating packet info");
    if(0 > ps_rx_pkt_info_generate(pkt_ref_ptr, &rx_meta_info_ptr, &ps_errno))
    {
      meta_pkt_info_ptr = &(PS_RX_META_GET_PKT_INFO(rx_meta_info_ptr));
      switch(ps_errno)
      {
        case E_WOULD_BLOCK:
        {
          /*------------------------------------------------------------------
            IP fragment scenario! Set the iface in the corresponding
            Frag handle and free the Meta Info.
          ------------------------------------------------------------------*/
          ps_ip_fraghdl_set_bridge_device(meta_pkt_info_ptr->fraghdl,
                                          (void *)bridge_iface_ptr,
                                          IP_FRAGHDL_BRIDGE_TYPE_IFACE_BRIDGE,
                                          ps_iface_frag_tx_cmd);
          PS_RX_META_INFO_FREE(&rx_meta_info_ptr);
          PS_TX_META_INFO_FREE(&tx_meta_info_ptr);
          return 0;
        }

        case E_BAD_DATA:
        {
#ifdef FEATURE_DATA_PS_IPV6
          LOG_MSG_DATA_PATH_INFO_4( "ps_iface_input_internal: E_BAD_DATA, ip %d, "
                                    "type %d, code %d, offset 0x%X",
                                    meta_pkt_info_ptr->ip_vsn,
                                    meta_pkt_info_ptr->ptcl_info.icmp.type,
                                    meta_pkt_info_ptr->ptcl_info.icmp.code,
                                    meta_pkt_info_ptr->ptcl_info.icmp.prob_offset);
          if(meta_pkt_info_ptr->ip_vsn == IP_V6)
          {
            if(0 != meta_pkt_info_ptr->ptcl_info.icmp.type)
            {
              memset(&icmp6, 0, sizeof(icmp6));
              meta_pkt_info_ptr->if_ptr = this_iface_ptr;
              IFACE_INC_INSTANCE_STATS(this_iface_ptr, pkts_dropped_rx, 1);
              icmp6.cmn_hdr.type = (uint8)meta_pkt_info_ptr->ptcl_info.icmp.type;
              icmp6.cmn_hdr.code = (uint8)meta_pkt_info_ptr->ptcl_info.icmp.code;
              icmp6.msg_type.param_prob_offset = meta_pkt_info_ptr->ptcl_info.icmp.prob_offset;
              icmpv6_error_send(pkt_ref_ptr, &icmp6, meta_pkt_info_ptr);
              PS_RX_META_INFO_FREE(&rx_meta_info_ptr);
              return -1;
            }
          }
#endif
        }

        case E_INVALID_ARG:
        case E_VERSION_MISMATCH:
        case E_NO_MEMORY:
        default:
        {
          /*-----------------------------------------------------------------
            Insufficient information to filter, packet consumed, return.
            Currently do not allow such packets, Need to DO research more,
            what kind we SHOULD forward!!
          -----------------------------------------------------------------*/
          LOG_MSG_ERROR_INT_1("ps_iface_input_internal(): "
                          "Dropping packet, reason: %d ", ps_errno );
          IFACE_INC_INSTANCE_STATS(this_iface_ptr, pkts_dropped_rx, 1);
          PS_RX_META_INFO_FREE(&rx_meta_info_ptr);
          PS_TX_META_INFO_FREE(&tx_meta_info_ptr);
          DPL_LOG_NETWORK_RX_DROPPED_PACKET
            (this_iface_ptr,*pkt_ref_ptr,DPL_IID_NETPROT_IP);
          dsm_free_packet( pkt_ref_ptr );
          return -1;
        }
      }
    }
  }

  /*-----------------------------------------------------------------------
      Reset filter result so that filtering happens on this iface. Else, if
      filtering happened on physical iface, filtering would be bypassed on
      logical iface, which is incorrect.
  -----------------------------------------------------------------------*/
  LOG_MSG_DATA_PATH_INFO_0("ps_iface_input_internal: Resetting filter result");

  PS_RX_META_RESET_FILTER_RESULT(rx_meta_info_ptr, IP_FLTR_CLIENT_SOCKETS);
  
  /* Send low latency indication incase of ICMP fragmented scenario */
  if((rx_meta_info_ptr->pkt_info.fraghdl != NULL) &&
	 (rx_meta_info_ptr->pkt_info.is_pkt_info_valid == TRUE))
    {
      /* v4 pkt */
      if(rx_meta_info_ptr->pkt_info.ip_vsn == IP_V4)
      {
         if(rx_meta_info_ptr->pkt_info.ip_hdr.v4.protocol == PS_IPPROTO_ICMP)
         {
           if((rx_meta_info_ptr->pkt_info.ptcl_info.icmp.type == ICMP_ECHO_REPLY) ||
	   	   (rx_meta_info_ptr->pkt_info.ptcl_info.icmp.type == ICMP_ECHO_REQ))
           {
             ps_iface_low_latency_traffic_status_cb((uint32)NULL);
           }
         }
       }
       /* v6 pkt */
       else if(rx_meta_info_ptr->pkt_info.ip_vsn == IP_V6)
       {
         if(rx_meta_info_ptr->pkt_info.ip_hdr.v6.hdr_type == PS_IPPROTO_ICMP6)
         {
           if((rx_meta_info_ptr->pkt_info.ptcl_info.icmp.type == ICMP6_ECHO_REQ) ||
	   	   (rx_meta_info_ptr->pkt_info.ptcl_info.icmp.type == ICMP6_ECHO_RSP))
           {
             ps_iface_low_latency_traffic_status_cb((uint32)NULL);
           }
         }
       }
    }

  /*-------------------------------------------------------------------------
    Check if the packet gets picked up by a sockets filter, pass it up the
    stack then.
  -------------------------------------------------------------------------*/
  if(PS_IFACEI_NUM_FILTERS(this_iface_ptr, IP_FLTR_CLIENT_SOCKETS) > 0)
  {
    LOG_MSG_DATA_PATH_INFO_0("ps_iface_input_internal: Executing rx filters");

    if (0 > ps_pkt_info_filter_rx_pkt_internal(this_iface_ptr,
                                               IP_FLTR_CLIENT_SOCKETS,
                                               rx_meta_info_ptr,
                                               execute_iface_filter,
                                               execute_group_filter,
                                               &ps_errno))
    {
      LOG_MSG_ERROR_INT_1("ps_iface_input_internal(): "
                      "Dropping packet, reason: %d ", ps_errno );
      IFACE_INC_INSTANCE_STATS(this_iface_ptr, pkts_dropped_rx, 1);
      PS_RX_META_INFO_FREE(&rx_meta_info_ptr);
      PS_TX_META_INFO_FREE(&tx_meta_info_ptr);
      DPL_LOG_NETWORK_RX_DROPPED_PACKET
        (this_iface_ptr,*pkt_ref_ptr,DPL_IID_NETPROT_IP);
      dsm_free_packet( pkt_ref_ptr );
      return -1;
    }

    /*---------------------------------------------------------------------
      Reset socket_fi_source since once full filters are executed, filter result
      is not coming from Hardware or DPM
    ---------------------------------------------------------------------*/ 
    PS_RX_META_SET_SOCKET_FI_SOURCE(rx_meta_info_ptr,
                                    PS_IFACE_IPFLTR_DL_SOCKET_RESULT_SOURCE_NONE);
    /*-------------------------------------------------------------------------
      If filter passed, send the packet to TCP/IP stack
    -------------------------------------------------------------------------*/
    if (PS_RX_META_GET_FILTER_RESULT(rx_meta_info_ptr, IP_FLTR_CLIENT_SOCKETS) !=
          PS_IFACE_IPFLTR_NOMATCH)
    {     
      PS_TX_META_INFO_FREE(&tx_meta_info_ptr);
      LOG_MSG_DATA_PATH_INFO_0("ps_iface_input_internal: Routing to IP layer");
      return PS_IFACE_GET_INPUT_F_PTR(this_iface_ptr)(this_iface_ptr,
                                                      *pkt_ref_ptr,
                                                      rx_meta_info_ptr);                                                    
    }  
    else
    { 
      /*-----------------------------------------------------------------------
        If bridge iface is not null for the given iface and filter failed for
        the iface, pkt has to go to bridge iface. CR 804881
      -----------------------------------------------------------------------*/    
      if (bridge_iface_ptr == NULL)
      {      
        if_ptr = this_iface_ptr->iface_private.back_iface_ptr;

        if (PS_IFACE_IS_VALID(if_ptr) && 
            NULL != if_ptr->iface_private.rx_proc_f_ptr)
        {
          return if_ptr->iface_private.rx_proc_f_ptr( if_ptr,
                                                      pkt_ref_ptr,
                                                      rx_meta_info_ptr);
        }
      }      
    }
  }

  /*-------------------------------------------------------------------------
    Handle the case where iface is not bridged

    If iface is configured to consume packet, then send it to TCP/IP stack,
    else, send the packet up the iface chain.

    Iface is configured to consume packet if
      1. There is no back_iface_ptr, meaning that this is the topmost iface in
         the chain
      2. Re-assembly flag is set to TRUE
      3. IPSec flag is set to TRUE
  -------------------------------------------------------------------------*/
  if (bridge_iface_ptr == NULL)
  { 
    LOG_MSG_DATA_PATH_INFO_0(
                   "ps_iface_input_internal: bridge iface pointer is NULL");
    PS_TX_META_INFO_FREE(&tx_meta_info_ptr);

    if (PS_IFACE_GET_REASSEMBLY_FLAG(this_iface_ptr) == TRUE ||
        PS_IFACE_GET_IPSEC_PROCESSING_FLAG(this_iface_ptr) == TRUE ||
        this_iface_ptr->iface_private.back_iface_ptr == NULL)
    {
      LOG_MSG_DATA_PATH_INFO_0(
                 "ps_iface_input_internal: reached last iface in the chain");

      return PS_IFACE_GET_INPUT_F_PTR(this_iface_ptr)(this_iface_ptr,
                                                      *pkt_ref_ptr,
                                                      rx_meta_info_ptr);
    }
    else
    {
      LOG_MSG_DATA_PATH_INFO_0("ps_iface_input_internal: no processing needed"
                               " at this iface, routing to upper ifaces");
      next_iface_ptr = this_iface_ptr->iface_private.back_iface_ptr;
      return next_iface_ptr->iface_private.rx_proc_f_ptr( next_iface_ptr,
                                                          pkt_ref_ptr,
                                                          rx_meta_info_ptr);
    }
  }

  /*-------------------------------------------------------------------------
    If control reaches here, it means that packet needs to be forwarded to
    bridge iface
  -------------------------------------------------------------------------*/

  /*-------------------------------------------------------------------------
    Drop packet if bridged interface is not up or routeable

    No need to check for validity of bridge iface as ps_iface_state does it
  -------------------------------------------------------------------------*/
  LOG_MSG_DATA_PATH_INFO_0("ps_iface_input_internal: Bridge iface is not null");

  if (!(ps_iface_state(bridge_iface_ptr) & (IFACE_UP | IFACE_ROUTEABLE)))
  {
    LOG_MSG_INFO1_1("ps_iface_input_internal(): "
                    "Bridge iface 0x%p not in UP or ROUTEABLE state"
                    "Dropping packet",bridge_iface_ptr);
    IFACE_INC_INSTANCE_STATS(this_iface_ptr, pkts_dropped_rx, 1);
    DPL_LOG_NETWORK_RX_DROPPED_PACKET
      (this_iface_ptr,*pkt_ref_ptr,DPL_IID_NETPROT_IP);
    dsm_free_packet(pkt_ref_ptr);
    PS_RX_META_INFO_FREE(&rx_meta_info_ptr);
    PS_TX_META_INFO_FREE(&tx_meta_info_ptr);
    return -1;
  }

  /*-------------------------------------------------------------------------
    Copy fields from incoming RX meta_info_ptr into TX meta info and
    clear RX meta info, the created TX meta info used in further bridging
    scenarios!
  -------------------------------------------------------------------------*/
  LOG_MSG_DATA_PATH_INFO_0(
            "ps_iface_input_internal: Copying Rx meta info to tx meta info");
  if(tx_meta_info_ptr == NULL)
  {
    PS_TX_META_INFO_AND_RT_META_INFO_GET(tx_meta_info_ptr);

    if (NULL == tx_meta_info_ptr ||
        NULL == PS_TX_META_GET_RT_META_INFO_PTR(tx_meta_info_ptr))
    {
      IFACE_INC_INSTANCE_STATS(this_iface_ptr, pkts_dropped_rx, 1);
      LOG_MSG_ERROR_INT_0("ps_iface_input_internal(): "
                      "Out of metainfo items, Dropping packet");
      PS_RX_META_INFO_FREE(&rx_meta_info_ptr);
      PS_TX_META_INFO_FREE(&tx_meta_info_ptr);
      DPL_LOG_NETWORK_RX_DROPPED_PACKET
        (this_iface_ptr,*pkt_ref_ptr,DPL_IID_NETPROT_IP);
      dsm_free_packet(pkt_ref_ptr);
      return -1;
    }
  }

  PS_TX_META_INFO_COPY_FROM_RX_META_INFO(rx_meta_info_ptr, tx_meta_info_ptr);
  PS_RX_META_INFO_FREE(&rx_meta_info_ptr);

#ifdef FEATURE_DATA_RM_NET
  /*-------------------------------------------------------------------------
    If bridge interface is broadcast, add next hop IP addr metainfo for
    lan LLC module
  -------------------------------------------------------------------------*/
  if( PS_IFACE_GET_IS_BCAST_IFACE(bridge_iface_ptr) == TRUE)
  {
    /*-----------------------------------------------------------------------
      Set the next hop address and the IP address scope for the forwarded
      packet - later, extract IP address and check if broadcast, etc.
    -----------------------------------------------------------------------*/
    if (-1 == ps_iface_fill_next_hop_addr(bridge_iface_ptr,
                                          tx_meta_info_ptr,
                                          &ps_errno))
    {
      LOG_MSG_ERROR_INT_1("Unable to get next_hop_addr %d!, Dropping packet", ps_errno);
      IFACE_INC_INSTANCE_STATS(this_iface_ptr, pkts_dropped_tx, 1);
      PS_TX_META_INFO_FREE(&tx_meta_info_ptr);
      DPL_LOG_NETWORK_RX_DROPPED_PACKET
        (this_iface_ptr,*pkt_ref_ptr,DPL_IID_NETPROT_IP);
      dsm_free_packet(pkt_ref_ptr);
      return -1;
    }
  } /* PS_IFACE_GET_IS_BCAST_IFACE(bridge_iface_ptr) == TRUE */
#endif /* FEATURE_DATA_RM_NET */

  /*-------------------------------------------------------------------------
    If we make it here, the pkt needs to be forwarded to the bridged iface.
    If fraghdl is non-NULL, get fragments.
  -------------------------------------------------------------------------*/
  if (tx_meta_info_ptr != NULL &&
      (PS_TX_META_GET_RT_META_INFO_PTR(tx_meta_info_ptr) != NULL) &&
      NULL !=
        (fraghdl = (&(PS_TX_META_GET_PKT_INFO(tx_meta_info_ptr)))->fraghdl) &&
      TRUE != ps_ip_fraghdl_is_local(fraghdl))
  {
    while( NULL != (frag_ptr = ip_fraghdl_get_fragment(fraghdl )) && 
		    TRUE != ps_ip_fraghdl_is_local(fraghdl))
    {
      PS_TX_META_INFO_DUP(tx_meta_info_ptr, &tmp_meta_info_ptr);
      LOG_MSG_DATA_PATH_INFO_0("ps_iface_input_internal(): "
                               "Sending fragments to bridge iface");

      if( -1 == ps_iface_tx_cmd(bridge_iface_ptr,
                                &frag_ptr,
                                tmp_meta_info_ptr))
      {
        retval = -1;
      }
    }
  }

  LOG_MSG_DATA_PATH_INFO_0("ps_iface_input_internal(): "
                           "Sending it to bridge iface");

  /*-------------------------------------------------------------------------
    If we make it here, the pkt needs to be forwarded to the bridged iface.
  -------------------------------------------------------------------------*/
  if( -1 == ps_iface_tx_cmd(bridge_iface_ptr,
                            pkt_ref_ptr,
                            tx_meta_info_ptr))
  {
    retval = -1;
  }

  return retval;

} /* ps_iface_input_internal() */

/*===========================================================================

                             EXTERNAL FUNCTIONS

===========================================================================*/


/*===========================================================================
FUNCTION PS_IFACEI_DEFAULT_TX_CMD()

DESCRIPTION
  This function is the default TX function.  It is assigned to the tx
  function pointer when no explicit TX function is registered.

  It frees the data passed in.

PARAMETERS
  this_iface_ptr:    interface on which this is being called
  pkt_ref_ptr:       ref to dsm item ptr for Tx - freed
  meta_info_ptr:     pointer to meta info type - also freed.
  tx_cmd_info:       ignored

RETURN VALUE
  -1: indicating error

DEPENDENCIES
  Should not be called for logical ifaces

SIDE EFFECTS
  None
===========================================================================*/
int ps_ifacei_default_tx_cmd
(
  ps_iface_type        *this_iface_ptr,
  dsm_item_type       **pkt_ref_ptr,
  ps_tx_meta_info_type *meta_info_ptr,
  void                 *tx_cmd_info
)
{
  ps_flow_type                               * ps_flow_ptr = NULL;
  ps_phys_link_type                          * phys_link_ptr = NULL;
  ps_phys_link_higher_layer_proto_enum_type    protocol;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_DATA_PATH_INFO_1(" ps_ifacei_default_tx_cmd 0x%x", this_iface_ptr);
  if((meta_info_ptr != NULL) &&
     (PS_TX_META_GET_RT_META_INFO_PTR(meta_info_ptr) != NULL))
  {
    ps_flow_ptr   = PS_FLOW_GET_FLOW_FROM_META_INFO(meta_info_ptr);
    phys_link_ptr = PS_FLOW_GET_PHYS_LINK(ps_flow_ptr);
  }

  /*-------------------------------------------------------------------------
    If phys link pointer is NULL send it on default phys link.
  -------------------------------------------------------------------------*/
  if(phys_link_ptr == NULL)
  {
    phys_link_ptr = PS_IFACE_GET_PHYS_LINK(this_iface_ptr);
  }

  protocol = ( ps_iface_addr_family_is_v4(this_iface_ptr))
    ? PS_PHYS_LINK_HIGHER_LAYER_PROTOCOL_IPV4
    : PS_PHYS_LINK_HIGHER_LAYER_PROTOCOL_IPV6;

  return ps_phys_link_tx_cmd(phys_link_ptr,
                             pkt_ref_ptr,
                             protocol,
                             meta_info_ptr);
} /* ps_ifacei_default_tx_cmd() */


/*===========================================================================
FUNCTION PS_IFACEI_LOGICAL_DEFAULT_TX_CMD()

DESCRIPTION
   This function is registered with a logical iface to call
   ps_iface_tx_cmd with the associated iface the logical iface is bound to.

PARAMETERS
  this_iface_ptr:    Pointer to ps_iface on which to operate
  item_ref_ptr:      Data to transfer
  meta_info_ref_ptr: Tx info for transmit
  tx_cmd_info:       Void ptr to tx cmd info

RETURN VALUE
  0 if SUCCESS
 -1 if FAILURE

DEPENDENCIES
  Should be called only for logical ifaces

SIDE EFFECTS
  None
===========================================================================*/
int ps_ifacei_logical_default_tx_cmd
(
  ps_iface_type         *this_iface_ptr,
  dsm_item_type        **pkt_ref_ptr,
  ps_tx_meta_info_type  *meta_info_ptr,
  void                  *tx_cmd_info
)
{
  ps_flow_type          * logical_flow_ptr = NULL;
  ps_iface_type         * assoc_iface_ptr = NULL;
  ps_tx_meta_info_type  * logical_mi_ptr = NULL;
  ps_tx_meta_info_type  * local_mi_ptr = NULL;
  int                     ret_val;
  ps_iface_ipfltr_result_type fi_result;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (!PS_IFACE_IS_VALID(this_iface_ptr)||
      !PS_IFACEI_IS_LOGICAL(this_iface_ptr))
  {
    LOG_MSG_ERROR_1("ps_ifacei_logical_default_tx_cmd(): "
                    "Invalid iface 0x%p, Dropping packet", this_iface_ptr);
    dsm_free_packet(pkt_ref_ptr);
    PS_TX_META_INFO_FREE(&meta_info_ptr);
    return -1;
  }

  assoc_iface_ptr = PS_IFACE_GET_ASSOC_IFACE(this_iface_ptr);
  if (!PS_IFACE_IS_VALID(assoc_iface_ptr))
  {
    LOG_MSG_ERROR_1("ps_ifacei_logical_default_tx_cmd(): "
                    "Invalid assoc iface 0x%p, Dropping packet", assoc_iface_ptr);
    dsm_free_packet(pkt_ref_ptr);
    PS_TX_META_INFO_FREE(&meta_info_ptr);
    return -1;
  }

  LOG_MSG_DATA_PATH_INFO_2(" ps_ifacei_logical_default_tx_cmd 0x%x:%d",  
                   this_iface_ptr->name, this_iface_ptr->instance);
  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

  if (PS_TX_AND_RT_META_INFO_IS_NULL(meta_info_ptr) == TRUE)
  {
    /*-----------------------------------------------------------------------
      Send this iface's default flow's meta-info downstream
    -----------------------------------------------------------------------*/
    logical_flow_ptr = PS_IFACE_GET_DEFAULT_FLOW(this_iface_ptr);
    logical_mi_ptr = PS_FLOW_GET_META_INFO_FROM_FLOW(logical_flow_ptr);
    if (logical_mi_ptr ==  NULL)
    {
      LOG_MSG_ERROR_INT_0("ps_ifacei_logical_default_tx_cmd(): "
                      "NULL logical meta info, Dropping packet");
      DPL_LOG_NETWORK_TX_DROPPED_PACKET
        (assoc_iface_ptr,*pkt_ref_ptr,DPL_IID_NETPROT_IP);
      dsm_free_packet(pkt_ref_ptr);
      PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
      return -1;
    }
    if (PS_TX_META_GET_PKT_META_INFO_PTR(logical_mi_ptr) == NULL)
    {
      LOG_MSG_ERROR_INT_0("ps_ifacei_logical_default_tx_cmd(): "
                      "NULL pkt meta info, Dropping packet");
      DPL_LOG_NETWORK_TX_DROPPED_PACKET
        (assoc_iface_ptr,*pkt_ref_ptr,DPL_IID_NETPROT_IP);
      dsm_free_packet(pkt_ref_ptr);
      PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
      return -1;
    }
    PS_TX_META_INFO_DUP(logical_mi_ptr, &local_mi_ptr);
  }
  else
  {
    /*-----------------------------------------------------------------------
      1. Extract the flow_ptr from the meta-info. Under following conditions,
         which could happen if logical flow got deleted in the middle of data
         transfer, use the default flow_ptr of this iface.
           a. if flow is invalid OR
           b. if flow is not logical OR
           c. if flow doesn't have logical meta info
      2. If "flags" is set in the meta-info passed in, allocate
         new meta-info and copy the flow_ptr's meta-info into it. Also copy
         the flags from the meta-info passed in.
      3. If "flags" is NOT set in the meta-info passed in duplicate
         the flow_ptr's meta-info.
      4. Pass the new meta-info (from 2. or 3.) downstream, and free the
         meta-info passed in.
      5. If the passed metainfo contains a valid packet info, just poupulate
         in the outgoing metainfo.
      6. In Steps 1-5, if a new metainfo is not created, then just forward
         the incoming metainfo as it is, if it contain a valid packet info.�
    -----------------------------------------------------------------------*/
    /* STEP 1 */
    logical_flow_ptr = PS_FLOW_GET_FLOW_FROM_META_INFO(meta_info_ptr);
    if (!PS_FLOW_IS_VALID(logical_flow_ptr) ||
        PS_FLOWI_GET_ASSOC_PS_FLOW(logical_flow_ptr) == NULL)
    {
      logical_flow_ptr = PS_IFACE_GET_DEFAULT_FLOW(this_iface_ptr);
    }

    logical_mi_ptr = PS_FLOW_GET_META_INFO_FROM_FLOW(logical_flow_ptr);
    if (logical_mi_ptr == NULL)
    {
      LOG_MSG_INFO1_1("ps_ifacei_logical_default_tx_cmd(): "
                      "Using default flow as meta info couldn't be obtained "
                      "from logical flow 0x%p", logical_flow_ptr);

      logical_flow_ptr = PS_IFACE_GET_DEFAULT_FLOW(this_iface_ptr);
      logical_mi_ptr = PS_FLOW_GET_META_INFO_FROM_FLOW(logical_flow_ptr);
      if (logical_mi_ptr == NULL)
      {
        LOG_MSG_ERROR_INT_0("ps_ifacei_logical_default_tx_cmd(): "
                        "logical_mi_ptr NULL, Dropping packet");
        DPL_LOG_NETWORK_TX_DROPPED_PACKET
          (assoc_iface_ptr,*pkt_ref_ptr,DPL_IID_NETPROT_IP);
        dsm_free_packet(pkt_ref_ptr);
        PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
        return -1;
      }
    }

    /* STEP 2 */
    if (PS_TX_META_GET_PKT_META_INFO_PTR(meta_info_ptr) != NULL &&
        PS_TX_META_GET_TX_FLAGS(meta_info_ptr) != 0)
    {
      /* Create a new TX meta info here */
      PS_TX_META_INFO_GET_ALL(local_mi_ptr);

      if (PS_TX_META_INFO_ALL_IS_NULL(local_mi_ptr) == TRUE)
      {
        PS_TX_META_INFO_FREE(&meta_info_ptr);
        PS_TX_META_INFO_FREE(&local_mi_ptr);
        LOG_MSG_ERROR_INT_0("ps_ifacei_logical_default_tx_cmd(): "
                        "Out of metainfo items, Dropping packet");
        DPL_LOG_NETWORK_TX_DROPPED_PACKET
          (assoc_iface_ptr,*pkt_ref_ptr,DPL_IID_NETPROT_IP);
        dsm_free_packet(pkt_ref_ptr);
        PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
        DS_UTILS_ERROR_FATAL("ps_ifacei_logical_default_tx_cmd(): Out of metainfo items");
        return -1;
      }

      PS_TX_META_INFO_COPY(logical_mi_ptr, local_mi_ptr);

      PS_TX_META_SET_TX_FLAGS(local_mi_ptr,
                              PS_TX_META_GET_TX_FLAGS(meta_info_ptr));

      PS_TX_META_SET_DOS_ACK_HANDLE(local_mi_ptr,
                                    PS_TX_META_GET_DOS_ACK_HANDLE(meta_info_ptr));
    }
    else /* STEP 3 */
    {
      PS_TX_META_INFO_DUP(logical_mi_ptr, &local_mi_ptr);
    }

    /*-----------------------------------------------------------------------
      STEP 5 : Copy the Packet info [from incoming metainfo] if it is set.
      If a packet has already generated PktInfo, this will make that we dont
      end up creating multiple packet infos and also avoids frag handling.
      STEP 6: If local_mi_ptr is NULL here, just pass incoming metainfo
      as outgoing.
    -----------------------------------------------------------------------*/
    if (local_mi_ptr != NULL)
    {
      if (PS_TX_META_IS_PKT_INFO_VALID(meta_info_ptr))
      {
        /*-------------------------------------------------------------------
        Copy the packet info, do sanity check before copying.
        -------------------------------------------------------------------*/
        if (!(PS_TX_AND_RT_META_INFO_IS_NULL(local_mi_ptr)))
        {
          PS_TX_META_SET_PKT_INFO(local_mi_ptr,
                                  PS_TX_META_GET_PKT_INFO(meta_info_ptr));
        }
      }

      PS_TX_META_INFO_FREE(&meta_info_ptr);
    }
    else
    {
      local_mi_ptr = meta_info_ptr;
    }
  } /* else (PS_TX_AND_RT_META_INFO_IS_NULL(meta_info_ptr)... */
  
  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

  fi_result =
    PS_TX_META_GET_FILTER_RESULT(logical_mi_ptr, IP_FLTR_CLIENT_QOS_OUTPUT);

  ret_val = ps_iface_tx_cmd(assoc_iface_ptr, pkt_ref_ptr, local_mi_ptr);

  /*-----------------------------------------------------------------------
    Reset the packet info from metainfo sitting in logical iface.
  -----------------------------------------------------------------------*/
  if ( FALSE == PS_TX_AND_RT_META_INFO_IS_NULL(logical_mi_ptr) )
  {
    memset(&(PS_TX_META_GET_PKT_INFO(logical_mi_ptr)), 0,
           sizeof(ip_pkt_info_type));
  }

  if (fi_result == PS_IFACE_IPFLTR_NOMATCH ||
      fi_result == (uint32) PS_IFACE_GET_DEFAULT_FLOW(assoc_iface_ptr))
  {
    PS_TX_META_RESET_FILTER_RESULT(logical_mi_ptr, IP_FLTR_CLIENT_QOS_OUTPUT);
  }

  return ret_val;
} /* ps_ifacei_logical_default_tx_cmd() */


/*===========================================================================
FUNCTION PS_IFACE_TX_CMD()

DESCRIPTION
  This function will transmit to the iface specified - it checks if the state
  is UP, ROUTEABLE or CONFIGURING if it isn't then the interface must be a
  route on demand interface, if it is not, then the default tx functions is
  called (which frees the memory).

  Note: this function does not pay attention to the flow control state of the
        interface.  The client callback must do this.

  Note: This function will not free meta info and DSM items as corresponding
        Tx function is responsible for freeing up memory.

PARAMETERS
  this_iface_ptr:    ptr to interface control block on which to operate on.
  pkt_ref_ptr:       ref to dsm item ptr received for tx
  meta_info_ptr:     ptr to meta info (per pkt information)

RETURN VALUE
  0: if the packet was transmitted
 -1: if not

DEPENDENCIES
  None

SIDE EFFECTS
  The memory that was passed in will be freed by one of the tx functions.
===========================================================================*/
int ps_iface_tx_cmd
(
  ps_iface_type         *this_iface_ptr,
  dsm_item_type        **pkt_ref_ptr,
  ps_tx_meta_info_type  *meta_info_ptr
)
{
  ps_iface_tx_cmd_f_ptr_type tx_cmd;
  void                       *tx_cmd_info;
  boolean                    is_transmit = FALSE;
  errno_enum_type            ps_errno;
  void                      *fraghdl = NULL;
  int                        retval = 0;
  dsm_item_type             * frag_ptr = NULL;
  ps_tx_meta_info_type      *tmp_meta_info_ptr = NULL;
  ps_flow_type              *ps_flow_ptr = NULL;
  ps_phys_link_type         *ps_phys_link_ptr = NULL ;
  ip_pkt_info_type          *meta_pkt_info_ptr = NULL;
  uint64                     packet_len_in_bytes = 0;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (!PS_IFACE_IS_VALID(this_iface_ptr))
  {
    LOG_MSG_ERROR_INT_0("ps_iface_tx_cmd(): "
                    "Invalid iface , Dropping packet");
    DS_UTILS_ASSERT(0);
    dsm_free_packet(pkt_ref_ptr);
    PS_TX_META_INFO_FREE(&meta_info_ptr);
    return -1;
  }

  LOG_MSG_DATA_PATH_INFO_2("ps_iface_tx_cmd: iface: 0x%x:%d",
                           this_iface_ptr->name,
                           this_iface_ptr->instance);

  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

  tx_cmd = this_iface_ptr->iface_private.tx_cmd;
  tx_cmd_info = this_iface_ptr->iface_private.tx_cmd_info;

  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

  /*-------------------------------------------------------------------------
    The code below updates last IO time which is used for Network Statistics
    and Linger features which are relevant only when FEATURE_DATA_BRL is on
  -------------------------------------------------------------------------*/

  IFACE_INC_INSTANCE_STATS(this_iface_ptr, pkts_tx, 1);

  packet_len_in_bytes = dsm_length_packet(*pkt_ref_ptr);
  IFACE_INC_INSTANCE_STATS(this_iface_ptr, bytes_tx, packet_len_in_bytes);

  LOG_MSG_DATA_PATH_INFO_0(" ps_iface_tx_cmd Stats incremented");
  /*-------------------------------------------------------------------------
    For IPV4 ifaces, run the packet through the QOS filters to classify
    the packet appropriately.
  -------------------------------------------------------------------------*/
  if(PS_IFACEI_NUM_FILTERS(this_iface_ptr, IP_FLTR_CLIENT_QOS_OUTPUT) > 0)
  {
    LOG_MSG_DATA_PATH_INFO_0("ps_iface_tx_cmd: Iface has QoS filters registered");
    if(ps_pkt_info_filter_tx_pkt(this_iface_ptr,
                                 IP_FLTR_CLIENT_QOS_OUTPUT,
                                 pkt_ref_ptr,
                                 &meta_info_ptr,
                                 &ps_errno) < 0)
    {
      LOG_MSG_DATA_PATH_INFO_1("ps_iface_tx_cmd: ps_pkt_info_filter_tx_pkt returned ps_errono = %d",
                               ps_errno);
      switch(ps_errno)
      {
        /*-------------------------------------------------------------------
          Insufficient information to filter, packet consumed, return.
        -------------------------------------------------------------------*/
        case E_WOULD_BLOCK:
          /*--------------------------------------------------------------------
            IP fragment scenario! Set the iface in the corresponding Frag handle
            and free the Meta Info.
          --------------------------------------------------------------------*/
          meta_pkt_info_ptr = &(PS_TX_META_GET_PKT_INFO(meta_info_ptr));
          ps_ip_fraghdl_set_bridge_device(meta_pkt_info_ptr->fraghdl,
                                          (void *)this_iface_ptr,
                                          IP_FRAGHDL_BRIDGE_TYPE_IFACE_BRIDGE,
                                          ps_iface_frag_tx_cmd);
          PS_TX_META_INFO_FREE(&meta_info_ptr);
          return 0;
		  
        case E_NOT_SUPPORTED:
          LOG_MSG_INFO1_1("ps_iface_tx_cmd(): "
                          "Got an unsupported protocol: packet routed to the default bearer" 
                          "reason: %d ", ps_errno );
          break;
        case E_INVALID_ARG:
        case E_BAD_DATA:
        case E_VERSION_MISMATCH:
        case E_NO_MEMORY:
        default:
          /*-------------------------------------------------------------------
            Insufficient information to filter, packet consumed, return.
          -------------------------------------------------------------------*/
          LOG_MSG_ERROR_INT_1("ps_iface_tx_cmd(): "
                          "Dropping packet, reason: %d ", ps_errno );
          IFACE_INC_INSTANCE_STATS(this_iface_ptr, pkts_dropped_tx, 1);
          PS_TX_META_INFO_FREE( &meta_info_ptr );
          DPL_LOG_NETWORK_TX_DROPPED_PACKET
            (this_iface_ptr,*pkt_ref_ptr,DPL_IID_NETPROT_IP);
          dsm_free_packet( pkt_ref_ptr );
          return -1;
      }
    }
  }

 /*-------------------------------------------------------------------------
    Header compression filtering
  -------------------------------------------------------------------------*/
  if(PS_IFACEI_NUM_FILTERS(this_iface_ptr, IP_FLTR_CLIENT_HEADER_COMP) > 0)
  {
    if(ps_pkt_info_filter_tx_pkt(this_iface_ptr,
                                 IP_FLTR_CLIENT_HEADER_COMP,
                                 pkt_ref_ptr,
                                 &meta_info_ptr,
                                 &ps_errno) < 0)
    {
      switch(ps_errno)
      {
        /*-------------------------------------------------------------------
          Insufficient information to filter, packet consumed, return.
        -------------------------------------------------------------------*/
        case E_WOULD_BLOCK:
        
        LOG_MSG_INFO1_0(" ps_iface_tx_cmd IP fragment scenario! Set the iface in the corresponding Frag handle"
                          "and free the Meta Info.");
          /*--------------------------------------------------------------------
            IP fragment scenario! Set the iface in the corresponding Frag handle
            and free the Meta Info.
          --------------------------------------------------------------------*/
          meta_pkt_info_ptr = &(PS_TX_META_GET_PKT_INFO(meta_info_ptr));
          ps_ip_fraghdl_set_bridge_device(meta_pkt_info_ptr->fraghdl,
                                          (void *)this_iface_ptr,
                                          IP_FRAGHDL_BRIDGE_TYPE_IFACE_BRIDGE,
                                          ps_iface_frag_tx_cmd);
          PS_TX_META_INFO_FREE(&meta_info_ptr);
          return 0;

        case E_INVALID_ARG:
        case E_BAD_DATA:
        case E_VERSION_MISMATCH:
        case E_NO_MEMORY:
        default:
          LOG_MSG_ERROR_1("ps_iface_tx_cmd(): "
                          "Dropping packet, reason: %d ", ps_errno );
          IFACE_INC_INSTANCE_STATS(this_iface_ptr, pkts_dropped_tx, 1);
          PS_TX_META_INFO_FREE( &meta_info_ptr );
          DPL_LOG_NETWORK_TX_DROPPED_PACKET
            (this_iface_ptr,*pkt_ref_ptr,DPL_IID_NETPROT_IP);
          dsm_free_packet( pkt_ref_ptr );
          return -1;
      }
    }
  }

  /* Get Flow ptr from Metainfo, to log the packet accordingly!
     Also, update stats (avoid multiple NULL checks! */
  if ((meta_info_ptr != NULL) &&
      (PS_TX_META_GET_RT_META_INFO_PTR(meta_info_ptr) != NULL))
  {
    ps_flow_ptr = PS_FLOW_GET_FLOW_FROM_META_INFO(meta_info_ptr);

    /*-------------------------------------------------------------------------
     Check whether the packet is destined for a multicast address.
     Today, scope is set for packets going over WLAN iface only so this stat
     are not reliable
    -------------------------------------------------------------------------*/
    if (PS_TX_META_GET_IP_ADDR_SCOPE( meta_info_ptr) == IP_ADDR_MULTICAST)
    {
      IFACE_INC_INSTANCE_STATS(this_iface_ptr, mcast_pkts_tx, 1);
    }
  }

  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);  

  if (! PS_FLOW_IS_VALID(ps_flow_ptr))
  {
    ps_flow_ptr = PS_IFACEI_GET_DEFAULT_FLOW(this_iface_ptr);
  }

  /*-------------------------------------------------------------------------
   Drop the packet if flow is not valid
  -------------------------------------------------------------------------*/  
  if (! PS_FLOW_IS_VALID(ps_flow_ptr))
  {
    LOG_MSG_ERROR_1("PS flow invalid 0x%p Dropping packet", ps_flow_ptr);
    IFACE_INC_INSTANCE_STATS(this_iface_ptr, pkts_dropped_tx, 1);
    DPL_LOG_NETWORK_TX_DROPPED_PACKET
      (this_iface_ptr,*pkt_ref_ptr,DPL_IID_NETPROT_IP);
    PS_TX_META_INFO_FREE( &meta_info_ptr );
    dsm_free_packet( pkt_ref_ptr );
    PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);
    return -1;  
  }
  
  /*-------------------------------------------------------------------------
   Check if packets need to be dropped on the flow.
  -------------------------------------------------------------------------*/
  if ((PS_FLOW_GET_CAPABILITY(ps_flow_ptr, PS_FLOW_CAPABILITY_DATA_DISALLOWED)))
  {
    LOG_MSG_ERROR_INT_0("PS_FLOW_CAPABILITY_DATA_DISALLOWED:Dropping packet");
    IFACE_INC_INSTANCE_STATS(this_iface_ptr, pkts_dropped_tx, 1);
    PS_TX_META_INFO_FREE( &meta_info_ptr );
    DPL_LOG_NETWORK_TX_DROPPED_PACKET
      (this_iface_ptr,*pkt_ref_ptr,DPL_IID_NETPROT_IP);
    dsm_free_packet( pkt_ref_ptr );
    PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);
    return -1;
  }

  FLOW_INC_INSTANCE_STATS(ps_flow_ptr, pkts_tx, 1);
  DPL_LOG_FLOW_TX_PACKET(ps_flow_ptr, *pkt_ref_ptr, DPL_IID_NETPROT_IP);

  ps_phys_link_ptr = PS_FLOW_GET_PHYS_LINK(ps_flow_ptr);
  
  /*-------------------------------------------------------------------------
   Drop the packet if physlink is not valid
  -------------------------------------------------------------------------*/    
  if (NULL != ps_phys_link_ptr)
  {
    PHYS_LINK_INC_INSTANCE_STATS(ps_phys_link_ptr, pkts_tx, 1);
  }

  LOG_MSG_DATA_PATH_INFO_0(" ps_iface_tx_cmd : Flow physlink stats updated");
  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);
  
  /*-------------------------------------------------------------------------
    If the interface is ROUTEABLE or UP call the TX function
  -------------------------------------------------------------------------*/
  if(PS_IFACEI_GET_STATE(this_iface_ptr) != IFACE_DISABLED  &&
     PS_IFACEI_GET_STATE(this_iface_ptr) != IFACE_DOWN      &&
     PS_IFACEI_GET_STATE(this_iface_ptr) != IFACE_COMING_UP)
  {
    LOG_MSG_DATA_PATH_INFO_0(" ps_iface_tx_cmd : transmit true");
    is_transmit = TRUE;
  }

  /*-------------------------------------------------------------------------
    Transmit the packet if transmit flag is TRUE
  -------------------------------------------------------------------------*/
  if (TRUE == is_transmit)
  {
    /*-----------------------------------------------------------------------
      Send the queued fragments, if Metainfo is generated already.
      Do explicit TCP ACK prioritization & tag the DSM packet, if metainfo is
      not generated by this time.
      - If Metainfo is generated already - no need to do explicit tagging
        (TCP ACK tagging happens implicitly while generating packet info).
    -----------------------------------------------------------------------*/
    if(meta_info_ptr != NULL &&
      (PS_TX_META_GET_RT_META_INFO_PTR( meta_info_ptr) != NULL) &&
       PS_TX_META_IS_PKT_INFO_VALID( meta_info_ptr ))
    {
      /*---------------------------------------------------------------------
        Fragment Case!
      ---------------------------------------------------------------------*/
      if (NULL != (fraghdl = (&(PS_TX_META_GET_PKT_INFO( meta_info_ptr)))->fraghdl) &&
        ( TRUE != ps_ip_fraghdl_is_local(fraghdl) ))
        /* (TRUE != (PS_TX_META_GET_PKT_INFO(meta_info_ptr)).is_local_frag)) */
    {
      while( NULL != (frag_ptr = ip_fraghdl_get_fragment(fraghdl )) )
      {
        PS_TX_META_INFO_DUP(meta_info_ptr, &tmp_meta_info_ptr);
        /*---------------------------------------------------------------------
          Log packet on the Tx side
        ---------------------------------------------------------------------*/
        DPL_LOG_NETWORK_TX_PACKET(this_iface_ptr,
                                  frag_ptr,
                                  DPL_IID_NETPROT_IP);

        LOG_MSG_DATA_PATH_INFO_2( " ps_iface_tx_cmd : calling tx_cmd on iface "
                                  "0x%p, instance %d",
                                  PS_IFACE_IS_VALID(this_iface_ptr)?
                                    this_iface_ptr->name:0,
                                  PS_IFACE_IS_VALID(this_iface_ptr)?
                                    this_iface_ptr->instance:0 
                                 );
        if( -1 == tx_cmd(this_iface_ptr,
                         &frag_ptr,
                         tmp_meta_info_ptr,
                         tx_cmd_info) )
        {
          IFACE_INC_INSTANCE_STATS(this_iface_ptr, pkts_dropped_tx, 1);
          retval = -1;
          LOG_MSG_ERROR_2("ps_iface_tx_cmd(): "
                          "Failed to transmit data on iface 0x%x:%d",
                          this_iface_ptr->name, this_iface_ptr->instance);
          PS_TX_META_INFO_FREE( &tmp_meta_info_ptr );
        }
      }
    }
    }
    /*---------------------------------------------------------------------
      Explicit TCP ACK Prioritization tagging:
      -  Check if TCK ACK PRIORITY NV item is enabled.
      -  Tag the packet if its a TCP ACK packet, we hit this block if the
         packet info generation doesn't happen during filter execution.
    ---------------------------------------------------------------------*/
    else
    {
      /* Check for the TCP_ACK_PRIORITY NV item */
      if( ps_pkt_info_get_tcp_ack_prio_enabled() )
      {
#define PKT_SIZE 80
        if (packet_len_in_bytes <= PKT_SIZE)
        {
          if (TCP_ACK_REG == ps_pkt_info_check_tcp_ack(*pkt_ref_ptr,
                                                       &ps_errno))
          {
            dsm_item_set_priority(*pkt_ref_ptr, DSM_QUEUE_PRIORITY_HIGH);
          }
        }
      }
    }

    /*-----------------------------------------------------------------------
      Log packet on the Tx side
    -----------------------------------------------------------------------*/
    DPL_LOG_NETWORK_TX_PACKET(this_iface_ptr,
                              *pkt_ref_ptr,
                              DPL_IID_NETPROT_IP);

    LOG_MSG_DATA_PATH_INFO_0(" ps_iface_tx_cmd : calling tx_cmd");                                    
    /*-----------------------------------------------------------------------
      Send this packet
    -----------------------------------------------------------------------*/
    if( -1 == tx_cmd(this_iface_ptr,
                     pkt_ref_ptr,
                     meta_info_ptr,
                     tx_cmd_info) )
    {
      IFACE_INC_INSTANCE_STATS(this_iface_ptr, pkts_dropped_tx, 1);
      retval = -1;
      LOG_MSG_ERROR_2("ps_iface_tx_cmd(): "
                      "Failed to transmit data on iface 0x%x:%d",
                      this_iface_ptr->name, this_iface_ptr->instance);
    }
  }
  /*-------------------------------------------------------------------------
    Otherwise Free the packet.
    If the IFACE state is invalid, the packet shouldn't have reached here.
  -------------------------------------------------------------------------*/
  else
  {
    LOG_MSG_ERROR_1("ps_iface_tx_cmd(): "
                    "IFACE state: 0x%x , Dropping packet!",
                    PS_IFACEI_GET_STATE(this_iface_ptr) );
    IFACE_INC_INSTANCE_STATS(this_iface_ptr, pkts_dropped_tx, 1);
    PS_META_INFO_FREE( &meta_info_ptr );
    DPL_LOG_NETWORK_TX_DROPPED_PACKET
      (this_iface_ptr,*pkt_ref_ptr,DPL_IID_NETPROT_IP);

    dsm_free_packet( pkt_ref_ptr );
    retval = -1;
  }

  return retval;
} /* ps_iface_tx_cmd() */

/*===========================================================================
FUNCTION PS_IFACE_INPUT()

DESCRIPTION
   Interface client gives the protocol stack a packet to transmit.

PARAMETERS
  this_iface_ptr:    ptr to interface control block on which to operate on.
  pkt_ref_ptr:       ref to dsm item ptr received
  meta_info_ptr:     ptr to meta info (TX/RX meta information)

RETURN VALUE
  0: data was transfered
 -1: data was not transferred, but memory was freed

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
int ps_iface_input
(
  ps_iface_type         *this_iface_ptr,
  dsm_item_type        **pkt_ref_ptr,
  ps_meta_info_type_ex  *meta_info_ex_ptr
)
{
  ps_tx_meta_info_type  * tx_meta_info_ptr = NULL;
  ps_rx_meta_info_type  * rx_meta_info_ptr = NULL;
  boolean             dl_opt_rval;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (pkt_ref_ptr == NULL || *pkt_ref_ptr == NULL)
  {
    IFACE_INC_INSTANCE_STATS(this_iface_ptr, pkts_dropped_tx, 1);
    DS_UTILS_ERROR_FATAL("ps_iface_input(): Packet ptr is NULL!");
    return -1;
  }
  
  if (!PS_IFACE_IS_VALID(this_iface_ptr))
  {
    LOG_MSG_INFO1_1("ps_iface_input(): "
                    "rcvd data on invalid iface 0x%p freeing!",
                    this_iface_ptr);
    dsm_free_packet(pkt_ref_ptr);
    return -1;
  }  

  /* Copy TX / RX meta information from incoming meta_info_ex_ptr,  */
  if(meta_info_ex_ptr != NULL)
  {
    tx_meta_info_ptr = meta_info_ex_ptr->tx_meta_info_ptr;
    rx_meta_info_ptr = meta_info_ex_ptr->rx_meta_info_ptr;
    /* Clear meta_info_ex_ptr */
    PS_META_INFO_FREE_EX(&meta_info_ex_ptr);
  }

  /*-------------------------------------------------------------------------
    Run DL opt filters if OPT Rm iface is not NULL
  -------------------------------------------------------------------------*/
  if (PS_IFACEI_GET_OPT_RM_IFACE(this_iface_ptr) != NULL)
  {
    dl_opt_rval = ps_dl_iface_opt_rx_pkt(this_iface_ptr, pkt_ref_ptr, &rx_meta_info_ptr);
    if (TRUE == dl_opt_rval || NULL == *pkt_ref_ptr)
  {
    PS_RX_META_INFO_FREE(&rx_meta_info_ptr);
    PS_TX_META_INFO_FREE(&tx_meta_info_ptr);
      return (TRUE == dl_opt_rval) ? 0 : -1;
    }
  }

  return ps_iface_input_internal(this_iface_ptr,
                                 pkt_ref_ptr,
                                 rx_meta_info_ptr,
                                 tx_meta_info_ptr);
} /* ps_iface_input() */


/*===========================================================================
FUNCTION PS_IFACE_FRAG_TX_CMD()

DESCRIPTION
  This function is called when Frag handle time out happens which flushes
  all the packets inside FragQ using this TX function.

  This function transmits out the passed IP fragment, by calling
  tx_cmd of specified iface.

PARAMETERS
  device_ptr:    Iface
  dsm_item_ptr:  Data Packet
  meta_info_ptr: Meta Info associated with the Packet

RETURN VALUE
 0 for success, -1 in case of an error.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
int ps_iface_frag_tx_cmd
(
  void                 * device_ptr,
  dsm_item_type       ** dsm_item_ptr,
  ps_tx_meta_info_type * meta_info_ptr
)
{

  ps_iface_tx_cmd_f_ptr_type  tx_cmd;
  void                       *tx_cmd_info;
  ps_iface_type              *this_iface_ptr = (ps_iface_type *)device_ptr;

  /*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/
  if ( (!PS_IFACE_IS_VALID(this_iface_ptr)) || (dsm_item_ptr == NULL) )
  {
    LOG_MSG_ERROR_INT_0("ps_iface_frag_tx_cmd(): "
                    "Iface/dsm item is NULL");
    return -1;
  }

  /*-------------------------------------------------------------------------
    Send the packet directly onto the iface, do not use ps_iface_tx_cmd here,
    as there are chances that packet will be queued again.
  -------------------------------------------------------------------------*/
  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);
  tx_cmd = this_iface_ptr->iface_private.tx_cmd;
  tx_cmd_info = this_iface_ptr->iface_private.tx_cmd_info;
  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

  /*---------------------------------------------------------------------
    Log packet on the Tx side
  ---------------------------------------------------------------------*/
  DPL_LOG_NETWORK_TX_PACKET(this_iface_ptr,
                            *dsm_item_ptr,
                            DPL_IID_NETPROT_IP);

  if( -1 == tx_cmd(this_iface_ptr,
                         dsm_item_ptr,
                   meta_info_ptr,
                   tx_cmd_info) )
  {
    IFACE_INC_INSTANCE_STATS(this_iface_ptr, pkts_dropped_tx, 1);
    LOG_MSG_ERROR_2("ps_iface_frag_tx_cmd(): "
                    "Failed to transmit data on iface 0x%x:%d",
                    this_iface_ptr->name, this_iface_ptr->instance);
    return -1;
  }

  return 0;
} /* ps_iface_frag_tx_cmd() */


