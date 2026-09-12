
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


        M M G S D I  N V  F U N C T I O N S


GENERAL DESCRIPTION

  This file contains the functions accessing MMGSDI NV and EFS data.

                        COPYRIGHT INFORMATION

Copyright (c) 2001 - 2005, 2009 - 2018, 2021 QUALCOMM Technologies, Inc (QTI) and
its licensors. All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/mmgsdi/src/mmgsdi_nv.c#6 $$ $DateTime: 2021/09/09 00:04:08 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
03/25/21   rps     Allow reading of EF IMSI for buggy card
10/13/20   vgd     Allow reading of EF AD for buggy card
06/05/20   sg      Verify pin after pin is unblocked for buggy card
03/17/20   cj      Use well-defined size type for reading EFS file
12/27/18   sg      Added NV check for setting no. of available logical channels
11/30/18   vdc     Purge temporary identities in INIT+FCN refresh mode
05/19/17   nr      Creating a new efs item for override_opl_pnn_lookup_for_non_rplmn
04/24/17   nr      Support to store more FPLMNs in cache for MMGSDI_USIM_FPLMN
04/13/17   me      Support to disable GBA_U service 
03/29/17   nr      Add support to cache PIN1 of the SIM card when PSM mode enabled
03/14/17   nc      Better checks for return values 
02/13/17   sk      Deprecate direct read of GCF NV 947
02/08/17   vdc     Send verify PIN command after enabling the PIN for buggy card
01/26/17   shr     Add support for SIM file fallback to File System 
01/12/17   nc      Better check of return values 
01/10/17   bcho    Support for slot specific CDMA LESS NV feature
12/28/16   ll      Add test mode logging enabled APIs for sensitive info control
10/06/16   me      Removed functions related to GSTK
09/22/16   kv      Add crit sect protection for NV/EFS access
09/21/16   bcho    Handling of parallel requests in NVRUIM
06/29/16   shr     Support for NV based Toolkit disablement
06/19/16   bcho    F3 frequency reduction by conditional logic
05/20/16   vdc     Remove F3 messages for memory allocation failure
05/13/16   nr      Skip Search Req for PB files
05/12/16   vdc     Remove excessive logging
05/10/16   vdc     Guard with FEATURE_MMGSDI_3GPP2 for 3GPP2 related functions
04/13/16   ar      CDMA/HDR support on 2nd SUB
03/28/16   kv      Added support to handle remote SIM Params
03/14/16   av      Rework internal function used by MMGSDI to read NV items
01/22/16   bcho    Featurize use of non-gsma entries
12/04/15   bcho    Invoke gsdi_fs_get_file_size with uint32 type length param
10/05/15   sp      Reduce signed/unsigned conversions in UIM
10/06/15   kv      Remove diag access to security sensitive NVs in FTM mode
09/25/15   kv      Re-cache RTRE NV on MCFG refresh
09/10/15   kv      Features_status_list is split into device and slot items
08/28/15   hh      Removed JCDMA related code
08/25/15   bcho    Support for CDMALess Device
08/21/15   at      Support for slot type in MMGSDI APIs
08/13/15   sp      merging mmgsdiutil_mem_free macros into one
08/11/15   av      Remove code related to NV 3461 (NV_ENS_ENABLED_I)
07/24/15   lxu     enable MMGSDI_FEATURE_IGNORE_INIT_ERROR by default
06/18/15   hh      Replace nv_cmd() with nv_cmd_ext()
06/05/15   nr      Extending support for refresh retry to multi slot
05/21/15   av      Use MCFG Context enum instead of typecast
04/29/15   bcho    Support for App specific cache
04/29/15   hh      Differentiate CSG support for W and LTE for EF-TST
04/21/15   kk      Fixed ENS settings after NV Refresh
04/03/15   av      Migrate to mcfg EFS APIs
03/24/15   kk      Fixed ver number in 67211 table
03/03/15   lm      Remove double semicolon at end of the line
02/20/15   vv      Remove perso related code
12/22/14   av      Featurize SE13_TABLE_LOOKUP_GET_FIRST_GSMA_MATCH default
12/03/14   bcho    featurize support to use stored rplmn name across operators
12/01/14   av      Featurize FEATURE_OPL_PNN_LOOKUP_FOR_NON_RPLMN default
11/26/14   av      Disable CSG feature by default
11/14/14   hh      Fix compilation warnings
11/12/14   hh      Use default value of NV 70361 when read fails
11/06/14   hh      Call GSTK NV API to read GSTK features in NV 65674
10/14/14   av      Handle appropriately zero ICCID reported by uimdrv
10/14/14   tl      Fix compiler warnings
10/14/14   am      Compilation warnings elimination
10/12/14   hh      Add support to get Voice Domain Preference
10/02/14   kk      Fixed KW error
09/30/14   kk      Enhancements in NV handling
09/24/14   gm      Remove GOBI mode code
09/30/14   ar      Support to store more FPLMNs in cache for MMGSDI_GSM_FPLMN
08/29/14   hh      Fix compliation warnings
08/06/14   ar      Replace MMGSDIUTIL_TMC_MEM_MALLOC_AND_VALIDATE with new macro
07/30/14   ar      Revert FR 20469 changes to align with NAS check-in dates
07/28/14   av      Fix version23 of NV item 67211
07/25/14   av      Auto onchip activation for multi slots
07/25/14   ar      Support to store more FPLMNs in cache for MMGSDI_GSM_FPLMN
07/16/14   yt      Use correct size for memset()
07/15/14   yt      Support for NV refresh
07/14/14   tl      Remove always ON feature FEATURE_MODEM_RCINIT
06/02/14   bcho    New item to decide on blocking of CDMA EPRL access
05/09/14   vv      Deprecate MMGSDI_FEATURE_UICC_RAT_INDICATOR_SUPPORT
04/06/14   am      Added mmgsdi heap alloc clean up logic
04/01/14   av      NVRUIM headers cleanup
03/25/14   vdc     Added new item for verifying PIN when PIN is changed
02/27/14   dy      Add default_apn, bip_apn_mapping_tbl, rat_priority NV item
02/25/14   hh      Purge temporary mobile identities during UICC Reset Refresh
02/25/14   bcho    Added support to store OPLMN List in EFS and cache it
01/16/14   av      New item for deciding bw card and nv for 3gpd credentials
01/10/14   bcho    Refresh 3G Session Reset as per ETSI spec under NV feature
01/10/14   df      Use v2.0 diag macros
01/09/14   ar      Replace old Macros with MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE
12/24/13   am      Added NV protection to Recovery API
12/18/13   yt      Skip app termination as part of session deactivation
12/09/13   vdc     Send RTRE configuration to NV asynchronously
12/05/13   ar      Replaced all mmgsdi_malloc direct calls with macro
11/21/13   vv      Halt 3gpp subscription if 3gpp2 perso fails
10/17/13   am      Added new item for plmn comparison in the nvitem 67211
10/08/13   tl      Addition of an RPM info EFS structure type
10/04/13   bcho    F3 message reduction
10/04/13   vv      Added NV support for ENS sub-features
09/26/13   av      Enable OnChip on bootup based on EFS item
08/30/13   tl      Convert channel info table from static to dynamic allocation
08/20/13   vv      Update EF-TST to indicate support for IMS
08/20/13   spo     Automatic Subscription provisioning enhancements in UIM
07/29/13   am      Added support to determine CSG support dynamically
07/26/13   tkl     Add NV support for enabling use of SMS-PP envelope command
06/24/13   vdc     Added support for triple SIM
05/17/13   av      Halt 3gpp2 subscription if 3gpp perso fails
05/16/13   vdc     Replace memcpy with safer version memscpy
05/15/13   bcho    Added new items to mmgsdi features status nvitem 67211
05/13/13   av      Fixed KW errors
04/25/13   vdc     Added select_dfcdma to mmgsdi feature status nvitem 67211
04/17/13   tl      Replace Featurization with NV
04/11/13   tkl     Add NV suport for SETUP Call display alpha value
04/05/13   av      Move UIM tasks to group4 of RCINIT
04/04/13   bcho    Added new items to mmgsdi features status nvitem 67211
03/27/13   spo     Added new items to mmgsdi feature status list
02/19/13   av      Merge mmgsdi,gstk,qmiuim/cat conf files to mmgsdi.conf
01/04/13   vv      Handle NV write command in main task
01/03/13   abg     Added API mmgsdi_nv_init_read_and_cache_nv_items to read NV during task-init
12/26/12   av      Do not allow writing to the file that is being refreshed
11/05/12   av      Expand features status list nvitem to control skipping
                   checking security attributes before reading EF
10/31/12   tl      CSIM conformance updating EF MECRP
10/31/12   tl      CSIM conformance updating EF MODEL
10/21/12   av      Update conf file only if necessary
10/16/12   yt      Add new items to mmgsdi feature status list
09/28/12   yt      Update user profile parameters after successful OTASP Commit
08/24/12   abg     fix default value of ignore_init_error
04/20/12   av      Enabled detouring of nv/efs calls to offtarget framework
03/08/12   av      Added new items to mmgsdi features status nvitem 67211
01/30/12   shr     Use GSTK NV item (65674) to determine if the OTASP IMSI_M
                   enhancement is enabled or not
01/13/12   at      Added NV item caching for NV_DISABLE_CM_CALL_TYPE_I
12/21/11   shr     Legacy GSDI removal updates
11/15/11   nmb     RPM Updates
10/10/11   av      Updated default values of MMGSDI Features
09/16/11   av      Replace feature #ifdefs with an NVITEM
07/12/11   at      Added NV item caching for NV_SERVICE_DOMAIN_PREF_I
06/05/11   vs      Updated NV item caching logic to include NV_ESN_ME_I
12/22/10   yt      Removed FEATURE_UIM_ZI_MEMORY_REDUCTION
11/24/10   js      Resolve race condition between GSDI NV READ/WRITE
10/25/10   nmb     Copy NV item data to NV command buffer
10/04/10   nmb     Added NV queue to address NV deadlocks
09/08/10   nmb     mmgsdi_task_cmd_type memory reduction
07/14/10   js      Fixed compilation warnings
05/13/10   vs      Fix for NV and GSDI deadlock
10/21/09   rn      Fixed klocwork errors
09/26/09   kp      ZI memory reduction changes
07/20/09   mib     Replaced flag TEST_FRAMEWORK with FEATURE_UIM_TEST_FRAMEWORK
02/27/09   nd      Virtuim changes for QTF
03/08/06   jar     Merged in Branched SIM Lock
05/11/05   sst     Lint fixes
11/15/04   jar     Clear out GSDI NV Signal when NV Response is received
02/21/04   jar     Removed gsdi_wait_nv() since it is not being used and
                   causes compiler errors.
11/05/03   jar     Added support for Slot Preference Changes.  Switch from
                   gsdi_wait_nv() to gsdi_wait() to allow for GSDI_RPT_SIG
                   handling.
07/02/03   tml     Fixed gsdi_put_nv
12/05/02   jar     Added #include "err.h" for ERR Macros.
11/26/02   jar     Removed RTRE Code.
08/30/02 jar/ck    Added gsdi_send_rtre_command.  changed gsdi_nvi to gsdi_nv
                   _cmd.  Localized gsdi_wait_nv.  Added internal feature
                   MMGSDI_RTRE.
08/18/02   jar     Created gsdi_nv.h
===========================================================================*/


/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "mmgsdi_nv.h"
#include "nv.h"
#include "uim_msg.h"
#include "err.h"
#include "mmgsdiutil.h"
#include "mmgsdi_refresh.h"
#include "mmgsdisessionlib_v.h"
#include "mmgsdi.h"
#include "queue.h"
#include "fs_public.h"
#include "gstk_exp_v.h"
#include "mmgsdilib_v.h"
#include "uim_common_efs.h"
#include "mmgsdi_refresh.h"
#include "mmgsdi_efs.h"
#include "mmgsdilib_p.h"

#include "mcfg_fs.h"

#include "fs_lib.h"
#include "fs_public.h"
#include "fs_diag_access.h"
#include "fs_stdlib.h"

#ifdef FEATURE_FEMTO_CSG
#include "sys_v.h"
#endif /* FEATURE_FEMTO_CSG */
#include "sys.h"

#include "qmi_uim.h"
#include "qmi_cat.h"
#ifdef FEATURE_GSTK
#include "gstk_nv_p.h"
#endif /* FEATURE_GSTK */
#ifdef FEATURE_ESTK
#include "estk.h"
#endif /* FEATURE_ESTK */

#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */

static mmgsdi_nv_item_cache_table_type *mmgsdi_nv_item_cache_table_ptr[MMGSDI_NV_CONTEXTS_MAX];

/* defines for RPM ICCID change identification support */
#define MMGSDI_RPM_ICCID_FILE "/nv/item_files/modem/uim/mmgsdi/rpm_iccid"

/* MAX version until where the backward compatability of the legacy
    features_status_list is supported */
#define MMGSDI_MAX_VER_LEGACY_FEATURES_LIST_SUPPORTED        25

/* Max possible feature-elements in SLOT_FEATURES_STATUS_LIST_TYPE NVITEM struct */
#define MMGSDI_MAX_ITEMS_IN_SLOT_SPECIFIC_FEATURE_NVITEM    100

/* Max possible feature-elements in DEVICE_FEATURES_STATUS_LIST_TYPE NVITEM struct */
#define MMGSDI_MAX_ITEMS_IN_DEVICE_SPECIFIC_FEATURE_NVITEM  100

/* ----------------------------------------------------------------------------
   STRUCTURE:      MMGSDI_DEVICE_SPECIFIC_FEATURES_STATUS_LIST

   DESCRIPTION:
     Data Structure to hold status of mmgsdi device specific features as it reads it from
     efs NVITEM (73763) "MMGSDI_DEVICE_SPECIFIC_FEATURES_STATUS_LIST_TYPE"
     * Size of this struct should remain the SAME at all times.
     * Everytime new mmgsdi item(s) is/are required to be exposed to the
       users via this NVITEM
       - add the entry in mmgsdi_device_features_list_init_table
       - add item in enum "mmgsdi_nv_device_specific_features_enum_type"
       - Notify QXDM team of the modification.
       - In the release notes, mention the new version of this NVITEM.
       - In the release notes, suggest upgrading QXDM to the version to be
         released two weeks from that date (assuming that QXDM would pick our
         updated NVITEM by then). Otherwise, the user won't be able to see and
         update(set) the new entry in the NVITEM. The MMGSDI code would by
         default set the unset entries to their default values.
-------------------------------------------------------------------------------*/
static struct {
  uint8 version;
  mmgsdi_feature_status_enum_type   item[MMGSDI_MAX_ITEMS_IN_DEVICE_SPECIFIC_FEATURE_NVITEM];
} mmgsdi_device_features_status_list;

/* ----------------------------------------------------------------------------
   STRUCTURE:      MMGSDI_SLOT_SPECIFIC_FEATURES_STATUS_LIST

   DESCRIPTION:
     Data Structure to hold status of mmgsdi slot specific features as it reads it from
     efs NVITEM (73764) "MMGSDI_SLOT_SPECIFIC_FEATURES_STATUS_LIST_TYPE"
     * Size of this struct should remain the SAME at all times.
     * Everytime new mmgsdi item(s) is/are required to be exposed to the
       users via this NVITEM
       - add the entry in mmgsdi_slot_features_list_init_table
       - add item in enum "mmgsdi_nv_slot_specific_features_enum_type"
       - Notify QXDM team of the modification.
       - In the release notes, mention the new version of this NVITEM.
       - In the release notes, suggest upgrading QXDM to the version to be
         released two weeks from that date (assuming that QXDM would pick our
         updated NVITEM by then). Otherwise, the user won't be able to see and
         update(set) the new entry in the NVITEM. The MMGSDI code would by
         default set the unset entries to their default values.
-------------------------------------------------------------------------------*/
static struct {
  uint8 version;
  mmgsdi_feature_status_enum_type   item[MMGSDI_MAX_ITEMS_IN_SLOT_SPECIFIC_FEATURE_NVITEM];
} mmgsdi_slot_features_status_list[MMGSDI_MAX_NUM_SLOTS];

