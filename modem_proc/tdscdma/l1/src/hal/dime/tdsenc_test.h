#ifndef TDSENC_TEST_H
#define TDSENC_TEST_H




/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                         tdsenc_test.h

GENERAL DESCRIPTION

  This file is used to verify HW ENC.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2011 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  $Header: //components/rel/tdscdma.mpss/4.3/l1/src/hal/dime/tdsenc_test.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
5/25/11    XC      Initial draft

===========================================================================*/



/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "tdscdma_variation.h"
#if 0
extern "C" {
#include "rex.h"
#include "task.h"
#include "appmgr.h"
#include "tdsl1std.h"
}
#endif

#include "comdef.h"
#include "customer.h"
#include "tdsl1std.h"
#include "tdsl1ulcfg.h"
#include "msg.h"

#ifdef FEATURE_BOLT_MODEM
#include "tdsenc_common.h"
#endif /*FEATURE_BOLT_MODEM*/
//#if defined(FEATURE_TDSCDMA_PLT)

/*===========================================================================

                                 TYPES

===========================================================================*/
#define TDSENC_TX_ERAMA_SIZE              440
#define TDSENC_TX_ERAMB_SIZE              400
#define TDSENC_TX_RMRAM_SIZE              500
#ifdef FEATURE_BOLT_MODEM
#define TDSENC_TX_CINTLV_RAM_SIZE						500
#endif /*FEATURE_BOLT_MODEM*/
#define TDSENC_DEBUG_BUF_SIZE             100
//#define TDSENC_TEST_MAX_NUM_FRM           16
#define TDSENC_TEST_MAX_NUM_FRM           8
#define TDSENC_NUM_TTI_TEST               3

#define TDSENC_TEST_UL_MAX_TRCH               8
#define TDSENC_TEST_UL_MAX_NUM_TTI            8
#define TDSENC_TEST_UL_MAX_NUM_TB            15
#define TDSENC_TEST_UL_MAC_MAX_HDR_SIZE       6
#define TDSENC_TEST_UL_MAX_TB_SIZE            640
#define TDSENC_TEST_UL_MAX_NUM_RF             8


