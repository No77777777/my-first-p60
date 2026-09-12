/*========================================================================
  Copyright (c) 2018-2019 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
======================================================================== */
/**
@file mm_msg_base.h

@brief This file contains a utility for generating diagnostic messages.
 This file defines macros for printing debug messages on the target or
 in simulation.
*/

/*========================================================================
Edit History

$Header: //components/rel/avs.mpss/9.0.c51/api/mmutils/mm_msg_base.h#1 $

when       who     what, where, why
--------   ---     -------------------------------------------------------
05/03/18   rv      Created file.
11/12/18   akr     Cosim Implementation Integration
========================================================================== */

#ifndef _MM_MSG_BASE_H
#define _MM_MSG_BASE_H

/* =======================================================================
INCLUDE FILES FOR MODULE
========================================================================== */

#include "mmdefs.h"
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#ifndef MM_MSG_BASE_NEEDED
#error "HASH error to provent accidental inclusion of mm_msg_base.h, please include mm_msg.h instead of this"
#endif
//***************************************************************************
// Debug message features
//***************************************************************************

/** @addtogroup mm_logging
@{ */

//AKR: initialized to ADSP for UPD and MDSP for RPD.
//ideally for tgt, we need DIAG solution. For sim, when we have compile time master domain MACRO, we should use that.


/* Enable the following to use the lightweight simulator debug log
 * implemented in avs/main/utils/src/mm_cosim_msg.c and the
 * corresponding cosim */
#if defined(SIM) && !defined(ROOTPD_AVS_INTEGRATION)   //user pd
#define MM_MSG_USE_MM_DBG_LOG_COSIM
static const uint32_t mm_msg_domain_id = 2; //ADSP
#endif

#if defined (SIM) && defined(ROOTPD_AVS_INTEGRATION)   //rootpd
static const uint32_t mm_msg_domain_id = 1; //MDSP
#define MM_MSG_USE_RPD_MM_DBG_LOG_COSIM
#endif

/* New ssids can be requested by emailing asw.diag.request currently all ssid are mapped to MSG_SSID_QDSP6 */
/*Define SSID for QDSP6*/
#if defined(__hexagon__) || defined(__qdsp6__)
/* Include Diag message utitlity headers */
#include "msg.h"
#include "msgcfg.h"
#elif defined(__ARMCC_VERSION) || defined(__GNUC__)

/**
Place holder for diag (MSG_) utility defintions for ARM and GNUC.
ARM platforms dont support Diag, so need to define MSG macros.
*/

/* Define SSID and priotiy masks*/
#define MSG_LEGACY_LOW   /**< Low priority debug message. */
#define MSG_LEGACY_MED   /**< Medium priority debug message. */
#define MSG_LEGACY_HIGH  /**< High priority debug message. */
#define MSG_LEGACY_ERROR /**< Error priority debug message. */
#define MSG_LEGACY_FATAL /**< Fatal priority debug message. */

/* Undefine earlier definitions of MSG macros, and define for SIM */
#undef MSG
#undef MSG_1
#undef MSG_2
#undef MSG_3
#undef MSG_4
#undef MSG_5
#undef MSG_6
#undef MSG_7
#undef MSG_8
#undef MSG_9
#undef MSG_FATAL

#elif defined(_WIN64) || defined(_WIN32)
/**
Place holder for diag MSG_ utils definitions for Windows platforms.
ARM platforms dont support Diag, so need to define MSG_ macros.
*/

/* Define SSID and priotiy masks*/
#define MSG_LEGACY_LOW   /**< Low priority debug message. */
#define MSG_LEGACY_MED   /**< Medium priority debug message. */
#define MSG_LEGACY_HIGH  /**< High priority debug message. */
#define MSG_LEGACY_ERROR /**< Error priority debug message. */
#define MSG_LEGACY_FATAL /**< Fatal priority debug message. */

/* Undefine earlier definitions of MSG macros, and define for SIM */
#undef MSG
#undef MSG_1
#undef MSG_2
#undef MSG_3
#undef MSG_4
#undef MSG_5
#undef MSG_6
#undef MSG_7
#undef MSG_8
#undef MSG_9
#undef MSG_FATAL

#endif /* define SSID*/

/*For simultation purposes */
#if defined(SIM)

