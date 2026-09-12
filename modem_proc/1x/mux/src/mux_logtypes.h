#ifndef MUX_LOGTYPES_H
#define MUX_LOGTYPES_H

/*===========================================================================

                      MUX Log Services Header File

General Description
  This file contains declarations for packets to be logged for the MUX player.

  Copyright (c) 2001 - 2014 Qualcomm Technologies, Inc. 
  All Rights Reserved Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc. and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc.

===========================================================================*/

/*===========================================================================

                             Edit History

$PVCSPath: L:/src/asw/COMMON/vcs/mux_logtypes.h_v   1.7   20 Aug 2002 11:27:12   randyp  $
$Header: //components/rel/1x.mpss/8.0/mux/src/mux_logtypes.h#1 $ $DateTime: 2019/06/11 23:51:40 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/23/14   eye     Added support for 0x1877 Power Reporting Log Packet.
10/03/12   srk     Remove the unused TX power thresholds saved for logging.
08/02/12   srk     Added Device only DTX parameters in 1x Adv logging
02/11/11   jtm     Added RCPM FPC_MODE support for 1xA.
12/03/10   jtm     Removed MDR/SCCH support.
06/15/10   jtm     Organized definitions.
06/15/10   jtm     Added new 1xA Perf Log format and FET Stats log.
06/10/10   jtm     Relocated Mux specific log types here from log_dmss.h to
                   eliminate the dependency on that file. Also changed PACKED
                   to PACK().
05/20/10   jtm     Relocated ffpc logging types here and renamed using mux_*.
                   Replaced PACKED with PACK().
02/04/10   jtm     Removed FEATURE_PLT.
03/26/09   jtm     Eliminated implicit includes.
04/27/05   vlc     Main lined code in current targets, cleaned out obsolete targets,
                   cleaned out obsolete features.
04/20/04   jrp     Fixed lint warnings.
11/15/02   jrp     Added band class to LOG_EA_PROBE_F packet.
08/20/02   jrp     Added log type for REACH probe, log code BD.
07/10/02   dlb2    Featurized previous change with MSMHW_EXTENDED_ENERGY_MATRIX.
04/24/02   dlb2    Increased size of ser field for sch frame info to 16 bits.
01/09/02   bn      Added the frame offset into RDA log
11/20/01   bn      Added RDA logging logic
09/05/01   bn      Created the REL A Logging header file for the MUX layer

===========================================================================*/

//common
#include "1x_variation.h"
#include "comdef.h"
#include "customer.h"

//CP
#include "cai.h"

//other
#include "log.h"
#include "log_codes.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/* This buffer can hold up to a duration up to 200 ms */
#define FRDCH_LOG_BUFFER_SIZE (256)
/*the threshold = the buffer size - the maximum of byte per record in the
worst case analysic */
#define FRDCH_LOG_BUFFER_THRESHOLD (FRDCH_LOG_BUFFER_SIZE - 32)

/******************         COMMON MUX LOGGING TYPES            *****************/

typedef union
{
  struct
  {
    uint8 rpich     :1;
    uint8 fch       :1;
    uint8 dcch      :1;
    uint8 sch0      :1;
    uint8 reserved  :4;
  } individual_channels;
  uint8 mask; /* Use to init entire mask. */

} channels_info_type;

/*****************  FDCH FRAME INFORMATION **********************/
typedef struct
{
  uint8  reference_buf[1];

  /*
  Fill the Reference Buffer with the following:

  - If FCH is assigned, include 1 byte for FCH_CATEGORY

  - If DCCH is assigned, include 1 byte for DCCH_CATEGORY

  - If SCH is assigned, include 1 byte. The possible value for the SCH frame categories are:
    sufficient quality, insufficient quality or DTX

  - If SCH is assigned, and SCH frame category is insufficient quality and LTU number is greater than
    zero, then including 1 byte of LTU CRC */

} fdch_frame_type;

typedef struct
{
  uint8  for_sch_rc;
  uint8  for_sch0_mux_option_hi;
  uint8  for_sch0_mux_option_lo;
  uint8  sch_frame_length;
  uint8  sch0_num_bits;
  uint8  num_mux_pdu_per_sch_frame;
  uint8  num_ltu_per_sch_frame;

} fwd_sch_information_type;

typedef struct
{
  channels_info_type  assigned_channels;
  uint8           num_frames;
  uint8           reference_buf[1];
  /*
  Fill the Reference Buffer with the following:

  - If FCH is assigned, include 1 byte for FOR_FCH_RC

  - If DCCH is assigned, include 1 byte for FOR_DCCH_RC

  - If SCH is assigned, include 1 instance of FWD_SCH_INFORMATION_TYPE

  - Include NUM_FRAMES instances of FDCH_FRAME_TYPE
  */
} fdch_sub_record_type;


typedef struct
{
  log_hdr_type hdr;    // Log header (length, code, timestamp)
  uint8        frame_offset;
  uint8        num_sub_records;
  uint8        reference_buf[1];

  /*
  Fill the Reference Buffer with NUM_SUB_RECORDS instances of FDCH_SUB_RECORD_TYPE
  */

} log_fdch_information_type;


/*****************  RDCH FRAME INFORMATION  **********************/

typedef struct
{
  uint8 sr_id;  //THIS FIELD IS NOT INCLUDED IF MUX_PDU_CATEGORY INDICATES A DTX SCH FRAME

} r_sch_mux_pdu_information_type;


typedef struct
{
  uint8  reference_buf[1];

  /*
  Fill the Reference Buffer with the following:

  - If FCH is assigned, include 1 byte for FCH_FORMAT

  - If DCCH is assigned, include 1 byte for DCCH_FORMAT

  - If SCH is assigned, include NUM_MUX_PDU_PER_SCH_FRAME instances of R_SCH_MUX_PDU_INFORMATION_TYPE

  */

} rdch_frame_type;

