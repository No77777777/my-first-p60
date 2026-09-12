/*===========================================================================

                    GL1   T C X O   M A N A G E R

DESCRIPTION
  This file contains code for TCXO Manager handling.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2006-2015 Qualcomm Technologies, Inc.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_hw_tcxo_mgr3.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/02/16   og      CR968371. Refresh rpush freq error prior to the forced rpush.
20/08/15   am      CR890659 Ensure NAS ID to TCXO ID conversion is correct
30/06/15   ws      CR852519 - Resolve GL1 enum conversion compile warnings
12/05/15   am      CR828682 Enable Rpush update from all G clients
21/04/15   cs      CR808438 Ensure any pending TT requests are sent before cell
                   information is updated
31/03/15   pa      CR815621:GRM restructuring for TH 2.0.
15/04/14   zc      CR647502 Adding MSIM Featurization around IS_RPUSH_POSSIBLE API
20/03/14   zc      CR635796 Remove Multi SIM Featurization around RGS update to sleep controller
14/03/14   zc      CR620657 GSM should send AS_ID information to GPS as part of RPush
18/02/14   am      CR604674 Always use as_id based RGS API for multisim target
09/12/13   ws      CR586937 XPLUSG workaround - force tcxo as-d to TCXOMGR_AS_ID_2
                   to avoid RGS corruption from WCDMA subscription
16/09/13   cs      CR503042 Updated XO RGS handling for Triton
                   CR541864 Use non-recursive feu data reset function
06/09/13   cs      CR503042 Updated XO RGS handling for Triton
10/06/13   cgc     CR494390 Remove obsolete TCXO code for XO targets
19/06/12   cs      CR370453 Use the correct wait sig for the TRM/TCXO Manager timeouts
14/03/12   pg      Add support for QuRT
07/07/11   cs      Proper fix for CR294718 that only ever defines one timer instance
15/02/11   cs      CR274647 Use the RPUSH interval from tcxomanager if available
03/08/10   cs      CR229805 Change to use an appropriate global define
                   for the source to give correct ack/report settings
27/04/10   cs      Always apply/ack RESTRICTION_XO for XO targets
04/03/10   cs      CR228695 Cache the rpush ppm value so always valid for active
                   scell/dedicated cell to avoid feeding in bad ncell values
04/02/09   ws      Removed legacy Q4 MDSP services and image for Q6 targets
14/01/10   pg      Adding support for BLAST OS
12/08/09   cs      Avoid unnecessary TCXO restriction updates
28/05/09   cs      Updates to avoid spurious restriction update when TCXO released
05/11/08   cs      CR151843 Queue a DPC for resume restriction if called from ISR
22/09/08   cs      Adding XO support for targets
26/08/08   og      Mustang GPLT changes.
30/06/08   cs      CR149200 Invert PDM/Rotator sign under FEATURE_UMTS_NEGATE_ALL_TCXOMGR_VALUES
27/06/08   ip      CR144361 Move TCXO PDM clock control to tcxomgr under FEATURE_TCXOMGR_TCXO_PDM
22/11/07   cs      Allow the rpush freq_err sign change under FEATURE_TCXOMGR_NEGATE_FREQ_ERROR
                   and remove the acc_freq_err from the rpush as not required
11/06/07   cs      Reapply Update API to support instantaneous rotator value callback
11/06/07   cs      Fix for FTM TCXO release in sweep test
08/06/07   cs      Remove API update and fix WPLT build
04/06/07   cs      Update API to support instantaneous rotator value callback
24/05/07   cs      Make some FEU/TCXO restriction debug conditional
18/05/07   cs      Send back correct PPM rotator val for feu
16/05/07   cs      Improvements to manage state change better for feu
04/05/07   cs      Updates for TCXO restriction support and rotator push,
                   also includes CR117942 fix for TCXOMgr correct release
18/04/07   cs      Put back new rpush interface change that was removed
                   in previous checkin
20/03/07   cs      Update to support restriction management
09/03/07   cs      Updated interface for TCXO Mgr 3.0 Rotator Push for GPS.
12/02/07   agv     Added TCXO Mgr 3.0 Rotator Push for GPS.
24/01/07   agv     Added NULL params to tcxomgr_set_callbacks due to design mods.
23/01/07   agv     Added FEATURE_GSM_TRM around rex calls to allow WPLT to build.
04/12/06   cs      Modified tcxo for ftm use
23/10/06   agv     Fixed a crash for TCXO Mgr 3.0 when release to WCDMA occurs.
05/10/06   agv     Initial version of TCXO Mgr 3.0 support for 7600.
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "geran_variation.h"
#include "comdef.h"

#ifdef CUST_H
#include "customer.h"
#endif

#include "timetick.h"
#include "rex.h"
#include "msg.h"

#include "err.h"
#include "l1_os.h"
#include "task.h"
#include "gl1_hw_tcxo_mgr3.h"
#include "nas_exp.h"
#include "l1_task.h"
#include "ms.h"
#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "l1_drx.h"
#include "l1_isr.h"
#include "gtmrs.h"
#include "gtmrs_g.h"

#include "tcxomgr.h"

#include "geran_dual_sim.h"
#include "gpl1_dual_sim.h"
#include "geran_dual_sim_g.h"

#ifdef FEATURE_TCXOMGR_DYN_RPUSH
#include "tcxomgr_modem.h"
#endif /* FEATURE_TCXOMGR_DYN_RPUSH */

#define GL1_FEU_HDR                      "GL1_FEU: "

#define GL1_TCXO_HDR                     "GL1_XO: "

