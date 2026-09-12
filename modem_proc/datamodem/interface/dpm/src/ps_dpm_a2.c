/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                             P S _ D P M _ A 2. C

GENERAL DESCRIPTION
  This is the implementation of the Data Path Manager module

Copyright (c) 2014-15 QUALCOMM Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                            EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dpm/src/ps_dpm_a2.c#2 $
  $Author: pwbldsvc $ $DateTime: 2019/10/22 02:53:17 $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
04/09/14    ash    Added Prefix Delegation support.
11/01/13    rk     Removed feature mobileap.
09/10/13    bvd    created file.
===========================================================================*/
/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

                                INCLUDE FILES

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/

#include "ps_dpm_hw.h"
#include "ps_rm_defs.h"
#include "ps_rm_svc.h"
#include "ps_logging.h"
#include "ps_dpm.h"
#include "ps_dpm_dpl.h"

#include "a2_dl_per.h"
#include "a2_log_ext.h"
#include "a2_ipa_if.h"

/*===========================================================================

                           DEFINES

===========================================================================*/



/*===========================================================================

                           GLOBAL DATA DECLARATIONS

===========================================================================*/

typedef ipa_acc_dpl_cb_fn_param_s  ps_dpm_hw_dpl_cb_fn_param_type;

/*-----------------------------------------------------------------------
  Indicates if hardware dpl logging is enabled
-----------------------------------------------------------------------*/
boolean ps_dpm_hw_dpl_enabled = FALSE;

/*-----------------------------------------------------------------------
  Indicates if hardware dpl logging is available. It doesnt mean that
  it has been enabled. DPM is master and it controls whether to enable
  or disable logging.
-----------------------------------------------------------------------*/
boolean ps_dpm_hw_dpl_available = FALSE;

/*===========================================================================

                           STATIC DATA DECLARATIONS

===========================================================================*/

void ps_dpmi_hw_wait_for_ev_cback
(
  void
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  (void) ps_rm_wait((rex_sigs_type) (1 << PS_RM_HW_EV_CBACK_SIGNAL));

  return;
} /* ps_dpmi_hw_wait_for_ev_cback() */

/*===========================================================================

                             INTERNAL FUNCTIONS

===========================================================================*/

/*===========================================================================
FUNCTION   ps_dpm_hw_dpl_cb()

DESCRIPTION
  This function is registered with hardware at powerup and is called by
  hardware to indicate that dpl on hardware is enabling.

DEPENDENCIES
  None.

PARAMETERS
  cb_param : Callback parameters

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
static void ps_dpm_hw_dpl_cb
(
  ps_dpm_hw_dpl_cb_fn_param_type * cb_param
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

   LOG_MSG_INFO1_1("ps_dpm_hw_dpl_cb : Hardware logging is available %d",
                    cb_param->is_available);

  /*-----------------------------------------------------------------------
     if hardware indicates that dpl is available then set 
     ps_dpm_hw_dpl_available to TRUE. DPL logging would available when
     log codes are enabled in qxdm
  -----------------------------------------------------------------------*/

  if (TRUE == cb_param->is_available)
  {
    ps_dpm_hw_dpl_available = TRUE;


    if (TRUE == log_status( LOG_DATA_PROTOCOL_LOGGING_NETWORK_IP_RM_TX_80_BYTES_C) ||
        TRUE == log_status( LOG_DATA_PROTOCOL_LOGGING_NETWORK_IP_RM_RX_80_BYTES_C) ||
        TRUE == log_status( LOG_DATA_PROTOCOL_LOGGING_NETWORK_IP_UM_TX_80_BYTES_C) ||
        TRUE == log_status( LOG_DATA_PROTOCOL_LOGGING_NETWORK_IP_UM_RX_80_BYTES_C))
    {    
      ps_dpm_hw_control_dpl_logging(DPL_PACKET_PARTIAL_LENGTH);
    }
  }
  else
  {
    /*-----------------------------------------------------------------------
    Indicates that hardware dpl logging is unavailable. 
    -----------------------------------------------------------------------*/
    ps_dpm_hw_dpl_available = FALSE;
    ps_dpm_hw_dpl_enabled   = FALSE;
  }
  
} /* ps_dpm_hw_dpl_cb */

