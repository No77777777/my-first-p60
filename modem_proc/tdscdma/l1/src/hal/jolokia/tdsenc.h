#ifndef TDS_ENC_H
#define TDS_ENC_H

/*============================================================================
              E N C O D E R   D R I V E R   H E A D E R   F I L E

DESCRIPTION
This files contains all necessary macros and definitions for interfacing with
the encoder hardware.

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2006-2011 Qualcomm Technologies, Incorporated.  All Rights Reserved.
============================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/l1/src/hal/jolokia/tdsenc.h#1 $ 
$DateTime: 2019/08/19 10:51:23 $ 
$Author: pwbldsvc $

when        who     what, where, why
--------    ---     --------------------------------------------------------
01/12/10    cdf     file created.

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "tdscdma_variation.h"
#include "customer.h"
#include "tdsuecomdef.h"
#include "queue.h"
#include "tdsl1macdata.h"
#include "tdsulutil.h"
#include "tdsl1mtask.h"
#include "tdsulstates.h"
#include "tdsenci.h"


#ifdef FEATURE_TDSCDMA_L1_LOGGING_ENABLED
#include "tdsullog.h"
#endif

#include "tfw_sw_intf_msg.h"

#ifdef FEATURE_TDSCDMA_PLT
#include "tdsenc_test.h"
#endif
#include "tdsl1m_rcinit.h"

#include "tdsuldpchctrl.h"

#include "tdsl1ulcfg.h"
/* ---------------------------- */
/* Check for necessary features */
/* ---------------------------- */

#if (!defined(FEATURE_TDSCDMA_PWR_TFC_ELMN) && defined(FEATURE_TDSCDMA_HSUPA))
#error "FEATURE_TDSCDMA_HSUPA requires FEATURE_TDSCDMA_PWR_TFC_ELMN"
#endif

/*===========================================================================

                                 TYPES

===========================================================================*/

typedef enum
{
  TDSENC_DEBUG_TX_PWR_DISABLE,
  TDSENC_DEBUG_DPCCH_PWR,
  TDSENC_DEBUG_RACH_PWR,
  TDSENC_DEBUG_BOTH_RACH_DPCCH_PWR
}tdsenc_debug_pwr_values_enum_type;


/* Encoder data flow control structure. Always store transport channels in
 * descending order of TTIs i.e 80, 40, 20 and then 10ms TTIs. This allows
 * for efficient computation of number of active transport channels at any
 * given time, we well as start address of free location in ERAMb. Note that
 * if we do not write to ERAMb in descending TTI order, then we will have to
 * manage ERAMb memory with holes in it. This will lead to inefficient usage
 * of free ERAM space. With the descending TTI scheme while writing to ERAMb,
 * we always have one free chunk in ERAMb memory whose depth is dependent on
 * expiry of the first TrCH with the largest TTI value.
 * For eg. If a 80 ms TrCH expires, then we have the whole of ERAMb available.
 * If we have one 80ms, 40ms and 20 ms TrCH each, the free pointer in ERAMb
 * will be equal to:
 * (a) the start address of the 20ms TrCH when CRF = 1
 * (b) the start address of the 40ms TrCH when CRF = 3
 * (c) 0 when CRF = 7
 */
typedef struct {
    /* TrCH id */
  uint8                 trch_id;
    /* TTI */
  tdsl1_tti_enum_type      tti;
    /* active ? - useful for debugging */
  boolean               active;
} tdsenc_eram_ctrl_struct_type;

#ifdef FEATURE_TDSCDMA_UL_MEMPOOL_LOGGING
/* UL MP log states */
typedef enum {
  TDSENC_MP_LOG_STATES_NULL = 0,
  TDSENC_MP_LOG_STATES_UPPCH,
  TDSENC_MP_LOG_STATES_PRACH,
  TDSENC_MP_LOG_STATES_DPCH,
  TDSENC_MP_LOG_STATES_ERUCCH,

  TDSENC_MP_LOG_STATES_MAX
} tdsenc_mp_log_state_enum_type;

#endif /* FEATURE_TDSCDMA_UL_MEMPOOL_LOGGING */

