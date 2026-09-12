#ifndef DS3GTPUTMGR_H
#define DS3GTPUTMGR_H
/*===========================================================================

   3 G   D A T A   S E R V I C E S   T H R O U G H P U T   M A N A G E R


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
  $Header: //components/rel/data.mpss/3.5..2.1/3gpp/ds3gex/inc/ds3gtputmgr.h#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/07/14   vm     Initial version.

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
#include "ds3gtput_thres_mgr.h"


#define DS3G_ULDL_TPUT_T_TMEAS_DEFAULT_VALUE 250 /* in Milliseconds*/
#define DS3G_ULDL_TPUT_T_TMEAS_MIN_VALUE 50 /* in Milliseconds*/
#define DS3G_ULDL_TPUT_T_TMEAS_MAX_VALUE 500 /* in Milliseconds*/

#define DS3G_INVALID_DL_TPUT          0xFFFFFFFF
#define DS3G_INVALID_ACTUAL_INTERVAL  0xFFFFFFFF
#define DS3G_INVALID_DL_CONFIDENCE    0xFFFF
#define DS3G_MAX_DL_CONFIDENCE        0x7


#define DS3G_UPLINK_TPUT_WCDMA_TACCUM_DEFAULT_VALUE 100 /* in Milliseconds*/
#define DS3G_UPLINK_TPUT_WCDMA_TACCUM_MIN_VALUE  100 /* in Milliseconds*/
#define DS3G_UPLINK_TPUT_WCDMA_TACCUM_MAX_VALUE 5000 /* in Milliseconds*/

#define DS3G_UPLINK_TPUT_LTE_TACCUM_DEFAULT_VALUE 50 /* in Milliseconds*/
#define DS3G_UPLINK_TPUT_LTE_TACCUM_MIN_VALUE 50 /* in Milliseconds*/
#define DS3G_UPLINK_TPUT_LTE_TACCUM_MAX_VALUE 20000 /* in Milliseconds*/

#define DS3G_UL_INVALID_THROUGHPUT 0xFFFFFFFF
#define DS3G_UL_INVALID_CONFIDENCE 0xFF
#define DS3G_UL_INVALID_QUEUE_SIZE 0xFFFFFFFF

#define DS3G_UL_DEFAULT_TPUT_BETA 1
#define DS3G_UL_RESET_TPUT_BETA   0

typedef struct
{
  uint32         frequency;
  uint64         timer_count;
  boolean        need_to_report;
  uint32         actual_ul_data_rate;
  uint32         actual_dl_data_rate;
}ds3gtputmgr_uldl_throughput_report_client_info_s;

typedef enum
{
  DS3G_TPUT_CLIENT_MIN = 0,
  DS3G_TPUT_CLIENT_QMI = DS3G_TPUT_CLIENT_MIN,
  DS3G_TPUT_CLIENT_WRRC,
  DS3G_TPUT_CLIENT_CNT
}ds3gtputmgr_throughput_report_client_e_type;

typedef enum
{
  DS3G_TPUT_REPORT_DL,
  DS3G_TPUT_REPORT_UL
}ds3gtputmgr_throughput_report_dir_e_type;

typedef struct
{
  rex_timer_type uldl_throughput_timer;
  boolean achievable_wcdma_in_progress;
  boolean achievable_lte_in_progress;
}ds3gtputmgr_uldl_throughput_info_s;

typedef struct
{
 uint32                      frequency;
 ds3gsubsmgr_subs_id_e_type  ds3g_subs_id;
}ds3gtputmgr_reporting_info;

typedef struct
{
  rex_timer_type uplink_throughput_timer;
  /* uplink reporting timer */
  uint32  uplink_throughput_reporting;
  /* uplink reporting period set by client */
  uint32  actual_throughput_reporting;
  /* actual uplink reporting */
  boolean wcdma_in_progress;
  /* wcdma reporting in progress */
  boolean lte_in_progress;
  /* lte in progress */
  uint32  uplink_data_rate_kbps;
  /* Uplink data rate in kbps */
  uint8   uplink_confidence;
  /* Uplink confidence */
  boolean uplink_queue_size_req;
  /* if client set uplink_queue_size_req,
     if true,need to pass over all uplink queue size*/
  uint32 uplink_queue_size;
  /* Uplink data queue size */
}ds3gtputmgr_uplink_throughput_info_s;



typedef struct
{
 uint32                      reporting_interval;
 ds3gsubsmgr_subs_id_e_type  ds3g_subs_id;
}ds3gtputmgr_reporting_interval_info;

typedef enum
{
  DS3G_REPORT_ENABLED,
  /**< Throughput reporting enabled \n */
   DS3G_REPORT_DISABLED_NO_DATA_CALL,
  /**< No data call \n */
   DS3G_REPORT_DISABLED_ALL_CALLS_DORMANT,
  /**< All calls dormant \n */
  DS3G_REPORT_DISABLED_UNSUPPORTED_RAT,
  /**< Unsupported RAT \n */
  DS3G_REPORT_DISABLED_BY_CLIENT
  /**< Report disabled \n */
} ds3gtputmgr_est_reporting_status_e_type;

typedef struct
{
  ds3gtputmgr_est_reporting_status_e_type      reporting_status;
  uint32                                       actual_interval;
  uint32                                       requested_interval;
  boolean                                      update_reporting_status;
}ds3gtputmgr_dl_est_reporting_status_info_s;

