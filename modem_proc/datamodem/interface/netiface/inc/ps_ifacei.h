#ifndef PS_IFACEI_H
#define PS_IFACEI_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                             P S _ I F A C E I . H

DESCRIPTION
  Header file defining all of the internal data types

DEPENDENCIES
  None of these functions should EVER be called from Interrupt context!

Copyright (c) 2008-2015 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                            EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/netiface/inc/ps_ifacei.h#2 $
  $DateTime: 2019/10/22 02:53:17 $

  when      who    what, where, why
--------    ---    ----------------------------------------------------------
08/20/15    gk     Added support for low latency traffic status indication
11/09/14    yl     IP data path re-architecture
11/11/13    rk     Removed feature mobileap.
04/13/12    pgm    Doing flow ctrl check on Phys link only for non-NAT ifaces 
                   as NAT ifaces dont have phys link and flow ctrl check fails
                   with NULL phys link.
08/16/09    pp     ps_iface_fill_next_hop_addr API introduced.
12/14/08    pp     Created module as part of Common Modem Interface:
                   Public/Private API split.
===========================================================================*/


/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"

#include "ps_iface.h"
#include "ps_crit_sect.h"
#include "ps_iface_defs.h"
#include "ps_svc.h"
#ifdef FEATURE_DATA_RM_NET
#include "ps_tx_meta_info.h"
#endif /* FEATURE_DATA_RM_NET */
#include "ds_Utils_DebugMsg.h"
#ifdef FEATURE_DATA_REFLECTIVE_QOS
#include "ps_flowi_event.h"
#endif /* FEATURE_DATA_REFLECTIVE_QOS */

/*---------------------------------------------------------------------------
  Tuning the number of ps iface ip filter buffers needed by this module
---------------------------------------------------------------------------*/
#define PS_IFACE_IPFLTR_BUF_SIZE ((sizeof(ps_iface_ipfilteri_type) + 3) & ~3)

#ifdef FEATURE_DATA_PS_LOW_MEM_CHIPSET
  #define PS_IFACE_IPFLTR_BUF_NUM       20
  #define PS_IFACE_IPFLTR_BUF_HIGH_WM   16
  #define PS_IFACE_IPFLTR_BUF_LOW_WM     5

#else
  #define PS_IFACE_IPFLTR_BUF_NUM       160
  #define PS_IFACE_IPFLTR_BUF_HIGH_WM   120
  #define PS_IFACE_IPFLTR_BUF_LOW_WM    30

#endif /* FEATURE_DATA_PS_LOW_MEM_CHIPSET */

#include <timetick.h>

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================

                         INTERNAL DATA DECLARATIONS

===========================================================================*/

/*---------------------------------------------------------------------------
  The PS iface Authentication credentials type
---------------------------------------------------------------------------*/
#define PS_AUTH_MAX_USER_ID_LEN   (255)
#define PS_AUTH_MAX_PASSWORD_LEN  (255)
typedef struct
{
  uint8  user_id_len;
  char   user_id[PS_AUTH_MAX_USER_ID_LEN];
  uint8  password_len;
  char   password[PS_AUTH_MAX_PASSWORD_LEN];
} ps_iface_auth_credential_type;

typedef struct
{
  ps_iface_auth_alg_pref_bitmask_e_type  alg_pref;
  ps_iface_auth_credential_type          auth_creds;
  void *                                 mode_spec_auth_info_ptr;
} ps_iface_auth_info_type;

/*---------------------------------------------------------------------------
  Enum describing IFACE error codes
---------------------------------------------------------------------------*/

typedef enum
{
  PS_IFACE_SUCCESS                    = 0x0,

  PS_IFACE_ERROR_HANDLE_ALLOC_FAILED  = 1,
  PS_IFACE_ERROR_EVENT_REG_FAILED     = 2,
  PS_IFACE_ERROR_IFACE_INVALID        = 3,
  PS_IFACE_ERROR_IFACE_NOT_LOGICAL    = 4,
  PS_IFACE_ERROR_IFACE_NOT_ASSOCIATED = 5,
  PS_IFACE_ERROR_INVALID_IFACE_STATE  = 6,
  PS_IFACE_ERROR_BRING_UP_CMD_FAILED  = 7,
  PS_IFACE_ERROR_VERSION_MISMATCH                         = 8,
  PS_IFACE_ERROR_INVALID_ARG                              = 9,
  PS_IFACE_ERROR_IFACE_HARDWARE_INCAPBALE                 = 10,
  PS_IFACE_ERROR_IFACE_NOT_BRIDGED                        = 11,
  PS_IFACE_ERROR_V6_ADDRESS_NOT_EXTERNAL                  = 12,
  PS_IFACE_ERROR_INVALID_ERRNO                            = 13,
  PS_IFACE_ERROR_INVALID_IPFLTR_CLIENT                    = 14,
  PS_IFACE_ERROR_INVALID_IPFLTR_HANDLE                    = 15,
  
  PS_IFACE_ERROR_OUT_OF_FILTER_SPACE                      = 100,
  PS_IFACE_ERROR_INVALID_FILTER_PARAM                     = 101,
  PS_IFACE_ERROR_OPT_FILTER_ERR                           = 102,
  PS_IFACE_ERROR_FILTER_NOT_FOUND                         = 103,
  
  PS_IFACE_ERROR_HW_ERR                                   = 200,
  PS_IFACE_ERROR_HW_FILTER_ERR                            = 201,
  PS_IFACE_ERROR_HW_FILTER_IFACE_CAPABLILITY_FALSE        = 202,
  PS_IFACE_ERROR_HW_OP_NOT_SUPPORTED                      = 203,
  
  PS_IFACE_ERROR_MAX
} ps_iface_error_enum_type;

