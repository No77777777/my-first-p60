/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                P B M _ E C C.C

GENERAL DESCRIPTION
  This file contains the PBM ECC related functions.

  Copyright (c) 2016 - 2018, 2021, 2025 by QUALCOMM Technologies, Inc(QTI).
  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/pbm/src/pbm_ecc.c#8 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/06/25   skc     Support Emergency number upto 15 digits
11/14/22   skc     Get 3GPP2 MCC from correct variable in CM SS EVT  
08/04/21   tq      Mark 112 as normal ECC number in serving MCC based ECC list
10/01/21   tq      Clear serving MCC based ECC number in no service
09/15/21   tq      Handle Redundant back to back QMI_PBM_EMERGENCY_LIST_IND
07/29/21   shp     Support serving MCC based ECC number for china
03/02/21   skc     Ignore CM SS Event for ECC numbers for deactivated Session
08/02/18   nr      Do not populate simless ecc numbers if card is present in UE
06/12/18   nr      Remove stale MCC value in case of card is absent in slot
05/16/18   dd      Update current mcc source correctly
11/15/17   rps     Remove hardcoded ECC no for Indian operator, except 112
08/31/17   tq      Leave critical section before return
04/19/17   bcho    Moving QCRIL ECC DB to PBM
11/30/16   dd      Adding pbm_ for all applicable functions
11/25/16   ar      Add support for 20 ECC numbers rather than 5
11/21/16   bcho    Read NV hardcoded ECC config on NV Refresh
11/21/16   ar      Add check for cache variable NULL
11/17/16   ar      Return error if ECC file size is more
11/14/16   ar      Return error if cache is not available
10/18/16   nr      Logging improvements
09/27/16   nr      Adding VZW specific emergency numbers to simless ecc list
09/10/16   nr      NAL China emergency requirement
08/31/16   nr      F3 improvements
07/27/16   nr      112 as emergency in all RATs
07/21/16   sp      ECC optimizations for IoE devices
05/25/16   sp      F3 improvements
05/19/16   nr      Logging improvements
04/21/16   sp      Move ecc sim cache initialization to ecc related file for IoE
04/18/16   sp      Initial Revision
===========================================================================*/
#include "uim_variation.h"
#include "customer.h"
#include "pbm.h"
#include "uim_msg.h"
#include "pbm_common_types.h"
#include "pbmefs.h"
#include "pbm_conversion.h"
#include "pbmtask_i.h"
#include "pbmlib.h"
#include "pbmutils.h"
#include "pbmuim.h"
#include "pbmgsdi.h"
#include "pbm_nv_refresh.h"
#include "cm.h"
#include "pbmcache.h"
#ifdef FEATURE_DUAL_SIM
#include "cm_dualsim.h"
#endif
#include "atomic_ops.h"
#include "stringl/stringl.h"

#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif

/*===========================================================================
                              MACROS
===========================================================================*/
/* As per the spec, CSIM, RUIM and SIM can have EF-ECC of max length 15.
   We are supporting 60 length for not breaking the backward compatibility*/
#define PBM_NON_USIM_ECC_DATA_LEN_MAX        60

#define PBM_ECC_VALID_WITH_UIM     0x01
#define PBM_ECC_VALID_WITHOUT_UIM  0x02

/*===========================================================================
                       Declare data for the ECC.
===========================================================================*/
static uint32 ecc_has_entries = 0;

/* This variable tracks the 3gpp REL version in the build by reading LTE NV Item */
lte_3gpp_release_ver_e  pbm_3gpp_rel_ver  = LTE_3GPP_REL11 ;

/* This variable is read from NV pbm_hardcode_ecc_config (NV 69736 ) to decide the
     source of hardcoded ecc list ( i.e. either from code or from NV 69737)*/
static boolean          pbm_hardcode_ecc_config = FALSE ;

/* CM stuff */
static cm_client_id_type pbm_cm_client_id = (cm_client_id_type) -1;

PBM_NV_ECC_LIST_PER_SUB  *pbm_slot_specific_ecc_list_ptr[PBM_MAX_SLOTS][PBM_NV_EMERGENCY_NUMBERS_PER_SUB] = {{NULL}};

/* emergency lists will be there 2 for UIMs */
PB_CACHE                 ecc_sim_cache[MAX_UIM_APPS];   /* Emergency numbers */

/* Network ECC, NV ECC,Hardcoded ECC */
PB_CACHE                 ecc_other_cache;      /* Emergency numbers */

/* MCC retrieved from IMSI, ICCID, CM SS event and other sources */
static uint16 pbm_mcc_table[PBM_SLOT_3][PBM_ECC_MCC_SOURCE_MAX] = {{0}};

/* MCC retrieved from CM SS event */
static uint16 pbm_serving_mcc[PBM_SLOT_3] = {0};

/*===========================================================================
FUNCTION PBM_ECC_BCD_TO_ASCII

DESCRIPTION
  This function converts an ECC number into an ASCII string.  Note that
  the input string must be at least 7 characters in length.  This function
  will always set the first 7 characters.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
int pbm_ecc_bcd_to_ascii (
  const byte *ecc_number,
  char       *ascii_out
)
{
  int  bcd_index       = 0;
  byte asc_1           = 0;
  byte asc_2           = 0;
  int  returned_length = -1;
  int  ascii_index     = 0;

  PBM_CHECK_PTR2_RET(ecc_number, ascii_out, 0);

  /*******************************/
  /* Decode ECC's BCD into ascii */
  /*******************************/
  for ( bcd_index = 0; bcd_index < 3; bcd_index++)
  {
    asc_1 = (ecc_number[bcd_index] & 0x0F);
    asc_2 = (ecc_number[bcd_index] & 0xF0) >> 4;

    if ((returned_length == -1) && ((ascii_out[ascii_index++] = pbm_bcd_to_char_mapping[asc_1]) == '\0'))
    {
      returned_length = ascii_index - 1;
    }
    if ((returned_length == -1) && ((ascii_out[ascii_index++] = pbm_bcd_to_char_mapping[asc_2]) == '\0'))
    {
      returned_length = ascii_index - 1;
    }
  }

  /* Null terminate the ascii string */
  ascii_out[ascii_index] = '\0';
  if (returned_length == -1)
  {
    returned_length = ascii_index;
  }

  return returned_length;
} /* pbm_ecc_bcd_to_ascii */


/*===========================================================================
FUNCTION PBM_ECC_NUMBER_OF_VALID_EMERGENCY_NUMBERS_IN_OTA_LIST

DESCRIPTION
  This function returns number of valid (length is upto 15) emergency numbers in OTA list

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
static uint8 pbm_ecc_number_of_valid_emergency_numbers_in_ota_list(
  cm_mm_msim_ss_info_s_type      *ii_info_ptr
)
{
  int     stack_index                    = 0 ; /* stack index to iterate over the stacks in ii_info_ptr  */
  uint8   num_list_len                                   = 0;
  uint8   rec_in_stack                                   = 0;
  uint8   number_of_valid_emergency_number               = 0;

  /* Calculate the number of ecc entries received in CM SS event */
  for ( stack_index = 0 ; stack_index < ii_info_ptr->number_of_stacks && stack_index < CM_NO_STACKS; stack_index++)
  {
    num_list_len = ii_info_ptr->stack_info[stack_index].emerg_num_list.num_list_len;
    UIM_MSG_HIGH_2("num_list_len = 0x%x "
                   "stack_index = 0x%x",
                   num_list_len, stack_index);
    for ( rec_in_stack = 0 ;
          rec_in_stack < num_list_len && rec_in_stack < CM_MAX_EMERGENCY_NUM_COUNT ;
          rec_in_stack++ )
    {
      uint16 emerg_num_len = ii_info_ptr->stack_info[stack_index].emerg_num_list.num_list[rec_in_stack].num.len;

      if (emerg_num_len == 0 || emerg_num_len > PBM_ECC_MAX_LEN)
      {
        UIM_MSG_ERR_3("Incorrect Emergency number length for item %d "
                      "of %d stack of len %d",
                      rec_in_stack ,stack_index,num_list_len);
        continue;
      }
      number_of_valid_emergency_number++;
    }
  }

  UIM_MSG_HIGH_1("number_of_valid_emergecny_number = 0x%x",
                 number_of_valid_emergency_number);
  return number_of_valid_emergency_number;
}


/*===========================================================================
FUNCTION PBM_IS_OTA_UPDATE_REQUIRED

DESCRIPTION
 This function is to check if the OTA ECC cache need to be updated based on
 the cache content and latest CM SS ECC data

 TRUE - Requires an update and hence need to re-build the cache and send
        ecc ready event to client
 FALSE - Doesnt need an update - probably the CM SS data and cache data
         are identical

PARAMETERS

SIDE EFFECTS

===========================================================================*/
boolean pbm_is_ota_ecc_update_required(
  PB_CACHE                  *ecc_cache,
  cm_mm_msim_ss_info_s_type *ii_info_ptr
)
{
  int rec_in_stack                   = 0 ;
  int cache_rec_iter                 = 0 ; /* cache record iterator */
  int stack_index                    = 0 ; /* stack index to iterate over the stacks in ii_info_ptr  */
  uint8  num_of_ota_records_in_cache = 0 ;
    uint8           num_of_valid_ota_records_received    = 0;

  PBM_CHECK_PTR2_RET(ecc_cache, ecc_cache->pb_cache_array, FALSE);

  /* Calculate the Network ECC records for this session in cache */
  for(cache_rec_iter = 0 ; cache_rec_iter < ecc_cache->num_of_records ; cache_rec_iter++ )
  {
    if (ecc_cache->pb_cache_array[cache_rec_iter] != NULL  &&
        pbm_match_found(PBM_CAT_NONE, PBM_FIELD_NETWORK_ECC , NULL, 0, 0,
                        ecc_cache->pb_cache_array[cache_rec_iter],
                        PBM_SLOT_NONE))
    {
      num_of_ota_records_in_cache++;
    }
  }

  /* Calculate the valid number of Emergency numbers entries received in CM SS event. */
  num_of_valid_ota_records_received = pbm_ecc_number_of_valid_emergency_numbers_in_ota_list(ii_info_ptr);

  /* If there is a mismatch in received number of ecc entries from network and current list -
      then proceed to build the cache */
  if( num_of_valid_ota_records_received > 0 && num_of_ota_records_in_cache != num_of_valid_ota_records_received )
  {
    UIM_MSG_HIGH_2("Mis match in Network ECC list in cache (0x%x)"
                   " and valid Network ECC list received cm ss event (0x%x) - Rebuild ecc cache",
                    num_of_ota_records_in_cache, num_of_valid_ota_records_received);
    return TRUE ;
  }

  UIM_MSG_HIGH_1("Number of ecc entries match in cache and cm ss list 0x%x",
                  num_of_ota_records_in_cache);
  for( cache_rec_iter = 0 ; cache_rec_iter < ecc_cache->num_of_records ; cache_rec_iter++ )
  {
    if(ecc_cache->pb_cache_array[cache_rec_iter] != NULL &&
       pbm_match_found(PBM_CAT_NONE, PBM_FIELD_NETWORK_ECC , NULL, 0, 0,
                       ecc_cache->pb_cache_array[cache_rec_iter],
                       PBM_SLOT_NONE))
    {
      /* Found a NETWORK ECC entry in the cache - check this if this is present in the current cm ss ecc list */
      boolean number_match = FALSE ;
      for (stack_index = 0; stack_index < ii_info_ptr->number_of_stacks && stack_index < CM_NO_STACKS; stack_index++)
      {
        for ( rec_in_stack = 0 ;
              rec_in_stack < ii_info_ptr->stack_info[stack_index].emerg_num_list.num_list_len && rec_in_stack < CM_MAX_EMERGENCY_NUM_COUNT ;
              rec_in_stack++ )
        {
          char   emerg_num[PBM_ECC_MAX_LEN + 1];
          uint16 emerg_num_len = ii_info_ptr->stack_info[stack_index].emerg_num_list.num_list[rec_in_stack].num.len;
          uint8  cnt;

          if (emerg_num_len == 0 || emerg_num_len > PBM_ECC_MAX_LEN)
          {
            UIM_MSG_ERR_4("Incorrect Emergency number length for item %d "
                          "of %d stack of len %d, emerg_num_len = 0x%x",
                           rec_in_stack ,stack_index,ii_info_ptr->stack_info[stack_index].emerg_num_list.num_list_len, emerg_num_len);
            continue;
          }

          memset(emerg_num, 0, sizeof(emerg_num));
                 (void)memscpy(emerg_num,
                  sizeof(emerg_num),
                 (char*)ii_info_ptr->stack_info[stack_index].emerg_num_list.num_list[rec_in_stack].num.buf,
                  emerg_num_len);

          /* Check the number and emergency service category in the cache entry matches with current entry in cm ss list */
          if((pbm_match_found(PBM_CAT_NONE,
                              PBM_FIELD_NETWORK_ECC,
                              emerg_num,
                              (uint16)emerg_num_len+1 ,
                              PBM_SEARCHTYPE_EXACT,
                              ecc_cache->pb_cache_array[cache_rec_iter],
                              PBM_SLOT_NONE)))
          {
            UIM_MSG_HIGH_3("Number matched in cm ss and cache for index in "
                           "cache 0x%x stack 0x%x index in stack list 0x%x",
                           cache_rec_iter,stack_index,rec_in_stack);
            for( cnt = 0 ; cnt < ecc_cache->pb_cache_array[cache_rec_iter]->num_fields ; cnt++ )
            {
              if( ecc_cache->pb_cache_array[cache_rec_iter]->fields && ecc_cache->pb_cache_array[cache_rec_iter]->fields[cnt].field_id == PBM_FIELD_ECC_SERVICE_CATEGORY )
              {
                uint8 *ecc_category = (uint8 *)pbm_get_field_data_ptr(ecc_cache->pb_cache_array[cache_rec_iter]->fields , cnt);
                if( ecc_category != NULL )
                {
                  if( *ecc_category == ii_info_ptr->stack_info[stack_index].emerg_num_list.num_list[rec_in_stack].num_type )
                  {
                    number_match = TRUE ;
                  }
                  UIM_MSG_HIGH_2("Service category in cm ss 0x%x and "
                                 "cache record 0x%x ",
                                 ii_info_ptr->stack_info[stack_index].emerg_num_list.num_list[rec_in_stack].num_type ,
                                 *ecc_category );
                  break;
                }/* ecc_category != NULL */
              }
            }/* for cnt=0 */
          }

          if( number_match )
          {
            break;
          }
        } /* for rec_in_stack=0 */

        if( number_match )
        {
          break;
        }
      }

      if( number_match == FALSE )
      {
        UIM_MSG_HIGH_1("Number NOT found in the cache for record in "
                       "cache 0x%x - Rebuild ecc cache",cache_rec_iter);
        return TRUE ;
      }
    }
  }/* for cache_rec_iter=0 */

  UIM_MSG_HIGH_0("OTA ECC cache update is not required for this CM SS event");
  return FALSE ;
}/* pbm_is_ota_ecc_update_required */


