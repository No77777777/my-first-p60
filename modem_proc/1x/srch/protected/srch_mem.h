#ifndef SRCH_MEM_H
#define SRCH_MEM_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

           S R C H    D Y N A M I C   M E M O R Y   M O D U L E

GENERAL DESCRIPTION
      This module contains external interfaces and declarations for the
      searcher dynamic memory allocation module.

EXTERNALIZED FUNCTIONS
      srch_mem_init
      srch_mem_deinit
      srch_mem_allocate
      srch_mem_release

INITIALIZATION AND SEQUENCING REQUIREMENTS
      Must call srch_mem_init() before any other functions in this module
      are invoked.

 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Copyright (c) 2009 - 2020 Qualcomm Technologies, Inc.
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

$Header: //components/rel/1x.mpss/8.0/srch/protected/srch_mem.h#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/22/20   vaa     Implement QSH MDUMP framework
02/05/16   pk      1x2GTA Feature changes
12/03/15   sst     Modify some F3 messages for Reduction effort
02/20/15   ab      Enhanced Debug buffer for Idle State
03/29/13   bph     Mainline RX_DIV_RT_CTRL, DYN_MEM, and RXD_IN_ACCESS
11/04/11   mca     Added TSYNC client
10/25/11   adw     Integrate TRAM support.
04/28/11   sst     Added 1xtol client
02/03/11   adw     Updated lto1x client name.
11/01/10   jtm     Modem 1x SU API changes.
04/29/10   cjb     Initialize memory area to 0 before returning the pointer
                   (irrespective of allocation method)
03/18/10   cjb     Clear pointer after releasing memory
02/12/10   sst     Added SRCH_MEM_NULL_PTR_WARN
10/14/09   bb      Added SRCH_MEM_LTE_TT_STM and
                   SRCH_MEM_IDLE_MEAS_SECT_DATA
09/25/09   adw     Added additional clients for srch pwr and srch stm.
09/14/09   adw     Initial revision.

===========================================================================*/


/*===========================================================================

                        INCLUDE FILES FOR MODULE

===========================================================================*/

/* Common */
#include "1x_variation.h"
#include "comdef.h"
#include "customer.h"
#include "modem_1x_defs.h"
#include "modem_1x_utils.h"

/* Srch */
#include "srchi.h"
#include "srchi_i.h"

/* Other */


/*===========================================================================

                  DEFINITIONS AND DECLARATIONS FOR MODULE

===========================================================================*/

/*-------------------------------------------------------------------------
      Constants and Macros
--------------------------------------------------------------------------*/
#define SRCH_MEM_NULL_PTR_CHK( ptr )                                          \
          if ( ptr == NULL )                                                  \
          {                                                                   \
            ONEX_ERR_FATAL( "Attempt to access unallocated memory", 0, 0, 0 );     \
          }

/* following macro should be used in cases where NULL ptr was unexpected, but
   should not result in a fatal error */
#define SRCH_MEM_NULL_PTR_WARN( ptr, retval )                                 \
          if ( ptr == NULL )                                                  \
          {                                                                   \
            M1X_MSG( SRCH, LEGACY_ERROR,                                      \
              "Attempt to access unallocated memory" );                       \
            { return retval; }                                                \
          }

/*-------------------------------------------------------------------------
      Typedefs
-------------------------------------------------------------------------*/

/* Unique identifier for each client of the dynamic memory module */
typedef enum
{
  SRCH_MEM_FIRST_CLIENT,
  SRCH_MEM_NA_STM = SRCH_MEM_FIRST_CLIENT,
  SRCH_MEM_CDMA_STM,
  SRCH_MEM_ACQ_STM,
  SRCH_MEM_SYNC_STM,
  SRCH_MEM_SLEW_STM,
  SRCH_MEM_IDLE_STM,
  SRCH_MEM_IDLE_CTL_STM,
  SRCH_MEM_SCHED_STM,
  SRCH_MEM_ZZ_STM,
  SRCH_MEM_RTL_STM,
  SRCH_MEM_TRAFFIC_STM,
  SRCH_MEM_TT_STM,
  SRCH_MEM_TCG_STM,
  SRCH_MEM_COMMON_STM,
  SRCH_MEM_AFC_STM,
  SRCH_MEM_RX_DIV_STM,
  SRCH_MEM_RX_DIV_AUTO_STM,
  SRCH_MEM_SYS_MEAS_STM,
  SRCH_MEM_FING_DATA,
  SRCH_MEM_PEAK_DATA,
  SRCH_MEM_SECT_DATA,
  SRCH_MEM_PWR_DATA,
  SRCH_MEM_STM_DATA,
  SRCH_MEM_STM_IDLE_DATA,
  SRCH_MEM_LTO1X_STM,
  SRCH_MEM_1XTOL_STM,
  SRCH_MEM_TRAM_STM,
  SRCH_MEM_TSYNC_DATA,
  SRCH_MEM_QTA_DATA,
  SRCH_MEM_FIRST_NON_PRINTED,
  SRCH_MEM_IS95A_STM = SRCH_MEM_FIRST_NON_PRINTED,
  SRCH_MEM_IS2000_STM,
  SRCH_MEM_QPCH_STM,
  SRCH_MEM_ONTL_STM,
  SRCH_MEM_OFFTL_STM,
  SRCH_MEM_MAX_CLIENTS
}
srch_mem_client_type;



