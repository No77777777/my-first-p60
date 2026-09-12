/*===========================================================================
  FILE: ds_Profile_DBMgr.cpp

  OVERVIEW: This file provides implementation of the DBMgr class.

  DEPENDENCIES: None

  Copyright (c) 2013 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================
EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2013-01-16 ez Created module
  2013-04-10 am Added implementation

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "ds_profile_db.h"
#include "ds_Profile_DBMgr.h"
#include "ds_Profile_Profile.h"
#include "ds_Profile_TechMgr.h"
#include "ds_Profile_File.h"
#include "ds_Profile_FileEFS.h"
#include "ds_Profile_TechMgr3GPP.h"
#include "ds_Profile_TechMgr3GPP2.h"
#include "ds_Profile_Platform.h"
#include "ds_Profile_Log.h"
#include "ps_system_heap.h"
#include "ds_Profile_NVMgr.h"

extern "C"
{
#include "ds3geventmgr.h"
#include "ds_profile_3gpp2_mgr.h"
#include "ds3gsubsmgr.h"
#include "ds_profile_db_internal.h"
}

#include <stringl/stringl.h>

using namespace ds::Profile;

DBMgr *DBMgr::theInstance = NULL;

/*===========================================================================

                     GLOBAL DECLARATIONS

===========================================================================*/


/*===========================================================================

                     Internal FUNCTIONS

===========================================================================*/
static ds_profile_db_result_type ValidateCreationParams(const ds_profile_db_creation_params_type *params)
{
  ds_profile_db_result_type res;
  if (DB_UNSPECIFIED_PROFILE_NUM != params->profile_num)
  {
    if (DB_RESULT_SUCCESS != (res = Profile::ValidateProfileNum(params->profile_num)))
    {
      return res;
    }
  }

  return Profile::ValidateSubscriptionId(params->subscription_id);
}

static inline bool WithinNumberPoolRange(ds_profile_db_profile_num_type num)
{
  return ((num >= TechMgr::ALLOC_RANGE_MIN) && (num < TechMgr::ALLOC_RANGE_MAX));
}
/*===========================================================================

                     MEMBER FUNCTIONS

===========================================================================*/
DBMgr* DBMgr::Instance(void)
{
  return theInstance;
}

DBMgr::DBMgr(void) : mutex(NULL), callbackMgr(NULL),
  profiles_for_power_up(NULL), profiles_count_for_power_up(0)
{
  memset(techMgrs, 0, sizeof(techMgrs));
  memset(numbersPool, false, sizeof(numbersPool));
  memset(subsProfilesReady, false, sizeof(subsProfilesReady));
}

static void _internal_deinit(void)
{
  DBMgr::Deinit();
}

ds_profile_db_result_type DBMgr::Init(void)
{
  PROFILE_DB_LOG_HIGH("Init(): started");
  ds_profile_db_result_type res = DB_RESULT_SUCCESS;
  boolean is_sfs = false;
  NVMgr *nvMgr  = NULL;
  uint8 subs_id = 0;
  ds3gsubsmgr_client_type  ds_profile_db_subs_client_id;
  ds3geventmgr_filter_type        filter_info;
  void *heapForFile = NULL;
  typedef enum
  {
    DS_PROFILE_DB_LOCAL_ERR_NONE         = 0,
    DS_PROFILE_DB_LOCAL_ERR_DEVICE_MODE  = 1,
    DS_PROFILE_DB_LOCAL_ERR_CARD_REFRESH = 2,
    DS_PROFILE_DB_LOCAL_ERR_ATTACH_PROFILE = 3
  } ds_profile_db_local_err_type;

  ds_profile_db_local_err_type status = 
    DS_PROFILE_DB_LOCAL_ERR_NONE;

  if (theInstance)
  {
    return DB_RESULT_ERR_INVAL;
  }

  if (DB_RESULT_SUCCESS != (res = Platform::Instance()->Init()))
  {
    return res;
  }

  /*Alloc NV MGR*/
  nvMgr = new NVMgr();
  if (!nvMgr)
  {
    return DB_RESULT_ERR_OUT_OF_MEMORY;
  }

  NVMgr::ds_profile_db_nv_init_efs_conf_file();

  //Read NV here and update DBMgr
  nvMgr->ReadFromNV(DS_PROFILE_DB_ENABLE_SFS,
                             &is_sfs,
                             sizeof(is_sfs),
                             DB_PROFILE_ACTIVE_SUBSCRIPTION_1);
  
  Platform::Instance()->SetIsSFS(is_sfs, DB_PROFILE_ACTIVE_SUBSCRIPTION_1);
  nvMgr->ReadFromNV(DS_PROFILE_DB_ENABLE_SFS,
                    &is_sfs,
                    sizeof(is_sfs),
                    DB_PROFILE_ACTIVE_SUBSCRIPTION_2);

  //Set no matter what
  Platform::Instance()->SetIsSFS(is_sfs, DB_PROFILE_ACTIVE_SUBSCRIPTION_2);
  PS_SYSTEM_HEAP_MEM_ALLOC(heapForFile, MAX_TLV_SIZE, void*);

  if (heapForFile == NULL)
  {
    return DB_RESULT_ERR_OUT_OF_MEMORY;
  }

  ds_profile_db_set_heap_mem_for_file(heapForFile);
  theInstance = new DBMgr();
  if (!theInstance)
  {
    return DB_RESULT_ERR_OUT_OF_MEMORY;
  }

  do 
  {
    theInstance->callbackMgr = Platform::Instance()->GetCallbackMgrSingleton();
    if (!theInstance->callbackMgr)
    {
      res = DB_RESULT_FAIL;
      break;
    }

    if (!(theInstance->mutex = Platform::Instance()->GetMutex()))
    {
      res = DB_RESULT_ERR_OUT_OF_MEMORY;
      break;
    }

    theInstance->techMgrs[DB_TECH_3GPP] = new TechMgr3GPP();
    if (!theInstance->techMgrs[DB_TECH_3GPP])
    {
      res = DB_RESULT_ERR_OUT_OF_MEMORY;
      break;
    }
    #ifdef FEATURE_DATA_IS707
    theInstance->techMgrs[DB_TECH_3GPP2] = new TechMgr3GPP2();
    if (!theInstance->techMgrs[DB_TECH_3GPP2])
    {
      res = DB_RESULT_ERR_OUT_OF_MEMORY;
      break;
    }
    #endif

    /*Read the efs file and store in the member variable in NV-MGR*/
    (void)nvMgr->ReadEFSFile(DB_EFS_FILE_TYPE_EPC_SUPPORT);

    /*Set the value of EPC support variable, irrespectively */
    theInstance->SetEPCSupportVal(nvMgr->GetEPCSupportVal());

    if (DB_RESULT_SUCCESS != (res = PowerUpSequence()))
    {
      break;
    }

  memset(theInstance->ids, 
         0,
         sizeof(powerup_Mh_Cb_Id)*DS_PROFILE_NUM_MODE_HANDLERS);

    /* should not be enabled, as it could try to use platform resources which were already
       released by other cleanup procedures */
    //(void)atexit(_internal_deinit);
    memset(&filter_info, 0, sizeof(ds3geventmgr_filter_type));
    filter_info.tech = PS_SYS_TECH_ALL;

    for(subs_id = 0; subs_id < DS3GSUBSMGR_SUBS_ID_COUNT; subs_id++)
    {
      filter_info.ps_subs_id = (ps_sys_subscription_enum_type)
         ds3gsubsmgr_subs_id_cm_to_ds((sys_modem_as_id_e_type)subs_id);
      if (ds3geventmgr_event_reg(DS3GEVENTMGR_NV_REFRESH_EV,
                                 DS3GEVENTMGR_CLIENT_ID_PROFILE,
                                 &filter_info,
                                 ds_profile_db_ds3g_cmd_hdlr,
                                 NULL ) == FALSE)
      {
        status = DS_PROFILE_DB_LOCAL_ERR_CARD_REFRESH;
        res = DB_RESULT_FAIL;
        break;
      }
    }

    /*-------------------------------------------------------------------------
      Register for subsmgr events
    -------------------------------------------------------------------------*/
    if (ds3gsubsmgr_event_reg(DS3GSUBSMGR_DEVICE_MODE_EV,
                              ds_profile_db_ds3g_subsmgr_event_cb,
                              NULL,
                              &ds_profile_db_subs_client_id) == FALSE)
    {
      status = DS_PROFILE_DB_LOCAL_ERR_DEVICE_MODE;
      res = DB_RESULT_FAIL;
      break;
    }

    memset(&filter_info, 0, sizeof(ds3geventmgr_filter_type));
    filter_info.tech = PS_SYS_TECH_ALL;

    for(subs_id = 0; subs_id < DS3GSUBSMGR_SUBS_ID_COUNT; subs_id++)
    {
      filter_info.ps_subs_id = (ps_sys_subscription_enum_type)
        ds3gsubsmgr_subs_id_cm_to_ds((sys_modem_as_id_e_type)subs_id);
      if (ds3geventmgr_event_reg(DS3GEVENTMGR_APM_NEW_ATTACH_PROFILE_EV,
                                 DS3GEVENTMGR_CLIENT_ID_PROFILE,
                                 &filter_info,
                                 ds_profile_db_ds3g_cmd_hdlr,
                                 NULL ) == FALSE)
      {
        status = DS_PROFILE_DB_LOCAL_ERR_ATTACH_PROFILE;
        res = DB_RESULT_FAIL;
        break;
      }
    }

    res = DB_RESULT_SUCCESS;

  } while (0);


  if (DB_RESULT_SUCCESS != res) 
  {
    delete theInstance;
    theInstance = NULL;
    Platform::Instance()->Deinit();
  }

  PROFILE_DB_LOG_HIGH_2("Profile DB Init Exiting with error global error %d, "
                        "local error %d",res, status);

  return res;
}

ds_profile_db_result_type DBMgr::PowerUpSequence(void)
{
  Profile **profiles = NULL;
  unsigned int profiles_count = DS_PROFILE_DB_MAX_PROFILE_COUNT;
  ds_profile_db_result_type res;
  unsigned int created_on_the_fly = 0;
  ds_profile_db_subs_type subs = 
    (ds_profile_db_subs_type)ps_sys_get_default_data_subscription();

  PS_SYSTEM_HEAP_MEM_ALLOC(profiles, profiles_count * sizeof(Profile*),
                           Profile**);
  if (NULL == profiles) {
    res = DB_RESULT_ERR_OUT_OF_MEMORY;
    goto bail;
  }
  memset(profiles, 0, profiles_count * sizeof(Profile *));

  /* mark in cache special profiles (KDDI / OMH / etc) as if they exist, but do not create them */
  if (DB_RESULT_SUCCESS != (res = theInstance->MarkSpecialProfilesInCache(subs)))
  {
    goto bail;
  }

  /* read all profiles from EFS */
  if (DB_RESULT_SUCCESS != (res = theInstance->FromFileToCache(profiles, &profiles_count, subs)))
  {
    goto bail;
  }

  /* create some special profiles on the fly */
  created_on_the_fly = DS_PROFILE_DB_MAX_PROFILE_COUNT - profiles_count;
  if (DB_RESULT_SUCCESS != (res = theInstance->OnTheFlyToCache(profiles + profiles_count, &created_on_the_fly, subs)))
  {
    goto bail;
  }
  profiles_count += created_on_the_fly;

  theInstance->profiles_for_power_up = profiles;
  theInstance->profiles_count_for_power_up = profiles_count;
  theInstance->subsProfilesReady[subs] = TRUE;

  /* Notify to clients if any */
  if (DB_RESULT_SUCCESS != 
      (res = theInstance->callbackMgr->Notify(NULL, 0, DB_PROFILES_READY_ON_SUBS_EVENT, subs)))
  {
     PROFILE_DB_LOG_ERROR("PowerUpSequence(): Failed to Notify clients about profiles ready on subs");
     //goto bail;
  }
  
  res = DB_RESULT_SUCCESS;

bail:

if (res != DB_RESULT_SUCCESS)
{
  /* in case of error, also clean cache and release numbers */
  theInstance->CleanupProfiles(DS_PROFILE_DB_MAX_PROFILE_COUNT, profiles, true, true);
  theInstance->profiles_for_power_up = NULL;
  theInstance->profiles_count_for_power_up = 0;
}
  PROFILE_DB_LOG_HIGH_3("PowerUpSequence(): Finished with err %d, read %d profiles from EFS, "
                        "created in cache %d profiles on the fly",
                        res, profiles_count - created_on_the_fly, created_on_the_fly);

  return res;
}

