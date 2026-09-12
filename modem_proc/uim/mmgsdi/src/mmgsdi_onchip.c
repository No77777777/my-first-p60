/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


                M M G S D I   O N C H I P   F U N C T I O N S


GENERAL DESCRIPTION

  This source file contains the ONCHIP related functions.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2016 - 2017 ,2019 - 2020 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/mmgsdi/src/mmgsdi_onchip.c#4 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/23/20   vgd     Added support for get_app_capabilities_sync in onchip mode
02/25/20   vgd     Build onchip auth response data is 3GPP spec defined format
03/04/19   vm      Consider perso status along with app status to move to post perso step 
05/23/17   nr      DTF handlers for session activation and de-activation
03/14/17   nc      Better checks for return values  
01/12/17   nc      Better check of return values 
01/03/17   sk      Removed USIM spec version
09/06/16   me      Update pin2 status to enabled_not_verified
08/30/16   bcho    Optimization of MMGSDI attribute cache access functions
07/29/16   bcho    Drop older SIMLOCK RUN algo response if new request sent
06/22/16   tkl     Stub out additional APIs
06/14/16   sp      Remove command type parameter from validate_client_id function
06/10/16   sp      Move onchip entry function to onchip file
06/05/16   vdc     Loop through only for number of sessions for updations
05/20/16   tkl     Review of macros used by MMGSDI
05/20/16   vdc     Remove F3 messages for memory allocation failure
05/12/16   sp      Send Power down command to drivers when onchip is activated
05/10/16   shr     Avoid duplicated tags/invalid length in OnChip config
05/10/16   vdc     Feature guard with FEATURE_MMGSDI_3GPP2 for 3GPP2 related functions
04/27/16   sp      Fix low level compiler warnings
04/05/16   sp      Initial revision

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "mmgsdi.h"
#include "uim_msg.h"
#include "mmgsdiutil.h"
#include "mmgsdicache.h"
#include "mmgsdi_common_rsp.h"
#include "mmgsdi_nv.h"
#include "mmgsdi_gen.h"
#include "mmgsdi_cnf.h"
#include "mmgsdi_card_init.h"
#include "mmgsdi_evt.h"
#include "mmgsdi_uicc.h"
#include "mmgsdi_icc.h"
#ifdef FEATURE_SIMLOCK
#include "mmgsdi_simlock.h"
#endif /* FEATURE_SIMLOCK */

#ifdef FEATURE_LTE
#include "lte_aka_wrap.h"
#endif /* FEATURE_LTE */


/*===========================================================================

           DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

/* Macros to set and check if a tag for USIM authenticate is present
  to avoid an invalid configuration in the onchip TLV */
#define MMGSDI_SET_AUTH_TAG_PRESENT(var, tag)    { (var) |= (uint32)(1 << (tag)); }
#define MMGSDI_IS_AUTH_TAG_PRESENT(var, tag)     ( (var) & (uint32)(1 << (tag)) )

/* Length of AUTN of AKA algorithm */
#define MMGSDI_AUTN_LEN                 16

/* Position of RAND data len*/
#define MMGSDI_RAND_LEN_POS             0

/* Position of RAND data */
#define MMGSDI_RAND_DATA_POS            1

/* Position of AUTN data len */
#define MMGSDI_AUTN_LEN_POS             (MMGSDI_RAND_DATA_POS + MMGSDI_RAND_LEN)

/* Position of AUTN data*/
#define MMGSDI_AUTN_DATA_POS            (MMGSDI_AUTN_LEN_POS + 1)

/* Size of tag in auth response */
#define MMGSDI_AUTH_RESP_TAG_LEN        1

/* Position of length in auth response */
#define MMGSDI_AUTH_RESP_LEN_POS        1

/* Position of data in auth response */
#define MMGSDI_AUTH_RESP_DATA_POS       2

/* Default value for the onchip USIM authenticate
  in case of GSM security context */
const static uint8 default_gsm_auth_rsp_ptr[] = { 0x04, 0x2B, 0x4D, 0x56,
                                                  0x5D, 0x08, 0x02, 0x0B,
                                                  0x01, 0x01, 0x19, 0x00,
                                                  0x77, 0x67 };

/* Default value for the onchip USIM authenticate
  in case of 3G security context */
const static uint8 default_3g_auth_rsp_ptr[]  = { 0xDB, 0x10, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x10, 0x00,
                                                  0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x10,
                                                  0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00,
                                                  0x08, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00,
                                                  0x00 };

/* ===========================================================================
  STRUCTURE:      MMGSDI_ONCHIP_TAG_TO_EF_TYPE

===========================================================================*/
typedef PACKED struct PACKED_POST
{
 mmgsdi_activate_onchip_tlv_enum_type      tag;
 mmgsdi_file_enum_type                     usim_ef;
 mmgsdi_file_enum_type                     gsm_ef;
} mmgsdi_onchip_tag_to_ef_type;


static const mmgsdi_onchip_tag_to_ef_type mmgsdi_onchip_tag_to_ef_table[] =
{
 { MMGSDI_ACT_ONCHIP_IMSI_TAG,      MMGSDI_USIM_IMSI,      MMGSDI_GSM_IMSI},
 { MMGSDI_ACT_ONCHIP_LOCI_TAG,      MMGSDI_USIM_LOCI,      MMGSDI_GSM_LOCI},
 { MMGSDI_ACT_ONCHIP_KC_TAG,        MMGSDI_USIM_KC,        MMGSDI_GSM_KC},
 { MMGSDI_ACT_ONCHIP_PLMN_TAG,      MMGSDI_NO_FILE_ENUM,   MMGSDI_GSM_PLMN},
 { MMGSDI_ACT_ONCHIP_SST_TAG,       MMGSDI_NO_FILE_ENUM,   MMGSDI_GSM_SST},
 { MMGSDI_ACT_ONCHIP_ACC_TAG,       MMGSDI_USIM_ACC,       MMGSDI_GSM_ACC},
 { MMGSDI_ACT_ONCHIP_FPLMN_TAG,     MMGSDI_USIM_FPLMN,     MMGSDI_GSM_FPLMN},
 { MMGSDI_ACT_ONCHIP_PHASE_TAG,     MMGSDI_NO_FILE_ENUM,   MMGSDI_GSM_PHASE},
 { MMGSDI_ACT_ONCHIP_AD_TAG,        MMGSDI_USIM_AD,        MMGSDI_GSM_AD},
 { MMGSDI_ACT_ONCHIP_PLMNSEL_TAG,   MMGSDI_NO_FILE_ENUM,   MMGSDI_GSM_HPLMN},
 { MMGSDI_ACT_ONCHIP_EPSLOCI_TAG,   MMGSDI_USIM_EPSLOCI,   MMGSDI_NO_FILE_ENUM},
 { MMGSDI_ACT_ONCHIP_HPLMNWACT_TAG, MMGSDI_USIM_HPLMNWACT, MMGSDI_NO_FILE_ENUM},
 { MMGSDI_ACT_ONCHIP_PLMNWACT_TAG,  MMGSDI_USIM_PLMNWACT,  MMGSDI_NO_FILE_ENUM},
 { MMGSDI_ACT_ONCHIP_UST_TAG,       MMGSDI_USIM_UST,       MMGSDI_NO_FILE_ENUM},
 { MMGSDI_ACT_ONCHIP_EST_TAG,       MMGSDI_USIM_EST,       MMGSDI_NO_FILE_ENUM},
 { MMGSDI_ACT_ONCHIP_EPSNSC_TAG,    MMGSDI_USIM_EPSNSC,    MMGSDI_NO_FILE_ENUM},
 { MMGSDI_ACT_ONCHIP_KEYSPS_TAG,    MMGSDI_USIM_KEYSPS,    MMGSDI_NO_FILE_ENUM},
 { MMGSDI_ACT_ONCHIP_RPLMNACT_TAG,  MMGSDI_USIM_RPLMNACT,  MMGSDI_NO_FILE_ENUM},
 { MMGSDI_ACT_ONCHIP_KEYS_TAG,      MMGSDI_USIM_KEYS,      MMGSDI_NO_FILE_ENUM},
 { MMGSDI_ACT_ONCHIP_KCGPRS_TAG,    MMGSDI_USIM_KCGPRS,    MMGSDI_GSM_KCGPRS}
}; /* mmgsdi_onchip_tag_to_ef_table */


/*===========================================================================
FUNCTION MMGSDI_ONCHIP_INIT_AUTO_ACTIVATION_DATA

DESCRIPTION
  Reads the EFS item to figure out if the OnChip should be enabled on boot up
  or not. If it dictates that OnChip be auto activated on boot up, the EFS
  item contents are validated and are read into the MMGSDI global. This is done
  for all the instances of this EFS item (assuming the corresponding slot index
  is enabled via NV 70210.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE:
  None

SIDE EFFECTS
  None
===========================================================================*/
void mmgsdi_onchip_init_auto_activation_data (
  uint8 index
)
{
  int                                      efs_data_len         = 0;
  mmgsdi_app_enum_type                     mmgsdi_app_type      = MMGSDI_APP_NONE;
  boolean                                  auto_act_enabled     = FALSE;
  uint8                                    efs_slot_index       = MMGSDI_SLOT_1_INDEX;
  uim_common_efs_context_type              efs_context          = UIM_COMMON_EFS_CONTEXT_MAX;

  struct
  {
    uint8   auto_activation_enabled;
    uint8   app_type;
    uint8   slot_id;
    uint8   rfu[13];
  } onchip_auto_act_data;

  efs_context = mmgsdi_util_get_common_efs_item_context(index);
  if (efs_context == UIM_COMMON_EFS_CONTEXT_MAX)
  {
    return;
  }

  if(mmgsdi_generic_data_ptr == NULL)
  {
    return;
  }

  memset(&onchip_auto_act_data, 0x00, sizeof(onchip_auto_act_data));

  /* Lets parse the EFS data; we copy it to our global only if it is valid */
  if (uim_common_efs_read(UIM_COMMON_EFS_MMGSDI_ONCHIP_SUPPORT,
                          UIM_COMMON_EFS_ITEM_FILE_TYPE,
                          efs_context,
                          (uint8 *)&onchip_auto_act_data,
                          sizeof(onchip_auto_act_data)) !=
        UIM_COMMON_EFS_SUCCESS)
  {
    UIM_MSG_ERR_1("Invalid onchip EFS item len:0x%x", efs_data_len);
    return;
  }

  if(MMGSDI_SUCCESS != mmgsdi_util_get_slot_index(onchip_auto_act_data.slot_id,
                                                  &efs_slot_index) ||
     index != efs_slot_index)
  {
    return;
  }

  /* Validate the input that indicates whether onchip auto activation is
     enabled or not.  */
  switch(onchip_auto_act_data.auto_activation_enabled)
  {
    case 1:
      auto_act_enabled = TRUE;
      break;
    case 0:
    default:
      /* EFS item suggested that onchip auto-activation should not be performed.
         No need to validate rest of the contents of the EFS item */
      return;
  }

  /* Validate app type from the EFS item. OnChip currently supported only
     for SIM and USIM */
  switch(onchip_auto_act_data.app_type)
  {
    case 1:
      mmgsdi_app_type = MMGSDI_APP_SIM;
      break;
    case 3:
      mmgsdi_app_type = MMGSDI_APP_USIM;
      break;
    default:
      UIM_MSG_ERR_1("Invalid app type mentioned in OnChip EFS item: 0%x",
                    onchip_auto_act_data.app_type);
      return;
  }

  mmgsdi_generic_data_ptr->onchip_act_data[index].auto_activation_enabled =
    auto_act_enabled;
  mmgsdi_generic_data_ptr->onchip_act_data[index].app_type =
    mmgsdi_app_type;
} /* mmgsdi_onchip_init_auto_activation_data */


/* ============================================================================

   FUNCTION:      MMGSDI_ONCHIP_GET_FILE_ATTR

   DESCRIPTION:
     This function returns file attribute for onchip files
     Currently support ICCID only

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type

     MMGSDI_SUCCESS:          The funtion was successful in getting the file
                              attributes of a particular file.
     MMGSDI_ERROR:
     MMGSDI_INCORRECT_PARAMS: The parameters supplied to the function are not
                              within appropriate ranges.

   SIDE EFFECTS:
     None

=============================================================================*/
mmgsdi_return_enum_type mmgsdi_onchip_get_file_attr(
  mmgsdi_get_file_attr_req_type * req_ptr,
  mmgsdi_protocol_enum_type       protocol
)
{
  mmgsdi_return_enum_type             mmgsdi_status   = MMGSDI_ERROR;
  mmgsdi_client_req_extra_info_type  *extra_param_ptr = NULL;
  mmgsdi_sw_status_type               status_word     = {GSDI_SIM_SW1_NORM_END,
                                                         GSDI_SIM_SW2_NORM_END};
  mmgsdi_data_type                    rsp_data        = {0, NULL};
  /* Default File Attribute for onchip EFs
     Currently support ICCID only since it is received as part of link established
     */
   uint8             default_onchip_iccid_attr_usim[] = {0x62, 0x1C, 0x82, 0x02, 0x41,
                                                         0x21, 0x83, 0x02, 0x2F, 0xE2,
                                                         0xA5, 0x03, 0x80, 0x01, 0x71,
                                                         0x8A, 0x01, 0x05, 0x8B, 0x03,
                                                         0x2F, 0x06, 0x03, 0x80, 0x02,
                                                         0x00, 0x0A, 0x88, 0x01, 0x10};

   uint8             default_onchip_iccid_attr_sim[]  = {0x00, 0x00, 0x00, 0x0A, 0x2F,
                                                         0xE2, 0x04, 0x00, 0x0F, 0x00,
                                                         0xFF, 0x01, 0x02, 0x00, 0x00};

  MMGSDIUTIL_RETURN_IF_NULL_2(req_ptr, req_ptr->request_header.response_cb);

  if (MMGSDI_EF_ENUM_ACCESS == req_ptr->access.access_method &&
      MMGSDI_ICCID          == req_ptr->access.file.file_enum)
  {
    if (MMGSDI_UICC == protocol)
    {
      rsp_data.data_len = sizeof(default_onchip_iccid_attr_usim);
      rsp_data.data_ptr = default_onchip_iccid_attr_usim;
    }
    else if (MMGSDI_ICC == protocol)
    {
      rsp_data.data_len = sizeof(default_onchip_iccid_attr_sim);
      rsp_data.data_ptr = default_onchip_iccid_attr_sim;
    }
    else
    {
      return MMGSDI_ERROR;
    }

    /* Allocate and populate extra parameter */
    mmgsdi_status = mmgsdi_util_alloc_and_populate_extra_param(
                          MMGSDI_GET_FILE_ATTR_REQ,
                          (void*)req_ptr,
                          &extra_param_ptr);
    if (mmgsdi_status != MMGSDI_SUCCESS || extra_param_ptr == NULL)
    {
      return MMGSDI_ERROR;
    }

    mmgsdi_status = mmgsdi_cnf_build_and_queue(MMGSDI_SUCCESS,
                                               &req_ptr->request_header,
                                               extra_param_ptr,
                                               (void*)&rsp_data,
                                               TRUE,
                                               status_word);

    /* Free allocated extra_param_ptr */
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
  }

  return mmgsdi_status;
}/* mmgsdi_onchip_get_file_attr */


