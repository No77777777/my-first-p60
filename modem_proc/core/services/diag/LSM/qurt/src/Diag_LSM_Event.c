/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

              Legacy Service Mapping layer implementation for Events

GENERAL DESCRIPTION
  Contains main implementation of Legacy Service Mapping layer for Diagnostic 
  Event Services.

EXTERNALIZED FUNCTIONS
  event_report
  event_report_payload
  
INITIALIZATION AND SEQUENCING REQUIREMENTS
  
Copyright (c) 2007-2010, 2012, 2014-2017, 2019 by QUALCOMM Technologies, Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/services/diag/LSM/qurt/src/Diag_LSM_Event.c#1 $

when       who    what, where, why
--------   ---    ----------------------------------------------------------
01/04/19   kdey   Added API event_status
09/29/17   sa     Send IOCTL for mask update after diagID is set.
05/25/17   sp     Support for DIAG Extended Log and Event APIs on UserPD
09/06/16   as     Created event_mask_is_set()
06/18/15   sa     Added support for time sync feature.
01/29/15   xy     Fixed compilation warnings  
10/31/14   sr     Fixed the issue with events in UserPD 
07/05/12   sg     Changes to bringup Diag MultiPD
08/25/10   sg     Fixed compiler warnings
04/12/10   sg     Merging both DCM rtos and WM files 
06/22/09   mad    Removed inclusion of diagdiag.h and diagpkti.h
01/19/09   mad    Removed inclusion of diagtune.h, updated copyright
09/30/08   mad    Changes to take out Component services, and use 
                  filesystem calls to access windiag driver.
05/01/08   JV     Added support to update the copy of event_mask in this process
                  during initialization and also on mask change
11/12/07   mad    Created      

===========================================================================*/
                     
                      
/* ==========================================================================
   Include Files
========================================================================== */

#include "comdef.h"
#include "event.h" 
#include "Diag_LSMi.h" /* for declaration of windiag Handle */
#include "DiagSvc_Malloc.h"
#include "event_defs.h"
#include "queue.h"
#include "eventi.h"
#include "Diag_LSM_Event_i.h"
#include "diag_shared_i.h" /* for definition of diag_data struct. */
#include "Diag_LSM_Qdi_wrapper.h"
#include "time_svc.h" //for time_get
#include "stringl.h"

/*this keeps track of number of failures to WriteFile().
This will currently be used only internally.*/
static unsigned int gEvent_commit_fail = 0;

/* Time Get Function Pointer from diag.c */
extern uint8 (*diag_time_get_LSM)(qword time);

/*Local Function declarations*/
static byte *event_alloc(event_id_enum_type id, uint8 payload_length, int* pAlloc_Len, unsigned int version, void *parameter);

/* The event mask. */
static unsigned char event_mask[EVENT_MASK_SIZE];

#define EVENT_MASK_BIT_SET(id) \
  (event_mask[(id)/8] & (1 << ((id) & 0x07)))
  

/* Externalized functions */
/*===========================================================================

FUNCTION EVENT_REPORT

DESCRIPTION
   Static events are reported by the mobile device to indicate actions, such
   as state changes and configuration, which are directly related to the
   operating standards of the system. The intent is to provide a reporting
   mechanism that minimizes resource usage (in particular, bandwidth) and
   maximizes storage and transmission priority for operating information that
   is considered most important in software test and verification.

PARAMETERS
   event_id   - ID of the event to be reported, as defined in "event_defs.h"

RETURN VALUE
   None

=========================================================================*/
DLL_API_GLOBAL void
event_report(event_id_enum_type event_id)
{

   event_report_payload_ext(event_id, 0, NULL, DIAG_EXT_API_VERSION_0, NULL);
   return;

} /* end event_report */

/*===========================================================================

FUNCTION EVENT_REPORT_EXT

DESCRIPTION
   Report an event without payload data . Similar to event_report() except
   that this function can take additional parameters - version and void * to
   specify information regarding where the event is being generated.

PARAMETERS
   event_id   - ID of the event to be reported, as defined in "event_defs.h"
   version    - Specifies the version; currently, there are two versions:
                  0 - Same as event_report() (for backwards compatibility)
                  1 - The outgoing event report header will have support to
                      include ID field to differentiate the events coming
                      from different instances of the processor.
   *parameter - Based on the version passed in, this pointer is interpreted as:
                  Version       Pointer
                  0             NULL
                  1             uint32 * (Instance ID)

RETURN VALUE
   TRUE: The event was successfully generated
   FALSE: The event was not successfully generated

===========================================================================*/
DLL_API_GLOBAL boolean
event_report_ext(event_id_enum_type event_id, unsigned int version, void *parameter)
{

   return event_report_payload_ext(event_id, 0, NULL, version, parameter);

} /* end event_report_ext */

