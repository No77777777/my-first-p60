/**
  @file policyman_util.c

  @brief Policy Manager utility functions.
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

  $Header: //components/rel/mmcp.mpss/7.9.0/policyman/src/policyman_util.c#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/

#include "policyman_util.h"
#include "policyman_dbg.h"
#include "policyman_rf.h"
#include "mre_xml.h"

#include "modem_mem.h"
#include <stringl/stringl.h>



/*=============================================================================
  XML parsing helpers
=============================================================================*/


/*-------- policyman_util_get_base --------*/
boolean policyman_util_get_base(
  mre_xml_element_t const       *pElem,
  policyman_base_t              *pBase
)
{
  static MRE_STR_VALUE_TABLE(map) =
  {
    MRE_STR_VALUE_ENTRY("none",      POLICYMAN_BASE_NONE),
    MRE_STR_VALUE_ENTRY("config",    POLICYMAN_BASE_CONFIG),
    MRE_STR_VALUE_ENTRY("hardware",  POLICYMAN_BASE_HARDWARE),
    MRE_STR_VALUE_ENTRY("current",   POLICYMAN_BASE_CURRENT),
  };

  boolean      succeeded = FALSE;
  char const  *pStr;

  pStr = mre_xml_get_attribute(pElem, "base");

  if (NON_NULL(pStr))
  {
    mre_status_t  status;

    MRE_LOOKUP_STR_VALUE(map, pStr, policyman_base_t, pBase);
    if (MRE_SUCCEEDED(status))
    {
      succeeded = TRUE;
    }
    else
    {
      POLICYMAN_UT_MSG_1("invalid 'base' specified: %s", pStr);
    }
  }

  return succeeded;
}

/*-------- policyman_util_get_subs --------*/
mre_status_t policyman_util_get_subs(
  mre_xml_element_t const       *pElem,
  sys_modem_as_id_e_type         policySubs,
  policyman_subs_t              *pSubs
)
{
  mre_status_t      status = MRE_STATUS_SUCCESS;
  char const       *pStr;
  size_t            subsId;
  policyman_subs_t  executeSubs;

  pStr = mre_xml_get_attribute(pElem, "subs");

  if (pStr == NULL)
  {
    *pSubs = SUBS_THIS;
  }
  else
  {
    executeSubs = policyman_str_to_subs(pStr);

    /* New "this" or "other" subs value */
    if (executeSubs != SUBS_NONE)
    {
      *pSubs = executeSubs;
    }
    else
    {
      subsId = atoi(pStr);
      if (subsId <= POLICYMAN_MAX_SUB)
      {
        *pSubs = ((subsId - 1) == policySubs) ? SUBS_THIS : SUBS_OTHER;
      }
      else
      {
        POLICYMAN_MSG_ERROR_3( SUBS_PREFIX "element has a subs id of %d, but there are only %d subs allowed",
                               policySubs,
                               subsId,
                               POLICYMAN_MAX_SUB );

        status = MRE_STATUS_ERR_MALFORMED_XML;
      }
    }
  }

  return status;
}

/*-------- policyman_check_stack_is_valid --------*/
boolean policyman_check_stack_is_valid(
  sys_modem_stack_id_e_type stack
)
{

  return    stack == SYS_MODEM_STACK_ID_1
         || stack == SYS_MODEM_STACK_ID_2;
}

/*-------- policyman_util_get_rats_from_bands --------*/
void policyman_util_get_rats_from_bands(
  cm_band_pref_e_type        gwBands,
  sys_lte_band_mask_e_type   lteBands,
  cm_band_pref_e_type        tdsBands,
  uint32                    *pratMask
)
{
  if (NULL != pratMask)
  {
    *pratMask = 0;

    if (gwBands & ALL_GSM_BANDS)
    {
      *pratMask |= SYS_SYS_MODE_MASK_GSM;
    }

    if (gwBands & ALL_WCDMA_BANDS)
    {
      *pratMask |= SYS_SYS_MODE_MASK_WCDMA;
    }

    if (!SYS_LTE_BAND_MASK_CHK_IF_EMPTY(&lteBands))
    {
      *pratMask |= SYS_SYS_MODE_MASK_LTE;
    }

    if (tdsBands & ALL_TDS_BANDS)
    {
      *pratMask |= SYS_SYS_MODE_MASK_TDS;
    }
  }
}



