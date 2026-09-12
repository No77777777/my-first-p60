#ifndef IPA_TEST_UTIL_CIPH_SNOW3G_F8_H
#define IPA_TEST_UTIL_CIPH_SNOW3G_F8_H

#include <iostream>
#include "IPA_test_util_ciph_clbctypes.h"
#include "IPA_test_util_ciph_snow3G.h"
#include "IPA_test_util_ciph_f8Base.h"
#include "IPA_test_util_ciph_qcryptoUtils.h"

namespace qcrypto
{
//! Snow3G f8 confidentiality algorithm TS35.215
class Snow3Gf8 : public f8Base {

public:

  /*=======
  METHODS
  =======*/

  /*=========================================================*/
  // FUNCTION: Snow3Gf8() Constructor
  //
  //! Constructor for the Snow3Gf8 class
  //
  // REVISION HISTORY
  // 5 March 2007. Created by Samson Jim
  /*=========================================================*/
  Snow3Gf8();

  /*=========================================================*/
  // FUNCTION: Snow3Gf8() Destructor
  //
  //! Destructor for the UEA2Engine class
  //
  // REVISION HISTORY
  // 5 March 2007. Created by Samson Jim
  /*=========================================================*/
  ~Snow3Gf8(){};

  /*=========================================================*/
  // FUNCTION: f8
  //
  //! perform f8 de/ciphering function
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
    \param cont continue from the last de/ciphering
   */
  // REVISION HISTORY
  // 5 March 2007. Created by Samson Jim
  /*=========================================================*/
  virtual void f8(uint8* key, int32 count, int32 bearer, int32 dir, uint8 *data, int32 length, uint8 *keystrm, bool willCont);

private:

  /*=========
  VARIABLES
  =========*/

  //! Snow 3G keystream generator
  Snow3G _snow3G;

  bool _willCont;           //!< will continue the ciphering in the next call
  uint32   _lastKS;         //!< last 32-bit keystream
  int32  _lastBitLength;    //!< the last outstanding bit ciphered
}; //Ends [class Snow3Gf8]
}
#endif // Ends [ifndef _SNOW3G_F8_H_]



