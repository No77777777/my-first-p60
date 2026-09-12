/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


            S I M   L O C K   3 G P P   F U N C T I O N S


GENERAL DESCRIPTION

  This source file contains the simlock 3gpp functions.


EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2014 - 2019 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/simlock/src/simlock_3gpp.c#4 $$ $DateTime: 2019/12/27 15:53:22 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/28/19   sg      Added support for NS+SP category
12/23/19   tq      Check wild digit only for lock data 
09/13/19   vdc     SimLock solution for activation at Point Of Sale
12/03/18   vgd     Remote SIM Unlock support for SIMLOCK_RSU_MODE_D
08/11/17   av      Pass structure by reference to avoid compiler issues
01/26/17   bcho    Support for slot policy valid card must on all slots
01/19/17   ar      Modify wild digit support for GID in SP-EHPLMN category
01/19/17   vdc     Added stub file for simlock 3GPP2 functions
01/09/17   nr      Pass parameters by value in simlock_3gpp_parse_imsi ()
12/20/16   vdc     Support for simlock policy wait for full srv or imsi matched
11/03/16   bcho    Added support for ICCID simlock
10/28/16   nr      Support remote and local configurations together
10/21/16   ar      Move local variable to heap to reduce stack usage
10/06/16   ar      Added support for SP + EHPLMN simlock
09/02/16   vdc     Extend full and expired service policies on any slot
08/25/16   vdc     Apply same config for VALID_CARD_MUST_ON_ANY_SLOT policy
08/10/16   av      Reduce F3 logging in simlock
08/03/16   vdc     Added support for SPN based SIMLOCK feature
01/22/16   av      Support for autolock whitelist
10/05/15   ar      Adding MCCs in list which have default MNC len as 3 digits
06/08/15   stv     Populate retry values in run algo response
02/17/15   vv      Indicate the reason for invalid subscription
01/08/15   vv      Fix for parsing the non-standard IMSIs
01/06/15   vv      Support for identifying test SIMs
12/30/14   bcho    Support for more then 1 bytes of GID1/GID2
12/26/14   vv      Support for whitelist and blacklist codes in same category
09/29/14   vv      Added support for emergency only mode
08/29/14   vv      Added support for temporary unlock
04/14/14   vv      Added support for wild char in lock codes
02/14/14   vv      Initial revision
===========================================================================*/


/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "customer.h"

#ifdef FEATURE_SIMLOCK

#include "simlock_3gpp.h"
#include "simlock_util.h"
#include "simlock_category.h"
#include "simlock_timer.h"
#include "simlock_platform.h"
#include "simlock_policy.h"

#define SIMLOCK_IMSI_BYTE_0          0x00
#define SIMLOCK_IMSI_BYTE_1          0x01
#define SIMLOCK_IMSI_BYTE_2          0x02
#define SIMLOCK_IMSI_BYTE_3          0x03
#define SIMLOCK_IMSI_BYTE_4          0x04
#define SIMLOCK_IMSI_BYTE_5          0x05

#define SIMLOCK_IMSI_LOW_BYTE        0x0F
#define SIMLOCK_IMSI_HIGH_BYTE       0xF0

#define SIMLOCK_MNC_2_DIGITS         0x02
#define SIMLOCK_MNC_3_DIGITS         0x03
#define SIMLOCK_MNC_LEN_BYTE         0x03

#define SIMLOCK_IS_VALID_MSIN_CHAR(c)              \
  ((c >= '0' && c <= '9') ? TRUE : FALSE)

/*===========================================================================
FUNCTION SIMLOCK_3GPP_GET_NUM_OF_MNC_DIGITS

DESCRIPTION
  This function determines the number of MNC digits

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  Number of MNC digits

SIDE EFFECTS
  NONE
===========================================================================*/
static uint8 simlock_3gpp_get_num_of_mnc_digits
(
  simlock_ad_type      ad_data
)
{
  /* Old SIM cards had only 2 digits MNC and EF-AD with 3 bytes only.
     Default to 2 digits to keep backward compatibility */
  uint8  num_mnc_digits        = SIMLOCK_MNC_2_DIGITS;

  /* EF-AD 4th byte will tell the MNC len which has been populated correctly
     inside mmgsdi_card_init_update_ad_mnc_len during initialization */
  if (ad_data.ad_len > SIMLOCK_MNC_LEN_BYTE &&
      ad_data.ad[SIMLOCK_MNC_LEN_BYTE] != 0xFF)
  {
    if ( ((ad_data.ad[SIMLOCK_MNC_LEN_BYTE] & 0x0F) == SIMLOCK_MNC_2_DIGITS ) ||
         ((ad_data.ad[SIMLOCK_MNC_LEN_BYTE] & 0x0F) == SIMLOCK_MNC_3_DIGITS ) )
    {
      num_mnc_digits = ad_data.ad[SIMLOCK_MNC_LEN_BYTE] & 0x0F;
    }
  }
  else
  {
    SIMLOCK_MSG_MED_1("EF-AD has invalid MNC length: 0x%x",
                      ad_data.ad[SIMLOCK_MNC_LEN_BYTE]);
  }

  return num_mnc_digits;
} /* simlock_3gpp_get_num_of_mnc_digits */


/*===========================================================================
FUNCTION SIMLOCK_3GPP_PARSE_ICCID

DESCRIPTION
  This function parses the ICCID from the card and provides the data in
  a structured format

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_result_enum_type simlock_3gpp_parse_iccid
(
  const simlock_iccid_file_type           * iccid_data_ptr,
  simlock_iccid_type                      * out_data_ptr
)
{
  uint8 index     = 0;
  uint8 digit_cnt = 0;

  if(iccid_data_ptr == NULL ||
     iccid_data_ptr->iccid_len == 0 ||
     iccid_data_ptr->iccid_len > SIMLOCK_ICCID_LEN ||
     out_data_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  for(index = 0; index < iccid_data_ptr->iccid_len; index++)
  {
    if((iccid_data_ptr->iccid_data[index] & 0x0F) <= 9)
    {
      out_data_ptr->iccid_data[digit_cnt] = (iccid_data_ptr->iccid_data[index]& 0x0F) + '0';
    }
    else
    {
      /* For invalid digit, use character 'a' */
      out_data_ptr->iccid_data[digit_cnt] = 'a';
    }
    digit_cnt++;

    if(((iccid_data_ptr->iccid_data[index] >> 4) & 0x0F) <= 9)
    {
      out_data_ptr->iccid_data[digit_cnt] = ((iccid_data_ptr->iccid_data[index] >> 4)& 0x0F) + '0';
    }
    else
    {
      /* For invalid digit, use character 'a' */
      out_data_ptr->iccid_data[digit_cnt] = 'a';
    }
    digit_cnt++;
  }

  out_data_ptr->iccid_len = digit_cnt;
  return SIMLOCK_SUCCESS;
} /* simlock_3gpp_parse_iccid */


