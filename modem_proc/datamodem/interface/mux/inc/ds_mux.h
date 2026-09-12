#ifndef DS_MUX_H
#define DS_MUX_H
/*===========================================================================

                                 D S   M U X  

                                 H E A D E R   F I L E

DESCRIPTION
  This is the internal header file for the DS MUX . This file
  contains all the functions, definitions and data types needed for 
  MUX protocol processing

  DS MUX 27.010 core protocol and logic is declared in this file

  Copyright (c) 2016 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/mux/inc/ds_mux.h#2 $
  $DateTime: 2019/10/22 02:53:17 $
  $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/01/16   ad      initial version
===========================================================================*/

/*===========================================================================

                      INCLUDE FILES

===========================================================================*/
#include "ds_mux_defs.h"
#include "ds_mux_svc.h"
#include "ds_mux_phys.h"
#include "ds_mux_io.h"

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/



/*--------------------------------------------------------------------------
   Outstanding commands and messages queue.
---------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
 DLCI control block: Data types to hold dlci config  
---------------------------------------------------------------------------*/

typedef struct
{
   uint8                     dlci_id;
  /**< Self-reference ID   */
  
  uint32                     flow_control_mask;
  /**< Flow control mask   */
  
  ds_mux_dlci_state_enum_type state;
  /**< Currnet connection sate   */
  /*--------------------------------------------------------------------------
    DLCI Parameters, Parameters will be set through PN messages, 
    AT +CMUX command or having default values
  --------------------------------------------------------------------------*/
  ds_mux_subset_enum_type  frame_type;                         
   /**< 0-UIH, 1-UI, 2-I frames.            */
  uint8                    priority;                    
   /**< The priority is a number in the range 0-63.
      Not supported, All DLCIs will be having same priority*/
  uint16                   frame_size_N1;            
   /**< Max frame size (1-32768).           */
  uint16                   response_timer_T1;            
   /**<  Time UE waits for an acknowledgement before
   resorting to other action (e.g. transmitting a frame)
   default 100ms min value 10 ms.
 */
  uint8                     re_transmissions_N2;      
   /**< Max re-tries N2 (0-255).            */

  uint8                     curr_transmissions_N2; 
   
  uint16                    response_timer_T2;            
   /**< Time mux control channel waits before 
 re-transmitting a command default 300ms and
 min value 20 ms.
 */
  uint32                     elapse_wake_up_time_T3; 
   
  uint32                     max_wake_up_time_T3;            
   /**<  Time UE waits for response for a 
 power wake-up command default 10ms and min
 value 1 ms.
 */
  uint8                       window_size_k;                
  /**< Window size default 2, range (1-7). 
     Not supported*/

  ds_mux_cl_type_enum_type    cl_type;
  /**< Convergence layer type*/
  ds_mux_io_frame_struct_type *curr_msg_frame;
  /**< MSG is pending for this message */
  ds_mux_io_frame_struct_type *curr_cmd_frame;
   /**< UA is pending for this command */
  q_type                      outstanding_cmd_frame_q;
  /**< queue is holding outstanding commands pending to be sent */
  q_type                      outstanding_msg_frame_q;
  /**< queue is holding outstanding messages pending to be sent */

  ds_mux_timer_handle_type     t1_timer_handle;
  /* retry  timer for T1*/
  ds_mux_timer_handle_type     t2_timer_handle;
  /* retry  timer for T2*/
  ds_mux_timer_handle_type     t3_timer_handle;
  /* retry  timer for T3*/

  /**< Convergence layer type*/
  dsm_item_type                *pending_rx_cl4_data_ptr;

} ds_mux_dlci_param_type;

typedef struct 
{
  
  ds_mux_state_enum_type       ds_mux_state;
  boolean                      is_initiator;
  boolean                      is_cmux_enable;
  ds_mux_peer_info_type        peer_state_info;
}ds_mux_state_info_type;

/*---------------------------------------------------------------------------
  Request and Release call back declarations for MUX.
---------------------------------------------------------------------------*/
typedef void (*ds_mux_event_cb_fcn)
(
  ds_mux_ext_client_event_enum_type event_name,     /* Event name  */
  void *                            event_payload,  /* Info assosiated to event: If Any else NULL  */
  void *                            cb_user_data    /* User data specified during registration */
);

