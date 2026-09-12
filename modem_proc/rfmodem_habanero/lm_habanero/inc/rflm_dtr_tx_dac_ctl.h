#ifndef RFLM_DTR_TX_DAC_CTL_H
#define RFLM_DTR_TX_DAC_CTL_H

/*!
   @file
   txlm_hal.h

   @brief
   This file implements the TXLM MDM9K HAL functionality. These are platform
   dependent.

*/

/*===========================================================================

Copyright (c) 2010 - 2015 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$DateTime: 2021/03/09 05:29:45 $ $Author: pwbldsvc $
$Header: //components/rel/rfmodem_habanero.mpss/1.11/lm_habanero/inc/rflm_dtr_tx_dac_ctl.h#1 $

when       who     what, where, why
------------------------------------------------------------------------------- 
02/24/16   jhap    removed the compilation warning .
02/16/15   jhap      Added API to check if current modem requires fuse based BBRx compensation 
12/22/15   jhap    Added Dac bringup changes for Jcala.
06/29/15   dej     Add Mutex Locking for Shared registers 
06/25/15   lm      Added TxDAC msb cal codes dump API 
06/22/15   lm      Added chipset read API for DAC cal params override  
06/10/15   lm      Added chipset enum read API  
04/22/15   dr      Adding target specific apis to target folder. Including prototypes here. 
04/13/15   lm      Added empty DAC reprogram API interface to avoid compiling errors 
01/08/14   kai     Update ET DAC config settings
12/20/13   cri     Changes for LTE digital modem settings
12/09/13   st      DAC Mission Mode: Restore TXDAC Mode for Companion
11/06/13   vrb     Fixes for compiler warnings
10/29/13   st      DAC Sleep Wakeup
10/29/13   st      Add IREF Update Latch for Bootup
10/29/13   st      Remove JTAG DBG DAC Disable
10/29/13   st      DAC Bootup Sequence
10/25/13   st      Fix Configuration codes
10/25/13   cvd     DAC IREF caliberation support 
10/16/13   st      Initial version.
============================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/

#ifdef _cplusplus
extern "C" {
#endif

#include "txlm_intf.h"
#include "rflm_dtr_tx_typedef_ag.h"
#include "rflm_features.h"
#if (RFLM_FEATURE_BUILD_MODE!=RFLM_FEATURE_BUILD_FW_LIB)
#include "rfcom.h"
#include "rflm_api_cmn.h"
#endif
#define DAC_PWR_ON_CLK_DIV_PGM		0
#define DAC_PWR_ON_CLK_PLL_SEL		0
#define DAC_PWR_ON_CLK_DAC1_CLK_SRC	0


/* Section 4.1, Table 4.1 of Jacala Programming guide tx_cfg_3_lp */

#define DAC_PWRUP_TXDAC_CFG0   0x02A2211B
#define DAC_PWRUP_TXDAC_CFG1   0x8004D140
#define DAC_PWRUP_TXDAC_CFG2   0xF1E405AE
#define DAC_PWRUP_TXDAC_CFG3	 0x0C



/* Section 4.1, Table 4.1 of Jacala Programming guide tx_cfg_4_lp */
#define DAC_MSBCAL_TXDAC_CFG0  0x02A2211B
#define DAC_MSBCAL_TXDAC_CFG1  0x8000D144
#define DAC_MSBCAL_TXDAC_CFG2  0xF1E405AE
#define DAC_MSBCAL_TXDAC_CFG3  0x0C

#define DAC_MSBCAL_REGARRAY_MASK 0x3F
#define DAC_MSBCAL_REGARRAY_MASK_0 	0x3F
#define DAC_MSBCAL_REGARRAY_MASK_1 	0xFC0
#define DAC_MSBCAL_REGARRAY_MASK_2 	0x3F00
#define DAC_MSBCAL_REGARRAY_MASK_3 	0xFC000
#define DAC_MSBCAL_REGARRAY_SHFT_0 0
#define DAC_MSBCAL_REGARRAY_SHFT_1 6
#define DAC_MSBCAL_REGARRAY_SHFT_2 12
#define DAC_MSBCAL_REGARRAY_SHFT_3 18

#define DAC_DCCAL_REGARRAY_MASK 0x7F
#define DAC_DCCAL_REGARRAY_MASK_0 	0x7F
#define DAC_DCCAL_REGARRAY_MASK_1 	0x3F80
#define DAC_DCCAL_REGARRAY_SHFT_0 0
#define DAC_DCCAL_REGARRAY_SHFT_1 7

