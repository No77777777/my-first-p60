
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

              Diag Interface

GENERAL DESCRIPTION
   Contains main implementation of Diagnostic Log Services.

EXTERNALIZED FUNCTIONS
   log_set_code
   log_set_length
   log_set_timestamp
   log_submit
   log_free
   log_get_length
   log_get_code
   log_status
   log_alloc
   log_commit
   log_shorten

INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright (c) 2007-2012, 2014-2017 by QUALCOMM Technologies, Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

                              Edit History

$Header: //components/rel/core.mpss/10.0/services/diag/LSM/qurt/src/Diag_LSM_Log.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/04/18   gn      Deprecated unused legacy commands, Featurized commands that 
                   are applicable in a specific configuration
09/29/17   sa      Send IOCTL for mask update after diagID is set.
05/25/17   sp      Support for DIAG Extended Log and Event APIs on UserPD
27/01/17   nk      Fixed issue with log_submit api
09/06/16   as      Created log_mask_is_set()
03/10/16   tbg     Added DLL_API_GLOBAL flag to public APIs
02/06/15   xy      Removed diagbuf_mpd_commit() from log_free()
01/29/15   xy      Fixed compilation warnings
12/04/14   vk      Multi-PD Optimizations
09/19/15   is      Update log_commit_fail to unsigned
11/27/13   ph      Updated MSG 1.0 API references to MSG.2.0.
07/05/12   sg      Changes to bringup Diag MultiPD
10/19/10   mad     Fixed log_shorten
04/09/10   sg      Merging DCM rtos and wm files
09/23/09   sg      Mainlined when MSM5000_IRAM_FWD is not defined
07/07/09   mad     Removed unwanted headers: queue.h,diagpkti.h, diagi.h,
                   diagpkt.h,assert.h,qw.h,log_codes.h,diagtune.h,string.h
01/27/09   mad     modified log_submit() to allocate memory only if log mask is
                   enabled.
09/30/08   mad     Changes to take out Component services, and use
                   filesystem calls to access windiag driver.
09/04/08   mad     Fixed bug in log_free(): to free the correct pointer
05/01/08   JV      Added support to update the copy of log_mask in this process
                   during initialization and also on mask change
02/11/08   JV      Changed the comparison of IDiagPkt_Send()'s return value from
                   AEE_FAILURE to AEE_SUCCESS
01/16/08   JV      Created stubs for log_on_demand_register and log_on_demand_unregister
                   as we do not have function pointer support for Diag 1.5A
11/20/07   JV      Created

===========================================================================*/


/* ==========================================================================
   Include Files
========================================================================== */

#include "diagdiag_v.h"           /* For diag_log_rsp_type */
#include "log.h"
#include "msg.h"                  /* For MSG_LOW */
#include "Diag_LSM_Log_i.h"       /* For LSM log init and deinit */
#include "Diag_LSMi.h"            /* For declaration of windiag driver HANDLE etc */
#include "diagbuf_mpd.h"          /* For mpd functions */
#include "diaglogi.h"             /* For Diag log_header_type and functions */
#include "diag_shared_i.h"        /* For definition diag datatypes. */
#include "log_arrays_i.h"         /* For log_last_item_tbl */
#include "Diag_LSM_Qdi_wrapper.h" /* For LSM QDI wrapper functions */
#include "diag_diagIDi.h"
#include "diagi_v.h"              /* For diagbuf_header_type */

#include <stringl/stringl.h>      /* For memscpy */
#include "time_svc.h"             /* For time_get() */

#include "dll_global_def.h"       /* definitions for shared object API export macros */

/* The content of this table are computed in diag_lsm_log_init().  'uint16' is used
 * instead of 'int' to reduce data bus cycles to access an element of this
 * array.  The last entry is the offset to the byte following the array.  It
 * is used for range checking. */
static uint16 log_mask_offset_tbl[LOG_EQUIP_ID_LAST+1];

/* 'log_mask' is the mask of bits used to represent the configuration of all
* log codes.  '1' denotes the code being enabled, '0' denotes disabled.
*/

static byte log_mask[LOG_MASK_SIZE];

void *log_commit_last = NULL; /* Many writers, 1 reader (DIAG) */

/* Internal function declarations */
static boolean log_status_mask( log_code_type code );
static boolean log_search_mask( unsigned int id, unsigned int item );
boolean log_mask_enabled( const byte *xx_mask_ptr, unsigned int xx_id,
                                 unsigned int xx_item );

