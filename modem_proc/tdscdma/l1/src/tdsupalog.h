#ifndef TDSUPALOG_H
#define TDSUPALOG_H

/*===========================================================================
                           T D S U P A L O G . H

DESCRIPTION
This file contains or includes files that contain Layer 1 searcher log packet
structure definitions, prototypes for Layer 1 seacher logging functions, any
constant definitions that are needed, and any external variable declarations
needed for Layer 1 searcher logging.


    Copyright (c) 1999-2011 by Qualcomm Technologies, Inc.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdsupalog.h#1 $ 
$DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when        who     what, where, why
--------    ---     --------------------------------------------------------

07/25/2011  weijunz    initial edition                       


===========================================================================*/

#include "log.h"
#include "log_codes.h"
#include "log_codes_tdscdma.h"

/*-----------------------------------------------------------------------
     CONSTANTS                                                         
-------------------------------------------------------------------------*/
/*! \brief Max number of E_AGCH channel */
#define TDSEDL_MAX_NUM_EAGCH 4 
/*! \brief Max number of SCHEDULED E-HICH channel */
#define TDSEDL_MAX_NUM_EHICH 4

/*! \brief TDSCDMA EDL chan midamble info struct
 *  
 * 	16  bits total
 */
typedef struct {
 
  /*! \brief allocation mode (0,1,2) */
  uint16                    alloc_mode:3;
  /*! \brief midamble configuration (2,4,6,8,10,12,14,16) */
  uint16                    config:5;
   /*! \brief midamble shift (0...15) */
  uint16                    shift:4;
  uint16                    :4;
  
} tdsupalog_edl_midamble_info_type;

/*! \brief TDSCDMA EAGCH TS  info struct
 *  
 * 	32  bits total
 */
typedef struct {
  /*! \brief time slot number (0,2,3,4,5,6) */
  uint32                    ts_num:3;
  /*! \brief first chan code (1,2,3,....,14,15,16) */
  uint32                    first_code:5;
  /*! \brief second chan code (1,2,3,....,14,15,16) */
  uint32                    sec_code:5;
  /*! \brief allocation mode (0,1,2) */
  uint32                    alloc_mode:3;
  /*! \brief midamble configuration (2,4,6,8,10,12,14,16) */
  uint32                    config:5;
   /*! \brief midamble shift (0...15) */
  uint32                    shift:4;
  uint32                    :7;
  
} tdsupalog_eagch_timeslot_info_type;

/*! \brief TDSCDMA EHICH TS  info struct
 *  
 * 	32  bits total
 */
typedef struct {
  /*! \brief time slot number (0,2,3,4,5,6) */
  uint32                    ts_num:3;
  /*! \brief  chan code (1,2,3,....,14,15,16) */
  uint32                    chan_code:5;
  /*! \brief E-HICH index */
  uint32                    ei:2;
  /*! \brief allocation mode (0,1,2) */
  uint32                    alloc_mode:3;
  /*! \brief midamble configuration (2,4,6,8,10,12,14,16) */
  uint32                    config:5;
   /*! \brief midamble shift (0...15) */
  uint32                    shift:4;
  uint32                      :10;
  
} tdsupalog_sched_ehich_timeslot_info_type;


/*! \brief TDSCDMA_L1_UPA_EAGCH_CHAN_INFO_LOG_PACKET 
          (LOG CODE : 0XD040)
   This log packet is used for external and internal !
 
   This log packet output E-AGCH channel configuration paramters and action
   recieved in CPHY_SETUP_REQ message from L3
  
   The worset case packet size for this log packet is 96+32+32+128=288 bits total
 */
typedef struct {
         /* Log header */
  log_hdr_type header;

  /*! Version  */
  uint32  version:8;
  /*! \brief start sub frame number for this configuration */
  uint32           start_sub_fn:13;
  /*! \brief tpc step size(1,2,3 db) */
  uint32           tpc_step: 2 ;
  /*! \brief rdi presence (0,1) */
  uint32           rdi_presence: 1 ;
  /*! \brief chan action 0(disable), 1(enable),2(reconfig),3(no action) */
  uint32           action: 2 ;
  /*! \brief number of E-AGCH 1~4 */
  uint32           num_eagch: 3 ;
  uint32                :3;

  /*! \brief ernti */
  uint16           ernti ;
  /*! \brief bler target */
  int8             bler_target ;

  /*! \brief */
  uint8 reserved;
  
  tdsupalog_eagch_timeslot_info_type eagch_ts_info[TDSEDL_MAX_NUM_EAGCH] ;
 
} tdsupalog_eagch_chan_info_log_pkt_type;

/* Macro to determine the sizes of the log packet */
#define TDSUPALOG_EAGCH_INFO_PKT_LEN(num)  ( \
    FPOS(tdsupalog_eagch_chan_info_log_pkt_type, eagch_ts_info)+  \
    ( num * sizeof(tdsupalog_eagch_timeslot_info_type)))

/*! \brief TDSCDMA_L1_UPA_SCHED_EHICH_CHAN_INFO_LOG_PACKET 
          (LOG CODE : 0XD041)
   This log packet is used for external and internal !
 
   This log packet output Scheduled E-HICH channel configuration paramters and action
   recieved in CPHY_SETUP_REQ message from L3
  
   The worset case packet size for this log packet is 96+32+128=256 bits total
 */
typedef struct {
       /* Log header */
  log_hdr_type header;

  /*! Version  */
  uint32  version:8;
  /*! \brief start sub frame number for this configuration */
  uint32           start_sub_fn:13;
  /*! \brief N_EHICH (4~15)*/
  uint32           n_ehich: 4 ;
  /*! \brief number of E-HICH*/
  uint32           num_ehich: 4 ;
   /*! \brief chan action 0(disable), 1(enable),2(reconfig),3(no action) */
  uint32           action:3 ;

  tdsupalog_sched_ehich_timeslot_info_type sched_ehich_ts_info[TDSEDL_MAX_NUM_EHICH] ;
 
} tdsupalog_sched_ehich_chan_info_log_pkt_type;


/* Macro to determine the sizes of the log packet */
#define TDSUPALOG_SCHED_EHICH_INFO_PKT_LEN(num)  ( \
    FPOS(tdsupalog_sched_ehich_chan_info_log_pkt_type, sched_ehich_ts_info)+  \
    ( num * sizeof(tdsupalog_sched_ehich_timeslot_info_type)))

/*! \brief TDSCDMA_L1_UPA_NON_SCHED_EHICH_CHAN_INFO_LOG_PACKET 
          (LOG CODE : 0XD042)
   This log packet is used for external and internal !
 
   This log packet output Non Scheduled E-HICH channel configuration paramters and action
   recieved in CPHY_SETUP_REQ message from L3
  
   The worset case packet size for this log packet is 96+32+32=160 bits total
 */
