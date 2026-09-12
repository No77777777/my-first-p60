#include "IPA_test_util_ciph_qcryptoUtils.h"

void qcrypto::CopyU32to4U8Array(uint32 u32Val, uint8 u8Array[])
{
  for(int i=0; i<4; i++)
  {
      u8Array[i] = (uint8) (u32Val>>24);
      u32Val = (u32Val<<8);
  }
}

uint32 qcrypto::Copy4U8toU32(uint8* u8Array)
{
  uint32  output = 0;
  for (int iByte=0; iByte<4; iByte++)
      output = (output<<8) | (uint32) u8Array[iByte];
  return output;
}