diag_wcdma_wl1_frame_cb_type diag_wcdma_wl1_frame_cb = NULL;
diag_wcdma_wl1_frame_cb_ext_type diag_wcdma_wl1_frame_cb_ext = NULL;

/* External Function Implementations */
/*===========================================================================

FUNCTION LOG_ALLOC

DESCRIPTION
  This function allocates a buffer of size 'length' for logging data.  The
  specified length is the length of the entire log, including the log
  header.  This operation is inteneded only for logs that do not require
  data accumulation.

  !!! The header is filled in automatically by this routine.

PARAMETERS
  log_code_type code   - log code
  unsigned int length  - size of log to alloc

DEPENDENCIES:
   Diag log service must be initialized.
   log_commit() or log_free() must be called ASAP after this call.

RETURN VALUE
  A pointer to the allocated buffer is returned on success.
  If the log code is disabled or there is not enough space, NULL is returned.

SIDE EFFECTS
  Since this allocation is made from a shared resource pool, log_commit()
  or log_free() must be called as soon as possible and in a timely fashion.

  If you need to log accumulated data, store the accumulated data in your
  own memory space and use log_submit() to log the data.
===========================================================================*/
DLL_API_GLOBAL void *
log_alloc(log_code_type code, unsigned int length)
{

   return log_alloc_ext(code, length, DIAG_EXT_API_VERSION_0, NULL);

} /* end log_alloc */

/*===========================================================================

FUNCTION LOG_ALLOC_EXT

DESCRIPTION
  Allocate a buffer of size 'length' for logging data. Supports version field

PARAMETERS
  code                 ID of the log to be reported
  length               The specified length is the length of the entire log,
                             	including the log header.
  version              Specifies the version - currently, there are two versions
                          0 - same as log_alloc() (for backwards compatibility)
                          1 - The outgoing log packet header will have support to
                              include ID field to differentiate the log pkts
                              coming from different instances of the processor.
 *parameter           Based on the version passed in, this pointer is interpreted
                      as follows-
                        version       pointer
                         0             NULL
                         1             uint32 * . The uint32 value this pointer points to,
                                       is the ID of the instance generating the
                                       log packet
=========================================================================*/

DLL_API_GLOBAL void *
log_alloc_ext(log_code_type code, unsigned int length, unsigned int version, void * parameter)
{

   /* Variable Init */
   diag_log_rsp_type * log_pkt_ptr = NULL;
   log_header_type * hdr_ptr = NULL;
   void * log_entry_ptr = NULL;

   /* QDI Handle sanity check */
   if (diag_qdi_handle < DIAG_FD_MIN_VALUE)
      return NULL;

   /* Sanity checks */
   if (((version > DIAG_EXT_API_VERSION_0) && (parameter == NULL)) || (version > DIAGBUF_MAX_VERSION_NUM))
   {
      return FALSE;
   }

   /* Length check */
   if (length <= sizeof(log_header_type))
   {
      MSG_1(MSG_SSID_DIAG, MSG_LEGACY_ERROR, "Alloc invalid length %d", length);
   }
   else if ((length + LOG_DIAGPKT_OFFSET) > DIAG_MAX_TX_PKT_SIZ)
   {
      MSG_2(MSG_SSID_DIAG, MSG_LEGACY_ERROR, "Attempt to allocate %d but Max Length of a log packet supported is %d", (length + LOG_DIAGPKT_OFFSET), DIAG_MAX_TX_PKT_SIZ);
   }
   else if (log_status(code))
   {
      /* Allocate enough for the entire log response packet, not just the log entry */
      log_pkt_ptr = (diag_log_rsp_type *) diagbuf_mpd_alloc_chain(DIAG_LOG_F, FPOS(diag_log_rsp_type, log) + length, 0, DIAGBUF_ALLOC_REQUEST_LOG, 1, version, parameter);

      if (log_pkt_ptr != NULL)
      {
         /* Fill in the log response (record) packet */
         diagbuf_mpd_write_stream_id (log_pkt_ptr, DIAG_STREAM_1);
         log_pkt_ptr->more = 0;
         log_pkt_ptr->cmd_code = DIAG_LOG_F;
         log_pkt_ptr->len = (uint16) length;

         /* Fill in the log header */
         hdr_ptr = (log_header_type *) &(log_pkt_ptr->log);
         hdr_ptr->len = (uint16) length;
         hdr_ptr->code = code;

         /* Fill in the timestamp (taking `version` into account) */
         log_set_timestamp_ext(hdr_ptr, version, parameter);

         /* The header pointer is the same as the log entry pointer */
         log_entry_ptr = (void *) hdr_ptr;
      }
      else
      {
         /* Dropped a log */
        MSG_1(MSG_SSID_DIAG, MSG_LEGACY_LOW, "Dropped log 0x%x", code);
      }
   }

  return log_entry_ptr;

} /* end log_alloc_ext */