typedef struct
{
  uint8  rev_sch_rc;
  uint8  rev_sch0_mux_option_hi;
  uint8  rev_sch0_mux_option_lo;
  uint8  sch_frame_length;
  uint8  sch0_num_bits;
  uint8  num_mux_pdu_per_sch_frame;
} rev_sch_information_type;

typedef struct
{
  channels_info_type  assigned_channels;
  uint8           num_frames;
  uint8           reference_buf[1];
  /*
  Fill the Reference Buffer with the following:

  - If FCH is assigned, include 1 byte for REV_FCH_RC and 1 byte for FCH_GATING.

  - If DCCH is assigned, include 1 byte for REV_DCCH_RC

  - If SCH is assigned, include 1 instance of REV_SCH_INFORMATION_TYPE

  - Include NUM_FRAMES instances of RDCH_FRAME_TYPE
  */

} rdch_sub_record_type;



typedef struct
{
  log_hdr_type hdr;    // Log header (length, code, timestamp)
  uint8        frame_offset;
  uint8        num_sub_records;
  uint8        reference_buf[1];
  /*
  Fill the Reference Buffer with NUM_SUB_RECORDS instances of RDCH_SUB_RECORD_TYPE
  */

} log_puma_rdch_information_type;

/*****************  RDA FRAME INFORMATION **********************/

/* This buffer can hold up to a duration up to 200 ms */
#define RDA_LOG_BUFFER_SIZE (512)
/*the threshold = the buffer size - the maximum of byte per record in the
worst case analysic */
#define RDA_LOG_BUFFER_THRESHOLD (RDA_LOG_BUFFER_SIZE - 72)

typedef struct
{
  uint8   quality_decision     :1;
  uint8   status               :2;
  uint8   reserve              :5;
} dcch_quadec_status_type;

typedef union
{
  struct
  {
    uint16   fch_rc       :3;
    uint16   dcch_rc      :3;
    uint16   sch_rc       :3;
    uint16   frame_offset :7;
  } chan_rc;
  uint16  mask;
} chan_rc_type;

typedef union
{
  struct
  {
    uint16  ltu_crc;
    uint16  ser;
    uint8   status; /* frame CRC */
    uint8   reserved[3];
  } dev_sch_conv;

  struct
  {
    uint16  min_llr;       /* Log Likelihood Ratio */
    uint8   num_iteration; /* Indicates the number of iterations run by the TD */
    uint8   crc_pass;
    uint8   reserved[4];
  } dev_sch_turbo;
} sch_code_type;

typedef struct
{
  uint32    norm_ren_energy;
  uint16    ser;      /* symbol error rate */
  uint8     decision; /* Full, Haft, Quater, Eight, Erasure */
  uint8     status;

} fch_rda_sub_record_type;

typedef struct
{
  uint32                      norm_ren_energy;
  uint32                      quality_threshold;
  uint16                      fch_nt_io;
  uint16                      pc_acc;
  uint16                      ser;      /* symbol error rate */
  uint8                       decision; /* Full, DTX, Erasure */
  dcch_quadec_status_type     quastat;

} dcch_rda_sub_record_type;

typedef struct
{
  uint32         norm_ren_energy;
  sch_code_type  code_info;
  uint16         iq_acc;
  uint16         sch_nt_io;
  uint16         rate;
  uint8          decision;  /* Full, DTX, Erasure */
  uint8          code_type; /* turbo or conv */
} sch_rda_sub_record_type;

typedef struct
{
  chan_rc_type        rc;                    /* RC structure for all the channels and frame offset*/
  channels_info_type  assigned_channels;     /* Channels bit-mask */
  uint8               num_sub_records;
  uint8               reference_buf[1];      /* reference buffer */
  /*  For each 20 ms time framce:
  Fill the Reference Buffer with NUM_SUB_RECORDS instances of
  - If FCH is active then there are 4 fch_rda_sub_record_type
  sub-records( Full, Haft, Quater, and Eight)
  - If DCCH is active then there is 1 dcch_rda_sub_record_type sub-record
  - If SCH is active then there is 1 sch_rda_sub_record_type sub-record
  */
} rda_sub_record_header_type;

typedef struct
{
  log_hdr_type                  hdr;      /* Log header (length, code, timestamp) */
  rda_sub_record_header_type    srheader;
} log_puma_rda_information_type;



/*****************  BCCH FRAME INFORMATION  **********************/
/* LOG_BCCH_FRAME_INFO_C */
typedef PACK( struct )
{
  log_hdr_type hdr;

  byte data_rate;
  byte code_rate;
  byte code_chan;

  byte num_frames;
  byte frame_quality[1]; /* Array size '(num_frames + 7) /8 */

} mux_bcch_frame_info_type;


/*****************  FCCCH FRAME INFORMATION  **********************/
/* LOG_BCCH_FRAME_INFO_C */
typedef PACK( struct )
{
  log_hdr_type hdr;

  byte data_rate;
  byte code_rate;
  byte code_chan;

  byte num_frames;
  byte frame_quality[1]; /* Array size '(num_frames + 7) /8 */

} mux_fccch_frame_info_type;


/*****************  FRAME OR MESSAGE DATA LOG  **********************/
/* Used for logging a raw message or frame data */
typedef PACK( struct )
{
  log_hdr_type hdr;

   byte msg[1]; /* Length to be determined programmatically */
} mux_msg_log_type;

