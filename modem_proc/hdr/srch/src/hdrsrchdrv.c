/*===*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                       H D R    S R C H    D R I V E R S

                           Search Drivers Module

GENERAL DESCRIPTION

   This file contains the drivers for the searcher for HDR MSM Chipset.
   Functions are provided to load the search list to the DSP, start
   searching, start TA searches and to read respective search results.
   The main difference between searechers in IS-95 chips and HDR chips
   is that HDR searcher is has most of its functionality in the DSP
   firmware. The microprocessor loads a list of searches and waits for
   the result interrupt.

EXTERNALIZED FUNCTIONS

REGIONAL FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

   Copyright (c) 2000 - 2013 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

============================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/hdr.mpss/6.0/srch/src/hdrsrchdrv.c#1 $

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when         who     what, where, why
----------   ---     -------------------------------------------------------
07/08/2015   rmv     Check and delay release div if div search is active 
08/22/2014   vke     Cancel the searches in ASDiv processing only 
                     when Searches are active
09/03/2013   wsh     Replaced FEATURE_DIME/TRITON_BOLT_MODEM with HDR version
04/22/2013   arm     Added changes for DSDA coexistence manager.
11/13/2012   vlc     Modified hdrsrchdrv_send_srch_list() to work with new 
                     Search4 API for optimized idle time line. 
03/16/2012   arm     Resolve TCB errors for Dime. 
03/19/2012   ljl     Fixed Dime compiler errors.
02/08/2012   rkc     Sent TICKLE sig when aborting srch callback is received.
02/01/2012   mbs     Modified burst length for connected mode measurement
01/11/2012   ljl     Added the parameter for search priority to hdrsrchdrv_enable_srch4().
10/17/2011   rmg     Removed inclusion of clk.h and clkregim.h.
07/07/2011   kss     Removed shutdown message for NikeL as it is not used.
04/27/2011   smd     IRAT L2DO updates for NikeL.
04/12/2011   smd     Used srch4 search structure. 
01/26/2011   ljl     Fixed the incorrect srch_cnt for the last carrier.
11/18/2010   smd     Added support for offline search.
10/13/2010   rkc     Fixed check for aborted srch in srch dump callback.
07/02/2010   ljl     Mainlined FEATURE_HDRSRCH_1X_PILOT_SEARCH.
06/25/2010   smd     Supported hstr slam in slam cfg message.
06/21/2010   cnx     Fixed compiler warnings.
05/27/2010   vlc     Added initialization of common search parameters to 
                     hdrsrchdrv_init(). 
04/15/2010   smd     Added HSTR and MSTR in MSTR_SLAM message.
03/18/2010   ljl     Supported FEATURE_HDR_DIV_SRCH_WORKAROUND.
10/14/2009   kss     Removed command to FW to disable DRC (done by RTCMAC). 
10/02/2009   kss     Send message to disable DRC.
10/01/2009   ljl     Processed srch app cfg rsp msg.
09/28/2009   ljl     Saved the demod index before starting search.
09/28/2009   smd     Fixed KW errors.
09/14/2009   ljl     Supported srch MSGR interface.
07/06/2009   lyl     Fixed the compilation issue in RUMI build.
06/30/2009   lyl     Updated for generic responses process.
06/25/2009   wsh     Fixed MOB compile error on ARM compiler
05/18/2009   ljl     Updated for firmware MSGR interface.
03/06/2009   pmk     Corrected hdrsrchdrv_dump_cb
12/10/2008   smd     Replaced assert.h with amssassert.h
11/20/2008   kss     Removed/replaced intlocks.
08/29/2008   lyl     Supported primay chain ASET search.
08/08/2008   rkc     Sorted srch list by carrier before sending to DSP.
06/06/2008   rkc     Saved time to search for logging.
12/17/2007   jyw     Updated with revB MSTR macros.
11/20/2007   rmg     Removed compilation errors on 7800.
10/17/2007   ljl     Updated for Rev B.
08/19/2007   grl     Resolved Lint High warnings.
08/01/2007   grl     Fixed RCVT 2.2 compiler warnings.
07/20/2007   rkc     Use freq_offset when not FEATURE_HDRSRCH_1X_PILOT_SEARCH
06/18/2007   rkc     Added freq_offset input to hdrsrchdrv_send_srch_list()
04/10/2007   mt      Changes for 1x pilot search support.
03/09/2007   grl     Added search4 registration APIs for firmware downloading.
11/27/2006   jyw     Added hdrsrchdrv_get_mstr_x8.
03/23/2005   hrk     Replaced clk_busy_wait by hdrsrch_timed_wait_usecs
09/07/2004   sq      Call SRCH4 API to abort HDR searches
07/28/2004   sq      Moved MSTR update from SRCH4 into dump callback
06/01/2004   sq      Added APIs to enable/disable SRCH4
05/05/2004   aaj     Implemented DSP halt work-around to halt on known codes
11/18/2003   aaj     Cleaned up unused drivers code
10/16/2003   aaj     Code cleanup
10/06/2003   aaj     Mainlined T_MSM6500 and HDRSRCH_MULTIPATH_WIN_CENTER
09/10/2003   mpa     Converted F3 messages to MSG 2.0
09/04/2003   sq      Updated the number of pilot bursts for idle and traffic
                     states from 1 to 2. 
07/30/2003   sq      Update HDR searcher status after abort.
                     Enable DSP timer interrupt during initialization.
03/11/2003   ajn     Updated Copyright notice
11/14/2002   aaj     Clear DUMP signal when aborting searches with no dump
11/11/2002   ajn     Added missing header file.
11/05/2002   aaj     Do not enable AGC/DC loops before RF tune is complete
09/18/2002   aaj     Support for multipath window centering for HDR
09/17/2002   ajn     Collect random data from searcher peaks (eng & pos)
04/29/2002   aaj     Add wait in shutdown proc. Separate dump isr from drvinit
04/25/2002   aaj     Moved writing SCALE factor to hdrsrchdrv_startup_proc
04/24/2002   aaj     ASSERT on num tasks not being zero when starting srches
04/15/2002   aaj     Add ASSERTs to catch the case when numRes > 16
04/08/2002   aaj     Initialize srch div as per AT's diversity setting
04/02/2002   aaj     Estimate search duration to set lost dump timer
03/21/2002   aaj     Round mstrx2 to nearest pos when converting from x8->x2
02/21/2002   ajn     Added "dump partial results" flag to abort search
02/08/2002   sst     Added srch abort command
                     Clear srch abort signal on isr and programmed srch
09/05/2001   aaj     Antenna Diversity integrated in the idle state
08/02/2001   aaj     Handle MDSP Halts, Diversity Changes
06/04/2001   aaj     Read MSTR before compensating for peak positions
04/23/2001   aaj     Major changes to drivers to support TAZ3
03/08/2001   aaj     Added dump_cnt to hdrsrchdrv struct
03/02/2001   aaj     included customer.h file
02/28/2001   aaj     Added RX diversity control function
02/07/2001   aaj     Added rx_agc_db function
02/06/2001   aaj     Moved log functions to hdrsrchlog.c.Function name changes
12/18/2000   aaj     Changed HDR_SRCH_SELECT to normal search single antenna
12/12/2000   aaj     Added searcher log records support
10/10/2000   aaj     MSTR compensation added back to the code.
09/16/2000   aaj     We don't compensate for MSTR offset after reading the
                     searcher peak position anymore.Doesn't affect acq.
09/11/2000   aaj     Removed RX_AGC_MODE initialization
07/27/2000   ajn     Merged into HDR Search Task-Code baseline
06/27/2000   aaj     Compiled the code successfully
06/21/2000   aaj     Moved functions from set files to srchdrv
06/07/2000   aaj     Added more stuff
05/31/2000   aaj     Added TA searcher results support
05/25/2000   aaj     Created this file
==========================================================================*/



