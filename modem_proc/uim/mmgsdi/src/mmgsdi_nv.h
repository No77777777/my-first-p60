#ifndef MMGSDI_NV_H
#define MMGSDI_NV_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


        M M G S D I  N V  F U N C T I O N S


GENERAL DESCRIPTION

  This file contains the header information for functions accessing
  MMGSDI NV and EFS data.

                      COPYRIGHT INFORMATION

Copyright (c) 2001 - 2003, 2011 - 2018, 2021 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/mmgsdi/src/mmgsdi_nv.h#5 $$ $DateTime: 2021/09/09 00:04:08 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
03/25/21   rps     Allow reading of EF IMSI for buggy card
10/13/20   vgd     Allow reading of EF AD for buggy card
06/05/20   sg      Verify pin after pin is unblocked for buggy card
12/27/18   sg      Added NV check for setting no. of available logical channels
11/30/18   vdc     Purge temporary identities in INIT+FCN refresh mode
05/19/17   nr      Creating a new efs item for override_opl_pnn_lookup_for_non_rplmn
04/24/17   nr      Support to store more FPLMNs in cache for MMGSDI_USIM_FPLMN
04/13/17   me      Support to disable GBA_U service  
02/13/17   sk      Deprecate direct read of GCF NV 947
02/08/17   vdc     Send verify PIN command after enabling the PIN for buggy card
01/26/17   shr     Add support for SIM file fallback to File System
01/10/17   bcho    Support for slot specific CDMA LESS NV feature
10/06/16   me      Removed functions related to GSTK
06/29/16   shr     Support for NV based Toolkit disablement
05/13/16   nr      Skip Search Req for PB files
05/10/16   vdc     Guard with FEATURE_MMGSDI_3GPP2 for 3GPP2 related functions
04/13/16   ar      CDMA/HDR support on 2nd SUB
03/28/16   kv      Added support to handle remote SIM Params
03/14/16   av      Rework internal function used by MMGSDI to read NV items
01/22/16   bcho    Featurize use of non-gsma entries
11/26/15   sp      Updated copyright
10/06/15   kv      Remove diag access to security sensitive NVs in FTM mode
09/10/15   kv      Features_status_list is split into device and slot items
08/28/15   hh      Removed JCDMA related code
08/25/15   bcho    Support for CDMALess Device
08/21/15   at      Support for slot type in MMGSDI APIs
08/11/15   av      Remove code related to NV 3461 (NV_ENS_ENABLED_I)
07/17/15   bcho    Remove compilation error
04/29/15   bcho    Support for App specific cache
04/29/15   hh      Differentiate CSG support for W and LTE for EF-TST
02/20/15   vv      Remove perso related code
12/03/14   bcho    featurize support to use stored rplmn name across operators
11/06/14   hh      Call GSTK NV API to read GSTK features in NV 65674
10/14/14   av      Handle appropriately zero ICCID reported by uimdrv
10/12/14   hh      Add support to get Voice Domain Preference
09/09/14   kk      Enhancements in NV handling
09/30/14   ar      Support to store more FPLMNs in cache for MMGSDI_GSM_FPLMN
07/30/14   ar      Revert FR 20469 changes to align with NAS check-in dates
07/25/14   av      Auto onchip activation for multi slots
07/25/14   ar      Support to store more FPLMNs in cache for MMGSDI_GSM_FPLMN
07/15/14   yt      Support for NV refresh
07/14/14   tl      Remove always ON feature FEATURE_MODEM_RCINIT
06/02/14   bcho    New item to decide on blocking of CDMA EPRL access
05/09/14   vv      Deprecate MMGSDI_FEATURE_UICC_RAT_INDICATOR_SUPPORT
04/06/14   am      Added mmgsdi heap alloc clean up logic
03/28/14   ar      Enabled N.A. PLMN comparison feature by default
03/25/14   vdc     Added new item for verifying PIN when PIN is changed
02/27/14   dy      Add default_apn, bip_apn_mapping_tbl, rat_priority NV item
02/25/14   hh      Purge temporary mobile identities during UICC Reset Refresh
02/25/14   bcho    Added support to store OPLMN List in EFS and cache it
01/16/14   av      New item for deciding bw card and nv for 3gpd credentials
01/10/14   bcho    Refresh 3G Session Reset as per ETSI spec under NV feature
12/18/13   yt      Skip app termination as part of session deactivation
11/21/13   vv      Halt 3gpp subscription if 3gpp2 perso fails
10/17/13   am      Added new item for plmn comparison in the nvitem 67211
10/08/13   tl      Addition of an RPM info EFS structure type
10/08/13   ar      Support to select CDMA before TP by default
10/04/13   vv      Added NV support for ENS sub-features
09/26/13   av      Enable OnChip on bootup based on EFS item
08/29/13   vv      Update EF-TST to indicate support for IMS
08/20/13   spo     Automatic Subscription provisioning enhancements in UIM
07/29/13   am      Added support to determine CSG support dynamically
06/24/13   vdc     Added support for triple SIM
05/17/13   av      Halt 3gpp2 subscription if 3gpp perso fails
05/15/13   bcho    Added new items to mmgsdi features status nvitem 67211
04/25/13   vdc     Added select_dfcdma to mmgsdi feature status nvitem 67211
04/17/13   tl      Replace Featurization with NV
04/05/13   av      Move UIM tasks to group4 of RCINIT
04/04/13   bcho    Added new items to mmgsdi features status nvitem 67211
03/27/13   spo     Add new items to mmgsdi feature status list
01/03/12   abg     Added API mmgsdi_nv_init_read_and_cache_nv_items to read NV during task-init
12/26/12   av      Do not allow writing to the file that is being refreshed
11/05/12   av      Expand features status list nvitem to control skipping
                   checking security attributes before reading EF
