#ifndef TMS_RCEVT_H
#define TMS_RCEVT_H
/** vi: tw=128 ts=3 sw=3 et :
@file tms_rcevt.h
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
Copyright (c) 2015-2016,18 by Qualcomm Technologies, Inc.  All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.
===============================================================================*/
/*=============================================================================
Edit History
$Header: //components/rel/core.mpss/10.0/debugtools/api/tms_rcevt.h#1 $
$DateTime: 2019/04/24 00:03:26 $
$Change: 18983508 $
$Author: pwbldsvc $
===============================================================================*/

#include "tms_dll_api.h"                                                         /**< MUST PRECEDE CLIENT DEFINITION OF DLL_API_EXPORT */

#include "stdlib.h"
#include "DALSys.h"
#include "timer.h"

#include "rcecb.h"
#include "rcesn.h"
#include "rcevt.h"
#include "rcxh.h"

#include "err.h"

#if defined(__cplusplus)
extern "C"
{
#endif

/////////////////////////////////////////////////////////////////////
// Tuneable Controls
/////////////////////////////////////////////////////////////////////

#define RCEVT_NODE_POOL_SIZ      16                                              // node_pool storage allocation (minimum size)
#define RCEVT_NOTIFY_POOL_SIZ    RCEVT_NODE_POOL_SIZ                             // notify_pool storage allocation (minimum size)
#define RCEVT_HASHTABLE_BUCKETS  31                                              // hashtable buckets storage allocation (minimum size)
#define RCEVT_SIGEX_MAXSIZ       3                                               // maximum size of the sigex.parm[] union/struct, pointers (maximum size)

/////////////////////////////////////////////////////////////////////
// Localized Manifest Constants
/////////////////////////////////////////////////////////////////////

#define RCE_NULL                 ((void*)0)
#define RCE_THRESHOLD_INIT       (0)
#define RCE_THRESHOLD_DEFAULT    (1)
#define RCE_STATE_INIT           (0)
#define RCE_STATE_DEFAULT        (0)
#define RCE_NDE_COOKIE           (0xc00c1e00)                                    // cookie marker
#define RCE_NFY_COOKIE           (0xc00c1e11)                                    // cookie marker

/* Controls default, API is present for clients to specify their own timeout
*/
#if defined(RCINIT_SPI_NOR_TIMEOUTS)
#define RCECB_CALLBACK_MAX_TIME     30000                                        // maximum allocated callback execution time before ERR_FATAL (Unoptimized at 10s)
#else
#define RCECB_CALLBACK_MAX_TIME     10000                                        // maximum allocated callback execution time before ERR_FATAL (Unoptimized at 10s)
#endif
#define RCESN_CALLBACK_MAX_TIME     100                                          // maximum allocated callback execution time before ERR_FATAL
#define RCEVT_CALLBACK_MAX_TIME     100                                          // maximum allocated callback execution time before ERR_FATAL

/////////////////////////////////////////////////////////////////////
// Localized Type Declarations
/////////////////////////////////////////////////////////////////////

typedef struct
{
#if 1 // !defined(RCINIT_EXCLUDE_KERNEL_DAL)
   DALSYSSyncHandle mutex_dal;

#elif 0 // !defined(RCINIT_EXCLUDE_KERNEL_POSIX)
   pthread_mutex_t mutex_posix;

#elif 0 // !defined(RCINIT_EXCLUDE_KERNEL_QURT)
   qurt_mutex_t mutex_qurt;

#endif

} rce_mutex_t, * rce_mutex_p;

typedef unsigned long rce_sigex_t;

typedef unsigned long rce_hash_t;

typedef unsigned long rce_cookie_t;

typedef timer_type rce_expiry_t;

struct rce_nfy_sigex_s                                                           // sigex overlay storage based on type
{
   void* parm[RCEVT_SIGEX_MAXSIZ];                                               // reflects maximum size (pointers)
};

// RCE Internal Signaling Structure
/////////////////////////////////////////////////////////////////////

struct rce_nfy_s
{
   rce_sigex_t sigex_type;                                                       // sigex type storage

   rce_expiry_t expiry;                                                          // expiry timer

   RCEVT_THRESHOLD threshold;                                                    // threshold value (RCEVT ONLY)

   struct rce_nfy_sigex_s sigex;                                                 // sigex overlay storage based on type

   struct rce_nfy_s* next;                                                       // next in notify queue

   rce_cookie_t nfy_cookie;                                                      // init cookie
};

typedef struct rce_nfy_s rce_nfy_t, * rce_nfy_p;

// RCE Internal Event Structure
/////////////////////////////////////////////////////////////////////

struct rce_nde_s
{
   RCEVT_NAME name;                                                              // name text

   rce_expiry_t expiry;                                                          // expiry timer

   RCEVT_THRESHOLD threshold;                                                    // threshold value (RCEVT ONLY)

   RCESN_STATE state_curr;                                                       // state data current (RCESN ONLY)

   RCESN_STATE state_prev;                                                       // state data previous (RCESN ONLY)

   struct rce_nfy_s* notify_head;                                                // head notify queue

   struct rce_nde_s* next;                                                       // next node

   rce_mutex_t mutex;                                                            // mutex

   rce_cookie_t nde_cookie;                                                      // init cookie
};

typedef struct rce_nde_s rce_nde_t, * rce_nde_p;

// RCE Internal Signaling Structure Pool
/////////////////////////////////////////////////////////////////////

typedef struct rce_nfy_pool_s
{
   struct rce_nfy_s nfy_pool[RCEVT_NOTIFY_POOL_SIZ];

   struct rce_nfy_pool_s* next;                                                  // dynamic use; pointers to pool

} rce_nfy_pool_t, * rce_nfy_pool_p;                                              // storage type

// RCE Internal Event Structure Pool
/////////////////////////////////////////////////////////////////////

typedef struct rce_nde_pool_s
{
   struct rce_nde_s nde_pool[RCEVT_NODE_POOL_SIZ];

   struct rce_nde_pool_s* next;                                                  // dynamic use; pointers to pool

} rce_nde_pool_t, * rce_nde_pool_p;                                              // storage type

struct rce_internal_s
{
   rce_nde_pool_p nde_pool_head_p;                                               // dynamic storage tracking

   rce_nfy_pool_p nfy_pool_head_p;                                               // dynamic storage tracking

   rce_nfy_p nfy_pool_free_p;                                                    // next allocation head

   rce_nde_p nde_pool_free_p;                                                    // next allocation head

   rce_mutex_t mutex;                                                            // critical section mutex protects localized storage

   rce_mutex_t mutex_create;

   boolean dynamic_use;                                                          // allow dynamic pools expansion

   unsigned long init_flag;                                                      // service init

};

extern struct rce_internal_s rce_internal;                                       // rce internal instruments

extern rce_nfy_pool_t rce_nfy_pool_static;                                       // first pool static storage

extern rce_nde_pool_t rce_nde_pool_static;                                       // first pool static storage

extern rce_nde_p rce_hashtab[RCEVT_HASHTABLE_BUCKETS];                           // hashtable lookup buckets

/////////////////////////////////////////////////////////////////////
// Internal Forward Declarations
/////////////////////////////////////////////////////////////////////

#define nde2rcecb(x)    ((RCECB_HANDLE)x)                                        // typecasts as accessor functions
#define nde2rcesn(x)    ((RCESN_HANDLE)x)                                        // typecasts as accessor functions
#define nde2rcevt(x)    ((RCEVT_HANDLE)x)                                        // typecasts as accessor functions

#define rcecb2nde(x)    ((rce_nde_p)x)                                           // typecasts as accessor functions
#define rcesn2nde(x)    ((rce_nde_p)x)                                           // typecasts as accessor functions
#define rcevt2nde(x)    ((rce_nde_p)x)                                           // typecasts as accessor functions

rce_nde_p rce_nde_hashtab_get(rce_hash_t const hash);
void rce_nde_hashtab_put(rce_nde_p const nde_p, rce_hash_t const hash);
rce_hash_t rce_nmehash(RCEVT_NAME const name);
int rce_nmelen(RCEVT_NAME const name);
int rce_nmecmp(RCEVT_NAME const name_1, RCEVT_NAME const name_2, int len);
rce_nfy_p rce_nfy_pool_init(void);
rce_nfy_p rce_nfy_pool_alloc(void);
rce_nfy_p rce_nfy_pool_free(rce_nfy_p const nfy_p);
rce_nde_p rce_nde_pool_init(void);
rce_nde_p rce_nde_pool_alloc(void);
rce_nde_p rce_nde_pool_free(rce_nde_p const nde_p);
void rce_hashtab_init(void);
void rce_init(void);
void rce_term(void);

void rce_mutex_init_dal(rce_mutex_p mutex_p);
void rce_mutex_lock_dal(rce_mutex_p mutex_p);
void rce_mutex_unlock_dal(rce_mutex_p mutex_p);

void rce_mutex_init_posix(rce_mutex_p mutex_p);
void rce_mutex_lock_posix(rce_mutex_p mutex_p);
void rce_mutex_unlock_posix(rce_mutex_p mutex_p);

void rce_mutex_init_qurt(rce_mutex_p mutex_p);
void rce_mutex_lock_qurt(rce_mutex_p mutex_p);
void rce_mutex_unlock_qurt(rce_mutex_p mutex_p);

/**
API, Service initialization
@param
None
@return
None
*/
DLL_API_WARN_UNUSED_RESULT
DLL_API_LOCAL RCECB_BOOL rcecb_init(void);

/**
API, Service termination
@param
None
@return
None
*/
DLL_API_WARN_UNUSED_RESULT
DLL_API_LOCAL RCECB_BOOL rcecb_term(void);


/**
API, Service initialization
@param
None
@return
None
*/
DLL_API_WARN_UNUSED_RESULT
DLL_API_LOCAL RCEVT_BOOL rcevt_init(void);

/**
API, Service termination
@param
None
@return
None
*/
DLL_API_WARN_UNUSED_RESULT
DLL_API_LOCAL RCEVT_BOOL rcevt_term(void);

/**
API, Service initialization
@param
None
@return
None
*/
DLL_API_WARN_UNUSED_RESULT
DLL_API_LOCAL RCESN_BOOL rcesn_init(void);

/**
API, Service termination
@param
None
@return
None
*/
DLL_API_WARN_UNUSED_RESULT
DLL_API_LOCAL RCESN_BOOL rcesn_term(void);

/**
Initialization of service prior to use
@return
None.
*/
DLL_API_WARN_UNUSED_RESULT
DLL_API_LOCAL RCXH_BOOL rcxh_init(void);

/**
Termination of service following use
@return
None.
*/
DLL_API_WARN_UNUSED_RESULT
DLL_API_LOCAL RCXH_BOOL rcxh_term(void);

/////////////////////////////////////////////////////////////////////
// RCECB
/////////////////////////////////////////////////////////////////////

/**
API, Create an event, with name

Prerequsite call before posting event -- create the event
before use to avoid a potential "race condition" between
contexts that will post the event and concurrently wait on
the event being posted.

For performance, cache the resulting handle for use within
the API. Name based use cases will take a penalty when doing
a dictionary lookup on every call.

@param[in] NULL terminated string, name of the event
@return
RCECB_NULL -- Error
RCECB_HANDLE -- Opaque handle to existing event
*/
DLL_API_WARN_UNUSED_RESULT 
RCECB_HANDLE rcecb_create_name(RCECB_NAME const name);

/**
API, Search an event, with handle
See prerequsite note with rcecb_create_name
@param[in] Opaque handle of the event
@param[in] Callback function pointer
@return
RCECB_BOOL -- Boolean reflecting event previously created
*/
DLL_API_WARN_UNUSED_RESULT  
RCECB_BOOL rcecb_search_handle(RCECB_HANDLE const handle, RCECB_CALLBACK_FNSIG const fnsig);

/**
API, Search an event, with name
See prerequsite note with rcecb_create_name
@param[in] NULL terminated string, name of the event
@param[in] Callback function pointer
@return
RCECB_BOOL -- Boolean reflecting event previously created
*/
DLL_API_WARN_UNUSED_RESULT 
RCECB_BOOL rcecb_search_name(RCECB_NAME const name, RCECB_CALLBACK_FNSIG const fnsig);

/**
API, Count of registrants for event, with name
See prerequsite note with rcecb_create_name
@param[in] NULL terminated string, name of the event
@return
int -- Count of registrants for specific event
*/
DLL_API_WARN_UNUSED_RESULT 
int rcecb_getregistrants_name(RCECB_NAME const name);

/**
API, Count of registrants for event, with handle
See prerequsite note with rcecb_create_name
@param[in] Opaque handle of the event
@return
int -- Count of registrants for specific event
*/
DLL_API_WARN_UNUSED_RESULT 
int rcecb_getregistrants_handle(RCECB_HANDLE const handle);

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
RCECB_HANDLE rcecb_register_parm0_handle(RCECB_HANDLE const handle, RCECB_CALLBACK_FNSIG_P0 const fnsig);

/**
API, Register callback to event, with handle
See prerequsite note with rcecb_create_name
@param[in] Opaque handle of the event
@param[in] Callback function pointer
@param[in] Data Parameter
@return
RCECB_NULL -- Error
RCECB_HANDLE -- Opaque handle to existing event
*/
DLL_API_WARN_UNUSED_RESULT 
RCECB_HANDLE rcecb_register_parm1_handle(RCECB_HANDLE const handle, RCECB_CALLBACK_FNSIG_P1 const fnsig, RCECB_PARM const p1);

/**
API, Register callback to event, with handle
See prerequsite note with rcecb_create_name
@param[in] Opaque handle of the event
@param[in] Callback function pointer
@param[in] Data Parameter
@param[in] Data Parameter
@return
RCECB_NULL -- Error
RCECB_HANDLE -- Opaque handle to existing event
*/
DLL_API_WARN_UNUSED_RESULT 
RCECB_HANDLE rcecb_register_parm2_handle(RCECB_HANDLE const handle, RCECB_CALLBACK_FNSIG_P2 const fnsig, RCECB_PARM const p1, RCECB_PARM const p2);

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
RCECB_HANDLE rcecb_register_parm0_name(RCECB_NAME const name, RCECB_CALLBACK_FNSIG_P0 const fnsig);

/**
API, Register callback to event, with name
See prerequsite note with rcecb_create_name
@param[in] NULL terminated string, name of the event
@param[in] Callback function pointer
@param[in] Data Parameter
@return
RCECB_NULL -- Error
RCECB_HANDLE -- Opaque handle to existing event
*/
DLL_API_WARN_UNUSED_RESULT 
RCECB_HANDLE rcecb_register_parm1_name(RCECB_NAME const name, RCECB_CALLBACK_FNSIG_P1 const fnsig, RCECB_PARM const p1);

/**
API, Register callback to event, with name
See prerequsite note with rcecb_create_name
@param[in] NULL terminated string, name of the event
@param[in] Callback function pointer
@param[in] Data Parameter
@param[in] Data Parameter
@return
RCECB_NULL -- Error
RCECB_HANDLE -- Opaque handle to existing event
*/
DLL_API_WARN_UNUSED_RESULT 
RCECB_HANDLE rcecb_register_parm2_name(RCECB_NAME const name, RCECB_CALLBACK_FNSIG_P2 const fnsig, RCECB_PARM const p1, RCECB_PARM const p2);

/**
API, Unegister callback to event, with handle
@param[in] Opaque handle of the event
@param[in] Callback function pointer
@return
RCECB_NULL -- Error
RCECB_HANDLE -- Opaque handle to existing event
*/
DLL_API_WARN_UNUSED_RESULT 
RCECB_HANDLE rcecb_unregister_parm0_handle(RCECB_HANDLE const handle, RCECB_CALLBACK_FNSIG_P0 const fnsig);

/**
API, Unegister callback to event, with handle
@param[in] Opaque handle of the event
@param[in] Callback function pointer
@param[in] Data Parameter
@return
RCECB_NULL -- Error
RCECB_HANDLE -- Opaque handle to existing event
*/
DLL_API_WARN_UNUSED_RESULT 
RCECB_HANDLE rcecb_unregister_parm1_handle(RCECB_HANDLE const handle, RCECB_CALLBACK_FNSIG_P1 const fnsig, RCECB_PARM const p1);

/**
API, Unegister callback to event, with handle
@param[in] Opaque handle of the event
@param[in] Callback function pointer
@param[in] Data Parameter
@param[in] Data Parameter
@return
RCECB_NULL -- Error
RCECB_HANDLE -- Opaque handle to existing event
*/
DLL_API_WARN_UNUSED_RESULT 
RCECB_HANDLE rcecb_unregister_parm2_handle(RCECB_HANDLE const handle, RCECB_CALLBACK_FNSIG_P2 const fnsig, RCECB_PARM const p1, RCECB_PARM const p2);

/**
API, Unegister callback to event, with name
@param[in] NULL terminated string, name of the event
@param[in] Callback function pointer
@return
RCECB_NULL -- Error
RCECB_HANDLE -- Opaque handle to existing event
*/
DLL_API_WARN_UNUSED_RESULT 
RCECB_HANDLE rcecb_unregister_parm0_name(RCECB_NAME const name, RCECB_CALLBACK_FNSIG_P0 const fnsig);

/**
API, Unegister callback to event, with name
@param[in] NULL terminated string, name of the event
@param[in] Callback function pointer
@param[in] Data Parameter
@return
RCECB_NULL -- Error
RCECB_HANDLE -- Opaque handle to existing event
*/
DLL_API_WARN_UNUSED_RESULT 
RCECB_HANDLE rcecb_unregister_parm1_name(RCECB_NAME const name, RCECB_CALLBACK_FNSIG_P1 const fnsig, RCECB_PARM const p1);

/**
API, Unegister callback to event, with name
@param[in] NULL terminated string, name of the event
@param[in] Callback function pointer
@param[in] Data Parameter
@param[in] Data Parameter
@return
RCECB_NULL -- Error
RCECB_HANDLE -- Opaque handle to existing event
*/
DLL_API_WARN_UNUSED_RESULT 
RCECB_HANDLE rcecb_unregister_parm2_name(RCECB_NAME const name, RCECB_CALLBACK_FNSIG_P2 const fnsig, RCECB_PARM const p1, RCECB_PARM const p2);

/**
API, Signal event, with handle
See prerequsite note with rcecb_create_name
@param[in] Opaque handle of the event
@return
RCECB_NULL -- Error
RCECB_HANDLE -- Opaque handle to existing event
*/
DLL_API_WARN_UNUSED_RESULT 
RCECB_HANDLE rcecb_signal_handle(RCECB_HANDLE const handle);

/**
API, Signal event, with handle and timetick
See prerequsite note with rcecb_create_name
@param[in] Opaque handle of the event
@param[in] Maximum timer value associated to event
@return
RCECB_NULL -- Error
RCECB_HANDLE -- Opaque handle to existing event
*/
DLL_API_WARN_UNUSED_RESULT 
RCECB_HANDLE rcecb_signal_handle_timetick(RCECB_HANDLE const handle, unsigned long* const timetick_p);

/**
API, Signal event, with name
See prerequsite note with rcecb_create_name
@param[in] NULL terminated string, name of the event
@return
RCECB_NULL -- Error
RCECB_HANDLE -- Opaque handle to existing event
*/
DLL_API_WARN_UNUSED_RESULT 
RCECB_HANDLE rcecb_signal_name(RCECB_NAME const name);

/**
API, Signal event, with name and timetick
See prerequsite note with rcecb_create_name
@param[in] NULL terminated string, name of the event
@param[in] Maximum timer value associated to event
@return
RCECB_NULL -- Error
RCECB_HANDLE -- Opaque handle to existing event
*/
DLL_API_WARN_UNUSED_RESULT 
RCECB_HANDLE rcecb_signal_name_timetick(RCECB_NAME const name, unsigned long* const timetick_p);

/////////////////////////////////////////////////////////////////////
// RCESN
/////////////////////////////////////////////////////////////////////

/**
API, Create an event, with name

Prerequsite call before posting event -- create the event
before use to avoid a potential "race condition" between
contexts that will post the event and concurrently wait on
the event being posted.

For performance, cache the resulting handle for use within
the API. Name based use cases will take a penalty when doing
a dictionary lookup on every call.

@param[in] NULL terminated string, name of the event
@return
RCESN_NULL -- Error
RCESN_HANDLE -- Opaque handle to existing event
*/
DLL_API_WARN_UNUSED_RESULT 
RCESN_HANDLE rcesn_create_name(RCESN_NAME const name);

/**
API, Search an event, with name
See prerequsite note with rcesn_create_name
@param[in] NULL terminated string, name of the event
@return
RCESN_BOOL -- Boolean reflecting event previously created
*/
DLL_API_WARN_UNUSED_RESULT 
RCESN_BOOL rcesn_search_name(RCESN_NAME const name);

/**
API, Count of registrants for event, with handle
See prerequsite note with rcesn_create_name
@param[in] Opaque handle of the event
@return
int -- Count of registrants for specific event
*/
DLL_API_WARN_UNUSED_RESULT 
int rcesn_getregistrants_handle(RCESN_HANDLE const handle);

/**
API, Count of registrants for event, with name
See prerequsite note with rcesn_create_name
@param[in] NULL terminated string, name of the event
@return
int -- Count of registrants for specific event
*/
DLL_API_WARN_UNUSED_RESULT 
int rcesn_getregistrants_name(RCESN_NAME const name);

/**
API, Current State of event, with handle
See prerequsite note with rcesn_create_name
@param[in] Opaque handle of the event
@return
RCESN_STATE -- Count of registrants for specific event
*/
DLL_API_WARN_UNUSED_RESULT 
RCESN_STATE rcesn_getstatecurr_handle(RCESN_HANDLE const handle);

/**
API, Current State of event, with handle
See prerequsite note with rcesn_create_name
@param[in] Opaque handle of the event
@param[in] State to apply
@return
RCESN_STATE -- State of event
*/
DLL_API_WARN_UNUSED_RESULT 
RCESN_STATE rcesn_setstatecurr_handle(RCESN_HANDLE const handle, RCESN_STATE const state);

/**
API, Previous State of event, with handle
See prerequsite note with rcesn_create_name
@param[in] Opaque handle of the event
@return
RCESN_STATE -- State of event
*/
DLL_API_WARN_UNUSED_RESULT 
RCESN_STATE rcesn_getstateprev_handle(RCESN_HANDLE const handle);

/**
API, Previous State of event, with handle
See prerequsite note with rcesn_create_name
@param[in] Opaque handle of the event
@param[in] State to apply
@return
RCESN_STATE -- State of event
*/
DLL_API_WARN_UNUSED_RESULT 
RCESN_STATE rcesn_setstateprev_handle(RCESN_HANDLE const handle, RCESN_STATE const state);

/**
API, Current State of event, with name
See prerequsite note with rcesn_create_name
@param[in] Opaque handle of the event
@return
RCESN_STATE -- State of event
*/
DLL_API_WARN_UNUSED_RESULT 
RCESN_STATE rcesn_getstatecurr_name(RCESN_NAME const name);

/**
API, Previous State of event, with name
See prerequsite note with rcesn_create_name
@param[in] Opaque handle of the event
@return
RCESN_STATE -- State of event
*/
DLL_API_WARN_UNUSED_RESULT 
RCESN_STATE rcesn_getstateprev_name(RCESN_NAME const name);

/**
API, Register notification to event, with handle
See prerequsite note with rcesn_create_name
@param[in] Opaque handle of the event
@param[in] Notification type (DAL, POSIX, QURT, REX)
@param[in] Notification sigex structure pointer, type specific values
@return
RCESN_NULL -- Error
RCESN_HANDLE -- Opaque handle to existing event
*/
DLL_API_WARN_UNUSED_RESULT 
RCESN_HANDLE rcesn_register_sigex_handle(RCESN_HANDLE const handle, RCESN_SIGEX_TYPE const type, RCESN_SIGEX const sigex);

/**
API, Register notification to event, with name
See prerequsite note with rcesn_create_name
@param[in] NULL terminated string, name of the event
@param[in] Notification type (DAL, POSIX, QURT, REX)
@param[in] Notification sigex structure pointer, type specific values
@return
RCESN_NULL -- Error
RCESN_HANDLE -- Opaque handle to existing event
*/
DLL_API_WARN_UNUSED_RESULT 
RCESN_HANDLE rcesn_register_sigex_name(RCESN_NAME const name, RCESN_SIGEX_TYPE const type, RCESN_SIGEX const sigex);

/**
API, Unegister notification to event, with handle
See prerequsite note with rcesn_create_name
@param[in] Opaque handle of the event
@param[in] Notification type (DAL, POSIX, QURT, REX)
@param[in] Notification sigex structure pointer, type specific values
@return
RCESN_NULL -- Error
RCESN_HANDLE -- Opaque handle to existing event
*/
DLL_API_WARN_UNUSED_RESULT 
RCESN_HANDLE rcesn_unregister_sigex_handle(RCESN_HANDLE const handle, RCESN_SIGEX_TYPE const type, RCESN_SIGEX const sigex);

/**
API, Unegister notification to event, with name
See prerequsite note with rcesn_create_name
@param[in] NULL terminated string, name of the event
@param[in] Notification type (DAL, POSIX, QURT, REX)
@param[in] Notification sigex structure pointer, type specific values
@return
RCESN_NULL -- Error
RCESN_HANDLE -- Opaque handle to existing event
*/
DLL_API_WARN_UNUSED_RESULT 
RCESN_HANDLE rcesn_unregister_sigex_name(RCESN_NAME const name, RCESN_SIGEX_TYPE const type, RCESN_SIGEX const sigex);

/**
API, Signal event, with handle
See prerequsite note with rcesn_create_name
@param[in] Opaque handle of the event
@param[in] Current state to apply before notifications
@return
RCESN_NULL -- Error
RCESN_HANDLE -- Opaque handle to existing event
*/
DLL_API_WARN_UNUSED_RESULT 
RCESN_HANDLE rcesn_signal_handle(RCESN_HANDLE const handle, RCESN_STATE const state);

/**
API, Signal event, with handle and timetick
See prerequsite note with rcesn_create_name
@param[in] Opaque handle of the event
@param[in] Current state to apply before notifications
@param[in] Maximum timer value associated to event
@return
RCESN_NULL -- Error
RCESN_HANDLE -- Opaque handle to existing event
*/
DLL_API_WARN_UNUSED_RESULT 
RCESN_HANDLE rcesn_signal_handle_timetick(RCESN_HANDLE const handle, RCESN_STATE const state, unsigned long* const timetick_p);

/**
API, Signal event, with name
See prerequsite note with rcesn_create_name
@param[in] Opaque handle of the event
@param[in] Current state to apply before notifications
@return
RCESN_NULL -- Error
RCESN_HANDLE -- Opaque handle to existing event
*/
DLL_API_WARN_UNUSED_RESULT 
RCESN_HANDLE rcesn_signal_name(RCESN_NAME const name, RCESN_STATE const state);

/**
API, Signal event, with name and timetick
See prerequsite note with rcesn_create_name
@param[in] Opaque handle of the event
@param[in] Current state to apply before notifications
@param[in] Maximum timer value associated to event
@return
RCESN_NULL -- Error
RCESN_HANDLE -- Opaque handle to existing event
*/
DLL_API_WARN_UNUSED_RESULT 
RCESN_HANDLE rcesn_signal_name_timetick(RCESN_NAME const name, RCESN_STATE const state, unsigned long* const timetick_p);

/**
API, Wait for event at specific threshold, with handle
See prerequsite note with rcesn_create_name
@param[in] Opaque handle of the event
@param[in] Notification type (DAL, POSIX, QURT, REX)
@param[in] Notification sigex structure pointer, type specific values
@return
RCESN_NULL -- Error
RCESN_HANDLE -- Opaque handle to existing event
*/
DLL_API_WARN_UNUSED_RESULT 
RCESN_HANDLE rcesn_wait_sigex_handle(RCESN_HANDLE const handle, RCESN_SIGEX_TYPE const type, RCESN_SIGEX const sigex);

/**
API, Wait for event at specific state, with name
See prerequsite note with rcesn_create_name
@param[in] NULL terminated string, name of the event
@param[in] Notification type (DAL, POSIX, QURT, REX)
@param[in] Notification sigex structure pointer, type specific values
@return
RCESN_NULL -- Error
RCESN_HANDLE -- Opaque handle to existing event
*/
DLL_API_WARN_UNUSED_RESULT 
RCESN_HANDLE rcesn_wait_sigex_name(RCESN_NAME const name, RCESN_SIGEX_TYPE const type, RCESN_SIGEX const sigex);

/**
API, Wait for event, one time per call, until compare met, with handle
See prerequsite note with rcesn_create_name
@param[in] Opaque handle of the event
@param[in] Comparator function pointer
@return
RCESN_NULL -- Error
RCESN_HANDLE -- Opaque handle to existing event
*/
DLL_API_WARN_UNUSED_RESULT 
RCESN_HANDLE rcesn_wait_handle(RCESN_HANDLE const handle, RCESN_FNSIG_COMPARE const compare);

/**
API, Wait for event, one time per call, until compare met, with name
See prerequsite note with rcesn_create_name
@param[in] NULL terminated string, name of the event
@param[in] Comparator function pointer
@return
RCESN_NULL -- Error
RCESN_HANDLE -- Opaque handle to existing event
*/
DLL_API_WARN_UNUSED_RESULT 
RCESN_HANDLE rcesn_wait_name(RCESN_NAME const name, RCESN_FNSIG_COMPARE const compare);

/**
API, Common Comparision Functions
See prerequsite note with rcesn_create_name
Clients have the ability to supply comparator callback functions for use. Primarily
these functions will capture the policy logic and determine if notification should
occur with a TRUE/FALSE return. NOTE: these callback functions are never allowed
to block by policy; to do so is considered a bug. Callbacks are exeucted and if
they do not complete within an gracious time period (~20ms), the system is halted.
The timer is not deferred, and will preculde DVCS sleep and power collapse.
@param[in] Opaque handle of the event
@return
RCESN_BOOL -- Boolean reflecting comparision status
*/
DLL_API_WARN_UNUSED_RESULT 
RCESN_BOOL rcesn_compare_lt_0(RCESN_HANDLE const handle);

DLL_API_WARN_UNUSED_RESULT 
RCESN_BOOL rcesn_compare_eq_0(RCESN_HANDLE const handle);

DLL_API_WARN_UNUSED_RESULT 
RCESN_BOOL rcesn_compare_ne_0(RCESN_HANDLE const handle);

DLL_API_WARN_UNUSED_RESULT 
RCESN_BOOL rcesn_compare_gt_0(RCESN_HANDLE const handle);

DLL_API_WARN_UNUSED_RESULT 
RCESN_BOOL rcesn_compare_lt_prev(RCESN_HANDLE const handle);

DLL_API_WARN_UNUSED_RESULT 
RCESN_BOOL rcesn_compare_eq_prev(RCESN_HANDLE const handle);

DLL_API_WARN_UNUSED_RESULT 
RCESN_BOOL rcesn_compare_ne_prev(RCESN_HANDLE const handle);

DLL_API_WARN_UNUSED_RESULT 
RCESN_BOOL rcesn_compare_gt_prev(RCESN_HANDLE const handle);

DLL_API_WARN_UNUSED_RESULT 
RCESN_BOOL rcesn_compare_mask_t_0001(RCESN_HANDLE const handle);

DLL_API_WARN_UNUSED_RESULT 
RCESN_BOOL rcesn_compare_mask_f_0001(RCESN_HANDLE const handle);

/////////////////////////////////////////////////////////////////////
// RCEVT
/////////////////////////////////////////////////////////////////////

/**
API, Count of registrants for event, with handle
See prerequsite note with rcevt_create_name
@param[in] Opaque handle of the event
@return
int -- Count of registrants for specific event
*/
DLL_API_WARN_UNUSED_RESULT
int rcevt_getregistrants_handle(RCEVT_HANDLE const handle);

/**
API, Count of registrants for event, with name
See prerequsite note with rcevt_create_name
@param[in] NULL terminated string, name of the event
@return
int -- Count of registrants for specific event
*/
DLL_API_WARN_UNUSED_RESULT
int rcevt_getregistrants_name(RCEVT_NAME const name);

/////////////////////////////////////////////////////////////////////
// Internal Forward Declarations : Specialized, NOT PUBLIC BY DESIGN
/////////////////////////////////////////////////////////////////////

RCECB_HANDLE rcecb_signal_handle_nolocks(RCECB_HANDLE const handle);

RCECB_HANDLE rcecb_signal_name_nolocks(RCECB_NAME const name);

#if defined(__cplusplus)
}
#endif

#endif