/*===========================================================================
FUNCTION SIMLOCK_3GPP_PARSE_IMSI

DESCRIPTION
  This function parses the 3GPP IMSI from the card and provides the data in
  a structured format

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_result_enum_type simlock_3gpp_parse_imsi
(
  const simlock_imsi_type                  imsi_data,
  const simlock_ad_type                    ad_data,
  simlock_decoded_subscription_data_type * out_data_ptr
)
{
  uint8                     num_mnc_digits    = 0;
  uint8                     imsi_len          = 0;
  uint8                     msin_len          = 0;
  uint8                     msin_offset       = 0;

  if((imsi_data.imsi_len == 0)    ||
     (out_data_ptr == NULL))
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  /* first byte in the buffer is the length */
  imsi_len = imsi_data.imsi[SIMLOCK_IMSI_BYTE_0];

  if(imsi_len < SIMLOCK_MIN_VALID_IMSI_LEN ||
     imsi_len > SIMLOCK_MAX_VALID_IMSI_LEN ||
     imsi_len > (imsi_data.imsi_len + 1))
  {
    SIMLOCK_MSG_ERR_1("imsi length invalid 0x%x", imsi_data.imsi_len);
    return SIMLOCK_GENERIC_ERROR;
  }

  /* Get the number of mnc digits */
  num_mnc_digits = simlock_3gpp_get_num_of_mnc_digits(ad_data);

  /* -----------------------------------------------------------
    Parse MCC/MNC and MSIN from the IMSI:
    B0     B1       B2       B3       B4     B5   B6   B7    B8
    LEN  MCC1PB   MCC3MCC2 MNC2MNC1 D1MNC3  D3D2 D5D4 D7D6  D9D8

    2-digit MNC
    B0     B1       B2       B3       B4     B5   B6   B7    B8
    LEN  MCC1PB   MCC3MCC2 MNC2MNC1  D2D1   D4D3 D6D5 D8D7  D10D9
  -------------------------------------------------------------*/

  if(imsi_data.imsi_len > SIMLOCK_IMSI_BYTE_3)
  {
    /* Decode MCC digits in ASCII */
    out_data_ptr->mcc[0] = (((imsi_data.imsi[SIMLOCK_IMSI_BYTE_1] & SIMLOCK_IMSI_HIGH_BYTE) >> 4) + '0');
    out_data_ptr->mcc[1] = ((imsi_data.imsi[SIMLOCK_IMSI_BYTE_2] & SIMLOCK_IMSI_LOW_BYTE) + '0');
    out_data_ptr->mcc[2] = (((imsi_data.imsi[SIMLOCK_IMSI_BYTE_2] & SIMLOCK_IMSI_HIGH_BYTE) >> 4) + '0');

    /* Decode MNC digits in ASCII */
    out_data_ptr->mnc.mnc_data[0] = ((imsi_data.imsi[SIMLOCK_IMSI_BYTE_3] & SIMLOCK_IMSI_LOW_BYTE) + '0');
    out_data_ptr->mnc.mnc_data[1] = (((imsi_data.imsi[SIMLOCK_IMSI_BYTE_3] & SIMLOCK_IMSI_HIGH_BYTE) >> 4) + '0');
  }

  if(num_mnc_digits == 2)
  {
    out_data_ptr->mnc.mnc_len = 2;
  }
  else
  {
    out_data_ptr->mnc.mnc_len = SIMLOCK_MNC_MAX;
    if(imsi_data.imsi_len > SIMLOCK_IMSI_BYTE_4)
    {
      out_data_ptr->mnc.mnc_data[2] = ((imsi_data.imsi[SIMLOCK_IMSI_BYTE_4] & SIMLOCK_IMSI_LOW_BYTE) + '0');
    }
  }

  /* Decode d6 and d7 in ASCII. Irrespective of MNC length (2 or 3 digits),
     msin_data[0] is considered d6
     msin_data[1] is considered d7 */

  /* Decode MSIN in ASCII */
  do
  {
    char c = '0';

    if(imsi_data.imsi_len > SIMLOCK_IMSI_BYTE_4)
    {
      if(num_mnc_digits == 2)
      {
        c = ((imsi_data.imsi[SIMLOCK_IMSI_BYTE_4] & SIMLOCK_IMSI_LOW_BYTE) + '0');
        if(!SIMLOCK_IS_VALID_MSIN_CHAR(c))
        {
          break;
        }
        out_data_ptr->msin.msin_data[msin_len++] = c;

        c = (((imsi_data.imsi[SIMLOCK_IMSI_BYTE_4] & SIMLOCK_IMSI_HIGH_BYTE) >> 4) + '0');
        if(!SIMLOCK_IS_VALID_MSIN_CHAR(c))
        {
          break;
        }
        out_data_ptr->msin.msin_data[msin_len++] = c;
      }
      else
      {
        c = (((imsi_data.imsi[SIMLOCK_IMSI_BYTE_4] & SIMLOCK_IMSI_HIGH_BYTE) >> 4) + '0');
        if(!SIMLOCK_IS_VALID_MSIN_CHAR(c))
        {
          break;
        }
        out_data_ptr->msin.msin_data[msin_len++] = c;
      }
    }

    if(imsi_data.imsi_len > SIMLOCK_IMSI_BYTE_5)
    {
      for(msin_offset = SIMLOCK_IMSI_BYTE_5; msin_offset <= imsi_len; msin_offset++)
      {
        c = ((imsi_data.imsi[msin_offset] & SIMLOCK_IMSI_LOW_BYTE) + '0');
        if(!SIMLOCK_IS_VALID_MSIN_CHAR(c))
        {
          break;
        }

        if((msin_len + 1) > SIMLOCK_MSIN_MAX)
        {
          SIMLOCK_MSG_ERR_1("Invalid MSIN length: 0x%x", msin_len);
          return SIMLOCK_GENERIC_ERROR;
        }

        out_data_ptr->msin.msin_data[msin_len++] = c;

        c = (((imsi_data.imsi[msin_offset] & SIMLOCK_IMSI_HIGH_BYTE) >> 4) + '0');
        if(!SIMLOCK_IS_VALID_MSIN_CHAR(c))
        {
          break;
        }

        if((msin_len + 1) > SIMLOCK_MSIN_MAX)
        {
          SIMLOCK_MSG_ERR_1("Invalid MSIN length: 0x%x", msin_len);
          return SIMLOCK_GENERIC_ERROR;
        }

        out_data_ptr->msin.msin_data[msin_len++] = c;
      }
    }
  } while(0);

  out_data_ptr->msin.msin_len = msin_len;
  return SIMLOCK_SUCCESS;
} /* simlock_3gpp_parse_imsi */


/*===========================================================================
FUNCTION SIMLOCK_3GPP_EXTRACT_PLMN_FROM_EHPLMN_DATA

DESCRIPTION
  This function fills the EHPLMN from ehplmn data array to Simlock structure

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  None

SIDE EFFECTS
  NONE
===========================================================================*/
static void simlock_3gpp_extract_plmn_from_ehplmn_data
(
  const simlock_subscription_data_type    * sim_data_ptr,
  simlock_decoded_subscription_data_type  * sim_subs_data_ptr,
  uint8                                     index
)
{
  uint8     offset    = 0;

  if (sim_data_ptr == NULL || sim_subs_data_ptr == NULL || index >= SIMLOCK_EHPLMN_NUM_MAX ||
      (index * SIMLOCK_PLMN_ID_LEN + 2) >= sim_data_ptr->ehplmn.ehplmn_len)
  {
    return;
  }

  offset = index * SIMLOCK_PLMN_ID_LEN;

  /* -----------------------------------------------------------
     Decode MCC and MNC

     Parse MCC/MNC from EHPLMN:
     B0          B1          B2
     MCC2MCC1    MNC3MCC3    MNC2MNC1
    ------------------------------------------------------------*/
  sim_subs_data_ptr->ehplmn.ehplmn_entry[index].mcc[0] = ((sim_data_ptr->ehplmn.ehplmn_data[offset] & SIMLOCK_IMSI_LOW_BYTE) + '0');
  sim_subs_data_ptr->ehplmn.ehplmn_entry[index].mcc[1] = (((sim_data_ptr->ehplmn.ehplmn_data[offset] & SIMLOCK_IMSI_HIGH_BYTE) >> 4) + '0');
  sim_subs_data_ptr->ehplmn.ehplmn_entry[index].mcc[2] = ((sim_data_ptr->ehplmn.ehplmn_data[offset + 1] & SIMLOCK_IMSI_LOW_BYTE) + '0');

  sim_subs_data_ptr->ehplmn.ehplmn_entry[index].mnc.mnc_data[0] = ((sim_data_ptr->ehplmn.ehplmn_data[offset + 2] & SIMLOCK_IMSI_LOW_BYTE) + '0');
  sim_subs_data_ptr->ehplmn.ehplmn_entry[index].mnc.mnc_data[1] = (((sim_data_ptr->ehplmn.ehplmn_data[offset + 2] & SIMLOCK_IMSI_HIGH_BYTE) >> 4) + '0');

  /* Check length of MNC and decode 3rd MNC byte accordingly */
  if((sim_data_ptr->ehplmn.ehplmn_data[offset + 1] & SIMLOCK_IMSI_HIGH_BYTE) == 0xF0)
  {
    sim_subs_data_ptr->ehplmn.ehplmn_entry[index].mnc.mnc_len = 2;
  }
  else
  {
    sim_subs_data_ptr->ehplmn.ehplmn_entry[index].mnc.mnc_len = SIMLOCK_MNC_MAX;
    sim_subs_data_ptr->ehplmn.ehplmn_entry[index].mnc.mnc_data[2] = (((sim_data_ptr->ehplmn.ehplmn_data[offset + 1] & SIMLOCK_IMSI_HIGH_BYTE) >> 4) + '0');
  }
} /* simlock_3gpp_extract_plmn_from_ehplmn_data */


/*===========================================================================
FUNCTION SIMLOCK_3GPP_RUN_ALGORITHM_SIM

DESCRIPTION
  This function validates the 3GPP SIM code from the card and the
  lock data retrieved from the category file

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_code_in_list_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_code_in_list_enum_type simlock_3gpp_run_algorithm_sim
(
  const simlock_decoded_subscription_data_type *sim_subs_data_ptr,
  simlock_slot_enum_type                        slot,
  simlock_category_file_data_type              *category_data_ptr
)
{
  simlock_result_enum_type           simlock_status    = SIMLOCK_SUCCESS;
  uint32                             code_index        = 0;
  simlock_code_in_list_enum_type     code_in_list      = SIMLOCK_CODE_IN_NOLIST;
  boolean                            code_in_whitelist = FALSE;
  boolean                            code_in_blacklist = FALSE;

  if(category_data_ptr == NULL || sim_subs_data_ptr == NULL)
  {
    return code_in_list;
  }

  SIMLOCK_MSG_LOW_6("run_algo_sim: From sim - mcc[0],mcc[1],mcc[2]: 0x%x,0x%x,0x%x; mnc[0],mnc[1],mnc[2]: 0x%x,0x%x,0x%x",
                    sim_subs_data_ptr->mcc[0], sim_subs_data_ptr->mcc[1], sim_subs_data_ptr->mcc[2],
                    sim_subs_data_ptr->mnc.mnc_data[0], sim_subs_data_ptr->mnc.mnc_data[1], sim_subs_data_ptr->mnc.mnc_data[2]);

  /* if autolock is not enabled, atleast one valid code is expected */
  if(!category_data_ptr->category_header.auto_lock &&
     (category_data_ptr->category_header.num_of_codes == 0 ||
      category_data_ptr->code_data_ptr == NULL))
  {
    SIMLOCK_MSG_ERR_0("no valid code data");
    return code_in_list;
  }

  /* Compare the IMSI from the file and SIM data */
  for (code_index = 0; code_index < category_data_ptr->category_header.num_of_codes;
       code_index++)
  {
    if(simlock_util_check_if_mcc_matches(category_data_ptr->code_data_ptr[code_index].mcc,
                                         sim_subs_data_ptr->mcc,
                                         FALSE)                                 &&
       simlock_util_check_if_mnc_matches(&(category_data_ptr->code_data_ptr[code_index].mnc),
                                         &(sim_subs_data_ptr->mnc),
                                         FALSE))
    {
      if(simlock_util_check_if_msin_matches(&(category_data_ptr->code_data_ptr[code_index].msin),
                                            &(sim_subs_data_ptr->msin)))
      {
        if(category_data_ptr->code_data_ptr[code_index].code_type == SIMLOCK_CODE_WHITELIST)
        {
          code_in_list = SIMLOCK_CODE_IN_WHITELIST;
          code_in_whitelist = TRUE;
        }
        if(category_data_ptr->code_data_ptr[code_index].code_type == SIMLOCK_CODE_BLACKLIST)
        {
          code_in_list = SIMLOCK_CODE_IN_BLACKLIST;
          code_in_blacklist = TRUE;
        }
      }
    }
  }

  /* If autolock is enabled and,
     - no codes were present during locking process, lock the device to
       the current sim data.
     - a valid list (whitelist) of codes were present during the locking
       process, the intention is to auto lock the device to the current sim
       data if it matches with any of the codes present in the whitelist of
       auto-lock codes.
     In both these cases, we update the lock code data in the category data
     file */
  if(category_data_ptr->category_header.auto_lock  &&
     (category_data_ptr->category_header.num_of_codes == 0 ||
      code_in_whitelist))
  {
    simlock_category_code_data_type    sim_code_data;

    SIMLOCK_MSG_MED_0("autolock enabled, updating code data");

    simlock_category_initialize_code_data(&sim_code_data);

    /* Copy the sim_subs_data to sim_code_data to write to file */
    sim_code_data.mnc         = sim_subs_data_ptr->mnc;
    sim_code_data.msin        = sim_subs_data_ptr->msin;
    simlock_memscpy(sim_code_data.mcc, sizeof(sim_code_data.mcc),
                    sim_subs_data_ptr->mcc, sizeof(sim_subs_data_ptr->mcc));

    /* write the lock codes from SIM */
    simlock_status = simlock_category_update_and_write_lock_data(&category_data_ptr->category_header,
                                                                 &sim_code_data,
                                                                 slot,
                                                                 SIMLOCK_SFS_LOCAL);
    code_in_list = (simlock_status == SIMLOCK_SUCCESS ? SIMLOCK_CODE_IN_WHITELIST : SIMLOCK_CODE_IN_NOLIST);
  }

  if(code_in_whitelist && code_in_blacklist)
  {
    code_in_list = SIMLOCK_CODE_IN_WHITELIST_AND_BLACKLIST;
  }

  SIMLOCK_MSG_MED_1("sim code in list type: 0x%x", code_in_list);
  return code_in_list;
} /* simlock_3gpp_run_algorithm_sim */


