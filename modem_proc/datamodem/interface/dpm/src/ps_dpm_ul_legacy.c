/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                    P S _ D P M _ L E G A C Y . C

GENERAL DESCRIPTION
  This is the implementation of  legacy path Data Path Manager module

Copyright (c) 2013 QUALCOMM Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                            EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dpm/src/ps_dpm_ul_legacy.c#3 $
  $Author: pwbldsvc $ $DateTime: 2021/08/02 22:53:02 $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
09/10/13    bvd    created file.
===========================================================================*/
/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

                                INCLUDE FILES

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
#include "ps_dpmi.h"
#include "ps_iface.h"
#include "ps_lan_llc.h"
#include "ps_handle_mgr.h"
#include "ds_qmi_svc.h"
#include "ps_logging.h"

dsm_watermark_type     ps_dpm_ul_legacy_wm;
q_type                 ps_dpm_ul_legacy_wm_q;

extern ps_crit_sect_type         ps_dpm_crit_section;

static ps_meta_info_type_ex    * meta_info_ex_ptr;
static ps_tx_meta_info_type    * tx_meta_info_ptr;

static boolean legacy_wm_flow_enabled = TRUE;

#define   PS_DPM_UL_LEGACY_WM_LOW   15000
#define   PS_DPM_UL_LEGACY_WM_HIGH  45000
#define   PS_DPM_UL_LEGACY_WM_DNE   800000

#define PS_DPM_VALID_IFACE_STATE_FOR_DATA_PATH                           \
          ~(IFACE_STATE_INVALID | IFACE_DISABLED | IFACE_DOWN)

/*===========================================================================

                             INTERNAL FUNCTIONS

===========================================================================*/
static void ps_dpm_ul_legacyi_non_empty_data_cback
(
 dsm_watermark_type  * wm,
 void                * cb_data
)
{
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /*-------------------------------------------------------------------------
    Set Rx Signal
  -------------------------------------------------------------------------*/
  PS_SET_SIGNAL(PS_DPM_UL_LEGACY_SIG);

} /* ps_dpm_ul_legacyi_non_empty_data_cback() */


static void ps_dpm_ul_legacyi_wm_low_cb_func
(
  struct dsm_watermark_type_s * legacy_wm_ptr,
  void                        * user_data_ptr
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  LOG_MSG_DATA_PATH_INFO_0 ("In ps_dpm_ul_legacyi_wm_low_cb_func");

  legacy_wm_flow_enabled = TRUE;
  
  /*-------------------------------------------------------------------------
    Once legacy wm goes from flow disabled to flow enabled state, it has to 
    flow enable all the wm on DPM RM. In ps_dpm_rm_enable_all_flows, loop is 
    ran through all DPM RMs to enable flow on each of them by setting the
    DPM UL signal
  -------------------------------------------------------------------------*/
  ps_dpm_rm_enable_all_flows();

}/*ps_dpm_ul_legacyi_wm_low_cb_func()*/

static void ps_dpm_ul_legacyi_wm_high_cb_func
(
  struct dsm_watermark_type_s * legacy_wm_ptr,
  void                        * user_data_ptr
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_DATA_PATH_INFO_0 ("In ps_dpm_ul_legacyi_wm_high_cb_func");

  legacy_wm_flow_enabled = FALSE;

}/*ps_dpm_ul_legacyi_wm_high_cb_func()*/