/* Tick time in frames for frequency error updates via rotator push */
#define CGPS_FEU_RPUSH_SLOW_SEND_TICKS   ( 217 )
#define CGPS_FEU_RPUSH_FAST_SEND_TICKS   ( 1 )

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#define FEATURE_TCXOMGR_MSIM_RPUSH_API
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

#define GL1_TCXO_DATA_INIT { 0, CGPS_FEU_RPUSH_SLOW_SEND_TICKS, FALSE, FALSE }

/* Control structure for the frequency error updates sent by rotator push */
typedef struct {
  int16                     frame_ticks;                 /* Number of ticks */
  int16                     rpush_frame_thresh;          /* Number of ticks to wait before rpush */
  uint32                    acc_samples;                 /* Number of samples taken */
  boolean                   first_sample_slow_clk_taken; /* Indicate if first sample so take timestamp */
  timetick_type             first_sample_slow_clk;       /* First sample slow clock timestamp */
  timetick_type             last_sample_slow_clk;        /* Final sample slow clock timestamp, can be same as above */
  tcxomgr_client_state_type tcxo_state;                  /* Are we in dedicated or idle or unknown */
  boolean                   force_rpush;                 /* Set to force a push */
  boolean                   rpush_sent;                  /* Has the push actually been sent */
} gl1_hw_cgps_feu_t;

/* Control structure for the frequency error updates sent by rotator push */
static gl1_hw_cgps_feu_t gl1_hw_cgps_feu[NUM_GERAN_DATA_SPACES];

/* Final rotator push */
static void    gl1_hw_tcxo_rpush( gas_id_t gas_id );
static boolean gl1_hw_is_rpush_possible( gas_id_t gas_id );

static void gl1_hw_internal_reset_cgps_feu( boolean reset_state,
                                            gas_id_t gas_id );

static void gl1_hw_cgps_feu_push( gas_id_t gas_id );

static void   gl1_hw_set_rpush_update_interval( gas_id_t gas_id );
static uint16 gl1_hw_get_rpush_update_interval( gas_id_t gas_id );

static void  gl1_hw_set_rpush_freq_err_in_ppm( gas_id_t gas_id );
static int32 gl1_hw_get_rpush_freq_err_in_ppm( gas_id_t gas_id );

static void    gl1_hw_set_cgps_feu_rpush_enable( boolean  rpush_enable, gas_id_t gas_id );
static boolean gl1_hw_get_cgps_feu_rpush_enable( gas_id_t gas_id );

/* Control structure for the frequency error updates sent by rotator push */
typedef struct {
  int32          rpush_freq_err_in_ppm;     /* Used to hold valid rpush freq err in PPM*/
  uint16         rpush_update_interval;     /* Used for storing away the active RPUSH
                                               update interval in GSM frames */
  boolean        feu_rpush_enable;          /* Track rpush status set from cb */
  boolean        callbacks_registered;      /* Track rpush status set from cb */
} gl1_hw_tcxo_data_t;

/* Control structure for the frequency error updates sent by rotator push */
static gl1_hw_tcxo_data_t gl1_hw_tcxo_data[NUM_GERAN_DATA_SPACES] =
                            { INITIAL_VALUE(GL1_TCXO_DATA_INIT) };

/*===========================================================================
FUNCTION     GL1_TCXO_RPUSH_MODE_CB

DESCRIPTION
  Invoked by TCXO Manager when the rotator push mode changes.
  If the mode is not TCXOMGR_RPUSH_MODE_OFF,
  we will report the error to GPS
  For GSM the timing of the Push is implicit in the AFC Algorithm update, and all
  the Call Back is telling us is if we send the Raw Results to the
  TCXO Manager for the GPS.
  TCXO manager is not providing the client details in the callback as it
  enables RPUSH for all clients.

DEPENDENCIES
  None.

PARAMETERS
  boolean gl1_tcxo_enable_push - Whether rotator push is being enabled or disabled.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void gl1_hw_tcxo_rpush_mode_cb( tcxomgr_rpush_mode_cb_data_type gl1_tcxo_push_mode )
{
  int i;
  boolean gl1_tcxo_enable_push =
            ( gl1_tcxo_push_mode.mode != TCXOMGR_RPUSH_MODE_OFF );

  if ( gl1_tcxo_enable_push )
  {
     MSG_GERAN_HIGH_0( GL1_TCXO_HDR"Rotator Push ON" );

     /* enable for all G clients */
     for ( i = GERAN_DATA_SPACE_INDEX_1; i < NUM_GERAN_DATA_SPACES; i++ )
     {
       if ( !gl1_hw_get_cgps_feu_rpush_enable( (gas_id_t)i ) )
       {
         /* Force a push at first sample read when first enabled */
         gl1_hw_set_cgps_feu_sample_threshold( CGPS_FEU_RPUSH_FAST_SEND_TICKS, (gas_id_t)i );
       }

       /* Set the RPUSH update interval in GSM frames */
       gl1_hw_set_rpush_update_interval((gas_id_t)i);
     }
  }
  else
  {
     MSG_GERAN_HIGH_0( GL1_TCXO_HDR"Rotator Push OFF" );
  }

  /* Set the local GL1 flag to say whether rpush on/off */
  for ( i = GERAN_DATA_SPACE_INDEX_1; i < NUM_GERAN_DATA_SPACES; i++ )
  {
    gl1_hw_set_cgps_feu_rpush_enable(gl1_tcxo_enable_push, (gas_id_t)i);
  }
}

