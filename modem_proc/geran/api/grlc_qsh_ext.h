/*****************************************************************************
***
*** TITLE
***
***  GERAN RLC QSH Support
***
***
*** DESCRIPTION
***
***  Definitions of QSH mertric dumps and crash dump for GERAN RLC uplink 
***  and downlink tasks
***
***
*** Copyright (c) 2001-2016 Qualcomm Technologies, Inc.
*** All Rights Reserved. QUALCOMM Proprietary.
***
*** Export of this technology or software is regulated by the U.S.
*** Government. Diversion contrary to U.S. law prohibited.
***
*****************************************************************************/


/*****************************************************************************
***
***                        EDIT HISTORY FOR MODULE
***
***  This section contains comments describing changes made to the module.
***  Notice that changes are listed in reverse chronological order.
***
*** $Header: //components/rel/geran.mpss/7.4.0/api/grlc_qsh_ext.h#1 $
***
*** when       who     what, where, why
*** --------   ---     ------------------------------------------------------
*** 03/03/15    hv     Added QSH support
*****************************************************************************/

#ifndef INC_GRLC_QSH_H
#define INC_GRLC_QSH_H

#include <qsh.h>

#define GRLCUL_QSH_MAJOR_VER 1
#define GRLCUL_QSH_MINOR_VER 1

#define GRLCDL_QSH_MAJOR_VER 1
#define GRLCDL_QSH_MINOR_VER 1


/* --------  Definitions for trace buffer for inter-task messages  -------- */

/* RLC Uplink Task message buffer element.
** Each contains basic info for every message received and sent by RLC Uplink Task (per gas_id)
*/
typedef struct
{
  /* total 16 octets per message element for every message sent and received by RLC uplink task */

  uint8   msg_set;      /* say MAC & RLC */
  uint8   msg_id;       /* 1st octet of msg_frag */

  uint8   state;        /* US_NULL, US_ACK_XFER, etc. */
  uint8   sub_state;    /* sub state if any */

  uint8   realloc_state;/* reallocation state */
  uint8   gas_id;       /* subscriber number */

  uint8   msg_frag[10]; /* the first 10 bytes of message */
} grlc_ul_trace_msg_t;

/* -----------------------  Definitions for QSH Dump  --------------------- */

typedef enum
{
  GRLCUL_QSH_DUMP_TAG_DATA   = 0,
  GRLCUL_QSH_DUMP_TAG_DATA_2 = 1,
} grlcul_qsh_dump_tag_e;

typedef enum
{
  GRLCDL_QSH_DUMP_TAG_DATA   = 0,
  GRLCDL_QSH_DUMP_TAG_DATA_2 = 1,
} grlcdl_qsh_dump_tag_e;

