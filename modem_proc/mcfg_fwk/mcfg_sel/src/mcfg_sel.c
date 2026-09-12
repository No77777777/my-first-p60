/*==============================================================================

                M O D E M   C O N F I G   S E L E C T I O N

GENERAL DESCRIPTION
  Contains the logic to automatically select a configuration file based on
  an outside identifying parameter, such as the ICCID read from the UIM.

Copyright (c) 2012-2014 by QUALCOMM Technologies Incorporated.
==============================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/mcfg_fwk.mpss/7.8.1.1/mcfg_sel/src/mcfg_sel.c#1 $
$DateTime: 2021/04/08 00:04:19 $
$Author: pwbldsvc $
$Change: 30003610 $

when     who  what, where, why
-------- ---  ----------------------------------------------------------------
09/07/12 bd   Created module

==============================================================================*/

#include "comdef.h"

#include "mcfg_common.h"
#include "mcfg_int.h"
#include "mcfg_nv.h"
#include "mcfg_osal.h"
#include "mcfg_sel.h"
#include "mcfg_trl.h"
#include "mcfg_uim.h"
#include "mcfg_utils.h"

#include "persistent_device_configuration_v01.h"
#include "qmi_pdc_svc.h"
#include "mcfg_svc.h"

#ifdef FEATURE_MCFG_REFRESH
#include "mcfg_refresh_i.h"
#endif
#include "mcfg_proc.h"

#include "mcfg_sel_db.h"
#include "mcfg_setting.h"
/*==============================================================================

                  DEFINITIONS AND DECLARATIONS FOR MODULE

==============================================================================*/
/*==============================================================================
  Constants and Macros
==============================================================================*/

/*==============================================================================
  Typedefs
==============================================================================*/
typedef enum mcfg_sel_hw_filter_type{
  MCFG_SEL_HW_FILTER_MIN = 0,
  MCFG_SEL_HW_FILTER_CARRIER = 0,
  MCFG_SEL_HW_FILTER_MULTISIM,
  //MCFG_SEL_HW_FILTER_OS,
  //MCFG_SEL_HW_FILTER_DEPLOYMENT,
  MCFG_SEL_HW_FILTER_NUMS
}mcfg_sel_hw_filter_e_type;

typedef struct mcfg_sel_config_match_s {
  boolean carrier_match;
  boolean imsi_3gpp2_match;
  boolean imsi_3gpp_match;
  boolean iin_match;
  boolean wild_card_match;
}mcfg_sel_config_match_s_type;

/*==============================================================================
  Variables
==============================================================================*/

/*==============================================================================

                 PRIVATE FUNCTION FORWARD DECLARATIONS

==============================================================================*/

static boolean mcfg_sel_config_is_feature_tagged
(
  const mcfg_config_info_s_type *config_info
);

static boolean mcfg_sel_config_is_better_match_by_iccid
(
  const char                    *iccid_ascii,
  const mcfg_config_info_s_type *candidate_config,
  const mcfg_config_info_s_type *selected_config,
  boolean                       *selected_config_is_wildcard
);

static void mcfg_sel_config_is_match_by_iccid
(
  const char					*iccid_ascii,
  const mcfg_config_info_s_type *candidate_config,
  mcfg_sel_config_match_s_type	*candidate_config_match_p
);

static boolean mcfg_sel_config_is_direct_iccid_match_with_iin_string
(
  const char *uim_iccid,
  const char *iin_string
);

static boolean mcfg_sel_config_is_better_match_by_version
(
  const mcfg_config_info_s_type *candidate_config,
  const mcfg_config_info_s_type *selected_config
);

static boolean mcfg_sel_config_is_direct_iccid_match
(
  const char                     *uim_iccid,
  const mcfg_trl_iin_list_s_type *config_iin_list
);

static boolean mcfg_sel_config_is_direct_plmn_match
(
  const mcfg_trl_carrier_mcc_mnc_s_type *uim_plmn,
  const mcfg_trl_plmn_list_s_type       *config_plmn_list
);

static uint32 mcfg_sel_find_best_match_by_iccid
(
  const char            *iccid_ascii,
  mcfg_config_id_s_type *config_list,
  uint32                 num_configs
);

static boolean mcfg_sel_iccid_matches_last
(
  mcfg_sub_id_type_e_type sub_id,
  const char             *iccid_ascii
);

#ifndef FEATURE_MCFG_SEL_DB
static void mcfg_sel_save_iccid
(
  mcfg_sub_id_type_e_type sub_id,
  const char             *iccid_ascii
);
#endif /* FEATURE_MCFG_SEL_DB */

static void mcfg_sel_select_sw_config
(
  mcfg_sub_id_type_e_type sub_id,
  mcfg_config_id_s_type  *config_id,
  mcfg_activation_source_e_type source
);

#ifdef FEATURE_MCFG_SELECTION_BY_IMSI
static void mcfg_sel_config_is_match_by_plmn
(
  mcfg_trl_carrier_mcc_mnc_s_type *plmn,
  const mcfg_config_info_s_type   *candidate_config,
  mcfg_sel_config_match_s_type    *candidate_config_match_p,
  boolean                         is_3gpp2
);

static boolean mcfg_sel_config_is_better_match_by_plmn
(
  mcfg_trl_carrier_mcc_mnc_s_type *plmn,
  const mcfg_config_info_s_type   *candidate_config,
  const mcfg_config_info_s_type   *selected_config,
  boolean                         *selected_config_is_wildcard
);

static boolean mcfg_sel_plmn_matches_last
(
  mcfg_sub_id_type_e_type          sub_id,
  mcfg_trl_carrier_mcc_mnc_s_type *plmn,
  mcfg_uim_ext_session_e_type      session_type
);

static void mcfg_sel_save_plmn
(
  mcfg_sub_id_type_e_type          sub_id,
  mcfg_trl_carrier_mcc_mnc_s_type *plmn,
  mcfg_uim_ext_session_e_type      session_type
);
#endif

/*==============================================================================

                     INTERNAL FUNCTION DECLARATIONS

==============================================================================*/

/*===========================================================================

  FUNCTION mcfg_sel_config_ids_match

  DESCRIPTION
    Checks whether two config IDs are the same.

  DEPENDENCIES
    None

  PARAMETERS
    config_id1 [in]
    config_id2 [in]

  RETURN VALUE
    TRUE if the config IDs match, FALSE otherwise

  SIDE EFFECTS
    None

===========================================================================*/
boolean mcfg_sel_config_ids_match
(
  const mcfg_config_id_s_type *config_id1,
  const mcfg_config_id_s_type *config_id2
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_CHECK_NULL_PTR_RET_FALSE(config_id1);
  MCFG_CHECK_NULL_PTR_RET_FALSE(config_id2);

  return (config_id1->id_len == config_id2->id_len &&
          config_id1->id_len <= MCFG_CONFIG_ID_SIZE_MAX &&
          memcmp(config_id1->id, config_id2->id, config_id1->id_len) == 0);
} /* mcfg_sel_config_ids_match() */


/*===========================================================================

  FUNCTION mcfg_sel_config_is_feature_tagged

  DESCRIPTION
    Checks config is feature_tagged type.

  DEPENDENCIES
    None

  PARAMETERS
    config_info [in]

  RETURN VALUE
    TRUE if feature_tagged, FALSE otherwise

  SIDE EFFECTS
    None

===========================================================================*/
static boolean mcfg_sel_config_is_feature_tagged
(
  const mcfg_config_info_s_type *config_info
)
{
  boolean feature_tagged = FALSE;
  mcfg_trl_mcfg_header_s_type mcfg_header;
  
  if (mcfg_trl_decode_tlv(
             config_info, MCFG_TRL_TLV_ID_MCFG_HEADER, &mcfg_header,
             sizeof(mcfg_header)))
  {
    MCFG_MSG_LOW_1("config fmt ver. %d", mcfg_header.mcfg_format_ver_num);
    if(mcfg_header.mcfg_format_ver_num == MCFG_FORMAT_NUM_FEATAGGED)
    {
      feature_tagged = TRUE;
    }
  }

 return feature_tagged;
} /* mcfg_sel_config_is_feature_tagged() */


/*===========================================================================

  FUNCTION mcfg_sel_config_is_better_match_by_iccid

  DESCRIPTION
    Determines whether the candidate config is a better match than the
    selected config based on the current ICCID, using version as a
    tiebreaker.

  DEPENDENCIES
    None

  PARAMETERS
    iccid_ascii      [in] ICCID of the current UIM; ASCII encoding
    candidate_config [in] trailer record of the config to consider against
      the current selection
    selected_config  [in] trailer record of the config currently selected
      as the best match; if no config is selected, this must be all zeros
    selected_config_is_wildcard [in/out] set to TRUE if the selected config
      is a wildcard (matches any ICCID); will be updated with the wildcard
      state of the candidate config if this function returns TRUE. Must be
      set to FALSE if no config is selected yet.

  RETURN VALUE
    TRUE if the candidate config is a better match than the selected config,
    FALSE otherwise

  SIDE EFFECTS
    None

===========================================================================*/
static boolean mcfg_sel_config_is_better_match_by_iccid
(
  const char                    *iccid_ascii,
  const mcfg_config_info_s_type *candidate_config,
  const mcfg_config_info_s_type *selected_config,
  boolean                       *selected_config_is_wildcard
)
{
  boolean candidate_is_better_match = FALSE;
  mcfg_trl_iin_list_s_type candidate_iin_list;
  char name[MCFG_TRL_CARRIER_NAME_MAX_LEN+1];
  char candidate_iin_string[MCFG_TRL_IIN_STRING_MAX_LEN+1];

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_CHECK_NULL_PTR_RET_FALSE(iccid_ascii);
  MCFG_CHECK_NULL_PTR_RET_FALSE(candidate_config);
  MCFG_CHECK_NULL_PTR_RET_FALSE(selected_config);
  MCFG_CHECK_NULL_PTR_RET_FALSE(selected_config_is_wildcard);

  MCFG_MSG_MED_1("Evaluating candidate config with len %d", candidate_config->config_len);

  if (mcfg_trl_decode_tlv(
        candidate_config, MCFG_TRL_TLV_ID_CARRIER_NAME, 
        name, MCFG_TRL_CARRIER_NAME_MAX_LEN+1))
  {
    MCFG_MSG_SPRINTF_1("Config: %s", name);
  }

  if (candidate_config->description_len == 0)
  {
    MCFG_MSG_ERROR("Candidate config trailer record is empty!");
  }
  else if (!mcfg_trl_decode_tlv(
             candidate_config, MCFG_TRL_TLV_ID_IIN_LIST, &candidate_iin_list,
             sizeof(candidate_iin_list)))
  {
    MCFG_MSG_ERROR("Couldn't decode IIN list");
  }
  else if (candidate_iin_list.iin_flag & MCFG_TRL_IIN_FLAG_WILDCARD)
  {
    /* If the candidate is flagged as a wildcard (generic config that works with
       any UIM), it is a better match than the selected config if (a) we don't
       have a selected config yet, or (b) the selected config is also a wildcard
       but is an older version. Otherwise, the selected config is a direct ICCID
       match, so it is better than the candidate. */
    MCFG_MSG_MED_1("Candidate is wildcard (flag 0x%x)",
                   candidate_iin_list.iin_flag);
    if (selected_config->description_len == 0 ||
        (*selected_config_is_wildcard &&
         mcfg_sel_config_is_better_match_by_version(
           candidate_config, selected_config)))
    {
      MCFG_MSG_MED("Candidate is the current best match");
      candidate_is_better_match = TRUE;
      *selected_config_is_wildcard = TRUE;
    }
  }
  else if (mcfg_sel_config_is_direct_iccid_match(iccid_ascii, &candidate_iin_list) || 
           ( mcfg_trl_decode_tlv( candidate_config, MCFG_TRL_TLV_ID_IIN_STRING, candidate_iin_string,
                                  MCFG_TRL_IIN_STRING_MAX_LEN+1) && 
             mcfg_sel_config_is_direct_iccid_match_with_iin_string(iccid_ascii, candidate_iin_string)))
  {
    /* The candidate is a direct ICCID match, so it is a better match than the
       selected config if (a) the selected config is a wildcard, (b) we don't
       have a selected config yet, or (c) the selected config is an older
       version. Otherwise, the selected config is a direct ICCID match with a
       newer version, so it is better than the candidate. */
    MCFG_MSG_MED("Candidate is direct ICCID match");
    if (*selected_config_is_wildcard ||
        mcfg_sel_config_is_better_match_by_version(
          candidate_config, selected_config))
    {
      MCFG_MSG_MED("Candidate is the current best match");
      candidate_is_better_match = TRUE;
      *selected_config_is_wildcard = FALSE;
    }
  }
  else
  {
    MCFG_MSG_LOW_1("mcfg_sel_config_is_better_match_by_iccid not found %d", candidate_is_better_match);
  }

  return candidate_is_better_match;
} /* mcfg_sel_config_is_better_match_by_iccid() */