/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/
#include "hdr_variation.h"
#include "customer.h"

#include "hdrsrchdrv.h"

#include "hdrsrchrf.h"
#include "hdrsrchfing.h"
#include "hdrmdspmcr.h"
#include "hdrsrchtask.h"
#include "hdrsrchstate.h"
#include "hdrsrchi.h"
#include "hdrsrchmsg.h"
#include "msm.h"
#include "hdrsrchutil.h"
#include "hdrmdsp.h"
#include "hdrdebug.h"
#include "err.h"

#if defined(FEATURE_SEC)
#include "secutil.h"
#endif

#include "timetick.h"
#include "amssassert.h"

#include "srch_hw.h"
#include "srch4trans.h"
#include "srch4trans_t.h"

/*==========================================================================

                     DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, typesdefs,
and other items needed by this module.

==========================================================================*/

#define  HDRSRCH_DEMOD_CLOCK_KHZ        (40*1000)
  /* approximate hdr searcher hardware clock speed is used to estimate
     duration of searcher tasks */

#define  HDRSRCHDRV_DEFAULT_PRIORITY    SRCH4_PRIO_HDR_REACQ_PAGING
  /* Default priority setting for all HDR searches    */

#define  HDR_MAX_PILOT_PN_OFFSET        511  
  /* The valid PN range 0 to 511 */

#define  HDRSRCHDRV_MAX_SRCH_TIME       0xFF
  /* Value to log for max srch time in time_to_search (uint8) field */

hdrsrchdrv_struct_type  hdrsrchdrv;
/* hdrsrch driver parameters */


/* <EJECT> */
/*==========================================================================

                     Random Data Collection

==========================================================================*/


#if defined(FEATURE_SEC)

/* Declare temporary buffer for filling in random data */
#define SEC_RND_DEFN(n) \
  byte sec_rnd_buf[n], *sec_rnd_ptr = sec_rnd_buf;

/* Add byte of random data to buffer */
#define SEC_RND_DATA(x) \
  *sec_rnd_ptr++ = (x);

/* Submit buffer to security utility module */
#define SEC_RND_DONE( ) \
  secutil_fill_rnd_data( sec_rnd_buf, sec_rnd_ptr - sec_rnd_buf);

#else

/* No-ops */

#define SEC_RND_DEFN(n)
#define SEC_RND_DATA(x)
#define SEC_RND_DONE( )

#endif /* FEATURE_SEC && !FEATURE_SEC_CALLS_SEARCHER_RANDOM */

/* <EJECT> */
#if !( defined T_MSM8960 || defined FEATURE_HDR_DIME_MODEM )
/*=========================================================================

FUNCTION     : HDRSRCHDRV_SEND_SHUTDOWN_MSG

DESCRIPTION  : This function sends the shutdown message to disable RxAGC.

DEPENDENCIES : None

INPUT        : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchdrv_send_shutdown_msg( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchdrv.fw_msg.srch_shutdown_msg.antennaMode[0] = 0x0;

  hdrsrchmsg_send_msg( &hdrsrchdrv.fw_msg.srch_shutdown_msg,
                       sizeof( hdrfw_srch_shutdown_msg_t ) );
    /* Send the msg */
} /* hdrsrchdrv_send_shutdown_msg() */
#endif

/*=========================================================================

FUNCTION     : HDRSRCHDRV_SEND_HYPERSPACE_CFG_MSG

DESCRIPTION  : This function sends the message to config hyperspace flag.

DEPENDENCIES : None

INPUT        : hyperspace - Hyperspace flag

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchdrv_send_hyperspace_cfg_msg
( 
  uint16                     hyperspace
    /* hyper space flag */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchdrv.fw_msg.hyperspace_cfg_msg.hyperspaceConfig = hyperspace;
  hdrsrchmsg_send_msg( &hdrsrchdrv.fw_msg.hyperspace_cfg_msg,
                       sizeof( hdrfw_hyperspace_cfg_msg_t ) );
} /* hdrsrchdrv_send_hyperspace_cfg_msg() */


