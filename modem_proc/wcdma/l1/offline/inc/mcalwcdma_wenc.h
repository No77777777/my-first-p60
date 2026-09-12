#ifndef __MCALWCDMA_WENC_H__
#define __MCALWCDMA_WENC_H__
/*============================================================================
                        M C A L W C D M A _ W E N C. H
DESCRIPTION
  This module has definition and declaration related to MCAL (modem core
  abstraction layer) W Encoder APIs.
 
INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

============================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/inc/mcalwcdma_wenc.h#3 $

when      who     what, where, why
--------  ---     --------------------------------------------------------------
08/08/15   raj    wrapper for programming the EUL TTI mode register
06/03/15   cjz    SW changes to bringup DC-HSUPA on Tabasco RUMI
06/03/15   svh     TA featurisation.
01/16/15   vr     Global reorg for W + W.
06/11/14   tsk    Fixes for Jolokia first CRM 1.0.
06/08/14   ar     Added FTM support for EUL
06/03/14   tsk    Compilation fixes for JOLOKIA 1.0
05/22/14   tsk    Featurization for JOLOKIA 1.0
03/31/14   ar     Modified ERAM/RMRAM readback code and added functionality to output to diag
02/14/14   gp     Enhanced HW readback logging routines
02/14/14   ar     Porting Bolt specific uplink changes on to Bolt 2.0
01/23/14   ash    SW workaround for Resetting TXC0 and TXR0
07/10/13   ar     Initial version : Modem core abstraction layer changes 

==============================================================================*/
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "wcdma_variation.h"
#include "wenc.h"
#include "ulstates.h"
#include "l1rrcif.h"
#include "ul.h"
#include "l1macdata.h"
#include "wenci.h"
#include "l1utils.h"
#include "eulenc.h"

#include "comdef.h"
#include "msmhwio.h"
#include "msmhwioreg.h"

#ifdef FEATURE_WCDMA_L1_TM_CIPHER_SUPPORTED
#include "msm.h"
#endif

#if defined(FEATURE_WCDMA_BOLT_2_0) && !defined(FEATURE_WCDMA_JOLOKIA_MODEM)
#include "wenc_ukernel_cfg_ram.h"
#include "wenc_ukernel_chintlv_api.h"
#include "wenc_ukernel_enc_parm.h"  
#include "wenc_ukernel_lmem_mmap.h"
#include "wenc_ukernel_wav_api.h"
#endif /* defined(FEATURE_WCDMA_BOLT_2_0) && !defined (FEATURE_WCDMA_JOLOKIA_MODEM) */

#ifdef FEATURE_WCDMA_BOLT_2_0
#ifdef DEBUG_WCDMA_ERAM_RMRAM_DUMP
typedef enum
{
  UMTS_10MS_TYPE,
  UMTS_10MS_EUL_TYPE,
  UMTS_SC_2MS_TYPE,
  UMTS_DC_2MS_TYPE,
  MAX_UMTS_DATA_BUFF_TYPE
} mcalwcdma_wenc_umts_data_buf_enum_type;
#endif
#endif

#if defined(FEATURE_WCDMA_BOLT_2_0) && !defined(FEATURE_WCDMA_JOLOKIA_MODEM)

/*===========================================================================*/
/*                                                                           */
/*                DEFINITIONS AND DECLARATIONS FOR MODULE                    */
/*                                                                           */
/* This section contains definitions for constants, macros, types, variables */
/* and other items needed by this module.                                    */
/*===========================================================================*/
/** These are manually created uKernel API structures because 
 ** the auto-generated micro kernel API uses a
 ** union for TDS and UMTS which is causing a memory mapping
 ** mismatch when trying to memcpy the software shadow to the
 ** Encoder LMEM, it will result in overwriting of other LMEM
 ** regions inadvertently
 **/
typedef union {
    struct {
#define ENC_ENC3G_MMAP_EUL_WORD_COUNT  17
        mcalwcdma_wenc_enc_tx_enc_common_cfg_struct_type common_cfg;
        mcalwcdma_wenc_enc_tx_enc_eul_common_struct_type common;
        mcalwcdma_wenc_enc_tx_umts_enc_eul_struct_type   umts;
    };
    uint32 Words[17];
} mcalwcdma_wenc_enc_enc3g_mmap_eul_struct_type;

/* Channel interleaver LMEM memory map structure. */
typedef union {
    struct {
#define ENC_CHINTLV_MMAP_UMTS_WORD_COUNT	14
        mcalwcdma_wenc_enc_tx_chintlv_common_cfg_struct_type common_cfg;
        mcalwcdma_wenc_enc_tx_chintlv_umts_struct_type       umts;
    };
    uint32 Words[14];
}mcalwcdma_wenc_enc_chintlv_mmap_umts_struct_type;

/* R99 and EUL encoder LMEM memory map structur */
typedef union {
    struct {
#define ENC_ENC3G_MMAP_R99_WORD_COUNT  40
        mcalwcdma_wenc_enc_tx_enc_common_cfg_struct_type   common_cfg;
        mcalwcdma_wenc_enc_tx_enc_r4r99_common_struct_type common;
        mcalwcdma_wenc_enc_tx_umts_enc_r99_struct_type r99;
    };
    uint32 Words[40];
} mcalwcdma_wenc_enc_enc3g_mmap_r99_struct_type;

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/

typedef enum{
  ENC_LMEM_R99_CFG0_PARAMS,
  ENC_LMEM_R99_CFG1_PARAMS,
  ENC_LMEM_CIPH_PARAMS,
  ENC_LMEM_NON_CFG_RAM_PARAMS,
  ENC_LMEM_PRACH_SECOND_FRM_PARAMS,
  ENC_LMEM_EUL_ENC_PARAMS,
  ENC_LMEM_R99_CHINTLV_PARAMS,
  ENC_LMEM_EUL_CHINTLV_PARAMS,
  MAX_ENC_LMEM_PARAM_TYPES
} mcalwcdma_wenc_enc_lmem_param_enum_type;

typedef enum
{
  ENC_LMEM_R99_CMD_ENUM_TYPE,
  ENC_LMEM_EUL_C0_CMD_ENUM_TYPE,
  ENC_LMEM_EUL_C1_CMD_ENUM_TYPE,
  ENC_LMEM_MAX_CMD_TYPES
} mcalwcdma_wenc_enc_lmem_cmd_enum_type;

typedef struct {
  uint8 trch_idx;
  ul_rm_parms_struct_type ul_rm_parms;
  ul_trch_coding_struct_type ul_trch_coding_parms;
  ul_trch_turbo_ileav_parms_type ul_trch_turbo_ileav_parms;
  uint16 eramb_start_addr;
  uint8 tti_enc_val;
  l1_ul_coding_enum_type cctrch_coding_type;
} mcalwcdma_wenc_write_trch_cfg1_ram_struct_type;

typedef struct {
  WfwTxEulDBParamStruct async_params;
  uint32 cfn;
  uint32 subfn;
  uint32 pduDoneCnt;
  uint32 writeCompleteCnt;
} mcalwcdma_wenc_async_param_readback_struct_type;

typedef struct
{
  #define MCALWCDMA_WENC_ENC_READBACK_DATA_SIZE 0x400
  uint32 data_dump[5][MCALWCDMA_WENC_ENC_READBACK_DATA_SIZE];
  uint32 cfn;
  #ifdef DEBUG_WCDMA_ERAM_RMRAM_DUMP
  mcalwcdma_wenc_umts_data_buf_enum_type buf_type;
  #endif
  mcalwcdma_wenc_enc_mk_cmd_struct_type            cmd;
  mcalwcdma_wenc_enc_tx_r4r99_enc_cfg_struct_type  cfg_ram;
  mcalwcdma_wenc_enc_enc3g_mmap_r99_struct_type    non_cfg_ram;
  mcalwcdma_wenc_enc_chintlv_mmap_umts_struct_type chintlv;
  mcalwcdma_wenc_enc_enc3g_mmap_eul_struct_type    eul_enc_param;
} mcalwcdma_wenc_hw_readback_struct_type;

typedef struct
{
  uint32 wenc_r99_dch_erama_write_addr;
  #ifdef DEBUG_ERAMA_RMRAM_DATA_DUMP
  uint32 mcalwcdma_wenc_data_dump[NUM_TX_MEM_PAGES][SIZE_OF_TX_MEM_PAGE];
  #endif
} mcalwcdma_wenc_ext_cntrl_params_type;

extern mcalwcdma_wenc_ext_cntrl_params_type mcalwcdma_wenc_ext_cntrl_params;

#define MCALWCDMA_WENC_ENC_READBACK_BUF_SIZE 20