static void ps_dpmi_hw_suspend_bearer_cb
(
  uint8                  uid,
  uint32                 cb_data
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO1_2("ps_dpmi_hw_suspend_bearer_cb(): uid %d cb_data %d",
                  uid, cb_data);

  PS_RM_SET_SIGNAL(PS_RM_HW_EV_CBACK_SIGNAL);

  return;
} /* ps_dpmi_hw_suspend_bearer_cb() */
 
/*===========================================================================

                             EXTERNAL FUNCTIONS

===========================================================================*/
int ps_dpm_hw_get_iface_stat_adjustment
(
  uint8                            uid,
  ps_iface_type                  * iface_ptr,  
  ps_dpm_hw_stats_info_type      * stats_ptr
)
{ 
  a2_dl_per_eps_throughput_stat_s stats;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (NULL == stats_ptr)
  {  
    LOG_MSG_ERROR_INT_0("stats_ptr is NULL");
    return -1;
  }

  LOG_MSG_INFO3_2("ps_dpm_hw_get_iface_stat_adjustment() : iface_ptr 0x%x:%d ", 
                  PS_IFACE_IS_VALID(iface_ptr)?iface_ptr->name:0,
                  PS_IFACE_IS_VALID(iface_ptr)?iface_ptr->instance:0);
  
  memset(stats_ptr, 0, sizeof(ps_dpm_hw_stats_info_type));
  memset(&stats, 0, sizeof(a2_dl_per_eps_throughput_stat_s));

  do
  {
    a2_dl_per_get_eps_throughput_stat(uid, &stats); 
    
    if(PS_IFACE_IS_ADDR_FAMILY_V4(iface_ptr) == TRUE)
    {
      stats_ptr->dl_stats.num_ipv4_bytes += stats.ipv4_num_bytes;
      stats_ptr->dl_stats.num_ipv4_pkts += (uint32)stats.ipv4_num_pkts;
    }
    else
    {
      stats_ptr->dl_stats.num_ipv6_bytes += stats.ipv6_num_bytes;
      stats_ptr->dl_stats.num_ipv6_pkts += (uint32)stats.ipv6_num_pkts;
    }
   
    return 0;
  } while(0);  
  
  LOG_MSG_ERROR_INT_0("ps_dpm_hw_get_iface_stat_adjustment() : Stat adj failed ");                 
  return -1;                  
} /* ps_dpm_hw_get_iface_stat_adjustment */

int ps_dpm_hw_reg_bearer
(
  uint8                      uid,
  uint8                      eps_id,
  uint32                     subs_id,
  dsm_watermark_type	   * l2_to_ps_wm_ptr,
  dsm_watermark_type	   * hw_to_ps_wm_ptr,
  sio_stream_id_type         sio_stream_id,
  ps_sys_rat_ex_enum_type    rat  
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-----------------------------------------------------------------------
    Register l2_to_ps wmk
  -----------------------------------------------------------------------*/
  a2_dl_per_register_pdcp_watermark(uid, l2_to_ps_wm_ptr);
  
  /*-----------------------------------------------------------------------
    Register a2 to ps with A2
  -----------------------------------------------------------------------*/
  a2_dl_per_register_ds_watermark(uid, hw_to_ps_wm_ptr);
                                 
  return 0;
} /* ps_dpm_hw_reg_bearer */

