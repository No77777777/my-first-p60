/*!
  @file
  qtrace_util.h

  @brief
  Definitions needed by qtrace.h but shouldn't be visible to clients.
*/

/*==============================================================================

  Copyright (c) 2016 QUALCOMM Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/utils.mpss/5.4/api/qsh/backend/qtrace_util.h#1 $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
05/09/19   mm      CR 2446793: Fix windows MOB
03/07/19   mm      CR 2411856: Store buffer config during init on MOB
12/07/18   mm      CR 2363373: Enable stubs on linux MOB
10/03/18   rs      CR 2326931: QTrace Common Debug API enhancement
10/10/18   mm      CR 2290332: API to input hash id and parameters
09/10/18   mm      CR 2311501: Support for 64-bit arguments
08/29/18   mm      CR 2304039: Use tag bit to indicate internal-only log
08/27/18   mm      CR 2303750: MOB fix
08/22/18   mm      CR 2300075: Logging cleanup
08/20/18   mm      CR 2299870: Add QTRACE_INT() for internal logs
08/13/18   rs      CR 2295699: Feature Flag to Enable/Disable Qtrace logging
06/19/18   yx      CR 2263684: Ulog to QTrace migration in QSH_LOG
06/18/18   am      CR 2251065: FR51021 QTrace master buffer to use large memory
04/02/18   mm      CR 2206706: QTrace adjustment for QTF on MOB
09/07/17   mm      CR 2106081: Support for removing logs by client
07/03/17   mm      CR 2070787: Support for format strings with %s
02/10/17   mm      CR 1104665: Adding QTRACE_TAGS()
01/12/17   mm      CR 1081130: Initial checkin
==============================================================================*/

#ifndef QTRACE_UTIL_H
#define QTRACE_UTIL_H

/*==============================================================================

                           INCLUDE FILES

==============================================================================*/

#include <comdef.h>
#include <qsh_util.h>
#include <qsh_buffer.h>
#include <mtrace_type1.h>
#include <qtrace_util_autogen.h>

/*==============================================================================

                   EXTERNAL DEFINITIONS AND TYPES

==============================================================================*/

/*!
  Bitmask of modes considered to be offload.
*/
#define QTRACEI_BUFFER_MODE_OFFLOAD \
  ( \
    QTRACE_BUFFER_MODE_PERIODIC | \
    QTRACE_BUFFER_MODE_EVENT | \
    QTRACE_BUFFER_MODE_WRAP \
  )

/*!
  @brief
  Message types as strings to form string passed to tools.
*/
#define QTRACE_LEVEL_DEBUG_STR \
  "0"
#define QTRACE_LEVEL_MED_STR \
  "1"
#define QTRACE_LEVEL_HIGH_STR \
  "2"
#define QTRACE_LEVEL_ERROR_STR \
  "3"

/*!
  @brief
  Declaring storage for common buffer context id.
*/
extern uint8 qtrace_buffer_common[];

/*!
  @brief
  Declaring storage for common buffer context id.
*/
extern uint8 qtrace_buffer_summary[];

/*!
  @brief
  Declaring storage for analysis buffer context id.
*/
extern uint8 qtrace_buffer_analysis[];

/*!
  @brief
  Declaring storage for cmndbg buffer context id.
*/
extern uint8 qtrace_buffer_cmndbg[];

/*!
  @brief
  Declaring storage for profile_hf buffer context id.
*/
extern uint8 qtrace_buffer_profile_hf[];

/*!
  @brief
  Declaring storage for profile_lf buffer context id.
*/
extern uint8 qtrace_buffer_profile_lf[];


/*!
  @brief
  Implementation of corresponding user-facing macro.

  @note
  Works for 0 <= n <= 5 tags.
*/
#define QTRACEI_TAGS(...) \
  QSH_EXPAND(QSH_CONCAT(QTRACEI_TAGS_, QSH_NARG(__VA_ARGS__)) \
    (__VA_ARGS__))