#ifdef FEATURE_MCFG_SELECTION_BY_IMSI
/*===========================================================================

  FUNCTION mcfg_sel_config_is_better_match_by_plmn

  DESCRIPTION
    Determines whether the candidate config is a better match than the
    selected config based on the current PLMN, using version as a
    tiebreaker.

  DEPENDENCIES
    None

  PARAMETERS
    plmn      [in] PLMN of the current UIM, including decoded mcc & mnc
    candidate_config [in] trailer record of the config to consider against
      the current selection
    selected_config  [in] trailer record of the config currently selected
      as the best match; if no config is selected, this must be all zeros
    selected_config_is_wildcard [in/out] set to TRUE if the selected config
      is a wildcard (matches any ICCID); will be updated with the wildcard
      state of the candidate config if this function returns TRUE. Must be
      set to FALSE if no config is selected yet.

  RETURN VALUE
    TRUE if the candidate config is a better match than the selected config,
    FALSE otherwise

  SIDE EFFECTS
    None

===========================================================================*/
static boolean mcfg_sel_config_is_better_match_by_plmn
(
  mcfg_trl_carrier_mcc_mnc_s_type *plmn,
  const mcfg_config_info_s_type   *candidate_config,
  const mcfg_config_info_s_type   *selected_config,
  boolean                         *selected_config_is_wildcard
)
{
  boolean candidate_is_better_match = FALSE;
  mcfg_trl_plmn_list_s_type candidate_plmn_list;
  char name[MCFG_TRL_CARRIER_NAME_MAX_LEN+1];

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_CHECK_NULL_PTR_RET_FALSE(plmn);
  MCFG_CHECK_NULL_PTR_RET_FALSE(candidate_config);
  MCFG_CHECK_NULL_PTR_RET_FALSE(selected_config);
  MCFG_CHECK_NULL_PTR_RET_FALSE(selected_config_is_wildcard);

  MCFG_MSG_MED_1("Evaluating candidate config with len %d", candidate_config->config_len);
  if (mcfg_trl_decode_tlv(
        candidate_config, MCFG_TRL_TLV_ID_CARRIER_NAME, 
        name, MCFG_TRL_CARRIER_NAME_MAX_LEN+1))
  {
    MCFG_MSG_SPRINTF_1("Config: %s", name);
  }

  if (candidate_config->description_len == 0)
  {
    MCFG_MSG_ERROR("Candidate config trailer record is empty!");
  }
  else if (!mcfg_trl_decode_tlv(
             candidate_config, MCFG_TRL_TLV_ID_CARRIER_PLMN_LIST, &candidate_plmn_list,
             sizeof(candidate_plmn_list)))
  {
    MCFG_MSG_ERROR("Couldn't decode MCC-MNC list");
  }
  else if (candidate_plmn_list.plmn_flag & MCFG_TRL_PLMN_FLAG_WILDCARD)
  {
    /* If the candidate is flagged as a wildcard (generic config that works with
       any UIM), it is a better match than the selected config if (a) we don't
       have a selected config yet, or (b) the selected config is also a wildcard
       but is an older version. Otherwise, the selected config is a direct ICCID
       match, so it is better than the candidate. */
    MCFG_MSG_MED_1("Candidate is wildcard (flag 0x%x)",
                   candidate_plmn_list.plmn_flag);
    if (selected_config->description_len == 0 ||
        (*selected_config_is_wildcard &&
         mcfg_sel_config_is_better_match_by_version(
           candidate_config, selected_config)))
    {
      MCFG_MSG_MED("Candidate is the current best match");
      candidate_is_better_match = TRUE;
      *selected_config_is_wildcard = TRUE;
    }
  }
  else if (mcfg_sel_config_is_direct_plmn_match(plmn, &candidate_plmn_list))
  {
    /* The candidate is a direct ICCID match, so it is a better match than the
       selected config if (a) the selected config is a wildcard, (b) we don't
       have a selected config yet, or (c) the selected config is an older
       version. Otherwise, the selected config is a direct ICCID match with a
       newer version, so it is better than the candidate. */
    MCFG_MSG_MED("Candidate is direct MCC-MNC match");
    if (*selected_config_is_wildcard ||
        mcfg_sel_config_is_better_match_by_version(
          candidate_config, selected_config))
    {
      MCFG_MSG_MED("Candidate is the current best match");
      candidate_is_better_match = TRUE;
      *selected_config_is_wildcard = FALSE;
    }
  }

  return candidate_is_better_match;
} /* mcfg_sel_config_is_better_match_by_plmn() */
#endif /* FEATURE_MCFG_SELECTION_BY_IMSI */

/*===========================================================================

  FUNCTION mcfg_sel_config_is_better_match_by_version

  DESCRIPTION
    Determines whether the candidate config is a better match than the
    selected config based on their versions. Note that this comparison
    mode only considers the version TLV, so it is best suited as a
    tiebreaker rather than the primary decision logic.

  DEPENDENCIES
    None

  PARAMETERS
    candidate_config [in] trailer record of the config to consider against
      the current selection
    selected_config  [in] trailer record of the config currently selected
      as the best match; if no config is selected, this must be all zeros

  RETURN VALUE
    TRUE if the candidate config is a better match than the selected config,
    FALSE otherwise

  SIDE EFFECTS
    None

===========================================================================*/
static boolean mcfg_sel_config_is_better_match_by_version
(
  const mcfg_config_info_s_type *candidate_config,
  const mcfg_config_info_s_type *selected_config
)
{
  boolean candidate_is_better_match = FALSE;
  mcfg_trl_config_version_s_type candidate_version;
  mcfg_trl_config_version_s_type selected_version;
#ifndef FEATURE_MCFG_FEATURETAG
  mcfg_trl_category_s_type candidate_category;
  mcfg_trl_category_s_type selected_category;
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_CHECK_NULL_PTR_RET_FALSE(selected_config);

  /* If we haven't selected a config yet, the candidate automatically becomes
     the new selection. */
  if (selected_config->description_len == 0)
  {
    MCFG_MSG_MED("Candidate wins version comparison by default");
    return TRUE;
  }

#ifndef FEATURE_MCFG_FEATURETAG
  if (!mcfg_trl_decode_tlv(candidate_config, MCFG_TRL_TLV_ID_CATEGORY,
                           &candidate_category, sizeof(mcfg_trl_category_s_type)) ||
      !mcfg_trl_decode_tlv(selected_config, MCFG_TRL_TLV_ID_CATEGORY,
                           &selected_category, sizeof(mcfg_trl_category_s_type)))
  {
    MCFG_MSG_ERROR("Couldn't decode config category TLV for volte non-volte");
  }
  else
  {
    if ((candidate_category.category & MCFG_CATEGORY_BITMASK_VOLTE) > 0 && 
        (selected_category.category & MCFG_CATEGORY_BITMASK_VOLTE) == 0 )
    {
      MCFG_MSG_MED("candidate is better match with volte");
      return TRUE;
    }
    else if ((selected_category.category & MCFG_CATEGORY_BITMASK_VOLTE) > 0 && 
        (candidate_category.category & MCFG_CATEGORY_BITMASK_VOLTE) == 0 )
    {
      MCFG_MSG_MED("candidate is not a better match with non volte");
      return FALSE;
    }
  }
#endif

  if (!mcfg_trl_decode_tlv(candidate_config, MCFG_TRL_TLV_ID_CONFIG_VERSION,
                                &candidate_version, sizeof(candidate_version)) ||
           !mcfg_trl_decode_tlv(selected_config, MCFG_TRL_TLV_ID_CONFIG_VERSION,
                                &selected_version, sizeof(selected_version)))
  {
    MCFG_MSG_ERROR("Couldn't decode config version TLV");
  }
  else
  {
    MCFG_MSG_MED_4("Comparing candidate v%hhu.%hhu vs. selected v%hhu.%hhu",
                   candidate_version.maj_ver, candidate_version.min_ver,
                   selected_version.maj_ver, selected_version.min_ver);
    if (candidate_version.maj_ver > selected_version.maj_ver ||
        (candidate_version.maj_ver == selected_version.maj_ver &&
         candidate_version.min_ver > selected_version.min_ver))
    {
      candidate_is_better_match = TRUE;
    }
  }

  return candidate_is_better_match;
} /* mcfg_sel_config_is_better_match_by_version() */


