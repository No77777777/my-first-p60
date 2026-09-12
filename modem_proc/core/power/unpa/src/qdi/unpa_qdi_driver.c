/*==============================================================================
  FILE:         unpa_qdi_driver.c
  
  OVERVIEW:     Implements the driver side of the UNPA/QDI interface in the rootPD.
                Handles requests from userPDs to create clients and query handles and issue
                requests and queries.

  
  DEPENDENCIES: None

                Copyright (c) 2016-2018 Qualcomm Technologies, Inc. (QTI).
                All Rights Reserved.
                Qualcomm Confidential and Proprietary.
                Export of this technology or software is regulated by the U.S. Government.
                Diversion contrary to U.S. law prohibited.
=================================================================================
$Header: //components/rel/core.mpss/10.0/power/unpa/src/qdi/unpa_qdi_driver.c#1 $
$DateTime: 2019/04/24 00:03:26 $
===============================================================================*/
#include <stdlib.h>
#include "comdef.h"
#include "unpa.h"
#include "unpa_resource.h"
#include "CoreVerify.h"
#include "qurt.h"
#include "unpa_qdi.h"

/*==============================================================================
                              TYPES & DEFINITIONS
 =============================================================================*/
typedef struct unpa_qdi_client_obj_s
{
  qurt_qdi_obj_t base;
  char           client_name[UNPA_MAX_NAME_LEN+1];
  unpa_client   *client;
  int32          qdih;
} unpa_qdi_client_obj;
  
typedef struct unpa_qdi_query_obj_s
{
  qurt_qdi_obj_t    base;
  unpa_query_handle qh;
} unpa_qdi_query_obj;

/*==============================================================================
                            INTERNAL FUNCTION DEFINITIONS
 =============================================================================*/
/* 
 * unpa_qdiQueryInvoke
 */
static int unpa_qdiQueryInvoke(int clientHandle, qurt_qdi_obj_t *ptr, int method,
                               qurt_qdi_arg_t a1, qurt_qdi_arg_t a2,
                               qurt_qdi_arg_t a3, qurt_qdi_arg_t a4,
                               qurt_qdi_arg_t a5, qurt_qdi_arg_t a6,
                               qurt_qdi_arg_t a7, qurt_qdi_arg_t a8,
                               qurt_qdi_arg_t a9)
{
  unpa_qdi_query_obj *obj = (unpa_qdi_query_obj *)ptr;

  CORE_VERIFY_PTR(obj);
  
  switch (method)
  {  
    case UNPA_QDI_QUERY:
    {
      int err;
      unpa_query_result result = {{0}};
      unpa_query_status ret    = unpa_query(obj->qh, (unpa_query_id)a1.num, &result);

      err = qurt_qdi_copy_to_user(clientHandle, a2.ptr, 
                                  &result, sizeof(unpa_query_result));
                                   
      return (err < 0 ? UNPA_QUERY_QDI_ERROR : ret);
    }
  
    default:
      return qurt_qdi_method_default(clientHandle, ptr, method,
                                     a1, a2, a3, a4, a5, a6, a7, a8, a9);
  }
}
 
/* 
 * unpa_qdiQueryRelease
 */ 
static void unpa_qdiQueryRelease(unpa_qdi_query_obj *obj)
{
  free(obj);
  return;
}

/* 
 * unpa_qdiClientInvoke
 */ 
static int unpa_qdiClientInvoke(int clientHandle, qurt_qdi_obj_t *ptr, int method,
                                qurt_qdi_arg_t a1, qurt_qdi_arg_t a2,
                                qurt_qdi_arg_t a3, qurt_qdi_arg_t a4,
                                qurt_qdi_arg_t a5, qurt_qdi_arg_t a6,
                                qurt_qdi_arg_t a7, qurt_qdi_arg_t a8,
                                qurt_qdi_arg_t a9)
{
  unpa_qdi_client_obj *obj = (unpa_qdi_client_obj *)ptr;

  CORE_VERIFY_PTR(obj);
  
  switch (method)
  {  
    case UNPA_QDI_ISSUE_SCALAR_REQUEST:
    { 
      /* a1: state, a2: request_attr */
      obj->client->request_attr |= (unsigned int)a2.num | UNPA_REQUEST_REMOTE_CLIENT;
       
      /* Save the client handle to return in response to unpa_get_qdi_client_handle */
      obj->qdih = clientHandle;
      
      unpa_issueRequest(obj->client, (unpa_resource_state)a1.num);
      return 0;
    }

    default:
      return qurt_qdi_method_default(clientHandle, ptr, method,
                                     a1, a2, a3, a4, a5, a6, a7, a8, a9);
  }
}

