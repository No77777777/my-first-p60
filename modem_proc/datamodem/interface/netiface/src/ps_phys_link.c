
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                             P S _ P H Y S _ L I N K . C

DESCRIPTION
  File defining all of the data types and the interface control block
  for the ps physical link architecture.

EXTERNAL FUNCTIONS

  PS_PHYS_LINK_CREATE()
    Used to create ps_phys_links.

  PS_PHYS_LINK_UP_CMD()
    Used to bring up the phys link.  Can be called by external entity.

  PS_PHYS_LINK_DOWN_CMD()
    Used to tear down the phys link.  Can be called by external entity.

  PS_PHYS_LINK_ENABLE_FLOW()
    Enable data flow on the phys link.  User passes in a mask indicating the
    flow control bit being enabled.

  PS_PHYS_LINK_DISABLE_FLOW()
    Disable data flow on the phys link.  User passes in a mask indicating the
    flow control bit being disabled.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None for the module.  Each phys_link is created by calling
  ps_phys_link_create().

Copyright (c) 2003-2014 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                            EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/netiface/src/ps_phys_link.c#3 $
  $Author: pwbldsvc $ $DateTime: 2021/02/09 01:09:33 $

when        who    what, where, why
--------    ---    ---------------------------------------------------------- 
11/30/11    su     Adding log code support for DPL logging  
06/18/08    pp     Metainfo optimizations.
02/27/07    scb    Fixed HIGH lint errors
02/09/07    ac     EMPA fix the cleanup issue for link protocol.
02/01/07    msr    Added ps_phys_sdb_status_handler()
12/12/06    ifk    Featurized IPHC and ROHC code.
09/12/06    msr    Removed redundant state field in event_info structure
06/05/06    mp     Moved DPL link logging control block from ps_iface to
                   ps_phys_link
04/25/06    msr    L4/Tasklock code review changes
02/22/06    msr    Using single critical section
02/06/06    msr    Updated for L4 tasklock/crit sections.
01/17/06    ssh    Cast assignments for ppp_input and ppp_output
12/23/05    rt     Changed one F3 message in ps_phys_link_tx_cmd from HIGH
                   to LOW.
12/09/05    sv     Added support for new data path framework.
08/16/05    msr    Fixed PS_BRANCH_TASKFREE()
08/15/05    msr    Removed support for flow logging. Using ps_flows instead
08/12/05    sv     Return ENETGOINGDORMANT when bringup command is called in
                   GOING_DOWN state.
05/12/05    mct    Lint changes.
04/17/05    msr    Moved support for QOS to ps_flow.
04/16/05    ks     Changes due to addition of PHYS_LINK_NULL state
01/12/05    msr    Added code review comments.
01/10/05    msr    Added support for flow logging.
01/08/05    msr    Added ps_phys_link_get_first_rx_fltr(),
                   ps_phys_link_get_rx_filter_handle(), and
                   ps_phys_link_get_rx_filter_by_handle().
12/02/04    msr    Initializing phys link's state to PHYS_LINK_DOWN when it
                   is created.
11/02/04    mct    Added QOS parameter validation.
10/31/04   msr/ks  Changed the variable name in the data protocol logging
                   control block which is initialized in
                   ps_phys_link_create().
10/14/04   ks/msr  Added support for Data Protocol Logging.
08/12/04    sv     Flow control the physlink when we are bringing up/tearing
                   down the physlink.
05/10/04    mct    Fixed lint errors.
02/13/04    ak     Moved sdb interface into this file.
12/20/03    ak     Updated for phys_link/iface separation.
===========================================================================*/
/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

                                INCLUDE FILES

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
#include "datamodem_variation.h"
#include "customer.h"
#include "comdef.h"
#include "amssassert.h"

#include "ds_flow_control.h"
#include "dserrno.h"
#include "err.h"
#include "msg.h"
#include "ps_ifacei_event.h"
#include "ps_phys_link.h"
#include "ps_pkt_info.h"
#include "ps_phys_linki_event.h"

#include "ps_utils.h"
#include "ps_crit_sect.h"
#include "ps_system_heap.h"
#include "ds_Utils_DebugMsg.h"

#include "ps_logging_diag.h"
#include "ps_phys_linki.h"
/*===========================================================================

                           LOCAL DATA DECLARATIONS

===========================================================================*/
/*---------------------------------------------------------------------------
  Data type used in PS_PHYS_LINK_FLOW_CTRL_CMD
---------------------------------------------------------------------------*/
typedef struct
{
  ps_phys_link_internal_cmd_type  cmd_type;
  ps_phys_link_type             * ps_phys_link_ptr;
  uint64                          flow_mask;
  boolean                         is_enabled;
  uint32                          allowed_num_bytes;
  boolean                         ack_required;
  uint64                          prev_mask;
  boolean                         report_legacy_flow_ctl_flag;
  boolean                         report_ul_burst_flow_ctl_flag;
#ifdef FEATURE_DATA_FLOW_MGMT
  ps_phys_link_data_traffic_enum_type     traffic_type;
  ps_phys_link_flow_ctl_reason_enum_type  flow_ctl_reason;
#endif /* FEATURE_DATA_FLOW_MGMT */
} ps_phys_linki_flow_ctrl_cmd_info_type;

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

                             FORWARD DECLARATIONS

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
void ps_phys_linki_default_tx_cmd
(
  ps_phys_link_type     * this_phys_link_ptr,
  dsm_item_type        ** pkt_ref_ptr,
  ps_tx_meta_info_type  * meta_info_ptr,
  void                  * tx_cmd_info
);

static ps_phys_link_bearer_id_ex_type ps_phys_link_bearer_ids_ex[PS_PHYS_LINK_MAX_BEARER_EX_ID];
static ps_phys_linki_prev_log_info    ps_phys_link_prev_log_status;
/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

                             EXTERNAL FUNCTIONS

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/

uint8   ps_phys_linki_get_bearer_id_ex
(
  void
)
{
  uint8            index = 0;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for ( index = 2; index < PS_PHYS_LINK_MAX_BEARER_EX_ID; index++)
  {
    if ( FALSE == ps_phys_link_bearer_ids_ex[index].in_use )
    {
      ps_phys_link_bearer_ids_ex[index].in_use= TRUE;
      return index;
    }
  }
  LOG_MSG_ERROR_INT_0("ps_phys_linki_get_bearer_id_ex, No free bearer ex id ");
  
  return 0xFF;
}/* ps_phys_linki_get_bearer_id_ex */

void ps_phys_linki_free_bearer_id_ex
(
  uint8                  index
)
{

  if ( index < PS_PHYS_LINK_MAX_BEARER_EX_ID )
  {
    ps_phys_link_bearer_ids_ex[index].in_use= FALSE;
  }

  LOG_MSG_INFO2_1("ps_phys_linki_free_bearer_id_ex(): free bearer ex id", index);
}/* ps_phys_linki_free_bearer_id_ex */

