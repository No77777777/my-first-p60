#ifndef RCEVT_QURT_H
#define RCEVT_QURT_H
/** vi: tw=128 ts=3 sw=3 et :
@file rcevt_qurt.h
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
$Header: //components/rel/core.mpss/10.0/api/debugtools/rcevt_qurt.h#1 $
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

#include "qurt.h"
#include "stringl/stringl.h"

#include "rcevt.h"

#if defined(__cplusplus)
extern "C"
{
#endif

/** @addtogroup rcinit_api @{ */

////////////////////////////////////////
// REGISTER
////////////////////////////////////////

// qurt_anysignal_t client_qurt_signal;
// qurt_anysignal_init(&client_qurt_signal);
//
// if (RCEVT_NULL == rcevt_register_name_qurt(CLIENT_EVENTNAME, &client_qurt_signal, CLIENT_QURT_SIGNAL_MASK))
// {
// MSG("error"); // handle error
// }

////////////////////////////////////////
// BLOCKING WAIT
////////////////////////////////////////

// unsigned int recv = qurt_anysignal_wait(&qurt_signal, CLIENT_QURT_SIGNAL_MASK);
// if (0 != (recv & RCEVT_QURT_SIGNAL_MASK))
// {
// MSG("event handler");
// }

////////////////////////////////////////
// UNREGISTER
////////////////////////////////////////

// if (RCEVT_NULL == rcevt_unregister_name_qurt(CLIENT_EVENTNAME, &client_qurt_signal, CLIENT_QURT_SIGNAL_MASK))
// {
// MSG("error"); // handle error
// }
//
// qurt_anysignal_destroy(&qurt_signal);

////////////////////////////////////////
// SIGNAL FROM OTHER CONTEXT
////////////////////////////////////////

// if (RCEVT_NULL == rcevt_create_name(CLIENT_EVENTNAME)) // Prior to Use
// {
// MSG("error"); // handle error
// }

// if (RCEVT_NULL == rcevt_signal_name(CLIENT_EVENTNAME))
// {
// MSG("error"); // handle error
// }

/**
API, NHLOS Specific Structure
*/
typedef struct
{
   qurt_anysignal_t* signal;

   unsigned int mask;

} RCEVT_SIGEX_SIGQURT;

/**
API, Register notification to event, with handle
@param[in] Opaque handle of the event
@param[in] Notification NHLOS specific notification information
@param[in] Notification NHLOS specific notification information
@return
RCEVT_NULL -- Error
RCEVT_HANDLE -- Opaque handle to existing event
*/
DLL_API_WARN_UNUSED_RESULT
DLL_API_STATIC_INLINE RCEVT_HANDLE rcevt_register_handle_qurt(RCEVT_HANDLE const handle, qurt_anysignal_t* const signal, unsigned int const mask)
{
   RCEVT_HANDLE rc = RCEVT_NULL;

   if (RCEVT_NULL != handle && RCEVT_NULL != signal)
   {
      RCEVT_SIGEX_SIGQURT sigex;
      sigex.signal = signal;
      sigex.mask = mask;
      rc = rcevt_register_sigex_handle(handle, RCEVT_SIGEX_TYPE_SIGQURT, &sigex);
   }

   return rc;
}

/**
API, Register notification to event, with name
@param[in] NULL terminated string, name of the event
@param[in] Notification NHLOS specific notification information
@param[in] Notification NHLOS specific notification information
@return
RCEVT_NULL -- Error
RCEVT_HANDLE -- Opaque handle to existing event
*/
DLL_API_WARN_UNUSED_RESULT
DLL_API_STATIC_INLINE RCEVT_HANDLE rcevt_register_name_qurt(RCEVT_NAME const name, qurt_anysignal_t* const signal, unsigned int const mask)
{
   RCEVT_HANDLE rc = RCEVT_NULL;

   if (RCEVT_NULL != name && RCEVT_NULL != signal)
   {
      RCEVT_SIGEX_SIGQURT sigex;
      sigex.signal = signal;
      sigex.mask = mask;
      rc = rcevt_register_sigex_name(name, RCEVT_SIGEX_TYPE_SIGQURT, &sigex);
   }

   return rc;
}

/**
API, Unegister notification to event, with handle
@param[in] Opaque handle of the event
@param[in] Notification NHLOS specific notification information
@param[in] Notification NHLOS specific notification information
@return
RCEVT_NULL -- Error
RCEVT_HANDLE -- Opaque handle to existing event
*/
DLL_API_WARN_UNUSED_RESULT
DLL_API_STATIC_INLINE RCEVT_HANDLE rcevt_unregister_handle_qurt(RCEVT_HANDLE const handle, qurt_anysignal_t* const signal, unsigned int const mask)
{
   RCEVT_HANDLE rc = RCEVT_NULL;

   if (RCEVT_NULL != handle && RCEVT_NULL != signal)
   {
      RCEVT_SIGEX_SIGQURT sigex;
      sigex.signal = signal;
      sigex.mask = mask;
      rc = rcevt_unregister_sigex_handle(handle, RCEVT_SIGEX_TYPE_SIGQURT, &sigex);
   }

   return rc;
}

/**
API, Unegister notification to event, with name
@param[in] NULL terminated string, name of the event
@param[in] Notification NHLOS specific notification information
@param[in] Notification NHLOS specific notification information
@return
RCEVT_NULL -- Error
RCEVT_HANDLE -- Opaque handle to existing event
*/
DLL_API_WARN_UNUSED_RESULT
DLL_API_STATIC_INLINE RCEVT_HANDLE rcevt_unregister_name_qurt(RCEVT_NAME const name, qurt_anysignal_t* const signal, unsigned int const mask)
{
   RCEVT_HANDLE rc = RCEVT_NULL;

   if (RCEVT_NULL != name && RCEVT_NULL != signal)
   {
      RCEVT_SIGEX_SIGQURT sigex;
      sigex.signal = signal;
      sigex.mask = mask;
      rc = rcevt_unregister_sigex_name(name, RCEVT_SIGEX_TYPE_SIGQURT, &sigex);
   }

   return rc;
}

/** @} end_addtogroup rcinit_api */

#if defined(__cplusplus)
}
#endif

#endif