typedef struct
{
  uint32 dbg_erama_word_cnt;

  mcalwcdma_wenc_enc_mmap_struct_type                    *wenc_lmem_mmap_ptr;
  mcalwcdma_wenc_enc_atech_cmds_t_struct_type             wenc_encoding_cmd_shadow_struct;
  mcalwcdma_wenc_enc_atech_cmds_t_struct_type            *wenc_encoding_cmd_enc_lmem_ptr;
  mcalwcdma_wenc_enc_tx_r4r99_enc_cipher_ctl_struct_type  wenc_r99_ciph_ctl_shadow_struct;
  mcalwcdma_wenc_enc_tx_r4r99_enc_crc_ctl_struct_type    *wenc_r99_crc_ctl_vpe_lmem_ptr;
  mcalwcdma_wenc_enc_tx_r4r99_enc_cfg_struct_type        *wenc_r99_cfg_ram_param_enc_lmem_ptr;
  mcalwcdma_wenc_enc_chintlv_mmap_umts_struct_type       *wenc_r99_chintlv_param_enc_lmem_ptr;
  mcalwcdma_wenc_enc_wcdma_ib_struct_type                *wenc_r99_eul_eram_vpe_lmem_ptr;
  mcalwcdma_wenc_enc_enc3g_mmap_r99_struct_type           wenc_r99_non_cfg_ram_param_shadow_struct;
  mcalwcdma_wenc_enc_tx_r4r99_enc_crc_ctl_struct_type     wenc_r99_crc_ctl_shadow_struct;
  mcalwcdma_wenc_enc_tx_r4r99_enc_cipher_ctl_struct_type *wenc_r99_ciph_ctl_vpe_lmem_ptr;
  mcalwcdma_wenc_enc_tx_r4r99_enc_cfg_struct_type         wenc_r99_cfg_ram_param_shadow_struct;
  mcalwcdma_wenc_enc_enc3g_mmap_r99_struct_type          *wenc_r99_non_cfg_ram_param_enc_lmem_ptr;
  mcalwcdma_wenc_enc_chintlv_mmap_umts_struct_type        wenc_r99_chintlv_param_shadow_struct;
  mcalwcdma_wenc_enc_chintlv_mmap_umts_struct_type       *wenc_eul_chintlv_param_enc_lmem_ptr[WL1_MAX_UL_CARR_IDX];
  mcalwcdma_wenc_enc_enc3g_mmap_eul_struct_type          *wenc_eul_encoding_param_enc_lmem_ptr[WL1_MAX_UL_CARR_IDX];
  mcalwcdma_wenc_enc_enc3g_mmap_eul_struct_type           wenc_eul_encoding_param_shadow_struct[WL1_MAX_UL_CARR_IDX];
  mcalwcdma_wenc_enc_chintlv_mmap_umts_struct_type        wenc_eul_chintlv_param_shadow_struct[WL1_MAX_UL_CARR_IDX];

  #ifdef DEBUG_WCDMA_ERAM_RMRAM_DUMP
  mcalwcdma_wenc_async_param_readback_struct_type mcalwcdma_wenc_readback_async_params[MCALWCDMA_WENC_ENC_READBACK_BUF_SIZE];
  mcalwcdma_wenc_hw_readback_struct_type mcalwcdma_wenc_hw_readback_data[MCALWCDMA_WENC_ENC_READBACK_BUF_SIZE];
  uint8  mcalwcdma_wenc_enc_trigger_bmsk;
  uint32 mcalwcdma_wenc_hw_reg_readback[80];
  uint32 mcalwcdma_wenc_data_dump_retx[4][NUM_TX_MEM_PAGES][SIZE_OF_TX_MEM_PAGE];
  uint32 mcalwcdma_wenc_hw_data_dump_buf_idx;
  uint8 mcalwcdma_wenc_readback_async_params_buf_idx;
  #endif
} mcalwcdma_wenc_int_cntrl_params_type;

/*===========================================================================
FUNCTION        mcalwcdma_wenc_dpch_state_setup

DESCRIPTION     This function writes the Scrambling code, channel type 
                and endianness during DPCH setup. 

DEPENDENCIES    Inputs : Scrambling code, scrambling code type, phy chan type

RETURN VALUE    None

SIDE EFFECTS    1) Will update scrambling code in HW 
                2) Will set the endianness for R99 Data and the Fall time
                3) Will update the physical channel type based on input
===========================================================================*/

void mcalwcdma_wenc_dpch_state_setup(
   uint32 scrambling_code,
   l1_ul_dpch_sc_enum_type  scrambling_code_type,
   uint8 chan
   , wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        mcalwcdma_wenc_process_phychan_cfg_hsdpcch

DESCRIPTION     This function writes HS-DPCCH OVSF code to HW

DEPENDENCIES    Inputs : Number of DPDCH active 

RETURN VALUE    None

SIDE EFFECTS    Will set the HS-DPCCH OVSF based on if DPDCH is active or not
===========================================================================*/
void mcalwcdma_wenc_process_phychan_cfg_hsdpcch(uint8 num_dpdch);

/*===========================================================================
FUNCTION        mcalwcdma_wenc_dpch_tx_init

DESCRIPTION     This function is used to initialize DPCH parameters. 

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    1) Will initialize the ERAMA write pointer so writes to ERAMA 
                   after this point will be from the beginning of ERAMA
                2) Will write Phychan type to HW as DPCH
                3) Will clear active Transport Channels information in HW
                
===========================================================================*/
void mcalwcdma_wenc_dpch_tx_init( void );

/*===========================================================================
FUNCTION        mcalwcdma_wenc_erama_write_init

DESCRIPTION     In this function WL1 will initialize the parameters for writing to 
                ERAMA every TTI.
                1. Choose the Memory mapping (Using HW Default values, by prog 0)
                2. Program the Phychan type
                3. Program number of Active trchs and ERAMA trchs
                4. Program filler polarity
                5. Program endianness of data when written to ERAMA (Little End)

DEPENDENCIES    Inputs : Physical channel type
                         Number of active transport channels 
                         Num of transport channels to be written to ERAMA this frame
                         Filler polarity based on filler val(0 or 2)

RETURN VALUE    FALSE if encoder is busy, TRUE otherwise

SIDE EFFECTS    1) Reset ERAMA write address 
                2) Write the physical channel type
                3) Set the number of active and erama transport channels
                4) Set the filler polarity
===========================================================================*/
void mcalwcdma_wenc_erama_write_init(
   l1_ul_phychan_enum_type chan,
   uint8 num_active_trchs,
   uint8 num_erama_trchs,
   l1_ul_filler_polarity_enum_type filler_val, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        mcalwcdma_wenc_phychan_start_encoding

DESCRIPTION     This function is used to trigger R99 encoding. 
                It programs which frame in the TTI is being programmed
                and the last row and column of data in RMRAM, before trigerring
                encoding.

DEPENDENCIES    Inputs : Current uplink state, TTI for PRACH, next radio frame mod 8 
                         longest active TTI incase of DPCH 
  

RETURN VALUE    None

SIDE EFFECTS    1) Reset ERAMA Write pointer 
                2) Write the frame number within the TTI that the encoding is for
                3) Encoding is triggered in HW
===========================================================================*/
void mcalwcdma_wenc_phychan_encoding_control(
   ul_state_enum_type wl1_ul_state,
   l1_tti_enum_type prach_tti,
   uint8 nrf,
   l1_tti_enum_type longest_active_tti,
   uint8 num_active_trch,
   uint8 num_erama_trch
   , wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        mcalwcdma_wenc_phychan_tx_init

DESCRIPTION     This function programs phychan parameters, namely 
                1. Enable DPDCH or not
                2. Spreading Factor
                3. OVSF
                4. Scrambling code and Scrambling code type

DEPENDENCIES    Input : OVSF, Spreading factor, scrambling code, 
                        scrambling code type, phychan type

RETURN VALUE    None

SIDE EFFECTS    1) OVSF code is written for DPDCH 
                2) Uplink Scrambling code is written for PRACH and DPCH
                3) Spreading factor is written for both PRACH and DPCH
===========================================================================*/
void mcalwcdma_wenc_phychan_tx_init(
   uint16 dpch_ovsf,
   l1_sf_enum_type sf,
   uint32 prach_sc,
   l1_ul_dpch_sc_enum_type  dpch_scrambling_code_type,
   uint32 dpch_scrambling_code,
   l1_ul_phychan_enum_type ul_phychan
   , wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        mcalwcdma_wenc_prach_signal_tx

DESCRIPTION     This function basically sets phychan type to PRACH and sets 
                endianness. 

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    1) Physical Channel type is updated to PRACH
===========================================================================*/
void mcalwcdma_wenc_prach_signal_tx( void );

