/**
  @file policy_device_config.c

  @brief  Policy which reads the device configuration and makes it available
          to the rest of modem.
*/

/*
    Copyright (c) 2013-2018,2021 QUALCOMM Technologies Incorporated.
    All Rights Reserved.
    Qualcomm Technologies Confidential and Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by this
  document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

  $Header: //components/rel/mmcp.mpss/7.9.0/policyman/src/policyman_device_config.c#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/

#include "mre_engine.h"
#include "mre_regex.h"
#include "mre_efs.h"

#include "policyman_call_mode.h"
#include "policyman_cfgitem.h"
#include "policyman_device_config.h"
#include "policyman_dbg.h"
#include "policyman_diag.h"
#include "policyman_efs.h"
#include "policyman_policies.h"
#include "policyman_policy.h"
#include "policyman_rat_capability.h"
#include "policyman_rf.h"
#include "policyman_rules.h"
#include "policyman_subs.h"
#include "policyman_task.h"
#include "policyman_util.h"
#include "policyman_xml.h"

#include "log.h"
#include "trm.h"
#include <stringl/stringl.h>



STATIC size_t  g_num_Sim             = 0;
STATIC size_t  g_maxActiveVoiceSubs  = 0;

#define PARAMETER_MASK_NONE           0x00000000  /* Only for initialization, not testing */
#define PARAMETER_MASK_NMAXVOICE      0x00000001  /* max_active_voice is present */
#define PARAMETER_MASK_NMAXDATA       0x00000002  /* max_active_data is present */
#define PARAMETER_MASK_CONFIG         0x00000004  /* configs are present */

typedef struct
{
  POLICYMAN_CMD_HDR;

  int  explicit_index;
} policyman_explicit_index_cmd_t;


#if (defined(FEATURE_CDMA) || defined(CDMA_FEATURE_DEFINED) || defined(FEATURE_CDMA1X) || defined(FEATURE_CDMA_1900) || defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_RX_DIVERSITY))
  #define CDMA_FEATURE_DEFINED
#endif

/*-------- policyman_device_config_init --------*/
void policyman_device_config_init(
  void
)
{
  g_num_Sim            = 0;
  g_maxActiveVoiceSubs = 0;
}

/*=============================================================================
  Device configuration item and APIs to access it.
=============================================================================*/

/*-------- policyman_device_config_dtor --------*/
STATIC void policyman_device_config_dtor(
  void  *pObj
)
{
  device_configuration_t *pMe = (device_configuration_t *)pObj;

  policyman_mem_free((void *)pMe->pECallRatMask);
  policyman_mem_free((void *)pMe->pConfigs);
  mre_simple_dtor(pObj);
}

 /*-------- policyman_device_config_item_new --------*/
device_configuration_t * policyman_device_config_item_new(
  sys_modem_as_id_e_type  subsId
)
{
  return  POLICYMAN_CFGITEM_NEW( POLICYMAN_ITEM_DEVICE_CONFIGURATION,
                                 device_configuration_t,
                                 policyman_device_config_dtor,
                                 subsId );
}

/*-------- policyman_efs_put_file_device_config --------*/
STATIC mre_efs_status_t policyman_efs_put_file_device_config(
  policyman_item_t const    *pItem
)
{
  mre_efs_status_t        efs_status  = MRE_EFS_STATUS_SUCCESS;
  device_configuration_t *pDcItem      = (device_configuration_t *)pItem;
  device_config_efs_v4_t  dev_cfg_efs;

  dev_cfg_efs.version          = POLICYMAN_DEVICE_CONFIG_VERSION_TAG + POLICYMAN_DEVICE_CONFIG_VERSION_NUM;
  dev_cfg_efs.num_sims         = pDcItem->num_sims;
  dev_cfg_efs.max_active_voice = pDcItem->max_active_voice;
  dev_cfg_efs.max_active_data  = pDcItem->max_active_data;
  dev_cfg_efs.explicit_index   = pDcItem->explicit_index;

  efs_status = mre_efs_put_file( POLICYMAN_DEVICE_CONFIG_NV_PATH,
                                 (void *)&dev_cfg_efs,
                                 sizeof(dev_cfg_efs),
                                 FALSE,
                                 SYS_MODEM_AS_ID_NONE );
  return efs_status;
}


/*-------- policyman_device_config_update_to_efs --------*/
void policyman_device_config_update_to_efs(
  policyman_item_t const *pItem
)
{
  device_configuration_t  *pDcItem = (device_configuration_t *)pItem;

  g_num_Sim = pDcItem->num_sims;
  policyman_efs_put_file_device_config(pItem);

  POLICYMAN_MSG_HIGH_4( "Wrote device config to efs: numSims %d, maxActiveVoice %d, maxActiveData %d, explicit index %d",
                        pDcItem->num_sims,
                        pDcItem->max_active_voice,
                        pDcItem->max_active_data,
                        pDcItem->explicit_index );
}

#define EFS_VALIDATE(test)                            \
  test_is_valid     = (test);                         \
  if (test_is_valid) { validities |= BM(num_tests); } \
  ++num_tests;                                        \
  config_is_valid  &= test_is_valid

/*-------- policyman_device_config_validate_efs_config --------*/
STATIC boolean policyman_device_config_validate_efs_config(
  device_config_efs_v4_t  *dev_cfg_efs,
  device_context_t        *pEfsDevCfg
)
{
  boolean  config_is_valid = TRUE;  // Overall result
  boolean  test_is_valid   = TRUE;  //
  size_t   validities      = 0;     // Bitmask of test results
  size_t   num_tests       = 0;     // Number of tests performed

  EFS_VALIDATE(   (dev_cfg_efs->version > 0)
               && (dev_cfg_efs->version <= POLICYMAN_DEVICE_CONFIG_VERSION_NUM) );

  EFS_VALIDATE(   (dev_cfg_efs->num_sims > 0)
               && (dev_cfg_efs->num_sims <= POLICYMAN_NUM_SUBS) );

  pEfsDevCfg->num_sims = dev_cfg_efs->num_sims;

  EFS_VALIDATE(   (dev_cfg_efs->max_active_voice > 0)
               && (dev_cfg_efs->max_active_voice <= POLICYMAN_NUM_SUBS) );

  pEfsDevCfg->max_active_voice = dev_cfg_efs->max_active_voice;

  // V1 and V2 have config (feature and ratMask)
  // Map to V2 structure (common with V1) and verify
  {
    if (dev_cfg_efs->version < 3)
    {
      device_config_efs_v2_t  *dev_cfg_efs_v2 = (device_config_efs_v2_t *)dev_cfg_efs;

      EFS_VALIDATE(   (dev_cfg_efs_v2->config.feature >= SYS_SUBS_FEATURE_MODE_NORMAL)
                   && (dev_cfg_efs_v2->config.feature <= SYS_SUBS_FEATURE_MODE_SRLTE) );
    }
  }

  // V1 does not have max_active_data.
  // V2 has max active data in different location from V3
  {
    if (2 == dev_cfg_efs->version)
    {
      device_config_efs_v2_t  *dev_cfg_efs_v2 = (device_config_efs_v2_t *)dev_cfg_efs;

      EFS_VALIDATE(   (dev_cfg_efs_v2->max_active_data >= 0)
                   && (dev_cfg_efs_v2->max_active_data <= POLICYMAN_NUM_SUBS) );

      pEfsDevCfg->max_active_data = dev_cfg_efs_v2->max_active_data;
    }

    if (3 == dev_cfg_efs->version)
    {
      device_config_efs_v3_t  *dev_cfg_efs_v3 = (device_config_efs_v3_t *)dev_cfg_efs;

      EFS_VALIDATE(   (dev_cfg_efs_v3->max_active_data >= 0)
                   && (dev_cfg_efs_v3->max_active_data <= POLICYMAN_NUM_SUBS) );

      pEfsDevCfg->max_active_data = dev_cfg_efs_v3->max_active_data;
      pEfsDevCfg->explicit_index  = dev_cfg_efs_v3->explicit_index;
    }

    if (4 == dev_cfg_efs->version)
    {
      EFS_VALIDATE(   (dev_cfg_efs->max_active_data >= 0)
                   && (dev_cfg_efs->max_active_data <= POLICYMAN_NUM_SUBS) );

      pEfsDevCfg->max_active_data = dev_cfg_efs->max_active_data;
      pEfsDevCfg->explicit_index  = dev_cfg_efs->explicit_index;
    }
  }

  POLICYMAN_MSG_HIGH_3( "device_config_validate_efs: config_is_valid %d, num_tests %d, validities 0x%04x",
                         config_is_valid,
                         num_tests,
                         validities );

  return config_is_valid;
}

/*-------- policyman_retrieve_device_config --------*/
STATIC policyman_status_t policyman_retrieve_device_config(
  device_context_t   *pEfsDevCfg
)
{
  mre_efs_status_t        efs_status      = MRE_EFS_STATUS_ERROR;
  device_config_efs_v4_t *dev_cfg_efs_ptr = NULL;
  size_t                  dev_cfg_size    = 0;
  policyman_status_t      status          = POLICYMAN_STATUS_SUCCESS;

  efs_status = mre_efs_get_file( POLICYMAN_DEVICE_CONFIG_NV_PATH,
                                 (void **)&dev_cfg_efs_ptr,
                                 &dev_cfg_size,
                                 SYS_MODEM_AS_ID_NONE );

  if (MRE_EFS_STATUS_SUCCESS != efs_status)
  {
    POLICYMAN_MSG_HIGH_2( "No persisted device config from EFS; efs_status %d, buffer 0x%08x",
                           efs_status,
                           dev_cfg_efs_ptr );

    status = POLICYMAN_STATUS_ERR;
    goto Done;
  }

  if ((dev_cfg_efs_ptr->version & POLICYMAN_DEVICE_CONFIG_VERSION_TAG) != POLICYMAN_DEVICE_CONFIG_VERSION_TAG)
  {
    // Version marker not present
    POLICYMAN_MSG_ERROR_2( "Invalid device config from EFS; size %d, vers 0x%08x",
                           dev_cfg_size,
                           dev_cfg_efs_ptr->version );

    efs_status = MRE_EFS_STATUS_ERROR;
    status     = POLICYMAN_STATUS_ERR_INVALID_VERSION;
    goto Done;
  }

  dev_cfg_efs_ptr->version &= POLICYMAN_DEVICE_CONFIG_VERSION_MASK; // Remove tag and leave version

  if (!policyman_device_config_validate_efs_config(dev_cfg_efs_ptr, pEfsDevCfg))
  {
    POLICYMAN_MSG_ERROR_1( "Invalid device version from EFS; vers 0x%08x", dev_cfg_efs_ptr->version);

    efs_status = MRE_EFS_STATUS_ERROR;
    status = POLICYMAN_STATUS_ERR_INVALID_VERSION;
    goto Done;
  }

  POLICYMAN_MSG_HIGH_5( "---------- Device Config from EFS(v%d): nSim %d, maxActiveVoice %d, maxActiveData %d, explicit index %d",
                        dev_cfg_efs_ptr->version,
                        pEfsDevCfg->num_sims,
                        pEfsDevCfg->max_active_voice,
                        pEfsDevCfg->max_active_data,
                        pEfsDevCfg->explicit_index );

  POLICYMAN_MSG_SEPARATOR(POLICYMAN_SEPARATOR_HYPHEN);

  efs_status = MRE_EFS_STATUS_SUCCESS;

Done:
  POLICYMAN_MEM_FREE_IF(dev_cfg_efs_ptr);
  return status;
}

/*-------- policyman_device_config_get_default_from_efs --------*/
policyman_status_t policyman_device_config_get_default_from_efs(
  cfgitem_execute_ctx_t *pCfgCtx
)
{
  device_configuration_t  *pDcItem     = NULL;
  device_context_t         efsDevCfg;
  policyman_status_t       status;


  efsDevCfg.num_sims         = 0;
  efsDevCfg.max_active_voice = 0;
  efsDevCfg.max_active_data  = 0;
  efsDevCfg.explicit_index   = -1;

  status = policyman_retrieve_device_config(&efsDevCfg);
  if (POLICYMAN_SUCCEEDED(status))
  {
    pDcItem = policyman_device_config_item_new(pCfgCtx->subsId);

    pDcItem->num_sims         = efsDevCfg.num_sims;
    pDcItem->max_active_voice = efsDevCfg.max_active_voice;
    pDcItem->max_active_data  = efsDevCfg.max_active_data;
    pDcItem->modified         = FALSE;
    pDcItem->explicit_index   = efsDevCfg.explicit_index;

    // Since we have got a valid config, update the static value

    g_num_Sim = efsDevCfg.num_sims;

    /*  Don't update any config here. They will be created later from XML or default function */
    pDcItem->hardware_mask = policyman_rf_get_overall_rat_mask();
    pDcItem->num_cfgs      = 0;
    pDcItem->pConfigs      = NULL;
    pDcItem->e911_num_cfgs = 0;
    pDcItem->pECallRatMask = NULL;

    pCfgCtx->pItem = (policyman_item_t *)pDcItem;
  }

  return status;
}


/*-------- policyman_device_config_compare --------*/
boolean policyman_device_config_compare(
  policyman_item_t  *pData1,
  policyman_item_t  *pData2
  )
{
  device_configuration_t  *pDcNewItem = (device_configuration_t *) pData1;
  device_configuration_t  *pDcOldItem = (device_configuration_t *) pData2;
  size_t                  nCfgBytes;

  if (
        pDcNewItem == NULL
     || pDcOldItem == NULL
     || pDcNewItem->num_sims            != pDcOldItem->num_sims
     || pDcNewItem->num_cfgs            != pDcOldItem->num_cfgs
     || pDcNewItem->max_active_voice    != pDcOldItem->max_active_voice
     || pDcNewItem->max_active_data     != pDcOldItem->max_active_data
     || pDcNewItem->current_cfg_index   != pDcOldItem->current_cfg_index
     || pDcNewItem->explicit_index      != pDcOldItem->explicit_index
     || pDcNewItem->forceUpdate
     )
  {
    return FALSE;
  }

  nCfgBytes = pDcNewItem->num_sims * pDcNewItem->num_cfgs * sizeof(uint32);

  return memcmp(pDcNewItem->pConfigs, pDcOldItem->pConfigs, nCfgBytes) == 0;
}

/*-------- policyman_device_config_display --------*/
void policyman_device_config_display(
  policyman_item_t  *pItem
)
{
  device_configuration_t  *pDcItem = (device_configuration_t *) pItem;
  sys_overall_feature_t    feature;
  sys_subs_feature_t       subsFeature = SYS_SUBS_FEATURE_MODE_NORMAL;
  size_t                   nSim;
  uint32                   ratCap;

  policyman_device_config_overall_feature(pItem, &feature);

  POLICYMAN_MSG_HIGH_9( " Device configuration: nSim %d, maxActiveVoice %d, maxActiveData %d, nCfgs %d, nECallCfgs %d, hardware_mask 0x%04x\t"
                "overall feature %d, CfgIndex %d, index modem controlled %d",
                        pDcItem->num_sims,
                        pDcItem->max_active_voice,
                        pDcItem->max_active_data,
                        pDcItem->num_cfgs,
                        pDcItem->e911_num_cfgs,
                        pDcItem->hardware_mask,
                       feature,
                       pDcItem->current_cfg_index,
                       pDcItem->explicit_index );

  for ( nSim = 0;
       (pDcItem->num_cfgs > 0) && nSim < pDcItem->num_sims;
        nSim++
      )
  {
    (void)policyman_device_config_get_current_subphone_capability(pItem, nSim, &ratCap);
    subsFeature = device_config_get_subs_feature(pDcItem, 0, nSim);

    POLICYMAN_MSG_HIGH_3( "  subphone[%d] feature %d, ratMask 0x%04x",
                          nSim,
                          subsFeature,
                          ratCap );
  }
}

