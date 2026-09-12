#ifndef _POLICYMAN_RAT_CAPABILITY_H_
#define _POLICYMAN_RAT_CAPABILITY_H_

/**
  @file policyman_rat_capability.h

  @brief
*/

/*
    Copyright (c) 2013-2017 QUALCOMM Technologies Incorporated.
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

  $Header: //components/rel/mmcp.mpss/7.9.0/policyman/inc/policyman_rat_capability.h#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/

#include "policyman_cfgitem.h"
#include "policyman_diag.h"
#include "policyman_rf.h"
#include "policyman_rules.h"

#define PM_EXCLUDE_LTE_FOR_TEMP_DDS  "pm:exclude_lte_for_temp_dds"

typedef struct policyman_policy_rat_capability_t  policyman_policy_rat_capability_t;

#define PM_ALLOW_TDS_ON_THIS_SUB "pm:allow_tds_on_this_sub"

/*-------- policyman_policy_rat_capability --------*/
/**
@brief  The capability policy.
*/
extern  policyman_policy_rat_capability_t policyman_policy_rat_capability;



/*-------- policyman_rat_config_parse_rats --------*/
/**
@brief  Utility function to turn a string of RATs into a mask.

@param

@return

*/
boolean policyman_rat_config_parse_rats(
  char const  *pStr,
  uint32      *pMask
);


/*=============================================================================
  RAT capability APIs
=============================================================================*/


/*-------- rat_capability_info_t --------*/
/**
@brief  Structure to store information about how a rat capability is
        to be computed.
*/
typedef struct
{
  policyman_base_t  base;
  uint32            includeMask;
  uint32            excludeMask;
} rat_capability_info_t;



/*-------- rat_capability_t --------*/
/**
@brief  Configuration item structure for rat_capability.
*/
typedef struct
{
  POLICYMAN_ITEM;

  uint32  ratMask;

  uint32  subphoneCap;
  uint32  ratIncludeMask;
  uint32  ratExcludeMask;
  boolean removeLteWhenNotUseful;
} rat_capability_t;

/* Structure version info */
#define POLICYMAN_RAT_CAPABILITY_VERSION_TAG      0xBDCA0000
#define POLICYMAN_RAT_CAPABILITY_VERSION_MASK     0x0000FFFF
#define POLICYMAN_RAT_CAPABILITY_VERSION_NUM      1
#define POLICYMAN_RAT_CAPABILITY_MAX_RESTRICTIONS 5

typedef struct
{
  sys_band_mask_type        gw_band_mask;
  sys_lte_band_mask_e_type  lte_band_mask;
  sys_band_mask_type        tds_band_mask;
  uint32                    rat_mask;
  sys_band_mask_type        gw_bst_mask;
  sys_lte_band_mask_e_type  lte_bst_mask;
  sys_band_mask_type        tds_bst_mask;
} rat_capability_efs_t;

/* Structures used to read/write to EFS config item */
typedef struct
{
  size_t                    version;
  sys_band_mask_type        gw_band_mask;
  sys_lte_band_mask_e_type  lte_band_mask;
  sys_band_mask_type        tds_band_mask;
  uint32                    rat_mask;
  sys_band_mask_type        gw_bst_mask;
  sys_lte_band_mask_e_type  lte_bst_mask;
  sys_band_mask_type        tds_bst_mask;

  size_t                    num_policy_restrictions;
  rf_restrictions_t         rfRestrictions[POLICYMAN_RAT_CAPABILITY_MAX_RESTRICTIONS];
} rat_capability_efs_v1_t;



/*-------- policyman_rat_capability_clone --------*/
policyman_item_t * policyman_rat_capability_clone(
  policyman_item_t const *pItem
);


/*-------- policyman_rat_capability_item_new --------*/
/**
@brief

@param

@return

*/
policyman_item_t* policyman_rat_capability_item_new(
  mre_set_t              *pItemSet,
  sys_modem_as_id_e_type  subsId,
  uint32                  mask
);

/*-------- policyman_rat_capability_get_default_from_efs --------*/
/**
@brief

@param

@return

*/
policyman_status_t policyman_rat_capability_get_default_from_efs(
  cfgitem_execute_ctx_t *pCfgCtx
);


