/*======================================================================
                        core_api_version.S 

GENERAL DESCRIPTION
 This file is to provide CORE API version
  [Major revision #] â€“ Major # change in api_version.h will be incremented, 
    if user pd must upgrade or non-backwards compatible change made through API definition.  
    This could include, but not limited to, any # of  existing function parameter change, type of function parameter change or return type of function change.  
  [Minor revsion #]  Minor # change in api_version.h. 
    Transparent change in published APIs, i.e. user PDs donâ€™t have to be upgraded, and changes  remain backwards compatible, in terms of API definition. 
    This could include, but not limited to, any bug fix for existing APIs or  newly added APIs. This is the default tag by PW tool. No action to be made by developer. 
EXTERNALIZED FUNCTIONS
  none

INITIALIZATION AND SEQUENCING REQUIREMENTS
  none

 Copyright (c) 2016  by Qualcomm Technologies, Inc.  All Rights Reserved.
 ======================================================================*/
/*======================================================================
 *
 *                       EDIT HISTORY FOR FILE
 *
 *   This section contains comments describing changes made to the
 *   module. Notice that changes are listed in reverse chronological
 *   order.
 *
 *  
 *
 *
 * when         who     what, where, why
 * ----------   ---     ------------------------------------------------
 * 09/08/2016   tkuo    Enable CORE API runtime check.
 * 08/26/2016   tkuo    Add runtime check support for CORE_API_VERSION.
 * 07/27/2016   tkuo    created. 
 ======================================================================*/
 /* __ASSEMBLY__ must be defined first before using api_version.h */
#define __ASSEMBLY__
#include <api_version.h>

  .section ".core_api_version","a", @note
  .global core_api_version_note
  .align 4
  .type core_api_version_note, @object
  core_api_version_note:
    .string COMPONENT_NAME
    .string API_VER_MAJOR_NAME
    .string API_VER_MAJOR 
    .string API_VER_MINOR_NAME  
    .string API_VER_MINOR  
  .size core_api_version_note,.-core_api_version_note

  .section ".text","ax", @progbits
/* query funtion call to core_api_version_note*/   
  .type core_api_version_query_func, @function
  .global core_api_version_query_func
  .hidden core_api_version_query_func
  core_api_version_query_func:
  r0 = add(pc, ##core_api_version_note@PCREL)
  jumpr r31
  .size core_api_version_query_func,.-core_api_version_query_func