ds_profile_db_result_type DBMgr::MarkSpecialProfilesInCache(ds_profile_db_subs_type subs)
{
#ifdef FEATURE_DATA_IS707
  ds_profile_db_profile_num_type kddi_min_num = 0, kddi_max_num = 0;
  ds_profile_db_profile_num_type omh_min_num = 0, omh_max_num = 0;
  ds_profile_db_result_type res;
#endif

  /* KDDI */
  /* on error, ignore */
#ifdef FEATURE_DATA_IS707
  res = ds_profile_3gpp2_mgr_get_profiles_range(DB_3GPP2_PROFILE_KDDI,
                                                &kddi_min_num,
                                                &kddi_max_num);


  if (DB_RESULT_SUCCESS == res)
  {
    if (DB_RESULT_SUCCESS != 
        (res = MarkRangeInCacheDuringInit(DB_TECH_TO_TECHMASK(DB_TECH_3GPP2),
                                          kddi_min_num, kddi_max_num, 
                                          false, subs)))
    {
      goto bail;
    }
  }


  /* OMH */
  /* on error, ignore */
  res = ds_profile_3gpp2_mgr_get_profiles_range(DB_3GPP2_PROFILE_OMH,
                                                &omh_min_num,
                                                &omh_max_num);

  if (DB_RESULT_SUCCESS == res)
  {
    if (DB_RESULT_SUCCESS != 
        (res = MarkRangeInCacheDuringInit(DB_TECH_TO_TECHMASK(DB_TECH_3GPP2),
                                          omh_min_num, omh_max_num, 
                                          false, subs)))
    {
      goto bail;
    }
  }
  return DB_RESULT_SUCCESS;

bail:

  ReleaseRangeInCache(kddi_min_num, 
                      kddi_max_num,
                       DB_TECH_TO_TECHMASK(DB_TECH_3GPP2), 
                      false, 
                      subs);
  ReleaseRangeInCache(omh_min_num, 
                      omh_max_num, 
                      DB_TECH_TO_TECHMASK(DB_TECH_3GPP2), 
                      false, 
                      subs);

  return res;
#else
  return DB_RESULT_SUCCESS;
#endif
}

void DBMgr::PowerUpFinished(void)
{
  PROFILE_DB_LOG_HIGH("PowerUpFinished(): started");

  if (callbackMgr->WithinCallback())
  {
    return;
  }

  if (0 != mutex->Lock())
  {
    return;
  }
  AutoReleseMutex a_m(mutex);

  if (profiles_for_power_up)
  {
    CleanupProfiles(profiles_count_for_power_up, 
                    profiles_for_power_up);
  }

  profiles_for_power_up = NULL;
  profiles_count_for_power_up = 0;
}

void DBMgr::Deinit(void)
{
  theInstance->PowerUpFinished();

  Platform::Instance()->Deinit();

  delete theInstance;
  theInstance = NULL;
}

DBMgr::~DBMgr(void)
{
  /* release tech managers */
  TechMgr *techMgr = NULL;
  for (int tech = 0; tech < DB_TECH_MAX; tech++)
  {
    if ((techMgr = (GetTechManager((ds_profile_db_tech_type)tech))))
    {
      delete techMgr;
    }
  }
  _internal_deinit();
  delete mutex;
}

ds_profile_db_result_type DBMgr::CreateProfile(
  const ds_profile_db_creation_params_type *params,
  Profile *profile)
{
  bool new_profile = false;
  bool num_specified = false;
  boolean enable_deleting_default_profile = true;

  if (!profile || !params)
  {
    return DB_RESULT_ERR_INVAL_OP;
  }

  if (callbackMgr->WithinCallback())
  {
    return DB_RESULT_ERR_INVAL_OP;
  }

  if (0 != mutex->Lock())
  {
    return DB_RESULT_FAIL;
  }
  AutoReleseMutex a_m(mutex);


  ds_profile_db_result_type res = ValidateCreationParams(params);
  if (DB_RESULT_SUCCESS != res)
  {
    return res;
  }

  /* profile should have an unspecified number*/
  if (DB_UNSPECIFIED_PROFILE_NUM != profile->GetNum())
  {
    return DB_RESULT_ERR_INVAL_PROFILE_NUM;
  }

  File *file = NULL;

  ds_profile_db_profile_num_type new_num = params->profile_num;

  /* set all the creation params  */
  //  to avoid warning C4800: conversion between boolean to bool
  profile->SetPersistency((params->persistent) != 0);


  if (DB_RESULT_SUCCESS != (res = profile->SetSubscriptionId(params->subscription_id, 
                                                             false)))
  {
    goto bail;
  }

  if (DB_RESULT_SUCCESS != (res = profile->SetProfileclient(params->profile_client, false)))
  {
    goto bail;
  }

  if (DB_UNSPECIFIED_PROFILE_NUM != new_num)
  {
    profile->SetNumber(new_num);
    num_specified = true;
  }

  /* the checks whether the profile can be created for a specific number are done inside */
  if (DB_RESULT_SUCCESS != (res = GetNumForProfile(profile, 
                                                   &new_num, 
                                                   params->subscription_id, 
                                                   false)))
  {
    goto bail;
  }

  /* This is to deal with special case where we creating over the existing profile 
     (required by ATCOP */
  new_profile = true;

  if (DB_UNSPECIFIED_PROFILE_NUM == profile->GetNum())
  {
    profile->SetNumber(new_num);
    res = cacheMgr.CreateCache(profile);
  } 
  else
  {
    /* currently we overwrite the existing profile when the profile num is specified during creation */
    res = cacheMgr.UpdateOrCreateCache(profile, true, &new_profile);
  }

  if (DB_RESULT_SUCCESS != res)
  {
    goto bail;
  }

  if (!ShouldBeTreatedAsNonPersistent(profile))
  {
    file = Platform::Instance()->GetFile(profile->GetNum(),
                                         profile->GetSubcriptionId(),
                                         File::F_CREATE_OVERWRITE);
    if (!file)
    {
      res = DB_RESULT_ERR_FILE_ACCESS;
      goto bail;
    }
    if (DB_RESULT_SUCCESS != (res = file->Write(profile, false, false, true)))
    {
      goto bail;
    }
  }

  if (DB_RESULT_SUCCESS != (res = NotifyClients(profile, DB_CREATE_PROFILE_EVENT, profile->GetSubcriptionId())))
  {
    goto bail;
  }

  if (file)
  {
    delete file;
  }

  if (DB_RESULT_SUCCESS != SetCacheAttachFlag(profile->GetNum(),
                                              profile->GetSubcriptionId(),
                                              profile->GetSupportedTechMask(),
                                              params->is_attach))
  {
    PROFILE_DB_LOG_ERROR("Could not set cache attach flag ");
  }

  PROFILE_DB_LOG_MED_1("CreateProfile(): success, profile num %d",
                       params->profile_num);

  return DB_RESULT_SUCCESS;

bail:

  profile->SetNumber(DB_UNSPECIFIED_PROFILE_NUM);

  if (res != DB_RESULT_ERR_INVAL_PROFILE_NUM && num_specified != true)
  {
    if (DB_UNSPECIFIED_PROFILE_NUM != new_num)
    {
      if (file)
      {
        file->Delete(new_num, params->subscription_id);
        delete file;
      }
    }

    cacheMgr.RemoveCache(new_num, profile->GetSupportedTechMask(), profile->GetSubcriptionId());
    if (res != DB_RESULT_ERR_OUT_OF_PROFILES)
    {
      ReleaseNumForProfile(!ShouldBeTreatedAsNonPersistent(profile),
                           new_num,
                           profile->GetSupportedTechMask(),
                           profile->GetSubcriptionId(),
                           enable_deleting_default_profile); /* 0 - means we don't check permissions for deletion */
    }
  }


  PROFILE_DB_LOG_MED_4("CreateProfile(): profile num %d, persistency set to %d, "
                       "subs id %d, res %d",
                       params->profile_num,
                       params->persistent,
                       params->subscription_id,
                       res);

  return res;
}

ds_profile_db_result_type DBMgr::DeleteProfile(
  ds_profile_db_profile_num_type  profile_num,
  ds_profile_db_supported_tech_mask tech_mask,
  ds_profile_db_subs_type subs,
  boolean enable_deleting_default_profile /* = false */)
{
  boolean is_attach = false;
  
  if (callbackMgr->WithinCallback())
  {
    return DB_RESULT_ERR_INVAL_OP;
  }

  if (0 != mutex->Lock())
  {
    return DB_RESULT_FAIL;
  }
  AutoReleseMutex a_m(mutex);

  cacheMgr.GetAttachFlag(profile_num,subs,tech_mask,&is_attach);
  if(is_attach)
  {
    PROFILE_DB_LOG_ERROR_2("DeleteProfile(): deleting of profile %d with attach flag %d is not allowed",
		                    profile_num, is_attach);
    return DB_RESULT_ERR_INVAL_PROFILE_NUM;
  }

  return DeleteProfileInternal(profile_num,
                               tech_mask, 
                               subs, 
                               enable_deleting_default_profile);

}

ds_profile_db_result_type DBMgr::DeleteProfileAttach(
  ds_profile_db_profile_num_type  profile_num,
  ds_profile_db_supported_tech_mask tech_mask,
  ds_profile_db_subs_type subs)
{
  if (callbackMgr->WithinCallback())
  {
    return DB_RESULT_ERR_INVAL_OP;
  }

  if (0 != mutex->Lock())
  {
    return DB_RESULT_FAIL;
  }
  AutoReleseMutex a_m(mutex);

  return DeleteProfileInternal(profile_num,
                               tech_mask,
                               subs,
                               FALSE);
}
ds_profile_db_result_type DBMgr::SetProfile(
  Profile * profile,
  bool delete_on_file /*= false*/)
{
  ds_profile_db_result_type res = DB_RESULT_FAIL;

  if (!profile)
  {
    return DB_RESULT_ERR_INVAL_OP;
  }

  if (DB_RESULT_SUCCESS != 
      (res = Profile::ValidateSubscriptionId(profile->GetSubcriptionId())))
  {
    return res;
  }

  if (callbackMgr->WithinCallback())
  {
    return DB_RESULT_ERR_INVAL_OP;
  }

  if (0 != mutex->Lock())
  {
    return DB_RESULT_FAIL;
  }

  AutoReleseMutex a_m(mutex);

  /* profile should not have an unspecified number*/
  if (DB_UNSPECIFIED_PROFILE_NUM == profile->GetNum())
  {
    res = DB_RESULT_ERR_INVAL_PROFILE_NUM;
    goto bail;
  }

  if (!CanWriteProfile(profile->GetNum(), profile->GetSupportedTechMask()))
  {
    res = DB_RESULT_ERR_INVAL_OP;
    goto bail;
  }

  res = SetProfileInternal(profile, false, delete_on_file);

bail:
  PROFILE_DB_LOG_MED_4("SetProfile(): profile number %d, tech_mask %d, subs %d, res %d",
                       profile->GetNum(), 
                       profile->GetSupportedTechMask(),
                       profile->GetSubcriptionId(),
                       res);

  return res;
}

