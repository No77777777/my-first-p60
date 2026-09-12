#ifndef TDSENCI_H
#define TDSENCI_H

/*============================================================================
              E N C O D E R   D R I V E R   I N T E R N A L
                          H E A D E R   F I L E

DESCRIPTION
This files contains all necessary macros and definitions for interfacing with
the encoder hardware.

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2000~2011 QUALCOMM Technologies, Incorporated.  All Rights Reserved.
============================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/l1/src/hal/jolokia/tdsenci.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when        who     what, where, why
--------    ---     --------------------------------------------------------
03/20/11    cdf     add E-RUCCH register definitions
02/16/11    cxc     file created.
                    
===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "tdscdma_variation.h"
#include "comdef.h"
#include "msm.h"

/*===========================================================================

                     MACROS

===========================================================================*/


#define TDSENCI_TESTMEM_ERAMB 0x15
#define TDSENCI_UL_CIPHERING_F8_ALG_DIR 0
#define TDSENCI_UL_CIPHERING_MODE_NORMAL 0
#define TDSENCI_UL_CIPHERING_MODE_CONTINUE 1

#define TDSENCI_ENABLE                  1
#define TDSENCI_DISABLE                 0
#define TDSENCI_DAC0_ENABLE             1
#define TDSENCI_DAC0_DISABLE            0
#define TDSENCI_DAC1_ENABLE             1
#define TDSENCI_DAC1_DISABLE            0
#define TDSENCI_DAC0_FCAL_DISABLE       0
#define TDSENCI_DAC1_FCAL_DISABLE       0
#define TDSENCI_DAC0_GSM_SDM_DISABLE    0
#define TDSENCI_SYS_MODE_DEFAULT_VAL    0
#define TDSENCI_TX_MEM_ENABLE           1
#define TDSENCI_TX_MODE_c_TXR_ENABLE		1
#define TDSENCI_TX_MODE_c_TXC_ENABLE    1
#define TDSENCI_TX_MODE_COMMON_TX_MEM_EN 1
#define TDSENCI_CDMA_TX_DISABLE         0
#define TDSENCI_UMTS_TX_DISABLE         0
#define TDSENCI_OFDMA_TX_DISABLE        0
#define TDSENCI_TXC0_ENABLE             1
#define TDSENCI_TXC1_DISABLE            0
#define TDSENCI_TXR0_ENABLE             1
#define TDSENCI_TXR1_DISABLE            0
#define TDSENCI_TDS_CTRL_ENABLE         1
#define TDSENCI_TX0_ENABLE              1
#define TDSENCI_TX0_DISABLE              0
#define TDSENCI_TX1_DISABLE             0
#define TDSENCI_UMTS_TX_C1_ENABLE			1
#define TDSENCI_UMTS_TX_C1_DISABLE     0
#define TDSENCI_SWP_DISABLE             0
#define TDSENCI_LITTLE_ENDIAN           0
#define TDSENCI_BIG_ENDIAN           1 
#define TDSENCI_FALL_TIME               0x55
#define TDSENCI_ZEROS_FLUSH_NUM               0x0f
#define TDSENCI_LITTLE_ENDIAN_FRMT_FOR_R4_UL        1
#define TDSENCI_BIG_ENDIAN_FRMT_FOR_EUL             0
#define TDSENCI_BIG_ENDIAN_FRMT      0
#define TDSENCI_NUM_ACTIVE_TRCHS_CCTRCH2_DEFAULT_VAL 0
#define TDSENCI_NUM_ACTIVE_TRCHS_CCTRCH1_DEFAULT_VAL 0
#define TDSENCI_NUM_ERAMA_TRCHS_DEFAULT_VAL          0
#define TDSENCI_ERAMA_INIT              1
#define TDSENCI_CLEAR_MUX_ORDER         1
#define TDSENCI_CCTRCH2_SI_TYPE_DEFAULT_VAL          0
#define TDSENCI_SI_TYPE_FRAME_BASED                  0
#define TDSENCI_CCTRCH2_ADDR_DEFAULT_VAL             0
#define TDSENCI_CCTRCH1_ADDR_DEFAULT_VAL             0
#define TDSENCI_CCTRCH2_ADDR_BLANK_TYPE_DEFAULT_VAL  0
#define TDSENCI_CCTRCH1_ADDR_BLANK_TYPE_DEFAULT_VAL  0
#define TDSENCI_SLOT_DATA_NUM_DEFAULT_VAL            0
#define TDSENCI_EQ_FILLER_BIT_DEFAULT_VAL            0
#define TDSENCI_START_ENC_NOW_DCH_RACH               1

#define TDSENCI_TX_TDS_CCTRCH_2_BANK_TYPE_BMSK	0x8000
#define TDSENCI_TX_TDS_CCTRCH_2_BANK_TYPE_SHIFT	0x13
#define TDSENCI_TX_TDS_CCTRCH_1_BANK_TYPE_BMSK	0x200
#define TDSENCI_TX_TDS_CCTRCH_1_BANK_TYPE_SHIFT	0x9
#define TDSENCI_TX_TDS_CCTRCH_2_ADDR_DPCH_BMSK	0x7fc00
#define TDSENCI_TX_TDS_CCTRCH_1_ADDR_DPCH_BMSK	0x1ff

#ifdef FEATURE_TDSCDMA_HSUPA
#define TDSENCI_START_ENC_NOW_ERUCCH                 1
#define TDSENCI_ERUCCH_TTI_5MS                       0
#define TDSENCI_ERUCCH_TTI_10MS                      1
#define TDSENCI_ERUCCH_RMRAM_ADDR                    479
#define TDSENCI_ERUCCH_EINI                          1
#define TDSENCI_ERUCCH_EMINUS                        26
#define TDSENCI_ERUCCH_EPLUS                         378
#define TDSENCI_ERUCCH_ERAM_INCRACC_INIT						1
#define TDSENCI_ERUCCH_ERAM_BANK											2
#endif /* FEATURE_TDSCDMA_HSUPA */


#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_ENABLE( enable) \
{	\
	uint32 val = \
   ((uint32)((enable) << HWIO_TX_ENABLE_ENABLE_SHFT) & HWIO_TX_ENABLE_ENABLE_BMSK);  \
   HWIO_TX_ENABLE_OUT(val); \
   TDSL1_MSG1(HIGH, "TX_ENABLE: val = %x", val); \
}
#else
#define TDSENCI_WRITE_TX_ENABLE( enable) \
{	\
	uint32 val = \
   ((uint32)((enable) << HWIO_TX_ENABLE_ENABLE_SHFT) & HWIO_TX_ENABLE_ENABLE_BMSK);  \
   HWIO_TX_ENABLE_OUT(val); \
}
#endif /*FEATURE_TDSCDMA_PLT|TEST_FRAMEWORK*/

#define TDSENCI_WRITE_TX_MODE_c_0(txr_en,\
																	txc_en)\
{\
	uint32 val = \
	((uint32)((txr_en)<<HWIO_TX_MODE_c_TXR_EN_SHFT) & HWIO_TX_MODE_c_TXR_EN_BMSK) | \
	((uint32)((txc_en)<<HWIO_TX_MODE_c_TXC_EN_SHFT) & HWIO_TX_MODE_c_TXC_EN_BMSK );\
	HWIO_TX_MODE_c_OUTI(0, val);\
}

#define TDSENCI_WRITE_TX_MODE_c_1(txr_en,\
																	txc_en)\
{\
	uint32 val = \
	((uint32)((txr_en)<<HWIO_TX_MODE_c_TXR_EN_SHFT) & HWIO_TX_MODE_c_TXR_EN_BMSK) | \
	((uint32)((txc_en)<<HWIO_TX_MODE_c_TXC_EN_SHFT) & HWIO_TX_MODE_c_TXC_EN_BMSK );\
	HWIO_TX_MODE_c_OUTI(1, val);\
}

#define TDSENCI_WRITE_TX_MODE_COMMON(tx_mem_en)\
{\
	uint32 val = \
	((uint32) ((TDSENCI_ENABLE)<<HWIO_TX_MODE_COMMON_FLT_TX_EN_SHFT) & HWIO_TX_MODE_COMMON_FLT_TX_EN_BMSK) | \
	((uint32) ((TDSENCI_ENABLE)<<HWIO_TX_MODE_COMMON_MOD_TX_EN_SHFT) & HWIO_TX_MODE_COMMON_MOD_TX_EN_BMSK) | \
	((uint32) ((TDSENCI_ENABLE)<<HWIO_TX_MODE_COMMON_ENC_TX_EN_SHFT) & HWIO_TX_MODE_COMMON_ENC_TX_EN_BMSK) | \
	((uint32) ((TDSENCI_ENABLE)<<HWIO_TX_MODE_COMMON_CTL_TX_EN_SHFT) & HWIO_TX_MODE_COMMON_CTL_TX_EN_BMSK) | \
	((uint32) ((tx_mem_en)<<HWIO_TX_MODE_COMMON_TX_MEM_EN_SHFT) & HWIO_TX_MODE_COMMON_TX_MEM_EN_BMSK) | \
	((uint32) ((TDSENCI_DISABLE)<<HWIO_TX_MODE_COMMON_SYS_MODE_SHFT) & HWIO_TX_MODE_COMMON_SYS_MODE_BMSK);\
	HWIO_TX_MODE_COMMON_OUT(val);\
}

#define TDSENCI_WRITE_TXC_TECH_SEL_An()\
{\
	HWIO_TXC_TECH_SEL_An_OUTI(0,0x01);\
}


#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_CTRL(enable) \
{ \
	uint32 val = ((uint32)((enable) << HWIO_TX_TDS_CTRL_ENABLE_SHFT) & HWIO_TX_TDS_CTRL_ENABLE_BMSK); \
  HWIO_TX_TDS_CTRL_OUT(val);\
  TDSL1_MSG1(HIGH, "TX_TDS_CTRL: val = %x", val); \
}
#else
#define TDSENCI_WRITE_TX_CTRL(enable) \
{ \
	uint32 val = ((uint32)((enable) << HWIO_TX_TDS_CTRL_ENABLE_SHFT) & HWIO_TX_TDS_CTRL_ENABLE_BMSK); \
  HWIO_TX_TDS_CTRL_OUT(val);\
}
#endif

#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_DAC_OUTPUT_SEL( tx0, \
                                         tx1, \
                                         swp) \
{ \
	uint32 val = \
   ((uint32)((tx0) << HWIO_TX_DAC_OUTPUT_SEL_TX0_SHFT) & HWIO_TX_DAC_OUTPUT_SEL_TX0_BMSK) | \
   ((uint32)((tx1) << HWIO_TX_DAC_OUTPUT_SEL_TX1_SHFT) & HWIO_TX_DAC_OUTPUT_SEL_TX1_BMSK) | \
   ((uint32)((swp) << HWIO_TX_DAC_OUTPUT_SEL_SWP_SHFT) & HWIO_TX_DAC_OUTPUT_SEL_SWP_BMSK); \
  HWIO_TX_DAC_OUTPUT_SEL_OUT(val);\
  TDSL1_MSG1(HIGH, "TX_DAC_OUTPUT_SEL: val = %x", val); \
}
#else
#define TDSENCI_WRITE_TX_DAC_OUTPUT_SEL( tx0, \
                                         tx1, \
                                         swp) \
{ \
	uint32 val = \
   ((uint32)((tx0) << HWIO_TX_DAC_OUTPUT_SEL_TX0_SHFT) & HWIO_TX_DAC_OUTPUT_SEL_TX0_BMSK) | \
   ((uint32)((tx1) << HWIO_TX_DAC_OUTPUT_SEL_TX1_SHFT) & HWIO_TX_DAC_OUTPUT_SEL_TX1_BMSK) | \
   ((uint32)((swp) << HWIO_TX_DAC_OUTPUT_SEL_SWP_SHFT) & HWIO_TX_DAC_OUTPUT_SEL_SWP_BMSK); \
  HWIO_TX_DAC_OUTPUT_SEL_OUT(val);\
}
#endif

/* Select the endian-ness of data:
 * 0: Little-endian
 * 1: Big-endian
 * Applies to both A2 and FW AHB interfaces. 
 */
#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_MEM_ACCESS_CFG(ahb_endian) \
{ \
  uint32 val = ((uint32) ((ahb_endian) << HWIO_TX_MEM_ACCESS_CFG_AHB_ENDIAN_SHFT) & HWIO_TX_MEM_ACCESS_CFG_AHB_ENDIAN_BMSK); \
  HWIO_TX_MEM_ACCESS_CFG_OUT(val); \
  TDSL1_MSG1(HIGH, "TX_MEM_ACCESS_CFG: val = %x", val); \
}
#else
#define TDSENCI_WRITE_TX_MEM_ACCESS_CFG(ahb_endian) \
{ \
	uint32 val = ((uint32) ((ahb_endian) << HWIO_TX_MEM_ACCESS_CFG_AHB_ENDIAN_SHFT) & HWIO_TX_MEM_ACCESS_CFG_AHB_ENDIAN_BMSK); \
  HWIO_TX_MEM_ACCESS_CFG_OUT(val); \
}
#endif

/* TD_TX_ERAM_AB_TRCHS programs the number of transport channels that need to be
 * transferred from ERAMA to ERAMB. It also programs the number of transport 
 * channels that are active for each CCTrCH in the current frame and this is used 
 * by hardware to read the active transport channels from ERAMB.
 */
#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_ERAM_AB_TRCHS( num_active_trchs_cctrch2, \
                                        num_active_trchs_cctrch1, \
                                        num_erama_trchs) \
{ \
	uint32 val = \
  ((uint32)((num_active_trchs_cctrch2) << HWIO_TX_UNIFIED_ENC_NUM_ACTIVE_TRCHS_NUM_ACTIVE_TRCHS_CCTRCH2_SHFT) & HWIO_TX_UNIFIED_ENC_NUM_ACTIVE_TRCHS_NUM_ACTIVE_TRCHS_CCTRCH2_BMSK) | \
  ((uint32)((num_active_trchs_cctrch1) << HWIO_TX_UNIFIED_ENC_NUM_ACTIVE_TRCHS_NUM_ACTIVE_TRCHS_CCTRCH1_SHFT) & HWIO_TX_UNIFIED_ENC_NUM_ACTIVE_TRCHS_NUM_ACTIVE_TRCHS_CCTRCH1_BMSK) | \
  ((uint32)((num_erama_trchs) << HWIO_TX_UNIFIED_ENC_NUM_ACTIVE_TRCHS_NUM_ERAMA_TRCHS_SHFT) & HWIO_TX_UNIFIED_ENC_NUM_ACTIVE_TRCHS_NUM_ERAMA_TRCHS_BMSK); \
  HWIO_TX_UNIFIED_ENC_NUM_ACTIVE_TRCHS_OUT(val); \
  TDSL1_MSG1(HIGH, "TX_UNIFIED_ENC_NUM_ACTIVE_TRCHS: val = %x", val); \
}
#else
#define TDSENCI_WRITE_TX_ERAM_AB_TRCHS( num_active_trchs_cctrch2, \
                                        num_active_trchs_cctrch1, \
                                        num_erama_trchs) \
{ \
      uint32 val = \
    ((uint32)((num_active_trchs_cctrch2) << HWIO_TX_UNIFIED_ENC_NUM_ACTIVE_TRCHS_NUM_ACTIVE_TRCHS_CCTRCH2_SHFT) & HWIO_TX_UNIFIED_ENC_NUM_ACTIVE_TRCHS_NUM_ACTIVE_TRCHS_CCTRCH2_BMSK) | \
    ((uint32)((num_active_trchs_cctrch1) << HWIO_TX_UNIFIED_ENC_NUM_ACTIVE_TRCHS_NUM_ACTIVE_TRCHS_CCTRCH1_SHFT) & HWIO_TX_UNIFIED_ENC_NUM_ACTIVE_TRCHS_NUM_ACTIVE_TRCHS_CCTRCH1_BMSK) | \
    ((uint32)((num_erama_trchs) << HWIO_TX_UNIFIED_ENC_NUM_ACTIVE_TRCHS_NUM_ERAMA_TRCHS_SHFT) & HWIO_TX_UNIFIED_ENC_NUM_ACTIVE_TRCHS_NUM_ERAMA_TRCHS_BMSK); \
    HWIO_TX_UNIFIED_ENC_NUM_ACTIVE_TRCHS_OUT(val);\
}
#endif

