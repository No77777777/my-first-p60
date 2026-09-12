/*===*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                  H D R    F I N G E R    D R I V E R S

                           Finger Drivers Module

GENERAL DESCRIPTION

   This file contains drivers for the HDR rake receiver operation.

EXTERNALIZED FUNCTIONS

    hdrsrchfing_read_fing_status :
       Updates the hdrsrchfing.fings[] finger database with latest finger status

    hdrsrchfing_program_fings  :
       Writes the modified finger parameters to the DSP.

    hdrsrchfing_diversity_init :
       This function enables or disables the antenna diversity. It also
       sets the total finger count and finger count per pilot as per div

REGIONAL FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 2000 - 2018 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

============================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/hdr.mpss/6.0/srch/src/hdrsrchfing.c#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when         who     what, where, why
----------   ---     ------------------------------------------------------- 
09/11/2018   vke     Reset all fingers to clear stray fingers if any
06/26/2018   vke     Changes to process slam response status from fw
08/29/2017   vko     Add support for 1xSRLTE+W
12/16/2016   svu     Fixed Klocwork issues
06/18/2015   tnp     Fix compiler warnings
10/07/2014   rmv     For Bringup of JO hw at reduced clks, limit the max fingers 
05/12/2014   vke     Klockworks fix
10/17/2011   rmg     Removed inclusion of clk.h.
12/14/2010   ljl     Limited the max fingers for revB on 9k.
11/04/2010   lyl     Removed unecessary finger update with diversity switch.
06/21/2010   cnx     Fixed compiler warnings.
10/13/2009   ljl     Fixed the condition to send finger assignment message.
10/09/2009   ljl     Sent finger assignment message if finger mask is non-zero.
09/30/2009   smd     Fixed KW errors.
07/17/2009   ljl     Set Rake CI Caps for QAM16 and QAM64.
06/30/2009   lyl     Updated for generic responses process.
06/25/2009   wsh     Fixed MOB compile error on ARM compiler
06/19/2009   grl     Switched to the hdrsrchrfdiv API.
05/18/2009   ljl     Updated for firmware MSGR interface.
05/07/2009   etv     Removed obsolete field usage from finger assign message.
04/25/2014   ljl     Supported finger assignment and slam message.
03/06/2009   pmk     Added support for MSGR
02/04/2009   lyl     Fixed the lint errors.
01/19/2009   smd     Fixed an assigment bug in are_fings_locked().
12/23/2008   lyl     Fixed the wrong value of the finger positions in F3.
12/12/2008   smd     Added check finger lock status for RevB.
11/20/2008   kss     Removed/replaced intlocks.
11/20/2008   rkc     Fixed lint errors.
10/07/2008   lyl     Fixed lint errors.
07/17/2008   smd     Fixed klocwork errors.
04/24/1008   ljl     Updated finger diversity status.
04/09/2008   ljl     Removed HDRSRCHFING_DEMOD_CARRIER_ENABLED().
01/30/2008   jyw     Deassigned fingers if the demod is dropped.
01/10/2008   ljl     Updated PN offset in ASP structure.
01/03/2008   ljl     Deassigned fingers per carrier.
12/26/2007   jyw     Relocated the UT feature into hdrsrchutil.c.
12/17/2007   jyw     Updated with revB MSTR macros.
10/17/2007   ljl     Added Rev B support.
08/19/2007   grl     Resolved Lint High warnings.
05/30/2007   ljl     Added hdrsrchfing_are_fings_locked().
03/13/2007   jyw     Fixed finger jumps at state changes.
03/08/2007   grl     Fixed RCVT compiler warning by removing local var.
02/02/2007   ljl     Added FEATURE_HDR_MPS_MIN_PREAMBLE_THRESH.
12/11/2006   ljl     Added hdrsrchfing_are_fings_assigned().
05/04/2006   sq      Check assignment status when changing diversity setting
05/01/2006   sq      Added API hdrsrchfing_check_fing_assign_status
01/23/2005   jyw     Expanded the number of fing programming for the div mode.
11/20/2005   grl     Set initial Ior/Nt finger value during div transitions.
11/14/2005   grl     Initialized finger filtered sinr value.
06/23/2005   sq      Changed finger preamble detection threshold for MPS
05/31/2005   grl     Removed extra include of hdrsrchrf.
03/23/2005   hrk     Replaced clk_busy_wait by hdrsrch_timed_wait_usecs
11/29/2004   grl     Added connected state stat logging support.
08/30/2004   aaj     Changed finger assignment wait to 900ms from 1700ms
05/12/2004   sq      Mainlined FEATURE_HDR_TC_DIVERSITY_ONLY
10/06/2003   aaj     Mainlined T_MSM6500 and HDRSRCH_MULTIPATH_WIN_CENTER
10/02/2003   sq      Limit diversity finger assignement for each pilot.
09/10/2003   mpa     Converted F3 messages to use MSG2.0
07/20/2003   sq      Support for T_MSM6500
06/10/2003   sq      Changed finger lock thresholds for MPS testing
03/11/2003   ajn     Updated Copyright notice
02/26/2003   aaj     Changes to mps threshold logic
02/26/2003   sq      Removed compilation warnings
                     Removed unnecessary MPS message
02/19/2003   sq      Restore finger thresholds according to MPS flag
02/11/2003   sq      Deassign the upper fingers when switching diversity
01/30/2003   sq      Moved MPS related #defines from hdrsrchfing.h
                     Added support for turning on diversity only in traffic
01/21/2003   sq      Added support for MPS testing
11/11/2002   ajn     Added missing header file.
09/18/2002   aaj     Support for multipath window centering for HDR
09/17/2002   ajn     Collect random data from finger RSSI
09/09/2002   aaj     Occasionally wait for 1 slot for past fing assignment
08/07/2002   aaj     Get absolute values in check stray finger function
06/19/2002   ajn     Corrected comparison between f1 and f1 to btw f1 and f2
06/06/2002   aaj     Corrected a comment about finger lock age. 32hrs->32mins
                     Maximum wait time for jump2hyperspace 16slots->4slots
04/29/2002   aaj     Removed program fings from fing_init. Do not override mask
04/10/2002   aaj     Override past finger assignment if there is a clash
04/08/2002   aaj     Initialize finger div as per AT's diversity setting
04/02/2002   aaj     Declare system loss if slam takes too long
02/18/2002   aaj     Initialize antenna selection param for all fingers
01/28/2002   aaj     Fixed fing_mask to be 16 bit var to support 12 fingers
12/05/2001   aaj     Added code to check stray finger (>320 chips away)
11/01/2001   aaj     Added delay before j2h to avoid DSP crash. More comments
09/05/2001   aaj     Antenna Diversity integrated in the idle state
09/05/2001   aaj     Write diversity flag to the DSP
08/27/2001   aaj     Removed clk_busy_wait from pgm_fings. Wait in j2hyper
08/02/2001   aaj     Antenna Diversity Changes for fingers
07/17/2001   aaj     Fixed a couple of TODO items
06/27/2001   aaj     Added code to check stray finger (>480 chipx8 away)
04/23/2001   aaj     Major changes to drivers to support TAZ3
03/08/2001   aaj     Added INTLOCK around finger status read function
03/08/2001   ajn     Added disable all fingers function
03/02/2001   aaj     included customer.h file
02/06/2001   aaj     Moved log functions to hdrsrchlog.c.Function name changes
11/29/2000   aaj     Added log support for finger log record
11/13/2000   ajn     Sector structure reorg
10/23/2000   ajn     Added VCS Header line
10/19/2000   ajn     Added finger/sector update function.
10/09/2000   ajn/aaj Added finger/sector deassign functions
09/25/2000   aaj     Added finger unlock counter update function
09/16/2000   aaj     changed finger posx8 to pure MSTR_RTC_OFFSETx8
09/15/2000   aaj     Added finger driver init routine
06/27/2000   aaj     Compiled the code successfully
06/21/2000   aaj     Incorporated code review comments
06/10/2000   aaj     Created this file
==========================================================================*/



