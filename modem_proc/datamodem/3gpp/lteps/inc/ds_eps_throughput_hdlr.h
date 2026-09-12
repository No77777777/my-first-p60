/*!
  @file
  ds_eps_throughput_hdlr.c

  @brief
  REQUIRED brief one-sentence description of this C module.

  @detail
  OPTIONAL detailed description of this C module.

*/


#ifndef __DS_EPS_TPUT_HDLR_H__
#define __DS_EPS_TPUT_HDLR_H__

/*===========================================================================

  Copyright (c) 2009-2015 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

===========================================================================*/


/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/data.mpss/3.5..2.1/3gpp/lteps/inc/ds_eps_throughput_hdlr.h#3 $

when       who     what, where, why
--------   ---     -------------------------------------------------------------

==============================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "datamodem_variation.h"

#ifdef FEATURE_DATA_LTE
#include "ds_eps_bearer_context.h"
#include "ds_3gpp_msg.h"
#include "ds3gtimer.h"
#include "ds3gtputmgr.h"
#include "dsmsgr.h"
#include "lte_cphy_msg.h"
#ifdef FEATURE_DATA_RAVE_SUPPORT 
#include "lte_mac_ind.h"
#include "lte_mac_msg.h"
#endif
#include "ps_sys.h"

#include "log.h"
#include "log_codes.h"

/*===========================================================================

                   INTERNAL DEFINITIONS AND TYPES

===========================================================================*/
#define DS_LTE_DL_TPUT_T_ACCUMULATE_DEFAULT_VALUE 512 /* in Milliseconds*/
#define DS_LTE_DL_TPUT_T_ACCUMULATE_MIN_VALUE 10 /* in Milliseconds*/
#define DS_LTE_DL_TPUT_T_ACCUMULATE_MAX_VALUE 1000 /* in Milliseconds*/
#define DS_EPS_SEC_TO_MS_MULTIPLIER 1000
#define DS_EPS_KBPS_TO_BPS_MULTIPLIER 1000
#define DS_EPS_KILOBIT_TO_BYTES 125

#define DS_EPS_MAX_UL_CONF_KBPS  200000
/*capping uplink data rate to 200Mbps*/

#define DS_EPS_MAX_DL_TPUT_CONFIDENCE_LEVEL 1000
#define DS_EPS_MIN_DL_TPUT_CONFIDENCE_LEVEL 0
#define DS_EPS_MID_DL_TPUT_CONFIDENCE_LEVEL 500

#define DS_EPS_MAX_DL_TPUT_ALPHA 1000000
#define DS_EPS_MIN_DL_TPUT_ALPHA 1


#define DS_EPS_MAX_UL_TPUT_ALPHA 1000000


#define DS_EPS_DL_TPUT_LOG_VERSION 1

/*Describe*/
typedef enum
{
   DS_EPS_DL_TPUT_ESTM_STATUS_STOP_WAIT_FOR_ML1,
   DS_EPS_DL_TPUT_ESTM_STATUS_STOP,
   DS_EPS_DL_TPUT_ESTM_STATUS_START_WAIT_FOR_ML1,
   DS_EPS_DL_TPUT_ESTM_STATUS_START
}ds_eps_dl_tput_estm_status_type_e;


typedef struct
{
   uint32               ul_configured_kbps;
   uint32               dl_configured_kbps; 
  uint16               uplink_scaling_factor;
}ds_eps_configured_throughput_estimation_s;


typedef struct
{
   uint16 ds_eps_filtered_confidence_level;
   uint64 ds_eps_filtered_dl_throughput_est;
   uint64 time_since_last_estimatation;
   ds_eps_dl_tput_estm_status_type_e status;
   boolean ds_eps_ignore_historical_data;
   uint32 ds_eps_running_t_accumuate;
   boolean ds_eps_is_ml1_active;
   boolean ds_eps_is_first_report;
} ds_eps_downlink_throughput_estimation_s;


typedef struct
{
   uint8  ul_confidence_level;
   uint32 ul_throughput_est;
   uint32 taccumuate;
   uint32 treporting;
   uint32 l2reporting;
   uint32 alpha;
} ds_eps_uplink_throughput_estimation_s;

