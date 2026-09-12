/*!                                     
  @file
  IPA_test_mutils_zuc.c
  

  @brief
  REQUIRED brief one-sentence description of this C module.

  @detail
  OPTIONAL detailed description of this C module.
  - DELETE this section if unused.

*/

/*==============================================================================

  Copyright (c) 2010 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

==============================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/dataipa.mpss/3.2/ipa/ipa_stride/IPA_test_mutils_zuc.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/04/12   kp      Added code for ZUC integrity feature

==============================================================================*/

/*==============================================================================
  
                             INCLUDE FILES
  
  ==============================================================================*/

#if 1

#include <msg.h>
#include <fs_public.h>
#include "ipa_test_mutils_zuc.h"

/*==============================================================================

                   INTERNAL DEFINITIONS AND TYPES

==============================================================================*/

#define NO__ZUC_SLIDING_WINDOW__
#define NO__ZUC_OFF_TARGET__
//#define __ZUC_FAKE__
#define __ZUC_ALL_INLINED__

//#define __ZUC_FETCH__
#define FEATURE_QDSP6

//#define __ZUC_SHIFTED_S_ARRAY__
#define NO__ZUC_SHIFTED_EK_D__

//#define __ZUC_OFF_TARGET__

//#define zucDebug

/* ——————————————————————- */
typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned int u32;
typedef unsigned long long u64;
/* ——————————————————————- */


#ifdef __ZUC_OFF_TARGET__

#include <stdlib.h>
#include <string.h>

typedef struct dsm_item_s
{
  u32 used;
  u8  *data_ptr;
  struct dsm_item_s *pkt_ptr;
  u8  Data [2048];
} dsm_item_type;

#ifndef NULL
#define NULL (void *)0
#endif

#else
#include "dsmutil.h"
#endif

#define ZUC_WD_ENDIAN(a) ((((a) & 0xFF) << 24) | (((a) & 0xFF00) << 8) | (((a) >> 8) & 0xFF00) | (((a) >> 24) & 0xFF))

#if 0
static inline void zuc_l2fetch (u32 Rs, u32 Rt)
{
   __asm__  __volatile__
 (
   "l2fetch(%0, %1)\n"
   :: "r" (Rs), "r" (Rt)
 );
}
#endif

#endif


typedef struct
{
 u8  Iv [16];

#ifdef __ZUC_SLIDING_WINDOW__
 u32  LfsrRealTab[32];
 u32* LfsrTab;
#else
 u32 LfsrTab [16];
#endif

 u32 F_R [2];	/* the registers of F */
 u32 BRC_X [4];	/* the outputs of BitReorganization */
} zuc_Ctx_t;


#if (defined(T_QDSP6) || defined(FEATURE_QDSP6) || defined (qdsp6)) && defined (__ZUC_ALL_INLINED__)

static u32 zuc_AddM(u32 a, u32 b) __attribute__ ((__always_inline__));
static void zuc_LFSRWithInitialisationMode (u32 u, zuc_Ctx_t *Ctx) __attribute__ ((__always_inline__));
static void zuc_LFSRWithWorkMode (zuc_Ctx_t *Ctx) __attribute__ ((__always_inline__));
static void zuc_BitReorganization (zuc_Ctx_t *Ctx) __attribute__ ((__always_inline__));
static u32 zuc_L1(u32 X) __attribute__ ((__always_inline__));
static u32 zuc_L2(u32 X) __attribute__ ((__always_inline__));
static u32 zuc_F (zuc_Ctx_t *Ctx) __attribute__ ((__always_inline__));;
static void  zuc_Initialization (u8* k, zuc_Ctx_t *Ctx) __attribute__ ((__always_inline__));
static void zuc_IvInit (u32 COUNT, u32 BEARER, u32 DIRECTION, u8 *IV) __attribute__ ((__always_inline__));

#endif


static uint8 zuc_debug = 0; 
static boolean zuc_debug_init = FALSE;


#if defined (__ZUC_SHIFTED_S_ARRAY__)

typedef struct
{
 u16 S0_8 [256];
 u8  S1_0 [256];
} zuc_S_t;

static const zuc_S_t zuc_S =

