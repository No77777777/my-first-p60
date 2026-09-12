#ifndef DS3GTPUTUTIL_H
#define DS3GTPUTUTIL_H
/*===========================================================================

   3 G   D A T A   S E R V I C E S   T H R O U G H P U T   U T I L I T Y


                            H E A D E R    F I L E

DESCRIPTION
  This file contains functions and definitions exported by 3G Ds throughput util
  to other Data Services software units.


  Copyright (c) 2001 - 2017 by Qualcomm Technologies Incorporated. All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/12/17   lb      Initial version.

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
#include "ds_eps_throughput_hdlr.h"
#include "ds3gtput_thres_mgr.h"
#include "ps_sys.h"
#include "ps_sys_ioctl.h"

#define DS_TPUT_LOGGING_VERSION_1 1
#define DS_TPUT_LOGGING_VERSION_2 2
#define DS_MAX_APN_LEN_LOG_PKT  104

typedef enum
{
   DS_TPUT_UL_EST        = 0,   ///<  Uplink Throughput Estimation 
   DS_TPUT_CONF          = 1,   ///<  Configured Throughput Estimation 
   DS_TPUT_DL_AVAIL      = 2,   ///<  Downlink Available Throughput 
   DS_TPUT_ULDL_ACH_ACT  = 3,   ///<  Uplink Downlink Achievable Actual Throughput 
   DS_TPUT_INFO_PER_APN  = 4,    ///<  Throughput Info Per Apn
   DS_TPUT_LCE_CRITERIA = 5,    ///<  Threshold based throughput reporting criteria from QMI
   DS_TPUT_LCE_DATA_RATE_EVAL = 6,    ///< Evaluate data rate reported from lower layer with lce criteria
   DS_TPUT_LCE_DL_REPORT = 7,   ///< Report downlink throughput to AP
   DS_TPUT_LCE_UL_REPORT = 8    ///< Report uplink throughput to AP
} ds_tput_info_enum;

typedef struct
{
  char   apn_string[DS_MAX_APN_LEN_LOG_PKT]; ///< APN NAME : 104 bytes, 
                                             ///< 101 bytes for APN name, plus 
                                             ///< 3 bytes for 32 bits alignment
  uint32 uplink_actual_rate;                 ///< UPLINK ACTUAL RATE
  uint32 uplink_allowed_rate;                ///< UPLINK ALLOWED RATE
  uint32 uplink_queue_size;                  ///< UPLINK QUEUE SIZE
}ds_per_pdn_tput_info_s;

typedef struct
{
  uint32 ul_achievable_throughput;  ///<  Uplink achievable Throughput in kbps
  uint32 dl_achievable_throughput;  ///<  Downlink achievable Throughput in kbps
  uint32 ul_actual_throughput;      ///<  Uplink Actual Throughput in kbps
  uint32 dl_actual_throughput;      ///<  Downlink Actual Throughput in kbps
}ds_uldl_ach_act_tput_info_s;

typedef struct
{
  uint64 time_diff;                 ///<  Time difference since last calculation
  uint64 ml1_bytes;                 ///<  Total ml1 bytes given to DS during time frame
  uint64 pdcp_bytes;                ///<  Total Pdcp bytes given to DS during time frame
  uint32 filtered_confidence_level; ///<  Filtered Confidence Level
  uint32 filtered_avail_tput;       ///<  Filtered Downlink Throughput in kbps
} ds_dl_avail_tput_info_s;

typedef struct
{
  uint32 ul_configured_kbps;        ///<  Uplink Configured Throughput in kbps
  uint32 dl_configured_kbps;        ///<  Downlink Configured Throughput in kbps
  uint16 uplink_scaling_factor;     ///<  Uplink Scaling factor
}ds_conf_tput_info_s;

typedef struct
{
  uint32 ul_confidence_level;       ///< Uplink Confidence
  uint32 ul_throughput_est;         ///<  Uplink Throughput Estimation in kbps
  uint32 taccumuate;                ///<  Uplink Taccumulate Interval in msec
  uint32 treporting;                ///<  Uplink Treporting Interval in msec
  uint32 l2reporting;               ///<  Uplink L2 reporting Interval in msec
  uint32 alpha;                     ///<  Uplink Alpha Filtering Parameter
} ds_ul_tput_info_est_s;

/*LCE related*/
typedef enum
{
  QMI_ACTION_INVALID = 0,
  QMI_ACTION_STOP = 1,
  QMI_ACTION_START = 2  
}ds_tput_qmi_action_enum;

