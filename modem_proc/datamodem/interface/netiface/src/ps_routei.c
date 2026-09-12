/*===========================================================================

                   P R O T O C O L   S E R V I C E S

              I N T E R N A L   R O U T I N G   T A B L E

GENERAL DESCRIPTION
  This file defines the structure and management functions for
  the kernel's routing table. The table is a fixed size array
  for lookup efficiency.

  Functions and data structures are specific to the IP version
  and named accordingly.

EXTERNALIZED FUNCTIONS
  routei_alloc_v4()       - Allocate an entry in the IPv4 routing table
  routei_free_v4()        - Free an entry in the IPv4 routing table
  routei_check_v4_table() - Check all IPv4 entries for consistency
  routei_get_v4_route()   - Do an IPv4 address-based routing lookup
  routei_acl_list()       - Accessor function for the registered-ACL list

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

Copyright (c) 2002-2009 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header: //components/rel/data.mpss/3.5..2.1/interface/netiface/src/ps_routei.c#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ---------------------------------------------------------- 
02/04/13    svj    New DIAG event to generate data cal status information   
02/07/07    scb    Fixed Klockworks High errors
04/28/06    mct    Added flag to netpolicy to ensure iface is set to inuse
                   only for bringup calls.
11/10/05    ks     Added check for policy_info_ptr in routei_acl_post_processing
05/12/05    rt     Fixed Lint errors.
08/25/04    vp     Changed the declaration of _errno back to extern
                   errno_enum_type to fix linking issue.
06/11/04    vp     Changes for representation of IP addresses as struct ps_in_addr
05/10/04    mct    Fixed lint errors.
03/16/04    mct    Renamed acl_ipfilter_info_type to ip_pkt_info_type.
01/30/04    usb    Added common ACL post processing function, added support
                   for multiple ACL lists.
02/20/03    om     More comments, added FEATURE_DATA_PS.
02/12/03    om     Added routei_check_v4_table(), fixed adding of routes.
08/30/02    om     Updated signature of routei_free_v4().
08/13/02    mvl    updated ps_iface state macros for name change
03/08/02    om     Created Module
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "datamodem_variation.h"
#include "target.h"
#include "customer.h"


#include "msg.h"
#include "err.h"
#include "amssassert.h"

#include "ps_acl.h"
#include "ps_in.h"
#include "ps_iface.h"
#include "ps_ifacei_utils.h"
#include "ps_route.h"
#include "ps_routei.h"
#include "ps_routei_acl.h"
#include "ps_data_call_info_logging.h"
#include "ds_Utils_DebugMsg.h"
#include <stringl/stringl.h>

/* <EJECT> */
/*===========================================================================

                        MACRO DEFINITIONS

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                        TYPE DEFINITIONS

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/* Various lists of routing ACLs */
acl_type* routei_acl_list_array[ROUTE_ACL_TYPE_MAX];

/* Definition of the actual table */
routei_v4_routing_table_type routei_v4_routing_table;