/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/

#include "hdr_variation.h"
#include "comdef.h"

#include "customer.h"

#include "hdrsrchfing.h"
#include "hdrsrchrf.h"
#include "hdrsrchrfdiv.h"
#include "hdrsrchutil.h"
#include "hdrsrchdrv.h"
#include "hdrmdspmcr.h"
#include "hdrsrchdef.h"
#include "hdrsrchset.h"
#include "hdrsrchlog.h"

#include "hdrmdsp.h"

#include "hdrsrchmsg.h"

#include "hdrdebug.h"
#include "err.h"

#if defined(FEATURE_SEC)
#include "secutil.h"
#endif

/*==========================================================================

                     DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, typesdefs,
and other items needed by this module.

==========================================================================*/

#define HDRSRCHFING_DEFAULT_INIT_SINR         0
  /* The default initial finger SINR (Ior/Nt) value. */

#define HDRSRCH_FING_DEFAULT_LOCK_LO         16
#define HDRSRCH_FING_DEFAULT_LOCK_HI         32
  /* Default finger thresholds(Out-of-lock/In-lock) used by firmware    */


#define MPS_PS2ENG_TABLE_SIZE                28
#define MPS_PS2ENG_TABLE_MIN                 24 /* -0.5 dB units */

  /* MPS specific table has been implemented to get accurate dB conversion
     for MPS testing. The range of this table is:
     From: MPS_PS2ENG_TABLE_MIN
     To:   MPS_PS2ENG_TABLE_MIN  + MPS_PS2ENG_TABLE_SIZE                 */

uint16  mps_ps2eng_table[ MPS_PS2ENG_TABLE_SIZE ] =
{
  32,        // -12.0 dB
  29,        // -12.5 dB
  26,        // -13.0 dB
  23,        // -13.5 dB
  20,        // -14.0 dB
  18,        // -14.5 dB
  16,        // -15.0 dB
  14,        // -15.5 dB
  13,        // -16.0 dB
  11,        // -16.5 dB
  10,        // -17.0 dB
  9,        // -17.5 dB
  8,        // -18.0 dB
  7,        // -18.5 dB
  7,        // -19.0 dB
  6,        // -19.5 dB
  5,        // -20.0 dB
  5,        // -20.5 dB
  4,        // -21.0 dB
  4,        // -21.5 dB
  3,        // -22.0 dB
  3,        // -22.5 dB
  3,        // -23.0 dB
  2,        // -23.5 dB
  2,        // -24.0 dB
  2,        // -24.5 dB
  2,        // -25.0 dB
  1         // -25.5 dB
}; /*mps_ps2eng_table*/

hdrsrchfing_struct_type hdrsrchfing;
/* this is an all encompassing structure which includes all of the fing
   info in one place */

/* <EJECT> */
/*==========================================================================

                     Random Data Collection

==========================================================================*/


#if defined(FEATURE_SEC)

/* Declare temporary buffer for filling in random data */
#define SEC_RND_DEFN(n) \
  byte sec_rnd_buf[n], *sec_rnd_ptr=sec_rnd_buf;

/* Add byte of random data to buffer */
#define SEC_RND_DATA(x) \
  *sec_rnd_ptr++ = (x);

/* Submit buffer to security utility module */
#define SEC_RND_DONE( ) \
  secutil_fill_rnd_data( sec_rnd_buf, sec_rnd_ptr-sec_rnd_buf);

#define SEC_RND_BUFF_LIMIT_CHECK( ) \
  HDR_ASSERT( ( sec_rnd_ptr - sec_rnd_buf ) <= ( HDRSRCH_MAX_DEMOD_CARRIERS_CNT * HDR_MAX_FINGS_CNT * 1 ) );
    /* KW fix : Check for buffer offset limit */

#else

/* No-ops */

#define SEC_RND_DEFN(n)
#define SEC_RND_DATA(x)
#define SEC_RND_DONE( )
#define SEC_RND_BUFF_LIMIT_CHECK( )

#endif /* FEATURE_SEC && !FEATURE_SEC_CALLS_SEARCHER_RANDOM */

/*=========================================================================

FUNCTION     : HDRSRCHFING_MPS_ECIO_TO_ENG

DESCRIPTION  : This function converts an energy value in Ec/Io (-0.5dB) to
               a linear scale value

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

uint16 hdrsrchfing_mps_ecio_to_eng
(
  int                             eng
    /* Energy in -0.5dB units */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( eng < MPS_PS2ENG_TABLE_MIN )
  {
    /* We don't need to use finger MPS table but regular table will do */
    return hdrsrchset_ecio_to_eng( eng );
  }

  if ( eng > MPS_PS2ENG_TABLE_MIN + MPS_PS2ENG_TABLE_SIZE - 1 )
  {

    return mps_ps2eng_table[ MPS_PS2ENG_TABLE_SIZE - 1 ];
      /* Lower end Saturation at the last entry in the table */
  }

  return mps_ps2eng_table[ eng - MPS_PS2ENG_TABLE_MIN];

} /* hdrsrchfing_mps_ecio_to_eng() */


/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHFING_INIT

DESCRIPTION  : This function initializes finger database. Deassigns all
               the fingers in the hardware.

DEPENDENCIES : This should be executed only one time after hdrsrch is started.
               This should not be called every time system loss happens.

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchfing_init( void )
{
  int8    c;           /* loop counter */

  int8    f;           /* loop counter */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Fingers are deassigned in the start state. One time initialization.
     NOTE: SHOULD NOT BE CALLED EVERYTIME SYSTEM LOSS OCCURS  */

  /* TBD */
  /* Check the firmware register HDR_NUM_FINGERS. 
     If it is different from HDR_MAX_FINGS_CNT, fatal error.*/

  for ( c = 0; c < HDRSRCH_MAX_DEMOD_CARRIERS_CNT; c++ )
  {
    for ( f = 0; f < HDR_MAX_FINGS_CNT; f++ )
    {

      hdrsrchfing.fings[c][f].demod_carrier = c;
        /* Set demod carrier */

      hdrsrchfing.fings[c][f].pn_offset = HDR_NO_PN_OFFSET;
        /* set PN offset to FFFF to indicate it is unlocked finger */

      hdrsrchfing.fings[c][f].dsp_regs.ant_sel = hdrsrchfing.params.default_ant;
        /* initialize default antenna for all fingers */

      hdrsrchfing.fings[c][f].dsp_regs.init_sinr = HDRSRCHFING_DEFAULT_INIT_SINR;
        /* Initialize default finger Ior/Nt. */

      hdrsrchfing.fings[c][f].modified  = FALSE;
    }/* for f */
  } /* for c */

  hdrsrchfing.mps.pending = FALSE;
    /* Default is the no MPS threshold change pending */

  hdrsrchfing.mps.thresh  = HDRSRCH_MPS_PILOT_THRESH - 6;
    /* initialize the pilot_drop threshold to MPS+3dB i.e. -12dB+3dB=-9dB */

  /* Set default to 1 demod carrier */
  hdrsrchfing.params.max_demod_carriers = HDRSRCH_MAX_DEMOD_CARRIERS_CNT;
    /* default maximum number of demod carriers */

  hdrsrchfing.params.demod_carrier_mask = 0;
    /* By default no demod carrier is enabled */

  hdrsrchfing.params.max_fings    = HDR_MAX_FINGS_CNT;
    /* Default maximum number of fingers per carrier */

  hdrsrchfing.params.max_fings_pp = HDR_MAX_FINGS_PER_PILOT;
    /* Default maximum number of fingers per pilot */

  msgr_init_hdr( &hdrsrchfing.fw_msg.assign_msg.hdr, 
                 MSGR_HDR_SRCH, 
                 HDR_FW_FING_ASSIGN_CMD );
    /* Init message header */

  msgr_init_hdr( &hdrsrchfing.fw_msg.lock_thresh_msg.hdr, 
                 MSGR_HDR_SRCH, 
                 HDR_FW_FING_LOCK_THRESH_CMD );
    /* Init message header */

}/* hdrsrchfing_init() */


