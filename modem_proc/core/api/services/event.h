#ifndef EVENT_H
#define EVENT_H

/*!
@ingroup event_service
@file event.h
@brief 
This file describes the Diagnostics Event Service APIs. Event Services
must be initialized before using the event services apis

*/

/*
Copyright (c) 2000-2010, 2013, 2016, 2019 by Qualcomm Technologies, Incorporated.
All Rights Reserved.

*/

/*===========================================================================
                          Edit History

$Header: //components/rel/core.mpss/10.0/api/services/event.h#1 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
01/04/19   kdey    Added API event_status
09/29/16   ph      Diag and ULOG integration.
08/02/13   sr      Added support for extended event APIs
05/22/10   sg      Doxygenated the file 
09/30/09   as      Removed inclusion of customer.h  
08/05/09   JV      Removed the CUST_H featurization around the inclusion of 
                   customer.h.
06/22/09   mad     Featurized inclusion of customer.h
06/11/09   JV      Featurized inclusion of customer.h to ease merging of the 
                   Q6 branch
12/02/08   mad     Took out internal functions and those not needed 
                   for stream driver model.
01/02/08   mad     Added extern "C" modifiers for function declarations
05/14/04   eav     Added FEATURE_SAVE_DEBUG_TRACE.  Added the function
                   event_save_circ_buffer_to_efs, which is called from err.c
04/17/01   lad     Moved constants to diagtune.h.
                   Moved event definitions to event_defs.h.
02/23/01   lad     Updated API for core diagnostics service.
11/17/00   jal     Bit fields in event_id_type came out in inverted order
                   from what we wanted.  Fixed.
11/13/00   lcc     Added event and type definitions for some events.
11/10/00   lad     Made obsolete event_report_data() a macro to NULL.
11/09/00   jal     Took the old event IDs out (again!)
11/07/00   jal     Renamed event_extra_data_type to event_payload_type
10/17/00   jal     Changes for new event accumulation mechanism
05/15/00   lad     Changed truncated timsteamp from uint32 to uint16.
04/11/00   lad     Increased # of customer reserved event IDs to 0x100.
12/16/99   lad     Added support for event reporting service.
01/21/99   lad     Created file.

===========================================================================*/



#include "comdef.h"		       /* Definitions for byte, word, etc. */

/* Since the IDs and type definitions are part of the API, include it here */
#include "event_defs.h"
#include "qw.h"
#include "diagbuffer_defs.h"

/* -------------------------------------------------------------------------
   Function Defintions
   ------------------------------------------------------------------------- */