/*===========================================================================

	FUNCTION mcfg_sel_config_is_better_match_by_carrier

	DESCRIPTION
		Determines whether the candidate config is a better match than the
		selected config based on the current CARRIER, using version as a
		tiebreaker.

	DEPENDENCIES
		None

	PARAMETERS
		carrier 		 [in] CARRIER of the current UIM, including decoded mcc & mnc
		candidate_config [in] trailer record of the config to consider against
			the current selection
		selected_config  [in] trailer record of the config currently selected
			as the best match; if no config is selected, this must be all zeros
                mcfg_sel_config_match_s_type [in] It will be used to compare with the 
                        candidate config matching status with (IMSI/IIN/wildcard)

	RETURN VALUE
		TRUE if the candidate config is a better match than the selected config,
		FALSE otherwise

	SIDE EFFECTS
		None

===========================================================================*/
static boolean mcfg_sel_config_is_better_match_by_carrier(mcfg_slot_index_type_e_type sloti,
                                                          pdc_selection_carrier_enum_v01 carrier,
                                                          const mcfg_config_info_s_type *candidate_config,
                                                          const mcfg_config_info_s_type *selected_config,
                                                          mcfg_sel_config_match_s_type  *selected_config_match)
{
  boolean 				candidate_is_better_match = FALSE;
  mcfg_trl_category_s_type category;

#ifdef FEATURE_MCFG_FEATURETAG
  boolean 				candidate_feature_tagged = FALSE;
  boolean 				selected_feature_tagged = FALSE;
  boolean 				selected_config_is_wildcard = FALSE;
  mcfg_uim_card_info_s_type *p_card_info = NULL;
  mcfg_sel_config_match_s_type candidate_config_match;
#endif

  mcfg_trl_config_version_s_type candidate_version;
  char						name[MCFG_TRL_CARRIER_NAME_MAX_LEN + 1];
  pdc_get_feature_ind_msg_v01 feature;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_CHECK_NULL_PTR_RET_FALSE(candidate_config);
  MCFG_CHECK_NULL_PTR_RET_FALSE(selected_config);
  MCFG_CHECK_NULL_PTR_RET_FALSE(selected_config_match);

  memset(&candidate_config_match, 0, sizeof(candidate_config_match));
  MCFG_MSG_MED_1("Evaluating candidate config with len %d", candidate_config->config_len);

  if (mcfg_trl_decode_tlv(candidate_config, MCFG_TRL_TLV_ID_CARRIER_NAME,
                          name, MCFG_TRL_CARRIER_NAME_MAX_LEN + 1))
  {
    MCFG_MSG_SPRINTF_1("Config: %s", name);
  }

  if (candidate_config->description_len == 0)
  {
    MCFG_MSG_ERROR("Candidate config trailer record is empty!");
  }
  else if (!mcfg_trl_decode_tlv(candidate_config, MCFG_TRL_TLV_ID_CONFIG_VERSION,
                                &candidate_version, sizeof(candidate_version)))
  {
    MCFG_MSG_ERROR("Couldn't decode config version");
  }

#ifndef FEATURE_MCFG_FEATURETAG
  else if (!mcfg_trl_decode_tlv(candidate_config, MCFG_TRL_TLV_ID_CATEGORY,
                                &category, sizeof(mcfg_trl_category_s_type)))
  {
    MCFG_MSG_ERROR("Couldn't decode category");
  }
  else if ((category.category & MCFG_CATEGORY_BITMASK_LAB) > 0)
  {
    MCFG_MSG_LOW("skip lab MBN");
  }
  else if ((carrier == candidate_version.maj_ver) && ((category.category & MCFG_CATEGORY_BITMASK_SUBSI) > 0))
  {
    MCFG_MSG_MED("Candidate is direct carrier match");

    if (mcfg_sel_config_is_better_match_by_version(candidate_config, selected_config))
    {
      MCFG_MSG_MED("Candidate is the current best match by version");
      candidate_is_better_match = TRUE;
    }
  }

#else /* FEATURE_MCFG_FEATURETAG defined */

  else
  {
    if (mcfg_trl_decode_tlv(candidate_config, MCFG_TRL_TLV_ID_CATEGORY,
                            &category, sizeof(mcfg_trl_category_s_type)))
    {
      if ((category.category & MCFG_CATEGORY_BITMASK_SUBSI) > 0)
      {
        MCFG_MSG_LOW("subsidized lab MBN");
        return FALSE;
      }
      else if ((category.category & MCFG_CATEGORY_BITMASK_LAB) > 0)
      {
        MCFG_MSG_LOW("skip lab MBN");
        return FALSE;
      }
    }

    if (carrier == candidate_version.maj_ver)
    {
      MCFG_MSG_MED("Candidate is direct carrier match");
      candidate_config_match.carrier_match = TRUE;
      candidate_feature_tagged = mcfg_sel_config_is_feature_tagged(candidate_config);
      selected_feature_tagged = mcfg_sel_config_is_feature_tagged(selected_config);

      if (candidate_feature_tagged)
      {
        if (selected_feature_tagged)
        {
          MCFG_MSG_MED("Both Candidate and selected are feature_tagged");

          if (mcfg_sel_config_is_better_match_by_version(candidate_config, selected_config))
          {
            MCFG_MSG_MED("Candidate is the current best match by version");
            candidate_is_better_match = TRUE;
          }
        }
        else
        {
          MCFG_MSG_MED("Candidate is feature_tagged, replacing non-feature_tagged selection");
          candidate_is_better_match = TRUE;
        }
      }
      else /* !candidate_feature_tagged */
      {
        if (selected_feature_tagged)
        {
          MCFG_MSG_MED("Candidate is non-feature_tagged, keep feature_tagged selection");
        }
        else
        {
          MCFG_MSG_MED("Both Candidate and selected are not feature_tagged");

          do
          {
            p_card_info = mcfg_uim_get_card_info_p(sloti);
            if (!p_card_info)
            {
              MCFG_MSG_ERROR_1("fail to get card info for sloti %d", sloti);
              break;
            }

            if (!candidate_config_match.carrier_match)
            {
              break;
            }

            mcfg_sel_config_is_match_by_iccid(p_card_info->ICCID, 
                                              candidate_config, 
                                              &candidate_config_match);

            if ((p_card_info->plmn[MCFG_UIM_EXT_SESSION_1X][0].mcc == 460) &&
                (p_card_info->plmn[MCFG_UIM_EXT_SESSION_1X][0].mnc == 3))
            {
              mcfg_sel_config_is_match_by_plmn(&p_card_info->plmn[MCFG_UIM_EXT_SESSION_1X][0], 
                                               candidate_config, 
                                               &candidate_config_match,
                                               TRUE);
            }

            if (p_card_info->plmn[MCFG_UIM_EXT_SESSION_GW][0].mcc > 0)
            {
              mcfg_sel_config_is_match_by_plmn(&p_card_info->plmn[MCFG_UIM_EXT_SESSION_GW][0], 
                                               candidate_config, 
                                               &candidate_config_match,
                                               FALSE);
            }

            if((carrier == PDC_SELECTION_CARRIER_GEN_UMTS_EU_V01) ||
               (!selected_config_match->carrier_match) ||
               (selected_config->config_len == 0))
            {
              MCFG_MSG_MED("Candidate is the current best match by carrier");
              candidate_is_better_match = TRUE;
              break;
            }

            if (candidate_config_match.imsi_3gpp2_match != selected_config_match->imsi_3gpp2_match)
            {
              if(candidate_config_match.imsi_3gpp2_match)
              {
                MCFG_MSG_MED("Candidate is the current best match with CT 1x plmn");
                candidate_is_better_match = TRUE;
              }
              break;
            }

            if (candidate_config_match.imsi_3gpp_match != selected_config_match->imsi_3gpp_match)
            {
              if(candidate_config_match.imsi_3gpp_match)
              {
                MCFG_MSG_MED("Candidate is the current best match with plmn");
                candidate_is_better_match = TRUE;
              }
              break;
            }

            if (candidate_config_match.iin_match != selected_config_match->iin_match)
            {
              if(candidate_config_match.iin_match)
              {
                MCFG_MSG_MED("Candidate is the current best match with iccid");
                candidate_is_better_match = TRUE;
              }
              break;
            }

            /* If none of IMSI/IIN matched, check for wildcard match */
            if(!(candidate_config_match.imsi_3gpp2_match ||
                 candidate_config_match.imsi_3gpp_match ||
                 candidate_config_match.iin_match) &&
                (candidate_config_match.wild_card_match != selected_config_match->wild_card_match))
            {
              if(candidate_config_match.wild_card_match)
              {
                MCFG_MSG_MED("Candidate is the current best match as wildcard");
                candidate_is_better_match = TRUE;
              }
              break;
            }

            if(mcfg_sel_config_is_better_match_by_version(candidate_config, selected_config))
            {
              MCFG_MSG_MED("Candidate is the current best match with version");
              candidate_is_better_match = TRUE;
              break;
            }

          }
          while (0);

          if(candidate_is_better_match)
          {
            *selected_config_match = candidate_config_match;
          }
        }
      }

    }
  }
#endif

  return candidate_is_better_match;
  } /* mcfg_sel_config_is_better_match_by_carrier() */


/*===========================================================================

  FUNCTION mcfg_sel_config_is_direct_iccid_match

  DESCRIPTION
    Compares the ICCID of the current UIM against a list of IINs (essentially
    ICCID prefixes) associated with a config.

  DEPENDENCIES
    None

  PARAMETERS
    uim_iccid       [in]
    config_iin_list [in]

  RETURN VALUE
    TRUE if one or more of the IINs in config_iin_list match uim_iccid,
    FALSE otherwise

  SIDE EFFECTS
    None

===========================================================================*/
static boolean mcfg_sel_config_is_direct_iccid_match
(
  const char                     *uim_iccid,
  const mcfg_trl_iin_list_s_type *config_iin_list
)
{
  uint32 i;
  int ret;
  char iccid_prefix[MCFG_UIM_ICCID_STR_BUF_LEN];
  boolean is_direct_iccid_match = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_CHECK_NULL_PTR_RET_FALSE(uim_iccid);
  MCFG_CHECK_NULL_PTR_RET_FALSE(config_iin_list);

  for (i = 0; i < config_iin_list->iin_list_count; i++)
  {
    ret = snprintf(iccid_prefix, sizeof(iccid_prefix), "%lu",
                   config_iin_list->iin_list[i]);
    MCFG_MSG_SPRINTF_1("Comparing ICCID against prefix '%s'", iccid_prefix);
    if (ret <= 0 || ret > sizeof(iccid_prefix))
    {
      MCFG_MSG_ERROR_1("Error converting IIN to string: %d", ret);
    }
    else if (strncmp(uim_iccid, iccid_prefix, ret) == 0)
    {
      is_direct_iccid_match = TRUE;
      break;
    }
  }

  return is_direct_iccid_match;
} /* mcfg_sel_config_is_direct_iccid_match() */

/*===========================================================================

  FUNCTION mcfg_sel_config_is_direct_iccid_match_with_iin_string

  DESCRIPTION
    Compares the ICCID of the current UIM against a list of IINs (essentially
    ICCID prefixes) associated with a config.

  DEPENDENCIES
    None

  PARAMETERS
    uim_iccid       [in]
    config_iin_list [in]

  RETURN VALUE
    TRUE if one or more of the IINs in config_iin_list match uim_iccid,
    FALSE otherwise

  SIDE EFFECTS
    None

===========================================================================*/
static boolean mcfg_sel_config_is_direct_iccid_match_with_iin_string
(
  const char *uim_iccid,
  const char *iin_string
)
{
  char iin[MCFG_TRL_IIN_STRING_MAX_LEN+1];
  uint32 len, offset = 0;
  boolean is_direct_iccid_match = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_CHECK_NULL_PTR_RET_FALSE(uim_iccid);
  MCFG_CHECK_NULL_PTR_RET_FALSE(iin_string);

  while (mcfg_utils_get_phrase((char*)iin_string, &offset, MCFG_TRL_IIN_STRING_MAX_LEN+1, iin))
  {
    len = strlen(iin);

    MCFG_MSG_SPRINTF_1("comparing ICCID against IIN string '%s'", iin);
    if (strlen(uim_iccid) > len && strncasecmp(uim_iccid, iin, len)==0)
    {
      MCFG_MSG_SPRINTF_1("match found for ICCID '%s'", uim_iccid);
      is_direct_iccid_match = TRUE;
      break;
    }
  }

  return is_direct_iccid_match;
} /* mcfg_sel_config_is_direct_iccid_match_with_iin_string() */

/*===========================================================================

  FUNCTION mcfg_sel_config_is_direct_plmn_match

  DESCRIPTION
    Compares the PLMN of the current UIM against a list of PLMNs (essentially
    PLMN prefixes) associated with a config.

  DEPENDENCIES
    None

  PARAMETERS
    uim_plmn       [in]
    config_plmn_list [in]

  RETURN VALUE
    TRUE if one or more of the PLMNs in config_plmn_list match uim_plmn,
    FALSE otherwise

  SIDE EFFECTS
    None

===========================================================================*/
static boolean mcfg_sel_config_is_direct_plmn_match
(
  const mcfg_trl_carrier_mcc_mnc_s_type *uim_plmn,
  const mcfg_trl_plmn_list_s_type       *config_plmn_list
)
{
  uint32 i;
  boolean is_direct_plmn_match = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_CHECK_NULL_PTR_RET_FALSE(uim_plmn);
  MCFG_CHECK_NULL_PTR_RET_FALSE(config_plmn_list);

  for (i = 0; i < config_plmn_list->plmn_list_count; i++)
  {
    MCFG_MSG_MED_4("Comparing PLMN %d:%d against prefix %d:%d", 
                   uim_plmn->mcc,
                   uim_plmn->mnc,
                   config_plmn_list->plmn_list[i].mcc,
                   config_plmn_list->plmn_list[i].mnc);
    if (config_plmn_list->plmn_list[i].mcc == uim_plmn->mcc &&
        config_plmn_list->plmn_list[i].mnc == uim_plmn->mnc)
    {
      is_direct_plmn_match = TRUE;
      break;
    }
  }

  return is_direct_plmn_match;
} /* mcfg_sel_config_is_direct_plmn_match() */

