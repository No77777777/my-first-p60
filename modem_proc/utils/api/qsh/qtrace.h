/*!
  @file
  qtrace.h

  @brief
  Interface for QSH trace buffer.

  @example
  Basic buffer configuration:

  // define id and name
  #define A2_QTRACE_BUFFER_ID \
    0
  #define A2_QTRACE_BUFFER_NAME \
    "A2_DBG"

  qtrace_context_id_t context_id;

  // flush periodically or upon impending wraparound
  context_id = qtrace_buffer_cfg(
    QSH_CLT_A2,
    A2_QTRACE_BUFFER_ID,
    A2_QTRACE_BUFFER_NAME,
    QTRACE_SIZE_1K,
    QTRACE_BUFFER_MODE_PERIODIC | QTRACE_BUFFER_MODE_WRAP);

  @example
  Basic usage:

  QTRACE(
    context_id,
    QSH_CLT_A2,
    QTRACE_LEVEL_DEBUG,
    SYS_MODEM_AS_ID_NONE,
    QTRACE_TAG_NONE,
    "Test debug message: 0x%08X", -1);
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

$Header: //components/rel/utils.mpss/5.4/api/qsh/qtrace.h#1 $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
05/06/19   mm      CR 2380036: QTrace throttle feature
03/07/19   mm      CR 2411856: Store buffer config during init on MOB
10/03/18   rs      CR 2326931: QTrace Common Debug API enhancement
10/10/18   mm      CR 2290332: API to input hash id and parameters
10/01/18   mm      CR 2317056: Poweron log copy + buffer disable feature
09/07/18   rs      CR 2311053: FR 49785 - Qtrace support for event-based flush
09/10/18   mm      CR 2311501: Support for 64-bit arguments
08/29/18   mm      CR 2304039: Use tag bit to indicate internal-only log
08/22/18   mm      CR 2300075: Add poweron log stats
08/20/18   mm      CR 2299870: Add QTRACE_INT() for internal logs
07/19/18   rs      CR 2266460: Qtrace statistics log packets
06/19/18   yx      CR 2263684: Ulog to QTrace migration in QSH_LOG
06/18/18   am      CR 2251065: FR51021 QTrace master buffer to use large memory
03/22/18   rs      CR 2210987: Added Macro for Qtrace buffer size
01/17/18   am      CR 2151280: QTrace string table enhancement
10/18/17   mm      CR 2125824: Fix QTRACE_STR()
09/07/17   mm      CR 2106081: Support for removing logs by client
08/02/17   mm      CR 2085905: Add QTRACE_S() for %s support
07/03/17   mm      CR 2070787: Support for format strings with %s
05/10/17   mm      CR 2032197: Adding qtrace_buffer_init_ext()
02/10/17   mm      CR 1104665: Interface revisions
01/12/17   mm      CR 1081130: Feature bringup
08/05/16   vd      initial version
==============================================================================*/

#ifndef QTRACE_H
#define QTRACE_H

/*==============================================================================

                           INCLUDE FILES

==============================================================================*/

#include <comdef.h>
#include <qtrace_util.h>
#include <qsh_types.h>
#include <qsh_ext_types.h>
#include <qsh_util.h>
#include <mtrace_type1.h>
#include <sys.h>
#include <mutils_q.h>

/*==============================================================================

                   EXTERNAL DEFINITIONS AND TYPES

==============================================================================*/

/*!
  @brief
  Suggested sizes to be used for each buffer.
*/
#define QTRACE_SIZE_MIN                 (QTRACE_SIZE_256B)
#define QTRACE_SIZE_256B                (256)
#define QTRACE_SIZE_512B                (512)
#define QTRACE_SIZE_1K                  (1024)
#define QTRACE_SIZE_2K                  (2048)
#define QTRACE_SIZE_4K                  (4096)
#define QTRACE_SIZE_8K                  (8192)
#define QTRACE_SIZE_16K                 (16384)
#define QTRACE_SIZE_32K                 (32768)
#define QTRACE_SIZE_64K                 (65536)

