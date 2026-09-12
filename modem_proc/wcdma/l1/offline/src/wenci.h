#ifndef WENCI_H
#define WENCI_H

/*============================================================================
              E N C O D E R   D R I V E R   I N T E R N A L
                          H E A D E R   F I L E

DESCRIPTION
This files contains all necessary macros and definitions for interfacing with
the encoder hardware.

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2000,2012 Qualcomm Technologies, Inc.  All Rights Reserved.
============================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/l1/vcs/enci.h_v   1.11   01 May 2002 09:20:04   yshi  $
$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/src/wenci.h#3 $ $DateTime: 2020/01/22 08:56:27 $ $Author: pwbldsvc $

when        who     what, where, why
--------    ---     --------------------------------------------------------
08/01/15    vr      Changes and fixes needed for ATLAS RUMI.
07/07/15    yw      Create new file for HWIO interface access (UL part)
01/16/15    vr      Global reorg for W + W.
06/03/14    tsk     Compilation fixes for JOLOKIA 1.0
05/22/14    tsk     Featurization for JOLOKIA 1.0
05/13/14    ymu     Feature name FEATURE_WCDMA_DC_HSUPA_FRAME replacement with FEATURE_WCDMA_DC_HSUPA 
03/31/14    ar      Removed legacy macros which are not applicable to Bolt
02/14/14    ar      Porting Bolt specific uplink changes on to Bolt 2.0
12/20/13    gp      eramb_rd_start_addr should be limited to 10 bits.
05/22/13    oh      Adding DC-HSUPA feature framework support.
05/09/12    ar      BOLT Compilation changes
05/06/13    at      SW Workaround for TXHW-A2 issue and cleanup FEATURE_WCDMA_NIKEL_ERAMA_ACCESS_FIX
04/04/13    at      Cleanup FEATURE_WCDMA_TRITON_MCAL_API featurization
09/11/12    ash     Featurized HW Reg Operations for Triton Compilation
08/10/12    at      Changes for Dime RUMI Tx bring-up
07/24/12    geg     Upmerge WCDMA.MPSS.1.0 to WCDMA.MPSS.2.0
06/23/12    at      Changes for Dime RUMI Tx bring-up
02/08/12    vs      Feature cleanup.
01/31/12    raj     feature cleanup
07/18/11     sp     Added support to use AHB interface for R99 ERAMA WRITE
03/30/11    gnk     Mainlined several features
10/13/09    ka      Code changes to bring up UL DPCCH on 8220.
07/22/09    gnk     Mainlining the feature FEATURE_WCDMA_OFFLINE_DEMOD_SUPPORT
06/01/09    asm     Added a unified Ciphering Algorithm enumeration across Access Stratum
                    including Snow3G ciphering functionality. Also mainlined and cleaned up 
                    now-defunct featurization
10/03/08    hk      Bringing in 7k Mailine fixes
11/01/07    gnk     Mainlining the msm6246 changes
09/13/06    asr     Changed the feature name protecting file inclusion for this file from ENCI_H
                    to WENCI_H.
05/08/06    au      Replaced calls to MSM_IN and MSM_OUT with HWIO_IN and HWIO_OUT.
10/14/05    gs      Ciphering regiter bit width, bit position and other
                    related changes
10/07/07    eav     Changed reference from ADDR to ADDRI
09/14/05    gs      Updates for 6280 register name changes
04/18/05    asm     Merged Raven branch encoder files to mainline
11/04/04    src     Merged in 6275-specific changes.
10/11/04    gs      Added macros for Bc,Bd,Rate adjust update
                    Added macros for writing Tx control and data registers
                    for 6275 interface
01/14/04    gs      Added macro for ciphering mode.
                    Updated macros WRITE_TX_ERAMA_TB_CTRL_WORD to include
                    ciphering_mode.
11/10/03    yus     Replace FEATURE_6250_COMPILE with FEATURE_FAST_ENC_START_TIME.
10/22/03    yus     Fix bugs in WRITE_TX_ERAMA_TB_CIPHERING_CTL_WORD1 and
                    WRITE_TX_ERAMA_TB_CIPHERING_CTL_WORD2 macro when it's used 
                    in Saber.
08/18/03    yus     Added a software workaround for the MSM6250's ERAMA double 
                    writing bug. The software workaround uses the Test memories 
                    instead of ERAMA for modulator writing.
                    This workaround is under flag 
                    FEATURE_MSM6250_ERAMA_HW_BUG_SW_WORKAROUND.
08/07/03    yus     Separate START_ENC_TIME to UL_START_ENC_TIME and
                    UL_PRACH_START_ENC_TIME.
08/07/03    yus     Added #define START_ENC_TIME. It has different valus
                    for MSM6200 and MSM6250 due to hardware changes.
06/10/03    yus     Add HWIO_XXXX style macros support.
04/29/02    sk      Replaced FEATURE_MSM5200C with feature specific names.
03/06/02    sk/sh   Merged MSM5200C related changes.
01/18/02    gs      Added the macro writing for Ciphering Control words.
11/02/01    sk      added TX_ACCUM_FREEZE_EN, TX_ACCUM_FREEZE_DIS
04/17/01    sk      Added memory dump size defines.
09/08/00    sk      file created.
                    
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "wcdma_variation.h"
#include "comdef.h"
#include "msm.h"
#include "l1mathutil.h"
#include "wl1hwioif.h"
#include "mcalwcdma_wenc.h"

#if defined(FEATURE_WCDMA_BOLT_2_0) && !defined (FEATURE_WCDMA_JOLOKIA_MODEM)
/*===========================================================================

                     MACROS

===========================================================================*/
/* Note - TrCH Contrl RAM is implemented using registers. So, it can
 * be viewed using simple MSM_IN() calls.
 */

