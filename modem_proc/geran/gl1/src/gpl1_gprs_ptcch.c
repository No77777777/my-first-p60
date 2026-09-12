/*=========================================================================
L 1   T R A N S F E R   P T C C H  L O G I C A L  C H A N N E L

D R I V E R

GENERAL DESCRIPTION
   This module contains PTCCH handler functions that are commonly used by all
   allocation types.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/geran.mpss/7.4.0/gl1/src/gpl1_gprs_ptcch.c#1 $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
11/05/15   pjr      CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
17/03/15   shr      CR808932: COEX support for DTM
03/10/14   aga      CR732455 Changes required to tune away framework for TH 1.0 single sim build 
09/06/13   ws       CR663438 Turn off Continuous TA when commanded by Packet Assignement
30/05/14   ws       CR657674 Data + MMS Phase 1
24/09/13   sp       CR449651: Use new log packet for GPRS Burst metrics
15/08/13   sk       CR524039 GBTA bringup changes
29/07/13   sk       CR519663 Partial QBTA bringup changes
05/03/13   pg       CR458425: correct input parameters for PTCCH metrics callback
24/08/11   kb       CR302696 - Added initialization function to initialize gprs ptcch data values
31/01/12   ab       CR 309236  GCF Test Failures-  Timing Advance index not updated.
27/07/11   ab       CR298128 Enhancement of SNR reselection in Transfer mode.
20/01/10   cs       CR221032 21.3.1 failure with SAIC enabled as AFC/TT not updating
19/01/09   nt       CR151077 - Enhancement to SNR based reselection
14/03/08   ip       Moved structures to use with PSHO
                    SNR for SAIC/ESAIC bursts.
15/01/08   ws       Added suport for ESIAC logging and SAIC AFC in PTM
31/08/07   agv      CR127468 Fix for SAIC TT handling
14/11/06   cs       CR102226 Fix the Uplink Power Logging to use unslammed slot
06/04/06   av       Feature SAIC MIMO elements added to the burst metrics.
23/11/05   og       Update tx call back routines to provide more information.
22/11/05   og       Changes to logging interfaces.
12/08/05   ws       Updated for initial DTM release
08/08/05   og       Restructuring PL1 before addition of DTM.
07/29/05   gfr      Support for NPL1 DTM interface changes
04/07/05   og       Make use of the NPL1 camp timeslot buffering scheme.
23/06/05   og       Lint error/warnings removal.
02/06/05   og       Removal of header-header includes.
01/06/05   og       Add validity flag check to the metrics callback routines.
25/04/05   ws       Replaced gl1_msg_conditional_abort_dl_ptcch()
                    gl1_msg_abort_dl_ptcch()
18/04/05   og       Initialise the instantiated usf dl tfi structure correctly.
18/03/05   og       Fix to PDAN vs TBF suspension clash handling.
21/01/05   og       Fix to dl ptcch msg metrics timeslot logging.
17/01/05   ws       Lint updates
11/10/05   og       Added/modified ptcch functional interfaces.
20/12/04   og       Update the relative timeslot of NPL1s ptcch dl handlers
                    in between dl burst reads.
30/11/04   og       Enable interference measurements when cuta is ON.
28/10/04   ws       Lint tidy up
28/10/04   ws       Added EGPRS logging for message and burst metrics
27/09/04   ws       added ptcch_bad_snr_valid flag - Fix for CR 48942
01/09/04   ws       Make sure we don't ignore a TAV in MAC_L1_POWER_CONTROL msg.
17/08/04   ws       Cleaned up code and linted
02/08/04   ws       Corrected validation of TAV to determine if HW should be updated
                    with new value ( fix for TC 42.5.5.2)
02/08/04   ws       Added ptcch_rx_cb.log = NULL in gpl1_gprs_config_ptcch()
30/07/04   rm       Removed #define macros
26/05/04   sv       Changed "l1_isrtsk_blk" structure parameters for better
                    code readability.
23/06/04   og       Egprs Qual measurements handling.
05/05/04   kf       Changes for ts slamming.
13/05/04   rm       Added more function descriptions
28/04/04   rm       Added new TA functionalities and cleaned up lint errors
19/03/04   ws       Added support for bad snr detection on PTCCH/D decodes
02/17/04   gfr      AFC cleanup
26/11/03   ws       Lint cleanup
08/09/03   ws       Added PTCCH/D logging fuction calls.
22/08/03   ws       Added check for ta being received on PTCCH are sensible
08/08/03   ws       Reduced msg levels for ptcch/d callback
17/06/03   ws       Added PTCCH/D retry as per GSM 05.10
29/01/03   ws       Added check to PTCCH/D to only set Ta between 0 and 63 bits
16/10/02   ws       Added CRC check in ptcch dl callback when setting new TA
8/08/02    ws       Corrected Lint error's and Warning's
01/08/02   ws       Corrected discarded cb protoype
                    Changed FEATURE_GPRS to FEATURE_GSM_GPRS_L1
                    Added switch around all MSG_XX
18/07/02   ws       Aligned fixed allocation storage of con. ta params with dynamic
                    allocation. Now all accessed via global_ta_params
16/07/02   ws       Corrected error with parameter type for cont. ta  in
                    gpl1_gprs_config_ptcch()
15/07/02   ws       Added support for global_ta_params for dynamic and downlink
                    allocation types.
27/05/02   ws       Added gpl1_gprs_config_ptcch() to (re)config ptcch channels
15/11/01   ws       Initial version

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
/* Alphabetical Order */
#include "geran_variation.h"
#include "customer.h"


#include "comdef.h"
#include "msg.h" // f3 messaging
#include "gl1_defs.h"
#include "gl1_defs_g.h"
#include "gl1_msg_g.h"
#include "l1_isr.h"
#include "l1_task.h"
#include "l1_utils.h" /* for gl1_get_FN() */
#include "gpl1_gprs_if.h"
#include "gpl1_gprs_isr.h"
#include "gpl1_gprs_utils.h"

#include "gpl1_gprs_log.h"

#include "gpl1_gprs_ptcch.h"
#include "gl1_msg_pdch.h"
#include "gpl1_gprs_serving_cell.h"
#include "geran_eng_mode_info.h"

#include "geran_dual_sim.h"

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/

static void gpl1_gprs_add_to_pending_ul_ta_params(
     timing_advance_params_T ta_params, gas_id_t gas_id
     );

static boolean gpl1_gprs_calc_ptcch_tn_clash(
     uint8 ptcch_tn,
     uint8 lowest_dl_timeslot,
     gas_id_t gas_id
     );


/**
 ** Lookup table for scheduling the Uplink PTCCH Logical channels. The TAI value is used to
 ** index the approriate frame number (mod 416  ) where the corresponding NPL1 functions are called.
 ** The values shown below are extracted from GSM 05.08.
 **/

static const uint16 ptcch_ul_lookup[16] = {12,38,64,90,116,142,168,194,220,246,272,298,324,350,376,402};

/**
 ** Lookup table for scheduling the Downlink PTCCH logical channel. The TAI values are used to
 ** index the frame numbers where downlink PTCCH starts.
 ** i.e. Therefore index to buffer below = TAI
 **/

static const uint16 ptcch_dl_lookup[16] = {116,116,116,116,220,220,220,220,324,324,324,324,12,12,12,12};

typedef struct
{

/**
 ** fixed bits to send in PTCCH UL if access burst length is 8 bit
 **/
uint8 gpl1_gprs_ul_ptcch_8_bit ;

/**
 ** Fixed bits to send in PTCCH UL if access burst length is 11 bit
 **/

uint8 gpl1_gprs_ul_ptcch_11_bit[2] ;
uint8 ptcch_ul_blk_start_cnt ;

uint8 ptcch_dl_blk_start_cnt ;

uint8 ptcch_dl_blocks_left ;
/* buffer for storing DL PTCCH data */
gl1_defs_rx_pkt_data_type         gpl1_gprs_dyn_ptcch_dl_data; /* buffer for storing DL PTCCH data */

/* buffer for storing DL metrics from PTCCH */
gl1_defs_rx_pkt_metrics_type    gpl1_gprs_dyn_ptcch_dl_metrics[GL1_DEFS_FRAMES_IN_BLK_PERIOD];

gl1_defs_rx_pkt_mem_type                     ptcch_dl_data_info;

gpl1_gprs_ta_T                               gpl1_gprs_ta_info;
}gpl1_gprs_ptcch_data_t;

static gpl1_gprs_ptcch_data_t  gpl1_gprs_ptcch_data[NUM_GERAN_DATA_SPACES];


/*===========================================================================

FUNCTION init_gpl1_gprs_ptcch_data

DESCRIPTION
             Initializes gprs ptcch data values.

PARAMS
             gas_id.

DEPENDENCIES

RETURN VALUE None

===========================================================================*/
void init_gpl1_gprs_ptcch_data( gas_id_t gas_id )
{
  memset ( &gpl1_gprs_ptcch_data[gas_id], 0 , sizeof ( gpl1_gprs_ptcch_data_t) );
  gpl1_gprs_ptcch_data[gas_id].gpl1_gprs_ul_ptcch_8_bit = 0x7F;
  gpl1_gprs_ptcch_data[gas_id].gpl1_gprs_ul_ptcch_11_bit[0] = 0xff;
  gpl1_gprs_ptcch_data[gas_id].gpl1_gprs_ul_ptcch_11_bit[1] = 0x7;
  gpl1_gprs_ptcch_data[gas_id].ptcch_dl_blk_start_cnt = GPL1_INVALID;
  gpl1_gprs_ptcch_data[gas_id].ptcch_ul_blk_start_cnt = GPL1_INVALID;
  gpl1_gprs_ptcch_data[gas_id].ptcch_dl_blocks_left = GPL1_INVALID;
}