typedef struct {
     /* Log header */
  log_hdr_type header;

  /*! Version  */
  uint32  version:8;
   /*! \brief N_EHICH(4~15)*/
  uint32           n_ehich:4 ;
  /*! \brief chan code(1,2...15)*/
  uint32           chan_code:5 ;
   /*! \brief time slot number(0....6)*/
  uint32           ts_num:3  ;
    /*! \brief allocation mode (0,1,2) */
  uint32                    alloc_mode:3;
  /*! \brief midamble configuration (2,4,6,8,10,12,14,16) */
  uint32                    config:5;
   /*! \brief midamble shift (0...15) */
  uint32                    shift:4;


   /*! \brief chan action 0(disable), 1(enable),2(reconfig),3(no action) */
  uint32           action:2 ;
    /*! \brief start sub frame number for this configuration */
  uint32           start_sub_fn:13;
  /*! \brief signature group index(0...19)*/
  uint32           sig_grp_idx:5;
  uint32           :12 ;
 
 
} tdsupalog_non_sched_ehich_chan_info_log_pkt_type;



/*! \brief TDSCDMA_L1_UPA_NON_SCHED_GRANT_INFO_LOG_PACKET 
          (LOG CODE : 0XD043)
   This log packet is used for external and internal !
 
   This log packet output Non Scheduled grant information paramters and action
   recieved in CPHY_SETUP_REQ message from L3
  
   The worset case packet size for this log packet is 96+32+32+32=192 bits total
 */

typedef struct {

  /* Log header */
  log_hdr_type header;

  /*! Version  */
  uint32  version:8;
  /*! \brief start sub frame number for this configuration */
  uint32           start_sub_fn:13;
  /*! \brief activation time : CFN*/
  uint32          act_time:8 ;
  uint32            :3;
  
   /*! \brief Time resource related info*/
  uint32           trri:8;
   /*! \brief Power resource related info*/
  uint32          prri:8 ;
   /*! \brief code resource related info*/
  uint32          crri:8 ;
    /*! \brief repetition period*/
  uint32          rep_period:8 ;

    /*! \brief repetition length*/
  uint32          rep_length:8 ;
  /*! \brief number of E-UCCH instance: 1~8*/
  uint32          num_eucch:3 ;
  /*! \brief sub frame 0 or 1 of activation time*/
  uint32          sub_fn:1 ;
   /*! \brief chan action 0(disable), 1(enable),2(reconfig),3(no action) */
  uint32           action:2 ;
  uint32            :18;
 
} tdsupalog_non_sched_grant_info_log_pkt_type;


/*-------------------------------------------------------------------------

			  LOG PACKET: TDSCDMA_L1_UPA_EUL_CFG_LOG_PACKET
				(LOG CODE: 0xD049)			

This structure is used to log HSUPA E-PUCH and E-RUCCH configuration

This log generated every time RRC send a E-PUCH, E-RUCCH configuratio via CPHY_SETUP_REQ
to L1, to indicate L1 to do HSUPA setup or reconfig. If only non-scheduled HSUPA is configured,
then E-RUCCH config in this log package is not appliable

this log package size is fixed, 224 BYTEs
--------------------------------------------------------------------------*/

/*! \brief tdsupalog_erucch_cfg_struct_type, E-RUCCH configuration struct type,  168 bytes */
typedef struct{
  /*! \brief number of access serving class, range 1~8 */
  uint8 num_asc;

  /*! \brief number of PRACH-FPACH pairs, range 1~8  */
  uint8 num_prach_fpach ;

  /*! \brief UpPCH shift, in 16chips (0..127),See 25.331 v9.1.0, 10.3.2.5 and 10.3.6.39a */
  uint8 upPch_shift;
  
  /*! \brief Indicate what UL sync codes are available, 25.331 10.3.6.78o */
  uint8  ul_sync_code_mask;
  
  /*! \brief Desired UpPCH received power, in dBm (-120..-58) 
  ** See 25.331, 10.3.6.78d 
  */
  int8 prx_upPch_des;
  
  /*! \brief UL sync code power ramp step when no FPACH received after sent SYNC_UL. 
  ** The value is (0, 1, 2, 3) in dB.  See 25.331: 10.3.6.78d 
  */
  uint8 pwr_ramp_step;
  
  /*! \brief Maximum numbers of SYNC_UL transmissions.
  ** The value is (1, 2, 4, 8). 
  ** See 25.331: 10.3.6.78d 
  */
  uint8 max_sync_ul_tx;

  /** reserved */
  uint8 reserved;
  
  /*! \brief access serving class config list, index 0~num_asc-1 is in use */
  tdsullog_asc_cfg_type  asc_cfg[8];
  
  /*! \brief PRACH config, index 0~num_prach_fpach-1 is in use */
  tdsullog_prach_cfg_type  prach_cfg[8];
  
  /*! \brief FPACH config, index 0~num_prach_fpach-1 is in use */
  tdsullog_fpach_cfg_type  fpach_cfg[8];
} tdsupalog_erucch_cfg_struct_type;

/*! \brief TDSCDMA EUL Time Slot info struct
 *  
 * 	16  bits total
 */
typedef struct {
 
   /*! \brief Time slot (TS1,TS2,TS3,TS4,TS5) */
  uint16                    ts:3;
  /*! \brief allocation mode (0,1,2) */
  uint16                    alloc_mode:3;
  /*! \brief midamble configuration (2,4,6,8,10,12,14,16) */
  uint16                    config:5;
   /*! \brief midamble shift (0...15) */
  uint16                    shift:4;
  uint16                    :1;
  
} tdsupalog_eul_ts_info_type;


/*! \brief	    LOG PACKET: TDSCDMA_L1_UPA_EUL_CFG_LOG_PACKET
				(LOG CODE: 0xD049)			

This structure is used to log HSUPA E-PUCH and E-RUCCH configuration

This log generated every time RRC send a E-PUCH, E-RUCCH configuratio via CPHY_SETUP_REQ
to L1, to indicate L1 to do HSUPA setup or reconfig. If only non-scheduled HSUPA is configured,
then E-RUCCH config in this log package is not appliable

this log package size is fixed, 224 BYTEs
*/

