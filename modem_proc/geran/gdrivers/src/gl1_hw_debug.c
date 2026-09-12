/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                G L 1  F R A M E   L A Y E R

GENERAL DESCRIPTION
   This file provides general GL1 debug functionality.
   Originally derived from gl1_hw.c whic is now splitup.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2008-2015 Qualcomm Technologies, Inc.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_hw_debug.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
18/06/13   cs       Major Triton TSTS Syncup
10/04/13   ap       472264 DSDS: Fix the gas_id problems when the IQ logging is enabled
14/03/12   pg       Add support for QuRT
14/04/11   tjw      Full featurisation before using mdsp_get_iq_data(gas_id)
30/03/11   cja      IRAT for Nikel
08/02/11   cja      Initial changes for Nikel
11/01/11   cgc      Fix compiler warning add cast #200
02/08/10   tjw      Removed unused clk.h (CMI phase 1 re-clean up)
02/07/10    ws      Support for IQ logging on QDSP6
14/01/10    pg      Adding support for BLAST OS
05/05/09    cs      Mux in the IQ logged GPRS timeslot with the TSC
01/12/08    cs      New file based on code in original gl1_hw.c

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "geran_variation.h"
#include "comdef.h"

#ifdef CUST_H
#include "customer.h"
#endif


#include "gtmrs.h"
#include "gtmrs_g.h"          /* GSM timer services */
#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "gl1_hw_debug.h"
#include "mdsp_intf.h"
#include "mdsp_intf_g.h"

#include "msg.h"            /* Diagnostics MSGs */
#include "err.h"            /* ERR_FATAL call   */

#include "rex.h"

#include "geran_dual_sim.h"
#include "geran_multi_sim.h"


#ifdef FEATURE_GSM_LOG_IQ_SAMPLES
#include "gpl1_gprs_log.h"
#endif /* FEATURE_GSM_LOG_IQ_SAMPLES */

/*===========================================================================

                     DEFINITIONS FOR MODULE

===========================================================================*/

/*===========================================================================

 LOCAL STORAGE

===========================================================================*/
#if  defined (FEATURE_GSM_LOG_IQ_SAMPLES)
static uint8 gl1_hw_iq_log_tn[ NUM_GERAN_DATA_SPACES ];
#endif
/*===========================================================================

                           FUNCTION PROTOTYPES

===========================================================================*/

#if  defined (FEATURE_GSM_LOG_IQ_SAMPLES)
/* MAX_NUM_IQ_SAMPLES defined in gfw_sw_intf.h
 * Add 1 to size to account for number of IQ samples
 */
#define MDSP_IQ_SAMPLES_MEM_SIZE ( MAX_NUM_IQ_SAMPLES + 1 )
#define IQ_HDR "IQ LOG: "

/* Structure containing a block of IQ samples (4 frames)
   - locked: set to true when this block is being written to flash
   - need_to_sync: set to true after we have written the samples to flash
     to indicate that the data is stale
*/
typedef struct
{
   uint32  FN;
   uint16  TSC;
   uint16  channel;
   boolean locked;
   boolean need_to_sync;
   uint16  samples[8][MDSP_IQ_SAMPLES_MEM_SIZE];
} gl1_hw_iq_samples_block_type;

#define NUM_IQ_MSG_BUFFS 3

/* Control structure for a buffer of IQ samples.
   - enabled: indicates if I/Q sample saving is enabled or not
   - busy: set to true when we are writing to the file to prevent other CRC errors
     from causing a collision
   - msg: actual samples - first two buffers are a ping pong buffer for back to
     back xCCH and the third for SACCH/26 blocks
 */
typedef struct
{
   boolean                      enabled[GL1_HW_NUM_CHANNEL_TYPES];
   uint16                       radio_blocks[GL1_HW_NUM_CHANNEL_TYPES];
   boolean                      busy;
   gl1_hw_iq_samples_block_type msg[NUM_IQ_MSG_BUFFS];
} gl1_hw_iq_samples_struct;

/* Used to set the desired logging ts, can be modified globally */
static boolean gl1_hw_iq_init_once[ NUM_GERAN_DATA_SPACES ] = { INITIAL_VALUE( FALSE ) };

/* Used to set the desired logging ts, can be modified globally */
uint16 gl1_hw_gprs_iq_slot[ NUM_GERAN_DATA_SPACES ] = { INITIAL_VALUE( 1 ) };

/* Saved IQ samples NOTE: This is very big so only enable if required */
static gl1_hw_iq_samples_struct gl1_hw_iq_samples[ NUM_GERAN_DATA_SPACES ];