gpl1_gprs_ta_T l1_get_gpl1_gprs_ta_info ( gas_id_t gas_id )
{
  gpl1_gprs_ptcch_data_t *gpl1_gprs_ptcch_data_ptr = &gpl1_gprs_ptcch_data[gas_id];
  return gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info ;
}

void l1_set_gpl1_gprs_ta_info (gpl1_gprs_ta_T gpl1_gprs_ta_info, gas_id_t gas_id )
{
  gpl1_gprs_ptcch_data_t *gpl1_gprs_ptcch_data_ptr = &gpl1_gprs_ptcch_data[gas_id];
  gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info  = gpl1_gprs_ta_info ;
}
/*===========================================================================

FUNCTION  gpl1_gprs_tx_ptcch_discarded_cb

DESCRIPTION
  This function is called by Non Portable Layer 1 when the scheduled
  ptcch transmision is discarded by NPL1 due to higher priority of
  another PDCH being scheduled or the message buffer in the mDSP is full.

  Passes back the message data and the msg_hdr which descrbes the
  received data.

DEPENDENCIES



RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gpl1_gprs_tx_ptcch_disc_cb( gl1_msg_tx_discarded_data_type  msgs_discarded[ GL1_DEFS_MAX_UL_MSGS ],
                                 uint8                           num_msg_txd,
                                 gas_id_t gas_id)
{
  NOTUSED(msgs_discarded);
  if(num_msg_txd)
    MSG_GERAN_ERROR_1_G("PTCCH MSG DISCARDED %d\n",num_msg_txd);
}


/*===========================================================================

FUNCTION  gpl1_gprs_tx_ptcch_scheduled_cb

DESCRIPTION

  This function is called by Non Portable Layer 1 when a scheduled PTCCH
  is sucessfully transmitted.


  Passes back an array of pointers to the scheduled messages and the number
  of pointers in the array.

DEPENDENCIES



RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gpl1_gprs_tx_ptcch_sched_cb( gl1_msg_tx_scheduled_data_type  msgs_txd[ GL1_DEFS_MAX_UL_MSGS ],
                                  uint8                           num_msg_txd, gas_id_t gas_id )
{
  gpl1_gprs_ptcch_data_t *gpl1_gprs_ptcch_data_ptr = &gpl1_gprs_ptcch_data[gas_id];
  /* Read out value of abort_callbacks from isr store */
  const gpl1_ps_store_T*  gpl1_store_ptr = gpl1_read_xfer_isr_store( gas_id );

  NOTUSED(msgs_txd);
  NOTUSED(num_msg_txd);

  /* If sub state is set to TRANSFER_STOP then ignore callback */
  if(gpl1_store_ptr->abort_callbacks)
  {
    return;
  }

#ifdef PL1_FW_SIM
  MSG_GERAN_MED_0_G("PTCCH MSG SCHEDULED CALLBACK");
#endif


  /* We have transmitted PTCCH/U therefore scheduling of the PTCCH/D
   * is allowed since it will contain our TA value */
  gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_dl_valid = TRUE;


}


/*===========================================================================

FUNCTION  gpl1_gprs_rx_ptcch_data_cb

DESCRIPTION
  This function is called by Non Portable Layer 1 when a scheduled
  PTCCH is received.


  Passes back the message data and the msg_hdr which descrbes the
  received data.

DEPENDENCIES



RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/


static void gpl1_gprs_rx_ptcch_data_cb(
                                gl1_defs_rx_pkt_data_type  *msg_data,
                                boolean                    cb_valid, gas_id_t gas_id
                               )
{
  gpl1_gprs_ptcch_data_t *gpl1_gprs_ptcch_data_ptr = &gpl1_gprs_ptcch_data[gas_id];
  uint8              tai;
  uint8              ta_value;
  uint8              current_ta;
  usf_dl_tfi_info_t  usf_dl_tfi_info =
      { FALSE, FALSE, 0xffff, 0xffff, 0xffff, GL1_DEFS_PDCH_UNALLOCATED };

  /* Pointer used to read the transfer/dtm common
   * data store.
   */
  const gpl1_ps_store_T * gpl1_store_ptr = gpl1_read_xfer_isr_store( gas_id );

  NOTUSED(cb_valid);

  /* Read out value of abort_callbacks from isr store
   * If sub state is set to TRANSFER_STOP then ignore callback
   */
  if(gpl1_store_ptr->abort_callbacks)
  {
    return;
  }

#ifdef PL1_FW_SIM
  MSG_GERAN_MED_0_G("PTCCH MSG DATA RECEIVED");
#endif
#ifdef FEATURE_G2X_TUNEAWAY
  /* During trials noticed that the following null ptr was being dereferenced
     Need to find out how this is happening*/
  if(gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.global_ta_params == NULL)
  {
    MSG_GERAN_ERROR_0_G("GTA ta params is NULL");
    return;
  }
  else
  {
    tai = gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.global_ta_params->tai;
  }
#else
  tai = gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.global_ta_params->tai;
#endif /* FEATURE_G2X_TUNEAWAY */

  /* Change the relative timeslot reported by NPl1 to the absolute
   * timeslot for logging purposes.
   */
  msg_data->tn = (gl1_defs_tn_type) ( (uint8) (msg_data->tn) + (uint8) (msg_data->camp_tn) );

  /* Depending on the log packet specified in QXDM, we can log ptcch/d + pdtch/d
   * or just ptcch/d
   */

  gpl1_log_gprs_message_metrics_a_ver2(GL1_MSG_DL_PTCCH,gl1_get_FN( gas_id ),msg_data,&usf_dl_tfi_info, gas_id);

 // gpl1_log_gprs_dlptcch_message_metrics(gl1_get_FN(),&msg_data->hdr);

  if(msg_data->msg[0].crc_passed)
  {
    /* Decode attempt good so reset decode attempt to zero */
    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_decode_attempts = 0;
    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.downlink_tai = tai;

    /* Retrieve ms's TA from PTCCH/D msg using assigned TAI */
    ta_value = (msg_data->msg[0].data[tai] & 0x7f);


    /* check if it is within the range 0 .. 63 bits
     * the range 64 .. 126 are reserved for future use
     * the value 127 indicates the TA field does not contain
     * a TA value - ref GSM 04.04 sec 6.1
     */
    if(ta_value < 64)
    {
      /* validate the received TA is a sensible value
       * If it has changed by more than 5 bits from the
       * last value then ignore it
       */
      current_ta = gl1_msg_pdch_retrieve_ta(gas_id);

      if (current_ta > ta_value)
      {
        if ((current_ta - ta_value) > 5)
        {
          /* ignore ta */
          MSG_GERAN_ERROR_2_G("TA Value out of range!! rcvd %d curr %d",ta_value,current_ta);
        }
      }else
      {
        if((ta_value - current_ta) > 5)
        {
          /* ignore ta */
          MSG_GERAN_ERROR_2_G("TA Value out of range!! rcvd %d curr %d",ta_value,current_ta);

        }
      }

      /* Inform new TA to NPL1 */
      gl1_msg_store_ta(ta_value,gas_id);
      MSG_GERAN_HIGH_2_G(" cont ta set = %d [%d]",ta_value,gl1_get_FN( gas_id ));

      /* Enable Tx's as we now have a valid TA */
      gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ta_valid = TRUE;

    }else
    {
      MSG_GERAN_LOW_1_G(" cont ta out of range",gl1_get_FN( gas_id ));
    }

  }else
  {

    if (++gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_decode_attempts < 4)
    {
      /* decode failed so attempt to read next DL PTCCH message */
      gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.downlink_tai += 4;

      /* wrap arround if greater than dl tai lookup table */
      if (gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.downlink_tai > 15)
      {
         gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.downlink_tai -= 16;
      }

      MSG_GERAN_HIGH_3_G("PL1: PTCCH/D unsucc.[%d] attempts %d tai %d",gl1_get_FN( gas_id ),gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_decode_attempts,gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.downlink_tai);

    }else
    {
      /* decoded all 4 available DL PTCCH messages report failure */
      MSG_GERAN_ERROR_0_G(" 4 x TA messages failed");
      gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_decode_attempts=0;
      gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.downlink_tai = tai;
    }


  }

}


/*===========================================================================

FUNCTION  gpl1_gprs_ptcch_metrics_cb

DESCRIPTION
  This function is called by Non Portable Layer 1 when a scheduled
  PTCCH metrics is received.



DEPENDENCIES



RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#define SNR_BAD_THRESHOLD_dBx16    32 /* dBx16 => 2 dB */
#define SNR_POST_BAD_THRESHOLD_dBx16_5dB    80 /* dBx16 => 5 dB */

