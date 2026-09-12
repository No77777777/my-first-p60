#ifndef SECAPI_CIPHER_H
#define SECAPI_CIPHER_H

/** 
  @brief General Cryptographic Cipher Algorithm APIs.
  Defines APIs to use Cipher algorithms.
 * 
 */

/*===========================================================================
Copyright (c) 2020, 2022 Qualcomm Technologies, Inc.
All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.
===========================================================================*/

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "secapi_engine.h"

/** @addtogroup crypto_secalgoapi 
  @{ */

/**
 Opaque cipher context pointer
*/
typedef struct secapi_cipher_ctx_s * SECAPI_CIPHER_HANDLE;

/* Encrypt arg. definitions for secapi_cipher_init */
#define SECAPI_ENCRYPT          true
#define SECAPI_DECRYPT          false

/**
 Cipher algorithms
*/
typedef enum {
  SECAPI_CIPHER_AES128,
  SECAPI_CIPHER_AES192,
  SECAPI_CIPHER_AES256,

  /* Stream chiper, as described in rfc7539 */
  SECAPI_CIPHER_CHACHA20,

  SECAPI_CIPHER_MAX_ALG_COUNT
}SECAPI_CIPHER_ALG;

/**
 Cipher Modes
*/
typedef enum {
  /* Confidentiality */
  SECAPI_CTR,
  
  /* Confidentiality  and integrity */
  SECAPI_CCM,
  SECAPI_GCM,

  /* Authenticated encryption as described in rfc7539 */
  SECAPI_CHACHA20_POLY1305,

  /* Used for algorithms that do not require/accept a mode */
  SECAPI_CIPHER_MODE_DEFAULT,

  SECAPI_CIPHER_MAX_MODE_COUNT
}SECAPI_CIPHER_MODE;

/* Cipher Context Parameter Types */
typedef enum {
  SECAPI_CIPHER_SET_DIR,             /** Possible Values: SECAPI_ENCRYPT, SECAPI_DECRYPT                                        */
  SECAPI_CIPHER_SET_KEY,             /** Data buffer and size                                                                   */
  SECAPI_CIPHER_SET_IV,              /** Data buffer and size                                                                   */
  SECAPI_CIPHER_SET_NONCE,           /** Data buffer and size                                                                   */

  SECAPI_CIPHER_SET_XCM_PAYLOAD_LEN, /** CCM/GCM Params: Possible Values: 0 to MAX_UINT32                                       */
  SECAPI_CIPHER_SET_XCM_TAG_LEN,     /** CCM/GCM Params: Possible Values: 0 to MAX_UINT32                                       */
  SECAPI_CIPHER_SET_XCM_HDR_LEN,     /** CCM/GCM Params: Possible Values: 0 to MAX_UINT32                                       */
  SECAPI_CIPHER_SET_XCM_TAG,         /** CCM/GCM Params: Data buffer and size                                                   */
  SECAPI_CIPHER_GET_XCM_TAG,         /** CCM/GCM Params: Data buffer and size                                                   */
  SECAPI_CIPHER_SET_COUNTER_SZ,      /** Possible Values: 32 to 128 bits                                                        */

  SECAPI_CIPHER_MAX_PARAM_TYPE_COUNT
}SECAPI_CIPHER_PARAM_TYPE;


/**
 * @brief Creates a context for cipher operation and returns a
 *        handle to the caller. It is the caller responsiblity
 *        to call secapi_cipher_free() to properly clean up
 *        cipher context and release the resources.
 *
 * @param env    [in] Pointer to environment structure
 *
 * @return Non zero value if successful.
 */
SECAPI_CIPHER_HANDLE secapi_cipher_new(void);

/**
 * @brief Clears cipher context and releases internal resources.
 *
 * @param h      [in] handle to the cipher context
 *
 * @return SECAPI_SUCCESS if successful. SECAPI Error Code
 *         otherwise.
 */