/*=========================================================================

FUNCTION     : HDRSRCHDRV_SET_DUMP_STATUS

DESCRIPTION  : This function sets the dump status.

DEPENDENCIES : None

INPUT        : TRUE: The dump is ready for process
               FALSE: The dump is not ready for process

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchdrv_set_dump_status
( 
  boolean                    status
    /* Whether the dump is ready */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* We reuse the flag defined in hdrsrchmsg. 
     Waiting means not ready and not waiting means ready.*/
  if ( status == TRUE )
  {
    hdrsrchmsg_set_rsp_waiting( HDRSRCH_SRCH_RSP, FALSE );
  }
  else
  {
    hdrsrchmsg_set_rsp_waiting( HDRSRCH_SRCH_RSP, TRUE );
  }
} /* hdrsrchdrv_set_dump_status() */


/*=========================================================================

FUNCTION     : HDRSRCHDRV_DUMP_IS_READY

DESCRIPTION  : This function returns the dump status.

DEPENDENCIES : None

INPUT        : None

RETURN VALUE : TRUE : The dump is ready
               FALSE: The dump is not ready

SIDE EFFECTS : None

=========================================================================*/

boolean hdrsrchdrv_dump_is_ready( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  return hdrsrchmsg_rsp_is_rcvd( HDRSRCH_SRCH_RSP );

} /* hdrsrchdrv_dump_is_ready() */


/*=========================================================================

FUNCTION     : HDRSRCHDRV_SAVE_TIME_TO_SEARCH

DESCRIPTION  : This function calculates the time to search in sclks from the
               start of a submitted search to the current time, normally a 
               successful search dump.  This time delta is saved for logging 
               purposes.  Since logging time_to_search is an unsigned 8-bit
               field, the delta is capped at 0xFF.

               Note: Do not need to worry about wrap around condition as long
                     as timetick_type is unsigned.

DEPENDENCIES : Assume hdrsrchdrv.time_to_search_start_sclk was saved when srch
               was started.  It will be cleared in this function after its use.

INPUTS       : None

RETURN VALUE : None

SIDE EFFECTS : hdrsrchdrv.time_to_search is updated

=========================================================================*/

void hdrsrchdrv_save_time_to_search( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchdrv.time_to_search_sclk =
    timetick_get() - hdrsrchdrv.time_to_search_start_sclk;
    /* Calculate time time to search.  Start time was stored earlier when srch
       was started.
       Note: Do not worry about wrap-around since timetick_type is unsigned. */

  if ( hdrsrchdrv.time_to_search_sclk > HDRSRCHDRV_MAX_SRCH_TIME )
  {
    hdrsrchdrv.time_to_search_sclk = HDRSRCHDRV_MAX_SRCH_TIME;
      /* Cap the maximum to the size of the logging time_to_search field's size */
  }

  hdrsrchdrv.time_to_search_start_sclk = 0;

} /* hdrsrchdrv_save_time_to_search */


/* EJECT */
/*=========================================================================

FUNCTION     : HDRSRCHDRV_COMPARE_SLIST_ENTRY

DESCRIPTION  : This function is called via qsort to sort an array of
               hdrsrchdrv_srch_list_struct_type using the carrier index field.

DEPENDENCIES : None

INPUT        : slist1 - 1st srch list entry to compare
               slist2 - 2nd srch list entry to compare

RETURN VALUE : -1 if 1st entry has a carrier index < 2nd entry's
                0 if 1st entry has a carrier index == 2nd entry's
                1 if 1st entry has a carrier index > 2nd entry's

SIDE EFFECTS : None

=========================================================================*/

int hdrsrchdrv_compare_slist_entry
(
  const void                                  *slist1,
    /* 1st srch list entry to compare */
  const void                                  *slist2
    /* 2nd srch list entry to compare */
)
{
  hdrsrchdrv_srch_list_struct_type*     srch_list1 =
    (hdrsrchdrv_srch_list_struct_type *)slist1;
    /* 1st srch list entry to compare */
  hdrsrchdrv_srch_list_struct_type*     srch_list2 =
    (hdrsrchdrv_srch_list_struct_type *)slist2;
    /* 2nd srch list entry to compare */
  int           ret_val = 0;
    /* -1, 0 or 1 return value */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( ( srch_list1 != NULL ) && ( srch_list2 != NULL ) )
  {
    if ( srch_list1->srch_task.carrier_idx == srch_list2->srch_task.carrier_idx )
 
    {
      ret_val = 0;
    }
    else if ( srch_list1->srch_task.carrier_idx < srch_list2->srch_task.carrier_idx)
    {
      ret_val = -1;
    }
    else if ( srch_list1->srch_task.carrier_idx > srch_list2->srch_task.carrier_idx)
    {
      ret_val = 1;
    }
  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, "Cannot compare NULL srch entries!" );
  }

  return ret_val;

} /* hdrsrchdrv_compare_slist_entry */


/* EJECT */
/*=========================================================================

FUNCTION     : HDRSRCHDRV_SORT_SLIST

DESCRIPTION  : This function perfroms a qsort to sort an array of
               hdrsrchdrv_srch_list_struct_type using the carrier index field.
               The number of carrier indexes encountered and the number of
               entries within each carrier index are also stored in the
               hdrsrchdrv data structure.

DEPENDENCIES : None

INPUT        : None

RETURN VALUE : -1 if 1st entry has a carrier index < 2nd entry's
                0 if 1st entry has a carrier index == 2nd entry's
                1 if 1st entry has a carrier index > 2nd entry's

SIDE EFFECTS : hdrsrchdrv.num_carriers and hdrsrchdrv.carrier_info are updated

=========================================================================*/

