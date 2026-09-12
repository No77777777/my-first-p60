/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                             P S _ I F A C E I _ E V E N T . C


GENERAL DESCRIPTION
  This is the implementation of the PS interface event functions. This
  file implement functions used for both ps_iface and phys_link events.

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2002-2022 QUALCOMM Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                            EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/netiface/src/ps_ifacei_event.c#5 $
  $Author: pwbldsvc $ $DateTime: 2023/05/28 01:35:47 $

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when        who    what, where, why
--------    ---    ---------------------------------------------------------- 
08/20/15    gk     Added support for low latency traffic status indication
05/29/15    pg     Fixed LLVM warnings.
12/20/14    pg     Added TRAT/SRAT info in Handoff events.
06/30/14    ss     New event added: IFACE_EXTENDED_IP_CONFIG_EX_EV
04/09/14    ash    Added Prefix Delegation support.
06/21/13    ash    Support MTU upto 2000
05/03/13    svj    Fix for missing addr fmly TLV in get_last_data_call_status msg
02/04/13    svj    New DIAG event to generate data cal status information   
04/23/12    am     Reset shareable flag when iface goes down.
04/09/12    pgm    Moving Iface to Routeable in states other than
                   down/coming_down state for all iface except SIO.
02/01/12    am     Reset the DL opt config info when iface goes down.
11/30/11    su     Adding log code support for DPL logging
09/15/11    mct    Don't generate IFACE_OUT_OF_USE on registration since
                   if iface is down it will do that through DOWN_EV reg.
05/09/11    asn    Adding support for new active Iface event
12/08/10    rp     RAM optimization changes.
11/18/10    sy     Removed ps_ifacei_event_cback()
08/05/09    jee    Added a notification for DIAG upon IFACE UP event
06/30/10    ea     Clear the cache when the iface is down.
06/19/10    vs     Increased PS_IFACE_EVT_BUF_NUM due to QMI Dual IP support
09/21/09   mga     Merged from eHRPD branch
09/09/09    ss     Critical section released in error cases.
08/29/08    am     Increased PS_IFACE_EVT_BUF_NUM and high WM for low-tiers.
08/19/08    dm     Added support to post EXTENDED_IP_CONFIG_EV when DHCP
                   handle is not valid at the time of iface bring up
04/01/08    ssh    Adjusted iface event bufs to allow for MIPv6
04/03/08    am     BCMCS 2.0 Support.
06/26/07    rs     Added code to stop the Querier V1 present timer if the
                   interface is getting disabled.
05/07/07    mct    IPv6 Neighbor Discovery
04/17/07    es     Added support for EXTENDED_IP_CONFIG_EV.
04/09/07    vp     Additions for mip6_bootstrap_info
03/27/07    msr    Added support for RF_CONDITIONS_CHANGED_EV
02/08/07    scb    Fixed Klocwork High errors
01/23/07    msr    EMPA enhancements
11/29/06    msr    Fixed bug in ps_iface_change_pri_phys_link()
11/06/06    hm     Using a single ps_iface_free_domain_name_list function
                   instead of two different versions for SIP and DNS.
11/04/06    hm     Freeing domain name search on iface down indication.
11/02/06    mct    Added support for RFC3041: IPv6 Privacy Extensions.
11/01/06    rt     Free SIP server and domain name fn's used during IFACE
                   DOWN indication to free the lists.
11/01/06    rt     Merged EIDLE_SCI feature changes.
09/12/06    msr    Removed redundant state field in event_info structure
09/05/06    ks     Include network_params_info_type struct in ps_iface
                   instead of only auth_info to hold network config params.
08/14/06    sv     Merged slotted/Mode attribute ioctl.
08/15/06    rt     Resetting the SIP server list during IFACE down indication
04/25/06    msr    L4/Tasklock code review changes
02/22/06    msr    Using single critical section
02/13/06    msr    Fixed compilation errors
02/06/06    msr    Updated for L4 tasklock/crit sections.
10/18/05    msr    Removed support for FLOW_ACTIVATE_MODIFIED_EV and
                   PHYS_LINK_UP_MODIFIED_EV and changed
                   ps_iface_ipfltr_updated_ind() to conform to ipfltr_info
10/12/05    mct    Added support for new iface flow added/deleted events.
09/13/05    sv     Added support for new HDR rev0 rate inertia and HDR HPT mode
                   ioctls.
09/09/05    jd     On iface down indication, clear mode independent
                   authentication parameters
09/01/05    rt     Added TECH_CHANGED_EV in ps_iface_generic_ind
08/29/05    mct    Fixed missing taskfrees.
08/16/05    ks     Clear ps_iface stats from QMI on iface_down
08/16/05    msr    Fixed PS_BRANCH_TASKFREE()
08/15/05    mct    Fixed naming causing some issues w/C++ compilers.
08/15/05    mct    Added support for QOS_AWARE and QOS_UNAWARE events.
08/03/05    msr    Fixed a bug in ps_iface_change_pri_phys_link().
07/25/05    rt     Added function ps_iface_down_ind_ex()
06/20/05    mct    Added support for new multicast events.
05/13/05    ks     fixed lint errors
05/13/05    ks     fixed bug introduced in lint changes.
05/12/05    mct    Lint changes.
05/03/05    msr    Not enforcing any state checks
04/17/05    msr    Changed the behaviour of ps_iface_change_pri_phys_link().
                   Enforcing that iface is GOING_DOWN when phys link or flow
                   is GOING_NULL.
04/16/05    ks     Added validation of events before invoking them to avoid
                   duplicate events.
04/16/05    ks     Added PHYS_LINK_NULL state for PHYS_LINK_GONE_EV.
01/10/05    sv     Merged IPSEC changes.
01/08/05    vp     Flushing of net info (both v4/6) during ps_iface_down_ind()
11/19/04    msr    Using internal ps_iface macros instead of external ones.
11/19/04    msr    Added F3 messages to iface indications.
11/16/04    ks     Added check to ensure only valid phys_link events can be
                   registered with ps_iface_event_cback_reg(). Added
                   registering of phys link coming up, going down and gone
                   events.
11/08/04    mct    Changed TASKFREE to PS_BRANCH_TASKFREE to fix compilation
                   errors.
11/03/04    msr    Fixed missing brace and TASKFREE().
10/25/04    msr    Changed ps_iface_pri_phys_link_change_ind_ex() to
                   ps_iface_change_pri_phys_link() and changed the
                   implementation. Freeing buffers if event registration
                   fails in ps_iface_event_init().
10/29/04    mct    Fixed QOS merge issue where iface was being flow controlld
                   when multiple phys links were present.
10/12/04    mct    Call cbacks associated with the MT event. Added new
                   generic indication, ps_iface_generic_ind for new events.
10/05/04    ks     Set the bridge iface ptr to NULL in ps_iface_down_ind().
08/12/04    sv     Added support to distinguish IFACE_FLOW_ENABLED event
                   from PHYS_LINK_FLOW_ENABLED event.
08/09/04    mct    Fixed ps_iface_down_ind for logical ifaces to not call
                   ref_cnt_dec if phys_link has already been disassociated.
08/06/04    sv     Invoke iface flow enabled event when primary phys link
                   is flow enabled.
08/02/04    mct    Changed macros to internal ps_iface macros and included
                   ps_ifacei_utils.h.
07/30/04    ifk    Added ps_iface_valid_ra_ind()
07/30/04    vp     Changes due to consolidation of MTU and net_info structure
                   in ps_iface.
07/16/04    ak     On down_ind(), clear ps iface ref cnt.
06/11/04    vp     Changes for representation of IP addresses as struct
                   ps_in_addr or struct ps_in6_addr.
03/24/04    ak     Handle DOWN and DISABLED case in dorm_action().
03/22/04    ak     Changed dorm_action_func_ptr to ...f_ptr.
03/10/04    ak     Inc ref_cnt on routeable indiciation.
02/18/04    usb    Client id support for ps iface IP filtering.
02/04/04    ak     Updated for phys link and iface separation.  Various
                   fucntions have now moved to ps_phys_linki_event.c.  Also
                   added in events for transient states.
11/03/03    aku    Added Null check in ps_iface_alloc_event_cback_buf() after
                   call to ps_mem_get_buf().
10/21/03    usb    Added required included files.
10/14/03    mct    Added callbacks for the coming_up, going_down, and
                   phys_link equivalent events.
02/03/04    aku    Added support for logical ifaces
01/30/04    usb    Reset in_use, shreable and looked_up flags in iface down.
12/26/03    usb    Handling for new ps_iface state CONFIGURING.
11/03/03    aku    Added Null check in ps_iface_alloc_event_cback_buf() after
                   call to ps_mem_get_buf().
10/21/03    usb    Added required included files.
10/14/03    mct    Added callbacks for the coming_up, going_down, and
                   phys_link equivalent events.
08/25/03    aku    ps_mem_free() takes pointer-to-pointer as arg.
08/15/03    aku    created file.
===========================================================================*/
/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

                                INCLUDE FILES

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
#include "datamodem_variation.h"
#include "customer.h"
#include "comdef.h"

#include "msg.h"
#include "queue.h"
#include "ps_ifacei_event.h"
#include "ps_ifacei_utils.h"
#include "ps_iface.h"
#include "ps_ifacei.h"
#include "ds_flow_control.h"
#include "ps_phys_link.h"
#include "ps_phys_linki_event.h"
#include "ps_mem.h"
#include "ps_utils.h"
#include "ps_flowi.h"
#include "ps_flowi_event.h"
#include "ps_iface_dns_cache.h"
#include "ps_ifacei_addr_mgmt.h"
#include "ps_data_call_info_logging.h"
#include "event_defs.h"
#include "ps_sys.h"
#include "ps_sys_conf.h"
#include <stringl/stringl.h>
#include "ps_logging_helper.h"

#ifdef FEATURE_DATA_PS_IPV6
#include "ps_iface_addr_v6.h"
#include "ps_ifacei_addr_v6.h"
#include "ps_ip6_addr.h"
#include "dhcp6_client.h"
#endif /* FEATURE_DATA_PS_IPV6 */

#include "dhcp_client.h"

#ifdef FEATURE_DATA_EMBMS
#include "ps_iface_embms.h"
#endif /* FEATURE_DATA_EMBMS */

#include "ds_Utils_DebugMsg.h"

#include "ps_logging_diag.h"
#include "ps_system_heap.h"

#include "ps_dpm.h"
#include "ps_phys_link_event_int.h"
#include "ds_sys.h"

#ifdef FEATURE_DATA_PS_464XLAT
#include "ps_dpm_clat.h"
#endif /* FEATURE_DATA_PS_464XLAT */
/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

                           LOCAL DATA DECLARATIONS

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/

#define IFACE_1ST_EV  IFACE_UP_EV

typedef struct
{
  ps_iface_event_cback_type event_cback_f_ptr;
  void                    * user_data_ptr;
} ps_ifacei_event_cb_info_type;

/*---------------------------------------------------------------------------
  GLOBAL_IFACE_EVENT_HANDLE_Q - this is used to store callbacks that are
    intended to be called when events happen onANY interface.
  GLOBAL_IFACE_EVENT_Q_INITIALIZED - has the global queue been initialized?

  NOTE: these are separate (not in a struct) to insure that the
        initialization variables are set correctly.
---------------------------------------------------------------------------*/
static q_type  global_iface_event_handle_q;

/*---------------------------------------------------------------------------
  Event buffer used by ps_iface to register for global phys link events
---------------------------------------------------------------------------*/

static void *ps_iface_phys_link_ev_buf;

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

                             INTERNAL FUNCTIONS

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
/*===========================================================================
FUNCTION PS_IFACEI_GENERATE_DATA_BEARER_TECH()

DESCRIPTION
  This function will generate data bearer tech indications

PARAMETERS
  this_iface_ptr: ptr to the interface on which we are operating

RETURN VALUE
  None

DEPENDENCIES
  Must be called in a iface critical section; This event should be posted
  only on up ifaces.

SIDE EFFECTS
  None
===========================================================================*/
void ps_ifacei_generate_data_bearer_tech_ind
(
  ps_iface_type             *this_iface_ptr
)
{
  sint15                                     ps_errno = 0;
#ifdef FEATURE_DATA_WLAN_MAPCON
  ds_sys_system_status_info_type             data_bearer_ex;
#else
  ps_iface_ioctl_bearer_tech_changed_type    data_bearer;
#endif/* FEATURE_DATA_WLAN_MAPCON */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#ifdef FEATURE_DATA_WLAN_MAPCON
  if (ps_iface_ioctl(this_iface_ptr,
                     PS_IFACE_IOCTL_GET_BEARER_TECHNOLOGY_EX,
                     &data_bearer_ex,
                     &ps_errno) == 0)
  {
    /* Post bearer tech ind  */
    ps_iface_generic_ind( this_iface_ptr,
                          IFACE_BEARER_TECH_EX_CHANGED_EV,
                          &data_bearer_ex ); 
  }
  else
  {
    LOG_MSG_ERROR_1(" Cannot query data bearer tech on iface 0x%x", this_iface_ptr);
  }
#else
  if (ps_iface_ioctl(this_iface_ptr,
                     PS_IFACE_IOCTL_GET_BEARER_TECHNOLOGY_EX,
                     &data_bearer,
                     &ps_errno) == 0)
  {
   
    /* Post bearer tech ind  */
    ps_iface_generic_ind( this_iface_ptr,
                          IFACE_BEARER_TECH_CHANGED_EV,
                          &data_bearer) ); 
  }
  else
  {
    LOG_MSG_ERROR_1(" Cannot query data bearer tech on iface 0x%x", this_iface_ptr);
  }
#endif /* FEATURE_DATA_WLAN_MAPCON */
}/* ps_ifacei_generate_data_bearer_tech_ind */

/*===========================================================================

FUNCTION PS_IFACEI_IS_EVENT_SET()

DESCRIPTION
  This function checks if the bit corresponding to the event is already set in
  the event handle. 
 
PARAMETERS
  event_handle  : event handle
  event         : event to check for
 
RETURN VALUE
  TRUE if bit set
  FALSE otherwise

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean ps_ifacei_is_event_set
( 
  ps_ifacei_event_handle_type * event_handle, 
  ps_iface_event_enum_type      event 
)  
{
  uint64 bit = 1;
  uint64 result = 0;
  if (event >= IFACE_1ST_EV && event < IFACE_EVENT_MAX )
  {
    result = (event < (IFACE_1ST_EV + 64 )                                  
      ? (event_handle)->event_mask[0] & (uint64)(bit << (event - IFACE_1ST_EV))  
      : (event_handle)->event_mask[1] & (uint64)(bit << (event - IFACE_1ST_EV - 64))) ;
  }
  if (result)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
} /* ps_ifacei_is_event_set */

/*===========================================================================
FUNCTION PS_IFACEI_SET_EVENT_BIT()

DESCRIPTION
  This function sets the bit corresponding to the event in the event handle. 
 
PARAMETERS
  event_handle  : event handle
  event         : event to set
 
RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void ps_ifacei_set_event_bit
( 
  ps_ifacei_event_handle_type * event_handle, 
  ps_iface_event_enum_type      event 
)                 
{
  uint64 bit = 1;
  if (event < IFACE_1ST_EV + 64)  
  {
    event_handle->event_mask[0] |= (uint64)(bit << (event - IFACE_1ST_EV));
  }
  else
  {
    event_handle->event_mask[1] |= (uint64)(bit << (event - IFACE_1ST_EV - 64));
  }
} /* ps_ifacei_set_event_bit */

/*===========================================================================
FUNCTION PS_IFACEI_UNSET_EVENT_BIT()

DESCRIPTION
  This function unsets the bit corresponding to the event in the event handle. 
 
PARAMETERS
  event_handle  : event handle
  event         : event to set
 
RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void ps_ifacei_unset_event_bit
( 
  ps_ifacei_event_handle_type * event_handle, 
  ps_iface_event_enum_type      event 
)                 
{
  uint64 bit = 1;
  if (event < IFACE_1ST_EV + 64)  
  {
    event_handle->event_mask[0] &=  ~((uint64)(bit << (event - IFACE_1ST_EV)));
  }
  else
  {
    event_handle->event_mask[1] &=  ~((uint64)(bit << (event - IFACE_1ST_EV - 64)));
  }
} /* ps_ifacei_unset_event_bit */

/*===========================================================================
FUNCTION PS_IFACEI_COMPARE_EVENT_HANDLE()

DESCRIPTION
  This function compares passed in cb fn and user data ptr against values in
  the event handle queue. 
 
PARAMETERS
  item_ptr      : event handle in queue
  compare_val   : struct containing cb fn and user data ptr
 
RETURN VALUE
  TRUE if values match
  FALSE otherwise

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
static int ps_ifacei_compare_event_handle
( 
  void *item_ptr,
  void *compare_val 
)
{
  ps_ifacei_event_handle_type  *evt_handle = (ps_ifacei_event_handle_type*)item_ptr;
  ps_ifacei_event_cb_info_type *cb_info = (ps_ifacei_event_cb_info_type*)compare_val;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if ((evt_handle->event_cback_f_ptr == cb_info->event_cback_f_ptr) &&
      (evt_handle->user_data_ptr == cb_info->user_data_ptr))
  {
    return TRUE;
  }
  return FALSE;
} /* ps_ifacei_compare_event_handle */