/*===========================================================================

FUNCTION LOG_SHORTEN

DESCRIPTION
  This function shortens the length of a previously allocated logging buffer in
  legacy code. This is used when the size of the record is not known at allocation
  time.Now that diagbuf is not used in the LSM layer and we just use memory from
  a pre-allocated pool, calling log_shorten, does not free the excess memory, it just
  updates the length field.

PARAMETERS
   void * ptr  - pointer to the log pkt
  unsigned int length - length of the log pkt

DEPENDENCIES:
   Diag log service must be initialized.
   This must be called prior to log_commit().

NOTE
   log_shorten_ext is defined in 'log.h' as a macro wrapper for log_shorten

RETURN VALUE
  None.

===========================================================================*/
DLL_API_GLOBAL void
log_shorten(void * ptr, unsigned int length)
{

   /* Variable Init */
   diag_log_rsp_type * diag_log = NULL;

   if (ptr)
   {
      /* Point to the log record */
      diag_log = (diag_log_rsp_type *) ((byte *)ptr - LOG_DIAGPKT_OFFSET);

      if ( length < diag_log->len )
      {
         diagbuf_mpd_shorten((void *) diag_log, length + LOG_DIAGPKT_OFFSET);

         /* Set the log packet length to the new length */
         diag_log->len = (uint16) length;

         /* log_set_length takes the log itself, not the log packet */
         log_set_length(ptr, length);
      }
   }

} /* end log_shorten */

/*===========================================================================

FUNCTION LOG_COMMIT

DESCRIPTION
  This function commits a log buffer allocated by log_alloc().  Calling this
  function tells the logging service that the user is finished with the
  allocated buffer.

PARAMETERS
   void * ptr  - pointer to the log pkt to commit.

DEPENDENCIES
   Diag log service must be initialized.
   This must be called using a pointer returned from to log_alloc().

SIDE EFFECTS
  Since this allocation is made from a shared resource pool, this must be
  called as soon as possible after a log_alloc call.  This operation is not
  intended for logs that take considerable amounts of time ( > 0.01 sec ).

NOTE
  log_commit_ext is defined in 'log.h' as a macro wrapper for log_commit

RETURN VALUE
  None

===========================================================================*/
DLL_API_GLOBAL void
log_commit(void * ptr)
{

   /* Variable Init */
   diagbuf_header_type * pkt_hdr = NULL;
   diag_log_rsp_type * log_record_ptr = NULL;
   void *ptr_first = NULL; // Pointer to the first packet incase of chaining

   if (ptr)
   {
      /* Set last-commited log pointer to current log entry */
      log_commit_last = ptr;

      /* Access the diagbuf header for the current log entry */
      log_record_ptr = (diag_log_rsp_type *) ((byte *)ptr - LOG_DIAGPKT_OFFSET);
      pkt_hdr = (diagbuf_header_type *) ((byte *)log_record_ptr - sizeof(diagbuf_header_type));

      if (pkt_hdr == NULL)
      {
         return;
      }

      /* Version-specific operations */
      switch (pkt_hdr->version)
      {
         case DIAGBUF_VERSION_1:
         case DIAGBUF_VERSION_2:
         {
            if (pkt_hdr->chain == DIAGBUF_SECOND_PKT_CHAIN)
            {
               /*
                  Commit the second packet first in case diagbuf_drain() is
                  called in between the two commits in this function; this
                  is to prevent the scenario where the first packet is
                  committed and the second one is not.
               */
               diagbuf_mpd_commit(log_record_ptr);

               /*
                  Use pointer arithmetic to move from the second packet to the first.
                  Commit the first packet (NOTE: First packet is not a response type,
                  it is a diag_cmd_ext_vN_type + DEAD + pad).
               */
               ptr_first = (void *)(pkt_hdr - diagbuf_mpd_ver_len[pkt_hdr->version-1]);
               diagbuf_mpd_commit(ptr_first);
            }
            break;
         }

         case DIAGBUF_VERSION_0:
         {
            diagbuf_mpd_commit(log_record_ptr);
            break;
         }

         default:
         {
            /* Don't do anything for unsupported versions */
            break;
         }
      }
   }

   return;

} /* end log_commit */