{
// S0_8 [256] = 
{
 0x3e<< 8, 0x72<< 8, 0x5b<< 8, 0x47<< 8, 0xca<< 8, 0xe0<< 8, 0x00<< 8, 0x33<< 8, 0x04<< 8, 0xd1<< 8, 0x54<< 8, 0x98<< 8, 0x09<< 8, 0xb9<< 8, 0x6d<< 8, 0xcb<< 8,
 0x7b<< 8, 0x1b<< 8, 0xf9<< 8, 0x32<< 8, 0xaf<< 8, 0x9d<< 8, 0x6a<< 8, 0xa5<< 8, 0xb8<< 8, 0x2d<< 8, 0xfc<< 8, 0x1d<< 8, 0x08<< 8, 0x53<< 8, 0x03<< 8, 0x90<< 8,
 0x4d<< 8, 0x4e<< 8, 0x84<< 8, 0x99<< 8, 0xe4<< 8, 0xce<< 8, 0xd9<< 8, 0x91<< 8, 0xdd<< 8, 0xb6<< 8, 0x85<< 8, 0x48<< 8, 0x8b<< 8, 0x29<< 8, 0x6e<< 8, 0xac<< 8,
 0xcd<< 8, 0xc1<< 8, 0xf8<< 8, 0x1e<< 8, 0x73<< 8, 0x43<< 8, 0x69<< 8, 0xc6<< 8, 0xb5<< 8, 0xbd<< 8, 0xfd<< 8, 0x39<< 8, 0x63<< 8, 0x20<< 8, 0xd4<< 8, 0x38<< 8,
 0x76<< 8, 0x7d<< 8, 0xb2<< 8, 0xa7<< 8, 0xcf<< 8, 0xed<< 8, 0x57<< 8, 0xc5<< 8, 0xf3<< 8, 0x2c<< 8, 0xbb<< 8, 0x14<< 8, 0x21<< 8, 0x06<< 8, 0x55<< 8, 0x9b<< 8,
 0xe3<< 8, 0xef<< 8, 0x5e<< 8, 0x31<< 8, 0x4f<< 8, 0x7f<< 8, 0x5a<< 8, 0xa4<< 8, 0x0d<< 8, 0x82<< 8, 0x51<< 8, 0x49<< 8, 0x5f<< 8, 0xba<< 8, 0x58<< 8, 0x1c<< 8,
 0x4a<< 8, 0x16<< 8, 0xd5<< 8, 0x17<< 8, 0xa8<< 8, 0x92<< 8, 0x24<< 8, 0x1f<< 8, 0x8c<< 8, 0xff<< 8, 0xd8<< 8, 0xae<< 8, 0x2e<< 8, 0x01<< 8, 0xd3<< 8, 0xad<< 8,
 0x3b<< 8, 0x4b<< 8, 0xda<< 8, 0x46<< 8, 0xeb<< 8, 0xc9<< 8, 0xde<< 8, 0x9a<< 8, 0x8f<< 8, 0x87<< 8, 0xd7<< 8, 0x3a<< 8, 0x80<< 8, 0x6f<< 8, 0x2f<< 8, 0xc8<< 8,
 0xb1<< 8, 0xb4<< 8, 0x37<< 8, 0xf7<< 8, 0x0a<< 8, 0x22<< 8, 0x13<< 8, 0x28<< 8, 0x7c<< 8, 0xcc<< 8, 0x3c<< 8, 0x89<< 8, 0xc7<< 8, 0xc3<< 8, 0x96<< 8, 0x56<< 8,
 0x07<< 8, 0xbf<< 8, 0x7e<< 8, 0xf0<< 8, 0x0b<< 8, 0x2b<< 8, 0x97<< 8, 0x52<< 8, 0x35<< 8, 0x41<< 8, 0x79<< 8, 0x61<< 8, 0xa6<< 8, 0x4c<< 8, 0x10<< 8, 0xfe<< 8,
 0xbc<< 8, 0x26<< 8, 0x95<< 8, 0x88<< 8, 0x8a<< 8, 0xb0<< 8, 0xa3<< 8, 0xfb<< 8, 0xc0<< 8, 0x18<< 8, 0x94<< 8, 0xf2<< 8, 0xe1<< 8, 0xe5<< 8, 0xe9<< 8, 0x5d<< 8,
 0xd0<< 8, 0xdc<< 8, 0x11<< 8, 0x66<< 8, 0x64<< 8, 0x5c<< 8, 0xec<< 8, 0x59<< 8, 0x42<< 8, 0x75<< 8, 0x12<< 8, 0xf5<< 8, 0x74<< 8, 0x9c<< 8, 0xaa<< 8, 0x23<< 8,
 0x0e<< 8, 0x86<< 8, 0xab<< 8, 0xbe<< 8, 0x2a<< 8, 0x02<< 8, 0xe7<< 8, 0x67<< 8, 0xe6<< 8, 0x44<< 8, 0xa2<< 8, 0x6c<< 8, 0xc2<< 8, 0x93<< 8, 0x9f<< 8, 0xf1<< 8,
 0xf6<< 8, 0xfa<< 8, 0x36<< 8, 0xd2<< 8, 0x50<< 8, 0x68<< 8, 0x9e<< 8, 0x62<< 8, 0x71<< 8, 0x15<< 8, 0x3d<< 8, 0xd6<< 8, 0x40<< 8, 0xc4<< 8, 0xe2<< 8, 0x0f<< 8,
 0x8e<< 8, 0x83<< 8, 0x77<< 8, 0x6b<< 8, 0x25<< 8, 0x05<< 8, 0x3f<< 8, 0x0c<< 8, 0x30<< 8, 0xea<< 8, 0x70<< 8, 0xb7<< 8, 0xa1<< 8, 0xe8<< 8, 0xa9<< 8, 0x65<< 8,
 0x8d<< 8, 0x27<< 8, 0x1a<< 8, 0xdb<< 8, 0x81<< 8, 0xb3<< 8, 0xa0<< 8, 0xf4<< 8, 0x45<< 8, 0x7a<< 8, 0x19<< 8, 0xdf<< 8, 0xee<< 8, 0x78<< 8, 0x34<< 8, 0x60<< 8,
},
//S1_0 [256] = 
{
 0x55<< 0, 0xc2<< 0, 0x63<< 0, 0x71<< 0, 0x3b<< 0, 0xc8<< 0, 0x47<< 0, 0x86<< 0, 0x9f<< 0, 0x3c<< 0, 0xda<< 0, 0x5b<< 0, 0x29<< 0, 0xaa<< 0, 0xfd<< 0, 0x77<< 0,
 0x8c<< 0, 0xc5<< 0, 0x94<< 0, 0x0c<< 0, 0xa6<< 0, 0x1a<< 0, 0x13<< 0, 0x00<< 0, 0xe3<< 0, 0xa8<< 0, 0x16<< 0, 0x72<< 0, 0x40<< 0, 0xf9<< 0, 0xf8<< 0, 0x42<< 0,
 0x44<< 0, 0x26<< 0, 0x68<< 0, 0x96<< 0, 0x81<< 0, 0xd9<< 0, 0x45<< 0, 0x3e<< 0, 0x10<< 0, 0x76<< 0, 0xc6<< 0, 0xa7<< 0, 0x8b<< 0, 0x39<< 0, 0x43<< 0, 0xe1<< 0,
 0x3a<< 0, 0xb5<< 0, 0x56<< 0, 0x2a<< 0, 0xc0<< 0, 0x6d<< 0, 0xb3<< 0, 0x05<< 0, 0x22<< 0, 0x66<< 0, 0xbf<< 0, 0xdc<< 0, 0x0b<< 0, 0xfa<< 0, 0x62<< 0, 0x48<< 0,
 0xdd<< 0, 0x20<< 0, 0x11<< 0, 0x06<< 0, 0x36<< 0, 0xc9<< 0, 0xc1<< 0, 0xcf<< 0, 0xf6<< 0, 0x27<< 0, 0x52<< 0, 0xbb<< 0, 0x69<< 0, 0xf5<< 0, 0xd4<< 0, 0x87<< 0,
 0x7f<< 0, 0x84<< 0, 0x4c<< 0, 0xd2<< 0, 0x9c<< 0, 0x57<< 0, 0xa4<< 0, 0xbc<< 0, 0x4f<< 0, 0x9a<< 0, 0xdf<< 0, 0xfe<< 0, 0xd6<< 0, 0x8d<< 0, 0x7a<< 0, 0xeb<< 0,
 0x2b<< 0, 0x53<< 0, 0xd8<< 0, 0x5c<< 0, 0xa1<< 0, 0x14<< 0, 0x17<< 0, 0xfb<< 0, 0x23<< 0, 0xd5<< 0, 0x7d<< 0, 0x30<< 0, 0x67<< 0, 0x73<< 0, 0x08<< 0, 0x09<< 0,
 0xee<< 0, 0xb7<< 0, 0x70<< 0, 0x3f<< 0, 0x61<< 0, 0xb2<< 0, 0x19<< 0, 0x8e<< 0, 0x4e<< 0, 0xe5<< 0, 0x4b<< 0, 0x93<< 0, 0x8f<< 0, 0x5d<< 0, 0xdb<< 0, 0xa9<< 0,
 0xad<< 0, 0xf1<< 0, 0xae<< 0, 0x2e<< 0, 0xcb<< 0, 0x0d<< 0, 0xfc<< 0, 0xf4<< 0, 0x2d<< 0, 0x46<< 0, 0x6e<< 0, 0x1d<< 0, 0x97<< 0, 0xe8<< 0, 0xd1<< 0, 0xe9<< 0,
 0x4d<< 0, 0x37<< 0, 0xa5<< 0, 0x75<< 0, 0x5e<< 0, 0x83<< 0, 0x9e<< 0, 0xab<< 0, 0x82<< 0, 0x9d<< 0, 0xb9<< 0, 0x1c<< 0, 0xe0<< 0, 0xcd<< 0, 0x49<< 0, 0x89<< 0,
 0x01<< 0, 0xb6<< 0, 0xbd<< 0, 0x58<< 0, 0x24<< 0, 0xa2<< 0, 0x5f<< 0, 0x38<< 0, 0x78<< 0, 0x99<< 0, 0x15<< 0, 0x90<< 0, 0x50<< 0, 0xb8<< 0, 0x95<< 0, 0xe4<< 0,
 0xd0<< 0, 0x91<< 0, 0xc7<< 0, 0xce<< 0, 0xed<< 0, 0x0f<< 0, 0xb4<< 0, 0x6f<< 0, 0xa0<< 0, 0xcc<< 0, 0xf0<< 0, 0x02<< 0, 0x4a<< 0, 0x79<< 0, 0xc3<< 0, 0xde<< 0,
 0xa3<< 0, 0xef<< 0, 0xea<< 0, 0x51<< 0, 0xe6<< 0, 0x6b<< 0, 0x18<< 0, 0xec<< 0, 0x1b<< 0, 0x2c<< 0, 0x80<< 0, 0xf7<< 0, 0x74<< 0, 0xe7<< 0, 0xff<< 0, 0x21<< 0,
 0x5a<< 0, 0x6a<< 0, 0x54<< 0, 0x1e<< 0, 0x41<< 0, 0x31<< 0, 0x92<< 0, 0x35<< 0, 0xc4<< 0, 0x33<< 0, 0x07<< 0, 0x0a<< 0, 0xba<< 0, 0x7e<< 0, 0x0e<< 0, 0x34<< 0,
 0x88<< 0, 0xb1<< 0, 0x98<< 0, 0x7c<< 0, 0xf3<< 0, 0x3d<< 0, 0x60<< 0, 0x6c<< 0, 0x7b<< 0, 0xca<< 0, 0xd3<< 0, 0x1f<< 0, 0x32<< 0, 0x65<< 0, 0x04<< 0, 0x28<< 0,
 0x64<< 0, 0xbe<< 0, 0x85<< 0, 0x9b<< 0, 0x2f<< 0, 0x59<< 0, 0x8a<< 0, 0xd7<< 0, 0xb0<< 0, 0x25<< 0, 0xac<< 0, 0xaf<< 0, 0x12<< 0, 0x03<< 0, 0xe2<< 0, 0xf2<< 0,
},
};