#define QTRACEI_TAGS_0() \
  ""
#define QTRACEI_TAGS_1(t1) \
  t1
#define QTRACEI_TAGS_2(t1, t2) \
  t1 "|" t2
#define QTRACEI_TAGS_3(t1, t2, t3) \
  t1 "|" t2 "|" t3
#define QTRACEI_TAGS_4(t1, t2, t3, t4) \
  t1 "|" t2 "|" t3 "|" t4
#define QTRACEI_TAGS_5(t1, t2, t3, t4, t5) \
  t1 "|" t2 "|" t3 "|" t4 "|" t5

/*!
  @brief
  Implementation of corresponding user-facing macro.
*/
#define QTRACEI_STR_TBL(_var, ...) \
  QSH_EXPAND(QSH_CONCAT(QTRACEI_STR_TBL_, QSH_NARG(__VA_ARGS__)) \
    (_var, QSH_NARG(__VA_ARGS__), __VA_ARGS__))

#define QTRACEI_STR_TBL_N(_var, _count, ...) \
  QSH_EXPAND(QSH_CONCAT(QTRACEI_STR_TBL_, QSH_NARG(__VA_ARGS__)) \
    (_var, _count, __VA_ARGS__))

/*!
  @brief
  Attribute to attach to QTrace string tables.
*/
#ifdef FEATURE_QSH_ON_TARGET
  #define QTRACEI_STR_ATTR \
    __attribute__((section("QTRACE_STR.fmt.rodata"))) ALIGN(1)
#else
  #define QTRACEI_STR_ATTR
#endif /* FEATURE_QSH_ON_TARGET */

/*!
  MTrace constants.
*/
#define QTRACEI_MTRACE_HEADER_SIZE_WORDS \
  (sizeof(mtrace_type1_msg_header_t) / sizeof(uint32))
#define QTRACEI_MTRACE_ARG_COUNT_MAX \
  (13)

/*!
  Expands to qshrink format string.
*/
#define QTRACEI_QSR_STR(_client, _level, _tag_str, _fmt) \
  QSH_STR(__LINE__) ":" \
    _level ## _STR ":" \
    _client ## _STR ":" \
    __FILENAME__ ":" \
    _tag_str ":" \
    _fmt

