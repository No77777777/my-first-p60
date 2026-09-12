#ifndef DS3G_GENERIC_CONFIG_H
#define DS3G_GENERIC_CONFIG_H
/*===========================================================================

                3 G   D A T A   S E R V I C E S   E V E N T   M A N A G E R

                           H E A D E R   F I L E

GENERAL DESCRIPTION
  This software unit contains functions for 3G Data Services event handling.
  It provides APIs for DS3G event registration and notification.

  Copyright (c) 2001-2015 by Qualcomm Technologies Incorporated. All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $PVCSPath:
  $Header:

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
09/23/16   js      Initial version.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/


#include "datamodem_variation.h"
#include "comdef.h"
#include "ds3gsubsmgr.h"
#include "sys_v.h"
#include "mcfg_fs.h"
#include "ds3g_pco_mgr.h"
#include "ds_3gpp_config_mgr.h"


/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

=============================================================================*/

#define DS3G_GENERIC_CONFIG_EFS_TAG       "CarrierConfiguration"
#define DS3G_GENERIC_CONFIG_NODE_TAG      "Node"
#define DS3G_GENERIC_CONFIG_APN_TAG       "APN"
#define DS3G_GENERIC_CONFIG_PCO_LIST_TAG  "PcoList"
#define DS3G_GENERIC_CONFIG_INT_PCO_TAG   "InternalPco"
#define DS3G_GENERIC_CONFIG_EXT_PCO_TAG   "ExternalPco"
#define DS3G_GENERIC_CONFIG_PCO_TAG       "PCO"
#define DS3G_GENERIC_CONFIG_PCO_LIST_NUM   2

#define DS3G_GENERIC_CONFIG_ROOT_DIR      "/data/3gpp"
#define DS3G_GENERIC_CONFIG_FILE          "/data/3gpp/ds3g_carrier_config.xml"

#define DS3G_GENERIC_CONFIG_PCO_CLIENT_INT   0x01
#define DS3G_GENERIC_CONFIG_PCO_CLIENT_EXT   0x02


/*===========================================================================
FUNCTION      DS3G_GENERIC_CONFIG_POWERUP_INIT

DESCRIPTION   This function performs Powerup Initialization of the Module
  
PARAMETERS    None 
    
DEPENDENCIES  None
  
RETURN VALUE  None
   
SIDE EFFECTS  None
===========================================================================*/
void ds3g_generic_config_powerup_init(void);

/*===========================================================================
FUNCTION      DS3G_GENERIC_CONFIG_POWERUP_INIT_PER_SUBS_ID

DESCRIPTION   This function performs initialization of the generic 
              configuration per subscription

PARAMETERS    sys_modem_as_id_e_type     subs_id  
    
DEPENDENCIES  None
  
RETURN VALUE  None
   
SIDE EFFECTS  None
===========================================================================*/
void ds3g_generic_config_powerup_init_per_subs_id
(
  sys_modem_as_id_e_type     subs_id 
);

/*===========================================================================
FUNCTION      DS3G_GENERIC_CONFIG_CHECK_PCO_CLIENT_TYPE

DESCRIPTION   This function checks whether the current pco should be sent to 
              internal/external clients 

PARAMETERS    uint16                        protocol_id,
              char                         *apn_name,
              ds3gsubsmgr_subs_id_e_type    subs_index  
    
DEPENDENCIES  None
  
RETURN VALUE  uint8
   
SIDE EFFECTS  None
===========================================================================*/
uint8 ds3g_generic_config_check_pco_client_type
(
  uint16                                    protocol_id,
  char                                     *apn_name,
  ds3gsubsmgr_subs_id_e_type                subs_index 
);

#endif /* DS3G_GENERIC_CONFIG_H */
