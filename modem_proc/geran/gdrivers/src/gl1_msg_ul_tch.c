/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

         G L 1  M E S S A G E  L A Y E R -  T R A F F I C   C H A N N E L

GENERAL DESCRIPTION
   This module contains the procedures used for communicating on the ul TCH.
   The procedures used herein are intended for use by the
   portable layer 1 software.

EXTERNALIZED FUNCTIONS

   gl1_msg_cfg_ul_tch_facch
   gl1_msg_tx_tch
   gl1_msg_tx_facch
   gl1_msg_hr_vocoder_tx

   gl1_msg_abort_tx_tch_facch_fr
   gl1_msg_abort_tx_tch_facch_hr

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_msg_ul_tch.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
11/05/15   pjr      CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
08/04/15   pjr      CR818535 FR25951 Changes to support Single Voice Session 
02/01/14   jk       CR595130: GL1 changes to support VAMOS-II
22/01/13   ws       CR602047 - Allow MVS/GHDI to operate on SUB2 if VS driver is not present
07/08/13   jk       CR490807:Support for shifted SACCH  
11/11/13   ws       CR 573757 Featurise GSMTR-1 registers and Voice services driver
15/08/13   sk       CR524039 GBTA bringup changes
7/16/13    zc       CR470197 Replace memcpy with memscpy
20/05/13   cs       TSTS Updates
16/05/13   pjr      CR487402: Changes to improve interaction with Voice Services
03/04/13   pjr      CR470940 included gl1_hw_vs_g.h
31/03/13   ws       CR466973 - Triton DSDS BU: Fixed stability issues with VS driver
07/03/13   dv       CR461080: Introduction of new GSM VS interface
13/03/13   mc       CR.459487 : COEX Support - GERAN arbitrator framework
08/06/12   pa       GPLT: Replaced FEATUREN_GERAN_RF_SUPPORT with FEATURE_GL1_GPLT_CANNED_RSLT.
13/04/12   pg       CR350709: Unbalanced vocoder mutex lock/unlock pair
10/02/12   ws       CR 336194 Enable TD-SCDMA IRAT for GERAN
16/12/11   ws       Free floating API change - provide API for NAS GSVOICE to copy vocoder
                    data from/to GL1 to MVS structures
26/05/11   cja      For Nikel BU remove audio calls
19/05/11   cja      Changes for GPLT
26/04/11   ws       Removed deprecated header voc.h
17/01/10   ab       Updated - Resolve Klocwork Buffer Overflow errors
14/01/11   ab       Resolve Klocwork Buffer Overflow errors
02/08/10   tjw      Removed unused clk.h (CMI phase 1 re-clean up)
05/08/09   ws       Removed FEATURE_GSM_MDSP_DTM it's now mainlined
27/11/08   ws       Added FEATURE_MVS_UNDER_GHDI to remove all direct MVS calls.
28/07/98   hv       Added KxMutex support
28/07/08   ws       Added FEATURE_GSM_8K_QDSP6_AUDIO for chanding Endian of hoing sequence
20/04/07   cs       CR117035 Report HR DTX correctly for SACCH Measurement reports
08/12/05   gfr      Inform frame layer when aborting an ongoing transmit.
08/09/05   gfr      Clean up aborting.
07/29/05   gfr      DTM support
06/09/05   dp       Changes to uplink aborts to support calling FTN before
                    aborts
05/18/05   gfr      Lint cleanup
05/05/05   yh       Code clean up
04/21/05   yh       Support for panic reset
04/12/05   gfr      Consolidated GSM hw layer parameters
04/08/05   gfr      Combined GSM and GPRS channel types
03/16/05   yh       Lint
01/05/05   dp       Merged audio optimizations to mainline
12/20/04   gfr      Remove handlers after abort
11/01/04   yhong    Abort function assumes gl1_msg_frame_tick_notification() is
                    called after the abort.
08/18/04   yhong    Initial version. Contains only the uplink functions taken
                    from gl1_msg_tch.c
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#ifdef CUST_H
#include "customer.h"
#endif


#include "comdef.h"
#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "gl1_hwi.h"
#include "gl1_msg_g.h"
#include "gl1_msgi.h"
#include "msg.h"

#ifdef FEATURE_DATA_GCSD
#include "l1_ds.h"
#include "l1_ds_v.h"
#include "l1_ds_v_g.h"
#endif

#include "sys_cnst.h"
#include "gl1_msgi_tch.h"
#include "gtmrs.h"
#include "gtmrs_g.h"

#include "geran_dual_sim.h"
#include "geran_multi_sim.h"

#include <string.h>
#include "gl1_mutex.h"
#include <stringl/stringl.h>

/*****************************************************

 LOCAL STORAGE

******************************************************/

/* Number of messages to send or receive */
/* This number will be 0,1, or 2         */
static uint8 tx_message_counter[NUM_GERAN_DATA_SPACES] =
               { INITIAL_VALUE( 0 ) };

/* Configured Callback pointers  */
static gl1_msg_tx_cb_type       ul_tch_callback[NUM_GERAN_DATA_SPACES]   =
               { INITIAL_VALUE_S( 0 ) };

static gl1_msg_tx_cb_type       ul_facch_callback[NUM_GERAN_DATA_SPACES] =
               { INITIAL_VALUE_S( 0 ) };


