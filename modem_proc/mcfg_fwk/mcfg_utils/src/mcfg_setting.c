/*==============================================================================

          MODEM_CFG  -   S E T T I N G

GENERAL DESCRIPTION
  Simple APIs to access ModemCfg-specific NV settings.  Supports both new-style 
  MCFG NV items and old-style NV items, though it is recommended that 
  both exisiting and new code accessing old-style items use the legacy NV API
  when possible.  

EXTERNALIZED FUNCTIONS
  mcfg_nv_read
  mcfg_nv_write

INITIALIZATION AND SEQUENCING REQUIREMENTS
  EFS must be initialized

Copyright (c) 2012-2013 by QUALCOMM Technologies Incorporated.
==============================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/mcfg_fwk.mpss/7.8.1.1/mcfg_utils/src/mcfg_setting.c#1 $
$DateTime: 2021/04/08 00:04:19 $
$Author: pwbldsvc $
$Change: 30003610 $

when     who  what, where, why
-------- ---  ---------------------------------------------------------------- 
01/09/14 sk   Fix for CR596963
02/14/12 sbt  Initial creation

==============================================================================*/

/*==============================================================================

                      INCLUDE FILES FOR MODULE

==============================================================================*/

#include "mcfg_feature_config.h"
#include "mcfg_common.h"
#include "mcfg_nv.h"
#include "mcfg_osal.h"
#include "mcfg_int.h"

#include "mcfg_setting.h"
#include "persistent_device_configuration_v01.h"
#include "mcfg_uim.h"
#include "mcfg_utils_i.h"
#include "mcfg_task.h"
#include "mcfg_sel.h"


/*==============================================================================

                LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

==============================================================================*/

/*==============================================================================
  Private Constants and Macros
==============================================================================*/
#ifdef MCFG_TARGET_EFS_LIMIT_SETTING
#define MCFG_SETTING_DEFAULT_SPACE_LIMIT (MCFG_TARGET_EFS_LIMIT_SETTING)
#else
#define MCFG_SETTING_DEFAULT_SPACE_LIMIT (256)
#endif
#define MCFG_SETTING_CURR_VER (2)

/*==============================================================================
  Private Typedefs
==============================================================================*/
/* MCFG Setting based on Slot
*/
typedef PACKED struct PACKED_POST
{
  /* Reserved for consistency between old and new-style indexed items */
  uint8     index;

  /* Struct version */
  uint32    version;

  /* Selection Reserved */
  uint8     selection_mode;
  uint8     flex_mapping;

  /* openmkrt subsidized carrier*/
  uint8     subsidized_carrier;
  uint8     selection_reserved;

  /* Refresh*/
  uint8     refresh_mode;
  uint8     refresh_timer;
  uint8     refresh_debug;
  uint8     refresh_reserved;

  /* SSR*/
  uint8     ssr_mode;
  uint8     ssr_timer;
  uint8     ssr_retries;
  uint8     ssr_retry_timer;

  /* storage */
  uint16    space_limit;    // EFS space usage limit in kilo bytes
  uint8     remote_storage; // remote storage discover feature 1:enabled 0:Disabled

  /* New Feature Tags */
  /* HW specific features */
  uint8     multisim;    // for differentiating SS / DR_DSDS / SR_DSDS 
  uint8     os;          // WIN vs LA

  /* SW specific features */ 
  uint8     market;      // automotive vs ...? <-- room for extension needed?
  uint8     deployment;  // commercial is active low: alternate: enum named deployment: room for extension needed? 
  uint16    ims_feature; // bitmask to be used for multiple IMS related features, e.g. volte, vowifi, etc
  uint16    country;     // Enum for all countries available 
  uint16    variant;     // Miscellaneous tag for user defined variants
  uint16    technology;  // Tag available to differentiate various technologies

  /* Reserved */
  uint8     Reserved[10];

}mcfg_setting_s_type;


typedef enum mcfg_features_e_type{
  MCFG_SETTINGS_MIN = 0,
  CARRIER_FEAT = MCFG_SETTINGS_MIN,
  SELECTION_MODE_FEAT,
  FLEX_MAPPING_FEAT,
  REFRESH_MODE_FEAT,
#ifdef FEATURE_MCFG_FEATURETAG                                                                                                                
  MULTISIM_FEAT,
  OS_FEAT,
  MARKET_FEAT,
  DEPLOYMENT_FEAT,
  IMS_FEATURE_FEAT,
  COUNTRY_FEAT,
  VARIANT_FEAT,
#endif
  MCFG_SETTINGS_MAX
}mcfg_features_e_type;

typedef struct mcfg_feat_map_s_type{
  mcfg_features_e_type feat;
  uint32 pdc_data_offset;
  uint32 pdc_token_offset;
  uint32 mcfg_data_offset;
  uint16 data_len;
  mcfg_config_type_e_type category;
} mcfg_feat_map_s_type;

