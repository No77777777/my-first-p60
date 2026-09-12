#ifndef RCECB_H
#define RCECB_H
/** vi: tw=128 ts=3 sw=3 et :
@file rcecb.h
@brief This file contains the API for the Run Control Framework, API 3.0
*/
/*=============================================================================
NOTE: The @brief description above does not appear in the PDF.
The tms_mainpage.dox file contains the group/module descriptions that
are displayed in the output PDF generated using Doxygen and LaTeX. To
edit or update any of the group/module text in the PDF, edit the
tms_mainpage.dox file or contact Tech Pubs.
===============================================================================*/
/*=============================================================================
Copyright (c) 2015-2016 Qualcomm Technologies, Inc.
All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.
===============================================================================*/
/*=============================================================================
Edit History
$Header: //components/rel/core.mpss/10.0/api/debugtools/rcecb.h#1 $
$DateTime: 2019/04/24 00:03:26 $
$Change: 18983508 $
$Author: pwbldsvc $
===============================================================================*/

#include "tms_dll_api.h"                                                         /**< MUST PRECEDE CLIENT DEFINITION OF DLL_API_EXPORT */

#if defined(DLL_API_BUILD_RCINIT)                                                /**< REQUIRED TO BE DECLARED IN TECH AREA SOURCE MODULES (SU/VU VISIBLE) */
#undef  DLL_API_EXPORT_RCINIT                                                    /**< REDEFINITION FOR THIS SPECIFIC MODULE */
#define DLL_API_EXPORT_RCINIT DLL_API_LOCAL                                      /**< CONTROL OVER DLL SYMBOL VISIBILITY BY EXPORT */
#else
#undef  DLL_API_EXPORT_RCINIT                                                    /**< REDEFINITION FOR THIS SPECIFIC MODULE */
#define DLL_API_EXPORT_RCINIT DLL_API_IMPORT_HELPER                              /**< CONTROL OVER DLL SYMBOL VISIBILITY BY EXPORT */
#endif

#include "timer.h"

#if defined(__cplusplus)
extern "C"
{
#endif

/** @addtogroup rcinit_api @{ */

/**
API, Typedefs and Enumerations
*/
typedef char const* RCECB_NAME;                                                  /**< Name */
typedef void* RCECB_HANDLE;                                                      /**< Opaque Handle */
typedef enum { RCECB_FALSE = 0, RCECB_TRUE } RCECB_BOOL;                         /**< Boolean Query Return */

typedef unsigned long RCECB_PARM;

typedef void* RCECB_CALLBACK_FNSIG;                                              /**< Function Pointer Handle */
typedef void (*RCECB_CALLBACK_FNSIG_P0)(void);                                   /**< Specialized Function Pointer */
typedef void (*RCECB_CALLBACK_FNSIG_P1)(RCECB_PARM);                             /**< Specialized Function Pointer */
typedef void (*RCECB_CALLBACK_FNSIG_P2)(RCECB_PARM, RCECB_PARM);                 /**< Specialized Function Pointer */

/**
API, Enumeration of Supported Notification Types
*/
typedef enum
{
   RCECB_SIGEX_TYPE_UNKNOWN            = 0x00,
   RCECB_SIGEX_TYPE_SIGNATIVE          = 0x10,
   RCECB_SIGEX_TYPE_CALLBACK_P0        = 0x11,
   RCECB_SIGEX_TYPE_CALLBACK           = RCECB_SIGEX_TYPE_CALLBACK_P0,           /**< Obsolete */
   RCECB_SIGEX_TYPE_CALLBACK_P1        = 0x12,
   RCECB_SIGEX_TYPE_CALLBACK_P2        = 0x13,
   RCECB_SIGEX_TYPE_NONE               = 0xff                                    /**< RCECB [0x10..0x1f] */

} RCECB_SIGEX_TYPE;                                                              /**< Specialized Enum Values DO NOT Overlap RCEVT, RCECB, RCESN */

typedef void* RCECB_SIGEX;                                                       /**< Opaque Handle to Abstracted Event Notification Structures */

#define RCECB_ZERO   0                                                           /**< Manifest Constant Assist */
#define RCECB_NULL   (void*)0                                                    /**< ISO/ANSI-C, Forward reference NULL pointer */

/**
API, Register callback to event, with handle
See prerequsite note with rcecb_create_name
@param[in] Opaque handle of the event
@param[in] Callback function pointer
@return
RCECB_NULL -- Error
RCECB_HANDLE -- Opaque handle to existing event
*/
DLL_API_WARN_UNUSED_RESULT 
DLL_API_EXPORT_RCINIT RCECB_HANDLE rcecb_register_handle(RCECB_HANDLE const handle, RCECB_CALLBACK_FNSIG const fnsig);

/**
API, Register callback to event, with name
See prerequsite note with rcecb_create_name
@param[in] NULL terminated string, name of the event
@param[in] Callback function pointer
@return
RCECB_NULL -- Error
RCECB_HANDLE -- Opaque handle to existing event
*/
DLL_API_WARN_UNUSED_RESULT 
DLL_API_GLOBAL RCECB_HANDLE rcecb_register_name(RCECB_NAME const name, RCECB_CALLBACK_FNSIG const fnsig);

CORE_WARN_DEPRECATED_MSG_ATTR("Deprecated API","Migrate to new API rcecb_register_name")
DLL_API_STATIC_INLINE RCECB_HANDLE rcecb_register_context_name(RCECB_NAME const name, RCECB_CALLBACK_FNSIG const fnsig) /**< OBSOLETE; CLIENTS USE rcecb_register_name */
{
   return rcecb_register_name(name, fnsig);
}

/**
API, Unegister callback to event, with handle
@param[in] Opaque handle of the event
@param[in] Callback function pointer
@return
RCECB_NULL -- Error
RCECB_HANDLE -- Opaque handle to existing event
*/
DLL_API_WARN_UNUSED_RESULT 
DLL_API_EXPORT_RCINIT RCECB_HANDLE rcecb_unregister_handle(RCECB_HANDLE const handle, RCECB_CALLBACK_FNSIG const fnsig);

/**
API, Unegister callback to event, with name
@param[in] NULL terminated string, name of the event
@param[in] Callback function pointer
@return
RCECB_NULL -- Error
RCECB_HANDLE -- Opaque handle to existing event
*/
DLL_API_WARN_UNUSED_RESULT 
DLL_API_EXPORT_RCINIT RCECB_HANDLE rcecb_unregister_name(RCECB_NAME const name, RCECB_CALLBACK_FNSIG const fnsig);

CORE_WARN_DEPRECATED_MSG("Deprecated API","Migrate to new API rcecb_unregister_name")
DLL_API_STATIC_INLINE RCECB_HANDLE rcecb_unregister_context_name(RCECB_NAME const name, RCECB_CALLBACK_FNSIG const fnsig) /**< OBSOLETE; CLIENTS USE rcecb_unregister_name */
{
   return rcecb_unregister_name(name, fnsig);
}

/** @} end_addtogroup rcinit_api */

#if defined(__cplusplus)
}
#endif

#endif
