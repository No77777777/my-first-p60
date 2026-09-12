/*!
   @file
   rfcommon_autopin_db.c

   @brief
   Definition of autopin data base
   This file contains all implementation related to AutoPin EFS data base.
   This file has the logic to read an AutoPin DAT file in the EFS and the
   data is stored in local data structures. 

*/

/*===========================================================================

  Copyright (c) 2013-2020 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

==============================================================================*/

/*==============================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/rfa.mpss/4.11/rf/common/rf/autopin/src/rfcommon_autopin_db.c#1 $
when       who     what, where, why
------------------------------------------------------------------------------- 
06/05/20   cv      Linking errors fix for TDSCDMA removal
01/20/20   ssl     FR 55657: Replace RF Enter/Exit with RF Sleep/Wakeup for L+L DSDS awake
05/28/16   wxl     Add API to get Autopin flag in efs and NV 
05/23/16   wxl     Temp method to avoid NS test crash for WTR2965
05/11/16   wxl     Add API to get Autopin disable bitmask from NV 
07/20/15   cdb     Initial version.

============================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/

#include "comdef.h"
#include <stringl/stringl.h>

#include "rfcommon_msg.h"
#include "rfcommon_nv.h"
#include "rfcommon_nv_mm.h"
#include "modem_mem.h"
#include "fs_lib.h"
#include "fs_public.h"
#include "rfcommon_time_profile.h"
#include "stringl.h" /* for memscpy */
#include "rfdevice_msg_log.h"
#include "rfcommon_efs.h"
#include "rfcommon_autopin_db.h"

#include "rfwcdma_data.h"
#include "rfwcdma_core_xpt.h"
#include "rf_tdscdma_core_xpt.h"
#include "rf_cdma_data.h"
#include "rf_cdma_ept.h"
#include "rf_tdscdma_mc.h"
#include "rflte_state.h"
#include "rf_cdma_nv.h"

/*===========================================================================
                           Global/Const Vars
===========================================================================*/
/*----------------------------------------------------------------------------*/
/*!
  @brief
  File name suffix for autopin dat file
*/
static const char autopin_efs_suffix[] = "autopin.dat";

/*----------------------------------------------------------------------------*/
/*!
  @brief
  EFS base folder for dat file
*/
static const char autopin_efs_base_path[] =  "/rfc/";

/*----------------------------------------------------------------------------*/
/*!
  @brief
  autopin EFS data
*/
static boolean autopin_efs_data_read_once = FALSE;
static autopin_efs_data_t autopin_efs_data = {0};

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This helper function gets the autopin EFS data

  @details
  Only check for a valid file one time. Afterwards just return pointer
  For result to be valid: File exist, file size match, revision # match
  
  @return
  if successful, return pointer to the data struct
  if fail, return NULL
