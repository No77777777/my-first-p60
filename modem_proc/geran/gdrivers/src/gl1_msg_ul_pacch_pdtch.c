/*===========================================================================

     IMPLEMENTATION OF GPRS NPL1 UPLINK PACCH PDTCH FUNCTIONS

GENERAL DESCRIPTION
   This file implements GPRS NPL1 uplink PACCH PDTCH functions.

   Some PACCH and PDTCH points of consideration are:
   1.The processing is based on a block period consisting of 4 frames. Messages
   may be submitted only in Frame 2 and 3 of a block. In the fourth frame of the
   block, if there are messages to be sent then 4 transmit bursts are scheduled.
   A transmit notification callback is invoked on the third burst.

   2.Dynamic allocation adds some additional complexity since submitted messages
   may not always be transmitted next block. Transmission is dependent on
   whether an USF was decoded in the previous block.

   3.USF Granularity is an aspect of dynamic allocation, where in the next 4
   blocks are considered allocation when an USF is decoded.


EXTERNALIZED FUNCTIONS
gl1_msgi_ul_pacch_pdtch_init
gl1_msgi_ul_pacch_pdtch_terminate
gl1_msg_cfg_ul_pacch_pdtch
gl1_msg_set_ul_alloc_info
gl1_msg_set_usf_alloc_info
gl1_msg_tx_pacch_pdtch_fixed
gl1_msg_tx_pacch_pdtch_dynamic
gl1_msg_set_pacch_pdtch_dynamic_signal_info
gl1_msgi_ul_pacch_pdtch_hdlr


INITIALIZATION AND SEQUENCING REQUIREMENTS
The following is the desired order in which functions should be called-
gl1_msgi_ul_pacch_pdtch_init
gl1_msg_set_ul_alloc_info
gl1_msg_cfg_ul_pacch_pdtch
  gl1_msg_tx_pacch_pdtch_fixed
  gl1_msg_tx_pacch_pdtch_dynamic
  gl1_msg_set_pacch_pdtch_dynamic_signal_info
  gl1_msg_set_usf_alloc_info
    gl1_msgi_ul_pacch_pdtch_hdlr
gl1_msgi_ul_pacch_pdtch_terminate

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_msg_ul_pacch_pdtch.c#1 $ $DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who     what, where, why
--------   ---     ------------------------------------------------------- 
11/05/15   pjr     CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
09/04/15   cja     CR819956 G2T changes to support new RF API's  FEATURE_NEW_RF_API
19/03/15   shr     CR806808   0x5237 GPRS Tx Schedule to log OTA TN rather than slammed TN
13/02/15   pa      CR792105: Allow either GBTA or IRAT. 
11/12/14   sp      CR765250: correct prio issued for RRBP messages.
31/10/14   xz      CR748816 Extend dual data space for building RF script variable 
11/09/14   am      CR677361 Addressing KW GL1 P1 errors
31/07/14   xz      CR697883 Not allow GBTA if current idle frame is doing TDS IRAT in xfer mode
30/05/14   jj      CR656854  Resolve Critical Klocwork errors in gdrivers code space
20/06/14   pjr     DATA+MMS feature - USF utilisation calculation
21/05/14   fm      CR668521 correct the array index to make channel type align with MCS 
15/08/13   sk      CR524039 GBTA bringup changes
29/07/13   sk      CR519663 Partial QBTA bringup changes
20/05/13   cs      TSTS Updates
13/03/13   mc      CR.459487 : COEX Support - GERAN arbitrator framework
28/10/10   cja     Tidy-up changes for CR251982 (no change of actual code)
06/10/10   cja     CR251982 Do not change Tx schedule mid-block
29/07/10   ws      Added Missing PACKED_POST for TX schedule log packet
29/07/10   ws      Added Missing PACKED_POST for TX schedule log packet
23/09/09   ps      Resolve Klocwork Buffer Overflow errors
05/08/09   ws      Removed FEATURE_GSM_MDSP_DTM it's now mainlined
14/03/08   ip      Added PSHO chnages in feature FEATURE_GPRS_PS_HANDOVER
03/10/07   og      Esnure the hdr ptr is not NULL in SRB loopback. Resolves
                   CR129485.
05/03/07   ws      Removed filtered Lint warnings
28/11/06   ws      High Lint warning fixes
31/07/06   agv     CR85810 NB AB TSC, and TA fix.
16/03/06   og      Take timeslot gaps into account when deciding on Tta/Trb
                   or Ttb/Tra. Resolves CR 88935 and CR 88975.
03/09/06   ws      Set Header ptr to NULL as it's not used in EGPRS SRB loopback CR89059
02/17/06   gfr     Added modulation for power backoff to RF driver.
01/04/05   gfr     Latch USF list at start of block to avoid problems if
                   PL1 changes it before the USFs come back from the DL side
11/29/05   gfr     Regenerate channel list after getting scheduled results
                   to stop issuing tx commands for unused slots.
11/22/05   gfr     Change tx scheduled and discarded callbacks to take
                   structures instead of just message data pointers.  Return
                   the timeslot and offset_to_sob for scheduled transmits.
10/26/05   gfr     Make USF granularity message show 4 or 1 instead of 1 or 0
10/18/05   gfr     Abort immediately if we are waiting for block start,
                   this cleans out USF granularity allocations properly
10/14/05   gfr     Trace scheduled and discarded callbacks
09/30/05   gfr     Abort discarded transmits - this allows the tx clock
                   to be disabled for two frames in such blocks
08/05/05   gfr     Support USF granularity with EDA
07/29/05   gfr     DTM support
06/02/05   gfr     Make handler static
04/12/05   gfr     Consolidate GSM hw layer parameters
04/08/05   gfr     Use new interface to specify saving of untxed messages
04/08/05   gfr     Combined GSM and GPRS channel types
04/03/05   gfr     Start with GMSK for loopback to avoid spurious cap warnings
03/23/05   gfr     Enforce power class E2 (limit 8PSK uplink power)
03/22/05   gfr     Use tx tag to keep track of encoded data.
03/15/05   gfr     Support looping back a different downlink timeslot.
01/14/05   gfr     Send the right channel type for loopback transmits.
12/07/04   yhong   Lint cleanup
12/06/04   yhong   Support for FEATURE_GSM_MDSP_EGPRS
10/01/04   gfr     Set the TSC to 0 for access bursts.
09/15/04   gfr     Delay applying usf granularity allocs.
07/22/04   gfr     EGPRS support.
06/13/04   gfr     New more robust and simpler uplink handler.
05/07/04   gfr     Fix bug with adding handler for usf granularity.
03/05/04   gfr     Lint cleanup.
02/26/04   gfr     Added missing arfcn_to_word prototype.
01/27/04   gfr     Support for logging tx scheduled results.
12/15/03   gfr     Fix bug in multislot usf granularity code.
11/05/03   gfr     Support for quad-band.
06/02/03   gfr     Do not abort uplink if we have already written message
                   data to the mdsp.
05/28/03   sr/gfr  Abort USF granularity allocations.
05/12/03   gfr     Remove burst_schedule code.
05/01/03   gfr     Change featuring from FEATURE_GSM to FEATURE_GSM_GPRS
04/04/03   gfr     Call gl1_msgi_burst_schedule when scheduling bursts.
02/24/03   gfr     Lint cleanup.
12/12/02   sg      Suppressed a redundent error msg
12/03/02   sg      Fixed FA msg transmission problem when granularity is set
11/06/02   sg      Removed the check in setting USFs
10/10/02   gr      Changed GPRS TX interface to accept TXLEV vs. power index.
09/19/02   gr      Fix timing advance problem when changing coding schemes.
09/09/02   gr      Coding scheme msg length lut moved to gl1_defs.h
09/06/02   sg      arfcn and tx ss chnaged to take effect next block
07/16/02   gr      Set timing advance to 0 for access bursts
06/28/02   thh      Featurize this whole module under FEATURE_GSM.  This is
                    required to allow dualmode build to make single mode
                    (WCDMA) executable.
04/16/02   bk      Added abort functionality, invoking sched. cb even if no tx
                   took place.
03/14/02   bk      Got rid of some debug messages and fixed a bug to support
                   timeslots > 4
03/07/02   sg      Implemented dyanamic allocation- code framework changed
                     (USF implementation not tested)
01/15/02   bk      Changed table order to reflect coding enum order change
??/??/01   bk      Initial revision

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#ifdef CUST_H
#include "customer.h"
#endif


#include "comdef.h"
#include "gl1_defs.h"
#include "gl1_defs_g.h"
#include "gl1_msg_g.h"
#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "gl1_msg_pdch.h"
#include "gl1_msgi.h"
#include "msg.h"
#include "gtmrs.h"
#include "gtmrs_g.h"

#include "geran_dual_sim.h"
#include "geran_multi_sim.h"

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/

/* We are a power class E2 mobile, which means capping 8PSK transmit power */
#define POWER_CLASS_E2


/* The initial setting for the number of USF granularity blocks, 3 instead
   of 4 because the current block is not counted */
#define INIT_USF_GRAN_BLKS 3

/* The offset from when the callback happens to the start of the block */
#define UL_PDCH_OFFSET_TO_SOB  1

/* Macro to get the tx modulation type for a given coding scheme */
#define MODULATION(cs)  \
  (IS_8PSK_MODULATION(cs) ? GL1_DEFS_MODULATION_8PSK : GL1_DEFS_MODULATION_GMSK)

#define FRAMES_PER_BLOCK    4

#ifdef FEATURE_GSM_MDSP_EGPRS

/* Size of the MAC header in the EGPRS uplink data block */
#define GL1_EGPRS_UL_MAC_HDR_OCTET_SIZE  (1U)
#define GL1_EGPRS_UL_RLC_HDR_OCTET_SIZE  (2U)

#define GL1_EGPRS_UL_HDR_OCTET_SIZE  ((GL1_EGPRS_UL_MAC_HDR_OCTET_SIZE) + (GL1_EGPRS_UL_RLC_HDR_OCTET_SIZE))

#endif

/* States for the handler */
typedef enum
{
   UL_STATE_INACTIVE,
   UL_STATE_WAITING_FOR_BLOCK_START,
   UL_STATE_FRAME_0,
   UL_STATE_FRAME_1,
   UL_STATE_FRAME_2,
   UL_STATE_FRAME_3
} ul_pdch_state_type;


