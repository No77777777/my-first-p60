#ifndef DS_WLAN_MEAS_TIMER_H
#define DS_WLAN_MEAS_TIMER_H


/*===========================================================================

                        DS_WLAN_MEAS_TIMER.H

DESCRIPTION
   

                
EXTERNALIZED FUNCTIONS

 Copyright (c) 2015 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================
                       EDIT HISTORY FOR FILE
  $Header: //components/rel/data.mpss/3.5..2.1/appsrv/wlan_meas/inc/ds_wlan_meas_timer.h#2 $
  $Author: pwbldsvc $  $DateTime: 2019/10/22 02:53:17 $
  
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  when        who                            what, where, why
--------    -------                ----------------------------------------
05/30/2015    Youjunf                          First version of file
===========================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#include "datamodem_variation.h"
#include "comdef.h"

#if defined(FEATURE_ELS_WIFIMEAS_LTE_U_PH1) || defined(FEATURE_ELS_LWA)

#include "rex.h"

/*===========================================================================
                   EXTERNAL DEFINITIONS AND TYPES
===========================================================================*/
typedef enum
{
  DS_WLAN_MEAS_INVALID_TIMERID_T0 = 0,
  DS_WLAN_MEAS_LTEU_PERIODIC_REPORT_TIMERID_T1,
  DS_WLAN_MEAS_PDCP_WA1_REPORT_TIMERID_T2,
  DS_WLAN_MEAS_PDCP_WA2_REPORT_TIMERID_T3,
  DS_WLAN_MEAS_PDCP_WS_REPORT_TIMERID_T4,
  DS_WLAN_MEAS_PDCP_WN_REPORT_TIMERID_T5,
  DS_WLAN_MEAS_MAX_TIMERID = DS_WLAN_MEAS_PDCP_WN_REPORT_TIMERID_T5
}ds_wlan_meas_timer_id_e_type;


/*---------------------------------------------------------------------------
  DS Wlan meas Timer structure
---------------------------------------------------------------------------*/
typedef struct
{
  rex_timer_type   timer;
}ds_wlan_meas_timer_info_s_type;


/*---------------------------------------------------------------------------
  DS Wlan meas Timer list structure
---------------------------------------------------------------------------*/
typedef struct
{
  ds_wlan_meas_timer_info_s_type  lteu_periodic_report_timer;     /*T01*/
  ds_wlan_meas_timer_info_s_type  pdcp_wa1_report_timer;          /*T02*/
  ds_wlan_meas_timer_info_s_type  pdcp_wa2_report_timer;          /*T03*/
  ds_wlan_meas_timer_info_s_type  pdcp_ws_report_timer;           /*T04*/
  ds_wlan_meas_timer_info_s_type  pdcp_wm_report_timer;           /*T05*/
}ds_wlan_meas_timer_list_s_type;

/*===========================================================================
            EXTERNAL FUNCTION DECLARATIONS
===========================================================================*/


/*===========================================================================
FUNCTION      DS_WLAN_MEAS_TIMER_INIT_TIMERS

DESCRIPTION   This function initializes wlan_meas timers.

PARAMETERS    None

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds_wlan_meas_timer_init_timers(void);


/*===========================================================================
FUNCTION      DS_WLAN_MEAS_TIMER_START_TIMER

DESCRIPTION   Starts the wlan_meas timer for specified timer_id and duration. 
 
PARAMETERS 
  timer_id
  duration 

DEPENDENCIES  None.

RETURN VALUE  None

SIDE EFFECTS  Starts Timer.
===========================================================================*/
void ds_wlan_meas_timer_start_timer(uint8 timer_id, uint32 duration);


/*===========================================================================
FUNCTION      DS_WLAN_MEAS_TIMER_RESTART_TIMER

DESCRIPTION   restart the wlan_meas timer for specified timer_id and duration. 
 
PARAMETERS 
  timer_id
  duration 

DEPENDENCIES  None.

RETURN VALUE  None

SIDE EFFECTS  restarts Timer.
===========================================================================*/
void ds_wlan_meas_timer_restart_timer(uint8 timer_id, uint32 duration);


/*===========================================================================
FUNCTION      DS_DSD_TIMER_STOP_TIMER

DESCRIPTION   Stop the wlan_meas Timer for specified timer_id. 
 
PARAMETERS 
  timer_id 
 
DEPENDENCIES  None.

RETURN VALUE  None

SIDE EFFECTS  Stops timer.
===========================================================================*/
void ds_wlan_meas_timer_stop_timer(uint8 timer_id);


#endif /* FEATURE_ELS_WIFIMEAS_LTE_U_PH1 || FEATURE_ELS_LWA*/
#endif /* DS_WLAN_MEAS_TIMER_H */