/*===========================================================================

FUNCTION EVENT_REPORT_PAYLOAD

DESCRIPTION
   Report an event with payload data. Payload structure format must be published.

PARAMETERS
   event_id       - ID of the event to be reported, as defined in "event_defs.h"
   length         - Length of the associated payload
   *payload       - Pointer to the payload structure. Payload should be published.

RETURN VALUE
   None

=========================================================================*/
DLL_API_GLOBAL void
event_report_payload(event_id_enum_type event_id, uint8 length, void *payload)
{

  event_report_payload_ext(event_id, length, payload, DIAG_EXT_API_VERSION_0, NULL);
  return;

} /* end event_report_payload */

/*===========================================================================

FUNCTION EVENT_REPORT_PAYLOAD_EXT

DESCRIPTION
   Report an event with payload data .Payload structure format must be published

PARAMETERS
   event_id       - ID of the event to be reported, as defined in "event_defs.h"
   length         - Length of the associated payload
   *data          - Pointer to the payload structure. Payload should be published.
   version        - Specifies the version; currently, there are two versions:
                      0 - Same as event_report (for backwards compatibility)
                      1 - The outgoing event report header will have support to
                          include ID field to differentiate the events coming
                          from different instances of the processor.
   *parameter     - Based on the version passed in, this pointer is interpreted as:
                      Version       Pointer
                      0             NULL
                      1             uint32 * (Instance ID)

RETURN VALUE
   TRUE: The event was successfully generated
   FALSE: The event was not successfully generated

=========================================================================*/
DLL_API_GLOBAL boolean
event_report_payload_ext(event_id_enum_type event_id, uint8 length, void *payload, unsigned int version, void * parameter)
{

   /* Variable Init */
   byte * pEvent = NULL;
   event_store_type * temp = NULL;
   int Alloc_Len = 0;
   boolean ret_value = FALSE;

   /* QDI Handle sanity check */
   if (diag_qdi_handle < DIAG_FD_MIN_VALUE)
      return FALSE;

   /* Version sanity check */
   if ((version > DIAG_EXT_API_VERSION_1) || ((version > DIAG_EXT_API_VERSION_0) && (parameter == NULL)))
   {
      return FALSE;
   }

   /* Allocate the event */
   pEvent = event_alloc(event_id, length, &Alloc_Len, version, parameter);

   if (pEvent)
   {
     /* Copy over the payload (if one exists) */
     if ((payload != NULL) && (length > 0))
     {
       temp = (event_store_type *) (pEvent + FPOS(diag_data, rest_of_data));
       memscpy(temp->payload.payload, temp->payload.length, payload, length);
     }

     /* Write the event to the guest OS Diag Driver */
     if(diag_lsm_write_data(pEvent, Alloc_Len) < Alloc_Len)
     {
       gEvent_commit_fail++;
     }
     else
     {
       ret_value = TRUE;
     }

     /* Free the local allocation */
     DiagSvc_Free(pEvent, GEN_SVC_ID);
   }

   return ret_value;

} /* end event_report_payload_ext */

/*===========================================================================
FUNCTION   Diag_LSM_Event_Init

DESCRIPTION
  Initializes the event service. 
  Mask will be updated later after diagID has been set.

DEPENDENCIES
  None.

RETURN VALUE
  FALSE = failure, else TRUE

SIDE EFFECTS
  None

===========================================================================*/

boolean Diag_LSM_Event_Init(void)
{
   return TRUE;
} /* Diag_LSM_Event_Init */

/*===========================================================================

FUNCTION    Diag_LSM_Event_DeInit

DESCRIPTION
  De-Initialize the Diag Event service.

DEPENDENCIES
  None.

RETURN VALUE
  boolean: returns TRUE; currently does nothing.

SIDE EFFECTS
  None

===========================================================================*/
boolean Diag_LSM_Event_DeInit(void)
{
  return TRUE;
} /* Diag_LSM_Event_DeInit */

/*===========================================================================

FUNCTION    event_mask_is_set

DESCRIPTION
  Checks if the event mask is set for this user process

DEPENDENCIES
  None.

RETURN VALUE
  FALSE if not set, else TRUE

SIDE EFFECTS
  None

===========================================================================*/
boolean event_mask_is_set(void)
{
  for (int i = 0; i < EVENT_MASK_SIZE; i++)
  {
     if (event_mask[i] > 0)
     {
         return TRUE;
     }
  }
  return FALSE;
} /* event_mask_is_set */

