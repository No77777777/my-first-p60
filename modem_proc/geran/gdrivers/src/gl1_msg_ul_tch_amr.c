/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

    G L 1  M E S S A G E  L A Y E R -  T R A F F I C   C H A N N E L

GENERAL DESCRIPTION
    This module contains the procedures used for communicating on the ul TCH.
    The procedures used herein are intended for use by the
    portable layer 1 software.

EXTERNALIZED FUNCTIONS
    gl1_msg_vocoder_tx_amr_fr
    gl1_msg_tx_tch_amr_fr
    gl1_msg_tx_facch_amr_fr
    gl1_msg_tx_ratscch_amr_fr
    gl1_msg_tx_sid_first_amr_fr
    gl1_msg_tx_sid_update_amr_fr
    gl1_msg_tx_saved_sid_update_amr_fr
    gl1_msg_tx_save_sid_update_amr_fr_hr
    gl1_msg_abort_tx_tch_amr_fr
    gl1_msg_cfg_ul_tch_amr_fr_hr
    gl1_msg_cfg_ul_facch_amr_fr_hr
    gl1_msg_cfg_ul_ratscch_amr_fr_hr
    gl1_msg_cfg_ul_sid_first_amr_fr_hr
    gl1_msg_cfg_ul_sid_update_amr_fr_hr
    gl1_msg_vocoder_tx_amr_hr
    gl1_msg_tx_tch_amr_hr
    gl1_msg_tx_facch_amr_hr
    gl1_msg_tx_ratscch_amr_hr
    gl1_msg_tx_sid_first_amr_hr
    gl1_msg_tx_sid_update_amr_hr
    gl1_msg_tx_saved_sid_update_amr_hr
    gl1_msg_abort_tx_tch_amr_hr

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

   EDIT HISTORY FOR MODULE

   This section contains comments describing changes made to the module.
   Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_msg_ul_tch_amr.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who      what, where, why
--------   ---      ---------------------------------------------------------
14/08/14    ws      CR889273 Fix Static declarations of variables which are not Dual data spaced
11/05/15   pjr      CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
08/04/15   pjr      CR818535 FR25951 Changes to support Single Voice Session 
08/04/14   jk       CR649527:VAMOS2 Fixes for Freqeucny hopping and DTX ON cases
02/01/14   jk       CR595130: GL1 changes to support VAMOS-II
22/01/13   ws       CR602047 - Allow MVS/GHDI to operate on SUB2 if VS driver is not present
07/08/13   jk       CR490807:Support for shifted SACCH
11/11/13   ws       CR 573757 Featurise GSMTR-1 registers and Voice services driver
15/08/13   sk       CR524039 GBTA bringup changes
07/16/13   zc       CR470197 Replace memcpy with memscpy
21/05/13   cs       TSTS Updates
03/04/13   pjr      CR470940 included gl1_hw_vs_g.h
31/03/13   ws       CR466973 - Triton DSDS BU: Fixed stability issues with VS driver
07/03/13   dv       CR461080: Introduction of new GSM VS interface
13/03/13   mc       CR459487 : COEX Support - GERAN arbitrator framework
11/06/12   pg       CR357951: Avoid infinite loop on aborting AMR HR and clean up
13/04/12   pg       CR350709: Unbalanced vocoder mutex lock/unlock pair
16/12/11   ws       Free floating API change - provide API for NAS GSVOICE to copy vocoder
                    data from/to GL1 to MVS structures
07/09/11   ap       CR300821 Counter values are wrapping
26/04/11   ws       Removed deprecated header voc.h
17/01/10   ab       Updated - Resolve Klocwork Buffer Overflow errors
14/01/11   ab       Resolve Klocwork Buffer Overflow errors
02/08/10   tjw      Removed unused clk.h (CMI phase 1 re-clean up)
08/01/10   dv       CR222761 - Uplink SID_UPDATE frame not saved properly
20/08/09   dv       CR193404 - CR191821 breaks AMR-NB functionality
05/08/09   ws       Removed FEATURE_GSM_MDSP_DTM it's now mainlined
04/08/09   dv       CR191821 - 8K AMR-WB L1 Modem Changes for GSM
28/07/98   hv       Added KxMutex support
13/06/08   ws       swap Endian for 8K targets that support QDSP6 Vocoder
12/05/08   dv       Added FEATURE_GSM_AMR_WB - L1 component
20/04/07   cs       CR117035 Report HR DTX correctly for SACCH Measurement reports
11/07/05   gfr      QLint cleanup
08/12/05   gfr      Move global state parameters into the state machines,
                    rework aborting to make more sense
08/10/05   gfr      Use exported mDSP defines for vocoder encoding word.
07/29/05   gfr      DTM support
06/09/05   dp       Changes to uplink aborts to support calling FTN before
                    aborts.  Fixed bug in aborting TCH AMR and channels with
                    middle SACCH block
05/05/05   yh       Code clean up
05/04/05   gfr      Fix bug using wrong counter for non-TCH channels
04/21/05   yh       Support for panic reset
04/12/05   gfr      Consolidated GSM hw layer parameters
04/08/05   gfr      Combined GSM and GPRS channel types
01/12/05   yhong    Reset amr_tx_tch_current and amr_tx_tch_next when aborting
01/05/05   dp       Merged audio optimizations to mainline
12/20/04   gfr      Remove handlers after abort
11/01/04   yhong    Abort function assumes gl1_msg_frame_tick_notification() is
                    called after the abort.
08/18/04   yhong    Initial version. Contains only the uplink functions taken
                    from gl1_msg_tch_amr.c
===========================================================================*/

/*===========================================================================

    INCLUDE FILES FOR MODULE

===========================================================================*/

#include "geran_variation.h"
#include "comdef.h"

#ifdef CUST_H
#include "customer.h"
#endif

#if  defined (FEATURE_GSM_AMR)

#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "gl1_hwi.h"
#include "gl1_msg_g.h"
#include "gl1_msgi.h"
#include "msg.h"
#include "gtmrs.h"
#include "gtmrs_g.h"


#include "geran_dual_sim.h"
#include "geran_multi_sim.h"

/* We need this because we hack certain control bits in the encoded
   data directly instead of using a proper layering interface */
#include "mdsp_intf.h"
#include "mdsp_intf_g.h"

#include "sys_cnst.h"
#include "gl1_msgi_tch_amr.h"

#include <string.h>
#include "gl1_mutex.h"
#include <stringl/stringl.h>


/*****************************************************

 LOCAL STORAGE

******************************************************/

/* Number of messages to send or receive */
/* This number will be 0,1, or 2         */
static uint8    amr_tx_tch_message_counter[NUM_GERAN_DATA_SPACES] =
                     { INITIAL_VALUE( 0 ) };

static uint8    amr_tx_message_counter[NUM_GERAN_DATA_SPACES] =
                     { INITIAL_VALUE( 0 ) };


/* Configured Callback pointers  */
static gl1_msg_tx_cb_type   amr_ul_tch_callback[NUM_GERAN_DATA_SPACES] =
                                             {INITIAL_VALUE_S(0)};

static gl1_msg_tx_cb_type   amr_ul_facch_callback[NUM_GERAN_DATA_SPACES] =
                                             {INITIAL_VALUE_S(0)};

static gl1_msg_tx_cb_type   amr_ul_ratscch_callback[NUM_GERAN_DATA_SPACES] =
                                             {INITIAL_VALUE_S(0)};

static gl1_msg_tx_cb_type   amr_ul_sid_first_callback[NUM_GERAN_DATA_SPACES] =
                                             {INITIAL_VALUE_S(0)};

static gl1_msg_tx_cb_type   amr_ul_sid_update_callback[NUM_GERAN_DATA_SPACES] =
                                             {INITIAL_VALUE_S(0)};

static boolean       amr_hr_middle_facch_block[NUM_GERAN_DATA_SPACES] =
                            { INITIAL_VALUE( FALSE ) };

static boolean       amr_hr_sacch_in_middle[NUM_GERAN_DATA_SPACES] =
                            { INITIAL_VALUE( FALSE ) };

#ifdef FEATURE_VAMOS_II
static boolean       amr_middle_facch_hr_block_vamos2[NUM_GERAN_DATA_SPACES] =
                            { INITIAL_VALUE( FALSE ) };
#endif

extern gl1_defs_loopback_type loopback_type[NUM_GERAN_DATA_SPACES];
extern uint8 gl1_loopback_data[NUM_GERAN_DATA_SPACES][MAX_LOOPBACK_OCTETS];

uint8 amr_tx_sid_update_data[NUM_GERAN_DATA_SPACES][MAX_SID_UPDATE_OCTETS];

extern boolean gl1_dtx_used[NUM_GERAN_DATA_SPACES];
#ifdef FEATURE_GSM_AMR_WB
static boolean ul_wb_amr_flag[NUM_GERAN_DATA_SPACES] =
                            { INITIAL_VALUE( FALSE ) };
#endif
static gl1_hw_channel_type ul_amr_channel_type[NUM_GERAN_DATA_SPACES] =
                            { INITIAL_VALUE( GL1_HW_TCH_AFS ) };

/* Block types */
typedef enum
{
   GL1_MSG_TX_TCH_BLOCK_UNDEFINED,
   GL1_MSG_TX_TCH_BLOCK_TCH,
   GL1_MSG_TX_TCH_BLOCK_FACCH,
   GL1_MSG_TX_TCH_BLOCK_RATSCCH,
   GL1_MSG_TX_TCH_BLOCK_SID_FIRST,
   GL1_MSG_TX_TCH_BLOCK_SID_UPDATE
} gl1_msg_tx_tch_amr_block_type;

/* Store the local frames */
typedef struct
{
    gl1_hw_channel_type           channel_type;
    gl1_msg_tx_cb_type            amr_ul_callback;
    gl1_msg_tx_tch_amr_block_type block;

    /* State variables */
    boolean                     active;
    uint8                       burst_num;
    uint8                       frame_count;

    /* Frame layer parameters */
    ARFCN_T                     ARFCN[GL1_DEFS_FRAMES_IN_BLK_PERIOD];
    int16                       power_index[GL1_DEFS_FRAMES_IN_BLK_PERIOD];
    gl1_hw_tx_signal_type       signal_info;
    gl1_hw_tx_burst_type        burst_info;
    gl1_hw_tx_tags_type         tags;
} amr_tx_data_storage_type;