typedef enum
{
  DS3G_TPUT_METHOD_NONE,
  /**< no throughput report method is configued \n */
  DS3G_TPUT_METHOD_TIMER_BASED,
  /**< throughput report base on periodic \n */
  DS3G_TPUT_METHOD_THRES_BASED,
  /**< throughput report base on threshold \n */
  DS3G_TPUT_METHOD_CNT
  /**< throughput RAT count \n */  
} ds3gtputmgr_tput_report_method_e_type;

typedef PACK(struct)
{
  /* Feature enable/disable. Default value = FALSE */
  boolean                 w2l_enable;
  /* The time (in seconds) where modem can spent in
     finding LTE cells before going back to UMTS. Default value = 5s */
  uint32                  tsearch; 
  /* Threshold limit in kpbs to trigger searching for LTE cell. Default value = 64 kpbs */   
  uint32                  threshold; 
  /*The minimum time (in seconds) that the UE must be in Thr1 
    to trigger searching for LTE cells. Default value = 600s */ 
  uint32                  t1; 
  /* The minimum time (in seconds) that the UE must wait
     to evaluate singaling power/quality of LTE ferq while in WCDMA connected state.
     Default Value = 30s */
  uint32                  t2; 
  /* Reference Signal Received Quality LTE frequency (in dB) must meet. Default value = */
  int32                   lte_rsrq;
  /* Reference Signal Received Power that LTE frequency (in dB) must meet. Default value =  */ 
  int32                   lte_rsrp; 
}ds3gtputmgr_w2l_redir_config_s;

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/


/*===========================================================================
FUNCTION      DS3GTPUTMGR_INIT

DESCRIPTION   This function initializes the ds3g throughput mgr services. 
              This function must be called once, at Data Services Task startup.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputmgr_init( void );

/*===========================================================================
FUNCTION      DS3GTPUTMGR_PROCESS_INIT_COMPLETE_CMD

DESCRIPTION   This function registers for PS sys events and timer objects

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputmgr_process_init_complete_cmd( void );

/*===========================================================================
FUNCTION      DS3G_GET_ACTUAL_THROUGHPUT_REFRESH_INTERVAL

DESCRIPTION   For the given subscription ID, this function returns the time 
              interval(in msec), the actual throughput rate gets refreshed
              for every bearer/RLP depending on the current sys mode and
              the number of active calls 

DEPENDENCIES  The throughput Info event timer should be running to refresh 
              actual throughput for all bearers/RLP's  

RETURN VALUE  uint32 - Refresh Interval in (msec)
 
              0       - If the timer is not running (or) if the subscription
                        ID is invalid
 
SIDE EFFECTS  None
===========================================================================*/
uint32 ds3gtputmgr_get_actual_tput_refresh_interval
(
  ds3gsubsmgr_subs_id_e_type ds3g_subs_id
);

/*===========================================================================
FUNCTION      DS3GTPUTMGR_GET_ESTIMATED_DL_TPUT_INTERVAL

DESCRIPTION   For the given subscription ID, this function returns the 
              for t_report interval

DEPENDENCIES  

RETURN VALUE  uint32 - T_accumulate multiplier 

SIDE EFFECTS  None
===========================================================================*/
uint32 ds3gtputmgr_get_estimated_dl_tput_requested_interval
(
  ds3gsubsmgr_subs_id_e_type ds3g_subs_id
);

/*===========================================================================
FUNCTION      DS3GTPUTMGR_PROCESS_TIMER_EXPIRED_CMD

DESCRIPTION   This function processes the DS_CMD_DS3GTPUTMGR_TIMER_EXPIRED. 
              It determines which timer has expired, and calls the appropriate
              function to handle the timer expiry.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputmgr_process_timer_expired_cmd
(
  ds_cmd_type  *cmd_ptr
);

/*===========================================================================
FUNCTION      DS3GTPUTMGR_ULDL_TIMER_INTIT

DESCRIPTION   This function registers for PS sys events and timer objects related 
to throughput reporting ul dl 

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputmgr_uldl_timer_init(void);
#ifdef FEATURE_DATA_RAVE_SUPPORT 
/*===========================================================================
FUNCTION      DS3GTPUTMGR_UPLINK_THROUGHPUT_INTIT

DESCRIPTION   This function registers for PS sys events and initializes 
the uplink throughput reporting variables 

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputmgr_uplink_throughput_init(void);
/*==============================================================================
FUNCTION      DS3GTPUTMGR_UPDATE_UPLINK_THROUGHPUT_CONFIGURE

DESCRIPTION   This functions is invoked by modehandler in wcdma and lte mode 
              during call transition. This inturn will clear the timer and
              inform lower layer if needed/ start the uplink timer and inform
              the upper layer

DEPENDENCIES  None 
 
RETURN VALUE  None
===========================================================================*/
void ds3gtputmgr_update_uplink_throughput_configure
(
  ds3gsubsmgr_subs_id_e_type  ds3g_subs_id,
   boolean                    tput_module
);
/*===========================================================================
FUNCTION      DS3GTPUTMGR_GET_UPLINK_QUEUE_SIZE_REQ

DESCRIPTION   This function get cached uplink_queue_size_req 
              it is configured by client

DEPENDENCIES  None

RETURN VALUE Boolean

SIDE EFFECTS  None
===========================================================================*/

