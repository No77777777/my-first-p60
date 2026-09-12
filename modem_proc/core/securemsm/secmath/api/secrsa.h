/** @file  
                        secrsa.h

  This file contains the definitions of the constants, data structures
  and interfaces that provide RSA PKCS #1 v2.1 Alogrithms support. It
  defines the external interfaces for RSA encrpytion/decryption, signature/
  verification, key generation/import/export.


  Copyright (c) 2016 Copyright Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.

**/

/*=============================================================================
                EDIT HISTORY


 when       who    what, where, why
 --------   ---     ----------------------------------------------------------
 07/28/16   SA     Initial version

=============================================================================*/

#ifndef CE_RSA_H
#define CE_RSA_H

#ifdef __cplusplus
extern "C" {
#endif


#include "secmath.h"
#include "comdef.h"

typedef enum {
  /* Result OK */
  CE_SUCCESS = E_SECMATH_SUCCESS,
  /* Generic Error */
  CE_ERROR_FAILURE = E_SECMATH_FAILURE,
  /* Not a failure but no operation was performed */
  CE_ERROR_NOP = E_SECMATH_NOP,
  /* Algorithm failed test vectors */
  CE_ERROR_FAIL_TESTVECTOR = E_SECMATH_FAIL_TESTVECTOR,
  /* Not enough space for output */
  CE_ERROR_BUFFER_OVERFLOW = E_SECMATH_BUFFER_OVERFLOW,
  /* Out of memory */
  CE_ERROR_NO_MEMORY = E_SECMATH_MEM,
  /* Generic invalid argument */
  CE_ERROR_INVALID_ARG = E_SECMATH_INVALID_ARG,
  /* Invalid size of prime requested */
  CE_ERROR_INVALID_SIZE = E_SECMATH_INVALID_PRIME_SIZE,
  /* Operation not supported */
  CE_ERROR_NOT_SUPPORTED = E_SECMATH_NOT_SUPPORTED,
  /* invalid signature  */
  CE_ERROR_INVALID_SIGNATURE = 0x1000,
  /* decrypt error  */
  CE_ERROR_DECRYPT_ERROR = 0x1002,
  /* invalid packet  */
  CE_ERROR_INVALID_PACKET = 0x1003,
  /* message too long  */
  CE_ERROR_MESSAGE_TOO_LONG = 0x1004,
  /* Modulus too small (OAEP) */
  CE_ERROR_MODULUS_TOO_SMALL = 0x1005,
} CeErrorType;

#define STATIC static
#define CONST const
typedef void  VOID;
typedef unsigned long  INT32;
typedef unsigned char  UINT8;
#define BOOLEAN UINT8

#define TRUE 1
#define FALSE 0
#define S_BIGINT_POS  0   /* positive */
#define S_BIGINT_NEG  1   /* negative */
#ifndef NULL
  #define NULL (void*)0
#endif

typedef struct S_BIGINT {
  BigInt bi;
	int sign;
} S_BIGINT;

/*RSA padding type. PKCS #1 v2.1*/
typedef enum {
  /*PKCS1 v1.5 signature*/
  CE_RSA_PAD_PKCS1_V1_5_SIG = 1,
  /*PKCS1 v1.5 encryption*/
  CE_RSA_PAD_PKCS1_V1_5_ENC = 2,
  /*OAEP Encryption*/
  CE_RSA_PAD_PKCS1_OAEP = 3,
  /*PSS Signature*/
  CE_RSA_PAD_PKCS1_PSS = 4,
  /* No Padding */
  CE_RSA_NO_PAD = 5,
  /*PSS with unknown saltlen*/
  CE_RSA_PAD_PKCS1_PSS_AUTORECOVER_SALTLEN = 6,
  CE_RSA_PAD_INVALID = 0x7FFFFFFF,
} CE_RSA_PADDING_TYPE;

typedef struct CE_RSA_OAEP_PAD_INFO {
  /** index of Hash & Mask generation function desired */
  INT32 hashidx;
  /** Label to add to the message */
  UINT8 *label;
  /** Length of label */
  INT32 labellen;
} CE_RSA_OAEP_PAD_INFO;

/*index of hash algorithm used for generating signature */
typedef enum
{
   CE_HASH_IDX_NULL        = 0x1,
   CE_HASH_IDX_SHA1        = 0x2,
   CE_HASH_IDX_SHA256      = 0x3,
   CE_HASH_IDX_SHA224      = 0x4,
   CE_HASH_IDX_SHA384      = 0x5,
   CE_HASH_IDX_SHA512      = 0x6,
   CE_HASH_IDX_MAX         = 0x7,
   CE_HASH_IDX_INVALID     = 0x7FFFFFFF
}CE_HASH_IDX;

typedef struct CE_RSA_PSS_PAD_INFO {
  /** index of Hash & Mask generation function desired */
  INT32 hashidx;
  /** Length of salt */
  INT32 saltlen;
} CE_RSA_PSS_PAD_INFO;

/** RSA PKCS key */
typedef struct CE_RSA_KEY {
  /** Type of key, CE_RSA_PRIVATE or CE_RSA_PUBLIC */
  INT32 type;
  /** RSA key bit length **/
  INT32 bitLength;
  /** The public exponent */
  S_BIGINT *e;
  /** The private exponent */
  S_BIGINT *d;
  /** The modulus */
  S_BIGINT *N;
  /** The p factor of N */
  S_BIGINT *p;
  /** The q factor of N */
  S_BIGINT *q;
  /** The 1/q mod p CRT param */
  S_BIGINT *qP;
  /** The d mod (p - 1) CRT param */
  S_BIGINT *dP;
  /** The d mod (q - 1) CRT param */
  S_BIGINT *dQ;
} CE_RSA_KEY;
typedef struct
{
  BLONG *a;                            //< Pointer to the value
  int n;                               //< Current length of a
} BigNum;
typedef struct CE_RSA_KEY_BIGNUM {
  /** Type of key, CE_RSA_KEY_PUBLIC for encryption or
    *  CE_RSA_KEY_PRIVATE/CE_RSA_KEY_PRIVATE_CRT for
    *  decryption or CE_RSA_KEY_PRIVATE_PUBLIC or
    *  CE_RSA_KEY_PRIVATE_CRT_PUBLIC for key generation */
  INT32 type;
  /** RSA key bit length **/
  int bitLength;
  /** The public exponent */
  BigNum e;
  /** The private exponent */
  BigNum d;
  /** The modulus */
  BigNum N;
  /** The p factor of N */
  BigNum p;
  /** The q factor of N */
  BigNum q;
  /** The 1/q mod p CRT param */
  BigNum qP;
  /** The d mod (p - 1) CRT param */
  BigNum dP;
  /** The d mod (q - 1) CRT param */
  BigNum dQ;
} CE_RSA_KEY_BIGNUM;

/*CE RSA Key Type*/
typedef enum {
  CE_RSA_KEY_PUBLIC = 0,             /*public key*/
  CE_RSA_KEY_PRIVATE = 1,            /*private key in non CRT representation */
  CE_RSA_KEY_PRIVATE_CRT = 2,        /*private key in CRT representation*/
  CE_RSA_KEY_PRIVATE_PUBLIC = 3,     /*private/public key pair*/
  CE_RSA_KEY_PRIVATE_CRT_PUBLIC = 4, /*private CRT/public key pair*/
  CE_RSA_KEY_INVALID = 0x7FFFFFFF
} CE_RSA_KEY_TYPE;

/*CE RSA Key Size*/
#define CE_RSA_KEY_SIZE(key) ((key)->bi.n * BLONG_SIZE)

/**
   (PKCS #1 v1.5) PKCS1 pad then encrypt
  @param[in]  key          The RSA key to encrypt to
  @param[in]  padding_type Type of padding (CE_RSA_PAD_PKCS1_V1_5_ENC or CE_RSA_PAD_PKCS1_OAEP)
  @param[in]  padding_info The parameters for OAEP padding
  @param[in]  msg          The plaintext
  @param[in]  msglen       The length of the plaintext (octets)
  @param[out] cipher       The ciphertext
  @param[in,out] cipherlen The max size and resulting size of the ciphertext

  @return 
   CE_SUCCESS     - Function executes successfully.
   CE_ERROR_NOT_SUPPORTED - the feature is not supported.
   CE_ERROR_INVALID_PACKET - invalid packet.
   CE_ERROR_BUFFER_OVERFLOW - not enough space for output.

  @dependencies
  None. 
*/
INT32 ce_rsa_encrypt(
    CE_RSA_KEY *key,
    CE_RSA_PADDING_TYPE padding_type,
    void *padding_info,
    UINT8 *msg,
    INT32 msglen,
    UINT8 *cipher,
    INT32 *cipherlen);


#ifdef __cplusplus
}
#endif