/*!
  Implementation of user-facing macro.

  @note
  Can be used explicitly by clients for logging on MOB without bypassing
  normal qtrace.
*/
#if defined(FEATURE_QSH_STUBS) || defined(FEATURE_HEXSIM)
  #define QTRACE_MOB( \
    _context_id, \
    _client, \
    _level, \
    _subs_id, \
    _tag_str, \
    _fmt, \
    ... \
  ) \
    QTRACEI_MOB_WRAPPER(_context_id, _client, _level, _subs_id, _tag_str, \
      _fmt, ##__VA_ARGS__)

  /*!
    Wrapper needed to ensure correct arg count for 64-bit args.
  */
  #define QTRACEI_MOB_WRAPPER(_context_id, _client, _level, _subs_id, \
    _tag_str, _fmt, ...) \
    QSH_EXPAND(QSH_CONCAT(QTRACEI_MOB_, QSH_NARG(__VA_ARGS__))( \
      (_context_id), \
      (_client), \
      ((uint8) (_subs_id)) & 0x3, \
      QTRACEI_QSR_STR(_client, _level, _tag_str, _fmt), \
      ##__VA_ARGS__))

  #define QTRACEI_MOB_0(_context_id, _client, _subs_id, _qsr) \
    qtracei_flush_mob(_context_id, _client, _subs_id, _qsr, 0)
  #define QTRACEI_MOB_1(_context_id, _client, _subs_id, _qsr, \
    _a0) \
    qtracei_flush_mob(_context_id, _client, _subs_id, _qsr, 1, \
      (uint32)(_a0))
  #define QTRACEI_MOB_2(_context_id, _client, _subs_id, _qsr, \
    _a0, _a1) \
    qtracei_flush_mob(_context_id, _client, _subs_id, _qsr, 2, \
      (uint32)(_a0), (uint32)(_a1))
  #define QTRACEI_MOB_3(_context_id, _client, _subs_id, _qsr, \
    _a0, _a1, _a2) \
    qtracei_flush_mob(_context_id, _client, _subs_id, _qsr, 3, \
      (uint32)(_a0), (uint32)(_a1), (uint32)(_a2))
  #define QTRACEI_MOB_4(_context_id, _client, _subs_id, _qsr, \
    _a0, _a1, _a2, _a3) \
    qtracei_flush_mob(_context_id, _client, _subs_id, _qsr, 4, \
      (uint32)(_a0), (uint32)(_a1), (uint32)(_a2), (uint32)(_a3))
  #define QTRACEI_MOB_5(_context_id, _client, _subs_id, _qsr, \
    _a0, _a1, _a2, _a3, _a4) \
    qtracei_flush_mob(_context_id, _client, _subs_id, _qsr, 5, \
      (uint32)(_a0), (uint32)(_a1), (uint32)(_a2), (uint32)(_a3), \
      (uint32)(_a4))
  #define QTRACEI_MOB_6(_context_id, _client, _subs_id, _qsr, \
    _a0, _a1, _a2, _a3, _a4, _a5) \
    qtracei_flush_mob(_context_id, _client, _subs_id, _qsr, 6, \
      (uint32)(_a0), (uint32)(_a1), (uint32)(_a2), (uint32)(_a3), \
      (uint32)(_a4), (uint32)(_a5))
  #define QTRACEI_MOB_7(_context_id, _client, _subs_id, _qsr, \
    _a0, _a1, _a2, _a3, _a4, _a5, _a6) \
    qtracei_flush_mob(_context_id, _client, _subs_id, _qsr, 7, \
      (uint32)(_a0), (uint32)(_a1), (uint32)(_a2), (uint32)(_a3), \
      (uint32)(_a4), (uint32)(_a5), (uint32)(_a6))
  #define QTRACEI_MOB_8(_context_id, _client, _subs_id, _qsr, \
    _a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7) \
    qtracei_flush_mob(_context_id, _client, _subs_id, _qsr, 8, \
      (uint32)(_a0), (uint32)(_a1), (uint32)(_a2), (uint32)(_a3), \
      (uint32)(_a4), (uint32)(_a5), (uint32)(_a6), (uint32)(_a7))
  #define QTRACEI_MOB_9(_context_id, _client, _subs_id, _qsr, \
    _a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8) \
    qtracei_flush_mob(_context_id, _client, _subs_id, _qsr, 9, \
      (uint32)(_a0), (uint32)(_a1), (uint32)(_a2), (uint32)(_a3), \
      (uint32)(_a4), (uint32)(_a5), (uint32)(_a6), (uint32)(_a7), \
      (uint32)(_a8))
  #define QTRACEI_MOB_10(_context_id, _client, _subs_id, _qsr, \
    _a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9) \
    qtracei_flush_mob(_context_id, _client, _subs_id, _qsr, 10, \
      (uint32)(_a0), (uint32)(_a1), (uint32)(_a2), (uint32)(_a3), \
      (uint32)(_a4), (uint32)(_a5), (uint32)(_a6), (uint32)(_a7), \
      (uint32)(_a8), (uint32)(_a9))
  #define QTRACEI_MOB_11(_context_id, _client, _subs_id, _qsr, \
    _a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9, _a10) \
    qtracei_flush_mob(_context_id, _client, _subs_id, _qsr, 11, \
      (uint32)(_a0), (uint32)(_a1), (uint32)(_a2), (uint32)(_a3), \
      (uint32)(_a4), (uint32)(_a5), (uint32)(_a6), (uint32)(_a7), \
      (uint32)(_a8), (uint32)(_a9), (uint32)(_a10))
  #define QTRACEI_MOB_12(_context_id, _client, _subs_id, _qsr, \
    _a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9, _a10, _a11) \
    qtracei_flush_mob(_context_id, _client, _subs_id, _qsr, 12, \
      (uint32)(_a0), (uint32)(_a1), (uint32)(_a2), (uint32)(_a3), \
      (uint32)(_a4), (uint32)(_a5), (uint32)(_a6), (uint32)(_a7), \
      (uint32)(_a8), (uint32)(_a9), (uint32)(_a10), (uint32)(_a11))