/*****************  REACH FRAME INFORMATION  **********************/
typedef struct
{
  log_hdr_type  hdr;

  /* Gain adjustment of the Enhanced Access Channel or Reverse Common
   * Control Channel relative to the Reverse Pilot Channel.  This field
   * is received on the Enhanced Access Parameters Message.  It is a two's
   * complement value in units of 0.125 dB. */
  uint8         rlgain_com_pilot;

  /* Access sub-attempt number of the access attempt where the access
   * probe belongs to.  There is a sub-attempt for each BS to which an
   * Access probe is sent. */
  uint8         sub_att_number;

  /* Sequence number of the access sub-attempt. */
  uint8         sequence_number;

  /* Probe number of the access sequence corresponding to the access probe. */
  uint8         access_prob_number;

  /* One less than the maximum number of access probes a mobile station is
   * to transmit in a single access probe sequence. */
  uint8         each_num_step;

  /* Number of Persistence Tests Performed. */
  uint8         psist_delay;

  /* Enhanced Access Channel sequence backoff
   * One less than the maximum number of slots a mobile station is to
   * delay due to random backoff between successive EA probe sequences. */
  uint8         sequence_bkoff;

  /* Enhanced Access Channel probe backoff.
   * One less than the maximum number of slots a mobile station is to
   * delay due to random backoff between consecutive EA probes. */
  uint8         probe_bkoff;

  /* N, where the slot duration of the Enhanced Access Channel is (N+1) x 1.25ms. */
  uint8         each_slot;

  /* Forward Common Control Channel number. */
  uint8         fccch_id;

  /* Enhanced Access Channel identification */
  uint8         each_id;

  /* First slot offset for the Enhanced Access Channels.  This field is
   * received in the Enhanced Access Parameters Message. */
  uint8         each_slot_offset1;

  /* Base station identification. */
  uint16        base_id;

  /* Relative slot offset for the Enhanced Access Channels.  This field is
   * received in the Enhanced Access Parameters Message. */
  uint8         each_slot_offset2;

  /* Enhanced Access Channel rate selection for transmission.  This field
   * can be coded as RATE_WORD, in Table 2-29 of IS-2000-3, which describes
   * the rate and frame length and number of information bits per frame. */
  uint8         acc_rate;

  /* Received AGC power. */
  uint8         rx_agc;

  /* Transmit AGC power. */
  uint8         tx_agc;

  /* Transmit gain adjust. */
  uint8         tx_adj;

  /* Interference-correction */
  uint8         int_corr;

  /* floor(SYS_TIME/EACH_SLOTs), where SYS_TIME is the system time in units
   * of 1.25ms, at which the preamble is to be transmitted.  This field is
   * the lower 32 bits of this 42 bit value. */
  uint32        acc_preamble_tx_low32bit;

  /* This field is the high 10 bits of the 42 bit value, whose lower 32 bits
   * is the acc_preamble_tx_low32bit field. */
  uint16        acc_preamble_tx_high16bit;

  /* Length in bits of the transmitted access probe. */
  uint16        pdu_length;

  /* CDMA band class (cellular or PCS) */
  uint8         band_class;

} mux_reach_frame_info_log_type;

/*---------------------------------------------------------------------------
                  REVERSE LINK FRAME STATS ENTRY TYPE
---------------------------------------------------------------------------*/
#define MUX_LOG_R_MUX1_CNT  100

typedef PACK(struct)
{
  log_hdr_type  hdr;
  byte          vals[ MUX_LOG_R_MUX1_CNT ];
    /* Mixed mode bits for sequential reverse link traffic frames. */
}
mux_log_r_mux1_type;


/*---------------------------------------------------------------------------
                    ACCESS CHANNEL MESSAGE ENTRY TYPE
---------------------------------------------------------------------------*/

/* LOG_AC_MSG_C */
typedef PACK(struct)
{
  log_hdr_type  hdr;

  byte          msg[ CAI_AC_MAX_MSG_BYTES ];
    /* Actual access channel message bits. */

}
mux_log_ac_msg_type;


/*---------------------------------------------------------------------------
                    REVERSE TRAFFIC CHANNEL ENTRY TYPE
---------------------------------------------------------------------------*/

/* LOG_R_TC_MSG_C */
typedef PACK(struct)
{
  log_hdr_type  hdr;

  byte          msg[ CAI_REV_TC_MAX_MSG_BYTES ];
    /* Actual reverse link traffic channel message bits. */

}
mux_log_r_tc_msg_type;


/*---------------------------------------------------------------------------
                     SYNC CHANNEL MESSAGE ENTRY TYPE
---------------------------------------------------------------------------*/

/* LOG_SC_MSG_C */
typedef PACK(struct)
{
  log_hdr_type hdr;

  byte          msg[ CAI_SC_MAX_MSG_BYTES ];
    /* Actual sync channel message bits. */

}
mux_log_sc_msg_type;


/*---------------------------------------------------------------------------
                    PAGING CHANNEL MESSAGE ENTRY TYPE
---------------------------------------------------------------------------*/

/* LOG_PC_MSG_C */
typedef PACK(struct)
{
  log_hdr_type hdr;

  byte          msg[ CAI_PC_MAX_MSG_BYTES ];
    /* Actual paging channel message bits. */

}
mux_log_pc_msg_type;


/* FORWARD TRAFFIC CHANNEL MESSAGE ENTRY TYPE */
/* LOG_F_TC_MSG_C */
typedef PACK(struct)
{
  log_hdr_type hdr;

  byte          msg[ CAI_FWD_TC_MAX_MSG_BYTES ];
    /* Actual forward link traffic channel message bits. */

}
mux_log_f_tc_msg_type;



/*---------------------------------------------------------------------------
                         MARKOV FRAME STATISTICS
---------------------------------------------------------------------------*/
#define MUX_LOG_MAR_CNT 100

