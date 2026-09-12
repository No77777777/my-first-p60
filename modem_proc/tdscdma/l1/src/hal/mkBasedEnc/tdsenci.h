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

$Header: //components/rel/tdscdma.mpss/4.3/l1/src/hal/mkBasedEnc/tdsenci.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when        who     what, where, why
--------    ---     --------------------------------------------------------
08/15/13    alexyh  include encoder header file from common FW.
03/20/11    cdf     add E-RUCCH register definitions
02/16/11    cxc     file created.
                    
===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "tdscdma_variation.h"
#include "comdef.h"
#include "msm.h"

#ifdef FEATURE_THOR_MODEM
#include "enc_3g_cfg_api.h"
#include "enc_3g_rif.h"
#include "enc_api.h"
#include "enc_chintlv_rif.h"
#else /*FEATURE_BOLT_MODEM*/
#include "tdsenc_3g_cfg_api.h"
#include "tdsenc_3g_rif.h"
#include "tdsenc_api.h"
#include "tdsenc_chintlv_rif.h"
#endif /*FEATURE_THOR_MODEM*/

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

#define TDSENCI_IU_IRQ_PEND_WDATA_CMD0_BMSK  (0x1 << 12)
#define TDSENCI_IU_IRA_PEND_WDATA_CMD1_BMSK  (0x1 << 12)

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

#define TDSENCI_CMD_STATUS_LD_DONE_MASK						 0x1
#define TDSENCI_CMD_STATUS_ENC_DONE_MASK					 0x10
#define TDSENCI_CMD_STATUS_CINTLV_DONE_MASK        0x100
#define TDSENCI_CMD_STATUS_EOB_STATUS_MASK				 0x1000
#define TDSENCI_CMD_STATUS_EOB_STATUS_SHIFT				 3

#define TDSENC_VPE_LMEM_VBUFF_LEN    8000
#define TDSENC_VPE_LMEM_VBUFF_INIT_BANK    0						
#define TDSENC_VPE_LMEM_VBUFF_MIN_BANK    0
#define TDSENC_VPE_LMEM_VBUFF_MAX_BANK    7
/*===========================================================================

                     ENC-LMEM ADDR MICRO

===========================================================================*/

/*1700 words*/
#define TDSENC_ENC_LMEM_CMD_BASIC_OFFSET    6800

/*0 words*/
#define TDSENC_ENC_LMEM_CMD0_OFFSET    (TDSENC_ENC_LMEM_CMD_BASIC_OFFSET+0)
#define TDSENC_ENC_LMEM_CMD0_ADDR    (HWIO_ENC_LMEM_PAGE0_START_ADDR+TDSENC_ENC_LMEM_CMD0_OFFSET)

/*9 words*/
#define TDSENC_ENC_LMEM_CMD1_OFFSET    (TDSENC_ENC_LMEM_CMD_BASIC_OFFSET+36)
#define TDSENC_ENC_LMEM_CMD1_ADDR    (HWIO_ENC_LMEM_PAGE0_START_ADDR+TDSENC_ENC_LMEM_CMD1_OFFSET)

/*0 words*/
#define TDSENC_VPE_LMEM_ENC_IB_BASIC_OFFSET    0
/*0 words*/
#define TDSENC_VPE_LMEM_ENC_IB_R4_OFFSET    (TDSENC_VPE_LMEM_ENC_IB_BASIC_OFFSET+0)
/*440 words*/
#define TDSENC_VPE_LMEM_ENC_IB_ERUCCH_OFFSET    (TDSENC_VPE_LMEM_ENC_IB_BASIC_OFFSET+1760)

#define TDSENC_VPE_LMEM_ENC_IB_R4_ADDR	    (tdsenc_vpe0_lmem_line_addr+TDSENC_VPE_LMEM_ENC_IB_R4_OFFSET)
#define TDSENC_VPE_LMEM_ENC_IB_ERUCCH_ADDR    (tdsenc_vpe0_lmem_line_addr+TDSENC_VPE_LMEM_ENC_IB_ERUCCH_OFFSET)