/*===========================================================================
FUNCTION PS_PHYS_LINK_CREATE()

DESCRIPTION
  This will memset()s the private data struct to 0, initializes the
  "this_phys_link_ptr" to point to the interface control block itself,
  and initializes the queues.

PARAMETERS
  this_phys_link_ptr: Ptr to interface control blocks on which to operate on.
  n_links:            Number of phys links to create.

RETURN VALUE
  -1 - error in phys link creation
   0 - on success

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
int ps_phys_link_create
(
  ps_phys_link_type      *this_phys_link_ptr,
  uint8                   n_links
)
{
  uint8 instance;
  ps_phys_link_type     *curr_phys_link_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

  if(this_phys_link_ptr == NULL || n_links == 0)
  {
    DS_UTILS_ASSERT(0);
    PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);

    return -1;
  }

  /*-------------------------------------------------------------------------
    No need to enter critical section as phys links are created at power up
    and PS knows about phys links only after they are created and until then
    only DS context accesses them
  -------------------------------------------------------------------------*/

  for(instance=0; instance < n_links; instance++)
  {
    /*-----------------------------------------------------------------------
      Make sure that the phys link has not already been created.
    -----------------------------------------------------------------------*/
    curr_phys_link_ptr = &(this_phys_link_ptr[instance]);

    if (curr_phys_link_ptr->phys_private.this_phys_link == curr_phys_link_ptr)
    {
      continue;
    }

    LOG_MSG_QTRACE_HFT_HIGH(PS_TAG_NETIFACE, PS_SYS_DEFAULT_SUBS,
                    "ps_phys_link_create(): Creating phys link 0x%x, instance %d",
                    curr_phys_link_ptr, instance);

    /*-----------------------------------------------------------------------
      Reset the private info and initialize all of the queue structures
    -----------------------------------------------------------------------*/
    memset(&(curr_phys_link_ptr->phys_private),
           0,
           sizeof(curr_phys_link_ptr->phys_private));
    curr_phys_link_ptr->phys_private.state = PHYS_LINK_NULL;

    /*-----------------------------------------------------------------------
      clear out al logging related fields
    -----------------------------------------------------------------------*/
    memset(&(curr_phys_link_ptr->dpl_link_cb),
           0,
           sizeof(curr_phys_link_ptr->dpl_link_cb));

    /*-----------------------------------------------------------------------
      Initialize the event queue.
    -----------------------------------------------------------------------*/
    (void)q_init(&(curr_phys_link_ptr->phys_private.event_handle_q));

    curr_phys_link_ptr->phys_private.this_phys_link = curr_phys_link_ptr;
    curr_phys_link_ptr->phys_private.instance       = instance;
    curr_phys_link_ptr->dormancy_info_code          = PS_EIC_NETWORK_NOT_SPECIFIED;

    memset (&curr_phys_link_ptr->event_info_cache,
            0,
            sizeof (phys_link_event_info_cache_type));
    curr_phys_link_ptr->event_info_cache.phys_link_event_info.state =
      PHYS_LINK_NULL;
      
    PS_PHYS_LINK_RESET_BEARER_ID(&this_phys_link_ptr[0]);   
    curr_phys_link_ptr->bearer_id_ex = ps_phys_linki_get_bearer_id_ex();

    
  }

  PS_PHYS_LINKI_SET_CAPABILITY(&this_phys_link_ptr[0],
                               PS_PHYS_LINK_CAPABILITY_PRIMARY);

  this_phys_link_ptr[0].tx_f_ptr = ps_phys_linki_default_tx_cmd;

  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);
  return 0;

} /* ps_phys_link_create() */


/*===========================================================================
FUNCTION PS_PHYS_LINK_DELETE()

DESCRIPTION
  This will memset()s the private data struct to 0, resets the
  "this_phys_link_ptr" to NULL,  and destroys the queues.

PARAMETERS
  this_phys_link_ptr: Ptr to interface control blocks on which to operate on.
  n_links:            Number of phys links to create.

RETURN VALUE
  -1 - error in phys link creation
   0 - on success

DEPENDENCIES
  Currently only 3gpp um ifaces are supposed to call this as they allocate phy
  links dynamically

SIDE EFFECTS
  None
===========================================================================*/
int ps_phys_link_delete
(
  ps_phys_link_type      *this_phys_link_ptr,
  uint8                   n_links
)
{
  uint8                              instance;
  ps_phys_link_type                * curr_phys_link_ptr;
  ps_phys_linki_event_handle_type  * handle_ptr = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

  if(this_phys_link_ptr == NULL || n_links == 0)
  {
    PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);

    return -1;
  }

  for(instance=0; instance < n_links; instance++)
  {
    /*-----------------------------------------------------------------------
      Make sure that the phys link has not already been deleted.
    -----------------------------------------------------------------------*/
    curr_phys_link_ptr = &(this_phys_link_ptr[instance]);

    if ( !(PS_PHYS_LINK_IS_VALID(curr_phys_link_ptr)) )
    {
      LOG_MSG_INFO1_2("ps_phys_link_delete(): "
                      "Already destroyed phys link 0x%p, instance %d",
                      this_phys_link_ptr,
                      this_phys_link_ptr->phys_private.instance);
      continue;
    }

    LOG_MSG_INFO2_2("ps_phys_link_delete(): "
                    "Deleting phys link 0x%p, instance %d",
                    curr_phys_link_ptr, instance);

    /*-----------------------------------------------------------------------
      make sure all the existing handles are invalidated and destroy the 
      event queue.
    -----------------------------------------------------------------------*/
    handle_ptr = q_check(&(curr_phys_link_ptr->phys_private.event_handle_q));

    while( NULL != handle_ptr )
    {
      handle_ptr->is_phys_link_valid = FALSE;

      handle_ptr = q_next(&(curr_phys_link_ptr->phys_private.event_handle_q),
                          &(handle_ptr->link));
    }

    (void)q_destroy(&(curr_phys_link_ptr->phys_private.event_handle_q));

    ps_phys_linki_free_bearer_id_ex(curr_phys_link_ptr->bearer_id_ex);
        
    memset (curr_phys_link_ptr, 0, sizeof (ps_phys_link_type));
  }
  
  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

  return 0;

} /* ps_phys_link_delete() */