/* ----------------------------------------------------------------------------
   STRUCTURE:      MMGSDI_LEGACY_DEVICE_FEATURES_MAPPING_LIST_TYPE

   DESCRIPTION:
     Data Structure to hold mapping between legacy features status list enums
     to the device specific features status list enums
-------------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{
  mmgsdi_nv_device_specific_features_enum_type device_feature;
  mmgsdi_nv_features_enum_type                 legacy_feature;
} mmgsdi_legacy_device_features_mapping_list_type;

static mmgsdi_legacy_device_features_mapping_list_type mmgsdi_legacy_device_features_mapping_list_table[] =
{
  { MMGSDI_FEATURE_ECC_NOT_PRESENT_SUPPORT,                        MMGSDI_FEATURE_LEGACY_ECC_NOT_PRESENT_SUPPORT},
  { MMGSDI_FEATURE_SPECIAL_USIM_ARR,                               MMGSDI_FEATURE_LEGACY_SPECIAL_USIM_ARR},
  { MMGSDI_FEATURE_CDMA_DF_SELECT_BEFORE_TP,                       MMGSDI_FEATURE_LEGACY_CDMA_DF_SELECT_BEFORE_TP},
  { MMGSDI_FEATURE_CHANGE_INST_CLASS,                              MMGSDI_FEATURE_LEGACY_CHANGE_INST_CLASS},
  { MMGSDI_FEATURE_CARD_SLOT_CHECK,                                MMGSDI_FEATURE_LEGACY_CARD_SLOT_CHECK},
  { MMGSDI_FEATURE_TMSI_PLMN_CHECK,                                MMGSDI_FEATURE_LEGACY_TMSI_PLMN_CHECK},
  { MMGSDI_FEATURE_IGNORE_INIT_ERROR,                              MMGSDI_FEATURE_LEGACY_IGNORE_INIT_ERROR},
  { MMGSDI_FEATURE_ALLOW_SEND_APDU_ONLY,                           MMGSDI_FEATURE_LEGACY_ALLOW_SEND_APDU_ONLY},
  { MMGSDI_FEATURE_PREF_MODE_NOT_FROM_RUIM,                        MMGSDI_FEATURE_LEGACY_PREF_MODE_NOT_FROM_RUIM},
  { MMGSDI_FEATURE_ALLOW_ACCESS_BEFORE_APP_READY,                  MMGSDI_FEATURE_LEGACY_ALLOW_ACCESS_BEFORE_APP_READY},
  { MMGSDI_FEATURE_SKIP_UICC_SECURITY_ATTR_CHECK_BEFORE_READ,      MMGSDI_FEATURE_LEGACY_SKIP_UICC_SECURITY_ATTR_CHECK_BEFORE_READ},
  { MMGSDI_FEATURE_BLOCK_WRITES_TO_REFRESH_FILES,                  MMGSDI_FEATURE_LEGACY_BLOCK_WRITES_TO_REFRESH_FILES},
  { MMGSDI_FEATURE_SE13_TABLE_LOOKUP_GET_FIRST_GSMA_MATCH,         MMGSDI_FEATURE_LEGACY_SE13_TABLE_LOOKUP_GET_FIRST_GSMA_MATCH},
  { MMGSDI_FEATURE_MMGSDI_UNBLOCK_PIN_HANDLE_INVALID_SW,           MMGSDI_FEATURE_LEGACY_MMGSDI_UNBLOCK_PIN_HANDLE_INVALID_SW},
  { MMGSDI_FEATURE_EXPLICIT_SELECT_DFCDMA_BEFORE_TELECOM_ADN_READ, MMGSDI_FEATURE_LEGACY_EXPLICIT_SELECT_DFCDMA_BEFORE_TELECOM_ADN_READ},
  { MMGSDI_FEATURE_OPL_PNN_LOOKUP_FOR_NON_RPLMN,                   MMGSDI_FEATURE_LEGACY_OPL_PNN_LOOKUP_FOR_NON_RPLMN},
  { MMGSDI_FEATURE_MULTISIM_AUTO_PROVISIONING,                     MMGSDI_FEATURE_LEGACY_MULTISIM_AUTO_PROVISIONING},
  { MMGSDI_FEATURE_NORTH_AMERICAN_PLMN_COMPARISON,                 MMGSDI_FEATURE_LEGACY_NORTH_AMERICAN_PLMN_COMPARISON},
  { MMGSDI_FEATURE_SKIP_PROV_APP_TERMINATION,                      MMGSDI_FEATURE_LEGACY_SKIP_PROV_APP_TERMINATION},
  { MMGSDI_FEATURE_REFRESH_3G_SESSION_RESET_PER_ETSI_SPEC,         MMGSDI_FEATURE_LEGACY_REFRESH_3G_SESSION_RESET_PER_ETSI_SPEC},
  { MMGSDI_FEATURE_USE_SFI,                                        MMGSDI_FEATURE_LEGACY_USE_SFI},
  { MMGSDI_FEATURE_VERIFY_PIN_AFTER_CHANGE_OR_ENABLE_OR_UNBLOCK,   MMGSDI_FEATURE_LEGACY_VERIFY_PIN_AFTER_CHANGE_OR_ENABLE_OR_UNBLOCK},
  { MMGSDI_FEATURE_SUPPORT_EXTENDED_FPLMN,                         MMGSDI_FEATURE_LEGACY_SUPPORT_EXTENDED_FPLMN},
  { MMGSDI_FEATURE_USE_STORED_RPLMN_NAME_ACROSS_OPERATORS,         MMGSDI_FEATURE_LEGACY_USE_STORED_RPLMN_NAME_ACROSS_OPERATORS},
  { MMGSDI_FEATURE_KEEP_CACHE_AFTER_SESSION_DEACTIVATION,          MMGSDI_FEATURE_LEGACY_KEEP_CACHE_AFTER_SESSION_DEACTIVATION}
};

/* ----------------------------------------------------------------------------
   STRUCTURE:      MMGSDI_LEGACY_SLOT_FEATURES_MAPPING_LIST_TYPE

   DESCRIPTION:
     Data Structure to hold mapping between legacy features status list enums
     to the slot specific features status list enums
-------------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{
  mmgsdi_nv_slot_specific_features_enum_type slot_feature;
  mmgsdi_nv_features_enum_type               legacy_feature;
} mmgsdi_legacy_slot_features_mapping_list_type;

static mmgsdi_legacy_slot_features_mapping_list_type mmgsdi_legacy_slot_features_mapping_list_table[] =
{
  { MMGSDI_FEATURE_ABSENT_MANDATORY_FILES_INIT,                    MMGSDI_FEATURE_LEGACY_ABSENT_MANDATORY_FILES_INIT},
  { MMGSDI_FEATURE_RUIM_ALLOW_ESN_FAIL,                            MMGSDI_FEATURE_LEGACY_RUIM_ALLOW_ESN_FAIL},
  { MMGSDI_FEATURE_RUIM_ZERO_ESN_FAIL,                             MMGSDI_FEATURE_LEGACY_RUIM_ZERO_ESN_FAIL},
  { MMGSDI_FEATURE_UIM_RUIM_SUPPORT_SCI,                           MMGSDI_FEATURE_LEGACY_UIM_RUIM_SUPPORT_SCI},
  { MMGSDI_FEATURE_UIM_AN_HRPD_FALLBACK,                           MMGSDI_FEATURE_LEGACY_UIM_AN_HRPD_FALLBACK},
  { MMGSDI_FEATURE_UIM_MISCONFIG_RUIM_N5_WORKAROUND,               MMGSDI_FEATURE_LEGACY_UIM_MISCONFIG_RUIM_N5_WORKAROUND},
  { MMGSDI_FEATURE_CSIM_UPP_UPDATE_AFTER_OTASP,                    MMGSDI_FEATURE_LEGACY_CSIM_UPP_UPDATE_AFTER_OTASP},
  { MMGSDI_FEATURE_UIM_GSM_DCS_1800,                               MMGSDI_FEATURE_LEGACY_UIM_GSM_DCS_1800},
  { MMGSDI_FEATURE_USE_3GPD_CREDENTIALS_FROM_NV,                   MMGSDI_FEATURE_LEGACY_USE_3GPD_CREDENTIALS_FROM_NV},
  { MMGSDI_FEATURE_OPLMN_LIST_CONFIGURABLE_OTA,                    MMGSDI_FEATURE_LEGACY_OPLMN_LIST_CONFIGURABLE_OTA},
  { MMGSDI_FEATURE_PURGE_TEMP_IDENTITIES,                          MMGSDI_FEATURE_LEGACY_PURGE_TEMP_IDENTITIES},
  { MMGSDI_FEATURE_BLOCK_RUIM_EPRL_ACCESS,                         MMGSDI_FEATURE_LEGACY_BLOCK_RUIM_EPRL_ACCESS}
};

/* ----------------------------------------------------------------------------
   STRUCTURE:      MMGSDI_SLOT_FEATURES_LIST_TYPE

   DESCRIPTION:
     Data Structure to hold the slot specific features status list default
     values and the version introduced
-------------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{
  mmgsdi_nv_slot_specific_features_enum_type feature;
  uint8                                      version_introduced;
  mmgsdi_feature_status_enum_type            default_value;
} mmgsdi_slot_features_list_type;

static mmgsdi_slot_features_list_type mmgsdi_slot_features_list_init_table[] =
{
  { MMGSDI_FEATURE_ABSENT_MANDATORY_FILES_INIT,                                      0, MMGSDI_FEATURE_ENABLED   },
  { MMGSDI_FEATURE_RUIM_ALLOW_ESN_FAIL,                                              2, MMGSDI_FEATURE_DISABLED  },
  { MMGSDI_FEATURE_RUIM_ZERO_ESN_FAIL,                                               2, MMGSDI_FEATURE_ENABLED   },
  { MMGSDI_FEATURE_UIM_RUIM_SUPPORT_SCI,                                             2, MMGSDI_FEATURE_ENABLED   },
  { MMGSDI_FEATURE_UIM_AN_HRPD_FALLBACK,                                             2, MMGSDI_FEATURE_DISABLED  },
  { MMGSDI_FEATURE_UIM_MISCONFIG_RUIM_N5_WORKAROUND,                                 2, MMGSDI_FEATURE_ENABLED   },
  { MMGSDI_FEATURE_CSIM_UPP_UPDATE_AFTER_OTASP,                                      3, MMGSDI_FEATURE_DISABLED  },
  { MMGSDI_FEATURE_UIM_GSM_DCS_1800,                                                 9, MMGSDI_FEATURE_DISABLED  },
  { MMGSDI_FEATURE_USE_3GPD_CREDENTIALS_FROM_NV,                                    18, MMGSDI_FEATURE_DISABLED  },
  { MMGSDI_FEATURE_OPLMN_LIST_CONFIGURABLE_OTA,                                     20, MMGSDI_FEATURE_DISABLED  },
  { MMGSDI_FEATURE_PURGE_TEMP_IDENTITIES,                                           20, MMGSDI_FEATURE_DISABLED  },
  { MMGSDI_FEATURE_BLOCK_RUIM_EPRL_ACCESS,                                          22, MMGSDI_FEATURE_DISABLED  },
  { MMGSDI_FEATURE_SKIP_SEARCH_FOR_PB_FILES,                                        23, MMGSDI_FEATURE_DISABLED  },
  { MMGSDI_FEATURE_CDMA_LESS,                                                       24, MMGSDI_FEATURE_DISABLED  },
  { MMGSDI_FEATURE_DISABLE_GBA_U,                                                   25, MMGSDI_FEATURE_DISABLED  },
  { MMGSDI_FEATURE_CARD_RESET_ON_REFRESH_FAILURE,                                   26, MMGSDI_FEATURE_DISABLED  },
  { MMGSDI_FEATURE_PERSISTENT_CACHE,                                                27, MMGSDI_FEATURE_DISABLED  },
  { MMGSDI_FEATURE_OVERRIDE_USE_SFI_TO_FALSE,                                       28, MMGSDI_FEATURE_DISABLED  },
  { MMGSDI_FEATURE_PURGE_TEMP_IDENTITIES_INIT_FCN_REFRESH_WITH_IMSI_CHANGE,         29, MMGSDI_FEATURE_DISABLED  },
  { MMGSDI_FEATURE_PURGE_TEMP_IDENTITIES_3G_SESSION_RESET_REFRESH_WITH_IMSI_CHANGE, 30, MMGSDI_FEATURE_DISABLED  },
  { MMGSDI_FEATURE_PRIORITIZE_5G_RAT,                                               31, MMGSDI_FEATURE_DISABLED  },
  { MMGSDI_FEATURE_IGNORE_FEW_PLMNS_IN_EHPLMN,                                      32, MMGSDI_FEATURE_DISABLED  },
  { MMGSDI_FEATURE_IGNORE_READ_SEC_ATTRIBUTES_FOR_EF_USIM_AD,                       33, MMGSDI_FEATURE_DISABLED  },
  { MMGSDI_FEATURE_IGNORE_READ_SEC_ATTRIBUTES,                                      34, MMGSDI_FEATURE_DISABLED  }
};

/* ----------------------------------------------------------------------------
   STRUCTURE:      MMGSDI_DEVICE_FEATURES_LIST_TYPE

   DESCRIPTION:
     Data Structure to hold the device specific features status list default
     values and the version introduced
-------------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{
  mmgsdi_nv_device_specific_features_enum_type feature;
  uint8                                        version_introduced;
  mmgsdi_feature_status_enum_type              default_value;
} mmgsdi_device_features_list_type;

static mmgsdi_device_features_list_type mmgsdi_device_features_list_init_table[] =
{
  { MMGSDI_FEATURE_ECC_NOT_PRESENT_SUPPORT,                         0, MMGSDI_FEATURE_ENABLED  },
  { MMGSDI_FEATURE_SPECIAL_USIM_ARR,                                0, MMGSDI_FEATURE_ENABLED  },
  { MMGSDI_FEATURE_CDMA_DF_SELECT_BEFORE_TP,                        0, MMGSDI_FEATURE_ENABLED  },
  { MMGSDI_FEATURE_CHANGE_INST_CLASS,                               0, MMGSDI_FEATURE_ENABLED  },
  { MMGSDI_FEATURE_CARD_SLOT_CHECK,                                 0, MMGSDI_FEATURE_DISABLED },
  { MMGSDI_FEATURE_TMSI_PLMN_CHECK,                                 0, MMGSDI_FEATURE_DISABLED },
  { MMGSDI_FEATURE_IGNORE_INIT_ERROR,                               2, MMGSDI_FEATURE_ENABLED  },
  { MMGSDI_FEATURE_ALLOW_SEND_APDU_ONLY,                            2, MMGSDI_FEATURE_DISABLED },
  { MMGSDI_FEATURE_PREF_MODE_NOT_FROM_RUIM,                         2, MMGSDI_FEATURE_ENABLED  },
  { MMGSDI_FEATURE_ALLOW_ACCESS_BEFORE_APP_READY,                   4, MMGSDI_FEATURE_DISABLED },
  { MMGSDI_FEATURE_SKIP_UICC_SECURITY_ATTR_CHECK_BEFORE_READ,       5, MMGSDI_FEATURE_ENABLED  },
  { MMGSDI_FEATURE_BLOCK_WRITES_TO_REFRESH_FILES,                   6, MMGSDI_FEATURE_ENABLED  },
#ifdef FEATURE_CUST_1
  { MMGSDI_FEATURE_SE13_TABLE_LOOKUP_GET_FIRST_GSMA_MATCH,          7, MMGSDI_FEATURE_ENABLED  },
#else
  { MMGSDI_FEATURE_SE13_TABLE_LOOKUP_GET_FIRST_GSMA_MATCH,          7, MMGSDI_FEATURE_DISABLED },
#endif /* FEATURE_CUST_1 */
  { MMGSDI_FEATURE_MMGSDI_UNBLOCK_PIN_HANDLE_INVALID_SW,            9, MMGSDI_FEATURE_DISABLED },
  { MMGSDI_FEATURE_EXPLICIT_SELECT_DFCDMA_BEFORE_TELECOM_ADN_READ, 10, MMGSDI_FEATURE_DISABLED },
  /* This feature is overridden by UIM_COMMON_EFS_MMGSDI_OVERRIDE_OPL_PNN_LOOKUP_FOR_NON_RPLMN
     when it is not set or not provisioned */
