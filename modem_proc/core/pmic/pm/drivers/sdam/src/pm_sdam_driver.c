/*! \file pm_sdam_driver.c
*  \n
*  \brief SDAM driver initialization.
*  \n
*  \n &copy; Copyright 2018 QUALCOMM Technologies Incorporated, All Rights Reserved
*/
/* =======================================================================
                            Edit History
This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/pmic/pm/drivers/sdam/src/pm_sdam_driver.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/24/18   rl      Created
========================================================================== */
/*===========================================================================

                     INCLUDE FILES

===========================================================================*/
#include "pm_sdam_driver.h"
#include "CoreVerify.h"
#include "hw_module_type.h"
#include "pm_malloc.h"
#include "device_info.h"
#include "pm_target_information.h"
#include "pm_version_p.h"
#include "pm_ulog.h"

/*===========================================================================

                         DEFINITIONS

===========================================================================*/

#define REG_SDAM(reg) ((sdam_ptr->sdam_reg_table->base_address)     \
    + ((sdam_index)*(sdam_ptr->sdam_reg_table->peripheral_offset))  \
    +  (sdam_ptr->sdam_reg_table->reg))

#define SLAVE_SDAM sdam_ptr->comm_ptr->slave_id

#define SDAM_SIZE_MULTIPLIER 32

/*===========================================================================

                        STATIC VARIABLES

===========================================================================*/

/* Static global variable to store the sdam data */
static pm_sdam_data_type *pm_sdam_data_arr[PM_MAX_NUM_PMICS];

static pm_sdam_reg_info_type sdam_reg =
{
  .base_address      = 0xB000,
  .peripheral_offset = 0x0100,
  .mem_start         = 0x0040,
  .trig_arg          = 0x0042,
  .trig_busy_ret     = 0x0043,
  .sdam_size         = 0x0044,
  .trig_set          = 0x00E5,
  .trig_clr          = 0x00E6,
};


/*===========================================================================

                        FUNCTION DEFINITIONS

===========================================================================*/
void
pm_sdam_driver_init(pm_comm_info_type *comm_ptr,
                    peripheral_info_type *peripheral_info,
                    uint8 pmic_index)
{
  pm_sdam_data_type         *sdam_ptr       = NULL;
  pm_sdam_supported_periph  *sdam_supported = NULL;
  pm_err_flag_type          err_flag        = PM_ERR_FLAG__SUCCESS;
  uint8                     sdam_index      = 0;
  static DALSYSSyncHandle   sync_handle_copy;

  pm_target_data_type *target_ptr = pm_target_info_target_data();

  if (target_ptr == NULL)
  {
    PM_LOG_MSG_ERR_FATAL("Unable to find target_data");
  }

 if(comm_ptr == NULL || peripheral_info == NULL)
 {
   PM_LOG_MSG_ERR_FATAL("Invalid comm_ptr or peripheral info received");
 }

  sdam_ptr = pm_sdam_data_arr[pmic_index];

  sdam_index = (peripheral_info->base_address & (~sdam_reg.base_address)) >> 8;

  if(sdam_ptr == NULL)
  {
    pm_malloc( sizeof(pm_sdam_data_type), (void**)&(sdam_ptr));

    /* Assign Comm ptr */
    sdam_ptr->comm_ptr = comm_ptr;
  
    /* SDAM Register Info */
    sdam_ptr->sdam_reg_table = &sdam_reg;
  
    if(sdam_ptr->sdam_reg_table == NULL)
    {
      PM_LOG_MSG_ERR_FATAL("SDAM Reg Table not found");
    }
  
    sdam_ptr->num_of_periph = target_ptr->num_of_sdam[pmic_index];
  
    /* Num of peripherals cannot be 0 if this driver init gets called */
    if(sdam_ptr->num_of_periph == 0)
    {
      PM_LOG_MSG_ERR_FATAL("0 peripherals returned in config, %d", pmic_index);
    }
  
    /* Allocate memory to store sdam size */
    pm_malloc( (sizeof(uint32) * sdam_ptr->num_of_periph),
              (void**)&(sdam_ptr->sdam_mem_size) );
  
    /* Get Supported SDAMS on this EE */
    sdam_supported = target_ptr->sdam_supported_periph;
  
    if(sdam_supported == NULL)
    {
      PM_LOG_MSG_ERR_FATAL("Supported SDAMs not found in config");
    }
  
    sdam_ptr->sdam_supported_periph = &sdam_supported[pmic_index];
  
    if(sync_handle_copy == NULL)
    {
      if( DAL_SUCCESS !=
          DALSYS_SyncCreate(DALSYS_SYNC_ATTR_RESOURCE,
          &(sdam_ptr->sdam_dal_sync_handle), NULL) )
      {
        PM_LOG_MSG_ERR_FATAL("Unable to obtain mutex for sdam");
      }
  
      sync_handle_copy = sdam_ptr->sdam_dal_sync_handle;
    }
    else
    {
      sdam_ptr->sdam_dal_sync_handle = sync_handle_copy;
    }
  
    /* Get size of sdam memory */
    err_flag = pm_comm_read_byte(SLAVE_SDAM, REG_SDAM(sdam_size),
                            (uint8*)(&(sdam_ptr->sdam_mem_size[sdam_index])), 0);
  
    sdam_ptr->sdam_mem_size[sdam_index] *= SDAM_SIZE_MULTIPLIER;
  
    pm_sdam_data_arr[pmic_index] = sdam_ptr;
  }
  else
  {
    /* If already initialized for this pmic do not reinitialize but get the size
    * size of memory associated with peripheral and store the information
    */
    /* Get size of sdam memory */
    if(sdam_ptr->sdam_mem_size !=NULL && sdam_index <= sdam_ptr->num_of_periph)
    {
      err_flag = pm_comm_read_byte(SLAVE_SDAM, REG_SDAM(sdam_size),
                          (uint8*)(&(sdam_ptr->sdam_mem_size[sdam_index])), 0);
    }
    else
    {
      PM_LOG_MSG_ERR_FATAL("Sdam Index > Num of SDAMs or mem size ptr NULL");
    }
    sdam_ptr->sdam_mem_size[sdam_index] *= SDAM_SIZE_MULTIPLIER;
  }

  return;
}

pm_sdam_data_type*
pm_sdam_get_data(uint8 pmic_index)
{
  if(pmic_index < PM_MAX_NUM_PMICS)
  {
    return pm_sdam_data_arr[pmic_index];
  }
  return NULL;
}


uint8
pm_sdam_get_num_periph(uint8 pmic_index)
{
  if( (pmic_index < PM_MAX_NUM_PMICS) &&
      (pm_sdam_data_arr[pmic_index] != NULL))
  {
      return pm_sdam_data_arr[pmic_index]->num_of_periph;
  }

  return 0;
}

