
#ifndef DS3GTPUT_THRES_MGR_H
#define DS3GTPUT_THRES_MGR_H
/*===========================================================================

   3 G   D A T A   S E R V I C E S   T H R O U G H P U T   
             T H R E S H O L D   M A N A G E R


                  H E A D E R    F I L E

DESCRIPTION
  This file contains functions and definitions exported by 3G Ds throughput mgr
  to other Data Services software units.


  Copyright (c) 2001 - 2018 by Qualcomm Technologies Incorporated. All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $PVCSPath: L:/src/asw/MM_DATA/vcs/ds3gtimer.h_v   1.0   08 Aug 2002 11:19:50   akhare  $
  $Header: //components/rel/data.mpss/3.5..2.1/3gpp/ds3gex/inc/ds3gtput_thres_mgr.h#1 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/02/18   lb     Initial version.

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "customer.h"

#include "dstask_v.h"
#include "ds3gsubsmgr.h"
#include "ds3gmgrint.h"
#include "ds3gtimer.h"
#include "rex.h"
#include "ps_sys_ioctl.h"


#define DS3G_MAX_NUM_THRESHOLD 8
#define DS3G_INVALID_THROUGHPUT 0xFFFFFFFF
#define DS3G_INVALID_THRESHOLD 0xFFFFFFFF

/*-----------------------------------------
For threshold based throughput reporting,
UE shall use the following default values:
WCDMA:
    hesteresisMs: 10,000 ms
    DL hysteresisDlKbps: 5,000
    DL threshold: {1Mbpps, 10Mbps}
    UL hysteresisDlKbps: 2,500
    UL theshold: {500kbps, 2Mbps}
LTE:
    hesteresisMs: 10,000 ms
    DL hysteresisDlKbps: 10,000
    DL threshold: {2Mbpps, 20Mbps}
    UL hysteresisDlKbps: 5,000
    UL theshold: {1Mbps, 4Mbps}
--------------------------------------------*/
#define DS3G_TPUT_THRES_DEFAULT_UMTS_HYST_TIME    10000
#define DS3G_TPUT_THRES_DEFAULT_UMTS_DL_MAG       5000
#define DS3G_TPUT_THRES_DEFAULT_UMTS_DL_THRES_NUM 2
#define DS3G_TPUT_THRES_DEFAULT_UMTS_DL_THRES1    1000000
#define DS3G_TPUT_THRES_DEFAULT_UMTS_DL_THRES2    10000000
#define DS3G_TPUT_THRES_DEFAULT_UMTS_UL_MAG       2500
#define DS3G_TPUT_THRES_DEFAULT_UMTS_UL_THRES_NUM 2
#define DS3G_TPUT_THRES_DEFAULT_UMTS_UL_THRES1    500000
#define DS3G_TPUT_THRES_DEFAULT_UMTS_UL_THRES2    2000000

#define DS3G_TPUT_THRES_DEFAULT_LTE_HYST_TIME     10000
#define DS3G_TPUT_THRES_DEFAULT_LTE_DL_MAG        10000
#define DS3G_TPUT_THRES_DEFAULT_LTE_DL_THRES_NUM  2
#define DS3G_TPUT_THRES_DEFAULT_LTE_DL_THRES1     2000000
#define DS3G_TPUT_THRES_DEFAULT_LTE_DL_THRES2     20000000
#define DS3G_TPUT_THRES_DEFAULT_LTE_UL_MAG        5000
#define DS3G_TPUT_THRES_DEFAULT_LTE_UL_THRES_NUM  2
#define DS3G_TPUT_THRES_DEFAULT_LTE_UL_THRES1     1000000
#define DS3G_TPUT_THRES_DEFAULT_LTE_UL_THRES2     4000000

typedef struct
{
  uint32                                       data_rate;
  uint8                                        confidence;
  boolean                                      moved_out;
}ds3gtputthres_dl_throughput_info_s;

typedef enum
{  
  DS3G_THRESHOLD_EVAL_STOPPED,
  /**< no threshold report timer is enabled \n */
  DS3G_THRESHOLD_EVAL_RUNNING
  /**< threshold report timer is triggered to run \n */
} ds3gtputthres_report_state_e_type;

typedef enum
{
  DS3G_TPUT_DL,
  /**< downlink \n */
  DS3G_TPUT_UL,
  /**< uplink \n */
  DS3G_TPUT_DIR_CNT
  /**< signal direction count \n */
} ds3gtputthres_tput_direction_e_type;

