#ifndef SECAPI_PKEY_H
#define SECAPI_PKEY_H

/** 
  @brief General Cryptographic Public-key Algorithm APIs.
  Defines APIs to use publc-key algorithms.
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
  Opaque PKEY context pointer handle
*/
typedef struct secapi_pkey_ctx_s * SECAPI_PKEY_HANDLE;

/**
 * PKEY algorithms
 */
typedef enum {
  SECAPI_PKEY_CURVE25519,

  SECAPI_PKEY_MAX_ALG_COUNT
} SECAPI_PKEY_ALG;

/**
 * PKEY paramater types
 */
typedef enum {

  /* Accepts a byte-array and length to set the public key/exponent */
  SECAPI_PKEY_PARAM_SET_PUBKEY,

  /* Accepts a byte-array and length to set the private exponent */
  SECAPI_PKEY_PARAM_SET_PRIVKEY,

  /* Accepts a byte-array and length to set the peer public key */
  SECAPI_PKEY_PARAM_SET_PEERKEY,

  /* Accepts a byte-array and length to return the public exponent/key */
  SECAPI_PKEY_PARAM_GET_PUBKEY,

  /* Accepts a byte-array and length to return the private exponent/key */
  SECAPI_PKEY_PARAM_GET_PRIVKEY,

  SECAPI_PKEY_PARAM_MAX

} SECAPI_PKEY_PARAM_TYPE;


/**
 * @brief Allocates memory for a context for pkey operations and returns a
 *        handle to the caller.
 *        It is the caller's responsiblity to call secapi_pkey_free() to
 *        properly clean up pkey context and release any associated resources.
 *
 * @param env    [in] Pointer to environment structure
 *
 * @return       Non zero value if successful.
 */
SECAPI_PKEY_HANDLE secapi_pkey_new(void);

/**
 * @brief Clears the pkey context and releases internal resources.
 *
 * @param h      [in] handle to the pkey context.
 *
 * @return       0 if successful, negative value otherwise.
 */
int secapi_pkey_free(SECAPI_PKEY_HANDLE h);

/**
 * @brief Initializes a previously-allocated handle for pkey operations, based
 *        on the alogrithm and engine chosen.
 *
 * @param h      [in] handle to the pkey context
 * @param algo   [in] pkey algorithm type enumerated in SECAPI_PKEY_ALG
 * @param engine [in] engine type enumerated in SECAPI_ENGINE_TYPE. If the
 *               engine parameter is passed as 0, the default engine will be
 *               used.
 *
 * @return       0 if successful, negative value otherwise.
 */
int secapi_pkey_init(SECAPI_PKEY_HANDLE h,
                    SECAPI_PKEY_ALG algo,
                    SECAPI_ENGINE_TYPE engine);

/**
 * @brief Resets pkey context and clears internal states in order to re-use the
 *        pkey context for another pkey operation. One can use the interface to
 *        re-use the allocated pkey context for different pkey algorithm.
 *
 * @param h      [in] handle to the pkey context
 *
 * @return 0 if successful. negative value otherwise.
 */
int secapi_pkey_reset(SECAPI_PKEY_HANDLE h);

/**
 * @brief Sets or gets control or context parameters, related to the operation
 *        of the chosen algorithm. When the paramter name contains _GET_, this
 *        typically signifies that the caller is requesting for the value of a
 *        parameter to be returned to them. Otherwise the parameter signifies
 *        the setting of a control variable.
 *
 * @param h     [in]      handle to the pkey context
 * @param type  [in]      pkey param type (parameter name indicates get/set)
 * @param val   [in]      Integer input parameter
 * @param buf   [in, out] pointer to a buffer-parameter which
 *                        can be used to pass data to or
 *                        retrieve data from the context
 * @param isz   [in]      Size-parameter, used to indicate the
 *                        length of the buffer parameter
 * @param osz   [out]     Size-parameter, used to indicate the
 *                        length of the buffer being returned
 *
 * @return      0 if successful. negative value otherwise.
 */