/*===========================================================================
FUNCTION PS_IFACEI_EVENT_RESET_IFACE()

DESCRIPTION
  it resets variables of iface.
 
PARAMETERS
  this_iface_ptr: ptr to interface control block on which to operate on.
 
RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void ps_ifacei_event_reset_iface
(
  ps_iface_type                  * this_iface_ptr,
  ps_iface_event_enum_type         event,
  ps_iface_down_event_info_type  * down_event_info_ptr
)
{
  struct ps_mcast_cb          * cur_grp_ptr;
  ps_iface_event_info_u_type    event_info;
  int32                         timer_ret_val;
  int16                         ps_errno; 
  uint32                        call_type;
  ps_iface_type               * comp_iface_ptr = NULL;
  ps_iface_down_mh_event_info_type down_event_info;
#ifdef FEATURE_DATA_PS_464XLAT
  ps_dpm_clat_dereg_pdn_cntxt_info_type dereg_pdn_context = { 0 };
  ps_dpm_clat_pdn_cntxt_info_type * clat_pdn_cntxt = NULL;
#endif /* FEATURE_DATA_PS_464XLAT */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Update data_call_status_info only on EPC_IFACE. If proc_id is none,
    active_out_of_use issued event_reset_iface previously.
    When ps_iface_down_ind calls event_reset_iface again, this time we don't
    need to update data_call_status_info as the interface is already down.
  -------------------------------------------------------------------------*/
  if ((this_iface_ptr->name == EPC_IFACE) && 
     (down_event_info_ptr->proc_id != PS_IFACE_PROC_ID_NONE))
  {
    /*-----------------------------------------------------------------------
      If ref_cnt is greater than one, network tore down all the calls.
      PS_IFACE_PROC_ID_ALL is used as call_type to show all calls are down
    -----------------------------------------------------------------------*/
    if (PS_IFACE_GET_REF_CNT(this_iface_ptr) > 1)
    {
      call_type = PS_DATA_CALL_INFO_LOGGING_ALL_CALLS;
    }
    else 
    {
      /*---------------------------------------------------------------------
        Check if this is a bridged iface. If so this is RMNET call and update 
        the call_type from down_event_info , else if it is NULL update call
        type as modem embedded call.
      ---------------------------------------------------------------------*/ 
      if (this_iface_ptr->iface_private.tx_bridge_ptr != NULL)
      {
        call_type = down_event_info_ptr->proc_id;
      }
      else
      {
        call_type = PS_DATA_CALL_INFO_LOGGING_MODEM_EMBEDDED;
      }
    }

    /*-----------------------------------------------------------------------
      Generate DIAG events with call status information
    -----------------------------------------------------------------------*/
    ps_data_call_info_logging_event_report(this_iface_ptr,
                                 PS_DATA_CALL_INFO_LOGGING_STATUS_TERMINATED,
                                 call_type);
  }  

  /* profile may be attached */
  if((event != IFACE_ACTIVE_OUT_OF_USE_EV) || (this_iface_ptr->name != UMTS_IFACE))
  {
    PS_IFACEI_RESET_3GPP_PROFILE_ID(this_iface_ptr);
  }
  PS_IFACEI_RESET_3GPP2_PROFILE_ID(this_iface_ptr);
  PS_IFACEI_RESET_RM_INST_HANDLE(this_iface_ptr);
  PS_IFACE_SET_APN_TYPE(this_iface_ptr, PS_IFACE_APN_TYPE_UNSPECIFIED);
  PS_IFACE_RESET_PROPERTY( this_iface_ptr, PS_IFACE_SUPPORTED_FLAG_KEEP_ALIVE );
  
  /*-------------------------------------------------------------------------
    The ref cnt is forced to zero.
  -------------------------------------------------------------------------*/
  PS_IFACE_RESET_REF_CNT(this_iface_ptr);

  /*-------------------------------------------------------------------------
    If IGMP V1 querier present timer is running, it needs to be stopped.
  -------------------------------------------------------------------------*/
  if (this_iface_ptr->iface_private.querier_v1_timer != 0)
  {
    if (ps_timer_is_running(this_iface_ptr->iface_private.querier_v1_timer))
    {
      timer_ret_val =
        ps_timer_cancel(this_iface_ptr->iface_private.querier_v1_timer);
      if (timer_ret_val != PS_TIMER_SUCCESS)
      {
        LOG_MSG_ERROR_1("ps_ifacei_event_reset_iface(): "
                        "Error cancelling IGMP v1 querier present timer 0x%x",
                        this_iface_ptr->iface_private.querier_v1_timer);
        return;
      }
    }
  }

  /*-------------------------------------------------------------------------
    Cancel all the running timers for each of the multicast groups
    associated with this interface.
  -------------------------------------------------------------------------*/
  cur_grp_ptr = this_iface_ptr->iface_private.if_mcast_grps;
  while (cur_grp_ptr != NULL)
  {
    if (ps_timer_is_running(cur_grp_ptr->mcast_report_timer))
    {
      timer_ret_val = ps_timer_cancel(cur_grp_ptr->mcast_report_timer);
      if (timer_ret_val != PS_TIMER_SUCCESS)
      {
        LOG_MSG_ERROR_1("ps_ifacei_event_reset_iface(): "
                        "Error cancelling MCAST report timer 0x%x",
                        cur_grp_ptr->mcast_report_timer);
        PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
        return;
      }
    }

    cur_grp_ptr = cur_grp_ptr->next_mcast_grp;
  } /* end of while */

  /*-------------------------------------------------------------------------
    Reset post proc related state
  -------------------------------------------------------------------------*/
  PS_IFACEI_RESET_IN_USE(this_iface_ptr);
  PS_IFACEI_RESET_LOOKED_UP(this_iface_ptr);
  PS_IFACEI_RESET_PROC_ID(this_iface_ptr);

  /*-------------------------------------------------------------------------
    Reset associations to other ifaces
  -------------------------------------------------------------------------*/
  PS_IFACE_SET_ASSOC_IFACE(this_iface_ptr, NULL);
  PS_IFACE_SET_TRAT_IFACE(this_iface_ptr, NULL);
  this_iface_ptr->iface_private.tx_bridge_ptr = NULL;

  /*-------------------------------------------------------------------------
    Reset flow control mask. Otherwise, iface will forever be flow
    controlled forever.
  -------------------------------------------------------------------------*/
  ps_iface_enable_flow(this_iface_ptr,
                       this_iface_ptr->iface_private.tx_flow_mask);
  
  /*------------------------------------------------------------------------- 
    Dereg CLAT PDN CONTXT if it's not freed as part of bearer deregistration.
    This will happen in case of DDS switch. 
   -------------------------------------------------------------------------*/
#ifdef FEATURE_DATA_PS_464XLAT
  if(NULL != this_iface_ptr->clat_pdn_cntxt)
  {
    dereg_pdn_context.iface_ptr = this_iface_ptr;
    clat_pdn_cntxt = 
          (ps_dpm_clat_pdn_cntxt_info_type *)this_iface_ptr->clat_pdn_cntxt;
    dereg_pdn_context.client_pdn_handle = clat_pdn_cntxt->client_pdn_handle;
	
    (void) ps_dpm_clat_dereg_pdn_context( &dereg_pdn_context );
  }
#endif /* FEATURE_DATA_PS_464XLAT */
  /*-------------------------------------------------------------------------
    Reset statistics
  -------------------------------------------------------------------------*/
  memset(&this_iface_ptr->iface_i_stats,
          0,
          sizeof(this_iface_ptr->iface_i_stats));

  /*-------------------------------------------------------------------------
    Clear pkt stats adjustment cache
  -------------------------------------------------------------------------*/
  memset(&(this_iface_ptr->stat_adjustment_cache),
          0,
          sizeof(ps_iface_stat_adjustment_type) );

  /*-------------------------------------------------------------------------
    Clear network config information in iface
  -------------------------------------------------------------------------*/
  memset(&(this_iface_ptr->net_cfg_params),
          0,
          sizeof(network_params_info_type) );         
  /*-------------------------------------------------------------------------
    Clear bearer tech info in event info cache
  -------------------------------------------------------------------------*/
  memset(&(this_iface_ptr->event_info_cache.bearer_tech_changed_info),
          0,
          sizeof(this_iface_ptr->event_info_cache.bearer_tech_changed_info) );

#ifdef FEATURE_DATA_WLAN_MAPCON
  memset(&(this_iface_ptr->event_info_cache.bearer_tech_ex_changed_info),
          0,
          sizeof(ds_sys_system_status_info_type) );
#endif /* FEATURE_DATA_WLAN_MAPCON */

  memset(&(this_iface_ptr->event_info_cache.changed_apn_param_info),
         0,
         sizeof(this_iface_ptr->event_info_cache.changed_apn_param_info));

  memset(this_iface_ptr->event_info_cache.dpd_ping_stat_info,
         0,
         sizeof(ps_iface_dpd_ping_stat_info_type));

  /*-------------------------------------------------------------------------
    Clear DNS cache
  -------------------------------------------------------------------------*/
  (void) ps_iface_dns_cache_flush(this_iface_ptr, &ps_errno);

  /*-------------------------------------------------------------------------
    Clear the optimized Rm path and filters for this iface
  -------------------------------------------------------------------------*/
  PS_IFACEI_SET_OPT_RM_IFACE(this_iface_ptr, NULL);
  PS_SYSTEM_HEAP_MEM_FREE(PS_IFACEI_GET_OPT_FILTER_BASE(this_iface_ptr));
  PS_IFACEI_SET_OPT_FILTER_BASE(this_iface_ptr, NULL);

  /*-------------------------------------------------------------------------
   Clear the power save filtering mode and auto exit mode for this iface
  -------------------------------------------------------------------------*/
  PS_IFACEI_RESET_POWERSAVE_FILTERING_MODE(this_iface_ptr);
  PS_IFACEI_RESET_POWERSAVE_FILTERING_AUTO_EXIT_MODE(this_iface_ptr);
  
  /*-------------------------------------------------------------------------
    Reset is_shareable flag 
  -------------------------------------------------------------------------*/
  PS_IFACE_SET_IS_SHAREABLE(this_iface_ptr, FALSE);

  /*-------------------------------------------------------------------------
    Free the memory used for modem reserved pco information 
  -------------------------------------------------------------------------*/
  if (this_iface_ptr->modem_reserved_pco_ptr != NULL)
  {
    if (this_iface_ptr->modem_reserved_pco_ptr->pco_info_ptr != NULL)
    {
      PS_SYSTEM_HEAP_MEM_FREE(
        this_iface_ptr->modem_reserved_pco_ptr->pco_info_ptr);
    }
    PS_SYSTEM_HEAP_MEM_FREE(this_iface_ptr->modem_reserved_pco_ptr);
  }

  if (IFACE_ACTIVE_OUT_OF_USE_EV != event)
  {
    /*-------------------------------------------------------------------------
     Free the memory used for operator reserved pco information and msisdn info
     only if Iface is down.
     As in LTE attach PDN case IFACE_ACTIVE_OUT_OF_USE_EV is intermideate state, 
     don't clear.
    -------------------------------------------------------------------------*/
    if ( (event == IFACE_DOWN_EV) &&
         (this_iface_ptr->operator_reserved_pco_list_ptr != NULL) )
    {
      PS_SYSTEM_HEAP_MEM_FREE(this_iface_ptr->operator_reserved_pco_list_ptr);
    }

    if ( (event == IFACE_DOWN_EV) &&
         (this_iface_ptr->msisdn_ptr != NULL) )
    {
      PS_SYSTEM_HEAP_MEM_FREE(this_iface_ptr->msisdn_ptr);
    }

    /*-----------------------------------------------------------------------
      Reset all of the IP configuration
    -----------------------------------------------------------------------*/
    memset(&(this_iface_ptr->net_info), 0, sizeof(network_info_type));
    PS_IFACE_SET_MTU(this_iface_ptr, DEFAULT_DATA_UNIT);

    ps_iface_free_sip_addr_list(&this_iface_ptr->sip_serv_addr_list);
    ps_iface_free_domain_name_list
    (
      &this_iface_ptr->sip_serv_domain_name_list
    );

    ps_iface_free_domain_name_list
    (
      &this_iface_ptr->domain_name_search_list
    );

    if (ps_iface_addr_family_is_v4(this_iface_ptr))
    {
      this_iface_ptr->iface_private.ipv4_addr = 0;
    }
#ifdef FEATURE_DATA_PS_IPV6
    else if (ps_iface_addr_family_is_v6(this_iface_ptr))
    {
      (void) ps_iface_delete_all_v6_prefixes(this_iface_ptr);
    }
#endif /* FEATURE_DATA_PS_IPV6 */

    memset(&(this_iface_ptr->iface_private.pdn_teardown_config),
           0,
           sizeof(this_iface_ptr->iface_private.pdn_teardown_config));
  
    (void) ps_iface_ipfltr_delete_all(this_iface_ptr, 
                                      IP_FLTR_CLIENT_QOS_OUTPUT, 
                                      &ps_errno); 
                                      
    (void) ps_iface_ipfltr_delete_all(this_iface_ptr, 
                                      IP_FLTR_CLIENT_SOCKETS, 
                                      &ps_errno);  

    /*-----------------------------------------------------------------------
      Change the current state to DOWN
    -----------------------------------------------------------------------*/
    this_iface_ptr->iface_private.state = IFACE_DOWN;
  }

  /*-------------------------------------------------------------------------
    Invoke event callbacks
  -------------------------------------------------------------------------*/
  event_info.iface_down_info.proc_id = down_event_info_ptr->proc_id;
  event_info.iface_down_info.state   = down_event_info_ptr->state;
  event_info.iface_down_info.netdown_reason =
    down_event_info_ptr->netdown_reason;
  event_info.iface_down_info.addr_family = down_event_info_ptr->addr_family;
  memscpy(&(event_info.iface_down_info.bearer_tech), 
          sizeof(ds_sys_system_status_info_type),
          &(down_event_info_ptr->bearer_tech),
          sizeof(ds_sys_system_status_info_type));  

  /*-------------------------------------------------------------------------
    Set the data path state to default HW
  -------------------------------------------------------------------------*/
   PS_IFACE_SET_DATA_PATH_FLAG(this_iface_ptr, PS_IFACE_BRIDGING_ALLOWED);

  /*---------------------------------------------------------------------------
    This to make sure that data_path_bridge_mask is reset in below scenario.

    Scenario(CR 980505):
      1. v4v6 attah PDN is setup.
      2. v4 Rmnet call is made from UI.
      3. Call is kept idle and it went into powersave mode. Corresponding
         bridge_mask is set for bot hv4 and v6 um_ifaces.
      4. Now, tethered v4 call is made and arbitration happened.
      5. Since there is no call made and torn down on v6 iface it will retain 
         the bridge_mask set in step 3.
      6. From now on, all the subsequent calls will be incorrectly unbridged
         until another call is made on v6 and both v4 and v6 calls are torn
         down.
  ---------------------------------------------------------------------------*/
  if( PS_IFACE_IS_PHYSICAL_IFACE(this_iface_ptr) )
  {
    comp_iface_ptr = ps_dpm_get_companion_iface( this_iface_ptr);

    if( PS_IFACE_IS_VALID( comp_iface_ptr) )
    {
      /*-----------------------------------------------------------------------
        This is to make sure that we won't reset any iWLAN iface bridge_mask 
        which is up with no EPC iface.
      -----------------------------------------------------------------------*/
      if( ( NULL == comp_iface_ptr->iface_private.back_iface_ptr ) &&
          ( 0 == PS_IFACE_GET_REF_CNT(comp_iface_ptr)) )
      {
        /*-------------------------------------------------------------------------
          Set the data path state to default HW
         ------------------------------------------------------------------------*/  
        PS_IFACE_SET_DATA_PATH_FLAG(comp_iface_ptr, PS_IFACE_BRIDGING_ALLOWED);
        LOG_MSG_INFO1_2("ps_ifacei_event_reset_iface: comp iface bridge_mask "
                        "is reset for iface 0x%x:%d"
                        ,comp_iface_ptr->name
                        ,comp_iface_ptr->instance );
      }
    }
	
    /*---------------------------------------------------------------------
    Reset logical iface in case of race condition where physical iface
    is brought down even before logical iface registers for events
    on physical iface
    ---------------------------------------------------------------------*/							 
    if (this_iface_ptr->iface_private.back_iface_ptr)
    {
      if ((IFACE_DOWN == 
        PS_IFACEI_GET_STATE(this_iface_ptr->iface_private.back_iface_ptr)) &&
        (this_iface_ptr == 
        PS_IFACE_GET_ASSOC_IFACE(this_iface_ptr->iface_private.back_iface_ptr))
        )
      {
        memset(&down_event_info, 0, sizeof(down_event_info));
        down_event_info.bearer_tech.rat_value =  DS_SYS_RAT_UNKNOWN;
        ps_iface_down_ind(this_iface_ptr->iface_private.back_iface_ptr, 
                  &down_event_info);		 

      }
    }	
  }
  
  /*-------------------------------------------------------------------------
    Reset the DPL dropped count for the iface. Dropped count should be 
    maintained per call
  -------------------------------------------------------------------------*/
  DPL_RESET_IFACE_LOG_DROP_COUNT(this_iface_ptr);
  
  PS_IFACE_RESET_CAPABILITY( this_iface_ptr, 
                             PS_IFACE_CAPABILITY_CLAT_CAPABLE ); 	  
  
  LOG_MSG_INFO2_4("ps_ifacei_event_reset_iface(): "
                  "Invoke ev callbacks, iface 0x%x:%d, ev %d, down reason %d",
                  this_iface_ptr->name,
                  this_iface_ptr->instance,
                  event,
                  event_info.iface_down_info.netdown_reason);

  ps_ifacei_invoke_event_cbacks(this_iface_ptr,
                                NULL,
                                event,
                                event_info);															

  /*-------------------------------------------------------------------------
    NOTE!!!! BELOW OPERATIONS MUST BE PERFORMED AFTER IFACE_DOWN_EV IS POSTED
  -------------------------------------------------------------------------*/

  PS_IFACEI_INC_COOKIE(this_iface_ptr);

} /* ps_ifacei_event_reset_iface() */ 


/*===========================================================================
FUNCTION PS_IFACEI_PHYS_LINK_EVENT_CBACK()

DESCRIPTION
  This event callback will be registered by ps_iface as a global event
  callback with phys link. This function will invoke all the global phys link
  event callbacks registed via ps_iface and also the iface specific phys link
  event callbacks.

  Uses ps_ifacei_inoke_event_cback().  Each call to that function results in
  two sets of callbacks:

      callbacks registered with that particular iface
      callbacks registered on the global iface event queue.

  Note that if two iface's point to the same phys_link.  In that case, the
  callbacks registered with the global iface event queue will be called
  twice.  However, each call will be called with a different IFACE ptr.  Up
  to client to distinguish correctly.

PARAMETERS
  this_phys_link_ptr: ptr to the phys link on which we are operating
  this_iface_ptr: ptr to the interface on which we are operating
  event: the event for which callbacks need to be called
  event_info: event information that needs to be passed into callback.
  user_data_ptr: Pointer to user data info

RETURN VALUE
  None

DEPENDENCIES
  Must be called in a TASKLOCK

SIDE EFFECTS
  None
===========================================================================*/
static void ps_ifacei_phys_link_event_cback
(
  ps_phys_link_type           * this_phys_link_ptr,
  ps_iface_event_enum_type      event,
  ps_iface_event_info_u_type    event_info,
  void                        * user_data_ptr
)
{
  ps_iface_type  *iface_ptr = NULL;
  int             iface_index;
  boolean         invoke_cbacks = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Determine if the phys link on which this event occurred is the primary
    phys link of one the created ifaces.
  -------------------------------------------------------------------------*/
  for (iface_index = 0; iface_index < MAX_SYSTEM_IFACES; iface_index++)
  {
    iface_ptr = global_iface_ptr_array[iface_index];

    /*-----------------------------------------------------------------------
      Since ifaces are added sequentially and they are never deleted, as
      soon as we get a NULL ptr there is no more iface pointers in the list.
    -----------------------------------------------------------------------*/
    if (iface_ptr == NULL)
    {
      break;
    }

    /*-----------------------------------------------------------------------
      Call callbacks only if the event occured on primary phys link of the
      iface
    -----------------------------------------------------------------------*/
    if (PS_IFACEI_GET_PHYS_LINK(iface_ptr) == this_phys_link_ptr)
    {
     DS_INTERFACE_LOG_MSG_INFO1_3(PS_LOGGING_CAT_PS_IFACE, 
                                  "ps_ifacei_phys_link_event_cback(): "
                      "Phys link evt %d on iface=0x%x:%d",
                      event, iface_ptr->name, iface_ptr->instance);

      switch(event)
      {
        case PHYS_LINK_UP_EV:
          invoke_cbacks = TRUE;
          break;

        case PHYS_LINK_DOWN_EV:
          invoke_cbacks = TRUE;
          break;

        case PHYS_LINK_GONE_EV:
          invoke_cbacks = TRUE;
          break;

        case PHYS_LINK_COMING_UP_EV:
          invoke_cbacks = TRUE;
          break;

        case PHYS_LINK_GOING_DOWN_EV:
          invoke_cbacks = TRUE;
          break;

        default:
          break;
      }

      if (invoke_cbacks == TRUE)
      {
        /*-------------------------------------------------------------------
          Not always invoking callbacks.  Flow-control events will be invoked
          automatically by enable/disable_flow calls above.  Don't want to
          duplicate here.
        -------------------------------------------------------------------*/
        ps_ifacei_invoke_event_cbacks(iface_ptr,
                                      NULL,
                                      event,
                                      event_info);

      } /* invoke cbacks? */
    } /* iface has this phys link */
  } /* for all ifaces... */

} /* ps_ifacei_phys_link_event_cback() */

