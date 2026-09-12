/** vi: tw=128 ts=3 sw=3 et :
@file rcinit_init.c
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
Copyright (c) 2015-2018,2020 Qualcomm Technologies, Inc.
All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.
=============================================================================*/
/*=============================================================================
Edit History
$Header: //components/rel/core.mpss/10.0/debugtools/rcinit/src/rcinit_workloop.c#3 $
$DateTime: 2020/04/28 05:22:08 $
$Change: 23608255 $
$Author: pwbldsvc $
===============================================================================*/

#include "err.h"
#include "stringl/stringl.h"

#include "tms_dll_api.h"
#include "tms_utils.h"
#include "tms_utils_msg.h"
#include "tms_rcinit.h"
#include "qurt.h"
#include "queue.h"
#include "rcinit_workloop.h"

#include "diag.h"
#include "rcevt_qurt.h"
#include "tms_rcevt.h"

#if !defined(RCINIT_EXCLUDE_KERNEL_REX)
#include "rcinit_dal.h"
#include "rcinit_rex.h"
#elif !defined(RCINIT_EXCLUDE_KERNEL_DAL)
#include "rcinit_dal.h"
#else
#E_FAIL NHLOS INTERFACE NOT DEFINED PROPERLY
#endif

#define RCINIT_WORKLOOP_CTXT_PROCESS 0x1
#define RCINIT_DIAG_MSG_MASK_CHANGE_NOTIFY    0x2  


typedef struct
{
  qurt_anysignal_t sig;
  q_type           queue;
  boolean          init;
} rcinit_workloop_internal_s;

typedef struct
{
  /* q_link_type - Must be the first element */
  q_link_type                node;
  rcinit_workloop_handle_t   *handle;
}rcinit_workloop_data_s;

static rcinit_workloop_internal_s rcinit_workloop_internal;

extern void rcinit_workloop_process_data( rcinit_workloop_handle_t * handle );

/**
API, Service Initialization
@param
None
@return
None
*/
DLL_API_GLOBAL rcinit_err_t rcinit_handle_init(rcinit_workloop_handle_t * handle, char * rcecb_str, char * rcesn_str, char * rcevt_str, rcinit_workloop_method_t method, rcinit_autogen_internal_t * autogen_internal)
{
   handle->client_internal = malloc(sizeof(struct rcinit_internal_s));

   if ( handle->client_internal == NULL )
   {
      rcinit_handle_deinit(handle);
      return E_FAIL;
   }

   handle->client_internal->autogen_internal = autogen_internal;

   handle->method = method;

   if (rcecb_str)
   {
      if ( NULL == (handle->rcecb_handle=rcecb_create_name(rcecb_str)))
      {
         rcinit_handle_deinit(handle);
         return E_FAIL;
      }
   }

   if (rcesn_str)
   {
      if ( NULL == (handle->rcesn_handle=rcesn_create_name(rcesn_str)))
      {
         rcinit_handle_deinit(handle);
         return E_FAIL;
      }
   }

   if (rcevt_str)
   {
      if ( NULL == (handle->rcevt_handle=rcevt_create_name(rcevt_str)))
      {
         rcinit_handle_deinit(handle);
         return E_FAIL;
      }
   }

   return E_SUCCESS;
}

DLL_API_GLOBAL rcinit_err_t rcinit_handle_deinit(rcinit_workloop_handle_t * handle )
{
   if ( handle->client_internal != NULL )
   {
      //Store this info in rcinit_internal_static.q_of_rcinit_client_internal_q_data_p to facilitate debugging
      //free(handle->client_internal);
      handle->client_internal = NULL;
   }

   if (handle->rcecb_handle)
   {
     // TODO: Check if there is a way to remove rce entry
     handle->rcecb_handle = NULL;
   }

   if (handle->rcesn_handle)
   {
      // TODO: Check if there is a way to remove rce entry
     handle->rcesn_handle = NULL;
   }

   if (handle->rcevt_handle)
   {
      // TODO: Check if there is a way to remove rce entry
     handle->rcevt_handle = NULL;
   }

   return E_SUCCESS;
}