#ifdef FEATURE_CUST_1
  { MMGSDI_FEATURE_OPL_PNN_LOOKUP_FOR_NON_RPLMN,                   11, MMGSDI_FEATURE_ENABLED  },
#else
  { MMGSDI_FEATURE_OPL_PNN_LOOKUP_FOR_NON_RPLMN,                   11, MMGSDI_FEATURE_DISABLED },
#endif /* FEATURE_CUST_1 */
  { MMGSDI_FEATURE_MULTISIM_AUTO_PROVISIONING,                     13, MMGSDI_FEATURE_DISABLED },
  { MMGSDI_FEATURE_NORTH_AMERICAN_PLMN_COMPARISON,                 14, MMGSDI_FEATURE_ENABLED  },
  { MMGSDI_FEATURE_SKIP_PROV_APP_TERMINATION,                      16, MMGSDI_FEATURE_ENABLED  },
  { MMGSDI_FEATURE_REFRESH_3G_SESSION_RESET_PER_ETSI_SPEC,         17, MMGSDI_FEATURE_DISABLED },
  { MMGSDI_FEATURE_USE_SFI,                                        19, MMGSDI_FEATURE_ENABLED  },
  { MMGSDI_FEATURE_VERIFY_PIN_AFTER_CHANGE_OR_ENABLE_OR_UNBLOCK,   21, MMGSDI_FEATURE_DISABLED },
  { MMGSDI_FEATURE_SUPPORT_EXTENDED_FPLMN,                         23, MMGSDI_FEATURE_ENABLED  },
  { MMGSDI_FEATURE_USE_STORED_RPLMN_NAME_ACROSS_OPERATORS,         24, MMGSDI_FEATURE_ENABLED  },
  { MMGSDI_FEATURE_KEEP_CACHE_AFTER_SESSION_DEACTIVATION,          25, MMGSDI_FEATURE_ENABLED  },
  { MMGSDI_FEATURE_SE13_TABLE_LOOKUP_USE_NON_GSMA_ENTRY,           26, MMGSDI_FEATURE_ENABLED  },
  { MMGSDI_FEATURE_SIM_FILE_FALLBACK_TO_FS,                        27, MMGSDI_FEATURE_ENABLED  },
  { MMGSDI_FEATURE_DISABLE_APDU_CONVERSION_ON_CH_0,                28, MMGSDI_FEATURE_DISABLED },
  { MMGSDI_FEATURE_DEFAULT_LEGACY_LOGICAL_CHANNELS,                29, MMGSDI_FEATURE_DISABLED }
};

/*---------------------------------------------------------------------------
  To store list of status of external features (i.e. features outside of MMGSDI)
  for each slot/subscription.
     nas_cfg_feature_csg:         Status of CSG Feature (NAS NV 70361)
     ims_cfg_feature_ims_support: Status of IMS Support (IMS NV 67218)
     rpm_enabled: Indicates whether RPM feature is enabled
                                  (from MMODE NV 67210 )
     volte_using_ims:             VOLTE using IMS (CM NV 66048)
                                  The following values are considered as supported:
                                    SYS_VOICE_DOMAIN_PREF_IMS_PS_VOICE_ONLY
                                    SYS_VOICE_DOMAIN_PREF_CS_VOICE_PREFERRED
                                    SYS_VOICE_DOMAIN_PREF_IMS_PS_VOICE_PREFERRED
-------------------------------------------------------------------------------*/
static struct {
  mmgsdi_feature_status_enum_type  nas_cfg_feature_csg;
  mmgsdi_feature_status_enum_type  ims_cfg_feature_ims_support;
  mmgsdi_feature_status_enum_type  rpm_enabled;
  mmgsdi_feature_status_enum_type  volte_using_ims;
} mmgsdi_external_features_status_list[MMGSDI_MAX_NUM_SLOTS];

/*---------------------------------------------------------------------------
  To store the status of ENS feature for each slot
---------------------------------------------------------------------------*/
static boolean mmgsdi_ens_slot_enabled[MMGSDI_MAX_NUM_SLOTS];

/*---------------------------------------------------------------------------
  To store status of mmgsdi ENS sub features
---------------------------------------------------------------------------*/
static mmgsdi_ens_sub_features_status_list_type mmgsdi_ens_sub_features_status_list;

/*---------------------------------------------------------------------------
  To store status of mmgsdi override_opl_pnn_lookup_for_non_rplmn feature
---------------------------------------------------------------------------*/
static mmgsdi_feature_override_feature_status_enum_type override_opl_pnn_lookup_for_rplmn[MMGSDI_MAX_NUM_SLOTS];

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
)
{
  switch (mmgsdi_nv_context)
  {
    case MMGSDI_NV_CONTEXT_DEVICE_SPECIFIC:
    case MMGSDI_NV_CONTEXT_PRIMARY:
      return 0;
    case MMGSDI_NV_CONTEXT_SECONDARY:
      return 1;
    case MMGSDI_NV_CONTEXT_TERTIARY:
      return 2;
    default:
      break;
  }

  return 0;
} /* mmgsdi_nv_convert_mmgsdi_nv_context */


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
nv_stat_enum_type mmgsdi_nv_get_item(
  nv_items_enum_type         item,
  mmgsdi_nv_item_cache_type *mmgsdi_nv_data_ptr,
  mmgsdi_nv_context_type     nv_context
)
{
  nv_cmd_type                   *mmgsdi_nv_cmd_ptr        = NULL;
  nv_cmd_ext_type                mmgsdi_nv_cmd_ext        = {{0}};
  nv_stat_enum_type              nv_status                = NV_NOTALLOC_S;
  mmgsdi_task_enum_type          mmgsdi_task              = MMGSDI_TASK_MAX;
  nv_item_type                  *nv_data_ptr              = NULL;
  mmgsdi_cache_init_enum_type    mmgsdi_nv_caching_status = MMGSDI_CACHE_NOT_FOUND;

  UIM_MSG_HIGH_1("In mmgsdi_nv_get_item, item: 0x%x", item);

  /* Get current task info */
  mmgsdi_task = mmgsdi_util_get_current_task();

  if(mmgsdi_task >= MMGSDI_NUM_TASKS)
  {
    UIM_MSG_ERR_0("Could not determine MMGSDI Task");
    return NV_BADPARM_S;
  }

  /* Device specific items are accessed from primary context,
     if the context is DEVICE SPECIFIC, convert it into PRIMARY CONTEXT */
  if(nv_context == MMGSDI_NV_CONTEXT_DEVICE_SPECIFIC)
  {
    nv_context = MMGSDI_NV_CONTEXT_PRIMARY;
  }

  mmgsdi_nv_caching_status =
    mmgsdi_nv_get_nv_item_from_cache(item, mmgsdi_nv_data_ptr, nv_context);

  if(MMGSDI_CACHE_NOT_FOUND == mmgsdi_nv_caching_status)
  {
    /* This NV item is not in the list of NV items cached by MMGSDI. */
    return NV_NOTALLOC_S;
  }
  else if(MMGSDI_CACHE_INIT == mmgsdi_nv_caching_status)
  {
    /* NV items is cached by MMGSDI already. Return value from cache rather
       than send the NV command to NV module */
    return mmgsdi_nv_data_ptr->item_status;
  }

  /* We reached here because MMGSDI does not have the NV item cached. Read it
     via the NV module */

  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(mmgsdi_nv_cmd_ptr, sizeof(nv_cmd_type));

  if(mmgsdi_nv_cmd_ptr == NULL)
  {
    return NV_NOTALLOC_S;
  }

  /* Set mmgsdi_nv_cmd_ext */
  mmgsdi_nv_cmd_ext.nvcmd = mmgsdi_nv_cmd_ptr;
  mmgsdi_nv_cmd_ext.context = mmgsdi_nv_convert_mmgsdi_nv_context(nv_context);

  /* Provide signal to be set */
  mmgsdi_nv_cmd_ptr->sigs = MMGSDI_NV_READ_SIG;

  /* Card EF based NVs should not be read in async manner. Because first NV
     module puts  request in their queue and then again it gets queued to
     NVRUIM task. NVRUIM may already have multiple requests queued so MMGSDI
     needs to wait till its request get processed by NVRUIM.
     Also it may lead to deadlock as here MMGSDI waits for signal to be set
     after NV read and MMGSDI can not process Read request because it is in
     wait state. MMGSDI can't differentiate between card based and EFS based
     NVs so always read NV items synchronously*/
  mmgsdi_nv_cmd_ptr->tcb_ptr = NULL;

  /* Command goes on no queue when done */
  mmgsdi_nv_cmd_ptr->done_q_ptr = NULL;

  /* Item to read */
  mmgsdi_nv_cmd_ptr->item = item;
  mmgsdi_nv_cmd_ptr->cmd  = NV_READ_F;

  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(nv_data_ptr,
                                     sizeof(nv_item_type));

  if(nv_data_ptr == NULL)
  {
    MMGSDIUTIL_TMC_MEM_FREE(mmgsdi_nv_cmd_ptr);
    return NV_NOTALLOC_S;
  }

  /* Set the Data Ptr for NV Item to read */
   mmgsdi_nv_cmd_ptr->data_ptr = nv_data_ptr;

  /* Clear REX Signals */
  (void) rex_clr_sigs( rex_self(), MMGSDI_NV_READ_SIG );

  /* Send Command to NV */
  nv_cmd_ext(&mmgsdi_nv_cmd_ext);

  /* Clear Signal */
  (void) rex_clr_sigs( rex_self(), MMGSDI_NV_READ_SIG );

  (void)mmgsdi_nv_put_nv_item_in_cache(
          item,
          nv_data_ptr,
          mmgsdi_nv_cmd_ptr->status,
          nv_context);

  nv_status = mmgsdi_nv_cmd_ptr->status;
  MMGSDIUTIL_TMC_MEM_FREE(mmgsdi_nv_cmd_ptr);
  MMGSDIUTIL_TMC_MEM_FREE(nv_data_ptr);

  /* Need to copy the recently cached item into out ptr. If the item was
     successfully written to the cache, the item_status should contain the
     correct status  */
  if(MMGSDI_CACHE_INIT ==
      mmgsdi_nv_get_nv_item_from_cache(item, mmgsdi_nv_data_ptr, nv_context))
  {
    return mmgsdi_nv_data_ptr->item_status;
  }

  return NV_FAIL_S;
} /* mmgsdi_nv_get_item */


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
nv_stat_enum_type mmgsdi_nv_put_item(
  nv_items_enum_type      item,
  const nv_item_type     *data_ptr,
  mmgsdi_nv_context_type  nv_context
)
{
  nv_cmd_type             *mmgsdi_nv_cmd_ptr  = NULL;
  nv_cmd_ext_type          mmgsdi_nv_cmd_ext  = {{0}};
  mmgsdi_return_enum_type  mmgsdi_status      = MMGSDI_ERROR;
  nv_item_type            *cmd_data_ptr       = NULL;
  uint32                   mmgsdi_nv_cmd_size = 0;

  UIM_MSG_HIGH_1("In mmgsdi_nv_put_item, item: %d", item);

  if(data_ptr == NULL)
  {
    return NV_BADPARM_S;
  }

  /* Device specific items are accessed from primary context,
     if the context is DEVICE SPECIFIC, convert it into PRIMARY CONTEXT */
  if(nv_context == MMGSDI_NV_CONTEXT_DEVICE_SPECIFIC)
  {
    nv_context = MMGSDI_NV_CONTEXT_PRIMARY;
  }

  /* Allocate memory for the NV command */
  mmgsdi_util_cal_align_size(sizeof(nv_cmd_type), &mmgsdi_nv_cmd_size);

  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(mmgsdi_nv_cmd_ptr,
                                     (mmgsdi_nv_cmd_size + sizeof(nv_item_type)));

  if(mmgsdi_nv_cmd_ptr == NULL)
  {
    return NV_NOTALLOC_S;
  }

  /* Copy data to write */
  cmd_data_ptr = (nv_item_type*)((uint8*)mmgsdi_nv_cmd_ptr + mmgsdi_nv_cmd_size);
  mmgsdi_memscpy(cmd_data_ptr,
                 sizeof(nv_item_type),
                 data_ptr,
                 sizeof(nv_item_type));

  /* Notify main MMGSDI task when done */
  mmgsdi_nv_cmd_ptr->tcb_ptr    = UIM_GSDI_TCB;
  mmgsdi_nv_cmd_ptr->sigs       = MMGSDI_NV_WRITE_SIG;
  mmgsdi_nv_cmd_ptr->done_q_ptr = &mmgsdi_nv_q;

  /* Item to write */
  mmgsdi_nv_cmd_ptr->item     = item;
  mmgsdi_nv_cmd_ptr->cmd      = NV_WRITE_F;
  mmgsdi_nv_cmd_ptr->data_ptr = cmd_data_ptr;

  /* Set mmgsdi_nv_cmd_ext */
  mmgsdi_nv_cmd_ext.nvcmd     = mmgsdi_nv_cmd_ptr;
  mmgsdi_nv_cmd_ext.context   = mmgsdi_nv_convert_mmgsdi_nv_context(nv_context);

  /* Send Command to NV */
  nv_cmd_ext(&mmgsdi_nv_cmd_ext);

  mmgsdi_status = mmgsdi_nv_put_nv_item_in_cache(
                    item,
                    data_ptr,
                    NV_DONE_S,
                    nv_context);

  UIM_MSG_HIGH_1("MMGSDI NV Cache write status=0x%x", mmgsdi_status);

  return NV_DONE_S;
}/* mmgsdi_nv_put_item */


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
)
{
  uint32 i    = 0x0;
  uint32 mask = 0x1;

  if (nv_context >= MMGSDI_NV_CONTEXTS_MAX ||
      mmgsdi_nv_item_cache_table_ptr[nv_context] == NULL)
  {
    UIM_MSG_ERR_1("nv_context - 0x%x is invalid or mmgsdi_nv_item_cache_table_ptr is NULL",
                  nv_context);
    return MMGSDI_CACHE_NOT_FOUND;
  }

  for (i=0; i<MMGSDI_NV_ITEM_CACHE_SIZE; i++)
  {
    if (mmgsdi_nv_item_cache_table_ptr[nv_context]->item[i].item_enum == item_enum)
    {
      /* Create bit mask using the index of the item to determine if cached */
      if (mmgsdi_nv_item_cache_table_ptr[nv_context]->is_cached_mask &
          (mask << i))
      {
        *mmgsdi_nv_data_ptr =
           mmgsdi_nv_item_cache_table_ptr[nv_context]->item[i];

        UIM_MSG_HIGH_1("Got NV item: %d in MMGSDI NV Cache", item_enum);
        return MMGSDI_CACHE_INIT;
      }
      else
      {
        return MMGSDI_CACHE_NOT_INIT;
      }
    }
  }

  UIM_MSG_ERR_1("Getting item: %d will cause deadlocks. Add to NV cache",
                item_enum);

  return MMGSDI_CACHE_NOT_FOUND;
} /* mmgsdi_nv_get_nv_item_from_cache */


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
)
{
  uint32 i    = 0x0;
  uint32 mask = 0x1;

  if (nv_context >= MMGSDI_NV_CONTEXTS_MAX ||
      mmgsdi_nv_item_cache_table_ptr[nv_context] == NULL)
  {
    UIM_MSG_ERR_1("nv_context - 0x%x is invalid or mmgsdi_nv_item_cache_table_ptr is NULL",
                  nv_context);
    return MMGSDI_ERROR;
  }

  for (i=0; i<MMGSDI_NV_ITEM_CACHE_SIZE; i++)
  {
    if (mmgsdi_nv_item_cache_table_ptr[nv_context]->item[i].item_enum == item_enum)
    {
      mmgsdi_nv_item_cache_table_ptr[nv_context]->is_cached_mask |= (mask << i);
      mmgsdi_nv_item_cache_table_ptr[nv_context]->item[i].item_status =
        item_status;
      if (item_status == NV_DONE_S)
      {
        switch(item_enum)
        {
          case NV_UIM_CDMA_PREF_SLOT_I:
            mmgsdi_nv_item_cache_table_ptr[nv_context]->item[i].item_value.uim_cdma_pref_slot =
              data_ptr->uim_cdma_pref_slot;
            break;

          case NV_UIM_GSM_PREF_SLOT_I:
            mmgsdi_nv_item_cache_table_ptr[nv_context]->item[i].item_value.uim_gsm_pref_slot =
              data_ptr->uim_gsm_pref_slot;
            break;

          case NV_RTRE_CONFIG_I:
            mmgsdi_nv_item_cache_table_ptr[nv_context]->item[i].item_value.rtre_config =
              data_ptr->rtre_config;
            break;

          case NV_MMGSDI_ME_CONFIG_PARAM_I:
            mmgsdi_nv_item_cache_table_ptr[nv_context]->item[i].item_value.mmgsdi_me_config_param =
              data_ptr->mmgsdi_me_config_param;
            break;

          case NV_UIM_SELECT_DEFAULT_USIM_APP_I:
            mmgsdi_nv_item_cache_table_ptr[nv_context]->item[i].item_value.uim_select_default_usim_app =
              data_ptr->uim_select_default_usim_app;
            break;

          case NV_MEID_ME_I:
            mmgsdi_memscpy(&mmgsdi_nv_item_cache_table_ptr[nv_context]->item[i].item_value.meid,
                           sizeof(mmgsdi_nv_item_cache_table_ptr[nv_context]->item[i].item_value.meid),
                           &data_ptr->meid,
                           sizeof(data_ptr->meid));
            break;

          case NV_UE_IMEI_I:
            mmgsdi_nv_item_cache_table_ptr[nv_context]->item[i].item_value.ue_imei =
              data_ptr->ue_imei;
            break;

          case NV_HOMEZONE_ENABLED_I:
            mmgsdi_nv_item_cache_table_ptr[nv_context]->item[i].item_value.homezone_enabled =
              data_ptr->homezone_enabled;
            break;

          case NV_ESN_ME_I:
            mmgsdi_nv_item_cache_table_ptr[nv_context]->item[i].item_value.esn =
              data_ptr->esn;
            break;

          case NV_SCM_I:
            mmgsdi_nv_item_cache_table_ptr[nv_context]->item[i].item_value.scm =
              data_ptr->scm;
            break;

          case NV_MOB_CAI_REV_I:
            mmgsdi_nv_item_cache_table_ptr[nv_context]->item[i].item_value.mob_cai_rev =
              data_ptr->mob_cai_rev;
            break;

          case NV_SERVICE_DOMAIN_PREF_I:
            mmgsdi_nv_item_cache_table_ptr[nv_context]->item[i].item_value.service_domain_pref =
              data_ptr->service_domain_pref;
            break;

          case NV_DISABLE_CM_CALL_TYPE_I:
            mmgsdi_nv_item_cache_table_ptr[nv_context]->item[i].item_value.disable_cm_call_type =
              data_ptr->disable_cm_call_type;
            break;

          default:
            break;
        }
      }
      UIM_MSG_HIGH_1("Put NV item: %d in MMGSDI NV Cache", item_enum);
      return MMGSDI_SUCCESS;
    }
  }

  UIM_MSG_ERR_1("Using item: %d will cause deadlocks. Add to NV cache",
                item_enum);
  return MMGSDI_ERROR;
} /* mmgsdi_nv_put_nv_item_in_cache */