void hdrsrchdrv_sort_slist
(
  hdrsrchdrv_srch_list_struct_type*     srch_list,
    /* srch list to sort */
  uint8                                 srch_cnt
    /* number of searches */
)
{
  uint8                                 i;
    /* Loop variables */
  hdrsrchdrv_carr_idx_struct_type*      carrier_ptr = hdrsrchdrv.carrier_info;
    /* Pointer to carrier_info field in hdrsrchdrv data structure */
  hdrsrchdrv_srch_list_struct_type*     srch_list_ptr = srch_list;
    /* Pointer to srch_list.  Init to 1st entry in srch_list. */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  memset( hdrsrchdrv.carrier_info, 0, sizeof( hdrsrchdrv.carrier_info ) );
    /* Initialize carrier info. */

  /* Use qsort to sort the srch_list based.   Only sort
     if there is something to sort */
  if ( ( srch_list != NULL ) && ( srch_cnt != 0 ) )
  {
    if ( hdrsrch_state_id == HDRSRCH_CONNECTED_STATE )
    {
      /* Only need to sort if traffic state */
      qsort( srch_list,
             srch_cnt,
             sizeof( srch_list[0] ),
             hdrsrchdrv_compare_slist_entry );
    }
    else
    {
      /* Entire srch list must belong to single carrier.
         No need to sort.  Quickly fill out carrier info for validation
         and exit function. */
      carrier_ptr->carrier_idx              = srch_list_ptr->srch_task.carrier_idx;
      carrier_ptr->demod_idx                = srch_list_ptr->srch_task.carrier_idx;
      carrier_ptr->srch_task_cnt            = srch_cnt;
      hdrsrchdrv.num_carriers               = 1;
      return;
    }
  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR,
                  "Cannot sort NULL/empty srch list!" );
    hdrsrchdrv.num_carriers = 0;
      /* Number of carriers must be 0. */
    return;
  }

  /* Now that srch_list is sorted, determine how many carriers are in the
     srch_list and how many of each carrier index's srch entries.  This
     information will be useful in processing dumps for RevB multi-carriers. */
  carrier_ptr->carrier_idx              = srch_list_ptr->srch_task.carrier_idx;
  carrier_ptr->demod_idx                = srch_list_ptr->srch_task.carrier_idx;
  carrier_ptr->srch_task_cnt            = 1;
  hdrsrchdrv.num_carriers               = 1;

  srch_list_ptr++;
    /* Skip 1st entry in srch list since it has alrady been accounted for. */

  for ( i = 1; i < srch_cnt; i++ )
  {
    if ( ( srch_list_ptr != NULL ) &&
         ( hdrsrchdrv.num_carriers <= HDRSRCH_MAX_DEMOD_CARRIERS_CNT ) )
    {
      if ( srch_list_ptr->srch_task.carrier_idx !=
           carrier_ptr->carrier_idx )
      {
        /* Found a new carrier index. */
        carrier_ptr++;
        carrier_ptr->carrier_idx          = srch_list_ptr->srch_task.carrier_idx;
        carrier_ptr->demod_idx            = srch_list_ptr->srch_task.carrier_idx;
        carrier_ptr->srch_task_cnt        = 1;
        hdrsrchdrv.num_carriers++;

        if ( hdrsrchdrv.num_carriers > HDRSRCH_MAX_DEMOD_CARRIERS_CNT )
        {
          ERR_FATAL( "Too many carriers %d", hdrsrchdrv.num_carriers, 0, 0 );
        }

      }
      else
      {
        carrier_ptr->srch_task_cnt++;
          /* Entry has same carrier index as previous entry.
             Increment current idx's cnt. */
      }

      srch_list_ptr++;
      /* Move to next entry in srch list */
    }
  } /* for i loop */

} /* hdrsrchdrv_sort_slist */


/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHDRV_SEND_SRCH_LIST

DESCRIPTION  : This function accepts a list  of search parameters as an
               input and programs the searches to the DSP if search is
               not already pending with the DSP. It also accepts the SRAM
               capture control flag as a parameter which controls whether
               the sample ram will be refreshed before or after the searches
               are executed.

               This function copies the parameters to the DSP RAM and writes
               the search count register to start searching.

DEPENDENCIES : None

INPUTS       : srch_list[] => This list contains the search parameters
               cap_ctrl    => Sample Buffer capture control
               srch_cnt    => Number of searches in the list
               search_system => type of system to be searched, DO or 1x
               freq_offset => Frequency offset ( 4.6875 Hz/LSB )

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchdrv_send_srch_list
(
hdrsrchdrv_srch_list_struct_type *slist,
/* search list */

hdrsrchdrv_cap_ctrl_enum_type     cap_ctrl,
/* search capture control */

uint8                             srch_cnt,
/* number of searches */

hdrsrch_system_enum_type          search_system,
  /* type of system to be searched DO or 1x */

int16                             freq_offset,
/* freq offset ( 4.6875 Hz/LSB )   */

boolean                           atomic_search
  /* Perform atomic search or can reschedule? */
)
{
  uint8   s, i;
    /* Loop variables */

  srch4_srch_type                 srch_type;
    /* whether 1x pilots or DO pilots are searched */

  srch4_hdr_search_type           *srch_params_ptr;
    /* parameters to call start search function */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  ASSERT( srch_cnt <= HDR_MAX_SRCH_CNT );

  /* Searches are scheduled very frequently in all the HDR states. Before
     sending search, check if the DSP is alive by confirming that the halt
     code is zero  */

  if ( hdrsrchutil_check_mdsp_halt() )
  {
    hdrsrchutil_handle_mdsp_halt();
      /* handle the DSP halt emergency */

    return;
  }

  if ( hdrsrchdrv.active_srch != SRCH_STATUS_INACTIVE  )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, "Nested searches! S=%d", 
                    hdrsrchdrv.active_srch );
      /* Nested search ? Not acceptable.... fix it */

    return;
  }

  if ( search_system == HDRSRCH_SYSTYPE_1X ) 
  {
    srch_type = SRCH4_1X;
  }
  else
  {
    srch_type = SRCH4_HDR_SHOULDERS;
  }

  hdrsrchdrv_sort_slist( slist, srch_cnt );
    /* Sort srch list based on carrier index.  For RevB, this will make life
       easier for srch dump processing. */

  /* Initialize srch results to valid since not all states validate srch
   results, e.g., reacq state */
  for ( i = 0; i < HDRSRCH_MAX_DEMOD_CARRIERS_CNT; i++ )
  {
    hdrsrchdrv.carrier_info[i].srch_valid = TRUE;
      /* Assume srch results are valid. */
  }

  for ( s = 0; s < srch_cnt; s++ )
  {
    (void) srch4_hdr_add_task( &( ( slist + s )->srch_task ), srch_type );
  }/* for */

  /* Start a timer to catch any occurance of a search request to the DSP
     which does not result in search dump within a reasonable amount of
     time. Additional margin is given for software delays */

  HDRSRCH_SIG_CLR( HDRSRCH_LOSTDUMP_SIG );
    /* Ensure the signal is not currently active */

  hdrsrch_add_to_wait_mask( HDRSRCH_LOSTDUMP_SIG );
    /* Listen for this unfortunate possibility */

  hdrsrchdrv.list_cnt = srch_cnt;
    /* Remember number of searches */

  hdrsrchdrv.active_srch = SRCH_STATUS_ACTIVE;
    /* Set the search flag to non-TA search value */

  hdrsrchdrv.time_to_search_start_sclk = timetick_get();
    /* Save start of search time in sclks */

  hdrsrchdrv.system = search_system;
    /* Record the type of the system we are searching */
  
  srch_params_ptr = &hdrsrchdrv.srch_params;
  srch_params_ptr->freq_offset = freq_offset;                /* Freq offset */
  srch_params_ptr->dump_cb_func = hdrsrchdrv_dump_cb;        /* dump call back */
  srch_params_ptr->lost_cb_func = hdrsrchdrv_lostdump_cb;    /* lost dump cb */
  srch_params_ptr->cb_data = NULL;                           /* data ptr for ret */
  srch_params_ptr->srch_type = srch_type;                    /* 1x or DO pilot*/
  srch_params_ptr->offline_search = FALSE;                   /* indicate offline search */
