/*!
  @file
  qsh_util.h

  @brief
  Contains definitions required to be public. Client should not use anything
  in this file.
*/

/*==============================================================================

  Copyright (c) 2018 QUALCOMM Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

==============================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/utils.mpss/5.4/api/qsh/backend/qsh_util.h#2 $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
05/01/19   rn      CR 2442657: Clean up Sherlock ulog
12/07/18   mm      CR 2363373: Enable stubs on linux MOB
11/21/18   mm      CR 2349489: Use qurt signal instead of rex
08/29/18   mm      CR 2305647: Add bytes/words conversion macros
06/19/18   yx      CR 2263684: Ulog to QTrace migration in QSH_LOG 
06/18/18   am      CR 2251065: FR51021 QTrace master buffer to use large memory
05/23/18   rs      CR 2113799: Disabled Metric START/STOP by default
05/22/18   mm      CR 2223829: Hexsim support
04/02/18   mm      CR 2206706: QTrace adjustment for QTF on MOB
04/02/18   rs      CR 2216443: Restricting Qtrace() on QEVENT_FULL() for QTF MOB
01/17/18   am      CR 2151280: QTrace string table enhancement
08/18/17   mm      CR 2095406: Removed qsh_can_log()
01/12/17   mm      CR 1081130: Trace feature
10/11/15   ca      CR 920884: QSH rule management feature.
07/06/15   mm      CR 866981: Adding support for Linux MOB
03/03/15   mm      Removed EVENT
02/19/15   mm      Added EVENT to qsh_msg_type_e
01/22/15   mm      Interface changes for dumping data structures
11/21/14   mm      Added TRM client
08/11/14   sn      Changed client CFM -> CFCM
05/23/14   mm      Initial check-in                 
==============================================================================*/

#ifndef QSH_UTIL_H
#define QSH_UTIL_H

/*==============================================================================

                           INCLUDE FILES

==============================================================================*/

#if !defined(TEST_FRAMEWORK) || defined(FEATURE_HEXSIM)
  #define FEATURE_QSH_ON_TARGET
  #define FEATURE_QSH_WDOG_HB_DYNAMIC
#endif

#ifdef TEST_FRAMEWORK
  #error code not present
#endif

#include <comdef.h>
#include <rex.h>
#include <ULog.h>
#include <ULogFront.h>
#include <msg.h>
#include <qsh_types.h>
#include <qsh_str.h>

/*==============================================================================

                   EXTERNAL DEFINITIONS AND TYPES

==============================================================================*/


/*==============================================================================

                             MACROS

==============================================================================*/

/*! @brief
    This Macro provides the compiler with branch prediction information.

    Calling QSH_LIKELY(expr) informs the compiler that it is expected that
    expr == 1 <NOTE: expr has to be equal to 1 not greater than 1>.

    Calling QSH_UNLIKELY(expr) informs the compiler that it is expected
    that expr is going to be 0, hence dont perform prefetching of instructions
    inside.

    @usage
    Use "if (QSH_LIKELY(a==b))" {} if expression '(a==b)' is expected to be
    TRUE most of the time else use "if (QSH_UNLIKELY(a==b)"
*/
#if defined(__GNUC__) && !defined(_lint)
  #define QSH_LIKELY(x) __builtin_expect((x), 1)
  #define QSH_UNLIKELY(x) __builtin_expect((x), 0)
#else
  #define QSH_LIKELY(x) (x)
  #define QSH_UNLIKELY(x) (x)
#endif /* ! (__GNUC__) */

/*! Words to bytes and vice-versa. */
#define QSH_WORDS_TO_BYTES(x) \
  ((x) << 2)
#define QSH_BYTES_TO_WORDS(x) \
  ((x) >> 2)
#define QSH_BYTES_TO_WORDS_CEIL(x) \
  (((x)+3)>>2)

/*! Bytes to bits and vice-versa. */
#define QSH_BYTES_TO_BITS(x) \
  ((x) << 3)
#define QSH_BITS_TO_BYTES(x) \
  (((x)+7)>>3)

/*! @brief Macro to stringify given input. Macro structure borrowed from
    <msg_qsr.h>.
    @note Needed if macro requires additional layer of expansion to produce
    desired value.
*/
#define QSH_STR(x) \
  _QSH_STR(x)
#define _QSH_STR(x) \
  #x

/*! @brief Helper macros borrowed from <mcs_comdef.h>. Can't directly include
    due to resulting unrelated compilation errors.
*/
#define QSH_EXPAND(x) \
  x

#ifdef _MSC_VER
  #define QSH_NARG(...) QSH_NARG_REV(QSH_WRAPPER(__VA_ARGS__))
#else
  #define QSH_NARG(...) QSH_NARG_REV(dummy, ## __VA_ARGS__)
#endif

#ifndef T_WINNT

