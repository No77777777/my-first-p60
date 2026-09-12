#ifndef DS_MUX_API_H
#define DS_MUX_API_H
/*===========================================================================

                                 D S   M U X   A P I 

                                 H E A D E R   F I L E

DESCRIPTION

  This file contains all the functions, definitions and data types needed 
  for other tasks to interface with DS MUX for MUX initialization

  Copyright (c) 2016 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/mux/inc/ds_mux_api.h#2 $
  $DateTime: 2019/10/22 02:53:17 $
  $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/01/16   ad      initial version
===========================================================================*/

/*===========================================================================

                      INCLUDE FILES

===========================================================================*/
#include "ds_mux.h"

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/**
  @brief Invoke Passive init API
*/
ds_mux_result_enum_type  ds_mux_passive_init
(
  dlci_cmux_param_type    *dlci_param
);

/**
  @brief Invoke Active init API
*/
ds_mux_result_enum_type  ds_mux_active_init
(
  sio_port_id_type                    sio_port_id,
  ds_mux_ext_client_event_info_type  *event_info
);

/**
  @brief Set operation mode value into CMUX structure
*/
ds_mux_result_enum_type  ds_mux_set_oprt_mode
(
  ds_mux_mode_enum_type    mode,
  dlci_cmux_param_type    *dlci_param
);

/**
  @brief Set sub set value into CMUX structure
*/
ds_mux_result_enum_type  ds_mux_set_subset
(
  ds_mux_subset_enum_type  subset,
  dlci_cmux_param_type    *dlci_param
);

/**
  @brief Set port speed value into CMUX structure
*/
ds_mux_result_enum_type  ds_mux_set_port_speed
(
  ds_mux_port_speed_enum_type    port_speed,
  dlci_cmux_param_type          *dlci_param
);

/**
  @brief Set frame size value into CMUX structure
*/
ds_mux_result_enum_type  ds_mux_set_N1
(
  uint16                   frame_size,
  dlci_cmux_param_type    *dlci_param
);

/**
  @brief Set UA ack timer value into CMUX structure
*/
ds_mux_result_enum_type  ds_mux_set_T1
(
  uint16                   acknowledgement_timer,
  dlci_cmux_param_type    *dlci_param
);

/**
  @brief Set number of re-transmissions value into CMUX structure
*/
ds_mux_result_enum_type  ds_mux_set_N2
(
  uint8                    num_re_transmissions,
  dlci_cmux_param_type    *dlci_param
);

/**
  @brief Set response timer into CMUX structure
*/
ds_mux_result_enum_type  ds_mux_set_T2
(
  uint16                   response_timer,
  dlci_cmux_param_type    *dlci_param
);

/**
  @brief Set wake-up timer into CMUX structure
*/
ds_mux_result_enum_type  ds_mux_set_T3
(
  uint16                    wake_up_timer,
  dlci_cmux_param_type     *dlci_param
);

/**
  @brief Set window size into CMUX structure
*/
ds_mux_result_enum_type  ds_mux_set_K
(
  uint8                    window_size,
  dlci_cmux_param_type    *dlci_param
);


/*===========================================================================
                      PUBLIC FUNCTION DECLARATIONS FOR LOGICAL SIO PORT
===========================================================================*/

/**
  @brief Function will be called from sio, to initialize mux driver.
         Function invoke call_back  to register all the devices associated 
         with this driver
*/

uint32 ds_mux_logical_driver_init
( 
  sio_register_device_driver_cb_type register_driver_cb_fn
);


#endif /* DS_MUX_API_H */