/*=============================================================================

                      FUNCTION DECLARATIONS FOR MODULE

=============================================================================*/

/*=============================================================================

FUNCTION       SRCH_MEM_INIT
DESCRIPTION    Initialize the srch memory module. This function should
               only be called once during srch task startup and must be
               called before invoking any other routines in the srch
               memory module. This routine performs all necessary work
               to initialize the search memory client data strucutre
               in preparation for handling memory allocation requests.

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None

=============================================================================*/
void srch_mem_init( void );

/*=============================================================================

FUNCTION       SRCH_MEM_DEINIT
DESCRIPTION    De-initialize the srch memory module. This function clears
               the search memory client data structure and returns the
               memory module to a pre-initialization state. Once this
               routine is called, srch_mem_init() again needs to be called
               before any other calls to the search memory module can be
               issued.

DEPENDENCIES   The srch_mem_int() routine must have been called previously.

RETURN VALUE   None

SIDE EFFECTS   None

=============================================================================*/
void srch_mem_deinit( void );

/*=============================================================================

FUNCTION       SRCH_MEM_ALLOCATE
DESCRIPTION    Dynamically allocate memory for search data structures.
               This function ensures a valid client and then allocates
               memory appropriately. If the client has already allocated
               memory, then this routine returns the pointer previously
               allocated to the client.

DEPENDENCIES   The srch_mem_int() routine must have been called previously.

               Note this routine is not re-entrant.

RETURN VALUE   Returns a pointer to the newly allocated memory block.
               If the memory block could not be allocated, this function
               will result in an error fatal as something has gone
               seriously wrong if search is denied the necessary memory.

SIDE EFFECTS   Memory area is zero initialized.

=============================================================================*/
void* srch_mem_allocate
(
  /* Unique identifier corresponding to each client */
  srch_mem_client_type client,

  /* Number of bytes to allocate */
  size_t size
);

/*=============================================================================

FUNCTION       SRCH_MEM_RELEASE
DESCRIPTION    Releases memory allocated to the client. This function
               ensures a valid client and then releases memory accordingly.
               If there is any descrepancy between the release pointer and
               the client's allocated memory pointer (such as a client
               attempting to release memory it did not allocate), then
               this routine will issue an error message and proceed without
               releasing the un-owned memory.

               If dynamic memory is disabled, this function de-registers
               a client from the memory module "clients" data structure and
               then returns without actually releasing any memory.

DEPENDENCIES   The srch_mem_int() routine must have been called previously.

               Note this routine is not re-entrant.

               Clients should *not* call this function directly, but instead
               use the SRCH_MEM_RELEASE() macro.

RETURN VALUE   None

SIDE EFFECTS   None.

=============================================================================*/
#define SRCH_MEM_RELEASE( client, ptr )          \
          srch_mem_release( (client), (ptr) );   \
          (ptr) = NULL

void srch_mem_release
(
  /* Unique identifier corresponding to each client */
  srch_mem_client_type client,

  /* Pointer to memory to release */
  void *alloc_ptr
);

/*=============================================================================

FUNCTION       SRCH_MEM_OUTSTANDING_ALLOCS
DESCRIPTION    Verifies that no search modules have outstanding allocations.
               This function uses the allocated bitmask maintained inside
               the search memory module to determine whether or not there are
               any clients with allocated memory.

               The purpose of this routine is to quickly verify that all
               outstanding allocation requests have been released and that
               there are no search modules actively using memory resources.
               This function should be called when 1x goes inactive as a
               sanity check that all memory has been released and we are
               ready to shutdown.

DEPENDENCIES   The srch_mem_int() routine must have been called previously.

RETURN VALUE   Returns TRUE if there are currently clients with allocated
               memory.

SIDE EFFECTS   None

=============================================================================*/
boolean srch_mem_outstanding_allocs( void );

/*=============================================================================

FUNCTION       SRCH_MEM_EXPORT_CLIENTS
DESCRIPTION    Exports a pointer to the search memory data structure. This
               pointer can be used to peek inside the search memory module.

DEPENDENCIES   The srch_mem_int() routine must have been called previously.

RETURN VALUE   Pointer to the search memory data structure.

SIDE EFFECTS   None

=============================================================================*/
void* srch_mem_export_clients( void );


#endif /* SRCH_MEM_H */