typedef enum
{
  DS3G_TPUT_RAT_LTE,
  /**< LTE RAT \n */
  DS3G_TPUT_RAT_UMTS,
  /**< WCDMA RAT \n */
  DS3G_TPUT_VALID_RAT_CNT,
  /**< supported throughput RAT count \n */
  DS3G_TPUT_RAT_OTHERS = DS3G_TPUT_VALID_RAT_CNT
  /**< rat other then LTE and UMTS \n */  
} ds3gtputthres_tput_rat_e_type;

typedef struct
{
  uint8                                 num_thresholds;
  uint32                                hyst_time;
  uint32                                hyst_mag;
  uint32                                thresholds[DS3G_MAX_NUM_THRESHOLD]; 
}ds3gtputthres_config_info_s;

typedef enum
{
  DS3G_TPUT_STOP,
  /**< action is start*/
  DS3G_TPUT_START 
  /**< action is stop*/
} ds3gtputthres_tput_action_e_type;

typedef struct
{
  /*cache QMI action of threshold based throughput reporting*/
  ds3gtputthres_tput_action_e_type              cached_action;
  /*last reported throughput*/
  uint32                                        last_reported_data_rate;
  /*last throughput reported by lower layer*/
  uint32                                        last_data_rate;
  /*left threshold of last reported throughput*/
  uint32                                        last_left_threshold; 
  /*right threshold of last reported throughput*/
  uint32                                        last_right_threshold;
  /*reporting state, the timer is running or stopped*/
  ds3gtputthres_report_state_e_type             state;
  rex_timer_type                                hyst_timer;
}ds3gtputthres_eval_info_s;

typedef struct
{
  ps_sys_ul_thrput_thres_criteria_type    ul_thres_criteria;
  sys_modem_as_id_e_type                  subs_id;
}ds3gtputthres_ul_info_type;

typedef struct
{
  ps_sys_dl_thrput_thres_criteria_type    dl_thres_criteria;
  sys_modem_as_id_e_type                  subs_id;
}ds3gtputthres_dl_info_type;

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================
FUNCTION      DS3GTPUTTHRES_INIT

DESCRIPTION   This function registers for PS sys events and initializes
              the throughput threshold reporting variables

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputthres_init
(
  void 
);

/*===========================================================================
FUNCTION      DS3GTPUTTHRES_IS_TPUT_DIRECTION_VALID

DESCRIPTION   This function validate tput direction.

PARAMETERS 
tput_direction: Uplink or Downlink 

DEPENDENCIES  None.

RETURN VALUE  TRUE: tput direction is valid, downlink or uplink
              FALSE: tput direction is not valid.
              
SIDE EFFECTS  None.
===========================================================================*/
boolean ds3gtputthres_is_tput_direction_valid
(
  ds3gtputthres_tput_direction_e_type  tput_direction
);

/*===========================================================================
FUNCTION      DS3GTPUTTHRES_IS_TPUT_RAT_VALID

DESCRIPTION   This function validate tput RAT info.

PARAMETERS    
tput_rat: throughput reporting RAT 
 
DEPENDENCIES  None.

RETURN VALUE  TRUE: tuput rat is valid, LTE or UMTS
              FALSE: tput rat is not valid.
              
SIDE EFFECTS  None.
===========================================================================*/
boolean ds3gtputthres_is_tput_rat_valid
(
  ds3gtputthres_tput_rat_e_type  tput_rat
);

/*===========================================================================
FUNCTION      DS3GTPUTTHRES_GET_CURRENT_RAT

DESCRIPTION   This function gets current RAT 
 
PARAMETERS 
ds3g_subs_id: Subscription ID 
 
DEPENDENCIES  None

RETURN VALUE  LTE 
              UMTS 
              OTHER RAT(Not supported by throughput report)
 
SIDE EFFECTS  None
===========================================================================*/
ds3gtputthres_tput_rat_e_type ds3gtputthres_get_current_rat
(
  ds3gsubsmgr_subs_id_e_type  ds3g_subs_id
);