/* UL test vector case id */
typedef enum
{
  /* Plz do not change the value, it's one to one matched with tplt code.
   * tplt_ul_testvector_case_enum_type
   */
  TDSENC_UL_ENC_UNIT_TEST_PRACH_0 = 0,
  TDSENC_UL_ENC_UNIT_TEST_PRACH_3 = 3,
  TDSENC_UL_ENC_UNIT_TEST_PRACH_6 = 6,
  TDSENC_UL_ENC_UNIT_TEST_PRACH_7 = 7,

  TDSENC_UL_ENC_UNIT_TEST_DPCH_0             = 100,
  TDSENC_UL_ENC_UNIT_TEST_DPCH_2_INTRLV      = 102,
  TDSENC_UL_ENC_UNIT_TEST_DPCH_SLOT_INTRLV_2 = 112,
  TDSENC_UL_ENC_UNIT_TEST_DPCH_M_TRCH_TB_1   = 121,
  TDSENC_UL_ENC_UNIT_TEST_DPCH_M_TRCH_TB_5   = 125,

  TDSENC_UL_ENC_UNIT_TEST_ERUCCH_0 = 200,
  TDSENC_UL_ENC_UNIT_TEST_ERUCCH_6 = 206,
  TDSENC_UL_ENC_UNIT_TEST_ERUCCH_10 = 210,

  TDSENC_UL_ENC_UNIT_TEST_DPCH_ERUCCH_0 = 300, /* Concurrent test for DPCH and E-RUCCH */
  TDSENC_UL_ENC_UNIT_TEST_DPCH_ERUCCH_1 = 301, /* Concurrent test for DPCH and E-RUCCH */
  TDSENC_UL_ENC_UNIT_TEST_DPCH_ERUCCH_12 = 312,/* 5ms ERUCCH, 1 DCH */
  TDSENC_UL_ENC_UNIT_TEST_LAST_CASE,

  TDSENC_UL_PRACH_TEST_0 = 400,
  TDSENC_UL_PRACH_TEST_3 = 403,
  TDSENC_UL_PRACH_TEST_6 = 406,
  TDSENC_UL_PRACH_TEST_7 = 407,

  TDSENC_UL_DPCH_TEST_0             = 500,
  TDSENC_UL_DPCH_TEST_0_FW          = 501,
  TDSENC_UL_DPCH_TEST_2_INTRLV      = 502,
  TDSENC_UL_DPCH_TEST_2_SLOT_INTRLV = 503, /* SW Test vectors\UL RUMI\TEST_2: slot based 2nd interleaving */
  TDSENC_UL_DPCH_TEST_M_TRCH_TB_1   = 504, /* SW Test vectors\UL RUMI\TEST_1: 2 TrCHs */
  TDSENC_UL_DPCH_TEST_M_TRCH_TB_5   = 505, /* SW Test vectors\UL RUMI\TEST_5: 4 TrCHs */
  
  TDSENC_UL_CDP_NO_PRACH_OR_DPCH    = 600,
  TDSENC_UL_CDP_PRACH               = 601, /* ul_sync_fpach_prach.pl */
  TDSENC_UL_CDP_DPCH                = 602, /* ul_dl_dch_config.pl */
  TDSENC_UL_CDP_DPCH_TWO_TRCH       = 603, /* ul_dch_config_two_trch.pl */
  TDSENC_UL_CDP_DPCH_SRB_TTI_10MS   = 604, /* ul_dl_dch_config_SRB_tti10ms.pl */
  TDSENC_UL_CDP_DPCH_AMR_PLUS_SRB   = 605, /* ul_dl_dch_config_AMR+SRB.pl */
  TDSENC_UL_CDP_PS_8K_8K_PLUS_SRB   = 606, /* ul_dl_dch_config_P8k_P8k+SRB.pl */
  TDSENC_UL_CDP_PS_64K_64K_PLUS_SRB = 607, /* ul_dl_dch_config_P64k_P64k+SRB.pl */
  TDSENC_UL_CDP_PS_384K_64K_PLUS_SRB = 608, /* ul_dl_dch_config_P64k_P384k+SRB.pl */

  TDSENC_UL_MAX_TEST_CASE           = 0x7FFF
} tdsenc_ul_testvector_case_enum_type;