#ifdef TDSCDMA_FEATURE_DIME_DBG_UL_ENC
typedef enum {
  ENC_ENUM_TD_TX_FILLER_POLARITY = 0,
  ENC_ENUM_TD_TX_TRBLK_PARAM_TRCH,
  ENC_ENUM_TD_TX_ERAMB_ADDR_TRCH,
  ENC_ENUM_TD_TX_ERAMA_INCRACC_INIT,
  ENC_ENUM_TD_TX_ERAMA_INCRACC_3,
  ENC_ENUM_TD_TX_ERAMA_INCRACC_2,
  ENC_ENUM_TD_TX_ERAMA_INCRACC_1,
  ENC_ENUM_TD_TX_ERAMA_INCRACC_0,
  ENC_ENUM_TD_TX_NEW_FRAME_SW,
  ENC_ENUM_TD_TX_TRCH_MUX_ORDER_CCTRCH1,
  ENC_ENUM_TD_TX_TRCH_MUX_ORDER_CCTRCH2,
  ENC_ENUM_TD_TX_CODING_PARAM_TRCH,
  ENC_ENUM_TD_TX_CODE_SEG_SIZE_TRCH,
  ENC_ENUM_TD_TX_RM_EINI_TRCH,
  ENC_ENUM_TD_TX_RM_EMINUS_TRCH,
  ENC_ENUM_TD_TX_RM_EPLUS_TRCH,
  ENC_ENUM_TD_TX_TURBO_INTLV_PARAM0_TRCH,
  ENC_ENUM_TD_TX_TURBO_INTLV_PARAM1_TRCH,
  ENC_ENUM_TD_TX_ERAM_AB_TRCHS,
  ENC_ENUM_TD_TX_CCTRCH_ADDR,
  ENC_ENUM_TD_TX_SLOT_DATA_NUM0_CCTRCH1,
  ENC_ENUM_TD_TX_SLOT_DATA_NUM1_CCTRCH1,
  ENC_ENUM_TD_TX_SLOT_DATA_NUM2_CCTRCH1,
  ENC_ENUM_TD_TX_SLOT_DATA_NUM0_CCTRCH2,
  ENC_ENUM_TD_TX_SLOT_DATA_NUM1_CCTRCH2,
  ENC_ENUM_TD_TX_SLOT_DATA_NUM2_CCTRCH2,
  ENC_ENUM_TD_TX_CCTRCH_SI,
  ENC_ENUM_TD_TX_OFFL_ENC_FRAME,
  ENC_ENUM_TD_TX_ENC_TIMING_CTL_DCH_RACH,
  ENC_ENUM_TD_TX_RM_EINI_ERUCCH,         
  ENC_ENUM_TD_TX_RM_EMINUS_ERUCCH,  
  ENC_ENUM_TD_TX_RM_EPLUS_ERUCCH,     
  ENC_ENUM_TD_TX_ENC_TIMING_CTL_ERUCCH, 
  ENC_ENUM_TD_TX_ERUCCH_DATA0,           
  ENC_ENUM_TD_TX_ERUCCH_DATA1,           
  ENC_ENUM_TD_TX_TTI_ERUCCH,            
  ENC_ENUM_TD_TX_ERUCCH_ADDR,            
  ENC_ENUM_TD_TX_ENABLE,                
  ENC_ENUM_TD_TX_MODE,                   
  ENC_ENUM_TD_TX_CTRL,                   
  ENC_ENUM_TD_TX_DAC_OUTPUT_SEL,         
  ENC_ENUM_TD_TX_MEM_ACCESS_CFG,         
  ENC_ENUM_TD_TX_TXC_VALID_FALL_TIME,    
  ENC_ENUM_TD_TX_ENDIAN_FRMT_CTL,        
  ENC_ENUM_TD_TX_CIPHER_CK,            
  ENC_ENUM_TD_TX_MAX
} tdsenc_enc_program_log_reg_enum;

typedef struct 
{
  tdsenc_enc_program_log_reg_enum reg;

  uint8  num_val;
  uint32 val_1;
  uint32 val_2;
} tdsenc_enc_program_log_type;

