/*!
  @file
  mtrace_type1.h

  @brief
  MCS generic trace module - format type 1.

*/

/*===========================================================================

  Copyright (c) 2019 Qualcomm Technologies Incorporated. All Rights Reserved

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

  $Header: //components/rel/mcs.mpss/5.2/api/mtrace_type1.h#1 $

===========================================================================*/

#ifndef MTRACE_TYPE1_H
#define MTRACE_TYPE1_H

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/

#include "mcs_comdef.h"

#ifndef __ASSEMBLER__
#include <atomic_ops.h>
#endif


/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/

/*! @brief Cache management block size for type1 mtrace. */
#define MTRACE_TYPE1_BLOCK_SIZE_BYTES                 64
#define MTRACE_TYPE1_BLOCK_SIZE_WORDS                 (MTRACE_TYPE1_BLOCK_SIZE_BYTES/4)


/*! @brief Flags for type1 descriptor (OR together). */
#define MTRACE_TYPE1_FLAG_CACHE_WT                    (0<<0)  /* Write-through or uncached */
#define MTRACE_TYPE1_FLAG_CACHE_WB                    (1<<0)  /* Write-back cached */
#define MTRACE_TYPE1_FLAG_LEN_VAR                     (0<<1)  /* Variable-length message */
#define MTRACE_TYPE1_FLAG_LEN_FIX32                   (1<<1)  /* Fixed-length 32 byte message */
//#define MTRACE_TYPE1_FLAG_LEN_TBD                     (2..3<<1)  /* TBD */
#define MTRACE_TYPE1_FLAG_STR_NOKEEP                  (0<<3)  /* arg0 string is stripped from image */
#define MTRACE_TYPE1_FLAG_STR_KEEP                    (1<<3)  /* arg0 string is present in image */
#define MTRACE_TYPE1_FLAG_FMT_BIN                     (0<<4)  /* arg0 is binary */
#define MTRACE_TYPE1_FLAG_FMT_STR                     (1<<4)  /* arg0 is a printf-format string */
#define MTRACE_TYPE1_FLAG_FMT_QSTR                    (2<<4)  /* arg0 is a QShrink string from CREATE_QSTR macro */
//#define MTRACE_TYPE1_FLAG_FMT_TBD                     (3<<4)  /* TBD */
#define MTRACE_TYPE1_FLAG_DESC_IND                    (1<<6)  /* Indirect descriptor API & callback used */
//#define MTRACE_TYPE1_FLAG_TBD                       (x<<7)  /* TBD */
#define MTRACE_TYPE1_FLAG_TAG_SUB                     (1<<8)  /* Tag contains subscriber ID in tag bit[1:0] */
//#define MTRACE_TYPE1_FLAG_TBD                       (x<<9..31)  /* TBD */


// *** Backwards compatibility flags, remove when clients are converted to above ***
#define MTRACE_TYPE1_FLAG_WT                          MTRACE_TYPE1_FLAG_CACHE_WT
#define MTRACE_TYPE1_FLAG_WB                          MTRACE_TYPE1_FLAG_CACHE_WB
#define MTRACE_TYPE1_FLAG_STR                         MTRACE_TYPE1_FLAG_STR_NOKEEP
#define MTRACE_TYPE1_FLAG_VAR                         MTRACE_TYPE1_FLAG_LEN_VAR
// *** End of backwards compatibility block ***


/*! @brief Generic MTrace type 1 variable argument macro.
*/
#define MTRACE_TYPE1_BIN(_desc, _tag, ...) do {                                               \
  MTRACE_TYPE1_FUNC(_MTRACE_TYPE1_CHECK_BIN,mtrace_type1_var_wb,_tag,_desc, ## __VA_ARGS__);  \
} while (0)

#define MTRACE_TYPE1_BIN_IND(_desc, _tag, ...) do {                                                   \
  MTRACE_TYPE1_FUNC(_MTRACE_TYPE1_CHECK_BIN_IND,mtrace_type1_var_wb_ind,_tag,_desc, ## __VA_ARGS__);  \
} while (0)

/*! @brief MTrace type 1 variable argument macro, where printf() style
           tracing is desired, arg0 being the format string.
*/
#define MTRACE_TYPE1_STR(_desc, _tag, _fmt, ...) do {                                                 \
  static const char _fmtstr[] MTRACE_TYPE1_STRING_CONST_SECTION ALIGN(1) = _fmt;                      \
  MTRACE_TYPE1_FUNC(_MTRACE_TYPE1_CHECK_STR,mtrace_type1_var_wb,_tag,_desc,_fmtstr, ## __VA_ARGS__);  \
} while (0)

