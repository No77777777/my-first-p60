/*===========================================================================
                      DS_KEEPALIVE_ASSIST_CONFIG.H

DESCRIPTION
 Config header file for keepalive assist.

EXTERNALIZED FUNCTIONS

 Copyright (c) 2017 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================
                       EDIT HISTORY FOR FILE
  $Header: //components/rel/data.mpss/3.5..2.1/appsrv/aps/inc/ds_keepalive_assist_config.h#2 $
  $Author: pwbldsvc $  $DateTime: 2019/10/22 02:53:17 $
  
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
05/01/17   Youjunf First version of file
===========================================================================*/

#ifndef DS_KEEPALIVE_ASSIST_CONFIG_H
#define DS_KEEPALIVE_ASSIST_CONFIG_H

/*===========================================================================
                   INCLUDE FILES
===========================================================================*/
#include "datamodem_variation.h"

/*===========================================================================
                   EXTERNAL FUNCTIONS
===========================================================================*/

/*===========================================================================
FUNCTION      DS_KEEPALIVE_ASSIST_CFG_INIT

DESCRIPTION   This function initializes modem assist keep-alive configuration module 

PARAMETERS    None

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void   ds_keepalive_assist_cfg_init(void);



/*===========================================================================
FUNCTION      DS_KEEPALIVE_ASSIST_CFG_READ_FROM_EFS

DESCRIPTION   This function read efs and cache params 

PARAMETERS    None

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds_keepalive_assist_cfg_read_from_efs(uint8 subs_idx);

/*===========================================================================
FUNCTION      DS_KEEPALIVE_ASSIST_CFG_GET_PDN_MAX_RETRY

DESCRIPTION   This function get pdn_max_retry 

PARAMETERS    None

DEPENDENCIES  None

RETURN VALUE  pdn_max_retry

SIDE EFFECTS  None
===========================================================================*/
uint16 ds_keepalive_assist_cfg_get_pdn_max_retry(uint8 subs_idx);


/*===========================================================================
FUNCTION      DS_KEEPALIVE_ASSIST_CFG_GET_PDN_RETRY_INTERVAL

DESCRIPTION   This function get pdn_retry_interval

PARAMETERS    None

DEPENDENCIES  None

RETURN VALUE  pdn_retry_interval

SIDE EFFECTS  None
===========================================================================*/
uint32 ds_keepalive_assist_cfg_get_pdn_retry_interval(uint8 subs_idx);  


#endif /* DS_KEEPALIVE_ASSIST_CONFIG_H */