/* Key variables in GERAN RLC Uplink Task (per gas_id)
** This structure also contains the array of structures holding basic info of the last 30 messages sent and received
**
** Total size is 617 octets by gas_id of RLC Uplink Task
*/
typedef struct
{
  /*------------------------
  ** Uplink RLC sub-states
  **------------------------
  */

  /* set to TRUE after one time initialisation of UL has been done.
  */
  uint8 b_done_ul_one_time_init;

  /*-------------------------------------
  ** uplink RLC state machine variables
  **-------------------------------------
  */

  /* current rlc ul state
  */
  uint8 state;

  /*--------------------------------------------------------------------------
  ** Flag set when LLC sends GPRS Suspend signal. Cleared when LLC sends GPRS
  ** Resume signal and at power up
  **--------------------------------------------------------------------------
  */
  uint8 suspend_state;

  /* Stall Indicator field value.
  */
  uint8 si;

  /* Block sequence number
  */
  uint16  bsn;

  /*------------------------------------------------------
  ** transmit flags. Only meaningful during transmission
  **------------------------------------------------------
  */

  /*---------------------
  ** transmit variables
  **---------------------
  */

  /* segment index variable.
  ** Index to the next free location for a radio block to be attached
  ** to the transmit array. It is always greater than or equal to V(S).
  */
  uint16 seg_ind;


  /* V(S) send state variable.
  ** Denotes the sequence number of the next-in-seq RLC data block to be sent.
  ** Set to zero initially and incremented after each transmission of a
  ** PENDING_TRANSMIT (PTX) data block which has not been transmitted before.
  ** The present of the FIFO means this variable is only incremented after PL1
  ** has performed a Get-access on a data block.
  **
  ** This variable controls the PTX blocks.
  ** The range is between V(A) and In_index.
  */
  uint16 vs;

  /* V(S)F send state variable which holds the BSN of the next block to be sent
  ** to the RLC-PL1 UL FIFO. This is different to V(S) since V(S) is only
  ** incremented when PL1 performs a Get-access whereas this is incremented
  ** after a block is sent to the FIFO.
  **
  ** The range is between V(S) and seg_ind
  */
  uint16 vsf;

  /* V(A) acknowledge state.
  ** In acknowledged mode it holds the BSN value of the oldest RLC data block
  ** that has not been positively ack'ed (NACK or PACK)
  **
  ** In unack mode, it marks the oldest data block sent after a paknak is
  ** received. In this way, we can check the distance between V(A) and V(S)
  ** to detect 'stall' condition so that post_tx_actions() knows when to
  ** start T3182.
  */
  uint16 va;

  /* Send state variable for the pending ack block.
  ** This variable holds the BSN of the next PACK block to be sent to the FIFO.
  ** This is required for RLC to keep track of the next PACK block to send to
  ** the FIFO on different invocations.
  **
  ** This variable covers the un-ack blocks which includes the NACK blocks
  ** hence the range is between V(A) and V(S). When RLC has no new NACK blocks
  ** or PTX blocks to send it re-sends the PACK blocks. When pack_vsf passes V(S)
  ** it wraps back to V(A) again.
  **
  ** Since a PUAN could result in V(A) being advanced, on every invocation
  ** pack_vsf is updated to be in its correct range
  */
  uint16 pack_vsf;

  /* Send state variable for the NACK blocks.
  ** This variable holds the BSN of block where the search for the next NACK
  ** block to send to the FIFO starts
  **
  ** The range is between V(A) and V(S).
  */
  uint16 nack_vsf;


  /* Starting sequence number (SSN) in the packet ack/nack msg from the n/w.
  */
  uint16 ssn;

  /* Re-transmit count for the last block (CV = 0). In unack mode,
  ** when waiting for the FAI to set after sending the last block with
  ** CV = 0, RLC keeps a count of the number of re-transmissions. When the count
  ** reaches 4, no more re-tx takes place and the timer t3182 will expire
  ** causing an abnormal release with random access.
  */
  uint8 re_tx_cv_0_cnt;

  /*-------------------------------
  ** operational status variables
  **-------------------------------
  */

  /* Boolean indicating access condition. FALSE at power up and on receipt of
  ** MR_NO_PS_ACCESS_IND. TRUE on receipt of MR_PS_ACCESS_REQ
  */
  uint8 b_access;

  /* uplink release cause as indicated by MAC
  */
  uint8 ul_release_cause;

  /* Current ul rlc mode
  */
  uint8 rlc_mode;

  /* Current number of ts assigned.
  */

  /* pre_emptive_tx for egprs
  */
  uint8 pre_emptive_tx;

  /* In one-phase access mode, this value should be assigned the coding scheme
  ** as follows:
  ** if alloc_cnf.tlli_block_cs = 0, tlli_cs = RM_CS_1
  ** if alloc_cnf.tlli_block_cs = 1, tlli_cs = alloc_cnf.cs
  **
  ** For EGPRS
  ** if alloc_cnf.tlli_block_cs = 0, tlli_cs = RM_MCS_1
  ** if alloc_cnf.tlli_block_cs = 1, tlli_cs = alloc_cnf.mcs
  */
  uint8 tlli_cs;

  /* MCS of the last correctly received data block
  */
  uint8 last_valid_mcs;

  /* contention resolution status of uplink access.
  ** Only used during one-phase access. When unresolved the tlli field must
  ** be included in rlc uplink data blocks. When resolved the tlli field is
  ** omitted.
  **
  ** Default is unresolved
  */
  uint8 contention_status;

  /* Flag to indicate a block has been transmitted by PL1. This is set to FALSE
  ** at the start of a TBF. It is set by the post_tx_action() function to trigger
  ** RLC to start sending request to re-allocate. Because when RLC is in transfer
  ** MAC and PL1 may not tey reached that state
  */
  uint8 b_first_blk_sent;

  /* flag to indicate to L1 to start checking the contents of the UL FIFO
  ** for data blocks. This is set to TRUE at the beginning of a TBF and
  ** after a coding change takes place.
  **
  ** The transmit function loads up the FIFO with data blocks and checks
  ** this flag. If set, it will send L1 the PH_DATA_REQ and clears the
  ** flag. So that L1 only receives the primitive once for the duration
  ** of a TBF, unless there is a coding scheme change.
  */
  uint8 b_first_fifo_load;

  /*--------------------------------
  ** countdown procedure variables
  **--------------------------------
  */

  /* current CV (Countdown Value). This value is pre-calculated and loaded
  ** into radio block during segmentation.
  */
  uint8 cv;

  /* Indicate when a forced release of a TBF is required, eg. NORMAL_RELEASE
  */
  uint8 b_forced_release;

  /* bsn of the block with CV value being 0. This is used to index the
  ** block to re-send block with CV = 0 to the peer during release state
  */
  uint8 bsn_of_cv_0_block;

  uint16  e_bsn_of_cv_0_block;

  /* flag to indicate RLC data block with CV = 0 has been sent
  */
  uint8 b_cv_0;

  /* CV value of the previously transmitted PTX block
  */
  uint8 ptx_cv;

  /* CV value of the previously transmitted PACK block (for un-ack mode)
  */
  uint8 pak_cv;

  /*------------------------------
  ** RLC uplink operation timers
  **------------------------------
  */

  /* MS Counter for handling cell-reselection. This is a signed value
  */
  int8  n3102;

  /* Contention resolution counter
  */
  uint8 n3104;

  uint16  timer_await_ul_acknack_expiry_cnt;
  uint8 guard_timer_state;

  /* stat of t3182 expiry
  */
  uint16  timer_t3182_expiry_cnt;

  /* Count the number of times this timer has been activated since the last power up
  */
  uint16  timer_await_ass_guard_expiry_cnt;

  /* Number of times stale pdu delete timer has expired since power up.
  */
  uint16 timer_stale_pdu_delete_expiry_cnt;

  /*----------------------------------------
  ** transmit resource related information
  **----------------------------------------
  */

  /* the current reallocation sub state within the xfer state
  */
  uint8 realloc_state;

  /* PDU sequence number where PRR type 2 should be sent
  */
  uint16  realloc_psn;

  /* Number of llc pdu octets received and have not been delivered with
  ** acknowledgement.
  */
  uint16  llc_pdus;
  uint32  llc_octets;
  uint16  susp_state_data_pdus;

  /* Logging counters for each type of blocks transmitted during an uplink TBF
  */
  uint16  nack_cnt;
  uint16  ptx_cnt;
  uint16  pack_cnt;
  uint16  num_ests;

  /* Flag is set when an error has occurred which requires RLC UL to recover
  ** gracefully by flushing its queue and starting afresh. At present, only
  ** dup item exhaustion will lead to this sort of recovery
  */
  uint8 b_error_recovery_required;

  /* Pending ack queue stores pointers to the LLC PDU message that are in xfer and
  ** awaiting acknowledgement from the NW. When a PDU is loaded into 'cur_pdu_ptr',
  ** a copy is entered into this queue. When a PDU is acknowledged by the NW it is
  ** removed from this queue.
  */
  uint16  paq_in;
  uint16  paq_out;
  uint16  paq_ctr;
  uint16  paq_ctr_max;

  /*
  */
  uint16  psn_to_free_inclusive;

  /* Flag to indicate if LLC is flow-controlled (1)
  */
  uint16  flow_control_suspend;

  /* Counts the number of panic resets requested by MAC from power up
  */
  uint16  panic_reset_cnt;

  /* used to delete incoming signalling pdu under no service case
  */
  uint8 no_ps_access_del_pdu_cause;

  /* Set when an additional flow control event is needed to adjust flow control based on tx blankings
  */
  uint8 b_adj_flow_control;

  /* The current octet count in the uplink temporary queue. This value and the ''llc_octets' together
  ** determines the flow control
  */
  uint32  temp_queue_octet_cnt;
  uint16  temp_queue_pdu_cnt;

  /* uplink abnormal release counts
  */
  uint16 access_reject_cnt;
  uint16 arc_retry_cnt;
  uint16 arc_wo_retry_cnt;
  uint16 arc_sys_info_cnt;

  /* Event Counts
  */
  uint16 llc_event_cnt;
  uint16 mac_event_cnt;
  uint16 pl1_event_cnt;
  uint16 tmr_event_cnt;

  /* PDU & octet counts
  */
  uint32 ack_tx_pdu_cnt;
  uint32 ack_tx_oct_cnt;
  uint32 unack_tx_pdu_cnt;
  uint32 unack_tx_oct_cnt;
  uint16 coding_sch_changes;

  /* General purpose uplink counters
  */
  uint32 ul_gen_purpose_count_a;

  /* 617 bytes */
} grlcul_dump_tag_data_s;