/* Storage for a single message */
typedef struct
{
   uint8                 *msg_data;
   gl1_defs_coding_type  coding_scheme;
   gl1_hw_pkt_msg_type   alloc_type;
   gl1_defs_tn_type      tn;
   gl1_hw_tx_tags_type   tags;
   boolean               discarded;
         geran_pdu_priority_t  priority;
} ul_pdch_msg_type;


/* Possible allocation types for an uplink channel */
typedef enum
{
  CHANNEL_FREE,  /* Unallocated */
  CHANNEL_FA,    /* Fixed allocation */
  CHANNEL_DA     /* Dynamic allocation */
} ul_pdch_channel_allocation_type;


/* Uplink channel structure, one per timeslot */
typedef struct
{
   ul_pdch_channel_allocation_type allocation;
   boolean                         scheduled;    /* if fw is scheduling this tx */
   int16                           power_level;  /* power from PL1 */
   int16                           power_index;  /* power for frame layer */
   gl1_defs_modulation_type        modulation;
         gl1_hw_channel_type             channel_type;
         geran_pdu_priority_t            priority;
} ul_pdch_channel_type;


/* Parameters particular to a block */
typedef struct
{
   /* ARFCN information for the block */
   ARFCN_T                    arfcn[GL1_DEFS_FRAMES_IN_BLK_PERIOD];

   /* Which timeslot we are camped on for this block - used by PL1 */
   gl1_defs_tn_type           camp_tn;

   /* Messages submitted for transmission this block */
   ul_pdch_msg_type           msgs[GL1_DEFS_MAX_ASSIGNED_UL_TS];
   uint8                      num_msgs;

   /* Tags for active (non-discarded) messages */
   gl1_hw_tx_tags_type        tags[GL1_DEFS_MAX_ASSIGNED_UL_TS];
   uint8                      num_tags;
   boolean                    generate_tags;

   /* Channel allocation for each slot */
   ul_pdch_channel_type       channels[GL1_DEFS_SLOTS_IN_FRAME];

   /* Combined and sorted channel info */
   gl1_hw_tx_pkt_burst_type   burst_info;
   gl1_hw_tx_signal_type      signal_info[GL1_DEFS_MAX_ASSIGNED_UL_TS];
   uint8                      num_channels;

   /* Flag if we are using loopback data this block - EGPRS only */
   boolean                    srb_loopback;
   gl1_defs_tn_type           loopback_tn[GL1_DEFS_SLOTS_IN_FRAME];

   /* Keep track of which USFs are mapped to which slots for this block */
   gl1_defs_usf_type          usf_list[GL1_DEFS_SLOTS_IN_FRAME];

   /* Number of uplink timeslots assigned to the MS including
    * timeslot gaps in the case of non-contiguous timeslots.
    * This is used to decide on using Tta/Trb or Ttb/Tra when
    * attempting to perform power monitors.
    */
   uint8                      num_ul_timeslots;

} ul_pdch_block_params_type;


/* Collected ul handler parameters */
typedef struct
{
   /* Static configuration parameters */
   uint8                  tsc;
   /* Added AB TSC Sync Sequence bits. */
   uint8                  ab_tsc;
   uint8                  bsic;
   gl1_msg_tx_pkt_cb_type callbacks;

   /* State variables */
   ul_pdch_state_type     state;
   boolean                allow_data_writes;
   boolean                abort;

   /* The master USF list */
   gl1_defs_usf_type      usf_list[GL1_DEFS_SLOTS_IN_FRAME];

   /* Keep track of allocated slots - used to map scheduled tx to a timeslot */
   gl1_defs_tn_type       allocated_slots[GL1_DEFS_MAX_ASSIGNED_DL_TS];
   uint8                  num_allocated_slots;

   /* USF granularity variables */
   boolean                usf_granularity;
   boolean                usf_granularity_allocation;
   boolean                usf_granularity_allocation_pending;
   uint8                  usf_granularity_blocks[GL1_DEFS_SLOTS_IN_FRAME];

   /* USF utilisation variables */
   uint16 blanked_bursts_mask[FRAMES_PER_BLOCK];
   uint16 scheduled_blocks_mask;
   uint8 num_scheduled_blocks;
   uint8 num_blanked_blocks;
   uint16 num_consecutive_blanked_blocks;

   /* Block parameters, ping-pong style */
   ul_pdch_block_params_type block_params[2];
   uint8 this_block_ptr;
   uint8 next_block_ptr;

} ul_pdch_params_type;


/* U/L parameters */
static ul_pdch_params_type ul_pdch_params[NUM_GERAN_DATA_SPACES];


/* Lookup table for mapping coding scheme to channel type */
static const gl1_hw_channel_type cs_to_chan_type_lut[] =
{
   GL1_HW_PDTCHU_CS1, /* CS-1 coding scheme   */
   GL1_HW_PDTCHU_CS2, /* CS-2 coding scheme   */
   GL1_HW_PDTCHU_CS3, /* CS-3 coding scheme   */
   GL1_HW_PDTCHU_CS4, /* CS-4 coding scheme   */
   GL1_HW_PACCHU,     /* Access coding scheme */
   GL1_HW_PACCHU_EXT, /* Extended access      */
   GL1_HW_PDTCHU_MCS1,
   GL1_HW_PDTCHU_MCS2,
   GL1_HW_PDTCHU_MCS3,
   GL1_HW_PDTCHU_MCS4,
   GL1_HW_PDTCHU_MCS5,
   GL1_HW_PDTCHU_MCS6,
   GL1_HW_PDTCHU_MCS7,
   GL1_HW_PDTCHU_MCS8,
   GL1_HW_PDTCHU_MCS9
};

/* For converting coding scheme to length */
extern const uint8 cs_to_msg_length_lut[];

/* Used for logging */
extern uint16 arfcn_to_word(ARFCN_T arfcn);

extern uint8 gpl1_gprs_ts_conv_unslam(uint8 slammed, gas_id_t gas_id );


/* Handler */
static boolean gl1_msgi_ul_pacch_pdtch_hdlr
(
   boolean start_of_block,
   uint32   time_stamp,
   gas_id_t gas_id
);


/*===========================================================================

FUNCTION ul_pdch_default_scheduled_callback

DESCRIPTION
   Default scheduled data callback if nothing else is installed.

===========================================================================*/
void ul_pdch_default_scheduled_callback
(
   gl1_msg_tx_scheduled_data_type msgs_txd[ GL1_DEFS_MAX_UL_MSGS ],
   uint8 num_msg_txd,
   gas_id_t gas_id
)
{
   MSG_GERAN_ERROR_0_G("No scheduled data callback installed!");

}


/*===========================================================================

FUNCTION ul_pdch_default_discarded_callback

DESCRIPTION
   Default discarded data callback if nothing else is installed.

===========================================================================*/
void ul_pdch_default_discarded_callback
(
   gl1_msg_tx_discarded_data_type msgs_discarded[ GL1_DEFS_MAX_UL_MSGS ],
   uint8 num_msg_txd,
   gas_id_t gas_id
)
{
   MSG_GERAN_ERROR_0_G("No discarded data callback installed!");

}

/*===========================================================================

FUNCTION ul_pdch_default_usf_utilisation_callback

DESCRIPTION
   Default usf utilisation callback if nothing else is installed.

===========================================================================*/
void ul_pdch_default_usf_utilisation_callback( gl1_msg_usf_utilisation_data_type usf_utilisation,
                                               gas_id_t                          gas_id)
{
   MSG_GERAN_ERROR_0_G("No USF utilisation callback installed!");
}

/*===========================================================================

FUNCTION ul_pdch_discard_message

DESCRIPTION
   Discards the given message.  Called when an error occurs during
   PL1 scheduling.

===========================================================================*/
static void ul_pdch_discard_message (uint8 *msg_data,gas_id_t gas_id)
{
   gl1_msg_tx_discarded_data_type msg_discarded;

   /* Fill in the callback structure */
   msg_discarded.msg_data = msg_data;
   msg_discarded.offset_to_sob = 0;

   /* Make the callback */
   ul_pdch_params[gas_id].callbacks.discarded_callback(&msg_discarded, 1,gas_id);
}



/*===========================================================================

FUNCTION ul_pdch_init_block_params

DESCRIPTION
   This function initializes a block parameter structure.

===========================================================================*/
static void ul_pdch_init_block_params (ul_pdch_block_params_type *block_params)
{
   uint8 tn;

   block_params->num_msgs = 0;
   block_params->num_tags = 0;
   block_params->num_channels = 0;
   block_params->num_ul_timeslots = 0;
   block_params->srb_loopback = FALSE;

   for (tn = 0; tn < GL1_DEFS_SLOTS_IN_FRAME; tn++)
   {
      block_params->channels[tn].allocation = CHANNEL_FREE;
      block_params->channels[tn].scheduled = FALSE;
   }
}



/*===========================================================================

FUNCTION gl1_msgi_ul_pacch_pdtch_init

DESCRIPTION
   This function initializes data structures that are used by all PDCHs.

===========================================================================*/
void gl1_msgi_ul_pacch_pdtch_init( gas_id_t gas_id )
{
   uint8 i;

   L1_STATE_CHANGE(ul_pdch_params[gas_id].state = UL_STATE_INACTIVE, "UL_STATE_INACTIVE");
   ul_pdch_params[gas_id].allow_data_writes = TRUE;
   ul_pdch_params[gas_id].callbacks.scheduled_callback = ul_pdch_default_scheduled_callback;
   ul_pdch_params[gas_id].callbacks.discarded_callback = ul_pdch_default_discarded_callback;
   ul_pdch_params[gas_id].usf_granularity = FALSE;
   ul_pdch_params[gas_id].usf_granularity_allocation = FALSE;
   ul_pdch_params[gas_id].usf_granularity_allocation_pending = FALSE;
   ul_pdch_params[gas_id].abort = FALSE;

   ul_pdch_params[gas_id].scheduled_blocks_mask = 0;
   ul_pdch_params[gas_id].num_scheduled_blocks = 0;
   ul_pdch_params[gas_id].num_blanked_blocks = 0;
   ul_pdch_params[gas_id].num_consecutive_blanked_blocks = 0;

   for (i = 0; i < FRAMES_PER_BLOCK; i++)
   {
     ul_pdch_params[gas_id].blanked_bursts_mask[i] = 0;
  }

   for (i = 0; i < GL1_DEFS_SLOTS_IN_FRAME; i++)
   {
      ul_pdch_params[gas_id].usf_list[i] = GL1_DEFS_PDCH_UNALLOCATED;
      ul_pdch_params[gas_id].usf_granularity_blocks[i] = 0;
   }

   /* Initialize the block parameters */
   ul_pdch_init_block_params(&ul_pdch_params[gas_id].block_params[0]);
   ul_pdch_init_block_params(&ul_pdch_params[gas_id].block_params[1]);

}