/*===========================================================================

                         EXTERNAL MACROS

===========================================================================*/
#define PS_IFACE_GET_INPUT_F_PTR(ps_iface_ptr)                           \
  (PS_IFACE_GET_BASE_IFACE(ps_iface_ptr)->iface_private.ip_input)

#define PS_IFACE_GET_TIMETICK_MS() (uint32)timetick_get_ms()

/*===========================================================================

                         EXTERNAL FUNCTIONS

===========================================================================*/
/*===========================================================================
FUNCTION PS_IFACE_INIT()

DESCRIPTION
  Initializes ps_iface module

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ps_iface_init
(
  void
);

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
);

/*===========================================================================
FUNCTION PS_IFACE_GET_REF_CNT()

DESCRIPTION
  This macro returns the interface reference count

PARAMETERS
  iface_ptr: pointer to the interface in question.

RETURN VALUE
  the reference count for the passed in interface
===========================================================================*/
#define PS_IFACE_GET_REF_CNT  ps_iface_get_ref_cnt
INLINE uint32 ps_iface_get_ref_cnt
(
  ps_iface_type *iface_ptr
)
{
  uint32  ref_cnt = 0;
  uint8   index   = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (PS_IFACE_IS_VALID(iface_ptr))
  {
    PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

    for (index = 0; index < PS_POLICY_MGR_IFACE_PRIORITY_MAX; index++)
    {
      ref_cnt += iface_ptr->iface_private.ref_cnt[index];
    }

    PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);
  }

  return ref_cnt;

} /*  ps_iface_get_ref_cnt() */

/*===========================================================================
FUNCTION PS_IFACE_RESET_REF_CNT()

DESCRIPTION
  This macro resets the interface reference count to 0

PARAMETERS
  iface_ptr: pointer to the interface in question.

RETURN VALUE
  None
===========================================================================*/
#define PS_IFACE_RESET_REF_CNT  ps_iface_reset_ref_cnt
INLINE void ps_iface_reset_ref_cnt
(
  ps_iface_type *iface_ptr
)
{
  uint8   index   = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (PS_IFACE_IS_VALID(iface_ptr))
  {
    PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

    for (index = 0; index < PS_POLICY_MGR_IFACE_PRIORITY_MAX; index++)
    {
      iface_ptr->iface_private.ref_cnt[index] = 0;
      iface_ptr->iface_private.priority_mask = 0;
    }

    PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);
  }
} /*  ps_iface_reset_ref_cnt() */

/*===========================================================================
FUNCTION PS_IFACE_GET_APP_PRIORITY

DESCRIPTION
  Returns the first non-zero app priority field stored in an iface.

PARAMETERS
  iface_ptr - Pointer to ps iface control block.

RETURN VALUE
  app_priority stored in physical ifaces. In case of error, -1 is returned.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
int32 ps_iface_get_app_priority
(
  ps_iface_type    *iface_ptr
);


/** 
  @def   PS_IFACE_GET_PDN_TEARDOWN_LOCAL_ABORT_CONFIG(iface_ptr)
  @brief This macro will get the value of local abort flag
  
  @param
    iface_ptr[in]    pointer to iface
 
  @return
    pdn teardown config value

*/
#define PS_IFACE_GET_PDN_TEARDOWN_LOCAL_ABORT_CONFIG ps_iface_get_pdn_teardown_local_abort_config
INLINE boolean ps_iface_get_pdn_teardown_local_abort_config
(
  ps_iface_type *iface_ptr
)
{
  ps_iface_type * base_iface_ptr;
  
  if(PS_IFACE_IS_VALID(iface_ptr))
  {
    base_iface_ptr = PS_IFACE_GET_BASE_IFACE(iface_ptr);
    if (PS_IFACE_IS_VALID(base_iface_ptr))
    {
      return base_iface_ptr->iface_private.pdn_teardown_config.local_abort;
    }
  }
  
  return FALSE;
} /*  ps_iface_get_pdn_teardown_config() */