#endif /* defined(FEATURE_QSH_STUBS) || defined(FEATURE_HEXSIM) */

/*!
  Macro to type-cast all arguments to void.
*/
#define QTRACEI_UNUSED_N(...) \
  QSH_EXPAND(QSH_CONCAT(QTRACEI_UNUSED_, QSH_NARG(__VA_ARGS__)) \
    (__VA_ARGS__))

#define QTRACEI_UNUSED(_var) \
  (void) (_var)

#define QTRACEI_UNUSED_0()
#define QTRACEI_UNUSED_1(_a0) \
  QTRACEI_UNUSED(_a0)
#define QTRACEI_UNUSED_2(_a0, _a1) \
  QTRACEI_UNUSED_1(_a0), QTRACEI_UNUSED(_a1)
#define QTRACEI_UNUSED_3(_a0, _a1, _a2) \
  QTRACEI_UNUSED_2(_a0, _a1), QTRACEI_UNUSED(_a2)
#define QTRACEI_UNUSED_4(_a0, _a1, _a2, _a3) \
  QTRACEI_UNUSED_3(_a0, _a1, _a2), QTRACEI_UNUSED(_a3)
#define QTRACEI_UNUSED_5(_a0, _a1, _a2, _a3, _a4) \
  QTRACEI_UNUSED_4(_a0, _a1, _a2, _a3), QTRACEI_UNUSED(_a4)
#define QTRACEI_UNUSED_6(_a0, _a1, _a2, _a3, _a4, _a5) \
  QTRACEI_UNUSED_5(_a0, _a1, _a2, _a3, _a4), QTRACEI_UNUSED(_a5)
#define QTRACEI_UNUSED_7(_a0, _a1, _a2, _a3, _a4, _a5, _a6) \
  QTRACEI_UNUSED_6(_a0, _a1, _a2, _a3, _a4, _a5), QTRACEI_UNUSED(_a6)
#define QTRACEI_UNUSED_8(_a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7) \
  QTRACEI_UNUSED_7(_a0, _a1, _a2, _a3, _a4, _a5, _a6), QTRACEI_UNUSED(_a7)
#define QTRACEI_UNUSED_9(_a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8) \
  QTRACEI_UNUSED_8(_a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7), QTRACEI_UNUSED(_a8)
#define QTRACEI_UNUSED_10(_a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9) \
  QTRACEI_UNUSED_9(_a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8), QTRACEI_UNUSED(_a9)
#define QTRACEI_UNUSED_11(_a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9, _a10) \
  QTRACEI_UNUSED_10(_a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9), QTRACEI_UNUSED(_a10)
#define QTRACEI_UNUSED_12(_a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9, _a10, _a11) \
  QTRACEI_UNUSED_11(_a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9, _a10), QTRACEI_UNUSED(_a11)

/*!
  Macros to populate tag.
*/
#define QTRACEI_TAG_SET(_tag, _subs_id) \
  do { \
    (_tag).fields.subs_id = ((uint8) (_subs_id)) & 0x3; \
    (_tag).fields.internal = FALSE; \
    (_tag).fields.type = QSH_BUFFER_TYPE_MESSAGE; \
    (_tag).fields.reserved = 0; \
  } while (0)