/* LOG_MAR_C */
typedef PACK(struct)
{
  log_hdr_type  hdr;

  byte          count;
    /* number of entries in entry array */

  PACK(struct)
  {
    byte          mar_inx;
    /* Decision indices for sequential frames. Each index value is computed
       according to the following formula:

         mar_inx = (EXPECTED_MUX1 * 16) + ACTUAL_MUX1

       where EXPECTED_MUX1(i) is the MUX1 index (1 to 14, a la CAI) predicted
       by the Markov process for frame i, and ACTUAL_MUX1 is the MUX1 index
       actually decided upon by the mobile's rate determination algorithm for
       frame i. */

    byte          bit_errs;
      /* Bit error counts for sequential frames. */
  }entry[ MUX_LOG_MAR_CNT];

}
mux_log_mar_type;


/* Access Probe Information Type
*/
/* LOG_ACC_INFO_C */
typedef PACK(struct)
{
  log_hdr_type hdr;

  byte seq_num;
    /* Access probe sequence number */
  byte probe_num;
    /* Access probe number */
  byte rx_agc;
    /* Receive AGC */
  byte tx_adj;
    /* TX gain adjust */
  byte psist;
    /* number of persistence tests performed */
  byte ra;
    /* access channel number */
  byte rn;
    /* PN randomization delay */
  byte rs;
    /* Sequence backoff */
  byte rt;
    /* probe backoff */
  byte int_corr;
    /* interference-correction  */

}
mux_log_acc_info_type;


/* Dedicated forward channel set mask.
*/
typedef PACK(union) {

  PACK(struct) {
    byte f_fch      : 1; /* F-FCH  active      */
    byte f_sch0     : 1; /* F-SCH1 active      */
    byte f_sch1     : 1; /* F-SCH0 active      */
    byte f_dcch     : 1; /* F-DCCH active      */
    byte reserved   : 4; /* unused/reserved    */

  } chan_set;

  byte mask; /* Used to init the entire field */

} mux_log_ded_fwd_chan_set_type;

/* Dedicated reverse channel set mask.
*/
typedef PACK(union) {

  PACK(struct) {
    byte r_fch     : 1; /* R-FCH  active     */
    byte r_sch0    : 1; /* R-SCH1 active     */
    byte r_sch1    : 1; /* R-SCH0 active     */
    byte r_dcch    : 1; /* R-DCCH active     */
    byte reserved  : 4; /* Unused/reserved   */

  } chan_set;

  byte mask; /* Used to init the entire field */

} mux_log_ded_rev_chan_set_type;


typedef PACK(struct) {
  uint16 f_fch_rc  :4; /* Forward Fundamental Channel Radio Config       */
  uint16 f_sch0_rc :4; /* Forward Supplemental Channel 0 Radio Config    */
  uint16 f_sch1_rc :4; /* Forward Supplemental Channel 1 Radio Config    */
  uint16 f_dcch_rc :4; /* Forward Dedicated Control Channel Radio Config */

} mux_log_fwd_ch_rc_type;

typedef PACK(struct) {
  uint16 r_fch_rc  :4; /* Reverse Fundamental Channel Radio Config       */
  uint16 r_sch0_rc :4; /* Reverse Supplemental Channel 0 Radio Config    */
  uint16 r_sch1_rc :4; /* Reverse Supplemental Channel 1 Radio Config    */
  uint16 r_dcch_rc :4; /* Reverse Dedicated Control Channel Radio Config */

} mux_log_rev_ch_rc_type;

/* Enum for FCH frame type */
typedef enum {
  MUX_LOG_FCH_TYPE_PAYLOAD_ONLY = 0,
  MUX_LOG_FCH_TYPE_DIM_BURST    = 1,
  MUX_LOG_FCH_TYPE_BLANK_BURST  = 2

} mux_log_fch_type_enum_type;

/* Enum for SCH channels */
typedef enum {
  MUX_LOG_SCH_FULL_RATE   = 0,
  MUX_LOG_SCH_BLANK_FRAME = 1,
  MUX_LOG_SCH_ERASUE      = 2,
  MUX_LOG_SCH_NOT_ACTIVE  = 3  /* Valid for SCH only */

} mux_log_sch_rate_enum_type; /* All SCH and SCCH channels */

/* Forward Frame Types frame record.  This structure defines each frame entry
** in the sub-record.
*/
typedef PACK(union) {

  PACK(struct) {
    byte rate : 4; /* Use log_f_fch_rate_enum_type */
    byte type : 4; /* Use log_fch_type_enum_type   */
  } f_fch;

  byte f_sch0; /* Use log_sch_rate_enum_type */
  byte f_sch1; /* Use log_sch_rate_enum_type */

  byte f_dcch; /* DCCH MUX type */

} mux_log_fwd_frame_types_frm_type;

/* Slowest case: 25bytes/1byte(per 20ms frame) --> 0.5 sec per log
** Fastest case: 25bytes/2bytes(per 20ms frame) --> 0.25 sec per log
*/
#define MUX_LOG_MAX_FRAME_TYPES_SIZE (25 + 4)

/* Forward Frame Types sub-record.  Used to log multiple frames
** with the same active channel set.  If the active channel set
** changes, a new sub-record is started.
*/
typedef PACK(struct) {
  mux_log_ded_fwd_chan_set_type ch_set_mask; /* Set of active channels */

  word frame_cnt; /* # of frames represented in this sub-record */

  byte len_per_frame; /* Length, in bytes, of each frame entry */

  /* Array of frame type indicators. One per channel per frame.
  ** Note: There will be an entry for each member that is associated with an
  ** active channel in the active channel mask.  Not all members will exist
  ** in each frame entry.  Entries appear in order of active LSB to MSB in
  ** the channel set mask (FCH first).
  */
  PACK(union) {
    /* Array of multiple frame entries.
    ** Up to 4 entries (4 ch or FCH + SCCH) per frame
    */
    mux_log_fwd_frame_types_frm_type frame_type_ind[4];

    /* Member for referenceing offsets in buf */
    byte buf_ref[MUX_LOG_MAX_FRAME_TYPES_SIZE - 4];

  } var_len_buf; /* sizeof(var_len_buf) gives max length */

} mux_log_fwd_frame_types_subrecord_type; /* FTSS */