/* ============================================================================
  FUNCTION:      MMGSDI_ONCHIP_VALIDATE_AND_POPULATE_SLOT_CACHE

  DESCRIPTION:
    This function is called to do following:
      1  Populate the ICCID data from configuration data provided
         in the ACTIVATE_ONCHIP_USIM/SIM Command.
      2  if IMSI is present check whether it is a test IMSI or not

  DEPENDENCIES:
    Can only be called after the mmgsdi_util_validate_onchip_config()

  LIMITATIONS:

  RETURN VALUE:
    mmgsdi_return_enum_type

    MMGSDI_SUCCESS:          The command processing was successful.
    MMGSDI_ERROR:            The command processing was not successful.
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                             within appropriate ranges.

  SIDE EFFECTS:

============================================================================*/
static mmgsdi_return_enum_type mmgsdi_onchip_validate_and_populate_slot_cache(
  mmgsdi_onchip_sim_data_type *onchip_data_ptr,
  mmgsdi_slot_id_enum_type     slot_id
)
{
  mmgsdi_data_type              tlv_imsi_data;
  mmgsdi_imsi_data_type         imsi             = {{0}};
  mmgsdi_return_enum_type       mmgsdi_status    = MMGSDI_ERROR;
  mmgsdi_session_type_enum_type session_type     = MMGSDI_MAX_SESSION_TYPE_ENUM;

  memset(&tlv_imsi_data, 0, sizeof(mmgsdi_data_type));

  MMGSDIUTIL_RETURN_IF_NULL(onchip_data_ptr);

  /* Make sure that IMSI is one of the test IMSIs. For all other IMSIs,
   return ERROR so that we do not end up activating onchip for fake
   IMSIs (to avoid a situation where someone can for example make an
   emergency call using this fake IMSI) */

  tlv_imsi_data.data_len = onchip_data_ptr->onchip_tlv[MMGSDI_ACT_ONCHIP_IMSI_TAG].len;
  tlv_imsi_data.data_ptr = onchip_data_ptr->onchip_tlv[MMGSDI_ACT_ONCHIP_IMSI_TAG].data_ptr;

  if(tlv_imsi_data.data_ptr != NULL)
  {
    mmgsdi_status = mmgsdi_util_convert_to_imsi_data_type(&tlv_imsi_data,
                                                          &imsi);
    if (mmgsdi_status != MMGSDI_SUCCESS )
    {
      return mmgsdi_status;
    }

    if(FALSE == mmgsdi_card_init_is_imsi_test_imsi(imsi))
    {
      MMGSDI_DEBUG_MSG_ERROR_0("Configured IMSI is not a test IMSI for Onchip");
      return MMGSDI_ERROR;
    }
  }

  /* If ICCID tag is present in onchip configuration data then write it to the cache*/
  if(onchip_data_ptr->onchip_tlv[MMGSDI_ACT_ONCHIP_ICCID_TAG].data_ptr != NULL)
  {
    switch(slot_id)
    {
     case MMGSDI_SLOT_1:
       session_type = MMGSDI_CARD_SESSION_SLOT_1;
       break;
     case MMGSDI_SLOT_2:
       session_type = MMGSDI_CARD_SESSION_SLOT_2;
       break;
     case MMGSDI_SLOT_3:
       session_type = MMGSDI_CARD_SESSION_SLOT_3;
       break;
     default:
       MMGSDI_DEBUG_MSG_ERROR_0("Invalid slot id");
       return MMGSDI_INCORRECT_PARAMS;
    }

    mmgsdi_status = mmgsdi_cache_write_item(MMGSDI_ICCID,
                                           session_type,
                                           onchip_data_ptr->onchip_tlv[MMGSDI_ACT_ONCHIP_ICCID_TAG].len,
                                           0,
                                           onchip_data_ptr->onchip_tlv[MMGSDI_ACT_ONCHIP_ICCID_TAG].data_ptr,
                                           MMGSDI_DATA_FROM_CARD_COMPLETE,
                                           0);

    if ( mmgsdi_status != MMGSDI_SUCCESS )
    {
      MMGSDI_DEBUG_MSG_ERROR_0("Failed to Write Cache Data: 0x%x");
      return mmgsdi_status;
    }
  }

  return MMGSDI_SUCCESS;
} /* mmgsdi_onchip_validate_and_populate_slot_cache */


/*===========================================================================
FUNCTION: MMGSDI_ONCHIP_GET_EF_ENUM_FROM_ONCHIP_TAG

DESCRIPTION
  Maps ONCHIP tags to USIM or GSM EF enum depending on the onchip mode
  passed in as parameter.

DEPENDENCIES
  None

RETURN VALUE
  mmgsdi_file_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static mmgsdi_file_enum_type mmgsdi_onchip_get_ef_enum_from_onchip_tag (
  mmgsdi_activate_onchip_tlv_enum_type tag,
  mmgsdi_onchip_mode_enum_type         onchip_mode)
{
  uint8 table_size = 0;
  uint8 index      = 0;

  if (onchip_mode != MMGSDI_ONCHIP_MODE_SIM &&
      onchip_mode != MMGSDI_ONCHIP_MODE_USIM)
  {
    return MMGSDI_NO_FILE_ENUM;
  }

  table_size =
    sizeof(mmgsdi_onchip_tag_to_ef_table) / sizeof(mmgsdi_onchip_tag_to_ef_table[0]);

  for(index = 0; index < table_size; index++)
  {
    if(mmgsdi_onchip_tag_to_ef_table[index].tag == tag)
    {
      return (onchip_mode == MMGSDI_ONCHIP_MODE_USIM) ?
                mmgsdi_onchip_tag_to_ef_table[index].usim_ef : mmgsdi_onchip_tag_to_ef_table[index].gsm_ef;
    }
  }

  MMGSDI_DEBUG_MSG_ERROR_1("Unhandled TAG 0x%x", tag);
  return MMGSDI_NO_FILE_ENUM;
} /* mmgsdi_onchip_get_ef_enum_from_onchip_tag */


/* ============================================================================
   FUNCTION:      MMGSDI_ONCHIP_CLEAR_USIM_AUTHENTICATE

   DESCRIPTION:
     This function clears the authenticate responses for onchip

   DEPENDENCIES:
     None

   LIMITATIONS:
     This function is limited to the use of UICC

   RETURN VALUE:
     None

   SIDE EFFECTS:

============================================================================*/
static void mmgsdi_onchip_clear_usim_authenticate(
   mmgsdi_slot_id_enum_type              slot_id
)
{
  mmgsdi_data_type      *auth_rsp_ptr  = NULL;
  mmgsdi_slot_data_type *slot_data_ptr = NULL;

  slot_data_ptr = mmgsdi_util_get_slot_data_ptr(slot_id);
  if(slot_data_ptr == NULL)
  {
    return;
  }

  /* Clear data for authenticate algorithm */
  auth_rsp_ptr = &slot_data_ptr->onchip_sim_data.usim_auth_3g_rsp;
  auth_rsp_ptr->data_len = 0;
  MMGSDIUTIL_TMC_MEM_FREE( auth_rsp_ptr->data_ptr );

  auth_rsp_ptr = &slot_data_ptr->onchip_sim_data.usim_auth_gsm_rsp;
  auth_rsp_ptr->data_len = 0;
  MMGSDIUTIL_TMC_MEM_FREE( auth_rsp_ptr->data_ptr );

  auth_rsp_ptr = &slot_data_ptr->onchip_sim_data.usim_auth_auts_rsp;
  auth_rsp_ptr->data_len = 0;
  MMGSDIUTIL_TMC_MEM_FREE( auth_rsp_ptr->data_ptr );

  auth_rsp_ptr = &slot_data_ptr->onchip_sim_data.usim_auth_algo_root_key;
  auth_rsp_ptr->data_len = 0;
  MMGSDIUTIL_TMC_MEM_FREE( auth_rsp_ptr->data_ptr );

  auth_rsp_ptr = &slot_data_ptr->onchip_sim_data.usim_auth_milenage_op_data;
  auth_rsp_ptr->data_len = 0;
  MMGSDIUTIL_TMC_MEM_FREE( auth_rsp_ptr->data_ptr );

  slot_data_ptr->onchip_sim_data.usim_auth_error_rsp[GSDI_SW1_OFFSET] =
    (uint8)SW1_NO_STATUS;
  slot_data_ptr->onchip_sim_data.usim_auth_error_rsp[GSDI_SW2_OFFSET] =
    (uint8)SW2_NORMAL_END;

  slot_data_ptr->onchip_sim_data.usim_auth_algo =
    MMGSDI_ONCHIP_AUTH_ALGORITHM_FIXED;
  slot_data_ptr->onchip_sim_data.usim_auth_algo_res_length = 0;
  slot_data_ptr->onchip_sim_data.usim_auth_milenage_seq = 0;
} /* mmgsdi_onchip_clear_usim_authenticate */


