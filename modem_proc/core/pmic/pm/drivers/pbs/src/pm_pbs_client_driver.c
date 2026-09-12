/*! \file pm_pbs_client_driver.c
*  \n
*  \brief PBS Client driver initialization.
*  \n  
*  \n &copy; Copyright 2018 QUALCOMM Technologies Incorporated, All Rights Reserved
*/
/* =======================================================================
                            Edit History
This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/pmic/pm/drivers/pbs/src/pm_pbs_client_driver.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/13/13   kt      Initial version
========================================================================== */
/*===========================================================================

                     INCLUDE FILES

===========================================================================*/
#include "pm_pbs_client_driver.h"
#include "CoreVerify.h"
#include "hw_module_type.h"
#include "pm_malloc.h"
#include "device_info.h"
#include "pm_target_information.h"
#include "pm_version_p.h"
#include "pm_ulog.h"
/*===========================================================================

                        STATIC VARIABLES

===========================================================================*/

/* Static global variable to store the PBS data */
static pm_pbs_client_data_type *pm_pbs_client_data_arr[PM_MAX_NUM_PMICS];

static pm_pbs_client_register_info_type pbs_client_reg =
{
  .base_address       = 0x7100,
  .peripheral_offset  = 0x100,
  .trig_ctl           = 0x042,
  .en_ctl1            = 0x046,
  .scratch1           = 0x050,
  .scratch2           = 0x051,
};


/*===========================================================================

                        FUNCTION DEFINITIONS

===========================================================================*/
void pm_pbs_client_driver_init(pm_comm_info_type *comm_ptr, peripheral_info_type *peripheral_info, uint8 pmic_index)
{
    pm_pbs_client_data_type *pbs_client_ptr = NULL;
    static uint8 run_once = 0;
    static DALSYSSyncHandle sync_handle_copy;
    pm_pbs_supported_clients *pbs_supported_clients;
    pm_target_data_type *target_ptr = pm_target_info_target_data();

    if (target_ptr == NULL)
    {
      PM_LOG_MSG_ERR_FATAL("Unable to find target_data");
    }


    // Abort if the new PBS client driver is used on HW with old PBS client subtype
    if (peripheral_info->peripheral_subtype == PM_HW_MODULE_PBS_CLIENT)
    {
        CORE_VERIFY(0);
    }

    if ((peripheral_info->peripheral_subtype != PM_HW_MODULE_PBS_CLIENT_1TRIG) &&
        (peripheral_info->peripheral_subtype != PM_HW_MODULE_PBS_CLIENT_4TRIG))
    {
        return;
    }

    pbs_client_ptr = pm_pbs_client_data_arr[pmic_index];

    if (pbs_client_ptr == NULL)
    {
       pm_malloc( sizeof(pm_pbs_client_data_type), (void**)&pbs_client_ptr);

        /* Assign Comm ptr */
        pbs_client_ptr->comm_ptr = comm_ptr;

        /* PBS Register Info - Obtaining Data through dal config */
        pbs_client_ptr->pbs_client_reg_table = &pbs_client_reg;

        CORE_VERIFY_PTR(pbs_client_ptr->pbs_client_reg_table);

        pbs_client_ptr->num_of_peripherals = target_ptr->num_of_pbs_client[pmic_index];

        /* Num of peripherals cannot be 0 if this driver init gets called */
        CORE_VERIFY(pbs_client_ptr->num_of_peripherals != 0);

        //Get Supported SW PBS Clients
        pbs_supported_clients = target_ptr->pbs_supported_clients;
        pbs_client_ptr->pbs_supported_clients = &pbs_supported_clients[pmic_index];
        CORE_VERIFY_PTR(pbs_client_ptr->pbs_supported_clients);

        //This section is run once since only one DALSync Handle is needed for all PMIC's
        if(run_once==0)
        {
            pbs_client_ptr->pbs_dal_sync_handle = NULL;
            if(DAL_SUCCESS != DALSYS_SyncCreate(DALSYS_SYNC_ATTR_RESOURCE, &(pbs_client_ptr->pbs_dal_sync_handle), NULL))
                CORE_VERIFY(0);
            sync_handle_copy=pbs_client_ptr->pbs_dal_sync_handle;
            run_once=1;
        }
        else
        {
            pbs_client_ptr->pbs_dal_sync_handle=sync_handle_copy;
        }

        pm_pbs_client_data_arr[pmic_index] = pbs_client_ptr;
    }

}

pm_pbs_client_data_type* pm_pbs_client_get_data(uint8 pmic_index)
{
    if(pmic_index < PM_MAX_NUM_PMICS)
    {
        return pm_pbs_client_data_arr[pmic_index];
    }
    return NULL;
}


uint8 pm_pbs_client_get_num_peripherals(uint8 pmic_index)
{
  if( (pmic_index < PM_MAX_NUM_PMICS) &&
      (pm_pbs_client_data_arr[pmic_index] != NULL))
  {
      return pm_pbs_client_data_arr[pmic_index]->num_of_peripherals;
  }

  return NULL;
}