typedef struct{
  /* Log header */
  log_hdr_type header;

  /*! Version  */
  uint32  version:8;

  /*! \brief action type:Start,Reconfig,Diasable*/
  uint32  action:8 ;

  /*! \brief start sub frame number for this configuration */
  uint32  start_sub_fn:13;
  
  /*! \brief 25.331, 10.3.5.7d: HARQ RV config type, 0: RV TABLE; 1: RV0
      If this value is "rv0", the UE shall only use E_DCH RV index 0. 
      If this value is "RV TABLE" then UE shall use an RSN based RV index.  */
  uint32 harq_rv_cfg:1;
  
  /*! \brief SNPL report type. 0: SNPL report TYPE1; 1:SNPL report TYPE2 */
  uint32 snpl_type:1;
  
  /*! \brief beacon channel path loss estimation switch. 0:OFF; 1:ON
  if set to "ON", it indicates that the UE may take into account 
  path loss estimated from beacon function physical channels */
  uint32 beacon_ple_est:1;

  /*! \brief Power control step size, unit is dB, range 1~3 */
  uint8 tpc_step:2;
  
  /*! \brief E-PUCH synchronization adjust step size, unit is 1/8 chip, range 0~7, 
      mapping to actual value 1~8
  */
  uint8 ul_sync_step:3;
  
  /*! \brief E-PUCH synchronization adjust frequency, unit is sub frame(5ms),
       range 0~7, mapping to actual value 1~8
  */
  uint8 ul_sync_freq:3;

  /*! \brief PRX_DES_BASE, unit is dBm, range: -112dbm ...-50dbm in step of 1dbm */
  int8 prx_des_base;
  
  /*! \brief Pe-base power control gap, range 1~255 */
  uint8 p_base_contr_gap;
  
  /*! \brief minimum allowed code rate, range (0..63), Maps 0.055 to 1.0 in steps of 0.015 */
  uint8 min_code_rate;

  /*! \brief maximum allowed code rate, range (0..63), Maps 0.055 to 1.0 in steps of 0.015 */
  uint8 max_code_rate;
  
    
  /*! \brief number of QPSK reference ETFCS table, range 1~8 */
  uint8 num_ref_e_tfcs_qpsk;
  
  /*! \brief number of 16QAM reference ETFCS table, range 1~8 */
  uint8 num_ref_e_tfcs_16qam;

  /*! \brief QPSK reference code rate list, only index 0~num_ref_e_tfcs_qpsk-1 is valid, 
  for each IE, range : 0...10. Mapped to code rate from 0 to 1.0 in step 0.1 */
  uint8 ref_code_rate_qpsk[8];

  /*! \brief  QPSK reference beta gain list. only index 0~num_ref_e_tfcs_qpsk-1 is valid, 
  for each item, range is from -15dB to 16dB*/
  int8 ref_gain_factor_qpsk[8];

  /*! \brief 16QAM reference code rate list, only index 0~num_ref_e_tfcs_16qam-1 is valid, 
  for each IE, range : 0...10. Mapped to code rate from 0 to 1.0 in step 0.1 */
  uint8 ref_code_rate_16qam[8];

  /*! \brief  16QAM reference beta gain list. only index 0~num_ref_e_tfcs_16qam-1 is valid, 
  for each item, range is from -15dB to 16dB*/
  int8 ref_gain_factor_16qam[8];


  tdsupalog_eul_ts_info_type  epuch_ts_info[TDSL1_MAX_EPUCH_TS];
  
  /*! \brief E-RUCCH configuration */
  tdsupalog_erucch_cfg_struct_type erucch_cfg;
} tdsupalog_eul_cfg_log_pkt_type;





/*-------------------------------------------------------------------------

			  LOG PACKET: TDSCDMA_L1_UPA_ETFC_RESTRICT_INFO_LOG_PACKET
				(LOG CODE: 0xD044)			

This structure is used to log HSUPA E-TFC selection restriction info

This log generated every 200 ms, it can contain maximum 40 samples, each samples
is generated every 5 sub frames, only those sub frames which serving grant is
available, and E-PUCH is sent will generate a sample, if serving grant is not available, 
sample is not generated.

max size = 12 + 4 + 16*40 = 656 Byte, max bandwidth = 3.28 KB/s
--------------------------------------------------------------------------*/
#define TDSUPALOG_PKT_SAMPLE_MAX  40

/*==========================HEADER STRUCTURE====================================*/

typedef enum {
  TDSEULLOG_CH_COEXT_TYPE_DPCH = 0,
  TDSEULLOG_CH_COEXT_TYPE_HSSICH,
  TDSEULLOG_CH_COEXT_TYPE_ERUCCH,
  TDSEULLOG_CH_COEXT_TYPE_UPPCH
} tdsupalog_other_ch_coext_type_enum_type;

/*! \brief tdsupalog_etfc_restrict_info_struct_type, 12 bytes */
typedef struct {
  /*! \brief sub frame number offset between this sample and first_sample_sub_fn, sub frame is 
       counted wrap around
  */
  uint32 subfn_offset:6;
  /*! \brief SG_TYPE: serving grant type
      0: scheduled grant available, and allocated TTI
      1: non-scheduled grant available
  */
  uint32 sg_type:2;
  /*! \brief current SNPL measurement calculated result
  */
  uint32 snpl_calc:6;
  /*! \brief  current SNPL report value
  */
  uint32 snpl_rpt:6;

  /*! \brief other UL channel type which is coexist with E-PUCH, 2 bits, 0:DPCH, 1:HS-SICH, 2:E-RUCCH; 3:UpPCH
  */
  uint8 other_ch_type:2;
  /*! \brief power related resource, 0~31
  */
  uint32 prri:5;
  /*! \brief UPH: UE power headroom measurement result, 0~31
  */
  uint32 uph:5;
  /*! \brief PEBASE: Pe-base, in unit of 1 dBm
  */
  int8 pebase;
  /*! \brief, DTX_FLAG, DTX flag, 1:DTX, 0:NOT DTX
  */
  uint8 dtx_flag:1;
  /*! \brief MAX_SUP_ETFCI: max supported E-TFCI,0~63
  */
  uint8 max_sup_etfci:7;
  /*! \brief MSX_SG_ETFCI: max E-TFCI by serving grant,0~63
  */
  uint16 max_sg_etfci:7;
  /*! \brief ETFCI: MAC selected E-TFCI, 0~63
  */
  uint16 etfci:7;
  /*! \brief  HARQ ID, 0~4
  */
  uint16 harq_id:2;
  /*! \brief  UPA MAC buffer size in byte
  */
  uint32 buf_status;
} tdsupalog_etfc_restrict_info_struct_type;