/* Forward Frame Types Log Record
*/
/* LOG_FWD_FRAME_TYPES_C */
typedef PACK(struct)
{
  log_hdr_type hdr;

  byte frame_offset; /* Frame offset in 1.25 ms units. */

  word so;           /* Service option */

  mux_log_fwd_ch_rc_type fwd_ch_rc;
                     /* Forward channel Radio Config Info */

  byte sr_cnt;       /* Count of sub-records */

  /* Series of var length log_fwd_frame_types_subrecord_type */
  PACK(union) {
    /* Buffer will contain at least 1 subrecord */
    mux_log_fwd_frame_types_subrecord_type sr[1];

    /* Member for referenceing offsets in buf */
    byte buf_ref[MUX_LOG_MAX_FRAME_TYPES_SIZE];

  } var_len_buf; /* sizeof(var_len_buf) gives max length */

}
mux_log_fwd_frame_types_type;


/* Threshold to determine when the buf is full and the log should be sent.
*/
#define MUX_LOG_FWD_FRAME_TYPES_BUF_THRESHOLD \
  (sizeof(mux_log_fwd_frame_types_type) -     \
   FPOS(mux_log_fwd_frame_types_subrecord_type, var_len_buf.frame_type_ind[4]))


/* Reverse Frame Types frame record.  This structure defines each frame entry
** in the sub-record.
*/
typedef PACK(union) {

  PACK(struct) {
    byte rate : 4; /* Use log_r_fch_rate_enum_type */
    byte type : 4; /* Use log_fch_type_enum_type   */
  } r_fch;

  byte r_sch0; /* Use log_sch_rate_enum_type */
  byte r_sch1; /* Use log_sch_rate_enum_type */

  byte r_dcch; /* DCCH frame category */

} mux_log_rev_frame_types_frm_type;

/* Reverse Frame Types sub-record.  Used to log multiple frames with the same
** active channel set.  If the active channel set changes, a new
** sub-record is started.
*/
typedef PACK(struct) {
  mux_log_ded_rev_chan_set_type ch_set_mask; /* Set of active channels */

  word frame_cnt; /* # of frames represented in this sub-record */

  byte len_per_frame; /* Length, in bytes, of each frame entry */

  /* Array of frame type indicators. One per channel per frame.
  ** Note: There will be an entry for each member that is associated with an
  ** active channel in the active channel mask.  Not all members will exist
  ** in each frame entry.  Entries appear in order of active LSB to MSB in
  ** the channel set mask (FCH first).
  */
  PACK(union) {
    /* Array of multiple frame entries.
    ** Up to 4 entries (4 ch) per frame
    */
    mux_log_rev_frame_types_frm_type frame_type_ind[4]; /* 4 is the max per frame */

    /* Member for referenceing offsets in buf */
    byte buf_ref[MUX_LOG_MAX_FRAME_TYPES_SIZE - 4];

  } var_len_buf; /* sizeof(var_len_buf) gives max length */

} mux_log_rev_frame_types_subrecord_type; /* FTSS */

/* Reverse Frame Types Log Record
*/
/* LOG_REV_FRAME_TYPES_C */
typedef PACK(struct)
{
  log_hdr_type hdr;

  byte frame_offset; /* Frame offset in 1.25 ms units. */

  word so;           /* Service option */

  mux_log_rev_ch_rc_type rev_ch_rc;
                     /* Reverse channel Radio Config Info */

  byte sr_cnt;       /* Count of sub-records */

  /* Series of var length log_rev_frame_types_subrecord_type */
  PACK(union) {
    /* Buffer will contain at least 1 subrecord */
    mux_log_rev_frame_types_subrecord_type sr[1];

    /* Member for referenceing offsets in buf */
    byte buf_ref[MUX_LOG_MAX_FRAME_TYPES_SIZE];

  } var_len_buf; /* sizeof(var_len_buf) gives max length */

}
mux_log_rev_frame_types_type;



/* Threshold to determine when the buf is full and the log should be sent.
*/
#define MUX_LOG_REV_FRAME_TYPES_BUF_THRESHOLD \
  (sizeof(mux_log_fwd_frame_types_type) -     \
   FPOS(mux_log_rev_frame_types_subrecord_type, var_len_buf.frame_type_ind[4]))

/*---------------------------------------------------------------------------
                   FAST FORWARD POWER CONTROL LOG TYPE
---------------------------------------------------------------------------*/
/* Slowest case: 75bytes/3bytes(per 20ms frame) --> 0.5 sec per log
** Fastest case: 75bytes/5bytes(per 20ms frame) --> 0.3 sec per log
*/
#define MUX_LOG_MAX_FFPC_SIZE (75 + 4) /* 75 frame entries + 4 per sr */

/* Fast Forward Power Control frame record.  This structure defines each
** frame entry in the sub-record.
*/
typedef PACK(struct) {
  word dec_history; /* Decision History: 16 1-bit decisions per frame */

  /* List of ffpc setpoint indicators that are active in the channel set mask.
  ** Note: There will be an entry for each member that is associated with an
  ** active channel in the active channel mask.  Not all members will exist
  ** in each frame entry.  Entries appear in order of active LSB to MSB in
  ** the channel set mask (FCH first).
  */
  PACK(union) {
    uint8 f_fch;       /* F-FCH setpoint in 0.125 bB units       */
    uint8 f_sch0;      /* F-SCH0 setpoint in 0.125 bB units      */
    uint8 f_sch1;      /* F-SCH1 setpoint in 0.125 bB units      */
    uint8 f_dcch;      /* F-DCCH setpoint in 0.125 bB units      */
    int8  f_sch_delta; /* F-SCH delta setpoint in 0.125 dB units */

  } setpts[5]; /* Up to five entries based on channel set mask. */

} mux_log_ffwd_pctrl_frame_type;

