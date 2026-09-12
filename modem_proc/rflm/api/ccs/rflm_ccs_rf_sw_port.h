/*!
  @file
  rflm_ccs_rf_sw_port.h

  @brief
  RFC (CCS) Utility Driver.

*/

/*===========================================================================

  Copyright (c) 2015 Qualcomm Technologies, Incorporated. 
  All rights reserved.
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
/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rflm.mpss/2.11/api/ccs/rflm_ccs_rf_sw_port.h#1 $
$Date: 2021/03/09 $
when         who     what, where, why
--------   ---     -----------------------------------------------------------
10/15/15   sbo     FW compilation Fix
10/15/15   sbo     Initial version  TA RF SW
==============================================================================*/

#ifndef RFLM_CCS_RF_SW_PORT_H
#define RFLM_CCS_RF_SW_PORT_H

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include "rflm.h"
#include "rflm_ccs_rf_event_intf.h"
#include "rflm_ccs_rf_intf.h"




/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/


#define SW_PORT_SYNC_SIG_MASK 0x1 

#define SW_PORT_ACT_CMD_MAX_CNT 8

/*! @brief Structure for dedicated tast queue.
*/
typedef struct {

  rflm_ccs_rf_tq_handle_t *tq_handle;
  uint32 *tq_data_ptr;
//  uint32 pdmem_offset;
  uint16 tq_data_bytes;
  uint8  tq_index;
  uint32 active_cmd_cnt;
  qurt_signal_t  sw_port_sig;
  rflm_ccs_rf_event_header_task_type  global_header_memory_buffer[SW_PORT_ACT_CMD_MAX_CNT];

} rflm_ccs_rf_dedicated_tq_s;


/*===========================================================================

                    EXTERNAL FUNCTION PROTOTYPES

===========================================================================*/

/*----------------------------------------------------------------------------*/
void rflm_ccs_request_dedicated_tq( uint32 tq_index );
/*----------------------------------------------------------------------------*/
void rflm_ccs_release_dedicated_tq( void );
/*----------------------------------------------------------------------------*/
rflm_ccs_rf_event_header_task_type* 
    rflm_ccs_request_dedicated_tq_free_global_header_buffer(unsigned int *sig_mask);
/*----------------------------------------------------------------------------*/
void rflm_ccs_dedicated_tq_clear_sig(unsigned int sig_mask);
/*----------------------------------------------------------------------------*/
void rflm_ccs_dedicated_tq_set_sig(unsigned int sig_mask);
/*----------------------------------------------------------------------------*/
void rflm_ccs_dedicated_tq_wait_all_sig(unsigned int sig_mask);
/*----------------------------------------------------------------------------*/
void rflm_ccs_dedicated_tq_event_done_wait();
/*----------------------------------------------------------------------------*/
void rflm_ccs_dedicated_tq_check_available_mem(void);
/*----------------------------------------------------------------------------*/
void rflm_ccs_event_done_callback_sw_port (
  /*!< RF handle */
  rflm_ccs_rf_tq_handle_t *tq_handle_p,
  /*!< Task index giving the interrupt */
  uint32 task_idx,
  /*!< Status of task */
  ccs_rf_task_status_t status,
  /*!< Other arguments */
  void *arg);

/*----------------------------------------------------------------------------*/
boolean
rflm_ccs_send_common_rf_event
( 
   rflm_ccs_rf_event_header_task_type * cmd,
   unsigned int   sig_mask
);

#endif