/* Current state of ciphering */
/* Indicates that this is the middle FACCH block with a SACCH in between */
/* The bursts are assymetric in this case */
static boolean middle_facch_hr_block[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( FALSE ) };

#ifdef FEATURE_VAMOS_II
static boolean middle_facch_hr_block_vamos2[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( FALSE ) };
#endif

/* TCH Tx vars */
static  boolean transmit_burst_tch[NUM_GERAN_DATA_SPACES] =
                  { INITIAL_VALUE( FALSE ) };

static  uint8 burst_num_tch[NUM_GERAN_DATA_SPACES] =
                { INITIAL_VALUE( 0 ) };

/* For calculating: DTX_USED for SACCH rpts */
boolean gl1_dtx_used[NUM_GERAN_DATA_SPACES] =
          { INITIAL_VALUE( FALSE ) };

uint8 gl1_loopback_data[NUM_GERAN_DATA_SPACES][ MAX_LOOPBACK_OCTETS ];

/* Block types */
typedef enum
{
   GL1_MSG_TX_TCH_BLOCK_TCH,
   GL1_MSG_TX_TCH_BLOCK_FACCH
#ifdef FEATURE_DATA_GCSD
   ,GL1_MSG_TX_TCH_BLOCK_CSTCH
#endif
} gl1_msg_tx_tch_block_type;

/* Store the local frames */
typedef struct
{
    gl1_msg_tx_tch_block_type   block;
    gl1_msg_tx_cb_type          ul_callback;
    gl1_hw_channel_type         channel_type;

    /* Frame layer parameters */
    ARFCN_T                     ARFCN[GL1_DEFS_FRAMES_IN_BLK_PERIOD];
    int16                       power_index[GL1_DEFS_FRAMES_IN_BLK_PERIOD];
    gl1_hw_tx_signal_type       signal_info;
    gl1_hw_tx_burst_type        burst_info;
    gl1_hw_tx_tags_type         tags;
} tx_data_storage_type;

#define NUM_TX_STORAGE 4

/* Local Storage */
static tx_data_storage_type tx_data_storage[NUM_GERAN_DATA_SPACES][NUM_TX_STORAGE];

static uint8 tx_tch_current[NUM_GERAN_DATA_SPACES] =
               { INITIAL_VALUE( 0 ) };

static uint8 tx_tch_next[NUM_GERAN_DATA_SPACES] =
               { INITIAL_VALUE( 0 ) };

static uint8 tx_TSC[NUM_GERAN_DATA_SPACES] =
               { INITIAL_VALUE( 0 ) };

static boolean tch_abort[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( FALSE ) };

#ifdef FEATURE_VAMOS_II
static gl1_vamos2_tch_wait_type vamos2_ul_tch_wait_state[NUM_GERAN_DATA_SPACES] =
               { INITIAL_VALUE( NO_WAIT ) };
#endif 

#ifdef FEATURE_DATA_GCSD
/* Number of octets in GCSD uplink messages */
#define TCHF144_OCTETS 38
#define TCHF96_OCTETS 30
#define FACCH_OCTETS 23
#endif

/*****************************************************

 LOCAL FUNCTION PROTOTYPES

******************************************************/

static boolean fr_tx_handler(boolean start_of_block,
                             uint32  time_stamp,gas_id_t gas_id );

static boolean hr_tx_handler( boolean start_of_block,
                              uint32  time_stamp,gas_id_t gas_id );

#ifdef FEATURE_DATA_GCSD
extern void gcsd_ota_tx_callback(uint8 **curr_buf,
                                 uint8 *prev_buf,
                                 boolean tx_enabled,
                                 gas_id_t gas_id);
#endif

