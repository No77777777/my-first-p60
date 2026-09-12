/*
===========================================================================
*/
/**
  @file ClockTarget.c

*/
/*
  ====================================================================

  Copyright (c) 2018 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  QUALCOMM Proprietary and Confidential.

  ====================================================================
  $Header: //components/rel/core.mpss/10.0/systemdrivers/clock/image/mss/hw/nicobar/ClockTarget.c#1 $
  $DateTime: 2019/04/24 00:03:26 $
  $Author: pwbldsvc $
  ====================================================================
*/


/*=========================================================================
      Include Files
==========================================================================*/

#include "ClockImage.h"
#include "HALclkHWIO.h"
#include "HALhwio.h"
#include "uSleep_mode_trans.h"
#include "rcecb.h"
#include "rcinit.h"
#include "ClockSWEVT.h"

/*
 * External header files.
 */
#include <sleep_lpr.h>
#include <npa.h>
#include <npa_resource.h>
#include <npa_remote.h>
#include <npa_remote_resource.h>

#include "DALDeviceId.h"

/*
 * KVP & RPM header files
 */
#include "kvp.h"
#include "rpm.h"
#include "rpmclient.h"

/*=========================================================================
      Macros
==========================================================================*/


/*
 * NPA Resource handles
 */
#define NPA_RESOURCE_XO                                                \
  Clock_XO.resource.handle

#define CLOCK_NPA_NODE_NAME_CXO                   "/xo/cxo"
#define CLOCK_NPA_NODE_NAME_BIMC                  "/clk/bimc"

/*=========================================================================
      Type Definitions
==========================================================================*/

/*
 * Clock_XOType
 *
 * Structure containing the XO NPA node and resource data.
 *
 *  resource  - CXO resource data - /node/xo/cxo
 *  node      - CXO node data     - /xo/cxo
 *  lpr       - CXO lpr           - handle to sleep LPR.
 */
typedef struct
{
  npa_resource_definition resource;
  npa_node_definition     node;
  npa_client_handle       lpr;
} ClockXOType;

static npa_client_handle  hClientBIMC;

/*=========================================================================
      Prototypes
==========================================================================*/

static boolean HAL_clk_GPLLStubEnableVote (HAL_clk_SourceDescType *pSource);
static void  Clock_UImageCallback (uSleep_state_notification state);
static void Clock_TargetPostInit (void);
static DALResult Clock_InitXO(ClockDrvCtxt *pCtxt);
static npa_resource_state Clock_NPAXODriverFunc(npa_resource *pResource, npa_client *pClient, npa_resource_state nState);
static void Clock_NPABIMCNodeAvailableCallback(void *pContext, unsigned int nEventType, void *pNodeName, unsigned int nNodeNameSize);
static void Clock_DefineRemoteResourceNodes(ClockDrvCtxt *pCtxt);


/*=========================================================================
      Data
==========================================================================*/

/*
 * Stubbed PLL control for GPLLs.
 */
HAL_clk_SourceControlType HAL_clk_GPLLStubPLLControl =
{
  .EnableVote = HAL_clk_GPLLStubEnableVote,
};

/*
 * ClockXO
 *
 * Low power resource data.
 */
static ClockXOType Clock_XO =
{
  /*
   * resource
   */
  {
    CLOCK_NPA_NODE_NAME_CXO,
    "on/off",
    CLOCK_SOURCE_REQUIRED,  /* max state */
    &npa_max_plugin, 
    NPA_RESOURCE_DEFAULT,
  },

  /*
   * node
   */
  { 
    "/node" CLOCK_NPA_NODE_NAME_CXO,     /* name */
    Clock_NPAXODriverFunc,               /* driver_fcn */
    NPA_NODE_DEFAULT,                    /* attributes */
    NULL,                                /* data */
    NPA_EMPTY_ARRAY,
    1, &Clock_XO.resource
  }
};

/*=========================================================================
      Data
==========================================================================*/

/*
 * BIMC NPA remote resource used
 */
static npa_remote_resource_definition Clock_aBIMCNPARemoteResource =
{
  CLOCK_NPA_NODE_NAME_BIMC,
  "clk2\x00\x00\x00\x00",
  "/protocol/rpm/rpm",
  &npa_max_plugin,
  npa_remote_resource_local_aggregation_driver_fcn,
  "KHz",
  NPA_MAX_STATE
};


/*
 * kvp for holding the �keep CXO resource en/dis abled' in the Sleep Set 
 */
