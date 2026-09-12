#ifndef DS3GMGR_EX_H
#define DS3GMGR_EX_H
/*===========================================================================

                      D S 3 G M G R _EX . H
                   
DESCRIPTION
  This software unit contains routines to handle mode specific handler registration
  and misc. functions(This is an INTERNAL file).

EXTERNALIZED FUNCTIONS
  ds3g_reg_iwlan_s2b_emc_get_current_plmn()
    Register the get current plam handler with iWLAN
  ds3g_iwlan_lte_ho_chk_sharing_plmn()
    Check if srat and trat has sharing plmn 

 Copyright (c)2003-2018 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================

                            EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/3gpp/ds3gex/inc/ds3gmgr_ex.h#1 $
  $Author: pwbldsvc $

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when         who    what, where, why
--------   ---    ----------------------------------------------------------
04/25/18   graces   Created Module
===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"
#include "ds3gmgr.h"
#include "dstask_v.h"
#include "ds_epc_pdn_ctl.h"

/*===========================================================================

                          PUBLIC DATA DECLARATIONS

===========================================================================*/

/*===========================================================================

                        PUBLIC FUNCTION DECLARATIONS

===========================================================================*/
/*---------------------------------------------------------------------------
  Function pointer to get current PLMN on iWLAN for EMC call
---------------------------------------------------------------------------*/
typedef void (*ds3g_get_current_plmn_hdlr_f_type )
(
  sys_plmn_id_s_type*            current_plmn_id,
  sys_modem_as_id_e_type         subs_id
);

/*---------------------------------------------------------------------------
  Variable to get current PLMN on iWLAN
---------------------------------------------------------------------------*/
extern ds3g_get_current_plmn_hdlr_f_type
                                ds3g_get_current_plmn_hdlr_f_ptr;

/*===========================================================================
FUNCTION ds_epc_iwlan_lte_ho_chk_sharing_plmn

DESCRIPTION
  This function is used to check PLMN between LTE and iWLAN are in EPLMN list

PARAMETERS
  subs_id    current sub id 

DEPENDENCIES
  None.

RETURN VALUE  
  boolean - TRUE if both PLMNs from LTE and iWLAN are in EPLMN list
            else FALSE

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds3g_iwlan_lte_ho_chk_sharing_plmn
(
  uint32          subs_id
);

/*===========================================================================
FUNCTION ids_ds3g_reg_iwlan_s2b_emc_get_current_plmn

DESCRIPTION
  This function is used to register current PLMN on iWLAN from iWLAN MH for
  EMC call
 
PARAMETERS
  

DEPENDENCIES
  None.

RETURN VALUE  
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_ds3g_reg_iwlan_s2b_emc_get_current_plmn
(
  ds3g_get_current_plmn_hdlr_f_type  get_current_plmn_hdlr_f
);
#endif /*DS3GMGR_EX_H */