#define NUM_AMR_TX_STORAGE 4
/* Local Storage */
static amr_tx_data_storage_type amr_tx_data_storage[NUM_GERAN_DATA_SPACES][NUM_AMR_TX_STORAGE];
static uint8 amr_tx_tch_current[NUM_GERAN_DATA_SPACES] =
               { INITIAL_VALUE( 0 ) };

static uint8 amr_tx_tch_next[NUM_GERAN_DATA_SPACES] =
               { INITIAL_VALUE( 0 ) };

static uint8 amr_tx_TSC[NUM_GERAN_DATA_SPACES] =
               { INITIAL_VALUE( 0 ) };

static uint8 amr_tx_data_storage_size[NUM_GERAN_DATA_SPACES] =
               { INITIAL_VALUE( 0 ) };

#ifdef FEATURE_VAMOS_II
static gl1_vamos2_tch_wait_type vamos2_ul_amr_tch_wait_state [NUM_GERAN_DATA_SPACES] =
               { INITIAL_VALUE( NO_WAIT ) };
#endif
/*****************************************************

 LOCAL FUNCTION PROTOTYPES

******************************************************/

static boolean amr_tx_handler_fr( boolean start_of_block,
                                  uint32  time_stamp,gas_id_t gas_id);


static boolean amr_tx_handler_hr( boolean start_of_block,
                                 uint32  time_stamp,gas_id_t gas_id );
static boolean amr_tch_tx_handler_hr( boolean start_of_block,
                                      uint32  time_stamp,gas_id_t gas_id );

/*===========================================================================
FUNCTION gl1_msgi_init_ul_tch_amr

DESCRIPTION
   This function initializes data structures used by uplink TCH AMR.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msgi_init_ul_tch_amr(gas_id_t gas_id)
{
   uint8 i = 0;

   amr_tx_tch_message_counter[gas_id]                           = 0;
   amr_tx_message_counter[gas_id]                               = 0;

   amr_ul_tch_callback[gas_id].data_cb                          = NULL;
   amr_ul_tch_callback[gas_id].metrics_cb                       = NULL;

   amr_ul_facch_callback[gas_id].data_cb                        = NULL;
   amr_ul_facch_callback[gas_id].metrics_cb                     = NULL;

   amr_ul_ratscch_callback[gas_id].data_cb                      = NULL;
   amr_ul_ratscch_callback[gas_id].metrics_cb                   = NULL;

   amr_ul_sid_first_callback[gas_id].data_cb                    = NULL;
   amr_ul_sid_first_callback[gas_id].metrics_cb                 = NULL;

   amr_ul_sid_update_callback[gas_id].data_cb                   = NULL;
   amr_ul_sid_update_callback[gas_id].metrics_cb                = NULL;

   for (i = 0; i < NUM_AMR_TX_STORAGE; i++)
   {
      amr_tx_data_storage[gas_id][i].amr_ul_callback.data_cb    = NULL;
      amr_tx_data_storage[gas_id][i].amr_ul_callback.metrics_cb = NULL;
   }

   amr_tx_tch_current[gas_id]               = 0;
   amr_tx_tch_next[gas_id]                  = 0;
   amr_tx_data_storage_size[gas_id]         = 0;
}



/*===========================================================================
FUNCTION ul_tch_amr_add_storage

DESCRIPTION
   Allocate and fill in the next storage unit for AMR uplink.

===========================================================================*/
static void ul_tch_amr_add_storage
(
   ARFCN_T                       ARFCN[GL1_DEFS_FRAMES_IN_BLK_PERIOD],
   int16                         TXLEV[GL1_DEFS_FRAMES_IN_BLK_PERIOD],
   gl1_hw_channel_type           channel_type,
   gl1_msg_tx_tch_amr_block_type block_type,
   gl1_msg_tx_cb_type            amr_ul_callback,
   gas_id_t                      gas_id
)
{
   int16 TX_PWR_IDX[GL1_DEFS_FRAMES_IN_BLK_PERIOD];
   uint8 i;
   amr_tx_data_storage_type *storage;

   /* Sanity - we are probably dead if this happens */
   if (amr_tx_data_storage_size[gas_id] == NUM_AMR_TX_STORAGE)
   {
      MSG_GERAN_ERROR_0_G("AMR TX storage full!");
      return;
   }

   /* Shortcut */
   storage = &amr_tx_data_storage[gas_id][amr_tx_tch_next[gas_id]];

   /* Translate GSM TXLEV to HW power index. ARFCN[0] sufficient since
   ** the MESSAGE (4 blocks) will be transmitted on the same band.  */
   gl1_msgi_map_txlev_to_pwr_idx(ARFCN[0], TXLEV, TX_PWR_IDX, 4, gas_id);

   /* Add to storage */
   for (i = 0; i < GL1_DEFS_FRAMES_IN_BLK_PERIOD; i++)
   {
     storage->ARFCN[i] = ARFCN[i];
     storage->power_index[i] = TX_PWR_IDX[i];
   }

   storage->channel_type = channel_type;
   storage->block = block_type;
   storage->amr_ul_callback = amr_ul_callback;

   storage->active = FALSE;
   storage->frame_count = 0;
   storage->burst_num = 0;

   /* Fill in static frame layer parameters */
   memset(&storage->signal_info, 0, sizeof(storage->signal_info));
   memset(&storage->burst_info, 0, sizeof(storage->burst_info));
   storage->burst_info.tsc            = amr_tx_TSC[gas_id];
   storage->burst_info.bsic           = NO_BSIC;
   storage->burst_info.timing_advance = gl1_msg_retrieve_ta(gas_id);
   storage->burst_info.channel_type   = channel_type;
   storage->signal_info.tn            = gl1_msg_get_cs_timeslot(gas_id);

   amr_tx_tch_next[gas_id] = (amr_tx_tch_next[gas_id] + 1) % NUM_AMR_TX_STORAGE;
   amr_tx_data_storage_size[gas_id]++;
}



/*===========================================================================

FUNCTION ul_tch_amr_abort_current

DESCRIPTION
   This function is used to attempt to abort the currently active
   state machine.  It assumes that only one state machine is active and
   will cleanup everything if the abort succeeds.

===========================================================================*/
static boolean ul_tch_amr_abort_current (boolean full_rate,gas_id_t gas_id)
{
   uint8 i;

   /* If the current block is TCH or FACCH and it has already started txing
      we let it finish */
   if ((amr_tx_data_storage[gas_id][amr_tx_tch_current[gas_id]].block == GL1_MSG_TX_TCH_BLOCK_TCH ||
        amr_tx_data_storage[gas_id][amr_tx_tch_current[gas_id]].block == GL1_MSG_TX_TCH_BLOCK_FACCH) &&
       amr_tx_data_storage[gas_id][amr_tx_tch_current[gas_id]].active)
   {
      return FALSE;
   }

   MSG_4(MSG_SSID_DFLT, MSG_LEGACY_HIGH,
         "Aborting AMR TCH: type=%d, fr=%d, burst=%d, FN=%d",
         amr_tx_data_storage[gas_id][amr_tx_tch_current[gas_id]].block, full_rate,
         amr_tx_data_storage[gas_id][amr_tx_tch_current[gas_id]].burst_num, GSTMR_GET_FN_GERAN(gas_id));

   /* Inform the frame layer we are aborting this transmit */
   (void) gl1_hw_abort_tx_bursts(&amr_tx_data_storage[gas_id][amr_tx_tch_current[gas_id]].tags,gas_id);

   /* Clean up all state machines just to be safe */
   for (i = 0; i < NUM_AMR_TX_STORAGE; i++)
   {
      amr_tx_data_storage[gas_id][i].block = GL1_MSG_TX_TCH_BLOCK_UNDEFINED;
   }
   amr_tx_tch_current[gas_id] = 0;
   amr_tx_tch_next[gas_id] = 0;
   amr_tx_data_storage_size[gas_id] = 0;
   amr_hr_sacch_in_middle[gas_id] = FALSE;

   /* Check which handler needs to be removed */
   if (amr_tx_message_counter[gas_id] > 0)
   {
      if (full_rate)
      {
         gl1_msgi_remove_ft_handler(amr_tx_handler_fr,gas_id);
      }
      else
      {
         gl1_msgi_remove_ft_handler(amr_tx_handler_hr,gas_id);
      }
      amr_tx_message_counter[gas_id] = 0;
   }

   /* This counter only used for TCH HR */
   if (amr_tx_tch_message_counter[gas_id] > 0)
   {
      gl1_msgi_remove_ft_handler(amr_tch_tx_handler_hr,gas_id);
      amr_tx_tch_message_counter[gas_id] = 0;
   }

   /* Return that aborting is complete */
   return TRUE;
}