static kvp_t* pCXODisableKVP; 
static kvp_t* pCXOEnableKVP; 

/*
 * kvp values 0 and 1 
 */
static const uint32 nCXODisableReq = 0; /* Represents the 0 value for the kvp */
static const uint32 nCXOEnableReq  = 1; /* Represents the 1 value for the kvp */

/*
 * kvp enable request macro 
 */
#define KVP_ENABLE_REQUEST 0x62616e45

/*
 * Each 'kvp' item has three fields: key, size of value and value 
 */
#define KVP_ITEM_SZ 4*3

/*=========================================================================
      Functions
==========================================================================*/

/* ===========================================================================
**  HAL_clk_EnableCXiPeakLMVote
**
** ======================================================================== */

boolean  HAL_clk_EnableCXiPeakLMVote(void)
{ 
  /* 
   * Check if this feature is enabled
   */
  if (HWIO_INF(TCSR_CXIP_LM_VOTE_FEATURE_ENABLE,CXIP_LM_VOTE_FEATURE_ENABLE))
  {
    /* 
     * Vote from CDSP Client if frequency is switching to Turbo(Set LM_VOTE_CLINET0 as 1)
     */
     HWIO_OUTF(TCSR_CXIP_LM_VOTE_CLIENT2, CXIP_LM_VOTE_CLIENT2, 1);

    /*
     * Propagation delay
     */
    HAL_clk_BusyWait(1);

    /* Poll until DANGER becomes zero */
   
    while(HWIO_IN(TCSR_CXIP_LM_DANGER)==1);
   
  }
  
  return TRUE;

}  /* END HAL_clk_EnableCXiPeakLMVote */


/* ===========================================================================
**  HAL_clk_DisableCXiPeakLMVote
**
** ======================================================================== */

boolean  HAL_clk_DisableCXiPeakLMVote(void)
{
   /*
   * CX-iPeak Un-vote
   */
  if(HWIO_INF(TCSR_CXIP_LM_VOTE_FEATURE_ENABLE,CXIP_LM_VOTE_FEATURE_ENABLE))
  {
    /*
     * Clear vote by setting LM_VOTE for CLIENT0 to 0
     */
    HWIO_OUTF(TCSR_CXIP_LM_VOTE_CLIENT2, CXIP_LM_VOTE_CLIENT2, 0);
  }
  
  return TRUE;
  
} /* END HAL_clk_DisableCXiPeakLMVote */

/* =========================================================================
**  Function : QDSP6SSClockStateChangeCallback
** =========================================================================*/

static void QDSP6SSClockStateChangeCallback
(
  void         *pContext,
  unsigned int  nEventType,
  void         *pData,
  unsigned int  nDataSize
)
{
  ClockNodeType *pClock = (ClockNodeType *)pContext;
  ClockDomainNodeType *pDomain;
  ClockMuxConfigType  *pNewConfig = NULL , *pOldConfig = NULL;
  npa_prepost_change_data *event_data; 
  uint32 post_freq,pre_freq; 
  DALResult eResult;
  boolean  bResult;
  
  if(pData == NULL || pClock == NULL)
  {
    DALSYS_LogEvent(
      0,
      DALSYS_LOGEVENT_FATAL_ERROR,
      "DALLOG Device Clock: Invalid Data passed in Q6ClockStateChangeCallback");
  }
  
  event_data = (npa_prepost_change_data *)pData; 
  post_freq = NPA_TO_HZ(event_data->to_state);
  pre_freq = NPA_TO_HZ(event_data->from_state);
  pDomain = pClock->pDomain;
  
  eResult = Clock_FindClockConfig(pDomain ,post_freq , CLOCK_FREQUENCY_HZ_AT_LEAST , &pNewConfig);
  
  if ((eResult != DAL_SUCCESS) || (pNewConfig == NULL))
  {
    DALSYS_LogEvent(
      0,
      DALSYS_LOGEVENT_FATAL_ERROR,
      "DALLOG Device Clock: New configuration required to be set for q6 clock is not supported ");
  }
  
  eResult = Clock_FindClockConfig(pDomain ,pre_freq , CLOCK_FREQUENCY_HZ_AT_LEAST , &pOldConfig);
  
  if ((eResult != DAL_SUCCESS) || (pOldConfig == NULL))
  {
    DALSYS_LogEvent(
      0,
      DALSYS_LOGEVENT_FATAL_ERROR,
      "DALLOG Device Clock: Old configuration required to be set for q6 clock is not supported ");
  }
  
  if(nEventType == NPA_TRIGGER_PRE_CHANGE_EVENT)
  {
 
    /*
     * Place CX-iPeak mitigation vote if q6 clock needs to be run on TURBO
     */
 
    if(pOldConfig->eVRegLevel != RAIL_VOLTAGE_LEVEL_TUR && pNewConfig->eVRegLevel == RAIL_VOLTAGE_LEVEL_TUR)
    {
      bResult = HAL_clk_EnableCXiPeakLMVote();
      if(!bResult)
      {
        DALSYS_LogEvent(
          DALDEVICEID_CLOCK,
          DALSYS_LOGEVENT_FATAL_ERROR,
          "Failed to vote for CX iPeak mitigation");
       }
    }

  }
  else
  {
 
     /*
      * Remove CX-iPeak mitigation vote if q6 clock is not running on TURBO
      */

     if(pOldConfig->eVRegLevel == RAIL_VOLTAGE_LEVEL_TUR && pNewConfig->eVRegLevel != RAIL_VOLTAGE_LEVEL_TUR)
     {
       bResult = HAL_clk_DisableCXiPeakLMVote();
       if(!bResult)
       {
          DALSYS_LogEvent(
            DALDEVICEID_CLOCK,
            DALSYS_LOGEVENT_FATAL_ERROR,
            "Failed to un-vote for CX iPeak mitigation");
        }
     }  

  }
}


