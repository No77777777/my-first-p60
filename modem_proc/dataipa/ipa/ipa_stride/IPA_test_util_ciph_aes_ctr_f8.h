#ifndef IPA_TEST_UTIL_CIPH_AES_CTR_EEA2_H
#define IPA_TEST_UTIL_CIPH_AES_CTR_EEA2_H

/*!
  \file
  - implement 128-bit EEA2 AES for LTE
  - has bit level and will continue capabilities
  - maintain states

  - Revision History:
  - Date / Who / What
    - 2008-10-16 Samson Jim Initial version

  - Copyright (C) 2008 Qualcomm Technologies Inc., All rights reserved.
*/

#include "IPA_test_util_ciph_aes_ctr.h"
#include "IPA_test_util_ciph_f8Base.h"

namespace qcrypto
{
class AES_CTR_F8 : f8Base
{
public:
  AES_CTR_F8();

  /*=========================================================*/
  // FUNCTION: f8
  //
  //! perform f8 de/ciphering function
  //
  /*!
    - Output data:
    -# Output bit stream after cipher/deciphering stroed in *data.\n
    -# keystream generated from AES-128 CTR
    -# length has to be a multiple of byte when the willCont is used
    \param *key    128 bit, Confidentiality Key
    \param count  32-bit, Count, Frame dependent input
    \param bearer 5-bit, Bearer identity (in the LSB side)
    \param dir    1 bit, direction of transmission
    \param *data   length number of bits, input bit stream.(IBS)
    \param length 16 bit Length, i.e., the number of bits to be encrypted or decrypted.
    \param *keyStrm an array of keyStrm
    \param cont continue from the last de/ciphering
   */
  // REVISION HISTORY
  // 17 Oct 2008. Created by Samson Jim
  /*=========================================================*/
  virtual void f8(uint8* key, int32 count, int32 bearer, int32 dir, uint8 *data, int32 length, uint8 *keystrm, bool willCont);

  /* destructor */
  ~AES_CTR_F8()
  {};

private:
  AES_CTR myAES_CTR;            //!< AES CTR algorithm

  //! @name internal state for maintaining continuous de/ciphering
  //! @{
  bool myWillCont;                  //!< will continue
  vector<uint8> myNextTVtr;         //!< my next 128-bit input block
  vector<uint8> myKeystrmVtr;       //!< remaining keystream before scheduling AES CTR for another block
  uint32 myBitLen;                  //!< the bit length in the myKeystrmVtr
  //! @}
};
}
#endif