/*===========================================================================
FUNCTION gl1_msgi_init_ul_tch

DESCRIPTION
   This function initializes data structures used by uplink TCH.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msgi_init_ul_tch(gas_id_t gas_id)
{
   uint8 i = 0;
   tx_message_counter[gas_id]                           = 0;

   ul_tch_callback[gas_id].data_cb                      = NULL;
   ul_tch_callback[gas_id].metrics_cb                   = NULL;

   ul_facch_callback[gas_id].data_cb                    = NULL;
   ul_facch_callback[gas_id].metrics_cb                 = NULL;

   transmit_burst_tch[gas_id]                           = FALSE;
   burst_num_tch[gas_id]                                = 0;

   for(i = 0; i < NUM_TX_STORAGE; i++)
   {
      tx_data_storage[gas_id][i].ul_callback.data_cb    = NULL;
      tx_data_storage[gas_id][i].ul_callback.metrics_cb = NULL;
   }

   tx_tch_current[gas_id]                               = 0;
   tx_tch_next[gas_id]                                  = 0;
   tch_abort[gas_id]                                    = FALSE;
}

/*===========================================================================
FUNCTION ul_tch_add_storage

DESCRIPTION
   Allocate and fill in the next storage unit for uplink.

===========================================================================*/
static void ul_tch_add_storage
(
   ARFCN_T                   ARFCN[GL1_DEFS_FRAMES_IN_BLK_PERIOD],
   int16                     TXLEV[GL1_DEFS_FRAMES_IN_BLK_PERIOD],
   gl1_hw_channel_type       channel_type,
   gl1_msg_tx_tch_block_type block_type,
   gl1_msg_tx_cb_type        ul_callback,
   gas_id_t                  gas_id
)
{
   int16 TX_PWR_IDX[GL1_DEFS_FRAMES_IN_BLK_PERIOD];
   uint8 i;
   tx_data_storage_type *storage;

   /* Shortcut */
   storage = &tx_data_storage[gas_id][tx_tch_next[gas_id]];

   /* Translate GSM TXLEV to HW power index. ARFCN[0] sufficient since
   ** the MESSAGE (4 blocks) will be transmitted on the same band.  */
   gl1_msgi_map_txlev_to_pwr_idx(ARFCN[0], TXLEV, TX_PWR_IDX, 4, gas_id);

   /* Store parameters */
   for (i = 0; i < GL1_DEFS_FRAMES_IN_BLK_PERIOD; i++)
   {
       storage->ARFCN[i] = ARFCN[i];
       storage->power_index[i] = TX_PWR_IDX[i];
   }
   storage->channel_type = channel_type;
   storage->block = block_type;
   storage->ul_callback = ul_callback;

   /* Fill in static frame layer parameters */
   memset(&storage->signal_info, 0, sizeof(storage->signal_info));
   memset(&storage->burst_info, 0, sizeof(storage->burst_info));
   storage->burst_info.tsc            = tx_TSC[gas_id];
   storage->burst_info.bsic           = NO_BSIC;
   storage->burst_info.channel_type   = channel_type;
   storage->burst_info.timing_advance = gl1_msg_retrieve_ta(gas_id);
   storage->signal_info.tn            = gl1_msg_get_cs_timeslot(gas_id);

   tx_tch_next[gas_id] = (tx_tch_next[gas_id] + 1) % NUM_TX_STORAGE;
}




/*===========================================================================

FUNCTION gl1_msg_tx_tch_fr

DESCRIPTION
  This function is used to transmit a message on the TCH/FACCH on the next
  block boundary.  The message contents should span 4 frames ie 1 block.
  This function may be called at task level but should be called in the block
  before the data is to transmitted.   If data to be transmitted is for
  frame #4,5,6,7, then this function should be completed before start of
  frame #3, ie called in frames 0,1,2.

===========================================================================*/
void gl1_msg_tx_tch_fr ( ARFCN_T ARFCN[4], int16  TXLEV[4], uint8  *data,
#ifdef FEATURE_VAMOS_II
                         gl1_vamos2_tch_wait_type vamos2_tch_wait_state,
#endif
                         gas_id_t gas_id)
{
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
   // In case of GPLT and no RF, return default/canned result
   MSG_GERAN_HIGH_0_G("GPLT DBG: gl1_msg_tx_tch_fr: Return ");
   return;
#endif /* (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/

   /* Only loopback A & B implemented in s/w. C still handled my mdsp */
   if ( loopback_type[gas_id] == GL1_DEFS_LOOPBACK_TYPE_A ||
        loopback_type[gas_id] == GL1_DEFS_LOOPBACK_TYPE_B )
   {
       /* Overwrite the passed data with loopback data */
       data = gl1_loopback_data[gas_id];
   }

   if (data == NULL || ARFCN == NULL)
   {
       MSG_GERAN_ERROR_2_G("Rcvd NULL data %d or ARFCN %d pointers", data, ARFCN);
       return;
   }

   /* Validate current_chan */
   if (current_chan[gas_id] != GL1_HW_TCH_FS && current_chan[gas_id] != GL1_HW_TCH_EFS)
   {
      MSG_GERAN_ERROR_2_G("Invalid current_chan for tx_tch_fr %d, FN=%d", current_chan[gas_id], GSTMR_GET_FN_GERAN(gas_id));
   }

   /* Write the data */
   gl1_hw_write_tx_data(current_chan[gas_id], data, MAX_TCH_FS_OCTETS, NULL, 0,
                        gl1_msg_get_cs_timeslot(gas_id), &tx_data_storage[gas_id][tx_tch_next[gas_id]].tags,gas_id);

   /* Store the SM parameters */
   ul_tch_add_storage(ARFCN, TXLEV, current_chan[gas_id], GL1_MSG_TX_TCH_BLOCK_TCH, ul_tch_callback[gas_id],gas_id);

#ifdef FEATURE_VAMOS_II
   vamos2_ul_tch_wait_state[gas_id] = vamos2_tch_wait_state;
#endif
   
   /* register a handler if one does not exist */
   if (tx_message_counter[gas_id] == 0)
   {
       gl1_msgi_add_ft_handler( fr_tx_handler, GL1_MSG_FT_HDLR_UL,gas_id );
   }
   tx_message_counter[gas_id]++;
}