int ps_dpm_hw_dereg_bearer
(
  uint8                     uid,
  sio_port_id_type          sio_port_id,
  boolean                   is_data_path_bridged
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /*-----------------------------------------------------------------------
    Dereg DPL and unbridge the bearer if it is bridged. This is done only
    in case of A2
  -----------------------------------------------------------------------*/
  if (TRUE == is_data_path_bridged)
  {
    ps_dpm_hw_dereg_dpl(uid);
    if( sio_port_id != SIO_PORT_NULL )
    {
      ps_dpm_hw_unbridge_bearer(uid, sio_port_id);
    }
  }
  /*-----------------------------------------------------------------------
    Dereg l2_to_ps wmk
  -----------------------------------------------------------------------*/
  a2_dl_per_deregister_ds_watermark(uid);
  
  /*-----------------------------------------------------------------------
    Dereg a2 to ps with A2
  -----------------------------------------------------------------------*/
  a2_dl_per_deregister_pdcp_watermark(uid);

  //A2 handles above calls in calling task itself so it neednt have ev cback
                                 
  return 0;
} /* ps_dpm_hw_dereg_bearer */



int16 ps_dpm_hw_bridge_bearer
(
  uint8                            uid,
  sio_port_id_type                 sio_port_id 
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  a2_dl_per_bridge_stream(uid, A2_IPV4, sio_port_id);    
  
  a2_dl_per_bridge_stream(uid, A2_IPV6, sio_port_id); 
   
  return 0;
} /* ps_dpm_hw_bridge_bearer() */


int16 ps_dpm_hw_unbridge_bearer
(
  uint8                            uid,
  sio_port_id_type                 sio_port_id  
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  LOG_MSG_INFO1_2("ps_dpm_hw_unbridge_bearer() : "
                  "uid %d sio_port_id %d ", uid, sio_port_id);

  a2_dl_per_unbridge_stream(uid, A2_IPV4, sio_port_id); 
  
  a2_dl_per_unbridge_stream(uid, A2_IPV6, sio_port_id); 

  return 0;
} /* ps_dpm_hw_unbridge_bearer() */

int ps_dpm_hw_config_dpl
(
  uint8                      uid,
  uint16                     dpl_pkt_len_v4,
  uint16                     dpl_pkt_len_v6,
  uint32                     dpm_um_handle,
  ps_iface_type            * iface_ptr
)
{
#ifndef TEST_FRAMEWORK 
  uint16                dpl_pkt_len;
  ipa_err_code_e        ipa_err_code;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if ( dpl_pkt_len_v6 < dpl_pkt_len_v4)
  {
    dpl_pkt_len = dpl_pkt_len_v4;
  }
  else
  {
    dpl_pkt_len = dpl_pkt_len_v6;
  }    

  if (0 != dpl_pkt_len)
  {
    ipa_err_code =
      ipa_wan_ds_config_dpl(uid, 
                            iface_ptr->dpl_net_cb.recv_dpl_id.ifname, 
                            dpl_pkt_len, 
                            dpm_um_handle);

    if (IPA_SUCCESS != ipa_err_code)
    {
      LOG_MSG_ERROR_INT_2("ps_dpm_hw_config_dpl(): "
                      "Couldn't config DPL on uid %d with IPA, err %d",
                      uid, ipa_err_code);
      return -1;
    }
  }
  else
  {
    LOG_MSG_INFO2_1("ps_dpmi_config_dpl_with_hw(): "
                    "Not configuring DPL with IPA on uid %d, as DPL is not "
                    "enabled", uid);
  }
#endif /* TEST_FRAMEWORK */
  return 0;
} /* ps_dpm_hw_config_dpl() */

void ps_dpm_hw_dereg_dpl
(
  uint8                      uid  
)
{
#ifndef TEST_FRAMEWORK
  ipa_err_code_e        ipa_err_code;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  ipa_err_code =
    ipa_wan_ds_config_dpl(uid, 0xDE, 0, 0);
    
  if (IPA_SUCCESS != ipa_err_code)
  {
    LOG_MSG_ERROR_INT_2("ps_dpmi_dereg_dpl_with_ipa(): "
                    "Couldn't dereg DPL on uid %d with IPA, err %d",
                    uid, ipa_err_code);                    
  }
#endif /* TEST_FRAMEWORK */  
} /* ps_dpm_hw_dereg_dpl() */