/* =========================================================================
**  Function : Clock_InitTarget
** =========================================================================*/
/*
  See ClockDriver.h.
*/

DALResult Clock_InitTarget (ClockDrvCtxt *pDrvCtxt)
{
  DALResult     eResult;
  RCECB_HANDLE  hRCECB;
  npa_event_handle  npacpunodehandle;
  ClockNodeType *pClock;
  ClockIdType nClockId;
  ClockImageCtxtType *pImageCtxt;
  
  /*-----------------------------------------------------------------------*/
  /* Initialize the XO module.                                             */
  /*-----------------------------------------------------------------------*/

  eResult = Clock_InitXO(pDrvCtxt);
  if (eResult != DAL_SUCCESS)
  {
    DALSYS_LogEvent(
      DALDEVICEID_CLOCK, DALSYS_LOGEVENT_FATAL_ERROR,
      "Unable to init XO.");

    return eResult;
  }
  
  Clock_DefineRemoteResourceNodes(pDrvCtxt);
 
  eResult = Clock_InitImage(pDrvCtxt);
  if (eResult != DAL_SUCCESS)
  {
    return DAL_ERROR;
  }
  
  /*
   * Remove any votes that may be leftover from earlier boot (MBA). We rely
   * on BCM votes keeping GPLL0 on implicitly.
   */
  HWIO_OUTF(GCC_MSS_Q6_GPLL_ENA_VOTE, GPLL0, 0);

  /*
   * Vote for MSS-specific GPLL0 branch. We can't set the SLEEP_ENA bit
   * because it's apparently required for the RSC to function on the way down.
   */
  HWIO_OUTF(GCC_MSS_Q6_CLOCK_BRANCH_ENA_VOTE, MSS_GPLL0_DIV_CLK_SRC_ENA, 1);
  
  /*
   * Set divider to div-2 to provide MPSS with a 150MHz source.
   * The input clock to CDIV is 300 MHz (gpll0_out_aux2_div)
   */
  HWIO_OUTF(GCC_MSS_PLL0_MAIN_DIV_CDIVR, CLK_DIV, 1);

  /*
   * Register for the end of RCINIT.
   */
  hRCECB = rcecb_register_name(RCINIT_RCECB_INITGROUPS, Clock_TargetPostInit);
  if (hRCECB == RCECB_NULL)
  {
    return DAL_ERROR;
  }
  
  pImageCtxt  = (ClockImageCtxtType *)pDrvCtxt->pImageCtxt;  
  
  if(pImageCtxt->pBSP->bCPUCXIpeakEnabled)
  {
    Clock_GetClockId(pDrvCtxt,"clk_q6", &nClockId);
    pClock = Clock_GetClockNode(pDrvCtxt, nClockId);
  
    npacpunodehandle = npa_create_custom_event("/clk/cpu",
                                             "CX-iPeakMitigation",
                                              NPA_TRIGGER_POST_CHANGE_EVENT | NPA_TRIGGER_PRE_CHANGE_EVENT,	 
                                              NULL,
                                              QDSP6SSClockStateChangeCallback,
                                              (void *)pClock);
  }
  
  return DAL_SUCCESS;

} /* END Clock_InitTarget */