/*===========================================================================

  MACRO
    ADD_TABLE_ENTRY

  DESCRIPTION
    This macro aids in adding new features to the feature table

  DEPENDENCIES
    None

  ARGUMENTS
    feature - Feature name 
    pdc_feat_name - Name of feature member in pdc_set_feature_req
    mcfg_feat_name - Name of feature member in mcfg_setting
    config_type - Config type associated with this feature

  SIDE EFFECTS
    None

===========================================================================*/
#define ADD_TABLE_ENTRY(feature, pdc_feat_name, mcfg_feat_name, config_type ) \
  { feature, \
    FPOS(pdc_set_feature_req_msg_v01, pdc_feat_name), \
    FPOS(pdc_set_feature_req_msg_v01, pdc_feat_name##_valid), \
    FPOS(mcfg_setting_s_type, mcfg_feat_name), \
    FSIZ(mcfg_setting_s_type, mcfg_feat_name), \
    config_type \
  }


/*===========================================================================

  MACRO
    ADD_TABLE_ENTRY

  DESCRIPTION
    This table lists offset data for features existing in both pdc_feat_feature
    and mcfg_setting. Features listed only in mcfg_setting are not included in
    this table. 

  DEPENDENCIES
    None

  ARGUMENTS
    feature - Feature name 
    pdc_feat_name - Name of feature member in pdc_set_feature_req
    mcfg_feat_name - Name of feature member in mcfg_setting
    config_type - Config type associated with this feature

  NOTES
    Update mcfg_features_e_type before including new features in the table below

===========================================================================*/
mcfg_feat_map_s_type mcfg_feat_table [] = {
 /*               feature name,        pdc member name, mcfg member name,  config type */                  
 ADD_TABLE_ENTRY( CARRIER_FEAT,        carrier,        subsidized_carrier, MCFG_TYPE_HW ),
 ADD_TABLE_ENTRY( SELECTION_MODE_FEAT, selection_mode, selection_mode,     MCFG_TYPE_SW ),
 ADD_TABLE_ENTRY( FLEX_MAPPING_FEAT,   flex_mapping,   flex_mapping,       MCFG_TYPE_SW ),
 ADD_TABLE_ENTRY( REFRESH_MODE_FEAT,   refresh_mode,   refresh_mode,       MCFG_TYPE_SW ),
                                                                                         
#ifdef FEATURE_MCFG_FEATURETAG                                                           
 ADD_TABLE_ENTRY( MULTISIM_FEAT,       multisim,       multisim,           MCFG_TYPE_HW ),
 ADD_TABLE_ENTRY( OS_FEAT,             os,             os,                 MCFG_TYPE_HW ),
 ADD_TABLE_ENTRY( MARKET_FEAT,         market,         market,             MCFG_TYPE_SW ),
 ADD_TABLE_ENTRY( DEPLOYMENT_FEAT,     deployment,     deployment,         MCFG_TYPE_SW ),
 ADD_TABLE_ENTRY( IMS_FEATURE_FEAT,    ims_feature,    ims_feature,        MCFG_TYPE_SW ),
 ADD_TABLE_ENTRY( COUNTRY_FEAT,        country,        country,            MCFG_TYPE_SW ),
 ADD_TABLE_ENTRY( VARIANT_FEAT,        variant,        variant,            MCFG_TYPE_SW )
#endif 
};

/*==============================================================================
  Private Variables
==============================================================================*/
mcfg_setting_s_type mcfg_setting[MCFG_NUM_OF_UIM_SLOT];

#ifdef FEATURE_MCFG_FEATURETAG
static mcfg_setting_s_type mcfg_setting_orig[MCFG_NUM_OF_UIM_SLOT];
#endif
/*==============================================================================
  Private Function Prototypes
==============================================================================*/

void mcfg_feature_print(mcfg_slot_index_type_e_type sloti);
/*==============================================================================

                    FUNCTION DECLARATIONS FOR MODULE

==============================================================================*/


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
)
{
  uint16 mask = 0; 


#ifdef MCFG_DEBUG
  MCFG_MSG_MED_3("ims_feat_update() args - ims_feat: %d, b_val=%d b_pos=%d", 
                 ims_feat, b_val, b_pos);
#endif

  mask = (1 << b_pos) & 0xFFFF; 
  ims_feat = (ims_feat & ~mask) | (b_val << b_pos);

#ifdef MCFG_DEBUG
  MCFG_MSG_MED_2("ims_feat_update() result - ims_feat: %d, mask=0x%04X", 
                 ims_feat, mask);
#endif
  
  return ims_feat; 
}

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
boolean mcfg_settings_updated(uint8 sloti) 
{
  mcfg_nv_status_e_type nv_ret;
  boolean status = TRUE;
  mcfg_nv_status_e_type write_status; 

  uint8 index_sz = sizeof(uint8);
  uint8 *pCurr;
  uint8 *pOrig;

  MCFG_ASSERT_COND_RET_FALSE(sloti < MCFG_NUM_OF_UIM_SLOT);
  pCurr = (uint8 *)&mcfg_setting[sloti] + index_sz;
  pOrig = (uint8 *)&mcfg_setting_orig[sloti] + index_sz;

  nv_ret = mcfg_nv_read(MCFG_NV_ITEM_SETTING, MCFG_SETTING_BKUP, sloti, 
                        &mcfg_setting_orig[sloti], sizeof(mcfg_setting_s_type));
  if(nv_ret != MCFG_NV_STATUS_OK)
  {
    MCFG_MSG_MED_1("Failed to read mcfg setting backup for slot %d", sloti);
  }
  else if(memcmp(pCurr, pOrig, sizeof(mcfg_setting_s_type) - sizeof(uint8)))
  {
    MCFG_MSG_MED_1("mcfg_setting updated for slot %d", sloti);
#ifdef MCFG_DEBUG
    mcfg_utils_print_buffer(pOrig, sizeof(mcfg_setting_s_type) - index_sz, "original setting");
    mcfg_utils_print_buffer(pCurr, sizeof(mcfg_setting_s_type) - index_sz, "current setting");
#endif

    /* mcfg_setting can be changed outside of pdc_set_feature, e.g. country / IMS feature
       updated via mcfg_setting_update_feature(). In such cases write latest changes
       to MCFG_SETTING_CURR to ensure file is kept up to date */
    write_status = mcfg_nv_write(MCFG_NV_ITEM_SETTING, MCFG_SETTING_CURR, sloti,
                   &mcfg_setting[sloti], sizeof(mcfg_setting_s_type));
    mcfg_feature_print(sloti);
  }
  else 
  {
    MCFG_MSG_MED("mcfg_setting unchanged");
    status = FALSE;
  }

  return status;
}