/*===========================================================================
FUNCTION     GL1_TCXO_RPUSH_FLAG_CB

DESCRIPTION
  Invoked by TCXO Manager when the rotator push flag changes. For GSM the
  timing of the Push is implicit in the AFC Algorithm update, and all
  the Call Back is telling us is if we send the Raw Results to the
  TCXO Manager for the GPS.
  TCXO manager is not providing the client details in the callback as it
  enables RPUSH for all clients.

DEPENDENCIES
  None.

PARAMETERS
  boolean gl1_tcxo_enable_push - Whether rotator push is being enabled or disabled.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void gl1_hw_tcxo_rpush_flag_cb( boolean gl1_tcxo_enable_push )
{
  int i;
  if ( gl1_tcxo_enable_push )
  {
     MSG_GERAN_LOW_0( GL1_TCXO_HDR"Rotator Push ON" );

     for ( i = GERAN_DATA_SPACE_INDEX_1; i < NUM_GERAN_DATA_SPACES; i++ )
     {
       if ( !gl1_hw_get_cgps_feu_rpush_enable((gas_id_t)i) )
       {
         /* Force a push at first sample read when first enabled */
         gl1_hw_set_cgps_feu_sample_threshold( CGPS_FEU_RPUSH_FAST_SEND_TICKS, (gas_id_t)i );
       }

       /* Set the RPUSH update interval in GSM frames */
       gl1_hw_set_rpush_update_interval((gas_id_t)i);
     }
  }
  else
  {
     MSG_GERAN_LOW_0( GL1_TCXO_HDR"Rotator Push OFF" );
  }

  for ( i = GERAN_DATA_SPACE_INDEX_1; i < NUM_GERAN_DATA_SPACES; i++ )
  {
    /* Set the local GL1 flag to say whether rpush on/off */
    gl1_hw_set_cgps_feu_rpush_enable( gl1_tcxo_enable_push, (gas_id_t)i );
  }
}

/*===========================================================================
FUNCTION     gl1_hw_tcxo_release

DESCRIPTION
  Releases control of the TCXO.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void gl1_hw_tcxo_release( gas_id_t gas_id )
{
  const gl1_hw_cgps_feu_t*  gl1_hw_cgps_feu_ptr = &gl1_hw_cgps_feu[gas_id];

#ifdef FEATURE_TCXOMGR_MSIM_RPUSH_API
  tcxomgr_rpush_mode_cb_registration_type gl1_hw_mode_cb_dereg_info;
#endif

#ifdef FEATURE_TCXOMGR_MSIM_RPUSH_API
  gl1_hw_mode_cb_dereg_info.client_info.id    = TCXOMGR_CLIENT_GSM;
  gl1_hw_mode_cb_dereg_info.client_info.as_id = gl1_hw_map_gas_id_to_tcxo_asid(gas_id);
  gl1_hw_mode_cb_dereg_info.func_ptr          = NULL;
#endif /* FEATURE_TCXOMGR_MSIM_RPUSH_API */

  /* Refresh the rpush freq error before the final rpush
   * Ensure this is only done if we have valid accum. samples.
   */
  if( 0 != gl1_hw_cgps_feu_ptr->acc_samples )
  {
    gl1_hw_set_rpush_freq_err_in_ppm( gas_id );

    /*
     * Force a final Rpush on release to avoid pending push
     * and also for Multi-SIM will mean update sent when requested
     * during TCXO release as well as early sleep etc.
     */
    gl1_hw_cgps_feu_push( gas_id );
  }

  /* NULL callbacks on release as next SUB will always reinstall */
  if ( gl1_hw_tcxo_data[gas_id].callbacks_registered )
  {
#ifndef FEATURE_TCXOMGR_MSIM_RPUSH_API
    /* Unregister GSM TCXO Mgr callbacks to avoid unwanted printouts etc. */
    tcxomgr_set_callbacks( TCXOMGR_CLIENT_GSM,
                           NULL, NULL, NULL, NULL,
                           NULL, NULL, NULL, NULL,
                           NULL );
#else
    tcxomgr_register_rpush_mode_cb( gl1_hw_mode_cb_dereg_info );
#endif /* !FEATURE_TCXOMGR_MSIM_RPUSH_API */

    gl1_hw_tcxo_data[gas_id].callbacks_registered = FALSE;
  }

  MSG_GERAN_LOW_0_G( GL1_TCXO_HDR"tcxo_release" );
}

/*===========================================================================
FUNCTION     GL1_HW_REQUEST_TCXO_MGR_3

DESCRIPTION
  This function requests TCXO Manager 3 before GSM can
  attempt to acquire.

DEPENDENCIES
  None.

PARAMETERS
  ( NOTE : Active task can be GSM of FTM )
  rex_tcb_type  *                   - Active task tcb
  void         (*task_wait_handler) - Active task handler
  rex_sigs_type                     - Active task wait signal
  rex_sigs_type                     - Active task complete sig

RETURN VALUE
  boolean - FALSE if TCXO Mgr 3.0 request times out.

SIDE EFFECTS
  None.
===========================================================================*/
void gl1_hw_request_tcxo_mgr_3( rex_tcb_type  *task_ptr,
                                void (*task_wait_handler)(rex_sigs_type, gas_id_t),
                                rex_sigs_type  task_wait_sig,
                                rex_sigs_type  task_done_sig,
                                gas_id_t       gas_id )
{
  /*TCXO Mode can either be High/Low/Off. Whenever the mode changes, the mode callback will called
    utilizing the tcxomgr_rpush_mode_cb_registration_type information that we are registering in this function*/
#ifdef FEATURE_TCXOMGR_MSIM_RPUSH_API
  tcxomgr_rpush_mode_cb_registration_type gl1_hw_mode_cb_reg_info;

  gl1_hw_mode_cb_reg_info.client_info.id    = TCXOMGR_CLIENT_GSM;
  gl1_hw_mode_cb_reg_info.client_info.as_id = gl1_hw_map_gas_id_to_tcxo_asid(gas_id);
  gl1_hw_mode_cb_reg_info.func_ptr          = gl1_hw_tcxo_rpush_mode_cb;
#endif /* FEATURE_TCXOMGR_MSIM_RPUSH_API */

  /* Enable GPS Rpush enable callbacks whenever SUB goes active */
  if ( !gl1_hw_tcxo_data[gas_id].callbacks_registered )
  {
#ifndef FEATURE_TCXOMGR_MSIM_RPUSH_API
    tcxomgr_set_callbacks( TCXOMGR_CLIENT_GSM,
                           NULL, NULL, NULL,
                           NULL, NULL, NULL,
                           gl1_hw_tcxo_rpush_flag_cb,
                           NULL, NULL );
#else
    MSG_GERAN_LOW_0_G( GL1_TCXO_HDR"Registering tcxomgr rpush mode cb" );

    tcxomgr_register_rpush_mode_cb( gl1_hw_mode_cb_reg_info );
#endif /* !FEATURE_TCXOMGR_MSIM_RPUSH_API */

    gl1_hw_tcxo_data[gas_id].callbacks_registered = TRUE;
  }

  MSG_GERAN_LOW_0_G( GL1_TCXO_HDR"explicit grant for XO" );

  return;
}