/*=========================================================================

FUNCTION     : HDRSRCHFING_CHECK_FING_ASSIGN_STATUS

DESCRIPTION  : This function checks whether there is an outstanding finger
               assignment. If yes, wait for some time for it to complete.
               If the finger mask is not cleared after the waiting, return
               FALSE. Otherwise it returns TRUE.

DEPENDENCIES : None

RETURN VALUE : TRUE when a new finger assignment can be started.
               FALSE otherwise

SIDE EFFECTS : None

=========================================================================*/

boolean hdrsrchfing_check_fing_assign_status( void )
{
  boolean status;         /* Finger assignment status */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  status = hdrsrch_wait_for_high_pri_rsp( HDRSRCHFING_ASSIGNMENT_DONE_WAIT_US, 
                                          HDRSRCH_FING_ASSIGN_RSP );
      /* wait for 0.5 slot for the previous assignment to complete      */

  return status;

} /* hdrsrchfing_check_fing_assign_status */


/*=========================================================================

FUNCTION     : HDRSRCHFING_UPDATE_DIVERSITY

DESCRIPTION  : This function will update diversity flag after diversity
               is turned on/off.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchfing_update_diversity ( boolean div_flag )
{

  uint8  c;                                /* carrier index register */

  uint8  f;                                /* finger index register */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  for ( c = 0; (c < hdrsrchfing.params.max_demod_carriers) && (c < HDRSRCH_MAX_DEMOD_CARRIERS_CNT); c++ ) 
  {  
    if ( HDRSRCHFING_DEMOD_CARRIER_ENABLED( c ) )
    {
      for ( f = 0; (f < hdrsrchfing.params.max_fings) && (f < HDR_MAX_FINGS_CNT); f++ )
      {
        if ( HDR_FING_ASSIGNED( c, f ) )
        {
          hdrsrchfing.fings[c][f].dsp_regs.div  = 
                                     hdrsrchrfdiv_diversity_demod_is_enabled();
            /* Update the finger diversity status for assigned fingers. */

        }/* if fing assigned */

      }/* for f */
    } /* if ( HDRSRCHFING_DEMOD_CARRIER_ENABLED( c ) ) */
  } /* for c */

}/* hdrsrchfing_update_diversity( boolean ) */


/*=========================================================================

FUNCTION     : HDRSRCHFING_DIVERSITY_INIT

DESCRIPTION  : This function initializes parameters for the RX diversity

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchfing_diversity_init
(
  boolean div_flag               /* diversity control flag */
)
{

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchfing_update_diversity( div_flag );
    /* make sure that this function is executed after setting max_fings. Don't
       change its position */

#ifdef FEATURE_HDR_NEEDS_QDSP6_PORT
  /* Set the Rake C/I Caps for QAM16 and QAM64 */
  if ( div_flag == TRUE )
  {
    HDRFING_SET_RAKE_CI_CAP_DIV();
  }
  else
  {
    HDRFING_SET_RAKE_CI_CAP();
  }
#endif /* FEATURE_HDR_NEEDS_QDSP6_PORT */

  hdrsrchfing.params.default_ant  = HDR_FING_ANT0;
    /* always select antenna0 by default. The diversity finger pairing is
       done in the firmware */

} /* hdrsrchfing_diversity_init() */


/*=========================================================================

FUNCTION     : HDRSRCHFING_UPDATE_FING_TIMERS

DESCRIPTION  : Updates the finger lock timer statistics

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchfing_update_fing_timers( void )
{
  int8 c;                       /* Carrier index */
  int8 f;                       /* Finger index */
  int8 num_assigned_fingers = 0; /* Used to count assigned fingers. */
  int8 num_locked_fingers = 0;   /* Used to count locked fingers. */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  for ( c = 0; (c < hdrsrchfing.params.max_demod_carriers) && (c < HDRSRCH_MAX_DEMOD_CARRIERS_CNT); c++ ) 
  {
    if ( HDRSRCHFING_DEMOD_CARRIER_ENABLED( c ) )
    {
      for ( f = 0; (f < hdrsrchfing.params.max_fings) && (f < HDR_MAX_FINGS_CNT); f++ )
      {
        if ( HDR_FING_ASSIGNED( c, f ) )
        {
          num_assigned_fingers++;
            /* Increment the number of assigned fingers for logging purposes. */

          if( hdrsrchfing.fings[c][f].locked )
          {
            /*---------------*
             * Finger LOCKED *
             *---------------*/
            num_locked_fingers++;
              /* Increment the number of locked fingers for logging purposes. */

            /* Lock age is saturated at 0xffff which can happen after
               approx 32 mins of continual finger lock if triage is executed
               every 30ms */

            if ( hdrsrchfing.fings[c][f].lock_age < 0xffff )
            {
              hdrsrchfing.fings[c][f].lock_age++;
                /* increment the lock age */
            }/* if */

            hdrsrchfing.fings[c][f].unlock_age = 0;
              /* reset the unlock age */

          }/* if fing locked */
          else
          {
            /*-----------------*
             * Finger UNLOCKED *
             *-----------------*/

            hdrsrchfing.fings[c][f].lock_age = 0;
              /* finger is unlocked */

            /* Lock age is saturated at 0xffff which can happen after
               approx 32 hours of continual finger unlock if triage is executed
               every 30ms */

            if ( hdrsrchfing.fings[c][f].unlock_age < 0xffff )
            {
              hdrsrchfing.fings[c][f].unlock_age++;
                /* increment unlock age */

            }/* if */

          }/* else fing is unlocked */

        }/* if fing is assigned */

      }/* for f */
    }/* if ( HDRSRCHFING_DEMOD_CARRIER_ENABLED( c ) ) */
  }/* for c */

  hdrsrchlog_inc_conn_state_stats_counter( HDRSRCHLOG_LOCKED_FINGERS_COUNT, 
                                           num_locked_fingers );
    /* Keep track of the number of locked fingers for logging purposes. */

  hdrsrchlog_inc_conn_state_stats_counter( HDRSRCHLOG_ASSIGNED_FINGERS_COUNT, 
                                           num_assigned_fingers );
    /* Keep track of the number of assigned fingers for logging purposes. */

}/* hdrsrchfing_update_fing_timers( ) */


