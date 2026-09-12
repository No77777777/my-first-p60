
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                            Event Service

General Description
  This file containsthe implementation for the event reporting service for uimage mode.
  These APIs will be called by clients when they want to generate a new event. 
  The APIs will check to see if uImage mode is active or not when they are called.
  If active, the API will write the event packet into the uImage diag_buffer using 
  the diag_buffer interface. A dedicated diag_buffer will be set aside for uImage logging.
  This buffer will act as a circular buffer and will not be drained while in uImage mode.
  Upon exiting uImage mode, Diag task will drain out the uImage diag_buffer as part of 
  it’s regular draining process.




Initialization and Sequencing Requirements
  Call event_init() before reporting any events.

Copyright (c) 2015 by QUALCOMM Technologies, Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*==========================================================================

                              Edit History

$Header: //components/rel/core.mpss/10.0/services/diag/micro_diagbuffer/src/micro_diagbuffer_event_api.c#1 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
08/12/15   xy      Verified DIAG uimage APIs in island mode 
06/15/15   xy      Added new DIAG APIs for uimage mode 
05/12/15   xy      Added support for DIAG uImage APIs 
05/12/15   xy      Created file.
===========================================================================*/
#include "micro_event.h"
#include "eventi.h" /*for EVENT_MASK_SIZE*/
#include "diag_v.h" /*for DIAGBUF_VERSION_1*/
#include "diagdiag_v.h" /*for MAX_VALUE_UINT32 */
#include "msgcfg.h"
#include "event.h"
#include "micro_msg_diag_service.h"
#include "micro_diagbuffer.h"

uint32 micro_alloc_count_event = 0;

extern unsigned char diag_event_composite_mask[EVENT_MASK_SIZE];
extern micro_diagbuffer_handle uimage_diagbuffer;
/*===========================================================================
FUNCTION MICRO_EVENT_REPORT_EXT

DESCRIPTION
   Report an event without payload data . This function can take additional
   parameters - version and void * to specify information regarding where
   the event is being generated.
 
PARAMETERS
   event_id       ID of the event to be reported.Event IDs are defined in event_defs.h
   version        Specifies the version - currently, there are two versions 
                  0 - same as event_report()if not running in uImage mode. In uImage mode,
                      the buffering/queueing behavior is slightly different
                  1 - The outgoing event report header will have support to
                      include ID field to differentiate the events coming
                      from different instances of the processor.
                      same as event_report_ext()if not running in uImage mode. In uImage mode,
                      the buffering/queueing behavior is slightly different
  *parameter      Based on the version passed in, this pointer is interpreted
                  as follows-
                  version       pointer
                  0             NULL
                  1             uint32 * . The uint32 value this pointer points to,
                                is the ID of the instance generating this event.
 
RETURN VALUE 
  TRUE: The event was successfully queued
  FALSE: The event was not queued
=========================================================================*/
boolean micro_event_report_ext(event_id_enum_type event_id, unsigned int version, void *parameter)
{
  micro_diagbuffer_result ret_value = MICRO_DIAGBUFFER_ERROR;
  boolean status = FALSE;
  boolean ret = FALSE;

  if (((version == DIAGBUF_VERSION_1) && (parameter == NULL)) || (version > DIAGBUF_VERSION_1))
  {
    return FALSE;
  }

/*check if it is in micro image mode*/
  if (diag_island_mode_check())
  {
    /*check if the event is enabled or not*/
    status = event_mask_enabled(diag_event_composite_mask, event_id);   
    if (status) 
    {
      /* allocate the event in diag_buffer*/
      ret_value = MICRO_DIAGBUFFER_EVENT( uimage_diagbuffer, event_id, version, (void*)parameter );
      if ( micro_alloc_count_event == MAX_VALUE_UINT32 )
      {
        MICRO_MSG_EXT(MSG_SSID_DIAG, MSG_LEGACY_LOW, 0, NULL, "Variable micro_alloc_count_event has reached maximum value.");
      }
      else
      {
        micro_alloc_count_event++;
      }
      ret = MICRO_DIAGBUFFER_GET_RESULT(ret_value);
    }
  }
  else
  {
    /*if not in micro image mode, call regular API*/
    ret = event_report_ext(event_id, version, parameter);
  }
  return ret;
}


/*===========================================================================
FUNCTION MICRO_EVENT_REPORT_PAYLOAD_EXT

DESCRIPTION
   Report an event with payload data .Payload structure format must be published
 
PARAMETERS
   event_id   ID of the event to be reported.Event IDs are defined in event_defs.h
   length     Length of the associated  payload
   *data      Pointer to the payload structure .Payload should be published.
   version    Specifies the version - currently, there are two versions 
              0 - same as event_report_payload()if not running in uImage mode. In uImage mode,
                  the buffering/queueing behavior is slightly different
              1 - The outgoing event report header will have support to
                  include ID field to differentiate the events coming
                  from different instances of the processor.
                  same as event_report_payload_ext()if not running in uImage mode. In uImage mode,
                  the buffering/queueing behavior is slightly different
   
   *parameter   Based on the version passed in, this pointer is interpreted
                as follows-
                version       pointer
                 0             NULL
                 1             uint32 * . The uint32 value this pointer points to,
                               is the ID of the instance generating this event.
 
RETURN VALUE 
  TRUE:  The event was successfully queued
  FALSE: The event was not queued
=========================================================================*/
boolean
micro_event_report_payload_ext(event_id_enum_type event_id, uint8 length, void *payload, unsigned int version, void *parameter)
{
  micro_diagbuffer_result ret_value = MICRO_DIAGBUFFER_ERROR;
  boolean status = FALSE;
  boolean ret = FALSE;

  if (((version == DIAGBUF_VERSION_1) && (parameter == NULL)) || (version > DIAGBUF_VERSION_1))
  {
    return FALSE;
  }
  if (length <= 0 || payload == NULL)
  {
    return FALSE;
  }

  /*check if it is in micro image mode*/
  if (diag_island_mode_check())
  {
    /*check if the event is enabled or not*/
    status = event_mask_enabled(diag_event_composite_mask, event_id);   
    if (status) 
    {
      /* allocate the event in diag_buffer */
     ret_value =  MICRO_DIAGBUFFER_EVENT_PAYLOAD( uimage_diagbuffer, event_id,  version, parameter, 
	                          length, payload);

      if ( micro_alloc_count_event == MAX_VALUE_UINT32 )
      {
        MICRO_MSG_EXT(MSG_SSID_DIAG, MSG_LEGACY_LOW, 0, NULL, "Variable micro_alloc_count_event has reached maximum value.");
      }
      else
      {
        micro_alloc_count_event++;
      }
      ret = MICRO_DIAGBUFFER_GET_RESULT(ret_value);
    }
  }
  else
  {
    /*if not in micro image mode, call regular API*/
    ret = event_report_payload_ext(event_id, length, payload, version, parameter);
  }

  return ret;
}
