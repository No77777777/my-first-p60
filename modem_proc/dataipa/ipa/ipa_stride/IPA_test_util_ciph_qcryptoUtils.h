#ifndef IPA_TEST_UTIL_CIPH_QCRYPTO_UTILS_H
#define IPA_TEST_UTIL_CIPH_QCRYPTO_UTILS_H

#include <iostream>
#include "IPA_test_util_ciph_clbtypes.h"

#include <vector>

using std::vector;
using std::cerr;

namespace qcrypto
{
/*=========================================================*/
// FUNCTION: copyU32to4U8Array
//
//! copy an usigned int to an array of unsigned char
//
/*!
  \param uint32       an uint32 value
  \param u8Array   the return uint8 array
  independent of Big/Little Endian
 */
// REVISION HISTORY
// 5 March 2007. Modified by Samson Jim
/*=========================================================*/
void CopyU32to4U8Array(uint32 u32Val, uint8 u8Array[]);

/*=========================================================*/
// FUNCTION: copy4U8toU32
//
//! copy four unsigned char to an unsigned int
//
/*!
  \param  u8Array  uint8 array, size of 4
  \return uint32 value
  e.g.
  char x[4]=(0x2b, 0xd6, 0x45, 0x9f)
  int y=0x2bd6459f
 */
// REVISION HISTORY
// 5 March 2007. Modified by Samson Jim
/*=========================================================*/
uint32 Copy4U8toU32(uint8* u8Array);

/*=========================================================*/
// FUNCTION: insertBitsArray
//
//! insert some bits into the array
//
/*!
  - insert some bits into the array
    - e.g. X={00000000 00000001 00000010 00000011 00000000}\n
    insert I=111100000 (the first three bits) in front of the second element\n
    insertBitsArray(&X, 5, I, 3, 1);\n
    result: X={00000000 11100000 00100000 01000000 01100000}\n
  \param TArray      the pointer to the array (should have enough size to contain all elements after insertion)
  \param lengthArray the length of the array
  \param insertBits  the inserting bits
  \param lengthBit   the length of the bits inserting
  \param startArray  the position of where the bits inserting
 */
// REVISION HISTORY
// 5 March 2007. Modified by Samson Jim
/*=========================================================*/
template <typename T>
void InsertBitsArray(T* TArray, int lengthArray, T insertBits, int lengthBit, int startArray)
{
  if (lengthBit==0)
    return;
  int sizeofTBit = sizeof(T)*8;

  //error checking
  if (startArray>=lengthArray)
    std::cerr<<"IPAUtils.h, insertBitsArray, the startArray is larger then the array length\n";
  if (lengthBit > sizeofTBit)
    std::cerr<<"IPAUtils.h, insertBitsArray, lengthBit > the size of T\n";

  if (lengthBit==sizeofTBit)
  {
    for(int iEle=(lengthArray-1); iEle>startArray; iEle--)
      TArray[iEle]=TArray[iEle-1];
    TArray[startArray]=insertBits;
  }
  else
  {
    int left = sizeofTBit - lengthBit;
    int right = lengthBit;
    T leftMask=0;
    T rightMask=(T)ULONG_MAX;
    if (right!=sizeofTBit)
      leftMask = ((T)ULONG_MAX) << right;
    rightMask = ((T)ULONG_MAX) >> left;
    T lastT;
    for(int iEle=startArray; iEle<lengthArray; iEle++)
    {
      lastT=TArray[iEle];
      TArray[iEle]=(insertBits&rightMask)<<left | (lastT&leftMask)>>right;
      insertBits = lastT;
    }
  }
}

/*=========================================================*/
// FUNCTION: insertBitsVtr
//
//! insert some bits into the vector
//
/*!
  - insert some bits into the vector
    - e.g.  X={00000000 00000001 00000010 00000011 00000000}\n
    insert  I=11100000 (the first three bits) in front of the second element\n
    insertBitsArray(X, I, 3, 1);\n
    result: X={00000000 11100000 00100000 01000000 01100000}\n
  \param vtrT      the pointer to the vector (should have enough size to contain all elements after insertion)
  \param insertBits  the inserting bits
  \param lengthBit   the length of the bits inserting
  \param startVtr  the position of where the bits inserting
 */
// REVISION HISTORY
// 5 March 2007. Modified by Samson Jim
/*=========================================================*/
template <typename T>
vector<T> InsertBitsVtr(vector<T> vtrT, T insertBits, int lengthBit, int startVtr)
{
  if (lengthBit==0)
    return vtrT;
  int sizeofTBit = sizeof(T)*8;

  //error checking
  if (startVtr>=(int)vtrT.size())
    cerr<<"IPAUtils.h, insertBitsVtr, the startVtr is larger then the vector length\n";
  if (lengthBit > sizeofTBit)
    cerr<<"IPAUtils.h, insertBitsVtr, lengthBit > the size of T\n";

  if (lengthBit==sizeofTBit)
  {
    for(int iEle=(int)(vtrT.size()-1); iEle>startVtr; iEle--)
      vtrT[iEle]=vtrT[iEle-1];
    vtrT[startVtr]=insertBits;
  }
  else if(lengthBit>0)
  {
    int left = sizeofTBit - lengthBit;
    int right = lengthBit;
    T leftMask=0;
    T rightMask=(T)ULONG_MAX;
    if (right!=sizeofTBit)
      leftMask = ((T)ULONG_MAX) << right;
    rightMask = ((T)ULONG_MAX) >> left;
    T lastT;
    for(int iEle=startVtr; iEle<(int)vtrT.size(); iEle++)
    {
      lastT=vtrT[iEle];
      vtrT[iEle]=(insertBits&rightMask)<<left | (lastT&leftMask)>>right;
      insertBits = lastT;
    }
  }
  return vtrT;
}

/*=========================================================*/
// FUNCTION: OffsetBitsVtr
//
//! offset bits in the vector, result=T>>offset
//
/*!
  \param vtrT        the vector
  \param offset      number of bits to offset
 */
// REVISION HISTORY
// 5 March 2007. Modified by Samson Jim
/*=========================================================*/
template <typename T>
vector<T> OffsetBitsVtr(vector<T> vtrT, uint32 offset)
{
  uint32 sizeofTBit = sizeof(T)*8;
  if (vtrT.size()*sizeofTBit<=offset)
  {
    vtrT.assign(vtrT.size(), 0x00);
  }
  if (offset==0)
    return vtrT;

  if (offset >= sizeofTBit)
  {
    uint32 numTOffset = offset/sizeofTBit;
    for(uint32 iEle=(uint32)(vtrT.size()-1); iEle>=numTOffset; iEle--)
      vtrT[iEle]=vtrT[iEle-numTOffset];
    for(uint32 iE=0; iE<numTOffset; iE++)
      vtrT[iE]=0;
    offset-=numTOffset*sizeofTBit;
  }

  if (offset!=0)
  {
    vtrT=InsertBitsVtr<T>(vtrT, 0x00, offset, 0);
  }
  return vtrT;
}

/*=========================================================*/
// FUNCTION: ShiftBitsVtr
//
//! shift bits in the vector from the front, result = T<<offset
//
/*!
  \param vtrT        the vector
  \param offset      number of bits to offset
 */
// REVISION HISTORY
// 2 Oct 2008. Modified by Samson Jim
/*=========================================================*/
template <typename T>
vector<T> ShiftBitsVtr(vector<T> vtrT, uint32 offset)
{
  uint32 sizeofTBit = sizeof(T)*8;
  uint32 numErase = offset/sizeofTBit;
  vtrT.erase(vtrT.begin(), vtrT.begin()+numErase);

  uint32 offsetBit = sizeofTBit-offset%sizeofTBit;
  if (offsetBit<sizeofTBit)
  {
    vtrT.push_back(T(0));
    vtrT=OffsetBitsVtr<T>(vtrT, offsetBit);
    vtrT.erase(vtrT.begin());
  }
  return vtrT;
}

/*=========================================================*/
// FUNCTION: AddWithCarry
//
//! add a to the least significant entry with carry
//
/*!
  \param vtrT        the vector
  \param val         a value to add to the vector
 */
// REVISION HISTORY
// 20 Oct 2008. Modified by Samson Jim
/*=========================================================*/
template <typename T>
vector<T> AddWithCarry(vector<T> vtrT, T val)
{
  T prevT = vtrT.back();
  vtrT.back() += val;

  //bring the carry bit to the next
  uint32 iT=(uint32)vtrT.size()-1;
  while(vtrT[iT]<prevT && iT>0)
  {
    iT--;
    prevT = vtrT[iT];
    vtrT[iT]++;
  }
  return vtrT;
}

}
#endif