#else
/* the s-boxes */
static const u8 zuc_S0 [256] =
{
 0x3e,0x72,0x5b,0x47,0xca,0xe0,0x00,0x33,0x04,0xd1,0x54,0x98,0x09,0xb9,0x6d,0xcb,
 0x7b,0x1b,0xf9,0x32,0xaf,0x9d,0x6a,0xa5,0xb8,0x2d,0xfc,0x1d,0x08,0x53,0x03,0x90,
 0x4d,0x4e,0x84,0x99,0xe4,0xce,0xd9,0x91,0xdd,0xb6,0x85,0x48,0x8b,0x29,0x6e,0xac,
 0xcd,0xc1,0xf8,0x1e,0x73,0x43,0x69,0xc6,0xb5,0xbd,0xfd,0x39,0x63,0x20,0xd4,0x38,
 0x76,0x7d,0xb2,0xa7,0xcf,0xed,0x57,0xc5,0xf3,0x2c,0xbb,0x14,0x21,0x06,0x55,0x9b,
 0xe3,0xef,0x5e,0x31,0x4f,0x7f,0x5a,0xa4,0x0d,0x82,0x51,0x49,0x5f,0xba,0x58,0x1c,
 0x4a,0x16,0xd5,0x17,0xa8,0x92,0x24,0x1f,0x8c,0xff,0xd8,0xae,0x2e,0x01,0xd3,0xad,
 0x3b,0x4b,0xda,0x46,0xeb,0xc9,0xde,0x9a,0x8f,0x87,0xd7,0x3a,0x80,0x6f,0x2f,0xc8,
 0xb1,0xb4,0x37,0xf7,0x0a,0x22,0x13,0x28,0x7c,0xcc,0x3c,0x89,0xc7,0xc3,0x96,0x56,
 0x07,0xbf,0x7e,0xf0,0x0b,0x2b,0x97,0x52,0x35,0x41,0x79,0x61,0xa6,0x4c,0x10,0xfe,
 0xbc,0x26,0x95,0x88,0x8a,0xb0,0xa3,0xfb,0xc0,0x18,0x94,0xf2,0xe1,0xe5,0xe9,0x5d,
 0xd0,0xdc,0x11,0x66,0x64,0x5c,0xec,0x59,0x42,0x75,0x12,0xf5,0x74,0x9c,0xaa,0x23,
 0x0e,0x86,0xab,0xbe,0x2a,0x02,0xe7,0x67,0xe6,0x44,0xa2,0x6c,0xc2,0x93,0x9f,0xf1,
 0xf6,0xfa,0x36,0xd2,0x50,0x68,0x9e,0x62,0x71,0x15,0x3d,0xd6,0x40,0xc4,0xe2,0x0f,
 0x8e,0x83,0x77,0x6b,0x25,0x05,0x3f,0x0c,0x30,0xea,0x70,0xb7,0xa1,0xe8,0xa9,0x65,
 0x8d,0x27,0x1a,0xdb,0x81,0xb3,0xa0,0xf4,0x45,0x7a,0x19,0xdf,0xee,0x78,0x34,0x60
};

