/*!
  @file
  gsi_core.h

  @brief
  This file contains declaration of all the GSI core functions


*/
/*===========================================================================

  Copyright (c) 2015-2019 QUALCOMM Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: 

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/11/19   gk      CR2386972: Avoid setting endp_delay on peripheral endpoints
                   as a work around for HW bug.
10/13/17   pgm     New return data for prev_opcode and compcode for InterEE cmd
10/05/17   scv     Changes to read GSI completion response without busywaiting to reduce MPPS
10/25/16   da      Add ring base Va to the Channel core context
04/17/16   tz      Add OOB/RE GSI statistic counters
03/27/16   da      Add SSR clean-up support
03/22/16   tz      Support GSI inter-EE commands
02/16/16   tz      Support disabling MCS during FW verification
12/24/15   tz      GSI statistics collection support
12/17/15   tz      Support MCS breakpoint IRQ
03/12/15   tz      GSI test-framework changes
11/22/15   tz      Support for MCS/IPA sequencers verification
10/01/15   tz      GSI driver optimizations
04/22/15   pgm     Interrupt handling changes.
02/15/15   tz      Initial version
===========================================================================*/
#ifndef GSI_CORE_H
#define GSI_CORE_H

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include <comdef.h>
#include "gsi_hw_common.h"
#include "gsi.h"
#include "gsi_osal.h"


#define ENABLE_MCS_x2_CLK
/*==============================================================================

                      Macros

==============================================================================*/
#define GSI_CORE_MAX_IEP_NAME_LEN 40
#define GSI_CORE_MAX_FW_SIZE             (2*2048 * sizeof(uint32))
#define GSI_CORE_MAX_NUMBER_OF_IEPS 16

#define GSI_UC_IEP_DEF(iep_name, iep_offset)    {#iep_name, iep_offset}
#define GASM(b0, b1, b2, b3, b4, b5)       b5, b4, b3, b2, b1, b0, 0, 0

/*===========================================================================

                    EXTERNAL FUNCTION PROTOTYPES

===========================================================================*/



/*===========================================================================

                   INTERNAL DEFINITIONS AND TYPES

===========================================================================*/

/*! @brief GSI event information type */
typedef struct
{
  /*! General event type */
  gsi_event_type_e event_type;
  /*! Per event type -
  For GSI_EVENT_TYPE_GLOBAL of type gsi_global_event_type_e.
  For GSI_EVENT_TYPE_CHANNEL of type gsi_channel_event_type_e.
  For GSI_EVENT_TYPE_ER of type gsi_er_event_type_e.
  */
  uint32 event_specific_type;
  /*! EE associated with this event */
  uint8 ee;
  /*! Channel/ER associated with this event - valid only for non-global events */
  uint8 index;
  /*! Client user data */
  uint32 user_data;
} gsi_core_event_info_s;

/*!@brief GSI core call-back - invoked for EE/channel events */
typedef void (*gsi_core_callback_func_type)(gsi_core_event_info_s* event_info, uint32 user_data); 

/*! @brief Config structure to initialize GSI EE configurations */
typedef struct
{
  /*! MSI/Discrete interrupt */
  gsi_hw_channel_interrupt_type_e int_type;
  /*! MSI address to be written by GSI -
      valid only for MSI mode */
  uint64 msi_addr;
  /*! Value of MSI vector to be written by GSI to the MSI 
      register space - valid only for MSI mode */
  uint32 msi_vec;
  /*! Scratch register - set according to configuration type
      with the gsi_hw_ee_scratch_s format */
  uint64 scratch;
  /*! EE weight used with the channel weight in the RR algorithm for 
      channel arbitration */
  uint8 weight;
  /*! Max allocated channels */
  uint8 max_channels;
  /*! Max allocated events */
  uint8 max_ev_channels;
  /*! Mask interrupt types */
  uint32 ee_irq_mask;
  /*! Mask event interrupt */
  uint32 src_ieob_irq_mask;
  /*! Mask channel control interrupt */
  uint32 src_ch_irq_mask;
  /*! Mask event control interrupt */
  uint32 src_ev_ch_irq_mask;
  /*! Mask inter EE control interrupt */
  uint32 inter_ee_src_ch_irq_mask;
  /*! Mask inter EE event control interrupt */
  uint32 inter_ee_src_ev_ch_irq_mask;
  /* GSI IRQ number */
  uint32 gsi_irq;
} gsi_core_ee_cfg_s;

