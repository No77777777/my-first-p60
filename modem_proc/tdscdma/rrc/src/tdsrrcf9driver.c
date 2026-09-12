/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

               M S M 5 2 0 0  F 9  ( I N T E G R I T Y   E N G I N E ) 
                              D R I V E R


GENERAL DESCRIPTION

  This is a driver for the F9 Engine that implements the Integrity
  Protection algorithm for TDS.

EXTERNALIZED FUNCTIONS

  
 
                    
INITIALIZATION AND SEQUENCING REQUIREMENTS
  


  Copyright (c) 2010-2012 by Qualcomm Technologies, Incorporated.  
  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcf9driver.c#1 $    $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $
   
when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
07/19/13   eg     Mainlined FEATURE_WCDMA_A2_POWER_COLLAPSE 
03/28/12   hx     Fix KW compile warnings
03/10/12   hx     Merged to VU_MODEM_WCDMA_NIKEL_3_0_MHCI.00.00.00.00.08.04
03/10/11   hx     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "tdscdma_variation.h"
#include "customer.h"
#include "tdsrrcmm_v.h"
#include "msm.h"
#include "msg.h"
#include "err.h"
#include "string.h"
#include "tdsrrcf9driver.h"
#include "tdsrrccommon.h"
#include "bit.h"
#include "stdio.h"
#include "math.h"


#include "tdsrrcsmc.h"

//number of bits that can be written to R9 RAM at one shot.
#define TDSRRC_SDU_WRITE_LEN_BITS         2624
#ifdef FEATURE_TDSCDMA_SNOW3G_SECURITY
#endif

/*NV to control the different integrity options*/
extern uint8 tdsrrc_integrity_opt_nv;

/*===========================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains local definitions for constants, macros, types,
  variables and other items needed by this module.

===========================================================================*/

/* UNIONS: used to remove "endian" issues */
typedef union {
  uint32 b32;
  uint16 b16[2];
  uint8 b8[4];
} TDSSW_DWORD;

typedef union {
  uint16 b16;
  uint8 b8[2];
} TDSSW_WORD;

/* 16 bit rotate left */
#define TDSROL16(a,b) (uint16)((a<<b)|(a>>(16-b)))

/* GLOBALS: the subkey arrays */
static uint16 KLi1[8], KLi2[8];
static uint16 KOi1[8], KOi2[8], KOi3[8];
static uint16 KIi1[8], KIi2[8], KIi3[8];

int TdsFIVal;

uint8 tdsf9_cs_key[16];
uint8 tdsf9_ps_key[16];

uint32 tdsf9_fresh;

/*========================================================================
  FUNCTION: static uint16 FI
  
  DESCRIPTION: 
    The FI function.  It includes the S7 and S9 tables. Transforms a 16-bit 
    value.

  DEPENDENCIES
    None.

  RETURN VALUE
    static uint16

  ========================================================================*/
static uint16 FI ( uint16 in, uint16 subkey )
{

  uint16 nine, seven;
  static uint16 S7[] = {
    54, 50, 62, 56, 22, 34, 94, 96, 38, 6, 63, 93, 2, 18, 123, 33, 
    55, 113, 39, 114, 21, 67, 65, 12, 47, 73, 46, 27, 25, 111, 124, 81, 
    53, 9, 121, 79, 52, 60, 58, 48, 101, 127, 40, 120, 104, 70, 71, 43, 
    20, 122, 72, 61, 23, 109, 13, 100, 77, 1, 16, 7, 82, 10, 105, 98, 
    117, 116, 76, 11, 89, 106, 0, 125, 118, 99, 86, 69, 30, 57, 126, 87, 
    112, 51, 17, 5, 95, 14, 90, 84, 91, 8, 35, 103, 32, 97, 28, 66, 
    102, 31, 26, 45, 75, 4, 85, 92, 37, 74, 80, 49, 68, 29, 115, 44, 
    64, 107, 108, 24, 110, 83, 36, 78, 42, 19, 15, 41, 88, 119, 59, 3};

  static uint16 S9[] = {
    167, 239, 161, 379, 391, 334, 9, 338, 38, 226, 48, 358, 452, 385, 90, 397, 
    183, 253, 147, 331, 415, 340, 51, 362, 306, 500, 262, 82, 216, 159, 356, 177,
    175, 241, 489, 37, 206, 17, 0, 333, 44, 254, 378, 58, 143, 220, 81, 400,
    95, 3, 315, 245, 54, 235, 218, 405, 472, 264, 172, 494, 371, 290, 399, 76,
    165, 197, 395, 121, 257, 480, 423, 212, 240, 28, 462, 176, 406, 507, 288, 223,
    501, 407, 249, 265, 89, 186, 221, 428, 164, 74, 440, 196, 458, 421, 350, 163,
    232, 158, 134, 354, 13, 250, 491, 142, 191, 69, 193, 425, 152, 227, 366, 135,
    344, 300, 276, 242, 437, 320, 113, 278, 11, 243, 87, 317, 36, 93, 496, 27,
    487, 446, 482, 41, 68, 156, 457, 131, 326, 403, 339, 20, 39, 115, 442, 124,
    475, 384, 508, 53, 112, 170, 479, 151, 126, 169, 73, 268, 279, 321, 168, 364,
    363, 292, 46, 499, 393, 327, 324, 24, 456, 267, 157, 460, 488, 426, 309, 229,
    439, 506, 208, 271, 349, 401, 434, 236, 16, 209, 359, 52, 56, 120, 199, 277,
    465, 416, 252, 287, 246, 6, 83, 305, 420, 345, 153, 502, 65, 61, 244, 282,
    173, 222, 418, 67, 386, 368, 261, 101, 476, 291, 195, 430, 49, 79, 166, 330,
    280, 383, 373, 128, 382, 408, 155, 495, 367, 388, 274, 107, 459, 417, 62, 454,
    132, 225, 203, 316, 234, 14, 301, 91, 503, 286, 424, 211, 347, 307, 140, 374,
    35, 103, 125, 427, 19, 214, 453, 146, 498, 314, 444, 230, 256, 329, 198, 285,
    50, 116, 78, 410, 10, 205, 510, 171, 231, 45, 139, 467, 29, 86, 505, 32,
    72, 26, 342, 150, 313, 490, 431, 238, 411, 325, 149, 473, 40, 119, 174, 355,
    185, 233, 389, 71, 448, 273, 372, 55, 110, 178, 322, 12, 469, 392, 369, 190,
    1, 109, 375, 137, 181, 88, 75, 308, 260, 484, 98, 272, 370, 275, 412, 111,
    336, 318, 4, 504, 492, 259, 304, 77, 337, 435, 21, 357, 303, 332, 483, 18,
    47, 85, 25, 497, 474, 289, 100, 269, 296, 478, 270, 106, 31, 104, 433, 84,
    414, 486, 394, 96, 99, 154, 511, 148, 413, 361, 409, 255, 162, 215, 302, 201,
    266, 351, 343, 144, 441, 365, 108, 298, 251, 34, 182, 509, 138, 210, 335, 133,
    311, 352, 328, 141, 396, 346, 123, 319, 450, 281, 429, 228, 443, 481, 92, 404,
    485, 422, 248, 297, 23, 213, 130, 466, 22, 217, 283, 70, 294, 360, 419, 127,
    312, 377, 7, 468, 194, 2, 117, 295, 463, 258, 224, 447, 247, 187, 80, 398,
    284, 353, 105, 390, 299, 471, 470, 184, 57, 200, 348, 63, 204, 188, 33, 451,
    97, 30, 310, 219, 94, 160, 129, 493, 64, 179, 263, 102, 189, 207, 114, 402,
    438, 477, 387, 122, 192, 42, 381, 5, 145, 118, 180, 449, 293, 323, 136, 380,
    43, 66, 60, 455, 341, 445, 202, 432, 8, 237, 15, 376, 436, 464, 59, 461};

  /* The 16 bit input is split into two unequal halves, 9 bits and 7 bits, as in
     the subkey. */

  nine = (uint16)(in >> 7);
  seven = (uint16)(in & 0x7F);

  /*Now run the various operations. */
  nine = (uint16)(S9[nine] ^ seven);
  seven = (uint16)(S7[seven] ^ (nine & 0x7F) );
  
  seven ^= (subkey>>9);
  nine ^= (subkey & 0x1FF);
  
  nine = (uint16) (S9[nine] ^ seven);
  seven = (uint16) (S7[seven] ^ (nine & 0x7F));
  
  in = (uint16) ((seven << 9) + nine);

  return (in);
  
} /* FI */

