#ifndef DIAG_TDS_CMD_H
#define DIAG_TDS_CMD_H
/*==========================================================================

      Diagnostic Services Packet Processing Command Code Defintions

Description
  This file contains packet id definitions for the serial interface to
  the dmss.  

Copyright (c) 1993-2010 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                            Edit History
                            
$Header: //components/rel/tdscdma.mpss/4.3/diag/inc/tdsdiag.h#1 $
$DateTime: 2019/08/19 10:51:23 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---    -----------------------------------------------------------
03/23/12   mkh    Enabled Diag Commands for TDSCDMA 
===========================================================================*/

#include "tdscdma_variation.h"
#include "diagpkt.h"
#include "customer.h"
#include "rfm.h"

/* Wrap the file with this feature to help non-TDSCDMA builds. */
#ifdef FEATURE_TDSCDMA

#include "tdssrchzz.h"
#include "tdsenc.h"   /* renamed enc.h to tdsenc.h */
#include "mm.h"
#ifdef FEATURE_TDSCDMA_PLT
#include "tpltrpccmd.h"   //zhengming
#endif

/* Max num of digits in dialed number for TDSCDMA call origination packet */
#define DIAG_MAX_TDSCDMA_DIGITS 32

#define TDSCDMA_DIAG_VERNO_F                 0
#define TDSCDMA_DIAG_RRC_GET_STATE_F         1
#define TDSCDMA_DIAG_BLER_RATE_CHANGE_F      8
#define TDSCDMA_DIAG_ORIG_F                 12
#define TDSCDMA_DIAG_END_F                  13
#define TDSCDMA_STATUS_F                    14
#define TDSCDMA_ADDITIONAL_STATUS_F         15
#define TDSCDMA_MANUAL_CQI_UPD_CONFIG_F     21
#define TDSCDMA_RLC_FLOW_CONTROL_CONFIG_F   23
#define TDSCDMA_DIAG_EUL_UL_VAR_UPD_F       24
#define TDSCDMA_DIAG_TDSCDMA_STATS_F        26
#define TDSCDMA_DIAG_TDSCDMA_CLEAR_STATS_F  27
#define TDSCDMA_DEC_TBSET_DUMP_F            30
#define TDSCDMA_CQI_OVERIDE_PARAMS_F        35
#define TDSCDMA_RXD_ENABLE_DSABLE_PARAMS_F  45

/*zhengming: TPLT commands*/
#define TDSCDMA_PLT_COMMAND_F    40
#define TDSCDMA_PLT_RESULT_F    41

#define TDSCDMA_PLT_MODE_F        36

#define TDSCDMA_DIAG_IMSI_SIZE              (sizeof(imsi_data_T))

/*zhengming: 
PLT START TDSCDMA MODE 
*/

#ifdef FEATURE_TDSCDMA_PLT
DIAGPKT_SUBSYS_REQ_DEFINE(TDSCDMA, TDSCDMA_PLT_COMMAND_F)
//uint32 diag_routing_header;  //routing header
uint32 cmd_id;                 //what is this for?
tplt_store_cmd_param_union_type params; 
DIAGPKT_REQ_END

DIAGPKT_SUBSYS_RSP_DEFINE(TDSCDMA, TDSCDMA_PLT_COMMAND_F)
 uint32         tplt_status;
DIAGPKT_RSP_END

DIAGPKT_SUBSYS_REQ_DEFINE(TDSCDMA, TDSCDMA_PLT_RESULT_F)
 uint32 cmd_id;                 
DIAGPKT_REQ_END

DIAGPKT_SUBSYS_RSP_DEFINE(TDSCDMA, TDSCDMA_PLT_RESULT_F)
  uint32 cmd_id; 
  tplt_result_param_union_type rsp_result; 
DIAGPKT_RSP_END


typedef struct {
  byte cmd_code;
  byte subsys_id;
  uint16 subsys_cmd_code;
  uint32 tplt_status;
  uint16 delayed_rsp_id;
  uint16 response_cnt;
} DIAG_SUBSYS_TDSCDMA_TDSCDMA_PLT_COMMAND_F_rsp_v2_type;

#endif

#ifdef FEATURE_TDSCDMA_PLT
DIAGPKT_SUBSYS_REQ_DEFINE(TDSCDMA, TDSCDMA_PLT_MODE_F)
 /* RxD state */
 uint8 tplt_mode;