/*===========================================================================
FUNCTION     GL1_HW_MAP_GAS_ID_TO_TCXO_ASID

DESCRIPTION
  Set the correct TCXO asid for the chosen gas_id.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  TCXO asid

SIDE EFFECTS
  None.
===========================================================================*/
tcxomgr_as_id_enum_type gl1_hw_map_gas_id_to_tcxo_asid( gas_id_t gas_id )
{
  tcxomgr_as_id_enum_type tcxo_asid = TCXOMGR_AS_ID_NONE;
  sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id( gas_id );

  switch ( as_id )
  {
    case SYS_MODEM_AS_ID_1:
      tcxo_asid =  TCXOMGR_AS_ID_1;
    break;

    case SYS_MODEM_AS_ID_2:
      tcxo_asid =  TCXOMGR_AS_ID_2;
    break;

#if defined ( FEATURE_TRIPLE_SIM )
    case SYS_MODEM_AS_ID_3:
      tcxo_asid =  TCXOMGR_AS_ID_3;
    break;
#endif /* FEATURE_TRIPLE_SIM */

    default:
      MSG_GERAN_ERROR_2_G( "Invalid gas_id %d as_id %d",
                           gas_id, as_id );
    break;
  }

  return ( tcxo_asid );
}

/*===========================================================================
FUNCTION     GL1_HW_MAP_TCXO_ASID_TO_GAS_ID

DESCRIPTION
  Set the correct gas_id from the TCXO asid.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  gas_id

SIDE EFFECTS
  None.
===========================================================================*/
gas_id_t gl1_hw_map_tcxo_asid_to_gas_id( tcxomgr_as_id_enum_type tcxo_asid )
{
  sys_modem_as_id_e_type as_id = SYS_MODEM_AS_ID_NONE;

  switch ( tcxo_asid )
  {
    case TCXOMGR_AS_ID_1:
      as_id =  SYS_MODEM_AS_ID_1;
    break;

    case TCXOMGR_AS_ID_2:
      as_id =  SYS_MODEM_AS_ID_2;
    break;

#if defined ( FEATURE_TRIPLE_SIM )
    case TCXOMGR_AS_ID_3:
      as_id =  SYS_MODEM_AS_ID_3;
    break;
#endif /* FEATURE_TRIPLE_SIM */

    case TCXOMGR_AS_ID_NONE:
      as_id = SYS_MODEM_AS_ID_NONE;
    break;

    default:
      MSG_GERAN_ERROR_1( "Invalid tcxo_id:%d",
                         tcxo_asid );
    break;
  }

  return ( geran_map_nas_id_to_gas_id(as_id) );
}

/*===========================================================================
FUNCTION     GL1_HW_UPDATE_RAW_FE_TCXO_MGR

DESCRIPTION
  This function passes the Raw Accumulated Freq Err and Number of samples
  for the Rotator Push.

DEPENDENCIES
  None.

PARAMETERS
  int32 - Accumulated FError.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void gl1_hw_update_raw_fe_tcxo_mgr( int32 freq_err, gas_id_t gas_id )
{
  gl1_hw_cgps_feu_t *gl1_hw_cgps_feu_ptr = &gl1_hw_cgps_feu[gas_id];

  /* Check for rpush enable valid for system config */
  if ( !gl1_hw_is_rpush_possible( gas_id ) )
  {
    return;
  }

  /* Ensure only happens on valid first sample */
  if ( !gl1_hw_cgps_feu_ptr->first_sample_slow_clk_taken )
  {
    /* First update sample so store slow clock */
    gl1_hw_cgps_feu_ptr->first_sample_slow_clk       = timetick_get_safe();
    gl1_hw_cgps_feu_ptr->last_sample_slow_clk        = gl1_hw_cgps_feu_ptr->first_sample_slow_clk;
    gl1_hw_cgps_feu_ptr->first_sample_slow_clk_taken = TRUE;
  }
  else
  {
    /* Keep updating last sample time with slow clock value so valid when sample finally sent */
    gl1_hw_cgps_feu_ptr->last_sample_slow_clk        = timetick_get_safe();
  }

  gl1_hw_cgps_feu_ptr->acc_samples++;

  /* Capture freq err for later rpush execution */
  gl1_hw_set_rpush_freq_err_in_ppm( gas_id );

  MSG_GERAN_LOW_3_G( GL1_FEU_HDR"gl1_hw_update_cgps_feu freq_err:%d acc_samples:%d freq_err_ppm:%d",
                     freq_err, gl1_hw_cgps_feu_ptr->acc_samples,
                     gl1_hw_get_rpush_freq_err_in_ppm(gas_id) );

  MSG_GERAN_LOW_2_G( GL1_FEU_HDR"gl1_hw_update_cgps_feu first_clock:%d last_clock:%d",
                     gl1_hw_cgps_feu_ptr->first_sample_slow_clk,
                     gl1_hw_cgps_feu_ptr->last_sample_slow_clk );
}