/*-------- policyman_device_config_get_default_sims --------*/
void policyman_device_config_get_default_sims(
  size_t  *pnSims,
  size_t  *pnActiveVoice,
  size_t  *pnActiveData
)
{
  mre_efs_status_t               status;
  void                          *pData       = NULL;
  size_t                         size;
  sys_modem_device_mode_e_type   device_mode = SYS_MODEM_DEVICE_MODE_SINGLE_SIM;

#ifdef FEATURE_DUAL_SIM
  //  In test mode and limited mode we only work as single-SIM
  if (  !policyman_test_mode_is_enabled()
     && !policyman_cfgitem_limited_capabilities_is_enabled()
     )
  {
    status = mre_efs_get_file( DEVICE_MODE_NV_FILE,
                               &pData,
                               &size,
                               SYS_MODEM_AS_ID_NONE );

    if (  (status == MRE_EFS_STATUS_SUCCESS)
       && (size == sizeof(sys_modem_device_mode_e_type)) )
    {
      device_mode = *((sys_modem_device_mode_e_type *) pData);
      POLICYMAN_MEM_FREE_IF(pData);
      POLICYMAN_MSG_HIGH_1("Successful EFS read of device_mode NV; device_mode %d", device_mode);
    }
  }
#endif

  *pnActiveData = 1 ;

  switch (device_mode)
  {
    case SYS_MODEM_DEVICE_MODE_SINGLE_SIM:
      *pnSims = 1;
      *pnActiveVoice = 1;
      break;

    case SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_STANDBY:
      *pnSims = 2;
      *pnActiveVoice = 1;
      break;

    case SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE:
      *pnSims = 2;
      *pnActiveVoice = 2;
      break;

    case SYS_MODEM_DEVICE_MODE_TRIPLE_SIM_TRIPLE_STANDBY:
      *pnSims = 3;
      *pnActiveVoice = 1;
      break;

    default:
      POLICYMAN_MSG_ERROR_1("Unknown device mode from NV: %d, setting default device mode SINGLE_SIM", device_mode);
      *pnSims = 1;
      *pnActiveVoice = 1;
      break;
  }
}


/*-------- policyman_device_config_get_default --------*/
policyman_item_t* policyman_device_config_get_default(
  mre_set_t              *pItemSet,
  sys_modem_as_id_e_type  subsId
)
{
  device_configuration_t      *pDcItem            = NULL;
  size_t                       nSims          = 1;
  size_t                       maxActiveVoice = 1;
  size_t                       maxActiveData  = 1;
  size_t                       idx;


  if (  !policyman_test_mode_is_enabled()
     && !policyman_cfgitem_limited_capabilities_is_enabled()
     )
  {
    policyman_device_config_get_default_sims(&nSims, &maxActiveVoice, &maxActiveData);
  }

  pDcItem           = policyman_device_config_item_new(subsId);

  pDcItem->num_sims         = nSims;
  pDcItem->max_active_voice = maxActiveVoice;
  pDcItem->max_active_data  = maxActiveData;
  pDcItem->explicit_index   = -1;
  pDcItem->hardware_mask    = policyman_rf_get_overall_rat_mask();
  pDcItem->readFromDefault  = TRUE;

  /* Determine the maximum active voice subs ever possible */
  if (maxActiveVoice > g_maxActiveVoiceSubs)
  {
    g_maxActiveVoiceSubs = maxActiveVoice;
  }

  pDcItem->num_cfgs          = 1;
  pDcItem->pConfigs          = policyman_mem_alloc(sizeof(subs_info_t) * nSims);
  pDcItem->current_cfg_index = 0;
  pDcItem->e911_num_cfgs     = 1;
  pDcItem->pECallRatMask     = (uint32 *)policyman_mem_alloc((sizeof(uint32) * nSims));

  /* Since we have got a valid config, update the static value
   */
  g_num_Sim = nSims;
  
  pDcItem->pConfigs[0].feature = SYS_SUBS_FEATURE_MODE_NORMAL;
  
  if (nSims == 2)
  {
    pDcItem->pConfigs[0].feature = SYS_SUBS_FEATURE_MODE_DUAL_MULTIMODE;
    pDcItem->max_active_data     = nSims;
  }
  
  pDcItem->pConfigs[0].ratInfo = pDcItem->hardware_mask;
  pDcItem->pECallRatMask[0]    = pDcItem->hardware_mask;

  for (idx = 1 ; idx < nSims ; ++idx)
  {
    uint32 excludeMask = SYS_SYS_MODE_MASK_HDR | SYS_SYS_MODE_MASK_CDMA;

    pDcItem->pConfigs[idx].feature = SYS_SUBS_FEATURE_MODE_DUAL_MULTIMODE;
    pDcItem->pConfigs[idx].ratInfo = pDcItem->hardware_mask & ~(excludeMask);
    pDcItem->pECallRatMask[idx]    = pDcItem->hardware_mask & ~(excludeMask);
  }

  POLICYMAN_MSG_HIGH_1(SUBS_PREFIX "Default device configuration created", pDcItem->asubs_id );

  return (policyman_item_t *)pDcItem;
}

/*-------- device_config_get_subphone_index --------*/
STATIC size_t device_config_get_subphone_index(
  size_t  num_sims,
  size_t  cfgIdx,
  size_t  simIdx
)
{
  return  (num_sims * cfgIdx) + simIdx;
}

/*-------- device_config_get_base_config --------*/
uint32 device_config_get_base_config(
  device_configuration_t const  *pDcItem,
  size_t                         cfgIdx,
  size_t                         simIdx
)
{
  size_t  index;

  index  = device_config_get_subphone_index(pDcItem->num_sims, cfgIdx, simIdx);

  return ( cfgIdx >= pDcItem->num_cfgs || simIdx > pDcItem->num_sims) ? 0
         : pDcItem->pConfigs[index].ratInfo;
}

/*-------- device_config_get_config --------*/
uint32 device_config_get_config(
  device_configuration_t const  *pDcItem,
  size_t                         cfgIdx,
  size_t                         simIdx
)
{
  uint32 ratInfo;

  ratInfo = device_config_get_base_config( pDcItem, cfgIdx, simIdx);

  return ratInfo & pDcItem->hardware_mask;
}

/*-------- device_config_get_current_config --------*/
uint32 device_config_get_current_config(
  device_configuration_t const  *pDcItem,
  size_t                         simIdx
)
{
  return device_config_get_config(pDcItem, pDcItem->current_cfg_index, simIdx);
}

/*-------- device_config_get_subs_feature --------*/
sys_subs_feature_t device_config_get_subs_feature(
  device_configuration_t const  *pDcItem,
  size_t                         cfgIdx,
  size_t                         simIdx
)
{
  size_t             index;
  sys_subs_feature_t subsFeature;

  index       = device_config_get_subphone_index(pDcItem->num_sims, cfgIdx, simIdx);
  subsFeature = pDcItem->pConfigs[index].feature;

  return subsFeature;
}

/*-------- policyman_device_config_check_parameters --------*/
STATIC policyman_status_t policyman_device_config_check_parameters(
  device_configuration_t  *pItem,
  void                    *pParam
)
{
  policyman_status_t status = POLICYMAN_STATUS_SUCCESS;

  if (pItem == NULL || pParam == NULL)
  {
    POLICYMAN_MSG_ERROR_0("Invalid parameter passed");
    status = POLICYMAN_STATUS_ERR_INVALID_ARGS;
    goto Done;
  }

  if (policyman_cfgitem_id((policyman_item_t const *)pItem) != POLICYMAN_ITEM_DEVICE_CONFIGURATION)
  {
    POLICYMAN_MSG_ERROR_0("Wrong policy item passed");
    status = POLICYMAN_STATUS_ERR_INVALID_ITEM_ID;
  }

Done:
  return status;
}

/*-------- policyman_device_config_num_sims --------*/
policyman_status_t policyman_device_config_num_sims(
  policyman_item_t const  *pItem,
  size_t                  *pnSims
)
{
  device_configuration_t  *pDcItem = (device_configuration_t *) pItem;
  policyman_status_t       status;

  status = policyman_device_config_check_parameters(pDcItem, pnSims);
  if (POLICYMAN_SUCCEEDED(status))
  {
    *pnSims = pDcItem->num_sims;
  }

  return status;
}


/*-------- policyman_device_config_max_active_voice --------*/
policyman_status_t policyman_device_config_max_active_voice(
  policyman_item_t const  *pItem,
  size_t                  *pnMaxActive
)
{
  device_configuration_t  *pDcItem = (device_configuration_t *) pItem;
  policyman_status_t       status;

  status = policyman_device_config_check_parameters(pDcItem, pnMaxActive);
  if (POLICYMAN_SUCCEEDED(status))
  {
    *pnMaxActive = pDcItem->max_active_voice ;
  }

  return status;
}

/*-------- policyman_device_config_max_active_data --------*/
policyman_status_t policyman_device_config_max_active_data(
  policyman_item_t const  *pItem,
  size_t                  *pnMaxActiveData
)
{
  device_configuration_t  *pDcItem = (device_configuration_t *) pItem;
  policyman_status_t       status;

  status = policyman_device_config_check_parameters(pDcItem, pnMaxActiveData);
  if (POLICYMAN_SUCCEEDED(status))
  {
    *pnMaxActiveData = pDcItem->max_active_data;
  }

  return status;
}


/*-------- policyman_device_overall_config_feature --------*/
policyman_status_t policyman_device_config_overall_feature(
  policyman_item_t const  *pItem,
  sys_overall_feature_t   *pFeature
)
{
  device_configuration_t  *pDcItem     = (device_configuration_t *) pItem;
  sys_subs_feature_t       subsFeature = SYS_SUBS_FEATURE_MODE_NORMAL;
  policyman_status_t       status;

  status = policyman_device_config_check_parameters(pDcItem, pFeature);
  if (POLICYMAN_FAILED(status))
  {
    return status;
  }

  // get subs feature only when we have a valid config
  if (pDcItem->num_cfgs > 0)
  {
    subsFeature = device_config_get_subs_feature(pDcItem, 0, 0);
  }

  if (pDcItem->num_sims > 1)
  {
    *pFeature = SYS_OVERALL_FEATURE_MODE_MULTISIM;
  }
  else if (subsFeature == SYS_SUBS_FEATURE_MODE_SRLTE)
  {
    *pFeature = SYS_OVERALL_FEATURE_MODE_SRLTE;
  }
  else
  {
    *pFeature = SYS_OVERALL_FEATURE_MODE_NORMAL;
  }

  return (pDcItem->modified & DEV_CONFIG_FEATURE_MODIFIED)?
          POLICYMAN_STATUS_WRN_MODIFIED : POLICYMAN_STATUS_SUCCESS;
}


/*-------- policyman_device_config_num_cfgs --------*/
policyman_status_t policyman_device_config_num_cfgs(
  policyman_item_t const  *pItem,
  size_t                  *pnCfgs
)
{
  device_configuration_t  *pDcItem = (device_configuration_t *) pItem;
  policyman_status_t       status;

  status = policyman_device_config_check_parameters(pDcItem, pnCfgs);
  if (POLICYMAN_SUCCEEDED(status))
  {
    *pnCfgs = pDcItem->num_cfgs;
  }

  return status;
}

/*-------- policyman_device_config_get_config --------*/
policyman_status_t policyman_device_config_get_config(
  policyman_item_t const  *pItem,
  size_t                  nCfg,
  size_t                  nSim,
  uint32                  *pRatCfg
)
{
  device_configuration_t  *pDcItem = (device_configuration_t *) pItem;
  policyman_status_t       status;

  status = policyman_device_config_check_parameters(pDcItem, pRatCfg);
  if (  POLICYMAN_SUCCEEDED(status)
     && nCfg < pDcItem->num_cfgs
     && nSim < pDcItem->num_sims
     )
  {
    *pRatCfg = device_config_get_config(pDcItem, nCfg, nSim);
  }

  return status;
}


/*-------- policyman_device_config_get_subs_feature --------*/
policyman_status_t policyman_device_config_get_subs_feature(
  policyman_item_t const  *pItem,
  size_t                  nCfg,
  size_t                  nSim,
  sys_subs_feature_t      *pFeature
)
{
  device_configuration_t  *pDcItem = (device_configuration_t *) pItem;
  policyman_status_t       status;

  status = policyman_device_config_check_parameters(pDcItem, pFeature);
  if (  POLICYMAN_SUCCEEDED(status)
     && nCfg < pDcItem->num_cfgs
     && nSim < pDcItem->num_sims
     )
  {
    *pFeature = device_config_get_subs_feature(pDcItem, nCfg, nSim);
  }

  return status;
}


/*-------- policyman_get_current_device_config --------*/
policyman_status_t policyman_get_current_device_config(
  policyman_item_t **pDcItem
)
{
  policyman_item_id_t  id     = POLICYMAN_ITEM_DEVICE_CONFIGURATION;
  policyman_status_t   status;

  status = policyman_get_items( (policyman_item_id_t const *)&id,
                                1,
                                (policyman_item_t const **)pDcItem );

  return status;
}

/*-------- policyman_get_current_num_sim --------*/
QSH_MDUMP_FN_ATTR_PM policyman_status_t policyman_get_current_num_sim(
  size_t *pnSim
)
{
  device_configuration_t   *pDcItem = NULL;
  policyman_status_t        status = POLICYMAN_STATUS_SUCCESS;

  /* Return nSIM if it has been read once
       The value doesn't change in run time
   */
  if (g_num_Sim > 0)
  {
    *pnSim = g_num_Sim;
    return status;
  }

  status = policyman_get_current_device_config((policyman_item_t **)&pDcItem);

  if (status == POLICYMAN_STATUS_SUCCESS)
  {
    *pnSim = pDcItem->num_sims;

    /* Update nSim as well if we have got info correctly
      */
      g_num_Sim = *pnSim;
    }

  MRE_RELEASE_IF(pDcItem);
  return status;
}

/*-------- policyman_get_max_capability_for_all_configs --------*/
STATIC void policyman_get_max_capability_for_all_configs(
  device_configuration_t  *pDcItem,
  size_t                   nSim,
  uint32                  *pRatCfg
)
{
  size_t   nCfg;
  boolean  include1x;

  *pRatCfg = 0;
  for (nCfg = 0 ; nCfg < pDcItem->num_cfgs ; ++nCfg)
  {
    *pRatCfg |= device_config_get_config(pDcItem, nCfg, nSim);
  }

  // for primary card add CH in max cap if allowed
  include1x = policyman_get_subs_boolean_with_default("pm:include_1x_for_hlos", SYS_MODEM_AS_ID_1, FALSE);

  if (nSim == 0 && include1x)
  {
    *pRatCfg |= (SYS_SYS_MODE_MASK_CDMA | SYS_SYS_MODE_MASK_HDR);
  }
}

/*-------- policyman_device_config_get_max_capability --------*/
policyman_status_t policyman_device_config_get_max_capability(
  policyman_item_t const  *pItem,
  size_t                   nSim,
  uint32                  *pRatCfg
)
{
  device_configuration_t  *pDcItem = (device_configuration_t *)pItem;
  policyman_status_t       status;

  status = policyman_device_config_check_parameters(pDcItem, pRatCfg);
  if (POLICYMAN_FAILED(status))
  {
    return status;
  }

  if (nSim >= pDcItem->num_sims)
  {
    POLICYMAN_MSG_ERROR_2( "requested config for nSim = %d while device supports %d cards",
                           nSim,
                           pDcItem->num_sims );

    return POLICYMAN_STATUS_ERR_INVALID_ARGS;
  }

  policyman_get_max_capability_for_all_configs(pDcItem, nSim, pRatCfg);
  return POLICYMAN_STATUS_SUCCESS;
}

