#include "IPA_test_util_ciph_aes_ctr_f8.h"
#include "IPA_test_util_ciph_qcryptoUtils.h"

#include <iterator>
#include <vector>

using namespace std;
using namespace qcrypto;

AES_CTR_F8::AES_CTR_F8():
  f8Base("AES CTR"),
  myWillCont(false),
  myBitLen(0)
{
  myNextTVtr.resize(AES_128_NUM_BYTE_PER_BLK);
  myKeystrmVtr.resize(AES_128_NUM_BYTE_PER_BLK);
}

void AES_CTR_F8::f8(uint8 *key, int32 count, int32 bearer, int32 dir, uint8 *data, int32 length, uint8 *keystrm, bool willCont)
{

  if(!myWillCont)
  {
    vector<uint8> keyVtr;
    copy(key, key+AES_128_NUM_BYTE_PER_BLK*sizeof(key[0]), back_inserter(keyVtr));
    myAES_CTR.SetKey(keyVtr);

    // T1 = COUNT[0]..COUNT[31] | BEARER[0]..BEARER[4] | DIRECTION | 0^26 (i.e. 26 zero bits)
    myNextTVtr.clear();
    myNextTVtr.resize(AES_128_NUM_BYTE_PER_BLK, 0);
    uint8 cntAry[NUM_BYTE_PER_WORD];
    CopyU32to4U8Array(count, cntAry);
    for(uint32 iB=0; iB<NUM_BYTE_PER_WORD; iB++)
      myNextTVtr[iB] = cntAry[iB];
    myNextTVtr[4] = ((uint8)bearer<<3) | ((uint8)dir<<2);

    //reset the internal state
    myBitLen = 0;
    myKeystrmVtr.resize(AES_128_NUM_BYTE_PER_BLK);
  }

  vector<uint8> PVtr;
  uint32 numByteP = (length+NUM_BIT_PER_BYTE-1)/NUM_BIT_PER_BYTE;
  copy(data, data+numByteP*sizeof(data[0]), back_inserter(PVtr));

  vector<uint8> OVtr; // the keystream vector
  vector<uint8> partialPVtr, partialOVtr, partialCVtr;
  uint32 scheduleLen = length;
  uint32 lastBlkLen = myBitLen;
  /*
  - ciphering the partial 128 bits from the last key schedule
  - modify PVtr if myBitLen !=0, extract some partial bits at the front from PVtr to partialPVtr
  */
  if (myBitLen!=0 && myWillCont)
  {
    // the partial keystream (O)
    partialOVtr = myKeystrmVtr;
    if((uint32)length<myBitLen)
    {
      if(myBitLen-length>=NUM_BIT_PER_BYTE)
        partialOVtr.erase(partialOVtr.begin()+numByteP, partialOVtr.end());
      uint32 numBitShift = NUM_BIT_PER_BYTE - length % NUM_BIT_PER_BYTE;
      if( numBitShift < NUM_BIT_PER_BYTE)
      {
        uint8 byteMask = (0xFF << numBitShift);
        partialOVtr.back()&=byteMask;
      }
      myKeystrmVtr = ShiftBitsVtr<uint8>(myKeystrmVtr, (uint32)length);
    }
    else
      myKeystrmVtr.clear();

    // the partial plaintext (P)
    uint32 numCiphBit = (myBitLen<(uint32)length)?myBitLen:length;
    uint32 numByte = numCiphBit / NUM_BIT_PER_BYTE;
    uint32 numBit  = numCiphBit % NUM_BIT_PER_BYTE;
    if(numByte != 0)
    {
      partialPVtr.insert(partialPVtr.begin(), PVtr.begin(), PVtr.begin()+numByte);
      PVtr.erase(PVtr.begin(), PVtr.begin()+numByte);
    }
    if(numBit != 0)
    {
      uint8 bitMask = (0xFF<<(NUM_BIT_PER_BYTE-numBit));
      partialPVtr.push_back(PVtr.front()&bitMask);
      PVtr = ShiftBitsVtr<uint8>(PVtr, numBit);
    }

    // the partial ciphertext (C)
    for(uint32 iB=0; iB<partialOVtr.size(); iB++)
      partialCVtr.push_back(partialPVtr[iB]^partialOVtr[iB]);

    scheduleLen -= numCiphBit;
    lastBlkLen -= numCiphBit;
  }

  vector<uint8> CVtr;
  if(scheduleLen>0)
  {
    // generate Ts
    vector<uint8> TVtr;
    uint32 numT = (scheduleLen+127)/ 128;
    for(uint32 iT=0; iT<numT; iT++)
    {
      TVtr.insert(TVtr.end(), myNextTVtr.begin(), myNextTVtr.end());
      myNextTVtr=AddWithCarry<uint8>(myNextTVtr,1);
    }
    PVtr.resize(TVtr.size(), 0);
    OVtr.resize(TVtr.size(), 0);
    CVtr = myAES_CTR.Encrypt(PVtr, TVtr, OVtr);

    // combine all new key schedule C and O with the partial vectors
    if(myBitLen!=0 && myWillCont)
    {
      uint32 numBitShift = myBitLen % NUM_BIT_PER_BYTE;
      if(numBitShift!=0)
      {
        ShiftBitsVtr<uint8>(CVtr, numBitShift);
        CVtr.front()|=partialCVtr.back();
        CVtr.insert(CVtr.begin(), partialCVtr.begin(), partialCVtr.end()-1);

        OVtr.front()|=partialOVtr.back();
        OVtr.insert(OVtr.begin(), partialOVtr.begin(), partialOVtr.end()-1);

      }
      else
      {
        CVtr.insert(CVtr.begin(), partialCVtr.begin(), partialCVtr.end());
        OVtr.insert(OVtr.begin(), partialOVtr.begin(), partialOVtr.end());
      }
    }

    //update myKeystrmVtr
    lastBlkLen = scheduleLen % 128;
    if(lastBlkLen!=0)
    {
      myKeystrmVtr.clear();
      myKeystrmVtr.insert(myKeystrmVtr.begin(), OVtr.end()-AES_128_NUM_BYTE_PER_BLK, OVtr.end());
      myKeystrmVtr = ShiftBitsVtr<uint8>(myKeystrmVtr, lastBlkLen);
      myBitLen = 128-lastBlkLen;
    }
    else
    {
      myKeystrmVtr.clear();
      myBitLen = 0;
    }
  }
  else
  {
    CVtr = partialCVtr;
    OVtr = partialOVtr;
  }

  //update the *data and *keystrm
  for(uint32 iB=0; iB<numByteP; iB++)
  {
    data[iB] = CVtr[iB];
    keystrm[iB] = OVtr[iB];
  }

  //update the internal state variables
  myWillCont = willCont;


}