#define DBG_LOW_PRIO   MSG_LEGACY_LOW   /**< Low priority debug message. */
#define DBG_MED_PRIO   MSG_LEGACY_MED   /**< Medium priority debug message. */
#define DBG_HIGH_PRIO  MSG_LEGACY_HIGH  /**< High priority debug message. */
#define DBG_ERROR_PRIO MSG_LEGACY_ERROR /**< Error priority debug message. */
#define DBG_FATAL_PRIO MSG_LEGACY_FATAL /**< Fatal priority debug message. */

/** Enable (1) or disable (0) the debug messagef */
#define mmlog_debugmsg_enable 1

/** Variable for setting priority threshold for the debug message in simulator. */
extern uint8_t mmlogs_debugmsg_lowest_prio;

/*
DBG_MSG for debugging. Currently compile time as we are supposed
to have a single build. Prints the file and line number and the
message. Message is printf format and takes a variable number of
arguments.
*/

/*if __FILENAME macro is not available, just leave that field blank  */
#ifndef __FILENAME__
#define __FILENAME__ ""
#endif

/* assert is for priority == MSG_LEGACY_FATAL (i.e on FATAL)
 * disable warning "conditional expression is constant" for mmlog_debugmsg_enable */
#if defined(__QDSP6_ARCH__)

#include <hexagon_sim_timer.h>

// clang-format off
#define PRIO_STR(prio) \
      ( (prio == DBG_LOW_PRIO)      ?  " LOW    "  :     \
      ( (prio == DBG_MED_PRIO)      ?  " MEDIUM "  :     \
      ( (prio == DBG_HIGH_PRIO)     ?  " HIGH   "  :     \
      ( (prio == DBG_ERROR_PRIO)    ?  " ERROR  "  :     \
                                       " FATAL  " ))))

// clang-format on
#undef DBG_MSG
#undef DBG_MSG_CAST


#if defined (MM_DEBUG_DISABLE)
#define DBG_MSG(num_args, priority, message, ...)

#elif defined (MM_MSG_USE_MM_DBG_LOG_COSIM)

/* This is used to guard against resolving #include "mm_msg.h" with other
 * header files named mm_msg.h
 */
#define MM_MSG_HAVE_MM_DBG_LOG

typedef union mm_dbg_log_header_word_t mm_dbg_log_header_word_t;
#include "gk_begin_pack.h"
union mm_dbg_log_header_word_t
{
   uint32_t w;
   struct {
      uint32_t msg_line :20;
      uint32_t msg_priority : 5;
      uint32_t domain_id :3;
      uint32_t num_words : 4;
   } b;
}
#include "gk_end_pack.h"
;

typedef struct mm_dbg_log_msg_t
{
   mm_dbg_log_header_word_t msg_header;
   const char* msg_format_ptr;
   const char* msg_filename_ptr;
}
mm_dbg_log_msg_t;

/* The time calculations in the "standard" DBG_MSG below seem to assume
 * a fixed CPU clock of 500MHz. The time range which can fit in an uint32_t
 * using a right shifted 500MHz clock would be:
 *   bits     range (s)
 *   0           8.6
 *   4         137.4
 *   8        2199.0
 *   10       8796.1
 */
#define MM_DBG_LOG_CYCLES_SHIFT (8)

void mm_dbg_log(const mm_dbg_log_msg_t* msg_info_ptr, uint32_t cycles_scaled, ...);
void mm_dbg_log_init(void);

