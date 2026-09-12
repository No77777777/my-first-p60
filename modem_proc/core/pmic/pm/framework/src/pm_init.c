/*! \file   pm_proc_app.c
*
*  \brief   This file contains PMIC Modem initialization
*  \details PMIC Modem Proc initialization to initialize driver resources
*
*  &copy; Copyright 2018 QUALCOMM Technologies Incorporated, All Rights Reserved
*/

/*===========================================================================

EDIT HISTORY FOR MODULE

This document is created by a code generator, therefore this section will
not contain comments describing changes made to the module.

$Header: //components/rel/core.mpss/10.0/pmic/pm/framework/src/pm_init.c#4 $ 

when        who     what, where, why
--------    ---     ----------------------------------------------------------
01/07/14    aks     Add multiple PMIC support >2
07/12/12    umr     Add support for BUA.
07/12/12    vk      Satisfy COREBSP Dependency to pm_init() and deploy in RCINIT
                    Framework

===========================================================================*/

#include "DDIPlatformInfo.h"
#include "pm_ulog.h"
#include "pm_bcl_comp_app.h"
#include "pm_comm.h"
#include "pm_target_information.h"
#include "pm_resource_manager.h"
#include "device_info.h"
#include "pm_modem_proc_npa.h"
#include "pm_rtc_application.h"
#include "pm_comm_qdi.h"

#ifdef FEATURE_PMIC_DIAG_SERVICES
#include "pm_diag.h"
#include "pmapp_diag.h"
#endif

/* PMIC Application includes */
#include "pm_rtc_application.h"
#include "pm_uicc_app.h"
#include "pm_target.h"

#define PM_PRIMARY_PMIC_INDEX  0

boolean bPMICStub = FALSE;

void pm_init(void)
{
    pm_model_type pm_model = PMIC_IS_UNKNOWN;

    static boolean call_once = FALSE;

    if(FALSE != call_once)
         return;

    call_once = TRUE;

    pm_ulog_init();

    pm_comm_channel_init_internal();
    pm_version_detect();
  
    //enable stub based on pmic detection
    pm_model = pm_get_pmic_model(PM_PRIMARY_PMIC_INDEX);
    if ((PMIC_IS_UNKNOWN == pm_model) || (PMIC_IS_INVALID == pm_model))
    {
        bPMICStub = TRUE;
    }
    else
    {
        bPMICStub = FALSE;
    }
    
    pm_target_info_init();
	
	pm_target_pre_init();

    pm_comm_info_init();
    pm_comm_qdi_init();

    /*
    * Stub out driver initialization in applicable environments.
    */
    if (bPMICStub == FALSE)
    {
        pm_resource_manager_init();
        
        pm_rtc_app_init();
        
        #ifdef FEATURE_PMIC_DIAG_SERVICES
            pm_app_diag_init();
            pm_diag_init();
        #endif

    }
    
    pm_modem_proc_npa_init ();
    
    pm_comm_post_init();
    
    pmapp_bcl_comp_init();

}
