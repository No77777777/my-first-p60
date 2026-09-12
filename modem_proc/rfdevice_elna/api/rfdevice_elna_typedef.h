#ifndef RFDEVICE_ELNA_TYPEDEF_H
#define RFDEVICE_ELNA_TYPEDEF_H

/*=============================================================================

Copyright (c) 2014 by Qualcomm Technologies, Inc.  All Rights Reserved.

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

  $Header: //components/rel/rfdevice_elna.mpss/1.22/api/rfdevice_elna_typedef.h#1 $
  $DateTime: 2021/03/09 03:41:28 $
  $Author: pwbldsvc $

when         who   what, where, why
--------   ---   ---------------------------------------------------------------
02/17/15   vv    eLNA sleep/wakeup PM trigger support
12/03/14   vv    Initial version

=============================================================================*/

/*=============================================================================

                            INCLUDE FILES FOR MODULE

=============================================================================*/
#include "comdef.h"
#include "rfcom.h"

#include "rf_hal_bus_types.h"
#include "rf_buffer_intf.h"

/*==========================================================================*/
/* DEFINES and ENUMS  */
/*==========================================================================*/

#define RFDEVICE_ELNA_REG_ADDR_INVALID 0xFF    /*Default/invalid values for register address*/
#define RFDEVICE_ELNA_REG_DATA_INVALID -1      /*Default/invalid values for register data*/
#define ELNA_MAX_SCRIPT_SIZE           10      /*Maximum number of writes from the eLNA device*/

typedef enum
{
  RFDEVICE_ELNA_SLEEP,
  RFDEVICE_ELNA_WAKEUP,
  RFDEVICE_ELNA_GAIN,
  RFDEVICE_ELNA_INVALID,
} rfdevice_elna_req_enum_type;


/*==========================================================================*/
/* DATA STRUCTURES */
/*==========================================================================*/

typedef struct
{
  uint8 gsm[RFM_MAX_DEVICES][RFCOM_NUM_GSM_BANDS];
  uint8 tdscdma[RFM_MAX_DEVICES][RFCOM_NUM_TDSCDMA_BANDS];
  uint8 cdma[RFM_MAX_DEVICES][RFM_CDMA_MAX_BAND];
  uint8 wcdma[RFM_MAX_DEVICES][RFCOM_NUM_WCDMA_BANDS];
  uint8 lte[RFM_MAX_DEVICES][RFCOM_NUM_LTE_BANDS];
} rfdevice_elna_port_info_type;


typedef struct
{
  uint8 gsm[RFM_MAX_DEVICES][RFCOM_NUM_GSM_BANDS][RFDEVICE_MAX_RX_GAIN_STATES];
  uint8 tdscdma[RFM_MAX_DEVICES][RFCOM_NUM_TDSCDMA_BANDS][RFDEVICE_MAX_RX_GAIN_STATES];
  uint8 cdma[RFM_MAX_DEVICES][RFM_CDMA_MAX_BAND][RFDEVICE_MAX_RX_GAIN_STATES];
  uint8 wcdma[RFM_MAX_DEVICES][RFCOM_NUM_WCDMA_BANDS][RFDEVICE_MAX_RX_GAIN_STATES];
  uint8 lte[RFM_MAX_DEVICES][RFCOM_NUM_LTE_BANDS][RFDEVICE_MAX_RX_GAIN_STATES];
} rfdevice_elna_gain_info_type;

typedef struct
{
  const uint16 *addr; /* uint16 for extended addressing */
  const int16 *data;  /* int to denote invalid by -ve */
} rfdevice_elna_reg_settings_type;

typedef struct
{
  boolean extended_cmd;  /* RFFE Indicator to use extended cmd or not */
  rf_hal_bus_resource_script_settings_type settings;
} rfdevice_elna_bus_cfg_type;

typedef struct
{
  rfdevice_elna_req_enum_type req_type;
  uint8 gain_index;
  uint8 port;
} rfdevice_elna_params_cfg_type;

typedef struct
{
  /* eLNA Gain */
  uint8 elna_gain_script_size;
  uint8 elna_num_gain_states;
  rfdevice_elna_reg_settings_type elna_gain;
} rfdevice_elna_port_settings_type;

typedef struct
{
  /* Trigger */
  uint8 elna_trigger_script_size;
  rfdevice_elna_reg_settings_type elna_trigger;

  /* sleep */
  uint8 elna_sleep_script_size;
  rfdevice_elna_reg_settings_type elna_sleep;

  /* wakeup */
  uint8 elna_wakeup_script_size;
  rfdevice_elna_reg_settings_type elna_wakeup;

} rfdevice_elna_common_settings_type;


typedef struct
{
  /* Number of active ports */
  uint8 num_ports;

 /*Low power mode trigger for sleep/wakeup */  
  boolean lpm_pm_trig_flag;

  /* Pointer to port data */
  rfdevice_elna_port_settings_type port_settings;

  /* Pointer to port independent data */
  rfdevice_elna_common_settings_type common_settings;

  rfdevice_elna_bus_cfg_type elna_device_bus_cfg; 

} rfdevice_elna_settings_type;

#endif /*RFDEVICE_ELNA_TYPEDEF_H */

