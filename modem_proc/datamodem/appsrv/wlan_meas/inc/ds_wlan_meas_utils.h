#ifndef DS_WLAN_MEAS_UTILS_H
#define DS_WLAN_MEAS_UTILS_H


/*===========================================================================

                        DS_WLAN_MEAS_UTILS.H

DESCRIPTION

   header of wlan meas utility

EXTERNALIZED FUNCTIONS

 Copyright (c) 2015 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================
                       EDIT HISTORY FOR FILE
  $Header: //components/rel/data.mpss/3.5..2.1/appsrv/wlan_meas/inc/ds_wlan_meas_utils.h#2 $
  $Author: pwbldsvc $  $DateTime: 2019/10/22 02:53:17 $

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  when        who                            what, where, why
--------    -------                ----------------------------------------
05/18/2015    Youjunf                          First version of file
===========================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/


/*===========================================================================
            EXTERNAL FUNCTION DECLARATIONS
===========================================================================*/
void ds_wlan_meas_utils_quick_sort(int16 source[], uint8 start, uint8 tail, uint8 idx[]);

boolean ds_wlan_meas_utils_is_channel_in_5GHz(uint16 channel_freq);

#endif /* DS_WLAN_MEAS_UT_COMMON_H */