/*===========================================================================

FUNCTION gl1_msg_tx_tch_hr

DESCRIPTION
  This function is used to transmit a message on the TCH on the next
  block boundary.  The message contents should span 4 frames ie 1 block.
  This function may be called at task level but should be called in the block
  before the data is to transmitted.   If data to be transmitted is for
  frame #4,5,6,7, then this function should be completed before start of
  frame #3, ie called in frames 0,1,2.

===========================================================================*/
void gl1_msg_tx_tch_hr ( ARFCN_T ARFCN[4], int16  TXLEV[4], uint8  *data,
#ifdef FEATURE_VAMOS_II
                         gl1_vamos2_tch_wait_type vamos2_tch_wait_state,
#endif
                         gas_id_t gas_id)
{
   /* Only loopback A,B,D,E,F implemented in s/w. C still handled my mdsp */
   if ( loopback_type[gas_id] == GL1_DEFS_LOOPBACK_TYPE_A ||
        loopback_type[gas_id] == GL1_DEFS_LOOPBACK_TYPE_B ||
        loopback_type[gas_id] == GL1_DEFS_LOOPBACK_TYPE_D ||
        loopback_type[gas_id] == GL1_DEFS_LOOPBACK_TYPE_E ||
        loopback_type[gas_id] == GL1_DEFS_LOOPBACK_TYPE_F )
   {
      /* Overwrite passed data with loopback */
      data = gl1_loopback_data[gas_id];
   }

   if (data == NULL || ARFCN == NULL)
   {
      MSG_GERAN_ERROR_2_G("Rcvd NULL data %d or ARFCN %d pointers", data, ARFCN);
      return;
   }

   /* Write data to frame layer */
   gl1_hw_write_tx_data(GL1_HW_TCH_HS, data, MAX_TCH_HS_OCTETS, NULL, 0,
                        gl1_msg_get_cs_timeslot(gas_id), &tx_data_storage[gas_id][tx_tch_next[gas_id]].tags,gas_id);

   /* Store the SM parameters */
   ul_tch_add_storage(ARFCN, TXLEV, GL1_HW_TCH_HS, GL1_MSG_TX_TCH_BLOCK_TCH, ul_tch_callback[gas_id],gas_id);

#ifdef FEATURE_VAMOS_II
   vamos2_ul_tch_wait_state[gas_id] = vamos2_tch_wait_state;
#endif

   /* register a handler if one does not exist */
   if (tx_message_counter[gas_id] == 0)
   {
       gl1_msgi_add_ft_handler( hr_tx_handler, GL1_MSG_FT_HDLR_UL,gas_id );
   }
   tx_message_counter[gas_id]++;
}



/*===========================================================================

FUNCTION gl1_msg_tx_facch_fr

DESCRIPTION
  This function is used to transmit a message on the FACCH on the next
  block boundary.  The message contents should span 4 frames ie 1 block.
  This function may be called at task level but should be called in the block
  before the data is to transmitted.   If data to be transmitted is for
  frame #4,5,6,7, then this function should be completed before start of
  frame #3, ie called in frames 0,1,2.

===========================================================================*/
void gl1_msg_tx_facch_fr ( ARFCN_T ARFCN[4], int16  TXLEV[4], uint8  *data,
#ifdef FEATURE_VAMOS_II
                           gl1_vamos2_tch_wait_type vamos2_tch_wait_state,
#endif 
                           gas_id_t gas_id )
{
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
   // In case of GPLT and no RF, return default/canned result
   MSG_GERAN_HIGH_0_G("GPLT DBG: gl1_msg_tx_facch_fr: Return ");
   return;
#endif /* (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/

   if (data == NULL || ARFCN == NULL)
   {
      MSG_GERAN_ERROR_2_G("Rcvd NULL data %d or ARFCN %d pointers",
                data, ARFCN);
      return;
   }

   gl1_hw_write_tx_data(GL1_HW_FACCH, data, MAX_FACCH_OCTETS, NULL, 0,
                        gl1_msg_get_cs_timeslot(gas_id), &tx_data_storage[gas_id][tx_tch_next[gas_id]].tags,gas_id);

   /* Store the SM parameters */
   ul_tch_add_storage(ARFCN, TXLEV, GL1_HW_FACCH, GL1_MSG_TX_TCH_BLOCK_FACCH, ul_facch_callback[gas_id],gas_id);

#ifdef FEATURE_VAMOS_II
   vamos2_ul_tch_wait_state[gas_id] = vamos2_tch_wait_state;
#endif

   /* register a handler if one does not exist */
   if (tx_message_counter[gas_id] == 0)
   {
      gl1_msgi_add_ft_handler(fr_tx_handler, GL1_MSG_FT_HDLR_UL,gas_id);
   }

   tx_message_counter[gas_id]++;
}



