/*! \file
*  
*  \brief  pm_npa_device_ldo.c ----This file contains init functions for the RPM PMIC NPA
*          driver
*  \details 
*  
*  
*  &copy; Copyright 2012-2013 Qualcomm Technologies Incorporated, All Rights Reserved
*/
/*===========================================================================

            EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/pmic/pm/npa/src/pm_npa_device_ldo.c#1 $
  
when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
01/16/13   umr     Add Suppressible request support.
03/07/12   hs      Use DAl APIs to replace memory allocation. 
02/12/12   wra     KVP based device setting initial file 
===========================================================================*/
/*===========================================================================

             INCLUDE FILES FOR MODULE

===========================================================================*/
#include "pm_npa_device_ldo.h"
#include "pm_malloc.h" // pm_malloc
#include "pm_target_information.h"
#include "pm_sw_event.h"
#include "pm_npa.h"
#include "pm_ulog.h"

// LDO Create and Destroy Client Callbacks
void pm_npa_ldo_kvps_create_client_fcn(npa_client *client)
{
  // Allocate storage space because NPA discards the data otherwise.
  pm_malloc(sizeof(pm_npa_ldo_kvps), (void **)&client->resource_data);

  DALSYS_memset(client->resource_data, 0, sizeof(pm_npa_ldo_kvps));
}

void pm_npa_ldo_kvps_destroy_client_fcn(npa_client *client)
{
  // Clean up our allocated storage space.
  DALSYS_Free(client->resource_data);
}

static void
pm_npa_ldo_kvps_aggregate(pm_npa_ldo_kvps *kvps, pm_npa_ldo_kvps *request)
{
  // Aggregate Software Enable
  kvps->sw_enable_value = MAX(kvps->sw_enable_value, request->sw_enable_value);

  // Aggregate Software Mode Enable
  kvps->ldo_sw_mode_value = MAX(kvps->ldo_sw_mode_value, request->ldo_sw_mode_value);

  // Aggregate Maximum Voltage
  kvps->uv_value = MAX(kvps->uv_value, request->uv_value);

  return;
}

npa_resource_state pm_npa_ldo_kvps_update_fcn(npa_resource *resource, npa_client_handle client)
{
  npa_client_handle active_client = resource->clients;
  pm_npa_ldo_kvps  *kvps, *reqd_kvps, *client_req;
  npa_client_type req_type = client->type;


  // Firstly, save the client's request so that we don't lose it later.
  client_req = (pm_npa_ldo_kvps *)client->resource_data;
  DALSYS_memcpy(client_req, NPA_PENDING_REQUEST(client).pointer.vector, sizeof(pm_npa_ldo_kvps));
  NPA_PENDING_REQUEST(client).pointer.vector = (npa_resource_state *)client_req;

  /* Ovearall agg, both Reqd and supp */
  kvps = (pm_npa_ldo_kvps *)resource->state_vector;
  DALSYS_memcpy(kvps, client_req, sizeof(pm_npa_ldo_kvps));

  /* Reqd request Only*/
  reqd_kvps = (pm_npa_ldo_kvps *)resource->required_state_vector;
  if (req_type == NPA_CLIENT_VECTOR)
  {
    DALSYS_memcpy(reqd_kvps, client_req, sizeof(pm_npa_ldo_kvps));
  }

  while (active_client)
  {
    // Find the saved request for this client.
    pm_npa_ldo_kvps *request = active_client->resource_data;

    // If this is the current client, we have already started with his values.
    if (client == active_client)
    {
      active_client = active_client->next;
      continue;
     }
    /* Aggregate for both Suppressable and Required */
    pm_npa_ldo_kvps_aggregate (kvps, request);
    pm_sw_event(PMIC_LDO_AGG1, 3, 0, kvps->sw_enable_value, kvps->ldo_sw_mode_value); 
    pm_sw_event(PMIC_LDO_AGG2, 3, 0, kvps->uv_value); 

    if (req_type == NPA_CLIENT_VECTOR && active_client->type == NPA_CLIENT_VECTOR)
    {
      /* Aggregate only Required */
      pm_npa_ldo_kvps_aggregate (reqd_kvps, request);
      pm_sw_event(PMIC_LDO_AGG1, 3, 0, kvps->sw_enable_value);
    }
    active_client = active_client->next;
  }
  
   
  //Print final aggregated active set values     
  PM_LOG_MSG_DEBUG("ResName=%s ClientName=%s ActiveSet: enable = %d, mode=%d, uV = %d", 
            client->resource_name, client->name, kvps->sw_enable_value, kvps->ldo_sw_mode_value, 
            kvps->uv_value);
            
  //Print final aggregated required set values
  PM_LOG_MSG_DEBUG("ResName=%s ClientName=%s RequiredSet: enable = %d, mode=%d, uV = %d", 
            client->resource_name, client->name, reqd_kvps->sw_enable_value, reqd_kvps->ldo_sw_mode_value, 
            reqd_kvps->uv_value);
        
   return kvps->uv_value; // Return the voltage for quick perusal in logs; "real state" is in resource->state_vector.
}

npa_resource_plugin pm_npa_ldo_kvps_plugin =
{
  pm_npa_ldo_kvps_update_fcn,
  NPA_CLIENT_VECTOR | NPA_CLIENT_SUPPRESSIBLE_VECTOR,
  pm_npa_ldo_kvps_create_client_fcn,
  pm_npa_ldo_kvps_destroy_client_fcn,
};

/**
 * pm_npa_ldo_kvps_init
 *
 * See pm_npa_vreg.h
 */
pm_err_flag_type
pm_npa_ldo_kvps_init (pm_npa_ldo_kvps *kvps)
{
  if (kvps == NULL)
  {
    return PM_ERR_FLAG_INVALID_PARAMETER;
  }

  kvps->sw_enable_key = PM_NPA_KEY_SOFTWARE_ENABLE;
  kvps->sw_enable_size = 4;
  kvps->sw_enable_value = 0;

  kvps->ldo_sw_mode_key = PM_NPA_KEY_LDO_SOFTWARE_MODE;
  kvps->ldo_sw_mode_size = 4;
  kvps->ldo_sw_mode_value = 0;

  kvps->uv_key = PM_NPA_KEY_MICRO_VOLT;
  kvps->uv_size = 4;
  kvps->uv_value = 0;

  return PM_ERR_FLAG_SUCCESS;
}

npa_resource_plugin *pm_npa_get_ldo_kvps_plugin(void)
{
    return(&pm_npa_ldo_kvps_plugin);
}
