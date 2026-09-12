#ifndef _ECC_SW_CORE_H
#define _ECC_SW_CORE_H

#include "uc_comdef.h"

/*

  Copyright 2021 Qualcomm Technologies Inc. 
  All Rights Reserved.
  Qualcomm Confidential and Proprietary

*/

/* 
 * Define EXPLICIT_INT_TYPES if the environment does not
 * have sys/types.h and, if necessary, stdint.h
 */
 
#define ECC_MAX_KEY_SIZE (ECC_MAX_MODULUS_BITS/8)

struct ecc_domain_vector_type
{
   uint8_t   *m;
   uint8_t   *a;
   uint8_t   *b;
   uint8_t   *G_x;
   uint8_t   *G_y;
   uint8_t   *n;
   uint32_t  key_size;         //size in bits
   uint32_t  h;
};

/* EC Curve types */
typedef enum {
  SECAPI_UC_EC_CURVE_P256 = 0,
  SECAPI_UC_EC_CURVE_P384,
  SECAPI_UC_EC_CURVE_P521,
  SECAPI_UC_EC_CURVE_MAX,
} SECAPI_UC_EC_CURVE_ID;

sw_crypto_errno_enum_type pkx_ecc_keygen(uint8_t *peer_key, size_t *peer_keylen, uint8_t *priv_key, size_t *priv_keylen, SECAPI_UC_EC_CURVE_ID curve_id);

sw_crypto_errno_enum_type pkx_ecc_dh(uint8_t *shared, size_t *shared_len, uint8_t *peerkey, size_t peerkey_len, uint8_t *privkey, size_t privkey_len, SECAPI_UC_EC_CURVE_ID ecc_curve_id);


#endif /* _ECC_SW_CORE_H */