/** 
  @def   PS_IFACEI_INC_COOKIE(iface_ptr)
  @brief Increments the cookie of given iface 
 
  This method Increments the cookie of given iface
  
  @param
    iface_ptr[in]    pointer to iface
 
  @return
    cookie value after increment

*/
#define PS_IFACEI_INC_COOKIE( iface_ptr )     \
  if (PS_IFACE_IS_VALID(iface_ptr))           \
  {                                           \
    iface_ptr->iface_private.cookie++;        \
  }                
    
/**
  @brief  This macro returns whether the first packet drop in powersave mode is 
  reported or not.
  
  @return  TRUE      If Enabled.
           FALSE     If disabled.
  
  @code
  @endcode
*/
#define PS_IFACE_IS_FIRST_POWERSAVE_PACKET_DROP_REPORTED                       \
    ps_iface_is_first_powersave_packet_drop_reported
  
boolean ps_iface_is_first_powersave_packet_drop_reported
(
  void
);
  
/**
  @brief  This macro sets the powersave mode first packet drop reported setting for
  a given iface
  
  @param[in] report - TRUE if reported already
                      FALSE If not reported 

  @return  TRUE      If Enabled.
           FALSE     If disabled.

  @code
  @endcode
*/
#define PS_IFACE_SET_POWERSAVE_FIRST_PACKET_DROP_REPORTED(report)       \
        ps_iface_set_first_powersave_packet_drop_reported(report)

void ps_iface_set_first_powersave_packet_drop_reported
(
  boolean reported
);
    
/**
  @brief  This macro Returns ip_filters powersave mode configuration
  
  @param[in] iface_ptr - Pointer to iface control block

  @return    TRUE        If Enabled.
             FALSE       If disabled.

  @code
  @endcode
*/
#define PS_IFACE_GET_POWERSAVE_FILTERING_MODE(iface_ptr)                         \
  ( PS_IFACE_IS_VALID(iface_ptr)           \
      ?iface_ptr->iface_private.powersave_filtering_settings.enable_filter_mode  \
      :FALSE)        

/**
  @brief  This macro returns ip_filters power save AUTO EXIT mode configuration
  
  @param[in] iface_ptr - Pointer to iface control block
  
  @return    TRUE        If Enabled.
             FALSE       If disabled.
  
  @code
  @endcode
*/
#define PS_IFACE_GET_POWERSAVE_FILTERING_AUTO_EXIT_MODE(iface_ptr)                   \
  ( PS_IFACE_IS_VALID(iface_ptr)                                                     \
      ?iface_ptr->iface_private.powersave_filtering_settings.enable_auto_exit_mode   \
      :FALSE)

/**
  @brief  This macro calls ioctl to enable/disable powersave mode
  
  @param[in]  iface_ptr - Pointer to iface control block
  @param[in]  enable    - TRUE for Enable
                          FALSE for Disable
  @param[out] ps_errno    Failure reason.
                          DS_EFAULT : Invalid arguments
                          DS_NOMEMORY : Out of memory
                          DS_EINVAL : Invalid Operation
  
  @code
  @endcode
*/
#define PS_IFACE_POWERSAVE_FILTERING_AUTO_EXIT_MODE_HDLR(iface_ptr, enable, ps_errno) \
  if( PS_IFACE_IS_VALID(iface_ptr))                                                   \
  {                                                                                   \
    if( -1 == ps_iface_ioctl( iface_ptr,                                              \
                              PS_IFACE_IOCTL_SET_POWERSAVE_FILTERING_MODE,            \
                              &enable,                                                \
                                 &ps_errno) )                                         \
    {                                                                                 \
      LOG_MSG_ERROR_1("Failed to set poser_save_ipfilters_mode errno%d",ps_errno);    \
    }                                                                                 \
  }                                                                                  

/**
  @brief  This macro resets the powersave filtering mode setting of a given
          iface to FALSE

  @param[in]  this_iface_ptr - Pointer to iface control block

  @code
  @endcode
*/
#define PS_IFACEI_RESET_POWERSAVE_FILTERING_MODE( this_iface_ptr )                    \
 (this_iface_ptr)->iface_private.powersave_filtering_settings.enable_filter_mode      \
   = FALSE;

/**
  @brief  This macro resets the powersave filtering auto exit mode setting of a
          given iface to FALSE

  @param[in]  this_iface_ptr - Pointer to iface control block

  @code
  @endcode
*/
#define PS_IFACEI_RESET_POWERSAVE_FILTERING_AUTO_EXIT_MODE( this_iface_ptr )          \
 (this_iface_ptr)->iface_private.powersave_filtering_settings.enable_auto_exit_mode   \
   = FALSE;