/*===========================================================================
FUNCTION        mcalwcdma_wenc_set_endianness

DESCRIPTION     This function sets endianness for the read back format of data 
                in ERAM. This needs to be set to 1 when accessing other TxMEM
                regions.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    1) Set the endianness of data 
                2) Fall time for Tx conditioning block is set 
===========================================================================*/
void mcalwcdma_wenc_set_endianness_fall_time( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        mcalwcdma_wenc_tx_abort

DESCRIPTION     This function programs phychan type to '0' to abort any tx 
                operation. 

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    Phychan type is cleared in HW
===========================================================================*/
void mcalwcdma_wenc_tx_abort( void );

/*===========================================================================
FUNCTION        mcalwcdma_wenc_wl1_enc_init

DESCRIPTION     This function is called during wl1_enc_init. 
                It initializes ERAM parameters and HS-DPCCH OVSF. 

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    1) Number of active and erama transport channels are cleared 
                2) ERAMA write location is initialized
                3) HS-DPCCH OVSF code is written with one DPDCH present
===========================================================================*/
void mcalwcdma_wenc_wl1_enc_init(void);

/*===========================================================================
FUNCTION        mcalwcdma_wenc_init_eul_hw

DESCRIPTION     This function writes the coding and the turbo interleaving 
                parameters to HW for EUL 

DEPENDENCIES    Inputs :  Carrier Index, 
                          EUL Pre rate matching parameters,
                          Turbo Interleaving parameters

RETURN VALUE    None

SIDE EFFECTS    1) Encoding Parameters are written to HW 
                2) Turbo Interleaving parameters are written to HW
                
===========================================================================*/

void mcalwcdma_wenc_init_eul_hw(
   wl1_ul_carr_id_enum_type carr_idx,
   const eul_pre_rm_param_struct_type* hw_coding_param_ptr, 
  ul_trch_turbo_ileav_parms_type *eulenc_turbo_ileav_parms_ptr
  , wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        mcalwcdma_wenc_hsrach_pmbl_hw_cfg

DESCRIPTION     This function initializes some HW parameters for HS-RACH, 
                such as phychan type, endianness and Tx Mode

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    1) Endianness of data is written to HW 
                2) Fall time for TX conditioning block is written to HW
                3) Phychan type is set to PRACH
===========================================================================*/
void mcalwcdma_wenc_hsrach_pmbl_hw_cfg( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        mcalwcdma_wenc_hsrach_process_tx_data

DESCRIPTION     This function programs basic parameters while HS_RACH 
                is in progress, such as ERAMA write pointer reinitialization,
                endianness control and phychan type configuration

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    1) ERAMA Write location is initialized 
                2) Phychan type is set to PRACH in HW
===========================================================================*/
void mcalwcdma_wenc_hsrach_process_tx_data(void);

/*===========================================================================
FUNCTION        mcalwcdma_wenc_wl1ulhsdpchmgr_activate_init

DESCRIPTION     This function performs HW register writes during the activation 
                of DPCH channels during FACH state as part of HS RACH 

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    1) ERAMA Write location is initialized 
                2) Phychan type is set to DPCH in HW
                3) Clear number of Active and ERAMA Transport Channels in HW
===========================================================================*/
void mcalwcdma_wenc_wl1ulhsdpchmgr_activate_init(void);


/*===========================================================================
FUNCTION        mcalwcdma_wenc_write_erama_ciph_ctl_words

DESCRIPTION     This function writes the ciphering control words in ERAMA 
                This function is called before each transport block's data is
                written to ERAMA.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    1) Transport block Control words for ciphering are written to 
                   ERAMA 
===========================================================================*/
void mcalwcdma_wenc_write_erama_ciph_ctl_words(uint32 algo, uint8 offset, uint8 key_idx, uint8 bearer_id, uint16 cipher_len, uint32 count);