/*===========================================================================

FUNCTION gl1_msgi_ul_pacch_pdtch_terminate

DESCRIPTION
   Terminates all common PDCH processing

===========================================================================*/
void gl1_msgi_ul_pacch_pdtch_terminate( gas_id_t gas_id )
{
   /* Make sure we are not in the middle of something */
   if (ul_pdch_params[gas_id].state != UL_STATE_WAITING_FOR_BLOCK_START)
   {
      MSG_GERAN_ERROR_1_G("Terminating ul while in state %d", ul_pdch_params[gas_id].state);

      /* Disable any usf granularity, try to abort and hope for the best */
      ul_pdch_params[gas_id].usf_granularity = FALSE;
      gl1_msg_abort_ul_pacch_pdtch(gas_id);
   }
}



/*=============================================================================

FUNCTION gl1_msg_abort_ul_pacch_pdtch

DESCRIPTION
  This function aborts any pending uplink.  If a block is already in progress
  it will wait until it is complete.

============================================================================= */
void gl1_msg_abort_ul_pacch_pdtch( gas_id_t gas_id )
{
   ul_pdch_block_params_type *block_params;
   uint16 i;

   switch (ul_pdch_params[gas_id].state)
   {
      /* If we are inactive, nothing to do */
      case UL_STATE_INACTIVE:
         break;

      /* If we are waiting for a block start we can abort immediately */
      case UL_STATE_WAITING_FOR_BLOCK_START:
         /* Abort any messages written to the frame layer */
         block_params = &ul_pdch_params[gas_id].block_params[ul_pdch_params[gas_id].next_block_ptr];
         for (i = 0; i < block_params->num_msgs; i++)
         {
            (void)gl1_hw_abort_tx_bursts(&block_params->msgs[i].tags,gas_id);
         }

         /* Clean up any pending USF granularity blocks */
         ul_pdch_params[gas_id].usf_granularity_allocation_pending = FALSE;
         if (ul_pdch_params[gas_id].usf_granularity_allocation)
         {
            for (i = 0; i < GL1_DEFS_SLOTS_IN_FRAME; i++)
            {
               ul_pdch_params[gas_id].usf_granularity_blocks[i] = 0;
            }
            ul_pdch_params[gas_id].usf_granularity_allocation = FALSE;
         }

         /* Clean up the block params for the next block */
         ul_pdch_init_block_params(block_params);

         /* Reset state and remove handler */
         L1_STATE_CHANGE(ul_pdch_params[gas_id].state = UL_STATE_INACTIVE, "UL_STATE_INACTIVE");
         gl1_msgi_remove_ft_handler(gl1_msgi_ul_pacch_pdtch_hdlr,gas_id);
         break;

      default:
         /* Set a flag to abort at the end of the current block */
         ul_pdch_params[gas_id].abort = TRUE;
         break;
   }
}



/*===========================================================================

FUNCTION gl1_msg_set_ul_alloc_info

DESCRIPTION
  Sets the current USF allocation.  Called from gl1_msg_dl_cfg_pacch_pdtch().

===========================================================================*/
void gl1_msg_set_ul_alloc_info
(
   gl1_defs_rx_pkt_alloc_type alloc_info[GL1_DEFS_MAX_ASSIGNED_DL_TS],
   uint8   num_tn,
   boolean usf_granularity,
   gas_id_t gas_id
)
{
   uint8 i;

   /* Clear out previous USF allocation */
   for (i = 0; i < GL1_DEFS_SLOTS_IN_FRAME; i++)
   {
      ul_pdch_params[gas_id].usf_list[i] = GL1_DEFS_PDCH_UNALLOCATED;
   }

   /* Set new allocation */
   for (i = 0; i < num_tn; i++)
   {
      ul_pdch_params[gas_id].usf_list[alloc_info[i].tn] = alloc_info[i].usf;
   }

   /* Save USF granularity state */
   if (ul_pdch_params[gas_id].usf_granularity != usf_granularity)
   {
      MSG_GERAN_MED_2_G("USF granularity set to %d, FN=%d",
              usf_granularity ? 4 : 1, GSTMR_GET_FN_GERAN(gas_id));
      ul_pdch_params[gas_id].usf_granularity = usf_granularity;
   }
}



/*===========================================================================

FUNCTION gl1_msg_set_usf_alloc_info

DESCRIPTION
  Called from gl1_msgi_pbcch_pccch_pacch_pdtch_handler() to pass the
  decoded USFs for the current block.  Used to keep track of USF granularity
  allocations.

===========================================================================*/
void gl1_msg_set_usf_alloc_info
(
   gl1_defs_rx_pkt_alloc_type usfs[GL1_DEFS_MAX_ASSIGNED_DL_TS],
   uint32 num_usfs,
   gas_id_t gas_id
)
{
   uint8 i, tn;
   gl1_defs_usf_type *usf_list;

   /* This needs to be called at the right time in the state machine */
   if (ul_pdch_params[gas_id].state == UL_STATE_FRAME_3)
   {
      MSG_GERAN_ERROR_2_G("Getting USF alloc info in wrong state, FN=%d",
                GSTMR_GET_FN_GERAN(gas_id), 0 );
   }

   /* Figure out which usf list to use.  If a transmit is active we use
      the list latched at the start of the block, otherwise we need to use
      the master list.  This is to ensure that the right USFs are used for
      the active block while allowing PL1 to change the USF list at any
      time. */
   if (ul_pdch_params[gas_id].state == UL_STATE_INACTIVE ||
       ul_pdch_params[gas_id].state == UL_STATE_WAITING_FOR_BLOCK_START)
   {
      usf_list = ul_pdch_params[gas_id].usf_list;
   }
   else
   {
      usf_list = ul_pdch_params[gas_id].block_params[ul_pdch_params[gas_id].this_block_ptr].usf_list;
   }

   /* Find what slots we have been allocated */
   ul_pdch_params[gas_id].num_allocated_slots = 0;
   for (i = 0; i < num_usfs; i++)
   {
    if (usfs[i].usf != GL1_DEFS_PDCH_UNALLOCATED)
    {
      /* Check decoded usf against the assigned USF for the timeslot */
      if (usf_list[usfs[i].tn] == usfs[i].usf)
      {
         ul_pdch_params[gas_id].allocated_slots[ul_pdch_params[gas_id].num_allocated_slots++] =
           usfs[i].tn;

         /* If this is EDA then all higher allocated slots are ours too */
         if (gl1_msg_pdch_get_mac(gas_id) == GL1_DEFS_EXT_DYNAMIC_ALLOC)
         {
            /* Go through all higher slots and assign us any with a valid USF */
            for (tn = (uint8)usfs[i].tn + 1; tn <= (uint8)GL1_DEFS_TN_7; tn++)
            {
               if ((usf_list[tn] != GL1_DEFS_PDCH_UNALLOCATED) &&
                   (ul_pdch_params[gas_id].num_allocated_slots < GL1_DEFS_MAX_ASSIGNED_DL_TS))
               {
                  ul_pdch_params[gas_id].allocated_slots[ul_pdch_params[gas_id].num_allocated_slots++] =
                    (gl1_defs_tn_type)tn;
               }
            }

            /* No need to look at any more decoded USFs */
            break;
         }
      }
   }
  }

   /* Flag any USF granularity allocations as pending for the handler */
   if (ul_pdch_params[gas_id].usf_granularity && ul_pdch_params[gas_id].num_allocated_slots > 0)
   {
      ul_pdch_params[gas_id].usf_granularity_allocation_pending = TRUE;

      /* Check if we need to add the handler */
      if (ul_pdch_params[gas_id].state == UL_STATE_INACTIVE)
      {
         L1_STATE_CHANGE(ul_pdch_params[gas_id].state = UL_STATE_WAITING_FOR_BLOCK_START, "UL_STATE_WAITING_FOR_BLOCK_START");
         gl1_msgi_add_ft_handler(gl1_msgi_ul_pacch_pdtch_hdlr, GL1_MSG_FT_HDLR_UL_PDCH,gas_id);
      }
   }

}


/*=============================================================================

FUNCTION gl1_msg_cfg_ul_pacch_pdtch

DESCRIPTION
  Configures PACCH and PDTCH with the training sequence, BSIC and callback
  information.

============================================================================= */
void gl1_msg_cfg_ul_pacch_pdtch
(
   uint8 tsc,
   uint8 ab_tsc,
   uint8 bsic,
   const gl1_msg_tx_pkt_cb_type *cb_info,
   gas_id_t gas_id
)
{
   ul_pdch_params[gas_id].tsc = tsc;
   /* Added sync sequence bits for AB TSC. */
   ul_pdch_params[gas_id].ab_tsc = ab_tsc;
   ul_pdch_params[gas_id].bsic = bsic;
   ul_pdch_params[gas_id].callbacks = *cb_info;

   /* Ensure something is installed as a callback */
   if (ul_pdch_params[gas_id].callbacks.scheduled_callback == NULL)
   {
      ul_pdch_params[gas_id].callbacks.scheduled_callback = ul_pdch_default_scheduled_callback;
   }
   if (ul_pdch_params[gas_id].callbacks.discarded_callback == NULL)
   {
      ul_pdch_params[gas_id].callbacks.discarded_callback = ul_pdch_default_discarded_callback;
   }
   if (ul_pdch_params[gas_id].callbacks.usf_utilisation_callback == NULL)
   {
      ul_pdch_params[gas_id].callbacks.usf_utilisation_callback = ul_pdch_default_usf_utilisation_callback;
   }
}



