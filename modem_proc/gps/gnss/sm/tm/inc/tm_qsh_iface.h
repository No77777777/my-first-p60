#ifndef TM_QSH_IF_H
#define TM_QSH_IF_H

#include "gps_variation.h"
#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================

                  TM_QSH_IF Header File

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
05/30/17   kpn     Nascent version to check the functionality of DTF for SM
===========================================================================*/


#include "qsh_ext.h"
#include "qsh_client.h"

#define SM_TM_QSH_MINOR_VER 0
#define SM_TM_QSH_MAJOR_VER 1

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif /*FEATURE_QSH_EVENT_NOTIFY_HANDLER*/

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
#define TM_QSH_EVENT_CMD_CODE_CFG 0
#define TM_QSH_EVENT_CMD_CODE_EVT_ID 8
#define TM_QSH_EVENT_CMD_CODE_SUB_ID 16
#define TM_QSH_EVENT_CMD_CODE_NUM_WRDS 24
#endif


typedef enum
{
  TM_QSH_EVENT_MIN,
  TM_QSH_EVENT_MAX                 /* (NA)   For Internal bound checking only */
}tm_qsh_event_e;

typedef enum
{
  TM_QSH_EVENT_NOTIFY_MIN,      /*First event*/
  TM_QSH_EVENT_NOTIFY_MAX                 /* (NA)   For Internal bound checking only */
}tm_qsh_event_notify_e;
  
typedef struct
{
} tm_qsh_events; /*Place holder for all the QSH events to be added in future*/

extern tm_qsh_events tm_qsh_events_db;

typedef struct
{
}tm_qsh_handlers;/*Place holder for all the QSH event handlers to be added in future*/
extern tm_qsh_handlers tm_qsh_handlers_db;

/*======================================================================================

FUNCTION tm_qsh_cb

DESCRIPTION
  This function is called by QSH along with an action ID, whenever a QSH event happens.
  This QSH event is one which would have already been configured by the client. When a QSH
  event happens, QSH calls this callback with the relevant data defined by the client.

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS

=========================================================================================*/
void tm_qsh_cb(qsh_client_cb_params_s* qsh_cb_params_ptr);


/*======================================================================================

FUNCTION tm_qsh_report_event

DESCRIPTION
  This function is to report events to QSH from client

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS

=========================================================================================*/
	
void tm_qsh_report_event(sys_modem_as_id_e_type e_as_id, tm_qsh_event_e event_id);
/*===========================================================================

FUNCTION tm_qsh_initialize_events

DESCRIPTION
  This function is to initialize TM-QSH related events

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/

void tm_qsh_initialize_events(void);


/*===========================================================================

FUNCTION tm_qsh_initialize_events

DESCRIPTION
  This function is to initialize TM QSH related data structures.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/

void tm_qsh_init(void);

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif
/*======================================================================================

FUNCTION tm_qsh_event_notify_handler

DESCRIPTION
  This function is to handle the Event Notify from QSH whenever a QSH event happens.
  Client is expected to induce the expected cross scenario

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS

=========================================================================================*/

void tm_qsh_event_notify_handler(qsh_client_cb_params_s* qsh_cb_params_ptr);
/*======================================================================================

FUNCTION tm_qsh_action_cfg_handler

DESCRIPTION
  This function is to handle Action Config called by QSH whenver a cmd is issued via QXDM
  Client is expected to configure the event or handler databases, if any, accordingly.

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS

=========================================================================================*/

void tm_qsh_action_cfg_handler(qsh_client_cb_params_s* qsh_cb_params_ptr);



#ifdef __cplusplus
}
#endif

#endif /* TM_IFACE_H */



