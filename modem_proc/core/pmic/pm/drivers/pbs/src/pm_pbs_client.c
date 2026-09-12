/*! \file pm_pbs_client.c
*  \n
*  \brief This file contains the implementation of the public APIs for PBS Client module.
*  \n
*  \details Implementation file for PBS Client module. Each of the APIs checks for
*           access and then if necessary directs the call to Driver implementation.
*  \n  
*  &copy; Copyright 2013 QUALCOMM Technologies Incorporated, All Rights Reserved
*/
/*===========================================================================
EDIT HISTORY FOR MODULE

This document is created by a code generator, therefore this section will
not contain comments describing changes made to the module.

$Header: //components/rel/core.mpss/10.0/pmic/pm/drivers/pbs/src/pm_pbs_client.c#1 $ 

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
06/16/13   kt      Created.
===========================================================================*/
/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include "pm_pbs_client_driver.h"
#include "pm_pbs_client.h"
#include "CoreVerify.h"
#include "DALSys.h"
#include "pm_ulog.h"

/*===========================================================================

                         DEFINITIONS

===========================================================================*/
//Max wait time in uS 
#define SEQ_MAX_WAIT_TIME 500

//Busy Wait Poll time in uS 
#define SEQ_BUSYWAIT_POLL_TIME 5

#define BUSY_INDICATOR 0xFF

#define PM_PBS_MUTEX_LOCK(handle, trigger)       \
   if ( trigger == PM_PBS_CLIENT_SW_TRIGGER)     \
        DALSYS_SyncEnter(handle)

#define PM_PBS_MUTEX_UNLOCK(handle, trigger)     \
   if ( trigger == PM_PBS_CLIENT_SW_TRIGGER)     \
        DALSYS_SyncLeave(handle)

#define REG_PBS(reg)                                                                 \
    ((pbs_client_ptr->pbs_client_reg_table->base_address)                            \
    + ((pbs_client_index)*(pbs_client_ptr->pbs_client_reg_table->peripheral_offset)) \
    +  (pbs_client_ptr->pbs_client_reg_table->reg))

#define SLAVE_ID_PBS pbs_client_ptr->comm_ptr->slave_id

#define PBS_CLIENT_SW_TRIG_MASK 0x80

/*===========================================================================

                        FUNCTION PROTOTYPES

===========================================================================*/   

static pm_err_flag_type pm_pbs_client_sw_seq_done(
                                       pm_pbs_client_data_type *pbs_client_ptr, 
                                       pm_register_address_type addr);

/*===========================================================================

                        FUNCTION DEFINITIONS

===========================================================================*/

pm_err_flag_type pm_pbs_client_sw_trigger(uint8 pmic_chip, 
                                          pm_pbs_client_type pbs_client_index, 
                                          pm_pbs_client_trigger trigger, 
                                          uint8 *trigger_argument, 
                                          uint8 *trigger_return_value,  
                                          pm_pbs_client_trigger_type trigger_type)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG__SUCCESS;
    pm_pbs_client_data_type *pbs_client_ptr = pm_pbs_client_get_data(pmic_chip);
    uint8 trigger_mask = 1<<trigger; //for hw triggers
    
    //Input Error Checks 
    if ((pbs_client_ptr == NULL) || 
        (pbs_client_index >= pbs_client_ptr->num_of_peripherals) || 
        (pbs_client_index >= PM_PBS_CLIENT_INVALID) || 
        (trigger >= PM_PBS_CLIENT_TRIGGER_INVALID) || 
        (trigger_type >= PM_PBS_CLIENT_TYPE_INVALID) ||
        (pbs_client_ptr->pbs_supported_clients->client_type != pbs_client_index) ||
        ((trigger==PM_PBS_CLIENT_SW_TRIGGER) && (trigger_argument == NULL)) ||
        ((trigger !=PM_PBS_CLIENT_SW_TRIGGER) && (trigger_type != PM_PBS_CLIENT_FNF)))
    {   
        err_flag = (pbs_client_ptr==NULL) ? PM_ERR_FLAG__FEATURE_NOT_SUPPORTED : 
                   PM_ERR_FLAG__INPUT_INVALID;
        PM_LOG_MSG_ERROR(err_flag, "error = %d", err_flag);
        return err_flag;
    }
    
    //Enter Sync(Mutex Lock) if sw trigger
    PM_PBS_MUTEX_LOCK(pbs_client_ptr->pbs_dal_sync_handle, trigger);
    
    if (trigger == PM_PBS_CLIENT_SW_TRIGGER)
    {
        err_flag |=pm_pbs_client_sw_seq_done(pbs_client_ptr, REG_PBS(scratch2));
        // Release lock and return if seq was not completed in 500uS
        if(err_flag != PM_ERR_FLAG__SUCCESS)
        {
            PM_PBS_MUTEX_UNLOCK(pbs_client_ptr->pbs_dal_sync_handle, trigger);
            PM_LOG_MSG_ERROR(err_flag, "error = %d", err_flag);
            return err_flag;
        }
        // If previous seq completed
        // Write to SCRATCH1 register if any argument 
        // value is to be used with the PBS sequence
        err_flag |= pm_comm_write_byte(SLAVE_ID_PBS, REG_PBS(scratch1), 
                                       *trigger_argument, 0);
        err_flag |= pm_comm_write_byte(SLAVE_ID_PBS, REG_PBS(scratch2), 
                                       (uint8)BUSY_INDICATOR, 0);
        
        //Change trigger_mask for sw triggers 
        trigger_mask = PBS_CLIENT_SW_TRIG_MASK;    
    }
    
    // Trigger the PBS sequence
    err_flag |= pm_comm_write_byte(SLAVE_ID_PBS, REG_PBS(trig_ctl), 
                                   trigger_mask, 0);
    
    
    if(trigger_type != PM_PBS_CLIENT_FNF)
    {
        err_flag |=pm_pbs_client_sw_seq_done(pbs_client_ptr, REG_PBS(scratch2));

        if (trigger_return_value != NULL)
        {
        // Read from SCRATCH2 register 
		//if any return value expected from the PBS sequence
        err_flag |= pm_comm_read_byte(SLAVE_ID_PBS, REG_PBS(scratch2), 
                                      trigger_return_value, 0);
        }
    }

    //Exit Sync (Mutex Unlock)
    PM_PBS_MUTEX_UNLOCK(pbs_client_ptr->pbs_dal_sync_handle, trigger);
    
    PM_LOG_MSG_ERROR(err_flag, "error = %d", err_flag);
    
    return err_flag;
}

pm_err_flag_type pm_pbs_client_sw_seq_done(pm_pbs_client_data_type *pbs_client_ptr, pm_register_address_type addr)
{
    uint8 scratch_data=0;   
    uint32 wait_time = 0; 
    pm_err_flag_type err_flag = PM_ERR_FLAG__SUCCESS;
    do
    {   //Read Scratch1 register 
        err_flag |= pm_comm_read_byte(SLAVE_ID_PBS, addr, &scratch_data, 0);
        
        if(scratch_data == BUSY_INDICATOR)
        {   
            //Wait for completion of PBS sequence in progress
            DALSYS_BusyWait(SEQ_BUSYWAIT_POLL_TIME);
            if ((wait_time += SEQ_BUSYWAIT_POLL_TIME) >=SEQ_MAX_WAIT_TIME) 
            {   
                //Return error flag if sequence is not competed in max time
                err_flag = PM_ERR_FLAG__DATA_VERIFY_FAILURE;
                break; 
            }
        }
    }while (scratch_data==BUSY_INDICATOR);
	
    return err_flag;
}