/* 
 * unpa_qdiClientRelease
 */ 
static void unpa_qdiClientRelease(unpa_qdi_client_obj *obj)
{
  unpa_destroyClient(obj->client);
  free(obj);
  return;
}

/* 
 * unpa_qdiBaseInvoke
 */
static int unpa_qdiBaseInvoke(int clientHandle, qurt_qdi_obj_t *ptr, int method,
                              qurt_qdi_arg_t a1, qurt_qdi_arg_t a2,
                              qurt_qdi_arg_t a3, qurt_qdi_arg_t a4,
                              qurt_qdi_arg_t a5, qurt_qdi_arg_t a6,
                              qurt_qdi_arg_t a7, qurt_qdi_arg_t a8,
                              qurt_qdi_arg_t a9)
{
  switch (method)
  {
    case QDI_OPEN:
    {
      return qurt_qdi_new_handle_from_obj_t(clientHandle, ptr);
    }
  
    case UNPA_QDI_CREATE_CLIENT:
    {
      int err;
      unpa_client *client;
      unpa_qdi_client_obj *obj;
      
      obj = calloc(1, sizeof(unpa_qdi_client_obj));
      CORE_VERIFY_PTR(obj);
      
      /* Copy the client_name into a local buffer; a3.num is the strlen */
      err = qurt_qdi_copy_from_user(clientHandle, 
                                    obj->client_name, a2.ptr, 
                                    MIN(a3.num, UNPA_MAX_NAME_LEN));
      if (err < 0)
      {
        free(obj);
        return -1;
      }
      
      client = unpa_createClient(obj->client_name, a4.num, a1.ptr);
      if (client)
      {
        int rch;
        
        obj->base.invoke  = (qurt_qdi_pfn_invoke_t)unpa_qdiClientInvoke;
        obj->base.refcnt  = QDI_REFCNT_INIT;
        obj->base.release = (qurt_qdi_pfn_release_t)unpa_qdiClientRelease;
        obj->client       = client;
        
        /* Create a QDI handle from the object */
        rch = qurt_qdi_handle_create_from_obj_t(clientHandle, &obj->base);
        CORE_VERIFY(rch >= 0);

        /* Return handle to caller */
        err = qurt_qdi_copy_to_user(clientHandle, a7.ptr, &rch, sizeof(rch));
        if (err < 0)
        {
          unpa_destroyClient(client);
          free(obj);
          return -1;
        }
        
        return 0;
      }
      
      free(obj);
      return -1;
    }
    
    case UNPA_QDI_CREATE_QUERY_HANDLE:
    {
      int err;
      unpa_query_handle   qh;
      unpa_qdi_query_obj *obj = NULL;
      
      qh = unpa_getQueryHandle((const char *)a1.ptr);
      
      if (qh)
      {
        int rqh;
        
        obj = calloc(1, sizeof(unpa_qdi_query_obj));
        CORE_VERIFY_PTR(obj);
        
        obj->base.invoke  = (qurt_qdi_pfn_invoke_t)unpa_qdiQueryInvoke;
        obj->base.refcnt  = QDI_REFCNT_INIT;
        obj->base.release = (qurt_qdi_pfn_release_t)unpa_qdiQueryRelease;
        obj->qh           = qh;
        
        /* Create a QDI handle from the object */
        rqh = qurt_qdi_handle_create_from_obj_t(clientHandle, &obj->base);
        CORE_VERIFY(rqh >= 0);
        
        /* Return handle to caller */
        err = qurt_qdi_copy_to_user(clientHandle, a2.ptr, &rqh, sizeof(rqh));
        return (err < 0 ? -1 : 0);
      }
      
      return -1;
    }
    
    default:
      return qurt_qdi_method_default(clientHandle, ptr, method,
                                     a1, a2, a3, a4, a5, a6, a7, a8, a9);
  }
}

/*==============================================================================
                            INTERNAL FUNCTION DEFINITIONS
 =============================================================================*/
static const struct
{
  qurt_qdi_obj_t base;
} unpa_qdi =
{
  {unpa_qdiBaseInvoke, QDI_REFCNT_PERM, 0}
};

/* 
 * unpa_qdiDriverInit
 */
void unpa_qdiDriverInit(void)
{
  CORE_VERIFY(0 == qurt_qdi_register_devname("/dev/unpa", (void *)&unpa_qdi));
}