ds_profile_db_result_type DBMgr::SetProfileInternal(
  Profile * profile,
  bool send_reset_event, /* = false */
  bool delete_from_file  /*= false*/)
{
  File                      *file = NULL;
  File                      *file_3gpp2 = NULL;
  ds_profile_db_result_type res;
  bool                      create_3gpp2 = false;
  File::FlagType            flag = File::F_READ;
  boolean is_attach = FALSE;
  typedef enum 
  {
    DS_PROFILE_DB_ERR_NONE          = 0,
    DS_PROFILE_DB_ERR_NOTIFY_CLIENT = 1

  } ds_profile_db_local_err_type;

  ds_profile_db_local_err_type local_err = 
    DS_PROFILE_DB_ERR_NONE;

  if (!cacheMgr.GetCache(profile->GetNum(), 
                         profile->GetSupportedTechMask(), 
                         profile->GetSubcriptionId()))
  {
    PROFILE_DB_LOG_ERROR("SetProfile(): trying to modify"
                         " a non-existing profile");
    return DB_RESULT_ERR_INVAL_PROFILE_NUM;
  }


 if (DB_RESULT_SUCCESS != cacheMgr.GetAttachFlag(profile->GetNum(),
                                                  profile->GetSubcriptionId(),
                                                  profile->GetSupportedTechMask(),
                                                  &is_attach))
  {
    PROFILE_DB_LOG_ERROR("SetProfile(): Error Geting Attach");
  }

  if (is_attach)
  {
    profile->SetAttachProfileFlag(TRUE, TRUE);
    PROFILE_DB_LOG_ERROR_1("Setting Attach Profile flag %d", is_attach);
  }

  if (DB_RESULT_SUCCESS != (res = cacheMgr.UpdateCache(profile)))
  {
    goto bail;
  }

  if (DB_RESULT_SUCCESS != (res = NotifyClients(profile,
                                                send_reset_event ?
                                                DB_RESET_PROFILE_EVENT :
                                                DB_MODIFY_PROFILE_EVENT,
                                                profile->GetSubcriptionId())))
  {
    local_err = DS_PROFILE_DB_ERR_NOTIFY_CLIENT;
  }

  if (!ShouldBeTreatedAsNonPersistent(profile))
  {
    if (AllowToCreateFileOnFirstModify(profile))
    {
      file_3gpp2 = Platform::Instance()->GetFile(profile->GetNum(), 
                                                 profile->GetSubcriptionId(), 
                                                 flag);
      if (!file_3gpp2)
      {
        flag = File::F_WRITE_CREATE;
	      create_3gpp2 = true;
      }
      /*local variable file2 to check if 3GPP2 default profile exists already or not*/
      else
      {
        file_3gpp2->Close();
        delete file_3gpp2;
      }
    }
    file = Platform::Instance()->GetFile(profile->GetNum(), 
                                         profile->GetSubcriptionId(), 
                                         flag);
    if (!file)
    {
      res = DB_RESULT_ERR_FILE_ACCESS;
      goto bail;
    }
    if (flag == File::F_READ)
    {
      if (DB_RESULT_SUCCESS != (res = file->ReadTLV()))
      {
        goto bail;
      }
      profile->SetReadTime(time_get_secs());
    } 

    if (DB_RESULT_SUCCESS != (res = 
                              file->Open(profile->GetNum(),
                                         File::F_RW, 
                                         profile->GetSubcriptionId())))
    {
      goto bail;
    }

    if (DB_RESULT_SUCCESS != (res = file->Write(profile, 
                                                delete_from_file, 
                                                send_reset_event, 
                                                false, 
                                                create_3gpp2)))
    {
      goto bail;
    }
  }

  res = DB_RESULT_SUCCESS;
  /* fall through */

bail:
  if (file)
  {
    delete file;
  }
  profile->ResetParamTracking();
  PROFILE_DB_LOG_HIGH_2("SetProfile(): Exiting with local_res %d ,res %d",
                        local_err, res);
  return res;
}


ds_profile_db_result_type DBMgr::GetProfile(
  ds_profile_db_profile_num_type profile_num,
  ds_profile_db_supported_tech_mask tech_mask,
  ds_profile_db_subs_type subs,
  Profile **profile)
{  
  ds_profile_db_result_type res;
  /* TODO, in phase 2, when the whole Profile is fetched from cache, tracking list
     might be cleaned */
  if (callbackMgr->WithinCallback())
  {
    return DB_RESULT_ERR_INVAL_OP;
  }

  if (0 != mutex->Lock())
  {
    return DB_RESULT_FAIL;
  }
  AutoReleseMutex a_m(mutex);

  if (!profile)
  {
    res = DB_RESULT_ERR_INVAL_OP;
    goto bail;
  }

  //Making change for CR631257 where get param based on EPC
  if (DB_RESULT_SUCCESS != 
      (res = cacheMgr.GetCache(profile_num, tech_mask, 
                               subs, profile, false)))
  {
    goto bail;
  }

  if (profile)
  {
    if (*profile)
    {
      (*profile)->SetSubscriptionId(subs, false);
    }
  }

  res = DB_RESULT_SUCCESS;

bail:

  LOG_MSG_QTRACE_HFT_MED(PS_TAG_DSPROFILEDB, (ps_sys_subscription_enum_type)ds_profile_db_convert_dbsubs_to_dssys_subs(subs),
                         "GetProfile(): profile number %d, "
                         "tech_mask %d, subs %d, err %d", profile_num, 
                         tech_mask, subs, res);
  return res;
}

ds_profile_db_result_type DBMgr::GetProfiles(
  ds_profile_db_supported_tech_mask tech_mask,
  ds_profile_db_subs_type subs,
  Profile ***profiles,
  uint16 *profiles_count)
{
  ds_profile_db_info_type param_info = { 0 };
  return GetProfilesInternal(tech_mask,
                             INVALID_PROFILE_PARAM,
                             &param_info, subs, profiles, profiles_count);
}

ds_profile_db_result_type DBMgr::GetProfiles(
  ds_profile_db_supported_tech_mask tech_mask,
  ds_profile_db_ident_type ident,
  const ds_profile_db_info_type *param_info,
  ds_profile_db_subs_type subs,
  Profile ***profiles,
  uint16 *profiles_count)
{
  return GetProfilesInternal(tech_mask, ident, param_info,
                             subs, profiles, profiles_count);
}


ds_profile_db_result_type DBMgr::GetProfilesInternal(
  ds_profile_db_supported_tech_mask tech_mask,
  ds_profile_db_ident_type ident,
  const ds_profile_db_info_type *param_info,
  ds_profile_db_subs_type subs,
  Profile ***profiles,
  uint16 *profiles_count)
{
  ds_profile_db_result_type res;
  uint16 count = 0;

  if (callbackMgr->WithinCallback())
  {
    return DB_RESULT_ERR_INVAL_OP;
  }

  if (0 != mutex->Lock())
  {
    return DB_RESULT_FAIL;
  }
  AutoReleseMutex a_m(mutex);

  if (!profiles || !profiles_count || !param_info)
  {
    res = DB_RESULT_ERR_INVAL_OP;
    goto bail;
  }

  /* make sure the param_info is valid */
  if (INVALID_PROFILE_PARAM != ident)
  {
    if (!param_info->len || !param_info->buf)
    {
      if (!((ident == 
             GENERAL_PROFILE_PARAM_APN_NAME) 
            && (param_info->len == 0)))
      {
        res = DB_RESULT_ERR_INVAL_OP;
        goto bail;
      }
    }
  }

  if (DB_RESULT_SUCCESS != (res = 
                            Profile::ValidateTechMask(tech_mask)))
  {
    return res;
  }

  count = cacheMgr.GetNumOfCacheEntry(tech_mask, subs);
  if (!count)
  {
    *profiles_count = count;
    *profiles = NULL;
    res = DB_RESULT_SUCCESS;
    goto bail;
  }

  PS_SYSTEM_HEAP_MEM_ALLOC(*profiles, (count) * sizeof(Profile*), 
                           Profile**);
  if (NULL == *profiles) {
    return DB_RESULT_ERR_OUT_OF_MEMORY;
  }
  memset(*profiles, 0, count * sizeof(Profile *));

  /* iterate over cache table and get all the  profiles */
  count = 0;
  res = cacheMgr.GetProfilesFromInternalCache(profiles, &count, 
                                              tech_mask, ident, 
                                              param_info, subs);
  if (DB_RESULT_SUCCESS != res)
  {
    goto bail;
  }

  *profiles_count = count;
  res = DB_RESULT_SUCCESS;

bail:
  if (DB_RESULT_SUCCESS != res && profiles != NULL ) 
  {
    CleanupProfiles(count, *profiles);
  }

  PROFILE_DB_LOG_ERROR_5("GetProfilesInternal(): Profiles 0x%p, profiles_count"
                         "0x%p or param_info 0x%p pointers, ident %d, res %d",
                         profiles, profiles_count, param_info, ident, res);

  return res;
}

ds_profile_db_result_type DBMgr::ResetProfile(
  ds_profile_db_profile_num_type num,
  ds_profile_db_supported_tech_mask tech_mask,
  ds_profile_db_subs_type subs)
{
  if (callbackMgr->WithinCallback())
  {
    return DB_RESULT_ERR_INVAL_OP;
  }

  if (0 != mutex->Lock())
  {
    return DB_RESULT_FAIL;
  }
  AutoReleseMutex a_m(mutex);
  ds_profile_db_result_type res = DB_RESULT_SUCCESS;

  /* do a quick before you go to heavy operations of constructing a full profile from cache */
  CacheMgr::cache_entry *c_e = cacheMgr.GetCache(num, tech_mask, subs);
  do 
  {
    if (!c_e)
    {
      res = DB_RESULT_ERR_INVAL_PROFILE_NUM;
      break;
    }

    if (!CanWriteProfile(num, c_e->tech_mask))
    {
      res = DB_RESULT_ERR_INVAL_PROFILE_NUM;
      break;
    }

    /* get an existing profile and check its tech mask */
    Profile *profile = NULL;
    bool persistency = false;

    if (DB_RESULT_SUCCESS != (res = GetProfile(num, tech_mask, subs, &profile)))
    {
      break;
    }

    /* we need to keep those intact */
    if (profile)
    {
      persistency = !ShouldBeTreatedAsNonPersistent(profile);
      subs = profile->GetSubcriptionId();
      delete profile;
    } 

    /* now create a new profile (a default one) with the same tech mask */
    profile = Profile::CreateInstance(tech_mask);
    if (!profile)
    {
      res = DB_RESULT_ERR_OUT_OF_PROFILES;
      break;
    }

    profile->SetNumber(num);
    profile->SetPersistency(persistency);
    profile->SetSubscriptionId(subs, false);

    res = SetProfileInternal(profile, true);
    delete profile;

  } while (0);

  PROFILE_DB_LOG_MED_3("ResetProfile(): profile num %d, "
                       "tech_mask %d, res %d", num, 
                       tech_mask, res);
  return res;
}