/*===========================================================================
FUNCTION      DS3GTPUTTHRES_GET_CURRENT_CONFIG

DESCRIPTION   This function gets the throughput threshold configure base on 
              current subs id, tput direction and RAT.

PARAMETERS 
ds3g_subs_id: subscription ID 
direction: reporting direction, uplink or downlink

DEPENDENCIES  None.

RETURN VALUE  None

SIDE EFFECTS  None.
===========================================================================*/
ds3gtputthres_config_info_s*  ds3gtputthres_get_current_config
(
  ds3gsubsmgr_subs_id_e_type           ds3g_subs_id,
  ds3gtputthres_tput_direction_e_type  direction
);

/*===========================================================================
FUNCTION      DS3GTPUTTHRES_RAT_DS3G_TO_PS

DESCRIPTION   This function is to convert rat info from DS 3GPP format to PS 
              format.
 
PARAMETERS 
ds3g_rat: Throughput reporting RAT

DEPENDENCIES  None

RETURN VALUE  PS RAT

SIDE EFFECTS  None
===========================================================================*/
ps_sys_rat_ex_enum_type ds3gtputthres_rat_ds3g_to_ps
(
  ds3gtputthres_tput_rat_e_type ds3g_rat,
  ds3gsubsmgr_subs_id_e_type    ds3g_subs_id
);

/*===========================================================================
FUNCTION      DS3GTPUTTHRES_RAT_PS_TO_DS3G

DESCRIPTION   This function is to convert rat info from PS format to DS 3GPP 
              format.
 
PARAMETERS ps_rat: PS RAT

DEPENDENCIES  None

RETURN VALUE  Throughput RAT

SIDE EFFECTS  None
===========================================================================*/
ds3gtputthres_tput_rat_e_type ds3gtputthres_rat_ps_to_ds3g
(
  ps_sys_rat_ex_enum_type     ps_rat,
  ds3gsubsmgr_subs_id_e_type  ds3g_subs_id  
);

/*===========================================================================
FUNCTION      DS3GTPUTTHRES_INIT_CONFIG

DESCRIPTION   This function initialize configure info to default value for 
              threshold based throughput reporting.
 
PARAMETERS 
ds3g_subs_id: subscription ID
direction: throughput direciton, uplink or downlink
 
DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputthres_init_config
(
  ds3gsubsmgr_subs_id_e_type            ds3g_subs_id,
  ds3gtputthres_tput_direction_e_type   direction  
);

/*===========================================================================
FUNCTION      DS3GTPUTTHRES_INIT_EVAL_INFO

DESCRIPTION   This function initialize evaluation info for threshold based
              throughput reporting
 
PARAMETERS 
ds3g_subs_id: subscription ID
direction: throughput direciton, uplink or downlink 
 
DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputthres_init_eval_info
(
  ds3gsubsmgr_subs_id_e_type             ds3g_subs_id,
  ds3gtputthres_tput_direction_e_type    direction  
);

/*===========================================================================
FUNCTION      DS3GTPUTTHRES_UPDATE_EVAL_THRES_INFO

DESCRIPTION   This function update left and right threshold evaluation info 
              for threshold based throughput reporting
 
PARAMETERS 
ds3g_subs_id: subscription ID
direction: throughput direciton, uplink or downlink 
 
DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputthres_update_eval_thres_info
(
  ds3gsubsmgr_subs_id_e_type             ds3g_subs_id,
  ds3gtputthres_tput_direction_e_type    direction  
);

/*===========================================================================
FUNCTION      DS3GTPUTTHRES_RESET_EVAL_INFO

DESCRIPTION   This function reset evaluation info for threshold based
              throughput reporting
 
PARAMETERS 
ds3g_subs_id: subscription ID
direction: throughput direciton, uplink or downlink 
 
DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputthres_reset_eval_info
(
  ds3gsubsmgr_subs_id_e_type             ds3g_subs_id,
  ds3gtputthres_tput_direction_e_type    direction  
);

/*===========================================================================
FUNCTION      DS3GTPUTTHRES_START_HYST_TIMER

DESCRIPTION   This function starts hysteresis timer for threshold reporting.

PARAMETERS 
ds3g_subs_id: subscription ID
direction: throughput direciton, uplink or downlink  

DEPENDENCIES  None.

RETURN VALUE  None

SIDE EFFECTS  None.
===========================================================================*/
void ds3gtputthres_start_hyst_timer
(
  ds3gsubsmgr_subs_id_e_type          ds3g_subs_id,
  ds3gtputthres_tput_direction_e_type direction
);