static const u8 zuc_S1 [256] =
{
 0x55,0xc2,0x63,0x71,0x3b,0xc8,0x47,0x86,0x9f,0x3c,0xda,0x5b,0x29,0xaa,0xfd,0x77,
 0x8c,0xc5,0x94,0x0c,0xa6,0x1a,0x13,0x00,0xe3,0xa8,0x16,0x72,0x40,0xf9,0xf8,0x42,
 0x44,0x26,0x68,0x96,0x81,0xd9,0x45,0x3e,0x10,0x76,0xc6,0xa7,0x8b,0x39,0x43,0xe1,
 0x3a,0xb5,0x56,0x2a,0xc0,0x6d,0xb3,0x05,0x22,0x66,0xbf,0xdc,0x0b,0xfa,0x62,0x48,
 0xdd,0x20,0x11,0x06,0x36,0xc9,0xc1,0xcf,0xf6,0x27,0x52,0xbb,0x69,0xf5,0xd4,0x87,
 0x7f,0x84,0x4c,0xd2,0x9c,0x57,0xa4,0xbc,0x4f,0x9a,0xdf,0xfe,0xd6,0x8d,0x7a,0xeb,
 0x2b,0x53,0xd8,0x5c,0xa1,0x14,0x17,0xfb,0x23,0xd5,0x7d,0x30,0x67,0x73,0x08,0x09,
 0xee,0xb7,0x70,0x3f,0x61,0xb2,0x19,0x8e,0x4e,0xe5,0x4b,0x93,0x8f,0x5d,0xdb,0xa9,
 0xad,0xf1,0xae,0x2e,0xcb,0x0d,0xfc,0xf4,0x2d,0x46,0x6e,0x1d,0x97,0xe8,0xd1,0xe9,
 0x4d,0x37,0xa5,0x75,0x5e,0x83,0x9e,0xab,0x82,0x9d,0xb9,0x1c,0xe0,0xcd,0x49,0x89,
 0x01,0xb6,0xbd,0x58,0x24,0xa2,0x5f,0x38,0x78,0x99,0x15,0x90,0x50,0xb8,0x95,0xe4,
 0xd0,0x91,0xc7,0xce,0xed,0x0f,0xb4,0x6f,0xa0,0xcc,0xf0,0x02,0x4a,0x79,0xc3,0xde,
 0xa3,0xef,0xea,0x51,0xe6,0x6b,0x18,0xec,0x1b,0x2c,0x80,0xf7,0x74,0xe7,0xff,0x21,
 0x5a,0x6a,0x54,0x1e,0x41,0x31,0x92,0x35,0xc4,0x33,0x07,0x0a,0xba,0x7e,0x0e,0x34,
 0x88,0xb1,0x98,0x7c,0xf3,0x3d,0x60,0x6c,0x7b,0xca,0xd3,0x1f,0x32,0x65,0x04,0x28,
 0x64,0xbe,0x85,0x9b,0x2f,0x59,0x8a,0xd7,0xb0,0x25,0xac,0xaf,0x12,0x03,0xe2,0xf2
};
#endif

