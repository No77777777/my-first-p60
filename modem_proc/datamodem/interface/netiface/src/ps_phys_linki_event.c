/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                    P S _ P H Y S _ L I N K I _ E V E N T . C


GENERAL DESCRIPTION
  This is the implementation of the PS phys link event functions. This
  file implement functions used for phys_link events.

EXTERNAL FUNCTIONS

  PS_PHYS_LINK_EVENT_INIT()
    Called at power-up to initialize event arrays, etc.

  PS_PHYS_LINK_ALLOC_EVENT_CBACK_BUF
    Called by client to allocate a buffer used when registering for phys link
    events.

  PS_PHYS_LINK_FREE_EVENT_CBACK_BUF
    Called by client to free a buffer which was used when registering for
    phys link events.

  PS_PHYS_LINK_EVENT_CBACK_REG
    Called by client when registering for phys link events.

  PS_PHYS_LINK_EVENT_CBACK_DEREG
    Called by client when de-registering for phys link events.

  PS_PHYS_LINK_DOWN_IND
    Can only be called by phys link owner!!!  Indicates that the phys link
    down.

  PS_PHYS_LINK_UP_IND
    Can only be called by phys link owner!!!  Indicates that the phys link
    up.

  PS_PHYS_LINK_GON_IND
    Can only be called by phys link owner!!!  Indicates that the phys link
    has gone NULL (i.e, physical layer indicates end of the data session).

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Must call ps_phys_link_event_init() at startup.

Copyright (c) 2003-2014 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                            EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/netiface/src/ps_phys_linki_event.c#3 $
  $Author: pwbldsvc $ $DateTime: 2021/05/25 01:37:25 $

when        who    what, where, why
--------    ---    ---------------------------------------------------------- 
11/30/11    su     Adding log code support for DPL logging  
12/08/10    rp     RAM optimization changes.
06/19/10    vs     Increasing phys link ev buffer due to QMI Dual IP support
03/26/09    pp     CMI De-featurization.
02/01/07    msr    Added ps_phys_link_dos_ack_ind()
09/12/06    msr    Removed redundant state field in event_info structure
05/10/06    rt     Fixed a typo in ps_phys_link_down_ind_ex().
04/25/06    msr    L4/Tasklock code review changes
02/22/06    msr    Using single critical section
02/06/06    msr    Updated for L4 tasklock/crit sections.
10/18/05    msr    Removed support for FLOW_ACTIVATE_MODIFIED_EV and
                   PHYS_LINK_UP_MODIFIED_EV
08/16/05    msr    Fixed PS_BRANCH_TASKFREE()
05/12/05    mct    Lint changes.
04/17/05    msr    Changed ps_phys_link_gone_ind() to ensure that all flows
                   are released.
04/16/05    ks     Added changes for PHYS_LINK_NULL state.
11/19/04    msr    Added F3 messages to indications.
11/17/04    ks     Setting event_info.link_state to PHYS_LINK_DOWN for
                   PHYS_LINK_GONE_EV in PS_PHYS_LINK_EVENT_CBACK_REG().
11/02/04    msr    Checking if ps_mem_get_buf succeeded in
                   ps_phys_link_alloc_event_cback_buf().
10/06/04    sv     Added phys_link_down_ind_ex function.
08/12/04    sv     Enable physlink flow upon up/down_ind()
05/10/04    mct    Fixed lint errors.
05/21/04    aku    Notify clients about PHYS_LINK_DOWN_EV when processing the
                   GONE indication.
12/30/03    ak     Extended for IFACE/PHYS LINK separation.
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
#include "amssassert.h"
#include "err.h"
#include "msg.h"
#include "ps_utils.h"
#include "ps_crit_sect.h"
#include "ps_iface_defs.h"
#include "ds_flow_control.h"
#include "ps_mem.h"
#include "ps_system_heap.h"
#include "ps_phys_link.h"
#include "ps_phys_linki_event.h"
#include "ps_ifacei_event.h"
#include "queue.h"
#include "ds_Utils_DebugMsg.h"

#include "ps_logging_diag.h"


/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

                           LOCAL DATA DECLARATIONS

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/

typedef struct
{
  ps_phys_link_event_cback_type  event_cback_f_ptr;
  void                         * user_data_ptr;
} ps_phys_linki_event_cb_info_type;

/*---------------------------------------------------------------------------
  GLOBAL_PHYS_LINK_EVENT_HANDLE_Q - this is used to store callbacks that are
    intended to be called when events happen on ANY phys_link.
---------------------------------------------------------------------------*/
q_type  global_phys_link_event_handle_q;

#ifdef FEATURE_DATA_FLOW_MGMT
  static ps_phys_linki_flow_event_reg_type   ps_phys_linki_flow_event_reg = PS_PHYS_LINKI_FLOW_EVENT_REG_NONE;
#endif /* FEATURE_DATA_FLOW_MGMT */

/*===========================================================================

                             INTERNAL FUNCTIONS

===========================================================================*/
/*===========================================================================
FUNCTION PS_PHYS_LINKI_IS_EVENT_SET()

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
boolean ps_phys_linki_is_event_set
( 
  ps_phys_linki_event_handle_type  * event_handle, 
  ps_iface_event_enum_type           event 
)  
{
  uint32 result = 0;
  if (event >= IFACE_MIN_EV && event < PHYS_LINK_MAX_EV )
  {
    result = (event_handle)->event_mask & (uint32)(1 << (event - IFACE_MIN_EV));
  }

  if(event >= PHYS_LINK_EX_MIN_EV && event < PHYS_LINK_EX_MAX_EV)
  {
    result = (event_handle)->event_mask & (uint32)(1 << (event - (PHYS_LINK_EX_MIN_EV - PHYS_LINK_MAX_EV)));
  }
  if (result)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
} /* ps_phys_linki_is_event_set */