/*-------- policyman_device_config_get_current_subphone_capability --------*/
policyman_status_t policyman_device_config_get_current_subphone_capability(
  policyman_item_t const  *pItem,
  size_t                   nSim,
  uint32                  *pRatCfg
)
{
  device_configuration_t  *pDcItem  = (device_configuration_t *)pItem;
  policyman_status_t       status;

  status = policyman_device_config_check_parameters(pDcItem, pRatCfg);
  if (POLICYMAN_FAILED(status))
  {
    return status;
  }

  if (nSim >= pDcItem->num_sims)
  {
    POLICYMAN_MSG_ERROR_2( "requested config for nSim = %d while device supports %d cards",
                           nSim,
                           pDcItem->num_sims );

    return POLICYMAN_STATUS_ERR_INVALID_ARGS;
  }

  *pRatCfg = device_config_get_current_config(pDcItem, nSim);
  return POLICYMAN_STATUS_SUCCESS;
}

/*-------- get_e911_subphone_capability --------*/
policyman_status_t  get_e911_subphone_capability(
  device_configuration_t  *pdc,
  size_t                   numItems,
  uint32                  *pRatArray
  )
{
  boolean             primarySubsActive;
  boolean             secondarySubsActive;
  size_t                   nSim;

  policyman_state_t  *pState  = policyman_state_get_state();
  policyman_status_t  status  = POLICYMAN_STATUS_ERR;

    primarySubsActive   = policyman_is_cm_subs_active(pState, SYS_MODEM_AS_ID_1);
    secondarySubsActive = policyman_is_cm_subs_active(pState, SYS_MODEM_AS_ID_2);

  POLICYMAN_MSG_HIGH_2( "e911_subphone_capability active subs: "
                        "primary %d, secondary %d",
                          primarySubsActive,
                          secondarySubsActive );


    // return error if both subs active. current subphone cap is the correct one.
  if (!primarySubsActive || !secondarySubsActive)
    {
    for (nSim = 0; nSim < numItems; nSim++)
    {
      size_t nSubs = nSim;

      if (secondarySubsActive)
      {
        nSubs += numItems;
      }

      pRatArray[nSim] = pdc->pECallRatMask[nSubs] & pdc->hardware_mask;
    }
    status = POLICYMAN_STATUS_SUCCESS;
  }

  return status;
}

/*-------- validate_e911_parameters --------*/
STATIC policyman_status_t  validate_e911_parameters(
  device_configuration_t  *pdc,
  size_t                   numItems,
  uint32                  *pRatArray
)
{
  policyman_status_t  status = POLICYMAN_STATUS_ERR_INVALID_ARGS;

  if (numItems <= pdc->num_sims)
  {
    status = policyman_device_config_check_parameters(pdc, pRatArray);
  }

  return status;
}

/*-------- copy_e911_config_from_current_config --------*/
STATIC void copy_e911_config_from_current_config(
  device_configuration_t  *pdc,
  size_t                   numItems,
  uint32                  *pRatArray
)
{
  size_t  nSim;

  for (nSim = 0; nSim < numItems; nSim++)
  {
    pRatArray[nSim] = device_config_get_current_config(pdc, nSim);
  }
}

/*-------- print_e911_configs --------*/
STATIC void print_e911_configs(
  size_t                   numItems,
  uint32                  *pRatArray
)
{
  size_t  nSim;

  for (nSim = 0; nSim < numItems; nSim++)
  {
    POLICYMAN_MSG_HIGH_3( "e911_subphone_capability pRatArray[idx %d of %d] mask = 0x%04x",
                          nSim,
                          numItems,
                          pRatArray[nSim] );
  }
}

/*-------- policyman_get_e911_subphone_capability --------*/
policyman_status_t policyman_get_e911_subphone_capability(
  policyman_item_t const  *pItem,
  size_t                   numItems,
  uint32                  *pRatArray
)
{
  policyman_status_t       status;
  device_configuration_t  *pdc = (device_configuration_t *) pItem;

  status = validate_e911_parameters(pdc, numItems, pRatArray);
  if (POLICYMAN_FAILED(status))
  {
    return status;
  }

  if (pdc->e911_num_cfgs < 2)
  {
    copy_e911_config_from_current_config(pdc, numItems, pRatArray);
  }
  else if (pdc->num_sims > 1 && pdc->pECallRatMask != NULL)
  {
    status = get_e911_subphone_capability(pdc, numItems, pRatArray);
  }

  print_e911_configs(numItems, pRatArray);

  return status;
}

/*-------- policyman_device_config_get_current_config_index --------*/
policyman_status_t policyman_device_config_get_current_config_index(
  policyman_item_t const  *pItem,
  size_t                  *pIndex
)
{
  device_configuration_t  *pDcItem = (device_configuration_t *)pItem;
  policyman_status_t       status;

  status = policyman_device_config_check_parameters(pDcItem, pIndex);
  if (POLICYMAN_SUCCEEDED(status))
  {
    *pIndex = pDcItem->current_cfg_index;
  }

  return status;
}

/*-------- policyman_device_config_max_active_subs --------*/
policyman_status_t policyman_device_config_max_active_subs(
  policyman_item_t const  *pItem,
  size_t                  *pnMaxActive
)
{
  device_configuration_t  *pDcItem = (device_configuration_t *)pItem;
  policyman_status_t       status;

  status = policyman_device_config_check_parameters(pDcItem, pnMaxActive);
  if (POLICYMAN_SUCCEEDED(status))
  {
    *pnMaxActive = g_maxActiveVoiceSubs;
  }

  return status;
}

/*-------- policyman_check_and_update_configurations --------*/
STATIC void policyman_check_and_update_configurations(
  device_configuration_t *pDcItem
)
{
  size_t   nSim;

  pDcItem->hardware_mask = policyman_rf_get_overall_rat_mask();

  if (pDcItem->num_cfgs == 0)
  {
    POLICYMAN_MSG_HIGH_0("no configuration specified, adding <CHGWLT+GWLT>");

    /* single config is supported when not defined by XML */
    pDcItem->num_cfgs          = 1;
    pDcItem->current_cfg_index = 0;
    pDcItem->explicit_index    = -1;
    pDcItem->pConfigs = (subs_info_t *)policyman_mem_alloc((sizeof(subs_info_t) * pDcItem->num_sims));

    pDcItem->pConfigs[0].ratInfo = pDcItem->hardware_mask;

    for (nSim = 1; nSim < pDcItem->num_sims; nSim++)
    {
      uint32 excludeMask = SYS_SYS_MODE_MASK_HDR | SYS_SYS_MODE_MASK_CDMA;
      pDcItem->pConfigs[nSim].ratInfo = pDcItem->hardware_mask & ~(excludeMask);
    }
  }

  /*Create and Fill in the e911 config as well if not created */
  if (pDcItem->e911_num_cfgs == 0)
  {
    POLICYMAN_MSG_HIGH_0("no e911 configuration specified, adding <CHGWLT+GWLT>");

  /* single e911 config is supported when not defined by XML */
    pDcItem->e911_num_cfgs = 1;
    pDcItem->pECallRatMask = (uint32 *)policyman_mem_alloc((sizeof(uint32) * pDcItem->num_sims));

    for (nSim = 0; nSim < pDcItem->num_sims; nSim++)
    {
      pDcItem->pECallRatMask[nSim] = device_config_get_current_config(pDcItem, nSim);
    }
  }
}

/*-------- policyman_ensure_configurations --------*/
void policyman_ensure_configurations(
  mre_set_t       *pItemSet
)
{
  device_configuration_t  *pDcItem           = NULL;
  boolean                  item_is_present;

  item_is_present = policyman_cfgitem_is_present_in_itemset( pItemSet,
                                                             POLICYMAN_ITEM_DEVICE_CONFIGURATION,
                                                             SYS_MODEM_AS_ID_1,
                                                             (policyman_item_t  **)&pDcItem);

  if (item_is_present)
  {
    policyman_check_and_update_configurations(pDcItem);
    ref_cnt_obj_release(pDcItem);
  }
  else
  {
    POLICYMAN_MSG_ERROR_0("failed to get device config item");
  }
}

/*-------- device_config_copy_configs --------*/
STATIC void device_config_copy_configs(
  device_configuration_t const *pCurrentDcItem,
  device_configuration_t       *pDcItem
)
{
  /* Allocate memory for subphone configs if that is NULL */
  if (pDcItem->num_cfgs == 0 && pCurrentDcItem->num_cfgs > 0)
  {
    size_t sizeConfigs;

    pDcItem->num_cfgs = pCurrentDcItem->num_cfgs;
    sizeConfigs   = pCurrentDcItem->num_sims * pCurrentDcItem->num_cfgs * sizeof(subs_info_t);

    pDcItem->pConfigs = (subs_info_t *)policyman_mem_alloc(sizeConfigs);
    memscpy(pDcItem->pConfigs, sizeConfigs, pCurrentDcItem->pConfigs, sizeConfigs);
  }

  /* Allocate memory for e911 configs if that is NULL */
  if (pDcItem->e911_num_cfgs == 0 && pCurrentDcItem->e911_num_cfgs > 0)
  {
    size_t sizeE911Configs;

    pDcItem->e911_num_cfgs = pCurrentDcItem->e911_num_cfgs;
    sizeE911Configs    = pCurrentDcItem->num_sims * pCurrentDcItem->e911_num_cfgs * sizeof(uint32);

    pDcItem->pECallRatMask  = (uint32 *)policyman_mem_alloc(sizeE911Configs);
    memscpy(pDcItem->pECallRatMask, sizeE911Configs, pCurrentDcItem->pECallRatMask, sizeE911Configs);
  }
}

/*-------- device_config_copy_parameters --------*/
STATIC void device_config_copy_parameters(
  device_configuration_t const *pCurrentDcItem,
  device_configuration_t       *pDcItem
)
{
  pDcItem->num_sims          = pCurrentDcItem->num_sims;
  pDcItem->max_active_voice  = pCurrentDcItem->max_active_voice;
  pDcItem->max_active_data   = pCurrentDcItem->max_active_data;
  pDcItem->current_cfg_index = pCurrentDcItem->current_cfg_index;
  pDcItem->explicit_index    = pCurrentDcItem->explicit_index;
  pDcItem->hardware_mask     = pCurrentDcItem->hardware_mask;
  pDcItem->readFromDefault   = pCurrentDcItem->readFromDefault;

  device_config_copy_configs(pCurrentDcItem, pDcItem);
}

/*-------- device_config_get_num_configs --------*/
STATIC size_t device_config_get_num_configs(
  void
)
{
  device_configuration_t  *pDcItem = NULL;
  size_t                   numCfgs = 0;
  policyman_status_t       status;

  status = policyman_get_current_device_config((policyman_item_t **)&pDcItem);
  if (POLICYMAN_SUCCEEDED(status))
  {
    numCfgs = pDcItem->num_cfgs;
  }
  MRE_RELEASE_IF(pDcItem);

  return numCfgs;
}


/*-------- policyman_device_config_item_clone --------*/
device_configuration_t *policyman_device_config_item_clone(
  device_configuration_t  *pDcItem
)
{
  device_configuration_t *pNew;

  pNew = policyman_device_config_item_new(SYS_MODEM_AS_ID_1);
  device_config_copy_parameters(pDcItem, pNew);
  return pNew;
}


/*-------- policyman_device_config_clone --------*/
policyman_item_t * policyman_device_config_clone(
  policyman_item_t const *pItem
)
{
  device_configuration_t *pDvcItem = (device_configuration_t *)pItem;

  return (policyman_item_t *)policyman_device_config_item_clone(pDvcItem);
}


/*-------- sub_has_1x_in_user_mode_pref --------*/
STATIC boolean sub_has_1x_in_user_mode_pref (
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId
)
{
  uint32  subsModePref;
  boolean subHasCdma;

  subsModePref = policyman_ph_get_mode_pref_mask(pState, subsId);

  return ((subsModePref & SYS_SYS_MODE_MASK_CDMA) != 0);
}


/*-------- cdma_should_move_based_on_dds --------*/
STATIC boolean cdma_should_move_based_on_dds(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  currentCdmaSubs,
  sys_modem_as_id_e_type  otherSubs
)
{
  boolean bothSubsHave1xInModePref;
  boolean neitherSubsHas1xInModePref;
  boolean otherSubsIsTheDdsSubs;

  bothSubsHave1xInModePref = sub_has_1x_in_user_mode_pref(pState, currentCdmaSubs)
                             && sub_has_1x_in_user_mode_pref(pState, otherSubs);

  neitherSubsHas1xInModePref = !sub_has_1x_in_user_mode_pref(pState, currentCdmaSubs)
                               && !sub_has_1x_in_user_mode_pref(pState, otherSubs);

  otherSubsIsTheDdsSubs = policyman_subs_is_dds_subs(pState, otherSubs);

  return (bothSubsHave1xInModePref || neitherSubsHas1xInModePref) && otherSubsIsTheDdsSubs;
}


/*-------- cdma_should_move_based_on_mode_pref --------*/
STATIC boolean cdma_should_move_based_on_mode_pref(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  currentCdmaSubs,
  sys_modem_as_id_e_type  otherSubs
)
{
  return !sub_has_1x_in_user_mode_pref(pState, currentCdmaSubs)
         && sub_has_1x_in_user_mode_pref(pState, otherSubs);
}

/*-------- cdma_is_present_in_subphone --------*/
STATIC boolean cdma_is_present_in_subphone(
  policyman_state_t       *pState,
  device_configuration_t  *pCurrentDcItem,
  sys_modem_as_id_e_type   subsId
)
{
  boolean cdmaIsPresent = FALSE;

  if (NULL != pCurrentDcItem)
  {
    uint32 ratMask;

    ratMask = device_config_get_current_config(pCurrentDcItem, subsId);

    cdmaIsPresent = ((ratMask & SYS_SYS_MODE_MASK_CDMA) != 0);
  }

  return cdmaIsPresent;
}


/*-------- get_current_cdma_subs --------*/
STATIC sys_modem_as_id_e_type get_current_cdma_subs (
  policyman_state_t       *pState,
  device_configuration_t  *pDcItem
)
{
  size_t                 nSim;
  size_t                 subsIdx;
  sys_modem_as_id_e_type cdmaSubs = SYS_MODEM_AS_ID_1;

  if (POLICYMAN_SUCCEEDED(policyman_get_current_num_sim(&nSim)))
  {
    for (subsIdx = SYS_MODEM_AS_ID_1; subsIdx < nSim; subsIdx++)
    {
      if (cdma_is_present_in_subphone(pState, pDcItem, subsIdx))
      {
        cdmaSubs = (sys_modem_as_id_e_type)subsIdx;
        break;
      }
    }
  }

  return cdmaSubs;
}


/*-------- cdma_is_present_on_card --------*/
STATIC boolean cdma_is_present_on_card(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId
)
{
  boolean cdmaIsFromCard;
  boolean cdmaIsFromSim;
  boolean cdmaIsFromNV;

  cdmaIsFromCard = policyman_uim_sim_is_3gpp2(pState, subsId, &cdmaIsFromSim, &cdmaIsFromNV);

  POLICYMAN_MSG_HIGH_3( SUBS_PREFIX "CDMA is from card (sim %d, NV %d)",
                        subsId,
                        cdmaIsFromSim,
                        cdmaIsFromNV );

  return cdmaIsFromCard;
}


/*-------- cdma_is_present_on_all_cards --------*/
STATIC boolean cdma_is_present_on_all_cards(
  policyman_state_t *pState
)
{
  boolean            cdmaIsOnAllSubs = FALSE;
  size_t             nSim            = 1;
  size_t             subsId;
  policyman_status_t status;

  status = policyman_get_current_num_sim(&nSim);

  if (POLICYMAN_SUCCEEDED(status))
  {
    cdmaIsOnAllSubs = TRUE;

    for (subsId = SYS_MODEM_AS_ID_1;
        (cdmaIsOnAllSubs && subsId < nSim);
        subsId++)
    {
      cdmaIsOnAllSubs = cdma_is_present_on_card(pState, subsId);
    }
  }

  return cdmaIsOnAllSubs;
}