/* the constants D */
#if defined(__ZUC_SHIFTED_EK_D__)
static const u32 zuc_EK_d[16] =
{
 0x44D7 << 8, 0x26BC << 8, 0x626B << 8, 0x135E << 8, 0x5789 << 8, 0x35E2 << 8, 0x7135 << 8, 0x09AF << 8,
 0x4D78 << 8, 0x2F13 << 8, 0x6BC4 << 8, 0x1AF1 << 8, 0x5E26 << 8, 0x3C4D << 8, 0x789A << 8, 0x47AC << 8
};
#else
static const u32 zuc_EK_d[16] =
{
 0x44D7, 0x26BC, 0x626B, 0x135E, 0x5789, 0x35E2, 0x7135, 0x09AF,
 0x4D78, 0x2F13, 0x6BC4, 0x1AF1, 0x5E26, 0x3C4D, 0x789A, 0x47AC
};
#endif

/* ——————————————————————- */
/* c = a + b mod 2^31 - 1 */
static u32 zuc_AddM(u32 a, u32 b)
{
 u32 c = a + b;
 return (c & 0x7FFFFFFF) + (c >> 31);
}

/* LFSR with initialization mode */
#define zuc_MulByPow2(x, k) ((((x) << k) | ((x) >> (31 - k))) & 0x7FFFFFFF)


static void zuc_LFSRWithInitialisationMode(u32 u, zuc_Ctx_t *Ctx)
{
 u32 f, v;
 f = Ctx -> LfsrTab [0];
 v = zuc_MulByPow2(Ctx -> LfsrTab [0], 8);
 f = zuc_AddM(f, v);

 v = zuc_MulByPow2(Ctx -> LfsrTab [4], 20);
 f = zuc_AddM(f, v);

 v = zuc_MulByPow2(Ctx -> LfsrTab [10], 21);
 f = zuc_AddM(f, v);

 v = zuc_MulByPow2(Ctx -> LfsrTab [13], 17);
 f = zuc_AddM(f, v);

 v = zuc_MulByPow2(Ctx -> LfsrTab [15], 15);
 f = zuc_AddM(f, v);
 f = zuc_AddM(f, u);

#ifdef __ZUC_SLIDING_WINDOW__

 Ctx->LfsrTab[0] = Ctx->LfsrTab[16] = f;
 Ctx->LfsrTab++;

 if (Ctx->LfsrTab >= Ctx->LfsrRealTab+16)
 {
  Ctx->LfsrTab = Ctx->LfsrRealTab;
 }

#else

 /* update the state */
 Ctx -> LfsrTab [0] = Ctx -> LfsrTab [1];
 Ctx -> LfsrTab [1] = Ctx -> LfsrTab [2];
 Ctx -> LfsrTab [2] = Ctx -> LfsrTab [3];
 Ctx -> LfsrTab [3] = Ctx -> LfsrTab [4];
 Ctx -> LfsrTab [4] = Ctx -> LfsrTab [5];
 Ctx -> LfsrTab [5] = Ctx -> LfsrTab [6];
 Ctx -> LfsrTab [6] = Ctx -> LfsrTab [7];
 Ctx -> LfsrTab [7] = Ctx -> LfsrTab [8];
 Ctx -> LfsrTab [8] = Ctx -> LfsrTab [9];
 Ctx -> LfsrTab [9] = Ctx -> LfsrTab [10];
 Ctx -> LfsrTab [10] = Ctx -> LfsrTab [11];
 Ctx -> LfsrTab [11] = Ctx -> LfsrTab [12];
 Ctx -> LfsrTab [12] = Ctx -> LfsrTab [13];
 Ctx -> LfsrTab [13] = Ctx -> LfsrTab [14];
 Ctx -> LfsrTab [14] = Ctx -> LfsrTab [15];
 Ctx -> LfsrTab [15] = f;

#endif
}