#define TDSENC_ENC_PROGRAM_MAX_CNT 70
extern uint8 tdsenc_enc_program_log_cnt;
extern tdsenc_enc_program_log_type tdsenc_enc_program_log_buf[];

#endif  // TDSCDMA_FEATURE_DIME_DBG_UL_ENC
/*==========================================================================

                         CONCURRENCY CONTROL PRIMITIVES

===========================================================================*/


/*definitions for 2nd interlv/physical channel mapping caculations on Triton*/
typedef struct
{
    /*last colum index, index start from 0*/
    uint8   last_colum_index;
    /*num of rows, min value is 1*/
    uint16 num_rows;
}tdsenc_2nd_intlv_param_type;


typedef struct
{
    int16 row;
    int16 column;
}tdsenc_2nd_intlv_matrix_position_type;

typedef struct
{
    tdsenc_2nd_intlv_matrix_position_type start;
    tdsenc_2nd_intlv_matrix_position_type last;
}tdsenc_2nd_intlv_position_type;

typedef struct
{
    tdsenc_2nd_intlv_position_type slot_sfn_pos;
    tdsenc_2nd_intlv_position_type slot_subsfn0_pos;
    tdsenc_2nd_intlv_position_type slot_subsfn1_pos;
}tdsenc_slot_subsfn_segement_params_type;

typedef struct
{
    tdsenc_slot_subsfn_segement_params_type slot[TDSL1_UL_MAX_NUM_TS];
}tdsenc_subsfn_segement_params_type;

typedef struct
{
    tdsenc_2nd_intlv_matrix_position_type ch0_start_pos;
    tdsenc_2nd_intlv_matrix_position_type ch0_last_pos;
    uint16  ch0_num_jump;
    uint16  ch0_num_bits_last_jump;
}tdsenc_phy_chan0_mapping_params_type;

typedef struct
{
    tdsenc_2nd_intlv_matrix_position_type ch1_start_pos;
    tdsenc_2nd_intlv_matrix_position_type ch1_last_pos;
    uint16 ch1_num_bits_before_first_jump;
    uint16 ch1_num_bits_first_jump;
}tdsenc_phy_chan1_mapping_params_type;

typedef struct
{
    tdsenc_phy_chan0_mapping_params_type ch0_subsfn0[TDSL1_UL_MAX_NUM_TS];
    tdsenc_phy_chan0_mapping_params_type ch0_subsfn1[TDSL1_UL_MAX_NUM_TS];
    tdsenc_phy_chan1_mapping_params_type ch1_subsfn0[TDSL1_UL_MAX_NUM_TS];
    tdsenc_phy_chan1_mapping_params_type ch1_subsfn1[TDSL1_UL_MAX_NUM_TS];
}tdsenc_phy_chan_mapping_params_type;

/* for tdsenc dynamic memory allocation */
#define TDSENC_2NDXLVR_COLS 30

/* waiting count for FW tx block status */
#define MAX_FW_TX_HW_BLOCK_ACCESS_WAIT_CNT 20

#ifdef TDSCDMA_FEATURE_UL_HWRAM_DBG
#define ERAMB_DUMP_BUF_SIZE  10
#define RMRAM_DUMP_BUF_SIZE  10
#define INVALID_FRAME_NUM    0xFFFF
#endif /* TDSCDMA_FEATURE_UL_HWRAM_DBG */