/*-------- policyman_determine_cdma_subs --------*/
STATIC sys_modem_as_id_e_type policyman_determine_cdma_subs(
  policyman_state_t      *pState,
  device_configuration_t *pDcItem
)
{
  sys_modem_as_id_e_type currentCdmaSubs;
  sys_modem_as_id_e_type otherSubs;

  currentCdmaSubs = get_current_cdma_subs(pState, pDcItem);
  otherSubs = OTHER_SUB(currentCdmaSubs);

  if (cdma_is_present_on_all_cards(pState))
  {
    if (cdma_should_move_based_on_mode_pref(pState, currentCdmaSubs, otherSubs))
    {
      POLICYMAN_MSG_HIGH_2( "CDMA moved from sub %d -> %d due to mode_pref",
                            currentCdmaSubs,
                            otherSubs );

      currentCdmaSubs = otherSubs;
    }
    else if (cdma_should_move_based_on_dds(pState, currentCdmaSubs, otherSubs))
    {
      POLICYMAN_MSG_HIGH_2( "CDMA moved from sub %d -> %d due to DDS",
                            currentCdmaSubs,
                            otherSubs );

      currentCdmaSubs = otherSubs;
    }
  }
  else if (cdma_is_present_on_card(pState, otherSubs))
  {
    POLICYMAN_MSG_HIGH_2( "CDMA moved from sub %d -> %d due to card",
                          currentCdmaSubs,
                          otherSubs );

    currentCdmaSubs = otherSubs;
  }

  return currentCdmaSubs;
}


#define NO_CFG 0xFFFF

/*-------- policyman_find_lte_1x_cfg_for_subs --------*/
STATIC size_t policyman_find_lte_1x_cfg_for_subs(
  device_configuration_t *pDcItem,
  sys_modem_as_id_e_type  ddsSubs,
  sys_modem_as_id_e_type  cdmaSubs
)
{
  size_t  nCfg;
  size_t  index     = 0;
  size_t  lteIndex  = NO_CFG;
  size_t  cdmaIndex = NO_CFG;
  uint32  ratMaskLte;
  uint32  ratMaskCdma;

  /* Attempt to find config that meets both L(dds) and C preference.  If none found, fall back to first
        LTE preference, then CDMA preference */
  for (nCfg = 0; nCfg < pDcItem->num_cfgs; nCfg++)
  {
    ratMaskLte  = device_config_get_base_config(pDcItem, nCfg, SUBS_ID_TO_NSIM(ddsSubs));
    ratMaskCdma = device_config_get_base_config(pDcItem, nCfg, SUBS_ID_TO_NSIM(cdmaSubs));

    if (   (ratMaskLte & SYS_SYS_MODE_MASK_LTE) != 0
        && (ratMaskCdma & SYS_SYS_MODE_MASK_CDMA) != 0)
    {
      index = nCfg;
      goto Done;
    }

    // Keep track of first config index that meets either LTE or CDMA requirement, in case both cannot be met
    if (   (ratMaskLte & SYS_SYS_MODE_MASK_LTE) != 0
        && (lteIndex == NO_CFG))
    {
      lteIndex = nCfg;
    }

    if (   (ratMaskCdma & SYS_SYS_MODE_MASK_CDMA) != 0
        && (cdmaIndex == NO_CFG))
    {
      cdmaIndex = nCfg;
    }
  }

  // Use config index that meets LTE or CDMA if both could not be met
  if (lteIndex != NO_CFG)
  {
    index = lteIndex;
  }
  else if (cdmaIndex != NO_CFG)
  {
    index = cdmaIndex;
  }

Done:
  return index;
}


/*-------- policyman_update_current_cfg_index --------*/
STATIC boolean policyman_update_current_cfg_index(
  device_configuration_t *pDcItem,
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  ddsSubs
)
{
  size_t  index;
  boolean index_updated = FALSE;

  /*  If index is explicitly set, no work to do - just return.
   */
  if (-1 != pDcItem->explicit_index)
  {
    index = (size_t) pDcItem->explicit_index;
  }
  else
  {
    sys_modem_as_id_e_type cdmaSubs;

    cdmaSubs = policyman_determine_cdma_subs(pState, pDcItem);
    index    = policyman_find_lte_1x_cfg_for_subs(pDcItem, ddsSubs, cdmaSubs);
  }

  if (index != pDcItem->current_cfg_index)
  {
    POLICYMAN_MSG_HIGH_3( "configuration index: old %d, new %d, explicit %d",
                          pDcItem->current_cfg_index,
                          index,
                          pDcItem->explicit_index );

    pDcItem->current_cfg_index = index;
    index_updated = TRUE;
  }

  return index_updated;
}

/*-------- set_explicit_index --------*/
STATIC void set_explicit_index(
  policyman_state_t       *pState,
  mre_set_t               *pItemSet,
  sys_modem_as_id_e_type   ddsSubs,
  int                      explicitIndex
)
{
  device_configuration_t  *pCurrentDcItem;
  boolean                  explicit_index_updated = FALSE;

  pCurrentDcItem = policyman_get_device_config(pItemSet);

  if (NULL != pCurrentDcItem)
  {
    device_configuration_t *pDcItem;

    pDcItem = policyman_device_config_item_clone(pCurrentDcItem);

    if (pDcItem->explicit_index != explicitIndex)
    {
      explicit_index_updated = TRUE;
    }

    pDcItem->explicit_index = explicitIndex;
    if ( policyman_update_current_cfg_index(pDcItem, pState, ddsSubs) ||
         explicit_index_updated
       )
    {
      mre_set_replace(pItemSet, &pDcItem);
    }
    ref_cnt_obj_release(pDcItem);
  }

  MRE_RELEASE_IF(pCurrentDcItem);
}

/*-------- policyman_device_config_get_updated_explicit_index --------*/
STATIC void policyman_device_config_get_updated_explicit_index(
  policyman_state_t    *pState,
  mre_set_t            *pItemSet,
  int                  *pExplicitIndex
)
{
  device_configuration_t **ppItem = NULL;

  ppItem = (device_configuration_t **)policyman_cfgitem_find_item_in_itemset( pItemSet,
                                                                              POLICYMAN_ITEM_DEVICE_CONFIGURATION,
                                                                              SYS_MODEM_AS_ID_1 );

  if (NON_NULL(ppItem) && NON_NULL(*ppItem))
  {
    *pExplicitIndex = (*ppItem)->explicit_index;
    ref_cnt_obj_release(*ppItem);
  }
  else
  {
    (void)policyman_device_config_get_explicit_index(pExplicitIndex);
  }
}

/*-------- policyman_device_config_update_subphone_config --------*/
void policyman_device_config_update_subphone_config(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  ddsSubs,
  mre_set_t              *pItemSet
)
{
  int                   explicitIndex;

  // we don't overwrite explcit index here so get whatever we currently have
  (void)policyman_device_config_get_updated_explicit_index(pState, pItemSet, &explicitIndex);
  set_explicit_index(pState, pItemSet, ddsSubs, explicitIndex);
}


/*-------- policyman_device_config_lte_is_present --------*/
boolean policyman_device_config_lte_is_present(
  policyman_state_t      *pState,
  mre_set_t              *pItemSet,
  sys_modem_as_id_e_type  subsId
)
{
  device_configuration_t  *pCurrentDcItem;
  boolean                  lteIsPresent = FALSE;
  uint32                   ratMask;

  pCurrentDcItem = policyman_get_device_config(pItemSet);

  if (NULL != pCurrentDcItem)
  {
    ratMask = device_config_get_current_config(pCurrentDcItem, subsId);

    lteIsPresent = ((ratMask & SYS_SYS_MODE_MASK_LTE) != 0);
  }

  MRE_RELEASE_IF(pCurrentDcItem);

  return lteIsPresent;
}

/*-------- policyman_device_config_lte_is_on_multiple_subs --------*/
boolean policyman_device_config_lte_is_on_multiple_subs(
  policyman_state_t      *pState,
  mre_set_t              *pItemSet
)
{
  boolean            lteIsOnAllSubs = TRUE;
  size_t             nSim           = 1;
  size_t             subsId;
  policyman_status_t status;

  status = policyman_get_current_num_sim(&nSim);
  if (POLICYMAN_FAILED(status) || nSim == 1)
  {
    return FALSE;
  }

  for ( subsId = SYS_MODEM_AS_ID_1;
        (POLICYMAN_SUCCEEDED(status) && lteIsOnAllSubs && subsId < nSim);
        subsId++)
  {
    lteIsOnAllSubs = policyman_device_config_lte_is_present(pState, pItemSet, subsId);
  }

  return lteIsOnAllSubs;
}

/*-------- policyman_device_config_determine_lte_subs --------*/
STATIC sys_modem_as_id_e_type policyman_determine_lte_subs(
  policyman_state_t      *pState,
  mre_set_t              *pItemSet
)
{
  size_t                 nSim         = 1;
  sys_modem_as_id_e_type lteSubs      = SYS_MODEM_AS_ID_1;
  boolean                lteIsPresent = FALSE;
  size_t                 idx;
  policyman_status_t     status;

  status = policyman_get_current_num_sim(&nSim);
  for ( idx = SYS_MODEM_AS_ID_1;
        POLICYMAN_SUCCEEDED(status) && !lteIsPresent && idx < nSim;
        idx++ )
  {
    lteIsPresent = policyman_device_config_lte_is_present(pState, pItemSet, idx);
    if (lteIsPresent)
    {
      lteSubs = (sys_modem_as_id_e_type)idx;
    }
  }

  return lteSubs;
}


/*-------- policyman_device_config_determine_lte_subs --------*/
STATIC sys_modem_as_id_e_type policyman_device_config_determine_lte_subs(
  policyman_state_t  *pState,
  mre_set_t          *pItemSet
)
{
  sys_modem_as_id_e_type lteSubs = SYS_MODEM_AS_ID_1;

  if (policyman_subs_dds_is_permanent(pState))
  {
    lteSubs = policyman_subs_get_dds_subs(pState);
  }
  else
  {
    lteSubs = policyman_determine_lte_subs(pState, pItemSet);
  }

  return lteSubs;
}

/*-------- policyman_device_config_update_cdma_subphone_config --------*/
void policyman_device_config_update_cdma_subphone_config(
  policyman_state_t      *pState,
  mre_set_t              *pItemSet
)
{
  sys_modem_as_id_e_type lteSubs;

  // Retrieve which subs has LTE so that it will remain unchanged. Using as DDS subs.
  lteSubs = policyman_device_config_determine_lte_subs(pState, pItemSet);

  // Run config update, locking LTE subs in place, but allowing CDMA subs to change
  policyman_device_config_update_subphone_config(pState, lteSubs, pItemSet);
}

/*-------- policyman_execute_process_explicit_index --------*/
STATIC void policyman_execute_process_explicit_index(
  policyman_cmd_t *pCmd
)
{
  policyman_explicit_index_cmd_t *pDevCfgCmd = (policyman_explicit_index_cmd_t *) pCmd;
  policyman_sandbox_t            *pSandbox   = pCmd->pSandbox;
  mre_set_t                      *pItemSet;
  sys_modem_as_id_e_type          ddsSubs;

  pItemSet = policyman_itemset_new();
  ddsSubs  = policyman_subs_get_dds_subs(pSandbox->pState);

  policyman_state_enter_crit_section();

  set_explicit_index(pSandbox->pState, pItemSet, ddsSubs, pDevCfgCmd->explicit_index);

  policyman_state_leave_crit_section();

  policyman_policies_run_policy_check(pSandbox, pItemSet);
  policyman_cfgitem_update_items(pSandbox->pState, pItemSet, &pCmd->clientMsg);
  MRE_RELEASE_IF(pItemSet);
}

/*-------- policyman_device_config_set_explicit_index --------*/
policyman_status_t policyman_device_config_set_explicit_index(
  int  explicitIndex
)
{
  policyman_explicit_index_cmd_t *pCmd;
  policyman_status_t              status = POLICYMAN_STATUS_SUCCESS;
  size_t                          numCfgs;

  POLICYMAN_MSG_HIGH_1("received explicit config index %d from client", explicitIndex);

  numCfgs = device_config_get_num_configs();

  if (explicitIndex >= (int)numCfgs)
  {
    POLICYMAN_MSG_ERROR_2( "explicit index %d exceeds number of configs (%d)",
                           explicitIndex,
                           numCfgs );

    status = POLICYMAN_STATUS_ERR_INVALID_ARGS;
    goto Done;
  }

  pCmd = POLICYMAN_CMD_NEW( policyman_explicit_index_cmd_t,
                            policyman_execute_process_explicit_index,
                            NULL,
                            NULL );

  pCmd->explicit_index = explicitIndex;

  policyman_queue_put_cmd((policyman_cmd_t *) pCmd);
  ref_cnt_obj_release(pCmd);

Done:
  return status;
}

/*-------- policyman_device_config_get_explicit_index --------*/
policyman_status_t policyman_device_config_get_explicit_index(
  int *pExplicitIndex
)
{
  policyman_status_t       status;
  device_configuration_t  *pDcItem = NULL;

  if (NULL == pExplicitIndex)
  {
    return POLICYMAN_STATUS_ERR_INVALID_ARGS;
  }

  status = policyman_get_current_device_config((policyman_item_t **)&pDcItem);
  if (POLICYMAN_FAILED(status))
  {
    POLICYMAN_MSG_ERROR_0("policyman_device_config_get_explicit_index called before device config available");
    return POLICYMAN_STATUS_ERR_NOT_READY;
  }

  *pExplicitIndex = pDcItem->explicit_index;
  MRE_RELEASE_IF(pDcItem);

  return POLICYMAN_STATUS_SUCCESS;
}

/*-------- policyman_device_config_reset_subs_feature --------*/
void policyman_device_config_reset_subs_feature(
  sys_modem_as_id_e_type subsId
)
{
  device_configuration_t  *pDcItem;
  size_t                   cfgIndex;

  (void)policyman_get_current_device_config((policyman_item_t **)&pDcItem);

  for (cfgIndex = 0; cfgIndex < pDcItem->num_cfgs; cfgIndex++)
  {
    size_t        index;
    subs_info_t  *pSubsInfo;

    index     = device_config_get_subphone_index(pDcItem->num_sims, cfgIndex, SUBS_ID_TO_NSIM(subsId));
    pSubsInfo = &pDcItem->pConfigs[index];

    if (pSubsInfo->feature != SYS_SUBS_FEATURE_MODE_DUAL_MULTIMODE)
    {
      POLICYMAN_MSG_HIGH_3( SUBS_PREFIX "cfg %d -- resetting feature from %d to NORMAL",
                            subsId,
                            cfgIndex,
                            pSubsInfo->feature );

      pSubsInfo->feature = SYS_SUBS_FEATURE_MODE_NORMAL;
    }
  }
  MRE_RELEASE_IF(pDcItem);
}

/*-------- policyman_get_device_config_from_itemset --------*/
device_configuration_t *policyman_get_device_config_from_itemset(
  mre_set_t *pItemSet
)
{
  device_configuration_t  **ppDcItem;
  device_configuration_t   *pDcItem   = NULL;

  /* check if itemSet already has device config item */
  ppDcItem = (device_configuration_t **)policyman_cfgitem_find_item_in_itemset( pItemSet,
                                                                            POLICYMAN_ITEM_DEVICE_CONFIGURATION,
                                                                            SYS_MODEM_AS_ID_1 );
  if (NON_NULL(ppDcItem) && NON_NULL(*ppDcItem))
  {
    pDcItem = *ppDcItem;
  }

  return pDcItem;
}

/*-------- policyman_get_device_config --------*/
device_configuration_t *policyman_get_device_config(
  mre_set_t *pItemSet
)
{
  device_configuration_t  **ppDcItem;
  device_configuration_t   *pDcItem   = NULL;

  /* check if itemSet already has device config item */
  ppDcItem = (device_configuration_t **)policyman_cfgitem_find_item_in_itemset( pItemSet,
                                                                            POLICYMAN_ITEM_DEVICE_CONFIGURATION,
                                                                            SYS_MODEM_AS_ID_1 );
  if (NULL != ppDcItem && NULL != *ppDcItem)
  {
    pDcItem = *ppDcItem;
  }
  else
  {
    pDcItem = (device_configuration_t *) policyman_cfgitem_get_item( POLICYMAN_ITEM_DEVICE_CONFIGURATION,
                                                                     SYS_MODEM_AS_ID_1 );
  }

  return pDcItem;
}


