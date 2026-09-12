/*============================================================================
@file DiagBuffer.h

Diag shim for ULog.

Copyright (c) 2015, 2017-2018 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
============================================================================*/

/*===========================================================================
                              Edit History

 $Header: //components/rel/core.mpss/10.0/api/services/diagbuffer.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/05/18   sa      Removed Non-Public APIs.
03/06/17   ph      diagbuffer API changes and minor updates.
05/28/15   vk      Corrected typo in a write macro name
01/22/15   vk      Created file.
===========================================================================*/

#ifndef DIAGBUFFER_H
#define DIAGBUFFER_H

#include "diagbuffer_defs.h"
#include "ULog.h"
#include "ULogFront.h"

#ifdef __cplusplus
extern "C" {
#endif

/* DiagBuffer constants */
#define DIAGBUFFER_MAX_NAME_SIZE ULOG_MAX_NAME_SIZE

/*============================================================================
 * Read and Format
 *===========================================================================*/

// Values for the outputFormat entry of ulogdiag_cmd_type_read
#define DIAGBUFFER_FORMAT_ASCII    									0
#define DIAGBUFFER_FORMAT_BINARY   									1


// the following definitions are used for the outputFormat fields
// of the diag cmd read request response. They indicate the ususal format
// field and indicate that the last message in the read was invalid
#define DIAGBUFFER_FORMAT_ASCII_OVERRUN_DETECTED     2
#define DIAGBUFFER_FORMAT_BINARY_OVERRUN_DETECTED    3
#define DIAGBUFFER_FORMAT_DIAG     									 4
#define DIAGBUFFER_FORMAT_DIAG_NHDLC                 5
#define DIAGBUFFER_FORMAT_DIAG_OVERRUN_DETECTED      6


/* The following are different modes that diag can set to*/
#define DIAGBUFFER_MODE_STREAM  0
#define DIAGBUFFER_MODE_CBUFFER 2
#define DIAGBUFFER_MAX_MODE     2

/*
 Error Codes

 These Error codes are return values from DiagBuffer functions.
 DiagBuffer functions additionally use the standard DAL return codes.
 DAL_SUCCESS is the return value to test for success in most operations.
 DAL_ERROR and the values below are returned by functions.
 Error values 10,000 to 20,000 are reserved for DiagBuffer Diag Plugin usage.
*/
typedef enum
{
  DIAGBUFFER_ERR_INVALIDNAME = 1,       /*  The name provided is invalid. */
  DIAGBUFFER_ERR_INVALIDPARAMETER,      /*  A parameter provided is invalid. */
  DIAGBUFFER_ERR_ALREADYCONNECTED,      /*  The handle is already connected to a log. */
  DIAGBUFFER_ERR_ALREADYCREATED,        /*  The log has already been created. */
  DIAGBUFFER_ERR_NOTCONNECTED,          /*  The handle is not connected to a log. */
  DIAGBUFFER_ERR_ALREADYENABLED,
  DIAGBUFFER_ERR_INITINCOMPLETE,
  DIAGBUFFER_ERR_READONLY,
  DIAGBUFFER_ERR_INVALIDHANDLE,
  DIAGBUFFER_ERR_MALLOC,                /*  Memory allocation failed */
  DIAGBUFFER_ERR_ASSIGN,                /*  Error setting memory in the DiagBuffer */
  DIAGBUFFER_ERR_INSUFFICIENT_BUFFER,   /*  Buffer provided to a read was insufficient
                                            for even a single item to be copied into*/
  DIAGBUFFER_ERR_NAMENOTFOUND,          /*  Could not find a log of that name. */
  DIAGBUFFER_ERR_MISC,                  /*  Errors that don't fit the other categories */
  DIAGBUFFER_ERR_OVERRUN,               /*  Overrun condition detected while reading */
  DIAGBUFFER_ERR_NOTSUPPORTED,          /*  Response to an unsupported request */
} DIAGBUFFER_ERRORS;


/*  Buffer sizes that can be queried in order to manually
 allocate memory for a DiagBuffer.
 See: diagbuffer_query_value
*/
typedef enum
{
  /* These queries do not require a DiagBuffer handle */
  DIAGBUFFER_VALUE_BUFFER,
  DIAGBUFFER_VALUE_SHARED_HEADER,
  DIAGBUFFER_VALUE_READCORE,
  DIAGBUFFER_VALUE_WRITECORE,
  DIAGBUFFER_VALUE_LOCKCORE,

  /* Below here the queries require a DiagBuffer handle */
  DIAGBUFFER_VALUE_LOG_BASE = 0x100,
  DIAGBUFFER_VALUE_LOG_READFLAGS = DIAGBUFFER_VALUE_LOG_BASE,
} DIAGBUFFER_VALUE_ID;


/*
 Lock Types

 Each log can select the locking mechanism used to protect data
 accesses.  It is up to the DiagBuffer definer to select the appropriate
 lock type depending on the use cases.
*/
typedef enum
{
  DIAGBUFFER_LOCK_NONE = 0,              /*  No locking is provided for this
                                            DiagBuffer. The user is responsible for
                                            providing mutual exclusion if logging
                                            from multiple threads.
                       */

  DIAGBUFFER_LOCK_OS,                    /* An OS lock is provided. The DiagBuffer can
                                            be written from more than one thread.
                                            OS locks are assumed to work across
                                            cores in the case where one OS is
                                            managing all cores.
                     */

  DIAGBUFFER_LOCK_SPIN,                  /* Spinlocks are NOT currently supported.
                                            An error will be returned.
                     */

  DIAGBUFFER_LOCK_INT,                   /* An interrupt lock is provided. The DiagBuffer
                                           can be written from more than one thread.
                                           Interrupt locks are NOT multi-core safe.
                     */

  DIAGBUFFER_LOCK_UINT32 = 0x7FFFFFFF    /* force enum to 32-bits */
} DIAGBUFFER_LOCK_TYPE;


/*
    Memory Type
    Control how memory is provided for the DiagBuffer.
*/
typedef enum
{
  DIAGBUFFER_ALLOC_LOCAL = 0x01,         /* Memory will be allocated using
                                            DALSYS_Malloc.
                     */

  DIAGBUFFER_ALLOC_MANUAL = 0x04,        /* Memory allocation is handled
                                            manually, and is provided to DiagBuffer
                                            through the diagbuffer_AssignMemory
                                            function.
                     */

  DIAGBUFFER_ALLOC_TYPE_MASK = 0xFF,     /* Mask for all possible allocation bit
                                            types.
                     */

  DIAGBUFFER_ALLOC_UINT32 = 0x7FFFFFFF   /* force enum to 32-bits */
} DIAGBUFFER_ALLOC_TYPE;


/*
   Memory Usage
   Control the amount and type of memory access available to the DiagBuffer.
*/
typedef enum
{
  DIAGBUFFER_MEMORY_USAGE_FULLACCESS = 0x0300,    /* The processor is expected to read and
                                                     write from this log.
                          */

  DIAGBUFFER_MEMORY_USAGE_READABLE = 0x0100,      /* The processor will only read from this
                                                     log.
                          */

  DIAGBUFFER_MEMORY_USAGE_WRITEABLE = 0x0200,     /* The processor will only write to this
                                                     log.
                          */

  DIAGBUFFER_MEMORY_USAGE_UINT32 = 0x7FFFFFFF     /* force enum to 32-bits */
} DIAGBUFFER_MEMORY_USAGE_TYPE;


/*
   Memory Configuration
   Control if the memory is configured and used as shared
 */
typedef enum
{
  DIAGBUFFER_MEMORY_CONFIG_LOCAL = 0x010000,      /* Header information is stored in local
                                                     memory. Because the DiagBuffer is used
                                                     only by one processor (or a symmetrical
                                                     multi-processing system), there is no
                                                     need to store the DiagBuffer in any other
                                                     location.
                          */

  DIAGBUFFER_MEMORY_CONFIG_SHARED = 0x020000,     /* Header information is stored in the
                                                     allocated buffer memory. This configuration
                                                     is not currently supported and will
                                                     return DIAGBUFFER_ERR_NOTSUPPORTED
                          */
  DIAGBUFFER_MEMORY_CONFIG_UINT32 = 0x7FFFFFFF    /* force enum to 32-bits */
} DIAGBUFFER_MEMORY_CONFIG_TYPE;


/* Memory type

 Memory type is a parameter specifying the allocation, usage, and configuration
 of the memory used by the DiagBuffer. Each DiagBuffer must specify one of each of the
 DIAGBUFFER_ALLOC_TYPE, DIAGBUFFER_MEMORY_USAGE_TYPE, and DIAGBUFFER_MEMORY_CONFIG_TYPE
 values.
 It is up to the DiagBuffer creator to select the appropriate memory type.
 Standard memory allocation types are provided below.
 Most DiagBuffers will be created as local DiagBuffers.
*/
#define DIAGBUFFER_MEMORY_LOCAL        (DIAGBUFFER_MEMORY_CONFIG_LOCAL | DIAGBUFFER_MEMORY_USAGE_FULLACCESS | DIAGBUFFER_ALLOC_LOCAL)
#define DIAGBUFFER_MEMORY_LOCAL_MANUAL (DIAGBUFFER_MEMORY_CONFIG_LOCAL | DIAGBUFFER_MEMORY_USAGE_FULLACCESS | DIAGBUFFER_ALLOC_MANUAL)

/* Shared Memory type logs are not supported and will return DIAGBUFFER_ERR_NOTSUPPORTED */
#define DIAGBUFFER_MEMORY_SHARED       (DIAGBUFFER_MEMORY_CONFIG_SHARED | DIAGBUFFER_MEMORY_USAGE_FULLACCESS | DIAGBUFFER_ALLOC_MANUAL)


/* Optional timestamp source selection, if NULL default will be used */
typedef ULOG_ALT_TS_SRC DIAGBUFFER_ALT_TS_SRC;

/*
 Interface Types

 Interface Types are used by the logging core.
 Generally logging into a DiagBuffer is only of one type, but as of 7/2012 messages
 of either type (Raw or RealTime) can be written to a DiagBuffer.
 Data should only be written into the buffer using the following APIs.
 diagbuffer_create, diagbuffer_write_raw, diagbuffer_create, diagbuffer_static_create,
 _diagbuffer_printf, _diagbuffer_static_printf, diagbuffer_vprintf, _diagbuffer_write_data,
 diagbuffer_write_char_array, diagbuffer_write_string, diagbuffer_write_word_array,
 diagbuffer_write_csv, diagbuffer_write_vector.
*/

typedef enum
{
  DIAGBUFFER_INTERFACE_INVALID = 0,           /* Invalid DiagBuffer Type */

  DIAGBUFFER_INTERFACE_RAW,                   /* Raw data writing provided in
                                                 DiagBuffer.h/ULogFront.h
                        */

  DIAGBUFFER_INTERFACE_REALTIME,              /* Real-Time data writing provided in
                                                 DiagBuffer.h/ULogFront.h.
                        */

  DIAGBUFFER_INTERFACE_UINT32 = 0x7FFFFFFF    /* force enum to 32-bits */
} DIAGBUFFER_INTERFACE_TYPE;


/*
   DiagBuffer Message Format Types

   These values are stored within every message to indicate what the message
   type being recorded is.
*/
typedef enum
{
  DIAGBUFFER_REALTIME_SUBTYPE_RESERVED_FOR_RAW = 0,  /* RAW entries set the msg format to 0 */
  DIAGBUFFER_REALTIME_SUBTYPE_PRINTF,
  DIAGBUFFER_REALTIME_SUBTYPE_BYTEDATA,
  DIAGBUFFER_REALTIME_SUBTYPE_STRINGDATA,
  DIAGBUFFER_REALTIME_SUBTYPE_WORDDATA,
  DIAGBUFFER_REALTIME_SUBTYPE_CSVDATA,
  DIAGBUFFER_REALTIME_SUBTYPE_VECTOR,
  DIAGBUFFER_REALTIME_SUBTYPE_MULTIPART,
  DIAGBUFFER_REALTIME_SUBTYPE_MULTIPART_STREAM_END,  /* only used for streaming QDSS or ALT data streams */

  DIAGBUFFER_SUBTYPE_REALTIME_TOKENIZED_STRING,
  DIAGBUFFER_SUBTYPE_RESERVED1,
  DIAGBUFFER_SUBTYPE_RESERVED2,
  DIAGBUFFER_SUBTYPE_RESERVED3,
  DIAGBUFFER_SUBTYPE_RAW8,                           /* Raw output formatted as uint8s  */
  DIAGBUFFER_SUBTYPE_RAW16,                          /* Raw output formatted as uint16s  */
  DIAGBUFFER_SUBTYPE_RAW32,                          /* Raw output formatted as uint32s */

  DIAGBUFFER_SUBTYPE_DIAG_MSG_QSR4,
  DIAGBUFFER_SUBTYPE_DIAG_MSG_QSR3,
  DIAGBUFFER_SUBTYPE_DIAG_MSG_QSR2,
  DIAGBUFFER_SUBTYPE_DIAG_MSG,
  DIAGBUFFER_SUBTYPE_DIAG_MSG_SPRINTF,
  DIAGBUFFER_SUBTYPE_DIAG_LOG,
  DIAGBUFFER_SUBTYPE_DIAG_EVENT,
  DIAGBUFFER_SUBTYPE_DIAG_EVENT_PAYLOAD,

  DIAGBUFFER_REALTIME_SUBTYPE_UINT32 = 0x7FFFFFFF    /* force enum to 32-bits */
} DIAGBUFFER_REALTIME_SUBTYPES;


/* The first word of each msg is len/fmt */
#define DIAGBUFFER_LEN_MASK 0xffff0000
#define DIAGBUFFER_FMT_MASK 0x0000ffff


/*
   DiagBuffer Status Indicators
   DiagBuffer status is returned as the first word from the diagbuffer_Read functions.
*/
typedef enum
{
  DIAGBUFFER_LOG_WRAPPED = 0x0001,         /* Not an error condition. Just an
                                              indicator that the DiagBuffer has wrapped
                                              at least once.
                       */

  DIAGBUFFER_ERR_LARGEMSG = 0x0002,        /* A message too large for the buffer
                                              was written, and therefore was dropped.
                                              This indicates the message was too
                                              large for the entire buffer.
                       */

  DIAGBUFFER_ERR_LARGEMSGOUT = 0x0004,     /* A message too large for the output
                                              buffer was dropped because it could
                                              not fit.
                       */

  DIAGBUFFER_ERR_RESET = 0x0008,           /* The DiagBuffer has been reset at least once. */

  DIAGBUFFER_ERR_UINT32 = 0x7FFFFFFF       /* force enum to 32-bits */
} DIAGBUFFER_WRITER_STATUS_TYPE;


/*
 ReadFlags Indicators
 ReadFlags is returned from a query.
*/
typedef enum
{
  DIAGBUFFER_RD_FLAG_REWIND = 0x0001,      /* The read pointer was rewound
                                              by the Auto rewind feature.
                       */

  DIAGBUFFER_RD_FLAG_UINT32 = 0x7FFFFFFF   /* force enum to 32-bits */
} DIAGBUFFER_RD_FLAG_TYPE;


/*
  Attribute Types
  DiagBuffers can be customized with the following attributes.
 */
typedef enum
{
  DIAGBUFFER_ATTRIBUTE_READ_AUTOREWIND = 0x1,  /* Rewind the read pointer for this DiagBuffer
                                                  to the earliest message after the entire
                                                  DiagBuffer has been read, and zero bytes have
                                                  been transferred.
                         */

  DIAGBUFFER_ATTRIBUTE_UINT32 = 0x7FFFFFFF    /* force enum to 32-bits */
} DIAGBUFFER_ATTRIBUTE_TYPE;


/*
  DiagBuffer Core VTable Struct
  Different types of write operations (Tee, STM, alternate) can be defined
  using different functions in this table.
 */
typedef struct
{
  diagbuffer_result (*write) (diagbuffer_handle h, uint32 firstMsgCount, const char * firstMsgContent, uint32 secondMsgCount, const char * secondMsgContent, DIAGBUFFER_INTERFACE_TYPE interfaceType);
  DALBOOL (*multipartMsgBegin) (diagbuffer_handle h);
  void (*multipartMsgEnd) (diagbuffer_handle h);
} DIAGBUFFER_CORE_VTABLE;

/*----------------------------------------------------------------------------
 * Function Declarations and Documentation
 * -------------------------------------------------------------------------*/

/**
 * <!-- diagbuffer_create -->
 *
 * @brief  - Create a new DiagBuffer.
 * This API replaces ULogCore_LogCreate(), ULogFront_RawInit() and
 * ULogFront_RealTimeInit() of ULog.
 *
 * Create a new DiagBuffer, and add it to the buffers managed by the DiagBuffer
 * subsystem.
 *
 * @param             h : A DiagBuffer handle is returned at this pointer.
 * @param    bufferName : The buffer name (24 chars max, including NULL).
 * @param    bufferSize : The size in bytes of the diag buffer. A request
 *                        of size 0 will leave the buffer unallocated.
 * @param    memoryType : The memory configuration.
 * @param      lockType : The type of locking required.
 *
 * @return           DAL_SUCCESS   : If the create completed successfully.  The diag
 *                                   buffer will have been enabled and will be ready
 *                   to use.
 *   DIAGBUFFER_ERR_INITINCOMPLETE : If DIAGBUFFER_MEMORY_LOCAL_MANUAL is used.
 *   DIAGBUFFER_ERR_INITINCOMPLETE : If a buffersize of 0 is used.
 *                                   The log can be enabled later after diagbuffer_assign_memory
 *                                   sets up all the parts. Errors are defined in the
 *                                   DIAGBUFFER_ERRORS enum.
 */
#define diagbuffer_create(h, bufferName, bufferSize, memoryType, lockType)   \
            ULogCore_LogCreate(h, bufferName, bufferSize, memoryType, (ULOG_LOCK_TYPE) lockType, ULOG_INTERFACE_REALTIME)


/**
 * <!-- diagbuffer_set_lock_type -->
 *
 * @brief Switches the lock type for a Diag Buffer.
 * This API replaces ULogCore_SetLockType() of ULog.
 *
 * @param h        : A DiagBuffer handle.
 * @param lockType : The desired lock type for the log
 *
 */
#define diagbuffer_set_lock_type(h, lockType)   \
            ULogCore_SetLockType(h, (ULOG_LOCK_TYPE) lockType)


/**
 * <!-- diagbuffer_set_header -->
 *
 * @brief Set the header for the DiagBuffer.
 * This API replaces ULogCore_HeaderSet() of ULog.
 *
 * Apply the supplied string to a given DiagBuffer header member variable.
 * The header is stored with a DiagBuffer, and will not be overwritten
 * unless this function is called again.
 *
 * There are no real limitations to this string. The system
 * determines the length of the supplied string, allocates that
 * amount of memory, and copies the string to it.
 *
 * @param            h : A DiagBuffer handle.
 * @param   headerText : The string to store in the log.
 *
 * @return DAL_SUCCESS : if the header was successfully stored.
 */
#define diagbuffer_set_header(h, headerText)   \
            ULogCore_HeaderSet(h, headerText)

/**
 * <!-- diagbuffer_assign_memory -->
 *
 * @brief Assign memory to specific buffers in DiagBuffer.
 * This API replaces ULogCore_MemoryAssign() of ULog.
 *
 * When a DiagBuffer is created with the allocation type
 * diagbuffer_ALLOC_MANUAL, the buffers it encapsulate must be
 * manually assigned memory with this function. All supplied
 * buffers must be word aligned. If a buffer is not word aligned,
 * this function will fail.
 *
 * @param            h : A DiagBuffer handle.
 * @param           id : Which buffer to assign.
 * @param    bufferPtr : A pointer to the buffer memory to assign.
 *                       Note that ALL buffers must be word aligned.
 * @param   bufferSize : The size in bytes of the buffer.
 *
 * @return DAL_SUCCESS : if the memory was assigned successfully.
 *                       Errors are defined in the DIAGBUFFER_ERRORS
 *                       enum.
 */
#define diagbuffer_assign_memory(h, id, bufferPtr, bufferSize)   \
            ULogCore_MemoryAssign(h, (ULOG_VALUE_ID) id, bufferPtr, bufferSize)


/**
 * <!-- diagbuffer_enable -->
 *
 * @brief Enable a DiagBuffer that is already defined.
 * This API replaces ULogCore_Enable() of ULog.
 *
 * Enable a DiagBuffer. A DiagBuffer can need manual enabling if it has
 * manual allocation, has been disabled, or buffer memory was not assigned.
 *
 * @param            h : A DiagBuffer handle.
 *
 * @return DAL_SUCCESS : if the log was enabled successfully.
 *                       Errors are defined in the DIAGBUFFER_ERRORS
 *                       enum.
 */
#define diagbuffer_enable(h)   \
            ULogCore_Enable(h)


/**
 * <!-- diagbuffer_is_enabled -->
 *
 * @brief Check if a DiagBuffer is enabled.
 * This API replaces ULogCore_IsEnabled() of ULog.
 *
 * @param            h : A DiagBuffer handle.
 * @param         core : A vtable to fill in with the appropriate
 *                       interface functions for the current log
 *
 * @return        TRUE : if the DiagBuffer is enabled.
 */
#define diagbuffer_is_enabled(h, core)   \
            ULogCore_IsEnabled(h, core)


/**
 * <!-- diagbuffer_read -->
 *
 * @brief Read data from the buffer in the DiagBuffer.
 * This API replaces ULogCore_Read() and ULogCore_ReadEx of ULog.
 *
 * Read data from the buffer in the DiagBuffer(represented by h).
 * This data is unformatted. Call diagbuffer_convert_to_ascii to format each message.
 * Only full messages are placed in the output buffer, thus it is incorrect
 * to use outputCount != outputSize as an indicator that the buffer in the
 * DiagBuffer is empty.
 *
 * An empty buffer will yield a read of only status, and an outputCount = 4.
 *
 * @param                  h : A DiagBuffer handle of the buffer to read.
 * @param         outputSize : Size of the output buffer.
 * @param          outputMem : A pointer to the output buffer.
 * @param        outputCount : The amount of data placed in the output buffer.
 * @param outputMessageLimit : The maximum number of messages to read from
 *                             the buffer.
 *                             Passing 0 would cause it to read as many messages as
 *                             can fit in passed buffer.
 *                             Value 0 is used in legacy ULogCore_Read()
 *
 * @return       DAL_SUCCESS : if the read completed.
 */
#define diagbuffer_read(h, outputSize, outputMem, outputCount, outputMessageLimit)   \
            ULogCore_ReadEx(h, outputSize, outputMem, outputCount, outputMessageLimit)


/**
 * <!-- diagbuffer_allocate_memory -->
 *
 * @brief Allocate memory to the buffer encapsulated in DiagBuffer.
 * This API replaces ULogCore_Allocate() of ULog.
 *
 * When a DIagBuffer is created with no output buffer, this function can
 * be used to allocate that memory.  It is intended for use from
 * applications enabling and reading DiagBuffer from the system.
 *
 * This only works for DiagBuffers created with DIAGBUFFER_ALLOC_LOCAL
 * allocation type.
 *
 * @param                  h : A DiagBuffer handle of the log to read.
 * @param         bufferSize : Size of the circular buffer to allocate.
 *
 * @return       DAL_SUCCESS : if the memory was allocated successfully.
 *                             Errors are defined in the DIAGBUFFER_ERRORS
 *                             enum.
 */
#define diagbuffer_allocate_memory(h, bufferSize)   \
            ULogCore_Allocate(h, bufferSize)


/**
 * <!-- diagbuffer_disable -->
 *
 * @brief Disable a DiagBuffer. Drop message writes until re-enabled.
 * This API replaces ULogCore_Disable() of ULog.
 *
 * @param                  h : A DiagBuffer handle.
 *
 * @return       DAL_SUCCESS : if the log was disabled successfully.
 *                             Errors are defined in the DIAGBUFFER_ERRORS
 *                             enum.
 */
#define diagbuffer_disable(h)   \
            ULogCore_Disable(h)


 /**
 * <!-- diagbuffer_convert_to_ascii -->
 *
 * @brief Format an individual message retrieved with the diagbuffer_read function.
 * This API replaces ULogCore_MsgFormat() of ULog.
 *
 * Format and output an individual message read with the
 * diagbuffer_read function.
 *
 * @param               h : A DiagBuffer handle to the buffer we are formatting.
 * @param             msg : The input message.
 * @param       msgString : The output string.
 * @param   msgStringSize : The size of the output string
 * @param     msgConsumed : Number of bytes consumed from the original msg buffer.
 *
 * @return    DAL_SUCCESS : if the format was successful. Errors are
 *                          defined in the DIAGBUFFER_ERRORS enum.
 */
#define diagbuffer_convert_to_ascii(h, msg, msgString, msgStringSize, msgConsumed)   \
            ULogCore_MsgFormat(h, msg, msgString, msgStringSize, msgConsumed)


/**
 * <!-- diagbuffer_get_header -->
 *
 * @brief Read the header from the supplied DiagBuffer.
 * This API replaces ULogCore_HeaderRead() of ULog.
 *
 * @param                h : A handle of the DiagBuffer to access.
 * @param     statusString : The output string.
 * @param statusStringSize : The size of the output string
 *
 * @return     DAL_SUCCESS : if the format was successful. Errors are
 *                           defined in the DIAGBUFFER_ERRORS enum.
 */
#define diagbuffer_get_header(h, headerReadOffset, headerString, headerStringSize, headerActualLength)   \
            ULogCore_HeaderRead(h, headerReadOffset, headerString, headerStringSize, headerActualLength)


/**
 * <!-- diagbuffer_connect -->
 *
 * @brief Connect a DiagBuffer handle to a buffer defined elsewhere.
 * This API replaces ULogCore_Connect() of ULog.
 *
 * Connect a DiagBuffer handle to a buffer that has been, or will be defined
 * elsewhere. A successful return allows logging through the handle, although
 * no data will be written to a physical buffer until the buffer is defined
 * with the diagbuffer_create function.
 *
 * @param                h : A DiagBuffer handle.
 * @param       bufferName : The name of the buffer to connect with.
 *
 * @return     DAL_SUCCESS : if the connect was successful. Errors are
 *                           defined in the DIAGBUFFER_ERRORS enum.
 */
#define diagbuffer_connect(h, bufferName)   \
            ULogCore_Connect(h, bufferName)


/**
 * <!-- ULOG_FILTER_CMD -->
 *
 * @brief Filter a DiagBuffer command based on the filter level.
 * This API replaces ULOG_FILTER_CMD() of ULog.
 *
 * DiagBuffer does not filter messages, but this macro is intended to
 * facilitate filtering without requiring it. This macro compares the
 * provided level against the level of the provided log. If the
 * provided level is <= the log level then the provided function
 * is executed.
 *
 * @param           handle : A DiagBuffer handle. The level is retrieved
 *                           from this handle.
 * @param            level : The level of this command.
 * @param         function : The function to execute if the levels indicate
 *                           acceptance.
 *
 * @return        The current log value.
 *                0 if the log handle is NULL.
 */
#define DIAGBUFFER_FILTER_MACRO(handle, level, function)    (if (level <= ULogCore_LogLevelGet(handle)) { function }


/**
 * <!-- diagbuffer_add_tee -->
 *
 * @brief Group handles together.
 *  This API replaces ULogCore_TeeAdd() of ULog.
 *
 * It group handles together so that any logging information will be copied to
 * multiple DiagBuffers.
 *
 * Note: the first handle may points to an updated object after a call to
 * this function, whereas the second one will not.
 *
 * @param               h1 : A DiagBuffer handle, which may be one to a single
 *                           or a group of logs
 * @param               h2 : A DiagBuffer handle, which may be one to a single
 *                           or a group of logs to be added to h1
 *
 * @return              A handle to the grouped logs
 */
#define diagbuffer_add_tee(h1, h2)   \
            ULogCore_TeeAdd(h1, h2)


/**
 * <!-- diagbuffer_remove_tee -->
 *
 * @brief Remove some handle(s)
 *  This API replaces ULogCore_TeeRemove() of ULog.
 *
 * Note: the first handle may points to an updated object after a call to
 * this function, whereas the second one will not.
 *
 * @param               h1 : A DiagBuffer handle, which may be one to a single
 *                           or a group of logs
 * @param               h2 : A DiagBuffer handle, which may be one to a single
 *                           or a group of logs to be removed from h1
 *
 * @return              A handle to the resulting group of logs
 */
#define diagbuffer_remove_tee(h1, h2)   \
            ULogCore_TeeRemove(h1, h2)

/**
 * <!-- diagbuffer_set_timestamp_src_fn -->
 *
 * @brief Allows the source of the timestamp to be altered by the user
 *  This API replaces ULogCore_SetTimestampSrcFn() of ULog.
 *
 * @param                  h : A DiagBuffer Handle
 * @param altULogTimeStampFn : Fn pointer to the users timestamp source fn
 *
 * @return       DAL_SUCCESS : if the timestamp function was correctly set
 *                       Errors are defined in the DIAGBUFFER_ERRORS enum
 *
 */
#define diagbuffer_set_timestamp_src_fn(h, altULogtimeStampFn)   \
            ULogCore_SetTimestampSrcFn(h, altULogTimeStampFn)


/**
 * <!-- diagbuffer_set_transport_to_ram -->
 *
 * @brief Set transport for DiagBuffer to RAM i.e. Write log messages to the
 *        circular RAM buffer.
 *  This API replaces ULogCore_SetTransportToRAM() of ULog.
 *
 * Writing messages to the circular RAM buffer is the default behaviour for DiagBuffer.
 *
 * @param                  h : A DiagBuffer handle
 *
 * @return       DAL_SUCCESS : if the transport is changed,
                   DAL_ERROR : if it isn't
 */
#define diagbuffer_set_transport_to_ram(h, altuLogTimerStampFn)   \
            ULogCore_SetTransportToRAM(h)


/**
 * <!-- diagbuffer_set_transport_to_stm -->
 *
 * @brief Write debug messages to the QDSS/STM transport
 *  This API replaces ULogCore_SetTransportToStm() and ULogCore_SetTransportToStmAscii of ULog.
 *
 * @param                  h : A DiagBuffer handle
 * @param       protocol_num : A QDSS protocol number (0 to 127) is assigned to the DiagBuffer
 *                             handle and this protocol number can be used to match logs to
 *                             the handle when reading the QDSS output.
 *                      mode : 0 for binary
 *                             1 for  ASCII
 *
 * @return       DAL_SUCCESS : if the transport is changed,
 *                 DAL_ERROR : if it isn't
 */
typedef enum
{
   DIAGBUFFER_STM_BINARY = 0,
   DIAGBUFFER_STM_ASCII  = 1
}DIAGBUFFER_STM_MODES;

#define diagbuffer_set_transport_to_stm(h, protocol_num, mode)({\
\
  DALResult result = DAL_ERROR;\
  if (mode == DIAGBUFFER_STM_BINARY)\
    result = ULogCore_SetTransportToStm(h, protocol_num);\
  else if(mode == DIAGBUFFER_STM_ASCII)\
    result = ULogCore_SetTransportToStmAscii(h, protocol_num);\
  (result);\
})


