/*!
  @file
  rflm_wcdma_ccs.h

  @brief
  rflm ccs definitions for WCDMA
  
  @detail
  Provides all teh CCS event seq create, enqueue and commit related callflow APIs:
  - memory allocation for each rf event
 
*/

/*==============================================================================

  Copyright (c) 2013 - 2016 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.

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

$Header: //components/rel/rflm.mpss/2.11/rftech/wcdma/inc/rflm_wcdma_ccs.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/20/16   ag      Increase out done time for tune away/tuneback Trigger event
06/25/16   ag      Increase Rx Tune Script size
06/07/16   ag      Use CCS static memory for ILPC on WTR2k
03/24/16   sbo     Move Txagc to Static Scripts in PDMEM.
02/16/16   sbo     RFLM CCSMMU Changes
11/18/15   ak      Changes to increase therm size per the MSS update
11/16/15   ag      Increase Rx event size and Autopin event size for TA
11/05/15   ag      CCS event size and mem alloc changes for Autopin 
06/02/15   gh/zhh  Add support for therm_enable event
09/04/15   ak      CCS memory optimization to support eLNA
08/25/15   aa      Support for stand alone FBRX gain state change 
08/14/15   vc      RFLM buffer increase to 20
07/28/15   yb      VSWR support
06/05/15   rmb     Add RFLM handle in get Tx TQ api.
06/02/15   gh/zhh  Add support for therm_enable event
06/01/15   sd      optimize CCS memory allocation to fix crash 
05/26/15   ac      optimisation of the CCS memory for events
05/18/15   ak      Merging all CCS changes
05/15/15   ak      Retaining only the memory overlay changes
04/30/15   ac      addition of 2nd buffer for tq pair
02/11/15   dw      Added device pair in the argument of ccs tq allocation
02/06/15   ska     Increased TX_ON/OFF event size to 57 words 
10/27/14   dw     Added support for rflm_wcdma_rf_send_deadline_script
10/13/14   dr      Adding TABASCO featurization
09/16/14   rmb     Increase CCS Script size for Trigger event in Tune back.
09/15/14   dw      WXE based Tx tune support for Single Tx
08/15/14   aro/gh  Add FBRX ILPC support
08/06/14   dw      Increase Tx tune event size
08/01/14   dw      Initial support for single Tx DSDA
07/08/14   kai     Add DTR events double buffers
07/07/14   ac      rf task prioritization 
07/01/14   ac      NBEE fix to add car as param,cleanup
06/25/14   aro     Convert AGC events to non issue seq to reduce CCS memory
06/12/14   zhh     Pass event size while allocate memory for CCS task
06/04/14   dw      Optimize CCS memory allocation and undo the overlay of Tx on/off events
05/15/14   aro     Updated Tx Power meter trigger event dsize
05/15/14   aro     Overlayed Tuner and power meter event memory
05/02/14    aa     Updated the FBRx CCS event size
04/18/14   rmb     Add proper Preload offset time return for home and neighbour scripts.
04/17/14   dw      Added intitial tuner closed loop support
04/02/14   aa      Added support for FBRX CCS Enable/Disable Scripts 
03/19/14   ac      drx enable/disable mem overlay change for FET,OFT warning fix
03/04/14   ac      asdiv bringup support
02/27/14   ac      task_idx wrap around crash and defensive checks
02/28/14   kg      Adding Tuner Open Loop Event Support
02/21/14   rmb     Add the rflm handle id in IRAT Global structure.
01/31/14   rmb     Added call back for enqueuing RF events in CCS.
01/30/14   bn      Two alternating buffers for TxAGC event shared with Rx tune 
01/23/14   ac      cmn TQ change
01/20/14   ac      static allocation changes
01/16/14   ac      fix for stack corruption for tx_on/off event
01/15/14   ac      tx_on/drx/dtx size,tx_on dm corruption fix
01/08/14   dw      Add mdsp/FED support for wakeup-tune config
12/10/13   ac      irat script building changes
12/04/13   vbh     Mem alloc for all the set LNA events, increase the size for ccs memory check 
11/19/13   ac      ccs buffer size check,ccs_seq fix for the issue seq,tx_on/off size
11/07/13   ac      callback for enqueue
10/24/13   bn      Adding rflm_wcdma_ccs_commit_txagc_seq fn.
10/24/13   ac      minor fix for the lna update memory corruption
10/22/13   ac      tx_on/off and meas_trigger timing def
10/15/13   ac      added struct change for seq ptr,removed exit() to sw api,
                   added mem alloc func
10/10/13   ac      the apis to be accessed by sw moved to rflm_wcdma_sw_api.h
10/07/13   ac      warning fix, tq flag for rx and tx,max size check,rf script action time 
09/26/13    ac     Initial version
==============================================================================*/