/* Ciphering confidentiality key */
#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_CIPHER_CK(ck_index, key_info_index, data) \
{	\
	switch (ck_index) \
	{ \
		case 0: \
			HWIO_TX_UNIFIED_ENC_CIPHER_CK0_m_OUTI(key_info_index, data); \
			break; \
		case 1: \
			HWIO_TX_UNIFIED_ENC_CIPHER_CK1_m_OUTI(key_info_index, data); \
			break; \
		case 2: \
			HWIO_TX_UNIFIED_ENC_CIPHER_CK2_m_OUTI(key_info_index, data); \
			break; \
		case 3: \
			HWIO_TX_UNIFIED_ENC_CIPHER_CK3_m_OUTI(key_info_index, data); \
			break; \
		case 4: \
			HWIO_TX_UNIFIED_ENC_CIPHER_CK4_m_OUTI(key_info_index, data); \
			break; \
		case 5: \
			HWIO_TX_UNIFIED_ENC_CIPHER_CK5_m_OUTI(key_info_index, data); \
			break; \
		default: \
			ERR_FATAL("ERROR: ck_index=%d", ck_index, 0, 0 ); \
			break; \
	} \
  TDSL1_MSG3(HIGH, "TX_UNIFIED_ENC_CIPHER_CK: ck_idx=%d,  ck_info_idx=%d,  data = %x", ck_index, key_info_index, data); \
}
#else
#define TDSENCI_WRITE_TX_CIPHER_CK(ck_index, key_info_index, data) \
{	\
	switch (ck_index) \
        { \
            case 0: \
                HWIO_TX_UNIFIED_ENC_CIPHER_CK0_m_OUTI(key_info_index, data); \
                break; \
            case 1: \
                HWIO_TX_UNIFIED_ENC_CIPHER_CK1_m_OUTI(key_info_index, data); \
                break; \
            case 2: \
                HWIO_TX_UNIFIED_ENC_CIPHER_CK2_m_OUTI(key_info_index, data); \
                break; \
            case 3: \
                HWIO_TX_UNIFIED_ENC_CIPHER_CK3_m_OUTI(key_info_index, data); \
                break; \
            case 4: \
                HWIO_TX_UNIFIED_ENC_CIPHER_CK4_m_OUTI(key_info_index, data); \
                break; \
            case 5: \
                HWIO_TX_UNIFIED_ENC_CIPHER_CK5_m_OUTI(key_info_index, data); \
                break; \
            default: \
                ERR_FATAL("ERROR: ck_index=%d", ck_index, 0, 0 ); \
                break; \
        } \
}
#endif


/* Radio frame count for R99 first interleaving operation */
#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_OFFL_ENC_FRAME(frame_cnt) \
{	\
	uint32 val = ((uint32)((frame_cnt) << HWIO_TX_UNIFIED_ENC_OFFL_ENC_FRAME_FRAME_CNT_SHFT) & HWIO_TX_UNIFIED_ENC_OFFL_ENC_FRAME_FRAME_CNT_BMSK); \
  HWIO_TX_UNIFIED_ENC_OFFL_ENC_FRAME_OUT(val); \
  TDSL1_MSG1(HIGH, "TX_UNIFIED_ENC_OFFL_ENC_FRAME: val = %x", val); \
}
#else
#define TDSENCI_WRITE_TX_OFFL_ENC_FRAME(frame_cnt) \
{	\
      uint32 val = ((uint32)((frame_cnt) << HWIO_TX_UNIFIED_ENC_OFFL_ENC_FRAME_FRAME_CNT_SHFT) & HWIO_TX_UNIFIED_ENC_OFFL_ENC_FRAME_FRAME_CNT_BMSK); \
    HWIO_TX_UNIFIED_ENC_OFFL_ENC_FRAME_OUT(val); \
}
#endif

/* This register programs the polarity of the filler bits used for code block 
 * segmentation and radio frame equalization for 1st and 2nd CCTrCHs. 
 */
#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_FILLER_POLARITY( rffm_eq_filler_bit_cctrch2, \
                                          rffm_eq_filler_bit_cctrch1) \
{ \
	uint32 val = \
   ((uint32)((rffm_eq_filler_bit_cctrch2) << HWIO_TX_UNIFIED_ENC_FILLER_POLARITY_RFRM_EQ_FILLER_BIT_CCTRCH2_SHFT) & HWIO_TX_UNIFIED_ENC_FILLER_POLARITY_RFRM_EQ_FILLER_BIT_CCTRCH2_BMSK) | \
   ((uint32)((rffm_eq_filler_bit_cctrch1) << HWIO_TX_UNIFIED_ENC_FILLER_POLARITY_RFRM_EQ_FILLER_BIT_CCTRCH1_SHFT)& HWIO_TX_UNIFIED_ENC_FILLER_POLARITY_RFRM_EQ_FILLER_BIT_CCTRCH1_BMSK); \
  HWIO_TX_UNIFIED_ENC_FILLER_POLARITY_OUT(val); \
  TDSL1_MSG1(HIGH, "TX_UNIFIED_ENC_FILLER_POLARITY: val = %x", val); \
}
#else
#define TDSENCI_WRITE_TX_FILLER_POLARITY( rffm_eq_filler_bit_cctrch2, \
                                          rffm_eq_filler_bit_cctrch1) \
{ \
      uint32 val = \
     ((uint32)((rffm_eq_filler_bit_cctrch2) << HWIO_TX_UNIFIED_ENC_FILLER_POLARITY_RFRM_EQ_FILLER_BIT_CCTRCH2_SHFT) & HWIO_TX_UNIFIED_ENC_FILLER_POLARITY_RFRM_EQ_FILLER_BIT_CCTRCH2_BMSK) | \
     ((uint32)((rffm_eq_filler_bit_cctrch1) << HWIO_TX_UNIFIED_ENC_FILLER_POLARITY_RFRM_EQ_FILLER_BIT_CCTRCH1_SHFT)& HWIO_TX_UNIFIED_ENC_FILLER_POLARITY_RFRM_EQ_FILLER_BIT_CCTRCH1_BMSK); \
    HWIO_TX_UNIFIED_ENC_FILLER_POLARITY_OUT(val); \
}
#endif

/* This register is used for both DCH and RACH since DCH and RACH are mutually 
 * exclusive. This register determines when HW starts DCH and RACH encoding the 
 * data that needs to be transmitted in the following frame.
 * This register must be the last register written by software in a frame. 
 */
 #if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_ENC_TIMING_CTL_DCH_RACH(start_enc_now_dch_rach) \
{ \
  uint32 val = ((uint32)((start_enc_now_dch_rach) << HWIO_TX_UNIFIED_DCH_RACH_ENC_TRIGGER_START_ENC_NOW_DCH_RACH_SHFT) & HWIO_TX_UNIFIED_DCH_RACH_ENC_TRIGGER_START_ENC_NOW_DCH_RACH_BMSK); \
  HWIO_TX_UNIFIED_DCH_RACH_ENC_TRIGGER_OUT(val);\
  TDSL1_MSG1(HIGH, "TX_UNIFIED_DCH_RACH_ENC_TRIGGER: val = %x", val); \
}
#else
#define TDSENCI_WRITE_TX_ENC_TIMING_CTL_DCH_RACH(start_enc_now_dch_rach) \
{ \
    uint32 val = ((uint32)((start_enc_now_dch_rach) << HWIO_TX_UNIFIED_DCH_RACH_ENC_TRIGGER_START_ENC_NOW_DCH_RACH_SHFT) & HWIO_TX_UNIFIED_DCH_RACH_ENC_TRIGGER_START_ENC_NOW_DCH_RACH_BMSK); \
    HWIO_TX_UNIFIED_DCH_RACH_ENC_TRIGGER_OUT(val);\
}
#endif

/* This register programs the transport block parameters for a transport channel 
 * (DCH or RACH) in a frame. A maximum of eight transport channels in a frame are 
 * supported. */
#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_TRBLK_PARAM_TRCH(trch_num, \
                                          trblk_size, \
                                          crc_size, \
                                          num_trblks) \
{ \
	uint32 val = \
  ((uint32)((trblk_size) << HWIO_TX_UNIFIED_ENC_TRBLK_PARAM_TRCHc_TRBLK_SIZE_SHFT) & HWIO_TX_UNIFIED_ENC_TRBLK_PARAM_TRCHc_TRBLK_SIZE_BMSK) | \
  ((uint32)((crc_size) << HWIO_TX_UNIFIED_ENC_TRBLK_PARAM_TRCHc_CRC_SIZE_SHFT) & HWIO_TX_UNIFIED_ENC_TRBLK_PARAM_TRCHc_CRC_SIZE_BMSK) | \
  ((uint32)((num_trblks) << HWIO_TX_UNIFIED_ENC_TRBLK_PARAM_TRCHc_NUM_TRBLKS_SHFT) & HWIO_TX_UNIFIED_ENC_TRBLK_PARAM_TRCHc_NUM_TRBLKS_BMSK); \
  HWIO_TX_UNIFIED_ENC_TRBLK_PARAM_TRCHc_OUTI(trch_num, val); \
  TDSL1_MSG2(HIGH, "TX_UNIFIED_ENC_TRBLK_PARAM_TRCH%d: val = %x", trch_num, val); \
}
#else
#define TDSENCI_WRITE_TX_TRBLK_PARAM_TRCH(trch_num, \
                                          trblk_size, \
                                          crc_size, \
                                          num_trblks) \
{ \
      uint32 val = \
    ((uint32)((trblk_size) << HWIO_TX_UNIFIED_ENC_TRBLK_PARAM_TRCHc_TRBLK_SIZE_SHFT) & HWIO_TX_UNIFIED_ENC_TRBLK_PARAM_TRCHc_TRBLK_SIZE_BMSK) | \
    ((uint32)((crc_size) << HWIO_TX_UNIFIED_ENC_TRBLK_PARAM_TRCHc_CRC_SIZE_SHFT) & HWIO_TX_UNIFIED_ENC_TRBLK_PARAM_TRCHc_CRC_SIZE_BMSK) | \
    ((uint32)((num_trblks) << HWIO_TX_UNIFIED_ENC_TRBLK_PARAM_TRCHc_NUM_TRBLKS_SHFT) & HWIO_TX_UNIFIED_ENC_TRBLK_PARAM_TRCHc_NUM_TRBLKS_BMSK); \
    HWIO_TX_UNIFIED_ENC_TRBLK_PARAM_TRCHc_OUTI(trch_num, val); \
}
#endif

/* This register programs the starting address in ERAMB of transport channel n. 
 * Hardware writes transport channel n to ERAMB starting at the address specified 
 * by this register. 
 */
#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_ERAMB_ADDR_TRCH(trch_num, \
                                         eramb_start_addr) \
{ \
  uint32 val = ((uint32)((eramb_start_addr) << HWIO_TX_UNIFIED_ENC_ERAMB_ADDR_TRCHc_DATA_SHFT) & HWIO_TX_UNIFIED_ENC_ERAMB_ADDR_TRCHc_DATA_BMSK); \
  HWIO_TX_UNIFIED_ENC_ERAMB_ADDR_TRCHc_OUTI(trch_num, val); \
  TDSL1_MSG2(HIGH, "TX_UNIFIED_ENC_ERAMB_ADDR_TRCH%d: val = %x", trch_num, val); \
}
#else
#define TDSENCI_WRITE_TX_ERAMB_ADDR_TRCH(trch_num, \
                                         eramb_start_addr) \
{ \
    uint32 val = ((uint32)((eramb_start_addr) << HWIO_TX_UNIFIED_ENC_ERAMB_ADDR_TRCHc_DATA_SHFT) & HWIO_TX_UNIFIED_ENC_ERAMB_ADDR_TRCHc_DATA_BMSK); \
    HWIO_TX_UNIFIED_ENC_ERAMB_ADDR_TRCHc_OUTI(trch_num, val); \
}
#endif
/* SW shall program it every 10ms. 
 * If it is set to 1, TD_TX_TRCH_MUX_ORDER_CCTRCH1/2 registers are cleared. 
 * If SW choose to command this, this register must be programmed before writing
 * TD_TX_TRCH_MUX_ORDER_CCTRCH1/2. 
 */
#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_NEW_FRAME_SW(new_frame_sw) \
{ \
  uint32 val = (uint32)((new_frame_sw) << HWIO_TX_TDS_NEW_FRAME_SW_NEW_FRAME_SW_SHFT) & HWIO_TX_TDS_NEW_FRAME_SW_NEW_FRAME_SW_BMSK; \
  HWIO_TX_TDS_NEW_FRAME_SW_OUT(val); \
  TDSL1_MSG1(HIGH, "TX_TDS_NEW_FRAME_SW: val = %x",  val); \
}
#else
#define TDSENCI_WRITE_TX_NEW_FRAME_SW(new_frame_sw) \
{ \
    uint32 val = (uint32)((new_frame_sw) << HWIO_TX_TDS_NEW_FRAME_SW_NEW_FRAME_SW_SHFT) & HWIO_TX_TDS_NEW_FRAME_SW_NEW_FRAME_SW_BMSK; \
    HWIO_TX_TDS_NEW_FRAME_SW_OUT(val); \
}
#endif
/* This register defines the order in which the transport channels to be mapped 
 * into 1st CCTrCH are processed. This register programs the starting address in 
 * ERAMB of the transport channel. It also provides an index to TRCH_CFG1_RAM for 
 * reading the parameters of the transport channel associated with channel coding 
 * and rate matching.
 */