#define TXDAC_MSBCAL_SIZE 148
#define TXDAC_DC_CAL_SIZE 2
#define TXDAC_CAL_DATA_SIZE (TXDAC_MSBCAL_SIZE+TXDAC_DC_CAL_SIZE)

#define DAC_FCAL_CLK_10_CYCLES							1
#define DAC_PWR_ON_TX_ALL_DONE_WAIT_US					100  /* Actual 11us - Safe Margin added */
#define TXDAC_PWR_ON_MSBCAL_MEM_WR_DONE_WAIT_US			100	/* actual 568 XO Cycles - Safe Margin added */
#define ETDAC_PWR_ON_MSBCAL_MEM_WR_DONE_WAIT_US			100  /* Actual 284 XO Cycles - Safe Margin added */
#define TXDAC_PWR_ON_DCCAL_MEM_WR_DONE_WAIT_US			100	/* Actual 8 XO Cycles - Safe Margin added */


#define GNSS_OFFSET_GSM 88474 
#define GNSS_OFFSET_TDS 55680
#define GNSS_OFFSET_LTE 153600
#define GNSS_BLANKING_NULL_ON 0x4000 /* set bit 14  LTE bands 13 and 14 on case */ 
#define GNSS_BLANKING_NULL_OFF 0x8000 /* set bit 15 for LTE bands 13 and 14 off case*/ 
#define GNSS_STMR_MASK 0xFFFFFF 
#define GNSS_BLANKING_RESET 0x0 /* to reset the registers value to 0*/
#define GNSS_NAV_CTL_L1 0x2
#define GNSS_NAV_CTL_L5 0x10
#define GNSS_NAV_CTL_L1_L5 0x12


extern void rflm_dtr_tx_shared_reg_lock_init(void);
extern void rflm_dtr_tx_shared_reg_lock();
extern void rflm_dtr_tx_shared_reg_unlock();
void rflm_dtr_remove_hwdbg_flags(void);
void rflm_dtr_tx_enable_xogate(void);
void rflm_dtr_tx_disable_xogate(void);
void rflm_dtr_tx_enable_fcal_clk(void);
void rflm_dtr_tx_disable_fcal_clk(void);
void rflm_dtr_tx_set_dac_clk_src_xo(void);
void rflm_dtr_tx_set_dac_clk_src_dtr(void);
void rflm_dtr_tx_txdac_enable_rt_cal_clk(txlm_chain_type chain);
void rflm_dtr_tx_txdac_disable_rt_cal_clk(txlm_chain_type chain);
void rflm_dtr_tx_enable_dac_clk_cgc(void);
void rflm_dtr_tx_txdac_pwr_on_set_cfg(txlm_chain_type chain);
void rflm_dtr_tx_txdac_msbcal_set_cfg(txlm_chain_type chain);
void rflm_dtr_tx_txdac_pwr_on_restore_fusecode(txlm_chain_type chain, uint32 data);
void rflm_dtr_tx_set_rtune_range_bit(txlm_chain_type chain, uint32 rtune_range_bit);
void rflm_dtr_tx_set_avg_err_bit(txlm_chain_type chain, uint32 avg_err_bit);
void rflm_dtr_tx_txdac_pwr_on_set_mode(txlm_chain_type chain);
void rflm_dtr_tx_txdac_factory_cal_set_mode(txlm_chain_type chain);
void rflm_dtr_tx_dac_factory_cal_clear_mode(txlm_chain_type chain);
void rflm_dtr_tx_txdac_pwr_on_pwr_on_reset(txlm_chain_type chain);
void rflm_dtr_tx_txdac_load_regarray(txlm_chain_type chain, uint8* data);
void rflm_dtr_tx_txdac_read_msbcal_from_regarray(txlm_chain_type chain,uint32 mask,uint8 size,uint32 * data);
void rflm_dtr_tx_txdac_read_dccal_from_regarray(txlm_chain_type chain,uint32 mask,uint32 * data);
void rflm_dtr_tx_etdac_read_msbcal_from_regarray(txlm_chain_type chain,uint32 mask,uint8 size,uint32 * data);
void rflm_dtr_tx_txdac_pwr_on_read_all_done(txlm_chain_type chain, uint32* data);
void rflm_dtr_tx_dac_factory_cal_msbcal_mem_rd(txlm_chain_type chain);
void rflm_dtr_tx_dac_factory_cal_msbcal_mem_rd_done(txlm_chain_type chain, uint32* data);
void rflm_dtr_tx_dac_pwr_on_msbcal_mem_wr(txlm_chain_type chain);
void rflm_dtr_tx_dac_pwr_on_msbcal_mem_wr_done(txlm_chain_type chain, uint32* data);
void rflm_dtr_tx_txdac_pwrdn(txlm_chain_type chain);
void rflm_dtr_tx_etdac_pwrdn(txlm_chain_type chain);
void rflm_dtr_tx_txdac_en(txlm_chain_type chain);
void rflm_dtr_tx_etdac_en(txlm_chain_type chain);
void rflm_dtr_tx_start_dac_clk(uint8 dac_div_pgm);
void rflm_dtr_tx_read_qfuse(txlm_chain_type chain, uint32* qfuse_rd);
void rflm_dtr_tx_read_overflow_bit(txlm_chain_type chain, uint32* overflow_bit);
void rflm_dtr_tx_read_0_1_fuseflag_bit(uint32* fuseflag_bit);
void rflm_dtr_tx_read_avg_error(txlm_chain_type chain, uint32* avg_error);
void rflm_dtr_tx_read_dac_status_tx_all_done(txlm_chain_type chain,uint32* data);
void rflm_dtr_tx_read_dac_status_msbcal_wr_done(txlm_chain_type chain,uint32* data);
void rflm_dtr_tx_read_dac_status_dccal_wr_done(txlm_chain_type chain,uint32* data);
void rflm_dtr_tx_read_dac_status_i_q_en(txlm_chain_type chain,uint32* data);
void rflm_dtr_tx_set_manual_iref_update(txlm_chain_type chain);
void rflm_dtr_tx_clear_manual_iref_update(txlm_chain_type chain);
void rflm_dtr_tx_dac_power_up_clk_enable(void);
void rflm_dtr_tx_txdac_power_up(txlm_chain_type chain);
void rflm_dtr_tx_txdac_power_down(txlm_chain_type chain);
void rflm_dtr_tx_txdac_reactivate(txlm_chain_type chain, boolean blocking);
void rflm_dtr_tx_dac_iref_cfg(txlm_chain_type chain, const uint32* data, 
  uint32 iref_update_start_time,
  uint32 iref_update_stop_time );