void rcinit_onetime_init(void)
{
   if ( rcinit_workloop_internal.init != TRUE )
   {
     rcinit_workloop_internal.init = TRUE;
     q_init(&rcinit_workloop_internal.queue);
     qurt_anysignal_init(&rcinit_workloop_internal.sig);
   }

   #if !defined(RCINIT_EXCLUDE_DALCFG)
      rcinit_internal_devcfg_check_load();                                       // load devcfg configs first, potentially allows a runtime override later
   #endif

   if (rcecb_init()) {;}

   if (rcesn_init()) {;}

   if (rcevt_init()) {;}

   if (rcxh_init()) {;}

   rcinit_internal_static.define = (rce_nde_p)rcevt_create_name(RCINIT_RCEVT_DEFINE); // handle to define event, internal, ok to observe
   rcinit_internal_static.defineack = (rce_nde_p)rcevt_create_name(RCINIT_RCEVT_DEFINEACK); // handle to defineack event, internal, ok to observe

   if (RCINIT_NULL == rcinit_internal_static.define ||                               // must have rcevt allocated
       RCINIT_NULL == rcinit_internal_static.defineack)                              // must have rcevt allocated
   {
      ERR_FATAL("initialization", 0, 0, 0);
   }

   q_init(&rcinit_internal_static.q_of_rcinit_client_internal_q_data_p);

}

DLL_API_GLOBAL rcinit_err_t rcinit_process_autogen_table( rcinit_workloop_handle_t * handle )
{
   rcinit_workloop_data_s * data;
   rcinit_client_internal_q_data_s *qdata;

   data = malloc(sizeof(rcinit_workloop_data_s));

   if ( data == NULL )
   {
      return E_FAIL;
   }

   qdata = malloc(sizeof(rcinit_client_internal_q_data_s));

   if ( qdata == NULL )
   {
     free(data);
     return E_FAIL;
   }

   data->handle = handle;
   qdata->client_internal = handle->client_internal;

   (void) q_link( qdata , &(qdata->node) );
   q_put(&(rcinit_internal_static.q_of_rcinit_client_internal_q_data_p), 
         &(qdata->node) );
  
   /* Queue the data and signal rcinit workloop to take it up for further processing */
   /* Insert in the queue */
   (void) q_link( data , &(data->node) );
   q_put(&(rcinit_workloop_internal.queue), 
         &(data->node) );

   (void)qurt_anysignal_set(&rcinit_workloop_internal.sig, RCINIT_WORKLOOP_CTXT_PROCESS);

   return E_SUCCESS;
}

DLL_API_GLOBAL rcinit_err_t rcinit_workloop_update_rcinit_internal( rcinit_workloop_handle_t * handle )
{
   /* Cache client_internal in rcinit_internal */
   rcinit_internal = handle->client_internal;

   return E_SUCCESS;
}