/* Fast Forward Power Control sub-record.  Used to log multiple frames with
** the same active channel set.  If the active channel set changes, a new
** sub-record is started.
*/
typedef PACK(struct) {
  mux_log_ded_fwd_chan_set_type ch_set_mask; /* Set of active channels */

  word frame_cnt; /* # of frames represented in this sub-record */

  byte len_per_frame; /* Length, in bytes, of each frame entry */

  /* Variable length buffer containing 'frame_cnt'
  ** entries of log_ffwd_pctrl_frame_type
  */
  PACK(union) {

    /* Each frame entry is variable length based on channel set mask */
    mux_log_ffwd_pctrl_frame_type frame_vals[1];

    /* Member for referenceing offsets in buf */
    byte buf_ref[MUX_LOG_MAX_FFPC_SIZE - 4];

  } var_len_buf; /* sizeof(var_len_buf) gives max length */

} mux_log_ffwd_pctrl_subrecord_type;

/* Fast Forward Power Control Log Record
*/
/* LOG_FFWD_PCTRL_C */
typedef PACK(struct)
{
  log_hdr_type hdr;

  byte frame_offset; /* Frame offset in 1.25 ms units. */

  word so;               /* Service Option                 */
  byte fpc_mode;         /* FPC Mode                       */
  byte fpc_pri_chan;     /* Sub-channel Indicator          */
  byte fpc_subchan_gain; /* Sub-channel gain 0.25dB units  */

  byte sr_cnt;       /* Count of sub-records */

  /* Series of var length log_ffwd_pctrl_subrecord_type */
  PACK(union) {
    /* Buffer will contain at least 1 subrecord */
    mux_log_ffwd_pctrl_subrecord_type sr[1];

    /* Member for referenceing offsets in buf */
    byte buf_ref[MUX_LOG_MAX_FFPC_SIZE];

  } var_len_buf; /* sizeof(var_len_buf) gives max length */

}
mux_log_ffwd_pctrl_type;

/* Threshold to determine when the buf is full and the log should be sent.
*/
#define MUX_LOG_FFPC_BUF_THRESHOLD                          \
  (sizeof(mux_log_ffwd_pctrl_type) -                        \
    (FPOS(mux_log_ffwd_pctrl_subrecord_type, var_len_buf) + \
     sizeof(mux_log_ffwd_pctrl_frame_type)))

/*---------------------------------------------------------------------------
                 REVERSE POWER CONTROL LOG TYPE
---------------------------------------------------------------------------*/
/* Slowest case: 175bytes/7bytes(per 20ms frame) --> 0.5 sec per log
** Fastest case: 175bytes/9bytes(per 20ms frame) --> 0.38 sec per log
*/
#define MUX_LOG_MAX_REV_PCTRL_SIZE 175

/* Reverse Power Control frame record.  This structure defines each
** frame entry in the sub-record.
*/
typedef PACK(struct) {

  uint16 dec_history; /* Power control decision history */

  uint8  rx_agc;       /* RX AGC vals            */
  uint8  tx_pwr;       /* TX CAGC vals           */
  uint8  tx_gain_adj;  /* Gain adjust pctrl vals */

  /* List rpctrl setpoint indicators that are active in chan_mask.
  ** Note: There will be an entry for each member that is associated with an
  ** active channel in the active channel mask.  Not all members will exist
  ** in each frame entry.  Entries appear in order of active LSB to MSB in
  ** the channel set mask (FCH first).
  */
  PACK(union) {
    int16 r_fch_pich;  /* R-FCH TX power in 0.125 bB units  */
    int16 r_sch0_pich; /* R-SCH0 TX power in 0.125 bB units */
    int16 r_sch1_pich; /* R-SCH1 TX power in 0.125 bB units */
    int16 r_dcch_pich; /* R-DCCH TX power in 0.125 bB units */

  } tx_ch_pwr[4]; /* array of channel pwr vals. Up to 4 per frame. */
} mux_log_rev_pctrl_frame_type;

/* Reverse Power Control sub-record.  Used to log multiple frames with
** the same active channel set.  If the active channel set changes, a new
** sub-record is started.
*/
typedef PACK(struct) {
  mux_log_ded_rev_chan_set_type ch_set_mask; /* Set of active channels */

  word frame_cnt;   /* # of frames represented in this sub-record */

  byte len_per_frame; /* Length, in bytes, of each frame entry */

  /* Variable length buffer containing 'frame_cnt'
  ** entries of log_rev_pctrl_frame_type
  */
  PACK(union) {

    /* Each frame entry is variable length based on channel set mask */
    mux_log_rev_pctrl_frame_type frame_vals[1];

    /* Member for referenceing offsets in buf */
    byte buf_ref[MUX_LOG_MAX_REV_PCTRL_SIZE - 3];

  } var_len_buf; /* sizeof(var_len_buf) gives max length */

} mux_log_rev_pctrl_subrecord_type;

/* Reverse Power Control Log Record
*/
/* LOG_REV_PCTRL_C */
typedef PACK(struct)
{
  log_hdr_type hdr;

  byte frame_offset; /* Frame offset in 1.25 ms units. */

  byte band_class;   /* Band Class */

  mux_log_rev_ch_rc_type rev_ch_rc;
                     /* Reverse channel Radio Config Info */

  byte pilot_gating_rate; /* Pilot gating rate       */
  byte step_size;         /* Power control step size */

  byte sr_cnt;       /* Count of sub-records */

  /* Series of var length log_rev_pctrl_subrecord_type */
  PACK(union) {

    /* Buffer will contain at least 1 subrecord */
    mux_log_rev_pctrl_subrecord_type sr[1];

    /* Member for referenceing offsets in buf */
    byte buf_ref[MUX_LOG_MAX_REV_PCTRL_SIZE];

  } var_len_buf; /* sizeof(var_len_buf) gives max length */

}
mux_log_rev_pctrl_type;

