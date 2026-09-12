/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

         E R R O R     R E P O R T I N G    S E R V I C E S

GENERAL DESCRIPTION
  This module provides error related functionality for root PD.

Copyright (c) 2015 - 2019 by Qualcomm Technologies, Inc.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        Edit History

$Header: //components/rel/core.mpss/10.0/debugtools/err/src/err_user.c#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/27/15   din     Adapted from err.c

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "qurt.h"
#include "erri.h"
#include <stdlib.h>
#include <stringl.h>
#include "err_qdi_client.h"
#include "err_smem_log.h"
#include "tms_utils.h"
#include "qurt_process.h"


/*===========================================================================

                 Defines and variable declarations for module

===========================================================================*/

/*===========================================================================

                              Function definitions

===========================================================================*/

void err_execute_external_cb_post_STM_helper_w(unsigned int pid)
{
  err_execute_external_cb_post_STM_helper();
}

void err_execute_external_cb_normal_helper_w(unsigned int pid)
{
  err_execute_external_cb_normal_helper();
}

void err_execute_external_cb_late_helper_w(unsigned int pid)
{
  err_execute_external_cb_late_helper();
}

void err_execute_external_cb_last_helper_w(unsigned int pid)
{
  err_execute_external_cb_last_helper();
}

void error_register_qurt_cb(err_cb_bucket_t bucket)
{
  qurt_cb_data_t cb_data;
  cb_data.cb_arg = 0;

  /*QuRT has 0-255 buckets. We can register any number of callbacks in each bucket.
    Order of execution of buckets is 0,1,2,...255 . Lower Number , first execution.
    In each bucket , order of execution will be "first register , first serve".
    We are mapping error buckets to QuRT buckets here like below :
    0 = PRE STM , 1 = POST STM , 2 = NORMAL , 3 = LATE , 4 = LAST */

  switch(bucket)
  {
    case  ERRCB_BUCKET_POST_STM :
      cb_data.cb_func = &err_execute_external_cb_post_STM_helper_w;
      (void)qurt_process_dump_register_cb(&cb_data,QURT_PROCESS_DUMP_CB_ERROR,1);
      break;

    case  ERRCB_BUCKET_NORMAL:
      cb_data.cb_func = &err_execute_external_cb_normal_helper_w;
      (void)qurt_process_dump_register_cb(&cb_data,QURT_PROCESS_DUMP_CB_ERROR,2);
      break;;

    case  ERRCB_BUCKET_LATE:
      cb_data.cb_func = &err_execute_external_cb_late_helper_w;
      (void)qurt_process_dump_register_cb(&cb_data,QURT_PROCESS_DUMP_CB_ERROR,3);
      break;

    case  ERRCB_BUCKET_LAST:
      cb_data.cb_func = &err_execute_external_cb_last_helper_w;
      (void)qurt_process_dump_register_cb(&cb_data,QURT_PROCESS_DUMP_CB_ERROR,4);
      break;

    default:
      break;

  };
}

/*===========================================================================

FUNCTION       err_execute_external_cb_pre_STM_helper

DESCRIPTION
  Abstraction for err_execute_external_cb_pre_STM API

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void err_execute_external_cb_pre_STM_helper(void)
{
  /* passing always 0 since QuRT does not have support to pass
    argument with callback at QDI layer, and we need to call
    user pre STM CBs while root pd crash */
  err_execute_external_cb_pre_STM(0);
}

void err_put_log (word line, const char *file_ptr)
{
  //err logging is not supported in this build
}

/*=========================================================================

FUNCTION err_user_qdi_communicate

DESCRIPTION
  Registers CB with Root

DEPENDENCIES
  None

RETURN VALUE
  TRUE if successfull
  FALSE if failed

SIDE EFFECTS
  None

===========================================================================*/

void err_user_qdi_communicate(err_cb_bucket_t bucket)
{
  switch(bucket)
  {
    case ERRCB_BUCKET_PRE_STM:
      err_qdi_client_communicate_user_cb( ERRCB_BUCKET_PRE_STM, err_execute_external_cb_pre_STM_helper);
      /* Spawn user exception handler if not done */
      err_spawn_exception_handler();
      break;

    case  ERRCB_BUCKET_POST_STM :
      err_qdi_client_communicate_user_cb( ERRCB_BUCKET_POST_STM, err_execute_external_cb_post_STM_helper);
      error_register_qurt_cb(ERRCB_BUCKET_POST_STM);
      break;

    case  ERRCB_BUCKET_NORMAL:
      err_qdi_client_communicate_user_cb( ERRCB_BUCKET_NORMAL, err_execute_external_cb_normal_helper);
      error_register_qurt_cb(ERRCB_BUCKET_NORMAL);
      break;

    case  ERRCB_BUCKET_LATE:
      err_qdi_client_communicate_user_cb( ERRCB_BUCKET_LATE, err_execute_external_cb_late_helper);
      error_register_qurt_cb(ERRCB_BUCKET_LATE);
      break;

    case  ERRCB_BUCKET_LAST:
      err_qdi_client_communicate_user_cb( ERRCB_BUCKET_LAST, err_execute_external_cb_last_helper);
      error_register_qurt_cb(ERRCB_BUCKET_LAST);
      break;

    default:
      break;

  };
}

/*=========================================================================

FUNCTION err_set_image_version_info

DESCRIPTION
  Error set image versioning information. This is enabled only for user PD. 
  Should be only called after err_init is called.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

DLL_API_GLOBAL void err_set_image_version_info
( 
  char * arg_QC_IMAGE_VERSION_STRING_AUTO_UPDATED,
  char * arg_IMAGE_VARIANT_STRING_AUTO_UPDATED,
  char * arg_OEM_IMAGE_VERSION_STRING_AUTO_UPDATED,
  char * arg_OEM_IMAGE_UUID_STRING_AUTO_UPDATED
)
{
  strlcpy(coredump.image.qc_image_version_string, 
          arg_QC_IMAGE_VERSION_STRING_AUTO_UPDATED, ERR_IMAGE_VERSION_SIZE); 
  strlcpy(coredump.image.image_variant_string, 
          arg_IMAGE_VARIANT_STRING_AUTO_UPDATED, ERR_IMAGE_VERSION_SIZE);
  strlcpy(coredump.image.oem_image_version_string, 
          arg_OEM_IMAGE_VERSION_STRING_AUTO_UPDATED,ERR_IMAGE_VERSION_SIZE);
  strlcpy(coredump.image.oem_image_uuid_string,
          arg_OEM_IMAGE_UUID_STRING_AUTO_UPDATED, ERR_IMAGE_VERSION_SIZE);  
}