/*===========================================================================
FUNCTION SIMLOCK_3GPP_RUN_ALGORITHM_CP

DESCRIPTION
  This function validates the 3GPP CP code from the card and the
  lock data retrieved from the category file

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_code_in_list_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_code_in_list_enum_type simlock_3gpp_run_algorithm_cp
(
  const simlock_decoded_subscription_data_type *sim_subs_data_ptr,
  simlock_slot_enum_type                        slot,
  simlock_category_file_data_type              *category_data_ptr
)
{
  simlock_result_enum_type           simlock_status    = SIMLOCK_SUCCESS;
  uint32                             code_index        = 0;
  simlock_code_in_list_enum_type     code_in_list      = SIMLOCK_CODE_IN_NOLIST;
  boolean                            code_in_whitelist = FALSE;
  boolean                            code_in_blacklist = FALSE;

  if(category_data_ptr == NULL || sim_subs_data_ptr == NULL)
  {
    return code_in_list;
  }

  SIMLOCK_MSG_LOW_8("run_algo_cp: From sim - mcc[0],mcc[1],mcc[2]: 0x%x,0x%x,0x%x; mnc[0],mnc[1],mnc[2]: 0x%x,0x%x,0x%x; gid1Len,gid2Len: 0x%x,0x%x",
                    sim_subs_data_ptr->mcc[0], sim_subs_data_ptr->mcc[1], sim_subs_data_ptr->mcc[2],
                    sim_subs_data_ptr->mnc.mnc_data[0], sim_subs_data_ptr->mnc.mnc_data[1], sim_subs_data_ptr->mnc.mnc_data[2],
                    sim_subs_data_ptr->gid1.gid_len, sim_subs_data_ptr->gid2.gid_len);

  /* if autolock is not enabled, atleast one valid code is expected */
  if(!category_data_ptr->category_header.auto_lock &&
     (category_data_ptr->category_header.num_of_codes == 0 ||
      category_data_ptr->code_data_ptr == NULL))
  {
    SIMLOCK_MSG_ERR_0("no valid code data");
    return code_in_list;
  }

  /* compare MCCs/MNCs, gid1 and gid2 from the file and IMSI */
  for (code_index = 0; code_index < category_data_ptr->category_header.num_of_codes;
       code_index++)
  {
    if(simlock_util_check_if_mcc_matches(category_data_ptr->code_data_ptr[code_index].mcc,
                                         sim_subs_data_ptr->mcc,
                                         FALSE)                                  &&
       simlock_util_check_if_mnc_matches(&(category_data_ptr->code_data_ptr[code_index].mnc),
                                         &(sim_subs_data_ptr->mnc),
                                         FALSE))
    {
      /* the card should be configured to have a valid gid1
         and gid2 (any value between, 0x00 - 0xFE). If not,
         the card is not valid */
      if((sim_subs_data_ptr->gid1.gid_len > 0) &&
         (sim_subs_data_ptr->gid2.gid_len > 0))
      {
        if(simlock_util_check_if_gid_matches(&(category_data_ptr->code_data_ptr[code_index].gid1),
                                             &(sim_subs_data_ptr->gid1),
                                             TRUE,
                                             FALSE)                       &&
           simlock_util_check_if_gid_matches(&(category_data_ptr->code_data_ptr[code_index].gid2),
                                             &(sim_subs_data_ptr->gid2),
                                             TRUE,
                                             FALSE))
        {
          if(category_data_ptr->code_data_ptr[code_index].code_type == SIMLOCK_CODE_WHITELIST)
          {
            code_in_list = SIMLOCK_CODE_IN_WHITELIST;
            code_in_whitelist = TRUE;
          }
          if(category_data_ptr->code_data_ptr[code_index].code_type == SIMLOCK_CODE_BLACKLIST)
          {
            code_in_list = SIMLOCK_CODE_IN_BLACKLIST;
            code_in_blacklist = TRUE;
          }
        }
      }
    }
  }

  /* If autolock is enabled and,
     - no codes were present during locking process, lock the device to
       the current sim data.
     - a valid list (whitelist) of codes were present during the locking
       process, the intention is to auto lock the device to the current sim
       data if matches with any of the codes present in the whitelist of
       auto-lock codes.
     In both these cases, we update the lock code data in the category data
     file */
  if(category_data_ptr->category_header.auto_lock  &&
     (category_data_ptr->category_header.num_of_codes == 0 ||
      code_in_whitelist))
  {
    simlock_category_code_data_type    sim_code_data;

    SIMLOCK_MSG_MED_0("autolock enabled, updating code data");

    if((sim_subs_data_ptr->gid1.gid_len == 0) ||
       (sim_subs_data_ptr->gid2.gid_len == 0))
    {
      SIMLOCK_MSG_ERR_2("autolock is enabled, but gid1 : 0x%x or"
                        " gid2 :0x%x is invalid",
                         sim_subs_data_ptr->gid1.gid_data[0], sim_subs_data_ptr->gid2.gid_data[0]);
      return SIMLOCK_CODE_IN_NOLIST;
    }

    simlock_category_initialize_code_data(&sim_code_data);

    /* Copy the sim_subs_data to sim_code_data to write to file */
    sim_code_data.mnc         = sim_subs_data_ptr->mnc;
    sim_code_data.gid1        = sim_subs_data_ptr->gid1;
    sim_code_data.gid2        = sim_subs_data_ptr->gid2;
    simlock_memscpy(sim_code_data.mcc, sizeof(sim_code_data.mcc),
                    sim_subs_data_ptr->mcc, sizeof(sim_subs_data_ptr->mcc));

    /* format the category data to write to file.
       set the GID1 and GID2 lengths explicitly to 1 since
       in autolock case, only 1 byte for GIDs is supported
       as per the 22.022 Spec */
    sim_code_data.gid1.gid_len = SIMLOCK_MIN_GID_LEN;
    sim_code_data.gid2.gid_len = SIMLOCK_MIN_GID_LEN;

    /* write the lock codes from SIM */
    simlock_status = simlock_category_update_and_write_lock_data(&category_data_ptr->category_header,
                                                                 &sim_code_data,
                                                                 slot,
                                                                 SIMLOCK_SFS_LOCAL);
    code_in_list = (simlock_status == SIMLOCK_SUCCESS ? SIMLOCK_CODE_IN_WHITELIST : SIMLOCK_CODE_IN_NOLIST);
  }

  if(code_in_whitelist && code_in_blacklist)
  {
    code_in_list = SIMLOCK_CODE_IN_WHITELIST_AND_BLACKLIST;
  }

  SIMLOCK_MSG_MED_1("cp code in list type: 0x%x", code_in_list);
  return code_in_list;
} /* simlock_3gpp_run_algorithm_cp */


