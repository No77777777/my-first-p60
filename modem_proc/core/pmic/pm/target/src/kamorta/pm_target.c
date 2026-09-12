/*! \file pm_target_information.c
*  
*  \brief This file contains target specific PMIC settings common across processors.
*  \n   
*  &copy; Copyright 2012-2014 QUALCOMM Technologies Incorporated, All Rights Reserved
*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This document is created by a code generator, therefore this section will
  not contain comments describing changes made to the module.

$Header: //components/rel/core.mpss/10.0/pmic/pm/target/src/kamorta/pm_target.c#5 $ 

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include "pm_target.h"
#include "DALSys.h"
#include "pm_version_p.h"
#include "pm_utils.h"
#include "pm_comm.h"
#include "device_info.h"
#include "DDIChipInfo.h"
#include "ChipInfoDefs.h"
#include "pm_ulog.h"
#include "pm_target_information.h"
#include "pm_dal_prop_ids.h"
#include "npa.h"
#include "npa_resource.h"
#include "npa_remote_resource.h"
#include "npa_scheduler.h"
#include "pm_clk_p.h"

/*===========================================================================

                VARIABLES DEFINITIONS

===========================================================================*/

extern boolean bPMICStub;

#define PMIC_RSRC_CLK_HOLD   "/pm/rclkha1/en"
#define PMIC_NODE_CLK_HOLD   "/node/pm/rclkha1"
#define PM_CLK_HOLD_CHIP_ID  0
#define PM_CLK_BUFFER_ID     PM_RF_CLK1

npa_resource_state
pm_target_clk_buff_hold_driver_fcn (npa_resource      *resource,
                                    npa_client_handle  client,
                                    npa_resource_state state);


/*
* Resource Definiton
*/
npa_resource_definition pm_clk_buff_hold_rsrc = 
{
  .name       = PMIC_RSRC_CLK_HOLD,
  .units      = "enable",
  .max        = 1,
  .plugin     = &npa_max_plugin,
  .attributes = ( NPA_RESOURCE_DEFAULT |
                  NPA_RESOURCE_DRIVER_UNCONDITIONAL_FIRST ),
  .data       = NULL,
  .query_fcn  = NULL,
}; 

/*
* Node Definiton
*/
npa_node_definition pm_clk_buff_hold_node = 
{
  .name             = PMIC_NODE_CLK_HOLD, 
  .driver_fcn       = pm_target_clk_buff_hold_driver_fcn, 
  .attributes       = NPA_NODE_DEFAULT,
  .data             = NULL, 
  .dependency_count = 0, 
  .dependencies     = NULL, 
  .resource_count   = 1, 
  .resources        = &pm_clk_buff_hold_rsrc,
};

static pm_npa_pam_data_type *pam_data = NULL;
static pm_target_data_type  *target_data = NULL;
extern boolean  bPMICStub;
static char *pmic_props = "/modem/kamorta";

char*
pm_target_get_pmic_props (boolean *default_flag)
{
  if(default_flag == NULL)
  {
    PM_LOG_MSG_ERR_FATAL("Null Pointer Default Flag");
  }
  if ( PMIC_IS_PM2250 == pm_get_pmic_model(0)) 
  {
    pmic_props = "/modem/agatti";
  }

  *default_flag = FALSE;

  return pmic_props;
}

pm_err_flag_type
pm_target_get_pmk_info(boolean *is_pmk_present, 
                       uint8 *pmk_index)
{
  pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;

  if(is_pmk_present == NULL || pmk_index == NULL)
  {
    PM_LOG_MSG_INFO("Null pointer passed");
    return PM_ERR_FLAG_INVALID_PARAMETER;
  }

  if(PM_ERR_FLAG_SUCCESS == pm_is_pmic_supported(PMIC_IS_PMK8002, pmk_index))
  {
    *is_pmk_present = TRUE;
  }
  else
  {
    *is_pmk_present = FALSE;
    *pmk_index = (PM_MAX_NUM_PMICS+1);
  }

  return err_flag;
}

pm_npa_pam_data_type*
pm_target_get_pam_data(void)
{
  pm_model_type pm_model    = PMIC_IS_UNKNOWN;
  uint8         check_pmic  = 0;

  /* For Nicobar get model of pmic 1 for loading AU vs Regular PAM*/
  pm_model = pm_get_pmic_model(check_pmic);

  if(pm_model == PMIC_IS_UNKNOWN || pm_model == PMIC_IS_INVALID)
  {
    if (bPMICStub == FALSE)
    {
      PM_LOG_MSG_ERR_FATAL ("1st PMIC should be detected for Nicobar");
    }
  }
 pam_data = (pm_npa_pam_data_type*)
            pm_target_info_get_prop_info(PM_PAM_DATA);

  return pam_data; 
}


pm_target_data_type*
pm_target_get_target_data(void)
{
  pm_model_type pm_model    = PMIC_IS_UNKNOWN;
  uint8         check_pmic  = 0;

  /* For Nicobar get model of pmic 1 for loading AU vs Regular PAM*/
  pm_model = pm_get_pmic_model(check_pmic);

  if(pm_model == PMIC_IS_UNKNOWN || pm_model == PMIC_IS_INVALID)
  {
    if (bPMICStub == FALSE)
    {
      PM_LOG_MSG_ERR_FATAL ("1st PMIC should be detected for Nicobar");
    }
  }
  target_data = (pm_target_data_type*)
              pm_target_info_get_prop_info(PM_TARGET_DATA);
  return target_data;
}

npa_resource_state
pm_target_clk_buff_hold_driver_fcn (npa_resource       *resource,
                                    npa_client_handle  client,
                                    npa_resource_state state)
{
  pm_err_flag_type err_flag = PM_ERR_FLAG__SUCCESS;
  
  if(resource == NULL || client == NULL)
  {
    PM_LOG_MSG_ERR_FATAL("Invalid Inputs");
  }
  
  if(state > resource->definition->max)
  {
    PM_LOG_MSG_ERR_FATAL("Invalid enable state %x", state);
  }

  if(client->type == NPA_CLIENT_REQUIRED)
  {
    err_flag |= pm_clk_hold_enable( PM_CLK_HOLD_CHIP_ID,
                                    PM_CLK_BUFFER_ID,
                                    (pm_on_off_type )state );

    if(err_flag != PM_ERR_FLAG_SUCCESS)
    {
      PM_LOG_MSG_ERR_FATAL ("Unable to set clk buff hold");
    }
  }
  else if (client->type == NPA_CLIENT_INITIALIZE)
  {
    //Retrun state during initialization call
    return state;
  }
  else
  {
    PM_LOG_MSG_ERR_FATAL ("Client type not supported");
  }


  PM_LOG_MSG_INFO("Done Rsrc=%s, Client=%s, state=%x, Type=0x%x, attr=0x%x",
                   client->resource_name, client->name, state, 
                   client->type, client->request_attr);

return state;
}

void
pm_target_clk_buff_hold_init (void)
{
  if (bPMICStub == TRUE)
  {
    npa_stub_resource(pm_clk_buff_hold_rsrc.name);
  }
  else
  {
    npa_define_node (&pm_clk_buff_hold_node, NULL, NULL);
  }
  return;
}

void
pm_target_pre_init(void)
{
	return;
}