/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                      PDCOMM NV Items Module

GENERAL DESCRIPTION
  This module contains the PDCOMMS interface routines for the Non-Volatile Memory
  module.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


  Copyright (c) 2011 - 2012 Qualcomm Technologies Incorporated.
  Qualcomm Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2014 Qualcomm Atheros, Inc.
  Qualcomm Atheros Confidential and Proprietary. All Rights Reserved. 
  Copyright (c) 2016-19 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

Version Control

  $Header: //components/rel/gnss8.mpss/11.1/gnss/pd_comms/src/pd_comms_nv_efs.c#1 $
  $DateTime: 2019/10/24 01:30:36 $
  $Author: pwbldsvc $

=============================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when       who          what, where, why
--------   ---          ----------------------------------------------------------
09/30/19   skm          Support for 911 nDDS on DSDS targets
04/10/18   py           variable to keep track of current DD sub and its get and set function
09/13/10   ssheshad     Initial Release.

=============================================================================

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
#include "pd_comms_nv_efs.h"
#include "msg.h"
#include "pdapi.h"
#include "stringl.h"

sys_modem_as_id_e_type pd_comms_nv_current_sub = PD_COMMS_CONFIG_MIN_SUB_ID;

/* ensure the order of the items in this table is same as the item definitions */
/* CGPS_NV_EFS_REG_ST_ITEM_INTERNAL - EFS path is /cgps/nv/item_files/... */
/* CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL - EFS path is /nv/item_files/cgps/nv/item_files/... */
cgps_nv_efs_reg_item_struct pdcomm_nv_reg_items[] =
{
  /* data location, item file name, size */

  /*CFPS_NV_EFS_PDCOMM_APN_PROFILES , */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "pdcomms/gnss_pd_comms_lbs_apn_profile_list",
    sizeof(pdsm_pa_apn_profiles_type) * PDSM_PA_MAX_APN_PROFILES, 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },

  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "pdcomms/gnss_pd_comms_xtra_apn_profile_list",
    sizeof(pdsm_pa_apn_profiles_type) * PDSM_PA_MAX_APN_PROFILES, 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },


  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "pdcomms/gnss_pd_comms_emergency_lbs_apn_profile_list",
    sizeof(pdsm_pa_apn_profiles_type) * PDSM_PA_MAX_APN_PROFILES, 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL },


  /* Default RSA Certificate Key length */
  { CGPS_NV_EFS_REG_ST_ITEM_EXTERNAL, "pdcomms/gnss_pd_comms_ssl_cert_key_length",
    sizeof(uint16), 0, CGPS_NV_EFS_REG_TYPE_INTEGRAL }
  /* add entries below this line and make sure you update the CGPS_NV_EFS_PDCOMMS_MAX_ITEM*/
};


/*RAM Copy of the NV's*/

typedef struct
{
  pdsm_pa_apn_profiles_type gnss_pd_comms_lbs_apn_profile_list[PDSM_PA_MAX_APN_PROFILES];
  pdsm_pa_apn_profiles_type gnss_pd_comms_xtra_apn_profile_list[PDSM_PA_MAX_APN_PROFILES];
  pdsm_pa_apn_profiles_type gnss_pd_comms_emergency_lbs_apn_profile_list[PDSM_PA_MAX_APN_PROFILES];
  uint16                    gnss_pd_comms_ssl_certificate_keysize;
} pd_comms_nv_cached_struct_type;


static pd_comms_nv_cached_struct_type pd_comms_nv_cached_values[PD_COMMS_CONFIG_MAX_SUBS];

/*===========================================================================

FUNCTION pd_comms_nv_load_default_values_to_cache

DESCRIPTION
  This function is used to load default values to cache
  
RETURN VALUE
  None
  
DEPENDENCIES
 
===========================================================================*/
void pd_comms_nv_load_default_values_to_cache(void)
{
  sys_modem_as_id_e_type sub = PD_COMMS_CONFIG_MIN_SUB_ID;
  for(;sub<PD_COMMS_CONFIG_MAX_SUBS;sub++)
  {
    memset((void *)pd_comms_nv_cached_values[sub].gnss_pd_comms_lbs_apn_profile_list, 0, sizeof(pd_comms_nv_cached_values[sub].gnss_pd_comms_lbs_apn_profile_list));
    memset((void *)pd_comms_nv_cached_values[sub].gnss_pd_comms_xtra_apn_profile_list, 0, sizeof(pd_comms_nv_cached_values[sub].gnss_pd_comms_xtra_apn_profile_list));
    memset((void *)pd_comms_nv_cached_values[sub].gnss_pd_comms_emergency_lbs_apn_profile_list, 0, sizeof(pd_comms_nv_cached_values[sub].gnss_pd_comms_emergency_lbs_apn_profile_list));
  }
}