/*===========================================================================
FUNCTION      DS3GTPUTTHRES_STOP_HYST_TIMER

DESCRIPTION   This function stops hysteresis timer for 
              threshold reporting.

PARAMETERS 
ds3g_subs_id: subscription ID
direction: throughput direciton, uplink or downlink   

DEPENDENCIES  None.

RETURN VALUE  None

SIDE EFFECTS  None.
===========================================================================*/
void ds3gtputthres_stop_hyst_timer
(
  ds3gsubsmgr_subs_id_e_type          ds3g_subs_id,
  ds3gtputthres_tput_direction_e_type direction
);

/*===========================================================================
FUNCTION      DS3GTPUTTHRES_START_STOP_TIMER_HDLR

DESCRIPTION   This function start the timer if it is not running or stop the 
              timer if it is running.
 
PARAMETERS 
ds3g_subs_id: subscription ID
direction: throughput direciton, uplink or downlink 
start_or_stop_timer: START or STOP the timer
 
DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputthres_start_stop_timer_hdlr
(
  ds3gsubsmgr_subs_id_e_type             ds3g_subs_id,
  ds3gtputthres_tput_direction_e_type    direction,
  ds3gtputthres_tput_action_e_type       start_or_stop_timer
);

/*===========================================================================
FUNCTION      DS3GTPUTTHRES_PROCESS_DS_CMD

DESCRIPTION   This function processes DS command
 
PARAMETERS 
 cmd_ptr: command pointer
 
DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputthres_process_ds_cmd
(
  const ds_cmd_type  *cmd_ptr
);

/*===========================================================================
FUNCTION      DS3GTPUTTHRES_CRITERIA_IS_UPDATED

DESCRIPTION   This function checks whether the criteria is changed. And if it 
              is changed and the fields in the criteria, which will get updated,
              are not all disabled(0), update to new criteria. 
 
PARAMETERS 
  new_thres_config_ptr: new threshold configuration pointer
  old_thres_config_ptr: previous threshold configuration pointer
  ds3g_subs_id; subscription ID
 
DEPENDENCIES  None

RETURN VALUE 

TRUE  --- ds3gtputthres_criteria_is_updated
FALSE --- ds3gtputthres_criteria_is_not_updated

SIDE EFFECTS  None
===========================================================================*/
boolean ds3gtputthres_criteria_is_updated
(
  ps_sys_dl_thrput_thres_criteria_type     *new_thres_config_ptr,
  ds3gtputthres_config_info_s              *old_thres_config_ptr,
  ds3gsubsmgr_subs_id_e_type                ds3g_subs_id
);