/*! \brief	    LOG PACKET: TDSCDMA_L1_UPA_ETFC_RESTRICT_INFO_LOG_PACKET
				(LOG CODE: 0xD044)			

This structure is used to log HSUPA E-TFC selection restriction info

This log generated every 200 ms, it can contain maximum 40 samples, each samples
is generated every 5 sub frames, only those sub frames which serving grant is
available, and E-PUCH is sent will generate a sample, if serving grant is not available, 
sample is not generated.

max size = 12 + 4 + 12*40 = 336 Byte, every 200ms, max bandwidth 1.68 KB/s
*/
typedef struct
{
  /* Log header */
  log_hdr_type header;

  /*! Version  */
  uint32 version:8;

  /*! \brief Number of samples in this log package, range 0~40
  */
  uint32 num_samples:6;

  /*! \brief subframe number for first sample
  */
  uint32 first_sample_sub_fn:13;

  /*! \brief UE max allowed transmit power, from NV config or RF capability
  */
  uint32 max_epuch_pwr_avail:5;

  /*! \brief E-TFCI restriction sample info, length is variable, actual number is num_samples
  */
  tdsupalog_etfc_restrict_info_struct_type etfc_restrict_info[TDSUPALOG_PKT_SAMPLE_MAX];
} tdsupalog_etfc_restrict_pkt_type;

#define TDSEULLOG_ETFC_RESTRICT_INFO_PKT_LEN(num_samples)  (FPOS(tdsupalog_etfc_restrict_pkt_type,etfc_restrict_info) + num_samples * sizeof(tdsupalog_etfc_restrict_info_struct_type))
#define TDSEULLOG_INT8_INVALID_VAL   -128
#define TDSEULLOG_UINT8_INVALID_VAL   0xFF

extern uint32 tdseulcfg_etfc_restrict_log_pkt_sample_num[2];
extern uint32 tdseulcfg_etfc_restrict_log_pkt_subfn_cnt[2];

extern tdsupalog_etfc_restrict_pkt_type tdseulcfg_etfc_restrict_info_log_pkt[2];
extern uint8 tdseulcfg_etfc_restrict_buf_idx;
extern uint8 tdseulcfg_etfc_restrict_log_submit_buf_idx;
extern boolean tdseulcfg_etfc_restrict_dirty_bit[2];

/*-------------------------------------------------------------------------

			  LOG PACKET: TDSCDMA_L1_UPA_HARQ_DB_INFO_LOG_PACKET
				(LOG CODE: 0xD045)			

This structure is used to log HSUPA HARQ database info

This log generated every 100 ms, in every subframe, L1 generate 0~8 samples, 
the number is depending on how many HARQ process which is NOT at inactive state,
so the length of this log packet is a variable, at worst case, log packet size 
is: 12 + 4 + 24 + 4*TDSUPALOG_PKT_SAMPLE_MAX/2*8 =  680 bytes
max bandwidth = 6.8 KB/s
--------------------------------------------------------------------------*/

/*! \brief ,tdsupalog_harq_db_info_sample_struct_type, 4 bytes
*/
typedef struct {
  /*! \brief HARQ ID, 3 bits, range 0~7, 0~3 for scheduled transmission, 4~7 for non-scheduled transmission
  */
  uint32 harq_id:3;
  /*! \brief HARQ status,  
       TDSEUL_INACTIVE_HARQ_PROC = 0, // inactive HARQ process
       TDSEUL_INIT_TX_HARQ_PROC = 1, // new transmission, E-HICH is not received
       TDSEUL_RE_TX_HARQ_PROC = 2, // retransmission, E-HICH is not received
       TDSEUL_NACK_RVD_HARQ_PROC = 3, // NACK is received, pending for retrasmission
  */
  uint32 harq_status:2;
  /*! \brief E-TFCI selected by MAC for this HARQ, 6 bits, 0~63
  */
  uint32 etfci:6;
  /*! \brief  only 7 LSB of last transmit sub frame number, 0~127
  */
  uint32 last_tx:7;
  /*! \brief  an index to record RTX_TIMER value, real time in ms = rtx_tmr * 5, 
              for example, 1 means RTX_TIMER = 5 ms
              range 0~127
  */
  uint32 rtx_tmr:7;
  /*! \brief  indicate how many times of transmission(both new tx and re-tx), including this transmission
              valid range 0~15
  */
  uint32 curr_tx_nb:4;
  /*! \brief  indicator if this HARQ selected for next TTI, 0:not selected; 1:selected
  */
  uint32 is_selected:1;
  /*! \brief  indicator if RTX_TIMER is timeout  0:not timeout; 1:timeout
  */
  uint32 rtx_timeout_flag:1;
  /*! \brief  indicator if number of transmission exceed limit  0:not exceed; 1:exceed
  */
  uint32 tx_num_exd_flag:1;
} tdsupalog_harq_db_info_sample_struct_type;

/*! \brief tdsupalog_harq_db_info_hdr_struct_type, 24 bytes
 */
typedef struct{
  /*! \brief number of sub frames elapsed for this log package, 0~40
  */
  uint16 num_sub_frames;
  /*! \brief start sub frame number of this log package
  */
  uint16 start_sub_frame_num;
  /*! \brief array, number of samples in each sub frames, array index valid range is 0 ~  num_sub_frames,
             for each item, valid range is 0~8, if set to 0, it means there is no sample in that sub frame
  */
  uint8 num_samples_each_subframe[TDSUPALOG_PKT_SAMPLE_MAX/2];
} tdsupalog_harq_db_info_hdr_struct_type;


/*! \brief      LOG PACKET: TDSCDMA_L1_UPA_HARQ_DB_INFO_LOG_PACKET
				(LOG CODE: 0xD045)			

This structure is used to log HSUPA HARQ database info

This log generated every 100 ms, in every subframe, L1 generate 0~8 samples, 
the number is depending on how many HARQ process which is NOT at inactive state,
so the length of this log packet is a variable, at worst case, log packet size 
is: 12 + 4 + 24 + 4*TDSUPALOG_PKT_SAMPLE_MAX/2*8 =  680 bytes
max bandwidth = 6.8 KB/s
*/
typedef struct {

  /* Log header */
  log_hdr_type header;

  /*! Version  */
  uint32 version:8;

  uint32 reserved:8;

  /*! \brief  total number of samples carried in this log package */
  uint32 num_samples:16;
  
  /*! \brief HARQ database info log package header info
  */
  tdsupalog_harq_db_info_hdr_struct_type  harq_db_hdr_info;

  /*! \brief HARQ database info log package sample info, length is variable, actual number is num_samples
  */
  tdsupalog_harq_db_info_sample_struct_type  harq_db_sample_info[TDSUPALOG_PKT_SAMPLE_MAX/2*8];
} tdsupalog_harq_db_pkt_type;

#define TDSEULLOG_HARQ_DB_PKT_LEN(total_sample_num)  (FPOS(tdsupalog_harq_db_pkt_type,harq_db_sample_info) + sizeof(tdsupalog_harq_db_info_sample_struct_type)*total_sample_num)



