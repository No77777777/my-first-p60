/*!
  @file
  rflm_lte_log.h

  @brief
  RF LOG API definitions for LTE to interact with SW and LM
  
  @detail
  This file will contain all definitios and declarations to be
  shared with SW and LM for LTE LM Logging
  
  @todo
 

*/

/*==============================================================================

  Copyright (c) 2014 - 2015 Qualcomm Technologies, Inc. (QTI). All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

==============================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rflm.mpss/2.11/api/lte/rflm_lte_log.h#1 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
10/13/15   kab     Fix FTM RF PCC TxAGC log packet
06/25/15   ska     Adding support for TxAGC logs 0x184C version3
05/28/15   kab     ULCA 0x184C log packet support
04/23/15   ska     Modified TxAGC log packet 0x184C version2
01/29/14   svi     Making FED build happy
01/28/14   svi     Initial Version - Support for LTE LM Logging


==============================================================================*/

#ifndef RFLM_LTE_LOG_H
#define RFLM_LTE_LOG_H


/*==============================================================================

                           INCLUDE FILES

==============================================================================*/

#include "rflm.h"
#include "rflm_api_lte.h"
#include "rflm_lte.h"
#include "rflm_diag_log.h"
#include "log.h"

#include "rflm_time.h"
#if (RFLM_FEATURE_BUILD_MODE != RFLM_FEATURE_BUILD_FW_LIB)
#include "prof_hist.h"
#endif

/*==============================================================================

                EXTERNAL DEFINITIONS AND TYPES : MACROS

==============================================================================*/
#if (RFLM_FEATURE_BUILD_MODE != RFLM_FEATURE_BUILD_FW_LIB)
extern prof_hist_log_packet_handle_t rf_lte_hist_packet_handle;
extern prof_hist_param_t rf_lte_tx_fed_hist;
extern prof_hist_param_t rf_lte_tx_mtpl_hist;
extern prof_hist_param_t rf_lte_rx_inst_rssi_hist;
extern prof_hist_param_t rf_lte_rx_config_lna_hist;
extern prof_hist_param_t rf_lte_rx_toggle_hist;
extern prof_hist_param_t rf_lte_rx_exec_config_hist;
#endif


/*==============================================================================

                EXTERNAL DEFINITIONS AND TYPES : MACROS

==============================================================================*/



/************************************************************/
/*                   MISC DEFINITIONS                       */
/************************************************************/
#if (RFLM_FEATURE_BUILD_MODE != RFLM_FEATURE_BUILD_FW_LIB)
void rflm_log_and_clear_hist();
#endif

/************************************************************/
/*            COMMON LM TO LTE LM CONVERSION                */
/************************************************************/

/*----------------------------------------------------------------------------*/
/*! @brief RFLM LTE Internal log packet ID */
#define rflm_lte_diag_log_return_code_t rflm_diag_log_return_code_t

/*----------------------------------------------------------------------------*/



/************************************************************/
/*                LOG PACKET IDENTIFICATION                 */
/************************************************************/

/*----------------------------------------------------------------------------*/
/*! @brief RFLM LTE Internal log packet ID */
#define RFLM_LTE_TXAGC_INTERNAL_LOG_PACKET_ID                0x184D

/*----------------------------------------------------------------------------*/
/*! @brief RFLM LTE External log packet ID */
#define RFLM_LTE_TXAGC_EXTERNAL_LOG_PACKET_ID                0x184C

/*----------------------------------------------------------------------------*/
/*! @brief RFLM LTE Internal TxAGC user ID */
#define RFLM_LTE_LOG_TXAGC_INTERNAL_PACKET_ID                0

/*----------------------------------------------------------------------------*/
/*! @brief RFLM LTE External TxAGC user ID */
#define RFLM_LTE_LOG_TXAGC_EXTERNAL_PACKET_ID                1

/*----------------------------------------------------------------------------*/
/*! @brief RFLM LTE Internal TxAGC tag size */
#define RFLM_LTE_LOG_TXAGC_INTERNAL_PACKET_TAG_SIZE_WORDS    0

/*----------------------------------------------------------------------------*/
/*! @brief RFLM LTE External TxAGC tag size */
#define RFLM_LTE_LOG_TXAGC_EXTERNAL_PACKET_TAG_SIZE_WORDS    0



