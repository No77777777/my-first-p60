
//! ===========================================================================
//! \brief  Simple wrapper class for the AES encrypr/decrypt functions.
//!
//!  - Description: Biggest purpose is to do some conversion between types.
//!
//!  - Revision History:
//!  - Date / Who / What
//!  - 2008-08-13 mbrehler Initial version
//!
//!  - Copyright (C) 2008 Qualcomm Technologies Inc., All rights reserved.
//! ===========================================================================

#ifndef IPA_TEST_UTIL_CIPH_AES_H
#define IPA_TEST_UTIL_CIPH_AES_H
#include <assert.h>
#include <iostream>
#include <iomanip>
#include "IPA_test_util_ciph_clbtypes.h"
#include <vector>
#include <amssassert.h>

using namespace std;

// 2 quick helpers
vector<uint8> stringHex2vectorOctet(const string& str);
std::ostream& operator<<(std::ostream &o, const vector<uint8>& vec);

#if 1

class AES_class
{
public:
  AES_class()
  {};
  AES_class(const vector<uint8>& key) : myKey(key)
  {
    CheckKeySize();
  }

  void SetKey(const vector<uint8>& key)
  {
    myKey = key;
    CheckKeySize();
  }
  const vector<uint8>& GetKey() const
  {
    return myKey;
  }
  vector<uint8> Encrypt(const vector<uint8>& clearMsg);
  vector<uint8> Encrypt(const uint8* const clearMsg, size_t numBytes);

  vector<uint8> Decrypt(const vector<uint8>& cipherMsg);
  vector<uint8> Decrypt(const uint8* const cipherMsg, size_t numbytes);

  ~AES_class()
  {};

private:
  vector<uint8> myKey;
  void CheckKeySize()
  {
    ASSERT(myKey.size()==16); // nothing else supported for now
  }
  void SetKeyInCInterface();
};
#endif
#endif /* IPA_TEST_UTIL_CIPH_AES_H */