/*-------------------------------------------------------------------------

			  LOG PACKET: TDSCDMA_L1_UPA_EDL_DEC_AND_SG_INFO_LOG_PACKET
				(LOG CODE: 0xD046)			

This structure is used to log HSUPA HARQ database info

This log generated every 200 ms, max packet size = 12 + 4 + 8*40 = 336 BYTEs,
max bandwidth = 1.68 KB/s
--------------------------------------------------------------------------*/

/*! \brief tdsupalog_edl_dec_and_sg_info_struct_type , 8 BYTE
*/
typedef struct {
  /*! \brief serving grant type
       0: No serving grant
       1: scheduled serving grant, next TTI is allocated TTI
       2: scheduled serving grant, next TTI is spacing TTI
       3: non-scheduled serving grant
  */
  uint32 sg_type:2;
  /*! \brief, E-AGCH received flag
             1: E-AGCH is received in previous sub frame
  */
  uint32 ag_flag:1;
  /*! \brief PRRI, power resource related information, 0~31
  */
  uint32 prri:5;
  /*! \brief  CRRI, code resource related information, 0~30
  */
  uint32 crri:5;
  /*! \brief  TRRI, timeslot resource related information, bit 4~0 mapping to TS1~TS5
  */
  uint32 trri:5;
  /*! \brief, E-UCCH number indicator, 0~7
  */
  uint32 eni:3;
  /*! \brief, RDI, resource duration indicator, 0~7
  */
  uint32 rdi:3;
  /*! \brief  scheduled or non-scheduled E-HICH received flag
  */
  uint32 hich_rvd_flag:1;
  /*! \brief ACK/NACK in scheduled/non-scheduled E-HICH
  */
  uint32 hich_ack_nack:1;
  /*! \brief HARQ ID mapping to scheduled/non-scheduled E-HICH
  */
  uint32 hich_harq_id:3;
  /*! \brief HARQ ID select for next TTI, 0~7
  */
  uint32 select_harq_id:3;
  /*! \brief new transmission or retransmission flag, 
      1: new transmission, 0:retransmission
  */
  uint32 new_tx:1;
  /*! \brief mac selected E-TFCI for this E-PUCH transmission, could be new transmission 
       or retransmission, range is 0~63
  */
  uint32 etfci:6;
  /*! \brief  for select HARQ ID, indicate how many times of transmission(both new tx and re-tx), 
              including this transmission, valid range 0~15
  */
  uint32 curr_tx_nb:5;
  /*! \brief  for select HARQ ID, max transmission number(both new tx and re-tx) for selected HARQ ID,
              configured by MAC, valid range 0~15
  */
  uint32 max_tx_nb:5;
  /*! \brief  an index to record RTX_TIMER value, real time in ms = rtx_tmr * 5, 
              for example, 1 means RTX_TIMER = 5 ms
              range 0~127
  */
  uint32 rtx_tmr:7;
  /*! \brief  an index to RTX_TIMER expire value value, real time in ms = rtx_tmr_exp * 5, 
              for example, 1 means RTX_TIMER expire is 5 ms
              range 0~127
  */
  uint32 rtx_tmr_exp:7;
  /*! \brief E-RUCCH transmission flag, 0: NO E-RUCCH, 1: E-RUCCH is OTA in previous sub frame
  */
  uint32 erucch_flag:1;  
  /*! \brief  UPA MAC buffer size in byte
  */
  uint32 buf_status;
} tdsupalog_edl_dec_and_sg_info_struct_type;

/*! \brief      LOG PACKET: TDSCDMA_L1_UPA_EDL_DEC_AND_SG_INFO_LOG_PACKET
				(LOG CODE: 0xD046)			

This structure is used to log HSUPA HARQ database info

This log generated every 200 ms, in every subframe, max packet size = 12 + 4 + 8*40 = 336 BYTEs,
max bandwidth = 1.68 KB/s
--------------------------------------------------------------------------*/

typedef struct {
  /* Log header */
  log_hdr_type header;

  /*! Version  */
  uint32 version:8;

  /*! \brief Number of samples 0~40
  */
  uint32 num_samples:8;

  /*! \brief first log packet start sub frame number
  */
  uint32 first_sample_sub_fn:16;

  /*! \brief  EDL decode result and serving grant info log package sample info, length is variable,
              actual size is num_samples
  */
  tdsupalog_edl_dec_and_sg_info_struct_type dec_and_sg_sample_info[TDSUPALOG_PKT_SAMPLE_MAX];
} tdsupalog_edl_dec_and_sg_pkt_type;

extern tdsupalog_edl_dec_and_sg_pkt_type tdseulcfg_dec_and_sg_log_pkt[2];
extern uint8 tdseulcfg_dec_and_sg_buf_idx;
extern uint8 tdseulcfg_dec_and_sg_log_submit_buf_idx;
extern uint32 tdseulcfg_dec_and_sg_log_pkt_sample_num[2];
extern boolean tdseulcfg_dec_and_sg_dirty_bit[2];


#define TDSEULLOG_EDL_DEC_AND_SG_PKT_LEN(num_samples)  (FPOS(tdsupalog_edl_dec_and_sg_pkt_type,dec_and_sg_sample_info) + sizeof(tdsupalog_edl_dec_and_sg_info_struct_type)*num_samples)



typedef enum {
  TDSUPALOG_NO_TPC,
  TDSUPALOG_TPC_AGCH,
  TDSUPALOG_TPC_HICH,
  TDSUPALOG_TPC_AGCH_AND_HICH,
  TDSUPALOG_TPC_CONFLICT
} tdsupalog_tpc_cmd_src_info_enum_type;

/*! \brief ,tdsupalog_upa_eagch_pwr_ctl_info_struct_type, 12 bytes
*/
typedef struct{
  /*! \brief , TPC command, -1:DOWN; +1:UP; 0:HOLD or NO TPC cmd
  */
  int8 tpc_cmd;
  /*! \brief TPC accumulate result, unit of 1dB
  */
  int8 tpc_accum;
  /*! \brief Pe-base, Q7 format, resolution is 1/128 dBm
  */
  int16 pebase;
  /*! \brief TPC adjustment in HSUPA outer loop power control algorithm, Q9 format, resolution is 1/512 dBm
  */
  int16 tpc_adj;
  /*! \brief, E-PUCH transmit power, Q9 format, resolution is 1/512 dB
  */
  int16 epuch_tx_pwr;
  /*! \brief ACK/NACK flag, 0: E-HICH is not received; +1: ACK, -1: NACK
  */
  int8 ack_nack_flag;
  /*! \brief a counter, used to count how many sub frames between two consective TPC command, 
  for scheduled transmission, TPC command is carried in E-AGCH,
  for non-scheduled transmission, TPC command is carried in E-HICH
  */
  uint8 gap_cnt;
  /*! \brief , current path loss, unit is dB
  */
  uint8 curr_pathloss;
  /*! \brief , path loss of initial transmision, unit is dB
  */
  uint8 init_tx_pathloss;
  /*! \brief, E-PUCH transmission flag, 0: NOT send E-PUCH, 1: send E-PUCH
  */
  uint16 tx_epuch_flag:1;
  /*! \brief, E-TFCI selected by MAC
  */
  uint16 etfci:7;
  /*! \brief E-PUCH power control type, 0: close loop, 1:open loop
  */
  uint16 pwr_ctl_type:1;
  /*! \brief E-PUCH modulation type, 0:QPSK; 1:16QAM
  */
  uint16 epuch_mod_type:1;
  /*! \brief, HARQ power offset
  */
  uint16 pwr_offset:3;
  /*! \brief, SF index, 3 bits, mapping
		0: SF1
		1: SF2
		2: SF4
		3: SF8
		4: SF16
  */
  uint16 sf_idx:3;
  /*! \brief, beta_0_e, related to E-TFCI, unit is 0.001 dB 
  */
  int16 beta_0_e;
} tdsupalog_upa_epuch_pwr_ctl_info_struct_type;



 /*! \brief tdsupalog_upa_eagch_pwr_ctl_info_struct_type,  8 BYTE
  */
