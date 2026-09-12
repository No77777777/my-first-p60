#ifndef RFDEVICE_QTUNER_LOG_H
#define RFDEVICE_QTUNER_LOG_H

/*!
   @file
   rfdevice_qtuner_log.h

   @brief
   This file contains headers for QTuner device level log packets.
 
   @addtogroup RF_LOG_DEFN_COMMON
   @{
*/

/*==============================================================================

  Copyright (c) 2015 Qualcomm Technologies Incorporated. All Rights Reserved

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

  $Header: //components/rel/rfdevice_qtuner.mpss/1.32/api/rfdevice_qtuner_log.h#1 $
  $DateTime: 2021/03/09 04:14:45 $
  $Author: pwbldsvc $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
06/22/15   yb      Initial version

==============================================================================*/

/*--- IMPORTANT ----*/
/* Only these base .h files may be added. Complex data structures are forbidden */
#include "log.h"
#include "rfcommon_log.h"
//#include "ftm_log.h"
/*--- IMPORTANT ----*/

#define RFDEVICE_QTUNER_LOG_MAX_REG_WRITES  16

/*INFO_DATA_C Log packet version*/
#define RFDEVICE_ATUNER_INTERNAL_DATA_C_PKT_VER0 0 

/* Enum for transaction type */
typedef enum{
  RFDEVICE_QTUNER_LOG_INIT,
  RFDEVICE_QTUNER_LOG_PROGRAM_TUNE_CODE,
  RFDEVICE_QTUNER_LOG_DISABLE,
  RFDEVICE_QTUNER_LOG_INVALID = 0xFF,
}rfdevice_qtuner_log_transaction_type;

#ifdef __cplusplus
extern "C" {
#endif

/*==========================================================================*/
/* LOG_ATUNER_INTERNAL_DATA_C   */
/*==========================================================================*/  
typedef PACK(struct)
{
  /*Log packet header, this contains information about the length of log 
  record, the log code and the timestamp associated with the packet*/ 
  log_hdr_type log_header;

  /*Version*/
  uint16 pkt_ver;
  
  /*Tuner instance Number*/
  uint8 tuner_id;

} rfdevice_qtuner_info_data_pkt_header_type;


typedef PACK(struct)
{
  uint8 reg_addr;
  uint8 reg_data;
}rfdevice_qtuner_info_data_pkt_reg_addr_data;

typedef PACK(struct)
{
  /* Number of register writes */
  uint8 num_writes;

  /* Type of transactions */
  uint8 transaction_type;

  rfdevice_qtuner_info_data_pkt_reg_addr_data reg_addr_data[RFDEVICE_QTUNER_LOG_MAX_REG_WRITES];

} rfdevice_qtuner_info_data_pkt_transactions_type;


typedef PACK(struct)
{
  /* Info Data Header */
  rfdevice_qtuner_info_data_pkt_header_type hdr_data;
  
  /*Transaction Data info*/
  rfdevice_qtuner_info_data_pkt_transactions_type transactions;

} rfcommon_qtuner_device_info_data_pkt_type;

#ifdef __cplusplus
}
#endif

/*! @} */

#endif /* RFDEVICE_QTUNER_LOG_H */