/*===========================================================================

  FUNCTION mcfg_setting_backup

  DESCRIPTION
    Backup current set of mcfg_setting values

  DEPENDENCIES
    None

  PARAMETERS
    sloti - slot index 

  RETURN VALUE

  SIDE EFFECTS
    None

===========================================================================*/
void mcfg_setting_backup(uint8 slot_id, mcfg_setting_store_e_type store) 
{
  mcfg_nv_status_e_type status;

  MCFG_ASSERT_COND_RET(slot_id < MCFG_NUM_OF_UIM_SLOT);

#ifdef MCFG_DEBUG
  MCFG_MSG_MED_2("Backing up mcfg_setting to store# %d, slot %d:", store, slot_id);
  mcfg_utils_print_buffer((uint8 *) &mcfg_setting[slot_id], sizeof(mcfg_setting_s_type) - sizeof(uint8), "mcfg_setting");
#endif /* MCFG_DEBUG */

  status = mcfg_nv_write(MCFG_NV_ITEM_SETTING, store, slot_id,
                &mcfg_setting[slot_id], 
                sizeof(mcfg_setting_s_type));

  if (status != MCFG_NV_STATUS_OK)
  {
    MCFG_MSG_ERROR_3("Couldn't write store %d of mcfg_settings for slot %d: status %d",
                     store, slot_id, status);
  }
  else
  {
    MCFG_MSG_MED_3("Saved store %d of mcfg_settings for slot %d: status %d",
                     store, slot_id, status);
  }
}

/*===========================================================================

  FUNCTION mcfg_feat_categories_updated

  DESCRIPTION
    Determine whether feature belonging to specific category has been updated

  DEPENDENCIES
    None

  PARAMETERS
   pdc_set_feature_req_msg_v01 *feature -- set feature req
   mcfg_config_type_e_type category -- related MBN type feature affects

  RETURN VALUE
    boolean

  SIDE EFFECTS
    None

===========================================================================*/
boolean mcfg_feat_categories_updated
(
   pdc_set_feature_req_msg_v01 *feature
)
{
  uint8 *valid_token;
  boolean status = FALSE;
  boolean mcfg_hw_setting = FALSE;
  boolean mcfg_sw_setting = FALSE;
  int setting;
     
  for (setting = MCFG_SETTINGS_MIN; setting < MCFG_SETTINGS_MAX; setting++)
  {
    valid_token = (uint8 *)feature + mcfg_feat_table[setting].pdc_token_offset; 
    if (*valid_token == TRUE)
    {
      if (mcfg_feat_table[setting].category == MCFG_TYPE_SW)
      {
        MCFG_MSG_MED("setting mcfg_sw_setting to true");
        mcfg_sw_setting = TRUE;
      }
      else if (mcfg_feat_table[setting].category == MCFG_TYPE_HW)
      {
        if (setting != CARRIER_FEAT || feature->carrier != PDC_SELECTION_OPEN_MARKET_V01)
        {
          MCFG_MSG_MED("setting mcfg_hw_setting to true");
          mcfg_hw_setting = TRUE;
        }
      }
    }
  }

  if (mcfg_hw_setting && mcfg_sw_setting)
  {
    status = TRUE;
    MCFG_MSG_MED("mcfg_feat_categories_updated: more than 1 category updated in single set_feature req");
  }
  MCFG_MSG_LOW_1("mcfg_feat_categories_updated status = %d", status); 

  return status;
}

#endif

