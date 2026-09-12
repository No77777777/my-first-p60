/*=============================================================================

                err_qdi_client.c

GENERAL DESCRIPTION
      ERR QDI Client Process Code that goes to main image

EXTERNAL FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS
   None.

Copyright (c) 2014 - 2019 by Qualcomm Technologies, Inc.  All Rights Reserved.

=============================================================================*/

/*=============================================================================

                        EDIT HISTORY FOR MODULE

 This section contains comments describing changes made to the module.
 Notice that changes are listed in reverse chronological order.


$Header: //components/rel/core.mpss/10.0/debugtools/err/src/err_qdi_client.c#1 $ 
$DateTime: 2019/04/24 00:03:26 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ------------------------------------------------------------
07/31/14   din     Added qdi calls for err on User PD.
=============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include "err_qdi_client.h"
#include "erri.h"
#include "tms_utils.h"

static int err_qdi_client_handle = -1;

/*===========================================================================

FUNCTION    ERR_QDI_INIT

DESCRIPTION
  Initialize the err client qdi service.

DEPENDENCIES
  None.

RETURN VALUE
  
SIDE EFFECTS
  None

===========================================================================*/
void err_qdi_init(void) 
{
   void * stack_addr = calloc(1, TMS_TASK_STACK_2K );

   if ( stack_addr == NULL )
   {
     /* Still proceed with Error QDI initilization */
   }

   err_qdi_client_handle = qurt_qdi_open(ERR_DRIVER_NAME, &coredump, &err_fatal_params, stack_addr, TMS_TASK_STACK_2K);
   
   if(err_qdi_client_handle < 0)
   {
     ERR_FATAL("err_qdi_client_init :qdi_open failed\n", 0, 0, 0);
     return;
   }

}  /* err_qdi_init */


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
)
{
  return qurt_qdi_handle_invoke(err_qdi_client_handle, ERR_QDI_COMMUNICATE_USER_CB, bucket, cb );

} /* err_qdi_client_communicate_user_cb */

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
)
{
  qurt_qdi_handle_invoke(err_qdi_client_handle, ERR_QDI_COMMUNICATE_TID,
		                tid, fault_pc, sp, badva, cause, fp, lr, ssr);
}  /* err_qdi_client_send_pdr_info */
