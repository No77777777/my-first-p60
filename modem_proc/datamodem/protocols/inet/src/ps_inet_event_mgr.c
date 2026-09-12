/*===========================================================================

@file: ps_inet_event_mgr.c

DESCRIPTION
 The protocol services inet memory pool manager file. Contains definitions
 and initializations for inet memory pools.

EXTERNAL FUNCTIONS
  INET_MEM_POOL_INIT()
    Initialzes memory pools for inet module.

Copyright (c) 2008-2014 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================

                             EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Note that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/protocols/inet/src/ps_inet_event_mgr.c#2 $
  $DateTime: 2019/10/22 02:53:17 $
  $Author: pwbldsvc $

when       who     what, where, why
--------   ---     -----------------------------------------------------------
13/12/17    my     CR 2157129 : FR 42790: F3 reduction
11/04/14   kc      Added ps_inet_event_mgr_deinit() and event reg optimization
05/22/09   kk      Q6 compiler warning fixes.
11/07/2008 msr     Created the module
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"

#include "msg.h"
#include "ds_Sock_Def.h"
#include "ps_socket_cmd.h"
#include "ps_iface.h"
#include "ps_crit_sect.h"
#include "ps_flowi_event.h"
#include "ps_phys_link.h"
#include "ps_tcp.h"
#include "ps_socket_defs.h"
#include "ps_inet_event_mgr.h"
#include "ds_Utils_DebugMsg.h"
#include "ps_ifacei_event.h" 
#include "ps_phys_linki_event.h"

#define PS_INET_EVENT_MGR_ASSERT(expression)  \
          ps_inet_event_mgr_assert_wrapper(__LINE__, expression)

/*===========================================================================

                    LOCAL DATA DEFINITIONS

===========================================================================*/
static void  * ps_inet_iface_flow_enabled_event_handle;
static void  * ps_inet_ps_flow_tx_enabled_event_handle;
static void  * ps_inet_phys_link_flow_enabled_event_handle;

/*===========================================================================

FUNCTION ps_inet_event_mgr_assert_wrapper

DESCRIPTION
  Wrapper function for PS_INET_EVENT_MGR ASSERT
 
DEPENDENCIES 
  None

RETURN VALUE
  None
 
SIDE EFFECTS

=============================================================================*/
static void ps_inet_event_mgr_assert_wrapper
(
  unsigned int     line_num,
  int              expression
)
{
  if ( !expression )
  {
    ERR_FATAL("PS_INET_EVENT_MGR_ERR_FATAL at line:%d ",  
                                   line_num,0,0);  
  }
  
}/* ps_inet_event_mgr_assert_wrapper */

/*===========================================================================
FUNCTION      DSSOCKI_IFACE_EV_CBACK()

DESCRIPTION   Called when the IFACE goes up/down or when IP address changes.
              This posts a command to PS and is processed in PS task context.

DEPENDENCIES

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
static void ps_ineti_iface_ev_cback
(
  ps_iface_type               * iface_ptr,
  ps_iface_event_enum_type      event,
  ps_iface_event_info_u_type    event_info,
  void                        * user_info_ptr
)
{
  struct tcb              * tcb_ptr = NULL;
  ps_socket_cmd_type      * flow_enabled_cmd_buf_ptr;
  int                       count = 0;

#ifdef FEATURE_SEC_IPSEC
  ps_ipsec_info_type      * ipsec_info_ptr;
  int32                     idx;
  boolean                   in_iface_list = FALSE;
#endif /* FEATURE_SEC_IPSEC */
  (void)event_info;
  (void)user_info_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (!PS_IFACE_IS_VALID( iface_ptr))
  {
    DATA_PROTO_ERROR_LOG_1(PS_BAD_ARG_OR_NULL_PTR, iface_ptr);
    return;
  }

  LOG_MSG_INFO2_3( "Rcvd evt %d on iface 0x%x:%d",
                   event, iface_ptr->name, iface_ptr->instance);

  switch (event)
  {
    case IFACE_FLOW_ENABLED_EV:
    {
      /*---------------------------------------------------------------------
        Loop through list of TCBs
      ---------------------------------------------------------------------*/
      PS_ENTER_CRIT_SECTION( &global_ps_crit_section);

      tcb_ptr = (struct tcb *) q_check( &sock_config_cb.tcp_cb_q );
      while (NULL != tcb_ptr && count < q_cnt(&sock_config_cb.tcp_cb_q) )
      {
        PS_INET_EVENT_MGR_ASSERT( SOCKAVAIL != tcb_ptr->sockfd);

        if (NULL != tcb_ptr->ipcb.meta_info_ptr)
        {
          /*-----------------------------------------------------------------
            Ignore all the sockets that are not bound to the ps_iface on which
            this event is generated
          -----------------------------------------------------------------*/
#ifdef FEATURE_SEC_IPSEC
          ipsec_info_ptr =
            &( PS_META_GET_IPSEC_INFO( tcb_ptr->ipcb.meta_info_ptr));

          for (idx = 0; idx < ipsec_info_ptr->iface_cnt; idx++)
          {
            if (iface_ptr == ipsec_info_ptr->iface_list[idx])
            {
              in_iface_list = TRUE;
              break;
            }
          }
#endif /* FEATURE_SEC_IPSEC */

          if (iface_ptr ==
                PS_META_GET_ROUTING_CACHE( tcb_ptr->ipcb.meta_info_ptr)
#ifdef FEATURE_SEC_IPSEC
              || TRUE == in_iface_list
#endif /* FEATURE_SEC_IPSEC */
             )
          {
            flow_enabled_cmd_buf_ptr =
              ps_socket_cmd_alloc( PS_SOCKET_CMD_FLOW_ENABLED_IND);

            if (NULL != flow_enabled_cmd_buf_ptr)
            {
              ps_socket_cmd_send( &flow_enabled_cmd_buf_ptr,
                                  tcb_ptr->sockfd,
                                  (int32) tcb_ptr,
                                  ds_Sock_Protocol_QDS_TCP);
            }
            else
            {
              LOG_MSG_ERROR_0( "Couldn't alloc cmd buf");
            }
          }
        }

        tcb_ptr =
          (struct tcb *) q_next(&(sock_config_cb.tcp_cb_q), &(tcb_ptr->link));
		count++;

      } /* while tcb in list */

      PS_LEAVE_CRIT_SECTION( &global_ps_crit_section);
      break;
    }

    default:
    {
      //LOG_MSG_ERROR_1( "Unknown evt %d", event);
      PS_INET_EVENT_MGR_ASSERT( 0);
      break;
    }
  } /* switch (event) */

  return;
} /* ps_ineti_iface_ev_cback() */




