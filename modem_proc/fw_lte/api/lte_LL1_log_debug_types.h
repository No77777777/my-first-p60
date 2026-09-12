/*!
  @file   for_apex.h

  @brief  Message Router structure to be imported by APEX

  @detail
  Defines Message Router related types and interfaces.

*/

/*===========================================================================

  Copyright (c) 2011 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary

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

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: 

when       who     what, where, why
--------   ---     ----------------------------------------------------------

===========================================================================*/
#ifndef LTE_LL1_LOG_DEBUG_TYPES_H
#define LTE_LL1_LOG_DEBUG_TYPES_H

/* debug-log id */
typedef enum
{
  LTE_LL1_LOG_DEBUG_RESERVED = 0,
  // add id's below

  LTE_LL1_LOG_DEBUG_SRCH_FFE_RESULTS,
  LTE_LL1_LOG_DEBUG_SRCH_SSS_POST_RESULTS

} lte_LL1_log_debug_id_e;

typedef struct
{
  lte_LL1_log_debug_id_e    id;       ///< which debug log
  lte_LL1_sys_time_struct   sys_time; ///< sys-time that this debug log corresponds to
  uint32                    label;    ///< to send more information about the debug log
                                      ///< eg: symbol-number
} lte_LL1_log_debug_hdr_s;


/*==========================================================================*/
/*! @brief
 * Rx ON toggle Info Log (Log code ID: 0xB136) 
 
  log_client_details array entries[0 to 5] correspond to following clients voting
  LTE_LL1_SS_TOGGLE,   
  LTE_LL1_LIGHT_SLEEP_TOGGLE,
  LTE_LL1_MS_TOGGLE,
  LTE_LL1_NB_TOGGLE,
  LTE_LL1_TDD_RISE_EDGE_TOGGLE,
  LTE_LL1_TDD_SSF_FALL_EDGE_TOGGLE
  
---------------------------------------------------------------------------*/
#define LTE_LL1_LOG_RX_ON_TOGGLE_VERSION 41
#define LTE_LL1_LOG_RX_TOGGLE_NUMBER_OF_RECORDS 5
#define LTE_LL1_LOG_RX_ON_TOGGLE_NUM_CLIENT_ARRAY 6
typedef struct
{
  uint32   action_time                  : 24;  ///< USTMR time, 0xFFFFFFFF if INVALID time
  uint32   vote                         : 2;   ///< OFF =0 , ON = 1, DONT_CARE = 2
  uint32   reserved0                    : 6;
} lte_LL1_rx_toggle_log_client_details_s;

typedef struct
{
  uint32 frame_num                        : 10; ///< Range 0..1023
  uint32 subframe_num                     : 4;  ///< Range 0..9
  uint32 rx_on                            : 1;  ///< Boolean OFF/ON
  uint32 rx_en                            : 1;  ///< Boolean DISABLE/ENABLE
  uint32 eval_sticky_vote                 : 1;  ///< Boolean OFF/ON
  uint32 rf_api_called_bmsk               : 2;  ///< 2 APIs to call. Bitmask of which are called Bit(0) RX_ON; Bit(1)RX_EN
  uint32 reserved1                        : 13; 
  uint32 rx_toggle_action_time;
  uint32 rx_toggle_fed_calling_time ;
  uint32 rx_toggle_fed_return_time ; 
  uint32 subframe_boundary_ustmr; 
  uint32 subframe_boundary_mstmr;
  uint32 rx_toggle_state0;///< Unused. place holder for any debug fields that may need to be populated
  uint32 rx_toggle_state1;///< Unused. place holder for any debug fields that may need to be populated
  uint32 rx_toggle_state2;///< Unused. place holder for any debug fields that may need to be populated

  lte_LL1_rx_toggle_log_client_details_s  lte_LL1_rx_toggle_log_client_details[LTE_LL1_LOG_RX_ON_TOGGLE_NUM_CLIENT_ARRAY];

} lte_LL1_log_rx_toggle_records_t;

typedef struct
{
  uint32 version                          : 8;  ///< Range: 0..255
  uint32 carrier                          : 4;  ///< Range 0..9
  uint32 number_of_records                : 5;  ///< Range: 1-20
  uint32 reserved2                        : 15;

  lte_LL1_log_rx_toggle_records_t  lte_LL1_log_rx_toggle_records[LTE_LL1_LOG_RX_TOGGLE_NUMBER_OF_RECORDS];

}lte_LL1_log_rx_toggle_ind_struct;

#endif