/* Local prototypes */
static void gl1_hw_iq_mark_buffer_stale( gas_id_t gas_id );

static void gl1_hw_reset_iq_radio_block_cnt( gas_id_t gas_id );
static void gl1_hw_set_iq_radio_block_cnt( gl1_hw_channel_type channel
                                           gas_id_t            gas_id );

#endif /* FEATURE_GSM_SAVE_IQ_SAMPLES | FEATURE_GSM_LOG_IQ_SAMPLES */

#if defined ( FEATURE_GSM_LOG_IQ_SAMPLES )
static void gl1_hw_log_iq_samples( gl1_hw_channel_type channel, uint32 frm_index,
                                   uint32 msg_index, gas_id_t gas_id );

static void gl1_hw_log_iq_samples_dpc( uint32 channel_and_index );
#endif /* FEATURE_GSM_LOG_IQ_SAMPLES */



/*===========================================================================

                                FUNCTIONS

===========================================================================*/

/*===========================================================================

FUNCTION gl1_hw_get_debug_data

DESCRIPTION
  This function retrieves a debug packet from the mDSP if available.

  data   - pointer to where to put decoded data results
  offset - offset (in words) into the mDSP debug buffer
  length - number of words to retrieve starting at offset

DEPENDENCIES
  The receive command must have been issued 2 frames earlier.

RETURN VALUE
  TRUE if packet available, FALSE otherwise

SIDE EFFECTS
  data will be modified even if results are FALSE.
===========================================================================*/
boolean gl1_hw_get_debug_data( uint16 *data, uint16 offset, uint16 length,
                               gas_id_t gas_id )
{
#if   defined FEATURE_GSM_LOG_IQ_SAMPLES
   return mdsp_get_iq_data( (int16*)data, length, gas_id );
#else
   return mdsp_get_debug_data( data, offset, length, gas_id );
#endif
}


#if  defined (FEATURE_GSM_LOG_IQ_SAMPLES)
/*===========================================================================

FUNCTION gl1_hw_iq_samples_init

DESCRIPTION
 Init the IQ sample saving.

===========================================================================*/
void gl1_hw_iq_samples_init( gas_id_t gas_id )
{
   /* Disable for all channel types */
   gl1_hw_channel_type i = GL1_HW_NUM_CHANNEL_TYPES;

   if ( !gl1_hw_iq_init_once[gas_id] )
   {
     while ( i-- )
     {
       gl1_hw_iq_samples[gas_id].enabled[ i ] = FALSE;
     }

     /* Reset all of the channel block counts */
     gl1_hw_reset_iq_radio_block_cnt( gas_id );

     /* All data is stale */
     gl1_hw_iq_mark_buffer_stale( gas_id );

     gl1_hw_iq_init_once[gas_id] = TRUE;
   }
}

/*===========================================================================

FUNCTION gl1_hw_iq_mark_buffer_stale

DESCRIPTION
 Mark all buffers as stale.

===========================================================================*/
static void gl1_hw_iq_mark_buffer_stale( gas_id_t gas_id )
{
   uint16 i = NUM_IQ_MSG_BUFFS;

   while ( i-- )
   {
     gl1_hw_iq_samples[gas_id].msg[i].need_to_sync = TRUE;
     gl1_hw_iq_samples[gas_id].msg[i].locked       = FALSE;
   }
}

/*===========================================================================

FUNCTION gl1_hw_iq_samples_force_enable

DESCRIPTION
  Force the enable the IQ sample saving for the given channel type.

===========================================================================*/
void gl1_hw_iq_samples_force_enable( gl1_hw_channel_type channel, boolean enable,
                                     gas_id_t            gas_id )
{
   gl1_hw_iq_samples[gas_id].enabled[ channel ] = enable;
}

/*===========================================================================

FUNCTION gl1_hw_set_iq_samples_tn

DESCRIPTION
  Sets which timeslot to save IQ samples for.

===========================================================================*/
void gl1_hw_set_iq_samples_tn( gl1_hw_channel_type channel,
                               gl1_defs_tn_type    tn, gas_id_t gas_id )
{
   /* Set the timeslot in the mDSP if this channel type is enabled */
   if ( gl1_hw_is_iq_channel_enabled( channel, gas_id ) )
   {
      gl1_hw_iq_log_tn[gas_id] = (uint8)tn;
   }
}