typedef struct
{
  /* uplink physical channel data buffer shared with L2 */
  tdsl1_ul_tx_data_type tdsenc_l1_ul_tx_buf;

  /* UL Phychan drop pending flag */
  boolean tdsenc_drop_pending_flg;

  /* Encoder ERAM control buffer */
  tdsenc_eram_ctrl_struct_type tdsenc_eram_ctl_buf[TDSUE_MAX_TRCH];
  uint8 tdsenc_eram_ctl_buf_idx;

  /* number of TrCh's from MAC that found matching TTI boundary
   * Used to catch MAC TFCI selection error regarding TTI boundary */
  uint8 tdsenc_num_trch_tti_matched;

  uint8 tdsenc_num_active_trchs;

  /* ERAMb start address indexed by TrCH id */
  uint16  tdsenc_eramb_start_addr[TDSUE_MAX_TRCH];

  /* TFC shadow - always updated whenever new TrCHs are provided by MAC */
  tdsl1_ul_trch_state_enum_type tdsenc_tfc[TDSUE_MAX_TRCH];

  /* List of ERAMa TrCH ids */
  uint8 tdsenc_new_erama_trch_list[TDSUE_MAX_TRCH];

  /* Map of TRCH_CFG0_RAM index to actual TrCH ids.  The ENC driver stores TrCH
   * parameters that are used to perform TB concatenation by hw in TrCH_CFG0_RAM.
   * The actual data that needs to transferred from ERAMa to ERAMb is written
   * later into ERAMa (by ENC driver). The order in which TrCHs need to be
   * written is descending TTI order and not the order in which we see TrCHs in
   * Tx buffer filled by MAC.
   */
  uint8 tdsenc_trch_cfg0_idx[TDSUE_MAX_TRCH];
  uint8 tdsenc_cfg0_idx;  // TRCH_CFG0_RAM index

  /* Free location in ERAMb to store data from the next TrCH */
  uint16 tdsenc_eramb_free_addr;

  /* This value is used to save the pathloss sent at last time */
  uint16 tdsenc_pre_pathloss_q8;

  /* This variable is used to set the endian format for R4 UL and HSUPA channel.
   * bit0 is used for R4 UL channel: 0-big endian, 1-little endian
   * bit1 is used for HSUPA:         0-big endian, 1-little endian
   */
  uint32 tdsenc_endian_frmt;

  #ifdef FEATURE_TDSCDMA_UL_MEMPOOL_LOGGING
  /*  Mempool logging data structures  */
  tfw_tx_mp_log_config_cmd_t     tdsenc_mp_cmd; 
  boolean tdsenc_mp_log_cfg_enableFlag;
  int32   tdsenc_mp_log_cfg_nSubframes;
  uint16  tdsenc_mp_log_cfg_nSamples;
  uint8   tdsenc_mp_log_cfg_logPoint;
  boolean tdsenc_mp_log_cfg_crashFlag;
  uint8   tdsenc_mp_log_cfg_startsf_offset;

  /* UL state condition to send mempool logging request */
  boolean tdsenc_mp_log_uppch_f;
  boolean tdsenc_mp_log_prach_f;
  boolean tdsenc_mp_log_dpch_f;   
  boolean tdsenc_mp_log_erucch_f;

  /* Current count */
  uint16  tdsenc_mp_log_uppch_cnt;
  uint16  tdsenc_mp_log_prach_cnt;
  uint16  tdsenc_mp_log_dpch_cnt;
  uint16  tdsenc_mp_log_erucch_cnt;

  /* Count condition to send mempool logging request */
  uint16  tdsenc_mp_log_uppch_cnt_to_send;
  uint16  tdsenc_mp_log_prach_cnt_to_send;
  uint16 tdsenc_mp_log_dpch_cnt_to_send;
  uint16 tdsenc_mp_log_erucch_cnt_to_send;

  uint16 tdsenc_mp_prach_beta;
  uint16 tdsenc_mp_dpch_beta;
  uint16 tdsenc_mp_erucch_beta;
  #endif /* FEATURE_TDSCDMA_UL_MEMPOOL_LOGGING */

  tdsenc_2nd_intlv_param_type tdsenc_2ndIntlv_slot_based_param[TDSL1_UL_MAX_NUM_TS];
  tdsenc_2nd_intlv_param_type tdsenc_2ndIntlv_frame_based_param;

  /*sub frame segement params*/
  tdsenc_subsfn_segement_params_type tdsenc_2nd_intlv_subsfn_segement_params;
  /*physical channel mpping params*/
  tdsenc_phy_chan_mapping_params_type tdsenc_phy_chan_mapping_params;

  #ifdef TDSCDMA_FEATURE_UL_HWRAM_DBG
  int16  tdsenc_r4_ram_logging_frame_num;
  int16  tdsenc_erucch_ram_logging_frame_num;
  #endif /*TDSCDMA_FEATURE_UL_HWRAM_DBG*/

}tdsenc_global_struct_type;