/*-------- policyman_util_rat_mask_to_sd_mode_pref --------*/
sd_ss_mode_pref_e_type policyman_util_rat_mask_to_sd_mode_pref(
  uint32 mask
)
{
  sd_ss_mode_pref_e_type sd_mode_pref = (sd_ss_mode_pref_e_type)mask;

  if ((mask & (SYS_SYS_MODE_MASK_GSM|SYS_SYS_MODE_MASK_WCDMA)) == (SYS_SYS_MODE_MASK_GSM|SYS_SYS_MODE_MASK_WCDMA))
  {
    sd_mode_pref |= SD_SS_MODE_PREF_GW;
  }

  if ((mask & (SYS_SYS_MODE_MASK_GSM|SYS_SYS_MODE_MASK_LTE)) == (SYS_SYS_MODE_MASK_GSM|SYS_SYS_MODE_MASK_LTE))
  {
    sd_mode_pref |= SD_SS_MODE_PREF_GSM_LTE;
  }

  if ((mask & (SYS_SYS_MODE_MASK_GSM|SYS_SYS_MODE_MASK_TDS)) == (SYS_SYS_MODE_MASK_GSM|SYS_SYS_MODE_MASK_TDS))
  {
    sd_mode_pref |= SD_SS_MODE_PREF_TDS_GSM;
  }

  if ((mask & (SYS_SYS_MODE_MASK_GSM|SYS_SYS_MODE_MASK_WCDMA|SYS_SYS_MODE_MASK_LTE)) == (SYS_SYS_MODE_MASK_GSM|SYS_SYS_MODE_MASK_WCDMA|SYS_SYS_MODE_MASK_LTE))
  {
    sd_mode_pref |= SD_SS_MODE_PREF_GWL;
  }

  if ((mask & (SYS_SYS_MODE_MASK_LTE|SYS_SYS_MODE_MASK_WCDMA)) == (SYS_SYS_MODE_MASK_LTE|SYS_SYS_MODE_MASK_WCDMA))
  {
    sd_mode_pref |= SD_SS_MODE_PREF_WCDMA_LTE;
  }

  if ((mask & (SYS_SYS_MODE_MASK_TDS|SYS_SYS_MODE_MASK_WCDMA)) == (SYS_SYS_MODE_MASK_TDS|SYS_SYS_MODE_MASK_WCDMA))
  {
    sd_mode_pref |= SD_SS_MODE_PREF_TDS_WCDMA;
  }

  if ((mask & (SYS_SYS_MODE_MASK_TDS|SYS_SYS_MODE_MASK_LTE)) == (SYS_SYS_MODE_MASK_TDS|SYS_SYS_MODE_MASK_LTE))
  {
    sd_mode_pref |= SD_SS_MODE_PREF_TDS_LTE;
  }

  if ((mask & (SYS_SYS_MODE_MASK_GSM|SYS_SYS_MODE_MASK_TDS|SYS_SYS_MODE_MASK_LTE)) == (SYS_SYS_MODE_MASK_GSM|SYS_SYS_MODE_MASK_TDS|SYS_SYS_MODE_MASK_LTE))
  {
    sd_mode_pref |= SD_SS_MODE_PREF_TDS_GSM_LTE;
  }

  if ((mask & (SYS_SYS_MODE_MASK_GSM|SYS_SYS_MODE_MASK_TDS|SYS_SYS_MODE_MASK_WCDMA)) == (SYS_SYS_MODE_MASK_GSM|SYS_SYS_MODE_MASK_TDS|SYS_SYS_MODE_MASK_WCDMA))
  {
    sd_mode_pref |= SD_SS_MODE_PREF_TDS_GSM_WCDMA;
  }

  if ((mask & (SYS_SYS_MODE_MASK_GSM|SYS_SYS_MODE_MASK_WCDMA|SYS_SYS_MODE_MASK_LTE|SYS_SYS_MODE_MASK_TDS)) == (SYS_SYS_MODE_MASK_GSM|SYS_SYS_MODE_MASK_WCDMA|SYS_SYS_MODE_MASK_LTE|SYS_SYS_MODE_MASK_TDS))
  {
    sd_mode_pref |= SD_SS_MODE_PREF_TDS_GSM_WCDMA_LTE;
  }

  if ((mask & (SYS_SYS_MODE_MASK_LTE|SYS_SYS_MODE_MASK_TDS|SYS_SYS_MODE_MASK_WCDMA)) == (SYS_SYS_MODE_MASK_LTE|SYS_SYS_MODE_MASK_TDS|SYS_SYS_MODE_MASK_WCDMA))
  {
    sd_mode_pref |= SD_SS_MODE_PREF_TDS_WCDMA_LTE;
  }

  POLICYMAN_MSG_HIGH_1("rat_mask to sd_mode_pref returned %d", sd_mode_pref);
  return sd_mode_pref;
}


/*-------- policyman_util_check_valid_sys_mode --------*/
boolean policyman_util_check_valid_sys_mode(
  sys_sys_mode_e_type sys_mode
)
{
  if (   sys_mode != SYS_SYS_MODE_CDMA
      && sys_mode != SYS_SYS_MODE_HDR
      && sys_mode != SYS_SYS_MODE_GSM
      && sys_mode != SYS_SYS_MODE_WCDMA
      && sys_mode != SYS_SYS_MODE_LTE
      && sys_mode != SYS_SYS_MODE_TDS
     )
  {
    return FALSE;
  }

  return TRUE;
}

