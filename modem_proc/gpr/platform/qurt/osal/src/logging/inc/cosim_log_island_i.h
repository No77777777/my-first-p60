/*========================================================================
  Copyright (c) 2020-2022 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
======================================================================== */
/**
@file cosim_log_island_i.h

@brief This file declares cosim logging utility APIs.
*/

/*========================================================================
Edit History

$Header: 

when       who     what, where, why
--------   ---     -------------------------------------------------------
03/04/2020   rv      Created file.
========================================================================== */

#ifndef _COSIM_LOG_ISLAND_I_H_
#define _COSIM_LOG_ISLAND_I_H_

#include "ar_osal_types.h"
#include "ar_osal_timer.h"
#ifdef __qdsp6__
#include <hexagon_sim_timer.h>
#endif


#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

/* The time calculations in the "standard" DBG_MSG below seem to assume
 * a fixed CPU clock of 500MHz. The time range which can fit in an uint32_t
 * using a right shifted 500MHz clock would be:
 *   bits     range (s)
 *   0           8.6
 *   4         137.4
 *   8        2199.0
 *   10       8796.1
 */
#define AR_DBG_LOG_CYCLES_SHIFT (8)

typedef union ar_dbg_log_header_word_t ar_dbg_log_header_word_t;
union ar_dbg_log_header_word_t
{
   uint32_t w;
   struct
   {
      uint32_t msg_line : 20;
      uint32_t msg_priority : 5;
      uint32_t domain_id : 3;
      uint32_t num_words : 4;
   } b;
}
;

typedef struct ar_dbg_log_msg_t
{
   ar_dbg_log_header_word_t msg_header;
   const char *             msg_format_ptr;
   const char *             msg_filename_ptr;
} ar_dbg_log_msg_t;

void ar_dbg_log(const ar_dbg_log_msg_t *msg_info_ptr, uint32_t time_ms, ...);
void ar_dbg_log_init(void);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // #ifndef _COSIM_LOG_ISLAND_I_H_