/*===========================================================================
FUNCTION gl1_msg_tx_tch_amr_fr

DESCRIPTION
   This function is used to transmit a message on the TCH on the next
   block boundary.  The message contents should span 4 frames ie 1 block.
   This function may be called at task level but should be called in the block
   before the data is to transmitted.   If data to be transmitted is for
   frame #4,5,6,7, then this function should be completed before start of
   frame #3, ie called in frames 0,1,2.

DEPENDENCIES
   gl1_msg_cfg_ul_tch() and gl1_msg_store_ta(), must have been called at some time prior.

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
void gl1_msg_tx_tch_amr_fr( ARFCN_T ARFCN[4], int16  TXLEV[4],
                            uint8  *data, boolean onset_flag,
                            uint8 curr_ul_cmi, uint8 curr_dl_cmi,
                            uint8 curr_cmr, uint8 curr_cmc,
                            gl1_msg_amr_mode_type curr_ul_codec_mode,
                            uint8 phase,
#ifdef FEATURE_VAMOS_II
                            gl1_vamos2_tch_wait_type vamos2_tch_wait_state,
#endif
                            gas_id_t gas_id)
{
   static gl1_msg_amr_mode_type gl1_msg_loopback_I_prev_codec_mode[NUM_GERAN_DATA_SPACES];
   static uint8                 gl1_msg_loopback_I_prev_cmr[NUM_GERAN_DATA_SPACES];
   static uint8                 gl1_msg_loopback_I_prev_cmi[NUM_GERAN_DATA_SPACES];

   if (!data || !ARFCN)
   {
      MSG_GERAN_ERROR_2_G("Rcvd NULL data %d or ARFCN %d pointers",
                data, ARFCN);
      return;
   }

   if ( loopback_type[gas_id] == GL1_DEFS_LOOPBACK_TYPE_A ||
        loopback_type[gas_id] == GL1_DEFS_LOOPBACK_TYPE_B )
   {
      /*
       * As per specs: All uplink in band signalling codewords carry CMI in
       * case of loopbacks A and B(??)
       */
      *gl1_loopback_data[gas_id]     = 0;
      *(gl1_loopback_data[gas_id]+1) = (uint8)((curr_ul_cmi << 6) |
                                       (curr_ul_cmi << 4) |
                                       (phase << 3)       |
                                       (onset_flag << 2));
      *(gl1_loopback_data[gas_id]+2) = (uint8)curr_ul_codec_mode;
      *(gl1_loopback_data[gas_id]+3) = MDSP_AMR_VOC_FRAME_TYPE_SPEECH_GOOD;

      /* Overwrite passed in data pointer with loopback one */
      data = gl1_loopback_data[gas_id];
   }
   else if (loopback_type[gas_id] == GL1_DEFS_LOOPBACK_TYPE_I)
   {
      /*
       * If previous block was not a FACCH.
       */
      if (!gl1_msg_loopback_I_prev_block_was_facch[gas_id])
      {
         /*
          * Save UL CMR, UL CMI and codec mode, in case the next block is a FACCH
          * block. The spec says that D/L CMI should be looped back as U/L CMR;
          * D/L CMC should be looped back as U/L CMI. Also, the U/L codec
          * mode should be based on D/L CMC.
          */
         gl1_msg_loopback_I_prev_cmr[gas_id] = curr_dl_cmi;
         gl1_msg_loopback_I_prev_cmi[gas_id] = curr_cmc;
         gl1_msg_loopback_I_prev_codec_mode[gas_id] = curr_ul_codec_mode;
      }

      *gl1_loopback_data[gas_id] = 0;
      *(gl1_loopback_data[gas_id]+1) = (uint8)((gl1_msg_loopback_I_prev_cmr[gas_id] << 6) |
                                       (gl1_msg_loopback_I_prev_cmi[gas_id] << 4) |
                                       (phase << 3)                       |
                                       (onset_flag << 2));
      *(gl1_loopback_data[gas_id]+2) = (uint8)gl1_msg_loopback_I_prev_codec_mode[gas_id];
      *(gl1_loopback_data[gas_id]+3) = MDSP_AMR_VOC_FRAME_TYPE_SPEECH_GOOD;

      /* Overwrite passed in data pointer with loopback one */
      data = gl1_loopback_data[gas_id];
   }
   else
   {
      *data     = 0;
      *(data+1) = (uint8)((curr_cmr << 6)    |
                          (curr_ul_cmi << 4) |
                          (phase << 3)       |
                          (onset_flag << 2));
   }

   /* Write data to frame layer */
   gl1_hw_write_tx_data_amr(ul_amr_channel_type[gas_id],
                            data,
                            MAX_TCH_OCTETS_AMR,
                            gl1_msg_get_cs_timeslot(gas_id),
                            &amr_tx_data_storage[gas_id][amr_tx_tch_next[gas_id]].tags,gas_id);

   /* Add parameters to the next store */
   ul_tch_amr_add_storage(ARFCN, TXLEV,
                          ul_amr_channel_type[gas_id],
                          GL1_MSG_TX_TCH_BLOCK_TCH,
                          amr_ul_tch_callback[gas_id],gas_id);

#ifdef FEATURE_VAMOS_II
   vamos2_ul_amr_tch_wait_state[gas_id] = vamos2_tch_wait_state;
#endif

   /* register a handler if one does not exist */
   if (amr_tx_message_counter[gas_id] == 0)
   {
      gl1_msgi_add_ft_handler( amr_tx_handler_fr, GL1_MSG_FT_HDLR_UL,gas_id );
   }

   amr_tx_message_counter[gas_id]++;
}



/*===========================================================================
FUNCTION gl1_msg_tx_facch_amr_fr

DESCRIPTION
   This function is used to transmit a message on the FACCH on the next
   block boundary.  The message contents should span 4 frames ie 1 block.
   This function may be called at task level but should be called in the block
   before the data is to transmitted.   If data to be transmitted is for
   frame #4,5,6,7, then this function should be completed before start of
   frame #3, ie called in frames 0,1,2.

DEPENDENCIES
   gl1_msg_cfg_ul_tch() and gl1_msg_store_ta(), must have been called at some time prior.

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
void gl1_msg_tx_facch_amr_fr ( ARFCN_T ARFCN[4], int16  TXLEV[4],
                               uint8  *data, boolean onset_flag,
                               uint8 curr_ul_cmi, uint8 curr_cmr, uint8 phase,
#ifdef FEATURE_VAMOS_II
                               gl1_vamos2_tch_wait_type vamos2_tch_wait_state,
#endif
                               gas_id_t gas_id)
{
   gl1_hw_channel_type chan = GL1_HW_FACCH_AFS;

   if (!data || !ARFCN)
   {
      MSG_GERAN_ERROR_2_G("Rcvd NULL data %d or ARFCN %d pointers",
                data, ARFCN);
      return;
   }

   *data = 0;
   *(data+1) = (uint8)((curr_cmr << 6)    |
                       (curr_ul_cmi << 4) |
                       (phase << 3)       |
                       (onset_flag << 2));
#ifdef FEATURE_GSM_AMR_WB
   if(ul_wb_amr_flag[gas_id])
   {
       chan = GL1_HW_FACCH_WFS;
   }
#endif

   gl1_hw_write_tx_data_amr(chan,
                            data,
                            MAX_FACCH_OCTETS_AMR,
                            gl1_msg_get_cs_timeslot(gas_id),
                            &amr_tx_data_storage[gas_id][amr_tx_tch_next[gas_id]].tags,gas_id);

   /* Add parameters to the next store */
   ul_tch_amr_add_storage(ARFCN,
                          TXLEV,
                          chan,
                          GL1_MSG_TX_TCH_BLOCK_FACCH,
                          amr_ul_facch_callback[gas_id],gas_id);

#ifdef FEATURE_VAMOS_II
   vamos2_ul_amr_tch_wait_state[gas_id] = vamos2_tch_wait_state;
#endif

   /* register a handler if one does not exist */
   if (amr_tx_message_counter[gas_id] == 0)
   {
      gl1_msgi_add_ft_handler(amr_tx_handler_fr, GL1_MSG_FT_HDLR_UL,gas_id);
   }
   amr_tx_message_counter[gas_id]++;
}


/*===========================================================================
FUNCTION gl1_msg_tx_ratscch_amr_fr

DESCRIPTION
   This function is used to transmit a message on the RATSCCH on the next
   block boundary.  The message contents should span 4 frames ie 1 block.
   This function may be called at task level but should be called in the block
   before the data is to transmitted.   If data to be transmitted is for
   frame #4,5,6,7, then this function should be completed before start of
   frame #3, ie called in frames 0,1,2.

DEPENDENCIES
   gl1_msg_cfg_ul_tch() and gl1_msg_store_ta(), must have been called at some time prior.

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
void gl1_msg_tx_ratscch_amr_fr
(
   ARFCN_T ARFCN[4],
   int16  TXLEV[4],
   uint8  *data,
   boolean onset_flag,
   uint8 curr_ul_cmi,
   uint8 curr_cmr,
   uint8 phase,
#ifdef FEATURE_VAMOS_II
   gl1_vamos2_tch_wait_type vamos2_tch_wait_state,
#endif
   gas_id_t gas_id
)
{
   if (!data || !ARFCN)
   {
      MSG_GERAN_ERROR_2_G("Rcvd NULL data %d or ARFCN %d pointers",
                data, ARFCN);
      return;
   }

   *data = 0;
   *(data+1) = (uint8)((curr_cmr << 6)    |
                       (curr_ul_cmi << 4) |
                       (phase << 3)       |
                       (onset_flag << 2));
   *(data+2) = MDSP_AMR_VOC_CODEC_MODE_DONT_CARE;
   *(data+3) = MDSP_AMR_VOC_FRAME_TYPE_RATSCCH;

   gl1_hw_write_tx_data_amr(ul_amr_channel_type[gas_id],
                            data,
                            MAX_RATSCCH_OCTETS,
                            gl1_msg_get_cs_timeslot(gas_id),
                            &amr_tx_data_storage[gas_id][amr_tx_tch_next[gas_id]].tags,gas_id);

   /* Add parameters to the next store */
   ul_tch_amr_add_storage(ARFCN, TXLEV, ul_amr_channel_type[gas_id],
                          GL1_MSG_TX_TCH_BLOCK_RATSCCH,
                          amr_ul_ratscch_callback[gas_id],gas_id);

#ifdef FEATURE_VAMOS_II
   vamos2_ul_amr_tch_wait_state[gas_id] = vamos2_tch_wait_state;
#endif

   /* register a handler if one does not exist */
   if (amr_tx_message_counter[gas_id] == 0)
   {
      gl1_msgi_add_ft_handler(amr_tx_handler_fr, GL1_MSG_FT_HDLR_UL,gas_id);
   }
   amr_tx_message_counter[gas_id]++;

}


/*===========================================================================
FUNCTION gl1_msg_tx_sid_first_amr_fr

DESCRIPTION
   This function is used to transmit a message on the SID_FIRST on the next
   block boundary.  The message contents should span 4 frames ie 1 block.
   This function may be called at task level but should be called in the block
   before the data is to transmitted.   If data to be transmitted is for
   frame #4,5,6,7, then this function should be completed before start of
   frame #3, ie called in frames 0,1,2.

DEPENDENCIES
   gl1_msg_cfg_ul_tch() and gl1_msg_store_ta(), must have been called at some time prior.

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
void gl1_msg_tx_sid_first_amr_fr( ARFCN_T ARFCN[4], int16  TXLEV[4],
                                  uint8 * data,
                                  uint8 curr_ul_cmi,
                                  uint8 curr_cmr, uint8 phase,
#ifdef FEATURE_VAMOS_II
                                  gl1_vamos2_tch_wait_type vamos2_tch_wait_state,
#endif
                                  gas_id_t gas_id )
{
   if (!data || !ARFCN)
   {
      MSG_GERAN_ERROR_2_G("Rcvd NULL data %d or ARFCN %d pointers",
                data, ARFCN);
      return;
   }

   *data = 0;
   *(data+1) = (uint8)((curr_cmr << 6)   |
                       (curr_ul_cmi << 4)|
                       (phase << 3));
   *(data+2) = MDSP_AMR_VOC_CODEC_MODE_DONT_CARE;
   *(data+3) = MDSP_AMR_VOC_FRAME_TYPE_SID_FIRST;

   gl1_hw_write_tx_data_amr(ul_amr_channel_type[gas_id],
                            data,
                            MAX_SID_FIRST_OCTETS,
                            gl1_msg_get_cs_timeslot(gas_id),
                            &amr_tx_data_storage[gas_id][amr_tx_tch_next[gas_id]].tags,gas_id);

   /* Add parameters to the next store */
   ul_tch_amr_add_storage(ARFCN, TXLEV,
                          ul_amr_channel_type[gas_id],
                          GL1_MSG_TX_TCH_BLOCK_SID_FIRST,
                          amr_ul_sid_first_callback[gas_id],gas_id);

