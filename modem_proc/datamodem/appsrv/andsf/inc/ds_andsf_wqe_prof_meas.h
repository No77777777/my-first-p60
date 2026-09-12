/*===========================================================================
                      DS_ANDSF_WQE_PROF_MEAS.H

DESCRIPTION
 Header file for wifi measurement for each wqe profile.

EXTERNALIZED FUNCTIONS

 Copyright (c) 2015 - 2019 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================
                            EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  when        who                            what, where, why
--------    -------                ----------------------------------------
11/10/2015   Youjunf                  First version of file
===========================================================================*/
#ifndef DS_ANDSF_WQE_PROF_MEAS_H
#define DS_ANDSF_WQE_PROF_MEAS_H

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#include "datamodem_variation.h"

#ifdef FEATURE_DATA_ANDSF_RULE_MGR
#include "comdef.h"
#include "ps_sys.h"
#include "ds_wlan_meas_comm_mgr.h"
#include "rex.h"

/*===========================================================================
                   EXTERNAL DEFINITIONS AND TYPES
===========================================================================*/

/*===========================================================================
                           FUNCTION DECLARATIONS
===========================================================================*/
#ifdef __cplusplus
extern "C"
{
#endif


/*===========================================================================
FUNCTION    DS_ANDSF_WQE_PROF_MEAS_INIT

DESCRIPTION
  Initialize WQE profile measurement module for windows.

PARAMETERS
  None

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ds_andsf_wqe_prof_meas_init(void);


/*===========================================================================
FUNCTION    DS_ANDSF_WQE_PROF_SET_SUBSTITUTE_WQE_MASK

DESCRIPTION
  Substitute function handles set wqe prof request from IMS if no CNE profiles support on HLOS

PARAMETERS
  req_wqe_prof_mask - requested wqe profile mask

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ds_andsf_wqe_prof_set_substitute_wqe_prof_mask(uint64 req_wqe_prof_mask, uint32 subs_idx, uint64 measurement_id);


/*===========================================================================
FUNCTION    DS_ANDSF_WQE_PROF_PROCESS_WIFI_MEAS_CONFIG_STATUS

DESCRIPTION
  This function process wifi meas config status in windows wqe module

PARAMETERS
  wifi_meas_config_status_ptr - wifi meas configuration status from AP

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ds_andsf_wqe_prof_process_wifi_meas_config_status
(
  const ps_sys_wlan_wifi_meas_report_config_status_type *wifi_meas_config_status_ptr
);


/*===========================================================================
FUNCTION      DS_ANDSF_WQE_PROF_PROCESS_WIFI_MEAS_REPORT

DESCRIPTION
  This function process wifi meas report in windows wqe module

PARAMETERS
  wifi_meas_report_ptr - wifi meas report from AP

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ds_andsf_wqe_prof_process_wifi_meas_report
(
  const ds_wlan_meas_report_info_s_type *wifi_meas_report_ptr
);


void ds_andsf_wqe_meas_report_wait_timer_expire_hdlr(uint32 timer_index);

#ifdef __cplusplus
}
#endif

#endif /*FEATURE_DATA_ANDSF_RULE_MGR*/
#endif /* DS_ANDSF_WQE_PROF_MEAS_H */