/*===========================================================================
FUNCTION SIMLOCK_3GPP_RUN_ALGORITHM_SP

DESCRIPTION
  This function validates the 3GPP SP code from the card and the
  lock data retrieved from the category file

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_code_in_list_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_code_in_list_enum_type simlock_3gpp_run_algorithm_sp
(
  const simlock_decoded_subscription_data_type *sim_subs_data_ptr,
  simlock_slot_enum_type                        slot,
  simlock_category_file_data_type              *category_data_ptr
)
{
  simlock_result_enum_type           simlock_status    = SIMLOCK_SUCCESS;
  uint32                             code_index        = 0;
  simlock_code_in_list_enum_type     code_in_list      = SIMLOCK_CODE_IN_NOLIST;
  boolean                            code_in_whitelist = FALSE;
  boolean                            code_in_blacklist = FALSE;

  if(category_data_ptr == NULL || sim_subs_data_ptr == NULL)
  {
    return code_in_list;
  }

  SIMLOCK_MSG_LOW_7("run_algo_sp: From sim - mcc[0],mcc[1],mcc[2]: 0x%x,0x%x,0x%x; mnc[0],mnc[1],mnc[2]: 0x%x,0x%x,0x%x; gid1Len: 0x%x",
                    sim_subs_data_ptr->mcc[0], sim_subs_data_ptr->mcc[1], sim_subs_data_ptr->mcc[2],
                    sim_subs_data_ptr->mnc.mnc_data[0], sim_subs_data_ptr->mnc.mnc_data[1], sim_subs_data_ptr->mnc.mnc_data[2],
                    sim_subs_data_ptr->gid1.gid_len);

  /* if autolock is not enabled, atleast one valid code is expected */
  if(!category_data_ptr->category_header.auto_lock &&
     (category_data_ptr->category_header.num_of_codes == 0 ||
      category_data_ptr->code_data_ptr == NULL))
  {
    SIMLOCK_MSG_ERR_0("no valid code data");
    return code_in_list;
  }

  /* compare MCCs/MNCs and gid1 from the file and IMSI */
  for (code_index = 0; code_index < category_data_ptr->category_header.num_of_codes;
       code_index++)
  {
    if(simlock_util_check_if_mcc_matches(category_data_ptr->code_data_ptr[code_index].mcc,
                                         sim_subs_data_ptr->mcc,
                                         FALSE) &&
       simlock_util_check_if_mnc_matches(&(category_data_ptr->code_data_ptr[code_index].mnc),
                                         &(sim_subs_data_ptr->mnc),
                                         FALSE))
    {
      /* the card should be configured to have a valid gid1
        (any value between, 0x00 - 0xFE). If not,
        the card is not valid */
      if(sim_subs_data_ptr->gid1.gid_len > 0)
      {
        if(simlock_util_check_if_gid_matches(&(category_data_ptr->code_data_ptr[code_index].gid1),
                                             &(sim_subs_data_ptr->gid1),
                                             TRUE,
                                             FALSE))
        {
          if(category_data_ptr->code_data_ptr[code_index].code_type == SIMLOCK_CODE_WHITELIST)
          {
            code_in_list = SIMLOCK_CODE_IN_WHITELIST;
            code_in_whitelist = TRUE;
          }
          if(category_data_ptr->code_data_ptr[code_index].code_type == SIMLOCK_CODE_BLACKLIST)
          {
            code_in_list = SIMLOCK_CODE_IN_BLACKLIST;
            code_in_blacklist = TRUE;
          }
        }
      }
    }
  }

  /* If autolock is enabled and,
     - no codes were present during locking process, lock the device to
       the current sim data.
     - a valid list (whitelist) of codes were present during the locking
       process, the intention is to auto lock the device to the current sim
       data if matches with any of the codes present in the whitelist of
       auto-lock codes.
     In both these cases, we update the lock code data in the category data
     file */
  if(category_data_ptr->category_header.auto_lock  &&
     (category_data_ptr->category_header.num_of_codes == 0 ||
      code_in_whitelist))
  {
    simlock_category_code_data_type    sim_code_data;

    SIMLOCK_MSG_MED_0("autolock enabled, updating code data");

    if(sim_subs_data_ptr->gid1.gid_len == 0)
    {
      SIMLOCK_MSG_ERR_1("autolock is enabled, but gid1 : 0x%x is invalid",
                        sim_subs_data_ptr->gid1.gid_data[0]);
      return code_in_list;
    }

    simlock_category_initialize_code_data(&sim_code_data);

    /* Copy the sim_subs_data to sim_code_data to write to file */
    sim_code_data.mnc         = sim_subs_data_ptr->mnc;
    sim_code_data.gid1        = sim_subs_data_ptr->gid1;
    simlock_memscpy(sim_code_data.mcc, sizeof(sim_code_data.mcc),
                    sim_subs_data_ptr->mcc, sizeof(sim_subs_data_ptr->mcc));

    /* format the category data to write to file.
       set the GID1 length explicitly to 1, since in
       autolock case, only 1 byte for GID is supported,
       as per the 22.022 Spec */
    sim_code_data.gid1.gid_len = SIMLOCK_MIN_GID_LEN;

    /* write the lock codes from SIM */
    simlock_status = simlock_category_update_and_write_lock_data(&category_data_ptr->category_header,
                                                                 &sim_code_data,
                                                                 slot,
                                                                 SIMLOCK_SFS_LOCAL);
    code_in_list = (simlock_status == SIMLOCK_SUCCESS ? SIMLOCK_CODE_IN_WHITELIST : SIMLOCK_CODE_IN_NOLIST);
  }

  if(code_in_whitelist && code_in_blacklist)
  {
    code_in_list = SIMLOCK_CODE_IN_WHITELIST_AND_BLACKLIST;
  }

  SIMLOCK_MSG_MED_1("sp code in list type: 0x%x", code_in_list);
  return code_in_list;
} /* simlock_3gpp_run_algorithm_sp */


/*===========================================================================
FUNCTION SIMLOCK_3GPP_RUN_ALGORITHM_NS

DESCRIPTION
  This function validates the 3GPP NS code from the card and the
  lock data retrieved from the category file

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_code_in_list_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_code_in_list_enum_type simlock_3gpp_run_algorithm_ns
(
  const simlock_decoded_subscription_data_type *sim_subs_data_ptr,
  simlock_slot_enum_type                        slot,
  simlock_category_file_data_type              *category_data_ptr
)
{
  simlock_result_enum_type           simlock_status    = SIMLOCK_SUCCESS;
  uint32                             code_index        = 0;
  simlock_code_in_list_enum_type     code_in_list      = SIMLOCK_CODE_IN_NOLIST;
  boolean                            code_in_whitelist = FALSE;
  boolean                            code_in_blacklist = FALSE;

  if(category_data_ptr == NULL || sim_subs_data_ptr == NULL)
  {
    return code_in_list;
  }

  SIMLOCK_MSG_LOW_8("run_algo_ns: From sim - mcc[0],mcc[1],mcc[2]: 0x%x,0x%x,0x%x; mnc[0],mnc[1],mnc[2]: 0x%x,0x%x,0x%x; d6,d7:0x%x,0x%x",
                    sim_subs_data_ptr->mcc[0], sim_subs_data_ptr->mcc[1], sim_subs_data_ptr->mcc[2],
                    sim_subs_data_ptr->mnc.mnc_data[0], sim_subs_data_ptr->mnc.mnc_data[1], sim_subs_data_ptr->mnc.mnc_data[2],
                    sim_subs_data_ptr->msin.msin_data[0], sim_subs_data_ptr->msin.msin_data[1]);

  /* if autolock is not enabled, atleast one valid code is expected */
  if(!category_data_ptr->category_header.auto_lock &&
     (category_data_ptr->category_header.num_of_codes == 0 ||
      category_data_ptr->code_data_ptr == NULL))
  {
    SIMLOCK_MSG_ERR_0("no valid code data");
    return code_in_list;
  }

  /* compare MCCs/MNCs, imsi_d6 and imsi_d7 from the file and IMSI */
  for (code_index = 0; code_index < category_data_ptr->category_header.num_of_codes;
       code_index++)
  {
    if(simlock_util_check_if_mcc_matches(category_data_ptr->code_data_ptr[code_index].mcc,
                                         sim_subs_data_ptr->mcc,
                                         FALSE) &&
       simlock_util_check_if_mnc_matches(&(category_data_ptr->code_data_ptr[code_index].mnc),
                                         &(sim_subs_data_ptr->mnc),
                                         FALSE))
    {
      if(simlock_util_check_if_imsi_digit_matches(category_data_ptr->code_data_ptr[code_index].msin.msin_data[0],
                                                  sim_subs_data_ptr->msin.msin_data[0],
                                                  FALSE)                        &&
         simlock_util_check_if_imsi_digit_matches(category_data_ptr->code_data_ptr[code_index].msin.msin_data[1],
                                                  sim_subs_data_ptr->msin.msin_data[1],
                                                  FALSE))
      {
        if(category_data_ptr->code_data_ptr[code_index].code_type == SIMLOCK_CODE_WHITELIST)
        {
          code_in_list = SIMLOCK_CODE_IN_WHITELIST;
          code_in_whitelist = TRUE;
        }
        if(category_data_ptr->code_data_ptr[code_index].code_type == SIMLOCK_CODE_BLACKLIST)
        {
          code_in_list = SIMLOCK_CODE_IN_BLACKLIST;
          code_in_blacklist = TRUE;
        }
      }
    }
  }

  /* If autolock is enabled and,
     - no codes were present during locking process, lock the device to
       the current sim data.
     - a valid list (whitelist) of codes were present during the locking
       process, the intention is to auto lock the device to the current sim
       data if matches with any of the codes present in the whitelist of
       auto-lock codes.
     In both these cases, we update the lock code data in the category data
     file */
  if(category_data_ptr->category_header.auto_lock  &&
     (category_data_ptr->category_header.num_of_codes == 0 ||
      code_in_whitelist))
  {
    simlock_category_code_data_type    sim_code_data;

    SIMLOCK_MSG_MED_0("autolock enabled, updating code data");

    simlock_category_initialize_code_data(&sim_code_data);

    /* Copy the sim_subs_data to sim_code_data to write to file */
    sim_code_data.mnc         = sim_subs_data_ptr->mnc;
    sim_code_data.msin        = sim_subs_data_ptr->msin;
    simlock_memscpy(sim_code_data.mcc, sizeof(sim_code_data.mcc),
                    sim_subs_data_ptr->mcc, sizeof(sim_subs_data_ptr->mcc));

    /* write the lock codes from SIM */
    simlock_status = simlock_category_update_and_write_lock_data(&category_data_ptr->category_header,
                                                                 &sim_code_data,
                                                                 slot,
                                                                 SIMLOCK_SFS_LOCAL);
    code_in_list = (simlock_status == SIMLOCK_SUCCESS ? SIMLOCK_CODE_IN_WHITELIST : SIMLOCK_CODE_IN_NOLIST);
  }

  if(code_in_whitelist && code_in_blacklist)
  {
    code_in_list = SIMLOCK_CODE_IN_WHITELIST_AND_BLACKLIST;
  }

  SIMLOCK_MSG_MED_1("ns code in list type: 0x%x", code_in_list);
  return code_in_list;
} /* simlock_3gpp_run_algorithm_ns */