/*=============================================================================

FUNCTION gl1_msg_tx_pacch_pdtch_fixed

DESCRIPTION
   This function is invoked by PL1 to request a PDCH specific transmission.

============================================================================= */
void gl1_msg_tx_pacch_pdtch_fixed
(
   gl1_defs_tx_pkt_data_type          *msg_data,
   gl1_msg_ul_pacch_pdtch_signal_type *signal_info,
   ARFCN_T                            arfcn[GL1_DEFS_FRAMES_IN_BLK_PERIOD],
   gl1_defs_coding_type               coding_scheme,
     gl1_defs_fixed_block_type          block_type,
   gas_id_t                           gas_id
)
{
   ul_pdch_block_params_type *block_params;
   ul_pdch_channel_type *ul_chan;
   int32 i;
   uint8 *msg;
   boolean write_ok;

   /* Pick up the data pointer to store for callbacks */
   msg = (IS_EGPRS_CODING(coding_scheme) ? msg_data->egprs.hdr : msg_data->gprs.data);

   /* Check the window state */
   if (!ul_pdch_params[gas_id].allow_data_writes)
   {
      MSG_GERAN_ERROR_1_G("Message submitted when window closed, FN=%d",GSTMR_GET_FN_GERAN(gas_id));
      ul_pdch_discard_message(msg,gas_id);
      return;
   }

   /* Get a pointer to the next block parameters to configure */
   block_params = &ul_pdch_params[gas_id].block_params[ul_pdch_params[gas_id].next_block_ptr];

   /* Check that we are not trying to schedule too many messages */
   if (block_params->num_msgs >= GL1_DEFS_MAX_ASSIGNED_UL_TS)
   {
      MSG_GERAN_ERROR_0_G("Too many messages scheduled.");
      ul_pdch_discard_message(msg,gas_id);
      return;
   }

   /* Check we are not in loopback */
   if (block_params->srb_loopback)
   {
      MSG_GERAN_ERROR_1_G("Loopback already scheduled this block, FN=%d",GSTMR_GET_FN_GERAN(gas_id));
      ul_pdch_discard_message(msg,gas_id);
      return;
   }

   /* Check if we need to add the handler */
   if (ul_pdch_params[gas_id].state == UL_STATE_INACTIVE)
   {
      L1_STATE_CHANGE(ul_pdch_params[gas_id].state = UL_STATE_WAITING_FOR_BLOCK_START, "UL_STATE_WAITING_FOR_BLOCK_START");
      gl1_msgi_add_ft_handler(gl1_msgi_ul_pacch_pdtch_hdlr, GL1_MSG_FT_HDLR_UL_PDCH,gas_id);
   }

   /* Get a pointer to the channel for this timeslot */
   ul_chan = &block_params->channels[signal_info->tn];

   /* Sanity check this channel was not already assigned */
   if (ul_chan->allocation == CHANNEL_FA)
   {
      MSG_GERAN_ERROR_2_G("Slot %d already allocated, FN=%d", signal_info->tn,GSTMR_GET_FN_GERAN(gas_id));
      ul_pdch_discard_message(msg,gas_id);
      return;
   }

   /* Write the message to the frame layer */
   write_ok = gl1_hw_write_tx_pkt_data
    (
      msg_data,
      cs_to_msg_length_lut[coding_scheme],
      signal_info->tn,
      GL1_HW_PDCH,
      cs_to_chan_type_lut[coding_scheme],
      &block_params->msgs[block_params->num_msgs].tags,
      gas_id
    ); /*lint !e661*/ /* no out-of-bounds, lint is confused */

   /* Check the write */
   if (!write_ok)
   {
      /* Make the discarded message callback */
      ul_pdch_discard_message(msg,gas_id);
      return;
   }

   /* Configure the channel */
   ul_chan->allocation = CHANNEL_FA;
   ul_chan->power_level = signal_info->TXLEV;
   ul_chan->modulation = MODULATION(coding_scheme);

     /* Configure channel type appropriate to the single block type */
     switch ( block_type )
     {
       case GL1_DEFS_FIXED_PACCH:
            ul_chan->channel_type = GL1_HW_RRBP_HI;
            block_params->msgs[block_params->num_msgs].priority = GPDU_HIGH_PRIORITY;   
            break;
       case GL1_DEFS_FIXED_RRBP_HI:
            ul_chan->channel_type = GL1_HW_RRBP_HI;
             block_params->msgs[block_params->num_msgs].priority = GPDU_HIGH_PRIORITY;   
            break;
       case GL1_DEFS_FIXED_RRBP_LO:
            ul_chan->channel_type = GL1_HW_RRBP_LO;
            block_params->msgs[block_params->num_msgs].priority = GPDU_LOW_PRIORITY;   
            break;

       default:
            break;
     }

   /* Save the block parameters */
   block_params->msgs[block_params->num_msgs].msg_data = msg;
   block_params->msgs[block_params->num_msgs].coding_scheme = coding_scheme;
   block_params->msgs[block_params->num_msgs].alloc_type = GL1_HW_PDCH;
   block_params->msgs[block_params->num_msgs].tn = signal_info->tn;
   block_params->msgs[block_params->num_msgs].discarded = FALSE;
   block_params->num_msgs++;
   for (i = 0; i < GL1_DEFS_FRAMES_IN_BLK_PERIOD; i++)
   {
      block_params->arfcn[i] = arfcn[i];
   }

   /* Cancel any pending abort */
   ul_pdch_params[gas_id].abort = FALSE;
}

#ifdef FEATURE_GPRS_PS_HANDOVER
/*=============================================================================

FUNCTION gl1_msg_tx_psho_pacch_pdtch_fixed

DESCRIPTION
   This function is invoked by PL1 to request a PDCH specific transmission.

============================================================================= */
void gl1_msg_tx_psho_pacch_pdtch_fixed
(
   gl1_defs_tx_pkt_data_type          *msg_data,
   gl1_msg_ul_pacch_pdtch_signal_type *signal_info,
   ARFCN_T                            arfcn[GL1_DEFS_FRAMES_IN_BLK_PERIOD],
   gl1_defs_coding_type               coding_scheme,
   gas_id_t                           gas_id
)
{
   ul_pdch_block_params_type *block_params;
   ul_pdch_channel_type *ul_chan;
   int32 i;
   uint8 *msg;
   boolean write_ok;

   /* Pick up the data pointer to store for callbacks */
   msg = (IS_EGPRS_CODING(coding_scheme) ? msg_data->egprs.hdr : msg_data->gprs.data);

   /* Check the window state */
   if (!ul_pdch_params[gas_id].allow_data_writes)
   {
      MSG_GERAN_ERROR_1_G("Message submitted when window closed, FN=%d",GSTMR_GET_FN_GERAN(gas_id));
      ul_pdch_discard_message(msg,gas_id);
      return;
   }

   /* Get a pointer to the next block parameters to configure */
   block_params = &ul_pdch_params[gas_id].block_params[ul_pdch_params[gas_id].next_block_ptr];

   /* Check that we are not trying to schedule too many messages */
   if (block_params->num_msgs >= GL1_DEFS_MAX_ASSIGNED_UL_TS)
   {
      MSG_GERAN_ERROR_0_G("Too many messages scheduled.");
      ul_pdch_discard_message(msg,gas_id);
      return;
   }

   /* Check we are not in loopback */
   if (block_params->srb_loopback)
   {
      MSG_GERAN_ERROR_1_G("Loopback already scheduled this block, FN=%d",GSTMR_GET_FN_GERAN(gas_id));
      ul_pdch_discard_message(msg,gas_id);
      return;
   }

   /* Check if we need to add the handler */
   if (ul_pdch_params[gas_id].state == UL_STATE_INACTIVE)
   {
      L1_STATE_CHANGE(ul_pdch_params[gas_id].state = UL_STATE_WAITING_FOR_BLOCK_START, "UL_STATE_WAITING_FOR_BLOCK_START");
      gl1_msgi_add_ft_handler(gl1_msgi_ul_pacch_pdtch_hdlr, GL1_MSG_FT_HDLR_UL_PDCH,gas_id);
   }

   /* Get a pointer to the channel for this timeslot */
   ul_chan = &block_params->channels[signal_info->tn];

   /* Sanity check this channel was not already assigned */
   if (ul_chan->allocation == CHANNEL_FA)
   {
      MSG_GERAN_ERROR_2_G("Slot %d already allocated, FN=%d", signal_info->tn,GSTMR_GET_FN_GERAN(gas_id));
      ul_pdch_discard_message(msg,gas_id);
      return;
   }

   /* Write the message to the frame layer */
   write_ok = gl1_hw_write_tx_pkt_data
    (
      msg_data,
      cs_to_msg_length_lut[coding_scheme],
      signal_info->tn,
      GL1_HW_USF,
      cs_to_chan_type_lut[coding_scheme],
      &block_params->msgs[block_params->num_msgs].tags,
      gas_id
    ); /*lint !e661*/ /* no out-of-bounds, lint is confused */

   /* Check the write */
   if (!write_ok)
   {
      /* Make the discarded message callback */
      ul_pdch_discard_message(msg,gas_id);
      return;
   }

   /* Configure the channel
    * @@OG: Changed this from CHANNEL_FA.
    */
   ul_chan->allocation = CHANNEL_DA;
   ul_chan->power_level = signal_info->TXLEV;
   ul_chan->modulation = MODULATION(coding_scheme);
     ul_chan->channel_type = cs_to_chan_type_lut[coding_scheme];

   /* Save the block parameters */
   block_params->msgs[block_params->num_msgs].msg_data = msg;
   block_params->msgs[block_params->num_msgs].coding_scheme = coding_scheme;

   /* @@OG: Changed this from GL1_HW_PDCH */
   block_params->msgs[block_params->num_msgs].alloc_type = GL1_HW_USF;
   block_params->msgs[block_params->num_msgs].tn = signal_info->tn;
   block_params->msgs[block_params->num_msgs].discarded = FALSE;
         block_params->msgs[block_params->num_msgs].priority  = GPDU_LOW_PRIORITY;
   block_params->num_msgs++;
   for (i = 0; i < GL1_DEFS_FRAMES_IN_BLK_PERIOD; i++)
   {
      block_params->arfcn[i] = arfcn[i];
   }

   /* Cancel any pending abort */
   ul_pdch_params[gas_id].abort = FALSE;
}
#endif /*FEATURE_GPRS_PS_HANDOVER */

