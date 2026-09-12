#ifndef SRCH_BM_H
#define SRCH_BM_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                S R C H _ B M   H E A D E R    F I L E


GENERAL DESCRIPTION
  This file contains the definitions of state machine control payload
  data structures.

  Copyright (c) 2005 - 2013 Qualcomm Technologies, Inc. 
  All Rights Reserved Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc. and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/1x.mpss/8.0/srch/protected/srch_bm.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/01/09   adw     Categorized included header files and standardized format.
03/25/09   adw     Removed unnecessary customer.h include.
07/11/06   trc     STM interface changes
08/08/05   awj     Changed srch_bm_return_checked_out_buffers() name.
08/05/05   ejv     Remove srch_slot.h.
07/26/05   awj     Changed srch_bm_free to take a void * argument
07/25/05   awj     Added new functionality for resetting and reporting.
04/20/05   awj     Initial revision
===========================================================================*/


/*===========================================================================

                        INCLUDE FILES FOR MODULE

===========================================================================*/

/* Common */
#include "comdef.h"

/* Other */
#include "stm.h"


/*===========================================================================

                  DEFINITIONS AND DECLARATIONS FOR MODULE

===========================================================================*/

/*-------------------------------------------------------------------------
      Macros
-------------------------------------------------------------------------*/

#define srch_malloc srch_bm_malloc
#define srch_free   srch_bm_free


/*===========================================================================

                    FUNCTION DECLARATIONS FOR MODULE

===========================================================================*/

/*===========================================================================

FUNCTION SRCH_BM_INIT_CMD_BUF_POOL

DESCRIPTION    Maps static arrays into buffer manager space for dynamic
               allocation.  To alter the number or size of buffers, see
               documentation in 1x/srch/common/bm.c.  Make sure to add
               buffer lists in order of increasing size.

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None

===========================================================================*/
extern void srch_bm_init_cmd_buf_pool(void);

/*===========================================================================

FUNCTION SRCH_BM_RESET_CMD_BUF_POOL

DESCRIPTION    Reset the buffer pool to a fresh state.

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None

===========================================================================*/
extern void srch_bm_reset_cmd_buf_pool(void);

/*===========================================================================

FUNCTION SRCH_BM_RETURN_CHECKED_OUT_BUFFERS

DESCRIPTION    Emit messages for each buffer currently checked out.

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None

===========================================================================*/
extern void srch_bm_return_checked_out_buffers(stm_group_type *group);

/*===========================================================================

FUNCTION SRCH_BM_MALLOC

DESCRIPTION    Request a buffer of size 'size'.  If it's unavailable, we can't
               carry on, so ERR_FATAL.

               This function is #defined to be equivalent to
               void *srch_malloc(uint32)

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None

===========================================================================*/
extern void *srch_bm_malloc
(
  uint32   command_type  /* Specify which payload data structure you want */
);

/*===========================================================================

FUNCTION SRCH_BM_FREE

DESCRIPTION    Return a buffer to the pool.  Failure shouldn't occur unless
               pointers are invalid, so ERR_FATAL.

               This function is #defined to be equivalent to
               void srch_free(uint32)

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None

===========================================================================*/
extern void srch_bm_free
(
  void  *ptr           /* old buffer to be returned to the pool */
);

/*===========================================================================

FUNCTION SRCH_BM_GROUP_HEAPCLEAN

DESCRIPTION    Send out a buffer usage report and reset the statistics
               after a group has exited.

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None

===========================================================================*/
extern void srch_bm_group_heapclean(stm_group_type *group);

#endif /* SRCH_BM_H */