boolean ds3gtputmgr_get_uplink_queue_size_req
(
  ds3gsubsmgr_subs_id_e_type  ds3g_subs_id
);
/*===========================================================================
FUNCTION      DS3GTPUTMGR_GET_UPLINK_DATA_RATE_KBPS

DESCRIPTION   This function get cached uplink_data_rate_kbps

DEPENDENCIES  None

RETURN VALUE None

SIDE EFFECTS  None
===========================================================================*/
uint32 ds3gtputmgr_get_uplink_data_rate_kbps
(
  ds3gsubsmgr_subs_id_e_type  ds3g_subs_id
);
/*===========================================================================
FUNCTION      DS3GTPUTMGR_GET_ULDL_THROUGHPUT_FREQUENCY

DESCRIPTION   This function returns the uldl frequency for the given ds3g 
              subs_id 

DEPENDENCIES  None

RETURN VALUE  uint32 - Frequency
 
              0       - If the timer is not running (or) if the subscription
                        ID is invalid
 
SIDE EFFECTS  None
===========================================================================*/
uint32 ds3gtputmgr_get_uplink_throughput_reporting
(
  ds3gsubsmgr_subs_id_e_type ds3g_subs_id
);

/*===========================================================================
FUNCTION      DS3GTPUTMGR_SET_UPLINK_THROUGHPUT_REPORTING

DESCRIPTION   This function configures frequency to generate 
              uldl throughput information Ind periodically. 

DEPENDENCIES  None

RETURN VALUE  TRUE  - If Throughput Info Ind interval has been set for 
                      the Subscription ID
              FALSE - Otherwise
 
SIDE EFFECTS  None
===========================================================================*/
boolean ds3gtputmgr_set_uplink_throughput_reporting
(
  ds3gsubsmgr_subs_id_e_type ds3g_subs_id,
  uint32                     uplink_throughput_reporting,
  boolean                    uplink_queue_size_req
);
#endif
/*===========================================================================
FUNCTION      DS3GTPUTMGR_GET_ULDL_THROUGHPUT_FREQUENCY

DESCRIPTION   This function returns the uldl frequency for the given ds3g 
              subs_id 

DEPENDENCIES  None

RETURN VALUE  uint32 - Frequency
 
              0       - If the timer is not running (or) if the subscription
                        ID is invalid
 
SIDE EFFECTS  None
===========================================================================*/
uint32 ds3gtputmgr_get_uldl_throughput_frequency
(
  ds3gtputmgr_throughput_report_client_e_type  client,
  ds3gsubsmgr_subs_id_e_type ds3g_subs_id
);
/*===========================================================================
FUNCTION      DS3GTPUTMGR_RESET_ULDL_THROUPUT_REPORT_CLIENT_INFO

DESCRIPTION   This function resets uldl throughput timer count for the 
              corresponding ds3g subs id

DEPENDENCIES  None

RETURN VALUE None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputmgr_reset_uldl_throuput_report_client_info
(
  ds3gtputmgr_throughput_report_client_e_type  client,
  ds3gsubsmgr_subs_id_e_type ds3g_subs_id
);


/*===========================================================================
FUNCTION      DS3GTPUTMGR_RESET_WCDMA_ACHIEVABLE_IN_PROGRESS

DESCRIPTION   This function resets wcdma achievable  variable which
              tracks whether the achievable throughput timer is running for wcdma
              rat on a particular subscription

DEPENDENCIES  None

RETURN VALUE None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputmgr_reset_wcdma_achievable_in_progress
(
  ds3gsubsmgr_subs_id_e_type ds3g_subs_id
);


/*===========================================================================
FUNCTION      DS3GTPUTMGR_RESET_LTE_ACHIEVABLE_IN_PROGRESS

DESCRIPTION   This function resets lte achievable   variable which
              tracks whether the achievable throughput timer is running for lte
              rat on a particular subscription

DEPENDENCIES  None

RETURN VALUE None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputmgr_reset_lte_achievable_in_progress
(
  ds3gsubsmgr_subs_id_e_type ds3g_subs_id
);


/*===========================================================================
FUNCTION      DS3GTPUTMGR_RESET_LTE_ACHIEVABLE_IN_PROGRESS

DESCRIPTION   This function resets lte achievable   variable which
              tracks whether the achievable throughput timer is running for lte
              rat on a particular subscription

DEPENDENCIES  None

RETURN VALUE None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputmgr_set_wcdma_achievable_in_progress
(
  ds3gsubsmgr_subs_id_e_type ds3g_subs_id
);


/*===========================================================================
FUNCTION      DS3GTPUTMGR_SET_LTE_ACHIEVABLE_IN_PROGRESS

DESCRIPTION    This function sets lte achievable   variable which
              tracks whether the achievable throughput timer is running for lte
              rat on a particular subscription

DEPENDENCIES  None

RETURN VALUE None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputmgr_set_lte_achievable_in_progress
(
  ds3gsubsmgr_subs_id_e_type ds3g_subs_id
);


/*===========================================================================
FUNCTION      DS3GTPUTMGR_GET_LTE_ACHIEVABLE_IN_PROGRESS

DESCRIPTION   This function gets lte achievable   variable which
              tracks whether the achievable throughput timer is running for lte
              rat on a particular subscription

DEPENDENCIES  None

RETURN VALUE None

SIDE EFFECTS  None
===========================================================================*/
boolean ds3gtputmgr_get_lte_achievable_in_progress
(
  ds3gsubsmgr_subs_id_e_type ds3g_subs_id
);

