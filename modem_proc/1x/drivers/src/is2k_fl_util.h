#ifndef IS2K_FL_UTIL_H
#define IS2K_FL_UTIL_H
/*!
  \page decutilapi IS-2000 Forward Link Decoder Module External Utilities API
*/
/*=============================================================================

Copyright(c) 2015 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/1x.mpss/8.0/drivers/src/is2k_fl_util.h#1 $

when       who     what, where, why
--------   ---     ------------------------------------------------------------
02/27/15   jh      Updates to work on Jolokia 
02/12/15   jh      Initial version

=============================================================================*/

/* External interfaces */
#include <stddef.h>

/*! Define debug level below which the decoder module will call API
    is2k_fl_debug_ext() */
#define DEBUG_LEVEL 0

/*! Caller must implement a malloc() equivalent routine, to be paired
    with is2k_fl_free_ext() */
void *is2k_fl_malloc_ext( size_t size );

/*! Caller must implement a free() equivalent routine, to be paired
    with is2k_fl_malloc() */
void is2k_fl_free_ext( void *pMem );

/*! Caller must implement a routine to (possibly) emit debug information.
    If level is negative, the function should assert/terminate. */
void is2k_fl_debug_ext( int level, const char *fmt, ... );

/*! Caller must implement a timestamping routine if the library is compiled
    with profiling enabled. */
size_t is2k_fl_timestamp_ext( void );

/*! Returns the max size of the timestamp + 1, used to calculate rollover */
size_t is2k_fl_timestamp_rollover( void );

/*! Caller must implement a timestamp delta routine if the library is compiled
    with profiling eneabled.  Can return whatever units are desired, but
    microseconds are preferred. */
size_t is2k_fl_timestamp_delta_ext( size_t start, size_t finish );

#endif /* IS2K_FL_UTIL_H */
