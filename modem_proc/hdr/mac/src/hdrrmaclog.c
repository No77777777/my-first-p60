/*===*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

               H D R   R M A C   L O G   F U N C T I O N S

GENERAL DESCRIPTION

  This module provides general purpose logging routines used in various
  states for rmac logging module.

EXTERNALIZED FUNCTIONS (Global)

  hdrrmaclog_update_tune_away_rri_info
  hdrrmaclog_reset_tune_away_rri_info
  hdrrmaclog_log_tune_away_rri_info
  hdrrmaclog_reset_conn_state_stats
  hdrrmaclog_inc_conn_state_stats_counter 
  hdrrmaclog_log_conn_state_stats


EXTERNALIZED FUNCTIONS (Regional)


INITIALIZATION AND SEQUENCING REQUIREMENTS


  Copyright (c) 2004 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

============================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/hdr.mpss/6.0/mac/src/hdrrmaclog.c#1 $

when         who     what, where, why
----------   ---     --------------------------------------------------------- 
12/16/2013   sat     Changed memcpy() to assignment or memscpy() (safer version).
06/10/2010   kss     Remove cast of REX_ENABLE_PREMPTION due to Core change.
05/18/2010   arm     Typecast REX_ENABLE_PREMPTION() to void for Lint warnings  
12/04/2008   rmg     Removed intlocks.
12/04/2008   rmg     Lint fixes.
08/23/2007   etv     Fixed high lint warnings.
08/05/2004   grl     Created file.

==========================================================================*/

/* <EJECT> */
/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/

#include "customer.h"
#include "hdrrmaclog.h"
#include "hdrlogi.h"

/*===========================================================================

            DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

#define HDRRMACLOG_CONN_STATE_STATS_VER 0
  /* Version number of the connected state stats packet. */

#define HDRRMACLOG_RRI_INFO_STATS_VER 0
  /* Version number of the RRI info stats packet. */

/* Internal state information for hdrrmaclog. */
struct {
  hdrrmaclog_tune_away_rri_info_struct_subtype rri_info;
    /* RRI related information before and after the tune away. */

  hdrrmaclog_conn_state_stats_struct_subtype conn_state_stats;
    /* Connected state statistics related to the RMAC layer. */

} hdrrmaclog_i;

/*===========================================================================

                      FUNCTION  DEFINITIONS

===========================================================================*/

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRRMACLOG_UPDATE_TUNE_AWAY_RRI_INFO

DESCRIPTION  : This function saves the RRI information before and after 
               the last HDR to 1x tune away so that it may be logged
               when requested.
               
DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrrmaclog_update_tune_away_rri_info(
  hdrrmac_rate_enum_type rri_before,
    /* RRI before the tune away. */

  hdrrmac_rate_enum_type rri_after
    /* RRI after the tune away. */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Enter critical section. */
  REX_DISABLE_PREMPTION();

  /* Save the RRI information before and after the tune away. */
  hdrrmaclog_i.rri_info.hdrtc_tune_away_tx_rri = (uint8) rri_before;
  hdrrmaclog_i.rri_info.hdrtc_tune_back_tx_rri = (uint8) rri_after;

  /* Save accumulated RRI values for before and after the tune away
     along with the total number of rri values accululated since the
     start of the last call. */
  hdrrmaclog_i.rri_info.since_call.total_rri_count++;
  hdrrmaclog_i.rri_info.since_call.cum_hdrtc_tune_away_rri += 
    (uint8) rri_before;
  hdrrmaclog_i.rri_info.since_call.cum_hdrtc_tune_back_rri += 
    (uint8) rri_after;

  /* Save accumulated RRI values for before and after the tune away
     along with the total number of rri values accululated since the
     last reset. */
  hdrrmaclog_i.rri_info.since_reset.total_rri_count++;
  hdrrmaclog_i.rri_info.since_reset.cum_hdrtc_tune_away_rri += 
    (uint8) rri_before;
  hdrrmaclog_i.rri_info.since_reset.cum_hdrtc_tune_back_rri += 
    (uint8) rri_after;

  /* Exit critical section */
  REX_ENABLE_PREMPTION();

} /* hdrrmaclog_update_tune_away_rri_info */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRRMACLOG_LOG_TUNE_AWAY_RRI_INFO

DESCRIPTION  : This function logs the tune_away info log packet. It gets a
               log record from the diag log buffer, fills it with the latest 
               connected state info and commits the log record to
               the diag for delivery. 
               
DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrrmaclog_log_tune_away_rri_info( void )
{
  LOG_HDR_TUNE_AWAY_RRI_INFO_C_type *log_ptr;
    /* Pointer to the log structure. */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  
  log_ptr = (LOG_HDR_TUNE_AWAY_RRI_INFO_C_type *)
            log_alloc( (log_code_type) LOG_HDR_TUNE_AWAY_RRI_INFO_C,
                       sizeof( LOG_HDR_TUNE_AWAY_RRI_INFO_C_type ) );
    /* Actually log the packet if the log mask is set properly. */

  if ( log_ptr )
  {
    /* Enter critical section. */
    REX_DISABLE_PREMPTION();

#ifdef FEATURE_MEMCPY_REMOVAL
    log_ptr->rri_info = hdrrmaclog_i.rri_info;
#else
    memcpy( (void *)( &log_ptr->rri_info ),
            (void *)( &hdrrmaclog_i.rri_info ),
            sizeof( hdrrmaclog_tune_away_rri_info_struct_subtype ) );
#endif /* FEATURE_MEMCPY_REMOVAL */
      /* Copy log info into the log record */

    /* Exit critical section */
    REX_ENABLE_PREMPTION();

    log_commit( ( log_type * ) log_ptr );
      /* Commit the log packet to the diag port. */
  }
} /* hdrrmaclog_log_tune_away_rri_info */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHLOG_RESET_TUNE_AWAY_RRI_INFO