/*===========================================================================

FUNCTION gl1_msg_tx_facch_hr

DESCRIPTION
  This function is used to transmit a half-rate message on the FACCH on the
  next block boundary.  The message contents should span 4 frames ie 1 block.
  This function may be called at task level but should be called in the block
  before the data is to transmitted.   If data to be transmitted is for
  frame #4,5,6,7, then this function should be completed before start of
  frame #3, ie called in frames 0,1,2.

===========================================================================*/
void gl1_msg_tx_facch_hr ( ARFCN_T ARFCN[4], int16  TXLEV[4], uint8  *data, boolean middle_facch_block,
#ifdef FEATURE_VAMOS_II
                           boolean middle_facch_block_vamos2,
                           gl1_vamos2_tch_wait_type vamos2_tch_wait_state,
#endif
                           gas_id_t gas_id )
{
   if (data == NULL || ARFCN == NULL)
   {
      MSG_GERAN_ERROR_2_G("Rcvd NULL data %d or ARFCN %d pointers",
                data, ARFCN);
      return;
   }

   middle_facch_hr_block[gas_id] = middle_facch_block;

#ifdef FEATURE_VAMOS_II
   middle_facch_hr_block_vamos2[gas_id] = middle_facch_block_vamos2;
   vamos2_ul_tch_wait_state[gas_id] = vamos2_tch_wait_state;
#endif

   gl1_hw_write_tx_data(GL1_HW_FACCH_HS, data, MAX_FACCH_OCTETS, NULL, 0,
                        gl1_msg_get_cs_timeslot(gas_id), &tx_data_storage[gas_id][tx_tch_next[gas_id]].tags,gas_id);

   /* Store the SM parameters */
   ul_tch_add_storage(ARFCN, TXLEV, GL1_HW_FACCH_HS, GL1_MSG_TX_TCH_BLOCK_FACCH, ul_facch_callback[gas_id],gas_id);

   /* register a handler if one does not exist */
   if (tx_message_counter[gas_id] == 0)
   {
       gl1_msgi_add_ft_handler(hr_tx_handler, GL1_MSG_FT_HDLR_UL,gas_id);
   }
   tx_message_counter[gas_id]++;
}





#ifdef FEATURE_DATA_GCSD
/*===========================================================================

FUNCTION gl1_msg_tx_cstch

DESCRIPTION
  This function is used to transmit a CS data message on the next
  block boundary.  The message contents should span 4 frames ie 1 block.
  This function may be called at task level but should be called in the block
  before the data is to transmitted.   If data to be transmitted is for
  frame #4,5,6,7, then this function should be completed before start of
  frame #3, ie called in frames 0,1,2.

===========================================================================*/
void gl1_msg_tx_cstch
(
   ARFCN_T             ARFCN[4],
   int16               TXLEV[4],
   uint8               *cs_data,
   uint8               *facch_data,
   boolean             facch_available,
  gas_id_t             gas_id
)
{
   gl1_hw_channel_type  chan = current_chan[gas_id];
   uint8                p_len, s_len = 0;

   if (!cs_data || !ARFCN || !facch_data )
   {
      MSG_GERAN_ERROR_3_G("Rcvd NULL cs_data %d or ARFCN %d or facch_data %d ptrs",
                cs_data, ARFCN, facch_data);
      return;
   }

   switch (current_chan[gas_id])
   {
      case GL1_HW_TCH_F144:
         p_len = TCHF144_OCTETS;

         if (facch_available)
         {
            chan = GL1_HW_TCH_F144_AND_FACCH;
            s_len = FACCH_OCTETS;
         }

         /* Only loopback A & B implemented in s/w. C still handled my mdsp */
         if ( loopback_type[gas_id] == GL1_DEFS_LOOPBACK_TYPE_A ||
              loopback_type[gas_id] == GL1_DEFS_LOOPBACK_TYPE_B )
         {
             cs_data = gl1_loopback_data[gas_id];
         }
         break;

      case GL1_HW_TCH_F96:
         p_len = TCHF96_OCTETS;

         if (facch_available)
         {
            chan = GL1_HW_TCH_F96_AND_FACCH;
            s_len = FACCH_OCTETS;
         }

         /* Only loopback A & B implemented in s/w. C still handled my mdsp */
         if ( loopback_type[gas_id] == GL1_DEFS_LOOPBACK_TYPE_A ||
              loopback_type[gas_id] == GL1_DEFS_LOOPBACK_TYPE_B )
         {
            cs_data = gl1_loopback_data[gas_id];
         }
         break;

      default:
         chan = GL1_HW_TCH_F96;
         p_len = TCHF96_OCTETS;
         MSG_GERAN_HIGH_1_G("Invalid channel type %d", current_chan[gas_id]);
         break;
    }

    /* Write the data to the frame layer */
    gl1_hw_write_tx_data(chan, cs_data, p_len, facch_data, s_len,
                         gl1_msg_get_cs_timeslot(gas_id), &tx_data_storage[gas_id][tx_tch_next[gas_id]].tags,gas_id);

    /* Store the SM parameters */
    ul_tch_add_storage(ARFCN, TXLEV, chan, GL1_MSG_TX_TCH_BLOCK_CSTCH, ul_tch_callback[gas_id],gas_id);

    /* register a handler if one does not exist */
    if (tx_message_counter[gas_id] == 0)
    {
        gl1_msgi_add_ft_handler( fr_tx_handler, GL1_MSG_FT_HDLR_UL,gas_id );
    }
    tx_message_counter[gas_id]++;
}
#endif



/*===========================================================================

FUNCTION gl1_msg_cfg_ul_tch

DESCRIPTION
  This function is used to configure the TSC and callbacks for the TCH.  The
  callbacks will be invoked by the frame tick handler as required.

===========================================================================*/
void gl1_msg_cfg_ul_tch (uint8 TSC, gl1_msg_tx_cb_type *cb,gas_id_t gas_id)
{
   if (!cb)
   {
      MSG_GERAN_ERROR_0_G("Rcvd NULL cb pointer");
      return;
   }

   tx_TSC[gas_id] = TSC;
   ul_tch_callback[gas_id].data_cb = cb->data_cb;
}



