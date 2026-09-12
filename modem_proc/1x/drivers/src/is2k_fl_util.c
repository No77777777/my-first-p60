/*!
  \page decutil IS-2000 Forward Link Decoder Module External Utilities
*/
/*=============================================================================

Copyright(c) 2020 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/1x.mpss/8.0/drivers/src/is2k_fl_util.c#2 $

when       who     what, where, why
--------   ---     ------------------------------------------------------------
01/22/20   vaa     Implement QSH MDUMP framework
02/27/15   jh      Updates to work on Jolokia 
02/12/15   jh      Initial version

=============================================================================*/

/* External interfaces */
#include <stdio.h>
#include <stdarg.h>
#include "modem_mem.h"
#include "m1x_hwio_mpss.h"
#include "m1x_diag.h"
#include "err.h"
#include "modem_1x_utils.h"

/* Other */
#include "m1x_time_i.h"

/* Public interfaces */
#include "is2k_fl_util.h"

void *is2k_fl_malloc_ext( size_t size )
{
  return modem_mem_alloc( size, MODEM_MEM_CLIENT_1X );
}

void is2k_fl_free_ext( void *pMem )
{
  modem_mem_free( pMem, MODEM_MEM_CLIENT_1X );
}

void is2k_fl_debug_ext( int level, const char *fmt, ... )
{
  char out_string[100];
  va_list argp;

  /* Splat arguments into a string formatter */
  va_start(argp,fmt);
  vsnprintf(out_string,sizeof(out_string),fmt,argp);
  va_end(argp);

  /* Push out the string constructed above to a DIAG F3 string */
  M1X_STR( MUX, LEGACY_HIGH, "%s", out_string );

  /* We only get here if the library indicates that graceful recovery is not
     possible. */
  if (level < 0)
  {
    ONEX_ERR_FATAL("Decoder assert with error code (%d)",level,0,0);
  }
}

size_t is2k_fl_timestamp_ext( void )
{
  /* Return the XO counter (19.2MHz nominal rate) */
  #if   defined( HWIO_STMR_TIME_RD_ADDR )  
  /* for Jolokia, need to check 1st b/c HWIO_UNIV_STMR_MSTMR_ADDR is defined 
     but we should not use */
  return ( HWIO_IN(STMR_TIME_RD) );
  #elif defined( HWIO_UNIV_STMR_MSTMR_ADDR )
  return( HWIO_IN(UNIV_STMR_MSTMR) );
  #elif defined( HWIO_MSS_STMR_TIME_RD_ADDR )
  return( HWIO_IN(MSS_STMR_TIME_RD) );
  #else
  #error "Need hardware register for XO counter"
  #endif
}

size_t is2k_fl_timestamp_rollover( void )
{
    #if   defined( HWIO_STMR_TIME_RD_ADDR )  
    return( HWIO_STMR_TIME_RD_RMSK + 1 );
    #elif defined( HWIO_UNIV_STMR_MSTMR_ADDR )
    return( HWIO_UNIV_STMR_MSTMR_RMSK + 1 );
    #elif defined( HWIO_MSS_STMR_TIME_RD_ADDR )
    return( HWIO_MSS_STMR_TIME_RD_RMSK + 1 );
    #else
    #error "Need hardware register for XO counter mask"
    #endif
}

size_t is2k_fl_timestamp_delta_ext( size_t start, size_t finish )
{
  size_t duration;

  /* Handle any rollover cases, where finish could be smaller than start */
  if ( finish < start )
  {
    finish = finish + is2k_fl_timestamp_rollover();
  }

  /* Convert the result into microseconds, given the XO nom rate is 19.2M */
  duration = ( ( finish - start ) * 10 ) / 192;

  return( duration );
}