/************************************************************/
/*                   LOG PACKET VERSIONS                    */
/************************************************************/


/*----------------------------------------------------------------------------*/
/*!@brief Enumeration indicating the TxAGC Internal Log Packet version */
typedef enum
{
  RFLM_LTE_TXAGC_INTERNAL_LOG_VER_1 = 1, /*!< TxAGC Internal Log Packet Version 1 */
  RFLM_LTE_TXAGC_INTERNAL_LOG_VER_2 = 2, /*!< TxAGC Internal Log Packet Version 2 */
} rflm_lte_agc_txagc_internal_version_type;

/*----------------------------------------------------------------------------*/
/*!@brief Enumeration indicating the TxAGC External Log Packet version */
typedef enum
{
  RFLM_LTE_TXAGC_EXTERNAL_LOG_VER_1 = 1, /*!< TxAGC External Log Packet Version 1 */
  RFLM_LTE_TXAGC_EXTERNAL_LOG_VER_2 = 2, /*!< TxAGC External Log Packet Version 2 */
  RFLM_LTE_TXAGC_EXTERNAL_LOG_VER_3 = 3, /*!< TxAGC External Log Packet Version 3 */
} rflm_lte_agc_txagc_external_version_type;

/*----------------------------------------------------------------------------*/



/************************************************************/
/*                   MISC DEFINITIONS                       */
/************************************************************/


/*----------------------------------------------------------------------------*/
/*! @brief RFLM LTE Log data to be filled in reserved fields */
#define RFLM_LTE_LOG_RESERVED_FIELD_DATA                     0

/*----------------------------------------------------------------------------*/



/*==============================================================================

              EXTERNAL DEFINITIONS AND TYPES : ENUMS

==============================================================================*/

/************************************************************/
/*                  LOG API RETURN STATUS                   */
/************************************************************/

/*----------------------------------------------------------------------------*/
/*! 
  @brief  
  Enumeration indicating error type for RFLM LTE Logging
*/
typedef enum
{
  RFLM_LTE_LOG_ERR_NONE,         /*!< RFLM LTE Logging Success */
  RFLM_LTE_LOG_DIAG_ALLCO_ERR,   /*!< RFLM LTE Logging Failure as Diag Lof Alloc Failed */
  RFLM_LTE_LOG_ERR,              /*!< RFLM LTE Logging Failure */
  RFLM_LTE_LOG_MAX_ERR           /*!< RFLM LTE Logging Invalid Status */

}rflm_lte_log_err_t;

/*==============================================================================

            EXTERNAL DEFINITIONS AND TYPES : STRUCTURES

==============================================================================*/