#define QSH_CONCAT(...) \
  QSH_EXPAND(QSH_CONCAT_256(__VA_ARGS__, \
    ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,\
    ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,\
    ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,\
    ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,\
    ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,\
    ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,\
    ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,))

#define QSH_CONCAT_256( \
  a0,a1,a2,a3,a4,a5,a6,a7,a8,a9, \
  a10,a11,a12,a13,a14,a15,a16,a17,a18,a19, \
  a20,a21,a22,a23,a24,a25,a26,a27,a28,a29, \
  a30,a31,a32,a33,a34,a35,a36,a37,a38,a39, \
  a40,a41,a42,a43,a44,a45,a46,a47,a48,a49, \
  a50,a51,a52,a53,a54,a55,a56,a57,a58,a59, \
  a60,a61,a62,a63,a64,a65,a66,a67,a68,a69, \
  a70,a71,a72,a73,a74,a75,a76,a77,a78,a79, \
  a80,a81,a82,a83,a84,a85,a86,a87,a88,a89, \
  a90,a91,a92,a93,a94,a95,a96,a97,a98,a99, \
  a100,a101,a102,a103,a104,a105,a106,a107,a108,a109, \
  a110,a111,a112,a113,a114,a115,a116,a117,a118,a119, \
  a120,a121,a122,a123,a124,a125,a126,a127,a128,a129, \
  a130,a131,a132,a133,a134,a135,a136,a137,a138,a139, \
  a140,a141,a142,a143,a144,a145,a146,a147,a148,a149, \
  a150,a151,a152,a153,a154,a155,a156,a157,a158,a159, \
  a160,a161,a162,a163,a164,a165,a166,a167,a168,a169, \
  a170,a171,a172,a173,a174,a175,a176,a177,a178,a179, \
  a180,a181,a182,a183,a184,a185,a186,a187,a188,a189, \
  a190,a191,a192,a193,a194,a195,a196,a197,a198,a199, \
  a200,a201,a202,a203,a204,a205,a206,a207,a208,a209, \
  a210,a211,a212,a213,a214,a215,a216,a217,a218,a219, \
  a220,a221,a222,a223,a224,a225,a226,a227,a228,a229, \
  a230,a231,a232,a233,a234,a235,a236,a237,a238,a239, \
  a240,a241,a242,a243,a244,a245,a246,a247,a248,a249, \
  a250,a251,a252,a253,a254,a255) \
  a0##a1##a2##a3##a4##a5##a6##a7##a8##a9## \
  a10##a11##a12##a13##a14##a15##a16##a17##a18##a19## \
  a20##a21##a22##a23##a24##a25##a26##a27##a28##a29## \
  a30##a31##a32##a33##a34##a35##a36##a37##a38##a39## \
  a40##a41##a42##a43##a44##a45##a46##a47##a48##a49## \
  a50##a51##a52##a53##a54##a55##a56##a57##a58##a59## \
  a60##a61##a62##a63##a64##a65##a66##a67##a68##a69## \
  a70##a71##a72##a73##a74##a75##a76##a77##a78##a79## \
  a80##a81##a82##a83##a84##a85##a86##a87##a88##a89## \
  a90##a91##a92##a93##a94##a95##a96##a97##a98##a99## \
  a100##a101##a102##a103##a104##a105##a106##a107##a108##a109## \
  a110##a111##a112##a113##a114##a115##a116##a117##a118##a119## \
  a120##a121##a122##a123##a124##a125##a126##a127##a128##a129## \
  a130##a131##a132##a133##a134##a135##a136##a137##a138##a139## \
  a140##a141##a142##a143##a144##a145##a146##a147##a148##a149## \
  a150##a151##a152##a153##a154##a155##a156##a157##a158##a159## \
  a160##a161##a162##a163##a164##a165##a166##a167##a168##a169## \
  a170##a171##a172##a173##a174##a175##a176##a177##a178##a179## \
  a180##a181##a182##a183##a184##a185##a186##a187##a188##a189## \
  a190##a191##a192##a193##a194##a195##a196##a197##a198##a199## \
  a200##a201##a202##a203##a204##a205##a206##a207##a208##a209## \
  a210##a211##a212##a213##a214##a215##a216##a217##a218##a219## \
  a220##a221##a222##a223##a224##a225##a226##a227##a228##a229## \
  a230##a231##a232##a233##a234##a235##a236##a237##a238##a239## \
  a240##a241##a242##a243##a244##a245##a246##a247##a248##a249## \
  a250##a251##a252##a253##a254##a255