/* Threshold to determine when the buf is full and the log should be sent.
*/
#define MUX_LOG_REV_PCTRL_BUF_THRESHOLD \
  (sizeof(mux_log_rev_pctrl_type) - \
    (FPOS(mux_log_rev_pctrl_subrecord_type, var_len_buf) + \
     sizeof(mux_log_rev_pctrl_frame_type)))

/* FTM Log Type
*/
/* LOG_FTM_C */
typedef PACK(struct)
{
  log_hdr_type hdr;

  dword fch_total_frames;       /* total count FCH frames */
  dword fch_fer;                /* total FCH frame error count */
  dword sch_total_frames;       /* total count of SCH frames */
  dword sch_fer;                /* total SCH frame error count */
  dword sch_num_dtx;            /* total SCH DTX count */
  /* Support FEATURE_FTM_DCCH */
  dword dcch_total_frames;      /* total count DCCH frames */
  dword dcch_fer;               /* total DCCH frame error count */
  dword dcch_num_dtx;           /* total DCCH DTX count */
  /* End of FEATURE_FTM_DCCH */
}
mux_log_ftm_type;

/*---------------------------------------------------------------------------
                 1X POWER INFORMATION LOG TYPE
---------------------------------------------------------------------------*/
/* LOG_1X_PHYSICAL_LAYER_POWER_INFO_C */
#define MUX_1X_PWR_LOG_VERSION 0x00 /* Initial Version */

typedef PACK(struct)
{
  log_hdr_type  hdr;
  uint8   log_pkt_version;
  uint8   padding[7];         /* Ensure byte alignment of ustmr */
  uint64  ustmr;              /* Time when first sample in packet was collected */
  uint8   number_of_fingers;  /* 0-8 */ 
  uint8   aset_size;          /* 0-6 */ 
  uint8   rxd;                /* TRUE if RxD is enabled during the 20ms duration. */
  uint8   txpwr;              /* Latest non-zero Tx power used, in 0.5dBm steps. Max Tx power is 24dBm. */
  uint8   num_drx;            /* Number of DRX PCGs (1.25ms) during the 20ms duration.	*/
  uint8   fl_smartblanked;    /* TRUE if FL smart blanking is used in this frame (20ms)  */
  uint8   dtx_gating;         /* 0 (no DTX), 25% or 50% gating. */
  uint8   rl_smartblanked;    /* TRUE if RL smart blanking is used in this frame (20ms) */
  uint8   fl_early_terminated;/* Number of early terminated PCGs during the 20ms duration.*/
  uint8   rl_early_terminated;/* Number of early terminated PCGs during the 20ms duration.*/
} mux_1x_pwr_info_type;

#ifdef FEATURE_IS2000_1X_ADV
/*---------------------------------------------------------------------------
                 1X ADVANCED INFORMATION LOG TYPE
---------------------------------------------------------------------------*/
/* LOG_1X_ADV_L1_PERF_C  */

#define MUX_1XA_LOG_VERSION 0x02 /* Added Device only DTX parameters */
/* #define MUX_1XA_LOG_VERSION 0x01 - Added RCPM FFPC mode data */
/* #define MUX_1XA_LOG_VERSION 0x00 - Initial revision */

