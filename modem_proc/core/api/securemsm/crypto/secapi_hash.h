#ifndef SECAPI_HASH_H
#define SECAPI_HASH_H

/** 
  @brief General Cryptographic Hash Algorithm APIs.
  Defines APIs to use Hash/digest algorithms.
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
#include "secapi_engine.h"

/** @addtogroup crypto_secalgoapi 
  @{ */

/* Opaque hash context pointer */
typedef struct secapi_hash_ctx_s * SECAPI_MD_HANDLE;

#define SECAPI_BUF_CACHED        0
#define SECAPI_BUF_UNCACHED      1

#define SECAPI_BAM_BLOCKING      0
#define SECAPI_BAM_NON_BLOCKING  1

/* Possible types for SECAPI_HASH_ORDER parameter in cipher context */
#define SECAPI_HASH_BEFORE_ENCR  0
#define SECAPI_HASH_AFTER_ENCR   1

/* Hash algorithms */
typedef enum {
  SECAPI_SHA1,

  SECAPI_SHA2_224,
  SECAPI_SHA2_256,
  SECAPI_SHA2_384,
  SECAPI_SHA2_512,

  SECAPI_HASH_MAX_ALG_COUNT
}SECAPI_HASH_ALG;

typedef enum {
  SECAPI_HMAC_SHK = 0,                 /** Secondary HW Key - OEM key */
  SECAPI_HMAC_PHK,                     /** Primary HW Key - QC key    */

  SECAPI_HMAC_MAX_HW_KEYS
}SECAPI_HMAC_HW_KEY_TYPE;

/* Hash Context Parameter Types */
typedef enum {
  SECAPI_HASH_SET_MSG_MEM_TYPE = 0,    /** Possible Values: SECAPI_BUF_CACHED, SECAPI_BUF_UNCACHED                              */

  SECAPI_HASH_SET_BAM_DSC_MEM_TYPE,    /** Possible Values: SECAPI_BUF_CACHED, SECAPI_BUF_UNCACHED                              */
  SECAPI_HASH_SET_BAM_MODE,            /** Possible Values: SECAPI_BAM_BLOCKING, SECAPI_BAM_NON_BLOCKING                        */

  SECAPI_HASH_GET_DIGEST_SIZE,         /** Returns required digest size in the context                                        */

  SECAPI_HASH_SET_HMAC_HW_KEY_TYPE,    /** Possible Values: values enumerated in SECAPI_AUTH_HW_KEY_TYPE                       */

  SECAPI_HASH_SET_HMAC_KEY,            /** Set HMAC mode, the buffer (and size) point to the HMAC key                         */

  SECAPI_HASH_GET_BLOCK_SIZE,          /** Get the internal block size of the hash algorithm in use                           */

  SECAPI_HASH_SET_HASH_ORDER,          /** Possible Values: SECAPI_HASH_BEFORE_ENCR, SECAPI_HASH_AFTER_ENCR                     */
  SECAPI_HASH_GET_HASH_ORDER,          /** Possible Values: SECAPI_HASH_BEFORE_ENCR, SECAPI_HASH_AFTER_ENCR                     */
  SECAPI_HASH_GET_HASH_ALG,            /** Possible Values: SECAPI_HASH_ALG enumerations                                       */

  SECAPI_HASH_SET_COPY,                /** Copies the hash context passed as an argument into the current hash context handle */

  SECAPI_HASH_SET_WRAPPED_KEY,         /** Accepts data buffer and size. Used to set a wrapped key                            */

  SECAPI_HASH_MAX_PARAM_TYPE_COUNT
}SECAPI_HASH_PARAM_TYPE;


/**
 * @brief Creates a context for hash operation and returns a
 *        handle to the caller. It is the caller responsiblity
 *        to call secapi_hash_free() to properly clean up hash
 *        context and release the resources.
 *
 * @param env    [in] Pointer to environment structure
 *
 * @return Non zero value if successful.
 */
SECAPI_MD_HANDLE secapi_hash_new(void);