/* <EJECT> */
/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/
/*===========================================================================

FUNCTION   ROUTEI_ALLOC_V4()

DESCRIPTION
  This functions returns the index to an entry in the routing
  table to be used for a new route.
  Route entries are prioritized as HOST -> NETWORK -> DEFAULT.

DEPENDENCIES
  The new destination and gateway are not already in the table.
  Caller must ensure that this function is called with contexts locked.

PARAMETERS
  rtype  - Specifies the type of entry to allocate

RETURN VALUE
  Index of the new entry, -1 if the table is full

SIDE EFFECTS
  None
===========================================================================*/
int routei_alloc_v4
(
  route_type_enum_type  rt_type,
  uint32                destination,
  uint8                 metric,
  uint16*               cost_ptr
)
{
  int hst_range = 0;
  int net_range = 0;
  int index = 0;
  int j;
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  /**/
  /* Chack if the table is already full */
  if ( routei_v4_routing_table.table_size_used
       ==
       ROUTEI_V4_ROUTING_TABLE_SIZE )
  {
    return -1;
  }

  /* Determine the host-, net-, and default route entry ranges */
  if ( routei_v4_routing_table.entry[hst_range].flags.host )
  {
    while( routei_v4_routing_table.entry[hst_range].flags.host ) hst_range++;
  }
  net_range = hst_range;
  if ( (routei_v4_routing_table.entry[net_range].flags.gateway)
         &&
         (routei_v4_routing_table.entry[net_range].dst_addr.ps_s_addr
          !=
          ROUTE_DEFAULT_DST) )
  {
    while( (routei_v4_routing_table.entry[net_range].flags.gateway)
           &&
           (routei_v4_routing_table.entry[net_range].dst_addr.ps_s_addr
            !=
            ROUTE_DEFAULT_DST) ) net_range++;
  }
  DS_UTILS_ASSERT( net_range <= (int)routei_v4_routing_table.table_size_used );

  /* Allocate an entry in the requested group. Each group is sorted */
  /* by ascending cost and then metric to speed up routing lookups. */
  switch( rt_type )
  {
    case ROUTE_HOST:
      /* Insert route into the first group */
      index = 0;
      for ( j=0; j<hst_range; j++ )
      {
        if (routei_v4_routing_table.entry[j].dst_addr.ps_s_addr == destination)
        {
          if ( ROUTEI_V4_ROUTE_IS_CHEAPER( j, metric, cost_ptr ) )
          {
            /* Found a more expensive route to same dst, insert before */
            index = j;

            hst_range++;
            break;
          }
          else
          {
            /* Found a cheaper route to same dst, insert after */
            index = j + 1;
          }
        }
      }
      hst_range++;
      break;

    case ROUTE_NET:
      /* Insert route after all host routes */
      index = hst_range;
      for ( j=hst_range; j<net_range; j++ )
      {
        if (routei_v4_routing_table.entry[j].dst_addr.ps_s_addr == destination)
        {
          if ( ROUTEI_V4_ROUTE_IS_CHEAPER( j, metric, cost_ptr ) )
          {
            /* Found a more expensive route to same dst, insert before */
            index = j;

            net_range++;
            break;
          }
          else
          {
            /* Found a cheaper route to same dst, insert after */
            index = j + 1;
          }
        }
      }
      net_range++;
      break;

    case ROUTE_DEFAULT:
      /* Insert route after all host and network routes */
      index = net_range;
      for ( j=net_range;
            j<(int)routei_v4_routing_table.table_size_used;
            j++ )
      {
        if (routei_v4_routing_table.entry[j].dst_addr.ps_s_addr == destination)
        {
          if ( ROUTEI_V4_ROUTE_IS_CHEAPER( j, metric, cost_ptr ) )
          {
            /* Found a more expensive route to same dst, insert before */
            index = j;
            break;
          }
          else
          {
            /* Found a cheaper route to same dst, insert after */
            index = j + 1;
          }
        }
      }
      break;

    default:
      LOG_MSG_ERROR_INT_1("routei_alloc_v4(): "
                      "RT: Unknown route type %d", rt_type );
      return -1;
  }

  /* Move all entries above <index> one up */
  for ( j=(int)routei_v4_routing_table.table_size_used; j>index; j-- )
  {
    /**/
    memscpy( &(routei_v4_routing_table.entry[j]),
             sizeof( routei_v4_routing_table.entry[0] ),
            &(routei_v4_routing_table.entry[j-1]),
            sizeof( routei_v4_routing_table.entry[0] ) );
    /**/
  }
  routei_v4_routing_table.table_size_used++;
  routei_v4_routing_table.default_start_index = (unsigned)(hst_range +
                                                           net_range);

  return index;
  /**/
} /* routei_alloc_v4() */

/*===========================================================================

FUNCTION   ROUTEI_FREE_V4()

DESCRIPTION
  This functions frees up a used entry in the routing table.

DEPENDENCIES
  Caller must ensure that this function is called with contexts locked.

PARAMETERS
  rt_index  - Index of entry to remove

RETURN VALUE
  Number of remaining entries for success or -1 if index is not valid

SIDE EFFECTS
  None
===========================================================================*/
int routei_free_v4
(
  uint32  rt_index
)
{
  int i;
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Sanity check on the index */
  if ( rt_index >= routei_v4_routing_table.table_size_used )
  {
    LOG_MSG_ERROR_INT_2("routei_free_v4(): "
                    "RTI: Del index too large (%d/%d)",
                    rt_index, routei_v4_routing_table.table_size_used );
    return -1;
  }

  /* Move all entries above one down and adjust table size */
  for ( i=(int)rt_index; i<(int)routei_v4_routing_table.table_size_used; i++ )
  {
    memscpy( &(routei_v4_routing_table.entry[i]),
             sizeof( routei_v4_routing_table.entry[0] ),
            &(routei_v4_routing_table.entry[i+1]),
            sizeof( routei_v4_routing_table.entry[0] ) );
  }
  routei_v4_routing_table.table_size_used--;

  /* Adjust starting index of default entries */
  if ( rt_index < routei_v4_routing_table.default_start_index )
  {
    routei_v4_routing_table.default_start_index--;
  }

  return (int)routei_v4_routing_table.table_size_used;

} /* routei_free_v4() */


