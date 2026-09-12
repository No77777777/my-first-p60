#ifndef DS_WLAN_MEAS_PROC_H
#define DS_WLAN_MEAS_PROC_H

/*===========================================================================
                      DS_WLAN_MEAS_PROC.H

DESCRIPTION

   header of wlan meas processor

EXTERNALIZED FUNCTIONS

 Copyright (c) 2015 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================
                       EDIT HISTORY FOR FILE
  $Header: //components/rel/data.mpss/3.5..2.1/appsrv/wlan_meas/inc/ds_wlan_meas_proc.h#2 $
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
#include "datamodem_variation.h"
#include "comdef.h"

#if defined(FEATURE_ELS_WIFIMEAS_LTE_U_PH1) || defined(FEATURE_ELS_LWA)

#include "ds_wlan_meas_lteu_sm.h"
#include "ds_wlan_meas_timer.h"


/*===========================================================================
                   EXTERNAL DEFINITIONS AND TYPES
===========================================================================*/

typedef struct
{
#ifdef FEATURE_ELS_WIFIMEAS_LTE_U_PH1
  ds_wlan_meas_lteu_periodic_sm_s_type *lteu_periodic_sm_ptr;
#endif

#ifdef FEATURE_ELS_LWA
  ds_wlan_meas_pdcp_wa1_sm_s_type      *pdcp_wa1_sm_ptr;
  ds_wlan_meas_pdcp_wa2_sm_s_type      *pdcp_wa2_sm_ptr;
  ds_wlan_meas_pdcp_wn_sm_s_type       *pdcp_wn_sm_ptr;
  ds_wlan_meas_pdcp_ws_sm_s_type       *pdcp_ws_sm_ptr;
#endif 

  ds_wlan_meas_timer_list_s_type       timer_list;
}ds_wlan_meas_proc_s_type;



extern ds_wlan_meas_proc_s_type  wlan_meas_proc;

/*===========================================================================
            EXTERNAL FUNCTION DECLARATIONS
===========================================================================*/

/*===========================================================================
FUNCTION      DS_DSD_WLAN_MEAS_PROC_INIT

DESCRIPTION   This function initializes wlan_meas processor and all wlan_meas 
              statemachines.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  Variables are initialized to default value. 
===========================================================================*/
void ds_wlan_meas_proc_init(void);


/*===========================================================================
FUNCTION      DS_WLAN_MEAS_PROC_GET_LTEU_MEAS_ENABLED

DESCRIPTION   This function returns TRUE if ds lteu meas NV is enabled

PARAMETERS    None

DEPENDENCIES  None

RETURN VALUE  TRUE   return TRUE if ds lteu meas nv is enabled
              FALSE  return FALSE if ds lteu meas nv is disabled

SIDE EFFECTS  None
===========================================================================*/
boolean ds_wlan_meas_proc_get_lteu_meas_enabled(void);



/*===========================================================================
FUNCTION      DS_WLAN_MEAS_PROC_GET_LWA_MEAS_ENABLED

DESCRIPTION   This function returns TRUE if ds lwa meas NV is enabled

PARAMETERS    None

DEPENDENCIES  None

RETURN VALUE  TRUE   return TRUE if ds lwa meas nv is enabled
              FALSE  return FALSE if ds lwa meas nv is disabled

SIDE EFFECTS  None
===========================================================================*/
boolean ds_wlan_meas_proc_get_lwa_meas_enabled(void);

#endif /* FEATURE_ELS_WIFIMEAS_LTE_U_PH1 || FEATURE_ELS_LWA*/

#endif /* DS_WLAN_MEAS_MGR_H */
