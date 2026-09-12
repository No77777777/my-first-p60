/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


            S I M   L O C K   3 G P P 2   F U N C T I O N S


GENERAL DESCRIPTION

  This source file contains the simlock functions.


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

$Header: //components/rel/uim.mpss/6.1.0/simlock/src/simlock_3gpp2.c#3 $$ $DateTime: 2019/12/23 02:54:10 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/23/19   tq      Check wild digit only for lock data 
09/13/19   vdc     SimLock solution for activation at Point Of Sale
12/03/18   vgd     Remote SIM Unlock support for SIMLOCK_RSU_MODE_D
08/11/17   av      Pass structure by reference to avoid compiler issues
01/26/17   bcho    Support for slot policy valid card must on all slots
01/19/17   vdc     Added stub file for simlock 3GPP2 functions
01/09/17   nr      Improve logging and pass parameters by value in simlock_3gpp2_decode_imsi ()
12/20/16   vdc     Support for simlock policy wait for full srv or imsi matched
10/28/16   nr      Support remote and local configurations together
10/21/16   ar      Move local variable to heap to reduce stack usage
10/06/16   ar      Added support for SP + EHPLMN simlock
09/02/16   vdc     Extend full and expired service policies on any slot
08/25/16   vdc     Apply same config for VALID_CARD_MUST_ON_ANY_SLOT policy
08/10/16   av      Reduce F3 logging in simlock
01/22/16   av      Support for autolock whitelist
06/08/15   stv     Populate retry values in run algo response
02/17/15   vv      Indicate the reason for invalid subscription
01/21/15   vv      Remove check for the IMSI class
01/06/15   vv      Support for identifying test SIMs
12/30/14   bcho    Support for more then 1 bytes of GID1/GID2
12/29/14   yt      Resolve compiler warnings
12/26/14   vv      Support for whitelist and blacklist codes in same category
09/29/14   vv      Added support for emergency only mode
08/29/14   vv      Added support for temporary unlock
04/14/14   vv      Added support for identifying conflicting codes
02/14/14   vv      Initial revision
===========================================================================*/


/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "customer.h"

#ifdef FEATURE_SIMLOCK

#include "simlock_3gpp2.h"
#include "simlock_util.h"
#include "simlock_category.h"
#include "simlock_timer.h"
#include "simlock_platform.h"
#include "simlock_policy.h"

#define SIMLOCK_PREFIX_MASK        0x00FFC000
#define SIMLOCK_THOUSANDS_MASK     0x00003C00
#define SIMLOCK_LAST_3_MASK        0x000003FF

#define SIMLOCK_PREFIX_SHIFT       14
#define SIMLOCK_THOUSANDS_SHIFT    10
#define SIMLOCK_LAST_3_SHIFT       0