#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW                           
  srch_params_ptr->atomic_search = atomic_search;            /* Perform atomic search or not */
#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */                           

  HDR_MSG_SRCH_2( MSG_LEGACY_LOW,
                  "IRAT MEAS: rtc cnt 0x%x srch mode %d",
                  srch_params_ptr->capturedRTCCnt, srch_params_ptr->mode);
  (void) srch4_hdr_start_srch( srch_params_ptr );

}/* hdrsrchdrv_send_srch_slist() */


/*===========================================================================

FUNCTION        HDRSRCHDRV_UPDATE_MSTR_X8

DESCRIPTION     This function updates the MSTR variable maintained by the
                searcher driver.

DEPENDENCIES    none

RETURN VALUE    none

SIDE EFFECTS    none

===========================================================================*/

void hdrsrchdrv_update_mstr_x8()
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( hdrsrchdrv.mstr_valid )
  {
#ifndef FEATURE_HDR_REVB
    if ( HDRMSTR_GET_REF_PN() <= HDR_MAX_PILOT_PN_OFFSET )
#else
    if ( hdrsrchutil_get_ref_sector_ptr() != NULL )
#endif /* FEATURE_HDR_REVB */
    {
      /* Update the MSTR if tracking sector is valid */

      hdrsrchdrv.mstr_x8 = HDRMSTR_GET_MSTR_X8();
        /* Read the MSTR_RTC_OFFSET */
    }
  }
  else
  {
    hdrsrchdrv.mstr_x8 = 0;
  }

}/* hdrsrchdrv_update_mstr_x8() */


/*===========================================================================

FUNCTION        HDRSRCHDRV_GET_MSTR_X8

DESCRIPTION     The function returns the MSTR in the unit of cx8. The MSTR is
                read from the mdsp if reference pilot tracking is still valid.
                otherwise, the function returns the recent good MSTR.

DEPENDENCIES    none

RETURN VALUE    The MSTR in the unit of cx8.

SIDE EFFECTS    none

===========================================================================*/

uint32 hdrsrchdrv_get_mstr_x8( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef FEATURE_HDR_REVB
  if ( HDRMSTR_GET_REF_PN() <= HDR_MAX_PILOT_PN_OFFSET )
#else
  if ( hdrsrchutil_get_ref_sector_ptr() != NULL )
#endif /* FEATURE_HDR_REVB */
  {
    /* MSTR tracking sector is valid */

    return HDRMSTR_GET_MSTR_X8();
      /* Return the current MSTR */
  }
  else
  {
    /* MSTR finger is not tracking on a valid sector 
     * return the recent good MSTR */

    return hdrsrchdrv.mstr_x8;
      /* Return the recent good MSTR */
  }
}/* hdrsrchdrv_get_mstr_x8() */


/*===========================================================================

FUNCTION        HDRSRCHDRV_MSM16CPY

DESCRIPTION     This routine copies 16 bit data between the ARM memory and the
                DDSP memory. It accepts source and destination pointers of
                16 bit word type and the number of 16 bit words to be copied.
                This routine may be moved to another file once the files for
                common services are defined.

                INPUTS:
                       src  : uint16 pointer
                       dest : uint16  pointer
                       len  : number of uint16 types to be copied.

                OUTPUT:
                        None

DEPENDENCIES    none

RETURN VALUE    none

SIDE EFFECTS    none

LIMITATIONS     It can copy at the most 32768 words.
===========================================================================*/

void hdrsrchdrv_mem16cpy
(
uint16 *src,
uint16 *dest,
uint16 len
)
{
  uint16 cnt;  /* temporary variable */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  for ( cnt = 0; cnt < len; cnt++ )
  {
    *dest++ = *src++;
    /* copy the source word onto the destination */

  }/* for cnt */

}/* hdrsrchdrv_mem16cpy() */

/* <EJECT> */
/*===========================================================================

FUNCTION     : HDRSRCHDRV_GET_RX_AGC_DB

DESCRIPTION  : This function returns the RX AGC estimate in dB units

DEPENDENCIES : None

PARAMETERS   : None

RETURN VALUE : None

SIDE EFFECTS : None

===========================================================================*/
int16 hdrsrchdrv_get_rx_agc_db
(
  uint8     antenna
    /* antenna number */
)
{
  int16 rx_agc;
  int16 rx_power;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( antenna == 0 )
  {
    rx_agc = (int16) HDRAGC_GET_RX0_AGC();
      /* read Antenna 0 AGC loop accumulator */
  }
  else
  {
    rx_agc = (int16) HDRAGC_GET_RX1_AGC();
      /* read Antenna 0 AGC loop accumulator */
  }

  rx_power = -64 + (int16) ( ((int32) (3 * rx_agc) ) >> 11 );

  return rx_power;


}/* hdrsrchutil_get_rx_agc_db */