/*! @brief Config structure to initialize GSI channel context*/
typedef struct
{
  /*! Channel ring base address */
  uint64 ring_ba;
  /*! Channel ring virtual base address */
  uint64 ring_va;
  /*! Channel ring length */
  uint16 ring_len;
  /*! Type of protocol supported by channel */
  gsi_hw_channel_protocol_type_e protocol_type;
  /*! Channel direction */
  gsi_hw_channel_direction_type_e direction;
  /*! Ring element size in bytes */
  gsi_hw_channel_element_size_type_e elem_size;
  /*! Event ring index where the device will insert completion events */
  uint8 er_index;
  /*! Priority of the channel to be considered by RE Engine round 
      robin algorithm */
  uint8 ch_weight;
  /*! DB mode */
  gsi_hw_channel_db_mode_type_e db_mode;
  /*! Scratch register - set according to configuration type:
  For MHI/GPI mode - with gsi_hw_mhi_ch_scratch_s type. 
  For xDCI mode - with gsi_hw_xdci_ch_scratch_s type.
  */
  uint64 scratch[2];
  /*! Limit the number of pre-fetch buffer segments for channel */
  gsi_hw_channel_max_prefetch_type_e max_pref;
} gsi_core_tr_ch_cfg_s;

/*! @brief Config structure to initialize GSI event context */
typedef struct
{
  /*! Type of protocol supported by channel */
  gsi_hw_channel_protocol_type_e protocol_type;
  /*! Interrupt type - MSI/IRQ*/
  gsi_hw_channel_interrupt_type_e int_type;
  /*! MSI address to be written by GSI -
      valid only for MSI mode */
  uint64 msi_addr;
  /*! Value of MSI vector to be written by GSI to the MSI 
      register space - valid only for MSI mode */
  uint32 msi_vec;
  /*! Interrupt moderation cycle counter */
  uint16 intmodcyc;
  /*! Interrupt moderation packet counter */
  uint8 intmodcount;
  /*! Ring element size in bytes */
  gsi_hw_channel_element_size_type_e elem_size;
  /*! Event ring base address */
  uint64 ring_ba;
  /*! Event ring length */
  uint16 ring_len;
  /*! Address to which updated event RP should be written on every
      event generation. Used for MHI device write to update RP in the
      MHI event context.
      This value is not used by GPI channels - SW should set it to 
      0 in such case. */
  uint64 rp_update_addr;
  /*! Scratch register of gsi_hw_mhi_ev_scratch_s/
      gsi_hw_xdci_ev_scratch_s type.*/
  uint64 scratch;
} gsi_core_ev_ch_cfg_s;

/*! @brief Structure to hold state of ring read from GSI channel/event */
typedef struct
{
  /*! Channel protocol */
  gsi_hw_channel_protocol_type_e protocol;
  /*! Ring base address */
  uint64 ring_ba;
  /*! Ring length */
  uint32 ring_length;
  /*! Ring write pointer */
  uint64 ring_wp;
  /*! Ring read pointer */
  uint64 ring_rp;
} gsi_core_ring_state_s;

/*! @brief Structure to hold state of channel context read from GSI channel */
typedef struct
{
  gsi_core_ring_state_s ring;
  /*! Channel state */
  gsi_hw_channel_state_type_e state;
  /*! Channel direction */
  gsi_hw_channel_direction_type_e direction;
  /*! Channel protocol */
  gsi_hw_channel_protocol_type_e protocol;
  /*! ER ID */
  uint32 er_id;
} gsi_core_ch_ctx_state_s;

/*! @brief Structure to hold state of event context read from GSI event */
typedef struct
{
  gsi_core_ring_state_s ring;
  gsi_hw_event_channel_state_type_e state;
  uint32 intype;
  uint64 msi_addr;
  uint32 int_modt;
  uint32 int_modc;
  uint32 intvec;
} gsi_core_ev_ctx_state_s;

/*!@brief GSI client handle returned for GSI init */
typedef uint32 gsi_core_gsi_handle_t;

/*!@brief Channel handle returned for channel init */
typedef uint32 gsi_core_ch_handle_t;

/*!@brief Event ring handle returned for event init */
typedef uint32 gsi_core_ev_handle_t;

/*!@brief Channel ID passed to GSI */
typedef uint8 gsi_core_ch_id_t;