typedef struct{
  /*! \brief E-AGCH reveived sub frame number 
  */
  uint32 sub_fn:13;
  /*! \brief E-AGCH SIR target, L1 send to FW, dB, Q10
  */
  uint32 sir_tgt:16;
  /*! \brief E-AGCH SIR target hit low/high boundary flag
  */
  uint32 hit_bound:1;
  /*! \brief E-AGCH outer loop power control enable flag, 0:disable, 1:enable
  */
  uint32 enable:1;
  /*! \brief 1 bit pad
  */
  uint32 pad1:1;
  /*! \brief ECSN carried in last time received E-AGCH
  */
  uint32 last_ecsn:3;
  /*! \brief ESCN carried in current received E-AGCH
  */
  uint32 curr_ecsn:3;
  /*! \brief E-AGCH SIR target increase adjust step size, dB, Q10
  */
  uint32 sir_up_step_size:10;
  /*! \brief E-AGCH SIR target decrease adjust step size, dB, Q10
  */
  uint32 sir_down_step_size:10;
  /*! \brief delta SIR, Q4
  */
  uint32 delta_sir:6;
} tdsupalog_upa_eagch_pwr_ctl_info_struct_type;

/*! \brief      LOG PACKET: TDSCDMA_L1_UPA_PWR_CTL_INFO_LOG_PACKET
				(LOG CODE: 0xD047)			

This structure is used to log HSUPA E-PUCH inner loop power control, and 
E-AGCH outer loop power control related info

This log generated every 200 ms, L1 create each sample every 5 ms, so one packet
will contain 40 samples
max size = 12 + 12 + 16 *40 + 8*40 = 984 BYTE, max bandwidth = 4.9 KB/s
*/
typedef struct{
  /* Log header */
  log_hdr_type header;

  /*! Version  */
  uint8 version;
  /*! \brief PRXdes_base, this is configured by RRC message when HSUPA is setup/reconfigure, unit of 1dBm
  */
  int8 prx_des_base;
  /*! \brief start sub frame number for the first E-PUCH, E-AGCH sample
  */
  uint16 first_epuch_sample_sub_fn:13;
    /*! \brief a flag to indicate if HSUPA E-PUCH outer loop power control 
             optimization is turned ON/OFF, 0:OFF; 1:ON
  */
  uint16 epuch_olpc_enable:1;
  /*! \brief TPC step size, 1: 1dB; 2: 2dB; 3: 3dB
  */
  uint16 tpc_step_size:2;
    /*! \brief BLER target in HSUPA OLPC algorithm, an index value, ranges from 0 to 63, 
      index mapping to float equation is
      10^(- epuch_olpc_bler_tgt_idx * 0.05), eg. epuch_olpc_bler_tgt_idx = 20 means 
      10^( - 20 * 0.05) = 10 %
  */
  uint16 epuch_olpc_bler_tgt_idx:6;
  /*! \brief TPC adj step size in HSUPA OLPC algorithm, Q9 format, 1/512 dB resolution
  */
  uint16 epuch_olpc_tpc_adj_step_size:10;
  /*! \brief delta_down in HSUPA OLPC algorithm, Q9 format, 1/512 dB resolution
  */
  int16 epuch_olpc_delta_down_step_size;
  
  /*! \brief number of E-PUCH inner loop power control info samples, 0~40
  */
  uint32 num_epuch_samples:6;
  /*! \brief number of E-AGCH outer loop power control info samples, 0-40
  */
  uint32 num_eagch_samples:6;
  /* \brief Pebase power control gap, indicate Pe-base will reset when no grant for this many number of TTIs,
            255 means always use close loop
  */
  uint32 pebase_pwr_ctl_gap:8;
  /*! \brief number of TPC command is UP
  */
  uint32 num_tpc_up:6;
  /*! \brief number of TPC command is DOWN
  */
  uint32 num_tpc_down:6;
  /*! \brief E-PUCH inner loop power control info, length is variable, actual size is num_epuch_samples
  */
  tdsupalog_upa_epuch_pwr_ctl_info_struct_type epuch_pwr_ctl_info[TDSUPALOG_PKT_SAMPLE_MAX];

  /*! \brief E-AGCH outer loop power control info, length is variable, actual size is num_eagch_samples
  */
  tdsupalog_upa_eagch_pwr_ctl_info_struct_type eagch_pwr_ctl_info[TDSUPALOG_PKT_SAMPLE_MAX];
} tdsupalog_upa_pwr_ctl_pkt_type;

#define TDSEULLOG_UPA_PWR_CTL_INFO_PKT_LEN(num_samples_puch, num_samples_agch)  (FPOS(tdsupalog_upa_pwr_ctl_pkt_type,epuch_pwr_ctl_info) + num_samples_puch * sizeof(tdsupalog_upa_epuch_pwr_ctl_info_struct_type) + num_samples_agch*sizeof(tdsupalog_upa_eagch_pwr_ctl_info_struct_type))

extern tdsupalog_upa_pwr_ctl_pkt_type tdseulcfg_upa_pwr_ctl_info_log_pkt[2];
extern uint8 tdseulcfg_upa_pwr_ctl_buf_idx;
extern uint8 tdseulcfg_upa_pwr_ctl_log_submit_buf_idx;
extern uint32 tdseulcfg_upa_epuch_pwr_ctl_log_pkt_sample_num[2];
extern uint32 tdseulcfg_upa_eagch_pwr_ctl_log_pkt_sample_num[2];
extern boolean tdseulcfg_upa_pwr_ctl_dirty_bit[2];