extern tdsenc_global_struct_type *tdsenc_global;  

extern boolean tdsl1_ul_pwr_stats_tx_enable; 

/*===========================================================================

                                 FUNCTIONS

===========================================================================*/


/*=========================================================================

FUNCTION WL1_ENC_INIT

DESCRIPTION
  This routine intializes the encoder driver. It initializes the TrCH mux
  order free queue and the TrCH mux queue.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=========================================================================*/
void tdsenc_init
(
  void
);

/*===========================================================================
FUNCTION tdsenc_set_drop_pending_flg

DESCRIPTION
  This function is used by other module to set tdsenc_drop_pending_flg 
  variable. 
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void tdsenc_set_drop_pending_flg(boolean drop_pending_flg);


/*===========================================================================
FUNCTION tdsenc_get_drop_pending_flg

DESCRIPTION
  This function is used by other module to get tdsenc_drop_pending_flg 
  variable. 
  
DEPENDENCIES
  None

RETURN VALUE
  tdsenc_drop_pending_flg

SIDE EFFECTS
  None
===========================================================================*/

boolean tdsenc_get_drop_pending_flg(void);


/*=========================================================================

FUNCTION ENC_GET_DATA

DESCRIPTION
  This function processes tx data delivered by MAC. It sets up
  appropriate encoder control registers and transfers tx data into ERAMa.
  It sets up TRCH_CFG_0_RAM, TRCH_CFG_1_RAM and TRCH_CTL_RAM as well as the
  number of active/erama transport channels. It calculates RM parameters, SF
  and Gain Factors. It has be done with all its processing within 6ms into the
  frame leaving the encoder at least 4 ms to cipher/encode ERAMa data.

DEPENDENCIES
  This function depends on MAC to have successfully built tx data into the
  Tx buffer before the GET_DATA event.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=========================================================================*/
void tdsenc_get_data
(
  void
);

/*=========================================================================

FUNCTION tdsenc_cleanup_response_to_l1m

DESCRIPTION
  This function send response to L1M if the channel is drop pending.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=========================================================================*/
void tdsenc_cleanup_response_to_l1m
(
  tdsl1_ul_phychan_enum_type ul_phychan
);

/*===========================================================================
FUNCTION ENC_SET_CIPHERING_KEYS

DESCRIPTION
  This function updates the ciphering key for uplink ciphering operation
  to the ciphering key index locations requested.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
==========================================================================*/
void tdsenc_set_ciphering_key(
  /* ciphering key index (one of the 4 keys) to which the key is set */
  uint8 index,
  /* ciphering key info. Pointer to the array of 4 word32 (= 128 bit) */
  uint8 *ciphering_key_data);

/*===========================================================================
FUNCTION tdsenc_prach_encoding_init

DESCRIPTION
  This function initialize some encoding control information that would be
  used for PRACH transmission. 

DEPENDENCIES
  This function is invoked one time before start PRACH ENC programming

RETURN VALUE
  None

SIDE EFFECTS
  None
==========================================================================*/
void tdsenc_prach_encoding_init(void);


/*===========================================================================
FUNCTION tdsenc_dpch_encoding_init

DESCRIPTION
  This function initialize some encoding control information that would be
  used for DPCH transmission. 

DEPENDENCIES
  This function is invoked one time before start PRACH ENC programming

RETURN VALUE
  None

SIDE EFFECTS
  None
==========================================================================*/
void tdsenc_dpch_encoding_init(void);



/*===========================================================================
FUNCTION 
  tdsenc_tx_reg_init

DESCRIPTION
  This function initializes HW registers when L1 task is first created, such as 
  enable TX, Enable TDS feature, set little endian format, initialize ERAMA 
  buffer, etc.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
==========================================================================*/
void tdsenc_tx_reg_init(void);

