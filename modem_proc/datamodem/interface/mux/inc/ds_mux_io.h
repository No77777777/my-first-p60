#ifndef DS_MUX_IO_H
#define DS_MUX_IO_H
/*===========================================================================

                                 D S   M U X   I O

                                 H E A D E R   F I L E

DESCRIPTION
  This is the external header file for the MUX IO. This file
  contains all the functions, definitions and data types needed   
  for MUX input - output processing

  Rx signal handler - Handle all incoming traffic from host
  Tx signal hanlder - Handle all outgoing traffic to host

  Copyright (c) 2016 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/mux/inc/ds_mux_io.h#2 $
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

#define DS_MUX_IO_WM_DEFAULT_LO   175000  //TBD
#define DS_MUX_IO_WM_DEFAULT_HI   350000  //TBD
#define DS_MUX_IO_WM_DEFAULT_DNE  400000  //TBD

typedef enum
{
  DS_MUX_IO_FRAME_INIT     = 0,
  DS_MUX_IO_FRAME_START_PARSED,
  DS_MUX_IO_FRAME_ADDR_PARSED,
  DS_MUX_IO_FRAME_CTRL_PARSED,
  DS_MUX_IO_FRAME_LEN_PARSED,
  DS_MUX_IO_FRAME_EXT_LEN_PARSED,
  DS_MUX_IO_FRAME_INFO_PARSED,
  DS_MUX_IO_FRAME_FCS_PARSED,
  DS_MUX_IO_FRAME_STOP_PARSED  
}ds_mux_io_parse_frame_state_enum_type;

/**
 *  A mux frame will be identified with this structure
 **/
typedef struct
{
  ds_mux_frame_type_enum_type frame_type;
  uint8                       dlci;
  uint8                       control_field;
  boolean                     command_response;
  boolean                     poll_final;
  uint16                      length;
  dsm_item_type*              information_ptr;
  uint8                       fcs;
} ds_mux_io_frame_type;


/*--------------------------------------------------------------------------
   decoded frames  queue.
---------------------------------------------------------------------------*/
typedef struct    
{
  q_link_type             link;      /* Link for queue */
  ds_mux_io_frame_type    frame;  /* pointer to frame*/
} ds_mux_io_frame_struct_type;

typedef struct
{
  q_type                                 decoded_frame_q;
  /* Queue will be holding FRAMES parsed  from last DSM items*/
  ds_mux_io_parse_frame_state_enum_type  curr_frame_state;
  /* currnet frame state */
  ds_mux_io_frame_struct_type           *curr_frame_ptr;  
  /* Current frame*/
}ds_mux_io_rx_info;


typedef struct
{
  /* Hold all the messages assosiated with DLCIN */
  ds_mux_io_rx_info   rx_info;
  /* physical port receive info */

} ds_mux_io_info_type;

/**
  @brief Initialize I/O structure pointer to NULL
*/
void ds_mux_io_init
(

);

/**
  @brief De-initialize all WMKs  and queue. Free-up memory associated 
         with un-deliver frame 
*/
void ds_mux_io_deinit
(

);

/**
  @brief Initialize all WMKs  and queue
*/
ds_mux_result_enum_type ds_mux_io_initialize_wmk
(

);

/**
  @brief Tx signal handler for outgoing data over logical interface
*/
boolean ds_mux_io_tx_sig_hdlr
(
  ds_mux_sig_enum_type      sig,
  void                    * user_data_ptr
);

/**
  @brief Rx signal handler for incoming data over physical interface
*/
boolean ds_mux_io_rx_sig_hdlr
(
  ds_mux_sig_enum_type      sig,
  void                    * user_data_ptr
);

/**
  @brief Invoke appropriate data transmission function based on 
         convergence layer 
  
*/
ds_mux_result_enum_type  ds_mux_io_transmit_data
(
  uint8               dlci_id,
  uint8               fc_flag,
  uint8               rtc_asserted_flag,
  uint8               cd_asserted_flag,
  dsm_item_type     **tx_pkt_ptr_ptr
);

/**
  @brief Encode command-response mux frame into DSM item and transmit over
         physical interface
*/
ds_mux_result_enum_type ds_mux_io_transmit_cmd_response
(
  ds_mux_io_frame_type    *frame
);

/**
  @brief Encode message-response mux frame into DSM item and transmit over
         physical interface
*/
ds_mux_result_enum_type ds_mux_io_transmit_msg_response
(
  ds_mux_io_frame_type    *frame
);

/**
  @brief Encode message-command mux frame into DSM item and transmit over
         physical interface
*/
ds_mux_result_enum_type ds_mux_io_transmit_msg_cmd
( 
  ds_mux_io_frame_struct_type           *output_frame 
);

/**
  @brief Encode command mux frame into DSM item and transmit over
         physical interface
*/
ds_mux_result_enum_type ds_mux_io_transmit_cmd
( 
  ds_mux_io_frame_struct_type           *output_frame 
);


/**
  @brief Function will prepare wake of sequnce(3 F9s) and send
         over physical interface
*/
boolean ds_mux_io_transmit_wake_up_seq
(

);

/**
  @brief Function will check mux and DLCI status and return 
         TRUE if transmission is allowed
*/
boolean ds_mux_io_is_transmission_allowed
(

);

/**
  @brief Function will de-initialize IO and free all pendig frames
*/
void ds_mux_io_shut_down
(
  void
);


#endif /* DS_MUX_IO_H */