const uint8 simlock_bcd_to_num[] =
{ 0xFF, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

const uint8 simlock_min_to_num[] =
{ '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' };


/*===========================================================================
FUNCTION SIMLOCK_3GPP2_DECODE_IMSI

DESCRIPTION
  This function decodes the mcc, mnc and msin

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_result_enum_type simlock_3gpp2_decode_imsi
(
 uint16                                     in_mcc,
 uint8                                      in_mnc,
 uint32                                     in_min1,
 uint16                                     in_min2,
 simlock_decoded_subscription_data_type   * out_data_ptr
)
{
  uint8   digit        = 0;

  if(out_data_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  if(in_mcc <= 999)
  {
    /* Converting Mobile Country Code */
    digit = simlock_min_to_num[in_mcc/100];
    out_data_ptr->mcc[0] = digit;
    in_mcc %= 100;

    digit = simlock_min_to_num[in_mcc / 10];
    out_data_ptr->mcc[1] = digit;

    digit = simlock_min_to_num[in_mcc % 10];
    out_data_ptr->mcc[2] = digit;
  }
  else
  {
#ifndef FEATURE_UIM_TEST_FRAMEWORK
    /* Error in converting Mobile Country Code of IMSI */
    return SIMLOCK_GENERIC_ERROR;
#endif /* !FEATURE_UIM_TEST_FRAMEWORK */
  }

  if(in_mnc <= 99)
  {
    /* Convert Mobile Network Code */
    digit = simlock_min_to_num[in_mnc / 10];
    out_data_ptr->mnc.mnc_data[0] = digit;

    digit = simlock_min_to_num[in_mnc % 10];
    out_data_ptr->mnc.mnc_data[1] = digit;

    out_data_ptr->mnc.mnc_len = 2;
  }
  else
  {
#ifndef FEATURE_UIM_TEST_FRAMEWORK
    /* Error in converting Mobile Network Code of IMSI */
    return SIMLOCK_GENERIC_ERROR;
#endif /* !FEATURE_UIM_TEST_FRAMEWORK */
  }

  if (in_min2 <= 999)
  {
    /* Convert First Three Digits of IMSI */
    digit = simlock_min_to_num[in_min2 / 100];
    out_data_ptr->msin.msin_data[0] = digit;
    in_min2 %= 100;

    digit = simlock_min_to_num[in_min2 / 10];
    out_data_ptr->msin.msin_data[1] = digit;

    digit = simlock_min_to_num[in_min2 % 10];
    out_data_ptr->msin.msin_data[2] = digit;

    out_data_ptr->msin.msin_len = 3;
  }
  else
  {
#ifndef FEATURE_UIM_TEST_FRAMEWORK
    /* Error in converting First 3 Digits of IMSI */
    return SIMLOCK_GENERIC_ERROR;
#endif /* !FEATURE_UIM_TEST_FRAMEWORK */
  }

  if (in_min1 > 0)
  {
    uint16  second_three = 0;
    uint16  last_three   = 0;
    uint8   thousands    = 0;

    /* Convert MIN 1 Portion of IMSI */
    second_three = (uint16)((in_min1 & SIMLOCK_PREFIX_MASK) >> SIMLOCK_PREFIX_SHIFT);
    thousands = (uint16)((in_min1 & SIMLOCK_THOUSANDS_MASK) >> SIMLOCK_THOUSANDS_SHIFT);
    last_three = (uint16)((in_min1 & SIMLOCK_LAST_3_MASK) >> SIMLOCK_LAST_3_SHIFT);

    thousands = simlock_bcd_to_num[thousands];
    if ( (thousands != 0xFF)
    &&   (second_three <= 999)
    &&   (last_three <= 999) )
    {
      /* IMSI data is of the right format */
      digit = simlock_min_to_num[second_three / 100];
      out_data_ptr->msin.msin_data[3] = digit;

      second_three %= 100;

      digit = simlock_min_to_num[second_three / 10];
      out_data_ptr->msin.msin_data[4] = digit;

      digit = simlock_min_to_num[second_three % 10];
      out_data_ptr->msin.msin_data[5] = digit;

      out_data_ptr->msin.msin_data[6] = thousands;

      digit = simlock_min_to_num[last_three / 100];
      out_data_ptr->msin.msin_data[7] = digit;

      last_three %= 100;

      digit = simlock_min_to_num[last_three / 10];
      out_data_ptr->msin.msin_data[8] = digit;

      digit = simlock_min_to_num[last_three % 10];
      out_data_ptr->msin.msin_data[9] = digit;

      out_data_ptr->msin.msin_len = 10;
    } /* end if - last 7 digits of IMSI are correct format */
  } /* end if MIN 1 is a valid number */

  return SIMLOCK_SUCCESS;
} /* simlock_3gpp2_decode_imsi */


/*===========================================================================
FUNCTION SIMLOCK_3GPP2_PARSE_IMSI

DESCRIPTION
  This function parses the 3GPP2 IMSI from the card and provides the data in
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
static simlock_result_enum_type simlock_3gpp2_parse_imsi
(
  simlock_imsi_type                           imsi_data,
  simlock_decoded_subscription_data_type    * out_data_ptr
)
{
  simlock_result_enum_type  simlock_status    = SIMLOCK_SUCCESS;
  uint32                    min1_in_bcd       = 0;
  uint16                    mcc_in_bcd        = 0;
  uint16                    min2_in_bcd       = 0;
  uint8                     mnc_in_bcd        = 0;

  if(out_data_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  if(imsi_data.imsi_len != 10)
  {
    return SIMLOCK_GENERIC_ERROR;
  }

  mcc_in_bcd = (uint16)(((imsi_data.imsi[9] & 0x03) << 8) +
                        imsi_data.imsi[8]);

  mnc_in_bcd = (imsi_data.imsi[6] & 0x7F);

  min1_in_bcd = (uint32)((imsi_data.imsi[5] << 16) +
                         (imsi_data.imsi[4] << 8)  +
                         (imsi_data.imsi[3]));

  min2_in_bcd = (uint16)((imsi_data.imsi[2] << 8) +
                         (imsi_data.imsi[1]));

  SIMLOCK_MSG_HIGH_4("mcc_in_bcd 0x%x mnc_in_bcd 0x%x min1_in_bcd 0x%x min2_in_bcd 0x%x",
                      mcc_in_bcd, mnc_in_bcd, min1_in_bcd, min2_in_bcd);

  simlock_status = simlock_3gpp2_decode_imsi(mcc_in_bcd,
                                             mnc_in_bcd,
                                             min1_in_bcd,
                                             min2_in_bcd,
                                             out_data_ptr);
  if(simlock_status != SIMLOCK_SUCCESS)
  {
    return SIMLOCK_GENERIC_ERROR;
  }
  return SIMLOCK_SUCCESS;
} /* simlock_3gpp2_parse_imsi */