/* LFSR with work mode */
static void zuc_LFSRWithWorkMode (zuc_Ctx_t *Ctx)
{
 u32 f, v;
 f = Ctx -> LfsrTab [0];

 v = zuc_MulByPow2(Ctx -> LfsrTab [0], 8);
 f = zuc_AddM(f, v);

 v = zuc_MulByPow2(Ctx -> LfsrTab [4], 20);
 f = zuc_AddM(f, v);

 v = zuc_MulByPow2(Ctx -> LfsrTab [10], 21);
 f = zuc_AddM(f, v);

 v = zuc_MulByPow2(Ctx -> LfsrTab [13], 17);
 f = zuc_AddM(f, v);

 v = zuc_MulByPow2(Ctx -> LfsrTab [15], 15);
 f = zuc_AddM(f, v);

#ifdef __ZUC_SLIDING_WINDOW__

 Ctx->LfsrTab[0] = Ctx->LfsrTab[16] = f;
 Ctx->LfsrTab++;

 if (Ctx->LfsrTab >= Ctx->LfsrRealTab+16)
 {
  Ctx->LfsrTab = Ctx->LfsrRealTab;
 }

#else

 /* update the state */
 Ctx -> LfsrTab [0]  = Ctx -> LfsrTab [1];
 Ctx -> LfsrTab [1]  = Ctx -> LfsrTab [2];
 Ctx -> LfsrTab [2]  = Ctx -> LfsrTab [3];
 Ctx -> LfsrTab [3]  = Ctx -> LfsrTab [4];
 Ctx -> LfsrTab [4]  = Ctx -> LfsrTab [5];
 Ctx -> LfsrTab [5]  = Ctx -> LfsrTab [6];
 Ctx -> LfsrTab [6]  = Ctx -> LfsrTab [7];
 Ctx -> LfsrTab [7]  = Ctx -> LfsrTab [8];
 Ctx -> LfsrTab [8]  = Ctx -> LfsrTab [9];
 Ctx -> LfsrTab [9]  = Ctx -> LfsrTab [10];
 Ctx -> LfsrTab [10] = Ctx -> LfsrTab [11];
 Ctx -> LfsrTab [11] = Ctx -> LfsrTab [12];
 Ctx -> LfsrTab [12] = Ctx -> LfsrTab [13];
 Ctx -> LfsrTab [13] = Ctx -> LfsrTab [14];
 Ctx -> LfsrTab [14] = Ctx -> LfsrTab [15];
 Ctx -> LfsrTab [15] = f;

#endif
}


/* BitReorganization */
static void zuc_BitReorganization (zuc_Ctx_t *Ctx)
{
 Ctx -> BRC_X [0] = ((Ctx -> LfsrTab [15] & 0x7FFF8000) << 1) | (Ctx -> LfsrTab [14] & 0xFFFF);
 Ctx -> BRC_X [1] = ((Ctx -> LfsrTab [11] & 0xFFFF) << 16) | (Ctx -> LfsrTab [9] >> 15);
 Ctx -> BRC_X [2] = ((Ctx -> LfsrTab [7] & 0xFFFF) << 16) | (Ctx -> LfsrTab [5] >> 15);
 Ctx -> BRC_X [3] = ((Ctx -> LfsrTab [2] & 0xFFFF) << 16) | (Ctx -> LfsrTab [0] >> 15);
}


#define zuc_ROT(a, k) (((a) << k) | ((a) >> (32 - k)))
/* zuc_L1 */
static u32 zuc_L1(u32 X)
{
return (X ^ zuc_ROT(X, 2) ^ zuc_ROT(X, 10) ^ zuc_ROT(X, 18) ^ zuc_ROT(X, 24));
}
/* zuc_L2 */
static u32 zuc_L2(u32 X)
{
return (X ^ zuc_ROT(X, 8) ^ zuc_ROT(X, 14) ^ zuc_ROT(X, 22) ^ zuc_ROT(X, 30));
}
#define zuc_MAKEU32(a, b, c, d) (((u32)(a) << 24) | ((u32)(b) << 16) | ((u32)(c) << 8) | ((u32)(d)))

/* F */
static u32 zuc_F (zuc_Ctx_t *Ctx)
{
 u32 W, W1, W2, u, v;
 W = (Ctx -> BRC_X [0] ^ Ctx -> F_R [0]) + Ctx -> F_R [1];
 W1 = Ctx -> F_R [0] + Ctx -> BRC_X [1];
 W2 = Ctx -> F_R [1] ^ Ctx -> BRC_X [2];
 u = zuc_L1((W1 << 16) | (W2 >> 16));
 v = zuc_L2((W2 << 16) | (W1 >> 16));

#if defined (__ZUC_SHIFTED_S_ARRAY__)
 Ctx -> F_R [0] = ((zuc_S.S0_8 [(u >> 24)] | zuc_S.S1_0 [((u >> 16) & 0xFF)]) << 16) | zuc_S.S0_8 [((u >> 8) & 0xFF)] | zuc_S.S1_0[(u & 0xFF)];
 Ctx -> F_R [1] = ((zuc_S.S0_8 [(v >> 24)] | zuc_S.S1_0 [((v >> 16) & 0xFF)]) << 16) | zuc_S.S0_8 [((v >> 8) & 0xFF)] | zuc_S.S1_0[(v & 0xFF)];
#else
 Ctx -> F_R [0] = zuc_MAKEU32(zuc_S0[u >> 24], zuc_S1[(u >> 16) & 0xFF], zuc_S0[(u >> 8) & 0xFF], zuc_S1[u & 0xFF]);
 Ctx -> F_R [1] = zuc_MAKEU32(zuc_S0[v >> 24], zuc_S1[(v >> 16) & 0xFF], zuc_S0[(v >> 8) & 0xFF], zuc_S1[v & 0xFF]);
#endif

 return W;
}