/*===========================================================================
FUNCTION     GL1_HW_TCXO_RPUSH

DESCRIPTION
  This function passes the Freq Err for GPS to use.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void gl1_hw_tcxo_rpush( gas_id_t gas_id )
{
  gl1_hw_cgps_feu_t *gl1_hw_cgps_feu_ptr = &gl1_hw_cgps_feu[gas_id];

  /* Check for rpush enable valid for system config */
  if ( !gl1_hw_is_rpush_possible( gas_id ) )
  {
    return;
  }

  MSG_GERAN_LOW_2_G( GL1_FEU_HDR"gl1_hw_tcxo_rpush Samples:%d State:%d",
                     gl1_hw_cgps_feu_ptr->acc_samples, gl1_hw_cgps_feu_ptr->tcxo_state );

  if ( !gl1_hw_get_cgps_feu_rpush_enable( gas_id ) )
  {
    MSG_GERAN_LOW_1_G( GL1_FEU_HDR"Rpush not enabled rpush_enable:%d",
                       gl1_hw_get_cgps_feu_rpush_enable( gas_id ) );

    /* Reset data so if/when enabled sample info will be fresh */
    gl1_hw_cgps_feu_ptr->rpush_sent = TRUE;
  }
  /* Only send if we have taken a sample in the required period and rpush enabled */
  else if ( gl1_hw_cgps_feu_ptr->acc_samples )
  {
#ifdef FEATURE_TCXOMGR_MSIM_RPUSH_API
    tcxomgr_rpush_data_type rpush_data;
#endif /* FEATURE_TCXOMGR_MSIM_RPUSH_API */

    int32 freq_err = gl1_hw_get_rpush_freq_err_in_ppm(gas_id);

#if defined  FEATURE_TCXOMGR_NEGATE_FREQ_ERROR
    freq_err *= ( -1 );
#endif /* FEATURE_TCXOMGR_NEGATE_FREQ_ERROR | FEATURE_UMTS_NEGATE_ALL_TCXOMGR_VALUES */

    /* Finally push the update */
#ifndef FEATURE_TCXOMGR_MSIM_RPUSH_API
    tcxomgr_rpush( TCXOMGR_CLIENT_GSM, freq_err, gl1_hw_cgps_feu_ptr->acc_samples,
                   gl1_hw_cgps_feu_ptr->first_sample_slow_clk,
                   gl1_hw_cgps_feu_ptr->last_sample_slow_clk,
                   gl1_hw_cgps_feu_ptr->tcxo_state );
#else
    rpush_data.client_info.id        = TCXOMGR_CLIENT_GSM;
    rpush_data.client_info.as_id     = gl1_hw_map_gas_id_to_tcxo_asid(gas_id);
    rpush_data.rot_val               = freq_err;
    rpush_data.num_samples           = gl1_hw_cgps_feu_ptr->acc_samples;
    rpush_data.begin_acq_time        = gl1_hw_cgps_feu_ptr->first_sample_slow_clk;
    rpush_data.end_acq_time          = gl1_hw_cgps_feu_ptr->last_sample_slow_clk;
    rpush_data.state                 = gl1_hw_cgps_feu_ptr->tcxo_state;

    tcxomgr_rotator_push( rpush_data );
#endif /* !FEATURE_TCXOMGR_MSIM_RPUSH_API */

    /* Reset stored data for next sample set */
    gl1_hw_cgps_feu_ptr->rpush_sent = TRUE;

    MSG_GERAN_HIGH_2_G( GL1_FEU_HDR"FreqErrPPM:%d State:%d",
                        freq_err, gl1_hw_cgps_feu_ptr->tcxo_state );
  }
  else
  {
    MSG_GERAN_LOW_1_G( GL1_FEU_HDR"No samples to push ! State:%d",
                       gl1_hw_cgps_feu_ptr->tcxo_state );

    gl1_hw_cgps_feu_ptr->rpush_sent = FALSE;
  }

  /* Only reset if rpush sent out otherwise wait for next push before starting again */
  if ( gl1_hw_cgps_feu_ptr->rpush_sent ||
       ( gl1_hw_cgps_feu_ptr->frame_ticks >= gl1_hw_cgps_feu_ptr->rpush_frame_thresh ) )
  {
    /* Clear stored data as sent or long averaging period reached */
    gl1_hw_reset_cgps_feu( FALSE, gas_id );
  }
}

/*===========================================================================
FUNCTION     GL1_HW_RESET_CGPS_FEU

DESCRIPTION
  This function resets the Freq Err control structure for GPS to use.

DEPENDENCIES
  None.

PARAMETERS
  boolean reset_state - whether to set state to not known.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void gl1_hw_reset_cgps_feu( boolean reset_state, gas_id_t gas_id )
{
  /* Check for rpush enable valid for system config */
  if ( !gl1_hw_is_rpush_possible( gas_id ) )
  {
    return;
  }

  /* No rpush possible check to avoid circular dependency */
  gl1_hw_internal_reset_cgps_feu( reset_state, gas_id );
}