/*===========================================================================
FUNCTION SIMLOCK_3GPP_RUN_ALGORITHM_NW

DESCRIPTION
  This function validates the 3GPP NW code from the card and the
  lock data retrieved from the category file

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_code_in_list_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_code_in_list_enum_type simlock_3gpp_run_algorithm_nw
(
  const simlock_decoded_subscription_data_type *sim_subs_data_ptr,
  simlock_slot_enum_type                        slot,
  simlock_category_file_data_type              *category_data_ptr
)
{
  simlock_result_enum_type           simlock_status    = SIMLOCK_SUCCESS;
  uint32                             code_index        = 0;
  simlock_code_in_list_enum_type     code_in_list      = SIMLOCK_CODE_IN_NOLIST;
  boolean                            code_in_whitelist = FALSE;
  boolean                            code_in_blacklist = FALSE;

  if(category_data_ptr == NULL || sim_subs_data_ptr == NULL)
  {
    return code_in_list;
  }

  SIMLOCK_MSG_LOW_6("run_algo_nw: From sim - mcc[0],mcc[1],mcc[2]: 0x%x,0x%x,0x%x; mnc[0],mnc[1],mnc[2]: 0x%x,0x%x,0x%x",
                    sim_subs_data_ptr->mcc[0], sim_subs_data_ptr->mcc[1], sim_subs_data_ptr->mcc[2],
                    sim_subs_data_ptr->mnc.mnc_data[0], sim_subs_data_ptr->mnc.mnc_data[1], sim_subs_data_ptr->mnc.mnc_data[2]);

  /* if autolock is not enabled, atleast one valid code is expected */
  if(!category_data_ptr->category_header.auto_lock &&
     (category_data_ptr->category_header.num_of_codes == 0 ||
      category_data_ptr->code_data_ptr == NULL))
  {
    SIMLOCK_MSG_ERR_0("no valid code data");
    return code_in_list;
  }

  /* compare MCCs/MNCs from the file and IMSI */
  for (code_index = 0; code_index < category_data_ptr->category_header.num_of_codes;
       code_index++)
  {
    if(simlock_util_check_if_mcc_matches(category_data_ptr->code_data_ptr[code_index].mcc,
                                         sim_subs_data_ptr->mcc,
                                         FALSE) &&
       simlock_util_check_if_mnc_matches(&(category_data_ptr->code_data_ptr[code_index].mnc),
                                         &(sim_subs_data_ptr->mnc),
                                         FALSE))
    {
      if(category_data_ptr->code_data_ptr[code_index].code_type == SIMLOCK_CODE_WHITELIST)
      {
        code_in_list = SIMLOCK_CODE_IN_WHITELIST;
        code_in_whitelist = TRUE;
      }
      if(category_data_ptr->code_data_ptr[code_index].code_type == SIMLOCK_CODE_BLACKLIST)
      {
        code_in_list = SIMLOCK_CODE_IN_BLACKLIST;
        code_in_blacklist = TRUE;
      }
    }
  }

  /* If autolock is enabled and,
     - no codes were present during locking process, lock the device to
       the current sim data.
     - a valid list (whitelist) of codes were present during the locking
       process, the intention is to auto lock the device to the current sim
       data if matches with any of the codes present in the whitelist of
       auto-lock codes.
     In both these cases, we update the lock code data in the category data
     file */
  if(category_data_ptr->category_header.auto_lock  &&
     (category_data_ptr->category_header.num_of_codes == 0 ||
      code_in_whitelist))
  {
    simlock_category_code_data_type    sim_code_data;

    SIMLOCK_MSG_MED_0("autolock enabled, updating code data");

    simlock_category_initialize_code_data(&sim_code_data);

    /* Copy the sim_subs_data to sim_code_data to write to file */
    sim_code_data.mnc         = sim_subs_data_ptr->mnc;
    simlock_memscpy(sim_code_data.mcc, sizeof(sim_code_data.mcc),
                    sim_subs_data_ptr->mcc, sizeof(sim_subs_data_ptr->mcc));

    /* write the lock codes from SIM */
    simlock_status = simlock_category_update_and_write_lock_data(&category_data_ptr->category_header,
                                                                 &sim_code_data,
                                                                 slot,
                                                                 SIMLOCK_SFS_LOCAL);
    code_in_list = (simlock_status == SIMLOCK_SUCCESS ? SIMLOCK_CODE_IN_WHITELIST : SIMLOCK_CODE_IN_NOLIST);
  }

  if(code_in_whitelist && code_in_blacklist)
  {
    code_in_list = SIMLOCK_CODE_IN_WHITELIST_AND_BLACKLIST;
  }

  SIMLOCK_MSG_MED_1("nw code in list type: 0x%x", code_in_list);
  return code_in_list;
} /* simlock_3gpp_run_algorithm_nw */


/*===========================================================================
FUNCTION SIMLOCK_3GPP_RUN_ALGORITHM_SPN

DESCRIPTION
  This function validates the 3GPP SPN code from the card and the
  lock data retrieved from the category file

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_code_in_list_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_code_in_list_enum_type simlock_3gpp_run_algorithm_spn
(
  const simlock_decoded_subscription_data_type *sim_subs_data_ptr,
  simlock_slot_enum_type                        slot,
  simlock_category_file_data_type              *category_data_ptr
)
{
  simlock_result_enum_type           simlock_status    = SIMLOCK_SUCCESS;
  uint32                             code_index        = 0;
  simlock_code_in_list_enum_type     code_in_list      = SIMLOCK_CODE_IN_NOLIST;
  boolean                            code_in_whitelist = FALSE;
  boolean                            code_in_blacklist = FALSE;

  if(category_data_ptr == NULL || sim_subs_data_ptr == NULL)
  {
    return code_in_list;
  }

  SIMLOCK_MSG_LOW_6("run_algo_spn: From sim - mcc[0]: 0x%x, mcc[1]: 0x%x, mcc[2]: 0x%x, mnc[0]: 0x%x, mnc[1]: 0x%x, mnc[2]: 0x%x",
                    sim_subs_data_ptr->mcc[0], sim_subs_data_ptr->mcc[1], sim_subs_data_ptr->mcc[2],
                    sim_subs_data_ptr->mnc.mnc_data[0], sim_subs_data_ptr->mnc.mnc_data[1], sim_subs_data_ptr->mnc.mnc_data[2]);

  /* if autolock is not enabled, atleast one valid code is expected */
  if(!category_data_ptr->category_header.auto_lock &&
     (category_data_ptr->category_header.num_of_codes == 0 ||
      category_data_ptr->code_data_ptr == NULL))
  {
    SIMLOCK_MSG_ERR_0("no valid code data");
    return code_in_list;
  }

  /* Compare the IMSI from the file and SIM data */
  for (code_index = 0; code_index < category_data_ptr->category_header.num_of_codes;
       code_index++)
  {
    if(simlock_util_check_if_mcc_matches(category_data_ptr->code_data_ptr[code_index].mcc,
                                         sim_subs_data_ptr->mcc,
                                         FALSE) &&
       simlock_util_check_if_mnc_matches(&(category_data_ptr->code_data_ptr[code_index].mnc),
                                         &(sim_subs_data_ptr->mnc),
                                         FALSE))
    {
      if(simlock_util_check_if_spn_matches(&category_data_ptr->code_data_ptr[code_index].spn,
                                           &sim_subs_data_ptr->spn))
      {
        if(category_data_ptr->code_data_ptr[code_index].code_type == SIMLOCK_CODE_WHITELIST)
        {
          code_in_list = SIMLOCK_CODE_IN_WHITELIST;
          code_in_whitelist = TRUE;
        }
        if(category_data_ptr->code_data_ptr[code_index].code_type == SIMLOCK_CODE_BLACKLIST)
        {
          code_in_list = SIMLOCK_CODE_IN_BLACKLIST;
          code_in_blacklist = TRUE;
        }
      }
    }
  }

  /* If autolock is enabled and,
     - no codes were present during locking process, lock the device to
       the current sim data.
     - a valid list (whitelist) of codes were present during the locking
       process, the intention is to auto lock the device to the current sim
       data if it matches with any of the codes present in the whitelist of
       auto-lock codes.
     In both these cases, we update the lock code data in the category data
     file */
  if(category_data_ptr->category_header.auto_lock  &&
     (category_data_ptr->category_header.num_of_codes == 0 ||
      code_in_whitelist))
  {
    simlock_category_code_data_type    sim_code_data;

    SIMLOCK_MSG_MED_0("autolock enabled, updating code data");

    simlock_category_initialize_code_data(&sim_code_data);

    /* Copy the sim_subs_data to sim_code_data to write to file */
    sim_code_data.mnc         = sim_subs_data_ptr->mnc;
    sim_code_data.spn         = sim_subs_data_ptr->spn;
    simlock_memscpy(sim_code_data.mcc, sizeof(sim_code_data.mcc),
                    sim_subs_data_ptr->mcc, sizeof(sim_subs_data_ptr->mcc));

    /* write the lock codes from SIM */
    simlock_status = simlock_category_update_and_write_lock_data(&category_data_ptr->category_header,
                                                                 &sim_code_data,
                                                                 slot,
                                                                 SIMLOCK_SFS_LOCAL);
    code_in_list = (simlock_status == SIMLOCK_SUCCESS ? SIMLOCK_CODE_IN_WHITELIST : SIMLOCK_CODE_IN_NOLIST);
  }

  if(code_in_whitelist && code_in_blacklist)
  {
    code_in_list = SIMLOCK_CODE_IN_WHITELIST_AND_BLACKLIST;
  }

  SIMLOCK_MSG_MED_1("spn code in list type: 0x%x", code_in_list);
  return code_in_list;
} /* simlock_3gpp_run_algorithm_spn */