/*!
  @brief 
  QTrace context id.
*/
typedef struct qtracei_buffer_desc_s qtracei_buffer_desc_s;
typedef qtracei_buffer_desc_s * qtrace_context_id_t;

/*!
  @brief
  Context id initialized upon bootup to be used as a failsafe / fallback.
*/
#define QTRACE_CONTEXT_ID_COMMON \
  ((qtrace_context_id_t) &qtrace_buffer_common)

/*!
  @brief
  Context id to be used for logging upon ANALYSIS action.
*/
#define QTRACE_CONTEXT_ID_ANALYSIS \
  ((qtrace_context_id_t) &qtrace_buffer_analysis)

/*!
  @brief
  Context id to be used for cmndbg debug buffer
*/
#define QTRACE_CONTEXT_ID_CMNDBG \
  ((qtrace_context_id_t) &qtrace_buffer_cmndbg)

/*!
  @brief
  Context id to be used for profile_hf buffer
*/
#define QTRACE_CONTEXT_ID_PROFILE_HF \
  ((qtrace_context_id_t) &qtrace_buffer_profile_hf)

/*!
  @brief
  Context id to be used for profile_hf buffer
*/
#define QTRACE_CONTEXT_ID_PROFILE_LF \
  ((qtrace_context_id_t) &qtrace_buffer_profile_lf)


/*!
  @brief 
  Client defined trace buffer id.
*/
typedef uint8 qtrace_buffer_id_t;

/*!
  @brief 
  Modes in which a buffer can be configured. Used as a bitmask.

  @note
  Not all bitmask combinations are valid. Exceptions:

  - STREAM: cannot also have modes PERIODIC, EVENT, or WRAP
*/
typedef enum
{
  /*! no flush */
  QTRACE_BUFFER_MODE_NO_FLUSH   = 0x0,

  /*! flush on every QTRACE() in caller's context */
  QTRACE_BUFFER_MODE_STREAM     = 0x1,

  /*! flush periodically in QSH task context */
  QTRACE_BUFFER_MODE_PERIODIC   = 0x2,

  /*! flush upon occurence of an event in QSH task context */
  QTRACE_BUFFER_MODE_EVENT      = 0x4,

  /*! flush upon wraparound in QSH task context */
  QTRACE_BUFFER_MODE_WRAP       = 0x8,

  /*! Not part of common buffer */
  QTRACE_BUFFER_MODE_NO_COMMON  = 0x10,

  /*! max */
  QTRACE_BUFFER_MODE_MAX        = 0x20
} qtrace_buffer_mode_e;

/*!
  @brief
  Type to be used as mode bitmask.
*/
typedef uint32 qtrace_buffer_mode_mask_t;

/*!
  @brief
  Whether or not mode can be changed at run-time for external builds.
*/
typedef enum
{
  /*! No restriction on mode change. */
  QTRACE_BUFFER_TYPE_NONE,

  /*! Mode change restricted to internal builds only. */
  QTRACE_BUFFER_TYPE_RESTRICT
} qtrace_buffer_type_e;

/*!
  @brief 
  Type to describe generic logging behavior.
*/
typedef enum
{
  /*! For low-frequency logging like:
    - config
    - events
    - control path
    - critical messages
    - errors
  */
  QTRACE_LOG_CAT_LOW_FREQ,

  /*! For high-frequency logging like: (AS layers)
    - logs per TTI or other interval
    - datapath logs
  */
  QTRACE_LOG_CAT_HIGH_FREQ,

  QTRACE_LOG_CAT_MAX
} qtrace_log_cat_e;

/*!
  @brief
  Message level for tracking logs.
*/
typedef enum
{
  /*! debug */
  QTRACE_LEVEL_DEBUG,

  /*! medium */
  QTRACE_LEVEL_MED,

  /*! high */
  QTRACE_LEVEL_HIGH,

  /*! error */
  QTRACE_LEVEL_ERROR
} qtrace_level_e;

