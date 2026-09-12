#ifndef TLE_DM_EVENT_H
#define TLE_DM_EVENT_H

/*===========================================================================

              TLE DM Event Header File.

DESCRIPTION
   This file contains DM Event types used internally by the TLE.

  
  Copyright (c) 2010 - 2012 Qualcomm Technologies Incorporated.
  Qualcomm Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2014 Qualcomm Atheros, Inc.
  Qualcomm Atheros Confidential and Proprietary. All Rights Reserved. 

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/tle/1_0/common/inc/tle_dm_event.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/22/10   atien   Initial version
===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"    /* Definition for basic types and macros */
#include "target.h"    /* Target configuration file */
#include "customer.h"  /* Customer configuration file */
#include "event_defs.h"
#include "tle_common.h"
#include "tle_api.h"

/*===========================================================================

                     DEBUGGING RELATED DEFINITIONS 
                      (MUST DELETE THEM)

*===========================================================================*/



/*===========================================================================

                     DEFINES FOR MODULE

===========================================================================*/



/*==========================================================================
                     
                     MACRO DEFINITIONS
                     
===========================================================================*/

                     
                     
/*===========================================================================

                     DATA TYPE DEFINITIONS FOR MODULE

                        !!!!!!! ATTENTION !!!!!!!!
Size/Order of data fields in the following data types should not
be changed as they are synchronized with external tool software.
===========================================================================*/

#ifdef FEATURE_CGPS_PLAYBACK
#pragma pack(push)
#pragma pack(1)
#endif

typedef PACKED struct PACKED_POST 
{
  uint32 q_StartGpsMsec;
  uint16 w_StartGpsWk;
  uint16 w_GtpSessID;
  uint8  u_Source; /* 0: Periodic time out with threshold check, 
                      1: Periodic time out no thresholdcheck (every 8 hours)
                      2: Upload caused by visited memory pool exhaust */ 
} tlm_GtpSessStartEventType;

typedef PACKED struct PACKED_POST 
{
  uint32 q_EndGpsMsec;
  uint16 w_EndGpsWk;
  uint16 w_GtpSessID;
  uint8  u_EndStatus; /* 0: SUCCESS
                         1: Abort, Download GTP session started
                         2: Abort, ACK time out */  
} tle_GtpSessEndEventType;

typedef PACKED struct PACKED_POST
{
  uint32 q_TdpMeasEnableGpsMsec;
  uint16 w_TdpMeasEnableGpsWk;
} tle_TdpMeasEnableEventType;

typedef PACKED struct PACKED_POST
{
  uint32 q_TdpMeasDisableGpsMsec;
  uint16 w_TdpMeasDisableGpsWk;
  uint8  u_Mask; /* Bit 0: Memory full
                    Bit 1: No valid GNSS fix 
                    Bit 2: Global upload disabled
                    Bit 4: TDP not enabled
                    Bit 3: Serving cell belongs to opted out network */
} tle_TdpMeasDisableEventType;

typedef PACKED struct PACKED_POST
{
  uint32 q_TdpMeasRequestGpsMsec;
  uint16 w_TdpMeasRequestGpsWk;
} tle_TdpMeasRequestEventType;

typedef PACKED struct PACKED_POST
{
  uint32 q_TdpMeasRcvdGpsMsec;
  uint16 w_TdpMeasRcvdGpsWk;
} tle_TdpMeasRcvdEventType;

/* Position report Event */
typedef  PACKED struct PACKED_POST
{
  uint8  u_Version;
  uint16 w_GpsWeek;
  uint32 q_GpsMsec;      
} tle_PositionReportEventType;

#ifdef FEATURE_CGPS_PLAYBACK
#pragma pack(pop)
#endif

typedef union
{
  tlm_GtpSessStartEventType   z_TdpSessStartEvent;
  tle_GtpSessEndEventType     z_TdpSessEndEvent;
  tle_TdpMeasEnableEventType  z_TdpMeasEnableEvent;
  tle_TdpMeasDisableEventType z_TdpMeasDisableEvent;
  tle_TdpMeasRequestEventType z_TdpMeasRequestEvent;
  tle_TdpMeasRcvdEventType    z_TdpMeasRcvdEvent;
} tle_TdpEventUnionType;

/*===========================================================================

FUNCTION tle_report_event

DESCRIPTION
  Report event: GPS XTRA-T generic event report without payload.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void tle_report_event(event_id_enum_type e_event);


/*===========================================================================

FUNCTION tle_report_event_xtra_t_session_begin

DESCRIPTION
  Report event: GPS XTRA-T Session initiated

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void tle_report_event_xtra_t_session_begin (tle_session_source_e_type e_req_source);


/*===========================================================================

FUNCTION  tle_report_event_xtra_t_session_done

DESCRIPTION
  Report event: GPS XTRA-T Session terminates normally.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void tle_report_event_xtra_t_session_done (tle_session_source_e_type  e_req_source);


/*===========================================================================

FUNCTION tle_report_event_xtra_t_session_end

DESCRIPTION
  Report event: GPS XTRA-T Session termination reason

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void tle_report_event_xtra_t_session_end(tle_sess_end_e_type reason);

/*===========================================================================

FUNCTION tle_report_event_conn_failure

DESCRIPTION
  Report event: GPS XTRA-T Connection Failure with reason code
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void tle_report_event_conn_failure(tle_sess_end_e_type *p_end_reason);

/*===========================================================================

FUNCTION tle_report_event_conn_type

DESCRIPTION
  Report event: GPS XTRA-T Connection Type
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void tle_report_event_conn_type(tle_conn_e_type *p_conn_type);


/*===========================================================================

FUNCTION
  tle_report_tdp_event

DESCRIPTION
  This functions reports and logs a TDP related event

PARAMETERS
  e_TdpEvent - The event to be reported
  (input)
 
  pz_Event  - Points to the event data to be reported
  (input)

DEPENDENCIES
  None

RETURN VALUE
  TRUE  - If the event is generated successfuly
  FALSE - Otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean tle_report_tdp_event(event_id_enum_type e_TdpEvent,
                             tle_TdpEventUnionType * pz_Event);
#endif /* TLE_DM_EVENT_H */