extern void tdseulcfg_send_upa_pwr_ctl_info_to_diag(void);


/*-------------------------------------------------------------------------

			  LOG PACKET: TDSCDMA_L1_UPA_L1_STATS_INFO_LOG_PACKET
				(LOG CODE: 0xD048)			

This structure is used to statistic HSUPA L1 working status, shows data thoughput
serving grant status, transmit power, etc.

This log package is generated every 1 second.
log package size is 12 + 4 + 68 + 56 = 140 BYTE, max bandwidth = 0.14 KB/s
--------------------------------------------------------------------------*/

/*! \brief scheduled transmission L1 statistic info, 68 BYTE
*/
typedef struct {
  /*! \brief start sub frame number of this statistic
  */
  uint16 start_sub_fn;
  /*! \brief indicate how many sub frames are included in this statistic
  */
  uint8 num_sub_frames;
  /*! \brief number of scheduled serving grant available sub frames, only count
             allocated TTI
  */
  uint8 num_sg_avail_sub_frames;
  /*! \brief number of new transmission
  */
  uint8 num_new_tx;
  /*! \brief number of retransmission
  */
  uint8 num_re_tx;
  /*! \brief number of ACK received in E-HICH
  */
  uint8 num_ack;
  /*! \brief number of NACK received in E-HICH
  */
  uint8 num_nack;
  /*! \brief number of NACK received in E-HICH for new transmission
  */
  uint8 num_nack_new_tx;
  /*! \brief number of HARQ faied due to max number of retransmission, or retransmission
       timer timeout
  */
  uint8 num_failed_harq;
  /*! \brief total BLER, 0~1, Q15 format
  */
  uint16 total_bler;
  /*! \brief BLER for new transmission, 0~1, Q15 format
  */
  uint16 new_tx_bler;
  /*! \brief residual BLER, error of HARQ failure after max no. of RE-TX or RTX_TIMER timeout, 0~1, Q15 format
  */
  uint16 residual_bler;
  /*! \brief HSUPA L1 throughput, sum of the number of transmitted bits per TTI,
       counting only new transmissions
  */
  uint32 upa_l1_tput;
  /*! \brief Sum of the number of transmitted bits per TTI,
       counting both new transmissions and retransmission
  */
  uint32 sum_raw_bits;
  /*! \brief Sum of the number of bits that could have been transmitted
       per TTI based on available power
  */
  uint32 sum_pwr_bits;
  /*! \brief Average number of bits that could have been transmitted
        per TTI based on serving grant
  */
  uint32 sum_sg_bits;
  /*! \brief Sum of the scheduled buffer status in bytes
  */
  uint32 sum_s_buf;
  /*! \brief Sum of all E-TFCI counting only new transmissions
  */
  uint16 sum_etfci;
  /*! \brief number of E-AGCH received
  */
  uint8 num_agch;
  /*! \brief number of spacing TTI
  */
  uint8 num_spacing_tti;
  /*! \brief sum of PRRI field in E-AGCH, only count allocated TTI
  */
  int16 sum_prri;  
  /*! \brief current Pe-base value, unit of 1dBm
  */
  int8 pebase;
  /*! \brief number of TPC command is UP
  */
  uint8 num_tpc_up;
  /*! \brief number of TPC command is DOWN
  */
  uint8 num_tpc_down;
  /*! \brief number of DTX
  */
  uint8 num_dtx;
  /*! \brief sum of virtual unit, one code with spread factor SF
       equals to 16/SF virtual unit, count all codes in all E-PUCH timeslot 
       only count allocated TTI
  */
  uint16 sum_num_vu;
  /*! \brief number of HARQ process used for scheduled transmission which RTX_TIMER timeout
  */
  uint8 num_rtx_timeout;
  /*! \brief number of HARQ process used for scheduled transmission 
       which exceed max n transmission number
  */
  uint8 num_tx_num_exceed;
  /*! \brief Number of times the first retransmission was successfully
		 ACKed; enables to calculate BLER of first transmission
		 statistics
  */
  uint8 num_first_tx_acked;
  /*! \brief Number of TTIs transmission is limited by power counting
      only new transmissions
  */
  uint8 num_pwr_limited;
  /*! \brief Number of TTIs transmission is limited by serving grant
       counting only new transmissions
  */
  uint8 num_sg_limited;
  /*! \brief number of TTI when SI is transmitted alone in E-PUCH (E-TFCI=0),
       counting only new transmission
  */
  uint8 num_si_epuch;
  /*! \brief sum E-PUCH power in dBm
  */
  int16 sum_epuch_pwr;
  /*! \brief number of successful E-RUCCH transmission
  */
  uint8 num_success_erucch;
  /*! \brief number of unsuccessful E-RUCCH transmission, only count no FPACH received
       scenario, abort E-RUCCH by L1 is not counted
  */
  uint8 num_failed_erucch;
  /*! \brief number of E-RUCCH abort scenario
  */
  uint8 num_abort_erucch;
  /*! \brief number of E-HICH losted
  */
  uint8 num_ehich_lost;
  /*! \brief number of E-AGCH losted
  */
  uint8 num_eagch_lost;
  /*! \brief sum of number of timeslot in E-AGCH serving grant, only count allocated TTI
  */
  uint8 sum_num_ts;
  /*! \brief sum of number of sub frames which E-TFCI supported by serving grant is smaller 
       than E-TFCI supported by UE power headroom
  */
  uint16 num_sg_smaller_than_uph;
  /*! \brief an array of 4, index is HARQ ID, this counts how many times this HARQ ID
       retransmission failed(retransmission all get NACK)
  */
  uint8 num_harq_fail_per_id[TDSEULSG_MAX_NUM_HARQ_PROCESS/2];
} tdsupalog_scheduled_tx_stats_struct_type;


/*! \brief non-scheduled transmission L1 statistic info, 56 BYTE
  */