static void gpl1_gprs_ptcch_metrics_cb(
                                      gl1_defs_rx_pkt_metrics_type *md[GL1_DEFS_MAX_DL_MSGS ],
                                      uint8    num_msgs,
                                      uint8    burst_num,
                                      gas_id_t gas_id
                                )
{
  gpl1_gprs_ptcch_data_t *gpl1_gprs_ptcch_data_ptr = &gpl1_gprs_ptcch_data[gas_id];
  if( FALSE != md[0]->valid )
  {
    /* Read out value of abort_callbacks from isr store */
    const gpl1_ps_store_T*  gpl1_store_ptr = gpl1_read_xfer_isr_store( gas_id );

    /* If sub state is set to TRANSFER_STOP then ignore callback */
    if( FALSE == gpl1_store_ptr->abort_callbacks )
    {
      const uint32  FN = gl1_get_FN( gas_id  );
      uint8* const  num_bursts_ptr = &( gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_snr_buff.num_bursts );

#ifdef PL1_FW_SIM
      MSG_GERAN_MED_0_G("PTCCH METRICS RECEIVED ");
#endif

      /* Calculate the absolute timeslot for logging */
      md[0]->tn = (gl1_defs_tn_type) ( (uint8) (md[0]->tn) + (uint8) (md[0]->camp_tn) );

      /* Depending on the log packet specified in QXDM, we can log ptcch/d + pdtch/d
       * or just ptcch/d
       */
      gpl1_log_gprs_burst_metrics_a( GL1_MSG_DL_PTCCH, FN, md[0],gas_id );
      gpl1_log_gprs_burst_metrics_a_ver2( GL1_MSG_DL_PTCCH, FN, md[0], gas_id );


#ifdef FEATURE_GSM_MDSP_ESAIC
      /* If ESAIC is used in a burst the mDSP sets bit 14 of */
      /* the misc_flag true. Additionally, for AFC tracking */
      /* the flag selects the specific GMSK SAIC threshold. */
      if ( gl1_msg_saic_in_burst( md[0]->misc_flags ) )
      {
        /* Update TT and AFC with the metrics data for the first burst only*/
        gl1_msg_dch_tt_update ( (int16)md[0]->timing_offset,
                                gl1_msg_gprs_saic_tt_threshold_snr( md[0] ), TRUE, gas_id );

        gl1_msg_dch_afc_update( (int16)md[0]->freq_offset,
                                gl1_msg_gprs_saic_afc_threshold_snr( md[0] ), TRUE,gas_id );
      }
      else
#endif /* FEATURE_GSM_MDSP_ESAIC */
      {
        /* Update TT and AFC with the metrics data for the first burst only*/
        gl1_msg_dch_tt_update ( (int16)md[0]->timing_offset, md[0]->snr, FALSE, gas_id );

        gl1_msg_dch_afc_update( (int16)md[0]->freq_offset, md[0]->snr, FALSE,gas_id );
      }

      gl1_msg_dch_afc_adjust(gas_id);

#ifdef FEATURE_GSM_MDSP_ESAIC
      /* If ESAIC is used in a burst the mDSP sets bit 14 of the misc_flag true */
      if (gl1_msg_saic_in_burst(md[0]->misc_flags))
      {
        /* store snr into buffer in dBx16 for averaging in ptcch data cb */
        gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_snr_buff.snr_bursts[ *num_bursts_ptr ] =
                                           gl1_msg_SNR_to_dBx16(md[0]->pre_mimo_trained_complete_snr);
      }
      else
      {
        /* store snr into buffer in dBx16 for averaging in ptcch data cb */
        gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_snr_buff.snr_bursts[ *num_bursts_ptr ] =
                                                                    gl1_msg_SNR_to_dBx16(md[0]->snr);
      }
#else
      /* store snr into buffer in dBx16 for averaging in ptcch data cb */
      gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_snr_buff.snr_bursts[ *num_bursts_ptr ] =
                                                                    gl1_msg_SNR_to_dBx16(md[0]->snr);
#endif

      gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_snr_buff.norm_snr_bursts[ *num_bursts_ptr ] =
                                                                     gl1_msg_SNR_to_dBx16(md[0]->snr);

      MSG_GERAN_LOW_3_G("ptcch snr db = %d snr = %d nbst %d",
              gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_snr_buff.snr_bursts[ *num_bursts_ptr ],
              md[0]->snr,
              *num_bursts_ptr );

      /* Increment the number of ptcch bursts received */
      (*num_bursts_ptr)++;

      if( (*num_bursts_ptr) == 4 )
      {
       dBx16_T  av_snr = 0;
       dBx16_T  av_snr_norm = 0;

       /* 4 bursts collected, so average snr and check threshold */
       /* it can be pre mimo or post norm burst avg snr */
       av_snr = ( gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_snr_buff.snr_bursts[0]
                           + gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_snr_buff.snr_bursts[1]
                           + gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_snr_buff.snr_bursts[2]
                           + gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_snr_buff.snr_bursts[3] ) >> 2; /*lint !e702: shift of signed quantity */

      /* avg burst snr */
       av_snr_norm = ( gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_snr_buff.norm_snr_bursts[0]
                           + gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_snr_buff.norm_snr_bursts[1]
                           + gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_snr_buff.norm_snr_bursts[2]
                           + gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_snr_buff.norm_snr_bursts[3] ) >> 2; /*lint !e702: shift of signed quantity */

      /*
      ** under high interferance pre-mimo snr can b very low and to avoid unnecessary reselection
      ** a check is made against post processed avg burst snr.
      */
      if ( ( av_snr < SNR_BAD_THRESHOLD_dBx16 ) && //premimo snr av_snr < 2dB
           ( av_snr_norm < SNR_POST_BAD_THRESHOLD_dBx16_5dB ) )  // burst snr < 5dB
      {
        gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_snr_bad = TRUE;
      }
      else
      {
        gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_snr_bad = FALSE;
      }

        /* If the PTCCH SNR is BAD then check to see that the SNR of the PDTCH is bad too. If not
        then overrule this decision to flag the SNR as bad to RR in the measurement report. */
        if ( gpl1_is_pdtch_snr_bad(gas_id) == FALSE )
        {
          if ( gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_snr_bad == TRUE )
          {
            gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_snr_bad = FALSE;
            MSG_GERAN_HIGH_0_G("PDTCH overruling the PTCCH bad SNR");
          }
        }
        /* Let the serving_meas_ind handler know that ptcch_snr_bad flag is valid */
        gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_snr_bad_valid = TRUE;

        /* reset num burst for next ptcch message */
        *num_bursts_ptr = 0;

        MSG_GERAN_MED_3_G( "av snr %d %d [%d]", av_snr, av_snr_norm, FN );
      }

    } /* End of abort callbacks is FALSE */
  }   /* End of if metrics are valid */
  else
  {
    MSG_GERAN_ERROR_1_G("Invalid ptcch burst %u metrics", burst_num);
  }
} /*lint !e715: num msgs not used */

/*===========================================================================

FUNCTION  gpl1_gprs_config_ptcch

DESCRIPTION
  This function is called to configure or reconfigure the uplink and downlink
  PTCCH channels. Any reconfiguration of the PTCCH channel will involve changes
  to both the uplink and downlink PTCCH channel handlers.

DEPENDENCIES

  requires dynamic and frequency parametrs to be pre-configured

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gpl1_gprs_config_ptcch (
                            uint8                   tsc,
                            uint8                   lowest_dl_timeslot,
                            cont_ta_information_T  *ta_params_ptr,
                            gas_id_t gas_id
                            )
{
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
  gl1_defs_rx_pkt_cb_type   ptcch_rx_cb;
  gl1_msg_tx_pkt_cb_type    ptcch_tx_cb;
  gl1_defs_coding_type      cs = GL1_DEFS_ACCESS_CODING;
  uint8                     slammed_slot = 0;

  /*Configure DL PTCCH callbacks  */
  ptcch_rx_cb.metrics_callback = gpl1_gprs_ptcch_metrics_cb;
  ptcch_rx_cb.data_callback = gpl1_gprs_rx_ptcch_data_cb;
  ptcch_rx_cb.discarded_callback = NULL;
  ptcch_rx_cb.log_callback = NULL;

  /*Configure DL PTCCH callbacks  */
  ptcch_tx_cb.discarded_callback = gpl1_gprs_tx_ptcch_disc_cb;
  ptcch_tx_cb.scheduled_callback = gpl1_gprs_tx_ptcch_sched_cb;

  /* pick coding scheme acording to sys info parameter */
  if(l1_idle_data->layer1_extra_gprs_sys_info.access_burst_type == 0)
  {
  cs = GL1_DEFS_ACCESS_CODING;

  }else if(l1_idle_data->layer1_extra_gprs_sys_info.access_burst_type == 1)
  {
    cs = GL1_DEFS_EXT_ACCESS_CODING;
  }

  slammed_slot = ta_params_ptr->ta_timeslot - lowest_dl_timeslot;

  /* Configure NPL1 for Uplink PTCCH callback */
  gl1_msg_cfg_ul_ptcch( (gl1_defs_tn_type)slammed_slot,
                        cs,
                        l1_idle_data->campedon_cell_BSIC,
                        &ptcch_tx_cb,
                        gas_id );

  /* Configure NPL1 for DL PTCCH callback */
  gl1_msg_cfg_dl_ptcch( (gl1_defs_tn_type) slammed_slot,
                        tsc,
                        &ptcch_rx_cb,gas_id );

  /* This function is called to clear out the accumulated SNR storage variables */
  gpl1_reset_pdtch_snr_variables(gas_id);

  MSG_GERAN_HIGH_2_G("ta cfg tn %d tai %d",ta_params_ptr->ta_timeslot,ta_params_ptr->tai);
}