/*===========================================================================
FUNCTION 
  tdsenc_set_endian_frmt

DESCRIPTION
  This function support set endian format in HW register for PRACH&DPCH and 
  HSUPA channel seperately.
  bit0 is used for R4 UL channel: 0-big endian, 1-little endian
  bit1 is used for HSUPA:         0-big endian, 1-little endian

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
==========================================================================*/

void tdsenc_set_endian_frmt(uint32 endian_frmt);


/*=========================================================================

FUNCTION tdsenc_program_enc_for_r4_chan

DESCRIPTION
  This function is used to program HW ENC, including CFG0_RAM, CFG1_RAM, RMRAM,
  ERAMA, ERAMB address and other control registers. This function will be called
  each time when transmit PRACH or DPCH data.

DEPENDENCIES
  This function depends on MAC to have successfully built tx data into the
  Tx buffer and L1SW plan to send this data at the next frame.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=========================================================================*/
void tdsenc_program_enc_for_r4_chan
(
  uint8 num_trchs, 
  uint8 frame_cnt, 
  uint8 num_active_trchs, 
  tdsl1_ul_filler_polarity_enum_type filler_val
);


#ifdef FEATURE_TDSCDMA_HSUPA
/*=========================================================================

FUNCTION tdsenc_program_enc_for_erucch

DESCRIPTION
  This function is used to program HW ENC for E-RUCCH only. This function will 
  be called each time when transmit E-RUCCH data.

DEPENDENCIES
  This function depends on FPACH received successfully and data got from EUL 
  module.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=========================================================================*/
void tdsenc_program_enc_for_erucch
(
  uint8 *erucch_data, 
  uint8  erucch_tti
);

#endif /* FEATURE_TDSCDMA_HSUPA */


/*=========================================================================

FUNCTION tdsenc_update_expired_trchs

DESCRIPTION
  This function free expired TrCHs previously got from MAC. Base on the first 
  TrCH in descending TTI order that has expired, update the start position of 
  ERAMb free address.

DEPENDENCIES
  None.
  
RETURN VALUE
  None.

SIDE EFFECTS
  None.

=========================================================================*/
void tdsenc_update_expired_trchs(tdsl1_tti_enum_type exp_tti);


/*=========================================================================

FUNCTION tdsenc_update_trch_active_status

DESCRIPTION
  This function updates each TrCH status base on DPCH data got from MAC. Update
  ERAM control variables accordingly.
  Check whether the number of TRCH is consistent with TRCH data really transmitted.

DEPENDENCIES
  UL got DPCH data from MAC.
  
RETURN VALUE
  FALSE means error exist in MAC data. TRUE otherwise.

SIDE EFFECTS
  None.

=========================================================================*/
boolean tdsenc_update_trch_active_status(uint8 *num_erama_trchs_ptr);

/*=========================================================================

FUNCTION tdsenc_update_slotusage_for_meas

DESCRIPTION
  This function set uplink slotusage as the struct defined by MEAS.
  MEAS uses the information to identify idle slots and schedule
  measurements.

DEPENDENCIES
  tdsulra_uppch_fpach_config, tdsulra_prach_config and tdsuldpchctrl_hist_config_cmd
  is set as latest command before this function is called with action
  TDSL1_UL_MEAS_SU_XXX_SET.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=========================================================================*/
void tdsenc_update_slotusage_for_meas(uint16 action_bitmask);


/*=========================================================================

FUNCTION tdsenc_fill_eram_ctl_buf

DESCRIPTION
  This function fills the encoder ERAMb TrCH order buffer with TrCH info
  for the specified TTI.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
void tdsenc_fill_eram_ctl_buf
(
  uint8            num_erama_trchs,
  tdsl1_tti_enum_type tti_val
);


/*=========================================================================

FUNCTION tdsenc_ul_create_cmd_msg

DESCRIPTION
  This function creates uplink message with the appropriate header to be sent 
  to the MSGR interface

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=========================================================================*/
void tdsenc_ul_create_cmd_msg(
  /* Message header */
  msgr_hdr_struct_type* hdr, 
  /* Header UMID */
  msgr_umid_type umid
);