/*=============================================================================
  Device configuration action parsing and APIs
=============================================================================*/


typedef struct
{
  sys_subs_feature_t    feature;
  rat_capability_info_t ratInfo;
} subs_info_action_t;


typedef struct
{
  POLICYMAN_ACTION_BASE;

  size_t                    num_sims;
  size_t                    max_active_voice;
  size_t                    max_active_data;
  size_t                    num_cfgs;
  uint32                    modified;
  uint32                    parameterMask;
  char const               *pConfigName;
  size_t                    e911_num_cfg;
  size_t                    current_cfg_index;
  rat_capability_info_t    *pECallRatCfg;
  mre_named_obj_namespace_t ns;
  lookup_method_t           lookup_method;
  subs_info_action_t        subs_info[];
} device_config_action_t;


/*-------- policyman_feature_is_supported_by_hardware --------*/
boolean policyman_feature_is_supported_by_hardware(
  mre_set_t   *pItemSet
)
{
  boolean                  is_supported = FALSE;
  device_configuration_t  *pDc          = NULL;

  pDc = policyman_get_device_config(pItemSet);

  // ensure an item with a valid config before checking the feature
  if (NULL != pDc && pDc->num_cfgs > 0)
  {
    sys_subs_feature_t  feature;

    feature = device_config_get_subs_feature(pDc, 0, 0);

    switch (feature)
    {
      case SYS_SUBS_FEATURE_MODE_NORMAL:
        is_supported = TRUE;
        break;

      case SYS_SUBS_FEATURE_MODE_DUAL_MULTIMODE:
        is_supported = TRUE;
        break;

      case SYS_SUBS_FEATURE_MODE_SRLTE:
#if defined(FEATURE_1XSRLTE)
        is_supported = TRUE;
#endif
        if (!is_supported)
        {
          POLICYMAN_MSG_ERROR_0("SRLTE specified in configuration, but not supported");
        }

        break;

      case SYS_SUBS_FEATURE_MODE_SVLTE:
      case SYS_SUBS_FEATURE_MODE_SGLTE:
      default:
        POLICYMAN_MSG_ERROR_1("Unsupported feature S[VG]LTE or invalid feature type: %d", feature);
        break;
    }
  }

  REF_CNT_OBJ_RELEASE_IF(pDc);

  return is_supported;
}


/*-------- policyman_device_config_get_feature --------*/
STATIC boolean policyman_device_config_get_feature(
  char const          *pStr,
  sys_subs_feature_t  *pFeature
)
{
  static MRE_STR_VALUE_TABLE(map) =
  {
    MRE_STR_VALUE_ENTRY("normal",      SYS_SUBS_FEATURE_MODE_NORMAL),
    MRE_STR_VALUE_ENTRY("svlte",       SYS_SUBS_FEATURE_MODE_SVLTE),
    MRE_STR_VALUE_ENTRY("sglte",       SYS_SUBS_FEATURE_MODE_SGLTE),
    MRE_STR_VALUE_ENTRY("srlte",       SYS_SUBS_FEATURE_MODE_SRLTE),
    MRE_STR_VALUE_ENTRY("dualmmode",   SYS_SUBS_FEATURE_MODE_DUAL_MULTIMODE),
  };

  mre_status_t        status  = MRE_STATUS_SUCCESS;
  sys_subs_feature_t  feature = SYS_SUBS_FEATURE_MODE_NORMAL;
  char                token[32];

  if (policyman_get_token(&pStr, token, sizeof(token)))
  {
    MRE_LOOKUP_STR_VALUE(map, token, sys_subs_feature_t, &feature);
    if (MRE_FAILED(status))
    {
      POLICYMAN_MSG_ERROR_0("Invalid feature value in <device_configuration>");
      POLICYMAN_UT_MSG_S1("feature = \"%s\" specified in <device_configuration>", pStr);
    }
  }

  *pFeature = feature;
  return MRE_SUCCEEDED(status);
}

/*-------- device_config_action_get_subphone --------*/
STATIC subs_info_action_t *device_config_action_get_subphone(
  device_config_action_t  *pAction,
  size_t                   nCfg,
  size_t                   nSim
)
{
  size_t cfgIndex = device_config_get_subphone_index(pAction->num_sims, nCfg, nSim);
  return &pAction->subs_info[cfgIndex];
}

/*-------- device_config_read_mask --------*/
STATIC boolean device_config_read_mask(
  rat_capability_info_t   *pRatInfo,
  char const              *pRatStr
)
{
  boolean  succeeded   = FALSE;

  /* Read Rat mask provided in this subphone string */
  pRatInfo->base        = POLICYMAN_BASE_NONE;
  pRatInfo->excludeMask = 0;

  succeeded = policyman_rat_config_parse_rats( pRatStr,
                                              &pRatInfo->includeMask );
  if (succeeded)
  {
    POLICYMAN_MSG_HIGH_1("Include Mask = 0x%04x", pRatInfo->includeMask);
  }

  return succeeded;
}

/*-------- device_config_get_e911_info --------*/
STATIC rat_capability_info_t *device_config_get_e911_info(
  device_config_action_t  *pAction,
  size_t                   nCfg,
  size_t                   nSim
)
{
  size_t cfgIndex;

  cfgIndex = device_config_get_subphone_index(pAction->num_sims, nCfg, nSim);

  return &pAction->pECallRatCfg[cfgIndex];
}

/*-------- device_config_get_attribute_for_subphone --------*/
STATIC mre_status_t device_config_get_attribute_for_subphone(
  mre_xml_element_t const        *pChild,
  size_t                          nSim,
  char const                    **ppStr
)
{
  mre_status_t   status = MRE_STATUS_SUCCESS;

  if (nSim < 1)
  {
    *ppStr = policyman_xml_get_attribute(pChild, "primary");
  }
  else
  {
    *ppStr = policyman_xml_get_attribute(pChild, "secondary");
  }

  if (*ppStr == NULL)
  {
    POLICYMAN_MSG_ERROR_0("no primary or secondary attribute present in XML");
    status = MRE_STATUS_ERR_INVALID_ACTION;
  }

  return status;
}

/*-------- device_config_read_e911_ratMask --------*/
STATIC mre_status_t device_config_read_e911_ratMask(
  device_config_action_t        *pAction,
  mre_xml_element_t const       *pChild,
  size_t                         nCfg,
  size_t                         nSim
)
{
  rat_capability_info_t       *pSubPhoneInfo;
  mre_status_t                 status;
  boolean                      haveMask      = FALSE;
  char const                  *pStr          = NULL;

  status = device_config_get_attribute_for_subphone(pChild, nSim, &pStr);
  if (MRE_SUCCEEDED(status))
  {
    pSubPhoneInfo = device_config_get_e911_info(pAction, nCfg, nSim);
    haveMask = device_config_read_mask(pSubPhoneInfo, pStr);

    if (haveMask)
    {
      POLICYMAN_MSG_HIGH_2("read subphone[%d] e911 mask = 0x%04x", nSim, pSubPhoneInfo->includeMask);
      status = MRE_STATUS_SUCCESS;
    }
  }

  return status;
}

/*-------- device_config_read_subphone_ratMask --------*/
STATIC mre_status_t device_config_read_subphone_ratMask(
  subs_info_action_t            *pSubPhoneInfo,
  mre_xml_element_t const       *pChild,
  size_t                         nSim
)
{
  mre_status_t   status;
  boolean        haveMask  = FALSE;
  char const    *pStr      = NULL;

  status = device_config_get_attribute_for_subphone(pChild, nSim, &pStr);
  if (MRE_SUCCEEDED(status))
  {
    haveMask = device_config_read_mask(&pSubPhoneInfo->ratInfo, pStr);

    if (haveMask)
    {
      POLICYMAN_MSG_HIGH_2("read subphone[%d] capability 0x%04x", nSim, pSubPhoneInfo->ratInfo.includeMask);
      status = MRE_STATUS_SUCCESS;
    }
  }

  return status;
}


/*-------- policyman_device_config_get_subs_info --------*/
STATIC mre_status_t policyman_device_config_get_subs_info(
  device_config_action_t        *pAction,
  mre_xml_element_t const       *pElem,
  size_t                         nCfg
)
{
  mre_status_t         status = MRE_STATUS_ERR_INVALID_ACTION;
  size_t               subsId;
  subs_info_action_t  *pSubsInfo;
  char const          *pStr;

  pStr = policyman_xml_get_attribute(pElem, "id");
  if (pStr == NULL)
  {
    goto Done;
  }

  subsId = atoi(pStr);
  if (subsId > pAction->num_sims)
  {
    POLICYMAN_MSG_ERROR_2( "device_configuration: config has a subs id of %d, but there are only %d SIMs",
                           subsId,
                           pAction->num_sims );
    goto Done;
  }

  /* get subphone info */
  pSubsInfo = device_config_action_get_subphone(pAction, nCfg, (subsId - 1));

  /*  Get any feature mode associated with this subs - default is "normal".
   */
  pSubsInfo->feature = SYS_SUBS_FEATURE_MODE_NORMAL;
  pStr = policyman_xml_get_attribute(pElem, "feature");
  if (pStr != NULL)
  {
    POLICYMAN_MSG_ERROR_0("Setting features in <device_configuration> is deprecated");

    if (!policyman_device_config_get_feature(pStr, &pSubsInfo->feature))
    {
      goto Done;
    }

    pAction->modified |= 0;
  }

  pElem = policyman_xml_get_child(pElem, 0);
  if (pElem == NULL)
  {
    goto Done;
  }

  status = policyman_rat_capability_read_info( pElem,
                                               &pSubsInfo->ratInfo );

Done:
  return status;
}

/*-------- read_device_config_from_inline_configs --------*/
STATIC mre_status_t read_device_config_from_inline_configs(
  device_config_action_t        *pAction,
  mre_xml_element_t const       *pChild,
  size_t                         nCfg
)
{
  mre_status_t  status = MRE_STATUS_SUCCESS;
  size_t        nSim;

  for (nSim = 0; nSim < pAction->num_sims; nSim++)
  {
    subs_info_action_t *pSubPhoneInfo;

    pSubPhoneInfo = device_config_action_get_subphone(pAction, nCfg, nSim);

    pSubPhoneInfo->feature = SYS_SUBS_FEATURE_MODE_NORMAL;
    status = device_config_read_subphone_ratMask(pSubPhoneInfo, pChild, nSim);
  }

  return status;
}

/*-------- read_device_config_from_expanded_configs --------*/
STATIC mre_status_t read_device_config_from_expanded_configs(
  device_config_action_t        *pAction,
  mre_xml_element_t const       *pChild,
  size_t                         cfgIdx
)
{
  mre_status_t  status = MRE_STATUS_FAILED;
  size_t        childIdx;
  size_t        childCount;

  /* Get number of children to detect if this is old style config */
  childCount = policyman_xml_num_children(pChild);

  if (childCount != pAction->num_sims)
  {
    POLICYMAN_UT_MSG_3( "device_configuration: number of SIMs is %d but configuration %d has %d elements",
                        pAction->num_sims,
                        cfgIdx,
                        childCount );
    goto Done;
  }

  status = MRE_STATUS_SUCCESS;

  for (childIdx = 0 ; !MRE_FAILED(status) && childIdx < childCount ; ++childIdx)
  {
    mre_xml_element_t const *pSubs = policyman_xml_get_child(pChild, childIdx);

    if (pSubs == NULL)
    {
      status = MRE_STATUS_ERR_NOT_PRESENT;
      break;
    }

    if (!policyman_xml_tag_is(pSubs, "subs"))
    {
      POLICYMAN_UT_MSG_0("device_configuration: top level elements of 'config' must be 'subs'");
      goto Done;
    }

    status = policyman_device_config_get_subs_info(pAction, pSubs, cfgIdx);
  }

Done:
  return status;
}

/*-------- policyman_device_config_read_cfg --------*/
STATIC mre_status_t policyman_device_config_read_cfg(
  device_config_action_t        *pAction,
  mre_xml_element_t const       *pChild,
  size_t                         cfgIdx
)
{
  mre_status_t  status;

  /* Read the subphone info from nested subs_id tags first. If not available
       then look for feature and Rat mask attributes on primary and secondary subphones
  */
  status = read_device_config_from_expanded_configs(pAction, pChild, cfgIdx);

  if (MRE_FAILED(status))
  {
    status = read_device_config_from_inline_configs(pAction, pChild, cfgIdx);
  }

  return status;
}

/*-------- policyman_device_config_read_e911_config --------*/
STATIC mre_status_t policyman_device_config_read_e911_config(
  device_config_action_t        *pAction,
  mre_xml_element_t const       *pChild,
  size_t                         cfgIdx
)
{
  mre_status_t  status   = MRE_STATUS_SUCCESS;
  size_t        simIdx;

  for (simIdx = 0; simIdx < pAction->num_sims; simIdx++)
  {
    status = device_config_read_e911_ratMask(pAction, pChild, cfgIdx, simIdx);
  }

  return status;
}

/*-------- policyman_device_config_get_num_active_voice --------*/
STATIC boolean policyman_device_config_get_num_active_voice(
  size_t                        *pnActive,
  mre_xml_element_t const       *pCfg
)
{
  char const  *pStr;
  boolean      succeeded = FALSE;

  /* max_active is not a mandatory parameter */
  pStr = policyman_xml_get_attribute(pCfg, "max_active");

  if (pStr == NULL)
  {
    pStr = policyman_xml_get_attribute(pCfg, "max_active_voice");
  }

  succeeded = (pStr != NULL);

  if (succeeded)
  {
    *pnActive= atoi(pStr);
  }
  else
  {
    POLICYMAN_UT_MSG_0("<device_configuration> max_active/max_active_voice attribute is not present");
  }

  return succeeded;
}

/*-------- policyman_device_config_get_num_active_data --------*/
STATIC boolean policyman_device_config_get_num_active_data(
  size_t                        *pnActiveData,
  mre_xml_element_t const *pCfg
)
{
  char const *pStr;
  boolean     succeeded = FALSE;

  pStr = policyman_xml_get_attribute(pCfg, "max_active_data");
  succeeded = (pStr != NULL);

  if (succeeded)
  {
    *pnActiveData= atoi(pStr);
  }
  else
  {
    POLICYMAN_MSG_HIGH_0("<device_configuration> max_active_data attribute is not present");
  }

  return succeeded;
}


/*-------- policyman_device_config_get_num_sims --------*/
STATIC boolean policyman_device_config_get_num_sims(
  size_t                        *pnSims,
  mre_xml_element_t const  *pElem
)
{
  char const *pStr;
  boolean     succeeded = FALSE;

  /* num_sims is a mandatory parameter */
  pStr = policyman_xml_get_attribute(pElem, "num_sims");
  succeeded = (pStr != NULL);

  if (succeeded)
  {
    *pnSims = atoi(pStr);
  }
  else
  {
    POLICYMAN_MSG_ERROR_0("<device_configuration> requires a num_sims attribute");
  }

  return succeeded;
}

/*-------- policyman_update_configs_for_action_execute --------*/
STATIC void policyman_update_configs_for_action_execute(
  device_configuration_t  *pDcItem,
  device_config_action_t  *pDcAction,
  policyman_state_t       *pState,
  mre_set_t               *pItemSet
)
{
  size_t index;

  // add entries to the list
  pDcItem->num_cfgs = pDcAction->num_cfgs;

  // delete the previous configs[] array and create a new one
  POLICYMAN_MEM_FREE_IF(pDcItem->pConfigs);
  pDcItem->pConfigs = (subs_info_t *)policyman_mem_alloc(sizeof(subs_info_t) * pDcAction->num_cfgs * pDcAction->num_sims);

  for (index = 0; index < pDcAction->num_sims * pDcAction->num_cfgs; index++)
  {
    pDcItem->pConfigs[index].feature = pDcAction->subs_info[index].feature;
    pDcItem->pConfigs[index].ratInfo = policyman_rat_capability_evaluate( &pDcAction->subs_info[index].ratInfo,
                                                                          pItemSet,
                                                                          SYS_MODEM_AS_ID_1);
  }

  {
    sys_modem_as_id_e_type  ddsSubs;

  ddsSubs = policyman_subs_get_dds_subs(pState);
    (void)policyman_update_current_cfg_index(pDcItem, pState, ddsSubs);
  }

  POLICYMAN_MSG_HIGH_2( "<device_configuration>: updated %d config entries with current index %d",
                        pDcAction->num_cfgs,
                        pDcItem->current_cfg_index );
}