/*===========================================================================

FUNCTION  gpl1_gprs_add_ta_params

DESCRIPTION
  This function is called to add the new timing advance parameters for UL TBF.

DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_add_ul_ta_params(timing_advance_params_T timing_advance_params,
                                boolean l1_called_from_sngl_blk,
                                uint8 single_block_ta_value, gas_id_t gas_id)
{
   if (l1_called_from_sngl_blk)
   {
      gpl1_gprs_add_to_active_ul_ta_params(timing_advance_params, l1_called_from_sngl_blk,single_block_ta_value, gas_id);
   }
   else
      gpl1_gprs_add_to_pending_ul_ta_params(timing_advance_params, gas_id);
}

/*===========================================================================

FUNCTION  gpl1_gprs_add_to_active_ul_ta_params

DESCRIPTION
  This function adds timing advance parameters to active_ta_params for UL TBF.

DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_add_to_active_ul_ta_params (timing_advance_params_T ta_params,
                                           boolean l1_called_from_sngl_blk,
                                           uint8 single_block_ta_value, gas_id_t gas_id)
{
  gpl1_gprs_ptcch_data_t *gpl1_gprs_ptcch_data_ptr = &gpl1_gprs_ptcch_data[gas_id];
  gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.ta_param_ptr = NULL;

  /* concurrent tbf not possible from here so setup global ta params */
  gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.global_ta_params = &(gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.ul_ta_params);

  if (ta_params.cont_ta_ie.present)
  {
    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.ul_ta_params = ta_params.cont_ta_ie;

    /* setup ul ta as valid and dl ta as invalid */
    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.ul_cont_ta_valid = TRUE;

    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.dl_cont_ta_valid = FALSE;

  }
  else
  {
    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.ul_cont_ta_valid = FALSE;

    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.dl_cont_ta_valid = FALSE;

    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.ul_ta_params.present = FALSE;
  }

  /* Check to see if a TA value has been specified */
  if (ta_params.ta_value_ie.present)
  {
    /* concurrent tbf not possible from here so setup global ta params */
    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.tav = ta_params.ta_value_ie;

    /* send ta value to NPL1 */
    gl1_msg_store_ta(gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.tav.ta_value,gas_id);

    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ta_valid = TRUE;
  }
  else
  {
    /* TA is not valid until we received a PTCCH/D on our TAI subchannel */
    /* or we receive a TA value */
    if (l1_called_from_sngl_blk)
       gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.tav.ta_value = single_block_ta_value;
    else
       gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.tav.present= FALSE;

    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ta_valid = TRUE;
  }
}

/*===========================================================================

FUNCTION  gpl1_gprs_add_to_active_dl_ta_params

DESCRIPTION
  This function adds timing advance parameters to active_ta_params for DL TBF.

DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_add_to_active_dl_ta_params (timing_advance_params_T ta_params, gas_id_t gas_id)
{
  gpl1_gprs_ptcch_data_t *gpl1_gprs_ptcch_data_ptr = &gpl1_gprs_ptcch_data[gas_id];
  /* concurrent tbf not possible from here so setup global ta params */
  gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.global_ta_params = &(gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.dl_ta_params);

  /* Ensure ta param pointer is null to stop cont. TA params swapping to UL */
  gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.ta_param_ptr = NULL;

  /* Check to see if there are continuous timing advance params present */
  if (ta_params.cont_ta_ie.present)
  {
    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.dl_ta_params = ta_params.cont_ta_ie;

    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.dl_cont_ta_valid = TRUE;

    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.ul_cont_ta_valid = FALSE;

  }
  else
  {
    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.dl_cont_ta_valid = FALSE;

    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.ul_cont_ta_valid = FALSE;

    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.dl_ta_params.present = FALSE;
  }

  /* Check to see if a TA value has been specified */
  if (ta_params.ta_value_ie.present)
  {
    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.tav = ta_params.ta_value_ie;

    /* send ta value to NPL1 */
    gl1_msg_store_ta(gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.tav.ta_value,gas_id);

    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ta_valid = TRUE;
  }
  else
  {
    /* TA is not valid until we received a PTCCH/D on our TAI subchannel */
    /* or we receive a TA value */
    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ta_valid = FALSE;

    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.tav.present = FALSE;
  }
}

/*===========================================================================

FUNCTION  gpl1_gprs_add_to_pending_ul_ta_params

DESCRIPTION
  This function adds timing advance parameters to pending_ta_params for UL TBF.

DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gpl1_gprs_add_to_pending_ul_ta_params (timing_advance_params_T ta_params, gas_id_t gas_id)
{
  gpl1_gprs_ptcch_data_t *gpl1_gprs_ptcch_data_ptr = &gpl1_gprs_ptcch_data[gas_id];
  /* Check to see if there are continuous timing advance params present */
  if (ta_params.cont_ta_ie.present)
  {
    /* write cont. ta from the message into the pending store */
    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.ul_ta_params = ta_params.cont_ta_ie;

    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.ul_cont_ta_valid = TRUE;

    /* If we currently have DL Cont TA params write them back to pending store */
    if (gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.dl_ta_params.present)
    {
      gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.dl_cont_ta_valid = TRUE;
      gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.dl_ta_params = 
                            gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.dl_ta_params;

    }
    else
    {
      /* Set valid = FALSE to indicate DL params are empty*/
      gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.dl_cont_ta_valid = FALSE;
    }

  }
  else
  {
    /* If we currently have DL Cont TA params write them back to pending store */
    if (gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.dl_ta_params.present)
    {
      gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.dl_cont_ta_valid = TRUE;
      gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.dl_ta_params = 
                            gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.dl_ta_params;

    }
    else
    {
      /* Set valid = FALSE to indicate DL params are empty*/
      gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.dl_cont_ta_valid = FALSE;
    }

    /* Set valid = FALSE to indicate UL params are empty*/
    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.ul_cont_ta_valid = FALSE;

  }

  /* Check to see if a TA value has been specified */
  if (ta_params.ta_value_ie.present)
  {
    /* Write ta value to the pending store */
    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.tav = ta_params.ta_value_ie;

  }
  else
  {
    /* no TA value present so store current params in the pending dl structure */
    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.tav.present = FALSE;
  }
}

/*===========================================================================

FUNCTION  gpl1_gprs_add_to_pending_dl_ta_params

DESCRIPTION
  This function adds timing advance parameters to pending_ta_params for DL TBF.

DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_add_to_pending_dl_ta_params (timing_advance_params_T ta_params, gas_id_t gas_id)
{
  gpl1_gprs_ptcch_data_t *gpl1_gprs_ptcch_data_ptr = &gpl1_gprs_ptcch_data[gas_id];
  /* Check to see if there are continuous timing advance params present */
  if (ta_params.cont_ta_ie.present)
  {
    /* write cont. ta from the message into the pending store */
    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.dl_ta_params = ta_params.cont_ta_ie;

    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.dl_cont_ta_valid = TRUE;

    /* If we currently have UL Cont TA params write them back to pending store */
    if (gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.ul_ta_params.present)
    {
      gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.ul_cont_ta_valid = TRUE;
      gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.ul_ta_params = 
                            gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.ul_ta_params;

    }
    else
    {
      /* Set valid = FALSE to indicate UL params are empty*/
      gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.ul_cont_ta_valid = FALSE;
    }

  }else
  {
    /* If we currently have UL Cont TA params write them back to pending store */
    if (gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.ul_ta_params.present)
    {
      gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.ul_cont_ta_valid = TRUE;
      gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.ul_ta_params = 
                gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.ul_ta_params;

    }
    else
    {
      /* Set valid = FALSE to indicate UL params are empty*/
      gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.ul_cont_ta_valid = FALSE;
    }

    /* Set valid = FALSE to indicate DL params are empty*/
    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.dl_cont_ta_valid = FALSE;
  }

  /* Check to see if a TA value has been specified */
  if (ta_params.ta_value_ie.present)
  {
    /* Write ta value to the pending store */
    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.tav =  ta_params.ta_value_ie;

  }else
  {
    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.tav.present = FALSE;
  }
}

/*===========================================================================

FUNCTION  gpl1_gprs_config_global_ta_param

DESCRIPTION
  This function reconfigure active ta_parameters with the new global timing
  advance values.

DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean gpl1_gprs_config_global_ta_param(global_packet_ta_T global_ta, gas_id_t gas_id)
{
  gpl1_gprs_ptcch_data_t *gpl1_gprs_ptcch_data_ptr = &gpl1_gprs_ptcch_data[gas_id];
  uint8 stored_tav;
  boolean ignore_reconfigure = FALSE;

  if (global_ta.downlink_tai.present)
  {
     /* Check if new cont ta params have changed */
    if ( (gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.dl_cont_ta_valid) &&
         (gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.dl_ta_params.ta_timeslot == global_ta.downlink_tai.ta_timeslot) &&
         (gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.dl_ta_params.tai == global_ta.downlink_tai.tai)
       )
    {
       /* Cont. TA params are same as current so ignore this reconfigure */
       ignore_reconfigure = TRUE;

       MSG_GERAN_LOW_0_G("ignore dl ptcch");
    }
    else
    {
       /* Cont TA params different so reconfigure */
       ignore_reconfigure = FALSE;

       gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.dl_ta_params = global_ta.downlink_tai;

       gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.dl_cont_ta_valid = TRUE;

       MSG_GERAN_MED_2_G("dl cont ta changed tai %d tn %d",global_ta.downlink_tai.ta_timeslot,global_ta.downlink_tai.tai);
    }
  }

  if (global_ta.uplink_tai.present)
  {
     /* Check if new cont ta params have changed */
     if ( (gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.ul_cont_ta_valid) &&
          (gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.ul_ta_params.ta_timeslot == global_ta.uplink_tai.ta_timeslot) &&
          (gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.ul_ta_params.tai == global_ta.uplink_tai.tai)
        )
     {
        /* Cont. TA params are same as current so ignore this reconfigure */
        ignore_reconfigure = TRUE;

        MSG_GERAN_LOW_0_G("ignore ul ptcch");
     }
     else
     {
        /* Cont TA params different so reconfigure */
        ignore_reconfigure = FALSE;

        gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.ul_ta_params = global_ta.uplink_tai;

        gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.ul_cont_ta_valid = TRUE;

        MSG_GERAN_MED_2_G("ul cont ta changed tai %d tn %d",global_ta.uplink_tai.tai,global_ta.uplink_tai.ta_timeslot);
     }
  }


  if (global_ta.tav.present)
  {
     /* Check if the TAV is the same as the current stored value */
     stored_tav = gl1_msg_pdch_retrieve_ta(gas_id);

     /* If we don't currently have a valid TA don't ignore the reconfigure */
     if ((stored_tav != global_ta.tav.ta_value) ||
         (!gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ta_valid) ||
          (ignore_reconfigure == FALSE))
     {
       ignore_reconfigure = FALSE;

       gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.tav = global_ta.tav;

       MSG_GERAN_MED_1_G("ta value  = %d",global_ta.tav.ta_value);
     }
     else
     {
       /* TAV params are same as current so ignore this reconfigure */
       ignore_reconfigure = TRUE;
     }
  }
  else
  {
    MSG_GERAN_LOW_0_G("tav not present");
  }

  return(ignore_reconfigure);
}

