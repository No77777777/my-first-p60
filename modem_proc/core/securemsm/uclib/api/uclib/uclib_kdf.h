#ifndef _UCLIB_KDF
#define _UCLIB_KDF

/* ====================================================================
 * Copyright (c) 2017 - 2020 Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary – Qualcomm Technologies, Inc.
 * ====================================================================
 */

/*===========================================================================

Unified Crypto Libraray - KDF public APIs

GENERAL DESCRIPTION
    This is the public APIs used for key derivation function

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS

===========================================================================*/

/*===========================================================================
                     INCLUDE FILES FOR MODULE
===========================================================================*/
#include <stdint.h>
#include <stdlib.h>
#include "uclib_engine.h" /* For UCLIB_ENGINE_TYPE */
#include "uclib_env.h"    /* For UCLIB Environment type */

/*===========================================================================
                 DEFINITIONS AND TYPE DECLARATIONS
 ===========================================================================*/

/* Opaque KDF context pointer handle */
typedef struct kdf_ctx_s * UCLIB_KDF_HANDLE;

/* KDF algorithms */
typedef enum
{
    UCLIB_KDF_ALG_INVALID = 0,

    UCLIB_KBKDF_COUNTER_MODE,          // SP 800-108 KBKDF
    UCLIB_KBKDF_FEEDBACK_MODE,         // SP 800-108 KBKDF
    UCLIB_KBKDF_PIPELINE_MODE,         // SP 800-108 KBKDF
    UCLIB_PBKDF2,                      // SP 800-132 PBKDF
    UCLIB_HKDF,                        // HMAC KDF (Extract and Expand)

    UCLIB_KDF_MAX_ALG_COUNT
} UCLIB_KDF_ALG;

/* KBKDF R length */
typedef enum
{
    UCLIB_KBKDF_R_LEN_INVALID = 0,

    UCLIB_KBKDF_R_LEN_8_BITS,
    UCLIB_KBKDF_R_LEN_16_BITS,
    UCLIB_KBKDF_R_LEN_24_BITS,
    UCLIB_KBKDF_R_LEN_32_BITS,

    UCLIB_KBKDF_MAX_R_LEN_COUNT
} UCLIB_KBKDF_R_LEN;

typedef enum
{
  UCLIB_HKDF_MODE_EXTRACT_AND_EXPAND,
  UCLIB_HKDF_MODE_EXTRACT,
  UCLIB_HKDF_MODE_EXPAND,

  UCLIB_HKDF_MODE_MAX,
} UCLIB_HKDF_MODE;

/* KBKDF counter location */
typedef enum
{
  UCLIB_KBKDF_CTR_LOC_INVALID  = 0,

  UCLIB_KBKDF_CTR_LOC_BEFORE_ITERATION,   // for feedback and pipeline mode case
  UCLIB_KBKDF_CTR_LOC_AFTER_ITERATION,    // for feedback and pipeline mode case
  UCLIB_KBKDF_CTR_LOC_BEFORE_FIXED_DATA,  // for counter mode, it is same with
  UCLIB_KBKDF_CTR_LOC_AFTER_FIXED_DATA,   // for counter, feedback and pipeline modes

  UCLIB_KBKDF_MAX_CTR_LOC_COUNT
} UCLIB_KBKDF_CTR_LOC;

/* KDF paramater types */
typedef enum
{
  UCLIB_KDF_PARAM_INVALID  = 0,

  // common
  UCLIB_KDF_SET_MAC_ALG,            // defined in UCLIB_MAC_ALG
  UCLIB_KDF_SET_HASH_ALG,           // defined in UCLIB_HASH_ALG,    used in HMAC case.
  UCLIB_KDF_SET_OUTPUT_KEY_LEN,     // defined in UCLIB_HASH_ALG,    used in HMAC case.
  UCLIB_KDF_SET_HW_KEY_TYPE,        // defined in UCLIB_KDF_HW_KEY_TYPE - integer argument
  UCLIB_KDF_GET_KEY,
  UCLIB_KDF_SET_CONTEXT,            // Set the value of a label/context. Accepts a buffer and size as input
  UCLIB_KDF_SET_INPUT_KEY,          // Set the input key. Accepts a buffer and size as input
  UCLIB_KDF_SET_SALT,               // Set the salt. Accepts a buffer and size as input

  // common for KBKDF
  UCLIB_KDF_SET_KBKDF_CIPHER_ALG,   // defined in UCLIB_CIPHER_ALG,  used in CMAC case.
  UCLIB_KDF_SET_KBKDF_R_LEN,        // defined in UCLIB_KBKDF_R_LEN
  UCLIB_KDF_SET_KBKDF_CTR_LOC,      // defined in UCLIB_KBKDF_CTR_LOC
  UCLIB_KDF_SET_KBKDF_KEY,          // byte array, length is defined in cipher.h, should be in uclib_cipher.h
  UCLIB_KDF_SET_KBKDF_LABEL,        // byte array, any length
  UCLIB_KDF_SET_KBKDF_CONTEXT,      // byte array, any length

  //HWKM wrapped key for counter mode only
  UCLIB_KDF_SET_KBKDF_WRAPPED_KEY,  // Wrapped key For CMAC operation

  // for KBKDF feedback mode only
  UCLIB_KDF_SET_KBKDF_IV,           // byte array

  // for PBKDF
  UCLIB_KDF_SET_PBKDF_PASSWORD,     // byte array, any length
  UCLIB_KDF_SET_PBKDF_SALT,         // byte array, any length
  UCLIB_KDF_SET_PBKDF_ITERATION,

  // for HBKDF
  UCLIB_HKDF_SET_MODE,              // Accepts an integer argument representing the HKDF mode of operation (UCLIB_HKDF_MODE)

  UCLIB_KDF_PARAM_MAX

} UCLIB_KDF_PARAM_TYPE;