/*===========================================================================
FUNCTION      DS3GTPUTMGR_GET_WCDMA_ACHIEVABLE_IN_PROGRESS

DESCRIPTION   This function gets wcdma achievable   variable which
              tracks whether the achievable throughput timer is running for wcdma
              rat on a particular subscription

DEPENDENCIES  None

RETURN VALUE None

SIDE EFFECTS  None
===========================================================================*/
boolean ds3gtputmgr_get_wcdma_achievable_in_progress
(
  ds3gsubsmgr_subs_id_e_type ds3g_subs_id
);
#ifdef FEATURE_DATA_RAVE_SUPPORT 
/*===========================================================================
FUNCTION      DS3GTPUTMGR_SET_LTE_UPLINK_IN_PROGRESS

DESCRIPTION   This function sets lte uplink  variable which
              tracks whether the uplink throughput timer is running for lte
              rat on a particular subscription

DEPENDENCIES  None

RETURN VALUE None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputmgr_set_lte_uplink_in_progress
(
  ds3gsubsmgr_subs_id_e_type ds3g_subs_id,
    boolean                  uplink_status
);


/*===========================================================================
FUNCTION      DS3GTPUTMGR_GET_LTE_UPLINK_IN_PROGRESS

DESCRIPTION   This function gets lte uplink   variable which
              tracks whether the uplink throughput timer is running for
              lte rat on a particular subscription

DEPENDENCIES  None

RETURN VALUE None

SIDE EFFECTS  None
===========================================================================*/
boolean ds3gtputmgr_get_lte_uplink_in_progress
(
  ds3gsubsmgr_subs_id_e_type ds3g_subs_id
);

/*===========================================================================
FUNCTION      DS3GTPUTMGR_SET_UPLINK_THROUGHPUT_RATE_PARAMS

DESCRIPTION   This function sets lte uplink  variable which
              tracks whether the uplink throughput timer is running for lte
              rat on a particular subscription

DEPENDENCIES  None

RETURN VALUE None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputmgr_set_uplink_throughput_rate_params
(
  ds3gsubsmgr_subs_id_e_type  ds3g_subs_id,
  uint32  uplink_datarate_kbps,
  uint8   confidence,
  uint32  uplink_queue_size
);

/*===========================================================================
FUNCTION      DS3GTPUTMGR_GET_WCDMA_UPLINK_IN_PROGRESS

DESCRIPTION   This function gets wcdma uplink   variable which
              tracks whether the uplink throughput timer is running for
              wcdma rat for a particular subscription

DEPENDENCIES  None

RETURN VALUE None

SIDE EFFECTS  None
===========================================================================*/
boolean  ds3gtputmgr_get_wcdma_uplink_in_progress
(
  ds3gsubsmgr_subs_id_e_type ds3g_subs_id
);

/*===========================================================================
FUNCTION      DS3GTPUTMGR_SET_WCDMA_UPLINK_IN_PROGRESS

DESCRIPTION   This function sets wcdma uplink  variable which
              tracks whether the uplink throughput timer is running for wcdma
              rat on a particular subscription

DEPENDENCIES  None

RETURN VALUE None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputmgr_set_wcdma_uplink_in_progress
(
  ds3gsubsmgr_subs_id_e_type ds3g_subs_id,
  boolean                    uplink_status
);
#endif
/*===========================================================================
FUNCTION      DS3GTPUTMGR_SET_ULDL_THROUGHPUT_TIMER_COUNT

DESCRIPTION   This function sets uldl throughput timer count for the corresponding
              ds3g subs id

DEPENDENCIES  None

RETURN VALUE None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputmgr_set_uldl_throughput_timer_count
(
  ds3gtputmgr_throughput_report_client_e_type  client,
  ds3gsubsmgr_subs_id_e_type ds3g_subs_id,
  uint64                     new_timer_count
);

/*===========================================================================
FUNCTION      DS3GTPUTMGR_GET_ULDL_THROUGHPUT_TIMER_COUNT

DESCRIPTION   This function gets uldl throughput timer count for the corresponding
              ds3g subs id

DEPENDENCIES  None

RETURN VALUE None

SIDE EFFECTS  None
===========================================================================*/
uint64 ds3gtputmgr_get_uldl_throughput_timer_count
(
  ds3gtputmgr_throughput_report_client_e_type  client,
  ds3gsubsmgr_subs_id_e_type ds3g_subs_id
);

