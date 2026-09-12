#ifndef TDSDECHS_H
#define TDSDECHS_H

/*==========================================================================
             TD-SCDMA L1 HSDPA configuration header file

DESCRIPTION
  This file contains definition, declaration required for HSDPA configuration
  and maintainance code.

  Copyright (c) 2010 - 2011 by Qualcomm Technologies Incorporated.
  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdsdechs.h#1 $
$DateTime: 2019/08/19 10:51:23 $
$Author: pwbldsvc $

when       who    what, where, why
--------   ---    ----------------------------------------------------------
01/12/11   owen     cleanup and new TDS RRC/L1 IF
05/09/11   owen     cleanup more for logging
===========================================================================*/

#include "tdscdma_variation.h"
#include "tdsuecomdef.h"
#include "customer.h"
#include "tfw_sw_intf_msg.h"


/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */
//#include "tdsdec.h"
#include "tdshslog.h"
/* ---------------------------- */
/* Check for necessary features */
/* ---------------------------- */

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */
/* for Open-Loop CQI */
#define TDS_NON_PDSCH_TRX_SUBFRAMES_K  40      /* for reset SE, se_filter_reset_threshold*/
#define TDS_PDSCH_SIR_MIN              -10    /* in dbm */
#define TDS_PDSCH_SIR_MAX              10     /* in dbm */
#define TDS_PDSCH_BLER_TARGET          10     /*  actual value * 100 , 10% in real*/
#define TDS_PDSCH_SIR_ADJ_SSIZE        50     /*  actual value * 100,  0.5dbm in real  */
#define TDS_NON_PDSCH_TRX_SUBFRAMES_M  10     /* reset_bler_threshold_m */

/* is not used in initial release */
#define TDS_PDSCH_SIR_ADJ_K1           0.125  /* 0.125dbm */
#define TDS_PDSCH_SIR_ADJ_K2           0.5    /* 0.5dmb */
#define TDS_NON_PDSCH_TRX_SUBFRAMES_N  100    /* for reset BLER_calc, from Sys team, bler window size */

#define TDS_HSDPA_MIN                  0.001
/* does not change for TDS since it will not be included in TDS v1/v2 implementation
** marked by Owen on Aug 30, 2010
*/ 

/*! @brief Number of HS SCCH debug log buffer */
#define TDSDECHS_SCCH_DBG_LOG_NUM_BUF 2
  
/*! @brief Period for every statistics output */
#define TDS_HSDPA_DECODE_STAT_PERIOD 100 // 100 frames = 1s
  
/* HS decode status fields */
/* ----------------------- */
/*! @brief Number of HS decode status log packet buffers */
#define TDS_HS_DEC_STATUS_LOG_PACKET_BUF_SIZE 8

/* from sys team, to be optimized */
#define TDS_OL_CQI_WINDOW_MAX_SZ  100   
/* Number of HS decode status buffer */
#define TDSDECHS_DECODE_STATUS_NUM_BUF TDSHSLOG_DECODE_STATUS_LOG_MAX_SAMPLE

/* Various fields in mDSP decode status buffer for a sub-frame */

#define TDSDECHS_STATUS_SCCH_DEMOD_ATTEMPTED_BMSK 0x8000
#define TDSDECHS_STATUS_SCCH_DEMOD_ATTEMPTED_REG  0
#define TDSDECHS_STATUS_SCCH_DEMOD_ATTEMPTED_SHFT 15

#define TDSDECHS_STATUS_SCCH_DEMOD_VALID_BMSK 0x4000
#define TDSDECHS_STATUS_SCCH_DEMOD_VALID_REG  0
#define TDSDECHS_STATUS_SCCH_DEMOD_VALID_SHFT 14

#define TDSDECHS_STATUS_SCCH_DATA_BMSK 0x3FFF
#define TDSDECHS_STATUS_SCCH_DATA_REG  0
#define TDSDECHS_STATUS_SCCH_DATA_SHFT 0

#define TDSDECHS_STATUS_HARQ_ID_BMSK 0x0038
#define TDSDECHS_STATUS_HARQ_ID_REG  0
#define TDSDECHS_STATUS_HARQ_ID_SHFT 3