#ifdef FEATURE_VAMOS_II
   vamos2_ul_amr_tch_wait_state[gas_id] = vamos2_tch_wait_state;
#endif

   /* register a handler if one does not exist */
   if (amr_tx_message_counter[gas_id] == 0)
   {
      gl1_msgi_add_ft_handler( amr_tx_handler_fr, GL1_MSG_FT_HDLR_UL,gas_id );
   }
   amr_tx_message_counter[gas_id]++;
}


/*===========================================================================
FUNCTION gl1_msg_tx_sid_update_amr_fr

DESCRIPTION
   This function is used to transmit a message on the SID_UPDATE on the next
   block boundary.  The message contents should span 4 frames ie 1 block.
   This function may be called at task level but should be called in the block
   before the data is to transmitted.   If data to be transmitted is for
   frame #4,5,6,7, then this function should be completed before start of
   frame #3, ie called in frames 0,1,2.

DEPENDENCIES
   gl1_msg_cfg_ul_tch() and gl1_msg_store_ta(), must have been called at some time prior.

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
void gl1_msg_tx_sid_update_amr_fr( ARFCN_T ARFCN[4], int16  TXLEV[4],
                                   uint8  *data,
                                   uint8 curr_ul_cmi, uint8 curr_cmr, uint8 phase,
#ifdef FEATURE_VAMOS_II
                                   gl1_vamos2_tch_wait_type vamos2_tch_wait_state,
#endif
                                   gas_id_t gas_id )
{
   if (!data || !ARFCN)
   {
      MSG_GERAN_ERROR_2_G("Rcvd NULL data %d or ARFCN %d pointers",
                data, ARFCN);
      return;
   }

   *data = 0;
   *(data+1) = (uint8)((curr_cmr << 6)    |
                       (curr_ul_cmi << 4) |
                       (phase << 3));
   *(data+2) = MDSP_AMR_VOC_CODEC_MODE_DONT_CARE;
   *(data+3) = MDSP_AMR_VOC_FRAME_TYPE_SID_UPDATE;

   gl1_hw_write_tx_data_amr(ul_amr_channel_type[gas_id],
                            data,
                            MAX_SID_UPDATE_OCTETS,
                            gl1_msg_get_cs_timeslot(gas_id),
                            &amr_tx_data_storage[gas_id][amr_tx_tch_next[gas_id]].tags,gas_id);

   /* Add parameters to the next store */
   ul_tch_amr_add_storage(ARFCN, TXLEV,
                          ul_amr_channel_type[gas_id],
                          GL1_MSG_TX_TCH_BLOCK_SID_UPDATE,
                          amr_ul_sid_update_callback[gas_id],gas_id);

#ifdef FEATURE_VAMOS_II
  vamos2_ul_amr_tch_wait_state[gas_id] = vamos2_tch_wait_state;
#endif

   /* register a handler if one does not exist */
   if (amr_tx_message_counter[gas_id] == 0)
   {
      gl1_msgi_add_ft_handler( amr_tx_handler_fr, GL1_MSG_FT_HDLR_UL,gas_id );
   }
   amr_tx_message_counter[gas_id]++;
}


/*===========================================================================
FUNCTION gl1_msg_tx_saved_sid_update_amr_fr

DESCRIPTION
   This function is used to transmit a message on the SID_UPDATE on the next
   block boundary.  The message contents should span 4 frames ie 1 block.
   This function may be called at task level but should be called in the block
   before the data is to transmitted.   If data to be transmitted is for
   frame #4,5,6,7, then this function should be completed before start of
   frame #3, ie called in frames 0,1,2.

DEPENDENCIES
   gl1_msg_cfg_ul_tch() and gl1_msg_store_ta(), must have been called at some time prior.

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
void gl1_msg_tx_saved_sid_update_amr_fr( ARFCN_T ARFCN[4], int16  TXLEV[4],
                                         uint8 curr_ul_cmi,
                                         uint8 curr_cmr, uint8 phase ,
#ifdef FEATURE_VAMOS_II
                                         gl1_vamos2_tch_wait_type vamos2_tch_wait_state,
#endif
                                         gas_id_t gas_id )
{
   if (!ARFCN)
   {
      MSG_GERAN_ERROR_0_G("Rcvd NULL ARFCN pointer");
      return;
   }

   *amr_tx_sid_update_data[gas_id] = 0;
   *(amr_tx_sid_update_data[gas_id]+1) = (uint8)((curr_cmr << 6)    |
                                         (curr_ul_cmi << 4) |
                                         (phase << 3));
   *(amr_tx_sid_update_data[gas_id]+2) = MDSP_AMR_VOC_CODEC_MODE_DONT_CARE;
   *(amr_tx_sid_update_data[gas_id]+3) = MDSP_AMR_VOC_FRAME_TYPE_SID_UPDATE;

   gl1_hw_write_tx_data_amr(ul_amr_channel_type[gas_id],
                            amr_tx_sid_update_data[gas_id],
                            MAX_SID_UPDATE_OCTETS,
                            gl1_msg_get_cs_timeslot(gas_id),
                            &amr_tx_data_storage[gas_id][amr_tx_tch_next[gas_id]].tags,gas_id);

   /* Add parameters to the next store */
   ul_tch_amr_add_storage(ARFCN, TXLEV,
                          ul_amr_channel_type[gas_id],
                          GL1_MSG_TX_TCH_BLOCK_SID_UPDATE,
                          amr_ul_sid_update_callback[gas_id],gas_id);

#ifdef FEATURE_VAMOS_II
   vamos2_ul_amr_tch_wait_state[gas_id] = vamos2_tch_wait_state;
#endif

   /* register a handler if one does not exist */
   if (amr_tx_message_counter[gas_id] == 0)
   {
      gl1_msgi_add_ft_handler( amr_tx_handler_fr, GL1_MSG_FT_HDLR_UL,gas_id );
   }
   amr_tx_message_counter[gas_id]++;
}


/*===========================================================================
FUNCTION gl1_msg_cfg_ul_tch_amr_fr_hr,  gl1_msg_cfg_ul_facch_amr_fr_hr
    gl1_msg_cfg_ul_ratscch_amr_fr_hr, gl1_msg_cfg_ul_sid_first_amr_fr_hr
    gl1_msg_cfg_ul_sid_update_amr_fr_hr

DESCRIPTION
   These functions are used to configure the TSC and callbacks for the TCH,
   FACCH, RATSCCH, SID_FIRST, SID_UPDATE.  The callbacks will be invoked by
   the frame tick handler as required.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
void gl1_msg_cfg_ul_tch_amr_fr_hr ( uint8 TSC,
                                    gl1_msg_tx_cb_type *cb ,gas_id_t gas_id)
{
   if (!cb)
   {
      MSG_GERAN_ERROR_0_G("Rcvd NULL cb pointer");
      return;
   }

   amr_tx_TSC[gas_id] = TSC;
   amr_ul_tch_callback[gas_id].data_cb = cb->data_cb;
}

void gl1_msg_cfg_ul_facch_amr_fr_hr ( uint8 TSC,
                                      gl1_msg_tx_cb_type *cb,gas_id_t gas_id )
{
   if (!cb)
   {
      MSG_GERAN_ERROR_0_G("Rcvd NULL cb pointer");
      return;
   }

   amr_tx_TSC[gas_id] = TSC;
   amr_ul_facch_callback[gas_id].data_cb = cb->data_cb;
}

void gl1_msg_cfg_ul_ratscch_amr_fr_hr ( uint8 TSC,
                                        gl1_msg_tx_cb_type *cb,gas_id_t gas_id )
{
   if (!cb)
   {
      MSG_GERAN_ERROR_0_G("Rcvd NULL cb pointer");
      return;
   }

   amr_tx_TSC[gas_id] = TSC;
   amr_ul_ratscch_callback[gas_id].data_cb = cb->data_cb;
}


void gl1_msg_cfg_ul_sid_first_amr_fr_hr ( uint8 TSC,
                                          gl1_msg_tx_cb_type *cb,gas_id_t gas_id )
{
   if (!cb)
   {
      MSG_GERAN_ERROR_0_G("Rcvd NULL cb pointer");
      return;
   }
   amr_tx_TSC[gas_id] = TSC;
   amr_ul_sid_first_callback[gas_id].data_cb = cb->data_cb;
}

void gl1_msg_cfg_ul_sid_update_amr_fr_hr ( uint8 TSC,
                                           gl1_msg_tx_cb_type *cb,gas_id_t gas_id )
{
   if (!cb)
   {
      MSG_GERAN_ERROR_0_G("Rcvd NULL cb pointer");
      return;
   }
   amr_tx_TSC[gas_id] = TSC;
   amr_ul_sid_update_callback[gas_id].data_cb = cb->data_cb;
}

/*===========================================================================

FUNCTION gl1_msg_abort_tx_tch_amr_fr

DESCRIPTION
   These functions are used to terminate any TCH bursts pending.

===========================================================================*/
boolean gl1_msg_abort_tx_tch_amr_fr (gas_id_t gas_id)
{
   uint8 idx;

   /* Check if anything is active */
   if (amr_tx_message_counter[gas_id] == 0) return TRUE;

   /* Go through all pending transmits and abort them */
   if (amr_tx_message_counter[gas_id] > 1)
   {
      idx = (amr_tx_tch_current[gas_id] + 1) % NUM_AMR_TX_STORAGE;
      while (amr_tx_message_counter[gas_id] > 1)
      {
         /* Inform the frame layer we are aborting this transmit */
         (void) gl1_hw_abort_tx_bursts(&amr_tx_data_storage[gas_id][idx].tags,gas_id);

         /* Clear out the block */
         MSG_GERAN_HIGH_2_G("Aborting AMR TCH FR: type=%d, FN=%d",
                  amr_tx_data_storage[gas_id][idx].block, GSTMR_GET_FN_GERAN(gas_id));
         amr_tx_data_storage[gas_id][idx].block = GL1_MSG_TX_TCH_BLOCK_UNDEFINED;
         amr_tx_message_counter[gas_id]--;

         idx = (idx + 1) % NUM_AMR_TX_STORAGE;
      }

      /* Re-adjust the next pointer */
      amr_tx_tch_next[gas_id] = (amr_tx_tch_current[gas_id] + 1) % NUM_AMR_TX_STORAGE;
      amr_tx_data_storage_size[gas_id] = 1;
   }

   /* Call generic function to abort the current state machine */
   return ul_tch_amr_abort_current(TRUE,gas_id);
}