/*========================================================================
  FUNCTION: static uint32 FO
  
  DESCRIPTION:
    The FO() function.  Transforms a 32-bit value.  Uses <index> to identify
    the appropriate subkeys to use.

  DEPENDENCIES
    None.

  RETURN VALUE 
    static u32
  
  ========================================================================*/
static uint32 FO( uint32 in, int index )
{

  uint16 left, right;

  /* Split the input into two 16-bit words */
  left = (uint16) (in >> 16);
  right = (uint16) in;
  
  /* now apply the same basic transformation three times */
  left ^= KOi1[index];
  TdsFIVal = 1;
  left = FI( left, KIi1[index] );
  left ^= right;

  right ^= KOi2[index];
  ++TdsFIVal;
  right = FI( right, KIi2[index] );
  right ^= left;

  left ^= KOi3[index];
  ++TdsFIVal;
  left = FI( left, KIi3[index] );
  left ^= right;
  

  in = (right << 16) + left;

  return (in);

} /* FO */
  
/*========================================================================
  FUNCTION: static u32 FL
  
  DESCRIPTION:
  The FL() function.  Transforms a 32-bit value.  Uses <index> to identify
  the appropriate subkeys to use.

  DEPENDENCIES
    None.

  RETURN VALUE
    static u32
  
  ========================================================================*/
static uint32 FL ( uint32 in, int index )
{
  
  uint16 l, r, a, b;

  /* Split out the left and right halves */
  l = (uint16) (in >> 16 );
  r = (uint16) (in);
  
  /* Do the FL() operations */
  a = (uint16) (l & KLi1[index]);
  r ^= TDSROL16(a,1);
  
  b = (uint16) (r | KLi2[index]);
  l ^= TDSROL16(b,1);
  
  /* Put the two halves back together */
  in = (l<<16) + r;

  return( in );

} /*  FL */

/*========================================================================

  FUNCTION RRCF9_KASUMI

  DESCRIPTION
    This function is implementation of KASUMI, the core algorithm for the
    3GPP Confidentiality and Integrity algorithms.
 
    This has been coded for clarity, not necessarily for efficiency.
 
    This will compile and run correctly on both Intel (little endian)
    and Sparc (big endian) machines. (Compilers used supported 32-bit ints

  DEPENDENCIES
    None.

  RETURN VALUE
    None
  ========================================================================*/
void tdsrrcf9_kasumi( uint8* data)
{

  /* Start by gettin the data into two 32-bit words (endian correct) */
  TDSSW_DWORD *d;
  uint32 temp,left, right;
  int n;
  
  d = (TDSSW_DWORD*) data;
  left = (d[0].b8[0]<<24) +  (d[0].b8[1]<<16) + (d[0].b8[2]<<8) + (d[0].b8[3]);
  right = (d[1].b8[0]<<24) + (d[1].b8[1]<<16) + (d[1].b8[2]<<8) + (d[1].b8[3]);

  n = 0;

  do
  { 

    temp = FL( left, n );

    temp = FO( temp, n );
    ++n;
    right ^= temp;

    temp = FO( right, n );
    temp = FL( temp, n );
    ++n;
    left ^= temp;

  } while (n <= 7);

  /* Return the correct endian result. */
  d[0].b8[0] = (uint8)( left >> 24 );
  d[0].b8[1] = (uint8)( left >> 16 );
  d[0].b8[2] = (uint8)( left >> 8 );
  d[0].b8[3] = (uint8)( left );
  
  d[1].b8[0] = (uint8)( right >> 24 );
  d[1].b8[1] = (uint8)( right >> 16 );
  d[1].b8[2] = (uint8)( right >> 8 );
  d[1].b8[3] = (uint8)( right );
  
} /* tdsrrcf9_kasumi */

/*========================================================================

  FUNCTION RRCF9_KEY_SCHEDULE

  DESCRIPTION

    This function builds the key schedule.  Most "key" operations use 16-bit
    subkeys so  build uint16-sized arrays that are "endian" correct.

  DEPENDENCIES
    None.

  RETURN VALUE
    None
 ========================================================================*/
void tdsrrcf9_key_schedule( uint8* k )
{

  static uint16 C[] = { 0x0123, 0x4567, 0x89AB, 0xCDEF, 
                        0xFEDC, 0xBA98, 0x7654, 0x3210};
  uint16 key[8], kprime[8];
  TDSSW_WORD *k16;
  int n;

  /* Start by ensuring the subkeys are endian correct on a 16-bit basis */
  k16 = (TDSSW_WORD *)k;
  for ( n = 0; n < 8; ++n )
  {

    key[n] = (uint16) ((k16[n].b8[0]<<8) + (k16[n].b8[1]));
    
  } /* Ends [for ( n = 0; n < 8; ++n )] */

  /* Now build the K'[] keys */
  for (n = 0; n < 8; ++n) 
  {

    kprime[n] = (uint16) (key[n] ^ C[n]);
    
  } /* Ends [for (n = 0; n < 8; ++n)] */

  /*Finally construct the various sub keys */
  for (n =0; n < 8; ++n )
  {

    KLi1[n] = TDSROL16( key[n], 1);
    KLi2[n] = kprime[ (n+2) & 0x7 ];
    KOi1[n] = TDSROL16( key[(n+1) & 0x7], 5);
    KOi2[n] = TDSROL16( key[(n+5) & 0x7], 8);
    KOi3[n] = TDSROL16( key[(n+6) & 0x7], 13);
    KIi1[n] = kprime[ (n+4) & 0x7 ];
    KIi2[n] = kprime[ (n+3) & 0x7 ];
    KIi3[n] = kprime[ (n+7) & 0x7 ];

  } /* Ends [for (n =0; n < 8; ++n )] */

} /* tdsrrcf9_key_schedule */