/* Key variables in GERAN RLC Downlink Task (per gas_id)
** No array of messages is required as downlink operations are much simpler than uplink task
**
** Total size is 83 octets by gas_id of RLC Downlink Task
*/
typedef struct
{
  /*-------------------------
  ** Downlink RLC sub-states
  **-------------------------
  */
  /* Determines if MS in GPRS or EGPRS mode
  */
  uint8 tbf_mode;

  /* EGPRS Window Size */
  uint16  ws;

  /* current state of rlc downlink state machine
  */
  uint8 state;

  /* V(R) Receive state variable
  **
  ** Denotes the end of the receiver window and hence holds the BSN value of
  ** the next block to be received.
  **
  ** In ack mode V(R) is set to 1+BSN' -- where BSN' is the most recently
  ** received block -- if the block is error free and the BSN's is still
  ** within the k window size.
  **
  ** In un-ack mode, V(R) is set to 1+BSN'.
  */
  uint16  vr;

   /*
   ** set during each start of DL TBF to know what physical
   ** array size FW is doing EDGE IR.
   */
   uint16 ir_oper_win_size;
   /*
   ** Its like max bsn (actual bsn and not bsn +1) to know
   ** what max bsn we are operating and if this falls in
   ** operating range , ms_out_of_mem =0 else it is made 1.
   **
   */
   uint16 vr_max;
   /*
   ** Set when bsn falls outside physical memory space required
   ** by FW for EDGE IR
   */
   uint8 ms_out_of_mem;

  /* V(Q) Receive 'window' state variable
  **
  ** Denotes the lowest BSN not yet received and therefore represents the start
  ** of the receive window.
  **
  ** In ack mode, when a block whose BSN is equal to V(Q) is received, V(Q) is
  ** set to V(R) if all blocks are received OK or it is set to the value of the
  ** BSN that is closest to V(Q) in the receive window element whose value
  ** is not RECEIVED.
  **
  ** In un-ack mode, if V(R) - V(Q) > k, ie. V(R) points passed k from V(Q)
  ** then V(Q) is set to: V(Q) = V(R) - k, ie. the start of the rx window, or
  ** V(Q) is shifted along.
  */
  uint16  vq;

  /* Re-assembly indexes for DL UNACK mode when out of seq BSN are detected in a frame.
  ** ass_start points at the next in seq BSN to be assembled.
  ** ass_end points at the highest rcved BSN in the frame number currently processed.
  ** When a new DL block is received in a new frame number, then all blocks
  ** between ass_start and ass_end are re-assembled into an LLC pdu.
  */
  uint16  ass_start;
  uint16  ass_end;

  /* Starting sequence number (SSN)
  ** Assigned thevalue of the receive state variable V(R).
  ** This is returned with the receive block bitmap (RBB) in the packet downlink
  ** acknack message.
  */
  uint16  ssn;

  /* boolean to indicate if RLC has sent to MAC the RM_DL_FIRST_BLOCK_IND
  ** message so MAC can enter DL packet transfer mode.
  ** This is set to FALSE at assignment and set to TRUE after RLC has received
  ** the 1st data block from the n/w and has sent off the indication to MAC
  */
  uint8 b_first_block_ind_sent;

  /* current channel coding scheme for gprs or egprs.
  */
  uint8 cs;

  /* the last valid channel coding scheme
  */
  uint8 last_valid_channel_coding;

  /* current downlink block sequence number.
  ** It is the last bsn that have been assembled into a pdu.
  */
  uint16  bsn;

  uint16  timer_t3190_expiry_cnt;
  uint16  timer_t3192_expiry_cnt;

  /* ENG MODE block counters*/

  uint16  eng_mode_rcvd_dl_block_cnt;
  uint16  eng_mode_rcvd_dup_dl_block_cnt;

  /* Receive block counter. This is a free running counter initialised at
  ** grlc dl task start up.
  */
  uint32  rx_blk_cnt;

  /* When Tuneaway occurs, tx/rx actitivies are discarded for reading paging on the other subscriber.
  ** This var holds the VETO status for GRLC downlink task, which disables (vote against TuneAway) when
  ** un-ack mode xfer is taking place. This variable is set to FALSE on starting up and when an RLC
  ** ack-mode downlink TBF is being assigned, and it is set to TRUE on an ack-mode RLC downlink TBF.
  ** This variable will reduce un-necessary voting when many short downlink TBF occurs
  */
  uint8 b_block_tuneaway;

  /* PDU & octet counts
  */
  uint32 ack_rx_pdu_cnt;
  uint32 ack_rx_oct_cnt;
  uint32 unack_rx_pdu_cnt;
  uint32 unack_pdu_filled_cnt;
  uint32 unack_rx_oct_cnt;
  uint32 unack_oct_filled_cnt;
  uint32 ack_nack_req_cnt;

  /* General purpose downlink counters
  */
  uint32 dl_gen_purpose_count_a;

  /* total 83 */
} grlcdl_dump_tag_data_s;