/*===========================================================================
FUNCTION PS_PHYS_LINKI_SET_EVENT_BIT()

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
void ps_phys_linki_set_event_bit
( 
  ps_phys_linki_event_handle_type  * event_handle, 
  ps_iface_event_enum_type           event 
)                 
{
  if (event >= IFACE_MIN_EV && event < PHYS_LINK_MAX_EV )
  {
    event_handle->event_mask |= (uint32)(1 << (event - IFACE_MIN_EV));
  }
  if(event >= PHYS_LINK_EX_MIN_EV && event < PHYS_LINK_EX_MAX_EV)
  {
    event_handle->event_mask |= (uint32)(1 << (event - (PHYS_LINK_EX_MIN_EV - PHYS_LINK_MAX_EV)));
  }
  
} /* ps_phys_linki_set_event_bit */

/*===========================================================================
FUNCTION PS_PHYS_LINKI_UNSET_EVENT_BIT()

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
void ps_phys_linki_unset_event_bit
( 
  ps_phys_linki_event_handle_type  * event_handle, 
  ps_iface_event_enum_type           event 
)                 
{
  if (event >= IFACE_MIN_EV && event < PHYS_LINK_MAX_EV )
  {
  event_handle->event_mask &=  ~((uint32)(1 << (event - IFACE_MIN_EV)));
  }
  if(event >= PHYS_LINK_EX_MIN_EV && event < PHYS_LINK_EX_MAX_EV)
  {
    event_handle->event_mask &= ~(uint32)(1 << (event - PHYS_LINK_EX_MIN_EV + PHYS_LINK_MAX_EV));
  }
  
} /* ps_phys_linki_unset_event_bit */

/*===========================================================================
FUNCTION PS_PHYS_LINKI_COMPARE_EVENT_HANDLE()

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
static int ps_phys_linki_compare_event_handle
( 
  void *item_ptr,
  void *compare_val 
)
{
  ps_phys_linki_event_handle_type  *evt_handle = (ps_phys_linki_event_handle_type*)item_ptr;
  ps_phys_linki_event_cb_info_type *cb_info = (ps_phys_linki_event_cb_info_type*)compare_val;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if ((evt_handle->event_cback_f_ptr == cb_info->event_cback_f_ptr) &&
      (evt_handle->user_data_ptr == cb_info->user_data_ptr))
  {
    return TRUE;
  }
  return FALSE;
} /* ps_phys_linki_compare_event_handle */

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

                             EXTERNAL FUNCTIONS

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
/*===========================================================================
FUNCTION PS_PHYS_LINK_EVENT_INIT()

DESCRIPTION
  This function initializes the global queues for ps_phys_link.  It also
  registers the ps_iface phys_link function on the global phys_link queue.

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
void ps_phys_link_event_init
(
  void
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Initialize global event queue
  -------------------------------------------------------------------------*/
  (void)q_init(&global_phys_link_event_handle_q);
} /* ps_phys_link_event_init() */

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
                                 INDICATIONS

  These are intended to only be called by the interface clents/owners.  While
  there is no way to enforce this, having someone else call the indications
  is not supported behavior.
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
/*===========================================================================
FUNCTION PS_PHYS_LINK_DOWN_IND()

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
void ps_phys_link_down_ind
(
  ps_phys_link_type *this_phys_link_ptr
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  ps_phys_link_down_ind_ex(this_phys_link_ptr, PS_EIC_NOT_SPECIFIED);

} /* ps_phys_link_down_ind() */



/*===========================================================================
FUNCTION PS_PHYS_LINK_UP_IND()

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
void ps_phys_link_up_ind
(
  ps_phys_link_type *this_phys_link_ptr
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  ps_phys_link_up_ind_ex(this_phys_link_ptr, PS_EIC_NOT_SPECIFIED);
  return;
} /* ps_phys_link_up_ind() */


/*===========================================================================
FUNCTION PS_PHYS_LINK_GONE_IND()

DESCRIPTION
  The physical link of the given interface has been aborted, indicating
  that the session has gone NULL.  This is called in lieu of
  phys link_down_ind().

  THIS FUNCTION SHOULD NOT BE CALLED IN A ISR.

PARAMETERS
  this_phys_link_ptr: ptr to interface control block on which to operate
  on.

RETURN VALUE
  None

DEPENDENCIES

SIDE EFFECTS
  Note that the clients that have registered for the PHYS_LINK_DOWN callback
  also get notified that the phys link has transitioned to the down state.
===========================================================================*/
void ps_phys_link_gone_ind
(
  ps_phys_link_type *this_phys_link_ptr
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  ps_phys_link_gone_ind_ex(this_phys_link_ptr, PS_EIC_NOT_SPECIFIED);
  return;
} /* ps_phys_link_gone_ind() */