/*========================================================================

  FUNCTION RRCF9_PERFORM_F9_ALGO

  DESCRIPTION
    This function is implementation of f9, the 3GPP Confidentiality algorithm.
      
    This has been coded for clarity, not necessarily for efficiency.

    This will compile and run correctly on both Intel (little endian)
    and Sparc (big endian) machines. (Compilers used supported 32-bit ints)

  DEPENDENCIES

    None.

  RETURN VALUE

    None
========================================================================*/

uint8* tdsrrcf9_perform_f9_algo
( 
  uint8* key,
  int count,
  int fresh,
  int dir,
  uint8* data,
  int length
)
{

  int numExtraZeros;

  /*var for count */
  int k,n;

  /* Holds the CRC chained data */
  tdsrrc_register64 A;     

  /* Holds the XOR of all the Kasumi outputs */
  tdsrrc_register64 B;     

  uint8 ModKey[16];

  int tempLength, tempVal;

  /* Static memory for the result */
  static uint8 mac_i[4];
  int i;

  uint8 FinalBit[8] = {0x80, 0x40, 0x20, 0x10, 8,4,2,1};

  /*Following lines of code changed to remove ceil and non-integer division.*/
  /* numExtraZeros = (int) ceil ( ( (double)length + 2. )/ 8. ) % 8; */

  tempLength = length + 2;

  tempVal = tempLength / 8;

  if((tempLength % 8) > 0)
  {
     tempVal = tempVal+1;
  }

  numExtraZeros = tempVal % 8;

  if ( numExtraZeros )
  {
    
    numExtraZeros = 8-numExtraZeros;

  } /* Ends [if ( numExtraZeros )] */

  /* Start by initializing the block cipher */
  tdsrrcf9_key_schedule( key );

  /* Next initialize the MAC chain.  Make sure we have the data in the right byte
     order.
     <A> holds our chaining value...
     <B> is the running XOR of all KASUMI o/ps */

  for( n = 0; n < 4; ++n )
  {

    A.b8[n] = (uint8)( count >> (24 - (n*8)) );
    A.b8[n+4] = (uint8)( fresh >> (24 - (n*8)) );

  } /* Ends [for( n = 0; n < 4; ++n )] */

  tdsrrcf9_kasumi( A.b8 );
  
  B.b32[0] = A.b32[0];
  B.b32[1] = A.b32[1];

  /* Now run the blocks until we reach the last block */
  while ( length >= 64 )
  {
    
    for ( k = 0; k < 8 ; ++k )
    {

      /*   A.b8[k] ^= *data++; */
      A.b8[k] ^= *data;
      data++;
    } /* Ends [for ( k = 0; k < 8 ; ++k )] */

    tdsrrcf9_kasumi( A.b8 );

    length -= 64;

    /* Running XOR across the block outputs */
    B.b32[0] ^= A.b32[0];
    B.b32[1] ^= A.b32[1];

    
  } /* Ends [while ( length >= 64 )] */
  
  /* Process whole bytes in the last block */
  n = 0;
  while (length >= 8)
  {
    
    /*  A.b8[n++] ^= *data++; */
    A.b8[n++] ^= *data;
    length -= 8;
    data++;

  } /* Ends [while (length >= 8)] */

  /* Now add the direction bit to the input bit stream.
     If length (which holds the # of data bits in the
     last byte) is non-zero we add it in, otehrwise, it
     has to start a new byte. */

  if( length )
  {

    i = *data;

    if (dir)
    {

      i |= FinalBit[length];

    } /* Ends [if (dir)] */

  } /* Ends [if( length )] */
  else
  {

    i = dir ? 0x80 : 0;

  } /* Ends [else] */

  A.b8[n++] ^= (uint8)i;

  /*Now add in the final '1' bits.  The problem here 
    is if the message length happens to be n*64-1.
    If so we need to process this block and then
    create a new input block of 0x8000000000000000.*/

  /* ... then we've filled the block */
  if ( (length == 7) && (n == 8) )
  {
    
    tdsrrcf9_kasumi( A.b8 );
    
    /* Running XOR across the block outputs */
    B.b32[0] ^= A.b32[0];
    B.b32[1] ^= A.b32[1];

    /* Toggle first bit */
    A.b8[0] ^= 0x80;
    i=0x80;
    n = 1;

  } /* Ends [if ( (length == 7) && (n == 8) )] */
  else 
  {

    /* We finished off the last byte, so start a new one...*/
    if (length == 1)
    {

      i = 0x80;

    } /* Ends [if (length == 1)] */
    /* We added a new byte of "dir" */
    else if ( length == 0)
    {
      
      A.b8[n-1] ^= 0x40;
      i |= 0x40;
    } /* Ends [else if ( length == 0)] */
    else 
    {
      if((length >=0) && (length < 7))
      {    
        A.b8[n-1] ^= FinalBit[ length+1 ];
        i |= FinalBit[ length+1 ];
	  }


    } /* Ends [else] */

  } /* Ends [else] */
 
  tdsrrcf9_kasumi( A.b8 );

  /* Running XOR across the block outputs */
  B.b32[0] ^= A.b32[0];
  B.b32[1] ^= A.b32[1];


  /* Final step is to KASUMI what we have using the key XORd with 0xAAA...*/
  for (n = 0; n < 16; ++n)
  {

    ModKey[n] = (uint8)*key++ ^ 0xAA;    
    
  } /* Ends [for (n = 0; n < 16; ++n)] */
  tdsrrcf9_key_schedule( ModKey );

  tdsrrcf9_kasumi( B.b8 );
  
  /* We return the left-most 32-bits of the result */
  for (k = 0; k < 4; ++k )
  {
    
    mac_i[k] = B.b8[k];
    
  } /* Ends [for (k = 0; k < 4; ++k )] */

  return ( mac_i );

} /* tdsrrcf9_perform_f9_algo */