/*!@brief Event ID passed to GSI */
typedef uint8 gsi_core_ev_id_t;

/*!@brief GSI MCS statistics structure */
typedef struct {
  /*! Number of times UL Transfer ring became full */
  uint32 num_ul_ring_full;
  /*! Number of times DL Transfer ring became empty */
  uint32 num_dl_ring_empty;
  /*! Number of times OOB event sent on DL channel */
  uint32 num_oob;
  /*! Number of times UL Transfer ring wraparound events */
  uint32 num_ul_ring_wraparound;
  /*! Number of times DL Transfer ring wraparound events */
  uint32 num_dl_ring_wraparound;
}gsi_core_mcs_stats_s;

/**
 * @brief   Structure for MCS isr pointers 
 */
typedef struct  { 
  char name[GSI_CORE_MAX_IEP_NAME_LEN]; 
  uint32 iep_offset; 
} gsi_mcs_iep;

/**
 * @brief   Structure for GSI FW image keeper. 
 */
typedef struct 
{
  gsi_mcs_iep iep[GSI_CORE_MAX_NUMBER_OF_IEPS];
  uint32 fw_num_lines;
  uint8 image[GSI_CORE_MAX_FW_SIZE];
} gsi_fw_image;

typedef enum
{  
  GSI_CORE_GLOB_IRQ_NONE    = 0x00000000,
  GSI_CORE_GLOB_IRQ_ERR_INT = 0x00000001,
  GSI_CORE_GLOB_IRQ_GP_INT1 = 0x00000002,
  GSI_CORE_GLOB_IRQ_GP_INT2 = 0x00000004,
  GSI_CORE_GLOB_IRQ_GP_INT3 = 0x00000008,
  GSI_CORE_GLOB_IRQ_MAX     = 0x7FFFFFFF
} gsi_core_glob_irq_e;


/*!Enable only global err irqs*/
#define GSI_CORE_GLOB_IRQ_ENABLED_MASK   GSI_CORE_GLOB_IRQ_ERR_INT

/*!Enable all gen err irqs - disable breakpoint IRQ */
#define GSI_CORE_GEN_IRQ_ENABLED_MASK  (HWIO_EE_n_CNTXT_GSI_IRQ_EN_GSI_MCS_STACK_OVRFLOW_BMSK | \
                                        HWIO_EE_n_CNTXT_GSI_IRQ_EN_GSI_CMD_FIFO_OVRFLOW_BMSK  | \
                                        HWIO_EE_n_CNTXT_GSI_IRQ_EN_GSI_BUS_ERROR_BMSK)

#define GSI_CORE_GLOB_IRQ_CLR_MASK   0x0000000F
#define GSI_CORE_IRQ_CLR_MASK   0xFFFFFFFF
#define GSI_CORE_ERROR_LOG_CLR_VAL   0x1

/*==============================================================================

                                FUNCTIONS

==============================================================================*/
/*==============================================================================

  FUNCTION:  gsi_core_isr

==============================================================================*/
/*!
  @brief
  GSI ISR to be registered by client on GSI IRQ

*/
/*============================================================================*/
void* gsi_core_isr(void* ctx);

/*==============================================================================

  FUNCTION:  gsi_osal_register_isr

==============================================================================*/
/*!
  @brief
  OS AL method to register the ISR on the specified IRQ.

  @param
  irq, IRQ line
  isr, ISR handler
  ctx, isr context

  @retval GSI_STATUS_SUCCESS
  @retval other error codes for failure

*/
/*============================================================================*/
gsi_status_e gsi_core_register_isr
(
  uint32            irq, 
  gsi_osal_isr_type isr, 
  void              *data
);
/*==============================================================================

  FUNCTION:  gsi_core_gsi_init

==============================================================================*/
/*!
  @brief
  Initializes the GSI core driver 

  @param
  base_address, GSI module base address
  ee, EE to use by client 
  ctl_cb, control CB to register for this EE - will be called on global and 
  channel control events
  ctl_user_data, control CB client user data
  xfer_cb, transfer CB to register for this EE - will be called on transfer completion events
  xfer_user_data, xfer CB client user data
  client_id, output parameter with ID of allocated client

  @retval GSI_STATUS_SUCCESS
  @retval other error codes for failure

*/
/*============================================================================*/
gsi_status_e gsi_core_gsi_init
(
  uint32 base_address,
  gsi_hw_ee_type_e ee,
  gsi_core_callback_func_type ctl_cb,
  uint32 ctl_user_data,
  gsi_core_callback_func_type xfer_cb,
  uint32 xfer_user_data,
  gsi_core_gsi_handle_t*  client_id
);