typedef PACK(struct)
{
  boolean                        is_valid_rat;///< RAT validation flag
  ds3gtputthres_tput_rat_e_type  rat;///< RAT
  boolean                        is_valid_hyst_time;///< hysteresis timer validation flag
  uint32                         hyst_time;///< hysteresis timer
  boolean                        is_valid_hyst_mag_chng_size;///< hysteresis magnitude size validation flag
  uint32                         hyst_mag_chng_size;///< hysteresis magnitude size
  boolean                        is_valid_threshold_list;///< threshold list validation flag
  uint32                         threshold_list[PS_SYS_THRESHOLD_LIST_LEN];///< threshold list
  uint8                          threshold_list_len;///< the length of threshold list
  ds_tput_qmi_action_enum        action;///< threshold based throughput reporting action
  boolean                        reset_to_defaults;///< reset threshold based throughput reporting flag
}ds_lce_criteria_s;

typedef PACK(struct)
{  
  ds3gtputthres_tput_direction_e_type    direction;    ///< direction of reporting
  ds_lce_criteria_s                      criteria;     ///< criteria received from QMI
} ds_lce_criteria_info_s;

typedef PACK(struct)
{  
  ds3gtputthres_tput_direction_e_type      direction;       ///< direction of reporting
  uint32                                   data_rate;       ///< data rate reported from lower layer
  uint32                                   last_reported_data_rate;  ///< data rate reported last time
  uint32                                   left_threshold;   ///< left threshold after reporting the data rate
  uint32                                   right_threshold;  ///< right threshold after reporting the data rate
  boolean                                  thres_criteria_is_met;  ///< data rate is met threshold criteria
  boolean                                  magnitude_criteria_is_met;   ///< data rate is met magnitude criteria
  ds3gtputthres_report_state_e_type        hyst_timer_state;  ///< hysteresis timer is running or stopped
} ds_lce_data_rate_evaluation_info_s; 

typedef PACK(struct)
{  
  uint32   downlink_allowed_rate; ///< downlink data rate
  uint8    confidence_level;      ///< downlink confidence level
  boolean  is_suspended;          ///< suspending flag
} ds_lce_downlink_report_info_s; 

typedef PACK(struct)
{  
  uint32   uplink_allowed_rate; ///< uplink data rate
  uint8    confidence_level;    ///< uplink confidence level
  uint32   uplink_queue_size;   ///< uplink queue size
} ds_lce_uplink_report_info_s;

typedef union
{
  ds_ul_tput_info_est_s         ul_est_tput_info;         
  ///<  Uplink Throughput Estimation  enum: DS_TPUT_UL_EST 
  ds_conf_tput_info_s           conf_tput_info;           
  ///<  Configured Throughput Report enum: DS_TPUT_CONF
  ds_dl_avail_tput_info_s       dl_avail_tput_info;       
  ///<  Downlink Throughput Estimation enum: DS_TPUT_DL_AVAIL
  ds_uldl_ach_act_tput_info_s   uldl_ach_act_tput_info;   
  ///< Uplink Downlink Achievable Actual Throughput Info enum : DS_TPUT_ULDL_ACH_ACT
  ds_per_pdn_tput_info_s        pdn_tput_info;            
  ///< Per Pdn Throughput Info
  ds_lce_criteria_info_s              lce_report_criteria_info;   
  ///< lce reporting criteria from QMI
  ds_lce_data_rate_evaluation_info_s  lce_data_rate_evaluation_info;  
  ///< evaluate data rate reported from lower layer with lce criteria
  ds_lce_downlink_report_info_s       lce_dl_reporting_info;
  ///< Report downlink throughput to AP
  ds_lce_uplink_report_info_s         lce_ul_reporting_info;
  ///< Report uplink throughput to AP
}ds_tput_info_u_type; 