/* # active TrCHs, # ERAMa TrCHs */
#define WRITE_TX_ERAM_AB_TRCHS(_num_active_trchs, _num_erama_trchs) \
do { \
  mcalwcdma_wenc_int_cntrl_params.wenc_r99_non_cfg_ram_param_shadow_struct.common.enc_cfg.num_active_trchs_cctrch1 = _num_active_trchs; \
  mcalwcdma_wenc_int_cntrl_params.wenc_r99_non_cfg_ram_param_shadow_struct.common.enc_cfg.num_erama_trchs = _num_erama_trchs; \
}while(0)
  
/*--------------------------------------------------------------------*/

/* TRCH_CFG0_RAM parameters */
#define WRITE_TX_PHY_CH_TYPE(_phy_ch_type) \
  mcalwcdma_wenc_int_cntrl_params.wenc_r99_non_cfg_ram_param_shadow_struct.r99.phy_ch_type = _phy_ch_type;

#define WRITE_TX_TRBLK_PARAM_TRCH(_trch_num, _trblk_size, _crc_size, _num_trblks) \
do{ \
  mcalwcdma_wenc_int_cntrl_params.wenc_r99_cfg_ram_param_shadow_struct.cfg0[_trch_num].trblk_size = _trblk_size; \
  mcalwcdma_wenc_int_cntrl_params.wenc_r99_cfg_ram_param_shadow_struct.cfg0[_trch_num].crc_size = _crc_size; \
  mcalwcdma_wenc_int_cntrl_params.wenc_r99_cfg_ram_param_shadow_struct.cfg0[_trch_num].num_trblks = _num_trblks; \
}while(0)

#define WRITE_TX_ERAMB_ADDR_TRCH(_trch_num, _eramb_start_addr) \
  mcalwcdma_wenc_int_cntrl_params.wenc_r99_cfg_ram_param_shadow_struct.cfg0[_trch_num].eramb_start_addr = _eramb_start_addr; \

/*--------------------------------------------------------------------*/

/* TRCH_CFG1_RAM parameters */
#define WRITE_TX_CODING_PARAM_TRCH(_trch_num, _pre_rm_bits, _trch_tti, \
  _num_code_segs, _coding_rate_type) \
  do { \
    mcalwcdma_wenc_int_cntrl_params.wenc_r99_cfg_ram_param_shadow_struct.cfg1[_trch_num].num_pre_rm_bits = _pre_rm_bits; \
    mcalwcdma_wenc_int_cntrl_params.wenc_r99_cfg_ram_param_shadow_struct.cfg1[_trch_num].trch_tti = _trch_tti; \
    mcalwcdma_wenc_int_cntrl_params.wenc_r99_cfg_ram_param_shadow_struct.cfg1[_trch_num].num_code_segs = _num_code_segs; \
    mcalwcdma_wenc_int_cntrl_params.wenc_r99_cfg_ram_param_shadow_struct.cfg1[_trch_num].coding_rate_type = _coding_rate_type; \
  }while (0)
  
#define WRITE_TX_CODE_SEG_SIZE_TRCH(_trch_num, _puncture_flag, \
  _num_filler_bits, _num_bits_code_seg) \
  do { \
    mcalwcdma_wenc_int_cntrl_params.wenc_r99_cfg_ram_param_shadow_struct.cfg1[_trch_num].puncture_flag = _puncture_flag; \
    mcalwcdma_wenc_int_cntrl_params.wenc_r99_cfg_ram_param_shadow_struct.cfg1[_trch_num].num_filler_bits = _num_filler_bits; \
    mcalwcdma_wenc_int_cntrl_params.wenc_r99_cfg_ram_param_shadow_struct.cfg1[_trch_num].num_bits_code_seg = _num_bits_code_seg; \
  }while (0)
  
#define WRITE_TX_RM_EINI_TRCH(_trch_num, _initial_error2, _initial_error1) \
  do { \
    mcalwcdma_wenc_int_cntrl_params.wenc_r99_cfg_ram_param_shadow_struct.cfg1[_trch_num].initial_error1 = _initial_error1; \
    mcalwcdma_wenc_int_cntrl_params.wenc_r99_cfg_ram_param_shadow_struct.cfg1[_trch_num].initial_error2 = _initial_error2; \
  }while(0)

#define WRITE_TX_RM_EMINUS_TRCH(_trch_num, _eminus2, _eminus1) \
  do { \
    mcalwcdma_wenc_int_cntrl_params.wenc_r99_cfg_ram_param_shadow_struct.cfg1[_trch_num].eminus1 = _eminus1; \
    mcalwcdma_wenc_int_cntrl_params.wenc_r99_cfg_ram_param_shadow_struct.cfg1[_trch_num].eminus2 = _eminus2; \
  } while(0)

#define WRITE_TX_RM_EPLUS_TRCH(_trch_num, _eplus2, _eplus1) \
  do { \
    mcalwcdma_wenc_int_cntrl_params.wenc_r99_cfg_ram_param_shadow_struct.cfg1[_trch_num].eplus1 = _eplus1; \
    mcalwcdma_wenc_int_cntrl_params.wenc_r99_cfg_ram_param_shadow_struct.cfg1[_trch_num].eplus2 = _eplus2; \
  } while (0)

