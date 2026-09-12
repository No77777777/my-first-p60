/*! \file
 *  
 *  \brief  rpm_settings.c ----This file contains customizable target specific driver settings & PMIC registers.
 *  \details This file contains customizable target specific 
 * driver settings & PMIC registers. This file is generated from database functional
 * configuration information that is maintained for each of the targets.
 *  
 *    PMIC code generation Version: 1.0.0.0
 *    PMIC code generation Resource Setting Information Version: VU.Please Provide Valid Label - Not Approved
 *    PMIC code generation Software Register Information Version: VU.Please Provide Valid Label - Not Approved
 *    PMIC code generation Processor Allocation Information Version: VU.Please Provide Valid Label - Not Approved
 *    This file contains code for Target specific settings and modes.
 *  
 *  &copy; Copyright 2010 Qualcomm Technologies Incorporated, All Rights Reserved
 */

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This document is created by a code generator, therefore this section will
  not contain comments describing changes made to the module.

$Header: //components/rel/core.mpss/10.0/settings/pmic/pm/config/kamorta/pm_config_target_agatti.c#1 $ 

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES

===========================================================================*/

#include "pm_config_target.h"
#include "pm_gpio.h"
#include "pm_ldo.h"

//Cocos(PM2250)
static uint32   num_of_ldo[]  =      {22};
static uint32   num_of_smps[] =      {04};
static uint32   num_of_gpio[] =      {10};
static uint32   num_of_mpp[]  =      {00};
static uint32   num_of_rtc[] =       {01};
static uint32   num_of_talm[] =      {01};
static uint32   num_of_megaxo[] =    {01};
static uint32   num_of_boost[]  =    {00};
static uint32   num_of_vs[] =        {00};
static uint32   num_of_pbs_client[] ={02};
static uint32   num_of_sdam[]       ={07};
static uint32   num_of_bob[]        ={00};



//Populate PBS clients corresponding to modem For each pmic
static pm_pbs_supported_clients 
pbs_supported_clients[7] =
{
  {PM_PBS_CLIENT_INVALID},
  {PM_PBS_CLIENT_INVALID},
  {PM_PBS_CLIENT_INVALID},
  {PM_PBS_CLIENT_INVALID},
  {PM_PBS_CLIENT_INVALID},
  {PM_PBS_CLIENT_INVALID},
  {PM_PBS_CLIENT_INVALID},
};

//Populate supported SDAM corressponding to pmic
static pm_sdam_supported_periph sdam_supported_periph[7] =
{
  {PM_SDAM_4},
  {PM_SDAM_INVALID},
  {PM_SDAM_INVALID},
  {PM_SDAM_INVALID},
  {PM_SDAM_INVALID},
  {PM_SDAM_INVALID},
  {PM_SDAM_INVALID},
};


static pmapp_bcl_cfg_info_type pmapp_bcl_info =
{ //pmic_chip
  0
};

//Trigger config for external client GPS, ESR Sequence
static pm_pbs_client_trig_cfg_type pbs_esr_trig_cfg =
{
  .supported   = FALSE,
  .pmic_chip   = 0,
  .pbs_client  = PM_PBS_CLIENT_INVALID,
  .trigger     = PM_PBS_CLIENT_TRIGGER_INVALID,
  .trig_type   = PM_PBS_CLIENT_TYPE_INVALID,
  .entry_arg   = 0,
  .exit_arg    = 0,
};

pm_target_data_type 
pm_target_data_agatti = 
{
  .num_of_ldo                   = num_of_ldo,
  .num_of_smps                  = num_of_smps,
  .num_of_gpio                  = num_of_gpio,
  .num_of_mpp                   = num_of_mpp,
  .num_of_rtc                   = num_of_rtc,
  .num_of_talm                  = num_of_talm,
  .num_of_xo                    = num_of_megaxo,
  .num_of_boost                 = num_of_boost,
  .num_of_vs                    = num_of_vs,
  .num_of_pbs_client            = num_of_pbs_client,
  .num_of_sdam                  = num_of_sdam,
  .num_of_bob                   = num_of_bob,
  .pbs_supported_clients        = pbs_supported_clients,
  .sdam_supported_periph        = sdam_supported_periph,
  .pmapp_bcl_info               = &pmapp_bcl_info,
  .pbs_esr_trig_cfg             = &pbs_esr_trig_cfg,
};