/*--------------------------------------------------------------------------
  External client event information.
---------------------------------------------------------------------------*/
typedef struct
{
  ds_mux_event_cb_fcn   event_cb_fn;
  void                 *client_data;
}ds_mux_ext_client_event_info_type;


/*--------------------------------------------------------------------------
  External client event and device einformation.
---------------------------------------------------------------------------*/
typedef struct
{
  sio_port_id_type                     sio_port_id;
  ds_mux_ext_client_event_info_type   *event_info;
}ds_mux_ext_client_info_type;


typedef enum
{
  DS_MUXI_MIN_CMD               = -1,
  DS_MUXI_PHY_PORT_OPEN         =  0,
  DS_MUXI_LOGICAL_PORT_OPEN     =  1,
  DS_MUXI_LOGICAL_PORT_CLOSE    =  2,
  DS_MUXI_LOGICAL_PORT_IOCTL    =  3,
  DS_MUXI_PHY_PORT_ASSIGN       =  4,
  DS_MUXI_T1_TIMER_EXPIRE       =  5,
  DS_MUXI_T2_TIMER_EXPIRE       =  6,
  DS_MUXI_T3_TIMER_EXPIRE       =  7,
  DS_MUXI_INACTIVITY_TIMER_EXPIRE  =  8,  
  DS_MUXI_PHY_SIO_FLUSH         =  9,
  DS_MUXI_LOOP_BACK_TEST        =  10,
  DS_MUXI_SSR_SHUT_DOWN         =  11,
  DS_MUXI_SSR_POWER_UP          =  12,

  /* Always must be last */
  DS_MUXI_MAX_CMD_ID
} ds_muxi_cmd_id_enum_type;

/*--------------------------------------------------------------------------
  DS MUX  PHY PORT RDM open call back paramters
---------------------------------------------------------------------------*/
typedef struct
{
  sio_port_id_type port_id;
}ds_mux_phys_rdm_open_info_type;

/*--------------------------------------------------------------------------
  DS MUX  PHY PORT RDM Assign call back paramters
---------------------------------------------------------------------------*/
typedef struct
{
  rdm_assign_status_type status;
  rdm_service_enum_type  service;
  rdm_device_enum_type   device;
}ds_mux_phys_rdm_assign_info_type;

/*--------------------------------------------------------------------------
  DS MUX  PHY PORT RDM open call back paramters
---------------------------------------------------------------------------*/
typedef struct
{
  uint8  dlci_id;
}ds_mux_logical_open_info_type;
/*--------------------------------------------------------------------------
  DS MUX close  paramters
---------------------------------------------------------------------------*/
typedef struct
{
  uint8  dlci_id;
}ds_mux_logical_close_info_type;

/*--------------------------------------------------------------------------
  DS MUX  IOCTL  paramters
---------------------------------------------------------------------------*/
typedef struct
{
  uint8                     dlci_id;
  boolean                   data_valid;
  boolean                   rtc;
  boolean                   fc;
  boolean                   ic;
}ds_mux_logical_ioctl_info_type;

/*--------------------------------------------------------------------------
  DS MUX  Timer Expire  Call back info
---------------------------------------------------------------------------*/
typedef struct
{
  uint8                     dlci_id;
}ds_mux_timer_expire_cb_info_type;

/*--------------------------------------------------------------------------
  DS MUX  Test App Timer Expire  Call back info
---------------------------------------------------------------------------*/
typedef struct
{
  uint8                     client_id;
}ds_mux_test_app_timer_cb_info_type;


/*--------------------------------------------------------------------------
  DS MUX command info type
---------------------------------------------------------------------------*/
typedef struct
{
  ds_muxi_cmd_id_enum_type    cmd_id;

  union
  {
    ds_mux_phys_rdm_open_info_type    phy_port_open_info;
    ds_mux_phys_rdm_assign_info_type  phy_assign_port_info;
    ds_mux_logical_open_info_type     logical_port_open_info;
    ds_mux_logical_close_info_type    logical_port_close_info;
    ds_mux_logical_ioctl_info_type    logical_port_ioctl_info;
    ds_mux_timer_expire_cb_info_type  timer_expire_cb_info;
    ds_mux_test_app_timer_cb_info_type test_app_timer_cb_info;
  } cmd_info;

} ds_muxi_cmd_info_type;


/*===========================================================================
                      PUBLIC FUNCTION DECLARATIONS
===========================================================================*/