/*===========================================================================

FUNCTION gl1_msg_cfg_ul_facch

DESCRIPTION
  This function is used to configure the TSC and callbacks for the FACCH.
  The callbacks will be invoked by the frame tick handler as required.

===========================================================================*/
void gl1_msg_cfg_ul_facch (uint8 TSC, gl1_msg_tx_cb_type *cb,gas_id_t gas_id)
{
   if (!cb)
   {
      MSG_GERAN_ERROR_0_G("Rcvd NULL cb pointer");
      return;
   }

   tx_TSC[gas_id] = TSC;
   ul_facch_callback[gas_id].data_cb = cb->data_cb;
}

#ifdef FEATURE_DATA_GCSD
/*===========================================================================

FUNCTION gl1_msg_cs_data_rx  ,  gl1_msg_cs_data_tx

DESCRIPTION
  This function is used to transfer data to or from the GSM circuit
  switched data handler.  These functions may be called by the task level.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_cs_data_tx (uint8 **data,gas_id_t gas_id)
{
   uint8 *previous_buf;
   static uint8 *current_buf[NUM_GERAN_DATA_SPACES] =
                   { INITIAL_VALUE( NULL ) };

   if (data == NULL)
   {
      MSG_GERAN_ERROR_0_G("Rcvd NULL data ptr");
      return;
   }

   previous_buf = *data;
   if (previous_buf != current_buf[gas_id])
   {
      MSG_GERAN_HIGH_0_G("gl1_msg_cs_data_tx: msg mismatch");
   }

   gcsd_ota_tx_callback (&current_buf[gas_id], previous_buf, TRUE,gas_id);

   *data = current_buf[gas_id];
}
#endif

/*===========================================================================

FUNCTION gl1_msg_abort_tx_tch_facch_fr

DESCRIPTION
  This function is used to terminate any TCH bursts pending. Terminates
  after the block completes

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean gl1_msg_abort_tx_tch_facch_fr ( gas_id_t gas_id )
{
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
   // In case of GPLT and no RF, return default/canned result
   MSG_GERAN_HIGH_0_G("GPLT DBG: gl1_msg_abort_tx_tch_facch_fr: Return TRUE");
   return TRUE;
#endif /* (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/
   /* If we have no active handlers, nothing to do */
   if (tx_message_counter[gas_id] == 0) return TRUE;

   /* If the burst_num_tch[gas_id] is zero we have not yet issued a tx so we can
      abort immediately */
   if (burst_num_tch[gas_id] == 0)
   {
      /* Inform the frame layer we are aborting this transmit */
      (void) gl1_hw_abort_tx_bursts(&tx_data_storage[gas_id][tx_tch_current[gas_id]].tags,gas_id);
      tx_tch_current[gas_id] = 0;
      tx_tch_next[gas_id] = 0;
      tx_message_counter[gas_id] = 0;
      transmit_burst_tch[gas_id] = FALSE;
      tch_abort[gas_id] = FALSE;
      gl1_msgi_remove_ft_handler(fr_tx_handler,gas_id);
      return TRUE;
   }
   else
   {
      tch_abort[gas_id] = TRUE;
      return FALSE;
   }
}



/*===========================================================================

FUNCTION gl1_msg_abort_tx_tch_facch_hr

DESCRIPTION
  This function is used to terminate any TCH bursts pending. Terminates
  after the block completes

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean gl1_msg_abort_tx_tch_facch_hr ( gas_id_t gas_id )
{
   /* If we have no active handlers, nothing to do */
   if (tx_message_counter[gas_id] == 0) return TRUE;

   /* If the burst_num_tch[gas_id] is zero we have not yet issued a tx so we can
      abort immediately */
   if (burst_num_tch[gas_id] == 0)
   {
      /* Inform the frame layer we are aborting this transmit */
      (void) gl1_hw_abort_tx_bursts(&tx_data_storage[gas_id][tx_tch_current[gas_id]].tags,gas_id);
      tx_tch_current[gas_id] = 0;
      tx_tch_next[gas_id] = 0;
      tx_message_counter[gas_id] = 0;
      transmit_burst_tch[gas_id] = FALSE;
      tch_abort[gas_id] = FALSE;
      gl1_msgi_remove_ft_handler(hr_tx_handler,gas_id);
      return TRUE;
   }
   else
   {
      tch_abort[gas_id] = TRUE;
      return FALSE;
   }
}



/****************************************************************************
 ****************************************************************************
 ****************************************************************************
                            LOCAL   FUNCTIONS
 ****************************************************************************
 ****************************************************************************
 ***************************************************************************/