/*===========================================================================
  FUNCTION MMGSDI_NV_DIAG_ACCESS_CHECK_CB

  DESCRIPTION
    This function is called by the Diag module as a result of any Diag access
    of the MMGSDI access controlled EFS directory. It is initially registered
    with Diag for knowing the access of the EFS directory.

    Note: In order to enable the NVs in the restricted NV files list, the
    corresponding MCFG HW MBN entry needs to be updated.

  PARAMETERS
    directory_name_ptr: Name of the file that needs to be permitted
    op_type      : File operation type

  RETURN VALUE
    TRUE:  If we want to allow access
    FALSE: If we want to reject access

  DEPENDENCIES
    QMI UIM service must be registered with Diag module via
    diag_fs_register_access_check API.

  SIDE EFFECTS
    None

===========================================================================*/
static boolean mmgsdi_nv_diag_access_check_cb(
  char      * directory_name_ptr,
  uint32      op_type
)
{
  (void)directory_name_ptr;

  if ( (op_type == DIAG_FS_CHECK_OPEN) ||
       (op_type == DIAG_FS_CHECK_DISP_DIRS) ||
       (op_type == DIAG_FS_CHECK_DISP_FILES) ||
       (op_type == DIAG_FS_CHECK_ITERATE) ||
       (op_type == DIAG_FS_CHECK_READ) ||
       (op_type == DIAG_FS_CHECK_GET_ATTRIB) ||
       (op_type == DIAG_FS_CHECK_CLOSE) ||
       (op_type == DIAG_FS_CHECK_OPENDIR) ||
       (op_type == DIAG_FS_CHECK_READDIR) ||
       (op_type == DIAG_FS_CHECK_CLOSEDIR) ||
       (op_type == DIAG_FS_CHECK_STAT) ||
       (op_type == DIAG_FS_CHECK_LSTAT) ||
       (op_type == DIAG_FS_CHECK_FSTAT) ||
       (op_type == DIAG_FS_CHECK_STATFS) ||
       (op_type == DIAG_FS_CHECK_ACCESS))
  {
    /* Allow access */
    return TRUE;
  }

  return FALSE;
} /* mmgsdi_nv_diag_access_check_cb */


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
mmgsdi_return_enum_type mmgsdi_nv_item_cache_init (
  void
)
{
  uint8 nv_context = 0;
  static const char      *efs_directory_list[] = {UIM_COMMON_EFS_PATH_MODE_UIM_MMGSDI_SECURE_DIR};

  for(nv_context = 0; nv_context < MMGSDI_NV_CONTEXTS_MAX; nv_context++)
  {
    MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(mmgsdi_nv_item_cache_table_ptr[nv_context],
      sizeof(mmgsdi_nv_item_cache_table_type));
    if (mmgsdi_nv_item_cache_table_ptr[nv_context] == NULL)
    {
      return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
    }
    mmgsdi_nv_item_cache_table_ptr[nv_context]->is_cached_mask      = 0x0;
    mmgsdi_nv_item_cache_table_ptr[nv_context]->item[0x0].item_enum = NV_UIM_CDMA_PREF_SLOT_I;
    mmgsdi_nv_item_cache_table_ptr[nv_context]->item[0x1].item_enum = NV_UIM_GSM_PREF_SLOT_I;
    mmgsdi_nv_item_cache_table_ptr[nv_context]->item[0x2].item_enum = NV_RTRE_CONFIG_I;
    mmgsdi_nv_item_cache_table_ptr[nv_context]->item[0x3].item_enum =
      NV_MMGSDI_ME_CONFIG_PARAM_I;
    mmgsdi_nv_item_cache_table_ptr[nv_context]->item[0x4].item_enum =
      NV_UIM_SELECT_DEFAULT_USIM_APP_I;
    mmgsdi_nv_item_cache_table_ptr[nv_context]->item[0x5].item_enum = NV_MEID_ME_I;
    mmgsdi_nv_item_cache_table_ptr[nv_context]->item[0x6].item_enum = NV_UE_IMEI_I;
    mmgsdi_nv_item_cache_table_ptr[nv_context]->item[0x7].item_enum = NV_HOMEZONE_ENABLED_I;
    mmgsdi_nv_item_cache_table_ptr[nv_context]->item[0x8].item_enum = NV_ESN_ME_I;
    mmgsdi_nv_item_cache_table_ptr[nv_context]->item[0x9].item_enum = NV_SCM_I;
    mmgsdi_nv_item_cache_table_ptr[nv_context]->item[0xA].item_enum = NV_MOB_CAI_REV_I;
    mmgsdi_nv_item_cache_table_ptr[nv_context]->item[0xB].item_enum = NV_SERVICE_DOMAIN_PREF_I;
    mmgsdi_nv_item_cache_table_ptr[nv_context]->item[0xC].item_enum = NV_DISABLE_CM_CALL_TYPE_I;
  }

  /* Register call back for mmgsdi secure efs directory */
  diag_fs_register_access_check((const char **)efs_directory_list,
                                 sizeof(efs_directory_list) / sizeof(efs_directory_list[0]),
                                 mmgsdi_nv_diag_access_check_cb);

  return MMGSDI_SUCCESS;
} /* mmgsdi_nv_item_cache_init */


/*===========================================================================
FUNCTION MMGSDI_NV_SLOT_FEATURE_WRITE_DEFAULT_VALUE

DESCRIPTION
  Function used to write the default value of MMGSDI Slot Feature to global
  mmgsdi_slot_features_status_list.

DEPENDENCIES

RETURN VALUE
  void

SIDE EFFECTS
  None
===========================================================================*/
static void mmgsdi_nv_slot_feature_write_default_value(
  mmgsdi_nv_slot_specific_features_enum_type nv_feature,
  uint8                                      index)
{
  if(index > (MMGSDI_MAX_NUM_SLOTS - 1) ||
     nv_feature >= (sizeof(mmgsdi_slot_features_list_init_table) / sizeof(mmgsdi_slot_features_list_type)))
  {
    return;
  }

  mmgsdi_slot_features_status_list[index].item[nv_feature] = mmgsdi_slot_features_list_init_table[nv_feature].default_value;
} /* mmgsdi_nv_slot_feature_write_default_value */


/*===========================================================================
FUNCTION MMGSDI_NV_DEVICE_FEATURE_WRITE_DEFAULT_VALUE

DESCRIPTION
  Function used to write the default value of MMGSDI Device Feature to global
  mmgsdi_device_features_status_list.

DEPENDENCIES

RETURN VALUE
  void

SIDE EFFECTS
  None
===========================================================================*/
static void mmgsdi_nv_device_feature_write_default_value(
  mmgsdi_nv_device_specific_features_enum_type nv_feature)
{
  if(nv_feature >= (sizeof(mmgsdi_device_features_list_init_table) / sizeof(mmgsdi_device_features_list_type)))
  {
    return;
  }

  mmgsdi_device_features_status_list.item[nv_feature] = mmgsdi_device_features_list_init_table[nv_feature].default_value;
} /* mmgsdi_nv_device_feature_write_default_value */


/*===========================================================================
FUNCTION MMGSDI_NV_GET_NUM_OF_SLOT_FEATURES_IN_VER

DESCRIPTION
  Function used to get no of features until a particular version in mmgsdi
  slot features list

DEPENDENCIES

RETURN VALUE
  num_features_in_ver

SIDE EFFECTS
  None
===========================================================================*/
static uint8 mmgsdi_nv_get_num_of_slot_features_in_ver(
  uint8 version_to_count
)
{
  uint8 i                   = 0;
  uint8 no_of_entries       = sizeof(mmgsdi_slot_features_list_init_table) / sizeof(mmgsdi_slot_features_list_type);
  uint8 num_features_in_ver = 0;

  for (i = 0; i < no_of_entries; i++)
  {
    if(version_to_count >= mmgsdi_slot_features_list_init_table[i].version_introduced)
    {
      num_features_in_ver++;
    }
    else
    {
      break;
    }
  }

  return num_features_in_ver;
}/* mmgsdi_nv_get_num_of_slot_features_in_ver */


/*===========================================================================
FUNCTION MMGSDI_NV_GET_NUM_OF_DEVICE_FEATURES_IN_VER

DESCRIPTION
  Function used to get no of features until a particular version in mmgsdi
  slot features list

DEPENDENCIES

RETURN VALUE
  num_features_in_ver

SIDE EFFECTS
  None
===========================================================================*/
static uint8 mmgsdi_nv_get_num_of_device_features_in_ver(
  uint8 version_to_count
)
{
  uint8 i                   = 0;
  uint8 no_of_entries       = sizeof(mmgsdi_device_features_list_init_table) / sizeof(mmgsdi_device_features_list_type);
  uint8 num_features_in_ver = 0;

  for (i = 0; i < no_of_entries; i++)
  {
    if(version_to_count >= mmgsdi_device_features_list_init_table[i].version_introduced)
    {
      num_features_in_ver++;
    }
    else
    {
      break;
    }
  }

  return num_features_in_ver;
}/* mmgsdi_nv_get_num_of_device_features_in_ver */


/*===========================================================================
FUNCTION MMGSDI_NV_WRITE_DEFAULT_VALUE_OF_DELTA_SLOT_FEATURES

DESCRIPTION
  Function used to write the default values of MMGSDI Slot Features that are
  delta between the two versions of mmgsdi_slot_features_status_list.

DEPENDENCIES

RETURN VALUE
  void

SIDE EFFECTS
  None
===========================================================================*/
static void mmgsdi_nv_write_default_value_of_delta_slot_features(
  uint8 version_in_nv,
  uint8 index
)
{
  uint8 mmgsdi_feature        = 0;
  uint8 num_features_in_nv    = 0;
  uint8 num_features_in_build = 0;
  uint8 no_of_entries         = 0;
  uint8 curr_version_in_build = 0;

  no_of_entries = sizeof(mmgsdi_slot_features_list_init_table) / sizeof(mmgsdi_slot_features_list_type);

  curr_version_in_build = mmgsdi_slot_features_list_init_table[no_of_entries - 1].version_introduced;

  if(version_in_nv >= curr_version_in_build)
  {
    UIM_MSG_LOW_0("Array out-of-bounds while writing default values for MMGSDI features");
    return;
  }

  num_features_in_nv    = mmgsdi_nv_get_num_of_slot_features_in_ver(version_in_nv);
  num_features_in_build = mmgsdi_nv_get_num_of_slot_features_in_ver(curr_version_in_build);

  /* Since mmgsdi_features_total_till_version array is indexed by the
     feature_status_list *version*, it's value at version version_in_nv is
     essentially the first feature of the delta list...no need to bail out in
     case writing fails... */
  for(mmgsdi_feature = num_features_in_nv; mmgsdi_feature < num_features_in_build; mmgsdi_feature++)
  {
    mmgsdi_nv_slot_feature_write_default_value((mmgsdi_nv_slot_specific_features_enum_type)mmgsdi_feature,
                                               index);
  }
} /* mmgsdi_nv_write_default_value_of_delta_slot_features */


/*===========================================================================
FUNCTION MMGSDI_NV_WRITE_DEFAULT_VALUE_OF_DELTA_DEVICE_FEATURES

DESCRIPTION
  Function used to write the default values of MMGSDI Slot Features that are
  delta between the two versions of mmgsdi_device_features_status_list.

DEPENDENCIES

RETURN VALUE
  void

SIDE EFFECTS
  None
===========================================================================*/
static void mmgsdi_nv_write_default_value_of_delta_device_features(
  uint8 version_in_nv
)
{
  uint8 mmgsdi_feature        = 0;
  uint8 num_features_in_nv    = 0;
  uint8 num_features_in_build = 0;
  uint8 no_of_entries         = 0;
  uint8 curr_version_in_build = 0;

  no_of_entries = sizeof(mmgsdi_device_features_list_init_table) / sizeof(mmgsdi_device_features_list_type);

  curr_version_in_build = mmgsdi_device_features_list_init_table[no_of_entries - 1].version_introduced;

  if(version_in_nv >= curr_version_in_build)
  {
    UIM_MSG_LOW_0("Array out-of-bounds while writing default values for MMGSDI features");
    return;
  }

  num_features_in_nv    = mmgsdi_nv_get_num_of_device_features_in_ver(version_in_nv);
  num_features_in_build = mmgsdi_nv_get_num_of_device_features_in_ver(curr_version_in_build);

  /* Since mmgsdi_features_total_till_version array is indexed by the
     feature_status_list *version*, it's value at version version_in_nv is
     essentially the first feature of the delta list...no need to bail out in
     case writing fails... */
  for(mmgsdi_feature = num_features_in_nv; mmgsdi_feature < num_features_in_build; mmgsdi_feature++)
  {
    mmgsdi_nv_device_feature_write_default_value((mmgsdi_nv_device_specific_features_enum_type )mmgsdi_feature);
  }
} /* mmgsdi_nv_write_default_value_of_delta_device_features */


