/*============================================================================
  @file therm_target_utils.c

  Define NPA nodes representing WCSS Cold Temperature state.

  Copyright (c) 2019 QUALCOMM Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
============================================================================*/
/*=======================================================================
$Header: //components/rel/core.mpss/10.0/power/thermal/src/target/divar/therm_target_utils.c#3 $
$DateTime: 2021/09/22 05:47:29 $
$Author: pwbldsvc $
========================================================================*/
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "npa_resource.h"
#include "npa_remote_resource.h"
#include "CoreVerify.h"
#include "therm_log.h"
#include "../therm_target_utils.h"
#include "DalDevice.h"
#include "DALStdErr.h"
#include "DDISpmi.h"
#include "SpmiTypes.h"
#include "kvp.h"
#include "rpm.h"
#include "rpmclient.h"
#include "pm_npa_device.h"

/*=======================================================================

                                MACROS

========================================================================*/ 
#define ARRAY_SIZE(x)                          sizeof(x)/sizeof(x[0])                   
#define SPARE_SLAVE_ID                         0x0
#define SDAM3_BASE_ADDRESS                     0xB200
#define SPARE_RETENTION_WCSS_CX_ACTIVE_LB      (SDAM3_BASE_ADDRESS+0x55)
#define SPMI_MAX_READ_COUNT                    2


/*=======================================================================

                  STATIC MEMBER / FUNCTION DECLARATIONS / DEFINITIONS

========================================================================*/

static npa_resource_state therm_node_wcss_cx_coldtemp_driver_fcn(npa_resource *resource,
                                                         npa_client   *client,
                                                         npa_resource_state state);

/*=======================================================================

                  GLOBAL DEFINITIONS

========================================================================*/

static npa_resource_plugin therm_wcss_cx_coldtemp_plugin;
static DalDeviceHandle *thermal_spmi_dal_handle = NULL;
static boolean thermal_spmi_initialized = FALSE;

static npa_resource_definition therm_wcss_cx_coldtemp_resource[] =
{
  {
    "/therm/coldtemp/wcss_cx",               /* name of resource */
    "State Level",                         /* Units of the resource */
    0x1,                                   /* Maximum value of resource */
    &therm_wcss_cx_coldtemp_plugin,
    NPA_RESOURCE_DRIVER_UNCONDITIONAL,
    (void *)0x0,
    NULL
  },
};

static npa_resource_state initial_wcss_cx_coldtemp_state = 0;

npa_node_definition therm_wcss_cx_coldtemp_node =
{
  "/node/therm/coldtemp/wcss_cx",                  /* Node name - info only */
  therm_node_wcss_cx_coldtemp_driver_fcn,          /* Driver function for temperature */
  NPA_NODE_DEFAULT,                              /* No attributes */
  NULL,                                          /* No User Data */
  NPA_EMPTY_ARRAY,                               /* No Dependencies */
  NPA_ARRAY(therm_wcss_cx_coldtemp_resource)       /* Resources */
};

static const char *therm_resources_for_userpds[] = 
{ 
  "/therm/coldtemp/wcss_cx",
  "/therm/mitigate/cpuv_restriction_cold",
};

/*=======================================================================

                  LOCAL FUNCTION DEFINITIONS

========================================================================*/

/**
  @brief therm_read_byte_array

  Funtion to read continuous WCSS_CX sapre register values.
  Regsiter values are stored in the array passed as argument.

  @param  : data     : Array Start Address.
            numBytes : Size of array.

  @return : 0 on success or -1 on failure.
*/
static int therm_read_byte_array(uint8 *data, uint32 numBytes) 
{
  int                  ret_val = -1;
  SpmiBus_ResultType   spmi_result = SPMI_BUS_SUCCESS;
  uint32               dataReadLength = 0;
  uint8                count = 0;
   
  if(thermal_spmi_initialized == TRUE)
  {
    do
    {
      count++;
      spmi_result = DalSpmi_ReadLong(thermal_spmi_dal_handle,
                                     SPARE_SLAVE_ID, 
                                     (SpmiBus_AccessPriorityType)1, 
                                     SPARE_RETENTION_WCSS_CX_ACTIVE_LB, 
                                     data, 
                                     numBytes, 
                                     &dataReadLength);
      if(spmi_result == SPMI_BUS_SUCCESS)
      {
        ret_val = 0;
        break;
      }
      else
      {
        therm_log_printf(THERM_LOG_LEVEL_ERROR, 2,
                          "SPMI read Failed with error code %d, Count %d", spmi_result, count);
      }
      if(count < SPMI_MAX_READ_COUNT)
      {
        DALSYS_BusyWait(5);
      }
    }while(count < SPMI_MAX_READ_COUNT);   
  }
  return ret_val ;
}