/* ============================================================================
  FUNCTION:      MMGSDI_ONCHIP_POPULATE_CACHE

  DESCRIPTION:
    This function is called to populate the USIM/SIM Cache with the
    configuration data provided in the ACTIVATE_ONCHIP_USIM/SIM Command.

  DEPENDENCIES:
    Can only be called after the mmgsdi_util_validate_onchip_config()

  LIMITATIONS:

  RETURN VALUE:
    mmgsdi_return_enum_type

    MMGSDI_SUCCESS:          The command processing was successful.
    MMGSDI_ERROR:            The command processing was not successful.
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                             within appropriate ranges.

  SIDE EFFECTS:

============================================================================*/
static mmgsdi_return_enum_type mmgsdi_onchip_populate_cache(
  mmgsdi_slot_id_enum_type                   slot_id,
  mmgsdi_session_id_type                     session_id,
  mmgsdi_onchip_mode_enum_type               onchip_mode
)
{
  uint16                               index                  = 0;
  uint8                                len                    = 0;
  uint8                                tag                    = MMGSDI_ACT_ONCHIP_TLV_START_TAG;
  uint8                                tag_size               = 1;
  uint8                                len_size               = 1;
  uint8                               *tlv_data_ptr           = NULL;
  mmgsdi_file_enum_type                file_name              = MMGSDI_NO_FILE_ENUM;
  mmgsdi_data_type                    *auth_rsp_ptr           = NULL;
  uint32                               auth_tag_present       = 0;
  mmgsdi_return_enum_type              mmgsdi_status          = MMGSDI_SUCCESS;
  mmgsdi_slot_data_type               *slot_data_ptr          = NULL;
  mmgsdi_client_id_type                client_id              = MMGSDI_CLIENT_ID_ZERO;
  mmgsdi_session_type_enum_type        session_type           = MMGSDI_MAX_SESSION_TYPE_ENUM;
  mmgsdi_access_type                   file_access;
  mmgsdi_aid_type                      aid;
  uint8                                file_cnt_index         = 0;
  mmgsdi_onchip_sim_config_data_type   onchip_config_data     = {0, NULL};

  memset(&aid, 0x00, sizeof(mmgsdi_aid_type));
  memset(&file_access, 0x00, sizeof(mmgsdi_access_type));
  file_access.access_method  = MMGSDI_EF_ENUM_ACCESS;

  /* --------------------------------------------------------------------------
     Validate the input parameters
     ------------------------------------------------------------------------*/
  slot_data_ptr = mmgsdi_util_get_slot_data_ptr(slot_id);
  if(slot_data_ptr == NULL)
  {
    return MMGSDI_ERROR;
  }

  onchip_config_data = slot_data_ptr->onchip_sim_data.onchip_config_data;
  MMGSDIUTIL_RETURN_IF_NULL(onchip_config_data.data_ptr);

  mmgsdi_status = mmgsdi_util_get_session_type(session_id,
                                               &session_type,
                                               &aid);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  mmgsdi_status = mmgsdi_util_get_client_id(session_id, &client_id);
  if(MMGSDI_SUCCESS != mmgsdi_status)
  {
    return mmgsdi_status;
  }

  for(tag = MMGSDI_ACT_ONCHIP_AUTH_KEY_TAG; tag < MMGSDI_ACT_ONCHIP_MAX_ENUM_TAG; tag++)
  {
    len          = slot_data_ptr->onchip_sim_data.onchip_tlv[tag].len;
    tlv_data_ptr = slot_data_ptr->onchip_sim_data.onchip_tlv[tag].data_ptr;

    if(tlv_data_ptr == NULL)
    {
      continue;
    }

    switch(tag)
    {
      case MMGSDI_ACT_ONCHIP_AUTH_KEY_TAG:
        if (len > 0 && len <= MMGSDI_ONCHIP_SIM_AUTH_KEY_LEN)
        {
          mmgsdi_memscpy(slot_data_ptr->onchip_sim_data.auth_key,
            sizeof(slot_data_ptr->onchip_sim_data.auth_key),
            tlv_data_ptr,
            (uint32)len);
        }
        else if (len > MMGSDI_ONCHIP_SIM_AUTH_KEY_LEN)
        {
          MMGSDI_DEBUG_MSG_ERROR_2("INVALID AUTH KEY LEN 0x%x > 0x%x",
                                   len, MMGSDI_ONCHIP_USIM_AUTH_KEY_LEN);
          return MMGSDI_ERROR;
        }
        continue;

      case MMGSDI_ACT_ONCHIP_USIM_AUTH_3G_RSP_TAG:
      case MMGSDI_ACT_ONCHIP_USIM_AUTH_GSM_RSP_TAG:
      case MMGSDI_ACT_ONCHIP_USIM_AUTH_AUTS_RSP_TAG:
      case MMGSDI_ACT_ONCHIP_USIM_AUTH_ROOT_KEY_TAG:
      case MMGSDI_ACT_ONCHIP_USIM_MILENAGE_OP_TAG:
        if (tag == (uint8)MMGSDI_ACT_ONCHIP_USIM_AUTH_3G_RSP_TAG)
        {
          auth_rsp_ptr = &slot_data_ptr->onchip_sim_data.usim_auth_3g_rsp;
          MMGSDI_SET_AUTH_TAG_PRESENT(auth_tag_present, (int)MMGSDI_ACT_ONCHIP_USIM_AUTH_3G_RSP_TAG);
        }
        else if (tag == (uint8)MMGSDI_ACT_ONCHIP_USIM_AUTH_GSM_RSP_TAG)
        {
          auth_rsp_ptr = &slot_data_ptr->onchip_sim_data.usim_auth_gsm_rsp;
          MMGSDI_SET_AUTH_TAG_PRESENT(auth_tag_present, (int)MMGSDI_ACT_ONCHIP_USIM_AUTH_GSM_RSP_TAG);
        }
        else if (tag == (uint8)MMGSDI_ACT_ONCHIP_USIM_AUTH_AUTS_RSP_TAG)
        {
          auth_rsp_ptr = &slot_data_ptr->onchip_sim_data.usim_auth_auts_rsp;
          MMGSDI_SET_AUTH_TAG_PRESENT(auth_tag_present, (int)MMGSDI_ACT_ONCHIP_USIM_AUTH_AUTS_RSP_TAG);
        }
        else if (tag == (uint8)MMGSDI_ACT_ONCHIP_USIM_AUTH_ROOT_KEY_TAG)
        {
          auth_rsp_ptr = &slot_data_ptr->onchip_sim_data.usim_auth_algo_root_key;
          MMGSDI_SET_AUTH_TAG_PRESENT(auth_tag_present, (int)MMGSDI_ACT_ONCHIP_USIM_AUTH_ROOT_KEY_TAG);
        }
        else if (tag == (uint8)MMGSDI_ACT_ONCHIP_USIM_MILENAGE_OP_TAG)
        {
          auth_rsp_ptr = &slot_data_ptr->onchip_sim_data.usim_auth_milenage_op_data;
          MMGSDI_SET_AUTH_TAG_PRESENT(auth_tag_present, (int)MMGSDI_ACT_ONCHIP_USIM_MILENAGE_OP_TAG);
        }

        /* Zero old value */
        auth_rsp_ptr->data_len = 0;
        MMGSDIUTIL_TMC_MEM_FREE( auth_rsp_ptr->data_ptr );

        if ( len > 0 )
        {
          /* Not validating the payload because invalid responses might
             be used for adverse scenario test cases */

          /* Allocate buffer */
          MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(auth_rsp_ptr->data_ptr,
                                             (uint32)len);

          if (auth_rsp_ptr->data_ptr == NULL)
          {
            mmgsdi_status = MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
            break;
          }

          /* Copy response in buffer */
          auth_rsp_ptr->data_len = len;
          mmgsdi_memscpy(auth_rsp_ptr->data_ptr,
                         len,
                         tlv_data_ptr,
                         len);
        }
        continue;

      case MMGSDI_ACT_ONCHIP_USIM_AUTH_ERR_RSP_TAG:
        if(len == sizeof(slot_data_ptr->onchip_sim_data.usim_auth_error_rsp))
        {
          MMGSDI_SET_AUTH_TAG_PRESENT(auth_tag_present, (int)MMGSDI_ACT_ONCHIP_USIM_AUTH_ERR_RSP_TAG);

          mmgsdi_memscpy(slot_data_ptr->onchip_sim_data.usim_auth_error_rsp,
            sizeof(slot_data_ptr->onchip_sim_data.usim_auth_error_rsp),
            tlv_data_ptr,
            (uint32)len);
        }
        else
        {
          MMGSDI_DEBUG_MSG_ERROR_1("INVALID AUTH ERR LEN 0x%x", len);
          return MMGSDI_ERROR;
        }
        continue;

      case MMGSDI_ACT_ONCHIP_USIM_AUTH_ALGO_TAG:
        if ( len == sizeof(uint8) )
        {
          uint8 algorithm = 0;
          MMGSDI_SET_AUTH_TAG_PRESENT(auth_tag_present, (int)MMGSDI_ACT_ONCHIP_USIM_AUTH_ALGO_TAG);

          algorithm = *tlv_data_ptr;

          if (algorithm == (uint8)MMGSDI_ONCHIP_AUTH_ALGORITHM_FIXED ||
              algorithm == (uint8)MMGSDI_ONCHIP_AUTH_ALGORITHM_XOR   ||
              algorithm == (uint8)MMGSDI_ONCHIP_AUTH_ALGORITHM_MILENAGE)
          {
            slot_data_ptr->onchip_sim_data.usim_auth_algo =
              (mmgsdi_onchip_auth_algo_enum_type)algorithm;
          }
          else
          {
            MMGSDI_DEBUG_MSG_ERROR_1("INVALID ALGORITHM 0x%x", algorithm);
            return MMGSDI_ERROR;
          }
        }
        else
        {
          MMGSDI_DEBUG_MSG_ERROR_1("INVALID ALGO LEN 0x%x", len);
          return MMGSDI_ERROR;
        }

        continue;

      case MMGSDI_ACT_ONCHIP_USIM_AUTH_RES_LEN_TAG:
        if ( len == sizeof(uint8) )
        {
          MMGSDI_SET_AUTH_TAG_PRESENT(auth_tag_present, (int)MMGSDI_ACT_ONCHIP_USIM_AUTH_RES_LEN_TAG);
          slot_data_ptr->onchip_sim_data.usim_auth_algo_res_length = *tlv_data_ptr;
        }
        else
        {
          MMGSDI_DEBUG_MSG_ERROR_1("INVALID RES LEN 0x%x", len);
          return MMGSDI_ERROR;
        }

        continue;

      case MMGSDI_ACT_ONCHIP_USIM_MILENAGE_SEQ_TAG:
        if ( len == sizeof(uint64) )
        {
          uint64 milenage_seq = 0;
          uint8  length       = 0;

          MMGSDI_SET_AUTH_TAG_PRESENT(auth_tag_present, (int)MMGSDI_ACT_ONCHIP_USIM_MILENAGE_SEQ_TAG);

          /* Milenage sequence is a 6 byte long number. It is passed as 8 byte
             data, we need to use only 6 lower bytes. */
          length = len - 2;
          tlv_data_ptr += 2;

          while(length)
          {
            milenage_seq = milenage_seq * 0x100 + (uint64)*tlv_data_ptr;
            tlv_data_ptr++;
            length--;
          }
          slot_data_ptr->onchip_sim_data.usim_auth_milenage_seq = milenage_seq;
        }
        else
        {
          MMGSDI_DEBUG_MSG_ERROR_1("INVALID SEQ LEN 0x%x", len);
          return MMGSDI_ERROR;
        }

        index += (tag_size + len + len_size);

        continue;

      case MMGSDI_ACT_ONCHIP_ICCID_TAG:
        /*since we have already populated ICCID data in cache as part of
          mmgsdi_onchip_validate_and populate_slot_cache, we can skip it here*/
        continue;

      default:
        file_name = mmgsdi_onchip_get_ef_enum_from_onchip_tag(tag, onchip_mode);
        if (file_name == MMGSDI_NO_FILE_ENUM)
        {
          mmgsdi_status = MMGSDI_ERROR;
        }
        break;
    }

    if ( mmgsdi_status != MMGSDI_SUCCESS )
    {
      break;
    }

    if ( len > 0 )
    {
      if (file_name == MMGSDI_NO_FILE_ENUM)
      {
        return MMGSDI_INCORRECT_PARAMS;
      }

      mmgsdi_status = mmgsdi_cache_write_item(file_name,
                                              session_type,
                                              len,
                                              0,
                                              tlv_data_ptr,
                                              MMGSDI_DATA_FROM_CARD_COMPLETE,
                                              0);

      if ( mmgsdi_status != MMGSDI_SUCCESS )
      {
        MMGSDI_DEBUG_MSG_ERROR_1("Failed to Write Cache Data: 0x%x", file_name);
        break;
      }
    }
  }

  /* Now that we have cached the UST/SST (assuming it is present in the onchip
     configuration) and other EFs, if for any of those EFs that are known to
     have a corresponding service bit in UST/SST per the spec, that service bit
     is disabled, delete that EF entry from the MMGSDI EFs cache. Idea is that
     we should allow EF reads only if the corresponding UST/SST service bit is
     available */
  for(file_cnt_index = 0;
      file_cnt_index < sizeof(mmgsdi_onchip_tag_to_ef_table) / sizeof(mmgsdi_onchip_tag_to_ef_table[0]);
      file_cnt_index++)
  {
    file_access.file.file_enum = (onchip_mode == MMGSDI_ONCHIP_MODE_USIM) ?
      mmgsdi_onchip_tag_to_ef_table[file_cnt_index].usim_ef : mmgsdi_onchip_tag_to_ef_table[file_cnt_index].gsm_ef;

    if(file_access.file.file_enum != MMGSDI_NO_FILE_ENUM &&
       MMGSDI_SUCCESS != mmgsdi_util_is_file_ok_in_svc_table(session_id,
                                                             aid.app_type,
                                                             &file_access))
    {
      (void) mmgsdi_cache_delete_item(session_id, 1, &file_access.file.file_enum, 0, NULL);
    }
  }

  if (onchip_mode == MMGSDI_ONCHIP_MODE_SIM)
  {
    /* Nothing further to be done if the onchip mode is SIM */
    return mmgsdi_status;
  }

  /* Check on the onchip usim authenticate tags
      - If error is present, no other tag should be present
      - If AUTS is present, no other tag should be present
      - It is possible to have 3G context and GSM context at same time */
  if (
      (MMGSDI_IS_AUTH_TAG_PRESENT(auth_tag_present, (int)MMGSDI_ACT_ONCHIP_USIM_AUTH_ERR_RSP_TAG) &&
       (MMGSDI_IS_AUTH_TAG_PRESENT(auth_tag_present, (int)MMGSDI_ACT_ONCHIP_USIM_AUTH_3G_RSP_TAG) ||
        MMGSDI_IS_AUTH_TAG_PRESENT(auth_tag_present, (int)MMGSDI_ACT_ONCHIP_USIM_AUTH_GSM_RSP_TAG) ||
        MMGSDI_IS_AUTH_TAG_PRESENT(auth_tag_present, (int)MMGSDI_ACT_ONCHIP_USIM_AUTH_AUTS_RSP_TAG)))
      ||
      (MMGSDI_IS_AUTH_TAG_PRESENT(auth_tag_present, (int)MMGSDI_ACT_ONCHIP_USIM_AUTH_AUTS_RSP_TAG) &&
       (MMGSDI_IS_AUTH_TAG_PRESENT(auth_tag_present, (int)MMGSDI_ACT_ONCHIP_USIM_AUTH_3G_RSP_TAG) ||
        MMGSDI_IS_AUTH_TAG_PRESENT(auth_tag_present, (int)MMGSDI_ACT_ONCHIP_USIM_AUTH_GSM_RSP_TAG)))
     )
  {
    MMGSDI_DEBUG_MSG_ERROR_0("Too many onchip authenticate tags");
    mmgsdi_status = MMGSDI_ERROR;
  }
  /* Check on the onchip usim authenticate tags for milenage and xor
     - Root key must be present */
  if (
      (slot_data_ptr->onchip_sim_data.usim_auth_algo == MMGSDI_ONCHIP_AUTH_ALGORITHM_XOR ||
       slot_data_ptr->onchip_sim_data.usim_auth_algo == MMGSDI_ONCHIP_AUTH_ALGORITHM_MILENAGE)
      &&
      (!MMGSDI_IS_AUTH_TAG_PRESENT(auth_tag_present, (int)MMGSDI_ACT_ONCHIP_USIM_AUTH_ROOT_KEY_TAG))
     )
  {
    MMGSDI_DEBUG_MSG_ERROR_0("Root key is missing");
    mmgsdi_status = MMGSDI_ERROR;
  }

  /* Default values for USIM authenticate
     They are used only if:
      - error and AUTS are not present in the TLV
      - values for GSM context or 3G context were not present in the TLV */
  if (mmgsdi_status == MMGSDI_SUCCESS &&
      slot_data_ptr->onchip_sim_data.usim_auth_algo == MMGSDI_ONCHIP_AUTH_ALGORITHM_FIXED &&
      !MMGSDI_IS_AUTH_TAG_PRESENT(auth_tag_present, (int)MMGSDI_ACT_ONCHIP_USIM_AUTH_ERR_RSP_TAG) &&
      !MMGSDI_IS_AUTH_TAG_PRESENT(auth_tag_present, (int)MMGSDI_ACT_ONCHIP_USIM_AUTH_AUTS_RSP_TAG))
  {
    if (!MMGSDI_IS_AUTH_TAG_PRESENT(auth_tag_present, (int)MMGSDI_ACT_ONCHIP_USIM_AUTH_GSM_RSP_TAG))
    {
      auth_rsp_ptr = &slot_data_ptr->onchip_sim_data.usim_auth_gsm_rsp;

      MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(auth_rsp_ptr->data_ptr,
        sizeof(default_gsm_auth_rsp_ptr));
      if (auth_rsp_ptr->data_ptr != NULL)
      {
        /* Copy response in buffer */
        auth_rsp_ptr->data_len = sizeof(default_gsm_auth_rsp_ptr);
        mmgsdi_memscpy(auth_rsp_ptr->data_ptr,
          sizeof(default_gsm_auth_rsp_ptr),
          default_gsm_auth_rsp_ptr,
          sizeof(default_gsm_auth_rsp_ptr));
      }
      else
      {
        mmgsdi_status = MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
      }
    }
    if (!MMGSDI_IS_AUTH_TAG_PRESENT(auth_tag_present, (int)MMGSDI_ACT_ONCHIP_USIM_AUTH_3G_RSP_TAG))
    {
      auth_rsp_ptr = &slot_data_ptr->onchip_sim_data.usim_auth_3g_rsp;

      MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(auth_rsp_ptr->data_ptr,
                                         sizeof(default_3g_auth_rsp_ptr));
      if (auth_rsp_ptr->data_ptr != NULL)
      {
        /* Copy response in buffer */
        auth_rsp_ptr->data_len = sizeof(default_3g_auth_rsp_ptr);
        mmgsdi_memscpy(auth_rsp_ptr->data_ptr,
          sizeof(default_3g_auth_rsp_ptr),
          default_3g_auth_rsp_ptr,
          sizeof(default_3g_auth_rsp_ptr));
      }
      else
      {
        mmgsdi_status = MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
      }
    }
  }

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /* Clear data for authenticate algorithm */
    mmgsdi_onchip_clear_usim_authenticate(slot_id);
  }

  return mmgsdi_status;
} /* mmgsdi_onchip_populate_cache */