/*===========================================================================
FUNCTION SIMLOCK_3GPP2_RUN_ALGORITHM_RUIM

DESCRIPTION
  This function validates the 3GPP2 RUIM code from the card and the
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
static simlock_code_in_list_enum_type simlock_3gpp2_run_algorithm_ruim
(
  const simlock_decoded_subscription_data_type  * sim_subs_data_ptr,
  simlock_slot_enum_type                          slot,
  simlock_category_file_data_type               * category_data_ptr
)
{
  simlock_result_enum_type           simlock_status    = SIMLOCK_SUCCESS;
  uint32                             code_index        = 0;
  simlock_code_in_list_enum_type     code_in_list      = SIMLOCK_CODE_IN_NOLIST;
  boolean                            code_in_whitelist = FALSE;
  boolean                            code_in_blacklist = FALSE;

  if(category_data_ptr == NULL)
  {
    return code_in_list;
  }

  SIMLOCK_MSG_LOW_6("run_algo_ruim: From sim - mcc[0],mcc[1],mcc[2]: 0x%x,0x%x,0x%x; mnc[0],mnc[1],mnc[2]: 0x%x,0x%x,0x%x;",
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

  /* Compare MCCs/MNCs from the file and IMSI */
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

  SIMLOCK_MSG_MED_1("ruim code in list type: 0x%x", code_in_list);
  return code_in_list;
} /* simlock_3gpp2_run_algorithm_ruim */