/*-------- policyman_get_num_config_from_all_configs --------*/
STATIC size_t policyman_get_num_config_from_all_configs(
  mre_xml_element_t const *pElem,
  char const              *pTagString
)
{
  size_t                         index;
  mre_xml_element_t const       *pChild;
  size_t                         numConfigs = 0;

  /* loop through all configs to figure out which are subphone capabilities
       and which ones are e911 capabilities
   */
  for (index = 0; index < policyman_xml_num_children(pElem); index++)
  {
    pChild = policyman_xml_get_child(pElem, index);

    if (pChild != NULL && policyman_xml_tag_is(pChild, pTagString))
    {
      numConfigs++;
    }
  }

  return numConfigs;
}

/*-------- device_config_read_masked_values_from_action --------*/
STATIC void device_config_read_masked_values_from_action(
  device_config_action_t *pDcAction,
  policyman_state_t      *pState,
  mre_set_t              *pItemSet,
  device_configuration_t *pDcItem
)
{
  if (pDcAction->parameterMask & PARAMETER_MASK_NMAXVOICE)
  {
    pDcItem->max_active_voice = pDcAction->max_active_voice;
  }

  if (pDcAction->parameterMask & PARAMETER_MASK_NMAXDATA)
  {
    pDcItem->max_active_data = pDcAction->max_active_data;
  }

  /* Update config always from XML */
  if (pDcAction->parameterMask & PARAMETER_MASK_CONFIG)
  {
    /* See if config already exists in list of supported configs, update the index accordingly
      */
    if ((pDcItem->num_cfgs > 0) && (!pDcItem->readFromDefault))
    {
      POLICYMAN_MSG_ERROR_1("device config update is not allowed, already having %d configs", pDcItem->num_cfgs);
    }
    else
    {
      pDcItem->readFromDefault = FALSE;
      policyman_update_configs_for_action_execute(pDcItem, pDcAction, pState, pItemSet);
    }
  }
}

/*-------- read_e911_config_from_action_includeMask --------*/
STATIC void read_e911_config_from_action_includeMask(
  device_config_action_t *pDcAction,
  device_configuration_t *pDcItem
)
{
  size_t index;

  for (index = 0; index < pDcItem->num_sims * pDcItem->e911_num_cfgs; index++)
  {
    pDcItem->pECallRatMask[index] = pDcAction->pECallRatCfg[index].includeMask;
  }
}

/*-------- device_config_read_e911_config_from_action --------*/
STATIC void device_config_read_e911_config_from_action(
  device_config_action_t *pDcAction,
  device_configuration_t *pDcItem
)
{
  /* Don't allow update if we already have entries in e911 configs[]. */
  if (pDcItem->e911_num_cfgs > 0)
  {
    POLICYMAN_MSG_HIGH_1( "Current e911 config already has %d entries, not updating from action",
                          pDcItem->e911_num_cfgs );
    return;
  }

  /* Free the already allocated array first. */
  pDcItem->e911_num_cfgs = pDcAction->e911_num_cfg;
  POLICYMAN_MEM_FREE_IF(pDcItem->pECallRatMask);

  if (pDcItem->e911_num_cfgs > 0)
  {
    size_t               e911CfgSize;

    e911CfgSize        = sizeof(uint32) * pDcItem->num_sims * pDcItem->e911_num_cfgs;
    pDcItem->pECallRatMask = (uint32 *)policyman_mem_alloc(e911CfgSize);

    read_e911_config_from_action_includeMask(pDcAction, pDcItem);
  }
}

/*-------- policyman_device_config_create --------*/
STATIC device_configuration_t* policyman_device_config_create(
  device_config_action_t  *pDcAction,
  policyman_state_t       *pState,
  mre_set_t               *pItemSet
)
{
  device_configuration_t   *pDcItem;
  device_configuration_t   *pCurrentDcItem = NULL;
  size_t                    nCfg;
  size_t                    nE911Cfg;
  policyman_status_t        status;

  nCfg     = pDcAction->num_cfgs;
  nE911Cfg = pDcAction->e911_num_cfg;

  POLICYMAN_MSG_HIGH_1(SRC_LINE "action <device_configuration> executing", LINE_NUM(pDcAction));

  /* Read current config and determine if we need to add configs that are defined in action */
  status = policyman_get_current_device_config((policyman_item_t **)&pCurrentDcItem);

  if (POLICYMAN_SUCCEEDED(status))
  {
    nE911Cfg = pCurrentDcItem->e911_num_cfgs;
    nCfg     = (pDcAction->num_cfgs > pCurrentDcItem->num_cfgs)
               ? pDcAction->num_cfgs
               : pCurrentDcItem->num_cfgs;

    POLICYMAN_MSG_HIGH_4( "current num_cfgs %d; action has %d configs, new num_cfgs %d, new e911_num_cfg %d",
                          pCurrentDcItem->num_cfgs,
                          pDcAction->num_cfgs,
                          nCfg,
                          nE911Cfg );
  }

  /* determine the size of new device config item and create it */
  pDcItem = (device_configuration_t *)policyman_device_config_item_new(SYS_MODEM_AS_ID_1);

  /* max_active_data defaults to 1 */
  pDcItem->max_active_data = 1;
  pDcItem->explicit_index  = -1;
  pDcItem->hardware_mask   = policyman_rf_get_overall_rat_mask();
  pDcItem->readFromDefault = FALSE;

  /* restore all parameters from existing device config */
  if (NULL != pCurrentDcItem)
  {
    device_config_copy_parameters((device_configuration_t const *)pCurrentDcItem, pDcItem);
  }

  /* Read all mandatory and optional parameters one by one */
  pDcItem->num_sims = pDcAction->num_sims;
  g_num_Sim         = pDcItem->num_sims;

  /* Determine the maximum active voice subs ever possible */
  if (pDcItem->max_active_voice > g_maxActiveVoiceSubs)
  {
    g_maxActiveVoiceSubs = pDcItem->max_active_voice;
  }

  device_config_read_masked_values_from_action(pDcAction, pState, pItemSet, pDcItem);

  /* Read e911 config next */
  device_config_read_e911_config_from_action(pDcAction, pDcItem);

  MRE_RELEASE_IF(pCurrentDcItem);
  POLICYMAN_MSG_HIGH_1(SRC_LINE "<device_configuration> item created", LINE_NUM(pDcAction));

  return pDcItem;
}

/*-------- policyman_device_config_execute --------*/
STATIC boolean policyman_device_config_execute(
  mre_action_t const  *pAction,
  void                *pCtx
)
{
  device_config_action_t  *pDcAction  = (device_config_action_t *) pAction;
  policyman_state_t       *pState     = POLICY_EXECUTE_STATE(pCtx);
  mre_set_t               *pItemSet   = POLICY_EXECUTE_ITEMSET(pCtx);
  device_configuration_t  *pDcItem    = NULL;

  pDcItem = policyman_device_config_create(pDcAction, pState, pItemSet);
  mre_set_replace(pItemSet, &pDcItem);
  ref_cnt_obj_release(pDcItem);

  return TRUE;
}


/*-------- policyman_device_config_present --------*/
STATIC boolean policyman_device_config_present(
  void
)
{
  boolean                 isPresent;
  device_configuration_t  *pDcItem;

  pDcItem = (device_configuration_t *) policyman_cfgitem_get_item( POLICYMAN_ITEM_DEVICE_CONFIGURATION,
                                                               SYS_MODEM_AS_ID_1 );

  isPresent = (pDcItem != NULL && (pDcItem->modified & DEV_CONFIG_NUM_SIMS_MODIFIED) == 0);

  MRE_RELEASE_IF(pDcItem);

  return isPresent;
}


/*-------- policyman_device_config_if_execute --------*/
STATIC boolean policyman_device_config_if_execute(
  mre_action_t const  *pAction,
  void                *pCtx
)
{
  device_config_action_t  *pDcAction        = (device_config_action_t *)pAction;
  mre_set_t               *pItemSet         = POLICY_EXECUTE_ITEMSET(pCtx);
  device_configuration_t  *pDcItem;
  boolean                  succeeded        = FALSE;
  boolean                  item_is_present;

  item_is_present = policyman_cfgitem_find_item_based_on_lookup_method( pDcAction->lookup_method,
                                                                        pItemSet,
                                                                        POLICYMAN_ITEM_DEVICE_CONFIGURATION,
                                                                        SYS_MODEM_AS_ID_1,
                                                                        (policyman_item_t **)&pDcItem );

  if (item_is_present)
  {
      succeeded =  (pDcItem)->num_cfgs > 0;
      ref_cnt_obj_release(pDcItem);
      POLICYMAN_UT_MSG_1(SRC_LINE "<device_config_if> skipping update - item present", LINE_NUM(pAction));
  }
  else
  {
    succeeded = policyman_device_config_execute(pAction, pCtx);
  }

  return succeeded;
}


/*-------- policyman_device_config_action_dtor --------*/
STATIC void policyman_device_config_action_dtor(
  void  *pObj
)
{
  device_config_action_t  *pMe = (device_config_action_t *) pObj;

  POLICYMAN_MEM_FREE_IF(pMe->pConfigName);
  POLICYMAN_MEM_FREE_IF(pMe->pECallRatCfg);
  policyman_action_dtor(pObj);
}


/*-------- device_config_allocate_e911_config_array --------*/
STATIC void device_config_allocate_e911_config_array(
  mre_xml_element_t const  *pElem,
  policyman_policy_t             *pPolicy,
  device_config_action_t         *pAction
)
{
  size_t                  nCfgs;

  nCfgs = policyman_get_num_config_from_all_configs(pElem, DEV_CFG_STR_E911_CONFIG);

  POLICYMAN_UT_MSG_2( "--------------- device_config: %d children, %d 'e911_config' entries",
                      policyman_xml_num_children(pElem),
                      nCfgs);

  if (nCfgs > 0)
  {
    size_t                  e911CfgSize;

    // The e911 rat config is an array (num_sims by nCfgs) in size
    e911CfgSize           = sizeof(rat_capability_info_t) * pAction->num_sims * nCfgs;

    pAction->e911_num_cfg = nCfgs;
    pAction->pECallRatCfg = (rat_capability_info_t *)policyman_mem_alloc(e911CfgSize);
  }
}

/*-------- device_config_allocate_new_action --------*/
STATIC device_config_action_t * device_config_allocate_new_action(
  mre_xml_element_t const  *pElem,
  policyman_policy_t             *pPolicy,
  size_t                          nSims
)
{
  size_t                  nCfgs;
  size_t                  dcSize;
  device_config_action_t *pAction;

  //
  // Allocate the base device config action
  //
  nCfgs = policyman_get_num_config_from_all_configs(pElem, DEV_CFG_STR_CONFIG);

  POLICYMAN_UT_MSG_2( "--------------- device_config: %d children, %d 'config' entries",
                      policyman_xml_num_children(pElem),
                      nCfgs );

  // The subs_info field is an array (nSims by nCfgs) in size
  dcSize =  sizeof(device_config_action_t)
          + sizeof(subs_info_action_t) * nSims * nCfgs;

  pAction = (device_config_action_t *)policyman_action_new( dcSize,
                                                            (mre_policy_t *)pPolicy,
                                                            NULL,  // No execute
                                                            policyman_device_config_action_dtor,
                                                            SUBS_THIS,
                                                            pElem );
  pAction->num_cfgs         = nCfgs;
  pAction->num_sims         = nSims;

  // Allocate the e911 config array (part of the device config action)
  device_config_allocate_e911_config_array(pElem, pPolicy, pAction);

  return pAction;
}


/*-------- device_config_read_e911_and_subphone_configs --------*/
STATIC mre_status_t device_config_read_e911_and_subphone_configs(
  mre_xml_element_t const  *pElem,
  device_config_action_t         *pAction
)
{
  size_t        i;
  size_t        nCfgs     = 0;
  size_t        nE911Cfgs = 0;
  mre_status_t  status    = MRE_STATUS_SUCCESS;

  /* Loop through all configs and read subphone configs and e911 configs */
  for (i = 0; MRE_SUCCEEDED(status) && i < policyman_xml_num_children(pElem); ++i)
  {
    mre_xml_element_t const *pChild = policyman_xml_get_child(pElem, i);

    if (NULL == pChild)
    {
      continue;
    }

    if (policyman_xml_tag_is(pChild, DEV_CFG_STR_CONFIG))
    {
      /* read subphone config */
      pAction->parameterMask |= PARAMETER_MASK_CONFIG;
      status = policyman_device_config_read_cfg(pAction, pChild, nCfgs);
      ++nCfgs;
    }
    else if (policyman_xml_tag_is(pChild, DEV_CFG_STR_E911_CONFIG))
    {
      /* read e911 config: Maximum 2 configs are allowed */
      status = policyman_device_config_read_e911_config(pAction, pChild, nE911Cfgs);
      ++nE911Cfgs;
    }
    else
    {
      POLICYMAN_MSG_ERROR_1( "Device_configuration contains child other than 'config' or 'e911_config' at line %d",
                             mre_xml_get_line_number(pChild) );

      status = MRE_STATUS_ERR_MALFORMED_XML;
    }
  }

  return status;
}


/*-------- policyman_device_config_create_new_action --------*/
mre_status_t policyman_device_config_create_new_action(
  mre_xml_element_t const  *pElem,
  policyman_policy_t       *pPolicy,
  mre_action_t            **ppAction,
  pfn_execute_t             executefn
)
{
  mre_status_t             status         = MRE_STATUS_ERR_INVALID_ACTION;
  device_config_action_t  *pAction        = NULL;
  size_t                   nSims          = 1;
  size_t                   nMaxVoice      = 1;
  size_t                   nMaxData       = 1;
  uint32                   parameterMask  = 0;
  char const              *pStr;

  POLICYMAN_UT_MSG_0("--------------- <device_configuration> Parse the action");

  //  Get the number of SIM slots and simultaneously active subscriptions.
  if (!policyman_device_config_get_num_sims(&nSims, pElem))
  {
    goto Done;
  }

  if (policyman_device_config_get_num_active_voice(&nMaxVoice, pElem))
  {
    parameterMask |= PARAMETER_MASK_NMAXVOICE;

    /* Determine the maximum active voice subs ever possible */
    if (nMaxVoice > g_maxActiveVoiceSubs)
    {
      g_maxActiveVoiceSubs = nMaxVoice;
    }
  }

  if (policyman_device_config_get_num_active_data(&nMaxData, pElem))
  {
    parameterMask |= PARAMETER_MASK_NMAXDATA;
  }

  pAction = device_config_allocate_new_action( pElem,
                                               pPolicy,
                                               nSims );
  pAction->execute          = executefn;
  pAction->max_active_voice = nMaxVoice;
  pAction->max_active_data  = nMaxData;
  pAction->parameterMask    = parameterMask;

  status = device_config_read_e911_and_subphone_configs(pElem, pAction);

  // Search method lookup
  pStr = mre_xml_get_attribute(pElem, "method");
  pAction->lookup_method = policyman_util_str_to_lookup_method(pStr);

Done:
  if (MRE_FAILED(status))
  {
    POLICYMAN_MSG_ERROR_0("<device_configuration> read config failed");
    MRE_RELEASE_IF(pAction);
  }

  *ppAction = (mre_action_t *) pAction;

  return status;
}


/*-------- policyman_device_config_action_new --------*/
mre_status_t policyman_device_config_action_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t                   *pPolicy,
  mre_action_t                  **ppAction
)
{
  return policyman_device_config_create_new_action( pElem,
                                                    (policyman_policy_t *) pPolicy,
                                                    ppAction,
                                                    policyman_device_config_execute);
}