DESCRIPTION  : This function resets the saved rri information from before
               and after the last tune away.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrrmaclog_reset_tune_away_rri_info
( 
  hdrrmaclog_reset_stats_type stats_reset
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  
  /* Enter critical section. */
  REX_DISABLE_PREMPTION();

  switch ( stats_reset )
  {
    case HDRRMACLOG_ALL_STATS:

      memset( ( void* ) &hdrrmaclog_i.rri_info, 0, 
              sizeof( hdrrmaclog_tune_away_rri_info_struct_subtype ) );
        /* Reset all of the statistics tracked for this log packet. */

      hdrrmaclog_i.rri_info.version = HDRRMACLOG_RRI_INFO_STATS_VER;
        /* Set the version number of the packet for future expandability. */

      break;

    case HDRRMACLOG_LAST_CALL_STATS:

      memset( ( void* ) &hdrrmaclog_i.rri_info.since_call, 0, 
              sizeof( hdrrmaclog_rri_stats_struct ) );
        /* Reset only the statistics tracked since the start of the
           last call. */

      /* Reset information about the current tune away. */
      hdrrmaclog_i.rri_info.hdrtc_tune_back_tx_rri = 0;
      hdrrmaclog_i.rri_info.hdrtc_tune_away_tx_rri = 0;

      break;
  }

  /* Exit critical section */
  REX_ENABLE_PREMPTION();

} /* hdrrmaclog_reset_tune_away_rri_info */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRRMACLOG_LOG_CONN_STATE_STATS

DESCRIPTION  : This function logs the connected state info. It gets a log
               record from the diag log buffer, fills it with the latest 
               connected state info and commits the log record to
               the diag for delivery.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrrmaclog_log_conn_state_stats( void )
{
  LOG_HDR_CONN_STATE_RMAC_STATS_C_type *log_ptr;  
    /* Pointer to the log struct. */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  
  log_ptr = (LOG_HDR_CONN_STATE_RMAC_STATS_C_type *)
            log_alloc( (log_code_type) LOG_HDR_CONN_STATE_RMAC_STATS_C,
                       sizeof( LOG_HDR_CONN_STATE_RMAC_STATS_C_type ) );
    /* Actually log the packet if the log mask is set properly. */

  if ( log_ptr )
  {
    /* Enter critical section. */
    REX_DISABLE_PREMPTION();

#ifdef FEATURE_MEMCPY_REMOVAL
    log_ptr->conn_state_rmac_stats  = hdrrmaclog_i.conn_state_stats ;
#else
    memcpy( (void *)( &log_ptr->conn_state_rmac_stats ),
            (void *)( &hdrrmaclog_i.conn_state_stats ),
            sizeof( hdrrmaclog_conn_state_stats_struct_subtype ) );
#endif /* FEATURE_MEMCPY_REMOVAL */
      /* Copy log info into the log record */

    /* Exit critical section */
    REX_ENABLE_PREMPTION();

    log_commit( ( log_type * ) log_ptr );
      /* Commit the log packet to the diag port. */
  }
} /* hdrrmaclog_log_conn_state_stats */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRRMACLOG_INC_CONNECTED_STATE_STATS_COUNTER

DESCRIPTION  : Increments the appropriate connected state counter by
               the specified amount.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrrmaclog_inc_conn_state_stats_counter
( 
  hdrrmaclog_conn_state_stats_count_type counter,
    /* The counter to increment. */

  uint16                                 inc_count
    /* How much to increment the counter by. */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  
  /* Enter critical section. */
  REX_DISABLE_PREMPTION();

  hdrrmaclog_i.conn_state_stats.since_call_counts[counter] += inc_count;
    /* Increment the specified counter kept since the last AT reset. */

  hdrrmaclog_i.conn_state_stats.since_reset_counts[counter] += inc_count;
    /* Increment the specified counter kept since the beginning of
       the last call. */

  /* Exit critical section */
  REX_ENABLE_PREMPTION();

} /* hdrrmaclog_inc_conn_state_stats_counter */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRRMACLOG_RESET_CONN_STATE_STATS

DESCRIPTION  : This function resets the RMAC connected state stats as
               specified by the stats reset parameter.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrrmaclog_reset_conn_state_stats
( 
  hdrrmaclog_reset_stats_type stats_reset
    /* Which statistics to clear... all or just the ones related to the 
       last call. */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Enter critical section. */
  REX_DISABLE_PREMPTION();

  switch ( stats_reset )
  {
    case HDRRMACLOG_ALL_STATS:
      memset( ( void* ) &hdrrmaclog_i.conn_state_stats, 0, 
              sizeof( hdrrmaclog_conn_state_stats_struct_subtype ) );
        /* Reset all of the statistics tracked for this log packet. */

      hdrrmaclog_i.conn_state_stats.version = HDRRMACLOG_CONN_STATE_STATS_VER;
        /* Set the version number of the packet for future expandability. */

      break;
    case HDRRMACLOG_LAST_CALL_STATS:
      memset( ( void* ) &hdrrmaclog_i.conn_state_stats.since_call_counts[0], 0,
              sizeof( uint32 ) * (uint8) HDRRMACLOG_NUM_CONN_STATE_STATS_COUNTS );
        /* Reset only the statistics tracked since the start of the
           last call. */

      break;

  }

  /* Exit critical section */
  REX_ENABLE_PREMPTION();

} /* hdrrmaclog_reset_conn_state_stats */