DIAGPKT_REQ_END

 /* Response packet */
DIAGPKT_SUBSYS_RSP_DEFINE(TDSCDMA, TDSCDMA_PLT_MODE_F)
   uint8  status;
DIAGPKT_RSP_END
#endif


/* TDSCDMA Diag RRC Get State
 */

DIAGPKT_SUBSYS_REQ_DEFINE(TDSCDMA, TDSCDMA_DIAG_RRC_GET_STATE_F)
DIAGPKT_REQ_END

DIAGPKT_SUBSYS_RSP_DEFINE(TDSCDMA, TDSCDMA_DIAG_RRC_GET_STATE_F)
 uint8                      rrc_curr_state; /* rrc current state */
DIAGPKT_RSP_END

/* Paging statistics structure. */
typedef PACKED struct{
  uint32 rrcpg1_num_pages;
  uint32 rrcpg1_num_pages_matched;
  uint32 drx_pich_demod_error;
  uint32 drx_cycle_len;
  uint32 num_drx_cycles;
} page_stats_type;

/* Data Structure to store the intra frequency reselection statistics */
typedef PACKED struct
{
  /* Intra frequency reselection success count since last power up */
  uint32 intra_freq_resel_success_cnt;
  /* Intra frequency reselection failure count due to N-BCCH set up failue since last power up */
  uint32 intra_freq_nbcch_setup_failure_cnt;
  /* Intra frequency reselection failure count due to SIB wait time out since last power up */
  uint32 intra_freq_sib_wait_timeout_cnt;
  /* Intra frequency reselection failure count due to transition failures since last power up */
  uint32 intra_freq_transition_fail_cnt;
  /* Intra frequency reselection failure count due to other causes since last power up */
  uint32 intra_freq_failed_other_causes_cnt;
}intra_freq_resel_stats_type;

/* Data Structure to store the inter frequency reselection statistics */
typedef PACKED struct
{
  /* Inter frequency reselection success count since last power up */
  uint32 inter_freq_resel_success_cnt;
  /* Inter frequency reselection failure count due to N-BCCH set up failue since last power up */
  uint32 inter_freq_nbcch_setup_failure_cnt;
  /* Inter frequency reselection failure count due to SIB wait time out since last power up */
  uint32 inter_freq_sib_wait_timeout_cnt;
  /* Inter frequency reselection failure count due to transition failures since last power up */
  uint32 inter_freq_transition_fail_cnt;
  /* Inter frequency reselection failure count due to other causes since last power up */
  uint32 inter_freq_failed_other_causes_cnt;
}inter_freq_resel_stats_type;

/* Data Structure to store the TTOG reselection statistics */
typedef PACKED struct
{
  /* TTOG cell reselection count since last power up */
  uint32 ttog_resel_cnt;
  /* TTOG cell reselection failure count since last power up */
  uint32 ttog_resel_success_cnt;
  /* TTOG cell reselection failure count since last power up */
  uint32 ttog_resel_failure_bch_reading_fail_cnt;
  /* TTOG cell reselection failure count since last power up */
  uint32 ttog_resel_failure_acq_fail_cnt;
  /* TTOG cell reselection failure count since last power up */
  uint32 ttog_resel_failure_sib_timeout_cnt;
  /* TTOG cell reselection failure count since last power up */
  uint32 ttog_resel_failure_other_cause_cnt;
}ttog_resel_stats_type;

/* Data Structure to store the OOS related statistics */
typedef PACKED struct
{
  /* Number of times OOS is triggered in idle mode since last power up */
  uint32 oos_idle_cnt;
    /* Number of times OOS is triggered in connected mode since last power up */
  uint32 oos_connected_cell_dch_cnt;
      /* Number of times OOS is triggered in connected mode since last power up */
  uint32 oos_connected_cell_fach_cnt;
        /* Number of times OOS is triggered in connected mode since last power up */
  uint32 oos_connected_cell_pch_cnt;
        
}oos_stats_type;


/* Data Structure to store the TDTOL reselection statistics */
typedef PACKED struct
{
  /* TDTOL cell reselection count since last power up */
  uint32 ttol_resel_cnt;
  /* TDTOL cell reselection failure count since last power up */
  uint32 ttol_resel_success_cnt;
  /* TDTOL cell reselection failure count since last power up */
  uint32 ttol_resel_failure_bch_reading_fail_cnt;
  /* TDTOL cell reselection failure count since last power up */
  uint32 ttol_resel_failure_acq_fail_cnt;
  /* TDTOL cell reselection failure count since last power up */
  uint32 ttol_resel_failure_sib_timeout_cnt;
  /* TDTOL cell reselection failure count since last power up */
  uint32 ttol_resel_failure_other_cause_cnt;
}ttol_resel_stats_type;

