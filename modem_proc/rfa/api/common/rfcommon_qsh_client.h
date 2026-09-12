#ifndef RFCOMMON_QSH_CLIENT_H
#define RFCOMMON_QSH_CLIENT_H

/*!
  @file
  rfcommon_qsh_client.h

  @brief
  RFA Common client for QSH
*/

/*==============================================================================

  Copyright (c) 2020 Qualcomm Technologies Incorporated. All Rights Reserved

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

  $Header: //components/rel/rfa.mpss/4.11/api/common/rfcommon_qsh_client.h#1 $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
01/20/20   moh     QSH MDUMP support 
01/16/20   moh     Initial Release - RF Client for QSH 

==============================================================================*/
#ifdef FEATURE_QSH_MDUMP
#include "comdef.h"
#include "rfm_umid_list.h"
#include "rf_apps_task.h"
#include "rfcommon_msg.h"
#include "rfm_mode_types.h"
#include "msg.h"

#define QSH_CLT_RF_MAJOR_VER                          1
#define QSH_CLT_RF_MINOR_VER                          0
#define RF_MAX_SUBS                                   2
#define METRIC_RF_TX_INFO_ROW_SAMPLING_PERIOD_MS      200
#define METRIC_RF_TX_INFO_ROW_ELEMENT_COUNT           5

#define METRIC_RF_ATUNER_TX_INFO_ROW_SAMPLING_PERIOD_MS  100
#define METRIC_RF_ATUNER_TX_INFO_ROW_ELEMENT_COUNT       5

#ifdef __cplusplus
extern "C"
{
#endif

/*----------------------------------------------------------------------------*/
//boolean rf_qsh_client_register(sys_modem_as_id_e_type subs_id);
void rf_qsh_client_register(void); 

typedef void (*rftech_qsh_mdump_fp_type)( void );
boolean rf_qsh_cb_register(rfcom_mode_enum_type tech, rftech_qsh_mdump_fp_type func_ptr);
void rf_qsh_client_deregister();

void rf_qsh_callback_ind_handler(void *cmd_ptr,
                                 rf_dispatch_cid_info_type *cid_info,
                                 void *cb_data);


#ifdef __cplusplus
}
#endif

#endif // FEATURE_QSH_MDUMP


#endif /* RFCOMMON_QSH_CLIENT_H */