/*1142 words*/
#define TDSENC_VPE_LMEM_WAVE_IB_BASIC_OFFSET    4568
/*124 words*/
#define TDSENC_VPE_LMEM_WAVE_IB_R4_OFFSET    (TDSENC_VPE_LMEM_WAVE_IB_BASIC_OFFSET+496)
/*2004 words*/
#define TDSENC_VPE_LMEM_WAVE_IB_ERUCCH_OFFSET    (TDSENC_VPE_LMEM_WAVE_IB_BASIC_OFFSET+8016)

/*0 words*/
#define TDSENC_VPE_LMEM_WAVE_IB_R4_BANK_0_OFFSET 	 (TDSENC_VPE_LMEM_WAVE_IB_R4_OFFSET+0)
/*1064 words*/
#define TDSENC_VPE_LMEM_WAVE_IB_R4_BANK_1_OFFSET		 (TDSENC_VPE_LMEM_WAVE_IB_R4_OFFSET+4256)
/*0 words*/
#define TDSENC_VPE_LMEM_WAVE_IB_ERUCCH_BANK_0_OFFSET   (TDSENC_VPE_LMEM_WAVE_IB_ERUCCH_OFFSET+0)
/*6 words*/
#define TDSENC_VPE_LMEM_WAVE_IB_ERUCCH_BANK_1_OFFSET   (TDSENC_VPE_LMEM_WAVE_IB_ERUCCH_OFFSET+6)

#define TDSENC_VPE_LMEM_WAVE_IB_R4_BANK_0_ADDR     (tdsenc_vpe0_lmem_line_addr+TDSENC_VPE_LMEM_WAVE_IB_R4_BANK_0_OFFSET)
#define TDSENC_VPE_LMEM_WAVE_IB_R4_BANK_1_ADDR     (tdsenc_vpe0_lmem_line_addr+TDSENC_VPE_LMEM_WAVE_IB_R4_BANK_1_OFFSET)
#define TDSENC_VPE_LMEM_WAVE_IB_ERUCCH_BANK_0_ADDR   (tdsenc_vpe0_lmem_line_addr+TDSENC_VPE_LMEM_WAVE_IB_ERUCCH_BANK_0_OFFSET)
#define TDSENC_VPE_LMEM_WAVE_IB_ERUCCH_BANK_1_ADDR   (tdsenc_vpe0_lmem_line_addr+TDSENC_VPE_LMEM_WAVE_IB_ERUCCH_BANK_1_OFFSET)

/*(3406+30) words*/
#define TDSENC_ENC_LMEM_PARAMS_BASIC_OFFSET    13744

/*0 words*/
#define TDSENC_ENC_LMEM_R4_ENC_PARAMS_OFFSET    (TDSENC_ENC_LMEM_PARAMS_BASIC_OFFSET+0)
#define TDSENC_ENC_LMEM_R4_ENC_PARAMS_ADDR    (HWIO_ENC_LMEM_PAGE0_START_ADDR+TDSENC_ENC_LMEM_R4_ENC_PARAMS_OFFSET)

/*55 words*/
#define TDSENC_ENC_LMEM_R4_ENC_CFG_OFFSET    (TDSENC_ENC_LMEM_PARAMS_BASIC_OFFSET+220)
#define TDSENC_ENC_LMEM_R4_ENC_CFG_ADDR    (HWIO_ENC_LMEM_PAGE0_START_ADDR+TDSENC_ENC_LMEM_R4_ENC_CFG_OFFSET)

/*127 words*/
#define TDSENC_ENC_LMEM_R4_CINTLV_PARAMS_OFFSET    (TDSENC_ENC_LMEM_PARAMS_BASIC_OFFSET+508)
#define TDSENC_ENC_LMEM_R4_CINTLV_PARAMS_ADDR	    (HWIO_ENC_LMEM_PAGE0_START_ADDR+TDSENC_ENC_LMEM_R4_CINTLV_PARAMS_OFFSET)

/*239 words*/
#define TDSENC_ENC_LMEM_R4_CINTLV_CFG_OFFSET    (TDSENC_ENC_LMEM_PARAMS_BASIC_OFFSET+956)
#define TDSENC_ENC_LMEM_R4_CINTLV_CFG_ADDR    (HWIO_ENC_LMEM_PAGE0_START_ADDR+TDSENC_ENC_LMEM_R4_ENC_CFG_OFFSET)

