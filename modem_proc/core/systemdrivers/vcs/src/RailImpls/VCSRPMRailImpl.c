/*
===========================================================================
*/
/**
  @file VCSRPMRailImpl.c
  
  RPM functions for NPA rail resource.
*/
/*  
  ====================================================================

  Copyright (c) 2017 QUALCOMM Technologies Incorporated. All Rights Reserved.  
  QUALCOMM Proprietary and Confidential. 

  ==================================================================== 
  $Header: //components/rel/core.mpss/10.0/systemdrivers/vcs/src/RailImpls/VCSRPMRailImpl.c#1 $
  $DateTime: 2019/04/24 00:03:26 $
  $Author: pwbldsvc $
 
  ====================================================================
*/ 


/*=========================================================================
      Include Files
==========================================================================*/

#include "VCSDriver.h"
#include "VCSRPMRailImpl.h"
#include <DALSys.h>
#include "DALDeviceId.h"
#include <npa_resource.h>
#include "npa_scheduler.h"
#include "voltage_level.h"
#include "npa.h"

/*=========================================================================
      Prototypes
==========================================================================*/

static DALResult VCS_RPMRailImplInitImpl(VCSRailNodeType *);
static DALResult VCS_RPMRailImplSetCorner(npa_client_handle, rail_voltage_level);


/*=========================================================================
      Data
==========================================================================*/

VCSRailControlType VCSRPMRailImplControl =
{
  .fpInit      = VCS_RPMRailImplInitImpl,
  .fpSetCorner = VCS_RPMRailImplSetCorner,
};

rail_voltage_level  nRails[RAIL_VOLTAGE_LEVEL_NUM_LEVELS]=
{
   RAIL_VOLTAGE_LEVEL_OFF,

   RAIL_VOLTAGE_LEVEL_RET,

   RAIL_VOLTAGE_LEVEL_MIN_SVS,    
   
   RAIL_VOLTAGE_LEVEL_LOW_SVS,   

   RAIL_VOLTAGE_LEVEL_SVS,      

   RAIL_VOLTAGE_LEVEL_SVS_L1,      

   RAIL_VOLTAGE_LEVEL_NOM,        

   RAIL_VOLTAGE_LEVEL_NOM_L1,  

   RAIL_VOLTAGE_LEVEL_NOM_L2,       

   RAIL_VOLTAGE_LEVEL_TUR,           

   RAIL_VOLTAGE_LEVEL_TUR_L1         
};

/*=========================================================================
      Functions
==========================================================================*/

/* =========================================================================
**  Function : VCS_RPMRailImplSetCorner
** =========================================================================*/
/**
  Set RPM to a requested corner.

  @param hClient     [in] -- Pointer to client making request.
  @param eCornerRail [in] -- Requested corner.

  @return
  DAL_SUCCESS -- Sucessfully configured RPM to requested corner.
  DAL_ERROR -- Failed to configure RPM to requested corner.

  @dependencies
  None.
*/

static DALResult VCS_RPMRailImplSetCorner
(
  npa_client_handle  hClient,
  rail_voltage_level eCorner
)
{
  VCSRailNodeType *pRail;

  pRail = VCS_CLIENT_TO_RAIL(hClient);

  /*-----------------------------------------------------------------------*/
  /* Sanity.                                                               */
  /*-----------------------------------------------------------------------*/

  if (pRail == NULL)
  {
    return DAL_ERROR_INVALID_PARAMETER;
  }

  /*-----------------------------------------------------------------------*/
  /* Increase dependency before increasing                                 */
  /*-----------------------------------------------------------------------*/

  VCS_NPASatisfyIncreasingDependency(hClient);
  
  /* Nothing RPM specific to be done in SetCorner */
  
  /*-----------------------------------------------------------------------*/
  /* Decrease dependency after decreasing .                                */
  /*-----------------------------------------------------------------------*/

  VCS_NPASatisfyDecreasingDependency(hClient);

  /*-----------------------------------------------------------------------*/
  /* Update context.                                                       */
  /*-----------------------------------------------------------------------*/

  pRail->eCorner = eCorner;

  return DAL_SUCCESS;

} /* END of VCS_RPMRailImplSetCorner */

/* =========================================================================
**  Function : VCS_RPMRailImplResourceQuery
** =========================================================================*/
/**
  NPA rail resource query function.

  This function is called to get the following rail information:
    -- Number of corners.
    -- Voltage table for each corner.
    -- Current corner (id / table index).

  @param *pResource  [in]  -- Pointer to the resource in question
  @param  nID        [in]  -- ID of the query.
  @param *pResult    [out] -- Pointer to the data to be filled by this function.

  @return
  npa_query_status - NPA_QUERY_SUCCESS, if query supported.
                   - NPA_QUERY_UNSUPPORTED_QUERY_ID, if query not supported.

  @dependencies
  None.
*/