/* Data Structure to store the GTOTD reselection statistics */
typedef PACKED struct
{
  /* GTOTD cell reselection count since last power up */
  uint32 gtot_resel_cnt;
  /* GTOTD cell reselection failure count since last power up */
  uint32 gtot_resel_success_cnt;
  /* GTOTD cell reselection failure count since last power up */
  uint32 gtot_resel_failure_bch_reading_fail_cnt;
  /* GTOTD cell reselection failure count since last power up */
  uint32 gtot_resel_failure_acq_fail_cnt;
  /* GTOTD cell reselection failure count since last power up */
  uint32 gtot_resel_failure_sib_timeout_cnt;
  /* GTOTD cell reselection failure count since last power up */
  uint32 gtot_resel_failure_other_cause_cnt;
}gtot_resel_stats_type;

/* Data Structure to store the LTOTD reselection statistics */
typedef PACKED struct
{
  /* LTOTD cell reselection count since last power up */
  uint32 ltot_resel_cnt;
  /* LTOTD cell reselection failure count since last power up */
  uint32 ltot_resel_failure_cnt;  
}ltot_resel_stats_type;

/* Data Structure to store the acq  statistics */
typedef PACKED struct
{
    uint32 td_acq_attemp_cnt;
    uint32 td_acq_attemp_succeed_cnt;
    uint32 td_acq_attemp_failed_cnt;
}acq_attemp_stats_type;

typedef PACKED struct
{
    uint32 td2td_bplmn_cnt;
    uint32 td2g_bplmn_cnt;
    uint32 td2l_bplmn_cnt;
    uint32 l2td_bplmn_cnt;
    uint32 g2td_bplmn_cnt;
}bplmn_stats_type;

typedef PACKED struct
{
    uint32 fast_return_triggered_cnt;
    uint32 fast_return_succeed_cnt;
}fast_return_stats_type;

typedef PACKED struct
{
    uint32 rce_attemp_cnt;  
    uint32 rce_succeed_cnt;    
    uint32 rce_failed_cnt;
}rrc_connetion_setup_stats_type;

/* Data Structure to store RACH Transmission RRC Retry related statistics */
typedef PACKED struct
{
  /*Count which captures number of times RRC is successful in receiving RRC 
    procedure level response message from N/W for any RRC message sent on RACH. */
  uint32 succful_rrc_tran_retry_cnt ;
  /*Count which captures number of times RRC Retry Failed due L2 ACK IND 
    with cause failure in sending RRC message on RACH transport channel */
  uint32 rrc_retry_fail_due_to_l2_ack_ind;
  /*Counter is incremented when RRC exhausts with max time re-transmission
    before receiving RRC procedure level response message on RACH */
  uint32 rrc_retry_fail_due_to_others;
}rach_transmission_stats_type;

/*Systems dash board structure definitions */
/* Sarao: Data Structure for L2 RACH stats */
typedef PACKED struct
{
  /* Count of random access procedure success since last power up */
  uint32 rach_success_cnt;
  /* Count of random access procedure failure due to RACH transmission was aborted by L1 */
  uint32 rach_failure_due_to_abort_cnt;
  /* Count of random access procedure failure due to Max RACH Attempts */   
  uint32 rach_failure_due_to_max_rach_attempts_cnt;
  /* Count of random access procedure failure due to other reasons */
  uint32 rach_failure_due_to_other_reasons_cnt;
}tdscdma_l2_rach_stats_type;




/************************************************************************** 
    PACKET DEFINITIONS
**************************************************************************/

/* TDSCDMA version  Diag Request Packet */

DIAGPKT_SUBSYS_REQ_DEFINE(TDSCDMA, TDSCDMA_DIAG_VERNO_F)
DIAGPKT_REQ_END

 /* The response packet is defined below. */
DIAGPKT_SUBSYS_RSP_DEFINE(TDSCDMA, TDSCDMA_DIAG_VERNO_F)
 /* command code */