ds_profile_db_result_type DBMgr::ResetProfileParam(
  ds_profile_db_profile_num_type num,
  ds_profile_db_supported_tech_mask tech_mask,
  ds_profile_db_ident_type       ident,
  ds_profile_db_subs_type        subs,
  boolean mark_invalid)
{
  /* TODO revisit the implementation in phase 2 */
  if (callbackMgr->WithinCallback())
  {
    return DB_RESULT_ERR_INVAL_OP;
  }

  if (0 != mutex->Lock())
  {
    return DB_RESULT_FAIL;
  }
  AutoReleseMutex a_m(mutex);
  ds_profile_db_result_type res = 
    DB_RESULT_SUCCESS;
   /* get an existing profile */
    Profile *profile = NULL;

  do 
  {
    /* do a quick before you go to heavy operations of constructing a full profile from cache */
    CacheMgr::cache_entry *c_e = cacheMgr.GetCache(num, tech_mask, subs);
    if (!c_e)
    {
      res = DB_RESULT_ERR_INVAL_PROFILE_NUM;
      break;
    }

    if (!CanWriteProfile(num, c_e->tech_mask))
    {
      res = DB_RESULT_ERR_INVAL_OP;
      break;
    }

    if (DB_RESULT_SUCCESS != 
        (res = GetProfile(num, tech_mask, subs, &profile)))
    {
      break;
    }

    /* reset the required param inside that profile */
    if (profile)
    {
      if (DB_RESULT_SUCCESS != 
          (res = profile->ResetParam(ident, mark_invalid)))
      {
        break;
      }

      res = SetProfile(profile, true);
    }
    /* fall through */
  } while (0);

  if (profile)
  {
    delete profile;
  }
 
  PROFILE_DB_LOG_MED_4("ResetProfileParam(): profile num %d, "
                       "tech_mask %d, ident %d, err %d", num, 
                       tech_mask, ident, res);
  return res;
}

ds_profile_db_result_type DBMgr::GetDefaultProfileNum(
  ds_profile_db_profile_num_type *num,
  ds_profile_db_tech_type tech,
  ds_profile_db_family_type family)
{
  if (callbackMgr->WithinCallback())
  {
    return DB_RESULT_ERR_INVAL_OP;
  }

  if (0 != mutex->Lock())
  {
    return DB_RESULT_FAIL;
  }

  AutoReleseMutex a_m(mutex);
  ds_profile_db_result_type res;
  TechMgr *techMgr = NULL;

  if (!num)
  {
    res = DB_RESULT_ERR_INVAL_OP;
    goto bail;
  }
  
  if (DB_RESULT_SUCCESS != 
      (res = Profile::ValidateTech(tech)))
  {
    goto bail;
  }

  techMgr = GetTechManager(tech);
  if (!techMgr)
  {
    res =  DB_RESULT_ERR_INVAL_PROFILE_TYPE;
    goto bail;
  }

  res = techMgr->GetDefaultProfileNum(num, family);

bail:
  PROFILE_DB_LOG_MED_3("GetDefaultProfileNum(): "
                       "tech %d, family %d, err %d", 
                       tech, family, res);
  return res;
}

ds_profile_db_result_type DBMgr::GetDefaultProfileNum(
  ds_profile_db_profile_num_type *num,
  ds_profile_db_tech_type tech,
  ds_profile_db_family_type family,
  ds_profile_db_subs_type subs)
{
  if (callbackMgr->WithinCallback())
  {
    return DB_RESULT_ERR_INVAL_OP;
  }

  if (0 != mutex->Lock())
  {
    return DB_RESULT_FAIL;
  }

  AutoReleseMutex a_m(mutex);
  ds_profile_db_result_type res;

  do 
  {
    if (!num)
    {
      res = DB_RESULT_ERR_INVAL_OP;
      break;
    }


    if (DB_RESULT_SUCCESS != (res = Profile::ValidateTech(tech)))
    {
      break;
    }

    if (DB_RESULT_SUCCESS != (res = Profile::ValidateSubscriptionId(subs)))
    {
      break;
    }

    TechMgr *techMgr = GetTechManager(tech);
    if (!techMgr)
    {
      res = DB_RESULT_ERR_INVAL_PROFILE_TYPE;
      break;
    }

    res = techMgr->GetDefaultProfileNum(num, family, subs);
  } while (0);

  PROFILE_DB_LOG_MED_4("GetDefaultProfileNum(): tech %d, "
                       "family %d, subs %d, err %d", 
                       tech, family, subs, res);
  return res;
}

ds_profile_db_result_type DBMgr::SetDefaultProfileNum(
  ds_profile_db_profile_num_type num,
  ds_profile_db_tech_type tech,
  ds_profile_db_family_type family)
{
  if (callbackMgr->WithinCallback())
  {
    return DB_RESULT_ERR_INVAL_OP;
  }

  if (0 != mutex->Lock())
  {
    return DB_RESULT_FAIL;
  }

  AutoReleseMutex a_m(mutex);
  ds_profile_db_result_type res;
  do 
  {
    if (DB_RESULT_SUCCESS != (res = Profile::ValidateTech(tech)))
    {
      break;
    }

    TechMgr *techMgr = GetTechManager(tech);
    if (!techMgr)
    {
      PROFILE_DB_LOG_ERROR_1("SetDefaultProfileNum(): unsupported technology %d", tech);
      res = DB_RESULT_ERR_INVAL_PROFILE_TYPE;
      break;
    }

    res = techMgr->SetDefaultProfileNum(num, family);

  } while (0);

  PROFILE_DB_LOG_MED_4("SetDefaultProfileNum(): num %d tech %d, "
                       "family %d, err %d",num, tech, family, res);
  return res;
}

ds_profile_db_result_type DBMgr::SetDefaultProfileNum(
  ds_profile_db_profile_num_type num,
  ds_profile_db_tech_type tech,
  ds_profile_db_family_type family,
  ds_profile_db_subs_type subs)
{
  if (callbackMgr->WithinCallback())
  {
    return DB_RESULT_ERR_INVAL_OP;
  }

  if (0 != mutex->Lock())
  {
    return DB_RESULT_FAIL;
  }
  AutoReleseMutex a_m(mutex);
  ds_profile_db_result_type res;

  do 
  {
    if (DB_RESULT_SUCCESS != (res = Profile::ValidateTech(tech)))
    {
      break;
    }

    if (DB_RESULT_SUCCESS != (res = Profile::ValidateSubscriptionId(subs)))
    {
      break;
    }

    /* if profile num exists, make sure its subs id is equal to the one we are
       trying to set */
    Profile *profile = NULL;
    if (DB_RESULT_SUCCESS == (res = cacheMgr.GetCache(num, DB_TECH_TO_TECHMASK(tech), subs, &profile, false)))
    {
      if ((profile) && (profile->GetSubcriptionId() != subs))
      {
        if(profile)
        {
          delete profile;
        }
        res = DB_RESULT_ERR_INVAL_OP;
        break;
      }
    }

    TechMgr *techMgr = GetTechManager(tech);
    if (!techMgr)
    {
      res = DB_RESULT_ERR_INVAL_PROFILE_TYPE;
    }
    else
    {
    res = 
      techMgr->SetDefaultProfileNum(num, family, subs);
    }
    if(profile)
    {
      delete profile;
    }

  } while (0);

  PROFILE_DB_LOG_MED_5("SetDefaultProfileNum(): num %d tech %d, "
                       "family %d subs %d, err %d",
                       num, tech, family, subs, res);
  return res;

}

ds_profile_db_result_type DBMgr::GetProfilesRange(
  ds_profile_db_supported_tech_mask tech_mask,
  ds_profile_db_profile_num_type *min_num,
  ds_profile_db_profile_num_type *max_num)
{
  if (callbackMgr->WithinCallback())
  {
    return DB_RESULT_ERR_INVAL_OP;
  }

  if (0 != mutex->Lock())
  {
    return DB_RESULT_FAIL;
  }
  AutoReleseMutex a_m(mutex);

  if (!min_num || !max_num)
  {
    return DB_RESULT_ERR_INVAL_OP;
  }

  ds_profile_db_result_type res;
  if (DB_RESULT_SUCCESS != (res = Profile::ValidateTechMask(tech_mask)))
  {
    return res;
  }

  TechMgr *techMgr = NULL;
  *min_num = 0;
  *max_num = DB_MAXIMUM_PROFILE_NUM;
  ds_profile_db_profile_num_type tmp_min_num = 0;
  ds_profile_db_profile_num_type tmp_max_num = 0;

  /* find intersection of all ranges for technologies in mask */
  DB_FOR_EVERY_TECH_IN_MASK_DO_START(tech_mask, tech)
  if ((techMgr = GetTechManager(tech)))
  {
    techMgr->GetProfilesRange(&tmp_min_num, &tmp_max_num);
  }
  if (tmp_min_num > *min_num)
  {
    *min_num = tmp_min_num;
  }
  if (tmp_max_num < *max_num)
  {
    *max_num = tmp_max_num;
  }
  if (*min_num > *max_num)
  {
    return DB_RESULT_ERR_INVAL_PROFILE_TYPE;
  }
  DB_FOR_EVERY_TECH_IN_MASK_DO_END

  return DB_RESULT_SUCCESS;
}

ds_profile_db_callback_id_type DBMgr::RegisterForNotifications(
  ds_profile_db_profile_num_type num,
  ds_profile_db_supported_tech_mask tech_mask,
  ds_profile_db_cb_type cback,
  ds_profile_db_subs_type subs,
  void *user_data,
  bool mode_handler /* = false */,
  bool treat_tech_mask_as_OR  /*  = false */)
{
  static int iter = 0;
  typedef enum
  {
    DS_PROFILE_DB_ERR_REG_NONE = 0,
    DS_PROFILE_DB_ERR_REG_NULL_CBACK_FUNC = 1,
    DS_PROFILE_DB_ERR_REG_NO_PROFILE = 2,
    DS_PROFILE_DB_ERR_REG_TECH_MASK_UNSUPPORTED = 3
  }ds_profile_db_register_err_type;
  
  ds_profile_db_register_err_type local_err = 
    DS_PROFILE_DB_ERR_REG_NONE;
  ds_profile_db_callback_id_type callback_id = 0;
  /* registration per profile is not allowed for non-existing ones */
  CacheMgr::cache_entry *c_e = NULL;

  if (callbackMgr->WithinCallback())
  {
    return 0;
  }

  if (0 != mutex->Lock())
  {
    return 0;
  }

  AutoReleseMutex a_m(mutex);

  if (DB_RESULT_SUCCESS != Profile::ValidateTechMask(tech_mask))
  {
    return 0;
  }

  if (DB_RESULT_SUCCESS != Profile::ValidateSubscriptionId(subs))
  {
    return 0;
  }

  if (NULL == cback)
  {
    local_err = DS_PROFILE_DB_ERR_REG_NULL_CBACK_FUNC;
    callback_id = 0;
    goto bail;
  }

  if (DB_UNSPECIFIED_PROFILE_NUM != num)
  {
    if (!(c_e = cacheMgr.GetCache(num, tech_mask, subs)))
    {
      local_err = DS_PROFILE_DB_ERR_REG_NO_PROFILE;
      callback_id = 0;
      goto bail;
    }

    if (!DB_IS_SUBSET_OF_TECHMASK(tech_mask, c_e->tech_mask))
    {
      local_err = DS_PROFILE_DB_ERR_REG_TECH_MASK_UNSUPPORTED;
      callback_id = 0;
      goto bail;
    }
  }

  callback_id = callbackMgr->Register(num,
                                      tech_mask,
                                      cback,
                                      user_data,
                                      subs,
                                      mode_handler,
                                      treat_tech_mask_as_OR);
  if (!callback_id)
  {
    return 0;
  }

  if (mode_handler)
  {
    if (DB_RESULT_SUCCESS != NotifyClientWithPowerUpEvent(num, tech_mask, 
                                                          treat_tech_mask_as_OR,
                                                          cback, callback_id, 
                                                          user_data,
                                                          subs, DB_POWER_UP_EVENT))
    {
      DeregisterForNotifications(callback_id, subs);
      return 0;
    }
    ids[iter].callback_id = callback_id;
    ids[iter].cback = cback;
    ids[iter].treat_tech_mask_as_OR = treat_tech_mask_as_OR;
    ids[iter].tech_mask = tech_mask;
    ids[iter].user_data = user_data;
    ids[iter].num = num;
    iter++;
  }
  else
  {
    /* If profiles ready on given subs, notify registered clients */
    if(theInstance->subsProfilesReady[subs] && 
     (DB_RESULT_SUCCESS != callbackMgr->Notify(cback, callback_id, DB_PROFILES_READY_ON_SUBS_EVENT, user_data, subs)))
    {
      DeregisterForNotifications(callback_id, subs);
      PROFILE_DB_LOG_ERROR("RegisterForNotifications(): Failed to Notify clients");
      return 0;
    }
  }
  
bail:
  PROFILE_DB_LOG_MED_8("RegisterForNotifications(): profile num %d, tech_mask %d "
                       "mode_handler %d treat_tech_mask_as_OR %d, callback_id %d, "
                       "subs %d ready %d local_err %d",num, tech_mask, mode_handler, 
                       treat_tech_mask_as_OR, callback_id,
                       subs, theInstance->subsProfilesReady[subs], local_err);

  return callback_id;
}