/*=============================================================================
FUNCTION gl1_msg_tx_pacch_pdtch_dynamic

DESCRIPTION
   This function is invoked by PL1 to request a USF msg transmission.

============================================================================= */
void gl1_msg_tx_pacch_pdtch_dynamic
(
   gl1_defs_tx_pkt_data_type  *msg_data,
   gl1_defs_coding_type       coding_scheme,
         geran_pdu_priority_t       priority,
   gas_id_t                   gas_id
)
{
   ul_pdch_block_params_type *block_params;
   uint8 *msg;
   boolean write_ok;

   /* Pick up the data pointer to store for callbacks */
   msg = (IS_EGPRS_CODING(coding_scheme) ? msg_data->egprs.hdr : msg_data->gprs.data);

   /* Check the window state */
   if (!ul_pdch_params[gas_id].allow_data_writes)
   {
      MSG_GERAN_ERROR_1_G("Message submitted when window closed, FN=%d",GSTMR_GET_FN_GERAN(gas_id));
      ul_pdch_discard_message(msg,gas_id);
      return;
   }

   /* Get a pointer to the next block parameters to configure */
   block_params = &ul_pdch_params[gas_id].block_params[ul_pdch_params[gas_id].next_block_ptr];

   /* Check that we are not trying to schedule too many messages */
   if (block_params->num_msgs >= GL1_DEFS_MAX_ASSIGNED_UL_TS)
   {
      MSG_GERAN_ERROR_0_G("Too many messages scheduled.");
      ul_pdch_discard_message(msg,gas_id);
      return;
   }

   /* Check we are not in loopback */
   if (block_params->srb_loopback)
   {
      MSG_GERAN_ERROR_1_G("Loopback already scheduled this block, FN=%d",GSTMR_GET_FN_GERAN(gas_id));
      ul_pdch_discard_message(msg,gas_id);
      return;
   }

   /* Check if we need to add the handler */
   if (ul_pdch_params[gas_id].state == UL_STATE_INACTIVE)
   {
      L1_STATE_CHANGE(ul_pdch_params[gas_id].state = UL_STATE_WAITING_FOR_BLOCK_START, "UL_STATE_WAITING_FOR_BLOCK_START");
      gl1_msgi_add_ft_handler(gl1_msgi_ul_pacch_pdtch_hdlr, GL1_MSG_FT_HDLR_UL_PDCH,gas_id);
   }

   /* Write the message to the frame layer */
   write_ok = gl1_hw_write_tx_pkt_data
    (
      msg_data,
      cs_to_msg_length_lut[coding_scheme],
      GL1_DEFS_TN_0,
      GL1_HW_USF,
      cs_to_chan_type_lut[coding_scheme],
      &block_params->msgs[block_params->num_msgs].tags,
      gas_id
    ); /*lint !e661*/ /* no out-of-bounds, lint is confused */

   /* Block will be Tx'ed if USF match. */
   MSG_GERAN_HIGH_2_G("USF Block: FN=%d,tag=%d",
             GSTMR_GET_FN_GERAN(gas_id),
             block_params->msgs[block_params->num_msgs].tags.data_tag);


   /* Check for a discarded message */
   if (!write_ok)
   {
      /* Make the discarded message callback */
      ul_pdch_discard_message(msg,gas_id);
      return;
   }

   /* Save the block parameters */
   block_params->msgs[block_params->num_msgs].msg_data = msg;
   block_params->msgs[block_params->num_msgs].coding_scheme = coding_scheme;
   block_params->msgs[block_params->num_msgs].alloc_type = GL1_HW_USF;
   block_params->msgs[block_params->num_msgs].discarded = FALSE;
         block_params->msgs[block_params->num_msgs].priority = priority;
   block_params->num_msgs++;

   /* Cancel any pending abort */
   ul_pdch_params[gas_id].abort = FALSE;
}


/*=============================================================================
FUNCTION gl1_msg_tx_pacch_pdtch_loopback

DESCRIPTION
   This function is invoked by PL1 to request an SRB loopback transmission.

============================================================================= */
void gl1_msg_tx_pacch_pdtch_loopback
(
   gl1_msg_ul_pacch_pdtch_signal_type *signal_info,
   ARFCN_T                            arfcn[GL1_DEFS_FRAMES_IN_BLK_PERIOD],
   gl1_defs_tn_type                   tn_to_loopback,
   gas_id_t                           gas_id
)
{
   ul_pdch_block_params_type *block_params;
   ul_pdch_channel_type *ul_chan;
   int32 i;

   /* Check the window state */
   if (!ul_pdch_params[gas_id].allow_data_writes)
   {
      MSG_GERAN_ERROR_0_G("Loopback submitted when window closed");
      return;
   }

   /* Get a pointer to the next block parameters to configure */
   block_params = &ul_pdch_params[gas_id].block_params[ul_pdch_params[gas_id].next_block_ptr];

   /* Check that we are not trying to schedule too many messages */
   if (block_params->num_msgs >= GL1_DEFS_MAX_ASSIGNED_UL_TS)
   {
      MSG_GERAN_ERROR_0_G("Too many messages scheduled.");
      return;
   }

   /* Check that we are not trying to schedule regular data already */
   if (block_params->num_msgs > 0 && !block_params->srb_loopback)
   {
      MSG_GERAN_ERROR_1_G("Cannot combine loopback with regular data, FN=%d",GSTMR_GET_FN_GERAN(gas_id));
      return;
   }

   /* Check if we need to add the handler */
   if (ul_pdch_params[gas_id].state == UL_STATE_INACTIVE)
   {
      L1_STATE_CHANGE(ul_pdch_params[gas_id].state = UL_STATE_WAITING_FOR_BLOCK_START, "UL_STATE_WAITING_FOR_BLOCK_START");
      gl1_msgi_add_ft_handler(gl1_msgi_ul_pacch_pdtch_hdlr, GL1_MSG_FT_HDLR_UL_PDCH,gas_id);
   }

   /* Get a pointer to the channel for this timeslot */
   ul_chan = &block_params->channels[signal_info->tn];

   /* Sanity check this channel was not already assigned */
   if (ul_chan->allocation == CHANNEL_FA)
   {
      MSG_GERAN_ERROR_2_G("Slot %d already allocated, FN=%d", signal_info->tn,GSTMR_GET_FN_GERAN(gas_id));
      return;
   }

   /* Configure the channel */
   ul_chan->allocation = CHANNEL_FA;
   ul_chan->power_level = signal_info->TXLEV;
   /* Actual modulation will be set when we retrieve the loopback data,
      assume GMSK to avoid warnings about unknown modulation type*/
   ul_chan->modulation = GL1_DEFS_MODULATION_GMSK;

   /* The block must be all loopback */
   block_params->srb_loopback = TRUE;
   block_params->loopback_tn[signal_info->tn] = tn_to_loopback;

   /* Save the block parameters - not ever used except num_msgs */
   block_params->msgs[block_params->num_msgs].msg_data = NULL;
   block_params->msgs[block_params->num_msgs].coding_scheme = GL1_DEFS_CS1_CODING;
   block_params->msgs[block_params->num_msgs].alloc_type = GL1_HW_LOOPBACK;
   block_params->msgs[block_params->num_msgs].tn = signal_info->tn;
   block_params->msgs[block_params->num_msgs].discarded = FALSE;
         block_params->msgs[block_params->num_msgs].priority = GPDU_LOW_PRIORITY;
   block_params->num_msgs++;
   for (i = 0; i < GL1_DEFS_FRAMES_IN_BLK_PERIOD; i++)
   {
      block_params->arfcn[i] = arfcn[i];
   }

   /* Cancel any pending abort */
   ul_pdch_params[gas_id].abort = FALSE;
}


/*=============================================================================
FUNCTION gl1_msg_set_pacch_pdtch_dynamic_signal_info

DESCRIPTION
   This function is to be invoked following the submission of USF specific
   messages.
   In the case of dynamic or extended dynamic allocation, the submitted messages
   are not tied to any particular timeslot. Hence the power and frequency for
   all the assigned timeslots must be submitted every block until all the
   submitted USF messages have been transmitted. This function allows the
   updating of the frequency and power information for the next four frames.

   The frequencies and powers take effect two frames following the frame in
   which this function was called.

============================================================================= */
void gl1_msg_set_pacch_pdtch_dynamic_signal_info
(
   gl1_msg_ul_pacch_pdtch_signal_type signal_info[GL1_DEFS_MAX_ASSIGNED_UL_TS],
   uint8                              num_tn,
   ARFCN_T                            arfcn[GL1_DEFS_FRAMES_IN_BLK_PERIOD],
   gas_id_t                           gas_id
)
{
   ul_pdch_block_params_type *block_params;
   ul_pdch_channel_type *ul_chan;
   int32 i;

   /* Get a pointer to the next block parameters */
   block_params = &ul_pdch_params[gas_id].block_params[ul_pdch_params[gas_id].next_block_ptr];

   /* Copy the signal information over for each channel */
   for (i = 0; i < num_tn; i++)
   {
      /* Get the channel pointer */
      ul_chan = &block_params->channels[signal_info[i].tn];

      /* If there is already an fa channel on this slot, skip the da */
      if (ul_chan->allocation != CHANNEL_FA)
      {
      /* Configure the channel */
      ul_chan->allocation = CHANNEL_DA;
      ul_chan->power_level = signal_info[i].TXLEV;
      ul_chan->modulation = GL1_DEFS_MODULATION_UNKNOWN;
            ul_chan->channel_type = cs_to_chan_type_lut[ul_pdch_params[gas_id].block_params[ul_pdch_params[gas_id].next_block_ptr].msgs[i].coding_scheme];
      }
   }

   /* Save the ARFCNs */
   for (i = 0; i < GL1_DEFS_FRAMES_IN_BLK_PERIOD; i++)
   {
      block_params->arfcn[i] = arfcn[i];
   }

}


/*=============================================================================

FUNCTION  ul_pdch_calc_power_index

DESCRIPTION
  Calculates the power index to use on the given timeslot.  May be different
  from the configured power index if the channel is 8PSK since we are a
  class E2 mobile and thus cap 8PSK power to 26 dBm (DCS/PCS) or 27 dBm (GSM).

  These values are taken from 05.05, 4.1.1

============================================================================= */
static void ul_pdch_calc_power_index (uint8 tn,gas_id_t gas_id)
{
   ul_pdch_block_params_type *block_params;
   int16 power_level;
#ifdef POWER_CLASS_E2
   sys_band_T band;
   boolean capped = FALSE;
#endif

   /* Get the current block parameters */
   block_params = &ul_pdch_params[gas_id].block_params[ul_pdch_params[gas_id].this_block_ptr];
   power_level = block_params->channels[tn].power_level;

#ifdef POWER_CLASS_E2
   /* If the modulation is 8PSK or unknown we cap */
   if (block_params->channels[tn].modulation != GL1_DEFS_MODULATION_GMSK)
   {
      band = block_params->arfcn[0].band;
      switch (band)
      {
         case SYS_BAND_EGSM_900:
         case SYS_BAND_PGSM_900:
         case SYS_BAND_CELL_850:
            if (power_level < 8)
            {
               power_level = 8;
               capped = TRUE;
            }
            break;

         case SYS_BAND_PCS_1900:
         case SYS_BAND_DCS_1800:
            if (power_level < 2 || power_level > 28)
            {
               power_level = 2;
               capped = TRUE;
            }
            break;

         default:
            MSG_GERAN_ERROR_1_G("Unknown tx band %d", band);
            break;
      }

      /* Print a warning if we capped without being sure */
      if (capped && block_params->channels[tn].modulation == GL1_DEFS_MODULATION_UNKNOWN)
      {
         MSG_GERAN_HIGH_3_G("Power capped (%d->%d) for unknown modulation, tn=%d",
                  block_params->channels[tn].power_level,power_level,tn);
      }
   }
#endif

   /* Map from power level to power index */
   gl1_msgi_map_txlev_to_pwr_idx
    (
      block_params->arfcn[0],
      &power_level,
      &block_params->channels[tn].power_index,
      1,
      gas_id
    );
}