//-LD  uint16  cmd_code;
  /* TDSCDMA class mark */
  //uint8 class_mark;
  /* Rev number of TDSCDMA standard  4 MSBs are used to set
     to represent year of release 0 - 1999  1 - 2000 etc
     4 LSBs will be set to the month */
  //uint8 g3gpp_rev;
  /* Release num, from 3 up */
  //uint8 g3gpp_rel;
  /* Vocoder DSP version num, 0 ...0xffff */
  uint16  vdsp_ver;
  /* MDSP version number(first part) 16 bits */
  uint16  mdsp_ver_rom;
  /* MDSP version number(second part) 16 bits */
  uint16  mdsp_ver_ram;
  /* Indicates if DRX is used for cell updating */
  //uint8 drx_index;
  /* MSM version ( first part) */
  //uint8 msm_ver_maj;
  /* MSM version ( second part) */
  //uint8 msm_ver_min;
  //uint8 model_num;
  /* TDSCDMA model number */
DIAGPKT_RSP_END


/* TDSCDMA Diag L1 BLER meas rate change packet
 */

DIAGPKT_SUBSYS_REQ_DEFINE(TDSCDMA, TDSCDMA_DIAG_BLER_RATE_CHANGE_F)
 /* BLER meas duration in ms */
 uint16 bler_meas_duration_ms;
DIAGPKT_REQ_END

DIAGPKT_SUBSYS_RSP_DEFINE(TDSCDMA, TDSCDMA_DIAG_BLER_RATE_CHANGE_F)
DIAGPKT_RSP_END


DIAGPKT_SUBSYS_REQ_DEFINE(TDSCDMA, TDSCDMA_RXD_ENABLE_DSABLE_PARAMS_F)
 /* RxD state */
 uint8 rxd_state;
DIAGPKT_REQ_END

 /* Response packet */
DIAGPKT_SUBSYS_RSP_DEFINE(TDSCDMA, TDSCDMA_RXD_ENABLE_DSABLE_PARAMS_F)
   uint8  status;
DIAGPKT_RSP_END

/* TDSCDMA Diag Call Origination Request Packet */

DIAGPKT_SUBSYS_REQ_DEFINE(TDSCDMA, TDSCDMA_DIAG_ORIG_F)
 /* DM shall set it to the number of valid digits in DIGITS field */
 uint8  num_digits;

 /* Dialed digits; ASCII chars; 0-9, #, * only */
 uint8  digits[DIAG_MAX_TDSCDMA_DIGITS];

 /* Rate of the vocoder: 0 ~ 7 for
    4.75, 5.15, 5.90, 6.70, 7.40, 7.95, 10.2, 12.2 kbps
  */
 uint8  amr_rate;
DIAGPKT_REQ_END

 /* The response packet is defined below. */
DIAGPKT_SUBSYS_RSP_DEFINE(TDSCDMA, TDSCDMA_DIAG_ORIG_F)
DIAGPKT_RSP_END

/* TDSCDMA Diag Call End Request Packet */

DIAGPKT_SUBSYS_REQ_DEFINE(TDSCDMA, TDSCDMA_DIAG_END_F)
DIAGPKT_REQ_END

 /* The response packet is defined below. */
DIAGPKT_SUBSYS_RSP_DEFINE(TDSCDMA, TDSCDMA_DIAG_END_F)
DIAGPKT_RSP_END

/*---------------------------------------------------------------------------
    TDSCDMA TMC STATUS
---------------------------------------------------------------------------*/
/* Request packet */
DIAGPKT_SUBSYS_REQ_DEFINE(TDSCDMA, TDSCDMA_STATUS_F) 
DIAGPKT_REQ_END

 /* Response packet */
DIAGPKT_SUBSYS_RSP_DEFINE(TDSCDMA, TDSCDMA_STATUS_F)
 /* TMC state */
 uint8 tmc_state;
 
DIAGPKT_RSP_END

/*---------------------------------------------------------------------------
    TDSCDMA ADDITIONAL STATUS
---------------------------------------------------------------------------*/
/* Request packet */
DIAGPKT_SUBSYS_REQ_DEFINE(TDSCDMA, TDSCDMA_ADDITIONAL_STATUS_F) 
DIAGPKT_REQ_END

 /* Response packet */