*/
autopin_efs_data_t* rfcommon_autopin_efs_get_data(void)
{
  char file_path[AUTOPIN_DB_MAX_PATH_SIZE] = "\0"; // full file path
  uint8* data = NULL;                              // ptr to EFS data
  int32 data_size = 0;          
  struct fs_stat temp_buf;                         // fs buffer
  int i;
  
  /* EFS file will be read only once */
  if (autopin_efs_data_read_once == FALSE)
  {
    autopin_efs_data_read_once = TRUE;
    autopin_efs_data.valid = FALSE;
    
    (void)strlcat( file_path, autopin_efs_base_path, AUTOPIN_DB_MAX_PATH_SIZE);
    (void)strlcat( file_path, autopin_efs_suffix, AUTOPIN_DB_MAX_PATH_SIZE);

    /* Check if the EFS file exists */
    if( efs_stat( file_path, &temp_buf) == -1)
    {
      /* file not found */
      MSG_SPRINTF_1(MSG_SSID_RF, MSG_LEGACY_MED,
                    "rfcommon_autopin_efs_get_data: FILE not found: %s ",
                    file_path);
      return NULL;
    }
    /* Check if size matches expected */
    if( temp_buf.st_size != AUTOPIN_DB_DAT_SIZE)
    {
      MSG_SPRINTF_3(MSG_SSID_RF, MSG_LEGACY_MED,
                    "rfcommon_autopin_efs_get_data: File size(%d) != EXPECTED(%d). FILE %s",
                    (int)(temp_buf.st_size), AUTOPIN_DB_DAT_SIZE, file_path);
      return NULL;
    }

    data_size = (int32)temp_buf.st_size;

    /* Allocate memory for data */
    data = (uint8 *)modem_mem_alloc( data_size, MODEM_MEM_CLIENT_RFA);

    if( data == NULL)
    {
      MSG_SPRINTF_2(MSG_SSID_RF, MSG_LEGACY_MED,
                    "rfcommon_autopin_efs_get_data: Mem Alloc failed"
                    " for scenario data. FILE %s, size=%d",
                      file_path, data_size);
      return NULL;
    }

    /* Obtain file data */
    data_size = rfcommon_efs_get( file_path, data, temp_buf.st_size);

    MSG_SPRINTF_2(MSG_SSID_RF, MSG_LEGACY_MED,
                  "rfcommon_autopin_efs_get_data: File %s found. File size(%d)",
                  file_path, data_size);
    
    /* check for matching revision */
    if( data[0] != AUTOPIN_DB_REVISION)
    {
      MSG_SPRINTF_3(MSG_SSID_RF, MSG_LEGACY_MED,
                    "rfcommon_autopin_efs_get_data: File revision(%d) != EXPECTED(%d). FILE %s",
                    data[0], AUTOPIN_DB_REVISION, file_path);
      return NULL;
    }

    /* File exist, file size match, revision # match */
    /* Extract contents to record struct */
    autopin_efs_data.revision=data[0];
    memscpy( &(autopin_efs_data.record[0]), AUTOPIN_DB_MAX_TECH*sizeof(autopin_efs_data.record[0]), &data[1], data_size-1);
   
    /* print to log */
    MSG_SPRINTF_1(MSG_SSID_RF, MSG_LEGACY_MED, "rfcommon_autopin_efs_get_data: revision %d", autopin_efs_data.revision);
    for (i=0; i<AUTOPIN_DB_MAX_TECH;i++)
    {
      MSG_SPRINTF_2(MSG_SSID_RF, MSG_LEGACY_MED, "rfcommon_autopin_efs_get_data: record[%d].autopin_enable %d", i,autopin_efs_data.record[i].autopin_enable);
      MSG_SPRINTF_2(MSG_SSID_RF, MSG_LEGACY_MED, "rfcommon_autopin_efs_get_data: record[%d].loop_speed_acq %d", i,autopin_efs_data.record[i].loop_speed_acq);
      MSG_SPRINTF_2(MSG_SSID_RF, MSG_LEGACY_MED, "rfcommon_autopin_efs_get_data: record[%d].loop_speed_trk %d", i,autopin_efs_data.record[i].loop_speed_trk);
      MSG_SPRINTF_2(MSG_SSID_RF, MSG_LEGACY_MED, "rfcommon_autopin_efs_get_data: record[%d].pin_step %d", i,autopin_efs_data.record[i].pin_step);
      MSG_SPRINTF_2(MSG_SSID_RF, MSG_LEGACY_MED, "rfcommon_autopin_efs_get_data: record[%d].trk_thresh %d", i,autopin_efs_data.record[i].trk_thresh);
    }
    /* Free memory for data since content is copied out */
    modem_mem_free( data, MODEM_MEM_CLIENT_RFA);

    /* Set flags */
    autopin_efs_data.valid = TRUE;
  }
  
  if (autopin_efs_data.valid == FALSE)
  {
    return NULL;
  }
  else
  {
    return &autopin_efs_data;
  }
} /* rfcommon_autopin_efs_get_data */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function check autopin enable or not from EFS
  Called when RF tech during init process 
  
  @input
  tech info (invalid WCDMA/TDSCDMA/CDMA/LTE)
  
  @return
  if autopin enable, return TRUE
  if autopin disable, return FALSE