/**
 * @brief Zeros hash context and releases internal resources.
 *
 * @param h      [in] handle to the hash context
 *
 * @return SECAPI_SUCCESS if successful. SECAPI Error Code
 *         otherwise.
 */
int secapi_hash_free(SECAPI_MD_HANDLE h);

/**
 * @brief Resets internal states in order to use the handle for
 *        fresh hash operation. The interface retains the hash
 *        algorithm and engine that user has already initialized
 *        for the same handle using Hash Init interface.
 *
 * @param h      [in] handle to the hash context
 *
 * @return SECAPI_SUCCESS if successful. SECAPI Error Code
 *         otherwise.
 */
int secapi_hash_reset(SECAPI_MD_HANDLE h);

/**
 * @brief Writes/reads hash context parameters. Parameters can
 *        be passed as value using [val] or as an array of bytes
 *        (8-bit) using [ibuf, isz] arguments. If client does
 *        not know the required buffer size for a type, it can
 *        call the API with ibuf as NULL and provide valid
 *        pointer to osz to retrieve required buffer size for
 *        associated parameter. Refer to SECAPI_HASH_PARAM_TYPE
 *        definition for more details of each parameter type. To
 *        read any hash parameter, [ibuf, isz] are required.
 *
 * @param h      [in] handle to the hash context
 * @param type   [in] Hash context parameter type
 * @param val    [in] Hash context parameter can be passed by
 *               value using this parameter.
 * @param ibuf   [in, out] pointer to buffer in order to
 *               write/read to/from hash context
 * @param isz    [in] ibuf size in bytes
 * @param osz    [out] pointer to required size on return from
 *               API
 *
 * @return SECAPI_SUCCESS if successful. SECAPI Error Code
 *         otherwise.
 */
int secapi_hash_ctrl(SECAPI_MD_HANDLE h,
                    SECAPI_HASH_PARAM_TYPE type,
                    int val,
                    uint8_t *ibuf,
                    size_t isz,
                    size_t *osz);

/**
 * @brief Initializes hash context based on requested hash
 *        algorithm and acceleration engine. If engine parameter
 *        is passed as zero, the default engine will be used.
 *        The default engine is defined in SECAPI configuration
 *        file.
 *
 * @param h      [in] handle to the hash context
 * @param alg    [in] hash type enumerated in SECAPI_HASH_ALG
 * @param engine [in] engine type enumerated in
 *               SECAPI_ENGINE_TYPE
 *
 * @return SECAPI_SUCCESS if successful. SECAPI Error Code
 *         otherwise.
 */
int secapi_hash_init(SECAPI_MD_HANDLE h, SECAPI_HASH_ALG alg, SECAPI_ENGINE_TYPE engine);

/**
 * @brief  Calculates hash of a message. The hash result is
 *         stored in hash context. API returns error when one
 *         passes null pointer message or (msg + msg_sz) range
 *         causes address overflow. Hash of zero-length message
 *         is supported.
 *
 * @param h        [in] handle to the hash context
 * @param msg      [in] pointer to message buffer
 * @param msg_sz   [in] message buffer size
 *
 * @return SECAPI_SUCCESS if successful. SECAPI Error Code
 *         otherwise.
 */
int secapi_hash_update(SECAPI_MD_HANDLE h, const uint8_t *msg, size_t msg_sz);

/**
 * @brief Copies hash result to digest buffer. Digest buffer
 *        size should be large enough for copying hash result.
 *        User can retrieve required digest buffer size using
 *        secapi_hash_ctrl api. API returns error when caller
 *        passes null pointer digest or digest size as zero.
 *        Digest buffer address range represented by [dgst,
 *        dgst_sz] should not cause address overflow or API
 *        returns with error otherwise.
 *
 * @param h         [in]    handle to the hash context
 * @param dgst      [in]    pointer to digest buffer
 * @param dgst_sz   [in]    digest buffer size
 *
 * @return SECAPI_SUCCESS if successful. SECAPI Error Code
 *         otherwise.
 */
int secapi_hash_final(SECAPI_MD_HANDLE h, uint8_t *dgst, size_t dgst_sz);

/** @} */  /** end_addtogroup crypto_secalgoapi */

#endif
