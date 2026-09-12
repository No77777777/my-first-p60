#ifndef SECAPI_KDF_H
#define SECAPI_KDF_H

/** 
  @brief General Cryptographic KDF Algorithm APIs.
  Defines APIs to use KDF (Key Derivation Function) algorithms.
 * 
 */

/*===========================================================================
Copyright (c) 2020 Qualcomm Technologies, Inc.
All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.
===========================================================================*/

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "secapi_engine.h"

/** @addtogroup crypto_secalgoapi 
  @{ */

/**
  Opaque KDF context pointer handle
*/
typedef struct secapi_kdf_ctx_s * SECAPI_KDF_HANDLE;

/**
  KDF algorithms
*/
typedef enum
{
  SECAPI_HKDF,                  /**< HKDF */

  SECAPI_KDF_MAX_ALG_COUNT      /**< Maximum number of KDF algorithm types */
} SECAPI_KDF_ALG;

/**
  HKDF modes
*/
typedef enum
{
  SECAPI_HKDF_MODE_EXTRACT_AND_EXPAND,
  SECAPI_HKDF_MODE_EXTRACT,
  SECAPI_HKDF_MODE_EXPAND,

  SECAPI_HKDF_MODE_MAX,
} SECAPI_HKDF_MODE;

/**
  KDF paramater types
*/
typedef enum
{
  // common
  SECAPI_KDF_SET_HASH_ALG,          /**< Set the Hash algorithm (defined in SECAPI_HASH_ALG) */
  SECAPI_KDF_SET_OUTPUT_KEY_LEN,    /**< Set the length of the output key */
  SECAPI_KDF_GET_KEY,               /**< Get the key */
  SECAPI_KDF_SET_CONTEXT,           /**< Set the value of a label/context. Accepts a buffer and size as input */
  SECAPI_KDF_SET_INPUT_KEY,         /**< Set the input key. Accepts a buffer and size as input */
  SECAPI_KDF_SET_SALT,              /**< Set the salt. Accepts a buffer and size as input */

  // for HKDF
  SECAPI_HKDF_SET_MODE,             /**< Accepts an integer argument representing the HKDF mode of operation (SECAPI_HKDF_MODE) */

  SECAPI_KDF_PARAM_MAX              /**< Maximum number of KDF control parameters */

} SECAPI_KDF_PARAM_TYPE;


/**
 * @brief
 * Allocates memory for a context for KDF operations and returns a
 * handle to the caller.
 * It is the caller's responsiblity to call secapi_kdf_free() to
 * properly clean up KDF context and release any associated resources.
 *
 * @param
 * None
 *
 * @return
 * Non zero value if successful.
 */
SECAPI_KDF_HANDLE secapi_kdf_new(void);


/**
 * @brief
 * Clears the KDF context and releases internal resources.
 *
 * @param h [in] handle to the KDF context.
 *
 * @return
 * 0 if successful, negative value otherwise.
 */
int secapi_kdf_free(SECAPI_KDF_HANDLE h);


/**
 * @brief
 * Initializes a previously-allocated handle for KDF operations, based
 * on the alogrithm and engine chosen.
 *
 * @param h      [in] Handle to the KDF context
 * @param algo   [in] KDF algorithm type enumerated in SECAPI_KDF_ALG
 * @param engine [in] Engine type enumerated in SECAPI_ENGINE_TYPE
 *
 * @return
 * 0 if successful, negative value otherwise.
 */
int secapi_kdf_init
(
  SECAPI_KDF_HANDLE   h,
  SECAPI_KDF_ALG      algo,
  SECAPI_ENGINE_TYPE  engine
);

/**
 * @brief
 * Resets KDF context and clears internal states in order to re-use the
 * KDF context for another KDF operation. One can use the interface to
 * re-use the allocated KDF context for different KDF algorithm.
 *
 * @param h [in] handle to the KDF context
 *
 * @return
 * 0 if successful. negative value otherwise.
 */
int secapi_kdf_reset(SECAPI_KDF_HANDLE h);


/**
 * @brief
 * Sets or gets control or context parameters, related to the operation
 * of the chosen algorithm. When the paramter name contains _GET_, this
 * typically signifies that the caller is requesting for the value of a
 * parameter to be returned to them. Otherwise the parameter signifies
 * the setting of a control variable.
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
 * @return
 * 0 if successful. negative value otherwise.
 */
int secapi_kdf_ctrl
(
  SECAPI_KDF_HANDLE       h,
  SECAPI_KDF_PARAM_TYPE   type,
  int                     val,
  uint8_t                 *buf,
  size_t                  isz,
  size_t                  *osz
);


/**
 * @brief
 * Derives a Key function. The generated key will stay in the context.
 * 
 * @param  h [in]  Handle to the KDF context
 * 
 * @return
 * 0 if successful. negative value otherwise.
 */
int secapi_kdf(SECAPI_KDF_HANDLE h);

/** @} */  /** end_addtogroup crypto_secalgoapi */

#endif