/*===========================================================================

FUNCTION  gpl1_gprs_add_global_ta_param

DESCRIPTION
  This function adds new global timing advance values to pending timing advance
  parameters

DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_add_global_ta_param(global_packet_ta_T global_ta, gas_id_t gas_id)
{
  gpl1_gprs_ptcch_data_t *gpl1_gprs_ptcch_data_ptr = &gpl1_gprs_ptcch_data[gas_id];
  /* Check to see if there are continuous timing advance params present, if both uplink
   * and downlink ta parameters are available then we use Uplink parameters since most tx's
   * will be on uplink
   */
  if (global_ta.uplink_tai.present)
  {

     MSG_GERAN_HIGH_2_G("ul cont tai %d tn %d",global_ta.uplink_tai.tai,global_ta.uplink_tai.ta_timeslot);

     /* write cont. ta from the message into the pending store */
     gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.ul_ta_params = global_ta.uplink_tai;

     gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.ul_cont_ta_valid = TRUE;

     gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.dl_cont_ta_valid = FALSE;

  }

  if(global_ta.downlink_tai.present)
  {

     MSG_GERAN_HIGH_2_G(" dl cont tai %d tn %d",global_ta.downlink_tai.tai,global_ta.downlink_tai.ta_timeslot);

     /* write cont. ta from the message into the pending store */
     gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.dl_ta_params = global_ta.downlink_tai;

     gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.dl_cont_ta_valid = TRUE;

     if (!global_ta.uplink_tai.present)
     {
       /* Ensure ul cont ta. is disabled if not present in the message */
        gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.ul_cont_ta_valid = FALSE;
     }

     gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.dl_cont_ta_valid = TRUE;

  }

 /* For PTR we need to use one of the above cont. ta. configs.
  * If we are using ul tbf cont ta config and this is not present
  * then we need to swap over to the dl tbf. This is also true for
  * the dl tbf cont ta. config.
  * If neither ul or dl cont ta is present we need to turn off cont ta.
  */

  if ( (gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.ul_cont_ta_valid) && (gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.global_ta_params == &gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.ul_ta_params) )
  {
    /***** We are using ul cont ta params *****/

    if (gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.ul_cont_ta_valid == FALSE)
    {
      if (gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.dl_cont_ta_valid)
      {
        /* swap over and use new dl cont ta config */
        MSG_GERAN_LOW_0_G(" swap to dl params ");

        gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.ta_param_ptr = &(gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.dl_ta_params);
      }
      else
      {
        /* turn off cont ta. */

        MSG_GERAN_ERROR_0_G(" No cont ta params present");
      }
    }
  }
  else if ( (gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.dl_cont_ta_valid) && (gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.global_ta_params == &gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.dl_ta_params) )
  {
    /**** We are using dl cont ta params ****/
    if (gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.dl_cont_ta_valid == FALSE)
    {
      if (gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.ul_cont_ta_valid)
      {
         MSG_GERAN_LOW_0_G("Swap to ul params");

         /* swap over and use new ul cont ta config */
         gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.ta_param_ptr = &(gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.ul_ta_params);
      }
      else
      {
         /* turn off cont ta. */
         MSG_GERAN_ERROR_0_G("No cont ta params present");
      }
    }
  }

  /* Check to see if a TA value has been specified */
  if (global_ta.tav.present)
  {
    /* Write ta value to the pending store */
    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.tav = global_ta.tav;

  }
  else
  {
    /* no TA value present */

    /* setup pending ta value to not present */
    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.tav.present = FALSE;
  }
}

/*===========================================================================

FUNCTION  gpl1_gprs_reset_pending_ta_params

DESCRIPTION
  This function reset pending_ta_params variables

DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_reset_pending_ta_params(gas_id_t gas_id)
{
  gpl1_gprs_ptcch_data_t *gpl1_gprs_ptcch_data_ptr = &gpl1_gprs_ptcch_data[gas_id];
  /* ta parameters unchanged so set pending flags to flase */
  gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.dl_cont_ta_valid = FALSE;

  gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.ul_cont_ta_valid = FALSE;

}

/*===========================================================================

FUNCTION  gpl1_gprs_swap_dl_cont_ta_to_ul

DESCRIPTION
  This function switches cont ta param from dl to ul ta param

DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_swap_dl_cont_ta_to_ul(gas_id_t gas_id)
{
  gpl1_gprs_ptcch_data_t *gpl1_gprs_ptcch_data_ptr = &gpl1_gprs_ptcch_data[gas_id];
  if ( (gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.global_ta_params) == &(gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.dl_ta_params) )
  {
     /* If UL cont. ta params are not available then need to disable
      * transmission when the release is actioned
      */
     gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.ta_param_ptr = &(gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.ul_ta_params);
  }
}

/*===========================================================================

FUNCTION  gpl1_gprs_swap_ul_cont_ta_to_dl

DESCRIPTION
  This function switches cont ta param from ul to dl ta param

DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_swap_ul_cont_ta_to_dl(gas_id_t gas_id)
{
  gpl1_gprs_ptcch_data_t *gpl1_gprs_ptcch_data_ptr = &gpl1_gprs_ptcch_data[gas_id];
  if ( (gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.global_ta_params) == &(gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.ul_ta_params) )
  {
     /* check if we are using the ul cont. ta params
      * dosen't matter if we are using dl cont. parameters
      */
    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.ta_param_ptr = &(gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.dl_ta_params);
  }
}

/*===========================================================================

FUNCTION  gpl1_gprs_setup_cont_ta_params

DESCRIPTION
  This function setup continuous timing advance parameters for PTCCH.

DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_setup_cont_ta_params(gas_id_t gas_id)
{
  gpl1_gprs_ptcch_data_t *gpl1_gprs_ptcch_data_ptr = &gpl1_gprs_ptcch_data[gas_id];

    /* We don't have either UL or Downlink pending PTCCH assignments, therefore turn off Cont. TA*/
  if ( ((!gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.ul_cont_ta_valid) &&
        (!gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.dl_cont_ta_valid)) &&
        (!gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.ta_param_ptr) )
  {
    
    MSG_GERAN_HIGH_0_G("CONT TA params turned off");
    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.ul_ta_params.present = FALSE;
    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.ul_cont_ta_valid = FALSE;
    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.dl_ta_params.present = FALSE;
    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.dl_cont_ta_valid = FALSE;
    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_dl_valid = FALSE;
    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_decode_attempts = 0;

    /* abort any ongoing dl ptcch */
    gl1_msg_abort_dl_ptcch(gas_id);
  }
  else
  {

    /* setup cont. timing advance params for downlink tbf */
    if (gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.dl_cont_ta_valid)
    {
       /* check if there are any current dl cont ta params
        * and we are using the DL TAI
        */
       if ( (gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.dl_cont_ta_valid) && (gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.global_ta_params == &gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.dl_ta_params) )
       {
         /* check if the tai or ta timeslot has changed */
         if ( (gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.dl_ta_params.ta_timeslot != gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.dl_ta_params.ta_timeslot) ||
              (gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.dl_ta_params.tai != gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.dl_ta_params.tai) )
         {
           gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_reconfig = TRUE;

           /* invalidate DL PTCCH incase UL PTCCH has already been sent */
           gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_dl_valid = FALSE;

           gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_decode_attempts =0;

           /* set tai for first DL Ptcch attempt */
           gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.downlink_tai =  gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.dl_ta_params.tai;

           /* abort any ongoing dl ptcch */
           gl1_msg_abort_dl_ptcch(gas_id);

           MSG_GERAN_LOW_0_G("DL TA PTCCH Invalidated");
         }
       }

       /* Check if we currently don't have cont. TA params */
       if ((!gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.dl_cont_ta_valid) && (!gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.global_ta_params->present))
       {
         MSG_GERAN_HIGH_0_G("CONT TA params turned on");

         gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_reconfig = TRUE;

         /* invalidate DL PTCCH incase UL PTCCH has already been sent */
         gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_dl_valid = FALSE;

         gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_decode_attempts =0;

         /* set tai for first DL Ptcch attempt */
         gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.downlink_tai = gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.dl_ta_params.tai;

         gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.global_ta_params = &gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.dl_ta_params;
       }

       /* downlink Cont. TA has been updated so update active params */
       gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.dl_ta_params = gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.dl_ta_params;

       /* dl ta available so set valid flag to false and disable uplink ta flag */
       gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.dl_cont_ta_valid = TRUE;

       /* disable dl ta valid flags in the pending ta store */
       gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.dl_cont_ta_valid = FALSE;

    }

    /* setup cont. timing advance params for uplink tbf*/
    if (gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.ul_cont_ta_valid)
    {
      /* check if there are any current ul cont ta params
       * and we are currently using the UL TAI
       */
      if ((gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.ul_cont_ta_valid) && (gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.global_ta_params == &gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.ul_ta_params))
      {

        /* check if the tai or ta timeslot has changed */
        if ( (gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.ul_ta_params.ta_timeslot != gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.ul_ta_params.ta_timeslot) ||
             (gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.ul_ta_params.tai != gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.ul_ta_params.tai) )
        {
           gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_reconfig = TRUE;

           /* invalidate DL PTCCH incase UL PTCCH has already been sent */
           gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_dl_valid = FALSE;

           gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_decode_attempts =0;

           /* set tai for first DL Ptcch attempt */
           gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.downlink_tai = gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.ul_ta_params.tai;

           /* abort any ongoing dl ptcch */
           gl1_msg_abort_dl_ptcch(gas_id);

           MSG_GERAN_LOW_0_G("DL TA PTCCH Invalidated");
        }
      }

      /* Check if we currently don't have cont. TA params */
      if ( (!gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.ul_cont_ta_valid) && (!gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.global_ta_params->present) )
      {

        MSG_GERAN_HIGH_0_G("CONT TA params turned on");

        gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_reconfig = TRUE;

        /* invalidate DL PTCCH incase UL PTCCH has already been sent */
        gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_dl_valid = FALSE;

        gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_decode_attempts =0;

        /* set tai for first DL Ptcch attempt */
        gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.downlink_tai = gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.ul_ta_params.tai;

        gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.global_ta_params = &gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.ul_ta_params;
      }

      /* uplink Cont. TA has been updated so update active params */
      gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.ul_ta_params = gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.ul_ta_params;

      /* ul ta available so set valid flag to false and disable uplink ta flag */
      gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.ul_cont_ta_valid = TRUE;

      /* disable ul ta valid flags in the pending ta store */
      gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.ul_cont_ta_valid = FALSE;
    }


  }/* End of Else no cont TA present */

  /* check that we have a ta value so we can send normal bursts and we don't currently
   * have a tav in use
   */
  if (  (gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.tav.present) &&
        ((!gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ta_valid) ||
        (!gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.global_ta_params->present))
     )
  {
    /* Update pending ta value to the active store  */
    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.tav = gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.tav;

    /* send ta value to NPL1 */
    gl1_msg_store_ta(gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.tav.ta_value,gas_id);

    MSG_GERAN_HIGH_1_G("tav set %d",gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.tav.ta_value);

    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ta_valid = TRUE;

  }
  else
  {
    /* no valid ta value available so wait until the cont. ta provides one */
    //gpl1_gprs_ta_info.ta_valid = FALSE;
     MSG_GERAN_LOW_0_G("no ta value");
  }

  if (gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.ta_param_ptr)
  {
    MSG_GERAN_LOW_0_G("cont ta params swapped");

    /* If we have swapped to Cont TA params that are empty then turn off Cont TA params */
    if (!gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.ta_param_ptr->present)
    {
      MSG_GERAN_HIGH_0_G("CONT TA params turned off");
      gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.ul_ta_params.present = FALSE;
      gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.ul_cont_ta_valid = FALSE;
      gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.dl_ta_params.present = FALSE;
      gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.dl_cont_ta_valid = FALSE;
      gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_dl_valid = FALSE;
      gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_decode_attempts = 0;

      /* abort any ongoing dl ptcch */
      gl1_msg_abort_dl_ptcch(gas_id);
    }
    else
    {

      /* ul tbf has been released so swap tai parameters to dl tbf ones */
      /* Check if the continuous ta parameters have changed */
      if ( (gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.global_ta_params->present) &&
           ( (gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.global_ta_params->ta_timeslot != gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.ta_param_ptr->ta_timeslot)  ||
             (gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.global_ta_params->tai != gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.ta_param_ptr->tai)
           )
         )
      {
        gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_reconfig = TRUE;

        MSG_GERAN_LOW_0_G("cont ta params changed");

        /* params changed so invalidate DL PTCCH incase UL PTCCH has already been sent */
        gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_dl_valid = FALSE;

        gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_decode_attempts =0;

        /* set tai for first DL Ptcch attempt */
        gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.downlink_tai = gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.ta_param_ptr->tai;

        /* abort any ongoing dl ptcch */
        gl1_msg_abort_dl_ptcch(gas_id);
      }

      /* assign new parameters to ta pointer and null the pending pointer */
      gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.global_ta_params = gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.ta_param_ptr;

    }

    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.pending_ta_params.ta_param_ptr = NULL;
  }

}