/*===========================================================================
FUNCTION PS_PHYS_LINK_DOS_ACK_IND()

DESCRIPTION
  Indicates that a packet sent over an access channel is acknowledged by the
  network

  THIS FUNCTION SHOULD NOT BE CALLED IN A ISR.

PARAMETERS
  phys_link_ptr           : ptr to a phys link
  dos_ack_handle          : handle to the DOS packet
  dos_ack_status_info_ptr : status of DOS transmission

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void ps_phys_link_dos_ack_ind
(
  ps_phys_link_type                      * phys_link_ptr,
  int32                                    dos_ack_handle,
  ps_phys_link_dos_ack_status_info_type  * dos_ack_status_info_ptr
)
{
  ps_iface_event_info_u_type  event_info;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

  if (!(PS_PHYS_LINK_IS_VALID(phys_link_ptr)))
  {
    LOG_MSG_ERROR_1("ps_phys_link_dos_ack_ind(): invalid phys_link %p", 
                    phys_link_ptr );
    PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
          
    return;
  }

  LOG_MSG_INFO2_1("ps_phys_link_dos_ack_ind(): "
                  "PHYS LINK DOS ACK IND 0x%p", phys_link_ptr);

  memset( &event_info, 0, sizeof(event_info) );

  event_info.dos_ack_info.handle      = dos_ack_handle;
  event_info.dos_ack_info.status_info = *dos_ack_status_info_ptr;

  ps_ifacei_invoke_event_cbacks( NULL,
                                 phys_link_ptr,
                                 PHYS_LINK_707_DOS_ACK_EV,
                                 event_info);

  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

} /* ps_phys_link_dos_ack_ind() */



/*===========================================================================
FUNCTION PS_PHYS_LINK_DOWN_IND_EX()

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
void ps_phys_link_down_ind_ex
(
  ps_phys_link_type *this_phys_link_ptr,
  ps_extended_info_code_enum_type info_code
)
{
  ps_iface_event_info_u_type event_info;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

  if (!(PS_PHYS_LINK_IS_VALID(this_phys_link_ptr)))
  {
    LOG_MSG_ERROR_1("ps_phys_link_down_ind_ex(): invalid phys_link %p",
                     this_phys_link_ptr );
    PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
          
    return;
  }

  LOG_MSG_INFO2_2("ps_phys_link_down_ind_ex(): "
                  "PHYS LINK DOWN IND 0x%p, state %d",
                  this_phys_link_ptr,
                  PS_PHYS_LINKI_GET_STATE(this_phys_link_ptr));

  /*-------------------------------------------------------------------------
    store the previous state, change the current link state to DOWN and call
    the callbacks.
  -------------------------------------------------------------------------*/
  event_info.phys_link_event_info.state =
    PS_PHYS_LINK_GET_STATE(this_phys_link_ptr);
  event_info.phys_link_event_info.info_code = info_code;

  if (PS_PHYS_LINKI_GET_CAPABILITY(this_phys_link_ptr,
                                   PS_PHYS_LINK_CAPABILITY_PRIMARY))
  {
    this_phys_link_ptr->dormancy_info_code = info_code;
  }

  PS_PHYS_LINKI_SET_STATE(this_phys_link_ptr, PHYS_LINK_DOWN);

  ps_ifacei_invoke_event_cbacks( NULL,
                                 this_phys_link_ptr,
                                 PHYS_LINK_DOWN_EV,
                                 event_info);

  /*-------------------------------------------------------------------------
    Need to enable flow so that DS_WRITE_EVENT can be posted in sockets
    layer when phys link is in this state
  -------------------------------------------------------------------------*/
  ps_phys_link_enable_flow(this_phys_link_ptr, DS_FLOW_PHYS_LINK_MASK);

  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

} /* ps_phys_link_down_ind_ex() */

/*===========================================================================
FUNCTION PS_PHYS_LINK_UP_IND_EX()

DESCRIPTION
  The physical link of the given interface is up.  Call callbacks.

  THIS FUNCTION SHOULD NOT BE CALLED IN A ISR.

PARAMETERS
  this_iface_ptr: ptr to interface control block on which to operate on.
  info_code:  Extended info code for the event.

RETURN VALUE
  None

DEPENDENCIES

SIDE EFFECTS
  None
===========================================================================*/
void ps_phys_link_up_ind_ex
(
  ps_phys_link_type                * this_phys_link_ptr,
  ps_extended_info_code_enum_type    info_code
)
{
  ps_iface_event_info_u_type event_info;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

  if(!(PS_PHYS_LINK_IS_VALID(this_phys_link_ptr)))
  {
    LOG_MSG_ERROR_1("ps_phys_link_up_ind_ex(): invalid phys_link %p",
                     this_phys_link_ptr );
    PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
          
    return;
  }

  LOG_MSG_INFO2_2("ps_phys_link_up_ind_ex(): "
                  "PHYS LINK UP IND 0x%p, state %d",
                  this_phys_link_ptr,
                  PS_PHYS_LINKI_GET_STATE(this_phys_link_ptr));

  /*-------------------------------------------------------------------------
    store the previous state, change the current link state to UP and call
    the callbacks.
  -------------------------------------------------------------------------*/
  event_info.phys_link_event_info.state =
    PS_PHYS_LINK_GET_STATE(this_phys_link_ptr);
  event_info.phys_link_event_info.info_code = info_code;

  PS_PHYS_LINKI_SET_STATE(this_phys_link_ptr, PHYS_LINK_UP);

  ps_ifacei_invoke_event_cbacks( NULL,
                                 this_phys_link_ptr,
                                 PHYS_LINK_UP_EV,
                                 event_info);

  /*-------------------------------------------------------------------------
    Need to enable flow so that DS_WRITE_EVENT can be posted in sockets
    layer when phys link is in this state
  -------------------------------------------------------------------------*/
  ps_phys_link_enable_flow(this_phys_link_ptr, DS_FLOW_PHYS_LINK_MASK);

  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

} /* ps_phys_link_up_ind_ex() */