void mcfg_feature_print
(
  mcfg_slot_index_type_e_type sloti
)
{
  MCFG_ASSERT_COND_RET(sloti < MCFG_NUM_OF_UIM_SLOT);

  MCFG_MSG_LOW_1("mcfg_setting.sloti %x", sloti);
  MCFG_MSG_LOW_1("mcfg_setting.version %x", mcfg_setting[sloti].version);
  MCFG_MSG_LOW_1("mcfg_setting.selection_mode %d", mcfg_setting[sloti].selection_mode);
  MCFG_MSG_LOW_1("mcfg_setting.flex_mapping %d", mcfg_setting[sloti].flex_mapping);
  MCFG_MSG_LOW_1("mcfg_setting.carrier %d", mcfg_setting[sloti].subsidized_carrier);
  MCFG_MSG_LOW_1("mcfg_setting.refresh_mode %d", mcfg_setting[sloti].refresh_mode);
  MCFG_MSG_LOW_1("mcfg_setting.refresh_timer %d", mcfg_setting[sloti].refresh_timer);
  MCFG_MSG_LOW_1("mcfg_setting.refresh_debug %d", mcfg_setting[sloti].refresh_debug);
  MCFG_MSG_LOW_1("mcfg_setting.ssr_mode %x", mcfg_setting[sloti].ssr_mode);
  MCFG_MSG_LOW_1("mcfg_setting.ssr_timer %x", mcfg_setting[sloti].ssr_timer);
  MCFG_MSG_LOW_1("mcfg_setting.ssr_retries %x", mcfg_setting[sloti].ssr_retries);
  MCFG_MSG_LOW_1("mcfg_setting.ssr_retry_timer %x", mcfg_setting[sloti].ssr_retries);
  MCFG_MSG_LOW_1("mcfg_setting.space_limit %x", mcfg_setting[sloti].space_limit);
  MCFG_MSG_LOW_1("mcfg_setting.remote_storage %x", mcfg_setting[sloti].remote_storage);
#ifdef FEATURE_MCFG_FEATURETAG
  MCFG_MSG_LOW_1("mcfg_setting.multisim %d", mcfg_setting[sloti].multisim);
  MCFG_MSG_LOW_1("mcfg_setting.os %d", mcfg_setting[sloti].os);
  MCFG_MSG_LOW_1("mcfg_setting.market %d", mcfg_setting[sloti].market);
  MCFG_MSG_LOW_1("mcfg_setting.deployment %d", mcfg_setting[sloti].deployment);
  MCFG_MSG_LOW_1("mcfg_setting.ims_feature 0x%02x", mcfg_setting[sloti].ims_feature);
  MCFG_MSG_LOW_1("mcfg_setting.country %x", mcfg_setting[sloti].country);
  MCFG_MSG_LOW_1("mcfg_setting.variant %d", mcfg_setting[sloti].variant);
  MCFG_MSG_LOW_1("mcfg_setting.technology %d", mcfg_setting[sloti].technology);

  MCFG_MSG_LOW_1("mcfg_setting_orig.sloti %x", sloti);
  MCFG_MSG_LOW_1("mcfg_setting_orig.version %x", mcfg_setting_orig[sloti].version);
  MCFG_MSG_LOW_1("mcfg_setting_orig.selection_mode %d", mcfg_setting_orig[sloti].selection_mode);
  MCFG_MSG_LOW_1("mcfg_setting_orig.flex_mapping %d", mcfg_setting_orig[sloti].flex_mapping);
  MCFG_MSG_LOW_1("mcfg_setting_orig.carrier %d", mcfg_setting_orig[sloti].subsidized_carrier);
  MCFG_MSG_LOW_1("mcfg_setting_orig.refresh_mode %d", mcfg_setting_orig[sloti].refresh_mode);
  MCFG_MSG_LOW_1("mcfg_setting_orig.refresh_timer %d", mcfg_setting_orig[sloti].refresh_timer);
  MCFG_MSG_LOW_1("mcfg_setting_orig.refresh_debug %d", mcfg_setting_orig[sloti].refresh_debug);
  MCFG_MSG_LOW_1("mcfg_setting_orig.ssr_mode %x", mcfg_setting_orig[sloti].ssr_mode);
  MCFG_MSG_LOW_1("mcfg_setting_orig.ssr_timer %x", mcfg_setting_orig[sloti].ssr_timer);
  MCFG_MSG_LOW_1("mcfg_setting_orig.ssr_retries %x", mcfg_setting_orig[sloti].ssr_retries);
  MCFG_MSG_LOW_1("mcfg_setting_orig.ssr_retry_timer %x", mcfg_setting_orig[sloti].ssr_retries);
  MCFG_MSG_LOW_1("mcfg_setting_orig.space_limit %x", mcfg_setting_orig[sloti].space_limit);
  MCFG_MSG_LOW_1("mcfg_setting_orig.remote_storage %x", mcfg_setting_orig[sloti].remote_storage);
  MCFG_MSG_LOW_1("mcfg_setting_orig.multisim %d", mcfg_setting_orig[sloti].multisim);
  MCFG_MSG_LOW_1("mcfg_setting_orig.os %d", mcfg_setting_orig[sloti].os);
  MCFG_MSG_LOW_1("mcfg_setting_orig.market %d", mcfg_setting_orig[sloti].market);
  MCFG_MSG_LOW_1("mcfg_setting_orig.deployment %d", mcfg_setting_orig[sloti].deployment);
  MCFG_MSG_LOW_1("mcfg_setting_orig.ims_feature 0x%02x", mcfg_setting_orig[sloti].ims_feature);
  MCFG_MSG_LOW_1("mcfg_setting_orig.country %x", mcfg_setting_orig[sloti].country);
  MCFG_MSG_LOW_1("mcfg_setting_orig.variant %d", mcfg_setting_orig[sloti].variant);
  MCFG_MSG_LOW_1("mcfg_setting_orig.technology %d", mcfg_setting_orig[sloti].technology);
#endif
  
  return;
}