#define WRITE_TX_TURBO_INTLV_PARAM0_TRCH(_trch_num, _num_columns_minus1, \
  _last_column, _num_row_index, _last_row_bit_exch, _last_row) \
  do { \
    mcalwcdma_wenc_int_cntrl_params.wenc_r99_cfg_ram_param_shadow_struct.cfg1[_trch_num].num_columns_minus1 = _num_columns_minus1; \
    mcalwcdma_wenc_int_cntrl_params.wenc_r99_cfg_ram_param_shadow_struct.cfg1[_trch_num].last_column = _last_column; \
    mcalwcdma_wenc_int_cntrl_params.wenc_r99_cfg_ram_param_shadow_struct.cfg1[_trch_num].num_row_index = _num_row_index; \
    mcalwcdma_wenc_int_cntrl_params.wenc_r99_cfg_ram_param_shadow_struct.cfg1[_trch_num].last_row_bit_exch = _last_row_bit_exch; \
    mcalwcdma_wenc_int_cntrl_params.wenc_r99_cfg_ram_param_shadow_struct.cfg1[_trch_num].last_row = _last_row; \
  } while (0)

#define WRITE_TX_TURBO_INTLV_PARAM1_TRCH(_trch_num, _prime_num_index, \
  _prime_number) \
  do { \
    mcalwcdma_wenc_int_cntrl_params.wenc_r99_cfg_ram_param_shadow_struct.cfg1[_trch_num].prime_num_index = _prime_num_index; \
    mcalwcdma_wenc_int_cntrl_params.wenc_r99_cfg_ram_param_shadow_struct.cfg1[_trch_num].prime_number = _prime_number; \
  } while (0)

/*--------------------------------------------------------------------*/

/* Other parameters */
#define WRITE_TX_DPDCH_OVSF_NUM(dpdch_ovsf_num) \
  do { \
    wenc_ext_cntrl_params.wfw_intf_tx_async_write_ptr->r99Common.dpdchOvsfNum = dpdch_ovsf_num; \
  } while (0)

#define WRITE_TX_DATA_SPR_FACTOR(data_spr_factor) \
  do { \
   wenc_ext_cntrl_params.wfw_intf_tx_async_write_ptr->r99Common.dpdchSpreadFactor = mcalwcdma_wenc_get_hw_sprd_factor(data_spr_factor); \
  } while (0)

#define WRITE_TX_SCR_CODE_INIT(carr_id, code, code_type) \
  do { \
    wenc_ext_cntrl_params.wfw_intf_dpcch_async_write_ptr[carr_id]->dchScrCodeInitData0 = code ; \
    wenc_ext_cntrl_params.wfw_intf_dpcch_async_write_ptr[carr_id]->scrCodeType = code_type; \
  } while(0)

#define WRITE_TX_BETA_GAIN(spr_gain_pre, spr_gain_data, spr_gain_ctl) 1


#define WRITE_DPCH_TX_BETA_GAIN(ul_bc, ul_bd) \
   enc_set_bc_bd_pair_val(ul_bc, ul_bd ,wsub_id)
 
#define ENC_SET_RATE_ADJ_IN_DELTA(delta) \
   enc_set_rate_adj_in_cm_delta(delta ,wsub_id)

/*--------------------------------------------------------------------*/

/* ERAMa write data control */

#define UL_CIPHERING_F8_ALG_DIR 0
#define UL_CIPHERING_MODE_NORMAL 0
#define UL_CIPHERING_MODE_CONTINUE 1

#define WRITE_TX_ERAMA_INIT(dont_care) \
do { \
    mcalwcdma_wenc_ext_cntrl_params.wenc_r99_dch_erama_write_addr = 0; \
}while (0) \

#define WENC_NUM_ERAMA_WORDS_WRITTEN() mcalwcdma_wenc_ext_cntrl_params.wenc_r99_dch_erama_write_addr

#define WRITE_TX_ERAMA_TB_CTRL_WORD(ciphering_en, ciphering_mode, hdr_size, trch_cfg0_idx) \
  do { \
    mcalwcdma_wenc_int_cntrl_params.wenc_r99_crc_ctl_shadow_struct.erama_trch_index_w0 = trch_cfg0_idx; \
    mcalwcdma_wenc_int_cntrl_params.wenc_r99_crc_ctl_shadow_struct.ciphering_en_w0 = ciphering_en; \
    mcalwcdma_wenc_int_cntrl_params.wenc_r99_crc_ctl_shadow_struct.header_size_w0 = hdr_size; \
    mcalwcdma_wenc_int_cntrl_params.wenc_r99_crc_ctl_shadow_struct.will_continue_w0 = ciphering_mode; \
    mcalwcdma_wenc_int_cntrl_params.wenc_r99_crc_ctl_vpe_lmem_ptr = (mcalwcdma_wenc_enc_tx_r4r99_enc_crc_ctl_struct_type*)&(mcalwcdma_wenc_int_cntrl_params.wenc_r99_eul_eram_vpe_lmem_ptr->dch.Words[mcalwcdma_wenc_ext_cntrl_params.wenc_r99_dch_erama_write_addr]); \
    mcalwcdma_wenc_int_cntrl_params.wenc_r99_crc_ctl_vpe_lmem_ptr->Words[0] = mcalwcdma_wenc_int_cntrl_params.wenc_r99_crc_ctl_shadow_struct.Words[0]; \
    mcalwcdma_wenc_ext_cntrl_params.wenc_r99_dch_erama_write_addr++; \
  }while(0)
    