/*===========================================================================
FUNCTION PS_IFACEI_VALIDATE_EVENT_CBACKS()

DESCRIPTION
  This function will validate all of the event callbacks for a given
  interface or phys link. The validation is successful if the ps_iface or
  the phys_link is not already in the same state as the occuring event.

PARAMETERS
  this_iface_ptr: ptr to the interface on which we are operating
  this_phys_link_ptr: ptr to the phys_link on which we are operating
  event: the event for which callbacks need to be called
  event_info: event information that needs to be passed into callback.

RETURN VALUE
  TRUE: callback must be invoked
  FALSE: don't invoke callback

DEPENDENCIES
  Must be called in a iface critical section

SIDE EFFECTS
  None
===========================================================================*/
static boolean ps_ifacei_validate_event_cbacks
(
  const ps_iface_type             *this_iface_ptr,
  const ps_phys_link_type         *this_phys_link_ptr,
  ps_iface_event_enum_type   event,
  ps_iface_event_info_u_type event_info
)
{
  boolean invoke_cbacks = TRUE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Note - Not validating iface and phys_link ptrs as this function is
    called only from ps_ifacei_invoke_event_cbacks() which validates them
    just before calling this
  -------------------------------------------------------------------------*/

  if (NULL != this_iface_ptr)
  {
    switch (event)
    {
      case IFACE_UP_EV :
        if (event_info.iface_up_info.state == IFACE_UP)
        {
          invoke_cbacks = FALSE;
        }
        break;

      case IFACE_DOWN_EV :
        if (event_info.iface_down_info.state == IFACE_DOWN)
        {
          invoke_cbacks = FALSE;
        }
        break;

      case IFACE_ROUTEABLE_EV :
        if (event_info.state == IFACE_ROUTEABLE)
        {
          invoke_cbacks = FALSE;
        }
        break;

      case IFACE_CONFIGURING_EV :
        if (event_info.state == IFACE_CONFIGURING)
        {
          invoke_cbacks = FALSE;
        }
        break;

      case IFACE_COMING_UP_EV :
        if (event_info.state == IFACE_COMING_UP)
        {
          invoke_cbacks = FALSE;
        }
        break;

      case IFACE_GOING_DOWN_EV :
        if (event_info.state == IFACE_GOING_DOWN)
        {
          invoke_cbacks = FALSE;
        }
        break;

      case IFACE_PHYS_LINK_COMING_UP_EV :
        if (event_info.phys_link_event_info.state == PHYS_LINK_COMING_UP)
        {
          invoke_cbacks = FALSE;
        }
        break;

      case IFACE_PHYS_LINK_GOING_DOWN_EV :
        if (event_info.phys_link_event_info.state == PHYS_LINK_GOING_DOWN)
        {
          invoke_cbacks = FALSE;
        }
        break;

      case IFACE_PHYS_LINK_DOWN_EV :
        if (event_info.phys_link_event_info.state == PHYS_LINK_DOWN)
        {
          invoke_cbacks = FALSE;
        }
        break;

      case IFACE_PHYS_LINK_GONE_EV :
        if (event_info.phys_link_event_info.state == PHYS_LINK_NULL)
        {
          invoke_cbacks = FALSE;
        }
        break;

      case IFACE_PHYS_LINK_UP_EV :
        if (event_info.phys_link_event_info.state == PHYS_LINK_UP)
        {
          invoke_cbacks = FALSE;
        }
        break;
#ifdef FEATURE_DATA_FLOW_MGMT
      case IFACE_PHYS_LINK_FLOW_CONTROL_EV:
        {
          invoke_cbacks = TRUE;
           break;
         }
#endif /* FEATURE_DATA_FLOW_MGMT */
      default:
        /* do nothing */
        break;
    } /* switch */

    return invoke_cbacks;
  }

  if (NULL != this_phys_link_ptr)
  {
    switch (event)
    {
      /*---------------------------------------------------------------------
        Don't check if duplicate PHYS_LINK_UP_EV is generated as UMTS calls
        ps_phys_link_up_ind() to indicate that the network modified QoS
        autonomously
      ---------------------------------------------------------------------*/
      case PHYS_LINK_UP_EV :
        break;

      case PHYS_LINK_COMING_UP_EV :
        if (event_info.phys_link_event_info.state == PHYS_LINK_COMING_UP)
        {
          invoke_cbacks = FALSE;
        }
        break;

      case PHYS_LINK_GOING_DOWN_EV :
        if (event_info.phys_link_event_info.state == PHYS_LINK_GOING_DOWN)
        {
          invoke_cbacks = FALSE;
        }
        break;

      case PHYS_LINK_DOWN_EV :
        if (event_info.phys_link_event_info.state == PHYS_LINK_DOWN)
        {
          invoke_cbacks = FALSE;
        }
        break;

      case PHYS_LINK_GONE_EV :
        if (event_info.phys_link_event_info.state == PHYS_LINK_NULL)
        {
          invoke_cbacks = FALSE;
        }
        break;

      case PHYS_LINK_GOING_NULL_EV :
        if (event_info.phys_link_event_info.state == PHYS_LINK_GOING_NULL)
        {
          invoke_cbacks = FALSE;
        }
        break;

      case PHYS_LINK_RESUMING_EV :
        if (event_info.phys_link_event_info.state == PHYS_LINK_RESUMING)
        {
          invoke_cbacks = FALSE;
        }
        break;
#ifdef FEATURE_DATA_FLOW_MGMT
     case IFACE_PHYS_LINK_FLOW_CONTROL_EV:
        {
          invoke_cbacks = TRUE;
          break;
        }
#endif /* FEATURE_DATA_FLOW_MGMT */
      default:
        /* do nothing */
        break;

    } /* switch */

    return invoke_cbacks;
  }

  return invoke_cbacks;

} /* ps_ifacei_validate_event_cbacks() */



/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

                             EXTERNAL FUNCTIONS

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
/*===========================================================================
FUNCTION PS_IFACE_EVENT_INIT()

DESCRIPTION
  This function initializes the global queues for both ps_iface and
  ps_phys_link.  It also registers the ps_iface phys_link function on the
  global phys_link queue.

  Note: Currently, only the events in ps_iface need initialization. Hence,
  the implementation of this function resides in ps_ifacei_event.c.

PARAMETERS
  None

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void ps_iface_event_init
(
  void
)
{
  ps_iface_error_enum_type   ps_error_info = PS_IFACE_SUCCESS;
  int                        retval = 0; 
  ps_timer_handle_type       timer_handle = PS_TIMER_INVALID_HANDLE;
  /*-------------------------------------------------------------------------
    Note - To subscribe/register for a new phys link event, just add it in 
    this array
  -------------------------------------------------------------------------*/
  ps_iface_event_enum_type   phys_link_events[] = 
  { 
    PHYS_LINK_UP_EV,
    PHYS_LINK_DOWN_EV,
    PHYS_LINK_GONE_EV,
    PHYS_LINK_COMING_UP_EV,
    PHYS_LINK_GOING_DOWN_EV
  };

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  do
  {
    timer_handle = ps_timer_alloc( ps_ifacei_low_latency_timer_expiry_cb, 
                                 NULL );

    if ( timer_handle == PS_TIMER_INVALID_HANDLE )
    {
      LOG_MSG_ERROR_INT_0("ps_iface_event_init(): Low latency timer allocation"
                      " failed");
      DS_UTILS_ASSERT(0);
      break;
    }
    else
    {
      ps_ifacei_set_low_latency_timer_handle( timer_handle );
    }

    /*-------------------------------------------------------------------------
      Initialize global event queue
    -------------------------------------------------------------------------*/
    (void) q_init(&global_iface_event_handle_q);

    /*-------------------------------------------------------------------------
      Subscribe the global ps_iface function for phys link events.
    -------------------------------------------------------------------------*/

    ps_iface_phys_link_ev_buf = 
      ps_phys_link_alloc_event_handle(NULL, ps_ifacei_phys_link_event_cback, NULL );
    if (NULL == ps_iface_phys_link_ev_buf)
    {
      ps_error_info = PS_IFACE_ERROR_HANDLE_ALLOC_FAILED;
      DS_UTILS_ASSERT(0);
      break;
    }
    
    retval = ps_phys_link_subscribe_event_list( ps_iface_phys_link_ev_buf,
    	                                        phys_link_events,
  	                                        ARRAY_SIZE(phys_link_events));
    if (0 != retval)
    {
      ps_error_info = PS_IFACE_ERROR_EVENT_REG_FAILED;
      ps_phys_link_free_event_handle(ps_iface_phys_link_ev_buf);
      DS_UTILS_ASSERT(0);
      break;
    }

    return;
  } while(0);

  LOG_MSG_ERROR_INT_1("ps_iface_event_init(): Error = %d", ps_error_info);

} /* ps_iface_event_init() */

/*===========================================================================
FUNCTION PS_IFACE_ALLOC_EVENT_HANDLE()

DESCRIPTION
  This function is used to allocate the memory used to register for ps_iface
  event callbacks.
  Once no longer needed, it should be freed using ps_iface_free_event_handle

PARAMETERS 
  iface_ptr: the iface on which events need to be registered 
  event_cback_f_ptr: the callback to be registered
  user_data_ptr: data to be called with the callback.

RETURN VALUE
  NULL: if allocation failed
  ptr to memory being allocated.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void *ps_iface_alloc_event_handle
(
  ps_iface_type            *iface_ptr,
  ps_iface_event_cback_type event_cback_f_ptr,
  void                     *user_data_ptr
)
{
  ps_ifacei_event_handle_type *handle_ptr = NULL;
  q_type                      *working_q_ptr = NULL;
  ps_ifacei_event_cb_info_type cb_info;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (event_cback_f_ptr == NULL)
  {
    return NULL;
  }
  if ((iface_ptr != NULL) && !(PS_IFACE_IS_VALID(iface_ptr)))
  {
    return NULL; //DS_ASSERT?? 
  }

  /*-------------------------------------------------------------------------
    Check that a handle with same cb fn ptr and user_data_ptr doesnt already
    exist.
  -------------------------------------------------------------------------*/
  cb_info.event_cback_f_ptr = event_cback_f_ptr;
  cb_info.user_data_ptr = user_data_ptr;

  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

  /*-------------------------------------------------------------------------
    If this registration is for global callbacks, then use the global
    variable as the working pointer, otherwise use the q from the iface
  -------------------------------------------------------------------------*/
  if (iface_ptr == NULL)
  {
    working_q_ptr = &global_iface_event_handle_q;
  }
  else 
  {
    working_q_ptr = 
     &(iface_ptr->iface_private.event_handle_q);
  }
  do
  {
    if ( NULL != q_linear_search( working_q_ptr,
                                ps_ifacei_compare_event_handle,
                                &cb_info ) )
    {
      LOG_MSG_ERROR_2("ps_iface_alloc_event_handle: already present cb_fn_ptr 0x%p, user_data_ptr 0x%p",
                      event_cback_f_ptr, user_data_ptr);
      break;
    }

    /*-------------------------------------------------------------------------
      Allocate a new event callback buffer.
    -------------------------------------------------------------------------*/
    PS_SYSTEM_HEAP_MEM_ALLOC(handle_ptr, sizeof(ps_ifacei_event_handle_type),
                             ps_ifacei_event_handle_type *);

    if (handle_ptr == NULL)
    {
      break;
    }

    /*-------------------------------------------------------------------------
      Initialize the event buffer
    -------------------------------------------------------------------------*/
    memset(handle_ptr,0,sizeof(ps_ifacei_event_handle_type));
    (void) q_link(handle_ptr, &(handle_ptr->link));
    handle_ptr->event_cback_f_ptr = event_cback_f_ptr;
    handle_ptr->user_data_ptr     = user_data_ptr;
    handle_ptr->iface_ptr         = iface_ptr;

    /* Enqueue the empty buffer */
    q_put(working_q_ptr, &(handle_ptr->link));
  } while (0);

  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

  return handle_ptr;

} /* ps_iface_alloc_event_handle() */


/*===========================================================================
FUNCTION PS_IFACE_FREE_EVENT_HANDLE()

DESCRIPTION
  This function is used to free the memory used to register for ps_iface
  event callbacks. 

PARAMETERS
  event_handle: the event handle to be freed

RETURN VALUE
  None.

DEPENDENCIES
  ps_iface_alloc_event_handle should have been called 

SIDE EFFECTS
  None
===========================================================================*/
void ps_iface_free_event_handle
(
  void *evt_handle
)
{
  ps_ifacei_event_handle_type* event_handle = NULL;
  q_type                     * working_q_ptr = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (evt_handle == NULL)
  {
    DS_UTILS_ASSERT(0);
    return;
  }

  event_handle = (ps_ifacei_event_handle_type*)evt_handle;
  /*-------------------------------------------------------------------------
    Validate the buffer that is passed in is on a queue
  -------------------------------------------------------------------------*/
  if (!Q_ALREADY_QUEUED(&(event_handle->link)))
  {
    DS_UTILS_ASSERT(0);
    return;
  }
  /*-------------------------------------------------------------------------
    Choose appropriate queue based on whether callback is global or per iface
  -------------------------------------------------------------------------*/
  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);
  if (event_handle->iface_ptr == NULL)
  {
    working_q_ptr = &global_iface_event_handle_q;
  }
  else 
  {
    working_q_ptr = 
     &(event_handle->iface_ptr->iface_private.event_handle_q);
  }

  /* Delete from the queue */
  q_delete(working_q_ptr, &(event_handle->link));

  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

  /*-------------------------------------------------------------------------
    Invalidate the buffer so that the caller cannot use it without
    explicitly allcating it again.
  -------------------------------------------------------------------------*/
  memset(event_handle, 0, sizeof(ps_ifacei_event_handle_type));

  PS_SYSTEM_HEAP_MEM_FREE(event_handle);

} /* ps_iface_free_event_handle() */