/**
  @brief Initialize MUX module. Read NV MUX configuration. Invoke
         internal module initilization(logical, physical, IO)
*/
void ds_muxi_init
(

);
/**
  @brief De-initialize MUX module. Invoke internal module 
         de-initilization(logical, physical, IO). Free memory and timers
*/
void ds_muxi_deinit
(

);

/**
  @brief Initialize CMUX paramters. AT+CMUX command is defined in 
         spec 27.007
*/
ds_mux_result_enum_type   ds_mux_initialize_cmux_param
(
  dlci_cmux_param_type    *cmux_param
);

/**
  @brief Function will store/set CMUX parameters and 
         Allocate the memory for internal modules.
*/
ds_mux_result_enum_type  ds_muxi_passive_init
(
  dlci_cmux_param_type    *dlci_param
);

/**
  @brief Invoke connect to sio port provided by client.
         Activate the MUX functionality 
*/
ds_mux_result_enum_type  ds_muxi_active_init
(
  boolean                            is_cmux_supported,
  sio_port_id_type                   sio_port_id,
  ds_mux_ext_client_event_info_type *event_info
);
 
 /**
   @brief Internal call back function when DS MUX initialized at bootup
          and +CMUX AT command initialization is disabled 
 */
 void ds_mux_bringup_call_back_fn
(
  ds_mux_ext_client_event_enum_type event_name,
  void *                            event_payload,
  void *                            cb_user_data
);
 
 /**
   @brief MUX Brinup function, Function will be invoked at
          power up init.
 */
ds_mux_result_enum_type ds_muxi_bringup
(

);
 
 /**
   @brief Set client parameters
 */
ds_mux_result_enum_type  ds_mux_set_client_param
(
  sio_port_id_type                   sio_port_id,
  ds_mux_ext_client_event_info_type *event_info
);

/**
  @brief Get current MUX state
*/
ds_mux_state_enum_type ds_mux_get_mux_state
(

);

/**
  @brief Set current MUX state
*/
void ds_mux_set_mux_state
(
  ds_mux_state_enum_type  state
);

/**
  @brief Get current MUX baud rate
*/
sio_bitrate_type ds_mux_get_baud_rate
(
  
);

/**
  @brief Return TRUE when MUX is initiator mode
*/
ds_mux_state_enum_type ds_mux_is_initiator
(

);

/**
  @brief Get current DLCI convergence type
*/
ds_mux_cl_type_enum_type ds_mux_get_cl_type
(
  uint8                       dlci
);

/**
  @brief Set MUX initiator mode
*/
void ds_mux_set_is_initiator
(
  boolean  is_initiator
);

/**
  @brief Get signal ID from DLCI ID
*/
ds_mux_sig_enum_type ds_mux_get_sig_by_id
(
  uint32  dlci_id
);

/**
  @brief Get message type from recevied message byte(Message byte contain EA bit)
*/
ds_mux_result_enum_type ds_mux_get_msg_type 
(
  byte                          msg_byte,
  ds_mux_msg_type_enum_type    *msg_type
);

/**
  @brief Get connection pointer corresponding to DLCI ID
*/
ds_mux_dlci_param_type* ds_mux_get_dlci_conn_ptr
(
  uint32                   dlci_id 
);

/**
  @brief Validate logical connection
*/
boolean  ds_mux_is_conn_valid
(
  uint8                           dlci_id
);

/**
  @brief Return TRUE when MUX initialization mode enable
*/
boolean ds_mux_is_cmux_initialization_enable
(

);

/**
  @brief Set +CMUX initialization mode
*/
void ds_mux_set_cmux_initialization
(
  boolean   cmux_init_flag
);

/**
  @brief Update physical port open status
*/
void ds_mux_update_phy_port_open_status
(
  ds_mux_result_enum_type  port_open_status
);


/**
  @brief process control frames. 
*/
ds_mux_result_enum_type ds_mux_process_dlci_rx_ctl_frames
(
  ds_mux_io_frame_type    *frame
);

/**
  @brief Process incoming frag data frames. Assembled or discard the current frame
         based on incoming frames 
*/
ds_mux_result_enum_type ds_mux_process_rx_frag_data_frames
(
  ds_mux_io_frame_type    *frame
);