typedef struct
{
  log_hdr_type hdr;
  uint8   version;
  uint8   band_class;
  uint16  channel;
  uint16  frame_num_with_offset;
  uint8   rev_gain_adj_acs1;
  uint8   rev_gain_adj_acs2plus;

  uint8   smart_blanking;
    /*-------------------------------------
      [7:6] - RESERVED

      [5:4] - FL Duty Cycle
      0 = Disabled, 1 = 1 of 4, 2 = 1 of 8

      [3:2] - RL Duty Cycle
      0 = Disabled, 1 = 1 of 4, 2 = 1 of 8

      [1] - Guaranteed Frame Flag
      0 = Not Guaranteed, 1 = Guaranteed

      [0] - Blanked Frame Flag
      0 = Not Blanked, 1 = Blanked
    --------------------------------------*/

  uint8   power_control;
    /*-------------------------------------
      [7] - RCPM PC Mode
      0 = 400Hz, 1 = 200Hz

      [6:4] - RCPM PC Step Size
      0 = 1dB, 1 = .5dB, 2 = .25dB, 3 = 1.5dB, 4 = 2dB

      [3] - Current PC Mode
      0 = 400Hz, 1 = 200Hz

      [2:0] - Current PC Step Size
      0 = 1dB, 1 = .5dB, 2 = .25dB, 3 = 1.5dB, 4 = 2dB
    --------------------------------------*/

  uint16  frame_data;
    /*-------------------------------------
      [15] - Tx Enabled
      0 = False, 1 = True

      [14] - R-SCH
      0 = Disabled, 1 = Enabled

      [13] - F-SCH
      0 = Disabled, 1 = Enabled

      [12:11] - N2m value for Forward Link RC11
      0 = 2 Frames, 1 = 4 Frames, 2 = 6 Frames, 3 = 8 Frames

      [10:7] - Early Decode PCG
      Range = 0-15

      [6:4] - FL Frame Rate
      0 = Blank, 1 = 1/8, 2 = 1/4, 3 = 1/2, 4 = Full

      [3:1] - RL Frame Rate
      0 = Blank, 1 = 1/8, 2 = 1/4, 3 = 1/2, 4 = Full

      [0] - FFPC Setpoint Freeze Flag
      0 = Not Frozen, 1 = Frozen
    --------------------------------------*/

  uint16  ack_masks[6];
  /*-------------------------------------
      [5] - Fwd FCH ACK Mask for No RL Blanking
      [4] - Fwd FCH ACK Mask for No RL Blanking
      [3] - Rev FCH ACK Mask
      [2] - Fwd SCH ACK Mask for RL Blanking
      [1] - Fwd SCH ACK Mask for No RL Blanking
      [0] - Rev SCH ACK Mask
    --------------------------------------*/

  uint32  mdsp_buff[192];
  /*-------------------------------------
    Consists of 32 groups of 6 uint32 entries.
    One group represents 1 half PCG.
    The break down of the 6 uint32 entries is shown below.

    Uint32 Index 0:
    *   [8:0] - 4 chip count
    *   [12:9] - PCG count
    *   [15:13] - RESERVED
    *   [31:16] - Tx Total Power (-1/640 dB Units)

    Uint32 Index 1:
    *   [15:0] - R-ACKCH0 Digital Gain
    *   [31:16] - R-PICH Digital Gain

    Uint32 Index 2:
    *   [15:0] - R-SCH Digital Gain
    *   [31:16] - R-FCH Digital Gain

    Uint32 Index 3:
    *   [15:0] - Tx BetaP Gain (-1/640 dB Units)
    *   [31:16] - Tx Open Loop Adjust (-1/640 dB Units)

    Uint32 Index 4:
    *   [15:0] - Tx Rate Adjust (-1/640 dB Units)
    *   [31:16] - Tx Gain Adjust (-1/512 dB Units)

    Uint32 Index 5:
    *   [15:0] - RESERVED
    *   [20-16] - RC11 Rate
    *   [31-21]  - RESERVED

    <repeat for next group of 6 uint32 entries>

    *NOTE*
    R-ACKCH0 Digital Gain, R-PICH Digital Gain,
    R-SCH Digital Gain, and R-FCH Digital Gain values
    are unit-less and are best displayed in ratios to
    Pilot digital gain in dB.

    Example Conversion:

    R-FCH/R-PICH in dB =
    20 * log10(R-FCH Digital Gain/R-PICH Digital Gain)
    --------------------------------------*/

    uint16 ffpc_data;
    /*-------------------------------------
      New logged parameters added in log version 1.

      [15:8] - RESERVED

      [7:4] - Current FFPC Mode in use
      0 = 800Hz,     1 = 400/400Hz, 2 = 200/600Hz,
      3 = Pri EIB,   4 = Pri QIB,   5 = QIB/EIB,
      6 = 400Hz/EIB, 7 = 1xA 400Hz, 8 = 1xA 200Hz,
      9 = 1xA 200/200Hz

      [3:0] - RCPM FFPC Mode
      0 = 800Hz,     1 = 400/400Hz, 2 = 200/600Hz,
      3 = Pri EIB,   4 = Pri QIB,   5 = QIB/EIB,
      6 = 400Hz/EIB, 7 = 1xA 400Hz, 8 = 1xA 200Hz,
      9 = 1xA 200/200Hz
    --------------------------------------*/

    uint16  device_only_dtx_params;
    /*-------------------------------------
      [15:5]  - Reserved

      [4:3]   - SCH State
      0 = OFF, 1 = ON, 2 = ACTIVE

      [2]     - TX State
      0 = OFF, 1 = ON

      [1]     - Critical Frame Flag
      0 = Not Critical, 1 = Critical

      [0]     - Device only DTX Frame Flag
      0 = Not DTX, 1 = DTX
    --------------------------------------*/
}
mux_1x_adv_info_type;

/*---------------------------------------------------------------------------
                 1X ADVANCED INFORMATION LOG TYPE
---------------------------------------------------------------------------*/
/* 2.1 LOG_1X_ADV_FET_STATS_C */
#define MUX_1XA_FET_STATS_VERSION 0x00
typedef struct
{
  log_hdr_type hdr;
  uint8   version;
  uint8   _reserved_;
  uint16  fwd_link_ack_mask;
  uint16  rev_link_ack_mask;

  uint8   fwd_link_ack_stats[50];
    /*-------------------------------------
      Each byte contains 1 frame's worth of data:
      BITS:
      7:4 = PCG FL Ack was received on
      3:1 = Transmitted frame rate
      0 = DTX frame indication
    --------------------------------------*/

  uint8   rev_link_ack_stats[50];
    /*-------------------------------------
      Each byte contains 1 frame's worth of data:
      BITS:
      7:4 = PCG RL Ack was transmitted on
      3:1 = Received frame rate
      0 = Guaranteed frame indication
    --------------------------------------*/

}
mux_1x_adv_fet_stats_type;
#endif /* FEATURE_IS2000_1X_ADV */

/***********************************************
          EXTERNAL DECLARATIONS
***********************************************/

/* Accumulation buffer */
extern mux_log_rev_pctrl_type txc_log_rpctrl_buf;
/* Sub-record pointer */
extern mux_log_rev_pctrl_subrecord_type *txc_log_rpctrl_sr_ptr;

#define TXC_RPCTRL_LOG_OFFSET \
(txc_log_rpctrl_buf.hdr.len - FPOS(mux_log_rev_pctrl_type, var_len_buf))

/* Reverse frame types log accumulation buffer */
/* Accumulation buffer */
extern mux_log_rev_frame_types_type           txc_log_frm_types_buf;

/* R-SCH0 frame type */
extern mux_log_sch_rate_enum_type txc_sch0_frame_rate;

/* Sub-record pointer */
extern mux_log_rev_frame_types_subrecord_type *txc_log_frm_types_sr_ptr;

#define TXC_FRM_TYPES_LOG_OFFSET \
(txc_log_frm_types_buf.hdr.len - FPOS(mux_log_rev_frame_types_type, var_len_buf))

#endif /* MUX_LOGTYPES_H */