/*===========================================================================
FUNCTION PS_IFACE_SUBSCRIBE_EVENT_LIST()

DESCRIPTION
  Subscribe to a list of events on an interface. Whenever an event in the list
  occurs, the callback for that event is called.  If an event in the list is
  true when registration happens the callback is called immediately.  This will
  fail if the event handle (with callback and user data pointer) is NULL.
  The same event handle can be used to subscribe to an additional set of events.
  If an event in the list had previously been subscribed to, it will be a NO-OP.

PARAMETERS
  event_handle: Event handle that holds the callback pointer. Should not be NULL.
  events:  Array of events that triggers the callbacks
  num_events : Number of elements in the array

RETURN VALUE
  0: on success
 -1: on failure

DEPENDENCIES
  ps_iface_alloc_event_handle should have been called

SIDE EFFECTS
  None
===========================================================================*/
int ps_iface_subscribe_event_list
(
  void                     *evt_handle,
  ps_iface_event_enum_type *events,
  uint8                     num_events 

)
{
  ps_ifacei_event_handle_type * event_handle = NULL, *first_event_handle = NULL;
  ps_iface_event_info_u_type    event_info;
  ps_iface_event_enum_type      event;
  boolean                       need_to_invoke_cback = FALSE;
  void                        * dhcp_client_handle = NULL;
  int                           ref_cnt = 0;
  boolean                       is_up = FALSE;
  boolean                       is_active = FALSE;
  boolean                       is_flow_event_reg = FALSE;
  uint8                         itr;
  q_type                      * working_q_ptr = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (evt_handle == NULL || events == NULL || num_events == 0)
  {
    DS_UTILS_ASSERT(0);
    return -1;
  }

  event_handle = (ps_ifacei_event_handle_type*)evt_handle;
  /*-------------------------------------------------------------------------
    Validate the buffer that is passed in is on a queue
  -------------------------------------------------------------------------*/
  if (!Q_ALREADY_QUEUED(&(event_handle->link)))
  {
    DS_UTILS_ASSERT(0);
    return -1;
  }

  /* Validate all events in the list */
  for (itr=0; itr<num_events; itr++)
  {
    if (events[itr] < IFACE_1ST_EV || events[itr] >= IFACE_EVENT_MAX)
    {

      LOG_MSG_ERROR_INT_1("ps_iface_subscribe_event_list(): "
                      "Invalid Iface event %d to register for ps_iface",
                      events[itr]);
      return -1;
    }
  }

  /*-------------------------------------------------------------------------
    As the contents of the buffer are supposed to be set by ps_iface
    functions it should NEVER have a NULL callback
  -------------------------------------------------------------------------*/
  if (event_handle->event_cback_f_ptr == NULL)
  {
    DS_UTILS_ASSERT(0);
    return -1;
  }

  /*-------------------------------------------------------------------------
    if not the global queue then make sure that the interface is valid
  -------------------------------------------------------------------------*/
  if (event_handle->iface_ptr != NULL &&
      !PS_IFACE_IS_VALID(event_handle->iface_ptr))
  {
    LOG_MSG_ERROR_1("ps_iface_subscribe_event_list(): "
                    "Invalid iface in event handle, 0x%p", event_handle->iface_ptr);
    return -1;
  }

  /*-------------------------------------------------------------------------
    check if the event is true - if so call the callback immediately
  -------------------------------------------------------------------------*/
  ref_cnt   = PS_IFACE_GET_REF_CNT( event_handle->iface_ptr );
  is_up     = PS_IFACE_IS_UP( event_handle->iface_ptr );
  is_active = PS_IFACE_GET_IS_ACTIVE( event_handle->iface_ptr );

  LOG_MSG_QTRACE_HFT_MED(PS_TAG_NETIFACE, PS_SYS_DEFAULT_SUBS,
                 "ps_iface_subscribe_event_list(): Number of events registered %d",
                 num_events);

  for (itr=0; itr<num_events; itr++)
  {
    if (events[itr] == IFACE_ALL_EV)
    {
      /* Set all bits */
      event_handle->event_mask[0] = 0xFFFFFFFFFFFFFFFF;
      event_handle->event_mask[1] = 0xFFFFFFFFFFFFFFFF;
      is_flow_event_reg = TRUE;
    }
    else 
    {
      /* NO-OP if event is already set in bitmask */
      if (ps_ifacei_is_event_set(event_handle, events[itr]))
      {
        continue;
      }

      ps_ifacei_set_event_bit(event_handle, events[itr]);
    }
    if (events[itr] == IFACE_FLOW_ENABLED_EV || events[itr] == IFACE_FLOW_DISABLED_EV)
    {
      is_flow_event_reg = TRUE;
    }

    /* For global registation no need to invoke cback */
    if (event_handle->iface_ptr == NULL)
    {
      continue;
    }

    need_to_invoke_cback = FALSE;

    /*-------------------------------------------------------------------------
      Initializing "event_info" 
    -------------------------------------------------------------------------*/
    memset(&event_info, 0, sizeof(event_info));
    switch (events[itr])
    {
      /*-----------------------------------------------------------------------
                              IFACE_ACTIVE_OUT_OF_USE_EV
      -----------------------------------------------------------------------*/
      case IFACE_ACTIVE_OUT_OF_USE_EV:

        if( is_active && is_up && (ref_cnt == 0) )
        {
          LOG_MSG_INFO2_0( "reg for ACTIVE_OUT_OF_USE_EV, invoke CB");
          need_to_invoke_cback = TRUE;
        }
        else /* ref_cnt != 0 */
        {
          LOG_MSG_INFO2_0( "reg for ACTIVE_OUT_OF_USE_EV, CB not invoked");
        }
        break;
      /*-----------------------------------------------------------------------
                              IFACE_ENABLED_EV
      -----------------------------------------------------------------------*/
      case IFACE_ENABLED_EV:
        if (event_handle->iface_ptr->iface_private.state != IFACE_DISABLED)
        {
          event_info.state = event_handle->iface_ptr->iface_private.state;
          need_to_invoke_cback = TRUE;
        }
        break;

      /*-----------------------------------------------------------------------
                              IFACE_DISABLED_EV
      -----------------------------------------------------------------------*/
      case IFACE_DISABLED_EV:
        if (event_handle->iface_ptr->iface_private.state == IFACE_DISABLED)
        {
          event_info.state = IFACE_DISABLED;
          need_to_invoke_cback = TRUE;
        }
        break;

      /*-----------------------------------------------------------------------
                              IFACE_DOWN_EV
      -----------------------------------------------------------------------*/
      case IFACE_DOWN_EV:
        if (event_handle->iface_ptr->iface_private.state == IFACE_DOWN)
        {
          event_info.iface_down_info.state = IFACE_DOWN;
          event_info.iface_down_info.netdown_reason =
            event_handle->iface_ptr->event_info_cache.iface_down_info.netdown_reason;

          need_to_invoke_cback = TRUE;
        }
        break;

      /*-----------------------------------------------------------------------
                              IFACE_COMING_UP_EV
      -----------------------------------------------------------------------*/
      case IFACE_COMING_UP_EV:
        if (event_handle->iface_ptr->iface_private.state == IFACE_COMING_UP)
        {
          event_info.state = IFACE_COMING_UP;
          need_to_invoke_cback = TRUE;
        }
        break;

      /*-----------------------------------------------------------------------
                              IFACE_CONFIGURING_EV
      -----------------------------------------------------------------------*/
      case IFACE_CONFIGURING_EV:
        if (event_handle->iface_ptr->iface_private.state == IFACE_CONFIGURING)
        {
          event_info.state = IFACE_CONFIGURING;
          need_to_invoke_cback = TRUE;
        }
        break;

      /*-----------------------------------------------------------------------
                              IFACE_ROUTEABLE_EV
      -----------------------------------------------------------------------*/
      case IFACE_ROUTEABLE_EV:
        if (event_handle->iface_ptr->iface_private.state == IFACE_ROUTEABLE)
        {
          event_info.state = IFACE_ROUTEABLE;
          need_to_invoke_cback = TRUE;
        }
        break;

      /*-----------------------------------------------------------------------
                                 IFACE_UP_EV
      -----------------------------------------------------------------------*/
      case IFACE_UP_EV:
        if (event_handle->iface_ptr->iface_private.state == IFACE_UP)
        {
          event_info.iface_up_info.state = IFACE_UP;
          need_to_invoke_cback = TRUE;
        }
        break;

      /*-----------------------------------------------------------------------
                              IFACE_GOING_DOWN_EV
      -----------------------------------------------------------------------*/
      case IFACE_GOING_DOWN_EV:
        if (event_handle->iface_ptr->iface_private.state == IFACE_GOING_DOWN)
        {
          event_info.state = IFACE_GOING_DOWN;
          need_to_invoke_cback = TRUE;
        }
        break;

      /*-----------------------------------------------------------------------
                              IFACE_EXTENDED_IP_CONFIG_EV
      -----------------------------------------------------------------------*/
      case IFACE_EXTENDED_IP_CONFIG_EV:
        if (event_handle->iface_ptr->iface_private.state == IFACE_UP)
        {
          dhcp_client_handle = 
                       ps_iface_get_dhcp_stateless_client_handle(event_handle->iface_ptr);
          if (NULL == dhcp_client_handle)
          {
            event_info.extended_ip_config_succeeded = TRUE;
            need_to_invoke_cback = TRUE;
          }
          else if (TRUE == ps_iface_addr_family_is_v4(event_handle->iface_ptr))
          {
            if (!dhcp_client_is_dhcp_in_progress(dhcp_client_handle))
            {
              event_info.extended_ip_config_succeeded = TRUE;
              need_to_invoke_cback = TRUE;
            }
          }
  #ifdef FEATURE_DATA_PS_IPV6
          else if (TRUE == ps_iface_addr_family_is_v6(event_handle->iface_ptr))
          {
            if (!dhcp6_client_is_dhcp_in_progress(dhcp_client_handle))
            {
              event_info.extended_ip_config_succeeded = TRUE;
              need_to_invoke_cback = TRUE;
            }
          }
  #endif /* FEATURE_DATA_PS_IPV6 */
          else
          {
            LOG_MSG_ERROR_INT_0("ps_iface_event_cback_reg(): "
                            "iface addr family does not match dhcp clients");
          }
        }
        break;

      /*-----------------------------------------------------------------------
                            IFACE_FLOW_ENABLED_EV
      -----------------------------------------------------------------------*/
      case IFACE_FLOW_ENABLED_EV:
        if (event_handle->iface_ptr->iface_private.tx_flow_mask == 0)
        {
          event_info.flow_mask = 0;
          need_to_invoke_cback = TRUE;
        }
        break;

      /*-----------------------------------------------------------------------
                           IFACE_FLOW_DISABLED_EV
      -----------------------------------------------------------------------*/
      case IFACE_FLOW_DISABLED_EV:
        if (event_handle->iface_ptr->iface_private.tx_flow_mask != 0)
        {
          event_info.flow_mask = event_handle->iface_ptr->iface_private.tx_flow_mask;
          need_to_invoke_cback = TRUE;
        }
        break;

        /*-----------------------------------------------------------------------
                           IFACE_DELETE_EV and default

        It does not make sence to call the delete event here because the if
        interface has been deleted, then it is not in use and can't have
        callback called on/by it.
      -----------------------------------------------------------------------*/
      case IFACE_DELETE_EV:
        DS_UTILS_ASSERT(0);
        break;

      /*-----------------------------------------------------------------------
                                   IFACE_ALL_EV
       We send out Flow enabled/disabled event and Iface extended ip config 
       event everytime someone registers for iface all event. The reason being
       is in case of handoff if new trat is already flow disabled/enabled we
       would like logical iface to consume flow state as soon as handoff happens
      -----------------------------------------------------------------------*/      
      case IFACE_ALL_EV:
      {
        if (event_handle->iface_ptr->iface_private.tx_flow_mask == 0)
        {
          event_info.flow_mask = 0;
          event = IFACE_FLOW_ENABLED_EV;
        }
        else if (event_handle->iface_ptr->iface_private.tx_flow_mask != 0)
        {
          event_info.flow_mask = event_handle->iface_ptr->iface_private.tx_flow_mask;
          event = IFACE_FLOW_DISABLED_EV;
        }  
        
        event_handle->event_cback_f_ptr(event_handle->iface_ptr,
                                        event,
                                        event_info,
                                        event_handle->user_data_ptr);
        
        if (event_handle->iface_ptr->iface_private.state == IFACE_UP)
        {
          dhcp_client_handle = 
                    ps_iface_get_dhcp_stateless_client_handle(event_handle->iface_ptr);
          if (NULL == dhcp_client_handle)
          {
            event_info.extended_ip_config_succeeded = TRUE;      
            need_to_invoke_cback = TRUE;          
          }
          else if (TRUE == ps_iface_addr_family_is_v4(event_handle->iface_ptr))
          {
            if (!dhcp_client_is_dhcp_in_progress(dhcp_client_handle))
            {
              event_info.extended_ip_config_succeeded = TRUE;
              need_to_invoke_cback = TRUE;
            }
          }
  #ifdef FEATURE_DATA_PS_IPV6
          else if (TRUE == ps_iface_addr_family_is_v6(event_handle->iface_ptr))
          {
            if (!dhcp6_client_is_dhcp_in_progress(dhcp_client_handle))
            {
              event_info.extended_ip_config_succeeded = TRUE;   
              need_to_invoke_cback = TRUE;            
            }
          }
  #endif /* FEATURE_DATA_PS_IPV6 */
        }   
        if(need_to_invoke_cback == TRUE)
          event_handle->event_cback_f_ptr(event_handle->iface_ptr,
                                          IFACE_EXTENDED_IP_CONFIG_EV,
                                          event_info,
                                          event_handle->user_data_ptr);  
            
        need_to_invoke_cback = FALSE;  
        break;
      }

  #ifdef FEATURE_DATA_WLAN_MAPCON        
      case IFACE_HANDOFF_INIT_EV:
      case IFACE_HANDOFF_STATUS_SUCCESS_EV:
      case IFACE_HANDOFF_STATUS_FAILURE_EV:
        break;
  #endif /* FEATURE_DATA_WLAN_MAPCON */

      case IFACE_IPFLTR_UPDATED_EV:
      case IFACE_MT_REQUEST_EV:
      case IFACE_NET_INITIATED_QOS_AVAILABLE_EV:
      case IFACE_IPV6_EXTERNAL_ADDR_DELETED_EV:
  #ifdef FEATURE_DATA_WLAN_MAPCON      
      case IFACE_REVERSE_IP_TRANSPORT_CONFIG_EV:
      case IFACE_REVERSE_IP_TRANSPORT_IPFLTR_CHANGED_EV:
  #endif /* FEATURE_DATA_WLAN_MAPCON */    
      default:
        /* do nothing */;
        break;

    } /* switch(event) */

    /*-------------------------------------------------------------------------
      If the callback needs to be called, do so
    -------------------------------------------------------------------------*/
    if (need_to_invoke_cback)
    {
      event_handle->event_cback_f_ptr(event_handle->iface_ptr,
                                      events[itr],
                                      event_info,
                                      event_handle->user_data_ptr);
    }
  } /* for loop around event list*/

  /* Move the event_handle to the head of the queue if a flow event has been
     registered for. This will ensure that flow events get higher priority */
  if (is_flow_event_reg)
  {

    /*-------------------------------------------------------------------------
      Choose appropriate queue based on whether callback is global or per iface
    -------------------------------------------------------------------------*/
    PS_ENTER_CRIT_SECTION(&global_ps_crit_section);
    if (event_handle->iface_ptr == NULL)
    {
      working_q_ptr = &global_iface_event_handle_q;
    }
    else 
    {
      working_q_ptr = 
       &(event_handle->iface_ptr->iface_private.event_handle_q);
    }

    first_event_handle = q_check(working_q_ptr);
    if ((q_cnt(working_q_ptr) > 1) && (first_event_handle != event_handle))
    {
      /* Delete from the queue */
      q_delete(working_q_ptr, &(event_handle->link));
      /* Add to head of the queue */
      q_insert(working_q_ptr,  &(event_handle->link), &(first_event_handle->link));
    }

    PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);
  }

  return 0;

} /* ps_iface_subscribe_event_list() */



/*===========================================================================
FUNCTION PS_IFACE_UNSUBSCRIBE_EVENT_LIST()

DESCRIPTION
  Unsubscribe from a list of events on an interface. This will
  fail if the event handle (with callback and user data pointer) is NULL.
  If an event in the list had not previously been subscribed to, it will be a NO-OP.
  If an event handle is no longer needed, it should be freed using
  ps_iface_free_event_handle

PARAMETERS
  event_handle: Event handle that holds the callback pointer. Should not be NULL.
  events:  Array of events to unsubscribe from 
  num_events : Number of elements in the array

RETURN VALUE
  0: on success
 -1: on failure

DEPENDENCIES
  ps_iface_alloc_event_handle should have been called

SIDE EFFECTS
  None
===========================================================================*/
int ps_iface_unsubscribe_event_list
(
  void                     *evt_handle,
  ps_iface_event_enum_type *events,
  uint8                     num_events 
)
{
  q_type                      * working_q_ptr = NULL;
  ps_ifacei_event_handle_type * event_handle = NULL, * last_event_handle = NULL;
  boolean                    is_flow_event_unreg = FALSE;
  uint8                      itr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (evt_handle == NULL || events == NULL || num_events == 0)
  {
    DS_UTILS_ASSERT(0);
    return -1;
  }

  event_handle = (ps_ifacei_event_handle_type*)evt_handle;
  /*-------------------------------------------------------------------------
    Validate the buffer that is passed in is on a queue
  -------------------------------------------------------------------------*/
  if (!Q_ALREADY_QUEUED(&(event_handle->link)))
  {
    DS_UTILS_ASSERT(0);
    return -1;
  }

  /* Validate all events in the list */
  for (itr=0; itr<num_events; itr++)
  {
    if (events[itr] < IFACE_1ST_EV || events[itr] >= IFACE_EVENT_MAX)
    {

      LOG_MSG_ERROR_INT_1("ps_iface_unsubscribe_event_list(): "
                      "Invalid Iface event %d to de-register for ps_iface",
                      events[itr]);
      return -1;
    }
  }

  /*-------------------------------------------------------------------------
    As the contents of the buffer are supposed to be set by ps_iface
    functions it should NEVER have a NULL callback
  -------------------------------------------------------------------------*/
  if (event_handle->event_cback_f_ptr == NULL)
  {
    DS_UTILS_ASSERT(0);
    return -1;
  }

  /*-------------------------------------------------------------------------
    if not the global queue then make sure that the interface is valid
  -------------------------------------------------------------------------*/
  if (event_handle->iface_ptr != NULL &&
      !PS_IFACE_IS_VALID(event_handle->iface_ptr))
  {
    LOG_MSG_ERROR_1("ps_iface_subscribe_event_list(): "
                    "Invalid iface in event handle, 0x%p", event_handle->iface_ptr);
    return -1;
  }

  for (itr=0; itr<num_events; itr++)
  {
    LOG_MSG_INFO2_1("ps_iface_unsubscribe_event_list(): Dereg for event %d",
                    events[itr]);
    /* NO-OP if event is already unset in bitmask */
    if (!ps_ifacei_is_event_set(event_handle, events[itr]))
    {
      continue;
    }

    ps_ifacei_unset_event_bit(event_handle, events[itr]);
    /* Make sure both flow events are not registered for */
    if (((events[itr] == IFACE_FLOW_ENABLED_EV) &&
        (!ps_ifacei_is_event_set(event_handle, IFACE_FLOW_DISABLED_EV))) ||
        ((events[itr] == IFACE_FLOW_DISABLED_EV) &&
         (!ps_ifacei_is_event_set(event_handle, IFACE_FLOW_ENABLED_EV))))
    {
      is_flow_event_unreg = TRUE;
    }
    if (events[itr] == IFACE_ALL_EV)
    {
      /* Unset everything */
      event_handle->event_mask[0] = 0;
      event_handle->event_mask[1] = 0;
      /* Move it to end of the queue as well */
      is_flow_event_unreg = TRUE;
    }
  }
  /*-------------------------------------------------------------------------
    Move the event_handle to the tail of the queue if flow events have been
    de-registered from
  -------------------------------------------------------------------------*/
  if (is_flow_event_unreg)
  {
    PS_ENTER_CRIT_SECTION(&global_ps_crit_section);
    if (event_handle->iface_ptr == NULL)
    {
      working_q_ptr = &global_iface_event_handle_q;
    }
    else 
    {
      working_q_ptr = 
       &(event_handle->iface_ptr->iface_private.event_handle_q);
    }

    last_event_handle = q_last_check(working_q_ptr);
    if ((q_cnt(working_q_ptr) > 1) && (last_event_handle != event_handle))
    {
      /* Delete from the queue */
      q_delete(working_q_ptr, &(event_handle->link));
      /* Add to tail of the queue */
      q_insert_after(working_q_ptr, &(event_handle->link), &(last_event_handle->link));
    }

    PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

  }
  return 0;

} /* ps_iface_unsubscribe_event_list() */