#define MTRACE_TYPE1_STR_IND(_desc, _tag, _fmt, ...) do {                                                     \
  static const char _fmtstr[] MTRACE_TYPE1_STRING_CONST_SECTION ALIGN(1) = _fmt;                              \
  MTRACE_TYPE1_FUNC(_MTRACE_TYPE1_CHECK_STR_IND,mtrace_type1_var_wb_ind,_tag,_desc,_fmtstr, ## __VA_ARGS__);  \
} while (0)

#define MTRACE_TYPE1_STR_INTERNAL(_desc, _tag, _fmt, ...) do {                                        \
  static const char _fmtstr[] MTRACE_TYPE1_STRING_INTERNAL_CONST_SECTION ALIGN(1) = _fmt;             \
  MTRACE_TYPE1_FUNC(_MTRACE_TYPE1_CHECK_STR,mtrace_type1_var_wb,_tag,_desc,_fmtstr, ## __VA_ARGS__);  \
} while (0)

#define MTRACE_TYPE1_STR_INTERNAL_IND(_desc, _tag, _fmt, ...) do {                                            \
  static const char _fmtstr[] MTRACE_TYPE1_STRING_INTERNAL_CONST_SECTION ALIGN(1) = _fmt;                     \
  MTRACE_TYPE1_FUNC(_MTRACE_TYPE1_CHECK_STR_IND,mtrace_type1_var_wb_ind,_tag,_desc,_fmtstr, ## __VA_ARGS__);  \
} while (0)

/*! @brief MTrace type 1 variable argument macro, where printf() style
           tracing is desired, arg0 being the format string. This
           macro bypasses format string checking, if that is desired.
*/
#define MTRACE_TYPE1_STR_NOCHECK(_desc, _tag, _fmt, ...) do {                                         \
  static const char _fmtstr[] MTRACE_TYPE1_STRING_CONST_SECTION ALIGN(1) = _fmt;                      \
  MTRACE_TYPE1_FUNC(_MTRACE_TYPE1_CHECK_BIN,mtrace_type1_var_wb,_tag,_desc,_fmtstr, ## __VA_ARGS__);  \
} while (0)

#define MTRACE_TYPE1_STR_NOCHECK_IND(_desc, _tag, _fmt, ...) do {                                             \
  static const char _fmtstr[] MTRACE_TYPE1_STRING_CONST_SECTION ALIGN(1) = _fmt;                              \
  MTRACE_TYPE1_FUNC(_MTRACE_TYPE1_CHECK_BIN_IND,mtrace_type1_var_wb_ind,_tag,_desc,_fmtstr, ## __VA_ARGS__);  \
} while (0)

#define MTRACE_TYPE1_STR_NOCHECK_INTERNAL(_desc, _tag, _fmt, ...) do {                                \
  static const char _fmtstr[] MTRACE_TYPE1_STRING_INTERNAL_CONST_SECTION ALIGN(1) = _fmt;             \
  MTRACE_TYPE1_FUNC(_MTRACE_TYPE1_CHECK_BIN,mtrace_type1_var_wb,_tag,_desc,_fmtstr, ## __VA_ARGS__);  \
} while (0)

#define MTRACE_TYPE1_STR_NOCHECK_INTERNAL_IND(_desc, _tag, _fmt, ...) do {                                    \
  static const char _fmtstr[] MTRACE_TYPE1_STRING_INTERNAL_CONST_SECTION ALIGN(1) = _fmt;                     \
  MTRACE_TYPE1_FUNC(_MTRACE_TYPE1_CHECK_BIN_IND,mtrace_type1_var_wb_ind,_tag,_desc,_fmtstr, ## __VA_ARGS__);  \
} while (0)

