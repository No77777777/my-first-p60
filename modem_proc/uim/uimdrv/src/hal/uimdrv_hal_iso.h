#ifndef UIMDRV_HAL_ISO_H
#define UIMDRV_HAL_ISO_H
/*===========================================================================

                            U I M D R V  H A L  I S O

DESCRIPTION
  This file contains driver functions that are specific to ISO UIM

Copyright (c) 2016 by QUALCOMM Technologies, Inc (QTI) and its licensors. All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/hal/uimdrv_hal_iso.h#2 $
$DateTime: 2019/10/30 06:56:05 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
10/30/15   hyo     Cleaning up comment 
09/02/15   hyo     Initial Revision
===========================================================================*/
#include "uimdrv.h"
#include "uimglobals.h"

/*============================================================================= 
  EXTERNALIZED FUNCTION PROTOTYPES
=============================================================================*/
/**
 *  This function deactivates the UIM so it does becomes inactive
 *  and does not use power. The electrical lines to the UIM are
 *  all set low in the appropriate order.
 *  
 *  @param uim_ptr[IN] Pointer to the current instance of the
 *                     UIM global structure
 */
void uimdrv_iso_power_up
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function turns on the power for the UIM.
 *  It puts the I/O line in reception mode (stops sending break
 *  on the I/O line) and turns on the UIM clock.
 *  
 *  @param uim_ptr[IN] Pointer to the current instance of the
 *                     UIM global structure
 */
void uimdrv_iso_power_down
(
  uim_instance_global_type *uim_ptr
);


/**
 *  UIMDRV reset function specific to ISO UIM This function
 *  This function asserts the reset line on the UIM.
 *  Any data received by the phone will be flushed. Any transfer
 *  from the phone will be terminated and the buffers flushed.
 *  
 *  @param rsp_ptr[OUT] Pointer to command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the
 *                     UIM global structure
 */
void uimdrv_iso_reset
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr
);


/**
 *  UIMDRV update op param function specific to ISO UIM
 *  This function sets up the operational parameters of the UIM
 *  interface. It can change the baud rate of the UIM
 *  interface, change the UIM clock frequency or change the
 *  guardtime.
 *  
 *  @param op_param[IN] operational parameters of the UIM
 *                      interface
 *  @param uim_ptr[IN] Pointer to the current instance of the
 *                     UIM global structure
 *  @return boolean Whether CLK stop is performed successfully.
 */
boolean uimdrv_iso_update_op_param
(
  uim_op_params_type const *op_params,
  uim_instance_global_type *uim_ptr
);


/**
 *  UIMDRV send command function specific to ISO UIM
 *  This function sets up the transfer of an APDU to the UIM.
 *  This function starts the transfer, which, if all goes well,
 *  is finished by the uim_rx_isr.
 *  
 *  Side effect: If the command was successful, reponse buffer
 *  in UIM global is updated
 *  
 *  @param uim_req[IN] Pointer to command request buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the
 *                     UIM global structure
 */
void uimdrv_iso_send_command
(
  uim_req_buf_type         *uim_req,
  uim_instance_global_type *uim_ptr
);


/**
 *  This function sets up the transfer of a PPS request to the
 *  UIM. This function starts the transfer, which, if all goes
 *  well, is finished by the uim_rx_isr. Side effect: If the
 *  command was successful, reponse buffer in UIM global is
 *  updated
 *  
 *  Dependencies: This function starts a PPS exchange. This
 *  exchange can only be done immediately after an ATR is
 *  received.
 *  
 *  Side Effect: The UIM driver will continue to send out the PPS
 *  request.  Then the driver will expect the PPS response.
 *  
 *  @param pps_request[IN] Pointer to the PPS request and the
 *                    PPS response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the
 *                     UIM global structure
 */
void uimdrv_iso_send_pps
(
  uim_pps_req_buf_type const *pps_request,
  uim_instance_global_type   *uim_ptr
);


#if defined( FEATURE_UIM_T_1_SUPPORT )
/**
 *  This function builds a T=1 information block and sends it to the UIM card.
 *  
 *  Dependencies: This function uses global variables to control how the
 *  information block is compiled and sent.
 *  
 *  Side Effect: The information block will be sent and a response block
 *  will be expected or a timeout will occur.
 *  
 *  @param node_addree[IN] node address byte
 *  @param uim_ptr[IN] Pointer to the current instance of the
 *                     UIM global structure
 */
