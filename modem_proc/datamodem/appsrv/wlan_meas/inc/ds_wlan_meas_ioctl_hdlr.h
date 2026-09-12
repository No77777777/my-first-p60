#ifndef DS_WLAN_MEAS_IOCTL_HDLR_H
#define DS_WLAN_MEAS_IOCTL_HDLR_H

/*===========================================================================

                       DS_WLAN_MEAS_IOCTL_HDLR.H
DESCRIPTION

   header of wlan meas ioctls

EXTERNALIZED FUNCTIONS

 Copyright (c) 2015 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================
                       EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  when        who                            what, where, why
--------    -------                ----------------------------------------
05/18/2015    Youjunf                          First version of file
===========================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"

#if defined(FEATURE_ELS_WIFIMEAS_LTE_U_PH1) || defined(FEATURE_ELS_LWA) || defined(FEATURE_DATA_ANDSF)

/*===========================================================================
            EXTERNAL FUNCTION DECLARATIONS
===========================================================================*/

/*===================================================================================
FUNCTION DS_WLAN_MEAS_PS_IOCTL_CONFIG_STATUS

DESCRIPTION
  This function handles wifi meas config status IOCTL from PS

PARAMETERS
  arg_val_ptr[in]- carrys required information
  ps_errno   [in]   - type of error

DEPENDENCIES
  None.

RETURN VALUE
 0 - Success
 -1- Failure

SIDE EFFECTS
  None
========================================================================================*/
int ds_wlan_meas_ps_ioctl_meas_config_status(void *arg_val_ptr,  int16 *ps_errno);


/*=======================================================================================
FUNCTION DS_WLAN_MEAS_PS_IOCTL_MEAS_REPORT

DESCRIPTION
  This function handles wifi meas report IOCTL from PS

PARAMETERS
  arg_val_ptr[in]- carrys required information
  ps_errno   [in]   - type of error

DEPENDENCIES
  None.

RETURN VALUE
 0 - Success
 -1- Failure

SIDE EFFECTS
  None
==========================================================================================*/
int ds_wlan_meas_ps_ioctl_meas_report(void *arg_val_ptr,  int16 *ps_errno);


#endif /* FEATURE_ELS_WIFIMEAS_LTE_U_PH1 || FEATURE_ELS_LWA || FEATURE_DATA_ANDSF*/


int ds_wlan_meas_ps_ioctl_wifi_band_vacate_resp(void *arg_val_ptr,  int16 *ps_errno);

#endif /* DS_WLAN_MEAS_IOCTL_HDLR_H */





