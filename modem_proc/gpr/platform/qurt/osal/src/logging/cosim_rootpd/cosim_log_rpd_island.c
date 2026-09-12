/*========================================================================
Copyright (c) 2018-2022 Qualcomm Technologies, Inc.
All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.
======================================================================== */
/**
@file rpd_cosim_log_island.c

@brief This file contains a utility for generating diagnostic messages.
*/

/*========================================================================
Edit History

when       who     what, where, why
--------   ---     -------------------------------------------------------
2018-09-11 mtaneda Original Implementation.
04/25/19   akr     Adaptation to spf rootpd.
========================================================================== */

#include "cosim_log_rpd_island_i.h"
#include "ar_osal_log_i.h"

static uint32_t convert_ar_log_lvl_to_cosim_lvl(uint32_t ar_lvl)
{
   switch(ar_lvl)
   {
      case AR_LOW_PRIO:
      {
         return MSG_LEGACY_LOW;
      }
      case AR_MED_PRIO:
      {
         return MSG_LEGACY_MED;
      }
      case AR_HIGH_PRIO:
      {
         return MSG_LEGACY_HIGH;
      }      
      case AR_ERROR_PRIO:
      {
         return MSG_LEGACY_ERROR;
      }      
      case AR_FATAL_PRIO:
      {
         return MSG_LEGACY_FATAL;
      }
      default:
      {
         return MSG_LEGACY_LOW;
      }
   }
}

static const uint32_t ar_msg_domain_id = 1; // MDSP

/*
 * The size of the buffer must be a power of two.
 * In simulation, it can be relatively small, since it is read out by a cosim.
 * If this was going to be used on a target, the buffer would have to be sized
 * according to how fast debug tools can poll and extract it.
 */
#define AR_DBG_LOG_BUFFER_SIZE_WORDS 256

#define AR_DBG_LOG_BUFFER_SIZE_MASK (AR_DBG_LOG_BUFFER_SIZE_WORDS - 1)

/* Option to put the ar_rpd_dbg_log data in a specific section, etc. */
#define AR_DBG_LOG_ATTR

/*******************************************************
 * Data Definitions
 *******************************************************/

unsigned int AR_DBG_LOG_ATTR ar_rpd_dbg_log_wr_offset = 0;

uint32_t AR_DBG_LOG_ATTR ar_rpd_dbg_log_buffer[AR_DBG_LOG_BUFFER_SIZE_WORDS];

/* Exported only for the benefit of the dbg_log cosim which reads out this debug buffer */
extern unsigned int AR_DBG_LOG_ATTR ar_rpd_dbg_log_buffer_size_words;
unsigned int AR_DBG_LOG_ATTR ar_rpd_dbg_log_buffer_size_words = AR_DBG_LOG_BUFFER_SIZE_WORDS;

/*******************************************************
 * Function Definitions
 *******************************************************/

static inline __attribute__((always_inline)) unsigned int atomic_add_masked_return(unsigned int *target,
                                                                                   unsigned int  v,
                                                                                   unsigned int  mask)
{
   unsigned int result;

   __asm__ __volatile__("1:     %0 = memw_locked(%2)\n"
                        "       %0 = add(%0, %3)\n"
                        "       %0 = and(%0, %4)\n"
                        "       memw_locked(%2, p0) = %0\n"
                        "       if !p0 jump 1b\n"
                        : "=&r"(result), "+m"(*target)
                        : "r"(target), "r"(v), "r"(mask)
                        : "p0");

   return result;
}

void ar_rpd_dbg_log(const ar_rpd_dbg_log_msg_t *msg_info_ptr, uint32_t time_ms, ...)
{

   // Re-assigning so that the cosim can find the symbol. If there's no reference, the compiler and linker will remove
   // it.
   ar_rpd_dbg_log_buffer_size_words = AR_DBG_LOG_BUFFER_SIZE_WORDS;

   va_list va_argp;

   /* Reserve words */
   uint32_t num_words = msg_info_ptr->msg_header.b.num_words;

   /* This returns the offset after adding (all the QURT atomic_ inline
    * assembly functions are like that, perhaps to reduce register usage).
    * So after completing the atomic operation, calculate back to the
    * starting offset.
    */
   uint32_t wr_offset = atomic_add_masked_return(&ar_rpd_dbg_log_wr_offset, num_words, AR_DBG_LOG_BUFFER_SIZE_MASK);

   wr_offset = (wr_offset - num_words) & AR_DBG_LOG_BUFFER_SIZE_MASK;

   ar_rpd_dbg_log_buffer[wr_offset] = (uintptr_t)msg_info_ptr;
   wr_offset                        = (wr_offset + 1) & AR_DBG_LOG_BUFFER_SIZE_MASK;

   ar_rpd_dbg_log_buffer[wr_offset] = time_ms;
   wr_offset                        = (wr_offset + 1) & AR_DBG_LOG_BUFFER_SIZE_MASK;

   va_start(va_argp, time_ms);
   uint32_t n_args = num_words - 2;
   while (n_args--)
   {
      ar_rpd_dbg_log_buffer[wr_offset] = va_arg(va_argp, uint32_t);
      wr_offset                        = (wr_offset + 1) & AR_DBG_LOG_BUFFER_SIZE_MASK;
   }
   va_end(va_argp);
}

void ar_rpd_dbg_log_init(void)
{
   /* Do not omit this function, or calls to it.
    * It is there to trigger a PC based callback
    * in the cosim which reads this log,
    * to complete the cosim's initialization.
    */
}

/**
Function to log with rootpd cosim
*/
void ar_log(uint32_t level, const char_t *log_tag, const char_t *fn, int32_t ln, const char_t *format, ...)
{

   bool_t is_island = FALSE;
#ifdef ISLAND_TEST
   is_island = qurt_island_get_status();
#else
    is_island = FALSE;
#endif
    if(is_island)
    {
        return;
    }

   // Print the variable arguments
   va_list _ArgList = { 0 };
   va_start(_ArgList, format);

   #define MAX_MESSAGE_SIZE 256

   char message[MAX_MESSAGE_SIZE];
   vsnprintf(message, MAX_MESSAGE_SIZE, format, _ArgList);

   // print
   uint32_t  xx_ss_mask = convert_ar_log_lvl_to_cosim_lvl(level);
   const ar_rpd_dbg_log_msg_t xx_mm_log_msg = { .msg_header.b.msg_line     = ln,
                                                .msg_header.b.msg_priority = xx_ss_mask,
                                                .msg_header.b.domain_id    = (ar_msg_domain_id),
                                                .msg_header.b.num_words    = 2,
                                                .msg_format_ptr            = (char *)message,
                                                .msg_filename_ptr          = fn };
   uint32_t                   time_ms        = (uint32_t)(hexagon_sim_read_pcycles() >> AR_DBG_LOG_CYCLES_SHIFT);
   ar_rpd_dbg_log(&xx_mm_log_msg, time_ms);

#ifdef DEBUG_AR_OSAL_LOG_UTIL
   static uint32_t counter = 0;
   printf("printed rootpd msg %ld, fn: %s, ln: %ld string: %s \n ", counter++, fn, ln, message);
#endif
   va_end(_ArgList);
}

void ar_dbg_log(const ar_rpd_dbg_log_msg_t* msg_info_ptr, uint32_t time_ms, ...)
{
  ar_rpd_dbg_log(msg_info_ptr, time_ms);
}
