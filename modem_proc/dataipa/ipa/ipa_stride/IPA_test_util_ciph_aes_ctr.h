/*!
  \file AES-CTR implementation
  - http://csrc.nist.gov/publications/nistpubs/800-38a/sp800-38a.pdf
  - see chapter 6.5

  - Revision History:
  - Date / Who / What
  - 2008-10-16 Samson Jim Initial version

  - Copyright (C) 2008 Qualcomm Technologies Inc., All rights reserved.
*/

#ifndef IPA_TEST_UTIL_CIPH_AES_CTR_H
#define IPA_TEST_UTIL_CIPH_AES_CTR_H

#include "IPA_test_util_ciph_aes.h"
#include "IPA_test_util_ciph_clbtypes.h"
#include "IPA_test_util_ciph_qcryptoConstant.h"

namespace qcrypto
{
class AES_CTR
{
public:
  /*!
    \param key 128-bit key, which vector size is 128/8=16
  */
  void SetKey(const vector<uint8>& key);

  /*!
    \param P plaintext
    \param T sequence of counter
    \return  ciphertext
  */
  vector<uint8> Encrypt(const vector<uint8>& P, vector<uint8> T, vector<uint8>& O);

  /*!
    \param C ciphertext
    \param T sequence of counter
    \return  plaintext
  */
  vector<uint8> Decrypt(const vector<uint8>& C, vector<uint8> T, vector<uint8>& O);

private:
  AES_class myAES;  //!< AES 128-bit engine
};
}

#endif