/*!
  @brief
  Possible fields to show by default in crashscope.
*/
typedef enum
{
  /*! no extra columns */
  QTRACE_FORMAT_NONE                = 0x0000,

  /*! QSH client id */
  QTRACE_FORMAT_CLIENT_ID           = 0x0001,

  /*! level */
  QTRACE_FORMAT_LEVEL               = 0x0002,

  /*! subscriber id */
  QTRACE_FORMAT_SUBS_ID             = 0x0004,

  /*! tag */
  QTRACE_FORMAT_TAG                 = 0x0008,

  /*! filename/line */
  QTRACE_FORMAT_FILE_LINE           = 0x0010,

  /* timestamp: pick no more than 2 bits */

  /*! XO time */
  QTRACE_FORMAT_TIMESTAMP_XO        = 0x1000,

  /*! slow clock */
  QTRACE_FORMAT_TIMESTAMP_SCLK      = 0x2000,

  /*! seconds */
  QTRACE_FORMAT_TIMESTAMP_SECONDS   = 0x4000,

  /*! time (hh:mm:ss) */
  QTRACE_FORMAT_TIMESTAMP_TIME      = 0x8000
} qtrace_format_e;

/*!
  @brief
  Bitmask of format fields. Set using qtrace_format_e.
*/
typedef uint16 qtrace_format_mask_t;

/*!
  @brief
  Default bitmask of format fields.
*/
#define QTRACE_FORMAT_MASK_DEFAULT \
  ((qtrace_format_mask_t) ( \
    QTRACE_FORMAT_CLIENT_ID | \
    QTRACE_FORMAT_LEVEL | \
    QTRACE_FORMAT_SUBS_ID | \
    QTRACE_FORMAT_TAG | \
    QTRACE_FORMAT_FILE_LINE | \
    QTRACE_FORMAT_TIMESTAMP_TIME))

/*!
  @brief
  Type used to store buffer configuration.
*/
typedef struct
{
  /*! client to which the buffer belongs */
  qsh_clt_e                   client;

  /*! buffer id: can take values 0-31 (both inclusive) */
  qtrace_buffer_id_t          id;

  /*! buffer name */
  const char *                name_ptr;

  /*! size of buffer in bytes */
  size_t                      size_bytes;

  /*! buffer mode: set using qtrace_buffer_mode_e */
  qtrace_buffer_mode_mask_t   mode_mask;

  /*! whether or not buffer mode can be changed at run-time */
  qtrace_buffer_type_e        buffer_type;

  /*! category in which to classify expected logging behavior */
  qtrace_log_cat_e            log_cat;

  /*! mask to indicate default format for this buffer in crashscope */
  qtrace_format_mask_t        format_mask;
} qtrace_buffer_cfg_s;

/*!
  @brief
  Macro to use if no tag is needed.
*/
#define QTRACE_TAG_NONE \
  ""

/*!
  @brief
  Macro to assemble tag string based on tags, joining with "|".
*/
#define QTRACE_TAGS(...) \
  QTRACEI_TAGS(__VA_ARGS__)

/*!
  @brief
  Macro defining a string to be passed as QTRACE() argument.

  @example
  QTRACE_STR(my_str, "test");
  QTRACE_S(..., "my_str=%s", my_str);
*/
#define QTRACE_STR(_var, _str) \
  static const char QTRACEI_STR_ATTR _var[] = _str

/*!
  @brief
  Macro defining table of strings to be passed as QTRACE() arguments with
  corresponding %s format specifier.

  @note
  Works for 1 <= n <= 256 variadic arguments.

  @example
  QTRACE_STR_TBL(my_str_tbl, "A", "B", "C");
  ...
  for (i = 0; i < ARR_SIZE(my_str_tbl); i ++)
  {
    QTRACE_S(..., "i=%d, my_str_tbl[i]=%s", i, my_str_tbl[i]);
  }
*/
#define QTRACE_STR_TBL(_var, ...) \
  QTRACEI_STR_TBL(_var, __VA_ARGS__)

/*!
  @brief
  Same as QTRACE_STR_TBL, but explicitly provide number of strings expected.
*/
#define QTRACE_STR_TBL_N(_var, _count, ...) \
  QTRACEI_STR_TBL_N(_var, _count, __VA_ARGS__)