int secapi_pkey_ctrl(SECAPI_PKEY_HANDLE h,
                    SECAPI_PKEY_PARAM_TYPE type,
                    int val,
                    uint8_t *buf,
                    size_t isz,
                    size_t *osz);

/**
 * @brief Generates a key.
 *        Parameters can be set up using the secapi_pkey_ctrl() API.
 *
 * @param h      [in]  handle to the pkey context
 *
 * @return 0 if successful. negative value otherwise.
 */
int secapi_pkey_keygen(SECAPI_PKEY_HANDLE h);

/**
 * @brief  Signs the message digest using the private key and returns the
 *         signature.
 *         Parameters can be set up using the secapi_pkey_ctrl() API.
 *
 * @param h           [in]  handle to the pkey context
 * @param md          [in]  pointer to message digest
 * @param md_len      [in]  message digest length
 * @param sig         [out] pointer to signature output buffer
 * @param sig_buf_len [in]  output buffer length
 * @param out_len     [out] length of data filled into output buffer
 *
 * @return 0 if successful. negative value otherwise.
 */
int secapi_pkey_sign(SECAPI_PKEY_HANDLE h,
                    const uint8_t *md,
                    size_t md_len,
                    uint8_t *sig,
                    size_t sig_buf_len,
                    size_t *out_len);

/**
 * @brief  Verify the message digest and signature by using public key.
 *         Parameters can be set up using the secapi_pkey_ctrl() API.
 *
 * @param h        [in]  handle to the pkey context
 * @param md       [in]  pointer to message digest
 * @param md_len   [in]  message digest length
 * @param sig      [in]  pointer to signature
 * @param sig_len  [in]  length of signature.
 *
 * @return 0 if successful. negative value otherwise.
 */
int secapi_pkey_verify(SECAPI_PKEY_HANDLE h,
                      const uint8_t *md,
                      size_t md_len,
                      const uint8_t *sig,
                      size_t sig_len);

/**
 * @brief  Encrypt the input message by using public key.
 *         Parameters can be set up using the secapi_pkey_ctrl() API.
 *         The value in out_len indicates the length of the data filled into
 *         the out buffer.
 *
 * @param h           [in]  handle to the pkey context
 * @param in          [in]  pointer to message
 * @param in_len      [in]  message length
 * @param out         [out] pointer to cipher output buffer
 * @param out_buf_len [in]  output buffer length
 * @param out_len     [out] length of data filled into output buffer
 *
 * @return 0 if successful. negative value otherwise.
 */
int secapi_pkey_encrypt(SECAPI_PKEY_HANDLE h,
                       const uint8_t *in,
                       size_t in_len,
                       uint8_t *out,
                       size_t out_buf_len,
                       size_t *out_len);

/**
 * @brief  Decrypt the input cipher message by using private key.
 *         Parameters can be set up using the secapi_pkey_ctrl() API.
 *         The value in out_len indicates the length of the data filled into
 *         the out buffer.
 *
 * @param h           [in]  handle to the pkey context
 * @param in          [in]  pointer to cipher message
 * @param in_len      [in]  cipher message length
 * @param out         [out] pointer to message output buffer
 * @param out_buf_len [in]  output buffer length
 * @param out_len     [out] length of data filled into output buffer
 *
 * @return 0 if successful. negative value otherwise.
 */
int secapi_pkey_decrypt(SECAPI_PKEY_HANDLE h,
                       const uint8_t *in,
                       size_t in_len,
                       uint8_t *out,
                       size_t out_buf_len,
                       size_t *out_len);

/**
 * @brief  Derive the shared secret/key
 *
 * @param h           [in]  handle to the pkey context
 * @param out         [out] pointer to output buffer
 * @param out_buf_len [in]  output buffer length
 * @param out_len     [out] length of data filled into output buffer
 *
 * @return 0 if successful. negative value otherwise.
 */
int secapi_pkey_derive(SECAPI_PKEY_HANDLE h,
                      uint8_t *out,
                      size_t out_buf_len,
                      size_t *out_len);

/** @} */  /** end_addtogroup crypto_secalgoapi */

#endif