/*===========================================================================
FUNCTION PBM_UPDATE_SERVING_MCC_ECC_IF_NEEDED.

DESCRIPTION
 This function is to check if serving MCC based ECC number need to be updated.
 if update requred then this function will udate the ECC list.

PARAMETERS

SIDE EFFECTS

===========================================================================*/
boolean pbm_update_serving_mcc_ecc_if_needed(
  uint16         updated_mcc,
  boolean        uim_present,
  pbm_slot_type  slot
)
{
  uint8                              i                     = 0;
  pbm_return_type                    ret_val               = PBM_ERROR;
  pbm_phonebook_type                 pb_id                 = {PBM_GPB};
  PB_CACHE                          *ecc_cache_ptr         = NULL;
  uint8                              card_present_mask     = 0x00; 
  uim_common_efs_status_enum_type    efs_status            = UIM_COMMON_EFS_ERROR;
  uim_common_efs_context_type        efs_context           = 0;
  uint8                              srv_flag_not_required = 0;
  boolean                            ecc_list_updated      = FALSE;
  typedef PACKED struct PACKED_POST
  {
    uint16                   mcc;
    uint8                    mask; /* b0 indicates entry if valid with UIM
                                      b1 indicates if entry if valid without UIM*/
    char                     dial_string[PBM_ECC_MAX_LEN + 1];
    uint8                    service_cat_len; /* currently this is only one byte */
    pbm_service_category_val service_cat_val;
    pbm_ecc_mode_type        emergency_mode;
    uint8                    special_srv;
  } PBM_ECC_TBL_MCC;

  static  PBM_ECC_TBL_MCC serving_mcc_hardcoded_ecc_table[] = {
    { 460, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "110",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X, 1},
    { 460, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "119",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X, 1},
    { 460, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "120",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X, 1},
    { 460, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "112",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X, 0},
    { 460, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "999",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X, 1},
    { 460, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "122",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X, 1},
  };

  PBM_VALIDATE_SLOT_RET(slot, FALSE);
  
  UIM_MSG_HIGH_2("pbm_update_serving_mcc_ecc_if_needed: updated_mcc: 0x%x stored_mcc: 0x%x ",
                 updated_mcc, pbm_serving_mcc[slot - 1]);

  
  /* Search through the ECC cache for hardcoded entries */
  pb_id.device_type = PBM_ECC_OTHER;
  ecc_cache_ptr     = pbm_pb_id_to_cache(pb_id);

  PBM_CHECK_PTR2_RET(ecc_cache_ptr, ecc_cache_ptr->pb_cache_array, 0);

  if(pbm_serving_mcc[slot - 1] == updated_mcc)
  {
    return FALSE;
  }
  
  for (i = 1; i <= ecc_cache_ptr->num_of_records; i++)
  {
    boolean match_found = FALSE;
    if (ecc_cache_ptr->pb_cache_array && ecc_cache_ptr->pb_cache_array[i])
    {
      if (pbm_match_found(PBM_CAT_NONE, PBM_FIELD_SERVING_MCC_HARDCODED_ECC, NULL, 0, 0,
                          ecc_cache_ptr->pb_cache_array[i],
                          slot))
      {
        ret_val = pbm_cache_delete(ecc_cache_ptr, ecc_cache_ptr->pb_cache_array[i]->record_id);
        if (PBM_SUCCESS != ret_val)
        {
          UIM_MSG_ERR_2("Error 0x%x while removing record 0x%x from ECC cache",
                        ret_val, ecc_cache_ptr->pb_cache_array[i]->record_id);
        }
        ecc_list_updated = TRUE;
      }
    }
  }

  /* Get current MCC value */
  pbm_serving_mcc[slot - 1] = updated_mcc;
  
  if(updated_mcc == PBM_INVALID_MCC)
  {
    return ecc_list_updated;
  }

  /* Select the table of MCC ECCs which is used when no SIM is inserted. */
  if (uim_present)
  {
    card_present_mask    = PBM_ECC_VALID_WITH_UIM;
  }
  else
  {
    card_present_mask    = PBM_ECC_VALID_WITHOUT_UIM;
  }
  
  efs_context = pbm_util_get_common_efs_item_context(slot - 1);
  if (efs_context >= UIM_COMMON_EFS_CONTEXT_MAX)
  {
    return TRUE;
  }

  efs_status = uim_common_efs_read(UIM_COMMON_EFS_PBM_ADD_SPECIAL_SRV_FLAG,
                                   UIM_COMMON_EFS_ITEM_FILE_TYPE,
                                   efs_context,
                                   (uint8 *)&srv_flag_not_required,
                                   (uint32)sizeof(srv_flag_not_required));
  if (efs_status != UIM_COMMON_EFS_SUCCESS)
  {
    UIM_MSG_ERR_2("UIM_COMMON_EFS_PBM_ADD_SPECIAL_SRV_FLAG failed for efs_context 0x%x, efs_status 0x%x", efs_context, efs_status);
  }

  /* If any entry is not in the current list, add it. */
  for (i = 0; i < ARR_SIZE(serving_mcc_hardcoded_ecc_table); i++)
  {
    if (updated_mcc == serving_mcc_hardcoded_ecc_table[i].mcc &&
        (card_present_mask & serving_mcc_hardcoded_ecc_table[i].mask))
    {      
      /* Use dummy session for HARDCODED ECC/ EFS records */
      ret_val = pbm_build_ecc_record(PBM_FIELD_SERVING_MCC_HARDCODED_ECC,
                                     (uint32)(strnlen((char *)serving_mcc_hardcoded_ecc_table[i].dial_string,
                                               PBM_ECC_MAX_LEN + 1) + 1),
                                     serving_mcc_hardcoded_ecc_table[i].dial_string,
                                     0, NULL, ((serving_mcc_hardcoded_ecc_table[i].service_cat_val) ? TRUE : FALSE),
                                     serving_mcc_hardcoded_ecc_table[i].service_cat_val,
                                     PBM_SESSION_DEFAULT,
                                     serving_mcc_hardcoded_ecc_table[i].emergency_mode,
                                     (srv_flag_not_required) ? 0 : serving_mcc_hardcoded_ecc_table[i].special_srv,
                                     slot);
      if (PBM_SUCCESS != ret_val)
      {
        UIM_MSG_ERR_1("Error 0x%x in building hardcoded record.", ret_val);
      }
      ecc_list_updated = TRUE;
    }
  }
  
  return ecc_list_updated;
}/* pbm_update_serving_mcc_ecc_if_needed */


/*===========================================================================
FUNCTION PBM_INIT_NV_ECC_PER_SLOT

DESCRIPTION
  This function is called at initialization and whenever a SIM is
  inserted or removed.  If uim_present indicates no SIM is present,
  then the longer hardcoded list of emergency numbers is used.  If
  the SIM is inserted, then the shorter list is used.

DEPENDENCIES
  None

SIDE EFFECTS
===========================================================================*/
void pbm_init_nv_ecc_per_slot (
  uint8     slot_index
)
{
  int                                i                           = 0;
  int                                j                           = 0;
  char                             **ecc_num_pptr                = NULL;
  pbm_nv_ecc_per_sub_s_type         *pbm_nv_ecc_list_per_sub_ptr = NULL;
  uim_common_efs_status_enum_type    efs_status                  = UIM_COMMON_EFS_ERROR;
  PBM_NV_ECC_LIST_PER_SUB           *ecc_node_ptr                = NULL;
  uim_common_efs_context_type        efs_context                 = 0;

  efs_context = pbm_util_get_common_efs_item_context(slot_index);
  if (efs_context >= UIM_COMMON_EFS_CONTEXT_MAX)
  {
    return;
  }

  /* Clear the slot specific ECC list to update latest content as part of NV refresh */
  pbm_free_nv_ecc_list(slot_index);
  
  PBM_MEM_ALLOC(pbm_nv_ecc_list_per_sub_ptr,
                PBM_NV_EMERGENCY_NUMBERS_PER_SUB*sizeof(pbm_nv_ecc_per_sub_s_type));
  if( !pbm_nv_ecc_list_per_sub_ptr )
  {
    return ;
  }
  memset((byte *)pbm_nv_ecc_list_per_sub_ptr, 0,
         PBM_NV_EMERGENCY_NUMBERS_PER_SUB*sizeof(pbm_nv_ecc_per_sub_s_type));

  efs_status = uim_common_efs_read(UIM_COMMON_EFS_PBM_NV_ECC_LIST_PER_SUB,
                                   UIM_COMMON_EFS_ITEM_FILE_TYPE,
                                   efs_context,
                                   (uint8 *)pbm_nv_ecc_list_per_sub_ptr,
                                   (uint32)PBM_NV_EMERGENCY_NUMBERS_PER_SUB*sizeof(pbm_nv_ecc_per_sub_s_type));
  if (efs_status != UIM_COMMON_EFS_SUCCESS )
  {
    UIM_MSG_ERR_1("PBM NV ECC per sub read failed 0x%x",efs_status);
    PBM_MEM_FREEIF(pbm_nv_ecc_list_per_sub_ptr);
    return;
  }

  PBM_MEM_ALLOC(ecc_num_pptr, PBM_NV_EMERGENCY_NUMBERS_PER_SUB*sizeof(char *));
  if( !ecc_num_pptr )
  {
    PBM_MEM_FREEIF(pbm_nv_ecc_list_per_sub_ptr);
    return;
  }

  memset((byte *)ecc_num_pptr, 0, PBM_NV_EMERGENCY_NUMBERS_PER_SUB*sizeof(char *));
  for ( i = 0; i < PBM_NV_EMERGENCY_NUMBERS_PER_SUB; i++ )
  {
    if( pbm_nv_ecc_list_per_sub_ptr[i].num_digits == 0 ||
        pbm_nv_ecc_list_per_sub_ptr[i].num_digits > PBM_ECC_MAX_LEN)
    {
      continue ;
    }

    if( pbm_nv_ecc_list_per_sub_ptr[i].category_length > PBM_EMERGENCY_SERVICE_CAT_LEN )
    {
      pbm_nv_ecc_list_per_sub_ptr[i].category_length = PBM_EMERGENCY_SERVICE_CAT_LEN ;
    }

    if( pbm_nv_ecc_list_per_sub_ptr[i].category_length )
    {
      pbm_nv_ecc_list_per_sub_ptr[i].emergency_category  &= PBM_EMERGENCY_SERVICE_CAT_VALID_MASK;
    }

    PBM_MEM_ALLOC(ecc_num_pptr[i], pbm_nv_ecc_list_per_sub_ptr[i].num_digits + 1);
    if( !ecc_num_pptr[i] )
    {
      PBM_MEM_FREEIF(pbm_nv_ecc_list_per_sub_ptr);
      return;
    }

    memset((byte *)ecc_num_pptr[i], 0,pbm_nv_ecc_list_per_sub_ptr[i].num_digits + 1);
    for (j = 0; j < pbm_nv_ecc_list_per_sub_ptr[i].num_digits; j++)
    {
      ecc_num_pptr[i][j] = pbm_nv_ecc_list_per_sub_ptr[i].digits[j];
    }
    ecc_num_pptr[i][j] = '\0';

    PBM_MEM_ALLOC(ecc_node_ptr, sizeof(PBM_NV_ECC_LIST_PER_SUB));
    if( ecc_node_ptr == NULL)
    {
      PBM_MEM_FREEIF(pbm_nv_ecc_list_per_sub_ptr);
      PBM_MEM_FREEIF(ecc_num_pptr[i]);
      return;
    }
    memset((byte *)ecc_node_ptr, 0, sizeof(PBM_NV_ECC_LIST_PER_SUB));

    ecc_node_ptr->dial_string     = ecc_num_pptr[i];
    ecc_node_ptr->string_length   = pbm_nv_ecc_list_per_sub_ptr[i].num_digits + 1;
    ecc_node_ptr->service_cat_len = pbm_nv_ecc_list_per_sub_ptr[i].category_length;
    if( pbm_nv_ecc_list_per_sub_ptr[i].category_length )
    {
      ecc_node_ptr->service_cat_val = pbm_nv_ecc_list_per_sub_ptr[i].emergency_category ;
    }

    /* Updating srv status flag to ECC global */
    ecc_node_ptr->srv_status_flag = pbm_nv_ecc_list_per_sub_ptr[i].value;

    pbm_slot_specific_ecc_list_ptr[slot_index][i] = ecc_node_ptr;
  }
  PBM_MEM_FREEIF(ecc_num_pptr);
  PBM_MEM_FREEIF(pbm_nv_ecc_list_per_sub_ptr);
} /* pbm_init_nv_ecc_per_slot */