/*===========================================================================
MACRO PS_IFACE_COMMON_IS_FLOW_ENABLED()

DESCRIPTION
  This macro returns whether all of iface, flow, and phys_link in question
  are flow enabled.

PARAMETERS
  iface_ptr : pointer to the interface
  flow_ptr  : ptr to a flow

RETURN VALUE
  TRUE  : if flow is enabled on all the entities
  FALSE : otherwise
===========================================================================*/
#define PS_IFACE_COMMON_IS_FLOW_ENABLED  ps_iface_common_is_flow_enabled
INLINE boolean ps_iface_common_is_flow_enabled
(
  ps_iface_type  * ps_iface_ptr,
  ps_flow_type   * ps_flow_ptr
)
{
  ps_phys_link_type  * phys_link_ptr;
  boolean              ret_val = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

  do
  {
    if (!PS_IFACE_IS_VALID(ps_iface_ptr) || !PS_FLOW_IS_VALID(ps_flow_ptr))
    {
      break;
    }

    phys_link_ptr = PS_FLOWI_GET_PHYS_LINK(ps_flow_ptr);
    if (NULL != phys_link_ptr)
    {
      ret_val = (PS_IFACEI_FLOW_ENABLED(ps_iface_ptr) &&
                 PS_FLOWI_IS_TX_ENABLED(ps_flow_ptr) &&
                 PS_PHYS_LINK_FLOW_ENABLED(phys_link_ptr));
    }
    else
    {
      ret_val = (PS_IFACEI_FLOW_ENABLED(ps_iface_ptr) &&
                 PS_FLOWI_IS_TX_ENABLED(ps_flow_ptr));
    }
  } while (0);

  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

  return ret_val;
} /* ps_iface_common_is_flow_enabled() */

/*===========================================================================
MACRO PS_IFACE_COMMON_DEFAULT_IS_FLOW_ENABLED()

DESCRIPTION
  This macro returns whether all of iface, its default flow, and its primary
  phys_link are flow enabled.

PARAMETERS
  iface_ptr : pointer to the interface

RETURN VALUE
  TRUE  : if flow is enabled on all the entities
  FALSE : otherwise
===========================================================================*/
#define PS_IFACE_COMMON_DEFAULT_IS_FLOW_ENABLED                          \
  ps_iface_common_default_is_flow_enabled
INLINE boolean ps_iface_common_default_is_flow_enabled
(
  ps_iface_type  * ps_iface_ptr
)
{
  ps_flow_type       * ps_flow_ptr = NULL;
  ps_phys_link_type  * phys_link_ptr = NULL;
  boolean              ret_val = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

  do
  {
    if (!PS_IFACE_IS_VALID(ps_iface_ptr))
    {
      break;
    }

    ps_flow_ptr   = PS_IFACEI_GET_DEFAULT_FLOW(ps_iface_ptr);
    phys_link_ptr = PS_IFACEI_GET_PHYS_LINK(ps_iface_ptr);

    ret_val =  (PS_IFACE_FLOW_ENABLED(ps_iface_ptr) &&
                PS_FLOW_IS_TX_ENABLED(ps_flow_ptr) &&
                PS_PHYS_LINK_FLOW_ENABLED(phys_link_ptr));
  } while (0);

  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

  return ret_val;
} /* ps_iface_common_default_is_flow_enabled() */

#ifdef FEATURE_DATA_REFLECTIVE_QOS
/*===========================================================================
FUNCTION PS_IFACE_GET_FLOW_FROM_PHYSLINK()

DESCRIPTION
  This macro returns flow matching the physlink in the iface

PARAMETERS
  iface_ptr     : pointer to the interface.
  phys_link_ptr : pointer to physlink

RETURN VALUE
  Value of the is_active variable
===========================================================================*/
#define PS_IFACE_GET_FLOW_FROM_PHYSLINK ps_iface_get_flow_from_physlink
INLINE ps_flow_type * ps_iface_get_flow_from_physlink
(
  ps_iface_type           * ps_iface_ptr,
  ps_phys_link_type       * phys_link_ptr
)
{
  ps_flow_type  * ps_flow_ptr = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /*-------------------------------------------------------------------------
    Algorithm
      1. Go through all secondary flow on the iface.
      2. For each flow, retrieve the physlink and compare against the physlink
         passed in function argument.
      3. If physlink match then return the flow ptr
  -------------------------------------------------------------------------*/
  ps_flow_ptr =
    (ps_flow_type *)list_peek_front(&(ps_iface_ptr->iface_private.flow.sec_flow_list));

  while (ps_flow_ptr != NULL)
  {
    if (PS_FLOW_GET_PHYS_LINK(ps_flow_ptr) == phys_link_ptr)
    {
      break;
    }
    else
    {
      ps_flow_ptr =
        (ps_flow_type *)list_peek_next(&(ps_iface_ptr->iface_private.flow.sec_flow_list),
                       &(ps_flow_ptr->link));
    }
  }
  
  return ps_flow_ptr;
} /* ps_iface_get_flow_from_physlink() */ 