/*===========================================================================

  FUNCTION mcfg_sanity_check_feature

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
mcfg_error_e_type mcfg_sanity_check_feature
(
  pdc_set_feature_req_msg_v01 *feature
)
{
  mcfg_uim_cmd_payload_s_type uim_cmd_payload;
  boolean reselect = FALSE;
  boolean changed = FALSE;
  mcfg_setting_s_type mcfg_setting_temp;

  MCFG_CHECK_NULL_PTR_RET_ERROR(feature, MCFG_ERR_NULL_POINTER);
  MCFG_ASSERT_COND_RET_ERROR(feature->slot_id < MCFG_NUM_OF_UIM_SLOT, MCFG_ERR_INVALID_PARAMETER);

  if ((feature->flex_mapping_valid && (feature->flex_mapping > 0)) 
      || (!feature->flex_mapping_valid && (mcfg_setting[feature->slot_id].flex_mapping > 0)))
  {
    if (feature->selection_mode_valid)
    {
       if(feature->selection_mode == PDC_SELECTION_MODE_DISABLED_V01)
       {
         MCFG_MSG_ERROR("sanity_check_feature enable flx mapping with auto selection disabled");
         return MCFG_ERR_INVALID_PARAMETER;
       }
    }
    else if (mcfg_setting[feature->slot_id].selection_mode == PDC_SELECTION_MODE_DISABLED_V01)
    {
      MCFG_MSG_ERROR("sanity_check_feature enable flx mapping while auto selection already disabled");
      return MCFG_ERR_INVALID_PARAMETER;
    }
  }

  if (feature->selection_mode_valid)
  {
    MCFG_ASSERT_COND_RET_ERROR(feature->selection_mode <= PDC_SELECTION_MODE_HYBRID_V01, MCFG_ERR_INVALID_PARAMETER);
  }

  if (feature->refresh_mode_valid)
  {
    MCFG_ASSERT_COND_RET_ERROR(feature->refresh_mode <= PDC_REFRESH_MODE_ENABLED_V01, MCFG_ERR_INVALID_PARAMETER);
  }

#ifdef FEATURE_MCFG_FEATURETAG
  if (mcfg_feat_categories_updated(feature))
  {
    MCFG_MSG_ERROR("sanity_check_feature HW and SW related features modified simultaneously");
    return MCFG_ERR_INVALID_PARAMETER;
  }
#endif

  return MCFG_ERR_NONE;
}

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
)
{
  mcfg_uim_cmd_payload_s_type uim_cmd_payload;
  boolean reselect = FALSE;
  boolean hwreselect = FALSE;
  boolean changed = FALSE;
  mcfg_setting_s_type mcfg_setting_temp;
  mcfg_nv_status_e_type nv_ret;

  uint16 data_sz;
  uint16 tbl_sz;
  uint8 *feat_data_p;
  uint8 *mcfg_data_p;
/* -------------------------------------------------------------------- */

  MCFG_CHECK_NULL_PTR_RET_ERROR(feature, MCFG_ERR_NULL_POINTER);
  MCFG_ASSERT_COND_RET_ERROR(feature->slot_id < MCFG_NUM_OF_UIM_SLOT, MCFG_ERR_INVALID_PARAMETER);

  MCFG_MSG_LOW_1("mcfg_set_feature sloti%d", feature->slot_id);
  if(mcfg_sanity_check_feature(feature)!= MCFG_ERR_NONE)
  {
    MCFG_MSG_ERROR("mcfg_sanity_check_feature failed");
    return MCFG_ERR_INVALID_PARAMETER;
  }

  memscpy(&mcfg_setting_temp, sizeof(mcfg_setting_s_type),
          &mcfg_setting[feature->slot_id], sizeof(mcfg_setting_s_type));

  for (int setting = MCFG_SETTINGS_MIN; setting < MCFG_SETTINGS_MAX; setting++)
  {
    switch (setting)
    {
      case SELECTION_MODE_FEAT:
        if (feature->selection_mode_valid)
        {
         switch (feature->selection_mode)
         {
           case PDC_SELECTION_MODE_DISABLED_V01:
             changed = mcfg_uim_nv71546_update(feature->slot_id, 
                                 0, 
                                 MCFG_UIM_NV_AUTOSELECT_MASK|MCFG_UIM_NV_IMSISELECT_MASK);
#ifdef FEATURE_MCFG_REMOTE_STORAGE
             if (feature->slot_id == MCFG_REFRESH_INDEX_0 &&
                 mcfg_setting[feature->slot_id].remote_storage > 0 && 
                 mcfg_setting[feature->slot_id].selection_mode != PDC_SELECTION_MODE_DISABLED_V01)
             {
               MCFG_MSG_MED("mcfg_set_feature selection disable with openmkt selection, remove remote mbns");
               mcfg_setting_temp.remote_storage = 0;
               mcfg_utils_delete_all(MCFG_TYPE_HW, MCFG_STORAGE_MASK_REMOTE, TRUE);
               mcfg_utils_delete_all(MCFG_TYPE_SW, MCFG_STORAGE_MASK_REMOTE, TRUE);

               mcfg_nv_delete(MCFG_NV_ITEM_MCFG_RFS_HW_DIGEST_VERSION, 0, MCFG_REFRESH_INDEX_0);
               mcfg_nv_delete(MCFG_NV_ITEM_MCFG_RFS_SW_DIGEST_VERSION, 0, MCFG_REFRESH_INDEX_0);
             }
#else
             MCFG_MSG_MED("mcfg_set_feature selection disabled");
#endif
             break;

           case PDC_SELECTION_MODE_IIN_V01:
           case PDC_SELECTION_MODE_IMSI_V01:
             feature->selection_mode = PDC_SELECTION_MODE_HYBRID_V01;

           case PDC_SELECTION_MODE_HYBRID_V01:
             changed = mcfg_uim_nv71546_update(feature->slot_id, 
                                 MCFG_UIM_NV_AUTOSELECT_MASK|MCFG_UIM_NV_IMSISELECT_MASK, 
                                 MCFG_UIM_NV_AUTOSELECT_MASK|MCFG_UIM_NV_IMSISELECT_MASK);
#ifdef FEATURE_MCFG_REMOTE_STORAGE
             if (feature->slot_id == MCFG_REFRESH_INDEX_0 &&
                 mcfg_setting[feature->slot_id].remote_storage == 0)
             {
               MCFG_MSG_MED("mcfg_set_feature selection enable, restart remote discover");
               mcfg_setting_temp.remote_storage = 1;
               mcfg_task_send_cmd_w_delay(MCFG_TASK_CMD_DISCOVER, 0);
             }
#else
             MCFG_MSG_MED("mcfg_set_feature selection enabled");
#endif
             break;

           default:
             return MCFG_ERR_NOT_SUPPORTED;
         }

         reselect = reselect || changed;
         mcfg_setting_temp.selection_mode = feature->selection_mode;
		}
         break;

    case FLEX_MAPPING_FEAT:
        if (feature->flex_mapping_valid)
        {
         changed = mcfg_uim_nv71546_update(feature->slot_id, 
                             feature->flex_mapping << 1, 
                             MCFG_UIM_NV_FLEXMAP_MASK);
         reselect = reselect || changed;
         mcfg_setting_temp.flex_mapping = feature->flex_mapping;
		}
         break;

    case REFRESH_MODE_FEAT:
        if (feature->refresh_mode_valid)
        {
         switch (feature->refresh_mode)
         {
           case PDC_REFRESH_MODE_DISABLED_V01:
             changed = mcfg_uim_nv71546_update(feature->slot_id, 
                                 MCFG_UIM_NV_REFRESH_MASK, 
                                 MCFG_UIM_NV_REFRESH_MASK);
             break;
   
           case PDC_REFRESH_MODE_ENABLED_V01:
             changed = mcfg_uim_nv71546_update(feature->slot_id, 
                                 0, 
                                 MCFG_UIM_NV_REFRESH_MASK);
             break;
   
           default:
             return MCFG_ERR_NOT_SUPPORTED;
         }

         reselect = reselect || changed;
         mcfg_setting_temp.refresh_mode = feature->refresh_mode;
		}
         break;

    default: 
         tbl_sz = sizeof(mcfg_feat_table)/sizeof(mcfg_feat_table[0]);
         if (setting >= tbl_sz)
         {
           MCFG_MSG_HIGH_2("setting not supported in feature table! setting: %d, table_sz: %d", setting, tbl_sz);
           return MCFG_ERR_NOT_SUPPORTED;
         }

         feat_data_p = (uint8 *) feature + mcfg_feat_table[setting].pdc_data_offset; 
         mcfg_data_p = (uint8 *) &mcfg_setting_temp + mcfg_feat_table[setting].mcfg_data_offset; 
         data_sz = mcfg_feat_table[setting].data_len; 

         if (memcmp(feat_data_p, mcfg_data_p, data_sz) != 0)
         {
           memscpy(mcfg_data_p, data_sz, feat_data_p, data_sz);

           if(setting == CARRIER_FEAT || setting == MULTISIM_FEAT)
           {
             hwreselect = TRUE;
           }
#ifdef FEATURE_MCFG_FEATURETAG
           else if (mcfg_feat_table[setting].category == MCFG_TYPE_HW)
           {
             hwreselect = TRUE;
           }
#endif
           reselect = TRUE;
         }
    }
  }

  memscpy(&mcfg_setting[feature->slot_id], sizeof(mcfg_setting_s_type),
          &mcfg_setting_temp, sizeof(mcfg_setting_s_type));

  mcfg_uim_nv71546_write(feature->slot_id);
  mcfg_nv_write(MCFG_NV_ITEM_SETTING, MCFG_SETTING_CURR, feature->slot_id,
                &mcfg_setting[feature->slot_id], 
                sizeof(mcfg_setting_s_type));

  /* mcfg_setting_orig needs to be populated before print */
  nv_ret = mcfg_nv_read(MCFG_NV_ITEM_SETTING, MCFG_SETTING_BKUP, feature->slot_id, 
               &mcfg_setting_orig[feature->slot_id], sizeof(mcfg_setting_s_type));

  if(nv_ret != MCFG_NV_STATUS_OK)
  {
    MCFG_MSG_MED_1("Failed to read mcfg setting backup for slot %d", feature->slot_id);
  }
  mcfg_feature_print(feature->slot_id);
  
  MCFG_MSG_MED_2("hwreselect: %d, reselect: %d", hwreselect, reselect);
  if (hwreselect)
  {
    if(mcfg_sel_hw_config() == MCFG_ERR_NONE)
    { 
      reselect = FALSE;
    }
  }

  if (reselect)
  {
    uim_cmd_payload.sloti = feature->slot_id;
    mcfg_uim_cmd_send(MCFG_UIM_CMD_RESELECT, &uim_cmd_payload);
  }
  return MCFG_ERR_NONE;
}


