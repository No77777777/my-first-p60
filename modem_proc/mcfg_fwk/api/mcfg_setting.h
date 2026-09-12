/*===========================================================================
                        mcfg_setting.h

DESCRIPTION

   MCFG setting for the MCFG module
 
Copyright (c) 2012-2015 by QUALCOMM Technologies Incorporated.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/mcfg_fwk.mpss/7.8.1.1/api/mcfg_setting.h#1 $ $DateTime: 2021/04/08 00:04:19 $ $Author: pwbldsvc $

when         who   what, where, why
----------   ---   ---------------------------------------------------------
11/2/15      cys   Create

===========================================================================*/

#ifndef MCFG_SETTING_H
#define MCFG_SETTING_H

#include "comdef.h"
#include "mcfg_common.h"
#include "persistent_device_configuration_v01.h"

/*==============================================================================
  Constants and Macros
==============================================================================*/

#ifdef FEATURE_MCFG_FEATURETAG

#define MCFG_IMS_FEAT_VOLTE_BIT   0
#define MCFG_IMS_FEAT_VOWIFI_BIT  1
#define MCFG_IMS_FEAT_HVOLTE_BIT  2

#endif /* FEATURE_MCFG_FEATURETAG */
/*==============================================================================
  Public defines & Typedefs
==============================================================================*/

typedef enum mcfg_setting_store_e {
  MCFG_SETTING_MIN = 0,
  MCFG_SETTING_CURR = 0,
  MCFG_SETTING_BKUP,
  MCFG_SETTING_MAX
} mcfg_setting_store_e_type;

/*==============================================================================
  Public Function Prototypes
==============================================================================*/

/*===========================================================================

  FUNCTION mcfg_set_feature

  DESCRIPTION
    Process the change request of feature setting for specified slot index

  DEPENDENCIES
    None

  PARAMETERS
    feature      [in]  feature setting for specified slot index

  RETURN VALUE
    mcfg_error_e_type mcfg error code

  SIDE EFFECTS
    None

===========================================================================*/
mcfg_error_e_type mcfg_set_feature
(
  pdc_set_feature_req_msg_v01 *feature
);

/*===========================================================================

FUNCTION mcfg_setting_space_limit

DESCRIPTION
  return EFS space limit allowed for MCFG in kilobytes, 0 means not configured
  
DEPENDENCIES
  None.

RETURN VALUE
  uint16

SIDE EFFECTS

===========================================================================*/
uint16 mcfg_setting_space_limit
(
  void
);

/*===========================================================================

FUNCTION mcfg_setting_remote_discover

DESCRIPTION
  return remote auto discover feature is enabled or not
  
DEPENDENCIES
  None.

RETURN VALUE
  boolean

SIDE EFFECTS

===========================================================================*/
boolean mcfg_setting_remote_discover
(
  void
);

#ifdef FEATURE_MCFG_FEATURETAG
/*===========================================================================

  FUNCTION mcfg_ims_feature_update

  DESCRIPTION
    Modifies the IMS feature bitmask 

  DEPENDENCIES
    None

  PARAMETERS
    ims_feat    ims_feature variable
    b_val       bit value to apply in ims_feat
    b_pos       position of bit to be modified

  RETURN VALUE

  SIDE EFFECTS
    None

===========================================================================*/
uint16 ims_feat_update
(
   uint16   ims_feat, 
   uint8    b_val, 
   uint16   b_pos
);

/*===========================================================================

  FUNCTION mcfg_settings_updated

  DESCRIPTION
    Confirms whether mcfg_settings file is different from mcfg_settings_orig

  DEPENDENCIES
    None

  PARAMETERS
    sloti - slot index

  RETURN VALUE

  SIDE EFFECTS
    None

===========================================================================*/
boolean mcfg_settings_updated(uint8 sloti);

/*===========================================================================

  FUNCTION mcfg_setting_backup

  DESCRIPTION
    Backup current set of mcfg_setting values

  DEPENDENCIES
    None

  PARAMETERS
    sloti - slot index 
    store - category of backup file

  RETURN VALUE

  SIDE EFFECTS
    None

===========================================================================*/
void mcfg_setting_backup(uint8 slot_id, mcfg_setting_store_e_type store);

/*===========================================================================

  FUNCTION mcfg_get_feature

  DESCRIPTION
    Queries the change request of feature setting for specified slot index.
    Return either current or previous feature settings based on feat_store input.

  DEPENDENCIES
    None

  PARAMETERS
    Sloti - Slot index used to access original mcfg_setting values
    Feat_store - context used to identify which mcfg_settings should be read
    feature - buffer to store setting values

  RETURN VALUE
    mcfg_setting_s_type - Original mcfg_setting values for sloti

  SIDE EFFECTS
    None

===========================================================================*/
mcfg_error_e_type mcfg_get_feature
(
  mcfg_slot_index_type_e_type sloti, 
  mcfg_setting_store_e_type feat_store, 
  pdc_get_feature_ind_msg_v01 *feature
);

/*===========================================================================

FUNCTION mcfg_setting_update_feature

  DESCRIPTION
  update feature for country code and volte

  DEPENDENCIES
  None.

  RETURN VALUE
  boolean True when updated successfully, false if invalid or there's no change

  SIDE EFFECTS

===========================================================================*/
boolean mcfg_setting_update_feature
(
  mcfg_slot_index_type_e_type sloti, 
  uint16    country,
  uint16    ims_feature
);
#else 
/*===========================================================================

  FUNCTION mcfg_get_feature

DESCRIPTION
    Queries the change request of feature setting for specified slot index
  
DEPENDENCIES
    None

  PARAMETERS
    sloti        [in]  specified slot index
    feature      [in]  pointer of feature settings

RETURN VALUE
    mcfg_error_e_type mcfg error code

SIDE EFFECTS
    None

===========================================================================*/
mcfg_error_e_type mcfg_get_feature
(
  mcfg_slot_index_type_e_type sloti, 
  pdc_get_feature_ind_msg_v01 *feature
);

#endif /* FEATURE_MCFG_FEATURETAG */
#endif /* MCFG_SETTING_H */