/*===========================================================================

FUNCTION LOG_FREE

DESCRIPTION
  This function frees the buffer in pre-allocated memory.

DEPENDENCIES:
  Diag log service must be initialized.

RETURN VALUE

SIDE EFFECTS
  None.
===========================================================================*/
DLL_API_GLOBAL void
log_free(void * ptr)
{
   diag_log_rsp_type * pkt_ptr = NULL;

   if (ptr)
   {
      /* Point to the log record */
      pkt_ptr = (diag_log_rsp_type *) ((byte *)ptr - LOG_DIAGPKT_OFFSET);

      /* We free the log by "shortening" the diagbuf allocation to 0 */
      diagbuf_mpd_shorten((void *) pkt_ptr, 0);
   }

} /* end log_free */

/*===========================================================================

FUNCTION LOG_SUBMIT

DESCRIPTION
  This function is called to log an accumlated log entry. If logging is
  enabled for the entry by the external device, then this function essentially
  does the folliwng:
  log = log_alloc ();
  memscpy (log, length, ptr, log->len);
  log_commit (log);

PARAMETERS
  void *ptr - pointer to log pkt

DEPENDENCIES
   Diag log service must be initialized.

RETURN VALUE
  Boolean indicating success.

SIDE EFFECTS
  None.
===========================================================================*/
DLL_API_GLOBAL boolean
log_submit(void * ptr)
{

   return log_submit_ext(ptr, DIAG_EXT_API_VERSION_0, NULL);

} /* end log_submit */

/*=========================================================================*/

DLL_API_GLOBAL boolean
log_submit_ext(void * ptr, unsigned int version, void * parameter)
{
   /* Variable Init */
   diag_log_rsp_type * log_pkt_ptr = NULL;
   log_header_type * log_ptr = (log_header_type *) ptr;
   log_code_type code;
   unsigned int length;
   boolean bReturnVal = FALSE;

   /* QDI Handle sanity check */
   if (diag_qdi_handle < DIAG_FD_MIN_VALUE)
      return FALSE;

   /* Sanity checks */
   if ((log_ptr == NULL) || ((version > DIAG_EXT_API_VERSION_0) && (parameter == NULL)) || (version > DIAGBUF_MAX_VERSION_NUM))
   {
      return FALSE;
   }

   /* Fetch values from the log header */
   code = log_ptr->code;
   length = log_ptr->len;

   /* Length check */
   if (length <= sizeof(log_header_type))
   {
      MSG_1(MSG_SSID_DIAG, MSG_LEGACY_ERROR,"Alloc invalid length %d", length);
   }
   else if ((length + LOG_DIAGPKT_OFFSET) > DIAG_MAX_TX_PKT_SIZ)
   {
      MSG_2(MSG_SSID_DIAG, MSG_LEGACY_ERROR, "Attempt to allocate %d but Max Length of a log packet supported is %d", (length + LOG_DIAGPKT_OFFSET), DIAG_MAX_TX_PKT_SIZ);
   }
   else if (log_status(code))
   {
      /* Allocate enough for the entire log response packet, not just the log entry */
      log_pkt_ptr = (diag_log_rsp_type *) diagbuf_mpd_alloc_chain(DIAG_LOG_F, FPOS(diag_log_rsp_type, log) + length, 0, DIAGBUF_ALLOC_REQUEST_LOG, 1, version, parameter);

      if (log_pkt_ptr != NULL)
      {
         /* Fill in the log response (record) packet */
         diagbuf_mpd_write_stream_id (log_pkt_ptr, DIAG_STREAM_1);
 
         log_pkt_ptr->more = 0;
         log_pkt_ptr->cmd_code = DIAG_LOG_F;
         log_pkt_ptr->len = (uint16) length;

         /* Copy the log into the struct */
         memscpy(&log_pkt_ptr->log, length, (void *)ptr, length);

         /* Commit log to buffer */
         log_commit(&log_pkt_ptr->log);
         bReturnVal = TRUE;
      }
      else
      {
        /* Dropped a log */
        MSG_1(MSG_SSID_DIAG, MSG_LEGACY_LOW, "Dropped log 0x%x", code);
      }
   }

   return bReturnVal;

} /* end log_submit_ext */