/*==============================================================================

  FUNCTION:  gsi_core_gsi_hw_init

==============================================================================*/
/*!
  @brief
  Initializes the GSI module and loads the sequencer image (
  depends on requested configuration).

  @param
  client_id, GSI client to initialize

  @retval GSI_STATUS_SUCCESS
  @retval other error codes for failure

*/
/*============================================================================*/
gsi_status_e gsi_core_gsi_hw_init
(
  gsi_core_gsi_handle_t  client_id,
  gsi_fw_image *fw_img
);

gsi_status_e gsi_test_core_gsi_hw_init
(
  gsi_core_gsi_handle_t  client_id
);

/*==============================================================================

  FUNCTION:  gsi_core_ee_init

==============================================================================*/
/*!
  @brief
  Initializes the GSI EE specific configurations

  @param
  client_id, client ID allocated using gsi_core_gsi_init().
  ee_cfg, EE configuration structure
  

  @retval GSI_STATUS_SUCCESS
  @retval other error codes for failure

*/
/*============================================================================*/
gsi_status_e gsi_core_ee_init
(
  gsi_core_gsi_handle_t  client_id,
  gsi_core_ee_cfg_s*      ee_cfg  
);


/*==============================================================================

  FUNCTION:  gsi_core_alloc_gsi_ch

==============================================================================*/
/*!
  @brief
  Allocates the GSI channel.

  @param
  client_id, GSI client ID allocated through gsi_core_gsi_init().
  ch_id, CHID of virtual channel to allocate.
  ch_handle, output parameter with allocated channel handle .
  

  @retval GSI_STATUS_SUCCESS
  @retval other error codes for failure

*/
/*============================================================================*/
gsi_status_e gsi_core_alloc_gsi_ch
(
  gsi_core_gsi_handle_t  client_id,
  gsi_core_ch_id_t       ch_id,
  gsi_core_ch_handle_t* ch_handle  
);

/*==============================================================================

  FUNCTION:  gsi_core_dealloc_gsi_ch

==============================================================================*/
/*!
  @brief
  De-allocates the GSI channel.

  @param
  ch_handle, allocated channel handle.
  Utilized as a channel ID if GSI_COMMAND_OPTIONS_CH_CLEANUP option is set.
  options, 32-bit options flags for the GSI operation.

  @retval GSI_STATUS_SUCCESS
  @retval other error codes for failure

*/
/*============================================================================*/
gsi_status_e gsi_core_dealloc_gsi_ch
(
  gsi_core_ch_handle_t ch_handle,
  uint32 options
);

/*==============================================================================

  FUNCTION:  gsi_core_ch_init_ctx

==============================================================================*/
/*!
  @brief
  Initializes the GSI channel context. 

  @param
  ch_handle, channel handle allocated using gsi_core_alloc_gsi_ch(). 
  ch_cfg, channel configuration structure.
  options, 32-bit options flags for the GSI operation.

  @retval GSI_STATUS_SUCCESS
  @retval other error codes for failure

  @note
  Client should allocate channel using call to gsi_core_alloc_gsi_ch() before 
  any channel operation.

*/
/*============================================================================*/
gsi_status_e gsi_core_ch_init_ctx
(
  gsi_core_ch_handle_t  ch_handle,
  gsi_core_tr_ch_cfg_s* ch_cfg,
  uint32 options
);

/*==============================================================================

  FUNCTION:  gsi_core_alloc_gsi_ev_ch

==============================================================================*/
/*!
  @brief
  Allocates the GSI event ring.

  @param
  client_id, GSI client ID allocated through gsi_core_gsi_init().
  ev_id, EV_ID of virtual event to allocate.
  ev_handle, output parameter with allocated channel handle. 

  @retval GSI_STATUS_SUCCESS
  @retval other error codes for failure

*/
/*============================================================================*/
gsi_status_e gsi_core_alloc_gsi_ev_ch
(
  gsi_core_gsi_handle_t  client_id,
  gsi_core_ev_id_t       ev_id,
  gsi_core_ev_handle_t* ev_handle  
);