#if !defined(RCINIT_EXCLUDE_KERNEL_REX)
void rcinit_workloop_ctxt(unsigned long arg_p)
#elif !defined(RCINIT_EXCLUDE_KERNEL_DAL)
void rcinit_workloop_ctxt(void* arg_p)
#endif 
{
  unsigned int    sigs;
  rcinit_workloop_data_s * data;
  
  RCEVT_HANDLE rcinit_diag_rc = RCEVT_NULL;
  RCEVT_SIGEX_SIGQURT sigex;
  uint64 rcinit_table_cnt = 0;
  
  /* Handshake with rcinit */
  rcinit_handshake_startup();
  
  /* Register for Diag F3 availability */
  sigex.signal = &rcinit_workloop_internal.sig;
  sigex.mask = RCINIT_DIAG_MSG_MASK_CHANGE_NOTIFY;
  rcinit_diag_rc = rcevt_register_sigex_name(DIAG_MSG_MASK_CHANGE_NOTIFY, RCEVT_SIGEX_TYPE_SIGQURT, &sigex);
  if (RCEVT_NULL == rcinit_diag_rc)
  {
     TMS_STARTUP_MSG_HIGH("Diag RC EVENT register failure");
  }

  for(;;)
  {
    /* Wait for some task signal */
    sigs = qurt_anysignal_wait( &rcinit_workloop_internal.sig,
		                          RCINIT_WORKLOOP_CTXT_PROCESS | RCINIT_DIAG_MSG_MASK_CHANGE_NOTIFY);
    
    if ( sigs & RCINIT_WORKLOOP_CTXT_PROCESS )
    {
      qurt_anysignal_clear( &rcinit_workloop_internal.sig, RCINIT_WORKLOOP_CTXT_PROCESS );

      /* Get from the queue */
      data = q_get(&(rcinit_workloop_internal.queue) );

      while ( data != NULL )
      {
        // Process the data
        rcinit_workloop_process_data(data->handle);

        free(data);
        data = q_get(&(rcinit_workloop_internal.queue) );
      }
    }
    
    if ( sigs & RCINIT_DIAG_MSG_MASK_CHANGE_NOTIFY )
    {
      rcinit_client_internal_q_data_s *qdata;
      struct rcinit_internal_s * rcinit_internal_p = NULL;

      qurt_anysignal_clear( &rcinit_workloop_internal.sig, RCINIT_DIAG_MSG_MASK_CHANGE_NOTIFY );
      
      if (FALSE == msg_status(MSG_SSID_TMS, MSG_BUILD_MASK_MSG_SSID_TMS))
      {
        continue;
      }
      
      /* If RCINIT completed and F3s were already printed */
      /* Get from the queue */
      qdata = q_check(&(rcinit_internal_static.q_of_rcinit_client_internal_q_data_p));

      while( qdata != NULL )
      {
         rcinit_internal_p = qdata->client_internal;

         /* If all the groups completed */
         if (rcinit_internal_p != NULL && rcinit_internal_p->group_start_time[RCINIT_GROUP_MAX] != 0)
         {
           /* RCINIT was completed for this table */
           uint64 i = RCINIT_GROUP_0;
           for (i = RCINIT_GROUP_0; i < RCINIT_GROUP_MAX; i++)
           {
              TMS_STARTUP_MSG_HIGH_3_64("RCINIT Group %llu took %llu (%llu ms)", i, TMS_TIME_DIFF(rcinit_internal_p->group_start_time[i+1], rcinit_internal_p->group_start_time[i]), TMS_TIME_DIFF_IN_MSEC(rcinit_internal_p->group_start_time[i+1], rcinit_internal_p->group_start_time[i]));
           }
           TMS_STARTUP_MSG_HIGH_3_64("RCINIT Table %llu Init completion took %llu (%llu ms)", rcinit_table_cnt, TMS_TIME_DIFF(rcinit_internal_p->group_start_time[RCINIT_GROUP_MAX], rcinit_internal_p->group_start_time[RCINIT_GROUP_0]), TMS_TIME_DIFF_IN_MSEC(rcinit_internal_p->group_start_time[RCINIT_GROUP_MAX], rcinit_internal_p->group_start_time[RCINIT_GROUP_0]));
           
           rcinit_table_cnt++;
         }
         qdata = q_next(&(rcinit_internal_static.q_of_rcinit_client_internal_q_data_p), &(qdata->node));
      }
      
      if (rcinit_table_cnt != 0)
      {
        rcinit_diag_rc = rcevt_unregister_sigex_name(DIAG_MSG_MASK_CHANGE_NOTIFY, RCEVT_SIGEX_TYPE_SIGQURT, &sigex);
        if (RCEVT_NULL == rcinit_diag_rc)
        {
          TMS_STARTUP_MSG_HIGH("Diag RC EVENT unregister failure");
        }
      }
    }
  }
}