int ps_dpm_hw_suspend_bearer
(
  uint8   uid
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  a2_dl_per_suspend_eps_bearer(uid, 0);

  LOG_MSG_INFO1_1("ps_dpm_hw_suspend_bearer(): Suspend bearer with uid %d",
                  uid);
                  
  ps_dpmi_hw_wait_for_ev_cback();
  
  return 0;
} /* ps_dpm_hw_suspend_bearer() */

void ps_dpm_hw_reg_event_cb
(
  void
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  a2_dl_per_register_suspend_cb(ps_dpmi_hw_suspend_bearer_cb);  
  
} /* ps_dpm_hw_reg_event_cb() */

void ps_dpm_hw_activate_bearer
(
  uint8   uid
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

} /* ps_dpm_hw_activate_bearer */


void ps_dpm_hw_dpl_powerup_init
(
  void
)
{
  ipa_err_code_e     ipa_err_code;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-----------------------------------------------------------------------
     Register callback with hardware. Hardware would indicate if dpl is
     available on hardware or not through the callback registered
  -----------------------------------------------------------------------*/
  ipa_err_code = ipa_acc_dpl_reg_cb (ps_dpm_hw_dpl_cb);
  if (IPA_SUCCESS != ipa_err_code)
  {
    LOG_MSG_ERROR_INT_1("ps_dpm_hw_dpl_powerup_init(): "
                    "Couldn't reg cb dpl with IPA, err %d", ipa_err_code);
    DS_UTILS_ASSERT(0);                    
  }
} /* ps_dpm_hw_dpl_powerup_init */

void ps_dpm_dpl_log_sio_info
(
  ps_iface_addr_family_type    addr_family,
  ps_dpm_rm_info_type        * dpm_rm_info_ptr,
  uint16                       num_bytes
)
{
  uint8                     iface_index = 0;
  ps_iface_type           * rm_iface_ptr       = NULL;
  ps_iface_type           * um_iface_ptr       = NULL;
  dpl_iface_config        * dpl_iface_info_ptr = NULL;
  ps_dpm_um_info_type     * dpm_um_info_ptr;
  dpl_sio_config            dpl_sio_info;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-----------------------------------------------------------------------
    1. Check the family. 
    2. Log V4 info if family is V4 
    3. Log V6 info if family is V6
    4. Log V4 and V6 info if family is ANY
    5. Log RM iface and corresponding UM iface info.
    6. In case of DUAL IP (2 Rm and 2 Um info will be logged)
  -----------------------------------------------------------------------*/
  memset(&dpl_sio_info, 0x0, sizeof(dpl_sio_info));

  dpl_sio_info.mux_id   = dpm_rm_info_ptr->mux_id;
  /*-----------------------------------------------------------------------
     Log V6 info if family is V4 or ANY
  -----------------------------------------------------------------------*/
  if (IPV4_ADDR == addr_family || IP_ANY_ADDR == addr_family )
  {
    rm_iface_ptr           =   dpm_rm_info_ptr->v4_iface_ptr;

    if (PS_IFACE_IS_VALID(rm_iface_ptr))
    {
        
      dpl_iface_info_ptr  =  &dpl_sio_info.ifaces_list[iface_index++];
      dpm_um_info_ptr     =  dpm_rm_info_ptr->v4_um_info_ptr;
      
      DPL_UPDATE_IFACE_INFO( dpl_iface_info_ptr, rm_iface_ptr);
      dpl_iface_info_ptr->iface_type = DPL_V4_RM_IFACE_TYPE;
      dpl_iface_info_ptr->ep_id      = 0;

      if (NULL != dpm_um_info_ptr && PS_IFACE_IS_VALID(dpm_um_info_ptr->v4_iface_ptr))
      {
        um_iface_ptr        =  dpm_um_info_ptr->v4_iface_ptr;
        dpl_iface_info_ptr  =  &dpl_sio_info.ifaces_list[iface_index++];
        
        DPL_UPDATE_IFACE_INFO( dpl_iface_info_ptr, um_iface_ptr);
        dpl_iface_info_ptr->iface_type = DPL_V4_UM_IFACE_TYPE;
        dpl_iface_info_ptr->ep_id      = 5;
      }
    }
  }
  /*-----------------------------------------------------------------------
     Log V6 info if family is V6 or ANY
  -----------------------------------------------------------------------*/
  if(IPV6_ADDR == addr_family  || IP_ANY_ADDR == addr_family )
  {
    rm_iface_ptr           =  dpm_rm_info_ptr->v6_iface_ptr;
    
    if (PS_IFACE_IS_VALID(rm_iface_ptr))
    {
      dpl_iface_info_ptr  =  &dpl_sio_info.ifaces_list[iface_index++];
      dpm_um_info_ptr     =  dpm_rm_info_ptr->v6_um_info_ptr;
      
      DPL_UPDATE_IFACE_INFO( dpl_iface_info_ptr, rm_iface_ptr);
      dpl_iface_info_ptr->iface_type = DPL_V6_RM_IFACE_TYPE;
      dpl_iface_info_ptr->ep_id      = 0;

      if (NULL != dpm_um_info_ptr && PS_IFACE_IS_VALID(dpm_um_info_ptr->v6_iface_ptr))
      {
         um_iface_ptr        =  dpm_um_info_ptr->v6_iface_ptr;
         dpl_iface_info_ptr  =  &dpl_sio_info.ifaces_list[iface_index++];
         
         DPL_UPDATE_IFACE_INFO( dpl_iface_info_ptr, um_iface_ptr);
         dpl_iface_info_ptr->iface_type = DPL_V6_UM_IFACE_TYPE;
          dpl_iface_info_ptr->ep_id      = 5;
      }
    }
  }
  if(PS_IFACE_IS_VALID(um_iface_ptr))
  {
    dpl_fill_apn_info(um_iface_ptr, dpl_sio_info.apn_desc, DPL_APN_DESC_S_LEN);
  }  
  dpl_sio_info.num_valid_ifaces = iface_index;
  ps_dpm_dpl_log_sio_desc_and_status (&dpl_sio_info);

}/*ps_dpm_dpl_log_sio_info*/