/*===========================================================================

FUNCTION hr_tx_handler

DESCRIPTION
  This function should be called every frame on the 26 frame multiframe. The
  handler is installed as part of a request to tx a msg by gl1_msg_tx_tch()
  or gl1_msg_tx_facch().
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
static boolean hr_tx_handler
(
   boolean  start_of_block,     /* next frame is start of block */
   uint32   time_stamp,          /* currently not used           */
   gas_id_t gas_id
)
{
    boolean remove_handler = FALSE;
    boolean encode_flag    = FALSE;
    static  uint8 frame_count[NUM_GERAN_DATA_SPACES] =
                    { INITIAL_VALUE( 0 ) };

    uint8 callback_burst = 1;
    uint8 trans_burst = 2;
    tx_data_storage_type *storage = &tx_data_storage[gas_id][tx_tch_current[gas_id]];
    uint8 delay_after_burst = 2;

    /* Is this the start of a 4 frame block */
    if (start_of_block)
    {
        if (((storage->block == GL1_MSG_TX_TCH_BLOCK_FACCH && burst_num_tch[gas_id] == 0) ||
             !(storage->block == GL1_MSG_TX_TCH_BLOCK_FACCH)) && tx_message_counter[gas_id])
        {
            transmit_burst_tch[gas_id] = TRUE;
            burst_num_tch[gas_id] = 0;
            frame_count[gas_id] = 0;
            encode_flag = TRUE;
        }
    }

#ifdef FEATURE_VAMOS_II
    if((vamos2_ul_tch_wait_state[gas_id] == WAIT_AFTER_BLOCK_START) && (burst_num_tch[gas_id] == 1) && (frame_count[gas_id] == 2))
    {
      transmit_burst_tch[gas_id] = FALSE;
      vamos2_ul_tch_wait_state[gas_id] = NO_WAIT;
    }
    else if(burst_num_tch[gas_id])
    {
      transmit_burst_tch[gas_id] = TRUE;
    }
	
    if(middle_facch_hr_block[gas_id] || middle_facch_hr_block_vamos2[gas_id])
    {
      delay_after_burst = (middle_facch_hr_block_vamos2[gas_id] == TRUE)?3:2;
    }
#endif	

    /* Now transmit the burst if there are bursts to transmit */
    /* Transmit every other frame for half rate */
    if ( transmit_burst_tch[gas_id] )
    {
#ifdef FEATURE_VAMOS_II
        if ( (burst_num_tch[gas_id] == delay_after_burst) && (middle_facch_hr_block[gas_id] || middle_facch_hr_block_vamos2[gas_id]) && storage->block == GL1_MSG_TX_TCH_BLOCK_FACCH)
#else
        if ( (burst_num_tch[gas_id] == delay_after_burst) && middle_facch_hr_block[gas_id] && storage->block == GL1_MSG_TX_TCH_BLOCK_FACCH)
#endif
        {
            /* This introduces a 2 frame delay between the 2nd and
               3 bursts of a middle HR fACCH block where the SACCH occurs
               The middle FACCH block bursta are on Fnmod26 = 8,10,13,15
               for HR=0
            */
            middle_facch_hr_block[gas_id] = FALSE;
#ifdef FEATURE_VAMOS_II
            middle_facch_hr_block_vamos2[gas_id] = FALSE;
#endif
        }
        else if ( frame_count[gas_id]++%2 == 0)
        {
            if (gl1_msg_is_ciphering_enabled(gas_id))
            {
                gl1_hw_update_ciphering_fn(gas_id);
            }

            /* Fill in dynamic frame layer parameters */
            storage->signal_info.tx_power_index = storage->power_index[burst_num_tch[gas_id]];
            storage->burst_info.arfcn = storage->ARFCN[burst_num_tch[gas_id]];
            storage->burst_info.encode_flag = encode_flag;
            storage->burst_info.burst_num = burst_num_tch[gas_id];

            /* Issue the tx command */
            gl1_hw_tx_dtm_gsm(&storage->signal_info, &storage->burst_info, &storage->tags,gas_id);
            burst_num_tch[gas_id]++;
        }

    }

    if (storage->block == GL1_MSG_TX_TCH_BLOCK_FACCH)
    {
        callback_burst = 3;
        trans_burst = 4;
    }
    /* Do the callback in the 1st burst frame ie right before burst is sent*/
    if (burst_num_tch[gas_id] == callback_burst)
    {
        if (storage->ul_callback.data_cb != NULL)
        {
            storage->ul_callback.data_cb(gas_id);
        }
    }

    /* Now was this the last burst */
    if (burst_num_tch[gas_id] == trans_burst )
    {
        transmit_burst_tch[gas_id] = FALSE;
        burst_num_tch[gas_id] = 0;
        tx_tch_current[gas_id] = (tx_tch_current[gas_id] + 1) % NUM_TX_STORAGE;

        /* Are there any more messages to transmit */
        if (--tx_message_counter[gas_id] == 0)
        {
            remove_handler = TRUE;
        }
        if (tch_abort[gas_id])
        {
            tch_abort[gas_id] = FALSE;
            tx_message_counter[gas_id] = 0;
            transmit_burst_tch[gas_id] = FALSE;
            remove_handler = TRUE;

            /*
            This is needed because we could be at a stage in our state machine
            when we've stored the rx/tx data but haven't retrieved them.  The
            TCH rx/tx fifo counters in gl1_msg_com.c will then be out of sync
            causing us to be off by a block in the nect call*/

            tx_tch_current[gas_id] = 0;
            tx_tch_next[gas_id] = 0;
        }
    }

    return(remove_handler);

}/*lint !e715*/