/*!
  @brief
  Macro to extract upper/lower 32 bits of 64-bit word.

  @note
  Must be used in QTRACE_S().

  @example
  uint64 timestamp;
  ...
  QTRACE_S(...,
    "timestamp=0x%016llX",
    QTRACE_ARG64(timestamp));
*/
#define QTRACE_ARG64(_x) \
  QTRACE_ARG64_LOWER(_x), QTRACE_ARG64_UPPER(_x)

#define QTRACE_ARG64_LOWER(_x) \
  ((uint32) (_x))
#define QTRACE_ARG64_UPPER(_x) \
  ((uint32) (((uint64) (_x)) >> 32))

/*!
  @brief
  Macro to interpret double as uint64. Simple type-cast is not sufficient
  as its value becomes truncated.

  @note
  Must be used in QTRACE_S().

  @example
  double my_dbl;
  ...
  QTRACE_S(...,
    "my_dbl=%f",
    QTRACE_ARG64_DBL(my_dbl));
*/
#define QTRACE_ARG64_DBL(_x) \
  QTRACE_ARG64(*((uint64 *) &(_x)))

/*! 
  @brief
  Logging with up to 12 optional arguments.
    1st arg: Trace context id (return value from qtrace_buffer_cfg())
    2nd arg: Client performing logging
    3rd arg: Msg level
    4th arg: Subscription id
    5th arg: Tag
    6th arg: Summary for the logging
    >= 7th arg: Arguments for the logging (optional)
*/
#define QTRACE( \
  _buffer_context_id,   /* qtrace_context_id_t */     \
  _client,              /* qsh_clt_e */               \
  _level,               /* qtrace_level_e */          \
  _subs_id,             /* sys_modem_as_id_e_type */  \
  _tag,                 /* string for tag */          \
  _summary,             /* string for format */       \
  ...                   /* variadic args */           \
) \
  QTRACEI_ ## _client ( \
    _buffer_context_id, \
    _client, \
    _level, \
    _subs_id, \
    _tag, \
    _summary, \
    ##__VA_ARGS__)

/*! 
  @brief
  Logging with up to 12 optional arguments, supporting %s specifiers.

  @note
  Any arguments corresponding to %s must be defined with QTRACE_STR or
  QTRACE_STR_TBL().

  @example
  QTRACE_STR my_str = "test";
  QTRACE_S(..., "my_str=%s", my_str);

  @example
  QTRACE_STR_TBL(my_str_tbl, "A", "B", "C");
  ...
  for (i = 0; i < ARR_SIZE(my_str_tbl); i ++)
  {
    QTRACE_S(..., "i=%d, my_str_tbl[i]=%s", i, my_str_tbl[i]);
  }
*/
#define QTRACE_S( \
  _buffer_context_id,   /* qtrace_context_id_t */     \
  _client,              /* qsh_clt_e */               \
  _level,               /* qtrace_level_e */          \
  _subs_id,             /* sys_modem_as_id_e_type */  \
  _tag,                 /* string for tag */          \
  _summary,             /* string for format */       \
  ...                   /* variadic args */           \
) \
  QTRACEI_S_ ## _client ( \
    _buffer_context_id, \
    _client, \
    _level, \
    _subs_id, \
    _tag, \
    _summary, \
    ##__VA_ARGS__)

/*!
  QTRACE() for logs which shouldn't be exposed externally.
*/
#define QTRACE_INT( \
  _buffer_context_id,   /* qtrace_context_id_t */     \
  _client,              /* qsh_clt_e */               \
  _level,               /* qtrace_level_e */          \
  _subs_id,             /* sys_modem_as_id_e_type */  \
  _tag,                 /* string for tag */          \
  _summary,             /* string for format */       \
  ...                   /* variadic args */           \
) \
  QTRACEI_INT_ ## _client ( \
    _buffer_context_id, \
    _client, \
    _level, \
    _subs_id, \
    _tag, \
    _summary, \
    ##__VA_ARGS__)

#define QTRACE_INT_S( \
  _buffer_context_id,   /* qtrace_context_id_t */     \
  _client,              /* qsh_clt_e */               \
  _level,               /* qtrace_level_e */          \
  _subs_id,             /* sys_modem_as_id_e_type */  \
  _tag,                 /* string for tag */          \
  _summary,             /* string for format */       \
  ...                   /* variadic args */           \
) \
  QTRACEI_INT_S_ ## _client ( \
    _buffer_context_id, \
    _client, \
    _level, \
    _subs_id, \
    _tag, \
    _summary, \
    ##__VA_ARGS__)