/*===========================================================================
MACRO PS_IFACEI_INSTALL_REFLECTIVE_QOS_FILTERS()

DESCRIPTION
  This macro installs filters to 

PARAMETERS
  iface_ptr : pointer to the interface
  flow_ptr  : ptr to a flow

RETURN VALUE
  TRUE  : if flow is enabled on all the entities
  FALSE : otherwise
===========================================================================*/
#define PS_IFACEI_INSTALL_REFLECTIVE_QOS_FILTERS  ps_ifacei_install_reflective_qos_fltr
INLINE ps_iface_ipfltr_handle_type ps_ifacei_install_reflective_qos_fltr
(
  ps_iface_type                         * iface_ptr,
  ps_iface_reflective_qos_filter_type   * reflective_qos_fltr
)
{
  ps_iface_ipfltr_add_param_type  fltr_add_param;
  ps_iface_ipfltr_handle_type     fltr_add_handle = PS_IFACE_IPFLTR_INVALID_HANDLE;  
  ip_filter_type                  tx_fltr;
  int16                           ps_errno;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  do
  {
    memset(&fltr_add_param, 0, sizeof(ps_iface_ipfltr_add_param_type));
    memset(&tx_fltr, 0, sizeof(tx_fltr));
    tx_fltr.ip_vsn = reflective_qos_fltr->ip_vsn; 
    if (IP_V4 == tx_fltr.ip_vsn)
    {
      tx_fltr.ip_hdr.v4.field_mask = IPFLTR_MASK_IP4_NEXT_HDR_PROT;
      tx_fltr.ip_hdr.v4.next_hdr_prot = reflective_qos_fltr->next_hdr_protocol;
    }
    else
    {
      tx_fltr.ip_hdr.v6.field_mask = IPFLTR_MASK_IP6_NEXT_HDR_PROT;
      tx_fltr.ip_hdr.v6.next_hdr_prot = reflective_qos_fltr->next_hdr_protocol;
    }
    
    if (PS_IPPROTO_UDP == reflective_qos_fltr->next_hdr_protocol)
    {
      tx_fltr.next_prot_hdr.udp.field_mask = 
        IPFLTR_MASK_UDP_DST_PORT;
      tx_fltr.next_prot_hdr.udp.dst.port = 
        reflective_qos_fltr->dst_port;
    }
    else if (PS_IPPROTO_TCP == reflective_qos_fltr->next_hdr_protocol)
    {
      tx_fltr.next_prot_hdr.tcp.field_mask = 
        IPFLTR_MASK_TCP_DST_PORT;
      tx_fltr.next_prot_hdr.tcp.dst.port = 
        reflective_qos_fltr->dst_port;      
    }
    else if(PS_IPPROTO_TCP_UDP == reflective_qos_fltr->next_hdr_protocol)
    {
      tx_fltr.next_prot_hdr.tcp_udp_port_range.field_mask = 
        IPFLTR_MASK_TCP_UDP_DST_PORT;
      tx_fltr.next_prot_hdr.tcp_udp_port_range.dst.port = 
        reflective_qos_fltr->dst_port;      
    }

    fltr_add_param.filter_type        = IPFLTR_DEFAULT_TYPE;
    fltr_add_param.enable             = TRUE;
    fltr_add_param.is_validated       = FALSE;

    fltr_add_param.fi_result   = 
      (ps_iface_ipfltr_result_type)reflective_qos_fltr->flow_ptr;
    fltr_add_param.fi_ptr_arr  = &tx_fltr;
    fltr_add_param.num_filters = 1;      
    fltr_add_handle = ps_iface_ipfltr_add(iface_ptr,
                                         IP_FLTR_CLIENT_QOS_OUTPUT,
                                         &fltr_add_param,
                                         &ps_errno);   
    if (PS_IFACE_IPFLTR_INVALID_HANDLE == fltr_add_handle)
    {
      LOG_MSG_ERROR_0("ps_ifacei_add_reflective_qos_filters : "
                      "Filter add failed");     
      break;   
    } 
  } while(0);

  return fltr_add_handle;
} /* ps_ifacei_install_reflective_qos_fltr */

