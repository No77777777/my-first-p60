/*======================= COPYRIGHT NOTICE ==================================*]
Copyright (c) 2018-2019 Qualcomm Technologies, Inc.
All Rights Reserved.  
Confidential and Proprietary - Qualcomm Technologies, Inc.                                  *]
[*===========================================================================*]
[*****************************************************************************]
[* FILE NAME:   comdef.h                    TYPE: C-header file              *]
[* DESCRIPTION: Contains the typedefs of various data types that should be   *]
[*  used instead of the actual data types so as to have common sizes across  *] 
[*      platforms.                                                           *]   
[*    Date              Comments                                  Programmer *]
[*      ------          --------                                  ---------- *]
[*    6/6/2005:    Initial revision,                        DSK              *]
[*****************************************************************************/

#ifndef _COMDEF_H_      
#define _COMDEF_H_
      
#ifdef TRUE
#undef TRUE
#endif

#ifdef FALSE
#undef FALSE
#endif
      
#define TRUE   1                            /* Boolean true value.          */
#define FALSE  0                            /* Boolean false value.         */

#define  ON   1                             /* On value.                    */
#define  OFF  0                             /* Off value.                   */

#ifdef _lint
  #define NULL 0
#endif

#ifndef NULL
  #define NULL  0
#endif
      
/* -------------------------------------------------------------------------
** Standard Types
** -------------------------------------------------------------------------*/

typedef  unsigned char      boolean;        /* Boolean value type.          */

typedef  unsigned short     uint16;         /* Unsigned 16 bit value        */
typedef  unsigned char      uint8;          /* Unsigned 8  bit value        */

typedef  signed short       int16;          /* Signed 16 bit value          */
typedef  signed char        int8;           /* Signed 8  bit value          */

#ifndef uint32
#ifndef _UINT32_DEFINED
#ifdef __linux__
typedef  unsigned int  uint32;         /* Unsigned 32 bit value        */
#else
typedef  unsigned long int  uint32;         /* Unsigned 32 bit value        */
#endif
#define _UINT32_DEFINED          /* Unsigned 32-bit value flag. */
#endif
#endif

#ifndef int32
#ifndef _INT32_DEFINED
#ifdef __linux__
typedef  signed int    int32;          /* Signed 32 bit value          */
#else
typedef  signed long int    int32;          /* Signed 32 bit value          */
#endif
#define _INT32_DEFINED          /* Unsigned 32-bit value flag. */
#endif
#endif

#ifndef WIN32
typedef  signed long long   int64;          /* Signed 64 bit value          */
typedef  unsigned long long uint64;         /* Unsigned 64 bit value        */
typedef  signed long long   int40;          /* Signed 40 bit value          */
#else
typedef  signed __int64   int64;            /* Signed 64 bit value          */
typedef  unsigned __int64 uint64;           /* Unsigned 64 bit value        */
typedef  signed __int64   int40;            /* Signed 40 bit value          */
#endif

/********** Macro defnitions that are commonly used.      * *****************/
/*===========================================================================

MACRO MAX
MACRO MIN

DESCRIPTION
  Evaluate the maximum/minimum of 2 specified arguments.

PARAMETERS
  x     parameter to compare to 'y'
  y     parameter to compare to 'x'

DEPENDENCIES
  'x' and 'y' are referenced multiple times, and should remain the same
  value each time they are evaluated.

RETURN VALUE
  MAX   greater of 'x' and 'y'
  MIN   lesser of 'x' and 'y'

SIDE EFFECTS
  None

============================================================================*/
#ifndef MAX
   #define  MAX( x, y ) ( ((x) > (y)) ? (x) : (y) )
#endif

#ifndef MIN
   #define  MIN( x, y ) ( ((x) < (y)) ? (x) : (y) )
#endif

#endif /* _COMDEF_H_ closure, END of file .                                 */
      