/* <EJECT> */

/*=========================================================================

FUNCTION     : HDRSRCHDRV_DIVERSITY_INIT

DESCRIPTION  : This function initializes the drivers depending on the RX
               diversity

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchdrv_diversity_init
(
  boolean div_flag               /* diversity control flag */
)
{
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /*-------------------------------------*
   * initialize common search parameters *
   *-------------------------------------*/
  if ( div_flag )
  {
      hdrsrchdrv.params.coh_int    = HDR_SRCH_COH_ACCUM;
      hdrsrchdrv.params.noncoh_int = HDR_SRCH_NONCOH_ACCUM_DIV;
      hdrsrchdrv.params.burst_len  = HDR_SRCH_BURST_LENGTH_DIV;
  }
  else
  {
      hdrsrchdrv.params.coh_int    = HDR_SRCH_COH_ACCUM;
      hdrsrchdrv.params.noncoh_int = HDR_SRCH_NONCOH_ACCUM_NON_DIV;
      hdrsrchdrv.params.burst_len  = HDR_SRCH_BURST_LENGTH_NON_DIV;
  }

  hdrsrchdrv.params.split_pilot = 0;
    /* don't split pilot for now */

  hdrsrchdrv.params.default_ant = 0;
    /* select ant0 if div is not on */

} /* hdrsrchdrv_diversity_init() */

/* <EJECT> */

/*=========================================================================

FUNCTION     : HDRSRCHDRV_STARTUP_PROC

DESCRIPTION  : This function does the startup processing

DEPENDENCIES : None

INPUT        : None

RETURN VALUE : None

SIDE EFFECTS : Changes firmware/hardware settings

=========================================================================*/

void hdrsrchdrv_startup_proc( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchdrv_send_hyperspace_cfg_msg( HDRSRCH_RESET_HYPERSPACE_MODE );
    /* Reset forces DSP to look for preamble on half slot boundaries and
       also disables transmission */

}/* hdrsrchdrv_startup_proc */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHDRV_SHUTDOWN_PROC

DESCRIPTION  : This function does the HDR shutdown processing

DEPENDENCIES : None

INPUT        : None

RETURN VALUE : None

SIDE EFFECTS : Changes firmware/hardware settings

=========================================================================*/

void hdrsrchdrv_shutdown_proc( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchfing_deassign_all_fings( );
    /* No fingers active, please. */

  (void) hdrsrch_wait_for_high_pri_rsp( HDRSRCHFING_ASSIGNMENT_DONE_WAIT_US, 
                                        HDRSRCH_FING_ASSIGN_RSP );
    /* wait to make sure that assignment is complete */

#if !( defined T_MSM8960 || defined FEATURE_HDR_DIME_MODEM )
  hdrsrchdrv_send_shutdown_msg();
    /* Disable Rx AGC so as to enter other mode of operation */
#endif

}/* hdrsrchdrv_shutdown_proc */


/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHDRV_ABORT_CB

DESCRIPTION  : This is the callback function passed to SRCH4. It is called
               after HDR searches are aborted.

DEPENDENCIES : None

INPUT        : Dummy parameters.

RETURN VALUE : None

SIDE EFFECTS

=========================================================================*/


void hdrsrchdrv_abort_cb (uint32 x, void* null_ptr )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchdrv_dump_cb( NULL );

} /* hdrsrchdrv_abort_cb             */


/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHDRV_ABORT_SEARCH

DESCRIPTION  : This function aborts the current search list

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : Changes firmware/hardware settings
               srchAbort is set
               Current search of list will finish,
                 then no more searches will be programmed.

=========================================================================*/

void hdrsrchdrv_abort_search
(
  boolean                         dump_partial_results
    /* Flag to generate DUMP signal for aborted search results */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  REX_DISABLE_PREMPTION();

  if (hdrsrchdrv.active_srch != SRCH_STATUS_INACTIVE)
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "Abort Search sent: %d searches programmed",
                    hdrsrchdrv.list_cnt );


    hdrsrchdrv.active_srch = dump_partial_results ? SRCH_STATUS_ABORT_WITH_DUMP
                                                  : SRCH_STATUS_ABORT_NO_DUMP;

    HDR_MSG_SRCH( MSG_LEGACY_HIGH,  "Aborting HDR Searches! " );   

    (void) srch4_hdr_abort_search( hdrsrchdrv_abort_cb, 
                                   NULL, 
                                   FALSE, 
                                   dump_partial_results );

      /* Command SRCH4 driver to abort HDR searches.
       * The number of searches completed (including the one in
       * process) will be placed into srchNumResults. A search
       * done interrupt will be sent to the ARM.
       */

      /* srchAbort will be cleared after the dump is received in
       * hdrsrchdrv_srch_dump_isr()
       */
  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "Abort Search sent: Search is inactive" );
  }

  /* TBD Need to check why need this. */
#if 0
  if ( !dump_partial_results )
  {
    /*HDRSRCH_SIG_CLR( HDRSRCH_DUMP_SIG );*/
    hdrsrchdrv_set_dump_status( FALSE );
      /* We do not need results so clear the dump signal */

    hdrsrch_discard_srch_dump();
      /* Discard the srch dump */
  }
#endif /* if 0 */

  REX_ENABLE_PREMPTION();

} /* hdrsrchdrv_abort_search */


/* <EJECT> */

/*=========================================================================

FUNCTION     : HDRSRCHDRV_INIT

DESCRIPTION  : This function initializes the HDR Search Driver routines

DEPENDENCIES : None

INPUT        : None

RETURN VALUE : None

SIDE EFFECTS
  Installs the DUMP handler ISR

=========================================================================*/