#define TDSDECHS_STATUS_WINNING_SCCH_CH_IDX_BMSK 0xC000
#define TDSDECHS_STATUS_WINNING_SCCH_CH_IDX_REG  1
#define TDSDECHS_STATUS_WINNING_SCCH_CH_IDX_SHFT 14

#define TDSDECHS_STATUS_DSCH_TB_SZ_BMSK 0x3FFF
#define TDSDECHS_STATUS_DSCH_TB_SZ_REG  1
#define TDSDECHS_STATUS_DSCH_TB_SZ_SHFT 0

#define TDSDECHS_STATUS_UNQUAN_CQI_BMSK 0xFFF0
#define TDSDECHS_STATUS_UNQUAN_CQI_REG  2
#define TDSDECHS_STATUS_UNQUAN_CQI_SHFT 4

#define TDSDECHS_STATUS_NEW_TX_BMSK 0x0002
#define TDSDECHS_STATUS_NEW_TX_REG  2
#define TDSDECHS_STATUS_NEW_TX_SHFT 1

#define TDSDECHS_STATUS_DSCH_STATUS_BMSK 0x0001
#define TDSDECHS_STATUS_DSCH_STATUS_REG  2
#define TDSDECHS_STATUS_DSCH_STATUS_SHFT 0

#define TDSDECHS_STATUS_DSCH_STATUS_NACK 0
#define TDSDECHS_STATUS_DSCH_STATUS_ACK  1
#define TDSDECHS_STATUS_DSCH_STATUS_AUTO_ACK  2

