/*================================================================================

                         Q M I _ U I M _ S I M L O C K _ P A R S I N G. C


DESCRIPTION
  This file contains functions for QMI UIM service's SIMLOCK TLV parsing functionality.


Copyright (c) 2016-2019 QUALCOMM Technologies, Inc(QTI).
All Rights Reserved.
QUALCOMM Technologies Confidential and Proprietary

$Header: //components/rel/uim.mpss/6.1.0/uimqmi/src/qmi_uim_simlock_parsing.c#3 $

when       who     what, where, why
--------   ---     -----------------------------------------------------------
12/28/19    sg     Added support for NS+SP category
09/10/19    vdc    SimLock solution for activation at Point Of Sale
12/03/18    vgd    Remote SIM Unlock support for SIMLOCK_RSU_MODE_D
07/13/17    av     Enhanced simlock RSU solution
12/27/16    dd     Fix PERSO_RETRIES TC failure
12/02/16    me     Combined multi-line ASSERT in one line
11/03/16    bcho   Added support for ICCID simlock
10/28/16    nr     Support remote and local configurations together
10/06/16    ar     Added support for SP + EHPLMN simlock
08/03/16    vdc    Added support for SPN based SIMLOCK feature
07/18/16    dd     Added feature FEATURE_UIM_TEST_FRAMEWORK
06/16/16    vdc    Initial Revision
=============================================================================*/

#include "uim_variation.h"
#include "comdef.h"
#include "qmi_framework.h"
#include "qmi_uim_parsing.h"
#include "amssassert.h"
#include "uim_msg.h"
#include "user_identity_module_v01.h"
#include "simlock_common.h"
#include "qmi_uim_cat_common.h"
#include "qmi_uim_encryption.h"
#include "qmi_uim_simlock.h"
#include <stringl/stringl.h>

/* These includes need to always be at bottom - offtarget use only */
#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */

#ifdef FEATURE_SIMLOCK
/*---------------------------------------------------------------------------
  Minimum length of Request TLVs with variable size
---------------------------------------------------------------------------*/
#define QMI_UIM_TLV_MIN_LEN_DEPERSONALIZATION        (3)

/*---------------------------------------------------------------------------
  Minimum MNC lengths
---------------------------------------------------------------------------*/
#define QMI_UIM_SIMLOCK_MNC_MIN                      (2)

/*---------------------------------------------------------------------------
  Minimum size of codes
---------------------------------------------------------------------------*/
/* The minimum len is derived with MNC_LEN as 2.
   For NW_CODE: 3 (MCC Bytes) + 1 (MNC_LEN Byte) + 2 (MNC Bytes) */
#define QMI_UIM_TLV_NW_CODE_MIN_LEN            (6)
#define QMI_UIM_TLV_NS_CODE_MIN_LEN            (8)
#define QMI_UIM_TLV_SP_CODE_MIN_LEN            (7)
#define QMI_UIM_TLV_CP_CODE_MIN_LEN            (8)
#define QMI_UIM_TLV_SIM_CODE_MIN_LEN           (6)
#define QMI_UIM_TLV_NW_TYPE2_CODE_MIN_LEN      (4)
#define QMI_UIM_TLV_SP_EXT_CODE_MIN_LEN        (8)
#define QMI_UIM_TLV_CP_EXT_CODE_MIN_LEN        (10)
#define QMI_UIM_TLV_SPN_CODE_MIN_LEN           (22)
#define QMI_UIM_TLV_SP_EHPLMN_MIN_LEN          (14)
#define QMI_UIM_TLV_ICCID_CODE_MIN_LEN         (8)

/*---------------------------------------------------------------------------
  The number of lists needed to accomodate the lock codes.
  Each list can fit SIMLOCK_NUM_CODES_MAX (20) codes.
---------------------------------------------------------------------------*/
#define QMI_UIM_PERSO_GET_LIST_COUNT(x)  ((x > 0) ? ((x - 1) / SIMLOCK_NUM_CODES_MAX + 1) : 0)

/*---------------------------------------------------------------------------
  Encypted/Decrypted magnitudes
---------------------------------------------------------------------------*/
#define QMI_UIM_DECRYPTED_PERSO_SECURE_REQ_MIN_LEN             (15)
#define QMI_UIM_DECRYPT_PERSO_SECURE_SLOT_OFFSET               (8)
#define QMI_UIM_DECRYPT_PERSO_SECURE_CATEGORY_OFFSET           (9)
#define QMI_UIM_DECRYPT_PERSO_SECURE_NUM_CODES_OFFSET          (10)
#define QMI_UIM_DECRYPT_PERSO_CODE_DATA_OFFSET                 (11)
#define QMI_UIM_DECRYPTED_DEPERSO_SECURE_REQ_LEN               (15)
#define QMI_UIM_DECRYPT_DEPERSO_SECURE_SLOT_OFFSET             (8)
#define QMI_UIM_DECRYPT_DEPERSO_SECURE_CATEGORY_OFFSET         (9)
#define QMI_UIM_DECRYPT_DEPERSO_SECURE_OPERATION_OFFSET        (10)
#define QMI_UIM_DECRYPT_DEPERSO_SECURE_DURATION_OFFSET         (11)
#define QMI_UIM_DECRYPTED_RESPONSE_LEN                         (12)
#define QMI_UIM_ENCRYPTED_RESULT_SUCCESS                       (0)
#define QMI_UIM_ENCRYPTED_RESULT_FAILURE                       (1)
#define QMI_UIM_DECRYPTED_EMERGENCY_ONLY_REQ_LEN               (9)
#define QMI_UIM_DECRYPT_EMERGENCY_ONLY_MODE_OFFSET             (8)
#define QMI_UIM_DECRYPTED_SIMLOCK_CONFIG_MIN_LEN               (9)
#define QMI_UIM_DECRYPT_SIMLOCK_CONFIG_OPERATION_OFFSET        (8)
#define QMI_UIM_DECRYPT_SIMLOCK_CONFIG_RESULT_OFFSET           (9)
#define QMI_UIM_DECRYPT_SIMLOCK_CONFIG_FILE_ID_OFFSET          (10)
#define QMI_UIM_DECRYPT_SIMLOCK_CONFIG_READ_LEN_OFFSET         (11)
#define QMI_UIM_DECRYPT_SIMLOCK_CONFIG_READ_DATA_OFFSET        (13)
#define QMI_UIM_DECRYPTED_SIMLOCK_CONFIG_LEN                   (11)
#define QMI_UIM_DECRYPTED_SIMLOCK_CONFIG_READ_LEN              (13)

/*---------------------------------------------------------------------------
  Maxmimum size represented by a unsigned 16 bit value
---------------------------------------------------------------------------*/
#define QMI_UIM_MAX_UINT16                                     (65535)

/*---------------------------------------------------------------------------
  Structure with map between lock category and min/max codes
---------------------------------------------------------------------------*/
typedef struct
{
  simlock_category_enum_type              category;
  uint8                                   num_codes_max;
  uint8                                   code_data_min;
  qmi_uim_simlock_extension_category_type extended_category;
} qmi_uim_category_code_size_map_type;

/*===========================================================================
  FUNCTION QMI_UIM_TLV_DEPERSONALIZATION()

  DESCRIPTION
    Parses the TLV item of deactivate perso

  PARAMETERS
    sdu_in                : input sdu
    len                   : length of the TLV item
    depersonalization_ptr : output with deactivate perso value

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_tlv_depersonalization
(
  dsm_item_type **                     sdu_in,
  uint16                               len,
  qmi_uim_tlv_depersonalization_type * depersonalization_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint8  feature;
    uint8  operation;
    uint8  ck_len;
    uint8  ck_value[QMI_UIM_CK_MAX_V01];
  } tlv_data;

  ASSERT(sdu_in && depersonalization_ptr);

  /* Check length */
  if (len < QMI_UIM_TLV_MIN_LEN_DEPERSONALIZATION)
  {
    return QMI_ERR_MALFORMED_MSG;
  }
  if (len > sizeof(tlv_data))
  {
    return QMI_ERR_ARG_TOO_LONG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, len);
  if (temp != len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Feature */
  switch(tlv_data.feature)
  {
    case UIM_PERSO_FEATURE_GW_NETWORK_V01:
      depersonalization_ptr->category = SIMLOCK_CATEGORY_3GPP_NW;
      break;
    case UIM_PERSO_FEATURE_GW_NETWORK_SUBSET_V01:
      depersonalization_ptr->category = SIMLOCK_CATEGORY_3GPP_NS;
      break;
    case UIM_PERSO_FEATURE_GW_SERVICE_PROVIDER_V01:
      depersonalization_ptr->category = SIMLOCK_CATEGORY_3GPP_SP;
      break;
    case UIM_PERSO_FEATURE_GW_CORPORATE_V01:
      depersonalization_ptr->category = SIMLOCK_CATEGORY_3GPP_CP;
      break;
    case UIM_PERSO_FEATURE_GW_UIM_V01:
      depersonalization_ptr->category = SIMLOCK_CATEGORY_3GPP_SIM;
      break;
    case UIM_PERSO_FEATURE_GW_SPN_V01:
      depersonalization_ptr->category = SIMLOCK_CATEGORY_3GPP_SPN;
      break;
    case UIM_PERSO_FEATURE_GW_SP_EHPLMN_V01:
      depersonalization_ptr->category = SIMLOCK_CATEGORY_3GPP_SP_EHPLMN;
      break;
    case UIM_PERSO_FEATURE_GW_ICCID_V01:
      depersonalization_ptr->category = SIMLOCK_CATEGORY_3GPP_ICCID;
      break;
    case UIM_PERSO_FEATURE_GW_NETWORK_SUBSET_SERVICE_PROVIDER_V01:
      depersonalization_ptr->category = SIMLOCK_CATEGORY_3GPP_NS_SP;
      break;
    case UIM_PERSO_FEATURE_1X_NETWORK_TYPE_1_V01:
      depersonalization_ptr->category = SIMLOCK_CATEGORY_3GPP2_NW_TYPE1;
      break;
    case UIM_PERSO_FEATURE_1X_NETWORK_TYPE_2_V01:
      depersonalization_ptr->category = SIMLOCK_CATEGORY_3GPP2_NW_TYPE2;
      break;
    case UIM_PERSO_FEATURE_1X_HRPD_V01:
      depersonalization_ptr->category = SIMLOCK_CATEGORY_3GPP2_HRPD;
      break;
    case UIM_PERSO_FEATURE_1X_SERVICE_PROVIDER_V01:
      depersonalization_ptr->category = SIMLOCK_CATEGORY_3GPP2_CP;
      break;
    case UIM_PERSO_FEATURE_1X_CORPORATE_V01:
      depersonalization_ptr->category = SIMLOCK_CATEGORY_3GPP2_CP;
      break;
    case UIM_PERSO_FEATURE_1X_RUIM_V01:
      depersonalization_ptr->category = SIMLOCK_CATEGORY_3GPP2_RUIM;
      break;
    default:
      return QMI_ERR_INVALID_ARG;
  }

  /* Operation */
  switch (tlv_data.operation)
  {
    case UIM_PERSO_OPERATION_DEACTIVATE_V01:
      /* Indicates the operation is to unlock the SIM */
      break;
    case UIM_PERSO_OPERATION_UNBLOCK_V01:
      return QMI_ERR_NOT_SUPPORTED;
    default:
      return QMI_ERR_INVALID_ARG;
  }

  if (tlv_data.ck_len == 0 ||
      tlv_data.ck_len > QMI_UIM_CK_MAX_V01)
  {
    return QMI_ERR_INVALID_ARG;
  }

  /* Check data length */
  if (len != QMI_UIM_TLV_MIN_LEN_DEPERSONALIZATION +
             tlv_data.ck_len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Control key length */
  depersonalization_ptr->ck.simlock_ck_length =
    tlv_data.ck_len;

  /* Control key value */
  (void)memscpy((void*)depersonalization_ptr->ck.simlock_ck_data,
                sizeof(depersonalization_ptr->ck.simlock_ck_data),
                (void*)tlv_data.ck_value,
                (uint32)depersonalization_ptr->ck.simlock_ck_length);

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_depersonalization */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_PERSONALIZATION_PARSE_ICCID_CODE()

  DESCRIPTION
    Parses the ICCID codes

  PARAMETERS
    num_codes           : number of ICCID codes
    code_data_buf_len   : length of raw code data
    code_data_buf_ptr   : pointer to raw code data
    num_lists           : number of lists needed for ICCID codes
    extended_category   : type of category. normal, ext or ext2
    iccid_code_data_ptr : output with ICCID codes

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_personalization_parse_iccid_code
(
  uint8                                    num_codes,
  uint16                                   code_data_buf_len,
  const uint8                            * code_data_buf_ptr,
  uint8                                    num_lists,
  simlock_category_data_type             * iccid_code_data_ptr
)
{
  qmi_error_e_type   ret_value         = QMI_ERR_NONE;
  uint8              code_cnt          = 0;
  uint8              list_cnt          = 0;
  uint8              list_code_cnt     = 0;
  uint8              mnc_len           = 0;
  const uint8      * current_code_ptr  = NULL;
  uint8              iccid_len         = 0;

  if ((code_data_buf_len == 0) ||
      (code_data_buf_ptr == NULL) ||
      (iccid_code_data_ptr == NULL))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  current_code_ptr = code_data_buf_ptr;

  for(code_cnt = 0; code_cnt < num_codes; code_cnt++)
  {
    /* Check the data size */
    if(current_code_ptr + SIMLOCK_MCC_LEN + sizeof(mnc_len) >
         code_data_buf_ptr + code_data_buf_len)
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    /* Each list will fit, SIMLOCK_NUM_CODES_MAX. So if
       list_code_cnt is SIMLOCK_NUM_CODES_MAX, then fill
       the next list */
    if(list_code_cnt == SIMLOCK_NUM_CODES_MAX)
    {
      if(list_cnt < num_lists)
      {
        list_code_cnt = 0;
        list_cnt++;
      }
      else
      {
        ret_value = QMI_ERR_MALFORMED_MSG;
        break;
      }
    }

    if((list_cnt >= num_lists) ||
       (list_code_cnt >= SIMLOCK_NUM_CODES_MAX))
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    (void)memscpy((void*)iccid_code_data_ptr[list_cnt].code_data.iccid_3gpp_code_list.iccid_code_data[list_code_cnt].nw_code.mcc,
                    SIMLOCK_MCC_LEN,
                    current_code_ptr,
                    SIMLOCK_MCC_LEN);
    current_code_ptr += SIMLOCK_MCC_LEN;

    mnc_len = *current_code_ptr;

    /* validate the mnc_len */
    if((mnc_len != QMI_UIM_SIMLOCK_MNC_MIN) &&
       (mnc_len != QMI_UIM_MNC_MAX_V01))
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    iccid_code_data_ptr[list_cnt].code_data.iccid_3gpp_code_list.iccid_code_data[list_code_cnt].nw_code.mnc.mnc_len = mnc_len;
    current_code_ptr += sizeof(mnc_len);

    if(current_code_ptr + mnc_len > code_data_buf_ptr + code_data_buf_len)
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    (void)memscpy((void*)iccid_code_data_ptr[list_cnt].code_data.iccid_3gpp_code_list.iccid_code_data[list_code_cnt].nw_code.mnc.mnc_data,
            SIMLOCK_MNC_MAX,
            current_code_ptr,
            iccid_code_data_ptr[list_cnt].code_data.iccid_3gpp_code_list.iccid_code_data[list_code_cnt].nw_code.mnc.mnc_len);
    current_code_ptr += mnc_len;

    /* Check the data size */
    if(current_code_ptr + sizeof(uint8) > code_data_buf_ptr + code_data_buf_len)
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    iccid_len = *current_code_ptr;

    /* validate the iccid_len */
    if(iccid_len == 0)
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    if(iccid_len > QMI_UIM_ICCID_DIGIT_MAX_V01)
    {
      ret_value = QMI_ERR_ARG_TOO_LONG;
      break;
    }

    iccid_code_data_ptr[list_cnt].code_data.iccid_3gpp_code_list.iccid_code_data[list_code_cnt].iccid.iccid_len = iccid_len;
    current_code_ptr += sizeof(uint8);

    /* Check the data size */
    if(current_code_ptr + iccid_len > code_data_buf_ptr + code_data_buf_len)
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    (void)memscpy((void*)iccid_code_data_ptr[list_cnt].code_data.iccid_3gpp_code_list.iccid_code_data[list_code_cnt].iccid.iccid_data,
            sizeof(iccid_code_data_ptr[list_cnt].code_data.iccid_3gpp_code_list.iccid_code_data[list_code_cnt].iccid.iccid_data),
            current_code_ptr,
            iccid_len);
    current_code_ptr += iccid_len;

    iccid_code_data_ptr[list_cnt].code_data.iccid_3gpp_code_list.num_iccid_codes++;
    list_code_cnt++;
  }

  if (ret_value == QMI_ERR_NONE &&
      current_code_ptr != code_data_buf_ptr + code_data_buf_len)
  {
    ret_value = QMI_ERR_MALFORMED_MSG;
  }

  return ret_value;
} /* qmi_uim_tlv_personalization_parse_iccid_code*/


/*===========================================================================
  FUNCTION QMI_UIM_TLV_PERSONALIZATION_PARSE_SPN_CODE()

  DESCRIPTION
    Parses the SPN codes

  PARAMETERS
    num_codes           : number of SPN codes
    code_data_buf_len   : length of raw code data
    code_data_buf_ptr   : pointer to raw code data
    num_lists           : number of lists needed for SPN codes
    spn_code_data_ptr   : output with SPN codes

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_personalization_parse_spn_code
(
  uint8                              num_codes,
  uint16                             code_data_buf_len,
  const uint8                      * code_data_buf_ptr,
  uint8                              num_lists,
  simlock_category_data_type     *   spn_code_data_ptr
)
{
  qmi_error_e_type   ret_value         = QMI_ERR_NONE;
  uint8              code_cnt          = 0;
  uint8              list_cnt          = 0;
  uint8              list_code_cnt     = 0;
  uint8              mnc_len           = 0;
  const uint8      * current_code_ptr  = NULL;

  if ((code_data_buf_len == 0) ||
      (code_data_buf_ptr == NULL) ||
      (spn_code_data_ptr == NULL))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  current_code_ptr = code_data_buf_ptr;

  for(code_cnt = 0; code_cnt < num_codes; code_cnt++)
  {
    /* Check the data size */
    if(current_code_ptr + SIMLOCK_MCC_LEN + sizeof(mnc_len) >
        code_data_buf_ptr + code_data_buf_len)
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    /* Each list will fit, SIMLOCK_NUM_CODES_MAX. So if
       list_code_cnt is SIMLOCK_NUM_CODES_MAX, then fill
       the next list */
    if(list_code_cnt == SIMLOCK_NUM_CODES_MAX)
    {
      if(list_cnt < num_lists)
      {
        list_code_cnt = 0;
        list_cnt++;
      }
      else
      {
        ret_value = QMI_ERR_MALFORMED_MSG;
        break;
      }
    }

    if((list_cnt >= num_lists) ||
       (list_code_cnt >= SIMLOCK_NUM_CODES_MAX))
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    (void)memscpy((void*)spn_code_data_ptr[list_cnt].code_data.spn_3gpp_code_list.spn_code_data[list_code_cnt].nw_code.mcc,
                  SIMLOCK_MCC_LEN,
                  current_code_ptr,
                  SIMLOCK_MCC_LEN);
    current_code_ptr += SIMLOCK_MCC_LEN;

    mnc_len = *current_code_ptr;

    /* validate the mnc_len */
    if((mnc_len != QMI_UIM_SIMLOCK_MNC_MIN) &&
       (mnc_len != QMI_UIM_MNC_MAX_V01))
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    spn_code_data_ptr[list_cnt].code_data.spn_3gpp_code_list.spn_code_data[list_code_cnt].nw_code.mnc.mnc_len = mnc_len;
    current_code_ptr += sizeof(mnc_len);

    if(current_code_ptr + mnc_len > code_data_buf_ptr + code_data_buf_len)
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    (void)memscpy((void*)spn_code_data_ptr[list_cnt].code_data.spn_3gpp_code_list.spn_code_data[list_code_cnt].nw_code.mnc.mnc_data,
                  SIMLOCK_MNC_MAX,
                  current_code_ptr,
                  spn_code_data_ptr[list_cnt].code_data.spn_3gpp_code_list.spn_code_data[list_code_cnt].nw_code.mnc.mnc_len);
    current_code_ptr += mnc_len;

    /* Check the data size */
    if(current_code_ptr + SIMLOCK_SPN_LEN > code_data_buf_ptr + code_data_buf_len)
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    (void)memscpy(spn_code_data_ptr[list_cnt].code_data.spn_3gpp_code_list.spn_code_data[list_code_cnt].spn.spn_data,
                  SIMLOCK_SPN_LEN,
                  current_code_ptr,
                  SIMLOCK_SPN_LEN);

    spn_code_data_ptr[list_cnt].code_data.spn_3gpp_code_list.spn_code_data[list_code_cnt].spn.spn_valid = TRUE;

    current_code_ptr += SIMLOCK_SPN_LEN;

    spn_code_data_ptr[list_cnt].code_data.spn_3gpp_code_list.num_spn_codes++;
    list_code_cnt++;
  }

  if (ret_value == QMI_ERR_NONE &&
      current_code_ptr != code_data_buf_ptr + code_data_buf_len)
  {
    ret_value = QMI_ERR_MALFORMED_MSG;
  }

  return ret_value;
} /* qmi_uim_tlv_personalization_parse_spn_code*/


