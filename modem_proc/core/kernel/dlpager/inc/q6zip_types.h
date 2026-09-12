/**
   @file q6zip_types.h 
   @brief Collection of macros and types for Q6ZIP 
    
   Copyright (c) 2014 by Qualcomm Technologies, Inc.  All Rights Reserved.
   Confidential and Proprietary - Qualcomm Technologies, Inc.
 */
#pragma once
#include "qurt_types.h"

/** @brief Size (in bytes) of a decompressed page */
#define Q6ZIP_DECOMPRESSED_PAGE_SIZE 4096

/** @brief Handy macro to get the number of elements in an array */
#define Q6ZIP_ARRAY_SIZE( array ) ( sizeof( ( array ) ) / sizeof( ( array )[ 0 ] ) )

/** @brief Handy macro to return minimum of two integers */
#define Q6ZIP_MIN( a, b ) ( ( a ) <= ( b ) ? ( a ) : ( b ) )

/** @brief Handy macro to get the offset of a field within a C-struct */
#define Q6ZIP_OFFSETOF( type, field ) ( (qurt_addr_t) &( ((type *) 0)->field ) )

/** @brief Macro to check if the algo is one of the supported ZIP algorithms */
#define Q6ZIP_IS_ZIP_ALGO( algo ) \
    ( ( algo ) == Q6ZIP_ALGO_ZIP_RW )

/** @brief Macro to check if the algo is one of the supported UNZIP
           algorithms */
#define Q6ZIP_IS_UNZIP_ALGO( algo ) \
    ( ( ( algo ) == Q6ZIP_ALGO_UNZIP_RO ) || \
      ( ( algo ) == Q6ZIP_ALGO_UNZIP_RW ) )

/** @brief Hint to compiler that condition is likely */
#define Q6ZIP_LIKELY( exp ) __builtin_expect((exp), 1)

/** @brief Hint to compiler that condition is unlikely */
#define Q6ZIP_UNLIKELY( exp ) __builtin_expect((exp), 0)

/**
 * @brief Various Q6ZIP implementations
 */
typedef enum
{
    Q6ZIP_IMPL_SW,   /**< Q6ZIP SW  */
    Q6ZIP_IMPL_IPA,  /**< Q6ZIP IPA */
    Q6ZIP_IMPL_NONE

} q6zip_impl_t;

/**
 * @brief Various algorithms available
 */
typedef enum
{
    Q6ZIP_ALGO_UNZIP_RO,  /**< RO decompress */
    Q6ZIP_ALGO_ZIP_RW,    /**< RW compress   */
    Q6ZIP_ALGO_UNZIP_RW,  /**< RW decompress */
    Q6ZIP_ALGO_MEMCPY,    /**< HW accelerated memcpy() */
    Q6ZIP_ALGO_MAX

} q6zip_algo_t;

/**
 * @brief Hints for the algorithms
 */
typedef enum
{
    Q6ZIP_ALGO_HINT_NONE,         /**< No hint specified  */
    Q6ZIP_ALGO_HINT_UNZIP_RO_TEXT /**< RO decompress code */

} q6zip_algo_hint_t;

/** @brief Various Q6ZIP errors */
typedef enum
{
    Q6ZIP_ERROR_NONE,                 /**< Success                    */
    Q6ZIP_ERROR_BAD_OPERATION,        /**< Bad operation/algo         */
    Q6ZIP_ERROR_CONTEXT_ALLOC_FAILED, /**< Failed to allocate context */
    Q6ZIP_ERROR_NULL_PTR,
    Q6ZIP_ERROR_GENERIC,              /**< Generic failure            */
    Q6ZIP_ERROR_NOT_IMPLEMENTED       /**< Feature not implemented    */

} q6zip_error_t;

/** @brief Abstract datatype of a buffer where len <= cap */
typedef struct
{
    /** @brief Pointer to buffer */
    void *       ptr;

    /** @brief Capacity of buffer */
    unsigned int cap;

    /** @brief Length (in use) of buffer */
    unsigned int len;

} q6zip_buffer_t;

/** @brief Type for an I/O vector - something that has address and length */
typedef struct
{
    /** @brief Address associated with this I/O vector */
    void * ptr;

    /** @brief Length (bytes) of this I/O vector */
    unsigned int len;

    #if defined( DLPAGER_PROVIDES_PHYS_ADDR_TO_Q6ZIP )
    /** @brief *Physical* address associated with this I/O vector */
    void * phys_ptr;
    #endif

} q6zip_iovec_t;

/** @brief Priority associated with a Q6ZIP request. Lower value means higher
           priority */
typedef unsigned short q6zip_priority_t;

/** @brief Constants for fixed priorites */
typedef enum
{
    Q6ZIP_FIXED_PRIORITY_IMPL = 0,       /**< Let implementation prioritize */
    Q6ZIP_FIXED_PRIORITY_HIGH = 1,       /**< Force high priority */
    Q6ZIP_FIXED_PRIORITY_LOW  = 0xFFFF   /**< Force low priority  */

} q6zip_fixed_priority_t;

/** @brief Q6ZIP response */
typedef struct
{
    /** @brief Error in response? */
    q6zip_error_t error;

    /** @brief Actual length (bytes) of destination buffer provided in the
               request. */
    unsigned int len;

} q6zip_response_t;

/** @brief Q6ZIP user's context. Similar to qurt_sysevent_pagefault_t
    @todo anandj This can be changed to be a void * if DL Pager will provide
          memory and manage its lifecycle. For now, CRD folks suggested that
          we keep it as it is */
typedef struct
{
    /** @brief ID of thread requesting Q6ZIP operation */
    unsigned int thread_id;

    /** @brief Faulting address */
    unsigned int fault_addr;

    /** @brief Opaque pointer to other stuff owned by user */
    void * other;

} q6zip_user_context_t;

typedef struct
{
    unsigned long long start;
    unsigned long long stop;

} q6zip_time_pair_t;