ds_profile_db_result_type DBMgr::DeregisterForNotifications(
  ds_profile_db_callback_id_type callback_id,
  ds_profile_db_subs_type subs)
{
  PROFILE_DB_LOG_MED_2("DeregisterForNotifications(): callback_id %d, subs %d", 
                       callback_id, subs);

  if (callbackMgr->WithinCallback())
  {
    return DB_RESULT_ERR_INVAL_OP;
  }

  if (0 != mutex->Lock())
  {
    return DB_RESULT_FAIL;
  }
  AutoReleseMutex a_m(mutex);

  return callbackMgr->Deregister(callback_id, subs);
}

boolean DBMgr:: ProfilesReadyOnSubs(
 ds_profile_db_subs_type subs
)
{
  return theInstance->subsProfilesReady[subs];
}


/* private functions */
TechMgr* DBMgr::GetTechManager(ds_profile_db_tech_type tech)
{
  return techMgrs[tech];
}

ds_profile_db_result_type DBMgr::InitCreationParams(ds_profile_db_creation_params_type *params)
{
  if (!params)
  {
    return DB_RESULT_ERR_INVAL;
  }

  params->profile_num     = DB_UNSPECIFIED_PROFILE_NUM;
  params->persistent      = TRUE;
  params->subscription_id = Profile::GetDefaultSubscriptionId();
  params->is_attach       = FALSE;

  return DB_RESULT_SUCCESS;
}

void DBMgr::ReleaseRangeInCache(
  ds_profile_db_profile_num_type start,
  ds_profile_db_profile_num_type end,
  ds_profile_db_supported_tech_mask tech_mask,
  bool is_persistent,
  ds_profile_db_subs_type subs)
{
  for (ds_profile_db_profile_num_type i = start; i <= end; i++)
  {
    cacheMgr.RemoveCache(i, tech_mask, subs);
    ReleaseNumForProfile(is_persistent,
                         i,
                         tech_mask,
                         subs);
  }
}





void DBMgr::CleanupProfiles(unsigned int profiles_count,
                            Profile **profiles,
                            bool   remove_from_cache, /* = false */
                            bool   dealocate_numbers /* = false */)
{
  if (!profiles)
  {
    return;
  }

  for (unsigned int i = 0; i < profiles_count; i++)
  {
    if (profiles[i])
    {
      if (remove_from_cache)
      {
        cacheMgr.RemoveCache(profiles[i]->GetNum(), profiles[i]->GetSupportedTechMask(),
                             profiles[i]->GetSubcriptionId());
      }

      if (dealocate_numbers)
      {
        ReleaseNumForProfile(!ShouldBeTreatedAsNonPersistent(profiles[i]),
                             profiles[i]->GetNum(),
                             profiles[i]->GetSupportedTechMask(),
                             profiles[i]->GetSubcriptionId());
      }

      delete profiles[i];
    }
  }

  PS_SYSTEM_HEAP_MEM_FREE(profiles);

}

ds_profile_db_result_type DBMgr::OnTheFlyToCache(Profile **profiles,
                                                 unsigned int *profiles_count,
                                                 ds_profile_db_subs_type subs)
{
#ifdef FEATURE_DATA_IS707
  
  /* create some profiles on the fly */
  /*************************************/
  /* for now it is only profile 0 for 3gpp2 */
  unsigned int create_on_the_fly_num = 1;
  ds_profile_db_result_type res = DB_RESULT_SUCCESS;
  Profile *profile = NULL;

  if (create_on_the_fly_num > *profiles_count)
  {
    return DB_RESULT_ERR_OUT_OF_PROFILES;
  }
  
  /* check that profile does not already exist in cache */
  if (cacheMgr.GetCache(0, DB_TECH_TO_TECHMASK(DB_TECH_3GPP2), subs))
  {
    *profiles_count = 0;
    res = DB_RESULT_SUCCESS;
    goto bail;
  }
  
    profile = Profile::CreateInstance(DB_TECH_3GPP2);

  if (!profile)
  {
    return DB_RESULT_ERR_OUT_OF_PROFILES;
  }

  profile->SetSubscriptionId(subs, false);
  profile->SetPersistency(false);
  profile->SetNumber(0);
  profiles[0] = profile;

  *profiles_count = create_on_the_fly_num;

  res = StoreInCacheDuringInit(profiles, *profiles_count, subs);
  if (DB_RESULT_SUCCESS)
  {
    goto bail;
  }
  res = DB_RESULT_SUCCESS;

bail:
  if (DB_RESULT_SUCCESS != res) 
  {
    if (profile)
    {
      delete profile;
    }
  }
  PROFILE_DB_LOG_HIGH_3("On the Fly to cache for subs %d, "
                        "profile_count %d, error %d", 
                        subs, *profiles_count, res);
  return res;
#else
  *profiles_count = 0;
  return DB_RESULT_SUCCESS;
#endif
}

ds_profile_db_result_type DBMgr::StoreInCacheDuringInit(Profile **profiles, unsigned int profiles_count,
                                                        ds_profile_db_subs_type subs)
{
  ds_profile_db_result_type res;

  for (unsigned int i = 0; i < profiles_count; i++) 
  {
    if (profiles[i]) 
    {
      if (DB_RESULT_SUCCESS != (res = 
                                GetNumForProfile(profiles[i], NULL, 
                                                 subs, false, true))) 
      {
        return res;
      }

      if (DB_RESULT_SUCCESS != 
          (res = cacheMgr.CreateCache(profiles[i]))) 
      {
        return res;
      }
      if (profiles[i]->GetAttachProfileFlag()) 
      {
        cacheMgr.SetAttachFlag(profiles[i]->GetNum(),
                               profiles[i]->GetSubcriptionId(),
                               profiles[i]->GetSupportedTechMask(),
                               profiles[i]->GetAttachProfileFlag());
      }
    }
  }

  return DB_RESULT_SUCCESS;
}

#ifdef FEATURE_DATA_IS707
ds_profile_db_result_type DBMgr::MarkRangeInCacheDuringInit(
  ds_profile_db_supported_tech_mask tech_mask,
  ds_profile_db_profile_num_type start_range,
  ds_profile_db_profile_num_type end_range,
  bool is_persistent,
  ds_profile_db_subs_type subs)
{
  ds_profile_db_result_type res;

  for (ds_profile_db_profile_num_type i = start_range; i < end_range; i++)
  {
    if (DB_RESULT_SUCCESS != (res = GetNumForProfile(i, tech_mask,
                                                     is_persistent, subs,
                                                     NULL, false, true)))
    {
      return res;
    }
    if (DB_RESULT_SUCCESS != 
        (res = cacheMgr.CreateCache(i, tech_mask, subs, 0, is_persistent, NULL)))
    {
      return res;
    }
  }

  return DB_RESULT_SUCCESS;
}
#endif

ds_profile_db_result_type DBMgr::FromFileToCache(Profile **profiles,
                                                 unsigned int *profiles_count,
                                                 ds_profile_db_subs_type subs)
{
  ds_profile_db_result_type res;

  /* read everything there is from EFS */
  /*************************************/
  File *file = Platform::Instance()->GetFile(subs);
  if (!file)
  {
    res = DB_RESULT_ERR_FILE_ACCESS;
    goto bail;
  }

  res = file->ReadAll(profiles, profiles_count);
  if (DB_RESULT_ERR_OUT_OF_PROFILES == res)
  {
    PROFILE_DB_LOG_HIGH_1(" More profiles read from EFS than expected . "
                          "Please delete excess files res %d", res);
  }
  else if (DB_RESULT_SUCCESS != res)
  {
    goto bail;
  }

  res = StoreInCacheDuringInit(profiles, *profiles_count, subs);
  if (DB_RESULT_SUCCESS != res)
  {
    PROFILE_DB_LOG_HIGH_1("FromFileToCache(): could not store profiles "
                          "fetched from EFS to cache,"
                          "error %d, still proceeding", res);
    res = DB_RESULT_SUCCESS;
  }

  res = DB_RESULT_SUCCESS;
  /* fall through */

bail:

  if (file)
  {
    delete file;
  }

  PROFILE_DB_LOG_ERROR_ON_EXIT(res);

  return res;
}

ds_profile_db_result_type DBMgr::GetNumForProfile(ds_profile_db_profile_num_type num,
                                                  ds_profile_db_supported_tech_mask tech_mask,
                                                  bool is_persistent,
                                                  ds_profile_db_subs_type subs,
                                                  ds_profile_db_profile_num_type *new_num /* = NULL */,
                                                  bool overwrite, /* = false */
                                                  bool during_init /* = false */)
{
  ds_profile_db_result_type res;
  ds_profile_db_profile_num_type new_num_internal;

  if (DB_UNSPECIFIED_PROFILE_NUM == num)
  {
    res = AllocateNum(&new_num_internal, subs, tech_mask);
  } else
  {
    res = CheckAndReserveNum(num, tech_mask, subs, during_init, overwrite);
    new_num_internal = num;
  }

  if (DB_RESULT_SUCCESS != res)
  {
    return res;
  }

  /* only decrease the count if the profile is actually a new one */
  if (0 == cacheMgr.GetCache(num, tech_mask, subs))
  {
    if (DB_RESULT_SUCCESS != (res = DecProfileCount(is_persistent, tech_mask, subs)))
    {
      goto bail;
    }
  }

  if (new_num)
  {
    *new_num = new_num_internal;
  }

  return DB_RESULT_SUCCESS;

bail:

  FreeNum(new_num_internal, tech_mask, FALSE, subs);
  return res;
}


ds_profile_db_result_type DBMgr::GetNumForProfile(const Profile *profile,
                                                  ds_profile_db_profile_num_type *new_num /* = NULL */,
                                                  ds_profile_db_subs_type subs,  /*= DB_PROFILE_ACTIVE_SUBSCRIPTION_1 */
                                                  bool overwrite, /* = false */
                                                  bool during_init /* = false */)
{
  return GetNumForProfile(profile->GetNum(),
                          profile->GetSupportedTechMask(),
                          !ShouldBeTreatedAsNonPersistent(profile),
                          profile->GetSubcriptionId(),
                          new_num,
                          overwrite,
                          during_init);
}

ds_profile_db_result_type DBMgr::ReleaseNumForProfile(
  bool is_persistent,
  ds_profile_db_profile_num_type num,
  ds_profile_db_supported_tech_mask tech_mask,
  ds_profile_db_subs_type subs,
  boolean enable_deleting_default_profile
)
{
  ds_profile_db_result_type res = FreeNum(num, tech_mask, enable_deleting_default_profile, subs);
  if (DB_RESULT_SUCCESS != res)
  {
    return res;
  }

  IncProfileCount(is_persistent, tech_mask, subs);

  return DB_RESULT_SUCCESS;
}

