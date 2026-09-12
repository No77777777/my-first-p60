#ifndef PS_PHYS_LINKI_EVENT_H
#define PS_PHYS_LINKI_EVENT_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

              P S _ P H Y S _ L I N K I _ E V E N T S . H

GENERAL DESCRIPTION
  This is the implementation of the PS phys link event functions. This
  file implement functions used for phys_link events.

EXTERNAL FUNCTIONS

  PS_PHYS_LINK_EVENT_INIT()
    Called at power-up to initialize event arrays, etc.

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Must call ps_phys_link_event_init() at startup.

Copyright (c) 2002 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
Copyright (c) 2003 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
Copyright (c) 2004 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                            EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/netiface/inc/ps_phys_linki_event.h#2 $
  $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
02/22/06    msr    Using single critical section.
02/06/06    msr    Updated for L4 tasklock/crit sections.
12/29/03    ak     created file.
===========================================================================*/


/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "customer.h"
#include "ps_iface_defs.h"
#include "ps_phys_link.h"
#include "queue.h"

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================

                         EXTERNAL DATA DECLARATIONS

===========================================================================*/
/*---------------------------------------------------------------------------
  GLOBAL_PHYS_LINK_EVENT_HANDLE_Q - this is used to store callbacks that are
    intended to be called when events happen on ANY phys_link.
---------------------------------------------------------------------------*/
extern q_type  global_phys_link_event_handle_q;

/*===========================================================================
TYPEDEF PS_PHYS_LINKI_EVENT_HANDLE_TYPE

DESCRIPTION
  This is the type that is placed on the phys link event callback queues.  It
  is only used internally and so should not be referenced outside of this
  file.
===========================================================================*/
typedef struct
{
  q_link_type                     link;
  uint32                          event_mask;
  ps_phys_link_type             * phys_link_ptr;
  ps_phys_link_event_cback_type   event_cback_f_ptr;
  /*---------------------------------------------------------------------------
    This flag is used to validate phys link while freeing the handle. Normal 
    validity checks won't work as bearer of this phys_link could have been 
    already freed and using it will result in invalid memory access
   --------------------------------------------------------------------------*/
  boolean                         is_phys_link_valid;
  void                           *user_data_ptr;
} ps_phys_linki_event_handle_type;

typedef enum
{
  PS_PHYS_LINKI_FLOW_EVENT_REG_NONE   = 0x0,
  PS_PHYS_LINKI_FLOW_EVENT_REG_LEGACY = 0x1,
  PS_PHYS_LINKI_FLOW_EVENT_REG_ULB    = 0x2
  
}ps_phys_linki_flow_event_reg_type;


/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

                      PUBLIC FUNCTION DEFINITIONS

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/

/*===========================================================================
FUNCTION PS_PHYS_LINK_EVENT_INIT()

DESCRIPTION
  This function initializes the global queues for ps_phys_link.  It also
  registers the phys_link function on the global phys_link queue.

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
);

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
);

/**
  @brief This function is used to free the memory used to register for ps_phys_link
  event callbacks.
 
  @param 
    event_handle[in]  the event handle to be freed
   
  @return
     None
  
  @dependencies 
    ps_phys_link_alloc_event_handle should have been called
*/

void ps_phys_link_free_event_handle
(
  void  * event_handle
);
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
  void                      * event_handle,
  ps_iface_event_enum_type  * events,
  uint8                       num_events
);
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
  void                      * event_handle,
  ps_iface_event_enum_type  * events,
  uint8                       num_events
);
/**
  @brief Check if given flow control event is registered. If so then return TRUE otherwise false
 
  @param 
    type[in]  Flow control event reg type
   
  @return
       TRUE   Registered 
       FALSE  Not Registered
  
  @dependencies 
    ps_phys_link_alloc_event_handle should have been called
*/
boolean ps_phys_linki_is_flow_event_reg
( 
  ps_phys_linki_flow_event_reg_type type
);
/**
  @brief Check if given flow control event is registered. If so then return TRUE otherwise false
 
  @param 
    type[in]  Flow control event reg type
   
  @return
       TRUE   Registered 
       FALSE  Not Registered
  
  @dependencies 
    ps_phys_link_alloc_event_handle should have been called
*/
#define PS_PHYS_LINKI_IS_FLOW_EVENT_REG(event_reg_type)  ps_phys_linki_is_flow_event_reg(event_reg_type)

#ifdef __cplusplus
}
#endif

#endif /* PS_PHYS_LINKI_EVENT_H */