/* Macro to extract the bit field value from the HS RL status mask */
#define TDSDECHS_GET_RL_STATUS_BF_PARAM(buffer, type) \
  ((buffer & TDSDECHS_##type##_BMSK) >> TDSDECHS_##type##_SHFT)

#define TDSDECHS_SET_RXD_STATUS_IN_HS_RL_MASK()   TDSDECHS_SET_RL_STATUS_BF_PARAM(RXD_TRANS, tdsrxdiv_is_active())

/* This macro gets the value form mDSP decode status buffer (W16 array)
   for the given field name */
#define TDSDECHS_STATUS_FIELD_VAL(buffer, field) \
  (((buffer)[TDSDECHS_STATUS_##field##_REG] & TDSDECHS_STATUS_##field##_BMSK) >> \
   HS_DECODE_STATUS_##field##_SHFT)
   
/* For HS decode status TB data bit logging, sub frame for frame N-2
   are logged at sub frame N, so there is 3 frame buffer, it will have
   3x5= 15 sub frames. Keep data buffered for 16 sub frames indexed with
   subframe mod 16 */
#define TDSDECHS_DSCH_TB_DATA_BIT_BUF_LEN 16
  
/* Number of log packet buffer for MAC hs header logging */
#define TDSDECHS_NUM_MAC_HS_HDR_LOG_PKT_BUF 2

/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */

/* Call back function type declaration */
/* ----------------------------------- */

/* Enum type declaration */
/* --------------------- */

/* This enum defines various actions for HS stat generation */
typedef enum 
{
  TDSDECHS_STAT_GEN_NOOP,
  TDSDECHS_STAT_GEN_START                                                       ,
  TDSDECHS_STAT_GEN_RESTART,
  TDSDECHS_STAT_GEN_STOP                                                   
} tdsdechs_stat_gen_action_enum_type;

/* This enum defines the various actions for pessimistic CQI logging */
typedef enum
{
  TDSDECHS_LOG_PESS_CQI,
  TDSDECHS_CLEAR_PESS_CQI,
  TDSDECHS_FLUSH_PESS_CQI
}tdsdechs_pess_cqi_action_enum_type;

/* Structure type declaration */
/* -------------------------- */

/* This structure defines HS status that is required for HS downlink channels
   stat generation */
typedef struct
{
  /* Quantized CQI from n-3 frame where n is rest of info */
  //uint8 quan_cqi;
  /* indicate if SCCH demod was even attempted or not */
  //boolean scch_demod_attempted;
  /* TRUE indicate SCCH was demodulated and CRC pass FALSE otherwise */
  //boolean scch_valid;
  boolean valid;
  /* DSCH CRC pass/fail status */
  boolean dsch_status;
  /* New transmission of HS DSCH block */
  boolean new_tx;
  /* HARQ process Id primary*/
  uint8 harq_proc_id;
  /* transport block size of this DSCH block */
  uint16 tb_size;

} tdsdechs_status_struct_type;

/* This structure defines HS-DSCH HARQ process stats */
typedef struct
{
  /* indicate that ACK has been received for this flag. If ACK is received then
     this process Id is supposed to be done for stat generation. It is not considered
     again untill new transmission is received.
     Set to TRUE for initialization */
  boolean ack_received;
  /* number of NACKs received. Init it to 0 when new transmission is received */
  uint8 num_nack_rece;
} tdsdechs_dsch_harq_proc_stat_struct_type;

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */
/* for OL cqi */
extern uint32 tdsdechs_pdsch_crc_hist[TDS_OL_CQI_WINDOW_MAX_SZ];
extern uint16 tds_ol_cqi_window_curr_index;     /* stop index in the window */
extern uint16 tds_ol_cqi_window_size;

/* the sub-frame number for the last arring HS-PDSCH data   */
extern uint16 tds_last_hspdsch_arriving_sfn;
extern uint16 tds_last_new_data_arriving_sfn; 

/* HS-DSCH MAC-d PDU deciphering related variables */
/* Deciphering start address of current MAC-d PDU */
extern uint16 tdsdechs_tb_deciph_start_addr;
/* Deciphering start bit position at start address */
extern uint16 tdsdechs_deciph_start_bit_pos;
/* Length with in MAC-d PDU size to decipher. This length is one less
   than actual length */
extern uint16 tdsdechs_tb_deciph_len;
/* Offset of first bit to be deciphered of current MAC-d PDU from start of
   HS-DSCH block */
extern uint16 tdsdechs_blk_deciph_first_bit_offset;

//extern uint16  tdsdechs_sich_powctl_gap;
/* HS-DSCH MAC-d SDU data transfer related variables */

/* DOB start address to read data from */
extern uint16 tdsdechs_tb_bg_xfer_start_addr;
/* Starting offset to read first bit at DOB start address */
extern uint8  tdsdechs_bg_xfer_start_offset;
/* MAC-d SDU length */
extern uint16 tdsdechs_tb_bg_xfer_len;

#ifdef FEATURE_HSDPA_L1_STATS

/* This maintains running count of new transmissions received and
   that are in error */
extern uint32 tdsdechs_num_new_tx_rece;
extern uint32 tdsdechs_new_tx_in_err;

extern uint16 tdsdechs_rec_cfn;

extern uint16 scch_stats_index;
extern uint16 dechs_decode_status_index;

extern tdsdechs_status_struct_type tdsdechs_decode_staus[TDS_HS_DEC_STATUS_LOG_PACKET_BUF_SIZE];
extern boolean tdsdechs_scch_stats[TDS_HS_DEC_STATUS_LOG_PACKET_BUF_SIZE];
extern uint8 tdsdechs_scch_stats_extra_info[TDS_HS_DEC_STATUS_LOG_PACKET_BUF_SIZE];
#endif /* FEATURE_HSDPA_L1_STATS */

/* The next pessimistic CQI command */
extern tdsdechs_pess_cqi_action_enum_type tdsdechs_pess_cqi_log_next_sample;


/* HS-SCCH decoding status pointer in SW/FW shared memory */
tfw_scch_result_buf_t* mcaltds_sfw_hs_scch_dec_ptr;
/* =======================================================================
**                        Function Declarations
** ======================================================================= */

/*===========================================================================
FUNCTION tdsdechs_init

DESCRIPTION
  This function initialize various variables in this module. It is intended
  to be called at L1 stack startup along with other module init.
  
DEPENDENCIES
  None
  
RETURN VALUE
  None

SIDE EFFECTS
  Variables get inited to their initial values
===========================================================================*/

extern void tdsdechs_init(void);

/*===========================================================================
FUNCTION tdsdechs_rxd_transition_callback

DESCRIPTION
  This is the callback function registered with RxD module. 
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void tdsdechs_rxd_transition_callback(
  /* State of the callback, TRUE if RxD transition has started */
  boolean transition);

/*===========================================================================
FUNCTION tdsdechs_srch_td_update_callback

DESCRIPTION
  This is the callback function registered with SRCH module. 
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void tdsdechs_srch_td_update_callback(void);


#ifdef FEATURE_HSDPA_L1_STATS

/* -------------------------------------------------------- */
/* Following functions handle HS statistics and its logging */
/* -------------------------------------------------------- */

/*===========================================================================
FUNCTION tdsdechs_stat_init

DESCRIPTION
  This function initializes the variables required for generation and
  accumulation of HS decode status and stats. It initialize the following
  variables
  
  tdsdechs_stat_accum_active is set to FALSE to indicate inactive
  tdsdechs_stats_gen_action is set to NOOP
  All HSDPA status log buffers are set to available
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  All above mentioned variables in descriptions are inited
===========================================================================*/

extern void tdsdechs_stat_init(void);

/*===========================================================================
FUNCTION tdsdechs_get_stat_action

DESCRIPTION
  This function returns the action that HS logging state machine needs
  to perform next.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern tdsdechs_stat_gen_action_enum_type tdsdechs_get_stat_action(void);

/*===========================================================================
FUNCTION tdsdechs_stat_do_action

DESCRIPTION
  This function changes HSDPA decode status and stat generation mode. Before
  it commit action provided function argument, it validates if that action
  can be accepted. Current action should be always NOOP that is reset at
  start and after every action completion
  
  When stat generation is inactive, only action that is accepted is START.
  When stat generation is active it can be RESTARTED or STOPPED
  
  There are 2 buffer for accumulating HS decode status. These are used when
  a existing accumulation need to be submitted for logging. It happens when
  accumulation is done or there is reconfiguration and previous configuration
  need to be flushed.
  
DEPENDENCIES
  None

RETURN VALUE
  TRUE or FALSE based on action accept validation

SIDE EFFECTS
  None
===========================================================================*/

extern boolean tdsdechs_stat_do_action(
  /* Action for HS decode statu accumulation and stat generation */
  tdsdechs_stat_gen_action_enum_type action,
  /* start CFN to start or restart accumulation/stat generation */
  uint16 start_cfn,
  /* mDSP info table index from where to read the information */
  uint8  info_table_index);
/*===========================================================================
FUNCTION tdsdechs_update_stats

DESCRIPTION
  This function is called periodically every frame to act on new action
  information, if set and do periodic decode status accumulation and stat
  generation.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void tdsdechs_update_stats(void);

/*===========================================================================
FUNCTION tdsdechs_pess_cqi_submit_log_buffer

DESCRIPTION
  This function is called to post HS pessimistic CQI submit command. If the 
  buffer is not ready to be submitted to diag the function returns without
  doing anything. This can so happen because multiple logs are instructed
  to be submitted to diag by the same L1 command.
  It checks for the log code in diag and submits the packet.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Flag indicating the log buffer is submitted for flushing is reset.
===========================================================================*/

extern void tdsdechs_pess_cqi_submit_log_buffer(void);

/*===========================================================================
FUNCTION tdsdechs_status_send_log_submit_cmd

DESCRIPTION
  This function is called to post HS log submit command. There is only one HS
  log submit command. HS cfg module calls all accumlated logging entities. Each
  entity is reponsible for maintaining log submit pending information. It keeps
  this information for HS decode status log submit pending information
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void tdsdechs_status_send_log_submit_cmd(void);

/*===========================================================================
FUNCTION tdsdechs_status_submit_log_buffer

DESCRIPTION
  This function is called from hscfg module in response of HS log submit
  command. It checks for hs decode status log submit flag. It is set to TRUE,
  it submit log buffer pointed by lof buffer index to submit.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void tdsdechs_status_submit_log_buffer(void);

/*===========================================================================
FUNCTION tdsdechs_quantize_cqi

DESCRIPTION
  This function quantize CQI from lowest value of 1 to max value of 30.
  Unquantized value is 12 bit number in 5.7 format.
  
DEPENDENCIES
  None

RETURN VALUE
  5 Bit unquantized value form 1 to 30

SIDE EFFECTS
  None
===========================================================================*/

extern uint8 tdsdechs_quantize_cqi(
  /* Unquantized CQI value */
  uint16 unquan_cqi);

/*===========================================================================
FUNCTION tdsdechs_init_stats

DESCRIPTION
  This function initialize stats information.
  
  It is called when ever HS stat generation is started at START or RESTART
  action OR
  when ever stats have been generated using max required number of samples
  
DEPENDENCIES
  None

RETURN VALUE
  5 Bit unquantized value form 1 to 30

SIDE EFFECTS
  None
===========================================================================*/

extern void tdsdechs_init_stats(void);

/*===========================================================================
FUNCTION tdsdechs_gen_stats

DESCRIPTION
  This function generate various stats for HS downlink channels.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void tdsdechs_gen_stats(
  /* HS decode status structure pointer */
  tdsdechs_status_struct_type *decode_status,
  boolean scch_valid, uint8 scch_invalid);

/*===========================================================================
FUNCTION tdsdechs_submit_stats

DESCRIPTION
  This function submits the HS downlink channel stats to logging. This is
  done when HS stat gen/accumulation action is STOP or RESTART or when stats
  have been generated using max number of samples.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void tdsdechs_submit_stats(void);

#endif /* FEATURE_HSDPA_L1_STATS */

/*===========================================================================
FUNCTION tdsdechs_scch_decoding_status_logging

DESCRIPTION
  This function is called from mcal_hscfg module for HS-SCCH decoding status
  logging
  
DEPENDENCIES
  FW/SW shared memory

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void tdsdechs_decoding_status_logging(
    tdshslog_decode_status_log_pkt_type* decode_status_pkt);

/*===========================================================================
FUNCTION tdsdechs_mac_hs_hdr_send_log_submit_cmd

DESCRIPTION
  This function is called to post MAC-hs header log submit command. There is
  only one HS log submit command. HS cfg module calls all accumlated logging
  entities. Each entity is reponsible for maintaining log submit pending
  information. It keeps this information for HS decode status log submit
  pending information
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void tdsdechs_mac_hs_hdr_send_log_submit_cmd(
  /* HS decode status log buffer index to submit */
  uint8 mac_hs_hdr_info_buf_idx);

/*===========================================================================
FUNCTION tdsdechs_mac_hs_hdr_submit_log_buffer

DESCRIPTION
  This function is called from hscfg module in response of HS log submit
  command. It checks for MAC-hs header log submit flag. It is set to TRUE,
  it submit log buffer pointed by log buffer index to submit.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void tdsdechs_mac_hs_hdr_submit_log_buffer(void);

/*===========================================================================
FUNCTION tdsdechs_flush_mac_hs_log_pkt

DESCRIPTION
  This function is called from hscfg module to flush MAC hs header log packet
  if any accumulation is pending. It is called when there is switch in DL
  HS config at any reconfig or stop action.
  Note that variables accessed by this functions are shared with function
  that reads TB header read. However both functions are event on Rx timeline
  so there for they are mutually exclusive.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void tdsdechs_flush_mac_hs_log_pkt(void);

/*===========================================================================
FUNCTION tdsdechs_reset_status()

DESCRIPTION
  This function reset HS DEC status: hs-scch crc statistics, hs-pdsch crc statistics,
  hs-pdsch OL cqi
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void tdsdechs_reset_status(void);

/*===========================================================================
FUNCTION tdsdechs_set_last_pdsch_arring_sfn(uint16)

DESCRIPTION
  This function is updating the subframe num of last arring HS-PDSCH
  
DEPENDENCIES
  
RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void tdsdechs_set_last_pdsch_arring_sfn(uint16 sub_fn);

/*===========================================================================
FUNCTION tdsdechs_set_last_new_data_arring_sfn(uint16)

DESCRIPTION
  This function is updating the subframe num of last arring HS-PDSCH
  
DEPENDENCIES
  
RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void tdsdechs_set_last_new_data_arring_sfn(uint16 sub_fn);

/*===========================================================================
FUNCTION tdsdechs_readout_olcqi_nv()

DESCRIPTION
  This function read out olcqi parameters from NV items
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void tdsdechs_readout_olcqi_nv(void);

/*===========================================================================
FUNCTION tdsdechs_olcqi_nv_validate()

DESCRIPTION
  This function validate olcqi parameters reading out from NV items
  
DEPENDENCIES
  None

RETURN VALUE
  TRUE or FALSE

SIDE EFFECTS
  None
===========================================================================*/
extern boolean tdsdechs_olcqi_nv_validate(void);


#endif /* TDSDECHS_H */	