/* ============================================================================
  FUNCTION:      MMGSDI_ONCHIP_POPULATE_SESSION_INFO

   DESCRIPTION:
     This function will populate the session information into the
     client registration table while activating provsioning session.

   DEPENDENCIES:
    Can only be called after the mmgsdi_util_validate_onchip_config()

   LIMITATIONS:

   RETURN VALUE:
    mmgsdi_return_enum_type

    MMGSDI_SUCCESS:          The command processing was successful.
    MMGSDI_ERROR:            The command processing was not successful.
    MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED: Fail to allocate memory.

   SIDE EFFECTS:
     None.

   CRITICAL SECTIONS:
     The function uses mmgsdi_client_app_data_crit_sect_ptr to protect accesses
     to session info ptr and channel info ptr.
============================================================================*/
static mmgsdi_return_enum_type mmgsdi_onchip_populate_session_info(
  mmgsdi_slot_id_enum_type             slot_id,
  mmgsdi_session_id_type               session_id,
  mmgsdi_int_app_info_type            *app_info_ptr
  )
{
  mmgsdi_return_enum_type    mmgsdi_status      = MMGSDI_ERROR;
  mmgsdi_session_info_type  *session_info_ptr   = NULL;
  uint8                      channel_info_index = MMGSDI_MAX_CHANNEL_INFO;
  uint8                      i                  = 0;
  uint8                      j                  = 0;

  /* Get a free index from channel info table to avoid using a channel that is
     being used. */
  MMGSDIUTIL_ENTER_CLIENT_APP_DATA_CRIT_SECT;
  do
  {
    mmgsdi_status = mmgsdi_util_get_free_channel_index(&channel_info_index);
    if (mmgsdi_status != MMGSDI_SUCCESS || channel_info_index >= MMGSDI_MAX_CHANNEL_INFO)
    {
      UIM_MSG_ERR_0("Could not find free channel index");
      break;
    }

    mmgsdi_status = mmgsdi_util_get_session_and_channel_info(
                      session_id,
                      &session_info_ptr,
                      NULL);
    if((mmgsdi_status != MMGSDI_SUCCESS) ||  (session_info_ptr == NULL))
    {
      UIM_MSG_ERR_0("Could not get GW Session info");
      break;
    }

    if (NULL == mmgsdi_channel_info_ptr_table[channel_info_index])
    {
      MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(
        mmgsdi_channel_info_ptr_table[channel_info_index],
        sizeof(mmgsdi_channel_info_type));
      if (NULL == mmgsdi_channel_info_ptr_table[channel_info_index])
      {
        mmgsdi_status = MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
        break;
      }
    }

    session_info_ptr->channel_info_index = (uint8)channel_info_index;
    memset(mmgsdi_channel_info_ptr_table[channel_info_index],
           0x00,
           sizeof(mmgsdi_channel_info_type));
    mmgsdi_channel_info_ptr_table[channel_info_index]->num_connections++;
    mmgsdi_channel_info_ptr_table[channel_info_index]->channel_id   = MMGSDI_DEFAULT_CHANNEL;
    mmgsdi_channel_info_ptr_table[channel_info_index]->slot_id      = slot_id;
    mmgsdi_channel_info_ptr_table[channel_info_index]->app_info_ptr = app_info_ptr;

    for(i = 0; i < MMGSDI_MAX_CLIENT_INFO; i++)
    {
      mmgsdi_len_type            num_sessions       = 0;

      if (mmgsdi_client_id_reg_table[i] == NULL)
      {
        continue;
      }

      num_sessions = mmgsdi_client_id_reg_table[i]->num_session;
      for(j = 0; (num_sessions > 0) && (j < MMGSDI_MAX_SESSION_INFO); j++)
      {
        uint8 curr_channel_info_index = 0;

        if (mmgsdi_client_id_reg_table[i]->session_info_ptr[j] == NULL)
        {
          continue;
        }

        num_sessions--;
        curr_channel_info_index = mmgsdi_client_id_reg_table[i]->session_info_ptr[j]->channel_info_index;
        if(curr_channel_info_index != channel_info_index)
        {
          if(/* Update channel info for sessions of provisioning type */
             (mmgsdi_client_id_reg_table[i]->session_info_ptr[j]->session_type ==
              session_info_ptr->session_type)
                                               ||
             /* Update channel info for sessions of card slot type */
             (
              (curr_channel_info_index < MMGSDI_MAX_CHANNEL_INFO)    &&
              mmgsdi_channel_info_ptr_table[curr_channel_info_index] &&
              (mmgsdi_channel_info_ptr_table[curr_channel_info_index]->channel_id ==
                MMGSDI_DEFAULT_CHANNEL) &&
              (
               /* Update channel info for sessions of card slot1 type */
               ((mmgsdi_channel_info_ptr_table[channel_info_index]->slot_id == MMGSDI_SLOT_1) &&
                (mmgsdi_client_id_reg_table[i]->session_info_ptr[j]->session_type ==
                 MMGSDI_CARD_SESSION_SLOT_1))
                                               ||
               /* Update channel info for sessions of card slot2 type */
               ((mmgsdi_channel_info_ptr_table[channel_info_index]->slot_id == MMGSDI_SLOT_2) &&
                (mmgsdi_client_id_reg_table[i]->session_info_ptr[j]->session_type ==
                 MMGSDI_CARD_SESSION_SLOT_2))
                                               ||
               /* Update channel info for sessions of card slot3 type */
               ((mmgsdi_channel_info_ptr_table[channel_info_index]->slot_id == MMGSDI_SLOT_3) &&
                (mmgsdi_client_id_reg_table[i]->session_info_ptr[j]->session_type ==
                 MMGSDI_CARD_SESSION_SLOT_3))
               )
              )
             )
          {
            UIM_MSG_HIGH_1("Channel mapping updated for session_id=0x%x",
                           mmgsdi_client_id_reg_table[i]->session_info_ptr[j]->session_id);
            if((curr_channel_info_index < MMGSDI_MAX_CHANNEL_INFO) &&
               (mmgsdi_channel_info_ptr_table[curr_channel_info_index] != NULL) &&
               (mmgsdi_channel_info_ptr_table[curr_channel_info_index]->num_connections > 0))
            {
              mmgsdi_channel_info_ptr_table[curr_channel_info_index]->num_connections--;
            }
            mmgsdi_channel_info_ptr_table[channel_info_index]->num_connections++;
            mmgsdi_client_id_reg_table[i]->session_info_ptr[j]->channel_info_index =
              (uint8)channel_info_index;
          }
        }
      } /* for j<MAX_SESSION_INFO */
    } /* for i<MAX_CLIENT_INFO */
  }while(0);

  MMGSDIUTIL_LEAVE_CLIENT_APP_DATA_CRIT_SECT;

  return mmgsdi_status;
}/* mmgsdi_onchip_populate_session_info */


/* ============================================================================
  FUNCTION:      MMGSDI_ONCHIP_ACTIVATE_SESSION_OP

  DESCRIPTION:
    This function activates session for onchip and brings subscription to ready

  DEPENDENCIES:

  LIMITATIONS:

  RETURN VALUE:
    mmgsdi_return_enum_type

    MMGSDI_SUCCESS:          The command processing was successful.
    MMGSDI_ERROR:            The command processing was not successful.
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                             within appropriate ranges.
    MMGSDI_NOT_SUPPORTED:    When the file has an entry in the Service Table
                             but the service table indicates the card does not
                             have the support

  SIDE EFFECTS:
    None.

  CRITICAL SECTIONS:
============================================================================*/
static mmgsdi_return_enum_type mmgsdi_onchip_activate_session_op(
  mmgsdi_slot_id_enum_type             slot_id,
  mmgsdi_session_id_type               session_id,
  mmgsdi_int_app_info_type            *app_info_ptr,
  mmgsdi_onchip_mode_enum_type         onchip_mode
  )
{
  mmgsdi_return_enum_type          mmgsdi_status    = MMGSDI_ERROR;
  mmgsdi_event_data_type          *evt_info_ptr     = NULL;
  mmgsdi_evt_session_notify_type   notify_type;
  mmgsdi_slot_data_type           *slot_data_ptr    = NULL;
  uint8                            app_sel_index    = 0;
  mmgsdi_session_type_enum_type    session_type     = MMGSDI_MAX_SESSION_TYPE_ENUM;

  MMGSDIUTIL_RETURN_IF_NULL(app_info_ptr);

  memset(&notify_type, 0x00, sizeof(notify_type));

  do
  {
    /* Update the channel information for session */
    mmgsdi_status = mmgsdi_onchip_populate_session_info(slot_id,
                                                        session_id,
                                                        app_info_ptr);
    if (mmgsdi_status != MMGSDI_SUCCESS)
    {
      break;
    }

    slot_data_ptr = mmgsdi_util_get_slot_data_ptr(slot_id);
    if (NULL == slot_data_ptr)
    {
      break;
    }

    mmgsdi_status = mmgsdi_util_get_session_type(session_id, &session_type, NULL);
    if (mmgsdi_status != MMGSDI_SUCCESS)
    {
      break;
    }

    /* Set corresponding app select index to the first location
    This needs to be populated before we can access session related cache*/

    app_sel_index = mmgsdi_util_get_prov_index_from_session(session_type);

    if(app_sel_index >= MMGSDI_MAX_PROV_APPS)
    {
      return MMGSDI_ERROR;
    }

    /* Set the provisiong app index of the corresponding session to first location*/
    slot_data_ptr->prov_app_indices[app_sel_index] = MMGSDI_PROV_INDEX_VALIDITY_MASK;

    /* Now populate the onchip cache if this the first time activation on the slot */
    if (slot_data_ptr->onchip_sim_data.onchip_config_data.data_ptr != NULL)
    {
      mmgsdi_status = mmgsdi_onchip_populate_cache(slot_id,
                                                          session_id,
                                                          onchip_mode);
      if (mmgsdi_status != MMGSDI_SUCCESS)
      {
        /* Reset corresponding app select index if activation fails */
        slot_data_ptr->prov_app_indices[app_sel_index] = 0;
        break;
      }
      else
      {
        MMGSDIUTIL_TMC_MEM_FREE( slot_data_ptr->onchip_sim_data.onchip_config_data.data_ptr);
        slot_data_ptr->onchip_sim_data.onchip_config_data.data_len = 0;
        memset(&(slot_data_ptr->onchip_sim_data.onchip_tlv),
               0,
               MMGSDI_ACT_ONCHIP_MAX_ENUM_TAG * sizeof(mmgsdi_onchip_config_tlv_type));
      }
    }

    /* Send SESSION_CHANGE_EVT */
    UIM_MSG_HIGH_0("MMGSDI_SESSION_CHANGED_EVT");

    /* Assign session notify type info */
    notify_type.notify_type = MMGSDI_EVT_NOTIFY_ALL_SESSIONS;
    notify_type.slot_id     = slot_id;
    mmgsdi_evt_build_and_send_session_changed(notify_type,
                                              TRUE,
                                              (const mmgsdi_int_app_info_type*)app_info_ptr);

    /* Sleep for 100 ms while the SESSION CHANGED propogates */
    mmgsdi_util_timed_sleep(100);

#ifdef FEATURE_LTE

    /* Initialize AKA algorithm if required */
    if (onchip_mode == MMGSDI_ONCHIP_MODE_USIM &&
        slot_data_ptr->onchip_sim_data.usim_auth_algo == MMGSDI_ONCHIP_AUTH_ALGORITHM_MILENAGE)
    {
      lte_init_milenage_aka(
        slot_data_ptr->onchip_sim_data.usim_auth_algo_root_key.data_ptr,
        int32touint8(slot_data_ptr->onchip_sim_data.usim_auth_algo_root_key.data_len),
        slot_data_ptr->onchip_sim_data.usim_auth_milenage_op_data.data_ptr,
        int32touint8(slot_data_ptr->onchip_sim_data.usim_auth_milenage_op_data.data_len),
        slot_data_ptr->onchip_sim_data.usim_auth_milenage_seq);
    }
#endif /* FEATURE_LTE */

    /* Allocate evt_info_ptr */
    MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(evt_info_ptr, sizeof(mmgsdi_event_data_type));
    if (NULL == evt_info_ptr)
    {
      mmgsdi_status = MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
      break;
    }

    UIM_MSG_HIGH_0("MMGSDI_PIN1_DISABLED_EVT");
    (void)mmgsdi_evt_build_pin(session_id,
                               MMGSDI_PIN1,
                               MMGSDI_PIN_DISABLED,
                               evt_info_ptr);

    mmgsdi_evt_notify_session(evt_info_ptr,
                              slot_id,
                              (const mmgsdi_int_app_info_type*)app_info_ptr,
                              TRUE);

    /* Sleep for another 100 ms while the PIN1 Disabled propogates */
    mmgsdi_util_timed_sleep(100);

    UIM_MSG_HIGH_0("MMGSDI_PIN2_DISABLED_EVT");
    (void)mmgsdi_evt_build_pin(session_id,
                               MMGSDI_PIN2,
                               MMGSDI_PIN_DISABLED,
                               evt_info_ptr);

    mmgsdi_evt_notify_session(evt_info_ptr,
                              slot_id,
                              (const mmgsdi_int_app_info_type*)app_info_ptr,
                              TRUE);

    /* Sleep for another 100 ms while the PIN2 Disabled propogates */
    mmgsdi_util_timed_sleep(100);
    app_info_ptr->perso_state = MMGSDI_SIMLOCK_VERIFIED;
    app_info_ptr->pre_perso_files_cached = TRUE;

#ifdef FEATURE_SIMLOCK
    mmgsdi_simlock_build_and_run_algo(TRUE, session_id);

    /* Sleep for another 100 ms while SIMLOCK module updates valid IMSI list */
    mmgsdi_util_timed_sleep(100);

    UIM_MSG_HIGH_0("MMGSDI_PERSO_EVT");

    /* Assign session notify type info */
    notify_type.notify_type = MMGSDI_EVT_NOTIFY_ALL_SESSIONS;
    notify_type.slot_id     = slot_id;
    app_info_ptr->perso_status = MMGSDI_PERSO_STATUS_DONE;

    mmgsdi_evt_build_and_send_perso(notify_type,
      (const mmgsdi_int_app_info_type*)app_info_ptr,
      app_info_ptr->perso_status,
      MMGSDI_MAX_PERSO_FEATURE_ENUM,
      0,
      0);
#endif /* FEATURE_SIMLOCK */

    /* Update SIM/USIM capabilities once perso event is sent as MMGSDI
       would have updated SIM/USIM capabilities after PERSO step if normal
       initilization would have taken place. */
    switch (app_info_ptr->app_data.app_type)
    {
      case MMGSDI_APP_SIM:
        mmgsdi_status = mmgsdi_icc_get_sim_capabilities(session_id,
                                                        slot_id);
        break;

      case MMGSDI_APP_USIM:
        mmgsdi_status = mmgsdi_uicc_get_capabilities(session_id,
                                                     MMGSDI_APP_USIM);
        break;

      default:
        mmgsdi_status = MMGSDI_ERROR;
        break;
    }

    if(mmgsdi_status != MMGSDI_SUCCESS)
    {
      UIM_MSG_ERR_1("get_capabilites failed for app = 0x%x",app_info_ptr->app_data.app_type);
      break;
    }

    UIM_MSG_HIGH_0("MMGSDI_SUBSCRIPTION_READY_EVT");
    mmgsdi_evt_build_and_send_subscription_ready(notify_type,
                  (const mmgsdi_int_app_info_type*)app_info_ptr);
    app_info_ptr->app_state = MMGSDI_APP_STATE_READY;

    /* Sleep for another 100 ms while the SUBS_READY propogates */
    mmgsdi_util_timed_sleep(100);
  }
  while (0);

  /* free evt_info_ptr */
  if (evt_info_ptr)
  {
    mmgsdi_evt_free_data(evt_info_ptr);
    MMGSDIUTIL_TMC_MEM_FREE(evt_info_ptr);
  }

  return mmgsdi_status;
}/* mmgsdi_onchip_activate_session_op */


