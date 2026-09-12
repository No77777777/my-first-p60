#ifndef MBA_AC_H
#define MBA_AC_H
 /**===========================================================================
 **
 ** FILE 
 **     mba_access_control.c
 ** 
 ** GENERAL DESCRIPTION 
 **     This file provides the APIs to use QTimers within
 **     the Modem subsystem. All QTimer functionality is based
 **     on a global system counter which runs at 19.2 Mhz.                                        
 ** 
 **==========================================================================*/


/*=============================================================================


                            EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/core.mpss/10.0/api/securemsm/mba/mba_ac.h#1 $

  when       who     what, where, why
  --------   ---     ----------------------------------------------------------
  07/01/12   mm      Initial Revision for MSM8974
 
=============================================================================*/


/*=============================================================================
                                MODULE INCLUDES
                          ADD NEW ONES UNDER THIS LINE    
=============================================================================*/
#include <IxErrno.h>
#include <string.h>
#include "com_dtypes.h"

/*=============================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

=============================================================================*/

#define MSA_AC_PAGE_4K           4096
/* Clears the lowest 12 bits on xx. */
#define MSA_AC_FLOOR_4K(xx)      ((xx) & ~(MSA_AC_PAGE_4K-1))

/* Modem EFS Partition */
typedef enum resource_id_s
{
  MBA_XPU_INVALID_PARTITION = -1,
  MBA_XPU_PARTITION_PMI=0,
  MBA_XPU_PARTITION_MODEM_EFS = 1,
  MBA_XPU_PARTITION_MODEM_UNCACHED_HEAP = 2,
  MBA_XPU_PARTITION_MODEM_RFSA = 3,
  MBA_XPU_PARTITION_RECLAIM_MBA = 4,
  MBA_XPU_PARTITION_DEBUG = 5,
  MBA_XPU_PARTITION_MAX_USAGE, //Please add new partitions above this
 }resource_id_type;


int mba_xpu_initialize(void);

int mba_xpu_lock_region(uint64 start, uint64 end, resource_id_type resource_id);

uint8 mba_seccrtl_allow_unlock_xpu(void);

int mba_xpu_scribble_unlock_region(resource_id_type resource_id);

#endif