void hdrsrchdrv_init( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* initialize the searcher driver variables to defaults */
  hdrsrchdrv.list_cnt        = 0;
  hdrsrchdrv.active_srch     = SRCH_STATUS_INACTIVE;
  hdrsrchdrv.dump_cnt        = 0;

  hdrsrchdrv.params.coh_int    = HDR_SRCH_COH_ACCUM;
  hdrsrchdrv.params.noncoh_int = HDR_SRCH_NONCOH_ACCUM_NON_DIV;
  hdrsrchdrv.params.burst_len  = HDR_SRCH_BURST_LENGTH_NON_DIV;

  /* Clear time_to_search logging data */
  hdrsrchdrv.time_to_search_start_sclk  = 0;
  hdrsrchdrv.time_to_search_sclk        = 0;

  hdrsrchdrv.params.split_pilot = 0;
  /* don't split pilot for now */

  hdrsrchdrv.params.default_ant = 0;
  /* select ant0 if div is not on */

  msgr_init_hdr( &hdrsrchdrv.fw_msg.mstr_slam_msg.hdr, 
                 MSGR_HDR_SRCH, 
                 HDR_FW_MSTR_SLAM_CMD );
    /* Init message header for HDR_FW_HSTR_OFFSET_CFG_CMD */

#if !(defined T_MSM8960 || defined FEATURE_HDR_DIME_MODEM )
  msgr_init_hdr( &hdrsrchdrv.fw_msg.srch_shutdown_msg.hdr, 
                 MSGR_HDR_SRCH, 
                 HDR_FW_SRCH_SHUTDOWN_CMD );
    /* Init message header for HDR_FW_HSTR_OFFSET_CFG_CMD */
#endif

  msgr_init_hdr( &hdrsrchdrv.fw_msg.hyperspace_cfg_msg.hdr, 
                 MSGR_HDR_SRCH, 
                 HDR_FW_HYPERSPACE_CFG_CMD );
    /* Init message header for HDR_FW_HSTR_OFFSET_CFG_CMD */

  hdrsrchdrv_set_srch4_param( SRCH4_MODE_DEFAULT, 0 );
  /* Initialize srch4 parameters */
} /* hdrsrchdrv_init */

/*=========================================================================

FUNCTION     : HDRSRCHDRV_SET_BURST_LEN

DESCRIPTION  : This function initializes the burst length for the
next search operation

DEPENDENCIES : None

INPUT        : Burst length to use next

RETURN VALUE : None

SIDE EFFECTS
  Retains the value till it is set again

=========================================================================*/

void hdrsrchdrv_set_burst_len( hdrsrchdrv_burst_len_type burst_len )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchdrv.params.burst_len  = burst_len;
}

/*=========================================================================

FUNCTION     : HDRSRCHDRV_LOSTDUMP_CB

DESCRIPTION  : This function will be called if there is a lost dump.

DEPENDENCIES : None

INPUT        : None

RETURN VALUE : None

SIDE EFFECTS

=========================================================================*/

void hdrsrchdrv_lostdump_cb
(
  void* data_ptr 
    /* data pointer if lostdump wants to return data to the client */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Clear time_to_search logging data */
  hdrsrchdrv.time_to_search_start_sclk  = 0;
  hdrsrchdrv.time_to_search_sclk        = 0;

  hdrsrchdrv.active_srch = SRCH_STATUS_INACTIVE;

  HDRSRCH_SIG_SET(HDRSRCH_LOSTDUMP_SIG);

} /* hdrsrchdrv_lostdump_cb             */

/*=========================================================================

FUNCTION     : HDRSRCHDRV_DUMP_CB

DESCRIPTION  : This function will be called after a status dump.

DEPENDENCIES : None

INPUT        : None

RETURN VALUE : None

SIDE EFFECTS

=========================================================================*/

void hdrsrchdrv_dump_cb(void* null_ptr )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchdrv_save_time_to_search();
    /* Save time to search for logging */

  hdrsrchdrv_update_mstr_x8( );
    /* Reads MSTR value from DSP    */


  /* Unless the search list was aborted with a request for no dump... */
  if ( hdrsrchdrv.active_srch != SRCH_STATUS_ABORT_NO_DUMP )
  {
    hdrsrchdrv.active_srch = SRCH_STATUS_INACTIVE;
      /* reset the search active flag to indicate that there are no
         searches active 

         The status has to be set to inactive before setting the DUMP_SIG.

         If the status is set to inactive after setting the SIG, hdrsrch task 
         might get scheduled and process the signal. As part of the signal
         processing eventually hdrsrchdrv_send_srch_list gets called which
         returns as the status is still active

         Note: This is only needed here for off target builds since on-target
               HDR srch task is single threaded and would not be swapped out.
       */

    hdrsrchdrv_set_dump_status( TRUE );
      /* A dump has occurred */
  }
  else
  {
    hdrsrchdrv.active_srch = SRCH_STATUS_INACTIVE;
      /* Set to inactive status */

    hdrsrchdrv_set_dump_status( FALSE );
      /* Ignore the dump */

    HDRSRCH_SIG_SET( HDRSRCH_TICKLE_SIG );
      /* Just get the main task loop to run so srch on idle can be processed. */
  }

  /* If it is for ASET primary chain search, don't increment dump count */
  if ( hdrsrchdrv.srch_task_type != HDRSRCH_PRI_CHAIN_ASET_SRCH )
  {
    hdrsrchdrv.dump_cnt++;
      /* count number of search dumps */
  }


} /* hdrsrchdrv_dump_cb             */


/*=========================================================================

FUNCTION     : HDRSRCHDRV_REQUEST_SRCH4_MDSP_APP

DESCRIPTION  : This function requests the SRCH4 application be loaded
               for HDR. It will download the firmware srch4 application
               to the MDSP if it isn't already downloaded.

DEPENDENCIES : It needs to be called when HDR gets the RF lock.

INPUT        : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchdrv_request_srch4_mdsp_app( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  (void) rex_clr_sigs( HDRSRCH_TCB, HDRSRCH_ADHOC_GP_SIG2 );
    /* Make sure this signal is unset. */

  srch4_request_mdsp_app( SRCH4_HDR_MASK, HDRSRCH_TCB, HDRSRCH_ADHOC_GP_SIG2 );
    /* Request the srch4 application and download it if necessary. */

} /* hdrsrchdrv_request_srch4_mdsp_app */

