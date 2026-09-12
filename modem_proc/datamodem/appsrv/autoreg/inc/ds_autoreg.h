#ifndef DS_AUTOREG_H
#define DS_AUTOREG_H
/*===========================================================================
 
                           DS_AUTOREG.H

DESCRIPTION
  Header file of DS auto registration module.
 
Copyright (c) 2016 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/appsrv/autoreg/inc/ds_autoreg.h#3 $
  $DateTime: 2020/03/05 22:04:26 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/09/16   hr      Initial version
===========================================================================*/

/*===========================================================================
                     INCLUDE FILES FOR MODULE
===========================================================================*/
#include "datamodem_variation.h"

#include "cm.h"
#include "ds_appsrv_utils.h"
/*===========================================================================
                      EXTERNAL FUNCTIONS DEFINITIONS
===========================================================================*/

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/*===========================================================================
FUNCTION      DS_AUTOREG_INIT

DESCRIPTION   Initializes the DS auto registration module
 
PARAMETERS    None
 
DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds_autoreg_init
(
  void
);

/*===========================================================================
FUNCTION      ds_autoreg_ps_ioctl_set_os_version_info

DESCRIPTION   This function handles setting OSVER IOCTL from PS
 
DEPENDENCIES  None

RETURN VALUE  0 - Success
              -1- Failure

SIDE EFFECTS  None
===========================================================================*/
int ds_autoreg_ps_ioctl_set_os_version_info
(
  void                    *arg_val_ptr,
  int16                   *ps_errno
);

/*===========================================================================
FUNCTION      ds_autoreg_ps_ioctl_set_sw_version_info

DESCRIPTION   This function handles setting SWVER IOCTL from PS
 
DEPENDENCIES  None

RETURN VALUE  0 - Success
              -1- Failure

SIDE EFFECTS  None
===========================================================================*/
int ds_autoreg_ps_ioctl_set_sw_version_info
(
  void                    *arg_val_ptr,
  int16                   *ps_errno
);

/*===========================================================================
FUNCTION      ds_autoreg_ps_ioctl_set_wlan_macid_info

DESCRIPTION   This function handles setting MACID IOCTL from PS
 
DEPENDENCIES  None

RETURN VALUE  0 - Success
              -1- Failure

SIDE EFFECTS  None
===========================================================================*/
int ds_autoreg_ps_ioctl_set_wlan_macid_info
(
  void                    *arg_val_ptr,
  int16                   *ps_errno
);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/*===========================================================================
FUNCTION      DS_AUTOREG_ICCID_INFO_EV_HDLR

DESCRIPTION   Handles ICCID event info
 
PARAMETERS    iccid_info_ptr: ICCID event info
 
DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds_autoreg_iccid_info_ev_hdlr
(
  ds_appsrv_iccid_info_s_type *iccid_info_ptr
);

/*===========================================================================
FUNCTION      DS_AUTOREG_NOTIFY_SS_EV_INFO

DESCRIPTION   Notifies DS AUTOREG module about ss event info
 
PARAMETERS    ss_info_ptr: CM SS event info
 
DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds_autoreg_notify_ss_ev_info
(
  const cm_mm_msim_ss_info_s_type *ss_info_ptr
);

#endif /* DS_AUTOREG_H */