void uimdrv_iso_t_1_send_i_block
(
  byte                    node_address,
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds a T=1 information block and sends it to the UIM card.
 *  
 *  Dependencies: This function uses global variables to control
 *  how the receive ready block is compiled and sent.
 *  
 *  Side Effect: The receive ready block will be sent and a response block
 *  will be expected or na timeout will occur.
 *  
 *  @param node_addree[IN] Node address byte
 *  @param r_block_code[IN] R-block response code
 *  @param uim_ptr[IN] Pointer to the current
 *                     instance of the UIM global structure
 */
void uimdrv_iso_t_1_send_r_block
(
  byte                    node_address, /*  */
  byte                    r_block_code, /* R-block response code */
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds a T=1 supervisory block and sends it to
 *  the UIM card.
 *  
 *  Dependencies: This function uses global variables to control how the
 *  supervisory block is compiled and sent.
 *  
 *  Side Effect: The supervisory block will be sent and a response block
 *  will be expected or a timeout will occur.
 *  
 *  @param node_addree[IN] Node address byte
 *  @param s_block_code[IN] S-block code
 *  @param s_block_data[IN] S-block data
 *  @param s_block_data_size[IN] S-block data size
 *  @param uim_ptr[IN] Pointer to the current
 *                     instance of the UIM global structure
 */
void uimdrv_iso_t_1_send_s_block
(
  byte                     node_address,        /* Node address byte */
  byte                     s_block_code,        /* S-block code */
  byte                     *s_block_data,       /* S-block data */
  byte                     s_block_data_size,    /* S-block data size */
  uim_instance_global_type *uim_ptr
);
#endif /* FEATURE_UIM_T_1_SUPPORT */


/**
 *  This function could be used to read the UART STATUS register value
 *  
 *  @param uim_ptr[IN] Pointer to the current
 *                     instance of the UIM global structure
 *  @return dword UART status (for the current slot if dual slot)
 */
dword uim_read_status
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function flushes the bytes in the RX FIFO by reading
 *  the bytes/words from the UART one by one and resets the UART
 *  RX to reset the error flags in UART. This way of reading the
 *  byte/words from the UART might, most of the times, take care
 *  of flushing those bytes/words that have just entered the
 *  UART and/or entering into the UART.
 *  
 *  @param uim_ptr[IN] Pointer to the current
 *                     instance of the UIM global structure
 */
void uimdrv_iso_flush_uart
(
  uim_instance_global_type *uim_ptr
);


/**
 *  Main interrupt handling method externed here to be
 *  registered in the interrupt controller
 *  
 *  @param uim_ptr[IN] Pointer to the current
 *                     instance of the UIM global structure
 */
void uimdrv_iso_rx_isr
(
  uim_instance_global_type *uim_ptr
);


/**
 *  Reconfigures the UART_DM to direct convention.
 *  
 *  @param uim_ptr[IN] Pointer to the current
 *                     instance of the UIM global structure
 */
void uimdrv_iso_reconfig_uartdm
(
  uim_instance_global_type *uim_ptr
);


/**
 *  Turns on the UIM related clocks. Operates on UIM1 or UIM2 depending
 *  on the instance_ptr.
 *  
 *  @param uim_ptr[IN] Pointer to the current
 *                     instance of the UIM global structure
 */
void UIMDRV_ISO_TCXO_MUST_BE_ON_SLOT
(
  uim_instance_global_type *uim_ptr
);


/**
 *  Turns off the UIM related clocks. Operates on UIM1 or UIM2
 *  depending on the value of the global variable uim_drv_slot.
 *  
 *  @param uim_ptr[IN] Pointer to the current
 *                     instance of the UIM global structure
 */
void UIMDRV_ISO_TCXO_CAN_BE_OFF_SLOT
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function starts the transfer of an APDU to the UIM.
 *  This function starts the transfer, which, if all goes well,
 *  is finished by the uim_rx_isr
 *  
 *  Dependencies: This function starts an APDU exchange. The
 *  exchange can only happen after the ATR has been received
 *  and, optionally, after a PPS exchange.
 *  
 *  Side Effect: The UIM driver will continue to send out the
 *  APDU. All the stages of sending the APDU are handled by
 *  the UIM driver.  Then the driver expects the APDU response.
 *  Some commands solicit a GET_RESPONSE or a FETCH command to
 *  be sent after the APDU.
 *  
 *  @param uim_req[IN] Pointer to the APDU command buffer, the
 *                command data buffer, the response buffer, the
 *                APDU status and the callback function
 *  @param uim_ptr[IN] Pointer to the current
 *                     instance of the UIM global structure
 */
void uimdrv_iso_send_apdu
(
  uim_req_buf_type         *uim_req, /* This points to the APDU command buffer, the
                                        command data buffer, the response buffer
                                        the APDU status and the callback function */
  uim_instance_global_type *uim_ptr
);
#endif /* UIMDRV_HAL_ISO_H */
