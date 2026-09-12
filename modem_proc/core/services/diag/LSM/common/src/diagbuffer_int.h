#ifndef DIAGBUFFER_INT_H
#define DIAGBUFFER_INT_H
/*==========================================================================

                      Diagnostic Packet Definitions

  Description: diagbuffer defintions


Copyright (c) 2016-2018 by QUALCOMM Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                            Edit History

  $Header: //components/rel/core.mpss/10.0/services/diag/LSM/common/src/diagbuffer_int.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/09/18   sa      Added Non-Public APIs.
03/06/17   ph      diagbuffer API changes and minor updates.
09/29/16   ph      Diag and ULOG integration.
===========================================================================*/

#include "err.h"
#include "queue.h"
#include "log.h"
#include "ULogBuildFeatures.h"
#include "diagbuffer.h"
#include "osal.h"
#include "assert.h"
#include "diagbuf_v.h"
#include "ULogCoreInternal.h"
#include "ULogInlines.h"
#include "diagcmd.h"
#include "diagtarget.h"
#include "diagdiag_v.h"
#include "diaglogi.h"

extern const uint8 msg_v4_dbase_guid[QSHRINK_4_DATABASE_GUID_LENGTH];
extern uint8 qsr_v4_check_bytes(uint8 arg_bytes, uint32 arg);
extern uint32 msg_get_qsr4_hash(uint32 qsr_hash_v4);
extern void qsr_v4_copy_args(uint8 dst[], uint8 src[], uint8 length);

/* diag_wcdma_wl1_frame_cb from diag.c - used to determine TimeStamp type */
extern diag_wcdma_wl1_frame_cb_type diag_wcdma_wl1_frame_cb;


#define DIAGBUFFER_BUFSIZE_MASK(log) (log->bufSizeMask)
#define DIAGBUFFER_BUFSIZE(log)      (log->bufSize)

/* Returns x rounded to the next word boundary */
#define DIAGBUFFER_PADCOUNT(x) ULOGINTERNAL_PADCOUNT(x)

typedef ULOG_TYPE DIAGBUFFER_TYPE;


boolean diagbuffer_drain(diagbuffer_handle handle, uint32* bytes_written);
boolean DIAGBUFFER_IS_EMPTY(diagbuffer_handle h);

#define diagbufferInternal_LockGet(log) \
            ULogInternal_LockGet(log)

#define diagbufferInternal_LockRelease(log) \
            ULogInternal_LockRelease(log)
						
#define diagbufferInternal_UseFastRam(log) \
					ULogInternal_UseFastRam(log)
					
#define diagbufferInternal_FastFmtSizeTS(h, total_size, format, idx_ptr)  \
					ULogInternal_FastFmtSizeTS(h, total_size, format, idx_ptr)

#define DIAGBUFFER_TIMESTAMP_SIZE ULOG_TIMESTAMP_SIZE

// Defines for the logStatus word in the header (ULOG_STRUCT) defined below.
#define DIAGBUFFER_STATUS_DEFINED               ULOG_STATUS_DEFINED               // Set if the log is defined
#define DIAGBUFFER_STATUS_ENABLED               ULOG_STATUS_ENABLED               // Set if the log is enabled
#define DIAGBUFFER_STATUS_MPM_ACTIVE            ULOG_STATUS_MPM_ACTIVE            // Set if the log is currently handling a multi-part message
#define DIAGBUFFER_STATUS_MALLOC_FAIL           ULOG_STATUS_MALLOC_FAIL           // Set if a malloc failed during log creation or use
#define DIAGBUFFER_STATUS_SETUP_FAIL            ULOG_STATUS_SETUP_FAIL            // Set if an initialization of the log failed
#define DIAGBUFFER_STATUS_LOG_SW_READ_STARTED   ULOG_STATUS_LOG_SW_READ_STARTED   // Set the first time the log is read
#define DIAGBUFFER_STATUS_LAST_MSG_WAS_OVERRUN  ULOG_STATUS_LAST_MSG_WAS_OVERRUN  // Set when an overrun occurs during reading

// ULog will try to write directly to the ram buffer in ULogInternal_FastFmtSizeTS
// to save time. These return codes indicate if the write could be done.
#define DIAGBUFFER_FAST_TO_RAM_SUCCESS	ULOG_FAST_TO_RAM_SUCCESS 
#define DIAGBUFFER_FAST_TO_RAM_UNAVAIL	ULOG_FAST_TO_RAM_UNAVAIL 
#define DIAGBUFFER_FAST_TO_RAM_FAIL			ULOG_FAST_TO_RAM_FAIL    

//the length is the top 16 bits of the first word.
#define DIAGBUFFER_LENGTH_MASK 			ULOG_LENGTH_MASK 
#define DIAGBUFFER_LENGTH_SHIFT			ULOG_LENGTH_SHIFT 


/**
 * <!-- diagbuffer_set_attribute -->
 *
 * @brief Set the DiagBuffer attributes
 * This API replaces ULogCore_AttributeSet() of ULog.
 *
 * OR in the provided attribute bits into the logs attribute mask.
 *
 * @param         h : The DiagBuffer handle to set attributes for
 * @param attribute : The attributes to set
 *
 * @return A handle to the named DiagBuffer
 */
#define diagbuffer_set_attribute(h, attribute)   \
            ULogCore_AttributeSet(h, (ULOG_ATTRIBUTE_TYPE) attribute)
            

/**
 * <!-- diagbuffer_get_handle -->
 *
 * @brief Get a DiagBuffer handle by its name.
 *  This API replaces ULogCore_HandleGet() of ULog.
 *
 * @param       bufferName : Name of a DiagBuffer to search for.
 *
 * @return      A handle to the named DiagBuffer
 */
#define diagbuffer_get_handle(bufferName)   \
            ULogCore_HandleGet(bufferName)
            

#endif
