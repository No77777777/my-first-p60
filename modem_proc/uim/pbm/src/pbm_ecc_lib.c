/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                P B M _ E C C _ L I B.C

GENERAL DESCRIPTION
  This file contains the APIs exported by PBM service which is executed as a
  library within the PBM client's task.

  Copyright (c) 2016 - 2017, 2021 - 2022 by QUALCOMM Technologies, Inc(QTI).
  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/pbm/src/pbm_ecc_lib.c#3 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/19/22   skc     Indicate source of emergency number 
07/29/21   shp     Support serving MCC based ECC number for china
09/06/17   tq      Relocate the special emergency number check
08/24/17   tq      Fix KW error
06/08/17   nr      Deprecate legacy APIs
04/11/17   nr      EM call handling on sub when voice call going on other sub
04/07/17   nr      Reset out params in pbm ecc lib api for all slots
03/16/17   nr      Treat numbers recieved in TAU accept as emergency
01/07/17   nr      Stop ECC lookup as soon as it is resolved as NAL number
11/02/16   nr      OTA emergency numbers are highest priority than other no's
09/10/16   nr      NAL China emergency requirement
06/20/16   nr      ECC number checking on card session when SUB is inactive
06/15/16   nr      Deprecate legacy APIs
05/19/16   nr      Logging improvements
04/18/16   sp      Intial Version
===========================================================================*/
#include "customer.h"
#include "uim_msg.h"
#include "pbm.h"
#include "pbmlib.h"
#include "pbmutils.h"
#include "pbmcache.h"
#include "pbmuim.h"
#include "pbmgsdi.h"
#include "pbmtask_i.h"
#include "sys.h"

#include "stdlib.h"
#include "stringl/stringl.h"

/*===========================================================================
                         DEFINITIONS
===========================================================================*/