10/31/12   tl      CSIM conformance updating EF MECRP
10/31/12   tl      CSIM conformance updating EF MODEL
10/21/12   av      Update conf file only if necessary
10/16/12   yt      Add new items to mmgsdi feature status list
09/28/12   yt      Update user profile parameters after successful OTASP Commit
03/08/12   av      Added new items to mmgsdi features status nvitem 67211
01/30/12   shr     Use GSTK NV item (65674) to determine if the OTASP IMSI_M
                   enhancement is enabled or not
01/13/12   at      Added NV item caching for NV_DISABLE_CM_CALL_TYPE_I
12/21/11   shr     Legacy GSDI removal updates
11/15/11   nmb     RPM Support
10/10/11   av      Updated default values of MMGSDI Features
09/16/11   av      Replace feature #ifdefs with an NVITEM
07/12/11   at      Added NV item caching for NV_SERVICE_DOMAIN_PREF_I
06/05/11   vs      Updated NV item caching logic to include NV_ESN_ME_I
10/04/10   nmb     Added NV queue to address NV deadlocks
05/13/10   vs      Fix for NV and GSDI deadlock
11/05/03   jar     Added GSDI - NV Interface function for SLOT Preference
                   Reads: gsdi_get_nv_slot_preferences().
11/26/02   jar     Removed RTRE Code
09/14/02   jar     Changed FEATURE_MMGSDI_RTRE to Internal Feature MMGSDI_RTRE.
08/30/02  jar/ck   Removed gsdi_wait_nv.  Added gsdi_send_rtre_command.
                   Added feature MMGSDI_RTRE.
08/18/02   jar     Created gsdi_nv.h
===========================================================================*/


/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "nv.h"
#include "mmgsdi.h"
#include "uim_common_efs.h"

/*===========================================================================

            DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/
/* The following NV items are currently read and written to by MMGSDI. If this
   changes then the cache size MMGSDI_NV_ITEM_CACHE_SIZE and the MMGSDI NV
   cache initialization routine must be updated appropriately. The items are:
   NV_UIM_CDMA_PREF_SLOT_I
   NV_UIM_GSM_PREF_SLOT_I
   NV_RTRE_CONFIG_I
   NV_MMGSDI_ME_CONFIG_PARAM_I
   NV_UIM_SELECT_DEFAULT_USIM_APP_I
   NV_MEID_ME_I
   NV_UE_IMEI_I
   NV_HOMEZONE_ENABLED_I
   NV_ESN_ME_I
   NV_SCM_I
   NV_MOB_CAI_REV_I
   NV_SERVICE_DOMAIN_PREF_I
   NV_DISABLE_CM_CALL_TYPE_I */
#define MMGSDI_NV_ITEM_CACHE_SIZE       0x0D

#define MMGSDI_NV_CONTEXTS_MAX 0x03

/* Max possible feature-elements in legacy FEATURES_STATUS_LIST_TYPE NVITEM struct */
#define MMGSDI_MAX_ITEMS_IN_FEATURE_NVITEM           63

typedef struct {
  nv_items_enum_type  item_enum;
  union {
    nv_uim_slot_type                                 uim_cdma_pref_slot;
    nv_uim_slot_type                                 uim_gsm_pref_slot;
    nv_rtre_configuration_type                       rtre_config;
    uint32                                           mmgsdi_me_config_param;
    boolean                                          uim_select_default_usim_app;
    qword                                            meid;
    nv_ue_imei_type                                  ue_imei;
    boolean                                          homezone_enabled;
    nv_esn_type                                      esn;
    byte                                             scm;
    byte                                             mob_cai_rev;
    nv_srv_domain_pref_type                          service_domain_pref;
    uint64                                           disable_cm_call_type;
  }item_value;
  nv_stat_enum_type   item_status;
} mmgsdi_nv_item_cache_type;