/*==============================================================================
FUNCTION      DS3GTPUTMGR_UPDATE_TPUT_ULDL_TIMER

DESCRIPTION   This functions is invoked by modehandler in wcdma and lte mode 
              during call transition. This inturn will clear the timer and inform
              lower layer if needed/ start the timer and inform the upper layer
 
DEPENDENCIES  None 
 
RETURN VALUE  None
===========================================================================*/
void ds3gtputmgr_update_tput_uldl_timer
(
   ds3gsubsmgr_subs_id_e_type ds3g_subs_id
);
/*===========================================================================
FUNCTION      DS3GTPUTMGR_REPORT_ULDL_THROUGHPUT_REQUEST

DESCRIPTION   This function invokes the mode handlers to request the report for 
              the uplink downlink achievable data rate and allowed data rate
              during the current sampling interval

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputmgr_report_uldl_throughput_request
(
  ds3gsubsmgr_subs_id_e_type  ds3g_subs_id,
  uint16                      timer_expiry_count
);

/*===========================================================================
FUNCTION      DS3GTPUTMGR_REPORT_ULDL_THROUGHPUT_RESPONSE

DESCRIPTION   This function is invoked by modehandler to report the achievable 
              and allowed  data rate from lte rats to ps framework

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/

void  ds3gtputmgr_report_uldl_throughput_response
(
  ds3gsubsmgr_subs_id_e_type  ds3g_subs_id,
  uint32  achievable_ul_data_rate,
  uint32  achievable_dl_data_rate
);


/*===========================================================================
FUNCTION      DS3GTPUTMGR_REPORT_ULDL_CONFIGURED_THROUGHPUT_RESPONSE

DESCRIPTION   This function invokes the mode handlers to request the report for 
              the uplink downlink data rate and allowed data rate during the current
              sampling interval

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void  ds3gtputmgr_report_uldl_configured_throughput_response
(
  ds3gsubsmgr_subs_id_e_type  ds3g_subs_id,
  uint32  uplink_configured_kbpse,
  uint32  downlink_configured_kbps
);
#ifdef FEATURE_DATA_RAVE_SUPPORT 
/*===========================================================================
FUNCTION      DS3GTPUTMGR_REPORT_UPLINK_THROUGHPUT_RESPONSE

DESCRIPTION   This function is invoked to report the uplink data rate 
              and confidence for wcdma/lte rats to ps framework


DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void  ds3gtputmgr_report_uplink_throughput_response
(
  ds3gsubsmgr_subs_id_e_type  ds3g_subs_id
);

/*===========================================================================
FUNCTION      DS3GTPUTMGR_REPORT_UPLINK_THROUGHPUT_STATUS_RESPONSE_TIMER_BASED

DESCRIPTION   This function is invoked to report the uplink data rate 
              and confidence for wcdma/lte rats to ps framework


DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void  ds3gtputmgr_report_uplink_throughput_status_response_timer_based
(
  ds3gsubsmgr_subs_id_e_type                ds3g_subs_id
);

ds3gtputmgr_est_reporting_status_e_type ds3gtputmgr_current_uplink_status
(
   ds3gsubsmgr_subs_id_e_type                           ds3g_subs_id
);
#endif

#ifdef FEATURE_DATA_RAVE_SUPPORT 
/*===========================================================================
FUNCTION      DS3GTPUTMGR_STOP_UPLINK_THROUGHPUT_TIMER

DESCRIPTION   This function stops the uldl timer and resets other statistics 
              related to uldl for each subs_id 

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputmgr_stop_uplink_throughput_timer
(
   ds3g_timer_enum_type           timer_id,
   ds3gsubsmgr_subs_id_e_type   ds3g_subs_id
);
#endif
/*===========================================================================
FUNCTION      DS3GTPUTMGR_SET_ACTUAL_DATARATES

DESCRIPTION   This function is invoked to set the actual uplink/downlink 
              data rates  for  each subscription


DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputmgr_set_actual_datarates
(
  ds3gsubsmgr_subs_id_e_type                   ds3g_subs_id,
  uint32                                       average_ul_data_rate,
  uint32                                       average_dl_data_rate,
  ds3gtputmgr_throughput_report_client_e_type  client
);

/*===========================================================================
FUNCTION      DS3GTPUTMGR_GET_ACTUAL_DATARATES

DESCRIPTION   This function is invoked to get the actual uplink/downlink
              data rates  for  each subscription


DEPENDENCIES  None

RETURN VALUE  uint32 actual data rate

SIDE EFFECTS  None
===========================================================================*/
uint32 ds3gtputmgr_get_actual_datarates
(
   ds3gsubsmgr_subs_id_e_type   ds3g_subs_id,
  ds3gtputmgr_throughput_report_client_e_type  client,
  ds3gtputmgr_throughput_report_dir_e_type     direction 
);

/*===========================================================================
FUNCTION      DS3GTPUTMGR_SET_ULDL_THROUGHPUT_FREQUENCY

DESCRIPTION   This function configures frequency to generate 
              uldl throughput information Ind periodically. 

DEPENDENCIES  None

RETURN VALUE  TRUE  - If Throughput Info Ind interval has been set for 
                      the Subscription ID
              FALSE - Otherwise
 
SIDE EFFECTS  None
===========================================================================*/
boolean ds3gtputmgr_set_uldl_throughput_frequency
(
  ds3gtputmgr_throughput_report_client_e_type  client,
  ds3gsubsmgr_subs_id_e_type ds3g_subs_id,
  uint32 frequency
);

/*===========================================================================
FUNCTION DS3GTPUTMGR_START_DL_TPUT_EST_TIMER

DESCRIPTION
  This function starts reporting timer for that subscriptions

  
PARAMETERS
  subs_id - Subscription

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/ 
void ds3gtputmgr_start_dl_tput_est_timer
(
   sys_modem_as_id_e_type subs_id
);


/*===========================================================================
FUNCTION DS3G_STOP_DL_TPUT_EST_TIMER

DESCRIPTION
  This function stops reporting timer for that subscriptions

  
PARAMETERS
  subs_id - Subscription

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/ 
void ds3g_stop_dl_tput_est_timer
(
   sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION      DS3GTPUTMGR_SET_EST_DL_TPUT_REPORT_STATUS

DESCRIPTION   This function sets the reporting status for the subs_id
              

DEPENDENCIES  None

RETURN VALUE  None
 
SIDE EFFECTS  None
===========================================================================*/
void ds3gtputmgr_set_est_dl_tput_report_status
(
  ds3gsubsmgr_subs_id_e_type ds3g_subs_id,
  ds3gtputmgr_est_reporting_status_e_type report_status
);