/*===========================================================================
FUNCTION MMMGSDI_ONCHIP_VALIDATE_CONFIG

DESCRIPTION
  This function is used to ensure all components of the TLV DATA are
  properly set.  Along with validation this function also populates
  tlv data length and pointers for all available tags.


DEPENDENCIES
  None

RETURN VALUE
  mmgsdi_return_enum_type:  MMGSDI_SUCCESS:  Properly Built TLV
                            MMGSDI_ERROR:    Improperly Built TLV

SIDE EFFECTS
  None
===========================================================================*/
static mmgsdi_return_enum_type mmgsdi_onchip_validate_config(
  mmgsdi_onchip_sim_data_type *onchip_data_ptr,
  mmgsdi_onchip_mode_enum_type onchip_sim_mode
)
{
  uint16                                index                  = 0;
  uint16                                len                    = 0;
  uint16                                tlv_len                = 0;
  uint16                                offset                 = 0;
  uint8                                 TAG_SIZE               = 1;
  uint8                                 LEN_SIZE               = 1;
  uint8                                *tlv_data_ptr           = NULL;
  mmgsdi_activate_onchip_tlv_enum_type  tag                    = MMGSDI_ACT_ONCHIP_MAX_ENUM_TAG;
  mmgsdi_onchip_sim_config_data_type   *onchip_config_data_ptr = NULL;


  /* --------------------------------------------------------------------------
     Validate the input parameters
     ------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL(onchip_data_ptr);

  onchip_config_data_ptr = &(onchip_data_ptr->onchip_config_data);

  if ( onchip_config_data_ptr->data_len == 0 )
  {
    /* A Zero Length is allowed for the purpose of
    ** getting to an idle screen but the call stacks
    ** will be stuck in limited service*/
    return MMGSDI_SUCCESS;
  }
  MMGSDIUTIL_RETURN_IF_NULL(onchip_config_data_ptr->data_ptr);

  if (onchip_config_data_ptr->data_len < MMGSDI_ONCHIP_TLV_PAYLOAD ||
      onchip_config_data_ptr->data_ptr[0] != MMGSDI_ACT_ONCHIP_TLV_START_TAG)
  {
    /* onchip data(TLV) must atleast have a master tag and two bytes for
       length of the value field of onchip data */
    return MMGSDI_ERROR;
  }

  /* Reset Onchip TLV cache info */
  memset(onchip_data_ptr->onchip_tlv,
         0,
         sizeof(onchip_data_ptr->onchip_tlv));

  /* byte 2 and 3 contain the length of the master TLV */
  tlv_len = onchip_config_data_ptr->data_ptr[1] + (onchip_config_data_ptr->data_ptr[2] * 0x100);
  tlv_data_ptr = onchip_config_data_ptr->data_ptr + MMGSDI_ONCHIP_TLV_PAYLOAD;

  /*Populate onchip_tlv data for the ONCHIP_TLV_START tag*/
  onchip_data_ptr->onchip_tlv[MMGSDI_ACT_ONCHIP_TLV_START_TAG].len = tlv_len;
  onchip_data_ptr->onchip_tlv[MMGSDI_ACT_ONCHIP_TLV_START_TAG].data_ptr = tlv_data_ptr;

  index = MMGSDI_ONCHIP_TLV_PAYLOAD;

  while (index < onchip_config_data_ptr->data_len )
  {
    /* Initialize the offset to 0 */
    offset = 0;
    tag = *tlv_data_ptr;
    switch ( tag )
    {
      case MMGSDI_ACT_ONCHIP_CK_TAG:
      case MMGSDI_ACT_ONCHIP_IK_TAG:
      case MMGSDI_ACT_ONCHIP_EPSLOCI_TAG:
      case MMGSDI_ACT_ONCHIP_HPLMNWACT_TAG:
      case MMGSDI_ACT_ONCHIP_PLMNWACT_TAG:
      case MMGSDI_ACT_ONCHIP_UST_TAG:
      case MMGSDI_ACT_ONCHIP_EST_TAG:
      case MMGSDI_ACT_ONCHIP_USIM_AUTH_3G_RSP_TAG:
      case MMGSDI_ACT_ONCHIP_USIM_AUTH_GSM_RSP_TAG:
      case MMGSDI_ACT_ONCHIP_USIM_AUTH_AUTS_RSP_TAG:
      case MMGSDI_ACT_ONCHIP_USIM_AUTH_ERR_RSP_TAG:
      case MMGSDI_ACT_ONCHIP_USIM_AUTH_ALGO_TAG:
      case MMGSDI_ACT_ONCHIP_USIM_AUTH_ROOT_KEY_TAG:
      case MMGSDI_ACT_ONCHIP_USIM_AUTH_RES_LEN_TAG:
      case MMGSDI_ACT_ONCHIP_USIM_MILENAGE_OP_TAG:
      case MMGSDI_ACT_ONCHIP_USIM_MILENAGE_SEQ_TAG:
      case MMGSDI_ACT_ONCHIP_EPSNSC_TAG:
      case MMGSDI_ACT_ONCHIP_KEYSPS_TAG:
      case MMGSDI_ACT_ONCHIP_KEYS_TAG:
      case MMGSDI_ACT_ONCHIP_RPLMNACT_TAG:
        /* These tags are valid only in case of usim */
        if (onchip_sim_mode != MMGSDI_ONCHIP_MODE_USIM)
        {
          MMGSDI_DEBUG_MSG_ERROR_1("Invalid tag for onchip SIM: 0x%x",
                                   *tlv_data_ptr);
          return MMGSDI_ERROR;
        }
        /* continue without breaking because the checks below
           apply to both SIM and USIM tags */

      case MMGSDI_ACT_ONCHIP_AUTH_KEY_TAG:
      case MMGSDI_ACT_ONCHIP_IMSI_TAG:
      case MMGSDI_ACT_ONCHIP_LOCI_TAG:
      case MMGSDI_ACT_ONCHIP_KC_TAG:
      case MMGSDI_ACT_ONCHIP_PLMN_TAG:
      case MMGSDI_ACT_ONCHIP_SST_TAG:
      case MMGSDI_ACT_ONCHIP_ACC_TAG:
      case MMGSDI_ACT_ONCHIP_FPLMN_TAG:
      case MMGSDI_ACT_ONCHIP_PHASE_TAG:
      case MMGSDI_ACT_ONCHIP_AD_TAG:
      case MMGSDI_ACT_ONCHIP_PLMNSEL_TAG:
      case MMGSDI_ACT_ONCHIP_ICCID_TAG:
      case MMGSDI_ACT_ONCHIP_KCGPRS_TAG:
        /* Validate the number of bytes remainging are valid
        ** in the TLV.  -1 for the purpose of converting
        ** entire data length to respective index
        */
        if ( index >= ( onchip_config_data_ptr->data_len - LEN_SIZE ) )
        {
          MMGSDI_DEBUG_MSG_ERROR_0("Invalid TLV Format...No Length");
          return MMGSDI_ERROR;
        }

        /* move the index to the 1 Byte Length past the Tag*/
        tlv_data_ptr++;

        /* Check if a Tag is repeated or if Tag length is invalid */
        if((onchip_data_ptr->onchip_tlv[tag].len != 0) ||
           (*tlv_data_ptr == 0))
        {
          MMGSDI_DEBUG_MSG_ERROR_2("Duplicate tag: 0x%x/Invalid TLV length: 0x%x",
                                   tag, *tlv_data_ptr);
          return MMGSDI_ERROR;
        }

        /*Populate onchip_tlv data for the current tag*/
        onchip_data_ptr->onchip_tlv[tag].len = *tlv_data_ptr;
        onchip_data_ptr->onchip_tlv[tag].data_ptr = tlv_data_ptr+ LEN_SIZE;

        offset = *tlv_data_ptr + LEN_SIZE;

        /* Move the tlv_data_ptr to the next Tag */
        tlv_data_ptr += offset;

        /* Now add the Length to the Offset */
        offset += TAG_SIZE;

        /* Update the Length accordingly as well */
        len += offset;

        index += offset;
        break;

      default:
        MMGSDI_DEBUG_MSG_ERROR_1("Unhandled Tag: 0x%x",
                                 ((*tlv_data_ptr - offset) - TAG_SIZE));
        return MMGSDI_ERROR;
    }
  }

  /* --------------------------------------------------------------------------
     Check to ensure the TLV Len is equal to the SUM of all Tags and Data
     contained within this config tlv
     ------------------------------------------------------------------------*/
  if ((tlv_len != len) ||
      (tlv_len + MMGSDI_ONCHIP_TLV_PAYLOAD != onchip_config_data_ptr->data_len))
  {
    MMGSDI_DEBUG_MSG_ERROR_3("BAD TLV:  TLV LEN=0x%x, LEN=0x%x, TOTAL LEN=0x%x",
                             tlv_len, len, onchip_config_data_ptr->data_len);
    return MMGSDI_ERROR;
  }

  return MMGSDI_SUCCESS;
} /* mmgsdi_util_validate_onchip_config */


/* ============================================================================
  FUNCTION:      MMGSDI_ONCHIP_SEND_REFRESH_EVT_FOR_ONCHIP_ACT

  DESCRIPTION:
    This function is called to send out the REFRESH RESET events to clients
    before the onchip acitvation is started.

  DEPENDENCIES:
    None

  LIMITATIONS:
    Should be used only for sending out the REFRESH RESET or RESET_AUTO events

  RETURN VALUE:
    None

  SIDE EFFECTS:
    None
============================================================================*/
static void mmgsdi_onchip_send_refresh_evt_for_onchip_act(
  mmgsdi_event_data_type   * evt_info_ptr,
  mmgsdi_slot_id_enum_type   mmgsdi_slot
)
{
  mmgsdi_session_info_type       *session_info_ptr = NULL;
  mmgsdi_slot_id_enum_type        slot             = MMGSDI_MAX_SLOT_ID_ENUM;
  uint32                          i                = 0;
  uint32                          j                = 0;
  mmgsdi_len_type                 num_sessions     = 0;
  mmgsdi_return_enum_type         mmgsdi_status    = MMGSDI_ERROR;

  MMGSDIUTIL_CHECK_NON_GSDI_TASK_CLIENT_APP_DATA_CRIT_SECT_ACCESS;

  if(evt_info_ptr == NULL)
  {
    return;
  }

  for(i=0; i<MMGSDI_MAX_CLIENT_INFO; i++)
  {
    if(mmgsdi_client_id_reg_table[i] == NULL)
    {
      continue;
    }

    num_sessions     = mmgsdi_client_id_reg_table[i]->num_session;

    /* Scan through all open sessions and send out the REFRESH event to
       valid sessions on slot 1*/
    for(j=0; (num_sessions > 0) && (j < MMGSDI_MAX_SESSION_INFO); j++)
    {
      session_info_ptr =
        mmgsdi_client_id_reg_table[i]->session_info_ptr[j];

      if(session_info_ptr == NULL)
      {
        /*Invalid Session, Continue to next*/
        continue;
      }
      num_sessions--;

      mmgsdi_status = mmgsdi_util_get_session_app_info(
                        session_info_ptr->session_id,
                        &slot,
                        NULL,
                        NULL,
                        NULL,
                        NULL,
                        NULL);

      if(mmgsdi_status != MMGSDI_SUCCESS || slot != mmgsdi_slot)
      {
        continue;
      }
      /* Fetch the app type from current app */
      if((session_info_ptr->channel_info_index < MMGSDI_MAX_CHANNEL_INFO) &&
         (mmgsdi_channel_info_ptr_table[session_info_ptr->channel_info_index] != NULL) &&
         (mmgsdi_channel_info_ptr_table[session_info_ptr->channel_info_index]->app_info_ptr != NULL))
      {
        evt_info_ptr->data.refresh.aid.app_type =
          mmgsdi_channel_info_ptr_table[session_info_ptr->channel_info_index]->app_info_ptr->app_data.app_type;
        evt_info_ptr->data.refresh.app_info.app_data.app_type =
          mmgsdi_channel_info_ptr_table[session_info_ptr->channel_info_index]->app_info_ptr->app_data.app_type;
      }
      mmgsdi_evt_notify_specific_session_id(session_info_ptr->session_id,
                                            evt_info_ptr);
    }
  }
} /* mmgsdi_onchip_send_refresh_evt_for_onchip_act */