/*===========================================================================
  FUNCTION QMI_UIM_TLV_PERSONALIZATION_PARSE_RUIM_CODE()

  DESCRIPTION
    Parses the RUIM codes

  PARAMETERS
    num_codes          : number of RUIM codes
    code_data_buf_len  : length of raw code data
    code_data_buf_ptr  : pointer to raw code data
    num_lists          : number of lists needed for RUIM codes
    ruim_code_data_ptr : output with RUIM codes

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_personalization_parse_ruim_code
(
  uint8                              num_codes,
  uint16                             code_data_buf_len,
  const uint8                    *   code_data_buf_ptr,
  uint8                              num_lists,
  simlock_category_data_type     *   ruim_code_data_ptr
)
{
  qmi_error_e_type   ret_value         = QMI_ERR_NONE;
  uint8              code_cnt          = 0;
  uint8              list_cnt          = 0;
  uint8              list_code_cnt     = 0;
  uint8              mnc_len           = 0;
  uint8              msin_len          = 0;
  const uint8      * current_code_ptr  = NULL;

  if ((code_data_buf_len == 0) ||
      (code_data_buf_ptr == NULL) ||
      (ruim_code_data_ptr == NULL))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  current_code_ptr = code_data_buf_ptr;

  for(code_cnt = 0; code_cnt < num_codes; code_cnt++)
  {
    /* Check the data size */
    if(current_code_ptr + SIMLOCK_MCC_LEN + sizeof(mnc_len) >
        code_data_buf_ptr + code_data_buf_len)
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    /* Each list will fit, SIMLOCK_NUM_CODES_MAX. So if
       list_code_cnt is SIMLOCK_NUM_CODES_MAX, then fill
       the next list */
    if(list_code_cnt == SIMLOCK_NUM_CODES_MAX)
    {
      if(list_cnt < num_lists)
      {
        list_code_cnt = 0;
        list_cnt++;
      }
      else
      {
        ret_value = QMI_ERR_MALFORMED_MSG;
        break;
      }
    }

    if((list_cnt >= num_lists) ||
       (list_code_cnt >= SIMLOCK_NUM_CODES_MAX))
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    (void)memscpy((void*)ruim_code_data_ptr[list_cnt].code_data.ruim_3gpp2_code_list.sim_code_data[list_code_cnt].nw_code.mcc,
                    SIMLOCK_MCC_LEN,
                    current_code_ptr,
                    SIMLOCK_MCC_LEN);
    current_code_ptr += SIMLOCK_MCC_LEN;

    mnc_len = *current_code_ptr;

    /* validate the mnc_len */
    if((mnc_len != QMI_UIM_SIMLOCK_MNC_MIN) &&
       (mnc_len != QMI_UIM_MNC_MAX_V01))
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    ruim_code_data_ptr[list_cnt].code_data.ruim_3gpp2_code_list.sim_code_data[list_code_cnt].nw_code.mnc.mnc_len = mnc_len;
    current_code_ptr += sizeof(mnc_len);

    /* Check the data size */
    if(current_code_ptr + mnc_len + sizeof(msin_len) > code_data_buf_ptr + code_data_buf_len)
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    (void)memscpy((void*)ruim_code_data_ptr[list_cnt].code_data.ruim_3gpp2_code_list.sim_code_data[list_code_cnt].nw_code.mnc.mnc_data,
                  SIMLOCK_MNC_MAX,
                  current_code_ptr,
                  ruim_code_data_ptr[list_cnt].code_data.ruim_3gpp2_code_list.sim_code_data[list_code_cnt].nw_code.mnc.mnc_len);
    current_code_ptr += mnc_len;

    msin_len = *current_code_ptr;

    /* validate the msin_len */
    if(msin_len > SIMLOCK_MSIN_MAX)
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    ruim_code_data_ptr[list_cnt].code_data.ruim_3gpp2_code_list.sim_code_data[list_code_cnt].imsi_msin.msin_len = msin_len;
    current_code_ptr += sizeof(msin_len);

    /* Check the data size */
    if(current_code_ptr + msin_len > code_data_buf_ptr + code_data_buf_len)
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    (void)memscpy((void*)ruim_code_data_ptr[list_cnt].code_data.ruim_3gpp2_code_list.sim_code_data[list_code_cnt].imsi_msin.msin_data,
                  SIMLOCK_MSIN_MAX,
                  current_code_ptr,
                  ruim_code_data_ptr[list_cnt].code_data.ruim_3gpp2_code_list.sim_code_data[list_code_cnt].imsi_msin.msin_len);
    current_code_ptr += msin_len;

    ruim_code_data_ptr[list_cnt].code_data.ruim_3gpp2_code_list.num_sim_codes++;
    list_code_cnt++;
  }

  if (ret_value == QMI_ERR_NONE &&
      current_code_ptr != code_data_buf_ptr + code_data_buf_len)
  {
    ret_value = QMI_ERR_MALFORMED_MSG;
  }

  return ret_value;
} /* qmi_uim_tlv_personalization_parse_ruim_code */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_PERSONALIZATION_PARSE_NW_TYPE2_CODE()

  DESCRIPTION
    Parses the NW type2 codes

  PARAMETERS
    num_codes              : number of NW Type2 codes
    code_data_buf_len      : length of raw code data
    code_data_buf_ptr      : pointer to raw code data
    num_lists              : number of lists needed for nw_type2 codes
    nw_type2_code_data_ptr : output with NW Type2 codes

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_personalization_parse_nw_type2_code
(
  uint8                              num_codes,
  uint16                             code_data_buf_len,
  const uint8                    *   code_data_buf_ptr,
  uint8                              num_lists,
  simlock_category_data_type     *   nw_type2_code_data_ptr
)
{
  qmi_error_e_type   ret_value         = QMI_ERR_NONE;
  uint8              code_cnt          = 0;
  uint8              list_cnt          = 0;
  uint8              list_code_cnt     = 0;
  const uint8      * current_code_ptr  = NULL;

  if ((code_data_buf_len == 0) ||
      (code_data_buf_ptr == NULL) ||
      (nw_type2_code_data_ptr == NULL))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  current_code_ptr = code_data_buf_ptr;

  for(code_cnt = 0; code_cnt < num_codes; code_cnt++)
  {
    /* Check the data size */
    if(code_data_buf_ptr + SIMLOCK_IRM_LEN > code_data_buf_ptr + code_data_buf_len)
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    /* Each list will fit, SIMLOCK_NUM_CODES_MAX. So if
       list_code_cnt is SIMLOCK_NUM_CODES_MAX, then fill
       the next list */
    if(list_code_cnt == SIMLOCK_NUM_CODES_MAX)
    {
      if(list_cnt < num_lists)
      {
        list_code_cnt = 0;
        list_cnt++;
      }
      else
      {
        ret_value = QMI_ERR_MALFORMED_MSG;
        break;
      }
    }

    if((list_cnt >= num_lists) ||
       (list_code_cnt >= SIMLOCK_NUM_CODES_MAX))
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    nw_type2_code_data_ptr[list_cnt].category_type = SIMLOCK_CATEGORY_3GPP2_NW_TYPE2;

    (void)memscpy((void*)nw_type2_code_data_ptr[list_cnt].code_data.nw_type2_3gpp2_code_list.nw_type2_code_data[list_code_cnt].irm,
                  SIMLOCK_IRM_LEN,
                  current_code_ptr,
                  SIMLOCK_IRM_LEN);
    current_code_ptr += SIMLOCK_IRM_LEN;

    nw_type2_code_data_ptr[list_cnt].code_data.nw_type2_3gpp2_code_list.num_nw_type2_codes++;
    list_code_cnt++;
  }

  if (ret_value == QMI_ERR_NONE &&
      current_code_ptr != code_data_buf_ptr + code_data_buf_len)
  {
    ret_value = QMI_ERR_MALFORMED_MSG;
  }

  return ret_value;
} /* qmi_uim_tlv_personalization_parse_nw_type2_code */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_PERSONALIZATION_PARSE_NW_TYPE1_CODE()

  DESCRIPTION
    Parses the NW type1 codes

  PARAMETERS
    num_codes              : number of NW Type1 codes
    code_data_buf_len      : length of raw code data
    code_data_buf_ptr      : pointer to raw code data
    num_lists              : number of lists needed for nw_type1 codes
    nw_type1_code_data_ptr : output with NW Type1 codes

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_personalization_parse_nw_type1_code
(
  uint8                              num_codes,
  uint16                             code_data_buf_len,
  const uint8                    *   code_data_buf_ptr,
  uint8                              num_lists,
  simlock_category_data_type     *   nw_type1_code_data_ptr
)
{
  qmi_error_e_type   ret_value         = QMI_ERR_NONE;
  uint8              code_cnt          = 0;
  uint8              list_cnt          = 0;
  uint8              list_code_cnt     = 0;
  uint8              mnc_len           = 0;
  const uint8      * current_code_ptr  = NULL;

  if ((code_data_buf_len == 0) ||
      (code_data_buf_ptr == NULL) ||
      (nw_type1_code_data_ptr == NULL))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  current_code_ptr = code_data_buf_ptr;

  for(code_cnt = 0; code_cnt < num_codes; code_cnt++)
  {
    /* Check the data size */
    if(current_code_ptr + SIMLOCK_MCC_LEN + sizeof(mnc_len) >
        code_data_buf_ptr + code_data_buf_len)
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    /* Each list will fit, SIMLOCK_NUM_CODES_MAX. So if
       list_code_cnt is SIMLOCK_NUM_CODES_MAX, then fill
       the next list */
    if(list_code_cnt == SIMLOCK_NUM_CODES_MAX)
    {
      if(list_cnt < num_lists)
      {
        list_code_cnt = 0;
        list_cnt++;
      }
      else
      {
        ret_value = QMI_ERR_MALFORMED_MSG;
        break;
      }
    }

    if((list_cnt >= num_lists) ||
       (list_code_cnt >= SIMLOCK_NUM_CODES_MAX))
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    (void)memscpy((void*)nw_type1_code_data_ptr[list_cnt].code_data.nw_type1_3gpp2_code_list.nw_code_data[list_code_cnt].mcc,
                    SIMLOCK_MCC_LEN,
                    current_code_ptr,
                    SIMLOCK_MCC_LEN);
    current_code_ptr += SIMLOCK_MCC_LEN;

    mnc_len = *current_code_ptr;

    /* validate the mnc_len */
    if((mnc_len != QMI_UIM_SIMLOCK_MNC_MIN) &&
       (mnc_len != QMI_UIM_MNC_MAX_V01))
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    nw_type1_code_data_ptr[list_cnt].code_data.nw_type1_3gpp2_code_list.nw_code_data[list_code_cnt].mnc.mnc_len = mnc_len;
    current_code_ptr += sizeof(mnc_len);

    /* Check the data size */
    if(current_code_ptr + mnc_len > code_data_buf_ptr + code_data_buf_len)
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    (void)memscpy((void*)nw_type1_code_data_ptr[list_cnt].code_data.nw_type1_3gpp2_code_list.nw_code_data[list_code_cnt].mnc.mnc_data,
                  SIMLOCK_MNC_MAX,
                  current_code_ptr,
                  nw_type1_code_data_ptr[list_cnt].code_data.nw_type1_3gpp2_code_list.nw_code_data[list_code_cnt].mnc.mnc_len);
    current_code_ptr += mnc_len;

    nw_type1_code_data_ptr[list_cnt].code_data.nw_type1_3gpp2_code_list.num_nw_codes++;
    list_code_cnt++;
  }

  if (ret_value == QMI_ERR_NONE &&
      current_code_ptr != code_data_buf_ptr + code_data_buf_len)
  {
    ret_value = QMI_ERR_MALFORMED_MSG;
  }

  return ret_value;
} /* qmi_uim_tlv_personalization_parse_nw_type1_code */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_PERSONALIZATION_PARSE_SIM_CODE()

  DESCRIPTION
    Parses the SIM codes

  PARAMETERS
    num_codes           : number of SIM codes
    code_data_buf_len   : length of raw code data
    code_data_buf_ptr   : pointer to raw code data
    num_lists           : number of lists needed for SIM codes
    sim_code_data_ptr   : output with SIM codes

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_personalization_parse_sim_code
(
  uint8                              num_codes,
  uint16                             code_data_buf_len,
  const uint8                    *   code_data_buf_ptr,
  uint8                              num_lists,
  simlock_category_data_type     *   sim_code_data_ptr
)
{
  qmi_error_e_type   ret_value         = QMI_ERR_NONE;
  uint8              code_cnt          = 0;
  uint8              list_cnt          = 0;
  uint8              list_code_cnt     = 0;
  uint8              mnc_len           = 0;
  uint8              msin_len          = 0;
  const uint8      * current_code_ptr  = NULL;

  if ((code_data_buf_len == 0) ||
      (code_data_buf_ptr == NULL) ||
      (sim_code_data_ptr == NULL))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  current_code_ptr = code_data_buf_ptr;

  for(code_cnt = 0; code_cnt < num_codes; code_cnt++)
  {
    /* Check the data size */
    if(current_code_ptr + SIMLOCK_MCC_LEN + sizeof(mnc_len) >
        code_data_buf_ptr + code_data_buf_len)
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    /* Each list will fit, SIMLOCK_NUM_CODES_MAX. So if
       list_code_cnt is SIMLOCK_NUM_CODES_MAX, then fill
       the next list */
    if(list_code_cnt == SIMLOCK_NUM_CODES_MAX)
    {
      if(list_cnt < num_lists)
      {
        list_code_cnt = 0;
        list_cnt++;
      }
      else
      {
        ret_value = QMI_ERR_MALFORMED_MSG;
        break;
      }
    }

    if((list_cnt >= num_lists) ||
       (list_code_cnt >= SIMLOCK_NUM_CODES_MAX))
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    (void)memscpy((void*)sim_code_data_ptr[list_cnt].code_data.sim_3gpp_code_list.sim_code_data[list_code_cnt].nw_code.mcc,
                    SIMLOCK_MCC_LEN,
                    current_code_ptr,
                    SIMLOCK_MCC_LEN);
    current_code_ptr += SIMLOCK_MCC_LEN;

    mnc_len = *current_code_ptr;

    /* validate the mnc_len */
    if((mnc_len != QMI_UIM_SIMLOCK_MNC_MIN) &&
       (mnc_len != QMI_UIM_MNC_MAX_V01))
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    sim_code_data_ptr[list_cnt].code_data.sim_3gpp_code_list.sim_code_data[list_code_cnt].nw_code.mnc.mnc_len = mnc_len;
    current_code_ptr += sizeof(mnc_len);

    /* Check the data size */
    if(current_code_ptr + mnc_len + sizeof(msin_len) > code_data_buf_ptr + code_data_buf_len)
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    (void)memscpy((void*)sim_code_data_ptr[list_cnt].code_data.sim_3gpp_code_list.sim_code_data[list_code_cnt].nw_code.mnc.mnc_data,
            SIMLOCK_MNC_MAX,
            current_code_ptr,
            sim_code_data_ptr[list_cnt].code_data.sim_3gpp_code_list.sim_code_data[list_code_cnt].nw_code.mnc.mnc_len);
    current_code_ptr += mnc_len;

    msin_len = *current_code_ptr;

    /* validate the msin_len */
    if(msin_len > SIMLOCK_MSIN_MAX)
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    sim_code_data_ptr[list_cnt].code_data.sim_3gpp_code_list.sim_code_data[list_code_cnt].imsi_msin.msin_len = msin_len;
    current_code_ptr += sizeof(msin_len);

    /* Check the data size */
    if(current_code_ptr + msin_len > code_data_buf_ptr + code_data_buf_len)
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    (void)memscpy((void*)sim_code_data_ptr[list_cnt].code_data.sim_3gpp_code_list.sim_code_data[list_code_cnt].imsi_msin.msin_data,
            SIMLOCK_MSIN_MAX,
            current_code_ptr,
            sim_code_data_ptr[list_cnt].code_data.sim_3gpp_code_list.sim_code_data[list_code_cnt].imsi_msin.msin_len);
    current_code_ptr += msin_len;

    sim_code_data_ptr[list_cnt].code_data.sim_3gpp_code_list.num_sim_codes++;
    list_code_cnt++;
  }

  if (ret_value == QMI_ERR_NONE &&
      current_code_ptr != code_data_buf_ptr + code_data_buf_len)
  {
    ret_value = QMI_ERR_MALFORMED_MSG;
  }

  return ret_value;
} /* qmi_uim_tlv_personalization_parse_sim_code */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_PERSONALIZATION_PARSE_CP_CODE()

  DESCRIPTION
    Parses the CP codes

  PARAMETERS
    num_codes           : number of CP codes
    code_data_buf_len   : length of raw code data
    code_data_buf_ptr   : pointer to raw code data
    num_lists           : number of lists needed for CP codes
    extended_category   : type of category. normal, ext or ext2
    cp_code_data_ptr    : output with CP codes

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_personalization_parse_cp_code
(
  uint8                                      num_codes,
  uint16                                     code_data_buf_len,
  const uint8                              * code_data_buf_ptr,
  uint8                                      num_lists,
  qmi_uim_simlock_extension_category_type    extended_category,
  simlock_category_data_type               * cp_code_data_ptr
)
{
  qmi_error_e_type   ret_value         = QMI_ERR_NONE;
  uint8              code_cnt          = 0;
  uint8              list_cnt          = 0;
  uint8              list_code_cnt     = 0;
  uint8              mnc_len           = 0;
  const uint8      * current_code_ptr  = NULL;

  if ((code_data_buf_len == 0) ||
      (code_data_buf_ptr == NULL) ||
      (cp_code_data_ptr == NULL))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  current_code_ptr = code_data_buf_ptr;

  for(code_cnt = 0; code_cnt < num_codes; code_cnt++)
  {
    /* Check the data size */
    if(current_code_ptr + SIMLOCK_MCC_LEN + sizeof(mnc_len) >
        code_data_buf_ptr + code_data_buf_len)
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    /* Each list will fit, SIMLOCK_NUM_CODES_MAX. So if
       list_code_cnt is SIMLOCK_NUM_CODES_MAX, then fill
       the next list */
    if(list_code_cnt == SIMLOCK_NUM_CODES_MAX)
    {
      if(list_cnt < num_lists)
      {
        list_code_cnt = 0;
        list_cnt++;
      }
      else
      {
        ret_value = QMI_ERR_MALFORMED_MSG;
        break;
      }
    }

    if((list_cnt >= num_lists) ||
       (list_code_cnt >= SIMLOCK_NUM_CODES_MAX))
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    (void)memscpy((void*)cp_code_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[list_code_cnt].nw_code.mcc,
                    SIMLOCK_MCC_LEN,
                    current_code_ptr,
                    SIMLOCK_MCC_LEN);
    current_code_ptr += SIMLOCK_MCC_LEN;

    mnc_len = *current_code_ptr;

    /* validate the mnc_len */
    if((mnc_len != QMI_UIM_SIMLOCK_MNC_MIN) &&
       (mnc_len != QMI_UIM_MNC_MAX_V01))
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    cp_code_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[list_code_cnt].nw_code.mnc.mnc_len = mnc_len;
    current_code_ptr += sizeof(mnc_len);

    /* Check the data size */
    if(current_code_ptr + mnc_len > code_data_buf_ptr + code_data_buf_len)
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    (void)memscpy((void*)cp_code_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[list_code_cnt].nw_code.mnc.mnc_data,
                  SIMLOCK_MNC_MAX,
                  current_code_ptr,
                  cp_code_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[list_code_cnt].nw_code.mnc.mnc_len);
    current_code_ptr += mnc_len;

    if (extended_category == QMI_UIM_CATEGORY_TYPE_EXT2 ||
        extended_category == QMI_UIM_CATEGORY_TYPE_EXT)
    {
      uint8 max_gid_len = QMI_UIM_GID_MAX_V01;
      uint8 gid_len     = 0;

      if (extended_category == QMI_UIM_CATEGORY_TYPE_EXT2)
      {
        max_gid_len = QMI_UIM_GID_EXT_MAX_V01;
      }

      /* Check the data size */
      if(current_code_ptr + sizeof(uint8) >
          code_data_buf_ptr + code_data_buf_len)
      {
        ret_value = QMI_ERR_MALFORMED_MSG;
        break;
      }

      gid_len = *current_code_ptr;

      /* validate the gid_len */
      if(gid_len == 0)
      {
        ret_value = QMI_ERR_MALFORMED_MSG;
        break;
      }

      if(gid_len > max_gid_len)
      {
        ret_value = QMI_ERR_ARG_TOO_LONG;
        break;
      }

      cp_code_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[list_code_cnt].gid1.gid_len = gid_len;
      current_code_ptr += sizeof(uint8);

      /* Check the data size */
      if(current_code_ptr + gid_len > code_data_buf_ptr + code_data_buf_len)
      {
        ret_value = QMI_ERR_MALFORMED_MSG;
        break;
      }

      (void)memscpy((void*)cp_code_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[list_code_cnt].gid1.gid_data,
              sizeof(cp_code_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[list_code_cnt].gid1.gid_data),
              current_code_ptr,
              gid_len);
      current_code_ptr += gid_len;

      /* Check the data size */
      if(current_code_ptr + sizeof(uint8) >
         code_data_buf_ptr + code_data_buf_len)
      {
        ret_value = QMI_ERR_MALFORMED_MSG;
        break;
      }

      gid_len = *current_code_ptr;

      /* validate the gid_len */
      if(gid_len == 0)
      {
        ret_value = QMI_ERR_MALFORMED_MSG;
        break;
      }

      if(gid_len > max_gid_len)
      {
        ret_value = QMI_ERR_ARG_TOO_LONG;
        break;
      }

      cp_code_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[list_code_cnt].gid2.gid_len = gid_len;
      current_code_ptr += sizeof(uint8);

      /* Check the data size */
      if(current_code_ptr + gid_len > code_data_buf_ptr + code_data_buf_len)
      {
        ret_value = QMI_ERR_MALFORMED_MSG;
        break;
      }

      (void)memscpy((void*)cp_code_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[list_code_cnt].gid2.gid_data,
              sizeof(cp_code_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[list_code_cnt].gid2.gid_data),
              current_code_ptr,
              gid_len);
      current_code_ptr += gid_len;
    }
    else
    {
      /* Check the data size */
      if(current_code_ptr + sizeof(uint8) + sizeof(uint8) >
          code_data_buf_ptr + code_data_buf_len)
      {
        ret_value = QMI_ERR_MALFORMED_MSG;
        break;
      }

      cp_code_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[list_code_cnt].gid1.gid_data[0] =
        *current_code_ptr;
      cp_code_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[list_code_cnt].gid1.gid_len = 1;

      current_code_ptr += sizeof(uint8);

      cp_code_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[list_code_cnt].gid2.gid_data[0] =
        *current_code_ptr;
      cp_code_data_ptr[list_cnt].code_data.cp_3gpp_code_list.cp_code_data[list_code_cnt].gid2.gid_len = 1;

      current_code_ptr += sizeof(uint8);
    }

    cp_code_data_ptr[list_cnt].code_data.cp_3gpp_code_list.num_cp_codes++;
    list_code_cnt++;
  }

  if (ret_value == QMI_ERR_NONE &&
      current_code_ptr != code_data_buf_ptr + code_data_buf_len)
  {
    ret_value = QMI_ERR_MALFORMED_MSG;
  }

  return ret_value;
} /* qmi_uim_tlv_personalization_parse_cp_code */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_PERSONALIZATION_PARSE_SP_CODE()

  DESCRIPTION
    Parses the SP codes

  PARAMETERS
    num_codes           : number of SP codes
    code_data_buf_len   : length of raw code data
    code_data_buf_ptr   : pointer to raw code data
    num_lists           : number of lists needed for SP codes
    extended_category   : type of category. normal, ext or ext2
    sp_code_data_ptr    : output with SP codes

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_personalization_parse_sp_code
(
  uint8                                    num_codes,
  uint16                                   code_data_buf_len,
  const uint8                            * code_data_buf_ptr,
  uint8                                    num_lists,
  qmi_uim_simlock_extension_category_type  extended_category,
  simlock_category_data_type             * sp_code_data_ptr
)
{
  qmi_error_e_type   ret_value         = QMI_ERR_NONE;
  uint8              code_cnt          = 0;
  uint8              list_cnt          = 0;
  uint8              list_code_cnt     = 0;
  uint8              mnc_len           = 0;
  const uint8      * current_code_ptr  = NULL;

  if ((code_data_buf_len == 0) ||
      (code_data_buf_ptr == NULL) ||
      (sp_code_data_ptr == NULL))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  current_code_ptr = code_data_buf_ptr;

  for(code_cnt = 0; code_cnt < num_codes; code_cnt++)
  {
    /* Check the data size */
    if(current_code_ptr + SIMLOCK_MCC_LEN + sizeof(mnc_len) >
        code_data_buf_ptr + code_data_buf_len)
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    /* Each list will fit, SIMLOCK_NUM_CODES_MAX. So if
       list_code_cnt is SIMLOCK_NUM_CODES_MAX, then fill
       the next list */
    if(list_code_cnt == SIMLOCK_NUM_CODES_MAX)
    {
      if(list_cnt < num_lists)
      {
        list_code_cnt = 0;
        list_cnt++;
      }
      else
      {
        ret_value = QMI_ERR_MALFORMED_MSG;
        break;
      }
    }

    if((list_cnt >= num_lists) ||
       (list_code_cnt >= SIMLOCK_NUM_CODES_MAX))
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    (void)memscpy((void*)sp_code_data_ptr[list_cnt].code_data.sp_3gpp_code_list.sp_code_data[list_code_cnt].nw_code.mcc,
                    SIMLOCK_MCC_LEN,
                    current_code_ptr,
                    SIMLOCK_MCC_LEN);
    current_code_ptr += SIMLOCK_MCC_LEN;

    mnc_len = *current_code_ptr;

    /* validate the mnc_len */
    if((mnc_len != QMI_UIM_SIMLOCK_MNC_MIN) &&
       (mnc_len != QMI_UIM_MNC_MAX_V01))
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    sp_code_data_ptr[list_cnt].code_data.sp_3gpp_code_list.sp_code_data[list_code_cnt].nw_code.mnc.mnc_len = mnc_len;
    current_code_ptr += sizeof(mnc_len);

    if(current_code_ptr + mnc_len > code_data_buf_ptr + code_data_buf_len)
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    (void)memscpy((void*)sp_code_data_ptr[list_cnt].code_data.sp_3gpp_code_list.sp_code_data[list_code_cnt].nw_code.mnc.mnc_data,
            SIMLOCK_MNC_MAX,
            current_code_ptr,
            sp_code_data_ptr[list_cnt].code_data.sp_3gpp_code_list.sp_code_data[list_code_cnt].nw_code.mnc.mnc_len);
    current_code_ptr += mnc_len;

    /* Check the data size */
    if(current_code_ptr + sizeof(uint8) > code_data_buf_ptr + code_data_buf_len)
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    if (extended_category == QMI_UIM_CATEGORY_TYPE_EXT2 ||
        extended_category == QMI_UIM_CATEGORY_TYPE_EXT)
    {
      uint8 max_gid_len = QMI_UIM_GID_MAX_V01;
      uint8 gid_len     = 0;

      if (extended_category == QMI_UIM_CATEGORY_TYPE_EXT2)
      {
        max_gid_len = QMI_UIM_GID_EXT_MAX_V01;
      }

      gid_len = *current_code_ptr;

      /* validate the gid_len */
      if(gid_len == 0)
      {
        ret_value = QMI_ERR_MALFORMED_MSG;
        break;
      }

      if(gid_len > max_gid_len)
      {
        ret_value = QMI_ERR_ARG_TOO_LONG;
        break;
      }

      sp_code_data_ptr[list_cnt].code_data.sp_3gpp_code_list.sp_code_data[list_code_cnt].gid1.gid_len = gid_len;
      current_code_ptr += sizeof(uint8);

      /* Check the data size */
      if(current_code_ptr + gid_len > code_data_buf_ptr + code_data_buf_len)
      {
        ret_value = QMI_ERR_MALFORMED_MSG;
        break;
      }

      (void)memscpy((void*)sp_code_data_ptr[list_cnt].code_data.sp_3gpp_code_list.sp_code_data[list_code_cnt].gid1.gid_data,
              sizeof(sp_code_data_ptr[list_cnt].code_data.sp_3gpp_code_list.sp_code_data[list_code_cnt].gid1.gid_data),
              current_code_ptr,
              gid_len);
      current_code_ptr += gid_len;
    }
    else
    {
      sp_code_data_ptr[list_cnt].code_data.sp_3gpp_code_list.sp_code_data[list_code_cnt].gid1.gid_data[0] =
        *current_code_ptr;
      sp_code_data_ptr[list_cnt].code_data.sp_3gpp_code_list.sp_code_data[list_code_cnt].gid1.gid_len = 1;

      current_code_ptr += sizeof(uint8);
    }

    sp_code_data_ptr[list_cnt].code_data.sp_3gpp_code_list.num_sp_codes++;
    list_code_cnt++;
  }

  if (ret_value == QMI_ERR_NONE &&
      current_code_ptr != code_data_buf_ptr + code_data_buf_len)
  {
    ret_value = QMI_ERR_MALFORMED_MSG;
  }

  return ret_value;
} /* qmi_uim_tlv_personalization_parse_sp_code*/