/*=============================================================================

FUNCTION  ul_pdch_latch_block_info

DESCRIPTION
  Latches certain parameters into the current block from the master lists.
  Called at block start.

============================================================================= */
static void ul_pdch_latch_block_info (gas_id_t gas_id)
{
   ul_pdch_block_params_type *block_params;
   uint8 i;

   /* Get the current block parameters */
   block_params = &ul_pdch_params[gas_id].block_params[ul_pdch_params[gas_id].this_block_ptr];

   /* Latch the USF list - this is in case PL1 changes the USF list before
      we get the list of decoded USFs from gl1_msg_set_usf_alloc_info */
   for (i = 0; i < GL1_DEFS_SLOTS_IN_FRAME; i++)
   {
      block_params->usf_list[i] = ul_pdch_params[gas_id].usf_list[i];
   }
}



/*=============================================================================

FUNCTION  ul_pdch_prepare_block_channel_info

DESCRIPTION
  Fills in the signal and burst info for the current block.  Called on the
  block start.

============================================================================= */
static void ul_pdch_prepare_block_channel_info (gas_id_t gas_id)
{
   ul_pdch_block_params_type *block_params;
   uint8 tn, i;
   uint8 da_mod_known;
   boolean single_da_mod;
   gl1_defs_modulation_type da_mod, mod;

   /* Get the current block parameters */
   block_params = &ul_pdch_params[gas_id].block_params[ul_pdch_params[gas_id].this_block_ptr];

   /* If there is no data to transmit, no need to do anything */
   if (block_params->num_msgs == 0) return;

   /* If this is a dynamic allocation and there are USF granularity blocks
      for this slot, then convert to fixed allocation */
   for (tn = 0; tn < GL1_DEFS_SLOTS_IN_FRAME; tn++)
   {
      if (block_params->channels[tn].allocation == CHANNEL_DA &&
          ul_pdch_params[gas_id].usf_granularity_blocks[tn] > 0)
      {
         /* Find a dynamic allocation message to convert */
         for (i = 0; i < block_params->num_msgs; i++)
         {
            if (block_params->msgs[i].alloc_type == GL1_HW_USF)
            {
               /* Convert from USF to PDCH based transmission */
               gl1_hw_convert_usf_to_pdch
                (
                  &block_params->msgs[i].tags,
                  (gl1_defs_tn_type)tn,
                  gas_id
                );

               /* Update our internal state */
               block_params->msgs[i].alloc_type = GL1_HW_PDCH;
               block_params->msgs[i].tn = (gl1_defs_tn_type)tn;
               block_params->channels[tn].allocation = CHANNEL_FA;
               block_params->channels[tn].modulation =
                 MODULATION(block_params->msgs[i].coding_scheme);
                             block_params->channels[tn].channel_type =
                                 cs_to_chan_type_lut[block_params->msgs[i].coding_scheme];

               break;
            }
         }
      }
   }

   /* Try and determine modulation for each dynamic allocation message
      being transmitted.  The logic works like this: if the first N DA
      messages have the same modulation, then we know that the first N DA
      channels will have to use this same modulation.  We cannot be sure of
      the modulation on the rest of the DA channels since it depends on where
      we get our USF allocations.
      This must be done after converting USF granularity messages to FA. */
   da_mod_known = 0;
   single_da_mod = TRUE;
   da_mod = GL1_DEFS_MODULATION_UNKNOWN;
   for (i = 0; i < block_params->num_msgs; i++)
   {
     if (block_params->msgs[i].alloc_type == GL1_HW_USF)
     {
        mod = MODULATION(block_params->msgs[i].coding_scheme);
        if (da_mod_known == 0)
        {
           da_mod = mod;
           da_mod_known = 1;
        }
        else if (da_mod == mod)
        {
           da_mod_known++;
        }
        else
        {
           single_da_mod = FALSE;
           break;
        }
     }
   }

   /* Fill in the channel information */
   block_params->num_channels = 0;
   for (tn = 0; tn < GL1_DEFS_SLOTS_IN_FRAME; tn++)
   {
     if (block_params->channels[tn].allocation != CHANNEL_FREE)
     {
      /* If this is dynamic alloc, check if we can be sure of the modulation */
      if (block_params->channels[tn].allocation == CHANNEL_DA &&
          (single_da_mod || da_mod_known > 0))
      {
         block_params->channels[tn].modulation = da_mod;
         if (da_mod_known > 0) da_mod_known--;
      }

      /* Calculate the power index */
      ul_pdch_calc_power_index(tn,gas_id);

      block_params->signal_info[block_params->num_channels].tn = (gl1_defs_tn_type)tn;
      block_params->signal_info[block_params->num_channels].tx_power_index =
        block_params->channels[tn].power_index;
      block_params->signal_info[block_params->num_channels].modulation =
        block_params->channels[tn].modulation;
                        block_params->signal_info[block_params->num_channels].channel_type =
                        block_params->channels[tn].channel_type;
                        block_params->signal_info[block_params->num_channels].priority =
                        block_params->msgs[block_params->num_channels].priority;

         block_params->num_channels++;
          /* Check that there are not too many */
         if (block_params->num_channels >= ARR_SIZE(block_params->signal_info))
         {
            //MSG_GERAN_ERROR_0_G("Too many channels");
            break;
         }
      }
   }

    /* Set the number of uplink timeslots including any gaps
         * between assigned timeslots.
         */
   if( 0 != block_params->num_channels )
   {
      block_params->num_ul_timeslots =
                          ((int32)block_params->signal_info[(block_params->num_channels-1)].tn
                            - (int32)block_params->signal_info[0].tn) + 1;
   }
   else
   {
       block_params->num_ul_timeslots = 0;
   }

    /* Sanity check that channels are available */
   if (block_params->num_channels == 0)
   {
      MSG_GERAN_ERROR_0_G("Message data submitted but no channels available");
   }

    /* Prepare the burst info structure */
    block_params->burst_info.tsc = ul_pdch_params[gas_id].tsc;
    /* Added AB TSC sync seq bits for NB AB change. */
    block_params->burst_info.ab_tsc = ul_pdch_params[gas_id].ab_tsc;
    block_params->burst_info.bsic = ul_pdch_params[gas_id].bsic;
    block_params->burst_info.timing_advance = gl1_msg_pdch_retrieve_ta(gas_id);

    /* Latch the camp tn */
    block_params->camp_tn = gl1_msg_pdch_get_camp_tn(gas_id);

    /* Set the number of assigned uplink slots now - technically this should
      come from PL1 but for now we assume that we attempt a tx on all
      assigned slots every block */
    gl1_hw_set_num_assigned_tx_slots(block_params->num_channels,gas_id);

    /* Need to generate the tags list now */
    block_params->generate_tags = TRUE;
}


/*=============================================================================

FUNCTION  ul_pdch_dec_usf_granularity_blocks

DESCRIPTION
  Reduces any USF granularity allocations and determines if there are
  outstanding allocations.

============================================================================= */
static void ul_pdch_dec_usf_granularity_blocks (gas_id_t gas_id)
{
   uint8 i;

   ul_pdch_params[gas_id].usf_granularity_allocation = FALSE;

   for (i = 0; i < GL1_DEFS_SLOTS_IN_FRAME; i++)
   {
      if (ul_pdch_params[gas_id].usf_granularity_blocks[i] > 0)
         ul_pdch_params[gas_id].usf_granularity_blocks[i]--;

      if (ul_pdch_params[gas_id].usf_granularity_blocks[i] > 0)
         ul_pdch_params[gas_id].usf_granularity_allocation = TRUE;
   }
}



/*=============================================================================
FUNCTION ul_pdch_log_tx_scheduled_results_pacch_pdtch

DESCRIPTION
  This function logs the tx schedule.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
============================================================================= */
static void ul_pdch_log_tx_scheduled_results_pacch_pdtch
(
  uint8 msg_tx_index[GL1_DEFS_MAX_UL_MSGS],
  uint8 num_msg_tx,
  gas_id_t gas_id
)
{
   LOG_GPRS_TX_SCHEDULED_RESULTS_C_type *log_pkt;
   ul_pdch_block_params_type *block_params;
   uint8 i, j;
   gl1_defs_tn_type tn;
   uint32 size;
   uint32 current_FN = GSTMR_GET_FN_GERAN(gas_id);

   /* Calculate how much space we need */
   size = sizeof(LOG_GPRS_TX_SCHEDULED_RESULTS_C_type)
           - (GL1_DEFS_MAX_ASSIGNED_UL_TS - num_msg_tx)
              * sizeof(log_gprs_tx_scheduled_result_type);

   /* Try to allocate memory for the log packet */
   log_pkt = (LOG_GPRS_TX_SCHEDULED_RESULTS_C_type *)
              log_alloc(LOG_GPRS_TX_SCHEDULED_RESULTS_C, size);
   if (log_pkt == NULL) return;

   /* Get the current block parameters */
   block_params = &ul_pdch_params[gas_id].block_params[ul_pdch_params[gas_id].this_block_ptr];

   /* Fill in the log info */
   log_pkt->num_msg_tx = num_msg_tx;
   log_pkt->FN = SUB_FN(current_FN,1);
   for (i = 0; i < GL1_DEFS_FRAMES_IN_BLK_PERIOD; i++)
   {
      log_pkt->arfcn[i] = arfcn_to_word(block_params->arfcn[i]);
   }

   /* Log the per-msg info */
   for (i = 0; i < num_msg_tx; i++)
   {
      /* Shortcut the message index */
      j = msg_tx_index[i];

      /* Fill in the log packet */
      log_pkt->msg_info[i].coding_scheme = block_params->msgs[j].coding_scheme;
      log_pkt->msg_info[i].msg_type = GL1_MSG_UL_PACCH_PDTCH;
      log_pkt->msg_info[i].dynamic_alloc =
        (block_params->msgs[j].alloc_type == GL1_HW_USF ? TRUE : FALSE);
      log_pkt->msg_info[i].timing_advance = block_params->burst_info.timing_advance;

      /* Determine the timeslot and power */
      tn = block_params->msgs[j].tn;
      log_pkt->msg_info[i].tn = gpl1_gprs_ts_conv_unslam((uint8)tn, gas_id);
      log_pkt->msg_info[i].power_index = block_params->channels[tn].power_index;
   }

   /* Send the log packet */
   log_commit((void *)log_pkt);
}