/*===========================================================================

  FUNCTION mcfg_sel_config_is_match_by_iccid

  DESCRIPTION
    Determines whether the candidate config is a match with the given ICCID.

  DEPENDENCIES
    None

  PARAMETERS
    iccid_ascii      [in] ICCID of the current UIM; ASCII encoding
    candidate_config [in] trailer record of the config
    candidate_config_match_p [out] match result is updated in this

  RETURN VALUE
    None
   
  SIDE EFFECTS
    None

===========================================================================*/
static void mcfg_sel_config_is_match_by_iccid
(
  const char                    *iccid_ascii,
  const mcfg_config_info_s_type *candidate_config,
  mcfg_sel_config_match_s_type  *candidate_config_match_p
)
{
  mcfg_trl_iin_list_s_type candidate_iin_list;
  char name[MCFG_TRL_CARRIER_NAME_MAX_LEN+1];
  char candidate_iin_string[MCFG_TRL_IIN_STRING_MAX_LEN+1];

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_CHECK_NULL_PTR_RET(iccid_ascii);
  MCFG_CHECK_NULL_PTR_RET(candidate_config);
  MCFG_CHECK_NULL_PTR_RET(candidate_config_match_p);

  if (candidate_config->description_len == 0)
  {
    MCFG_MSG_ERROR("Candidate config trailer record is empty!");
  }
  else if (!mcfg_trl_decode_tlv(
              candidate_config, MCFG_TRL_TLV_ID_IIN_LIST, &candidate_iin_list,
              sizeof(candidate_iin_list)))
  {
    MCFG_MSG_ERROR("Couldn't decode IIN list");
  }
  else if (candidate_iin_list.iin_flag & MCFG_TRL_IIN_FLAG_WILDCARD)
  {
    MCFG_MSG_MED_1("Candidate is wildcard (flag 0x%x) for IIN",
                   candidate_iin_list.iin_flag);
    candidate_config_match_p->wild_card_match = TRUE;
  }
  else if (mcfg_sel_config_is_direct_iccid_match(iccid_ascii, &candidate_iin_list) || 
           ( mcfg_trl_decode_tlv( candidate_config, MCFG_TRL_TLV_ID_IIN_STRING, candidate_iin_string,
                                  MCFG_TRL_IIN_STRING_MAX_LEN+1) && 
             mcfg_sel_config_is_direct_iccid_match_with_iin_string(iccid_ascii, candidate_iin_string)))
  {
    MCFG_MSG_MED("Candidate is direct ICCID match");
    candidate_config_match_p->iin_match = TRUE;
    candidate_config_match_p->wild_card_match = FALSE;
  }
  else
  {
    MCFG_MSG_LOW("Candidate not matched with ICCID ");
  }

} /* mcfg_sel_config_is_match_by_iccid() */

/*===========================================================================

  FUNCTION mcfg_sel_find_best_match_by_iccid

  DESCRIPTION
    Finds the best available software config for the current UIM based on
    ICCID.

  DEPENDENCIES
    None

  PARAMETERS
    iccid_ascii [in] Valid ICCID of the UIM currently inserted in slot 1;
                     ASCII encoding
    config_list [in] List of available configs
    num_configs [in] Number of available configs

  RETURN VALUE
    Index in config_list array of the best available config, or
    MCFG_CONFIG_LIST_SIZE_MAX if no suitable match found.

  SIDE EFFECTS
    None

===========================================================================*/
static uint32 mcfg_sel_find_best_match_by_iccid
(
  const char            *iccid_ascii,
  mcfg_config_id_s_type *config_list,
  uint32                 num_configs
)
{
  uint32 i;
  uint32 selected_config_index = MCFG_CONFIG_LIST_SIZE_MAX;
  mcfg_config_info_s_type selected_config_info;
  mcfg_config_info_s_type current_config_info;
  boolean selected_config_is_wildcard = FALSE;
  mcfg_trl_category_s_type category;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  memset(&selected_config_info, 0, sizeof(selected_config_info));
  memset(&current_config_info, 0, sizeof(current_config_info));
  for (i = 0; i < num_configs; i++)
  {
    if (!mcfg_utils_get_config_info(
          MCFG_TYPE_SW, &config_list[i], &current_config_info))
    {
      MCFG_MSG_ERROR_1("Couldn't get config info at index %lu", i);
    }
    else 
    {
      if (mcfg_trl_decode_tlv(&current_config_info, MCFG_TRL_TLV_ID_CATEGORY, 
             &category, sizeof(mcfg_trl_category_s_type)))
      {
        if ((category.category & MCFG_CATEGORY_BITMASK_SUBSI) > 0)
        {
          MCFG_MSG_LOW_1("subsidized MBN skip %d", i);
          continue;
        }
        else if ((category.category & MCFG_CATEGORY_BITMASK_LAB) > 0)
        {
          MCFG_MSG_LOW_1("lab MBN skip %d", i);
          continue;
        }
      }

      if (mcfg_sel_config_is_better_match_by_iccid(
               iccid_ascii, &current_config_info, &selected_config_info,
               &selected_config_is_wildcard))
      {
        selected_config_index = i;
        memscpy(&selected_config_info, sizeof(mcfg_config_info_s_type),
                &current_config_info, sizeof(mcfg_config_info_s_type));
      }
    }
  }

  if (selected_config_index == MCFG_CONFIG_LIST_SIZE_MAX)
  {
    MCFG_MSG_HIGH("No configs available for current ICCID");
  }

  return selected_config_index;
} /* mcfg_sel_find_best_match_by_iccid() */

#ifdef FEATURE_MCFG_SELECTION_BY_IMSI
/*===========================================================================

  FUNCTION mcfg_sel_config_is_match_by_plmn

  DESCRIPTION
    Determines whether the candidate config is a match for the given PLMN.

  DEPENDENCIES
    None

  PARAMETERS
    plmn      [in] PLMN of the current UIM, including decoded mcc & mnc
    candidate_config [in] trailer record of the config
    candidate_config_match_p [out] match result is updated in this
    is_3gpp2 [in] denotes if it is 3gpp2 or 3gpp PLMN

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
static void mcfg_sel_config_is_match_by_plmn
(
  mcfg_trl_carrier_mcc_mnc_s_type *plmn,
  const mcfg_config_info_s_type   *candidate_config,
  mcfg_sel_config_match_s_type    *candidate_config_match_p,
  boolean                         is_3gpp2
)
{
  mcfg_trl_plmn_list_s_type candidate_plmn_list;
  char name[MCFG_TRL_CARRIER_NAME_MAX_LEN+1];
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_CHECK_NULL_PTR_RET(plmn);
  MCFG_CHECK_NULL_PTR_RET(candidate_config);
  MCFG_CHECK_NULL_PTR_RET(candidate_config_match_p);

  if (candidate_config->description_len == 0)
  {
    MCFG_MSG_ERROR("Candidate config trailer record is empty!");
  }
  else if (!mcfg_trl_decode_tlv(
             candidate_config, MCFG_TRL_TLV_ID_CARRIER_PLMN_LIST, &candidate_plmn_list,
             sizeof(candidate_plmn_list)))
  {
    MCFG_MSG_ERROR("Couldn't decode MCC-MNC list");
  }
  else if (candidate_plmn_list.plmn_flag & MCFG_TRL_PLMN_FLAG_WILDCARD)
  {
    MCFG_MSG_MED_1("Candidate is wildcard (flag 0x%x) for MCC-MNC",
                   candidate_plmn_list.plmn_flag);
    candidate_config_match_p->wild_card_match = TRUE;
  }
  else if (mcfg_sel_config_is_direct_plmn_match(plmn, &candidate_plmn_list))
  {
    candidate_config_match_p->wild_card_match = FALSE;
    if(is_3gpp2)
    {
      candidate_config_match_p->imsi_3gpp2_match = TRUE;
      MCFG_MSG_MED("Candidate is direct 1X MCC-MNC match");
    }
    else
    {
      candidate_config_match_p->imsi_3gpp_match = TRUE;
      MCFG_MSG_MED("Candidate is direct MCC-MNC match");
    }
  }
  else
  {
    MCFG_MSG_LOW("Candidate is not matched with MCC-MNC ");
  }

} /* mcfg_sel_config_is_match_by_plmn() */

/*===========================================================================

  FUNCTION mcfg_sel_find_best_match_by_plmn

  DESCRIPTION
    Finds the best available software config for the current UIM based on
    plmn.

  DEPENDENCIES
    None

  PARAMETERS
    plmn        [in] Valid plmn of the UIM 
    config_list [in] List of available configs
    num_configs [in] Number of available configs

  RETURN VALUE
    Index in config_list array of the best available config, or
    MCFG_CONFIG_LIST_SIZE_MAX if no suitable match found.

  SIDE EFFECTS
    None

===========================================================================*/
static uint32 mcfg_sel_find_best_match_by_plmn
(
  mcfg_trl_carrier_mcc_mnc_s_type           *plmn,
  mcfg_config_id_s_type                     *config_list,
  uint32                                    num_configs
)
{
  uint32 i;
  uint32 selected_config_index = MCFG_CONFIG_LIST_SIZE_MAX;
  mcfg_config_info_s_type selected_config_info;
  mcfg_config_info_s_type current_config_info;
  boolean selected_config_is_wildcard = FALSE;
  mcfg_trl_category_s_type category;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  memset(&selected_config_info, 0, sizeof(selected_config_info));
  memset(&current_config_info, 0, sizeof(current_config_info));
  for (i = 0; i < num_configs; i++)
  {
    if (!mcfg_utils_get_config_info(
          MCFG_TYPE_SW, &config_list[i], &current_config_info))
    {
      MCFG_MSG_ERROR_1("Couldn't get config info at index %lu", i);
    }
    else
    {
      if (mcfg_trl_decode_tlv(&current_config_info, MCFG_TRL_TLV_ID_CATEGORY, 
             &category, sizeof(mcfg_trl_category_s_type)))
      {
        if ((category.category & MCFG_CATEGORY_BITMASK_SUBSI) > 0)
        {
          MCFG_MSG_LOW_1("subsidized MBN skip %d", i);
          continue;
        }
        else if ((category.category & MCFG_CATEGORY_BITMASK_LAB) > 0)
        {
          MCFG_MSG_LOW_1("lab MBN skip %d", i);
          continue;
        }
      }


      if (mcfg_sel_config_is_better_match_by_plmn(
               plmn, &current_config_info, &selected_config_info,
               &selected_config_is_wildcard))
      {
      selected_config_index = i;
      memscpy(&selected_config_info, sizeof(mcfg_config_info_s_type),
              &current_config_info, sizeof(mcfg_config_info_s_type));
      }
    }
  }

  if (selected_config_index == MCFG_CONFIG_LIST_SIZE_MAX)
  {
    MCFG_MSG_HIGH("No configs available for current plmn");
  }

  return selected_config_index;
} /* mcfg_sel_find_best_match_by_plmn() */
#endif /* FEATURE_MCFG_SELECTION_BY_IMSI */