/* GRLC_QSH_DUMP_TAG_DATA UL data structure for QSH */
typedef struct
{
  qsh_dump_tag_hdr_s        hdr;
  grlcul_dump_tag_data_s   dump_ul_data;
} grlcul_qsh_dump_tag_data_s;

/* GRLC_QSH_DUMP_TAG_DATA DL data structure for QSH */
typedef struct
{
  qsh_dump_tag_hdr_s        hdr;
  grlcdl_dump_tag_data_s   dump_dl_data;
} grlcdl_qsh_dump_tag_data_s;

/* Pointer declared as type of QSH data UL structure for parser */
extern grlcul_qsh_dump_tag_data_s  *grlcul_qsh_dump_tag_0_ptr;
extern grlcul_qsh_dump_tag_data_s  *grlcul_qsh_dump_tag_1_ptr;

/* Pointer declared as type of QSH data DL structure for parser */
extern grlcdl_qsh_dump_tag_data_s  *grlcdl_qsh_dump_tag_0_ptr;
extern grlcdl_qsh_dump_tag_data_s  *grlcdl_qsh_dump_tag_1_ptr;

/* ----------------  Definitions for QSH Metrics & Events  -----------------*/

typedef enum
{
  GRLCUL_QSH_METRIC_DATA   = 0,
} grlcul_qsh_metric_e;