typedef struct {
  mmgsdi_nv_item_cache_type  item[MMGSDI_NV_ITEM_CACHE_SIZE];
  uint32                     is_cached_mask;
} mmgsdi_nv_item_cache_table_type;

/* ===========================================================================
   STRUCTURE:      MMGSDI_REMOTE_SIM_TEST_PARAMS_TYPE

   DESCRIPTION:
     This structure contains the test parameters for a remote SIM card.
===========================================================================*/
typedef struct
{
  uint16 delay_interval;
} mmgsdi_remote_sim_test_params_type;

/*=============================================================================
  ENUM:   MMGSDI_NV_FEATURES_ENUM_TYPE
=======================================================================*/

typedef enum {
  MMGSDI_FEATURE_LEGACY_ABSENT_MANDATORY_FILES_INIT = 0,
  MMGSDI_FEATURE_LEGACY_ECC_NOT_PRESENT_SUPPORT,
  MMGSDI_FEATURE_LEGACY_SPECIAL_USIM_ARR,
  MMGSDI_FEATURE_LEGACY_DEPRECATED_1,
  MMGSDI_FEATURE_LEGACY_CDMA_DF_SELECT_BEFORE_TP,
  MMGSDI_FEATURE_LEGACY_CHANGE_INST_CLASS,
  MMGSDI_FEATURE_LEGACY_CARD_SLOT_CHECK,
  MMGSDI_FEATURE_LEGACY_TMSI_PLMN_CHECK,
  MMGSDI_FEATURE_LEGACY_IGNORE_INIT_ERROR,
  MMGSDI_FEATURE_LEGACY_ALLOW_SEND_APDU_ONLY,

  /* 10 */
  MMGSDI_FEATURE_LEGACY_PREF_MODE_NOT_FROM_RUIM,
  MMGSDI_FEATURE_LEGACY_RUIM_ALLOW_ESN_FAIL,
  MMGSDI_FEATURE_LEGACY_RUIM_ZERO_ESN_FAIL,
  MMGSDI_FEATURE_LEGACY_UIM_RUIM_SUPPORT_SCI,
  MMGSDI_FEATURE_LEGACY_UIM_AN_HRPD_FALLBACK,
  MMGSDI_FEATURE_LEGACY_UIM_MISCONFIG_RUIM_N5_WORKAROUND,
  MMGSDI_FEATURE_LEGACY_DEPRECATED_3,
  MMGSDI_FEATURE_LEGACY_CSIM_UPP_UPDATE_AFTER_OTASP,
  MMGSDI_FEATURE_LEGACY_ALLOW_ACCESS_BEFORE_APP_READY,
  MMGSDI_FEATURE_LEGACY_SKIP_UICC_SECURITY_ATTR_CHECK_BEFORE_READ,

  /*20 */
  MMGSDI_FEATURE_LEGACY_BLOCK_WRITES_TO_REFRESH_FILES,
  MMGSDI_FEATURE_LEGACY_SE13_TABLE_LOOKUP_GET_FIRST_GSMA_MATCH,
  MMGSDI_FEATURE_LEGACY_DEPRECATED_4,
  MMGSDI_FEATURE_LEGACY_MMGSDI_UNBLOCK_PIN_HANDLE_INVALID_SW,
  MMGSDI_FEATURE_LEGACY_UIM_GSM_DCS_1800,
  MMGSDI_FEATURE_LEGACY_DEPRECATED_2,
  MMGSDI_FEATURE_LEGACY_EXPLICIT_SELECT_DFCDMA_BEFORE_TELECOM_ADN_READ,
  MMGSDI_FEATURE_LEGACY_OPL_PNN_LOOKUP_FOR_NON_RPLMN,
  MMGSDI_FEATURE_LEGACY_DEPRECATED_5,
  MMGSDI_FEATURE_LEGACY_MULTISIM_AUTO_PROVISIONING,

  /* 30 */
  MMGSDI_FEATURE_LEGACY_NORTH_AMERICAN_PLMN_COMPARISON,
  MMGSDI_FEATURE_LEGACY_DEPRECATED_6,
  MMGSDI_FEATURE_LEGACY_SKIP_PROV_APP_TERMINATION,
  MMGSDI_FEATURE_LEGACY_REFRESH_3G_SESSION_RESET_PER_ETSI_SPEC,
  MMGSDI_FEATURE_LEGACY_USE_3GPD_CREDENTIALS_FROM_NV,
  MMGSDI_FEATURE_LEGACY_USE_SFI,
  MMGSDI_FEATURE_LEGACY_OPLMN_LIST_CONFIGURABLE_OTA,
  MMGSDI_FEATURE_LEGACY_PURGE_TEMP_IDENTITIES,
  MMGSDI_FEATURE_LEGACY_VERIFY_PIN_AFTER_CHANGE_OR_ENABLE_OR_UNBLOCK,
  MMGSDI_FEATURE_LEGACY_BLOCK_RUIM_EPRL_ACCESS,

  /*40*/
  MMGSDI_FEATURE_LEGACY_SUPPORT_EXTENDED_FPLMN,
  MMGSDI_FEATURE_LEGACY_USE_STORED_RPLMN_NAME_ACROSS_OPERATORS,
  MMGSDI_FEATURE_LEGACY_KEEP_CACHE_AFTER_SESSION_DEACTIVATION
} mmgsdi_nv_features_enum_type;