/*===========================================================================
FUNCTION      DS3GTPUTMGR_POST_REPORT_STATUS_IND_TIMER_BASED

DESCRIPTION   This function posts the reporting status indication to PS 
              framework for subs_id

DEPENDENCIES  None

RETURN VALUE  None
 
SIDE EFFECTS  None
===========================================================================*/
void ds3gtputmgr_post_report_status_ind_timer_based
(
  ds3gsubsmgr_subs_id_e_type ds3g_subs_id
);

/*===========================================================================
FUNCTION      DS3GTPUTMGR_SET_EST_DL_TPUT_ACTUAL_INTERVAL

DESCRIPTION   This function sets actual reporting inveral if that's different
              from what client requested
 
DEPENDENCIES  None

RETURN VALUE  None
 
SIDE EFFECTS  None
===========================================================================*/
void ds3gtputmgr_set_est_dl_tput_actual_interval
(
  ds3gsubsmgr_subs_id_e_type ds3g_subs_id,
  uint32 report_interval
);

/*===========================================================================
FUNCTION      DS3GTPUTMGR_GET_EST_DL_TPUT_ACTUAL_INTERVAL

DESCRIPTION   This function sets actual reporting inveral if that's different
              from what client requested
 
DEPENDENCIES  None

RETURN VALUE  None
 
SIDE EFFECTS  None
===========================================================================*/
uint32  ds3gtputmgr_get_est_dl_tput_actual_interval
(
  ds3gsubsmgr_subs_id_e_type ds3g_subs_id
);


/*===========================================================================
FUNCTION DS3G_IS_DL_TPUT_EST_TIMER_RUNNING

DESCRIPTION
  This function checks if reporting timer is running for that subscriptions

  
PARAMETERS
  subs_id - Subscription

DEPENDENCIES
  None.

RETURN VALUE
  boolean - TRUE is timer is running, FALSE otherwise

SIDE EFFECTS
  None.
===========================================================================*/ 
boolean ds3g_is_dl_tput_est_timer_running
(
   sys_modem_as_id_e_type subs_id
);


/*===========================================================================
FUNCTION DS3GTPUTMGR_CHECK_AND_UPDATE_DL_TPUT_REPORTING_TIMER

DESCRIPTION
  This function checks if reporting timer is needed to started or stoppedd based
  on technology and user preference

  
PARAMETERS
  subs_id - Subscription
  reporting_frequency - client reported reporting frequency
  need_to_start - whether reporting timer is actually needed or not

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/ 
void ds3gtputmgr_check_and_update_dl_tput_reporting_timer
(
   sys_modem_as_id_e_type subs_id,
   boolean need_to_report
);

/*==============================================================================
FUNCTION      DS3GTPUTMGR_UPDATE_DL_TPUT_UL_ACHIEVABLE_INFO_IND_INTERVAL

DESCRIPTION   This is a function registered to process the reporting interval
              command which is used to start uplink and downlink (a/b/c) reporting

 
DEPENDENCIES  None 
 
RETURN VALUE  None

===========================================================================*/
void ds3g_process_reporting_interval_cmd
( 
   const ds_cmd_type  *cmd_ptr
);


/*===========================================================================
FUNCTION DS3G_POST_DOWNLINK_THROUGHPUT_ESTIMATION_IND

DESCRIPTION
  This function posts the downlink throughput estimation to PS framework

  
PARAMETERS
  subs_id - Subscription
  ps_sys_dl_throughput_info_type - dl_tput_info

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/ 
void ds3g_post_downlink_throughput_estimation_ind
(
   ps_sys_dl_throughput_info_type dl_tput_info,
   sys_modem_as_id_e_type subs_id
);


/*===========================================================================
FUNCTION ds3gtputmgr_process_downlink_throughput_estimation_ind

DESCRIPTION
  This function posts the downlink throughput estimation to PS framework

  
PARAMETERS
  sys_modem_as_id_e_type subs_id
  uint32 data_rate
  uint8 confidence_level
  boolean freeze_flag
  boolean moved_out

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/ 
void ds3gtputmgr_process_downlink_throughput_estimation_ind
(
   sys_modem_as_id_e_type subs_id,
   uint32 data_rate,
   uint8 confidence_level,
   boolean freeze_flag,
   boolean moved_out
);


/*===========================================================================
FUNCTION      DS3G_PROCESS_ULDL_TIMER_CMD

DESCRIPTION   This function is called to process the uplink downlink timer 
              command 

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3g_process_uldl_timer_cmd
(
  const ds_cmd_type  *cmd_ptr
);


/*===========================================================================
FUNCTION      DS3G_PROCESS_DLTPUT_TIMER_CMD

DESCRIPTION   This function is called to process the downlink throughput timer 
              command 

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3g_process_dltput_timer_cmd
(
  const ds_cmd_type  *cmd_ptr
);

/*===========================================================================
FUNCTION      DS3GTPUTMGR_TIMER_CB

DESCRIPTION   This callback function isregistered with timer to
              notify of timer expirations. This function posts a command to
              DS task upon timer expiry with the timer ID for the expired timer

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputmgr_timer_cb
(
  unsigned long  timer_id_per_subs
);

/*===========================================================================
FUNCTION      ds3gtputmgr_use_timer_based_reporting

DESCRIPTION   This function checks if interval based report is enabled on
              the given subs_id and signal direction.

PARAMETERS    
ds3g_subs_id: subscription ID
direction: threshold based reporting direction
 
DEPENDENCIES  None.

RETURN VALUE 
TRUE: use timer based throughput reporting
FALSE: not use timer based throughput reporting

SIDE EFFECTS  None.
===========================================================================*/
boolean ds3gtputmgr_use_timer_based_reporting
(
  ds3gsubsmgr_subs_id_e_type           ds3g_subs_id,
  ds3gtputthres_tput_direction_e_type  direction
);

