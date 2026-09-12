#ifndef DS3GDSDNR5GPROC_H
#define DS3GDSDNR5GPROC_H
/*===========================================================================

           D A T A   S E R V I C E S   D S 3 G D S D N R 5 G P R O C
 
                   H E A D E R   F I L E

DESCRIPTION
  This file contains functions for processing 5G events.

  Copyright (c) 2018 by Qualcomm Technologies Incorporated. All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/3gpp/ds3gex/inc/ds3g_dsd_nr5g_proc.h#1 $
  $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/13/17   vj      Initial revision.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "sys.h"
#include "ds3gsubsmgr.h"
#include "dstask_v.h"

#ifdef FEATURE_LAPP

typedef struct
{
  boolean any_apn_on_5g;
}ds3g_dsd_nr5g_proc_5g_criteria_type;

typedef struct {
  sys_srv_status_e_type     srv_status;      /* srv status */
  sys_srv_domain_e_type     srv_domain;      /* srv domain */
  uint32                    rat_mask;        /* rat mask */
  uint64                    so_mask;         /* so mask */
  boolean                   roaming_status; /* roaming status */
}ds3g_dsd_nr5g_proc_nr5g_srv_info_type;

/*===========================================================================
FUNCTION      DS3G_DSD_NR5G_PROC_INIT

DESCRIPTION   This function registers for the messages arriving over the 
message router and the event indicating the signal pdn status 

PARAMETERS    None

DEPENDENCIES  None.

RETURN VALUE  None

SIDE EFFECTS  None.

===========================================================================*/

void ds3g_dsd_nr5g_proc_init();

/*===========================================================================
FUNCTION      DS3G_DSD_NR5G_PROC_ALL_CRITERIA_MET_FOR_5G

DESCRIPTION   This function checks if all criteria have been met to declare 5G 
              full service.

PARAMETERS    subs_index: Subs id

DEPENDENCIES  None.

RETURN VALUE  boolean.

SIDE EFFECTS  None.
===========================================================================*/
boolean ds3g_dsd_nr5g_proc_all_criteria_met_for_5g
(
  ds3gsubsmgr_subs_id_e_type      subs_index
);

/*===========================================================================
FUNCTION DS3G_DSD_NR5G_PROC_COMPUTE_PREF_SYSTEM

DESCRIPTION
  This function is used to generate the pref sys indication with 5G or LTE as 
  the case maybe.

PARAMETERS
  subs_index   - Subscription Id

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
void ds3g_dsd_nr5g_proc_compute_pref_system
(
  ds3gsubsmgr_subs_id_e_type subs_index
);

#endif /* FEATURE_LAPP */

#endif /*DS3G_DSD_NR5G_PROC_H */