/*===========================================================================

FUNCTION    event_update_mask

DESCRIPTION
  Updates the event mask for this user process

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean event_update_mask(void)
{
 	boolean status = TRUE;
   if(diag_qdi_handle>=DIAG_FD_MIN_VALUE)
   {
      dword maskLen_Recvd = 0;
      if(!qurt_qdi_handle_invoke(diag_qdi_handle, DIAG_QDI_IOCTL,DIAG_IOCTL_GETEVENTMASK, NULL,0, event_mask, sizeof(event_mask), &maskLen_Recvd) 
        || maskLen_Recvd != EVENT_MASK_SIZE)
      {
         status = FALSE;
      }
   }
   else
   {
      status = FALSE;
   }
	return status;
} /* event_update_mask */


/*==========================================================================

FUNCTION EVENT_ALLOC

DESCRIPTION
  This routine allocates an event item from the pre-mallocated memory pool
   and fills in the following information:
  
  Event ID
  Time stamp
  Payload length field
  version        - Specifies the version; currently, there are two versions:
                      0 - Same as event_report (for backwards compatibility)
                      1 - The outgoing event report header will have support to
                          include ID field to differentiate the events coming
                          from different instances of the processor.
   *parameter    - Based on the version passed in, this pointer is interpreted as:
                      Version       Pointer
                      0             NULL
                      1             uint32 * (Instance ID)
  
  //TODO :This routine also detects dropped events and handles the reporting of
  //dropped events.
    
RETURN VALUE
  A pointer to the allocated  event is returned.  
  NULL if the event cannot be allocated.
  The memory should be freed by the calling function, using DiagSvc_Free().
  pAlloc_Len is an output value, indicating the number of bytes allocated.

===========================================================================*/
static byte *
event_alloc(event_id_enum_type id, uint8 payload_length, int* pAlloc_Len, unsigned int version, void *parameter)
{
   /* Variable Init */
   byte * pEvent = NULL;
   event_store_type * temp = NULL;
   int alloc_len = 0;

   /* Sanity check for valid event id */
   if ((id > EVENT_LAST_ID) || !EVENT_MASK_BIT_SET (id) ||
		((version > DIAG_EXT_API_VERSION_0) && (parameter == NULL)))
   {
       return NULL;
   }

   /* Allocate the event */
   alloc_len =  FPOS(diag_data, rest_of_data) + FPOS (event_store_type, payload.payload) + payload_length;
   pEvent = (byte *) DiagSvc_Malloc(alloc_len, GEN_SVC_ID);

   if (pEvent)
   {
      /* Fill in the fact that this is an event */
      ((diag_data *) pEvent)->diag_data_type = DIAG_DATA_TYPE_EVENT;

      /* Advance the pointer to point to the event_store_type part */
      temp = (event_store_type *) (pEvent + FPOS(diag_data, rest_of_data));

      /* Fill out the event contents */
      time_get(temp->ts);
      temp->event_id.id = id;
      temp->payload.length = payload_length;
      temp->stream_id = DIAG_STREAM_1;

      /* Fill out version-specific fields */
      switch (version)
      {
         case DIAG_EXT_API_VERSION_1:
            temp->version = DIAG_EXT_API_VERSION_1;
            temp->id = *((uint32 *) parameter);
            break;

         case DIAG_EXT_API_VERSION_0:
            temp->version = DIAG_EXT_API_VERSION_0;
            temp->id = 0;
            break;

         default:
            /* Invalid version passed in, don't allocate */
            DiagSvc_Free(pEvent, GEN_SVC_ID);
            return NULL;
            break;
      }

      /* Return the number of bytes allocated */
      if(pAlloc_Len)
      {
         *pAlloc_Len = alloc_len;
      }
   }

   return pEvent;

} /* end event_alloc */

/*=================================================================================================

FUNCTION    event_status

DESCRIPTION
  This API checks whether the composite mask for a particular event ID is set or not.
  Composite mask includes preset mask, DCI mask, single listener mask and extended listener mask.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if the mask is set, FALSE otherwise.

SIDE EFFECTS
  None

====================================================================================================*/

DLL_API_GLOBAL boolean event_status(event_id_enum_type event_id)
{
  boolean enabled = FALSE;
  if (event_id <= EVENT_LAST_ID)
  {
    if(EVENT_MASK_BIT_SET(event_id))
    {
      enabled  = TRUE;
    }
  }
  return enabled;
}