/*===========================================================================

  FUNCTION mcfg_sel_find_best_match_by_carrier

  DESCRIPTION
    Finds the best available software config for the current UIM based on
    carrier.

  DEPENDENCIES
    None

  PARAMETERS
    carrier     [in] Valid carrier of the UIM 
    config_list [in] List of available configs
    num_configs [in] Number of available configs

  RETURN VALUE
    Index in config_list array of the best available config, or
    MCFG_CONFIG_LIST_SIZE_MAX if no suitable match found.

  SIDE EFFECTS
    None

===========================================================================*/
static uint32 mcfg_sel_find_best_match_by_carrier
(
  mcfg_slot_index_type_e_type                sloti,
  pdc_selection_carrier_enum_v01             carrier,
  mcfg_config_id_s_type                     *config_list,
  uint32                                    num_configs
)
{
  uint32 i;
  uint32 selected_config_index = MCFG_CONFIG_LIST_SIZE_MAX;
  mcfg_config_info_s_type selected_config_info;
  mcfg_config_info_s_type current_config_info;
  mcfg_sel_config_match_s_type selected_config_match;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  memset(&selected_config_info, 0, sizeof(selected_config_info));
  memset(&current_config_info, 0, sizeof(current_config_info));
  memset(&selected_config_match, 0, sizeof(selected_config_match));
  for (i = 0; i < num_configs; i++)
  {
    if (!mcfg_utils_get_config_info(
          MCFG_TYPE_SW, &config_list[i], &current_config_info))
    {
      MCFG_MSG_ERROR_1("Couldn't get config info at index %lu", i);
    }
    else if (mcfg_sel_config_is_better_match_by_carrier(
              sloti, carrier, &current_config_info, 
              &selected_config_info, &selected_config_match))
    {
      selected_config_index = i;
      memscpy(&selected_config_info, sizeof(mcfg_config_info_s_type),
              &current_config_info, sizeof(mcfg_config_info_s_type));
    }
  }

  if (selected_config_index == MCFG_CONFIG_LIST_SIZE_MAX)
  {
    MCFG_MSG_HIGH("No configs available for current carrier");
  }

  return selected_config_index;
} /* mcfg_sel_find_best_match_by_carrier() */

/*===========================================================================

  FUNCTION mcfg_sel_iccid_matches_last

  DESCRIPTION
    Checks whether the given ICCID is the same ICCID as we last used for
    ICCID-based autoselection.

  DEPENDENCIES
    None

  PARAMETERS
    iccid_ascii [in] Valid ICCID of the UIM currently inserted in slot 1;
                     ASCII encoding

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
static boolean mcfg_sel_iccid_matches_last
(
  mcfg_sub_id_type_e_type sub_id,
  const char             *iccid_ascii
)
{
  char last_iccid[MCFG_UIM_ICCID_STR_BUF_LEN];
  mcfg_nv_status_e_type status;
  boolean matches = FALSE;
  mcfg_slot_index_type_e_type slot_index;
  mcfg_config_id_s_type  config;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  slot_index = mcfg_uim_map_sub_id_to_slot_index(sub_id);
  MCFG_ASSERT_COND_RET_FALSE(slot_index < MCFG_NUM_OF_UIM_SLOT);

  if (!mcfg_utils_get_active_config(MCFG_TYPE_SW, &config, sub_id))
  {
    MCFG_MSG_LOW_1("there's not active config available for sloti %d, iccid reselect", slot_index);
    return FALSE;
  }

  if (mcfg_utils_selected_config_available(MCFG_TYPE_SW, sub_id))
  {
    MCFG_MSG_LOW_1("there's pending config available for sloti %d, iccid reselect", slot_index);
    return FALSE;
  }

  status = mcfg_nv_read(MCFG_NV_ITEM_LAST_AUTOSELECT_ICCID, 0, slot_index, 
                        last_iccid, sizeof(last_iccid));
  if (status != MCFG_NV_STATUS_OK)
  {
    if (status != MCFG_NV_STATUS_NOT_ACTIVE)
    {
      MCFG_MSG_ERROR_1("Couldn't read last autoselect ICCID NV item: status %d",
                       status);
    }
  }
  else if (strncmp(last_iccid, iccid_ascii, sizeof(last_iccid)) == 0)
  {
    matches = TRUE;
  }

  return matches;
} /* mcfg_sel_iccid_matches_last() */

#ifdef FEATURE_MCFG_SELECTION_BY_IMSI
/*===========================================================================

  FUNCTION mcfg_sel_plmn_matches_last

  DESCRIPTION
    Checks whether the given PLMN is the same PLMN as we last used for
    ICCID-based autoselection.

  DEPENDENCIES
    None

  PARAMETERS
    iccid_ascii [in] Valid ICCID of the UIM currently inserted in slot 1;
                     ASCII encoding

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
static boolean mcfg_sel_plmn_matches_last
(
  mcfg_sub_id_type_e_type          sub_id,
  mcfg_trl_carrier_mcc_mnc_s_type *plmn,
  mcfg_uim_ext_session_e_type      session_type
)
{
  mcfg_trl_carrier_mcc_mnc_s_type last_plmn;
  mcfg_slot_index_type_e_type sloti;
  mcfg_nv_status_e_type status;
  boolean matches = FALSE;
  mcfg_config_id_s_type  config;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  sloti = mcfg_uim_map_sub_id_to_slot_index(sub_id);
  MCFG_ASSERT_COND_RET_FALSE(sloti < MCFG_NUM_OF_UIM_SLOT);

  //Ignore if 1x plmn of CT is present
  if(session_type == MCFG_UIM_EXT_SESSION_GW)
  {
    status = mcfg_nv_read(MCFG_NV_ITEM_LAST_AUTOSELECT_PLMN, 
                          (uint8)MCFG_UIM_EXT_SESSION_1X, 
                          (mcfg_sub_id_type_e_type)sloti, 
                          &last_plmn, sizeof(last_plmn));
    if (status == MCFG_NV_STATUS_OK)
    {
      MCFG_MSG_HIGH_1("1X imsi present, ignore selection based on GW imsi for subi %d", sub_id);
      return TRUE;      
    }
  }

  if (!mcfg_utils_get_active_config(MCFG_TYPE_SW, &config, sub_id))
  {
    MCFG_MSG_LOW_1("there's not active config available for sloti %d, imsi reselect", sloti);
    return FALSE;
  }

  if (mcfg_utils_selected_config_available(MCFG_TYPE_SW, sub_id))
  {
    MCFG_MSG_LOW_1("there's pending config available for sloti %d, imsi reselect", sloti);
    return FALSE;
  }

  status = mcfg_nv_read(MCFG_NV_ITEM_LAST_AUTOSELECT_PLMN, 0, (mcfg_sub_id_type_e_type)sloti, 
                        &last_plmn, sizeof(last_plmn));
  if (status != MCFG_NV_STATUS_OK)
  {
    if (status != MCFG_NV_STATUS_NOT_ACTIVE)
    {
      MCFG_MSG_ERROR_1("Couldn't read last autoselect plmn NV item: status %d",
                       status);
    }
  }
  else if (memcmp(plmn, &last_plmn, sizeof(last_plmn)) == 0)
  {
    matches = TRUE;
  }

  return matches;
} /* mcfg_sel_plmn_matches_last() */
#endif /* FEATURE_MCFG_SELECTION_BY_IMSI */

#ifndef FEATURE_MCFG_SEL_DB
/*===========================================================================

  FUNCTION mcfg_sel_save_iccid

  DESCRIPTION
    Saves the given ICCID to NV cache as the last ICCID we ran
    auto-selection with.

  DEPENDENCIES
    None

  PARAMETERS
    iccid_ascii [in] Valid ICCID of the UIM currently inserted in slot 1;
                     ASCII encoding. Buffer size must be at least
                     MCFG_UIM_ICCID_STR_BUF_LEN bytes, and string must be
                     NULL-terminated

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
static void mcfg_sel_save_iccid
(
  mcfg_sub_id_type_e_type sub_id,
  const char             *iccid_ascii
)
{
  mcfg_nv_status_e_type status;
  mcfg_slot_index_type_e_type slot_index;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_CHECK_NULL_PTR_RET(iccid_ascii);

  slot_index = mcfg_uim_map_sub_id_to_slot_index(sub_id);
  MCFG_ASSERT_COND_RET(slot_index < MCFG_NUM_OF_UIM_SLOT);

  if(!mcfg_uim_nv_autoselect_enabled(slot_index))
  {
    MCFG_MSG_MED("Selection based on ICCID is disabled");
    if (mcfg_sel_iccid_matches_last(sub_id, iccid_ascii))
    {
      MCFG_MSG_HIGH("ICCID matches cache from last autoselect");
    }
    else
    {
      mcfg_sel_sw_config_store_updated(slot_index);
      mcfg_sel_save_iccid(sub_id, iccid_ascii);
    }

    return;
  }

  status = mcfg_nv_write(MCFG_NV_ITEM_LAST_AUTOSELECT_ICCID, 0, slot_index, 
                         (void *) iccid_ascii, MCFG_UIM_ICCID_STR_BUF_LEN);
  if (status != MCFG_NV_STATUS_OK)
  {
    MCFG_MSG_ERROR_1("Couldn't write last autoselect ICCID NV item: status %d",
                     status);
  }
} /* mcfg_sel_save_iccid() */

#ifdef FEATURE_MCFG_SELECTION_BY_IMSI
/*===========================================================================

  FUNCTION mcfg_sel_save_plmn

  DESCRIPTION
    Saves the given plmn to NV cache as the last plmn we ran
    auto-selection with.

  DEPENDENCIES
    None

  PARAMETERS
    plmn  [in] valid plmn of the UIM for specified subscription

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
static void mcfg_sel_save_plmn
(
  mcfg_sub_id_type_e_type          sub_id,
  mcfg_trl_carrier_mcc_mnc_s_type *plmn,
  mcfg_uim_ext_session_e_type      session_type

)
{
  mcfg_nv_status_e_type status;
  mcfg_slot_index_type_e_type sloti;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_CHECK_NULL_PTR_RET(plmn);

  sloti = mcfg_uim_map_sub_id_to_slot_index(sub_id);
  MCFG_ASSERT_COND_RET(sloti < MCFG_NUM_OF_UIM_SLOT);

  status = mcfg_nv_write(MCFG_NV_ITEM_LAST_AUTOSELECT_PLMN, (uint8)session_type, (mcfg_sub_id_type_e_type)sloti, 
                         (void *) plmn, sizeof(mcfg_trl_carrier_mcc_mnc_s_type));
  if (status != MCFG_NV_STATUS_OK)
  {
    MCFG_MSG_ERROR_1("Couldn't write last autoselect PLMN NV item: status %d",
                     status);
  }
} /* mcfg_sel_save_plmn() */
#endif /* FEATURE_MCFG_SELECTION_BY_IMSI */
#endif /* FEATURE_MCFG_SEL_DB */