#if defined(FEATURE_TDSCDMA_TRITON_UL)
/** enum for memory pointers in tx_mem */
typedef enum  {
  TDSENC_MEM_PTR_TDS_ERAMA_R4                  = 0     ,
  TDSENC_MEM_PTR_TDS_ERAMB_R4                  = 512  ,
  TDSENC_MEM_PTR_TDS_ERAMA_ERUCCH      = 896  ,
  TDSENC_MEM_PTR_TDS_ERAMB_ERUCCH      = 912  ,
  TDSENC_MEM_PTR_TDS_ERAM_EUL_0            = 1024 ,
  TDSENC_MEM_PTR_TDS_ERAM_EUL_1            = 2048 ,
  TDSENC_MEM_PTR_TDS_ERAM_ENC_CFGRAM = 3072 ,

  TDSENC_MEM_PTR_TDS_IRAM_R4_0                 = 0    + 0x2000 ,
  TDSENC_MEM_PTR_TDS_IRAM_R4_1                 = 512  + 0x2000 ,
  TDSENC_MEM_PTR_TDS_IRAM_EUL_0               = 1024 + 0x2000 ,
  TDSENC_MEM_PTR_TDS_IRAM_EUL_1               = 1536 + 0x2000 ,
  TDSENC_MEM_PTR_TDS_IRAM_EUL_BC             = 2048 + 0x2000 ,
  TDSENC_MEM_PTR_TDS_IRAM_ERUCCH_0        = 3072 + 0x2000 ,
  TDSENC_MEM_PTR_TDS_IRAM_ERUCCH_1        = 3200 + 0x2000 ,
  TDSENC_MEM_PTR_TDS_IRAM_MOD_CFGRAM = 3328 + 0x2000 ,

  TDSENC_MEM_PTR_UMTS_ERAMA_R99         = 0      ,
  TDSENC_MEM_PTR_UMTS_ERAMB_R99         = 512  ,
  TDSENC_MEM_PTR_UMTS_ERAM_EUL_0        = 1024 ,
  TDSENC_MEM_PTR_UMTS_ERAM_EUL_1        = 2048 ,
  TDSENC_MEM_PTR_UMTS_ERAM_ENC_CFGRAM   = 3072 ,

  TDSENC_MEM_PTR_UMTS_IRAM_R99_0  = 0    + 0x2000 ,
  TDSENC_MEM_PTR_UMTS_IRAM_R99_1  = 512  + 0x2000 ,
  TDSENC_MEM_PTR_UMTS_IRAM_EUL_0  = 1024 + 0x2000 ,
  TDSENC_MEM_PTR_UMTS_IRAM_EUL_1  = 3072 + 0x2000 ,

  TDSENC_MEM_PTR_C2K_1x_ERAM_DCCH   = 0   ,
  TDSENC_MEM_PTR_C2K_1x_ERAM_FCH    = 512  ,
  TDSENC_MEM_PTR_C2K_1x_ERAM_SCH    = 1024 ,
  
  TDSENC_MEM_PTR_C2K_1x_IRAM_DCCH_0   = 0   + 0x2000,
  TDSENC_MEM_PTR_C2K_1x_IRAM_DCCH_1   = 256    + 0x2000,
  TDSENC_MEM_PTR_C2K_1x_IRAM_FCH_0    = 512    + 0x2000,
  TDSENC_MEM_PTR_C2K_1x_IRAM_FCH_1    = 768  + 0x2000  ,
  TDSENC_MEM_PTR_C2K_1x_IRAM_SCH_0    = 1024 + 0x2000  ,
  TDSENC_MEM_PTR_C2K_1x_IRAM_SCH_1    = 2048 + 0x2000  ,

  TDSENC_MEM_PTR_C2K_REV0_ERAM = 0,
  TDSENC_MEM_PTR_C2K_REV0_IRAM_0         = 0   + 0x2000 ,
  TDSENC_MEM_PTR_C2K_REV0_IRAM_1         = 1024 + 0x2000 ,
  TDSENC_MEM_PTR_C2K_REV0_IRAM_GAIN_BUF  = 4800 + 0x2000 ,
  
  TDSENC_MEM_PTR_C2K_REVAB_ERAM_C0_PKT0   = 0   ,
  TDSENC_MEM_PTR_C2K_REVAB_ERAM_C0_PKT1   = 384 ,
  TDSENC_MEM_PTR_C2K_REVAB_ERAM_C0_PKT2   = 768 ,
  TDSENC_MEM_PTR_C2K_REVAB_ERAM_C0_PKT3   = 1152,
  TDSENC_MEM_PTR_C2K_REVAB_ERAM_C1_PKT0   = 1536,
  TDSENC_MEM_PTR_C2K_REVAB_ERAM_C1_PKT1   = 1920,
  TDSENC_MEM_PTR_C2K_REVAB_ERAM_C1_PKT2   = 2304,
  TDSENC_MEM_PTR_C2K_REVAB_ERAM_C1_PKT3   = 2688,
  TDSENC_MEM_PTR_C2K_REVAB_ERAM_C2_PKT0   = 3072,
  TDSENC_MEM_PTR_C2K_REVAB_ERAM_C2_PKT1   = 3456,
  TDSENC_MEM_PTR_C2K_REVAB_ERAM_C2_PKT2   = 3840,
  TDSENC_MEM_PTR_C2K_REVAB_ERAM_C2_PKT3   = 4224,

  TDSENC_MEM_PTR_C2K_REVAB_IRAM         = 0 + 0x2000    ,
  TDSENC_MEM_PTR_C2K_REVAB_IRAM_C0_0    = 1280 + 0x2000,
  TDSENC_MEM_PTR_C2K_REVAB_IRAM_C0_1    = 1856 + 0x2000,
  TDSENC_MEM_PTR_C2K_REVAB_IRAM_C1_0    = 2432 + 0x2000,
  TDSENC_MEM_PTR_C2K_REVAB_IRAM_C1_1    = 3072 + 0x2000,
  TDSENC_MEM_PTR_C2K_REVAB_IRAM_C2_0    = 3648 + 0x2000,
  TDSENC_MEM_PTR_C2K_REVAB_IRAM_C2_1    = 4224 + 0x2000,
  TDSENC_MEM_PTR_C2K_REVAB_GAIN_BUF     = 4800 + 0x2000
} tdsenc_mem_ptr_enum_type ;
#endif
typedef struct {
  /* pointer to transport block data */
  uint8          tb_data[TDSENC_TEST_UL_MAX_TB_SIZE];
  /* MAC header data */
  uint8          mac_hdr[TDS_L1_UL_MAC_MAX_HDR_SIZE];
  /* size of MAC header in bits (0..39)*/
  uint8          mac_hdr_size;
  /* ciphering flag for this block */
  boolean      ciphering_on;
  #if 1	
  /*#ifdef FEATURE_WCDMA_L1_TM_CIPHER_SUPPORTED*/
  /* TM ciphering state */
  tdsl1_tb_tm_cipher_state_enum_type tm_cipher_state;
  /*#endif*/
    /* radio bearer id */
  uint8          radio_bearer_id;
    /* ciphering key */
  uint8          ciphering_key_id;
    /* combination of hyper frame num and RLC seq num */
  uint32         count;
    /* offset in transport block from where ciphering should start */
  uint8          offset;
 /* Ciphering Algorithm: Kasumi or Snow3G*/
  uint32 cipher_algo;
  #endif
} tdsenc_test_ul_tb_data_type;