/*===========================================================================
FUNCTION SIMLOCK_3GPP2_RUN_ALGORITHM_NW_TYPE2

DESCRIPTION
  This function validates the 3GPP2 NW type2 code from the card and the
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
static simlock_code_in_list_enum_type simlock_3gpp2_run_algorithm_nw_type2
(
  const simlock_decoded_subscription_data_type  * sim_subs_data_ptr,
  simlock_slot_enum_type                          slot,
  simlock_category_file_data_type               * category_data_ptr
)
{
  simlock_result_enum_type           simlock_status    = SIMLOCK_SUCCESS;
  uint32                             code_index        = 0;
  simlock_code_in_list_enum_type     code_in_list      = SIMLOCK_CODE_IN_NOLIST;
  boolean                            code_in_whitelist = FALSE;
  boolean                            code_in_blacklist = FALSE;

  if(category_data_ptr == NULL)
  {
    return code_in_list;
  }

  SIMLOCK_MSG_LOW_4("run_algo_nw_type2: From sim - irm[0],irm[1],irm[2],irm[3]: 0x%x,0x%x,0x%x,0x%x",
                    sim_subs_data_ptr->msin.msin_data[0],
                    sim_subs_data_ptr->msin.msin_data[1],
                    sim_subs_data_ptr->msin.msin_data[2],
                    sim_subs_data_ptr->msin.msin_data[3]);

  /* if autolock is not enabled, atleast one valid code is expected */
  if(!category_data_ptr->category_header.auto_lock &&
     (category_data_ptr->category_header.num_of_codes == 0 ||
      category_data_ptr->code_data_ptr == NULL))
  {
    SIMLOCK_MSG_ERR_0("no valid code data");
    return code_in_list;
  }

  /* Compare IRM from the file and IMSI */
  for (code_index = 0; code_index < category_data_ptr->category_header.num_of_codes;
       code_index++)
  {
    if(simlock_util_check_if_irm_matches(&(category_data_ptr->code_data_ptr[code_index].msin),
                                         &(sim_subs_data_ptr->msin),
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

    /* First 4 bytes of MSIN data is required. */
    sim_code_data.msin        = sim_subs_data_ptr->msin;

    sim_code_data.msin.msin_len = SIMLOCK_IRM_LEN;
    memset(&sim_code_data.msin.msin_data[SIMLOCK_IRM_LEN], 0x7F,
            SIMLOCK_MSIN_MAX - SIMLOCK_IRM_LEN);

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

  SIMLOCK_MSG_MED_1("nw_type2 code in list type: 0x%x", code_in_list);
  return code_in_list;
} /* simlock_3gpp2_run_algorithm_nw_type2 */


/*===========================================================================
FUNCTION SIMLOCK_3GPP2_RUN_ALGORITHM_NW_TYPE1

DESCRIPTION
  This function validates the 3GPP2 NW type1 code from the card and the
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
static simlock_code_in_list_enum_type simlock_3gpp2_run_algorithm_nw_type1
(
  const simlock_decoded_subscription_data_type  * sim_subs_data_ptr,
  simlock_slot_enum_type                          slot,
  simlock_category_file_data_type               * category_data_ptr
)
{
  simlock_result_enum_type           simlock_status    = SIMLOCK_SUCCESS;
  uint32                             code_index        = 0;
  simlock_code_in_list_enum_type     code_in_list      = SIMLOCK_CODE_IN_NOLIST;
  boolean                            code_in_whitelist = FALSE;
  boolean                            code_in_blacklist = FALSE;

  if(category_data_ptr == NULL)
  {
    return code_in_list;
  }

  SIMLOCK_MSG_LOW_6("run_algo_nw_type1: From sim - mcc[0]:,mcc[1],mcc[2]: 0x%x,0x%x,0x%x; mnc[0]:,mnc[1],mnc[2]: 0x%x,0x%x,0x%x",
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

  SIMLOCK_MSG_MED_1("nw_type1 code in list type: 0x%x", code_in_list);
  return code_in_list;
} /* simlock_3gpp2_run_algorithm_nw_type1 */


/*===========================================================================
FUNCTION SIMLOCK_3GPP2_RUN_ALGORITHM

DESCRIPTION
  This function validates the 3GPP2 subscription on a card by verifying the
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
simlock_result_enum_type simlock_3gpp2_run_algorithm
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
  simlock_sfs_enum_type                   sfs_type                = SIMLOCK_SFS_LOCAL;

  if((sim_data_ptr == NULL)    ||
     (config_data_ptr == NULL) ||
     (subs_validity_ptr == NULL))
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  if(simlock_util_check_slot(slot) != SIMLOCK_SUCCESS)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_MSG_MED_0("simlock_3gpp2_run_algorithm");

  slot_policy = config_data_ptr->slot_policy;
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

  simlock_status = simlock_3gpp2_parse_imsi(sim_data_ptr->imsi,
                                            sim_subs_data_ptr);
  if(simlock_status != SIMLOCK_SUCCESS)
  {
    SIMLOCK_MSG_ERR_0("failed to parse imsi");
    SIMLOCK_MEM_FREE(sim_subs_data_ptr);
    return SIMLOCK_GENERIC_ERROR;
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

  /* set the subscription validity to TRUE. In case, none of the categories
         on a slot are enabled, the subscription is valid */
  subs_validity_ptr->is_subscription_valid = TRUE;

  for(category = SIMLOCK_CATEGORY_3GPP2_NW_TYPE1;
      category <= SIMLOCK_CATEGORY_3GPP2_RUIM; category++)
  {
    simlock_code_in_list_enum_type    code_in_list       = SIMLOCK_CODE_IN_NOLIST;
    simlock_category_file_data_type   category_file_data;

    memset(&category_file_data, 0x00,
          sizeof(simlock_category_file_data_type));

    /* not all 3gpp2 categories are supported */
    simlock_status = simlock_util_check_category_supported(category);
    if(simlock_status == SIMLOCK_UNSUPPORTED)
    {
      continue;
    }

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
      case SIMLOCK_CATEGORY_3GPP2_NW_TYPE1:
        code_in_list = simlock_3gpp2_run_algorithm_nw_type1(sim_subs_data_ptr, slot, &category_file_data);
        break;

      case SIMLOCK_CATEGORY_3GPP2_NW_TYPE2:
        code_in_list = simlock_3gpp2_run_algorithm_nw_type2(sim_subs_data_ptr, slot, &category_file_data);
        break;

      case SIMLOCK_CATEGORY_3GPP2_RUIM:
        code_in_list = simlock_3gpp2_run_algorithm_ruim(sim_subs_data_ptr, slot, &category_file_data);
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
} /* simlock_3gpp2_run_algorithm */

#endif /* FEATURE_SIMLOCK */