#ifndef RFLM_WCDMA_CCS_H
#define RFLM_WCDMA_CCS_H


/*==============================================================================

                           INCLUDE FILES

==============================================================================*/
#include "rflm.h"
#include "rflm_api_cmn.h"
#include "rflm_api_fbrx_fw.h"
#include "rflm_ccs_rf_intf.h"
#include "rflm_ccs_rf_event_intf.h"
#include "rflm_dm_api.h"
#include "rflm_api_wcdma.h"
#include "rflm_rxagc_api.h"
#include "rflm_cmn_intf.h"
#include "rflm_cmn_asd.h"
#include "rflm_wcdma_sw_api.h"
#include "rflm_dm_modem_defs.h"

/*==============================================================================

                   EXTERNAL DEFINITIONS AND TYPES

==============================================================================*/



/*==============================================================================
ccs tq enqueue related structure, will be removed to inc dir
==============================================================================*/

/*----------------------------------------------------------------------------*/
/*for now the max size allocation is same as what we have for dime*/

#ifdef FEATURE_RF_HAS_WTR2965
#define RFLM_WCDMA_TXAGC_SIZE 180
#define RFLM_WCDMA_RX_SET_LNA_SIZE 2
#define RFLM_WCDMA_TRIGGER_POWER_METER_SIZE 40 /* 2 */
#define RFLM_WCDMA_SET_TX_ON_SIZE 12
#define RFLM_WCDMA_SET_TX_OFF_SIZE 12
#define RFLM_WCDMA_ENABLE_DTX_SIZE 10
#define RFLM_WCDMA_DISABLE_DTX_SIZE 10
#define RFLM_WCDMA_ENABLE_DRX_SIZE 2
#define RFLM_WCDMA_DISABLE_DRX_SIZE 4
#define RFLM_WCDMA_READ_THERM_SIZE 10
#define RFLM_WCDMA_RX_TUNE_SIZE 410
#define RFLM_WCDMA_TX_TUNE_SIZE 0
#define RFLM_WCDMA_ENABLE_ASDIV_SIZE 10
#define RFLM_WCDMA_ENABLE_FBRX_SIZE 2
#define RFLM_WCDMA_DISABLE_FBRX_SIZE 2
#define RFLM_WCDMA_FBRX_ILPC_READBACK_SIZE 8
#define RFLM_WCDMA_THERM_SIZE 36
#define RFLM_WCDMA_FBRX_CONFIG_COUPLER_SIZE 8
#define RFLM_WCDMA_FBRX_GAIN_UPDATE_SIZE 2
#define RFLM_WCDMA_ENABLE_AUTOPIN_SIZE 95