/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
                                 INDICATIONS

  These are intended to only be called by the interface clients/owners. While
  there is no way to enforce this, having someone else call the indications
  is not supported behavior.
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
/*===========================================================================
FUNCTION PS_IFACE_ENABLE_IND()

DESCRIPTION
  The given interface has been enabled.  Call callbacks.

  THIS FUNCTION SHOULD NOT BE CALLED IN AN ISR.

PARAMETERS
  this_iface_ptr: ptr to interface control block on which to operate on.

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void ps_iface_enable_ind
(
  ps_iface_type *this_iface_ptr
)
{
  ps_iface_event_info_u_type event_info;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (!PS_IFACE_IS_VALID(this_iface_ptr))
  {
    DS_UTILS_ASSERT(0);
    return;
  }

  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

  LOG_MSG_QTRACE_HFT_MED(PS_TAG_NETIFACE, PS_SYS_DEFAULT_SUBS,
                 "ps_iface_enable_ind(): IFACE ENABLE IND 0x%x:%d, state %d",
                  this_iface_ptr->name,
                  this_iface_ptr->instance,
                  PS_IFACEI_GET_STATE(this_iface_ptr));

  event_info.state = this_iface_ptr->iface_private.state;
  if (event_info.state == IFACE_DISABLED)
  {
    this_iface_ptr->iface_private.state = IFACE_DOWN;

    /*-----------------------------------------------------------------------
      Invoke Call back only if the iface state is disabled, not otherwise.
    -----------------------------------------------------------------------*/
    ps_ifacei_invoke_event_cbacks(this_iface_ptr,
                                  NULL,
                                  IFACE_ENABLED_EV,
                                  event_info);
  }

  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

} /* ps_iface_enable_ind() */

/*===========================================================================
FUNCTION PS_IFACE_DISABLED_IND()

DESCRIPTION
  The given interface has been disabled, if the interface is in any state
  other than DOWN then force the state DOWN first.  Call callbacks.

  THIS FUNCTION SHOULD NOT BE CALLED IN AN ISR.

PARAMETERS
  this_iface_ptr: ptr to interface control block on which to operate on.

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void ps_iface_disabled_ind
(
  ps_iface_type *this_iface_ptr
)
{
  ps_iface_event_info_u_type event_info;
  ps_iface_down_mh_event_info_type   down_event_info;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (!PS_IFACE_IS_VALID(this_iface_ptr))
  {
    DS_UTILS_ASSERT(0);
    return;
  }

  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

  LOG_MSG_INFO2_3("ps_iface_disabled_ind(): "
                  "IFACE DISABLED IND 0x%x:%d, state %d",
                  this_iface_ptr->name,
                  this_iface_ptr->instance,
                  PS_IFACEI_GET_STATE(this_iface_ptr));

  /*-------------------------------------------------------------------------
    copy the current state information into the event_info varible, then
    process based on this state.
  -------------------------------------------------------------------------*/
  event_info.state = PS_IFACEI_GET_STATE(this_iface_ptr);
  switch(event_info.state)
  {
  /*-----------------------------------------------------------------------
      Don't invoke callback if iface is already in DISABLED state.
    -----------------------------------------------------------------------*/
  case IFACE_DISABLED:
    LOG_MSG_INFO2_3("ps_iface_disabled_ind(): "
                    "Interface 0x%x:%d already disabled, state %d",
                    this_iface_ptr->name,
                    this_iface_ptr->instance,
                    PS_IFACEI_GET_STATE(this_iface_ptr));
    break;

  case IFACE_CONFIGURING:
  case IFACE_ROUTEABLE:
  case IFACE_UP:
  case IFACE_COMING_UP:
  case IFACE_GOING_DOWN:
  case IFACE_LINGERING:
    /*-----------------------------------------------------------------------
      If we are in any state other than down we want to call the down
      indication - then change the state for the interface.
    -----------------------------------------------------------------------*/
    memset(&down_event_info, 0, sizeof(down_event_info));
    down_event_info.bearer_tech.rat_value =  DS_SYS_RAT_UNKNOWN;
    ps_iface_down_ind(this_iface_ptr, &down_event_info);    
    this_iface_ptr->iface_private.state = IFACE_DISABLED;

    ps_ifacei_invoke_event_cbacks(this_iface_ptr,
                                  NULL,
                                  IFACE_DISABLED_EV,
                                  event_info);
    break;

  case IFACE_DOWN:
    this_iface_ptr->iface_private.state = IFACE_DISABLED;

    ps_ifacei_invoke_event_cbacks(this_iface_ptr,
                                  NULL,
                                  IFACE_DISABLED_EV,
                                  event_info);
    break;

  default:
    DS_UTILS_ASSERT(0);
  } /* switch(state) */

  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

} /* ps_iface_disabled_ind() */


/*===========================================================================
FUNCTION PS_IFACE_DOWN_IND()

DESCRIPTION
  The given interface has been stopped.   Call callbacks. Pass the down 
  indication information. This also sets the physical link state to DOWN.

  THIS FUNCTION SHOULD NOT BE CALLED IN A ISR.

PARAMETERS
  this_iface_ptr: Ptr to interface control block on which to operate on.
  ps_iface_down_mh_event_info_type  : ptr to data structure containing information
                                 about down indication.

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void ps_iface_down_ind
(
  ps_iface_type                          * this_iface_ptr,
  ps_iface_down_mh_event_info_type       * down_ev_info_ptr
)
{
  ps_iface_down_event_info_type  down_event_info;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
   /*Check if Iface is valid & also iface down reasons should not be Handoff failure types - 12*/
  if (!PS_IFACE_IS_VALID(this_iface_ptr) || ((down_ev_info_ptr->down_reason >> 16) == 12))
  {
    DS_UTILS_ASSERT(0);
    return;
  }

  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

  LOG_MSG_INFO2_4("ps_iface_down_ind(): "
                  "IFACE DOWN IND 0x%x:%d, state %d subs_id %d",
                  this_iface_ptr->name,
                  this_iface_ptr->instance,
                  PS_IFACEI_GET_STATE(this_iface_ptr),
                  PS_IFACE_GET_SUBS_ID(this_iface_ptr));

  switch(PS_IFACEI_GET_STATE(this_iface_ptr))
  {
    case IFACE_DOWN:
    case IFACE_COMING_UP:
    case IFACE_CONFIGURING:
    case IFACE_ROUTEABLE:
    case IFACE_UP:
    case IFACE_GOING_DOWN:
    case IFACE_LINGERING:
    {
      down_event_info.addr_family =
               this_iface_ptr->iface_private.addr_family;
	  
	  /*-------------------------------------------------------------------------
        Make sure that all the secondary flows that belong to this iface are
        deleted
      -------------------------------------------------------------------------*/
      if (PS_IFACEI_GET_NUM_SEC_FLOWS(this_iface_ptr) != 0)
      {
        LOG_MSG_ERROR_2("ps_iface_down_ind(): "
                        "Flows are still present on iface, 0x%x:%d",
                        this_iface_ptr->name, this_iface_ptr->instance);
        DS_UTILS_ASSERT(0);
        return;
      }
	  
      /*---------------------------------------------------------------------
        Update event info for IFACE_DOWN_EV
      ---------------------------------------------------------------------*/
      down_event_info.proc_id = this_iface_ptr->iface_private.proc_id;
      down_event_info.state   = this_iface_ptr->iface_private.state;
      memscpy(&(down_event_info.bearer_tech), 
              sizeof(ds_sys_system_status_info_type),
              &(down_ev_info_ptr->bearer_tech),
              sizeof(ds_sys_system_status_info_type));

      /*---------------------------------------------------------------------
        If this Iface is being arbitrated on, overwrite down reason code,
        this solution is hacky and needs to change. The reason code shld
        come from mode handler.
      ---------------------------------------------------------------------*/
      if (PS_IFACEI_IS_IN_ARBITRATION( this_iface_ptr ))
      {
        down_event_info.netdown_reason = PS_NET_DOWN_REASON_APP_PREEMPTED;
        PS_IFACEI_RESET_ARBITRATION_IN_PROGRESS( this_iface_ptr );
      }
      else
      {
        down_event_info.netdown_reason = down_ev_info_ptr->down_reason;
      }
	  
	  /*---------------------------------------------------------------------
        Reset iface
      ---------------------------------------------------------------------*/
      ps_ifacei_event_reset_iface(this_iface_ptr,
                                  IFACE_DOWN_EV,
                                  &down_event_info);

      /*---------------------------------------------------------------------
        Subs ID should be reset only here on PS_IFACE_DOWN and not on
        IFACE_ACTIVE_OUT_OF_USE_EV
      ---------------------------------------------------------------------*/
      PS_IFACE_SET_SUBS_ID(this_iface_ptr, PS_SYS_DEFAULT_SUBS);
      break;
    }

    case IFACE_DISABLED:
    {
      LOG_MSG_ERROR_2("ps_iface_down_ind(): "
                       "Cannot move disabled iface %x:%d to down",
                       this_iface_ptr->name, this_iface_ptr->instance);
      break;
    }

    default:
    {
      DS_UTILS_ASSERT(0);
    }      
  } /* switch(iface_state) */

  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

} /* ps_iface_down_ind() */

/*===========================================================================
FUNCTION PS_IFACE_DOWN_IND_EX()

DESCRIPTION
  The given interface has been stopped.   Call callbacks. Pass the reason for
  the interface going down. This also sets the physical link state to DOWN.

  THIS FUNCTION SHOULD NOT BE CALLED IN A ISR.

PARAMETERS
  this_iface_ptr: Ptr to interface control block on which to operate on.
  ps_iface_net_down_reason_type : network down reason.

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void ps_iface_down_ind_ex
(
  ps_iface_type                * this_iface_ptr,
  ps_iface_net_down_reason_type  down_reason
)
{
  ps_iface_down_mh_event_info_type  down_event_info;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  memset(&down_event_info, 0, sizeof(down_event_info));
  down_event_info.bearer_tech.rat_value =  DS_SYS_RAT_UNKNOWN;
  down_event_info.down_reason = down_reason;
  ps_iface_down_ind(this_iface_ptr, &down_event_info);
} /* ps_iface_down_ind_ex() */

/*===========================================================================
FUNCTION PS_IFACE_CONFIGURING_IND()

DESCRIPTION
  The given interface has been set to configuring.

  THIS FUNCTION SHOULD NOT BE CALLED IN AN ISR.

PARAMETERS
  this_iface_ptr: Ptr to interface control block on which to operate on.

RETURN VALUE
  None

DEPENDENCIES

SIDE EFFECTS
  None
===========================================================================*/
// do a switch on all states: if disabled fail
void ps_iface_configuring_ind
(
  ps_iface_type *this_iface_ptr
)
{
  ps_flow_type                * ps_flow_ptr;
  ps_iface_event_info_u_type    event_info;
  ps_phys_link_type           * ps_phys_link_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (!PS_IFACE_IS_VALID(this_iface_ptr))
  {
    DS_UTILS_ASSERT(0);
    return;
  }

  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

  LOG_MSG_INFO2_4("ps_iface_configuring_ind(): "
                  "IFACE CONFIGURING IND 0x%x:%d, state %d subs_id %d",
                  this_iface_ptr->name,
                  this_iface_ptr->instance,
                  PS_IFACEI_GET_STATE(this_iface_ptr),
                  PS_IFACE_GET_SUBS_ID(this_iface_ptr));

  /*-------------------------------------------------------------------------
    Need to bind here since ifaces like SIO could potentially call
    routeable_ind() without calling iface_up_ind() first
  -------------------------------------------------------------------------*/
  /*-------------------------------------------------------------------------
    Can't bind default flow to primary phys link for logical ifaces since
    they don't have phys links. Binding is done explictly when logical iface
    associates with a physical iface

    Binding default flow of an iface with phys-link-dynamic-binding should
    already have happened before the iface reaching configuring state. This
    can be indicated by the non-NULL phys link pointer in the default flow.
  -------------------------------------------------------------------------*/
  if (!PS_IFACEI_IS_LOGICAL(this_iface_ptr))
  {
    ps_flow_ptr = PS_IFACEI_GET_DEFAULT_FLOW(this_iface_ptr);
    ps_phys_link_ptr = PS_IFACEI_GET_PHYS_LINK(this_iface_ptr);
    if (ps_flow_ptr == NULL || ps_phys_link_ptr == NULL)
    {

      PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
      LOG_MSG_ERROR_INT_0("ps_iface_configuring_ind:flow_ptr or phys_link_ptr is NULL");
      return;
    }
    else
    {
      (void)ps_flowi_bind_phys_link(ps_flow_ptr,ps_phys_link_ptr);
    }
  }

  /*-------------------------------------------------------------------------
    Store the old state, set the new state to configuring and call the
    callbacks
  -------------------------------------------------------------------------*/
  event_info.state = this_iface_ptr->iface_private.state;
  this_iface_ptr->iface_private.state = IFACE_CONFIGURING;
  LOG_MSG_INFO4_0 ("ps_iface_configuring_ind(): "
                   "Calling event cbacks");
  ps_ifacei_invoke_event_cbacks(this_iface_ptr,
                                NULL,
                                IFACE_CONFIGURING_EV,
                                event_info);
  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

} /* ps_iface_configuring_ind() */



/*===========================================================================
FUNCTION PS_IFACE_ROUTEABLE_IND()

DESCRIPTION
  The given interface has been set to routeable. The interface needs to be in
  the UP state before this indication is called.

  THIS FUNCTION SHOULD NOT BE CALLED IN AN ISR.

PARAMETERS
  this_iface_ptr: ptr to interface control block on which to operate on.

RETURN VALUE
  None

DEPENDENCIES

SIDE EFFECTS
  None
===========================================================================*/
// do a switch on all states: if disabled fail
void ps_iface_routeable_ind
(
  ps_iface_type *this_iface_ptr
)
{
  ps_flow_type                * ps_flow_ptr;
  ps_iface_event_info_u_type    event_info;
  ps_phys_link_type           * ps_phys_link_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (!PS_IFACE_IS_VALID(this_iface_ptr))
  {
    DS_UTILS_ASSERT(0);
    return;
  }

  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

  if (this_iface_ptr->name != SIO_IFACE)
  {
    if (PS_IFACEI_GET_STATE(this_iface_ptr) == IFACE_DOWN ||
        PS_IFACEI_GET_STATE(this_iface_ptr) == IFACE_GOING_DOWN)
    {
      LOG_MSG_ERROR_3("ps_iface_routeable_ind(): "
                      "Iface 0x%x:%d in unsupported state %d",
                      this_iface_ptr->name,
                      this_iface_ptr->instance,
                      PS_IFACEI_GET_STATE(this_iface_ptr));
      PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);
      return;
    }
  }

  LOG_MSG_INFO2_4("ps_iface_routeable_ind(): "
                  "IFACE ROUTEABLE IND 0x%x:%d, state %d subs_id %d",
                  this_iface_ptr->name,
                  this_iface_ptr->instance,
                  PS_IFACEI_GET_STATE(this_iface_ptr),
                  PS_IFACE_GET_SUBS_ID(this_iface_ptr));

  /*-------------------------------------------------------------------------
    Need to bind here since ifaces like SIO could potentially call
    routeable_ind() without calling iface_up_ind() first
  -------------------------------------------------------------------------*/
  /*-------------------------------------------------------------------------
    Can't bind default flow to primary phys link for logical ifaces since
    they don't have phys links. Binding is done explictly when logical iface
    associates with a physical iface

    Binding default flow of an iface with phys-link-dynamic-binding should
    already have happened before the iface reaching routeable state. This
    can be indicated by the non-NULL phys link pointer in the default flow.
  -------------------------------------------------------------------------*/
  if (!PS_IFACEI_IS_LOGICAL(this_iface_ptr))
  {
    ps_flow_ptr = PS_IFACEI_GET_DEFAULT_FLOW(this_iface_ptr);
    ps_phys_link_ptr = PS_IFACEI_GET_PHYS_LINK(this_iface_ptr);
    if (ps_flow_ptr == NULL || ps_phys_link_ptr == NULL)
    {

      PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
      LOG_MSG_ERROR_INT_0("ps_iface_routeable_ind(): "
                    "ps_iface_configuring_ind:flow_ptr or phys_link_ptr is NULL");
      return;
    }
    else
    {
      (void)ps_flowi_bind_phys_link(ps_flow_ptr,ps_phys_link_ptr);
    }
  }

  /*-------------------------------------------------------------------------
    Store the old state, set the new state to routeable and call the callbacks
  -------------------------------------------------------------------------*/
  event_info.state = this_iface_ptr->iface_private.state;
  this_iface_ptr->iface_private.state = IFACE_ROUTEABLE;
  
  ps_ifacei_invoke_event_cbacks(this_iface_ptr,
                                NULL,
                                IFACE_ROUTEABLE_EV,
                                event_info);
  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

} /* ps_iface_routeable_ind() */



