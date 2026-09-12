

#ifndef IPA_TEST_UTIL_CIPH_CLBCTYPES_H
#define IPA_TEST_UTIL_CIPH_CLBCTYPES_H
#include <comdef.h>
//-------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------
#define TRUE 1
#define FALSE 0


//-------------------------------------------------------------------
// Type Declarations
//-------------------------------------------------------------------
// integer

#if defined(WIN32) || defined(WIN64)
typedef signed _int64    long64;
#else
typedef signed long long long64;
#endif

typedef signed char      int8;
typedef signed short     int16;
//typedef signed int       int32;
#if defined(WIN32) || defined(WIN64)
typedef signed _int64    int64;
#else
typedef signed long long int64;
#endif

//typedef unsigned int       uint;
//typedef unsigned char      uint8;
//typedef unsigned short     uint16;
//typedef unsigned int       uint32;
#if defined(WIN32) || defined(WIN64)
//typedef unsigned _int64    uint64;
#else
//typedef unsigned long long uint64;
#endif


// The following types are deprecated and should not
// be used anymore

// word
typedef unsigned char       byte;     // deprecated: use uint8
typedef unsigned short      word16;   // deprecated: use uint16
typedef unsigned int        word32;   // deprecated: use uint32
#if defined(WIN32) || defined(WIN64)
typedef signed _int64       word64;   // deprecated: use uint64
#else
typedef signed long long    word64;   // deprecated: use uint64
#endif

// float
typedef float  float32;   // deprecated: use float
typedef double float64;   // deprecated: use double


#endif