/* KDF HW key type */
typedef enum
{
  UCLIB_KDF_KEY_INVALID = 0,

  UCLIB_KDF_SHK,                        // OEM HW Key
  UCLIB_KDF_PHK,                        // QC HW Key

  UCLIB_KDF_MAX_HW_KEYS
} UCLIB_KDF_HW_KEY_TYPE;


/**
 * @brief Allocates memory for a context for KDF operations and returns a
 *        handle to the caller.
 *        It is the caller's responsiblity to call uclib_kdf_free() to
 *        properly clean up KDF context and release any associated resources.
 *
 * @param env    [in] Pointer to environment structure
 *
 * @return       Non zero value if successful.
 */
UCLIB_KDF_HANDLE uclib_kdf_new( env_t *env );


/**
 * @brief Clears the KDF context and releases internal resources.
 *
 * @param h      [in] handle to the KDF context.
 *
 * @return       0 if successful, negative value otherwise.
 */
int uclib_kdf_free( UCLIB_KDF_HANDLE h );


/**
 * @brief Initializes a previously-allocated handle for KDF operations, based
 *        on the alogrithm and engine chosen.
 *
 * @param h      [in] Handle to the KDF context
 * @param algo   [in] KDF algorithm type enumerated in UCLIB_KDF_ALG
 * @param engine [in] Engine type enumerated in UCLIB_ENGINE_TYPE
 *
 * @return       0 if successful, negative value otherwise.
 */
int uclib_kdf_init( UCLIB_KDF_HANDLE   h,
                    UCLIB_KDF_ALG      algo,
                    UCLIB_ENGINE_TYPE  engine );

/**
 * @brief Resets KDF context and clears internal states in order to re-use the
 *        KDF context for another KDF operation. One can use the interface to
 *        re-use the allocated KDF context for different KDF algorithm.
 *
 * @param h       [in] handle to the KDF context
 *
 * @return 0 if successful. negative value otherwise.
 */
int uclib_kdf_reset( UCLIB_KDF_HANDLE  h );


/**
 * @brief Sets or gets control or context parameters, related to the operation
 *        of the chosen algorithm. When the paramter name contains _GET_, this
 *        typically signifies that the caller is requesting for the value of a
 *        parameter to be returned to them. Otherwise the parameter signifies
 *        the setting of a control variable.
 *
 * @param h     [in]      handle to the KDF context
 * @param type  [in]      KDF param type (parameter name indicates get/set)
 * @param val   [in]      Integer input parameter
 * @param buf   [in, out] Pointer to a buffer-parameter
 *                        which can be used to pass data to or
 *                        retrieve data from the context
 * @param isz   [in]      Size-parameter, used to indicate the
 *                        length of the buffer parameter
 * @param osz   [out]     Size-parameter, used to indicate the
 *                        length of the buffer being returned
 *
 * @return      0 if successful. negative value otherwise.
 */
int uclib_kdf_ctrl( UCLIB_KDF_HANDLE       h,
                    UCLIB_KDF_PARAM_TYPE   type,
                    int                    val,
                    uint8_t               *buf,
                    size_t                 isz,
                    size_t                *osz );


/**
  @brief   Derives a Key function. The generated key will stay in the context.

  @param  h              [in]      Handle to the KDF context

  @return 0 if successful. negative value otherwise.

*/
int uclib_kdf ( UCLIB_KDF_HANDLE   h );

#endif /* _UCLIB_KDF */