/*===========================================================================
FUNCTION SIMLOCK_3GPP_RUN_ALGORITHM_SP_EHPLMN

DESCRIPTION
  This function validates the 3GPP SP code and the EHPLMN code from the card
  with the lock data retrieved from the category file

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_code_in_list_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_code_in_list_enum_type simlock_3gpp_run_algorithm_sp_ehplmn
(
  const simlock_decoded_subscription_data_type *sim_subs_data_ptr,
  simlock_slot_enum_type                        slot,
  simlock_category_file_data_type              *category_data_ptr
)
{
  uint32                             code_index        = 0;
  simlock_code_in_list_enum_type     code_in_list      = SIMLOCK_CODE_IN_NOLIST;
  boolean                            code_in_whitelist = FALSE;
  boolean                            code_in_blacklist = FALSE;

  (void)slot;

  if(category_data_ptr == NULL || sim_subs_data_ptr == NULL)
  {
    return code_in_list;
  }

  SIMLOCK_MSG_LOW_6("simlock_3gpp_run_algorithm_sp_ehplmn: From IMSI - mcc[0]: 0x%x, mcc[1]: 0x%x, mcc[2]: 0x%x, mnc[0]: 0x%x, mnc[1]: 0x%x, mnc[2]: 0x%x",
                    sim_subs_data_ptr->mcc[0], sim_subs_data_ptr->mcc[1], sim_subs_data_ptr->mcc[2],
                    sim_subs_data_ptr->mnc.mnc_data[0], sim_subs_data_ptr->mnc.mnc_data[1], sim_subs_data_ptr->mnc.mnc_data[2]);

  /* Auto-lock is not supported for SP + EHPLMN category because there
     could be multiple EHPLMNs in card so which one to lock is not decided.*/
  if(category_data_ptr->category_header.num_of_codes == 0 ||
     category_data_ptr->code_data_ptr == NULL)
  {
    SIMLOCK_MSG_ERR_0("no valid code data");
    return code_in_list;
  }

  /* Compare the IMSI from the file and SIM data */
  for (code_index = 0; code_index < category_data_ptr->category_header.num_of_codes;
       code_index++)
  {
    SIMLOCK_MSG_LOW_6("From file - mcc[0]: 0x%x, mcc[1]: 0x%x, mcc[2]: 0x%x, mnc[0]: 0x%x, mnc[1]: 0x%x, mnc[2]: 0x%x",
                      category_data_ptr->code_data_ptr[code_index].mcc[0],
                      category_data_ptr->code_data_ptr[code_index].mcc[1],
                      category_data_ptr->code_data_ptr[code_index].mcc[2],
                      category_data_ptr->code_data_ptr[code_index].mnc.mnc_data[0],
                      category_data_ptr->code_data_ptr[code_index].mnc.mnc_data[1],
                      category_data_ptr->code_data_ptr[code_index].mnc.mnc_data[2]);

    if(simlock_util_check_if_mcc_matches(category_data_ptr->code_data_ptr[code_index].mcc,
                                         sim_subs_data_ptr->mcc,
                                         FALSE) &&
       simlock_util_check_if_mnc_matches(&(category_data_ptr->code_data_ptr[code_index].mnc),
                                         &(sim_subs_data_ptr->mnc),
                                         FALSE) &&
       simlock_util_check_if_gid_matches(&(category_data_ptr->code_data_ptr[code_index].gid1),
                                         &(sim_subs_data_ptr->gid1),
                                         FALSE,
                                         FALSE))
    {
      uint8    ehplmn_index = 0;

      for(ehplmn_index = 0; ehplmn_index < sim_subs_data_ptr->ehplmn.num_ehplmns; ehplmn_index++)
      {
        /* Check if the EHPLMN data of code is matching with the SIM data */
        if(simlock_util_check_if_mcc_matches(category_data_ptr->code_data_ptr[code_index].ehplmn_mcc,
                                             sim_subs_data_ptr->ehplmn.ehplmn_entry[ehplmn_index].mcc,
                                             FALSE) &&
           simlock_util_check_if_mnc_matches(&(category_data_ptr->code_data_ptr[code_index].ehplmn_mnc),
                                             &(sim_subs_data_ptr->ehplmn.ehplmn_entry[ehplmn_index].mnc),
                                             FALSE))
        {
          if(category_data_ptr->code_data_ptr[code_index].code_type == SIMLOCK_CODE_WHITELIST)
          {
            code_in_list = SIMLOCK_CODE_IN_WHITELIST;
            code_in_whitelist = TRUE;
            break;
          }
          if(category_data_ptr->code_data_ptr[code_index].code_type == SIMLOCK_CODE_BLACKLIST)
          {
            code_in_list = SIMLOCK_CODE_IN_BLACKLIST;
            code_in_blacklist = TRUE;
            break;
          }
        }
      }
    }
  }

  /* It is possible that one EHPLMN is in white list and other EHPLMN is in Blacklist */
  if(code_in_whitelist && code_in_blacklist)
  {
    code_in_list = SIMLOCK_CODE_IN_WHITELIST_AND_BLACKLIST;
  }

  SIMLOCK_MSG_MED_1("sp-ehplmn code in list type: 0x%x", code_in_list);
  return code_in_list;
} /* simlock_3gpp_run_algorithm_sp_ehplmn */


/*===========================================================================
FUNCTION SIMLOCK_3GPP_RUN_ALGORITHM_ICCID

DESCRIPTION
  This function validates the 3GPP ICCID code from the card and the
  lock data retrieved from the category file

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_code_in_list_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_code_in_list_enum_type simlock_3gpp_run_algorithm_iccid
(
  const simlock_decoded_subscription_data_type *sim_subs_data_ptr,
  simlock_slot_enum_type                        slot,
  simlock_category_file_data_type             * category_data_ptr
)
{
  simlock_result_enum_type           simlock_status    = SIMLOCK_SUCCESS;
  uint32                             code_index        = 0;
  simlock_code_in_list_enum_type     code_in_list      = SIMLOCK_CODE_IN_NOLIST;
  boolean                            code_in_whitelist = FALSE;
  boolean                            code_in_blacklist = FALSE;

  if(category_data_ptr == NULL || sim_subs_data_ptr == NULL)
  {
    return code_in_list;
  }

  SIMLOCK_MSG_LOW_6("run_algo_iccid: From sim - mcc[0]: 0x%x, mcc[1]: 0x%x, mcc[2]: 0x%x, mnc[0]: 0x%x, mnc[1]: 0x%x, mnc[2]: 0x%x",
                    sim_subs_data_ptr->mcc[0], sim_subs_data_ptr->mcc[1], sim_subs_data_ptr->mcc[2],
                    sim_subs_data_ptr->mnc.mnc_data[0], sim_subs_data_ptr->mnc.mnc_data[1], sim_subs_data_ptr->mnc.mnc_data[2]);

  /* if autolock is not enabled, atleast one valid code is expected */
  if(!category_data_ptr->category_header.auto_lock &&
     (category_data_ptr->category_header.num_of_codes == 0 ||
      category_data_ptr->code_data_ptr == NULL))
  {
    SIMLOCK_MSG_ERR_0("no valid code data");
    return code_in_list;
  }

  /* Compare MCC, MNC and ICCID from the file and SIM data */
  for (code_index = 0; code_index < category_data_ptr->category_header.num_of_codes;
       code_index++)
  {
    if(simlock_util_check_if_mcc_matches(category_data_ptr->code_data_ptr[code_index].mcc,
                                         sim_subs_data_ptr->mcc,
                                         FALSE) &&
       simlock_util_check_if_mnc_matches(&(category_data_ptr->code_data_ptr[code_index].mnc),
                                         &(sim_subs_data_ptr->mnc),
                                         FALSE))
    {
      if(simlock_util_check_if_iccid_matches(&(category_data_ptr->code_data_ptr[code_index].iccid),
                                             &(sim_subs_data_ptr->iccid)))
      {
        if(category_data_ptr->code_data_ptr[code_index].code_type == SIMLOCK_CODE_WHITELIST)
        {
          code_in_list = SIMLOCK_CODE_IN_WHITELIST;
          code_in_whitelist = TRUE;
        }
        if(category_data_ptr->code_data_ptr[code_index].code_type == SIMLOCK_CODE_BLACKLIST)
        {
          code_in_list = SIMLOCK_CODE_IN_BLACKLIST;
          code_in_blacklist = TRUE;
        }
      }
    }
  }

  /* If autolock is enabled and,
     - no codes were present during locking process, lock the device to
       the current sim data.
     - a valid list (whitelist) of codes were present during the locking
       process, the intention is to auto lock the device to the current sim
       data if it matches with any of the codes present in the whitelist of
       auto-lock codes.
     In both these cases, we update the lock code data in the category data
     file */
  if(category_data_ptr->category_header.auto_lock  &&
     (category_data_ptr->category_header.num_of_codes == 0 ||
      code_in_whitelist) &&
      sim_subs_data_ptr->iccid.iccid_len > 0)
  {
    simlock_category_code_data_type    sim_code_data;

    SIMLOCK_MSG_MED_0("autolock enabled, updating code data");

    simlock_category_initialize_code_data(&sim_code_data);

    /* Copy the sim_subs_data to sim_code_data to write to file */
    sim_code_data.mnc         = sim_subs_data_ptr->mnc;
    sim_code_data.iccid       = sim_subs_data_ptr->iccid;
    simlock_memscpy(sim_code_data.mcc, sizeof(sim_code_data.mcc),
                    sim_subs_data_ptr->mcc, sizeof(sim_subs_data_ptr->mcc));

    /* write the lock codes from SIM */
    simlock_status = simlock_category_update_and_write_lock_data(&category_data_ptr->category_header,
                                                                 &sim_code_data,
                                                                 slot,
                                                                 SIMLOCK_SFS_LOCAL);
    code_in_list = (simlock_status == SIMLOCK_SUCCESS ? SIMLOCK_CODE_IN_WHITELIST : SIMLOCK_CODE_IN_NOLIST);
  }

  if(code_in_whitelist && code_in_blacklist)
  {
    code_in_list = SIMLOCK_CODE_IN_WHITELIST_AND_BLACKLIST;
  }

  SIMLOCK_MSG_MED_1("iccid code in list type: 0x%x", code_in_list);
  return code_in_list;
} /* simlock_3gpp_run_algorithm_iccid */


