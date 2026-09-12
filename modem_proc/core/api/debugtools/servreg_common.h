#ifndef SERVREG_COMMON_H
#define SERVREG_COMMON_H
/*
#============================================================================
#  Name:
#    servreg_common.h 
#
#  Description:
#     Common header file for Service Registry feature
#
# Copyright (c) 2015-2018 by Qualcomm Technologies, Inc.  All Rights Reserved.
#============================================================================
*/
#include "stdlib.h"

#if defined(__cplusplus)
extern "C"
{
#endif

#define SERVREG_NULL         ((void*)0)

typedef char * SERVREG_NAME;
typedef void* SERVREG_DL_HANDLE;
typedef void* SERVREG_MON_HANDLE;
typedef void* SERVREG_SIGEX;

typedef unsigned long SERVREG_CB_PARAM;
typedef void (*SERVREG_FNSIG_CALLBACK)(SERVREG_CB_PARAM, SERVREG_CB_PARAM);

typedef enum
{
   SERVREG_FALSE = 0,
   SERVREG_TRUE  = 1
} SERVREG_BOOL;

typedef enum 
{
   SERVREG_SUCCESS        = 0,
   SERVREG_SYNCED         = 1,
   SERVREG_FAILURE        = -1,
   SERVREG_IN_PROGRESS    = -2,
   SERVREG_OVERFLOW       = -3,
   SERVREG_INVALID_HANDLE = -4,
   SERVREG_INVALID_PARAM  = -5,
   SERVREG_NO_MEMORY      = -6
}SERVREG_RESULT;

typedef enum
{
   SERVREG_SERVICE_STATE_DOWN           = 0x0FFFFFFF,           /* Logically exists right at this instance but not expected to respond */
   SERVREG_SERVICE_STATE_UP             = 0x1FFFFFFF,           /* Logically exists right at this instance */
   SERVREG_SERVICE_STATE_EARLY_DOWN     = 0x2FFFFFFF,           /* Logically exists right at this instance */
   SERVREG_SERVICE_STATE_UNINIT         = 0x7FFFFFFF            /* Logically never has existed */
}SERVREG_SERVICE_STATE;

/** =====================================================================
API, Enumeration of Supported Notification Types
* =====================================================================  */
typedef enum
{
   SERVREG_SIGEX_TYPE_UNKNOWN   = 0x00,
   SERVREG_SIGEX_TYPE_SIGQURT   = 0x33,
   SERVREG_SIGEX_TYPE_SIGREX    = 0x35,
   SERVREG_SIGEX_TYPE_NONE      = 0xff
} SERVREG_SIGEX_TYPE;

typedef enum
{
  RELATIONSHIP_NOT_ESTABLISHED = 0,
  WE_ARE_A_CHILD_PROCESS_AND_THIS_IS_A_SERVICE_IN_SAME_PROCESS,
  WE_ARE_A_CHILD_PROCESS_AND_THIS_IS_A_SERVICE_IN_OUR_PARENT,
  WE_ARE_A_CHILD_PROCESS_AND_THIS_IS_A_SERVICE_IN_A_SIBLING_PROCESS,
  WE_ARE_A_CHILD_PROCESS_AND_THIS_IS_A_SERVICE_OF_A_DIFFERNT_PARENT,
  WE_ARE_A_PARENT_PROCESS_AND_THIS_IS_A_SERVICE_IN_SAME_PROCESS,
  WE_ARE_A_PARENT_PROCESS_AND_THIS_IS_A_SERVICE_IN_CHILD_PROCESS,
  WE_ARE_A_PARENT_PROCESS_AND_THIS_IS_A_SERVICE_OF_A_DIFFERENT_PARENT,
}servreg_relationship_type;

#define SR_INITIAL_TRANSACTION_ID -1
#define SR_INVALID_TRANSACTION_ID -2

#if defined(__cplusplus)
}
#endif

#endif