/*===========================================================================
  Log Code: LOG_DS_TPUT_C

  Revision: 1

  Description: @external
               This log packet captures periodic throughput information

  Frequency: per configuratoin 

  Size: 
===========================================================================*/
LOG_RECORD_DEFINE(LOG_DS_TPUT_C)
  uint8                               version;               
  ///< Log packet version. Range: 0...255.
  uint8                               sub_id;	             
  ///< Subscriber id: 1 is for SUB1, 2 is for SUB2
  ds_tput_info_enum                   tput_info_id;         
  ///< Indicating which structure to parse in below tput_info 
  ds_tput_info_u_type                 tput_info;            
  ///< Throughput Info for particular throughput item
LOG_RECORD_END


/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/


/*===========================================================================
FUNCTION      DS3GTPUTUTIL_UL_TPUT_EST_LOGGING

DESCRIPTION   this function log the ul tput estimate info

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtpututil_ul_tput_est_logging
(
  ds_eps_uplink_throughput_estimation_s ul_tput_est_info,
  sys_modem_as_id_e_type                subs_id
);

/*===========================================================================
FUNCTION      DS3GTPUTUTIL_CONF_TPUT_LOGGING

DESCRIPTION   this function log the config tput info

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtpututil_conf_tput_logging
(
  ds_eps_configured_throughput_estimation_s config_tput_est,
  sys_modem_as_id_e_type                    subs_id
);

/*===========================================================================
FUNCTION      DS3GTPUTUTIL_DL_AVAIL_TPUT_LOGGING

DESCRIPTION   this function log the down link available tput info

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtpututil_dl_avail_tput_logging
(
  uint64                  time_diff,
  uint64                  ml1_bytes,
  uint64                  pdcp_bytes,
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION      DS3GTPUTUTIL_ULDL_ACH_ACT_TPUT_LOGGING

DESCRIPTION   this function log the uplink/downlink achievable 
              and actual tput info

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtpututil_uldl_ach_act_tput_logging
(
  uint32                  ach_ul_data_rate,
  uint32                  ach_dl_data_rate,
  uint32                  act_ul_data_rate,
  uint32                  act_dl_data_rate,
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION      DS3GTPUTUTIL_PER_PDN_TPUT_LOGGING

DESCRIPTION   this function log the per PDN tput info

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtpututil_per_pdn_tput_logging
(
  ds_sys_throughput_info_type   pdn_tput_info,
  sys_modem_as_id_e_type        subs_id
);

/*===========================================================================
FUNCTION      DS3GTPUTUTIL_LCE_CRITERIA_LOGGING

DESCRIPTION   This function logs the threshold based througput reporting 
              criteria received from QMI 

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtpututil_lce_criteria_logging
(
  ds3gtputthres_tput_direction_e_type   direction,
  ps_sys_thrput_thres_criteria_type     thres_criteria,
  sys_modem_as_id_e_type                subs_id
);

/*===========================================================================
FUNCTION      DS3GTPUTUTIL_LCE_DATA_RATE_EVALUATION_LOGGING

DESCRIPTION   This function logs the data rate received from lower layer and 
              the evaluation result

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtpututil_lce_data_rate_evaluation_logging
(
  ds3gtputthres_tput_direction_e_type  direction,
  uint32                               data_rate,
  ds3gtputthres_eval_info_s           *eval_info_ptr,
  boolean                              thres_criteria_is_met,
  boolean                              magnitude_criteria_is_met,
  sys_modem_as_id_e_type               subs_id
);

/*===========================================================================
FUNCTION      DS3GTPUTUTIL_LCE_DL_REPORTING_LOGGING

DESCRIPTION   This function logs the LCE downlink throughput reporting info

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtpututil_lce_dl_reporting_logging
(
  ps_sys_dl_throughput_info_type        dl_tput_info,
  sys_modem_as_id_e_type                subs_id
);

/*===========================================================================
FUNCTION      DS3GTPUTUTIL_LCE_UL_REPORTING_LOGGING

DESCRIPTION   This function logs the LCE uplink throughput reporting info

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtpututil_lce_ul_reporting_logging
(
  ps_sys_ul_throughput_info_type        ul_tput_info,
  sys_modem_as_id_e_type                subs_id
);

#endif /* DS3GTPUTUTIL_H */