/* 
  Does a couple of checks whether the profile number 
  can be reserved in the system.
  If the number is within the range that DB allocates numbers from
  this number will be reserved in it, so that we will not give it away
  for new profiles that will be created afterwards
*/
ds_profile_db_result_type DBMgr::CheckAndReserveNum(
  ds_profile_db_profile_num_type num,
  ds_profile_db_supported_tech_mask tech_mask,
  ds_profile_db_subs_type subs,
  bool during_init,
  bool overwrite)
{
  TechMgr *techMgr = NULL;
  ds_profile_db_result_type res = DB_RESULT_SUCCESS;
  bool is_epc = (tech_mask == (DB_TECH_TO_TECHMASK(DB_TECH_COMMON) |
                                 DB_TECH_TO_TECHMASK(DB_TECH_3GPP) |
                                 DB_TECH_TO_TECHMASK(DB_TECH_3GPP2))) ? true : false;


  /* two checks need to be done before the profile can be reserved 
     1. Either profile is in valid reservation range for every involved tech 
     2. Or profile is being set during init and it is within valid creation range 
   */
  DB_FOR_EVERY_TECH_IN_MASK_DO_START(tech_mask, tech)
  if ((techMgr = GetTechManager(tech)))
  {
    if (!techMgr->CanReserveProfileOutsideOfAllocationRange(num, during_init, &overwrite, is_epc))
    {
      res = DB_RESULT_ERR_INVAL_PROFILE_NUM;
      if (!during_init)
      {
        return res;
      } 
      else
      {
        break;
      }
    }
  }
  DB_FOR_EVERY_TECH_IN_MASK_DO_END

  //Handle 3GPP special case here.
  if ( DB_TECH_TO_TECHMASK(DB_TECH_3GPP) == tech_mask)
  {
    if ((techMgr = GetTechManager(DB_TECH_3GPP)))
    {
      res = techMgr->CheckAndReserveNum(num, subs, overwrite);
    }
  }
  else
  {
    /* at this point, either we passed the check 1) or we are during init */
    /* update numbers pool if the number is managed by it and the place is not taken already */
    if (WithinNumberPoolRange(num))
    {
      if (overwrite)
      {
        numbersPool[subs][num - TechMgr::ALLOC_RANGE_MIN] = true;
        return DB_RESULT_SUCCESS;
      }

      if (!numbersPool[subs][num - TechMgr::ALLOC_RANGE_MIN])
      {
        numbersPool[subs][num - TechMgr::ALLOC_RANGE_MIN] = true;
        return DB_RESULT_SUCCESS;
      } 
      else
      {
        return DB_RESULT_ERR_INVAL_PROFILE_NUM;
      }
    }
  }
  return res;
}

ds_profile_db_result_type DBMgr::AllocateNum(ds_profile_db_profile_num_type *num,
                                             ds_profile_db_subs_type subs,
                                             ds_profile_db_supported_tech_mask tech_mask)
{
  static int lastUnusedIndex[DB_PROFILE_ACTIVE_SUBSCRIPTION_MAX] = { 0 };
  TechMgr *techMgr = NULL;

  //Profile numbering of 3gpp profile should start from 1 -24
  //for backward compatibility

  if (DB_TECH_TO_TECHMASK(DB_TECH_3GPP) == tech_mask)
  {
    if (NULL != (techMgr = GetTechManager(DB_TECH_3GPP)))
    {
      return (techMgr->AllocateNum(num,subs,tech_mask));
    }
    else
    {
      return DB_RESULT_ERR_INVAL_PROFILE_TYPE;
    }
  }
  else
  {
    /* i counts the iteration, the actual index to use is lastUnusedIndex */
    for (int i = 0; i < DS_PROFILE_DB_MAX_PROFILE_COUNT;
         i++,
         lastUnusedIndex[subs] = (lastUnusedIndex[subs]+1) % DS_PROFILE_DB_MAX_PROFILE_COUNT)
    {
      if (false == numbersPool[subs][lastUnusedIndex[subs]])
      {
        numbersPool[subs][lastUnusedIndex[subs]] = true;
        *num = lastUnusedIndex[subs] + TechMgr::ALLOC_RANGE_MIN;
        lastUnusedIndex[subs] = (lastUnusedIndex[subs]+1) % DS_PROFILE_DB_MAX_PROFILE_COUNT;
        return DB_RESULT_SUCCESS;
      }
    }
  }
  return DB_RESULT_ERR_OUT_OF_PROFILES;
}

ds_profile_db_result_type DBMgr::FreeNum(
  ds_profile_db_profile_num_type num,
  ds_profile_db_supported_tech_mask mask,
  boolean enable_deleting_default_profile,
  ds_profile_db_subs_type subs)
{
  TechMgr* techMgr = NULL;

  if (!CanDeleteProfile(num, mask, subs, enable_deleting_default_profile))
  {
    return DB_RESULT_ERR_INVAL_PROFILE_NUM;
  }

  //Profile numbering of 3gpp profile should start from 1 -24
  //for backward compatibility

  if (DB_TECH_TO_TECHMASK(DB_TECH_3GPP) == mask)
  {
    if (NULL != (techMgr = GetTechManager(DB_TECH_3GPP)))
    {
      return (techMgr->FreeNum(num, mask, subs));
    }
    else
    {
      return DB_RESULT_ERR_INVAL_PROFILE_TYPE;
    }
  }

  /* if the number was never taken, return an error */
  if (WithinNumberPoolRange(num))
  {
    if (!numbersPool[subs][num - TechMgr::ALLOC_RANGE_MIN])
    {
      return DB_RESULT_ERR_INVAL_PROFILE_NUM;
    }

    numbersPool[subs][num - TechMgr::ALLOC_RANGE_MIN] = false;
  }

  return DB_RESULT_SUCCESS;
}

ds_profile_db_result_type DBMgr::DecProfileCount(
  bool is_persistent,
  ds_profile_db_supported_tech_mask mask,
  ds_profile_db_subs_type subs)
{
  TechMgr *techMgr = NULL;
  bool     firstTech = false;

  DB_FOR_EVERY_TECH_IN_MASK_DO_START(mask, tech)
  if ((techMgr = GetTechManager(tech)))
  {
    if (techMgr->GetCount(is_persistent, subs) <= 0)
    {
      return DB_RESULT_ERR_OUT_OF_PROFILES;
    }
  }
  DB_FOR_EVERY_TECH_IN_MASK_DO_END

    DB_FOR_EVERY_TECH_IN_MASK_DO_START(mask, tech)
  if ((techMgr = GetTechManager(tech)))
  {
    if (!firstTech)
    {
      techMgr->DecCount(is_persistent, subs);
      if (is_persistent)
      {
        firstTech = true;
      }
    }
  }
  DB_FOR_EVERY_TECH_IN_MASK_DO_END

  return DB_RESULT_SUCCESS;
}

ds_profile_db_result_type DBMgr::IncProfileCount(
  bool is_persistent,
  ds_profile_db_supported_tech_mask mask,
  ds_profile_db_subs_type subs)
{
  TechMgr *techMgr = NULL;
  bool firstTech   = false;

  DB_FOR_EVERY_TECH_IN_MASK_DO_START(mask, tech)
  if ((techMgr = GetTechManager(tech)))
  {
    if (!firstTech)
    {
      if (techMgr->IncCount(is_persistent, subs) <= 0)
      {
        return DB_RESULT_FAIL;
      }
       //If it is persistent consider EPC as single entity
       //CR772637
      if (is_persistent)
      {
        firstTech = true;
      }
    }
  }
  DB_FOR_EVERY_TECH_IN_MASK_DO_END

  return DB_RESULT_SUCCESS;
}


ds_profile_db_result_type DBMgr::NotifyClients(
  Profile *profile,
  ds_profile_db_event_type event,
  ds_profile_db_subs_type subs)
{
  ds_profile_db_result_type res;

  if (DB_RESULT_SUCCESS != (res = callbackMgr->Notify(profile, 
                                                      event, 
                                                      subs)))
  {
    goto bail;
  }

  res = DB_RESULT_SUCCESS;

bail:

  PROFILE_DB_LOG_MED_3("NotifyClients(): event %d, subs %d, error %d", 
                       event, subs, res);

  return res;
}

ds_profile_db_result_type DBMgr::NotifyClientWithPowerUpEvent(
  ds_profile_db_profile_num_type num,
  ds_profile_db_supported_tech_mask tech_mask,
  bool treat_tech_mask_as_OR,
  ds_profile_db_cb_type cback,
  ds_profile_db_callback_id_type callback_id,
  void *user_data,
  ds_profile_db_subs_type subs,
  ds_profile_db_event_type event,
  Profile **profiles,
  uint16 profile_count)
{
  PROFILE_DB_LOG_MED_3("NotifyClientWithPowerUpEvent(): profile num %d, "
                       "tech_mask %d, callback_id %d", num, tech_mask, 
                       callback_id);

  if (!profiles && profile_count == 0)
  {
    return callbackMgr->NotifyOnPowerUp(profiles_for_power_up,
                                        profiles_count_for_power_up,
                                        num,
                                        tech_mask,
                                        cback,
                                        callback_id,
                                        event,
                                        user_data,
                                        subs,
                                        treat_tech_mask_as_OR);
  } else
  {
    return callbackMgr->NotifyOnPowerUp(profiles,
                                        profile_count,
                                        num,
                                        tech_mask,
                                        cback,
                                        callback_id,
                                        event,
                                        user_data,
                                        subs,
                                        treat_tech_mask_as_OR);
  }
}

bool DBMgr::CanDeleteProfile(
  ds_profile_db_profile_num_type num,
  ds_profile_db_supported_tech_mask tech_mask,
  ds_profile_db_subs_type subs,
  boolean enable_deleting_default_profile)
{
  TechMgr *techMgr = NULL;

  DB_FOR_EVERY_TECH_IN_MASK_DO_START(tech_mask, tech)
  if ((techMgr = GetTechManager(tech)))
  {
    if (!techMgr->CanDeleteProfile(num, subs, enable_deleting_default_profile))
    {
      return false;
    }
  }
  DB_FOR_EVERY_TECH_IN_MASK_DO_END

  return true;
}

bool DBMgr::CanWriteProfile(
  ds_profile_db_profile_num_type num,
  ds_profile_db_supported_tech_mask tech_mask)
{
  TechMgr *techMgr = NULL;

  DB_FOR_EVERY_TECH_IN_MASK_DO_START(tech_mask, tech)
  if ((techMgr = GetTechManager(tech)))
  {
    if (!techMgr->CanWriteProfile(num))
    {
      return false;
    }
  }
  DB_FOR_EVERY_TECH_IN_MASK_DO_END

  return true;
}

bool DBMgr::ShouldBeTreatedAsNonPersistent(
  const Profile *profile)
{
  if ((profile) && (!profile->IsPersistent()))
  {
    return true;
  }

  TechMgr *techMgr = NULL;

  if (profile)
  {
    DB_FOR_EVERY_TECH_IN_MASK_DO_START(profile->GetSupportedTechMask(), tech)
    if ((techMgr = GetTechManager(tech)))
    {
      if (!techMgr->ShouldBeTreatedAsNonPersistent(profile->GetNum()))
      {
        return false;
      }
    }
    DB_FOR_EVERY_TECH_IN_MASK_DO_END
  } 
  else
  {
    PROFILE_DB_LOG_ERROR("Profile Ptr is NULL. Ds Profile DB is in a bad state");
  }

  return true;
}


/* normally, it is enforced that file creation for profiles which are being modified 
   have been issued. For some profiles we want to make an exception for this rule and
   actually allow creating the file during the modification if the file does not exist */
bool DBMgr::AllowToCreateFileOnFirstModify(
  const Profile *profile)
{
  TechMgr *techMgr = NULL;

  DB_FOR_EVERY_TECH_IN_MASK_DO_START(profile->GetSupportedTechMask(), tech)
  if ((techMgr = GetTechManager(tech)))
  {
    if (!techMgr->AllowToCreateFileOnFirstModify(profile->GetNum()))
    {
      return false;
    }
  }
  DB_FOR_EVERY_TECH_IN_MASK_DO_END

  return true;
}