/*===========================================================================
FUNCTION MMGSDI_NV_WRITE_DEFAULT_VALUE_OF_ALL_DEVICE_FEATURES

DESCRIPTION
  Function used to write the default values of all MMGSDI Device Features into
  mmgsdi_device_features_status_list.

DEPENDENCIES

RETURN VALUE
  void

SIDE EFFECTS
  None
===========================================================================*/
static void mmgsdi_nv_write_default_value_of_all_device_features(
  void)
{
  uint8 mmgsdi_feature        = 0;
  uint8 num_features_in_build = 0;

  num_features_in_build = sizeof(mmgsdi_device_features_list_init_table) / sizeof(mmgsdi_device_features_list_type);

  memset(&mmgsdi_device_features_status_list, 0x00, sizeof(mmgsdi_device_features_status_list));

  /* Update the Version in features_status_list to the current build version */
  mmgsdi_device_features_status_list.version = mmgsdi_device_features_list_init_table[num_features_in_build - 1].version_introduced;

  /* Update features_status_list with default values */
  for(mmgsdi_feature = 0; mmgsdi_feature < num_features_in_build; mmgsdi_feature++)
  {
    mmgsdi_nv_device_feature_write_default_value((mmgsdi_nv_device_specific_features_enum_type )mmgsdi_feature);
  }
} /* mmgsdi_nv_write_default_value_of_all_device_features */


/*===========================================================================
FUNCTION MMGSDI_NV_WRITE_DEFAULT_VALUE_OF_ALL_SLOT_FEATURES

DESCRIPTION
  Function used to write the default values of all MMGSDI Slot Features into
  mmgsdi_slot_features_status_list.

DEPENDENCIES

RETURN VALUE
  void

SIDE EFFECTS
  None
===========================================================================*/
static void mmgsdi_nv_write_default_value_of_all_slot_features(
  uint8   index)
{
  uint8 mmgsdi_feature        = 0;
  uint8 num_features_in_build = 0;

  if (index >= MMGSDI_MAX_NUM_SLOTS)
  {
    return;
  }

  num_features_in_build = sizeof(mmgsdi_slot_features_list_init_table) / sizeof(mmgsdi_slot_features_list_type);

  memset(&mmgsdi_slot_features_status_list[index], 0x00, sizeof(mmgsdi_slot_features_status_list[index]));

  /* Update the Version in features_status_list to the current build version */
  mmgsdi_slot_features_status_list[index].version = mmgsdi_slot_features_list_init_table[num_features_in_build - 1].version_introduced;

  /* Update features_status_list with default values */
  for(mmgsdi_feature = 0; mmgsdi_feature < num_features_in_build; mmgsdi_feature++)
  {
    mmgsdi_nv_slot_feature_write_default_value((mmgsdi_nv_slot_specific_features_enum_type )mmgsdi_feature,
                                               index);
  }
} /* mmgsdi_nv_write_default_value_of_all_slot_features */


/*===========================================================================
FUNCTION MMGSDI_NV_DEVICE_FEATURES_STATUS_LIST_UPDATE_VERSION

DESCRIPTION
  Function used to validate the MMGSDI device Features Status NVITEM's version.
  If what's in the NV is less than the one currently defined in the build,
  updates it to the current version defined and writes it back to the NV.
  It also writes back to the NV the default values of the delta items in
  in the list.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Version gets updated in the NV if it doesn't match the one in the build. If
  the version is updated, next time when the user reads the NVITEM, a
  dropdown of items corresponding to the updated version is shown instead of
  what the user saw the first time the NV read was done (corresponding to
  the version present in the NV at that point).
===========================================================================*/
static void mmgsdi_nv_device_features_status_list_update_version(
  void)
{
  uint8                         num_features_in_build = 0;

  num_features_in_build = sizeof(mmgsdi_device_features_list_init_table) / sizeof(mmgsdi_device_features_list_type);

  UIM_MSG_LOW_2("MMGSDI Device Features Status NV ITEM version - in NV: 0x%x, in build: 0x%x",
                 mmgsdi_device_features_status_list.version,
                 mmgsdi_device_features_list_init_table[num_features_in_build - 1].version_introduced);

  if (mmgsdi_device_features_status_list.version < mmgsdi_device_features_list_init_table[num_features_in_build - 1].version_introduced)
  {
    /* If the features_status_list version in NV is less than current
       features_status_list version in our code, meaning new features have been
       added since the version in NV till the current version, we write back
       the default value of all those delta features to the NV. */
    mmgsdi_nv_write_default_value_of_delta_device_features(
      mmgsdi_device_features_status_list.version);

    mmgsdi_device_features_status_list.version = mmgsdi_device_features_list_init_table[num_features_in_build - 1].version_introduced;

    if (uim_common_efs_write(UIM_COMMON_EFS_MMGSDI_DEVICE_SPECIFIC_FEATURE_STATUS_LIST,
                             UIM_COMMON_EFS_ITEM_FILE_TYPE,
                             UIM_COMMON_EFS_DEVICE,
                             (const char*)&mmgsdi_device_features_status_list,
                             sizeof(mmgsdi_device_features_status_list)) != UIM_COMMON_EFS_SUCCESS)
    {
      UIM_MSG_ERR_0("MMGSDI Device Features Status NV ITEM version update failed!");
    }
  }
} /* mmgsdi_nv_device_features_status_list_update_version */


/*===========================================================================
FUNCTION MMGSDI_NV_SLOT_FEATURES_STATUS_LIST_UPDATE_VERSION

DESCRIPTION
  Function used to validate the MMGSDI slot Features Status NVITEM's version.
  If what's in the NV is less than the one currently defined in the build,
  updates it to the current version defined and writes it back to the NV.
  It also writes back to the NV the default values of the delta items in
  in the list.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Version gets updated in the NV if it doesn't match the one in the build. If
  the version is updated, next time when the user reads the NVITEM, a
  dropdown of items corresponding to the updated version is shown instead of
  what the user saw the first time the NV read was done (corresponding to
  the version present in the NV at that point).
===========================================================================*/
static void mmgsdi_nv_slot_features_status_list_update_version(
  uint8                        index)
{
  uim_common_efs_context_type   efs_context           = UIM_COMMON_EFS_CONTEXT_MAX;
  uint8                         num_features_in_build = 0;

  efs_context = mmgsdi_util_get_common_efs_item_context(index);
  if (efs_context == UIM_COMMON_EFS_CONTEXT_MAX)
  {
    return;
  }

  num_features_in_build = sizeof(mmgsdi_slot_features_list_init_table) / sizeof(mmgsdi_slot_features_list_type);

  UIM_MSG_LOW_2("MMGSDI Slot Features Status NV ITEM version - in NV: 0x%x, in build: 0x%x",
                 mmgsdi_slot_features_status_list[index].version,
                 mmgsdi_slot_features_list_init_table[num_features_in_build - 1].version_introduced);

  if (mmgsdi_slot_features_status_list[index].version < mmgsdi_slot_features_list_init_table[num_features_in_build - 1].version_introduced)
  {
    /* If the features_status_list version in NV is less than current
       features_status_list version in our code, meaning new features have been
       added since the version in NV till the current version, we write back
       the default value of all those delta features to the NV. */
    mmgsdi_nv_write_default_value_of_delta_slot_features(
      mmgsdi_slot_features_status_list[index].version,
      index);

    mmgsdi_slot_features_status_list[index].version = mmgsdi_slot_features_list_init_table[num_features_in_build - 1].version_introduced;

    if (uim_common_efs_write(UIM_COMMON_EFS_MMGSDI_SLOT_SPECIFIC_FEATURE_STATUS_LIST,
                             UIM_COMMON_EFS_ITEM_FILE_TYPE,
                             efs_context,
                             (const char*)&mmgsdi_slot_features_status_list[index],
                             sizeof(mmgsdi_slot_features_status_list[index])) != UIM_COMMON_EFS_SUCCESS)
    {
      UIM_MSG_ERR_0("MMGSDI Slot Features Status NV ITEM version update failed!");
    }
  }
} /* mmgsdi_nv_slot_features_status_list_update_version */


/*===========================================================================
FUNCTION MMGSDI_NV_INIT_FEATURES_STATUS_NV_LOOKUP

DESCRIPTION
  Function used to get the status of MMGSDI features as defined by the NVITEM
  "MMGSDI_DEVICE_SPECIFIC_FEATURES_STATUS_LIST - 73763" and
  "MMGSDI_SLOT_SPECIFIC_FEATURES_STATUS_LIST - 73764"

  The global, "mmgsdi_device_features_status_list" and
  "mmgsdi_slot_features_status_list", are used to hold these EFS ITEM,
  These EFS items are expandable, yet the size is fixed. Please refer to the
  definition of these global structures.

  MMGSDI Features nv item is now split into two different efs items, namely,
  mmgsdi_device_features_status_list and  mmgsdi_slot_features_status_list.
  this is done to give the flexibility for MCFG and operators to configure
  features nv items per slot if the feature is applicable per operator which
  could be different operator cards on each slot.

  MMGSDI device specific features apply to all the slots and the settings are
  read from the conext 0 of the efs, whereas slot specific features apply per
  slot and are read from different contexts like 0, 1 etc and are applied
  accordingly to each slot.

  The legacy mmgsdi_nv_features_enum_type is now split into two enum tables
  mmgsdi_nv_device_specific_features_enum_type and
  mmgsdi_nv_slot_specific_features_enum_type.

  During initialization we look for all the three EFS files, the legacy
  features_status_list, new device_specific_features_status_list and
  also new slot_specific_features_list, if any of the new EFS files are present,
  the old legacy features list is ignored, if only legacy features list is present,
  the new device specific and slot specific features status list are generated
  based on the legacy features status list and this legacy EFS file is deleted.

  The mmgsdi code has full control over version of NVITEM. Taking an example,
  if in future, in version 2 of this struct(that has 10 items), the delta is 3
  items (from version 0 that had 7 items), and this build is loaded onto the
  device that already had the items set using version 0 of this NVITEM, then,
  on boot-up after loading the build, mmgsdi will validate the version it read
  from NV. If it is not 2, it will update it to 2 and write it back to NV.

DEPENDENCIES
  Latest QXDM version required to be able to set this NVITEM.

RETURN VALUE
  None

SIDE EFFECTS
  "version" item in this NVITEM is writable through QXDM (no QXDM support to
  hide it or grey it out) or QCN. However if what is written explicitly doesn't
  match what's in the build, mmgsdi will change it to what's in the build and
  it could be annoying for the QXDM user to see it change to something else
  (when the user reads the NVITEM next time).
===========================================================================*/
void mmgsdi_nv_init_features_status_nv_lookup(
  uint8     index)
{
  uim_common_efs_context_type      efs_context                            = UIM_COMMON_EFS_CONTEXT_MAX;
  boolean                          legacy_features_status_efs_present     = FALSE;
  boolean                          new_device_features_status_efs_present = FALSE;
  boolean                          new_slot_features_status_efs_present   = FALSE;
  mmgsdi_features_status_list_type features_status_list;

  efs_context = mmgsdi_util_get_common_efs_item_context(index);
  if (efs_context == UIM_COMMON_EFS_CONTEXT_MAX)
  {
    return;
  }

  /* Start off with the global struct initialised to 0 */
  memset(&features_status_list,
         0x00,
         sizeof(features_status_list));
  if(index == 0)
  {
    memset(&mmgsdi_device_features_status_list,
           0x00,
           sizeof(mmgsdi_device_features_status_list));
  }

  memset(&mmgsdi_slot_features_status_list[index],
         0x00,
         sizeof(mmgsdi_slot_features_status_list[index]));

  /* Read Device Specific Features Status list
     Since the efs is device specific only context 0 content applies */
  if (efs_context == UIM_COMMON_EFS_CONTEXT_0)
  {
    if (uim_common_efs_read(UIM_COMMON_EFS_MMGSDI_DEVICE_SPECIFIC_FEATURE_STATUS_LIST,
                            UIM_COMMON_EFS_ITEM_FILE_TYPE,
                            efs_context,
                            (uint8 *)&mmgsdi_device_features_status_list,
                            sizeof(mmgsdi_device_features_status_list)) ==
        UIM_COMMON_EFS_SUCCESS)
    {
      new_device_features_status_efs_present = TRUE;
    }
  }

  /* Read Slot Specific Features Status list for each context based on the index */
  if (uim_common_efs_read(UIM_COMMON_EFS_MMGSDI_SLOT_SPECIFIC_FEATURE_STATUS_LIST,
                          UIM_COMMON_EFS_ITEM_FILE_TYPE,
                          efs_context,
                          (uint8 *)&mmgsdi_slot_features_status_list[index],
                          sizeof(mmgsdi_slot_features_status_list[index])) ==
      UIM_COMMON_EFS_SUCCESS)
  {
    new_slot_features_status_efs_present = TRUE;
  }

  /* Read Legacy features status list
     If the newer features status list (slot and device) is not present -
     use this to build the newer lists and then delete legacy EFS
     if one of the slot or device list is present, just delete this efs file */
  /* See function description for more details */
  if (uim_common_efs_read(UIM_COMMON_EFS_MMGSDI_FEATURE_STATUS_LIST,
                          UIM_COMMON_EFS_ITEM_FILE_TYPE,
                          efs_context,
                          (uint8 *)&features_status_list,
                          sizeof(features_status_list)) ==
      UIM_COMMON_EFS_SUCCESS)
  {
    legacy_features_status_efs_present = TRUE;
  }

  /* if the legacy 67211 EFS file is only present - for backward compatability copy
     those settings into new EFS file and global */
  /* if one of the new features status EFS file is present, ignore 67211 setting and go ahead with new settings */
  if ((legacy_features_status_efs_present &&
      features_status_list.version <= MMGSDI_MAX_VER_LEGACY_FEATURES_LIST_SUPPORTED)&&
      (!new_slot_features_status_efs_present)  &&
      (!new_device_features_status_efs_present))
  {
    uint8 no_of_entries         = 0;
    uint8 i                     = 0;
    uint8 curr_version_in_build = 0;

    /* Extract the features status settings from legacy EFS file into slot
       specific features status list */
    if (!new_slot_features_status_efs_present)
    {
      mmgsdi_slot_features_status_list[index].version = features_status_list.version;
      no_of_entries       = sizeof(mmgsdi_legacy_slot_features_mapping_list_table) / sizeof(mmgsdi_legacy_slot_features_mapping_list_type);

      for (i = 0; i < no_of_entries; i++)
      {
        mmgsdi_slot_features_status_list[index].item[i] =
          features_status_list.item[mmgsdi_legacy_slot_features_mapping_list_table[i].legacy_feature];
      }

      /* if the legacy EFS version is greater than the max version set it to max version */
      no_of_entries = sizeof(mmgsdi_slot_features_list_init_table)/sizeof(mmgsdi_slot_features_list_type);
      curr_version_in_build = mmgsdi_slot_features_list_init_table[no_of_entries - 1].version_introduced;
      if(mmgsdi_slot_features_status_list[index].version > curr_version_in_build)
      {
        mmgsdi_slot_features_status_list[index].version = curr_version_in_build;
      }

      /* if the version is same as the build version update directly to EFS
         else, write the rest of the features and update */
      if(mmgsdi_slot_features_status_list[index].version == curr_version_in_build)
      {
        if (uim_common_efs_write(UIM_COMMON_EFS_MMGSDI_SLOT_SPECIFIC_FEATURE_STATUS_LIST,
                                 UIM_COMMON_EFS_ITEM_FILE_TYPE,
                                 efs_context,
                                 (const char*)&mmgsdi_slot_features_status_list[index],
                                 sizeof(mmgsdi_slot_features_status_list[index])) != UIM_COMMON_EFS_SUCCESS)
        {
          UIM_MSG_ERR_0("MMGSDI Slot Features Status NV ITEM version update failed!");
        }
      }
      else
      {
        /* update new EFS files */
        mmgsdi_nv_slot_features_status_list_update_version(index);
      }
    }

    /* Extract the features status settings from legacy EFS file into device
       specific features status list  - Device specific features status list
       only applies for context 0 */
    if(efs_context == UIM_COMMON_EFS_CONTEXT_0 &&
       !new_device_features_status_efs_present)
    {
      mmgsdi_device_features_status_list.version = features_status_list.version;
      no_of_entries       = sizeof(mmgsdi_legacy_device_features_mapping_list_table) / sizeof(mmgsdi_legacy_device_features_mapping_list_type);

      for (i = 0; i < no_of_entries; i++)
      {
        mmgsdi_device_features_status_list.item[i] =
          features_status_list.item[mmgsdi_legacy_device_features_mapping_list_table[i].legacy_feature];
      }

      /* if the legacy EFS version is greater than the max version set it to max version */
      no_of_entries = sizeof(mmgsdi_device_features_list_init_table)/sizeof(mmgsdi_device_features_list_type);
      curr_version_in_build = mmgsdi_device_features_list_init_table[no_of_entries - 1].version_introduced;
      if(mmgsdi_device_features_status_list.version > curr_version_in_build)
      {
        mmgsdi_device_features_status_list.version = curr_version_in_build;
      }

      /* if the version is same as the build version update directly to EFS
               else, write the rest of the features and update */
      if(mmgsdi_device_features_status_list.version == curr_version_in_build)
      {
        if (uim_common_efs_write(UIM_COMMON_EFS_MMGSDI_DEVICE_SPECIFIC_FEATURE_STATUS_LIST,
                                 UIM_COMMON_EFS_ITEM_FILE_TYPE,
                                 UIM_COMMON_EFS_DEVICE,
                                 (const char*)&mmgsdi_device_features_status_list,
                                 sizeof(mmgsdi_device_features_status_list)) != UIM_COMMON_EFS_SUCCESS)
        {
          UIM_MSG_MED_0("MMGSDI Device Features Status NV ITEM version update failed!");
        }
      }
      else
      {
        /* update new EFS files */
        mmgsdi_nv_device_features_status_list_update_version();
      }
    }

    /* Updates are done, Safe to delete the legacy 67211 EFS file since the
       content is captured into new EFS files successfully*/
    (void)gsdi_efs_delete_file(
            (const char *)UIM_COMMON_EFS_PATH_MMGSDI_FEATURE_STATUS_LIST,
            efs_context);
    return;
  }

  /* since legacy features status list is not present, update or create slot
     specific features list */
  if(new_slot_features_status_efs_present)
  {
    mmgsdi_nv_slot_features_status_list_update_version(index);
  }
  else
  {
    mmgsdi_nv_write_default_value_of_all_slot_features(index);
    if (uim_common_efs_write(UIM_COMMON_EFS_MMGSDI_SLOT_SPECIFIC_FEATURE_STATUS_LIST,
                             UIM_COMMON_EFS_ITEM_FILE_TYPE,
                             efs_context,
                             (const char*)&mmgsdi_slot_features_status_list[index],
                             sizeof(mmgsdi_slot_features_status_list[index])) != UIM_COMMON_EFS_SUCCESS)
    {
      UIM_MSG_MED_0("MMGSDI Slot Features Status NV ITEM version update failed!");
    }
  }

  /* since legacy features status list is not present, update or create
     device specific features list */
  if(efs_context == UIM_COMMON_EFS_CONTEXT_0)
  {
    if(new_device_features_status_efs_present)
    {
      mmgsdi_nv_device_features_status_list_update_version();
    }
    else
    {
      mmgsdi_nv_write_default_value_of_all_device_features();
      if (uim_common_efs_write(UIM_COMMON_EFS_MMGSDI_DEVICE_SPECIFIC_FEATURE_STATUS_LIST,
                               UIM_COMMON_EFS_ITEM_FILE_TYPE,
                               UIM_COMMON_EFS_DEVICE,
                               (const char*)&mmgsdi_device_features_status_list,
                               sizeof(mmgsdi_device_features_status_list)) != UIM_COMMON_EFS_SUCCESS)
      {
        UIM_MSG_MED_0("MMGSDI Device Features Status NV ITEM version update failed!");
      }
    }
  }

  /* Delete the legacy 67211 EFS file if present  */
  if(legacy_features_status_efs_present)
  {
    (void)gsdi_efs_delete_file(
            (const char *)UIM_COMMON_EFS_PATH_MMGSDI_FEATURE_STATUS_LIST,
            efs_context);
  }
} /* mmgsdi_nv_init_features_status_nv_lookup */


