#ifndef MICRO_EVENT_H
#define MICRO_EVENT_H

/*!
@ingroup event_service
@file micro_event.h
@brief 
This file describes the Diagnostics Event Service APIs for micro image mode. 
Event Services must be initialized before using the event services apis. 
These APIs will be called by clients when they want to generate a new event. 
The APIs will check to see if uImage mode is active or not when they are called.
If active, the API will write the event packet into the uImage diag_buffer using 
the diag_buffer interface. A dedicated diag_buffer will be set aside for uImage logging.
This buffer will act as a circular buffer and will not be drained while in uImage mode.
Upon exiting uImage mode, Diag task will drain out the uImage diag_buffer as part of 
it’s regular draining process.

*/

/*
Copyright (c) 2015 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

*/

/*===========================================================================
                          Edit History

$Header: //components/rel/core.mpss/10.0/api/services/micro_event.h#1 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
05/12/15   xy      Created file.
===========================================================================*/
#include "comdef.h"                   /* Definitions for byte, word, etc. */
#include "event_defs.h"

/*!
   @ingroup event_service
   @brief 
   Report an event without payload data. This function can take parameters - version and void * to specify
   information regarding where the event is being generated.

   @param[in] event_id                 ID of the event to be reported. Event IDs are defined in event_defs.h
   @param[in] version                  Specifies the version - currently, there are two versions 
                                       0 - same as event_report()if not running in uImage mode. In uImage mode,
                                           the buffering/queueing behavior is slightly different
                                       1 - The outgoing event report header will have support to
                                           include ID field to differentiate the events coming
                                           from different instances of the processor.
                                           same as event_report_ext()if not running in uImage mode. In uImage mode,
                                           the buffering/queueing behavior is slightly different
   @param[in] *parameter               Based on the version passed in, this pointer is interpreted
                                       as follows-
                                       version       pointer
                                        0             NULL
                                        1             uint32 *  The uint32 value this pointer points to,
                                                      is the ID of the instance generating this event.
 
   @dependencies 
   Event services must be initialized. event_init() should be called to initialize the event services

   @sideeffects
   None

   @return
   <ul>
   <li>Return TRUE on success. 
   <li>Return FLASE if the event code is disabled.
   </ul>

   @sa
   None

*/
  boolean micro_event_report_ext (event_id_enum_type event_id, unsigned int version, void *parameter);

/*!
   @ingroup event_service

   @brief
   Report an event with payload data. Payload structure format must be published.

   @param[in] event_id                 Event ID  to be reported. Event IDs are defined in event_defs.h

   @param[in] length                   length of the associated  payload. 

   @param[in] *data                    Pointer to the payload structure. Payload should be published.
                                       Please refer to 80-V6196-1 for details
   
   @param[in] version                  Specifies the version - currently, there are two versions 
                                       0 - same as event_report_payload()if not running in uImage mode. In
                                           uImage mode, the buffering/queueing behavior is slightly different
                                       1 - The outgoing event report header will have support to
                                           include ID field to differentiate the events coming
                                           from different instances of the processor.
                                           same as event_report_payload_ext()if not running in uImage mode.
                                           In uImage mode, the buffering/queueing behavior is slightly different
   
   @param[in] *parameter               Based on the version passed in, this pointer is interpreted
                                       as follows-
                                       version       pointer
                                       0             NULL
                                       1             uint32 *  The uint32 value this pointer points to,
                                                     is the ID of the instance generating this event.
 
   @dependencies 
   Event services must be initialized. event_init() should be called to initialize the event services

   @sideeffects
   None

   @return
   <ul>
   <li>Return TRUE on success. 
   <li>Return FLASE if the event code is disabled.
   </ul>

   @sa
   None

  */

  boolean micro_event_report_payload_ext (event_id_enum_type event_id, uint8 length, void *data, unsigned int version, void * parameter);

#endif		   /* MICRO_EVENT_H */