#define QTRACEI_TAG_SET_INT(_tag, _subs_id) \
  do { \
    (_tag).fields.subs_id = ((uint8) (_subs_id)) & 0x3; \
    (_tag).fields.internal = TRUE; \
    (_tag).fields.type = QSH_BUFFER_TYPE_MESSAGE; \
    (_tag).fields.reserved = 0; \
  } while (0)

/*!
  Internal macros called by per-client macros, if enabled.

  @note
  Format string must be included in variadic args for QTRACE_ARG64() to expand
  correctly on non-windows MOB; excluded from variadic args on windows MOB.
*/
#ifndef T_WINNT
  #define QTRACEI( \
    _buffer_ptr, \
    _client, \
    _level, \
    _subs_id, \
    _tag_str, \
    ... \
  ) \
    do { \
      qsh_buffer_tag_u _tag; \
      QTRACEI_TAG_SET(_tag, _subs_id); \
      QTRACEI_BACKEND(_buffer_ptr, _client, _level, _subs_id, \
        _tag_str, _tag.full, __VA_ARGS__); \
    } while (0)

  #define QTRACEI_S( \
    _buffer_ptr, \
    _client, \
    _level, \
    _subs_id, \
    _tag_str, \
    ... \
  ) \
    do { \
      qsh_buffer_tag_u _tag; \
      QTRACEI_TAG_SET(_tag, _subs_id); \
      QTRACEI_BACKEND_S(_buffer_ptr, _client, _level, _subs_id, \
        _tag_str, _tag.full, __VA_ARGS__); \
    } while (0)

  #define QTRACEI_INT( \
    _buffer_ptr, \
    _client, \
    _level, \
    _subs_id, \
    _tag_str, \
    ... \
  ) \
    do { \
      qsh_buffer_tag_u _tag; \
      QTRACEI_TAG_SET_INT(_tag, _subs_id); \
      QTRACEI_BACKEND_INT(_buffer_ptr, _client, _level, _subs_id, \
        _tag_str, _tag.full, __VA_ARGS__); \
    } while (0)

  #define QTRACEI_INT_S( \
    _buffer_ptr, \
    _client, \
    _level, \
    _subs_id, \
    _tag_str, \
    ... \
  ) \
    do { \
      qsh_buffer_tag_u _tag; \
      QTRACEI_TAG_SET_INT(_tag, _subs_id); \
      QTRACEI_BACKEND_INT_S(_buffer_ptr, _client, _level, _subs_id, \
        _tag_str, _tag.full, __VA_ARGS__); \
    } while (0)
#else
  #error code not present
#endif /* T_WINNT */