/*=============================================================================
  ENUM:   MMGSDI_NV_DEVICE_SPECIFIC_FEATURES_ENUM_TYPE
=======================================================================*/
typedef enum {
  MMGSDI_FEATURE_ECC_NOT_PRESENT_SUPPORT = 0,
  MMGSDI_FEATURE_SPECIAL_USIM_ARR,
  MMGSDI_FEATURE_CDMA_DF_SELECT_BEFORE_TP,
  MMGSDI_FEATURE_CHANGE_INST_CLASS,
  MMGSDI_FEATURE_CARD_SLOT_CHECK,
  MMGSDI_FEATURE_TMSI_PLMN_CHECK,
  MMGSDI_FEATURE_IGNORE_INIT_ERROR,
  MMGSDI_FEATURE_ALLOW_SEND_APDU_ONLY,
  MMGSDI_FEATURE_PREF_MODE_NOT_FROM_RUIM,
  MMGSDI_FEATURE_ALLOW_ACCESS_BEFORE_APP_READY,


  /* 10 */
  MMGSDI_FEATURE_SKIP_UICC_SECURITY_ATTR_CHECK_BEFORE_READ,
  MMGSDI_FEATURE_BLOCK_WRITES_TO_REFRESH_FILES,
  MMGSDI_FEATURE_SE13_TABLE_LOOKUP_GET_FIRST_GSMA_MATCH,
  MMGSDI_FEATURE_MMGSDI_UNBLOCK_PIN_HANDLE_INVALID_SW,
  MMGSDI_FEATURE_EXPLICIT_SELECT_DFCDMA_BEFORE_TELECOM_ADN_READ,
  MMGSDI_FEATURE_OPL_PNN_LOOKUP_FOR_NON_RPLMN,
  MMGSDI_FEATURE_MULTISIM_AUTO_PROVISIONING,
  MMGSDI_FEATURE_NORTH_AMERICAN_PLMN_COMPARISON,
  MMGSDI_FEATURE_SKIP_PROV_APP_TERMINATION,
  MMGSDI_FEATURE_REFRESH_3G_SESSION_RESET_PER_ETSI_SPEC,

  /*20 */
  MMGSDI_FEATURE_USE_SFI,
  MMGSDI_FEATURE_VERIFY_PIN_AFTER_CHANGE_OR_ENABLE_OR_UNBLOCK,
  MMGSDI_FEATURE_SUPPORT_EXTENDED_FPLMN,
  MMGSDI_FEATURE_USE_STORED_RPLMN_NAME_ACROSS_OPERATORS,
  MMGSDI_FEATURE_KEEP_CACHE_AFTER_SESSION_DEACTIVATION,
  MMGSDI_FEATURE_SE13_TABLE_LOOKUP_USE_NON_GSMA_ENTRY,
  MMGSDI_FEATURE_SIM_FILE_FALLBACK_TO_FS,
  MMGSDI_FEATURE_DISABLE_APDU_CONVERSION_ON_CH_0,
  MMGSDI_FEATURE_DEFAULT_LEGACY_LOGICAL_CHANNELS
} mmgsdi_nv_device_specific_features_enum_type;