#ifdef FEATURE_MCFG_FEATURETAG
/*===========================================================================

  FUNCTION mcfg_get_feature

  DESCRIPTION
    Return either current or previous feature settings based on feat_store input.

  DEPENDENCIES
    None

  PARAMETERS
    Sloti - Slot index used to access original mcfg_setting values

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
)
{
  mcfg_nv_status_e_type nv_ret;
  mcfg_setting_s_type *mcfg_setting_store = NULL; 

  MCFG_CHECK_NULL_PTR_RET_ERROR(feature, MCFG_ERR_NULL_POINTER);
  MCFG_ASSERT_COND_RET_ERROR(sloti < MCFG_NUM_OF_UIM_SLOT, MCFG_ERR_INVALID_PARAMETER);
  MCFG_MSG_LOW_1("mcfg_get_feature sloti%d", sloti);


  if (MCFG_SETTING_BKUP == feat_store)
  {
    MCFG_MSG_MED_1("cmp with original mcfg_settings (slot %d)", sloti);
    feat_store = MCFG_SETTING_BKUP; 
    mcfg_setting_store = mcfg_setting_orig;

    nv_ret = mcfg_nv_read(MCFG_NV_ITEM_SETTING, MCFG_SETTING_BKUP, sloti, 
                          &mcfg_setting_store[sloti], sizeof(mcfg_setting_s_type));
    if(nv_ret != MCFG_NV_STATUS_OK)
    {
      MCFG_MSG_MED_1("Failed to read mcfg settings for slot %d", sloti);
      return MCFG_ERR_INTERNAL;
    }
  }
  else 
  {
    MCFG_MSG_MED_1("cmp with current mcfg_settings (slot %d)", sloti);
    feat_store = MCFG_SETTING_CURR; 
    mcfg_setting_store = mcfg_setting;
  }

  if (mcfg_setting_store[sloti].version == 0)
  {
    return MCFG_ERR_INTERNAL;
  }

  feature->carrier_valid = TRUE;
  feature->carrier = mcfg_setting_store[sloti].subsidized_carrier;

  if (MCFG_SETTING_BKUP == feat_store)
  {
    feature->selection_mode_valid = TRUE;
    feature->selection_mode = mcfg_setting_store[sloti].selection_mode;

    feature->flex_mapping_valid = TRUE;
    feature->flex_mapping = mcfg_setting_store[sloti].flex_mapping;

    feature->refresh_mode_valid = TRUE;
    feature->refresh_mode = mcfg_setting_store[sloti].refresh_mode;
  }
  else 
  {
    feature->selection_mode_valid = TRUE;
    feature->selection_mode = 
      mcfg_uim_nv_autoselect_enabled(sloti) + (mcfg_uim_nv_imsiselect_enabled(sloti)<< 1);

    feature->flex_mapping_valid = TRUE;
    feature->flex_mapping = mcfg_uim_nv_flexmap_enabled(sloti);

    feature->refresh_mode_valid = TRUE;
    feature->refresh_mode = mcfg_uim_nv_refresh_enabled(sloti);
  }

  if (mcfg_setting_store[sloti].version == 2)
  {
    feature->multisim_valid = TRUE;
    feature->multisim = mcfg_setting_store[sloti].multisim;

    feature->os_valid = TRUE;
    feature->os = mcfg_setting_store[sloti].os;

    feature->market_valid = TRUE;
    feature->market = mcfg_setting_store[sloti].market;

    feature->deployment_valid = TRUE;
    feature->deployment = mcfg_setting_store[sloti].deployment;

    feature->ims_feature_valid = TRUE;
    feature->ims_feature = mcfg_setting_store[sloti].ims_feature;

    feature->country_valid = TRUE;
    feature->country = mcfg_setting_store[sloti].country;

    feature->variant_valid = TRUE;
    feature->variant = mcfg_setting_store[sloti].variant;
  }

  mcfg_feature_print(sloti);
  return MCFG_ERR_NONE;
}


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
)
{
  boolean changed = FALSE;
  MCFG_ASSERT_COND_RET_FALSE(sloti < MCFG_NUM_OF_UIM_SLOT);

  if (mcfg_setting[sloti].country != country)
  {
    MCFG_MSG_MED_3("mcfg_setting_update_feature sloti%d country code updated from %d to %d",
                 sloti, mcfg_setting[sloti].country, country);
    changed = TRUE;
    mcfg_setting[sloti].country = country; 
  }

  if (mcfg_setting[sloti].ims_feature != ims_feature)
  {
    MCFG_MSG_MED_3("mcfg_setting_update_feature sloti%d ims_feature updated from 0x%02x to 0x%02x",
                 sloti, mcfg_setting[sloti].ims_feature, ims_feature);
    changed = TRUE;
    mcfg_setting[sloti].ims_feature = ims_feature; 
  }

  return changed;
}

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
)
{
  boolean reselect = FALSE;

  MCFG_CHECK_NULL_PTR_RET_ERROR(feature, MCFG_ERR_NULL_POINTER);
  MCFG_ASSERT_COND_RET_ERROR(sloti < MCFG_NUM_OF_UIM_SLOT, MCFG_ERR_INVALID_PARAMETER);
  MCFG_MSG_LOW_1("mcfg_get_feature sloti%d", sloti);

  if (mcfg_setting[sloti].version == 0)
  {
    MCFG_MSG_MED("mcfg_get_feature: invalid version");
    return MCFG_ERR_INTERNAL;
  }

  feature->carrier_valid = TRUE;
  feature->carrier = mcfg_setting[sloti].subsidized_carrier;

  feature->selection_mode_valid = TRUE;
  feature->selection_mode = 
    mcfg_uim_nv_autoselect_enabled(sloti) + (mcfg_uim_nv_imsiselect_enabled(sloti)<< 1);

  feature->flex_mapping_valid = TRUE;
  feature->flex_mapping = mcfg_uim_nv_flexmap_enabled(sloti);

  feature->refresh_mode_valid = TRUE;
  feature->refresh_mode = mcfg_uim_nv_refresh_enabled(sloti);
  
  if (mcfg_setting[sloti].version == 2)
  {
    feature->multisim_valid = TRUE;
    feature->multisim = mcfg_setting[sloti].multisim;

    feature->os_valid = TRUE;
    feature->os = mcfg_setting[sloti].os;

    feature->market_valid = TRUE;
    feature->market = mcfg_setting[sloti].market;

    feature->deployment_valid = TRUE;
    feature->deployment = mcfg_setting[sloti].deployment;

    feature->ims_feature_valid = TRUE;
    feature->ims_feature = mcfg_setting[sloti].ims_feature;

    feature->country_valid = TRUE;
    feature->country = mcfg_setting[sloti].country;

    feature->variant_valid = TRUE;
    feature->variant = mcfg_setting[sloti].variant;
  }

  mcfg_feature_print(sloti);
  return MCFG_ERR_NONE;
}

#endif /* ! FEATURE_MCFG_FEATURETAG */

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
)
{
  return mcfg_setting[0].space_limit;
}

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
)
{
  return (mcfg_setting[0].remote_storage > 0);
}