#if defined(__ZUC_SHIFTED_EK_D__)
#define  zuc_MAKEU31(a, b, c) (((u32)(a) << 23) | b | c)
#else
#define  zuc_MAKEU31(a, b, c) (((u32)(a) << 23) | ((u32)(b) << 8) | (u32)(c))
#endif
static void  zuc_Initialization (u8* k, zuc_Ctx_t *Ctx)
{
 u32 w, nCount;

#ifdef __ZUC_SLIDING_WINDOW__
 Ctx->LfsrTab = Ctx->LfsrRealTab;
#endif

 /* expand key */

 Ctx -> LfsrTab [0]  = zuc_MAKEU31(k[0],  zuc_EK_d[0],  Ctx -> Iv[0]);
 Ctx -> LfsrTab [1]  = zuc_MAKEU31(k[1],  zuc_EK_d[1],  Ctx -> Iv[1]);
 Ctx -> LfsrTab [2]  = zuc_MAKEU31(k[2],  zuc_EK_d[2],  Ctx -> Iv[2]);
 Ctx -> LfsrTab [3]  = zuc_MAKEU31(k[3],  zuc_EK_d[3],  Ctx -> Iv[3]);
 Ctx -> LfsrTab [4]  = zuc_MAKEU31(k[4],  zuc_EK_d[4],  Ctx -> Iv[4]);
 Ctx -> LfsrTab [5]  = zuc_MAKEU31(k[5],  zuc_EK_d[5],  Ctx -> Iv[5]);
 Ctx -> LfsrTab [6]  = zuc_MAKEU31(k[6],  zuc_EK_d[6],  Ctx -> Iv[6]);
 Ctx -> LfsrTab [7]  = zuc_MAKEU31(k[7],  zuc_EK_d[7],  Ctx -> Iv[7]);
 Ctx -> LfsrTab [8]  = zuc_MAKEU31(k[8],  zuc_EK_d[8],  Ctx -> Iv[8]);
 Ctx -> LfsrTab [9]  = zuc_MAKEU31(k[9],  zuc_EK_d[9],  Ctx -> Iv[9]);
 Ctx -> LfsrTab [10] = zuc_MAKEU31(k[10], zuc_EK_d[10], Ctx -> Iv[10]);
 Ctx -> LfsrTab [11] = zuc_MAKEU31(k[11], zuc_EK_d[11], Ctx -> Iv[11]);
 Ctx -> LfsrTab [12] = zuc_MAKEU31(k[12], zuc_EK_d[12], Ctx -> Iv[12]);
 Ctx -> LfsrTab [13] = zuc_MAKEU31(k[13], zuc_EK_d[13], Ctx -> Iv[13]);
 Ctx -> LfsrTab [14] = zuc_MAKEU31(k[14], zuc_EK_d[14], Ctx -> Iv[14]);
 Ctx -> LfsrTab [15] = zuc_MAKEU31(k[15], zuc_EK_d[15], Ctx -> Iv[15]);

 /* set F_R1 and F_R2 to zero */
 Ctx -> F_R [0] = 0;
 Ctx -> F_R [1] = 0;
 nCount = 32;
 while (nCount > 0)
 {
  zuc_BitReorganization (Ctx);
  w = zuc_F (Ctx);
  zuc_LFSRWithInitialisationMode(w >> 1, Ctx);
  nCount --;
 }

}


static void zuc_IvInit (u32 COUNT, u32 BEARER, u32 DIRECTION, u8 *IV)
{
  u32 *IV32 = (u32 *)IV;

  IV32 [0] = ZUC_WD_ENDIAN(COUNT);  // Little to big endian conversion
  IV32 [1] = ((BEARER << 3) | ((DIRECTION&1)<<2)) & 0xFC;
  IV32 [2] = IV32 [0];
  IV32 [3] = IV32 [1];
}