/*===========================================================================
FUNCTION DSSOCKI_FLOW_EV_CBACK()

DESCRIPTION
  Called when any event happens on a flow.
  This posts a command to PS and is processed in PS task context.

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void ps_ineti_ps_flow_ev_cback
(
  ps_flow_type                * ps_flow_ptr,
  ps_iface_event_enum_type      event,
  ps_iface_event_info_u_type    event_info,
  void                        * user_info_ptr
)
{
  struct tcb              * tcb_ptr = NULL;
  ps_socket_cmd_type      * flow_enabled_cmd_buf_ptr;
  int                       count = 0;
  (void)event_info;
  (void)user_info_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (!PS_FLOW_IS_VALID( ps_flow_ptr))
  {
    DATA_PROTO_ERROR_LOG_1(PS_BAD_ARG_OR_NULL_PTR, ps_flow_ptr);
    return;
  }

#if 0
  LOG_MSG_INFO2_2( "Rcvd evt %d on flow 0x%p", event, ps_flow_ptr);
#endif

  switch (event)
  {
    case FLOW_TX_ENABLED_EV:
    {
      /*---------------------------------------------------------------------
        Loop through list of TCBs
      ---------------------------------------------------------------------*/
      PS_ENTER_CRIT_SECTION( &global_ps_crit_section);

      tcb_ptr = (struct tcb *) q_check( &sock_config_cb.tcp_cb_q );
      while (NULL != tcb_ptr && count < q_cnt(&sock_config_cb.tcp_cb_q))
      {
        PS_INET_EVENT_MGR_ASSERT( SOCKAVAIL != tcb_ptr->sockfd);

        if (NULL != tcb_ptr->ipcb.meta_info_ptr)
        {
          /*-----------------------------------------------------------------
            Ignore all the sockets that are not bound to the ps_flow on which
            this event is generated
          -----------------------------------------------------------------*/
          if (ps_flow_ptr ==
                PS_FLOW_GET_FLOW_FROM_META_INFO(tcb_ptr->ipcb.meta_info_ptr) ||
              PS_FLOWI_GET_CAPABILITY( ps_flow_ptr,
                (uint8)PS_FLOW_CAPABILITY_DEFAULT )
             )
          {
            flow_enabled_cmd_buf_ptr =
              ps_socket_cmd_alloc( PS_SOCKET_CMD_FLOW_ENABLED_IND);

            if (NULL != flow_enabled_cmd_buf_ptr)
            {
              ps_socket_cmd_send( &flow_enabled_cmd_buf_ptr,
                                  tcb_ptr->sockfd,
                                  (int32) tcb_ptr,
                                  ds_Sock_Protocol_QDS_TCP);
            }
            else
            {
              LOG_MSG_ERROR_0( "Couldn't alloc cmd buf");
            }
          }
        }

        tcb_ptr =
          (struct tcb *) q_next(&(sock_config_cb.tcp_cb_q), &(tcb_ptr->link));
		count++;

      } /* while tcb in list */

      PS_LEAVE_CRIT_SECTION( &global_ps_crit_section);
      break;
    }

    default:
    {
      //LOG_MSG_ERROR_1( "Unknown evt %d", event);
      PS_INET_EVENT_MGR_ASSERT( 0);
      break;
    }
  } /* switch (event) */

  return;
} /* ps_ineti_ps_flow_ev_cback() */