/*===========================================================================
                      BRING UP/TEAR DOWN COMMANDS
===========================================================================*/
/*===========================================================================
FUNCTION PS_PHYS_LINK_UP_CMD()

DESCRIPTION
  Brings up the physical link

PARAMETERS
  this_phys_link_ptr: ptr to interface control block on which to operate on.

  THIS FUNCTION SHOULD NOT BE CALLED IN AN ISR.

RETURN VALUE
   0: on success
  -1: on failure (which includes DS_EWOULDBLOCK)

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
int ps_phys_link_up_cmd
(
  ps_phys_link_type *this_phys_link_ptr,
  int16             *ps_errno,
  void              *client_data_ptr
)
{
  ps_iface_event_info_u_type        event_info;
  int                               ret_val = -1;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (ps_errno == NULL)
  {
    DS_UTILS_ERROR_FATAL("ps_phys_link_up_cmd(): NULL parameter is passed");
    return -1;
  }

  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

  if(!(PS_PHYS_LINK_IS_VALID(this_phys_link_ptr)))
  {
    PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
    return(ret_val);
  }

  LOG_MSG_INFO2_2("ps_phys_link_up_cmd(): "
                  "PS PHYS LINK UP CMD 0x%p in state %d",
                  this_phys_link_ptr,
                  PS_PHYS_LINKI_GET_STATE(this_phys_link_ptr));

  /*-------------------------------------------------------------------------
    Take action based on current phys link state
  -------------------------------------------------------------------------*/

  switch(PS_PHYS_LINK_GET_STATE(this_phys_link_ptr))
  {
    case PHYS_LINK_UP:
      /*---------------------------------------------------------------------
        Physical link is up: return success
      ---------------------------------------------------------------------*/
      ret_val = 0;
      break;

    case PHYS_LINK_COMING_UP:
    case PHYS_LINK_RESUMING:

      /*---------------------------------------------------------------------
        Physical link is coming up: return would block
      ---------------------------------------------------------------------*/
      *ps_errno = DS_EWOULDBLOCK;
      ret_val = -1;
      break;

    case PHYS_LINK_DOWN:

     /*----------------------------------------------------------------------
        Physical link is coming up from DORMANCY: if the bring up handler is
        registered set the state to coming up and call it.
      ---------------------------------------------------------------------*/
      if(this_phys_link_ptr->phys_link_up_cmd_f_ptr != NULL)
      {
        event_info.phys_link_event_info.state =
          PS_PHYS_LINK_GET_STATE(this_phys_link_ptr);
        event_info.phys_link_event_info.info_code = PS_EIC_NOT_SPECIFIED;

        PS_PHYS_LINKI_SET_STATE(this_phys_link_ptr, PHYS_LINK_RESUMING);

        if(this_phys_link_ptr->phys_link_up_cmd_f_ptr(this_phys_link_ptr,
                                                      client_data_ptr) < 0)
        {
          /*-----------------------------------------------------------------
            can't bring up the network for some reason, so return net no net
            and set the state to down
          -----------------------------------------------------------------*/
          PS_PHYS_LINKI_SET_STATE(this_phys_link_ptr, PHYS_LINK_DOWN);
          *ps_errno = DS_ENETDOWN;
        }
        else
        {
          /*-----------------------------------------------------------------
            Need to disable flow so that DS_WRITE_EVENT is not posted in
            sockets layer when phys link is in this state
          -----------------------------------------------------------------*/
          ps_phys_link_disable_flow(this_phys_link_ptr,
                                    DS_FLOW_PHYS_LINK_MASK);
          ps_ifacei_invoke_event_cbacks(NULL,
                                        this_phys_link_ptr,
                                        PHYS_LINK_RESUMING_EV,
                                        event_info);

          /*-----------------------------------------------------------------
            bring up is in progress so return would block
          -----------------------------------------------------------------*/
          *ps_errno = DS_EWOULDBLOCK;
        }
      } /* if(f_ptr registered) */

      /*---------------------------------------------------------------------
        The handler is not registered, so return option not supported
      ---------------------------------------------------------------------*/
      else
      {
        *ps_errno = DS_EOPNOTSUPP;
      }
      ret_val = -1;
      break;

    case PHYS_LINK_NULL:

     /*----------------------------------------------------------------------
        Physical link is coming up from NULL state: if the bring up handler
        is registered set the state to coming up and call it.
      ---------------------------------------------------------------------*/
      if(this_phys_link_ptr->phys_link_up_cmd_f_ptr != NULL)
      {
        event_info.phys_link_event_info.state =
          PS_PHYS_LINK_GET_STATE(this_phys_link_ptr);
        event_info.phys_link_event_info.info_code = PS_EIC_NOT_SPECIFIED;

        PS_PHYS_LINKI_SET_STATE(this_phys_link_ptr, PHYS_LINK_COMING_UP);

        if(this_phys_link_ptr->phys_link_up_cmd_f_ptr(this_phys_link_ptr,
                                                      client_data_ptr) < 0)
        {
          /*-----------------------------------------------------------------
            can't bring up the network for some reason, so return net no net
            and set the state back to NULL
          -----------------------------------------------------------------*/
          PS_PHYS_LINKI_SET_STATE(this_phys_link_ptr, PHYS_LINK_NULL);
          *ps_errno = DS_ENETNONET;
        }
        else
        {
          /*-----------------------------------------------------------------
            Need to disable flow so that DS_WRITE_EVENT is not posted in
            sockets layer when phys link is in this state
          -----------------------------------------------------------------*/
          ps_phys_link_disable_flow(this_phys_link_ptr,
                                    DS_FLOW_PHYS_LINK_MASK);
          ps_ifacei_invoke_event_cbacks(NULL,
                                        this_phys_link_ptr,
                                        PHYS_LINK_COMING_UP_EV,
                                        event_info
                                       );

          *ps_errno = DS_EWOULDBLOCK;
        }
      } /* if(f_ptr registered) */

      /*---------------------------------------------------------------------
        The handler is not registered, so return option not supported
      ---------------------------------------------------------------------*/
      else
      {
        *ps_errno = DS_EOPNOTSUPP;
      }
      ret_val = -1;
      break;

    case PHYS_LINK_GOING_DOWN:
      *ps_errno = DS_ENETGOINGDORMANT;
      ret_val = -1;
      break;

    case PHYS_LINK_GOING_NULL:
      /*---------------------------------------------------------------------
        Physical link is going down: return net close in progress
      ---------------------------------------------------------------------*/
      *ps_errno = DS_ENETCLOSEINPROGRESS;
      ret_val = -1;
      break;

    default:
      /*---------------------------------------------------------------------
        This should NEVER happen!
      ---------------------------------------------------------------------*/
      DS_UTILS_ASSERT(0);
      *ps_errno = DS_EOPNOTSUPP;
      ret_val = -1;
      break;
  }

  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

  return(ret_val);

} /* ps_phys_link_up_cmd() */

 
/*===========================================================================
FUNCTION PS_PHYS_LINK_DOWN_CMD()

DESCRIPTION
  This function is used to make the physical link go dormant.

PARAMETERS
  this_phys_link_ptr: ptr to phys link to tear down
  ps_errno: error code to return to caller.
  client_data_ptr: data, that is passed to the client handler function.

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
int ps_phys_link_down_cmd
(
  ps_phys_link_type   *this_phys_link_ptr,
  int16               *ps_errno,
  void                *client_data_ptr
)
{
  ps_iface_event_info_u_type event_info;
  int ret_val = -1;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (ps_errno == NULL)
  {
    DS_UTILS_ERROR_FATAL("ps_phys_link_down_cmd(): NULL parameter is passed");
    return -1;
  }

  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

  if(!(PS_PHYS_LINK_IS_VALID(this_phys_link_ptr)))
  {
    *ps_errno = DS_EINVAL;
    PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);

    return -1;
  }

  LOG_MSG_INFO2_2("ps_phys_link_down_cmd(): "
                  "PS PHYS LINK DOWN CMD 0x%p in state %d",
                  this_phys_link_ptr,
                  PS_PHYS_LINKI_GET_STATE(this_phys_link_ptr));

  switch(PS_PHYS_LINK_GET_STATE(this_phys_link_ptr))
  {
  case PHYS_LINK_DOWN:
    ret_val = 0;
    break;

  case PHYS_LINK_UP:
  case PHYS_LINK_COMING_UP:
  case PHYS_LINK_RESUMING:

    if(this_phys_link_ptr->phys_link_down_cmd_f_ptr != NULL)
    {
      event_info.phys_link_event_info.state =
          PS_PHYS_LINK_GET_STATE(this_phys_link_ptr);
        event_info.phys_link_event_info.info_code = PS_EIC_NOT_SPECIFIED;

      PS_PHYS_LINKI_SET_STATE(this_phys_link_ptr, PHYS_LINK_GOING_DOWN);

      if (this_phys_link_ptr->phys_link_down_cmd_f_ptr(this_phys_link_ptr,
                                                       client_data_ptr) < 0)
      {
        /*-------------------------------------------------------------------
          can't bring down the network for some reason, so return net no net
          and set the state back to
        -------------------------------------------------------------------*/
        PS_PHYS_LINKI_SET_STATE(this_phys_link_ptr,
                                event_info.phys_link_event_info.state);
        /* *ps_errno = ; //what error?  */

      }
      else
      {
        /*-------------------------------------------------------------------
          Need to disable flow so that DS_WRITE_EVENT is not posted in
          sockets layer when phys link is in this state
        -------------------------------------------------------------------*/
        ps_phys_link_disable_flow(this_phys_link_ptr,
                                  DS_FLOW_PHYS_LINK_MASK);
        ps_ifacei_invoke_event_cbacks(NULL,
                                      this_phys_link_ptr,
                                      PHYS_LINK_GOING_DOWN_EV,
                                      event_info
                                     );
        *ps_errno = DS_EWOULDBLOCK;
      }
    }
    else
    {
      *ps_errno = DS_EOPNOTSUPP;
    }
    ret_val = -1;
    break;

  case PHYS_LINK_GOING_DOWN:
    *ps_errno = DS_EWOULDBLOCK;
    ret_val = -1;
    break;

  case PHYS_LINK_GOING_NULL:
    *ps_errno = DS_ENETCLOSEINPROGRESS;
    ret_val = -1;
    break;

  case PHYS_LINK_NULL:
    *ps_errno = DS_ENETDOWN;
    ret_val = -1;
    break;

  default:
    DS_UTILS_ASSERT(0);
    *ps_errno = DS_EOPNOTSUPP;
    ret_val = -1;
    break;

  } /* switch(state) */

  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);
  return ret_val;

} /* ps_phys_link_down_cmd() */

