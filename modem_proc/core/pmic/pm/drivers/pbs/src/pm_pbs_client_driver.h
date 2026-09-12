#ifndef __PM_PBS_CLIENT_DRIVER_H__
#define __PM_PBS_CLIENT_DRIVER_H__

/*! \file pm_pbs_client_driver.h 
*  \n 
*  \brief PBS Client driver type definitions and init function prototype.   
*  \n  
*  \n &copy; Copyright 2013 QUALCOMM Technologies Incorporated, All Rights Reserved
*/
/* =======================================================================
                                Edit History
This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/pmic/pm/drivers/pbs/src/pm_pbs_client_driver.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/13/13   kt      Initial version
========================================================================== */
/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include "pm_err_flags.h"
#include "pm_target_information.h"
#include "DALSys.h"
#include "pm_pbs_client.h"
#include "pm_config_target.h"

/*===========================================================================

                        TYPE DEFINITIONS 

===========================================================================*/
typedef struct
{
    pm_register_address_type base_address;   
    pm_register_address_type peripheral_offset;
    pm_register_address_type trig_ctl;
    pm_register_address_type en_ctl1;
    pm_register_address_type scratch1;
    pm_register_address_type scratch2;
}pm_pbs_client_register_info_type;

typedef struct
{
    pm_comm_info_type                 *comm_ptr;
    pm_pbs_client_register_info_type*  pbs_client_reg_table;
    uint8                              num_of_peripherals;
    DALSYSSyncHandle                  pbs_dal_sync_handle;
    pm_pbs_supported_clients          *pbs_supported_clients;
}pm_pbs_client_data_type;

/*===========================================================================

                        FUNCTION PROTOTYPES

===========================================================================*/
void pm_pbs_client_driver_init(pm_comm_info_type *comm_ptr, peripheral_info_type *peripheral_info, uint8 pmic_index);

pm_pbs_client_data_type* pm_pbs_client_get_data(uint8 pmic_index);

#endif /* __PM_PBS_CLIENT_DRIVER_H__ */