/**
  @brief create_thermal_spmi_dal_handle

  Funtion to create SMPI DAL Handle.

  @param  : None.

  @return : 0 on success or -1 on failure.
*/
static int create_thermal_spmi_dal_handle()
{
  int           ret_val = -1;
  DALResult     dal_result = DAL_SUCCESS;
    
  dal_result = DAL_SpmiDeviceAttach("DALDEVICEID_SPMI_DEVICE", &thermal_spmi_dal_handle);

  if(dal_result == DAL_SUCCESS) 
  { 
    dal_result = DalDevice_Open(thermal_spmi_dal_handle, DAL_OPEN_SHARED);
    if (dal_result == DAL_SUCCESS)
    {
      thermal_spmi_initialized = TRUE;
      ret_val = 0;;
    }
    else
    {
      therm_log_printf(THERM_LOG_LEVEL_ERROR, 1,
                        "DAL Device Open failed with Error code %d",dal_result);
    }
  }
  else
  {
    therm_log_printf(THERM_LOG_LEVEL_ERROR, 1,
                      "DAL SPMI Device Attach failed with Error code %d",dal_result);
  }
  return ret_val;
}

/**
  @brief therm_node_wcss_cx_coldtemp_driver_fcn

  Read from WCSS spare register and Vote accordingly.  Nothing to update.

  @param  : resource: A dynamic system element that work requests can be made against.
            client: The handle to the clients registered to the
            resource.
            state: Update function state.

  @return : Resource state.
*/
static npa_resource_state therm_node_wcss_cx_coldtemp_driver_fcn(npa_resource *resource,
                                                              npa_client   *client,
                                                              npa_resource_state state)
{
  uint8  active_level[2]   = {0};
  uint32 active_voltage_mv =  0;
  uint32 active_voltage_uv = 0;

  if(client->type == NPA_CLIENT_INITIALIZE)
  {
  	
    if(create_thermal_spmi_dal_handle())
    {
      therm_log_printf(THERM_LOG_LEVEL_ERROR, 0,
                        "Creating SPMI DAL handle failed");
    }
    return state;
  }
  
  if(thermal_spmi_dal_handle == NULL || thermal_spmi_initialized == FALSE)
  {
    return state;
  }
  
  if(therm_read_byte_array(active_level,(uint32)(ARRAY_SIZE(active_level))))
  {
    therm_log_printf(THERM_LOG_LEVEL_ERROR, 0,
                      "Reading WCSS_CX spare register failed");
    return state;
  }
  
  active_voltage_mv = (uint32)(active_level[1]);
  active_voltage_mv = ((active_voltage_mv << 8) | ((uint32)(active_level[0])));  
  
  if(active_voltage_mv == 0)
  {
    therm_log_printf(THERM_LOG_LEVEL_ERROR, 0,
                      "PMIC WCSS_CX Active Voltage Read from spare register is zero");
    return state;
  }
  
  active_voltage_uv = active_voltage_mv * 1000; // Active voltage in micro volts
  
  kvp_t *thermal_kvp_active = kvp_create(0);
  kvp_t *thermal_kvp_sleep = kvp_create(0);
  
  kvp_put(thermal_kvp_active, PM_NPA_KEY_MICRO_VOLT, sizeof(active_voltage_uv), (void *)&(active_voltage_uv));
  kvp_put(thermal_kvp_sleep, PM_NPA_KEY_MICRO_VOLT, sizeof(active_voltage_uv), (void *)&(active_voltage_uv));
  
  rpm_sync_request(RPM_ACTIVE_SET, RPM_LDO_A_REQ, 8 , thermal_kvp_active);
  rpm_post_request(RPM_SLEEP_SET, RPM_LDO_A_REQ, 8 , thermal_kvp_sleep);
  
  therm_log_printf(THERM_LOG_LEVEL_INFO, 2,
                    "State %d, Applied Voltage on WCSS_CX is %umv", 
					  state, active_voltage_mv);
					  
  kvp_destroy(thermal_kvp_active);
  kvp_destroy(thermal_kvp_sleep);

  return state;
}

/*=======================================================================

                 PUBLIC FUNCTION DEFINITIONS

========================================================================*/

/**
  @brief therm_target_utils_init

  Thermal target utils init function.

  @param  : None.

  @return : None.
*/
void therm_target_utils_init( void )
{
  /* Copy all attributes of standard NPA max pluggin */
  memcpy(&therm_wcss_cx_coldtemp_plugin, &npa_max_plugin, sizeof(npa_resource_plugin));

  /* Define nodes */
  npa_define_node( &therm_wcss_cx_coldtemp_node, &initial_wcss_cx_coldtemp_state, NULL );

  /*Publish the required resources to user pd*/
  npa_remote_publish_resources(NPA_ALL_PDS, 
                                sizeof(therm_resources_for_userpds)/sizeof(char *),
                                  therm_resources_for_userpds );
}