/*===========================================================================
FUNCTION      DS3GTPUTTHRES_PROCESS_ACTION_STATE_CHANGE

DESCRIPTION   This function process QMI action state changes  
 
PARAMETERS 
  criteria_is_changed: boolean of whether the criteria is changed or not
  new_criteria_ptr: new threshold configuration pointer
  direction: threshold based reporting direction
  ds3g_subs_id; subscription ID
 
DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputthres_process_action_state_change
(
  boolean                                   criteria_is_changed,
  ps_sys_dl_thrput_thres_criteria_type     *new_criteria_ptr,
  ds3gtputthres_tput_direction_e_type       direction,
  ds3gsubsmgr_subs_id_e_type                ds3g_subs_id
);

/*===========================================================================
FUNCTION      DS3GTPUTTHRES_SET_THRES_BASED_REPORT_ACTION_STATE

DESCRIPTION   This function change to start/stop state. If it moves to start,
              then report to PS about the actual criteria. If it moves to stop,
              set throughput report method to NONE and reset stored evaluation info.
              Notify lower layer about the action state.
 
PARAMETERS 
  direction: threshold based reporting direction
  ds3g_subs_id; subscription ID
  qmi_action: threshold based reporting action from QMI
 
DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputthres_set_thres_based_report_action_state
(
   ds3gtputthres_tput_direction_e_type       direction,
   ds3gsubsmgr_subs_id_e_type                ds3g_subs_id,
   ds3gtputthres_tput_action_e_type          qmi_action
);

/*===========================================================================
FUNCTION      DS3GTPUTTHRES_NOTIFY_ACTION_CHANGE_TO_LOWER_LAYER

DESCRIPTION   Notify lower layer about the action state.
 
PARAMETERS 
  ds3g_subs_id; subscription ID
  direction: threshold based reporting direction
 
DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputthres_notify_action_change_to_lower_layer
(
  ds3gsubsmgr_subs_id_e_type           ds3g_subs_id,
  ds3gtputthres_tput_direction_e_type  direction
);

/*===========================================================================
FUNCTION      DS3GTPUTTHRES_RESET_EVAL_INFO

DESCRIPTION   This function reset evaluation info for threshold based
              throughput reporting
 
PARAMETERS 
ds3g_subs_id: subscription ID
direction: throughput direciton, uplink or downlink 
 
DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputthres_reset_eval_info
(
  ds3gsubsmgr_subs_id_e_type             ds3g_subs_id,
  ds3gtputthres_tput_direction_e_type    direction  
);

/*===========================================================================
FUNCTION      DS3G_PROCESS_UPLINK_TPUT_THRES_CRITERIA_CMD

DESCRIPTION   This function is called to process the uplink throughput 
              threshold criteria command 
 
PARAMETERS 
cmd_ptr: command pointer 
 
DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3g_process_uplink_tput_thres_criteria_cmd
(
  const ds_cmd_type  *cmd_ptr
);

/*===========================================================================
FUNCTION      DS3G_PROCESS_DOWNLINK_TPUT_THRES_CRITERIA_CMD

DESCRIPTION   This function is called to process the downlink throughput 
              threshold criteria command 
 
PARAMETERS 
cmd_ptr: command pointer 
 
DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3g_process_downlink_tput_thres_criteria_cmd
(
  const ds_cmd_type  *cmd_ptr
);

/*===========================================================================
FUNCTION      DS3GTPUTTHRES_SET_UPLINK_TPUT_THRES_CRITERIA

DESCRIPTION   invoked via PS_SYS IOCTL when AP invokes the QMI API to set 
              uplink throughput threshold criteria.
 
PARAMETERS 
arg_val_ptr: argument value pointer
subs_id: subscription ID
ps_errno: PS error 
 
DEPENDENCIES  None

RETURN VALUE  -1 FAIL
               0: SUCCESS

SIDE EFFECTS  None
===========================================================================*/
int ds3gtputthres_set_uplink_tput_thres_criteria
(
  void                    *arg_val_ptr,
  sys_modem_as_id_e_type   subs_id,
  int16                   *ps_errno
);

/*===========================================================================
FUNCTION      DS3GTPUTTHRES_SET_DOWNLINK_TPUT_THRES_CRITERIA

DESCRIPTION   invoked via PS_SYS IOCTL when AP invokes the QMI API to set 
              downlink throughput threshold criteria.
 
PARAMETERS 
arg_val_ptr: argument value pointer
subs_id: subscription ID
ps_errno: PS error 
 
DEPENDENCIES  None

RETURN VALUE  -1: FAIL
               0: SUCCESS

SIDE EFFECTS  None
===========================================================================*/
int ds3gtputthres_set_downlink_tput_thres_criteria
(
  void                    *arg_val_ptr,
  sys_modem_as_id_e_type   subs_id,
  int16                   *ps_errno
);

/*===========================================================================
FUNCTION      DS3GTPUTTHRES_POST_DOWNLINK_CRITERIA_THRES_BASED_REPORTING

DESCRIPTION   This function posts the actual downlink throughput threshold 
              criteria of the preferred/current RAT.
 
PARAMETERS 
ds3g_subs_id: subscription ID 
 
DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputthres_post_downlink_criteria_thres_based_reporting
(
  ds3gsubsmgr_subs_id_e_type  ds3g_subs_id
);

/*===========================================================================
FUNCTION      DS3GTPUTTHRES_POST_UPLINK_CRITERIA_THRES_BASED_REPORTING

DESCRIPTION   This function posts the actual uplink throughput threshold 
              criteria of the preferred/current RAT.
 
PARAMETERS 
ds3g_subs_id: subscription ID 
 
DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputthres_post_uplink_criteria_thres_based_reporting
(
  ds3gsubsmgr_subs_id_e_type  ds3g_subs_id
);

/*===========================================================================
FUNCTION      DS3GTPUTTHRES_IS_THRESHOLD_AND_MAGNITUDE_CRITERIA_MET

DESCRIPTION   This function checks if the new tput value is below the left 
              threshold or above the right threshold and if the difference
              between the new tput value and the last reported value is greater
              than or equal to the hysteresis magnitude(in kbps, configured by AP).

PARAMETERS    
  ds3g_subs_id --- ds3g subscription id
  direction  --- uplink or downlink
  data_rate  --- data rate
 
DEPENDENCIES  None.

RETURN VALUE  TRUE: threshold and magnitude criteria is met
              FALSE: at least one of threshold magnitude criteria is not met

SIDE EFFECTS  None.
===========================================================================*/
boolean ds3gtputthres_is_threshold_and_magnitude_criteria_met
(
  ds3gsubsmgr_subs_id_e_type           ds3g_subs_id,
  ds3gtputthres_tput_direction_e_type  direction,
  uint32                               data_rate
);