void zuc_dsm_EEA3 (u8* CipheringKey, u32 Count, u32 Bearer, u32 Direction, dsm_item_type *dsm_ptr)
{
 static u32 zuc_DataCnt = 0;
 zuc_Ctx_t Ctx;
 u32 NewKey0_Endian;
 u64 NewKey1_Endian_64 ;
 // internally, cpu still does little-endian operations, so need to reverse manipulations
 u64 NewLongKey;
 u64 LongKey;
 //u64 idx,idy;

 dsm_item_type * Next = dsm_ptr;
//  u64  NewLongKey, LongKey, OverflowKey;
  u64  OverflowKey;
  u32  i, NbByteInKeyForNext, NewKey0, NewKey1;
  u8   OverflowKeyCnt;
  u8 * ByteDataPtr, *ByteKeyPtr = NULL;

  u64 *AlignedPtr  = (u64 *)(((u32)Next -> data_ptr) &~(0x07L));
//  u32  AlignShift  = (u32)(64 + (((u32)(Next -> data_ptr) & (0x07L)) << 3)) & 0x03F;
  // compiler already does this simple math optimization
  u32  AlignShift  = (u32)((((u32)(Next -> data_ptr) & (0x07L)) << 3));
  u32  ByteCnt     = Next -> used + ((u32)(Next -> data_ptr) & (0x07L));
  //u32  Rs, Rt;

 if (dsm_ptr == NULL)
  return;

 zuc_IvInit (Count, Bearer, Direction, (u8 *)Ctx.Iv);
 zuc_Initialization (CipheringKey, &Ctx);

 zuc_BitReorganization (&Ctx);
 zuc_F (&Ctx); /* discard the output of zuc_F */
 zuc_LFSRWithWorkMode (&Ctx);

  while (Next != NULL)
  {
   zuc_DataCnt += Next -> used;

#if (defined(T_QDSP6) || defined(FEATURE_QDSP6) || defined (qdsp6)) && defined (__ZUC_FETCH__)
   if (Next -> used > 0)
   {
    Rt = (32 << 16) | (32 << 8) | (Next -> used >> 5);
    Rs = (u32)(Next -> data_ptr) & (~0x1F);

    zuc_l2fetch (Rs, Rt);
   }
#endif

  
   OverflowKey = 0;

   // Main encryption loop as long as at least 8 bytes of input data are available
   while (ByteCnt > 0)
   {
    // Generate the key
    zuc_BitReorganization (&Ctx);
    NewKey0 = zuc_F (&Ctx) ^ Ctx.BRC_X [3];
    zuc_LFSRWithWorkMode (&Ctx);
    zuc_BitReorganization (&Ctx);
    NewKey1 = zuc_F (&Ctx) ^ Ctx.BRC_X [3];
    zuc_LFSRWithWorkMode (&Ctx);

#ifdef __ZUC_FAKE__
    if (NewKey0 != 1) // (NewKey1 + 1))
    {
     NewKey0 = 0;
     NewKey1 = 0;
    }
#endif
  
      // swizzle bytes of keys to translate from little-endian to big-endian
      NewKey0_Endian = ZUC_WD_ENDIAN (NewKey0);
      NewKey1_Endian_64 = (u64)(ZUC_WD_ENDIAN (NewKey1));
      // internally, cpu still does little-endian operations, so need to reverse manipulations
      NewLongKey  = (NewKey1_Endian_64 << 32) | (NewKey0_Endian);
      LongKey     = (NewLongKey << AlignShift) | OverflowKey;
      OverflowKey = NewLongKey >> (64 - AlignShift);

      // Cipher
      if (ByteCnt >= 8)
      {
        
        *AlignedPtr ^= LongKey;
       
        AlignedPtr++;
        ByteCnt -= 8;
      }
      // have key bytes left over
      else
      {
        ByteDataPtr = (u8 *const)AlignedPtr;
        ByteKeyPtr  = (u8 *const)&LongKey;
  
        for (i=0; i<ByteCnt; i++)
        {
          ByteDataPtr [i] ^= ByteKeyPtr [i];
          
        break;
      }
   }


   }//end while bytecnt != 0

   NbByteInKeyForNext = ((8 - ByteCnt) & 0x07);
   Next = Next -> pkt_ptr;
   
   // Check if part of the current key have to be applied on the next dsm item
   if ((Next != NULL) && ((NbByteInKeyForNext > 0) || (AlignShift > 0)))
   {
    u32 ByteCnt2;

    // loop through blocks until leftover key bytes can't fill them
    while(NbByteInKeyForNext >= Next->used)
    {
      ByteCnt = Next->used;
      for (i=0; ((i<ByteCnt) && (ByteKeyPtr != NULL)); i++)
      {
        Next->data_ptr[i] ^= ByteKeyPtr[(8-NbByteInKeyForNext) + i];
        
      }
      NbByteInKeyForNext -= ByteCnt;

      
      Next = Next->pkt_ptr;

      
      if (Next == NULL)
        return;
    }

    // leftover key bytes cant fill data, do as much as possible
    ByteCnt = NbByteInKeyForNext;

    for (i=0; ((i<ByteCnt) && (ByteKeyPtr != NULL)); i++)
    {
      Next -> data_ptr [i] ^= ByteKeyPtr [(8-NbByteInKeyForNext) + i];
      
    }

    // apply leftover key bytes from alignshift
    ByteKeyPtr  = (u8 *)&OverflowKey;
    OverflowKeyCnt = 0;

    while( ((AlignShift >> 3) - OverflowKeyCnt) >= (Next->used - ByteCnt) )
    {
      ByteCnt2 = Next->used - ByteCnt;
      for (i=0; ((i< ByteCnt2) && (ByteKeyPtr != NULL)); i++)
      {
        Next -> data_ptr [i+ByteCnt] ^= ByteKeyPtr [i + OverflowKeyCnt];
        
      }

      OverflowKeyCnt += ByteCnt2;

      ByteCnt = 0;
     

      Next = Next->pkt_ptr;
      

      if (Next == NULL)
        return;
    }

    ByteCnt2 = (AlignShift >> 3) - OverflowKeyCnt;
    
    for (i=0; ((i< ByteCnt2) && (ByteKeyPtr != NULL)); i++)
    {
      Next -> data_ptr [i+ByteCnt] ^= ByteKeyPtr [i + OverflowKeyCnt];
      
    }
    ByteCnt += ByteCnt2;

    if(ByteCnt == Next->used)
    {
      ByteCnt = 0;
      // 4.
      

      Next = Next->pkt_ptr;

    }
   }
   else
   {
    ByteCnt = 0;
   }

   if (Next != NULL)
   {
    AlignedPtr = (u64 *)(((u32)(&Next->data_ptr [ByteCnt])) &~(0x07L));
    AlignShift = (u32)((((u32)(&Next->data_ptr [ByteCnt]) & (0x07L)) << 3));
    ByteCnt    = Next->used - ByteCnt + ((u32)(&Next -> data_ptr [ByteCnt]) & (0x07L));;
   }
   else
   {
    break;
   }
  }
}