/*=========================================================================

FUNCTION     : HDRSRCHFING_READ_FING_STATUS

DESCRIPTION  : This function reads the finger status information from the
               MSM and updates the hdrsrchfing.fings[] table with it. This is
               equivalent to the srch_fing_dmp() in the IS-95 series of s/w.
               This function should be called everytime the finger databases
               needs to be refreshed.

               It is recommended that this function be called at the end of the
               search dump ISR. Thus the set evaluation and triage routines
               get to work with the latest finger status.

DEPENDENCIES : None

INPUTS       : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchfing_read_fing_status( void )
{
  int8 c;                                 /* carrier */

  uint8 f;                                /* finger index register */

  /* Random Data buffer: max 8 fingers * 1 bytes/finger */
  SEC_RND_DEFN(HDRSRCH_MAX_DEMOD_CARRIERS_CNT * HDR_MAX_FINGS_CNT * 1 )

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  REX_DISABLE_PREMPTION();

  hdrsrchdrv_update_mstr_x8();
   /* Get latest MSTR dump */
  for ( c = 0; (c < hdrsrchfing.params.max_demod_carriers) && (c < HDRSRCH_MAX_DEMOD_CARRIERS_CNT) && (c < HDRFW_TOTAL_CARRIERS ); c++ ) 
  {
    if ( HDRSRCHFING_DEMOD_CARRIER_ENABLED( c ) )
    {
      for ( f = 0; (f < hdrsrchfing.params.max_fings) && (f < HDR_MAX_FINGS_CNT); f++ )
      {
        hdrsrchfing.fings[c][f].rssi         = HDRFING_GET_RSSI( c, f );
          /* read the finger RSSI. If the fing is unassigned then RSSI may be junk */

        hdrsrchfing.fings[c][f].posx8        = HDRFING_GET_RTC_OFFSET_X8( c, f );

        if( hdrsrchfing.fings[c][f].modified )
        {
          hdrsrchfing.fings[c][f].locked      = TRUE;
        }/* if */
        else
        {
          hdrsrchfing.fings[c][f].locked      = HDRFING_GET_LOCK( c, f );
          /* read lock status which is either 1 or 0 (boolean) */
        }/* else f */

        if ( hdrsrchfing.fings[c][f].locked )
        {
          SEC_RND_DATA( hdrsrchfing.fings[c][f].rssi & 0xFF )
            /* Collect random data */
        }

      }/* for f */
    } /* if ( HDRSRCHFING_DEMOD_CARRIER_ENABLED( c ) ) */
  } /* for c */

  REX_ENABLE_PREMPTION();

  SEC_RND_BUFF_LIMIT_CHECK ( )
    /* KW fix : Check for buffer offset limit */

  SEC_RND_DONE( )
    /* Submit random data to security utility module */

  /* Print the finger information */
  for ( c = 0; c < hdrsrchfing.params.max_demod_carriers; c++ )
  {
    for ( f = 0; f < hdrsrchfing.params.max_fings; f++ )
    {
      if( hdrsrchfing.fings[c][f].pn_offset != HDR_NO_PN_OFFSET )
      {

        HDR_MSG_SRCH_7( MSG_LEGACY_LOW, 
                        "f[%d][%d],l[%d],pn[%d],pos[%d],r[%d],sp[%08x]", 
                        c, f, hdrsrchfing.fings[c][f].locked,
                        hdrsrchfing.fings[c][f].pn_offset,
                        hdrsrchfing.fings[c][f].posx8,
                        hdrsrchfing.fings[c][f].rssi,
                        hdrsrchfing.fings[c][f].sect_ptr );
      }
    }
  }
}/* hdrsrchfing_read_fing_status() */


/*=========================================================================

FUNCTION     : HDRSRCHFING_WRITE_FING_PARAMS

DESCRIPTION  : Writes Finger Parameters to the DSP registers

DEPENDENCIES : None

INPUTS       : Carrier and Finger Index to be written to the hardware

RETURN VALUE : None

=========================================================================*/

void hdrsrchfing_write_fing_params
(
  int8       c,
    /* carrier */
  
  int8       f
    /* finger */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( hdrsrchfing.fings[c][f].pn_offset == HDR_NO_PN_OFFSET )
  {
    HDRFING_DISABLE( c, f );
    HDR_MSG_SRCH_2( MSG_LEGACY_LOW, "f[%d][%d] is disabled", c, f );
  }
  else
  {
    HDRFING_ENABLE( c, f );

    /*HDRFING_SET_PN( f, hdrsrchfing.fings[f].pn_offset );*/
    /* This is removed in RevB */

    /*HDRFING_START_LOCKED( f );*/
    /* This is removed in RevB */

    /*HDRFING_SELECT_ANT( f, hdrsrchfing.fings[f].dsp_regs.ant_sel );*/
    /* This is removed in RevB */

    /* TBD
    ** The finger assignment parameters will be written to the shared memory 
    ** negoitated to power-up. 
    ** The message will just have a flag to program the fingers. 
    ** One the message is received by FW, it will access the shared memory.
    */

    HDRFING_SET_RTC_X8( c, f, hdrsrchfing.fings[c][f].dsp_regs.rtc_offsetx8 );

    HDRFING_SET_ASP_MAP( c, f, hdrsrchfing.fings[c][f].dsp_regs.asp_idx );

    /*HDRFING_SET_DIVERSITY( f, hdrsrchfing.fings[f].dsp_regs.div );*/
    /* This is removed in RevB */

    HDRFING_SET_INIT_SNR( c, f, hdrsrchfing.fings[c][f].dsp_regs.init_sinr );

    HDR_MSG_SRCH_7( MSG_LEGACY_LOW, 
                    "f[%d][%d] commited to pn[%d] asp[%d] ch[%d] bd[%d] pos[%08x]",
                    c, f,
                    hdrsrchfing.fings[c][f].pn_offset,
                    hdrsrchfing.fings[c][f].dsp_regs.asp_idx,
                    hdrsrchfing.fings[c][f].sect_ptr->chan.chan_num,
                    hdrsrchfing.fings[c][f].sect_ptr->chan.band,
                    hdrsrchfing.fings[c][f].posx8 );
  }
} /* hdrsrchfing_write_fing_params */


/*=========================================================================

FUNCTION     : HDRSRCHFING_CHECK_STRAY_FING

DESCRIPTION  : This function looks for a stray finger which is more than
               ~320 chips from each other. DSP crashes if the fingers are
               more than those many chips from each other.

DEPENDENCIES : None

INPUTS       : None

RETURN VALUE : None
=========================================================================*/
extern int32 hdrsrchtri_distx8 ( int32 pos1,int32 pos2 );

