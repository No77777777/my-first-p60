#ifndef DS_MUX_PHYS_H
#define DS_MUX_PHYS_H
/*===========================================================================

                                 D S   M U X   P H Y S

                                 H E A D E R   F I L E

DESCRIPTION
  This is the Internal header file for the MUX Task. This file
  contains all the functions, definitions and data types needed for MUX
  task to interface to the SIO for physical port connection.


  Copyright (c) 2016 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/mux/inc/ds_mux_phys.h#2 $
  $DateTime: 2019/10/22 02:53:17 $
  $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/01/16   ad      initial version
===========================================================================*/

/*===========================================================================

                      INCLUDE FILES

===========================================================================*/

#include "sio.h"
#include "ds_mux_defs.h"

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/


/*--------------------------------------------------------------------------
      Physical connection control block: Data types to hold physical SIO connection info  
---------------------------------------------------------------------------*/
typedef struct
{

  dsm_watermark_type       tx_wmk;
  /* Transmit Watermark, to write to the SIO Port */
  
  q_type                   tx_q;
  /* Queue for the tx_wmk */
  
  boolean                  is_tx_flow_controlled;
  /* Tx Flow control, If true then cannot send any more data */

  boolean                  is_rx_flow_controlled;
  /* RX Flow control, if true then cannot receive any more data
     * DS MUX send FCon message */

  dsm_watermark_type       rx_wmk;
  /* Receive Watermark, to receive from the SIO Port */

  q_type                   rx_q;
  /* Queue for the rx_wmk */

  /*
   * SIO Stream ID, handle to talk to SIO lib
   */
  sio_stream_id_type       stream_id;
  /* SIO Stream ID  */
  
  boolean                  dte_ready_asserted;
  /* connection is ready to Rx and Tx data  */

  sio_port_id_type          sio_port;         
    
  /* sio_port id associated with device ID*/

  uint8                     rdm_oprt_retry;
  /* retry RDM operation*/

  ds_mux_timer_handle_type  timer_handle;
  /* retry  timer*/

  ds_mux_timer_handle_type  inactivity_timer_handle;
  /* retry  timer*/
  
} ds_mux_phys_sio_conn_info_type;

typedef enum
{
  DS_MUX_PHYS_RDM_OPRT_MIN                 = 0,
  DS_MUX_PHYS_RDM_ASSIGN_PORT              = 1,
  DS_MUX_PHYS_RDM_ASSIGN_PORT_FAILED       = 2,
  DS_MUX_PHYS_RDM_ASSIGN_PORT_IN_PROCESS   = 3,
  DS_MUX_PHYS_RDM_ASSIGN_PORT_COMPLETE     = 4,
  DS_MUX_PHYS_RDM_CLOSE_PORT               = 5
}ds_mux_phys_rdm_oprt_enum_type;

/*===========================================================================
                      PUBLIC FUNCTION DECLARATIONS
===========================================================================*/


/**
  @brief Initialize physical interface data structure to NULL.
*/
void ds_mux_phys_init
(

);

/**
  @brief De-Initialize physical interface data structure and de-allocate memory
         and timers.
*/
void ds_mux_phys_deinit
(

);


/**
  @brief Function to initialize the module. This function needs to be
         called once when passive init triggers it. All one time initializations
         will happen in this module.
*/
ds_mux_result_enum_type ds_mux_phys_conn_init
(

);

/**
  @brief Function to open physical SIO port. This function needs to be
         called once when Active init triggers it. All one time initializations
         will happen in this module.
*/
ds_mux_result_enum_type ds_mux_phys_port_connect
(
  boolean             is_cmux_supported,
  sio_port_id_type    sio_port_id
);

/**
  @brief .Return the status of physical port(TRUE - OPEN FALSE- CLOSE)
*/
boolean ds_mux_phys_is_port_open
(

);

/**
  @brief .RDM port open handler
*/
void ds_mux_phys_rdm_open_handler
(
  sio_port_id_type port_id
);

/**
  @brief .Return RX WMK pointer
*/
dsm_watermark_type* ds_mux_phy_get_rx_wmk
(

);

/**
  @brief .Invoke SIO transmit
*/
ds_mux_result_enum_type ds_mux_phys_transmit
( 
  dsm_item_type            *item_ptr
);

/**
  @brief .Get DTR status of physical device(UART)
*/
boolean ds_mux_phys_get_dtr_status
(

);

/**
  @brief .RDM assign port handler
*/
void ds_mux_phys_rdm_assign_port_handler
(
  rdm_assign_status_type status,
  rdm_service_enum_type  service,
  rdm_device_enum_type   device
);

/**
  @brief .InActivity timer expire handler
*/
void ds_mux_phys_inactivity_timer_expire_handler
(
  
);

/**
  @brief Start inactivity timer
*/
void ds_mux_phys_start_inactivity_timer
(

);

/**
  @brief Stop inactivity timer
*/
void ds_mux_phys_stop_inactivity_timer
(

);

/**
  @brief Invoke SIO API to flush data.
*/
boolean ds_mux_phys_flush_tx_data
(

);

/**
  @brief SIO flush handler
*/
void ds_mux_phys_sio_flush_handler
(
   void
);

/**
  @brief Invoke IOCTL to wake up Physical Device(UART)
*/
void  ds_mux_phys_invoke_wake_up_ioctl
(

);

/**
  @brief Function will check physical connection Rx WMK. Allow to enter 
         into powersave if WMK is empty
*/
boolean ds_mux_phys_is_power_save_allowed
(

);

/**
  @brief Function will de-initialize physical interface and close to UART port.
*/
void ds_mux_phys_shut_down_interface
(

);

#endif /* DS_MUX_PHYS_H */