typedef struct {
    /* number of transport blocks - also indicates no TrCH if zero */
  uint8              num_tb;
    /* transport block size in bits (0..5120) */
  uint16             tb_size;
    /* transport channel data */
  tdsenc_test_ul_tb_data_type trch_data_per_tti[TDSENC_TEST_UL_MAX_NUM_TB];
} tdsenc_test_ul_trch_data_type;

/* transport channel data */
typedef struct {
  tdsl1_tti_enum_type tti;
  /* number of TTIs */
  uint8              num_tti;
  /* transport channel data for TTIs*/
  tdsenc_test_ul_trch_data_type trch_data_set[TDSENC_TEST_UL_MAX_NUM_TTI];
} tdsenc_test_ul_trch_data_set_type;

typedef struct {
    /* number of transport channels */
  uint8                num_trch;
    /* data of all transport channels in the 10ms frame */
  tdsenc_test_ul_trch_data_set_type tx_data[TDSENC_TEST_UL_MAX_TRCH];
} tdsenc_test_tx_data_type;


typedef struct 
{
  /* Length in 32bit words*/
  uint32 len;
  
  /* ERAMA data got from test vector per TTI */
  uint32 data_per_rf[TDSENC_TX_ERAMA_SIZE];
 
} tdsenc_test_erama_data_struct_type;


typedef struct 
{
  uint8 num_rf;
  /* ERAMA data got from test vector */
  tdsenc_test_erama_data_struct_type data_buf[TDSENC_TEST_UL_MAX_NUM_RF];
} tdsenc_test_erama_buf_struct_type;


typedef struct 
{
  /* Length, number of 32 bit word */
  uint32 len;
  
  /* ERAMB data got from test vector per RF */
  uint32 data_per_rf[TDSENC_TX_ERAMB_SIZE];
} tdsenc_test_eramb_data_struct_type;


typedef struct 
{
  uint8 num_rf;
  
  /* ERAMB data got from test vector */
  tdsenc_test_eramb_data_struct_type data_buf[TDSENC_TEST_UL_MAX_NUM_RF];
} tdsenc_test_eramb_buf_struct_type;


typedef struct 
{
  /* Length */
  uint32 len;
  
  /* RMRAM data got from test vector */
  uint32 data_per_rf[TDSENC_TX_RMRAM_SIZE];
 
} tdsenc_test_rmram_data_struct_type;