void ps_dpm_config_dpl_sio
(
  ps_iface_addr_family_type    addr_family,
  ps_dpm_rm_info_type        * dpm_rm_info_ptr,
  uint16                       num_bytes
)
{
  ipa_err_code_e            ipa_err_code;
  sio_stream_id_type        sio_stream_id;
  uint8                     ifname;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (NULL == dpm_rm_info_ptr)
  {
    return;
  }
  /*---------------------------------------------------------------------
    Configure sio stream id with hardware if DPL is being handled by hardware
  ---------------------------------------------------------------------*/ 
  sio_stream_id  = dpm_rm_info_ptr->sio_stream_id;

  if (NULL != dpm_rm_info_ptr->v4_iface_ptr)
  {
     ifname = dpm_rm_info_ptr->v4_iface_ptr->dpl_net_cb.recv_dpl_id.ifname;
  }  
  if (NULL != dpm_rm_info_ptr->v6_iface_ptr)
  {
     ifname = dpm_rm_info_ptr->v6_iface_ptr->dpl_net_cb.recv_dpl_id.ifname;
  } 
  ipa_err_code = ipa_sio_config_dpl (sio_stream_id, ifname, num_bytes);
  if (IPA_SUCCESS != ipa_err_code)
  {
    LOG_MSG_ERROR_INT_1("ps_dpm_config_dpl_sio(): "
                    "Couldn't reg dpl sio stream id with IPA, err %d", 
                    ipa_err_code);
  }
  /*---------------------------------------------------------------------
    Log sio MUX ID and Iface DPL specific information 
  ---------------------------------------------------------------------*/ 
  ps_dpm_dpl_log_sio_info( addr_family, dpm_rm_info_ptr, num_bytes);    
} /* ps_dpm_config_dpl_sio */

