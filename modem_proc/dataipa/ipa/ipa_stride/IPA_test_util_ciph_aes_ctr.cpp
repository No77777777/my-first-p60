#include "IPA_test_util_ciph_aes_ctr.h"

using namespace std;
using namespace qcrypto;

void AES_CTR::SetKey(const vector<uint8>& key)
{
  myAES.SetKey(key);
}

vector<uint8> AES_CTR::Encrypt(const vector<uint8>& P, std::vector<uint8> T, vector<uint8>& O)
{
  vector<uint8> C;
  uint32 numB = (uint32) P.size();
  if(numB!=T.size())
    cerr<<"AES_CTR::Encrypt The sizes of the plaintext and counter sequence are not equal\n";
  C.resize(numB);
  O.resize(numB);
  O = myAES.Encrypt(T);
  for(uint32 iB=0; iB<numB; iB++)
    C[iB] = P[iB]^O[iB];
  return C;
}

vector<uint8> AES_CTR::Decrypt(const vector<uint8>& C, std::vector<uint8> T, vector<uint8>& O)
{
  return Encrypt(C, T, O);
}