typedef struct 
{
  /* Length */
  uint16 num_rf;
  
  /* ERAMB data got from test vector */
  tdsenc_test_rmram_data_struct_type data_buf[TDSENC_TEST_UL_MAX_NUM_RF];
} tdsenc_test_rmram_buf_struct_type;

#ifdef FEATURE_BOLT_MODEM
typedef struct
{
	uint16 data_len;
	uint32 data_per_rf[TDSENC_TX_CINTLV_RAM_SIZE];
}tdsenc_test_cintlv_ram_data_struct_type;

typedef struct
{
	/*Length*/
	uint16 num_rf;

	/*WAVE_IB data got from test vector*/
	tdsenc_test_cintlv_ram_data_struct_type data_buf[TDSENC_TEST_UL_MAX_NUM_RF];
}tdsenc_test_cintlv_ram_struct_type;

#endif/*FEATURE_BOLT_MODEM*/

typedef struct {
  /* The spreading factor of the code channel signalled by higher layer */
  tdsl1_sf_enum_type                      sf;

  /* Channelisation code (1..16) */
  uint8                                   chan_code;
} tdsenc_test_chan_code_struct_type;

typedef struct{
  /* Whether the physical channel is valid */
  boolean                                 valid;

  /* The number of TFCI bits */
  uint8                                   tfci_len;

  /* Amount of SS and TPC bits sent in this timeslot */
  tdsl1_ss_tpc_symb_enum_type             ss_tpc_symbols;

  /* Additional SS and TPC numbers */
  uint8                                   add_ss_tpc_symbols;

  /* number of channelisation code */
  uint8                                   num_chan_codes;

  tdsenc_test_chan_code_struct_type chan_code_array[TDSL1_MAX_NUM_CHAN_CODE_PER_SLOT];
} tdsenc_test_phychan_struct_type;

typedef struct {
  /* Whether the transport channel is valid */
  boolean                               valid;

  uint8                                 trch_id;

  /* Transmission time interval */
  tdsl1_tti_enum_type                   tti;

  /* Channel coding type and rate */
  tdsl1_coding_enum_type                coding_type;

  /* Static Rate Matching attribute, 1..256 */
  uint16                                staticRM;

  /* CRC size, in bit */
  tdsl1_crc_length_enum_type            crc_size;

  /* The number of transport blocks. */
  uint16                                tb_block_num;

  /* Transport block size. */
  uint16                                tb_size;
}tdsenc_test_trch_config_struct_type;


/* Trubo Interleaving parameters */
typedef struct {
  /* C - 1, where C is the number of columns */
  uint8                       num_columns_minus1;

  /* last column in the last row */
  uint8                       last_column;

  /* the number of rows in the interleaver table */
  uint8                       num_row_index;

  /* last row bit exchange condition */
  boolean                     last_row_bit_exch;

  /* index of last row with valid data */
  uint8                       last_row;

  /* index of prime number p used in the index table */
  uint8                       prime_num_index;

  /* prime number p */
  uint16                      prime_number;
} tdsenc_test_trch_turbo_ileav_parms_type;

typedef struct {
  /* The number of transport blocks. */
  uint16                      tb_block_num;

  /* Transport block size. */
  uint16                      tb_size;

  /* CRC size */
  tdsl1_crc_length_enum_type          crc_size;

  /* Number of code segments */
  uint8                       num_code_segs;

  /* Number of bits per code segment */
  uint32                      num_bits_per_code_seg;

  /* Rate matched size per radio frame, also known as Nij */
  uint32                      pre_rm_bits;

  /* TTI value */
  tdsl1_tti_enum_type                       tti;

  /* Coding rate type */
  tdsl1_coding_enum_type                       coding_rate_type;

  /* Number of filler bits to be used in the first code segment. */
  uint32                      num_filler_bits;

  /* Turbo Interleaving parameters. Valid only if coding type is Trubo coding */
  tdsenc_test_trch_turbo_ileav_parms_type   turbo_ileav_parms;  

  /* puncture or repetition */
  boolean                     punct_flag;

  /* Error variable eIni1, defined as array to support 80ms TTI,
   * which one to use dependents on radio frame index in a TTI */
  uint16                      eini1[8];

  /* Error variable eIni2, defined as array to support 80ms TTI,
   * which one to use dependents on radio frame index in a TTI */
  uint16                      eini2[8];

  /* Error variable eMinus1 */
  uint16                      eminus1;

  /* Error variable eMinus2 */
  uint16                      eminus2;

  /* Error variable ePlus1 */
  uint16                      eplus1;

  /* Error variable ePlus2 */
  uint16                      eplus2;
} tdsenc_test_tfc_config_struct_type;


