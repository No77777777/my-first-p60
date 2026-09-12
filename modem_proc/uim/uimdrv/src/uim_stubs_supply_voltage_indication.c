/*============================================================================
  FILE:         uim_stubs_supply_voltage_inidcation.c

  OVERVIEW:
   This is a stubs file for uim_supply_voltage_inidcation.c

  EXTERNALIZED FUNCTIONS:

  DEPENDENCIES: N/A

                Copyright (c) 2016 QUALCOMM Technologies, Inc (QTI) and its licensors.
                All Rights Reserved.
                QUALCOMM Technologies Confidential and Proprietary
============================================================================*/

/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/uim_stubs_supply_voltage_indication.c#1 $
$DateTime: 2019/10/30 06:56:05 $
$Author: pwbldsvc $

when          who        what, where, why
--------      ----       ---------------------------------------------------------
05/24/16      sam        Initial Revision
============================================================================*/
#include "uim_p.h"
#include "uimglobals.h"


/*===========================================================================
FUNCTION UIM_NOTIFY_VOLTAGE_CHANGE_INDICATION

DESCRIPTION
  stubs function for uim_notify_voltage_change_indication
===========================================================================*/
void uim_notify_voltage_change_indication 
(
  uim_instance_global_type     *uim_ptr, 
  uimdrv_qmi_indications_type  indication
)
{
  (void)uim_ptr;
  (void)indication;
}/*uim_notify_voltage_change_indication*/