/*===========================================================================

  FUNCTION mcfg_sel_select_sw_config

  DESCRIPTION
    If the given software configuration is different from the active config,
    then sets it as the new active config.

  DEPENDENCIES
    None

  PARAMETERS
    config_id [in]

  RETURN VALUE
    None

  SIDE EFFECTS
    May trigger a device reset to active a new config

===========================================================================*/
static void mcfg_sel_select_sw_config
(
  mcfg_sub_id_type_e_type sub_id,
  mcfg_config_id_s_type  *config_id,
  mcfg_activation_source_e_type source
)
{
  mcfg_config_id_s_type active_config_id;
#ifdef FEATURE_MCFG_REFRESH
  mcfg_refresh_state_e_type state;
  uint8 slot;
#endif
#ifdef FEATURE_MCFG_FEATURETAG
  uint8 sloti;
#endif
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_MSG_HIGH_3("Autoselect picked config with ID %02X%02X%02X",
                  config_id->id[0], config_id->id[1], config_id->id[2]);
  mcfg_utils_print_config(MCFG_TYPE_SW, config_id);

  if (mcfg_utils_get_active_config(MCFG_TYPE_SW, &active_config_id, sub_id) &&
      mcfg_sel_config_ids_match(config_id, &active_config_id))
  {

#ifdef FEATURE_MCFG_FEATURETAG
    sloti = (uint32) mcfg_uim_map_sub_id_to_slot_index(sub_id);
#endif

    if(mcfg_utils_selected_config_available(MCFG_TYPE_SW, sub_id))
    {
      MCFG_MSG_MED("Pending config is not the same as active one, deact first");
      //set pending config as null first before deactivate current active config
      mcfg_utils_set_selected_config(MCFG_TYPE_SW, NULL, sub_id);
      //deactivate current active config, so new pending one can be selected.
      mcfg_utils_deactivate_config(MCFG_TYPE_SW, sub_id, FALSE);
    }
#ifdef FEATURE_MCFG_FEATURETAG
    else if (mcfg_settings_updated(sloti))
    {
      MCFG_MSG_MED("Features updated after activation, deact curr active config");
      //deactivate current active config, so new pending one can be selected.
      mcfg_utils_deactivate_config(MCFG_TYPE_SW, sub_id, FALSE);
      // new mcfg_settings will be backed up during mbn activation
    }
#endif
    else
    {
      MCFG_MSG_HIGH("Already using correct config");
      return;
    }
  }

  if (!mcfg_utils_set_selected_config(MCFG_TYPE_SW, config_id, sub_id))
  {
    MCFG_MSG_ERROR("Couldn't set selected config!");
  }
  else if (source == MCFG_ACTIVATION_SUBSI_CARRIER && sub_id == MCFG_SUB_ID_FIRST &&
           mcfg_utils_get_active_config(MCFG_TYPE_HW, &active_config_id, MCFG_SUB_ID_FIRST))
  {
    MCFG_MSG_HIGH("Selected config based on carrier on sub0, reload HW config");
    mcfg_utils_deactivate_config(MCFG_TYPE_HW, MCFG_SUB_ID_FIRST, TRUE);
    mcfg_utils_set_selected_config(MCFG_TYPE_HW, &active_config_id, MCFG_SUB_ID_FIRST);
    mcfg_utils_activate_config(MCFG_TYPE_HW, MCFG_ACTIVATION_SUBSI_CARRIER);
  }
  else
  {
    MCFG_MSG_MED("Activating new config");
#ifdef FEATURE_MCFG_REFRESH

    slot = mcfg_refresh_sub_to_slot_index((mcfg_refresh_index_type)sub_id);
    if(mcfg_refresh_autoselect_enabled(slot))
    {
      state = mcfg_refresh_get_state(slot);
      if (state == MCFG_REFRESH_STATE_ACTIVATING_SLOT && source == MCFG_ACTIVATION_ICCID_SELECT)
      {
        MCFG_MSG_MED_1("Refresh clients for slot %d trigged by ICCID selection .....", slot);
        mcfg_utils_deactivate_config(MCFG_TYPE_SW, sub_id, TRUE);
        modem_cfg_process_single(MCFG_TYPE_SW, MCFG_REFRESH_TYPE_SLOT, (mcfg_refresh_index_type)sub_id);
        mcfg_refresh_set_state((mcfg_refresh_index_type)slot, MCFG_REFRESH_STATE_UPDATING_SLOT);
        mcfg_refresh_clients(MCFG_REFRESH_TYPE_SLOT, (mcfg_refresh_index_type)slot);
      }
      else if (source == MCFG_ACTIVATION_IMSI_SELECT)
      {
        MCFG_MSG_MED("Activating new config triggered by IMSI selection .....");
        mcfg_utils_activate_config(MCFG_TYPE_SW, MCFG_ACTIVATION_IMSI_SELECT);
      }
      else if (state == MCFG_REFRESH_STATE_IDLE)
      {
        MCFG_MSG_MED("Activating new config triggered by ICCID selection .....");
        mcfg_utils_activate_config(MCFG_TYPE_SW, MCFG_ACTIVATION_ICCID_SELECT);
      }
      else
      {
        MCFG_MSG_ERROR_2("Invalid state %d for slot %d.....", state, slot);
      }
    }
    else
#endif
    {
#ifndef REMOVE_QMI_PDC_CONFIG_CHANGE_IND_V01
      mcfg_svc_cmd_config_change_ind(MCFG_TYPE_SW, config_id);
#endif
      if (mcfg_utils_activate_config(MCFG_TYPE_SW, source) != MCFG_ERR_NONE)
      {
        MCFG_MSG_ERROR_1("Couldn't activate config! source %d", source);
      }
    }
  }
} /* mcfg_sel_select_sw_config() */

/*===========================================================================

  FUNCTION mcfg_sel_activate_sw_config

  DESCRIPTION
    Reactivate the currently active config when feature change occurs. 
    To be used when modem autoselection is off.

  DEPENDENCIES
    None

  PARAMETERS
    sub_id [in]
    carrier [in]

  RETURN VALUE
    None

  SIDE EFFECTS
    May trigger a device reset to active a new config

===========================================================================*/
void mcfg_sel_activate_sw_config
(
  pdc_selection_carrier_enum_v01 carrier,
  mcfg_sub_id_type_e_type sub_id
)
{
  mcfg_config_id_s_type  sw_config_id;
  mcfg_config_id_s_type  hw_config_id;
  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  //MCFG_CHECK_NULL_PTR_RET(feature);

  if (mcfg_utils_get_active_config(MCFG_TYPE_SW, &sw_config_id, sub_id))
  {
    MCFG_MSG_MED_1("Features updated after activation, reactivate curr active config on sub %d", sub_id);
    if (!mcfg_utils_deactivate_config(MCFG_TYPE_SW, sub_id, FALSE))
    {
      MCFG_MSG_ERROR_1("Couldn't deactivate config on sub %d!", sub_id);
    }
    else if (!mcfg_utils_set_selected_config(MCFG_TYPE_SW, &sw_config_id, sub_id))
    {
      MCFG_MSG_ERROR_1("Couldn't set selected config on sub %d!", sub_id);
    }
    else 
    {
      mcfg_utils_activate_config(MCFG_TYPE_SW, MCFG_ACTIVATION_SUBSI_CARRIER);
    }
  }
} /* mcfg_sel_activate_sw_config() */

/*==============================================================================
                    PUBLIC FUNCTION DECLARATIONS FOR MODULE
==============================================================================*/

#ifndef FEATURE_MCFG_SEL_DB
/*===========================================================================

  FUNCTION mcfg_sel_sw_config_store_updated

  DESCRIPTION
    Notifies the automatic configuration selection module that the stored
    SW configuration files have changed, e.g. new config uploaded, config
    deleted, etc. Ensures that automatic selection logic will run again
    at next boot to verify that we are using the best available config.

  DEPENDENCIES
    None

  PARAMETERS
    slot_index

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
void mcfg_sel_sw_config_store_updated
(
  mcfg_slot_index_type_e_type slot_index
)
{
  mcfg_slot_index_type_e_type start, end, i;
  mcfg_nv_status_e_type status;
  mcfg_uim_ext_session_e_type      session_type;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_ASSERT_COND_RET(slot_index <= MCFG_NUM_OF_UIM_SLOT);

  if (slot_index < MCFG_NUM_OF_UIM_SLOT)
  {
    start = slot_index;
    end = (slot_index+1);
    MCFG_MSG_MED_1("Last saved ICCID and IMSI NVs are deleted for sloti %d", slot_index);
  }
  else
  {
    start = MCFG_SLOT_INDEX_FIRST;
    end = MCFG_NUM_OF_UIM_SLOT;
    MCFG_MSG_MED("Last saved ICCID and IMSI NVs are deleted for all slots");
  }

  /* Erase the cached ICCID/PLMN so auto selection will run on next reboot */
  for (i = start; i < end; i++)
  {
    status = mcfg_nv_delete(MCFG_NV_ITEM_LAST_AUTOSELECT_ICCID, 0, i);
    if (status != MCFG_NV_STATUS_OK && status != MCFG_NV_STATUS_NOT_ACTIVE)
    {
      MCFG_MSG_ERROR_1("Couldn't cleanup last autoselect ICCID NV item: status %d",
                       status);
    }
#ifdef FEATURE_MCFG_SELECTION_BY_IMSI
    for (session_type = MCFG_UIM_EXT_SESSION_GW; session_type < MCFG_UIM_NUM_OF_EXT_SESSION; session_type++)
    {
      status = mcfg_nv_delete(MCFG_NV_ITEM_LAST_AUTOSELECT_PLMN, (uint8)session_type, i);
    if (status != MCFG_NV_STATUS_OK && status != MCFG_NV_STATUS_NOT_ACTIVE)
    {
        MCFG_MSG_ERROR_2("Couldn't cleanup last autoselect PLMN NV item: status %d session %d",
                         status, session_type);
      }
    }
#endif /*FEATURE_MCFG_SELECTION_BY_IMSI*/
  }
} /* mcfg_sel_sw_config_store_updated() */

/*===========================================================================

  FUNCTION mcfg_sel_sw_by_iccid

  DESCRIPTION
    Searches for a MCFG_SW matching the parameters for the given ICCID. If
    a match is found, the MCFG_SW will be selected and activated (if not
    already active).

  DEPENDENCIES
    None

  PARAMETERS
    iccid_ascii [in] Valid ICCID of the UIM currently inserted in slot 1;
                     ASCII encoding. Buffer size must be at least
                     MCFG_UIM_ICCID_STR_BUF_LEN bytes, and string must be
                     NULL-terminated

  RETURN VALUE
    None

  SIDE EFFECTS
    May result in a system reboot if the configuration is to be changed

===========================================================================*/
void mcfg_sel_sw_by_iccid
(
  mcfg_sub_id_type_e_type sub_id,
  const char             *iccid_ascii
)
{
  uint32 num_configs;
  mcfg_config_id_s_type *config_list;
  uint32 selected_config_index;
  mcfg_slot_index_type_e_type sloti;
  mcfg_config_id_s_type active_config_id;
  mcfg_config_id_s_type selected_config_id;
#ifdef FEATURE_MCFG_HW_CONFIG_REQUIRED
  mcfg_config_id_s_type   config_id;
#endif
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_CHECK_NULL_PTR_RET(iccid_ascii);

  sloti = mcfg_uim_map_sub_id_to_slot_index(sub_id);
  MCFG_ASSERT_COND_RET(sloti < MCFG_NUM_OF_UIM_SLOT);

  if(!mcfg_uim_nv_autoselect_enabled(sloti))
  {
    MCFG_MSG_MED("Selection based on ICCID is disabled");
    return;
  }

  if (mcfg_sel_iccid_matches_last(sub_id, iccid_ascii))
  {
    MCFG_MSG_HIGH("ICCID matches cache from last autoselect");
  }
#ifdef FEATURE_MCFG_HW_CONFIG_REQUIRED
  else if (!mcfg_utils_get_active_config(MCFG_TYPE_HW, &config_id, MCFG_SUB_ID_FIRST))
  {
    MCFG_MSG_ERROR("Platform MBN has to be activated first before auto-selection");
  }
#endif
  else
  {
    MCFG_MSG_SPRINTF_1("Running autoselect logic for ICCID: %s", iccid_ascii);
    config_list = mcfg_malloc(
      sizeof(mcfg_config_id_s_type) * MCFG_CONFIG_LIST_SIZE_MAX);
    if (config_list == NULL)
    {
      MCFG_MSG_ERROR("Couldn't allocate memory for config list!");
    }
    else
    {
      num_configs = mcfg_utils_list_configs(
        MCFG_TYPE_SW, MCFG_CONFIG_LIST_SIZE_MAX, config_list);
      if (num_configs == 0)
      {
        MCFG_MSG_HIGH("No configs present");
      }
      else
      {
        selected_config_index = mcfg_sel_find_best_match_by_iccid(
          iccid_ascii, config_list, num_configs);
        if (selected_config_index < MCFG_CONFIG_LIST_SIZE_MAX)
        {
          mcfg_sel_select_sw_config(sub_id, 
                                    &config_list[selected_config_index],
                                    MCFG_ACTIVATION_ICCID_SELECT);
        }
      }

#ifndef FEATURE_MCFG_SEL_DB
      /* Clear the last saved ICCID and IMSI NVs if MBN is getting updated as part of ICCID selection */
      if (mcfg_utils_selected_config_available(MCFG_TYPE_SW, sub_id))
      {
        MCFG_MSG_HIGH_1("MBN being updated as part of ICCID selection. Clean up saved ICCID and IMSI NVs, slot %d", sloti);
        mcfg_sel_sw_config_store_updated(sloti);
      }

      mcfg_sel_save_iccid(sub_id, iccid_ascii);
#endif
      mcfg_free(config_list);
    }
  }
} /* mcfg_sel_sw_by_iccid() */