/*===========================================================================
FUNCTION PS_PHYS_LINK_GO_NULL_CMD()

DESCRIPTION
  This function is used to tear down the physical link.

PARAMETERS
  this_phys_link_ptr: ptr to phys link to tear down
  ps_errno: error code to return to caller.
  client_data_ptr: data, that is passed to the client handler function.

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
int ps_phys_link_go_null_cmd
(
  ps_phys_link_type   *this_phys_link_ptr,
  int16               *ps_errno,
  void                *client_data_ptr
)
{
  ps_iface_event_info_u_type event_info;
  int ret_val = -1;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (ps_errno == NULL)
  {
    DS_UTILS_ERROR_FATAL("ps_phys_link_go_null_cmd(): NULL parameter is passed");
    return -1;
  }

  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

  if(!(PS_PHYS_LINK_IS_VALID(this_phys_link_ptr)))
  {
    *ps_errno = DS_EINVAL;
    PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
    
    return -1;
  }

  LOG_MSG_INFO2_2("ps_phys_link_go_null_cmd(): "
                  "PS PHYS LINK GO NULL CMD 0x%p in state %d",
                  this_phys_link_ptr,
                  PS_PHYS_LINKI_GET_STATE(this_phys_link_ptr));

  /*-------------------------------------------------------------------------
    Return SUCCESS immediately if this phys link is still used by other
    clients. This check MUST not be performed if phys link is COUPLED as
    in that case flows are transparent to mode handlers and hence phys link's
    ref cnt will be 1
  -------------------------------------------------------------------------*/
  if (PS_PHYS_LINKI_GET_CAPABILITY(this_phys_link_ptr,
                                   PS_PHYS_LINK_CAPABILITY_FLOW_DECOUPLED))
  {
    if (PS_PHYS_LINKI_GET_REF_CNT(this_phys_link_ptr) > 0)
    {
      PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
      LOG_MSG_INFO2_1("ps_phys_link_go_null_cmd(): "
                      "Flows are still bound to this phys link. Not tearing "
                      "phys link, 0x%p down", this_phys_link_ptr);
      return 0;
    }
  }

  switch (PS_PHYS_LINKI_GET_STATE(this_phys_link_ptr))
  {
  case PHYS_LINK_NULL:
    ret_val = 0;
    break;

  case PHYS_LINK_UP:
  case PHYS_LINK_COMING_UP:
  case PHYS_LINK_RESUMING:
  case PHYS_LINK_GOING_DOWN:
  case PHYS_LINK_DOWN:

    if(this_phys_link_ptr->phys_link_go_null_cmd_f_ptr != NULL)
    {
      event_info.phys_link_event_info.state =
          PS_PHYS_LINK_GET_STATE(this_phys_link_ptr);
        event_info.phys_link_event_info.info_code = PS_EIC_NOT_SPECIFIED;

      PS_PHYS_LINKI_SET_STATE(this_phys_link_ptr, PHYS_LINK_GOING_NULL);

      if (this_phys_link_ptr->phys_link_go_null_cmd_f_ptr(this_phys_link_ptr,
                                                          client_data_ptr) < 0)
      {
        PS_PHYS_LINKI_SET_STATE(this_phys_link_ptr,
                                event_info.phys_link_event_info.state);
        /* *ps_errno =; //error? */
      }
      else
      {
        /*-------------------------------------------------------------------
          Need to disable flow so that DS_WRITE_EVENT is not posted in
          sockets layer when phys link is in this state
        -------------------------------------------------------------------*/
        ps_phys_link_disable_flow(this_phys_link_ptr,
                                  DS_FLOW_PHYS_LINK_MASK);
        ps_ifacei_invoke_event_cbacks(NULL,
                                      this_phys_link_ptr,
                                      PHYS_LINK_GOING_NULL_EV,
                                      event_info
                                     );

        *ps_errno = DS_EWOULDBLOCK;
      }
    }
    else
    {
      *ps_errno = DS_EOPNOTSUPP;
    }
    ret_val = -1;
    break;

  case PHYS_LINK_GOING_NULL:
    *ps_errno = DS_EWOULDBLOCK;
    ret_val = -1;
    break;

  default:
    DS_UTILS_ASSERT(0);
    *ps_errno = DS_EOPNOTSUPP;
    ret_val = -1;
    break;

  } /* switch(state) */

  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);
  return ret_val;

} /* ps_phys_link_go_null_cmd() */