/*=============================================================================
  ENUM:   MMGSDI_NV_SLOT_SPECIFIC_FEATURES_ENUM_TYPE
=======================================================================*/
typedef enum {
  MMGSDI_FEATURE_ABSENT_MANDATORY_FILES_INIT = 0,
  MMGSDI_FEATURE_RUIM_ALLOW_ESN_FAIL,
  MMGSDI_FEATURE_RUIM_ZERO_ESN_FAIL,
  MMGSDI_FEATURE_UIM_RUIM_SUPPORT_SCI,
  MMGSDI_FEATURE_UIM_AN_HRPD_FALLBACK,
  MMGSDI_FEATURE_UIM_MISCONFIG_RUIM_N5_WORKAROUND,
  MMGSDI_FEATURE_CSIM_UPP_UPDATE_AFTER_OTASP,
  MMGSDI_FEATURE_UIM_GSM_DCS_1800,
  MMGSDI_FEATURE_USE_3GPD_CREDENTIALS_FROM_NV,
  MMGSDI_FEATURE_OPLMN_LIST_CONFIGURABLE_OTA,
  MMGSDI_FEATURE_PURGE_TEMP_IDENTITIES,
  MMGSDI_FEATURE_BLOCK_RUIM_EPRL_ACCESS,
  MMGSDI_FEATURE_SKIP_SEARCH_FOR_PB_FILES,
  MMGSDI_FEATURE_CDMA_LESS,
  MMGSDI_FEATURE_DISABLE_GBA_U,
  MMGSDI_FEATURE_CARD_RESET_ON_REFRESH_FAILURE,
  MMGSDI_FEATURE_PERSISTENT_CACHE,
  MMGSDI_FEATURE_OVERRIDE_USE_SFI_TO_FALSE,
  MMGSDI_FEATURE_PURGE_TEMP_IDENTITIES_INIT_FCN_REFRESH_WITH_IMSI_CHANGE,
  MMGSDI_FEATURE_PURGE_TEMP_IDENTITIES_3G_SESSION_RESET_REFRESH_WITH_IMSI_CHANGE,
  MMGSDI_FEATURE_PRIORITIZE_5G_RAT,
  MMGSDI_FEATURE_IGNORE_FEW_PLMNS_IN_EHPLMN,
  MMGSDI_FEATURE_IGNORE_READ_SEC_ATTRIBUTES_FOR_EF_USIM_AD,
  MMGSDI_FEATURE_IGNORE_READ_SEC_ATTRIBUTES
} mmgsdi_nv_slot_specific_features_enum_type;

/*=============================================================================
  ENUM:   MMGSDI_FEATURE_STATUS_ENUM_TYPE
=======================================================================*/
typedef enum {
  MMGSDI_FEATURE_DISABLED = 0, /* Feature disabled */
  MMGSDI_FEATURE_ENABLED  = 1  /* Feature enabled */
} mmgsdi_feature_status_enum_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      MMGSDI_FEATURES_STATUS_LIST_TYPE

   DESCRIPTION:
     Data Structure to hold status of legacy mmgsdi features as it reads it from
     efs NVITEM "MMGSDI_FEATURES_STATUS_LIST_TYPE"
     This item is not expanded anymore.
  -------------------------------------------------------------------------------*/
typedef struct {
  uint8 version;
  mmgsdi_feature_status_enum_type   item[MMGSDI_MAX_ITEMS_IN_FEATURE_NVITEM];
} mmgsdi_features_status_list_type;

/* ----------------------------------------------------------------------------
   ENUM:      MMGSDI_NV_EXTERNAL_FEATURES_ENUM

   DESCRIPTION:
     Enumeration corresponding to external features
-------------------------------------------------------------------------------*/
typedef enum {
  MMGSDI_EXT_FEAT_NAS_CFG_FEATURE_CSG = 0,
  MMGSDI_EXT_FEAT_IMS_CFG_FEATURE_IMS_SUPPORT,
  MMGSDI_EXT_FEAT_RPM_ENABLED,
  MMGSDI_EXT_FEAT_VOLTE_USING_IMS
} mmgsdi_nv_external_features_enum;

/*==============================================================================
  ENUM:   MMGSDI_FEATURE_OVERRIDE_FEATURE_STATUS_ENUM_TYPE

   DESCRIPTION:
     Enumeration corresponding to override feature status
================================================================================*/
typedef enum {
  MMGSDI_OVERRIDE_FEATURE_NOT_INITIALIZED = 0, /* Override feature is not initlized */
  MMGSDI_OVERRIDE_FEATURE_NO_OVERRIDE     = 1, /* Override feature no override */
  MMGSDI_OVERRIDE_FEATURE_DISABLED        = 2, /* Override feature disabled */
  MMGSDI_OVERRIDE_FEATURE_ENABLED         = 3, /* Override feature enabled */
} mmgsdi_feature_override_feature_status_enum_type;

/*===========================================================================
FUNCTION MMGSDI_NV_GET_ITEM

DESCRIPTION
  Get an item from the nonvolatile memory.

RETURN VALUE
  The NV return code, except for NV_NOTACTIVE_S, which is handled
  internally.

DEPENDENCIES
  This routine is not reentrant.  Shouldn't be a problem, as it doesn't exit
  till we're done.
===========================================================================*/
extern nv_stat_enum_type mmgsdi_nv_get_item(
  nv_items_enum_type         item,               /* which item */
  mmgsdi_nv_item_cache_type *mmgsdi_nv_data_ptr, /* pointer to space for item */
  mmgsdi_nv_context_type     nv_context          /* Conrtext of the item to access */
);

