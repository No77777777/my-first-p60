#ifndef IPA_TEST_UTIL_CIPH_F8_BASE_H
#define IPA_TEST_UTIL_CIPH_F8_BASE_H

/*!
  \file
  - base class for all UMTS/LTE f8 confidetiality function

  - Revision History:
  - Date / Who / What
    - 2008-10-16 Samson Jim Initial version

  - Copyright (C) 2008 Qualcomm Technologies Inc., All rights reserved.
*/

#include "IPA_test_util_ciph_clbtypes.h"
#include <string>

using namespace std;
namespace qcrypto
{
class f8Base
{
public:
  f8Base(string name){myName = name;}
  virtual ~f8Base(){}

    /*=========================================================*/
  // FUNCTION: f8( uint8 *key, int count, int bearer, int dir, uint8 *data, int length )
  //
  //! perform f8 ciphering/deciphering function
  //
  /*!
    - Output data:
    -# Output bit stream after cipher/deciphering stroed in *data.\n
    -# keystream generated from Snow3G
    \param *key    128 bit, Confidentiality Key
    \param count  32-bit, Count, Frame dependent input
    \param bearer 5-bit, Bearer identity (in the LSB side)
    \param dir    1 bit, direction of transmission
    \param *data   length number of bits, input bit stream.(IBS)
    \param length 16 bit Length, i.e., the number of bits to be encrypted or decrypted.
    \param *keyStrm an array of keyStrm
   */
  // REVISION HISTORY
  // 5 March 2007. Created by Samson Jim
  /*=========================================================*/
  virtual void f8( uint8 *key, int32 count, int32 bearer, int32 dir, uint8 *data, int32 length, uint8 *keyStrm, bool willCont )=0;

  string GetName(){return myName;}

private:
  string myName;
};
}
#endif