void hdrsrchfing_check_stray_fing( void )
{
  int32     distx8, max_diffx8 = 0;
  int32     fing_pos1 = 0, fing_pos2 = 0;   /* finger positions   */
  int8      f1,    f2;                      /* finger loop index */
  int8      maxf1 = 0, maxf2 = 0;     /* fingers that are too far apart */
  int32     max_pos1 = 0, max_pos2 = 0;  /* finger positions far apart */
  int8      c;                        /* carrier index */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  for ( c = 0; 
       ( ( c < hdrsrchfing.params.max_demod_carriers ) &&
          ( c < HDRSRCH_MAX_DEMOD_CARRIERS_CNT ) );
        c++ ) 
  {
    if ( HDRSRCHFING_DEMOD_CARRIER_ENABLED( c ) )
    {

      for ( f1 = 0; (f1 < hdrsrchfing.params.max_fings) && (f1 < HDR_MAX_FINGS_CNT); f1++ )
      {
        if ( !HDR_FING_ASSIGNED( c, f1 ) )
        {
          continue;
        }

        /* finger f1 is assigned, remember its position below */
        if ( hdrsrchfing.fings[c][f1].modified == TRUE ) 
        {
          /* f1 is assigned, but assignment is still pending  */
          fing_pos1 = hdrsrchfing.fings[c][f1].dsp_regs.rtc_offsetx8;
        }
        else
        {
          /* f1 was assigned in the past  */
          fing_pos1 = hdrsrchfing.fings[c][f1].posx8;
        }

        for ( f2 = f1 + 1; 
              ( ( f2 < hdrsrchfing.params.max_fings ) &&
                ( f2 < HDR_MAX_FINGS_CNT ) );
              f2++ )
        {
          if ( !HDR_FING_ASSIGNED( c, f2 ) )
          {
            continue;
          }

          /* finger f2 is assigned, remember its position below */
          if ( hdrsrchfing.fings[c][f2].modified == TRUE ) 
          {
            /* f2 is assigned, but assignment is still pending  */
            fing_pos2 = hdrsrchfing.fings[c][f2].dsp_regs.rtc_offsetx8;
          }
          else
          {
            /* f2 was assigned in the past  */
            fing_pos2 = hdrsrchfing.fings[c][f2].posx8;
          }

          distx8 = hdrsrchtri_distx8( fing_pos1, fing_pos2 );

          distx8 = LABS_VAL( distx8 );
            /* take absolute value of the distance */

          if ( distx8 > max_diffx8 )
          {
            max_diffx8 = distx8;
            maxf1      = f1;
            maxf2      = f2;
            max_pos1   = fing_pos1;
            max_pos2   = fing_pos2;
          }
        }/* f2 */
      }/* f1 */

      /* check to see if the fingers have been too far apart. This check does not
         make sense if one of the finger has not been re-assigned recently since
         the dsp_regs.rtc_offsetx8 is changed only after finger is re-assigned.
         Lock age is set to zero if the finger is re-assigned so use it */

      ASSERT( ( maxf1 < HDR_MAX_FINGS_CNT ) && ( maxf2 < HDR_MAX_FINGS_CNT ) );

      if ( max_diffx8 > (320 * 8)
           && hdrsrchfing.fings[c][maxf1].lock_age != 0xffff
           && hdrsrchfing.fings[c][maxf2].lock_age != 0xffff )
      {
        ERR("Two Fings far apart: F[%x] x8= %d %d", ((maxf2 << 8) | maxf1),
                                                    max_pos1,
                                                    max_pos2);
      }
    } /* if ( HDRSRCHFING_DEMOD_CARRIER_ENABLED( c ) ) */
  }/* c */

}/* hdrsrchfing_check_stray_fing */


/*=========================================================================

FUNCTION     : HDRSRCHFING_PROGRAM_FINGS_TO_FW

DESCRIPTION  : This function is very widely used by the demodulator module.
               It scans through the global hdrsrchfing.fings[] structure for the
               modified finger parameter structure. It then copies the those
               finger parameters to the MSM.

               It should be noted however that the finger assignment procedure
               for TAZ2 differs from its IS-95 counterparts. TAZ2 finger mask
               register, which controls the finger assignments, is sampled at
               a fixed rate of 600Hz. The DSP firmware reserves rights to act
               upon the finger assignment request when it feels appropriate.
               Hence it is possible that the uP assigns fingers to the DSP but
               the actual assignment takes place after a while.

               This function always operates on the hdrsrchfing.fings[] struct and
               hence does not accept any parameters.

DEPENDENCIES : None

INPUTS       : TRUE: assign finger and slam 
               FALSE: assign finger and no slam 

RETURN VALUE : TRUE: the FW msg is sent 
               FALSE: the FW msg is not sent 
=========================================================================*/
boolean hdrsrchfing_program_fings_to_fw
(
  boolean          slam
     /* whether to slam or not */
)
{
  int8   c;                                /* carrier index register */
  uint8  f;                                /* finger index register */
  uint16 fing_mask = 0;                    /* finger mask read from the DSP */
  boolean fing_assign_status;              /* assignment status     */
  boolean fing_modified = FALSE;           /* Finger is modified */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "hdrsrchfing_program_fings_to_fw %d", slam );

  memset( &hdrsrchfing.fw_msg.assign_msg, 
          0, 
          sizeof( hdrfw_fing_assign_msg_t ) );
    /* Clear the finger assign msg payload */

  msgr_init_hdr( &hdrsrchfing.fw_msg.assign_msg.hdr, 
                 MSGR_HDR_SRCH, 
                 HDR_FW_FING_ASSIGN_CMD );
    /* Init message header */

  hdrsrchfing_check_stray_fing();
    /* check to see if we are assigning any stray fingers */

  fing_assign_status = hdrsrch_wait_for_high_pri_rsp( HDRSRCHFING_ASSIGNMENT_DONE_WAIT_US,
                                                      HDRSRCH_FING_ASSIGN_RSP );
    /* Check if we can assign fingers */

  if ( fing_assign_status == FALSE ) 
  {
    return (FALSE);
      /* DSP is missing the timeline, skip the current finger assignment  */
  }

  for ( c = 0; (c < hdrsrchfing.params.max_demod_carriers)&& (c < HDRSRCH_MAX_DEMOD_CARRIERS_CNT) 
                             &&(c < HDRFW_TOTAL_CARRIERS ); c++) 
  {
    for ( f = 0; f < HDR_MAX_FINGS_CNT; f++ )
    {
      if ( hdrsrchfing.fings[c][f].modified )
      {
        /*--------------------------------------*
         * Finger f parameters need programming *
         *--------------------------------------*/

        hdrsrchfing.fings[c][f].modified = FALSE;

        HDR_FING_MASK_ENABLE( f, fing_mask );
          /* enable fing mask local variable */
  
        hdrsrchfing_write_fing_params( c, f );
          /* write finger parameters */

      }/* if fing modified */

    }/* for f */

    if ( fing_mask )
    {
      fing_modified = TRUE;
        /* Finger is modified */

      hdrsrchfing.fw_msg.assign_msg.assignMask[c] = fing_mask;
        /* Update message */

      fing_mask = 0;
        /* Clear finger mask */
    }
  } /* for c */

  if ( fing_modified == TRUE )
  {
    if ( slam == TRUE )
    {
      hdrsrchfing.fw_msg.assign_msg.slamWithFinger = TRUE;
    }
    else
    {
      hdrsrchfing.fw_msg.assign_msg.slamWithFinger = FALSE;
    }

    hdrsrchmsg_send_msg( &hdrsrchfing.fw_msg.assign_msg.hdr,
                         sizeof( hdrfw_fing_assign_msg_t ) );
      /* Send the message */

    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "Send finger msg, slamWithFinger %d", 
                    hdrsrchfing.fw_msg.assign_msg.slamWithFinger );

    if ( hdrmdsp_is_mdsp_active( ) )
	{
		hdrsrchmsg_set_rsp_waiting( HDRSRCH_FING_ASSIGN_RSP, TRUE );
  
		if ( slam == TRUE )
		{
		  hdrsrchmsg_set_rsp_waiting( HDRSRCH_MSTR_SLAM_RSP, TRUE );
			/* Set the flag */
		}
	}
	else
	{
		HDR_MSG_SRCH_1( MSG_LEGACY_MED, 
						"Fing assign sent while FW != active, Slam sent:%d", 
						slam );
	}	  

  }

  return (TRUE);

}/* hdrsrchfing_program_fings_to_fw() */