#define WRITE_TX_ERAMA_TB_CIPHERING_CTL_WORD1(algo, offset, key_idx, bearer_id, cipher_len) \
  do { \
    mcalwcdma_wenc_int_cntrl_params.wenc_r99_ciph_ctl_shadow_struct.algorithm_w1 = algo; \
    mcalwcdma_wenc_int_cntrl_params.wenc_r99_ciph_ctl_shadow_struct.bearer_w1 = bearer_id; \
    mcalwcdma_wenc_int_cntrl_params.wenc_r99_ciph_ctl_shadow_struct.ciphering_offset_w1 = offset; \
    mcalwcdma_wenc_int_cntrl_params.wenc_r99_ciph_ctl_shadow_struct.ck_index_w1 = key_idx; \
    mcalwcdma_wenc_int_cntrl_params.wenc_r99_ciph_ctl_shadow_struct.length_w1 = cipher_len; \
    mcalwcdma_wenc_int_cntrl_params.wenc_r99_ciph_ctl_shadow_struct.direction_w1 = UL_CIPHERING_F8_ALG_DIR; \
    mcalwcdma_wenc_int_cntrl_params.wenc_r99_ciph_ctl_vpe_lmem_ptr = (mcalwcdma_wenc_enc_tx_r4r99_enc_cipher_ctl_struct_type*)&(mcalwcdma_wenc_int_cntrl_params.wenc_r99_eul_eram_vpe_lmem_ptr->dch.Words[mcalwcdma_wenc_ext_cntrl_params.wenc_r99_dch_erama_write_addr]); \
    mcalwcdma_wenc_ext_cntrl_params.wenc_r99_dch_erama_write_addr++; \
  }while(0)

#define WRITE_TX_ERAMA_TB_CIPHERING_CTL_WORD2(count) \
  do { \
      mcalwcdma_wenc_int_cntrl_params.wenc_r99_ciph_ctl_shadow_struct.count_w1 = count; \
      mcalwcdma_wenc_int_cntrl_params.wenc_r99_ciph_ctl_vpe_lmem_ptr->Words[0] = mcalwcdma_wenc_int_cntrl_params.wenc_r99_ciph_ctl_shadow_struct.Words[0]; \
      mcalwcdma_wenc_int_cntrl_params.wenc_r99_ciph_ctl_vpe_lmem_ptr->Words[1] = mcalwcdma_wenc_int_cntrl_params.wenc_r99_ciph_ctl_shadow_struct.Words[1]; \
      mcalwcdma_wenc_ext_cntrl_params.wenc_r99_dch_erama_write_addr++; \
  } while (0)
  
#define WRITE_TX_ERAMA_DATA(erama_data) \
  do { \
    mcalwcdma_wenc_int_cntrl_params.wenc_r99_eul_eram_vpe_lmem_ptr->dch.Words[mcalwcdma_wenc_ext_cntrl_params.wenc_r99_dch_erama_write_addr++] = erama_data; \
  }while(0)
  
#define WRITE_PDU_DONE_TO_FW() \
  do { \
      wenc_ext_cntrl_params.wfw_intf_tx_async_write_ptr->r99Common.pduDoneCnt++; \
  }while (0)

#define WENC_RESET_R99_PDU_DONE_CNT() \
  do { \
      wenc_ext_cntrl_params.wfw_intf_tx_async_write_ptr->r99Common.pduDoneCnt = 0; \
  } while(0)
/*--------------------------------------------------------------------*/

#if defined(FEATURE_WCDMA_RUMI_SUPPORT) && defined(FEATURE_WCDMA_ATLAS_MODEM)
/* PBS CONTROL REG, used only for RUMI testing. */
#define WRITE_MSS_RESERVE_01_REG(x) wcdma_l1_ul_out_dword(WCDMA_L1_UL_HWIO_ADDR(MSS_RESERVE_01), x)
#endif /* defined(FEATURE_WCDMA_RUMI_SUPPORT) && defined(FEATURE_WCDMA_ATLAS_MODEM) */

#elif defined (FEATURE_WCDMA_JOLOKIA_MODEM)
/*===========================================================================

                     MACROS

===========================================================================*/
/* Start ENC time */
/* Don't change this value when EUL is present.Changing this value may result 
   in problems when EUL and DCH are present */
#define UL_START_ENC_TIME           0x7800
#define UL_PRACH_START_ENC_TIME     0x80007800

/* for freezing/unfreezing closed loop power control */
#define TX_ACCUM_FREEZE_EN     1
#define TX_ACCUM_FREEZE_DIS    0

/* 
 * Note - TrCH Contrl RAM is implemented using registers. 
 * So, it can be viewed using simple MSM_IN() calls.
 */

/* # active TrCHs, # ERAMa TrCHs */
#define WRITE_TX_ERAM_AB_TRCHS(num_active_trchs, num_erama_trchs) \
  WCDMA_L1_UL_HWIO_OUT (TX_ERAM_AB_TRCHS, ((uint32)num_active_trchs << 4) | (uint32)num_erama_trchs);

#define ENC_SCR_CODE_M           0x81000000L
#define ENC_SCR_CODE_LONG_V      0x81000000L  /* always set bit 24 to '1' */
#define ENC_SCR_CODE_SHORT_V     0x00000000L

#define WRITE_TX_BETA_GAIN(spr_gain_pre, spr_gain_data, spr_gain_ctl)  1
#define WRITE_DPCH_TX_BETA_GAIN(ul_bc, ul_bd) enc_set_bc_bd_pair_val(ul_bc, ul_bd ,wsub_id)
#define ENC_SET_RATE_ADJ_IN_DELTA(delta) enc_set_rate_adj_in_cm_delta(delta ,wsub_id)