/*===========================================================================

  FUNCTION tdsrrcf9_init_fresh_per_connection_sw

  DESCRIPTION

    This function performs initialization for the F9 device
    for each RRC connection. Each time an RRC connection
    is eastablished, once the input parameters are availabe
    this function should be called.

    The following are initialized by this function -
  
    FRESH - FRESH is a 32 bit value unique to a UE. It's given
    by the UTRAN in the security mode command.
   
   
  DEPENDENCIES

    None.

  RETURN VALUE

    None

===========================================================================*/
void tdsrrcf9_init_fresh_per_connection_sw
( 
  uint32 fresh_value      /* Value of Fresh */
)
{
  tdsf9_fresh = fresh_value;

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Fresh Value: 0x%x",tdsf9_fresh);
}
/*===========================================================================

  FUNCTION tdsrrcf9_init_key_values_sw

  DESCRIPTION

    This function performs initialization for the F9 device
    for the foll. parameters

    The following are initialized by this function -
  
    Integrity Key - 128 bit key used by the F9 engine. The
    key is different for CS connections and PS connections.
  
  DEPENDENCIES

    None.

  RETURN VALUE

    None

===========================================================================*/
void tdsrrcf9_init_key_values_sw
( 
  byte *cs_key,         /* Integrity Key value for PS domain */
  byte *ps_key          /* Integrity Key value for CS domain */
)
{
  uint32 count;

  for(count=0; count < (RRC_MAX_INTEGRITY_KEY_LENGTH); count++)
  {
    tdsf9_cs_key[count] = cs_key[count];
  }

  for(count=0; count < (RRC_MAX_INTEGRITY_KEY_LENGTH); count++)
  {
    tdsf9_ps_key[count] = ps_key[count];
  }
}
#ifdef FEATURE_TDSCDMA_SNOW3G_SECURITY
/*===========================================================================

  FUNCTION tdsrrcf9_generate_mac_for_message_using_snow3g_sw

  DESCRIPTION

    This function passes the encoded RRC message and the
  additional parameters that are unique to the message 
  to the integrity engine. It returns the generated
  MAC-I value.
  
  This function is to be used for both downlink and 
  uplink messages.
  

  DEPENDENCIES

    None.

  RETURN VALUE

    A uint32 value that represents the MAC-I (or XMAC-I) for
  this message.

===========================================================================*/
uint32 tdsrrcf9_generate_mac_for_message_using_snow3g_sw
( 
  uint32 count_i,                           /* COUNT-I value for the RB */
  tdsrrc_integrity_direction_type dir,         /* Direction - uplink or downlink message */
  uint8 *msg,                               /* Pointer to the encoded message.    */
  uint32 msg_length,                        /* Length of the message in bits */
  rrc_cn_domain_identity_e_type  cn_domain,  /* Key for which domain_id to be used */
  uecomdef_umts_integrity_protection_algorithm_e_type integrity_algo /*integrity protection algorithm*/
)
{
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"You are here");
  return 0;
}
#endif

/*===========================================================================

  FUNCTION tdsrrcf9_generate_mac_for_message_sw

  DESCRIPTION

    This function passes the encoded RRC message and the
  additional parameters that are unique to the message 
  to the integrity engine. It returns the generated
  MAC-I value.
  
  This function is to be used for both downlink and 
  uplink messages.
  

  DEPENDENCIES

    None.

  RETURN VALUE

    A uint32 value that represents the MAC-I (or XMAC-I) for
  this message.

===========================================================================*/
uint32 tdsrrcf9_generate_mac_for_message_sw
( 
  uint32 count_i,                           /* COUNT-I value for the RB */
  tdsrrc_integrity_direction_type dir,         /* Direction - uplink or downlink message */
  uint8 *msg,                               /* Pointer to the encoded message.    */
  uint32 msg_length,                        /* Length of the message in bits */
  rrc_cn_domain_identity_e_type  cn_domain,  /* Key for which domain_id to be used */
  uecomdef_umts_integrity_protection_algorithm_e_type integrity_algo /*integrity protection algorithm*/
)
{

  uint8  *macID;
  uint32 mac_id = 0;

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"CN Doamin = %d,Integrity Algo = %x[0=Invalid,1=UIA1,2=UIA2] in rrcsmc_int_ciph_info",cn_domain,integrity_algo);

  //check if cs or ps domain and send the appropriate keys
  if(cn_domain == RRC_CS_DOMAIN_CN_ID)
  {
    macID = tdsrrcf9_perform_f9_algo( tdsf9_cs_key, count_i, tdsf9_fresh, dir, msg, msg_length);
  }
  else
  {
    macID = tdsrrcf9_perform_f9_algo( tdsf9_ps_key, count_i, tdsf9_fresh, dir, msg, msg_length);
  }


  //clear mac_id;
  mac_id &=0x0;

  //or and shift the contents of byte* into an uint32 
  mac_id |= macID[0];
  mac_id <<= 8;

  mac_id |= macID[1];
  mac_id <<= 8;


  mac_id |= macID[2];
  mac_id <<= 8;


  //do not left shift after last one
  mac_id |= macID[3];
  
  TDSRRC_MSG3(MSG_LEGACY_HIGH,"Calc MAC: 0x%x DIR %d Count_I 0x%x", mac_id, dir, count_i);

  return mac_id;

}

/*===========================================================================

  FUNCTION tdsrrcf9_init_fresh_per_connection

  DESCRIPTION

    This function performs initialization for the F9 device
    for each RRC connection. Each time an RRC connection
    is eastablished, once the input parameters are availabe
    this function should be called.

    The following are initialized by this function -
  
    FRESH - FRESH is a 32 bit value unique to a UE. It's given
    by the UTRAN in the security mode command.
   
   
  DEPENDENCIES

    None.

  RETURN VALUE

    None

===========================================================================*/
void tdsrrcf9_init_fresh_per_connection
( 
  uint32 fresh_value      /* Value of Fresh */
)
{
  if ((tdsrrc_integrity_opt_nv == 0) || (tdsrrc_integrity_opt_nv == 1)) /* 0 or 1 is for sw integrity*/
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Doing SW integrity");
    tdsrrcf9_init_fresh_per_connection_sw(fresh_value);
    return;
  }

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Doing HW integrity");
  /* Write the fresh value */
}/*rrcf9_init_fresh_per_connection*/
/*===========================================================================

  FUNCTION tdsrrcf9_init_key_values 

  DESCRIPTION

    This function performs initialization for the F9 device
    for each RRC connection. Each time an RRC connection
    is eastablished, once the input parameters are availabe
    this function should be called.

    The following are initialized by this function -
  
    Integrity Key - 128 bit key used by the F9 engine. The
    key is different for CS connections and PS connections.
  
   
  DEPENDENCIES

    None.

  RETURN VALUE

    None

===========================================================================*/
void tdsrrcf9_init_key_values
( 
  byte *cs_key,         /* Integrity Key value for PS domain */
  byte *ps_key          /* Integrity Key value for CS domain */
)
{
  uint32 count;          /*local var to store count*/
  uint32 cs_int_key[4];  /*local int array to store cs key*/
  uint32 ps_int_key[4];  /*local int array to store ps key*/
  if ((tdsrrc_integrity_opt_nv == 0) || (tdsrrc_integrity_opt_nv == 1)) /* 0 or 1 is for sw integrity*/
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Doing SW integrity");
    tdsrrcf9_init_key_values_sw(cs_key,ps_key);
    return;
  }

  /*we receive a PS and CS IK as a pointer to a byte stream.  This will have
    to be copied to an Integer array as we have to put four bytes at a time
    in MSM_OUT.  The foll. lines of code copy the contents of cs_key and ps_key
    into 2 integer arrays of size 128 bits.  Cannot cast the byte stream to an
    integer stream as big endian gets converted to little endian*/

  for(count=0; count < (RRC_MAX_INTEGRITY_KEY_LENGTH /4); count++)
  {
    cs_int_key[count] = b_unpackd(&(cs_key[count*4]), 0, 32 );
  }

  for(count=0; count < (RRC_MAX_INTEGRITY_KEY_LENGTH /4); count++)
  {
    ps_int_key[count] = b_unpackd( &(ps_key[count*4]), 0, 32 );

  }
 
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"CS keys 0x%x 0x%x",cs_int_key[0],cs_int_key[1]);
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"CS Keys 0x%x 0x%x",cs_int_key[2],cs_int_key[3]);
 
 
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"PS keys 0x%x 0x%x",ps_int_key[0],ps_int_key[1]);
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"PS Keys 0x%x 0x%x",ps_int_key[2],ps_int_key[3]);
}/*tdsrrcf9_init_key_values*/