/*===========================================================================
FUNCTION      DS3GTPUTTHRES_REPORT_CRITERIA_EVALUATION

DESCRIPTION   This function evaluate throughput report base on threshold list,
              hysteresis magnitude and hysteresis timer.

PARAMETERS    
  ds3g_subs_id --- ds3g subscription id
  direction  --- uplink or downlink
  data_rate  --- data rate   

DEPENDENCIES  None.

RETURN VALUE  None

SIDE EFFECTS  None.
===========================================================================*/
void ds3gtputthres_report_criteria_evaluation
(
  ds3gsubsmgr_subs_id_e_type           ds3g_subs_id,
  ds3gtputthres_tput_direction_e_type  direction,
  uint32                               data_rate
);

/*===========================================================================
FUNCTION      DS3GTPUTTHRES_REPORT_DOWNLINK_THROUGHPUT_RESPONSE

DESCRIPTION   This function is invoked to report the downlink data rate
              and confidence for wcdma/lte rats to ps framework

PARAMETERS 
ds3g_subs_id: subscription ID 
 
DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputthres_report_downlink_throughput_response
(
  ds3gsubsmgr_subs_id_e_type  ds3g_subs_id
);

/*===========================================================================
FUNCTION      DS3GTPUTTHRES_REPORT_THROUGHPUT_RESPONSE

DESCRIPTION   This function is invoked to report the downlink/uplink data rate
              and confidence for wcdma/lte rats to ps framework, when 
              threshold report criteria are fulfilled.
PARAMETERS 
ds3g_subs_id: subscription ID 
direction: throughput reporting direction
 
DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputthres_report_throughput_response
(
  ds3gsubsmgr_subs_id_e_type                ds3g_subs_id,
  ds3gtputthres_tput_direction_e_type       direction
);

/*===========================================================================
FUNCTION      DS3GTPUTTHRES_GET_CANDIDATE_THRESHOLD

DESCRIPTION   This function find the candidate left and right threshold
              for the input data rate.

PARAMETERS    
  ds3g_subs_id --- ds3g subscription id
  direction  --- uplink or downlink
  data_rate  --- data rate
  candidate_left_thres_ptr -- pointer of left threshold to be updated
  candidate_right_thres_ptr -- pointer of right threshold to be update

DEPENDENCIES  None.

RETURN VALUE  None

SIDE EFFECTS  None.
===========================================================================*/
void ds3gtputthres_get_candidate_threshold
(
  ds3gsubsmgr_subs_id_e_type           ds3g_subs_id,
  ds3gtputthres_tput_direction_e_type  direction,
  uint32                               data_rate,
  uint32                              *candidate_left_thres_ptr,
  uint32                              *candidate_right_thres_ptr  
);

/*===========================================================================
FUNCTION      DS3GTPUTTHRES_HYST_TIMER_EXPIRE_HDLR

DESCRIPTION   This function set the correspond state machine to
              timer expired state. Report the throughput.
 
PARAMETERS    
  ds3g_subs_id --- ds3g subscription id
  timer_id --- timer id
 
DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputthres_hyst_timer_expire_hdlr
(
  ds3gsubsmgr_subs_id_e_type   ds3g_subs_id,
  ds3g_timer_enum_type         timer_id
);

/*===========================================================================
FUNCTION      DS3GTPUTTHRES_TPUT_THRES_ENABLED

DESCRIPTION   This function is to get whether threshold reporting is START.
 
PARAMETERS    
  ds3g_subs_id --- ds3g subscription id
  direction --- throughput reporting direction
 
DEPENDENCIES  None

RETURN VALUE  TRUE: threshold based reporting is enabled
              FALSE: threshold based reporting is not enabled  

SIDE EFFECTS  None
===========================================================================*/
boolean ds3gtputthres_tput_thres_enabled
(
  ds3gsubsmgr_subs_id_e_type           ds3g_subs_id,
  ds3gtputthres_tput_direction_e_type  direction 
);
#endif /* DS3GTPUT_THRES_MGR_H */