/* ERAMa write data control */
#define UL_CIPHERING_F8_ALG_DIR       0
#define UL_CIPHERING_MODE_NORMAL      0
#define UL_CIPHERING_MODE_CONTINUE    1

#define WRITE_TX_ERAMA_TB_CTRL_WORD(ciphering_en, ciphering_mode, hdr_size, trch_cfg0_idx) \
     WCDMA_L1_UL_HWIO_OUT (TX_UNIFIED_ENC_ERAM_DATA_INCRACC_1, (ciphering_en << 10) | \
        (ciphering_mode << 9) | (hdr_size << 3) | trch_cfg0_idx);
    
#define WRITE_TX_ERAMA_TB_CIPHERING_CTL_WORD1(algo, offset, key_idx, bearer_id, cipher_len) \
        WCDMA_L1_UL_HWIO_OUT (TX_UNIFIED_ENC_ERAM_DATA_INCRACC_2, (((algo) & 0x1) << 28) | (((offset) & 0x1F) << 23) | \
    (((key_idx) & 0x7) << 20) | (((UL_CIPHERING_F8_ALG_DIR) & 0x1) << 19) | \
    (((bearer_id) & 0x1F) << 14) | ((cipher_len) & 0x3FFF))

#define WRITE_TX_ERAMA_TB_CIPHERING_CTL_WORD2(count) WCDMA_L1_UL_HWIO_OUT(TX_UNIFIED_ENC_ERAM_DATA_INCRACC_3, count)
#define WRITE_TX_ERAMA_DATA(erama_data) WCDMA_L1_UL_HWIO_OUT(TX_UNIFIED_ENC_ERAM_DATA_INCRACC_0, erama_data);
#define WRITE_TX_ENC_TIMING_CTL(start_enc_now, start_enc_time) \
     WCDMA_L1_UL_HWIO_OUT(TX_UNIFIED_DCH_RACH_ENC_TRIGGER, ((uint32)start_enc_now) | start_enc_time);

/* Miscellaneous */
#define WRITE_TX_PHASE_ACC_RESET(x)     WCDMA_L1_UL_HWIO_OUT(TX_PHASE_ACC_RESET, x);
#define WRITE_BETA_GAIN_GROUP_DLY(x)    WCDMA_L1_UL_HWIO_OUT(TX_BETA_GAIN_GROUP_DLY, x);

/* Tx Conditioning Block controls */
 #define WRITE_TX_COND_RESET(x)         WCDMA_L1_UL_HWIO_OUT(TX_CONDITIONING_RESET, x);

/*-----------------------------------------------------------------------*/
/* TX General Control bit definitions
 * 07 : PHASE_ACC_RESET - 1 resets tcxo phase accumulator
 * 06 : PHSAE_ACC_BYPASS - 1 bypasses tcxo phase accumulator
 * 05 : CORDIC_BYPASS - 1 bypasses cordic rotator
 * 04 : TX_SPECTRAL_INVERSION - 1 when TX spectral inversion is needed
 * 03 : TX_DATA_FORMAT - selects format for TX_IQ_DATA[7:0] pins
 *      1 - two's complement format
 *      0 - offset binary format (0 -> -127.5, 1-> 127.5)
 * [02:01]: PA_CTL - programs the state of TX_PUNCT
 *      00 - overrides TX_PUNCT and clears it to 0
 *      01 - overrides TX_PUNCT and sets it to 1
 *      1x - normal operation (controled by PA_WARMUP in TX AGC)
 * 00 : TX_OUTPUT_INTERFACE - controls output of Tx chain
 *      1 - enable BB3 interface (bypass IPF, DAC), data at chipx8 rate
 *      0 - enable TX DAC interface, data at chipx16 rate
 */
/*--------------------------------------------------------------------*/

/* TX Test Control definitions */
#define TX_CLK_INVERT_MASK      0x04  /* TX_I_CLK polarity mask */
#define TX_CLK_INVERT_HI        0x04  /* invert TX_I_CLK pin */
#define TX_CLK_INVERT_NORMAL    0x00  /* normal operation */

#define TX_SET_IQ_MASK          0x02  /* FIR input control mask */
#define TX_SEQ_IQ_HI            0x02  /* forces both inputs to FIRs high */
#define TX_IQ_NORMAL            0x00  /* normal operation */

#define TX_TONE_ENA_MASK        0x01  /* internal Tx test tone generator */
#define TX_TONE_ENA             0x01  

/* enable and select Tx internal test tone generator */
#define TX_TONE_ENA_NORMAL      0x00  /* normal operation */

/* TX Filler Polarity definitions */
#define TX_RADIO_FRAME_EQ_FILLER_MASK  0x02

/* filler bit during radio frame equalization */
#define TX_RADIO_FRAME_EQ_FILLER_HI    0x02
#define TX_RADIO_FRAME_EQ_FILLER_LO    0x00
#define TX_CODE_BLK_SEG_FILLER_MASK    0x01

/* filler bit during code block segmentation */
#define TX_CODE_BLK_SEG_FILLER_HI      0x01
#define TX_CODE_BLK_SEG_FILLER_LO      0x00

/* TX Status definitions */
#define TX_PA_STATUS_MASK             0x40
#define TX_PA_ON_V                    0x40
#define TX_PA_OFF_V                   0x00
#define TX_TRCH_NUM_MASK              0x3C
#define TX_ENC_BUSY_MASK              0x02
#define TX_ENC_BUSY_V                 0x02
#define TX_ENC_IDLE_V                 0x00
#define TX_ENC_ERR_MASK               0x01
#define TX_ENC_ERR_V                  0x01
#define TX_ENC_NO_ERR_V               0x00