/*-------- policyman_util_str_to_rat --------*/
sys_sys_mode_e_type policyman_util_str_to_rat(
  const char              *pStr
)
{
  static MRE_STR_VALUE_TABLE(map) =
  {
    MRE_STR_VALUE_ENTRY("G",       SYS_SYS_MODE_GSM),
    MRE_STR_VALUE_ENTRY("L",       SYS_SYS_MODE_LTE),
    MRE_STR_VALUE_ENTRY("W",       SYS_SYS_MODE_WCDMA),
    MRE_STR_VALUE_ENTRY("GSM",     SYS_SYS_MODE_GSM),
    MRE_STR_VALUE_ENTRY("LTE",     SYS_SYS_MODE_LTE),
    MRE_STR_VALUE_ENTRY("WCDMA",   SYS_SYS_MODE_WCDMA),
    MRE_STR_VALUE_ENTRY("TDSCDMA", SYS_SYS_MODE_TDS),
    MRE_STR_VALUE_ENTRY("T",       SYS_SYS_MODE_TDS),
    MRE_STR_VALUE_ENTRY("CDMA",    SYS_SYS_MODE_CDMA),
    MRE_STR_VALUE_ENTRY("HDR",     SYS_SYS_MODE_HDR),
    MRE_STR_VALUE_ENTRY("C",       SYS_SYS_MODE_CDMA),
    MRE_STR_VALUE_ENTRY("H",       SYS_SYS_MODE_HDR),
  };

  sys_sys_mode_e_type rat = SYS_SYS_MODE_NONE;

  MRE_LOOKUP_STR_VALUE_NO_STATUS(map, pStr, sys_sys_mode_e_type, &rat);

  return rat;
}


/*-------- policyman_util_str_to_rat_mask --------*/
uint32 policyman_util_str_to_rat_mask(
  const char              *pStr
)
  {
  static uint32 mask[] =
    {
    0,                        // SYS_SYS_MODE_NO_SRV
    0,                        // SYS_SYS_MODE_AMPS
    SYS_SYS_MODE_MASK_CDMA,
    SYS_SYS_MODE_MASK_GSM,
    SYS_SYS_MODE_MASK_HDR,
    SYS_SYS_MODE_MASK_WCDMA,
    0,                        // SYS_SYS_MODE_GPS
    0,                        // SYS_SYS_MODE_GW
    0,                        // SYS_SYS_MODE_WLAN
    SYS_SYS_MODE_MASK_LTE,
    0,                        // SYS_SYS_MODE_GWL
    SYS_SYS_MODE_MASK_TDS,
  };

  sys_sys_mode_e_type rat = SYS_SYS_MODE_NONE;

  rat = policyman_util_str_to_rat(pStr);

  return (SYS_SYS_MODE_NONE == rat)? 0 : mask[rat];
}


/*-------- policyman_create_mcc_set --------*/
mre_plmn_mcc_set_t *policyman_create_mcc_set(
  sys_modem_as_id_e_type   subsId,
  sys_mcc_type            *pMccArray,
  size_t                   mccArrayLen
)
{
  mre_plmn_mcc_set_t   *pMccSet;
  sys_mcc_type          mcc;
  size_t                idx;

  pMccSet = MRE_NAMED_SET_NEW( mre_plmn_mcc_set_t,
                               NULL,
                               MRE_NAMED_MCC_SET,
                               sizeof(uint32),
                               10,
                               MRE_SET_NO_MAX_SIZE,
                               NULL,
                               NULL );

  pMccSet->subsId = subsId;

  for (idx = 0 ; idx < mccArrayLen ; ++idx)
  {
    mre_named_set_add((mre_named_set_t *)pMccSet, &pMccArray[idx]);
  }

  return pMccSet;
}

/*-------- policyman_util_str_to_lookup_method --------*/
lookup_method_t policyman_util_str_to_lookup_method(
  char const *pStr
)
{
  static MRE_STR_VALUE_TABLE(map) =
  {
    MRE_STR_VALUE_ENTRY("DB",               USE_DB_ONLY),
    MRE_STR_VALUE_ENTRY("ITEMSET",          USE_ITEMSET_ONLY),
    MRE_STR_VALUE_ENTRY("DB_ITEMSET",       USE_DB_ITEMSET),
  };

  lookup_method_t action = USE_DB_ITEMSET;

  if (NON_NULL(pStr))
  {
    MRE_LOOKUP_STR_VALUE_NO_STATUS(map, pStr, lookup_method_t, &action);
  }

  return action;
}

/*-------- policyman_subs_id_is_valid --------*/
boolean policyman_subs_id_is_valid(
  sys_modem_as_id_e_type subsId
)
{
  return    subsId > SYS_MODEM_AS_ID_NONE
         && subsId < POLICYMAN_NUM_SUBS;
}

