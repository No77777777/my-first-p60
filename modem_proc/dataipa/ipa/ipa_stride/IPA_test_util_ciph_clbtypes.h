#ifndef IPA_TEST_UTIL_CIPH_CLBTYPES_H
#define IPA_TEST_UTIL_CIPH_CLBTYPES_H

/*===================================================================
FILE: clbtypes.h

SERVICES:

DESCRIPTION:

PUBLIC CLASSES AND FUNCTIONS:

(c) COPYRIGHT 2000 Qualcomm QCT/Spinco Incorporated.
All Rights Reserved. Qualcomm QCT/Spinco Proprietary
===================================================================*/


//===================================================================
// INCLUDES AND PUBLIC DATA DECLARATIONS
//===================================================================

//#include <vector>
//#include <complex>
//using namespace std;

//-------------------------------------------------------------------
// Includes Files
//-------------------------------------------------------------------
#include "IPA_test_util_ciph_clbctypes.h"

//-------------------------------------------------------------------
// Type Declarations
//-------------------------------------------------------------------

// The basic types are defined in clbctypes.h to separate basic C types from
// types that require C++/STL

// complex numbers
/*typedef complex < double >       complexd;
typedef complex < float64 >      complex_float64;
typedef complex < int8 >         complex_int8;


// FIFO types
typedef complex_float64 CLBTYPES_CmplxSymType;
typedef vector <CLBTYPES_CmplxSymType> CLBTYPES_CmplxSymBufferType;

typedef float64 CLBTYPES_FloatSymType;
typedef vector <CLBTYPES_FloatSymType> CLBTYPES_FloatSymBufferType;

typedef int16 CLBTYPES_IntSymType;
typedef vector <CLBTYPES_IntSymType> CLBTYPES_IntSymBufferType;

typedef byte CLBTYPES_ByteSymType;
typedef vector <CLBTYPES_ByteSymType> CLBTYPES_ByteSymBufferType;
*/

namespace CLB{
  // Define enum type to specify modulation
  // Value of modulation type is bits per symbol - don't change this
  enum MOD_TYPE
  {
    BPSK  = 1,
    QPSK  = 2,
    QAM16 = 4,
    QAM64 = 6
  };

  enum StandardsType
  {LTE,
  WIMAX,
  WCDMA,
  CDMA2K};
};

#endif