/*===========================================================================

FUNCTION   ROUTEI_CHECK_V4_TABLE()

DESCRIPTION
  This functions does a consistency check on the v4 routing table.

DEPENDENCIES
  Caller must ensure that this function is called with contexts locked.

PARAMETERS
  None

RETURN VALUE
  0 for success, otherwise the entry # to the offending line

SIDE EFFECTS
  None
===========================================================================*/
int routei_check_v4_table( void )
{
  int i;
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Loop through entries and check them */
  for ( i = (int)routei_v4_routing_table.table_size_used - 1; i >= 0; i-- )
  {
    if ( routei_v4_routing_table.entry[i].if_ptr == NULL )
    {
      LOG_MSG_ERROR_INT_1("routei_check_v4_table(): "
                      "RTv4: NULL iface in entry %d", i );
      break;
    }
  }

  return ( i + 1 );

} /* routei_check_v4_table() */


/*===========================================================================

FUNCTION   ROUTEI_GET_V4_ROUTE()

DESCRIPTION
  This functions returns the index to an entry in the routing
  table for a given destination. A NULL gateway pointer is
  intended for a regular routing lookup, a specified gateway
  allows to find the entry index of a known route, e.g. to
  update or delete it.
  The result is purely based on IP address based routing.

DEPENDENCIES
  Caller must ensure that this function is called with contexts locked.

PARAMETERS
  destination  - Destination address
  gateway_ptr  - Optional pointer to gateway for this route

RETURN VALUE
  Index of the routing entry, -1 if no matching entry was found

SIDE EFFECTS
  None
===========================================================================*/
int routei_get_v4_route
(
  uint32   destination,
  uint32*  gateway_ptr
)
{
  int      index;
  boolean  rt_found = FALSE;

  /* Search the routing table from the top (host routes first,  */
  /* then network and default routes), optimization for default */
  /* destinations to skip all host and network routes.          */
  for (  index = ( (destination == ROUTE_DEFAULT_DST) ?
                   (int)routei_v4_routing_table.default_start_index : 0 );
         index < (int)routei_v4_routing_table.table_size_used;
         index++ )
  {
    if ( (ps_iface_state( routei_v4_routing_table.entry[index].if_ptr )
          !=
          IFACE_DISABLED)
         &&
         ((destination & routei_v4_routing_table.entry[index].dst_netmask)
         ==
         (routei_v4_routing_table.entry[index].dst_addr.ps_s_addr
          &
          routei_v4_routing_table.entry[index].dst_netmask)) )
    {
      /* Check if an explicit gateway was specified */
      if ( gateway_ptr != NULL )
      {
        if ( routei_v4_routing_table.entry[index].gateway.ps_s_addr
             == *gateway_ptr )
        {
          rt_found = TRUE;
          break;
        }
      }
      else
      {
        rt_found = TRUE;
        break;
      }
    }
  }

  if ( rt_found )
  {
    return index;
  }
  else
  {
    return -1;
  }
} /* routei_get_v4_route() */


/*===========================================================================

FUNCTION   ROUTEI_ACL_LIST()

DESCRIPTION
  This functions returns the pointer to the first ACL in the
  kernel's list of routing ACLs.

DEPENDENCIES
  None

PARAMETERS
  rlist_type - Type of ACL list to look for.

RETURN VALUE
  Pointer to ACL list

SIDE EFFECTS
  None
===========================================================================*/
acl_type** routei_acl_list
(
  route_acl_type_enum_type rlist_type
)
{
  if (rlist_type >= ROUTE_ACL_TYPE_MAX)
  {
    DS_UTILS_ASSERT(0);
    return NULL;
  }

  return &routei_acl_list_array[rlist_type];

} /* routei_acl_list() */