ds_profile_db_tech_type DBMgr::GetTechType(ds_profile_db_profile_num_type num,
                                           ds_profile_db_subs_type subs)
{
  ds_profile_db_tech_type tech_type = DB_TECH_INVALID;

  /* TODO revisit the implementation in phase 2 */
  if (callbackMgr->WithinCallback())
  {
    return DB_TECH_INVALID;
  }

  if (0 != mutex->Lock())
  {
    return DB_TECH_INVALID;
  }
  AutoReleseMutex a_m(mutex);

  if (DB_RESULT_SUCCESS != Profile::ValidateProfileNum(num))
  {
    return tech_type;
  }

  tech_type = cacheMgr.GetTechType(num, subs);
  return tech_type;
}

ds_profile_db_result_type DBMgr::GetPersistence(ds_profile_db_profile_num_type num,
                                                ds_profile_db_subs_type subs,
                                                boolean *is_persistent)
{
  ds_profile_db_result_type status = DB_RESULT_FAIL;

  /* TODO revisit the implementation in phase 2 */
  if (callbackMgr->WithinCallback())
  {
    return DB_RESULT_ERR_INVAL_OP;
  }

  if (0 != mutex->Lock())
  {
    return DB_RESULT_FAIL;
  }
  AutoReleseMutex a_m(mutex);

  if (DB_RESULT_SUCCESS != (status = Profile::ValidateProfileNum(num)))
  {
    return status;
  }

  status = cacheMgr.GetPersistence(num, subs, is_persistent);
  return status;
}

ds_profile_db_result_type DBMgr::GetProfileclient(ds_profile_db_profile_num_type num,
                                                ds_profile_db_subs_type subs,
                                                uint8 *profile_client)
{
  ds_profile_db_result_type status = DB_RESULT_FAIL;

  /* TODO revisit the implementation in phase 2 */
  if (callbackMgr->WithinCallback())
  {
    return DB_RESULT_ERR_INVAL_OP;
  }

  if (0 != mutex->Lock())
  {
    return DB_RESULT_FAIL;
  }
  AutoReleseMutex a_m(mutex);

  if (DB_RESULT_SUCCESS != (status = Profile::ValidateProfileNum(num)))
  {
    return status;
  }

  status = cacheMgr.GetProfileclient(num, subs, profile_client);
  return status;
}


ds_profile_db_result_type DBMgr::GetParamsInUse(Profile *profile,
                                                ds_profile_db_ident_type ident,
                                                ds_profile_db_info_type  *info,
                                                ds_profile_db_tech_type  tech_type,
                                                ds_profile_db_subs_type  subs)

{
  /* TODO revisit the implementation in phase 2 */
  if (callbackMgr->WithinCallback())
  {
    return DB_RESULT_ERR_INVAL_OP;
  }

  if (0 != mutex->Lock())
  {
    return DB_RESULT_FAIL;
  }
  AutoReleseMutex a_m(mutex);

  boolean is_tech_param_set = false;
  TechMgr *tech_mgr = GetTechManager(tech_type);
  if (tech_mgr)
  {
    tech_mgr->GetIsEpcParamSet(profile->GetNum(), ident, &is_tech_param_set, subs);
  }

  return (profile->GetParamInUse(ident, info, tech_type, is_tech_param_set));
}

void DBMgr::CommandHandler
(
  ds_cmd_type *cmd_ptr
)
{
  switch (cmd_ptr->hdr.cmd_id)
  {
    case DS_CMD_PROFILE_DB_NV_REFRESH_EXT:
      RefreshCommandHandler(cmd_ptr);
      break;
    default:
      break;
  }

}

void DBMgr::DeviceModeCommandHandler
(
  ds3gsubsmgr_event_info_type *event_info_ptr
)
{
  ds_profile_db_subs_type subs = DB_ACTIVE_SUBSCRIPTION_NONE;
  ds_profile_db_result_type res = DB_RESULT_SUCCESS;
  uint16 profile_count = 0;

  /*
  1.) If multi Sim then read the profiles from the other directories 
  2.) Else dont do anything if single sim 
  */
  if (callbackMgr->WithinCallback())
  {
    res = DB_RESULT_ERR_INVAL_OP;
    return;
  }

  if (0 != mutex->Lock())
  {
    res = DB_RESULT_FAIL;
    return;
  }

  AutoReleseMutex a_m(mutex);

  if (((event_info_ptr->device_mode.curr == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_STANDBY) ||
      (event_info_ptr->device_mode.curr == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE))
      && event_info_ptr->device_mode.prev == SYS_MODEM_DEVICE_MODE_SINGLE_SIM)
  {
    subs = DB_PROFILE_ACTIVE_SUBSCRIPTION_2;
    res = ReadFromEFSHelper(subs, DB_POWER_UP_EVENT, &profile_count);
    if (DB_RESULT_SUCCESS != res)
    {
      goto bail;
    }
	theInstance->subsProfilesReady[subs] = TRUE;
  } 
  else if ((event_info_ptr->device_mode.curr == SYS_MODEM_DEVICE_MODE_TRIPLE_SIM_TRIPLE_STANDBY) &&
           (event_info_ptr->device_mode.prev != SYS_MODEM_DEVICE_MODE_TRIPLE_SIM_TRIPLE_STANDBY))
  {
    //Read from subs2 and subs 3
    subs = DB_PROFILE_ACTIVE_SUBSCRIPTION_2;
    res = ReadFromEFSHelper(subs, DB_POWER_UP_EVENT, &profile_count);
    if (DB_RESULT_SUCCESS != res)
    {
      goto bail;
    }
	theInstance->subsProfilesReady[subs] = TRUE;
	
    subs = DB_PROFILE_ACTIVE_SUBSCRIPTION_3;
    res = ReadFromEFSHelper(subs, DB_POWER_UP_EVENT, &profile_count);
    if (DB_RESULT_SUCCESS != res)
    {
      goto bail;
    }
    profile_count = 0;
	theInstance->subsProfilesReady[subs] = TRUE;
  }

bail:
  PROFILE_DB_LOG_HIGH_3("DeviceModeCommand Handler current device mode %d, "
                        "prev device mode %d, error %d",
                        (int)event_info_ptr->device_mode.curr, 
                        (int)event_info_ptr->device_mode.prev, res);
}

void DBMgr::AttachProfileCommandHandler
(
  ds3geventmgr_attach_pdn_list_type   *event_info_ptr,
  ds_profile_db_subs_type                    subs_id
)
{
  ds_profile_db_profile_num_type nums[DS_PROFILE_DB_MAX_PERS_PROFILE_COUNT];
  ds_profile_db_result_type      result = DB_RESULT_FAIL;
  uint32                         num_attach_profiles = 0;
  uint32                         iter = 0;
  uint32                         attach_iter = 0;
  typedef enum 
  {
    DS_PROFILE_DB_LOCAL_ERR_NONE = 0,
    DS_PROFILE_DB_LOCAL_ERR_ATTACH_PROFILE_NUM_INVALID = 1,
    DS_PROFILE_DB_LOCAL_ERR_DELETE_FAIL
  } ds_profile_db_local_err_type;

  ds_profile_db_local_err_type local_err = 
    DS_PROFILE_DB_LOCAL_ERR_NONE;
/*---------------------------------------------------------------------------*/

  if (callbackMgr->WithinCallback())
  {
    result = DB_RESULT_ERR_INVAL_OP;
    return;
  }

  if (0 != mutex->Lock())
  {
    result = DB_RESULT_FAIL;
    return;
  }

  AutoReleseMutex a_m(mutex);

  memset(nums, 0,
         sizeof(ds_profile_db_profile_num_type)*
         DS_PROFILE_DB_MAX_PERS_PROFILE_COUNT);

  do 
  {
    if (event_info_ptr->attach_pdn_profile_list_num <= 0)
    {
      local_err = 
        DS_PROFILE_DB_LOCAL_ERR_ATTACH_PROFILE_NUM_INVALID;
      break;
    }

    result = cacheMgr.GetAllAttachProfiles(subs_id, 
                                           DB_TECH_3GPP,
                                           &nums[0],
                                           &num_attach_profiles);

    if (DB_RESULT_SUCCESS == result)
    {
      for (iter = 0; iter < event_info_ptr->attach_pdn_profile_list_num; iter++)
      {
        for (attach_iter = 0; attach_iter < num_attach_profiles; attach_iter++)
        {
          if (event_info_ptr->attach_pdn_profile_list[iter] ==
               nums[attach_iter])
          {
            result = DeleteProfileInternal(nums[attach_iter],
                                           DB_TECH_TO_TECHMASK(DB_TECH_3GPP),
                                           subs_id);
            if (DB_RESULT_SUCCESS != result) 
            {
              local_err = DS_PROFILE_DB_LOCAL_ERR_DELETE_FAIL;
            }
          }
        }
      }
    }   
  } while (0);

  PROFILE_DB_LOG_HIGH_3("AttachProfileCommandHandler Handler exit "
                        "with numprofile %d , num of attach profile present %d"
                        "and local err %d", 
                        event_info_ptr->attach_pdn_profile_list_num,
                        num_attach_profiles,
                        local_err);

  return;
}

void DBMgr::ResetNumbers(ds_profile_db_subs_type subs)
{
  for (int i=0; i < DB_TECH_MAX - 1; i++)
  {
    if ( techMgrs[i] )
    {
      techMgrs[i]->ResetNonPersistentCount(subs);
    }
  }
}

void DBMgr::RefreshCommandHandler
(
  ds_cmd_type *cmd_ptr
)
{
  ds_profile_db_num_tech_type removed_entries[DS_PROFILE_DB_MAX_PROFILE_COUNT];
  uint32 count = 0;
  Profile **profiles = NULL;
  ds_profile_db_result_type res;
  uint16 count_of_profiles_read = 0;
  boolean enable_default_profile_delete = TRUE;
  sys_modem_as_id_e_type subs_id = SYS_MODEM_AS_ID_NONE;

  if (callbackMgr->WithinCallback())
  {
    res = DB_RESULT_ERR_INVAL_OP;
    return;
  }

  if (0 != mutex->Lock())
  {
    res = DB_RESULT_FAIL;
    return;
  }

  AutoReleseMutex a_m(mutex);

  ds_profile_nv_refresh_cmd_type *cmd_info = (ds_profile_nv_refresh_cmd_type *)cmd_ptr->cmd_payload_ptr;

  /*
  1.) Clear the cache with subs id content 
  2.) Release the numbers. 
  3.) ReRead from the subs id directory and re populate 
  4.) Notify ModeHandlers about the New Event with same payload of new Profiles 
  5.) Consider all the corner cases 
  */

  memset(removed_entries, 
         0, 
         sizeof(ds_profile_db_num_tech_type)*DS_PROFILE_DB_MAX_PROFILE_COUNT);

  cacheMgr.RemoveCacheForSubs((ds_profile_db_subs_type)(cmd_info->subs), 
                              removed_entries, 
                              &count);
  for (uint32 i = 0; i < count; i++)
  {
    ReleaseNumForProfile((removed_entries[i].is_persistent == TRUE), 
                         removed_entries[i].profile_num,
                         removed_entries[i].tech_mask,
                         (ds_profile_db_subs_type)(cmd_info->subs),
                         enable_default_profile_delete);
  }
  ResetNumbers((ds_profile_db_subs_type)(cmd_info->subs));
  if(cmd_info->subs >= DB_PROFILE_ACTIVE_SUBSCRIPTION_1 && 
     cmd_info->subs < DB_PROFILE_ACTIVE_SUBSCRIPTION_MAX )
  {
    theInstance->subsProfilesReady[cmd_info->subs] = FALSE;
  }
  res = ReadFromEFSHelper((ds_profile_db_subs_type)(cmd_info->subs),
                          DB_REFRESH_PROFILE_EVENT, 
                          &count_of_profiles_read);
  if(res == DB_RESULT_SUCCESS && 
     cmd_info->subs >= DB_PROFILE_ACTIVE_SUBSCRIPTION_1 && 
     cmd_info->subs < DB_PROFILE_ACTIVE_SUBSCRIPTION_MAX )
  {
    theInstance->subsProfilesReady[cmd_info->subs] = TRUE;
  }
//Notify All Cients with 0 Profiles but NV refresh event.
  count_of_profiles_read = 0;
  if ((res = NotifyClientsDuringRefresh(profiles, 
                                        count_of_profiles_read, 
                                        DB_REFRESH_PROFILE_EVENT,
                                        (ds_profile_db_subs_type)(cmd_info->subs))) 
      != DB_RESULT_SUCCESS)
  {
    goto bail;
  }
  res = DB_RESULT_SUCCESS;
  /* fall through */
bail:
  
  subs_id = ds3gsubsmgr_subs_id_ds_to_cm((ds_sys_subscription_enum_type) cmd_info->subs);
  ds3geventmgr_set_and_notify_nv_refresh_status(DS3GEVENTMGR_CLIENT_ID_PROFILE,
                                                subs_id);
 
  PROFILE_DB_LOG_HIGH_2("RefreshCommand Handler for subs %d, error %d",
                        cmd_info->subs, res);
  return;
}