/* ============================================================================
  FUNCTION:      MMGSDI_ONCHIP_ACTIVATE

  DESCRIPTION:
    This function will be used to activate the ONCHIP USIM or SIM.  It will
    configure the USIM/SIM Cache.

  DEPENDENCIES:


  LIMITATIONS:
    USIM Authentication in USIM mode is not supported.

  RETURN VALUE:
    mmgsdi_return_enum_type

    MMGSDI_SUCCESS:          The command processing was successful.
    MMGSDI_ERROR:            The command processing was not successful.
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                             within appropriate ranges.

  SIDE EFFECTS:
    None.

  CRITICAL SECTIONS:
    The function uses mmgsdi_client_app_data_crit_sect_ptr to protect accesses
    to slot data ptr, channel info ptr, pin info ptr and app info ptr.
============================================================================*/
mmgsdi_return_enum_type mmgsdi_onchip_activate(
  mmgsdi_activate_onchip_sim_req_type  *onchip_sim_config_ptr,
  mmgsdi_onchip_mode_enum_type          onchip_mode
)
{
  mmgsdi_return_enum_type             mmgsdi_status          = MMGSDI_SUCCESS;
  mmgsdi_event_data_type             *evt_info_ptr           = NULL;
  mmgsdi_int_app_info_type           *app_info_ptr           = NULL;
  mmgsdi_int_app_info_type          **app_info_pptr          = NULL;
  mmgsdi_app_pin_info_type           *pin1_ptr               = NULL;
  mmgsdi_slot_data_type              *slot_data_ptr          = NULL;
  uint8                               channel_info_index     = MMGSDI_MAX_CHANNEL_INFO;
  mmgsdi_task_enum_type               mmgsdi_task            = MMGSDI_TASK_MAX;
  mmgsdi_slot_id_enum_type            slot_id                = MMGSDI_SLOT_1;
  uint8                               slot_index             = 0;
  mmgsdi_session_id_type              session_id             = 0;
  uint8                               app_index              = 0;
  mmgsdi_session_type_enum_type       session_type           = MMGSDI_GW_PROV_PRI_SESSION;
  mmgsdi_channel_info_type           *channel_info_ptr       = NULL;
  nv_stat_enum_type                   nv_status              = NV_FAIL_S;
  mmgsdi_nv_item_cache_type           select_app;
  mmgsdi_card_pdown_req_type          pdown_req;
  /* Default AID for onchip USIM */
  const uint8     default_onchip_usim_aid[]   = { MMGSDI_3GPP_APP_ID_RID_BYTE_1,
                                                  MMGSDI_3GPP_APP_ID_RID_BYTE_2,
                                                  MMGSDI_3GPP_APP_ID_RID_BYTE_3,
                                                  MMGSDI_3GPP_APP_ID_RID_BYTE_4,
                                                  MMGSDI_3GPP_APP_ID_RID_BYTE_5,
                                                  MMGSDI_USIM_APP_CODE_BYTE_1,
                                                  MMGSDI_USIM_APP_CODE_BYTE_2 };

  memset(&select_app, 0x00, sizeof(mmgsdi_nv_item_cache_type));
  memset(&pdown_req, 0x00, sizeof(mmgsdi_card_pdown_req_type));

  mmgsdi_task = mmgsdi_util_get_current_task();
  if(mmgsdi_task >= MMGSDI_NUM_TASKS)
  {
    /* In case the current task is undetermined,
       default to main task */
    mmgsdi_task = MMGSDI_TASK_MAIN;
  }

  /* --------------------------------------------------------------------------
     Validate the input Parameters
     ------------------------------------------------------------------------*/
  MMGSDIUTIL_RETURN_IF_NULL(onchip_sim_config_ptr);
  if (onchip_sim_config_ptr->onchip_sim.data_len > 0)
  {
    MMGSDIUTIL_RETURN_IF_NULL(onchip_sim_config_ptr->onchip_sim.data_ptr);
  }
  MMGSDIUTIL_RETURN_IF_EXCEEDS(onchip_sim_config_ptr->onchip_sim.data_len,
                               MMGSDI_ACTIVATE_ONCHIP_SIM_CONFIG_MAX_SIZE);

  /* --------------------------------------------------------------------------
     Check whether onchip is already active on the slot
     ------------------------------------------------------------------------*/

  slot_id = onchip_sim_config_ptr->request_header.slot_id;
  if (mmgsdi_util_get_slot_index(slot_id, &slot_index) != MMGSDI_SUCCESS)
  {
    return mmgsdi_common_rsp_activate_onchip(MMGSDI_ERROR, onchip_sim_config_ptr);
  }

  slot_data_ptr = mmgsdi_data_slot_ptr[slot_index];

  if(slot_data_ptr->onchip_sim_data.state == MMGSDI_ONCHIP_SIM_INIT)
  {
    UIM_MSG_HIGH_1("ONCHIP is already active on slot %x", slot_id);
    return mmgsdi_common_rsp_activate_onchip(MMGSDI_SUCCESS,onchip_sim_config_ptr);
  }


  /* --------------------------------------------------------------------------
     Copy the config data
     ------------------------------------------------------------------------*/

  if ( onchip_sim_config_ptr->onchip_sim.data_len > 0 )
  {
    slot_data_ptr->onchip_sim_data.onchip_config_data.data_len =
                            onchip_sim_config_ptr->onchip_sim.data_len;

    MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(slot_data_ptr->onchip_sim_data.onchip_config_data.data_ptr,
                                       onchip_sim_config_ptr->onchip_sim.data_len);
    if(slot_data_ptr->onchip_sim_data.onchip_config_data.data_ptr == NULL)
    {
      return mmgsdi_common_rsp_activate_onchip(MMGSDI_ERROR, onchip_sim_config_ptr);
    }

    mmgsdi_memscpy(slot_data_ptr->onchip_sim_data.onchip_config_data.data_ptr,
                   slot_data_ptr->onchip_sim_data.onchip_config_data.data_len,
                   onchip_sim_config_ptr->onchip_sim.data_ptr,
                   onchip_sim_config_ptr->onchip_sim.data_len);
  }

  do
  {
    /* --------------------------------------------------------------------------
       Validate the TLV Data
       ------------------------------------------------------------------------*/
    mmgsdi_status = mmgsdi_onchip_validate_config(
                         &(slot_data_ptr->onchip_sim_data),
                         onchip_mode);
    if ( mmgsdi_status != MMGSDI_SUCCESS )
    {
      MMGSDI_DEBUG_MSG_ERROR_0("Validate ONCHIP SIM TLV Failure");
      /* No external client...directly generate a response */
      break;
    }

    /* Set app select index and session id depending on slot. For simplicity,
       primary session is mapped to slot 1, secondary to slot 2 and so on. */
    if (slot_id == MMGSDI_SLOT_1)
    {
      session_id    = mmgsdi_generic_data_ptr->pri_gw_session_id;
      session_type  = MMGSDI_GW_PROV_PRI_SESSION;
    }
    else if (slot_id == MMGSDI_SLOT_2)
    {
      session_id    = mmgsdi_generic_data_ptr->sec_gw_session_id;
      session_type  = MMGSDI_GW_PROV_SEC_SESSION;
    }
    else
    {
      session_id    = mmgsdi_generic_data_ptr->ter_gw_session_id;
      session_type  = MMGSDI_GW_PROV_TER_SESSION;
    }

    /* Stop the onchip activation in the case of Auto provisioning if the session id
       being activated is already active on the other slot.,i.e.,crossmapping scenario */

    nv_status = mmgsdi_nv_get_item(NV_UIM_SELECT_DEFAULT_USIM_APP_I,
                                   &select_app,
                                   MMGSDI_NV_CONTEXT_DEVICE_SPECIFIC);
    UIM_MSG_HIGH_2("NV_UIM_SELECT_DEFAULT_USIM_APP_I Read: Status: 0x%x, Select Default App.: 0x%x",
                   nv_status, select_app.item_value.uim_select_default_usim_app);

    mmgsdi_status = mmgsdi_util_get_session_and_channel_info(
                      session_id,
                      NULL,
                      &channel_info_ptr);

    if (mmgsdi_status == MMGSDI_SUCCESS &&
        channel_info_ptr != NULL &&
        channel_info_ptr->slot_id != slot_id &&
        (nv_status != NV_DONE_S ||
         select_app.item_value.uim_select_default_usim_app))
    {
      UIM_MSG_ERR_2("Session type 0x%x active on slot 0x%x; cannot proceed with OnChip",
                    session_type, channel_info_ptr->slot_id);
      mmgsdi_status = MMGSDI_ERROR;
      break;
    }

    /* --------------------------------------------------------------------------
       Allocate Event info pointer
       ------------------------------------------------------------------------*/

    MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(evt_info_ptr,
                                       sizeof(mmgsdi_event_data_type));
    if (evt_info_ptr == NULL)
    {
      /* Directly generate a response */
      mmgsdi_status = MMGSDI_ERROR;
      break;
    }

    /* --------------------------------------------------------------------------
     Use the REFRESH SIM RESET Command to trigger the UI to get into
     the correct state so that a GSDI_SIM_INSERTED can be handled
     ------------------------------------------------------------------------*/
    UIM_MSG_HIGH_0("Sending out MMGSDI_REFRESH_STAGE_IN_PROGRESS");
    evt_info_ptr->evt                    = MMGSDI_REFRESH_EVT;
    evt_info_ptr->data.refresh.slot      = slot_id;
    evt_info_ptr->data.refresh.stage     = MMGSDI_REFRESH_STAGE_START;
    evt_info_ptr->data.refresh.mode      = MMGSDI_REFRESH_RESET;
    evt_info_ptr->data.refresh.orig_mode = MMGSDI_REFRESH_RESET;

    /* Send out the REFRESH start event */
    mmgsdi_onchip_send_refresh_evt_for_onchip_act(evt_info_ptr, slot_id);

    /* Allocating memory  for Slot data*/
    if(mmgsdi_data_slot_ptr[slot_index] == NULL)
    {
      MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(
        mmgsdi_data_slot_ptr[slot_index],
        sizeof(mmgsdi_slot_data_type));

      if(mmgsdi_data_slot_ptr[slot_index] == NULL)
      {
        mmgsdi_status = MMGSDI_ERROR;
        break;
      }
    }

    /* Reset session and application data */
    mmgsdi_util_preinit_mmgsdi(0,
                               MMGSDI_APP_NONE,
                               MMGSDI_REFRESH_RESET,
                               slot_id,
                               FALSE);

    mmgsdi_state_update_card_error_state(slot_id);

    mmgsdi_util_timed_sleep(200);

    UIM_MSG_HIGH_0("Sending out MMGSDI_REFRESH_STAGE_END_SUCCESS");
    evt_info_ptr->data.refresh.stage = MMGSDI_REFRESH_STAGE_END_SUCCESS;

    /* Send out the REFRESH END_SUCCESS event */
    mmgsdi_onchip_send_refresh_evt_for_onchip_act(evt_info_ptr, slot_id);

    /* free evt_info_ptr */
    mmgsdi_evt_free_data(evt_info_ptr);
    MMGSDIUTIL_TMC_MEM_FREE(evt_info_ptr);

    /* Allocating memory for application*/
    MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(
      mmgsdi_app_pin_table_ptr[slot_index][app_index],
      sizeof(mmgsdi_int_app_info_type));
    if(mmgsdi_app_pin_table_ptr[slot_index][app_index] == NULL)
    {
      mmgsdi_status = MMGSDI_ERROR;
      break;
    }

    /* Allocating memory  for PIN1 info*/
    MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(
      mmgsdi_app_pin1_table_ptr[slot_index][app_index],
      sizeof(mmgsdi_app_info_type));
    if(mmgsdi_app_pin1_table_ptr[slot_index][app_index] == NULL)
    {
      mmgsdi_status = MMGSDI_ERROR;
      break;
    }

    app_info_ptr  = mmgsdi_app_pin_table_ptr[slot_index][app_index];
    app_info_pptr = mmgsdi_app_pin_table_ptr[slot_index];
    pin1_ptr      = mmgsdi_app_pin1_table_ptr[slot_index][app_index];

    slot_data_ptr->onchip_sim_data.state         = MMGSDI_ONCHIP_SIM_NOT_VALID;
    slot_data_ptr->mmgsdi_uim_sanity_timer_value = MMGSDI_UIM_SANITY_ONCHIP_TIMER_VALUE;
    if (onchip_mode == MMGSDI_ONCHIP_MODE_USIM)
    {
      slot_data_ptr->protocol = MMGSDI_UICC;
    }
    else
    {
      slot_data_ptr->protocol = MMGSDI_ICC;
    }

    /* --------------------------------------------------------------------------
       Now Set the MMGSDI Global Data Variables
       ------------------------------------------------------------------------*/
    MMGSDIUTIL_ENTER_CLIENT_APP_DATA_CRIT_SECT;

    /* Assign the first index location for the app info global table */
    app_info_ptr->app_selected = TRUE;

    /* Assign the first pin1 from the pin1 table to the app */
    app_info_ptr->pin1_ptr = pin1_ptr;

    /* populate pin1 info */
    pin1_ptr->key_ref                   = MMGSDI_CHV1_KEY;
    pin1_ptr->pin_id                    = MMGSDI_PIN1;
    pin1_ptr->status                    = MMGSDI_PIN_DISABLED;
    pin1_ptr->pin_replacement           = MMGSDI_PIN_NOT_REPLACED_BY_UNIVERSAL;
    pin1_ptr->num_retries               = 3;
    pin1_ptr->num_unblock_retries       = 10;
    pin1_ptr->cached                    = TRUE;
    pin1_ptr->valid_num_retries         = TRUE;
    pin1_ptr->valid_num_unblock_retries = TRUE;

    /* populate pin2 info
       According to 31.102 clause 6.4: "Disabling of PIN2 is allowed.
       This is, however, not the case if PIN2 is mapped to the CHV2 of
       a GSM application.".In most commercial cases, the PIN2 is enabled,
       so set the status of PIN2 to "enabled and not verified" to better
       emulate real cards in the field. */
    app_info_ptr->pin2.key_ref                   = MMGSDI_CHV2_KEY;
    app_info_ptr->pin2.pin_id                    = MMGSDI_PIN2;
    app_info_ptr->pin2.status                    = MMGSDI_PIN_ENABLED_NOT_VERIFIED;
    app_info_ptr->pin2.pin_replacement           = MMGSDI_PIN_NOT_REPLACED_BY_UNIVERSAL;
    app_info_ptr->pin2.num_retries               = 3;
    app_info_ptr->pin2.num_unblock_retries       = 10;
    app_info_ptr->pin2.cached                    = TRUE;
    app_info_ptr->pin2.valid_num_retries         = TRUE;
    app_info_ptr->pin2.valid_num_unblock_retries = TRUE;

    if (onchip_mode == MMGSDI_ONCHIP_MODE_USIM)
    {
      app_info_ptr->app_data.app_type     = MMGSDI_APP_USIM;

      /* Set length to max len 16, it should not be 0 for USIM app */
      app_info_ptr->app_data.aid.data_len = MMGSDI_MAX_APP_ID_LEN;

      memset(app_info_ptr->app_data.aid.data_ptr,
             0x00,
             sizeof(app_info_ptr->app_data.aid.data_ptr)/sizeof(app_info_ptr->app_data.aid.data_ptr[0]));

      /* Set first seven byte of AID data to 3GPP USIM App identifier */
      mmgsdi_memscpy(app_info_ptr->app_data.aid.data_ptr,
                     sizeof(app_info_ptr->app_data.aid.data_ptr),
                     default_onchip_usim_aid,
                     sizeof(default_onchip_usim_aid));
    }
    else
    {
      app_info_ptr->app_data.app_type     = MMGSDI_APP_SIM;
      app_info_ptr->app_data.aid.data_len = 0;
    }
    app_info_ptr->app_state        = MMGSDI_APP_STATE_DETECTED;

    /* assign the global data to the slot1 global data */
    slot_data_ptr->app_info_pptr   = app_info_pptr;
    slot_data_ptr->mmgsdi_tp_state = MMGSDI_TP_STATE_DONE;

    mmgsdi_generic_data_ptr->me_capabilities.me_supports_fdn = TRUE;
    mmgsdi_generic_data_ptr->me_capabilities.me_supports_bdn = TRUE;
    mmgsdi_generic_data_ptr->me_capabilities.me_supports_acl = TRUE;

    /* Get a free index from channel info table to avoid using a channel that is
       being used. */
    mmgsdi_status = mmgsdi_util_get_free_channel_index(&channel_info_index);

    MMGSDIUTIL_LEAVE_CLIENT_APP_DATA_CRIT_SECT;

    if (mmgsdi_status      != MMGSDI_SUCCESS ||
        channel_info_index >= MMGSDI_MAX_CHANNEL_INFO)
    {
      UIM_MSG_ERR_0("Could not find free channel index");
      break;
    }

    /* --------------------------------------------------------------------------
       Now validate the IMSI value and populate the ICCID cache
       ------------------------------------------------------------------------*/
    mmgsdi_status = mmgsdi_onchip_validate_and_populate_slot_cache(&(slot_data_ptr->onchip_sim_data),
                                                                     slot_id);

    if (mmgsdi_status != MMGSDI_SUCCESS)
    {
      UIM_MSG_ERR_0("IMSI validation or ICCID population failed");
      break;
    }
  }while(0);

  if(mmgsdi_status != MMGSDI_SUCCESS)
  {
    MMGSDIUTIL_TMC_MEM_FREE( slot_data_ptr->onchip_sim_data.onchip_config_data.data_ptr);
    memset(&(slot_data_ptr->onchip_sim_data.onchip_tlv),
           0,
           MMGSDI_ACT_ONCHIP_MAX_ENUM_TAG * sizeof(mmgsdi_onchip_config_tlv_type));
    return mmgsdi_common_rsp_activate_onchip(mmgsdi_status, onchip_sim_config_ptr);
  }

  /* --------------------------------------------------------------------------
     Send Power down card comand to drivers to avoid processing card notifications,
     polling and proactive commands in onchip mode.
     ------------------------------------------------------------------------*/
  pdown_req.request_header.client_id         = mmgsdi_generic_data_ptr->client_id;
  pdown_req.request_header.orig_request_type = MMGSDI_CARD_PDOWN_REQ;
  pdown_req.request_header.request_type      = MMGSDI_CARD_PDOWN_REQ;
  pdown_req.request_header.slot_id           = slot_id;
  pdown_req.request_header.response_cb       = mmgsdi_util_internal_cb;
  pdown_req.option                           = MMGSDI_CARD_POWER_DOWN_ONLY;

  /* even if Power down is unsuccessful continue the onchip activation */
  (void)mmgsdi_gen_process_card_pdown(&pdown_req);

  if (onchip_mode == MMGSDI_ONCHIP_MODE_USIM)
  {
    /* Clear data for authenticate algorithm */
    mmgsdi_onchip_clear_usim_authenticate(slot_id);
  }

  /* Set the appropriate flag to indicate an activate ONCHIP SIM */
  slot_data_ptr->onchip_sim_data.state = MMGSDI_ONCHIP_SIM_INIT;

  /* Populate app cache table pointer along with CPHS cache pointer */
  (void)mmgsdi_cache_create_app_cache_table(app_info_ptr);

  /* --------------------------------------------------------------------------
     Now Notify the tasks that we have a SIM Inserted.
     ------------------------------------------------------------------------*/
  UIM_MSG_HIGH_0("MMGSDI_CARD_INSERTED_EVT");
  mmgsdi_evt_build_and_send_card_inserted(TRUE, 0, slot_id);

  /* Activate session if Auto Provisioning is needed */
  if ((nv_status != NV_DONE_S) ||
      (select_app.item_value.uim_select_default_usim_app == TRUE))
  {
    /* Sleep for another 100 ms while the SIM INSERTED propogates */
    mmgsdi_util_timed_sleep(100);

    /* Update channel info for session, Generate PIN Event,
       Perso Event, Subscription Ready Event */
    mmgsdi_status = mmgsdi_onchip_activate_session_op(
                                              slot_id,
                                              session_id,
                                              app_info_ptr,
                                              onchip_mode);
  }

  /* Directly generate a response */
  return mmgsdi_common_rsp_activate_onchip(mmgsdi_status,
                                           onchip_sim_config_ptr);
} /* mmgsdi_onchip_activate */