/*===========================================================================
FUNCTION        mcalwcdma_wenc_write_erama_data_per_tb

DESCRIPTION     This function performs actual data writes to HW. 
                It first write Ciphering control words
                Second writes mac header data
                Finally writes the transport block data

DEPENDENCIES    Inputs : Transport channel index, transport channel data, 
                         transport block size, ciphering mode, total bits to be
                         ciphered

RETURN VALUE    FALSE if tb size is 0, TRUE otherwise

SIDE EFFECTS    1) Write Ciphering control parameters to HW 
                2) Write actual per transport block data to HW
                3) Update number of words written to HW for debugging (readback)
===========================================================================*/
boolean mcalwcdma_wenc_write_erama_data_per_tb(
   uint8 trch_idx,
   l1_ul_tb_data_type *trch_data,
   uint16 tb_size,
   uint32 ciphering_mode,
   uint16 total_bits_to_cipher
   , wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        mcalwcdma_wenc_write_scrambling_code

DESCRIPTION     This function writes scrambling code to HW by calling API 
                provided by HW team. 

DEPENDENCIES    Inputs : Scrambling code, scrambling code type

RETURN VALUE    None

SIDE EFFECTS    Scrambling code is updated in HW
===========================================================================*/
void mcalwcdma_wenc_write_scrambling_code(
   wl1_ul_carr_id_enum_type carr_idx,
   uint32 scrambling_code,
   l1_ul_dpch_sc_enum_type  scrambling_code_type, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        mcalwcdma_wenc_write_trch_cfg1_ram

DESCRIPTION     This function writes rate matching, encoding and interleaving 
                parameters to config ram for DPDCH and PRACH message per trch. 

DEPENDENCIES    Inputs : 
                Transport Channel Index 
                Rate Matching Parameters.
                  EINI, EMINUS, EPLUS
                Encoding Parameters
                  PRE RM BITS, TTI, CODING TYPE, PUNCTURING FLAG, NUM CODE SEGMENTS,
                  NUMBER OF FILLER BITS, NUMBER OF BITS PER CODE SEGMENTS
                Turbo Interleaving Parameters
                  NUM COLUMNS, LAST COLUMN, NUM ROW INDEX, LAST ROW BIT EXCHANGE
                  LAST ROW, PRIME NUMBER, PRIME NUMBER INDEX
                Mux order configuration
                  ERAMB START ADDRESS

RETURN VALUE    None

SIDE EFFECTS    Update Config_1 RAM parameters per transport channel
===========================================================================*/
void mcalwcdma_wenc_write_trch_cfg1_ram
(
  uint8 mux_order_buff_idx,
  mcalwcdma_wenc_write_trch_cfg1_ram_struct_type *mcalwcdma_wenc_write_trch_cfg1_ram_val,
  l1_ul_phychan_enum_type channel
   );


/*===========================================================================

FUNCTION        mcalwcdma_wenc_trch_mux_order_cfg

DESCRIPTION     Transport channel order for R99 encoding per transport channel

DEPENDENCIES    Inputs : Transport channel index, ERAMB read start address

RETURN VALUE    None

SIDE EFFECTS    1) Mux order config in HW is updated with Transport channel index 
                   and ERAMB start address for this transport channel
===========================================================================*/

void mcalwcdma_wenc_trch_mux_order_shadow_cfg
     (
  uint8 buff_idx,
      uint8			trch_index,          //trch processing order
      uint16			eramb_rd_start_addr  //eramb read for the corresponding trch index
     );

/*===========================================================================

FUNCTION      mcalwcdma_wenc_trch_crc_params_cfg

DESCRIPTION   CRC parameters for R99 per transport channel
              If data is 1x0, then crc size is written to tb_size field.

DEPENDENCIES  Inputs : Transport channel index, 
                       transport block size,
                       crc size,
                       number of transport blocks,
                       eramb write start address

RETURN VALUE   None

SIDE EFFECTS   Updates CRC parameters in HW
===========================================================================*/
void mcalwcdma_wenc_trch_crc_params_shadow_cfg
(
   uint8   trch_idx,                  //transport ch index
   uint16  crc_trblk_size,
   uint8   crc_size,
   uint8   crc_num_trblks,
   uint16  crc_eramb_wr_start_addr
   );

/*===========================================================================

FUNCTION       mcalwcdma_wenc_trch_coding_params_cfg

DESCRIPTION    Transport channel coding parameters for R99 per transport channel

DEPENDENCIES   Inputs : Transport Channel index, 
                        number of pre rate matching bits,
                        transport channel TTI,
                        number of code segments,
                        coding type and rate,
                        puncturing flag,
                        number of filler bits, 
                        number of code segments

RETURN VALUE   None 

SIDE EFFECTS  Updates Encoding parameters in HW
===========================================================================*/
void mcalwcdma_wenc_trch_coding_params_shadow_cfg
     (
        uint8   trch_idx,
        uint32  enc_num_pre_rm_bits,   
        uint8     enc_trch_tti,          
        uint8     enc_num_code_segs,     
        l1_ul_coding_enum_type     enc_coding_rate_type,  
        boolean     enc_puncture_flag,     
        uint32     enc_num_filler_bits,   
        uint32     enc_num_bits_code_seg 
     );


/*===========================================================================

FUNCTION      mcalwcdma_wenc_rm_params_cfg

DESCRIPTION   Programming rate matching params for R99 per transport channel

DEPENDENCIES  Inputs : Transport channel index, Rate matching parameters 

RETURN VALUE  None

SIDE EFFECTS  Updates the Rate matching parameters in HW
===========================================================================*/
void mcalwcdma_wenc_rm_params_shadow_cfg
     (
       uint8   trch_idx,   //transport ch index
       uint32  einit2,  
       uint32  einit1,  
       uint32  eminus2, 
       uint32  eminus1, 
       uint32  eplus2, 
       uint32  eplus1
     );

/*===========================================================================

FUNCTION      mcalwcdma_wenc_tintlv_params_cfg

DESCRIPTION   Turbo interleaver parameters for R99 per transport channel

DEPENDENCIES  Inputs : Tranport channel index, 
                       Turbo interleaving parameters  

RETURN VALUE  None 

SIDE EFFECTS  Updates the Turbo interleaving parameters in HW
===========================================================================*/
void mcalwcdma_wenc_tintlv_params_shadow_cfg
     (
      uint8   trch_idx,                  //transport ch index
      uint16  enc_num_columns_minus1,  
      uint16  enc_last_column,         
      uint16  enc_num_row_index,       
      uint16  enc_last_row_bit_exch,   
      uint16  enc_last_row,            
      uint16  enc_prime_num_index, 
      uint16  enc_prime_number
     );

/*===========================================================================

FUNCTION      mcalwcdma_wenc_cipher_keys_cfg

DESCRIPTION   Programming a single ciphering key for R99

DEPENDENCIES  Inputs : Ciphering key index, 
                       128 Bit ciphering key,
                       boolean to specify whether keys should be printed to diag

RETURN VALUE  None

SIDE EFFECTS  Updates HW register corresponding to key index provided, 
              with the ciphering key given in input
===========================================================================*/
void mcalwcdma_wenc_cipher_keys_shadow_cfg
     (
       uint8   cipher_key_num,   //specifies the cipher key# from 0-5
       uint32  cipher_key[4],     //128 bit cipher key
       boolean print_ciph_msg
     , wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      mcalwcdma_wenc_get_hw_sprd_factor

DESCRIPTION   This function will return the value expected by uKernel for each 
              possible spreading factor.
              The values expected by uKernel are provided by FW in the WFW API,
              In SW we've stored these values in an array which can be indexed by
              l1_sf_enum_type.

DEPENDENCIES  wenc_r99_sw_to_hw_spreading_factor_table

RETURN VALUE  uKernel spreading factor value

SIDE EFFECTS  None
===========================================================================*/
uint32 mcalwcdma_wenc_get_hw_sprd_factor(l1_sf_enum_type sf);

/*===========================================================================

FUNCTION      mcalwcdma_wenc_write_r99_chintlv_params

DESCRIPTION   This function writes the R99 channel interleaver parameters to 
              uKernel.

DEPENDENCIES  Number of normal slots in the upcoming frame 
              Final spreading factor of the upcoming frame (Incorporating SF change for CM)

RETURN VALUE  None

SIDE EFFECTS  SW shadow of channel interleaver parameters is updated
===========================================================================*/
void mcalwcdma_wenc_write_r99_chintlv_params(
  uint8 num_slots_txed,
  l1_sf_enum_type sf
  , wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_BOLT_2_0
#ifdef DEBUG_WCDMA_ERAM_RMRAM_DUMP
/*===========================================================================

FUNCTION      mcalwcdma_wenc_readback_data_from_hw

DESCRIPTION   Debug function to readback Encoder data and parameters from 
              uKernel 

DEPENDENCIES  wenc_mmap_ptr should have been initialized

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void mcalwcdma_wenc_readback_data_from_hw(mcalwcdma_wenc_umts_data_buf_enum_type data_buf_type, uint8 print_bmsk, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION mcalwcdma_wenc_print_hw_readback_data

DESCRIPTION
  This function will print out ERAM/RMRAM data readback from HW.
 
DEPENDENCIES
  mcalwcdma_wenc_readback_data_from_hw must be called with the
  appropriate data type.

RETURN VALUE
  None
 
SIDE EFFECTS
  None
===========================================================================*/
void mcalwcdma_wenc_print_hw_readback_data(uint16 print_bmsk, uint8 print_idx, wsub_id_e_type wsub_id);
/*===========================================================================
FUNCTION mcalwcdma_wenc_readback_data_from_hw_for_prach

DESCRIPTION
  This function is used to readback ERAM and RMRAM data for PRACH only

DEPENDENCIES
  None

RETURN VALUE
  None
 
SIDE EFFECTS
  None
===========================================================================*/
void mcalwcdma_wenc_readback_data_from_hw_for_prach( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION mcalwcdma_wenc_log_eul_async_params

DESCRIPTION
  This function is used to log the async params written to FW

DEPENDENCIES
  None

RETURN VALUE
  None
 
SIDE EFFECTS
  None
===========================================================================*/
void mcalwcdma_wenc_log_eul_async_params(uint16 param_buf_id, wsub_id_e_type wsub_id);
#endif
#endif
/*===========================================================================

FUNCTION      mcalwcdma_wenc_initialize_eram_data_ptrs

DESCRIPTION   This function reads the Encoder input buffer location in VPE0_LMEM 
              from the WAV uKernel API.
              Thereafter it initializes the Encoder Input Buffer data pointers
              to the above location.
              It also initializes the locations that the R99 CRC and control words
              are written to.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void mcalwcdma_wenc_initialize_eram_data_ptrs(void);

/*===========================================================================

FUNCTION      mcalwcdma_wenc_initialize_hw_memory_map_pointers

DESCRIPTION   This function initializes the SW shadow of the uKernel structures

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  Clears SW shadow of ukernel structures
===========================================================================*/
void mcalwcdma_wenc_initialize_ukernel_sw_shadow(void);

/*===========================================================================

FUNCTION      mcalwcdma_wenc_initialize_hw_memory_map_pointers

DESCRIPTION   This function initializes the pointers into ENC_LMEM. 
              These include the CFG RAM parameters pointers, the channel
              interleaver parameters pointers, the enc parameter pointers and
              the command pointer.
              
DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  Inializes the uKernel pointers
===========================================================================*/
void mcalwcdma_wenc_initialize_hw_memory_map_pointers(void);

/*===========================================================================

FUNCTION      mcalwcdma_wenc_populate_rach_second_frame_cmd

DESCRIPTION   This function is used to populate the second frame contents 
              for RACH 20ms TTI case.
              Encoder input length is programmed to 0 since encoding is already
              done.
              Command paramters are populated in uKernel c0 structure.
              EIB line address and start address are populated to point to the
              start of the VPE0_LMEM area allocated for EIB
              Channel interleaver, Enc params  and CFG1 RAM parameters are programmed to uKernel
 
DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  Updates Channel interleaver, CFG1 RAM and ENC params in uKernel
===========================================================================*/
void mcalwcdma_wenc_populate_rach_second_frame_cmd( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      mcalwcdma_wenc_populate_eul_encoding_cmd

DESCRIPTION   This function populates the EUL encoding command structure 
              in uKernel.
              EUL Carrier 0 is populated in uKernel command struct c1 and
              EUL Carrier 1 is populated in uKernel command struct c2.
              uKernel command struct c0 is reserved for R99.
              The data to be populated includes:
              1) Tech mode : specifies EUL and carrier ID
              2) VBUFF Parameters : This is meta data about ERAMA data in the VPE0_LMEM
              3) Line Address : This is the start location fo the ERAMA data in the VPE0_LMEM
              4) Start Address : This is the start location within the VBUFF starting at
                                 line address
              5) Encoder input length : This specifies teh length of the input data
                                        to be copied from VPE0_LMEM to ENC_LMEM
DEPENDENCIES  Requires number of words written to VPE0_LMEM + CRC bits for each carrier 
              Requires harq bank index for each carrier 

RETURN VALUE  None

SIDE EFFECTS  EUL encoding command structure is populated in uKernel
===========================================================================*/
void mcalwcdma_wenc_populate_eul_encoding_cmd(uint32 num_eul_words, uint8 harq_bank, wl1_ul_carr_id_enum_type carr_idx, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      mcalwcdma_wenc_calc_eul_num_chans

DESCRIPTION   This function returns the number of EDPDCH channels configured 
              given the spreading factor 

DEPENDENCIES  spreading factor is provided as input

RETURN VALUE  uint8 value representing the number of EDPDCH channels

SIDE EFFECTS  None
===========================================================================*/
uint8 mcalwcdma_wenc_calc_eul_num_chans(l1_e_max_ch_codes_enum_type sf, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      mcalwcdma_wenc_populate_eul_chintlv_cfg

DESCRIPTION   This function updates the channel interleaver parameters for EUL. 
              HW team mentioned that num_chans for UMTS has to be set to the
              maximum number of channels always which is 5 (DPDCH + 4xEDPDCH)
              irrespective of the configuration.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  uKernel is updated with SW channel interleaver parameters 
===========================================================================*/
void mcalwcdma_wenc_populate_eul_chintlv_cfg(l1_e_max_ch_codes_enum_type sf_c0, 
                                             wl1_ul_carr_id_enum_type carr_idx, wsub_id_e_type wsub_id);


/*===========================================================================

FUNCTION      mcalwcdma_wenc_copy_cmd_to_lmem

DESCRIPTION   This function will copy the encoder command structures to uKernel 
              This needs to be done before each encoding, for both R99 and EUL 

DEPENDENCIES  SW shadow of the encoding command structure should be populated 
              before calling this function 

RETURN VALUE  None

SIDE EFFECTS  uKernel will be updated with the command structure for the corresponding 
              channel type 
===========================================================================*/
void mcalwcdma_wenc_copy_cmd_to_lmem(mcalwcdma_wenc_enc_lmem_cmd_enum_type cmd_type, wsub_id_e_type wsub_id);


/*===========================================================================

FUNCTION      mcalwcdma_wenc_copy_params_to_lmem

DESCRIPTION   This function writes parameters to HW ENC_LMEM. The name of 
              parameters to be written are passed to this function as an enum type
              along with number of active and erama transport channels and the
              carrier index.

DEPENDENCIES  Since this function blindly copies the values into uKernel HW, 
              the SW shadow structures should have been populated prior to calling
              this function.

RETURN VALUE  None

SIDE EFFECTS  Updates uKernel with Encoder parameters
===========================================================================*/

void  mcalwcdma_wenc_copy_params_to_lmem(
  mcalwcdma_wenc_enc_lmem_param_enum_type params_to_copy, 
  uint8 num_active_trch, 
  uint8 num_erama_trch, 
  uint8 active_trch_bmsk,
  wl1_ul_carr_id_enum_type carr_id, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      mcalwcdma_wenc_get_eul_eib_address

DESCRIPTION   This function returns the encoder input buffer location for 
              EUL given the carrier and eram bank index 

DEPENDENCIES  carrier index and eram bank index for that carrier

RETURN VALUE  EIB address in VPE0_LMEM for that carrier and eram bank

SIDE EFFECTS  None
===========================================================================*/
uint32 mcalwcdma_wenc_get_eul_eib_address(uint8 eram_bank_id, wl1_ul_carr_id_enum_type carr_id, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      mcalwcdma_wenc_get_enc_cmd_status

DESCRIPTION   This function reads and returns the encoder microkernel status bits 
              From the ukernel command  
 
DEPENDENCIES  Can be called only after wenc_lmem_mmap_ptr is initialized in 
              mcalwcdma_wenc_initialize_hw_memory_map_pointers
 
RETURN VALUE  Returns read back value of ukernel command status

SIDE EFFECTS  None
===========================================================================*/
uint32 mcalwcdma_wenc_get_enc_cmd_status(mcalwcdma_wenc_enc_lmem_cmd_enum_type cmd_type, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      mcalwcdma_wenc_get_enc_state

DESCRIPTION   This function reads and returns the encoder state bits 
              From the ukernel command  
 
DEPENDENCIES  Can be called only after wenc_lmem_mmap_ptr is initialized in 
              mcalwcdma_wenc_initialize_hw_memory_map_pointers
 
RETURN VALUE  Returns read back value of encoder state

SIDE EFFECTS  None
===========================================================================*/
uint32 mcalwcdma_wenc_get_enc_state(mcalwcdma_wenc_enc_lmem_cmd_enum_type cmd_type, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      mcalwcdma_wenc_ftm_set_crc_params

DESCRIPTION   This function fills the CRC params for EUL during FTM
 
DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  CRC parameters will be updated in ukernel shadow
===========================================================================*/
void mcalwcdma_wenc_ftm_set_crc_params(uint16 num_bits);
#elif defined (FEATURE_WCDMA_JOLOKIA_MODEM)

#define EULENC_ERAM_MEMORY_BANK_TTI_2MS_BMSK   0x00
#define EULENC_ERAM_MEMORY_BANK_TTI_10MS_BMSK  0x08

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/
/* Transport Channel Config 0 RMRAM rate matching params struct  */
typedef struct
{
  uint8 trch_idx;
  uint16 tb_size;
  uint8 num_tb;
  uint16 eramb_start_addr;
  uint8 crc;
} mcalwcdma_wenc_write_trch_cfg0_ram_struct_type;

/* Transport Channel Config 1 RMRAM rate matching params struct */
typedef struct {
    uint8 trch_idx;
    ul_rm_parms_struct_type ul_rm_parms;
    ul_trch_coding_struct_type ul_trch_coding_parms;
    ul_trch_turbo_ileav_parms_type ul_trch_turbo_ileav_parms;
    uint16 eramb_start_addr;
    uint8 tti_enc_val;
    l1_ul_coding_enum_type cctrch_coding_type;
} mcalwcdma_wenc_write_trch_cfg1_ram_struct_type;

/*===========================================================================
FUNCTION        mcalwcdma_wenc_dpch_state_setup

DESCRIPTION     This function writes the Scrambling code, channel type 
                and endianness during DPCH setup. 

DEPENDENCIES    Inputs : Scrambling code, scrambling code type, phy chan type

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
void mcalwcdma_wenc_dpch_state_setup(uint32 scrambling_code, 
   l1_ul_dpch_sc_enum_type  scrambling_code_type, uint8 chan, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        mcalwcdma_wenc_dpch_tx_init

DESCRIPTION     This function is used to initialize DPCH parameters. 
                We program  TX_MODE_COMMON with 0x7D to keep tx HW in UMTS
                mode, and to enable all the clocks for each Tx Blocks,
                namely
 
                1. Tx Mem
                2. Tx Control
                3. Tx Encoder
                4. Tx Modulator
                5. Tx Filter

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
void mcalwcdma_wenc_dpch_tx_init( void );

/*===========================================================================
FUNCTION        mcalwcdma_wenc_erama_write_init

DESCRIPTION     In this function we initialize the parameters for writing to 
                ERAMA every TTI.
                1. Choose the Memory mapping (Using HW Default values, by prog 0)
                2. Program the Phychan type
                3. Program number of Active trchs and ERAMA trchs
                4. Program filler polarity
                5. Program endianness of data when written to ERAMA (Little End)

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
boolean mcalwcdma_wenc_erama_write_init(
   uint8 chan,
   uint8 num_active_trchs,
   uint8 num_erama_trchs,
   uint8 filler_val, wsub_id_e_type wsub_id);


/*===========================================================================
FUNCTION        mcalwcdma_wenc_calc_last_row_col_for_interleaving

DESCRIPTION     This function calculates the last row and column in RMRAM 
                of the bits written to RMRAM.
                Last Row = (Number of Bits - 1) / 30
                Last Column = (Number of Bits - 1) % 30
 
                Number of bits can change based on Spreading Factor and whether
                or not Compressed Mode is active

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
uint32 mcalwcdma_wenc_calc_last_row_col_for_interleaving(
   uint8 num_slots_txed,
   l1_sf_enum_type sf
   );

/*===========================================================================
FUNCTION        mcalwcdma_wenc_phychan_start_encoding

DESCRIPTION     This function is used to trigger R99 encoding. 
                It programs which frame in the TTI is being programmed
                and the last row and column of data in RMRAM, before trigerring
                encoding.

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
void mcalwcdma_wenc_phychan_start_encoding(
   ul_state_enum_type wl1_ul_state,
   l1_tti_enum_type prach_tti,
   uint8 nrf,
   uint32 dch_rach_last_row_col_req_val,
   l1_tti_enum_type longest_active_tti
 , wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        mcalwcdma_wenc_copy_rmram0_to_rmram1

DESCRIPTION     This function will copy the rate matched data from RMRAM0 to 
                RMRAM1 

DEPENDENCIES    Spreading factor is required to determine number of bytes to 
                transfer

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
void mcalwcdma_wenc_copy_rmram0_to_rmram1(uint8 spreading_factor, uint8 num_cm_slots_txed);

/*===========================================================================
FUNCTION        mcalwcdma_wenc_debug_log_registers

DESCRIPTION     This function will use the HW SW register debug interface 
                to log changes to the list of registers provided. 
 
DEPENDENCIES    None

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
void mcalwcdma_wenc_debug_log_registers_start(uint32 reg_addr[8], uint8 list_len);

/*===========================================================================
FUNCTION        mcalwcdma_wenc_debug_log_registers_end

DESCRIPTION     This function will use the HW SW register debug interface 
                to log changes to the list of registers provided. 
 
DEPENDENCIES    None

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
void mcalwcdma_wenc_debug_log_registers_end(void);

/*===========================================================================
FUNCTION        mcalwcdma_wenc_phychan_tx_init

DESCRIPTION     This function programs phychan parameters, namely 
                1. Enable DPDCH or not
                2. Spreading Factor
                3. OVSF
                4. Scrambling code and Scrambling code type

DEPENDENCIES    Input : OVSF, Spreading factor, scrambling code, 
                        scrambling code type, phychan type

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
void mcalwcdma_wenc_phychan_tx_init(
   uint16 dpch_ovsf,
   l1_sf_enum_type sf,
   uint32 prach_sc,
   l1_ul_dpch_sc_enum_type  dpch_scrambling_code_type,
   uint32 dpch_scrambling_code,
   l1_ul_phychan_enum_type ul_phychan
   , wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        mcalwcdma_wenc_prach_signal_tx

DESCRIPTION     This function basically sets phychan type to PRACH and sets 
                endianness. 

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
void mcalwcdma_wenc_prach_signal_tx( void );

/*===========================================================================
FUNCTION        mcalwcdma_wenc_set_endianness

DESCRIPTION     This function sets endianness for the read back format of data 
                in ERAM. This needs to be set to 1 when accessing other TxMEM
                regions.

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
void mcalwcdma_wenc_set_endianness( void );

/*===========================================================================
FUNCTION        mcalwcdma_wenc_tx_abort

DESCRIPTION     This function programs phychan type to '0' to abort any tx 
                operation. 

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
void mcalwcdma_wenc_tx_abort( void );

/*===========================================================================
FUNCTION        mcalwcdma_wenc_wl1_enc_init

DESCRIPTION     This function is called during wl1_enc_init. 
                It initializes ERAM parameters and HS-DPCCH OVSF. 

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
void mcalwcdma_wenc_wl1_enc_init(void);

/*===========================================================================
FUNCTION        mcalwcdma_wenc_write_erama_data

DESCRIPTION     This function performs actual data writes to HW. 
                It first write Ciphering control words
                Second writes mac header data
                Finally writes the transport block data

DEPENDENCIES    Inputs : Transport channel index, transport channel data, 
                         transport block size, ciphering mode, total bits to be
                         ciphered

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
boolean mcalwcdma_wenc_write_erama_data
(
   uint8 trch_idx, l1_ul_tb_data_type *trch_data, uint16 tb_size, uint32 ciphering_mode, uint16 total_bits_to_cipher
   , wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        mcalwcdma_wenc_write_scrambling_code

DESCRIPTION     This function writes scrambling code to HW by calling API 
                provided by HW team. 

DEPENDENCIES    Inputs : Scrambling code, scrambling code type

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
void mcalwcdma_wenc_write_scrambling_code(
   uint32 scrambling_code,
   l1_ul_dpch_sc_enum_type  scrambling_code_type, wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_TABASCO_MODEM
#ifdef FEATURE_WCDMA_DC_HSUPA
/*===========================================================================
FUNCTION        mcalwcdma_wenc_write_scrambling_code

DESCRIPTION     This function writes scrambling code to HW for 2nd carrier by calling API
                provided by HW team.

DEPENDENCIES    Inputs : Scrambling code, scrambling code type

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void mcalwcdma_wenc_write_scrambling_code_sec(
   uint32 scrambling_code,
   l1_ul_dpch_sc_enum_type  scrambling_code_type, wsub_id_e_type wsub_id);
#endif /* FEATURE_WCDMA_DC_HSUPA */
#endif /* FEATURE_WCDMA_TABASCO_MODEM */

/*===========================================================================
FUNCTION        mcalwcdma_wenc_write_trch_cfg0_ram_rach_dup

DESCRIPTION     This function writes the transport channel CRC parameters to 
                config ram for the 2nd frame of 20 ms TTI RACH message.

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
void mcalwcdma_wenc_write_trch_cfg0_ram_rach_dup( mcalwcdma_wenc_write_trch_cfg0_ram_struct_type *trch_cfg0_ram_struct );

/*===========================================================================
FUNCTION        mcalwcdma_wenc_write_trch_cfg1_ram

DESCRIPTION     This function writes rate matching, encoding and interleaving 
                parameters to  config ram for DPDCH and PRACH message. 

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
void mcalwcdma_wenc_write_trch_cfg1_ram
(
   mcalwcdma_wenc_write_trch_cfg1_ram_struct_type *mcalwcdma_wenc_write_trch_cfg1_ram_val
   );

/*===========================================================================
FUNCTION        mcalwcdma_wenc_write_trch_cfg1_ram_rach_dup

DESCRIPTION     This function writes rate matching, encoding and interleaving 
                parameters to  config ram for 2nd frame of 20ms TTI RACH  message

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
void mcalwcdma_wenc_write_trch_cfg1_ram_rach_dup (
  mcalwcdma_wenc_write_trch_cfg1_ram_struct_type *trch_cfg1_ram_struct
);

/*===========================================================================
FUNCTION        mcalwcdma_wenc_process_phychan_cfg_hsdpcch

DESCRIPTION     This function writes HS-DPCCH OVSF code to HW

DEPENDENCIES    Inputs : Number of DPDCH active 

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
void mcalwcdma_wenc_process_phychan_cfg_hsdpcch(uint8 num_dpdch);

/*===========================================================================
FUNCTION        mcalwcdma_wenc_hsrach_process_tx_data_init

DESCRIPTION     This function initializes some HW parameters for HS-RACH, 
                such as phychan type, endianness and Tx Mode

DEPENDENCIES     

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
void mcalwcdma_wenc_hsrach_process_tx_data_init(void);

/*===========================================================================
FUNCTION        mcalwcdma_wenc_hsrach_process_tx_data

DESCRIPTION     This function programs basic parameters while HS_RACH 
                is in progress, such as ERAMA write pointer reinitialization,
                endianness control and phychan type configuration

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
void mcalwcdma_wenc_hsrach_process_tx_data(void);

/*===========================================================================
FUNCTION        mcalwcdma_wenc_wl1ulhsdpchmgr_activate_init

DESCRIPTION     This function performs HW register writes during the activation 
                of DPCH channels during FACH state as part of HS RACH 

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
void mcalwcdma_wenc_wl1ulhsdpchmgr_activate_init(void);

/*===========================================================================

FUNCTION    tx_umts_hsdpcch_ovsf_cfg

DESCRIPTION 

===========================================================================*/

void mcalwcdma_wenc_write_hsdpcch_ovsf
     (
       uint8  ovsf
     );

/*===========================================================================

FUNCTION    tx_umts_trch_mux_order_cfg

DESCRIPTION transport channel order for R99 encoding

===========================================================================*/

void mcalwcdma_wenc_write_mux_order
     (
      uint8			trch_index,          //trch processing order
      uint16			eramb_rd_start_addr  //eramb read for the corresponding trch index
     );

/*===========================================================================

FUNCTION    tx_umts_scr_cfg

DESCRIPTION 

===========================================================================*/

void mcalwcdma_wenc_scrambling_config
     (
        uint32  scr_code_init,
        l1_ul_dpch_sc_enum_type scr_code_type,
        uint32     pn_load
     );

#ifdef FEATURE_WCDMA_TABASCO_MODEM
#ifdef FEATURE_WCDMA_DC_HSUPA
/*===========================================================================
FUNCTION    tx_umts_scr_cfg for 2nd carrier

DESCRIPTION

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS
===========================================================================*/
void mcalwcdma_wenc_scrambling_config_sec
     (
        uint32  scr_code_init,
        l1_ul_dpch_sc_enum_type scr_code_type,
        uint32     pn_load
     );
#endif /*FEATURE_WCDMA_DC_HSUPA*/
#endif /*FEATURE_WCDMA_TABASCO_MODEM*/


/*===========================================================================

FUNCTION    mcalwcdma_write_eul_tti_mode

DESCRIPTION SW can program EUL TTI when required

===========================================================================*/

void mcalwcdma_write_eul_tti_mode
     (
      e_tti_enum_type tti
     );

/*===========================================================================

FUNCTION    tx_umts_phy_ch_type_cfg

DESCRIPTION SW can program this register when required

===========================================================================*/

void mcalwcdma_wenc_write_phychan_type
     (
      l1_ul_dpch_sc_enum_type  ch_type
     );

/*===========================================================================

FUNCTION    tx_umts_sw_trch_params_cfg

DESCRIPTION turbo interleaver parameters for R99

===========================================================================*/
void mcalwcdma_wenc_write_crc_params
     (
      uint8   trch_idx,                  //transport ch index
      uint16   crc_trblk_size,
      uint8     crc_size,
      uint8     crc_num_trblks,
      uint16     crc_eramb_wr_start_addr
     );

void mcalwcdma_wenc_write_trch_coding_params
     (
        uint8   trch_idx,
        uint32  enc_num_pre_rm_bits,   
        uint8     enc_trch_tti,          
        uint8     enc_num_code_segs,     
        l1_ul_coding_enum_type     enc_coding_rate_type,  
        boolean     enc_puncture_flag,     
        uint32     enc_num_filler_bits,   
        uint32     enc_num_bits_code_seg 
     );

/*===========================================================================

FUNCTION    tx_umts_sw_rm_params_cfg

DESCRIPTION programming rate matching params for R99

===========================================================================*/
void mcalwcdma_wenc_write_rm_parms
     (
       uint8   trch_idx,   //transport ch index
       uint32  einit2,  
       uint32  einit1,  
       uint32  eminus2, 
       uint32  eminus1, 
       uint32  eplus2, 
       uint32  eplus1
     );

/*===========================================================================

FUNCTION    tx_umts_sw_tintlv_params_cfg

DESCRIPTION turbo interleaver parameters for R99

===========================================================================*/
void mcalwcdma_wenc_write_turbo_intlv_params
     (
      uint8   trch_idx,                  //transport ch index
      uint16  enc_num_columns_minus1,  
      uint16  enc_last_column,         
      uint16  enc_num_row_index,       
      uint16  enc_last_row_bit_exch,   
      uint16  enc_last_row,            
      uint16  enc_prime_num_index, 
      uint16  enc_prime_number
     );

/*===========================================================================

FUNCTION    tx_umts_sw_cipher_keys_cfg

DESCRIPTION programming the ciphering keys required for R99

===========================================================================*/
void mcalwcdma_wenc_write_ciphering_keys
     (
       uint8   cipher_key_num,   //specifies the cipher key# from 0-5
       uint32  cipher_key[4]     //128 bit cipher key
     );

/*===========================================================================
FUNCTION        mcalwcdma_wenc_write_eul_tti_bank_idx

DESCRIPTION     This function writes the EUL ERAM bank index and TTI value to 
                HW and reinitializes the EUL INCRACC write location in EUL ERAM

DEPENDENCIES    Inputs : Carrier Index, Harq bank index, EUL TTI

RETURN VALUE    returns the value written to TX_EUL_ERAM_MEMORY_BANK HW register

SIDE EFFECTS    1) Updates HW register for HARQ Memory Bank 
                2) Reinitializes EUL INCRACC write address
===========================================================================*/
uint32 mcalwcdma_wenc_write_eul_tti_bank_idx(wl1_ul_carr_id_enum_type carr_idx,
                                             uint32 eram_bank_idx, e_tti_enum_type tti);

#if defined(DEBUG_ERAMA_RMRAM_DATA_DUMP) && defined(FEATURE_WCDMA_16_QAM)
/*===========================================================================
FUNCTION        mcalwcdma_wenc_eram_iram_data_dump

DESCRIPTION     This function reads Tx mem and stores the eram and iram 
                contents in  'mcalwcdma_wenc_data_dump'
                The TX Mem is organized into 15 pages of size 1024 words each.
                To read from Tx Mem we simply cycle through all the pages
                based on bitmask provided.

DEPENDENCIES    None

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
void mcalwcdma_wenc_eram_iram_data_dump(uint16 bitmask, uint8 new_or_retx);

#endif /* DEBUG_ERAMA_RMRAM_DATA_DUMP */

/* FEATURE_WCDMA_JOLOKIA_MODEM */


#else
/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/
typedef struct {
    uint8 trch_idx;
    ul_rm_parms_struct_type ul_rm_parms;
    ul_trch_coding_struct_type ul_trch_coding_parms;
    ul_trch_turbo_ileav_parms_type ul_trch_turbo_ileav_parms;
    uint16 eramb_start_addr;
    uint8 tti_enc_val;
    l1_ul_coding_enum_type cctrch_coding_type;
}mcalwcdma_wenc_write_trch_cfg1_ram_struct_type;

/*===========================================================================
FUNCTION        mcalwcdma_wenc_dpch_state_setup

DESCRIPTION     This function writes the Scrambling code, channel type 
                and endianness during DPCH setup. 

DEPENDENCIES    Inputs : Scrambling code, scrambling code type, phy chan type

RETURN VALUE    None

SIDE EFFECTS    1) Will update scrambling code in HW 
                2) Will set the endianness for R99 Data and the Fall time
                3) Will update the physical channel type based on input
===========================================================================*/

void mcalwcdma_wenc_dpch_state_setup(
   uint32 scrambling_code,
   l1_ul_dpch_sc_enum_type  scrambling_code_type,
   uint8 chan
   , wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        mcalwcdma_wenc_process_phychan_cfg_hsdpcch

DESCRIPTION     This function writes HS-DPCCH OVSF code to HW

DEPENDENCIES    Inputs : Number of DPDCH active 

RETURN VALUE    None

SIDE EFFECTS    Will set the HS-DPCCH OVSF based on if DPDCH is active or not
===========================================================================*/
void mcalwcdma_wenc_process_phychan_cfg_hsdpcch(uint8 num_dpdch);

/*===========================================================================
FUNCTION        mcalwcdma_wenc_dpch_tx_init

DESCRIPTION     This function is used to initialize DPCH parameters. 

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    1) Will initialize the ERAMA write pointer so writes to ERAMA 
                   after this point will be from the beginning of ERAMA
                2) Will write Phychan type to HW as DPCH
                3) Will clear active Transport Channels information in HW
                
===========================================================================*/
void mcalwcdma_wenc_dpch_tx_init( void );

/*===========================================================================
FUNCTION        mcalwcdma_wenc_erama_write_init

DESCRIPTION     In this function WL1 will initialize the parameters for writing to 
                ERAMA every TTI.
                1. Choose the Memory mapping (Using HW Default values, by prog 0)
                2. Program the Phychan type
                3. Program number of Active trchs and ERAMA trchs
                4. Program filler polarity
                5. Program endianness of data when written to ERAMA (Little End)

DEPENDENCIES    Inputs : Physical channel type
                         Number of active transport channels 
                         Num of transport channels to be written to ERAMA this frame
                         Filler polarity based on filler val(0 or 2)

RETURN VALUE    FALSE if encoder is busy, TRUE otherwise

SIDE EFFECTS    1) Reset ERAMA write address 
                2) Write the physical channel type
                3) Set the number of active and erama transport channels
                4) Set the filler polarity
===========================================================================*/
void mcalwcdma_wenc_erama_write_init(
   l1_ul_phychan_enum_type chan,
   uint8 num_active_trchs,
   uint8 num_erama_trchs,
   l1_ul_filler_polarity_enum_type filler_val, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        mcalwcdma_wenc_phychan_start_encoding

DESCRIPTION     This function is used to trigger R99 encoding. 
                It programs which frame in the TTI is being programmed
                and the last row and column of data in RMRAM, before trigerring
                encoding.

DEPENDENCIES    Inputs : Current uplink state, TTI for PRACH, next radio frame mod 8 
                         longest active TTI incase of DPCH 
  

RETURN VALUE    None

SIDE EFFECTS    1) Reset ERAMA Write pointer 
                2) Write the frame number within the TTI that the encoding is for
                3) Encoding is triggered in HW
===========================================================================*/
void mcalwcdma_wenc_phychan_start_encoding(
   ul_state_enum_type wl1_ul_state,
   l1_tti_enum_type prach_tti,
   uint8 nrf,
   l1_tti_enum_type longest_active_tti
   , wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        mcalwcdma_wenc_phychan_tx_init

DESCRIPTION     This function programs phychan parameters, namely 
                1. Enable DPDCH or not
                2. Spreading Factor
                3. OVSF
                4. Scrambling code and Scrambling code type

DEPENDENCIES    Input : OVSF, Spreading factor, scrambling code, 
                        scrambling code type, phychan type

RETURN VALUE    None

SIDE EFFECTS    1) OVSF code is written for DPDCH 
                2) Uplink Scrambling code is written for PRACH and DPCH
                3) Spreading factor is written for both PRACH and DPCH
===========================================================================*/
void mcalwcdma_wenc_phychan_tx_init(
   uint16 dpch_ovsf,
   l1_sf_enum_type sf,
   uint32 prach_sc,
   l1_ul_dpch_sc_enum_type  dpch_scrambling_code_type,
   uint32 dpch_scrambling_code,
   l1_ul_phychan_enum_type ul_phychan
   , wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        mcalwcdma_wenc_prach_signal_tx

DESCRIPTION     This function basically sets phychan type to PRACH and sets 
                endianness. 

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    1) Physical Channel type is updated to PRACH
===========================================================================*/
void mcalwcdma_wenc_prach_signal_tx( void );

/*===========================================================================
FUNCTION        mcalwcdma_wenc_set_endianness

DESCRIPTION     This function sets endianness for the read back format of data 
                in ERAM. This needs to be set to 1 when accessing other TxMEM
                regions.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    1) Set the endianness of data 
                2) Fall time for Tx conditioning block is set 
===========================================================================*/
void mcalwcdma_wenc_set_endianness_fall_time( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        mcalwcdma_wenc_tx_abort

DESCRIPTION     This function programs phychan type to '0' to abort any tx 
                operation. 

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    Phychan type is cleared in HW
===========================================================================*/
void mcalwcdma_wenc_tx_abort( void );

/*===========================================================================
FUNCTION        mcalwcdma_wenc_wl1_enc_init

DESCRIPTION     This function is called during wl1_enc_init. 
                It initializes ERAM parameters and HS-DPCCH OVSF. 

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    1) Number of active and erama transport channels are cleared 
                2) ERAMA write location is initialized
                3) HS-DPCCH OVSF code is written with one DPDCH present
===========================================================================*/
void mcalwcdma_wenc_wl1_enc_init(void);

/*===========================================================================
FUNCTION        mcalwcdma_wenc_write_eul_tti_bank_idx

DESCRIPTION     This function writes the EUL ERAM bank index and TTI value to 
                HW and reinitializes the EUL INCRACC write location in EUL ERAM

DEPENDENCIES    Inputs : Carrier Index, Harq bank index, EUL TTI

RETURN VALUE    returns the value written to TX_EUL_ERAM_MEMORY_BANK HW register

SIDE EFFECTS    1) Updates HW register for HARQ Memory Bank 
                2) Reinitializes EUL INCRACC write address
===========================================================================*/
uint32 mcalwcdma_wenc_write_eul_tti_bank_idx(wl1_ul_carr_id_enum_type carr_idx, uint32 eram_bank_idx, e_tti_enum_type tti);

/*===========================================================================
FUNCTION        mcalwcdma_wenc_init_eul_hw

DESCRIPTION     This function writes the coding and the turbo interleaving 
                parameters to HW for EUL 

DEPENDENCIES    Inputs :  Carrier Index, 
                          HARQ bank index, 
                          EUL Pre rate matching parameters,
                          Turbo Interleaving parameters

RETURN VALUE    None

SIDE EFFECTS    1) Encoding Parameters are written to HW 
                2) Turbo Interleaving parameters are written to HW
                
===========================================================================*/
void mcalwcdma_wenc_init_eul_hw(
   wl1_ul_carr_id_enum_type carr_idx,
   uint32 eram_bank_idx, 
   const eul_pre_rm_param_struct_type* hw_coding_param_ptr, 
   uint32 turbo_interlev_info, 
   uint32 turbo_interlev_prime_info, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        mcalwcdma_wenc_hsrach_pmbl_hw_cfg

DESCRIPTION     This function initializes some HW parameters for HS-RACH, 
                such as phychan type, endianness and Tx Mode

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    1) Endianness of data is written to HW 
                2) Fall time for TX conditioning block is written to HW
                3) Phychan type is set to PRACH
===========================================================================*/
void mcalwcdma_wenc_hsrach_pmbl_hw_cfg( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        mcalwcdma_wenc_hsrach_process_tx_data

DESCRIPTION     This function programs basic parameters while HS_RACH 
                is in progress, such as ERAMA write pointer reinitialization,
                endianness control and phychan type configuration

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    1) ERAMA Write location is initialized 
                2) Phychan type is set to PRACH in HW
===========================================================================*/
void mcalwcdma_wenc_hsrach_process_tx_data(void);

/*===========================================================================
FUNCTION        mcalwcdma_wenc_wl1ulhsdpchmgr_activate_init

DESCRIPTION     This function performs HW register writes during the activation 
                of DPCH channels during FACH state as part of HS RACH 

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    1) ERAMA Write location is initialized 
                2) Phychan type is set to DPCH in HW
                3) Clear number of Active and ERAMA Transport Channels in HW
===========================================================================*/
void mcalwcdma_wenc_wl1ulhsdpchmgr_activate_init(void);

/*===========================================================================
FUNCTION        mcalwcdma_wenc_write_erama_data_per_tb

DESCRIPTION     This function performs actual data writes to HW. 
                It first write Ciphering control words
                Second writes mac header data
                Finally writes the transport block data

DEPENDENCIES    Inputs : Transport channel index, transport channel data, 
                         transport block size, ciphering mode, total bits to be
                         ciphered

RETURN VALUE    FALSE if tb size is 0, TRUE otherwise

SIDE EFFECTS    1) Write Ciphering control parameters to HW 
                2) Write actual per transport block data to HW
                3) Update number of words written to HW for debugging (readback)
===========================================================================*/
boolean mcalwcdma_wenc_write_erama_data_per_tb(
   uint8 trch_idx,
   l1_ul_tb_data_type *trch_data,
   uint16 tb_size,
   uint32 ciphering_mode,
   uint16 total_bits_to_cipher
   , wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        mcalwcdma_wenc_write_scrambling_code

DESCRIPTION     This function writes scrambling code to HW by calling API 
                provided by HW team. 

DEPENDENCIES    Inputs : Scrambling code, scrambling code type

RETURN VALUE    None

SIDE EFFECTS    Scrambling code is updated in HW
===========================================================================*/
void mcalwcdma_wenc_write_scrambling_code(
   uint32 scrambling_code,
   l1_ul_dpch_sc_enum_type  scrambling_code_type, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        mcalwcdma_wenc_write_trch_cfg1_ram

DESCRIPTION     This function writes rate matching, encoding and interleaving 
                parameters to config ram for DPDCH and PRACH message per trch. 

DEPENDENCIES    Inputs : 
                Transport Channel Index 
                Rate Matching Parameters.
                  EINI, EMINUS, EPLUS
                Encoding Parameters
                  PRE RM BITS, TTI, CODING TYPE, PUNCTURING FLAG, NUM CODE SEGMENTS,
                  NUMBER OF FILLER BITS, NUMBER OF BITS PER CODE SEGMENTS
                Turbo Interleaving Parameters
                  NUM COLUMNS, LAST COLUMN, NUM ROW INDEX, LAST ROW BIT EXCHANGE
                  LAST ROW, PRIME NUMBER, PRIME NUMBER INDEX
                Mux order configuration
                  ERAMB START ADDRESS

RETURN VALUE    None

SIDE EFFECTS    Update Config_1 RAM parameters per transport channel
===========================================================================*/
void mcalwcdma_wenc_write_trch_cfg1_ram
(
   mcalwcdma_wenc_write_trch_cfg1_ram_struct_type *mcalwcdma_wenc_write_trch_cfg1_ram_val
   );


/*===========================================================================

FUNCTION        mcalwcdma_wenc_trch_mux_order_cfg

DESCRIPTION     Transport channel order for R99 encoding per transport channel

DEPENDENCIES    Inputs : Transport channel index, ERAMB read start address

RETURN VALUE    None

SIDE EFFECTS    1) Mux order config in HW is updated with Transport channel index 
                   and ERAMB start address for this transport channel
===========================================================================*/

void mcalwcdma_wenc_trch_mux_order_cfg
     (
      uint8			trch_index,          //trch processing order
      uint16			eramb_rd_start_addr  //eramb read for the corresponding trch index
     );

/*===========================================================================

FUNCTION      mcalwcdma_wenc_trch_crc_params_cfg

DESCRIPTION   CRC parameters for R99 per transport channel
              If data is 1x0, then crc size is written to tb_size field.

DEPENDENCIES  Inputs : Transport channel index, 
                       transport block size,
                       crc size,
                       number of transport blocks,
                       eramb write start address

RETURN VALUE   None

SIDE EFFECTS   Updates CRC parameters in HW
===========================================================================*/
void mcalwcdma_wenc_trch_crc_params_cfg
(
   uint8   trch_idx,                  //transport ch index
   uint16  crc_trblk_size,
   uint8   crc_size,
   uint8   crc_num_trblks,
   uint16  crc_eramb_wr_start_addr
   );

/*===========================================================================

FUNCTION       mcalwcdma_wenc_trch_coding_params_cfg

DESCRIPTION    Transport channel coding parameters for R99 per transport channel

DEPENDENCIES   Inputs : Transport Channel index, 
                        number of pre rate matching bits,
                        transport channel TTI,
                        number of code segments,
                        coding type and rate,
                        puncturing flag,
                        number of filler bits, 
                        number of code segments

RETURN VALUE   None 

SIDE EFFECTS  Updates Encoding parameters in HW
===========================================================================*/
void mcalwcdma_wenc_trch_coding_params_cfg
     (
        uint8   trch_idx,
        uint32  enc_num_pre_rm_bits,   
        uint8     enc_trch_tti,          
        uint8     enc_num_code_segs,     
        l1_ul_coding_enum_type     enc_coding_rate_type,  
        boolean     enc_puncture_flag,     
        uint32     enc_num_filler_bits,   
        uint32     enc_num_bits_code_seg 
     );


/*===========================================================================

FUNCTION      mcalwcdma_wenc_rm_params_cfg

DESCRIPTION   Programming rate matching params for R99 per transport channel

DEPENDENCIES  Inputs : Transport channel index, Rate matching parameters 

RETURN VALUE  None

SIDE EFFECTS  Updates the Rate matching parameters in HW
===========================================================================*/
void mcalwcdma_wenc_rm_params_cfg
     (
       uint8   trch_idx,   //transport ch index
       uint32  einit2,  
       uint32  einit1,  
       uint32  eminus2, 
       uint32  eminus1, 
       uint32  eplus2, 
       uint32  eplus1
     );

/*===========================================================================

FUNCTION      mcalwcdma_wenc_tintlv_params_cfg

DESCRIPTION   Turbo interleaver parameters for R99 per transport channel

DEPENDENCIES  Inputs : Tranport channel index, 
                       Turbo interleaving parameters  

RETURN VALUE  None 

SIDE EFFECTS  Updates the Turbo interleaving parameters in HW
===========================================================================*/
void mcalwcdma_wenc_tintlv_params_cfg
     (
      uint8   trch_idx,                  //transport ch index
      uint16  enc_num_columns_minus1,  
      uint16  enc_last_column,         
      uint16  enc_num_row_index,       
      uint16  enc_last_row_bit_exch,   
      uint16  enc_last_row,            
      uint16  enc_prime_num_index, 
      uint16  enc_prime_number
     );

/*===========================================================================

FUNCTION      mcalwcdma_wenc_cipher_keys_cfg

DESCRIPTION   Programming a single ciphering key for R99

DEPENDENCIES  Inputs : Ciphering key index, 
                       128 Bit ciphering key,
                       boolean to specify whether keys should be printed to diag

RETURN VALUE  None

SIDE EFFECTS  Updates HW register corresponding to key index provided, 
              with the ciphering key given in input
===========================================================================*/
void mcalwcdma_wenc_cipher_keys_cfg
     (
       uint8   cipher_key_num,   //specifies the cipher key# from 0-5
       uint32  cipher_key[4],     //128 bit cipher key
       boolean print_ciph_msg
     , wsub_id_e_type wsub_id);
#endif //FEATURE_WCDMA_BOLT_2_0

#ifdef FEATURE_WCDMA_JOLOKIA_MODEM
// Nothing to be done for JOLOKIA. Just to keep the interface same
#elif defined(FEATURE_WCDMA_BOLT_2_0)
//TODO : Resync function for Bolt
#else
/*===========================================================================
FUNCTION wenc_resync_tx_path()

DESCRIPTION
This function is used to clean up the Tx Data Path. Specifically 
the TXC0 and TXR0 are reset every time this function is executed.
The registers to perform the action is TX_MODE and TX_MODE_c.
This is only a temporary workaround until the root cause is debugged.

DEPENDENCIES
  None

RETURN VALUE
  None
 
SIDE EFFECTS
TXC0 and TXR0 will be RESET and the HW Internal states 
will be returned to power up INIT state
===========================================================================*/
void mcalwcdma_wenc_resync_tx_path( wsub_id_e_type wsub_id);
#endif
#endif /* __MCALWCDMA_WENC_H__ */