/*===========================================================================
FUNCTION     GL1_HW_INTERNAL_RESET_CGPS_FEU

DESCRIPTION
  This function resets the Freq Err control structure for GPS to use.
  This function is safe to use in gl1_hw_is_rpush_possible as no circular
  dependency on the function callstack.

DEPENDENCIES
  None.

PARAMETERS
  boolean reset_state - whether to set state to not known.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void gl1_hw_internal_reset_cgps_feu( boolean  reset_state,
                                            gas_id_t gas_id )
{
  gl1_hw_cgps_feu_t *gl1_hw_cgps_feu_ptr = &gl1_hw_cgps_feu[gas_id];

  MSG_GERAN_LOW_1_G( GL1_FEU_HDR"gl1_hw_internal_reset_cgps_feu reset_state:%d",
                     reset_state );

  gl1_hw_cgps_feu_ptr->frame_ticks                 = 0;
  gl1_hw_cgps_feu_ptr->rpush_frame_thresh          = gl1_hw_get_rpush_update_interval( gas_id );

  gl1_hw_cgps_feu_ptr->acc_samples                 = 0;

  gl1_hw_cgps_feu_ptr->first_sample_slow_clk_taken = FALSE;
  gl1_hw_cgps_feu_ptr->first_sample_slow_clk       = 0;

  gl1_hw_cgps_feu_ptr->last_sample_slow_clk        = 0;

  gl1_hw_cgps_feu_ptr->force_rpush                 = FALSE;
  gl1_hw_cgps_feu_ptr->rpush_sent                  = FALSE;

  /* Typically only done at startup */
  if ( reset_state )
  {
    gl1_hw_cgps_feu_ptr->tcxo_state                = TCXOMGR_STATE_NO_INFO;
  }
}

/*===========================================================================
FUNCTION     GL1_HW_CGPS_FEU_TICK

DESCRIPTION
  The frame tick handler for the feu updates.

DEPENDENCIES
  None.

PARAMETERS
  Active subs gas_id.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void gl1_hw_cgps_feu_tick( gas_id_t gas_id )
{
  /* Check for rpush enable valid for system config */
  if ( !gl1_hw_is_rpush_possible( gas_id ) )
  {
    return;
  }

  gl1_hw_cgps_feu[gas_id].frame_ticks++;

  MSG_GERAN_LOW_1_G( GL1_FEU_HDR"feu_tick frames:%d",
                     gl1_hw_cgps_feu[gas_id].frame_ticks );

  /* Now push the value */
  gl1_hw_cgps_feu_push( gas_id );
}

/*===========================================================================
FUNCTION     GL1_HW_CGPS_FEU_PUSH

DESCRIPTION
  Pushes the rpush estimate via tick or direct call.

DEPENDENCIES
  None.

PARAMETERS
  Active subs gas_id.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void gl1_hw_cgps_feu_push( gas_id_t gas_id )
{
  gl1_hw_cgps_feu_t *gl1_hw_cgps_feu_ptr = &gl1_hw_cgps_feu[gas_id];

  /* Check for rpush enable valid for system config */
  if ( !gl1_hw_is_rpush_possible( gas_id ) )
  {
    return;
  }

  /* Only fire off frequency error if timeout reached or force for page read/startup etc */
  if ( ( gl1_hw_cgps_feu_ptr->frame_ticks >= gl1_hw_cgps_feu_ptr->rpush_frame_thresh ) ||
       gl1_hw_cgps_feu_ptr->force_rpush )
  {
    MSG_GERAN_LOW_3_G( GL1_FEU_HDR"feu_tick frames:%d force_rpush:%d threshold:%d",
                       gl1_hw_cgps_feu_ptr->frame_ticks, gl1_hw_cgps_feu_ptr->force_rpush,
                       gl1_hw_cgps_feu_ptr->rpush_frame_thresh );

    /* Now push the value */
    gl1_hw_tcxo_rpush( gas_id );
  }
}

/*===========================================================================
FUNCTION     GL1_HW_GET_CGPS_FEU_STATE

DESCRIPTION
  If current state is idle then return TRUE, otherwise FALSE but may need
  changing as we also have the NO_INFO state to worry about after a data reset.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  boolean - TRUE if IDLE or no info/FALSE otherwise.

SIDE EFFECTS
  None.
===========================================================================*/
boolean gl1_hw_get_cgps_feu_state( gas_id_t gas_id )
{
  boolean is_idle = TRUE;

  /* Treat NO_INFO the same as IDLE */
  if ( TCXOMGR_STATE_TRAFFIC == gl1_hw_cgps_feu[gas_id].tcxo_state )
  {
    is_idle = FALSE;
  }

  MSG_GERAN_LOW_2_G( GL1_FEU_HDR"Get FEU state is_idle:%d state:%d",
                     is_idle, gl1_hw_cgps_feu[gas_id].tcxo_state );

  return ( is_idle );
}

/*===========================================================================
FUNCTION     GL1_HW_SET_CGPS_FEU_FORCE_RPUSH

DESCRIPTION
  Set the local rpush flag to force a send, should be used after page read
  for sleep case and DRX, when the state changes from idle->traffic or vice versa
  and at first transition to enabling rpush.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void gl1_hw_set_cgps_feu_force_rpush( gas_id_t gas_id )
{
  /* Check for rpush enable valid for system config */
  if ( !gl1_hw_cgps_feu[gas_id].force_rpush &&
       gl1_hw_is_rpush_possible( gas_id ) )
  {
    gl1_hw_cgps_feu[gas_id].force_rpush = TRUE;

    MSG_GERAN_LOW_0_G( GL1_FEU_HDR"Set FEU force_rpush TRUE" );
  }
}