/*===========================================================================

FUNCTION gl1_hw_is_iq_channel_enabled

DESCRIPTION
  Returns the status if IQ logging for the given channel type.

===========================================================================*/
boolean gl1_hw_is_iq_channel_enabled( gl1_hw_channel_type channel, gas_id_t gas_id )
{
  return ( gl1_hw_iq_samples[gas_id].enabled[ channel ] );
}
/*===========================================================================

FUNCTION gl1_hw_is_iq_tn_enabled

DESCRIPTION
  Returns the status if IQ logging for the given timeslot.

===========================================================================*/
boolean gl1_hw_is_iq_tn_enabled( uint8 tn, gas_id_t gas_id )
{
  if ( tn == gl1_hw_iq_log_tn[gas_id] )
  {
     return (TRUE);
  }
  else
  {
     return (FALSE);
  }
}

/*===========================================================================

FUNCTION gl1_hw_iq_samples_enable

DESCRIPTION
  Enables the IQ sample saving for the given channel type
  and init the channel log count.

===========================================================================*/
void gl1_hw_iq_samples_enable( gl1_hw_channel_type channel, boolean enable
                               gas_id_t            gas_id )
{
   gl1_hw_iq_samples[gas_id].enabled[channel] = enable;

   /* If we are enabling, mark all data as stale */
   if ( enable )
   {
      gl1_hw_iq_mark_buffer_stale( gas_id );

      /* Reset the block counter for the chosen channel */
      gl1_hw_set_iq_radio_block_cnt( channel, gas_id );
   }
}

/*===========================================================================

FUNCTION gl1_hw_reset_iq_radio_block_cnt

DESCRIPTION
  Reset all the channel log counts.

===========================================================================*/
static void gl1_hw_reset_iq_radio_block_cnt( gas_id_t gas_id )
{
   gl1_hw_channel_type i = GL1_HW_NUM_CHANNEL_TYPES;

   while ( i-- )
   {
     gl1_hw_set_iq_radio_block_cnt( gas_id, i );
   }
}

/*===========================================================================

FUNCTION gl1_hw_set_iq_radio_block_cnt

DESCRIPTION
  Set the channel log count for the chosen channel.

===========================================================================*/
static void gl1_hw_set_iq_radio_block_cnt( gl1_hw_channel_type channel,
                                           gas_id_t            gas_id )
{
  /* Figure out how many frames to log - TCH is interleaved over 8 frames */
  switch ( channel )
  {
    case GL1_HW_TCH_FS:
    case GL1_HW_TCH_EFS:
    case GL1_HW_TCH_AFS:
#ifdef FEATURE_GSM_AMR_WB
    case GL1_HW_TCH_WFS:
#endif /* FEATURE_GSM_AMR_WB */
      gl1_hw_iq_samples[gas_id].radio_blocks[channel] = 8;
    break;

    default:
      gl1_hw_iq_samples[gas_id].radio_blocks[channel] = 4;
    break;
  }
}

/*===========================================================================

FUNCTION gl1_hw_set_iq_state

DESCRIPTION
   This sets/resets gl1_trigger_iq_stateto trigger IQ Samples or not.

===========================================================================*/
void gl1_hw_set_iq_state( boolean state, gas_id_t gas_id )
{
  gl1_hw_channel_type i = GL1_HW_NUM_CHANNEL_TYPES;

  while ( i-- )
  {
    gl1_hw_iq_samples[gas_id].enabled[i] = state;
  }

  MSG_GERAN_HIGH_1_G( IQ_HDR"I/Q Logging = %d", state );
}