typedef struct {
  /* puncture limit * 100 */
  uint8                                       punct_lim;
  
  tdsl1_ul_filler_polarity_enum_type        filler_val;
  
  /* Whether second interleaving based on frame */
  boolean                                     frame_based_sec_intrlv;
} tdsenc_test_cctrch_config_type;

#ifdef FEATURE_TDSCDMA_HSUPA
typedef struct
{
  tfw_erucch_config_cmd_t             tfw_erucch_config;

  /* Stores E-RUCCH data. The size of E-RUCCH data is always 39 bits */
  uint8                               erucch_data[8];
} tdsenc_test_erucch_info_struct_type;
#endif /* FEATURE_TDSCDMA_HSUPA */

typedef struct {
  uint8  num_trch;
  tdsenc_test_cctrch_config_type          cctrch_config;
  tdsl1_ul_phychan_enum_type			  chan_type;
  
  //tdsenc_test_phychan_struct_type       phychan_info[TDSL1_MAX_NUM_TS];

  tdsenc_test_tfc_config_struct_type  tfc_config[TDSENC_TEST_UL_MAX_TRCH];

  #ifdef FEATURE_TDSCDMA_HSUPA
  tdsenc_test_erucch_info_struct_type    ul_erucch_info;
  #endif
  /* slot data */
  uint16                                  slot_data_num[TDSL1_UL_MAX_NUM_TS];
  uint8									trch_cfg0_idx[TDSENC_TEST_UL_MAX_TRCH];
} tdsenc_test_enc_config_type;


typedef struct{
  /* To validate HW programming at next sub-frame or not */
  boolean       validate_flag;

#ifdef FEATURE_BOLT_MODEM
  boolean      erucch_validate_flag;
#endif /*FEATURE_BOLT_MODEM*/

  /* How many Trchs are programed at this time */
  uint8         num_trch;

  /* How many Trchs are activated so far, including the newed programed Trchs
   * and un-expired Trchs
   */
  uint8         num_active_trch;
} tdsenc_test_control_struct_type;

extern tdsenc_test_enc_config_type* tdsenc_test_enc_config_ptr;
extern boolean tdsenc_test_unit_test_enabled;
tdsenc_test_control_struct_type tdsenc_test_ctrl_info;

#ifdef FEATURE_BOLT_MODEM
extern tdsenc_test_cintlv_ram_struct_type* tdsenc_test_cintlv_ram_buf_ptr;
extern tdsenc_test_cintlv_ram_struct_type* tdsenc_test_erucch_cintlv_ram_buf_ptr;
extern enc_mk_cmd_t tdsenc_test_cmd_dump_buf;
extern enc_tx_r4r99_enc_cfg_t tdsenc_test_r4_enc_cfg_dump_buf;
extern enc_enc3g_mmap_t tdsenc_test_r4_enc_params_dump_buf;
extern enc_tx_tds_dpch_chintlv_cfg_t tdsenc_test_r4_chintlv_cfg_dump_buf;
extern enc_chintlv_mmap_t tdsenc_test_r4_chintlv_params_dump_buf;
#endif/*FEATURE_BOLT_MODEM*/

/*===========================================================================

                                 DATA DEFINITION

===========================================================================*/

/*===========================================================================

                                 FUNCTIONS

===========================================================================*/

#if 0 // stuart

/*=========================================================================

FUNCTION tdsenc_test_get_complete_flag

DESCRIPTION
  This function is to get value of ENC test complete flag 

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=========================================================================*/

boolean tdsenc_test_get_complete_flag( void );

/*=========================================================================

FUNCTION tdsenc_test_set_complete_flag

DESCRIPTION
  This function is to set value of ENC test complete flag 

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=========================================================================*/

