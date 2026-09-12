/*===========================================================================
                      DS_EDU_MGR_CONFIG.H

DESCRIPTION
 config header file for EDU_MGR.

EXTERNALIZED FUNCTIONS

 Copyright (c) 2014 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  when        who                            what, where, why
--------    -------                ----------------------------------------
10/09/14    Youjunf                          First version of file
===========================================================================*/

#ifndef DS_EDU_MGR_CONFIG_H
#define DS_EDU_MGR_CONFIG_H

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#include "datamodem_variation.h"
#ifdef FEATURE_DATA_EDU_MGR

#include "comdef.h"
/*===========================================================================

                           FUNCTION DECLARATIONS

===========================================================================*/
void   ds_edu_mgr_cfg_init(void);
uint8  ds_edu_mgr_cfg_get_enable_status(void);
uint16 ds_edu_mgr_cfg_get_pdn_profile_id(void);
char*  ds_edu_mgr_cfg_get_apn_name(void);

#endif   /*FEATURE_DATA_EDU_MGR*/
#endif /* DS_EDU_MGR_CONFIG_H */