/*===========================================================================
MACRO PS_IFACEI_ADD_REFLECTIVE_QOS_FILTERS()

DESCRIPTION
  This macro adds reflective qos filters to iface

PARAMETERS
  iface_ptr : pointer to the interface
  flow_ptr  : ptr to a flow

RETURN VALUE
  TRUE  : if flow is enabled on all the entities
  FALSE : otherwise
===========================================================================*/
#define PS_IFACEI_ADD_REFLECTIVE_QOS_FILTERS  ps_ifacei_add_reflective_qos_filters
INLINE uint8 ps_ifacei_add_reflective_qos_filters
(
  ps_iface_type                         * iface_ptr,
  ps_iface_reflective_qos_filter_type   * reflective_qos_fltr
)
{
  boolean                         flow_found = FALSE;
  uint8                           ret_val    = 0;
  ps_iface_event_info_u_type      event_info;
  ps_iface_ipfltr_handle_type     fltr_add_handle = PS_IFACE_IPFLTR_INVALID_HANDLE; 
  ps_iface_ipfilteri_type       * filter_buf_ptr;
  q_type                        * q_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /*-----------------------------------------------------------------------
    1. Go through all QOS filters to check if any filter exists with
       fltr result as ps_flow_ptr
    2. If it exist then return
    3. Add new qos filter with protocol and src port
  -----------------------------------------------------------------------*/
  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

  do
  {
    if (!PS_IFACE_IS_VALID(iface_ptr))
    {
      break;
    }

    q_ptr = 
      &(iface_ptr->iface_private.ipfltr_info[IP_FLTR_CLIENT_QOS_OUTPUT]);
      
    for (filter_buf_ptr = (ps_iface_ipfilteri_type *)q_check(q_ptr);
         filter_buf_ptr != NULL;
         filter_buf_ptr = (ps_iface_ipfilteri_type *)q_next(q_ptr, &(filter_buf_ptr->link)))
    {
      if (reflective_qos_fltr->flow_ptr == (ps_flow_type *)filter_buf_ptr->fi_result) 
      {
        flow_found = TRUE;      
        break;                                            
      }
    }
  
    if (flow_found == FALSE)
    {
      fltr_add_handle = ps_ifacei_install_reflective_qos_fltr(iface_ptr,
                                                              reflective_qos_fltr);
      if (PS_IFACE_IPFLTR_INVALID_HANDLE == fltr_add_handle)
      {
        LOG_MSG_ERROR_0("ps_ifacei_add_reflective_qos_filters : "
                        "Filter add failed");  
        ret_val = -1;   
        break;   
      } 
      PS_FLOWI_SET_TX_FLTR_HANDLE(reflective_qos_fltr->flow_ptr, fltr_add_handle);

      memset(&event_info, 0, sizeof(ps_iface_event_info_u_type));
      event_info.reflective_qos_fltr_info.ip_vsn = reflective_qos_fltr->ip_vsn;
      event_info.reflective_qos_fltr_info.next_hdr_protocol = 
        reflective_qos_fltr->next_hdr_protocol;
      event_info.reflective_qos_fltr_info.dst_port = reflective_qos_fltr->dst_port;
      
      ps_flowi_invoke_event_cbacks(reflective_qos_fltr->flow_ptr, 
                                   FLOW_REFLECTIVE_QOS_FILTER_UPDATED_EV, 
                                   event_info);
    }
    else
    {
      ret_val = -1;
    }
  } while (0);

  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

  return ret_val;
} /* ps_ifacei_add_reflective_qos_filters() */
#endif /* FEATURE_DATA_REFLECTIVE_QOS */

/*===========================================================================
FUNCTION PS_IFACEI_GET_SOURCE_IFACE()

DESCRIPTION
  This macro returns a ps_iface_ptr pointing to the source iface of the
  logical iface that is specified.

PARAMETERS
  iface_ptr: pointer to the interface in question.

RETURN VALUE
  Source ps_iface_ptr if passed in iface_ptr is a logical iface.
  Null: Otherwise.
===========================================================================*/
#define PS_IFACEI_GET_SOURCE_IFACE( ps_iface_ptr)                         \
  (PS_IFACE_IS_LOGICAL( ps_iface_ptr)                                     \
     ? (ps_iface_ptr)->iface_private.assoc_iface_ptr                      \
     : NULL)

/*===========================================================================
FUNCTION PS_IFACE_FLOW_CTRL_CMD_HANDLER()

DESCRIPTION
  This function processes PS_IFACE_FLOW_CTRL_CMD.

PARAMETERS
  ps_cmd       : Must be PS_IFACE_FLOW_CTRL_CMD.
  cmd_data_ptr : cmd data

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ps_iface_flow_ctrl_cmd_handler
(
  ps_cmd_enum_type    ps_cmd,
  void              * cmd_data_ptr
);

/*===========================================================================
FUNCTION PS_IFACE_GO_NULL_CMD_EX_HANDLER()

DESCRIPTION
  This function processes PS_IFACE_GO_NULL_CTRL_CMD.

PARAMETERS
  ps_cmd          : Must be PS_IFACE_GO_NULL_CTRL_CMD.
  ps_cmd_data_ptr : cmd data

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ps_iface_go_null_cmd_ex_handler
(
  ps_cmd_enum_type    ps_cmd,
  void              * ps_cmd_data_ptr
 );


/*===========================================================================
FUNCTION PS_IFACE_GO_NULL_CMD_EX()

DESCRIPTION
  This function is used to stop an iface. Its just work around called by 
  EPC framework only for issue where modehandler gets the bring up
  and tear down in different command.

PARAMETERS
  this_iface_prtr: ptr to interface control block on which to operate on.
  client_data_ptr: data, that is passed to the client handler function.

RETURN VALUE
  0: on success
 -1: on failure

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
int ps_iface_go_null_cmd_ex
(
  ps_iface_type *this_iface_ptr,
  void          *client_data_ptr
);

/**
  @brief     This function sets the low_latency timer handle
  
  @param[in] timer_hdl - Handle to the the timer defined for low latency
  
  @return    0        If set successfully.
            -1        If not set successfully.
  
  @code
  @endcode
*/
int ps_ifacei_set_low_latency_timer_handle
( 
  ps_iface_ipfltr_handle_type timer_hdl 
);