#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_TRCH_MUX_ORDER_CCTRCH1(trch_cfg1_index_cctrch1, \
                                                trch_start_addr_cctrch1) \
{ \
	uint32 val = \
  ((uint32)((trch_cfg1_index_cctrch1) << HWIO_TX_UNIFIED_ENC_TRCH_MUX_ORDER_CCTRCH1_TRCH_CFG1_INDEX_CCTRCH1_SHFT) & HWIO_TX_UNIFIED_ENC_TRCH_MUX_ORDER_CCTRCH1_TRCH_CFG1_INDEX_CCTRCH1_BMSK) | \
  ((uint32)((trch_start_addr_cctrch1) << HWIO_TX_UNIFIED_ENC_TRCH_MUX_ORDER_CCTRCH1_TRCH_START_ADDR_CCTRCH1_SHFT) & HWIO_TX_UNIFIED_ENC_TRCH_MUX_ORDER_CCTRCH1_TRCH_START_ADDR_CCTRCH1_BMSK); \
  HWIO_TX_UNIFIED_ENC_TRCH_MUX_ORDER_CCTRCH1_OUT(val); \
  TDSL1_MSG1(HIGH, "TX_UNIFIED_ENC_TRCH_MUX_ORDER_CCTRCH1: val = %x",  val); \
}
#else
#define TDSENCI_WRITE_TX_TRCH_MUX_ORDER_CCTRCH1(trch_cfg1_index_cctrch1, \
                                                trch_start_addr_cctrch1) \
{ \
      uint32 val = \
    ((uint32)((trch_cfg1_index_cctrch1) << HWIO_TX_UNIFIED_ENC_TRCH_MUX_ORDER_CCTRCH1_TRCH_CFG1_INDEX_CCTRCH1_SHFT) & HWIO_TX_UNIFIED_ENC_TRCH_MUX_ORDER_CCTRCH1_TRCH_CFG1_INDEX_CCTRCH1_BMSK) | \
    ((uint32)((trch_start_addr_cctrch1) << HWIO_TX_UNIFIED_ENC_TRCH_MUX_ORDER_CCTRCH1_TRCH_START_ADDR_CCTRCH1_SHFT) & HWIO_TX_UNIFIED_ENC_TRCH_MUX_ORDER_CCTRCH1_TRCH_START_ADDR_CCTRCH1_BMSK); \
    HWIO_TX_UNIFIED_ENC_TRCH_MUX_ORDER_CCTRCH1_OUT(val); \
}
#endif
/* The same as TD_TX_TRCH_MUX_ORDER_CCTRCH1. This register defines the order in 
 * which the transport channels to be mapped into 2nd CCTrCH are processed. 
 */
#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_TRCH_MUX_ORDER_CCTRCH2(trch_cfg1_index_cctrch2, \
                                                trch_start_addr_cctrch2) \
{ \
	uint32 val = \
   ((uint32)((trch_cfg1_index_cctrch2) << HWIO_TX_TDS_TRCH_MUX_ORDER_CCTRCH2_TRCH_CFG1_INDEX_CCTRCH2_SHFT) & HWIO_TX_TDS_TRCH_MUX_ORDER_CCTRCH2_TRCH_CFG1_INDEX_CCTRCH2_BMSK) | \
   ((uint32)((trch_start_addr_cctrch2) << HWIO_TX_TDS_TRCH_MUX_ORDER_CCTRCH2_TRCH_START_ADDR_CCTRCH2_SHFT) & HWIO_TX_TDS_TRCH_MUX_ORDER_CCTRCH2_TRCH_START_ADDR_CCTRCH2_BMSK); \
   HWIO_TX_TDS_TRCH_MUX_ORDER_CCTRCH2_OUT(val); \
   TDSL1_MSG1(HIGH, "TX_TDS_TRCH_MUX_ORDER_CCTRCH2: val = %x",  val); \
}
#else
#define TDSENCI_WRITE_TX_TRCH_MUX_ORDER_CCTRCH2(trch_cfg1_index_cctrch2, \
                                                trch_start_addr_cctrch2) \
{ \
     uint32 val = \
    ((uint32)((trch_cfg1_index_cctrch2) << HWIO_TX_TDS_TRCH_MUX_ORDER_CCTRCH2_TRCH_CFG1_INDEX_CCTRCH2_SHFT) & HWIO_TX_TDS_TRCH_MUX_ORDER_CCTRCH2_TRCH_CFG1_INDEX_CCTRCH2_BMSK) | \
    ((uint32)((trch_start_addr_cctrch2) << HWIO_TX_TDS_TRCH_MUX_ORDER_CCTRCH2_TRCH_START_ADDR_CCTRCH2_SHFT) & HWIO_TX_TDS_TRCH_MUX_ORDER_CCTRCH2_TRCH_START_ADDR_CCTRCH2_BMSK); \
    HWIO_TX_TDS_TRCH_MUX_ORDER_CCTRCH2_OUT(val); \
}
#endif
/* This register programs the coding parameters of each transport channel being 
 * written to ERAMA. 
 */
#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_CODING_PARAM_TRCH(trch_num, \
                                           num_pre_rm_bits, \
                                           trch_tti, \
                                           num_code_segs, \
                                           coding_rate_type) \
{ \
	uint32 val = \
  ((uint32)((num_pre_rm_bits) << HWIO_TX_UNIFIED_ENC_CODING_PARAM_TRCHc_NUM_PRE_RM_BITS_SHFT) & HWIO_TX_UNIFIED_ENC_CODING_PARAM_TRCHc_NUM_PRE_RM_BITS_BMSK) | \
  ((uint32)((trch_tti) << HWIO_TX_UNIFIED_ENC_CODING_PARAM_TRCHc_TRCH_TTI_SHFT) & HWIO_TX_UNIFIED_ENC_CODING_PARAM_TRCHc_TRCH_TTI_BMSK) | \
  ((uint32)((num_code_segs) << HWIO_TX_UNIFIED_ENC_CODING_PARAM_TRCHc_NUM_CODE_SEGS_SHFT) & HWIO_TX_UNIFIED_ENC_CODING_PARAM_TRCHc_NUM_CODE_SEGS_BMSK) | \
  ((uint32)((coding_rate_type) << HWIO_TX_UNIFIED_ENC_CODING_PARAM_TRCHc_CODING_RATE_TYPE_SHFT) & HWIO_TX_UNIFIED_ENC_CODING_PARAM_TRCHc_CODING_RATE_TYPE_BMSK); \
  HWIO_TX_UNIFIED_ENC_CODING_PARAM_TRCHc_OUTI(trch_num,val);\
  TDSL1_MSG2(HIGH, "TX_UNIFIED_ENC_CODING_PARAM_TRCH%d: val = %x",  trch_num, val); \
}
#else
#define TDSENCI_WRITE_TX_CODING_PARAM_TRCH(trch_num, \
                                           num_pre_rm_bits, \
                                           trch_tti, \
                                           num_code_segs, \
                                           coding_rate_type) \
{ \
      uint32 val = \
    ((uint32)((num_pre_rm_bits) << HWIO_TX_UNIFIED_ENC_CODING_PARAM_TRCHc_NUM_PRE_RM_BITS_SHFT) & HWIO_TX_UNIFIED_ENC_CODING_PARAM_TRCHc_NUM_PRE_RM_BITS_BMSK) | \
    ((uint32)((trch_tti) << HWIO_TX_UNIFIED_ENC_CODING_PARAM_TRCHc_TRCH_TTI_SHFT) & HWIO_TX_UNIFIED_ENC_CODING_PARAM_TRCHc_TRCH_TTI_BMSK) | \
    ((uint32)((num_code_segs) << HWIO_TX_UNIFIED_ENC_CODING_PARAM_TRCHc_NUM_CODE_SEGS_SHFT) & HWIO_TX_UNIFIED_ENC_CODING_PARAM_TRCHc_NUM_CODE_SEGS_BMSK) | \
    ((uint32)((coding_rate_type) << HWIO_TX_UNIFIED_ENC_CODING_PARAM_TRCHc_CODING_RATE_TYPE_SHFT) & HWIO_TX_UNIFIED_ENC_CODING_PARAM_TRCHc_CODING_RATE_TYPE_BMSK); \
    HWIO_TX_UNIFIED_ENC_CODING_PARAM_TRCHc_OUTI(trch_num,val);\
}
#endif


/* This register programs the size of code segments for each transport channel 
 * being written to ERAMA. 
 */
#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_CODE_SEG_SIZE_TRCH( trch_num, \
                                             puncture_flag, \
                                             num_filler_bits, \
                                             num_bits_code_seg) \
{ \
	uint32 val = \
  ((uint32)((puncture_flag) << HWIO_TX_UNIFIED_ENC_CODE_SEG_SIZE_TRCHc_PUNCTURE_FLAG_SHFT) & HWIO_TX_UNIFIED_ENC_CODE_SEG_SIZE_TRCHc_PUNCTURE_FLAG_BMSK) | \
  ((uint32)((num_filler_bits) << HWIO_TX_UNIFIED_ENC_CODE_SEG_SIZE_TRCHc_NUM_FILLER_BITS_SHFT) & HWIO_TX_UNIFIED_ENC_CODE_SEG_SIZE_TRCHc_NUM_FILLER_BITS_BMSK) | \
  ((uint32)((num_bits_code_seg) << HWIO_TX_UNIFIED_ENC_CODE_SEG_SIZE_TRCHc_NUM_BITS_CODE_SEG_SHFT) & HWIO_TX_UNIFIED_ENC_CODE_SEG_SIZE_TRCHc_NUM_BITS_CODE_SEG_BMSK); \
  HWIO_TX_UNIFIED_ENC_CODE_SEG_SIZE_TRCHc_OUTI(trch_num,val);\
  TDSL1_MSG2(HIGH, "TX_UNIFIED_ENC_CODE_SEG_SIZE_TRCH%d: val = %x",  trch_num, val); \
}
#else
#define TDSENCI_WRITE_TX_CODE_SEG_SIZE_TRCH( trch_num, \
                                             puncture_flag, \
                                             num_filler_bits, \
                                             num_bits_code_seg) \
{ \
      uint32 val = \
    ((uint32)((puncture_flag) << HWIO_TX_UNIFIED_ENC_CODE_SEG_SIZE_TRCHc_PUNCTURE_FLAG_SHFT) & HWIO_TX_UNIFIED_ENC_CODE_SEG_SIZE_TRCHc_PUNCTURE_FLAG_BMSK) | \
    ((uint32)((num_filler_bits) << HWIO_TX_UNIFIED_ENC_CODE_SEG_SIZE_TRCHc_NUM_FILLER_BITS_SHFT) & HWIO_TX_UNIFIED_ENC_CODE_SEG_SIZE_TRCHc_NUM_FILLER_BITS_BMSK) | \
    ((uint32)((num_bits_code_seg) << HWIO_TX_UNIFIED_ENC_CODE_SEG_SIZE_TRCHc_NUM_BITS_CODE_SEG_SHFT) & HWIO_TX_UNIFIED_ENC_CODE_SEG_SIZE_TRCHc_NUM_BITS_CODE_SEG_BMSK); \
    HWIO_TX_UNIFIED_ENC_CODE_SEG_SIZE_TRCHc_OUTI(trch_num,val);\
}
#endif

/* This register programs the initial value of the error variables (eini1, eini2) 
 * used in the rate matching algorithm. 
 */
#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_RM_EINI_TRCH( trch_num, \
                                       initial_error2, \
                                       initial_error1) \
{ \
	uint32 val = \
  ((uint32)((initial_error2) << HWIO_TX_UNIFIED_ENC_RM_EINI_TRCHc_INITIAL_ERROR2_SHFT) & HWIO_TX_UNIFIED_ENC_RM_EINI_TRCHc_INITIAL_ERROR2_BMSK) | \
  ((uint32)((initial_error1) << HWIO_TX_UNIFIED_ENC_RM_EINI_TRCHc_INITIAL_ERROR1_SHFT) & HWIO_TX_UNIFIED_ENC_RM_EINI_TRCHc_INITIAL_ERROR1_BMSK); \
  HWIO_TX_UNIFIED_ENC_RM_EINI_TRCHc_OUTI(trch_num,val);\
  TDSL1_MSG2(HIGH, "TX_UNIFIED_ENC_RM_EINI_TRCH%d: val = %x",  trch_num, val); \
}
#else
#define TDSENCI_WRITE_TX_RM_EINI_TRCH( trch_num, \
                                       initial_error2, \
                                       initial_error1) \
{ \
      uint32 val = \
    ((uint32)((initial_error2) << HWIO_TX_UNIFIED_ENC_RM_EINI_TRCHc_INITIAL_ERROR2_SHFT) & HWIO_TX_UNIFIED_ENC_RM_EINI_TRCHc_INITIAL_ERROR2_BMSK) | \
    ((uint32)((initial_error1) << HWIO_TX_UNIFIED_ENC_RM_EINI_TRCHc_INITIAL_ERROR1_SHFT) & HWIO_TX_UNIFIED_ENC_RM_EINI_TRCHc_INITIAL_ERROR1_BMSK); \
    HWIO_TX_UNIFIED_ENC_RM_EINI_TRCHc_OUTI(trch_num,val);\
}
#endif

/* This register programs the decrement value of the error variables (eminus1, 
 * eminus2) used in the rate matching algorithm. 
 */
#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_RM_EMINUS_TRCH(trch_num, \
                                        eminus2, \
                                        eminus1) \
{ \
	uint32 val = \
  ((uint32)((eminus2) << HWIO_TX_UNIFIED_ENC_RM_EMINUS_TRCHc_EMINUS2_SHFT) & HWIO_TX_UNIFIED_ENC_RM_EMINUS_TRCHc_EMINUS2_BMSK) | \
  ((uint32)((eminus1) << HWIO_TX_UNIFIED_ENC_RM_EMINUS_TRCHc_EMINUS1_SHFT) & HWIO_TX_UNIFIED_ENC_RM_EMINUS_TRCHc_EMINUS1_BMSK); \
  HWIO_TX_UNIFIED_ENC_RM_EMINUS_TRCHc_OUTI(trch_num,val); \
  TDSL1_MSG2(HIGH, "TX_UNIFIED_ENC_RM_EMINUS_TRCH%d: val = %x",  trch_num, val); \
}
#else
#define TDSENCI_WRITE_TX_RM_EMINUS_TRCH(trch_num, \
                                        eminus2, \
                                        eminus1) \
{ \
      uint32 val = \
    ((uint32)((eminus2) << HWIO_TX_UNIFIED_ENC_RM_EMINUS_TRCHc_EMINUS2_SHFT) & HWIO_TX_UNIFIED_ENC_RM_EMINUS_TRCHc_EMINUS2_BMSK) | \
    ((uint32)((eminus1) << HWIO_TX_UNIFIED_ENC_RM_EMINUS_TRCHc_EMINUS1_SHFT) & HWIO_TX_UNIFIED_ENC_RM_EMINUS_TRCHc_EMINUS1_BMSK); \
    HWIO_TX_UNIFIED_ENC_RM_EMINUS_TRCHc_OUTI(trch_num,val); \
}
#endif
/* This register programs the increment value of the error variables (eplus1, 
 * ennnnn) used in the rate matching algorithm. 
 */