int secapi_cipher_free(SECAPI_CIPHER_HANDLE h);

/**
 * @brief Resets internal states in order to use the handle for
 *        fresh cipher operation. The interface retains the
 *        cipher algorithm, mode, engine, direction, and key
 *        that user has already initialized for the same handle
 *        using Cipher Init and Ctrl interfaces. IV/Nonce has to
 *        be re-initialized.
 *
 * @param h      [in] handle to the cipher context
 *
 * @return SECAPI_SUCCESS if successful. SECAPI Error Code
 *         otherwise.
 */
int secapi_cipher_reset(SECAPI_CIPHER_HANDLE h);

/**
 * @brief Writes/reads cipher context parameters. Parameters can
 *        be passed as value using [val] or as an array of bytes
 *        (8-bit) using [ibuf, isz] arguments. If client does
 *        not know the required buffer size for a type, it can
 *        call the API with ibuf as NULL and provide valid
 *        pointer to osz to retrieve required buffer size for
 *        associated parameter. Refer to SECAPI_CIPHER_PARAM_TYPE
 *        definition for more details of each parameter type. To
 *        read any cipher parameter, [ibuf, isz] are required.
 *
 * @param h      [in] handle to the cipher context
 * @param type   [in] Cipher context parameter type
 * @param val    [in] Cipher context parameter can be passed by
 *               value using this parameter.
 * @param ibuf   [in, out] pointer to buffer in order to
 *               write/read to/from cipher context
 * @param isz    [in] ibuf size in bytes
 * @param osz    [out] pointer to required size on return from
 *               API
 *
 * @return SECAPI_SUCCESS if successful. SECAPI Error Code
 *         otherwise.
 */
int secapi_cipher_ctrl(SECAPI_CIPHER_HANDLE h,
                      SECAPI_CIPHER_PARAM_TYPE type,
                      int val,
                      uint8_t *ibuf,
                      size_t isz,
                      size_t *osz);

/**
 * @brief Initializes cipher context based on requested cipher
 *        algorithm, cipher mode and acceleration engine. If
 *        engine parameter is passed as zero, the default engine
 *        will be used. The default engine is defined in SECAPI
 *        configuration file. User can set key and iv parameters
 *        later using secapi_cipher_ctx_ctrl API and passes NULL
 *        as either of key and iv parameters. Key and IV buffers
 *        size are implicitly specified by the cipher mode
 *        passed in the api. Encrypt parameter controls cipher
 *        direction in the context (Encryption vs. Decryption)
 *
 * @param h       [in] handle to the cipher context
 * @param alg     [in] cipher alg enumerated in SECAPI_CIPHER_ALG
 * @param mode    [in] cipher mode enumerated in
 *                SECAPI_CIPHER_MODE
 * @param engine  [in] engine type enumerated in
 *                SECAPI_ENGINE_TYPE
 * @param encrypt [in] direction (true: Encrypt, false:
 *                 Decrypt)
 *
 * @return SECAPI_SUCCESS if successful. SECAPI Error Code
 *         otherwise.
 */
int secapi_cipher_init(SECAPI_CIPHER_HANDLE h,
                      SECAPI_CIPHER_ALG alg,
                      SECAPI_CIPHER_MODE mode,
                      SECAPI_ENGINE_TYPE engine,
                      bool  encrypt);

/**
 * @brief  Encrypts/Decrypts input buffer [ibuf, isz], saves the
 *         cipher/plain text to output buffer [obuf, osz] and
 *         saves cipher context for further updates. The API
 *         will update output size on successful return in
 *         vld_osz pointer if provided by caller.
 *
 * @param h        [in]  handle to the cipher context
 * @param ibuf     [in]  pointer to input buffer
 * @param isz      [in]  input buffer size
 * @param obuf     [in]  pointer to output buffer
 * @param osz      [in]  output buffer size
 * @param vld_osz  [out] Size of data filled into output buffer
 *
 * @return SECAPI_SUCCESS if successful. SECAPI Error Code
 *         otherwise.
 */