/* =========================================================================
**  Function : Clock_TargetPostInit
** =========================================================================*/
/*
  Perform any necessary actions once image initialization has completed
  (i.e. end of rcinit).
*/

static void Clock_TargetPostInit (void)
{
  uSleep_notification_cb_handle hUImageCallback;

  /*
   * Register micro image callback used to vote for GPLL0 while in micro
   * image, since all BCM votes go away in that case.
   */
  hUImageCallback =
    uSleep_registerNotificationCallback(100, 100, Clock_UImageCallback);
  if (!hUImageCallback)
  {
    DALSYS_LogEvent(DALDEVICEID_CLOCK, DALSYS_LOGEVENT_WARNING,
      "Unable to register uSleep callback.");
  }
  
  /*
    This function is called when the final RCINIT group completes in order for
    the clock driver to reduce its vote on /clk/bimc from MAX_NPA_STATE to 1.
    This is needed to ensure that some entity (the clock driver in this instance)
    maintains a non-zero vote on /clk/bimc so that RPM doesn't power collapse
    BIMC while MPSS is in the active set.
   */
   
  
   if (hClientBIMC == NULL)
  {
    DALSYS_LogEvent (
      DALDEVICEID_CLOCK,
      DALSYS_LOGEVENT_FATAL_ERROR,
      "No /clk/bimc NPA client handle to vote on in RCECB callback");
  }

  npa_issue_scalar_request(hClientBIMC, 1);
  
} /* END Clock_TargetPostInit */


/* =========================================================================
**  Function : ClockStub_InitTarget
** =========================================================================*/
/*
  See ClockDriver.h.
*/

DALResult ClockStub_InitTarget (ClockDrvCtxt *pDrvCtxt)
{
  return DAL_SUCCESS;

} /* END ClockStub_InitTarget */


/* =========================================================================
**  Function : Clock_ProcessorSleep
** =========================================================================*/
/*
  See DDIClock.h
*/

DALResult Clock_ProcessorSleep
(
  ClockDrvCtxt *pDrvCtxt,
  ClockSleepModeType eMode,
  uint32 nFlags
)
{
  return DAL_SUCCESS;

} /* END Clock_ProcessorSleep */


/* =========================================================================
**  Function : Clock_ProcessorRestore
** =========================================================================*/
/*
  See DDIClock.h
*/

DALResult Clock_ProcessorRestore
(
  ClockDrvCtxt *pDrvCtxt,
  ClockSleepModeType eMode,
  uint32 nFlags
)
{
  return DAL_SUCCESS;

} /* END Clock_ProcessorRestore */


/* =========================================================================
**  Function : HAL_clk_GPLLStubEnableVote
** =========================================================================*/

static boolean HAL_clk_GPLLStubEnableVote
(
  HAL_clk_SourceDescType *pSource
)
{
  /*
   * Don't actually vote on PLL. We rely on BCM votes keeping GPLL0 on
   * implicitly. We can't enable our vote directly, since there are
   * suppressible domains requiring GPLL0, so our vote would never
   * go away. This would leave GPLL0 while entering XO shutdown. Relying
   * on BCM votes means GPLL0 will be on while active and automatically
   * turn off when modem goes down.
   */
  return TRUE;

} /* END HAL_clk_GPLLStubEnableVote */


/* =========================================================================
**  Function : Clock_UImageCallback
** =========================================================================*/

static void Clock_UImageCallback (uSleep_state_notification state)
{
  if (state == USLEEP_STATE_ENTER)
  {
    /*
     * When entering micro image mode, assert vote for GPLL0 since all BCM votes
     * will go away and we can't rely on GPLL0 being on anymore. XO is still
     * voted, so this is safe.
     */
    HWIO_OUTF(GCC_MSS_Q6_GPLL_ENA_VOTE, GPLL0, 1);
  }
  else if (state == USLEEP_STATE_EXIT)
  {
    /*
     * When exiting micro image, remove vote for GPLL0 since BCM votes are back
     * and we can rely on BCM keeping it on.
     */
    HWIO_OUTF(GCC_MSS_Q6_GPLL_ENA_VOTE, GPLL0, 0);
  }

} /* END Clock_UImageCallback */

/* =========================================================================
**  XO remote resource creation related functions
** =========================================================================*/