/*===========================================================================
FUNCTION PS_PHYS_LINK_GONE_IND_EX()

DESCRIPTION
  The physical link of the given interface has been aborted, indicating
  that the session has gone NULL.  This is called in lieu of
  phys link_down_ind().

  THIS FUNCTION SHOULD NOT BE CALLED IN A ISR.

PARAMETERS
  this_phys_link_ptr: ptr to interface control block on which to operate
  on.
  info_code:  Extended info code for the event.

RETURN VALUE
  None

DEPENDENCIES

SIDE EFFECTS
  Note that the clients that have registered for the PHYS_LINK_DOWN callback
  also get notified that the phys link has transitioned to the down state.
===========================================================================*/
void ps_phys_link_gone_ind_ex
(
  ps_phys_link_type                * this_phys_link_ptr,
  ps_extended_info_code_enum_type    info_code
)
{
  ps_iface_event_info_u_type event_info;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

  if(!(PS_PHYS_LINK_IS_VALID(this_phys_link_ptr)))
  {
    LOG_MSG_ERROR_1("ps_phys_link_gone_ind_ex(): invalid phys_link %p",
                     this_phys_link_ptr );
    PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
    
    return;
  }

  LOG_MSG_INFO2_2("ps_phys_link_gone_ind_ex(): "
                  "PHYS LINK GONE IND 0x%p, state %d",
                  this_phys_link_ptr,
                  PS_PHYS_LINKI_GET_STATE(this_phys_link_ptr));

  /*-------------------------------------------------------------------------
    Ensure that no flows are bound to a phys link if it is in FLOW_DECOUPLED
    mode
  -------------------------------------------------------------------------*/
  if (PS_PHYS_LINKI_GET_CAPABILITY(this_phys_link_ptr,
                                   PS_PHYS_LINK_CAPABILITY_FLOW_DECOUPLED))
  {
    if (PS_PHYS_LINKI_GET_REF_CNT(this_phys_link_ptr) != 0)
    {
      PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
      LOG_MSG_ERROR_1("ps_phys_link_gone_ind_ex(): "
                      "Flows are still bound to phys link, 0x%p",
                      this_phys_link_ptr);
      DS_UTILS_ASSERT(0);
      return;
    }
  }

  /*-------------------------------------------------------------------------
    store the previous state, change the current link state to UP and call
    the callbacks.
  -------------------------------------------------------------------------*/
  event_info.phys_link_event_info.state =
    PS_PHYS_LINK_GET_STATE(this_phys_link_ptr);
  event_info.phys_link_event_info.info_code = info_code;

  PS_PHYS_LINKI_SET_STATE(this_phys_link_ptr, PHYS_LINK_NULL);

  /*-------------------------------------------------------------------------
    Bearer id was reset when physlink goes down and modehandler does not set
    the bearer id again. This results in bearer id being 255. So the bearer
    id is reset only when physlink goes NULL.
  -------------------------------------------------------------------------*/
  PS_PHYS_LINK_RESET_BEARER_ID(this_phys_link_ptr);

  ps_ifacei_invoke_event_cbacks(NULL,
                                this_phys_link_ptr,
                                PHYS_LINK_GONE_EV,
                                event_info);

  /*-------------------------------------------------------------------------
    Reset flow control mask. Otherwise, iface will forever be flow controlled
    forever if client doesn't flow enable.

    Also, flow needs to be enabled flow so that DS_WRITE_EVENT can be posted
    in sockets layer when phys link is in this state
  -------------------------------------------------------------------------*/
  ps_phys_link_enable_flow(this_phys_link_ptr,
                           this_phys_link_ptr->phys_private.tx_flow_mask);

  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

} /* ps_phys_link_gone_ind_ex() */

/*===========================================================================
FUNCTION ps_phys_link_ran_asst_media_adapt_ind()

DESCRIPTION
  Indicates RAN assisted media adaptation information

  THIS FUNCTION SHOULD NOT BE CALLED IN A ISR.

PARAMETERS
  phys_link_ptr           : ptr to a phys link
  ran_asst_info           : RAN assisted media info

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void ps_phys_link_ran_asst_media_adapt_ind
(
  ps_phys_link_type                            *this_phys_link_ptr,
  ps_phys_link_ran_asst_media_adapt_info_type   ran_asst_info
)
{
  ps_iface_event_info_u_type event_info;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

  if (!(PS_PHYS_LINK_IS_VALID(this_phys_link_ptr)))
  {
    PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
    return;
  }

  LOG_MSG_INFO1_4("ps_phys_link_ran_asst_media_adapt_ind(): "
                  "ran asst ind phys link 0x%p, direction %d bit rate %d rohc %d", 
                  this_phys_link_ptr,
                  ran_asst_info.bit_rate_info.direction,
                  ran_asst_info.bit_rate_info.bit_rate,
                  ran_asst_info.is_rohc_enabled);

  memset(&event_info, 0 , sizeof(ps_iface_event_info_u_type));
  event_info.ran_asst_info.bit_rate_info.bit_rate = ran_asst_info.bit_rate_info.bit_rate;
  event_info.ran_asst_info.bit_rate_info.direction = ran_asst_info.bit_rate_info.direction;
  event_info.ran_asst_info.is_rohc_enabled = ran_asst_info.is_rohc_enabled;

  ps_ifacei_invoke_event_cbacks( NULL,
                                 this_phys_link_ptr,
                                 PHYS_LINK_RAN_ASST_MEDIA_ADAPT_INFO_EV,
                                 event_info);

  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

} /* ps_phys_link_ran_asst_media_adapt_ind() */