/*===========================================================================
FUNCTION PS_IFACE_UP_IND()

DESCRIPTION
  The given interface is up.  Call callbacks.

  THIS FUNCTION SHOULD NOT BE CALLED IN AN ISR.

PARAMETERS
  this_iface_ptr: ptr to interface control block on which to operate on.

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
// do a switch on all states: if disabled fail
void ps_iface_up_ind
(
  ps_iface_type *this_iface_ptr
)
{
  ps_flow_type                * ps_flow_ptr;
  ps_iface_event_info_u_type    event_info;
  ps_phys_link_type           * ps_phys_link_ptr;
  void                        * dhcp_client_handle = NULL;
  uint32                        call_type;
  boolean                       ret_val = FALSE;
  ps_iface_type               * um_iface_ptr = NULL;
  uint8                         data_path_bridge_mask = 0;
  uint8                         data_path_bridge_mask_comp = 0;
  ps_iface_type               * comp_iface_ptr = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (!PS_IFACE_IS_VALID(this_iface_ptr))
  {
    DS_UTILS_ASSERT(0);
    return;
  }

  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

  LOG_MSG_INFO2_4("ps_iface_up_ind(): "
                  "IFACE UP IND 0x%x:%d, state %d subs_id %d",
                  this_iface_ptr->name,
                  this_iface_ptr->instance,
                  PS_IFACEI_GET_STATE(this_iface_ptr),
                  PS_IFACE_GET_SUBS_ID(this_iface_ptr));

               
  if (UMTS_IFACE == this_iface_ptr->name)
  {
    ret_val = ps_dpm_check_dpm_um_info_exist(this_iface_ptr);
    if (FALSE == ret_val)
    {
      LOG_MSG_ERROR_INT_0("ps_iface_up_ind():   NULL DPM UM INFO");    
    }
  }  
  /*-------------------------------------------------------------------------
    If the ref count is zero then move IFACE to LINGERING, else move to UP
  -------------------------------------------------------------------------*/
  if ((PS_IFACE_GET_REF_CNT(this_iface_ptr) == 0) &&
      this_iface_ptr->linger_cmd_f_ptr != NULL &&
      this_iface_ptr->iface_private.linger_timeout_val != 0)
  {
    event_info.state = this_iface_ptr->iface_private.state;
    this_iface_ptr->iface_private.state = IFACE_LINGERING;
    ps_ifacei_invoke_event_cbacks( this_iface_ptr,
                                   NULL,
                                   IFACE_LINGERING_EV,
                                   event_info);

    (void) this_iface_ptr->linger_cmd_f_ptr( this_iface_ptr, NULL);
  }
  else
  {
    /*-----------------------------------------------------------------------
      Need to bind here since ifaces like SIO post ps_iface_up_ind() directly
      without calling ps_iface_bring_up_cmd() first
    -----------------------------------------------------------------------*/
    /*-----------------------------------------------------------------------
      Can't bind default flow to primary phys link for logical ifaces since
      they don't have phys links. Binding is done explictly when logical iface
      associates with a physical iface

      Binding default flow of an iface with phys-link-dynamic-binding should
      already have happened before the iface reaching up state. This
      can be indicated by the non-NULL phys link pointer in the default flow.
    -----------------------------------------------------------------------*/
    if (!PS_IFACEI_IS_LOGICAL(this_iface_ptr))
    {
      ps_flow_ptr = PS_IFACEI_GET_DEFAULT_FLOW(this_iface_ptr);
      ps_phys_link_ptr = PS_IFACEI_GET_PHYS_LINK(this_iface_ptr);
      if (ps_flow_ptr == NULL || ps_phys_link_ptr == NULL)
      {

        PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
        LOG_MSG_ERROR_INT_0("ps_iface_up_ind():flow_ptr or phys_link_ptr is NULL");
        return;
      }
      else
      {
        (void)ps_flowi_bind_phys_link(ps_flow_ptr,ps_phys_link_ptr);
      }
    }

    /*-----------------------------------------------------------------------
      store the previous state, change the current state to UP and call the
      callbacks.
    -----------------------------------------------------------------------*/
    if (PS_IFACE_GET_REF_CNT(this_iface_ptr) > 0)
    {
      event_info.iface_up_info.is_iface_in_use = TRUE;
    }
    else
    {
      event_info.iface_up_info.is_iface_in_use = FALSE;
    }

    event_info.iface_up_info.proc_id = this_iface_ptr->iface_private.proc_id;
    event_info.iface_up_info.state = this_iface_ptr->iface_private.state;

    this_iface_ptr->iface_private.state = IFACE_UP;
    ps_ifacei_invoke_event_cbacks(this_iface_ptr,
                                  NULL,
                                  IFACE_UP_EV,
                                  event_info);

    um_iface_ptr =  PS_IFACEI_GET_ASSOC_IFACE(this_iface_ptr);
    
    if (PS_IFACE_IS_VALID( um_iface_ptr))
    {
      /*-------------------------------------------------------------------------
      Imply physical iface data path bridge mask flag into logical iface iface. 
      -------------------------------------------------------------------------*/
      data_path_bridge_mask = PS_IFACE_GET_DATA_PATH_FLAG(um_iface_ptr);
      
      /*-------------------------------------------------------------------------
      Imply Companion iface data path bridge mask flag into current logical iface
      and physical iface, if Companion iface is valid and bridge mask is not equal. 
      --------------------------------------------------------------------------*/
      comp_iface_ptr = ps_dpm_get_companion_iface( um_iface_ptr);
      if( PS_IFACE_IS_VALID( comp_iface_ptr) )
      {
      /*-------------------------------------------------------------------------
        Copying copmanion iface bridge mask only when current umiface
        bridge mask is enabled for all clients.
               
        Example1:
               um_iface_ptr --> data_path_bridge_mask = 1111
               PS_IFACE_IOCTL_ENABLE_CLIENTS =  1111
               so the result = 1111
               which means umiface is in bridge state 
               so we need to copy from companion iface.

        Example2:
               um_iface_ptr --> data_path_bridge_mask = 1110
               PS_IFACE_IOCTL_ENABLE_CLIENTS = 1111
               so the result = 1110
               which means umiface is already in un-bridge state 
               so we dont need to copy from companion iface.
       --------------------------------------------------------------------------*/
        if( ( data_path_bridge_mask & PS_IFACE_BRIDGING_ALLOWED ) == 
                                      PS_IFACE_BRIDGING_ALLOWED )
        {
          data_path_bridge_mask_comp = PS_IFACE_GET_DATA_PATH_FLAG(comp_iface_ptr);
          LOG_MSG_INFO1_2("ps_iface_up_ind data_path_bridge_mask %d comp %d",
                           data_path_bridge_mask,data_path_bridge_mask_comp);	
          data_path_bridge_mask = data_path_bridge_mask_comp;
          PS_IFACE_SET_DATA_PATH_FLAG(um_iface_ptr, data_path_bridge_mask); 		  
        }
      }
      PS_IFACE_SET_DATA_PATH_FLAG(this_iface_ptr, data_path_bridge_mask);
    }
      
    /*-----------------------------------------------------------------------
      Generate bearer tech change when iface is up
    -----------------------------------------------------------------------*/
    ps_ifacei_generate_data_bearer_tech_ind(this_iface_ptr);

    dhcp_client_handle = 
                   ps_iface_get_dhcp_stateless_client_handle(this_iface_ptr);

    if (NULL == dhcp_client_handle)
    {
      event_info.extended_ip_config_succeeded = TRUE;
      ps_ifacei_invoke_event_cbacks(this_iface_ptr,
                                    NULL,
                                    IFACE_EXTENDED_IP_CONFIG_EV,
                                    event_info);
    }
  }

    if (this_iface_ptr->name == EPC_IFACE)
  {
    /*-----------------------------------------------------------------------
     If ref_cnt is greater than 1, all calls are up on the interface.
     PS_IFACE_PROC_ID_ALL is to convey all calls are up
    -----------------------------------------------------------------------*/
    if (PS_IFACE_GET_REF_CNT(this_iface_ptr) > 1)
    {
      call_type = PS_DATA_CALL_INFO_LOGGING_ALL_CALLS;
    }
    else
    {
      /*---------------------------------------------------------------------
        PS_IFACE_PROC_ID_LOCAL and PS_IFACE_PROC_ID_ANY represent modem
        originated calls.PS_IFACE_PROC_ID_ANY additionally represents call on
        a shared interface. All other proc_id values represent tethered calls
      ---------------------------------------------------------------------*/
      if (this_iface_ptr->iface_private.proc_id == PS_IFACE_PROC_ID_LOCAL ||
          this_iface_ptr->iface_private.proc_id == PS_IFACE_PROC_ID_ANY)
      {
        call_type = PS_DATA_CALL_INFO_LOGGING_MODEM_EMBEDDED;
      }
      else
      {
        call_type = this_iface_ptr->iface_private.proc_id;
      }
    }
    /*-----------------------------------------------------------------------
      Generate DIAG events with call status information
    -----------------------------------------------------------------------*/
    ps_data_call_info_logging_event_report(this_iface_ptr,
                                  PS_DATA_CALL_INFO_LOGGING_STATUS_ACTIVATED,
                                  call_type);
  }

  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

} /* ps_iface_up_ind() */



/*===========================================================================
FUNCTION PS_IFACE_PHYS_LINK_DOWN_IND()

DESCRIPTION
  The physical link of the given interface is down (707 dormancy).  Call
  callbacks.

  THIS FUNCTION SHOULD NOT BE CALLED IN A ISR.

PARAMETERS
  this_iface_ptr: ptr to interface control block on which to operate on.

RETURN VALUE
  None

DEPENDENCIES

SIDE EFFECTS
  None
===========================================================================*/
// do a switch on all phys link states
void ps_iface_phys_link_down_ind
(
  const ps_iface_type *this_iface_ptr
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (!PS_IFACE_IS_VALID(this_iface_ptr))
  {
    DS_UTILS_ASSERT(0);
    return;
  }

  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

  LOG_MSG_INFO2_3("ps_iface_phys_link_down_ind(): "
                  "IFACE PHYS LINK DOWN IND 0x%x:%d, state %d",
                  this_iface_ptr->name,
                  this_iface_ptr->instance,
                  PS_IFACEI_GET_STATE(this_iface_ptr));

  /*-------------------------------------------------------------------------
    Call the underlying phys link down ind.
  -------------------------------------------------------------------------*/
  ps_phys_link_down_ind(PS_IFACEI_GET_PHYS_LINK(this_iface_ptr));

  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

} /* ps_iface_phys_link_down_ind() */



/*===========================================================================
FUNCTION PS_IFACE_PHYS_LINK_UP_IND()

DESCRIPTION
  The physical link of the given interface is up.  Call callbacks.

  THIS FUNCTION SHOULD NOT BE CALLED IN A ISR.

PARAMETERS
  this_iface_ptr: ptr to interface control block on which to operate on.

RETURN VALUE
  None

DEPENDENCIES

SIDE EFFECTS
  None
===========================================================================*/
// do a switch on all phys link states
void ps_iface_phys_link_up_ind
(
  const ps_iface_type *this_iface_ptr
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (!PS_IFACE_IS_VALID(this_iface_ptr))
  {
    DS_UTILS_ASSERT(0);
    return;
  }

  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

  LOG_MSG_INFO2_3("ps_iface_phys_link_up_ind(): "
                  "IFACE PHYS LINK UP IND 0x%x:%d, state %d",
                  this_iface_ptr->name,
                  this_iface_ptr->instance,
                  PS_IFACEI_GET_STATE(this_iface_ptr));

  /*-------------------------------------------------------------------------
    Call the underlying phys link up ind.
  -------------------------------------------------------------------------*/
  ps_phys_link_up_ind(PS_IFACEI_GET_PHYS_LINK(this_iface_ptr));

  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

} /* ps_iface_phys_link_up_ind() */


#ifdef FEATURE_DATA_PS_IPV6
/*===========================================================================
FUNCTION PS_IFACE_VALID_RA_IND()

DESCRIPTION
  This fucntion is called to indicate that a valid RA is received
  Appropriate event callback is posted.

  THIS FUNCTION SHOULD NOT BE CALLED IN A ISR.

PARAMETERS
  this_iface_ptr: ptr to interface control block on which to operate on
  ra_life_time: lifetime of the RA

RETURN VALUE
  None

DEPENDENCIES

SIDE EFFECTS
  None
===========================================================================*/
void ps_iface_valid_ra_ind
(
  ps_iface_type                       *this_iface_ptr,
  uint16                               ra_life_time
)
{
  ps_iface_event_info_u_type event_info;
  ps_ipv6_iid_params_type    iid_param;
  ps_ip_addr_type            ip_addr;
  int16                      ps_errno;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (!PS_IFACE_IS_VALID(this_iface_ptr) || ra_life_time == 0)
  {
    DS_UTILS_ASSERT(0);
    return;
  }

  LOG_MSG_INFO2_3("ps_iface_valid_ra_ind(): "
                  "IFACE VALID RA IND 0x%x:%d, lifetime %d",
                  this_iface_ptr->name,
                  this_iface_ptr->instance,
                  ra_life_time);

  /*-------------------------------------------------------------------------
    Although iface contents are not changed, need to enter critical section
    as events are posted
  -------------------------------------------------------------------------*/
  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

  /*-------------------------------------------------------------------------
    Valid Prefix was received, if IPv6 privacy extensions are enabled
    generate a new private shareable address.
  -------------------------------------------------------------------------*/
    iid_param.is_unique   = FALSE;
    iid_param.app_request = FALSE;

    if ( -1 == ps_iface_generate_priv_ipv6_addr(this_iface_ptr,
                                               &ip_addr,
                                               &iid_param,
                                               &ps_errno))
    {
      if (ps_errno != DS_EWOULDBLOCK)
      {
        LOG_MSG_ERROR_INT_0("ps_iface_valid_ra_ind(): "
                        "Failed to create default IPv6 private shareable addr");
      }
    }

  /*-------------------------------------------------------------------------
    Call the event callback passing the RA life time
  -------------------------------------------------------------------------*/
  event_info.ra_lifetime = ra_life_time;

  ps_ifacei_invoke_event_cbacks(this_iface_ptr,
                                NULL,
                                IFACE_VALID_RA_EV,
                                event_info);

  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);
  return;

} /* ps_iface_valid_ra_ind() */
#endif /* FEATURE_DATA_PS_IPV6 */


/*===========================================================================
FUNCTION PS_IFACE_IPFLTR_UPDATED_IND()

DESCRIPTION
  This fucntion is called to indicate that the number of IP filters
  installed in this iface have changed. Appropriate event callback
  is posted.

  THIS FUNCTION SHOULD NOT BE CALLED IN A ISR.

PARAMETERS
  this_iface_ptr: ptr to interface control block on which to operate on
  client: filtering client id for which the filters changed
  n_added: number of new filters added
  n_deleted: number of existing filters deleted

RETURN VALUE
  None

DEPENDENCIES

SIDE EFFECTS
  None
===========================================================================*/
void ps_iface_ipfltr_updated_ind
(
  ps_iface_type                       *this_iface_ptr,
  ps_iface_ipfltr_client_id_enum_type client,
  int                                 prev_cnt,
  int                                 new_cnt
)
{
  ps_iface_event_info_u_type    event_info;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (this_iface_ptr != NULL && !PS_IFACE_IS_VALID(this_iface_ptr))
  {
    DS_UTILS_ASSERT(0);
    return;
  }

  /*-------------------------------------------------------------------------
    Although iface contents are not changed, need to enter critical section
    as events are posted
  -------------------------------------------------------------------------*/
  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

  /*-------------------------------------------------------------------------
    Call the event callback passing the currently installed num ip filters
  -------------------------------------------------------------------------*/
  event_info.ipfltr_info.client        = client;
  event_info.ipfltr_info.curr_fltr_cnt = new_cnt;

  LOG_MSG_INFO2_4("If 0x%p, client %d, n_filters %d -> %d",
                  this_iface_ptr,
                  client,
                  prev_cnt,
                  new_cnt);

  ps_ifacei_invoke_event_cbacks(this_iface_ptr,
                                NULL,
                                IFACE_IPFLTR_UPDATED_EV,
                                event_info);

  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

} /* ps_iface_ipfltr_updated_ind() */



/*===========================================================================
FUNCTION PS_IFACE_GENERIC_IND()

DESCRIPTION
  This function is called to indicate that an iface event has occurred. This
  function is to be used for iface events that are not directly applicable
  to the iface itself. (Ie. generic iface events)

  THIS FUNCTION SHOULD NOT BE CALLED IN A ISR.

PARAMETERS
  this_iface_ptr: ptr to interface control block on which to operate on
  event: event that has occurred.
  usr_data_ptr: data associated with the event.

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void ps_iface_generic_ind
(
  ps_iface_type                       *this_iface_ptr,
  ps_iface_event_enum_type             event,
  void                                *event_data_ptr
)
{
  ps_iface_event_info_u_type event_info;
  ps_iface_mcast_event_info_type            *mcast_info  = NULL;
  ps_iface_ioctl_bearer_tech_changed_type   *tech_change = NULL;
  ps_iface_ioctl_extended_ip_config_type *extended_ip_config_info = NULL;  
  ps_iface_ioctl_extended_ip_config_ex_type *extended_ip_config_ex_info 
                                               = NULL;
  ps_iface_up_event_info_type            *iface_in_use_ptr     = NULL;
#ifdef FEATURE_HDR_BCMCS_2p0
  #error code not present
#endif /* FEATURE_HDR_BCMCS_2p0 */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (!PS_IFACE_IS_VALID(this_iface_ptr))
  {
    DS_UTILS_ASSERT(0);
    return;
  }

  DS_INTERFACE_LOG_MSG_INFO1_3(PS_LOGGING_CAT_PS_IFACE, "ps_iface_generic_ind(): "
                  "IFACE GENERIC EVENT IND 0x%x:%d, event %d",
                  this_iface_ptr->name, this_iface_ptr->instance, event);

  /*-------------------------------------------------------------------------
    Initializing event_info to shut up lint
  -------------------------------------------------------------------------*/
  event_info.n_filters = 0;

  /*-------------------------------------------------------------------------
    Although iface contents are not changed, need to enter critical section
    as events are posted
  -------------------------------------------------------------------------*/
  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

  switch (event)
  {
    /*-----------------------------------------------------------------------
      IFACE_ACTIVE_OUT_OF_USE_EV
    -----------------------------------------------------------------------*/
    case IFACE_ACTIVE_OUT_OF_USE_EV:
      if (event_data_ptr == NULL)
      {
        PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
        LOG_MSG_ERROR_INT_1("NULL event data ptr %d", event);
        DS_UTILS_ASSERT(0);
        return;
      }
            
      ps_ifacei_event_reset_iface(this_iface_ptr, event, event_data_ptr);

      PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);
      return;

    /*-----------------------------------------------------------------------
      IFACE_ACTIVE_IN_USE_EV
    -----------------------------------------------------------------------*/
    case IFACE_ACTIVE_IN_USE_EV:
      if (event_data_ptr == NULL)
      {
        PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
        LOG_MSG_ERROR_INT_1("NULL event data ptr %d", event);
        DS_UTILS_ASSERT(0);
        return;
      }

      iface_in_use_ptr = (ps_iface_up_event_info_type *) event_data_ptr;

      event_info.iface_up_info.state = iface_in_use_ptr->state;
      event_info.iface_up_info.proc_id = iface_in_use_ptr->proc_id;
      event_info.iface_up_info.is_iface_in_use =
        iface_in_use_ptr->is_iface_in_use;

      break;

    /*-----------------------------------------------------------------------
      IFACE_MT_REQUEST_EV takes an event_data_ptr of type
      ps_iface_mt_handle_type *
    -----------------------------------------------------------------------*/
    case IFACE_MT_REQUEST_EV:
      if (event_data_ptr == NULL)
      {
        PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
        LOG_MSG_ERROR_INT_1("NULL event data ptr %d", event);
        DS_UTILS_ASSERT(0);
        return;
      }

      event_info.mt_handle = *(ps_iface_mt_handle_type *)(event_data_ptr);
      break;

    /*-----------------------------------------------------------------------
      Multicast events take an event_data_ptr of type
      ps_iface_mcast_event_info_type
    -----------------------------------------------------------------------*/
    case IFACE_MCAST_REGISTER_SUCCESS_EV:
    case IFACE_MCAST_REGISTER_FAILURE_EV:
    case IFACE_MCAST_DEREGISTERED_EV:
      if (event_data_ptr == NULL)
      {
        PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
        LOG_MSG_ERROR_INT_1("NULL event data ptr %d", event);
        DS_UTILS_ASSERT(0);
        return;
      }

      mcast_info                       = (ps_iface_mcast_event_info_type *)
                                         event_data_ptr;
      event_info.mcast_info.handle     = mcast_info->handle;
      event_info.mcast_info.info_code  = mcast_info->info_code;

#ifdef FEATURE_HDR_BCMCS_2p0
      #error code not present