boolean ps_dpm_ul_legacyi_sig_hdlr
(
  ps_sig_enum_type     sig,
  void               * user_data
)
{
  ps_meta_info_type_ex        *  dup_meta_info_ex_ptr = NULL;
  dsm_item_type               *  legacy_rx_pkt;
  ps_iface_type               *  rm_iface_ptr = NULL;
  ps_iface_type               *  um_iface_ptr = NULL;
  ps_dpm_rm_info_type         *  dpm_rm_info_ptr = NULL;
  uint16                         bytes_read;
  ps_dpm_qos_hdr_type            qos_hdr;
  ip_version_enum_type           ip_ver;
  lan_lle_enum_type              lle_instance;
  rmnet_sm_link_prot_e_type      link_prot;
  uint8                          data_format;
  boolean                        enable      = FALSE;
  sint15                         ps_errno;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  legacy_rx_pkt = dsm_dequeue(&ps_dpm_ul_legacy_wm);
  if (NULL == legacy_rx_pkt)
  {
    return TRUE;
  }

  /*-------------------------------------------------------------------------
    Validate Rm iface and get it's IP family
  -------------------------------------------------------------------------*/
  rm_iface_ptr = (ps_iface_type *) (legacy_rx_pkt->app_ptr);
  if (!PS_IFACE_IS_VALID(rm_iface_ptr))
  {
    LOG_MSG_ERROR_INT_0("ps_dpm_ul_legacyi_sig_hdlr():"
                    "Invalid Rm iface, Dropping packet");
    dsm_free_packet(&legacy_rx_pkt);
    return FALSE;
  }

  ip_ver =
    (IPV4_ADDR == ps_iface_get_addr_family(rm_iface_ptr)) ? IP_V4 : IP_V6;

  /*-------------------------------------------------------------------------
    Fetch DPM RM using app_field of DSM item and cache the needed info in
    local variables
  -------------------------------------------------------------------------*/
  PS_ENTER_CRIT_SECTION(&ps_dpm_crit_section);

  dpm_rm_info_ptr = ps_dpm_get_dpm_rm_info_by_handle(legacy_rx_pkt->app_field);
  if (NULL == dpm_rm_info_ptr)
  {
    LOG_MSG_ERROR_INT_0("ps_dpm_ul_legacyi_sig_hdlr():"
                    "Invalid dpm_rm info, Dropping packet");
    DPL_LOG_NETWORK_RX_DROPPED_PACKET
      (rm_iface_ptr,legacy_rx_pkt,DPL_IID_NETPROT_IP);
    dsm_free_packet(&legacy_rx_pkt);
    PS_LEAVE_CRIT_SECTION(&ps_dpm_crit_section);
    return FALSE;
  }

  link_prot        = dpm_rm_info_ptr->link_prot;
  data_format      = dpm_rm_info_ptr->data_format;

  lle_instance = (IP_V4 == ip_ver) ? dpm_rm_info_ptr->v4_lan_llc_instance
                                   : dpm_rm_info_ptr->v6_lan_llc_instance;

  PS_LEAVE_CRIT_SECTION(&ps_dpm_crit_section);

  /*-------------------------------------------------------------------------
    Extract QoS header from DSM item if QoS is negotiated as data format
  -------------------------------------------------------------------------*/
  memset((void *) &qos_hdr, 0, sizeof(ps_dpm_qos_hdr_type));

  if (PS_DPM_QOS_ENABLED == data_format)
  {
    bytes_read = dsm_pullup(&legacy_rx_pkt, (byte *) &qos_hdr, sizeof(qos_hdr));
    if (sizeof(qos_hdr) != bytes_read)
    {
      LOG_MSG_ERROR_INT_0("ps_dpm_ul_legacyi_sig_hdlr():"
                      "Invalid qos header, Dropping packet");
      DPL_LOG_NETWORK_RX_DROPPED_PACKET
        (rm_iface_ptr,legacy_rx_pkt,DPL_IID_NETPROT_IP);
      dsm_free_packet(&legacy_rx_pkt);
      return FALSE;
    }
  }

  /*-------------------------------------------------------------------------
    Update meta info if Um iface is not in DOWN state. Else, just send the
    packet up the stack to Rm iface and let the framework process the packet
  -------------------------------------------------------------------------*/
  um_iface_ptr = PS_IFACEI_GET_BRIDGE_IFACE(rm_iface_ptr);

  if (PS_DPM_VALID_IFACE_STATE_FOR_DATA_PATH & ps_iface_state(um_iface_ptr))
  {
    PS_TX_META_RESET_FILTER_RESULT(tx_meta_info_ptr, IP_FLTR_CLIENT_QOS_OUTPUT);
    meta_info_ex_ptr->tx_meta_info_ptr = tx_meta_info_ptr;
    PS_META_INFO_DUP(meta_info_ex_ptr, &dup_meta_info_ex_ptr);

    if(NULL == dup_meta_info_ex_ptr)
    {
      LOG_MSG_ERROR_INT_0("ps_dpm_ul_legacyi_sig_hdlr():"
                          "dup_meta_info_ex_ptr is NULL, Dropping packet");
      DPL_LOG_NETWORK_RX_DROPPED_PACKET
      (rm_iface_ptr,legacy_rx_pkt,DPL_IID_NETPROT_IP);
      dsm_free_packet(&legacy_rx_pkt);
      return FALSE;
    }

    if (PS_DPM_QOS_ENABLED == data_format)
    {
      ps_dpm_process_qos_hdr(legacy_rx_pkt,
                             um_iface_ptr,
                             ip_ver,
                             FALSE,
                             dup_meta_info_ex_ptr->tx_meta_info_ptr,
                             &qos_hdr,
                             NULL);
    }
  }

/*-------------------------------------------------------------------
   Execute powersave filters on incoming packet in legacy path. 
-------------------------------------------------------------------*/
  if( TRUE == PS_IFACE_GET_POWERSAVE_FILTERING_MODE(rm_iface_ptr) )
  {
    if( PS_IFACE_GET_POWERSAVE_FILTERING_AUTO_EXIT_MODE(rm_iface_ptr) )
    {
      ps_iface_set_powersave_filtering_mode( rm_iface_ptr, enable, 
                                            &ps_errno );
    }
  }
          
  if (RMNET_IP_MODE == link_prot)
  {
    (void) ps_iface_input(rm_iface_ptr, &legacy_rx_pkt, dup_meta_info_ex_ptr);
  }
  else
  {
    (void) lan_llc_rx_pkt(lle_instance, legacy_rx_pkt, dup_meta_info_ex_ptr);
  }

  return FALSE;
} /* ps_dpm_ul_legacyi_sig_hdlr() */