/*===========================================================================
                         FLOW CONTROL COMMANDS
===========================================================================*/
/*===========================================================================
FUNCTION PS_PHYS_LINK_ENABLE_FLOW()

DESCRIPTION
  Interface user enables flow on the interface.

PARAMETERS
  this_phys_link_ptr: ptr to interface control block on which to operate on.
  flow_mask: bit mask that identifies the caller.

  THIS FUNCTION SHOULD NOT BE CALLED IN AN ISR.

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void ps_phys_link_enable_flow
(
  ps_phys_link_type *this_phys_link_ptr,
  uint64             flow_mask
)
{
  ps_iface_event_info_u_type event_info;
  ds3g_flow_e_type           flow_type = DS_FLOW_ENABLE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  memset(&event_info, 0, sizeof(ps_iface_event_info_u_type));

  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

  if( !(PS_PHYS_LINK_IS_VALID(this_phys_link_ptr)) )
  {
    LOG_MSG_ERROR_1("ps_phys_link_enable_flow(): invalid phys_link %p",
                     this_phys_link_ptr );
    PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
   
    return;
  }

  /*-------------------------------------------------------------------------
    store the previous flow mask in the event info variable, and remove the
    client mask from the tx_flow_mask.
  -------------------------------------------------------------------------*/
  event_info.flow_mask = this_phys_link_ptr->phys_private.tx_flow_mask;
  DS_FLOW_CTRL_SET_MASK(flow_type,
                        this_phys_link_ptr->phys_private.tx_flow_mask,
                        flow_mask);

  /*-------------------------------------------------------------------------
    only call the callback on the transition to the flow being enabled
  -------------------------------------------------------------------------*/
  if( (PS_PHYS_LINK_FLOW_ENABLED(this_phys_link_ptr)) &&
      (event_info.flow_mask != DS_FLOW_IS_ENABLED)
    )
  {
    LOG_MSG_INFO2_3("ps_phys_link_enable_flow(): "
                    "client 0x%x enabling flow on phys link 0x%p -> mask 0x%x",
                    flow_mask,
                    this_phys_link_ptr,
                    this_phys_link_ptr->phys_private.tx_flow_mask);

    ps_ifacei_invoke_event_cbacks(NULL,
                                  this_phys_link_ptr,
                                  PHYS_LINK_FLOW_ENABLED_EV,
                                  event_info);
  }

  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

} /* ps_phys_link_enable_flow() */


/*===========================================================================
FUNCTION PS_PHYS_LINK_DISABLE_FLOW()

DESCRIPTION
  client disables flow on the interface.

  THIS FUNCTION SHOULD NOT BE CALLED IN AN ISR.

PARAMETERS
  this_phys_link_ptr: Ptr to interface control block on which to operate on.
  flow_mask: bit mask that identifies the caller.

RETURN VALUE
  None

DEPENDENCIES

SIDE EFFECTS
  None
===========================================================================*/
void ps_phys_link_disable_flow
(
  ps_phys_link_type *this_phys_link_ptr,
  uint64             flow_mask
)
{
  ps_iface_event_info_u_type event_info;
  ds3g_flow_e_type           flow_type = DS_FLOW_DISABLE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  memset(&event_info, 0, sizeof(ps_iface_event_info_u_type));

  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

  if(!(PS_PHYS_LINK_IS_VALID(this_phys_link_ptr)))
  {
    LOG_MSG_ERROR_1("ps_phys_link_disable_flow(): invalid phys_link %p",
                     this_phys_link_ptr );
    PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
    
    return;
  }

  /*-------------------------------------------------------------------------
    store the previous flow mask in the event info variable, and insert the
    client mask into the tx_flow_mask.
  -------------------------------------------------------------------------*/
  event_info.flow_mask = this_phys_link_ptr->phys_private.tx_flow_mask;
  DS_FLOW_CTRL_SET_MASK(flow_type,
                        this_phys_link_ptr->phys_private.tx_flow_mask,
                        flow_mask);

  /*-------------------------------------------------------------------------
    only call the callback on the transition to the flow being disabled
  -------------------------------------------------------------------------*/
  if(event_info.flow_mask == DS_FLOW_IS_ENABLED &&
     this_phys_link_ptr->phys_private.tx_flow_mask != DS_FLOW_IS_ENABLED)
  {
    LOG_MSG_INFO2_3("ps_phys_link_disable_flow(): "
                    "client 0x%x disabling flow on phys link 0x%p -> mask 0x%x",
                    flow_mask,
                    this_phys_link_ptr,
                    this_phys_link_ptr->phys_private.tx_flow_mask);

    ps_ifacei_invoke_event_cbacks(NULL,
                                  this_phys_link_ptr,
                                  PHYS_LINK_FLOW_DISABLED_EV,
                                  event_info);
  }

  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

} /* ps_phys_link_disable_flow() */

/*===========================================================================
FUNCTION PS_PHYS_LINKI_ENABLE_DISABLE_FLOW_EX()

DESCRIPTION
  Helper function which posts a cmd to PS task to either disable/enable flow on the phys link

  THIS FUNCTION SHOULD NOT BE CALLED IN AN ISR.

PARAMETERS
  phys_link_ptr: Ptr to phys link on which to operate on.
  flow_mask: bit mask that identifies the caller.
  flow_param: boolean to decide whether to enable or disable.

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void ps_phys_linki_enable_disable_flow_ex
(
  ps_phys_link_type  * phys_link_ptr,
  uint64               flow_mask,
  boolean              flow_param
)
{
  ps_phys_linki_flow_ctrl_cmd_info_type  * flow_ctrl_info_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  PS_SYSTEM_HEAP_MEM_ALLOC_ASSERT(flow_ctrl_info_ptr, 
                                  sizeof(ps_phys_linki_flow_ctrl_cmd_info_type),
                                  ps_phys_linki_flow_ctrl_cmd_info_type*);

  if (NULL == flow_ctrl_info_ptr)
  {
    return;
  }
  memset(flow_ctrl_info_ptr, 0x0, sizeof(ps_phys_linki_flow_ctrl_cmd_info_type));

  flow_ctrl_info_ptr->cmd_type         = PS_PHY_LINK_INTERNAL_CMD_LEGACY_TYPE;
  flow_ctrl_info_ptr->ps_phys_link_ptr = phys_link_ptr;
  flow_ctrl_info_ptr->flow_mask        = flow_mask;
  flow_ctrl_info_ptr->is_enabled       = flow_param;

  ps_send_cmd(PS_PHYS_LINK_FLOW_CTRL_CMD, flow_ctrl_info_ptr);
  return;

} /* ps_phys_linki_enable_disable_flow_ex() */

