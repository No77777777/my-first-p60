#ifndef __PM_CONFIG_TARGET_H__
#define __PM_CONFIG_TARGET_H__
/*===========================================================================
DESCRIPTION
This file contains prototype definitions prm device layer

Copyright (c) 2017 QUALCOMM Technologies, Inc. (QTI). All Rights Reserved.  
QUALCOMM Proprietary.  Export of this technology or software is regulated 
by the U.S. Government. Diversion contrary to U.S. law prohibited.
===========================================================================*/

/*===========================================================================

EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/settings/pmic/pm/interface/pm_config_target.h#1 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
01/10/17   rl     Created
===========================================================================*/

/*===========================================================================

INCLUDE FILES

===========================================================================*/

#include "comdef.h"
#include "pm_npa_device.h"

/*===========================================================================

TYPE DEFINITIONS

===========================================================================*/
/** 
  @enum pm_pbs_client_type
  @brief PBS Client types
 */
typedef enum 
{
  PM_PBS_CLIENT_0  = 0,
  PM_PBS_CLIENT_1  = 1,
  PM_PBS_CLIENT_2  = 2,
  PM_PBS_CLIENT_3  = 3,
  PM_PBS_CLIENT_INVALID
}pm_pbs_client_type;

/** 
  @enum pm_pbs_client_trigger
  @brief sw/hw triggers in each PBS client
 */
typedef enum 
{
  PM_PBS_CLIENT_HW_TRIGGER_0 = 0,
  PM_PBS_CLIENT_HW_TRIGGER_1 = 1,
  PM_PBS_CLIENT_HW_TRIGGER_2 = 2,
  PM_PBS_CLIENT_HW_TRIGGER_3 = 3,
  PM_PBS_CLIENT_SW_TRIGGER,
  PM_PBS_CLIENT_TRIGGER_INVALID
}pm_pbs_client_trigger;

typedef enum
{
  PM_PBS_CLIENT_FNF,
  PM_PBS_CLIENT_WAIT,
  PM_PBS_CLIENT_TYPE_INVALID,
}pm_pbs_client_trigger_type;

/** 
  @enum pm_sdam_type
  @brief SDAM Periph types
 */
typedef enum 
{
  PM_SDAM_1,
  PM_SDAM_2,
  PM_SDAM_3,
  PM_SDAM_4,
  PM_SDAM_5,
  PM_SDAM_6,
  PM_SDAM_7,
  PM_SDAM_8,
  PM_SDAM_INVALID
}pm_sdam_type;

typedef struct
{ 
  pm_pbs_client_type client_type;
}pm_pbs_supported_clients;

typedef struct
{ 
  pm_sdam_type sdam_periph;
}pm_sdam_supported_periph;


typedef struct pm_bcl_comp_limits
{
  uint32 min_mv;
  uint32 max_mv;
  uint32 step_mv; 
}pm_bcl_comp_limits;

typedef struct
{
  uint8 uicc_gpio;
  uint8 pbs_client;
  uint8 uicc1_ldo;
  uint8 uicc2_ldo;
  uint8 uicc3_ldo;
  uint8 uicc4_ldo;
} pm_uicc_specific_info_type;

typedef struct pmapp_bcl_cfg_info_type
{
  uint8  pmic_chip;
}pmapp_bcl_cfg_info_type;

typedef struct
{
  boolean                     supported;
  uint8                       pmic_chip; 
  pm_pbs_client_type          pbs_client;
  pm_pbs_client_trigger       trigger;
  pm_pbs_client_trigger_type  trig_type;
  uint8                       entry_arg; 
  uint8                       exit_arg;
}pm_pbs_client_trig_cfg_type;

typedef struct
{
  uint32                         *num_of_ldo;
  uint32                         *num_of_smps;
  uint32                         *num_of_gpio;
  uint32                         *num_of_mpp;
  uint32                         *num_of_rtc;
  uint32                         *num_of_talm;
  uint32                         *num_of_xo;
  uint32                         *num_of_boost;
  uint32                         *num_of_vs;
  uint32                         *num_of_pbs_client;
  uint32                         *num_of_sdam;
  uint32                         *num_of_bob;
  pm_pbs_supported_clients       *pbs_supported_clients;
  pm_sdam_supported_periph       *sdam_supported_periph;
  pmapp_bcl_cfg_info_type        *pmapp_bcl_info;
  pm_pbs_client_trig_cfg_type    *pbs_esr_trig_cfg;
  pm_pwr_resource_info_type      *mss_rail;
  uint32                         *step_uV_per_uS;
  pm_uicc_specific_info_type     *uicc_specific;
}pm_target_data_type;

#endif /* __PM_CONFIG_TARGET_H__ */