/**
  @brief     This function retrieves the low_latency timer handle
  
  @return    Handle for the low latency timer
  
  @code
  @endcode
*/
ps_iface_ipfltr_handle_type ps_ifacei_get_low_latency_timer_handle
(
  void
);

/**
  @brief     This function is invoked when the low latency traffic status timer
             is expired.
  
  @param[in] user_data_ptr - userdata passed when defining the low latency 
                             timer

  @return    Handle for the low latency timer
  
  @code
  @endcode
*/
void ps_ifacei_low_latency_timer_expiry_cb
(
  void * user_data_ptr
);

/**
  @brief     This callback function is called by IPA when low latency filters
             matched any packets(Uplink/Downlink). This starts/restarts the 
             low latency traffic status timer.
  
  @param[in] filter_hdl - Handle of the low latency hardware filter that 
                          matched a data packet

  @return    None
  
  @code
  @endcode
*/
void ps_iface_low_latency_traffic_status_cb
(
  uint32 filter_hdl
);

/**
  @brief      This function starts/restarts the low latency traffic status 
              timer and sends out indication for every ping request/response. 
              It also calculates time difference between two ping request and 
              sends the information in indication.
  
              Note : For first ping request, the default time interval as set 
                     in ps_iface_ping_pkt_default_interval_time_in_ms is sent 
                     out  .
      
  @param[in]  cmd            - command for which this handler is called
              user_data_ptr  - user data pointer passed
                     
  @return    None
      
  @code
  @endcode
*/

void ps_iface_low_latency_event_handler
( 
  ps_cmd_enum_type   cmd,
  void             * user_data_ptr
);

#ifdef FEATURE_DATA_RM_NET
/*===========================================================================
  FUNCTION PS_IFACE_FILL_NEXT_HOP_ADDR()

DESCRIPTION
  This function fills next hop address from either Pkt info or from iface
  pointer. Mainly used by Broadcast scenarios[Currently RmNet!]

  This function is introduced to avoid duplication in IFACE, IP Frag layers

DEPENDENCIES
  A valid Metainfo must be passed in which contain pkt info.
  A valid iface pointer must be passed in used in non-mcast scenarios.

PARAMETERS
  iface_ptr   - Iface pointer
  mi_ref_ptr  - Reference to meta info block
  errno       - Error value returned by the function

RETURN VALUE
  0 for successful processing of the pkt, -1 in case of error

  Currently this function is used only for RmNet cases - can be opened for
  all - if required!
===========================================================================*/
int ps_iface_fill_next_hop_addr
(
  ps_iface_type                        *iface_ptr,
  ps_tx_meta_info_type                 *meta_info_ptr,
  errno_enum_type                      *ps_errno
);
#endif /* FEATURE_DATA_RM_NET */

int ps_ifacei_default_tx_cmd
(
  ps_iface_type        *this_iface_ptr,
  dsm_item_type       **pkt_ref_ptr,
  ps_tx_meta_info_type *meta_info_ptr,
  void                 *tx_cmd_info
);

int ps_ifacei_logical_default_tx_cmd
(
  ps_iface_type         *this_iface_ptr,
  dsm_item_type        **pkt_ref_ptr,
  ps_tx_meta_info_type  *meta_info_ptr,
  void                  *tx_cmd_info
);

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
);


extern int ps_iface_arbitration_tear_down_cmd
(
  ps_iface_type *this_iface_ptr,
  int16         *ps_errno,
  void          *client_data_ptr
);

/*===========================================================================
FUNCTION ps_iface_log_qos_spec_params

DESCRIPTION
This function printf the qos spec params

PARAMETERS
None.

DEPENDENCIES
None.

RETURN VALUE
None.

SIDE EFFECTS
None.
===========================================================================*/
void ps_iface_log_qos_spec_params
(
  const qos_spec_type*         qos_spec
);