#endif /* FEATURE_HDR_BCMCS_2p0 */
      break;

    /*-----------------------------------------------------------------------
      IFACE_BEARER_TECH_CHANGED_EV takes an event_data_ptr of type
      ps_iface_ioctl_bearer_tech_changed_type *
    -----------------------------------------------------------------------*/
    case IFACE_BEARER_TECH_CHANGED_EV:
      if (this_iface_ptr->iface_private.state == IFACE_DOWN)
      {  
        LOG_MSG_INFO2_3("ps_iface_generic_ind():"
                        "Ignoring IFACE_BEARER_TECH_CHANGED_EV on iface 0x%x:%d"
                        "in state %d",
                        this_iface_ptr->name,
                        this_iface_ptr->instance,
                        this_iface_ptr->iface_private.state);

        PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);
    /*-----------------------------------------------------------------------
       Bearer tech is not expected to be posted by MH for down ifaces
    -----------------------------------------------------------------------*/
        DS_UTILS_ASSERT(0);
        return;
      }
      if (event_data_ptr == NULL)
      {
        PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
        LOG_MSG_ERROR_INT_1("NULL event data ptr %d", event);
        DS_UTILS_ASSERT(0);
        return;
      }

      tech_change = (ps_iface_ioctl_bearer_tech_changed_type *)event_data_ptr;
      memscpy (&(event_info.bearer_tech_changed_info.old_bearer_tech),
              sizeof (ps_iface_bearer_technology_type),
              &(tech_change->old_bearer_tech),
              sizeof (ps_iface_bearer_technology_type));
      memscpy (&(event_info.bearer_tech_changed_info.new_bearer_tech),
              sizeof (ps_iface_bearer_technology_type),
              &(tech_change->new_bearer_tech),
              sizeof (ps_iface_bearer_technology_type));

      if (memcmp(&(event_info.bearer_tech_changed_info),
                 &(this_iface_ptr->event_info_cache.bearer_tech_changed_info), 
                 sizeof(ps_iface_ioctl_bearer_tech_changed_type)) == 0)
      {
		PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);		
        return;
      }

      break;
#ifdef FEATURE_DATA_WLAN_MAPCON
    /*-----------------------------------------------------------------------
      IFACE_BEARER_TECH_EX_CHANGED_EV takes an event_data_ptr of type
      ds_sys_system_status_info_type *
    -----------------------------------------------------------------------*/
    case IFACE_BEARER_TECH_EX_CHANGED_EV:
	  if (this_iface_ptr->iface_private.state == IFACE_DOWN)
	  {
        LOG_MSG_INFO2_3("ps_iface_generic_ind():"
                        		              "Ignoring IFACE_BEARER_TECH_EX_CHANGED_EV on iface 0x%x:%d"
                        		              "in state ​%d",
                        			this_iface_ptr->name,
                        					this_iface_ptr->instance,
                        					 this_iface_ptr->iface_private.state);
					  
		PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);		
    /*-----------------------------------------------------------------------
       Bearer tech is not expected to be posted by MH for down ifaces
    -----------------------------------------------------------------------*/
        DS_UTILS_ASSERT(0);
        return;					  
	  }
	
      if (event_data_ptr == NULL)
      {
        PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
        LOG_MSG_ERROR_INT_1("NULL event data ptr %d", event);
        DS_UTILS_ASSERT(0);
        return;
      }

      memscpy (&(event_info.bearer_tech_ex_changed_info),
              sizeof (ds_sys_system_status_info_type),
              event_data_ptr,
              sizeof (ds_sys_system_status_info_type));

      if (memcmp(&(event_info.bearer_tech_ex_changed_info),
                 &(this_iface_ptr->event_info_cache.bearer_tech_ex_changed_info), 
                 sizeof(ds_sys_system_status_info_type)) == 0)
      {
		PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);		
        return;
      }

      break;
#endif /* FEATURE_DATA_WLAN_MAPCON */

    case IFACE_EXTENDED_IP_CONFIG_EV:
      if (event_data_ptr == NULL)
      {
        PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
        LOG_MSG_ERROR_INT_1("Invalid EXTENDED_IP_CONFIG event data ptr %d", event);
        DS_UTILS_ASSERT(0);
        return;
      }
      extended_ip_config_info =
        (ps_iface_ioctl_extended_ip_config_type *) event_data_ptr;
      event_info.extended_ip_config_succeeded = *extended_ip_config_info;
      break;

    case IFACE_EXTENDED_IP_CONFIG_EX_EV:
      if (event_data_ptr == NULL)
      {
        PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
        LOG_MSG_ERROR_INT_1("Invalid EXTENDED_IP_CONFIG_EX event data ptr %d", event);
        DS_UTILS_ASSERT(0);
        return;
      }
      /*---------------------------------------------------------------------
        In case the event is IFACE_EXTENDED_IP_CONFIG_EX_EV, also invoke 
        callbacks for IFACE_EXTENDED_IP_CONFIG_EV
        Once all external clients handle the new event, there will be no
        need to invoke the callback for old event.
        First invoke callbacks for old event.
      ---------------------------------------------------------------------*/
      extended_ip_config_ex_info =
        (ps_iface_ioctl_extended_ip_config_ex_type *) event_data_ptr;
      event_info.extended_ip_config_change_mask = 
        *extended_ip_config_ex_info;

      event_info.extended_ip_config_succeeded = 
        (event_info.extended_ip_config_change_mask==0)? FALSE:TRUE;
      ps_ifacei_invoke_event_cbacks(
        this_iface_ptr, NULL, IFACE_EXTENDED_IP_CONFIG_EV, event_info);

      /*---------------------------------------------------------------------
        Now that we have invoked callbacks for old event, set event info
        for the new event again.
        Callback for this event will be invoked at end of switch block.
      ---------------------------------------------------------------------*/
      event_info.extended_ip_config_change_mask = 
        *extended_ip_config_ex_info;
      break;
      
    /*-----------------------------------------------------------------------
      IFACE_QOS_AWARE/UNAWARE_SYSTEM_EV takes an event_data_ptr of type
      ps_extended_info_code_enum_type *
    -----------------------------------------------------------------------*/
    case IFACE_QOS_AWARE_SYSTEM_EV:
    case IFACE_QOS_UNAWARE_SYSTEM_EV:
      if (event_data_ptr == NULL)
      {
        PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
        LOG_MSG_ERROR_INT_1("NULL event data ptr %d", event);
        DS_UTILS_ASSERT(0);
        return;
      }

      event_info.qos_aware_info_code =
        *(ps_extended_info_code_enum_type *)(event_data_ptr);
      break;

    case IFACE_FLOW_ADDED_EV:
      event_info.flow_ptr = (ps_flow_type *)(event_data_ptr);
      break;

    case IFACE_FLOW_DELETED_EV:
    case IFACE_707_NETWORK_SUPPORTED_QOS_PROFILES_CHANGED_EV:
      break;

    case IFACE_OUTAGE_NOTIFICATION_EV:
      if (event_data_ptr == NULL)
      {
        PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
        LOG_MSG_ERROR_INT_1("NULL event data ptr %d", event);
        DS_UTILS_ASSERT(0);
        return;
      }

      event_info.outage_notification_info =
        *( (ps_iface_outage_notification_event_info_type *) event_data_ptr);
      event_info.outage_notification_info.time_to_outage += (uint32) msclock();
      break;

    case IFACE_RF_CONDITIONS_CHANGED_EV:
      if (event_data_ptr == NULL)
      {
        PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
        LOG_MSG_ERROR_INT_1("NULL event data ptr %d", event);
        DS_UTILS_ASSERT(0);
        return;
      }

      event_info.rf_conditions_change_info =
        *( (ps_iface_rf_conditions_info_type *) event_data_ptr);
      break;

    case IFACE_ENABLE_HDR_REV0_RATE_INERTIA_SUCCESS_EV:
      break;

    case IFACE_ENABLE_HDR_REV0_RATE_INERTIA_FAILURE_EV:
      if (event_data_ptr == NULL)
      {
        PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
        LOG_MSG_ERROR_INT_1("NULL event data ptr %d", event);
        DS_UTILS_ASSERT(0);
        return;
      }
      event_info.hdr_rev0_rate_inertia_failure_code =
        *(ps_hdr_rev0_rate_inertia_failure_code_enum_type*)(event_data_ptr);
      break;

    case IFACE_ENABLE_HDR_SET_EIDLE_SLOTTED_MODE_SUCCESS_EV:
      break;

    case IFACE_ENABLE_HDR_SET_EIDLE_SLOTTED_MODE_FAILURE_EV:
      if (event_data_ptr == NULL)
      {
        PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
        LOG_MSG_ERROR_INT_1("NULL event data ptr %d", event);
        DS_UTILS_ASSERT(0);
        return;
      }

      event_info.slotted_mode_info.hdr_slotted_mode_failure_code =
        *(ps_hdr_slotted_mode_failure_code_enum_type*)(event_data_ptr);
      break;

    case IFACE_ENABLE_HDR_SET_EIDLE_SLOTTED_MODE_SESSION_CHANGED_EV:
       if (event_data_ptr == NULL)
       {
         PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
         LOG_MSG_ERROR_INT_1("NULL event data ptr %d", event);
         DS_UTILS_ASSERT(0);
         return;
       }

       event_info.slotted_mode_info.sm_current_sci = *(uint8*)(event_data_ptr);
       break;

    case IFACE_MBMS_CONTEXT_ACT_SUCCESS_EV:
    case IFACE_MBMS_CONTEXT_ACT_FAILURE_EV:
    case IFACE_MBMS_CONTEXT_DEACT_SUCCESS_EV:
    case IFACE_MBMS_CONTEXT_DEACT_FAILURE_EV:
      if (event_data_ptr == NULL)
      {
        PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
        LOG_MSG_ERROR_INT_1("NULL event data ptr %d", event);
        DS_UTILS_ASSERT(0);
        return;
      }

      mcast_info = (ps_iface_mcast_event_info_type *)event_data_ptr;
      event_info.mcast_info.handle    = mcast_info->handle;
      break;

    case IFACE_AUTHENTICATING_EV:
    case IFACE_APP_PREEMPTED_EV:
      break;

    case IFACE_FAST_DORMANCY_STATUS_EV:
      if (event_data_ptr == NULL)
      {
        PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
        LOG_MSG_ERROR_INT_1("NULL event data ptr %d", event);
        DS_UTILS_ASSERT(0);
        return;
      }

      memscpy (&(event_info.fast_dorm_status),
              sizeof (ps_iface_fast_dormancy_status_type),
              event_data_ptr,
              sizeof (ps_iface_fast_dormancy_status_type));
      break;

#ifdef FEATURE_DATA_EMBMS

    case IFACE_EMBMS_TMGI_ACTIVATED_EV:
      if (event_data_ptr == NULL)
      {
        PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
        LOG_MSG_ERROR_INT_1("NULL event data ptr %d", event);
        DS_UTILS_ASSERT(0);
        return;
      }

      /*------------------------------------------------------------------------
        Copy the Payload into the local variable
      ------------------------------------------------------------------------*/
      memscpy((void*)&(event_info.embms_tmgi_act_ind_info),
              sizeof(ps_iface_embms_tmgi_activation_info_type),
             event_data_ptr,
             sizeof(ps_iface_embms_tmgi_activation_info_type));

      break;

  case IFACE_EMBMS_TMGI_DEACTIVATED_EV:
      if (event_data_ptr == NULL)
      {
        PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
        LOG_MSG_ERROR_INT_1("NULL event data ptr %d", event);
        DS_UTILS_ASSERT(0);
        return;
      }

      /*------------------------------------------------------------------------
        Copy the Payload into the local variable
      ------------------------------------------------------------------------*/
      memscpy((void*)&(event_info.embms_tmgi_deact_ind_info),
              sizeof(ps_iface_embms_tmgi_deactivation_info_type),
             event_data_ptr,
             sizeof(ps_iface_embms_tmgi_deactivation_info_type));

      break;

      case IFACE_EMBMS_TMGI_ACTIVATE_DEACTIVATE_RESPONSE_EV:
      if (event_data_ptr == NULL)
      {
        PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
        LOG_MSG_ERROR_INT_1("NULL event data ptr %d", event);
        DS_UTILS_ASSERT(0);
        return;
      }

      /*------------------------------------------------------------------------
        Copy the Payload into the local variable
      ------------------------------------------------------------------------*/
      memscpy((void*)&(event_info.embms_tmgi_act_deact_ind_info),
              sizeof(ps_iface_embms_tmgi_activation_deactivation_info_type),
             event_data_ptr,
             sizeof(ps_iface_embms_tmgi_activation_deactivation_info_type));

      break;

  case IFACE_EMBMS_ACTIVE_TMGI_LIST_EV:
  case IFACE_EMBMS_AVAILABLE_TMGI_LIST_EV:
  case IFACE_EMBMS_WARNING_TMGI_LIST_EV:
      if (event_data_ptr == NULL)
      {
        PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
        LOG_MSG_ERROR_INT_1("NULL event data ptr %d", event);
        DS_UTILS_ASSERT(0);
        return;
      }

      /*------------------------------------------------------------------------
        Copy the Payload into the local variable
      ------------------------------------------------------------------------*/
      memscpy((void*)&(event_info.embms_tmgi_list_ind_info),
              sizeof(ps_iface_embms_tmgi_list_info_type),
             event_data_ptr,
             sizeof(ps_iface_embms_tmgi_list_info_type));

      break;  

     case IFACE_EMBMS_AVAILABLE_SAI_LIST_EV:
       if (event_data_ptr == NULL)
       {
         PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
         LOG_MSG_ERROR_INT_1("NULL event data ptr %d", event);
         DS_UTILS_ASSERT(0);
         return;
       }
       /*------------------------------------------------------------------------
         Copy the Payload into the local variable
       ------------------------------------------------------------------------*/
       memscpy((void*)&(event_info.embms_sai_list_ind_info),
               sizeof(ps_iface_embms_sai_info_type),
              event_data_ptr,
              sizeof(ps_iface_embms_sai_info_type));


       break;
#endif /* FEATURE_DATA_EMBMS */

#ifdef FEATURE_DATA_WLAN_MAPCON   
  case IFACE_REVERSE_IP_TRANSPORT_CONFIG_EV:   
    /* No associated event data */
    break;
    
  case IFACE_HANDOFF_INIT_EV:
  case IFACE_HANDOFF_STATUS_SUCCESS_EV:
    if (event_data_ptr == NULL)
    {
      PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
      LOG_MSG_ERROR_INT_1("NULL event data ptr %d", event);
      DS_UTILS_ASSERT(0);
      return;
    }
    memscpy((void*)&(event_info.handoff_event_info),
            sizeof(ps_iface_handoff_event_info_type),
            event_data_ptr,
            sizeof(ps_iface_handoff_event_info_type));
    break;

  case IFACE_HANDOFF_STATUS_FAILURE_EV:
      if (event_data_ptr == NULL)
      {
        PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
        LOG_MSG_ERROR_INT_1("NULL event data ptr %d", event);
        DS_UTILS_ASSERT(0);
        return;
      }
     memscpy((void*)&(event_info.handoff_failure_event_info),
              sizeof(ps_iface_handoff_failure_event_info_type),
              event_data_ptr,
              sizeof(ps_iface_handoff_failure_event_info_type));
    break;
  
  case IFACE_REVERSE_IP_TRANSPORT_IPFLTR_CHANGED_EV:
    if (event_data_ptr == NULL)
    {
      PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
      LOG_MSG_ERROR_INT_1("NULL event data ptr %d", event);
      DS_UTILS_ASSERT(0);
      return;
    }
    /*------------------------------------------------------------------------
      Copy the Payload into the local variable
    ------------------------------------------------------------------------*/
    memscpy((void*)&(event_info.fltr_change_info),
            sizeof(ps_iface_ipfltr_change_type),
            event_data_ptr,
            sizeof(ps_iface_ipfltr_change_type));
    break;
#endif /* FEATURE_DATA_WLAN_MAPCON */ 
  case IFACE_POWERSAVE_FILTERING_MODE_CHANGE_EV:
    if (event_data_ptr == NULL)
    {
      PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
      LOG_MSG_ERROR_INT_1("NULL event data ptr %d", event);
      DS_UTILS_ASSERT(0);
      return;
    }
    event_info.powersave_filtering_info.powersave_mode = 
      *(boolean*)event_data_ptr;

    break;
    case IFACE_APN_PARAM_CHANGED_EV:
      if (event_data_ptr == NULL)
      {
        PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
        LOG_MSG_ERROR_INT_1("NULL event data ptr %d", event);
        DS_UTILS_ASSERT(0);
        return;
      }
      /*------------------------------------------------------------------------
      Copy the Payload into the local variable
    ------------------------------------------------------------------------*/
      memscpy((void *)&(event_info.apn_param_change_info_type),
              sizeof(ps_iface_changed_apn_param_info_type),
              event_data_ptr,
              sizeof(ps_iface_changed_apn_param_info_type)); 
    break;

     case IFACE_EMBMS_CONTENT_DESC_CONTROL_EV:
       if (event_data_ptr == NULL)
       {
         PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
         LOG_MSG_ERROR_INT_1("NULL event data ptr %d", event);
         DS_UTILS_ASSERT(0);
         return;
       }
       /*------------------------------------------------------------------------
         Copy the Payload into the local variable
       ------------------------------------------------------------------------*/
       memscpy((void*)&(event_info.embms_content_desc_info),
               sizeof(ps_iface_embms_content_desc_update_type),
              event_data_ptr,
              sizeof(ps_iface_embms_content_desc_update_type));


       break;

     case IFACE_EMBMS_SVC_INTEREST_IND_EV:
       if (event_data_ptr == NULL)
       {
         PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
         LOG_MSG_ERROR_INT_1("NULL event data ptr %d", event);
         DS_UTILS_ASSERT(0);
         return;
       }
       /*------------------------------------------------------------------------
         Copy the Payload into the local variable
       ------------------------------------------------------------------------*/
       memscpy((void*)&(event_info.embms_svc_interest_ind_info),
               sizeof(ps_iface_embms_svc_interest_ind_type),
              event_data_ptr,
              sizeof(ps_iface_embms_svc_interest_ind_type));
       break;

    case IFACE_RESET_EV:
      if (event_data_ptr == NULL)
      {
        PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
        LOG_MSG_ERROR_INT_1("NULL event data ptr %d", event);
        DS_UTILS_ASSERT(0);
        return;
      }
       /*------------------------------------------------------------------------
         Copy the Payload into the local variable
       ------------------------------------------------------------------------*/
       memscpy((void*)&(event_info.iface_down_info),
               sizeof(ps_iface_down_event_info_type),
               event_data_ptr,
               sizeof(ps_iface_down_event_info_type));
       break;

     case IFACE_SET_OFFLOAD_FC_EV:
       if (event_data_ptr == NULL)
       {
         PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
         LOG_MSG_ERROR_1("NULL event data ptr %d", event);
         DS_UTILS_ASSERT(0);
         return;
       }
       /*------------------------------------------------------------------------
         Copy the Payload into the local variable
       ------------------------------------------------------------------------*/
       memscpy((void*)&(event_info.disable_offload_fc),
               sizeof(event_info.disable_offload_fc),
               event_data_ptr,
               sizeof(event_info.disable_offload_fc));
       break;
      case IFACE_DPD_PING_INFO_EV:
       if (event_data_ptr == NULL)
       {
         PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
         LOG_MSG_ERROR_1("NULL event data ptr %d", event);
         DS_UTILS_ASSERT(0);
         return;
       }

        PS_SYSTEM_HEAP_MEM_ALLOC(event_info.dpd_ping_stat_info, 
                                 sizeof(ps_iface_dpd_ping_stat_info_type),
                                 ps_iface_dpd_ping_stat_info_type*);
        memset(event_info.dpd_ping_stat_info, 0, sizeof(ps_iface_dpd_ping_stat_info_type));
       /*------------------------------------------------------------------------
         Copy the Payload into the local variable
       ------------------------------------------------------------------------*/
        event_info.dpd_ping_stat_info->num_pings = 
                                              ((ps_iface_dpd_ping_stat_info_type*)event_data_ptr)->num_pings;
        event_info.dpd_ping_stat_info->pkts_loss = 
                                              ((ps_iface_dpd_ping_stat_info_type*)event_data_ptr)->pkts_loss;
        event_info.dpd_ping_stat_info->avg_rtt = 
                                              ((ps_iface_dpd_ping_stat_info_type*)event_data_ptr)->avg_rtt;
        event_info.dpd_ping_stat_info->user_data_ptr = 
                                              ((ps_iface_dpd_ping_stat_info_type*)event_data_ptr)->user_data_ptr;
        memscpy(&(event_info.dpd_ping_stat_info->rtt),
                sizeof(uint16)*(event_info.dpd_ping_stat_info->num_pings),
                &(((ps_iface_dpd_ping_stat_info_type*)event_data_ptr)->rtt),
                sizeof(uint16)*(((ps_iface_dpd_ping_stat_info_type*)event_data_ptr)->num_pings));
       break;

    case IFACE_NET_CLOSE_IN_PROGRESS_EV:
        break;

    default:
      PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
      LOG_MSG_INFO2_3("ps_iface_generic_ind(): "
                      "Invalid event %d on iface 0x%x:%d",
                      event, this_iface_ptr->name, this_iface_ptr->instance);
      DS_UTILS_ASSERT(0);
      return;
  }

  ps_ifacei_invoke_event_cbacks(this_iface_ptr, NULL, event, event_info);

  if(event == IFACE_DPD_PING_INFO_EV && event_info.dpd_ping_stat_info != NULL)
  {
    PS_SYSTEM_HEAP_MEM_FREE(event_info.dpd_ping_stat_info);
  }

  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

} /* ps_iface_generic_ind() */