/*===========================================================================
FUNCTION      DSSOCKI_PHYS_LINK_EV_CBACK()

DESCRIPTION   Called when the phys link changes state. This posts a command
              to PS and is processed in PS task context.

DEPENDENCIES

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
static void ps_ineti_phys_link_ev_cback
(
  ps_phys_link_type           * phys_link_ptr,
  ps_iface_event_enum_type      event,
  ps_iface_event_info_u_type    event_info,
  void                        * user_info_ptr
)
{
  struct tcb              * tcb_ptr = NULL;
  ps_socket_cmd_type      * flow_enabled_cmd_buf_ptr;
  ps_flow_type            * rt_flow_ptr;
  int                       count = 0;
  (void)event_info;
  (void)user_info_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  PS_ENTER_CRIT_SECTION( &global_ps_crit_section);

  if (!PS_PHYS_LINK_IS_VALID( phys_link_ptr))
  {
    DATA_PROTO_ERROR_LOG_1(PS_BAD_ARG_OR_NULL_PTR, phys_link_ptr);
    PS_BRANCH_LEAVE_CRIT_SECTION( &global_ps_crit_section);
    
    return;
  }

  LOG_MSG_INFO2_2( "Rcvd evt %d on phys link 0x%p", event, phys_link_ptr);

  switch (event)
  {
    case PHYS_LINK_FLOW_ENABLED_EV:
    {
      /*---------------------------------------------------------------------
        Loop through list of TCBs
      ---------------------------------------------------------------------*/

      tcb_ptr = (struct tcb *) q_check( &sock_config_cb.tcp_cb_q );
      while (NULL != tcb_ptr && count < q_cnt(&sock_config_cb.tcp_cb_q))
      {
        PS_INET_EVENT_MGR_ASSERT( SOCKAVAIL != tcb_ptr->sockfd);

        if (NULL != tcb_ptr->ipcb.meta_info_ptr)
        {
          /*-----------------------------------------------------------------
            Ignore all the sockets that are not bound to the ps_flow on which
            this event is generated
          -----------------------------------------------------------------*/
          rt_flow_ptr =
            PS_FLOW_GET_FLOW_FROM_META_INFO(tcb_ptr->ipcb.meta_info_ptr);

          if (( NULL != rt_flow_ptr &&
                phys_link_ptr == PS_FLOWI_GET_PHYS_LINK( rt_flow_ptr)) ||
              PS_PHYS_LINKI_GET_CAPABILITY( phys_link_ptr,
                (uint8)PS_PHYS_LINK_CAPABILITY_PRIMARY )
             )
          {
            flow_enabled_cmd_buf_ptr =
              ps_socket_cmd_alloc( PS_SOCKET_CMD_FLOW_ENABLED_IND);

            if (NULL != flow_enabled_cmd_buf_ptr)
            {
              ps_socket_cmd_send( &flow_enabled_cmd_buf_ptr,
                                  tcb_ptr->sockfd,
                                  (int32) tcb_ptr,
                                  ds_Sock_Protocol_QDS_TCP);
            }
            else
            {
              LOG_MSG_ERROR_0( "Couldn't alloc cmd buf");
            }
          }
        }

        tcb_ptr =
          (struct tcb *) q_next(&(sock_config_cb.tcp_cb_q), &(tcb_ptr->link));
		count++;

      } /* while tcb in list */

      break;
    }

    default:
    {
      //LOG_MSG_ERROR_1( "Unknown evt %d", event);
      PS_INET_EVENT_MGR_ASSERT( 0);
      break;
    }
  } /* switch (event) */
  PS_LEAVE_CRIT_SECTION( &global_ps_crit_section);

  return;
} /* ps_ineti_phys_link_ev_cback() */