/*-------- policyman_device_config_if_action_new --------*/
mre_status_t policyman_device_config_if_action_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t                   *pPolicy,
  mre_action_t                  **ppAction
)
{
  return policyman_device_config_create_new_action( pElem,
                                                    (policyman_policy_t *) pPolicy,
                                                    ppAction,
                                                    policyman_device_config_if_execute);
}


/*=============================================================================
  <define_config>
=============================================================================*/


/*-------- named_device_configuration_t --------*/
typedef struct
{
  MRE_NAMED_OBJECT_BASE;

  device_configuration_t  *pDcItem;
} named_device_config_t;


/*-------- policyman_named_config_dtor --------*/
STATIC void policyman_named_config_dtor(
  void  *pObj
)
{
  named_device_config_t *pMe = (named_device_config_t *) pObj;

  MRE_RELEASE_IF(pMe->pDcItem);
  mre_named_object_dtor(pObj);
}


/*-------- policyman_device_config_define_execute --------*/
STATIC boolean policyman_device_config_define_execute(
  mre_action_t const  *pAction,
  void                *pCtx
)
{
  boolean                  succeeded;
  device_config_action_t  *pDcAction    = (device_config_action_t *)pAction;
  policyman_state_t       *pState       = POLICY_EXECUTE_STATE(pCtx);
  mre_set_t               *pItemSet     = POLICY_EXECUTE_ITEMSET(pCtx);
  mre_policy_t            *pPolicy      = EXECUTE_CTX_POLICY(pCtx);
  device_configuration_t  *pDcItem;
  named_device_config_t   *pNamedConfig;

  pDcItem = policyman_device_config_create(pDcAction, pState, pItemSet);

  pNamedConfig = MRE_NAMED_OBJECT_NEW( named_device_config_t,
                                       policyman_named_config_dtor,
                                       MRE_NAMED_CONFIG,
                                       pDcAction->pConfigName );

  pNamedConfig->pDcItem = pDcItem;

  succeeded = mre_named_object_add( (mre_named_object_t *)pNamedConfig,
                                     pDcAction->ns,
                                     pPolicy );

  ref_cnt_obj_release(pNamedConfig);

  if (succeeded)
  {
    POLICYMAN_MSG_HIGH_1(SRC_LINE "<define_config>: named configuration successfully created", LINE_NUM(pDcAction));
    POLICYMAN_UT_MSG_S1("   name = %s", pNamedConfig->pName);
  }

  return succeeded;
}


/*-------- policyman_device_config_define_new --------*/
mre_status_t policyman_device_config_define_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_action_t            **ppAction
)
{
  mre_status_t              status;
  device_config_action_t   *pAction;
  char const               *pStr;

  status = policyman_device_config_create_new_action( pElem,
                                                      (policyman_policy_t *)pPolicy,
                                                      ppAction,
                                                      policyman_device_config_define_execute);

  if (MRE_FAILED(status))
  {
    goto Done;
  }

  pStr = policyman_xml_get_attribute(pElem, "name");
  if (pStr == NULL)
  {
    POLICYMAN_MSG_ERROR_0("<define_config> requires a \"name\" attribute");
    status = MRE_STATUS_ERR_INVALID_ACTION;
    goto Done;
  }

  pAction = (device_config_action_t *) *ppAction;
  pAction->pConfigName = policyman_str_dup(pStr);
  pAction->ns = mre_named_object_get_namespace(pElem);

Done:
  if (MRE_FAILED(status))
  {
    MRE_RELEASE_IF(*ppAction);
  }

  return status;
}


/*=============================================================================
  <use_config>
=============================================================================*/

typedef struct
{
  POLICYMAN_ACTION_BASE;

  char const      *pConfigName;
  lookup_method_t  lookup_method;
} use_config_action_t;

/*-------- policyman_use_config_action_dtor --------*/
STATIC void policyman_use_config_action_dtor(
  void  *pObj
)
{
  use_config_action_t *pMe = (use_config_action_t *) pObj;

  POLICYMAN_MEM_FREE_IF(pMe->pConfigName);
  policyman_action_dtor(pObj);
}

/*-------- policyman_use_config_find_config --------*/
STATIC device_configuration_t *policyman_use_config_find_config(
  mre_action_t const  *pAction,
  mre_policy_t        *pPolicy
)
{
  use_config_action_t     *pCfgAct   = (use_config_action_t *)pAction;
  device_configuration_t  *pConfig   = NULL;
  named_device_config_t   *pNamedCfg;

  /*  If there is a name in the configuration item, look up the object
   */
  if (pCfgAct->pConfigName != NULL)
  {
    pNamedCfg = (named_device_config_t *) mre_named_object_find( pCfgAct->pConfigName,
                                                                 MRE_NAMED_CONFIG,
                                                                 pPolicy );

    if (pNamedCfg == NULL)
    {
      POLICYMAN_MSG_ERROR_0("<use_config>: unable to find named configuration");
      goto Done;
    }

    if (pNamedCfg->pDcItem == NULL)
    {
      POLICYMAN_MSG_ERROR_0("<use_config>: named configuration has no config item");
      goto Done;
    }

    pConfig = pNamedCfg->pDcItem;
  }

Done:
  return pConfig;
}

/*-------- policyman_use_config_copy_configs --------*/
STATIC void policyman_use_config_copy_configs(
  mre_action_t const     *pAction,
  mre_policy_t           *pPolicy,
  device_configuration_t *pDcItem
)
{
  device_configuration_t  *pConfig;

  pConfig = policyman_use_config_find_config(pAction, pPolicy);
  if (pConfig == NULL || pDcItem == NULL)
  {
    POLICYMAN_MSG_ERROR_0("<use_config>: no configuration to copy configs");
    return;
  }

  device_config_copy_configs((device_configuration_t const *)pConfig, pDcItem);
}

/*-------- policyman_device_config_use_execute --------*/
STATIC boolean policyman_device_config_use_execute(
  mre_action_t const  *pAction,
  void                *pCtx
)
{
  mre_set_t               *pItemSet     = POLICY_EXECUTE_ITEMSET(pCtx);
  mre_policy_t            *pPolicy      = EXECUTE_CTX_POLICY(pCtx);
  device_configuration_t  *pConfig;
  device_configuration_t  *pDcItem;
  boolean                  succeeded = FALSE;

  pConfig = policyman_use_config_find_config(pAction, pPolicy);

  if (pConfig == NULL)
  {
    POLICYMAN_MSG_ERROR_1(SRC_LINE "<use_config>: no configuration", LINE_NUM(pAction));
    goto Done;
  }

  /* ensure subphone and e911 configs are filled properly in named config */
  policyman_check_and_update_configurations(pConfig);

  /* create a new device config with copying all configs and e911 configs  */
  pDcItem = policyman_device_config_item_new(SYS_MODEM_AS_ID_1);
  device_config_copy_parameters(pConfig, pDcItem);

  mre_set_replace(pItemSet, &pDcItem);

  // update all device globals here
  g_num_Sim            = pDcItem->num_sims;
  g_maxActiveVoiceSubs = pDcItem->max_active_voice;
  
  POLICYMAN_MSG_HIGH_4( SRC_LINE "<use_config>: configuration with num_sims %d, max_active_voice %d, max_active_data %d",
                        LINE_NUM(pAction),
                        pDcItem->num_sims,
                        pDcItem->max_active_voice,
                        pDcItem->max_active_data );

  ref_cnt_obj_release(pDcItem);
  succeeded = TRUE;

Done:
  return succeeded;
}

/*-------- policyman_device_config_use_if_execute --------*/
STATIC boolean policyman_device_config_use_if_execute(
  mre_action_t const  *pAction,
  void                *pCtx
)
{
  use_config_action_t     *pUAction     = (use_config_action_t *)pAction;
  mre_set_t               *pItemSet     = POLICY_EXECUTE_ITEMSET(pCtx);
  mre_policy_t            *pPolicy      = EXECUTE_CTX_POLICY(pCtx);
  boolean                  succeeded    = TRUE;
  device_configuration_t  *pDevCfg;
  boolean                  item_is_present;

  item_is_present = policyman_cfgitem_find_item_based_on_lookup_method( pUAction->lookup_method,
                                                                        pItemSet,
                                                                        POLICYMAN_ITEM_DEVICE_CONFIGURATION,
                                                                        SYS_MODEM_AS_ID_1,
                                                                        (policyman_item_t **)&pDevCfg);

  if (item_is_present)
  {
    POLICYMAN_UT_MSG_1(SRC_LINE "<use_config_if> item already present, copy configs", LINE_NUM(pAction));
    policyman_use_config_copy_configs(pAction, pPolicy, pDevCfg);
    ref_cnt_obj_release(pDevCfg);
  }
  else
  {
    succeeded = policyman_device_config_use_execute(pAction, pCtx);
  }

  return succeeded;
}


/*-------- policyman_device_config_use_create --------*/
mre_status_t policyman_device_config_use_create(
  mre_xml_element_t const  *pElem,
  policyman_policy_t             *pPolicy,
  mre_action_t                  **ppAction,
  pfn_execute_t                   executefn
)
{
  mre_status_t         status = MRE_STATUS_ERR_INVALID_ACTION;
  char const          *pStr;
  use_config_action_t *pAction;

  pStr = policyman_xml_get_attribute(pElem, "name");
  if (pStr == NULL)
  {
    POLICYMAN_MSG_ERROR_0("<use_config> requires a \"name\" attribute");
    goto Done;
  }

  pAction = POLICYMAN_ACTION_NEW( use_config_action_t,
                                  (mre_policy_t *)pPolicy,
                                  executefn,
                                  policyman_use_config_action_dtor,
                                  SUBS_THIS,
                                  pElem );

  pAction->pConfigName = policyman_str_dup(pStr);

  // Search method lookup
  pStr = policyman_xml_get_attribute(pElem, "method");
  pAction->lookup_method = policyman_util_str_to_lookup_method(pStr);

  *ppAction = (mre_action_t *) pAction;
  status = MRE_STATUS_SUCCESS;

Done:
  return status;
}


/*-------- policyman_device_config_use_new --------*/
mre_status_t policyman_device_config_use_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t                  *pPolicy,
  mre_action_t            **ppAction
)
{
  return policyman_device_config_use_create( pElem,
                                             (policyman_policy_t *) pPolicy,
                                             ppAction,
                                             policyman_device_config_use_execute );
}


/*-------- policyman_device_config_use_if_new --------*/
mre_status_t policyman_device_config_use_if_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t                  *pPolicy,
  mre_action_t            **ppAction
)
{
  return policyman_device_config_use_create( pElem,
                                             (policyman_policy_t *) pPolicy,
                                             ppAction,
                                             policyman_device_config_use_if_execute );
}


/*=============================================================================
  <feature>

  NOTE:
  The <feature> action is only supported as part of the <initial> block, and
  sets the global feature. This doesn't result in a device_configuration item
  change, because that is not expected to change.
=============================================================================*/


typedef struct
{
  POLICYMAN_ACTION_BASE;

  boolean               ssFeatureIsPresent;
  sys_subs_feature_t    feature;
  sys_subs_feature_t    singleSimFeature;
} feature_action_t;

/*-------- adjust_feature_mode --------*/
STATIC sys_subs_feature_t adjust_feature_mode(
  sys_modem_as_id_e_type   subsId,
  sys_subs_feature_t       subsFeature
)
{
  if (  !policyman_rf_hw_capability_supports_cdma()
     && (subsFeature == SYS_SUBS_FEATURE_MODE_SRLTE || subsFeature == SYS_SUBS_FEATURE_MODE_DUAL_MULTIMODE)
     )
  {
    POLICYMAN_MSG_HIGH_2( SUBS_PREFIX "No cdma support, subs feature changed from %d to NORMAL",
                          subsId,
                          subsFeature );

    subsFeature = SYS_SUBS_FEATURE_MODE_NORMAL;
  }

  return subsFeature;
}

/*-------- execute_feature_mode --------*/
STATIC boolean execute_feature_mode(
  policyman_action_t const     *pAction,
  void                         *pCtx,
  sys_modem_as_id_e_type        subsId
)
{
  mre_set_t               *pItemSet  = POLICY_EXECUTE_ITEMSET(pCtx);
  device_configuration_t  *pDcItem;

  /* We are reading feature here means it is not set in <device_config> item definition
        Update the SIM1 config with the feature if we have the subphone configs array.  */

  pDcItem = policyman_get_device_config(pItemSet);

  if (pDcItem != NULL)
  {
    size_t              cfgIndex;
    feature_action_t   *pfAction = (feature_action_t *)pAction;

    POLICYMAN_MSG_HIGH_4( SUBS_PREFIX SRC_LINE "updating feature to %d (ss feature %d)",
                          subsId,
                          LINE_NUM(pfAction),
                          pfAction->feature,
                          (pfAction->ssFeatureIsPresent ? pfAction->singleSimFeature : -1) );

    for (cfgIndex = 0; cfgIndex < pDcItem->num_cfgs; cfgIndex++)
    {
      size_t index;

      index = device_config_get_subphone_index(pDcItem->num_sims, cfgIndex, subsId);

      pDcItem->pConfigs[index].feature = adjust_feature_mode(subsId, pfAction->feature);

      // overwrite feature mode with single SIM feature if present
      if (pDcItem->num_sims == 1 && pfAction->ssFeatureIsPresent)
      {
        pDcItem->pConfigs[index].feature = adjust_feature_mode(subsId, pfAction->singleSimFeature);
      }
    }
  }

  REF_CNT_OBJ_RELEASE_IF(pDcItem);

  return TRUE;
}

/*-------- policyman_device_config_feature_execute --------*/
STATIC boolean policyman_device_config_feature_execute(
  mre_action_t const        *pAction,
  void                      *pCtx
)
{
  policyman_action_t const *pAct = (policyman_action_t *)pAction;

  return policyman_execute_action(pAct, pCtx, execute_feature_mode);
}


/*-------- policyman_device_config_feature_new --------*/
mre_status_t policyman_device_config_feature_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t                   *pPolicy,
  mre_action_t                  **ppAction
)
{
  mre_status_t           status = MRE_STATUS_ERR_MALFORMED_XML;
  char const            *pStr;
  sys_subs_feature_t     feature;
  feature_action_t      *pAction = NULL;
  policyman_subs_t       pm_subs;


  pStr = policyman_xml_get_text(pElem);
  if (!policyman_device_config_get_feature(pStr, &feature))
  {
    goto Done;
  }

  status = policyman_util_get_subs( pElem, POLICY_SUBS(pPolicy), &pm_subs);
  if (MRE_FAILED(status))
  {
    goto Done;
  }

  pAction = POLICYMAN_ACTION_NEW( feature_action_t,
                                  pPolicy,
                                  policyman_device_config_feature_execute,
                                  mre_action_dtor,
                                  pm_subs,
                                  pElem );

  pAction->feature          = feature;
  pAction->singleSimFeature = SYS_SUBS_FEATURE_MODE_NORMAL;

  // read single sim feature
  pStr = policyman_xml_get_attribute(pElem, "single_sim");
  if (pStr != NULL)
  {
    pAction->ssFeatureIsPresent = policyman_device_config_get_feature(pStr, &pAction->singleSimFeature);
  }

  status = MRE_STATUS_SUCCESS;

Done:
  *ppAction = (mre_action_t *) pAction;
  return status;
}


/*=============================================================================
  <select_config>
=============================================================================*/


typedef struct
{
  POLICYMAN_ACTION_BASE;

  int  cfgIndex;
} select_config_action_t;