/*===========================================================================
FUNCTION MMGSDI_NV_INIT_EXTERNAL_FEATURES_SESSION_ITEMS

DESCRIPTION
  Function used to get the status of session based external features (i.e.
  features outside of MMGSDI), stored in EFS based NV:
    NV 70361: CSG SUPPORT CONFIGURATION
    NV 67218: IMS ENABLE
    NV 67210: RPM INFO (Need further confirmation from MM folks whether this is
              ME based or session based NV item)
    NV 66048: VOICE DOMAIN PREF

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void mmgsdi_nv_init_external_features_session_items(
  uint8                        index
)
{
  uim_common_efs_context_type   efs_context        = UIM_COMMON_EFS_CONTEXT_MAX;
#ifdef FEATURE_FEMTO_CSG
  sys_csg_support_e_type        csg_support_status = SYS_CSG_SUPPORT_NONE;
#endif /* FEATURE_FEMTO_CSG */
  uint8                         ims_support_status = 0;
  sys_voice_domain_pref_e_type  voice_domain_pref  = SYS_VOICE_DOMAIN_PREF_NONE;

  PACKED struct PACKED_POST
  {
    uint8                       is_rpm_enabled;
    uint8                       app_max_num_reset;
    uint8                       app_reset_counter;
    uint32                      app_reset_timer;
    uint32                      app_reset_block_leak_rate_timer;
    uint8                       average_reset_time;
    uint32                      leak_rate_timer;
  } rpm_info;

  if (index >= sizeof(mmgsdi_external_features_status_list)/sizeof(mmgsdi_external_features_status_list[0]))
  {
    return;
  }

  efs_context = mmgsdi_util_get_common_efs_item_context(index);
  if (UIM_COMMON_EFS_CONTEXT_MAX == efs_context)
  {
    return;
  }

#ifdef FEATURE_FEMTO_CSG
#ifdef FEATURE_DISABLE_CSG_BYDEFAULT
  /* In this case, default value is support disabled */
  mmgsdi_external_features_status_list[index].nas_cfg_feature_csg = MMGSDI_FEATURE_DISABLED;
#else
  /* In this case, default value is support enabled */
  mmgsdi_external_features_status_list[index].nas_cfg_feature_csg = MMGSDI_FEATURE_ENABLED;
#endif /* FEATURE_DISABLE_CSG_BYDEFAULT */

  /* Read CSG SUPPORT CONFIGURATION (NV 70361) from EFS */
  if (uim_common_efs_read(UIM_COMMON_EFS_NON_UIM_CSG_SUPPORT_CONFIG,
                          UIM_COMMON_EFS_ITEM_FILE_TYPE,
                          efs_context,
                          (uint8 *)&csg_support_status,
                          sizeof(csg_support_status)) ==
      UIM_COMMON_EFS_SUCCESS)
  {
    /* Retrieve and store the status of the CSG feature */
    if(csg_support_status == SYS_CSG_SUPPORT_DISABLED)
    {
      mmgsdi_external_features_status_list[index].nas_cfg_feature_csg =
        MMGSDI_FEATURE_DISABLED;
    }
    else
    {
      mmgsdi_external_features_status_list[index].nas_cfg_feature_csg =
        MMGSDI_FEATURE_ENABLED;
    }
  }
#else
  mmgsdi_external_features_status_list[index].nas_cfg_feature_csg =
    MMGSDI_FEATURE_DISABLED;
#endif /* FEATURE_FEMTO_CSG */

  /* Read IMS ENABLE (NV 67218) from EFS, Default value: DISABLED */
  mmgsdi_external_features_status_list[index].ims_cfg_feature_ims_support =
    MMGSDI_FEATURE_DISABLED;
  if (uim_common_efs_read(UIM_COMMON_EFS_NON_UIM_IMS_ENABLE,
                          UIM_COMMON_EFS_ITEM_FILE_TYPE,
                          efs_context,
                          (uint8 *)&ims_support_status,
                          sizeof(ims_support_status)) ==
      UIM_COMMON_EFS_SUCCESS)
  {
    /* Retrieve and store the status of the IMS support
       0 - Disabled
       1 - Enabled
       2 - Proprietary IMS support */
    if(ims_support_status != 0)
    {
      mmgsdi_external_features_status_list[index].ims_cfg_feature_ims_support =
        MMGSDI_FEATURE_ENABLED;
    }
  }

  /* Read RPM info (NV 67210) from EFS, Default value: DISABLED */
  mmgsdi_external_features_status_list[index].rpm_enabled = MMGSDI_FEATURE_DISABLED;
  if (uim_common_efs_read(UIM_COMMON_EFS_NON_UIM_RPM_INFO,
                          UIM_COMMON_EFS_ITEM_FILE_TYPE,
                          efs_context,
                          (uint8 *)&rpm_info,
                          sizeof(rpm_info)) ==
      UIM_COMMON_EFS_SUCCESS)
  {
    if(rpm_info.is_rpm_enabled)
    {
      mmgsdi_external_features_status_list[index].rpm_enabled =
        MMGSDI_FEATURE_ENABLED;
    }
  }

  /* Read VOICE_DOMAIN_PREF (NV 66048) from EFS, Default value: DISABLED */
  mmgsdi_external_features_status_list[index].volte_using_ims = MMGSDI_FEATURE_DISABLED;
  if (uim_common_efs_read(UIM_COMMON_EFS_NON_UIM_VOICE_DOMAIN_PREF,
                          UIM_COMMON_EFS_ITEM_FILE_TYPE,
                          efs_context,
                          (uint8 *)&voice_domain_pref,
                          sizeof(voice_domain_pref)) ==
      UIM_COMMON_EFS_SUCCESS)
  {
    /* Retrieve and store the status of the VOLTE USING IMS support
       Enabled if the value is one of the following:
         SYS_VOICE_DOMAIN_PREF_IMS_PS_VOICE_ONLY
         SYS_VOICE_DOMAIN_PREF_CS_VOICE_PREFERRED
         SYS_VOICE_DOMAIN_PREF_IMS_PS_VOICE_PREFERRED
     */
    if (SYS_VOICE_DOMAIN_PREF_IMS_PS_VOICE_ONLY      == voice_domain_pref ||
        SYS_VOICE_DOMAIN_PREF_CS_VOICE_PREFERRED     == voice_domain_pref ||
        SYS_VOICE_DOMAIN_PREF_IMS_PS_VOICE_PREFERRED == voice_domain_pref)
    {
      mmgsdi_external_features_status_list[index].volte_using_ims =
        MMGSDI_FEATURE_ENABLED;
    }
  }

  UIM_MSG_MED_4("nas_cfg_feature_csg=0x%x, rpm_enabled=0x%x, ims_cfg_feature_ims_support=0x%x volte_using_ims=0x%x, cdma_less_device=0x%x",
                mmgsdi_external_features_status_list[index].nas_cfg_feature_csg,
                mmgsdi_external_features_status_list[index].rpm_enabled,
                mmgsdi_external_features_status_list[index].ims_cfg_feature_ims_support,
                mmgsdi_external_features_status_list[index].volte_using_ims);
}/* mmgsdi_nv_init_external_features_session_items */


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
void mmgsdi_nv_init_external_features_status_nv_lookup(
  uint8     index)
{
  /* Start off with the global struct initialised to 0 */
  memset(&mmgsdi_external_features_status_list[index], 0x00,
         sizeof(mmgsdi_external_features_status_list[index]));

  /* Init external features depending on slot based NV items
     With JCDMA removal, currently there is no slot based
     external features */

  /* Init external features depending on session based NV items */
  mmgsdi_nv_init_external_features_session_items(index);
} /* mmgsdi_nv_init_external_features_status_nv_lookup */


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
  uint8     slot_index)
{
  uim_common_efs_context_type  efs_context;
  uint8                        ens_enabled = 0;

  efs_context = mmgsdi_util_get_common_efs_item_context(slot_index);
  if (efs_context == UIM_COMMON_EFS_CONTEXT_MAX)
  {
    return;
  }

  /* Read the ENS Slot Enabled file from EFS */
  if ((uim_common_efs_read(UIM_COMMON_EFS_MMGSDI_ENS_SLOT_ENABLED,
                           UIM_COMMON_EFS_ITEM_FILE_TYPE,
                           efs_context,
                           (uint8 *)&ens_enabled,
                           sizeof(ens_enabled)) ==
       UIM_COMMON_EFS_SUCCESS) &&
       ens_enabled > 0 )
  {
    mmgsdi_ens_slot_enabled[slot_index] = MMGSDI_FEATURE_ENABLED;
  }
  else
  {
    /* The item is either inactive, or is active but is set to 0 explicitly */
    mmgsdi_ens_slot_enabled[slot_index] = MMGSDI_FEATURE_DISABLED;
  }

  UIM_MSG_LOW_2("ENS slot enabled EFS item status for slot_index 0x%x is 0x%x",
                slot_index, mmgsdi_ens_slot_enabled[slot_index]);
} /* mmgsdi_nv_init_ens_slot_enabled */


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
  mmgsdi_slot_id_enum_type     slot)
{
  uint8 slot_index = 0;

  if (mmgsdi_util_get_slot_index(slot, &slot_index) != MMGSDI_SUCCESS)
  {
    return FALSE;
  }

  return mmgsdi_ens_slot_enabled[slot_index];
} /* mmgsdi_nv_is_ens_enabled */


/*===========================================================================
FUNCTION MMGSDI_NV_GET_SLOT_FEATURE_STATUS

DESCRIPTION
  Function used to get the status of MMGSDI slot features listed in enum
  mmgsdi_nv_slot_specific_features_enum_type  as defined by -
  "MMGSDI_SLOT_FEATURES_STATUS_LIST".

DEPENDENCIES
  Latest QXDM version required to be able to read/write this NVITEM.
  This item is introduced in ver 3.14.1047

RETURN VALUE
  mmgsdi_feature_status_enum

SIDE EFFECTS
  None
===========================================================================*/
mmgsdi_feature_status_enum_type mmgsdi_nv_get_slot_feature_status(
  mmgsdi_nv_slot_specific_features_enum_type nv_feature,
  mmgsdi_nv_context_type                     nv_context)
{
  mmgsdi_feature_status_enum_type ret_value = MMGSDI_FEATURE_DISABLED;

  if(nv_feature >= (sizeof(mmgsdi_slot_features_list_init_table) / sizeof(mmgsdi_slot_features_list_type)))
  {
    return MMGSDI_FEATURE_DISABLED;
  }

#ifdef FEATURE_UIM_TEST_FRAMEWORK
  #error code not present
#endif /*FEATURE_UIM_TEST_FRAMEWORK*/

  if(nv_context >= MMGSDI_MAX_NUM_SLOTS)
  {
    return MMGSDI_FEATURE_DISABLED;
  }

  ret_value = mmgsdi_slot_features_status_list[nv_context].item[nv_feature];

  if(mmgsdi_slot_features_list_init_table[nv_feature].default_value != ret_value)
  {
    UIM_MSG_LOW_2("Status of Slot feature 0x%x is 0x%x", nv_feature, ret_value);
  }
  return ret_value;
} /* mmgsdi_nv_get_slot_feature_status */