/*===========================================================================

FUNCTION   ROUTEI_ACL_POST_PROCESSING()

DESCRIPTION
  This function is the common post processing function for the outbound
  routing ACLs. This function gets called for all ifaces and performs
  common post processing tasks for the iface that's picked up as a result
  of the execution of the ACL.

DEPENDENCIES
  Caller must ensure that this function is called with contexts locked.

PARAMETERS
  this_if_ptr - Iface ptr
  policy_info_ptr - Routing policy Info
  pkt_info_ptr - IP packet info

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void routei_acl_post_processing
(
  void*                    this_if_ptr,
  acl_policy_info_type*    policy_info_ptr,
  ip_pkt_info_type*        pkt_info_ptr
)
{
  ps_iface_type *if_ptr;
  ps_iface_apn_enum_type  apn_type = PS_IFACE_APN_TYPE_UNSPECIFIED;  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if_ptr = (ps_iface_type *) this_if_ptr;

  if (NULL == if_ptr)
  {
    DS_UTILS_ERROR_FATAL("routei_acl_post_processing(): got NULL Interface pointer ");
    return;
  }

  LOG_MSG_INFO2_2("routei_acl_post_processing(): Iface 0x%x::%d",
                  if_ptr->name, if_ptr->instance);

  /*-------------------------------------------------------------------------
     If an UP interface is selected by routing, this function is anyway
     called even if its already in use
  -------------------------------------------------------------------------*/
  if (PS_IFACE_GET_PROC_ID(if_ptr) == PS_IFACE_PROC_ID_NONE)
  {
    if (NULL == policy_info_ptr)
    {
      DS_UTILS_ERROR_FATAL("routei_acl_post_processing(): Got NULL policy_info_ptr");
    }

    /*-----------------------------------------------------------------------
      If the routing is for lookup only don't block the iface, this allows
      other clients to use the iface if they intend to do so. Else mark the
      iface state appropriately.
    -----------------------------------------------------------------------*/
    if (policy_info_ptr->lookup_only)
    {
      PS_IFACE_SET_LOOKED_UP(if_ptr);
    }
    else if (policy_info_ptr->bring_up)
    {
      PS_IFACE_SET_IN_USE(if_ptr);
      PS_IFACE_RESET_LOOKED_UP(if_ptr);

      if (policy_info_ptr->is_routeable)
      {
        PS_IFACE_SET_PROC_ID(if_ptr, PS_IFACE_PROC_ID_ANY);
      }
      else
      {
        PS_IFACE_SET_PROC_ID(if_ptr, policy_info_ptr->proc_id);
      }

      if (policy_info_ptr->pdp_info !=  NULL)
      {
        PS_IFACEI_SET_3GPP_PROFILE_ID(if_ptr, 
                                      (uint32)policy_info_ptr->pdp_info);
      }
      else
      {
        LOG_MSG_ERROR_INT_0("routei_acl_post_processing(): "
                        "Null 3gpp profile ID passed, not upating iface "
                        "state with 3gpp profile id" );
      }

      PS_IFACEI_SET_3GPP2_PROFILE_ID(if_ptr,
                                   policy_info_ptr->data_session_profile_id);
      PS_IFACE_SET_SUBS_ID(if_ptr, policy_info_ptr->subs_id);
      PS_IFACEI_SET_RM_INST_HANDLE(if_ptr, policy_info_ptr->rm_inst_handle);

      PS_IFACE_SET_APN_TYPE(if_ptr, policy_info_ptr->apn_type);
      LOG_MSG_INFO2_3("routei_acl_post_processing(): 3gpp prof id %d"
                      "3gpp2 prof id %d, subs id %d",policy_info_ptr->pdp_info,
                       policy_info_ptr->data_session_profile_id, 
                       policy_info_ptr->subs_id);
    }

    /*-----------------------------------------------------------------------
      If a logical interface is selected by routing with acl policy
      specifying an associated iface, set specified iface to TRAT
    -----------------------------------------------------------------------*/
    if (PS_IFACE_IS_LOGICAL(if_ptr) &&
        NULL != policy_info_ptr->assoc_iface_info.ps_iface_ptr)
    {
      PS_IFACE_SET_TRAT_IFACE( if_ptr,
                               policy_info_ptr->assoc_iface_info.ps_iface_ptr);
    }
  }
  else
  {
    /*-----------------------------------------------------------------------
      For shared calls, set apn type by checking the policy info and iface.
      Iface apn is set with the one that has valid value. For e.g if IMS
      APN is brought up followed by a default(unspecified) APN, or vice versa
      iface apn is set to IMS.
    -----------------------------------------------------------------------*/
    apn_type = (PS_IFACE_GET_APN_TYPE(if_ptr) != PS_IFACE_APN_TYPE_UNSPECIFIED) ?
                PS_IFACE_GET_APN_TYPE(if_ptr) : policy_info_ptr->apn_type;
    PS_IFACE_SET_APN_TYPE(if_ptr, apn_type);

    if (PS_IFACE_GET_PROC_ID(if_ptr) == PS_IFACE_PROC_ID_ANY)
    {
      if (FALSE == policy_info_ptr->is_routeable &&
          TRUE == policy_info_ptr->bring_up)
      {
        PS_IFACE_SET_PROC_ID(if_ptr, policy_info_ptr->proc_id); 
      }
    }

    /*-----------------------------------------------------------------------
      If a logical interface is selected by routing with acl policy specifying
      an associated iface, set assoc iface to TRAT of the logical iface
    -----------------------------------------------------------------------*/
    if (PS_IFACE_IS_LOGICAL(if_ptr) &&
        NULL != policy_info_ptr->assoc_iface_info.ps_iface_ptr &&
        (PS_IFACE_HANDOFF_CLASS_EPC ==
           policy_info_ptr->handoff_ctxt.handoff_class ||
         PS_IFACE_HANDOFF_CLASS_EPC_PREREG ==
           policy_info_ptr->handoff_ctxt.handoff_class))
    {
      PS_IFACE_SET_TRAT_IFACE( if_ptr,
                               policy_info_ptr->assoc_iface_info.ps_iface_ptr);
    }
  }

} /* routei_acl_post_processing() */