void ps_dpm_hw_control_dpl_logging
(
  uint32         pkt_len
)
{
  ipa_err_code_e        ipa_err_code;
  ipa_acc_dpl_state_e   ipa_dpl_state = IPA_ACC_DPL_STATE_DISABLE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
   LOG_MSG_INFO1_1(" ps_dpm_hw_control_dpl_logging pkt_len %d", pkt_len);

  /*-----------------------------------------------------------------------
    Change in qxdm log code indicates whether to enable or disable dpl
  -----------------------------------------------------------------------*/
  /*-----------------------------------------------------------------------
    1. Check if ipa already registered
    2. Check if its registered then check if log codes suggest only
       partial length if so enable hardware logging else disable hardware
       logging
  -----------------------------------------------------------------------*/   
  if (TRUE == ps_dpm_hw_dpl_available)
  {
    if (DPL_PACKET_PARTIAL_LENGTH == pkt_len)
    {
      ipa_dpl_state = IPA_ACC_DPL_STATE_ENABLE;
      ps_dpm_hw_dpl_enabled = TRUE;
    }
    else
    {
      ipa_dpl_state = IPA_ACC_DPL_STATE_DISABLE;
      ps_dpm_hw_dpl_enabled = FALSE;    
    }
  }

  ipa_err_code = ipa_acc_dpl_configure (ipa_dpl_state);
  if (IPA_SUCCESS != ipa_err_code)
  {
    LOG_MSG_ERROR_INT_2("ps_dpm_hw_control_dpl_logging(): "
                    "Couldn't reg/dereg dpl with IPA, err %d state %d", 
                     ipa_err_code, ipa_dpl_state);
    DS_UTILS_ASSERT(0);                    
  }

  /*-----------------------------------------------------------------------
    Go through all dpm rm info and dpm um info bearers and enable or disable
    logging
  -----------------------------------------------------------------------*/
  ps_dpm_control_dpl_hw_logging(pkt_len);

} /* ps_dpm_hw_control_dpl_logging */

/*===========================================================================
FUNCTION   PS_DPM_HW_REG_DPL_WMK

DESCRIPTION
  This function is used to register the DPM DPL WM with IPA

DEPENDENCIES
  None.

PARAMETERS
  ps_dpm_dpl_wmk_ptr - Pointer to ps_dpm_dpl_wmk

RETURN VALUE
  None
===========================================================================*/
void ps_dpm_hw_reg_dpl_wmk
(
  dsm_watermark_type *dpl_wmk_ptr
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  ipa_wan_ds_register_dpl_wm (dpl_wmk_ptr);
} /* ps_dpm_hw_reg_dpl_wmk() */

/*===========================================================================
FUNCTION   PS_DPM_HW_DPL_WMK_SIG_HANDLER()

DESCRIPTION
  Signal handler for PS_DPM_DPL_SIGNAL

DEPENDENCIES
  None.

PARAMETERS
  sig: the Signal that is handled
  user_data_ptr: NOT USED

RETURN VALUE
  TRUE: no more processing to be done
  FALSE: Needs to be called again.

SIDE EFFECTS
  None.
===========================================================================*/
boolean ps_dpm_hw_dpl_sig_handler
(
  ps_sig_enum_type    sig,
  void              * user_data_ptr
)
{
   dsm_item_type  *item_ptr        = NULL;
   ps_dpm_um_info_type   * dpm_um_info_ptr = NULL;
   ps_iface_type  *iface_ptr       = NULL;
   uint8                   version;   
   dsm_watermark_type *dpl_wmk_ptr = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

   dpl_wmk_ptr = ps_dpmi_dpl_get_wmk();
   item_ptr = dsm_dequeue(dpl_wmk_ptr);
   if ( item_ptr == NULL )
   {
     return TRUE;
   }

   iface_ptr = (ps_iface_type *) a2_log_get_dpl_pkt_info(item_ptr);
   if ( !PS_IFACE_IS_VALID( iface_ptr) )
   {
     LOG_MSG_INFO1_0 ("ps_dpm_hw_dpl_sig_handler(): Invalid iface in dsm item" );
     dsm_free_packet( &item_ptr);
     return FALSE;
   }

   PS_DPM_GLOBAL_STATS_INC(iface_ptr);

   DPL_LOG_NETWORK_RX_PACKET(iface_ptr, item_ptr, DPL_IID_NETPROT_IP);

   dsm_free_packet( &item_ptr);

   return FALSE;
} /* ps_dpm_hw_dpl_sig_handler() */