/*===========================================================================

FUNCTION mcfg_setting_init

DESCRIPTION
  Init setting from stored efs
  
DEPENDENCIES
  None.

RETURN VALUE
  booelan

SIDE EFFECTS

===========================================================================*/
boolean mcfg_setting_init(void) 
{
  mcfg_slot_index_type_e_type sloti;
  mcfg_uim_cmd_payload_s_type uim_cmd_payload;
  mcfg_nv_status_e_type nv_ret;

  memset(&mcfg_setting, 0, sizeof(mcfg_setting));
  for (sloti = 0; sloti < MCFG_NUM_OF_UIM_SLOT; sloti++)
  {
    nv_ret = mcfg_nv_read(MCFG_NV_ITEM_SETTING, MCFG_SETTING_CURR, sloti, 
                          &mcfg_setting[sloti], sizeof(mcfg_setting_s_type));
    if(nv_ret != MCFG_NV_STATUS_OK || 
      (mcfg_setting[sloti].version != MCFG_SETTING_CURR_VER))
    {
      MCFG_MSG_ERROR_2("mcfg_setting_init - Failed to read NV for slot:%d, status: %d",
                    sloti, nv_ret);
      mcfg_setting[sloti].subsidized_carrier = 0;
      if (sloti==0)
      {
        mcfg_setting[0].space_limit = MCFG_SETTING_DEFAULT_SPACE_LIMIT;
#ifdef FEATURE_MCFG_REMOTE_STORAGE
        mcfg_setting[sloti].remote_storage = 1;
#else
        mcfg_setting[sloti].remote_storage = 0;
#endif
      }
    }
      mcfg_setting[sloti].selection_mode = 
        mcfg_uim_nv_autoselect_enabled(sloti) + (mcfg_uim_nv_imsiselect_enabled(sloti)<< 1);
      mcfg_setting[sloti].flex_mapping = mcfg_uim_nv_flexmap_enabled(sloti);
    
    mcfg_setting[sloti].refresh_mode = mcfg_uim_nv_refresh_enabled(sloti);

    mcfg_setting[sloti].version = 2;

    mcfg_feature_print(sloti);
  }

  return TRUE;
} /* mcfg_setting_init() */

