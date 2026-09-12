#ifndef HDRSRCHBM_H
#define HDRSRCHBM_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                S R C H _ B M   H E A D E R    F I L E


GENERAL DESCRIPTION
  This file contains the definitions of state machine control payload
  data structures.

  Copyright (c) 2005 by Qualcomm Technologies, Inc.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/hdr.mpss/6.0/srch/src/hdrsrchbm.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/17/05   ljl     Initial revision
===========================================================================*/

#include "customer.h"
#include "comdef.h"
#include "stm.h"

/*-----------------------------------------------------------------------------
      Macros and Defines
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
      Exported Function Prototypes
-----------------------------------------------------------------------------*/

/*===========================================================================

FUNCTION HDRSRCHBM_INIT_CMD_BUF_POOL

DESCRIPTION
 Maps static arrays into buffer manager space for dynamic allocation.

DEPENDENCIES
 None

RETURN VALUE
 None

SIDE EFFECTS
 None

===========================================================================*/

void hdrsrchbm_init_cmd_buf_pool( void );


/*===========================================================================

FUNCTION HDRSRCHBM_RESET_CMD_BUF_POOL

DESCRIPTION
 Defend against memory leaks by resetting all buffers.

DEPENDENCIES
 None

RETURN VALUE
 None

SIDE EFFECTS
 None

===========================================================================*/

void hdrsrchbm_reset_cmd_buf_pool( void );


/*===========================================================================

FUNCTION HDRSRCHBM_RETURN_CHECKED_OUT_BUFFERS

DESCRIPTION
 Emit error messages for each buffer currently checked out. Clear the buffer,
 and return it to the pool.

DEPENDENCIES
 None

RETURN VALUE
 None

SIDE EFFECTS
 None

===========================================================================*/

void hdrsrchbm_return_checked_out_buffers
(
  stm_group_type             *group
    /* Stm group */
);


/*===========================================================================

FUNCTION HDRSRCHBM_MALLOC

DESCRIPTION
 Request a buffer of size 'size'.  If it's unavailable, we can't carry on,
 so ERR_FATAL.

DEPENDENCIES
 None

RETURN VALUE
 None

SIDE EFFECTS
 None

===========================================================================*/

void *hdrsrchbm_malloc
(
  uint32                     size
    /* specify the size of buffer you want */
);


/*===========================================================================

FUNCTION HDRSRCHBM_FREE

DESCRIPTION
 Return a buffer to the pool.  Failure shouldn't occur unless pointers are
 invalid, so ERR_FATAL.

DEPENDENCIES
 None

RETURN VALUE
 None

SIDE EFFECTS
 None

===========================================================================*/

void hdrsrchbm_free
(
  void                       *ptr
    /* old buffer to be returned to the pool */
);


/*===========================================================================

FUNCTION HDRSRCHBM_GROUP_HEAP_CLEAN

DESCRIPTION
 Send out a buffer usage report and reset the statistics after a group has
 exited.

DEPENDENCIES
 None

RETURN VALUE
 None

SIDE EFFECTS
 None

===========================================================================*/

void hdrsrchbm_group_heap_clean
(
  stm_group_type             *group
    /* Stm group */
);
#endif /* HDRSRCHBM_H */