/*===========================================================================
FUNCTION MMGSDI_NV_PUT_ITEM

DESCRIPTION
  Put an item on to the nonvolatile memory.

RETURN VALUE
  The NV return code, except for NV_NOTACTIVE_S, which is handled
  internally.

DEPENDENCIES
  This routine is not reentrant.  Shouldn't be a problem, as it doesn't exit
  till we're done.
===========================================================================*/
extern nv_stat_enum_type mmgsdi_nv_put_item(
  nv_items_enum_type      item,      /* which item */
  const nv_item_type     *data_ptr,  /* pointer to space for item */
  mmgsdi_nv_context_type  nv_context /* Conrtext of the item to access */
);

/*===========================================================================
FUNCTION MMGSDI_NV_ITEM_CACHE_INIT

DESCRIPTION
  Allocate memory and initialize the items in the cache

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE:
  None

SIDE EFFECTS
  None
===========================================================================*/
extern mmgsdi_return_enum_type mmgsdi_nv_item_cache_init (
  void);

/*===========================================================================
FUNCTION MMGSDI_NV_INIT_FEATURES_STATUS_NV_LOOKUP

DESCRIPTION
  Function used to get the status of MMGSDI features as defined by the NVITEM
  "MMGSDI_FEATURES_STATUS_LIST_FILE".
  The global, "features_status_list", used to hold the NVITEM, is expandable,
  yet the size is fixed. See the struct definition for more detailed doc on
  that. The global struct has a "version" item that is updated everytime we
  add new items to this global struct (starting with version 0 in the first
  implementation)
  The mmgsdi code has full control over version of NVITEM. Taking an example,
  if in future, in version 2 of this struct(that has 10 items), the delta is 3
  items (from version 0 that had 7 items), and this build is loaded onto the
  device that already had the items set using version 0 of this NVITEM, then,
  on boot-up after loading the build, mmgsdi will validate the version it read
  from NV. If it is not 2, it will update it to 2 and write it back to NV. At
  this point, however, the value of features/items 8 through 10 will be set to
  their default values by mmgsdi code.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  "version" item in this NVITEM is writable through QXDM (no QXDM support to
  hide it or grey it out) or QCN. However if what is written explicitly doesn't
  match what's in the build, mmgsdi will change it to what's in the build and
  it could be annoying for the QXDM user to see it change to something else
  (when the user reads the NVITEM next time).
===========================================================================*/
extern void mmgsdi_nv_init_features_status_nv_lookup(
  uint8     index);

/*===========================================================================
FUNCTION MMGSDI_NV_INIT_EXTERNAL_FEATURES_STATUS_NV_LOOKUP

DESCRIPTION
  Function used to get the status of external features (i.e. features
  outside of MMGSDI), stored in EFS based NV.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void mmgsdi_nv_init_external_features_status_nv_lookup(
  uint8     index);

/*===========================================================================
FUNCTION MMGSDI_NV_GET_EXTERNAL_FEATURE_STATUS

DESCRIPTION
  Function used to get the status of external features listed in the enum:
  mmgsdi_nv_external_features_enum.

DEPENDENCIES
  None

RETURN VALUE
  mmgsdi_feature_status_enum

SIDE EFFECTS
  None
===========================================================================*/
extern mmgsdi_feature_status_enum_type mmgsdi_nv_get_external_feature_status(
  mmgsdi_nv_external_features_enum nv_feature,
  mmgsdi_nv_context_type           index
);

/*===========================================================================
FUNCTION MMGSDI_NV_UPDATE_RPM_ICCID

DESCRIPTION
  Compare the previously stored ICCID to the current ICCID of the card.  If the
  values are different update nv with the new ICCID from the card.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE:
  None

SIDE EFFECTS
  rpm_iccid_has_changed_slot will be:
    TRUE : if the ICCID is different than the last card inserted in the ME
    FALSE: if the ICCID is the same as the last card inserted in the ME
===========================================================================*/
void mmgsdi_nv_update_rpm_iccid (
  mmgsdi_data_type         new_rpm_iccid,
  mmgsdi_slot_id_enum_type mmgsdi_slot
);

/*===========================================================================
FUNCTION MMGSDI_NV_GET_RPM_ICCID_HAS_CHANGED

DESCRIPTION
  Return whether the ICCID is different than the last ICCID read from the card

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE:
  Boolean - TRUE : if the ICCID is changed
            FALSE: if the ICCID is the same

SIDE EFFECTS
  None
===========================================================================*/
boolean mmgsdi_nv_get_rpm_iccid_has_changed (
  mmgsdi_slot_id_enum_type mmgsdi_slot
);

