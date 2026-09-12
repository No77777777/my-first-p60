#ifndef PS_INET_EVENT_MGR_H
#define PS_INET_EVENT_MGR_H
/*===========================================================================

          I N E T   M E M O R Y  P O O L  H E A D E R  F I L E

DESCRIPTION
  The inet module memory pool management header file.

EXTERNAL FUNCTIONS
  PS_INET_MEM_POOL_INIT()
    Initialized memory for inet module

Copyright (c) 2008 - 2014 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/protocols/inet/src/ps_inet_event_mgr.h#2 $
  $DateTime: 2019/10/22 02:53:17 $
  
when       who     what, where, why
--------   ---     -----------------------------------------------------------
11/04/14   kc      Added ps_inet_event_mgr_deinit()
===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "customer.h"


#ifdef __cplusplus
extern "C" {
#endif
/*===========================================================================

                        PUBLIC FUNCTION DECLARATIONS

===========================================================================*/
/*===========================================================================
FUNCTION PS_INET_EVENT_MGR_INIT()

DESCRIPTION
  Initializes inet memories.

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
);

/*===========================================================================
FUNCTION PS_INET_EVENT_MGR_DEINIT()

DESCRIPTION
  De-allocate inet memories.

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
);

#ifdef __cplusplus
}
#endif
#endif /* PS_INET_EVENT_MGR_H */