typedef struct {
  /*! \brief start sub frame number of this statistic
  */
  uint16 start_sub_fn;
  /*! \brief indicate how many sub frames are included in this statistic
  */
  uint8 num_sub_frames;
  /*! \brief number of scheduled serving grant available sub frames, only count
             allocated TTI
  */
  uint8 num_sg_avail_sub_frames;
  /*! \brief number of new transmission
  */
  uint8 num_new_tx;
  /*! \brief number of retransmission
  */
  uint8 num_re_tx;
  /*! \brief number of ACK received in E-HICH
  */
  uint8 num_ack;
  /*! \brief number of NACK received in E-HICH
  */
  uint8 num_nack;
  /*! \brief number of NACK received in E-HICH for new transmission
  */
  uint8 num_nack_new_tx;
  /*! \brief number of HARQ faied due to max number of retransmission, or retransmission
       timer timeout
  */
  uint8 num_failed_harq;
  /*! \brief total BLER, 0~1, Q15 format
  */
  uint16 total_bler;
  /*! \brief BLER for new transmission, 0~1, Q15 format
  */
  uint16 new_tx_bler;
  /*! \brief residual BLER, error of HARQ failure after max no. of RE-TX or RTX_TIMER timeout, 0~1, Q15 format
  */
  uint16 residual_bler;
  /*! \brief HSUPA L1 throughput, sum of the number of transmitted bits per TTI,
       counting only new transmissions
  */
  uint32 upa_l1_tput;
  /*! \brief Sum of the number of transmitted bits per TTI,
       counting both new transmissions and retransmission
  */
  uint32 sum_raw_bits;
  /*! \brief Sum of the number of bits that could have been transmitted
       per TTI based on available power
  */
  uint32 sum_pwr_bits;
  /*! \brief Average number of bits that could have been transmitted
        per TTI based on serving grant
  */
  uint32 sum_sg_bits;
  /*! \brief Sum of non-scheduled buffer status in bytes
  */
  uint32 sum_ns_buf;
  /*! \brief Sum of all E-TFCI counting only new transmissions
  */
  uint16 sum_etfci;
  /*! \brief sum of PRRI field in E-AGCH, only count allocated TTI
  */
  int16 sum_prri;  
  /*! \brief current Pe-base value, unit of 1dBm
  */
  int8 pebase;
  /*! \brief number of TPC command is UP
  */
  uint8 num_tpc_up;
  /*! \brief number of TPC command is DOWN
  */
  uint8 num_tpc_down;
  /*! \brief number of DTX
  */
  uint8 num_dtx;
  /*! \brief sum of virtual unit, one code with spread factor SF
       equals to 16/SF virtual unit, count all codes in all E-PUCH timeslot 
       only count allocated TTI
  */
  uint16 sum_num_vu;
  /*! \brief number of HARQ process used for scheduled transmission which RTX_TIMER timeout
  */
  uint8 num_rtx_timeout;
  /*! \brief number of HARQ process used for scheduled transmission 
       which exceed max n transmission number
  */
  uint8 num_tx_num_exceed;
  /*! \brief Number of times the first retransmission was successfully
		 ACKed; enables to calculate BLER of first transmission
		 statistics
  */
  uint8 num_first_tx_acked;
  /*! \brief Number of TTIs transmission is limited by power counting
      only new transmissions
  */
  uint8 num_pwr_limited;
  /*! \brief Number of TTIs transmission is limited by serving grant
       counting only new transmissions
  */
  uint8 num_sg_limited;
  /*! \brief number of TTI when SI is transmitted alone in E-PUCH (E-TFCI=0),
       counting only new transmission
  */
  uint8 num_si_epuch;
  /*! \brief sum E-PUCH power in dBm
  */
  int16 sum_epuch_pwr;
  /*! \brief number of E-HICH losted
  */
  uint8 num_ehich_lost;
  /*! \brief sum of number of timeslot in E-AGCH serving grant, only count allocated TTI
  */
  uint8 sum_num_ts;
} tdsupalog_non_schld_tx_stats_struct_type;


/*! \brief      LOG PACKET: TDSCDMA_L1_UPA_L1_STATS_INFO_LOG_PACKET
				(LOG CODE: 0xD048)			

This structure is used to statistic HSUPA L1 working status, shows data thoughput
serving grant status, transmit power, etc.

This log package is generated every 1 second.
log package size is 12 + 4 + 68 + 56 = 140 BYTE, max bandwidth = 0.14 KB/s
*/
typedef struct{
  /* Log header */
  log_hdr_type header;

  /*! Version  */
  uint32 version:8;

  /*! reserved  */
  uint32 curr_pathloss:8;
  
  /*! is snpl optimization feature enabled  */
  uint32 snpl_opt_enabled:1;

  /*! current SNPL measurement result  */
  uint32 snpl:7;

  /*! scheduled transmission statistic infomation included flag  */
  uint32 schld_stats_included:4;

 /*! non-scheduled transmission statistic infomation included flag  */
  uint32 non_schld_stats_included:4;

  /*! \brief scheduled transmission L1 statistic info
  */
  tdsupalog_scheduled_tx_stats_struct_type  schld_stats_info;

  /*! \brief non-scheduled transmission L1 statistic info
  */
  tdsupalog_non_schld_tx_stats_struct_type  non_schld_stats_info;
} tdsupalog_l1_stats_pkt_type;


#define TDSEULLOG_UPA_L1_STATS_PKT_LEN ( sizeof(tdsupalog_l1_stats_pkt_type))


typedef struct{
  /*! \brief Sum of the number of transmitted bits per TTI,
       counting only new transmissions
  */
  uint32 upa_l1_tput;
  /*! \brief Sum of the number of transmitted bits per TTI,
       counting both new and retransmissions
  */
  uint32 sum_raw_bits;
  /*! \brief Sum of the number of bits that could have been transmitted
       per TTI based on available power
  */
  uint32 sum_pwr_bits;
  /*! \brief Average number of bits that could have been transmitted
        per TTI based on SG
  */
  uint32 sum_sg_bits;
} tdseullog_l1_stats_sample_struct_type;

extern tdsupalog_l1_stats_pkt_type tdseulcfg_l1_stats_log_pkt[2];
extern uint8 tdseulcfg_l1_stats_buf_idx;
extern uint8 tdseulcfg_l1_stats_submit_buf_idx;
extern uint32 tdseulcfg_l1_stats_log_pkt_sample_num[2];
extern boolean tdseulcfg_l1_stats_dirty_bit[2];
extern tdseullog_l1_stats_sample_struct_type tdseulcfg_l1_stats_schld_tx_sum_data[2];
extern tdseullog_l1_stats_sample_struct_type tdseulcfg_l1_stats_non_sch_tx_sum_data[2];

extern void tdseulcfg_e_ul_cleanup_log_pkts_data(void);
extern void tdseulcfg_send_etfc_restrict_info_to_diag(void);
extern void tdseulcfg_send_harq_db_log_pkt_to_diag(void);
extern void tdseulcfg_send_edl_dec_and_sg_log_pkt_to_diag(void);
extern void tdseulcfg_send_l1_stats_log_pkt_to_diag(void);


#define tdseullog_flush_logpkts()\
      tdseulcfg_send_upa_pwr_ctl_info_to_diag();\
      tdseulcfg_send_etfc_restrict_info_to_diag();\
      tdseulcfg_send_edl_dec_and_sg_log_pkt_to_diag();\
      tdseulcfg_send_l1_stats_log_pkt_to_diag();\
      tdseulcfg_e_ul_cleanup_log_pkts_data()
      

#endif /* TDSUPALOG_H */