void tdsenc_test_set_complete_flag( boolean flag );


/*=========================================================================

FUNCTION tdsenc_test_get_success_flag

DESCRIPTION
  This function is to get value of ENC test success flag 

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=========================================================================*/

boolean tdsenc_test_get_success_flag( void );


/*=========================================================================

FUNCTION tdsenc_test_set_success_flag

DESCRIPTION
  This function is to set value of ENC test complete flag 

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=========================================================================*/

void tdsenc_test_set_success_flag( boolean flag );

#endif  // stuart
/*=========================================================================

FUNCTION tdsenc_test_set_erucch_data

DESCRIPTION
  This function is to set data for ERUCCH 

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=========================================================================*/

void tdsenc_test_set_erucch_data(uint8* data_ptr);

/*=========================================================================

FUNCTION tdsenc_test_erucch_validation

DESCRIPTION
  This function is to set data for ERUCCH 

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=========================================================================*/

void tdsenc_test_erucch_validation(uint16 sfn);

/*=========================================================================

FUNCTION tdsenc_test_update_max_tti_frm_num

DESCRIPTION
  This function is used to update maximum TTI frame number for PRACH 
  or DPCH. This number is used to judge whether the test is end.

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=========================================================================*/
void tdsenc_test_update_max_tti_frm_num (void);

/*=========================================================================

FUNCTION tdsenc_test_ul_build_frame_ind

DESCRIPTION
  This function build TrCH data for ERAMA according to test vectors.

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=========================================================================*/

void tdsenc_test_ul_build_frame_ind
(
  /* TTI required for data */
  tdsl1_tti_enum_type     tti,

  /* UL is ready to get data from MAC or not */
  boolean                 ul_tx_disabled,

  /* RF number to send */
  uint8                   rfn_to_send,

  /* pointer to Tx data buffer */
  tdsl1_ul_tx_data_type  *tx_buf_ptr
);

/*=========================================================================

FUNCTION tdsenc_test_proc

DESCRIPTION
  This function check and see if need to validate ENC programming.

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=========================================================================*/

void tdsenc_test_proc( void );

/*=========================================================================

FUNCTION tdsenc_test_start

DESCRIPTION
  This function is invoked at the begging of test to read and save test data. 
  Initialize ENC and enable SFN_CFN_UPDATE and GET_DATA event. 

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=========================================================================*/

void tdsenc_test_start(tdsenc_ul_testvector_case_enum_type case_num, 
                       uint16 repeat_cnt);

/*===========================================================================

FUNCTION   tdsenc_test_dealloc_test_data_buf

DESCRIPTION  This function shall de-allocate all test data buffers to be used 
             for UL PLT init/integration test.

DEPENDENCIES
  Buffers are allocated in tplt_ul_alloc_test_data_buf()
RETURN VALUE
  void

SIDE EFFECTS
  None.

===========================================================================*/
void  tdsenc_test_dealloc_test_data_buf (void);

/*=========================================================================

FUNCTION tdsenc_test_end

DESCRIPTION
  This function is invoked at the end of the test to release test data 

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=========================================================================*/
void tdsenc_test_end (void);

/*===========================================================================

FUNCTION  tdsenc_test_check_for_fpach_response

DESCRIPTION
  This function check if should response at the UPPCH_FPACH_CONFIG.
  ack_at_which_probe: indicate ACK responses to which probe
  to_use_which_prach: indicate which set of PRACH is used to send RACH data
                      after receive ACK.
   
DEPENDENCIES
  None.

RETURN VALUE
  TURE if all pending config are done, FALSE otherwise.

SIDE EFFECTS
  None
===========================================================================*/
boolean tdsenc_test_check_for_fpach_response(uint8 ack_at_which_probe, 
                                                  uint8 to_use_which_prach);

/*===========================================================================

FUNCTION  tdsenc_test_save_test_info

DESCRIPTION
   
DEPENDENCIES
  None.

RETURN VALUE
  TURE if all pending config are done, FALSE otherwise.

SIDE EFFECTS
  None
===========================================================================*/
void tdsenc_test_save_test_info( boolean validate_flg, uint8 num_trchs, uint8 num_active_trchs );