/**
  @brief This function is used to allocate the memory used to register for ps_phys_link
  event callbacks.
  Once no longer needed it should be freed using ps_phys_link_free_event_handle 
 
  @param 
    phys_link_ptr[in]  ptr to phys link on which to operate on
    event_cback_f_ptr[in] the callback to be registered
    user_data_ptr[in]  data to be called with the callback
   
  @return
     NULL if allocation failed
     ptr to memory being allocated
  
  @dependencies 
    None
*/
void *ps_phys_link_alloc_event_handle
(
  ps_phys_link_type              * phys_link_ptr,
  ps_phys_link_event_cback_type    event_cback_f_ptr,
  void                           * user_data_ptr
)
{
  ps_phys_linki_event_handle_type  * handle_ptr = NULL;
  q_type                           *working_q_ptr = NULL;
  ps_phys_linki_event_cb_info_type  cb_info;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (event_cback_f_ptr == NULL)
  {
    return NULL;
  }

  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

  if ((phys_link_ptr != NULL) && !(PS_PHYS_LINK_IS_VALID(phys_link_ptr)))
  {
    PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);

    return NULL;
  }

  /*-------------------------------------------------------------------------
    Check that a handle with same cb fn ptr and user_data_ptr doesnt already
    exist.
  -------------------------------------------------------------------------*/
  cb_info.event_cback_f_ptr = event_cback_f_ptr;
  cb_info.user_data_ptr = user_data_ptr;

  /*-------------------------------------------------------------------------
    If this registration is for global callbacks, then use the global
    variable as the working pointer, otherwise use the q from the flow
  -------------------------------------------------------------------------*/
  if (phys_link_ptr == NULL)
  {
    working_q_ptr = &global_phys_link_event_handle_q;
  }
  else 
  {
    working_q_ptr = 
     &(phys_link_ptr->phys_private.event_handle_q);
  }
  do
  {
    if ( NULL != q_linear_search( working_q_ptr,
                                ps_phys_linki_compare_event_handle,
                                &cb_info ) )
    {
      LOG_MSG_ERROR_2("ps_phys_link_alloc_event_handle: already present cb_fn_ptr 0x%p, user_data_ptr 0x%p",
                      event_cback_f_ptr, user_data_ptr);
      break;
    }

    /*-------------------------------------------------------------------------
      Allocate a new event callback buffer.
    -------------------------------------------------------------------------*/
    PS_SYSTEM_HEAP_MEM_ALLOC ( handle_ptr, 
                               sizeof(ps_phys_linki_event_handle_type),
                               ps_phys_linki_event_handle_type*);

    if (handle_ptr == NULL)
    {
      break;
    }

    /*-------------------------------------------------------------------------
      Initialize the event buffer
    -------------------------------------------------------------------------*/
    memset(handle_ptr,0,sizeof(ps_phys_linki_event_handle_type));
    (void) q_link(handle_ptr, &(handle_ptr->link));
    handle_ptr->event_cback_f_ptr = event_cback_f_ptr;
    handle_ptr->user_data_ptr     = user_data_ptr;
    handle_ptr->phys_link_ptr     = phys_link_ptr;
    handle_ptr->is_phys_link_valid = TRUE;

    /* Enqueue the empty buffer */
    q_put(working_q_ptr, &(handle_ptr->link));
  } while (0);

  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

  return handle_ptr;

} /* ps_phys_link_alloc_event_handle() */


/**
  @brief This function is used to free the memory used to 
  register for ps_phys_link event callbacks 
 
  @param 
    event_handle[in]  the event handle to be freed
   
  @return
     None
  
  @dependencies 
    ps_phys_link_alloc_event_handle should have been called
*/
void ps_phys_link_free_event_handle
(
  void *evt_handle
)
{
  ps_phys_linki_event_handle_type* event_handle = NULL;
  q_type                    * working_q_ptr = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (evt_handle == NULL)
  {
    DS_UTILS_ASSERT(0);
    return;
  }

  event_handle = (ps_phys_linki_event_handle_type*)evt_handle;
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
  do
  {
    if (event_handle->phys_link_ptr == NULL)
    {
      working_q_ptr = &global_phys_link_event_handle_q;
    }
    else 
    {
      if( FALSE == event_handle->is_phys_link_valid )
      {
        /*---------------------------------------------------------------------
          This happens when phys link is already freed. So just free event 
          handle 
          -------------------------------------------------------------------*/
        break;
      }
    
      working_q_ptr = 
       &(event_handle->phys_link_ptr->phys_private.event_handle_q);
    }

    /* Delete from the queue */
    q_delete(working_q_ptr, &(event_handle->link));
  }while(0);

  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

  /*-------------------------------------------------------------------------
    Invalidate the buffer so that the caller cannot use it without
    explicitly allcating it again.
  -------------------------------------------------------------------------*/
  memset(event_handle, 0, sizeof(ps_phys_linki_event_handle_type));

  PS_SYSTEM_HEAP_MEM_FREE(event_handle);

} /* ps_phys_link_free_event_handle() */