/*-------- policyman_rat_capability_get_default --------*/
/**
@brief

@param

@return

*/
policyman_item_t* policyman_rat_capability_get_default(
  mre_set_t              *pItemSet, 
  sys_modem_as_id_e_type  subsId
);


/*-------- policyman_rat_capability_compare --------*/
/**
@brief

@param

@return

*/
boolean policyman_rat_capability_compare(
  policyman_item_t  *pData1,
  policyman_item_t  *pData2
);



/*-------- policyman_rat_capability_read_info --------*/
/**
@brief

@param

@return

*/
policyman_status_t policyman_rat_capability_read_info(
  mre_xml_element_t const       *pElem,
  rat_capability_info_t         *pInfo
);



/*-------- policyman_rat_capability_evaluate --------*/
/**
@brief

@param

@return

*/
uint32 policyman_rat_capability_evaluate(
  rat_capability_info_t *pInfo,
  mre_set_t               *pItemSet,
  sys_modem_as_id_e_type   subsId
);


/*-------- policyman_rat_capability_action_new --------*/
/**
@brief  API to create a new policyman action for the rat_capability
        action in the policy_info.

@param[in]  pElem     Pointer to the XML document element for the
                      <rat_capability> node.
@param[out] ppAction  Pointer to storage for a pointer to the policyman_action_t
                      object created from the XML.

@return
  Status
*/
mre_status_t policyman_rat_capability_action_new(
  mre_xml_element_t const        *pElem,
  mre_policy_t                   *pPolicy,
  mre_action_t                  **ppAction
);

/*-------- policyman_rat_capability_if_action_new --------*/
mre_status_t policyman_rat_capability_if_action_new(
  mre_xml_element_t const        *pElem,
  mre_policy_t                   *pPolicy,
  mre_action_t                  **ppAction
);


/*-------- policyman_rat_capability_display --------*/
void policyman_rat_capability_display(
  policyman_item_t  *pItem
);

/*-------- policyman_rat_capability_on_update --------*/
void policyman_rat_capability_on_update(
  policyman_item_t  *pItem
);

/*-------- policyman_get_rats_for_ueMode --------*/
rat_capability_t * policyman_get_rats_for_ueMode(
  sys_ue_mode_e_type current_ue_mode
);


/*-------- policyman_rat_capability_str_to_rat --------*/
/**
@brief  Convert Rat capability string to rat mask

@param

@return

*/
uint32 policyman_rat_capability_str_to_rat(
  const char  *pRatName
);


/*-------- policyman_update_rats_bands_to_efs --------*/
/**
@brief  Update Rats and Bands to EFS file

@param

@return

*/
void policyman_update_rats_bands_to_efs(
  uint8 subsMask
);

/*-------- policyman_update_rats_bands_to_efs_per_tech --------*/
/**
@brief  Update Rats and Bands to EFS file

@param

@return

*/
void policyman_update_rats_bands_to_efs_per_tech(
  uint32  rat_include
);

/*-------- policyman_rat_capability_fill_subs_diag_log --------*/
void policyman_rat_capability_fill_subs_diag_log(
  policyman_item_t const  *pItem,
  LOG_PM_SUBS_INFO_type   *log_ptr
);

/*-------- policyman_rat_capability_get_max_config --------*/
uint32 policyman_rat_capability_get_max_config(
  mre_set_t              *pItemSet,
  sys_modem_as_id_e_type  subsId
);

/*-------- policyman_get_rat_capability_for_subs --------*/
policyman_status_t policyman_get_rat_capability_for_subs(
  sys_modem_as_id_e_type  subsId,
  uint32                 *pRatMask
);

/*-------- policyman_set_rat_capability_concurrency_masks --------*/
void policyman_set_rat_capability_concurrency_masks(
  policyman_state_t *pState,
  mre_set_t         *pItemSet
);

/*-------- policyman_rat_capability_get_limited_item --------*/
void policyman_rat_capability_get_limited_item(
  mre_set_t              *pItemSet,
  sys_modem_as_id_e_type  subsId
);

/*-------- policyman_rat_capability_mdump --------*/
void policyman_rat_capability_mdump(
  policyman_item_t const *pItem
);
#endif /* _POLICYMAN_RAT_CAPABILITY_H_ */