#ifdef __cplusplus
extern "C"
{
#endif


/*!
@defgroup event_service Diagnostic Event Service
@details
Static events are reported by the mobile device to indicate actions, such as state changes and
configuration, which are directly related to the operating standards of the system.
The intent is to provide a reporting mechanism that minimizes resource usage (in
particular, bandwidth) and maximizes storage and transmission priority for operating
information that is considered most important in software test and verification.

*/

/*!
   @ingroup event_service
   @brief 
   Report an event without payload data . 

   @param[in] event_id                 ID of the event to be reported.Event IDs are defined in event_defs.h

   @dependencies 
   Event services must be initialized.event_init() should be called to initialize the event services

   @sideeffects
   None

   @return
   None

   @sa
   None

*/
  
	void event_report (event_id_enum_type event_id);

/*!
   @ingroup event_service
   @brief 
   Report an event without payload data . Similar to event_report() except that this function can take
   additional parameters - version and void * to specify additional information regarding where the event
   is being generated.

   @param[in] event_id                 ID of the event to be reported.Event IDs are defined in event_defs.h
   @param[in] version                  Specifies the version - currently, there are two versions 
                                       0 - same as event_report() (for backwards compatibility)
                                       1 - The outgoing event report header will have support to
                                           include ID field to differentiate the events coming
                                           from different instances of the processor.
   @param[in] *parameter               Based on the version passed in, this pointer is interpreted
                                       as follows-
                                       version       pointer
                                        0             NULL
                                        1             uint32 * . The uint32 value this pointer points to,
                                                      is the ID of the instance generating this event.
 
   @dependencies 
   Event services must be initialized.event_init() should be called to initialize the event services

   @sideeffects
   None

   @return
   None

   @sa
   None

*/
  
	boolean event_report_ext (event_id_enum_type event_id, unsigned int version, void *parameter);
/*!
   @ingroup event_service

   @brief
   Report an event with payload data .Payload structure format must be published 

   @param[in] event_id                 Event ID  to be reported .Event IDs are defined in event_defs.h

   @param[in] length                   length of the associated  payload . 

   @param[in] *data                    Pointer to the payload structure .Payload should be published.
                                       Please refer to 80-V6196-1 for details 

   @dependencies 
   Event services must be initialized .event_init() should be called to initialize the event services

   @sideeffects
   None

   @return
   None

   @sa
   None

*/
  
  void event_report_payload (event_id_enum_type event_id, uint8 length, void *data);

  /*!
     @ingroup event_service

     @brief
     Report an event with payload data .Payload structure format must be published 

     @param[in] event_id                 Event ID  to be reported .Event IDs are defined in event_defs.h

     @param[in] length                   length of the associated  payload . 

     @param[in] *data                    Pointer to the payload structure .Payload should be published.
                                         Please refer to 80-V6196-1 for details
   
     @param[in] version                  Specifies the version - currently, there are two versions 
                                         0 - same as event_report_payload() (for backwards compatibility)
                                         1 - The outgoing event report header will have support to
                                             include ID field to differentiate the events
                                             coming from different instances of the processor.
   
     @param[in] *parameter                Based on the version passed in, this pointer is interpreted
                                         as follows-
                                         version       pointer
                                         0             NULL
                                         1             uint32 * . The uint32 value this pointer points to,
                                                       is the ID of the instance generating this event.

     @dependencies 
     Event services must be initialized .event_init() should be called to initialize the event services

     @sideeffects
     None

     @return
     None

     @sa
     None

  */

    boolean event_report_payload_ext (event_id_enum_type event_id, uint8 length, void *data, unsigned int version, void * parameter);

  /*!
     @ingroup event_service

     diagbuffer_event API:
      Users are recommended to use the macro DIAGBUFFER_EVENT_REPORT, rather than the
      diagbuffer_event API to future-proof their code against diag buffer changes
  */

    boolean _diagbuffer_event_payload(diagbuffer_handle handle, event_id_enum_type event_id,
                    uint8 length, void *payload,
                    unsigned int version, void *param);
  /*!
     @ingroup event_service

     @brief
     Writes a Diag event packet to the specified log handle. Users are recommended
     to use the macro rather than the API.

     @param handle : Handle to the diagbuffer.
                     if "SYSDIAG_HANDLE" is passed, then event gets allocated
                     in event heap of DIAG.

     @param event_id : Event ID to log, as defined in event_id_enum_type

     @param[in] length : length of the associated  payload. User need to to pass 0 if
                         there is no payload for the event.

     @param[in] *payload : Pointer to the payload structure.Payload should be published.
                           Please refer to 80-V6196-1 for details. Payload to be passed as
                           NULL if there is no payload associated for the event.
                           (Length to be passed as 0 and payload NULL for events without payload)

     @param version : Version number to interpret data type stored in 'param'

     @param param : If version == 0, param is ignored.
                    If version == 1, param is a pointer to a uint32 ID.

     @dependencies
     None

     @sideeffects
     None

     @return
     TRUE on success, FALSE on error

     @sa
     None

 */
    #define DIAGBUFFER_EVENT_REPORT(h, event_id, length, payload, version, param) \
            _diagbuffer_event_payload(h, event_id, length, payload, version, param)

/*!
     @ingroup event_service

     @brief
     This API checks whether the composite mask for a particular event ID is set or not.
     Composite mask includes preset mask, DCI mask, single listener mask and extended listener mask

     @param event_id : Event ID to whose mask should be checked

     @dependencies
     None

     @sideeffects
     None

     @return
     TRUE if the mask is set, FALSE otherwise.

     @sa
     None

 */

boolean event_status(event_id_enum_type event_id);

#ifdef __cplusplus
}
#endif /* for extern "C" modifier */
#endif		   /* EVENT_H */