DIAGPKT_SUBSYS_RSP_DEFINE(TDSCDMA, TDSCDMA_ADDITIONAL_STATUS_F)
 /* International Mobile Equipment ID */
 uint8 imei[9];
 /* International Mobile Subscriber ID */
 uint8 imsi[TDSCDMA_DIAG_IMSI_SIZE];
 /*l1_state*/
 uint8 l1_state;
 
DIAGPKT_RSP_END



/*---------------------------------------------------------------------------
    TDSCDMA ADDITIONAL STATUS
---------------------------------------------------------------------------*/
/* Request packet */
DIAGPKT_SUBSYS_REQ_DEFINE(TDSCDMA, TDSCDMA_MANUAL_CQI_UPD_CONFIG_F)
  boolean manual_cqi_update_flag;
  int32   manual_cqi_update_offset;
DIAGPKT_REQ_END

 /* Response packet */
DIAGPKT_SUBSYS_RSP_DEFINE(TDSCDMA, TDSCDMA_MANUAL_CQI_UPD_CONFIG_F)
   uint8  status;
DIAGPKT_RSP_END


#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL
/*---------------------------------------------------------------------------
    TDSCDMA ADDITIONAL STATUS
---------------------------------------------------------------------------*/
/* Request packet */
DIAGPKT_SUBSYS_REQ_DEFINE(TDSCDMA, TDSCDMA_RLC_FLOW_CONTROL_CONFIG_F)
   uint16 ws_step_period;
   uint16 ws_step_stage;
   uint16 init_ws;
DIAGPKT_REQ_END

 /* Response packet */
DIAGPKT_SUBSYS_RSP_DEFINE(TDSCDMA, TDSCDMA_RLC_FLOW_CONTROL_CONFIG_F)
   uint8  status;
DIAGPKT_RSP_END
#endif /* FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL */

#ifdef FEATURE_TDSCDMA_HSUPA
/*---------------------------------------------------------------------------
    TDSCDMA EUL UPLINK SUBSYSTEM VARIABLE UPDATE
---------------------------------------------------------------------------*/
/* Request packet */
DIAGPKT_SUBSYS_REQ_DEFINE(TDSCDMA, TDSCDMA_DIAG_EUL_UL_VAR_UPD_F)
  uint8  debug_enable_f3_message_type; /* Type of f3 message to be enabled from QXDM*/
  uint32 debug_var_val; /* bitmask to be used*/
DIAGPKT_REQ_END

/* Response packet for EUL UL VAR update request*/
DIAGPKT_SUBSYS_RSP_DEFINE(TDSCDMA, TDSCDMA_DIAG_EUL_UL_VAR_UPD_F)
  /* Response status */
  uint8  status;
DIAGPKT_RSP_END
#endif /*end of FEATURE_TDSCDMA_HSUPA*/

/* TDSCDMA Diag tdscdma statistics packet
 */

DIAGPKT_SUBSYS_REQ_DEFINE(TDSCDMA, TDSCDMA_DIAG_TDSCDMA_STATS_F)
DIAGPKT_REQ_END

DIAGPKT_SUBSYS_RSP_DEFINE(TDSCDMA, TDSCDMA_DIAG_TDSCDMA_STATS_F)
  /* Counts of the various slew values observed during DRX reacquisition. */
  srchzz_reacq_slew_stats_type      tdssrchzz_reacq_slew_stats;
  /* Counts of successful and failed reacquisitions, both list and 1/2/3. */
  srchzz_reacq_srch_stats_type      tdssrchzz_reacq_srch_stats;
  /* Number of times SFN mismatch is detected */
  uint32                            tdsdl_num_times_sfn_mismatch;
  /* Various paging statistics */
  page_stats_type                   page_stats;
  /* Various Intra freq cell reselection statistics */
  intra_freq_resel_stats_type   intra_freq_resel_stats;
  /* Various Inter freq cell reselection statistics */
  inter_freq_resel_stats_type   inter_freq_resel_stats;
  /* Various TDSCDMA to GSM cell reselection statistics */
  ttog_resel_stats_type    ttog_resel_stats;
  /* Various OOS related statistics */
  oos_stats_type  oos_stats;
  /* Various RACH transmission related statistics */
  rach_transmission_stats_type rach_transmission_stats;
  ttol_resel_stats_type ttol_resel_stats;
  gtot_resel_stats_type gtot_resel_stats;
  ltot_resel_stats_type ltot_resel_stats;
  acq_attemp_stats_type acq_attemp_stats;
  bplmn_stats_type bplmn_stats;
  fast_return_stats_type fast_return_stats;
  rrc_connetion_setup_stats_type rrc_connetion_setup_stats;
  /* + defang 02/21/2011: commented from original code */
  tdscdma_l2_rach_stats_type tdscdma_rach_stats;
  /* Sarao: Adding RACH related stats from TDS L2 */
  #if 0
  /* Data Structure to store RACH Transmission Preamble related statistics */
  rach_transmission_preamble_stats_type   rach_transmission_preamble_stats;
  #endif
  /* - defang 02/21/2011 */
