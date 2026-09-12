/*======================================================================

  Technology Selector External Interface Header File

 GENERAL DESCRIPTION
  This file contains external Interface declarations that Tech Selector shall use
  
 EXTERNALIZED FUNCTIONS


 INITIALIZATION AND SEQUENCING REQUIREMENTS

 Copyright (c) 2017 - 2018 Qualcomm Technologies, Inc. 
  Qualcomm Technologies Confidential and Proprietary. All Rights Reserved.
 Export of this technology or software is regulated by the U.S. Government.
 Diversion contrary to U.S. law prohibited.
======================================================================*/

/*=====================================================================

 EDIT HISTORY FOR MODULE

 This section contains comments describing changes made to the module.
 Notice that changes are listed in reverse chronological order.

  $Header:

 when      who   what, where, why
 ----      ----  --- --------------------------------------------------- 
03/02/16   ss    Initial creation of file.
======================================================================*/
#ifndef TECH_SEL_EXT_INTF_H
#define TECH_SEL_EXT_INTF_H
#include "comdef.h"
#include "tm_common.h"
#include "tm_data.h"
#include "lbs_sdp.h"

#define TECHSEL_ENABLE_NO_SDP_SNS  SDP_CTRL_NO_SNS_ALLOWED
#define TECHSEL_ENABLE_ALL_SDP_SNS SDP_CTRL_ALL_SNS_ALLOWED

extern boolean (*techSelExt_StartCpi)(boolean v_isEmergency);
extern boolean (*techselExt_StopCpi)(boolean v_isEmergency);
extern boolean (*techselExt_QueryLppmStatus)(tm_sess_power_mode_s_type z_PowerMode);
extern boolean (*techselExt_StartLppm)(tm_sess_power_mode_s_type z_PowerMode);
extern boolean (*techselExt_StopLppm)(void);
extern boolean(*techselExt_ModifyLppm)(tm_sess_power_mode_s_type z_PowerMode);
extern boolean (*techSelExt_GetBestAvailPos)(tm_core_get_best_avail_pos_param_s_type *p_get_best_avail_pos_param);
extern void (*techSelExt_AbortSession)(boolean ask_prtl, tm_sess_stop_reason_e_type abort_reason);
extern void (*techSelExt_SetSdpSensorControl)(uint32 q_SensorControlMask);
extern boolean (*techSelExt_GnssLeReportingOn)(void);
extern boolean (*techSelExt_GnssLeReportingOff)(void);
extern boolean (*techSelExt_StartSensorLe)(const mgp_SensorLEConfigStructType  * const pz_SensorLEConfig);
extern boolean (*techSelExt_StopSensorLe)(void);
extern boolean (*techSelExt_GetSensorLeRelDisp)(uint64 t_ModemTsFromMsec,uint64 t_ModemTsToMsec);

#endif