/* ============================================================================
  FUNCTION:      MMGSDI_ONCHIP_ACTIVATE_SESSION

  DESCRIPTION:
    This function activates session for onchip and brings subscription to ready

  DEPENDENCIES:

  LIMITATIONS:

  RETURN VALUE:
    mmgsdi_return_enum_type

    MMGSDI_SUCCESS:          The command processing was successful.
    MMGSDI_ERROR:            The command processing was not successful.

  SIDE EFFECTS:
    None.

  CRITICAL SECTIONS:
============================================================================*/
mmgsdi_return_enum_type mmgsdi_onchip_activate_session(
  mmgsdi_session_act_or_switch_prov_req_type *msg_ptr
)
{
  mmgsdi_return_enum_type       mmgsdi_status = MMGSDI_ERROR;
  mmgsdi_int_app_info_type     *app_info_ptr  = NULL;
  mmgsdi_sw_status_type         sw_status     = {0,0};
  mmgsdi_onchip_mode_enum_type  onchip_mode   = MMGSDI_ONCHIP_MODE_MAX_ENUM;
  uint8                         slot_index    = 0;

  MMGSDIUTIL_RETURN_IF_NULL(msg_ptr);

  if (mmgsdi_util_get_slot_index(msg_ptr->request_header.slot_id, &slot_index)
        != MMGSDI_SUCCESS)
  {
    return MMGSDI_ERROR;
  }

  /* In onchip case, the only app available per slot is present at first index */
  app_info_ptr  = mmgsdi_app_pin_table_ptr[slot_index][0];

  if(app_info_ptr == NULL ||
     app_info_ptr->app_state != MMGSDI_APP_STATE_DETECTED )
  {
    UIM_MSG_ERR_0("Can not activate onchip session on the app that already has a active session");
    return MMGSDI_ERROR;
  }

  if (MMGSDI_APP_SIM == app_info_ptr->app_data.app_type)
  {
    onchip_mode = MMGSDI_ONCHIP_MODE_SIM;
  }
  else
  {
    onchip_mode = MMGSDI_ONCHIP_MODE_USIM;
  }

  /* Update channel info for session, Populate cache,
     Generate PIN Event, Perso Event, Subscription Ready Event */
  mmgsdi_status = mmgsdi_onchip_activate_session_op(
                      msg_ptr->request_header.slot_id,
                      msg_ptr->request_header.session_id,
                      app_info_ptr,
                      onchip_mode);

  /* Send confirmation */
  mmgsdi_status = mmgsdi_cnf_build_and_queue(mmgsdi_status,
                                             &msg_ptr->request_header,
                                             NULL,
                                             NULL,
                                             FALSE,
                                             sw_status);

  return mmgsdi_status;
}/* mmgsdi_onchip_activate_session */


/* ============================================================================
  FUNCTION:      MMGSDI_ONCHIP_DEACTIVATE_SESSION

   DESCRIPTION:
     This function will only update the channel info table info in regard to the
     session that has been deactivated in the case of onchip.
     This function does not delete app related cache.

   DEPENDENCIES:

   LIMITATIONS:

   RETURN VALUE:
    mmgsdi_return_enum_type

    MMGSDI_SUCCESS:          The command processing was successful.
    MMGSDI_ERROR:            The command processing was not successful.

   SIDE EFFECTS:
     None.

   CRITICAL SECTIONS:
     The function uses mmgsdi_client_app_data_crit_sect_ptr to protect accesses
     to session info ptr and channel info ptr.
============================================================================*/
mmgsdi_return_enum_type mmgsdi_onchip_deactivate_session(
  mmgsdi_slot_id_enum_type             slot_id,
  mmgsdi_int_app_info_type            *app_info_ptr
)
{
  uint8                           client_index     = 0;
  uint8                           session_index    = 0;
  mmgsdi_session_info_type       *curr_session_ptr = NULL;
  mmgsdi_evt_session_notify_type  event_notify_info;

  MMGSDIUTIL_RETURN_IF_NULL(app_info_ptr);

  memset(&event_notify_info, 0x00, sizeof(mmgsdi_evt_session_notify_type));

  MMGSDIUTIL_ENTER_CLIENT_APP_DATA_CRIT_SECT;

  for (client_index = 0; client_index < MMGSDI_MAX_CLIENT_INFO; client_index++)
  {
    mmgsdi_len_type                 num_sessions     = 0;

    if (mmgsdi_client_id_reg_table[client_index] == NULL)
    {
      continue;
    }

    num_sessions = mmgsdi_client_id_reg_table[client_index]->num_session;
    for (session_index = 0; (num_sessions > 0) && (session_index < MMGSDI_MAX_SESSION_INFO); session_index++)
    {
      curr_session_ptr =
        mmgsdi_client_id_reg_table[client_index]->session_info_ptr[session_index];

      if(curr_session_ptr == NULL)
      {
        continue;
      }

      num_sessions--;
      if ((curr_session_ptr->channel_info_index < MMGSDI_MAX_CHANNEL_INFO) &&
          (mmgsdi_channel_info_ptr_table[curr_session_ptr->channel_info_index] != NULL) &&
          (mmgsdi_channel_info_ptr_table[curr_session_ptr->channel_info_index]->app_info_ptr != NULL))
      {
        if (slot_id != mmgsdi_channel_info_ptr_table[curr_session_ptr->channel_info_index]->slot_id)
        {
          continue;
        }

        if (mmgsdi_util_cmp_aid(
              &mmgsdi_channel_info_ptr_table[curr_session_ptr->channel_info_index]->app_info_ptr->app_data,
              &app_info_ptr->app_data, FALSE) == MMGSDI_SUCCESS)
        {
          if(mmgsdi_channel_info_ptr_table[curr_session_ptr->channel_info_index]->app_info_ptr != NULL)
          {
            if(curr_session_ptr->session_evt_cb_ptr != NULL)
            {
              event_notify_info.session_id = curr_session_ptr->session_id;
              event_notify_info.slot_id =
                mmgsdi_channel_info_ptr_table[curr_session_ptr->channel_info_index]->slot_id;
              event_notify_info.notify_type = MMGSDI_EVT_NOTIFY_SINGLE_SESSION;
              mmgsdi_evt_build_and_send_session_changed(
                event_notify_info,
                FALSE,
                mmgsdi_channel_info_ptr_table[curr_session_ptr->channel_info_index]->app_info_ptr);
            }

            if(mmgsdi_channel_info_ptr_table[curr_session_ptr->channel_info_index]->num_connections > 1)
            {
              mmgsdi_channel_info_ptr_table[curr_session_ptr->channel_info_index]->num_connections--;
            }
            else if(mmgsdi_channel_info_ptr_table[curr_session_ptr->channel_info_index]->num_connections == 1 )
            {
              /* channel data should be reset though it is freed below, so that
                 mmgsdi_session_release_app_info is processed correctly */
              mmgsdi_channel_info_ptr_table[curr_session_ptr->channel_info_index]->num_connections
                = 0;
              mmgsdi_channel_info_ptr_table[curr_session_ptr->channel_info_index]->channel_state
                = MMGSDI_CHANNEL_STATE_NONE;
              mmgsdi_channel_info_ptr_table[curr_session_ptr->channel_info_index]->select_rsp_type
                = MMGSDI_SELECT_RSP_NO_DATA;
                mmgsdi_channel_info_ptr_table[curr_session_ptr->channel_info_index]->app_info_ptr->app_state =
                MMGSDI_APP_STATE_DETECTED;
              mmgsdi_channel_info_ptr_table[curr_session_ptr->channel_info_index]->app_info_ptr->perso_state =
                  MMGSDI_SIMLOCK_NOT_VERIFIED;
              mmgsdi_channel_info_ptr_table[curr_session_ptr->channel_info_index]->app_info_ptr->gprs_anite_mcc = FALSE;
              mmgsdi_channel_info_ptr_table[curr_session_ptr->channel_info_index]->app_info_ptr->hzi_version = MMGSDI_UHZI_NONE;
              mmgsdi_channel_info_ptr_table[curr_session_ptr->channel_info_index]->app_info_ptr->app_selected = FALSE;
              mmgsdi_channel_info_ptr_table[curr_session_ptr->channel_info_index]->app_info_ptr->status_app_is_initialized = FALSE;
              mmgsdi_channel_info_ptr_table[curr_session_ptr->channel_info_index]->app_info_ptr->pre_perso_files_cached = FALSE;

              /*break channel to app mapping for USIM or SIM app*/
              mmgsdi_channel_info_ptr_table[curr_session_ptr->channel_info_index]->app_info_ptr = NULL;
              MMGSDIUTIL_TMC_MEM_FREE(mmgsdi_channel_info_ptr_table[curr_session_ptr->channel_info_index]);
            }

            curr_session_ptr->channel_info_index = MMGSDI_MAX_CHANNEL_INFO;
          }
        }
      }
    }
  }

  MMGSDIUTIL_LEAVE_CLIENT_APP_DATA_CRIT_SECT;

  return MMGSDI_SUCCESS;
}/*mmgsdi_onchip_deactivate_session*/


/*===========================================================================
FUNCTION:      MMGSDI_ONCHIP_CREATE_ONCHIP_REQUEST

DESCRIPTION
  This function will build a MMGSDI_ACTIVATE_ONCHIP_SIM_REQ and queue it to
  the MMGSDI Task for further processing.

PARAMS:
  client_id:         Client Id of the caller
  card_slot:         Physical slot for multiple slot targets. Only slot-1
                     is supported
  onchip_sim_config: TLV Data Len and TLV Data.
  onchip_mode:       ICC/UICC mode to be used for onchip behavior
  response_cb_ptr:   Response callback.
  client_ref:        User Data returned upon completion of this cmd.

DEPENDENCIES:
  The client must have a valid client ID.  If the client ID is not part
  of the client_id_table, and failure will be returned in the response.
  Achived by calling mmgsdi_client_id_reg().

  Note: If client fails to provide a client id, meaning the client_id
        passed is 0, then an internal client id will be used.

LIMITATIONS:
  None.

RETURN VALUE:
  mmgsdi_return_enum_type:

    MMGSDI_SUCCESS:          The command structure was properly generated
                             and queued onto the MMGSDI Command Queue.
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                             within appropriate ranges.
    MMGSDI_CMD_QUEUE_FULL:   The command was not Queued to the MMGSDI Task
                             because the max number of commands are already
                             queued.

SIDE EFFECTS:
  Access to a real card will not be possible without a power cycle
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_onchip_create_onchip_request (
  mmgsdi_client_id_type              client_id,
  mmgsdi_slot_id_enum_type           card_slot,
  mmgsdi_onchip_sim_config_data_type onchip_sim_config,
  mmgsdi_onchip_mode_enum_type       onchip_mode,
  mmgsdi_callback_type               response_cb_ptr,
  mmgsdi_client_data_type            client_ref
)
{
  mmgsdi_task_cmd_type                 * task_cmd_ptr  = NULL;
  mmgsdi_activate_onchip_sim_req_type  * msg_ptr       = NULL;
  mmgsdi_return_enum_type                mmgsdi_status = MMGSDI_ERROR;
  uint32                                 total_mem_len = 0;
  uint32                                 temp_mem      = 0;

  /* --------------------------------------------------------------------------
     Validate the input parameters
     ------------------------------------------------------------------------*/
  mmgsdi_status = mmgsdi_util_is_client_id_valid_in_cmd(client_id);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    client_id = mmgsdi_get_global_client_id();
  }

  if (mmgsdi_util_is_slot_valid(card_slot) == FALSE)
  {
    return MMGSDI_INCORRECT_PARAMS;
  }

  if ( onchip_sim_config.data_len > MMGSDI_ACTIVATE_ONCHIP_SIM_CONFIG_MAX_SIZE )
  {
    MMGSDI_DEBUG_MSG_ERROR_1("CONFIG DATA LEN INVALID 0x%x",
                             onchip_sim_config.data_len);
    return MMGSDI_ERROR;
  }
  MMGSDIUTIL_RETURN_IF_NULL(response_cb_ptr);

  if ( onchip_sim_config.data_len > 0 )
  {
    /* Check is only valid if the Len != 0 */
    MMGSDIUTIL_RETURN_IF_NULL(onchip_sim_config.data_ptr);
  }
  /* --------------------------------------------------------------------------
     Allocate 1 large block of memory for the task_cmd_ptr and for the
     onchip config data.
     NOTE:  Side effect of the following total len is that it may not
            match exactly as a result of mmgsdi_task_cmd_data_type being
            a union. Will be addressed by using the correct type size
            rather than sizeof mmgsdi_task_cmd_data_type
     ------------------------------------------------------------------------*/
  total_mem_len = sizeof(mmgsdi_task_cmd_type);
  mmgsdi_util_cal_align_size(total_mem_len, &temp_mem);
  MMGSDIUTIL_RETURN_IF_ALIGNED_VAR_OUT_OF_BOUNDS(total_mem_len, temp_mem);

  total_mem_len = temp_mem + onchip_sim_config.data_len;

  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(task_cmd_ptr, total_mem_len);
  if (task_cmd_ptr == NULL)
  {
    return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
  }
  /*---------------------------------------------------------------------------
    - Align pointers of the various structures members
  ---------------------------------------------------------------------------*/
  mmgsdi_util_align_mem( &task_cmd_ptr->cmd.cmd.act_onchip_sim_req, MMGSDI_ACTIVATE_ONCHIP_SIM_REQ);

  msg_ptr = &task_cmd_ptr->cmd.cmd.act_onchip_sim_req;

  /* ----------------------------------------------------------------------------
     Populate the command Request Header with the information provided by
     the client.
     ---------------------------------------------------------------------------*/
  task_cmd_ptr->cmd.cmd_enum                = MMGSDI_ACTIVATE_ONCHIP_SIM_REQ;
  msg_ptr->request_header.request_type      = MMGSDI_ACTIVATE_ONCHIP_SIM_REQ;
  msg_ptr->request_header.orig_request_type = MMGSDI_ACTIVATE_ONCHIP_SIM_REQ;
  msg_ptr->request_header.client_id         = client_id;
  msg_ptr->request_header.slot_id           = card_slot;
  msg_ptr->request_header.client_data       = client_ref;
  msg_ptr->request_header.response_cb       = response_cb_ptr;

  msg_ptr->onchip_sim.data_len = onchip_sim_config.data_len;
  msg_ptr->onchip_sim_mode     = onchip_mode;

  if ( onchip_sim_config.data_len > 0 )
  {
    /* --------------------------------------------------------------------------
       Set the Onchip configuration Data to point to the end of the task
       command pointer block allocated.  Then copy the configuration Data
       ------------------------------------------------------------------------*/
    mmgsdi_memscpy(msg_ptr->onchip_sim.data_ptr,
                   onchip_sim_config.data_len,
                   onchip_sim_config.data_ptr,
                   onchip_sim_config.data_len);
  }

  /* --------------------------------------------------------------------------
     Queue the command, free the command, and return the status
     ------------------------------------------------------------------------*/
  mmgsdi_status = mmgsdi_cmd(task_cmd_ptr);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /* Free the pointer since the task_cmd_ptr
    ** has not been put onto the command queue
    */
    MMGSDIUTIL_TMC_MEM_FREE(task_cmd_ptr);
  }

  UIM_MSG_HIGH_2("Queue of MMGSDI command: MMGSDI_ACTIVATE_ONCHIP_SIM_REQ status=0x%x, onchip mode=0x%x",
                 mmgsdi_status, onchip_mode);

  return mmgsdi_status;
} /* mmgsdi_onchip_create_onchip_request */


