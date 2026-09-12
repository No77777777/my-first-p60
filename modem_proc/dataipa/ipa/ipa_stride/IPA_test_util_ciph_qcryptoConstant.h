#ifndef IPA_TEST_UTIL_CIPH_QCRYPTO_CONSTANT_H
#define IPA_TEST_UTIL_CIPH_QCRYPTO_CONSTANT_H

#include "IPA_test_util_ciph_clbtypes.h"

namespace qcrypto
{
  const uint8   NUM_BIT_PER_BYTE         = 8;
  const uint8   NUM_BYTE_PER_WORD        = 4;
  const uint8   AES_128_NUM_BYTE_PER_BLK = 16; //!< 128/8 = 16
  const uint8   AES_128_NUM_BIT_PER_BLK  = 128;

  enum SecurityEngine
  {
    KASUMI = 0,
    SNOW3G = 1,
    AES128 = 2,
  };
  const uint32 securityEngineMaxLen  = 10;
  const char securityEngineLookup[][securityEngineMaxLen] =
  {
    "KASUMI",
    "SNOW3G",
    "AES128"
  };
}
#endif