/**
 * <!-- diagbuffer_set_transport_to_alt -->
 *
 * @brief Write debug messages using the provided handlers
 *  This API replaces ULogCore_SetTransportToAlt() of ULog.
 *
 * @param                  h : A DiagBuffer handle
 * @param  newTansportVTable : A Write, MultipartStart and MultipartEnd function table.
 *
 * @return       DAL_SUCCESS : if the transport is changed,
 *                 DAL_ERROR : if it isn't
 */
#define diagbuffer_set_transport_to_alt(h, newTransportVTable)   \
            ULogCore_SetTransportToAlt(h, newTansportVTable)


/*
  The following DiagBuffer Diag plugin ID's are explicitly reserved for current
  and future use.
*/
#define DIAGBUFFER_DIAG_PLUGIN_ID_NPA         ULOG_DIAG_PLUGIN_ID_NPA
#define DIAGBUFFER_DIAG_PLUGIN_ID_MCA         ULOG_DIAG_PLUGIN_ID_MCA
#define DIAGBUFFER_DIAG_PLUGIN_ID_RESERVED3   ULOG_DIAG_PLUGIN_ID_RESERVED3
#define DIAGBUFFER_DIAG_PLUGIN_ID_RESERVED4   ULOG_DIAG_PLUGIN_ID_RESERVED4
#define DIAGBUFFER_DIAG_PLUGIN_ID_RESERVED5   ULOG_DIAG_PLUGIN_ID_RESERVED5
#define DIAGBUFFER_DIAG_PLUGIN_ID_RESERVED6   ULOG_DIAG_PLUGIN_ID_RESERVED6
#define DIAGBUFFER_DIAG_PLUGIN_ID_RESERVED7   ULOG_DIAG_PLUGIN_ID_RESERVED7
#define DIAGBUFFER_DIAG_PLUGIN_ID_RESERVED8   ULOG_DIAG_PLUGIN_ID_RESERVED8