int secapi_cipher_update(SECAPI_CIPHER_HANDLE h,
                        const uint8_t *ibuf, size_t isz,
                        uint8_t *obuf, size_t osz,
                        size_t *vld_osz);

/**
 * @brief  Encrypt/Decrypt the last segment of input buffer
 *         saved in cipher context and saves the cipher/plain
 *         text to output buffer [obuf, osz]. The API will
 *         update output size on successful return in vld_osz
 *         pointer if provided by caller.
 *
 * @param h        [in] handle to the cipher context
 * @param obuf     [in] pointer to output buffer
 * @param osz      [in] pointer output buffer size
 * @param vld_osz  [out] Size of data filled into output buffer
 *
 * @return SECAPI_SUCCESS if successful. SECAPI Error Code
 *         otherwise.
 */
int secapi_cipher_final(SECAPI_CIPHER_HANDLE h,
                       uint8_t *obuf, size_t osz,
                       size_t *vld_osz);

/**
 * @brief  Updates additional authentication data when
 *         authenticated cipher mode (e.g CCM/GCM) is selected.
 *         It returns failure for other cipher modes.
 *
 * @param h        [in]  handle to the cipher context
 * @param aad      [in]  pointer to AAD buffer
 * @param aad_sz   [in]  AAD buffer size
 *
 * @return SECAPI_SUCCESS if successful. SECAPI Error Code
 *         otherwise.
 */
int secapi_cipher_update_aad(SECAPI_CIPHER_HANDLE h,
                            const uint8_t *aad, size_t aad_sz);

/**
 * @brief  Encrypts/Decrypts input buffer [ibuf, isz], saves the
 *         cipher/plain text to output buffer [obuf, osz],
 *         calculates hash of cipher/plain text and saves
 *         cipher/hash context for further updates. The API will
 *         update output size on successful return in vld_osz
 *         pointer if provided by caller.
 *
 * @param h        [in]  handle to the cipher context
 * @param ibuf     [in]  pointer to input buffer
 * @param isz      [in]  input buffer size
 * @param obuf     [in]  pointer to output buffer
 * @param osz      [in]  output buffer size
 * @param vld_osz  [out] pointer to output buffer size after
 *                 returning from API (optional)
 *
 * @return SECAPI_SUCCESS if successful. SECAPI Error Code
 *         otherwise.
 */
int secapi_hash_cipher_update(SECAPI_CIPHER_HANDLE h,
                             const uint8_t *ibuf, size_t isz,
                             uint8_t *obuf, size_t osz,
                             size_t *vld_osz);

/**
 * @brief  Encrypt/Decrypt the last segment of input buffer
 *         saved in cipher context, calculates hash of
 *         cipher/plain text in hash context and saves the
 *         cipher/plain text to output buffer [obuf, osz]. The
 *         API will update output size on successful return in
 *         vld_osz pointer if provided by caller. The hash
 *         result is stored in digest buffer [dgst, dgst_sz]
 *         provided by caller.
 *
 * @param h        [in] handle to the cipher context
 * @param dgst     [in] pointer to digest buffer
 * @param dgst_sz  [in] digest buffer size
 * @param obuf     [in] pointer to output buffer
 * @param osz      [in] output buffer size
 * @param vld_osz  [out] pointer to output buffer size after
 *                 returning from API (optional)
 *
 * @return SECAPI_SUCCESS if successful. SECAPI Error Code
 *         otherwise.
 */
int secapi_hash_cipher_final(SECAPI_CIPHER_HANDLE h,
                            uint8_t *dgst, size_t dgst_sz,
                            uint8_t *obuf, size_t osz,
                            size_t *vld_osz);

/** @} */  /** end_addtogroup crypto_secalgoapi */

#endif