/*===========================================================================

FUNCTION pd_comms_nv_efs_init

DESCRIPTION
  This function is used to initialize internal PDCOMM-NV data efs structure.
  
RETURN VALUE
  None
  
DEPENDENCIES
 
===========================================================================*/
void pd_comms_nv_efs_init(void)
{
  boolean ret_val = TRUE;
  ret_val =  cgps_nv_efs_init(CGPS_NV_EFS_PDCOMM, CGPS_NV_EFS_PDCOMMS_MIN_ITEM,
                              CGPS_NV_EFS_PDCOMMS_MAX_ITEM,
                              pdcomm_nv_reg_items);
  if (ret_val)
  {
    MSG_3(MSG_SSID_GPS, MSG_LEGACY_MED, "PDCOMMS_NV_EFS: NV EFS init successful", 0, 0, 0);
    pd_comms_nv_load_default_values_to_cache();
  }
  else
  {
    MSG_3(MSG_SSID_GPS, MSG_LEGACY_MED, "SM_NV_EFS: NV EFS init failed", 0, 0, 0);
  }
}

/*===========================================================================

FUNCTION pd_comms_nv_efs_reg_item_read

DESCRIPTION
  This function is used to read NV item. If the read fails, it sets it to
  default value.
  
RETURN VALUE
  None
  
DEPENDENCIES
 
===========================================================================*/
boolean pd_comms_nv_efs_reg_item_read(cgps_nv_efs_reg_item_type const item,
                                      uint32 data_size,
                                      void *data_ptr,
                                      boolean force_read_from_hw,
                                      sys_modem_as_id_e_type sub
                                      )
{
  cgps_nv_efs_reg_rw_stat_enum_type status = CGPS_NV_EFS_REG_RW_STAT_INVALID;
  boolean return_value = FALSE;
  boolean nv_read_needed = TRUE;

  if (!force_read_from_hw)
  {
    /* We can read from the cache. Do so! */
    switch (item)
    {
      case CGPS_NV_EFS_PDCOMM_LBS_APN_PROFILES:
        memscpy(data_ptr, data_size, (void *)pd_comms_nv_cached_values[sub].gnss_pd_comms_lbs_apn_profile_list,
                sizeof(pd_comms_nv_cached_values[sub].gnss_pd_comms_lbs_apn_profile_list));
        nv_read_needed = FALSE;
        break;
      case CGPS_NV_EFS_PDCOMM_XTRA_APN_PROFILES:
        memscpy(data_ptr, data_size, (void *)pd_comms_nv_cached_values[sub].gnss_pd_comms_xtra_apn_profile_list,
                sizeof(pd_comms_nv_cached_values[sub].gnss_pd_comms_xtra_apn_profile_list));
        nv_read_needed = FALSE;
        break;

      case CGPS_NV_EFS_PDCOMM_EMERGENCY_LBS_APN_PROFILES:
        memscpy(data_ptr, data_size, (void *)pd_comms_nv_cached_values[sub].gnss_pd_comms_emergency_lbs_apn_profile_list,
                sizeof(pd_comms_nv_cached_values[sub].gnss_pd_comms_emergency_lbs_apn_profile_list));
        nv_read_needed = FALSE;
        break;
#if 0
      case CGPS_NV_EFS_PDCOMM_SSL_CERTIFICATE_KEY_LENGTH:
        memscpy(data_ptr, data_size, (void*)pd_comms_nv_cached_values[sub].gnss_pd_comms_ssl_certificate_keysize,
                sizeof(pd_comms_nv_cached_values[sub].gnss_pd_comms_ssl_certificate_keysize));
        nv_read_needed = FALSE;
        break;
#endif

      default:
        break;
    }
  }

  if (nv_read_needed)
  {
    if ((item >= CGPS_NV_EFS_PDCOMMS_MIN_ITEM) &&
        (item <= CGPS_NV_EFS_PDCOMMS_MAX_ITEM))
    {      
      status =  cgps_nv_efs_reg_item_read_subsid(item, data_size, data_ptr,
                                          CGPS_NV_EFS_PDCOMMS_MIN_ITEM,
                                          CGPS_NV_EFS_PDCOMMS_MAX_ITEM, pdcomm_nv_reg_items,(cgps_nv_efs_sub_id_type)sub);
    }
    else
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "PDCOMM NV read item index %d, out of range", (int)item);
    }

    /* Update the cache with the read value */
    if (status == CGPS_NV_EFS_REG_RW_STAT_OK)
    {
      switch (item)
      {
        case CGPS_NV_EFS_PDCOMM_LBS_APN_PROFILES:
          memscpy((void *)pd_comms_nv_cached_values[sub].gnss_pd_comms_lbs_apn_profile_list,
                  sizeof(pd_comms_nv_cached_values[sub].gnss_pd_comms_lbs_apn_profile_list),
                  data_ptr, data_size);
          break;
        case CGPS_NV_EFS_PDCOMM_XTRA_APN_PROFILES:
          memscpy((void *)pd_comms_nv_cached_values[sub].gnss_pd_comms_xtra_apn_profile_list,
                  sizeof(pd_comms_nv_cached_values[sub].gnss_pd_comms_xtra_apn_profile_list),
                  data_ptr, data_size);
          break;

        case CGPS_NV_EFS_PDCOMM_EMERGENCY_LBS_APN_PROFILES:
          memscpy((void *)pd_comms_nv_cached_values[sub].gnss_pd_comms_emergency_lbs_apn_profile_list,
                  sizeof(pd_comms_nv_cached_values[sub].gnss_pd_comms_emergency_lbs_apn_profile_list),
                  data_ptr, data_size);
          break;
#if 0
        case CGPS_NV_EFS_PDCOMM_SSL_CERTIFICATE_KEY_LENGTH:
          memscpy((void*)pd_comms_nv_cached_values[sub].gnss_pd_comms_ssl_certificate_keysize,
                  sizeof(pd_comms_nv_cached_values[sub].gnss_pd_comms_ssl_certificate_keysize),
                  data_ptr,data_size);
          break;
#endif
        default:
          break;
      }
    }
    MSG_3(MSG_SSID_GPS, MSG_LEGACY_MED, "PDCOMM NV read from Hw", 0, 0, 0);
  }
  else
  {
    MSG_3(MSG_SSID_GPS, MSG_LEGACY_MED, "PDCOMM NV read from Cache", 0, 0, 0);
    status = CGPS_NV_EFS_REG_RW_STAT_OK;
  }

  if (status == CGPS_NV_EFS_REG_RW_STAT_OK)
  {
    MSG_3(MSG_SSID_GPS, MSG_LEGACY_MED, "PDCOMM_NV_EFS: NV EFS file read succeeded for item = %d", item, 0, 0);
    return_value = TRUE;
  }
  return return_value;

}