/*===========================================================================

                             EXTERNAL FUNCTIONS

===========================================================================*/
void ps_dpm_ul_legacy_init
(
  void
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Initiatlize the WM
  -------------------------------------------------------------------------*/
  dsm_queue_init(&ps_dpm_ul_legacy_wm,
                 PS_DPM_UL_LEGACY_WM_DNE,
                 &ps_dpm_ul_legacy_wm_q);

  dsm_set_low_wm(&ps_dpm_ul_legacy_wm, PS_DPM_UL_LEGACY_WM_LOW);
  dsm_set_hi_wm(&ps_dpm_ul_legacy_wm, PS_DPM_UL_LEGACY_WM_HIGH);
  dsm_set_dne(&ps_dpm_ul_legacy_wm, PS_DPM_UL_LEGACY_WM_DNE);

  ps_dpm_ul_legacy_wm.non_empty_func_data = NULL;
  ps_dpm_ul_legacy_wm.non_empty_func_ptr  =
    ps_dpm_ul_legacyi_non_empty_data_cback;

  ps_dpm_ul_legacy_wm.lowater_func_data = NULL;
  ps_dpm_ul_legacy_wm.lowater_func_ptr  =  
    ps_dpm_ul_legacyi_wm_low_cb_func;
    
  ps_dpm_ul_legacy_wm.hiwater_func_data = NULL;
  ps_dpm_ul_legacy_wm.hiwater_func_ptr  =  
    ps_dpm_ul_legacyi_wm_high_cb_func;
    
  /*-------------------------------------------------------------------------
    Initialize signal used for legacy path
  -------------------------------------------------------------------------*/
  (void) ps_set_sig_handler(PS_DPM_UL_LEGACY_SIG,
                            ps_dpm_ul_legacyi_sig_hdlr,
                            NULL);
  ps_enable_sig(PS_DPM_UL_LEGACY_SIG);

  /*-------------------------------------------------------------------------
    Pre-allocate meta info at powerup so that it is not allocated every time
    in data path. This meta info is DUPed and sent along with each legacy pkt
  -------------------------------------------------------------------------*/
  PS_META_INFO_GET_EX(meta_info_ex_ptr);
  PS_TX_META_INFO_GET_ALL(tx_meta_info_ptr);
  if (NULL == meta_info_ex_ptr || NULL == tx_meta_info_ptr)
  {
    DS_UTILS_ERROR_FATAL("ps_dpm_ul_legacy_init(): Couldn't alloc meta info");
    return;
  }

  meta_info_ex_ptr->tx_meta_info_ptr = tx_meta_info_ptr;
  return;

} /* ps_dpm_ul_legacy_init() */


boolean ps_dpm_ul_legacy_input
(
  ps_iface_type        * rm_iface_ptr,
  dsm_item_type        * rx_pkt,
  ps_dpm_rm_info_type  * dpm_rm_info_ptr
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    If legacy watermark is not flow enabled, then return true
  -------------------------------------------------------------------------*/
  if (FALSE == legacy_wm_flow_enabled)
  {
    dpm_rm_info_ptr->rx_pkt = rx_pkt;
    return TRUE;
  }
  
  /*-------------------------------------------------------------------------
    Enqueue packet in to legacy WM. These packets will handled by the
    legacy sig hdlr in PS task
  -------------------------------------------------------------------------*/
  rx_pkt->app_field = dpm_rm_info_ptr->handle;
  rx_pkt->app_ptr   = rm_iface_ptr;
  dsm_enqueue(&ps_dpm_ul_legacy_wm, &rx_pkt);

  return FALSE;
} /* ps_dpm_ul_legacy_input() */