/* read TX status */
#define READ_TX_STATUS()            WCDMA_L1_UL_HWIO_IN(TX_STATUS_RD) //WL1_HWIO_IN(TX_STATUS_RD)

/* Debug macros */
#define TX_ERAMA_SZ            280 //words
#define TESTMEM_ERAMA          0x14

#define TX_ERAMB_SZ            220  //words
#define TESTMEM_ERAMB          0x15

#define TX_TRCH_CFG0_RAM_SZ    16
#define TESTMEM_TRCH_CFG0_RAM  0x16 

#define TX_TRCH_CFG1_RAM_SZ    56
#define TESTMEM_TRCH_CFG1_RAM  0x17

#define TX_RMRAM_SZ            640
#define TESTMEM_RMRAM          0x18

/* FEATURE_WCDMA_JOLOKIA_MODEM */
#else //FEATURE_WCDMA_BOLT_2_0
/*===========================================================================

                     MACROS

===========================================================================*/
/* Start ENC time */
/* Don't change this value when EUL is present.Changing this value may result 
   in problems when EUL and DCH are present */
#define UL_START_ENC_TIME           0x7800
#define UL_PRACH_START_ENC_TIME 0x80007800

/* for freezing/unfreezing closed loop power control */
#define TX_ACCUM_FREEZE_EN  1
#define TX_ACCUM_FREEZE_DIS 0

/* Debug macros */
#define TX_ERAMA_SZ  280 //words
#define TESTMEM_ERAMA 0x14

#define TX_ERAMB_SZ  220  //words
#define TESTMEM_ERAMB 0x15

#define TX_TRCH_CFG0_RAM_SZ  16
#define TESTMEM_TRCH_CFG0_RAM 0x16 

#define TX_TRCH_CFG1_RAM_SZ  56
#define TESTMEM_TRCH_CFG1_RAM  0x17

#define TX_RMRAM_SZ  640
#define TESTMEM_RMRAM  0x18

/* Note - TrCH Contrl RAM is implemented using registers. So, it can
 * be viewed using simple MSM_IN() calls.
 */

/* # active TrCHs, # ERAMa TrCHs */
#define WRITE_TX_ERAM_AB_TRCHS(num_active_trchs, num_erama_trchs) \
  WCDMA_L1_UL_HWIO_OUT(TX_ERAM_AB_TRCHS, \
           ((uint32)num_active_trchs << 8) | (uint32)num_erama_trchs);

/*--------------------------------------------------------------------*/

/* TRCH_CFG0_RAM parameters */
#define  UL_TX_ABORT  0x00
#define WRITE_TX_PHY_CH_TYPE(phy_ch_type) \
  WCDMA_L1_UL_HWIO_OUT(TX_PHY_CH_TYPE, phy_ch_type);

#define WRITE_TX_TRBLK_PARAM_TRCH(trch_num, trblk_size, crc_size, num_trblks) \
  wcdma_l1_ul_out_dword(WCDMA_L1_UL_HWIO_ADDRI(TX_TRBLK_PARAM_TRCHc, 0) + trch_num * 4, \
  ((uint32)trblk_size << 16) | ((uint32)crc_size << 8) | (uint32)num_trblks);

#define WRITE_TX_ERAMB_ADDR_TRCH(trch_num, eramb_start_addr) \
  wcdma_l1_ul_out_dword(WCDMA_L1_UL_HWIO_ADDRI(TX_ERAMB_ADDR_TRCHc, 0) + trch_num * 4, (uint32)eramb_start_addr);

/*--------------------------------------------------------------------*/

/* TRCH_CFG1_RAM parameters */
#define WRITE_TX_CODING_PARAM_TRCH(trch_num, pre_rm_bits, trch_tti, \
  num_code_segs, coding_rate_type) \
  wcdma_l1_ul_out_dword(WCDMA_L1_UL_HWIO_ADDRI(TX_CODING_PARAM_TRCHc, 0) + trch_num * 4, ((uint32)pre_rm_bits << 16) \
  | ((uint32)trch_tti << 14) | ((uint32)num_code_segs << 8) \
  | ((uint32) coding_rate_type));

#define WRITE_TX_CODE_SEG_SIZE_TRCH(trch_num, puncture_flag, \
  num_filler_bits, num_bits_code_seg) \
  wcdma_l1_ul_out_dword(WCDMA_L1_UL_HWIO_ADDRI(TX_CODE_SEG_SIZE_TRCHc, 0) + trch_num * 4, (uint32)puncture_flag << 31 \
  | (uint32)num_filler_bits << 16 | (uint32)num_bits_code_seg);

#define WRITE_TX_RM_EINI_TRCH(trch_num, initial_error2, initial_error1) \
  wcdma_l1_ul_out_dword(WCDMA_L1_UL_HWIO_ADDRI(TX_RM_EINI_TRCHc, 0) + trch_num * 4, (uint32)initial_error2 << 16 \
  | (uint32)initial_error1);

#define WRITE_TX_RM_EMINUS_TRCH(trch_num, eminus2, eminus1) \
  wcdma_l1_ul_out_dword(WCDMA_L1_UL_HWIO_ADDRI(TX_RM_EMINUS_TRCHc, 0) + trch_num * 4, (uint32)eminus2 << 16 \
  | (uint32)eminus1);