/*===========================================================================

FUNCTION LOG_SET_TIMESTAMP

DESCRIPTION
  This function captures the system time and stores it in the given log record.

DEPENDENCIES
  Diag log service must be initialized.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
DLL_API_GLOBAL void
log_set_timestamp(void * plog_hdr_ptr)
{

   log_set_timestamp_ext(plog_hdr_ptr, DIAG_EXT_API_VERSION_0, NULL);

} /* end log_set_timestamp */

/*=========================================================================*/

DLL_API_GLOBAL void
log_set_timestamp_ext(void * plog_hdr_ptr, unsigned int version, void* parameter)
{
   qword time;
   uint32 *id = NULL;
   uint32 frame_number = 0;

   /* Store time API output in a qword */
   time_get(time);

   /* Version-specific operations */
   if ((diag_wcdma_wl1_frame_cb_ext != NULL) && (version == DIAG_EXT_API_VERSION_1) && (parameter != NULL))
   {
      id = (uint32 *)parameter;
      frame_number = diag_wcdma_wl1_frame_cb_ext(*id) & LOG_FN_MASK;
      time[0] >>= LOG_FN_NUM_BITS;
      time[0] <<= LOG_FN_NUM_BITS;
      time[0] |= frame_number;
   }
   else if(diag_wcdma_wl1_frame_cb != NULL)
   {
      frame_number = diag_wcdma_wl1_frame_cb() & LOG_FN_MASK;
      time[0] >>= LOG_FN_NUM_BITS;
      time[0] <<= LOG_FN_NUM_BITS;
      time[0] |= frame_number;
   }

   /* Store the qword properly within the log header */
   qw_equ_misaligned(((log_header_type *) plog_hdr_ptr)->ts, time);

} /* end log_set_timestamp_ext */

/*===========================================================================

FUNCTION LOG_SET_LENGTH

DESCRIPTION
  This function sets the length field in the given log record.

  !!! Use with caution.  It is possible to corrupt a log record using this
  command.  It is intended for use only with accumulated log records, not
  buffers returned by log_alloc().

DEPENDENCIES
  Diag log service must be initialized.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
DLL_API_GLOBAL void log_set_length( PACKED void *ptr, unsigned int length )
{
  if ( ptr )
  {
    /* All log packets are required to start with 'log_header_type'. */
    ((log_header_type *)ptr)->len = (uint16)length;
  }
} /* log_set_length */

/*===========================================================================

FUNCTION LOG_SET_CODE

DESCRIPTION
  This function sets the logging code in the given log record.

DEPENDENCIES
   Diag log service must be initialized.


RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
DLL_API_GLOBAL void log_set_code( PACKED void *ptr, log_code_type code )
{
  if ( ptr )
  {
    /* All log packets are required to start with 'log_header_type'. */
    ((log_header_type *)ptr)->code = code;
  }
	
} /* log_set_code */

/*===========================================================================

FUNCTION LOG_GET_LENGTH

DESCRIPTION
  This function returns the length field in the given log record.

DEPENDENCIES
   Diag log service must be initialized.

RETURN VALUE
  An unsigned int, the length

SIDE EFFECTS
  None.
===========================================================================*/
DLL_API_GLOBAL unsigned int log_get_length( PACKED void *ptr )
{
  unsigned int length = 0;
  if ( ptr )
  {
    log_header_type *plog = (log_header_type *)ptr;

    if ( plog )
    {
      length = plog->len;
    }
  }
  return (length);
}

/*===========================================================================

FUNCTION LOG_GET_CODE

DESCRIPTION
  This function returns the log code field in the given log record.

DEPENDENCIES
   Diag log service must be initialized.

RETURN VALUE
  log_code_type, the code

SIDE EFFECTS
  None.
===========================================================================*/
DLL_API_GLOBAL log_code_type log_get_code( PACKED void *ptr )
{
  log_code_type code = 0;
  if ( ptr )
  {
    log_header_type *plog = (log_header_type *)ptr;

    if ( plog )
    {
      code = (log_code_type)plog->code;
    }
  }
  return (code);
}

/*===========================================================================

FUNCTION LOG_STATUS

DESCRIPTION
  This function returns whether a particular code is enabled for logging.

DEPENDENCIES
   Diag log service must be initialized.

RETURN VALUE
  boolean indicating if enabled

SIDE EFFECTS
  None.
===========================================================================*/
DLL_API_GLOBAL boolean log_status( log_code_type code )
{
  boolean status = FALSE;

  status = log_status_mask(code);

  return (status);
} /* log_status */