/*===========================================================================
FUNCTION      ds3gtputmgr_use_threshold_based_reporting

DESCRIPTION   This function checks if threshold based report is enabled on
              the given subs_id and signal direction.

PARAMETERS    
ds3g_subs_id: subscription ID
direction: threshold based reporting direction
 
DEPENDENCIES  None.

RETURN VALUE 
TRUE: use threshold based throughput reporting
FALSE: not use threshold based throughput reporting

SIDE EFFECTS  None.
===========================================================================*/
boolean ds3gtputmgr_use_threshold_based_reporting
(
  ds3gsubsmgr_subs_id_e_type           ds3g_subs_id,
  ds3gtputthres_tput_direction_e_type  direction
);

/*===========================================================================
FUNCTION      DS3GTPUTMGR_GET_DOWNLINK_STATUS

DESCRIPTION   This function gets the downlink reporting status. 
 
PARAMETERS    
ds3g_subs_id: subscription ID
 
DEPENDENCIES  None

RETURN VALUE 
downlink status 

SIDE EFFECTS  None
===========================================================================*/
ds3gtputmgr_est_reporting_status_e_type ds3gtputmgr_get_downlink_status
(
  ds3gsubsmgr_subs_id_e_type   ds3g_subs_id
);

/*===========================================================================
FUNCTION      DS3GTPUTMGR_GET_TPUT_REPORT_METHOD

DESCRIPTION   This function gets current throughput reporting method
 
PARAMETERS    
ds3g_subs_id: subscription ID
direction: threshold based reporting direction 
 
DEPENDENCIES  None

RETURN VALUE 
throughput reporting method 

SIDE EFFECTS  None
===========================================================================*/
ds3gtputmgr_tput_report_method_e_type ds3gtputmgr_get_tput_report_method
(
  ds3gsubsmgr_subs_id_e_type          ds3g_subs_id,
  ds3gtputthres_tput_direction_e_type direction  
);

/*===========================================================================
FUNCTION      DS3GTPUTMGR_SET_TPUT_REPORT_METHOD

DESCRIPTION   This function sets current throughput reporting method
 
PARAMETERS    
ds3g_subs_id: subscription ID
direction: threshold based reporting direction 
method: throughput reporting method 
 
DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputmgr_set_tput_report_method
(
  ds3gsubsmgr_subs_id_e_type             ds3g_subs_id,
  ds3gtputthres_tput_direction_e_type    direction,
  ds3gtputmgr_tput_report_method_e_type  method
);

/*===========================================================================
FUNCTION      DS3GTPUTMGR_PROCESS_DS_CMD

DESCRIPTION   This function processes DS command
 
PARAMETERS 
 cmd_ptr: command pointer
 
DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputmgr_process_ds_cmd
(
  const ds_cmd_type  *cmd_ptr
);

/*==============================================================================
FUNCTION      DS3GTPUTMGR_UPDATE_TIMER_BASED_UPLINK_THROUGHPUT_CONFIGURE

DESCRIPTION   This functions updates lower layer about timer based uplink 
              throughput configure.
 
PARAMETERS    
ds3g_subs_id: subscription ID
any_lte_bearer_up: if ant LTE bearer is up 
any_wcdma_bearer_up: if ant WCDMA bearer is up 
 
DEPENDENCIES  None

RETURN VALUE  None 
 
SIDE EFFECTS  None 
===========================================================================*/
void ds3gtputmgr_update_timer_based_uplink_throughput_configure
(
  ds3gsubsmgr_subs_id_e_type  ds3g_subs_id,
  boolean                     any_lte_bearer_up,
  boolean                     any_wcdma_bearer_up
);

/*==============================================================================
FUNCTION      DS3GTPUTMGR_UPDATE_THRES_BASED_UPLINK_THROUGHPUT_CONFIGURE

DESCRIPTION   This functions updates lower layer about threshold based uplink 
              throughput configure.

PARAMETERS    
ds3g_subs_id: subscription ID
any_lte_bearer_up: if ant LTE bearer is up 
any_wcdma_bearer_up: if ant WCDMA bearer is up 
 
DEPENDENCIES  None

RETURN VALUE  None 
 
SIDE EFFECTS  None 
===========================================================================*/
void ds3gtputmgr_update_thres_based_uplink_throughput_configure
(
  ds3gsubsmgr_subs_id_e_type  ds3g_subs_id,
  boolean                     any_lte_bearer_up,
  boolean                     any_wcdma_bearer_up
);

