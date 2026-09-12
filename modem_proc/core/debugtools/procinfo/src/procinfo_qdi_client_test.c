/*
#============================================================================
#  Name:
#    procinfo_qdi_client_test.c 
#
#  Description:
#    Process Information QDI Client Process Code for test purposes 
#
# Copyright (c) 2018 by Qualcomm Technologies, Inc.  All Rights Reserved.
#============================================================================
*/
#include "stdarg.h"
#include "stdlib.h"
#include <stringl/stringl.h>

#include "comdef.h"               /* Definitions for byte, word, etc.     */

#include "tms_utils.h"
#include "tms_utils_msg.h"
#include "procinfo.h"

PROCINFO_RESULT procinfo_ret_internal_name = PROCINFO_QDI_FAILURE;
PROCINFO_RESULT procinfo_ret_domain_name = PROCINFO_QDI_FAILURE;
PROCINFO_RESULT procinfo_ret_oem_name = PROCINFO_QDI_FAILURE;

char procinfo_test_internal_name[PROCINFO_NAME_MAX_LEN] = {0};
char procinfo_test_domain_name[PROCINFO_NAME_MAX_LEN] = {0};
char procinfo_test_oem_name[PROCINFO_NAME_MAX_LEN] = {0};

/** =====================================================================
 * Function:
 *     procinfo_qdi_client_test
 *
 * Description:
 *     Test function for clients of procinfo.
 *
 * Parameters:
 *     None
 *
 * Returns:
 *     None
 * =====================================================================  */
void procinfo_qdi_client_test(void)
{ 
  //internal_name
  procinfo_ret_internal_name = procinfo_qdi_invoke_op(PROCINFO_OP_GET_NAME, PROCINFO_INTERNAL_NAME_TYPE, procinfo_test_internal_name, PROCINFO_NAME_MAX_LEN);
  if (PROCINFO_QDI_SUCCESS == procinfo_ret_internal_name)
  {
    TMS_MSG_SPRINTF_HIGH_1("internal_name: %s", procinfo_test_internal_name);
  }
  else
  {
    TMS_MSG_ERROR_1("procinfo_ret_internal_name %d", procinfo_ret_internal_name);
  }
  
  //domain_name 
  procinfo_ret_domain_name = procinfo_qdi_invoke_op(PROCINFO_OP_GET_NAME, PROCINFO_DOMAIN_NAME_TYPE, procinfo_test_domain_name, PROCINFO_NAME_MAX_LEN);
  if (PROCINFO_QDI_SUCCESS == procinfo_ret_domain_name)
  {
    TMS_MSG_SPRINTF_HIGH_1("domain_name: %s", procinfo_test_domain_name);
  }
  else
  {
    TMS_MSG_ERROR_1("procinfo_ret_domain_name %d", procinfo_ret_domain_name);
  }

  //oem_name
  procinfo_ret_oem_name = procinfo_qdi_invoke_op(PROCINFO_OP_GET_NAME, PROCINFO_OEM_NAME_TYPE, procinfo_test_oem_name, PROCINFO_NAME_MAX_LEN);
  if (PROCINFO_QDI_SUCCESS == procinfo_ret_oem_name)
  {
    TMS_MSG_SPRINTF_HIGH_1("oem_name: %s", procinfo_test_oem_name);
  }
  else
  {
    TMS_MSG_ERROR_1("procinfo_ret_oem_name %d", procinfo_ret_oem_name);
  }
}