/****************************************************************************
 ****************************************************************************
 ****************************************************************************
   LOCAL   FUNCTIONS
 ****************************************************************************
 ****************************************************************************
 ***************************************************************************/

/*===========================================================================
FUNCTION amr_tx_handler_fr

DESCRIPTION
   This function should be called every frame on the 26 frame multiframe. The
   handler is installed as part of a request to tx a msg by
   gl1_msg_tx_tch_amr_fr() or gl1_msg_tx_facch_amr_fr() or
   gl1_msg_tx_ratscch_amr_fr() or gl1_msg_tx_sid_update_amr_fr() or
   gl1_msg_tx_sid_first_amr_fr().

   The handler is told when the start of a block is to occur and will call
   the frame layer functions at the proper time.   When the handler senses
   that there are no more messages queued up to send, it will signal that
   it should be deregistered.

DEPENDENCIES
   None

RETURN VALUE
   TRUE if no messages to tx
   FALSE if messages still being transmitted

SIDE EFFECTS
   None
===========================================================================*/
static boolean amr_tx_handler_fr
(
   boolean start_of_block,
   uint32  time_stamp,
   gas_id_t gas_id
)
{
   boolean remove_handler = FALSE;
   boolean encode_flag    = FALSE;
   amr_tx_data_storage_type *storage;
#ifdef FEATURE_VAMOS_II
   static boolean transmit_tch_burst[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(FALSE) };
#endif

   /* Shortcut to state machine data */
   storage = &amr_tx_data_storage[gas_id][amr_tx_tch_current[gas_id]];

   if (!storage->active)
   {
      if (start_of_block)
      {
         encode_flag = TRUE;
         storage->active = TRUE;
#ifdef FEATURE_VAMOS_II
         transmit_tch_burst[gas_id] = TRUE;
#endif
      }
      else
      {
         return remove_handler;
      }
   }

#ifdef FEATURE_VAMOS_II
   if((vamos2_ul_amr_tch_wait_state[gas_id] == WAIT_AFTER_BLOCK_START) && (storage->burst_num == 1))
   {
     transmit_tch_burst[gas_id] = FALSE;
     vamos2_ul_amr_tch_wait_state[gas_id] = NO_WAIT;
   }
   else if((vamos2_ul_amr_tch_wait_state[gas_id] == WAIT_AFTER_BURST2) && (storage->burst_num == 3))
   {
     transmit_tch_burst[gas_id] = FALSE;
     vamos2_ul_amr_tch_wait_state[gas_id] = NO_WAIT;
   }
   else if(storage->burst_num)
   {
     transmit_tch_burst[gas_id] = TRUE;
   }
#endif /*FEATURE_VAMOS_II*/

#ifdef FEATURE_VAMOS_II
   if(transmit_tch_burst[gas_id])
#endif
   {
      /* Now transmit the burst if there are bursts to transmit */
      if (gl1_msg_is_ciphering_enabled(gas_id))
      {
        gl1_hw_update_ciphering_fn(gas_id);
      }

      /* Fill in dynamic frame layer parameters */
      storage->signal_info.tx_power_index = storage->power_index[storage->burst_num];
      storage->burst_info.arfcn = storage->ARFCN[storage->burst_num];
      storage->burst_info.encode_flag = encode_flag;
      storage->burst_info.burst_num   = storage->burst_num;

      /* Issue the tx command */
      gl1_hw_tx_dtm_gsm(&storage->signal_info, &storage->burst_info, &storage->tags,gas_id);
      storage->burst_num++;
    }

   /* Do the callback in the 1st burst frame ie right before burst is sent*/
   if (storage->burst_num == 2)
   {
      if (storage->amr_ul_callback.data_cb != NULL)
      {
         storage->amr_ul_callback.data_cb(gas_id);
      }
   }

   /* Now was this the last burst */
   if (storage->burst_num == 4)
   {
     amr_tx_tch_current[gas_id] = (amr_tx_tch_current[gas_id] + 1) % NUM_AMR_TX_STORAGE;
     if (amr_tx_data_storage_size[gas_id] > 0)
     {

       amr_tx_data_storage_size[gas_id]--;
     }
     if (amr_tx_message_counter[gas_id] > 0)
     {

       if (--amr_tx_message_counter[gas_id] == 0)
       {
         remove_handler = TRUE;
       }
     }
   }

   return(remove_handler);

} /*lint !e715*/

#ifdef FEATURE_GSM_AMR_WB
/*===========================================================================
FUNCTION gl1_msg_ul_set_wb_amr_mode

DESCRIPTION
   This function should be called whenever we get any assignment, HandOver,
   channel_mode_modify or DTM assignment command. It informs the drivers
   of the use of WB AMR codec.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
void gl1_msg_ul_set_wb_amr_mode(boolean wb_amr_flag,gas_id_t gas_id)
{
    MSG_GERAN_HIGH_2_G("ul wb amr flag: current = %d, new  = %d",ul_wb_amr_flag[gas_id],wb_amr_flag);
    ul_wb_amr_flag[gas_id] = wb_amr_flag;
    ul_amr_channel_type[gas_id] = (wb_amr_flag) ? GL1_HW_TCH_WFS:GL1_HW_TCH_AFS;
}
#endif /* FEATURE_GSM_AMR_WB */


/*===========================================================================
FUNCTION amr_tx_handler_hr

DESCRIPTION
   This function should be called whenever a facch, ratscch, sid_update
   and sid_first messages are to be transmitted.
   The handler will get invoked in every frame until the facch message has
   been completely loaded into the mdsp buffers.

   The handler is told when the start of a block is to occur and will call
   the frame layer functions at the proper time.   When the handler senses
   that there are no more messages queued up to send, it will signal that
   it should be deregistered.

DEPENDENCIES
   None

RETURN VALUE
   TRUE if no messages to tx
   FALSE if messages still being transmitted

SIDE EFFECTS
   None
===========================================================================*/
static boolean amr_tx_handler_hr
(
   boolean start_of_block,
   uint32  time_stamp,
   gas_id_t gas_id

)
{
   boolean remove_handler = FALSE;
   boolean encode_flag    = FALSE;
   amr_tx_data_storage_type *storage;
   boolean *sacch_in_middle;

#ifdef FEATURE_VAMOS_II
   uint8 burst_num = 0;
   boolean vamos2_ul_amr_tch_wait = FALSE;
#endif

   /* Shortcut to current state machine */
   storage = &amr_tx_data_storage[gas_id][amr_tx_tch_current[gas_id]];

   /* Check if we are active yet or waiting for the block start */
   if (!storage->active)
   {
      if (start_of_block)
      {
         encode_flag = TRUE;
         storage->active = TRUE;
      }
      else
      {
         return remove_handler;
      }
   }

   /* This introduces a 2 frame delay between the 2nd and
    3 bursts of a middle HR FACCH block where the SACCH occurs
    The middle FACCH block bursta are on Fnmod26 = 8,10,13,15
    for HR=0 */

#ifdef FEATURE_VAMOS_II
  if(gl1_hw_get_vamos2_support(gas_id))
    {
    if(amr_middle_facch_hr_block_vamos2[gas_id])
      {
      burst_num = 3;
      }
    else if(amr_hr_middle_facch_block[gas_id] ||
           ((amr_hr_sacch_in_middle[gas_id]) && (vamos2_ul_amr_tch_wait_state[gas_id] == NO_WAIT)))
     {
     burst_num = 2;
     }
    else if(vamos2_ul_amr_tch_wait_state[gas_id] == WAIT_AFTER_BLOCK_START)
     {
     burst_num = 1;
     vamos2_ul_amr_tch_wait = TRUE;
     }

  if(storage->burst_num == burst_num)
    {
    if(amr_middle_facch_hr_block_vamos2[gas_id] == TRUE)
      {
      sacch_in_middle = &amr_middle_facch_hr_block_vamos2[gas_id];
      amr_hr_middle_facch_block[gas_id] = FALSE;
      amr_hr_sacch_in_middle[gas_id] = FALSE;
      }
    else if( amr_hr_middle_facch_block[gas_id] == TRUE )
      {
      sacch_in_middle = &amr_hr_middle_facch_block[gas_id];
      amr_hr_sacch_in_middle[gas_id] = FALSE;
      }
    else if(vamos2_ul_amr_tch_wait == TRUE)
      {
      sacch_in_middle = &vamos2_ul_amr_tch_wait;
      vamos2_ul_amr_tch_wait_state[gas_id] = NO_WAIT;
      amr_hr_sacch_in_middle[gas_id] = FALSE;
      }
    else
      {
      sacch_in_middle = &amr_hr_sacch_in_middle[gas_id];
      }
    if(*sacch_in_middle)
      {
      *sacch_in_middle = FALSE;
      MSG_GERAN_HIGH_0_G("sacch_in_middle");
      return remove_handler;
      }
    }
  }
 else
#endif
  {
   if (storage->burst_num == 2)
   {
      if (storage->block == GL1_MSG_TX_TCH_BLOCK_FACCH)
      {
         sacch_in_middle = &amr_hr_middle_facch_block[gas_id];
      }
      else
      {
         sacch_in_middle = &amr_hr_sacch_in_middle[gas_id];
      }

      if (*sacch_in_middle)
      {
         *sacch_in_middle = FALSE;
         return remove_handler;
      }
   }
  }

   /* Take action based on the frame count */
   switch (storage->frame_count)
   {
      /* Transmit burst on alternate frames */
      case 0:
      case 2:
      case 4:
      case 6:
      {
         if (gl1_msg_is_ciphering_enabled(gas_id))
         {
            gl1_hw_update_ciphering_fn(gas_id);
         }

         /* Fill in dynamic frame layer parameters */
         storage->signal_info.tx_power_index = storage->power_index[storage->burst_num];
         storage->burst_info.arfcn = storage->ARFCN[storage->burst_num];
         storage->burst_info.encode_flag = encode_flag;
         storage->burst_info.burst_num   = storage->burst_num;

         /* Issue the tx command */
         gl1_hw_tx_dtm_gsm(&storage->signal_info, &storage->burst_info, &storage->tags,gas_id);
         storage->burst_num++;

         /* Check if we are done */
         if (storage->burst_num == 4)
         {
            amr_tx_tch_current[gas_id] = (amr_tx_tch_current[gas_id] + 1) % NUM_AMR_TX_STORAGE;
            if (amr_tx_data_storage_size[gas_id] > 0)
            {
              amr_tx_data_storage_size[gas_id]--;
            }
            if (amr_tx_message_counter[gas_id] > 0)
            {

             if (--amr_tx_message_counter[gas_id] == 0)
             {
                remove_handler = TRUE;
             }
            }
         }
         break;
      }

      /* Make callback */
      case 5:
      {
         if (storage->amr_ul_callback.data_cb != NULL)
         {
            storage->amr_ul_callback.data_cb(gas_id);
         }
         break;
      }

      /* Do nothing */
      default:
         break;
   }

   /* Increment frame count */
   storage->frame_count++;

   return remove_handler;

} /*lint !e715*/