/*===========================================================================
FUNCTION     GL1_HW_SET_CGPS_FEU_STATE

DESCRIPTION
  If current state is idle then return TRUE, otherwise FALSE but may need
  changing as we also have the NO_INFO state to worry about after a data reset.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  boolean - TRUE if IDLE/FALSE otherwise.

SIDE EFFECTS
  None.
===========================================================================*/
void gl1_hw_set_cgps_feu_state( boolean idle_mode, gas_id_t gas_id )
{
  /* Check for rpush enable valid for system config */
  if ( !gl1_hw_is_rpush_possible( gas_id ) )
  {
    return;
  }

  if ( idle_mode )
  {
    gl1_hw_cgps_feu[gas_id].tcxo_state = TCXOMGR_STATE_IDLE;
  }
  else
  {
    gl1_hw_cgps_feu[gas_id].tcxo_state = TCXOMGR_STATE_TRAFFIC;
  }

  MSG_GERAN_LOW_1_G( GL1_FEU_HDR"Set FEU state:%d",
                     gl1_hw_cgps_feu[gas_id].tcxo_state );
}

/*===========================================================================
FUNCTION     GL1_HW_SET_CGPS_FEU_SAMPLE_THRESHOLD

DESCRIPTION
  Set the local sample threshold to force a send. Used to force a
  single sample at startup.

DEPENDENCIES
  None.

PARAMETERS
  int16 - How many samples to take before sending.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void gl1_hw_set_cgps_feu_sample_threshold( int16 rpush_frame_thresh, gas_id_t gas_id )
{
  gl1_hw_cgps_feu[gas_id].rpush_frame_thresh = rpush_frame_thresh;

  MSG_GERAN_LOW_1_G( GL1_FEU_HDR"Set FEU rpush_frame_thresh:%d",
                   rpush_frame_thresh );
}

/*===========================================================================
FUNCTION     GL1_HW_SET_RPUSH_UPDATE_INTERVAL

DESCRIPTION
  Set the local update interval for the RPUSH info by converting the
  tcxomanager supplied interval in msecs to the local GSM frame count.

DEPENDENCIES
  Need tcxomanager to provide update interval in msecs.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void gl1_hw_set_rpush_update_interval( gas_id_t gas_id )
{
  gl1_hw_tcxo_data_t *gl1_hw_tcxo_data_ptr = &gl1_hw_tcxo_data[gas_id];

#ifdef FEATURE_TCXOMGR_DYN_RPUSH
  uint16 rpush_interval_msecs = tcxomgr_get_rpush_update_interval();

  /* Sanity if interval of zero msecs returned then use default period */
  if ( rpush_interval_msecs )
  {
    /* Convert from msecs to GSM frames with round down */
    gl1_hw_tcxo_data_ptr->rpush_update_interval = ( ( 13 * rpush_interval_msecs ) / 60 );

    /* Final check if interval too short */
    if ( !gl1_hw_tcxo_data_ptr->rpush_update_interval )
    {
      gl1_hw_tcxo_data_ptr->rpush_update_interval = CGPS_FEU_RPUSH_SLOW_SEND_TICKS;
    }

    MSG_GERAN_LOW_2_G( GL1_FEU_HDR"Set rpush update interval:%d msecs %d GSM frames",
                      rpush_interval_msecs, gl1_hw_tcxo_data_ptr->rpush_update_interval );
  }
  else
#endif /* FEATURE_TCXOMGR_DYN_RPUSH */
  {
    gl1_hw_tcxo_data_ptr->rpush_update_interval = CGPS_FEU_RPUSH_SLOW_SEND_TICKS;

    MSG_GERAN_LOW_1_G( GL1_FEU_HDR"Set rpush update interval:%d GSM frames",
                     gl1_hw_tcxo_data_ptr->rpush_update_interval );
  }
}

/*===========================================================================
FUNCTION     GL1_HW_GET_RPUSH_UPDATE_INTERVAL

DESCRIPTION
  Return the active rpush update interval in GSM frames.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  Active rpush reporting interval in GSM frames.

SIDE EFFECTS
  None.
===========================================================================*/
static uint16 gl1_hw_get_rpush_update_interval( gas_id_t gas_id )
{
  return ( gl1_hw_tcxo_data[gas_id].rpush_update_interval );
}

/*===========================================================================
FUNCTION     GL1_HW_SET_CGPS_FEU_RPUSH_ENABLE

DESCRIPTION
  Set the local rpush enable flag for status checking.

DEPENDENCIES
  None.

PARAMETERS
  boolean rpush_enable - local status flag set from cb.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void gl1_hw_set_cgps_feu_rpush_enable( boolean  rpush_enable, gas_id_t gas_id )
{
  gl1_hw_tcxo_data[gas_id].feu_rpush_enable = rpush_enable;

  MSG_GERAN_LOW_1_G( GL1_FEU_HDR"rpush_enable:%d",
                     rpush_enable );
}

/*===========================================================================
FUNCTION     GL1_HW_GET_CGPS_FEU_RPUSH_ENABLE

DESCRIPTION
  Set the local rpush enable flag for status checking.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  boolean - current rpush enabled status.

SIDE EFFECTS
  None.
===========================================================================*/
static boolean gl1_hw_get_cgps_feu_rpush_enable( gas_id_t gas_id )
{
  return ( gl1_hw_tcxo_data[gas_id].feu_rpush_enable );
}

/*===========================================================================

FUNCTION gl1_hw_get_rpush_freq_err_in_ppm

DESCRIPTION
  Returns the rpush freq value in ppm for TXC0 Mgr 3.0.

DEPENDENCIES
  None

PARAMETERS
  None.

RETURN VALUE
  int32 - Active scell/dedicated Rot/XO freq err in ppm value

SIDE EFFECTS
  None
===========================================================================*/
static int32 gl1_hw_get_rpush_freq_err_in_ppm( gas_id_t gas_id )
{
  return ( gl1_hw_tcxo_data[gas_id].rpush_freq_err_in_ppm );
}