#ifdef FEATURE_TDSCDMA_SNOW3G_SECURITY
/*===========================================================================

  FUNCTION RRCF9_GENERATE_MAC_FOR_MESSAGE

  DESCRIPTION

    This function passes the encoded RRC message and the
  additional parameters that are unique to the message 
  to the integrity engine. It returns the generated
  MAC-I value.
  
  This function is to be used for both downlink and 
  uplink messages.
  

  DEPENDENCIES

    None.

  RETURN VALUE

    A uint32 value that represents the MAC-I (or XMAC-I) for
  this message.

===========================================================================*/
uint32 tdsrrcf9_generate_mac_for_message_using_snow3g
( 
  uint32 count_i,                           /* COUNT-I value for the RB */
  tdsrrc_integrity_direction_type dir,         /* Direction - uplink or downlink message */
  uint8 *msg,                               /* Pointer to the encoded message.    */
  uint32 msg_length,                        /* Length of the message in bits */
  rrc_cn_domain_identity_e_type  cn_domain,  /* Key for which domain_id to be used */
  uecomdef_umts_integrity_protection_algorithm_e_type integrity_algo /*integrity protection algorithm*/
)
{
  uint32 local_sdu_len_bytes;              /* Local SDU length in bytes            */
  uint8 *local_sdu_ptr;                    /* Local pointer to store SDU */
  uint32 *local_sdu_uint32_ptr;            /* Local pointer to store SDU in unit32 */
  uint32 f9statusval=0;                      /* Status value */
  uint32 f9_control_val=0;                 /* Value written to F9 control reg      */
  uint32 local_sdu_len;                    /* Local SDU length divisible next by 64 */
  uint32 maci = 0 ;                        /* Local variable for MACI */
  uint32 count,i;                          /* Local counters */
  uint32 num_writes_in_continuous_mode=0;  /* Local var to store num of writes */
  uint32 sdu_len_last_write_len=0;
  volatile uint32 f9_done_status = 0;  /* Local variable to store interrupt status */
  //uint32 f9_done_status_clear = 1;  /* Local variable which is used to set F9_DONE_CLEAR_BIT */

  if ((tdsrrc_integrity_opt_nv == 0) || (tdsrrc_integrity_opt_nv == 1)) /* 0 or 1 is for sw integrity*/
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Doing SW integrity");
    maci = tdsrrcf9_generate_mac_for_message_using_snow3g_sw(count_i,                           
                                                   dir,         
                                                   msg,         
                                                   msg_length,  
                                                   cn_domain,  
                                                   integrity_algo);
    return(maci);
  }
  
  /*enable the clock before calling MAC calculation
    clock would have been disabled in init function
    to conserve power and improve sleep time*/