#else
#define RFLM_WCDMA_TXAGC_SIZE 180
#define RFLM_WCDMA_RX_SET_LNA_SIZE 2
#define RFLM_WCDMA_TRIGGER_POWER_METER_SIZE 40 /* 2 */
#define RFLM_WCDMA_SET_TX_ON_SIZE 38
#define RFLM_WCDMA_SET_TX_OFF_SIZE 38
#define RFLM_WCDMA_ENABLE_DTX_SIZE 15
#define RFLM_WCDMA_DISABLE_DTX_SIZE 15
#define RFLM_WCDMA_ENABLE_DRX_SIZE 2
#define RFLM_WCDMA_DISABLE_DRX_SIZE 4
#define RFLM_WCDMA_READ_THERM_SIZE 10
#define RFLM_WCDMA_RX_TUNE_SIZE 400
#define RFLM_WCDMA_TX_TUNE_SIZE 0
#define RFLM_WCDMA_ENABLE_ASDIV_SIZE 30
#define RFLM_WCDMA_ENABLE_FBRX_SIZE 2
#define RFLM_WCDMA_DISABLE_FBRX_SIZE 2
#define RFLM_WCDMA_FBRX_ILPC_READBACK_SIZE 8
#define RFLM_WCDMA_THERM_SIZE 30
#define RFLM_WCDMA_FBRX_CONFIG_COUPLER_SIZE 8
#define RFLM_WCDMA_FBRX_GAIN_UPDATE_SIZE 2
#define RFLM_WCDMA_THERM_SIZE 30
#define RFLM_WCDMA_ENABLE_AUTOPIN_SIZE 0
#endif

#define RFLM_WCDMA_CCS_MAX_TASK 4

#define RFLM_WCDMA_TUNER_OPEN_LOOP_SIZE RFLM_CMN_TUNER_SCRIPT_DATA_BUF_SIZE_WORDS

/*trigger time to be 2 bpgs ahead of slot boundary ((2*66.67)* 19.2)*/
#define RFLM_WCDMA_EVENT_SET_TX_ON_OFF_TRIGGER (uint32)(2560.128)

/*((4BPG)* 19.2) including the lock time delay */
#define RFLM_WCDMA_EVENT_RX_MEAS_TRIGGER_TIME (uint32)(5120)

/* Values for Neighbour and Home Preload offset time
   Neighbour - 1200 micro sec * 19.2 MHz = 23040 Ustmr units,    Home - 0 */
#define RFLM_WCDMA_NEIGHBOUR_PRELOAD_OFFSET_TIME 23040
#define RFLM_WCDMA_HOME_PRELOAD_OFFSET_TIME 0

#define RFLM_WCDMA_CCS_VALID_TIME_MASK (~1)&0x3FFFFF

#ifdef T_RUMI_EMULATION
#define RFLM_WCDMA_CCS_VALID_TIME_MASK_RUMI_DBG (~1)&0xFFFFFF
#define RFLM_WCDMA_CCS_RUMI_TIME_OFFSET 0xF00
#endif

#define RFLM_WCDMA_EVENT_RX_MEAS_PRELOAD_TIME (uint32)(76.8)

/*(5us* 19.2) asd delay */
#define RFLM_WCDMA_ASD_TIME_DELAY_USTMR                                 96

/* 25us FBRx measurement time */
#define RFLM_WCDMA_FBRX_MEAS_TIME                                       480

/* WCDMA event tags for dealine based send script API loggin */
#define RFLM_WCDMA_CCS_EVENT_TAG0                                0x12344321

#define RFLM_WCDMA_CCS_EVENT_TAG1                                0xABCDDBCA

#define RFLM_WCDMA_PHASE_COMP_MASK                                0xFFFF

#define RFLM_WCDMA_CCS_DEVICE_PAIR_INDEX                          2

typedef enum
{
  RFLM_WCDMA_VSWR_COUPLER_DIR_FWD = 0, /* FORWARD DIRECTION */
  RFLM_WCDMA_VSWR_COUPLER_DIR_REV, /* REVERSE DIRECTION */
  RFLM_WCDMA_VSWR_COUPLER_DIR_MAX
}rflm_wcdma_vswr_coupler_direction_type;

/*----------------------------------------------------------------------------*/
/*! @detail
  wcdma CCS memory allocation for the issue seq events.
*/

typedef union
{
  struct
  {
        uint32 rf_wcdma_event_enable_drx_lna_2[RFLM_WCDMA_ENABLE_DRX_SIZE];
        uint32 rf_wcdma_event_enable_drx_lna_3[RFLM_WCDMA_ENABLE_DRX_SIZE];
      };

      struct
      {
        uint32 rf_wcdma_event_disable_drx_lna_2[RFLM_WCDMA_DISABLE_DRX_SIZE];
        uint32 rf_wcdma_event_disable_drx_lna_3[RFLM_WCDMA_DISABLE_DRX_SIZE];
      };

}rflm_wcdma_ccs_mem_alloc_drx;