/*===========================================================================

FUNCTION  gpl1_gprs_config_ta_param

DESCRIPTION
  This function setup PTCCH physical layer allocation for timing advance.

DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_config_ta_param( boolean  transfer_init_ind,
                                uint8    tsc,
                                uint8    lowest_dl_timeslot,
                                gas_id_t gas_id )
{
  gpl1_gprs_ptcch_data_t *gpl1_gprs_ptcch_data_ptr = &gpl1_gprs_ptcch_data[gas_id];
  uint8 i=0;

  /* Check to see if there are any timing advance prameters available */

  if (transfer_init_ind)
  {
      /*  Initialize snr tn array  */
      for (i=0; i<4;i++)
      {
        /* initialize ptcch snr array */
        gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_snr_buff.snr_bursts[i] = 0;
      }

      gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_snr_buff.num_bursts = 0;
      gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_snr_bad = FALSE;
  }

  if( (gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.global_ta_params->present) &&
      ( (gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_reconfig == TRUE) || (transfer_init_ind) )
    )
  {
    /* check that our assigned ptcch params agree with our assigned channel, otherwise
     * disable the cont ta. params as we don't trust them
     */
    if( FALSE != gpl1_gprs_calc_ptcch_tn_clash( gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.global_ta_params->ta_timeslot,
                                                lowest_dl_timeslot, gas_id ) )
    {
      MSG_GERAN_ERROR_0_G("PTCCH params invalid");

      gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.global_ta_params->present = FALSE;
    }
    else
    {

      /* Configure PTCCH handlers for both uplink and downlink */
      gpl1_gprs_config_ptcch ( tsc,
                               lowest_dl_timeslot,
                               gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.global_ta_params, gas_id );

      gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_reconfig = FALSE;

      /* Since reconfigure of PTCCH channel will result in abort of DL, reset snr average for PTCCH/D */
      gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_snr_buff.num_bursts=0;

      gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_snr_bad = FALSE;

      if (transfer_init_ind)
      {
        /* preset decode attempts to zero */
        gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_decode_attempts =0;

        /* set tai for first DL Ptcch attempt */
        gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.downlink_tai = gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.global_ta_params->tai;

        /* intitalise gpl1_gprs_ta_info.ptcch_dl_valid as FALSE since we could be closer
         * to receiving a PTCCH_D than a PTCCH_U. In this case the TA
         * value decoded in the PTCCH/D will be invalid.
         */

        gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_dl_valid = FALSE;

      }
    }

  } /* end of if cont ta */
}

#ifdef FEATURE_GSM_COEX_SW_CXM
/*===========================================================================

FUNCTION  gpl1_check_if_ptcch_scheduled

DESCRIPTION
  This function determines if a PTCCH is scheduled for the specified FN and
  passes back the unslammed TN and direction (UL or DL) of the scheduled
  PTCCH.

DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean gpl1_check_if_ptcch_scheduled(uint32 FN,uint8 *ptcch_tn,boolean *dirn,gas_id_t gas_id)
{
  gpl1_gprs_ptcch_data_t *gpl1_gprs_ptcch_data_ptr = &gpl1_gprs_ptcch_data[gas_id];
  if(gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.global_ta_params !=NULL)
  {
  if(!gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.global_ta_params->present)
  {
     /* if no continuous TA params available then don't schedule PTCCH */
     return FALSE;
  }

  /* Check if FN pointed to by TAI value is this comming UL PTCCH frame */
  if( ((FN + 1 )% 416) == ptcch_ul_lookup[gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.global_ta_params->tai] )
  {
    /* Actual TS , not slammed */
    *ptcch_tn = gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.global_ta_params->ta_timeslot;
    *dirn = TRUE; // uplink

    return TRUE;
  }
  else if( (((FN + 1)%416) ==
       ptcch_dl_lookup[gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.downlink_tai])
       && (gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_dl_valid) )
  {

    *ptcch_tn = gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.global_ta_params->ta_timeslot;
    *dirn = FALSE; //downlink

    return TRUE;
  }
}
  return FALSE;
}
#endif /* FEATURE_GSM_COEX_SW_CXM */

/*===========================================================================

FUNCTION  gpl1_gprs_schedule_ptcch_tx

DESCRIPTION
  This function scehdule L1 for transmitting UL PTCCH


DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_schedule_ptcch_tx(ARFCN_T  gprs_arfcn,
                                 boolean access_burst_type, gas_id_t gas_id)
{
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
  gpl1_gprs_ptcch_data_t *gpl1_gprs_ptcch_data_ptr = &gpl1_gprs_ptcch_data[gas_id];
  uint8    tai;
  dBx16_T  txpwr = 5;

  uint8    eng_mode_ta;

  txpwr = (dBx16_T)gprs_serv_cell_meas_ptr->pch[gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.global_ta_params->ta_timeslot];

  tai = gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.global_ta_params->tai;

  if(!gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.global_ta_params->present)
  {
     /* if no continuous TA params available then don't schedule PTCCH */
     return;
  }

  /* Check if FN pointed to by TAI value is this comming UL PTCCH frame */
  if( ((frame_counters[gas_id].FNmod416 + 2)% 416) == ptcch_ul_lookup[tai] )
  {
    /* temp place holder for TX PWR to write Eng Mode Info */
    int16 tmp_tx_pwr[8] = {0};

    /* need to find a method of checking if the access burst type has changed since the
     * PTCCH UL was last configured and call gl1_msg_cfg_ul_ptcch()
     */

    if(access_burst_type == 0)
    {
      /* Need to pass pointer of data to transmit based on access burst type */
      gl1_msg_tx_ptcch(&gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ul_ptcch_8_bit,txpwr,gprs_arfcn,gas_id);
    }
    else if(access_burst_type == 1)
    {
      /* Set some Arbitary value for the time being */
      gl1_msg_tx_ptcch(gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ul_ptcch_11_bit,txpwr,gprs_arfcn,gas_id);
    }

    if(gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.tav.present)
    {
      geran_eng_mode_data_write(ENG_MODE_TIMING_ADVANCE,&(gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params.tav.ta_value),TRUE, gas_id);
    }
    else
    {
      eng_mode_ta = gl1_msg_pdch_retrieve_ta(gas_id);
      geran_eng_mode_data_write(ENG_MODE_TIMING_ADVANCE,&eng_mode_ta,TRUE, gas_id);
    }

    gpl1_log_timing_advance( &( gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.active_ta_params ),
                             gl1_msg_pdch_retrieve_ta(gas_id ),gas_id );

    gpl1_log_gprs_power_control( GL1_MSG_UL_PTCCH,
                                 (gl1_defs_tn_type)gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.global_ta_params->ta_timeslot,
                                 txpwr,
                                 (access_burst_type?GL1_DEFS_EXT_ACCESS_CODING:GL1_DEFS_ACCESS_CODING), gas_id );


    tmp_tx_pwr[0] = txpwr;
    geran_eng_mode_data_write(ENG_MODE_TX_PWR, tmp_tx_pwr, TRUE, gas_id);

    /* UL BLOCK start for UL PTCCH to be set in 1 frame */
    gpl1_gprs_ptcch_data_ptr->ptcch_ul_blk_start_cnt = 1;

  }
}