#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_RM_EPLUS_TRCH(trch_num, \
                                       eplus2, \
                                       eplus1) \
{ \
	uint32 val = \
  ((uint32)((eplus2) << HWIO_TX_UNIFIED_ENC_RM_EPLUS_TRCHc_EPLUS2_SHFT) & HWIO_TX_UNIFIED_ENC_RM_EPLUS_TRCHc_EPLUS2_BMSK) | \
  ((uint32)((eplus1) << HWIO_TX_UNIFIED_ENC_RM_EPLUS_TRCHc_EPLUS1_SHFT) & HWIO_TX_UNIFIED_ENC_RM_EPLUS_TRCHc_EPLUS1_BMSK); \
  HWIO_TX_UNIFIED_ENC_RM_EPLUS_TRCHc_OUTI(trch_num,val);\
  TDSL1_MSG2(HIGH, "TX_UNIFIED_ENC_RM_EPLUS_TRCH%d: val = %x",  trch_num, val); \
}
#else
#define TDSENCI_WRITE_TX_RM_EPLUS_TRCH(trch_num, \
                                       eplus2, \
                                       eplus1) \
{ \
	uint32 val = \
  ((uint32)((eplus2) << HWIO_TX_UNIFIED_ENC_RM_EPLUS_TRCHc_EPLUS2_SHFT) & HWIO_TX_UNIFIED_ENC_RM_EPLUS_TRCHc_EPLUS2_BMSK) | \
  ((uint32)((eplus1) << HWIO_TX_UNIFIED_ENC_RM_EPLUS_TRCHc_EPLUS1_SHFT) & HWIO_TX_UNIFIED_ENC_RM_EPLUS_TRCHc_EPLUS1_BMSK); \
  HWIO_TX_UNIFIED_ENC_RM_EPLUS_TRCHc_OUTI(trch_num,val);\
}
#endif

/* These two registers program the parameters needed by the Turbo Interleaver. */
#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_TURBO_INTLV_PARAM0_TRCH(trch_num, \
                                                 num_columns_minus1, \
                                                 last_column, num_row_index, \
                                                 last_row_bit_exch, \
                                                 last_row) \
{ \
	uint32 val = \
  ((uint32)((num_columns_minus1) << HWIO_TX_UNIFIED_ENC_TURBO_INTLV_PARAM0_TRCHc_NUM_COLUMNS_MINUS1_SHFT) & HWIO_TX_UNIFIED_ENC_TURBO_INTLV_PARAM0_TRCHc_NUM_COLUMNS_MINUS1_BMSK) | \
  ((uint32)((last_column) << HWIO_TX_UNIFIED_ENC_TURBO_INTLV_PARAM0_TRCHc_LAST_COLUMN_SHFT) & HWIO_TX_UNIFIED_ENC_TURBO_INTLV_PARAM0_TRCHc_LAST_COLUMN_BMSK) | \
  ((uint32)((num_row_index) << HWIO_TX_UNIFIED_ENC_TURBO_INTLV_PARAM0_TRCHc_NUM_ROW_INDEX_SHFT) & HWIO_TX_UNIFIED_ENC_TURBO_INTLV_PARAM0_TRCHc_NUM_ROW_INDEX_BMSK) | \
  ((uint32)((last_row_bit_exch) << HWIO_TX_UNIFIED_ENC_TURBO_INTLV_PARAM0_TRCHc_LAST_ROW_BIT_EXCH_SHFT) & HWIO_TX_UNIFIED_ENC_TURBO_INTLV_PARAM0_TRCHc_LAST_ROW_BIT_EXCH_BMSK) | \
  ((uint32)((last_row) << HWIO_TX_UNIFIED_ENC_TURBO_INTLV_PARAM0_TRCHc_LAST_ROW_SHFT) & HWIO_TX_UNIFIED_ENC_TURBO_INTLV_PARAM0_TRCHc_LAST_ROW_BMSK); \
  HWIO_TX_UNIFIED_ENC_TURBO_INTLV_PARAM0_TRCHc_OUTI(trch_num,val); \
  TDSL1_MSG2(HIGH, "TX_UNIFIED_ENC_TURBO_INTLV_PARAM0_TRCH%d: val = %x",  trch_num, val); \
}
#else
#define TDSENCI_WRITE_TX_TURBO_INTLV_PARAM0_TRCH(trch_num, \
                                                 num_columns_minus1, \
                                                 last_column, num_row_index, \
                                                 last_row_bit_exch, \
                                                 last_row) \
{ \
	uint32 val = \
  ((uint32)((num_columns_minus1) << HWIO_TX_UNIFIED_ENC_TURBO_INTLV_PARAM0_TRCHc_NUM_COLUMNS_MINUS1_SHFT) & HWIO_TX_UNIFIED_ENC_TURBO_INTLV_PARAM0_TRCHc_NUM_COLUMNS_MINUS1_BMSK) | \
  ((uint32)((last_column) << HWIO_TX_UNIFIED_ENC_TURBO_INTLV_PARAM0_TRCHc_LAST_COLUMN_SHFT) & HWIO_TX_UNIFIED_ENC_TURBO_INTLV_PARAM0_TRCHc_LAST_COLUMN_BMSK) | \
  ((uint32)((num_row_index) << HWIO_TX_UNIFIED_ENC_TURBO_INTLV_PARAM0_TRCHc_NUM_ROW_INDEX_SHFT) & HWIO_TX_UNIFIED_ENC_TURBO_INTLV_PARAM0_TRCHc_NUM_ROW_INDEX_BMSK) | \
  ((uint32)((last_row_bit_exch) << HWIO_TX_UNIFIED_ENC_TURBO_INTLV_PARAM0_TRCHc_LAST_ROW_BIT_EXCH_SHFT) & HWIO_TX_UNIFIED_ENC_TURBO_INTLV_PARAM0_TRCHc_LAST_ROW_BIT_EXCH_BMSK) | \
  ((uint32)((last_row) << HWIO_TX_UNIFIED_ENC_TURBO_INTLV_PARAM0_TRCHc_LAST_ROW_SHFT) & HWIO_TX_UNIFIED_ENC_TURBO_INTLV_PARAM0_TRCHc_LAST_ROW_BMSK); \
  HWIO_TX_UNIFIED_ENC_TURBO_INTLV_PARAM0_TRCHc_OUTI(trch_num,val);\
}
#endif

#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_TURBO_INTLV_PARAM1_TRCH(trch_num, \
                                                 prime_num_index, \
                                                 prime_number) \
{ \
	uint32 val = \
  ((uint32)((prime_num_index) << HWIO_TX_UNIFIED_ENC_TURBO_INTLV_PARAM1_TRCHc_PRIME_NUM_INDEX_SHFT) & HWIO_TX_UNIFIED_ENC_TURBO_INTLV_PARAM1_TRCHc_PRIME_NUM_INDEX_BMSK) | \
  ((uint32)((prime_number) << HWIO_TX_UNIFIED_ENC_TURBO_INTLV_PARAM1_TRCHc_PRIME_NUMBER_SHFT) & HWIO_TX_UNIFIED_ENC_TURBO_INTLV_PARAM1_TRCHc_PRIME_NUMBER_BMSK); \
  HWIO_TX_UNIFIED_ENC_TURBO_INTLV_PARAM1_TRCHc_OUTI(trch_num,val);\
  TDSL1_MSG2(HIGH, "TX_UNIFIED_ENC_TURBO_INTLV_PARAM1_TRCH%d: val = %x",  trch_num, val); \
}
#else
#define TDSENCI_WRITE_TX_TURBO_INTLV_PARAM1_TRCH(trch_num, \
                                                 prime_num_index, \
                                                 prime_number) \
{ \
	uint32 val = \
  ((uint32)((prime_num_index) << HWIO_TX_UNIFIED_ENC_TURBO_INTLV_PARAM1_TRCHc_PRIME_NUM_INDEX_SHFT) & HWIO_TX_UNIFIED_ENC_TURBO_INTLV_PARAM1_TRCHc_PRIME_NUM_INDEX_BMSK) | \
  ((uint32)((prime_number) << HWIO_TX_UNIFIED_ENC_TURBO_INTLV_PARAM1_TRCHc_PRIME_NUMBER_SHFT) & HWIO_TX_UNIFIED_ENC_TURBO_INTLV_PARAM1_TRCHc_PRIME_NUMBER_BMSK); \
  HWIO_TX_UNIFIED_ENC_TURBO_INTLV_PARAM1_TRCHc_OUTI(trch_num,val);\
}
#endif

/* This register programs each CCTrCH¡¯s 2nd Interleaving type */
#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_CCTRCH_SI(cctrch2_si_type, \
                                   cctrch1_si_type) \
{ \
	uint32 val = \
  ((uint32)((cctrch2_si_type) << HWIO_TX_TDS_CCTRCH_SI_CCTRCH_2_SI_TYPE_SHFT) & HWIO_TX_TDS_CCTRCH_SI_CCTRCH_2_SI_TYPE_BMSK) | \
  ((uint32)((cctrch1_si_type) << HWIO_TX_TDS_CCTRCH_SI_CCTRCH_1_SI_TYPE_SHFT) & HWIO_TX_TDS_CCTRCH_SI_CCTRCH_1_SI_TYPE_BMSK); \
  HWIO_TX_TDS_CCTRCH_SI_OUT(val); \
  TDSL1_MSG1(HIGH, "TX_TDS_CCTRCH_SI: val = %x",  val); \
}
#else
#define TDSENCI_WRITE_TX_CCTRCH_SI(cctrch2_si_type, \
                                   cctrch1_si_type) \
{ \
	uint32 val = \
  ((uint32)((cctrch2_si_type) << HWIO_TX_TDS_CCTRCH_SI_CCTRCH_2_SI_TYPE_SHFT) & HWIO_TX_TDS_CCTRCH_SI_CCTRCH_2_SI_TYPE_BMSK) | \
  ((uint32)((cctrch1_si_type) << HWIO_TX_TDS_CCTRCH_SI_CCTRCH_1_SI_TYPE_SHFT) & HWIO_TX_TDS_CCTRCH_SI_CCTRCH_1_SI_TYPE_BMSK); \
  HWIO_TX_TDS_CCTRCH_SI_OUT(val); \
}
#endif

/* This register programs each CCTrCH's RMRAM wr start address */
#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_CCTRCH_ADDR(cctrch2_addr_dpch, \
                                     cctrch2_addr_bank_type, \
                                     cctrch1_addr_dpch, \
                                     cctrch1_addr_bank_type) \
{ \
	uint32 val = \
  ((uint32)((cctrch2_addr_bank_type) << TDSENCI_TX_TDS_CCTRCH_2_BANK_TYPE_SHIFT) & TDSENCI_TX_TDS_CCTRCH_2_BANK_TYPE_BMSK) | \
  ((uint32)((cctrch2_addr_dpch) << HWIO_TX_TDS_CCTRCH_ADDR_CCTRCH_2_ADDR_DPCH_SHFT) & TDSENCI_TX_TDS_CCTRCH_2_ADDR_DPCH_BMSK) | \
  ((uint32)((cctrch1_addr_bank_type) << TDSENCI_TX_TDS_CCTRCH_1_BANK_TYPE_SHIFT)& TDSENCI_TX_TDS_CCTRCH_1_BANK_TYPE_BMSK) | \
  ((uint32)((cctrch1_addr_dpch) << HWIO_TX_TDS_CCTRCH_ADDR_CCTRCH_1_ADDR_DPCH_SHFT) & TDSENCI_TX_TDS_CCTRCH_1_ADDR_DPCH_BMSK); \
  HWIO_TX_TDS_CCTRCH_ADDR_OUT(val); \
  TDSL1_MSG1(HIGH, "TX_TDS_CCTRCH_ADDR: val = %x",  val); \
}
#else
#define TDSENCI_WRITE_TX_CCTRCH_ADDR(cctrch2_addr_dpch, \
                                     cctrch2_addr_bank_type, \
                                     cctrch1_addr_dpch, \
                                     cctrch1_addr_bank_type) \
{ \
	uint32 val = \
  ((uint32)((cctrch2_addr_bank_type) << TDSENCI_TX_TDS_CCTRCH_2_BANK_TYPE_SHIFT) & TDSENCI_TX_TDS_CCTRCH_2_BANK_TYPE_BMSK) | \
  ((uint32)((cctrch2_addr_dpch) << HWIO_TX_TDS_CCTRCH_ADDR_CCTRCH_2_ADDR_DPCH_SHFT) & TDSENCI_TX_TDS_CCTRCH_2_ADDR_DPCH_BMSK) | \
  ((uint32)((cctrch1_addr_bank_type) << TDSENCI_TX_TDS_CCTRCH_1_BANK_TYPE_SHIFT)& TDSENCI_TX_TDS_CCTRCH_1_BANK_TYPE_BMSK) | \
  ((uint32)((cctrch1_addr_dpch) << HWIO_TX_TDS_CCTRCH_ADDR_CCTRCH_1_ADDR_DPCH_SHFT) & TDSENCI_TX_TDS_CCTRCH_1_ADDR_DPCH_BMSK); \
  HWIO_TX_TDS_CCTRCH_ADDR_OUT(val); \
}
#endif

/* This register programs the number of bits of slot 1 and 2 in a subframe */
#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_SLOT_DATA_NUM0_CCTRCH1(slot2_data_num, \
                                                slot1_data_num) \
{ \
	uint32 val = \
  ((uint32)((slot2_data_num) << HWIO_TX_TDS_SLOT_DATA_NUM0_CCTRCH1_SLOT2_DATA_NUM_SHFT) & HWIO_TX_TDS_SLOT_DATA_NUM0_CCTRCH1_SLOT2_DATA_NUM_BMSK) | \
  ((uint32)((slot1_data_num) << HWIO_TX_TDS_SLOT_DATA_NUM0_CCTRCH1_SLOT1_DATA_NUM_SHFT) & HWIO_TX_TDS_SLOT_DATA_NUM0_CCTRCH1_SLOT1_DATA_NUM_BMSK); \
  HWIO_TX_TDS_SLOT_DATA_NUM0_CCTRCH1_OUT(val); \
  TDSL1_MSG1(HIGH, "TX_TDS_SLOT_DATA_NUM0_CCTRCH1: val = %x",  val); \
}
#else
#define TDSENCI_WRITE_TX_SLOT_DATA_NUM0_CCTRCH1(slot2_data_num, \
                                                slot1_data_num) \
{ \
	uint32 val = \
  ((uint32)((slot2_data_num) << HWIO_TX_TDS_SLOT_DATA_NUM0_CCTRCH1_SLOT2_DATA_NUM_SHFT) & HWIO_TX_TDS_SLOT_DATA_NUM0_CCTRCH1_SLOT2_DATA_NUM_BMSK) | \
  ((uint32)((slot1_data_num) << HWIO_TX_TDS_SLOT_DATA_NUM0_CCTRCH1_SLOT1_DATA_NUM_SHFT) & HWIO_TX_TDS_SLOT_DATA_NUM0_CCTRCH1_SLOT1_DATA_NUM_BMSK); \
  HWIO_TX_TDS_SLOT_DATA_NUM0_CCTRCH1_OUT(val); \
}
#endif