ds_profile_db_result_type DBMgr::NotifyClientsDuringRefresh(
  Profile **profiles,
  uint16 profile_count,
  ds_profile_db_event_type event,
  ds_profile_db_subs_type subs)
{
  

  ds_profile_db_result_type res;

  if (DB_RESULT_SUCCESS != (res = callbackMgr->Notify(profiles, 
                                                      profile_count,
                                                      event, 
                                                      subs)))
  {
    goto bail;
  }

  res = DB_RESULT_SUCCESS;
bail:

  PROFILE_DB_LOG_MED_2("NotifyClients(): event %d, error %d", 
                       event, res);
  return res;
}


ds_profile_db_result_type DBMgr::ReadFromEFSHelper
(
  ds_profile_db_subs_type subs,
  ds_profile_db_event_type event,
  uint16 *profile_count
)
{
  ds_profile_db_result_type res;
  Profile **profiles = NULL;
  unsigned int profiles_count = DS_PROFILE_DB_MAX_PROFILE_COUNT;
  unsigned int created_on_the_fly = 0;
  int iter = 0;
  File *file = NULL;

  /* read everything there is from EFS */
  /*************************************/
  PS_SYSTEM_HEAP_MEM_ALLOC(profiles, 
                           profiles_count * sizeof(Profile *),
                           Profile**);
  if (NULL == profiles)
  {
    res = DB_RESULT_ERR_OUT_OF_MEMORY;
    goto bail;
  }
  memset(profiles, 0, profiles_count * sizeof(Profile *));

  file = Platform::Instance()->GetFile(subs);
  if (!file)
  {
    res = DB_RESULT_ERR_FILE_ACCESS;
    goto bail;
  }
    /* mark in cache special profiles (KDDI / OMH / etc) as if they exist, but do not create them */
  if (DB_RESULT_SUCCESS != (res = theInstance->MarkSpecialProfilesInCache(subs)))
    {
      goto bail;
    }

  res = file->ReadAll(profiles, 
                      &profiles_count, 
                      subs);
  if (DB_RESULT_ERR_OUT_OF_PROFILES == res)
  {
    PROFILE_DB_LOG_HIGH_1("Profiles on EFS exceed the number supported, res = %d", res);
  }
  else if (DB_RESULT_SUCCESS != res)
  {
    goto bail;
  }

  
  res = StoreInCacheDuringInit(profiles, 
                               profiles_count, 
                               subs);
  if (DB_RESULT_SUCCESS != res)
  {
    PROFILE_DB_LOG_HIGH_1("FromFileToCache(): could not store "
                          "profiles fetched from EFS to cache,"
                          "error %d, still proceeding", res);
    res = DB_RESULT_SUCCESS;
  }

  if (subs == (ds_profile_db_subs_type)DB_PROFILE_ACTIVE_SUBSCRIPTION_1 || (subs == (ds_profile_db_subs_type)DB_PROFILE_ACTIVE_SUBSCRIPTION_2))
  {
    /* create some special profiles on the fly */
    created_on_the_fly = DS_PROFILE_DB_MAX_PROFILE_COUNT - profiles_count;
    if (DB_RESULT_SUCCESS != (res = 
                              theInstance->OnTheFlyToCache(profiles + profiles_count,
                                                           &created_on_the_fly, 
                                                           subs)))
    {
      goto bail;
    }
    profiles_count += created_on_the_fly;
  }
  if (file)
  {
    delete file;
    file = NULL;
  }
  //Notify Mode Handlers with the new profiles and the event with subs id.
  for (iter = 0; iter < DS_PROFILE_NUM_MODE_HANDLERS; iter++)
  {
    if (ids[iter].cback != NULL)
    {
      if ((res = NotifyClientWithPowerUpEvent(ids[iter].num,
                                              ids[iter].tech_mask,
                                              ids[iter].treat_tech_mask_as_OR,
                                              ids[iter].cback, ids[iter].callback_id,
                                              ids[iter].user_data,
                                              subs,
                                              event,
                                              profiles,
                                              profiles_count))
        != DB_RESULT_SUCCESS)
      {
        goto bail;
      }
    }
  }
  
  /* Notify clients about subs ready event */
  if (DB_POWER_UP_EVENT == event)
  {
    if (DB_RESULT_SUCCESS != (res = callbackMgr->Notify(NULL, 0, DB_PROFILES_READY_ON_SUBS_EVENT, subs)))
    {
      goto bail;
    }
  }
  
  res = DB_RESULT_SUCCESS;
  /* fall through */
bail:
  *profile_count = profiles_count;
  CleanupProfiles(profiles_count, profiles);
  if (file)
  {
    delete file;
    file = NULL;
  }

  PROFILE_DB_LOG_HIGH_2("Result From EFS helper exiting with %d, profiles count %d", 
                        res, profiles_count);
  return res;
}

ds_profile_db_result_type DBMgr::IsValidProfile(
  ds_profile_db_profile_num_type num,
  ds_profile_db_subs_type        subs,
  ds_profile_db_tech_type        tech_type,
  boolean                        *is_valid
)
{

  /* TODO revisit the implementation in phase 2 */
  if (callbackMgr->WithinCallback())
  {
    return DB_RESULT_FAIL;
  }

  if (0 != mutex->Lock())
  {
    return DB_RESULT_FAIL;
  }
  AutoReleseMutex a_m(mutex);

  if (DB_RESULT_SUCCESS != Profile::ValidateProfileNum(num))
  {
    return DB_RESULT_ERR_INVAL_PROFILE_NUM;
  }

  *is_valid = false;
  return cacheMgr.IsValidProfile(num, subs, tech_type, is_valid);
}

void DBMgr::SetEPCSupportVal
(
  uint8 epc_support_val
)
{
  if(theInstance)
  {
    if (epc_support_val > 1) 
    {
      theInstance->ds_profile_db_epc_support_val = false;
    }
    else
    {
      theInstance->ds_profile_db_epc_support_val = ((epc_support_val)?1:0);
    }
  }
}

bool DBMgr::GetEPCSupportVal
(
 void
)
{
  if (theInstance) 
  {
    return theInstance->ds_profile_db_epc_support_val;
  }
  /*Instance is not defined, return default value as false and log error*/
  return false;
}

ds_profile_db_result_type DBMgr::DeleteProfileInternal(
  ds_profile_db_profile_num_type  profile_num,
  ds_profile_db_supported_tech_mask tech_mask,
  ds_profile_db_subs_type subs,
  boolean enable_deleting_default_profile /* = false */)
{
  bool is_persistent = false;
  char apn_name[DS_PROFILE_DB_MAX_APN_NAME_LEN + 1];
  ds_profile_db_info_type info_type = { 0 };
  ds_profile_db_deleted_profile_entry deleted_profile = { 0 };
  Profile *profile = NULL;
  ds_profile_db_result_type res;

  File *file = NULL;

  /* do a quick before you go to heavy operations of constructing a full profile from cache */
  CacheMgr::cache_entry *c_e = cacheMgr.GetCache(profile_num, tech_mask, subs);
  if (!c_e)
  {
    res = DB_RESULT_ERR_INVAL_PROFILE_NUM;
    goto bail;
  }

  if (!CanDeleteProfile(profile_num, c_e->tech_mask, subs, enable_deleting_default_profile))
  {
    res = DB_RESULT_ERR_INVAL_PROFILE_NUM;
    goto bail;
  }

  if (DB_RESULT_SUCCESS != 
      (res = cacheMgr.GetCache(profile_num, 
                               tech_mask, 
                               subs, 
                               &profile)))
  {
    goto bail;
  }
  if (profile)
  {
    profile->SetSubscriptionId(subs, false);
    if (DB_RESULT_SUCCESS != 
        (res = 
         ReleaseNumForProfile(!ShouldBeTreatedAsNonPersistent(profile),
                              profile_num,
                              profile->GetSupportedTechMask(),
                              subs,
                              enable_deleting_default_profile)))
    {
      goto bail;
    }
  } 
  else
  {
    res = DB_RESULT_ERR_OUT_OF_MEMORY;
    goto bail;
  }

  /* should succeed as we checked cache validity previously */
  cacheMgr.RemoveCache(profile_num, tech_mask, subs);

  if (DB_RESULT_SUCCESS != (res = NotifyClients(profile, DB_DELETE_PROFILE_EVENT, subs)))
  {
    PROFILE_DB_LOG_ERROR_1("DeleteProfile(): could not notify clients, error %d", res);
  }

  is_persistent = !ShouldBeTreatedAsNonPersistent(profile);

  if (is_persistent)
  {
    file = Platform::Instance()->GetFile(subs);
    if (!file)
    {
      res = DB_RESULT_ERR_FILE_ACCESS;
      goto bail;
    }
    if (DB_RESULT_SUCCESS != (res = file->Delete(profile_num, subs)))
    {
      goto bail;
    }
  }
  deleted_profile.num = profile_num;
  deleted_profile.tech_mask = profile->GetSupportedTechMask();
  info_type.buf = apn_name;
  info_type.len = DS_PROFILE_DB_MAX_APN_NAME_LEN + 1;
  if (DB_RESULT_SUCCESS != profile->GetParam(GENERAL_PROFILE_PARAM_APN_NAME,
                                             &info_type))
  {
    goto bail;
  }
  memscpy(deleted_profile.apn_name, DS_PROFILE_DB_MAX_APN_NAME_LEN + 1, info_type.buf, info_type.len);

  ds_profile_add_entry_to_delete_profile_list(&deleted_profile);

  res = DB_RESULT_SUCCESS;
  /* fall through */

bail:

  if (file)
  {
    delete file;
  }

  if (profile)
  {
    delete profile;
  }

  PROFILE_DB_LOG_HIGH_3("DeleteProfile(): profile_num %d , tech_mask %d, res %d", 
                        profile_num, tech_mask, res);

  return res;
}

ds_profile_db_result_type DBMgr::SetCacheAttachFlag(ds_profile_db_profile_num_type    num,
                                             ds_profile_db_subs_type           subs,
                                             ds_profile_db_supported_tech_mask tech_type,
                                             boolean                           is_attach)
{
  return cacheMgr.SetAttachFlag(num, subs, tech_type, is_attach);
}