/*===========================================================================
FUNCTION gl1_msg_tx_tch_amr_hr

DESCRIPTION
   This function is used to transmit a message on the TCH on the next
   block boundary.  The message contents should span 4 frames ie 1 block.
   This function may be called at task level but should be called in the block
   before the data is to transmitted.   If data to be transmitted is for
   frame #4,5,6,7, then this function should be completed before start of
   frame #3, ie called in frames 0,1,2.

DEPENDENCIES
   gl1_msg_cfg_ul_tch() and gl1_msg_store_ta(), must have been called at some time prior.

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
void gl1_msg_tx_tch_amr_hr( ARFCN_T ARFCN[4], int16  TXLEV[4],
                            uint8  *data, boolean onset_flag,
                            uint8 curr_ul_cmi, uint8 curr_dl_cmi,
                            uint8 curr_cmr, uint8 curr_cmc,
                            gl1_msg_amr_mode_type curr_ul_codec_mode,
                            uint8 phase,
                            boolean sid_first_inh,
                            boolean sid_update_inh,
#ifdef FEATURE_VAMOS_II
                            gl1_vamos2_tch_wait_type vamos2_hr_tch_wait_state,
#endif

                            gas_id_t gas_id)
{
   static gl1_msg_amr_mode_type gl1_msg_loopback_I_prev_codec_mode[NUM_GERAN_DATA_SPACES];
   static uint8                 gl1_msg_loopback_I_prev_cmr[NUM_GERAN_DATA_SPACES];
   static uint8                 gl1_msg_loopback_I_prev_cmi[NUM_GERAN_DATA_SPACES];

   if (data == NULL || ARFCN == NULL)
   {
      MSG_GERAN_ERROR_2_G("Rcvd NULL data %d or ARFCN %d pointers",
                data, ARFCN);
      return;
   }

   if ( loopback_type[gas_id] == GL1_DEFS_LOOPBACK_TYPE_A ||
        loopback_type[gas_id] == GL1_DEFS_LOOPBACK_TYPE_B )
   {
      /*
       * As per specs: All uplink in band signalling codewords carry CMI in
       * case of loopbacks A and B(??)
       */
      *gl1_loopback_data[gas_id]     = 0;
      *(gl1_loopback_data[gas_id]+1) = (uint8) ((curr_ul_cmi << 6)    |
                                        (curr_ul_cmi << 4)    |
                                        (phase << 3)          |
                                        (onset_flag << 2)     |
                                        (sid_update_inh << 1) |
                                        (sid_first_inh << 0));
      *(gl1_loopback_data[gas_id]+2) = (uint8)curr_ul_codec_mode;
      *(gl1_loopback_data[gas_id]+3) = MDSP_AMR_VOC_FRAME_TYPE_SPEECH_GOOD;

      /* Overwrite data pointer with loopback one */
      data = gl1_loopback_data[gas_id];
   }
   else if (loopback_type[gas_id] == GL1_DEFS_LOOPBACK_TYPE_I)
   {
      /*
       * If previous block was a not FACCH.
       */
      if (!gl1_msg_loopback_I_prev_block_was_facch[gas_id])
      {
         /*
          * Save UL CMR, UL CMI and codec mode, in case the next block is a FACCH
          * block. The spec says that D/L CMI should be looped back as U/L CMR;
          * D/L CMC should be looped back as U/L CMI. Also, the U/L codec
          * mode should be based on D/L CMC.
          */
         gl1_msg_loopback_I_prev_cmr[gas_id] = curr_dl_cmi;
         gl1_msg_loopback_I_prev_cmi[gas_id] = curr_cmc;
         gl1_msg_loopback_I_prev_codec_mode[gas_id] = curr_ul_codec_mode;
      }

      *gl1_loopback_data[gas_id]     = 0;
      *(gl1_loopback_data[gas_id]+1) = (uint8)((gl1_msg_loopback_I_prev_cmr[gas_id] << 6) |
                                       (gl1_msg_loopback_I_prev_cmi[gas_id] << 4) |
                                       (phase << 3)                       |
                                       (onset_flag << 2));
      *(gl1_loopback_data[gas_id]+2) = (uint8)gl1_msg_loopback_I_prev_codec_mode[gas_id];
      *(gl1_loopback_data[gas_id]+3) = MDSP_AMR_VOC_FRAME_TYPE_SPEECH_GOOD;

      /* Overwrite data pointer with loopback one */
      data = gl1_loopback_data[gas_id];
   }
   else
   {
      *data     = 0;
      *(data+1) = (uint8)((curr_cmr << 6)       |
                          (curr_ul_cmi << 4)    |
                          (phase << 3)          |
                          (onset_flag << 2)     |
                          (sid_update_inh << 1) |
                          (sid_first_inh << 0));
   }

   /* Write data to frame layer */
   gl1_hw_write_tx_data_amr(GL1_HW_TCH_AHS, data,
                            MAX_TCH_OCTETS_AMR,
                            gl1_msg_get_cs_timeslot(gas_id),
                            &amr_tx_data_storage[gas_id][amr_tx_tch_next[gas_id]].tags,gas_id);

   /* Add parameters to the next store */
   ul_tch_amr_add_storage(ARFCN, TXLEV, GL1_HW_TCH_AHS,
                          GL1_MSG_TX_TCH_BLOCK_TCH,
                          amr_ul_tch_callback[gas_id],gas_id);

#ifdef FEATURE_VAMOS_II
   vamos2_ul_amr_tch_wait_state[gas_id] = vamos2_hr_tch_wait_state;
#endif

   /* register a handler if one does not exist */
   if (amr_tx_tch_message_counter[gas_id] == 0)
   {
      gl1_msgi_add_ft_handler( amr_tch_tx_handler_hr, GL1_MSG_FT_HDLR_UL,gas_id );
   }
   amr_tx_tch_message_counter[gas_id]++;
}



/*===========================================================================
FUNCTION gl1_msg_tx_facch_amr_hr

DESCRIPTION
   This function is used to transmit a message on the FACCH on the next
   block boundary.  The message contents should span 4 frames ie 1 block.
   This function may be called at task level but should be called in the block
   before the data is to transmitted.   If data to be transmitted is for
   frame #4,5,6,7, then this function should be completed before start of
   frame #3, ie called in frames 0,1,2.

DEPENDENCIES
   gl1_msg_cfg_ul_tch() and gl1_msg_store_ta(), must have been called at some time prior.

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
void gl1_msg_tx_facch_amr_hr ( ARFCN_T ARFCN[4], int16  TXLEV[4],
                               uint8  *data, boolean onset_flag,
                               uint8 curr_ul_cmi, uint8 curr_cmr, uint8 phase,
                               boolean middle_facch_block,
                               boolean sid_first_inh,
                               boolean sid_update_inh,
#ifdef FEATURE_VAMOS_II
                               boolean amr_middle_facch_block_vamos2,
                               gl1_vamos2_tch_wait_type vamos2_hr_tch_wait_state,
#endif
                               gas_id_t gas_id
                               )
{
   if (!data || !ARFCN)
   {
      MSG_GERAN_ERROR_2_G("Rcvd NULL data %d or ARFCN %d pointers",
                data, ARFCN);
      return;
   }

   amr_hr_middle_facch_block[gas_id] = middle_facch_block;

#ifdef FEATURE_VAMOS_II
   amr_middle_facch_hr_block_vamos2[gas_id] = amr_middle_facch_block_vamos2;
   vamos2_ul_amr_tch_wait_state[gas_id] = vamos2_hr_tch_wait_state;
#endif

   *data = 0;
   *(data+1) = (uint8)((curr_cmr << 6)       |
                       (curr_ul_cmi << 4)    |
                       (phase << 3)          |
                       (onset_flag << 2)     |
                       (sid_update_inh << 1) |
                       (sid_first_inh << 0));

   gl1_hw_write_tx_data_amr(GL1_HW_FACCH_AHS, data,
                            MAX_FACCH_OCTETS_AMR,
                            gl1_msg_get_cs_timeslot(gas_id),
                            &amr_tx_data_storage[gas_id][amr_tx_tch_next[gas_id]].tags,gas_id);

   /* Add parameters to the next store */
   ul_tch_amr_add_storage(ARFCN, TXLEV, GL1_HW_FACCH_AHS,
                          GL1_MSG_TX_TCH_BLOCK_FACCH,
                          amr_ul_facch_callback[gas_id],gas_id);

   /* register a handler if one does not exist */
   if (amr_tx_message_counter[gas_id] == 0)
   {
      gl1_msgi_add_ft_handler(amr_tx_handler_hr, GL1_MSG_FT_HDLR_UL,gas_id);
   }
   amr_tx_message_counter[gas_id]++;
}