/* This register programs the number of bits of slot 3 and 4 in a subframe */
#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_SLOT_DATA_NUM1_CCTRCH1(slot4_data_num, \
                                                slot3_data_num) \
{ \
	uint32 val = \
  ((uint32)((slot4_data_num) << HWIO_TX_TDS_SLOT_DATA_NUM1_CCTRCH1_SLOT4_DATA_NUM_SHFT) & HWIO_TX_TDS_SLOT_DATA_NUM1_CCTRCH1_SLOT4_DATA_NUM_BMSK) | \
  ((uint32)((slot3_data_num) << HWIO_TX_TDS_SLOT_DATA_NUM1_CCTRCH1_SLOT3_DATA_NUM_SHFT) & HWIO_TX_TDS_SLOT_DATA_NUM1_CCTRCH1_SLOT3_DATA_NUM_BMSK); \
  HWIO_TX_TDS_SLOT_DATA_NUM1_CCTRCH1_OUT (val); \
  TDSL1_MSG1(HIGH, "TX_TDS_SLOT_DATA_NUM1_CCTRCH1: val = %x",  val); \
}
#else
#define TDSENCI_WRITE_TX_SLOT_DATA_NUM1_CCTRCH1(slot4_data_num, \
                                                slot3_data_num) \
{ \
      uint32 val = \
    ((uint32)((slot4_data_num) << HWIO_TX_TDS_SLOT_DATA_NUM1_CCTRCH1_SLOT4_DATA_NUM_SHFT) & HWIO_TX_TDS_SLOT_DATA_NUM1_CCTRCH1_SLOT4_DATA_NUM_BMSK) | \
    ((uint32)((slot3_data_num) << HWIO_TX_TDS_SLOT_DATA_NUM1_CCTRCH1_SLOT3_DATA_NUM_SHFT) & HWIO_TX_TDS_SLOT_DATA_NUM1_CCTRCH1_SLOT3_DATA_NUM_BMSK); \
    HWIO_TX_TDS_SLOT_DATA_NUM1_CCTRCH1_OUT (val); \
}
#endif

/* This register programs the number of bits of slot 5 in a subframe */
#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_SLOT_DATA_NUM2_CCTRCH1(slot5_data_num) \
{ \
	uint32 val = \
  ((uint32)((slot5_data_num) << HWIO_TX_TDS_SLOT_DATA_NUM2_CCTRCH1_SLOT5_DATA_NUM_SHFT) & HWIO_TX_TDS_SLOT_DATA_NUM2_CCTRCH1_SLOT5_DATA_NUM_BMSK); \
  HWIO_TX_TDS_SLOT_DATA_NUM2_CCTRCH1_OUT(val);\
  TDSL1_MSG1(HIGH, "TX_TDS_SLOT_DATA_NUM2_CCTRCH1: val = %x",  val); \
}
#else
#define TDSENCI_WRITE_TX_SLOT_DATA_NUM2_CCTRCH1(slot5_data_num) \
{ \
	uint32 val = \
  ((uint32)((slot5_data_num) << HWIO_TX_TDS_SLOT_DATA_NUM2_CCTRCH1_SLOT5_DATA_NUM_SHFT) & HWIO_TX_TDS_SLOT_DATA_NUM2_CCTRCH1_SLOT5_DATA_NUM_BMSK); \
  HWIO_TX_TDS_SLOT_DATA_NUM2_CCTRCH1_OUT(val);\
}
#endif


/* This register programs the number of bits of slot 1 and 2 in a subframe */
#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_SLOT_DATA_NUM0_CCTRCH2(slot2_data_num, \
                                                slot1_data_num) \
{ \
	uint32 val = \
  ((uint32)((slot2_data_num) << HWIO_TX_TDS_SLOT_DATA_NUM0_CCTRCH2_SLOT2_DATA_NUM_SHFT) & HWIO_TX_TDS_SLOT_DATA_NUM0_CCTRCH2_SLOT2_DATA_NUM_BMSK) | \
  ((uint32)((slot1_data_num) << HWIO_TX_TDS_SLOT_DATA_NUM0_CCTRCH2_SLOT1_DATA_NUM_SHFT) & HWIO_TX_TDS_SLOT_DATA_NUM0_CCTRCH2_SLOT1_DATA_NUM_BMSK); \
  HWIO_TX_TDS_SLOT_DATA_NUM0_CCTRCH2_OUT(val);\
  TDSL1_MSG1(HIGH, "TX_TDS_SLOT_DATA_NUM0_CCTRCH2: val = %x",  val); \
}
#else
#define TDSENCI_WRITE_TX_SLOT_DATA_NUM0_CCTRCH2(slot2_data_num, \
                                                slot1_data_num) \
{ \
	uint32 val = \
  ((uint32)((slot2_data_num) << HWIO_TX_TDS_SLOT_DATA_NUM0_CCTRCH2_SLOT2_DATA_NUM_SHFT) & HWIO_TX_TDS_SLOT_DATA_NUM0_CCTRCH2_SLOT2_DATA_NUM_BMSK) | \
  ((uint32)((slot1_data_num) << HWIO_TX_TDS_SLOT_DATA_NUM0_CCTRCH2_SLOT1_DATA_NUM_SHFT) & HWIO_TX_TDS_SLOT_DATA_NUM0_CCTRCH2_SLOT1_DATA_NUM_BMSK); \
  HWIO_TX_TDS_SLOT_DATA_NUM0_CCTRCH2_OUT(val);\
}
#endif

/* This register programs the number of bits of slot 3 and 4 in a subframe */
#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_SLOT_DATA_NUM1_CCTRCH2(slot4_data_num, \
                                                slot3_data_num) \
{ \
	uint32 val = \
  ((uint32)((slot4_data_num) << HWIO_TX_TDS_SLOT_DATA_NUM1_CCTRCH2_SLOT4_DATA_NUM_SHFT) & HWIO_TX_TDS_SLOT_DATA_NUM1_CCTRCH2_SLOT4_DATA_NUM_BMSK) | \
  ((uint32)((slot3_data_num) << HWIO_TX_TDS_SLOT_DATA_NUM1_CCTRCH2_SLOT3_DATA_NUM_SHFT) & HWIO_TX_TDS_SLOT_DATA_NUM1_CCTRCH2_SLOT3_DATA_NUM_BMSK); \
  HWIO_TX_TDS_SLOT_DATA_NUM1_CCTRCH2_OUT(val); \
  TDSL1_MSG1(HIGH, "TX_TDS_SLOT_DATA_NUM1_CCTRCH2: val = %x",  val); \
}
#else
#define TDSENCI_WRITE_TX_SLOT_DATA_NUM1_CCTRCH2(slot4_data_num, \
                                                slot3_data_num) \
{ \
	uint32 val = \
  ((uint32)((slot4_data_num) << HWIO_TX_TDS_SLOT_DATA_NUM1_CCTRCH2_SLOT4_DATA_NUM_SHFT) & HWIO_TX_TDS_SLOT_DATA_NUM1_CCTRCH2_SLOT4_DATA_NUM_BMSK) | \
  ((uint32)((slot3_data_num) << HWIO_TX_TDS_SLOT_DATA_NUM1_CCTRCH2_SLOT3_DATA_NUM_SHFT) & HWIO_TX_TDS_SLOT_DATA_NUM1_CCTRCH2_SLOT3_DATA_NUM_BMSK); \
  HWIO_TX_TDS_SLOT_DATA_NUM1_CCTRCH2_OUT(val); \
}
#endif

/* This register programs the number of bits of slot 5 in a subframe */
#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_SLOT_DATA_NUM2_CCTRCH2(slot5_data_num) \
{ \
	uint32 val = \
  ((uint32)((slot5_data_num) << HWIO_TX_TDS_SLOT_DATA_NUM2_CCTRCH2_SLOT5_DATA_NUM_SHFT) & HWIO_TX_TDS_SLOT_DATA_NUM2_CCTRCH2_SLOT5_DATA_NUM_BMSK); \
  HWIO_TX_TDS_SLOT_DATA_NUM2_CCTRCH2_OUT(val); \
  TDSL1_MSG1(HIGH, "TX_TDS_SLOT_DATA_NUM2_CCTRCH2: val = %x",  val); \
}
#else
#define TDSENCI_WRITE_TX_SLOT_DATA_NUM2_CCTRCH2(slot5_data_num) \
{ \
	uint32 val = \
  ((uint32)((slot5_data_num) << HWIO_TX_TDS_SLOT_DATA_NUM2_CCTRCH2_SLOT5_DATA_NUM_SHFT) & HWIO_TX_TDS_SLOT_DATA_NUM2_CCTRCH2_SLOT5_DATA_NUM_BMSK); \
  HWIO_TX_TDS_SLOT_DATA_NUM2_CCTRCH2_OUT(val); \
}
#endif

/* ERAMa write data control.
 * TD_TX_ERAMA_INCRACC_INIT is a command register. A write to this register is 
 * used by hardware to reset the address pointer of ERAMA. This ensures that the 
 * following writes to ERAMA_DATA start from address location 0 of ERAMA 
 */
#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_ERAMA_INIT(dont_care) \
{ \
  uint32 val= ((uint32)((dont_care) << HWIO_TX_UNIFIED_ENC_ERAM_INCRACC_INIT_ERAM_INIT_SHFT) & HWIO_TX_UNIFIED_ENC_ERAM_INCRACC_INIT_ERAM_INIT_BMSK); \
	HWIO_TX_UNIFIED_ENC_ERAM_INCRACC_INIT_OUT(val); \
  TDSL1_MSG1(HIGH, "TX_UNIFIED_ENC_ERAM_INCRACC_INIT: val = %x",  val);\
}
#else
#define TDSENCI_WRITE_TX_ERAMA_INIT(dont_care) \
{ \
  uint32 val= ((uint32)((dont_care) << HWIO_TX_UNIFIED_ENC_ERAM_INCRACC_INIT_ERAM_INIT_SHFT) & HWIO_TX_UNIFIED_ENC_ERAM_INCRACC_INIT_ERAM_INIT_BMSK); \
	HWIO_TX_UNIFIED_ENC_ERAM_INCRACC_INIT_OUT(val); \
}
#endif

/* This register indicates if data in ERAMA is in little or big Endian format. This register is used only
for DCH/RACH mode
 */
#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_ENDIAN_FRMT_CTL(endian_frmt) \
{ \
  uint32 val = ((uint32)((endian_frmt) << HWIO_TX_UNIFIED_ENC_ENDIAN_FRMT_CTL_DCH_RACH_ENDIAN_FRMT_SHFT) & HWIO_TX_UNIFIED_ENC_ENDIAN_FRMT_CTL_DCH_RACH_ENDIAN_FRMT_BMSK); \
  HWIO_TX_UNIFIED_ENC_ENDIAN_FRMT_CTL_OUT(val);\
  TDSL1_MSG1(HIGH, "TX_UNIFIED_ENC_ENDIAN_FRMT_CTL: val = %x", val); \
}
#else
#define TDSENCI_WRITE_TX_ENDIAN_FRMT_CTL(endian_frmt) \
{ \
   uint32 val = ((uint32)((endian_frmt) << HWIO_TX_UNIFIED_ENC_ENDIAN_FRMT_CTL_DCH_RACH_ENDIAN_FRMT_SHFT) & HWIO_TX_UNIFIED_ENC_ENDIAN_FRMT_CTL_DCH_RACH_ENDIAN_FRMT_BMSK); \
  HWIO_TX_UNIFIED_ENC_ENDIAN_FRMT_CTL_OUT(val);\
}
#endif


#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_ERAMA_TB_CTRL_WORD(ciphering_en, \
                                            ciphering_mode, \
                                            hdr_size, \
                                            trch_cfg0_idx) \
{ \
	uint32 val = \
  ((uint32)((ciphering_en) << HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_1_CIPHERING_EN_W0_SHFT) & HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_1_CIPHERING_EN_W0_BMSK) | \
  ((uint32)((ciphering_mode) << HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_1_WILL_CONTINUE_W0_SHFT) & HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_1_WILL_CONTINUE_W0_BMSK) | \
  ((uint32)((hdr_size) << HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_1_HEADER_SIZE_W0_SHFT) & HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_1_HEADER_SIZE_W0_BMSK) | \
  ((uint32)((trch_cfg0_idx) << HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_1_ERAMA_TRCH_INDEX_W0_SHFT) & HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_1_ERAMA_TRCH_INDEX_W0_BMSK);\
  HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_1_OUT(val);\
  TDSL1_MSG1(HIGH, "TX_UNIFIED_ENC_ERAM_DATA_INCRACC_1: val = %x",  val); \
}
#else
#define TDSENCI_WRITE_TX_ERAMA_TB_CTRL_WORD(ciphering_en, \
                                            ciphering_mode, \
                                            hdr_size, \
                                            trch_cfg0_idx) \
{ \
	uint32 val = \
  ((uint32)((ciphering_en) << HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_1_CIPHERING_EN_W0_SHFT) & HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_1_CIPHERING_EN_W0_BMSK) | \
  ((uint32)((ciphering_mode) << HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_1_WILL_CONTINUE_W0_SHFT) & HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_1_WILL_CONTINUE_W0_BMSK) | \
  ((uint32)((hdr_size) << HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_1_HEADER_SIZE_W0_SHFT) & HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_1_HEADER_SIZE_W0_BMSK) | \
  ((uint32)((trch_cfg0_idx) << HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_1_ERAMA_TRCH_INDEX_W0_SHFT) & HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_1_ERAMA_TRCH_INDEX_W0_BMSK);\
  HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_1_OUT(val);\
}
#endif


/* If ciphering_en is 1, software programs two additional registers
 * (TD_TX_ERAMA_INCRACC_2/3) 
 */