/*===========================================================================
FUNCTION ps_iface_log_qos_spec_filter_params

DESCRIPTION
This function prints the filter specifications

PARAMETERS
fltr_buf_ptr   - Pointer to PS QOS spec
filter_index   - Filter Index

DEPENDENCIES

RETURN VALUE
None.

SIDE EFFECTS
None.

===========================================================================*/
void ps_iface_log_qos_spec_filter_params
(
  const ip_filter_type    *fltr_buf_ptr,        /* Pointer to PS QOS spec   */
  uint8                    filter_index
);


/*===========================================================================
FUNCTION PS_IFACEI_COPY_SIP_SERV_ADDR()

DESCRIPTION
   Copyies sip serv address from one iface to another

PARAMETERS
  dst_iface_ptr   :  ptr to destination iface.
  src_iface_ptr   :  ptr to source iface.

RETURN VALUE
  0: copied successfully
 -1: failure

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
int ps_ifacei_copy_sip_serv_addr
(
  ps_iface_type  *dst_iface_ptr,
  ps_iface_type  *src_iface_ptr
);

/**
  @brief  This function is used to get the powersave mode of ipfilters on the
          given interface.

  @param[in]  this_iface_ptr:   ptr to interface control block to operate on.
  @param[out] powersave_mode:   powersave filtering mode for a given iface
  @param[out] ps_errno          Failure reason.
                                DS_EFAULT : Invalid arguments
                                DS_NOMEMORY : Out of memory
                                DS_EINVAL : Invalid Operation

  @return     -1                On Failure.
               0                On Success.

  @code
  @endcode
*/
int ps_iface_get_powersave_filtering_mode
(
  ps_iface_type *this_iface_ptr,
  boolean       * powersave_mode,
  sint15        *ps_errno
);

/**
  @brief  This function is used to set the powersave mode of ipfilters on the
          given interface.

  @param[in]  this_iface_ptr:   ptr to interface control block to operate on.
  @param[out] powersave_mode:   powersave filtering mode for a given iface
  @param[out] ps_errno          Failure reason.
                                DS_EFAULT : Invalid arguments
                                DS_NOMEMORY : Out of memory
                                DS_EINVAL : Invalid Operation

  @return     -1                On Failure.
               0                On Success.

  @code
  @endcode
*/
int ps_iface_set_powersave_filtering_mode
(
  ps_iface_type *this_iface_ptr,
  boolean         powersave_mode,
  sint15        *ps_errno
);

/**
  @brief  This function is used to set the auto exit mode of ipfilters 
          installed.

  @param[in]  this_iface_ptr:   ptr to interface control block to operate on.
  @param[out] auto_exit_mode:   powersave auto_exit_mode mode for a given iface
  @param[out] ps_errno          Failure reason.
                                DS_EFAULT : Invalid arguments
                                DS_NOMEMORY : Out of memory
                                DS_EINVAL : Invalid Operation

  @return     -1                On Failure.
               0                On Success.

  @code
  @endcode
*/
int ps_iface_set_powersave_filtering_auto_exit_mode
(
  ps_iface_type *this_iface_ptr,
  boolean       auto_exit_mode,
  sint15        *ps_errno
);

/**
  @brief  This function is used to covert iface ioctl clients into equivalent bit number.
    bit number is used to track switch data path request for different clients.

  @param[in]  iface_ioctl_client_type:  Iface client type
  @param[out]   switch_data_path_client: switch data path client


  @return     -1                On Failure.
                   bit position   On Success.

  @code
  @endcode
*/
int ps_ifacei_get_data_path_client
(
  ps_iface_ioctl_clients_enum_type        iface_ioctl_client_type,
  ps_iface_data_path_clients_enum_type   *switch_data_path_client
);


/**
  @brief  This function is used to bring down all calls from physical iface. 
          This API is only applicable to physical iface.

  @param[in]  this_iface_ptr:  Iface client type
  @param[in]  netdown_reason:  Net down reason
  @param[in]  client_data_ptr:  Client data pointer
  @param[out]   ps_errno: Error information


  @return     -1       On Failure.
               0       On Success.

  @code
  @endcode
*/
int ps_ifacei_reset_cmd
(
  ps_iface_type *this_iface_ptr,
  int16         *ps_errno,
  ps_iface_net_down_reason_type  netdown_reason,
  void          *client_data_ptr
);

/**
  @brief   This function is used to reset variables of iface.
 
*@param[in]   This_iface_ptr: ptr to interface control block on which to operate on. 
*@param[in]   event type
*@param[in]    down event information
 
RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS
  None
*/
void ps_ifacei_event_reset_iface
(
  ps_iface_type                  * this_iface_ptr,
  ps_iface_event_enum_type         event,
  ps_iface_down_event_info_type  * down_event_info_ptr
);



#ifdef TEST_FRAMEWORK
#error code not present
#endif /*TEST_FRAMEWORK*/

#ifdef __cplusplus
}
#endif

#endif /* PS_IFACEI_H */