/*===========================================================================

FUNCTION  gpl1_gprs_schedule_ptcch_rx

DESCRIPTION
  This function SCHEDULE L1 for receiving DL PTCCH.


DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_schedule_ptcch_rx(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  gpl1_gprs_ptcch_data_t *gpl1_gprs_ptcch_data_ptr = &gpl1_gprs_ptcch_data[gas_id];
  uint8 i=0;
  transfer_data_T   *transfer_data_ptr =
                         l1_tskisr_blk->current_params.L1Data.pTransfer_data;
  if(!gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.global_ta_params->present)
  {
     /* if no continuous TA params available then don't schedule PTCCH */
     return;
  }

  /* Check if we need to set dl block start for PTCCH */
  if(gpl1_gprs_ptcch_data_ptr->ptcch_dl_blocks_left != GPL1_INVALID)
  {
    gpl1_gprs_ptcch_data_ptr->ptcch_dl_blk_start_cnt = 1;
  }

  /* Check if we need to schedule a DL PTCCH
   * If we do then make sure that this PTCCH/D will be valid when we decode it,
   * meaning have we previously scheduled a PTCCH/U
   */

  if( (((frame_counters[gas_id].FNmod416 + 2)%416) ==
       ptcch_dl_lookup[gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.downlink_tai])
       && (gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_dl_valid) )
  {
    /* Assign structure with pointers to metrics log and data buffers */
    for (i=0;i<GL1_DEFS_FRAMES_IN_BLK_PERIOD;i++)
    {
      gpl1_gprs_ptcch_data_ptr->ptcch_dl_data_info.metrics_data_mem[i] = &gpl1_gprs_ptcch_data_ptr->gpl1_gprs_dyn_ptcch_dl_metrics[i];
    }

    gpl1_gprs_ptcch_data_ptr->ptcch_dl_data_info.msg_data_mem = &gpl1_gprs_ptcch_data_ptr->gpl1_gprs_dyn_ptcch_dl_data;

    /* Update the camp timeslot */
    gl1_msg_pdch_set_camp_tn( (gl1_defs_tn_type) l1_get_current_timeslot(gas_id),gas_id );

    gl1_msg_rx_ptcch(l1_get_gprs_arfcns(gas_id),
                     &transfer_data_ptr->packet_transfer_agc,
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
                     &transfer_data_ptr->packet_transfer_div_agc,
#endif
                     &gpl1_gprs_ptcch_data_ptr->ptcch_dl_data_info,
                     gas_id);


    /* DL BLOCK start for PACCH or PDTCH to be set in this frame */
    gpl1_gprs_ptcch_data_ptr->ptcch_dl_blk_start_cnt = 1;

    gpl1_gprs_ptcch_data_ptr->ptcch_dl_blocks_left = 4;

  }  /* End of if */
}


/*===========================================================================

FUNCTION  gpl1_gprs_update_ptcch_ftn

DESCRIPTION
  This function updates PTCCH parameters for frame tick notification


DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_update_ptcch_ftn (gl1_msg_frame_tick_type  *ftn, gas_id_t gas_id)
{
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
  gpl1_gprs_ptcch_data_t *gpl1_gprs_ptcch_data_ptr = &gpl1_gprs_ptcch_data[gas_id];

  if (gpl1_gprs_ptcch_data_ptr->ptcch_ul_blk_start_cnt == 0)
  {
    ftn->next_frame_is_ul_ptcch_sacch_block_start = TRUE;

    /* set counter to invalid as block start has elapsed */
    gpl1_gprs_ptcch_data_ptr->ptcch_ul_blk_start_cnt = GPL1_INVALID;
  }
  else
    ftn->next_frame_is_ul_ptcch_sacch_block_start = FALSE;

  if (gpl1_gprs_ptcch_data_ptr->ptcch_dl_blk_start_cnt == 0)
  {
    /* decrement number of DL PTCCH blocks remaining */
    if(gpl1_gprs_ptcch_data_ptr->ptcch_dl_blocks_left)
    {
      ftn->next_frame_is_dl_ptcch_sacch_block_start = TRUE;

      /* set counter to invalid as block start has elapsed */
      gpl1_gprs_ptcch_data_ptr->ptcch_dl_blk_start_cnt = GPL1_INVALID;

      gpl1_gprs_ptcch_data_ptr->ptcch_dl_blocks_left--;
    }

    /* if there are no more DL PTCCH blocks left to decode
     * set to invalid
     */

    if(!(gpl1_gprs_ptcch_data_ptr->ptcch_dl_blocks_left))
      gpl1_gprs_ptcch_data_ptr->ptcch_dl_blocks_left = GPL1_INVALID;
  }
  else
    ftn->next_frame_is_dl_ptcch_sacch_block_start = FALSE;


  /*  Set-up for interference measurements, if frame is ptcch and being used
      for continuous time alignment mark timeslot.  */
  if (gpl1_gprs_ptcch_data_ptr->ptcch_ul_blk_start_cnt == 1)
  {
    /*  Then this frame is being used for ul ptcch  */
    gprs_serv_cell_meas_ptr->ta_meas_in_prog |= UL_PTCCH; /*lint !e655 bit-wise operation uses enums */

  }

  if (gpl1_gprs_ptcch_data_ptr->ptcch_dl_blk_start_cnt == 1)
  {
    /*  If the ta_meas_in_prog flag is already set do not modify here, wait until
        clear comming into this function. */
    /*  Then this frame is being used for dl ptcch  */
    gprs_serv_cell_meas_ptr->ta_meas_in_prog |= DL_PTCCH; /*lint !e655 bit-wise operation uses enums */

  }

  /*
   * Decrement all block start counters
   */

  if(gpl1_gprs_ptcch_data_ptr->ptcch_ul_blk_start_cnt != GPL1_INVALID)
     gpl1_gprs_ptcch_data_ptr->ptcch_ul_blk_start_cnt--;

  if(gpl1_gprs_ptcch_data_ptr->ptcch_dl_blk_start_cnt != GPL1_INVALID)
     gpl1_gprs_ptcch_data_ptr->ptcch_dl_blk_start_cnt--;

  return;
}

/*===========================================================================

FUNCTION  gpl1_gprs_calc_ptcch_tn_clash

DESCRIPTION
  This function assesses whether there is sufficient gap for ptcch
  timeslot allocation.


DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean gpl1_gprs_calc_ptcch_tn_clash( uint8  ptcch_tn,
                                       uint8  lowest_dl_timeslot, gas_id_t gas_id )
{
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
  uint8    highest_tn;
  boolean  invalid_tn = FALSE;

  /* assign lowest and highest tn's from current allocation */
  highest_tn = lowest_dl_timeslot + gprs_serv_cell_meas_ptr->highest_tn;

  /* validate ptcch tn as per GSM 05.02 sec 6.3.2.2.2 */
  if( ( ptcch_tn > highest_tn )
    ||( ptcch_tn < lowest_dl_timeslot ) )
  {
    invalid_tn = TRUE;
  }

  return(invalid_tn);
}

/*===========================================================================

FUNCTION  gpl1_gprs_timing_advance_valid

DESCRIPTION
  This function returns timing advance status


DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean gpl1_gprs_timing_advance_valid(gas_id_t gas_id)
{
  gpl1_gprs_ptcch_data_t *gpl1_gprs_ptcch_data_ptr = &gpl1_gprs_ptcch_data[gas_id];
#ifdef FEATURE_GSM_DTM
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  /* Use TAV from L1 SACCH header so no need to check
   * is we have a valid TAV before TX
   */
  if(l1_tskisr_blk->l1_state == L1_DTM_MODE)
  {
    return(TRUE);
  }else
  {
  return(gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ta_valid);
  }

#else
  return(gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ta_valid);
#endif
}

