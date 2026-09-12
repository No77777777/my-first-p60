#ifndef DS3GUTIL_H
#define DS3GUTIL_H
/*===========================================================================

                      D S 3 G U T I L . H
 
GENERAL DESCRIPTION
  Utility functions for DS module

Copyright (c) 2004-2016 by QUALCOMM Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE
This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/data.mpss/3.5..2.1/3gpp/dsmgr/inc/ds3gutil.h#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
02/10/15    sd     Added dial string validation utility function in DS3G.
10/15/13    sgd    Add ds3g_util_init to register callback functions

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "customer.h"
#include "ps_sys.h"

#include "ds3gcommonutil.h"


/*===========================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/
/*===========================================================================
FUNCTION       DS3G_SET_CM_OPRT_LPM_MODE

DESCRIPTION    Set CM operation mode to LPM

DEPENDENCIES   None

RETURN VALUE   TRUE: Successfully set 
               FALSE: Fail to set 

SIDE EFFECTS   None

===========================================================================*/
boolean ds3g_set_cm_oprt_lpm_mode( void );

/*===========================================================================
FUNCTION       DS3G_SET_CM_OPRT_ONLINE_MODE

DESCRIPTION    Set CM operation mode to online

DEPENDENCIES   None

RETURN VALUE   TRUE: Successfully set 
               FALSE: Fail to set 

SIDE EFFECTS   None

===========================================================================*/
boolean ds3g_set_cm_oprt_online_mode( void );

#ifdef FEATURE_DATA_PS_IPV6
/*===========================================================================
FUNCTION      DS3G_READ_IPV6_EFS_NV_PER_SUBS_ID

DESCRIPTION   Read & store common IPv6 efs items used by 3gpp & 3gpp2 MHs 
              per subscription

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3g_read_ipv6_efs_nv_per_subs_id
(
  sys_modem_as_id_e_type  subs_id
);
#endif /* FEATURE_DATA_PS_IPV6 */

#endif /* DS3GUTIL_H */