/*=========================================================================

FUNCTION     : HDRSRCHFING_PROGRAM_FINGS

DESCRIPTION  : This function is very widely used by the demodulator module.
               It scans through the global hdrsrchfing.fings[] structure for the
               modified finger parameter structure. It then copies the those
               finger parameters to the MSM.

               It should be noted however that the finger assignment procedure
               for TAZ2 differs from its IS-95 counterparts. TAZ2 finger mask
               register, which controls the finger assignments, is sampled at
               a fixed rate of 600Hz. The DSP firmware reserves rights to act
               upon the finger assignment request when it feels appropriate.
               Hence it is possible that the uP assigns fingers to the DSP but
               the actual assignment takes place after a while.

               This function always operates on the hdrsrchfing.fings[] struct and
               hence does not accept any parameters.

DEPENDENCIES : None

INPUTS       : None

RETURN VALUE : None
=========================================================================*/

boolean hdrsrchfing_program_fings( void )
{
  return hdrsrchfing_program_fings_to_fw( FALSE );
} /* hdrsrchfing_program_fings */


/*=========================================================================

FUNCTION     : HDRSRCHFING_PROGRAM_FINGS_AND_SLAM

DESCRIPTION  : This function assigns finger and set slam flag.

DEPENDENCIES : None

INPUTS       : None

RETURN VALUE : TRUE: the FW msg is sent 
               FALSE: the FW msg is not sent 
=========================================================================*/
boolean hdrsrchfing_program_fings_and_slam( void )
{
  HDR_MSG_SRCH( MSG_LEGACY_HIGH, "hdrsrchfing_program_fings_and_slam" );

  return hdrsrchfing_program_fings_to_fw( TRUE );
} /* hdrsrchfing_program_fings_and_slam */


/*=========================================================================

FUNCTION     : HDRSRCHFING_SET_RTC_OFFSET

DESCRIPTION  : Sets the RTC Offset of a finger

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchfing_set_rtc_offset
(
  hdrsrchfing_fing_struct_type *fing,
    /* finger to be updated */

  uint32  rtc_offset_x8
    /* RTC Offset to program to the finger */
)
{

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  fing->dsp_regs.rtc_offsetx8 = rtc_offset_x8;

} /* hdrsrchfing_set_rtc_offset( ) */


/* EJECT */
/*=========================================================================

FUNCTION     : HDRSRCHFING_DEASSIGN_FING

DESCRIPTION  : Sets the finger parameters so as to deassign it. The new
               assignment is NOT committed to hardware in this function.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchfing_deassign_fing
(
  hdrsrchfing_fing_struct_type *fing_ptr
    /* finger to be deassigned */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

   /* Clear the finger parameters */
  fing_ptr->pn_offset             = HDR_NO_PN_OFFSET;
  fing_ptr->locked                = FALSE;
  fing_ptr->sect_ptr              = NULL;
  fing_ptr->lock_age              = 0;
  fing_ptr->unlock_age            = 0;
  fing_ptr->modified              = TRUE;
  fing_ptr->dsp_regs.init_sinr    = HDRSRCHFING_DEFAULT_INIT_SINR;

} /* hdrsrchfing_deassign_fing */

/*=========================================================================

FUNCTION     : HDRSRCHFING_DEASSIGN_ALL_FINGS

DESCRIPTION  : Deassigns all the fingers

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchfing_deassign_all_fings ( void )
{
  int8    c;            /* carrier loop variable */

  int8    f;            /* finger loop variable */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  for (c = 0;(c < hdrsrchfing.params.max_demod_carriers) && (c < HDRSRCH_MAX_DEMOD_CARRIERS_CNT); c++ ) 
  {
    if ( HDRSRCHFING_DEMOD_CARRIER_ENABLED( c ) )
    {

      for ( f = 0; (f < hdrsrchfing.params.max_fings) && (f <  HDR_MAX_FINGS_CNT); f++ )
      {
        hdrsrchfing_deassign_fing( &hdrsrchfing.fings[c][f] );
      }/* for f */
    } /* if ( HDRSRCHFING_DEMOD_CARRIER_ENABLED( c ) ) */
  }/* for c */

  (void) hdrsrchfing_program_fings();
    /* deassign the fingers */

} /* hdrsrchfing_deassign_all_fings */


/* EJECT */
/*=========================================================================

FUNCTION     : HDRSRCHFING_RESET_ALL_FINGS

DESCRIPTION  : Reset all the fingers for fresh assignment

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : Should be used only during fast acq procedure

=========================================================================*/

void hdrsrchfing_reset_all_fings ( void )
{
  int8    c;            /* carrier loop variable */

  int8    f;            /* finger loop variable */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH( MSG_LEGACY_HIGH, "Resetting all fingers");

  for (c = 0;(c < hdrsrchfing.params.max_demod_carriers) && (c < HDRSRCH_MAX_DEMOD_CARRIERS_CNT); c++ ) 
  {
    if ( HDRSRCHFING_DEMOD_CARRIER_ENABLED( c ) )
    {
      for ( f = 0; (f < hdrsrchfing.params.max_fings) && (f <  HDR_MAX_FINGS_CNT); f++ )
      {
        if ( HDR_FING_ASSIGNED( c, f ) )
        {
           HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "Found finger with pn_offset %d assigned. Clearing it",
                  hdrsrchfing.fings[(c)][(f)].pn_offset);
        }/* if */
        hdrsrchfing_deassign_fing( &hdrsrchfing.fings[c][f] );
      }/* for f */
    } /* if ( HDRSRCHFING_DEMOD_CARRIER_ENABLED( c ) ) */
  }/* for c */

} /* hdrsrchfing_reset_all_fings */


/* EJECT */

/*=========================================================================

FUNCTION     : HDRSRCHFING_DEASSIGN_SECT_FINGS

DESCRIPTION  : Deassigns all the fingers that are assigned to this
               particular sector. However the deassignment is NOT
               committed to the hardware.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchfing_deassign_sect_fings
(
  hdrsrch_sect_struct_type* sect_ptr
    /* sector to be deassigned */
)
{
  int8 f;            /* finger loop variable */
  uint8 c = 0;       /* carrier */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  c = hdrsrchset_get_demod_carrier_index( sect_ptr->aset.subaset_idx ); 
    /* Get demod carrier index */

  ASSERT( c < HDRSRCH_MAX_DEMOD_CARRIERS_CNT );

  for ( f = 0; f < hdrsrchfing.params.max_fings; f++ )
  {
    if ( hdrsrchfing.fings[c][f].pn_offset == sect_ptr->pn_offset )
    {

      hdrsrchfing_deassign_fing( &hdrsrchfing.fings[c][f] );

    }/* if */

  }/* for f */

} /* hdrsrchfing_deassign_sect_fings */


/* EJECT */
/*=========================================================================

FUNCTION     : HDRSRCHFING_SAVE_RTC_OFFSET

DESCRIPTION  : Updates the finger with the current finger rtc offset.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : Fing position is updated to the current mdsp finger positions.

=========================================================================*/