/* shim for ULogFront.h */
/**
 * <!-- diagbuffer_write_raw -->
 *
 * @brief Copy data from srcDataArray to the raw buffer in DiagBuffer.
 *  This API replaces ULogFront_RawLog() of ULog.
 *
 * @param                h : A DiagBuffer handle.
 * @param     srcDataArray : A pointer to the data.
 * @param     srcDataCount : The amount of data to log.
 *
 * @return     DAL_SUCCESS : if the message was logged successfully.
 *                           Errors are defined in the DIAGBUFFER_ERRORS enum.
*/
#define diagbuffer_write_raw(h, srcDataArray, srcDataCount)   \
            ULogFront_RawLog(h, srcDataArray, srcDataCount)

/*----------------------------------------------------------------------------
 * ULOG_INTERFACE_REALTIME
 * -------------------------------------------------------------------------*/

/** The value for x MUST be a 4 byte "float" variable.  It cannot be immediate data or an 8 byte "double" */
#define DIAGBUFFER_RT_FLOAT(x) (*((uint32 *)((void *)&x)))

/** Macros for splitting up a 64 bit value */
#define DIAGBUFFER64_LOWWORD(x) (uint32)x
#define DIAGBUFFER64_HIGHWORD(x) (uint32)((uint64)x>>32)
#define DIAGBUFFER64_DATA(x) (uint32)x, (uint32)((uint64)x>>32)


