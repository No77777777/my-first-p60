#ifndef RFDEVICE_RFFE_COUPLER_TYPEDEF_H
#define RFDEVICE_RFFE_COUPLER_TYPEDEF_H

/*=============================================================================

Copyright (c) 2015 by Qualcomm Technologies, Inc.  All Rights Reserved.

Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. 
  Government. Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

  $Header: //components/rel/rfdevice_coupler.mpss/1.22/api/rfdevice_rffe_coupler_typedef.h#1 $
  $DateTime: 2021/03/09 03:37:09 $
  $Author: pwbldsvc $
 
when       who   what, where, why
--------   ---   ---------------------------------------------------------------
07/14/15   px    Initial version
 
=============================================================================*/
  
/*=============================================================================

                            INCLUDE FILES FOR MODULE
=============================================================================*/

#include "comdef.h"
#include "rfcom.h"
#include "rf_rffe_common.h"
#include "rf_hal_bus_types.h"
#include "rf_buffer_intf.h"
#include "rfdevice_comm_intf.h"
#include "rfdevice_coupler.h"

/*==========================================================================*/
/* DEFINES and ENUMS  */
/*==========================================================================*/

#define COUPLER_MAX_SCRIPT_SIZE 50 /* ? */
#define RFC_DEFINED_RFFE_TRANSACTION_TIME 0
#define RFDEVICE_COUPLER_MAX_REG_ADDR 0xFF
#define RFDEVICE_COUPLER_MAX_PORTS 0x20 /* Max 8 bit per RFC limitations- need to change to dynamic allocation*/

#define RFDEVICE_COUPLER_REG_INVALID -1


/*==========================================================================*/
/* DATA STRUCTURES  */
/*==========================================================================*/

typedef struct
{
  const int16** data_tbl; /* int to denote invalid by -ve */
  const uint16** reg_tbl; /* uint16 for extended addressing */
  uint8 script_size; /* Size of script */
} rfdevice_rffe_coupler_ag_settings_type;

typedef struct
{
  boolean extended_cmd;  /*!< RFFE Indicator to use extended cmd or not */
  rf_hal_bus_resource_script_settings_type settings;
} rfdevice_rffe_coupler_ag_cfg_type;

typedef struct
{
  /* Forward */
  uint8 coupler_fwd_script_size;
  rfdevice_rffe_coupler_ag_settings_type coupler_fwd;

  /* Reverse */
  uint8 coupler_rev_script_size;
  rfdevice_rffe_coupler_ag_settings_type coupler_rev;
  
  /* Open */
  uint8 coupler_open_script_size;
  rfdevice_rffe_coupler_ag_settings_type coupler_open;

} rfdevice_rffe_coupler_port_settings_type;

typedef struct
{
  /* Init */
  uint8 coupler_init_script_size;
  rfdevice_rffe_coupler_ag_settings_type coupler_init;

  /* Trigger */
  uint8 coupler_trigger_script_size;
  rfdevice_rffe_coupler_ag_settings_type coupler_trigger;
  
  /* wakeup */
  uint8 coupler_wakeup_script_size;
  rfdevice_rffe_coupler_ag_settings_type coupler_wakeup;
  
  /* sleep */
  uint8 coupler_sleep_script_size;
  rfdevice_rffe_coupler_ag_settings_type coupler_sleep;

} rfdevice_rffe_coupler_common_settings_type;


typedef struct
{ 
  /* Number of active ports */
  uint8 num_ports;
 
  /* Pointer to port data */
  rfdevice_rffe_coupler_port_settings_type port_settings_ag;

  /* Pointer to port independent data */
  rfdevice_rffe_coupler_common_settings_type common_settings_ag;

  /* Stores rf comm bus config information for the device */
  rfdevice_rffe_coupler_ag_cfg_type coupler_rfcom_bus_cfg;

} rfdevice_rffe_coupler_settings_type;

typedef struct
{
  /* Current mode config of Coupler */
  rfcom_mode_enum_type mode;
  /* Current band config of Coupler */
  rfcom_band_type_u band;
  /* gain(attenuation) to be programmed */
  rfdevice_coupling_gain_type gain;
  /* Direction to be programmed */
  rfdevice_coupling_direction_type direction;
  /* output select based on the capture type */
  rfdevice_coupler_output_type output_type;

} rfdevice_rffe_coupler_cfg_type;

typedef struct
{
  /* Default gain(attenuation)*/
  rfdevice_coupling_gain_type default_gain;
  /* Default output select based on the capture type */
  rfdevice_coupler_output_type default_output_type;
  /* Default direction for power measurements - forward or internal - to differentiate b/w TDET and HDET */
  rfdevice_coupling_direction_type default_direction;
  /* Port to select coupler and filter setting */
  uint8 port;
} rfdevice_rffe_coupler_rfc_cfg_type;

typedef struct
{
  rfdevice_rffe_coupler_rfc_cfg_type gsm[RFCOM_NUM_GSM_BANDS];
  rfdevice_rffe_coupler_rfc_cfg_type tdscdma[RFCOM_NUM_TDSCDMA_BANDS];
  rfdevice_rffe_coupler_rfc_cfg_type cdma[RFM_CDMA_MAX_BAND];
  rfdevice_rffe_coupler_rfc_cfg_type wcdma[RFCOM_NUM_WCDMA_BANDS];
  rfdevice_rffe_coupler_rfc_cfg_type lte[RFCOM_NUM_LTE_BANDS];
} rfdevice_rffe_coupler_rfc_cfg_band_map_type;

/*==========================================================================*/
/* Function Pointers  */
/*==========================================================================*/

typedef boolean (*coupler_sleep_fn_ptr)( void );
typedef boolean (*coupler_wakeup_fn_ptr)( void );
typedef boolean (*coupler_get_config_data_fn_ptr)( rfdevice_rffe_coupler_ag_settings_type* port_settings, rfdevice_rffe_coupler_cfg_type coupler_cfg);
typedef boolean (*coupler_get_gain_data_fn_ptr)( rfdevice_rffe_coupler_ag_settings_type* port_settings, rfdevice_rffe_coupler_cfg_type coupler_cfg);
typedef boolean (*coupler_get_filter_data_fn_ptr)( rfdevice_rffe_coupler_ag_settings_type* port_settings, rfdevice_rffe_coupler_cfg_type coupler_cfg);
typedef boolean (*coupler_get_output_type_data_fn_ptr)( rfdevice_rffe_coupler_ag_settings_type* port_settings, rfdevice_rffe_coupler_cfg_type coupler_cfg);

typedef struct 
{ 
  coupler_get_config_data_fn_ptr get_config_data;
  coupler_get_gain_data_fn_ptr get_gain_data;
  coupler_get_filter_data_fn_ptr get_filter_data;
  coupler_get_output_type_data_fn_ptr get_output_type_data;
}rfdevice_rffe_coupler_func_tbl_type;

#endif /*RFDEVICE_RFFE_COUPLER_TYPEDEF_H */