/*156 words*/
#define TDSENC_ENC_LMEM_ERUCCH_ENC_PARAMS_OFFSET    (TDSENC_ENC_LMEM_PARAMS_BASIC_OFFSET+624)
#define TDSENC_ENC_LMEM_ERUCCH_ENC_PARAMS_ADDR    (HWIO_ENC_LMEM_PAGE0_START_ADDR+TDSENC_ENC_LMEM_ERUCCH_ENC_PARAMS_OFFSET)

/*162 words*/
#define TDSENC_ENC_LMEM_ERUCCH_CINTLV_PARAMS_OFFSET    (TDSENC_ENC_LMEM_PARAMS_BASIC_OFFSET+648)
#define TDSENC_ENC_LMEM_ERUCCH_CINTLV_PARAMS_ADDR    (HWIO_ENC_LMEM_PAGE0_START_ADDR+TDSENC_ENC_LMEM_ERUCCH_CINTLV_PARAMS_OFFSET)

/*275 words*/
#define TDSENC_ENC_LMEM_ERUCCH_CINTLV_CFG_OFFSET    (TDSENC_ENC_LMEM_PARAMS_BASIC_OFFSET+1100)
#define TDSENC_ENC_LMEM_ERUCCH_CINTLV_CFG_ADDR    (HWIO_ENC_LMEM_PAGE0_START_ADDR+TDSENC_ENC_LMEM_ERUCCH_CINTLV_CFG_OFFSET)

/*30 words start from ENC_LMEM*/
#define TDSENC_ENC_LMEM_BUFFER_BASIC_OFFSET    120

/*1595 words offset from buffer*/
#define TDSENC_ENC_LMEM_EOB_0_OFFSET   (TDSENC_ENC_LMEM_BUFFER_BASIC_OFFSET+6380)
#define TDSENC_ENC_LMEM_EOB_0_ADDR    (HWIO_ENC_LMEM_PAGE0_START_ADDR+TDSENC_ENC_LMEM_EOB_0_OFFSET)

/*2065 words offset from  buffer*/
#define TDSENC_ENC_LMEM_EOB_1_OFFSET	   (TDSENC_ENC_LMEM_BUFFER_BASIC_OFFSET+8260)
#define TDSENC_ENC_LMEM_EOB_1_ADDR	    (HWIO_ENC_LMEM_PAGE0_START_ADDR+TDSENC_ENC_LMEM_EOB_1_OFFSET)

/*768 words offset from buffer*/
#define TDSENC_ENC_LMEM_EWB_R4_OFFSET    (TDSENC_ENC_LMEM_BUFFER_BASIC_OFFSET+3072)
#define TDSENC_ENC_LMEM_EWB_R4_ADDR    (HWIO_ENC_LMEM_PAGE0_START_ADDR+TDSENC_ENC_LMEM_EWB_R4_OFFSET)

/*===========================================================================

                     ENC-LMEM REGISTERS Programming MICRO

===========================================================================*/
#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_ENC_SS_EN(enable)\
{\
	uint32 val = (enable<<HWIO_ENC_SS_EN_ENC_SS_EN_SHFT)&HWIO_ENC_SS_EN_ENC_SS_EN_BMSK; \
	HWIO_ENC_SS_EN_OUT(val);\
	TDSL1_MSG1(HIGH, "ENC_SS_EN: val = %x", val);\
}
#else
#define TDSENCI_WRITE_ENC_SS_EN(enable)\
{\
	HWIO_ENC_SS_EN_OUT(enable);\
}
#endif /*FEATURE_TDSCDMA_PLT||TEST_FRAMEWORK*/

#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSENCI_WRITE_IU_CLK_CTL(enable)\
{\
	uint32 val = (enable<<HWIO_IU_CLK_CTL_IU_CLK_EN_SHFT)&HWIO_IU_CLK_CTL_IU_CLK_EN_BMSK;\
	HWIO_IU_CLK_CTL_OUT(val);\
	TDSL1_MSG1(HIGH, "ENC_SS_EN: val = %x", val);\
}
#else
#define TDSENCI_WRITE_IU_CLK_CTL(enable)\
{\
	uint32 val = (enable<<HWIO_IU_CLK_CTL_IU_CLK_EN_SHFT)&HWIO_IU_CLK_CTL_IU_CLK_EN_BMSK;\
	HWIO_IU_CLK_CTL_OUT(val);\
}
#endif /*FEATURE_TDSCDMA_PLT||TEST_FRAMEWORK*/

#endif