typedef enum
{
  GRLCDL_QSH_METRIC_DATA   = 0,
} grlcdl_qsh_metric_e;

/* Key metrics/events in GERAN RLC Downlink Task (per gas_id)
** This structure also contains the array of structures holding basic info of the last 25 messages sent and received
*/
typedef struct
{
  /* Number of llc pdu octets received and have not been delivered with
  ** acknowledgement.
  */
  uint16  susp_state_data_pdus;

  /* The current octet count in the uplink temporary queue. This value and the ''llc_octets' together
  ** determines the flow control
  */
  uint32  temp_queue_octet_cnt;
  uint16  temp_queue_pdu_cnt;

} grlcul_debug_metrics_s;



/* Key metrics/events in GERAN RLC Downlink Task (per gas_id)
** No array of messages is required as downlink operations are much simpler than uplink task
*/
typedef struct
{
  /* DL Timer Expiry Counts
  */
  uint16  timer_t3190_expiry_cnt;
  uint16  timer_t3192_expiry_cnt;

  /* PDU & octet counts
  */
  uint32 ack_rx_pdu_cnt;
  uint32 ack_rx_oct_cnt;
  uint32 unack_rx_pdu_cnt;
  uint32 unack_pdu_filled_cnt;
  uint32 unack_rx_oct_cnt;
  uint32 unack_oct_filled_cnt;
  uint32 ack_nack_req_cnt;

} grlcdl_debug_metrics_s;

/* GRLC QSH UL metrics structure */
typedef struct
{
  qsh_metric_hdr_s            hdr;
  grlcul_debug_metrics_s     metrics_data_ul;
} grlcul_qsh_metric_data_s;

/* GRLC QSH DL metrics structure */
typedef struct
{
  qsh_metric_hdr_s            hdr;
  grlcdl_debug_metrics_s     metrics_data_dl;
} grlcdl_qsh_metric_data_s;

/* EOF */

#endif /* INC_GRLC_QSH_H */