/* =============================================================================
   FUNCTION:      MMGSDI_CNF_BUILD_ONCHIP_SIM_DATA

   DESCRIPTION:
     This function builds cnf for the ONCHIP SIM ACTIVATION REQUEST

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type

     MMGSDI_SUCCESS:          The command processing was successful.
     MMGSDI_ERROR:            The command processing was not successful.

   SIDE EFFECTS:
     None
=============================================================================*/
mmgsdi_return_enum_type mmgsdi_cnf_build_onchip_sim_data(
  mmgsdi_act_onchip_sim_cnf_type         * act_onchip_sim_cnf,
  mmgsdi_slot_id_enum_type                 slot_id)
{
  mmgsdi_slot_data_type *slot_data_ptr = NULL;

  MMGSDIUTIL_RETURN_IF_NULL(act_onchip_sim_cnf);

  slot_data_ptr = mmgsdi_util_get_slot_data_ptr(slot_id);
  if(slot_data_ptr == NULL)
  {
    return MMGSDI_ERROR;
  }

  act_onchip_sim_cnf->onchip_sim_state.state =
    slot_data_ptr->onchip_sim_data.state;

  return MMGSDI_SUCCESS;
} /* mmgsdi_cnf_build_onchip_sim_data */


/*===========================================================================
  FUNCTION:      MMGSDI_ONCHIP_UICC_AUTH

  DESCRIPTION:
    This function executes the onchip usim authentication

  DEPENDENCIES:
    None

  LIMITATIONS:
    This function is limited to the use of UICC ie. (technologies of type UICC only).

  RETURN VALUE:
    MMGSDI_SUCCESS:          The command processing was successful.
    MMGSDI_ERROR:            The command processing was not successful.
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                             within appropriate ranges.

  SIDE EFFECTS:
     None
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_onchip_uicc_auth (
  const mmgsdi_auth_req_type   *req_ptr
)
{
  uint32                             index           = 0;
  mmgsdi_client_req_extra_info_type *extra_param_ptr = NULL;
  mmgsdi_return_enum_type            mmgsdi_status   = MMGSDI_SUCCESS;
  uim_rpt_type                       uim_report;
  mmgsdi_slot_data_type             *slot_data_ptr   = NULL;

  MMGSDIUTIL_RETURN_IF_NULL(req_ptr);
  memset(&uim_report, 0x00, sizeof(uim_rpt_type));

  /* Check context: in onchip mode, we support only GSM and 3G contexts */
  if (req_ptr->auth_context != MMGSDI_AUTN_USIM_GSM_CONTEXT &&
      req_ptr->auth_context != MMGSDI_AUTN_USIM_3G_CONTEXT)
  {
    UIM_MSG_ERR_1("Invalid context value: 0x%x", req_ptr->auth_context);
    return MMGSDI_ERROR;
  }

  /* Get a free index */
  mmgsdi_status = mmgsdi_util_get_client_request_table_free_index(&index);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  /* Populate the extra param */
  mmgsdi_status = mmgsdi_util_alloc_and_populate_extra_param(
        MMGSDI_USIM_AUTH_REQ, (void*)req_ptr, &extra_param_ptr);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    mmgsdi_util_free_client_request_table_index(index);
    return mmgsdi_status;
  }

  /* Populate the request table */
  mmgsdi_status = mmgsdi_util_populate_client_request_table_info(
                                  index,
                                  &req_ptr->request_header,
                                  extra_param_ptr);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    MMGSDIUTIL_TMC_MEM_FREE(extra_param_ptr);
    mmgsdi_util_free_client_request_table_index(index);
    return mmgsdi_status;
  }

  slot_data_ptr = mmgsdi_util_get_slot_data_ptr(req_ptr->request_header.slot_id);
  if(slot_data_ptr == NULL)
  {
    return MMGSDI_ERROR;
  }

  if(slot_data_ptr->onchip_sim_data.usim_auth_algo == MMGSDI_ONCHIP_AUTH_ALGORITHM_FIXED)
  {
    /* populate the necessary field in the uim_report_type */
    if (slot_data_ptr->onchip_sim_data.usim_auth_error_rsp[GSDI_SW1_OFFSET] !=
        (uint8)SW1_NO_STATUS)
    {
      /* Error response */
      uim_report.sw1 =
        (uim_sw1_type)slot_data_ptr->onchip_sim_data.usim_auth_error_rsp[GSDI_SW1_OFFSET];
      uim_report.sw2 =
        (uim_sw2_type)slot_data_ptr->onchip_sim_data.usim_auth_error_rsp[GSDI_SW2_OFFSET];
      uim_report.rpt_status = UIM_FAIL;
    }
    else if (slot_data_ptr->onchip_sim_data.usim_auth_auts_rsp.data_len > 0 &&
             slot_data_ptr->onchip_sim_data.usim_auth_auts_rsp.data_ptr != NULL)
    {
      /* AUTS response */
      uim_report.sw1        = SW1_NORMAL_END;
      uim_report.sw2        = SW2_NORMAL_END;
      uim_report.rpt_status = UIM_PASS;

      if (slot_data_ptr->onchip_sim_data.usim_auth_auts_rsp.data_len <= UIM_MAX_CHARS)
      {
        mmgsdi_memscpy(uim_report.rpt.autn.data,
          sizeof(uim_report.rpt.autn.data),
          slot_data_ptr->onchip_sim_data.usim_auth_auts_rsp.data_ptr,
          slot_data_ptr->onchip_sim_data.usim_auth_auts_rsp.data_len);
        uim_report.rpt.autn.data_length =
          uint32touint16(slot_data_ptr->onchip_sim_data.usim_auth_auts_rsp.data_len);
      }
      else
      {
        mmgsdi_util_free_client_request_table_index(index);
        return MMGSDI_ERROR;
      }
    }
    else if (slot_data_ptr->onchip_sim_data.usim_auth_3g_rsp.data_len > 0 &&
             slot_data_ptr->onchip_sim_data.usim_auth_3g_rsp.data_ptr != NULL &&
             req_ptr->auth_context == MMGSDI_AUTN_USIM_3G_CONTEXT)
    {
      /* 3G security context */
      uim_report.sw1        = SW1_NORMAL_END;
      uim_report.sw2        = SW2_NORMAL_END;
      uim_report.rpt_status = UIM_PASS;

      if (slot_data_ptr->onchip_sim_data.usim_auth_3g_rsp.data_len <= UIM_MAX_CHARS)
      {
        mmgsdi_memscpy(uim_report.rpt.autn.data,
          sizeof(uim_report.rpt.autn.data),
          slot_data_ptr->onchip_sim_data.usim_auth_3g_rsp.data_ptr,
          slot_data_ptr->onchip_sim_data.usim_auth_3g_rsp.data_len);
        uim_report.rpt.autn.data_length =
          uint32touint16(slot_data_ptr->onchip_sim_data.usim_auth_3g_rsp.data_len);
      }
      else
      {
        mmgsdi_util_free_client_request_table_index(index);
        return MMGSDI_ERROR;
      }
    }
    else if (slot_data_ptr->onchip_sim_data.usim_auth_gsm_rsp.data_len > 0 &&
             slot_data_ptr->onchip_sim_data.usim_auth_gsm_rsp.data_ptr != NULL &&
             req_ptr->auth_context == MMGSDI_AUTN_USIM_GSM_CONTEXT)
    {
      /* GSM security context */
      uim_report.sw1        = SW1_NORMAL_END;
      uim_report.sw2        = SW2_NORMAL_END;
      uim_report.rpt_status = UIM_PASS;

      if (slot_data_ptr->onchip_sim_data.usim_auth_gsm_rsp.data_len <= UIM_MAX_CHARS)
      {
        mmgsdi_memscpy(uim_report.rpt.autn.data,
          sizeof(uim_report.rpt.autn.data),
          slot_data_ptr->onchip_sim_data.usim_auth_gsm_rsp.data_ptr,
          slot_data_ptr->onchip_sim_data.usim_auth_gsm_rsp.data_len);
        uim_report.rpt.autn.data_length =
          uint32touint16(slot_data_ptr->onchip_sim_data.usim_auth_gsm_rsp.data_len);
      }
      else
      {
        mmgsdi_util_free_client_request_table_index(index);
        return MMGSDI_ERROR;
      }
    }
    else
    {
      /* Dafault case */
      mmgsdi_util_free_client_request_table_index(index);
      return MMGSDI_ERROR;
    }
  }
#ifdef FEATURE_LTE
  else if (slot_data_ptr->onchip_sim_data.usim_auth_algo ==
           MMGSDI_ONCHIP_AUTH_ALGORITHM_XOR)
  {
    uim_report.sw1        = SW1_NORMAL_END;
    uim_report.sw2        = SW2_NORMAL_END;
    uim_report.rpt_status = UIM_PASS;

    if (slot_data_ptr->onchip_sim_data.usim_auth_algo_root_key.data_len > 0 &&
        slot_data_ptr->onchip_sim_data.usim_auth_algo_root_key.data_ptr != NULL &&
        slot_data_ptr->onchip_sim_data.usim_auth_algo_res_length > 0 &&
        MMGSDI_RAND_LEN + MMGSDI_AUTN_LEN + 2 >= req_ptr->auth_req.data_len)
    {
      uint8 autn_data_len = 0;

      /* Authenticate request data should be in encoded form where first byte
         denotes RAND data len followed by RAND data. Then one byte for AUTN
         data len followed by AUTN data */
      if(req_ptr->auth_req.data_ptr[MMGSDI_RAND_LEN_POS] != MMGSDI_RAND_LEN)
      {
        mmgsdi_util_free_client_request_table_index(index);
        return MMGSDI_ERROR;
      }

      if(req_ptr->auth_req.data_ptr[MMGSDI_AUTN_LEN_POS] != MMGSDI_AUTN_LEN)
      {
        mmgsdi_util_free_client_request_table_index(index);
        return MMGSDI_ERROR;
      }

      if(LTE_AUTH_ERROR_INCORRECT_MAC ==
         lte_perform_xor_aka(
           slot_data_ptr->onchip_sim_data.usim_auth_algo_root_key.data_ptr,
           slot_data_ptr->onchip_sim_data.usim_auth_algo_root_key.data_len,
           &req_ptr->auth_req.data_ptr[MMGSDI_AUTN_DATA_POS],
           MMGSDI_AUTN_LEN,
           &req_ptr->auth_req.data_ptr[MMGSDI_RAND_DATA_POS],
           MMGSDI_RAND_LEN,
           (uint8*)uim_report.rpt.autn.data,
           &autn_data_len,
           slot_data_ptr->onchip_sim_data.usim_auth_algo_res_length))
      {
        uim_report.sw1 = SW1_SECURITY;
        uim_report.sw2 = SW2_APPLN_SPECIFIC_AUTH_ERR;
      }
      uim_report.rpt.autn.data_length = (word)autn_data_len;
    }
    else
    {
      mmgsdi_util_free_client_request_table_index(index);
      return MMGSDI_ERROR;
    }
  }
  else if (slot_data_ptr->onchip_sim_data.usim_auth_algo ==
           MMGSDI_ONCHIP_AUTH_ALGORITHM_MILENAGE)
  {
    uim_report.sw1        = SW1_NORMAL_END;
    uim_report.sw2        = SW2_NORMAL_END;
    uim_report.rpt_status = UIM_PASS;

    if (slot_data_ptr->onchip_sim_data.usim_auth_algo_res_length > 0 &&
        MMGSDI_RAND_LEN + MMGSDI_AUTN_LEN + 2 >= req_ptr->auth_req.data_len)
    {
      uint8            autn_data_len = 0;
      lte_auth_errno_e auth_error    = LTE_AUTH_3G_UMTS_SUCCESS;

      /* Authenticate request data should be in encoded form where first byte
         denotes RAND data len followed by RAND data. Then one byte for AUTN
         data len followed by AUTN data */
      if(req_ptr->auth_req.data_ptr[MMGSDI_RAND_LEN_POS] != MMGSDI_RAND_LEN)
      {
        mmgsdi_util_free_client_request_table_index(index);
        return MMGSDI_ERROR;
      }

      if(req_ptr->auth_req.data_ptr[MMGSDI_AUTN_LEN_POS] != MMGSDI_AUTN_LEN)
      {
        mmgsdi_util_free_client_request_table_index(index);
        return MMGSDI_ERROR;
      }

      auth_error = lte_perform_milenage_aka(
           &req_ptr->auth_req.data_ptr[MMGSDI_AUTN_DATA_POS],
           MMGSDI_AUTN_LEN,
           &req_ptr->auth_req.data_ptr[MMGSDI_RAND_DATA_POS],
           MMGSDI_RAND_LEN,
           (uint8*)uim_report.rpt.autn.data,
           &autn_data_len,
           slot_data_ptr->onchip_sim_data.usim_auth_algo_res_length);

      if(LTE_AUTH_ERROR_INCORRECT_MAC == auth_error)
      {
        uim_report.sw1 = SW1_SECURITY;
        uim_report.sw2 = SW2_APPLN_SPECIFIC_AUTH_ERR;
      }
      else if(LTE_AUTH_SYNCH_FAILURE == auth_error &&
              autn_data_len > 0 &&
              autn_data_len < 0xFF)
      {
        /* In case of sync failure, lte_perform_milenage_aka does not return
           response as specified in 3GPP Spec 31102 section 7.1.2.1. Length of
           AUTS is expected at index 1. But AUTS data itself starts from index 1.
           autn_data_len denotes size of AUTS data + size of 1 byte tag. 
           Tag is present at index 0. */
        memsmove(&uim_report.rpt.autn.data[MMGSDI_AUTH_RESP_DATA_POS],
                 UIM_MAX_CHARS - MMGSDI_AUTH_RESP_DATA_POS,
                 &uim_report.rpt.autn.data[MMGSDI_AUTH_RESP_LEN_POS],
                 autn_data_len - MMGSDI_AUTH_RESP_TAG_LEN);

        uim_report.rpt.autn.data[MMGSDI_AUTH_RESP_LEN_POS] =
          autn_data_len - MMGSDI_AUTH_RESP_TAG_LEN;

        /* autn_data_len is modified to indicate size of 1 byte tag + size of AUTS length byte + size of AUTS data. */
        autn_data_len = autn_data_len + 1;
      }

      uim_report.rpt.autn.data_length = (word)autn_data_len;
    }
    else
    {
      mmgsdi_util_free_client_request_table_index(index);
      return MMGSDI_ERROR;
    }
  }
#endif /* FEATURE_LTE */
  else
  {
    mmgsdi_util_free_client_request_table_index(index);
    return MMGSDI_ERROR;
  }

  /* Set the other values in uim_report */
  uim_report.rpt_type            = UIM_AUTHENTICATE_R;
  uim_report.cmd_transacted      = FALSE;
  uim_report.user_data           = index;

  /* Queue response */
  mmgsdi_uim_report(&uim_report);

  return mmgsdi_status;
} /* mmgsdi_onchip_uicc_auth */