typedef union
    {
      uint32 rf_wcdma_event_enable_dtx[RFLM_WCDMA_ENABLE_DTX_SIZE];
      uint32 rf_wcdma_event_disable_dtx[RFLM_WCDMA_DISABLE_DTX_SIZE];

}rflm_wcdma_ccs_mem_alloc_dtx;

typedef struct
{
  uint32 rf_wcdma_event_set_tx_on[RFLM_WCDMA_SET_TX_ON_SIZE];
  uint32 rf_wcdma_event_set_tx_off[RFLM_WCDMA_SET_TX_OFF_SIZE];

}rflm_wcdma_ccs_mem_alloc_tx_on_off;

typedef struct
{
  uint32 rf_wcdma_event_rx_set_lna[RFLM_WCDMA_RX_SET_LNA_SIZE];
  uint32 rf_wcdma_event_rx_set_lna_1[RFLM_WCDMA_RX_SET_LNA_SIZE];
  uint32 rf_wcdma_event_rx_set_lna_2[RFLM_WCDMA_RX_SET_LNA_SIZE];
  uint32 rf_wcdma_event_rx_set_lna_3[RFLM_WCDMA_RX_SET_LNA_SIZE];
  uint32 rf_wcdma_event_enable_drx_lna_0[2][RFLM_WCDMA_ENABLE_DRX_SIZE];
  uint32 rf_wcdma_event_enable_drx_lna_1[2][RFLM_WCDMA_ENABLE_DRX_SIZE];
  uint32 rf_wcdma_event_disable_drx_lna_0[2][RFLM_WCDMA_DISABLE_DRX_SIZE];
  uint32 rf_wcdma_event_disable_drx_lna_1[2][RFLM_WCDMA_DISABLE_DRX_SIZE];
  rflm_wcdma_ccs_mem_alloc_drx drx_lna;
  rflm_wcdma_ccs_mem_alloc_dtx dtx;
  union
  {
    uint32 rf_wcdma_event_trigger_power_meter[RFLM_WCDMA_TRIGGER_POWER_METER_SIZE];
    uint32 rf_wcdma_event_enable_autopin_g1[RFLM_WCDMA_ENABLE_AUTOPIN_SIZE];
    uint32 rf_wcdma_event_enable_autopin_g2[RFLM_WCDMA_ENABLE_AUTOPIN_SIZE];
    uint32 rf_wcdma_event_tuner_open_loop[RFLM_WCDMA_TUNER_OPEN_LOOP_SIZE];
    uint32 rf_wcdma_event_config_vswr_coupler_dir[RFLM_WCDMA_VSWR_COUPLER_DIR_MAX][RFLM_WCDMA_FBRX_CONFIG_COUPLER_SIZE];
  };
    uint32 rf_wcdma_event_rx_tune[RFLM_WCDMA_RX_TUNE_SIZE];
    uint32 rf_wcdma_event_txagc[2][RFLM_WCDMA_TXAGC_SIZE];
  uint32 rf_wcdma_event_enable_asdiv[RFLM_WCDMA_ENABLE_ASDIV_SIZE];
  rflm_wcdma_ccs_mem_alloc_tx_on_off tx_on_off;  
  uint32 rf_wcdma_event_enable_fbrx[RFLM_WCDMA_ENABLE_FBRX_SIZE];
  uint32 rf_wcdma_event_disable_fbrx[RFLM_WCDMA_DISABLE_FBRX_SIZE];
  uint32 rf_wcdma_event_fbrx_gain_update[RFLM_WCDMA_FBRX_GAIN_UPDATE_SIZE];
  #if (!( defined(FEATURE_JOLOKIA_MODEM) || defined(FEATURE_TABASCO_MODEM)))
  uint32 rf_wcdma_event_tx_tune[RFLM_WCDMA_TX_TUNE_SIZE];
  #endif
  uint32 rf_wcdma_event_fbrx_ilpc_readback[RFLM_WCDMA_FBRX_ILPC_READBACK_SIZE];
  uint32 rf_wcdma_event_therm_enable[RFLM_WCDMA_THERM_SIZE];
}rflm_wcdma_ccs_mem_alloc_s;

