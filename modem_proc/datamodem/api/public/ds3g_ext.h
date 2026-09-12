

#ifndef DS3G_EXT_H
#define DS3G_EXT_H
/*===========================================================================

                              D S 3 G   E X T H

                            H E A D E R   F I L E

DESCRIPTION
  This is the external header file for lower layers to use DS defined API and
  structures

  Copyright (c) 2009 - 2018 by Qualcomm Technologies, Incorporated.  All Rights Reserved.  
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/api/public/ds3g_ext.h#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/23/15   sa     Initial creation.

===========================================================================*/
#include "comdef.h"
#include "sys.h"
#include "cm_gw.h"
#include "ds_sys_ioctl.h"
#include "ps_iface_defs.h"

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/


/*===========================================================================

                      PUBLIC ENUM/STRUCT DECLARATIONS

===========================================================================*/

typedef enum
{
  DS3G_TX_POWER_BIN_1,                        /* x <= 0dBm */ 
  DS3G_TX_POWER_BIN_2,                        /* 0dBm < x <= 8dBm */ 
  DS3G_TX_POWER_BIN_3,                        /* 8dBm < x <= 15dBm */ 
  DS3G_TX_POWER_BIN_4,                        /* 15dBm < x <= 20dBm */ 
  DS3G_TX_POWER_BIN_5,                        /* x > 20dBm */ 
  DS3G_TX_POWER_BIN_MAX                       /*  MAX */ 
}ds3g_tx_power_bin_e_type;

/*===========================================================================
FUNCTION      DS3G_GET_APN_ROAMING_STATUS 

DESCRIPTION
  This API is an exposed API . This dispatches the function to check the
  roaming status and fills in the roaminf info pointer passed into it

PARAMETERS
  profile_id_3gpp     -  3GPP Profile number from AP
  profile_id_3gpp2    -  3GPP2 Profile number from AP
  roaming_status      -  pointer to the roaming status to be filled in
  subscription_id[in] -  Subscription id
  ps_errno [out]      -  Specifies type of error in case of error.
                             DS_EFAULT if parameter is invalid
                             DS_EINVAL if operation is invalid
                             DS_NOMEMORY if memory allocation fails

DEPENDENCIES
  None.

RETURN VALUE
  0  - Success
  -1 - Failure

SIDE EFFECTS
  None.
===========================================================================*/
int16 ds3g_get_apn_roaming_status
(
  uint8                           profile_id_3gpp,
  uint8                           profile_id_3gpp2,
  boolean                         *roaming_status,
  ds_sys_subscription_enum_type   subscription_id,
  int16                           *ps_errno
);

/*===========================================================================
FUNCTION DS3G_MCFG_CLEANUP

DESCRIPTION
   This API is called by MCFG framework before activating and deactivation MBN.
   This will perform some cleanup action based on the function calls made within

PARAMETERS
  as id : subscription id

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  none
===========================================================================*/
void ds3g_mcfg_cleanup
(
  sys_modem_as_id_e_type   as_id
);

/*===========================================================================
FUNCTION DS3G_MCFG_CLEANUP_EFS

DESCRIPTION
   This API is called by MCFG framework before activating MBN.
   This will perform some functional cleanup regardless data 
   task start or not.

PARAMETERS
  as id : subscription id

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  none
===========================================================================*/
void ds3g_mcfg_cleanup_efs
(
  sys_modem_as_id_e_type   as_id
);
/*===========================================================================
FUNCTION DS3GEX_CM_IS_RS_RA_PENDING

DESCRIPTION
   This API is called by CM to check if IPv6 RS/RA procedure is complete
   for Attach PDN

PARAMETERS
  subid : subscription id

DEPENDENCIES
  None

RETURN VALUE
  TRUE  - RS/RA pending
  FALSE - RS/RA not pending


SIDE EFFECTS
  none
===========================================================================*/
boolean ds3gex_cm_is_rs_ra_pending
(
  sys_modem_as_id_e_type subid
);

#endif /* DS3G_EXT_H */