/*----------------------------------------------------------------------------*/
/*! 
  @brief  
  TxAGC Internal Log packet
*/
typedef struct
{

  /*********************************************************/
  /*!                 LOG PACKET SETTINGS                  */
  /*********************************************************/ 
  
  uint32 log_version                       :  8; /*!< Internal Log Packet Version */
  uint32 log_info                          : 24; /*!< Reserved */

  uint32 log_index                         : 32; /*!< Log Index in Debug Structure */ 

  /*********************************************************/
  /*!               LOG SUB FRAME SETTINGS                 */
  /*********************************************************/ 

  uint32 frame_num                         : 32; /*!< Frame Number */

  uint32 sf_number                         :  8; /*!< Sub Frame Number */
  uint32 subframe_type                     : 16; /*!< Sub Frame Type */    
  uint32 sf_info                           :  8; /*!< Reserved */  

  uint32 freq_hopping_enable               :  8; /*!< Freq Hopping is enabled */
  uint32 txpll_afc_update                  :  8; /*!< AFC correction is done */
  uint32 enable_info                       : 16; /*!< Reserved */  

  uint32 slot1_active                      :  8; /*!< Active Slot - Slot 1 */
  uint32 slot2_active                      :  8; /*!< Active Slot - Slot 2 */
  uint32 srs1_active                       :  8; /*!< Active Slot - SRS 1 */
  uint32 srs2_active                       :  8; /*!< Active Slot - SRS 2 */

  uint32 slot1_timing                      :  32; /*!< Active Slot - Slot 1 */
  uint32 slot2_timing                      :  32; /*!< Active Slot - Slot 2 */
  uint32 srs1_timing                       :  32; /*!< Active Slot - SRS 1 */
  uint32 srs2_timing                       :  32; /*!< Active Slot - SRS 2 */
  
  uint32 tx_pll_afc_error                  : 32; /*!< tx pll afc error */

  uint32 tx_pll_afc_action_time            : 32; /*!< TxAGC Action Time */
  
  uint32 tx_pll_afc_ccs_tq_data_offset     : 32; /*!< Tx AGC CCS TQ Data Offset */


  /*********************************************************/
  /*!                TIMING LOG FOR SLOT 1                 */
  /*********************************************************/ 

  uint32 tx_on_off_event_slot1                   : 16; /*!< Tx ON or OFF Event */
  uint32 tx_en_high_low_event_slot1              : 16; /*!< Tx EN HIGH or LOW Event */  

  uint32 tx_on_off_action_time_slot1             : 32; /*!< Tx ON or OFF Action Time */

  uint32 tx_on_off_ccs_tq_data_offset_slot1      : 32; /*!< Tx ON or OFF Event CCS TQ Data Offset */

  uint32 tx_en_action_time_slot1                 : 32; /*!< Tx EN High or Low Action Time */
  
  uint32 tx_en_ccs_tq_data_offset_slot1          : 32; /*!< Tx EN High or Low Event CCS TQ Data Offset */

  uint32 tx_on_off_enqueued_slot1                :  8; /*! < TX ON OFF was enqueued to CCS */
  uint32 tx_on_off_serviced_slot1                :  8; /*! < TX ON OFF was serviced by CCS */
  uint32 tx_en_high_low_enqueued_slot1           :  8; /*! < TX EN HIGH LOW was enqueued to CCS */ 
  uint32 tx_en_high_low_serviced_slot1           :  8; /*! < TX EN HIGH LOW was serviced by CCS */  

  uint32 txagc_action_time_slot1                 : 32; /*!< TxAGC Action Time */
  
  uint32 txagc_ccs_tq_data_offset_slot1          : 32; /*!< Tx AGC CCS TQ Data Offset */

  uint32 pa_on_non_trigger_action_time_slot1     : 32; /*!< PA ON NON Trigger Action Time */
  
  uint32 pa_on_trigger_action_time_slot1         : 32; /*!< PA ON Trigger Action Time */
  
  uint32 qpoet_non_trigger_action_time_slot1     : 32; /*!< QPOET NON Trigger Action Time */
  
  uint32 qpoet_trigger_action_time_slot1         : 32; /*!< QPOET Trigger Action Time */
  
  uint32 wtr_non_trigger_action_time_slot1       : 32; /*!< WTR NON Trigger Action Time */
  
  uint32 wtr_trigger_action_time_slot1           : 32; /*!< WTR Trigger Action Time */

  uint32 wtr_wxe_non_trigger_action_time_slot1   : 32; /*!< WTR WXE NON Trigger Action Time */
  
  uint32 wtr_wxe_trigger_action_time_slot1       : 32; /*!< WTR WXE Trigger Action Time */
  
  uint32 asm_non_trigger_action_time_slot1       : 32; /*!< ASM NON Trigger Action Time */
  
  uint32 asm_trigger_action_time_slot1           : 32; /*!< ASM Trigger Action Time */
    
  uint32 aux_asm_non_trigger_action_time_slot1   : 32; /*!< AUX ASM NON Trigger Action Time */
  
  uint32 aux_asm_trigger_action_time_slot1       : 32; /*!< AUX ASM Trigger Action Time */

  uint32 fbrx_non_trigger_action_time_slot1      : 32; /*!< FBRx NON Trigger Action Time */
  
  uint32 fbrx_trigger_action_time_slot1          : 32; /*!< FBRx Trigger Action Time */

  uint32 ant_swap_tx0_low_action_time_slot1      : 32; /*!< ANT SWAP Tx 0 Low Action Time */

  uint32 ant_swap_tx0_high_action_time_slot1     : 32; /*!< ANT SWAP Tx 0 High Action Time */

  uint32 ant_swap_tx1_low_action_time_slot1      : 32; /*!< ANT SWAP Tx 1 Low Action Time */

  uint32 ant_swap_tx1_high_action_time_slot1     : 32; /*!< ANT SWAP Tx 1 High Action Time */
  

  /*********************************************************/
  /*!                TIMING LOG FOR SLOT 2                 */
  /*********************************************************/ 

  uint32 tx_on_off_event_slot2                   : 16; /*!< Tx ON or OFF Event */
  uint32 tx_en_high_low_event_slot2              : 16; /*!< Tx EN HIGH or LOW Event */  

  uint32 tx_on_off_action_time_slot2             : 32; /*!< Tx ON or OFF Action Time */

  uint32 tx_on_off_ccs_tq_data_offset_slot2      : 32; /*!< Tx ON or OFF Event CCS TQ Data Offset */

  uint32 tx_en_action_time_slot2                 : 32; /*!< Tx EN High or Low Action Time */
  
  uint32 tx_en_ccs_tq_data_offset_slot2          : 32; /*!< Tx EN High or Low Event CCS TQ Data Offset */

  uint32 tx_on_off_enqueued_slot2                :  8; /*! < TX ON OFF was enqueued to CCS */
  uint32 tx_on_off_serviced_slot2                :  8; /*! < TX ON OFF was serviced by CCS */
  uint32 tx_en_high_low_enqueued_slot2           :  8; /*! < TX EN HIGH LOW was enqueued to CCS */ 
  uint32 tx_en_high_low_serviced_slot2           :  8; /*! < TX EN HIGH LOW was serviced by CCS */  
  
  uint32 txagc_action_time_slot2                 : 32; /*!< TxAGC Action Time */
  
  uint32 txagc_ccs_tq_data_offset_slot2          : 32; /*!< Tx AGC CCS TQ Data Offset */

  uint32 pa_on_non_trigger_action_time_slot2     : 32; /*!< PA ON NON Trigger Action Time */
  
  uint32 pa_on_trigger_action_time_slot2         : 32; /*!< PA ON Trigger Action Time */
  
  uint32 qpoet_non_trigger_action_time_slot2     : 32; /*!< QPOET NON Trigger Action Time */
  
  uint32 qpoet_trigger_action_time_slot2         : 32; /*!< QPOET Trigger Action Time */
  
  uint32 wtr_non_trigger_action_time_slot2       : 32; /*!< WTR NON Trigger Action Time */
  
  uint32 wtr_trigger_action_time_slot2           : 32; /*!< WTR Trigger Action Time */

  uint32 wtr_wxe_non_trigger_action_time_slot2   : 32; /*!< WTR WXE NON Trigger Action Time */
  
  uint32 wtr_wxe_trigger_action_time_slot2       : 32; /*!< WTR WXE Trigger Action Time */
  
  uint32 asm_non_trigger_action_time_slot2       : 32; /*!< ASM NON Trigger Action Time */
  
  uint32 asm_trigger_action_time_slot2           : 32; /*!< ASM Trigger Action Time */
    
  uint32 aux_asm_non_trigger_action_time_slot2   : 32; /*!< AUX ASM NON Trigger Action Time */
  
  uint32 aux_asm_trigger_action_time_slot2       : 32; /*!< AUX ASM Trigger Action Time */

  uint32 fbrx_non_trigger_action_time_slot2      : 32; /*!< FBRx NON Trigger Action Time */
  
  uint32 fbrx_trigger_action_time_slot2          : 32; /*!< FBRx Trigger Action Time */

  uint32 ant_swap_tx0_low_action_time_slot2      : 32; /*!< ANT SWAP Tx 0 Low Action Time */

  uint32 ant_swap_tx0_high_action_time_slot2     : 32; /*!< ANT SWAP Tx 0 High Action Time */

  uint32 ant_swap_tx1_low_action_time_slot2      : 32; /*!< ANT SWAP Tx 1 Low Action Time */

  uint32 ant_swap_tx1_high_action_time_slot2     : 32; /*!< ANT SWAP Tx 1 High Action Time */
  

  /*********************************************************/
  /*!                 TIMING LOG FOR SRS 1                 */
  /*********************************************************/ 

  uint32 tx_on_off_event_srs1                   : 16; /*!< Tx ON or OFF Event */
  uint32 tx_en_high_low_event_srs1              : 16; /*!< Tx EN HIGH or LOW Event */  

  uint32 tx_on_off_action_time_srs1             : 32; /*!< Tx ON or OFF Action Time */

  uint32 tx_on_off_ccs_tq_data_offset_srs1      : 32; /*!< Tx ON or OFF Event CCS TQ Data Offset */

  uint32 tx_en_action_time_srs1                 : 32; /*!< Tx EN High or Low Action Time */
  
  uint32 tx_en_ccs_tq_data_offset_srs1          : 32; /*!< Tx EN High or Low Event CCS TQ Data Offset */

  uint32 tx_on_off_enqueued_srs1                :  8; /*! < TX ON OFF was enqueued to CCS */
  uint32 tx_on_off_serviced_srs1                :  8; /*! < TX ON OFF was serviced by CCS */
  uint32 tx_en_high_low_enqueued_srs1           :  8; /*! < TX EN HIGH LOW was enqueued to CCS */ 
  uint32 tx_en_high_low_serviced_srs1           :  8; /*! < TX EN HIGH LOW was serviced by CCS */  

  uint32 txagc_action_time_srs1                 : 32; /*!< TxAGC Action Time */
  
  uint32 txagc_ccs_tq_data_offset_srs1          : 32; /*!< Tx AGC CCS TQ Data Offset */

  uint32 pa_on_non_trigger_action_time_srs1     : 32; /*!< PA ON NON Trigger Action Time */
  
  uint32 pa_on_trigger_action_time_srs1         : 32; /*!< PA ON Trigger Action Time */
  
  uint32 qpoet_non_trigger_action_time_srs1     : 32; /*!< QPOET NON Trigger Action Time */
  
  uint32 qpoet_trigger_action_time_srs1         : 32; /*!< QPOET Trigger Action Time */
  
  uint32 wtr_non_trigger_action_time_srs1       : 32; /*!< WTR NON Trigger Action Time */
  
  uint32 wtr_trigger_action_time_srs1           : 32; /*!< WTR Trigger Action Time */

  uint32 wtr_wxe_non_trigger_action_time_srs1   : 32; /*!< WTR WXE NON Trigger Action Time */
  
  uint32 wtr_wxe_trigger_action_time_srs1       : 32; /*!< WTR WXE Trigger Action Time */
  
  uint32 asm_non_trigger_action_time_srs1       : 32; /*!< ASM NON Trigger Action Time */
  
  uint32 asm_trigger_action_time_srs1           : 32; /*!< ASM Trigger Action Time */
    
  uint32 aux_asm_non_trigger_action_time_srs1   : 32; /*!< AUX ASM NON Trigger Action Time */
  
  uint32 aux_asm_trigger_action_time_srs1       : 32; /*!< AUX ASM Trigger Action Time */

  uint32 fbrx_non_trigger_action_time_srs1      : 32; /*!< FBRx NON Trigger Action Time */
  
  uint32 fbrx_trigger_action_time_srs1          : 32; /*!< FBRx Trigger Action Time */

  uint32 ant_swap_tx0_low_action_time_srs1      : 32; /*!< ANT SWAP Tx 0 Low Action Time */

  uint32 ant_swap_tx0_high_action_time_srs1     : 32; /*!< ANT SWAP Tx 0 High Action Time */

  uint32 ant_swap_tx1_low_action_time_srs1      : 32; /*!< ANT SWAP Tx 1 Low Action Time */

  uint32 ant_swap_tx1_high_action_time_srs1     : 32; /*!< ANT SWAP Tx 1 High Action Time */

  /*********************************************************/
  /*!                 TIMING LOG FOR SRS 2                 */
  /*********************************************************/ 

  uint32 tx_on_off_event_srs2                   : 16; /*!< Tx ON or OFF Event */
  uint32 tx_en_high_low_event_srs2              : 16; /*!< Tx EN HIGH or LOW Event */  

  uint32 tx_on_off_action_time_srs2             : 32; /*!< Tx ON or OFF Action Time */

  uint32 tx_on_off_ccs_tq_data_offset_srs2      : 32; /*!< Tx ON or OFF Event CCS TQ Data Offset */

  uint32 tx_en_action_time_srs2                 : 32; /*!< Tx EN High or Low Action Time */
  
  uint32 tx_en_ccs_tq_data_offset_srs2          : 32; /*!< Tx EN High or Low Event CCS TQ Data Offset */

  uint32 tx_on_off_enqueued_srs2                :  8; /*! < TX ON OFF was enqueued to CCS */
  uint32 tx_on_off_serviced_srs2                :  8; /*! < TX ON OFF was serviced by CCS */
  uint32 tx_en_high_low_enqueued_srs2           :  8; /*! < TX EN HIGH LOW was enqueued to CCS */ 
  uint32 tx_en_high_low_serviced_srs2           :  8; /*! < TX EN HIGH LOW was serviced by CCS */  

  uint32 txagc_action_time_srs2                 : 32; /*!< TxAGC Action Time */
  
  uint32 txagc_ccs_tq_data_offset_srs2          : 32; /*!< Tx AGC CCS TQ Data Offset */

  uint32 pa_on_non_trigger_action_time_srs2     : 32; /*!< PA ON NON Trigger Action Time */
  
  uint32 pa_on_trigger_action_time_srs2         : 32; /*!< PA ON Trigger Action Time */
  
  uint32 qpoet_non_trigger_action_time_srs2     : 32; /*!< QPOET NON Trigger Action Time */
  
  uint32 qpoet_trigger_action_time_srs2         : 32; /*!< QPOET Trigger Action Time */
  
  uint32 wtr_non_trigger_action_time_srs2       : 32; /*!< WTR NON Trigger Action Time */
  
  uint32 wtr_trigger_action_time_srs2           : 32; /*!< WTR Trigger Action Time */

  uint32 wtr_wxe_non_trigger_action_time_srs2   : 32; /*!< WTR WXE NON Trigger Action Time */
  
  uint32 wtr_wxe_trigger_action_time_srs2       : 32; /*!< WTR WXE Trigger Action Time */
  
  uint32 asm_non_trigger_action_time_srs2       : 32; /*!< ASM NON Trigger Action Time */
  
  uint32 asm_trigger_action_time_srs2           : 32; /*!< ASM Trigger Action Time */
    
  uint32 aux_asm_non_trigger_action_time_srs2   : 32; /*!< AUX ASM NON Trigger Action Time */
  
  uint32 aux_asm_trigger_action_time_srs2       : 32; /*!< AUX ASM Trigger Action Time */

  uint32 fbrx_non_trigger_action_time_srs2      : 32; /*!< FBRx NON Trigger Action Time */
  
  uint32 fbrx_trigger_action_time_srs2          : 32; /*!< FBRx Trigger Action Time */

  uint32 ant_swap_tx0_low_action_time_srs2      : 32; /*!< ANT SWAP Tx 0 Low Action Time */

  uint32 ant_swap_tx0_high_action_time_srs2     : 32; /*!< ANT SWAP Tx 0 High Action Time */

  uint32 ant_swap_tx1_low_action_time_srs2      : 32; /*!< ANT SWAP Tx 1 Low Action Time */

  uint32 ant_swap_tx1_high_action_time_srs2     : 32; /*!< ANT SWAP Tx 1 High Action Time */
  

}rflm_lte_txagc_internal_log_packet_t;

/*----------------------------------------------------------------------------*/

/*==============================================================================

                    EXTERNAL FUNCTION PROTOTYPES

==============================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==============================================================================
              API input/output function definitions -- TXAGC 
==============================================================================*/

/************************************************************/
/*           DIAG LOG SUBPACKET API DEFINITIONS             */
/************************************************************/

/*----------------------------------------------------------------------------*/
rflm_lte_log_err_t
rflm_lte_external_txagc_diag_log_subpacket
(
  uint8                        context_id,
  uint32                       log_index,
  uint8                        carrier_idx
);

/*----------------------------------------------------------------------------*/
rflm_lte_log_err_t
rflm_lte_internal_txagc_diag_log_subpacket
(
  uint8                            context_id,
  uint32                           log_index,
  uint8                            carrier_idx
);

/*----------------------------------------------------------------------------*/



#ifdef __cplusplus
} // extern "C"
#endif

#endif /* RFLM_LTE_LOG_H */