void rflm_dtr_tx_read_dac_cfg(txlm_chain_type chain,uint32* data);
void rflm_dtr_tx_foundry_init_ag ( uint16 foundry_id );

void rflm_dtr_tx_read_dac_status_et_all_done(txlm_chain_type chain,uint32* data);

void rflm_dtr_restore_ave_error_and_overflow_bit(void);
boolean rflm_dtr_tx_txdac_reprogram_msbcal_code(txlm_chain_type chain);
void rflm_dtr_tx_dac_extract_msb_calcodes(txlm_chain_type chain,uint8 size,uint32* cal_data);

void rflm_dtr_tx_msb_cal_code_dump();
void rflm_dtr_tx_txdac_msb_reprogram_set_calib(txlm_chain_type chain);

boolean rflm_dtr_tx_and_et_dac_reprogram_msbcal_code(txlm_chain_type chain, boolean etdac_setup);
uint32 rfmodem_target_get_chipset_id(void);
void rflm_dtr_tx_dac_iref
(
  txlm_chain_type chain,
  boolean update_en,
  uint32 iref_idx,
  uint32 iref_update_start,
  uint32 iref_update_stop
);



/* enum to decide the band to apply blanking*/
typedef enum 
{
  GNSS_L1,
  GNSS_L5,
  GNSS_L1_L5
}gnss_blank_band_type;

#if (RFLM_FEATURE_BUILD_MODE!=RFLM_FEATURE_BUILD_FW_LIB)
/*==============================================================================

FUNCTION:  rflm_dtr_tx_gnss_blanking

@brief
API to perform GNSS blanking

@Input parameters
Handle , blanking on/off , ustmr time , gnss band to be blanked , LTE band info 

==============================================================================*/

rflm_err_t rflm_dtr_tx_gnss_blanking(rflm_handle_tx_t handle, boolean blank_on ,uint32 stmr_time ,rflte_mc_ext_gnss_blanking_type gnss_band , rfcom_lte_band_type 
band_info);
void rflm_dtr_gnss_blanking_ctl_reset(void);
#endif

boolean rfmodem_target_fuse_bbrx_compensation_needed(void);


#ifdef _cplusplus
}
#endif

#endif /* RFLM_DTR_TX_DAC_CTL_H */