/*the following code should be removed since we are using SW engine only*/
/*
#ifndef FEATURE_MCPM
#ifdef FEATURE_CLKREGIM_HEIRARCHICAL
  clk_regime_enable(CLK_RGM_F9_M);
#else
  clk_regime_enable1(MSM_CLK_ENA1_F9_BMSK);
#endif
#endif
*/
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"CN Doamin = %d,Integrity Algo = %x[0=Invalid,1=UIA1,2=UIA2] in rrcsmc_int_ciph_info",cn_domain,integrity_algo);

  /* Length of SDU in bytes */
  local_sdu_len_bytes =  msg_length / 8;
  local_sdu_len_bytes += ( msg_length % 8)? 1: 0;

  /*round off the length of the sdu to next length which is divisible by 64 and
    store copy the contents of the sdu into that ptr.  this is being done, bcos
    we will be casting the unsigned char * to a uint32 ptr */ 
  
  local_sdu_len = msg_length;    
  
  switch(local_sdu_len %64)
  {
    case 0:
      local_sdu_len = local_sdu_len + 64;
      break;

    case 63:
      local_sdu_len = local_sdu_len + 64 + 1;
      break;

    default:
      local_sdu_len = local_sdu_len + (64 - (local_sdu_len % 64));
      break;
  }
  
  /* Allocate memory for 64 bit aligned message pointer  */
  local_sdu_uint32_ptr = (uint32 *) tdsrrc_malloc ( (local_sdu_len/8));

  local_sdu_ptr = (uint8 *)local_sdu_uint32_ptr;

  /*check for calloc failure*/
  if(local_sdu_ptr == NULL)
  {
    ERR_FATAL("Memory alloc failed",0,0,0);
  }

  /*and set the contents to 0 */
  memset(local_sdu_ptr, 0, (local_sdu_len/8)); 

  memscpy(local_sdu_ptr, local_sdu_len_bytes, msg, msg_length);   
  
  /* Check the length, if message to be written is less than 2624 bits then
     it is normal mode operation.. do not set the operation_bitmask to true
   i.e. 2624 for the message  */
  {
    //anything less than 2622 bytes is operated as regular mode in the above condition
    //anything greater than 2622 i.e. 64*41 is treated as continuous mode operation
    //cheetah has a max sdu length limitation of 512 bytes.. so no need to implement
    //continuous mode 2 operation.

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Continuous mode operation");
    //////////////////////first write

    //reset control register
    f9_control_val = 0;

    if( cn_domain == RRC_CS_DOMAIN_CN_ID )
    {
    }
    else if( cn_domain == RRC_PS_DOMAIN_CN_ID )
    {
      /* PS Key selected  - Nothing to do here. */
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid Key for integrity check!");
    }
    
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Integrity Algo - SNOW:3G ");
    /* Indicate Integrity Algo - SNOW:3G */
    /* IndicateDirection which is needed for  SNOW:3G, 
     *this is not needed for KASUMI
     */
    if(dir == TDSRRC_INTEGRITY_DIR_UPLINK)
    {
      /*UPLINK, Bit 19 is set to '0'*/
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"DIR=Uplink ");
    }
    else
    {
      /*DOWNLINK, Bit 19 is set to '1'*/
    }

    TDSRRC_MSG2(MSG_LEGACY_HIGH,"CONTROL REG: 0x%x COUNTI 0x%x",f9_control_val,count_i);

    /* TDS_INTLOCK and TDS_INTFREE registers to F9 area.  This is needed to keep integrity of the
       F9 engine's control registers until the ENGINE is started.  Any TASK-SWITCH in
       between is potentially catastrophic for MAC-I calculation   */
    TDS_INTLOCK();

    /*write the control register and count values to the respective
    registers*/
    /*Write the contents of the message to the F9 engine*/
    /* Start the engine */
    TDS_INTFREE();
    /* Wait until interrupt is pending */
    do
    {
    }while(f9_done_status == 0);
    /* Clear the interrupt.Set F9_DONE_CLEAR flag which will 
        clear F9_DONE_STATUS register    . */
    //////////////////////intermediate writes for continuous mode 2 operations
    
    
    if(num_writes_in_continuous_mode > 2)
    {
      for(i=0; i < (num_writes_in_continuous_mode-2); i++)
      {

        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Continuous Mode 2 operation write %d",i);

        //reset control register for writes between first/last write        
        f9_control_val = 0;
        if( cn_domain == RRC_CS_DOMAIN_CN_ID )
        {
        }
        else if( cn_domain == RRC_PS_DOMAIN_CN_ID )
        {
          /* PS Key selected  - Nothing to do here. */
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid Key for integrity check!");
        }
  
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Integrity Algo - SNOW:3G ");
        /* Indicate Integrity Algo - SNOW:3G */
        /* IndicateDirection which is needed for  SNOW:3G, 
         *this is not needed for KASUMI
         */
        if(dir == TDSRRC_INTEGRITY_DIR_UPLINK)
        {
          /*UPLINK, Bit 19 is set to '0'*/
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"DIR=Uplink ");
        }
        else
        {
          /*DOWNLINK, Bit 19 is set to '1'*/
        }


        TDSRRC_MSG2(MSG_LEGACY_HIGH,"CONTROL REG: 0x%x COUNTI 0x%x",f9_control_val,count_i);

        /* TDS_INTLOCK and TDS_INTFREE registers to F9 area.  This is needed to keep integrity of the
           F9 engine's control registers until the ENGINE is started.  Any TASK-SWITCH in
           between is potentially catastrophic for MAC-I calculation   */
        TDS_INTLOCK();

        /*Write the contents of the message to the F9 engine*/
        //note contents of local_sdu_uint32_ptr continues to increment here..
        /* Start the engine */
        //free interrupts
        TDS_INTFREE();

        /* Wait until interrupt is pending */
        do
        {
        }while(f9_done_status == 0);
        /* Clear the interrupt.Set F9_DONE_CLEAR flag which will 
          clear F9_DONE_STATUS register    . */
      }
    }

    //reset control register for last write
    //for the second write we need to set control register to last write operation


    f9_control_val = 0;


    if( cn_domain == RRC_CS_DOMAIN_CN_ID )
    {
      /* CS Key selected */
    }
    else if( cn_domain == RRC_PS_DOMAIN_CN_ID )
    {
      /* PS Key selected  - Nothing to do here. */
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid Key for integrity check!");
    }
    
    
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Integrity Algo - SNOW:3G ");
    /* Indicate Integrity Algo - SNOW:3G */
    /* IndicateDirection which is needed for  SNOW:3G, 
     *this is not needed for KASUMI
     */
    if(dir == TDSRRC_INTEGRITY_DIR_UPLINK)
    {
      /*UPLINK, Bit 19 is set to '0'*/
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"DIR=Uplink ");
    }
    else
    {
      /*DOWNLINK, Bit 19 is set to '1'*/
    }

    TDSRRC_MSG2(MSG_LEGACY_HIGH,"CONTROL REG: 0x%x COUNTI 0x%x",f9_control_val,count_i);


    /* TDS_INTLOCK and TDS_INTFREE registers to F9 area.  This is needed to keep integrity of the
       F9 engine's control registers until the ENGINE is started.  Any TASK-SWITCH in
       between is potentially catastrophic for MAC-I calculation   */
    TDS_INTLOCK();
    /*Write the contents of the message to the F9 engine*/
    //note contents of local_sdu_uint32_ptr continues to increment here..
    for( count=0; count < (sdu_len_last_write_len/32); count++ )
    {
      if( local_sdu_uint32_ptr == NULL)
      {
        ERR_FATAL("Invalid pointer is accessed",0,0,0);
      }
    
      /* only for debugging, do not turn on regular builds as we are in intlock
         TDSRRC_MSG2(MSG_LEGACY_MED,"Data 0x%x %d",*local_sdu_uint32_ptr,count); */

      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Data 0x%x %d",*local_sdu_uint32_ptr,count);


      local_sdu_uint32_ptr++;
    }
    
   
    //free interrupts
    TDS_INTFREE();

    /*free the mem allocated for the local ptr*/
    tdsrrc_free( local_sdu_ptr);
    /* Wait until interrupt is pending */
    do
    {
    }while(f9_done_status == 0);
    /* Clear the interrupt.Set F9_DONE_CLEAR flag which will 
        clear F9_DONE_STATUS register    . */
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Status 0x%x Dir 0x%x MACI 0x%x", f9statusval, dir, maci);
    return(maci);
  }
}
#endif

/*===========================================================================

  FUNCTION RRCF9_GENERATE_MAC_FOR_MESSAGE

  DESCRIPTION

    This function passes the encoded RRC message and the
  additional parameters that are unique to the message 
  to the integrity engine. It returns the generated
  MAC-I value.
  
  This function is to be used for both downlink and 
  uplink messages.
  

  DEPENDENCIES

    None.

  RETURN VALUE

    A uint32 value that represents the MAC-I (or XMAC-I) for
  this message.

===========================================================================*/
uint32 tdsrrcf9_generate_mac_for_message
( 
  uint32 count_i,                           /* COUNT-I value for the RB */
  tdsrrc_integrity_direction_type dir,         /* Direction - uplink or downlink message */
  uint8 *msg,                               /* Pointer to the encoded message.    */
  uint32 msg_length,                        /* Length of the message in bits */
  rrc_cn_domain_identity_e_type  cn_domain,  /* Key for which domain_id to be used */
  uecomdef_umts_integrity_protection_algorithm_e_type integrity_algo /*integrity protection algorithm*/
)
{
  uint32 local_sdu_len_bits = 0;               /* Local SDU length in bits             */
  uint32 local_sdu_len_bytes = 0;              /* Local SDU length in bytes            */
  uint8 *local_sdu_ptr = NULL;                    /* Local pointer to store SDU */
  uint8  local_direction = 0;              /* Local variable for direction */
  uint32 *local_sdu_uint32_ptr = NULL;            /* Local pointer to store SDU in unit32 */
  uint32 f9statusval=0;                      /* Status value */
  uint32 f9_control_val=0;                 /* Value written to F9 control reg      */
  uint32 local_sdu_len;                    /* Local SDU length divisible next by 64 */
  uint32 maci = 0 ;                        /* Local variable for MACI */
  uint32 count,i;                          /* Local counters */
  uint32 num_writes_in_continuous_mode=0;  /* Local var to store num of writes */
  uint32 sdu_len_last_write_len=0;
  volatile uint32 f9_done_status = 0;  /* Local variable to store interrupt status */
  //uint32 f9_done_status_clear = 1;  /* Local variable which is used to set F9_DONE_CLEAR_BIT */
  if ((tdsrrc_integrity_opt_nv == 0) || (tdsrrc_integrity_opt_nv == 1)) /* 0 or 1 is for sw integrity*/
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Doing SW integrity");
    maci = tdsrrcf9_generate_mac_for_message_sw(count_i,
                                      dir,
                                      msg,
                                      msg_length,
                                      cn_domain,
                                      integrity_algo);
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"maci = 0x%x ",maci);
    return(maci);
  }


  TDSRRC_MSG2(MSG_LEGACY_HIGH,"CN Doamin = %d,Integrity Algo = %x[0=Invalid,1=UIA1,2=UIA2] in rrcsmc_int_ciph_info",cn_domain,integrity_algo);