#define DBG_MSG_LOG_COSIM(num_args, priority, message, ...)   \
      { \
         static const char xx_mm_log_format[] QSR_ATTR = (message); \
         static const char xx_mm_log_file[] QSR_ATTR = __FILENAME__; \
         static const mm_dbg_log_msg_t xx_mm_log_msg QSR_ATTR = \
         { \
            .msg_header.b.msg_line = __LINE__, \
            .msg_header.b.msg_priority = (priority), \
            .msg_header.b.domain_id = (mm_msg_domain_id),\
            .msg_header.b.num_words = 2 + (num_args), \
            .msg_format_ptr = xx_mm_log_format, \
            .msg_filename_ptr = xx_mm_log_file \
         }; \
         uint32_t cycles = (uint32_t)(hexagon_sim_read_pcycles() >> MM_DBG_LOG_CYCLES_SHIFT); \
         mm_dbg_log(&xx_mm_log_msg, cycles, ##__VA_ARGS__); \
}


#define DBG_MSG(num_args, priority, message, ...) \
   do { \
      if (mmlog_debugmsg_enable && (priority >= mmlogs_debugmsg_lowest_prio)) \
      { \
         DBG_MSG_LOG_COSIM(num_args, priority, message, ##__VA_ARGS__) \
         if (MSG_LEGACY_FATAL == (priority)) \
         { \
            assert(0); \
         } \
      } \
   } while(0)

#define DBG_MSG_CAST (uint32_t)
#elif defined(MM_MSG_USE_RPD_MM_DBG_LOG_COSIM)
//////////////////////////

/* This is used to guard against resolving #include "mm_msg.h" with other
 * header files named mm_msg.h
 */
#define MM_MSG_HAVE_MM_DBG_LOG

typedef union mm_rpd_dbg_log_header_word_t mm_rpd_dbg_log_header_word_t;
#include "gk_begin_pack.h"
union mm_rpd_dbg_log_header_word_t
{
   uint32_t w;
   struct {
      uint32_t msg_line :20;
      uint32_t msg_priority : 5;
      uint32_t domain_id :3;
      uint32_t num_words : 4;
   } b;
}
#include "gk_end_pack.h"
;

typedef struct mm_rpd_dbg_log_msg_t
{
   mm_rpd_dbg_log_header_word_t msg_header;
   const char* msg_format_ptr;
   const char* msg_filename_ptr;
}
mm_rpd_dbg_log_msg_t;

/* The time calculations in the "standard" DBG_MSG below seem to assume
 * a fixed CPU clock of 500MHz. The time range which can fit in an uint32_t
 * using a right shifted 500MHz clock would be:
 *   bits     range (s)
 *   0           8.6
 *   4         137.4
 *   8        2199.0
 *   10       8796.1
 */
#define MM_DBG_LOG_CYCLES_SHIFT (8)

void mm_rpd_dbg_log(const mm_rpd_dbg_log_msg_t* msg_info_ptr, uint32_t cycles_scaled, ...);
void mm_rpd_dbg_log_init(void);

#define DBG_MSG_LOG_COSIM(num_args, priority, message, ...)   \
      { \
         static const char xx_mm_log_format[] QSR_ATTR = (message); \
         static const char xx_mm_log_file[] QSR_ATTR = __FILENAME__; \
         static const mm_rpd_dbg_log_msg_t xx_mm_log_msg QSR_ATTR = \
         { \
            .msg_header.b.msg_line = __LINE__, \
            .msg_header.b.msg_priority = (priority), \
            .msg_header.b.domain_id = (mm_msg_domain_id),\
            .msg_header.b.num_words = 2 + (num_args), \
            .msg_format_ptr = xx_mm_log_format, \
            .msg_filename_ptr = xx_mm_log_file \
         }; \
         uint32_t cycles = (uint32_t)(hexagon_sim_read_pcycles() >> MM_DBG_LOG_CYCLES_SHIFT); \
         mm_rpd_dbg_log(&xx_mm_log_msg, cycles, ##__VA_ARGS__); \
}


#define DBG_MSG(num_args, priority, message, ...) \
   do { \
      if (mmlog_debugmsg_enable && (priority >= mmlogs_debugmsg_lowest_prio)) \
      { \
         DBG_MSG_LOG_COSIM(num_args, priority, message, ##__VA_ARGS__) \
         if (MSG_LEGACY_FATAL == (priority)) \
         { \
            assert(0); \
         } \
      } \
   } while(0)

#define DBG_MSG_CAST (uint32_t)
   //////////////////////////
#else  // !MM_DEBUG_DISABLE and !MM_MSG_USE_MM_DBG_LOG_COSIM and !MM_MSG_USE_RPD_MM_DBG_LOG_COSIM
#define DBG_MSG(num_args, priority, message, ...)                                                                                \
   do                                                                                                                  \
   {                                                                                                                   \
      if (mmlog_debugmsg_enable && (priority >= mmlogs_debugmsg_lowest_prio))                                          \
      {                                                                                                                \
         uint64_t cycles = hexagon_sim_read_pcycles();                                                                 \
         int      tms    = ((cycles >> 10) * 2148) >> 20;                                                              \
         int      tsec   = ((cycles >> 10) * 2200) >> 30;                                                              \
         int      tmin   = ((cycles >> 10) * 37) >> 30;                                                                \
         printf("%-6s %02d:%02d:%03d @ %25.25s : %4d : " message "\n",                                                 \
                PRIO_STR(priority),                                                                                    \
                tmin,                                                                                                  \
                tsec - 60 * tmin,                                                                                      \
                tms - 1000 * tsec,                                                                                     \
                __FILENAME__,                                                                                          \
                __LINE__,                                                                                              \
                ##__VA_ARGS__);                                                                                        \
         if (MSG_LEGACY_FATAL == priority)                                                                             \
            assert(0);                                                                                                 \
      }                                                                                                                \
   } while (0)
#endif // MM_DEBUG_DISABLE /* experimental mm log */
#elif defined(__ARMCC_VERSION) || defined(__GNUC__) || defined(_WIN32) || defined(_WIN64)

#undef DBG_MSG
#define DBG_MSG(num_args, priority, message, ...)                                                                                \
   do                                                                                                                  \
   {                                                                                                                   \
      if (mmlog_debugmsg_enable && (priority >= mmlogs_debugmsg_lowest_prio))                                          \
      {                                                                                                                \
         printf(message "\n", __LINE__, __FILENAME__, ##__VA_ARGS__);                                                  \
         if (MSG_LEGACY_FATAL == priority)                                                                             \
            assert(0);                                                                                                 \
      }                                                                                                                \
   } while (0)

#else

#error "Unknown compiler"

#endif /* DBG_MSG*/

/* Undefine earlier definitions of MSG macros, and define for SIM */
#undef MSG
#undef MSG_1
#undef MSG_2
#undef MSG_3
#undef MSG_4
#undef MSG_5
#undef MSG_6
#undef MSG_7
#undef MSG_8
#undef MSG_9
#undef MSG_FATAL

#ifndef DBG_MSG_CAST
#define DBG_MSG_CAST
#endif

#define MSG(xx_ss_id, xx_ss_mask, xx_fmt) DBG_MSG(0, xx_ss_mask, xx_fmt)

#define MSG_1(xx_ss_id, xx_ss_mask, xx_fmt, xx_arg1)                                                                   \
   DBG_MSG(1, xx_ss_mask, xx_fmt, DBG_MSG_CAST xx_arg1)

#define MSG_2(xx_ss_id, xx_ss_mask, xx_fmt, xx_arg1, xx_arg2)                                                          \
   DBG_MSG(2, xx_ss_mask, xx_fmt, DBG_MSG_CAST xx_arg1, DBG_MSG_CAST xx_arg2)

#define MSG_3(xx_ss_id, xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3)                                                 \
   DBG_MSG(3, xx_ss_mask, xx_fmt, DBG_MSG_CAST xx_arg1, DBG_MSG_CAST xx_arg2, DBG_MSG_CAST xx_arg3)

#define MSG_4(xx_ss_id, xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4)                                        \
   DBG_MSG(4, xx_ss_mask, xx_fmt, DBG_MSG_CAST xx_arg1, DBG_MSG_CAST xx_arg2, DBG_MSG_CAST xx_arg3,                    \
           DBG_MSG_CAST xx_arg4)

#define MSG_5(xx_ss_id, xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4, xx_arg5)                               \
   DBG_MSG(5, xx_ss_mask, xx_fmt, DBG_MSG_CAST xx_arg1, DBG_MSG_CAST xx_arg2, DBG_MSG_CAST xx_arg3,                    \
           DBG_MSG_CAST xx_arg4, DBG_MSG_CAST xx_arg5)

#define MSG_6(xx_ss_id, xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4, xx_arg5, xx_arg6)                      \
   DBG_MSG(6, xx_ss_mask, xx_fmt, DBG_MSG_CAST xx_arg1, DBG_MSG_CAST xx_arg2, DBG_MSG_CAST xx_arg3,                    \
           DBG_MSG_CAST xx_arg4, DBG_MSG_CAST xx_arg5, DBG_MSG_CAST xx_arg6)

#define MSG_7(xx_ss_id, xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4, xx_arg5, xx_arg6, xx_arg7)             \
   DBG_MSG(7, xx_ss_mask, xx_fmt, DBG_MSG_CAST xx_arg1, DBG_MSG_CAST xx_arg2, DBG_MSG_CAST xx_arg3,                    \
           DBG_MSG_CAST xx_arg4, DBG_MSG_CAST xx_arg5, DBG_MSG_CAST xx_arg6, DBG_MSG_CAST xx_arg7)

#define MSG_8(xx_ss_id, xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4, xx_arg5, xx_arg6, xx_arg7, xx_arg8)    \
   DBG_MSG(8, xx_ss_mask, xx_fmt, DBG_MSG_CAST xx_arg1, DBG_MSG_CAST xx_arg2, DBG_MSG_CAST xx_arg3,                    \
           DBG_MSG_CAST xx_arg4, DBG_MSG_CAST xx_arg5, DBG_MSG_CAST xx_arg6, DBG_MSG_CAST xx_arg7,                     \
           DBG_MSG_CAST xx_arg8)

#define MSG_9(                                                                                                         \
   xx_ss_id, xx_ss_mask, xx_fmt, xx_arg1, xx_arg2, xx_arg3, xx_arg4, xx_arg5, xx_arg6, xx_arg7, xx_arg8, xx_arg9)      \
   DBG_MSG(9, xx_ss_mask, xx_fmt, DBG_MSG_CAST xx_arg1, DBG_MSG_CAST xx_arg2, DBG_MSG_CAST xx_arg3,                    \
           DBG_MSG_CAST xx_arg4, DBG_MSG_CAST xx_arg5, DBG_MSG_CAST xx_arg6, DBG_MSG_CAST xx_arg7,                     \
           DBG_MSG_CAST xx_arg8, DBG_MSG_CAST xx_arg9)

#else  // redefine prio macros for non-SIM

/** @name Legacy debug priority messages
@{ */
#define DBG_LOW_PRIO MSG_LEGACY_LOW /**< Low priority debug message. */
#define DBG_MED_PRIO MSG_LEGACY_MED /**< Medium priority debug message. */
#define DBG_HIGH_PRIO MSG_LEGACY_HIGH   /**< High priority debug message. */
#define DBG_ERROR_PRIO MSG_LEGACY_ERROR /**< Error priority debug message. */
#define DBG_FATAL_PRIO MSG_LEGACY_FATAL /**< Fatal priority debug message. */
/** @} */ /* end_namegroup Legacy debug priority messages */

#endif /* SIMULATION */

/* Supporting macros for MM_MSG() logging utility.
 * MM_MSG is supported on both target and simulation builds. */

/* Utility to get variable argument
 * Slide the variable arguments to get the string '_N' to be appended */
#define MM_VA_NUM_ARGS_IMPL(a, b, c, d, e, f, g, h, i, j, _N, ...) _N
#define MM_VA_NUM_ARGS(...) MM_VA_NUM_ARGS_IMPL(, ##__VA_ARGS__, _9, _8, _7, _6, _5, _4, _3, _2, _1, )
/* Concatenates string x with y*/
#define MM_TOKENPASTE(x, y) x##y

/*Macro parses the text to MSG with _N as suffix, where N = {1, 2, 3, .... ,9} */
#define MM_MSG_x(_N) MM_TOKENPASTE(MSG, _N)

#ifndef MSG_SSID_DFLT
#define MSG_SSID_DFLT MSG_SSID_QDSP6
#endif // MSG_SSID_DFLT

//#ifndef USES_AUDIO_IN_ISLAND
/* This Logging macro supports variable arguments*/
#define MM_MSG(xx_ss_mask, xx_fmt, ...)                                                                                \
   MM_MSG_x(MM_VA_NUM_ARGS(__VA_ARGS__))(MSG_SSID_QDSP6, xx_ss_mask, xx_fmt, ##__VA_ARGS__)

// #else//island builds
//    #define MM_MSG(xx_ss_mask, xx_fmt, ...) MICRO_MSG(MSG_SSID_QDSP6, xx_ss_mask, xx_fmt, ##__VA_ARGS__)
//    // #define MM_MSG(xx_ss_mask, xx_fmt, ...)                                                                             \
//    //    if (qurt_island_get_status())                                                                                    \
//    //       MICRO_MSG(MSG_SSID_QDSP6, xx_ss_mask, xx_fmt, ##__VA_ARGS__)
//    //    else                                                                                                             \
//    //       MM_MSG_x(MM_VA_NUM_ARGS(__VA_ARGS__))(MSG_SSID_QDSP6, xx_ss_mask, xx_fmt, ##__VA_ARGS__)

// #endif   
/** @} */ /* end_addtogroup mm_logging */

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // #ifndef _MM_MSG_BASE_H