boolean ps_dpm_hw_pkt_info_is_filter_result_set
(
  dsm_item_type           * dsm_item_ptr
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  return FALSE;
} /* ps_dpm_hw_pkt_info_is_filter_result_set */

uint16 ps_dpm_hw_pkt_info_get_filter_result
(
  dsm_item_type           * dsm_item_ptr
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  return PS_IFACE_IPFLTR_RESULT_UNKNOWN;
} /* ps_dpm_hw_pkt_info_get_filter_result */

uint8 ps_dpm_hw_pkt_info_get_version
(
  dsm_item_type           * dsm_item_ptr
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  return 0;
} /* ps_dpm_hw_pkt_info_get_version */

boolean ps_dpm_hw_pkt_is_version_ipv4
(
  dsm_item_type           * dsm_item_ptr
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  return FALSE;
} /* ps_dpm_hw_pkt_is_version_ipv4 */

boolean ps_dpm_hw_pkt_is_version_ipv6
(
  dsm_item_type           * dsm_item_ptr
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  return FALSE;
} /* ps_dpm_hw_pkt_is_version_ipv6 */

void ps_dpm_hw_offload_checksum
(
  void
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  return;
} /* ps_dpm_hw_offload_checksum() */

#ifdef FEATURE_DATA_PS_464XLAT
int ps_dpm_hw_clat_reg_global_context
(
  ps_dpm_clat_global_cntxt_info_type   * clat_global_cntxt_info
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  return -1;
} /* ps_dpm_hw_clat_reg_global_context */

int ps_dpm_hw_clat_reg_pdn_context
(
  ps_dpm_clat_reg_pdn_cntxt_info_type       * clat_reg_info_ptr,
  ps_dpm_clat_hw_handle_type                * clat_hw_handle,
  ps_dpm_error_enum_type                    * dpm_err_no
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  return -1;
} /* ps_dpm_hw_clat_reg_pdn_context */

int ps_dpm_hw_clat_assoc_sio_stream
(
  ps_dpm_clat_hw_handle_type           * clat_hw_handle,
  sio_stream_id_type                     sio_stream_id
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  return -1;
} /* ps_dpm_hw_clat_assoc_sio_stream */

int ps_dpm_hw_clat_rereg_pdn_context
(
  ps_dpm_clat_reg_pdn_cntxt_info_type       * clat_reg_info_ptr,
  ps_dpm_clat_hw_handle_type                * clat_hw_handle
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  return -1;
} /* ps_dpm_hw_clat_rereg_pdn_context */

void ps_dpm_hw_clat_dereg_pdn_context
(
  ps_dpm_clat_hw_handle_type         * clat_hw_handle
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  return;
} /* ps_dpm_hw_clat_dereg_pdn_context */
#endif /* FEATURE_DATA_PS_464XLAT */
/**
  @brief  Registers CB with HW.
             call back would be invoked in two cases
             1)when continuous invalid packet count
             reaches threshold.
             2)first valid packet after invalid count threshold.
   
  @param[in] error_pkt_threshold:   threshold count
  @param[in] err_ind_cb_fn:         call back to be invoked by HW
   
  @return  TRUE : Pkt version is v6 
  
  @code
  @endcode
*/
void ps_dpm_hw_register_bearer_err_ind
(
  uint32                           err_pkt_threshold,
  void*                            err_ind_cb_fn_ptr
)
{
  //currently not supported in A2.
  return;
}
	
/**
  @brief  Deregisters CB with HW.
   
  @return  NA
  
  @code
  @endcode
*/
void ps_dpm_hw_deregister_bearer_err_ind
(
  void
)
{
  //currently not supported in A2.
  return;
}

void ps_dpm_hw_register_ul_monitor_cb
(
  void
)
{
  //currently not supported in A2.
  return;
}
