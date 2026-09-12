#ifndef DS_CAAS_MGR_EXT_H
#define DS_CAAS_MGR_EXT_H
/*===========================================================================
                      DS_CAAS_MGR.H

DESCRIPTION
 Header file of DS CAAS MGR module.

EXTERNALIZED FUNCTIONS

 Copyright (c) 2019 - 2024 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  when        who                            what, where, why
--------    -------                ----------------------------------------
10/10/19    ashisahu                          First version of file
===========================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#include "datamodem_variation.h"
#include "comdef.h"

#ifdef QWES_FEATURE_ENABLE
/*==============================================================================
                 EXTERNAL VARIABLE DEFINITIONS AND TYPES
==============================================================================*/


/*===========================================================================
                         GLOBAL VARIABLES
===========================================================================*/

/*===========================================================================
                    EXTERNAL FUNCTION DEFINITIONS
===========================================================================*/
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/*===========================================================================
FUNCTION      DS_CAAS_MGR_INIT

DESCRIPTION   This function initialize DS CAAS MGR module.

PARAMETERS    None
 
DEPENDENCIES  None 
 
RETURN VALUE  None 

SIDE EFFECTS  None
===========================================================================*/
void ds_caas_mgr_init(void);
#ifdef __cplusplus
}
#endif /* __cplusplus */



#endif /* QWES_FEATURE_ENABLE */

#endif /* DS_CAAS_MGR_EXT_H */