#define QTRACEI_QSRHASH( \
  _buffer_ptr, \
  _subs_id, \
  _qsr_hash, \
  ... \
) \
  do { \
    qsh_buffer_tag_u _tag; \
    QTRACEI_TAG_SET(_tag, _subs_id); \
    MTRACE_TYPE1_BIN_IND( \
      &((_buffer_ptr)->mtrace_desc_ptr), \
      _tag.full, \
      _qsr_hash, \
      ##__VA_ARGS__); \
  } while (0)

#define QTRACEI_QSRHASH_INT( \
  _buffer_ptr, \
  _subs_id, \
  _qsr_hash, \
  ... \
) \
  do { \
    qsh_buffer_tag_u _tag; \
    QTRACEI_TAG_SET_INT(_tag, _subs_id); \
    MTRACE_TYPE1_BIN_IND( \
      &((_buffer_ptr)->mtrace_desc_ptr), \
      _tag.full, \
      _qsr_hash, \
      ##__VA_ARGS__); \
  } while (0)

/*!
  Back-end macro used internally to QSH.
*/
#ifndef FEATURE_QSH_STUBS
  #define QTRACEI_BACKEND( \
    _buffer_ptr, \
    _client, \
    _level, \
    _subs_id, \
    _tag_str, \
    _tag, \
    _summary, \
    ... \
  ) \
    MTRACE_TYPE1_STR_IND( \
      &((_buffer_ptr)->mtrace_desc_ptr), \
      _tag, \
      QTRACEI_QSR_STR(_client, _level, _tag_str, _summary), \
      ##__VA_ARGS__)

  #define QTRACEI_BACKEND_S( \
    _buffer_ptr, \
    _client, \
    _level, \
    _subs_id, \
    _tag_str, \
    _tag, \
    _summary, \
    ... \
  ) \
    MTRACE_TYPE1_STR_NOCHECK_IND( \
      &((_buffer_ptr)->mtrace_desc_ptr), \
      _tag, \
      QTRACEI_QSR_STR(_client, _level, _tag_str, _summary), \
      ##__VA_ARGS__)

  #define QTRACEI_BACKEND_INT( \
      _buffer_ptr, \
      _client, \
      _level, \
      _subs_id, \
      _tag_str, \
      _tag, \
      _summary, \
      ... \
  ) \
    MTRACE_TYPE1_STR_INTERNAL_IND( \
      &((_buffer_ptr)->mtrace_desc_ptr), \
      _tag, \
      QTRACEI_QSR_STR(_client, _level, _tag_str, _summary), \
      ##__VA_ARGS__)

  #define QTRACEI_BACKEND_INT_S( \
    _buffer_ptr, \
    _client, \
    _level, \
    _subs_id, \
    _tag_str, \
    _tag, \
    _summary, \
    ... \
  ) \
    MTRACE_TYPE1_STR_NOCHECK_INTERNAL_IND( \
      &((_buffer_ptr)->mtrace_desc_ptr), \
      _tag, \
      QTRACEI_QSR_STR(_client, _level, _tag_str, _summary), \
      ##__VA_ARGS__)
#else
  #define QTRACEI_BACKEND( \
    _buffer_ptr, \
    _client, \
    _level, \
    _subs_id, \
    _tag_str, \
    _tag, \
    _summary, \
    ... \
  ) \
    QTRACEI_MOB_WRAPPER( \
      _buffer_ptr, \
      _client, \
      _level, \
      _subs_id, \
      _tag_str, \
      _summary, \
      ##__VA_ARGS__)

  #define QTRACEI_BACKEND_S( \
    _buffer_ptr, \
    _client, \
    _level, \
    _subs_id, \
    _tag_str, \
    _tag, \
    _summary, \
    ... \
  ) \
    QTRACEI_BACKEND(_buffer_ptr, _client, _level, _subs_id, _tag_str, _tag, \
      _summary, ##__VA_ARGS__)

  #define QTRACEI_BACKEND_INT( \
    _buffer_ptr, \
    _client, \
    _level, \
    _subs_id, \
    _tag_str, \
    _tag, \
    _summary, \
    ... \
  ) \
    QTRACEI_BACKEND(_buffer_ptr, _client, _level, _subs_id, _tag_str, _tag, \
      _summary, ##__VA_ARGS__)

  #define QTRACEI_BACKEND_INT_S( \
    _buffer_ptr, \
    _client, \
    _level, \
    _subs_id, \
    _tag_str, \
    _tag, \
    _summary, \
    ... \
  ) \
    QTRACEI_BACKEND(_buffer_ptr, _client, _level, _subs_id, _tag_str, _tag, \
      _summary, ##__VA_ARGS__)
#endif /* FEATURE_QSH_STUBS */

/*!
  Dummy macro used if any clients are disabled.
*/
#define QTRACEI_DUMMY( \
  _buffer_context_id, \
  _client, \
  _level, \
  _subs_id, \
  _tag_str, \
  _summary, \
  ... \
) \
  do { \
    (void) (_buffer_context_id); \
    (void) (_client); \
    (void) (_level); \
    (void) ((int) (_subs_id)); \
    (void) (_tag_str); \
    (void) (_summary); \
    QTRACEI_UNUSED_N(__VA_ARGS__); \
  } while (0)

#endif /* QTRACE_UTIL_H */