/*==============================================================================

  FUNCTION:  gsi_core_dealloc_gsi_ev_ch

==============================================================================*/
/*!
  @brief
  De-allocates the GSI event ring.

  @param
  ev_handle, allocated event ring handle.
  Utilized as a event ring ID if GSI_COMMAND_OPTIONS_EV_CLEANUP option is set.
  options, 32-bit options flags for the GSI operation.

  @retval GSI_STATUS_SUCCESS
  @retval other error codes for failure

*/
/*============================================================================*/
gsi_status_e gsi_core_dealloc_gsi_ev_ch
(
  gsi_core_ev_handle_t ev_handle,
  uint32 options
);


/*==============================================================================

  FUNCTION:  gsi_core_ev_init_ctx

==============================================================================*/
/*!
  @brief
  Initializes the GSI event context. 

  @param
  ev_handle, event handle allocated using gsi_core_alloc_gsi_ev_ch().
  ev_ch_cfg, channel configuration structure
  

  @retval GSI_STATUS_SUCCESS
  @retval other error codes for failure

  @note
  Client should allocate channel using call to gsi_core_alloc_gsi_ev_ch() before 
  any channel operation.

*/
/*============================================================================*/
gsi_status_e gsi_core_ev_init_ctx
(
  gsi_core_ev_handle_t  ev_handle,
  gsi_core_ev_ch_cfg_s* ev_ch_cfg  
);

/*==============================================================================

  FUNCTION:  gsi_core_ch_send_cmd

==============================================================================*/
/*!
  @brief
  Send command to GSI channel. In the completion of the command programming 
  the GSI driver will fetch the channel state from the channel context to 
  ch_state output parameter - client can use this value to check the command
  completion state.


  @param
  ch_handle, handle of GSI channel.
  Utilized as a channel ID if GSI_COMMAND_OPTIONS_CH_CLEANUP option is set.
  opcode, opcode of command to program.
  ch_state, output parameter with state of the channel at the completion of the
  command.
  options, 32-bit options flags for the GSI operation.

  @retval GSI_STATUS_SUCCESS
  @retval other error codes for failure

*/
/*============================================================================*/
gsi_status_e gsi_core_ch_send_cmd
(
  gsi_core_ch_handle_t ch_handle,
  gsi_hw_channel_cmd_opcode_type_e opcode,
  gsi_hw_channel_state_type_e* ch_state,
  uint32 options
);

/*==============================================================================

  FUNCTION:  gsi_core_ev_ch_send_cmd

==============================================================================*/
/*!
  @brief
  Send command to GSI event channel. In the completion of the command programming 
  the GSI driver will fetch the event state from the channel context to 
  ev_state output parameter - client can use this value to check the command
  completion state.


  @param
  ev_handle, handle of GSI channel.
  Utilized as a event ring ID if GSI_COMMAND_OPTIONS_EV_CLEANUP option is set.
  opcode, opcode of command to program.
  ev_state, output parameter with state of the event channel at the completion of the
  command.
  options, 32-bit options flags for the GSI operation.

  @retval GSI_STATUS_SUCCESS
  @retval other error codes for failure

*/
/*============================================================================*/
gsi_status_e gsi_core_ev_ch_send_cmd
(
  gsi_core_ev_handle_t ev_handle,
  gsi_hw_event_channel_cmd_opcode_type_e opcode,
  gsi_hw_event_channel_state_type_e* ev_state,
  uint32 options
);

/*==============================================================================

  FUNCTION:  gsi_core_start_gsi_ch

==============================================================================*/
/*!
  @brief
  Starts the allocated GSI channel by sending a start command to GSI. Note that 
  GSI channel context must be valid when performing this operation.

  @param
  ch_handle, channel handle allocated using gsi_core_alloc_gsi_ch().
  Utilized as a channel ID if GSI_COMMAND_OPTIONS_CH_CLEANUP option is set.
  options, 32-bit options flags for the GSI operation.

  @retval GSI_STATUS_SUCCESS
  @retval other error codes for failure

  @note
  Client should allocate channel using call to gsi_core_alloc_gsi_ch() before 
  any channel operation.

*/
/*============================================================================*/
gsi_status_e gsi_core_start_gsi_ch
(
  gsi_core_ch_handle_t  ch_handle,
  uint32 options
);