#ifdef FEATURE_MCFG_SELECTION_BY_IMSI
/*===========================================================================

  FUNCTION mcfg_sel_sw_by_plmn

  DESCRIPTION
    Searches for a MCFG_SW matching the parameters for the given plmn. If
    a match is found, the MCFG_SW will be selected and activated (if not
    already active).

  DEPENDENCIES
    None

  PARAMETERS
    plmn [in] Valid plmn of the UIM currently inserted for specified
              subscription

  RETURN VALUE
    None

  SIDE EFFECTS
    May result in a system reboot if the configuration is to be changed

===========================================================================*/
void mcfg_sel_sw_by_plmn
(
  mcfg_sub_id_type_e_type          sub_id,
  mcfg_trl_carrier_mcc_mnc_s_type *plmn,
  mcfg_uim_ext_session_e_type      session_type
)
{
  uint32 num_configs;
  mcfg_config_id_s_type *config_list;
  uint32 selected_config_index;
  mcfg_slot_index_type_e_type sloti;
#ifdef FEATURE_MCFG_HW_CONFIG_REQUIRED
  mcfg_config_id_s_type   config_id;
#endif
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_CHECK_NULL_PTR_RET(plmn);
  sloti = mcfg_uim_map_sub_id_to_slot_index(sub_id);
  MCFG_ASSERT_COND_RET(sloti < MCFG_NUM_OF_UIM_SLOT);

  if(!mcfg_uim_nv_imsiselect_enabled(sloti))
  {
    MCFG_MSG_MED("Selection based on IMSI is disabled");
    return;
  }

  if (mcfg_sel_plmn_matches_last(sub_id, plmn, session_type))
  {
    MCFG_MSG_HIGH("PLMN matches cache from last autoselect");
  }
#ifdef FEATURE_MCFG_HW_CONFIG_REQUIRED
  else if (!mcfg_utils_get_active_config(MCFG_TYPE_HW, &config_id, MCFG_SUB_ID_FIRST))
  {
    MCFG_MSG_ERROR("Platform MBN has to be activated first before auto-selection");
  }
#endif
  else
  {
    MCFG_MSG_HIGH_2("Running autoselect logic for mcc=%d mnc=%d", 
                    plmn->mcc, plmn->mnc);
    config_list = mcfg_malloc(
      sizeof(mcfg_config_id_s_type) * MCFG_CONFIG_LIST_SIZE_MAX);
    if (config_list == NULL)
    {
      MCFG_MSG_ERROR("Couldn't allocate memory for config list!");
    }
    else
    {
      num_configs = mcfg_utils_list_configs(
        MCFG_TYPE_SW, MCFG_CONFIG_LIST_SIZE_MAX, config_list);
      if (num_configs == 0)
      {
        MCFG_MSG_HIGH("No configs present");
      }
      else
      {
        selected_config_index = mcfg_sel_find_best_match_by_plmn(
          plmn, config_list, num_configs);
        if (selected_config_index < MCFG_CONFIG_LIST_SIZE_MAX)
        {
          mcfg_sel_select_sw_config(sub_id, 
                                    &config_list[selected_config_index],
                                    MCFG_ACTIVATION_IMSI_SELECT);
        }
      }
      mcfg_sel_save_plmn(sub_id, plmn, session_type);
      mcfg_free(config_list);
    }
  }
} /* mcfg_sel_sw_by_plmn */
#endif /* FEATURE_MCFG_SELECTION_BY_IMSI */

#else

boolean mcfg_sel_sw_get_config_by_carrier
(
  mcfg_slot_index_type_e_type    sloti,
  pdc_selection_carrier_enum_v01 carrier,
  mcfg_config_id_s_type         *config_id
)
{
  uint32 num_configs;
  mcfg_config_id_s_type *config_list;
  uint32 selected_config_index;
  boolean found = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_ASSERT_COND_RET_FALSE(carrier != PDC_SELECTION_OPEN_MARKET_V01);

  MCFG_MSG_HIGH_1("Find config for carrier %d", carrier);
  config_list = mcfg_malloc(
    sizeof(mcfg_config_id_s_type) * MCFG_CONFIG_LIST_SIZE_MAX);
  if (config_list == NULL)
  {
    MCFG_MSG_ERROR("Couldn't allocate memory for config list!");
  }
  else
  {
    num_configs = mcfg_utils_list_configs(
      MCFG_TYPE_SW, MCFG_CONFIG_LIST_SIZE_MAX, config_list);
    if (num_configs == 0)
    {
      MCFG_MSG_HIGH("No configs present");
    }
    else
    {
      selected_config_index = mcfg_sel_find_best_match_by_carrier(
        sloti, carrier, config_list, num_configs);
      if (selected_config_index < MCFG_CONFIG_LIST_SIZE_MAX)
      {
        found = TRUE;
        if (config_id)
        {
          memscpy(config_id, sizeof(mcfg_config_id_s_type),
                  &config_list[selected_config_index], 
                  sizeof(mcfg_config_id_s_type));
        }
      }
    }
    mcfg_free(config_list);
  }

  return found;
} /* mcfg_sel_sw_get_config_id_by_carrier */

/*===========================================================================

  FUNCTION mcfg_sel_enforce_selection

  DESCRIPTION
    Make sure the current active config matches the prior selected carrier.

  DEPENDENCIES
    None

  PARAMETERS
    sub_id
 
  RETURN VALUE
    None

===========================================================================*/
void mcfg_sel_enforce_selection
(
  mcfg_sub_id_type_e_type sub_id,
  mcfg_activation_source_e_type    source
)
{
  mcfg_config_id_s_type          config_id;
  mcfg_uim_card_info_s_type      *p_card_info;
  pdc_selection_carrier_enum_v01  carrier_index;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  p_card_info = mcfg_uim_get_card_info_p(mcfg_uim_map_sub_id_to_slot_index(sub_id));
  if (!p_card_info)
  {
    MCFG_MSG_ERROR("no card info retrieved");
    return;
  }

  if (mcfg_utils_get_active_config(MCFG_TYPE_SW, &config_id, sub_id))
  {
    if(mcfg_utils_config_id_matches(&config_id, &p_card_info->config_id))
    {
      MCFG_MSG_HIGH_2("config with carrier %d for subi %d already activated",
                     p_card_info->carrier_index, sub_id);
      return;
    }
  }
  
  if ((p_card_info->config_id.id_len > 0) && 
      !mcfg_utils_is_config_id_unique(MCFG_TYPE_SW, &p_card_info->config_id))
  {
    MCFG_MSG_HIGH_2("enforce config with carrier %d for subi %d to be activated", 
                    p_card_info->carrier_index, sub_id);
    mcfg_sel_select_sw_config(sub_id, &p_card_info->config_id, source);
    return;
  }

  if (mcfg_sel_db_get_carrier_index(mcfg_uim_map_sub_id_to_slot_index(sub_id),
                                    &carrier_index,
                                    &config_id))
  {
     MCFG_MSG_HIGH_2("reselect config with carrier %d for subi %d to be activated", 
                     carrier_index, sub_id);
     mcfg_sel_select_sw_config(sub_id, &config_id, source);
  }
  
  return; 
}

/*===========================================================================

  FUNCTION mcfg_sel_sw_by_iccid

  DESCRIPTION
    Searches for a MCFG_SW matching the parameters for the given ICCID. If
    a match is found, the MCFG_SW will be selected and activated (if not
    already active).

  DEPENDENCIES
    None

  PARAMETERS
    iccid_ascii [in] Valid ICCID of the UIM currently inserted in slot 1;
                     ASCII encoding. Buffer size must be at least
                     MCFG_UIM_ICCID_STR_BUF_LEN bytes, and string must be
                     NULL-terminated

  RETURN VALUE
    None

  SIDE EFFECTS
    May result in a system reboot if the configuration is to be changed

===========================================================================*/
void mcfg_sel_sw_by_iccid
(
  mcfg_sub_id_type_e_type sub_id,
  const char             *iccid_ascii
)
{
  uint32 num_configs;
  mcfg_config_id_s_type *config_list;
  mcfg_slot_index_type_e_type sloti;
  pdc_selection_carrier_enum_v01 carrier_index;
  mcfg_config_id_s_type config_id;
  boolean iccid_changed = FALSE;
  mcfg_uim_card_info_s_type  card_info;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_CHECK_NULL_PTR_RET(iccid_ascii);

  sloti = mcfg_uim_map_sub_id_to_slot_index(sub_id);
  MCFG_ASSERT_COND_RET(sloti < MCFG_NUM_OF_UIM_SLOT);

  iccid_changed = mcfg_uim_update_iccid(sloti, iccid_ascii);

  if(!mcfg_uim_nv_autoselect_enabled(sloti))
  {
    MCFG_MSG_MED("Selection based on ICCID is disabled");
    return;
  }

  if(!iccid_changed)
  {
    MCFG_MSG_MED("no update to ICCID, enforce active config matches selected carrier");

    
    if(mcfg_uim_get_card_info(mcfg_uim_map_sub_id_to_slot_index(sub_id),
                              iccid_ascii,
                              &card_info))
    {
      mcfg_setting_update_feature(mcfg_uim_map_sub_id_to_slot_index(sub_id), 
                                  card_info.country, (uint16)card_info.ims_feature);
      mcfg_sel_enforce_selection(sub_id, MCFG_ACTIVATION_ICCID_SELECT);
    }
    
    return;
  }

  if(mcfg_sel_db_get_carrier_index(sloti, &carrier_index, &config_id))
  {
    MCFG_MSG_SPRINTF_2("selection Carrier %d for ICCID: %s", carrier_index, iccid_ascii);
    mcfg_sel_select_sw_config(sub_id, 
                              &config_id,
                              MCFG_ACTIVATION_ICCID_SELECT);

  }
#ifdef FEATURE_MCFG_FEATURETAG
  else if (mcfg_settings_updated(sloti))
  {
    if (mcfg_utils_get_active_config(MCFG_TYPE_SW, &config_id, sub_id))
    {
      //deactivate current active config, and reactivate due to feature update
      mcfg_utils_deactivate_config(MCFG_TYPE_SW, sub_id, FALSE);
      mcfg_sel_select_sw_config(sub_id, 
                                &config_id,
                                MCFG_ACTIVATION_ICCID_SELECT);
    }
  }
#endif

} /* mcfg_sel_sw_by_iccid() */