#define QSH_NARG_REV(...) \
  QSH_EXPAND(QSH_NARG_256(__VA_ARGS__, \
    255, 254, 253, 252, 251, 250, \
    249, 248, 247, 246, 245, 244, 243, 242, 241, 240, \
    239, 238, 237, 236, 235, 234, 233, 232, 231, 230, \
    229, 228, 227, 226, 225, 224, 223, 222, 221, 220, \
    219, 218, 217, 216, 215, 214, 213, 212, 211, 210, \
    209, 208, 207, 206, 205, 204, 203, 202, 201, 200, \
    199, 198, 197, 196, 195, 194, 193, 192, 191, 190, \
    189, 188, 187, 186, 185, 184, 183, 182, 181, 180, \
    179, 178, 177, 176, 175, 174, 173, 172, 171, 170, \
    169, 168, 167, 166, 165, 164, 163, 162, 161, 160, \
    159, 158, 157, 156, 155, 154, 153, 152, 151, 150, \
    149, 148, 147, 146, 145, 144, 143, 142, 141, 140, \
    139, 138, 137, 136, 135, 134, 133, 132, 131, 130, \
    129, 128, 127, 126, 125, 124, 123, 122, 121, 120, \
    119, 118, 117, 116, 115, 114, 113, 112, 111, 110, \
    109, 108, 107, 106, 105, 104, 103, 102, 101, 100, \
    99, 98, 97, 96, 95, 94, 93, 92, 91, 90, \
    89, 88, 87, 86, 85, 84, 83, 82, 81, 80, \
    79, 78, 77, 76, 75, 74, 73, 72, 71, 70, \
    69, 68, 67, 66, 65, 64, 63, 62, 61, 60, \
    59, 58, 57, 56, 55, 54, 53, 52, 51, 50, \
    49, 48, 47, 46, 45, 44, 43, 42, 41, 40, \
    39, 38, 37, 36, 35, 34, 33, 32, 31, 30, \
    29, 28, 27, 26, 25, 24, 23, 22, 21, 20, \
    19, 18, 17, 16, 15, 14, 13, 12, 11, 10, \
    9,  8,  7,  6,  5,  4,  3,  2,  1,  0) )

#define QSH_NARG_256( \
  a0,a1,a2,a3,a4,a5,a6,a7,a8,a9, \
  a10,a11,a12,a13,a14,a15,a16,a17,a18,a19, \
  a20,a21,a22,a23,a24,a25,a26,a27,a28,a29, \
  a30,a31,a32,a33,a34,a35,a36,a37,a38,a39, \
  a40,a41,a42,a43,a44,a45,a46,a47,a48,a49, \
  a50,a51,a52,a53,a54,a55,a56,a57,a58,a59, \
  a60,a61,a62,a63,a64,a65,a66,a67,a68,a69, \
  a70,a71,a72,a73,a74,a75,a76,a77,a78,a79, \
  a80,a81,a82,a83,a84,a85,a86,a87,a88,a89, \
  a90,a91,a92,a93,a94,a95,a96,a97,a98,a99, \
  a100,a101,a102,a103,a104,a105,a106,a107,a108,a109, \
  a110,a111,a112,a113,a114,a115,a116,a117,a118,a119, \
  a120,a121,a122,a123,a124,a125,a126,a127,a128,a129, \
  a130,a131,a132,a133,a134,a135,a136,a137,a138,a139, \
  a140,a141,a142,a143,a144,a145,a146,a147,a148,a149, \
  a150,a151,a152,a153,a154,a155,a156,a157,a158,a159, \
  a160,a161,a162,a163,a164,a165,a166,a167,a168,a169, \
  a170,a171,a172,a173,a174,a175,a176,a177,a178,a179, \
  a180,a181,a182,a183,a184,a185,a186,a187,a188,a189, \
  a190,a191,a192,a193,a194,a195,a196,a197,a198,a199, \
  a200,a201,a202,a203,a204,a205,a206,a207,a208,a209, \
  a210,a211,a212,a213,a214,a215,a216,a217,a218,a219, \
  a220,a221,a222,a223,a224,a225,a226,a227,a228,a229, \
  a230,a231,a232,a233,a234,a235,a236,a237,a238,a239, \
  a240,a241,a242,a243,a244,a245,a246,a247,a248,a249, \
  a250,a251,a252,a253,a254,a255, a256, ...) a256

#else
#error code not present
#endif

#define QSH_WRAPPER(...) dummy, __VA_ARGS__

/*==============================================================================

  MACRO:  QSH_SUB_MOD

==============================================================================*/
/*!
  Subtract two consecutive timestamps with a given modulus.
*/
/*============================================================================*/
#define QSH_SUB_MOD(t1, t2, m) \
  ( ( (t2) >= (t1) ) ? ( (t2) - (t1) ) : ( (m) - ((t1) - (t2)) ) )

/*! @brief Logging with variable arguments for each message type.
*/
#ifdef FEATURE_QSH_ON_TARGET
  #define QSHI_LOG(client, category, msg_type, summary, ...) \
    msg_type ## _LOG_n(QSH_NARG(__VA_ARGS__), client, category, summary, \
    ##__VA_ARGS__)
#else
  #define QSHI_LOG(client, category, msg_type, summary, ...)
#endif

/*==============================================================================

                    EXTERNAL FUNCTION PROTOTYPES

==============================================================================*/
#endif /* QSH_UTIL_H */