/**
  @brief Prepare MSC message frame and send it on DLCI 0
*/
void ds_mux_prepare_and_send_msc_message
(
  uint8           dlci_id,
  uint8           fc_flag,
  uint8           rtc_asserted_flag,
  uint8           cd_asserted_flag,
  uint8           ic_asserted_flag,
  uint8           rtr_asserted_flag
);

/**
  @brief Prepare PSC message frame and send it on DLCI 0.
*/
boolean ds_mux_prepare_and_send_psc_msg
(

);

/**
  @brief Prepare UA MUX frame based on input parameters.
*/
void ds_mux_prepare_ua_response
(
  ds_mux_io_frame_type    *input_frame,
  ds_mux_io_frame_type    *output_frame
);

/**
  @brief Prepare SABM MUX frame based on input parameters.
*/
void ds_mux_prepare_sabm_frame
(
  uint8                    dlci_id,
  boolean                  is_cmd,
  ds_mux_io_frame_type    *output_frame
);

/**
  @brief Prepare DISC  MUX frame based on input parameters.
*/
void ds_mux_prepare_disc_frame
(
  uint8                    dlci_id,
  boolean                  is_cmd,
  ds_mux_io_frame_type    *output_frame
);

/**
  @brief Prepare DM MUX frame based on input parameters.
*/
void ds_mux_prepare_dm_frame
(
  uint8                    dlci_id,
  boolean                  is_cmd,
  ds_mux_io_frame_type    *output_frame
);

/**
  @brief Disable flow mask(Set flow mask bit)
*/
void  ds_mux_disable_flow_mask
(
  uint8                    dlci_id,
  ds_mux_flow_ctl_mask_enum_type fc_msk
);

/**
  @brief Enable flow mask(Reset flow mask bit)
*/
void  ds_mux_enable_flow_mask
(
  uint8                          dlci_id,
  ds_mux_flow_ctl_mask_enum_type fc_msk
);

/**
  @brief Return TRUE if flow is enabled
*/
boolean   ds_mux_is_flow_enabled
(
  uint8                           dlci_id
);

/**
  @brief Process DLCI disconnect. Notify logical interface about
         DLCI disconnect. Free DLCI connection pointer 
*/
void ds_mux_disconnect_dlci
(
  ds_mux_dlci_param_type  *dlci_conn
);

/**
  @brief Handle T1 timer expire. Send pending command  again. 
*/
void ds_mux_t1_timer_handler
(
  uint8         dlci_id
);

/**
  @brief Handle T2 timer expire. Send pending command message again. 
*/
void ds_mux_t2_timer_handler
(
  uint8         dlci_id
);

/**
  @brief Handle T3 timer expire. Send wake-up request again. 
*/
void ds_mux_t3_timer_handler
(
  uint8         dlci_id
);

/**
  @brief Return true when Physical Rx WM, Tx Data WMs, Command and Message 
         queue for each DLCI.
         
*/
boolean ds_mux_start_power_save_routine
(

);

/**
  @brief Check Physical Rx WM, Tx Data WMs, Command and Message queue for each DLCI.
         Move into power save state if it is allowed
*/
boolean ds_mux_complete_power_down
(

);

/**
  @brief Invoke physical wake-up ioctl.Prepare wake-up sequence and send to remote MUX
*/
void ds_mux_invoke_wake_up_routine
(

);

/**
  @brief Set MUX state to opened, Notify all DLCI to transmit the data.
         Start inactivity timer once we do first transmition
*/
void ds_mux_complete_wake_up
(

);

/**
  @brief Check Physical Rx WM, Tx Data WMs, Command and Message queue for each DLCI.
         Enter into power save(Prepare and send power save message) If it is allowed. 
*/
void ds_mux_enter_into_power_save
(

);

/**
  @brief DS MUX SSR shut down handler 
*/
void ds_mux_ssr_shut_down_handler
(

);

/**
  @brief DS MUX SSR Power up handler 
*/
void ds_mux_ssr_power_up_handler
(

);

/**
  @brief Check if SSR is in progress. 
*/
boolean ds_mux_is_ssr_shut_down_in_progress
(

);

/**
  @brief Shut down handler
*/
void ds_mux_ssr_shut_down_handler
(

);

/**
  @brief Set Peer MUX state
*/
void  ds_mux_set_peer_state
(
  ds_mux_peer_state_enum_type  peer_state
);

/**
  @brief Get Peer MUX state
*/
ds_mux_peer_state_enum_type ds_mux_get_peer_state
(

);


#endif /* DS_MUX_H */
