#ifndef _ECIES_SHARED_H_
#define _ECIES_SHARED_H_

/*===========================================================================

                             Unified Crypto 

GENERAL DESCRIPTION
Provide ECDH and ECDSA cryptography. Currently support only curve P256 which
is 128 bits security level, which is recommended by NIST

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2012, 2019 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/core.mpss/10.0/securemsm/crypto/shared/src/ecies_shared.h#1 $
  $DateTime: 2019/09/18 10:14:17 $
  $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/16/12   yk     initial version
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/


#include "ecies_core.h"
#include "uc_env.h"
#include "uc_comdef.h"


// for debugging
#define ECIES_OUTPUT_DEBUG_VALUES 0

// Limit for the length of the AD and plaintext in the AD versions
#define ECIES_MAX_DATA_LEN ((size_t)((1ULL<<31)-AES_BLOCKSIZE))

sw_crypto_errno_enum_type SW_ECIES_init(ecies_ctx_t *ctx, ecies_params_t *params);

sw_crypto_errno_enum_type SW_ECIES_update(
        ecies_ctx_t         *ctx,
        ecies_key_t         *key,
        ecies_purpose_t     purpose,
        const uint8_t       *msg,
        const uint32_t      msg_len,
        uint8_t             **ppAD,
        uint32_t            *pAD_len,
        uint8_t             *out,
        uint32_t            *out_len
        );

sw_crypto_errno_enum_type SW_ECIES_finish(ecies_ctx_t *ctx);

#endif /* _ECIES_SHARED_H_ */