/*===========================================================================
  FUNCTION PBM_SESSION_EMERGENCY_NUMBER_CAT_EXT2

  DESCRIPTION
    This function searches the ECC phonebook for the number just passed in
    to determine if the current number is an Emergency number.

  DEPENDENCIES

  RETURN VALUE
    pbm_return_type - PBM_SUCCESS

  SIDE EFFECTS
    none
===========================================================================*/
pbm_return_type pbm_session_emergency_number_cat_ext2(
  pbm_ecc_call_resolution_type   *pbm_ecc_call_resolution_ptr
)
{
  pbm_session_enum_type    l_session_type                = PBM_SESSION_MAX;
  pbm_record_id_type       rec_id                        = PBM_INVALID_REC_ID;
  pbm_phonebook_type       pb_id                         = {PBM_GPB};
  pbm_return_type          ret_val                       = PBM_ERROR;
  /* field found in the current record */
  pbm_field_id_e_type      found_field                   = PBM_FIELDID_NONE;
  uint32                   best_match_index_gw;
  /* index into priority array of best category so far */
  uint32                   best_match_index_1x;
  uint32                   i                             = 0;
  /* category found in the current record */
  uint8                    found_category                = 0;
  uint8                    handle                        = 0;
  boolean                  found_on_slot[PBM_NUM_SLOTS]  = {FALSE};

  /* Rel 11 specs say to prefer categories in network over sim, and SIM over others
   * NV aren't spec'd, so they go to the bottom */
  pbm_field_id_e_type ecc_order_preference[] =
  {
    PBM_FIELD_NETWORK_ECC,
    PBM_FIELD_SIM_ECC,
    PBM_FIELD_SERVING_MCC_HARDCODED_ECC,
    PBM_FIELD_HARDCODED_ECC,
    PBM_FIELD_NV_ECC
  };

  /* specs say to prefer categories in SIM over network, and network over others
   * NV aren't spec'd, so they go to the bottom */
  pbm_field_id_e_type ecc_order_preference_pre_rel11[] =
  {
    PBM_FIELD_SIM_ECC,
    PBM_FIELD_NETWORK_ECC,
    PBM_FIELD_SERVING_MCC_HARDCODED_ECC,
    PBM_FIELD_HARDCODED_ECC,
    PBM_FIELD_NV_ECC
  };
  byte my_ph_num[PBM_MAX_NUM_LENGTH+1]; //plus null char

  PBM_CHECK_PTR2_RET(pbm_ecc_call_resolution_ptr, pbm_ecc_call_resolution_ptr->num, PBM_ERROR);

  memset(my_ph_num, 0, sizeof(my_ph_num));
  memscpy(my_ph_num, sizeof(my_ph_num), pbm_ecc_call_resolution_ptr->num, pbm_ecc_call_resolution_ptr->len);

  UIM_MSG_HIGH_8("Checking num as emergency. Len:0x%x. Num:0x%x 0x%x 0x%x 0x%x 0x%x pbm_3gpp_rel_ver 0x%x p_session_type 0x%x",
                 pbm_ecc_call_resolution_ptr->len,
                 (pbm_ecc_call_resolution_ptr->len > 0 ? my_ph_num[0]:-1),
                 (pbm_ecc_call_resolution_ptr->len > 1 ? my_ph_num[1]:-1),
                 (pbm_ecc_call_resolution_ptr->len > 2 ? my_ph_num[2]:-1),
                 (pbm_ecc_call_resolution_ptr->len > 3 ? my_ph_num[3]:-1),
                 (pbm_ecc_call_resolution_ptr->len > 4 ? my_ph_num[4]:-1),
                 pbm_3gpp_rel_ver,
                 pbm_ecc_call_resolution_ptr->p_session_type);

  pbm_ecc_call_resolution_ptr->ecc_category_ptr_gw = 0;
  pbm_ecc_call_resolution_ptr->ecc_category_ptr_1x = 0;
  pbm_ecc_call_resolution_ptr->found_in_gw = FALSE;
  pbm_ecc_call_resolution_ptr->found_in_1x = FALSE;
  pbm_ecc_call_resolution_ptr->special_emergency_number = FALSE;
  pbm_ecc_call_resolution_ptr->special_emergency_number_in_serving_mcc = FALSE;
  pbm_ecc_call_resolution_ptr->special_emergency_number_subs_mask = 0;

  if (pbm_3gpp_rel_ver < LTE_3GPP_REL11)
  {
    memscpy( ecc_order_preference , ARR_SIZE(ecc_order_preference) ,
             ecc_order_preference_pre_rel11 , ARR_SIZE(ecc_order_preference_pre_rel11));
  }
  best_match_index_gw = ARR_SIZE(ecc_order_preference);
  best_match_index_1x = ARR_SIZE(ecc_order_preference);

  if (pbm_ecc_call_resolution_ptr->len == 0 || pbm_ecc_call_resolution_ptr->len > PBM_MAX_NUM_LENGTH)
  {
    return PBM_SUCCESS;
  }
  if (pbm_ecc_call_resolution_ptr->p_session_type < PBM_SESSION_GPB_1 ||
      pbm_ecc_call_resolution_ptr->p_session_type >= PBM_SESSION_MAX)
  {
    return PBM_ERROR;
  }

  for(l_session_type = PBM_SESSION_GPB_1; l_session_type < PBM_SESSION_MAX; l_session_type++)
  {
    if(PBM_LPB != pbm_session_type_to_pb_category(l_session_type))
    {
      continue;
    }

    /* only check in RATs which belong to the requested slot.*/
    if(pbm_session_info[l_session_type].slot_id != pbm_session_info[pbm_ecc_call_resolution_ptr->p_session_type].slot_id )
    {
      continue;
    }

    pb_id = pbm_uim_device_type_to_phonebook_type((pbm_uim_app_enum_type)l_session_type,PBM_ECC);

    /* If no valid slot_id associated with provisioning sessions means
       either card is in detected state or in card error state */
    if(!(pbm_session_info[pbm_ecc_call_resolution_ptr->p_session_type].slot_id >= PBM_SLOT_1 &&
         pbm_session_info[pbm_ecc_call_resolution_ptr->p_session_type].slot_id < PBM_NUM_SLOTS))
    {
      pbm_slot_type            slot           = PBM_SLOT_1;
      pbm_session_enum_type    card_session   = PBM_SESSION_DEFAULT;

      for(slot = PBM_SLOT_1; slot < PBM_NUM_SLOTS; slot++)
      {
        if(!found_on_slot[slot] && pbm_is_card_present_in_slot(slot))
        {
          found_on_slot[slot] = TRUE;
          break;
        }
      }

      /*
       * 1. Check first NV per slot cache content to see if the dialing string is emergency or not
       * 2. check Hard code cache if the number is not found in NV per sub
       */
      if(slot != PBM_NUM_SLOTS)
      {
        /* get session_index for GPB for same (slot) first */
        card_session = pbm_pb_type_and_slot_to_session_type(slot, PBM_GPB);
        pb_id        = pbm_uim_device_type_to_phonebook_type((pbm_uim_app_enum_type)card_session,
                                                              PBM_ECC);
      }
    }

    /* Try the numbers in ECC phonebook. */
    ret_val = pbm_session_enum_rec_init_ext(pb_id, (uint16)PBM_CAT_ECC, PBM_FIELD_NONE,
                                            my_ph_num, (uint16)strlen((char*)my_ph_num)+1,
                                            PBM_SEARCHTYPE_EXACT | PBM_DATATYPE_STR |
                                            PBM_FIELDID_PHONE | PBM_STRTYPE_PBM8, &handle);
    if (PBM_SUCCESS == ret_val)
    {
      while (PBM_SUCCESS == pbm_session_enum_next_rec_id_ext(&rec_id, handle))
      {
        pbm_ecc_mode_type emergency_mode     = NOT_EMERGENCY;
        boolean           srv_category_found = FALSE;
        uint8             srv_status_flag    = 0;

        srv_category_found  = pbm_find_ecc_cat_in_rec (rec_id, 
                                                       &found_category,
                                                       &found_field,
                                                       &emergency_mode,
                                                       &srv_status_flag);

        /* srv_status_flag associated to emergency number is set. It means dialed number
                 should be treated as normal in FULL srv and as emergency when no/limited srv */
        if(srv_status_flag)
        {
          pbm_ecc_call_resolution_ptr->special_emergency_number_subs_mask |= pbm_mapping_pbm_session_to_as_id_mask(l_session_type);
          if(found_field == PBM_FIELD_SERVING_MCC_HARDCODED_ECC)
          { 
            pbm_ecc_call_resolution_ptr->special_emergency_number_in_serving_mcc = TRUE;
          }
          else
          {
            pbm_ecc_call_resolution_ptr->special_emergency_number = TRUE;
          }
        }

        /* see if we can find a category */
        if (srv_category_found)
        {
          /* we found a category, Now, see if it's better than any category we may have. */
          for (i = 0; i < ARR_SIZE(ecc_order_preference); i++)
          {
            if (ecc_order_preference[i] == found_field)
            {
              if (i < best_match_index_gw &&
                  (emergency_mode == EMERGENCY_GW || emergency_mode == EMERGENCY_GW_1X))
              {
                best_match_index_gw = i;
                pbm_ecc_call_resolution_ptr->ecc_category_ptr_gw = found_category;
                UIM_MSG_HIGH_2("Picking GW emergency category 0x%x based on field 0x%x",
                               pbm_ecc_call_resolution_ptr->ecc_category_ptr_gw, found_field);
              }
              else if (i < best_match_index_1x &&
                       (emergency_mode == EMERGENCY_1X ||
                        emergency_mode == EMERGENCY_GW_1X))
              {
                best_match_index_1x = i;
                pbm_ecc_call_resolution_ptr->ecc_category_ptr_1x = found_category;
                UIM_MSG_HIGH_2("Picking 1X emergency category 0x%x based on field 0x%x",
                                pbm_ecc_call_resolution_ptr->ecc_category_ptr_1x, found_field);
              }
            }
          }
        }

        UIM_MSG_HIGH_4("best_match_index_gw 0x%x best_match_index_1x 0x%x i 0x%x rec_id 0x%x",
                        best_match_index_gw, best_match_index_1x, i, rec_id);
        if (emergency_mode == EMERGENCY_1X ||
            emergency_mode == EMERGENCY_GW_1X)
        {
          pbm_ecc_call_resolution_ptr->found_in_1x = TRUE;
        }
        if (emergency_mode == EMERGENCY_GW ||
            emergency_mode == EMERGENCY_GW_1X)
        {
          pbm_ecc_call_resolution_ptr->found_in_gw = TRUE;
        }
      }
      pbm_session_enum_rec_init_ext_free_handle(handle); /* free the handle */
    }
    else
    {
      UIM_MSG_ERR_1("pbm_session_enum_rec_init failed 0x%x", ret_val);
    }
  }

  /* OTA numbers are higher prority even though the number is special emergency number and
     the check is limited to GW only as there is no OTA no's in 3GPP2 */
  if(pbm_ecc_call_resolution_ptr->special_emergency_number &&
     best_match_index_gw == 0 && 
     ecc_order_preference[best_match_index_gw] == PBM_FIELD_NETWORK_ECC)
  {
    pbm_ecc_call_resolution_ptr->special_emergency_number = FALSE;
    pbm_ecc_call_resolution_ptr->special_emergency_number_subs_mask = SYS_MODEM_AS_ID_MASK_NONE;
    pbm_ecc_call_resolution_ptr->special_emergency_number_in_serving_mcc = FALSE;
  }

  UIM_MSG_HIGH_7("Emergency number status gw cat 0x%x 1X cat 0x%x found_in_gw 0x%x found_in_1x 0x%x session 0x%x special_emergency_number 0x%x special_emergency_number_subs_mask 0x%x",
                 pbm_ecc_call_resolution_ptr->ecc_category_ptr_gw,
                 pbm_ecc_call_resolution_ptr->ecc_category_ptr_1x,
                 pbm_ecc_call_resolution_ptr->found_in_gw,
                 pbm_ecc_call_resolution_ptr->found_in_1x,
                 pbm_ecc_call_resolution_ptr->p_session_type,
                 pbm_ecc_call_resolution_ptr->special_emergency_number,
                 pbm_ecc_call_resolution_ptr->special_emergency_number_subs_mask);

  UIM_MSG_HIGH_1("Emergency number status special_emergency_number_in_serving_mcc 0x%x",
                  pbm_ecc_call_resolution_ptr->special_emergency_number_in_serving_mcc);

  return PBM_SUCCESS;
}/* pbm_session_emergency_number_cat_ext2 */