/*===========================================================================

FUNCTION gl1_hw_get_iq_samples

DESCRIPTION
 Grab iq samples from the mdsp and store them in our buffer.

DEPENDENCIES
 None

RETURN VALUE
 None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_get_iq_samples( gl1_hw_channel_type channel, uint32 msg_index,
                            uint32 frm_index, uint8 TSC, gas_id_t gas_id )
{
   gl1_hw_iq_samples_struct *gl1_hw_iq_samples_p = &gl1_hw_iq_samples[gas_id];

   /* If this block is already being written then do nothing */
   if ( !gl1_hw_is_iq_channel_enabled( channel, gas_id ) ||
        gl1_hw_iq_samples_p->msg[msg_index].locked )
   {
      return;
   }

   /* Check if we are waiting for the start of a message */
   if ( gl1_hw_iq_samples_p->msg[msg_index].need_to_sync )
   {
      if ( frm_index != 0 )
      {
        return;
      }

      gl1_hw_iq_samples_p->msg[msg_index].need_to_sync = FALSE;
   }

   /* Retrieve the samples and save the frame number */
   if ( !gl1_hw_get_debug_data( gl1_hw_iq_samples_p->msg[msg_index].samples[frm_index],
                                0, MDSP_IQ_SAMPLES_MEM_SIZE, gas_id ) )
   {
      /* If the debug data was bad, give up on this block */
      gl1_hw_iq_samples_p->msg[msg_index].need_to_sync = TRUE;
   }

   MSG_GERAN_LOW_3_G( IQ_HDR"get FN %d channel %d msg_index %d",
            GSTMR_GET_FN_GERAN(gas_id), channel, msg_index );

   gl1_hw_iq_samples_p->msg[msg_index].FN      = GSTMR_GET_FN_GERAN(gas_id);

   /* Mux in the chosen GPRS IQ slot to be extracted by logging tool */
   gl1_hw_iq_samples_p->msg[msg_index].TSC     = ( ( TSC & 0xFF ) | ( ( gl1_hw_gprs_iq_slot[gas_id] & 0xFF ) << 8 ) );

   gl1_hw_iq_samples_p->msg[msg_index].channel = (uint16)channel;

   /* Need to fix the case for logging and saving as may need different status structs */
#if defined ( FEATURE_GSM_LOG_IQ_SAMPLES )
   gl1_hw_log_iq_samples( channel, frm_index, msg_index, gas_id );
#endif /* FEATURE_GSM_LOG_IQ_SAMPLES */
}
#endif /* FEATURE_GSM_SAVE_IQ_SAMPLES | FEATURE_GSM_LOG_IQ_SAMPLES */


#if defined ( FEATURE_GSM_LOG_IQ_SAMPLES )
/*===========================================================================

FUNCTION gl1_hw_log_iq_samples_dpc

DESCRIPTION
  A DPC (direct-procedure-call) function used when gl1_hw_log_iq_samples
  is called from ISR context (as it will normal be).
  L4 implements DPC differently so we need to have different functions
  depending on if we are rex based or L4 based.

===========================================================================*/
/* Macros to pack and unpack one uint16 and two uint8 parameters into a single uint32 */
#define GL1_LOG_APC_PACK( chn, frm, idx, gas_id ) \
  ( (((uint32)(chn)) << 16) | (((uint32)(frm) << 8 ) & 0xFF00 ) | \
     (((uint32)(idx) << 4 ) & 0x00F0 ) | (((uint32)(gas_id) & 0x000F ) ))
#define GL1_LOG_APC_UNPACK( val, chn, frm_idx, idx, gas_id ) \
  { (chn)     = (((uint32)(val)) >> 16); \
    (frm_idx) = ((((uint32)(val)) & 0xFF00 ) >> 8 ); \
    (idx)     = ((((uint32)(val)) & 0x00F0 ) >>  4); \
    (gas_id)  = (((uint32)(val)) & 0x000F);  }

static void gl1_hw_log_iq_samples_dpc( uint32 channel_and_index )
{
   uint32 channel, frm_index, msg_index, gas_id;

   GL1_LOG_APC_UNPACK( channel_and_index, channel, frm_index, msg_index, gas_id );

   gl1_hw_log_iq_samples( (gl1_hw_channel_type)channel, frm_index, msg_index, gas_id );
}

