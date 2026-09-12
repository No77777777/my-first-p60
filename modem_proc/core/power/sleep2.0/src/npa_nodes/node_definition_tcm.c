/*============================================================================
  FILE:         node_definition_tcm.c

  OVERVIEW:     This file provides the NPA node definition for the
                /core/cpu/tcm node. This node is used to control if
                target will do power collapse with tcm retention or
                no retention.

  DEPENDENCIES: None

                Copyright (c) 2016-2017 Qualcomm Technologies, Inc. (QTI).
                All Rights Reserved.
                Qualcomm Confidential and Proprietary.
                Export of this technology or software is regulated by the U.S. Government.
                Diversion contrary to U.S. law prohibited.
==============================================================================
$Header: //components/rel/core.mpss/10.0/power/sleep2.0/src/npa_nodes/node_definition_tcm.c#1 $
$DateTime: 2019/04/24 00:03:26 $
============================================================================*/
#include "comdef.h"
#include "npa.h"
#include "npa_resource.h"
#include "sleep_lpr.h"
#include "CoreVerify.h"
#include "sleep_npa.h"
#include "sleep_npai.h"

/*==============================================================================
                       INTERNAL FUNCTION DECLARATIONS
 =============================================================================*/
/**
 * tcmCacheNode_driver
 *
 * @brief Driver function for the tcm node. This function gets called
 *        every time a client request is made that changes the state of the node.
 *
 * @param resource: The NPA resource being requested.
 * @param client:   Pointer to the client making the request.
 * @param state:    New state of the resource.
 *
 * @return New state of the resource.
 */
static npa_resource_state tcmCacheNode_driver(npa_resource        *resource,
                                              npa_client          *client,
                                              npa_resource_state  state)
{
  /* Get the TCM resources's client handle to the sleep LPR node. */
  npa_client_handle lprClient = NPA_DEPENDENCY(resource, 0);

  /* At init time, register the LPR with the sleep LPR node */
  if(client->type == NPA_CLIENT_INITIALIZE)
  {
    sleepLPR_define("tcm", lprClient);
  }

  /* XML mode order:
   * 
   * SLEEP_LPRM_NUM(0) -- "noret"
   * SLEEP_LPRM_NUM(1) -- "ret"
   */
  if(SLEEP_TCM_LPR_RETENTION_ONLY == state)
  {
    /* Only retention LPRM is enabled */
    npa_issue_required_request(lprClient, SLEEP_LPRM_NUM(1));
  }
  else
  {
    /* TCM Memory can be powered OFF */
    npa_issue_required_request(lprClient, SLEEP_LPRM_NUM(0));
  }

  return state;
}

/*==============================================================================
                            NODE DEFINITION
 =============================================================================*/
/* The dependency array for the tcm node.  This node depends
 * on the sleep LPR node, as it makes requests to that node when the state
 * of this node changes. */
static npa_node_dependency g_tcmCacheDependency[1] =
{
  {
    SLEEP_LPR_NODE_NAME,
    NPA_CLIENT_REQUIRED,
  },
};

/* Resource definition for the /core/cpu/tcm resource. */
static npa_resource_definition g_tcmCacheResource[] =
{
  {
    SLEEP_TCM_LPR_NODE_NAME,  /* Name */
    "on/off",                 /* Units */
    1,                        /* Max State */
    &npa_binary_plugin,       /* Plugin */
    NPA_RESOURCE_DEFAULT,     /* Attributes */
    NULL,                     /* User Data */
  }
};

/* Node definition for the /core/cpu/tcm resource. */
npa_node_definition g_tcmCacheNode =
{
  SLEEP_LPR_CLIENT_NODE(SLEEP_TCM_LPR_NODE_NAME), /* Name */
  tcmCacheNode_driver,                            /* Driver_fcn */
  NPA_NODE_DEFAULT,                               /* Attributes */
  NULL,                                           /* Data */
  NPA_ARRAY(g_tcmCacheDependency),                /* Dependencies */
  NPA_ARRAY(g_tcmCacheResource)                   /* Resources */
};

