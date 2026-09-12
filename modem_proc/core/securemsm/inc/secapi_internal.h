#ifndef SECAPI_INTERNAL_H
#define SECAPI_INTERNAL_H
/** 
  @file secapi_internal.h
 */
/*===========================================================================
Copyright (c) {2018} Qualcomm Technologies, Inc.
All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE
  $Header: //components/rel/core.mpss/10.0/securemsm/inc/secapi_internal.h#2 $ 
  $DateTime: 2025/09/24 05:32:23 $ 
  $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
13-02-18  sahaj    Initial Version (FR#44835)
=============================================================================*/
#include "secerrno.h"
#include "uc_comdef.h"

/*
  Generates the ecc key pair when curve id is given 

  @param[out] peer_key               Pointer to the Peer Key.
  @param[in/out] peer_keylen         Peer key length.
  @param[out] priv_key               Poiner to the private key.
  @param[in/out] priv_keylen         Private key length.
  @param[in]  ecc_curve_id           ECC Curve ID.

  @return
  E_SUCCESS     - Function executes successfully. \n
  E_INVALID_ARG - sanity check failed.
  E_FAILURE     - Any other failures.

  @dependencies:
  @Limitation:
  @INFO:
*/
sw_crypto_errno_enum_type pkx_ecc_keygen(
                        uint8_t*              peer_key,
                        size_t*               peer_keylen,
                        uint8_t*              priv_key,
                        size_t*               priv_keylen,
                        SECAPI_UC_EC_CURVE_ID curve_id
);

/*
  Generates the shared secret key when private & peer keys given 
 
  @param[out] shared                 Poiner to the shared key.
  @param[in/out] shared_len          Shared key length.
  @param[in] peerkey                 Poiner to the peer key.
  @param[in] peerkey_len             Peer key length.
  @param[in] privkey                 Poiner to the private key.
  @param[in] privkey_len             Private key length.
  @param[in] ecc_curve_id            ECC curve ID.

  @return
  E_SUCCESS     - Function executes successfully. \n
  E_INVALID_ARG - sanity check failed.
  E_FAILURE     - Any other failures.

  @dependencies:
  @Limitation:
  @INFO:
*/
sw_crypto_errno_enum_type pkx_ecc_dh(
                        uint8_t*              shared,
                        size_t*               shared_len,
                        uint8_t*              peerkey,
                        size_t                peerkey_len,
                        uint8_t*              privkey,
                        size_t                privkey_len,
                        SECAPI_UC_EC_CURVE_ID ecc_curve_id
);
/**
Callback function pointer for SECAPI_GET_RANDOM.
*/
typedef void (*secapi_get_random_cb_fptr_type)( void* );

/**
  Registers a callback that allows the SECRND task to notify the calling 
  task when random data is available.

  In several different scenarios, a task may require random numbers before the 
  SECRND task has been initialized. In such cases, the call to 
  secapi_get_random() returns E_AGAIN. It is the calling task's 
  responsibility to register a callback. This allows the SECRND task to 
  notify the calling task when random data is available. Since the callback 
  function is fired from the SECRND task context, users should not execute 
  any blocking operations that can cause the watchdog to time out. 

  The calling task should not call secapi_get_random() in the callback function.
  
  @param[in] get_random_cb User-defined callback function.
  @param[in] usr_data      User-defined data (can be NULL).

  @return
  E_SUCCESS     - Successful. \n
  E_INVALID_ARG - Callback function pointer is null. \n
  E_NO_MEMORY   - Reached the maximum number of callback function pointers that can be registered. \n 
  E_FAILURE     - SECRND task has already been initialized.

  @dependencies
  None.
*/
extern secerrno_enum_type secapi_get_random_set_cb
(
  secapi_get_random_cb_fptr_type  get_random_cb,
  void*                           usr_data
);

#endif /* SECAPI_INTERNAL_H */