/*!
  Helper macros for logging to QSH/ANALYSIS buffer.
*/
#define QANALYSIS( \
  _client,              /* qsh_clt_e */               \
  _level,               /* qtrace_level_e */          \
  _subs_id,             /* sys_modem_as_id_e_type */  \
  _tag,                 /* string for tag */          \
  _summary,             /* string for format */       \
  ...                   /* variadic args */           \
) \
  QTRACE( \
    QTRACE_CONTEXT_ID_ANALYSIS, \
    _client, \
    _level, \
    _subs_id, \
    _tag, \
    _summary, \
    ##__VA_ARGS__)

#define QANALYSIS_S( \
  _client,              /* qsh_clt_e */               \
  _level,               /* qtrace_level_e */          \
  _subs_id,             /* sys_modem_as_id_e_type */  \
  _tag,                 /* string for tag */          \
  _summary,             /* string for format */       \
  ...                   /* variadic args */           \
) \
  QTRACE_S( \
    QTRACE_CONTEXT_ID_ANALYSIS, \
    _client, \
    _level, \
    _subs_id, \
    _tag, \
    _summary, \
    ##__VA_ARGS__)

/*!
  QANALYSIS() for internal logging.
*/
#define QANALYSIS_INT( \
  _client,              /* qsh_clt_e */               \
  _level,               /* qtrace_level_e */          \
  _subs_id,             /* sys_modem_as_id_e_type */  \
  _tag,                 /* string for tag */          \
  _summary,             /* string for format */       \
  ...                   /* variadic args */           \
) \
  QTRACE_INT( \
    QTRACE_CONTEXT_ID_ANALYSIS, \
    _client, \
    _level, \
    _subs_id, \
    _tag, \
    _summary, \
    ##__VA_ARGS__)

#define QANALYSIS_INT_S( \
  _client,              /* qsh_clt_e */               \
  _level,               /* qtrace_level_e */          \
  _subs_id,             /* sys_modem_as_id_e_type */  \
  _tag,                 /* string for tag */          \
  _summary,             /* string for format */       \
  ...                   /* variadic args */           \
) \
  QTRACE_INT_S( \
    QTRACE_CONTEXT_ID_ANALYSIS, \
    _client, \
    _level, \
    _subs_id, \
    _tag, \
    _summary, \
    ##__VA_ARGS__)

/*!
  Macro for special case when summary is already populated with fields like 
  client name, level, file, line etc by QSH client.
*/
#define QTRACE_QSRHASH( \
  _buffer_context_id,   /* qtrace_context_id_t */     \
  _subs_id,             /* sys_modem_as_id_e_type */  \
  _qsr_hash,            /* qshrink hash code */       \
  ...                   /* variadic args */           \
) \
  QTRACEI_QSRHASH( \
    _buffer_context_id, \
    _subs_id, \
    _qsr_hash, \
    ##__VA_ARGS__)

#define QTRACE_QSRHASH_INT( \
  _buffer_context_id,   /* qtrace_context_id_t */     \
  _subs_id,             /* sys_modem_as_id_e_type */  \
  _qsr_hash,            /* qshrink hash code */       \
  ...                   /* variadic args */           \
) \
  QTRACEI_QSRHASH_INT( \
    _buffer_context_id, \
    _subs_id, \
    _qsr_hash, \
    ##__VA_ARGS__)

/*! 
  @brief
  This Qtrace based API will be used for logging on Internal builds.
  API uses QSH/CMNDBG buffer for logging.
  
  @note
  No need to specify Trace context ID, by default 
  QTRACE_CONTEXT_ID_CMNDBG will be used for logging.
  
  Logging with up to 12 optional arguments.
    1nd arg: Client performing logging
    2rd arg: Msg level
    3th arg: Subscription id
    4th arg: Tag
    5th arg: Summary for the logging
    >= 6th arg: Arguments for the logging (optional)
*/
#define QTRACE_CMNDBG( \
  _client,              /* qsh_clt_e */               \
  _level,               /* qtrace_level_e */          \
  _subs_id,             /* sys_modem_as_id_e_type */  \
  _tag,                 /* string for tag */          \
  _summary,             /* string for format */       \
  ...                   /* variadic args */           \
) \
  QTRACEI_ ## _client ( \
    QTRACE_CONTEXT_ID_CMNDBG, \
    _client, \
    _level, \
    _subs_id, \
    _tag, \
    _summary, \
    ##__VA_ARGS__)

