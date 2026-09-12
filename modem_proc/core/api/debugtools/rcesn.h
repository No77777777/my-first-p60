#ifndef RCESN_H
#define RCESN_H
/** vi: tw=128 ts=3 sw=3 et :
@file rcesn.h
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
$Header: //components/rel/core.mpss/10.0/api/debugtools/rcesn.h#1 $
$DateTime: 2019/04/24 00:03:26 $
$Change: 18983508 $
$Author: pwbldsvc $
===============================================================================*/

// CLIENTS WILL USE THE APPROPRIATE NHLOS HEADER FILE TO ACCESS THE API AS DEPENDENCY.
// DIRECT CLIENT DEPENDENCY THROUGH THIS HEADER FILE ALONE IS NOT A SUPPORTED OPERATION.

// #include "rcesn_dal.h"     // NHLOS DAL
// #include "rcesn_posix.h"   // NHLOS POSIX
// #include "rcesn_qurt.h"    // NHLOS QURT
// #include "rcesn_rex.h"     // NHLOS REX

#include "tms_dll_api.h"                                                         /**< MUST PRECEDE CLIENT DEFINITION OF DLL_API_EXPORT */

#if defined(DLL_API_BUILD_RCINIT)                                                /**< REQUIRED TO BE DECLARED IN TECH AREA SOURCE MODULES (SU/VU VISIBLE) */
/* Export Default to be LOCAL for .SO */
#undef  DLL_API_EXPORT_RCINIT                                                    /**< REDEFINITION FOR THIS SPECIFIC MODULE */
#define DLL_API_EXPORT_RCINIT DLL_API_LOCAL                              /**< CONTROL OVER DLL SYMBOL VISIBILITY BY EXPORT */
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
typedef char const* RCESN_NAME;                                                  /**< Name */
typedef void* RCESN_HANDLE;                                                      /**< Opaque Handle */
typedef enum { RCESN_FALSE = 0, RCESN_TRUE } RCESN_BOOL;                         /**< Boolean Query Return */
typedef unsigned long RCESN_THRESHOLD;                                           /**< Threshold */
typedef unsigned long RCESN_STATE;                                               /**< State Value */
typedef RCESN_BOOL (*RCESN_FNSIG_COMPARE)(RCESN_HANDLE const);                   /**< Comparision Function (handle to 'curr' for registrant) */

/**
API, Enumeration of Supported Notification Types
*/
typedef enum
{
   RCESN_SIGEX_TYPE_UNKNOWN            = 0x00,
   RCESN_SIGEX_TYPE_SIGNATIVE          = 0x30,
   RCESN_SIGEX_TYPE_SIGDAL             = 0x31,
   RCESN_SIGEX_TYPE_SIGPOSIX           = 0x32,
   RCESN_SIGEX_TYPE_SIGQURT            = 0x33,
   RCESN_SIGEX_TYPE_SIGQURT_USER       = 0x34,
   RCESN_SIGEX_TYPE_SIGREX             = 0x35,
   RCESN_SIGEX_TYPE_NONE               = 0xff                                    /**< RCEVT [0x30..0x3f] */

} RCESN_SIGEX_TYPE;                                                              /**< Enum Values DO NOT Overlap RCEVT, RCECB, RCESN */

typedef void* RCESN_SIGEX;                                                       /**< Opaque Handle to Abstracted Event Notification Structures */

#define RCESN_ZERO   0                                                           /**< Manifest Constant Assist */
#define RCESN_NULL   (void*)0                                                    /**< ISO/ANSI-C, Forward reference NULL pointer */

/** @} end_addtogroup rcinit_api */

#if defined(__cplusplus)
}
#endif

#endif