/**
 * <!-- diagbuffer_static_init -->
 *
 * @brief Create a new DiagBuffer in the Static format.
 *  This API replaces ULogFront_RealTimeStaticInit() of ULog.
 *
 * Create a new RealTime log, See diagbuffer_LogCreate for details.
 *
 * @param                h : Log handle to be filled in.
 * @param          logName : Ascii log name (must be less than 24 chars).
 * @param  logMessageSlots : How many slots are needed for messages. Max is 9.
 * @param       memoryType : Not used currently. Use DIAGBUFFER_MEMORY_LOCAL.
 * @param         lockType : DIAGBUFFER_LOCK_OS or DIAGBUFFER_LOCK_NONE
 *
 * @return DAL_SUCCESS if the create completed successfully.
 */
#define diagbuffer_static_create(h, name, logMessageSlots, logBufMemType, logLockType)   \
            ULogFront_RealTimeStaticInit(h, name, logMessageSlots, logBufMemType, logLockType)


/**
 * <!-- _diagbuffer_printf -->
 *
 * Do not use this API. Use the DIAGBUFFER_PRINTF_n macros.
 *  This API replaces ULogFront_RealTimePrintf() of ULog.
 */
#define _diagbuffer_printf(h, dataCount, formatStr, ... )   \
            ULogFront_RealTimePrintf(h, dataCount, formatStr, ##__VA_ARGS__)


/**
 * <!-- DIAGBUFFER_PRINTF_N -->
 *
 * @brief Macros to provide some parameter count safety to the _diagbuffer_printf function.
 *  These replaces DIAGBUFFER_PRINTF_N() of ULog.
 *
 * These macros provide parameter count safety to the _diagbuffer_printf
 * function. These should be used instead of calling the function directly.
 *
 * @param                h : A handle indicating where to log.
 * @param        formatStr : The format string for the printf.
 * @param          p1 - pN : The data to log.  These must be (u)int32 in
 *                           length.  Smaller data types will be automatically
 *                           promoted.
 */
#define DIAGBUFFER_PRINTF_0(h, formatStr)    \
            _diagbuffer_printf(h, 0, formatStr)
#define DIAGBUFFER_PRINTF_1(h, formatStr, p1)    \
            _diagbuffer_printf(h, 1, formatStr, p1)
#define DIAGBUFFER_PRINTF_2(h, formatStr, p1, p2)    \
            _diagbuffer_printf(h, 2, formatStr, p1, p2)
#define DIAGBUFFER_PRINTF_3(h, formatStr, p1, p2, p3)    \
            _diagbuffer_printf(h, 3, formatStr, p1, p2, p3)
#define DIAGBUFFER_PRINTF_4(h, formatStr, p1, p2, p3, p4)    \
            _diagbuffer_printf(h, 4, formatStr, p1, p2, p3, p4)
#define DIAGBUFFER_PRINTF_5(h, formatStr, p1, p2, p3, p4, p5)    \
            _diagbuffer_printf(h, 5, formatStr, p1, p2, p3, p4, p5)
#define DIAGBUFFER_PRINTF_6(h, formatStr, p1, p2, p3, p4, p5, p6)    \
            _diagbuffer_printf(h, 6, formatStr, p1, p2, p3, p4, p5, p6)
#define DIAGBUFFER_PRINTF_7(h, formatStr, p1, p2, p3, p4, p5, p6, p7)    \
            _diagbuffer_printf(h, 7, formatStr, p1, p2, p3, p4, p5, p6, p7)
#define DIAGBUFFER_PRINTF_8(h, formatStr, p1, p2, p3, p4, p5, p6, p7, p8)    \
            _diagbuffer_printf(h, 8, formatStr, p1, p2, p3, p4, p5, p6, p7, p8)
#define DIAGBUFFER_PRINTF_9(h, formatStr, p1, p2, p3, p4, p5, p6, p7, p8, p9)    \
            _diagbuffer_printf(h, 9, formatStr, p1, p2, p3, p4, p5, p6, p7, p8, p9)
#define DIAGBUFFER_PRINTF_10(h, formatStr, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)    \
            _diagbuffer_printf(h, 10, formatStr, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)


/**
 * <!-- _diagbuffer_static_printf -->
 *
 * Do not use this API.  Use the DIAGBUFFER_STATIC_PRINTF_n macros.
 *  This API replaces ULogFront_RealTimeStaticPrintf() of ULog.
 */
#define _diagbuffer_static_printf(h, msgSlot, dataCount, formatStr, ... )   \
            ULogFront_RealTimeStaticPrintf(h, msgSlot, dataCount, formatStr, __VA_ARGS__)


/**
 * <!-- DIAGBUFFER_STATIC_PRINTF_N -->
 *
 * @brief Macros to write to Static DiagBuffer.
 *  These replaces ULOG_RT_STATIC_PRINTF_N() of ULog.
 *
 * These macros provide parameter count safety to the _diagbuffer_static_printf
 * function. These should be used instead of calling the function directly.
 *
 * @param                h : A handle indicating where to log.
 * @param          msgSlot : Which slot of the static DiagBuffer to write to.
 * @param        formatStr : The format string for the printf.
 * @param          p1 - pN : The data to log.  These must be (u)int32 in
 *                           length.  Smaller data types will be automatically
 *                           promoted.
 */
#define DIAGBUFFER_STATIC_PRINTF_0(h, msgSlot, formatStr) \
            _diagbuffer_static_printf(h, msgSlot, 10, formatStr,0,0,0,0,0,0,0,0,0,0)
#define DIAGBUFFER_STATIC_PRINTF_1(h, msgSlot, formatStr, p1)    \
            _diagbuffer_static_printf(h, msgSlot, 10, formatStr, p1,0,0,0,0,0,0,0,0,0)
#define DIAGBUFFER_STATIC_PRINTF_2(h, msgSlot, formatStr, p1, p2)    \
            _diagbuffer_static_printf(h, msgSlot, 10, formatStr, p1, p2,0,0,0,0,0,0,0,0)
#define DIAGBUFFER_STATIC_PRINTF_3(h, msgSlot, formatStr, p1, p2, p3)    \
            _diagbuffer_static_printf(h, msgSlot, 10, formatStr, p1, p2, p3,0,0,0,0,0,0,0)
#define DIAGBUFFER_STATIC_PRINTF_4(h, msgSlot, formatStr, p1, p2, p3, p4)    \
            _diagbuffer_static_printf(h, msgSlot, 10, formatStr, p1, p2, p3, p4,0,0,0,0,0,0)
#define DIAGBUFFER_STATIC_PRINTF_5(h, msgSlot, formatStr, p1, p2, p3, p4, p5)    \
            _diagbuffer_static_printf(h, msgSlot, 10, formatStr, p1, p2, p3, p4, p5,0,0,0,0,0)
#define DIAGBUFFER_STATIC_PRINTF_6(h, msgSlot, formatStr, p1, p2, p3, p4, p5, p6)    \
            _diagbuffer_static_printf(h, msgSlot, 10, formatStr, p1, p2, p3, p4, p5, p6,0,0,0,0)
#define DIAGBUFFER_STATIC_PRINTF_7(h, msgSlot, formatStr, p1, p2, p3, p4, p5, p6, p7)    \
            _diagbuffer_static_printf(h, msgSlot, 10, formatStr, p1, p2, p3, p4, p5, p6, p7,0,0,0)
#define DIAGBUFFER_STATIC_PRINTF_8(h, msgSlot, formatStr, p1, p2, p3, p4, p5, p6, p7, p8)    \
            _diagbuffer_static_printf(h, msgSlot, 10, formatStr, p1, p2, p3, p4, p5, p6, p7, p8,0,0)
#define DIAGBUFFER_STATIC_PRINTF_9(h, msgSlot, formatStr, p1, p2, p3, p4, p5, p6, p7, p8, p9)    \
            _diagbuffer_static_printf(h, msgSlot, 10, formatStr, p1, p2, p3, p4, p5, p6, p7, p8, p9,0)
#define DIAGBUFFER_STATIC_PRINTF_10(h, msgSlot, formatStr, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)    \
            _diagbuffer_static_printf(h, msgSlot, 10, formatStr, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

/**
 * <!-- diagbuffer_vprintf -->
 *
 * @brief Log data in the vprintf format.
 *  This API replaces ULogFront_RealTimeVprintf() of ULog.
 *
 * Log printf data to a DiagBuffer log. The printf formatting is not
 * executed until the log is pulled.  This makes a very performant call,
 * but also means all strings must be present when the buffer is read.
 *
 * @note This function serves identical purpose to _diagbuffer_printf,
 *       except it takes a va_list argument instead of being variadic.
 *
 * @param                h : A DiagBuffer handle.
 * @param        dataCount : the number of parameters being printed (Not
 *                           including the formatStr).  Limited to 10
 *                           parameters.
 * @param        formatStr : The format string for the printf.
 * @param               ap : A va_list of the parameters being printed.
 *
 * @return     DAL_SUCCESS : if the message was logged successfully.
 *                           Errors are defined in the DIAGBUFFER_ERRORS enum.
*/
#define diagbuffer_vprintf(h, dataCount, formatStr, ap)   \
            ULogFront_RealTimeVprintf(h, dataCount, formatStr, ap)


/**
 * <!-- _diagbuffer_write_data -->
 *
 * Do not use this API. Use the DIAGBUFFER_WRITE_DATA_n macros.
 *  This API replaces ULogFront_RealTimeData() of ULog.
 */
#define _diagbuffer_write_data(h, dataCount, ...)   \
            ULogFront_RealTimeData(h, dataCount, __VA_ARGS__);


/**
 * <!-- DIAGBUFFER_WRITE_DATA_N -->
 *
 * @brief Macros to provide some parameter count safety to the _diagbuffer_write_data function.
 *  These replaces ULOG_RT_DATA_N() of ULog.
 *
 * These macros provide parameter count safety to the _diagbuffer_write_data function.
 * These should be used instead of calling the function directly.
 *
 * @param                h : A handle indicating where to log.
 * @param          p1 - pN : The data to write. These must be (u)int32 in length.
 *                           Smaller data types will be automatically promoted.
 */
#define DIAGBUFFER_WRITE_DATA_1(h, p1)    \
            _diagbuffer_write_data(h, 1, p1)
#define DIAGBUFFER_WRITE_DATA_2(h, p1, p2)    \
            _diagbuffer_write_data(h, 2, p1, p2)
#define DIAGBUFFER_WRITE_DATA_3(h, p1, p2, p3)    \
            _diagbuffer_write_data(h, 3, p1, p2, p3)
#define DIAGBUFFER_WRITE_DATA_4(h, p1, p2, p3, p4)    \
            _diagbuffer_write_data(h, 4, p1, p2, p3, p4)
#define DIAGBUFFER_WRITE_DATA_5(h, p1, p2, p3, p4, p5)    \
            _diagbuffer_write_data(h, 5, p1, p2, p3, p4, p5)
#define DIAGBUFFER_WRITE_DATA_6(h, p1, p2, p3, p4, p5, p6)    \
            _diagbuffer_write_data(h, 6, p1, p2, p3, p4, p5, p6)
#define DIAGBUFFER_WRITE_DATA_7(h, p1, p2, p3, p4, p5, p6, p7)    \
            _diagbuffer_write_data(h, 7, p1, p2, p3, p4, p5, p6, p7)
#define DIAGBUFFER_WRITE_DATA_8(h, p1, p2, p3, p4, p5, p6, p7, p8)    \
            _diagbuffer_write_data(h, 8, p1, p2, p3, p4, p5, p6, p7, p8)
#define DIAGBUFFER_WRITE_DATA_9(h, p1, p2, p3, p4, p5, p6, p7, p8, p9)    \
            _diagbuffer_write_data(h, 9, p1, p2, p3, p4, p5, p6, p7, p8, p9)
#define DIAGBUFFER_WRITE_DATA_10(h, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)    \
            _diagbuffer_write_data(h, 10, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)


/**
 * <!-- diagbuffer_write_char_array -->
 *
 * @brief Write byte data to a DiagBuffer's buffer.
 *  This API replaces ULogFront_RealTimeCharArray() of ULog.
 *
 * @param               h : A DiagBuffer handle.
 * @param       byteCount : the number of bytes to write.
 * @param        byteData : A pointer to the data to write to the buffer.
 *
 * @return    DAL_SUCCESS : if the message was logged successfully.
 *                          Errors are defined in the DIAGBUFFER_ERRORS enum.
*/
#define diagbuffer_write_char_array(h, byteCount, byteData)   \
            ULogFront_RealTimeCharArray(h, byteCount, byteData)


/**
 * <!-- diagbuffer_write_string -->
 *
 * @brief write string data to a DiagBuffer's buffer.
 *  This API replaces ULogFront_RealTimeString() of ULog.
 *
 * This function copies the full string to the buffer.
 *
 * @param               h : A DiagBuffer handle.
 * @param          string : The string to write to the buffer.
 *
 * @return    DAL_SUCCESS : if the message was logged successfully.
 *                          Errors are defined in the DIAGBUFFER_ERRORS enum.
 */
#define diagbuffer_write_string(h, cStr)   \
            ULogFront_RealTimeString(h, cStr)


/**
 * <!-- diagbuffer_write_word_array -->
 *
 * @brief write word data to a DiagBuffer's buffer.
 *  This API replaces ULogFront_RealTimeWordArray() of ULog.
 *
 * @param                h : A DiagBuffer handle.
 * @param        wordCount : The number of words to write to the buffer.
 * @param         wordData : A pointer to the data to write to the buffer.
 *
 * @return     DAL_SUCCESS : if the message was logged successfully.
 *                           Errors are defined in the DIAGBUFFER_ERRORS enum.
*/
#define diagbuffer_write_word_array(h, wordCount, wordData)   \
            ULogFront_RealTimeWordArray(h, wordCount, wordData)


/**
 * <!-- diagbuffer_write_csv -->
 *
 * @brief write word data to a DiagBuffer. Output will be a CSV compatible format.
 *  This API replaces ULogFront_RealTimeCsv() of ULog.
 *
 * @param                h  : A DiagBuffer handle.
 * @param         wordCount : The number of words to write to the buffer.
 * @param          wordData : A pointer to the data to write to the buffer.
 *
 * @return      DAL_SUCCESS : if the message was logged successfully.
 *                            Errors are defined in the DIAGBUFFER_ERRORS enum.
*/
#define diagbuffer_write_csv(h, wordCount, wordData)   \
            ULogFront_RealTimeCsv(h, wordCount, wordData)


/**
 * <!-- diagbuffer_write_vector -->
 *
 * @brief write vector data to a DiagBuffer.
 *  This API replaces ULogFront_RealTimeVector() of ULog.
 *
 * Output will be a vector with the provided format. No newlines will be inserted
 *
 * @param                h : A DiagBuffer handle.
 * @param        formatStr : A format string with a single %x, %d, %i, or %u
 * @param   entryByteCount : 1, 2 or 4 byte size data
 * @param     vectorLength : How many elements to store.
 * @param           vector : Pointer to the array of elements.
 *
 * @return     DAL_SUCCESS : if the message was logged successfully.
 *                           Errors are defined in the DIAGBUFFER_ERRORS enum.
*/
#define diagbuffer_write_vector(h, formatStr, entryByteCount, vectorLength, vector)   \
            ULogFront_RealTimeVector(h, formatStr, entryByteCount, vectorLength, vector)

/**
 * <!-- diagbuffer_begin_multi_part_msg -->
 *
 * @brief Begin a multi part message.
 *  This API replaces ULogFront_RealTimeMultipartMsgBegin() of ULog.
 *
 * The DiagBuffer will be locked for the duration of the multi part message.  To
 * free the lock, call diagbuffer_end_multi_part_msg(). The first message of a
 * MPM is always a _diagbuffer_printf() with %m specifiers for the parts to
 * add in the subsequent logging before calling diagbuffer_end_multi_part_msg().
 *
 * @param                h : A DiagBuffer handle, which may be a handle to a single
 *                           or a group of DiagBuffers
 *
 * @return            TRUE : if the MPM was successfully started.
 *                   False : if the MPM wasn't successfully started. the multi part
 *                           message should not be continued further
 */
#define diagbuffer_begin_multi_part_msg(h)   \
            ULogFront_RealTimeMultipartMsgBegin(h)


/**
 * <!-- diagbuffer_end_multi_part_msg -->
 *
 * @brief End a multi part message
 *  This API replaces ULogFront_RealTimeMultipartMsgEnd() of ULog.
 *
 * The DiagBuffer will be locked for the duration of the multi part message.
 * This function frees the lock, ends the MPM message and completes the MPM bookkeeping.
 *
 * @param                h : A DiagBuffer handle, which may be a handle to a single or
 *                           a group of logs
 */
#define diagbuffer_end_multi_part_msg(h)   \
            ULogFront_RealTimeMultipartMsgEnd(h)

/**
 * <!-- diagbuffer_set_tx_mode -->
 *
 * @brief Allows the mode of the diagbuffer handle to be altered by clients.
 *  DIAGBUFFER_MODE_STREAM and DIAGBUFFER_MODE_CBUFFER are valid modes that
 *  can be set by the clients.
 *
 * The mode remains default if the user passes invalid mode
 * for the 2nd parameter
 *
 * @param h    : A diagbuffer handle.
 * @param mode : mode to set to the diagbuffer.
 *
 * @return DAL_SUCCESS if the mode was correctly set
 *        Errors are defined in the DIAGBUFFER_ERRORS enum.
 */
diagbuffer_result diagbuffer_set_tx_mode (diagbuffer_handle h, uint8 mode );

/**
 * <!-- diagbuffer_set_threshold -->
 *
 * @brief Allows the thresholds such as high_mark and low_mark of the
 *        diagbuffer handle to be altered by clients. high_mark indicates
 *        the threshold at which the data can be drained and low_mark signifies
 *        the point at which draining can be stopped.
 * 
 *
 * The mode remains default if the user passes invalid mode
 * for the 2nd parameter
 *
 * @param h        : A diagbuffer handle.
 * @param high_mark: high mark in terms of percentage.
 * @param low_mark : low mark in terms of percentage.
 *
 * @return DAL_SUCCESS if the thresholds are correctly set
 *        Errors are defined in the DIAGBUFFER_ERRORS enum.
 */
diagbuffer_result diagbuffer_set_threshold(diagbuffer_handle h, uint16 high_mark, uint16 low_mark );

#ifdef __cplusplus
}
#endif

#endif /* DIAGBUFFER_H */