/*===========================================================================

FUNCTION gl1_hw_log_iq_samples

DESCRIPTION
  Dump the samples for the given message to a file.  If called from ISR
  will queue a DPC to trigger a call in task context.

===========================================================================*/
static void gl1_hw_log_iq_samples( gl1_hw_channel_type channel, uint32   frm_index,
                                   uint32 msg_index,            gas_id_t gas_id )
{
#ifdef FEATURE_GSM_PLT
  #error code not present
#endif

  gl1_hw_iq_samples_struct *gl1_hw_iq_samples_p = &gl1_hw_iq_samples[gas_id];

  /* Always clear so we don't execute unless certain conditions are met */
  boolean iq_execute = FALSE;

  if ( msg_index >= NUM_IQ_MSG_BUFFS )
  {
    MSG_GERAN_ERROR_2_G( IQ_HDR"msg_index of %d out of range max is %d",
               msg_index, ( NUM_IQ_MSG_BUFFS - 1 ));

    return;
  }

  /* DPC call so execute as decision already made */
  if ( gl1_hw_iq_samples_p->msg[msg_index].locked )
  {
    iq_execute = TRUE;
  }
  else
  {
    /*
     * As 8 block interleave we call IQ capture twice but we only need to
     * log one of these as it contains the full IQ sample set
     */
    switch ( channel )
    {
      case GL1_HW_TCH_FS:
      case GL1_HW_TCH_EFS:
      case GL1_HW_TCH_AFS:
#ifdef FEATURE_GSM_AMR_WB
      case GL1_HW_TCH_WFS:
#endif /* FEATURE_GSM_AMR_WB */
        /* Ping-pong buffer */
        if ( msg_index & 0x01 )
        {
          /* Valid so perform log process */
          iq_execute = TRUE;
        }
      break;

      default:
        /* Valid so perform log process */
        iq_execute = TRUE;
      break;
    }
  }

  /* Only perform the whole log process when we need to, coping with double call for E/A/FS */
  if ( iq_execute )
  {
    /* If we are not enabled or we are busy or need to wait for block start
       then ignore the request */
    if ( !gl1_hw_is_iq_channel_enabled( channel, gas_id ) ||
          gl1_hw_iq_samples_p->busy ||
          gl1_hw_iq_samples_p->msg[msg_index].need_to_sync
       )
    {
      MSG_GERAN_LOW_3_G( IQ_HDR"enabled %d busy %d need_to_sync %d",
               gl1_hw_is_iq_channel_enabled( channel, gas_id ), gl1_hw_iq_samples_p->busy,
               gl1_hw_iq_samples_p->msg[msg_index].need_to_sync );

     return;
    }

    /* If we are in ISR mode we use rex DPC to execute in task context */
    if ( rex_is_in_irq_mode()
#ifdef FEATURE_GSM_PLT
         #error code not present
#endif
       )
    {
       /* If we already queued this message then ignore the request */
      if ( gl1_hw_iq_samples_p->msg[msg_index].locked )
      {
        MSG_GERAN_LOW_1_G( IQ_HDR"locked %d",
                 gl1_hw_iq_samples_p->msg[msg_index].locked);

        return;
      }

      /* Queue the request to DPC */
      gl1_hw_iq_samples_p->msg[msg_index].locked = TRUE;

      if ( !rexl4_queue_dpc( gl1_hw_log_iq_samples_dpc, GL1_LOG_APC_PACK( channel, frm_index, msg_index, (uint32)gas_id ) ) )
      {
        MSG_GERAN_ERROR_1_G( IQ_HDR"Failed to queue DPC FN %d",
                   GSTMR_GET_FN_GERAN(gas_id));
      }
      else
      {
        /* Wait for the DPC to be executed when we fall back into task context */
        MSG_GERAN_LOW_1_G( IQ_HDR"DPC queued FN %d",
                 GSTMR_GET_FN_GERAN(gas_id));
      }

      return;
    }

    /* Mark us as busy until we have written the data out */
    gl1_hw_iq_samples_p->busy                  = TRUE;
    gl1_hw_iq_samples_p->msg[msg_index].locked = TRUE;

    /* New block is written out so reduce block counter */
    gl1_hw_iq_samples_p->radio_blocks[ channel ]--;

    MSG_GERAN_LOW_3_G( IQ_HDR"msg_idx %d frm_idx %d channel %d",
             msg_index, frm_index, channel );

    MSG_GERAN_LOW_3_G( IQ_HDR"Actual FN %d IQ FN %d radio_blocks %d",
             GSTMR_GET_FN_GERAN(gas_id), gl1_hw_iq_samples_p->msg[msg_index].FN, gl1_hw_iq_samples_p->radio_blocks[ channel ] );

    gpl1_log_mdsp_debug_buffer( gl1_hw_iq_samples_p->msg[msg_index].samples[frm_index],
                                gl1_hw_iq_samples_p->msg[msg_index].FN,
                                gl1_hw_iq_samples_p->msg[msg_index].TSC,
                                gl1_hw_iq_samples_p->msg[msg_index].channel, gas_id );

    /* Clean up flags depending on how many frames in a radio block */
    if ( gl1_hw_iq_samples_p->radio_blocks[ channel ] == 0 )
    {
      /* Reset block count for the next trigger */
      gl1_hw_set_iq_radio_block_cnt( channel, gas_id );

      gl1_hw_iq_samples_p->msg[msg_index].need_to_sync = TRUE;
    }

    gl1_hw_iq_samples_p->msg[msg_index].locked = FALSE;
    gl1_hw_iq_samples_p->busy                  = FALSE;
  }
}
#endif /* FEATURE_GSM_LOG_IQ_SAMPLES */


/* EOF */