/*===========================================================================
FUNCTION PBM_SESSION_EMERGENCY_NUMBER_CAT_EXT

DESCRIPTION
  This function searches the ECC phonebook for the number just passed in
  to determine if the current number is an Emergency number.

  If the number is found in gw only  then found_in_gw is marked TRUE and relavent category
  if associated with the number will be filled in ecc_category_ptr_gw

  If the number is found in 1x only  then found_in_1x is marked TRUE and relavent category
  if associated with the number will be filled in ecc_category_ptr_1x

  If the number is found in both 1x and gw  then found_in_gw and found_in_1x are marked TRUE
  and relavent category if associated with the number will be filled in ecc_category_ptr_gw
  and ecc_category_ptr_1x

  Length does not include the trailing NULL.

DEPENDENCIES
  None

RETURN VALUE
  pbm_return_type - PBM_SUCCESS

SIDE EFFECTS
  none
===========================================================================*/
pbm_return_type pbm_session_emergency_number_cat_ext(
  pbm_session_enum_type        p_session_type,
  const byte                  *num,
  byte                         len,
  boolean                     *found_in_gw,
  boolean                     *found_in_1x,
  uint8                       *ecc_category_ptr_gw,
  uint8                       *ecc_category_ptr_1x
)
{
  pbm_ecc_call_resolution_type   pbm_ecc_call_resolution;
  pbm_return_type                ret_val                   = PBM_SUCCESS;

  PBM_CHECK_PTR5_RET(num, ecc_category_ptr_gw, ecc_category_ptr_1x, found_in_gw, found_in_1x, PBM_ERROR);

  *ecc_category_ptr_gw = 0;
  *ecc_category_ptr_1x = 0;
  *found_in_gw         = FALSE;
  *found_in_1x         = FALSE;

  if (len == 0 || len > PBM_MAX_NUM_LENGTH)
  {
    UIM_MSG_ERR_2("search for emergency num invalid length 0x%x 0x%x", len, PBM_MAX_NUM_LENGTH);
    return ret_val;
  }

  memset(&pbm_ecc_call_resolution, 0, sizeof(pbm_ecc_call_resolution_type));

  pbm_ecc_call_resolution.num = num;
  pbm_ecc_call_resolution.len = len;
  pbm_ecc_call_resolution.p_session_type = p_session_type;

  ret_val = pbm_session_emergency_number_cat_ext2(&pbm_ecc_call_resolution);

  *ecc_category_ptr_gw = pbm_ecc_call_resolution.ecc_category_ptr_gw;
  *ecc_category_ptr_1x = pbm_ecc_call_resolution.ecc_category_ptr_1x;
  *found_in_gw         = pbm_ecc_call_resolution.found_in_gw;
  *found_in_1x         = pbm_ecc_call_resolution.found_in_1x;

  return ret_val;
} /* pbm_session_emergency_number_cat_ext */


