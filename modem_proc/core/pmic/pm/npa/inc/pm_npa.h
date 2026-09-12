#ifndef PM_NPA_H
#define PM_NPA_H
/*===========================================================================


                  P M    NPA   H E A D E R    F I L E

DESCRIPTION
  This file contains prototype definitions to support interaction
  with the QUALCOMM Power Management ICs.

Copyright (c) 2010 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/pmic/pm/npa/inc/pm_npa.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/14/2010 umr     NPA Transaction support
06/03/2010 umr     Created.
===========================================================================*/
/*===========================================================================

                        INCLUDE FILES

===========================================================================*/
#include "rpm.h"
#include "pm_npa_device.h"
#include "pm_err_flags.h"
/*===========================================================================

                        TYPE DEFINITIONS

===========================================================================*/



/*===========================================================================

                        DEFINITIONS

===========================================================================*/

/*===========================================================================

                        GENERIC FUNCTION PROTOTYPES

===========================================================================*/

npa_resource_state
pm_npa_resource_set (npa_resource      *resource,
                     npa_client_handle  client,
                     npa_resource_state mode_id);

npa_resource_state
pm_npa_process_rsrc (npa_client_handle client,
                     npa_resource *resource,
                     npa_resource_state mode_id,
                     uint32 index, uint32 dep_idx);

npa_resource_state
pm_npa_resource_set_vdd_uvol (npa_resource      *resource,
                             npa_client_handle  client,
                             npa_resource_state req_uv);
                             
#endif /* PM_NPA_H */