/*! @brief
    Create a format string compatible with QShrink 4.0 and debugging tools
    designed to parse QTrace/Mtrace type 1 string records

    @detail
    The string passed to MTRACE_TYPE1_STR() will be placed in a
    dedicated section and generally stripped from the final image
    by the Qshrink tool and a database of stripped strings will be
    created for tools to reconstruct off-target.  The format of the
    string is colon-delimited as follows:

    <line_number>:<level>:<client_name>:<filename>:<text_tag>:<format_string>

      * line_number and filename will be populated automatically by macro

      level is a number [0-3] to be stringified: 0=DEBUG, 1=MED, 2=HIGH, 3=ERROR

      client_name is arbitrary subsystem name, e.g. "LFW" or "ML1_SCHED"

      text_tag is an arbitrary pipe-delimited string up to 32 chars, uppercased
      by Qshrink. Generally displayed by the tools in square brackets, and
      used as another way of searching and collating messages across subsystems
      based upon some common "feature" name or such.  Example:  "4G_ULL|30LAYERS"

      format_string is the usual printf-style string specifying both
      information and specifying subsequent arguments

*/
#ifdef __FILENAME__
/* __FILENAME__ is base filename passed by SCons */
#define MTRACE_TYPE1_CREATE_QSTR(_client,_level,_texttag,_fmt)  \
  MACRO_STRINGIFY(__LINE__) ":" MACRO_STRINGIFY(_level) ":"     \
  _client ":" __FILENAME__ ":" _texttag ":" _fmt
#else
/* Default to __FILE__ if we don't have __FILENAME__ */
#define MTRACE_TYPE1_CREATE_QSTR(_client,_level,_texttag,_fmt)  \
  MACRO_STRINGIFY(__LINE__) ":" MACRO_STRINGIFY(_level) ":"     \
  _client ":" __FILE__ ":" _texttag ":" _fmt
#endif


/*! @brief Section where Mtrace string constant data is placed.
    This can be set before including mtrace.h to override. */
#ifndef MTRACE_TYPE1_STRING_CONST_SECTION
#define MTRACE_TYPE1_STRING_CONST_SECTION  __attribute__((section("MTRACE_TYPE1_STR.fmt.rodata")))
#endif

/*! @brief Section where internal-only Mtrace string constant data is placed.
    These strings will be stripped from external-facing builds.
    This can be set before including mtrace.h to override. */
#ifndef MTRACE_TYPE1_STRING_INTERNAL_CONST_SECTION
#define MTRACE_TYPE1_STRING_INTERNAL_CONST_SECTION  __attribute__((section("MTRACE_TYPE1_STR.fmt.rodata.internal")))
#endif

/*===========================================================================
              INTERNAL HELPER MACROS, NOT TO BE USED EXTERNALLY
===========================================================================*/

/*! @brief Pack "header" argument for mtrace_type1 calls. */
#define MTRACE_TYPE1_HEADER(_num_args, _tag) \
  (uint32)((((_num_args) + 2) & 0xF) | ((1) << 4) | ((_tag) << 8))


/*! @brief Call the C trace function with a variable number of arguments.
           This may actually resolve via several layers of macros to perform
           useful compile-time type and format string checking, but will
           ultimately resolve to a function call to an appropriate mtrace type1
           function.
 */