/*===========================================================================
FUNCTION PBM_BUILD_ECC_RECORD

DESCRIPTION
  This function takes a name, number, and category and builds the appropriate
  record for PBM_ECC.  It then adds this record to the cache.

DEPENDENCIES
  None

SIDE EFFECTS
===========================================================================*/
pbm_return_type pbm_build_ecc_record(
  pbm_field_id_e_type   field_id,
  uint32                data_length,
  const char           *ecc_string,
  uint32                name_length,
  const uint16         *name_string,
  boolean               usim_ecc_serv_cat_valid,
  uint8                 usim_ecc_serv_cat,
  pbm_session_enum_type session_type,
  pbm_ecc_mode_type      emergency_mode,
  uint8                  srv_status_flag,
  pbm_slot_type          ecc_slot
)
{
  pbm_phonebook_type     pb_id            = {PBM_GPB};
  uint32                 number_of_fields = 0;
  uint32                 field_length[5]  = {0};
  int                    index            = 0;
  pbm_addr_cache_s_type *record           = NULL;
  PB_CACHE              *ecc_cache        = NULL;
  uint16                 location         = 0;

  switch (field_id)
  {
    case PBM_FIELD_HARDCODED_ECC:
    case PBM_FIELD_NV_ECC:
    case PBM_FIELD_SERVING_MCC_HARDCODED_ECC:
      pb_id.device_type = PBM_ECC_OTHER;
      ecc_cache = pbm_pb_id_to_cache (pb_id);
      break;
    case PBM_FIELD_NETWORK_ECC:
      if(PBM_LPB == pbm_session_type_to_pb_category(session_type))
      {
        ecc_cache = pbm_file_id_to_cache(session_type, PBM_FILE_ECC);
      }
      else /* The else case would be applicable for NV only builds */
      {
        pb_id.device_type = PBM_ECC_OTHER;
        ecc_cache = pbm_pb_id_to_cache (pb_id);
      }
      break;
    case PBM_FIELD_SIM_ECC:
      ecc_cache = pbm_file_id_to_cache(session_type, PBM_FILE_ECC);
      break;
    default:
      UIM_MSG_HIGH_1("invalid ecc category 0x%x", field_id);
      return PBM_ERROR;
  }

  PBM_CHECK_PTR3_RET(ecc_cache, ecc_cache->pb_cache_array, ecc_string, PBM_ERROR);

  if ( usim_ecc_serv_cat == 0xFF )
  {
    usim_ecc_serv_cat = PBM_EMERGENCY_SERVICE_CAT_DEFAULT ;
  }
  else if ( usim_ecc_serv_cat > PBM_EMERGENCY_SERVICE_CAT_VALID_MASK )
  {
    usim_ecc_serv_cat &= PBM_EMERGENCY_SERVICE_CAT_VALID_MASK;
  }

  if (data_length > 0)
  {
    field_length[number_of_fields++] =  data_length;
    if (name_length > 0)
    {
      field_length[number_of_fields++] =  name_length;
    }
    if (usim_ecc_serv_cat_valid)
    {
      field_length[number_of_fields++] = 1;
    }
    if(emergency_mode != NOT_EMERGENCY)
    {
      field_length[number_of_fields++] = 1;
    }
    if(srv_status_flag)
    {
      field_length[number_of_fields++] = 1;
    }

    /* Next format the number for the ECC cache indicating that it is from
       the SESSION. If we ever need to figure out which application it came
       from, that would be a different the field ID for now, each SESSION
       has a unique ID.*/
    record = pbm_cache_record_new();
    PBM_CHECK_PTR_RET(record, PBM_ERROR_MEM_FULL);
    if (!pbm_allocate_fields_internal(&record->fields, number_of_fields,
                                        field_length))
    {
      PBM_MEM_FREEIF(record);
      return PBM_ERROR;
    }
    record->unique_id = 0;
    record->slot = ecc_slot;

    /* hardcoded no's should skip the first PBM_NV_EMERGENCY_NUMBERS locations
       since they are used by NV */
    if(field_id == PBM_FIELD_HARDCODED_ECC)
    {
      uint8    loc_index = 0;

      for (loc_index = PBM_NV_EMERGENCY_NUMBERS+1; loc_index <= ecc_cache->num_of_records; loc_index++)
      {
        if ((ecc_cache->pb_cache_array[loc_index] == NULL) || (ecc_cache->pb_cache_array[loc_index]->num_fields == 0))
        {
          break;
        }
      }

      if (loc_index > ecc_cache->num_of_records)  /* No empty slot was found*/
      {
        pbm_free_fields_internal(&record->fields, 1);
        PBM_MEM_FREEIF(record);
        return PBM_ERROR_MEM_FULL;
      }
      location = loc_index;
    }
    /* Allocate the location.  Return an error if it is full. */
    else if (0 == (location = pbm_get_first_available(ecc_cache, FALSE)))
    {
      pbm_free_fields_internal(&record->fields, 1);
      PBM_MEM_FREEIF(record);
      return PBM_ERROR_MEM_FULL;
    }

    switch (field_id)
    {
      case PBM_FIELD_HARDCODED_ECC:
      case PBM_FIELD_NV_ECC:
      case PBM_FIELD_SERVING_MCC_HARDCODED_ECC:
        pb_id.device_type = PBM_ECC_OTHER;
        record->record_id = pbm_session_location_to_record_id(pb_id, location);
        break;
      case PBM_FIELD_NETWORK_ECC:
      case PBM_FIELD_SIM_ECC:
        pb_id = pbm_device_type_to_phonebook_type(session_type, PBM_ECC);
        record->record_id = pbm_session_location_to_record_id(pb_id, location);
        break;
      default:
        UIM_MSG_HIGH_1("invalid ecc category 0x%x", field_id);
        return PBM_ERROR;
    }

    record->category = (int) PBM_CAT_ECC;
    record->num_fields = (uint16) number_of_fields;
    index = 0;
    pbm_fill_field(&record->fields[index++], field_id,
                   (pbm_field_type_e_type) PBM_FT_PHONE,
                   (byte *) ecc_string, (uint16) data_length);

    if (name_length > 0)
    {
      pbm_fill_field(&record->fields[index++], PBM_FIELD_NAME,
                     (pbm_field_type_e_type) PBM_FT_UCS2,
                     (byte *) name_string, (uint16) name_length);
    }

    if (usim_ecc_serv_cat_valid)
    {
      pbm_fill_field(&record->fields[index++],
                     PBM_FIELD_ECC_SERVICE_CATEGORY,
                     (pbm_field_type_e_type) PBM_FT_BYTE,
                     (byte *) &usim_ecc_serv_cat, 1);
    }
    if(emergency_mode != NOT_EMERGENCY)
    {
      pbm_fill_field(&record->fields[index++],
                     PBM_FIELD_EMERGENCY_MODE,
                     (pbm_field_type_e_type) PBM_FT_BYTE,
                     (byte *) &emergency_mode, 1);
    }
    if(srv_status_flag)
    {
      pbm_fill_field(&record->fields[index++],
                     PBM_FIELD_SRV_STATUS_FLAG,
                     (pbm_field_type_e_type) PBM_FT_BYTE,
                     (byte *) & srv_status_flag, 1);
    }

    /* Finally, add the number to the cache */
    if (PBM_SUCCESS != pbm_cache_add(ecc_cache, record, TRUE))
    {
      pbm_free_fields_internal(&record->fields, 1);
      PBM_MEM_FREEIF(record);
      return PBM_ERROR;
    }
  }
  return PBM_SUCCESS;
} /* pbm_build_ecc_record */


/*===========================================================================
FUNCTION PBM_ECC_DELETE_MCC

DESCRIPTION
 This function is to delete MCC for given source

 RETURN VALUE
  None

PARAMETERS
  pbm_slot_type  pbm 
  pbm_ecc_mcc_source_type  mcc source

SIDE EFFECTS
  None
===========================================================================*/
void pbm_ecc_delete_mcc
(
  pbm_slot_type            pbm_slot,
  pbm_ecc_mcc_soruce_type  mcc_source
)
{
  pbm_ecc_update_mcc(pbm_slot, mcc_source, PBM_INVALID_MCC);
} /* pbm_ecc_delete_mcc */


/*===========================================================================
FUNCTION PBM_ECC_UPDATE_MCC

DESCRIPTION
 This function is to update MCC for given source

 RETURN VALUE
  None

PARAMETERS
  pbm_slot_type  slot
  pbm_ecc_mcc_source_type
  uint16  mcc

SIDE EFFECTS
  None
===========================================================================*/
void pbm_ecc_update_mcc
(
  pbm_slot_type            pbm_slot,
  pbm_ecc_mcc_soruce_type  mcc_source,
  uint16                   mcc
)
{
  uint8 index = 0; 

  if(pbm_util_get_slot_index(pbm_slot, &index) != PBM_SUCCESS)
  {
    return;
  }

  if(mcc_source >= PBM_ECC_MCC_SOURCE_MAX)
  {
    return;
  }

  pbm_mcc_table[index][mcc_source] = mcc;
} /* pbm_ecc_update_mcc */


/*===========================================================================
FUNCTION PBM_ECC_GET_CURRENT_MCC

DESCRIPTION
  This function returns MCC for the given slot from below sources as per their order
  1. EF-IMSI
  2. EF-IMSI_M
  3. EF-ICCID
  4. MCC of home network in limited service
  5. MCC stored in EFS
  6. MCC of roaming network in limited service

PARAMETERS
  pbm_slot  slot

SIDE EFFECTS

===========================================================================*/
uint16 pbm_ecc_get_current_mcc
(
  pbm_slot_type pbm_slot
)
{
  uint8 index      = 0; 
  uint8 loop_index = 0;

  if(pbm_util_get_slot_index(pbm_slot, &index) != PBM_SUCCESS)
  {
    return PBM_INVALID_MCC;
  }

  for(loop_index = 0; loop_index < PBM_ECC_MCC_SOURCE_MAX; loop_index++)
  {
    if(pbm_mcc_table[index][loop_index] != 0x00)
    {
      return pbm_mcc_table[index][loop_index];
    }
  }

  return PBM_INVALID_MCC;
} /* pbm_ecc_get_current_mcc */


/*===========================================================================
FUNCTION PBM_ECC_GET_CURRENT_MCC_AND_SOURCE

DESCRIPTION
  This function returns MCC and provides source of MCC for the given slot from below sources as per their order
  1. EF-IMSI
  2. EF-IMSI_M
  3. EF-ICCID
  4. MCC of home network in limited service
  5. MCC stored in EFS
  6. MCC of roaming network in limited service

DEPENDENCIES
  None.

RETURN VALUE
  uint16

PARAMETERS
  pbm_slot                  slot
  pbm_ecc_mcc_soruce_type   *source

SIDE EFFECTS
  None.
===========================================================================*/
uint16 pbm_ecc_get_current_mcc_and_source
(
  pbm_slot_type                  pbm_slot,
  pbm_ecc_mcc_soruce_type        *source
)
{
  uint8 index                        = 0; 
  pbm_ecc_mcc_soruce_type loop_index = PBM_ECC_MCC_SOURCE_IMSI;

  if(pbm_util_get_slot_index(pbm_slot, &index) != PBM_SUCCESS)
  {
    return PBM_INVALID_MCC;
  }

  for(loop_index = PBM_ECC_MCC_SOURCE_IMSI; loop_index < PBM_ECC_MCC_SOURCE_MAX; loop_index++)
  {
    if(pbm_mcc_table[index][loop_index] != 0x00)
    {
      *source = loop_index;
      return pbm_mcc_table[index][loop_index];
    }
  }
  return PBM_INVALID_MCC;
} /* pbm_ecc_get_current_mcc */


/*===========================================================================
FUNCTION PBM_ECC_STORE_MCC

DESCRIPTION
 This function is to store MCC value to EFS

 RETURN VALUE
  None

PARAMETERS
  None

SIDE EFFECTS
  None
===========================================================================*/
void pbm_ecc_store_mcc(void)
{
  uim_common_efs_status_enum_type    efs_status                  = UIM_COMMON_EFS_ERROR;
  uim_common_efs_context_type        efs_context                 = 0;
  uint16                             mcc                         = PBM_INVALID_MCC;
  uint8                              index                       = 0;
  pbm_slot_type                      slot                        = PBM_SLOT_1;

  for(slot = PBM_SLOT_1; slot <= PBM_SLOT_3; slot++)
  {
    mcc = pbm_ecc_get_current_mcc(slot);

    if(pbm_util_get_slot_index(slot, &index) != PBM_SUCCESS)
    {
      return;
    }

    efs_context = pbm_util_get_common_efs_item_context(index);
    if (efs_context >= UIM_COMMON_EFS_CONTEXT_MAX)
    {
      return;
    }

    efs_status = uim_common_efs_write(UIM_COMMON_EFS_PBM_LATEST_MCC,
                                      UIM_COMMON_EFS_ITEM_FILE_TYPE,
                                      efs_context,
                                      (const char *)&mcc,
                                      (uint32)sizeof(mcc));
    if (efs_status != UIM_COMMON_EFS_SUCCESS)
    {
      UIM_MSG_ERR_1("PBM NV ECC per slot write failed 0x%x", efs_status);
    }
  }
}/* pbm_ecc_store_mcc */


/*===========================================================================
FUNCTION PBM_ECC_READ_MCC

DESCRIPTION
 This function is to read stored MCC value from EFS

 RETURN VALUE
  None

PARAMETERS
  None

SIDE EFFECTS
  None
===========================================================================*/
void pbm_ecc_read_mcc(void)
{
  uim_common_efs_status_enum_type    efs_status                  = UIM_COMMON_EFS_ERROR;
  uim_common_efs_context_type        efs_context                 = 0;
  uint16                             mcc                         = PBM_INVALID_MCC;
  uint8                              index                       = 0;
  pbm_slot_type                      slot                        = PBM_SLOT_1;

  for(slot = PBM_SLOT_1; slot <= PBM_SLOT_3; slot++)
  {
    mcc = PBM_INVALID_MCC;

    if(pbm_util_get_slot_index(slot, &index) != PBM_SUCCESS)
    {
      return;
    }

    efs_context = pbm_util_get_common_efs_item_context(index);
    if (efs_context >= UIM_COMMON_EFS_CONTEXT_MAX)
    {
      return;
    }

    efs_status = uim_common_efs_read(UIM_COMMON_EFS_PBM_LATEST_MCC,
                                     UIM_COMMON_EFS_ITEM_FILE_TYPE,
                                     efs_context,
                                     (uint8 *)&mcc,
                                     (uint32)sizeof(mcc));
    if (efs_status != UIM_COMMON_EFS_SUCCESS)
    {
      UIM_MSG_ERR_1("PBM NV ECC per slot read failed 0x%x", efs_status);
    }

    pbm_ecc_update_mcc(slot, PBM_ECC_MCC_SOURCE_EFS, mcc);
  }
}/* pbm_ecc_read_mcc */