/*===========================================================================

FUNCTION pd_comms_nv_efs_reg_item_write

DESCRIPTION
  This function is used to write an NV item. 
 
  
RETURN VALUE
  None
  
DEPENDENCIES
 
===========================================================================*/
boolean pd_comms_nv_efs_reg_item_write(cgps_nv_efs_reg_item_type const item,
                                       uint32 data_size,
                                       void *data_ptr
                                       )
{
  cgps_nv_efs_reg_rw_stat_enum_type status = CGPS_NV_EFS_REG_RW_STAT_INVALID;
  boolean return_value = TRUE;
  boolean nv_write_needed = FALSE;
  sys_modem_as_id_e_type sub = pd_comms_nv_efs_get_current_dd_sub();

  if ((sub  < PD_COMMS_CONFIG_MIN_SUB_ID) || (sub  > PD_COMMS_CONFIG_MAX_SUB_ID))
  {
    MSG_1(MSG_SSID_GPS, MSG_LEGACY_ERROR, "PD_COMMS: Invalid Subs ID ​%d ", sub );
    return FALSE;
  }

  
  switch (item)
  {
    case CGPS_NV_EFS_PDCOMM_LBS_APN_PROFILES:
      memscpy((void *)pd_comms_nv_cached_values[sub].gnss_pd_comms_lbs_apn_profile_list,
              sizeof(pd_comms_nv_cached_values[sub].gnss_pd_comms_lbs_apn_profile_list),
              data_ptr, data_size);
      nv_write_needed = TRUE;
      break;

    case CGPS_NV_EFS_PDCOMM_XTRA_APN_PROFILES:
      memscpy((void *)pd_comms_nv_cached_values[sub].gnss_pd_comms_xtra_apn_profile_list,
              sizeof(pd_comms_nv_cached_values[sub].gnss_pd_comms_xtra_apn_profile_list),
              data_ptr, data_size);
      nv_write_needed = TRUE;
      break;

    case CGPS_NV_EFS_PDCOMM_EMERGENCY_LBS_APN_PROFILES:
      memscpy((void *)pd_comms_nv_cached_values[sub].gnss_pd_comms_emergency_lbs_apn_profile_list,
              sizeof(pd_comms_nv_cached_values[sub].gnss_pd_comms_emergency_lbs_apn_profile_list),
              data_ptr, data_size);
      nv_write_needed = TRUE;
      break;
#if 0
    case CGPS_NV_EFS_PDCOMM_SSL_CERTIFICATE_KEY_LENGTH:
      memscpy((void*)pd_comms_nv_cached_values.gnss_pd_comms_ssl_certificate_keysize,
              sizeof(pd_comms_nv_cached_values.gnss_pd_comms_ssl_certificate_keysize),
              data_ptr, data_size);
      nv_write_needed = TRUE;
      break;
#endif
    default:
      break;
  }

#ifdef FEATURE_GNSS_NO_NV_WRITES
  #error code not present
#endif

  if (nv_write_needed)
  {
    if ((item >= CGPS_NV_EFS_PDCOMMS_MIN_ITEM) &&
        (item <= CGPS_NV_EFS_PDCOMMS_MAX_ITEM))
    {
      status =  cgps_nv_efs_reg_item_async_write_subsid(
         item, data_size, data_ptr,
         CGPS_NV_EFS_PDCOMMS_MIN_ITEM,
         CGPS_NV_EFS_PDCOMMS_MAX_ITEM, pdcomm_nv_reg_items,
         (cgps_nv_efs_sub_id_type)sub);
    }
    else
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "PDCOMM NV write item index %d, out of range", (int)item);
    }
  }
  else
  {
    status = CGPS_NV_EFS_REG_RW_STAT_OK;
  }

  if (status == CGPS_NV_EFS_REG_RW_STAT_OK)
  {
    MSG_3(MSG_SSID_GPS, MSG_LEGACY_MED, "PDCOMM_NV_EFS: NV EFS file write succeeded for item = %d", item, 0, 0);
    return_value = TRUE;
  }
  else
  {
    MSG_3(MSG_SSID_GPS, MSG_LEGACY_MED, "PDCOMM_NV_EFS: NV EFS file write failed for item = %d", item, 0, 0);
    return_value = FALSE;
  }
  return return_value;

}