/*===========================================================================
FUNCTION PS_PHYS_LINK_ENABLE_FLOW_EX()

DESCRIPTION
  Posts a cmd to PS task enable flow on the phys link

  THIS FUNCTION SHOULD NOT BE CALLED IN AN ISR.

PARAMETERS
  phys_link_ptr: Ptr to phys link on which to operate on.
  flow_mask: bit mask that identifies the caller.

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void ps_phys_link_enable_flow_ex
(
  ps_phys_link_type  * phys_link_ptr,
  uint64               flow_mask
)
{
  ps_phys_linki_enable_disable_flow_ex(phys_link_ptr, flow_mask, TRUE);
  return;

} /* ps_phys_link_enable_flow_ex() */

/*===========================================================================
FUNCTION PS_PHYS_LINK_DISABLE_FLOW_EX()

DESCRIPTION
  Posts a cmd to PS task disable flow on the phys link

  THIS FUNCTION SHOULD NOT BE CALLED IN AN ISR.

PARAMETERS
  phys_link_ptr: Ptr to phys link on which to operate on.
  flow_mask: bit mask that identifies the caller.

RETURN VALUE
  None

DEPENDENCIES

SIDE EFFECTS
  None
===========================================================================*/
void ps_phys_link_disable_flow_ex
(
  ps_phys_link_type  * phys_link_ptr,
  uint64               flow_mask
)
{
  ps_phys_linki_enable_disable_flow_ex(phys_link_ptr, flow_mask, FALSE);
  return;

} /* ps_phys_link_disable_flow_ex() */

#ifdef FEATURE_DATA_FLOW_MGMT
void  ps_phys_linki_is_report_flow_ctl
(
  ps_phys_link_type                      * phys_link_ptr,
  ps_phys_link_flow_ctl_cmd_params_type  * flow_ctrl_info_ptr,
  boolean                                * report_legacy_flow_ctl_flag,
  boolean                                * report_ul_burst_flow_ctl_flag
)
{
  ps_phys_link_type             *this_phys_link_ptr = phys_link_ptr;
  uint64                         prev_flow_mask;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-----------------------------------------------------------------------
     Flow enable Suppress logic
   -----------------------------------------------------------------------*/
  if ( TRUE == flow_ctrl_info_ptr->is_flow_enable)
  {

     /*-------------------------------------------------------------------------
      Suppress flow  event when FLOW is being disabled/enabled due to iface flow enable 
      and disable. Iface flow enable/disable should be handled by client (e.g DFC) as flow disable
      can happen without phylink attached(e.g Logical iface flow controlled while handoff and srt down)
    -------------------------------------------------------------------------*/
    if ( DS_FLOW_PHYS_LINK_IFACE_MASK == flow_ctrl_info_ptr->flow_mask )
    {
      return;
    }
     /*-------------------------------------------------------------------------
      Update flow mask
    -------------------------------------------------------------------------*/
     prev_flow_mask = this_phys_link_ptr->phys_private.tx_flow_mask;
     DS_FLOW_CTRL_SET_MASK(DS_FLOW_ENABLE,
                           this_phys_link_ptr->phys_private.tx_flow_mask,
                           flow_ctrl_info_ptr->flow_mask);
    
    /*-------------------------------------------------------------------------
      Mask Transition to the flow being enabled
   -------------------------------------------------------------------------*/
    if ( (PS_PHYS_LINK_FLOW_ENABLED(this_phys_link_ptr)) &&
         (prev_flow_mask != DS_FLOW_IS_ENABLED )
       )
    {
      *report_legacy_flow_ctl_flag   = TRUE;
      *report_ul_burst_flow_ctl_flag  = TRUE;
      return ;
    }
    /*-------------------------------------------------------------------------
       In case of UL burst based flow control, MH can keep requesting N number of bytes while flow
       is already enabled hence checking current flow state as previous can  be enabled
    -------------------------------------------------------------------------*/
    if ( PS_PHYS_LINK_FLOW_ENABLED(this_phys_link_ptr) )
    {
      *report_ul_burst_flow_ctl_flag  = TRUE;
      return;
    }
    
  }
  /*-----------------------------------------------------------------------
     Flow Disable Suppress logic
   -----------------------------------------------------------------------*/
  else
  {  
    /*-------------------------------------------------------------------------
      Suppress flow ent event when FLOW is being disabled/enabled due to iface flow enable 
      and disable. Iface flow enable/disable should be handled by client (e.g DFC) as flow disable
      can happen without phylink attached(e.g Logical iface flow controlled while handoff and srt down)
    -------------------------------------------------------------------------*/
    if ( DS_FLOW_PHYS_LINK_IFACE_MASK == flow_ctrl_info_ptr->flow_mask )
    {
      return;
    }

    /*-------------------------------------------------------------------------
        Update flow mask
       -------------------------------------------------------------------------*/
    prev_flow_mask = this_phys_link_ptr->phys_private.tx_flow_mask;
    DS_FLOW_CTRL_SET_MASK(DS_FLOW_DISABLE,
                          this_phys_link_ptr->phys_private.tx_flow_mask,
                          flow_ctrl_info_ptr->flow_mask);

    /*-------------------------------------------------------------------------
     Mask transition to the flow being enabled
   -------------------------------------------------------------------------*/
    if ( prev_flow_mask == DS_FLOW_IS_ENABLED &&
         this_phys_link_ptr->phys_private.tx_flow_mask != DS_FLOW_IS_ENABLED )
    {
      *report_legacy_flow_ctl_flag   = TRUE;
       /*-------------------------------------------------------------------------
           When Flow is disabled due to watermark high then dont notify to legacy client
      -------------------------------------------------------------------------*/
      if ( DS_FLOW_LTE_PDCP_HGH_MASK != flow_ctrl_info_ptr->flow_mask )
      {
        *report_ul_burst_flow_ctl_flag  = TRUE;
      }
      return;
    }
    /*-------------------------------------------------------------------------
      When flow was disabled due watermark high and now DNE flow control is recevied then
      Notify to new clients
     -------------------------------------------------------------------------*/
    if ( DS_FLOW_LTE_PDCP_HGH_MASK == prev_flow_mask  &&
         DS_FLOW_LTE_PDCP_DNE_MASK == flow_ctrl_info_ptr->flow_mask )
    {
      *report_ul_burst_flow_ctl_flag  = TRUE;
    }
    
    /*-------------------------------------------------------------------------
      In case of ACK required; Send Indication to AP so that AP can acknowledge
     -------------------------------------------------------------------------*/
   if ( FALSE == this_phys_link_ptr->event_info_cache.ack_required &&
        TRUE  ==  flow_ctrl_info_ptr->ack_required )
   {
     *report_ul_burst_flow_ctl_flag  = TRUE;
      return;
   }
  }
  return ;
}/*  ps_phys_linki_is_report_flow_ctl */


