#include "IPA_test_util_ciph_snow3Gf8.h"
#include <stdlib.h>

using namespace qcrypto;

  /*==============
  PUBLIC FUNCTIONS
  ================*/

Snow3Gf8::Snow3Gf8():
  f8Base("SNOW3G"),
  _willCont(false),
  _lastKS(0),
  _lastBitLength(0)
{

}

void Snow3Gf8::f8( uint8 *key, int32 count, int32 bearer, int32 dir, uint8 *data, int32 length, uint8 *keyStrm, bool willCont )
{
  uint32 K[4],IV[4];
  int32 n = (length + 31)/32;
  int32 i=0;
  uint32 *KS;

  if (n==0)
  {
    if (_willCont)
      _willCont = willCont;
    return;
  }

  // if the last call has ordered to continue the f8 in the next data block
  if (!_willCont)
  {
    /*=============/
     Initialisation
    ================*/
    /*
        Load the confidentiality key for SNOW 3G initialization as in section 3.4 of TS35.215
        key = CK [0]|CK [1]|...|CK[127]
        K[3]= CK [0]|CK [1]|...|CK [31]
        K[2]= CK[32]|CK[33]|...|CK [63]
        K[1]= CK[64]|CK[65]|...|CK [95]
        K[0]= CK[96]|CK[97]|...|CK[127]
    */
    K[3] = Copy4U8toU32(key+0);
    K[2] = Copy4U8toU32(key+4);
    K[1] = Copy4U8toU32(key+8);
    K[0] = Copy4U8toU32(key+12);
    /* Prepare the initialization vector (IV) for SNOW 3G initialization as in
     3.4.of TS35.215 */
    IV[3] = count;
    IV[2] = (bearer << 27) | ((dir & 0x1) << 26);
    IV[1] = IV[3];
    IV[0] = IV[2];
    /* Run SNOW 3G algorithm to generate sequence of key stream bits KS */
    _snow3G.initialize(K,IV);
    _lastBitLength=0;
  }

  KS = (uint32 *)malloc(4*n);
  uint32 lastKS;
  /*if
    - !WILL_CONTINUE OR
    - needs to generate new keystream to cover the length OR
    - WILL_CONTINUR AND the last all-bit keystream have been used
    */
  if (!_willCont || length>(32-_lastBitLength) || ( _willCont & (_lastBitLength==0) ))
  {
    KS[n-1] = 0;
    // number of round needed to generate the required length of keystream
    int32 nGenKey = n;
    if (_willCont && _lastBitLength!=0 )
      nGenKey = (length - (32 - _lastBitLength) + 31 )/32;
    _snow3G.generateKeystream(nGenKey,(uint32*)KS, _willCont);
    lastKS = (nGenKey==0)?_lastKS:KS[nGenKey-1];
  }
  else
  {
    KS[0] = 0x00;
    lastKS = _lastKS;
  }

  /*
    _willCont && _lastBitLength!=0: last willCont bit is ON, need to restore part of the unciphered keystream
    n!=0: there is at least one bit need to be ciphered
  */
  if(_willCont && _lastBitLength!=0)
    InsertBitsArray<uint32>(KS, n, _lastKS, 32-_lastBitLength, 0);
  _lastKS = lastKS;
  _lastBitLength = (length+_lastBitLength)%32;

  //update willCont state for the next call
  _willCont = willCont;

  /*=============/
   Generate OBS
  ================*/
  /* Exclusive-OR the input data with keystream to generate the output bit
  stream */
  for (i=0; i<n; i++)
  {
      uint32 mask = 0xFFFFFFFF;
      uint32 KSi = KS[i];
      //only XOR up to IBS[LENGTH-1]
      if (length < (i+1)*32)
      {
          mask = mask << (32 - ((uint32)length % 32));
      }
      for( int32 j=0; j<4 && (i*32+j*8)<length; j++)
      {
          keyStrm[i*4+j] = (uint8) ((KSi & mask) >> 24);
          data[i*4+j] ^= keyStrm[i*4+j];
          KSi=KSi<<8;
          mask = mask << 8;
      }
  }
  free(KS);
}