/*===========================================================================

FUNCTION pd_comms_nv_set_current_sub_id

DESCRIPTION
     This function is used to set the current default subscription ID(usually Dedicated-Data sub). When EFS NV 
  read/write is performed without giving specific Subs ID, this current Subs ID is will be used. 
  
RETURN VALUE
  None
  
DEPENDENCIES
 
===========================================================================*/
void pd_comms_nv_efs_set_current_dd_sub(sys_modem_as_id_e_type e_sub_id)
{
  if ((e_sub_id < PD_COMMS_CONFIG_MIN_SUB_ID) || (e_sub_id > PD_COMMS_CONFIG_MAX_SUB_ID))
  {
    MSG_1(MSG_SSID_GPS, MSG_LEGACY_ERROR, "Invalid Subs ID ​%d ", e_sub_id);
    return;
  }
  pd_comms_nv_current_sub = e_sub_id;
  MSG_1(MSG_SSID_GPS, MSG_LEGACY_MED, "PDCOMM :Current sub_id updated to =%d", pd_comms_nv_current_sub);
  return;
}
/*===========================================================================

FUNCTION pd_comms_nv_get_current_sub_id

DESCRIPTION
     This function is used to get the current subscription ID(usually Dedicated-Data sub).
  
RETURN VALUE
  None
  
DEPENDENCIES
 
===========================================================================*/
sys_modem_as_id_e_type pd_comms_nv_efs_get_current_dd_sub()
{
  return (pd_comms_nv_current_sub);
}