/*! 
  @brief
  This Qtrace based API will be used for logging on Internal builds.
  API uses QSH/CMNDBG buffer for logging, supporting %s specifiers.

  @note
  Please refer to QTRACE_S() for %s usage.
  
*/
#define QTRACE_CMNDBG_S( \
  _client,              /* qsh_clt_e */               \
  _level,               /* qtrace_level_e */          \
  _subs_id,             /* sys_modem_as_id_e_type */  \
  _tag,                 /* string for tag */          \
  _summary,             /* string for format */       \
  ...                   /* variadic args */           \
) \
  QTRACEI_S_ ## _client ( \
    QTRACE_CONTEXT_ID_CMNDBG, \
    _client, \
    _level, \
    _subs_id, \
    _tag, \
    _summary, \
    ##__VA_ARGS__)

/*==============================================================================

                    EXTERNAL FUNCTION PROTOTYPES

==============================================================================*/

/*==============================================================================

  FUNCTION:  qtrace_buffer_init

==============================================================================*/
/*!
  @brief
  Initializes and configures the buffer.
  
  @return
  qtrace_context_id_t:
    QTrace context id to be used in QTRACE(...)
*/
/*============================================================================*/
qtrace_context_id_t qtrace_buffer_init
(
  /*! client to which the buffer belongs */
  qsh_clt_e                   client,

  /*! buffer id: can take values 0-3 (both inclusive) */
  qtrace_buffer_id_t          id,

  /*! buffer name */
  const char *                name_ptr,

  /*! size of buffer in bytes */
  size_t                      size_bytes,

  /*! buffer mode: set using qtrace_buffer_mode_e */
  qtrace_buffer_mode_mask_t   mode_mask,

  /*! whether or not buffer mode can be changed at run-time */
  qtrace_buffer_type_e        buffer_type,

  /*! category in which to classify expected logging behavior */
  qtrace_log_cat_e            log_cat
);

/*==============================================================================

  FUNCTIONS:  qtrace_buffer_init_ext_init
              qtrace_buffer_init_ext

==============================================================================*/
/*!
  @brief
  Initializes and configures the buffer, exposing all params. 
  
  @return
  qtrace_context_id_t:
    Trace context id that needs to be used in QTRACE(...)
*/
/*============================================================================*/
void qtrace_buffer_init_ext_init
(
  /*! buffer configuration to initialize */
  qtrace_buffer_cfg_s *   buffer_cfg_ptr
);

qtrace_context_id_t qtrace_buffer_init_ext
(
  /*! initialized buffer configuration */
  qtrace_buffer_cfg_s *   buffer_cfg_ptr
);

/*==============================================================================

  FUNCTION:  qtrace_buffer_deinit

==============================================================================*/
/*!
  @brief
  Safely deallocates buffer and restores context id to appropriate common
  context id.
*/
/*============================================================================*/
void qtrace_buffer_deinit
(
  /*! pointer to context id to reassign */
  qtrace_context_id_t *   context_id_ptr
);

/*==============================================================================

  FUNCTION: qtrace_buffer_flush_event 

==============================================================================*/
/*!
  @brief
  Initiate flush of provided buffer to diag in QSH context.

  @note
  Buffer must have mode bit QTRACE_BUFFER_MODE_EVENT set.
*/
/*============================================================================*/
void qtrace_buffer_flush_event 
(
  /*! buffer context id */
  qtrace_context_id_t   context_id
);

/*==============================================================================

                   INTERNAL DEFINITIONS AND TYPES

==============================================================================*/