#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_ERAMA_TB_CIPHERING_CTL_WORD1(algo, \
                                                      offset, \
                                                      ck_idx, \
                                                      bearer_id, \
                                                      cipher_len) \
{ \
	uint32 val = \
  ((uint32)((algo) << HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_2_ALGORITHM_W1_SHFT) & HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_2_ALGORITHM_W1_BMSK) | \
  ((uint32)((offset) <<HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_2_CIPHERING_OFFSET_W1_SHFT) & HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_2_CIPHERING_OFFSET_W1_BMSK) | \
  ((uint32)((ck_idx) << HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_2_CK_INDEX_W1_SHFT) & HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_2_CK_INDEX_W1_BMSK) | \
  (((TDSENCI_UL_CIPHERING_F8_ALG_DIR) << HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_2_DIRECTION_W1_SHFT) & HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_2_DIRECTION_W1_BMSK) | \
  ((uint32)((bearer_id) << HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_2_BEARER_W1_SHFT) & HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_2_BEARER_W1_BMSK) | \
  ((uint32)((cipher_len) << HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_2_LENGTH_W1_SHFT) & HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_2_LENGTH_W1_BMSK); \
  HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_2_OUT(val); \
  TDSL1_MSG1(HIGH, "TX_UNIFIED_ENC_ERAM_DATA_INCRACC_2: val = %x",  val); \
}
#else
#define TDSENCI_WRITE_TX_ERAMA_TB_CIPHERING_CTL_WORD1(algo, \
                                                      offset, \
                                                      ck_idx, \
                                                      bearer_id, \
                                                      cipher_len) \
{ \
	uint32 val = \
  ((uint32)((algo) << HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_2_ALGORITHM_W1_SHFT) & HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_2_ALGORITHM_W1_BMSK) | \
  ((uint32)((offset) <<HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_2_CIPHERING_OFFSET_W1_SHFT) & HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_2_CIPHERING_OFFSET_W1_BMSK) | \
  ((uint32)((ck_idx) << HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_2_CK_INDEX_W1_SHFT) & HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_2_CK_INDEX_W1_BMSK) | \
  (((TDSENCI_UL_CIPHERING_F8_ALG_DIR) << HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_2_DIRECTION_W1_SHFT) & HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_2_DIRECTION_W1_BMSK) | \
  ((uint32)((bearer_id) << HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_2_BEARER_W1_SHFT) & HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_2_BEARER_W1_BMSK) | \
  ((uint32)((cipher_len) << HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_2_LENGTH_W1_SHFT) & HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_2_LENGTH_W1_BMSK); \
  HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_2_OUT(val); \
}
#endif
  

/* Ciphering parameters for confidentiality f8 algorithm */
#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_ERAMA_TB_CIPHERING_CTL_WORD2(count) \
{ \
	uint32 val= ((uint32)(count) << HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_3_COUNT_W1_SHFT) & HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_3_COUNT_W1_BMSK; \
  HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_3_OUT(val); \
  TDSL1_MSG1(HIGH, "TX_UNIFIED_ENC_ERAM_DATA_INCRACC_3: val = %x",  val); \
}
#else
#define TDSENCI_WRITE_TX_ERAMA_TB_CIPHERING_CTL_WORD2(count) \
{ \
	uint32 val= ((uint32)(count) << HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_3_COUNT_W1_SHFT) & HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_3_COUNT_W1_BMSK; \
  HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_3_OUT(val); \
}
#endif 

/* TD_TX_ERAMA_DATA provides transport block data to be written to the encoder 
 * RAM(ERAMA). Any software writes to TD_TX_ERAMA_INCRACC will write to the Encoder 
 * RAM at an address that is determined by the hardware write address pointer. 
 * This write pointer will be reset by the register TD_TX_ERAMA_INCRACC_INIT and 
 * every successive write to this address range will increment the write address 
 * pointer. 
 */
#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_ERAMA_DATA(erama_data) \
{	\
	uint32 val=((uint32)(erama_data) << HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_0_ERAM_DATA_W0_SHFT) & HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_0_ERAM_DATA_W0_BMSK;\
  HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_0_OUT(val); \
  TDSL1_MSG1(HIGH, "TX_UNIFIED_ENC_ERAM_DATA_INCRACC_0: val = %lx",  val); \
}
#else
#define TDSENCI_WRITE_TX_ERAMA_DATA(erama_data) \
{	\
	uint32 val=((uint32)(erama_data) << HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_0_ERAM_DATA_W0_SHFT) & HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_0_ERAM_DATA_W0_BMSK;\
  HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_0_OUT(val); \
}
#endif

#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_ERAM_CTL(eram_bank) \
{ \
	uint32 val = \
  ((uint32)((eram_bank)  << HWIO_TX_UNIFIED_ERAM_CTL_ERAM_BANK_SEL_SHFT) & HWIO_TX_UNIFIED_ERAM_CTL_ERAM_BANK_SEL_BMSK); \
  HWIO_TX_UNIFIED_ERAM_CTL_OUT(val); \
  TDSL1_MSG1(HIGH, "TX_UNIFIED_ERAM_CTL   val = %x", val); \
}
#else
#define TDSENCI_WRITE_TX_ERAM_CTL(eram_bank) \
{ \
	uint32 val = \
  ((uint32)((eram_bank)  << HWIO_TX_UNIFIED_ERAM_CTL_ERAM_BANK_SEL_SHFT) & HWIO_TX_UNIFIED_ERAM_CTL_ERAM_BANK_SEL_BMSK); \
  HWIO_TX_UNIFIED_ERAM_CTL_OUT(val); \
}
#endif


/* This register provides the last row/col of DCH/RACH data present in IRAM for a slot for physical channel 0 */
#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_DPCH0_LAST_SLOTc_BANK0(slot_num, last_row,  last_col) \
{ \
	uint32 val = \
  ((uint32)((last_row) << HWIO_TX_TDS_DPCH0_LAST_SLOTc_BANK0_LAST_ROW_SHFT) & HWIO_TX_TDS_DPCH0_LAST_SLOTc_BANK0_LAST_ROW_BMSK) | \
  ((uint32)((last_col)  << HWIO_TX_TDS_DPCH0_LAST_SLOTc_BANK0_LAST_COL_SHFT) & HWIO_TX_TDS_DPCH0_LAST_SLOTc_BANK0_LAST_COL_BMSK); \
  HWIO_TX_TDS_DPCH0_LAST_SLOTc_BANK0_OUTI(slot_num, val); \
  TDSL1_MSG2(HIGH, "TX_TDS_DPCH0_LAST_SLOTc_BANK0  c = %d: val = %x", slot_num, val); \
}
#else
#define TDSENCI_WRITE_TX_DPCH0_LAST_SLOTc_BANK0(slot_num, last_row,  last_col) \
{ \
	uint32 val = \
  ((uint32)((last_row) << HWIO_TX_TDS_DPCH0_LAST_SLOTc_BANK0_LAST_ROW_SHFT) & HWIO_TX_TDS_DPCH0_LAST_SLOTc_BANK0_LAST_ROW_BMSK) | \
  ((uint32)((last_col)  << HWIO_TX_TDS_DPCH0_LAST_SLOTc_BANK0_LAST_COL_SHFT) & HWIO_TX_TDS_DPCH0_LAST_SLOTc_BANK0_LAST_COL_BMSK);  \
  HWIO_TX_TDS_DPCH0_LAST_SLOTc_BANK0_OUTI(slot_num, val); \
}
#endif

#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_DPCH0_LAST_SLOTc_BANK1(slot_num, last_row,  last_col) \
{ \
      uint32 val = \
    ((uint32)((last_row) << HWIO_TX_TDS_DPCH0_LAST_SLOTc_BANK1_LAST_ROW_SHFT) & HWIO_TX_TDS_DPCH0_LAST_SLOTc_BANK1_LAST_ROW_BMSK) | \
    ((uint32)((last_col)  << HWIO_TX_TDS_DPCH0_LAST_SLOTc_BANK1_LAST_COL_SHFT) & HWIO_TX_TDS_DPCH0_LAST_SLOTc_BANK1_LAST_COL_BMSK); \
    HWIO_TX_TDS_DPCH0_LAST_SLOTc_BANK1_OUTI(slot_num, val); \
    TDSL1_MSG2(HIGH, "TX_TDS_DPCH0_LAST_SLOTc_BANK1  c = %d: val = %x", slot_num, val); \
}
#else
#define TDSENCI_WRITE_TX_DPCH0_LAST_SLOTc_BANK1(slot_num, last_row,  last_col) \
{ \
      uint32 val = \
    ((uint32)((last_row) << HWIO_TX_TDS_DPCH0_LAST_SLOTc_BANK1_LAST_ROW_SHFT) & HWIO_TX_TDS_DPCH0_LAST_SLOTc_BANK1_LAST_ROW_BMSK) | \
    ((uint32)((last_col)  << HWIO_TX_TDS_DPCH0_LAST_SLOTc_BANK1_LAST_COL_SHFT) & HWIO_TX_TDS_DPCH0_LAST_SLOTc_BANK1_LAST_COL_BMSK); \
    HWIO_TX_TDS_DPCH0_LAST_SLOTc_BANK1_OUTI(slot_num, val); \
}
#endif

#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_DPCH1_LAST_SLOTc_BANK0(slot_num, last_row,  last_col) \
{ \
    uint32 val = \
  ((uint32)((last_row) << HWIO_TX_TDS_DPCH1_LAST_SLOTc_BANK0_LAST_ROW_SHFT) & HWIO_TX_TDS_DPCH1_LAST_SLOTc_BANK0_LAST_ROW_BMSK) | \
  ((uint32)((last_col)  << HWIO_TX_TDS_DPCH1_LAST_SLOTc_BANK0_LAST_COL_SHFT) & HWIO_TX_TDS_DPCH1_LAST_SLOTc_BANK0_LAST_COL_BMSK); \
  HWIO_TX_TDS_DPCH1_LAST_SLOTc_BANK0_OUTI(slot_num, val); \
  TDSL1_MSG2(HIGH, "TX_TDS_DPCH1_LAST_SLOTc_BANK0  c = %d: val = %x", slot_num, val); \
}
#else
#define TDSENCI_WRITE_TX_DPCH1_LAST_SLOTc_BANK0(slot_num, last_row,  last_col) \
{ \
    uint32 val = \
  ((uint32)((last_row) << HWIO_TX_TDS_DPCH1_LAST_SLOTc_BANK0_LAST_ROW_SHFT) & HWIO_TX_TDS_DPCH1_LAST_SLOTc_BANK0_LAST_ROW_BMSK) | \
  ((uint32)((last_col)  << HWIO_TX_TDS_DPCH1_LAST_SLOTc_BANK0_LAST_COL_SHFT) & HWIO_TX_TDS_DPCH1_LAST_SLOTc_BANK0_LAST_COL_BMSK); \
  HWIO_TX_TDS_DPCH1_LAST_SLOTc_BANK0_OUTI(slot_num, val); \
}
#endif

#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_DPCH1_LAST_SLOTc_BANK1(slot_num, last_row,  last_col) \
{ \
      uint32 val = \
    ((uint32)((last_row) << HWIO_TX_TDS_DPCH1_LAST_SLOTc_BANK1_LAST_ROW_SHFT) & HWIO_TX_TDS_DPCH1_LAST_SLOTc_BANK1_LAST_ROW_BMSK) | \
    ((uint32)((last_col)  << HWIO_TX_TDS_DPCH1_LAST_SLOTc_BANK1_LAST_COL_SHFT) & HWIO_TX_TDS_DPCH1_LAST_SLOTc_BANK1_LAST_COL_BMSK); \
    HWIO_TX_TDS_DPCH1_LAST_SLOTc_BANK1_OUTI(slot_num, val); \
    TDSL1_MSG2(HIGH, "TX_TDS_DPCH1_LAST_SLOTc_BANK1  c = %d: val = %x", slot_num, val); \
}
#else
#define TDSENCI_WRITE_TX_DPCH1_LAST_SLOTc_BANK1(slot_num, last_row,  last_col) \
{ \
      uint32 val = \
    ((uint32)((last_row) << HWIO_TX_TDS_DPCH1_LAST_SLOTc_BANK1_LAST_ROW_SHFT) & HWIO_TX_TDS_DPCH1_LAST_SLOTc_BANK1_LAST_ROW_BMSK) | \
    ((uint32)((last_col)  << HWIO_TX_TDS_DPCH1_LAST_SLOTc_BANK1_LAST_COL_SHFT) & HWIO_TX_TDS_DPCH1_LAST_SLOTc_BANK1_LAST_COL_BMSK); \
    HWIO_TX_TDS_DPCH1_LAST_SLOTc_BANK1_OUTI(slot_num, val); \
}
#endif

#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)                         
#define TDSENCI_WRITE_TX_DPCH0_SLOTc_BANK0(slot_num,num_jumps, bits_last_jump, start_row, start_col) \
{ \
      uint32 val = \
    ((uint32)((num_jumps) << HWIO_TX_TDS_DPCH0_SLOTc_BANK0_NUM_JUMPS_SHFT) & HWIO_TX_TDS_DPCH0_SLOTc_BANK0_NUM_JUMPS_BMSK) | \
    ((uint32)((bits_last_jump)  << HWIO_TX_TDS_DPCH0_SLOTc_BANK0_NUM_BITS_LAST_JUMP_SHFT) & HWIO_TX_TDS_DPCH0_SLOTc_BANK0_NUM_BITS_LAST_JUMP_BMSK)  | \
    ((uint32)((start_row) << HWIO_TX_TDS_DPCH0_SLOTc_BANK0_START_ROW_SHFT) & HWIO_TX_TDS_DPCH0_SLOTc_BANK0_START_ROW_BMSK) | \
    ((uint32)((start_col)  << HWIO_TX_TDS_DPCH0_SLOTc_BANK0_START_COL_SHFT) & HWIO_TX_TDS_DPCH0_SLOTc_BANK0_START_COL_BMSK); \
    HWIO_TX_TDS_DPCH0_SLOTc_BANK0_OUTI(slot_num, val); \
    TDSL1_MSG2(HIGH, "TX_TDS_DPCH0_SLOTc_BANK0  c = %d: val = %x", slot_num, val); \
}
#else
#define TDSENCI_WRITE_TX_DPCH0_SLOTc_BANK0(slot_num,num_jumps, bits_last_jump, start_row, start_col) \
{ \
      uint32 val = \
    ((uint32)((num_jumps) << HWIO_TX_TDS_DPCH0_SLOTc_BANK0_NUM_JUMPS_SHFT) & HWIO_TX_TDS_DPCH0_SLOTc_BANK0_NUM_JUMPS_BMSK) | \
    ((uint32)((bits_last_jump)  << HWIO_TX_TDS_DPCH0_SLOTc_BANK0_NUM_BITS_LAST_JUMP_SHFT) & HWIO_TX_TDS_DPCH0_SLOTc_BANK0_NUM_BITS_LAST_JUMP_BMSK)  | \
    ((uint32)((start_row) << HWIO_TX_TDS_DPCH0_SLOTc_BANK0_START_ROW_SHFT) & HWIO_TX_TDS_DPCH0_SLOTc_BANK0_START_ROW_BMSK) | \
    ((uint32)((start_col)  << HWIO_TX_TDS_DPCH0_SLOTc_BANK0_START_COL_SHFT) & HWIO_TX_TDS_DPCH0_SLOTc_BANK0_START_COL_BMSK); \
    HWIO_TX_TDS_DPCH0_SLOTc_BANK0_OUTI(slot_num, val); \
}
#endif