/*===========================================================================
  FUNCTION QMI_UIM_TLV_PERSONALIZATION_PARSE_SP_EHPLMN_CODE()

  DESCRIPTION
    Parses the SP + EHPLMN codes

  PARAMETERS
    num_codes               : number of SP codes
    code_data_buf_len       : length of raw code data
    code_data_buf_ptr       : pointer to raw code data
    num_lists               : number of lists needed for SP codes
    sp_ehplmn_code_data_ptr : output with SP codes

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_personalization_parse_sp_ehplmn_code
(
  uint8                              num_codes,
  uint16                             code_data_buf_len,
  const uint8                    *   code_data_buf_ptr,
  uint8                              num_lists,
  simlock_category_data_type     *   sp_ehplmn_code_data_ptr
)
{
  qmi_error_e_type   ret_value         = QMI_ERR_NONE;
  uint8              code_cnt          = 0;
  uint8              list_cnt          = 0;
  uint8              list_code_cnt     = 0;
  uint8              mnc_len           = 0;
  const uint8      * current_code_ptr  = NULL;
  uint8              gid_len           = 0;

  if ((code_data_buf_len == 0) ||
      (code_data_buf_ptr == NULL) ||
      (sp_ehplmn_code_data_ptr == NULL))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  current_code_ptr = code_data_buf_ptr;

  for(code_cnt = 0; code_cnt < num_codes; code_cnt++)
  {
    /* Check the data size */
    if(current_code_ptr + SIMLOCK_MCC_LEN + sizeof(mnc_len) >
        code_data_buf_ptr + code_data_buf_len)
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    /* Each list will fit, SIMLOCK_NUM_CODES_MAX. So if
       list_code_cnt is SIMLOCK_NUM_CODES_MAX, then fill
       the next list */
    if(list_code_cnt == SIMLOCK_NUM_CODES_MAX)
    {
      if(list_cnt < num_lists)
      {
        list_code_cnt = 0;
        list_cnt++;
      }
      else
      {
        ret_value = QMI_ERR_MALFORMED_MSG;
        break;
      }
    }

    if((list_cnt >= num_lists) ||
       (list_code_cnt >= SIMLOCK_NUM_CODES_MAX))
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    (void)memscpy((void*)sp_ehplmn_code_data_ptr[list_cnt].code_data.sp_ehplmn_3gpp_code_list.sp_ehplmn_code_data[list_code_cnt].nw_code.mcc,
                    SIMLOCK_MCC_LEN,
                    current_code_ptr,
                    SIMLOCK_MCC_LEN);
    current_code_ptr += SIMLOCK_MCC_LEN;

    mnc_len = *current_code_ptr;

    /* validate the mnc_len */
    if((mnc_len != QMI_UIM_SIMLOCK_MNC_MIN) &&
       (mnc_len != QMI_UIM_MNC_MAX_V01))
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    sp_ehplmn_code_data_ptr[list_cnt].code_data.sp_ehplmn_3gpp_code_list.sp_ehplmn_code_data[list_code_cnt].nw_code.mnc.mnc_len = mnc_len;
    current_code_ptr += sizeof(mnc_len);

    if(current_code_ptr + mnc_len > code_data_buf_ptr + code_data_buf_len)
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    (void)memscpy((void*)sp_ehplmn_code_data_ptr[list_cnt].code_data.sp_ehplmn_3gpp_code_list.sp_ehplmn_code_data[list_code_cnt].nw_code.mnc.mnc_data,
            SIMLOCK_MNC_MAX,
            current_code_ptr,
            sp_ehplmn_code_data_ptr[list_cnt].code_data.sp_ehplmn_3gpp_code_list.sp_ehplmn_code_data[list_code_cnt].nw_code.mnc.mnc_len);
    current_code_ptr += mnc_len;

    /* Check the data size */
    if(current_code_ptr + sizeof(uint8) > code_data_buf_ptr + code_data_buf_len)
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    /* Parse GID1 value */
    gid_len = *current_code_ptr;

    /* validate the gid_len */
    if(gid_len == 0)
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    if(gid_len > QMI_UIM_GID_EXT_MAX_V01)
    {
      ret_value = QMI_ERR_ARG_TOO_LONG;
      break;
    }

    sp_ehplmn_code_data_ptr[list_cnt].code_data.sp_3gpp_code_list.sp_code_data[list_code_cnt].gid1.gid_len = gid_len;
    current_code_ptr += sizeof(uint8);

    /* Check the data size */
    if(current_code_ptr + gid_len > code_data_buf_ptr + code_data_buf_len)
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    (void)memscpy((void*)sp_ehplmn_code_data_ptr[list_cnt].code_data.sp_ehplmn_3gpp_code_list.sp_ehplmn_code_data[list_code_cnt].gid1.gid_data,
            sizeof(sp_ehplmn_code_data_ptr[list_cnt].code_data.sp_ehplmn_3gpp_code_list.sp_ehplmn_code_data[list_code_cnt].gid1.gid_data),
            current_code_ptr,
            gid_len);
    current_code_ptr += gid_len;

    if(current_code_ptr + SIMLOCK_MCC_LEN + sizeof(mnc_len) >
        code_data_buf_ptr + code_data_buf_len)
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    (void)memscpy((void*)sp_ehplmn_code_data_ptr[list_cnt].code_data.sp_ehplmn_3gpp_code_list.sp_ehplmn_code_data[list_code_cnt].ehplmn_nw_code.mcc,
                    SIMLOCK_MCC_LEN,
                    current_code_ptr,
                    SIMLOCK_MCC_LEN);
    current_code_ptr += SIMLOCK_MCC_LEN;

    mnc_len = *current_code_ptr;

    /* validate the mnc_len */
    if((mnc_len != QMI_UIM_SIMLOCK_MNC_MIN) &&
       (mnc_len != QMI_UIM_MNC_MAX_V01))
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    sp_ehplmn_code_data_ptr[list_cnt].code_data.sp_ehplmn_3gpp_code_list.sp_ehplmn_code_data[list_code_cnt].ehplmn_nw_code.mnc.mnc_len = mnc_len;
    current_code_ptr += sizeof(mnc_len);

    if(current_code_ptr + mnc_len > code_data_buf_ptr + code_data_buf_len)
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    (void)memscpy((void*)sp_ehplmn_code_data_ptr[list_cnt].code_data.sp_ehplmn_3gpp_code_list.sp_ehplmn_code_data[list_code_cnt].ehplmn_nw_code.mnc.mnc_data,
            SIMLOCK_MNC_MAX,
            current_code_ptr,
            sp_ehplmn_code_data_ptr[list_cnt].code_data.sp_ehplmn_3gpp_code_list.sp_ehplmn_code_data[list_code_cnt].ehplmn_nw_code.mnc.mnc_len);
    current_code_ptr += mnc_len;

    sp_ehplmn_code_data_ptr[list_cnt].code_data.sp_ehplmn_3gpp_code_list.num_sp_ehplmn_codes++;
    list_code_cnt++;
  }

  if (ret_value == QMI_ERR_NONE &&
      current_code_ptr != code_data_buf_ptr + code_data_buf_len)
  {
    ret_value = QMI_ERR_MALFORMED_MSG;
  }

  return ret_value;
} /* qmi_uim_tlv_personalization_parse_sp_ehplmn_code*/