/*==============================================================================

  FUNCTION:  gsi_core_stop_gsi_ch

==============================================================================*/
/*!
  @brief
  Stops the allocated GSI channel by sending a stop command to GSI.
  The function implementation includes a retry mechanism in case the
  channel remains in STOP_IN_PROGRESS state.

  @param
  ch_handle, channel handle allocated using gsi_core_alloc_gsi_ch().
  Utilized as a channel ID if GSI_COMMAND_OPTIONS_CH_CLEANUP option is set.
  options, 32-bit options flags for the GSI operation.

  If GSI_COMMAND_OPTIONS_STOP_WAIT_DB option is set, the function will call
  the STOP_DB command.
  If the channel is a producer, and GSI_COMMAND_OPTIONS_INBOUND_CH is set,
  the function will perform the DMA Task flush WA.

  @retval GSI_STATUS_SUCCESS
  @retval GSI_STATUS_ERR_GENERAL_FAILURE - Stop operation failed

  @note
  Client should allocate channel using call to gsi_core_alloc_gsi_ch() before 
  any channel operation.

*/
/*============================================================================*/
gsi_status_e gsi_core_stop_gsi_ch
(
  gsi_core_ch_handle_t  ch_handle,
  uint32 options
);

/*==============================================================================

  FUNCTION:  gsi_core_reset_gsi_ch

==============================================================================*/
/*!
  @brief
  Resets the allocated GSI channel by sending a reset command to GSI. 
  In case of an inbound channel, and an aggregation is open, the WA will be
  applied.

  @param
  ch_handle, channel handle allocated using gsi_core_alloc_gsi_ch().
  Utilized as a channel ID if GSI_COMMAND_OPTIONS_CH_CLEANUP option is set.
  options, 32-bit options flags for the GSI operation.
  ring_va, Virtual address for the ring allocated for the channel.
  It is used for the GSI Producer reset WA with an open aggregation.
  ring_pa, Physical address for the ring allocated for the channel.
  It is used for the GSI Producer reset WA with an open aggregation.

  @retval GSI_STATUS_SUCCESS
  @retval other error codes for failure

  @note
  Client should allocate channel using call to gsi_core_alloc_gsi_ch() before 
  any channel operation. (Unless GSI_COMMAND_OPTIONS_CH_CLEANUP is set)
  @note
  Client should put GSI channel in stopped state invoking the reset operation.
*/
/*============================================================================*/
gsi_status_e gsi_core_reset_gsi_ch
(
  gsi_core_ch_handle_t  ch_handle,
  uint32 options,
  uint64 ring_va,
  uint64 ring_pa
);

/*==============================================================================

  FUNCTION:  gsi_core_ch_set_db

==============================================================================*/
/*!
  @brief
  Set doorbell on specified channel. 

  @param
  ch_handle, channel handle allocated using gsi_core_alloc_gsi_ch(). 

  @retval GSI_STATUS_SUCCESS
  @retval other error codes for failure

*/
/*============================================================================*/
gsi_status_e gsi_core_ch_set_db
(
  gsi_core_ch_handle_t  ch_handle,
  uint64 db_value
);

/*==============================================================================

  FUNCTION:  gsi_core_ev_set_db

==============================================================================*/
/*!
  @brief
  Set doorbell on specified event ring. 

  @param
  ev_handle, event handle allocated using gsi_core_alloc_gsi_ev_ch(). 

  @retval GSI_STATUS_SUCCESS
  @retval other error codes for failure

*/
/*============================================================================*/
gsi_status_e gsi_core_ev_set_db
(
  gsi_core_ev_handle_t  ev_handle,
  uint64 db_value
);

/*==============================================================================
  
  FUNCTION:  gsi_core_ev_get_rp

==============================================================================*/
/*!
  @brief
  Get current ring RP from specified event. 

  @param
  ev_handle, event handle. 
  rp, output parameter: new channel RP

  @retval GSI_STATUS_SUCCESS
  @retval other error codes for failure

*/
/*============================================================================*/
gsi_status_e gsi_core_ev_get_rp
(
  gsi_core_ev_handle_t  ev_handle,
  uint64 *rp
);

