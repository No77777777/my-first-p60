
#ifndef TM_DTF_H_
#define TM_DTF_H_
#include"tm_qsh_iface.h"

#include "gps_variation.h"
#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================

                  TM_DTF Header File

DESCRIPTION
  This header file contains definitions of data structure necessary interface between tm and qsh

  Copyright (c) 2000 - 2012 Qualcomm Technologies Incorporated.
  Qualcomm Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2015 Qualcomm Atheros, Inc.
  Qualcomm Atheros Confidential and Proprietary. All Rights Reserved. 
  Copyright (c) 2013 - 2018 Qualcomm Technologies, Inc. 
  Qualcomm Technologies Confidential and Proprietary. All Rights Reserved.
Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: 

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
08/13/18    py      Added DTF implementation for keeping same NAS ID in all LPP CP session
08/10/18    sum     Added DTF implementation for Meas report issue
05/30/17    kpn     Nascent version to check the functionality of DTF for SM
12/07/17    ap      Code to test SRVCC handover cross case
===========================================================================*/

#define TM_QSH_DTF_EVENTS_OFFSET TM_QSH_EVENT_MAX + 1  /* This is the offset fpr DTF events*/
#define TM_QSH_DTF_EVENT_NOTIFY_OFFSET TM_QSH_EVENT_NOTIFY_MAX + 1/* This is the offset fpr DTF event handlers*/


typedef enum
{
  TM_DTF_EVENT_GET_POS_ONGOING = TM_QSH_DTF_EVENTS_OFFSET,    /*First event*/
  TM_DTF_EVENT_FIRST_OTDOA_SESSION_OVER,
  TM_DTF_EVENT_GET_PAD_RECEIVED,
  TM_DTF_EVENT_MEAS_STARTED,
  TM_DTF_EVENT_MAX                 /* (NA)   For Internal bound checking only */
}tm_dtf_event_e;

typedef enum
{
  TM_DTF_EVENT_NOTIFY_START_GET_POS = TM_QSH_DTF_EVENT_NOTIFY_OFFSET,      /*First event*/
  TM_DTF_EVENT_NOTIFY_FIRST_OTDOA_SESSION_OVER,
  TM_DTF_EVENT_NOTIFY_PAD_RECEIVED,
  TM_DTF_EVENT_NOTIFY_MEAS_STARTED,
  TM_DTF_EVENT_NOTIFY_MAX                 /* (NA)   For Internal bound checking only */
}tm_dtf_event_notify_e;

typedef struct
{
  boolean status;
}tm_dtf_get_pos_ongoing;
typedef struct
{
   boolean status;
}tm_dtf_get_pos_pad_received;
typedef struct
{
   boolean status;
}tm_dtf_meas_started;

typedef struct
{
  boolean status;
}tm_dtf_get_lpp_cp_sess_abort;

typedef struct
{
  boolean status;
}tm_dtf_first_otdoa_session_over;

typedef struct
{
  tm_dtf_get_pos_ongoing pos_ongoing;
  tm_dtf_first_otdoa_session_over first_otdoa_over;
  tm_dtf_get_pos_pad_received pad_received;
  tm_dtf_meas_started meas_started;
} tm_dtf_events;

extern tm_dtf_events tm_dtf_events_db;

typedef struct
{
  uint8 sess_timeout;
}tm_dtf_start_get_pos;

typedef struct
{
  uint8 nas_sess_timeout;
}tm_dtf_update_NAS_timer;


typedef struct
{
   uint8 sess_timeout;
}tm_dtf_send_handover_complete;

typedef struct
{
  tm_dtf_start_get_pos start_get_pos;
  tm_dtf_send_handover_complete send_ho_complete;
  tm_dtf_update_NAS_timer handle_nas_ses_timeout;
}tm_dtf_handlers;
extern boolean b_tm_dtf_srvcc_ho_test_triggered;
extern tm_dtf_handlers tm_dtf_handlers_db;

#endif