/*=============================================================================

FUNCTION  ul_pdch_get_block_scheduled_results

DESCRIPTION
  Retrieves the tx scheduled results from the firmware and issues the
  appropriate callbacks.

============================================================================= */
static void ul_pdch_get_block_scheduled_results (gas_id_t gas_id)
{
   gl1_msg_tx_scheduled_data_type msg_tx[ GL1_DEFS_MAX_UL_MSGS ];
   gl1_msg_tx_discarded_data_type msg_discarded[GL1_DEFS_MAX_ASSIGNED_UL_TS];
   uint8 num_msg_tx, num_msg_discarded;
   ul_pdch_block_params_type *block_params;
   uint8 i, alloc_slots_index, msg_tx_index[GL1_DEFS_MAX_UL_MSGS];
   uint16 tn;

   /* Get the current block parameters */
   block_params = &ul_pdch_params[gas_id].block_params[ul_pdch_params[gas_id].this_block_ptr];

   /* Check if we have any outstanding messages */
   if (block_params->num_msgs == 0) return;

   /* Skip this in loopback mode */
   if (block_params->srb_loopback) return;

   /* Retrieve the schedule results for all messages this block, discarding
      any untransmitted messages */
   num_msg_tx = 0;
   num_msg_discarded = 0;
   alloc_slots_index = 0;

   ul_pdch_params[gas_id].scheduled_blocks_mask = 0;

   for (i = 0; i < block_params->num_msgs; i++)
   {
      if (gl1_hw_get_tx_scheduled_result(&block_params->msgs[i].tags, TRUE,gas_id))
      {
         /* Try and figure out which timeslot this must have gone out on */
         /* TODO: firmware should supply us this information directly since
            this method assumes everthing is working as expected and no one
            has reconfigured the USF list since the start of the block. */
         if (block_params->msgs[i].alloc_type == GL1_HW_USF)
         {
            while (TRUE)  /*lint !e716*/
            {
               /* Make sure we are in sync */
               if (alloc_slots_index == ul_pdch_params[gas_id].num_allocated_slots)
               {
                  MSG_GERAN_ERROR_3_G("Unknown slot for txed msg %d: num_alloc=%d, FN=%d",
                            i, ul_pdch_params[gas_id].num_allocated_slots, GSTMR_GET_FN_GERAN(gas_id));
                  break;
               }

               /* Check if we had a dynamic alloc channel on this slot */
               tn = (int32)ul_pdch_params[gas_id].allocated_slots[alloc_slots_index++];
               if (block_params->channels[tn].allocation == CHANNEL_DA)
               {
                  block_params->msgs[i].tn = (gl1_defs_tn_type) tn;

                  /* We know the modulation on this channel now */
                  if (block_params->channels[tn].modulation == GL1_DEFS_MODULATION_UNKNOWN)
                  {
                     block_params->channels[tn].modulation =
                       MODULATION(block_params->msgs[i].coding_scheme);
                     ul_pdch_calc_power_index(tn,gas_id);
                  }
                  break;
               }
            }
         }

         /* Mark this channel as being scheduled */
         block_params->channels[block_params->msgs[i].tn].scheduled = TRUE;

         /* Mark block as scheduled for usf utilisation calculation */
         ul_pdch_params[gas_id].scheduled_blocks_mask |= 1 << i;

         /* Save index for logging */
         msg_tx_index[num_msg_tx] = i;

         /* Fill in the callback data */
         msg_tx[num_msg_tx].msg_data = block_params->msgs[i].msg_data;
         msg_tx[num_msg_tx].tn = block_params->msgs[i].tn;
         msg_tx[num_msg_tx].camp_tn = block_params->camp_tn;
         msg_tx[num_msg_tx].offset_to_sob = UL_PDCH_OFFSET_TO_SOB;
         num_msg_tx++;
      }
      else
      {
         /* Fill in the callback data */
         msg_discarded[num_msg_discarded].msg_data = block_params->msgs[i].msg_data;
         msg_discarded[num_msg_discarded].offset_to_sob = UL_PDCH_OFFSET_TO_SOB;
         num_msg_discarded++;

         block_params->msgs[i].discarded = TRUE;
      }
   }

   MSG_GERAN_HIGH_1_G("USF DEBUG Scheduled blocks (MASK) %d", ul_pdch_params[gas_id].scheduled_blocks_mask);

   /* If the number of messages being transmitted is less than the number
      of current channels we can regenerate the list and remove channels
      that are not in use - this is not required by firmware but reduces
      some processing in later blocks.  Note num_msg_tx really cannot be
      greater than num_channels at this point. */
   if (num_msg_tx != block_params->num_channels)
   {
      MSG_GERAN_MED_2_G("Not all Tx channels transmitted, Tx sched = %d, Tx transmitted = %d", block_params->num_channels, num_msg_tx);
   }

   /* Trace the callbacks execution time */
   GSTMR_TRACE(0);

   /* Make the scheduled message callback */
   ul_pdch_params[gas_id].callbacks.scheduled_callback(msg_tx, num_msg_tx,gas_id);


   /* Log the results */
   if (num_msg_tx > 0)
   {
      ul_pdch_log_tx_scheduled_results_pacch_pdtch(msg_tx_index, num_msg_tx,gas_id);
   }

   GSTMR_TRACE(1);

   /* Make the discarded message callback */
   if (num_msg_discarded > 0)
   {
      ul_pdch_params[gas_id].callbacks.discarded_callback(msg_discarded, num_msg_discarded,gas_id);
   }

   GSTMR_TRACE(2);
}

/*=============================================================================

FUNCTION  ul_pdch_calculate_usf_utilisation

DESCRIPTION
  Determine which blocks were blanked based on the burst blanked. Also keep track
  of the number of blocks that were blanked consecutively
============================================================================= */
static void ul_pdch_calculate_usf_utilisation (gas_id_t gas_id)
{
   uint8 num_blanked_frames = 0;
   uint8 i, j;
   uint16 mask;
   gl1_msg_usf_utilisation_data_type msg_usf_utilisation;

   ul_pdch_params[gas_id].num_scheduled_blocks = 0;
   ul_pdch_params[gas_id].num_blanked_blocks = 0;

   for (i = 0; i < GL1_DEFS_SLOTS_IN_FRAME; i++)
   {
      /* Determine if a block was scheduled */
      mask = (1 << i);
      if (ul_pdch_params[gas_id].scheduled_blocks_mask & mask)
      {
         ul_pdch_params[gas_id].num_scheduled_blocks++;
         num_blanked_frames = 0;
         /* Check if any burst of the block were blanked */
         for (j = 0; j < FRAMES_PER_BLOCK; j++)
         {
            if (ul_pdch_params[gas_id].blanked_bursts_mask[j] & mask) num_blanked_frames++;
         }
         /* If two or more burst we blanked declare the burst blanked */
         if (num_blanked_frames >= 2) ul_pdch_params[gas_id].num_blanked_blocks++;
      }
   }

   /* Keep track of the number of consecutive blanked blocks */
   if (ul_pdch_params[gas_id].num_scheduled_blocks > 0)
   {
      if (ul_pdch_params[gas_id].num_scheduled_blocks == ul_pdch_params[gas_id].num_blanked_blocks)
      {
         ul_pdch_params[gas_id].num_consecutive_blanked_blocks += ul_pdch_params[gas_id].num_scheduled_blocks;
      }
      else
      {
         ul_pdch_params[gas_id].num_consecutive_blanked_blocks = 0;
      }
   }

   /* Clear the blanked burst masks */
   for (i = 0; i < FRAMES_PER_BLOCK; i++)
   {           
      ul_pdch_params[gas_id].blanked_bursts_mask[i] = 0;
   }

   /* Populate usf utilisation information for callback */
   msg_usf_utilisation.num_scheduled_blocks           = ul_pdch_params[gas_id].num_scheduled_blocks;          
   msg_usf_utilisation.num_blanked_blocks             = ul_pdch_params[gas_id].num_blanked_blocks;            
   msg_usf_utilisation.num_consecutive_blanked_blocks = ul_pdch_params[gas_id].num_consecutive_blanked_blocks;

   /* Call USF utilisation callback */
   ul_pdch_params[gas_id].callbacks.usf_utilisation_callback(msg_usf_utilisation, gas_id);

   MSG_GERAN_HIGH_3_G("USF DEBUG, usf utilisation, num_scheduled_blocks: %d, num_blanked_blocks: %d, num_consecutive_blanked_blocks: %d", 
      ul_pdch_params[gas_id].num_scheduled_blocks, 
      ul_pdch_params[gas_id].num_blanked_blocks,
      ul_pdch_params[gas_id].num_consecutive_blanked_blocks);
}