/*==============================================================================

  FUNCTION:  gsi_core_ch_get_rp

==============================================================================*/
/*!
  @brief
  Get current ring RP from specified channel. 

  @param
  ch_handle, channel handle allocated using gsi_core_alloc_gsi_ch(). 
  rp, output parameter: new channel RP

  @retval GSI_STATUS_SUCCESS
  @retval other error codes for failure

*/
/*============================================================================*/
gsi_status_e gsi_core_ch_get_rp
(
  gsi_core_ch_handle_t  ch_handle,
  uint64 *rp
);

/*==============================================================================

  FUNCTION:  gsi_core_ch_is_empty

==============================================================================*/
/*!
  @brief
  Check if the specified channel empty.

  @param
  ch_handle, channel handle allocated using gsi_core_alloc_gsi_ch().

  @retval boolean

*/
/*============================================================================*/
boolean gsi_core_ch_is_empty
(
  gsi_core_ch_handle_t  ch_handle
);




/*==============================================================================

  FUNCTION:  gsi_core_ev_mask_irq

==============================================================================*/
/*!
  @brief
  Mask/unmask event interrupt. 

  @param
  ev_handle, event handle allocated using gsi_core_alloc_gsi_ev_ch(). 
  mask, if TRUE mask event - if FALSE unmask event IRQ

  @retval GSI_STATUS_SUCCESS
  @retval other error codes for failure

*/
/*============================================================================*/
gsi_status_e gsi_core_ev_mask_irq
(
  gsi_core_ev_handle_t  ev_handle,
  boolean               mask
);

/*==============================================================================

  FUNCTION:  gsi_core_get_ee_ctx

==============================================================================*/
/*!
  @brief
  Get current context for specified EE. 

  @param
  ee_id, EE. 

  @retval context of specified EE or NULL if not supported

*/
/*============================================================================*/
gsi_osal_client_ctx_s* gsi_core_get_ee_ctx(gsi_hw_ee_type_e ee_id);

/*==============================================================================

  FUNCTION:  gsi_core_ev_reset_gsi_ev_ch

==============================================================================*/
/*!
  @brief
  Resets the allocated GSI event ring by sending a reset command to GSI. 

  @param
  ev_handle, event handle allocated using gsi_core_alloc_gsi_ev_ch().
  Utilized as a event ring ID if GSI_COMMAND_OPTIONS_EV_CLEANUP option is set.
  options, 32-bit options flags for the GSI operation.

  @retval GSI_STATUS_SUCCESS
  @retval other error codes for failure

  @note
  Client should allocate event using call to gsi_core_alloc_gsi_ev_ch() before 
  any event operation.
*/
/*============================================================================*/
gsi_status_e gsi_core_ev_reset_gsi_ev_ch
(
  gsi_core_ev_handle_t  ev_handle,
  uint32 options
);
/*==============================================================================

  FUNCTION:  gsi_core_gsi_deinit

==============================================================================*/
/*!
  @brief
  Initializes the GSI core driver 

  @param
  base_address, GSI module base address
  ee, EE to use by client 
  cb, CB to register for this EE
  user_data, CB client user data
  client_id, output parameter with ID of allocated client

  @retval GSI_STATUS_SUCCESS
  @retval other error codes for failure

*/
/*============================================================================*/
gsi_status_e gsi_core_gsi_deinit
(
  gsi_core_gsi_handle_t client_id
);

/*==============================================================================

  FUNCTION:  gsi_core_ch_send_inter_ee_cmd

==============================================================================*/
/*!
  @brief
  Send inter-EE command to GSI channel. Driver will issue the command on the 
  destination's EE inter-EE command register and will poll for the inter-EE 
  command completion interrupt.


  @param
  orig_ee_id, ID of originator EE. 
  dest_ee_id, ID of destination's channel EE. 
  ch_id, ID of channel in the destination EE.
  opcode, opcode of command to program.
  pcmd_res, output parameter: completion code from MCS and prev_opcode

  @retval GSI_STATUS_SUCCESS
  @retval other error codes for failure

*/
/*============================================================================*/
gsi_status_e gsi_core_ch_send_inter_ee_cmd
(
  uint32 orig_ee_id,
  uint32 dest_ee_id,
  uint32 ch_id,
  gsi_hw_channel_cmd_opcode_type_e opcode,
  gsi_hw_channel_inter_ee_prev_cmd_result_s *pcmd_res,
  boolean read_comp_only
);