/*==============================================================================
FUNCTION      DS3GTPUTMGR_UPDATE_UPLINK_THROUGHPUT_CONFIGURE_REPORTING_TIMER

DESCRIPTION   This functions updates reporting timer cache

PARAMETERS    
ds3g_subs_id: subscription ID
reporting_timer: reporting timer from lower layer
actual_reporting_timer: actual reporting timer from client
 
DEPENDENCIES  None

RETURN VALUE  None 
 
SIDE EFFECTS  None 
===========================================================================*/
void ds3gtputmgr_update_uplink_throughput_configure_reporting_timer
(
  ds3gsubsmgr_subs_id_e_type  ds3g_subs_id,
  uint32                      reporting_timer,
  uint32                      actual_reporting_timer
);

/*===========================================================================
FUNCTION DS3GTPUTMGR_GET_W2L_REDIR_T1

DESCRIPTION
  This function gets the T1: the minimum timer (in secs) that UE must be in
  throughput limit to trigger searching for LTE cells.
 
PARAMETERS
  subs_id: Subscription ID
  
DEPENDENCIES
  None.

RETURN VALUE  
  uint32 timer value

SIDE EFFECTS
  None

===========================================================================*/
uint32 ds3gtputmgr_get_w2l_redir_t1
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION DS3GTPUTMGR_GET_W2L_REDIR_THRESHOLD

DESCRIPTION
  This function gets the Threshold: throughput limit(in Kbps)that can be used
  to trigger searching for LTE cells.
 
PARAMETERS
  subs_id: Subscription ID
  
DEPENDENCIES
  None.

RETURN VALUE  
  uint32 threshold limit

SIDE EFFECTS
  None

===========================================================================*/
uint32 ds3gtputmgr_get_w2l_redir_threshold
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION      DS3GTPUTMGR_SET_THROUGHPUT_REPORT_STATUS

DESCRIPTION   This function set report status per client and subs ID.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputmgr_set_throughput_report_status
(
  ds3gtputmgr_throughput_report_client_e_type  client,
  ds3gsubsmgr_subs_id_e_type                   ds3g_subs_id,
  boolean                                      need_to_report
);

/*===========================================================================
FUNCTION      DS3GTPUTMGR_THROUGHPUT_REPORT_IS_NEEDED

DESCRIPTION   This function gets report status per client and subs ID.

DEPENDENCIES  None

RETURN VALUE   
  TRUE: throughput report for this client is needed
  FALSE: throughput report for this client is not needed

SIDE EFFECTS  None
===========================================================================*/
boolean ds3gtputmgr_throughput_report_is_needed
(
  ds3gtputmgr_throughput_report_client_e_type  client,
  ds3gsubsmgr_subs_id_e_type                   ds3g_subs_id
);

/*===========================================================================
FUNCTION      DS3GTPUTMGR_DOWNLINK_THROUGHPUT_REPORTING_CRITERIA_IS_MET

DESCRIPTION   This function checks if all criteria are met for downlink 
              throughput reporting for WCDMA RRC.

DEPENDENCIES  None

RETURN VALUE 
  TRUE: All criteria are met
  FALSE: Not all criteria are met

SIDE EFFECTS  None
===========================================================================*/
boolean ds3gtputmgr_downlink_throughput_reporting_criteria_is_met
( 
  ds3gsubsmgr_subs_id_e_type  ds3g_subs_id
);

/*===========================================================================
FUNCTION      DS3GTPUTMGR_START_ULDL_THROUGHPUT_TIMER

DESCRIPTION   This function starts the uldl timer if it is stopped

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputmgr_start_uldl_throughput_timer
(
   ds3gsubsmgr_subs_id_e_type  ds3g_subs_id
);

/*===========================================================================
FUNCTION      DS3GTPUTMGR_STOP_ULDL_THROUGHPUT_TIMER

DESCRIPTION   This function stops the uldl timer and resets other statistics
              related to uldl for each subs_id

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputmgr_stop_uldl_throughput_timer
(
   ds3gsubsmgr_subs_id_e_type  ds3g_subs_id
);

/*===========================================================================
FUNCTION      DS3GTPUTMGR_START_STOP_ULDL_THROUGHPUT_TIMER

DESCRIPTION   This function checks the conditions to decide whether starts 
              or stops the uldl throughput reporting timer 

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputmgr_start_stop_uldl_throughput_timer
(
   ds3gsubsmgr_subs_id_e_type  ds3g_subs_id
);

/*===========================================================================
FUNCTION      DS3GTPUTMGR_SET_W_RRC_THROUGHPUT_REPORT_FREQUENCY

DESCRIPTION   This function sets the reporting frequency for W RRC

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputmgr_set_w_rrc_throughput_report_frequency
( 
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION      DS3GTPUTMGR_DOWNLINK_THROUGHPUT_REPORT_HDLR

DESCRIPTION   This function calls the function to update the info for throughput 
              reporting to W RRC.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputmgr_downlink_throughput_report_hdlr
( 
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION      DS3GTPUTMGR_DOWNLINK_THROUGHPUT_REPORT_TO_W_RRC

DESCRIPTION   This function reports downlink actual thoughput to W RRC.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputmgr_downlink_throughput_report_to_w_rrc
( 
  ds3gsubsmgr_subs_id_e_type  ds3g_subs_id
);

/*===========================================================================
FUNCTION      DS3GTPUTMGR_REPORT_THROUGHPUT_TO_CLIENT

DESCRIPTION   This function reports throughput to each client

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtputmgr_report_throughput_to_client
(
  ds3gsubsmgr_subs_id_e_type                   ds3g_subs_id,
  ds3gtputmgr_throughput_report_client_e_type  client_index
);

#endif /* DS3GTPUTMGR_H */