#ifdef FEATURE_BOLT_MODEM
/*===========================================================================

FUNCTION  tdsenc_test_dump_cmd_data

DESCRIPTION
  This function is used to dump the cmd memory from enc_lmem
   
DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None
===========================================================================*/
void tdsenc_test_dump_cmd_data(void);

/*===========================================================================

FUNCTION  tdsenc_test_dump_r4_enc_cfg_data

DESCRIPTION
  This function is used to dump the r4 enc cfg memory from enc_lmem
   
DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None
===========================================================================*/
void tdsenc_test_dump_r4_enc_cfg_data(void);

/*===========================================================================

FUNCTION  tdsenc_test_dump_r4_enc_param_data

DESCRIPTION
  This function is used to dump the r4 enc cfg memory from enc_lmem
   
DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None
===========================================================================*/
void tdsenc_test_dump_r4_enc_param_data(void);

/*===========================================================================

FUNCTION  tdsenc_test_dump_r4_cintlv_cfg_data

DESCRIPTION
  This function is used to dump the r4 cintlv cfg memory from enc_lmem
   
DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None
===========================================================================*/
void tdsenc_test_dump_r4_cintlv_cfg_data(void);

/*===========================================================================

FUNCTION  tdsenc_test_dump_r4_cintlv_param_data

DESCRIPTION
  This function is used to dump the r4 cintlv params memory from enc_lmem
   
DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None
===========================================================================*/
void tdsenc_test_dump_r4_cintlv_param_data(void);

/*===========================================================================

FUNCTION  tdsenc_test_dump_erama_data

DESCRIPTION
  This function is used to dump the erama data from ENC_IB of VEP0-LMEM
   
DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None
===========================================================================*/
void tdsenc_test_dump_erama_data(void);

/*===========================================================================

FUNCTION  tdsenc_test_dump_eib_data

DESCRIPTION
  This function is used to dump the EIB data from ENC-LMEM
   
DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None
===========================================================================*/
void tdsenc_test_dump_eib_data(void);

/*===========================================================================

FUNCTION  tdsenc_test_dump_eramb_data

DESCRIPTION
  This function is used to dump the eramb data from EWB of ENC_LMEM
   
DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None
===========================================================================*/
void tdsenc_test_dump_eramb_data(void);
/*===========================================================================

FUNCTION  tdsenc_test_dump_rmram_data

DESCRIPTION
  This function is used to dump the rmram data from EOB of ENC_LMEM indicated by uK
   
DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None
===========================================================================*/
void tdsenc_test_dump_rmram_data(uint32 cmd_status);
/*===========================================================================

FUNCTION  tdsenc_test_dump_cintlv_ram_data

DESCRIPTION
  This function is used to dump the cintlv ram data from WAVE_IB of VPE0-LMEM
   
DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None
===========================================================================*/
void tdsenc_test_dump_cintlv_ram_data(uint16 sfn);
/*===========================================================================

FUNCTION  tdsenc_test_dump_erucch_data

DESCRIPTION
  This function is used to  dump the erucch data from ENC_IB of VEP0-LMEM
   
DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None
===========================================================================*/
void tdsenc_test_dump_erucch_data(void);
/*===========================================================================

FUNCTION  tdsenc_test_dump_erucch_rmram_data

DESCRIPTION
  This function is used to  dump the erucch rmram data from EOB of ENC_LMEM indicated by uK
   
DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None
===========================================================================*/
void tdsenc_test_dump_erucch_rmram_data(uint32 cmd_status);
/*===========================================================================

FUNCTION  tdsenc_test_dump_erucch_cintlv_ram_data

DESCRIPTION
  This function is used to  dump the erucch cintlv_ram data from WAVE_IB of VPE0_LMEM
   
DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None
===========================================================================*/
void tdsenc_test_dump_erucch_cintlv_ram_data(uint16 sfn);
#endif /*FEATURE_BOLT_MODEM*/

//#endif /* FEATURE_TDSCDMA_PLT */

#endif /* TDSENC_TEST_H */