/*===========================================================================
  FUNCTION QMI_UIM_TLV_PERSONALIZATION_PARSE_NS_CODE()

  DESCRIPTION
    Parses the NS codes

  PARAMETERS
    num_codes           : number of NS codes
    code_data_buf_len   : length of raw code data
    code_data_buf_ptr   : pointer to raw code data
    num_lists           : number of lists needed for NS codes
    nw_code_data_ptr    : output with NS codes

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_personalization_parse_ns_code
(
  uint8                              num_codes,
  uint16                             code_data_buf_len,
  const uint8                    *   code_data_buf_ptr,
  uint8                              num_lists,
  simlock_category_data_type     *   ns_code_data_ptr
)
{
  qmi_error_e_type   ret_value         = QMI_ERR_NONE;
  uint8              code_cnt          = 0;
  uint8              list_cnt          = 0;
  uint8              list_code_cnt     = 0;
  uint8              mnc_len           = 0;
  const uint8      * current_code_ptr  = NULL;

  if ((code_data_buf_len == 0) ||
      (code_data_buf_ptr == NULL) ||
      (ns_code_data_ptr == NULL))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  current_code_ptr = code_data_buf_ptr;

  for(code_cnt = 0; code_cnt < num_codes; code_cnt++)
  {
    /* Check the data size */
    if(current_code_ptr + SIMLOCK_MCC_LEN + sizeof(mnc_len) >
        code_data_buf_ptr + code_data_buf_len)
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    /* Each list will fit, SIMLOCK_NUM_CODES_MAX. So if
       list_code_cnt is SIMLOCK_NUM_CODES_MAX, then fill
       the next list */
    if(list_code_cnt == SIMLOCK_NUM_CODES_MAX)
    {
      if(list_cnt < num_lists)
      {
        list_code_cnt = 0;
        list_cnt++;
      }
      else
      {
        ret_value = QMI_ERR_MALFORMED_MSG;
        break;
      }
    }

    if((list_cnt >= num_lists) ||
       (list_code_cnt >= SIMLOCK_NUM_CODES_MAX))
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    (void)memscpy((void*)ns_code_data_ptr[list_cnt].code_data.ns_3gpp_code_list.ns_code_data[list_code_cnt].nw_code.mcc,
              SIMLOCK_MCC_LEN,
              current_code_ptr,
              SIMLOCK_MCC_LEN);
    current_code_ptr += SIMLOCK_MCC_LEN;

    mnc_len = *current_code_ptr;

    /* validate the mnc_len */
    if((mnc_len != QMI_UIM_SIMLOCK_MNC_MIN) &&
       (mnc_len != QMI_UIM_MNC_MAX_V01))
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    ns_code_data_ptr[list_cnt].code_data.ns_3gpp_code_list.ns_code_data[list_code_cnt].nw_code.mnc.mnc_len = mnc_len;
    current_code_ptr += sizeof(mnc_len);

    if(current_code_ptr + mnc_len > code_data_buf_ptr + code_data_buf_len)
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    (void)memscpy((void*)ns_code_data_ptr[list_cnt].code_data.ns_3gpp_code_list.ns_code_data[list_code_cnt].nw_code.mnc.mnc_data,
            SIMLOCK_MNC_MAX,
            current_code_ptr,
            ns_code_data_ptr[list_cnt].code_data.ns_3gpp_code_list.ns_code_data[list_code_cnt].nw_code.mnc.mnc_len);
    current_code_ptr += mnc_len;

    if(current_code_ptr + sizeof(uint8) + sizeof(uint8) >
         code_data_buf_ptr + code_data_buf_len)
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    ns_code_data_ptr[list_cnt].code_data.ns_3gpp_code_list.ns_code_data[list_code_cnt].imsi_digit6 = *current_code_ptr;
    current_code_ptr += sizeof(uint8);

    ns_code_data_ptr[list_cnt].code_data.ns_3gpp_code_list.ns_code_data[list_code_cnt].imsi_digit7 = *current_code_ptr;
    current_code_ptr += sizeof(uint8);

    ns_code_data_ptr[list_cnt].code_data.ns_3gpp_code_list.num_ns_codes++;
    list_code_cnt++;
  }

  if (ret_value == QMI_ERR_NONE &&
      current_code_ptr != code_data_buf_ptr + code_data_buf_len)
  {
    ret_value = QMI_ERR_MALFORMED_MSG;
  }

  return ret_value;
} /* qmi_uim_tlv_personalization_parse_ns_code */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_PERSONALIZATION_PARSE_NW_CODE()

  DESCRIPTION
    Parses the NW codes

  PARAMETERS
    num_codes           : number of NW codes
    code_data_buf_len   : length of raw code data
    code_data_buf_ptr   : pointer to raw code data
    num_lists           : number of lists needed for NW codes
    nw_code_data_ptr    : output with NW codes

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_personalization_parse_nw_code
(
  uint8                              num_codes,
  uint16                             code_data_buf_len,
  const uint8                    *   code_data_buf_ptr,
  uint8                              num_lists,
  simlock_category_data_type     *   nw_code_data_ptr
)
{
  qmi_error_e_type   ret_value         = QMI_ERR_NONE;
  uint8              code_cnt          = 0;
  uint8              list_cnt          = 0;
  uint8              list_code_cnt     = 0;
  uint8              mnc_len           = 0;
  const uint8      * current_code_ptr  = NULL;

  if ((code_data_buf_len == 0) ||
      (code_data_buf_ptr == NULL) ||
      (nw_code_data_ptr == NULL))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  current_code_ptr = code_data_buf_ptr;

  for(code_cnt = 0; code_cnt < num_codes; code_cnt++)
  {
    /* Check the data size */
    if(current_code_ptr + SIMLOCK_MCC_LEN + sizeof(mnc_len) >
        code_data_buf_ptr + code_data_buf_len)
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    /* Each list will fit, SIMLOCK_NUM_CODES_MAX. So if
       list_code_cnt is SIMLOCK_NUM_CODES_MAX, then fill
       the next list */
    if(list_code_cnt == SIMLOCK_NUM_CODES_MAX)
    {
      if(list_cnt < num_lists)
      {
        list_code_cnt = 0;
        list_cnt++;
      }
      else
      {
        ret_value = QMI_ERR_MALFORMED_MSG;
        break;
      }
    }

    if((list_cnt >= num_lists) ||
       (list_code_cnt >= SIMLOCK_NUM_CODES_MAX))
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    (void)memscpy((void*)nw_code_data_ptr[list_cnt].code_data.nw_3gpp_code_list.nw_code_data[list_code_cnt].mcc,
                    SIMLOCK_MCC_LEN,
                    current_code_ptr,
                    SIMLOCK_MCC_LEN);
    current_code_ptr += SIMLOCK_MCC_LEN;

    mnc_len = *current_code_ptr;

    /* validate the mnc_len */
    if((mnc_len != QMI_UIM_SIMLOCK_MNC_MIN) &&
       (mnc_len != QMI_UIM_MNC_MAX_V01))
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    nw_code_data_ptr[list_cnt].code_data.nw_3gpp_code_list.nw_code_data[list_code_cnt].mnc.mnc_len = mnc_len;
    current_code_ptr += sizeof(mnc_len);

    /* Check the data size */
    if(current_code_ptr + mnc_len > code_data_buf_ptr + code_data_buf_len)
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    (void)memscpy((void*)nw_code_data_ptr[list_cnt].code_data.nw_3gpp_code_list.nw_code_data[list_code_cnt].mnc.mnc_data,
            SIMLOCK_MNC_MAX,
            current_code_ptr,
            nw_code_data_ptr[list_cnt].code_data.nw_3gpp_code_list.nw_code_data[list_code_cnt].mnc.mnc_len);
    current_code_ptr += mnc_len;

    nw_code_data_ptr[list_cnt].code_data.nw_3gpp_code_list.num_nw_codes++;
    list_code_cnt++;
  }

  if (ret_value == QMI_ERR_NONE &&
      current_code_ptr != code_data_buf_ptr + code_data_buf_len)
  {
    ret_value = QMI_ERR_MALFORMED_MSG;
  }

  return ret_value;
} /* qmi_uim_tlv_personalization_parse_nw_code */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_PERSONALIZATION_CK()

  DESCRIPTION
    Parses the CK TLV item of activate perso

  PARAMETERS
    sdu_in                : input sdu
    len                   : length of the TLV item
    personalization_ptr   : output with activate perso value

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_tlv_personalization_ck
(
  dsm_item_type **                        sdu_in,
  uint16                                  len,
  qmi_uim_tlv_personalization_ck_type   * personalization_ck_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint8  ck_len;
    uint8  ck_value[QMI_UIM_CK_MAX_V01];
  } tlv_data;

  ASSERT(sdu_in && personalization_ck_ptr);

  /* Check length */
  if (len < sizeof(tlv_data.ck_len))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  if (len > sizeof(tlv_data))
  {
    return QMI_ERR_ARG_TOO_LONG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, len);
  if (temp != len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  if (tlv_data.ck_len == 0 ||
      tlv_data.ck_len > QMI_UIM_CK_MAX_V01)
  {
    return QMI_ERR_ARG_TOO_LONG;
  }

  /* Check data length */
  if (len != sizeof(tlv_data.ck_len) + tlv_data.ck_len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Control key length */
  personalization_ck_ptr->ck.simlock_ck_length =
    tlv_data.ck_len;

  /* Control key value */
  (void)memscpy((void*)personalization_ck_ptr->ck.simlock_ck_data,
                sizeof(personalization_ck_ptr->ck.simlock_ck_data),
                (void*)tlv_data.ck_value,
                (uint32)personalization_ck_ptr->ck.simlock_ck_length);

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_personalization_ck */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_PERSONALIZATION_PARSE_CODES()

  DESCRIPTION
    Parses the personalization code

  PARAMETERS
    num_codes           : number of codes
    code_data_buf_len   : length of raw code data
    code_data_buf_ptr   : pointer to raw code data
    list_cnt            : number of lists needed for the codes
    extended_category   : type of category. normal, ext or ext2
    code_data_ptr       : output with parsed codes

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_personalization_parse_codes
(
  uint8                                    num_codes,
  uint16                                   code_data_buf_len,
  const uint8                            * code_data_buf_ptr,
  uint8                                    num_lists,
  qmi_uim_simlock_extension_category_type  extended_category,
  simlock_category_data_type             * code_data_ptr
)
{
  qmi_error_e_type  ret_value         = QMI_ERR_NONE;

  if ((num_codes == 0) ||
      (code_data_buf_len == 0) ||
      (code_data_buf_ptr == NULL) ||
      (num_lists == 0) ||
      (code_data_ptr == NULL))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  switch(code_data_ptr->category_type)
  {
    case SIMLOCK_CATEGORY_3GPP_NW:
      ret_value = qmi_uim_tlv_personalization_parse_nw_code(num_codes,
                                                            code_data_buf_len,
                                                            code_data_buf_ptr,
                                                            num_lists,
                                                            code_data_ptr);
      break;

    case SIMLOCK_CATEGORY_3GPP_NS:
      ret_value = qmi_uim_tlv_personalization_parse_ns_code(num_codes,
                                                            code_data_buf_len,
                                                            code_data_buf_ptr,
                                                            num_lists,
                                                            code_data_ptr);
      break;

    case SIMLOCK_CATEGORY_3GPP_SP:
      ret_value = qmi_uim_tlv_personalization_parse_sp_code(num_codes,
                                                            code_data_buf_len,
                                                            code_data_buf_ptr,
                                                            num_lists,
                                                            extended_category,
                                                            code_data_ptr);
      break;

    case SIMLOCK_CATEGORY_3GPP_CP:
      ret_value = qmi_uim_tlv_personalization_parse_cp_code(num_codes,
                                                            code_data_buf_len,
                                                            code_data_buf_ptr,
                                                            num_lists,
                                                            extended_category,
                                                            code_data_ptr);
      break;

    case SIMLOCK_CATEGORY_3GPP_SIM:
      ret_value = qmi_uim_tlv_personalization_parse_sim_code(num_codes,
                                                             code_data_buf_len,
                                                             code_data_buf_ptr,
                                                             num_lists,
                                                             code_data_ptr);
      break;

    case SIMLOCK_CATEGORY_3GPP_SPN:
      ret_value = qmi_uim_tlv_personalization_parse_spn_code(num_codes,
                                                             code_data_buf_len,
                                                             code_data_buf_ptr,
                                                             num_lists,
                                                             code_data_ptr);
      break;

    case SIMLOCK_CATEGORY_3GPP_SP_EHPLMN:
      ret_value = qmi_uim_tlv_personalization_parse_sp_ehplmn_code(num_codes,
                                                                   code_data_buf_len,
                                                                   code_data_buf_ptr,
                                                                   num_lists,
                                                                   code_data_ptr);
      break;

    case SIMLOCK_CATEGORY_3GPP_ICCID:
      ret_value = qmi_uim_tlv_personalization_parse_iccid_code(num_codes,
                                                               code_data_buf_len,
                                                               code_data_buf_ptr,
                                                               num_lists,
                                                               code_data_ptr);
      break;

    case SIMLOCK_CATEGORY_3GPP2_NW_TYPE1:
      ret_value = qmi_uim_tlv_personalization_parse_nw_type1_code(num_codes,
                                                                  code_data_buf_len,
                                                                  code_data_buf_ptr,
                                                                  num_lists,
                                                                  code_data_ptr);
      break;

    case SIMLOCK_CATEGORY_3GPP2_NW_TYPE2:
      ret_value = qmi_uim_tlv_personalization_parse_nw_type2_code(num_codes,
                                                                  code_data_buf_len,
                                                                  code_data_buf_ptr,
                                                                  num_lists,
                                                                  code_data_ptr);
      break;

    case SIMLOCK_CATEGORY_3GPP2_RUIM:
      ret_value = qmi_uim_tlv_personalization_parse_ruim_code(num_codes,
                                                              code_data_buf_len,
                                                              code_data_buf_ptr,
                                                              num_lists,
                                                              code_data_ptr);
      break;

    case SIMLOCK_CATEGORY_3GPP2_HRPD:
    case SIMLOCK_CATEGORY_3GPP2_SP:
    case SIMLOCK_CATEGORY_3GPP2_CP:
      ret_value = QMI_ERR_NOT_SUPPORTED;
      break;

    default:
      ret_value = QMI_ERR_INVALID_ARG;
      break;
  }

  return ret_value;
} /* qmi_uim_tlv_personalization_parse_codes */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_PERSONALIZATION_CODE()

  DESCRIPTION
    Parses the lock code TLV item of activate perso

  PARAMETERS
    sdu_in                : input sdu
    len                   : length of the TLV item
    category              : simlock category
    extended_category     : type of category. normal, ext or ext2
    personalization_ptr   : output with activate perso value

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_tlv_personalization_code
(
  dsm_item_type **                               sdu_in,
  uint16                                         len,
  simlock_category_enum_type                     category,
  qmi_uim_simlock_extension_category_type        extended_category,
  qmi_uim_tlv_personalization_code_type        * personalization_code_ptr
)
{
  qmi_error_e_type  ret_value         = QMI_ERR_NONE;
  int32             temp              = 0;
  uint16            code_data_size    = 0;
  uint8             table_size        = 0;
  uint8             table_index       = 0;
  uint8             list_cnt          = 0;
  uint8           * code_data_ptr     = NULL;

  /*---------------------------------------------------------------------------
    Table with map between lock category and min/max code sizes. Extended flag
    value "QMI_UIM_CATEGORY_TYPE_EXT" denotes support of multi byte GID
    whereas extended flag value "QMI_UIM_CATEGORY_TYPE_EXT_2" denotes 8 bytes
    GID support for SP and CP category So minimum length for code is more than
    that of legacy category.
  ---------------------------------------------------------------------------*/
  const qmi_uim_category_code_size_map_type qmi_uim_simlock_category_table[] =
  {
    {SIMLOCK_CATEGORY_3GPP_NW,         QMI_UIM_PERSO_NUM_NW_MAX_V01,        QMI_UIM_TLV_NW_CODE_MIN_LEN,       QMI_UIM_CATEGORY_TYPE_NO_EXT},
    {SIMLOCK_CATEGORY_3GPP_NS,         QMI_UIM_PERSO_NUM_NS_MAX_V01,        QMI_UIM_TLV_NS_CODE_MIN_LEN,       QMI_UIM_CATEGORY_TYPE_NO_EXT},
    {SIMLOCK_CATEGORY_3GPP_SP,         QMI_UIM_PERSO_NUM_GW_SP_MAX_V01,     QMI_UIM_TLV_SP_CODE_MIN_LEN,       QMI_UIM_CATEGORY_TYPE_NO_EXT},
    {SIMLOCK_CATEGORY_3GPP_CP,         QMI_UIM_PERSO_NUM_GW_CP_MAX_V01,     QMI_UIM_TLV_CP_CODE_MIN_LEN,       QMI_UIM_CATEGORY_TYPE_NO_EXT},
    {SIMLOCK_CATEGORY_3GPP_SIM,        QMI_UIM_PERSO_NUM_SIM_MAX_V01,       QMI_UIM_TLV_SIM_CODE_MIN_LEN,      QMI_UIM_CATEGORY_TYPE_NO_EXT},
    {SIMLOCK_CATEGORY_3GPP2_NW_TYPE1,  QMI_UIM_PERSO_NUM_NW_MAX_V01,        QMI_UIM_TLV_NW_CODE_MIN_LEN,       QMI_UIM_CATEGORY_TYPE_NO_EXT},
    {SIMLOCK_CATEGORY_3GPP2_NW_TYPE2,  QMI_UIM_PERSO_NUM_1X_NW2_MAX_V01,    QMI_UIM_TLV_NW_TYPE2_CODE_MIN_LEN, QMI_UIM_CATEGORY_TYPE_NO_EXT},
    {SIMLOCK_CATEGORY_3GPP2_RUIM,      QMI_UIM_PERSO_NUM_SIM_MAX_V01,       QMI_UIM_TLV_SIM_CODE_MIN_LEN,      QMI_UIM_CATEGORY_TYPE_NO_EXT},
    {SIMLOCK_CATEGORY_3GPP_SP,         QMI_UIM_PERSO_NUM_GW_SP_MAX_V01,     QMI_UIM_TLV_SP_EXT_CODE_MIN_LEN,   QMI_UIM_CATEGORY_TYPE_EXT},
    {SIMLOCK_CATEGORY_3GPP_CP,         QMI_UIM_PERSO_NUM_GW_CP_MAX_V01,     QMI_UIM_TLV_CP_EXT_CODE_MIN_LEN,   QMI_UIM_CATEGORY_TYPE_EXT},
    {SIMLOCK_CATEGORY_3GPP_SPN,        QMI_UIM_PERSO_NUM_GW_SPN_MAX_V01,    QMI_UIM_TLV_SPN_CODE_MIN_LEN,      QMI_UIM_CATEGORY_TYPE_NO_EXT},
    {SIMLOCK_CATEGORY_3GPP_SP,         QMI_UIM_PERSO_NUM_GW_SP_MAX_V01,     QMI_UIM_TLV_SP_EXT_CODE_MIN_LEN,   QMI_UIM_CATEGORY_TYPE_EXT2},
    {SIMLOCK_CATEGORY_3GPP_CP,         QMI_UIM_PERSO_NUM_GW_CP_MAX_V01,     QMI_UIM_TLV_CP_EXT_CODE_MIN_LEN,   QMI_UIM_CATEGORY_TYPE_EXT2},
    {SIMLOCK_CATEGORY_3GPP_SP_EHPLMN,  QMI_UIM_PERSO_NUM_GW_SP_MAX_V01,     QMI_UIM_TLV_SP_EHPLMN_MIN_LEN,     QMI_UIM_CATEGORY_TYPE_NO_EXT},
    {SIMLOCK_CATEGORY_3GPP_ICCID,      QMI_UIM_PERSO_NUM_GW_ICCID_MAX_V01,  QMI_UIM_TLV_ICCID_CODE_MIN_LEN,    QMI_UIM_CATEGORY_TYPE_NO_EXT}
  };

  PACKED struct PACKED_POST
  {
    uint8  num_of_codes;
  } tlv_data;

  ASSERT(sdu_in && personalization_code_ptr);

  /* Check length */
  if (len < sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, sizeof(tlv_data));
  if (temp != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  table_size = sizeof(qmi_uim_simlock_category_table)/sizeof(qmi_uim_category_code_size_map_type);

  for (table_index = 0; table_index < table_size; table_index++)
  {
    if (category != qmi_uim_simlock_category_table[table_index].category ||
        extended_category != qmi_uim_simlock_category_table[table_index].extended_category)
    {
      continue;
    }

    /* Check the number of codes */
    if (tlv_data.num_of_codes == 0 ||
        tlv_data.num_of_codes > qmi_uim_simlock_category_table[table_index].num_codes_max)
    {
      return QMI_ERR_ARG_TOO_LONG;
    }

    /* Check data length */
    if( len < (sizeof(tlv_data) +
               (tlv_data.num_of_codes * qmi_uim_simlock_category_table[table_index].code_data_min)) )
    {
      return QMI_ERR_MALFORMED_MSG;
    }
    break;
  }

  if (table_index == table_size)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  code_data_size = len - sizeof(tlv_data);

  code_data_ptr = (uint8 *)uimqmi_malloc(code_data_size);
  if(code_data_ptr == NULL)
  {
    return QMI_ERR_NO_MEMORY;
  }

  /* Extract the code data */
  temp = dsm_pullup(sdu_in, (void*)code_data_ptr, code_data_size);
  if (temp != code_data_size)
  {
    uimqmi_free(code_data_ptr);
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Get the number of lists needed to fit all the codes */
  personalization_code_ptr->num_of_lists = QMI_UIM_PERSO_GET_LIST_COUNT(tlv_data.num_of_codes);
  if(personalization_code_ptr->num_of_lists == 0)
  {
    uimqmi_free(code_data_ptr);
    return QMI_ERR_MALFORMED_MSG;
  }

  personalization_code_ptr->code_ptr =
                (simlock_category_data_type *)uimqmi_malloc(personalization_code_ptr->num_of_lists *
                                                            sizeof(simlock_category_data_type));
  if(personalization_code_ptr->code_ptr == NULL)
  {
    uimqmi_free(code_data_ptr);
    return QMI_ERR_NO_MEMORY;
  }

  for(list_cnt = 0; list_cnt < personalization_code_ptr->num_of_lists;
      list_cnt++)
  {
    personalization_code_ptr->code_ptr[list_cnt].category_type = category;
  }

  ret_value = qmi_uim_tlv_personalization_parse_codes(tlv_data.num_of_codes,
                                                      code_data_size,
                                                      code_data_ptr,
                                                      personalization_code_ptr->num_of_lists,
                                                      extended_category,
                                                      personalization_code_ptr->code_ptr);

  uimqmi_free(code_data_ptr);
  return ret_value;
} /* qmi_uim_tlv_personalization_code */


/*===========================================================================
  FUNCTION QMI_UIM_DECRYPT_SLOT_TO_SIMLOCK_SLOT_ID()

  DESCRIPTION
    Converts decrypted slot to SimLock slot id.

  PARAMETERS
    decrypt_slot       : decrypt slot value
    simlock_slot_ptr   : output parameter containing SimLock slot id

  RETURN VALUE
    QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_decrypt_slot_to_simlock_slot_id
(
  uint8                       decrypt_slot,
  simlock_slot_enum_type    * simlock_slot_ptr
)
{
  ASSERT(simlock_slot_ptr);

  switch (decrypt_slot)
  {
    case 1:
      *simlock_slot_ptr = SIMLOCK_SLOT_1;
      break;
    case 2:
      *simlock_slot_ptr = SIMLOCK_SLOT_2;
      break;
    case 3:
      *simlock_slot_ptr = SIMLOCK_SLOT_3;
      break;
    default:
      *simlock_slot_ptr = SIMLOCK_SLOT_NUM;
      return QMI_ERR_INVALID_ARG;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_decrypt_slot_to_simlock_slot_id */


/*===========================================================================
  FUNCTION QMI_UIM_PARSE_SECURE_MESSAGE_HEADER_DATA()

  DESCRIPTION
    This function parses the encrypted header information

  PARAMETERS
    data_len         : raw header length
    data_ptr         : raw header data
    header_data_ptr  : parsed header data

  RETURN VALUE
    qmi_error_e_type

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_parse_secure_message_header_data
(
  uint32                                  data_len,
  const uint8                         *   data_ptr,
  qmi_uim_secure_header_type          *   header_data_ptr
)
{
  PACKED struct PACKED_POST
  {
    uint16 cmd_id;
    uint16 request_id;
    uint8  rfu[QMI_UIM_ENCRYPTED_MSG_RFU_SIZE];
  } raw_data;

  if((data_ptr == NULL) ||
     (header_data_ptr == NULL))
  {
     return QMI_ERR_INTERNAL;
  }

  if(data_len < sizeof(raw_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* copy the request header data to packed struct to be parsed */
  (void)memscpy(&raw_data,
                sizeof(raw_data),
                data_ptr,
                sizeof(raw_data));

  header_data_ptr->cmd_id = raw_data.cmd_id;
  header_data_ptr->request_id = raw_data.request_id;

  return QMI_ERR_NONE;
} /* qmi_uim_parse_secure_message_header_data */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_DECRYPT_REQUEST()

  DESCRIPTION
    Generic function that decrypts the encrypted request data

  PARAMETERS
    sdu_in                    : input sdu
    len                       : length of the TLV item
    decoded_data_len_ptr      : output decoded data len
    decoded_data_pptr         : output decoded data

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_decrypt_request
(
  dsm_item_type **         sdu_in,
  uint16                   len,
  uint32        *          decoded_data_len_ptr,
  uint8         **         decoded_data_pptr
)
{
  uint8                  * encrypted_data_ptr   = NULL;
  qmi_error_e_type         errval               = QMI_ERR_NONE;
  int32                    temp                 = 0;

  PACKED struct PACKED_POST
  {
    uint16  length;
  } tlv_data;

  ASSERT(sdu_in && decoded_data_pptr && decoded_data_len_ptr);

  /* Check length */
  if (len < sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data and ensure it is formed properly */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, sizeof(tlv_data));
  if (temp != sizeof(tlv_data.length))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  if (tlv_data.length == 0 ||
      len != (sizeof(tlv_data.length) + tlv_data.length))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  encrypted_data_ptr = (uint8 *)uimqmi_malloc(tlv_data.length);
  if(encrypted_data_ptr == NULL)
  {
    return QMI_ERR_NO_MEMORY;
  }

  /* Extract the code data */
  temp = dsm_pullup(sdu_in, (void*)encrypted_data_ptr, tlv_data.length);
  if (temp != tlv_data.length)
  {
    uimqmi_free(encrypted_data_ptr);
    return QMI_ERR_MALFORMED_MSG;
  }

  *decoded_data_len_ptr = tlv_data.length;

  /* caller is responsible to free this memory */
  *decoded_data_pptr = (uint8 *)uimqmi_malloc(tlv_data.length);
  if(*decoded_data_pptr == NULL)
  {
    uimqmi_free(encrypted_data_ptr);
    return QMI_ERR_NO_MEMORY;
  }

#ifndef FEATURE_QMI_UIM_SECURE_CHANNEL_TEST
  if (!qmi_uim_decrypt_payload(encrypted_data_ptr,
                               tlv_data.length,
                               *decoded_data_pptr,
                               decoded_data_len_ptr))
  {
    uimqmi_free(*decoded_data_pptr);
    *decoded_data_pptr = NULL;
    errval = QMI_ERR_AUTHENTICATION_FAILED;
  }
#else
  if (tlv_data.length > *decoded_data_len_ptr)
  {
    return QMI_ERR_INTERNAL;
  }

  (void)memscpy(*decoded_data_pptr,
                *decoded_data_len_ptr,
                encrypted_data_ptr,
                tlv_data.length);
#endif /* FEATURE_QMI_UIM_SECURE_CHANNEL_TEST */

  uimqmi_free(encrypted_data_ptr);
  return errval;
} /* qmi_uim_tlv_decrypt_request */


/*===========================================================================
  FUNCTION QMI_UIM_CONVERT_CATEGORY_TO_SIMLOCK_CATEGORY()

  DESCRIPTION
    Converts the incoming category to SimLock category.

  PARAMETERS
    convert_category       : category value to be converted
    simlock_category_ptr   : output parameter containing SimLock category

  RETURN VALUE
    QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_convert_category_to_simlock_category
(
  uint8                         convert_category,
  simlock_category_enum_type  * simlock_category_ptr
)
{
  ASSERT(simlock_category_ptr);

  switch (convert_category)
  {
    case 0:
      *simlock_category_ptr = SIMLOCK_CATEGORY_3GPP_NW;
      break;
    case 1:
      *simlock_category_ptr = SIMLOCK_CATEGORY_3GPP_NS;
      break;
    case 2:
      *simlock_category_ptr = SIMLOCK_CATEGORY_3GPP_SP;
      break;
    case 3:
      *simlock_category_ptr = SIMLOCK_CATEGORY_3GPP_CP;
      break;
    case 4:
      *simlock_category_ptr = SIMLOCK_CATEGORY_3GPP_SIM;
      break;
    case 5:
      *simlock_category_ptr = SIMLOCK_CATEGORY_3GPP2_NW_TYPE1;
      break;
    case 6:
      *simlock_category_ptr = SIMLOCK_CATEGORY_3GPP2_NW_TYPE2;
      break;
    case 7:
    case 8:
    case 9:
      return QMI_ERR_NOT_SUPPORTED;
    case 10:
      *simlock_category_ptr = SIMLOCK_CATEGORY_3GPP2_RUIM;
      break;
    case 11:
      *simlock_category_ptr = SIMLOCK_CATEGORY_3GPP_SPN;
      break;
    case 12:
      *simlock_category_ptr = SIMLOCK_CATEGORY_3GPP_SP_EHPLMN;
      break;
    case 13:
      *simlock_category_ptr = SIMLOCK_CATEGORY_3GPP_ICCID;
      break;
    case 14:
      *simlock_category_ptr = SIMLOCK_CATEGORY_3GPP_NS_SP;
      break;
    default:
      return QMI_ERR_INVALID_ARG;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_convert_category_to_simlock_category */


/*===========================================================================
  FUNCTION QMI_UIM_DECRYPT_OPERATION_TO_SIMLOCK_OPERATION()

  DESCRIPTION
    Converts decrypted operation to SimLock operation.

  PARAMETERS
    decrypt_operation       : decrypt operation value
    simlock_operation_ptr   : output parameter containing SimLock operation

  RETURN VALUE
    QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_decrypt_operation_to_simlock_operation
(
  uint8                                    decrypt_operation,
  qmi_uim_deperso_secure_operation_type  * simlock_operation_ptr
)
{
  ASSERT(simlock_operation_ptr);

  switch (decrypt_operation)
  {
    case 0:
      *simlock_operation_ptr = QMI_UIM_DEPERSO_UNLOCK_WITHOUT_CK;
      break;
    case 1:
      *simlock_operation_ptr = QMI_UIM_DEPERSO_TEMP_UNLOCK_ENABLE;
      break;
    case 2:
      *simlock_operation_ptr = QMI_UIM_DEPERSO_TEMP_UNLOCK_DISABLE;
      break;
    default:
      return QMI_ERR_INVALID_ARG;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_decrypt_operation_to_simlock_operation */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_DECRYPTED_PERSO_SECURE()

  DESCRIPTION
    Parses the TLV item for encrypted perso request

  PARAMETERS
    sdu_in                    : input sdu
    len                       : length of the TLV item
    decoded_perso_ptr         : output decoded perso secure data

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_tlv_decrypted_perso_secure
(
  dsm_item_type **                           sdu_in,
  uint16                                     len,
  qmi_uim_tlv_decrypted_perso_secure_type *  decoded_perso_ptr
)
{
  uint8                               * decrypted_perso_data_ptr   = NULL;
  uint16                                decrypted_perso_data_len   = 0;
  qmi_error_e_type                      errval                     = QMI_ERR_NONE;
  uint32                                temp_perso_len             = 0;
  uint8                                 list_cnt                   = 0;
  simlock_category_enum_type            category                   = SIMLOCK_CATEGORY_3GPP_NW;

  ASSERT(sdu_in && decoded_perso_ptr);

  memset(decoded_perso_ptr, 0x00, sizeof(qmi_uim_tlv_decrypted_perso_secure_type));

  /* decrypt the request data */
  errval = qmi_uim_tlv_decrypt_request(sdu_in, len,
                                       &temp_perso_len,
                                       &decrypted_perso_data_ptr);
  if((errval != QMI_ERR_NONE) ||
     (decrypted_perso_data_ptr == NULL))
  {
    UIM_MSG_ERR_0("Failed to decrypt request data");
    return errval;
  }

  do
  {
    if (temp_perso_len > QMI_UIM_MAX_UINT16)
    {
      UIM_MSG_ERR_1("Invalid perso data length: 0x%x", temp_perso_len);
      errval = QMI_ERR_INTERNAL;
      break;
    }

    decrypted_perso_data_len = (uint16)temp_perso_len;

    /* Check the minimum perso length with 15 to incorporate 1x nw type2 perso */
    if(decrypted_perso_data_len < QMI_UIM_DECRYPTED_PERSO_SECURE_REQ_MIN_LEN)
    {
      UIM_MSG_ERR_1("QMI-UIM: decrypted data len: 0x%x, less than expected",
                    decrypted_perso_data_len);
      errval = QMI_ERR_MALFORMED_MSG;
      break;
    }

    /* parse the header data */
    errval = qmi_uim_parse_secure_message_header_data(decrypted_perso_data_len,
                                                      decrypted_perso_data_ptr,
                                                      &decoded_perso_ptr->header);
    if(errval != QMI_ERR_NONE)
    {
      break;
    }

    errval = qmi_uim_decrypt_slot_to_simlock_slot_id(
                  decrypted_perso_data_ptr[QMI_UIM_DECRYPT_PERSO_SECURE_SLOT_OFFSET],
                  &decoded_perso_ptr->slot);
    if(errval != QMI_ERR_NONE)
    {
      break;
    }

    /* Get the number of lists needed to fit all the codes */
    decoded_perso_ptr->num_of_lists =
                  QMI_UIM_PERSO_GET_LIST_COUNT(decrypted_perso_data_ptr[QMI_UIM_DECRYPT_PERSO_SECURE_NUM_CODES_OFFSET]);
    if(decoded_perso_ptr->num_of_lists == 0)
    {
      errval = QMI_ERR_MALFORMED_MSG;
      break;
    }

    decoded_perso_ptr->code_ptr =
      (simlock_category_data_type*)uimqmi_malloc(decoded_perso_ptr->num_of_lists * sizeof(simlock_category_data_type));
    if(decoded_perso_ptr->code_ptr == NULL)
    {
      errval = QMI_ERR_NO_MEMORY;
      break;
    }

    errval = qmi_uim_convert_category_to_simlock_category(
                  decrypted_perso_data_ptr[QMI_UIM_DECRYPT_PERSO_SECURE_CATEGORY_OFFSET],
                  &category);
    if(errval != QMI_ERR_NONE)
    {
      break;
    }

    for(list_cnt = 0; list_cnt < decoded_perso_ptr->num_of_lists;
        list_cnt++)
    {
      decoded_perso_ptr->code_ptr[list_cnt].category_type = category;
    }

    errval = qmi_uim_tlv_personalization_parse_codes(decrypted_perso_data_ptr[QMI_UIM_DECRYPT_PERSO_SECURE_NUM_CODES_OFFSET],
                                                     decrypted_perso_data_len - QMI_UIM_DECRYPT_PERSO_CODE_DATA_OFFSET,
                                                     &decrypted_perso_data_ptr[QMI_UIM_DECRYPT_PERSO_CODE_DATA_OFFSET],
                                                     decoded_perso_ptr->num_of_lists,
                                                     QMI_UIM_CATEGORY_TYPE_NO_EXT,
                                                     decoded_perso_ptr->code_ptr);
  }
  while (0);

  uimqmi_free(decrypted_perso_data_ptr);

  return errval;
} /* qmi_uim_tlv_decrypted_perso_secure */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_DECRYPTED_DEPERSO_SECURE()

  DESCRIPTION
    Parses the TLV item for encrypted deperso request

  PARAMETERS
    sdu_in                    : input sdu
    len                       : length of the TLV item
    decoded_perso_ptr         : output decoded perso secure data

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_tlv_decrypted_deperso_secure
(
  dsm_item_type **                             sdu_in,
  uint16                                       len,
  qmi_uim_tlv_decrypted_deperso_secure_type *  decoded_deperso_ptr
)
{
  uint8                               * decrypted_deperso_data_ptr   = NULL;
  uint32                                decrypted_deperso_data_len   = 0;
  qmi_error_e_type                      errval                       = QMI_ERR_NONE;

  ASSERT(sdu_in && decoded_deperso_ptr);

  memset(decoded_deperso_ptr, 0x00, sizeof(qmi_uim_tlv_decrypted_deperso_secure_type));

  /* decrypt the request data */
  errval = qmi_uim_tlv_decrypt_request(sdu_in, len,
                                       &decrypted_deperso_data_len,
                                       &decrypted_deperso_data_ptr);
  if((errval != QMI_ERR_NONE) ||
     (decrypted_deperso_data_ptr == NULL))
  {
    UIM_MSG_ERR_0("Failed to decrypt request data");
    return errval;
  }

  do
  {
    /* check the length */
    if(decrypted_deperso_data_len < QMI_UIM_DECRYPTED_DEPERSO_SECURE_REQ_LEN)
    {
      UIM_MSG_ERR_1("QMI-UIM: decrypted data len: 0x%x, less than expected",
                    decrypted_deperso_data_len);
      errval = QMI_ERR_MALFORMED_MSG;
      break;
    }

    /* parse the header data */
    errval = qmi_uim_parse_secure_message_header_data(decrypted_deperso_data_len,
                                                      decrypted_deperso_data_ptr,
                                                      &decoded_deperso_ptr->header);
    if(errval != QMI_ERR_NONE)
    {
      break;
    }

    errval = qmi_uim_decrypt_slot_to_simlock_slot_id(
                  decrypted_deperso_data_ptr[QMI_UIM_DECRYPT_DEPERSO_SECURE_SLOT_OFFSET],
                  &decoded_deperso_ptr->slot);
    if(errval != QMI_ERR_NONE)
    {
      break;
    }

    errval = qmi_uim_convert_category_to_simlock_category(
                  decrypted_deperso_data_ptr[QMI_UIM_DECRYPT_DEPERSO_SECURE_CATEGORY_OFFSET],
                  &decoded_deperso_ptr->category_type);
    if(errval != QMI_ERR_NONE)
    {
      break;
    }

    errval = qmi_uim_decrypt_operation_to_simlock_operation(
                  decrypted_deperso_data_ptr[QMI_UIM_DECRYPT_DEPERSO_SECURE_OPERATION_OFFSET],
                  &decoded_deperso_ptr->operation);
    if(errval != QMI_ERR_NONE)
    {
      break;
    }

    if (decoded_deperso_ptr->operation == QMI_UIM_DEPERSO_TEMP_UNLOCK_ENABLE)
    {
      (void)memscpy((void*)&decoded_deperso_ptr->duration,
                    sizeof(uint32),
                    (void*)&decrypted_deperso_data_ptr[QMI_UIM_DECRYPT_DEPERSO_SECURE_DURATION_OFFSET],
                    sizeof(uint32));
    }
  }
  while (0);

  uimqmi_free(decrypted_deperso_data_ptr);

  return errval;
} /* qmi_uim_tlv_decrypted_deperso_secure */


/*===========================================================================
  FUNCTION QMI_UIM_DECRYPT_DEVICE_MODE()

  DESCRIPTION
    Converts decrypted device mode to the SIMLock device mode enum type

  PARAMETERS
    decrypt_device_mode : decrypt slot value
    simlock_slot_ptr    : output parameter containing SimLock slot id

  RETURN VALUE
    QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_decrypt_device_mode
(
  uint8                              decrypt_device_mode,
  simlock_device_mode_enum_type    * device_mode_ptr
)
{
  ASSERT(device_mode_ptr);

  switch (decrypt_device_mode)
  {
    case 0:
      *device_mode_ptr = SIMLOCK_DEVICE_NORMAL_OPERATION_MODE;
      break;
    case 1:
      *device_mode_ptr = SIMLOCK_DEVICE_EMERGENCY_ONLY_MODE;
      break;
    default:
      return QMI_ERR_INVALID_ARG;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_decrypt_device_mode */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_DECRYPTED_EMERGENCY_ONLY()

  DESCRIPTION
    Parses the TLV item for encrypted emergency only request

  PARAMETERS
    sdu_in                    : input sdu
    len                       : length of the TLV item
    decoded_perso_ptr         : output decoded perso secure data

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_tlv_decrypted_emergency_only
(
  dsm_item_type **                             sdu_in,
  uint16                                       len,
  qmi_uim_tlv_decrypted_emergency_only_type *  decoded_emergency_only_ptr
)
{
  uint8                  * decrypted_emergency_only_data_ptr   = NULL;
  uint32                   decrypted_emergency_only_data_len   = 0;
  qmi_error_e_type         errval                              = QMI_ERR_NONE;

  ASSERT(sdu_in && decoded_emergency_only_ptr);

  memset(decoded_emergency_only_ptr, 0x00, sizeof(qmi_uim_tlv_decrypted_emergency_only_type));

  /* decrypt the request data */
  errval = qmi_uim_tlv_decrypt_request(sdu_in, len,
                                       &decrypted_emergency_only_data_len,
                                       &decrypted_emergency_only_data_ptr);
  if((errval != QMI_ERR_NONE) ||
     (decrypted_emergency_only_data_ptr == NULL))
  {
    UIM_MSG_ERR_0("Failed to decrypt request data");
    return errval;
  }

  do
  {
    /* check the length */
    if(decrypted_emergency_only_data_len < QMI_UIM_DECRYPTED_EMERGENCY_ONLY_REQ_LEN)
    {
      UIM_MSG_ERR_1("QMI-UIM: decrypted data len: 0x%x, less than expected",
                    decrypted_emergency_only_data_len);
      errval = QMI_ERR_MALFORMED_MSG;
      break;
    }

    /* parse the header data */
    errval = qmi_uim_parse_secure_message_header_data(decrypted_emergency_only_data_len,
                                                      decrypted_emergency_only_data_ptr,
                                                      &decoded_emergency_only_ptr->header);
    if(errval != QMI_ERR_NONE)
    {
      break;
    }

    errval = qmi_uim_decrypt_device_mode(
                  decrypted_emergency_only_data_ptr[QMI_UIM_DECRYPT_EMERGENCY_ONLY_MODE_OFFSET],
                  &decoded_emergency_only_ptr->device_mode);
    if(errval != QMI_ERR_NONE)
    {
      break;
    }
  }
  while (0);

  uimqmi_free(decrypted_emergency_only_data_ptr);

  return errval;
} /* qmi_uim_tlv_decrypted_emergency_only */


/*===========================================================================
  FUNCTION QMI_UIM_DECRYPT_SIMLOCK_CONFIG_OPERATION

  DESCRIPTION
    Converts decrypted operation to operation.

  PARAMETERS
    decrypt_operation       : decrypt operation value
    simlock_operation_ptr   : output parameter containing operation

  RETURN VALUE
    QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_decrypt_simlock_config_operation
(
  uint8                                    decrypt_operation,
  qmi_uim_simlock_config_operation_type  * simlock_operation_ptr
)
{
  ASSERT(simlock_operation_ptr);

  switch (decrypt_operation)
  {
    case 0:
      *simlock_operation_ptr = QMI_UIM_SIMLOCK_CONFIG_OPERATION_READ;
      break;
    case 1:
      *simlock_operation_ptr = QMI_UIM_SIMLOCK_CONFIG_OPERATION_WRITE;
      break;
    case 2:
      *simlock_operation_ptr = QMI_UIM_SIMLOCK_CONFIG_OPERATION_ENABLE_REMOTE_SFS;
      break;
    case 3:
      *simlock_operation_ptr = QMI_UIM_SIMLOCK_CONFIG_OPERATION_DISABLE_REMOTE_SFS;
      break;
    default:
      return QMI_ERR_INVALID_ARG;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_decrypt_simlock_config_operation */


/*===========================================================================
  FUNCTION QMI_UIM_DECRYPT_SIMLOCK_CONFIG_RESULT

  DESCRIPTION
    Converts decrypted result to result.

  PARAMETERS
    decrypt_result       : decrypt result value
    simlock_result_ptr   : output parameter containing result

  RETURN VALUE
    QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_decrypt_simlock_config_result
(
  uint8                                    decrypt_result,
  simlock_result_enum_type               * simlock_result_ptr
)
{
  ASSERT(simlock_result_ptr);

  switch (decrypt_result)
  {
    case 0:
      *simlock_result_ptr = SIMLOCK_SUCCESS;
      break;
    case 1:
    case 2:
      *simlock_result_ptr = SIMLOCK_GENERIC_ERROR;
      break;
    case 3:
      *simlock_result_ptr = SIMLOCK_UNSUPPORTED;
      break;
    default:
      return QMI_ERR_INVALID_ARG;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_decrypt_simlock_config_result */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_DECRYPTED_SIMLOCK_CONFIG()

  DESCRIPTION
    Parses the TLV item for encrypted simlock config request

  PARAMETERS
    sdu_in                     : input sdu
    len                        : length of the TLV item
    decoded_simlock_config_ptr : output decoded simlock config data

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_tlv_decrypted_simlock_config
(
  dsm_item_type **                             sdu_in,
  uint16                                       len,
  qmi_uim_tlv_decrypted_simlock_config_type *  decoded_simlock_config_ptr
)
{
  uint8                               * decrypted_simlock_config_data_ptr  = NULL;
  uint32                                decrypted_simlock_config_data_len  = 0;
  qmi_error_e_type                      errval                             = QMI_ERR_NONE;

  ASSERT(sdu_in && decoded_simlock_config_ptr);

  memset(decoded_simlock_config_ptr, 0x00, sizeof(qmi_uim_tlv_decrypted_simlock_config_type));

  /* decrypt the request data */
  errval = qmi_uim_tlv_decrypt_request(sdu_in, len,
                                       &decrypted_simlock_config_data_len,
                                       &decrypted_simlock_config_data_ptr);
  if((errval != QMI_ERR_NONE) ||
     (decrypted_simlock_config_data_ptr == NULL))
  {
    UIM_MSG_ERR_0("Failed to decrypt request data");
    return errval;
  }

  do
  {
    /* check the length */
    if(decrypted_simlock_config_data_len < QMI_UIM_DECRYPTED_SIMLOCK_CONFIG_MIN_LEN)
    {
      errval = QMI_ERR_MALFORMED_MSG;
      break;
    }

    /* parse the header data */
    errval = qmi_uim_parse_secure_message_header_data(decrypted_simlock_config_data_len,
                                                      decrypted_simlock_config_data_ptr,
                                                      &decoded_simlock_config_ptr->header);
    if(errval != QMI_ERR_NONE)
    {
      break;
    }

    errval = qmi_uim_decrypt_simlock_config_operation(
                  decrypted_simlock_config_data_ptr[QMI_UIM_DECRYPT_SIMLOCK_CONFIG_OPERATION_OFFSET],
                  &decoded_simlock_config_ptr->operation);
    if(errval != QMI_ERR_NONE)
    {
      break;
    }

    if(decoded_simlock_config_ptr->operation == QMI_UIM_SIMLOCK_CONFIG_OPERATION_ENABLE_REMOTE_SFS)
    {
      decoded_simlock_config_ptr->operation_type.remote_sfs_operation =
               SIMLOCK_REMOTE_SFS_ENABLE;
    }

    if(decoded_simlock_config_ptr->operation == QMI_UIM_SIMLOCK_CONFIG_OPERATION_DISABLE_REMOTE_SFS)
    {
      decoded_simlock_config_ptr->operation_type.remote_sfs_operation =
               SIMLOCK_REMOTE_SFS_DISABLE;
    }

    if((decoded_simlock_config_ptr->operation == QMI_UIM_SIMLOCK_CONFIG_OPERATION_READ) ||
       (decoded_simlock_config_ptr->operation == QMI_UIM_SIMLOCK_CONFIG_OPERATION_WRITE))
    {
      /* check the length */
      if(decrypted_simlock_config_data_len < QMI_UIM_DECRYPTED_SIMLOCK_CONFIG_LEN)
      {
        errval = QMI_ERR_MALFORMED_MSG;
        break;
      }
    }

    if(decoded_simlock_config_ptr->operation == QMI_UIM_SIMLOCK_CONFIG_OPERATION_WRITE)
    {
      errval = qmi_uim_decrypt_simlock_config_result(
                  decrypted_simlock_config_data_ptr[QMI_UIM_DECRYPT_SIMLOCK_CONFIG_RESULT_OFFSET],
                  &decoded_simlock_config_ptr->operation_type.sfs_write_operation.result);
      if(errval != QMI_ERR_NONE)
      {
        break;
      }

      decoded_simlock_config_ptr->operation_type.sfs_write_operation.file_id =
              decrypted_simlock_config_data_ptr[QMI_UIM_DECRYPT_SIMLOCK_CONFIG_FILE_ID_OFFSET];
    }

    if (decoded_simlock_config_ptr->operation == QMI_UIM_SIMLOCK_CONFIG_OPERATION_READ)
    {
      uint16    temp_data_len         = 0;

      errval = qmi_uim_decrypt_simlock_config_result(
                  decrypted_simlock_config_data_ptr[QMI_UIM_DECRYPT_SIMLOCK_CONFIG_RESULT_OFFSET],
                  &decoded_simlock_config_ptr->operation_type.sfs_read_operation.result);
      if(errval != QMI_ERR_NONE)
      {
        break;
      }

      decoded_simlock_config_ptr->operation_type.sfs_read_operation.file_id =
              decrypted_simlock_config_data_ptr[QMI_UIM_DECRYPT_SIMLOCK_CONFIG_FILE_ID_OFFSET];

      memset(&decoded_simlock_config_ptr->operation_type.sfs_read_operation.data, 0x00, sizeof(simlock_data_type));

      /* in case of a failure, length and data might not be present */
      if(decoded_simlock_config_ptr->operation_type.sfs_read_operation.result != SIMLOCK_SUCCESS)
      {
        break;
      }

      /* check the length */
      if(decrypted_simlock_config_data_len < QMI_UIM_DECRYPTED_SIMLOCK_CONFIG_READ_LEN)
      {
        errval = QMI_ERR_MALFORMED_MSG;
        break;
      }

      /* copy the length */
      (void)memscpy((void*)&temp_data_len,
                    sizeof(uint16),
                    (void*)&decrypted_simlock_config_data_ptr[QMI_UIM_DECRYPT_SIMLOCK_CONFIG_READ_LEN_OFFSET],
                    sizeof(uint16));

      decoded_simlock_config_ptr->operation_type.sfs_read_operation.data.data_len = (uint32)temp_data_len;

      /* check if the data length is correct */
      if(decoded_simlock_config_ptr->operation_type.sfs_read_operation.data.data_len !=
                            (decrypted_simlock_config_data_len - QMI_UIM_DECRYPTED_SIMLOCK_CONFIG_READ_LEN))
      {
        UIM_MSG_ERR_1("QMI-UIM: invalid decrypted read data len: 0x%x",
                      decoded_simlock_config_ptr->operation_type.sfs_read_operation.data.data_len);
        errval = QMI_ERR_MALFORMED_MSG;
        break;
      }

      decoded_simlock_config_ptr->operation_type.sfs_read_operation.data.data_ptr =
            (uint8*)uimqmi_malloc(decoded_simlock_config_ptr->operation_type.sfs_read_operation.data.data_len);
      if(decoded_simlock_config_ptr->operation_type.sfs_read_operation.data.data_ptr == NULL)
      {
        errval = QMI_ERR_NO_MEMORY;
        break;
      }

      (void)memscpy((void*)decoded_simlock_config_ptr->operation_type.sfs_read_operation.data.data_ptr,
                    decoded_simlock_config_ptr->operation_type.sfs_read_operation.data.data_len,
                    (void*)&decrypted_simlock_config_data_ptr[QMI_UIM_DECRYPT_SIMLOCK_CONFIG_READ_DATA_OFFSET],
                    decoded_simlock_config_ptr->operation_type.sfs_read_operation.data.data_len);
    }
  }
  while (0);

  if (errval != QMI_ERR_NONE)
  {
    UIM_MSG_ERR_1("QMI-UIM: decrypted data len: 0x%x, less than expected",
                  decrypted_simlock_config_data_len);
  }

  uimqmi_free(decrypted_simlock_config_data_ptr);

  return errval;
} /* qmi_uim_tlv_decrypted_simlock_config */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_PERSONALIZATION_CATEGORY()

  DESCRIPTION
    Parses the TLV item of reactivate or reuse code perso feature

  PARAMETERS
    sdu_in            : input sdu
    len               : length of the TLV item
    category_type_ptr : output with perso feature to reactivate or reuse code

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_tlv_personalization_category
(
  dsm_item_type **                              sdu_in,
  uint16                                        len,
  qmi_uim_tlv_personalization_category_type   * category_type_ptr
)
{
  int32             temp   = 0;
  qmi_error_e_type  errval = QMI_ERR_NONE;

  PACKED struct PACKED_POST
  {
    uint8  feature;
  } tlv_data;

  ASSERT(sdu_in && category_type_ptr);

  /* Check length */
  if (len != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, len);
  if (temp != len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Update category */
  errval = qmi_uim_convert_category_to_simlock_category(
                  tlv_data.feature,
                  &category_type_ptr->category);
  if(errval != QMI_ERR_NONE)
  {
    return errval;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_personalization_category */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_REMOTE_UNLOCK_CONFIG()

  DESCRIPTION
    Parses the TLV item of remote unlock request

  PARAMETERS
    sdu_in            : input sdu
    len               : length of the TLV item
    config_data_ptr   : output with simlock config data to set

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_tlv_remote_unlock_config
(
  dsm_item_type **                              sdu_in,
  uint16                                        len,
  qmi_uim_tlv_remote_unlock_config_type      *  config_data_ptr
)
{
  int32     temp      = 0;

  PACKED struct PACKED_POST
  {
    uint16  length;
  } tlv_data;

  ASSERT(sdu_in && config_data_ptr);

  /* Check length */
  if (len < sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data and ensure it is formed properly */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, sizeof(tlv_data));
  if (temp != sizeof(tlv_data.length))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  if (tlv_data.length == 0 ||
      len != (sizeof(tlv_data.length) + tlv_data.length))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  config_data_ptr->config_data.data_ptr = (uint8 *)uimqmi_malloc(tlv_data.length);
  if(config_data_ptr->config_data.data_ptr == NULL)
  {
    return QMI_ERR_NO_MEMORY;
  }

  /* Extract the config data */
  temp = dsm_pullup(sdu_in, (void*)config_data_ptr->config_data.data_ptr, tlv_data.length);
  if (temp != tlv_data.length)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  config_data_ptr->config_data.data_len = tlv_data.length;

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_remote_unlock_config */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_REMOTE_UNLOCK_OPERATION

  DESCRIPTION
    Parses the 'operation' TLV item of remote unlock request

  PARAMETERS
    sdu_in                      : input sdu
    len                         : length of the TLV item
    remote_simlock_config_ptr   : output with remote simlock operation

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_tlv_remote_unlock_operation
(
  dsm_item_type                               **sdu_in,
  uint16                                        len,
  qmi_uim_tlv_remote_unlock_operation_type     *remote_unlock_operation_ptr
)
{
   int32             temp = 0;

  PACKED struct PACKED_POST
  {
    uint32  operation;
  } tlv_data;

  ASSERT(sdu_in && remote_unlock_operation_ptr);

  /* Check length */
  if (len != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, len);
  if (temp != len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  switch(tlv_data.operation)
  {
    case UIM_REMOTE_UNLOCK_RETRIEVE_ENCRYPTED_KEY_V01:
    case UIM_REMOTE_UNLOCK_PROCESS_SIMLOCK_DATA_V01:
    case UIM_REMOTE_UNLOCK_GENERATE_MAC_V01:
    case UIM_REMOTE_UNLOCK_GET_MAX_SUPPORTED_VERSION_V01:
    case UIM_REMOTE_UNLOCK_BLOB_REQUEST_V01:
    case UIM_REMOTE_UNLOCK_START_DELAY_TIMER_V01:
    case UIM_REMOTE_UNLOCK_STOP_DELAY_TIMER_V01:
      remote_unlock_operation_ptr->operation = tlv_data.operation;
      break;
    default:
      return QMI_ERR_INVALID_ARG;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_remote_unlock_operation */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_IMSI()

  DESCRIPTION
    Parses the TLV item of verify IMSI request

  PARAMETERS
    sdu_in            : input sdu
    len               : length of the TLV item
    imsi_data_ptr     : output with imsi data

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_tlv_imsi
(
  dsm_item_type **                              sdu_in,
  uint16                                        len,
  qmi_uim_tlv_imsi_type                      *  imsi_data_ptr
)
{
  int32     temp      = 0;

  PACKED struct PACKED_POST
  {
    uint8  length;
  } tlv_data;

  ASSERT(sdu_in && imsi_data_ptr);

  /* Check length */
  if (len < sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data and ensure it is formed properly */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, sizeof(tlv_data));
  if (temp != sizeof(tlv_data.length))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  if (tlv_data.length == 0 ||
      len != (sizeof(tlv_data.length) + tlv_data.length) ||
      tlv_data.length > sizeof(imsi_data_ptr->imsi_data.imsi))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract the config data */
  temp = dsm_pullup(sdu_in, (void*)imsi_data_ptr->imsi_data.imsi, tlv_data.length);
  if (temp != tlv_data.length)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  imsi_data_ptr->imsi_data.imsi_len = tlv_data.length;

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_imsi */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_CONVERT_SIMLOCK_CATEGORY()

  DESCRIPTION
    This function converts the simlock category to the values to be used
    in the TLV response

  PARAMETERS
    category            : Simlock category
    feature_ptr         : output parameter with feature

  RETURN VALUE
    qmi_error_e_type

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_response_convert_simlock_category
(
  simlock_category_enum_type      category,
  uint8                        *  feature_ptr
)
{
  qmi_error_e_type  result = QMI_ERR_NONE;

  ASSERT(feature_ptr);

  switch (category)
  {
    case SIMLOCK_CATEGORY_3GPP_NW:
      *feature_ptr = UIM_PERSO_FEATURE_GW_NETWORK_V01;
      break;
    case SIMLOCK_CATEGORY_3GPP_NS:
      *feature_ptr = UIM_PERSO_FEATURE_GW_NETWORK_SUBSET_V01;
      break;
    case SIMLOCK_CATEGORY_3GPP_SP:
      *feature_ptr = UIM_PERSO_FEATURE_GW_SERVICE_PROVIDER_V01;
      break;
    case SIMLOCK_CATEGORY_3GPP_CP:
      *feature_ptr = UIM_PERSO_FEATURE_GW_CORPORATE_V01;
      break;
    case SIMLOCK_CATEGORY_3GPP_SIM:
      *feature_ptr = UIM_PERSO_FEATURE_GW_UIM_V01;
      break;
    case SIMLOCK_CATEGORY_3GPP_SPN:
      *feature_ptr = UIM_PERSO_FEATURE_GW_SPN_V01;
      break;
    case SIMLOCK_CATEGORY_3GPP_SP_EHPLMN:
      *feature_ptr = UIM_PERSO_FEATURE_GW_SP_EHPLMN_V01;
      break;
    case SIMLOCK_CATEGORY_3GPP_ICCID:
      *feature_ptr = UIM_PERSO_FEATURE_GW_ICCID_V01;
      break;
    case SIMLOCK_CATEGORY_3GPP_NS_SP:
      *feature_ptr = UIM_PERSO_FEATURE_GW_NETWORK_SUBSET_SERVICE_PROVIDER_V01;
      break;
    case SIMLOCK_CATEGORY_3GPP2_NW_TYPE1:
      *feature_ptr = UIM_PERSO_FEATURE_1X_NETWORK_TYPE_1_V01;
      break;
    case SIMLOCK_CATEGORY_3GPP2_NW_TYPE2:
      *feature_ptr = UIM_PERSO_FEATURE_1X_NETWORK_TYPE_2_V01;
      break;
    case SIMLOCK_CATEGORY_3GPP2_HRPD:
      *feature_ptr = UIM_PERSO_FEATURE_1X_HRPD_V01;
      break;
    case SIMLOCK_CATEGORY_3GPP2_SP:
      *feature_ptr = UIM_PERSO_FEATURE_1X_SERVICE_PROVIDER_V01;
      break;
    case SIMLOCK_CATEGORY_3GPP2_CP:
      *feature_ptr = UIM_PERSO_FEATURE_1X_CORPORATE_V01;
      break;
    case SIMLOCK_CATEGORY_3GPP2_RUIM:
      *feature_ptr = UIM_PERSO_FEATURE_1X_RUIM_V01;
      break;
    default:
      result = QMI_ERR_MALFORMED_MSG;
      break;
  }

  return result;
} /* qmi_uim_response_convert_simlock_category */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_GET_CONFIGURATION_SIMLOCK()

  DESCRIPTION
    Composes the TLV content of personalization in the
    configuration

  PARAMETERS
    tlv_id              : id to be used for the TLV
    response_pptr       : output sdu
    get_status_ptr      : feature status information

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_get_configuration_simlock
(
  byte                                      tlv_id,
  dsm_item_type **                          response_pptr,
  const simlock_get_status_msg_resp_type  * get_status_ptr
)
{
  uint16  tlv_len = 0;
  uint8   i       = 0;

  PACKED struct PACKED_POST
  {
    uint8    num_features;
    PACKED struct PACKED_POST
    {
      uint8  feature_id;
      uint8  verify_left;
      uint8  unblock_left;
    } feature[QMI_UIM_MAX_NUMBER_PERSO_FEATURES];
  } tlv_data;

  ASSERT(response_pptr && get_status_ptr);

  memset(&tlv_data, 0, sizeof(tlv_data));

  for (i = 0;
       i < SIMLOCK_CATEGORY_COUNT_MAX && i < QMI_UIM_MAX_NUMBER_PERSO_FEATURES;
       i++)
  {
    ASSERT(tlv_data.num_features < QMI_UIM_MAX_NUMBER_PERSO_FEATURES);

    /* Default to slot 1 lock information */
    if (get_status_ptr->lock_info[0].category_info[i].category_status == FALSE)
    {
      continue;
    }

    if(qmi_uim_response_convert_simlock_category(get_status_ptr->lock_info[0].category_info[i].category,
                                                 &tlv_data.feature[tlv_data.num_features].feature_id) != QMI_ERR_NONE)
    {
      return FALSE;
    }

    /* When num_retries_max is zero, that indicates infinite retries */
    if (get_status_ptr->lock_info[0].category_info[i].num_retries_max == 0)
    {
      tlv_data.feature[tlv_data.num_features].verify_left = QMI_UIM_MAX_RETRIES;
    }
    else if (get_status_ptr->lock_info[0].category_info[i].num_retries_max <=
               get_status_ptr->lock_info[0].category_info[i].curr_retries)
    {
      tlv_data.feature[tlv_data.num_features].verify_left = 0;
    }
    else
    {
      if((get_status_ptr->lock_info[0].category_info[i].num_retries_max -
          get_status_ptr->lock_info[0].category_info[i].curr_retries) >= QMI_UIM_MAX_RETRIES)
      {
        tlv_data.feature[tlv_data.num_features].verify_left = QMI_UIM_MAX_RETRIES;
      }
      else
      {
        tlv_data.feature[tlv_data.num_features].verify_left =
          (uint8)(get_status_ptr->lock_info[0].category_info[i].num_retries_max -
                  get_status_ptr->lock_info[0].category_info[i].curr_retries);
      }
    }

    tlv_data.feature[tlv_data.num_features].unblock_left = 0;
    tlv_data.num_features++;
  }

  /* Calculare the TLV len */
  tlv_len = sizeof(uint8) + (tlv_data.num_features * sizeof(uint8) * 3);

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               tlv_len,
                               (void *)&tlv_data);

} /* qmi_uim_response_get_configuration_simlock */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_TEMPORARY_UNLOCK_TIME_LEFT()

  DESCRIPTION
    Composes the TLV content of temporary depersonalization time left

  PARAMETERS
    tlv_id              : id to be used for the TLV
    response_pptr       : output sdu
    num_slots           : number of slots supported
    get_status_ptr      : feature status information

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_temporary_unlock_time_left
(
  byte                                      tlv_id,
  dsm_item_type **                          response_pptr,
  uint8                                     num_slots,
  const simlock_get_status_msg_resp_type  * get_status_ptr
)
{
  uint16    tlv_len                                  = 0;
  uint16    offset                                   = 0;
  uint8     i                                        = 0;
  uint8     j                                        = 0;
  uint8     slot_cnt                                 = 0;
  uint8     total_features                           = 0;
  uint8  *  tlv_data_ptr                             = NULL;
  uint8     features_in_slot[QMI_UIM_MAX_CARD_COUNT] = {0};
  boolean   ret_value                                = TRUE;

  PACKED struct PACKED_POST
  {
    uint8   feature_id;
    uint32  time_left;
  } feature;

  ASSERT(response_pptr && get_status_ptr);

  slot_cnt = MIN(num_slots, QMI_UIM_MAX_CARD_COUNT);

  /* calculate the number of features active per slot */
  for (i = 0;
       i < SIMLOCK_SLOT_COUNT_MAX && i < slot_cnt;
       i++)
  {
    for(j = 0;
        j < SIMLOCK_CATEGORY_COUNT_MAX && j < QMI_UIM_MAX_NUMBER_PERSO_FEATURES;
        j++)
    {
      if ((get_status_ptr->lock_info[i].category_info[j].category_status) &&
          (get_status_ptr->remaining_time.duration[i][j] > 0))
      {
        features_in_slot[i]++;
        total_features++;
      }
    }
  }

  /* No active features are available */
  if(total_features == 0)
  {
    return TRUE;
  }
  
  /* calculate the total length */
  tlv_len = sizeof(uint8) +
            (slot_cnt * sizeof(uint8)) +
            (sizeof(feature) * total_features);

  tlv_data_ptr = (uint8*)uimqmi_malloc(tlv_len);
  if (tlv_data_ptr == NULL)
  {
    return FALSE;
  }

  /* Insert the number of slots in the TLV data */
  tlv_data_ptr[offset++] = slot_cnt;

  for (i = 0;
       i < SIMLOCK_SLOT_COUNT_MAX && i < slot_cnt;
       i++)
  {
    ASSERT(offset < tlv_len);

    /* Insert the number of active features per slot in the TLV data */
    tlv_data_ptr[offset++] = features_in_slot[i];

    /* Check the features that are active and insert the
       corresponding feature id and time left in the TLV data */
    for(j = 0;
        j < SIMLOCK_CATEGORY_COUNT_MAX && j < QMI_UIM_MAX_NUMBER_PERSO_FEATURES;
        j++)
    {
      if ((get_status_ptr->lock_info[i].category_info[j].category_status) &&
          (get_status_ptr->remaining_time.duration[i][j] > 0))
      {
        if(qmi_uim_response_convert_simlock_category(get_status_ptr->lock_info[i].category_info[j].category,
                                                     &feature.feature_id) != QMI_ERR_NONE)
        {
          uimqmi_free(tlv_data_ptr);
          return FALSE;
        }

        feature.time_left  = get_status_ptr->remaining_time.duration[i][j];

        ASSERT((offset + sizeof(feature)) <= tlv_len);
        (void)memscpy((void*)(tlv_data_ptr + offset),
                      tlv_len - offset,
                      &feature,
                      sizeof(feature));
        offset += sizeof(feature);
      }
    }
  }

  ret_value = qmi_svc_put_param_tlv(response_pptr,
                                    tlv_id,
                                    tlv_len,
                                   (void *)tlv_data_ptr);

  uimqmi_free(tlv_data_ptr);
  return ret_value;
} /* qmi_uim_response_temporary_unlock_time_left */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_CONVERT_SLOT_INDEX_TO_SLOT()

  DESCRIPTION
    Determines the QMI UIM slot id from the slot index

  PARAMETERS
    slot_index          : slot index
    qmi_slot_ptr        : output parameter containing QMI UIM slot id

  RETURN VALUE
    QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_response_convert_slot_index_to_slot
(
  uint8                               slot_index,
  uim_slot_enum_v01                 * qmi_slot_ptr
)
{
  ASSERT(qmi_slot_ptr);

  switch(slot_index)
  {
    case 0:
      *qmi_slot_ptr = UIM_SLOT_1_V01;
      break;

    case 1:
      *qmi_slot_ptr = UIM_SLOT_2_V01;
      break;

    case 2:
      *qmi_slot_ptr = UIM_SLOT_3_V01;
      break;

    case 3:
      *qmi_slot_ptr = UIM_SLOT_4_V01;
      break;

    default:
      return QMI_ERR_INTERNAL;
  }

  return QMI_ERR_NONE;
}/* qmi_uim_response_convert_slot_index_to_slot */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_PERSO_REACTIVATION_STATUS()

  DESCRIPTION
    Composes the TLV content of perso reactivation status

  PARAMETERS
    tlv_id              : id to be used for the TLV
    response_pptr       : output sdu
    num_slots           : number of slots supported
    get_status_ptr      : feature status information

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_perso_reactivation_status
(
  byte                                      tlv_id,
  dsm_item_type **                          response_pptr,
  uint8                                     num_slots,
  const simlock_get_status_msg_resp_type  * get_status_ptr
)
{
  uint16                 tlv_len                     = 0;
  uint16                 offset                      = 0;
  uint8                  i                           = 0;
  uint8                  j                           = 0;
  uint8                  total_features              = 0;
  uim_slot_enum_v01      slot                        = UIM_SLOT_1_V01;
  uint8               *  tlv_data_ptr                = NULL;
  boolean                ret_value                   = TRUE;

  PACKED struct PACKED_POST
  {
    uint8  slot;
    uint8  feature_id;
    uint8  verify_left;
    uint8  unblock_left;
  } tlv_data;

  num_slots = MIN(num_slots, QMI_UIM_MAX_CARD_COUNT);

  ASSERT(response_pptr && get_status_ptr);

  /* calculate the total number of features inactive with valid max retries and current retries */
  for (i = 0;
       i < SIMLOCK_SLOT_COUNT_MAX && i < num_slots;
       i++)
  {
    for(j = 0;
        j < SIMLOCK_CATEGORY_COUNT_MAX && j < QMI_UIM_MAX_NUMBER_PERSO_FEATURES;
        j++)
    {
      if (get_status_ptr->lock_info[i].category_info[j].category_ok_to_reactivate)
      {
        total_features++;
      }
    }
  }

  /* Calculate the total length. The total length includes
     one byte for the count of features in the TLV */
  tlv_len = sizeof(uint8) +
            (sizeof(tlv_data) * total_features);

  tlv_data_ptr = (uint8*)uimqmi_malloc(tlv_len);
  if (tlv_data_ptr == NULL)
  {
    return FALSE;
  }

  /* Insert the number of features in the TLV data */
  tlv_data_ptr[offset++] = total_features;

  for (i = 0;
       i < SIMLOCK_SLOT_COUNT_MAX && i < num_slots;
       i++)
  {
    /* Check the features that are inactive and insert the
       corresponding feature id and left in the TLV data */
    for(j = 0;
        j < SIMLOCK_CATEGORY_COUNT_MAX && j < QMI_UIM_MAX_NUMBER_PERSO_FEATURES;
        j++)
    {
      if (get_status_ptr->lock_info[i].category_info[j].category_ok_to_reactivate)
      {
        if(qmi_uim_response_convert_slot_index_to_slot(i, &slot) != QMI_ERR_NONE)
        {
          uimqmi_free(tlv_data_ptr);
          return FALSE;
        }

        tlv_data.slot = (uint8)slot;

        if(qmi_uim_response_convert_simlock_category(get_status_ptr->lock_info[i].category_info[j].category,
                                                     &tlv_data.feature_id) != QMI_ERR_NONE)
        {
          uimqmi_free(tlv_data_ptr);
          return FALSE;
        }

        /* When num_retries_max is zero, that indicates infinite relock retries */
        if (get_status_ptr->lock_info[i].category_info[j].num_retries_max == 0)
        {
          tlv_data.verify_left = QMI_UIM_MAX_RETRIES;
        }
        else if (get_status_ptr->lock_info[i].category_info[j].num_retries_max <=
              get_status_ptr->lock_info[i].category_info[j].curr_retries)
        {
          tlv_data.verify_left = 0;
        }
        else if((get_status_ptr->lock_info[i].category_info[j].num_retries_max -
              get_status_ptr->lock_info[i].category_info[j].curr_retries) >= QMI_UIM_MAX_RETRIES)
        {
          tlv_data.verify_left = QMI_UIM_MAX_RETRIES;
        }
        else
        {
          tlv_data.verify_left =
            (uint8)(get_status_ptr->lock_info[i].category_info[j].num_retries_max -
                    get_status_ptr->lock_info[i].category_info[j].curr_retries);
        }

        tlv_data.unblock_left = 0;

        ASSERT((offset + sizeof(tlv_data)) <= tlv_len);
        (void)memscpy((void*)(tlv_data_ptr + offset),
                      tlv_len - offset,
                      &tlv_data,
                      sizeof(tlv_data));
        offset += sizeof(tlv_data);
      }
    }
  }

  ret_value = qmi_svc_put_param_tlv(response_pptr,
                                    tlv_id,
                                    tlv_len,
                                   (void *)tlv_data_ptr);

  uimqmi_free(tlv_data_ptr);
  return ret_value;
} /* qmi_uim_response_perso_reactivation_status */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_SIMLOCK_INACTIVE_CONFIG_STATUS()

  DESCRIPTION
    Composes the TLV content of simlock inactive configuration

  PARAMETERS
    tlv_id              : id to be used for the TLV
    response_pptr       : output sdu
    get_status_ptr      : feature status information

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_simlock_inactive_config_status
(
  byte                                      tlv_id,
  dsm_item_type **                          response_pptr,
  const simlock_get_status_msg_resp_type  * get_status_ptr
)
{
  uint16                 tlv_len                     = 0;
  uint8                  index                       = 0;
  boolean                ret_value                   = TRUE;
  uint16                 totat_length_of_nicknames   = 0;
  uint16                 offset                      = 0;
  uint8                * tlv_data_ptr                = NULL;

  PACKED struct PACKED_POST
  {
    uint8    num_inactive_config;
    PACKED struct PACKED_POST
    {
      uint8  config_id;
      uint8  config_nickname_len;
      char   config_nickname[SIMLOCK_CONFIG_NICKNAME_LEN_MAX];

    } inactive_config[SIMLOCK_NUM_INACTIVE_CONFIG_MAX];
  } tlv_data;

  ASSERT(response_pptr && get_status_ptr);

  for (index = 0;
       index < get_status_ptr->inactive_config_data.count && index < SIMLOCK_NUM_INACTIVE_CONFIG_MAX;
       index++)
  {
    totat_length_of_nicknames+=get_status_ptr->inactive_config_data.inactive_config[index].config_nickname.nickname_len;
  }

  /* Calculate the TLV len */
  tlv_len = sizeof(uint8) + 
            (sizeof(uint8) + sizeof(uint8)) * get_status_ptr->inactive_config_data.count +
             totat_length_of_nicknames;

  tlv_data_ptr = (uint8*)uimqmi_malloc(tlv_len);
  if (tlv_data_ptr == NULL)
  {
    return FALSE;
  }

  ASSERT((offset + 1) <= tlv_len);
  tlv_data_ptr[offset++] = get_status_ptr->inactive_config_data.count;

  for (index = 0;
       index < get_status_ptr->inactive_config_data.count && index < SIMLOCK_NUM_INACTIVE_CONFIG_MAX;
       index++)
  {
    ASSERT((offset + 2) <= tlv_len);

    tlv_data_ptr[offset++] = get_status_ptr->inactive_config_data.inactive_config[index].config_id;
    tlv_data_ptr[offset++] = get_status_ptr->inactive_config_data.inactive_config[index].config_nickname.nickname_len;

    ASSERT((offset + get_status_ptr->inactive_config_data.inactive_config[index].config_nickname.nickname_len) <= tlv_len);

    memscpy(tlv_data_ptr + offset,
            tlv_len - offset,
            get_status_ptr->inactive_config_data.inactive_config[index].config_nickname.nickname,
            get_status_ptr->inactive_config_data.inactive_config[index].config_nickname.nickname_len);
    offset += get_status_ptr->inactive_config_data.inactive_config[index].config_nickname.nickname_len;
  }

  ret_value = qmi_svc_put_param_tlv(response_pptr,
                                    tlv_id,
                                    tlv_len,
                                    (void *)tlv_data_ptr);

  uimqmi_free(tlv_data_ptr);
  return ret_value;
} /* qmi_uim_response_simlock_inactive_config_status */


/*===========================================================================
  FUNCTION QMI_UIM_SIMLOCK_SLOT_INDEX_TO_UIM_SLOT()

  DESCRIPTION
    Determines the UIM slot id from the simlock slot index

  PARAMETERS
    slot_index          : slot index
    uim_slot_ptr        : output parameter containing uim slot

  RETURN VALUE
    QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_simlock_slot_index_to_uim_slot
(
  uint8                              slot_index,
  uim_slot_enum_v01                 *uim_slot_ptr
)
{
  ASSERT(uim_slot_ptr);

  switch(slot_index)
  {
    case 0:
      *uim_slot_ptr = UIM_SLOT_1_V01;
      break;

    case 1:
      *uim_slot_ptr = UIM_SLOT_2_V01;
      break;

    case 2:
      *uim_slot_ptr = UIM_SLOT_3_V01;
      break;

    default:
      return QMI_ERR_INTERNAL;
  }

  return QMI_ERR_NONE;
}/* qmi_uim_simlock_slot_index_to_uim_slot */


/*===========================================================================
  FUNCTION  QMI_UIM_RESPONSE_TEMPORARY_UNLOCK_STATUS()

  DESCRIPTION
    Prepares the temporary unlock indication based on the
    response of get status response

  PARAMETERS
    tlv_id                       : ID to be used for TLV
    indication_pptr              : Pointer to Indication pointer
    temporary_unlock_status_ptr  : Pointer to the SIMLOCK get status information
    num_slots       : Number of slots supported

  RETURN VALUE
    TRUE        : If the indication can be send to client
    FALSE       : If otherwise

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_temporary_unlock_status
(
  byte                                     tlv_id,
  dsm_item_type                          **indication_pptr,
  const simlock_get_status_msg_resp_type  *temporary_unlock_status_ptr,
  uint8                                    num_slots
)
{
  uint8    i                                        = 0;
  uint8    j                                        = 0;
  uint8    total_features                           = 0;
  uint16   offset                                   = 0;
  uint16   tlv_len                                  = 0;
  uint8    features_in_slot[QMI_UIM_MAX_CARD_COUNT] = {0};
  uint8   *ind_data_ptr                             = NULL;
  boolean  ret_val                                  = FALSE;

  PACKED struct PACKED_POST
  {
    uint8   feature_id;
    uint32  time_left;
  } temp_unlock_info;

  ASSERT(indication_pptr && temporary_unlock_status_ptr);

  num_slots = MIN(num_slots, QMI_UIM_MAX_CARD_COUNT);

  for (i = 0;
       i < SIMLOCK_SLOT_COUNT_MAX && i < num_slots;
       i++)
  {
    for(j = 0;
        j < SIMLOCK_CATEGORY_COUNT_MAX && j < QMI_UIM_MAX_NUMBER_PERSO_FEATURES;
        j++)
    {
      if ((temporary_unlock_status_ptr->lock_info[i].category_info[j].category_status) &&
          (temporary_unlock_status_ptr->remaining_time.duration[i][j] > 0))
      {
        features_in_slot[i]++;
        total_features++;
      }
    }
  }

  /* calculate the total length */
  tlv_len = sizeof(uint8) +
           (num_slots * sizeof(uint8)) +
           (num_slots * sizeof(uint8)) +
           (sizeof(temp_unlock_info) * total_features);

  ind_data_ptr = (uint8*)uimqmi_malloc(tlv_len);
  if (ind_data_ptr == NULL)
  {
    return FALSE;
  }

  /* Insert the number of slots in the TLV data */
  ind_data_ptr[offset++] = num_slots;

  for (i = 0;
       i < SIMLOCK_SLOT_COUNT_MAX && i < num_slots;
       i++)
  {
    uim_slot_enum_v01   slot = UIM_SLOT_1_V01;

    ASSERT(offset + 2 <= tlv_len);

    /* Insert the number of active features per slot in the TLV data */
    if(qmi_uim_simlock_slot_index_to_uim_slot(i,&slot) != QMI_ERR_NONE)
    {
      uimqmi_free(ind_data_ptr);
      return FALSE;
    }

    /* Copy slot and the number of features active in that slot */
    ind_data_ptr[offset++] = slot;
    ind_data_ptr[offset++] = features_in_slot[i];

    /* Check the features that are active and insert the
       corresponding feature id and time left in the TLV data */
    for(j = 0;
        j < SIMLOCK_CATEGORY_COUNT_MAX && j < QMI_UIM_MAX_NUMBER_PERSO_FEATURES;
        j++)
    {
      if ((temporary_unlock_status_ptr->lock_info[i].category_info[j].category_status) &&
          (temporary_unlock_status_ptr->remaining_time.duration[i][j] > 0))
      {
        /* Pack all the existing temporary unlock features */
        if(qmi_uim_response_convert_simlock_category(temporary_unlock_status_ptr->lock_info[i].category_info[j].category,
                                                     &temp_unlock_info.feature_id) != QMI_ERR_NONE)
        {
          uimqmi_free(ind_data_ptr);
          return FALSE;
        }

        temp_unlock_info.time_left = temporary_unlock_status_ptr->remaining_time.duration[i][j];

        ASSERT((offset + sizeof(temp_unlock_info)) <= tlv_len);
        (void)memscpy((void*)(ind_data_ptr + offset),
                      tlv_len - offset,
                      &temp_unlock_info,
                      sizeof(temp_unlock_info));
        offset += sizeof(temp_unlock_info);
      }
    }
  }

  ret_val = qmi_svc_put_param_tlv(indication_pptr,
                                  tlv_id,
                                  tlv_len,
                                  (void *)ind_data_ptr);

  uimqmi_free(ind_data_ptr);
  return ret_val;
} /* qmi_uim_response_temporary_unlock_status */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_PERSO_CONTROL_KEY_PRESENCE()

  DESCRIPTION
    Composes the TLV content of perso control key presence

  PARAMETERS
    tlv_id              : id to be used for the TLV
    response_pptr       : output sdu
    num_slots           : number of slots supported
    get_status_ptr      : feature status information

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_perso_control_key_presence
(
  byte                                      tlv_id,
  dsm_item_type **                          response_pptr,
  uint8                                     num_slots,
  const simlock_get_status_msg_resp_type  * get_status_ptr
)
{
  uint16    tlv_len                                  = 0;
  uint16    offset                                   = 0;
  uint8     i                                        = 0;
  uint8     j                                        = 0;
  uint8     slot_cnt                                 = 0;
  uint8     total_features                           = 0;
  uint8  *  tlv_data_ptr                             = NULL;
  uint8     features_in_slot[QMI_UIM_MAX_CARD_COUNT] = {0};
  boolean   ret_value                                = TRUE;

  PACKED struct PACKED_POST
  {
    uint8   feature_id;
    uint8   ck_present;
  } feature;

  ASSERT(response_pptr && get_status_ptr);

  slot_cnt = MIN(num_slots, QMI_UIM_MAX_CARD_COUNT);

  /* calculate the number of features active per slot */
  for (i = 0;
       i < SIMLOCK_SLOT_COUNT_MAX && i < slot_cnt;
       i++)
  {
    for(j = 0;
        j < SIMLOCK_CATEGORY_COUNT_MAX && j < QMI_UIM_MAX_NUMBER_PERSO_FEATURES;
        j++)
    {
      if (get_status_ptr->lock_info[i].category_info[j].category_status)
      {
        features_in_slot[i]++;
        total_features++;
      }
    }
  }

  /* No active features are available */
  if(total_features == 0)
  {
    return TRUE;
  }

  /* calculate the total length */
  tlv_len = sizeof(uint8) +
            (slot_cnt * sizeof(uint8)) +
            (sizeof(feature) * total_features);

  tlv_data_ptr = (uint8*)uimqmi_malloc(tlv_len);
  if (tlv_data_ptr == NULL)
  {
    return FALSE;
  }

  /* Insert the number of slots in the TLV data */
  tlv_data_ptr[offset++] = slot_cnt;

  for (i = 0;
       i < SIMLOCK_SLOT_COUNT_MAX && i < slot_cnt;
       i++)
  {
    ASSERT(offset < tlv_len);

    /* Insert the number of active features per slot in the TLV data */
    tlv_data_ptr[offset++] = features_in_slot[i];

    /* Check the features that are active and insert the
       corresponding feature id and time left in the TLV data */
    for(j = 0;
        j < SIMLOCK_CATEGORY_COUNT_MAX && j < QMI_UIM_MAX_NUMBER_PERSO_FEATURES;
        j++)
    {
      if (get_status_ptr->lock_info[i].category_info[j].category_status)
      {
        if(qmi_uim_response_convert_simlock_category(get_status_ptr->lock_info[i].category_info[j].category,
                                                     &feature.feature_id) != QMI_ERR_NONE)
        {
          uimqmi_free(tlv_data_ptr);
          return FALSE;
        }

        feature.ck_present  = get_status_ptr->lock_info[i].category_info[j].ck_present;

        ASSERT((offset + sizeof(feature)) <= tlv_len);
        (void)memscpy((void*)(tlv_data_ptr + offset),
                      tlv_len - offset,
                      &feature,
                      sizeof(feature));
        offset += sizeof(feature);
      }
    }
  }

  ret_value = qmi_svc_put_param_tlv(response_pptr,
                                    tlv_id,
                                    tlv_len,
                                    (void *)tlv_data_ptr);

  uimqmi_free(tlv_data_ptr);
  return ret_value;
} /* qmi_uim_response_perso_control_key_presence */


/*===========================================================================
  FUNCTION QMI_UIM_CONVERT_SIMLOCK_FILE_TYPE_TO_FILE_ID

  DESCRIPTION
    Converts the SIMLock file type to a file id

  PARAMETERS
    file        : simlock file type
    file_id_ptr : converted file id

  RETURN VALUE
    errval          : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_convert_simlock_file_type_to_file_id
(
  simlock_file_type       file,
  uint8                *  file_id_ptr
)
{
  ASSERT(file_id_ptr);

  if(file.file_type == SIMLOCK_FILE_TYPE_CONFIG)
  {
    *file_id_ptr = 0;
    return QMI_ERR_NONE;
  }

  if(file.file_type == SIMLOCK_FILE_TYPE_CATEGORY)
  {
    uint8          file_id_slot      = 1;

    /* simlock slot enums start with 0 (SIMLOCK_SLOT_1) but in
       file_id the slot starts with 1 */
    switch (file.data.category_data.slot)
    {
       case SIMLOCK_SLOT_1:
         file_id_slot = 1;
         break;

       case SIMLOCK_SLOT_2:
         file_id_slot = 2;
         break;

       case SIMLOCK_SLOT_3:
         file_id_slot = 3;
         break;

       case SIMLOCK_SLOT_4:
         file_id_slot = 4;
         break;

       default:
         return QMI_ERR_INTERNAL;
    }

    *file_id_ptr |= file_id_slot;
    *file_id_ptr <<= 4;
    *file_id_ptr |= (uint8)(file.data.category_data.category);
    return QMI_ERR_NONE;
  }

  return QMI_ERR_INTERNAL;
} /* qmi_uim_convert_simlock_file_type_to_file_id */


/*===========================================================================
  FUNCTION  QMI_UIM_SIMLOCK_REMOTE_SFS_ENCRYPTED_IND()

  DESCRIPTION
    Encrypts remote SFS operation request using Trust Zone application APIs

  PARAMETERS
    operation     : Operation requested (Read/Write)
    request_id    : Request ID for this request
    file          : The file to perform operation on
    write_data    : The data to be written (valid for write operation)
    resposne_pptr : Pointer to command response pointer

  RETURN VALUE
    TRUE        : If the encrypted response is build successfully
    FALSE       : If otherwise

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_simlock_remote_sfs_encrypted_ind
(
  uint16                              cmd_id,
  simlock_file_operation_enum_type    operation,
  simlock_token_id_type               request_id,
  simlock_file_type                   file,
  simlock_data_type                   write_data,
  dsm_item_type                    ** response_pptr
)
{
  boolean               encryption_used  = TRUE;
  mmgsdi_data_type      decrypted_data   = {0, NULL};
  mmgsdi_data_type      encrypted_data   = {0, NULL};
  qmi_error_e_type      errval           = QMI_ERR_NONE;

  PACKED struct PACKED_POST
  {
    uint16  cmd_id;
    uint16  request_id;
    uint8   rfu[QMI_UIM_ENCRYPTED_MSG_RFU_SIZE];
    uint8   operation;
    uint8   file_id;
  } fixed_data;

  ASSERT(response_pptr);

  memset(&fixed_data, 0x00, sizeof(fixed_data));

  fixed_data.cmd_id     = cmd_id;
  fixed_data.request_id = request_id;

  switch (operation)
  {
    case SIMLOCK_FILE_OPERATION_READ:
      fixed_data.operation  = 0;
      break;

    case SIMLOCK_FILE_OPERATION_WRITE:
      fixed_data.operation  = 1;
      break;

    default:
      return FALSE;
  }

  errval = qmi_uim_convert_simlock_file_type_to_file_id(file, &fixed_data.file_id);
  if(errval != QMI_ERR_NONE)
  {
    return FALSE;
  }

  /* calculate the length of decrypted indication */
  decrypted_data.data_len = sizeof(fixed_data);

  if(operation == SIMLOCK_FILE_OPERATION_WRITE)
  {
    if ((write_data.data_len == 0) ||
        (write_data.data_ptr == NULL))
    {
      return FALSE;
    }

    /* For this operation, the data_len (2 bytes) and data are sent
       in the indication payload */
    decrypted_data.data_len += (sizeof(uint16) + write_data.data_len);
    if (decrypted_data.data_len > QMI_UIM_MAX_UINT16)
    {
      return FALSE;
    }
  }

  decrypted_data.data_ptr = uimqmi_malloc(decrypted_data.data_len);
  if (decrypted_data.data_ptr == NULL)
  {
    return FALSE;
  }

  /* copy the fixed data */
  (void)memscpy((void*)decrypted_data.data_ptr,
                decrypted_data.data_len,
                (void*)&fixed_data,
                sizeof(fixed_data));

  if(operation == SIMLOCK_FILE_OPERATION_WRITE)
  {
    uint16  temp_data_len = 0;

    temp_data_len = (uint16)write_data.data_len;

    /* copy the length */
    (void)memscpy((void*)(decrypted_data.data_ptr + sizeof(fixed_data)),
                  (decrypted_data.data_len - sizeof(fixed_data)),
                  (uint8*)&temp_data_len,
                  sizeof(uint16));

    /* copy the data */
    (void)memscpy((void*)(decrypted_data.data_ptr + sizeof(fixed_data) + sizeof(uint16)),
                  (decrypted_data.data_len - sizeof(fixed_data) - sizeof(uint16)),
                  (void*)write_data.data_ptr,
                  write_data.data_len);
  }

  encryption_used = qmi_uim_encrypt_payload(decrypted_data,
                                            &encrypted_data);

  if (encryption_used == FALSE ||
      FALSE == qmi_uim_response_generic_payload_16bit(
                 UIMI_TLV_IND_TAG_MANDATORY_1,
                 response_pptr,
                 encrypted_data,
                 TRUE))
  {
    dsm_free_packet(response_pptr);
    errval = QMI_ERR_INTERNAL;
  }

  uimqmi_free(decrypted_data.data_ptr);
  decrypted_data.data_ptr = NULL;

  if(encrypted_data.data_ptr != NULL)
  {
    uimqmi_free(encrypted_data.data_ptr);
    encrypted_data.data_ptr = NULL;
  }

  return (errval == QMI_ERR_NONE ? TRUE : FALSE);
} /* qmi_uim_simlock_remote_sfs_encrypted_ind */

#endif /* FEATURE_SIMLOCK */

