/*!
  @file
  gfw_qsh_ext.h

  @brief
  QSH mini dump structure for gfw

*/

/*===========================================================================

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

===========================================================================*/
/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //source/qcom/qct/modem/fw/components/fw_geran.mpss/rel/17.0/api/gfw_qsh_ext.h#1 $
===========================================================================*/

#ifndef GFW_QSH_EXT_H
#define GFW_QSH_EXT_H

#include "vstmr_geran.h"

/*===========================================================================
                        MACROS/ENUM DEFINITION
===========================================================================*/

/*!  GFW QSH dump Major version */
#define GFW_QSH_MAJOR_VER  1

/*!  GFW QSH dump Mini version */
#define GFW_QSH_MINOR_VER  0


/*!  Structure and variables for logging transitions */
#define GFW_QSH_NUM_STATE_TRANSITIONS_LOGGED        (16)

/*!  Maximun number of GSTMR interupts */
#define GFW_DUMP_GSTMR_NUM_USER_INTS 10

/*!  Maximun number of DL Timeslots suppported */
#define MDSP_DUMP_MAX_DL_TS 5

/*!  Maximun number of monitors */
#define MDSP_DUMP_MAX_STANDALONE_RSSI_MEASUREMENTS 12

/*!  Type of events that the state machine can process. */
typedef enum
{
    GFW_QSH_SM_EVENT_APP =0,
    GFW_QSH_SM_EVENT_SET_APP_MODE,
    GFW_QSH_SM_EVENT_ASYNC_CMD,
    GFW_QSH_SM_EVENT_X2G,
    GFW_QSH_SM_EVENT_G2X
} gfw_qsh_dump_state_machine_event_e;

typedef enum
{
    GFW_QSH_SM_ASYNC_SLEEP,
    GFW_QSH_SM_ASYNC_WAKE,
    GFW_QSH_SM_ASYNC_SLEEP_QUERY,
    GFW_QSH_SM_ASYNC_ENTER_MODE
} gfw_qsh_dump_state_machine_async_e;

typedef enum 
{
  GFW_QSH_APP_MODE_ACTIVE,
  GFW_QSH_APP_MODE_MEASUREMENT,
  GFW_QSH_APP_MODE_IDLE,
  GFW_QSH_APP_MODE_MEASUREMENT_ACTIVE
} gfw_qsh_dump_app_mode_e;

/*!  internal fw state enum */
typedef enum
{
    GFW_QSH_STATE_DISABLED = 0,
    GFW_QSH_STATE_IDLE = 1,
    GFW_QSH_STATE_MEAS_SLEEP = 2,
    GFW_QSH_STATE_MEAS_IDLE = 3,
    GFW_QSH_STATE_MEAS_ACTIVE = 4,
    GFW_QSH_STATE_ACTIVE = 5,
    GFW_QSH_STATE_ACTIVE_SLEEP = 6,
    GFW_QSH_STATE_ACTIVE_GAP = 7,
    GFW_QSH_STATE_ACTIVE_AWAKE = 8,
    GFW_QSH_STATE_DELIMITER = 9,
    GFW_QSH_STATE_INVALID = -1,
} gfw_qsh_dump_internal_state_e;

typedef enum 
{
    GFW_QSH_MS_CLASS_12 = 0,
    GFW_QSH_MS_CLASS_33
} gfw_qsh_dump_multislot_class_e;

/*===========================================================================
                   STRUCTURE DEFINITIONS
===========================================================================*/

/**** Stability information structure defintions ****/ 

/*!  fw state machine event type */
typedef struct
{
    gfw_qsh_dump_state_machine_event_e eventType;
    union
    {
      boolean enable;
      boolean start;
      gfw_qsh_dump_app_mode_e mode;
      gfw_qsh_dump_state_machine_async_e asyncCmd;
    };
    void*                              data;

} gfw_qsh_dump_state_machine_event_t;  
  
/*!  fw state log*/
typedef struct
{
    gfw_qsh_dump_internal_state_e oldState;
    gfw_qsh_dump_internal_state_e newState;
    gfw_qsh_dump_state_machine_event_t event;
    uint16 frameNumber;
} gfw_qsh_dump_state_log_t;

/*!  shared variables*/
typedef struct
{
  uint32 gfwDebug;
  uint32 gfwDiag1;
  uint32 gfwDiag2;
  uint32 gfwDiag3;
  uint32 gfwDiag4;
  uint16 vamosSupport;
  uint16 x2gRxfeConfigAlpha;
  uint16 x2gStartAlpha;
  boolean useIdleModeAlpha;
}gfw_qsh_dump_state_rx_shared_variable_t;

