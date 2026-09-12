/*!
  @file
  rflm_diag_power_rfa.h

  @brief
  RFLM power change logging RFA interfaces

  @detail

*/

/*===========================================================================

  Copyright (c) 2015 Qualcomm Technologies, Inc.  All Rights Reserved.
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

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rflm.mpss/2.11/services/diag/inc/rflm_diag_power_rfa.h#1 $

when         who     what, where, why
----------   ---     --------------------------------------------------------
2015-07-06   rc      Add power logging support
2014-11-17   aw      Initial version
===========================================================================*/
#ifndef RFLM_DIAG_POWER_RFA_H
#define RFLM_DIAG_POWER_RFA_H

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/

/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/
#define RFLM_DIAG_POWER_ARGS_MAX_CNT                          6
#define RFLM_DIAG_POWER_ARGS_MAX_CNT                          6
#define RFLM_DIAG_POWER_RFA_TX_HANDLE                         0
#define RFLM_DIAG_POWER_RFA_RX_HANDLE                         1
#define RFLM_DIAG_POWER_RFA_DISABLE_POWER_LOGGING             0xFF
#define RFLM_DIAG_POWER_RFA_INVALID_CHANNEL                   0xFF
#define RFLM_DIAG_POWER_RFA_STATE_ON                          1
#define RFLM_DIAG_POWER_RFA_STATE_OFF                         0
#define RFLM_DIAG_POWER_RFA_STATE_DISABLE                     2
#define RFLM_DIAG_POWER_RFA_STATE_ENABLE                      3
#define RFLM_DIAG_POWER_RFA_MAX_HANDLE                        20

typedef struct
{
   uint8 length:8;
   uint8 sub_id:8;
   uint8 device_id:8;
   uint8 tech: 8;
} rflm_diag_power_buffer_item_header_t;

typedef struct
{
	int16 chg_data: 16;
	int16 chg_type: 16;
}rflm_diag_power_buffer_item_payload_t;

typedef struct
{
  rflm_diag_power_buffer_item_header_t  header;
  uint32 timestamp;
  rflm_diag_power_buffer_item_payload_t payload[RFLM_DIAG_POWER_ARGS_MAX_CNT];
}rflm_diag_power_buffer_item_t;

typedef enum
{
  RFLM_DIAG_POWER_TYPE_TX_AGC,
  RFLM_DIAG_POWER_TYPE_RX_AGC,
  RFLM_DIAG_POWER_TYPE_TX_CONFIG,
  RFLM_DIAG_POWER_TYPE_RX_CONFIG,
  RFLM_DIAG_POWER_TYPE_TX_DYNAMIC,
  RFLM_DIAG_POWER_TYPE_RX_DYNAMIC,
  RFLM_DIAG_POWER_TYPE_FBRX,
  RFLM_DIAG_POWER_TYPE_FBRX_DURATION,
  RFLM_DIAG_POWER_TYPE_FBRX_GAIN_STATE, 
	RFLM_DIAG_POWER_TYPE_RGI,
  RFLM_DIAG_POWER_TYPE_POWER, 
	RFLM_DIAG_POWER_TYPE_PA,
  RFLM_DIAG_POWER_TYPE_PATH,
  RFLM_DIAG_POWER_TYPE_STATE,
	RFLM_DIAG_POWER_TYPE_MOD, 
	RFLM_DIAG_POWER_TYPE_LNA,
  RFLM_DIAG_POWER_TYPE_RB,
	RFLM_DIAG_POWER_TYPE_MAX
}rflm_diag_power_type_e; 


/* 
   uint32 * rbuf_ptr : starting address of a ping-pong buffer
   uint32 num_of_rd  : Number of valid reads
   RFA can use it to read out buffer data
*/
typedef void (*rflm_diag_power_read_callback_t)(uint32* rbuf_ptr, uint32 num_of_rd);

/*===========================================================================

                           FUNCTION DECLARATIONS

===========================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================

                    EXTERNAL INTERFACE FUNCTIONS

===========================================================================*/

/* Used by RFA to notify power reporting thread 20ms switch */
extern void rflm_diag_power_switch_notify(void);

/* Used by RFA to set read data callback */
extern void rflm_diag_power_set_read_callback( rflm_diag_power_read_callback_t read_callback );
extern void rflm_diag_power_log_rx_agc_event( uint8 handle_id, uint8 lna_state );
extern void rflm_diag_power_log_trx_dynamic_event( uint8 handle_type, uint8 handle_id, uint8 state );
extern void rflm_diag_power_log_fbrx_event(uint8 handle_id, uint16 duration, uint8 gain_state);
extern void rflm_diag_power_log_enable_update(void);
extern boolean rflm_diag_power_log_enabled(void);
extern void rflm_diag_power_enable_qdss_log(void);
extern void rflm_diag_power_enable_qxdm_log(void);
extern boolean rflm_diag_power_qdss_log_enabled(void);
extern boolean rflm_diag_power_qxdm_log_enabled(void);
#ifdef __cplusplus
} // extern "C"
#endif

#endif