#ifdef FEATURE_TDSCDMA_SNOW3G_SECURITY
  if(integrity_algo == UE_UMTS_INTEGRITY_PROT_ALGO_UIA2)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Snow3g Algo is selected");
    maci = tdsrrcf9_generate_mac_for_message_using_snow3g(
      count_i,
      dir,
      msg,
      msg_length,
      cn_domain,
      integrity_algo
      );
   
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"maci = 0x%x ",maci);
    return(maci);
  }
#endif

  /* Length of SDU in bits */
  local_sdu_len_bits = msg_length;

  /* Length of SDU in bytes */
  local_sdu_len_bytes =  msg_length / 8;
  local_sdu_len_bytes += ( msg_length % 8)? 1: 0;

  /*round off the length of the sdu to next length which is divisible by 64 and
    store copy the contents of the sdu into that ptr.  this is being done, bcos
    we will be casting the unsigned char * to a uint32 ptr */ 
  
  local_sdu_len = msg_length;    
  
  switch(local_sdu_len %64)
  {
    case 0:
      local_sdu_len = local_sdu_len + 64;
      break;

    case 63:
      local_sdu_len = local_sdu_len + 64 + 1;
      break;

    default:
      local_sdu_len = local_sdu_len + (64 - (local_sdu_len % 64));
      break;
  }
  
  /* Allocate memory for 64 bit aligned message pointer  */
  local_sdu_uint32_ptr = (uint32 *) tdsrrc_malloc ( (local_sdu_len/8));
  local_sdu_ptr = (uint8 *)local_sdu_uint32_ptr;

  /*and set the contents to 0 */
  memset(local_sdu_ptr, 0, (local_sdu_len/8)); 

  /*check for calloc failure*/
  if(local_sdu_ptr == NULL)
  {
    ERR_FATAL("Memory alloc failed",0,0,0);
  }

  memscpy(local_sdu_ptr, local_sdu_len_bytes, msg, msg_length);    
  
  local_direction =  ((uint8)dir << 1) + 1;

  if (( local_sdu_len_bits % 8) == 0)
  {
    local_direction <<= 6;
    local_sdu_ptr[local_sdu_len_bytes] |=  local_direction;
  }
  else
  {
    if( (local_sdu_len_bits % 8 ) <= 6)
    {
      local_direction <<= (8 - ((local_sdu_len_bits % 8 )+2) );
      local_sdu_ptr[local_sdu_len_bytes - 1] |=  local_direction; 
    }
    else if( (local_sdu_len_bits % 8 ) == 7)
    {
      local_sdu_ptr[local_sdu_len_bytes - 1] |=  (uint8)dir;
      local_sdu_ptr[local_sdu_len_bytes] |=  0x80;

    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SDU Length");
    }
  } 

  
  /* Check the length, if message to be written is less than 2624 bits then
     it is normal mode operation.. do not set the operation_bitmask to true
   i.e. 2624 for the message  */
  if( local_sdu_len <= TDSRRC_SDU_WRITE_LEN_BITS )
  {  
    /* Normal mode operation  no change to operational bitmask */
    
    /* First setup the F9 control register */  
    /* Little endian mode */
    if( cn_domain == RRC_CS_DOMAIN_CN_ID )
    {
    }
    else if( cn_domain == RRC_PS_DOMAIN_CN_ID )
    {
      /* PS Key selected  - Nothing to do here. */
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid Key for integrity check!");
    }
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"CONTROL REG: 0x%x COUNTI 0x%x",f9_control_val,count_i);

    /* TDS_INTLOCK and TDS_INTFREE registers to F9 area.  This is needed to keep integrity of the
       F9 engine's control registers until the ENGINE is started.  Any TASK-SWITCH in
       between is potentially catastrophic for MAC-I calculation   */
    TDS_INTLOCK();

    /*write the control register and count values to the respective
    registers*/

    /*Write the contents of the message to the F9 engine*/
    for( count=0; count < (local_sdu_len/32); count++ )
    {
      if( local_sdu_uint32_ptr == NULL)
      {
        ERR_FATAL("Invalid pointer is accessed",0,0,0);
      }
    
      /*  Used for debugging purposes    TDSRRC_MSG2(MSG_LEGACY_MED,"Data 0x%x %d",*local_sdu_uint32_ptr,count);  */
    
      local_sdu_uint32_ptr++;
    }
    
    /* Start the engine */
    TDS_INTFREE();

    /*free the mem allocated for the local ptr*/
    tdsrrc_free( local_sdu_ptr);
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"F9DRIVER:-POLL ON F9_DONE_STATUS");
    /* Wait until interrupt is pending */
    do
    {
    }while(f9_done_status == 0);
    /*read the maci from the output register*/
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Status 0x%x Dir 0x%x MACI 0x%x", f9statusval, dir, maci);
    return(maci);

  }
  else
  {
    //anything less than 2622 bytes is operated as regular mode in the above condition
    //anything greater than 2622 i.e. 64*41 is treated as continuous mode operation
    //cheetah has a max sdu length limitation of 512 bytes.. so no need to implement
    //continuous mode 2 operation.

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Continuous mode operation");

    //get the number of times we need to write in continuous mode
    num_writes_in_continuous_mode = (local_sdu_len/TDSRRC_SDU_WRITE_LEN_BITS);
    
    if((local_sdu_len%TDSRRC_SDU_WRITE_LEN_BITS) != 0)
    {
      num_writes_in_continuous_mode++;
    }

    //////////////////////first write

    //reset control register
    f9_control_val = 0;

    //set the operation bitmask to 1 for continuous operation.
    /* Little endian mode */
    if( cn_domain == RRC_CS_DOMAIN_CN_ID )
    {
    }
    else if( cn_domain == RRC_PS_DOMAIN_CN_ID )
    {
      /* PS Key selected  - Nothing to do here. */
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid Key for integrity check!");
    }
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"CONTROL REG: 0x%x COUNTI 0x%x",f9_control_val,count_i);

    /* TDS_INTLOCK and TDS_INTFREE registers to F9 area.  This is needed to keep integrity of the
       F9 engine's control registers until the ENGINE is started.  Any TASK-SWITCH in
       between is potentially catastrophic for MAC-I calculation   */
    TDS_INTLOCK();

    /*write the control register and count values to the respective
    registers*/
    /*Write the contents of the message to the F9 engine*/
    for( count=0; count < (TDSRRC_SDU_WRITE_LEN_BITS/32); count++ )
    {
      if( local_sdu_uint32_ptr == NULL)
      {
        ERR_FATAL("Invalid pointer is accessed",0,0,0);
      }
    
      /*  only for debugging purposes TDSRRC_MSG2(MSG_LEGACY_MED,"Data 0x%x %d",*local_sdu_uint32_ptr,count);  */
    
    
      local_sdu_uint32_ptr++;
    }
        
    /* Start the engine */
    TDS_INTFREE();
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"F9DRIVER:-POLL ON F9_DONE_STATUS");
    /* Wait until interrupt is pending */
    do
    {
    }while(f9_done_status == 0);
    //////////////////////intermediate writes for continuous mode 2 operations
    
    
    if(num_writes_in_continuous_mode > 2)
    {
      for(i=0; i < (num_writes_in_continuous_mode-2); i++)
      {

        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Continuous Mode 2 operation write %d",i);

        //reset control register for writes between first/last write        
        f9_control_val = 0;
  
        //set the last iteration bit_mask to true
        // Little endian mode and operation mode for continuous mode 
        if( cn_domain == RRC_CS_DOMAIN_CN_ID )
        {
        }
        else if( cn_domain == RRC_PS_DOMAIN_CN_ID )
        {
          /* PS Key selected  - Nothing to do here. */
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid Key for integrity check!");
        }
  
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"CONTROL REG: 0x%x COUNTI 0x%x",f9_control_val,count_i);

        /* TDS_INTLOCK and TDS_INTFREE registers to F9 area.  This is needed to keep integrity of the
           F9 engine's control registers until the ENGINE is started.  Any TASK-SWITCH in
           between is potentially catastrophic for MAC-I calculation   */
        TDS_INTLOCK();

        /*Write the contents of the message to the F9 engine*/
        //note contents of local_sdu_uint32_ptr continues to increment here..
        for( count=0; count < (TDSRRC_SDU_WRITE_LEN_BITS/32); count++ )
        {
          if( local_sdu_uint32_ptr == NULL)
          {
            ERR_FATAL("Invalid pointer is accessed",0,0,0);
          }
  
          local_sdu_uint32_ptr++;
        }
  
        /* Start the engine */
/*lint -e553*/
#ifndef __GNUC__
#if (__ARMCC_VERSION >= 200000 )
#pragma arm
#endif
/* For QDSP 6 this syntax is not compatible*/
        __asm
        {
          NOP
          NOP
        }
#if (__ARMCC_VERSION >= 200000 )
#pragma thumb
#endif
#endif

/*lint +e553*/
        
        //reset the f9 control last iteration bitmask

        //free interrupts
        TDS_INTFREE();

        TDSRRC_MSG1(MSG_LEGACY_HIGH,"CONTROL REG: 0x%x",f9_control_val);
        /* Wait until interrupt is pending */
        do
        {
        }while(f9_done_status == 0);
        /* Clear the interrupt.Set F9_DONE_CLEAR flag which will 
          clear F9_DONE_STATUS register    . */
      }
    }

    //reset control register for last write
    //for the second write we need to set control register to last write operation

    sdu_len_last_write_len = (local_sdu_len % TDSRRC_SDU_WRITE_LEN_BITS);
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Continuous Mode last write,length ",sdu_len_last_write_len);
    
    f9_control_val = 0;

    //set the last iteration bit_mask to true
    // Little endian mode and operation mode for continuous mode 
    if( cn_domain == RRC_CS_DOMAIN_CN_ID )
    {
      /* CS Key selected */
    }
    else if( cn_domain == RRC_PS_DOMAIN_CN_ID )
    {
      /* PS Key selected  - Nothing to do here. */
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid Key for integrity check!");
    }
    
    
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"CONTROL REG: 0x%x COUNTI 0x%x",f9_control_val,count_i);

    /* TDS_INTLOCK and TDS_INTFREE registers to F9 area.  This is needed to keep integrity of the
       F9 engine's control registers until the ENGINE is started.  Any TASK-SWITCH in
       between is potentially catastrophic for MAC-I calculation   */
    TDS_INTLOCK();
    
    /*Write the contents of the message to the F9 engine*/
    //note contents of local_sdu_uint32_ptr continues to increment here..
    for( count=0; count < (sdu_len_last_write_len/32); count++ )
    {
      if( local_sdu_uint32_ptr == NULL)
      {
        ERR_FATAL("Invalid pointer is accessed",0,0,0);
      }
    
      /* only for debugging, do not turn on regular builds as we are in intlock
         TDSRRC_MSG2(MSG_LEGACY_MED,"Data 0x%x %d",*local_sdu_uint32_ptr,count); */

      local_sdu_uint32_ptr++;
    }
    
   
    /* Start the engine */
    //free interrupts
    TDS_INTFREE();

    /*free the mem allocated for the local ptr*/
    tdsrrc_free( local_sdu_ptr);
    /* Wait until interrupt is pending */
    do
    {
    }while(f9_done_status == 0);
    /* Clear the interrupt.Set F9_DONE_CLEAR flag which will 
      clear F9_DONE_STATUS register    . */
    /*read the maci from the output register*/
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Status 0x%x Dir 0x%x MACI 0x%x", f9statusval, dir, maci);
    return(maci);
  }
}

/*===========================================================================

  FUNCTION tdsrrcf9_update_key_value

  DESCRIPTION

    This function performs updates the key_value for the
    domain for which it is changed.  This is called whenever
    RRC_CIPHER_REQ is received from MM layer to update the
    key stored in the engine.

    The following are initialized by this function -
  
    Integrity Key - 128 bit key used by the F9 engine. The
    key is different for CS connections and PS connections.
  
    CN_DOMAIN - is the domain id for which the key has
    changed.   
   
  DEPENDENCIES

    None.

  RETURN VALUE

    None

===========================================================================*/
void tdsrrcf9_update_key_value
( 
  rrc_cn_domain_identity_e_type  cn_domain, /*domain id */
  byte *key_value                           /* Key value         */
)
{
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"domain is %d,key val is %d",cn_domain,key_value[0]);
  /* Template for a future release*/
}