/*===========================================================================
FUNCTION SIMLOCK_3GPP_RUN_ALGORITHM_NS_SP

DESCRIPTION
  This function validates the 3GPP NS and SP code from the card and the
  lock data retrieved from the category file

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_code_in_list_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_code_in_list_enum_type simlock_3gpp_run_algorithm_ns_sp
(
  const simlock_decoded_subscription_data_type *sim_subs_data_ptr,
  simlock_slot_enum_type                        slot,
  simlock_category_file_data_type              *category_data_ptr
)
{
  simlock_result_enum_type           simlock_status    = SIMLOCK_SUCCESS;
  uint32                             code_index        = 0;
  simlock_code_in_list_enum_type     code_in_list      = SIMLOCK_CODE_IN_NOLIST;
  boolean                            code_in_whitelist = FALSE;
  boolean                            code_in_blacklist = FALSE;

  if(category_data_ptr == NULL || sim_subs_data_ptr == NULL)
  {
    return code_in_list;
  }

  SIMLOCK_MSG_LOW_8("run_algo_ns_sp: From sim - mcc[0],mcc[1],mcc[2]: 0x%x, 0x%x, 0x%x; mnc[0],mnc[1],mnc[2]: 0x%x, 0x%x, 0x%x; d6,d7:0x%x, 0x%x",
                    sim_subs_data_ptr->mcc[0], sim_subs_data_ptr->mcc[1], sim_subs_data_ptr->mcc[2],
                    sim_subs_data_ptr->mnc.mnc_data[0], sim_subs_data_ptr->mnc.mnc_data[1], sim_subs_data_ptr->mnc.mnc_data[2],
                    sim_subs_data_ptr->msin.msin_data[0], sim_subs_data_ptr->msin.msin_data[1]);

  /* If autolock is not enabled, atleast one valid code is expected */
  if(!category_data_ptr->category_header.auto_lock &&
     (category_data_ptr->category_header.num_of_codes == 0 ||
      category_data_ptr->code_data_ptr == NULL))
  {
    SIMLOCK_MSG_ERR_0("no valid code data");
    return code_in_list;
  }

  /* Compare MCCs/MNCs, imsi_d6 and imsi_d7 and GID1 from the EF IMSI & EF GID1 */
  for (code_index = 0; code_index < category_data_ptr->category_header.num_of_codes;
       code_index++)
  {
    if(simlock_util_check_if_mcc_matches(category_data_ptr->code_data_ptr[code_index].mcc,
                                         sim_subs_data_ptr->mcc,
                                         FALSE) &&
       simlock_util_check_if_mnc_matches(&(category_data_ptr->code_data_ptr[code_index].mnc),
                                         &(sim_subs_data_ptr->mnc),
                                         FALSE))
    {
      if(simlock_util_check_if_imsi_digit_matches(category_data_ptr->code_data_ptr[code_index].msin.msin_data[0],
                                                  sim_subs_data_ptr->msin.msin_data[0],
                                                  FALSE)                        &&
         simlock_util_check_if_imsi_digit_matches(category_data_ptr->code_data_ptr[code_index].msin.msin_data[1],
                                                  sim_subs_data_ptr->msin.msin_data[1],
                                                  FALSE))
      {
        /* The card should be configured to have a valid gid1
           (any value between, 0x00 - 0xFE). If not, the card is not valid */
        if(sim_subs_data_ptr->gid1.gid_len > 0)
        {
          if(simlock_util_check_if_gid_matches(&(category_data_ptr->code_data_ptr[code_index].gid1),
                                               &(sim_subs_data_ptr->gid1),
                                               TRUE,
                                               FALSE))
          {
            if(category_data_ptr->code_data_ptr[code_index].code_type == SIMLOCK_CODE_WHITELIST)
            {
              code_in_list = SIMLOCK_CODE_IN_WHITELIST;
              code_in_whitelist = TRUE;
            }
            if(category_data_ptr->code_data_ptr[code_index].code_type == SIMLOCK_CODE_BLACKLIST)
            {
              code_in_list = SIMLOCK_CODE_IN_BLACKLIST;
              code_in_blacklist = TRUE;
            }
          }
        }
        else
        {
          uint8 gid_index = 0;

          /* Allow cards without GID1 or service disabled if all the GID1
             bytes are configured as wild digits (0xFF) */
          for(gid_index = 0;
              gid_index < category_data_ptr->code_data_ptr[code_index].gid1.gid_len && gid_index < SIMLOCK_MAX_GID_LEN;
              gid_index++)
          {
            if(category_data_ptr->code_data_ptr[code_index].gid1.gid_data[gid_index] != SIMLOCK_LOCK_CODE_WILD_DIGIT)
            {
              break;
            }
          }

          /* If all the GID1 bytes are configured as wild digits then no need to check with SIM's GID data */
          if(gid_index == category_data_ptr->code_data_ptr[code_index].gid1.gid_len)
          {
            if(category_data_ptr->code_data_ptr[code_index].code_type == SIMLOCK_CODE_WHITELIST)
            {
              code_in_list = SIMLOCK_CODE_IN_WHITELIST;
              code_in_whitelist = TRUE;
            }
            if(category_data_ptr->code_data_ptr[code_index].code_type == SIMLOCK_CODE_BLACKLIST)
            {
              code_in_list = SIMLOCK_CODE_IN_BLACKLIST;
              code_in_blacklist = TRUE;
            }
          }
        }
      }
    }
  }

  /* If autolock is enabled and,
     - no codes were present during locking process, lock the device to
       the current sim data.
     - a valid list (whitelist) of codes were present during the locking
       process, the intention is to auto lock the device to the current sim
       data if matches with any of the codes present in the whitelist of
       auto-lock codes.
     In both these cases, we update the lock code data in the category data
     file */
  if(category_data_ptr->category_header.auto_lock  &&
     (category_data_ptr->category_header.num_of_codes == 0 ||
      code_in_whitelist))
  {
    simlock_category_code_data_type    sim_code_data;

    SIMLOCK_MSG_MED_0("autolock enabled, updating code data");

    simlock_category_initialize_code_data(&sim_code_data);

    /* Copy the sim_subs_data to sim_code_data to write to file */
    sim_code_data.mnc         = sim_subs_data_ptr->mnc;
    sim_code_data.gid1        = sim_subs_data_ptr->gid1;
    sim_code_data.msin        = sim_subs_data_ptr->msin;
    memscpy(sim_code_data.mcc, sizeof(sim_code_data.mcc),
            sim_subs_data_ptr->mcc, sizeof(sim_subs_data_ptr->mcc));

    /* Write the lock codes from SIM */
    simlock_status = simlock_category_update_and_write_lock_data(&category_data_ptr->category_header,
                                                                 &sim_code_data,
                                                                 slot,
                                                                 SIMLOCK_SFS_LOCAL);
    code_in_list = (simlock_status == SIMLOCK_SUCCESS ? SIMLOCK_CODE_IN_WHITELIST : SIMLOCK_CODE_IN_NOLIST);
  }

  if(code_in_whitelist && code_in_blacklist)
  {
    code_in_list = SIMLOCK_CODE_IN_WHITELIST_AND_BLACKLIST;
  }

  SIMLOCK_MSG_MED_1("ns_sp code in list type: 0x%x", code_in_list);
  return code_in_list;
} /* simlock_3gpp_run_algorithm_ns_sp */