/**
  @brief Subscribe to a list of events on a phys link. Whenever an event in the list
  occurs, the callback for that event is called. If an event in the list is true
  when registration happens the callback is called immediately.  This will fail if
  the event handle (with callback and user data pointer) is NULL.  The same event
  handle can be used to subscribe to an additional set of events. If an event in
  the list had previously been subscribed to, it will be a NO-OP. 
 
  @param 
    event_handle[in]  Event handle that holds the callback
    pointer. Should not be NULL.
    events[in]  Array of events that triggers the callbacks
    num_events[in]  Number of elements in the array
   
  @return
     0 on success
     -1 on failure
  
  @dependencies 
    Event handle MUST be allocated using
    ps_phys_link_alloc_event_handle()
*/
int ps_phys_link_subscribe_event_list
(
  void                      * evt_handle,
  ps_iface_event_enum_type  * events,
  uint8                       num_events 

)
{
  ps_phys_linki_event_handle_type *  event_handle = NULL, *first_event_handle = NULL;
  ps_iface_event_info_u_type    event_info;
  boolean                       need_to_invoke_cback = FALSE;
  boolean                       is_priority_flow_event_reg = FALSE;
  uint8                         itr;
  q_type                      * working_q_ptr = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (evt_handle == NULL || events == NULL || num_events == 0)
  {
    DS_UTILS_ASSERT(0);
    return -1;
  }

  event_handle = (ps_phys_linki_event_handle_type*)evt_handle;
  /*-------------------------------------------------------------------------
    Validate the buffer that is passed in is on a queue
  -------------------------------------------------------------------------*/
  if (!Q_ALREADY_QUEUED(&(event_handle->link)))
  {
    DS_UTILS_ASSERT(0);
    return -1;
  }

  /* Validate all events in the list. phys link event can either be in legacy range 
     or new range defined by PHYS_LINK_EX_MIN_EV and PHYS_LINK_EX_MAX_EV*/
  for (itr=0; itr<num_events; itr++)
  {
    if ( events[itr] < IFACE_MIN_EV || 
        (events[itr] >= PHYS_LINK_MAX_EV && events[itr] < PHYS_LINK_EX_MIN_EV) || 
         events[itr] >= PHYS_LINK_EX_MAX_EV )
    {

      LOG_MSG_ERROR_1("ps_phys_link_subscribe_event_list(): "
                      "Invalid physlink event %d to register for ps_phys_link",
                      events[itr]);
      return -1;
    }
  }

  /*-------------------------------------------------------------------------
    As the contents of the buffer are supposed to be set by ps_phys_link
    functions it should NEVER have a NULL callback
  -------------------------------------------------------------------------*/
  if (event_handle->event_cback_f_ptr == NULL)
  {
    DS_UTILS_ASSERT(0);
    return -1;
  }

  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

  /*-------------------------------------------------------------------------
    if not the global queue then make sure that the interface is valid
  -------------------------------------------------------------------------*/
  if (event_handle->phys_link_ptr != NULL &&
      !PS_PHYS_LINK_IS_VALID(event_handle->phys_link_ptr))
  {
    LOG_MSG_ERROR_1("ps_phys_link_subscribe_event_list(): "
                    "Invalid physlink in event handle, 0x%p", event_handle->phys_link_ptr);
    PS_BRANCH_LEAVE_CRIT_SECTION(&global_ps_crit_section);
    return -1;
  }

  LOG_MSG_QTRACE_HFT_MED(PS_TAG_NETIFACE, PS_SYS_DEFAULT_SUBS,
                 "ps_phys_link_subscribe_event_list(): Number of events registered %d",
                 num_events);

  for (itr=0; itr<num_events; itr++)
  {
#ifdef FEATURE_DATA_FLOW_MGMT
   if ( IFACE_PHYS_LINK_FLOW_CONTROL_EV == events[itr])
   {
     ps_phys_linki_flow_event_reg |= PS_PHYS_LINKI_FLOW_EVENT_REG_ULB;
   }
#endif /* FEATURE_DATA_FLOW_MGMT */
  
    /* NO-OP if event is already set in bitmask */
    if (ps_phys_linki_is_event_set(event_handle, events[itr]))
    {
      continue;
    }

    ps_phys_linki_set_event_bit(event_handle, events[itr]);

    if (events[itr] == PHYS_LINK_FLOW_ENABLED_EV || events[itr] == PHYS_LINK_FLOW_DISABLED_EV)
    {
      is_priority_flow_event_reg = TRUE;
    }

    /* For global registation no need to invoke cback */
    if (event_handle->phys_link_ptr == NULL)
    {
      continue;
    }

    need_to_invoke_cback = FALSE;

    /*-------------------------------------------------------------------------
      Initializing "event_info" 
    -------------------------------------------------------------------------*/
    memset(&event_info, 0, sizeof(event_info));
    event_info.phys_link_event_info.state = PHYS_LINK_DOWN;

    /*-------------------------------------------------------------------------
      check if the event is true - if so call the callback immediately
    -------------------------------------------------------------------------*/
    switch (events[itr])
    {
      /*-----------------------------------------------------------------------
                              PHYS_LINK_DOWN_EV
      -----------------------------------------------------------------------*/
      case PHYS_LINK_DOWN_EV:
        if (PS_PHYS_LINKI_GET_STATE(event_handle->phys_link_ptr) == PHYS_LINK_DOWN)
        {
          event_info.phys_link_event_info.state     = PHYS_LINK_DOWN;
          event_info.phys_link_event_info.info_code = PS_EIC_NOT_SPECIFIED;
          need_to_invoke_cback                      = TRUE;
        }
        break;

      /*-----------------------------------------------------------------------
                            PHYS_LINK_COMING_UP_EV
      -----------------------------------------------------------------------*/
      case PHYS_LINK_COMING_UP_EV:
        if (PS_PHYS_LINKI_GET_STATE(event_handle->phys_link_ptr) == PHYS_LINK_COMING_UP)
        {
          event_info.phys_link_event_info.state     = PHYS_LINK_COMING_UP;
          event_info.phys_link_event_info.info_code = PS_EIC_NOT_SPECIFIED;
          need_to_invoke_cback                      = TRUE;
        }
        break;

      /*-----------------------------------------------------------------------
                            PHYS_LINK_UP_EV
      -----------------------------------------------------------------------*/
      case PHYS_LINK_UP_EV:
        if (PS_PHYS_LINKI_GET_STATE(event_handle->phys_link_ptr) == PHYS_LINK_UP)
        {
          event_info.phys_link_event_info.state     = PHYS_LINK_UP;
          event_info.phys_link_event_info.info_code = PS_EIC_NOT_SPECIFIED;
          need_to_invoke_cback                      = TRUE;
        }
        break;

      /*-----------------------------------------------------------------------
                           PHYS_LINK_GOING_DOWN_EV
      -----------------------------------------------------------------------*/
      case PHYS_LINK_GOING_DOWN_EV:
        if (PS_PHYS_LINKI_GET_STATE(event_handle->phys_link_ptr) == PHYS_LINK_GOING_DOWN)
        {
          event_info.phys_link_event_info.state     = PHYS_LINK_GOING_DOWN;
          event_info.phys_link_event_info.info_code = PS_EIC_NOT_SPECIFIED;
          need_to_invoke_cback                      = TRUE;
        }
        break;

      /*-----------------------------------------------------------------------
                           PHYS_LINK_GONE_EV
      -----------------------------------------------------------------------*/
      case PHYS_LINK_GONE_EV:
        if (PS_PHYS_LINKI_GET_STATE(event_handle->phys_link_ptr) == PHYS_LINK_NULL)
        {
          event_info.phys_link_event_info.state     = PHYS_LINK_NULL;
          event_info.phys_link_event_info.info_code = PS_EIC_NOT_SPECIFIED;
          need_to_invoke_cback                      = TRUE;
        }
        break;

      /*-----------------------------------------------------------------------
                           PHYS_LINK_GOING_NULL_EV
      -----------------------------------------------------------------------*/
      case PHYS_LINK_GOING_NULL_EV:
        if (PS_PHYS_LINKI_GET_STATE(event_handle->phys_link_ptr) == PHYS_LINK_GOING_NULL)
        {
          event_info.phys_link_event_info.state     = PHYS_LINK_GOING_NULL;
          event_info.phys_link_event_info.info_code = PS_EIC_NOT_SPECIFIED;
          need_to_invoke_cback                      = TRUE;
        }
        break;

      /*-----------------------------------------------------------------------
                           PHYS_LINK_RESUMING_EV
      -----------------------------------------------------------------------*/
      case PHYS_LINK_RESUMING_EV:
        if (PS_PHYS_LINKI_GET_STATE(event_handle->phys_link_ptr) == PHYS_LINK_RESUMING)
        {
          event_info.phys_link_event_info.state     = PHYS_LINK_RESUMING;
          event_info.phys_link_event_info.info_code = PS_EIC_NOT_SPECIFIED;
          need_to_invoke_cback                      = TRUE;
        }
        break;

      /*-----------------------------------------------------------------------
                            PHYS_LINK_FLOW_ENABLED_EV
      -----------------------------------------------------------------------*/
      case PHYS_LINK_FLOW_ENABLED_EV:
        if (PS_PHYS_LINKI_FLOW_ENABLED(event_handle->phys_link_ptr))
        {
          event_info.flow_mask = ALL_FLOWS_ENABLED;
          need_to_invoke_cback = TRUE;
        }
        break;

      /*-----------------------------------------------------------------------
                           PHYS_LINK_FLOW_DISABLED_EV
      -----------------------------------------------------------------------*/
      case PHYS_LINK_FLOW_DISABLED_EV:
        if (!PS_PHYS_LINKI_FLOW_ENABLED(event_handle->phys_link_ptr))
        {
          event_info.flow_mask = ps_phys_link_get_flow_mask(event_handle->phys_link_ptr);
          need_to_invoke_cback = TRUE;
        }
        break;

      default:
        need_to_invoke_cback = FALSE;
        break;

    } /* switch(event) */

    /*-------------------------------------------------------------------------
      If the callback needs to be called, do so
    -------------------------------------------------------------------------*/
    if (need_to_invoke_cback)
    {
      event_handle->event_cback_f_ptr(event_handle->phys_link_ptr,
                                      events[itr],
                                      event_info,
                                      event_handle->user_data_ptr);
    }
  } /* for loop around event list*/

  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);
  /* Move the event_handle to the head of the queue if a priority flow event has been
     registered for. */
  if (is_priority_flow_event_reg)
  {

    /*-------------------------------------------------------------------------
      Choose appropriate queue based on whether callback is global or per iface
    -------------------------------------------------------------------------*/
    PS_ENTER_CRIT_SECTION(&global_ps_crit_section);
    if (event_handle->phys_link_ptr == NULL)
    {
      working_q_ptr = &global_phys_link_event_handle_q;
    }
    else 
    {
      working_q_ptr = 
       &(event_handle->phys_link_ptr->phys_private.event_handle_q);
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

} /* ps_phys_link_subscribe_event_list() */

/**
  @brief Unsubscribe from a list of events on a phys link. This will
  fail if the event handle (with callback and user data pointer) is NULL. If an
  event in the list had not previously been subcribed to, it will be a NO-OP.
  If an event handle is no longer needed, it should be freed using
  ps_phys_link_free_event_handle 
 
  @param 
    event_handle[in]  Event handle that holds the callback
    pointer. Should not be NULL.
    events[in]  Array of events to unsubscribe from
    num_events[in]  Number of elements in the array
   
  @return
     0 on success
     -1 on failure
  
  @dependencies 
    ps_phys_link_alloc_event_handle should have been called
*/
int ps_phys_link_unsubscribe_event_list
(
  void                     *evt_handle,
  ps_iface_event_enum_type *events,
  uint8                     num_events 
)
{
  q_type                      * working_q_ptr = NULL;
  ps_phys_linki_event_handle_type  * event_handle = NULL, * last_event_handle = NULL;
  boolean                    is_priority_flow_event_unreg = FALSE;
  uint8                      itr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (evt_handle == NULL || events == NULL || num_events == 0)
  {
    DS_UTILS_ASSERT(0);
    return -1;
  }

  event_handle = (ps_phys_linki_event_handle_type*)evt_handle;
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
    if ( events[itr] < IFACE_MIN_EV || 
        (events[itr] >= PHYS_LINK_MAX_EV && events[itr] < PHYS_LINK_EX_MIN_EV) || 
         events[itr] >= PHYS_LINK_EX_MAX_EV )
    {

      LOG_MSG_ERROR_1("ps_phys_link_unsubscribe_event_list(): "
                      "Invalid physlink event %d to de-register for ps_phys_link",
                      events[itr]);
      return -1;
    }
  }

  /*-------------------------------------------------------------------------
    As the contents of the buffer are supposed to be set by ps_phys_link
    functions it should NEVER have a NULL callback
  -------------------------------------------------------------------------*/
  if (event_handle->event_cback_f_ptr == NULL)
  {
    DS_UTILS_ASSERT(0);
    return -1;
  }

  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

  /*-------------------------------------------------------------------------
    if not the global queue then make sure that the interface is valid
  -------------------------------------------------------------------------*/
  if (event_handle->phys_link_ptr != NULL &&
      !PS_PHYS_LINK_IS_VALID(event_handle->phys_link_ptr))
  {
    LOG_MSG_ERROR_1("ps_phys_link_unsubscribe_event_list(): "
                    "Invalid physlink in event handle, 0x%p", event_handle->phys_link_ptr);
    PS_BRANCH_LEAVE_CRIT_SECTION( &global_ps_crit_section );
    return -1;
  }

  for (itr=0; itr<num_events; itr++)
  {
    LOG_MSG_INFO2_1("ps_phys_link_unsubscribe_event_list(): Dereg for event %d",
                    events[itr]);

#ifdef FEATURE_DATA_FLOW_MGMT
    if ( IFACE_PHYS_LINK_FLOW_CONTROL_EV == events[itr])
    {
      ps_phys_linki_flow_event_reg &= (~PS_PHYS_LINKI_FLOW_EVENT_REG_ULB);
    }
#endif /* FEATURE_DATA_FLOW_MGMT */
    
    /* NO-OP if event is already unset in bitmask */
    if (!ps_phys_linki_is_event_set(event_handle, events[itr]))
    {
      continue;
    }

    ps_phys_linki_unset_event_bit(event_handle, events[itr]);
    /* Make sure both flow events are not registered for */
    if (((events[itr] == PHYS_LINK_FLOW_ENABLED_EV) &&
        (!ps_phys_linki_is_event_set(event_handle, PHYS_LINK_FLOW_DISABLED_EV))) ||
        ((events[itr] == PHYS_LINK_FLOW_DISABLED_EV) &&
         (!ps_phys_linki_is_event_set(event_handle, PHYS_LINK_FLOW_ENABLED_EV))))
    {
      is_priority_flow_event_unreg = TRUE;
    }
  }
  /*-------------------------------------------------------------------------
    Move the event_handle to the tail of the queue if flow events have been
    de-registered from
  -------------------------------------------------------------------------*/
  if (is_priority_flow_event_unreg)
  {
    if (event_handle->phys_link_ptr == NULL)
    {
      working_q_ptr = &global_phys_link_event_handle_q;
    }
    else 
    {
      working_q_ptr = 
       &(event_handle->phys_link_ptr->phys_private.event_handle_q);
    }

    last_event_handle = q_last_check(working_q_ptr);
    if ((q_cnt(working_q_ptr) > 1) && (last_event_handle != event_handle))
    {
      /* Delete from the queue */
      q_delete(working_q_ptr, &(event_handle->link));
      /* Add to tail of the queue */
      q_insert_after(working_q_ptr, &(event_handle->link), &(last_event_handle->link));
    }
  }

  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);
  return 0;

} /* ps_phys_link_unsubscribe_event_list() */

