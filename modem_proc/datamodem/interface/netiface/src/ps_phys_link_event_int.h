#ifndef PS_PHYS_LINK_EVENT_INT_H
#define PS_PHYS_LINK_EVENT_INT_H
/*===========================================================================
  @file ps_phys_link_event_int.h

  TODO

  Copyright (c) 2014 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/netiface/src/ps_phys_link_event_int.h#2 $
  $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

===========================================================================*/

/*===========================================================================

                                INCLUDE FILES

===========================================================================*/
#include "ps_iface_defs.h"
#include "ps_phys_linki_event.h"

/*===========================================================================

                              EXTERNAL FUNCTIONS

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
);
#endif /* PS_PHYS_LINK_EVENT_INT_H */