static npa_query_status VCS_RPMRailImplResourceQuery
(
  npa_resource   *pResource,
  unsigned int    nID,
  npa_query_type *pResult
)
{
  VCSRailNodeType     *pRail;
  VCSRPMRailImplType *pImpl;
  uint32              nindex, ncnt, nLevelCount;
  uint32              nNumLevels = 0;
  rail_voltage_level  nLevel;
  /*-----------------------------------------------------------------------*/
  /* Validate parameters.                                                  */
  /*-----------------------------------------------------------------------*/

  if (pResource == NULL || pResult == NULL)
  {
    return NPA_QUERY_NULL_POINTER;
  }

  pRail = VCS_RESOURCE_TO_RAIL(pResource);
  if (pRail == NULL || pRail->pImpl == NULL)
  {
    return NPA_QUERY_NULL_POINTER;
  }

  pImpl = (VCSRPMRailImplType *)pRail->pImpl;
  
  /*-----------------------------------------------------------------------*/
  /* Get the number of corners supported.                                  */
  /*-----------------------------------------------------------------------*/
  
  if( pRail->pActiveConfig == NULL)
  {
    nNumLevels =  RAIL_VOLTAGE_LEVEL_NUM_LEVELS;
  }
  else
  {
    for (nindex = 0; nindex<RAIL_VOLTAGE_LEVEL_NUM_LEVELS; nindex++)
    {
       if( pRail->pActiveConfig->eCornerMin <= nRails[nindex] &&
           pRail->pActiveConfig->eCornerMax >= nRails[nindex] )
       {
         nNumLevels++;
       }
    }
  }

  /*-----------------------------------------------------------------------*/
  /* Check for number of corners supported.                                */
  /*-----------------------------------------------------------------------*/

  if (nID == VCS_NPA_QUERY_VOLTAGE_NUM_ENTRIES)
  {
    pResult->data.value = nNumLevels;
    pResult->type = NPA_QUERY_TYPE_VALUE;

    return NPA_QUERY_SUCCESS;
  }

  /*-----------------------------------------------------------------------*/
  /* Check for current corner in VLVL.                                     */
  /*-----------------------------------------------------------------------*/

  if(nID == VCS_NPA_QUERY_VOLTAGE_CURRENT_LEVEL_ID)
  {
    pResult->data.value = pRail->eCorner;
    pResult->type = NPA_QUERY_TYPE_VALUE;

    return NPA_QUERY_SUCCESS;
  }

  /*-----------------------------------------------------------------------*/
  /* Check for corner VLVL at specified performance level.                 */
  /*-----------------------------------------------------------------------*/

  if (nID >= VCS_NPA_QUERY_VOLTAGE_LEVEL_ID && nID < VCS_NPA_QUERY_END_MARKER)
  {
    nindex = nID - VCS_NPA_QUERY_VOLTAGE_LEVEL_ID;
    if (nindex < nNumLevels)
    {
      if( pRail->pActiveConfig == NULL)
      {
        nLevel = nRails[nindex];
      }
      else
      {
        nLevelCount = 0;
        for (ncnt = 0; ncnt<RAIL_VOLTAGE_LEVEL_NUM_LEVELS; ncnt++)
        {
          if( pRail->pActiveConfig->eCornerMin <= nRails[ncnt] &&
              pRail->pActiveConfig->eCornerMax >= nRails[ncnt] )
          {
            nLevelCount++;
          }
          else
          { 
            continue;
          }
          if(nLevelCount == nindex + 1)
          {
            nLevel =  nRails[ncnt];
            break;
          }
        }
        if(ncnt == RAIL_VOLTAGE_LEVEL_NUM_LEVELS)
        {
          return NPA_QUERY_UNSUPPORTED_QUERY_ID;
        }
      }
      pResult->data.value = nLevel;
      pResult->type = NPA_QUERY_TYPE_VALUE;

      return NPA_QUERY_SUCCESS;
    }
  }

  return NPA_QUERY_UNSUPPORTED_QUERY_ID;

} /* END VCS_RPMRailImplResourceQuery */

/* =========================================================================
**  Function : VCS_RPMRailImplInitImpl
** =========================================================================*/
/**
  Initializes the RPM data for this rail.

  @param *pRail [in] -- Pointer rail node.

  @return
  DAL_ERROR if a corner list not initialized, other DAL_SUCCESS.

  @dependencies
  None.
*/

static DALResult VCS_RPMRailImplInitImpl
(
  VCSRailNodeType *pRail
)
{
  npa_remote_resource_definition nRemoteResource;
  VCSRPMRailImplType             *pImpl;
  
  /*-----------------------------------------------------------------------*/
  /* Sanity.                                                               */
  /*-----------------------------------------------------------------------*/

   if (pRail == NULL ||
      pRail->pImpl == NULL)
  {
    return DAL_ERROR_INVALID_PARAMETER;
  }
  
  pImpl = (VCSRPMRailImplType *)pRail->pImpl;
  
  nRemoteResource.local_resource_name = (char *)pImpl->szLocalName;
  nRemoteResource.remote_resource_name = (char *)pImpl->szRemoteName;
  nRemoteResource.units = (char *)pImpl->szUnits;
  
  nRemoteResource.protocol_type = "/protocol/rpm/rpm";
  nRemoteResource.plugin = &npa_max_plugin;
  nRemoteResource.driver_fcn =
  npa_remote_resource_local_aggregation_driver_fcn;
  nRemoteResource.max = NPA_MAX_STATE;
  nRemoteResource.attributes = NPA_RESOURCE_REMOTE_NO_INIT;

  npa_remote_define_resource(&nRemoteResource, 0, NULL);
  
  /*-----------------------------------------------------------------------*/
  /* Install function pointer for rail resource query.                     */
  /*-----------------------------------------------------------------------*/

  VCS_RAIL_RES_DEF_RAIL(pRail)->query_fcn = VCS_RPMRailImplResourceQuery;
  
  return DAL_SUCCESS;

} /* END VCS_RPMRailImplInitImpl */