/*===========================================================================
FUNCTION PBM_SESSION_CHECK_EMERGENCY_NUMBER_ON_ALL_SLOTS_EXT

DESCRIPTION
  This function searches the ECC phonebook for the number just passed in across all slots
  to determine if the current number is an Emergency number.

DEPENDENCIES
  None

RETURN VALUE
  pbm_return_type

SIDE EFFECTS
  none
===========================================================================*/
pbm_return_type pbm_session_check_emergency_number_on_all_slots_ext(
  pbm_ecc_call_resolution_type  *pbm_ecc_call_resolution_type_ptr
)
{
  pbm_session_enum_type   session_type                                 = PBM_SESSION_GPB_1;
  boolean                 resolved_as_emergency_in_gw[PBM_SESSION_MAX] = {FALSE};
  boolean                 resolved_as_emergency_in_1x[PBM_SESSION_MAX] = {FALSE};
  uint8                   gw_emergency_category[PBM_SESSION_MAX]       = {0};
  uint8                   emergency_category_1x[PBM_SESSION_MAX]       = {0};
  uint8                   asubs_mask_temp                              = SYS_MODEM_AS_ID_MASK_NONE;
  boolean                 special_emergency_number_in_serving_mcc      = FALSE;

  PBM_CHECK_PTR2_RET(pbm_ecc_call_resolution_type_ptr, pbm_ecc_call_resolution_type_ptr->num, PBM_ERROR);

  pbm_ecc_call_resolution_type_ptr->ecc_category_ptr_1x = 0;
  pbm_ecc_call_resolution_type_ptr->ecc_category_ptr_gw = 0;
  pbm_ecc_call_resolution_type_ptr->found_in_1x = FALSE;
  pbm_ecc_call_resolution_type_ptr->found_in_gw = FALSE;
  pbm_ecc_call_resolution_type_ptr->special_emergency_number = FALSE;
  pbm_ecc_call_resolution_type_ptr->special_emergency_number_in_serving_mcc = FALSE;
  pbm_ecc_call_resolution_type_ptr->special_emergency_number_subs_mask = 0;

  if (pbm_ecc_call_resolution_type_ptr->len == 0 || pbm_ecc_call_resolution_type_ptr->len > PBM_MAX_NUM_LENGTH)
  {
    UIM_MSG_ERR_2("search for emergency num invalid length 0x%x 0x%x",
                  pbm_ecc_call_resolution_type_ptr->len, PBM_MAX_NUM_LENGTH);
    return PBM_SUCCESS;
  }

  UIM_MSG_HIGH_0("pbm_session_check_emergency_number_on_all_slots ()");

  for(session_type = PBM_SESSION_GPB_1; session_type < PBM_SESSION_MAX; session_type++)
  {
    if(pbm_session_type_to_pb_category(session_type) != PBM_LPB)
    {
      continue;
    }

    pbm_ecc_call_resolution_type_ptr->p_session_type = session_type;
    pbm_ecc_call_resolution_type_ptr->special_emergency_number_subs_mask = SYS_MODEM_AS_ID_MASK_NONE;

    (void)pbm_session_emergency_number_cat_ext2(pbm_ecc_call_resolution_type_ptr);

    asubs_mask_temp |= pbm_ecc_call_resolution_type_ptr->special_emergency_number_subs_mask;

    if(pbm_ecc_call_resolution_type_ptr->found_in_gw)
    {
      resolved_as_emergency_in_gw[session_type] = TRUE;
      gw_emergency_category[session_type]       = pbm_ecc_call_resolution_type_ptr->ecc_category_ptr_gw;
    }

    if(pbm_ecc_call_resolution_type_ptr->found_in_1x)
    {
      resolved_as_emergency_in_1x[session_type] = TRUE;
      emergency_category_1x[session_type]       = pbm_ecc_call_resolution_type_ptr->ecc_category_ptr_1x;
    }
	
    if(pbm_ecc_call_resolution_type_ptr->special_emergency_number_in_serving_mcc)
    {
      special_emergency_number_in_serving_mcc = TRUE;
    }
  }

  memset(pbm_ecc_call_resolution_type_ptr, 0 , sizeof(pbm_ecc_call_resolution_type));

  if(asubs_mask_temp != SYS_MODEM_AS_ID_MASK_NONE)
  {
    pbm_ecc_call_resolution_type_ptr->special_emergency_number = TRUE;
    pbm_ecc_call_resolution_type_ptr->special_emergency_number_subs_mask = asubs_mask_temp;
  }

  if(special_emergency_number_in_serving_mcc)
  {
    pbm_ecc_call_resolution_type_ptr->special_emergency_number_in_serving_mcc = TRUE;
  }

  for(session_type = PBM_SESSION_GPB_1; session_type < PBM_SESSION_MAX; session_type++)
  {
    if(pbm_ecc_call_resolution_type_ptr->found_in_gw == FALSE && resolved_as_emergency_in_gw[session_type])
    {
      pbm_ecc_call_resolution_type_ptr->found_in_gw = TRUE;
      pbm_ecc_call_resolution_type_ptr->ecc_category_ptr_gw = gw_emergency_category[session_type];
    }
    if(pbm_ecc_call_resolution_type_ptr->found_in_1x == FALSE && resolved_as_emergency_in_1x[session_type])
    {
      pbm_ecc_call_resolution_type_ptr->found_in_1x = TRUE;
      pbm_ecc_call_resolution_type_ptr->ecc_category_ptr_1x = emergency_category_1x[session_type];
    }
  }

  return PBM_SUCCESS;
} /* pbm_session_check_emergency_number_on_all_slots_ext */