/* =========================================================================
**  Function : Clock_NPAXODriverFunc
** =========================================================================*/
/**
  Handle state changes on the CXO node.
 
  This function handles state changes on the XO node and will forward
  updates to the low-power-resource.
 
  @param pResource [in] -- The NPA resource being requested.
  @param pClient [in]   -- Pointer to the client making the request.
  @param nState [in]    -- New state of the resource.

  @return
  New state of the resource.

  @dependencies
  None.
*/ 

static npa_resource_state Clock_NPAXODriverFunc
(
  npa_resource      *pResource,
  npa_client        *pClient,
  npa_resource_state nState
)
{
  
  if (pResource->internal_state[NPA_REQUIRED_INDEX] > 0 )
  {
    nState = CLOCK_SOURCE_REQUIRED;
  }
  else
  {
    nState = CLOCK_SOURCE_NOT_REQUIRED;
  }
  
  /*-----------------------------------------------------------------------*/
  /* Issue a request to the sleep node to enable or disable this node's    */
  /* LPRMs during sleep.                                                   */
  /*  0: XO is not required at all by any insuppressible clocks.           */
  /*  1: XO is required by insuppressible clocks.                          */
  /*-----------------------------------------------------------------------*/
  
  if (Clock_XO.lpr)
  {
    if (nState == CLOCK_SOURCE_REQUIRED)
    {
      kvp_reset(pCXOEnableKVP);
      rpm_post_request(RPM_SLEEP_SET, RPM_CLOCK_0_REQ, 0, pCXOEnableKVP);

      /* Only allow the cxo.required LPRM when XO is required to be on */
      npa_issue_required_request(Clock_XO.lpr, SLEEP_LPRM_NUM(1));
    }
    else
    {
      kvp_reset(pCXODisableKVP);
      rpm_post_request(RPM_SLEEP_SET, RPM_CLOCK_0_REQ , 0, pCXODisableKVP);

      /* Only allow the cxo.shutdown LPRM when XO is not required */
      npa_issue_required_request(Clock_XO.lpr, SLEEP_LPRM_NUM(0));
    }
  }

  /*-----------------------------------------------------------------------*/
  /* Log XO LPR event                                                      */
  /*-----------------------------------------------------------------------*/

  Clock_SWEvent(CLOCK_EVENT_XO, 1, !nState);

  return nState;

} /* END Clock_NPAXODriverFunc */


/* =========================================================================
**  Function : Clock_LPRNodeAvailableCallback
** =========================================================================*/
/**
  Callback when the XO LPR node is created.
 
  This function is called by the NPA framework when the given LPR node is
  created.  The creation is delayed until all dependencies are also
  created.
 
  @param *pContext [in] -- Context passed in npa_define_node_cb
  @param nEventType [in] -- Zero.
  @param *pNodeName [in] -- Name of the node being created.
  @param nNodeNameSize [in] -- Length of the name.

  @return
  None.

  @dependencies
  None.
*/ 

static void Clock_LPRNodeAvailableCallback
(
  void        *pContext,
  unsigned int nEventType,
  void        *pNodeName,
  unsigned int nNodeNameSize
)
{
  /*-----------------------------------------------------------------------*/
  /* At init time, register the LPR with the sleep LPR node, and request   */
  /* a bit mask to use for the CXO LPRMs from the sleep LPR node.          */
  /*-----------------------------------------------------------------------*/

  Clock_XO.lpr =
    npa_create_sync_client(
      SLEEP_LPR_NODE_NAME,
      "/node/clock/cxo",
      NPA_CLIENT_REQUIRED);

  if (!Clock_XO.lpr)
  {
    DALSYS_LogEvent(
      DALDEVICEID_CLOCK,
      DALSYS_LOGEVENT_FATAL_ERROR,
      "Unable to create NPA client to cxo lpr");

    return;
  }

  sleepLPR_define("cxo", Clock_XO.lpr);

  /*-----------------------------------------------------------------------*/
  /* Thread safety.                                                        */
  /*-----------------------------------------------------------------------*/

  npa_resource_lock(NPA_RESOURCE_XO);

  /*-----------------------------------------------------------------------*/
  /* Trigger CPU state update.                                             */
  /*-----------------------------------------------------------------------*/

  npa_assign_resource_state(
    NPA_RESOURCE_XO,
    Clock_NPAXODriverFunc(
      NPA_RESOURCE_XO, NULL, NPA_RESOURCE_XO->request_state));

  /*-----------------------------------------------------------------------*/
  /* Free.                                                                 */
  /*-----------------------------------------------------------------------*/

  npa_resource_unlock(NPA_RESOURCE_XO);

} /* END Clock_LPRNodeAvailableCallback */


