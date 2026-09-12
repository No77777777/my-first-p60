/*=============================================================================

                err_qdi_client.h  --  Header File

GENERAL DESCRIPTION
      ERR QDI Layer Header File

EXTERNAL FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS
   None.

Copyright (c) 2007 - 2019 by Qualcomm Technologies, Inc.  All Rights Reserved.

=============================================================================*/

/*=============================================================================

                        EDIT HISTORY FOR MODULE

 This section contains comments describing changes made to the module.
 Notice that changes are listed in reverse chronological order.


$Header: //components/rel/core.mpss/10.0/debugtools/err/src/err_qdi_client.h#1 $ 
$DateTime: 2019/04/24 00:03:26 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ------------------------------------------------------------
07/31/14   din      Added qdi calls for err on User PD.
=============================================================================*/
#ifndef ERR_QDI_CLIENT_H
#define ERR_QDI_CLIENT_H

#include "err_qdi.h"

/*==============================================================================

                           EXTERN FUNCTIONS

=============================================================================*/

/*===========================================================================

FUNCTION    ERR_QDI_INVOKE_COMMUNICATE_USER_CB

DESCRIPTIONT
  Sends the user Process callback info to user PD.

DEPENDENCIES
  None.

RETURN VALUE
  
SIDE EFFECTS
  None

===========================================================================*/

err_qdi_e err_qdi_client_communicate_user_cb
(
  err_cb_bucket_t bucket,
  void *          cb
);

/*===========================================================================

FUNCTION    ERR_QDI_CLIENT_COMMUNICATE_TID

DESCRIPTION
  Initialize the err client qdi service.

DEPENDENCIES
  None.

RETURN VALUE
  
SIDE EFFECTS
  None

===========================================================================*/
void err_qdi_client_communicate_tid
(
  unsigned int tid, 
  unsigned int fault_pc, 
  unsigned int sp, 
  unsigned int badva, 
  unsigned int cause,
  unsigned int fp,
  unsigned int lr,
  unsigned int ssr
);

#endif /* ERR_QDI_CLIENT_H */