/*===========================================================================
/*===========================================================================
FUNCTION SIMLOCK_3GPP_RUN_ALGORITHM

DESCRIPTION
  This function validates the 3GPP subscription on a card by verifying the
  card data against the lock data. It checks the codes for each enabled
  category for validating the subscription. Also, it checks if a code is
  categorized as a blacklist or whitelist and accordingly updates the
  validity of the subscription.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_3gpp_run_algorithm
(
  const simlock_subscription_data_type    * sim_data_ptr,
  simlock_slot_enum_type                    slot,
  const simlock_config_data_type          * config_data_ptr,
  simlock_subscription_validity_info_type * subs_validity_ptr
)
{
  simlock_result_enum_type                simlock_status          = SIMLOCK_SUCCESS;
  simlock_category_enum_type              category                = SIMLOCK_CATEGORY_3GPP_NW;
  simlock_slot_policy_enum_type           slot_policy             = SIMLOCK_SLOT_POLICY_SAME_ON_ALL_SLOTS;
  simlock_decoded_subscription_data_type *sim_subs_data_ptr       = NULL;
  uint32                                  num_max_retries         = 0;
  uint32                                  current_retries         = 0;
  uint8                                   ehplmn_index            = 0;
  simlock_sfs_enum_type                   sfs_type                = SIMLOCK_SFS_LOCAL;

  if((sim_data_ptr == NULL)    ||
     (config_data_ptr == NULL) ||
     (subs_validity_ptr == NULL))
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  /* check the slot */
  if(simlock_util_check_slot(slot) != SIMLOCK_SUCCESS)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_MSG_MED_0("simlock_3gpp_run_algorithm");

  /* check the slot policy */
  slot_policy = config_data_ptr->slot_policy;

  /* if the slot policy is same on all the available slots or valid card must
     on any slot, block all slots if all valid slots expired, full service on
     any valid slot or IMSI match then it is expected to have one category
     file per each enabled category i.e. all the codes for that category are
     in one file */
  if(simlock_policy_uses_slot1_config_for_all_slots(slot_policy) != FALSE)
  {
    slot = SIMLOCK_SLOT_1;
  }

  /* if the device is in emergency only mode, no subscription is valid */
  if(simlock_util_is_device_in_emergency_mode(config_data_ptr))
  {
    subs_validity_ptr->is_subscription_valid = FALSE;
    subs_validity_ptr->failure_reason        = SIMLOCK_SUBSCRIPTION_INVALID_REASON_EMERGENCY_ONLY;
    return SIMLOCK_SUCCESS;
  }

  SIMLOCK_CHECK_AND_MEM_MALLOC(sim_subs_data_ptr, sizeof(simlock_decoded_subscription_data_type));
  if (sim_subs_data_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  simlock_status = simlock_3gpp_parse_imsi(sim_data_ptr->imsi,
                                           sim_data_ptr->ad,
                                           sim_subs_data_ptr);
  if(simlock_status != SIMLOCK_SUCCESS)
  {
    SIMLOCK_MSG_ERR_0("failed to parse imsi");
    SIMLOCK_MEM_FREE(sim_subs_data_ptr);
    return SIMLOCK_GENERIC_ERROR;
  }

  simlock_status = simlock_3gpp_parse_iccid(&sim_data_ptr->iccid,
                                            &sim_subs_data_ptr->iccid);
  if(simlock_status != SIMLOCK_SUCCESS)
  {
    SIMLOCK_MSG_HIGH_0("failed to parse ICCID");
  }

  /* initialize the failure reason as none */
  subs_validity_ptr->failure_reason = SIMLOCK_SUBSCRIPTION_INVALID_REASON_NONE;

  /* check if the MCC/MNC belongs to the
     test MCC/MNC */
  if(simlock_util_check_is_test_mcc_mnc(sim_subs_data_ptr->mcc,
                                        &(sim_subs_data_ptr->mnc)))
  {
    subs_validity_ptr->is_subscription_valid = TRUE;
    SIMLOCK_MEM_FREE(sim_subs_data_ptr);
    return SIMLOCK_SUCCESS;
  }

  sim_subs_data_ptr->gid1 = sim_data_ptr->gid1;
  sim_subs_data_ptr->gid2 = sim_data_ptr->gid2;
  sim_subs_data_ptr->spn  = sim_data_ptr->spn;

  /* The card should be configured to have a valid EHPLMN data. If not,
     then don't populate the EHPLMN data */
  if ((sim_data_ptr->ehplmn.ehplmn_len > 0)        &&
      ((sim_data_ptr->ehplmn.ehplmn_len % 3) == 0))
  {
    for(ehplmn_index = 0;
        (ehplmn_index < sim_data_ptr->ehplmn.ehplmn_len / SIMLOCK_PLMN_ID_LEN) &&
        (ehplmn_index < SIMLOCK_EHPLMN_NUM_MAX);
        ehplmn_index++)
    {
      simlock_3gpp_extract_plmn_from_ehplmn_data(sim_data_ptr, sim_subs_data_ptr, ehplmn_index);
    }
    sim_subs_data_ptr->ehplmn.num_ehplmns = ehplmn_index;
  }

  /* set the subscription validity to TRUE. In case, none of the categories
       on a slot are enabled, the subscription is valid */
  subs_validity_ptr->is_subscription_valid = TRUE;

  for(category = SIMLOCK_CATEGORY_3GPP_NW; category <= SIMLOCK_CATEGORY_3GPP_SIM;
      category++)
  {
    simlock_code_in_list_enum_type    code_in_list        = SIMLOCK_CODE_IN_NOLIST;
    simlock_category_file_data_type   category_file_data;

    memset(&category_file_data, 0x00,
            sizeof(simlock_category_file_data_type));

    /* if the category is not enabled, nothing to do */
    if(!simlock_util_check_category_enabled(config_data_ptr, category, slot))
    {
      continue;
    }

    /* if the category is temporarily unlocked, continue */
    if(simlock_timer_check_is_temporary_unlock_set(slot, category))
    {
      continue;
    }

    SIMLOCK_MSG_HIGH_1("category 0x%x enabled", category);

    sfs_type = simlock_config_read_category_sfs_type(config_data_ptr, category, slot);

    /* read the category data */
    simlock_status = simlock_category_read_lock_data(category,
                                                     slot,
                                                     &category_file_data,
                                                     sfs_type,
                                                     SIMLOCK_CONFIG_ID_DEFAULT);
    if(simlock_status != SIMLOCK_SUCCESS)
    {
      SIMLOCK_MSG_ERR_3("read lock data failed, category: 0x%x,"
                        " slot: 0x%x, sfs_type: 0x%x", category, slot, sfs_type);
      subs_validity_ptr->is_subscription_valid = FALSE;
      subs_validity_ptr->failed_category = category;

      /* this frees the code data in the category */
      simlock_category_free_code_data(&category_file_data);
      break;
    }

    switch(category)
    {
      case SIMLOCK_CATEGORY_3GPP_NW:
        code_in_list = simlock_3gpp_run_algorithm_nw(sim_subs_data_ptr, slot, &category_file_data);
        break;

      case SIMLOCK_CATEGORY_3GPP_NS:
        code_in_list = simlock_3gpp_run_algorithm_ns(sim_subs_data_ptr, slot, &category_file_data);
        break;

      case SIMLOCK_CATEGORY_3GPP_SP:
        code_in_list = simlock_3gpp_run_algorithm_sp(sim_subs_data_ptr, slot, &category_file_data);
        break;

      case SIMLOCK_CATEGORY_3GPP_CP:
        code_in_list = simlock_3gpp_run_algorithm_cp(sim_subs_data_ptr, slot, &category_file_data);
        break;

      case SIMLOCK_CATEGORY_3GPP_SIM:
        code_in_list = simlock_3gpp_run_algorithm_sim(sim_subs_data_ptr, slot, &category_file_data);
        break;

      case SIMLOCK_CATEGORY_3GPP_SPN:
        code_in_list = simlock_3gpp_run_algorithm_spn(sim_subs_data_ptr, slot, &category_file_data);
        break;

      case SIMLOCK_CATEGORY_3GPP_SP_EHPLMN:
        code_in_list = simlock_3gpp_run_algorithm_sp_ehplmn(sim_subs_data_ptr, slot, &category_file_data);
        break;

      case SIMLOCK_CATEGORY_3GPP_ICCID:
        code_in_list = simlock_3gpp_run_algorithm_iccid(sim_subs_data_ptr, slot, &category_file_data);
        break;

      case SIMLOCK_CATEGORY_3GPP_NS_SP:
        code_in_list = simlock_3gpp_run_algorithm_ns_sp(sim_subs_data_ptr, slot, &category_file_data);
        break;

      default:
        break;
    }

    num_max_retries = category_file_data.category_header.num_retries_max;
    current_retries = category_file_data.category_header.curr_retries;

    /* If blacklisted codes for this category matched, then no need to check
       other categories. This is because if more than one category is enabled
       as blacklist, then for a subscription to be valid, the codes of the all
       the enabled blacklisted categories should not match */
    if(code_in_list == SIMLOCK_CODE_IN_BLACKLIST)
    {
      subs_validity_ptr->is_subscription_valid = FALSE;
      subs_validity_ptr->failed_category = category;

      /* this frees the code data in the category */
      simlock_category_free_code_data(&category_file_data);
      break;
    }

    /* check if the code is whitelist. If yes and the codes for this category
       didn't match, then no need to check other categories. This is because
       if more than one category is enabled, then for a subscription to be
       valid, the codes of the all the enabled categories should match */
    if(simlock_category_is_whitelist(&category_file_data.category_header) &&
       (code_in_list == SIMLOCK_CODE_IN_NOLIST))
    {
      subs_validity_ptr->is_subscription_valid = FALSE;
      subs_validity_ptr->failed_category = category;

      /* this frees the code data in the category */
      simlock_category_free_code_data(&category_file_data);
      break;
    }

    /* check if the overall code indicator is blacklist. If yes and codes for
       this category matched in white and blacklist, then no need to check
       other categories */
    if(!simlock_category_is_whitelist(&category_file_data.category_header) &&
       (code_in_list == SIMLOCK_CODE_IN_WHITELIST_AND_BLACKLIST))
    {
      subs_validity_ptr->is_subscription_valid = FALSE;
      subs_validity_ptr->failed_category = category;

      /* this frees the code data in the category */
      simlock_category_free_code_data(&category_file_data);
      break;
    }

    /* this frees the code data in the category */
    simlock_category_free_code_data(&category_file_data);
  }

  SIMLOCK_MEM_FREE(sim_subs_data_ptr);

  /* If the subscription is not valid, check if the CK is present or not */
  if(!subs_validity_ptr->is_subscription_valid)
  {
    simlock_ck_present_type     ck_status = SIMLOCK_CK_PRESENT;

    subs_validity_ptr->failure_reason = SIMLOCK_SUBSCRIPTION_INVALID_REASON_LOCKED_WITH_CK;

    if(simlock_util_check_ck_status(config_data_ptr,
                                    subs_validity_ptr->failed_category,
                                    slot,
                                    &ck_status) == SIMLOCK_SUCCESS)
    {
      if(ck_status == SIMLOCK_CK_NOT_PRESENT)
      {
        subs_validity_ptr->failure_reason = SIMLOCK_SUBSCRIPTION_INVALID_REASON_LOCKED_WITHOUT_CK;
      }
      else
      {
        subs_validity_ptr->num_retries_max = num_max_retries;
        subs_validity_ptr->curr_retries = current_retries;
      }
    }
  }

  return simlock_status;
} /* simlock_run_algorithm_3gpp */

#endif /* FEATURE_SIMLOCK */