/*===========================================================================
FUNCTION MMGSDI_NV_GET_DEVICE_FEATURE_STATUS

DESCRIPTION
  Function used to get the status of MMGSDI device features listed in enum
  mmgsdi_nv_device_specific_features_enum_type  as defined by -
  "MMGSDI_DEVICE_FEATURES_STATUS_LIST".

DEPENDENCIES
  Latest QXDM version required to be able to read/write this NVITEM.
  This item is introduced in ver 3.14.1047

RETURN VALUE
  mmgsdi_feature_status_enum

SIDE EFFECTS
  None
===========================================================================*/
mmgsdi_feature_status_enum_type mmgsdi_nv_get_device_feature_status(
  mmgsdi_nv_device_specific_features_enum_type nv_feature)
{
  mmgsdi_feature_status_enum_type ret_value = MMGSDI_FEATURE_DISABLED;

  if(nv_feature >= (sizeof(mmgsdi_device_features_list_init_table) / sizeof(mmgsdi_device_features_list_type)))
  {
    return MMGSDI_FEATURE_DISABLED;
  }

  ret_value = mmgsdi_device_features_status_list.item[nv_feature];

  if(mmgsdi_device_features_list_init_table[nv_feature].default_value != ret_value)
  {
    UIM_MSG_LOW_2("Status of Device feature 0x%x is 0x%x", nv_feature, ret_value);
  }

  return ret_value;
} /* mmgsdi_nv_get_device_feature_status */


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
mmgsdi_feature_status_enum_type mmgsdi_nv_get_external_feature_status(
  mmgsdi_nv_external_features_enum nv_feature,
  mmgsdi_nv_context_type           index)
{
  mmgsdi_feature_status_enum_type ret_value = MMGSDI_FEATURE_DISABLED;

  if (index >= sizeof(mmgsdi_external_features_status_list)/sizeof(mmgsdi_external_features_status_list[0]))
  {
    return MMGSDI_FEATURE_DISABLED;
  }

  switch(nv_feature)
  {
    case MMGSDI_EXT_FEAT_NAS_CFG_FEATURE_CSG:
      ret_value = mmgsdi_external_features_status_list[index].nas_cfg_feature_csg;
      break;

    case MMGSDI_EXT_FEAT_IMS_CFG_FEATURE_IMS_SUPPORT:
      ret_value = mmgsdi_external_features_status_list[index].ims_cfg_feature_ims_support;
      break;

    case MMGSDI_EXT_FEAT_RPM_ENABLED:
      ret_value = mmgsdi_external_features_status_list[index].rpm_enabled;
      break;

    case MMGSDI_EXT_FEAT_VOLTE_USING_IMS:
      ret_value = mmgsdi_external_features_status_list[index].volte_using_ims;
      break;

    default:
      UIM_MSG_LOW_1("Invalid external feature 0x%x", nv_feature);
      return ret_value;
  }

  UIM_MSG_LOW_2("Status of external feature 0x%x is 0x%x", nv_feature, ret_value);

  return ret_value;
} /* mmgsdi_nv_get_external_feature_status */


/*===========================================================================
FUNCTION MMGSDI_NV_UPDATE_RPM_ICCID

DESCRIPTION
  Compare the previously stored ICCID to the current ICCID of the card.  If the
  values are different update nv with the new ICCID from the card.
  Note that if the current card has no ICCID or if the ICCID is a non-standard
  ICCID (less than 10 bytes), all zeros (10 bytes) are written to the nv (to
  indicate no/non-standard ICCID).

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE:
  None

SIDE EFFECTS
  mmgsdi_rpm_iccid_has_changed_slot will be:
    TRUE : if the ICCID is different than the last card inserted in the ME OR
           if the ICCID is zero or a non-standard ICCID
    FALSE: if the ICCID is the same as the last card inserted in the ME
===========================================================================*/
void mmgsdi_nv_update_rpm_iccid (
  mmgsdi_data_type         new_rpm_iccid,
  mmgsdi_slot_id_enum_type mmgsdi_slot
)
{
  uint8                   mmgsdi_rpm_iccid[MMGSDI_MAX_NUM_SLOTS][MMGSDI_ICCID_LEN];
  mmgsdi_slot_data_type  *slot_data_ptr                        = NULL;
  uint8                   slot_index                           = MMGSDI_SLOT_1_INDEX;
  uint8                   mmgsdi_invalid_iccid[MMGSDI_ICCID_LEN];

  UIM_MSG_HIGH_0("UPDATE RPM ICCID");

  memset(mmgsdi_invalid_iccid, 0x00, sizeof(mmgsdi_invalid_iccid));
  memset(mmgsdi_rpm_iccid, 0x00, sizeof(mmgsdi_rpm_iccid));

  if((new_rpm_iccid.data_ptr == NULL) ||
     (new_rpm_iccid.data_len != MMGSDI_ICCID_LEN))
  {
     /* Incase ICCID EF is not present on the card or there is some other
       issue reading ICCID from card (or ICCID reported by card in a non-
       standard ICCID) and we reached here with a NULL iccid or non-standard
       iccid, make sure we write 0x00s (10 bytes) as ICCID to the RPM EFS.
       This is to make sure we can treat this card as a new card if
       no/non-standard ICCID is present on the card (or there are issues
       reading ICCID from the card). */
    new_rpm_iccid.data_len = sizeof(mmgsdi_invalid_iccid);
    new_rpm_iccid.data_ptr = mmgsdi_invalid_iccid;
  }

  (void) mcfg_fs_read(MMGSDI_RPM_ICCID_FILE, (void *)mmgsdi_rpm_iccid,
                      sizeof(mmgsdi_rpm_iccid), MCFG_FS_TYPE_EFS, MCFG_FS_SUBID_NONE);

  /* Enter crit sect to protect global */
  MMGSDIUTIL_ENTER_CLIENT_APP_DATA_CRIT_SECT;

  do
  {
    slot_data_ptr = mmgsdi_util_get_slot_data_ptr(mmgsdi_slot);
    if(slot_data_ptr == NULL)
    {
      break;
    }

    if(MMGSDI_SUCCESS != mmgsdi_util_get_slot_index(mmgsdi_slot, &slot_index))
    {
      break;
    }

    if(memcmp(mmgsdi_rpm_iccid[slot_index],
              new_rpm_iccid.data_ptr,
              MMGSDI_ICCID_LEN))
    {
      slot_data_ptr->mmgsdi_rpm_iccid_has_changed = TRUE;
      mmgsdi_memscpy(mmgsdi_rpm_iccid[slot_index],
                     sizeof(mmgsdi_rpm_iccid[slot_index]),
                     new_rpm_iccid.data_ptr,
                     MMGSDI_ICCID_LEN);

      (void) mcfg_fs_write(MMGSDI_RPM_ICCID_FILE, (void *)mmgsdi_rpm_iccid,
                           sizeof(mmgsdi_rpm_iccid), MCFG_FS_O_CREAT|MCFG_FS_O_RDWR|MCFG_FS_O_AUTODIR|MCFG_FS_O_TRUNC,
                           0777, MCFG_FS_TYPE_EFS, MCFG_FS_SUBID_NONE);
    }
    else if(!memcmp(mmgsdi_rpm_iccid[slot_index],
                    mmgsdi_invalid_iccid,
                    MMGSDI_ICCID_LEN))
    {
      /* We reach here if EFS file already contains an invalid (all zeros) ICCID
         and current ICCID is also a non-standard one (or there is no ICCID on the
         card). In this case, since the EFS file already contains an invalid ICCID,
         no need to update the EFS file again with an invalid ICCID (all zeros) -
         simply update the global indicating that the ICCID has changed.
         Yes, this does mean that we would be indicating that the ICCID has changed
         even if the same card having no-ICCID that was inserted in the device the
         last time is re-inserted into the device... */
      slot_data_ptr->mmgsdi_rpm_iccid_has_changed = TRUE;
    }
    else
    {
      slot_data_ptr->mmgsdi_rpm_iccid_has_changed = FALSE;
    }

    if(slot_data_ptr->mmgsdi_rpm_iccid_has_changed)
    {
      UIM_MSG_HIGH_1("RPM ICCID has changed for SLOT 0x%x", mmgsdi_slot);
    }
  } while(0);

  MMGSDIUTIL_LEAVE_CLIENT_APP_DATA_CRIT_SECT;
} /* mmgsdi_nv_update_rpm_iccid */


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
)
{
  mmgsdi_slot_data_type  *slot_data_ptr = NULL;
  boolean                 rpm_iccid_has_changed = FALSE;

  /* Protect access to slot data table */
  MMGSDIUTIL_ENTER_CLIENT_APP_DATA_CRIT_SECT;

  slot_data_ptr = mmgsdi_util_get_slot_data_ptr(mmgsdi_slot);
  if(slot_data_ptr == NULL)
  {
    MMGSDIUTIL_LEAVE_CLIENT_APP_DATA_CRIT_SECT;
    return FALSE;
  }
  rpm_iccid_has_changed = slot_data_ptr->mmgsdi_rpm_iccid_has_changed;

  MMGSDIUTIL_LEAVE_CLIENT_APP_DATA_CRIT_SECT;
  return rpm_iccid_has_changed;
} /* mmgsdi_nv_get_rpm_iccid_has_changed */


/*===========================================================================
FUNCTION MMGSDI_NV_SET_DEFAULT_VALUE_OF_ALL_ENS_SUB_FEATURES

DESCRIPTION
  Function used to set the default values of all the ENS sub features
  (controlled by ens_sub_feature_status_list NVITEM), to the
  global mmgsdi_ens_sub_features_status_list.

DEPENDENCIES
  None

RETURN VALUE
  void

SIDE EFFECTS
  None
===========================================================================*/
static void mmgsdi_nv_set_default_value_of_all_ens_sub_features(
  void
)
{
  memset(&mmgsdi_ens_sub_features_status_list, 0x00, sizeof(mmgsdi_ens_sub_features_status_list));

  /* Update mmgsdi_ens_sub_features_status_list with default values */
  mmgsdi_ens_sub_features_status_list.scan_control_application_support = MMGSDI_FEATURE_ENABLED;
  mmgsdi_ens_sub_features_status_list.nw_selection_menu_control_application_support = MMGSDI_FEATURE_ENABLED;
  mmgsdi_ens_sub_features_status_list.acting_hplmn_rescan_feature_support = MMGSDI_FEATURE_ENABLED;
  mmgsdi_ens_sub_features_status_list.rat_balancing_support = MMGSDI_FEATURE_ENABLED;
} /* mmgsdi_nv_set_default_value_of_all_ens_sub_features */


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
void mmgsdi_nv_init_ens_sub_features_status_nv_lookup(
  void
)
{
  /* Start off with the global struct initialised to 0 */
  memset(&mmgsdi_ens_sub_features_status_list, 0x00, sizeof(mmgsdi_ens_sub_features_status_list));

  UIM_MSG_HIGH_0("mmgsdi_nv_init_ens_sub_features_status_nv_lookup");

  if (uim_common_efs_read(UIM_COMMON_EFS_MMGSDI_ENS_SUB_FEATURES_STATUS_LIST,
                          UIM_COMMON_EFS_ITEM_FILE_TYPE,
                          UIM_COMMON_EFS_DEVICE,
                          (uint8 *) &mmgsdi_ens_sub_features_status_list,
                          sizeof(mmgsdi_ens_sub_features_status_list)) != UIM_COMMON_EFS_SUCCESS)
  {
    /* Updating global mmgsdi_ens_sub_features_status_list with default values */
    mmgsdi_nv_set_default_value_of_all_ens_sub_features();
  }
} /* mmgsdi_nv_init_ens_sub_features_status_nv_lookup */


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
)
{
  mmgsdi_feature_status_enum_type ret_value = MMGSDI_FEATURE_DISABLED;

  switch(ens_feature)
  {
    case MMGSDI_NV_SCAN_CONTROL_APPLICATION_SUPPORT_I:
      ret_value = mmgsdi_ens_sub_features_status_list.scan_control_application_support;
      break;
    case MMGSDI_NV_NETWORK_SELECTION_MENU_CONTROL_APPLICATION_SUPPORT_I:
      ret_value = mmgsdi_ens_sub_features_status_list.nw_selection_menu_control_application_support;
      break;
    case MMGSDI_NV_ACTING_HPLMN_RESCAN_FEATURE_SUPPORT_I:
      ret_value = mmgsdi_ens_sub_features_status_list.acting_hplmn_rescan_feature_support;
      break;
    case MMGSDI_NV_RAT_BALANCING_SUPPORT_I:
      ret_value = mmgsdi_ens_sub_features_status_list.rat_balancing_support;
      break;
    default:
      UIM_MSG_HIGH_1("Invalid Feature 0x%x", ens_feature);
      return ret_value;
  }
  UIM_MSG_HIGH_2("Status of ENS sub feature 0x%x is 0x%x", ens_feature, ret_value);
  return ret_value;
} /* mmgsdi_nv_get_ens_sub_feature_status */


#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */


/*===========================================================================
FUNCTION MMGSDI_NV_REFRESH_FEATURE_STATUS_LIST

DESCRIPTION
  Function to refresh the MMGSDI feature status list for the index
  corresponding to the card slot on which refresh is occuring. It also
  restores the values of ME-specific items since they should not be part of
  the NV refresh.

DEPENDENCIES
  None

RETURN VALUE
  mmgsdi_return_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static mmgsdi_return_enum_type mmgsdi_nv_refresh_feature_status_list(
  uint8    efs_index)
{
  uim_common_efs_context_type        efs_context = UIM_COMMON_EFS_CONTEXT_MAX;

  if (efs_index >= MMGSDI_MAX_NUM_SLOTS)
  {
    return MMGSDI_INCORRECT_PARAMS;
  }

  efs_context = mmgsdi_util_get_common_efs_item_context(efs_index);
  if (efs_context == UIM_COMMON_EFS_CONTEXT_MAX)
  {
    return MMGSDI_INCORRECT_PARAMS;
  }

  if (uim_common_efs_read(UIM_COMMON_EFS_MMGSDI_SLOT_SPECIFIC_FEATURE_STATUS_LIST,
                          UIM_COMMON_EFS_ITEM_FILE_TYPE,
                          efs_context,
                          (uint8 *)&mmgsdi_slot_features_status_list[efs_index],
                          sizeof(mmgsdi_slot_features_status_list[efs_index])) ==
      UIM_COMMON_EFS_SUCCESS)
  {
    mmgsdi_nv_slot_features_status_list_update_version(efs_index);
  }
  else
  {
    mmgsdi_nv_write_default_value_of_all_slot_features(efs_index);
    if (uim_common_efs_write(UIM_COMMON_EFS_MMGSDI_SLOT_SPECIFIC_FEATURE_STATUS_LIST,
                             UIM_COMMON_EFS_ITEM_FILE_TYPE,
                             efs_context,
                             (const char*)&mmgsdi_slot_features_status_list[efs_index],
                             sizeof(mmgsdi_slot_features_status_list[efs_index])) != UIM_COMMON_EFS_SUCCESS)
    {
      UIM_MSG_MED_0("MMGSDI Slot Features Status NV ITEM version update failed!");
    }
  }

  return MMGSDI_SUCCESS;
} /* mmgsdi_nv_refresh_feature_status_list */