/*===========================================================================
FUNCTION PBM_ECC_SET_HARDCODED_ECCS

DESCRIPTION
  This function is called at initialization and whenever a SIM is
  inserted or removed.  If uim_present indicates no SIM is present,
  then the longer hardcoded list of emergency numbers is used.  If
  the SIM is inserted, then the shorter list is used.

DEPENDENCIES
  None

SIDE EFFECTS
===========================================================================*/
void pbm_ecc_set_hardcoded_eccs (
  boolean   uim_present
)
{
  pbm_phonebook_type pb_id = {PBM_GPB};

  typedef PACKED struct PACKED_POST
  {
    char                     dial_string[PBM_ECC_MAX_LEN + 1];
    boolean                  already_in_pbm;
    uint8                    service_cat_len; /* currently this is only one byte */
    pbm_service_category_val service_cat_val;
    pbm_ecc_mode_type        emergency_mode;
  } PBM_ECC_TBL;

  typedef PACKED struct PACKED_POST
  {
    uint16      mcc;
    uint8       mask; /* b0 indicates entry if valid with UIM
                         b1 indicates if entry if valid without UIM*/
    char                     dial_string[PBM_ECC_MAX_LEN + 1];
    uint8                    service_cat_len; /* currently this is only one byte */
    pbm_service_category_val service_cat_val;
    pbm_ecc_mode_type        emergency_mode;
  } PBM_ECC_TBL_MCC;

  typedef struct
  {
    boolean already_in_pbm[PBM_SLOT_3];
  }PBM_ECC_IN_PBM;

  uint8                              i                        = 0;
  uint8                              j                        = 0;
  uint8                              list_size                = 0;
  char                             **num_pptr                 = NULL;
  pbm_return_type                    ret_val                  = PBM_SUCCESS;
  pbm_hardcoded_ecc_s_type          *pbm_nv_hardcoded_ecc_ptr = NULL;
  PBM_ECC_TBL                       *nv_hardcoded_list_ptr    = NULL;
  PBM_ECC_TBL                       *current_list_ptr         = NULL;
  PB_CACHE                          *ecc_cache_ptr            = NULL;
  boolean                            add_entry_to_list        = FALSE ;
  pbm_feature_status_enum_type       ecc_hardcode_sim_absent_till_pin_lock = PBM_FEATURE_DISABLED;
  uim_common_efs_status_enum_type    efs_status               = UIM_COMMON_EFS_ERROR;
  pbm_slot_type                      slot                     = PBM_SLOT_1;
  PBM_ECC_IN_PBM                    *pbm_ecc_in_pbm_ptr       = NULL;

  PBM_ECC_TBL hardcoded_with_uim[] = {
    { "911",   FALSE, PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X}, /* 911 is ALWAYS an emergency number */
    { "112",   FALSE, PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
#if (defined (FEATURE_CDMA_800) || defined (FEATURE_CDMA_1900))
    { "*911",  FALSE, PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_1X},    /* These are specific to CDMA */
    { "#911",  FALSE, PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_1X},
#endif
  };

  PBM_ECC_TBL hardcoded_with_no_uim[] = {
    { "911",   FALSE, PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { "112",   FALSE, PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
#if (defined (FEATURE_CDMA_800) || defined (FEATURE_CDMA_1900))
    { "*911",  FALSE, PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_1X},
    { "#911",  FALSE, PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_1X},
#endif
#if defined (FEATURE_VZW_SPECIFIC_ECC_LIST)
    { "*911",  FALSE, PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { "#911",  FALSE, PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
#endif
#if defined (FEATURE_WCDMA) || defined (FEATURE_GSM)
    { "000",   FALSE, PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW},
    { "08",    FALSE, PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW},
    { "110",   FALSE, PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW},
    { "999",   FALSE, PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW},
    { "118",   FALSE, PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW},
    { "119",   FALSE, PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW},
#endif
  };

static  PBM_ECC_TBL_MCC hardcoded_mcc_ecc_table[] = {
    { 204, PBM_ECC_VALID_WITHOUT_UIM, "112",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},

    { 206, PBM_ECC_VALID_WITHOUT_UIM, "112",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 208, PBM_ECC_VALID_WITHOUT_UIM, "911",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 208, PBM_ECC_VALID_WITHOUT_UIM, "112",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},

    { 214, PBM_ECC_VALID_WITHOUT_UIM, "911",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 214, PBM_ECC_VALID_WITHOUT_UIM, "112",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},

    { 216, PBM_ECC_VALID_WITHOUT_UIM, "112",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},

    { 222, PBM_ECC_VALID_WITHOUT_UIM, "112",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 222, PBM_ECC_VALID_WITHOUT_UIM, "911",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 222, PBM_ECC_VALID_WITHOUT_UIM, "999",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 222, PBM_ECC_VALID_WITHOUT_UIM, "08",   PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 222, PBM_ECC_VALID_WITHOUT_UIM, "118",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 222, PBM_ECC_VALID_WITHOUT_UIM, "119",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},

    { 226, PBM_ECC_VALID_WITHOUT_UIM, "112",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 226, PBM_ECC_VALID_WITHOUT_UIM, "911",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},

    { 230, PBM_ECC_VALID_WITHOUT_UIM, "112",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 230, PBM_ECC_VALID_WITHOUT_UIM, "150",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 230, PBM_ECC_VALID_WITHOUT_UIM, "155",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 230, PBM_ECC_VALID_WITHOUT_UIM, "158",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},

    { 231, PBM_ECC_VALID_WITHOUT_UIM, "911",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 231, PBM_ECC_VALID_WITHOUT_UIM, "112",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},

    { 234, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "999",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 234, PBM_ECC_VALID_WITHOUT_UIM, "112",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 234, PBM_ECC_VALID_WITHOUT_UIM, "911",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},

    { 260, PBM_ECC_VALID_WITHOUT_UIM, "112",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 260, PBM_ECC_VALID_WITHOUT_UIM, "911",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},

    { 262, PBM_ECC_VALID_WITHOUT_UIM, "112",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},

    { 268, PBM_ECC_VALID_WITHOUT_UIM, "112",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},

    { 272, PBM_ECC_VALID_WITHOUT_UIM, "112",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 272, PBM_ECC_VALID_WITHOUT_UIM, "999",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},

    { 286, PBM_ECC_VALID_WITHOUT_UIM, "112",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},

    { 302, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "112",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 302, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "911",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 302, PBM_ECC_VALID_WITHOUT_UIM, "999",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},

    { 334, PBM_ECC_VALID_WITH_UIM,    "060",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 334, PBM_ECC_VALID_WITH_UIM,    "066",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},

    { 310, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "112",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 310, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "911",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 310, PBM_ECC_VALID_WITHOUT_UIM, "999",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},

    { 404, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "112",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},

    { 405, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "112",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},

    { 414, PBM_ECC_VALID_WITHOUT_UIM, "191",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 414, PBM_ECC_VALID_WITHOUT_UIM, "192",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 414, PBM_ECC_VALID_WITHOUT_UIM, "199",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},

    { 429, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "100",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 429, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "101",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 429, PBM_ECC_VALID_WITH_UIM,    "102",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 429, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "108",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 429, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "112",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},

    { 452, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "112",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 452, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "113",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 452, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "114",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 452, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "115",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},

    { 456, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "191",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 456, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "192",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 456, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "199",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},

    { 457, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "191",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 457, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "192",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 457, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "199",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},

    { 460, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "110",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 460, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "119",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 460, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "120",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 460, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "112",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 460, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "999",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 460, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "122",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},

    { 470, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "100",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 470, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "101",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 470, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "102",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 470, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "108",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 470, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "112",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},

    { 502, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "112",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 502, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "911",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 502, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "991",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 502, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "994",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 502, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "999",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},

    { 510, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "110",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 510, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "118",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 510, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "199",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 510, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "113",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 510, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "112",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},

    { 515, PBM_ECC_VALID_WITHOUT_UIM, "117",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 515, PBM_ECC_VALID_WITHOUT_UIM, "192",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 515, PBM_ECC_VALID_WITHOUT_UIM, "911",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},

    { 520, PBM_ECC_VALID_WITHOUT_UIM, "191",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 520, PBM_ECC_VALID_WITHOUT_UIM, "1669", PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 520, PBM_ECC_VALID_WITHOUT_UIM, "199",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 520, PBM_ECC_VALID_WITHOUT_UIM, "112",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 520, PBM_ECC_VALID_WITHOUT_UIM, "911",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},

    { 655, PBM_ECC_VALID_WITHOUT_UIM, "112",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 655, PBM_ECC_VALID_WITHOUT_UIM, "911",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},

    { 704, PBM_ECC_VALID_WITH_UIM,    "122",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},

    { 706, PBM_ECC_VALID_WITH_UIM,    "122",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},

    { 716, PBM_ECC_VALID_WITH_UIM,    "105",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},

    { 730, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "133",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},

    { 732, PBM_ECC_VALID_WITH_UIM,    "123",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},

    { 852, PBM_ECC_VALID_WITH_UIM|
           PBM_ECC_VALID_WITHOUT_UIM, "999",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},

    { 901, PBM_ECC_VALID_WITH_UIM,    "999",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    { 901, PBM_ECC_VALID_WITH_UIM,    "122",  PBM_EMERGENCY_SERVICE_CAT_LEN, PBM_EMERGENCY_SERVICE_CAT_DEFAULT, EMERGENCY_GW_1X},
    };

  ecc_hardcode_sim_absent_till_pin_lock = pbm_nv_get_feature_status(PBM_FEATURE_ECC_HARDCODE_SIM_ABSENT_LIST_TILL_PIN_LOCK);
  UIM_MSG_HIGH_2("uim_present 0x%x pbm_hardcode_ecc_config 0x%x",
                  uim_present, pbm_hardcode_ecc_config);

  PBM_MEM_ALLOC(pbm_ecc_in_pbm_ptr, ARR_SIZE(hardcoded_mcc_ecc_table) * sizeof(PBM_ECC_IN_PBM));
  if(pbm_ecc_in_pbm_ptr == NULL)
  {
    return ;
  }

  if ( !pbm_hardcode_ecc_config )
  {
    /* Select the table of ECCs which is used when no SIM is inserted. */
    if (uim_present)
    {
      current_list_ptr = hardcoded_with_uim;
      list_size        = ARR_SIZE(hardcoded_with_uim);
    }
    else
    {
      current_list_ptr = hardcoded_with_no_uim;
      list_size        = ARR_SIZE(hardcoded_with_no_uim);
    }
  }
  else
  {
    PBM_MEM_ALLOC(nv_hardcoded_list_ptr,PBM_HARDCODED_NV_EMERGENCY_NUMBERS * sizeof(PBM_ECC_TBL));
    if(nv_hardcoded_list_ptr == NULL)
    {
      return ;
    }

    PBM_MEM_ALLOC(pbm_nv_hardcoded_ecc_ptr,PBM_HARDCODED_NV_EMERGENCY_NUMBERS*sizeof(pbm_hardcoded_ecc_s_type));
    if( !pbm_nv_hardcoded_ecc_ptr )
    {
      PBM_MEM_FREEIF(nv_hardcoded_list_ptr);
      return ;
    }
    memset((byte *)nv_hardcoded_list_ptr, 0,PBM_HARDCODED_NV_EMERGENCY_NUMBERS*sizeof(PBM_ECC_TBL));
    memset((byte *)pbm_nv_hardcoded_ecc_ptr, 0,PBM_HARDCODED_NV_EMERGENCY_NUMBERS*sizeof(pbm_hardcoded_ecc_s_type));

    efs_status = uim_common_efs_read(UIM_COMMON_EFS_PBM_NV_HARDCODED_ECC_LIST,
                                     UIM_COMMON_EFS_ITEM_FILE_TYPE,
                                     UIM_COMMON_EFS_DEVICE,
                                     (uint8 *)pbm_nv_hardcoded_ecc_ptr,
                                     (uint32)PBM_HARDCODED_NV_EMERGENCY_NUMBERS*sizeof(pbm_hardcoded_ecc_s_type));
    if (efs_status != UIM_COMMON_EFS_SUCCESS )
    {
      UIM_MSG_ERR_1("PBM Hardcoded ECC read from NV failed 0x%x",efs_status);
      ret_val = PBM_ERROR;
    }

    PBM_MEM_ALLOC(num_pptr,PBM_HARDCODED_NV_EMERGENCY_NUMBERS*sizeof(char *));
    if( !num_pptr )
    {
      PBM_MEM_FREEIF(nv_hardcoded_list_ptr);
      PBM_MEM_FREEIF(pbm_nv_hardcoded_ecc_ptr);
      return;
    }

    memset((byte *)num_pptr, 0,PBM_HARDCODED_NV_EMERGENCY_NUMBERS*sizeof(char *));
    for ( i=0; i < PBM_HARDCODED_NV_EMERGENCY_NUMBERS&&ret_val==PBM_SUCCESS; i++ )
    {
      add_entry_to_list   =  FALSE ;
      if( pbm_nv_hardcoded_ecc_ptr[i].num_digits == 0 ||
          pbm_nv_hardcoded_ecc_ptr[i].num_digits > PBM_ECC_MAX_LEN)
      {
        continue ;
      }

      if( pbm_nv_hardcoded_ecc_ptr[i].category_length > PBM_EMERGENCY_SERVICE_CAT_LEN )
      {
        pbm_nv_hardcoded_ecc_ptr[i].category_length = PBM_EMERGENCY_SERVICE_CAT_LEN ;
      }

      if( pbm_nv_hardcoded_ecc_ptr[i].category_length )
      {
        pbm_nv_hardcoded_ecc_ptr[i].emergency_category  &= PBM_EMERGENCY_SERVICE_CAT_VALID_MASK;
      }

      switch( pbm_nv_hardcoded_ecc_ptr[i].hardcoded_type )
      {
        case PBM_HARDCODE_ALWAYS:
           add_entry_to_list = TRUE;
           break;

        case PBM_HARDCODE_CARD_ABSENT:
           add_entry_to_list = ( !uim_present  || ( uim_present &&
                                                    ecc_hardcode_sim_absent_till_pin_lock &&
                                                    !( pbm_pin1_verified.value & PBM_MASK_CARD_SESSION_PIN_VALUES) ) );
           break;

        case PBM_HARDCODE_CARD_PRESENT:
           add_entry_to_list = uim_present ;
           break;

        default:
           add_entry_to_list = FALSE ;
           break;
      }
      UIM_MSG_HIGH_2("add_entry_to_list 0x%x and hardcode type 0x%x",
                      add_entry_to_list, pbm_nv_hardcoded_ecc_ptr[i].hardcoded_type);

      if ( add_entry_to_list )
      {
        PBM_MEM_ALLOC(num_pptr[i], pbm_nv_hardcoded_ecc_ptr[i].num_digits + 1);
        if( !num_pptr[i] )
        {
          PBM_MEM_FREEIF(pbm_nv_hardcoded_ecc_ptr);
          PBM_MEM_FREEIF(nv_hardcoded_list_ptr);
          PBM_MEM_FREEIF(num_pptr);
          return;
        }
        else
        {
          memset((byte *)num_pptr[i], 0,pbm_nv_hardcoded_ecc_ptr[i].num_digits + 1);
        }

        for (j = 0; j < pbm_nv_hardcoded_ecc_ptr[i].num_digits; j++)
        {
          num_pptr[i][j] = pbm_nv_hardcoded_ecc_ptr[i].digits[j];
        }
        num_pptr[i][j] = '\0';

        memscpy(nv_hardcoded_list_ptr[i].dial_string,
                PBM_ECC_MAX_LEN + 1,
                num_pptr[i],
                pbm_nv_hardcoded_ecc_ptr[i].num_digits + 1);

        nv_hardcoded_list_ptr[i].already_in_pbm  = FALSE ;
        nv_hardcoded_list_ptr[i].service_cat_len = pbm_nv_hardcoded_ecc_ptr[i].category_length;
        if( pbm_nv_hardcoded_ecc_ptr[i].category_length )
        {
          nv_hardcoded_list_ptr[i].service_cat_val = pbm_nv_hardcoded_ecc_ptr[i].emergency_category ;
        }
        nv_hardcoded_list_ptr[i].emergency_mode = (pbm_ecc_mode_type)(pbm_nv_hardcoded_ecc_ptr[i].emergency_mode) ;
        UIM_MSG_HIGH_3(" PBM hardcode 0x%x string_length 0x%x emergency_mode 0x%x",
                         pbm_nv_hardcoded_ecc_ptr[i].hardcoded_type,
                         strnlen((char *)nv_hardcoded_list_ptr[i].dial_string,
                           PBM_ECC_MAX_LEN + 1) + 1,
                         nv_hardcoded_list_ptr[i].emergency_mode);
      }
    }

    PBM_MEM_FREEIF(pbm_nv_hardcoded_ecc_ptr);
    current_list_ptr = nv_hardcoded_list_ptr;
    list_size        = PBM_HARDCODED_NV_EMERGENCY_NUMBERS ;
  }

  rex_enter_crit_sect(&pbm_crit_sect);

  /* Search through the ECC cache for hardcoded entries */
  pb_id.device_type = PBM_ECC_OTHER;
  ecc_cache_ptr     = pbm_pb_id_to_cache(pb_id);
  if(ecc_cache_ptr == NULL)
  {
    rex_leave_crit_sect(&pbm_crit_sect);
    return;
  }

  for (i = 1; i <= ecc_cache_ptr->num_of_records; i++)
  {
    if (ecc_cache_ptr->pb_cache_array && ecc_cache_ptr->pb_cache_array[i])
    {
      boolean match_found = FALSE;

      if (pbm_match_found(PBM_CAT_NONE, PBM_FIELD_HARDCODED_ECC, NULL, 0, 0,
                          ecc_cache_ptr->pb_cache_array[i],
                          PBM_SLOT_NONE))
      {

        /* We found an entry that is a hardcoded ECC
           If the entry is not in the current list, remove it. */
        for (j = 0; j < list_size; j++)
        {
          if ( ((strnlen((char *)current_list_ptr[j].dial_string,
                   PBM_ECC_MAX_LEN + 1) + 1) > 0) &&
               (pbm_match_found(PBM_CAT_NONE, PBM_FIELD_HARDCODED_ECC,
                            current_list_ptr[j].dial_string,
                            (uint16)(strnlen((char *)current_list_ptr[j].dial_string,
                                       PBM_ECC_MAX_LEN + 1) + 1),
                            PBM_SEARCHTYPE_EXACT,
                            ecc_cache_ptr->pb_cache_array[i],
                            PBM_SLOT_NONE)) &&
               (pbm_match_found(PBM_CAT_NONE, PBM_FIELD_EMERGENCY_MODE,
                            &current_list_ptr[j].emergency_mode,
                            (uint16) sizeof(current_list_ptr[j].emergency_mode),
                            PBM_SEARCHTYPE_EXACT,
                            ecc_cache_ptr->pb_cache_array[i],
                            PBM_SLOT_NONE)) &&
                            (current_list_ptr[j].service_cat_len > 0) &&
               (pbm_match_found(PBM_CAT_NONE, PBM_FIELD_ECC_SERVICE_CATEGORY,
                            &current_list_ptr[j].service_cat_val,
                            (uint16) current_list_ptr[j].service_cat_len,
                            PBM_SEARCHTYPE_EXACT,
                            ecc_cache_ptr->pb_cache_array[i],
                            PBM_SLOT_NONE)))
          {
            match_found = TRUE;
            current_list_ptr[j].already_in_pbm = TRUE;
            break;
          }
        }

        if (match_found == FALSE)
        {
          /* The entry was not in the current list, so remove it. */
          ret_val = pbm_cache_delete(ecc_cache_ptr, ecc_cache_ptr->pb_cache_array[i]->record_id);
          if (PBM_SUCCESS != ret_val)
          {
            UIM_MSG_ERR_2("Error 0x%x while removing record 0x%x from ECC cache",
                           ret_val, ecc_cache_ptr->pb_cache_array[i]->record_id);
          }
        }
        continue;
      }

      for(slot = PBM_SLOT_1; slot <= PBM_SLOT_3; slot++) 
      {
        if (pbm_match_found(PBM_CAT_NONE, PBM_FIELD_HARDCODED_ECC, NULL, 0, 0,
                            ecc_cache_ptr->pb_cache_array[i],
                            slot))
        {
          uint8  index             = 0; 
          uint16 mcc               = 0;
          uint8  card_present_mask = 0x00;
          
          if(pbm_util_get_slot_index(slot, &index) != PBM_SUCCESS)
          {
             continue;
          }
          
          /* Select the table of MCC ECCs which is used when no SIM is inserted. */
          if (pbm_card_status[index])
          {
            card_present_mask    = PBM_ECC_VALID_WITH_UIM;
          }
          else
          {
            card_present_mask    = PBM_ECC_VALID_WITHOUT_UIM;
          }
          
          /* Get current MCC value */ 
          mcc = pbm_ecc_get_current_mcc(slot);

          for (j = 0; j < ARR_SIZE(hardcoded_mcc_ecc_table); j++)
          {
            if ( (mcc == hardcoded_mcc_ecc_table[j].mcc) &&
                 (card_present_mask & hardcoded_mcc_ecc_table[j].mask) &&
                 ((strnlen((char *)hardcoded_mcc_ecc_table[j].dial_string,
                    PBM_ECC_MAX_LEN + 1) + 1) > 0) &&
                 (pbm_match_found(PBM_CAT_NONE, PBM_FIELD_HARDCODED_ECC,
                                  hardcoded_mcc_ecc_table[j].dial_string,
                                  (uint16)(strnlen((char *)hardcoded_mcc_ecc_table[j].dial_string,
                                            PBM_ECC_MAX_LEN + 1) + 1),
                                  PBM_SEARCHTYPE_EXACT,
                                  ecc_cache_ptr->pb_cache_array[i],
                                  slot)) &&
                 (pbm_match_found(PBM_CAT_NONE, PBM_FIELD_EMERGENCY_MODE,
                                  &hardcoded_mcc_ecc_table[j].emergency_mode,
                                  (uint16) sizeof(hardcoded_mcc_ecc_table[j].emergency_mode),
                                  PBM_SEARCHTYPE_EXACT,
                                  ecc_cache_ptr->pb_cache_array[i],
                                  slot)) &&
                                  (hardcoded_mcc_ecc_table[j].service_cat_len > 0) &&
                 (pbm_match_found(PBM_CAT_NONE, PBM_FIELD_ECC_SERVICE_CATEGORY,
                                  &hardcoded_mcc_ecc_table[j].service_cat_val,
                                  (uint16) hardcoded_mcc_ecc_table[j].service_cat_len,
                                  PBM_SEARCHTYPE_EXACT,
                                  ecc_cache_ptr->pb_cache_array[i],
                                  slot)))
           {
              match_found = TRUE;
              pbm_ecc_in_pbm_ptr[j].already_in_pbm[slot - 1] = TRUE;
              break;
            }
          }

          if (match_found == FALSE)
          {
            /* The entry was not in the current list, so remove it. */
            ret_val = pbm_cache_delete(ecc_cache_ptr, ecc_cache_ptr->pb_cache_array[i]->record_id);
            if (PBM_SUCCESS != ret_val)
            {
              UIM_MSG_ERR_2("Error 0x%x while removing record 0x%x from ECC cache",
                             ret_val, ecc_cache_ptr->pb_cache_array[i]->record_id);
            }
          }
          break;
        }
      }
    }
  }

  /* If any entry is not in the current list, add it. */
  for (i = 0; i < list_size; i++)
  {
    if (FALSE == current_list_ptr[i].already_in_pbm)
    {
      /* Use dummy session for HARDCODED ECC/ EFS records */
      ret_val = pbm_build_ecc_record(PBM_FIELD_HARDCODED_ECC,
                                     (uint32)(strnlen((char *)current_list_ptr[i].dial_string,
                                               PBM_ECC_MAX_LEN + 1) + 1),
                                     current_list_ptr[i].dial_string,
                                     0, NULL, ((current_list_ptr[i].service_cat_val) ? TRUE : FALSE),
                                     current_list_ptr[i].service_cat_val,
                                     PBM_SESSION_DEFAULT,
                                     current_list_ptr[i].emergency_mode,
                                     0,
                                     PBM_SLOT_NONE);
      if (PBM_SUCCESS != ret_val)
      {
        UIM_MSG_ERR_1("Error 0x%x in building hardcoded record.", ret_val);
      }
    }
  }

  for(slot = PBM_SLOT_1; slot <= PBM_SLOT_3; slot++)
  {
    uint8  index             = 0; 
    uint16 mcc               = 0;
    uint8  card_present_mask = 0x00; 

    if(pbm_util_get_slot_index(slot, &index) != PBM_SUCCESS)
    {
      continue;
    }

    /* Get current MCC value */
    mcc = pbm_ecc_get_current_mcc(slot);

    if(mcc == PBM_INVALID_MCC)
    {
      continue;
    }

    /* Select the table of MCC ECCs which is used when no SIM is inserted. */
    if (uim_present)
    {
      card_present_mask    = PBM_ECC_VALID_WITH_UIM;
    }
    else
    {
      card_present_mask    = PBM_ECC_VALID_WITHOUT_UIM;
    }

    /* If any entry is not in the current list, add it. */
    for (i = 0; i < ARR_SIZE(hardcoded_mcc_ecc_table); i++)
    {
      if (mcc == hardcoded_mcc_ecc_table[i].mcc &&
          (card_present_mask & hardcoded_mcc_ecc_table[i].mask) &&
          FALSE == pbm_ecc_in_pbm_ptr[i].already_in_pbm[slot - 1])
      {
        /* Use dummy session for HARDCODED ECC/ EFS records */
        ret_val = pbm_build_ecc_record(PBM_FIELD_HARDCODED_ECC,
                                       (uint32)(strnlen((char *)hardcoded_mcc_ecc_table[i].dial_string,
                                                 PBM_ECC_MAX_LEN + 1) + 1),
                                       hardcoded_mcc_ecc_table[i].dial_string,
                                       0, NULL, ((hardcoded_mcc_ecc_table[i].service_cat_val) ? TRUE : FALSE),
                                       hardcoded_mcc_ecc_table[i].service_cat_val,
                                       PBM_SESSION_DEFAULT,
                                       hardcoded_mcc_ecc_table[i].emergency_mode,
                                       0,
                                       slot);
        if (PBM_SUCCESS != ret_val)
        {
          UIM_MSG_ERR_1("Error 0x%x in building hardcoded record.", ret_val);
        }
      }
    }
  }

  rex_leave_crit_sect(&pbm_crit_sect);

  PBM_MEM_FREEIF(pbm_ecc_in_pbm_ptr);

  if ( pbm_hardcode_ecc_config )
  {
    PBM_MEM_FREEIF(nv_hardcoded_list_ptr);
    for ( i = 0;i < PBM_HARDCODED_NV_EMERGENCY_NUMBERS ; i ++ )
    {
      if ( num_pptr[i])
      {
        PBM_MEM_FREEIF(num_pptr[i]);
      }
    }
    PBM_MEM_FREEIF(num_pptr);
  }
} /* pbm_ecc_set_hardcoded_eccs */


/*===========================================================================
FUNCTION PBM_ECC_CLEAR_ALL_ID

DESCRIPTION
  This function is called at initialization and whenever a SIM is
  removed.  It removes all entries in the emergency_cache that
  have the ID of the given field.

DEPENDENCIES
  None

SIDE EFFECTS
===========================================================================*/
void pbm_ecc_clear_all_id(
  pbm_session_enum_type      session_type,
  pbm_field_id_e_type        field_id
)
{
  PB_CACHE      *ecc_cache = NULL;
  int            i         = 0;
  boolean        uses_sim  = FALSE;

  UIM_MSG_HIGH_3("pbm_ecc_clear_all_id ecc_has_entries 0x%x field_id 0x%x session_type 0x%x",
                  ecc_has_entries, field_id, session_type);
  switch (field_id)
  {
    case PBM_FIELD_SIM_ECC:
      uses_sim = TRUE;
      break;
    case PBM_FIELD_NETWORK_ECC:
      uses_sim = FALSE;
      break;
    default:
      return;
  }

  /* If this ECC was not initialized, nothing to do. */
  if (!uses_sim ||
      (0 != (ecc_has_entries & PBM_SESSIONID_TO_SESSION_MASK(session_type))))
  {
    /* Search through the ECC cache for hardcoded entries */
    ecc_cache = pbm_device_id_to_cache(session_type, PBM_ECC);
    PBM_CHECK_PTR_RET(ecc_cache, VOID);

    for (i = 1; i <= ecc_cache->num_of_records; i++)
    {
      if (ecc_cache->pb_cache_array && ecc_cache->pb_cache_array[i])
      {
        if (pbm_match_found(PBM_CAT_NONE, field_id, NULL, 0, 0,
                            ecc_cache->pb_cache_array[i],
                            PBM_SLOT_NONE))
        {
          pbm_return_type ret_val    = PBM_SUCCESS;

          ret_val = pbm_cache_delete(ecc_cache,ecc_cache->pb_cache_array[i]->record_id);
          if (PBM_SUCCESS != ret_val)
          {
            UIM_MSG_ERR_3("Error %d in deleting ECC entry %d from ECC cache 0x%x",
                           ret_val, ecc_cache->pb_cache_array[i]->record_id, ecc_cache);
          }
        }
      }
    }
  }

  if (uses_sim)
  {
    /* Clear out the flag now. */
    PBM_SESSION_RESET(ecc_has_entries, session_type);
  }
  return;
}/* pbm_ecc_clear_all_id */


/*===========================================================================
FUNCTION PBM_INIT_ECC_NV

DESCRIPTION
  This function is called during boot.  It reads the ECC list as stored
  in NV.

  The idea is to always allow Emergency Calls through by keeping a list
  of valid Emergency Call Codes (ECC's) in NV in order for the ME to
  be able to call an emergency call even if no SIM is installed.

DEPENDENCIES
  None

SIDE EFFECTS
===========================================================================*/
static pbm_return_type pbm_init_ecc_nv (
  void
)
{
  uint8                             i               = 0;
  uint8                             j               = 0;
  uint16                            size            = 0;
  uint16                            name[PBM_TEXT_SIZE_CHARS];
  pbm_phonebook_type                pb_id           = {PBM_GPB};
  PB_CACHE                         *cache           = NULL;
  pbm_return_type                   ret_val         = PBM_SUCCESS;
  uim_common_efs_status_enum_type   efs_status      = UIM_COMMON_EFS_ERROR;
  pbm_nv_ecc_type                  *nv_ecc_item     = NULL;

  /* Add all the hardcoded numbers. */
  pbm_ecc_set_hardcoded_eccs(FALSE);

  pb_id.device_type = PBM_ECC_OTHER;
  cache = pbm_pb_id_to_cache(pb_id);
  PBM_CHECK_PTR_RET(cache, PBM_ERROR);

  PBM_MEM_ALLOC(nv_ecc_item,
                PBM_NV_EMERGENCY_NUMBERS*sizeof(pbm_nv_ecc_type));
  PBM_CHECK_PTR_RET(nv_ecc_item, PBM_ERROR);
  memset((byte *)nv_ecc_item,
          0,
         PBM_NV_EMERGENCY_NUMBERS*sizeof(pbm_nv_ecc_type));

  /* Read the NV ECC numbers from 67221 item  */
  efs_status = uim_common_efs_read(UIM_COMMON_EFS_PBM_NV_ECC_LIST,
                                   UIM_COMMON_EFS_ITEM_FILE_TYPE,
                                   UIM_COMMON_EFS_DEVICE,
                                   (uint8 *)nv_ecc_item,
                                   (uint32)PBM_NV_EMERGENCY_NUMBERS*sizeof(pbm_nv_ecc_type));

  if (efs_status != UIM_COMMON_EFS_SUCCESS)
  {
    UIM_MSG_ERR_1("NV read failed.status=0x%x",efs_status);
    ret_val = PBM_ERROR;
  }

  /* Read in the numbers from NV. */
  for(i=0; i<PBM_NV_EMERGENCY_NUMBERS && ret_val==PBM_SUCCESS; i++)
  {
    /* Discard ECC number of length more than PBM_ECC_MAX_LEN digit */ 
    if (nv_ecc_item[i].num_digits > PBM_ECC_MAX_LEN)
    {
      continue;
    }

    if (nv_ecc_item[i].num_digits > 0 && nv_ecc_item[i].status == NV_EMERGENCY_SD_NUM)
    {
      /* Need to add a NULL to the end of dial_nv. */
      char             *dial_string        = NULL;

      PBM_MEM_ALLOC(dial_string, nv_ecc_item[i].num_digits + 1);
      if(dial_string == NULL)
      {
        PBM_MEM_FREEIF(nv_ecc_item);
        return PBM_ERROR_MEM_FULL;
      }

      for (j = 0; j < nv_ecc_item[i].num_digits; j++)
      {
        dial_string[j] = nv_ecc_item[i].digits[j];
      }

      /* Add NULL at the end of the number */
      dial_string[j] = '\0';
      size = (uint16)strnlen((char *) nv_ecc_item[i].letters, NV_MAX_LTRS);

      if (size > PBM_TEXT_SIZE_CHARS)
      {
        size = PBM_TEXT_SIZE_CHARS;
      }

      if(size <= NV_MAX_LTRS)
      {
        if (!pbmconvert_pbm8_to_ucs2((uint8 *) nv_ecc_item[i].letters, size,
                                      name, ARR_SIZE(name)))
        {
          UIM_MSG_ERR_1("Could not convert all %d octets of the name",size);
        }
      }

      if (PBM_SUCCESS != pbm_build_ecc_record(PBM_FIELD_NV_ECC,
                                               nv_ecc_item[i].num_digits + 1,
                                               dial_string,
                                               (pbmutils_wstrlen(name) + 1)* sizeof(uint16),
                                               name,
                                               FALSE, 0,
                                               PBM_SESSION_DEFAULT,
                                               nv_ecc_item[i].emergency_mode,
                                               0,
                                               PBM_SLOT_NONE))
      {
        UIM_MSG_ERR_0("Could not build ECC record");
      }

      PBM_MEM_FREEIF(dial_string);
    }
  }

  PBM_MEM_FREEIF(nv_ecc_item);
  PBM_SET_PB_CACHE_READY(cache);
  return ret_val;
}/* pbm_init_ecc_nv */


/*===========================================================================
FUNCTION pbm_init_sim_ecc_cache

DESCRIPTION
 Fill the cache attributes and allocate the cache size for storing the
 OTA ECC numbers for USIM and GSM cards

 No OTA numbers for RUIM and CSIM

DEPENDENCIES

SIDE EFFECTS
===========================================================================*/
pbm_return_type pbm_init_sim_ecc_cache(
  pbm_session_enum_type session_type
)
{
  PB_CACHE *cache       = pbm_file_id_to_cache(session_type,PBM_FILE_ECC);
  uint32    num_records = 0;

  PBM_CHECK_PTR_RET(cache,PBM_ERROR_NOT_AVAILABLE);
  memset(cache, 0, sizeof(PB_CACHE));
  cache->pb_id =  pbm_file_id_to_pb_id(session_type, (PBM_FILE_ECC));

  /* Initialize the cache with possible no of OTA emergency numbers
     later we will resize the cache once we recieve the SIM ECC numbers */
  if( session_type == PBM_SESSION_LPB_GW  ||
      session_type == PBM_SESSION_LPB_GW_SECONDARY ||
      session_type == PBM_SESSION_LPB_GW_TERTIARY )
  {
    num_records = CM_MAX_EMERGENCY_NUM_COUNT;
  }

  num_records += PBM_NV_EMERGENCY_NUMBERS_PER_SUB;

  pbm_fill_pb_cache_info(&ecc_sim_cache[session_type],
                         session_type,
                         PBM_USIM_PB_ONE,
                         PBM_MAX_NUM_LENGTH,
                         PBM_ECC_MIN_LEN,
                         PBM_FILE_ECC,
                         MMGSDI_LINEAR_FIXED_FILE,
                         FALSE,
                         num_records,
                         NULL);
  PBM_MEM_ALLOC(cache->pb_cache_array,
                ((uint32) cache->num_of_records + 1) *
                  sizeof(pbm_addr_cache_s_type *));

  PBM_CHECK_PTR_RET(cache->pb_cache_array,PBM_ERROR_MEM_FULL);
  cache->start = 1;
  if (cache->num_of_records)
  {
    memset(cache->pb_cache_array, 0,
           ((uint32) cache->num_of_records + 1) *
             sizeof(pbm_addr_cache_s_type *));
    cache->circular = pbm_file_circular(PBM_FILE_ECC);
    cache->start = cache->num_of_records;
  }
  cache->records_in_use = 0;

  return PBM_SUCCESS;
} /* pbm_init_sim_ecc_cache */


/*===========================================================================
FUNCTION PBM_NV_HARDCODED_ECC_CONFIG_READ

DESCRIPTION
  Read and cache NV HADRCODED ECC CONFIG

DEPENDENCIES

SIDE EFFECTS
===========================================================================*/
void pbm_nv_hardcoded_ecc_config_read(void)
{
  uim_common_efs_status_enum_type    efs_status   = UIM_COMMON_EFS_ERROR;

  efs_status = uim_common_efs_read(UIM_COMMON_EFS_PBM_NV_HARDCODED_ECC_CONFIG,
                                   UIM_COMMON_EFS_ITEM_FILE_TYPE,
                                   UIM_COMMON_EFS_DEVICE,
                                   &pbm_hardcode_ecc_config,
                                   (uint32)sizeof(boolean));
  if (efs_status != UIM_COMMON_EFS_SUCCESS)
  {
    UIM_MSG_ERR_1("PBM NV read failed during reading Hardcoded Config file 0x%x", efs_status);
    pbm_hardcode_ecc_config = FALSE;
  }
}/* pbm_nv_hardcoded_ecc_config_read */


/*===========================================================================
FUNCTION PBM_ECC_INIT

DESCRIPTION
  Initialize Emergency Call Codes.

  The idea is to always allow Emergency Calls through by keeping a list
  of valid Emergency Call Codes (ECC's) in NV in order for the ME to
  be able to call an emergency call even if no SIM is installed.

DEPENDENCIES

SIDE EFFECTS
===========================================================================*/
pbm_return_type pbm_ecc_init(
  void
)
{
  pbm_phonebook_type                 pb_id        = {PBM_GPB};
  pbm_return_type                    ret_val      = PBM_SUCCESS;
  uim_common_efs_status_enum_type    efs_status   = UIM_COMMON_EFS_ERROR;
  uint8                              slot_index   = 0;
  pbm_session_enum_type              session_type = PBM_SESSION_MAX;
  pb_id.                             device_type  = PBM_ECC_OTHER;

  /* Initialize Emergency Call Codes from NV */
  ret_val = pbm_efs_cache_structure_init(&ecc_other_cache,
                                         pb_id,
                                         PBM_MAX_ECC_NUMBERS,
                                         FALSE);
  if (PBM_SUCCESS != ret_val)
  {
    UIM_MSG_ERR_1("Error 0x%x initializing emergency numbers", ret_val);
  }

  pbm_nv_hardcoded_ecc_config_read();

  /* Intilize ECC list per slot */
  for(slot_index = PBM_SLOT_1_INDEX; slot_index <= PBM_SLOT_3_INDEX; slot_index++)
  {
    (void)pbm_init_nv_ecc_per_slot(slot_index);
  }

   pbm_ecc_read_mcc();

   ret_val = pbm_init_ecc_nv();
   if (PBM_SUCCESS != ret_val)
   {
      UIM_MSG_ERR_1("Error during pbm_init_ecc_nv 0x%x", ret_val);
   }

   efs_status = uim_common_efs_read(UIM_COMMON_EFS_NON_UIM_LTE_3GPP_RELEASE_VER,
                                    UIM_COMMON_EFS_ITEM_FILE_TYPE,
                                    UIM_COMMON_EFS_DEVICE,
                                    (uint8 *)&pbm_3gpp_rel_ver,
                                    (uint32)sizeof(lte_3gpp_release_ver_e));
  if (efs_status != UIM_COMMON_EFS_SUCCESS)
  {
    pbm_3gpp_rel_ver  = LTE_3GPP_REL11 ;
  }
  UIM_MSG_HIGH_2(" pbm_rel_ver_read_from_nv status 0x%x value 0x%x", efs_status, pbm_3gpp_rel_ver);


  for(session_type = PBM_SESSION_DEFAULT; session_type < PBM_SESSION_MAX; session_type++)
  {
    PBM_CACHE_INIT(&ecc_sim_cache[session_type], PBM_FILE_ECC, session_type, PBM_ERROR);
    if((ret_val = pbm_init_sim_ecc_cache(session_type)) != PBM_SUCCESS)
    {
      return ret_val;
    }
  }

  return PBM_SUCCESS;
}/* pbm_ecc_init */


/*===========================================================================
FUNCTION PBM_CM_INIT

DESCRIPTION
   Initialize CM Client, for network emergency numbers.

DEPENDENCIES

SIDE EFFECTS
===========================================================================*/
pbm_return_type pbm_cm_init (
  void
)
{
  cm_client_status_e_type cm_status;

  cm_status = cm_client_init(CM_CLIENT_TYPE_PBM, &pbm_cm_client_id);
  if (cm_status != CM_CLIENT_OK)
  {
    UIM_MSG_ERR_2("cm_client_init failed, 0x%x 0x%x",cm_status, pbm_cm_client_id);
    return PBM_ERROR;
  }

  cm_status = cm_mm_client_ss_reg_msim(pbm_cm_client_id,
                                       pbm_cm_ss_event_cb,
                                       CM_CLIENT_EVENT_REG,
                                       CM_SS_EVENT_EMERG_NUM_LIST,
                                       CM_SS_EVENT_EMERG_NUM_LIST,
                                       NULL,
                                       SYS_MODEM_AS_ID_MASK_ANY);

  if (CM_CLIENT_OK != cm_status)
  {
    UIM_MSG_ERR_2("cm_mm_client_ss_reg_msim failed for emerg_num_list 0x%x 0x%x",
                   cm_status, pbm_cm_client_id);
    return PBM_ERROR;
  }

  cm_status = cm_mm_client_ss_reg_msim(pbm_cm_client_id,
                                       pbm_cm_ss_event_cb,
                                       CM_CLIENT_EVENT_REG,
                                       CM_SS_EVENT_SRV_CHANGED,
                                       CM_SS_EVENT_SRV_CHANGED,
                                       NULL,
                                       SYS_MODEM_AS_ID_MASK_ANY);
  
  if (CM_CLIENT_OK != cm_status)
  {
    UIM_MSG_ERR_2("cm_mm_client_ss_reg_msim failed for srv_changed 0x%x 0x%x",
                   cm_status, pbm_cm_client_id);
    return PBM_ERROR;
  }

  cm_status = cm_client_act(pbm_cm_client_id);
  if (CM_CLIENT_OK != cm_status)
  {
    UIM_MSG_ERR_2("cm_client_act failed 0x%x 0x%x", cm_status, pbm_cm_client_id);
    return PBM_ERROR;
  }
  return PBM_SUCCESS;
}/* pbm_cm_init */


/*===========================================================================
FUNCTION pbm_handle_cm_ss_evt

DESCRIPTION
  This function is called when we get an SS event and it will only look for
  the follwoing events.

  1. CM_SS_EVENT_EMERG_NUM_LIST event to add numbers to the ECC cache.
  2. CM_SS_EVENT_SRV_CHANGED event to update UE srv status.

DEPENDENCIES

SIDE EFFECTS
===========================================================================*/
void pbm_handle_cm_ss_evt (
  pbm_cm_ss_evt_type* cm_ss_evt_ptr
)
{
  uint8                           stack_idx                  = 0;
  uint8                           j                          = 0;
  PB_CACHE                       *ecc_cache                  = NULL;
  pbm_return_type                 ret_val                    = PBM_SUCCESS;
  pbm_session_enum_type           sess_type                  = PBM_SESSION_DEFAULT;
  cm_mm_msim_ss_info_s_type      *ii_info_ptr                = NULL;
  pbm_slot_type                   slot_id                    = PBM_SLOT_1;
  boolean                         card_present_on_any_slot   = FALSE;
  boolean                         cache_ready_require        = FALSE;

  PBM_CHECK_PTR_RET(cm_ss_evt_ptr, VOID);

  rex_enter_crit_sect(&pbm_crit_sect);

  switch (cm_ss_evt_ptr->ss_event)
  {
    case CM_SS_EVENT_EMERG_NUM_LIST:
    {
      boolean                    rebuild_ecc_cache = TRUE ;

      ii_info_ptr   = cm_ss_evt_ptr->ii_info_ptr;

      PBM_CHECK_PTR_RET(ii_info_ptr, rex_leave_crit_sect(&pbm_crit_sect));
      sess_type = pbm_mapping_as_id_to_pbm_session(ii_info_ptr->asubs_id);

      /* When Session got deactivated, do NOT update Emergency number list through CM SS Event */
      if(pbm_session_info[sess_type].slot_id == 0)
      {
        UIM_MSG_HIGH_1("Ignoring ECC number list from CM_SS_Event on sess_type = 0x%x as session is deactivated or card error ", sess_type);
        break;
      }

      UIM_MSG_HIGH_2("PBM received CM SS event for sess_type 0x%x as_id 0x%x ",
                      sess_type, ii_info_ptr->asubs_id);

      ecc_cache = pbm_file_id_to_cache(sess_type, PBM_FILE_ECC);
      PBM_CHECK_PTR2_RET(ecc_cache, ecc_cache->pb_cache_array, rex_leave_crit_sect(&pbm_crit_sect));

      rebuild_ecc_cache = pbm_is_ota_ecc_update_required(ecc_cache,ii_info_ptr) ;

      if ( sess_type != PBM_SESSION_DEFAULT && rebuild_ecc_cache )
      {
        pbm_ecc_clear_all_id(sess_type, PBM_FIELD_NETWORK_ECC);
        for (stack_idx = 0; stack_idx < ii_info_ptr->number_of_stacks && stack_idx < CM_NO_STACKS; stack_idx++)
        {
          UIM_MSG_HIGH_2("PBM CM SS stack 0x%x list_len 0x%x ",
                          stack_idx, ii_info_ptr->stack_info[stack_idx].emerg_num_list.num_list_len);
          for ( j = 0 ; j < ii_info_ptr->stack_info[stack_idx].emerg_num_list.num_list_len && j < CM_MAX_EMERGENCY_NUM_COUNT; j ++ )
          {
            char   emerg_num[sizeof(ii_info_ptr->stack_info[stack_idx].emerg_num_list.num_list[j].num.buf)+1];
            uint16 emerg_num_len = ii_info_ptr->stack_info[stack_idx].emerg_num_list.num_list[j].num.len;

            if (emerg_num_len == 0 ||
              (emerg_num_len >= sizeof(emerg_num)) ||
               (emerg_num_len > PBM_ECC_MAX_LEN))
            {
              UIM_MSG_ERR_4("Emergency number length is 0x%x for item 0x%x of 0x%x stack of len 0x%x",
                             emerg_num_len,
                             j,
                             stack_idx,
                             ii_info_ptr->stack_info[stack_idx].emerg_num_list.num_list_len);
              continue;
            }

            memset(emerg_num, 0, sizeof(emerg_num));
                   (void)memscpy(emerg_num,sizeof(emerg_num),
                   (char*)ii_info_ptr->stack_info[stack_idx].emerg_num_list.num_list[j].num.buf,
                   emerg_num_len);

            ret_val = pbm_build_ecc_record(PBM_FIELD_NETWORK_ECC,
                                           emerg_num_len + 1, /*Add 1 for NULL*/
                                           emerg_num, 0, NULL, TRUE,
                                           (uint8)ii_info_ptr->stack_info[stack_idx].emerg_num_list.num_list[j].num_type,
                                           sess_type,
                                           EMERGENCY_GW,
                                           0,
                                           PBM_SLOT_NONE);
            if (PBM_SUCCESS != ret_val)
            {
              UIM_MSG_ERR_3("Error 0x%x in building network ECC record 0x%x of 0x%x stack", ret_val, stack_idx, j);
              break;
            }
          }
        }
        ecc_cache = pbm_file_id_to_cache(sess_type, PBM_FILE_ECC);
        PBM_SET_PB_CACHE_READY(ecc_cache);
      }
    }
    break;

    case CM_SS_EVENT_SRV_CHANGED:
    {
      ii_info_ptr = cm_ss_evt_ptr->ii_info_ptr;
      PBM_CHECK_PTR_RET(ii_info_ptr, rex_leave_crit_sect(&pbm_crit_sect));

      switch(ii_info_ptr->asubs_id)
      {
        case SYS_MODEM_AS_ID_1:
        case SYS_MODEM_AS_ID_2:
        case SYS_MODEM_AS_ID_3:
        {
          pbm_session_enum_type pbm_session_gw = PBM_SESSION_DEFAULT;
          pbm_session_enum_type pbm_session_1x = PBM_SESSION_DEFAULT;
          uint16                updated_mcc    = 0;

            UIM_MSG_HIGH_2("PBM received CM SS event of type CM_SS_EVENT_SRV_CHANGED for sess_type 0x%x as_id 0x%x ",
                            sess_type, ii_info_ptr->asubs_id);

          if(ii_info_ptr->asubs_id == SYS_MODEM_AS_ID_1)
          {
            pbm_session_gw = PBM_SESSION_LPB_GW;
            pbm_session_1x = PBM_SESSION_LPB_1X;
          }
          else if (ii_info_ptr->asubs_id == SYS_MODEM_AS_ID_2)
          {
            pbm_session_gw = PBM_SESSION_LPB_GW_SECONDARY;
            pbm_session_1x = PBM_SESSION_LPB_1X_SECONDARY;
          }
          else
          {
            pbm_session_gw = PBM_SESSION_LPB_GW_TERTIARY;
            pbm_session_1x = PBM_SESSION_LPB_1X_TERTIARY;
          }

          if(PBM_SS_INFO_CHANGED(ii_info_ptr->stack_info[0].changed_fields))
          {
            if(pbm_card_status[PBM_SLOT_1 - 1]   ||
               pbm_card_status[PBM_SLOT_2 - 1]   ||
               pbm_card_status[PBM_SLOT_3 - 1])
            {
              card_present_on_any_slot = TRUE;
            }

            UIM_MSG_HIGH_4("PBM GW Service Status: 0x%x 1X srv status 0x%x roam_status: 0x%x",
                           pbm_session_info[pbm_session_gw].srv_status,
                           pbm_session_info[pbm_session_1x].srv_status,
                           ii_info_ptr->stack_info[stack_idx].roam_status,
                           ii_info_ptr->prl_3gpp2_mcc.mcc_list[0]);
            if(PBM_3GPP_SYS_MODE(ii_info_ptr->stack_info[stack_idx].sys_mode))
            {
              pbm_session_info[pbm_session_gw].srv_status  = ii_info_ptr->stack_info[stack_idx].srv_status;
              pbm_session_info[pbm_session_gw].roam_status = ii_info_ptr->stack_info[stack_idx].roam_status;
  
              slot_id = pbm_util_get_slot_for_prov_session(pbm_session_gw);

              if(pbm_session_info[pbm_session_gw].srv_status == SYS_SRV_STATUS_LIMITED ||
                 pbm_session_info[pbm_session_gw].srv_status == SYS_SRV_STATUS_LIMITED_REGIONAL ||
                 pbm_session_info[pbm_session_gw].srv_status == SYS_SRV_STATUS_SRV)
              {
                uint16                   current_mcc              = 0;
                pbm_ecc_mcc_soruce_type  source_of_current_mcc    = PBM_ECC_MCC_SOURCE_MAX;

                (void)pbm_get_mcc_from_plmn(ii_info_ptr->stack_info[stack_idx].sys_id.id.plmn, &updated_mcc);

                if(pbm_session_info[pbm_session_gw].srv_status == SYS_SRV_STATUS_LIMITED ||
                   pbm_session_info[pbm_session_gw].srv_status == SYS_SRV_STATUS_LIMITED_REGIONAL)
                {
                  current_mcc = pbm_ecc_get_current_mcc_and_source(slot_id, &source_of_current_mcc);
  
                  if(pbm_session_info[pbm_session_gw].roam_status == SYS_ROAM_STATUS_ON)
                  {
                    pbm_ecc_update_mcc(slot_id, PBM_ECC_MCC_SOURCE_LIMITED_ROAM_NW, updated_mcc);
                  }
                  else
                  {
                    pbm_ecc_update_mcc(slot_id, PBM_ECC_MCC_SOURCE_LIMITED_HOME_NW, updated_mcc);
                  }
  
                  UIM_MSG_HIGH_2("Source_of_current_mcc = 0x%x, slot_id = 0x%x ",
                                  source_of_current_mcc, slot_id);
                  /* If current MCC source is of high priority than the source of MCC
                     which we got from cm ss event,then do NOT send any indication,
                     beacuse there will be NO changes in Hardcoded ecc list */
                  if(((pbm_session_info[pbm_session_gw].roam_status == SYS_ROAM_STATUS_ON) &&
                      (source_of_current_mcc >=PBM_ECC_MCC_SOURCE_LIMITED_ROAM_NW))        ||
                     ((pbm_session_info[pbm_session_gw].roam_status != SYS_ROAM_STATUS_ON) &&
                      (source_of_current_mcc >=PBM_ECC_MCC_SOURCE_LIMITED_HOME_NW)))
                  {
                    if(current_mcc != updated_mcc)
                    {
                      if(pbm_card_status[PBM_SLOT_1 - 1] ||
                         pbm_card_status[PBM_SLOT_2 - 1] ||
                         pbm_card_status[PBM_SLOT_3 - 1])
                      {
                        card_present_on_any_slot = TRUE;
                      }
  
                      if(card_present_on_any_slot)
                      {
                        pbm_ecc_set_hardcoded_eccs(TRUE);
                      }
                      else
                      {
                        pbm_ecc_set_hardcoded_eccs(FALSE);
                      }
                      cache_ready_require = TRUE;
                    }
                  }
                }             
              }
              if(pbm_update_serving_mcc_ecc_if_needed(updated_mcc, card_present_on_any_slot, slot_id) || cache_ready_require )
              {
                PBM_SET_PB_CACHE_READY(&ecc_other_cache);
              }
            }
            else if(PBM_3GPP2_SYS_MODE(ii_info_ptr->stack_info[stack_idx].sys_mode))
            {
              pbm_session_info[pbm_session_1x].srv_status  = ii_info_ptr->stack_info[stack_idx].srv_status;
              pbm_session_info[pbm_session_1x].roam_status = ii_info_ptr->stack_info[stack_idx].roam_status;
              slot_id = pbm_util_get_slot_for_prov_session(pbm_session_1x);

              if(pbm_session_info[pbm_session_1x].srv_status == SYS_SRV_STATUS_LIMITED ||
                 pbm_session_info[pbm_session_1x].srv_status == SYS_SRV_STATUS_LIMITED_REGIONAL ||
                 pbm_session_info[pbm_session_1x].srv_status == SYS_SRV_STATUS_SRV)
              {
                updated_mcc = 0;
                /* In case of 1X, we need to take mcc from prl_3gpp2_mcc, it can be list of MCC but per CM,
                     camped MCC can be fetched from index 0 */
                updated_mcc = ii_info_ptr->prl_3gpp2_mcc.mcc_list[0];
              }

              if(pbm_update_serving_mcc_ecc_if_needed(updated_mcc, card_present_on_any_slot, slot_id))
              {
                PBM_SET_PB_CACHE_READY(&ecc_other_cache);
              }
            }
            else
            {
              slot_id = pbm_util_get_slot_for_prov_session(pbm_session_gw);
              if(ii_info_ptr->stack_info[stack_idx].sys_mode == SYS_SYS_MODE_NO_SRV )
              {
                if(pbm_update_serving_mcc_ecc_if_needed(updated_mcc, card_present_on_any_slot, slot_id))
                {
                  PBM_SET_PB_CACHE_READY(&ecc_other_cache);
                }
              }
              pbm_session_info[pbm_session_gw].srv_status  = ii_info_ptr->stack_info[stack_idx].srv_status;
              pbm_session_info[pbm_session_1x].srv_status  = ii_info_ptr->stack_info[stack_idx].srv_status;
              pbm_session_info[pbm_session_gw].roam_status = ii_info_ptr->stack_info[stack_idx].roam_status;
              pbm_session_info[pbm_session_1x].roam_status = ii_info_ptr->stack_info[stack_idx].roam_status;
            }
          }            
          /* This require to handle scenario where PLMN got chnage but no chaage in service */
          if(!PBM_SS_INFO_CHANGED(ii_info_ptr->stack_info[stack_idx].changed_fields) &&
               PBM_SS_SYS_ID_CHANGED(ii_info_ptr->stack_info[stack_idx].changed_fields))
          {
            updated_mcc = 0;
            if( PBM_3GPP_SYS_MODE(ii_info_ptr->stack_info[stack_idx].sys_mode))
            {
              slot_id = pbm_util_get_slot_for_prov_session(pbm_session_gw);
              if(pbm_session_info[pbm_session_gw].srv_status == SYS_SRV_STATUS_LIMITED ||
                 pbm_session_info[pbm_session_gw].srv_status == SYS_SRV_STATUS_LIMITED_REGIONAL ||
                 pbm_session_info[pbm_session_gw].srv_status == SYS_SRV_STATUS_SRV)
              {
                (void)pbm_get_mcc_from_plmn(ii_info_ptr->stack_info[stack_idx].sys_id.id.plmn, &updated_mcc);
              }
            }
            else if(PBM_3GPP2_SYS_MODE(ii_info_ptr->stack_info[stack_idx].sys_mode))
            {
              slot_id = pbm_util_get_slot_for_prov_session(pbm_session_1x);
 
              if(pbm_session_info[pbm_session_1x].srv_status == SYS_SRV_STATUS_LIMITED ||
                 pbm_session_info[pbm_session_1x].srv_status == SYS_SRV_STATUS_LIMITED_REGIONAL ||
                 pbm_session_info[pbm_session_1x].srv_status == SYS_SRV_STATUS_SRV)
              {
                updated_mcc = 0;
                updated_mcc = ii_info_ptr->prl_3gpp2_mcc.mcc_list[0];
              }
            }
            else
            {
              /* slot id for mode none */
              slot_id = pbm_util_get_slot_for_prov_session(pbm_session_gw);
           }
            if(pbm_update_serving_mcc_ecc_if_needed(updated_mcc, card_present_on_any_slot, slot_id))
            {
              PBM_SET_PB_CACHE_READY(&ecc_other_cache);
            }
          }
        }
        break;

        default:
          break;
      }
    }
    break;

    default:
      break;
  }

  rex_leave_crit_sect(&pbm_crit_sect);
}/* pbm_handle_cm_ss_evt */


/*===========================================================================
FUNCTION PBM_POPULATE_ECC_OTHER_IF_PIN_STATUS_CHANGE

DESCRIPTION
  This function takes effect when ecc_hardcode_sim_pin_lock_feature is enabled
  along with NV 69736 pbm_hardcode_ecc_config

  This will return TRUE if hardcode cache is re-built and FALSE if it is NOT re-built

PARAMETERS

SIDE EFFECTS

===========================================================================*/
boolean  pbm_populate_ecc_other_if_pin1_status_change(
  atomic_word_t pin1_prev_status
)
{
  boolean                   ecc_sim_pin_lock_feature = ( pbm_nv_get_feature_status(PBM_FEATURE_ECC_HARDCODE_SIM_ABSENT_LIST_TILL_PIN_LOCK) ) ? TRUE : FALSE ;
  boolean                   ret_val = FALSE ;

  UIM_MSG_HIGH_1("In pbm_populate_ecc_other_if_pin1_status_change pbm_hardcode_ecc_config 0x%x",
                 pbm_hardcode_ecc_config);
  if( pbm_hardcode_ecc_config && ecc_sim_pin_lock_feature )
  {
     boolean prev_pin_state = ( pin1_prev_status.value & PBM_MASK_CARD_SESSION_PIN_VALUES ) ? TRUE : FALSE ;
     boolean updated_pin_state = ( pbm_pin1_verified.value & PBM_MASK_CARD_SESSION_PIN_VALUES ) ? TRUE : FALSE ;
     UIM_MSG_HIGH_2("Masked prev pin 0x%x and current pin 0x%x",(pin1_prev_status.value & PBM_MASK_CARD_SESSION_PIN_VALUES) ,
                                                                (pbm_pin1_verified.value & PBM_MASK_CARD_SESSION_PIN_VALUES));
     if ( prev_pin_state != updated_pin_state )
     {
       pbm_ecc_set_hardcoded_eccs(TRUE);
       ret_val = TRUE ;
     }
  }

  return ret_val;
} /* pbm_populate_ecc_other_if_pin1_status_change */


/*===========================================================================
FUNCTION pbm_build_slot_specific_ecc_cache

DESCRIPTION
Builds the records in ecc_sim_cache as per the data configured in NV 73755

RETURN VALUE
  void.

DEPENDENCIES

===========================================================================*/
void pbm_build_slot_specific_ecc_cache(
  pbm_session_enum_type        session_type,
  pbm_ecc_mode_type            emergency_mode,
  uint8                        nv_ecc_per_sub
)
{
  uint8                 j            = 0;
  uint16                i            = 0;
  PB_CACHE             *cache        = NULL;
  pbm_return_type       pbm_ret      = PBM_SUCCESS;
  pbm_slot_type         slot_id      = PBM_MAX_SLOTS;
  uint8                 slot_index   = 0;
  boolean               found_in_sim = FALSE;

  PBM_SESSION_SET(ecc_has_entries, session_type);
  if(emergency_mode < EMERGENCY_GW || emergency_mode > EMERGENCY_GW_1X)
  {
    UIM_MSG_ERR_1("Incorrect emergency_mode 0x%x", emergency_mode);
    return;
  }

  PBM_VALIDATE_SESSION_RET(session_type, VOID);
  slot_id = pbm_session_info[session_type].slot_id;
  if(pbm_util_get_slot_index(slot_id, &slot_index) != PBM_SUCCESS)
  {
    return;
  }

  rex_enter_crit_sect(&pbm_crit_sect);

  cache = (PB_CACHE *) pbm_file_id_to_cache(session_type, PBM_FILE_ECC);
  if (!cache)
  {
    UIM_MSG_ERR_1("No Cache found for session 0x%x, PBM_FILE_ECC", session_type);
    rex_leave_crit_sect(&pbm_crit_sect);
    return;
  }

  for(j = 0; j < nv_ecc_per_sub; j++)
  {
    found_in_sim = FALSE;
    if(pbm_slot_specific_ecc_list_ptr[slot_index][j] == NULL)
    {
      continue;
    }

    for (i = 1; i <= cache->num_of_records; i++)
    {
      if (cache->pb_cache_array != NULL && cache->pb_cache_array[i])
      {
        if (pbm_match_found(PBM_CAT_NONE, PBM_FIELD_SIM_ECC, NULL, 0, 0,
                            cache->pb_cache_array[i],
                            PBM_SLOT_NONE))
        {
          /* Avoid adding from NV ECC per sub if same ECC number
             found SIM ECC cache.*/
          if ((pbm_slot_specific_ecc_list_ptr[slot_index][j]->string_length > 0) &&
               (pbm_match_found(PBM_CAT_NONE, PBM_FIELD_SIM_ECC,
                                pbm_slot_specific_ecc_list_ptr[slot_index][j]->dial_string,
                                (uint16) pbm_slot_specific_ecc_list_ptr[slot_index][j]->string_length,
                                PBM_SEARCHTYPE_EXACT,
                                cache->pb_cache_array[i],
                                PBM_SLOT_NONE)))
          {
            found_in_sim = TRUE;
            break;
          }
        }
      }
    }

    /* Avoid adding from NV ECC per sub if same ECC number
       found in SIM ECC cache.*/
    if(found_in_sim)
    {
      continue;
    }

    /* Adding PBM_FIELD_SIM_ECC EFS records to SIM cache */
    pbm_ret = pbm_build_ecc_record(PBM_FIELD_SIM_ECC,
                                   pbm_slot_specific_ecc_list_ptr[slot_index][j]->string_length,
                                   pbm_slot_specific_ecc_list_ptr[slot_index][j]->dial_string,
                                   0, NULL,
                                   ((pbm_slot_specific_ecc_list_ptr[slot_index][j]->service_cat_val) ? TRUE : FALSE),
                                   pbm_slot_specific_ecc_list_ptr[slot_index][j]->service_cat_val,
                                   session_type,
                                   emergency_mode,
                                   pbm_slot_specific_ecc_list_ptr[slot_index][j]->srv_status_flag,
                                   PBM_SLOT_NONE);
    if (PBM_SUCCESS != pbm_ret)
    {
      UIM_MSG_ERR_1("Error 0x%x in building NV ECC per sub record.", pbm_ret);
    }
  }
  cache->status = PBM_STATUS_READY;

  rex_leave_crit_sect(&pbm_crit_sect);
}/* pbm_build_slot_specific_ecc_cache */


/*===========================================================================
FUNCTION pbm_update_sim_ecc_cache

DESCRIPTION
Builds the records in ecc_sim_cache as per the data received in MMGSDI_ECC_EVT

In case of USIM and SIM cards , copies the OTA numbers pointers to the newly
allocated cache array pointer so that the numbers wont be lost


RETURN VALUE
  void.

DEPENDENCIES

===========================================================================*/
void pbm_update_sim_ecc_cache(
  pbm_session_enum_type        session_type,
  mmgsdi_ecc_evt_info_type    *ecc_evt_info
)
{
  PB_CACHE                         *cache                                      = NULL;
  pbm_field_id_e_type               field_id                                   = PBM_FIELD_NONE;
  int                               data_length                                = 0;   /* Length of ASCII version of ECC # */
  uint32                            name_length                                = 0;
  uint16                            ascii_emergency_alpha[PBM_TEXT_SIZE_CHARS] = {0}; /* Emergency number Alpha Identifier. */
  byte                              emergency_category                         = 0;   /* Emergency service category octet. */
  int                               ecc_offset                                 = 0;   /* Where in the ecc_list are we */
  int                               i                                          = 0;
  uint8                             j                                          = 0;
  pbm_return_type                   pbm_ret                                    = PBM_SUCCESS;
  pbm_ecc_mode_type                 emergency_mode                             = NOT_EMERGENCY;
  uint16                            number_of_records                          = 0;
  int                               record_length                              = 0;
  mmgsdi_file_structure_enum_type   file_type                                  = MMGSDI_LINEAR_FIXED_FILE;
  /* + and null for string */
  char                              number[PBM_NUM_SIZE]                       = {0};
  uint8                             nv_ecc_per_sub                             = PBM_NV_EMERGENCY_NUMBERS_PER_SUB;

  rex_enter_crit_sect(&pbm_crit_sect);

  cache = (PB_CACHE *) pbm_file_id_to_cache(session_type, PBM_FILE_ECC);
  if (!cache)
  {
    UIM_MSG_ERR_1("No Cache found for session 0x%x", session_type);
    rex_leave_crit_sect(&pbm_crit_sect);
    return;
  }

  pbm_util_get_emergency_mode(session_type, &emergency_mode);

  /* clear previous SIM ecc no's if any */
  pbm_ecc_clear_all_id(session_type, PBM_FIELD_SIM_ECC);

  if( ecc_evt_info != NULL &&
      ecc_evt_info->ecc_count != 0 && ecc_evt_info->ecc_data != NULL )
  {
    /* for any GSM sim no of ecc recs will be always 1
     * and max len would be GSDI_MAX_ECC_LEN */
    if (USE_USIM_SUPPORT(session_type))
    {
      /* QMI-PBM interface supports MAX of 255 records. Hence we are limiting
          number of records to be included in card emergency TLV */
      if((ecc_evt_info->ecc_count) + PBM_NV_EMERGENCY_NUMBERS_PER_SUB >= PBM_MAX_CARD_ECC)
      {
        nv_ecc_per_sub = PBM_MAX_CARD_ECC - (ecc_evt_info->ecc_count) -1;
      }

      record_length     = PBM_MAX_NUM_LENGTH;
      number_of_records = ecc_evt_info->ecc_count + CM_MAX_EMERGENCY_NUM_COUNT + nv_ecc_per_sub;
      file_type         = MMGSDI_LINEAR_FIXED_FILE;
    }
    else
    {
      record_length     = PBM_MAX_ECC_LEN;
      file_type         = MMGSDI_TRANSPARENT_FILE;

      /* SIM, RUIM and CSIM all have EF-ECC as transparent file with
         max size as 15 */
      if (ecc_evt_info->ecc_data->data.data_len > PBM_NON_USIM_ECC_DATA_LEN_MAX)
      {
        UIM_MSG_ERR_1("Incorrect ecc len 0x%x", ecc_evt_info->ecc_data->data.data_len);
        rex_leave_crit_sect(&pbm_crit_sect);
        return ;
      }

      /* each non-USIM ecc number is of maximum of 3 bytes encoded in BCD on SIM card */
      number_of_records = (uint16)(ecc_evt_info->ecc_data->data.data_len/3);

      /* QMI-PBM interface supports MAX of 255 records. Hence we are limiting
          number of records to be included in card emergency TLV */
      if(number_of_records + PBM_NV_EMERGENCY_NUMBERS_PER_SUB >= PBM_MAX_CARD_ECC)
      {
        nv_ecc_per_sub = PBM_MAX_CARD_ECC - (uint8)number_of_records -1;
      }
      number_of_records += (uint16)nv_ecc_per_sub;

      if(USE_GSM_SUPPORT(session_type))
      {
        number_of_records += CM_MAX_EMERGENCY_NUM_COUNT;
      }
    }

    if((USE_USIM_SUPPORT(session_type) || USE_GSM_SUPPORT(session_type)) &&
        cache->num_of_records < number_of_records)
    {
      /* This means the current EF ECC records are more than the previous
         instance , Hence we need to increase the sizeof cache->pb_cache_array */
      pbm_addr_cache_s_type    **pb_cache_array_temp = NULL;

      PBM_MEM_ALLOC(pb_cache_array_temp, ((uint32) number_of_records + 1) *
                         sizeof(pbm_addr_cache_s_type *));
      if( NULL == pb_cache_array_temp )
      {
        rex_leave_crit_sect(&pbm_crit_sect);
        return ;
      }
      memset(pb_cache_array_temp,0,
             ((uint32) number_of_records + 1) *
                                  sizeof(pbm_addr_cache_s_type *));
      memscpy(pb_cache_array_temp, ((uint32) number_of_records + 1) *
              sizeof(pbm_addr_cache_s_type *), cache->pb_cache_array, ((uint32) cache->num_of_records + 1) *
                       sizeof(pbm_addr_cache_s_type *));

      PBM_MEM_FREEIF(cache->pb_cache_array);
      cache->pb_cache_array = pb_cache_array_temp;
    }
    else if (USE_CDMA_SUPPORT(session_type) || USE_CSIM_SUPPORT(session_type))
    {
      PBM_MEM_FREEIF(cache->pb_cache_array);

      /* CSIM and RUIM ECC SIM cache is not allocated till now */
      PBM_MEM_ALLOC(cache->pb_cache_array,
                    ((uint32) number_of_records + 1) * sizeof(pbm_addr_cache_s_type *));
      if( NULL == cache->pb_cache_array )
      {
        rex_leave_crit_sect(&pbm_crit_sect);
        return ;
      }
    }

    pbm_fill_pb_cache_info(&ecc_sim_cache[session_type],
                           session_type,
                           PBM_USIM_PB_ONE,
                           record_length,
                           PBM_ECC_MIN_LEN,
                           PBM_FILE_ECC,
                           file_type,
                           FALSE,
                           number_of_records,
                           NULL);

    field_id = pbm_file_id_to_field_id(PBM_FILE_ECC);

    for(i = 0; i<ecc_evt_info->ecc_count; i++)
    {
      mmgsdi_data_type    *temp_ecc_data = &ecc_evt_info->ecc_data[i].data;

      if(temp_ecc_data == NULL || temp_ecc_data->data_ptr == NULL)
      {
        continue;
      }

     /* we would need different processing for an USIM and the rest
        since in USIM EC-ECC will be linear fixed and in normal SIM
        it will be a Transparent file */
      if (USE_USIM_SUPPORT(session_type))
      {
        data_length = pbm_ecc_bcd_to_ascii(temp_ecc_data->data_ptr,
                                            number);
        if (data_length <= 0)
        {
          UIM_MSG_ERR_1("SESSION ECC record 0x%x had 0 length", ecc_offset);
        }
        memset(ascii_emergency_alpha, 0, sizeof(ascii_emergency_alpha));

        /* since the last byte denotes Service Category we should not consider that */
        name_length = pbmconvert_sim_to_string(temp_ecc_data->data_ptr + 3,
                                               (uint16)(temp_ecc_data->data_len - 4),
                                               ascii_emergency_alpha,
                                               ARR_SIZE(ascii_emergency_alpha));
        if (name_length > PBM_TEXT_SIZE_CHARS) /*  KW error */
        {
          name_length = PBM_TEXT_SIZE_CHARS;
        }

        /* The last octet is the Service Category */
        emergency_category = temp_ecc_data->data_ptr[temp_ecc_data->data_len - 1];

        if (PBM_SUCCESS != pbm_build_ecc_record(field_id,
                                               (data_length>0?(uint32)data_length + 1:0),
                                                number,
                                                sizeof(uint16) * (name_length?name_length + 1:0),
                                                ascii_emergency_alpha,
                                                TRUE,
                                                emergency_category,
                                                session_type,
                                                 emergency_mode,
                                                 0,
                                                 PBM_SLOT_NONE))
        {
          UIM_MSG_ERR_0("Could not build PBM record");
        }
      }
      else
      {
       /* we need the below special processing since GSM and RUIM EF-ECCs are transparent files,
          and all the ecc codes are appended together in one record
          there exists only one EF ECC record in this case */
        for (j=0; j<cache->num_of_records; j++)
        {
          if (j < (temp_ecc_data->data_len/3))
          {
            data_length = pbm_ecc_bcd_to_ascii((temp_ecc_data->data_ptr + 3*j), number);
            if (data_length <= 0)
            {
              UIM_MSG_ERR_1("SESSION ECC record 0x%x had 0 length", ecc_offset);
            }
            if ((pbm_ret = pbm_build_ecc_record(field_id,
                                                data_length>0?(uint32)data_length + 1:0,
                                                number, 0,
                                                NULL, FALSE, 0,
                                                session_type, emergency_mode,
                                                0, PBM_SLOT_NONE)) !=PBM_SUCCESS)
            {
              UIM_MSG_ERR_0("Could not build PBM record");
            }
          }
        }
      }
      PBM_MEM_FREEIF(ecc_evt_info->ecc_data[i].data.data_ptr);
    }
    PBM_MEM_FREEIF(ecc_evt_info->ecc_data);
  }
  rex_leave_crit_sect(&pbm_crit_sect);

  /* Appending NV slot based numbers as well */
  pbm_build_slot_specific_ecc_cache(session_type, emergency_mode, nv_ecc_per_sub);
  pbm_ecc_set_hardcoded_eccs(TRUE);
  PBM_SET_PB_CACHE_READY(cache);
} /* pbm_update_sim_ecc_cache */


/*===========================================================================
FUNCTION   pbm_cm_ss_event_cb

DESCRIPTION
   This function is called when we get an SS event.

   This function is enhanced to listen the CM SS event which sends the emergency list
   per sub and per stack

DEPENDENCIES
   None

RETURNS
   None

SIDE EFFECTS
===========================================================================*/
void pbm_cm_ss_event_cb (cm_ss_event_e_type ss_event,
                                const cm_mm_msim_ss_info_s_type *ii_info_ptr)
{
  pbm_cm_ss_evt_type *cm_ss_evt_ptr;

  cm_ss_evt_ptr = (pbm_cm_ss_evt_type *)
                    pbm_cmd_alloc(sizeof(pbm_cm_ss_evt_type));
  if (!cm_ss_evt_ptr)
  {
     (void) rex_set_sigs(PBM_TCB, PBM_CMD_COMPLETE_SIG);
     return;
  }
  PBM_MEM_ALLOC(cm_ss_evt_ptr->ii_info_ptr,sizeof(cm_mm_msim_ss_info_s_type));
  PBM_CHECK_PTR_RET(cm_ss_evt_ptr->ii_info_ptr,VOID);
  cm_ss_evt_ptr->cmd_type = PBM_CM_SS_CB;
  cm_ss_evt_ptr->ss_event = ss_event;

  memscpy(cm_ss_evt_ptr->ii_info_ptr, sizeof(cm_mm_msim_ss_info_s_type), ii_info_ptr,sizeof(cm_mm_msim_ss_info_s_type));

  pbm_mmgsdi_resp_q_put(PBM_CMD_PTR(cm_ss_evt_ptr));

}/* pbm_cm_ss_event_cb */


/*===========================================================================
FUNCTION PBM_FIND_ECC_CAT_IN_REC

DESCRIPTION
  This function searches a given record id, assumed to be in the ECC
  phonebook for the ecc category.

  If it finds the field, it returns TRUE, and fills in ecc_category_ptr and
  found_field_ptr.

DEPENDENCIES

RETURN VALUE
  Boolean

SIDE EFFECTS
  none
===========================================================================*/
boolean pbm_find_ecc_cat_in_rec(
  pbm_record_id_type     rec_id,
  uint8                 *ecc_category_ptr,
  pbm_field_id_e_type   *found_field_ptr,
  pbm_ecc_mode_type     *emergency_mode,
  uint8                 *srv_status_flag_ptr
)
{
  uint8                  i           = 0;
  pbm_return_type        pbm_ret_val = PBM_ERROR;
  boolean                ret         = FALSE;
  pbm_field_s_type      *fields_buf  = NULL;
  uint32                 fields_size = 0;
  uint16                 category    = 0;
  int                    item_count  = 0;
  void                  *data_ptr    = NULL;

  PBM_CHECK_PTR4_RET(ecc_category_ptr, found_field_ptr, emergency_mode, srv_status_flag_ptr, FALSE);

  /* See if we can find a category */
  fields_size = (uint32)pbm_session_calculate_fields_size_from_id(rec_id);
  if (fields_size == 0)
  {
    UIM_MSG_ERR_1("invalid record passed to pbm_find_ecc_cat_in_rec 0x%x", rec_id);
    return FALSE;
  }

  PBM_MEM_ALLOC(fields_buf, fields_size);
  PBM_CHECK_PTR_RET(fields_buf, FALSE);

  pbm_ret_val = pbm_session_record_read(rec_id, &category, &item_count,
                                        (uint8*)fields_buf, fields_size);
  if (pbm_ret_val != PBM_SUCCESS)
  {
    UIM_MSG_ERR_1("pbm_record_read() failed with error 0x%x.", pbm_ret_val);
    PBM_MEM_FREEIF(fields_buf);
    return ret;
  }

  /* see if we can find the ecc cat field */
  for (i = 0; i < item_count; i++)
  {
    switch(fields_buf[i].field_id)
    {
      case PBM_FIELD_ECC_SERVICE_CATEGORY:
        data_ptr = pbm_get_field_data_ptr(fields_buf, i);
        PBM_CHECK_PTR_BREAK(data_ptr, ret, FALSE);

        *ecc_category_ptr = *(uint8*)data_ptr;
        ret = TRUE;
        break;

      case PBM_FIELD_EMERGENCY_MODE:
        data_ptr = pbm_get_field_data_ptr(fields_buf, i);
        PBM_CHECK_PTR_BREAK(data_ptr, ret, FALSE);

        *emergency_mode = *(uint8*)data_ptr;
        break;

      case PBM_FIELD_NETWORK_ECC:
      case PBM_FIELD_HARDCODED_ECC:
      case PBM_FIELD_NV_ECC:
      case PBM_FIELD_SIM_ECC:
      case PBM_FIELD_SERVING_MCC_HARDCODED_ECC:
        *found_field_ptr = fields_buf[i].field_id;
        break;

      /* See if we can find the service status flag field */
      case PBM_FIELD_SRV_STATUS_FLAG:
        data_ptr = pbm_get_field_data_ptr(fields_buf, i);
        PBM_CHECK_PTR_BREAK(data_ptr, ret, FALSE);

        *srv_status_flag_ptr = *(uint8*)data_ptr;
        break;

      default:
        break;
    }
  }

  UIM_MSG_HIGH_4("Found ECC cat 0x%x with field type 0x%x emergency_mode 0x%x srv_status_flag 0x%x",
                  *ecc_category_ptr, *found_field_ptr, *emergency_mode, *srv_status_flag_ptr);
  PBM_MEM_FREEIF(fields_buf);
  return ret;
}/* pbm_find_ecc_cat_in_rec */

