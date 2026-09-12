/*! \file pm_pbs_client_app.c
*  \n
*  \brief BCL Comparator Application Layer.
*  \n  
*  \n &copy; Copyright 2013 QUALCOMM Technologies Incorporated, All Rights Reserved
*/

/* =======================================================================
                            Edit History
This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/pmic/pm/application/common/src/pm_common_app.c#1 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
4/14/17   RL      File created
========================================================================== */
/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include "pmapp_common.h"
#include "pm_pbs_client.h"
#include "pm_ulog.h"
#include "pm_target_information.h"
#include "pm_pbs_client_driver.h"
#include "pm_version_p.h"
#include "pm_config_target.h"
#include "pm_ulog.h"

/*===========================================================================

                     DEFINITIONS 

===========================================================================*/

extern boolean bPMICStub;
                                                  
/*===========================================================================

                     FUNCTION DEFINITIONS

===========================================================================*/                                     
                                                  
pm_err_flag_type 
pmapp_fg_esr_qmode ( boolean entry_exit )                                     
{
  pm_err_flag_type              err_flag    = PM_ERR_FLAG_SUCCESS;
  pm_pbs_client_trig_cfg_type   *trig_cfg   = NULL;
  uint8                         trig_retval = 0;
  uint8                         trig_arg    = 0;
  pm_model_type                 pmic_model  = PMIC_IS_UNKNOWN;
  pm_target_data_type           *target_ptr = pm_target_info_target_data();

  if (target_ptr == NULL)
  {
    PM_LOG_MSG_ERR_FATAL("Unable to find target_data");
  }

  if(bPMICStub == TRUE)
  {
    return PM_ERR_FLAG__SUCCESS;
  }
    
  trig_cfg = target_ptr->pbs_esr_trig_cfg;
   
  if(trig_cfg == NULL || trig_cfg->supported == FALSE)
  {
    err_flag = PM_ERR_FLAG__FEATURE_NOT_SUPPORTED; 
    
    PM_LOG_MSG_ERROR(err_flag, "Feature not supported on this target");

    return err_flag;
  }
  
  pmic_model = pm_get_pmic_model(trig_cfg->pmic_chip);
  
  if(pmic_model == PMIC_IS_UNKNOWN || pmic_model == PMIC_IS_INVALID)
  {
    /* If pmic not available for example pmi not available in SS configuration 
     * Return success
     */
    PM_LOG_MSG_INFO("PMIC Not found, returning success");
    return PM_ERR_FLAG__SUCCESS;
  }

  
  trig_arg = (entry_exit ? trig_cfg->entry_arg : trig_cfg->exit_arg);

  
  err_flag = pm_pbs_client_sw_trigger(trig_cfg->pmic_chip, 
                                      trig_cfg->pbs_client,
                                      trig_cfg->trigger, 
                                      &trig_arg, 
                                      &trig_retval, 
                                      trig_cfg->trig_type);

  if(trig_retval != trig_arg)
  {
    /* Something went hay-wire in the way sequences are triggered, 
    *  Log Error
    */
    err_flag = PM_ERR_FLAG__DATA_VERIFY_FAILURE;
    PM_LOG_MSG_ERROR(err_flag, "Trigger retval %d, trigger argument %d", 
                     trig_retval, trig_arg);
  }
  
  PM_LOG_MSG_ERROR(err_flag, "PBS Trigger Failed !!!");
  
  PM_LOG_MSG_INFO("Completed entry/exit req = %d", entry_exit);
  
  return err_flag;
}
