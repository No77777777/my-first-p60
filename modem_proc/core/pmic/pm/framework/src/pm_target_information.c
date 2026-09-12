/*! \file pm_target_information.c
*  
*  \brief This file contains target specific PMIC settings common across processors.
*  \n   
*  &copy; Copyright 2018 QUALCOMM Technologies Incorporated, All Rights Reserved
*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This document is created by a code generator, therefore this section will
  not contain comments describing changes made to the module.

$Header: //components/rel/core.mpss/10.0/pmic/pm/framework/src/pm_target_information.c#1 $ 

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include "pm_target_information.h"
#include "DALSys.h"
#include "pm_version_p.h"
#include "pm_utils.h"
#include "pm_comm.h"
#include "device_info.h"
#include "DDIChipInfo.h"
#include "ChipInfoDefs.h"
#include "pm_ulog.h"
#include "pm_target.h"
#include "pm_config_target.h"
/*===========================================================================

                        STATIC VARIABLES 

===========================================================================*/

static DALSYS_PROPERTY_HANDLE_DECLARE(hProp_target);
static DALSYSPropertyVar prop_target;
static boolean default_cfg = FALSE;
pm_npa_pam_data_type *pm_pam_data = NULL;
pm_target_data_type  *pm_target_data = NULL;

static struct {
  boolean is_pmk_present;
  uint8   pmk_chip_index;
}pmk_info; 

extern boolean  bPMICStub;

void
pm_target_info_init(void)
{
  char* pmic_props = NULL;
  //uint8 pm_model   = 0;
  //uint8 pmk_chip;
  
  pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;

  pmic_props = pm_target_get_pmic_props(&default_cfg);

  if(pmic_props == NULL)
  {
    PM_LOG_MSG_ERR_FATAL("PMIC Props were NULL");
  }

  err_flag |= pm_target_get_pmk_info(&pmk_info.is_pmk_present, 
                                     &pmk_info.pmk_chip_index);

  if(err_flag != PM_ERR_FLAG_SUCCESS)
  {
    PM_LOG_MSG_ERR_FATAL("Unable to read PMK 0x%x", err_flag);
  }
  
  if(DAL_SUCCESS != DALSYS_GetDALPropertyHandleStr((const char*)pmic_props,
                                                    hProp_target))
  {
    PM_LOG_MSG_ERR_FATAL ("Failed hProp_target handle retrieval for %s",
                           pmic_props);
  }

  pm_pam_data = pm_target_get_pam_data();
  if (pm_pam_data == NULL)
  {
    PM_LOG_MSG_ERR_FATAL("NULL PAM Data Received");
  }

  pm_target_data = pm_target_get_target_data();

  if(pm_target_data == NULL)
  {
    PM_LOG_MSG_ERR_FATAL("NULL Target Data Received");
  }
  
  pm_target_clk_buff_hold_init ();

  return;
}

boolean 
pm_target_info_is_default_cfg(void)
{
  return default_cfg;
}

boolean 
pm_target_info_pmk_present(uint8 *pmic_index)
{
  if(pmk_info.is_pmk_present==TRUE)
  {
    *pmic_index=pmk_info.pmk_chip_index;
  }
  
  return pmk_info.is_pmk_present;
}

pm_npa_pam_data_type*
pm_target_info_pam_data(void)
{
  return pm_pam_data;
}

pm_target_data_type*
pm_target_info_target_data(void)
{
  return pm_target_data;
}

pm_err_flag_type 
pm_target_info_read_periph_rev(pm_comm_info_type *comm_ptr,
                               peripheral_info_type *peripheral_info)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG__SUCCESS;
    pm_register_address_type base_address = 
                  (pm_register_address_type)peripheral_info->base_address;
    uint32 digital_minor_index = 0;
    uint32 digital_major_index = 1;
    uint32 analog_minor_index = 2;
    uint32 analog_major_index = 3;
    uint32 peripheral_type_index = 4;
    uint32 peripheral_subtype_index = 5;
    uint32 peripheral_dummy_index = 6;
    const uint32 num_of_bytes = 7;
    pm_register_data_type temp_peripheral_info[7] = {0};

    if(!comm_ptr)
    {
        return PM_ERR_FLAG__COMM_TYPE_NOT_RECOGNIZED;
    }

    // Reset the peripheral info
    peripheral_info->peripheral_type = 0;
    peripheral_info->peripheral_subtype = 0;
    peripheral_info->analog_major_version = 0;
    peripheral_info->analog_minor_version = 0;
    peripheral_info->digital_major_version = 0;
    peripheral_info->digital_minor_version = 0;

    /* Burst read the peripheral info */
    pm_comm_read_byte_array(comm_ptr->slave_id, base_address, num_of_bytes, temp_peripheral_info, 0);

    /* When we burst read multiple bytes from a non-existent peripheral, the data returned
       should be same on all the bytes so we need to do the below check and return error */
    if(temp_peripheral_info[peripheral_subtype_index] == temp_peripheral_info[peripheral_dummy_index]) 
    {
        return PM_ERR_FLAG__INVALID_RESOURCE_ACCESS_ATTEMPTED;
    }

    /* Valid peripheral type can never be 0 */   
    if(temp_peripheral_info[peripheral_type_index] == 0) 
    {
        return PM_ERR_FLAG__INVALID_RESOURCE_ACCESS_ATTEMPTED;
    }
    
    peripheral_info->peripheral_type = temp_peripheral_info[peripheral_type_index];    
    peripheral_info->peripheral_subtype = temp_peripheral_info[peripheral_subtype_index];
    peripheral_info->digital_major_version = temp_peripheral_info[digital_major_index];
    peripheral_info->digital_minor_version = temp_peripheral_info[digital_minor_index];
    peripheral_info->analog_major_version = temp_peripheral_info[analog_major_index];
    peripheral_info->analog_minor_version = temp_peripheral_info[analog_minor_index];
    
    return err_flag;
}


void* 
pm_target_info_get_prop_info(const char* prop_id)
{
  void *info = NULL;

  if(DAL_SUCCESS == DALSYS_GetPropertyValue(hProp_target, prop_id, NULL, &prop_target))
  {
      info = (void*)prop_target.Val.pStruct;
  }

  return info;
}