boolean ps_phys_linki_is_flow_event_reg
( 
  ps_phys_linki_flow_event_reg_type type
)
{
#ifdef FEATURE_DATA_FLOW_MGMT
   boolean status = FALSE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if( ps_phys_linki_flow_event_reg & PS_PHYS_LINKI_FLOW_EVENT_REG_ULB ) 
  {
    status = TRUE;
  }
  return status;
#else
  return FALSE;
#endif /* FEATURE_DATA_FLOW_MGMT */
  
}/* ps_phys_linki_is_flow_event_reg */

/*===========================================================================
FUNCTION PS_PHYS_LINK_PACKET_DISCARD_IND()

DESCRIPTION
  To post packet discard indication.

PARAMETERS
  this_phys_link_ptr: ptr to phys link on which to operate on.

RETURN VALUE
  None

DEPENDENCIES

SIDE EFFECTS
  None
===========================================================================*/
void ps_phys_link_packet_discard_ind
(
  ps_phys_link_type           *this_phys_link_ptr
)
{
  ps_iface_event_info_u_type  event_info;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (!(PS_PHYS_LINK_IS_VALID(this_phys_link_ptr)))
  {
    return;
  }

  memset(&event_info, 0, sizeof(ps_iface_event_info_u_type));

  ps_ifacei_invoke_event_cbacks( NULL,
                                 this_phys_link_ptr,
                                 PHYS_LINK_PACKET_DISCARD_INFO_EV,
                                 event_info);
}/* ps_phys_link_packet_discard_ind */