/*===========================================================================
  Log Code: LOG_DS_LTE_DL_TPUT_C

  Revision: 1

  Description: @external
               This log packet captures periodic LTE downlink throughput
               information

  Frequency: every 500-1000 ms 

  Size: 29 bytes
===========================================================================*/
LOG_RECORD_DEFINE(LOG_DS_LTE_DL_TPUT_C)
  uint8  pkt_version;               /* version of log packet */
  uint64 time_diff;                 /* ms - Time difference since last calculation */
  uint64 ml1_bytes;                 /* total ml1 bytes given to DS during time frame */
  uint64 pdcp_bytes;                /* total pcdp bytes given to DS during time frame */
  uint8  filtered_confidence_level; /* 1 (low) - 7 (high).  */
  uint32 filtered_avail_tput;       /* Kilobits/second */
LOG_RECORD_END

/*===========================================================================
FUNCTION DS_EPS_SEND_LAST_REPORT_AND_RESET_INFO

DESCRIPTION
  This function sends the report and clears the info. No further information
  will not be stored/reported until LTE is back

  
PARAMETERS
  subs_id - Subscription

DEPENDENCIES
  None.

RETURN VALUE
  boolean - TRUE is message is received correctly

SIDE EFFECTS
  None.
===========================================================================*/ 

void ds_eps_send_last_report_and_reset_info
(
  sys_modem_as_id_e_type subs_id
);



/*===========================================================================
FUNCTION  DS_EPS_DL_TPUT_ESTIMATION_INIT

DESCRIPTION
  This function initializes default values of downlink throughput internal
  structures.


PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void ds_eps_dl_tput_estimation_init
(
   void
);

#ifdef FEATURE_DATA_RAVE_SUPPORT 
/*===========================================================================
FUNCTION  DS_EPS_UL_TPUT_ESTIMATION_INIT

DESCRIPTION
  This function initializes default values of uplink throughput internal
  structures.


PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void ds_eps_ul_tput_estimation_init
(
   void
);
#endif


/*===========================================================================
FUNCTION  DS_EPS_DL_TPUT_ESTIMATION_RESET

DESCRIPTION
  This function resets default values of downlink throughput internal
  structures.


PARAMETERS
  subs_id - Subscription ID to be reset

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void ds_eps_dl_tput_estimation_reset
(
   sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION  DS_EPS_DL_TPUT_PROCESS_ESTIMATION_INDICATION

DESCRIPTION
  This function processes the message that is received from LTE ML1 through
  message router


PARAMETERS
  lte_cphy_dl_tput_estm_report_ind_s - Estimation report message from ML1

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void ds_eps_dl_tput_process_estimation_indication
(
   lte_cphy_dl_tput_estm_report_ind_s *est_report
);


/*===========================================================================
FUNCTION  DS_EPS_CALCULATE_UPDATE_DL_TPUT_ESTIMATION

DESCRIPTION
  This function calculates and updates the filtered values of estimation


PARAMETERS
  subs_id - Subscription ID
  new_byte_count - new byte count ML1 and PDCP combined
  confidence_level - Confidence level from ML1 (0/500/1000)
  time_difference - bwteen new and old report
  alpha - 0-1000 (for preseving decimal)
 

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void ds_eps_calculate_update_dl_tput_estimation
(
  sys_modem_as_id_e_type subs_id,
  uint64 new_byte_count,
  uint32 confidence_level,
  uint64 time_difference,
  uint32 alpha
);


/*===========================================================================
FUNCTION  DS_EPS_GET_CURRENT_TPUT_VALUES

DESCRIPTION
  This function prepares the current estimation report in PS format


PARAMETERS
  subs_id - Subscription ID
  ps_sys_dl_throughput_info_type - the PS structure to send the indication
 

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void ds_eps_get_current_tput_values
(
   sys_modem_as_id_e_type subs_id,
   ps_sys_dl_throughput_info_type *dl_tput_info_type
);

/*===========================================================================
FUNCTION  DS_EPS_CHECK_AND_UPDATE_ML1_STATUS

DESCRIPTION
  This function checks the current status of ML1 reporting status and
  sends (start/stop) ML1 request if required

  If report frequency is 0, this function and reporting is going on
  this function will send to stop report request and return FALSE.
  If reporting frequcny is positive and reporting is stop, this function
  will send start request and return TRUE

PARAMETERS
  subs_id - sys_modem_as_id_e_type (Subscription ID)

DEPENDENCIES
  None.

RETURN VALUE
  TRUE is reporting is expected after this, FALSE otherwise

SIDE EFFECTS
  None.

===========================================================================*/

void ds_eps_check_and_update_ml1_status
(
   sys_modem_as_id_e_type subs_id,
   boolean need_to_report,
   boolean update_t_accumulate
);

