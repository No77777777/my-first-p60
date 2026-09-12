/*===========================================================================
                      DS_EDU_MGR_HTTP.H

DESCRIPTION
 HTTP header file for EDU_MGR.

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

#ifndef DS_EDU_MGR_HTTP_H
#define DS_EDU_MGR_HTTP_H

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#include "datamodem_variation.h"
#ifdef FEATURE_DATA_EDU_MGR
#include "ds_http_api.h"



extern uint32   ds_edu_mgr_http_session_id;
/*===========================================================================

                           FUNCTION DECLARATIONS

===========================================================================*/
void  ds_edu_mgr_http_init(void);


#endif   /*FEATURE_DATA_EDU_MGR*/
#endif /* DS_EDU_MGR_HTTP_H */
