#ifndef _ECIES_CORE_H
#define _ECIES_CORE_H

#include "uc_comdef.h"
#include "qrlbn_ecc.h"
#include "ecc_generic_shared.h"
/*

  Copyright 2018-2019 Qualcomm Technologies Inc. 
  All Rights Reserved.
  Qualcomm Confidential and Proprietary

*/

/* 
 * Define EXPLICIT_INT_TYPES if the environment does not
 * have sys/types.h and, if necessary, stdint.h
 */

/**
 * ECIES block cipher algorithms
 */
typedef enum 
{
  ECIES_AES_128 = 0,
  ECIES_CIPHER_MAX,
} ecies_cipher_t;

/**
 * Possible purposes of a key (or pair).
 */
typedef enum
{
  ECIES_PURPOSE_ENCRYPT = 0,    /* Usable with RSA, EC and AES keys. */
  ECIES_PURPOSE_DECRYPT = 1,    /* Usable with RSA, EC and AES keys. */
} ecies_purpose_t;

/**
 * Supported EC curves, used in ECDSA/ECIES.
 */
typedef enum
{
  ECIES_CURVE_X25519=0,
  ECIES_CURVE_P256 = 1,
  ECIES_CURVE_MAX,
} ecies_curve_t;

/**
 * Digests provided by keymaster implementations.
 */
typedef enum {
  ECIES_DIGEST_SHA_2_256 = 0,
  ECIES_DIGEST_MAX,
} ecies_digest_t;
/*
 * Key derivation functions, mostly used in ECIES.
 */
typedef enum {
  ECIES_KDF_X9_63 = 0,
  ECIES_KDF_MAX,
} ecies_kdf_t;

typedef struct _ecies_domain_vect {
  uint8_t   *m;
  uint8_t   *a;
  uint8_t   *b;
  uint8_t   *G_x;
  uint8_t   *G_y;
  uint8_t   *n;
  uint32_t   h;
} ecies_domain_vect_t;

typedef struct _ecies_key {
    union {
        qrlbn_ecc_affine_point_t * recipient_public_key;
        qrlbn_ecc_bigval_t * private_key;
    } enc_dec;
} ecies_key_t;

typedef struct _ecies_params {
  ecies_curve_t        ecies_curve;      /* ECC curve */
  ecies_kdf_t          kdf;              /* kdf */
  ecies_digest_t       digest;           /* digest hash function */
  ecies_cipher_t       cipher;           /* symetric algorithm (AES) scheme */
} ecies_params_t;

typedef struct _ecies_ctx {
  qrlbn_ecc_domain_t    domain;           /* ECC domain */
  ecies_params_t        params;              /* ECIES parameters */
} ecies_ctx_t;

/* 
 * Define either ECC_P256 or ECC_P192 to control which polynomial is used.
 * Of course, the binary must have been compiled with the same .h file
 * contents as are used by applications.
 *
 * Arguably, to avoid branching in version control systems, this
 * should be supplied by a -DP256 or -DP192 at compile time. It would be
 * good to include a run-time check to make sure the
 * library was compiled with the same modulus as the include file
 * specifies, but it seems require that the main "functions" be
 * macros, and I'm not sure I want to do that.
 */

sw_crypto_errno_enum_type ECIES_init(ecies_ctx_t *ctx, ecies_params_t *params);


sw_crypto_errno_enum_type ECIES_update(
        ecies_ctx_t         *ctx,
        ecies_key_t         *key,
        ecies_purpose_t     purpose,
        const uint8_t       *msg,
        const uint32_t      msg_len,
        uint8_t        	    **ppAD,
        uint32_t            *pAD_len,
        uint8_t             *out,
        uint32_t            *out_len
        );

sw_crypto_errno_enum_type ECIES_finish(ecies_ctx_t *ctx);

#endif /* _ECIES_CORE_H */