LOCAL void hdrsrchfing_save_rtc_offset
(
  int8   c,
    /* Carrier */
  
  uint8 idx 
    /* Finger index */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* If the finger is assigned to a sector and is in lock */

  if( ( idx < HDR_MAX_FINGS_CNT ) &&
      hdrsrchfing.fings[c][idx].locked &&
      ( hdrsrchfing.fings[c][idx].sect_ptr != NULL ) )
  {
    /* This is needed to avoid the finger pos jump when only ASET parameters
     * are changed */

    /* Define separate macros */
    hdrsrchfing.fings[c][idx].dsp_regs.rtc_offsetx8 = 
                       HDRFING_GET_RTC_OFFSET_X8( c, idx );
  }
} /* hdrsrchfing_save_rtc_offset( ) */



/*=========================================================================

FUNCTION     : HDRSRCHFING_UPDATE_WITH_SECTOR

DESCRIPTION  : Updates the finger parameters to match the parameters of
               the sector the finger is assigned to.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : If any changes are made, the "modified" flag is set.

=========================================================================*/

LOCAL void hdrsrchfing_update_with_sector
(
  int8  c,
    /* Carrier */
  
  uint8 f_idx
    /* finger index */
)
{
  hdrsrchfing_fing_struct_type *fing;
    /* finger to be updated */

  hdrsrch_sect_struct_type  *sect;
    /* Convineance pointer to finger's sector */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( f_idx >= HDR_MAX_FINGS_CNT )
  {
    ERR_FATAL( "Index of finger is out of array boundary: %d  ", 
               f_idx, 0, 0 );
  }

  fing = &hdrsrchfing.fings[c][f_idx];
  sect = hdrsrchfing.fings[c][f_idx].sect_ptr;

  /* If the finger is assigned to a sector ... */

  if ( sect != NULL )
  {
    /* If any finger parameter disagrees with the sector parameter,
       update with sector information and set modified flag */


    /* ASP Index */

    if ( fing->dsp_regs.asp_idx != sect->aset.asp_idx )
    {
      fing->dsp_regs.asp_idx        = sect->aset.asp_idx;
      fing->modified                = TRUE;
    }


    /* RPC Cell Index */

    if ( fing->dsp_regs.rpc_cell_idx != sect->aset.rpc_cell_idx )
    {
      fing->dsp_regs.rpc_cell_idx   = sect->aset.rpc_cell_idx;
      fing->modified                = TRUE;
    }


    /* RPC Walsh Index */

    if ( fing->dsp_regs.mac_idx != sect->aset.mac_idx )
    {
      fing->dsp_regs.mac_idx        = sect->aset.mac_idx;
      fing->modified                = TRUE;
    }

    /* Update the finger position if any of above parameters are changed */

    if ( fing->modified )
    {
      hdrsrchfing_save_rtc_offset( c, f_idx );
        /* Save the current mdsp finger position */
    }

  }

} /* hdrsrchfing_update_with_sector( ) */


/*=========================================================================

FUNCTION     : HDRSRCHFING_UPDATE_ALL_SECTORS

DESCRIPTION  : Updates all finger parameters to match the parameters of
               the sectors the fingers are assigned to.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : If any changes are made, the "modified" flag is set.

=========================================================================*/

void hdrsrchfing_update_all_sectors( )
{
  int8 f;            /* finger loop variable */
  int8 c;            /* carrier */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_ASSERT ( ( hdrsrchfing.params.max_demod_carriers <= HDRSRCH_MAX_DEMOD_CARRIERS_CNT ) 
  && ( hdrsrchfing.params.max_fings <= HDR_MAX_FINGS_CNT ) );
    /* KW fix : Check to limit the array index */

  /* TBD */
  /* Need to make sure max_demod_carriers has been updated correctly */

  for ( c = 0; c < hdrsrchfing.params.max_demod_carriers; c++ )
  {
    if ( HDRSRCHFING_DEMOD_CARRIER_ENABLED( c ) )
    {

      for ( f = 0; f < hdrsrchfing.params.max_fings; f++ )
      {
        hdrsrchfing_update_with_sector( c, f );
          /* Update the finger parameters */
      } 
    }/* if ( HDRSRCHFING_DEMOD_CARRIER_ENABLED( c ) ) */
    else
    {
      for ( f = 0; f < hdrsrchfing.params.max_fings; f++ )
      {
        if( hdrsrchfing.fings[c][f].pn_offset != HDR_NO_PN_OFFSET )
        {
          hdrsrchfing_deassign_fing( &hdrsrchfing.fings[c][f] );
            /* Update the finger parameters */
        }
      }  /* for f */ 
    }  /* if HDRSRCHFING_DEMOD_CARRIER_ENABLED( c ) */
  }  /* for c */
} /* hdrsrchfing_update_all_sectors( ) */


/*=========================================================================

FUNCTION     : HDRSRCHFING_UPDATE_FING_THRESH

DESCRIPTION  : This function sets the finger Out-of-lock/In-lock thresholds
               plus preamble detection threshold while doing MPS testing.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : Will change the lock_lo, lock_hi and preamble threshold 
               DSP registers

=========================================================================*/

void hdrsrchfing_update_fing_thresh( void )
{
  uint16                     lo, hi;
    /* finger thresholds lock_lo and lock_hi */


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if( hdrsrchfing.mps.thresh >= HDRSRCH_MPS_PILOT_THRESH )
  {
    lo = hdrsrchfing_mps_ecio_to_eng ( hdrsrchfing.mps.thresh + 10 );
      /* +10 represents -5dB, so the new lo threshold is (pilot_drop-5) dB */

    hi = hdrsrchfing_mps_ecio_to_eng ( hdrsrchfing.mps.thresh + 2 );
      /* The new Hi threshold is the lo value plus 3dB (8-6=2)            */

  }
  else
  {
    lo = HDRSRCH_FING_DEFAULT_LOCK_LO;
    hi = HDRSRCH_FING_DEFAULT_LOCK_HI;

  }

  hdrsrchfing.fw_msg.lock_thresh_msg.fingLockThreshLo = lo;
    /* Set the lo threshold to the default value */

  hdrsrchfing.fw_msg.lock_thresh_msg.fingLockThreshHi = hi;
    /* Set the hi threshold to the default value */

  hdrsrchmsg_send_msg( &hdrsrchfing.fw_msg.lock_thresh_msg, 
                       sizeof( hdrfw_fing_lock_thresh_msg_t ) );
    /* Send fing lock thresh message */

  HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "Change fing thresh:lo=%d hi=%d", lo, hi );


} /* hdrsrchfing_update_fing_thresh()   */


/*=========================================================================

FUNCTION     : HDRSRCHFING_RESTORE_FING_THRESH

DESCRIPTION  : This function restores pending finger threshold updates
               if last update did not happen because dsp clock was off

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : Will change the lock_lo and lock_hi in the DSP registers

=========================================================================*/

void hdrsrchfing_restore_fing_thresh( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( hdrsrchfing.mps.pending )
  {
    /* Yes threshold update is pending */
    hdrsrchfing_update_fing_thresh();

    hdrsrchfing.mps.pending = FALSE;
      /* since the threshold change is complete, set pending to false */
  }

} /* hdrsrchfing_restore_fing_thresh */