COMPILE_ASSERT(sizeof(rflm_wcdma_ccs_mem_alloc_s)<(4*CCS_RF_TASKQ_PAIRS_DATA_BUF_NUM_WORDS));

/*----------------------------------------------------------------------------*/
/*! @brief
  wcdma ccs taskQ structure
*/
typedef struct
{
  /*! @brief CCS RF TQ  taskQ pointer */
  rflm_ccs_rf_tq_handle_t *tq_handle;

  /*! @brief CCS RF TQ data memory pointer */
  uint32 *tq_data_ptr;

  /*! @brief Length (in bytes) of the data memory */
  uint16 tq_data_bytes;

  /*! @brief TQ index corresponding to tq_handle */
  uint8 tq_index;

} rflm_wcdma_ccs_tq_t;

/*----------------------------------------------------------------------------*/
/*! @brief
  wcdma irat ccs taskQ structure
*/
typedef struct
{
  /*! @brief CCS RF TQ  taskQ pointer */
  rflm_ccs_rf_tq_handle_t *tq_handle;

  /*! @brief CCS RF TQ data memory pointer */
  uint32 *tq_data_ptr;

  /*! @brief Length (in bytes) of the data memory */
  uint16 tq_data_bytes;

  /*! @brief TQ index corresponding to tq_handle */
  uint8 tq_index;

} rflm_wcdma_irat_ccs_tq_t;

/*----------------------------------------------------------------------------*/
/*! @brief
  wcdma ccs pair taskQ structure
*/
typedef struct
{
  /*! wcdma tx taskQ */
  rflm_wcdma_ccs_tq_t tx_tq;
  /*! wcdma rx taskQ */
  rflm_wcdma_ccs_tq_t rx_tq;

  /*! Associated device with the tq pair*/
  uint8 device;

  /*add helper function to associate handle with device pair nad search through to get the correct index*/
  uint8 device_pair;

} rflm_wcdma_ccs_tq_pair_t;

/*----------------------------------------------------------------------------*/
typedef struct
{
  /*! @brief flag to keep track in init and exit */
  uint32 rflm_wcdma_task_flag;

  /*! @brief w dedicated taskQ pair , double buffer , link with device id and handle*/
  rflm_wcdma_ccs_tq_pair_t wcdma_dedicated_pair_tqs[RFLM_WCDMA_CCS_DEVICE_PAIR_INDEX];

}rflm_wcdma_data_t;

/*----------------------------------------------------------------------------*/
typedef struct
{
  uint32 ccs_mem_size_byte;
  uint32 *ccs_mem_offset;
}rflm_wcdma_ccs_rf_mem_alloc_t;

/*----------------------------------------------------------------------------*/
typedef struct
{
  rflm_wcdma_event_id_type event_id;

  rflm_ccs_rf_event_header_task_type *event_header;

  rflm_wcdma_irat_ccs_tq_t rf_wcdma_tq;

} rflm_wcdma_irat_ccs;

/*----------------------------------------------------------------------------*/
typedef struct
{
  rflm_wcdma_event_id_type event_id;

  rflm_cmn_rf_asd_event_header_task_type *asd_event_header;

  rflm_wcdma_ccs_tq_t *asd_wcdma_tq;

} rflm_wcdma_asd_ccs;


/*----------------------------------------------------------------------------*/
typedef struct
{
  rflm_handle_rx_t handle_id;

  rflm_wcdma_event_id_type event_id;

  rflm_ccs_rf_event_header_task_type *event_header;

  rflm_wcdma_ccs_rf_mem_alloc_t ccs_wcdma_mem_alloc;

  rflm_wcdma_ccs_tq_t *rf_wcdma_tq;

  rflm_ccs_rf_seq_t *issue_seq_task[RF_WCDMA_EVENT_MAX][RFLM_HANDLE_COUNTS];

  rflm_ccs_rf_seq_scratch_t scratch;

  uint8 rflm_wcdma_task_index;

  uint32 rflm_wcdma_ccs_mem_alloc_tbl[RFLM_CCS_RF_TASKQ_NUM_ITEMS];

} rflm_wcdma_ccs;