/*==============================================================================

  FUNCTION:  gsi_core_get_iep

==============================================================================*/
/*!
  @brief
  Get MCS ISR pointers.

  @param  

  name, iep name
  iep_offset, offset in IRAM

  @retval IEP value

*/
/*============================================================================*/
uint32 gsi_core_get_iep 
(
  char                 *name
);

/*! Test hook for forcing invalid ER configuration during channel init */
void gsi_core_set_invalid_er(void);

gsi_status_e gsi_core_ch_get_wp
(
  gsi_core_ch_handle_t  ch_handle,
  uint64 *wp
);

gsi_status_e gsi_core_ev_get_wp
(
  gsi_core_ev_handle_t  ev_handle,
  uint64 *wp
);

/*==============================================================================

  FUNCTION:  gsi_core_ch_get_ctx

==============================================================================*/
/*!
  @brief
  Get current ring context from specified channel. 

  @param
  ee_id, EE number of requested channel
  ch_id, channel number
  ch_ctx, output parameter: read channel context

  @retval GSI_STATUS_SUCCESS
  @retval other error codes for failure

*/
/*============================================================================*/
gsi_status_e gsi_core_ch_get_ctx
(
  uint32  ee_id,
  uint32  ch_id,
  gsi_core_ch_ctx_state_s *ch_ctx
);

/*==============================================================================

  FUNCTION:  gsi_core_ev_get_ctx

==============================================================================*/
/*!
  @brief
  Get current ring context from specified event. 

  @param
  ee_id, EE number of requested channel
  ev_id, event number 
  ev_ctx, output parameter: read event context

  @retval GSI_STATUS_SUCCESS
  @retval other error codes for failure

*/
/*============================================================================*/
gsi_status_e gsi_core_ev_get_ctx
(
  uint32  ee_id,
  uint32  ev_id,
  gsi_core_ev_ctx_state_s *ev_ctx
);

/*==============================================================================

  FUNCTION:  gsi_core_get_mcs_stats

==============================================================================*/
/*!
  @brief
  Get current MCS statistics counters from . 

  @param
  stats, output parameter: statistics structure to update

  @retval GSI_STATUS_SUCCESS
  @retval other error codes for failure

*/
/*============================================================================*/
gsi_status_e gsi_core_get_mcs_stats
(
  gsi_core_mcs_stats_s *stats
);

/*==============================================================================

  FUNCTION:  gsi_core_enable_mcs

==============================================================================*/
/*!
  @brief
  Enable MCS operation - usually after image verification.

  @param  

  @retval GSI_STATUS_SUCCESS
  @retval other error codes for failure

*/
/*============================================================================*/
gsi_status_e gsi_core_enable_mcs 
(
  void
);

/*==============================================================================

  FUNCTION:  gsi_core_disable_mcs

==============================================================================*/
/*!
  @brief
  Disable MCS operation - usually before image verification.

  @param  

  @retval GSI_STATUS_SUCCESS
  @retval other error codes for failure

*/
/*============================================================================*/
gsi_status_e gsi_core_disable_mcs 
(
  void
);

/*==============================================================================

  FUNCTION:  gsi_core_alloc_a7_channels

==============================================================================*/
/*!
  @brief
  Allocate mock A7 MHI channels

  @param

  @retval 

*/
/*============================================================================*/
void gsi_core_alloc_a7_channels(void);

/*==============================================================================

  FUNCTION:  gsi_core_set_flow_control

==============================================================================*/
/*!
  @brief
  Enable/Disable flow controlling on inter EE consumer channels.

  @param

  @retval 

*/
/*============================================================================*/
gsi_status_e gsi_core_set_flow_control
(
  uint8  dst_ee_id, 
  uint8  dst_gsi_ch, 
  uint8  flow_control_state
);

#ifdef FEATURE_DATA_IPA_HW_RESET_ENABLE
/*==============================================================================

  FUNCTION:  gsi_core_boot_init

==============================================================================*/
/*!
  @brief
  Cold boot related initializations.

  @param

  @retval 

*/
/*============================================================================*/
void gsi_core_boot_init(void);

/*==============================================================================

  FUNCTION:  gsi_core_load_image

==============================================================================*/
/*!
  @brief
  Loads GSI MCS image onto IRAM

  @param

  @retval 

*/
/*============================================================================*/
void gsi_core_load_image(gsi_fw_image* mcs_fw_img);
#endif  /*  FEATURE_DATA_IPA_HW_RESET_ENABLE  */

#endif /* GSI_CORE_H */