/*===========================================================================
FUNCTION  DS_EPS_DL_ESTIMATED_THROUGHPUT_SET_RUNNING_T_ACCUMULATE

DESCRIPTION
  This function gets t_accumulate nv value from 3gpp nv manager and provides
  callers


PARAMETERS
  subs_id - Subscription ID
  t_accumulate - t_accumulate to set as running t_accumulate
 

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void ds_eps_dl_estimated_throughput_set_running_t_accumulate
(
   sys_modem_as_id_e_type subs_id,
   uint32 t_accumuate
);


/*===========================================================================
FUNCTION  DS_EPS_DL_ESTIMATED_THROUGHPUT_GET_RUNNING_T_ACCUMULATE

DESCRIPTION
  This function gets t_accumulate nv value from 3gpp nv manager and provides
  callers


PARAMETERS
  subs_id - Subscription ID
 

DEPENDENCIES
  None.

RETURN VALUE
  uint32 - running t_accumulate of the subscription

SIDE EFFECTS
  None.

===========================================================================*/
uint32 ds_eps_dl_estimated_throughput_get_running_t_accumulate
(
   sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION  DS_EPS_DL_ESTIMATED_IS_ML1_ACTIVE

DESCRIPTION
  This function gets ml1_active status


PARAMETERS
  subs_id - Subscription ID
 

DEPENDENCIES
  None.

RETURN VALUE
  1 - if ml1 is active
  0 - otherwise

SIDE EFFECTS
  None.

===========================================================================*/
uint32 ds_eps_dl_estimated_is_ml1_active
(
   sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION  DS_EPS_SET_DL_TPUT_REPORT_FREQUENCY

DESCRIPTION
  This function gets t_accumulate nv value from 3gpp nv manager and provides
  callers


PARAMETERS
  subs_id - Subscription ID
  *t_accumulate - inputer value to be stored pointer
 

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void ds_eps_dl_estimated_throughput_t_accumuate
(
   sys_modem_as_id_e_type subs_id,
   uint32 *t_accumuate
);

/*===========================================================================
FUNCTION DS_EPS_LTE_CPHY_DL_THROUGHPUT_ESTM_IND_HDLR

DESCRIPTION
  This function is used to handle the message from lTE ML1. This message is
  sent every t_accumulate time from ML1 when requested

  
PARAMETERS
  msgr_type - UMID of the message sent
  dsmsg_ptr - Pointer to the payload

DEPENDENCIES
  None.

RETURN VALUE
  boolean - TRUE is message is received correctly

SIDE EFFECTS
  None.
===========================================================================*/ 
boolean ds_eps_lte_cphy_dl_throughput_estm_ind_hdlr
(
  msgr_umid_type             msgr_type,
  sys_modem_as_id_e_type     subs_id,
  const msgr_hdr_struct_type *dsmsg_ptr
);

/*===========================================================================
FUNCTION DS_EPS_LTE_CPHY_DL_THROUGHPUT_REPORT_RSP_HDLR

DESCRIPTION
  This function is used to handle the message from NAS. This message is sent
  whenever T3420 timer expires

  Here we validate the message ID and we update ds_eps_emergency_auth_fail
  
PARAMETERS
  msgr_type - UMID of the message sent
  dsmsg_ptr - Pointer to the payload

DEPENDENCIES
  None.

RETURN VALUE
  boolean - TRUE is message is received correctly

SIDE EFFECTS
  None.
===========================================================================*/ 
boolean ds_eps_lte_cphy_dl_throughput_report_rsp_hdlr
(
  msgr_umid_type             msgr_type,
  sys_modem_as_id_e_type     subs_id,
  const msgr_hdr_struct_type *dsmsg_ptr
);


/*===========================================================================
FUNCTION DS_EPS_DL_TPUT_REPORTING_FREQUENCY_MODIFY_HDLR

DESCRIPTION
  This function is called clients modify the reporting frequency

PARAMETERS
  subs_id - Subscription for which report is being generated
  report_frequency - that is minimul of all clients

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_eps_dl_tput_reporting_frequency_modify_hdlr
(
   sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION      DS_EPS_GET_CURRENT_DL_TPUT_ESTIMATION

DESCRIPTION
  Handler for PS_SYS_IOCTL_3GPP_LTE_DL_THROUGHPUT_ESTIMATION. Retrieves
  LTE DL throuhput estimation values cached in MH

PARAMETERS 
  arg_val_ptr: Argument to be populated
  subs_id:     Subscription id
  ps_errno:    Err number if IOCTL cant be handled
   
  
 
DEPENDENCIES
  None.

RETURN VALUE
  0  - Success
  -1 - Failure
 
SIDE EFFECTS
  None.
===========================================================================*/ 
int16 ds_eps_get_current_dl_tput_estimation
(
  void                    *arg_val_ptr,
  sys_modem_as_id_e_type   subs_id,
  int16                   *ps_errno
);

/*===========================================================================
FUNCTION DS_EPS_DL_TPUT_TIMER_EXPIRE_HDLR

DESCRIPTION
  This function is called when the reporting timer expires and 3GPP MH will
  send filtered report to client through PS

PARAMETERS
  subs_id - Subscription for which report is being generated

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_eps_dl_tput_timer_expire_hdlr
(
   sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_EPS_LTE_CPHY_DL_THROUGHPUT_ESTM_IND_HDLR

DESCRIPTION
  This function is used to handle the message from LTE lowerlayer.This message 
  is received whenver lte updates the configured data rate changes in either
  uplink or downlink direction
  
PARAMETERS
  msgr_type - UMID of the message sent
  dsmsg_ptr - Pointer to the payload

DEPENDENCIES
  None.

RETURN VALUE
  boolean - TRUE is message is received correctly

SIDE EFFECTS
  None.
===========================================================================*/ 
boolean ds_eps_lte_cphy_ind_configured_throughput_hdlr
(
  msgr_umid_type              msgr_type,
  sys_modem_as_id_e_type     subs_id,
  const msgr_hdr_struct_type *dsmsg_ptr
);

/*==============================================================================
FUNCTION  DS_EPS_BEARER_HANDLE_UPLINK_SCALING_INFO

DESCRIPTION
  This function is used to handle the message 
  DS_3GPP_UPLINK_TPUT_SCALING_INFO_IND.MH updates the configured 
  throughput by the scaling factor
  
PARAMETERS
  msgrtype - UMID of the message
  dsmsg    - pointer to the message payload

DEPENDENCIES
  None.

RETURN VALUE
  TRUE is processing was successful
  False otherwise

SIDE EFFECTS
  None.

==============================================================================*/
boolean ds_eps_bearer_handle_uplink_scaling_info
(
  msgr_umid_type             msgrtype,
  sys_modem_as_id_e_type     subs_id,
  const msgr_hdr_struct_type *dsmsg_ptr
);

/*===========================================================================
FUNCTION  DS_EPS_GET_CONFIGURED_UL_DATA_RATE

DESCRIPTION
  This function gets the eps configured ul data rate per subscription


PARAMETERS
  subs_id - Subscription ID
  ps_sys_dl_throughput_info_type - the PS structure to send the indication
 

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
uint32 ds_eps_get_configured_ul_data_rate
(
   sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION  DS_EPS_GET_CONFIGURED_DL_DATA_RATE

DESCRIPTION
  This function gets the eps configured dl data rate per subscription


PARAMETERS
  subs_id - Subscription ID

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
uint32 ds_eps_get_configured_dl_data_rate
(
   sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_EPS_LTE_CPHY_CNF_ACHIEVABLE_THROUGHPUT_HDLR

DESCRIPTION
  This function is used to handle the message from lTE . This message is
  received as a response to the query requesting for lte achievable ul/dl
  data rate

PARAMETERS
  msgr_type - UMID of the message sent
  dsmsg_ptr - Pointer to the payload

DEPENDENCIES
  None.

RETURN VALUE
  boolean - TRUE is message is received correctly

SIDE EFFECTS
  None.
===========================================================================*/ 
boolean ds_eps_lte_cphy_cnf_achievable_throughput_hdlr
(
  msgr_umid_type             msgr_type,
  sys_modem_as_id_e_type     subs_id,
  const msgr_hdr_struct_type *dsmsg_ptr
);

#ifdef FEATURE_DATA_RAVE_SUPPORT 
/*===========================================================================
FUNCTION DS_EPS_LTE_MAC_ULCNF_TPUT_INFO_START

DESCRIPTION
  This function is used to handle the message from lTE . This message is
  received as a response to the query requesting for lte achievable ul/dl
  data rate

PARAMETERS
  msgr_type - UMID of the message sent
  dsmsg_ptr - Pointer to the payload

DEPENDENCIES
  None.

RETURN VALUE
  boolean - TRUE is message is received correctly

SIDE EFFECTS
  None.
===========================================================================*/ 
boolean ds_eps_lte_mac_ulcnf_tput_info_start_cnf
(
  msgr_umid_type             msgr_type,
  sys_modem_as_id_e_type     subs_id,
  const msgr_hdr_struct_type *dsmsg_ptr
);

/*===========================================================================
FUNCTION DS_EPS_LTE_MAC_ULCNF_TPUT_INFO_START

DESCRIPTION
  This function is used to handle the message from lTE . This message is
  received as a response to the query requesting for lte achievable ul/dl
  data rate

PARAMETERS
  msgr_type - UMID of the message sent
  dsmsg_ptr - Pointer to the payload

DEPENDENCIES
  None.

RETURN VALUE
  boolean - TRUE is message is received correctly

SIDE EFFECTS
  None.
===========================================================================*/ 
boolean ds_eps_lte_mac_ul_tput_info
(
  msgr_umid_type             msgr_type,
  sys_modem_as_id_e_type     subs_id,
  const msgr_hdr_struct_type *dsmsg_ptr
);

/*===========================================================================
FUNCTION DS_EPS_START_STOP_UPLINK_THROUGHPUT_REPORTING

DESCRIPTION
  This function is used to sent to lte lower later to start,stop calculating
  uplink data rate

DEPENDENCIES
  None.

RETURN VALUE
  boolean - TRUE is message is sent correctly

SIDE EFFECTS
  None.
===========================================================================*/ 
uint32 ds_eps_start_stop_uplink_throughput_reporting
(
   sys_modem_as_id_e_type subs_id,
   boolean                start_stop,
   uint32                 reporting_timer
);
#endif
/*===========================================================================
FUNCTION DS_EPS_THROUGHPUT_ULDL_START_IND

DESCRIPTION
  This function is used to sent to lte lower later to start calculating uldl
  achievable data rate.  


DEPENDENCIES
  None.

RETURN VALUE
  boolean - TRUE is message is sent correctly

SIDE EFFECTS
  None.
===========================================================================*/ 
boolean ds_eps_througput_uldl_start_ind
(
   sys_modem_as_id_e_type subs_id,
   uint32   timer_value,
   uint32   frequency
);
/*===========================================================================
FUNCTION DS_EPS_THROUGHPUT_ULDL_STOP_IND

DESCRIPTION
  This function is used to sent to lte lower later to stop calculating uldl
  achievable data rate.  

DEPENDENCIES
  None.

RETURN VALUE
  boolean - TRUE is message is sent correctly

SIDE EFFECTS
  None.
===========================================================================*/ 
boolean ds_eps_througput_uldl_stop_ind
(
   sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_EPS_THROUGHPUT_ULDL_STOP_IND

DESCRIPTION
  This function is used to sent to lte lower later to query uldl
  achievable data rate.  

DEPENDENCIES
  None.

RETURN VALUE
  boolean - TRUE is message is sent correctly

SIDE EFFECTS
  None.
===========================================================================*/ 
boolean ds_eps_througput_uldl_query_req
(
   sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_EPS_THROUGHPUT_HDLR_INIT

DESCRIPTION
  This function is used to initialize the 
  eps throughput handler module
  
PARAMETERS

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/ 
void ds_eps_throughput_hdlr_init(void);


/*===========================================================================
FUNCTION DS_EPS_LTE_CPHY_ACHIEVABLE_THROUGHPUT_STATUS_HDLR

DESCRIPTION
  This function is used to handle the message from lTE.This message is
  given by ml1 to provide its connected state.

PARAMETERS
  msgr_type - UMID of the message sent
  dsmsg_ptr - Pointer to the payload

DEPENDENCIES
  None.

RETURN VALUE
  boolean - TRUE is message is received correctly

SIDE EFFECTS
  None.
===========================================================================*/ 
boolean ds_eps_lte_cphy_achievable_throughput_status_hdlr
(
  msgr_umid_type             msgr_type,
  sys_modem_as_id_e_type     subs_id,
  const msgr_hdr_struct_type *dsmsg_ptr
);

/*===========================================================================
FUNCTION DS_EPS_LTE_ML1_IS_CONNECTED

DESCRIPTION
  This function is used to query the ml1 connection status

PARAMETERS
  subs_id

DEPENDENCIES
  None.

RETURN VALUE
  boolean - TRUE if ml1 is connected

SIDE EFFECTS
  None.
===========================================================================*/
boolean ds_eps_lte_ml1_is_connected
(
  sys_modem_as_id_e_type   subs_id 
);



#ifdef TEST_FRAMEWORK
#error code not present
#endif
#endif /* FEATURE_DATA_LTE */

#endif /* __DS_EPS_TPUT_HDLR_H__*/