/*===========================================================================

FUNCTION  gpl1_gprs_find_next_tai_during_suspension

DESCRIPTION
  The function search for timing advance event during
  tbf suspension


DEPENDENCIES


RETURN VALUE
  boolean  allowed_to_go.

SIDE EFFECTS
  None

===========================================================================*/
boolean  gpl1_gprs_find_next_tai_during_suspension( uint32  tbf_suspend_start,
                                                    uint32  tbf_suspend_end, gas_id_t gas_id )
{
  gpl1_gprs_ptcch_data_t *gpl1_gprs_ptcch_data_ptr = &gpl1_gprs_ptcch_data[gas_id];
  boolean  allowed_to_go = TRUE;

  uint32 next_dl_tai_fn, next_ul_tai_fn;
  uint8  ul_tai = gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.global_ta_params->tai;
  uint8  i = 0;

  /* test if we have are allocated Cont. TA params */
  if (gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.global_ta_params->present)
  {
    /* work out next absoloute FN when ul and dl ptcch is to be scheduled */
    if (ptcch_ul_lookup[ul_tai] > (tbf_suspend_start % 416))
    {
      next_ul_tai_fn =
            ((tbf_suspend_start + (ptcch_ul_lookup[ul_tai] - (tbf_suspend_start % 416)) ) % FRAMES_IN_HYPERFRAME);
    }else
    {
      next_ul_tai_fn =
            ((tbf_suspend_start + 416 - ((tbf_suspend_start % 416) - ptcch_ul_lookup[ul_tai])) % FRAMES_IN_HYPERFRAME);
    }

    if (ptcch_dl_lookup[gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.downlink_tai] > (tbf_suspend_start % 416))
    {
      next_dl_tai_fn =
            ((tbf_suspend_start + (ptcch_dl_lookup[gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.downlink_tai] - (tbf_suspend_start % 416)) ) % FRAMES_IN_HYPERFRAME);
    }else
    {
      /* Before assigning next PTCCH/D frame number in the future, check we are not currently
       * in between PTCCH/D bursts.
       */
      if (((tbf_suspend_start % 416) - ptcch_dl_lookup[gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.downlink_tai])  <= (3 * 26))
      {
         /* Assign PTCCH/D to previous dl tai frame number */
         next_dl_tai_fn =
               SUB_FN(tbf_suspend_start,((tbf_suspend_start % 416) - ptcch_dl_lookup[gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.downlink_tai] ));
      }else
      {
         next_dl_tai_fn =
               ((tbf_suspend_start + 416 - ((tbf_suspend_start % 416) - ptcch_dl_lookup[gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.downlink_tai])) % FRAMES_IN_HYPERFRAME);
      }

    }


    /* Scan for ptcch/d channel
     * Since there are 4 attempts to decode PTCCH/D channel let
     * suspension requests go through if this is not our last attempt to
     * decode PTCCH/D
     */
    if(gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_decode_attempts >=3)
    {
      while((i<4) && (allowed_to_go) &&(gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_dl_valid))
      {
        if ((!gpl1_gprs_is_frame_num_later((next_dl_tai_fn + (i * 26)),(tbf_suspend_end+4))) &&
            (gpl1_gprs_is_frame_num_later((next_dl_tai_fn + (i * 26)),tbf_suspend_start)))
        {
          /* Abort the Suspension */
          allowed_to_go = FALSE;
          MSG_GERAN_MED_3_G("PTCCH/D found abort BCCH [%d] tai %d end %d",gl1_get_FN( gas_id ),ptcch_dl_lookup[gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.downlink_tai],(tbf_suspend_end + 4));
        }
        i++;
      }

      /* scan for ptcch/u channel */
      if (!gpl1_gprs_is_frame_num_later(next_ul_tai_fn,tbf_suspend_end + 4))
      {
        /* Abort the Suspension */
        allowed_to_go = FALSE;
        MSG_GERAN_MED_3_G("PTCCH/U found abort BCCH [%d] tai %d end %d",gl1_get_FN( gas_id ),ptcch_ul_lookup[ul_tai],(tbf_suspend_end + 4));
      }
    }
    else
    {

      /* scan for ptcch/u channel
       * If found abort the suspension as not Txing the PTCCH/U can result in the
       * PTCCH/D been turned off, hence PTCCH/D CRC failures
       */
      if (!gpl1_gprs_is_frame_num_later(next_ul_tai_fn,tbf_suspend_end + 4))
      {
        allowed_to_go = FALSE;
        MSG_GERAN_ERROR_3_G("PTCCH/U found abort BCCH [%d] tai %d end %d",gl1_get_FN( gas_id ),ptcch_ul_lookup[ul_tai],(tbf_suspend_end + 4));
      }

      /* Check if there is a PTCCH/D that needs to be scheduled in the suspension window */
      while((i<4) && (allowed_to_go) &&(gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_dl_valid))
      {
        if ((!gpl1_gprs_is_frame_num_later((next_dl_tai_fn + (i * 26)),(tbf_suspend_end+4))) &&
            (gpl1_gprs_is_frame_num_later((next_dl_tai_fn + (i * 26)),tbf_suspend_start)))
        {
          if(allowed_to_go == TRUE)
          {
            /** Should only get here if a PTCCH/U was not found but a PTCCH/D was found in the suspension window.
             ** In which case allow the Suspension to proceed
             **/

            /* Allow the BCCH to be scheduled and increment the PTCCH/D to the next occurance */
            gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_decode_attempts++;

            /* decode failed so attempt to read next DL PTCCH message */
            gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.downlink_tai+=4;

            /* wrap arround if greater than dl tai lookup table */
            if (gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.downlink_tai > 15)
            {
              gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.downlink_tai -= 16;
            }

            /* Abort the PTCCH/D in case a number of PTCCH/D bursts have already been Rxd */
            gl1_msg_abort_dl_ptcch(gas_id);


            MSG_GERAN_MED_3_G("PTCCH/D found Allow BCCH [%d] tai %d end %d",gl1_get_FN( gas_id ),ptcch_dl_lookup[gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.downlink_tai],(tbf_suspend_end + 4));

          }else
          {
            /* Should only get here if a PTCCH/U and a PTCCH/D is found in the suspension window.
             * in this case abort the suspension
             */
            MSG_GERAN_MED_3_G("PTCCH/D found abort BCCH [%d] tai %d end %d",gl1_get_FN( gas_id ),ptcch_dl_lookup[gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.downlink_tai],(tbf_suspend_end + 4));
          }

        }

        i++;
      } /* end of while ((i<4.. */

    }

  }

  /* Return the allowed_to_go flag to client */
  return  allowed_to_go;
}





/*===========================================================================

FUNCTION  gpl1_gprs_get_global_ta_timeslot

DESCRIPTION
  This function returns global timing advance timeslot


DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
uint8 gpl1_gprs_get_global_ta_timeslot( gas_id_t gas_id )
{
  gpl1_gprs_ptcch_data_t *gpl1_gprs_ptcch_data_ptr = &gpl1_gprs_ptcch_data[gas_id];
  uint8  ta_timeslot = GPL1_INVALID;

  if( NULL != gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.global_ta_params )
  {
    ta_timeslot = gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.global_ta_params->ta_timeslot;
  }

  return  ta_timeslot;
}

/*===========================================================================

FUNCTION  gpl1_gprs_get_ptcch_snr_status

DESCRIPTION
  This function return ptcch snr status


DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean gpl1_gprs_get_ptcch_snr_status( gas_id_t gas_id )
{
  gpl1_gprs_ptcch_data_t *gpl1_gprs_ptcch_data_ptr = &gpl1_gprs_ptcch_data[gas_id];
  return(gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_snr_bad);
}

/*===========================================================================

FUNCTION  gpl1_gprs_get_ptcch_snr_valid

DESCRIPTION
  This function return ptcch snr status


DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean gpl1_gprs_get_ptcch_snr_valid( gas_id_t gas_id )
{
  gpl1_gprs_ptcch_data_t *gpl1_gprs_ptcch_data_ptr = &gpl1_gprs_ptcch_data[gas_id];
  return(gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_snr_bad_valid);
}


/*===========================================================================

FUNCTION  gpl1_gprs_reset_ptcch_snr_valid

DESCRIPTION
  This function sets ptcch snr status to FALSE


DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_reset_ptcch_snr_valid( gas_id_t gas_id )
{
  gpl1_gprs_ptcch_data_t *gpl1_gprs_ptcch_data_ptr = &gpl1_gprs_ptcch_data[gas_id];
  gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_snr_bad_valid = FALSE;
}

/*===========================================================================

FUNCTION  gpl1_gprs_set_lowest_dl_timeslot

DESCRIPTION
 This function informs the ptcch module of a change
 in the lowest downlink timeslot and allows the module
 to update the relative timeslot used to task NPL1.
 This function conveys this information to any active
 NPL1 handlers.

DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  gpl1_gprs_ptcch_set_lowest_dl_timeslot( uint8  timeslot,gas_id_t gas_id )
{
  gpl1_gprs_ptcch_data_t *gpl1_gprs_ptcch_data_ptr = &gpl1_gprs_ptcch_data[gas_id];
  if( NULL != gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.global_ta_params )
  {
    uint8  ptcch_absolute_timeslot = gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.global_ta_params->ta_timeslot;

    /* Tell NPL1 the value of the ptcch timeslot relative to the new
     * current timeslot. This is necessary to update currently active
     * ptcch handlers.
     */
    if(  ( GPL1_INVALID != ptcch_absolute_timeslot )
       &&( ptcch_absolute_timeslot >= timeslot ) )
    {
        gl1_msg_update_dl_ptcch_tn( (gl1_defs_tn_type)(ptcch_absolute_timeslot - timeslot), gas_id );
    }

  }
}

/*===========================================================================

FUNCTION  gpl1_gprs_ptcch_reconfig

DESCRIPTION
 This function informs the ptcch module of a change
 in the lowest downlink timeslot and allows the module
 to update the relative timeslot used to task NPL1.

DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  gpl1_gprs_ptcch_reconfig( gas_id_t gas_id )
{
  gpl1_gprs_ptcch_data_t *gpl1_gprs_ptcch_data_ptr = &gpl1_gprs_ptcch_data[gas_id];
    /* Make sure future ptcch scheduling is aware of
     * of the new lowest dl timeslot.
     */
    gpl1_gprs_ptcch_data_ptr->gpl1_gprs_ta_info.ptcch_reconfig = TRUE;
}


#ifdef FEATURE_GPRS_GBTA
/*===========================================================================

FUNCTION  gpl1_gprs_is_ptcch_active

DESCRIPTION
 Check if DL or UL PTCCH is active

DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean gpl1_gprs_is_ptcch_active(gas_id_t gas_id)
{
  return ( gprs_serv_cell_meas[gas_id].ta_meas_in_prog != NO_PTCCH);
}
#endif /* FEATURE_GPRS_GBTA */