/*=========================================================================

FUNCTION     : HDRSRCHFING_PROCESS_PILOT_DROP

DESCRIPTION  : This function restores the finger Out-of-lock/In-lock thresholds
               according to the pilot threshold

               For MPS testing, we set finger In-lock/Out-of-lock thresholds
               according to the pilot_drop value received from the base station.
               If the thresholds are for MPS then we need to operate fingers at
               much lower c/i so we change the finger lock hi and lo. Make sure
               we don't write to DSP in INACTIVE/SLEEP states since the DSP
               clock has been turned off.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : Will change the lock_lo and lock_hi in the DSP registers

=========================================================================*/

void hdrsrchfing_process_pilot_drop
(
  hdrsrch_pilot_strength_type pilot_drop
    /* pilot drop threshold based on which we will write the finger
       thresholds*/
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchfing.mps.thresh  = pilot_drop;
    /* new threshold is pilot_drop */

  if( hdrmdsp_check_dsp_clk() )
  {
    /* Write the finger thresholds depending upon the settings */

    hdrsrchfing_update_fing_thresh();
      /* write the fing thresholds. For MPS or for default */

    hdrsrchfing.mps.pending = FALSE;
      /* no threshold change is pending */
  }
  else
  {
    hdrsrchfing.mps.pending = TRUE;
      /* DSP clocks are off so we can't write right now. Mark it as pending */
  }

}/* hdrsrchfing_process_pilot_drop */


/*=========================================================================

FUNCTION     : HDRSRCHFING_ARE_FINGS_ASSIGNED

DESCRIPTION  : This function checks whether any finger is assigned.

DEPENDENCIES : None

RETURN VALUE : TRUE=   At least one finger is assigned.
               FALSE=  No finger is assigned.

SIDE EFFECTS : None

=========================================================================*/

boolean hdrsrchfing_are_fings_assigned( void )
{
  int8                       c;
    /* Carrier */

  int8                       f;
    /* Finger */

  boolean                    finger_assigned = FALSE;
    /* Whether any finger is assigned */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* TBD */
  /* Need to understand whether fll is affected by fingers 
     on all carrier or only the reference */

   for (c = 0;(c < hdrsrchfing.params.max_demod_carriers) && (c < HDRSRCH_MAX_DEMOD_CARRIERS_CNT); c++ ) 
  {
    if ( HDRSRCHFING_DEMOD_CARRIER_ENABLED( c ) )
    {

      for ( f = 0; (f < hdrsrchfing.params.max_fings) && (f <  HDR_MAX_FINGS_CNT); f++ )
      {
        if ( HDR_FING_ASSIGNED( c, f ) )
        {
          finger_assigned = TRUE;
          break;
        }/* if */
      }/* for f */
    }/* if ( HDRSRCHFING_DEMOD_CARRIER_ENABLED( c ) ) */
  }/* for c */

  return finger_assigned;

}  /* hdrsrchfing_are_fings_assigned */


/*=========================================================================

FUNCTION     : HDRSRCHFING_ARE_FINGS_LOCKED

DESCRIPTION  : This function checks whether any finger is locked.

DEPENDENCIES : None

RETURN VALUE : TRUE=   At least one finger is locked.
               FALSE=  No finger is locked.

SIDE EFFECTS : None

=========================================================================*/

boolean hdrsrchfing_are_fings_locked( void )
{
  int8                       c;
    /* Carrier */

  int8                       f;
    /* Finger */

  boolean                    finger_locked = FALSE;
    /* Whether any finger is assigned */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

   for (c = 0;(c < hdrsrchfing.params.max_demod_carriers) && (c < HDRSRCH_MAX_DEMOD_CARRIERS_CNT) 
                                        && (c < HDRFW_TOTAL_CARRIERS ); c++ ) 
  {
    if ( HDRSRCHFING_DEMOD_CARRIER_ENABLED( c ) )
    {
      for ( f = 0; (f < hdrsrchfing.params.max_fings) && (f <  HDR_MAX_FINGS_CNT); f++ )
      {
        hdrsrchfing.fings[c][f].locked = HDRFING_GET_LOCK( c, f );
          /* Read finger lock status from DSP */

        if ( HDR_FING_LOCKED( &hdrsrchfing.fings[c][f] ) )
        {
          finger_locked = TRUE;
        }/* if */
      }/* for f */
    }/* if ( HDRSRCHFING_DEMOD_CARRIER_ENABLED( c ) ) */
  }/* for c */

  return finger_locked;

}  /* hdrsrchfing_are_fings_locked */



/*=========================================================================

FUNCTION     : HDRSRCHFING_SET_DEMOD_CARRIER_MASK

DESCRIPTION  : This function sets the mask which indicates wich demod 
               carriers are enabled.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchfing_set_demod_carrier_mask
(
  uint8                      mask
    /* Demod carrier mask */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  ASSERT( hdrsrchfing.params.max_demod_carriers == 
          HDRSRCH_MAX_DEMOD_CARRIERS_CNT );

  /* TBD */
  /* Deassign fingers */

  hdrsrchfing.params.demod_carrier_mask = mask;

}  /* hdrsrchfing_set_demod_carrier_mask */


/*=========================================================================

FUNCTION     : HDRSRCHFING_LIMIT_MAX_FINGS

DESCRIPTION  : This function sets the max fingers.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchfing_limit_max_fings
(
  boolean          limited
    /* Whether to limit max fingers */
)
{

#ifdef FEATURE_HDR_JOLOKIA_MODEM_BRINGUP
  uint32           max_q6_clk_speed;
    /* Max Q6 supported frequency */

  max_q6_clk_speed = hdrsrchutil_get_max_q6_clk_speed();
      /* Get max Q6 supported frequency */
      /* max Q6 supported frequency is 691.2MHz for V2 parts
         max Q6 supported frequency is 345.6MHz or 441.6MHz for V1
         and earlier parts. To facilitate the Jolokia bringup on V1 and earlier
         HW with reduced Clks, limit the Max fingers
       */

  if ( (limited == TRUE)
       || (max_q6_clk_speed < 450)
     )
#else
  if (limited == TRUE)
#endif /* FEATURE_HDR_JOLOKIA_MODEM_BRINGUP */
  {
    hdrsrchfing.params.max_fings    = HDR_LIMITED_MAX_FINGS_CNT;
      /* Default maximum number of fingers per carrier */

    hdrsrchfing.params.max_fings_pp = HDR_LIMITED_MAX_FINGS_PER_PILOT;
      /* Default maximum number of fingers per pilot */
  }
  else
  {
    hdrsrchfing.params.max_fings    = HDR_MAX_FINGS_CNT;
      /* Default maximum number of fingers per carrier */

    hdrsrchfing.params.max_fings_pp = HDR_MAX_FINGS_PER_PILOT;
      /* Default maximum number of fingers per pilot */
  }
}  /* hdrsrchfing_set_max_fings */

/*=========================================================================

FUNCTION     : HDRSRCHFING_PROCESS_SLAM_RSP

DESCRIPTION  : This function process slam response from FW

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchfing_process_slam_rsp
(
  boolean slam_success
)
{
  if ( slam_success == FALSE )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, 
      "FingerAssign/Slam(%d) response: FingerAssign/Slam failed report system lost", 
      hdrsrchfing.fw_msg.assign_msg.slamWithFinger );

    hdrsrchutil_system_lost ( HDRSRCH_SYSLOST_MSTR_SLAM_TIMEOUT );
  }
  else
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
      "Slam response: Slam success=%d", slam_success );
  }
}