*/
boolean rfcommon_autopin_is_enabled_efs(rflm_tech_id_t tech)
{
#if defined(FEATURE_RF_AUTOPIN) && (RFLM_FEATURE_BUILD_MODE!=RFLM_FEATURE_BUILD_FW_LIB)
  autopin_efs_record_t* efs_record=NULL;
  efs_record = rfcommon_autopin_efs_get_record(tech);

  if (efs_record==NULL)
  {
    /* no valid EFS data. use hardcoded enables per tech */
    switch (tech)
    {
      case RFLM_TECH_WCDMA:
      case RFLM_TECH_TDSCDMA:
      case RFLM_TECH_1X:
      case RFLM_TECH_HDR:
      case RFLM_TECH_LTE:
        return TRUE;

      default:
        return FALSE;
    }
  }
  else
  {
    /* use enable setting from EFS file */
    return efs_record->autopin_enable;
  }
#else
  return FALSE;
#endif
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function gets the autopin disable bitmask from NV per tech and band
  This API is only called when tx_enable() check autopin 

  @details
  Per tech then band, get the NV bitmask value for autopin enable check

  @input
  tech info (invalid WCDMA/TDSCDMA/CDMA/LTE)

  @return
  if autopin enable, return TRUE
  if autopin disable, return FALSE
*/
boolean rfcommon_get_xpt_cfg_autopin_is_enabled(rflm_tech_id_t tech)
{
  boolean  xpt_cfg_autopin_enable = FALSE;
  rfm_device_enum_type tx_device;
  rfcom_wcdma_band_type wcdma_band;
  rfcom_tdscdma_band_type tdscdma_band;
  lm_handle_type txlm_buffer_idx = RFLTE_MC_TXLM_INVALID_HANDLE;
  uint32 nv_container = RFLTE_NV_TX_CONTAINER_DEFAULT;
  rflm_lte_dm_buf_type *lte_dm_buf = NULL;
  rflte_nv_tbl_type *nv_tbl_ptr  = NULL;

  uint8 sub_id=0;

  sub_id = rflte_mc_common_get_active_sub_id();

  switch (tech)
  {
    /* Get NV info for autopin check for each tech.
       The thirty item in NV xpt_cfg represents autopin enable or not.
       1: Disable  0:Enable*/
    case RFLM_TECH_WCDMA:
      tx_device = rfwcdma_data_get_tx_device();
      if (RFM_INVALID_DEVICE != tx_device)
      {
        wcdma_band = rfwcdma_data_get_curr_band(tx_device);
        xpt_cfg_autopin_enable = !(XPT_CFG_AUTOPIN_DISABLE_BITMASK & (uint8)rfwcdma_core_xpt_get_cfg_param(wcdma_band, DPD_CFG_THIRTY_ITEM));
      }
      break;

    #ifdef FEATURE_TDSCDMA
    case RFLM_TECH_TDSCDMA:
      tdscdma_band = rf_tdscdma_mc_state.curr_band_tx;
      if (RFCOM_BAND_TDSCDMA_INVALID != tdscdma_band)
      {
        xpt_cfg_autopin_enable = !(XPT_CFG_AUTOPIN_DISABLE_BITMASK & (uint8)rf_tdscdma_core_xpt_get_cfg_param(tdscdma_band, DPD_CFG_THIRTY_ITEM));
      }
      break;
      #endif
      
    case RFLM_TECH_1X:
    case RFLM_TECH_HDR:
	  #ifdef FEATURE_CDMA
      tx_device = rf_cdma_hdr_data_get_active_tx_device();
      if (RFM_INVALID_DEVICE != tx_device)
      {
        xpt_cfg_autopin_enable = !(XPT_CFG_AUTOPIN_DISABLE_BITMASK & (uint8)rf_cdma_xpt_get_cfg_param(tx_device, DPD_CFG_THIRTY_ITEM));
      }
      break;
	  #endif

    case RFLM_TECH_LTE:
      if (rflte_state_get_txlm_buffer_idx(sub_id,RFLTE_MC_PCELL_IDX, &txlm_buffer_idx))
      {
        if ((TRUE == rflte_state_get_txlm_dm_buf(txlm_buffer_idx, &lte_dm_buf)) && lte_dm_buf != NULL)
        {
          nv_tbl_ptr = (rflte_nv_tbl_type *) lte_dm_buf->path_info.nv_tbl_ptr;
          nv_container = lte_dm_buf->tx_data.tx_sm.curr_tx_nv_container;
          xpt_cfg_autopin_enable = !(XPT_CFG_AUTOPIN_DISABLE_BITMASK & ((uint8)((nv_tbl_ptr->tx_path_nv_data[nv_container]).tx_xpt_dpd_config_params[DPD_CFG_THIRTY_ITEM])));
        }
      }
      break;

    default:
      break;
  }

  MSG_2(MSG_SSID_RF, MSG_LEGACY_LOW,"For debug xpt cfg:tech %d and xpt_cfg_autopin_enable %d",tech,xpt_cfg_autopin_enable);
  return xpt_cfg_autopin_enable;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function gets the result of autopin disable bitmask from NV per tech and band && efs
  Only when both of them return TRUE, autopin can be enabled.

  @details
  Per tech then band, get the NV bitmask value && efs for autopin enable check 

  @input
  tech info (invalid WCDMA/TDSCDMA/CDMA/LTE)

  @return
  if autopin enable, return TRUE
  if autopin disable, return FALSE
*/
boolean rfcommon_autopin_is_enabled_efs_nv(rflm_tech_id_t tech)
{
#if defined(FEATURE_RF_AUTOPIN) && (RFLM_FEATURE_BUILD_MODE!=RFLM_FEATURE_BUILD_FW_LIB)
  autopin_efs_record_t* efs_record=NULL;
  boolean xpt_cfg_autopin_enable = FALSE;
  /* Get autopin info from EFS*/
  efs_record = rfcommon_autopin_efs_get_record(tech);
  /* Get autopin info from NV*/
  xpt_cfg_autopin_enable = rfcommon_get_xpt_cfg_autopin_is_enabled(tech);

  if (efs_record==NULL)
  {
    /* no valid EFS data. use xpt config NV check per band */
    return xpt_cfg_autopin_enable;
  }
  else
  {
    /* use enable setting from EFS file && XPT_CFG. Only both of them are TRUE, autopin is enabled*/
    return (efs_record->autopin_enable) && xpt_cfg_autopin_enable;
  }
#else
  return FALSE;
#endif
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This helper function gets the autopin EFS record for specific TECH

  @details
  Calls rfcommon_autopin_efs_get_data, which only check for a valid file one 
  time. Afterwards just return pointer to the TECH record
  
  @return
  if successful, return pointer to the data struct
  if fail, return NULL
*/
autopin_efs_record_t* rfcommon_autopin_efs_get_record(rflm_tech_id_t tech)
{
  autopin_efs_data_t* efs_data=NULL;

  efs_data = rfcommon_autopin_efs_get_data();
  if (efs_data==NULL)
  {
    return NULL;
  }
  else
  {
    /* use enable setting from EFS file */
    switch (tech)
    {
      case RFLM_TECH_WCDMA:
        return &efs_data->record[AUTOPIN_DB_WCDMA_IDX];
      break;

      case RFLM_TECH_1X:
      case RFLM_TECH_HDR:
        return &efs_data->record[AUTOPIN_DB_C2K_IDX];
      break;

      case RFLM_TECH_LTE:
        return &efs_data->record[AUTOPIN_DB_LTE_IDX];
      break;

      case RFLM_TECH_TDSCDMA:
        return &efs_data->record[AUTOPIN_DB_TDS_IDX];
      break;

      default:
        return NULL;
      break;
    }
  }
}