#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_DPCH0_SLOTc_BANK1(slot_num,num_jumps, bits_last_jump, start_row, start_col) \
{ \
      uint32 val = \
    ((uint32)((num_jumps) << HWIO_TX_TDS_DPCH0_SLOTc_BANK1_NUM_JUMPS_SHFT) & HWIO_TX_TDS_DPCH0_SLOTc_BANK1_NUM_JUMPS_BMSK) | \
    ((uint32)((bits_last_jump)  << HWIO_TX_TDS_DPCH0_SLOTc_BANK1_NUM_BITS_LAST_JUMP_SHFT) & HWIO_TX_TDS_DPCH0_SLOTc_BANK1_NUM_BITS_LAST_JUMP_BMSK)  | \
    ((uint32)((start_row) << HWIO_TX_TDS_DPCH0_SLOTc_BANK1_START_ROW_SHFT) & HWIO_TX_TDS_DPCH0_SLOTc_BANK1_START_ROW_BMSK) | \
    ((uint32)((start_col)  << HWIO_TX_TDS_DPCH0_SLOTc_BANK1_START_COL_SHFT) & HWIO_TX_TDS_DPCH0_SLOTc_BANK1_START_COL_BMSK); \
    HWIO_TX_TDS_DPCH0_SLOTc_BANK1_OUTI(slot_num, val); \
    TDSL1_MSG2(HIGH, "TX_TDS_DPCH0_SLOTc_BANK1  c = %d: val = %x", slot_num, val); \
}
#else
#define TDSENCI_WRITE_TX_DPCH0_SLOTc_BANK1(slot_num,num_jumps, bits_last_jump, start_row, start_col) \
{ \
      uint32 val = \
    ((uint32)((num_jumps) << HWIO_TX_TDS_DPCH0_SLOTc_BANK1_NUM_JUMPS_SHFT) & HWIO_TX_TDS_DPCH0_SLOTc_BANK1_NUM_JUMPS_BMSK) | \
    ((uint32)((bits_last_jump)  << HWIO_TX_TDS_DPCH0_SLOTc_BANK1_NUM_BITS_LAST_JUMP_SHFT) & HWIO_TX_TDS_DPCH0_SLOTc_BANK1_NUM_BITS_LAST_JUMP_BMSK)  | \
    ((uint32)((start_row) << HWIO_TX_TDS_DPCH0_SLOTc_BANK1_START_ROW_SHFT) & HWIO_TX_TDS_DPCH0_SLOTc_BANK1_START_ROW_BMSK) | \
    ((uint32)((start_col)  << HWIO_TX_TDS_DPCH0_SLOTc_BANK1_START_COL_SHFT) & HWIO_TX_TDS_DPCH0_SLOTc_BANK1_START_COL_BMSK); \
    HWIO_TX_TDS_DPCH0_SLOTc_BANK1_OUTI(slot_num, val); \
}
#endif

#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_DPCH1_SLOTc_BANK0(slot_num, bits_before_first_jump, bits_first_jump, start_row, start_col) \
{ \
      uint32 val = \
    ((uint32)((bits_before_first_jump) << HWIO_TX_TDS_DPCH1_SLOTc_BANK0_BITS_BEFORE_FIRST_JUMP_SHFT) & HWIO_TX_TDS_DPCH1_SLOTc_BANK0_BITS_BEFORE_FIRST_JUMP_BMSK) | \
    ((uint32)((bits_first_jump)  << HWIO_TX_TDS_DPCH1_SLOTc_BANK0_BITS_FIRST_JUMP_SHFT) & HWIO_TX_TDS_DPCH1_SLOTc_BANK0_BITS_FIRST_JUMP_BMSK)  | \
    ((uint32)((start_row) << HWIO_TX_TDS_DPCH1_SLOTc_BANK0_START_ROW_SHFT) & HWIO_TX_TDS_DPCH1_SLOTc_BANK0_START_ROW_BMSK) | \
    ((uint32)((start_col)  << HWIO_TX_TDS_DPCH1_SLOTc_BANK0_START_COL_SHFT) & HWIO_TX_TDS_DPCH1_SLOTc_BANK0_START_COL_BMSK); \
    HWIO_TX_TDS_DPCH1_SLOTc_BANK0_OUTI(slot_num, val); \
    TDSL1_MSG2(HIGH, "TX_TDS_DPCH1_SLOTc_BANK0  c = %d: val = %x", slot_num, val); \
}   
#else
#define TDSENCI_WRITE_TX_DPCH1_SLOTc_BANK0(slot_num, bits_before_first_jump, bits_first_jump, start_row, start_col) \
{ \
      uint32 val = \
    ((uint32)((bits_before_first_jump) << HWIO_TX_TDS_DPCH1_SLOTc_BANK0_BITS_BEFORE_FIRST_JUMP_SHFT) & HWIO_TX_TDS_DPCH1_SLOTc_BANK0_BITS_BEFORE_FIRST_JUMP_BMSK) | \
    ((uint32)((bits_first_jump)  << HWIO_TX_TDS_DPCH1_SLOTc_BANK0_BITS_FIRST_JUMP_SHFT) & HWIO_TX_TDS_DPCH1_SLOTc_BANK0_BITS_FIRST_JUMP_BMSK)  | \
    ((uint32)((start_row) << HWIO_TX_TDS_DPCH1_SLOTc_BANK0_START_ROW_SHFT) & HWIO_TX_TDS_DPCH1_SLOTc_BANK0_START_ROW_BMSK) | \
    ((uint32)((start_col)  << HWIO_TX_TDS_DPCH1_SLOTc_BANK0_START_COL_SHFT) & HWIO_TX_TDS_DPCH1_SLOTc_BANK0_START_COL_BMSK); \
    HWIO_TX_TDS_DPCH1_SLOTc_BANK0_OUTI(slot_num, val); \
}   
#endif

#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_DPCH1_SLOTc_BANK1(slot_num, bits_before_first_jump, bits_first_jump, start_row, start_col) \
{ \
      uint32 val = \
    ((uint32)((bits_before_first_jump) << HWIO_TX_TDS_DPCH1_SLOTc_BANK1_BITS_BEFORE_FIRST_JUMP_SHFT) & HWIO_TX_TDS_DPCH1_SLOTc_BANK1_BITS_BEFORE_FIRST_JUMP_BMSK) | \
    ((uint32)((bits_first_jump)  << HWIO_TX_TDS_DPCH1_SLOTc_BANK1_BITS_FIRST_JUMP_SHFT) & HWIO_TX_TDS_DPCH1_SLOTc_BANK1_BITS_FIRST_JUMP_BMSK)  | \
    ((uint32)((start_row) << HWIO_TX_TDS_DPCH1_SLOTc_BANK1_START_ROW_SHFT) & HWIO_TX_TDS_DPCH1_SLOTc_BANK1_START_ROW_BMSK) | \
    ((uint32)((start_col)  << HWIO_TX_TDS_DPCH1_SLOTc_BANK1_START_COL_SHFT) & HWIO_TX_TDS_DPCH1_SLOTc_BANK1_START_COL_BMSK); \
    HWIO_TX_TDS_DPCH1_SLOTc_BANK1_OUTI(slot_num, val); \
    TDSL1_MSG2(HIGH, "TX_TDS_DPCH1_SLOTc_BANK1  c = %d: val = %x", slot_num, val); \
}   
#else
#define TDSENCI_WRITE_TX_DPCH1_SLOTc_BANK1(slot_num, bits_before_first_jump, bits_first_jump, start_row, start_col) \
{ \
      uint32 val = \
    ((uint32)((bits_before_first_jump) << HWIO_TX_TDS_DPCH1_SLOTc_BANK1_BITS_BEFORE_FIRST_JUMP_SHFT) & HWIO_TX_TDS_DPCH1_SLOTc_BANK1_BITS_BEFORE_FIRST_JUMP_BMSK) | \
    ((uint32)((bits_first_jump)  << HWIO_TX_TDS_DPCH1_SLOTc_BANK1_BITS_FIRST_JUMP_SHFT) & HWIO_TX_TDS_DPCH1_SLOTc_BANK1_BITS_FIRST_JUMP_BMSK)  | \
    ((uint32)((start_row) << HWIO_TX_TDS_DPCH1_SLOTc_BANK1_START_ROW_SHFT) & HWIO_TX_TDS_DPCH1_SLOTc_BANK1_START_ROW_BMSK) | \
    ((uint32)((start_col)  << HWIO_TX_TDS_DPCH1_SLOTc_BANK1_START_COL_SHFT) & HWIO_TX_TDS_DPCH1_SLOTc_BANK1_START_COL_BMSK); \
    HWIO_TX_TDS_DPCH1_SLOTc_BANK1_OUTI(slot_num, val); \
}   
#endif


#define TDSENCI_WRITE_TX_DPCH0_LAST_SLOTc(bank_id, slot_num, last_row, last_col) \
                            if(0 == bank_id) \
                                TDSENCI_WRITE_TX_DPCH0_LAST_SLOTc_BANK0(slot_num, last_row, last_col)\
                            else \
                                TDSENCI_WRITE_TX_DPCH0_LAST_SLOTc_BANK1(slot_num, last_row, last_col) 

#define TDSENCI_WRITE_TX_DPCH1_LAST_SLOTc(bank_id, slot_num, last_row, last_col) \
                            if(0 == bank_id) \
                                TDSENCI_WRITE_TX_DPCH1_LAST_SLOTc_BANK0(slot_num, last_row, last_col)\
                            else \
                                TDSENCI_WRITE_TX_DPCH1_LAST_SLOTc_BANK1(slot_num, last_row, last_col) 

#define TDSENCI_WRITE_TX_DPCH0_SLOTc(bank_id,slot_num,num_jumps, bits_last_jump, start_row, start_col) \
                            if(0 == bank_id) \
                                TDSENCI_WRITE_TX_DPCH0_SLOTc_BANK0(slot_num,num_jumps, bits_last_jump, start_row, start_col)\
                            else \
                                TDSENCI_WRITE_TX_DPCH0_SLOTc_BANK1(slot_num,num_jumps, bits_last_jump, start_row, start_col) 

#define TDSENCI_WRITE_TX_DPCH1_SLOTc(bank_id, slot_num, bits_before_first_jump, bits_first_jump, start_row, start_col) \
                            if(0 == bank_id) \
                                TDSENCI_WRITE_TX_DPCH1_SLOTc_BANK0(slot_num, bits_before_first_jump, bits_first_jump, start_row, start_col)\
                            else \
                                TDSENCI_WRITE_TX_DPCH1_SLOTc_BANK1(slot_num, bits_before_first_jump, bits_first_jump, start_row, start_col)  

#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_MEM_PTR_EN(enable) \
{ \
    uint32 val = ((uint32)((enable) << HWIO_TX_UNIFIED_MEM_PTR_EN_DATA_SHFT) & HWIO_TX_UNIFIED_MEM_PTR_EN_DATA_BMSK); \
  HWIO_TX_UNIFIED_MEM_PTR_EN_OUT(val);\
  TDSL1_MSG1(HIGH, "TX_UNIFIED_MEM_PTR_EN: val = %x", val); \
}
#else
#define TDSENCI_WRITE_MEM_PTR_EN(enable) \
{ \
    uint32 val = ((uint32)((enable) << HWIO_TX_UNIFIED_MEM_PTR_EN_DATA_SHFT) & HWIO_TX_UNIFIED_MEM_PTR_EN_DATA_BMSK); \
  HWIO_TX_UNIFIED_MEM_PTR_EN_OUT(val);\
}
#endif

#ifdef FEATURE_TDSCDMA_HSUPA

#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_ERAM_CTL_ERUCCH(eram_bank_sel) \
{ \
	uint32 val = \
		((uint32)((eram_bank_sel) << HWIO_TX_UNIFIED_ERAM_CTL_ERAM_BANK_SEL_SHFT) & HWIO_TX_UNIFIED_ERAM_CTL_ERAM_BANK_SEL_BMSK); \
	HWIO_TX_UNIFIED_ERAM_CTL_OUT(val); \
	TDSL1_MSG1(HIGH, "TX_UNIFIED_ERAM_CTL: val = %x", val); \
}
#else
#define TDSENCI_WRITE_TX_ERAM_CTL_ERUCCH(eram_bank_sel) \
{ \
	uint32 val = \
		((uint32)((eram_bank_sel) << HWIO_TX_UNIFIED_ERAM_CTL_ERAM_BANK_SEL_SHFT) & HWIO_TX_UNIFIED_ERAM_CTL_ERAM_BANK_SEL_BMSK); \
	HWIO_TX_UNIFIED_ERAM_CTL_OUT(val); \
}
#endif

#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_ERAM_INCRACC_INIT_ERUCCH(inacc_init) \
{ \
	uint32 val = \
		((uint32)((inacc_init) << HWIO_TX_UNIFIED_ENC_ERAM_INCRACC_INIT_ERAM_INIT_SHFT) & HWIO_TX_UNIFIED_ENC_ERAM_INCRACC_INIT_ERAM_INIT_BMSK); \
	HWIO_TX_UNIFIED_ENC_ERAM_INCRACC_INIT_OUT(val); \
	TDSL1_MSG1(HIGH, "TX_UNIFIED_ENC_ERAM_INCRACC_INIT: val = %x", val); \
}
#else
#define TDSENCI_WRITE_TX_ERAM_INCRACC_INIT_ERUCCH(inacc_init) \
{ \
	uint32 val = \
		((uint32)((inacc_init) << HWIO_TX_UNIFIED_ENC_ERAM_INCRACC_INIT_ERAM_INIT_SHFT) & HWIO_TX_UNIFIED_ENC_ERAM_INCRACC_INIT_ERAM_INIT_BMSK); \
	HWIO_TX_UNIFIED_ENC_ERAM_INCRACC_INIT_OUT(val); \
}
#endif

#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_ERUCCH_DATA0(byte0, byte1, byte2, byte3) \
{ \
	uint32 val = \
  ((uint32)(((byte0) & 0xFF) << 24)) | \
  ((uint32)(((byte1) & 0xFF) << 16)) | \
  ((uint32)(((byte2) & 0xFF) << 8)) | \
  ((uint32)((byte3) & 0xFF)); \
  HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_0_OUT(val); \
  TDSL1_MSG1(HIGH, "TX_UNIFIED_ENC_ERAM_DATA_INCRACC_0: val = %lx", val);\
}
#else
#define TDSENCI_WRITE_TX_ERUCCH_DATA0(byte0, byte1, byte2, byte3) \
{ \
	uint32 val = \
  ((uint32)(((byte0) & 0xFF) << 24)) | \
  ((uint32)(((byte1) & 0xFF) << 16)) | \
  ((uint32)(((byte2) & 0xFF) << 8)) | \
  ((uint32)((byte3) & 0xFF)); \
  HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_0_OUT(val); \
  TDSL1_MSG1(HIGH, "TX_UNIFIED_ENC_ERAM_DATA_INCRACC_0: val = %x", val);	\
}
#endif