/*===========================================================================
FUNCTION MMGSDI_NV_CLEAR_NV_ITEM_CACHE

DESCRIPTION
  Function to clear the NV cache

DEPENDENCIES
  None

RETURN VALUE
  mmgsdi_return_enum_type

SIDE EFFECTS
  None
===========================================================================*/
void mmgsdi_nv_clear_nv_item_cache (
  nv_items_enum_type     item_enum,
  mmgsdi_nv_context_type nv_context
)
{
  uint32 i    = 0x0;
  uint32 mask = 0x1;

  if (nv_context >= MMGSDI_NV_CONTEXTS_MAX ||
      mmgsdi_nv_item_cache_table_ptr[nv_context] == NULL)
  {
    UIM_MSG_ERR_1("nv_context - 0x%x is invalid or mmgsdi_nv_item_cache_table_ptr is NULL",
                  nv_context);
    return;
  }

  UIM_MSG_LOW_2("clear NV item enum 0x%x context 0x%x", item_enum, nv_context);

  for (i=0; i<MMGSDI_NV_ITEM_CACHE_SIZE; i++)
  {
    if (mmgsdi_nv_item_cache_table_ptr[nv_context]->item[i].item_enum == item_enum)
    {
      /* Create bit mask using the index of the item to determine if cached */
      if (mmgsdi_nv_item_cache_table_ptr[nv_context]->is_cached_mask &
          (mask << i))
      {
        mmgsdi_nv_item_cache_table_ptr[nv_context]->is_cached_mask &= (~(mask << i));
        return;
      }
    }
  }

  return;
} /* mmgsdi_nv_clear_nv_item_cache */


/*===========================================================================
FUNCTION MMGSDI_NV_REFRESH_SLOT_ITEMS

DESCRIPTION
  Function to refresh the slot-specific items from NV/EFS at the index
  corresponding to the slot on which refresh is happening.

DEPENDENCIES
  None

RETURN VALUE
  mmgsdi_return_enum_type

SIDE EFFECTS
  None
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_nv_refresh_slot_items(
  mcfg_refresh_index_type    refresh_index)
{
  uim_common_efs_context_type  efs_context    = UIM_COMMON_EFS_CONTEXT_MAX;
  mmgsdi_nv_context_type       nv_efs_index   = MMGSDI_NV_CONTEXT_INVALID;
  uint8                        slot_index     = MMGSDI_SLOT_1_INDEX;

  switch (refresh_index)
  {
    case MCFG_REFRESH_INDEX_0:
      nv_efs_index = MMGSDI_NV_CONTEXT_PRIMARY;
      efs_context  = UIM_COMMON_EFS_CONTEXT_0;
      slot_index   = MMGSDI_SLOT_1_INDEX;
      break;
    case MCFG_REFRESH_INDEX_1:
      nv_efs_index = MMGSDI_NV_CONTEXT_SECONDARY;
      efs_context  = UIM_COMMON_EFS_CONTEXT_1;
      slot_index   = MMGSDI_SLOT_2_INDEX;
      break;
    case MCFG_REFRESH_INDEX_2:
      nv_efs_index = MMGSDI_NV_CONTEXT_TERTIARY;
      efs_context  = UIM_COMMON_EFS_CONTEXT_2;
      slot_index   = MMGSDI_SLOT_3_INDEX;
      break;
    default:
      return MMGSDI_INCORRECT_PARAMS;
  }

  /* Protect NV/EFS data access from external clients  during NV Refresh */
  MMGSDIUTIL_ENTER_CLIENT_APP_DATA_CRIT_SECT;

  /* Clear the NV cache
     With JCDMA removal, there is no catched NV items to be cleared */

  mmgsdi_nv_init_ens_slot_enabled(nv_efs_index);

  override_opl_pnn_lookup_for_rplmn[slot_index] = MMGSDI_OVERRIDE_FEATURE_NOT_INITIALIZED;

  (void)mmgsdi_nv_refresh_feature_status_list(nv_efs_index);

  (void)mmgsdi_refresh_retry_init(nv_efs_index);

  /* Init external features depending on slot based NV items
     With JCDMA removal, currently there is no slot based
     external features */

  MMGSDIUTIL_LEAVE_CLIENT_APP_DATA_CRIT_SECT;

  return MMGSDI_SUCCESS;
} /* mmgsdi_nv_refresh_slot_items */


/*===========================================================================
FUNCTION MMGSDI_NV_REFRESH_SESSION_ITEMS

DESCRIPTION
  Function to refresh the session-specific items from NV/EFS at the index
  corresponding to the slot on which refresh is happening.

DEPENDENCIES
  None

RETURN VALUE
  mmgsdi_return_enum_type

SIDE EFFECTS
  None
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_nv_refresh_session_items(
  mcfg_refresh_index_type    sub_index)
{
  uim_common_efs_context_type   efs_context              = UIM_COMMON_EFS_CONTEXT_0;
  mmgsdi_nv_context_type        nv_item_context          = MMGSDI_NV_CONTEXT_INVALID;
  uint8                         slot_index               = MMGSDI_SLOT_1_INDEX;
  mmgsdi_nv_item_cache_type     cached_mmgsdi_nv_data    = {0};
  mmgsdi_nv_item_cache_type     refreshed_mmgsdi_nv_data = {0};
  nv_stat_enum_type             result                   = NV_NOTALLOC_S;
  nv_stat_enum_type             cached_result            = NV_NOTALLOC_S;

  switch (sub_index)
  {
    case MCFG_REFRESH_INDEX_0:
      nv_item_context = MMGSDI_NV_CONTEXT_PRIMARY;
      efs_context     = UIM_COMMON_EFS_CONTEXT_0;
      slot_index      = MMGSDI_SLOT_1_INDEX;
      break;
    case MCFG_REFRESH_INDEX_1:
      nv_item_context = MMGSDI_NV_CONTEXT_SECONDARY;
      efs_context     = UIM_COMMON_EFS_CONTEXT_1;
      slot_index      = MMGSDI_SLOT_2_INDEX;
      break;
    case MCFG_REFRESH_INDEX_2:
      nv_item_context = MMGSDI_NV_CONTEXT_TERTIARY;
      efs_context     = UIM_COMMON_EFS_CONTEXT_2;
      slot_index      = MMGSDI_SLOT_3_INDEX;
      break;
    default:
      return MMGSDI_INCORRECT_PARAMS;
  }

  UIM_MSG_HIGH_1("Refresh of NV triggered on SUB: 0x%x", sub_index);

  /* Protect NV/EFS data access from external clients  during NV Refresh */
  MMGSDIUTIL_ENTER_CLIENT_APP_DATA_CRIT_SECT;

#ifdef FEATURE_3GPP_CSFB
  mmgsdi_nv_clear_nv_item_cache(NV_SERVICE_DOMAIN_PREF_I, nv_item_context);
  mmgsdi_nv_clear_nv_item_cache(NV_DISABLE_CM_CALL_TYPE_I, nv_item_context);

  (void)mmgsdi_nv_get_item(NV_SERVICE_DOMAIN_PREF_I, &cached_mmgsdi_nv_data, nv_item_context);
  (void)mmgsdi_nv_get_item(NV_DISABLE_CM_CALL_TYPE_I, &cached_mmgsdi_nv_data, nv_item_context);
#endif/* FEATURE_3GPP_CSFB */

  /* Clear and re cache NV 855 RTRE CONFIG if NV refresh is triggered */
  memset(&cached_mmgsdi_nv_data, 0x00, sizeof(mmgsdi_nv_item_cache_type));

  /* Get RTRE configuration from Cache*/
  cached_result = mmgsdi_nv_get_item(NV_RTRE_CONFIG_I, &cached_mmgsdi_nv_data,
                    nv_item_context);

  mmgsdi_nv_clear_nv_item_cache(NV_RTRE_CONFIG_I, nv_item_context);

  /* Get RTRE configuration from NV. If the new RTRE doesn't match
     with cached RTRE then re-cache the RTRE value*/
  result = mmgsdi_nv_get_item(NV_RTRE_CONFIG_I, &refreshed_mmgsdi_nv_data,
             nv_item_context);
  if(result        != NV_DONE_S ||
     cached_result != NV_DONE_S ||
     (cached_mmgsdi_nv_data.item_value.rtre_config !=
      refreshed_mmgsdi_nv_data.item_value.rtre_config))
  {
    mmgsdi_init_rtre_config_per_context(nv_item_context);
  }

  /* Init external features depending on session based NV items */
  mmgsdi_nv_init_external_features_session_items(slot_index);

  MMGSDIUTIL_LEAVE_CLIENT_APP_DATA_CRIT_SECT;

  return MMGSDI_SUCCESS;
} /* mmgsdi_nv_refresh_session_items */


/*===========================================================================
  FUNCTION MMGSDI_UTIL_GET_EFS_ITEM_INDEX_FOR_SLOT

  DESCRIPTION
    Returns the index of the EFS item based on the slot id.

  PARAMETERS
    slot id

  RETURN VALUE
    mmgsdi_nv_context_type

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
mmgsdi_nv_context_type mmgsdi_util_get_efs_item_index_for_slot(
  mmgsdi_slot_id_enum_type  slot_id
)
{
  switch (slot_id)
  {
    case MMGSDI_SLOT_1:
      return MMGSDI_NV_CONTEXT_PRIMARY;
    case MMGSDI_SLOT_2:
      return MMGSDI_NV_CONTEXT_SECONDARY;
    case MMGSDI_SLOT_3:
      return MMGSDI_NV_CONTEXT_TERTIARY;
    default:
      UIM_MSG_ERR_0("Invalid Slot");
      break;
  }

  return MMGSDI_NV_CONTEXT_INVALID;
} /* mmgsdi_util_get_efs_item_index_for_slot */


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
)
{
  switch (index)
  {
    case 0:
      return UIM_COMMON_EFS_CONTEXT_0;
    case 1:
      return UIM_COMMON_EFS_CONTEXT_1;
    case 2:
      return UIM_COMMON_EFS_CONTEXT_2;
    default:
      return UIM_COMMON_EFS_CONTEXT_MAX;
  }
}/*mmgsdi_util_get_common_efs_item_context*/


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
)
{
  mmgsdi_session_type_enum_type  session_type = MMGSDI_MAX_SESSION_TYPE_ENUM;

  MMGSDIUTIL_RETURN_IF_NULL(efs_context_ptr);

  if (mmgsdi_util_get_session_type(session_id, &session_type, NULL) != MMGSDI_SUCCESS)
  {
    return MMGSDI_ERROR;
  }

  switch (session_type)
  {
    case MMGSDI_GW_PROV_PRI_SESSION:
    case MMGSDI_1X_PROV_PRI_SESSION:
    case MMGSDI_CARD_SESSION_SLOT_1:
      *efs_context_ptr = UIM_COMMON_EFS_CONTEXT_0;
      break;
    case MMGSDI_GW_PROV_SEC_SESSION:
    case MMGSDI_1X_PROV_SEC_SESSION:
    case MMGSDI_CARD_SESSION_SLOT_2:
      *efs_context_ptr = UIM_COMMON_EFS_CONTEXT_1;
      break;
    case MMGSDI_GW_PROV_TER_SESSION:
    case MMGSDI_1X_PROV_TER_SESSION:
    case MMGSDI_CARD_SESSION_SLOT_3:
      *efs_context_ptr = UIM_COMMON_EFS_CONTEXT_2;
      break;
    default:
      return MMGSDI_ERROR;
  }

  return MMGSDI_SUCCESS;
} /* mmgsdi_util_get_efs_item_context */


/* ============================================================================
FUNCTION MMGSDI_NV_GET_REMOTE_TEST_PARAMS

DESCRIPTION
  This function checks if the test EFS item for Remote SIM is present. If
  present and slot ID matches the requested slot ID, it reads the file.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  boolean:  TRUE : If it is read successfully for the requested slot
            FALSE: If file not present or invalid file size

SIDE EFFECTS
  None
============================================================================*/
boolean mmgsdi_nv_get_remote_test_params(
  mmgsdi_slot_id_enum_type              mmgsdi_slot_id,
  mmgsdi_remote_sim_test_params_type  * test_param_ptr
)
{
  uint32                     file_len      = 0;
  uint8                      file_data[2]  = {0,0};
  char                     * file_handle_ptr = NULL;
  mmgsdi_return_enum_type    mmgsdi_status = MMGSDI_ERROR;

  if (test_param_ptr == NULL)
  {
    return FALSE;
  }

  memset(test_param_ptr, 0x00, sizeof(mmgsdi_remote_sim_test_params_type));

  switch (mmgsdi_slot_id)
  {
    case MMGSDI_SLOT_1:
      file_handle_ptr = UIM_COMMON_EFS_PATH_MMGSDI_RMT_SIM_TEST_FILE_SLOT_1;
      break;
    case MMGSDI_SLOT_2:
      file_handle_ptr = UIM_COMMON_EFS_PATH_MMGSDI_RMT_SIM_TEST_FILE_SLOT_2;
      break;
    case MMGSDI_SLOT_3:
      file_handle_ptr = UIM_COMMON_EFS_PATH_MMGSDI_RMT_SIM_TEST_FILE_SLOT_3;
      break;
    default:
      return FALSE;
  }

  if(gsdi_efs_name_test((const char *)file_handle_ptr,
                        UIM_COMMON_EFS_DEVICE) != MMGSDI_SUCCESS)
  {
    return FALSE;
  }

  file_len = sizeof(file_data);

  mmgsdi_status =  gsdi_efs_read_file((const char *)file_handle_ptr,
                                      &file_len,
                                      (uint8 *)&file_data,
                                      UIM_COMMON_EFS_DEVICE);

  if (mmgsdi_status != MMGSDI_SUCCESS || file_len != sizeof(file_data))
  {
    return FALSE;
  }

  test_param_ptr->delay_interval = ((uint16)file_data[0] << 8) | file_data[1];
  UIM_MSG_LOW_1("Test AUTH delay interval: 0x%x msec", test_param_ptr->delay_interval);

  return TRUE;
} /* mmgsdi_nv_get_remote_test_params */


/*===========================================================================
FUNCTION MMGSDI_NV_GET_OVERRIDE_OPL_PNN_LOOKUP_FOR_NON_RPLMN_FEATURE_STATUS

DESCRIPTION
  Function used to get the status of override_opl_pnn_lookup_for_non_rplmn
  feature status for the given slot and for first time it reads it from EFS
  file.

DEPENDENCIES
  None

RETURN VALUE
  mmgsdi_feature_override_feature_status_enum_type

SIDE EFFECTS
  None
===========================================================================*/
mmgsdi_feature_override_feature_status_enum_type mmgsdi_nv_get_override_opl_pnn_lookup_for_non_rplmn_feature_status(
  mmgsdi_slot_id_enum_type     slot
)
{
  uint8                           slot_index  = 0;
  uim_common_efs_context_type     efs_context = UIM_COMMON_EFS_CONTEXT_MAX;

  if (mmgsdi_util_get_slot_index(slot, &slot_index) != MMGSDI_SUCCESS)
  {
    return MMGSDI_OVERRIDE_FEATURE_NO_OVERRIDE;
  }

  if(override_opl_pnn_lookup_for_rplmn[slot_index] == MMGSDI_OVERRIDE_FEATURE_NOT_INITIALIZED)
  {
    efs_context = mmgsdi_util_get_common_efs_item_context(slot_index);
    if (efs_context == UIM_COMMON_EFS_CONTEXT_MAX)
    {
      return MMGSDI_OVERRIDE_FEATURE_NO_OVERRIDE;
    }

    /* Read the override_opl_pnn_lookup feature status file from EFS */
    if(uim_common_efs_read(UIM_COMMON_EFS_MMGSDI_OVERRIDE_OPL_PNN_LOOKUP_FOR_NON_RPLMN,
                           UIM_COMMON_EFS_ITEM_FILE_TYPE,
                           efs_context,
                           (uint8 *)&override_opl_pnn_lookup_for_rplmn[slot_index],
                           sizeof(uint8)) != UIM_COMMON_EFS_SUCCESS)
    {
      override_opl_pnn_lookup_for_rplmn[slot_index] = MMGSDI_OVERRIDE_FEATURE_NO_OVERRIDE;
    }
  }

  UIM_MSG_LOW_2("MMGSDI_OVERRIDE_OPL_PNN_LOOKUP_FOR_NON_RPLMN status 0x%x for slot_index is 0x%x",
                  slot_index, override_opl_pnn_lookup_for_rplmn[slot_index]);

  return override_opl_pnn_lookup_for_rplmn[slot_index];
} /* mmgsdi_nv_get_override_opl_pnn_lookup_for_non_rplmn_feature_status */