/*===========================================================================
FUNCTION gl1_hw_set_rpush_freq_err_in_ppm

DESCRIPTION
  This function sets the software copy of the accumulated frequency error.
  Used when the mdsp has done AFC and returned the upper layers the new value.
  This will ensure we always only capture actual serving/dedicated cell info.

DEPENDENCIES
  Sets local static gl1_hw_rpush_freq_err_in_ppm.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void gl1_hw_set_rpush_freq_err_in_ppm( gas_id_t gas_id )
{
  /* Check for rpush enable valid for system config */
  if ( !gl1_hw_is_rpush_possible( gas_id ) )
  {
    return;
  }

  gl1_hw_tcxo_data[gas_id].rpush_freq_err_in_ppm =
    gl1_hw_get_xo_acc_freq_err_in_ppm( gas_id );
}


/*===========================================================================
FUNCTION gl1_hw_is_rpush_possible

DESCRIPTION
  This function decides if rpush is valid for the current system config.
  It also allows for the gas_id to be changed in which case a data flush
  will occur as only valid SUB can ever be sending RGS updates.
  Rules are with more than one active G SUB only SUB0 can update (as always MMODE),
  otherwise with only one active G SUB then that is allowed to send Rpush estimates.
  For non-Multi-SIM then Rpush is always valid if enabled.
  New rule is that all active G SUB will send Rpush.

DEPENDENCIES
  None.

PARAMETERS
  Active gas_id to check rpush validity for.

RETURN VALUE
  TRUE if Rpush valid for that SUB depending on System config.

SIDE EFFECTS
  None.
===========================================================================*/
static boolean gl1_hw_is_rpush_possible( gas_id_t gas_id )
{
  boolean rpush_possible = FALSE;
#ifndef FEATURE_TCXOMGR_MSIM_RPUSH_API
  uint8   num_idle_subs  = gl1_get_num_idle_subs();
#endif

#ifdef FEATURE_TCXOMGR_MSIM_RPUSH_API
  /* Need to be camped */
  rpush_possible = l1_is_cell_acq_complete( gas_id );
#else
  /* Need at least one valid camped SUB */
  if ( num_idle_subs )
  {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    if ( gl1_msg_get_multi_sim_mode() )
    {
      /* Multi-SIM but only one active idle SUB */
      if ( num_idle_subs == 1 )
      {
        rpush_possible = TRUE;
      }
      /*
       * Only ever enable for Multi-MODE capable SUB
       * when more than one idle SUB available
       */
      else if ( gas_id == GERAN_ACCESS_STRATUM_ID_1 )
      {
        rpush_possible = TRUE;
      }
    }
    else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
    /* Not Multi-SIM and one idle SUB then allow Rpush */
    {
       rpush_possible = TRUE;
    }
  }


#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  MSG_GERAN_LOW_3_G( GL1_FEU_HDR"rpush poss:%d num_idle_subs:%d msim:%d",
                     rpush_possible, num_idle_subs, gl1_msg_get_multi_sim_mode() );
#else
  MSG_GERAN_LOW_2_G( GL1_FEU_HDR"rpush poss:%d num_idle_subs:%d",
                     rpush_possible, num_idle_subs );
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
#endif /* FEATURE_TCXOMGR_MSIM_RPUSH_API */

  return ( rpush_possible );
}

/*===========================================================================
FUNCTION gl1_hw_get_initial_rgs

DESCRIPTION
  This function sets the starting RGS estimate for the particular SUB
  based on whether Multi-SIM mode is active or not.

DEPENDENCIES
  None.

PARAMETERS
  Active gas_id.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
tcxomgr_vco_info_type gl1_hw_get_initial_rgs( gas_id_t gas_id )
{
  tcxomgr_vco_info_type vco_info;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  /* for multisim always use new iRAT as-id based API */
  vco_info =
    tcxomgr_get_subscription_rgs( gl1_hw_map_gas_id_to_tcxo_asid( gas_id ) );
#else
  vco_info = tcxomgr_get_stored_vco();
#endif

#if defined  FEATURE_TCXOMGR_NEGATE_FREQ_ERROR
  vco_info.rot_value *= ( -1 );
#endif

  return ( vco_info );
}

/*===========================================================================
FUNCTION gl1_hw_update_xo_rgs

DESCRIPTION
  This function sends the XO RGS value to XO manager.

DEPENDENCIES
  None.

PARAMETERS
  Active current active XO value, GL1 state info and gas_id.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void gl1_hw_update_xo_rgs( int16                     xo_freq_error,
                           tcxomgr_client_state_type state,
                           gas_id_t                  gas_id )
{
   tcxomgr_rgs_info_type rgs_update_data;

#if defined  FEATURE_TCXOMGR_NEGATE_FREQ_ERROR
   xo_freq_error *= ( -1 );
#endif /* FEATURE_TCXOMGR_NEGATE_FREQ_ERROR */

   rgs_update_data.valid_fields  =
     ( TCXOMGR_RGS_FIELD_ID + TCXOMGR_RGS_FIELD_VCO + TCXOMGR_RGS_FIELD_ROT + \
       TCXOMGR_RGS_FIELD_STATE + TCXOMGR_RGS_FIELD_AS_ID );

   rgs_update_data.id            = TCXOMGR_CLIENT_GSM;
   rgs_update_data.vco_val       = 0;
   rgs_update_data.rot_val       = xo_freq_error;

   rgs_update_data.state         = state;

   rgs_update_data.as_id         = gl1_hw_map_gas_id_to_tcxo_asid( gas_id );

   tcxomgr_update_rgs_values( &rgs_update_data );
}

/* END OF FILE */