/*!  shared variables*/
typedef struct
{
  uint8 cOverIthresh;
  uint16 cOverISigma;
  boolean aeqEnableSchFlag;
  uint8 cipherMode;
  uint32 cipherKey[4];
  uint8  mediaAccMode;
  boolean edgeModeFlag;
  boolean doIRFlag;
  uint16 tfiForIR;
  uint16 sbdTmsi[2];
  uint16 sbdPtmsi[2];
  uint16 sbdImsi[5];
  gfw_qsh_dump_multislot_class_e msc;

} gfw_qsh_dump_edge_shared_variable_t;

/*!  shared variable structure */
typedef struct
{
  gfw_qsh_dump_state_rx_shared_variable_t     rx;
  gfw_qsh_dump_edge_shared_variable_t   edge;
}gfw_qsh_dump_shared_vars_t;

/*!  gstmr driver structure */
typedef struct
{
  uint8 gstmrId;
  uint32 gstmrIrq;
  boolean deregisterIntPending;
  vstmr_geran_view_s *gTimeView;
  vstmr_geran_event_tbl_s gEventTbl;
  uint32 gstmrInitDone;
  void *gfwGstmrMsgQueDataPointerBuf[GFW_DUMP_GSTMR_NUM_USER_INTS];
  uint32 gfwGstmrMsgIdBuf[GFW_DUMP_GSTMR_NUM_USER_INTS];
  uint32 gfwFreeIntMask;
} gfw_qsh_dump_gstmr_dump_t;

typedef enum
{
  RFLM_GSM_QSH_DEFAULT_MOD,
  RFLM_GSM_QSH_UNKNOWN_MOD,
  RFLM_GSM_QSH_MOD_GMSK,
  RFLM_GSM_QSH_MOD_8PSK,
  RFLM_GSM_QSH_MAX_MODS
} gfw_qsh_gsm_mod_type;

typedef enum
{
  RFLM_GSM_QSH_ON_RAMP_OFF_RAMP,
  RFLM_GSM_QSH_OFF_RAMP,
  RFLM_GSM_QSH_ON_RAMP_NO_RAMP,
  RFLM_GSM_QSH_NO_RAMP,
  RFLM_GSM_QSH_KV_RAMP,
  RFLM_GSM_QSH_OFF_RAMP_ON_RAMP,
  RFLM_GSM_QSH_MAX_RAMP_TYPES
} gfw_qsh_gsm_ramp_type;

typedef struct
{
  boolean active;
  gfw_qsh_gsm_mod_type modulation;
  gfw_qsh_gsm_ramp_type ramp_type;
  uint32 transition_length;
} gfw_qsh_tx_slot_params;

/*!  tx param structure */
typedef struct
{
  boolean frame_id;
  uint8 config;
  gfw_qsh_tx_slot_params   tx_slot[5]; 
}gfw_qsh_dump_txFrame_t;

typedef struct
{
  uint32 chain;
  uint32 bufIndex;
  uint8 channel_id;
}gfw_qsh_lm_t;

/*!  burst scheduler param structure */
typedef struct
{
   uint8 bstType1;
   uint32 pRfBuf;
   uint32 pBstParamBuf;
   boolean bst1Scheduled;
   uint8 contBursts; 
   boolean bDmaReceived;
   uint32 uniqueBurstID;	
} gfw_qsh_dump_burstSched_t;

/*!  QSH stability mini dump structure */
typedef struct {
   gfw_qsh_dump_state_log_t gfwStateLog[GFW_QSH_NUM_STATE_TRANSITIONS_LOGGED];   //            256
   uint32 gfwSleepState; //4
   gfw_qsh_dump_shared_vars_t gfwSharedVars; //84
   uint16 rxAlpha; //2
   uint32 txAlpha; //4
   gfw_qsh_dump_gstmr_dump_t gstmrDump;//244
   uint32 txFSM; //4
   gfw_qsh_dump_txFrame_t txFrame; //60
   gfw_qsh_lm_t gfwTxlm; //10
   gfw_qsh_lm_t gfwPriRxlm; //10
   gfw_qsh_lm_t gfwSecRxlm; //10
   gfw_qsh_dump_burstSched_t burstSched[24]; //480
	
} gfw_qsh_dump_stability_t;    //   Total      1170


/*!  QSH mini dump structure */
typedef struct {
   gfw_qsh_dump_stability_t stabilityLog; //1170
} gfw_qsh_dump_s; 
                                                                       

/***************************************************************************/
/*                   TLV structure                                         */
/***************************************************************************/

/*! GFW QSH dump enum */
typedef enum
{
  GFW_QSH_DUMP_TAG_0 = 0,
  GFW_QSH_DUMP_TAG_1
} gfw_qsh_dump_tag_e;

#endif // GFW_QSH_EXT_H


