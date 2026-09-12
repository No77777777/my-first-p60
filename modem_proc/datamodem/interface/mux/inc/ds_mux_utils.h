#ifndef DS_MUX_UTILS_H
#define DS_MUX_UTILS_H
/*===========================================================================

                                 D S   M U X   U T I L S

                                 H E A D E R   F I L E

DESCRIPTION
  This is the external header file for the MUX UTILS . This file
  contains all the functions, definitions and data types needed 
  for decoding and encoding MUX packets.


  Copyright (c) 2016 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/mux/inc/ds_mux_utils.h#2 $
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
#include "ds_mux_io.h"
/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/


/* Initial value of FCS according to spec.
 */
#define DS_MUX_UTIL_FCS_INIT_VAL        0xFFu

/* Number of elements in crc table 
 */
#define DS_MUX_UTIL_CRC_TABLE_ELEMENTS          256


/**
  @brief Allocate memory for I/O mux frame and return memory address 
*/
ds_mux_io_frame_struct_type* ds_mux_util_alloc_frame
(

);

/**
  @brief  Decode recevied DSM item and generate frames. 
*/
ds_mux_result_enum_type ds_mux_util_decode_input
( 
  dsm_item_type                  *item_ptr,
  ds_mux_io_rx_info              *rx_info
);

/**
  @brief Encode recevied command frame struct into DSM item  
*/
ds_mux_result_enum_type ds_mux_util_encode_cmd_input
( 
  dsm_item_type                  **item_ptr,
  ds_mux_io_frame_type            *frame

);

/**
  @brief  Encode recevied data frame struct into DSM item  
*/
ds_mux_result_enum_type ds_mux_util_encode_data_input
( 
  dsm_item_type                  **item_ptr,
  ds_mux_io_frame_type            *frame

);

/**
  @brief  Encode recevied message frame struct into DSM item  
*/
ds_mux_result_enum_type ds_mux_util_encode_msg_input
( 
  dsm_item_type            **item_ptr,
  ds_mux_io_frame_type     *frame
);

/**
  @brief Prepare command frame structure
*/
void ds_mux_util_prepare_cmd_frame
(
  uint8                       dlci,
  ds_mux_frame_type_enum_type frame_type,
  boolean                     is_command,
    boolean                   is_poll,
  ds_mux_io_frame_type       *output_frame
);

/**
  @brief Prepare data frame structure
*/
void ds_mux_util_prepare_data_frame
(
  uint8                       dlci,
  ds_mux_frame_type_enum_type frame_type,
  ds_mux_io_frame_type       *output_frame
);

/**
  @brief Convert frame type
*/
ds_mux_frame_type_enum_type ds_mux_util_convert_frame_type
( 
  ds_mux_subset_enum_type cmux_frame_type
);

/**
  @brief Payload should contain F9s to comeout from power-save
*/
ds_mux_result_enum_type ds_mux_util_process_wake_up_request
(
  dsm_item_type                  *item_ptr,
  ds_mux_io_rx_info              *rx_info
);

/**
  @brief Process first frame recevied after wake-up hand-share.
         it should be valid packet
*/
ds_mux_result_enum_type ds_mux_util_process_first_frame_after_wake_up
(
  dsm_item_type                  *item_ptr,
  ds_mux_io_rx_info              *rx_info
);

/**
  @brief Review received payload, It should contain F9s ( wake -up sequence)
         to complete wake-up hand-shake
*/
ds_mux_result_enum_type ds_mux_util_process_wake_up_response
(
  dsm_item_type                  *item_ptr,
  ds_mux_io_rx_info              *rx_info
);

#endif /* DS_MUX_UTILS_H */