#define MTRACE_TYPE1_FUNC(_checkfn,_apifn,_tag,_desc,...)                   \
  MACRO_EXPAND(                                                             \
                MACRO_CONCAT(_MTRACE_TYPE1_,MACRO_COUNT_ARGS(__VA_ARGS__))  \
                  ( _checkfn,_apifn,                                        \
                    MACRO_COUNT_ARGS(__VA_ARGS__),                          \
                    _tag,_desc, ## __VA_ARGS__ )                            \
              )


/*!@{
    @brief Explicit argument count wrappers, constructed by client-facing variadic
           MTRACE_TYPE*_*() macros.  These are not to be called directly by clients.
*/

/* First level of macro expansion from MTRACE_TYPE1_BIN/STR().
   Builds the header word and unrolls and typecasts the variadic macro arguments */
#define _MTRACE_TYPE1_0(_checkfn,_apifn,_argc,_tag,_desc,...) \
  _MTRACE_TYPE1_VAR_WB_0(_checkfn,MACRO_CONCAT(_apifn,_0),MTRACE_TYPE1_HEADER(_argc,_tag),_desc)
#define _MTRACE_TYPE1_1(_checkfn,_apifn,_argc,_tag,_desc,_a0) \
  _MTRACE_TYPE1_VAR_WB_1(_checkfn,MACRO_CONCAT(_apifn,_1),MTRACE_TYPE1_HEADER(_argc,_tag),_desc,_a0)
#define _MTRACE_TYPE1_2(_checkfn,_apifn,_argc,_tag,_desc,_a0,_a1) \
  _MTRACE_TYPE1_VAR_WB_2(_checkfn,MACRO_CONCAT(_apifn,_2),MTRACE_TYPE1_HEADER(_argc,_tag),_desc,_a0,(unsigned)(_a1))
#define _MTRACE_TYPE1_3(_checkfn,_apifn,_argc,_tag,_desc,_a0,_a1,_a2) \
  _MTRACE_TYPE1_VAR_WB_3(_checkfn,MACRO_CONCAT(_apifn,_3),MTRACE_TYPE1_HEADER(_argc,_tag),_desc,_a0,(unsigned)(_a1),(unsigned)(_a2))
#define _MTRACE_TYPE1_4(_checkfn,_apifn,_argc,_tag,_desc,_a0,_a1,_a2,_a3) \
  _MTRACE_TYPE1_VAR_WB_4(_checkfn,MACRO_CONCAT(_apifn,_4),MTRACE_TYPE1_HEADER(_argc,_tag),_desc,_a0,(unsigned)(_a1),(unsigned)(_a2),(unsigned)(_a3))
#define _MTRACE_TYPE1_5(_checkfn,_apifn,_argc,_tag,_desc,_a0,_a1,_a2,_a3,_a4) \
  _MTRACE_TYPE1_VAR_WB_5(_checkfn,MACRO_CONCAT(_apifn,_5),MTRACE_TYPE1_HEADER(_argc,_tag),_desc,_a0,(unsigned)(_a1),(unsigned)(_a2),(unsigned)(_a3),(unsigned)(_a4))
#define _MTRACE_TYPE1_6(_checkfn,_apifn,_argc,_tag,_desc,_a0,_a1,_a2,_a3,_a4,_a5) \
  _MTRACE_TYPE1_VAR_WB_N(_checkfn,MACRO_CONCAT(_apifn,_N),MTRACE_TYPE1_HEADER(_argc,_tag),_desc,_a0,(unsigned)(_a1),(unsigned)(_a2),(unsigned)(_a3),(unsigned)(_a4),(unsigned)(_a5))
#define _MTRACE_TYPE1_7(_checkfn,_apifn,_argc,_tag,_desc,_a0,_a1,_a2,_a3,_a4,_a5,_a6) \
  _MTRACE_TYPE1_VAR_WB_N(_checkfn,MACRO_CONCAT(_apifn,_N),MTRACE_TYPE1_HEADER(_argc,_tag),_desc,_a0,(unsigned)(_a1),(unsigned)(_a2),(unsigned)(_a3),(unsigned)(_a4),(unsigned)(_a5),(unsigned)(_a6))
#define _MTRACE_TYPE1_8(_checkfn,_apifn,_argc,_tag,_desc,_a0,_a1,_a2,_a3,_a4,_a5,_a6,_a7) \
  _MTRACE_TYPE1_VAR_WB_N(_checkfn,MACRO_CONCAT(_apifn,_N),MTRACE_TYPE1_HEADER(_argc,_tag),_desc,_a0,(unsigned)(_a1),(unsigned)(_a2),(unsigned)(_a3),(unsigned)(_a4),(unsigned)(_a5),(unsigned)(_a6),(unsigned)(_a7))
#define _MTRACE_TYPE1_9(_checkfn,_apifn,_argc,_tag,_desc,_a0,_a1,_a2,_a3,_a4,_a5,_a6,_a7,_a8) \
  _MTRACE_TYPE1_VAR_WB_N(_checkfn,MACRO_CONCAT(_apifn,_N),MTRACE_TYPE1_HEADER(_argc,_tag),_desc,_a0,(unsigned)(_a1),(unsigned)(_a2),(unsigned)(_a3),(unsigned)(_a4),(unsigned)(_a5),(unsigned)(_a6),(unsigned)(_a7),(unsigned)(_a8))
#define _MTRACE_TYPE1_10(_checkfn,_apifn,_argc,_tag,_desc,_a0,_a1,_a2,_a3,_a4,_a5,_a6,_a7,_a8,_a9) \
  _MTRACE_TYPE1_VAR_WB_N(_checkfn,MACRO_CONCAT(_apifn,_N),MTRACE_TYPE1_HEADER(_argc,_tag),_desc,_a0,(unsigned)(_a1),(unsigned)(_a2),(unsigned)(_a3),(unsigned)(_a4),(unsigned)(_a5),(unsigned)(_a6),(unsigned)(_a7),(unsigned)(_a8),(unsigned)(_a9))
#define _MTRACE_TYPE1_11(_checkfn,_apifn,_argc,_tag,_desc,_a0,_a1,_a2,_a3,_a4,_a5,_a6,_a7,_a8,_a9,_a10) \
  _MTRACE_TYPE1_VAR_WB_N(_checkfn,MACRO_CONCAT(_apifn,_N),MTRACE_TYPE1_HEADER(_argc,_tag),_desc,_a0,(unsigned)(_a1),(unsigned)(_a2),(unsigned)(_a3),(unsigned)(_a4),(unsigned)(_a5),(unsigned)(_a6),(unsigned)(_a7),(unsigned)(_a8),(unsigned)(_a9),(unsigned)(_a10))
#define _MTRACE_TYPE1_12(_checkfn,_apifn,_argc,_tag,_desc,_a0,_a1,_a2,_a3,_a4,_a5,_a6,_a7,_a8,_a9,_a10,_a11) \
  _MTRACE_TYPE1_VAR_WB_N(_checkfn,MACRO_CONCAT(_apifn,_N),MTRACE_TYPE1_HEADER(_argc,_tag),_desc,_a0,(unsigned)(_a1),(unsigned)(_a2),(unsigned)(_a3),(unsigned)(_a4),(unsigned)(_a5),(unsigned)(_a6),(unsigned)(_a7),(unsigned)(_a8),(unsigned)(_a9),(unsigned)(_a10),(unsigned)(_a11))
#define _MTRACE_TYPE1_13(_checkfn,_apifn,_argc,_tag,_desc,_a0,_a1,_a2,_a3,_a4,_a5,_a6,_a7,_a8,_a9,_a10,_a11,_a12) \
  _MTRACE_TYPE1_VAR_WB_N(_checkfn,MACRO_CONCAT(_apifn,_N),MTRACE_TYPE1_HEADER(_argc,_tag),_desc,_a0,(unsigned)(_a1),(unsigned)(_a2),(unsigned)(_a3),(unsigned)(_a4),(unsigned)(_a5),(unsigned)(_a6),(unsigned)(_a7),(unsigned)(_a8),(unsigned)(_a9),(unsigned)(_a10),(unsigned)(_a11),(unsigned)(_a12))

/* Second level of macro expansion from MTRACE_TYPE1_BIN/STR().
   This first calls a dummy inline function specified by the
   top-level macro to check the descriptor type + format string
   (if _STR()) at compile time, and should not generate any code.
   After the dummy inline checking function, the real mtrace
   C function is called with the appropriate arguments. */
#define _MTRACE_TYPE1_VAR_WB_0(_checkfn,_apifn,_header,_desc) \
   _apifn(_header,_desc)
#define _MTRACE_TYPE1_VAR_WB_1(_checkfn,_apifn,_header,_desc,_a0) \
   _checkfn(_desc,_a0);                                           \
   _apifn((unsigned)(_a0),_header,_desc)
#define _MTRACE_TYPE1_VAR_WB_2(_checkfn,_apifn,_header,_desc,_a0,_a1) \
   _checkfn(_desc,_a0,_a1);                                           \
   _apifn((unsigned)(_a0),_a1,_header,_desc)
#define _MTRACE_TYPE1_VAR_WB_3(_checkfn,_apifn,_header,_desc,_a0,_a1,_a2) \
   _checkfn(_desc,_a0,_a1,_a2);                                           \
   _apifn((unsigned)(_a0),_a1,_a2,_header,_desc)
#define _MTRACE_TYPE1_VAR_WB_4(_checkfn,_apifn,_header,_desc,_a0,_a1,_a2,_a3) \
   _checkfn(_desc,_a0,_a1,_a2,_a3);                                           \
   _apifn((unsigned)(_a0),_a1,_a2,_a3,_header,_desc)
#define _MTRACE_TYPE1_VAR_WB_5(_checkfn,_apifn,_header,_desc,_a0,_a1,_a2,_a3,_a4) \
   _checkfn(_desc,_a0,_a1,_a2,_a3,_a4);                                           \
   _apifn((unsigned)(_a0),_a1,_a2,_a3,_a4,_header,_desc)
#define _MTRACE_TYPE1_VAR_WB_N(_checkfn,_apifn,_header,_desc,_a0,_a1,_a2,_a3,_a4,_a5,...) \
   _checkfn(_desc,_a0,_a1,_a2,_a3,_a4,_a5, ## __VA_ARGS__);                               \
   _apifn((unsigned)(_a0),_a1,_a2,_a3,_a4,_a5,_header,_desc, ## __VA_ARGS__)
/*!@}*/

/*===========================================================================
                   END OF INTERNAL HELPER MACROS
===========================================================================*/

#ifndef __ASSEMBLER__

/*! @brief Message header (2 * 32 bits). */
typedef struct
{
  uint32 length : 4;           /*!< Length of message in 32-bit words, including 2-word header */
  uint32 format : 3;           /*!< Format (always 1) */
  uint32 invalid : 1;          /*!< Invalid bit - contents are partially unpopulated when set. */
  uint32 tag : 8;              /*!< Message tag from caller */
  uint32 timestamp_msw : 16;   /*!< Bits[47:32] of XO timestamp */
  uint32 timestamp_lsw;        /*!< Bits[31:0] of XO timestamp */
  /* Followed by uint32[length-2] words of message body */

} mtrace_type1_msg_header_t;


/*! @brief Post-trace callback function. */
struct mtrace_type1_descriptor_s;
typedef void (*mtrace_type1_callback_t)(struct mtrace_type1_descriptor_s *desc,
                                        mtrace_type1_msg_header_t *msg,
                                        void *cb_arg);
typedef void (*mtrace_type1_callback_ind_t)(struct mtrace_type1_descriptor_s **desc,
                                            mtrace_type1_msg_header_t *msg,
                                            void *cb_arg);


/*! @brief Atomic synchronization information for descriptor. */
typedef union
{
  struct {
    uint32 wrap_cnt : 8;         /*!< Count of buffer wraparounds (saturated) */
    uint32 active_wr_ofs : 24;   /*!< Last written word of active writers (byte offset) */
    uint32 num_writers : 8;      /*!< Number of writers currently active */
    uint32 wr_ofs : 24;          /*!< Last contiguous word written (byte offset) */
  } fields;

  atomic64_word_t atomic;        /*!< Atomic access to all fields */
  uint64 full;                   /*!< Local snapshot atomic access */

} mtrace_type1_sync_t;


/*! @brief Callback function/argument as atomic unit. */
typedef union
{
  struct {
    void *cb_arg;                 /*!< User argument for post-trace callback */
    union {
      mtrace_type1_callback_t cb; /*!< Post-trace callback */
      mtrace_type1_callback_ind_t
        cb_ind;                   /*!< Post-trace callback, indirect */
    };
  } fields;

  atomic64_word_t atomic;         /*!< Atomic access of single dword. */
  uint64 full;                    /*!< Local snapshot atomic access */

} mtrace_type1_cb_atomic_t;


/*! @brief Type1 trace buffer descriptor. */
typedef struct mtrace_type1_descriptor_s
{
  mtrace_type1_sync_t sync;     /*!< Buffer synchronization info, atomic */

  uint32 *buf_ptr ALIGN(8);     /*!< Trace buffer pointer */
  uint32 buf_size;              /*!< Size (in bytes) of buffer */

  mtrace_type1_cb_atomic_t callback;  /*!< Callback function/argument */

  uint32 rd_ofs;                /*!< Last word read (byte offset) */
  uint32 seq_num;               /*!< Sequence # for offload handling */

  uint32 flags;                 /*!< Flags for descriptor. */

} mtrace_type1_descriptor_t;



/*===========================================================================

                    INTERNAL INLINE HELPER FUNCTIONS

===========================================================================*/

/*! @brief Dummy function to let compiler check the descriptor and format string
           of mtrace type 1 calls */
INLINE  __attribute__((format(printf,2,3)))
  void _MTRACE_TYPE1_CHECK_STR(mtrace_type1_descriptor_t *_descr, const char *fmt, ...) { }
INLINE  __attribute__((format(printf,2,3)))
  void _MTRACE_TYPE1_CHECK_STR_IND(mtrace_type1_descriptor_t **_descr, const char *fmt, ...) { }

/*! @brief Dummy function to let compiler check the descriptor of mtrace type 1 calls */
INLINE void _MTRACE_TYPE1_CHECK_BIN(mtrace_type1_descriptor_t *_descr,...) { }
INLINE void _MTRACE_TYPE1_CHECK_BIN_IND(mtrace_type1_descriptor_t **_descr,...) { }


/*===========================================================================

                    EXTERNAL FUNCTION PROTOTYPES

===========================================================================*/


#ifdef __cplusplus
extern "C" {
#endif

extern void mtrace_type1_init
(
  mtrace_type1_descriptor_t *desc,
  uint32 flags,
  void *trace_buf,
  uint32 trace_buf_size
);

extern void mtrace_type1_set_callback
(
  mtrace_type1_descriptor_t *desc,
  mtrace_type1_callback_t cb_func,
  void *cb_arg
);

extern void mtrace_type1_set_callback_ind
(
  mtrace_type1_descriptor_t *desc,
  mtrace_type1_callback_ind_t cb_func,
  void *cb_arg
);

extern int32 mtrace_type1_offload
(
  mtrace_type1_descriptor_t *desc,
  uint32 *sequence,
  void *offload_buf,
  uint32 offload_buf_size
);

extern int32 mtrace_type1_query_remaining
(
  mtrace_type1_descriptor_t *desc
);

extern uint32 mtrace_type1_salvage
(
  void *client_buf,
  uint32 client_buf_size,
  uint32 *start_offset
);

extern void mtrace_type1_copy
(
  mtrace_type1_descriptor_t *desc,
  mtrace_type1_msg_header_t *msg
);

extern void mtrace_type1_copy_ind
(
  mtrace_type1_descriptor_t **desc,
  mtrace_type1_msg_header_t *msg
);

extern void mtrace_type1_bin_header32_argp
(
  mtrace_type1_descriptor_t    *desc,
  uint8                         tag,
  uint32                        header,
  uint32                       *arg_ptr,
  uint32                        arg_count
);

/*! @brief Helper API to get the mtrace type1 descriptor pointer from an indirect pointer */
INLINE mtrace_type1_descriptor_t *mtrace_type1_ind_get_desc_ptr(mtrace_type1_descriptor_t **desc)
{
  return (mtrace_type1_descriptor_t *)((uintptr_t)*desc & -4);
}

/*! @brief Helper API to get the two LSbits of an mtrace type1 descriptor pointer,
   given an indirect mtrace type1 descriptor pointer */
INLINE unsigned int mtrace_type1_ind_get_desc_twolsb(mtrace_type1_descriptor_t **desc)
{
  return (unsigned int)(*desc) & 0x3;
}

/*! @brief Helper API to set the two LSbits of an mtrace type1 descriptor pointer,
   given an indirect mtrace type1 descriptor pointer */
INLINE void mtrace_type1_ind_set_desc_twolsb(mtrace_type1_descriptor_t **desc, unsigned int twolsbs)
{
  *desc = (mtrace_type1_descriptor_t *)((uintptr_t)mtrace_type1_ind_get_desc_ptr(desc) | (twolsbs & 0x3));
}



/*===========================================================================

                EXTERNAL FUNCTIONS RESOLVED BY MTRACE MACROS
                  **  DO NOT CALL THESE DIRECTLY  **

===========================================================================*/

/* Direct descriptor pointer API variants */
extern void mtrace_type1_var_wb_0(uint32 header, mtrace_type1_descriptor_t *desc);
extern void mtrace_type1_var_wb_1(uint32 a0, ...);
extern void mtrace_type1_var_wb_2(uint32 a0, uint32 a1, ...);
extern void mtrace_type1_var_wb_3(uint32 a0, uint32 a1, uint32 a2, ...);
extern void mtrace_type1_var_wb_4(uint32 a0, uint32 a1, uint32 a2, uint32 a3, ...);
extern void mtrace_type1_var_wb_5(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, ...);
extern void mtrace_type1_var_wb_N(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, ...);

/* Indirect descriptor pointer API variants, no zero-arg API provided */
extern void mtrace_type1_var_wb_ind_1(uint32 a0, ...);
extern void mtrace_type1_var_wb_ind_2(uint32 a0, uint32 a1, ...);
extern void mtrace_type1_var_wb_ind_3(uint32 a0, uint32 a1, uint32 a2, ...);
extern void mtrace_type1_var_wb_ind_4(uint32 a0, uint32 a1, uint32 a2, uint32 a3, ...);
extern void mtrace_type1_var_wb_ind_5(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, ...);
extern void mtrace_type1_var_wb_ind_N(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, ...);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // __ASSEMBLER__


#endif /* MTRACE_TYPE1_H */