/*===========================================================================
FUNCTION gl1_msg_tx_ratscch_amr_hr

DESCRIPTION
   This function is used to transmit a message on the RATSCCH on the next
   block boundary.  The message contents should span 4 frames ie 1 block.
   This function may be called at task level but should be called in the block
   before the data is to transmitted.   If data to be transmitted is for
   frame #4,5,6,7, then this function should be completed before start of
   frame #3, ie called in frames 0,1,2.

DEPENDENCIES
   gl1_msg_cfg_ul_tch() and gl1_msg_store_ta(), must have been called at some time prior.

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
void gl1_msg_tx_ratscch_amr_hr ( ARFCN_T ARFCN[4], int16  TXLEV[4],
                                 uint8  *data, boolean onset_flag,
                                 uint8 curr_ul_cmi, uint8 curr_cmr,
                                 uint8 phase,
                                 boolean sacch_in_middle,
                                 boolean sid_first_inh,
                                 boolean sid_update_inh,
#ifdef FEATURE_VAMOS_II
                                 boolean amr_middle_facch_block_vamos2,
                                 gl1_vamos2_tch_wait_type vamos2_hr_tch_wait_state,
#endif
                                 gas_id_t gas_id
                                 )
{
   if (!data || !ARFCN)
   {
      MSG_GERAN_ERROR_2_G("Rcvd NULL data %d or ARFCN %d pointers",
                data, ARFCN);
      return;
   }

   if ( !amr_hr_sacch_in_middle[gas_id])
   {
      amr_hr_sacch_in_middle[gas_id] = sacch_in_middle;
   }

#ifdef FEATURE_VAMOS_II
   if ( !amr_middle_facch_hr_block_vamos2[gas_id] )
   {
      amr_middle_facch_hr_block_vamos2[gas_id] = amr_middle_facch_block_vamos2;
   }
   vamos2_ul_amr_tch_wait_state[gas_id] = vamos2_hr_tch_wait_state;
#endif

   *data = 0;
   *(data+1) = (uint8)((curr_cmr << 6)       |
                       (curr_ul_cmi << 4)    |
                       (phase << 3)          |
                       (onset_flag << 2)     |
                       (sid_update_inh << 1) |
                       (sid_first_inh << 0));
   *(data+2) = MDSP_AMR_VOC_CODEC_MODE_DONT_CARE;
   *(data+3) = MDSP_AMR_VOC_FRAME_TYPE_RATSCCH;

   gl1_hw_write_tx_data_amr(GL1_HW_TCH_AHS, data, MAX_RATSCCH_OCTETS,
                            gl1_msg_get_cs_timeslot(gas_id),
                            &amr_tx_data_storage[gas_id][amr_tx_tch_next[gas_id]].tags,gas_id);

   /* Add parameters to the next store */
   ul_tch_amr_add_storage(ARFCN, TXLEV, GL1_HW_TCH_AHS,
                          GL1_MSG_TX_TCH_BLOCK_RATSCCH,
                          amr_ul_ratscch_callback[gas_id],gas_id);

   /* register a handler if one does not exist */
   if (amr_tx_message_counter[gas_id] == 0)
   {
      gl1_msgi_add_ft_handler(amr_tx_handler_hr, GL1_MSG_FT_HDLR_UL,gas_id);
   }
   amr_tx_message_counter[gas_id]++;
}


/*===========================================================================
FUNCTION gl1_msg_tx_sid_first_amr_hr

DESCRIPTION
   This function is used to transmit a message on the SID_FIRST on the next
   block boundary.  The message contents should span 4 frames ie 1 block.
   This function may be called at task level but should be called in the block
   before the data is to transmitted.   If data to be transmitted is for
   frame #4,5,6,7, then this function should be completed before start of
   frame #3, ie called in frames 0,1,2.

DEPENDENCIES
   gl1_msg_cfg_ul_tch() and gl1_msg_store_ta(), must have been called at some time prior.

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
void gl1_msg_tx_sid_first_amr_hr( ARFCN_T ARFCN[4], int16  TXLEV[4],
                                  uint8 * data,
                                  uint8 curr_ul_cmi,
                                  uint8 curr_cmr, uint8 phase,
                                  boolean sacch_in_middle,
#ifdef FEATURE_VAMOS_II
                                   boolean amr_middle_facch_block_vamos2,
                                  gl1_vamos2_tch_wait_type vamos2_hr_tch_wait_state,
#endif
                                  gas_id_t gas_id
                                  )
{
   if (!data || !ARFCN)
   {
      MSG_GERAN_ERROR_2_G("Rcvd NULL data %d or ARFCN %d pointers",
                data, ARFCN);
      return;
   }

   if ( !amr_hr_sacch_in_middle[gas_id])
   {
      amr_hr_sacch_in_middle[gas_id] = sacch_in_middle;
   }

#ifdef FEATURE_VAMOS_II
   if ( !amr_middle_facch_hr_block_vamos2[gas_id] )
   {
   amr_middle_facch_hr_block_vamos2[gas_id] = amr_middle_facch_block_vamos2;
   }

   vamos2_ul_amr_tch_wait_state[gas_id] = vamos2_hr_tch_wait_state;
#endif

   *data = 0;
   *(data+1) = (uint8)((curr_cmr << 6)    |
                       (curr_ul_cmi << 4) |
                       (phase << 3));
   *(data+2) = MDSP_AMR_VOC_CODEC_MODE_DONT_CARE;
   *(data+3) = MDSP_AMR_VOC_FRAME_TYPE_SID_FIRST;


   gl1_hw_write_tx_data_amr(GL1_HW_TCH_AHS, data,
                            MAX_SID_FIRST_OCTETS,
                            gl1_msg_get_cs_timeslot(gas_id),
                            &amr_tx_data_storage[gas_id][amr_tx_tch_next[gas_id]].tags,gas_id);

   /* Add parameters to the next store */
   ul_tch_amr_add_storage(ARFCN, TXLEV, GL1_HW_TCH_AHS,
                          GL1_MSG_TX_TCH_BLOCK_SID_FIRST,
                          amr_ul_sid_first_callback[gas_id],gas_id);

   /* register a handler if one does not exist */
   if (amr_tx_message_counter[gas_id] == 0)
   {
      gl1_msgi_add_ft_handler( amr_tx_handler_hr, GL1_MSG_FT_HDLR_UL,gas_id );
   }
   amr_tx_message_counter[gas_id]++;
}


/*===========================================================================
FUNCTION gl1_msg_tx_sid_update_amr_hr

DESCRIPTION
   This function is used to transmit a message on the SID_UPDATE on the next
   block boundary.  The message contents should span 4 frames ie 1 block.
   This function may be called at task level but should be called in the block
   before the data is to transmitted.   If data to be transmitted is for
   frame #4,5,6,7, then this function should be completed before start of
   frame #3, ie called in frames 0,1,2.

DEPENDENCIES
   gl1_msg_cfg_ul_tch() and gl1_msg_store_ta(), must have been called at some time prior.

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
void gl1_msg_tx_sid_update_amr_hr( ARFCN_T ARFCN[4], int16  TXLEV[4],
                                   uint8  *data,
                                   uint8 curr_ul_cmi, uint8 curr_cmr,
                                   uint8 phase,
                                   boolean sacch_in_middle,
#ifdef FEATURE_VAMOS_II
                                   boolean amr_middle_facch_block_vamos2,
                                   gl1_vamos2_tch_wait_type vamos2_hr_tch_wait_state,
#endif
                                   gas_id_t gas_id
                                   )
{
   if (!data || !ARFCN)
   {
      MSG_GERAN_ERROR_2_G("Rcvd NULL data %d or ARFCN %d pointers",
                data, ARFCN);
      return;
   }

   if ( !amr_hr_sacch_in_middle[gas_id])
   {
      amr_hr_sacch_in_middle[gas_id] = sacch_in_middle;
   }

#ifdef FEATURE_VAMOS_II
   if ( !amr_middle_facch_hr_block_vamos2[gas_id] )
   {
   amr_middle_facch_hr_block_vamos2[gas_id] = amr_middle_facch_block_vamos2;
   }
   vamos2_ul_amr_tch_wait_state[gas_id] = vamos2_hr_tch_wait_state;
#endif

   *data = 0;
   *(data+1) = (uint8)((curr_cmr << 6)    |
                       (curr_ul_cmi << 4) |
                       (phase << 3));
   *(data+2) = MDSP_AMR_VOC_CODEC_MODE_DONT_CARE;
   *(data+3) = MDSP_AMR_VOC_FRAME_TYPE_SID_UPDATE;

   gl1_hw_write_tx_data_amr(GL1_HW_TCH_AHS, data,
                            MAX_SID_UPDATE_OCTETS,
                            gl1_msg_get_cs_timeslot(gas_id),
                            &amr_tx_data_storage[gas_id][amr_tx_tch_next[gas_id]].tags,gas_id);

   /* Add parameters to the next store */
   ul_tch_amr_add_storage(ARFCN, TXLEV, GL1_HW_TCH_AHS,
                          GL1_MSG_TX_TCH_BLOCK_SID_UPDATE,
                          amr_ul_sid_update_callback[gas_id],gas_id);

   /* register a handler if one does not exist */
   if (amr_tx_message_counter[gas_id] == 0)
   {
      gl1_msgi_add_ft_handler( amr_tx_handler_hr, GL1_MSG_FT_HDLR_UL,gas_id );
   }
   amr_tx_message_counter[gas_id]++;
}