/* =========================================================================
**  Function : Clock_InitXO
** =========================================================================*/
/*
  See ClockMSS.h
*/

DALResult Clock_InitXO
(
  ClockDrvCtxt *pCtxt
)
{
  npa_resource_state nInitialState;

  /*-----------------------------------------------------------------------*/
  /* Allow space for two kvp values and populate them                      */
  /*-----------------------------------------------------------------------*/

  pCXODisableKVP = kvp_create(KVP_ITEM_SZ);
  pCXOEnableKVP  = kvp_create(KVP_ITEM_SZ);
  kvp_put(pCXOEnableKVP, KVP_ENABLE_REQUEST, sizeof(nCXOEnableReq), 
       (void *)&nCXOEnableReq);
  kvp_put(pCXODisableKVP, KVP_ENABLE_REQUEST, sizeof(nCXODisableReq), 
       (void *)&nCXODisableReq);

  /*-----------------------------------------------------------------------*/
  /* Init CXO node with initial vote of OFF in the sleep set.              */
  /*-----------------------------------------------------------------------*/

  Clock_XO.node.data = (npa_user_data)pCtxt;

  nInitialState = 0;

  npa_define_node_cb(
    &Clock_XO.node,
    &nInitialState,
    NULL,
    &Clock_XO.node
    );

  /*-----------------------------------------------------------------------*/
  /* Create callback for the Sleep XO LPR.                                 */
  /*-----------------------------------------------------------------------*/

  npa_resource_available_cb(
    SLEEP_LPR_NODE_NAME,
    Clock_LPRNodeAvailableCallback,
    NULL);
 
  /*-----------------------------------------------------------------------*/
  /* Good to go.                                                           */
  /*-----------------------------------------------------------------------*/

  return DAL_SUCCESS;

} /* END Clock_InitXO*/

/* =========================================================================
**  Function : Clock_DefineRemoteResourceNodes
** =========================================================================*/
/*
  Define Remote Resource Nodes.
*/
void Clock_DefineRemoteResourceNodes
(
ClockDrvCtxt *pCtxt
)
{ 
  /*-----------------------------------------------------------------------*/
  /* Create our BIMC remote node.                                          */
  /*-----------------------------------------------------------------------*/

  npa_remote_define_resource_cb(
    &Clock_aBIMCNPARemoteResource,
    (npa_resource_state)NPA_MAX_STATE,
    Clock_NPABIMCNodeAvailableCallback,
    NULL);

}


/* =========================================================================
**  Function : Clock_NPABIMCNodeAvailableCallback
** =========================================================================*/
/**
  Callback when a /clk/bimc node is created.

  This function is called by the NPA framework when the given /clk/bimc node
  is created.  The creation is delayed until all dependencies are also
  created.

  @param *pContext     [in] -- Context passed in npa_define_node_cb
  @param nEventType    [in] -- Zero.
  @param *pNodeName    [in] -- Name of the node being created.
  @param nNodeNameSize [in] -- Length of the name.

  @return
  None.

  @dependencies
  None.
*/

static void Clock_NPABIMCNodeAvailableCallback
(
  void        *pContext,
  unsigned int nEventType,
  void        *pNodeName,
  unsigned int nNodeNameSize
)
{

  hClientBIMC =
    npa_create_sync_client(
      "/clk/bimc",
      "/clock",
      NPA_CLIENT_SUPPRESSIBLE);

  if (hClientBIMC == NULL)
  {
    DALSYS_LogEvent (
      DALDEVICEID_CLOCK,
      DALSYS_LOGEVENT_FATAL_ERROR,
      "Unable to create NPA sync client %s->%s.",
      "/clk/bimc",
      "/clock");
  }

  /*-----------------------------------------------------------------------*/
  /* Place floor vote in the active set for BIMC so that RPM never power   */
  /* collapses BIMC while MPSS is in the active set.                       */
  /* The value of 1 is used as the minimal value above 0 which will be     */
  /* mapped to an actual performance level on RPM.                         */
  /*-----------------------------------------------------------------------*/

  npa_issue_scalar_request(hClientBIMC, NPA_MAX_STATE);

} /* END Clock_NPABIMCNodeAvailableCallback */