DIAGPKT_RSP_END

/* TDSCDMA Diag clear tdscdma statistics packet
 */

DIAGPKT_SUBSYS_REQ_DEFINE(TDSCDMA, TDSCDMA_DIAG_TDSCDMA_CLEAR_STATS_F)
DIAGPKT_REQ_END

DIAGPKT_SUBSYS_RSP_DEFINE(TDSCDMA, TDSCDMA_DIAG_TDSCDMA_CLEAR_STATS_F)
DIAGPKT_RSP_END

/* Maximum number of CCTrCh, TrCh ref type and TB size list for dump */
#define TDSCDMA_DEC_TBSET_DUMP_MAX_PARAM    3     
/* The valid values that the dump_state can take:
     0 - No dumping 
     1 - Dump only the ciphered data at the start of header read 
     2 - Dump only the deciphered data at end of data read 
     3 - Dump both */
#define TDSCDMA_DEC_MAX_DUMP_STATE          3

/* Request packet */
DIAGPKT_SUBSYS_REQ_DEFINE(TDSCDMA, TDSCDMA_DEC_TBSET_DUMP_F)
  uint8 dump_state;
  uint8 num_cctrch_ref_type;
  uint8 cctrch_ref_type[TDSCDMA_DEC_TBSET_DUMP_MAX_PARAM];
  uint8 num_trch_ref_type;
  uint8 trch_ref_type[TDSCDMA_DEC_TBSET_DUMP_MAX_PARAM];
  uint8 num_tb_size;
  uint16 tb_size[TDSCDMA_DEC_TBSET_DUMP_MAX_PARAM];
DIAGPKT_REQ_END

 /* Response packet */
DIAGPKT_SUBSYS_RSP_DEFINE(TDSCDMA, TDSCDMA_DEC_TBSET_DUMP_F)
   uint8  status;
DIAGPKT_RSP_END


#define TDSCDMA_CQI_TEST_PATTERN_LEN  32
/* Request packet */
DIAGPKT_SUBSYS_REQ_DEFINE(TDSCDMA, TDSCDMA_CQI_OVERIDE_PARAMS_F)
  uint8   override;
  uint8   pattern_indicator;
  uint8   pattern_len;
  uint8   repetition_cycle_repeat_count;
  uint8   test_pattern[TDSCDMA_CQI_TEST_PATTERN_LEN];
DIAGPKT_REQ_END

 /* Response packet */
DIAGPKT_SUBSYS_RSP_DEFINE(TDSCDMA, TDSCDMA_CQI_OVERIDE_PARAMS_F)
   uint8  status;
DIAGPKT_RSP_END

/*---------------------------------------------------------------------------
                         FUNCTION PROTOTYPE
---------------------------------------------------------------------------*/
extern void tdsdiag_init (void);
extern void tdseul_set_var_through_qxdm( uint8 debug_switch_type, uint32 val );

#ifdef FEATURE_TDSCDMA_PLT
/*zhengming: an overall function to handle TPLT command from DIAG*/
extern PACKED void * tdscdmadiag_plt_command_handler(PACKED void *req_ptr, uint16 pkt_len);

extern PACKED void * tdscdmadiag_plt_result_handler(PACKED void *req_ptr, uint16 pkt_len);
extern PACKED void * tdscdmadiag_plt_command_handler_v2(PACKED void *req_ptr, uint16 pkt_len);

/*for saving received DIAG commands*/
extern void tplt_save_diag_cmd_log(uint32 cmd_id, DIAG_SUBSYS_TDSCDMA_TDSCDMA_PLT_COMMAND_F_req_type  *req_ptr);

void tdscdmadiag_plt_command_delay_rsp(tplt_client_cmd_enum_type cmd_id, boolean status);


#endif
#endif  /* #ifdef FEATURE_TDSCDMA */

#endif  /* DIAG_TDS_CMD_H */