/*==============================================================================

                    EXTERNAL FUNCTION PROTOTYPES

==============================================================================*/

#ifdef __cplusplus
extern "C"
{
#endif


/*==============================================================================
  LM related API's prototypes
==============================================================================*/

/*----------------------------------------------------------------------------*/
void rflm_wcdma_ccs_allocate_static_mem_block(void);

/*----------------------------------------------------------------------------*/
void rflm_wcdma_ccs_deallocate_static_mem_block(void);

/*----------------------------------------------------------------------------*/
void rflm_wcdma_ccs_reset_state_machine(void);

/*----------------------------------------------------------------------------*/
rflm_ccs_rf_seq_t*
rflm_wcdma_event_allocate_task
(
  rflm_handle_rx_t const handle_id,
  rflm_wcdma_event_id_type event_id,
  uint16 event_max_size
  );
/*----------------------------------------------------------------------------*/
uint8
rflm_wcdma_get_lna_device_idx
(
  rflm_dm_handle_id_t handle_id
  );
/*----------------------------------------------------------------------------*/
uint8
rflm_wcdma_get_ccs_txagc_mem_block
( );

/*----------------------------------------------------------------------------*/
boolean
rflm_wcdma_get_tq_pair
(
  uint8                 device,
  rflm_tech_id_t        tech,
  boolean               is_irat,
  uint8 *tq_pair
  );
/*---------------------------------------------------------------------------*/
rflm_wcdma_ccs_tq_t*
rflm_wcdma_ccs_get_tx_tq(rflm_handle_tx_t handle_id);
/*---------------------------------------------------------------------------*/
rflm_wcdma_ccs_tq_t*
rflm_wcdma_ccs_get_rx_tq(rflm_handle_rx_t handle_id);

/*----------------------------------------------------------------------------*/
rflm_wcdma_ccs_tq_t*
rflm_wcdma_map_ccs_tq_buf_idx(uint8 device_pair_idx);

/*---------------------------------------------------------------------------*/
uint32
rflm_wcdma_get_irat_event_max_size
(
  rflm_wcdma_event_id_type event_id,
  rflm_wcdma_irat_meas_enum_type meas_scenario
  );
/*---------------------------------------------------------------------------*/
uint32
rflm_wcdma_get_event_max_size
(
  rflm_wcdma_event_id_type event_id
  );
/*---------------------------------------------------------------------------*/
void rflm_wcdma_ccs_rf_event_callback(rflm_ccs_rf_tq_handle_t *tq, uint32 task_idx, ccs_rf_task_status_t status, void *arg);
/*---------------------------------------------------------------------------*/
rflm_err_t rflm_wcdma_rf_send_irat_script(rflm_handle_rx_t handle_id,
                                          rflm_wcdma_rf_send_script_in_t  *in,
                                          rflm_wcdma_rf_send_script_out_t *out);
/*----------------------------------------------------------------------------*/
rflm_err_t rflm_wcdma_rf_send_asd_script(rflm_handle_rx_t handle_id,
                                         rflm_wcdma_rf_send_script_in_t  *in,
                                         rflm_wcdma_rf_send_script_out_t *out);
/*----------------------------------------------------------------------------*/
uint32
rflm_wcdma_ccs_commit_issue_seq
(
  uint32 script_size,
  rflm_ccs_rf_seq_t *issue_seq_ccs_ptr,
  rflm_wcdma_ccs_tq_t *rf_tq,
  uint32 *ccs_mem_offset,
  uint32 ustmr_action_time,
  uint8 priority
  );
/*----------------------------------------------------------------------------*/
uint32
rflm_wcdma_ccs_commit_txagc_seq
(
  rflm_ccs_rf_seq_t *issue_seq_ccs_ptr,
  rflm_wcdma_ccs_tq_t *rf_tq,
  uint8 ccs_block_idx,
  uint32 ustmr_action_time,
  uint32 handle
  );


#ifdef __cplusplus
} // extern "C"
#endif

#endif /* RFLM_WCDMA_CCS_H */