#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_ERUCCH_DATA1(byte4) \
{	\
	uint32 val = \
	  ((uint32)(((byte4) & 0xFF) << 24)) | \
	  ((uint32)(((0) & 0xFF) << 16)) | \
	  ((uint32)(((0) & 0xFF) << 8)) | \
	  ((uint32)((0) & 0xFF)); \
	HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_0_OUT(val); \
  TDSL1_MSG1(HIGH, "TX_UNIFIED_ENC_ERAM_DATA_INCRACC_0: val = %x", val);	\
}
#else
#define TDSENCI_WRITE_TX_ERUCCH_DATA1(byte4) \
{	\
  uint32 val = \
	  ((uint32)(((byte4) & 0xFF) << 24)) | \
	  ((uint32)(((0) & 0xFF) << 16)) | \
	  ((uint32)(((0) & 0xFF) << 8)) | \
	  ((uint32)((0) & 0xFF)); \
	HWIO_TX_UNIFIED_ENC_ERAM_DATA_INCRACC_0_OUT(val); \
  TDSL1_MSG1(HIGH, "TX_UNIFIED_ENC_ERAM_DATA_INCRACC_0: val = %x", val);	\
}
#endif

/* This register programs the initial value of the error variables (eini) used
 * in the rate matching algorithm
 */
#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_RM_EINI_ERUCCH(initial_error) \
{	\
  uint32 val=((uint32)(initial_error) << HWIO_TX_TDS_RM_EINI_ERUCCH_INITIAL_ERROR_ERUCCH_SHFT) & HWIO_TX_TDS_RM_EINI_ERUCCH_INITIAL_ERROR_ERUCCH_BMSK; \
	HWIO_TX_TDS_RM_EINI_ERUCCH_OUT(val); \
  TDSL1_MSG1(HIGH, "TX_TDS_RM_EINI_ERUCCH: val = %x", val); \
}
#else
#define TDSENCI_WRITE_TX_RM_EINI_ERUCCH(initial_error) \
{	\
  uint32 val=((uint32)(initial_error) << HWIO_TX_TDS_RM_EINI_ERUCCH_INITIAL_ERROR_ERUCCH_SHFT) & HWIO_TX_TDS_RM_EINI_ERUCCH_INITIAL_ERROR_ERUCCH_BMSK; \
	HWIO_TX_TDS_RM_EINI_ERUCCH_OUT(val); \
}
#endif

/* This register programs the decrement value of the error variables (eminus)
 * used in the rate matching algorithm
 */
#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_RM_EMINUS_ERUCCH(eminus) \
{ \
	uint32 val=((uint32)(eminus) << HWIO_TX_TDS_RM_EMINUS_ERUCCH_EMINUS_ERUCCH_SHFT) & HWIO_TX_TDS_RM_EMINUS_ERUCCH_EMINUS_ERUCCH_BMSK;\
  HWIO_TX_TDS_RM_EMINUS_ERUCCH_OUT(val); \
  TDSL1_MSG1(HIGH, "TX_TDS_RM_EMINUS_ERUCCH: val = %x", val);	\
}
#else
#define TDSENCI_WRITE_TX_RM_EMINUS_ERUCCH(eminus) \
{ \
	uint32 val=((uint32)(eminus) << HWIO_TX_TDS_RM_EMINUS_ERUCCH_EMINUS_ERUCCH_SHFT) & HWIO_TX_TDS_RM_EMINUS_ERUCCH_EMINUS_ERUCCH_BMSK;\
  HWIO_TX_TDS_RM_EMINUS_ERUCCH_OUT(val); \
}
#endif

/* This register programs the increment value of the error variables (eplus)
 * used in the rate matching algorithm
 */
#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_RM_EPLUS_ERUCCH(eplus) \
{	\
  uint32 val=((uint32)(eplus) << HWIO_TX_TDS_RM_EPLUS_ERUCCH_EPLUS_ERUCCH_SHFT) & HWIO_TX_TDS_RM_EPLUS_ERUCCH_EPLUS_ERUCCH_BMSK;\
  HWIO_TX_TDS_RM_EPLUS_ERUCCH_OUT(val); \
  TDSL1_MSG1(HIGH, "TX_TDS_RM_EPLUS_ERUCCH: val = %x", val); \
}
#else
#define TDSENCI_WRITE_TX_RM_EPLUS_ERUCCH(eplus) \
{	\
  uint32 val=((uint32)(eplus) << HWIO_TX_TDS_RM_EPLUS_ERUCCH_EPLUS_ERUCCH_SHFT) & HWIO_TX_TDS_RM_EPLUS_ERUCCH_EPLUS_ERUCCH_BMSK;\
  HWIO_TX_TDS_RM_EPLUS_ERUCCH_OUT(val); \
}
#endif

/* This register programs TTI for E-RUCCH */
#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_TTI_ERUCCH(tti) \
{	\
	uint32 val=((uint32)(tti) << HWIO_TX_TDS_TTI_ERUCCH_ERUCCH_TTI_SHFT) & HWIO_TX_TDS_TTI_ERUCCH_ERUCCH_TTI_BMSK; \
  HWIO_TX_TDS_TTI_ERUCCH_OUT(val); \
  TDSL1_MSG1(HIGH, "TX_TDS_TTI_ERUCCH: val = %x", tti);	\
}
#else
#define TDSENCI_WRITE_TX_TTI_ERUCCH(tti) \
{	\
	uint32 val=((uint32)(tti) << HWIO_TX_TDS_TTI_ERUCCH_ERUCCH_TTI_SHFT) & HWIO_TX_TDS_TTI_ERUCCH_ERUCCH_TTI_BMSK; \
  HWIO_TX_TDS_TTI_ERUCCH_OUT(val); \
}
#endif

/* This register programs each CCTrCH's RMRAM wr start address. */
#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_ERUCCH_ADDR(bank_type) \
{ \
	uint32 val = \
    ((uint32)((bank_type) << HWIO_TX_TDS_ERUCCH_ADDR_ERUCCH_BANK_SEL_SHFT) & HWIO_TX_TDS_ERUCCH_ADDR_ERUCCH_BANK_SEL_BMSK); \
  HWIO_TX_TDS_ERUCCH_ADDR_OUT(val); \
  TDSL1_MSG1(HIGH, "TX_TDS_ERUCCH_ADDR: val = %lx", val);\
}
#else
#define TDSENCI_WRITE_TX_ERUCCH_ADDR(bank_type) \
{ \
	uint32 val = \
    ((uint32)((bank_type) << HWIO_TX_TDS_ERUCCH_ADDR_ERUCCH_BANK_SEL_SHFT) & HWIO_TX_TDS_ERUCCH_ADDR_ERUCCH_BANK_SEL_BMSK); \
  HWIO_TX_TDS_ERUCCH_ADDR_OUT(val); \
}
#endif

#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_ERUCCH_ENC_TRIGGER(START_ENC_NOW_ERUCCH) \
{ \
	uint32 val = \
		((uint32)((START_ENC_NOW_ERUCCH)<< HWIO_TX_TDS_ERUCCH_ENC_TRIGGER_START_ENC_NOW_ERUCCH_SHFT) & HWIO_TX_TDS_ERUCCH_ENC_TRIGGER_START_ENC_NOW_ERUCCH_BMSK); \
	HWIO_TX_TDS_ERUCCH_ENC_TRIGGER_OUT(val); \
  TDSL1_MSG1(HIGH, "TX_TDS_ERUCCH_ENC_TRIGGER: val = %lx", val); \
}
#else
#define TDSENCI_WRITE_TX_ERUCCH_ENC_TRIGGER(START_ENC_NOW_ERUCCH) \
{\
	uint32 val = \
		((uint32)((START_ENC_NOW_ERUCCH)<< HWIO_TX_TDS_ERUCCH_ENC_TRIGGER_START_ENC_NOW_ERUCCH_SHFT) & HWIO_TX_TDS_ERUCCH_ENC_TRIGGER_START_ENC_NOW_ERUCCH_BMSK); \
	HWIO_TX_TDS_ERUCCH_ENC_TRIGGER_OUT(val);\
}
#endif

#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_ERUCCH_LAST_BANK0(last_row, last_col) \
{ \
  uint32 val = \
     ((uint32)((last_row)<< HWIO_TX_TDS_ERUCCH_LAST_BANK0_LAST_ROW_SHFT) & HWIO_TX_TDS_ERUCCH_LAST_BANK0_LAST_ROW_BMSK) | \
     ((uint32)((last_col)<< HWIO_TX_TDS_ERUCCH_LAST_BANK0_LAST_COL_SHFT) & HWIO_TX_TDS_ERUCCH_LAST_BANK0_LAST_COL_BMSK); \
  HWIO_TX_TDS_ERUCCH_LAST_BANK0_OUT(val); \
	TDSL1_MSG1(HIGH, "TX_TDS_ERUCCH_LAST_BANK0: val = %lx", val); \
}
#else
#define TDSENCI_WRITE_TX_ERUCCH_LAST_BANK0(last_row, last_col) \
{ \
  uint32 val = \
     ((uint32)((last_row)<< HWIO_TX_TDS_ERUCCH_LAST_BANK0_LAST_ROW_SHFT) & HWIO_TX_TDS_ERUCCH_LAST_BANK0_LAST_ROW_BMSK) | \
     ((uint32)((last_col)<< HWIO_TX_TDS_ERUCCH_LAST_BANK0_LAST_COL_SHFT) & HWIO_TX_TDS_ERUCCH_LAST_BANK0_LAST_COL_BMSK); \
  HWIO_TX_TDS_ERUCCH_LAST_BANK0_OUT(val); \
}
#endif

#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_ERUCCH_LAST_BANK1(last_row, last_col) \
{ \
  uint32 val = \
     ((uint32)((last_row)<< HWIO_TX_TDS_ERUCCH_LAST_BANK1_LAST_ROW_SHFT) & HWIO_TX_TDS_ERUCCH_LAST_BANK1_LAST_ROW_BMSK) | \
     ((uint32)((last_col)<< HWIO_TX_TDS_ERUCCH_LAST_BANK1_LAST_COL_SHFT) & HWIO_TX_TDS_ERUCCH_LAST_BANK1_LAST_COL_BMSK); \
  HWIO_TX_TDS_ERUCCH_LAST_BANK1_OUT(val); \
	TDSL1_MSG1(HIGH, "TX_TDS_ERUCCH_LAST_BANK1: val = %lx", val); \
}
#else
#define TDSENCI_WRITE_TX_ERUCCH_LAST_BANK1(last_row, last_col) \
{ \
  uint32 val = \
     ((uint32)((last_row)<< HWIO_TX_TDS_ERUCCH_LAST_BANK1_LAST_ROW_SHFT) & HWIO_TX_TDS_ERUCCH_LAST_BANK1_LAST_ROW_BMSK) | \
     ((uint32)((last_col)<< HWIO_TX_TDS_ERUCCH_LAST_BANK1_LAST_COL_SHFT) & HWIO_TX_TDS_ERUCCH_LAST_BANK1_LAST_COL_BMSK); \
  HWIO_TX_TDS_ERUCCH_LAST_BANK1_OUT(val); \
}
#endif

#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_ERUCCH_SUBFRAMEc_BANK0(c, start_row, start_col) \
{ \
	uint32 val = \
		((uint32)((start_row)<< HWIO_TX_TDS_ERUCCH_SUBFRAMEc_BANK0_START_ROW_SHFT) & HWIO_TX_TDS_ERUCCH_SUBFRAMEc_BANK0_START_ROW_BMSK) | \
		((uint32)((start_col)<< HWIO_TX_TDS_ERUCCH_SUBFRAMEc_BANK0_START_COL_SHFT) & HWIO_TX_TDS_ERUCCH_SUBFRAMEc_BANK0_START_COL_BMSK); \
  HWIO_TX_TDS_ERUCCH_SUBFRAMEc_BANK0_OUTI(c, val); \
	TDSL1_MSG2(HIGH, "TX_TDS_ERUCCH_SUBFRAMEc_BANK0: c = %d, val = %lx", c,val); \
}
#else
#define TDSENCI_WRITE_TX_ERUCCH_SUBFRAMEc_BANK0(c, start_row, start_col) \
{ \
	uint32 val = \
		((uint32)((start_row)<< HWIO_TX_TDS_ERUCCH_SUBFRAMEc_BANK0_START_ROW_SHFT) & HWIO_TX_TDS_ERUCCH_SUBFRAMEc_BANK0_START_ROW_BMSK) | \
		((uint32)((start_col)<< HWIO_TX_TDS_ERUCCH_SUBFRAMEc_BANK0_START_COL_SHFT) & HWIO_TX_TDS_ERUCCH_SUBFRAMEc_BANK0_START_COL_BMSK); \
  HWIO_TX_TDS_ERUCCH_SUBFRAMEc_BANK0_OUTI(c, val); \
}
#endif

#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_TX_ERUCCH_SUBFRAMEc_BANK1(c, start_row, start_col) \
{ \
	uint32 val = \
		((uint32)((start_row)<< HWIO_TX_TDS_ERUCCH_SUBFRAMEc_BANK1_START_ROW_SHFT) & HWIO_TX_TDS_ERUCCH_SUBFRAMEc_BANK1_START_ROW_BMSK) | \
		((uint32)((start_col)<< HWIO_TX_TDS_ERUCCH_SUBFRAMEc_BANK1_START_COL_SHFT) & HWIO_TX_TDS_ERUCCH_SUBFRAMEc_BANK1_START_COL_BMSK); \
  HWIO_TX_TDS_ERUCCH_SUBFRAMEc_BANK1_OUTI(c, val); \
	TDSL1_MSG2(HIGH, "TX_TDS_ERUCCH_SUBFRAMEc_BANK1: c = %d, val = %lx", c,val); \
}
#else
#define TDSENCI_WRITE_TX_ERUCCH_SUBFRAMEc_BANK1(c, start_row, start_col) \
{ \
	uint32 val = \
		((uint32)((start_row)<< HWIO_TX_TDS_ERUCCH_SUBFRAMEc_BANK1_START_ROW_SHFT) & HWIO_TX_TDS_ERUCCH_SUBFRAMEc_BANK1_START_ROW_BMSK) | \
		((uint32)((start_col)<< HWIO_TX_TDS_ERUCCH_SUBFRAMEc_BANK1_START_COL_SHFT) & HWIO_TX_TDS_ERUCCH_SUBFRAMEc_BANK1_START_COL_BMSK); \
  HWIO_TX_TDS_ERUCCH_SUBFRAMEc_BANK1_OUTI(c, val); \
}
#endif

#endif /* FEATURE_TDSCDMA_HSUPA */

#endif /* TDSENCI_H */