/*===========================================================================

                      EXTERNAL FUNCTION DEFINITIONS

===========================================================================*/
/*===========================================================================
FUNCTION PS_INET_EVENT_MGR_INIT()

DESCRIPTION
  inet module registers with ps_iface for flow control events

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ps_inet_event_mgr_init
(
  void
)
{
  ps_iface_event_enum_type iface_flow_event_reg_array[1];
  ps_iface_event_enum_type ps_flow_event_reg_array[1];
  ps_iface_event_enum_type phys_link_event_reg_array[1];

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Allocate event handle to register for iface event IFACE_FLOW_ENABLED_EV
  -------------------------------------------------------------------------*/
  ps_inet_iface_flow_enabled_event_handle = 
    (void*)ps_iface_alloc_event_handle(NULL, ps_ineti_iface_ev_cback, NULL);

  if (NULL == ps_inet_iface_flow_enabled_event_handle)
  {
    /* DATA_ERR_FATAL( "Could not alloc event buf"); */
	PS_INET_EVENT_MGR_ASSERT(0);
  }

   /*---------------------------------------------------------------------
    Subscribe for IFACE_FLOW_ENABLED_EV
  ---------------------------------------------------------------------*/
  iface_flow_event_reg_array[0] = IFACE_FLOW_ENABLED_EV;

  if (-1 == ps_iface_subscribe_event_list(
             ps_inet_iface_flow_enabled_event_handle,
             iface_flow_event_reg_array,
             1))
  {
    /* DATA_ERR_FATAL("Subscribe IFACE_FLOW_ENABLED_EV event failed"); */
    PS_INET_EVENT_MGR_ASSERT(0);
  }

  /*-------------------------------------------------------------------------
    Allocate event handle to register for flow event FLOW_TX_ENABLED
  -------------------------------------------------------------------------*/
  ps_inet_ps_flow_tx_enabled_event_handle =
    (void*)ps_flow_alloc_event_handle(NULL, ps_ineti_ps_flow_ev_cback, NULL);

  if (NULL == ps_inet_ps_flow_tx_enabled_event_handle)
  {
    /* DATA_ERR_FATAL( "Could not alloc event buf"); */
	PS_INET_EVENT_MGR_ASSERT(0);
  }

   /*---------------------------------------------------------------------
    Subscribe for FLOW_TX_ENABLED
  ---------------------------------------------------------------------*/
  ps_flow_event_reg_array[0] = FLOW_TX_ENABLED_EV;

  if ( -1 == ps_flow_subscribe_event_list (
             ps_inet_ps_flow_tx_enabled_event_handle,
             ps_flow_event_reg_array,
             1) )
  {
    /* DATA_ERR_FATAL("Subscribe FLOW_TX_ENABLED event failed"); */
    PS_INET_EVENT_MGR_ASSERT(0);
  }

  /*---------------------------------------------------------------------------------
    Allocate event handle to register for phys link event PHYS_LINK_FLOW_ENABLED_EV
  ---------------------------------------------------------------------------------*/
  ps_inet_phys_link_flow_enabled_event_handle = 
    (void*)ps_phys_link_alloc_event_handle(NULL, ps_ineti_phys_link_ev_cback, NULL);

  if (NULL == ps_inet_phys_link_flow_enabled_event_handle)
  {
    /* DATA_ERR_FATAL( "Could not alloc event buf"); */
	PS_INET_EVENT_MGR_ASSERT(0);
  }

  /*---------------------------------------------------------------------
    Subscribe for PHYS_LINK_FLOW_ENABLED_EV
  ---------------------------------------------------------------------*/
  phys_link_event_reg_array[0] = PHYS_LINK_FLOW_ENABLED_EV;

  if ( -1 == ps_phys_link_subscribe_event_list (
             ps_inet_phys_link_flow_enabled_event_handle,
             phys_link_event_reg_array,
             1))
  {
    /* DATA_ERR_FATAL("Subscribe PHYS_LINK_FLOW_ENABLED_EV event failed");*/
     PS_INET_EVENT_MGR_ASSERT(0);
  }

  return;
} /* ps_inet_event_mgr_init() */


/*===========================================================================
FUNCTION PS_INET_EVENT_MGR_DEINIT()

DESCRIPTION
  inet module de-registers with ps_iface for flow control events

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ps_inet_event_mgr_deinit
(
  void
)
{
  /*---------------------------------------------------------------------
    Free event handles
  ---------------------------------------------------------------------*/
  if (NULL != ps_inet_iface_flow_enabled_event_handle)
  {
    ps_iface_free_event_handle(ps_inet_iface_flow_enabled_event_handle);
    ps_inet_iface_flow_enabled_event_handle = NULL;
  }

  if (NULL != ps_inet_ps_flow_tx_enabled_event_handle)
  {
    ps_flow_free_event_handle(ps_inet_ps_flow_tx_enabled_event_handle);
    ps_inet_ps_flow_tx_enabled_event_handle = NULL;
  }

  if (NULL != ps_inet_phys_link_flow_enabled_event_handle)
  {
    ps_phys_link_free_event_handle(ps_inet_phys_link_flow_enabled_event_handle);
    ps_inet_phys_link_flow_enabled_event_handle = NULL;
  }

} /* ps_inet_event_mgr_deinit() */