/*===========================================================================

  FUNCTION mcfg_sel_sw_by_plmn

  DESCRIPTION
    Searches for a MCFG_SW matching the parameters for the given plmn. If
    a match is found, the MCFG_SW will be selected and activated (if not
    already active).

  DEPENDENCIES
    None

  PARAMETERS
    plmn [in] Valid plmn of the UIM currently inserted for specified
              subscription

  RETURN VALUE
    None

  SIDE EFFECTS
    May result in a system reboot if the configuration is to be changed

===========================================================================*/
void mcfg_sel_sw_by_plmn
(
  mcfg_sub_id_type_e_type          sub_id,
  mcfg_trl_carrier_mcc_mnc_s_type *plmn,
  mcfg_uim_ext_session_e_type      session_type
)
{
  mcfg_slot_index_type_e_type sloti;
  pdc_selection_carrier_enum_v01 carrier_index;
  mcfg_config_id_s_type config_id;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_CHECK_NULL_PTR_RET(plmn);
  sloti = mcfg_uim_map_sub_id_to_slot_index(sub_id);
  MCFG_ASSERT_COND_RET(sloti < MCFG_NUM_OF_UIM_SLOT);

  if(!mcfg_uim_nv_imsiselect_enabled(sloti))
  {
    MCFG_MSG_MED("Selection based on IMSI is disabled");
    return;
  }

  if(!mcfg_uim_read_and_update_uimdata(sloti, session_type, plmn))
  {
#ifndef FEATURE_MCFG_FEATURETAG
    return;
#else
    if (mcfg_settings_updated(sloti))
    {
      MCFG_MSG_MED("Features updated since previous activation");
    }
    else 
    {
      MCFG_MSG_MED("Features not updated, ignore");
      return;
    }
#endif
  }

  if(mcfg_sel_db_get_carrier_index(sloti, &carrier_index, &config_id))
  {
    MCFG_MSG_MED_3("selection Carrier %d for mccmnc: %d-%d", carrier_index, plmn->mcc, plmn->mnc);
    mcfg_sel_select_sw_config(sub_id, 
                              &config_id,
                              MCFG_ACTIVATION_IMSI_SELECT);

  }
#ifdef FEATURE_MCFG_FEATURETAG
  else if (mcfg_settings_updated(sloti))
  {
    if (mcfg_utils_get_active_config(MCFG_TYPE_SW, &config_id, sub_id))
    {
      //deactivate current active config, and reactivate due to feature update
      mcfg_utils_deactivate_config(MCFG_TYPE_SW, sub_id, FALSE);
      mcfg_sel_select_sw_config(sub_id, 
                                &config_id,
                                MCFG_ACTIVATION_IMSI_SELECT);
    }
  }
#endif

} /* mcfg_sel_sw_by_plmn */
#endif /* MCFG_XML_DB_SELECTION */

/*===========================================================================

  FUNCTION mcfg_sel_sw_by_carrier

  DESCRIPTION
    Searches for Carrier MBN 

  DEPENDENCIES
    None

  PARAMETERS
    plmn [in] Carrier Index

  RETURN VALUE
    None

  SIDE EFFECTS
    May result in a system reboot if the configuration is to be changed

===========================================================================*/
void mcfg_sel_sw_by_carrier
(
  mcfg_sub_id_type_e_type          sub_id,
  pdc_selection_carrier_enum_v01   carrier,
  mcfg_activation_source_e_type    source
)
{
  uint32 num_configs;
  mcfg_config_id_s_type *config_list;
  uint32 selected_config_index;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_ASSERT_COND_RET(carrier != PDC_SELECTION_OPEN_MARKET_V01);

  MCFG_MSG_HIGH_2("Running autoselect logic for subi %d carrier %d", 
                  sub_id, carrier);
  config_list = mcfg_malloc(
    sizeof(mcfg_config_id_s_type) * MCFG_CONFIG_LIST_SIZE_MAX);
  if (config_list == NULL)
  {
    MCFG_MSG_ERROR("Couldn't allocate memory for config list!");
  }
  else
  {
    num_configs = mcfg_utils_list_configs(
      MCFG_TYPE_SW, MCFG_CONFIG_LIST_SIZE_MAX, config_list);
    if (num_configs == 0)
    {
      MCFG_MSG_HIGH("No configs present");
    }
    else
    {
      selected_config_index = mcfg_sel_find_best_match_by_carrier(
        mcfg_uim_map_sub_id_to_slot_index(sub_id), 
        carrier, config_list, num_configs);
      if (selected_config_index < MCFG_CONFIG_LIST_SIZE_MAX)
      {
        mcfg_sel_select_sw_config(sub_id, 
                                  &config_list[selected_config_index],
                                  source);
      }
    }
    mcfg_free(config_list);
  }
} /* mcfg_sel_sw_by_carrier */

/*===========================================================================

  FUNCTION mcfg_sel_hw_is_best_match_by_setting

  DESCRIPTION
    Finds the best available hardware config that matches with 
    the given filter/setting 

  DEPENDENCIES
    None

  PARAMETERS
    feature [in] pointer to feature setting
    config_info [in] pointer to config trailer records
    filter [in] filter type that used for matching
    

  RETURN VALUE
    TRUE if matches, otherwise FALSE.

  SIDE EFFECTS
    None

===========================================================================*/
static boolean mcfg_sel_hw_is_best_match_by_setting
(
  pdc_get_feature_ind_msg_v01 *pFeature,
  mcfg_config_info_s_type     *config_info,
  mcfg_sel_hw_filter_e_type    filter_type
)
{
  mcfg_trl_category_s_type category;
  char name[MCFG_TRL_CARRIER_NAME_MAX_LEN + 1];
  boolean result = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (mcfg_trl_decode_tlv(config_info, MCFG_TRL_TLV_ID_CARRIER_NAME,
                          name, MCFG_TRL_CARRIER_NAME_MAX_LEN + 1))
  {
    MCFG_MSG_SPRINTF_1("Config: %s", name);
  }

  do
  {
     if (config_info->description_len == 0)
    {
      MCFG_MSG_ERROR("Candidate config trailer record is empty!");
      break;
    }
     
    if (!mcfg_trl_decode_tlv(config_info, MCFG_TRL_TLV_ID_CATEGORY,
                                  &category, sizeof(mcfg_trl_category_s_type)))
    {
      MCFG_MSG_ERROR("Couldn't decode category");
      break;
    }

    switch(filter_type)
    {
      case MCFG_SEL_HW_FILTER_CARRIER:
        if(!pFeature->carrier_valid)
        {
          result = TRUE;
        }
        else if((category.category & MCFG_CATEGORY_BITMASK_SUBSI) == 0)
        {
          result = (pFeature->carrier == PDC_SELECTION_OPEN_MARKET_V01);
        }
        else if(pFeature->carrier == PDC_SELECTION_OPEN_MARKET_V01)
        {
          result = FALSE;
        }
        else
        {
          result = (pFeature->carrier == 
            ((category.category & MCFG_CATEGORY_BITMASK_CARRIER) >> MCFG_CATEGORY_BIT_CARRIER));
        }
        MCFG_MSG_MED_1("carrier matching result: %d", result);
        break;

      case MCFG_SEL_HW_FILTER_MULTISIM:
        if(!pFeature->multisim_valid)
        {
          result = TRUE;
        }
        else if((category.category & MCFG_CATEGORY_BITMASK_DUALS) == 0)
        {
          result = (pFeature->multisim == PDC_DEVICE_MODE_SS_V01);
        }
        else
        {
          result = (pFeature->multisim != PDC_DEVICE_MODE_SS_V01);
        }
        MCFG_MSG_MED_1("MULTISIM matching result: %d", result);
        break;
      
      default:
        MCFG_MSG_MED_1("filter type %d not yet supported ", filter_type);
        break;
    }
  }
  while (0);
  
  return result;
} /* mcfg_sel_hw_is_best_match_by_setting() */

/*===========================================================================

  FUNCTION mcfg_sel_hw_config

  DESCRIPTION
    Searches for HW MBN based on setting

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    May result in a system reboot if the configuration is to be changed

===========================================================================*/
mcfg_error_e_type mcfg_sel_hw_config
(
  void
)
{
  pdc_get_feature_ind_msg_v01 feature;
  uint32 num_configs;
  uint32 i;
  mcfg_config_id_s_type *config_list;
  boolean *pListFilter = NULL;
  boolean *pListFilterTemp = NULL;
  uint32 found = 0;
  uint32 selected_config_index = MCFG_CONFIG_LIST_SIZE_MAX;
  mcfg_sel_hw_filter_e_type filter_i;
  mcfg_config_info_s_type config_info;
  mcfg_error_e_type result = MCFG_ERR_NONE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  MCFG_ASSERT_COND_RET_ERROR(mcfg_get_feature(
                        MCFG_SLOT_INDEX_FIRST, 
#ifdef FEATURE_MCFG_FEATURETAG
                        MCFG_SETTING_CURR,
#endif
                        &feature) == MCFG_ERR_NONE, MCFG_ERR_INTERNAL);

  MCFG_MSG_HIGH("Running autoselect logic for HW MBN ");
 
  config_list = mcfg_malloc(
    sizeof(mcfg_config_id_s_type) * MCFG_CONFIG_LIST_SIZE_MAX);
  if (config_list == NULL)
  {
    MCFG_MSG_ERROR("Couldn't allocate memory for config list!");
    result = MCFG_ERR_INTERNAL;
  }
  else
  {
    num_configs = mcfg_utils_list_configs(
      MCFG_TYPE_HW, MCFG_CONFIG_LIST_SIZE_MAX, config_list);
    if (num_configs == 0)
    {
      MCFG_MSG_HIGH("No HW configs present");
      result = MCFG_ERR_NOT_FOUND;
    }
    else
    {
      pListFilter = mcfg_malloc(2*num_configs*sizeof(*pListFilter));
      if(!pListFilter)
      {
        MCFG_MSG_ERROR("Couldn't allocate memory for pListFilter!");
        result = MCFG_ERR_INTERNAL;
      }
      else
      {
        memset(pListFilter, TRUE, 2*num_configs*sizeof(*pListFilter));
        pListFilterTemp = pListFilter+ num_configs;
        for (filter_i = MCFG_SEL_HW_FILTER_MIN; filter_i < MCFG_SEL_HW_FILTER_NUMS; filter_i++)
        {
          memset(pListFilterTemp, 0, num_configs*sizeof(*pListFilter));
          for (i = 0, found = 0; i < num_configs; i++)
          {
            if(pListFilter[i])
            {
              memset(&config_info, 0, sizeof(config_info));
              if (!mcfg_utils_get_config_info(
                    MCFG_TYPE_HW, &config_list[i], &config_info))
              {
                MCFG_MSG_ERROR("Couldn't get config info");
                pListFilterTemp[i] = FALSE;
                continue;
              }

#ifdef FEATURE_MCFG_FEATURETAG
              if(mcfg_sel_config_is_feature_tagged(&config_info))
              {
                MCFG_MSG_MED_1("mcfg_sel_hw found feature_tagged config %d", i);
                selected_config_index = i;
                //TODO: find latest version if multiple feature tagged mbns are found
                goto Done;
              }
#endif
              pListFilterTemp[i] = mcfg_sel_hw_is_best_match_by_setting(
                                      &feature, &config_info, filter_i);
              if(pListFilterTemp[i] == TRUE)
              {
                found++;
              }
            }
            else
            {
              pListFilterTemp[i] = FALSE;
            }
          }
       
          if(found > 0)
          {
            memscpy(pListFilter, num_configs*sizeof(*pListFilter),
                    pListFilterTemp, num_configs*sizeof(*pListFilter));
          }
        }

        for (i = 0, found = 0; i < num_configs; i++)
        {
          if(pListFilter[i] == TRUE)
          {
            if(selected_config_index == MCFG_CONFIG_LIST_SIZE_MAX)
            {
              selected_config_index = i;
            }
            found++;
          }
        }
        MCFG_MSG_MED_3("mcfg_sel_hw total %d found %d select %d", num_configs, found, selected_config_index);
        
        Done:
        if (selected_config_index < MCFG_CONFIG_LIST_SIZE_MAX)
        {
          if(mcfg_utils_set_selected_config(MCFG_TYPE_HW,
                                         &config_list[selected_config_index],
                                         MCFG_SUB_ID_FIRST))
          {
            MCFG_MSG_MED("mcfg_sel_hw activate new hw config...");
            result = mcfg_utils_activate_config(MCFG_TYPE_HW, MCFG_ACTIVATION_HW_SELECTION);
          }
          else
          {
            MCFG_MSG_ERROR("Cannot set selected HW config");
            result = MCFG_ERR_INTERNAL;
          }
        }
        else
        {
          result = MCFG_ERR_NOT_FOUND;
        }
      }
    }
    mcfg_free(pListFilter);
    mcfg_free(config_list);
  }

  return result;
} /* mcfg_sel_hw_config */