/*===========================================================================
FUNCTION   ROUTEI_ACL_IS_BETTER_MATCH ()

DESCRIPTION
  This function compares 2 ACL results and returns TRUE if the first ACL is
  is better than the second.

DEPENDENCIES
  None

PARAMETERS
  curr_res_priority - The first ACL result priority
  curr_acl_ptr      - The first ACL result ptr
  prev_res_priority - The second ACL result priority
  prev_acl_ptr      - The second ACL result ptr

RETURN VALUE
  boolean value

SIDE EFFECTS
  None
===========================================================================*/
boolean routei_acl_is_better_match
(
  int            curr_res_priority,
  acl_type      *curr_acl_ptr,
  int            prev_res_priority,
  acl_type      *prev_acl_ptr
)
{
/*-------------------------------------------------------------------------*/
  if ( NULL == curr_acl_ptr )
  {
    DS_UTILS_ERROR_FATAL("routei_acl_is_better_match(): routei_acl_get_best_match(): NULL pointer ");
    return FALSE;
  }

  if ( NULL == prev_acl_ptr )
  {
    /*---------------------------------------------------------------------
      Nothing to compare, the current acl ptr is better
    ---------------------------------------------------------------------*/
    return TRUE;
  }

  if ( curr_res_priority > prev_res_priority )
  {
    return TRUE;
  }
  /*-----------------------------------------------------------------------
    In case of tie give preference to the one that is not already looked up
  -----------------------------------------------------------------------*/
  else if ( curr_res_priority == prev_res_priority )
  {
    if ( TRUE == PS_IFACE_IS_LOOKED_UP(ACL_IF_PTR(curr_acl_ptr)) &&
         FALSE == PS_IFACE_IS_LOOKED_UP(ACL_IF_PTR(prev_acl_ptr)) )
    {
      /*-------------------------------------------------------------------
        Previous best iface was being looked up and this one is not,
        hence prefer this one over the previous best
      -------------------------------------------------------------------*/
      return TRUE;
    }
  }

  return FALSE;
}  /* routei_acl_is_better_match */



/*===========================================================================
FUNCTION ROUTEI_GET_APP_ID_FROM_PROC_ID ()

DESCRIPTION
  This function gets the app_id from given proc_id

DEPENDENCIES
  None

PARAMETERS
  app_id  - Application identifier
  proc_id - Process identifier

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean routei_get_app_id_from_proc_id(
  uint64                       *app_id,
  acl_proc_id_type              proc_id
)
{
  LOG_MSG_INFO2_1( "mapping req: proc_id [0x%x]", (uint32)proc_id );

  if ( app_id == NULL ) return FALSE;

  /* Check for proc_id match */
  switch ( proc_id )
  {
    case PS_DATA_CALL_INFO_LOGGING_PROC_ID_MODEM_EMBEDDED :
      *app_id = PS_DATA_CALL_INFO_LOGGING_APP_ID_MODEM_EMBEDDED;
      break;

    case PS_DATA_CALL_INFO_LOGGING_PROC_ID_TE_DUN :
      *app_id = PS_DATA_CALL_INFO_LOGGING_APP_ID_TE_DUN;
      break;

    case PS_DATA_CALL_INFO_LOGGING_PROC_ID_APPS1  :
      *app_id = PS_DATA_CALL_INFO_LOGGING_APP_ID_APPS;
      break;

    case PS_DATA_CALL_INFO_LOGGING_PROC_ID_TE_LAPTOP1 :
      *app_id = PS_DATA_CALL_INFO_LOGGING_APP_ID_TE_RMNET;
      break;

     default:
      break;
  }

  LOG_MSG_INFO2_1( "proc_id to app_id mapping complete: app_id:%d", app_id );
  return TRUE;
} /* routei_get_app_id_from_proc_id */