/*===========================================================================
FUNCTION gl1_msg_tx_saved_sid_update_amr_hr

DESCRIPTION
   This function is used to transmit a message on the SID_UPDATE on the next
   block boundary.  The message contents should span 4 frames ie 1 block.
   This function may be called at task level but should be called in the block
   before the data is to transmitted.   If data to be transmitted is for
   frame #4,5,6,7, then this function should be completed before start of
   frame #3, ie called in frames 0,1,2.

DEPENDENCIES
   gl1_msg_cfg_ul_tch() and gl1_msg_store_ta(), must have been called at some time prior.

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
void gl1_msg_tx_saved_sid_update_amr_hr( ARFCN_T ARFCN[4], int16  TXLEV[4],
                                         uint8 curr_ul_cmi,
                                         uint8 curr_cmr, uint8 phase,
                                         boolean sacch_in_middle,
#ifdef FEATURE_VAMOS_II
                                         boolean amr_middle_facch_block_vamos2,
                                         gl1_vamos2_tch_wait_type vamos2_hr_tch_wait_state,
#endif
                                         gas_id_t gas_id
                                         )
{
   if (!ARFCN)
   {
      MSG_GERAN_ERROR_0_G("Rcvd NULL ARFCN pointer");

      return;
   }

   if ( !amr_hr_sacch_in_middle[gas_id])
   {
      amr_hr_sacch_in_middle[gas_id] = sacch_in_middle;
   }

#ifdef FEATURE_VAMOS_II
   if ( !amr_middle_facch_hr_block_vamos2[gas_id] )
   {
      amr_middle_facch_hr_block_vamos2[gas_id] = amr_middle_facch_block_vamos2;
   }

   vamos2_ul_amr_tch_wait_state[gas_id] = vamos2_hr_tch_wait_state;
#endif

   *amr_tx_sid_update_data[gas_id]     = 0;
   *(amr_tx_sid_update_data[gas_id]+1) = (uint8)((curr_cmr << 6)    |
                                         (curr_ul_cmi << 4) |
                                         (phase << 3));
   *(amr_tx_sid_update_data[gas_id]+2) = MDSP_AMR_VOC_CODEC_MODE_DONT_CARE;
   *(amr_tx_sid_update_data[gas_id]+3) = MDSP_AMR_VOC_FRAME_TYPE_SID_UPDATE;

   gl1_hw_write_tx_data_amr(GL1_HW_TCH_AFS, amr_tx_sid_update_data[gas_id],
                            MAX_SID_UPDATE_OCTETS,
                            gl1_msg_get_cs_timeslot(gas_id),
                            &amr_tx_data_storage[gas_id][amr_tx_tch_next[gas_id]].tags,gas_id);

   /* Add parameters to the next store */
   ul_tch_amr_add_storage(ARFCN, TXLEV, GL1_HW_TCH_AHS,
                          GL1_MSG_TX_TCH_BLOCK_SID_UPDATE,
                          amr_ul_sid_update_callback[gas_id],gas_id);

   /* register a handler if one does not exist */
   if (amr_tx_message_counter[gas_id] == 0)
   {
      gl1_msgi_add_ft_handler( amr_tx_handler_hr, GL1_MSG_FT_HDLR_UL,gas_id );
   }
   amr_tx_message_counter[gas_id]++;
}

/*===========================================================================
FUNCTION gl1_msg_save_sid_update_amr_fr_hr

DESCRIPTION
   This funcion is used to save the SID_UPDATE payload when there is a
   pre-emption of SID_UPDATE by a RATSCCH or a FACCH.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
void gl1_msg_save_sid_update_amr_fr_hr( uint8  *data,gas_id_t gas_id)
{
   if (!data)
   {
      MSG_GERAN_ERROR_0_G("Rcvd NULL data pointer");
      return;
   }
   /* CR222761 - previously this cpy had a 4byte offset
    * however passed in data already accounts for this
    * offset so not needed
    */
   memscpy( amr_tx_sid_update_data[gas_id], sizeof(amr_tx_sid_update_data[gas_id]), data, MAX_SID_UPDATE_OCTETS );
}

/*===========================================================================

FUNCTION gl1_msg_abort_tx_tch_amr_hr

DESCRIPTION
   This function is used to terminate any TCH bursts pending. Terminates
   after the block completes.

===========================================================================*/
boolean gl1_msg_abort_tx_tch_amr_hr (gas_id_t gas_id)
{
   uint8 idx, count, tx_storage_count;

   /* Check if anything is active */
   count = amr_tx_tch_message_counter[gas_id] + amr_tx_message_counter[gas_id];

   /* If nothing is running we are done */
   if (count == 0)
   {
      return TRUE;
   }
   /* If we have pending transmit blocks we can just abort them */
   else if (count > 1)
   {
      idx = (amr_tx_tch_current[gas_id] + 1) % NUM_AMR_TX_STORAGE;
      /* set storeage loop counter to ensure below while loop cannot run for more than NUM_AMR_TX_STORAGE loops*/
      tx_storage_count = NUM_AMR_TX_STORAGE;
      while (((amr_tx_message_counter[gas_id] + amr_tx_tch_message_counter[gas_id]) > 1) && (tx_storage_count > 0))
      {
         /* Inform the frame layer we are aborting this transmit */
         (void) gl1_hw_abort_tx_bursts(&amr_tx_data_storage[gas_id][idx].tags,gas_id);

         /* Decrement appropriate count, make sure to remove the handler
            if it hits 0 */
         if (amr_tx_data_storage[gas_id][idx].block == GL1_MSG_TX_TCH_BLOCK_TCH)
         {
            if (amr_tx_tch_message_counter[gas_id] > 0)
            {
               if(--amr_tx_tch_message_counter[gas_id] == 0)
               {
                  gl1_msgi_remove_ft_handler(amr_tch_tx_handler_hr,gas_id);
               }
            }
         }
         else
         {
            if (amr_tx_message_counter[gas_id] > 0)
            {
              if (--amr_tx_message_counter[gas_id] == 0)
              {
                 gl1_msgi_remove_ft_handler(amr_tx_handler_hr,gas_id);
              }
            }
         }

         /* Clear out the block */
         MSG_GERAN_HIGH_3_G("Aborting AMR TCH HR: type=%d, FN=%d, tx_storage_count=%d",
                  amr_tx_data_storage[gas_id][amr_tx_tch_current[gas_id]].block, GSTMR_GET_FN_GERAN(gas_id), tx_storage_count);
         amr_tx_data_storage[gas_id][idx].block = GL1_MSG_TX_TCH_BLOCK_UNDEFINED;

         idx = (idx + 1) % NUM_AMR_TX_STORAGE;
         tx_storage_count--;
      }

      /* check message counters reached 0 on aborting - if not, the handler may not have been uninstalled */
      if(amr_tx_message_counter[gas_id] != 0)
      {
         MSG_GERAN_ERROR_1_G("amr_tx_message_counter[gas_id]=%d on TCH/AHS abort",amr_tx_message_counter[gas_id]);
         /* reset counter and uninstall handler (only happens if handler is actually installed) */
         amr_tx_message_counter[gas_id] = 0;
         gl1_msgi_remove_ft_handler(amr_tx_handler_hr,gas_id);
      }
      if(amr_tx_tch_message_counter[gas_id] != 0)
      {
         MSG_GERAN_ERROR_1_G("amr_tx_message_counter[gas_id]=%d on TCH/AHS abort",amr_tx_tch_message_counter[gas_id]);
         /* reset counter and uninstall handler (only happens if handler is actually installed) */
         amr_tx_tch_message_counter[gas_id] = 0;
         gl1_msgi_remove_ft_handler(amr_tch_tx_handler_hr,gas_id);
      }

      /* Adjust the next pointer since we now have only one sm in progress */
      amr_tx_tch_next[gas_id] = (amr_tx_tch_current[gas_id] + 1) % NUM_AMR_TX_STORAGE;
      amr_tx_data_storage_size[gas_id] = 1;
   }

   /* Attempt to abort current state machine */
   return ul_tch_amr_abort_current(FALSE,gas_id);
}



/*===========================================================================

FUNCTION amr_tch_tx_handler_hr

DESCRIPTION
   This function should be called every frame on the 26 frame multiframe. The
   handler is installed as part of a request to tx a msg by gl1_msg_tx_tch().
   The handler is told when the start of a block is to occur and will call
   the frame layer functions at the proper time.   When the handler senses
   that there are no more messages queued up to send, it will signal that
   it should be deregistered.

DEPENDENCIES
   None

RETURN VALUE
   TRUE if no messages to tx
   FALSE if messages still being transmitted

SIDE EFFECTS
   None
===========================================================================*/
static boolean amr_tch_tx_handler_hr
(
   boolean start_of_block,
   uint32  time_stamp,
   gas_id_t gas_id
)
{
   boolean remove_handler = FALSE;
   boolean encode_flag    = FALSE;
   amr_tx_data_storage_type *storage;
#ifdef FEATURE_VAMOS_II
   boolean transmit_burst = FALSE;
#endif

   /* Shortcut to current state machine */
   storage = &amr_tx_data_storage[gas_id][amr_tx_tch_current[gas_id]];

   /* If we are not active, check if this is the start of block yet */
   if (!storage->active)
   {
      if (start_of_block)
      {
         encode_flag = TRUE;
         storage->active = TRUE;
#ifdef FEATURE_VAMOS_II
         transmit_burst = TRUE;
#endif
      }
      else
      {
         return remove_handler;
      }
   }

#ifdef FEATURE_VAMOS_II
   if((vamos2_ul_amr_tch_wait_state[gas_id] == WAIT_AFTER_BLOCK_START) &&(storage->burst_num == 1))
   {
    transmit_burst = FALSE;
    vamos2_ul_amr_tch_wait_state[gas_id] = NO_WAIT;
   }
   else if(storage->burst_num)
   {
    transmit_burst = TRUE;
   }
#endif

#ifdef FEATURE_VAMOS_II
   if(transmit_burst)
#endif
   {
   /* Take action based on frame count */
   switch (storage->frame_count)
   {
      /* Transmit bursts in frame 0 and 2 */
      case 0:
      case 2:
      {
         if (gl1_msg_is_ciphering_enabled(gas_id))
         {
            gl1_hw_update_ciphering_fn(gas_id);
         }

         /* Fill in dynamic frame layer parameters */
         storage->signal_info.tx_power_index = storage->power_index[storage->burst_num];
         storage->burst_info.arfcn = storage->ARFCN[storage->burst_num];
         storage->burst_info.encode_flag = encode_flag;
         storage->burst_info.burst_num   = storage->burst_num;

         /* Issue the tx command */
         gl1_hw_tx_dtm_gsm(&storage->signal_info, &storage->burst_info, &storage->tags,gas_id);
         storage->burst_num++;

         /* If this was the last tx burst then end the state machine */
         if (storage->burst_num == 2)
         {
            amr_tx_tch_current[gas_id] = (amr_tx_tch_current[gas_id] + 1) % NUM_AMR_TX_STORAGE;
            if (amr_tx_data_storage_size[gas_id] > 0)
            {
              amr_tx_data_storage_size[gas_id]--;
            }
            if (amr_tx_tch_message_counter[gas_id] > 0)
            {

             if (--amr_tx_tch_message_counter[gas_id] == 0)
             {
                remove_handler = TRUE;
             }
            }
         }
         break;
      }

      /* Make callback in frame 1 */
      case 1:
      {
         if (storage->amr_ul_callback.data_cb != NULL)
         {
            storage->amr_ul_callback.data_cb(gas_id);
         }
         break;
      }

      /* Do nothing */
      default:
         break;
   }

   /* Increment frame count */
   storage->frame_count++;
   }

   return remove_handler;
} /*lint !e715*/


#endif /* FEATURE_GSM && FEATURE_GSM_AMR */