/*===========================================================================

FUNCTION DIAG_LSM_LOG_INIT

DESCRIPTION
 Initializes the log service. No masks will be updated here.
 Mask update is done after diagID has been set.

RETURN VALUE
 boolean indicating success

SIDE EFFECTS
 None.
===========================================================================*/

boolean Diag_LSM_Log_Init( void )
{
  boolean status = TRUE;
  int i = 0;

  log_mask_offset_tbl[0] = 0;

  for ( i = 0; i < LOG_EQUIP_ID_LAST; i++ )
  {
    log_mask_offset_tbl[i + 1] = 1 + log_mask_offset_tbl[i] +
       LOG_MASK_ARRAY_INDEX(log_last_item_tbl[i]);
  }
  return (status);
} /* Diag_LSM_Log_Init */

/*===========================================================================

FUNCTION    Diag_LSM_Log_DeInit

DESCRIPTION
  De-Initialize the Diag Log service.

DEPENDENCIES


RETURN VALUE
   boolean indicating success


SIDE EFFECTS


===========================================================================*/
boolean Diag_LSM_Log_DeInit( void )
{
  return (TRUE);
}     /* Diag_LSM_Log_DeInit */

/*===========================================================================
  
FUNCTION    log_mask_is_set
  
DESCRIPTION
  Checks if the log mask is set for this user process
  
DEPENDENCIES
  None.
  
RETURN VALUE
  FALSE if not set, else TRUE
  
SIDE EFFECTS
  None
  
===========================================================================*/
boolean log_mask_is_set(void)
{
  for (int i = 0; i < LOG_MASK_SIZE; i++)
  {
     if (log_mask[i] > 0)
     {
         return TRUE;
     }
  }
  return FALSE;
} /* log_mask_is_set */

/*===========================================================================

FUNCTION    log_update_mask

DESCRIPTION
  Update the log mask.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean log_update_mask( void )
{
  boolean status = TRUE;
  if ( diag_qdi_handle >= DIAG_FD_MIN_VALUE )
  {
    dword maskLen_Recvd = 0;

    if ( !(diag_lsm_ioctl(DIAG_IOCTL_GETLOGMASK, NULL, 0, log_mask, sizeof(log_mask), &maskLen_Recvd)) || maskLen_Recvd != LOG_MASK_SIZE )
    {
      status = FALSE;
    }
  }
  else
  {
    status = FALSE;
  }
  return (status);
} /* log_update_mask */

/*===========================================================================

FUNCTION LOG_STATUS_MASK

DESCRIPTION
  This routine is a wrapper for log_search_mask().  It is used to look up
  the given code in the log mask.

RETURN VALUE
  A boolean indicating if the specified log is enabled.

===========================================================================*/
static boolean
log_status_mask( log_code_type code )
{
  unsigned int id, item;
  boolean status = FALSE;

  id = LOG_GET_EQUIP_ID(code);
  item = LOG_GET_ITEM_NUM(code);

  status = log_search_mask(id, item);
  return (status);
}

/*===========================================================================

FUNCTION LOG_SEARCH_MASK

DESCRIPTION
  This function returns a boolean indicating TRUE if the given ID and 'item'
  denotes a valid and enabled log code.

===========================================================================*/
static boolean log_search_mask(
   unsigned int id,
   unsigned int item
   )
{
  boolean return_val = FALSE;

  /* if valid code val */
  if ( log_mask_enabled(log_mask, id, item) )
  {
    return_val = TRUE;
  }
  return (return_val);

} /* log_search_mask */


/*===========================================================================

FUNCTION LOG_MASK_ENABLED

DESCRIPTION
  This function returns a boolean indicating if the specified code is enabled.

  The equipment ID and item code are passed in to avoid duplicating the
  calculation within this call.  It has to be done for most routines that call
  this anyways.

===========================================================================*/
boolean log_mask_enabled(
   const byte *mask_ptr,
   unsigned int id,
   unsigned int item
   )
{
  unsigned int offset, index, mask;
  boolean enabled = FALSE;

  if ( id <= LOG_EQUIP_ID_LAST && item <= log_last_item_tbl[id] )
  {
    offset = log_mask_offset_tbl[id];
    index = LOG_MASK_ARRAY_INDEX(item);

    mask = LOG_MASK_BIT_MASK(item);


    if ( mask_ptr[offset + index] & mask )
    {
      enabled = TRUE;
    }
  }
  else
  {
    /*Nothing */
  }
  return (enabled);
} /* log_mask_enabled */