/*!
  The following are needed for compilation, but should not be accessed
  directly by clients.
*/

/*!
  Define flag to enable statistics at a small runtime cost.
*/
#define FEATURE_QTRACEI_STATS

/*!
  Define flag to enable throttling histogram.
*/
//#define FEATURE_QTRACEI_THROTTLE_HIST

/*!
  Alignment of internal buffer struct and buffer in bytes.
 
  @note
  Putting parentheses around definition results in compilation errors on MSVC.
*/
#define QTRACEI_BUFFER_ALIGN_BYTES \
  MTRACE_TYPE1_BLOCK_SIZE_BYTES

/*!
  Stats per log.
*/
typedef struct
{
  /*! number of times QTRACE() has been called for logging */
  uint32  log_count;

  /*! number of bytes logged */
  uint32  logged_bytes;
  
  /*! number of logs during poweron window */
  uint32  poweron_log_count;

  /*! number of bytes logged during poweron */
  uint32  poweron_logged_bytes;

  /*! number of times log has been flushed */
  uint32  flush_count;

  /*! accumulated number of times QTRACE() was called for logging */
  uint32  global_log_count;

  /*! accumulated number of time log has been flushed */
  uint32  global_flush_count;
  
  /*! min wraparound time in ticks */
  uint32  wrap_time_min;
} qtracei_buffer_stats_log_s;

/*!
  Throttling threshold type.
*/
typedef uint32 qtracei_throttle_thresh_t;

/*!
  Per-buffer throttle configuration as retrieved from XML.
*/
typedef struct
{
  /*! threshold per ms */
  qtracei_throttle_thresh_t   thresh;
} qtracei_throttle_cfg_s;

/*!
  Histogram bin type.
*/
typedef uint32 qtracei_throttle_hist_bin_t;

/*!
  Slot state, to be updated atomically.
*/
typedef union
{
  struct
  {
    /*! log count above which to trigger throttling */
    uint32  log_count_thresh;

    /*! upper bits of timetick to detect when new slot occurs */
    uint32  timetick_upper;
  }       fields;
  uint64  full;
} qtracei_throttle_slot_u;

/*!
  State to manage throttling.
*/
typedef struct
{
  /*! slot state, updated atomically */
  qtracei_throttle_slot_u         slot;

  /*! whether or not throttling is active, updated atomically */
  uint32                          active;

  /*! masked with log_count to check if throttle activation should proceed */
  uint32                          activate_mask;

  /*! stats */
  struct
  {
    /*! number of logs dropped due to throttling */
    uint32  log_count_drop;

    /*! number of logs dropped at start of current slot */
    uint32  log_count_drop_slot;

    /*! number of times throttling activated */
    uint32  activate_count;

    /*! number of times throttling deactivated */
    uint32  deactivate_count;
  }                               stats;

  /*! config retrieved from XML */
  qtracei_throttle_cfg_s          cfg;

#ifdef FEATURE_QTRACEI_THROTTLE_HIST
  /*! log count during last slot rollover */
  uint32                          log_count_rollover;

  /*! number of times data was clamped to last bin */
  uint32                          saturate_count;

  /*! pointer to histogram buffer */
  qtracei_throttle_hist_bin_t *   hist_ptr;
#endif /* FEATURE_QTRACEI_THROTTLE_HIST */
} qtracei_buffer_throttle_s;

/*!
  Offload state.
*/
typedef struct
{
  /*! link for periodic-based flush */
  mutils_q_link_s   link_periodic;

  /*! link for wrap-based flush */
  mutils_q_link_s   link_wrap;

  /*! fill level below which impending wrap is triggered */
  size_t            wrap_thresh_bytes;
} qtracei_buffer_offload_s;

/*!
  Basic stats accessed only in control path.
*/
typedef struct
{
  /*! # times this buffer has been initialized */
  uint16  init_count;

  /*! # times this buffer has been freed */
  uint16  deinit_count;

  /*! min free space in bytes seen during offload */
  uint32  avail_bytes_min;

  /*! # bytes offloaded */
  uint32  offloaded_bytes;

  /*! # bytes recovered via mtrace salvage */
  uint32  salvaged_bytes;

  /*! # bytes dropped during offload */
  uint32  dropped_bytes;
} qtracei_buffer_stats_s;