/*===========================================================================

FUNCTION fr_tx_handler

DESCRIPTION
  This function should be called every frame on the 26 frame multiframe. The
  handler is installed as part of a request to tx a msg by gl1_msg_tx_tch()
  or gl1_msg_tx_facch() or gl1_msg_tx_cstch().
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
static boolean fr_tx_handler
(
   boolean start_of_block,     /* next frame is start of block */
   uint32  time_stamp,         /* currently not used           */
   gas_id_t gas_id
)
{
    boolean remove_handler = FALSE;
    boolean encode_flag    = FALSE;
    tx_data_storage_type *storage = &tx_data_storage[gas_id][tx_tch_current[gas_id]];

    /* Is this the start of a 4 frame block */
    if (start_of_block)
    {
        /* tx_message_counter[gas_id] indicates number of messages to transmit */
        if (tx_message_counter[gas_id])
        {
            transmit_burst_tch[gas_id] = TRUE;
            burst_num_tch[gas_id] = 0;
            encode_flag = TRUE;
        }
    }

#ifdef FEATURE_VAMOS_II
    if((vamos2_ul_tch_wait_state[gas_id] == WAIT_AFTER_BLOCK_START) && (burst_num_tch[gas_id] == 1))
    {
      transmit_burst_tch[gas_id] = FALSE;
      vamos2_ul_tch_wait_state[gas_id] = NO_WAIT;
    }
    else if((vamos2_ul_tch_wait_state[gas_id] == WAIT_AFTER_BURST2) && (burst_num_tch[gas_id] == 3))
    {		 
      transmit_burst_tch[gas_id] = FALSE; 
      vamos2_ul_tch_wait_state[gas_id] = NO_WAIT;
    }
    else if(burst_num_tch[gas_id])
    {
      transmit_burst_tch[gas_id] = TRUE;
    }
#endif	/*FEATURE_VAMOS_II*/

    /* Now transmit the burst if there are bursts to transmit */
    if (transmit_burst_tch[gas_id])
    {
        if (gl1_msg_is_ciphering_enabled(gas_id))
        {
            gl1_hw_update_ciphering_fn(gas_id);
        }

        /* Fill in dynamic frame layer parameters */
        storage->signal_info.tx_power_index = storage->power_index[burst_num_tch[gas_id]];
        storage->burst_info.arfcn = storage->ARFCN[burst_num_tch[gas_id]];
        storage->burst_info.encode_flag = encode_flag;
        storage->burst_info.burst_num   = burst_num_tch[gas_id];

        /* Issue the tx command */
        gl1_hw_tx_dtm_gsm(&storage->signal_info, &storage->burst_info, &storage->tags,gas_id);
        burst_num_tch[gas_id]++;
    }

    /* Do the callback in the 1st burst frame ie right before burst is sent*/
    if (burst_num_tch[gas_id] == 2 /*burst*/)
    {
        if (storage->ul_callback.data_cb != NULL)
        {
            storage->ul_callback.data_cb(gas_id);
        }
    }
    /* Now was this the last burst */
    if (burst_num_tch[gas_id] == 4)
    {
        transmit_burst_tch[gas_id] = FALSE;
        burst_num_tch[gas_id] = 0;
        tx_tch_current[gas_id] = (tx_tch_current[gas_id] + 1) % NUM_TX_STORAGE;

        /* Are there any more messages to transmit */
        if (--tx_message_counter[gas_id] == 0)
        {
            remove_handler = TRUE;
        }
        if (tch_abort[gas_id])
        {
            tch_abort[gas_id] = FALSE;
            tx_message_counter[gas_id] = 0;
            transmit_burst_tch[gas_id] = FALSE;
            remove_handler = TRUE;

            /*
            This is needed because we could be at a stage in our state machine
            when we've stored the rx/tx data but haven't retrieved them.  The
            TCH rx/tx fifo counters in gl1_msg_com.c will then be out of sync
            causing us to be off by a block in the nect call*/

            tx_tch_current[gas_id] = 0;
            tx_tch_next[gas_id] = 0;
        }
    }
    return(remove_handler);

}/*lint !e715*/


/*=============================================================================
FUNCTION gl1_get_dtx_used_this_period

DESCRIPTION
   This function returns if DTX was used during the current SACCH reporting
   period then resets itself.

SIDE EFFECT:  dtx_used is reset for next reporting period

============================================================================= */
boolean gl1_get_dtx_used_this_period( gas_id_t gas_id )
{
    boolean was_dtx_used = gl1_dtx_used[gas_id];

    gl1_reset_dtx_used_flag(gas_id);

    return was_dtx_used;
}

/*=============================================================================
FUNCTION gl1_reset_dtx_used_flag

DESCRIPTION
   This function resets the reporting flag gl1_dtx_used[gas_id].

============================================================================= */
void gl1_reset_dtx_used_flag( gas_id_t gas_id )
{
    gl1_dtx_used[gas_id] = FALSE;

    MSG_GERAN_LOW_0_G("gl1_dtx_used[gas_id] RESET");
}

/*=============================================================================
FUNCTION gl1_set_dtx_used_flag

DESCRIPTION
   This function sets the reporting flag gl1_dtx_used[gas_id].

============================================================================= */
void gl1_set_dtx_used_flag( gas_id_t gas_id )
{
    gl1_dtx_used[gas_id] = TRUE;

    MSG_GERAN_LOW_0_G("gl1_dtx_used[gas_id] SET");
}