/*-------- policyman_device_config_select_config_execute --------*/
STATIC boolean policyman_device_config_select_config_execute(
  mre_action_t const  *pAction,
  void                *pCtx
)
{
  select_config_action_t  *pScAction  = (select_config_action_t *)pAction;
  size_t                   numCfgs;

  numCfgs = device_config_get_num_configs();

  if (pScAction->cfgIndex < (int)numCfgs)
  {
    policyman_state_t       *pState       = POLICY_EXECUTE_STATE(pCtx);
    mre_set_t               *pItemSet     = POLICY_EXECUTE_ITEMSET(pCtx);
    sys_modem_as_id_e_type   lteSubs;

    // Retrieve which subs has LTE, so it remains unchanged if explicit index is -1
    lteSubs = policyman_device_config_determine_lte_subs(pState, pItemSet);

    set_explicit_index(pState, pItemSet, lteSubs, pScAction->cfgIndex);

    POLICYMAN_MSG_HIGH_2( SRC_LINE "setting explicit device config index to %d",
                          LINE_NUM(pScAction),
                          pScAction->cfgIndex );
  }
  else
  {
    POLICYMAN_MSG_ERROR_3( SRC_LINE "explicit index %d exceeds number of configs (%d)",
                           LINE_NUM(pScAction),
                           pScAction->cfgIndex,
                           numCfgs );
  }

  return TRUE;
}

/*-------- policyman_device_config_select_config_new --------*/
mre_status_t policyman_device_config_select_config_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t                   *pPolicy,
  mre_action_t                  **ppAction
)
{
  char const              *pStr;
  int                      cfgIndex;
  select_config_action_t  *pAction     = NULL;
  mre_status_t             status      = MRE_STATUS_ERR_MALFORMED_XML;

  pStr = policyman_xml_get_attribute(pElem, "index");
  if (pStr == NULL)
  {
    POLICYMAN_MSG_ERROR_0("<select_config> requires a \"index\" attribute");
    goto Done;
  }

  cfgIndex = atoi(pStr);

  pAction = POLICYMAN_ACTION_NEW( select_config_action_t,
                                  pPolicy,
                                  policyman_device_config_select_config_execute,
                                  policyman_action_dtor,
                                  SUBS_THIS,
                                  pElem );

  pAction->cfgIndex = cfgIndex;

  status = MRE_STATUS_SUCCESS;

Done:
  *ppAction = (mre_action_t *) pAction;
  return status;
}

/*-----------------------------------------------------------------------------
  <imei> condition
-----------------------------------------------------------------------------*/


/*-------- policyman_imei_condition_t --------*/
typedef struct
{
  POLICYMAN_CONDITION_BASE;

  mre_regex_t *pRegEx;
} policyman_imei_condition_t;


/*-------- policyman_condition_imei_dtor --------*/
STATIC void policyman_condition_imei_dtor(
  void  *pObj
)
{
  policyman_imei_condition_t *pCond = (policyman_imei_condition_t *) pObj;

  MRE_RELEASE_IF(pCond->pRegEx);

  policyman_condition_dtor(pObj);
}


/*-------- policyman_device_imei_print --------*/
void policyman_device_imei_print(
  byte  *pImei
)
{
  POLICYMAN_MSG_HIGH_9( "Device IMEI:  %02x %02x %02x %02x %02x %02x %02x %02x %02x",
                        pImei[0], pImei[1], pImei[2],
                        pImei[3], pImei[4], pImei[5],
                        pImei[6], pImei[7], pImei[8] );
}


/*-------- policyman_condition_imei_evaluate --------*/
STATIC boolean policyman_condition_imei_evaluate(
  mre_condition_t const *pCondition,
  void                  *pCtx
)
{
  policyman_imei_condition_t  *pCond        = (policyman_imei_condition_t*)pCondition;
  mre_policy_t                *pPolicy  = EXECUTE_CTX_POLICY(pCtx);
  byte                        *rawIMEI      = policyman_state_get_raw_imei();
  char const                  *pIMEI        = policyman_state_get_imei();
  boolean                     result        = FALSE;

  policyman_device_imei_print(rawIMEI);
  POLICYMAN_UT_MSG_S1("IMEI: %s", pIMEI);

  result =      pIMEI != NULL
            &&  mre_regex_match(pCond->pRegEx, pIMEI);

  POLICYMAN_UT_MSG_3( SUBS_PREFIX SRC_LINE "condition <imei> with above device IMEI returns %d",
                      POLICY_SUBS(pPolicy),
                      LINE_NUM(pCond),
                      result );

  return result;
}


/*-------- policyman_condition_imei_new --------*/
mre_status_t policyman_condition_imei_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t                   *pPolicy,
  mre_condition_t               **ppCondition
)
{
  policyman_status_t          status = POLICYMAN_STATUS_ERR_INVALID_CONDITION;
  policyman_imei_condition_t  *pCondition = NULL;
  char const                  *pStr;
  mre_regex_t                 *pRegEx;

  pStr = policyman_xml_get_attribute(pElem, "regex");
  if (pStr == NULL)
  {
    POLICYMAN_UT_MSG_0("<imei>: must specify regex attribute");
    goto Done;
  }

  pRegEx = mre_regex_create(pStr);
  if (pRegEx == NULL)
  {
    POLICYMAN_UT_MSG_S1("<imei>: invalid regex: %s", pStr);
    goto Done;
  }

  /*  Allocate the condition
   */
  pCondition = POLICYMAN_CONDITION_NEW( policyman_imei_condition_t,
                                        pPolicy,
                                        policyman_condition_imei_evaluate,
                                        policyman_condition_imei_dtor,
                                        SUBS_THIS,
                                        pElem );

  pCondition->pRegEx = pRegEx;
  status = POLICYMAN_STATUS_SUCCESS;

Done:
  *ppCondition = (mre_condition_t *) pCondition;

  return status;
}

/*-------- policyman_device_config_allocate_and_init_diag_cfg_log --------*/
STATIC LOG_PM_CONFIG_INFO_type * policyman_device_config_allocate_and_init_diag_cfg_log(
  size_t    num_cfgs
)
{
  LOG_PM_CONFIG_INFO_type   *log_ptr;

  log_ptr  = LOG_PACKET_NEW( LOG_PM_CONFIG_INFO_C,
                             LOG_PM_CONFIG_INFO_type);

  if (NULL != log_ptr)
  {
    log_ptr->version      = LOG_PM_CONFIG_INFO_LOG_PACKET_VERSION;

    memset(&log_ptr->subs_cfg_info, 0, sizeof(log_ptr->subs_cfg_info) );
  }

  return log_ptr;
}

/*-------- policyman_device_config_fill_overall_info --------*/
STATIC void policyman_device_config_fill_overall_info(
  LOG_PM_CONFIG_INFO_type   *log_ptr
)
{
  policyman_rf_device_info_t    *pOverall;

  pOverall = policyman_rf_get_overall_info();

  if (NULL != pOverall)
  {
    log_ptr->hw_rat_mask  = pOverall->ratMask;
    log_ptr->hw_gw_bands  = pOverall->gwBands;
    log_ptr->lte_band_count = sizeof(sys_lte_band_mask_e_type) / sizeof(uint64);
#if defined(LTE_BAND_NUM) && (LTE_BAND_NUM == 256)
    log_ptr->hw_lte_bands_0 = pOverall->lteBands.bits_1_64;
    log_ptr->hw_lte_bands_1 = pOverall->lteBands.bits_65_128;
    log_ptr->hw_lte_bands_2 = pOverall->lteBands.bits_129_192;
    log_ptr->hw_lte_bands_3 = pOverall->lteBands.bits_193_256;
#else
    log_ptr->hw_lte_bands_0 = pOverall->lteBands;
    log_ptr->hw_lte_bands_1 = 0;
    log_ptr->hw_lte_bands_2 = 0;
    log_ptr->hw_lte_bands_3 = 0;
#endif
    log_ptr->hw_tds_bands = pOverall->tdsBands;
  }
  else
  {
    log_ptr->hw_rat_mask  = 0;
    log_ptr->hw_gw_bands  = 0;
    log_ptr->hw_lte_bands_0 = 0;
    log_ptr->hw_lte_bands_1 = 0;
    log_ptr->hw_lte_bands_2 = 0;
    log_ptr->hw_lte_bands_3 = 0;
    log_ptr->hw_tds_bands = 0;
  }
}

/*-------- policyman_device_config_fill_config_info --------*/
STATIC void policyman_device_config_fill_config_info(
  device_configuration_t    *pDcItem,
  LOG_PM_CONFIG_INFO_type   *log_ptr
)
{
  size_t   cfgIdx;
  size_t   maxIdx;

  maxIdx = (pDcItem->num_cfgs > ARR_SIZE(log_ptr->subs_cfg_info)) ? ARR_SIZE(log_ptr->subs_cfg_info)
                                                              : pDcItem->num_cfgs;

  for (cfgIdx = 0; cfgIdx < maxIdx; ++cfgIdx)
  {
    log_ptr->subs_cfg_info[cfgIdx].subs_feature = (uint8)pDcItem->pConfigs[cfgIdx].feature;
    log_ptr->subs_cfg_info[cfgIdx].reserved_1   = 0;
    log_ptr->subs_cfg_info[cfgIdx].reserved_2   = 0;
    log_ptr->subs_cfg_info[cfgIdx].reserved_3   = 0;
    log_ptr->subs_cfg_info[cfgIdx].rat_info     = pDcItem->pConfigs[cfgIdx].ratInfo;
  }
}

/*-------- policyman_device_config_fill_diag_cfg_log --------*/
void policyman_device_config_fill_diag_cfg_log(
  device_configuration_t    *pDcItem,
  LOG_PM_CONFIG_INFO_type   *log_ptr
)
{
  sys_overall_feature_t    feature;
  policyman_device_config_overall_feature((policyman_item_t *)pDcItem, &feature);

  log_ptr->version         = LOG_PM_CONFIG_INFO_LOG_PACKET_VERSION;
  log_ptr->changed_mask    = 0xFFFFFFFF;
  log_ptr->num_sim_cfg     = (pDcItem->num_cfgs & 0x0f) << 4;
  log_ptr->num_sim_cfg    |= (pDcItem->num_sims & 0x0f);
  log_ptr->max_active      = (pDcItem->max_active_data  & 0x0f) << 4;
  log_ptr->max_active     |= (pDcItem->max_active_voice & 0x0f);
  log_ptr->modified        = (uint8)pDcItem->modified;
  log_ptr->config_index    = (uint8)pDcItem->current_cfg_index;
  log_ptr->explicit_index  = pDcItem->explicit_index;
  log_ptr->overall_feature = (uint8) feature;
}

/*-------- policyman_device_config_create_and_send_diag_cfg_log --------*/
void policyman_device_config_create_and_send_diag_cfg_log(
  policyman_item_t  *pCfgItem,
  boolean            addSetToHistory,
  int                notifyHistoryIndex
)
{
  device_configuration_t    *pDcItem;
  LOG_PM_CONFIG_INFO_type   *log_ptr;

  pDcItem = (device_configuration_t *)pCfgItem;

  log_ptr = policyman_device_config_allocate_and_init_diag_cfg_log(pDcItem->num_cfgs);

  if (NULL != log_ptr)
  {
    policyman_device_config_fill_diag_cfg_log(pDcItem, log_ptr);

    policyman_device_config_fill_overall_info(log_ptr);

    policyman_device_config_fill_config_info(pDcItem, log_ptr);

    if (addSetToHistory)
    {
      policyman_cfgitem_update_notify_history_cfg(log_ptr, notifyHistoryIndex);
    }

    // Submit the log to DIAG
    log_commit( (void *)log_ptr );
  }
}

/*-------- policyman_device_configuration_get_limited_item --------*/
void policyman_device_configuration_get_limited_item(
  mre_set_t             *pItemSet,
  sys_modem_as_id_e_type subsId
)
{
  policyman_item_t     *pDc;

  pDc = policyman_device_config_get_default(pItemSet, subsId);

  mre_set_add(pItemSet, &pDc);
  ref_cnt_obj_release(pDc);
}

/*-----------------------------------------------------------------------------
  Subphone Cap Has
-----------------------------------------------------------------------------*/

typedef struct
{
  POLICYMAN_CONDITION_BASE;

  uint32  mask;
} policyman_subphone_cap_has_condition_t;



/*-------- evaluate_subphone_cap_has --------*/
static boolean evaluate_subphone_cap_has(
  policyman_condition_t const *pCondition,
  void                        *pCtx,
  sys_modem_as_id_e_type       subsId
)
{
  policyman_subphone_cap_has_condition_t  const *pCond    = (policyman_subphone_cap_has_condition_t *) pCondition;
  mre_policy_t                                  *pPolicy  = EXECUTE_CTX_POLICY(pCtx);
  boolean                                        hasRats  = FALSE;
  policyman_item_t                              *pItem    = NULL;
  policyman_status_t                             status;
  uint32                                         ratCap = 0;

  status = policyman_get_current_device_config((policyman_item_t **)&pItem);

  if (POLICYMAN_SUCCEEDED(status))
  {
    (void)policyman_device_config_get_current_subphone_capability(pItem, subsId, &ratCap);
    hasRats = ((ratCap & pCond->mask) == pCond->mask);
  }

  POLICYMAN_UT_MSG_6( SUBS_PREFIX SRC_LINE "condition <subphone_cap_has pmsubs=%d> ratCap=%d pCond->mask=%d returns %d",
                      POLICY_SUBS(pPolicy),
                      LINE_NUM(pCond),
                      subsId,
                      ratCap,
                      pCond->mask,
                      hasRats);

  return hasRats;
}


/*-------- boolean policyman_condition_subphone_cap_has_evaluate --------*/
static boolean policyman_condition_subphone_cap_has_evaluate(
  mre_condition_t const  *pCondition,
  void                   *pCtx
)
{
  policyman_condition_t const * pCond = (policyman_condition_t const *)pCondition;

  return policyman_evaluate_cond(pCond, pCtx, evaluate_subphone_cap_has);
}


/*-------- policyman_condition_subphone_cap_has_new --------*/
mre_status_t policyman_condition_subphone_cap_has_new(
  mre_xml_element_t const   *pElem,
  mre_policy_t              *pPolicy,
  mre_condition_t          **ppCondition
)
{
  mre_status_t                            status     = MRE_STATUS_ERR_MALFORMED_XML;
  policyman_subphone_cap_has_condition_t *pCondition;
  char const                             *pStr;
  uint32                                  mask       = 0;
  policyman_subs_t                        pm_subs;

  status = policyman_util_get_subs( pElem, POLICY_SUBS(pPolicy), &pm_subs);

  if (MRE_FAILED(status))
  {
    goto Done;
  }

  pStr = policyman_xml_get_text(pElem);

  if (pStr == NULL)
  {
    POLICYMAN_UT_MSG_1(SUBS_PREFIX "<subphone_cap_has> must specify RATs", POLICY_SUBS(pPolicy));
    goto Done;
  }

  if (!policyman_rat_config_parse_rats(pStr, &mask))
  {
    POLICYMAN_UT_MSG_1(SUBS_PREFIX "<subphone_cap_has> invalid RAT", POLICY_SUBS(pPolicy));
    goto Done;
  }

  pCondition = POLICYMAN_CONDITION_NEW( policyman_subphone_cap_has_condition_t,
                                        pPolicy,
                                        policyman_condition_subphone_cap_has_evaluate,
                                        policyman_condition_dtor,
                                        pm_subs,
                                        pElem );

  pCondition->mask      = mask;
  pCondition->pmSubsId  = pm_subs;

  *ppCondition = (mre_condition_t *) pCondition;
  status = MRE_STATUS_SUCCESS;

Done:
  return status;
}

/*-------- policyman_device_config_mdump --------*/
QSH_MDUMP_FN_ATTR_PM void policyman_device_config_mdump(
  policyman_item_t const *pItem
)
{  
  policyman_qsh_dump_collect((void*)pItem, sizeof(device_configuration_t));
}


/*-------- policyman_config_is_supported_by_hardware --------*/
boolean policyman_config_is_supported_by_hardware(
  void
)
{
  boolean                  is_supported = TRUE;

#if !defined(FEATURE_DUAL_SIM)
  if (g_num_Sim != 1)
  {
    is_supported = FALSE;
  }
#endif

  return is_supported;
}