/*=========================================================================

FUNCTION     : HDRSRCHDRV_RELEASE_SRCH4_MDSP_APP

DESCRIPTION  : This function releases the SRCH4 operation for HDR
               allowing the application to be removed from MDSP memory.

DEPENDENCIES : None

INPUT        : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchdrv_release_srch4_mdsp_app( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  srch4_release_mdsp_app( SRCH4_HDR_MASK );
    /* Release the srch4 MDSP application allowing MDSP services to swap
       it out of MDSP memory. */

} /* hdrsrchdrv_release_srch4_mdsp_app */


/* <EJECT> */

/*=========================================================================

FUNCTION     : HDRSRCHDRV_ENABLE_SRCH4

DESCRIPTION  : This function enables SRCH4 operation for HDR. It calls 
               SRCH4 APIs to enable MDSP clock, request a HPQ for use by
               HDRSRCH and set the HDR search priority.

DEPENDENCIES : It needs to be called when HDR gets the RF lock.
               hdrsrchdrv_request_srch4_mdsp_app must have been called.

INPUT        : search priority

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchdrv_enable_srch4
(
  srch4_priority_type priority
    /* Search priority */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  (void) srch4_hdr_register( priority );
    /* Request HPQ for HDR to use and set default HDR search priority
       This function also enables the search4 app and clocks.   */

} /* hdrsrchdrv_enable_srch4 */

/*=========================================================================

FUNCTION     : HDRSRCHDRV_DISABLE_SRCH4

DESCRIPTION  : This function disables SRCH4 operation for HDR and may
               turn off clocks to the MDSP and the srch4 hardware. Note
               this will not remove the srch4 from the MDSP memory.
               hdrsrchdrv_release_srch4_mdsp_app must be called to 
               permit the application's removal.

DEPENDENCIES : None

INPUT        : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchdrv_disable_srch4( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  srch4_hdr_deregister( );
    /* De-register HPQ for HDRSRCH. This function also disables the search4 app
       and clocks.  */

} /* hdrsrchdrv_disable_srch4 */

/*=========================================================================

FUNCTION     : HDRSRCHDRV_START_SLAM

DESCRIPTION  : This function sends the msg to start slam.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchdrv_start_slam
( 
  boolean   set_timing,
    /* Whether software gives firmware timing */

  uint16    hstr,
    /* hstr number */

  uint32    mstr_x8,
    /* mstr in chipx8 */

  uint64    cdma_sys_time,
    /* cdms system time */

  uint32    hdr_rtc
    /* hdr rtc  number */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchdrv.fw_msg.mstr_slam_msg.setMstr = set_timing ;
  hdrsrchdrv.fw_msg.mstr_slam_msg.hstr = hstr;
  hdrsrchdrv.fw_msg.mstr_slam_msg.mstrCx8 = mstr_x8;
  hdrsrchdrv.fw_msg.mstr_slam_msg.capturedCdmaSystemTime = cdma_sys_time;
  hdrsrchdrv.fw_msg.mstr_slam_msg.capturedRtcCx8 = hdr_rtc;
  hdrsrchmsg_send_msg( &hdrsrchdrv.fw_msg.mstr_slam_msg,
                       sizeof( hdrfw_mstr_slam_msg_t ) );
    /* Start MSTR slam operation */

  hdrsrchmsg_set_rsp_waiting( HDRSRCH_MSTR_SLAM_RSP, TRUE );
    /* Waiting for resposne */

} /* hdrsrchdrv_start_slam */

/*=========================================================================

FUNCTION     : HDRSRCHDRV_IS_SLAM_PENDING

DESCRIPTION  : This function checks whether it is waiting for slam response.

DEPENDENCIES : None

INPUT        : None

RETURN VALUE : TRUE  - The slam response is received.
               FALSE - The slam response is not received.

SIDE EFFECTS : None

=========================================================================*/

boolean hdrsrchdrv_is_slam_pending( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  return ( !hdrsrchmsg_rsp_is_rcvd( HDRSRCH_MSTR_SLAM_RSP ) );

} /* hdrsrchdrv_is_slam_pending() */

/*=========================================================================

FUNCTION     : HDRSRCHDRV_SET_SRCH4_PARAM

DESCRIPTION  : This function sets srch4 params

DEPENDENCIES : None

INPUT        : srch4 parameters

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchdrv_set_srch4_param
(
  srch4_mode_type mode, // srch4 mode
  uint16          rtc_cnt_cx2
  /* RTC position in cx2 when  streaming is stopped for connected mode
     and idle offline mode, to be passed on to SRCH FW for offline
     search  */
)
{
  hdrsrchdrv.srch_params.mode = mode;
  hdrsrchdrv.srch_params.capturedRTCCnt = rtc_cnt_cx2;
  HDR_MSG_SRCH_2( MSG_LEGACY_LOW,
        "IRAT MEAS: rtc cnt 0x%x srch mode %d",
         hdrsrchdrv.srch_params.capturedRTCCnt, hdrsrchdrv.srch_params.mode);
}

/*=========================================================================

FUNCTION     : HDRSRCHDRV_IS_SEARCH_ACTIVE

DESCRIPTION  : This function returns the current status of search

DEPENDENCIES : None

INPUT        : none

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

boolean hdrsrchdrv_is_search_active(void)
{
  return ! ( ( hdrsrchdrv.active_srch == SRCH_STATUS_INACTIVE ) || ( hdrsrchdrv.active_srch == SRCH_STATUS_ABORT_NO_DUMP ) );
}

/*=========================================================================

FUNCTION     : HDRSRCHDRV_IS_DIV_SEARCH_ACTIVE

DESCRIPTION  : This function returns whether Div search is active

DEPENDENCIES : None

INPUT        : none

RETURN VALUE : TRUE/FALSE

SIDE EFFECTS : None

=========================================================================*/

boolean hdrsrchdrv_is_div_search_active(void)
{
  return ( ( hdrsrchdrv.active_srch != SRCH_STATUS_INACTIVE ) &&
           ( hdrsrchdrv.active_srch != SRCH_STATUS_ABORT_NO_DUMP ) &&
           ( hdrsrchdrv.srch_task_type == HDRSRCH_DIVERSITY_SRCH ) );
}