#ifdef FEATURE_GSM_MDSP_EGPRS
/*=============================================================================

FUNCTION  ul_pdch_write_loopback_data

DESCRIPTION
  Called in SRB loopback mode to submit data each frame to the firmware for
  transmission.

============================================================================= */
static void ul_pdch_write_loopback_data
(
   ul_pdch_block_params_type *block_params,
   uint8 frame_index,
   gas_id_t gas_id
)
{
   uint8 len, i;
   gl1_defs_tn_type tn, dl_tn;
   gl1_defs_tx_pkt_data_type msg;
   gl1_hw_channel_type chan_type;

   /* @@OG Ensure the hdr pointer is not NULL or has a rogue value */
   static uint8  egprs_ul_hdr[NUM_GERAN_DATA_SPACES][GL1_EGPRS_UL_HDR_OCTET_SIZE] =
                   { INITIAL_VALUE_S( 0 )};

   /* Assign the pointer to the first octet */
   msg.egprs.hdr = &( egprs_ul_hdr[gas_id][0] );

   /* Go through all channels and write loopback data to the hw layer */
   for (i = 0; i < block_params->num_channels; i++)
   {
      /* Get the timeslot for uplink and downlink */
      tn = block_params->signal_info[i].tn;
      dl_tn = block_params->loopback_tn[tn];

      /* Grab data for this slot and frame */
      len = gl1_msg_pdch_get_loopback_data(frame_index, dl_tn, &msg.egprs.data[0],gas_id);
      if (len == MDSP_HARD_DECISIONS_GMSK)
      {
         chan_type = GL1_HW_PDTCHU_MCS1;
         block_params->channels[tn].modulation = GL1_DEFS_MODULATION_GMSK;
      }
      else if (len == MDSP_HARD_DECISIONS_8PSK)
      {
         chan_type = GL1_HW_PDTCHU_MCS5;
         block_params->channels[tn].modulation = GL1_DEFS_MODULATION_8PSK;
      }
      else if (len == 0)
      {
         MSG_GERAN_ERROR_1_G("No loopback data found for slot %d", tn);
      }
      else
      {
         MSG_GERAN_ERROR_1_G("Bad loopback data length %d", len);
         len = 0;  /* Use len as an error flag */
      }

     if (len != 0)
     {
      /* Convert length to bytes */
      len *= 2;

      /* Write the data to the mdsp, we will overwrite the previous frame's
         tag, but we do not need it anymore. */
      (void) gl1_hw_write_tx_pkt_data
       (
         &msg,
         len,
         tn,
         GL1_HW_LOOPBACK,
         chan_type,
         &block_params->msgs[i].tags, /* num_channels == num_msgs */
         gas_id
       );

      /* Calculate the power index now that we know the modulation */
      ul_pdch_calc_power_index((uint8)tn,gas_id);
      block_params->signal_info[i].tx_power_index =
        block_params->channels[tn].power_index;
      block_params->signal_info[i].modulation =
        block_params->channels[tn].modulation;
     }
   }

   /* Generate the tags now that they are ready */
   block_params->generate_tags = TRUE;
}
#endif


/*=============================================================================
FUNCTION  gl1_msgi_ul_pacch_pdtch_hdlr

DESCRIPTION
   When this handler is registered, it is invoked every frame until the handler
   decides to deregister itself.

   When this handler is first invoked the following happens:
   The handler does nothing until the frame before the start of the 4frame block
   boundary. In that frame, the frame layer is commanded to encode the submitted
   data and start sending bursts. Two frames later a check is made to determine
   if a USF was decoded and the data was transmitted.
   If the data transmission is successful (USF decoded in the previous block)
   then the transmission notification callback is invoked.

   The handler is deregistered when there are no pending messages to be
   transmitted.

============================================================================= */
#define TX_RESULT_INDEX_BURST_0     0
#define TX_RESULT_INDEX_BURST_1     1
#define TX_RESULT_INDEX_BURST_2     2
#define TX_RESULT_INDEX_BURST_3     3
static boolean gl1_msgi_ul_pacch_pdtch_hdlr
(
   boolean start_of_block,
   uint32   time_stamp,
   gas_id_t gas_id
)
{
   boolean remove_handler = FALSE;
   boolean encode_flag    = FALSE;
   boolean end_of_block   = FALSE;
   uint8   frame_index = 0;
   uint8   i;
   ul_pdch_block_params_type *block_params;


   /* Check the state */
   switch (ul_pdch_params[gas_id].state)
   {
      case UL_STATE_INACTIVE:
         MSG_GERAN_ERROR_0_G("UL PDCH inactive");
         return TRUE;

      case UL_STATE_WAITING_FOR_BLOCK_START:
         /* If not a block start then get out of here */
         if (!start_of_block) break;

         /* Switch current and next blocks */
         ul_pdch_params[gas_id].this_block_ptr = ul_pdch_params[gas_id].next_block_ptr;
         ul_pdch_params[gas_id].next_block_ptr = !ul_pdch_params[gas_id].next_block_ptr;

         /* Clean up the block params for the next block */
         ul_pdch_init_block_params
           (&ul_pdch_params[gas_id].block_params[ul_pdch_params[gas_id].next_block_ptr]);

         /* Latch the master USF list into this block */
         ul_pdch_latch_block_info(gas_id);

         L1_STATE_CHANGE(ul_pdch_params[gas_id].state = UL_STATE_FRAME_0, "UL_STATE_FRAME_0");
         /* fall through */

      case UL_STATE_FRAME_0:
         /* Prepare the channel information for this block */
         ul_pdch_prepare_block_channel_info(gas_id);

         /* Reduce any USF granularity allocations */
         ul_pdch_dec_usf_granularity_blocks(gas_id);

         ul_pdch_params[gas_id].blanked_bursts_mask[TX_RESULT_INDEX_BURST_2] = 
            mdsp_gprs_get_tx_burst_schedule ( gas_id );

         encode_flag = TRUE;
         frame_index = 0;
         ul_pdch_params[gas_id].allow_data_writes = FALSE;
         ul_pdch_params[gas_id].state++;
         break;

      case UL_STATE_FRAME_1:
         ul_pdch_params[gas_id].blanked_bursts_mask[TX_RESULT_INDEX_BURST_3] =
            mdsp_gprs_get_tx_burst_schedule ( gas_id );

         frame_index = 1;
         ul_pdch_params[gas_id].state++;
         break;

      case UL_STATE_FRAME_2:
         /* Calculate USF utilisation and call usf utilisation callback */
         ul_pdch_calculate_usf_utilisation(gas_id);                                       

         /* Get scheduled results for this block */
         ul_pdch_get_block_scheduled_results(gas_id);

         ul_pdch_params[gas_id].blanked_bursts_mask[TX_RESULT_INDEX_BURST_0] = 
             mdsp_gprs_get_tx_burst_schedule ( gas_id );

         frame_index = 2;
         ul_pdch_params[gas_id].allow_data_writes = TRUE;
         ul_pdch_params[gas_id].state++;
         break;

      case UL_STATE_FRAME_3:
         ul_pdch_params[gas_id].blanked_bursts_mask[TX_RESULT_INDEX_BURST_1] = 
            mdsp_gprs_get_tx_burst_schedule ( gas_id );

         frame_index = 3;
         end_of_block = TRUE;
         L1_STATE_CHANGE(ul_pdch_params[gas_id].state = UL_STATE_WAITING_FOR_BLOCK_START, "UL_STATE_WAITING_FOR_BLOCK_START");
         break;

      default:
         MSG_GERAN_ERROR_1_G("Unexpected state: %d", ul_pdch_params[gas_id].state);
         L1_STATE_CHANGE(ul_pdch_params[gas_id].state = UL_STATE_INACTIVE, "UL_STATE_INACTIVE");
         return TRUE;
   }

   /* If any USF granularity allocations are pending, apply them now.  This
      ensures that if the allocation occurs in the block start frame we do
      not incorrectly use the first block when decrementing the count.  This
      happens if the received block falls before the idle or ptcch frame and
      frame tick reslamming is enabled. */
   if (ul_pdch_params[gas_id].usf_granularity_allocation_pending)
   {
      for (i = 0; i < ul_pdch_params[gas_id].num_allocated_slots; i++)
      {
         ul_pdch_params[gas_id].usf_granularity_blocks[ul_pdch_params[gas_id].allocated_slots[i]] =
           INIT_USF_GRAN_BLKS;
      }
      ul_pdch_params[gas_id].usf_granularity_allocation = TRUE;
      ul_pdch_params[gas_id].usf_granularity_allocation_pending = FALSE;
   }

   /* If we are still waiting for block start, we are done.  Do this here to
      ensure that the above USF granularity code always runs. */
   if (ul_pdch_params[gas_id].state == UL_STATE_WAITING_FOR_BLOCK_START && !end_of_block)
   {
      return FALSE;
   }

   /* Get the current block parameters */
   block_params = &ul_pdch_params[gas_id].block_params[ul_pdch_params[gas_id].this_block_ptr];

#if  defined(FEATURE_GSM_MDSP_EGPRS)
   /* If we are a loopback block, write the data */
   if (block_params->srb_loopback)
   {
      ul_pdch_write_loopback_data(block_params, frame_index,gas_id);
   }
#endif

   /* Generate list of tags if required */
   if (block_params->generate_tags)
   {
      block_params->generate_tags = FALSE;
      block_params->num_tags = 0;
      for (i = 0; i < block_params->num_msgs; i++)
      {
         if (!block_params->msgs[i].discarded)
         {
            block_params->tags[block_params->num_tags++] = block_params->msgs[i].tags;
         }
      }
   }

   /* Check if we have any messages to transmit */
   if (block_params->num_tags > 0 && block_params->num_channels > 0)
   {
      /* Add the ARFCN to the burst info structure */
      block_params->burst_info.arfcn = block_params->arfcn[frame_index];
      block_params->burst_info.encode_flag = encode_flag;
      block_params->burst_info.burst_num = frame_index;

#if  defined(FEATURE_GSM_MDSP_EGPRS)
      /* If we are a loopback block, set the encode and loopback flags */
      block_params->burst_info.loopback_flag = block_params->srb_loopback;
      if (block_params->srb_loopback)
      {
         block_params->burst_info.encode_flag = TRUE;
      }
#endif

      /* Transmit the frame bursts */
      gl1_hw_tx_dtm_gprs
      (
         block_params->signal_info,
         block_params->num_channels,
         &block_params->burst_info,
         block_params->tags,
         block_params->num_tags,
         block_params->num_ul_timeslots,
         gas_id
      );
   }

   /* Check if this is the end of the block and there is no more data */
   if (end_of_block &&
       ul_pdch_params[gas_id].block_params[ul_pdch_params[gas_id].next_block_ptr].num_msgs == 0)
   {
      /* If we are aborting we need to clean out any USF granularity blocks */
      if (ul_pdch_params[gas_id].abort && ul_pdch_params[gas_id].usf_granularity_allocation)
      {
         for (i = 0; i < GL1_DEFS_SLOTS_IN_FRAME; i++)
         {
            ul_pdch_params[gas_id].usf_granularity_blocks[i] = 0;
         }
         ul_pdch_params[gas_id].usf_granularity_allocation = FALSE;
      }

      /* If we have no more USF gran blocks to keep track of we are done */
      if (!ul_pdch_params[gas_id].usf_granularity_allocation)
      {
         L1_STATE_CHANGE(ul_pdch_params[gas_id].state = UL_STATE_INACTIVE, "UL_STATE_INACTIVE");
         remove_handler = TRUE;
      }
   }

   return remove_handler;

/*lint -esym(715,time_stamp)*/ /* time_stamp parameter not used */
}