#define WRITE_TX_RM_EPLUS_TRCH(trch_num, eplus2, eplus1) \
  wcdma_l1_ul_out_dword(WCDMA_L1_UL_HWIO_ADDRI(TX_RM_EPLUS_TRCHc, 0) + trch_num * 4, (uint32)eplus2 << 16 \
  | (uint32)eplus1);

#define WRITE_TX_TURBO_INTLV_PARAM0_TRCH(trch_num, num_columns_minus1, \
  last_column, num_row_index, last_row_bit_exch, last_row) \
  wcdma_l1_ul_out_dword(WCDMA_L1_UL_HWIO_ADDRI(TX_TURBO_INTLV_PARAM0_TRCHc, 0) + trch_num * 4, \
  (uint32)num_columns_minus1 << 24 | (uint32)last_column << 16 | \
  (uint32)num_row_index << 12 | (uint32)last_row_bit_exch << 8 | \
  (uint32)last_row);

#define WRITE_TX_TURBO_INTLV_PARAM1_TRCH(trch_num, prime_num_index, \
  prime_number) \
  wcdma_l1_ul_out_dword(WCDMA_L1_UL_HWIO_ADDRI(TX_TURBO_INTLV_PARAM1_TRCHc, 0) + trch_num * 4, \
  (uint32)prime_num_index << 16 | (uint32)prime_number);

/*--------------------------------------------------------------------*/

/* TRCH_CTL_RAM parameters */
#define WRITE_TX_TRCH_MUX_ORDER(trch_cfg1_index, trch_start_addr) \
  WCDMA_L1_UL_HWIO_OUT(TX_TRCH_MUX_ORDER, \
           ( (uint32)(trch_cfg1_index << HWIO_TX_TRCH_MUX_ORDER_TRCH_CFG1_INDEX_SHFT) | \
             (uint32)( (trch_start_addr & HWIO_TX_TRCH_MUX_ORDER_TRCH_START_ADDR_BMSK) << HWIO_TX_TRCH_MUX_ORDER_TRCH_START_ADDR_SHFT)));

/* Other parameters */
#define WRITE_TX_DPDCH_OVSF_NUM(dpdch_ovsf_num) \
  WCDMA_L1_UL_HWIO_OUT(TX_DPDCH_OVSF_NUM, dpdch_ovsf_num);

#define WRITE_TX_DATA_SPR_FACTOR(data_spr_factor) \
  WCDMA_L1_UL_HWIO_OUT(TX_DATA_SPR_FACTOR, data_spr_factor);

#define ENC_SCR_CODE_M       0x81000000L
#define ENC_SCR_CODE_LONG_V  0x81000000L  /* always set bit 24 to '1' */
#define ENC_SCR_CODE_SHORT_V 0x00000000L

#define WRITE_TX_SCR_CODE_INIT(code) \
  WCDMA_L1_UL_HWIO_OUT (TX_SCR_CODE_INIT, (uint32) code);

#ifdef FEATURE_WCDMA_DC_HSUPA 
#define WRITE_TX_SCR_CODE_INIT_SEC(code) \
  WCDMA_L1_UL_HWIO_OUT( TX_SCR_CODE_INIT_C1 , (uint32)code );
#endif /* FEATUR_WCDMA_DC_HSUPA */


#define WRITE_TX_BETA_GAIN(spr_gain_pre, spr_gain_data, spr_gain_ctl) 1


#define WRITE_DPCH_TX_BETA_GAIN(ul_bc, ul_bd) \
   enc_set_bc_bd_pair_val(ul_bc, ul_bd ,wsub_id)
 
#define ENC_SET_RATE_ADJ_IN_DELTA(delta) \
   enc_set_rate_adj_in_cm_delta(delta ,wsub_id)

/*--------------------------------------------------------------------*/

/* ERAMa write data control */

#define UL_CIPHERING_F8_ALG_DIR 0
#define UL_CIPHERING_MODE_NORMAL 0
#define UL_CIPHERING_MODE_CONTINUE 1

/* ERAMa write data control */
/* Due to a HW bug, writes to ERAMA using INCRACC
 * are corrupted. Hence this interface is not used for Nikel */
#define WRITE_TX_ERAMA_INIT(dont_care) \
  WCDMA_L1_UL_HWIO_OUT(TX_ERAMA_INCRACC_INIT, dont_care);

#define WRITE_TX_ERAMA_TB_CTRL_WORD(ciphering_en, ciphering_mode, hdr_size, trch_cfg0_idx) \
  WCDMA_L1_UL_HWIO_OUT(TX_ERAMA_INCRACC, (ciphering_en << 16) | \
    (ciphering_mode << 15) | (hdr_size << 8) | trch_cfg0_idx);
    
#define WRITE_TX_ERAMA_TB_CIPHERING_CTL_WORD1(algo, offset, key_idx, bearer_id, cipher_len) \
  WCDMA_L1_UL_HWIO_OUT(TX_ERAMA_INCRACC, (((algo) & 0x1) << 28) | (((offset) & 0x1F) << 23) | \
    (((key_idx) & 0x7) << 20) | (((UL_CIPHERING_F8_ALG_DIR) & 0x1) << 19) | \
    (((bearer_id) & 0x1F) << 14) | ((cipher_len) & 0x3FFF))

#define WRITE_TX_ERAMA_TB_CIPHERING_CTL_WORD2(count) \
  WCDMA_L1_UL_HWIO_OUT(TX_ERAMA_INCRACC, count)

#define WRITE_TX_ERAMA_DATA(erama_data) \
  WCDMA_L1_UL_HWIO_OUT(TX_ERAMA_INCRACC, erama_data);

