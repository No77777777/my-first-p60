/*!
  @file
  rflm_ccs_rf_rffe_log.h

  @brief
  RF RFEE logging 

  @detail
  Provides API to program RF scripts during based on events
  */
/*============================================================================== 

  Copyright (c) 2018 Qualcomm Technologies Incorporated. All Rights Reserved 

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

  $Header: //components/rel/rflm.mpss/2.11/api/ccs/rflm_qdss_iq_log.h#1 $ 
  $Date: 2021/03/09 $   
  when       who     what, where, why 
  --------   ---    ------------------------------------------------------------- 
  09/24/19   ahm     initial version


  ==============================================================================*/

#ifndef RFLM_QDSS_IQ_LOG_H
#define RFLM_QDSS_IQ_LOG_H

/*===========================================================================

  INCLUDE FILES

  ===========================================================================*/

#include "rflm_hwintf.h"



/*===========================================================================

  INTERNAL DEFINITIONS AND TYPES

  ===========================================================================*/

#ifdef __cplusplus
extern "C" 
{
#endif



#define RFLM_QDSS_IQ_BUF_SIZE_WORDS   0x80000 // 2.0MB (number in words)
#define NUM_WORDS_TO_BYTES 4

#define RFLM_QDSS_LANE_MAX 4

#define RFLM_QDSS_LANE0_MASK 1<<0
#define RFLM_QDSS_LANE1_MASK 1<<1
#define RFLM_QDSS_LANE2_MASK 1<<2
#define RFLM_QDSS_LANE3_MASK 1<<3

#define RFLM_QDSS_RXFE_SEL0_SHFT 0
#define RFLM_QDSS_RXFE_SEL1_SHFT 4
#define RFLM_QDSS_RXFE_SEL2_SHFT 8
#define RFLM_QDSS_RXFE_SEL3_SHFT 12


#define QDSS_LOCKACCESS 0xC5ACCE55
#define IQ_LOG_SAMP_CNT_MAX 1024
#define RFLM_QDSS_NB_CFG_MASK RFLM_QDSS_LANE0_MASK|RFLM_QDSS_LANE1_MASK|RFLM_QDSS_LANE2_MASK|RFLM_QDSS_LANE3_MASK


typedef enum
{
  RFLM_QDSS_IQ_NB0=0,
  RFLM_QDSS_IQ_NB1,
  RFLM_QDSS_IQ_NB2,
  RFLM_QDSS_IQ_NB3,
  RFLM_QDSS_IQ_NB4,
  RFLM_QDSS_IQ_NB5,
  RFLM_QDSS_IQ_NB_MAX=RFLM_QDSS_IQ_NB5,
  RFLM_QDSS_IQ_DIS=0xFF
}rflm_qdss_nb_chain_enum_t;

typedef enum
{
  RFLM_QDSS_IQ_LOG_LANE0=0,
  RFLM_QDSS_IQ_LOG_LANE1,
  RFLM_QDSS_IQ_LOG_LANE2,
  RFLM_QDSS_IQ_LOG_LANE3,
  RFLM_QDSS_IQ_LOG_LANE_MAX=RFLM_QDSS_IQ_LOG_LANE3
}rflm_qdss_iq_lane_enum_t;


typedef enum
{
  RFLM_QDSS_IQ_LOGGING_UNINIT              = 0,
  RFLM_QDSS_IQ_LOGGING_PAUSED              = 1,   /*!< Disables IQ logging starightaway , when client triggers stop via FTM command or EFS */
  RFLM_QDSS_IQ_LOGGING_ENABLED             = 2,   /*!< Enables IQ logging starightaway , when client triggers start via FTM command or EFS */
}rflm_qdss_iq_logging_state_enum_t;

typedef struct
{
  rflm_qdss_nb_chain_enum_t rxfe_nb[RFLM_QDSS_LANE_MAX];
}rflm_qdss_iq_log_chain_external_t;


typedef struct
{
  uint32* log_buf_ddr;
  uint32 log_buf_phy;
  uint32 log_size;
}rflm_qdss_iq_log_buffer_t;

/*! @brief  QDSS IQ logging fields*/
typedef struct
{
  uint32 rxfe_nb_sel;      /* NB selection */
  uint32 rxfe_log_cfg;     /* Rxfe log config*/
} rflm_qdss_rxfe_iq_log_cfg_t;


typedef struct
{
  rflm_qdss_rxfe_iq_log_cfg_t log_cfg;
  rflm_qdss_iq_log_buffer_t log_buf_params;
  rflm_qdss_iq_logging_state_enum_t log_state;
  boolean rflm_qdss_feature_en;
}rflm_qdss_iq_log_state_t;

extern rflm_qdss_iq_log_state_t rflm_qdss_iq_log_state;
extern rflm_qdss_iq_log_chain_external_t rflm_qdss_iq_log_rxfe_info;

/*===========================================================================

FUNCTION: rflm_qdss_iq_log_rxfe_init

===========================================================================*/

boolean rflm_qdss_iq_log_rxfe_init(void);

/*===========================================================================

FUNCTION: rflm_qdss_iq_log_rxfe_start

===========================================================================*/

void rflm_qdss_iq_log_rxfe_start(void);

/*===========================================================================

FUNCTION: rflm_qdss_iq_log_rxfe_stop

===========================================================================*/

void rflm_qdss_iq_log_rxfe_stop(void);

/*===========================================================================

FUNCTION: rflm_qdss_iq_log_rxfe_deinit

===========================================================================*/

boolean rflm_qdss_iq_log_rxfe_deinit(void);

/*===========================================================================

FUNCTION: rflm_qdss_iq_log_filter_update

===========================================================================*/
void rflm_qdss_iq_log_filter_update(rflm_qdss_iq_log_chain_external_t rxfe_nb_info);

void rflm_qdss_iq_rxfe_nb_config(void);


#ifdef __cplusplus
}
#endif


#endif  // RFLM_CCS_RF_RFFE_LOG_H