/*!
  Forwarding callback type.
*/
typedef mtrace_type1_callback_ind_t qtracei_fwd_cb_t;

/*!
  Types of forwarding callbacks to be defined, if buffer needs custom callbacks.
*/
typedef enum
{
  /*! invoked when flush disabled */
  QTRACEI_BUFFER_FWD_CB_NO_FLUSH,

  /*! invoked when flush enabled */
  QTRACEI_BUFFER_FWD_CB_FLUSH,

  QTRACEI_BUFFER_FWD_CB_MAX
} qtracei_buffer_fwd_cb_e;

/*!
  Types for lookup tables of forwarding callbacks.
*/
typedef qtracei_fwd_cb_t qtracei_fwd_cb_tbl_t[QTRACEI_BUFFER_FWD_CB_MAX];
typedef const char * qtracei_fwd_cb_str_tbl_t[QTRACEI_BUFFER_FWD_CB_MAX];

/*!
  Internal data structure for buffer.

  @note
  When instantiated, align to cache line (64 bytes) for performance.
*/
struct qtracei_buffer_desc_s
{
  /*! instance of mtrace descriptor */
  mtrace_type1_descriptor_t     mtrace_desc;
  
  /*! pointer to mtrace descriptor currently used for this buffer */
  mtrace_type1_descriptor_t *   mtrace_desc_ptr;

  /*! pointer to mtrace descriptor accessed during poweron */
  mtrace_type1_descriptor_t *   mtrace_desc_poweron_ptr;

  /*! cached diag header */
  uint32                        diag_hdr;

  /*! current stream enable state (enabled and configured for streaming) */
  boolean                       stream_en;

  /*! flush reference count: number of flush operations pending */
  uint32                        ref_cnt_flush;

  /*! mtrace reference count: number of other buffers with mtrace descriptor
    pointing to this one */
  uint32                        ref_cnt_mtrace;

  /*! logging stats */
  qtracei_buffer_stats_log_s    stats_log;

#ifdef FEATURE_QTRACEI_STATS
  /*! time at last wraparound */
  uint64                        wrap_timestamp_last;
#endif /* FEATURE_QTRACEI_STATS */

  /*! throttle state */
  qtracei_buffer_throttle_s     throttle;

  /*! pointer to buffer owning mtrace descriptor, 
    or NULL if this buffer owns it */
  qtracei_buffer_desc_s *       master_desc_ptr;

  /*! externally-provided memory to use for mtrace, or NULL */
  uint8 *                       mtrace_mem_ptr;

  /*! state for offloading */
  qtracei_buffer_offload_s      offload;

  /*! config as passed by client */
  qtrace_buffer_cfg_s           cfg;

  /*! persistent stats */
  qtracei_buffer_stats_s        stats;

  /*! ptr to next trace buffer in top-level list */
  mutils_q_link_s               link;

  /*! ptr to buffer heap allocation, or NULL if statically allocated */
  void *                        heap_ptr;

  /*! pointer to lookup table of callbacks */
  qtracei_fwd_cb_tbl_t *        fwd_cb_tbl_ptr;

  /*! pointer to lookup table of callback names */
  qtracei_fwd_cb_str_tbl_t *    fwd_cb_str_tbl_ptr;

  /*! flag to indicate if buffer is currently disabled 
    (if set, mtrace_desc_ptr should also be NULL) */
  boolean                       disable;
};

#if defined(FEATURE_QSH_STUBS) || defined(FEATURE_HEXSIM)
/*==============================================================================

  FUNCTION:  qtracei_flush_mob

==============================================================================*/
/*!
  Assembles mtrace record and invokes diag API.
*/
/*============================================================================*/
void qtracei_flush_mob
(
  qtrace_context_id_t     context_id,
  qsh_clt_e               client_id,
  uint8                   subs_id,
  const char *            fmt,
  uint32                  arg_count,
  ...
);
#endif /* defined(FEATURE_QSH_STUBS) || defined(FEATURE_HEXSIM) */

#endif /* QTRACE_H */
