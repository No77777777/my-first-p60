#ifndef FTM_DEVICE_MANAGER_H
#define FTM_DEVICE_MANAGER_H
/*!
  @file
  ftm_device_manager.h

  @brief
  This module contains FTM common Control code for NPT test.
*/

/*==============================================================================

  Copyright (c) 2015 QUALCOMM Technologies, Inc. All Rights Reserved.

  Qualcomm Proprietary

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

==============================================================================*/

/*==============================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/rfa.mpss/4.11/rf/common/ftm/inc/ftm_device_manager.h#1 $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
07/20/15   zhw     Handle G extended/ber dispatch correctly
07/15/15   zhw     Enabled FTM device manager Bypass mode support
07/08/15   zhw     FTM Device Manager SSIM/MSIM mode support
06/15/15   zhw     Provide DTA Scenario awareness API for GSM Drx control
06/11/15   aro     Fixes to coex TH/AT for AT PreSi
06/09/15   zhw     Added NPT test support for DTA scenarios
06/05/15   zhw     Remap solution for FTM RF test
06/03/15   zhw     Added support for DRX query and GSM victim query
06/01/15   zhw     Initial Revision

==============================================================================*/

#include "comdef.h"
#include "rfm_mode_types.h"
#include "ftmdiag.h"
#include "rfm_device_types.h"
#include "rfdevice_logical_component_types.h"
#include "npa.h"
#include "rfm_subscriber_types.h"
#include "ftm_rf_test_interface.h"
#include "ftm_common_control.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef PACK(struct)
{
  uint8 enable_flag;
} ftm_device_manager_mode_pkt_type;


typedef enum
{
  FTM_DEVICE_MANAGER_SUB_0,
  FTM_DEVICE_MANAGER_SUB_1,
  FTM_DEVICE_MANAGER_TECH_NUM,
} ftm_device_manager_sub_enum_type;


typedef enum
{
  FTM_DEVICE_MANAGER_SSIM_MODE,
  FTM_DEVICE_MANAGER_MSIM_MODE,
  FTM_DEVICE_MANAGER_BYPASS_MODE,
  FTM_DEVICE_MANAGER_MODE_NUM
} ftm_device_manager_mode_enum_type;


typedef struct
{
  rfm_device_enum_type prx_device;
  /*!< CDMA Driver Status */

  rfm_device_enum_type drx_device;
  /*!< CDMA Driver Status */

  sys_band_class_e_type sys_band;
  /*!< CDMA Driver Status */

} ftm_device_manager_tech_data_type;

/*----------------------------------------------------------------------------*/
/*! 
  @brief
  Structure Containing Common Global Data used by FTM Device Manager. There should not
  be stray global variables. If global variable is required, it must be 
  contained within this structure. 
*/ 
typedef struct
{
  ftm_device_manager_mode_enum_type operating_mode;
         
  boolean npt_dta_scenario;
  /*!< CDMA Driver Status */

  ftm_device_manager_tech_data_type tech_data[FTM_DEVICE_MANAGER_TECH_NUM];
  /*!< CDMA Driver Status */

  rfm_device_enum_type device_mapping[RFM_MAX_WAN_DEVICES];
  /*!< CDMA Driver Status */

} ftm_device_manager_data_type;

void
ftm_device_manager_init
(
);

void
ftm_device_manager_set_explicit_device_mapping
( 
   rfm_device_enum_type legacy_dev, 
   rfm_device_enum_type assigned_device
);

rfm_device_enum_type 
ftm_device_manager_allocate_device
(
  ftm_subsys_id_type ftm_subsys_id,
  ftm_pkt_type* ftm_cmd_ptr
);

rfm_device_enum_type
ftm_device_manager_get_assigned_device
(
  rfm_device_enum_type legacy_dev
);

rfm_device_enum_type 
ftm_device_manager_allocate_ssim_device
(
  ftm_pkt_type* ftm_cmd_ptr,
  rfm_device_enum_type legacy_dev
);

rfm_device_enum_type 
ftm_device_manager_allocate_device_concurrent_mode
(
  ftm_pkt_type* ftm_cmd_ptr,
  rfm_device_enum_type legacy_dev
);

rfm_device_enum_type 
ftm_device_manager_allocate_msim_device
(
  ftm_pkt_type* ftm_cmd_ptr
);

rfm_device_enum_type 
ftm_device_manager_get_tech_prx_device
(
  ftm_rfstate_enum_type ftm_rf_tech
);

rfm_device_enum_type 
ftm_device_manager_get_tech_drx_device
(
  ftm_rfstate_enum_type ftm_rf_tech
);

void
ftm_device_manager_override_sub0_device
(
  rfm_device_enum_type device,
  sys_band_class_e_type sys_band
);

boolean 
ftm_device_manager_is_device_npt_victim
(
   rfm_device_enum_type device  
);

boolean 
ftm_device_manager_is_device_in_dta_scenario
(
   rfm_device_enum_type device  
);

void
ftm_device_manager_set_concurrent_mode
(
   boolean enable_flag
);

ftm_rsp_pkt_type
ftm_device_manager_set_bypass_mode
(
  ftm_common_pkt_type *ftm_req_data
);

ftm_device_manager_mode_enum_type
ftm_device_manager_get_operating_mode
(
   void
);

boolean
ftm_device_manager_is_subsys_id_supported
(
   ftm_subsys_id_type ftm_subsys_id
);

boolean
ftm_device_manager_is_subsys_id_extended_format
(
   ftm_subsys_id_type ftm_subsys_id
);

rfm_device_enum_type 
ftm_common_convert_subsys_id_to_legacy_device
(
  ftm_subsys_id_type ftm_subsys_id
);

rfm_mode_enum_type
ftm_common_convert_subsys_id_to_tech
(
  ftm_subsys_id_type ftm_subsys_id
);

sys_band_class_e_type
ftm_common_convert_ftm_mode_to_sys_band
(
   ftm_mode_id_type mode
);


uint32
ftm_common_convert_ftm_mode_to_rfcom_band
(
   ftm_mode_id_type mode
);

#ifdef __cplusplus
}
#endif

#endif /* FTM_COMMON_CONTROL_H */