/*===========================================================================
FUNCTION MMGSDI_NV_GET_SLOT_FEATURE_STATUS

DESCRIPTION
  Function used to get the status of MMGSDI features listed in enum
  mmgsdi_nv_features_enum_type  as defined by the NVITEM  -
  "MMGSDI_SLOT_FEATURES_STATUS_LIST".

  Certain features in this list are operator-specific (refer to
  mmgsdi_nv_is_feature_item_slot_based() for such features) which means those
  features can be disabled or enabled based on which card is present in
  that slot. For querying status of such items, the slot id needs to be
  passed as an argument to this function.
  For querying the status of all other items which are ME-specific and not
  card-dependent,  MMGSDI_SLOT_1 should be passed.

DEPENDENCIES
  Minimum QXDM version required to be able to read/write this NVITEM is
  QXDM 3.14.1047.

RETURN VALUE
  mmgsdi_feature_status_enum

SIDE EFFECTS
  None
===========================================================================*/
mmgsdi_feature_status_enum_type mmgsdi_nv_get_slot_feature_status(
  mmgsdi_nv_slot_specific_features_enum_type nv_feature,
  mmgsdi_nv_context_type                     nv_context
);

/*===========================================================================
FUNCTION MMGSDI_NV_GET_DEVICE_FEATURE_STATUS

DESCRIPTION
  Function used to get the status of MMGSDI features listed in enum
  mmgsdi_nv_features_enum_type  as defined by the NVITEM -
  "MMGSDI_DEVICE_FEATURES_STATUS_LIST".

  Certain features in this list are operator-specific (refer to
  mmgsdi_nv_is_feature_item_slot_based() for such features) which means those
  features can be disabled or enabled based on which card is present in
  that slot. For querying status of such items, the slot id needs to be
  passed as an argument to this function.
  For querying the status of all other items which are ME-specific and not
  card-dependent,  MMGSDI_SLOT_1 should be passed.

DEPENDENCIES
  Minimum QXDM version required to be able to read/write this NVITEM is
  QXDM 3.14.1047.

RETURN VALUE
  mmgsdi_feature_status_enum

SIDE EFFECTS
  None
===========================================================================*/
mmgsdi_feature_status_enum_type mmgsdi_nv_get_device_feature_status(
  mmgsdi_nv_device_specific_features_enum_type nv_feature
);

/*===========================================================================
FUNCTION MMGSDI_NV_GET_NV_ITEM_FROM_CACHE

DESCRIPTION
  This function returns MMGSDI_CACHE_INIT along with the NV contents from the
  MMGSDI NV cache, if the NV item is cached by MMGSDI already.
  If the NV item is not yet cached by MMGSDI, the function returns
  MMGSDI_CACHE_NOT_INIT.
  If the NV item is not in the static list of NV items that MMGSDI caches,
  the function returns MMGSDI_CACHE_NOT_FOUND.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE:
  mmgsdi_cache_init_enum_type

SIDE EFFECTS
  None
===========================================================================*/
mmgsdi_cache_init_enum_type mmgsdi_nv_get_nv_item_from_cache (
  nv_items_enum_type         item_enum,
  mmgsdi_nv_item_cache_type *mmgsdi_nv_data_ptr,
  mmgsdi_nv_context_type     nv_context
);

/*===========================================================================
FUNCTION MMGSDI_NV_INIT_ENS_SUB_FEATURES_STATUS_NV_LOOKUP

DESCRIPTION
  Function used to get the status of ens sub features stored in EFS based NV.
  If it fails to read, it sets the mmgsdi_ens_sub_features_status_list members
  to a default value.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void mmgsdi_nv_init_ens_sub_features_status_nv_lookup(
  void
);

/*===========================================================================
FUNCTION MMGSDI_NV_GET_ENS_SUB_FEATURE_STATUS

DESCRIPTION
  Function used to get the status of ENS sub features

DEPENDENCIES
  None

RETURN VALUE
  mmgsdi_feature_status_enum

SIDE EFFECTS
  None
===========================================================================*/
mmgsdi_feature_status_enum_type mmgsdi_nv_get_ens_sub_feature_status(
  mmgsdi_cached_nv_item_enum_type ens_feature
);

#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */

/*===========================================================================
FUNCTION MMGSDI_NV_INIT_ENS_SLOT_ENABLED

DESCRIPTION
  Function used to get the status of ENS slot enabled feature stored
  in EFS for each slot.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mmgsdi_nv_init_ens_slot_enabled(
  uint8     slot_index);

/*===========================================================================
FUNCTION MMGSDI_NV_IS_ENS_ENABLED

DESCRIPTION
  Function used to get the status of ENS slot enabled feature for the given
  slot

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean mmgsdi_nv_is_ens_enabled(
  mmgsdi_slot_id_enum_type     slot);


/*===========================================================================
FUNCTION MMGSDI_NV_REFRESH_SLOT_ITEMS

DESCRIPTION
  Function to refresh the slot-specific items from NV/EFS at the given index.

DEPENDENCIES
  None

RETURN VALUE
  mmgsdi_return_enum_type

SIDE EFFECTS
  None
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_nv_refresh_slot_items(
  mcfg_refresh_index_type    refresh_index);

/*===========================================================================
FUNCTION MMGSDI_NV_REFRESH_SESSION_ITEMS

DESCRIPTION
  Function to refresh the session-specific items from NV/EFS at the given
  index.

DEPENDENCIES
  None

RETURN VALUE
  mmgsdi_return_enum_type

SIDE EFFECTS
  None
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_nv_refresh_session_items(
  mcfg_refresh_index_type    sub_index
);

/*===========================================================================
  FUNCTION MMGSDI_UTIL_GET_EFS_ITEM_INDEX_FOR_SLOT

  DESCRIPTION
    Returns the index of the EFS item based on the slot id.

  PARAMETERS
    slot id
    efs_index_ptr

  RETURN VALUE
    mmgsdi_return_enum_type

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
mmgsdi_nv_context_type mmgsdi_util_get_efs_item_index_for_slot(
  mmgsdi_slot_id_enum_type  slot_id
);

/*===========================================================================
  FUNCTION MMGSDI_NV_CLEAR_NV_ITEM_CACHE

  DESCRIPTION
    Clears the nv cache for the item specified for that context.

  PARAMETERS
    item_enum
    nv_context

  RETURN VALUE
    void

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void mmgsdi_nv_clear_nv_item_cache (
  nv_items_enum_type     item_enum,
  mmgsdi_nv_context_type nv_context
);

/*===========================================================================
  FUNCTION MMGSDI_UTIL_GET_COMMON_EFS_ITEM_CONTEXT

  DESCRIPTION
    Returns the index of the common EFS item context based on the index.

  PARAMETERS
    index

  RETURN VALUE
    uim_common_efs_context_type

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
uim_common_efs_context_type mmgsdi_util_get_common_efs_item_context(
  uint8 index
);

/*===========================================================================
  FUNCTION MMGSDI_UTIL_GET_EFS_ITEM_CONTEXT()

  DESCRIPTION
    Returns the index of the EFS item based on the session id.

  PARAMETERS
    session id
    efs_context_ptr

  RETURN VALUE
    mmgsdi_return_enum_type

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_util_get_efs_item_context(
  mmgsdi_session_id_type        session_id,
  uim_common_efs_context_type  *efs_context_ptr
);

/* ============================================================================
FUNCTION MMGSDI_UTIL_GET_REMOTE_TEST_PARAMS

DESCRIPTION
  This function checks if the test EFS item for Remote SIM is present. If
  present and slot ID matches the requested slot ID, it reads the file.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  boolean:  TRUE : If it is read successfully for the requested slot
            FALSE: If slot not specified in EFS or file not present

SIDE EFFECTS
  None
============================================================================*/
boolean mmgsdi_nv_get_remote_test_params(
  mmgsdi_slot_id_enum_type              mmgsdi_slot_id,
  mmgsdi_remote_sim_test_params_type  * test_param_ptr
);


/*===========================================================================
  FUNCTION MMGSDI_NV_CONVERT_MMGSDI_NV_CONTEXT

  DESCRIPTION
    Convert mmgsdi_nv_context_type to nv_cmd_ext_type.context

  PARAMETERS
    mmgsdi_nv_context     : mmgsdi_nv_context_type

  RETURN VALUE
    nv context: uint8

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
uint8 mmgsdi_nv_convert_mmgsdi_nv_context(
  mmgsdi_nv_context_type  mmgsdi_nv_context
);

/*===========================================================================
FUNCTION MMGSDI_NV_PUT_NV_ITEM_IN_CACHE

DESCRIPTION
  This function copies the NV item to the cache

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE:
  mmgsdi_return_enum_type

SIDE EFFECTS
  None
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_nv_put_nv_item_in_cache (
  nv_items_enum_type     item_enum,
  const nv_item_type    *data_ptr,
  nv_stat_enum_type      item_status,
  mmgsdi_nv_context_type nv_context
);

/*===========================================================================
FUNCTION MMGSDI_NV_GET_OVERRIDE_OPL_PNN_LOOKUP_FOR_NON_RPLMN_FEATURE_STATUS

DESCRIPTION
  Function used to get the status of override_opl_pnn_lookup_for_non_rplmn
  feature status for the given slot

DEPENDENCIES
  None

RETURN VALUE
  mmgsdi_feature_override_feature_status_enum_type

SIDE EFFECTS
  None
===========================================================================*/
mmgsdi_feature_override_feature_status_enum_type mmgsdi_nv_get_override_opl_pnn_lookup_for_non_rplmn_feature_status(
  mmgsdi_slot_id_enum_type     slot
);

#endif /* MMGSDI_NV_H */
