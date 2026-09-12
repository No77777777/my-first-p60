#ifndef RFWCDMA_CORE_LOG_H
#define RFWCDMA_CORE_LOG_H
/*!
  @file
  rfwcdma_core_log.h

  @brief
  This file contains all declarations and definitions pertaining to WCDMA
  RF logging
*/

/*==============================================================================

  Copyright (c) 2013 Qualcomm Technologies, Inc. All Rights Reserved

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

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/rftech_wcdma.mpss/3.11/rf/core/inc/rfwcdma_core_log.h#1 $

when       who     what, where, why
--------   ---     ------------------------------------------------------------- 
10/06/15   ac/rmb  AGC logging change
08/26/15   ac     backingout the agc logging change for W plus W, will re-submit along with WL1 code
08/18/15   ac      agc logging changes for w plus w
05/21/15   sd      Dispatch clk voting in Rf task only when enabled
05/01/14   rmb     Add flush API for the new logging 0x4176 version 2.
07/30/13   aro     Added function to log AGC once
07/24/13   aro     Added initial framework to support AGC logging for WCDMA
07/24/13   aro     Initial version

==============================================================================*/

#include "comdef.h"
#include "rf_dispatch.h"
#include "rflm_wcdma_msg.h"
#include "rf_cmd_interface.h"
#include "rfm_wcdma_param_types.h"
#include "sys.h"

#ifdef __cplusplus
extern "C" {
#endif


/*----------------------------------------------------------------------------*/
boolean 
rfwcdma_log_init
(
  void
);
/*----------------------------------------------------------------------------*/
boolean
rfwcdma_core_log_agc_data
( 
  rfm_wcdma_log_agc_handle_type log_handle,rfm_wcdma_log_agc_param_type *intf_param  
);

/*----------------------------------------------------------------------------*/
boolean 
rfwcdma_core_log_agc_data_flush
( 
  rfm_wcdma_log_agc_handle_type log_handle 
);

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function is the handler for the command RFLM_WCDMA_NPA_ISSUE_CMD. 
  
  @details
  Calls the function npa_issue_required_request. 
 
*/
void
rflm_wcdma_npa_issue_cmd_handler
(
  void* cmd_ptr,
  rf_dispatch_cid_info_type *cid_info,
  void *cb_data 
);

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function is the handler for the command RFLM_WCDMA_NPA_CANCEL_CMD. 
  
  @details
  Calls the function npa_cancel_request. 
 
*/

void
rflm_wcdma_npa_cancel_cmd_handler
(
  void* cmd_ptr,
  rf_dispatch_cid_info_type *cid_info,
  void *cb_data 
);

/*--------------------------------------------------------------------------*/
rfm_wcdma_log_agc_handle_type rfwcdma_log_agc_create(sys_modem_as_id_e_type subscriber_id);
/*--------------------------------------------------------------------------*/
boolean rfwcdma_log_agc_delete (rfm_wcdma_log_agc_handle_type log_handle);
/*--------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* RFWCDMA_CORE_LOG_H */