#define WRITE_TX_ENC_TIMING_CTL(start_enc_now, start_enc_time) \
  WCDMA_L1_UL_HWIO_OUT(TX_ENC_TIMING_CTL, ((uint32) start_enc_now << 31) | start_enc_time);

/*--------------------------------------------------------------------*/

/* Miscellaneous */

#define WRITE_TX_PHASE_ACC_RESET(x)     WCDMA_L1_UL_HWIO_OUT(TX_PHASE_ACC_RESET, x);

#define WRITE_BETA_GAIN_GROUP_DLY(x)    WCDMA_L1_UL_HWIO_OUT(TX_BETA_GAIN_GROUP_DLY, x);

/*-----------------------------------------------------------------------*/

/* Tx Conditioning Block controls */
 #define WRITE_TX_COND_RESET(x)          WCDMA_L1_UL_HWIO_OUT(TX_CONDITIONING_RESET, x);

/*-----------------------------------------------------------------------*/
/* TX General Control bit definitions
 * 07 : PHASE_ACC_RESET - 1 resets tcxo phase accumulator
 * 06 : PHSAE_ACC_BYPASS - 1 bypasses tcxo phase accumulator
 * 05 : CORDIC_BYPASS - 1 bypasses cordic rotator
 * 04 : TX_SPECTRAL_INVERSION - 1 when TX spectral inversion is needed
 * 03 : TX_DATA_FORMAT - selects format for TX_IQ_DATA[7:0] pins
 *      1 - two's complement format
 *      0 - offset binary format (0 -> -127.5, 1-> 127.5)
 * [02:01]: PA_CTL - programs the state of TX_PUNCT
 *      00 - overrides TX_PUNCT and clears it to 0
 *      01 - overrides TX_PUNCT and sets it to 1
 *      1x - normal operation (controled by PA_WARMUP in TX AGC)
 * 00 : TX_OUTPUT_INTERFACE - controls output of Tx chain
 *      1 - enable BB3 interface (bypass IPF, DAC), data at chipx8 rate
 *      0 - enable TX DAC interface, data at chipx16 rate
 */

#define WRITE_TX_GENERAL_CTL(ctl_val) WCDMA_L1_UL_HWIO_OUT(TX_GENERAL_CTL, ctl_val);

#define WRITE_TX_ROT_ANGLE_0_CTL(pa_state_01_angle, pa_state_00_angle) \
  WCDMA_L1_UL_HWIO_OUT(TX_ROT_ANGLE_0_CTL, \
          ((uint32) pa_state_01_angle << 9) | pa_state_00_angle);

#define WRITE_TX_ROT_ANGLE_1_CTL(pa_state_11_angle, pa_state_10_angle) \
  WCDMA_L1_UL_HWIO_OUT(TX_ROT_ANGLE_1_CTL, \
          ((uint32) pa_state_11_angle << 9) | pa_state_10_angle);

/*--------------------------------------------------------------------*/

/* TX Test Control definitions */
#define TX_CLK_INVERT_MASK      0x04  /* TX_I_CLK polarity mask */
#define TX_CLK_INVERT_HI        0x04  /* invert TX_I_CLK pin */
#define TX_CLK_INVERT_NORMAL    0x00  /* normal operation */

#define TX_SET_IQ_MASK          0x02  /* FIR input control mask */
#define TX_SEQ_IQ_HI            0x02  /* forces both inputs to FIRs high */
#define TX_IQ_NORMAL            0x00  /* normal operation */

#define TX_TONE_ENA_MASK        0x01  /* internal Tx test tone generator */
#define TX_TONE_ENA             0x01  
  /* enable and select Tx internal test tone generator */
#define TX_TONE_ENA_NORMAL      0x00  /* normal operation */

#define WRITE_TX_TEST_CTL(test_val) WCDMA_L1_UL_HWIO_OUT(TX_TEST_CTL, test_val);

/*--------------------------------------------------------------------*/

/* TX Filler Polarity definitions */
#define TX_RADIO_FRAME_EQ_FILLER_MASK  0x02
  /* filler bit during radio frame equalization */
#define TX_RADIO_FRAME_EQ_FILLER_HI    0x02
#define TX_RADIO_FRAME_EQ_FILLER_LO    0x00

#define TX_CODE_BLK_SEG_FILLER_MASK    0x01
  /* filler bit during code block segmentation */
#define TX_CODE_BLK_SEG_FILLER_HI      0x01
#define TX_CODE_BLK_SEG_FILLER_LO      0x00

#define WRITE_TX_FILLER_POLARITY(filler_val) \
  WCDMA_L1_UL_HWIO_OUT(TX_FILLER_POLARITY, filler_val);

/*--------------------------------------------------------------------*/

/* TX Status definitions */
#define TX_PA_STATUS_MASK             0x40
#define TX_PA_ON_V                    0x40
#define TX_PA_OFF_V                   0x00

#define TX_TRCH_NUM_MASK              0x3C

#define TX_ENC_BUSY_MASK              0x02
#define TX_ENC_BUSY_V                 0x02
#define TX_ENC_IDLE_V                 0x00

#define TX_ENC_ERR_MASK               0x01
#define TX_ENC_ERR_V                  0x01
#define TX_ENC_NO_ERR_V               0x00

/* read TX status */
#define READ_TX_STATUS()    WCDMA_L1_UL_HWIO_IN(TX_STATUS)

/* read TX status with a mask */
#define READ_TX_STATUS_M(mask)    WCDMA_L1_UL_HWIO_INM(TX_STATUS, mask)
#endif //FEATURE_WCDMA_BOLT_2_0
#endif /* WENCI_H */
