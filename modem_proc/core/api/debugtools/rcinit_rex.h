#ifndef RCINIT_REX_H
#define RCINIT_REX_H
/** vi: tw=128 ts=3 sw=3 et :
@file rcinit_rex.h
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
Copyright (c) 2015-2016,2018 Qualcomm Technologies, Inc.
All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.
===============================================================================*/
/*=============================================================================
Edit History
$Header: //components/rel/core.mpss/10.0/api/debugtools/rcinit_rex.h#1 $
$DateTime: 2019/04/24 00:03:26 $
$Change: 18983508 $
$Author: pwbldsvc $
===============================================================================*/

#include "tms_dll_api.h"                                                         /**< MUST PRECEDE CLIENT DEFINITION OF DLL_API_EXPORT */

#if defined(DLL_API_BUILD_RCINIT)                                                /**< REQUIRED TO BE DECLARED IN TECH AREA SOURCE MODULES (SU/VU VISIBLE) */
#undef  DLL_API_EXPORT_RCINIT                                                    /**< REDEFINITION FOR THIS SPECIFIC MODULE */
#define DLL_API_EXPORT_RCINIT DLL_API_EXPORT_HELPER                              /**< CONTROL OVER DLL SYMBOL VISIBILITY BY EXPORT */
#else
#undef  DLL_API_EXPORT_RCINIT                                                    /**< REDEFINITION FOR THIS SPECIFIC MODULE */
#define DLL_API_EXPORT_RCINIT DLL_API_IMPORT_HELPER                              /**< CONTROL OVER DLL SYMBOL VISIBILITY BY EXPORT */
#endif

#include "rex.h"

#include "rcxh.h"
#include "rcecb.h"
#include "rcevt_rex.h"
#include "rcinit.h"

#if defined(__cplusplus)
extern "C"
{
#endif

/** @addtogroup rcinit_api @{ */

/**
API, Routine maps NHLOS task context identifier to RCINIT_INFO Opaque handle
@param[in] NHLOS task context identifier
@returns
RCINIT_GROUP -- Enumeration represents startup group for NHLOS tid
*/
DLL_API_WARN_UNUSED_RESULT 
CORE_WARN_DEPRECATED_MSG("Deprecated REX API","Migrate to new QuRT APIs")
DLL_API_EXPORT_RCINIT RCINIT_GROUP rcinit_lookup_group_rextask(rex_tcb_type* const tid);

/**
API, Routine maps NHLOS task name to RCINIT_INFO Opaque handle
@param[in] NULL terminated string, string identifier for task context
@returns
rex_tcb_type* -- NHLOS task context identifier
*/
DLL_API_WARN_UNUSED_RESULT 
CORE_WARN_DEPRECATED_MSG("Deprecated REX API","Migrate to new QuRT APIs")
DLL_API_EXPORT_RCINIT rex_tcb_type* rcinit_lookup_rextask(RCINIT_NAME const name);

// MAP Different NONHLOS Interfaces to RCINIT Priority Concept
// RCINIT priority database is 0 = low, 255 = high

#define RCINIT_MAP_PRIO_REX(x)         (x)

/** @} end_addtogroup rcinit_api */

#if defined(__cplusplus)
}
#endif

#endif