#ifdef FEATURE_DATA_PS_QOS
/*===========================================================================
FUNCTION PS_IFACE_CHANGE_PRI_PHYS_LINK()

DESCRIPTION
  This function is called to indicate that the primary phys link associated
  with the iface has changed.  The new primary phys link should be specified.

  THIS FUNCTION SHOULD NOT BE CALLED IN A ISR.

PARAMETERS
  iface_ptr: ptr to interface control block on which to operate on
  phys_link_ptr:  New primary phys link.
  info_code:      Extended info code indicating the reason of the primary
                  change.

RETURN VALUE
  None

DEPENDENCIES
  phys_link_ptr must be associatd with this iface.

SIDE EFFECTS
  Clients implcitly accessing the primary phys link through the iface ptr
  will obtain the new phys link.

  All the flows that are bound to old primary are migrated to new primary,
  and vice versa

  Capabilities of old and new primary phys links are swapped
===========================================================================*/
void ps_iface_change_pri_phys_link
(
  ps_iface_type                    * iface_ptr,
  ps_phys_link_type                * new_pri_phys_link_ptr,
  ps_extended_info_code_enum_type    info_code
)
{
  ps_phys_link_type           * curr_pri_phys_link_ptr;
  ps_flow_type                * flow_ptr;
  ps_iface_event_info_u_type    event_info;
  uint8                         tmp_val;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

  if (!PS_IFACE_IS_VALID(iface_ptr) ||
      !PS_PHYS_LINK_IS_VALID(new_pri_phys_link_ptr))
  {
    LOG_MSG_ERROR_2("ps_iface_change_pri_phys_link(): "
                    "Invalid iface 0x%p or phys link 0x%p",
                    iface_ptr, new_pri_phys_link_ptr);
    PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
    return;
  }

  LOG_MSG_INFO2_3("ps_iface_change_pri_phys_link(): "
                  "IFACE PRI PHYS LINK CHANGE IND 0x%x:%d, state %d",
                  iface_ptr->name,
                  iface_ptr->instance,
                  PS_IFACEI_GET_STATE(iface_ptr));

  curr_pri_phys_link_ptr = PS_IFACEI_GET_PHYS_LINK(iface_ptr);

  if (curr_pri_phys_link_ptr == NULL)
  {
    PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
    LOG_MSG_ERROR_1("ps_iface_change_pri_phys_link : "
                    "Invalid phys_link pointer 0x%p ", curr_pri_phys_link_ptr);
    DS_UTILS_ASSERT(0);
    return;
  }

  /*-------------------------------------------------------------------------
    Make sure the specified new primary phys link is associated with this
    iface.
  -------------------------------------------------------------------------*/
  if (PS_IFACEI_IS_PHYS_LINK_VALID(iface_ptr, new_pri_phys_link_ptr))
  {
    iface_ptr->iface_private.phys_link.primary =
      PS_PHYS_LINKI_GET_INST(new_pri_phys_link_ptr);
  }
  else
  {
    PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
    LOG_MSG_ERROR_2("ps_iface_change_pri_phys_link(): "
                    "Iface 0x%p, no associated phys link 0x%p",
                    iface_ptr, new_pri_phys_link_ptr);
    DS_UTILS_ASSERT(0);
    return;
  }

  /*-------------------------------------------------------------------------
    Migrate all the flows, that are bound to old primary, to new primary and
    vice versa. Bind default flow to new primary
  -------------------------------------------------------------------------*/
  (void) ps_flowi_unbind_phys_link(PS_IFACEI_GET_DEFAULT_FLOW(iface_ptr));

  flow_ptr = list_peek_front(&(iface_ptr->iface_private.flow.sec_flow_list));
  while (flow_ptr != NULL)
  {
    if (PS_FLOWI_GET_PHYS_LINK(flow_ptr) == new_pri_phys_link_ptr)
    {
      (void) ps_flowi_rebind_phys_link(flow_ptr, curr_pri_phys_link_ptr);
    }
    else if (PS_FLOWI_GET_PHYS_LINK(flow_ptr) == curr_pri_phys_link_ptr)
    {
      (void) ps_flowi_rebind_phys_link(flow_ptr, new_pri_phys_link_ptr);
    }

    flow_ptr = list_peek_next(&iface_ptr->iface_private.flow.sec_flow_list,
                              &flow_ptr->link);
  }

  (void) ps_flowi_bind_phys_link(PS_IFACEI_GET_DEFAULT_FLOW(iface_ptr),
                                 new_pri_phys_link_ptr);

  /*-------------------------------------------------------------------------
    Swap capability of old and new primary phys links
  -------------------------------------------------------------------------*/
  tmp_val = curr_pri_phys_link_ptr->phys_private.capability_mask;
  curr_pri_phys_link_ptr->phys_private.capability_mask =
    new_pri_phys_link_ptr->phys_private.capability_mask;
  new_pri_phys_link_ptr->phys_private.capability_mask = tmp_val;

  /*-------------------------------------------------------------------------
    Call the event callback passing the ptr to previous pri phys link
  -------------------------------------------------------------------------*/
  event_info.pri_changed_info.pri_phys_link_ptr = curr_pri_phys_link_ptr;
  event_info.pri_changed_info.info_code         = info_code;

  LOG_MSG_INFO1_3("ps_iface_change_pri_phys_link(): "
                  "If 0x%p, changed pri PL from 0x%p to 0x%p",
                  iface_ptr,
                  event_info.pri_changed_info.pri_phys_link_ptr,
                  new_pri_phys_link_ptr);

  ps_ifacei_invoke_event_cbacks(iface_ptr,
                                NULL,
                                IFACE_PRI_PHYS_LINK_CHANGED_EV,
                                event_info);
  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

} /* ps_iface_change_pri_phys_link() */
#endif /* FEATURE_DATA_PS_QOS */



/*===========================================================================
FUNCTION PS_IFACEI_DORM_STATUS

DESCRIPTION
  Called by the underlying phys link when the phys link wants to either
  orig from dormancy or is going dormant.  Calls functions registered by
  IFACE mgr to return information to phys link.  If no functions are
  registered, then returns TRUE (i.e, meaning go dormant or ok to orig
  from dorm).

PARAMETERS
  phys_link_ptr:  Ptr to phys link control block on which is asking the
                  question.
  action:         Either wants to orig or wants to go dormant.
  reason:         Applies only for orig's.  Indicates why we are originating.
  unique_mem_ptr: Most likely (and should be) set to to the iface_ptr.

RETURN VALUE
  - 1:  Do not go dormant or do not orig from dormancy.
    0:  Ok to go dormant or ok to orig from dormancy.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
int ps_ifacei_dorm_status
(
  ps_phys_link_type               *phys_link_ptr,
  ps_iface_dorm_action_enum_type   action,
  ps_iface_dorm_reason_enum_type   reason,
  void                            *unique_mem_ptr
)
{
  ps_iface_type     *this_iface_ptr;
  int                ret_val = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  this_iface_ptr = (ps_iface_type *)unique_mem_ptr;

  if (!PS_IFACE_IS_VALID(this_iface_ptr))
  {
    DS_UTILS_ERROR_FATAL("ps_ifacei_dorm_status(): Invalid ps_iface");
    return(-1);
  }

  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

  if ((PS_IFACEI_GET_STATE(this_iface_ptr) == IFACE_DOWN) ||
      (PS_IFACEI_GET_STATE(this_iface_ptr) == IFACE_DISABLED))
  {
    PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
    LOG_MSG_INFO1_1("ps_ifacei_dorm_status(): "
                    "Dorm status when iface 0x%p down or disabled",
                    this_iface_ptr);
    return(-1);
  }

  if (this_iface_ptr->dorm_action_f_ptr != NULL)
  {
    ret_val = this_iface_ptr->dorm_action_f_ptr(this_iface_ptr,
                                                phys_link_ptr,
                                                action,
                                                reason);
  }

  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

  return(ret_val);
} /* ps_ifacei_dorm_status() */



/*===========================================================================
FUNCTION PS_IFACEI_INVOKE_EVENT_CBACKS()

DESCRIPTION
  This function will invoke all of the event callbacks for a given interface
  and event.  It will also call the global callbacks for said event, if any.

PARAMETERS
  this_iface_ptr: ptr to the interface on which we are operating
  event: the event for which callbacks need to be called
  event_info: event information that needs to be passed into callback.

RETURN VALUE
  None

DEPENDENCIES
  Must be called in a iface critical section

SIDE EFFECTS
  None
===========================================================================*/
void ps_ifacei_invoke_event_cbacks
(
  ps_iface_type             *this_iface_ptr,
  ps_phys_link_type         *this_phys_link_ptr,
  ps_iface_event_enum_type   event,
  ps_iface_event_info_u_type event_info
)
{
  q_type                       * working_q_ptr        = NULL;
  q_type                       * working_global_q_ptr = NULL;
  void                         * event_buf_ptr;
  void                         * next_event_buf_ptr;
  ps_ifacei_event_handle_type  * ps_iface_event_handle;
  ps_phys_linki_event_handle_type * phys_link_event_handle;
  ps_iface_type                * logical_iface_ptr[1] = {0,};
  ps_iface_type                * phys_iface_ptr[1]    = {0,};
  uint8                          num_logical_iface    = 0;
  uint8                          num_iface            = 0;
  ps_iface_type                * root_iface_ptr       = NULL;
  ps_iface_type                * trat_iface_ptr       = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    If iface ptr is not NULL them validate iface ptr.
  -------------------------------------------------------------------------*/
  if (this_iface_ptr != NULL && !PS_IFACE_IS_VALID(this_iface_ptr))
  {
    DS_UTILS_ASSERT(0);
    return;
  }

  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

  /*-------------------------------------------------------------------------
    If physlink ptr is not NULL then validate phys link ptr - also make sure
    the event is in the valid range.
  -------------------------------------------------------------------------*/
  if (this_phys_link_ptr != NULL &&
        (!PS_PHYS_LINK_IS_VALID(this_phys_link_ptr) ||
        (event >= PHYS_LINK_MAX_EV && event < PHYS_LINK_EX_MIN_EV) || 
         event >= PHYS_LINK_EX_MAX_EV))
  {
    PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
    return;
  }

  /*-------------------------------------------------------------------------
    Validate if we need to invoke callbacks. If iface or phys_link is already
    in the same state as the occuring event, then do not invoke callbacks.
  -------------------------------------------------------------------------*/
  if (FALSE == ps_ifacei_validate_event_cbacks(this_iface_ptr,
                                               this_phys_link_ptr,
                                               event,
                                               event_info))
  {
    LOG_MSG_INFO2_1("ps_ifacei_invoke_event_cbacks(): "
                    "Not invoking event %d ", event);
    PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
    return;
  }
  
  /* --------------------------------------------------------------------------------
     (CR 2674890) Don't invoke Prefix Update Event if Handoff is in progress and 
     prefix kind is PREFIX_REMOVED.For other cases when Prefix is updated or added , 
     event should be invoked. 
     In case where handoff is success and prefix got updated while handover , we were 
     not posting prefix update indication. With this CR , we will be posting the 
     indication unless the prefix kind is not PREFIX_REMOVED.
   ---------------------------------------------------------------------------------*/

  if(event == IFACE_PREFIX_UPDATE_EV && event_info.prefix_info.kind == PREFIX_REMOVED )  
  {    
    /*get the logical iface*/      
    num_iface = 1;      
    phys_iface_ptr[0] = this_iface_ptr; 
			
    if ( ps_iface_get_root_iface( logical_iface_ptr,
				  &num_logical_iface,                                    
				  phys_iface_ptr,                                    
				  num_iface ) != 0 )      
    {        
      /* failed, the iface passed is a logical iface*/        
      root_iface_ptr = this_iface_ptr;      
    }      
    else      
    { 
      root_iface_ptr = logical_iface_ptr[0];	    
      /* found logical */        
      LOG_MSG_INFO1_4("ps_ifacei_invoke_event_cbacks(): "                    
                      "found logical iface [0x%x:%x] for physical iface [0x%x:%x]", 
                      root_iface_ptr->name, 
                      root_iface_ptr->instance, 
                      phys_iface_ptr[0]->name, 
                      phys_iface_ptr[0]->instance );               
    } 
    
    trat_iface_ptr = PS_IFACE_GET_TRAT_IFACE(root_iface_ptr);

    if(trat_iface_ptr != NULL && 		   
       root_iface_ptr->handoff_class != PS_IFACE_HANDOFF_CLASS_NONE &&		   
       root_iface_ptr->handoff_class != PS_IFACE_HANDOFF_CLASS_EPC_PREREG)	    
    {	    		  
      LOG_MSG_INFO1_3("ps_ifacei_invoke_event_cbacks(): "					
                      "SRAT brought down in middle of handoff."						
                      "Skipping IFACE_PREFIX_UPDATE_EV. TRAT iface [0x%x:%x],"
                      "handoff class %d",					
                      trat_iface_ptr->name, 
                      trat_iface_ptr->instance,
                      root_iface_ptr->handoff_class); 		
      PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
      return;	    
    }
  }

  /*-------------------------------------------------------------------------
    Start with the local callbacks
  -------------------------------------------------------------------------*/

  /* Trigger callbacks for new event handle */
  if (this_iface_ptr == NULL && this_phys_link_ptr == NULL)
  {
    working_q_ptr        = &global_iface_event_handle_q;
    working_global_q_ptr = &global_iface_event_handle_q;
  }
  else if (this_iface_ptr != NULL)
  {
    working_q_ptr = 
     &(this_iface_ptr->iface_private.event_handle_q);
    working_global_q_ptr = &global_iface_event_handle_q;
  }
  else if (this_phys_link_ptr != NULL)
  {
    /*-----------------------------------------------------------------------
      Phys link ptr will always be non-NULL if we reach here, the if is
      just to satisfy lint
    -----------------------------------------------------------------------*/
    working_q_ptr = 
     &(this_phys_link_ptr->phys_private.event_handle_q);
    working_global_q_ptr = &global_phys_link_event_handle_q;
  }

  for (;;)
  {
    /*-----------------------------------------------------------------------
      Traverse the queue of callbacks for this event
    -----------------------------------------------------------------------*/
    event_buf_ptr = q_check(working_q_ptr);
    while (event_buf_ptr != NULL)
    {
      next_event_buf_ptr =
        q_next(working_q_ptr, &((q_generic_item_type *) event_buf_ptr)->link);

      /*---------------------------------------------------------------------
        As both ps_iface and ps_phys_link use this function to call callback,
        cast event_buf_ptr to relevant type and post callback appropriately
      ---------------------------------------------------------------------*/
      if ((this_iface_ptr != NULL) ||
          ((this_iface_ptr == NULL) && (this_phys_link_ptr == NULL)))
      {
        ps_iface_event_handle = (ps_ifacei_event_handle_type *) event_buf_ptr;
        if (ps_ifacei_is_event_set(ps_iface_event_handle, event))
        {
          ps_iface_event_handle->event_cback_f_ptr
          (
            this_iface_ptr,
            event,
            event_info,
            ps_iface_event_handle->user_data_ptr
          );
        }
      }
      else
      {
        phys_link_event_handle =
          (ps_phys_linki_event_handle_type *) event_buf_ptr;
        if (ps_phys_linki_is_event_set(phys_link_event_handle, event))
        {
          phys_link_event_handle->event_cback_f_ptr
          (
            this_phys_link_ptr,
            event,
            event_info,
            phys_link_event_handle->user_data_ptr
          );
        }
      }

      event_buf_ptr = next_event_buf_ptr;
    } /* while (items in queue) */

    /*-----------------------------------------------------------------------
      If the working_q_ptr is set to global_iface_event_handle_q then we are done,
      otherwise we need to make one more pass through global event q.
    -----------------------------------------------------------------------*/
    if (working_q_ptr == working_global_q_ptr)
    {
      break;
    }
    else
    {
      working_q_ptr = working_global_q_ptr;
    }
  } /* for (all specific and global callbacks) */

  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

} /* ps_ifacei_invoke_event_cbacks() */