void ps_phy_linki_process_ul_burst_flow_ctl_cmd
(
  ps_phys_linki_flow_ctrl_cmd_info_type  * flow_ctrl_info_ptr
)
{
  ps_iface_event_info_u_type               event_info;
  ps_phys_link_type                       *this_phys_link_ptr = NULL;
  boolean                                  report_legacy_flow_ctl_flag = FALSE;
  boolean                                  report_ul_burst_flow_ctl_flag  = FALSE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  memset(&event_info, 0, sizeof(ps_iface_event_info_u_type));

  /*-----------------------------------------------------------------------
    1. Send indication to new clients  
       1.1 Cached allowed number of bytes and Ack required to suppress for "0" number of bytes
    2. Send indication to legacy client
  -----------------------------------------------------------------------*/
  this_phys_link_ptr = flow_ctrl_info_ptr->ps_phys_link_ptr;
    
  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);
  /*-----------------------------------------------------------------------
    Validation check should be  under ps critical section because MH can free ps phys
    link (same critical section is used before free) while PS is handling command.
  -----------------------------------------------------------------------*/
  if( !(PS_PHYS_LINK_IS_VALID(this_phys_link_ptr)) )
  {
    LOG_MSG_ERROR_1("ps_phy_linki_process_ul_burst_flow_ctl_cmd(): invalid phys_link %p",
                     this_phys_link_ptr );
    PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);
    return;
  }

  event_info.flow_ctl_params.ack_required         = flow_ctrl_info_ptr->ack_required;
  event_info.flow_ctl_params.allowed_num_of_bytes = flow_ctrl_info_ptr->allowed_num_bytes;
  event_info.flow_mask                            = flow_ctrl_info_ptr->flow_mask;
  report_legacy_flow_ctl_flag                     = flow_ctrl_info_ptr->report_legacy_flow_ctl_flag;
  report_ul_burst_flow_ctl_flag                   = flow_ctrl_info_ptr->report_ul_burst_flow_ctl_flag;
  
  event_info.flow_ctl_params.flow_ctl_reason      = flow_ctrl_info_ptr->flow_ctl_reason;
  event_info.flow_ctl_params.traffic_type         = flow_ctrl_info_ptr->traffic_type;
  /*-----------------------------------------------------------------------
    Suppress logging for ULB if no information changed from previous notification
    e.g. when MH sends the same info due to timer expiry
  -----------------------------------------------------------------------*/
  if ( this_phys_link_ptr             != ps_phys_link_prev_log_status.phys_link_ptr ||
       flow_ctrl_info_ptr->is_enabled != ps_phys_link_prev_log_status.is_enabled ||
       flow_ctrl_info_ptr->flow_mask  != ps_phys_link_prev_log_status.reported_mask ||
       report_legacy_flow_ctl_flag    != ps_phys_link_prev_log_status.report_legacy_flag ||
       report_ul_burst_flow_ctl_flag  != ps_phys_link_prev_log_status.report_ulb_flag )
  {
  LOG_MSG_INFO1_9("ps_phy_linki_process_ul_burst_flow_ctl_cmd(): "
                  "phys link 0x%p prev mask 0x%x current mask 0x%x recv mask 0x%x "
                  "is_enable %d  num bytes %d ack %d legacy %d  ulb flag %d ",
                  this_phys_link_ptr,
                  flow_ctrl_info_ptr->prev_mask,
                  this_phys_link_ptr->phys_private.tx_flow_mask,
                  flow_ctrl_info_ptr->flow_mask,
                  flow_ctrl_info_ptr->is_enabled,
                  event_info.flow_ctl_params.allowed_num_of_bytes,
                  event_info.flow_ctl_params.ack_required,
                  report_legacy_flow_ctl_flag,
                  report_ul_burst_flow_ctl_flag);

    ps_phys_link_prev_log_status.phys_link_ptr      = this_phys_link_ptr;
    ps_phys_link_prev_log_status.is_enabled         = flow_ctrl_info_ptr->is_enabled;
    ps_phys_link_prev_log_status.reported_mask      = flow_ctrl_info_ptr->flow_mask;
    ps_phys_link_prev_log_status.report_legacy_flag = report_legacy_flow_ctl_flag;  
    ps_phys_link_prev_log_status.report_ulb_flag    = report_ul_burst_flow_ctl_flag;
    ps_phys_link_prev_log_status.suppress_logging_cnt = 0;
    
  }
  else
  {
     ps_phys_link_prev_log_status.suppress_logging_cnt++;
  }
  /*-----------------------------------------------------------------------
    Step 1 : New Flow control event reporting
  -----------------------------------------------------------------------*/
  if ( TRUE == report_ul_burst_flow_ctl_flag)
  {
    ps_ifacei_invoke_event_cbacks(NULL,
                                  this_phys_link_ptr,
                                  IFACE_PHYS_LINK_FLOW_CONTROL_EV,
                                  event_info);
    this_phys_link_ptr->event_info_cache.ack_required=
      event_info.flow_ctl_params.ack_required;
  }
  /*-----------------------------------------------------------------------
    Step 2 : Legacy Flow control event reporting
  -----------------------------------------------------------------------*/
  if ( TRUE == report_legacy_flow_ctl_flag )
  {
    ps_ifacei_invoke_event_cbacks(NULL,
                                  this_phys_link_ptr,
                                  (( TRUE == flow_ctrl_info_ptr->is_enabled) ? 
                                     PHYS_LINK_FLOW_ENABLED_EV : PHYS_LINK_FLOW_DISABLED_EV),
                                  event_info);
  }
  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);
}/* ps_phy_linki_process_ul_burst_flow_ctl_cmd */
#endif /* FEATURE_DATA_FLOW_MGMT */
/*===========================================================================
FUNCTION PS_PHYS_LINK_FLOW_CTRL_CMD_HANDLER()

DESCRIPTION
  This function processes PS_PHYS_LINK_FLOW_CTRL_CMD.

PARAMETERS
  ps_cmd          : Must be PS_PHYS_LINK_FLOW_CTRL_CMD.
  ps_cmd_data_ptr : cmd data

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ps_phys_link_flow_ctrl_cmd_handler
(
  ps_cmd_enum_type    ps_cmd,
  void              * ps_cmd_data_ptr
)
{
  ps_phys_linki_flow_ctrl_cmd_info_type  * flow_ctrl_info_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (NULL == ps_cmd_data_ptr)
  {
    LOG_MSG_ERROR_INT_1("NULL cmd data, cmd %d", ps_cmd);
    return;
  }

  do
  {
    flow_ctrl_info_ptr =
      (ps_phys_linki_flow_ctrl_cmd_info_type *) ps_cmd_data_ptr;

    if (PS_PHYS_LINK_FLOW_CTRL_CMD != ps_cmd)
    {
      LOG_MSG_ERROR_INT_1("Invalid cmd %d", ps_cmd);
      break;
    }

    switch ( flow_ctrl_info_ptr->cmd_type )
    {
      case  PS_PHY_LINK_INTERNAL_CMD_LEGACY_TYPE:
      {
        if (TRUE == flow_ctrl_info_ptr->is_enabled)
        {
          ps_phys_link_enable_flow(flow_ctrl_info_ptr->ps_phys_link_ptr,
                               flow_ctrl_info_ptr->flow_mask);
        }
        else
        {
           ps_phys_link_disable_flow(flow_ctrl_info_ptr->ps_phys_link_ptr,
                                flow_ctrl_info_ptr->flow_mask);
        }
      }
      break;
#ifdef FEATURE_DATA_FLOW_MGMT
      case  PS_PHY_LINK_INTERNAL_CMD_UL_BURST_TYPE:
      {
        ps_phy_linki_process_ul_burst_flow_ctl_cmd( flow_ctrl_info_ptr );
      }
      break;
#endif /*FEATURE_DATA_FLOW_MGMT*/
      default:
      {
        LOG_MSG_ERROR_1("Invalid cmd %d", ps_cmd);
        break;
      }
    }
  } while (0);

  PS_SYSTEM_HEAP_MEM_FREE(flow_ctrl_info_ptr);
  return;

} /* ps_phys_link_flow_ctrl_cmd_handler() */