/*=========================================================================

FUNCTION tdsenc_ul_create_cmd_msg

DESCRIPTION
  This function is the wrapper function for msgr_send which sends uplink message 
  to the MSGR interface with the header and payload

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=========================================================================*/
void tdsenc_ul_send_cmd_msg(
  /* Message to send */
  msgr_hdr_struct_type* hdr,
  /* Size of the message */
  uint32 msg_size
);

#ifdef FEATURE_TDSCDMA_UL_MEMPOOL_LOGGING
/*=========================================================================

FUNCTION tdsenc_send_tx_mp_log

DESCRIPTION
  This function sets parameters for MP config command and sends the command
  if it meets the sending condition.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=========================================================================*/ 
void tdsenc_send_tx_mp_log (
  tdsenc_mp_log_state_enum_type state, uint16 nStartSf, uint16 nSamplesStartOffset);

/*===========================================================================
FUNCTION        tdsenc_send_tx_sw_plt_cfg

DESCRIPTION     set the tx sw/fw ptl cfg 

DEPENDENCIES    The command parameters

RETURN VALUE    None

SIDE EFFECTS    None.
===========================================================================*/
void tdsenc_send_tx_sw_plt_cfg(tdsenc_mp_log_state_enum_type cphy);
#endif

/*=========================================================================
FUNCTION        tdsenc_send_tx_hw_block_config

DESCRIPTION     This function sets and sends TDSCDMA_FW_TX_HW_BLOCK_CONFIG.

DEPENDENCIES    

RETURN VALUE    None.

SIDE EFFECTS    None.
=========================================================================*/
void tdsenc_send_tx_hw_block_config(boolean en_dis_flag);

/*=========================================================================
FUNCTION        tdsenc_send_tx_hw_block_disable_cmd_and_wait_cnf

DESCRIPTION     This function sends tx hw block disable command and 
                waits until FW provides an indication via shared memory
                flag that FW completed Tx diabaling.
                
DEPENDENCIES    

RETURN VALUE    None.

SIDE EFFECTS    None.
=========================================================================*/
void tdsenc_send_tx_hw_block_disable_cmd_and_wait_cnf(void);



#ifdef TDSCDMA_FEATURE_UL_HWRAM_DBG
/*=========================================================================
FUNCTION tdsenc_encoding_time_stamp

DESCRIPTION
  Read time stamp.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.
=========================================================================*/
void tdsenc_get_time_stamp(uint16 *ssfn, uint16 *chipx8);

/*=========================================================================
FUNCTION tdsenc_r4_eramb_rmram_dump

DESCRIPTION
  Read eramb and rmram.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.
=========================================================================*/
void tdsenc_erucch_eramb_rmram_dump(void);


/*=========================================================================
FUNCTION tdsenc_erucch_eramb_rmram_dump

DESCRIPTION
  Read eramb and rmram.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.
=========================================================================*/
void tdsenc_r4_eramb_rmram_dump(void);

/*=========================================================================
FUNCTION tdsenc_r4_ul_hw_logging

DESCRIPTION
  perform r4 ul hw logging.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.
=========================================================================*/
void tdsenc_r4_ul_hw_logging(void);

/*=========================================================================
FUNCTION tdsenc_erucch_ul_hw_logging

DESCRIPTION
  perform erucch ul hw logging.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.
=========================================================================*/
void tdsenc_erucch_ul_hw_logging(void);
#endif /*TDSCDMA_FEATURE_UL_HWRAM_DBG*/


/*===========================================================================
FUNCTION    
  tdsenc_alloc_dynamic_mem
DESCRIPTION
  when receive TDSL1_CPHY_START_TDSCDMA_MODE_REQ, L1M start to malloc memory for L1 modules, and initialize them.
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/
void tdsenc_alloc_dynamic_mem(void);

/*===========================================================================

FUNCTION    
  tdsenc_free_dynamic_mem
DESCRIPTION
  when receive TDSL1_CPHY_STOP_TDSCDMA_MODE_REQ, L1M start to free memory for L1 modules.
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/
void tdsenc_free_dynamic_mem(void);


#endif /* TDS_ENC_H */