#endif

/**
  count total bits numbers in S_BIGINT BLONG array
  @param[in]  s         S_BIGINT data

  @return 
   total bits numbers
   
  @dependencies
  None. 
*/
INT32 ce_util_count_bits(S_BIGINT *bi);

/**
  count total byte numbers in S_BIGINT BLONG array
  @param[in]  s         S_BIGINT data

  @return 
   total byte numbers
   
  @dependencies
  None. 
*/
INT32 ce_util_count_bytes(S_BIGINT *bi);

/** 
   Compute an RSA modular exponentiation 
  @param[in]  key        The RSA key to use 
  @param[in]  in         The input data to send into RSA
  @param[in]  inlen      The length of the input (octets)
  @param[out] out        The destination 
  @param[in,out] outlen  The max size and resulting size of the output
  @param[in]  which      Which exponent to use, e.g. PRIVATE or PUBLIC

  @return 
   CE_SUCCESS     - Function executes successfully. \n
   CE_ERROR_NOT_SUPPORTED - the feature is not supported. \n
   CE_ERROR_INVALID_PACKET - invalid packet. \n
   CE_ERROR_BUFFER_OVERFLOW - not enough space for output. \n

  @dependencies
  None. 

*/
INT32 ce_rsa_exptmod
(
    CE_RSA_KEY *key,
    CONST UINT8 *in,
    INT32 inlen,
    UINT8 *out,
    INT32 *outlen,
    INT32 which
);

/**
  PKCS #1 OAEP encode.
 
  @param[in] msg              The data to encode
  @param[in] msglen           The length of the data to encode (octets) 
  @param[in] lparam           Label to  identify which system 
                              encoded the message
  @param[in] lparamlen        The length of Label
  @param[in] modulus_bitlen   The bit length of the RSA modulus
  @param[in] hashidx          The hash index for encryption
  @param[out] out             [out] The destination for the encoded data
  @param[in,out] outlen       [in/out] The max size and resulting size of the encoded data

  @return 
   CE_SUCCESS     - Function executes successfully. \n
   CE_ERROR_NOT_SUPPORTED - the feature is not supported. \n
   CE_ERROR_INVALID_PACKET - invalid packet. \n
   CE_ERROR_BUFFER_OVERFLOW - not enough space for output. \n

  @dependencies
  None. 
 */
int ce_pkcs1_oaep_padding_encode
(
   const unsigned char     *msg,
   unsigned long           msglen,
   const unsigned char     *lparam,
   unsigned long           lparamlen,
   unsigned long           modulus_bitlen,
   int                     hash_idx,
   unsigned char           *out,
   unsigned long           *outlen
);    
    