#ifdef FEATURE_DATA_FLOW_MGMT
/*===========================================================================
FUNCTION PS_PHYS_LINK_FLOW_CTL_CMD()

DESCRIPTION
  Posts a cmd to PS task for flow control on the phys link

  THIS FUNCTION SHOULD NOT BE CALLED IN AN ISR.

PARAMETERS
  phys_link_ptr:             Ptr to phys link on which to operate on.
  flow_ctl_params           Flow control parameters

RETURN VALUE
  None

DEPENDENCIES

SIDE EFFECTS
  None
===========================================================================*/
void ps_phys_link_flow_ctl_cmd
(
  ps_phys_link_type                     * phys_link_ptr,
  ps_phys_link_flow_ctl_cmd_params_type   flow_ctl_params
)
{

  ps_phys_linki_flow_ctrl_cmd_info_type  * flow_ctrl_info_ptr;
  boolean                                  report_legacy_flow_ctl_flag = FALSE;
  boolean                                  report_ul_burst_flow_ctl_flag  = FALSE;
  uint64                                   prev_mask = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
 if( !(PS_PHYS_LINK_IS_VALID(phys_link_ptr)) )
 {
   LOG_MSG_ERROR_1("ps_phys_link_flow_ctl_cmd(): invalid phys_link %p",
                    phys_link_ptr );
   return;
 }

 do
 {
    prev_mask = phys_link_ptr->phys_private.tx_flow_mask;

    PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

    ps_phys_linki_is_report_flow_ctl ( phys_link_ptr,
                                       &flow_ctl_params, 
                                       &report_legacy_flow_ctl_flag, 
                                       &report_ul_burst_flow_ctl_flag);
    
    PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

    if ( TRUE == report_legacy_flow_ctl_flag )
    {
      break;
    }

    if ( (TRUE == report_ul_burst_flow_ctl_flag )&& 
         (TRUE == PS_PHYS_LINKI_IS_FLOW_EVENT_REG(PS_PHYS_LINKI_FLOW_EVENT_REG_ULB)) )
    {
      break;
    }
    //No event reporting

    LOG_MSG_INFO1_5(" ps_phys_link_flow_ctl_cmd Suppress evt legacy %d ulb %d prev_mask %d "
       "is enable %d req mask 0x%x", 
       report_legacy_flow_ctl_flag, report_ul_burst_flow_ctl_flag, prev_mask, 
       flow_ctl_params.is_flow_enable, flow_ctl_params.flow_mask);
    
    return;
  }while(0);

  
  PS_SYSTEM_HEAP_MEM_ALLOC_ASSERT(flow_ctrl_info_ptr, 
                                  sizeof(ps_phys_linki_flow_ctrl_cmd_info_type),
                                  ps_phys_linki_flow_ctrl_cmd_info_type*);
  
  if (NULL == flow_ctrl_info_ptr)
  {
    return;
  }
  memset(flow_ctrl_info_ptr, 0x0, sizeof(ps_phys_linki_flow_ctrl_cmd_info_type));
  
  flow_ctrl_info_ptr->cmd_type         = PS_PHY_LINK_INTERNAL_CMD_UL_BURST_TYPE;
  flow_ctrl_info_ptr->ps_phys_link_ptr = phys_link_ptr;
  flow_ctrl_info_ptr->flow_mask        = flow_ctl_params.flow_mask;
  flow_ctrl_info_ptr->allowed_num_bytes= flow_ctl_params.allowed_num_of_bytes;
  flow_ctrl_info_ptr->ack_required     = flow_ctl_params.ack_required;
  flow_ctrl_info_ptr->is_enabled       =  flow_ctl_params.is_flow_enable;
  flow_ctrl_info_ptr->report_legacy_flow_ctl_flag = report_legacy_flow_ctl_flag;
  flow_ctrl_info_ptr->report_ul_burst_flow_ctl_flag = report_ul_burst_flow_ctl_flag;
  flow_ctrl_info_ptr->prev_mask                     = prev_mask;
  
  flow_ctrl_info_ptr->traffic_type     =  flow_ctl_params.traffic_type;
  flow_ctrl_info_ptr->flow_ctl_reason  =  flow_ctl_params.flow_ctl_reason;
  ps_send_cmd(PS_PHYS_LINK_FLOW_CTRL_CMD, flow_ctrl_info_ptr);

  return;
}/* ps_phys_link_flow_ctl_cmd */
#endif /* FEATURE_DATA_FLOW_MGMT*/

void ps_phys_link_bearer_ids_ex_init
(
  void
)
{
  memset(&ps_phys_link_bearer_ids_ex, 0x0, sizeof(ps_phys_link_bearer_ids_ex));
  memset(&ps_phys_link_prev_log_status, 0x0, sizeof(ps_phys_link_prev_log_status));
  
}/* ps_phys_link_bearer_ids_ex_init */

