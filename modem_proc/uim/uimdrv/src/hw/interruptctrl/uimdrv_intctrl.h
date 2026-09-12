#ifndef UIMDRV_INTCTRL_H
#define UIMDRV_INTCTRL_H
/*============================================================================
  FILE:        uimdrv_intctrl.h

  OVERVIEW:    Contains the implementation of the UARTDM class for uim drivers.

  DEPENDENCIES: N/A

                Copyright (c) 2012-2016 QUALCOMM Technologies, Inc (QTI) and its licensors.
                All Rights Reserved.
                QUALCOMM Technologies Confidential and Proprietary
============================================================================*/

/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/hw/interruptctrl/uimdrv_intctrl.h#2 $
$DateTime: 2019/10/30 06:56:05 $
$Author: pwbldsvc $

when        who        what, where, why
------      ----       -----------------------------------------------------------
07/20/15    ks         Consolidating UIM bring-up changes
09/18/14    akv        UIMDRV UT enhancements
11/27/13    akv        Initialization of hw enumeration based on dal chip family
10/08/13    nmb        HW header file cleanup
07/12/13    nmb        Global Reorganization
07/12/13    akv        HW Enumeration - modifying target specific hardware values 
                       and organizing them in value-key pairs
12/05/12    js         UIM Parallel Processing changes
10/26/12    akv/ssr    HW Enumeration changes
============================================================================*/
#include "comdef.h"
#include "uimdrv_main.h"

/* Initialize interrupt controller */
void initialize_intctrl(uim_instance_global_type *uim_ptr);

/*Register for IRQ that is mapped to UIM card event IRQ in MPM wakeup table*/
void uim_register_for_mpm_wakeup_capability(uim_instance_global_type *uim_ptr);

#endif /* UIMDRV_INTCTRL_H */
