/*===========================================================================


   R F  D r i v e r  Meas(InterFreq meas) Main Control


DESCRIPTION
  This file contains all declarations and definitions necessary to use
  the WCDMA Main Control.

Copyright (c) 2008 - 2022 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
============================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rfa.mpss/4.11/rf/meas/rf/mc/src/rfmeas_mc.c#2 $
$DateTime: 2022/04/07 02:00:49 $
$Author: mplcsds1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/10/22   cv      Set flag to pick base address of IRAT buffer
01/15/18   am      F3 reduction
12/08/15   hkm     Return FAILURE in case of script building failure
09/01/15   hkm     Remove shadow backup code as that is not needed anymore
08/12/15   hkm     Debug enhancement to identify the exact failure cause during build scripts. 
05/06/15   nv      Adding logging support to log rfm meas APIs and their parameters. 
05/02/15   zhw     Remove MEAS Featurization
04/24/15   rmb     Add a source flag for enter, build, reset scripts and exit.
04/06/15   rmb     Merge back meas interface changes onto Mainline with feature flag 
03/19/15   rmb     Add support for Half scripts and NULL2X in common meas.
03/12/15   zhw     Adapt to unified meas param interface
11/21/14   par     Adding additional debug info for iRAT
08/25/14   piy     Removed Klockwork Errors
08/21/14   piy     Removed Compiler Warnings
08/11/14   par     Make sure the reset script is called once for target and once for source techs
11/27/13   pl      clean trigger buffer before building tune-back scripts
10/31/13   shb     Switch to C++ TRX common interface
10/18/13   pl      support building for preload and trigger script
07/22/13   nsh     Reduce f3 msg
05/31/13   pl      Avoid backing up shadow when building X2G cleanup script
04/24/13   pl      Enable Shadow back up and restore
04/08/13   pl      Adding pre-configuration into the meas framework
12/13/12   sc      Fix Klocwork errors
11/07/12   pl      Move buffer clean from script building to meas framework
11/07/12   pl      Added building of cleanup script building
10/05/12   aa      avoid W measure to be called twice in case of wtow ifreq
10/04/12   sr      Corrected the order of API calls, first call the target tech API
                   and then the src tech API. 
10/04/12   sr      IRAT code cleanup for dime. 
03/29/12   jfc     Declare global meas_script_entry_no variables
10/14/08   sr      Initial version of meas Main Control.

============================================================================*/

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include "comdef.h"
#include "rfmeas_types.h"
#include "rfmeas_mc.h"
#include "rfmeas_mdsp.h"
#include "rfcommon_core_device_manager.h"
#include "rfc_card.h"
#include "stringl.h"
#include "rflm_api_lte.h"

/*Logging buffer to keep a track of APIs that are getting called*/
rfm_meas_log_buffer_type rfmeas_mc_log_buffer = {0};

// These are referenced in dtm_meas file and need to be cleaned-up.
uint8 cur_meas_script_entry_no;
uint8 cur_meas_script_entry_page = 0;
uint8 cur_cleanup_script_entry_no = 0;
uint8 cur_cleanup_script_entry_page = 0;

/*----------------------------------------------------------------------------*/
#if 0
extern void*
rfc_common_get_cmn_device_param
(
  rfdevice_type_enum_type dev_type, uint8 instance
);
#endif
/*----------------------------------------------------------------------------*/
// IRAT interface function ptrs for each tech.
static rfmeas_mc_func_tbl_type *rfmeas_mc_func_tbl[RFCOM_NUM_MODES] = { NULL };
static uint8 rfm_meas_dev_backup_token[RFCMN_CORE_MAX_PHY_DEVICES] = {0xFF,0xFF,0xFF};
extern void rflm_lte_set_irat_flag( boolean irat_flag );

typedef enum{
    RFMEAS_NULL2X_TUNEAWAY,
    RFMEAS_SOURCE_TUNEAWAY_HALF_SCRIPT,	
    RFMEAS_TARGET_TUNEAWAY_HALF_SCRIPT,
    RFMEAS_BUILD_PRECONFIG,
    RFMEAS_BUILD_SOURCE_RESET,
    RFMEAS_BUILD_STARTUP,
    RFMEAS_BUILD_TARGET_RESET,
    RFMEAS_BUILD_CLEANUP,
}rfmeas_build_scripts_api_type;

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function will register the IRAT API function ptrs from each tech.
 
  @param
  tech: Tech which is registering its IRAT APIs func_ptr table.
  func_tbl_ptr : tech's IRAT APIs table.
 
  @retval 
  Returns TRUE if the registration is successful, else FALSE.
*/

boolean rfmeas_mc_register(rfcom_mode_enum_type tech, rfmeas_mc_func_tbl_type *func_tbl_ptr)
{
  boolean status = FALSE;

  if (tech >= RFCOM_NUM_MODES)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"Invalid tech (%d) during rfmeas API registeration!", tech);
    return status;
  }

  if (func_tbl_ptr == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"func_tbl_ptr == NULL during rfmeas API registeration!", 0);
    return status;
  }

  rfmeas_mc_func_tbl[tech] = func_tbl_ptr;
  status = TRUE;

  return (status);
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
    rfm_meas_common_get_irat_info_param function will basically allow RFSW to pass to
    L1 how much the start-up and clean-up scripts take, and IRAT specific info if necessary
    for individual IRAT combination.
 
  @param rfm_meas_irat_info_type: data structure for all info to be returned
  @param rfm_cb_handler_type: callback 
 
  @pre
  rfm_init() must have already been called.
  We should be in a certain technology.

  @retval
  Returns an enum type (rfm_meas_result_type) denoting whether the function did work
  correctly or not.
*/

rfm_meas_result_type rfmeas_mc_irat_info_get(rfm_meas_irat_info_type *irat_info_param, 
                                             rfm_cb_handler_type cb_handler )
{
  rfm_meas_result_type result = RFM_MEAS_COMMON_SUCCESS;
  rfcom_mode_enum_type src_tech = RFM_INVALID_MODE;
  rfcom_mode_enum_type target_tech = RFM_INVALID_MODE;

  if (irat_info_param == NULL)
  {
    result = RFM_MEAS_COMMON_FAILURE;
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"Pointer irat_info_param is NULL!", 0);
    return result;
  }

  src_tech = irat_info_param->header.source_tech;
  target_tech = irat_info_param->header.target_tech;

  if (src_tech >= RFCOM_NUM_MODES || src_tech == RFM_PARKED_MODE || src_tech == RFM_ALL_MODES ||
      target_tech >= RFCOM_NUM_MODES || target_tech == RFM_PARKED_MODE || target_tech == RFM_ALL_MODES)
  {
    result = RFM_MEAS_COMMON_FAILURE;
    MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR,"Invalid tech parameters: src_tech=%d, target_tech=%d", src_tech, target_tech);
    return result;
  }

  if (((RFM_PARKED_MODE < src_tech) && (src_tech < RFCOM_NUM_MODES)) && 
      ((RFM_PARKED_MODE < target_tech) && (target_tech < RFCOM_NUM_MODES)))
  {  
    if (rfmeas_mc_func_tbl[src_tech] == NULL || rfmeas_mc_func_tbl[target_tech] == NULL)
    {
      result = RFM_MEAS_COMMON_FAILURE;
      MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR,"rfmeas_mc_func_tbl  is NULL! src(0x%x) target(0x%x)", 
            rfmeas_mc_func_tbl[src_tech], rfmeas_mc_func_tbl[target_tech]);
      return result;
    }
  }

  MSG_2(MSG_SSID_RF, MSG_LEGACY_HIGH, "rfmeas_mc_irat_info_get(): source_tech= %d, target_tech= %d", 
          src_tech, target_tech);

  // tech's module to populate the required information(startup and cleanup timing info.) for IRAT operation.

  if (((RFM_PARKED_MODE < src_tech) && (src_tech < RFCOM_NUM_MODES)) && 
      ((RFM_PARKED_MODE < target_tech) && (target_tech < RFCOM_NUM_MODES)))
  {  
    if (rfmeas_mc_func_tbl[src_tech]->rfmeas_mc_irat_info_get_fp != NULL)
    {
      result &= rfmeas_mc_func_tbl[src_tech]->rfmeas_mc_irat_info_get_fp(irat_info_param);
    }
    else
    {
      MSG_1(MSG_SSID_RF, MSG_LEGACY_HIGH,"src_tech=%d rfmeas_mc_irat_info_get_fp == NULL", src_tech);
    }

    if (rfmeas_mc_func_tbl[target_tech]->rfmeas_mc_irat_info_get_fp != NULL)
    {
      result &= rfmeas_mc_func_tbl[target_tech]->rfmeas_mc_irat_info_get_fp(irat_info_param);
    }
    else
    {
      MSG_1(MSG_SSID_RF, MSG_LEGACY_HIGH,"target_tech=%d rfmeas_mc_irat_info_get_fp == NULL", target_tech);
    }
  }

  rfmeas_mc_common_log_data(RFM_MEAS_COMMON_GET_IRAT_INFO_PARAM,(void*)irat_info_param,cb_handler);

  return(result);
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
    This function is called first when the measurement is requested. Each technology
    basically prepares for the measurement.
 
  @param rfm_meas_enter_param : This is a pointer that basically has a header that has
  the target technology and the source technology. Based on this header, the rest is casted as
  a tech specific pointer.
  @param cb_handler : This is the call back function.
 
  @pre
  rfm_init() must have already been called.
  We should be in a certain technology.

  @retval
  Returns an enum type (rfm_meas_result_type) denoting whether the function did work
  correctly or not.
*/
rfm_meas_result_type rfmeas_mc_enter( rfm_meas_enter_param_type *meas_enter_param, 
                                      rfm_cb_handler_type cb_handler )
{
  rfcom_mode_enum_type src_tech = RFM_INVALID_MODE;
  rfcom_mode_enum_type target_tech = RFM_INVALID_MODE;
  rfm_meas_result_type result = RFM_MEAS_COMMON_SUCCESS;

  if (meas_enter_param == NULL)
  {
    result = RFM_MEAS_COMMON_FAILURE;
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"meas_enter_param is NULL!", 0);
    return result;
  }

  if (meas_enter_param->enter_params == NULL) 
  {
    result = RFM_MEAS_COMMON_FAILURE;
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"Enter params in meas_enter_param is NULL!", 0);
    return result;
  }

  src_tech = meas_enter_param->enter_params->header.source_tech;
  target_tech = meas_enter_param->enter_params->header.target_tech;

  /* Handle NULL2X separately as RF does not need anything to be populated for source */
  if ( meas_enter_param->enter_params->script_type == RFM_MEAS_PROGRAM_NULL2X ) 
  { 
    if ( !((RFM_PARKED_MODE < target_tech) && (target_tech < RFCOM_NUM_MODES)) ) 
    {
      result = RFM_MEAS_COMMON_FAILURE;
      MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"Invalid tech parameters for NULL2X: target_tech=%d", target_tech);
      return result;
    }

    if (rfmeas_mc_func_tbl[target_tech]->rfmeas_mc_enter_fp != NULL)
    {
      result &= rfmeas_mc_func_tbl[target_tech]->rfmeas_mc_enter_fp(meas_enter_param, FALSE);
    }
    else
    {
      MSG_1(MSG_SSID_RF, MSG_LEGACY_HIGH,"target_tech=%d rfmeas_mc_enter_fp == NULL", target_tech);
    }
  } /* if ( meas_enter_param->enter_params->script_type == RFM_MEAS_PROGRAM_NULL2X */

  /* For IRAT, QxTA and Half scripts */
  else
  {
    if (src_tech >= RFCOM_NUM_MODES || src_tech == RFM_PARKED_MODE || src_tech == RFM_ALL_MODES ||
        target_tech >= RFCOM_NUM_MODES || target_tech == RFM_PARKED_MODE || target_tech == RFM_ALL_MODES)
    {
      result = RFM_MEAS_COMMON_FAILURE;
      MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR,"Invalid tech parameters: src_tech=%d, target_tech=%d", src_tech, target_tech);
      return result;
    }

    if (((RFM_PARKED_MODE < src_tech) && (src_tech < RFCOM_NUM_MODES)) && 
        ((RFM_PARKED_MODE < target_tech) && (target_tech < RFCOM_NUM_MODES)))
    {  
      if (rfmeas_mc_func_tbl[src_tech] == NULL || rfmeas_mc_func_tbl[target_tech] == NULL)
      {
        result = RFM_MEAS_COMMON_FAILURE;
        MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR,"rfmeas_mc_func_tbl  is NULL! src(0x%x) target(0x%x)", 
              rfmeas_mc_func_tbl[src_tech], rfmeas_mc_func_tbl[target_tech]);
        return result;
      }
    }

    MSG_4(MSG_SSID_RF, MSG_LEGACY_HIGH, "rfmeas_mc_enter(): source_tech= %d, target_tech= %d source dev %d target dev %d", 
          src_tech, target_tech, 
          meas_enter_param->enter_params->header.source_param_v2[0].rx_dev_handle[0].device, 
          meas_enter_param->enter_params->header.target_param_v2[0].rx_dev_handle[0].device );

    if (((RFM_PARKED_MODE < src_tech) && (src_tech < RFCOM_NUM_MODES)) && 
        ((RFM_PARKED_MODE < target_tech) && (target_tech < RFCOM_NUM_MODES)))
    { 
      /* For Half script call enter only for the target tech */
      if (meas_enter_param->enter_params->script_type == RFM_MEAS_PROGRAM_TUNEAWAY_HALF_SCRIPT)
      {
        if (rfmeas_mc_func_tbl[target_tech]->rfmeas_mc_enter_fp != NULL)
        {
          result &= rfmeas_mc_func_tbl[target_tech]->rfmeas_mc_enter_fp(meas_enter_param, FALSE);
        }
        else
        {
          MSG_1(MSG_SSID_RF, MSG_LEGACY_HIGH,"target_tech=%d rfmeas_mc_enter_fp == NULL", target_tech);
        }
      } /* if (meas_enter_param->enter_params->script_type == RFM_MEAS_PROGRAM_TUNEAWAY_HALF_SCRIPT) */

      else
      {
        if (rfmeas_mc_func_tbl[target_tech]->rfmeas_mc_enter_fp != NULL)
        {
          result &= rfmeas_mc_func_tbl[target_tech]->rfmeas_mc_enter_fp(meas_enter_param, FALSE);
        }
        else
        {
          MSG_1(MSG_SSID_RF, MSG_LEGACY_HIGH,"target_tech=%d rfmeas_mc_enter_fp == NULL", target_tech);
        }

        if (rfmeas_mc_func_tbl[src_tech]->rfmeas_mc_enter_fp != NULL)
        {
          result &= rfmeas_mc_func_tbl[src_tech]->rfmeas_mc_enter_fp(meas_enter_param, TRUE);
        }
        else
        {
          MSG_1(MSG_SSID_RF, MSG_LEGACY_HIGH,"src_tech=%d rfmeas_mc_enter_fp == NULL", src_tech);
        }
      }

    } /*   if (((RFM_PARKED_MODE < src_tech) && (src_tech < RFCOM_NUM_MODES)) && 
        ((RFM_PARKED_MODE < target_tech) && (target_tech < RFCOM_NUM_MODES)))*/

  } /* For IRAT, QxTA and Half scripts */

  rfmeas_mc_common_log_data(RFM_MEAS_COMMON_SCRIPT_ENTER,(void*)meas_enter_param,cb_handler);

  return(result);
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
    This function is called to prepare the clean-up and start-up scripts.
 
  @param rfm_meas_setup_param : This is a pointer that basically has a header that has
  the target technology and the source technology. Based on this header, the rest is casted as
  a tech specific pointer.
  @param cb_handler : This is the call back function.
 
  @pre
  rfm_meas_common_enter function must have been called.

  @retval
  Returns an enum type (rfm_meas_result_type) denoting whether the function did work
  correctly or not.
*/
rfm_meas_result_type rfmeas_build_scripts( rfm_meas_setup_param_type *meas_scripts_param, 
                                           rfm_cb_handler_type cb_handler )
{
  rfcom_mode_enum_type src_tech = RFM_INVALID_MODE;
  rfcom_mode_enum_type target_tech = RFM_INVALID_MODE;
  rfm_meas_result_type result = RFM_MEAS_COMMON_SUCCESS;
  rf_buffer_intf *script_buf = NULL;
  rfm_meas_program_script_type script_type = RFM_MEAS_PROGRAM_STARTUP;
  rfcmn_core_dev_id_type dev_id = RFCMN_CORE_PHY_DEVICE_0;
  rfdevice_rxtx_common_class *temp_cmn_dev_ptr = NULL;

  uint8 api_result_bitmask = 0x0, shifter=1;
  
  if (meas_scripts_param == NULL)
  {
    result = RFM_MEAS_COMMON_FAILURE;
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"meas_scripts_param is NULL!", 0);
    return result;
  }

  if (meas_scripts_param->setup_params == NULL) 
  {
    result = RFM_MEAS_COMMON_FAILURE;
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"Setup params in meas_scripts_param is NULL!", 0);
    return result;
  }

  src_tech = meas_scripts_param->setup_params->header.source_tech;
  target_tech = meas_scripts_param->setup_params->header.target_tech;
  
  /*Commnet the following f3 msg due to the requirement from FT team*/
  /*MSG_3(MSG_SSID_RF, MSG_LEGACY_HIGH, "rfmeas_build_scripts(): source_tech= %d, target_tech= %d, script_type=%d", 
        src_tech, target_tech, meas_scripts_param->script_type); */

  /* Before building any script, clear the buffer of previous scripts from previous gaps */
  script_type = RFM_MEAS_PROGRAM_STARTUP;
  script_buf = rfmeas_mdsp_buffer_get(script_type);
  rf_buffer_clear(script_buf);

  script_type = RFM_MEAS_PROGRAM_PRELOAD;
  script_buf = rfmeas_mdsp_buffer_get(script_type);
  rf_buffer_clear(script_buf);

  script_type = RFM_MEAS_PROGRAM_TRIGGER;
  script_buf = rfmeas_mdsp_buffer_get(script_type);
  rf_buffer_clear(script_buf);

  script_type = RFM_MEAS_PROGRAM_CLEANUP;
  script_buf = rfmeas_mdsp_buffer_get(script_type);
  rf_buffer_clear(script_buf);

  /* For NULL2X scripts */
  if (meas_scripts_param->setup_params->script_type == RFM_MEAS_PROGRAM_NULL2X) 
  {
    if (target_tech >= RFCOM_NUM_MODES || target_tech == RFM_PARKED_MODE || target_tech == RFM_ALL_MODES)
    {
      result = RFM_MEAS_COMMON_FAILURE;
      MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"Invalid tech parameters for NULl2X: target_tech=%d", target_tech);
      return result;
    }

    /* Build only target tech tune away scripts */
    if ((RFM_PARKED_MODE < target_tech) && (target_tech < RFCOM_NUM_MODES))
    {
      if (rfmeas_mc_func_tbl[target_tech]->rfmeas_build_scripts_fp != NULL)
      {
        /* may need to split the build scripts into two APIs:
         target_tech -> cleanup() and src_tech -> startup() */
        result &= rfmeas_mc_func_tbl[target_tech]->rfmeas_build_scripts_fp(meas_scripts_param, FALSE);
        if(result != RFM_MEAS_COMMON_SUCCESS)
        {
          api_result_bitmask |= (shifter << RFMEAS_NULL2X_TUNEAWAY);
        }
      }
      else
      {
        MSG_1(MSG_SSID_RF, MSG_LEGACY_HIGH,"target_tech=%d rfmeas_build_scripts_fp == NULL", target_tech);
      }
    } /*     if ((RFM_PARKED_MODE < target_tech) && (target_tech < RFCOM_NUM_MODES))*/
   
  } /*  if (meas_scripts_param->setup_params->script_type == RFM_MEAS_PROGRAM_NULL2X) */

  /* For IRAT, QxTA and Half scripts */
  else
  {
    if (src_tech >= RFCOM_NUM_MODES || src_tech == RFM_PARKED_MODE || src_tech == RFM_ALL_MODES ||
        target_tech >= RFCOM_NUM_MODES || target_tech == RFM_PARKED_MODE || target_tech == RFM_ALL_MODES)
    {
      result = RFM_MEAS_COMMON_FAILURE;
      MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR,"Invalid tech parameters: src_tech=%d, target_tech=%d", src_tech, target_tech);
      return result;
    }

    if (rfmeas_mc_func_tbl[src_tech] == NULL || rfmeas_mc_func_tbl[target_tech] == NULL)
    {
      result = RFM_MEAS_COMMON_FAILURE;
      MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR,"rfmeas_mc_func_tbl  is NULL! src(0x%x) target(0x%x)", 
            rfmeas_mc_func_tbl[src_tech], rfmeas_mc_func_tbl[target_tech]);
      return result;
    }

    result = RFM_MEAS_COMMON_SUCCESS;

    /* For Half Scripts */
    if (meas_scripts_param->setup_params->script_type == RFM_MEAS_PROGRAM_TUNEAWAY_HALF_SCRIPT) 
    {
      /* Reset source tech scripts */
      if ((RFM_PARKED_MODE < src_tech) && (src_tech < RFCOM_NUM_MODES))
      {
        if(rfmeas_mc_func_tbl[src_tech]->rfmeas_build_reset_scripts_fp != NULL)
        {
          result &= rfmeas_mc_func_tbl[src_tech]->rfmeas_build_reset_scripts_fp(meas_scripts_param, TRUE);
          if(result != RFM_MEAS_COMMON_SUCCESS)
          {
            api_result_bitmask |= (shifter << RFMEAS_SOURCE_TUNEAWAY_HALF_SCRIPT);
          }
        }
        else
        {
          MSG_1(MSG_SSID_RF, MSG_LEGACY_HIGH,"source_tech=%d rfmeas_build_reset_scripts_fp == NULL", src_tech);
        }
      } /* if ((RFM_PARKED_MODE < src_tech) && (src_tech < RFCOM_NUM_MODES))*/
      result = RFM_MEAS_COMMON_SUCCESS;
      /* Build target tech tune away scripts */
      if ((RFM_PARKED_MODE < target_tech) && (target_tech < RFCOM_NUM_MODES))
      {
        if (rfmeas_mc_func_tbl[target_tech]->rfmeas_build_scripts_fp != NULL)
        {
          /* may need to split the build scripts into two APIs:
                 target_tech -> cleanup() and src_tech -> startup() */
          result &= rfmeas_mc_func_tbl[target_tech]->rfmeas_build_scripts_fp(meas_scripts_param, FALSE);
          if(result != RFM_MEAS_COMMON_SUCCESS)
          {
            api_result_bitmask |= (shifter << RFMEAS_TARGET_TUNEAWAY_HALF_SCRIPT);
          }		  
        }
        else
        {
          MSG_1(MSG_SSID_RF, MSG_LEGACY_HIGH,"target_tech=%d rfmeas_build_scripts_fp == NULL", target_tech);
        }
      } /*     if ((RFM_PARKED_MODE < target_tech) && (target_tech < RFCOM_NUM_MODES))*/
     
    } /*   if (meas_scripts_param->setup_params->script_type == RFM_MEAS_PROGRAM_TUNEAWAY_HALF_SCRIPT) */

    /* For QxTA and IRAT cases */
    else
    {
      // tech's module to populate the required information(startup and cleanup scripts.) for IRAT operation.
      result = RFM_MEAS_COMMON_SUCCESS;

      // Pre-configure any HW that can be programed at the time of build script
      if(rfmeas_mc_func_tbl[target_tech]->rfmeas_mc_pre_config_fp != NULL)
      {
         result &= rfmeas_mc_func_tbl[target_tech]->rfmeas_mc_pre_config_fp(meas_scripts_param, FALSE);
         if(result != RFM_MEAS_COMMON_SUCCESS)
         {
           api_result_bitmask |= (shifter << RFMEAS_BUILD_PRECONFIG);
         }
      }
      
       result = RFM_MEAS_COMMON_SUCCESS;

      // Reset any source tech settings to default for any non X2G cases
      if ( (target_tech == RFCOM_GSM_MODE) &&  (meas_scripts_param->setup_params->script_type != RFM_MEAS_PROGRAM_STARTUP))
      {
        // Do nothing
      }
      else if ((RFM_PARKED_MODE < src_tech) && (src_tech < RFCOM_NUM_MODES))
      {
        if(rfmeas_mc_func_tbl[src_tech]->rfmeas_build_reset_scripts_fp != NULL)
        {
          result &= rfmeas_mc_func_tbl[src_tech]->rfmeas_build_reset_scripts_fp(meas_scripts_param, TRUE);
          if(result != RFM_MEAS_COMMON_SUCCESS)
          {
            api_result_bitmask |= (shifter << RFMEAS_BUILD_SOURCE_RESET);
          } 
        }
        else
        {
          MSG_1(MSG_SSID_RF, MSG_LEGACY_HIGH,"source_tech=%d rfmeas_build_reset_scripts_fp == NULL", src_tech);
        }
      }
       result = RFM_MEAS_COMMON_SUCCESS;

      // Call target tech API first to build the Setup scripts. 
      if (target_tech == RFCOM_GSM_MODE &&  meas_scripts_param->setup_params->script_type != RFM_MEAS_PROGRAM_STARTUP)
      {
        // don't call the x2G cleanup build scripts API. because for X2G there will two separate API
        // calls from L1 to build startup and cleanup scripts.
      }
      else if ((RFM_PARKED_MODE < target_tech) && (target_tech < RFCOM_NUM_MODES))
      {
        if (rfmeas_mc_func_tbl[target_tech]->rfmeas_build_scripts_fp != NULL)
        {
          /* may need to split the build scripts into two APIs:
                 target_tech -> cleanup() and src_tech -> startup() */
          result &= rfmeas_mc_func_tbl[target_tech]->rfmeas_build_scripts_fp(meas_scripts_param, FALSE);
          if(result != RFM_MEAS_COMMON_SUCCESS)
          {
            api_result_bitmask |= (shifter << RFMEAS_BUILD_STARTUP);		  
          }	
        }
        else
        {
          MSG_1(MSG_SSID_RF, MSG_LEGACY_HIGH,"target_tech=%d rfmeas_build_scripts_fp == NULL", target_tech);
        }
      }

      script_type = RFM_MEAS_PROGRAM_TRIGGER;
      script_buf = rfmeas_mdsp_buffer_get(script_type);
      rf_buffer_clear(script_buf);
      result = RFM_MEAS_COMMON_SUCCESS;

      // Reset any target tech settings to default for any non X2G cases
      if ( (target_tech == RFCOM_GSM_MODE) &&  (meas_scripts_param->setup_params->script_type == RFM_MEAS_PROGRAM_STARTUP))
      {
        // Do nothing
      }
      else if ((RFM_PARKED_MODE < target_tech) && (target_tech < RFCOM_NUM_MODES))
      {
        if (rfmeas_mc_func_tbl[target_tech]->rfmeas_build_reset_scripts_fp != NULL)
        {
          result &= rfmeas_mc_func_tbl[target_tech]->rfmeas_build_reset_scripts_fp(meas_scripts_param, FALSE);
          if(result != RFM_MEAS_COMMON_SUCCESS)
          {		  
            api_result_bitmask |= (shifter << RFMEAS_BUILD_TARGET_RESET);	
          }	
        }
        else
        {
          MSG_1(MSG_SSID_RF, MSG_LEGACY_HIGH,"target_tech=%d rfmeas_build_reset_scripts_fp == NULL", target_tech);
        }
      }
      result = RFM_MEAS_COMMON_SUCCESS;

      // next call the source tech API to build the clean-up scripts.
      if (target_tech == RFCOM_GSM_MODE &&  meas_scripts_param->setup_params->script_type != RFM_MEAS_PROGRAM_CLEANUP)
      {
        // don't call the x2G startup build scripts API. because for X2G there will two separate API
        // calls from L1 to build startup and cleanup scripts.
      }
      else if ((RFM_PARKED_MODE < src_tech) && (src_tech < RFCOM_NUM_MODES))
      {
        if (rfmeas_mc_func_tbl[src_tech]->rfmeas_build_scripts_fp != NULL)
        {
          /* may need to split the build scripts into two APIs:
           src_tech -> cleanup() and target_tech -> startup() */

          result &= rfmeas_mc_func_tbl[src_tech]->rfmeas_build_scripts_fp(meas_scripts_param, TRUE);
          if(result != RFM_MEAS_COMMON_SUCCESS)
          {		  
           api_result_bitmask |= (shifter << RFMEAS_BUILD_CLEANUP);	
          }
        }
        else
        {
          MSG_1(MSG_SSID_RF, MSG_LEGACY_HIGH,"src_tech=%d rfmeas_build_scripts_fp == NULL", src_tech);
        }
      }

    } /* For QxTA and IRAT cases */
  } /* For IRAT, QxTA and Half scripts */

  if(api_result_bitmask != 0x0)
  {
    result = RFM_MEAS_COMMON_FAILURE;
    MSG_1(MSG_SSID_RF, MSG_LEGACY_HIGH,"rfmeas_build_scripts: rfmeas script building returned failure with bitmask: 0x%x", api_result_bitmask);
  }

  rfmeas_mc_common_log_data(RFM_MEAS_COMMON_SCRIPT_BUILD_SCRIPT,(void*)meas_scripts_param,cb_handler);

  return(result);
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
    This function is called to exit the measurement mode. It is used to put the RF
    in the right state and do any clean-ups required.
 
  @param rfm_meas_setup_param : This is a pointer that basically has a header that has
  the target technology and the source technology. Based on this header, the rest is casted as
  a tech specific pointer.
  @param cb_handler : This is the call back function.
 
  @pre
  rfm_meas_common_script_enter function must have been called.

  @retval
  Returns an enum type (rfm_meas_result_type) denoting whether the function did work
  correctly or not.
*/
rfm_meas_result_type rfmeas_mc_exit( rfm_meas_exit_param_type *meas_exit_param, 
                                     rfm_cb_handler_type cb_handler )
{
  rfm_meas_result_type result = RFM_MEAS_COMMON_SUCCESS;
  rfcom_mode_enum_type target_tech = RFM_INVALID_MODE;
  rfcom_mode_enum_type src_tech = RFM_INVALID_MODE;
  rfcmn_core_dev_id_type dev_id = RFCMN_CORE_PHY_DEVICE_0;
  rfdevice_rxtx_common_class *temp_cmn_dev_ptr = NULL;

  if (meas_exit_param == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "meas_exit_param is NULL");
    return RFM_MEAS_COMMON_FAILURE;
  }

  if (meas_exit_param->exit_params == NULL) 
  {
    result = RFM_MEAS_COMMON_FAILURE;
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"Exitparams in meas_exit_param is NULL!", 0);
    return result;
  }

  src_tech = meas_exit_param->exit_params->header.source_tech;
  target_tech = meas_exit_param->exit_params->header.target_tech;

  rflm_lte_set_irat_flag(TRUE);

  /* Handle NULL2X separately as RF does not need anything to be populated for source */
  if ( meas_exit_param->exit_params->script_type == RFM_MEAS_PROGRAM_NULL2X ) 
  { 
    if ( !((RFM_PARKED_MODE < target_tech) && (target_tech < RFCOM_NUM_MODES)) ) 
    {
      result = RFM_MEAS_COMMON_FAILURE;
      MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"Invalid tech parameters for NULL2X: target_tech=%d", target_tech);
      return result;
    }

    if (rfmeas_mc_func_tbl[target_tech]->rfmeas_mc_exit_fp != NULL)
    {
      result &= rfmeas_mc_func_tbl[target_tech]->rfmeas_mc_exit_fp(meas_exit_param, FALSE);
    }
    else
    {
      MSG_1(MSG_SSID_RF, MSG_LEGACY_HIGH,"target_tech=%d rfmeas_mc_exit_fp == NULL", target_tech);
    }
  } /* if ( meas_exit_param->exit_params->script_type == RFM_MEAS_PROGRAM_NULL2X */

  /* For IRAT, QxTA and Half scripts */
  else
  {
    if (src_tech >= RFCOM_NUM_MODES || src_tech == RFM_PARKED_MODE || src_tech == RFM_ALL_MODES ||
        target_tech >= RFCOM_NUM_MODES || target_tech == RFM_PARKED_MODE || target_tech == RFM_ALL_MODES)
    {
      result = RFM_MEAS_COMMON_FAILURE;
      MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR,"Invalid tech parameters: src_tech=%d, target_tech=%d", src_tech, target_tech);
      return result;
    }

    if (((RFM_PARKED_MODE < src_tech) && (src_tech < RFCOM_NUM_MODES)) && 
        ((RFM_PARKED_MODE < target_tech) && (target_tech < RFCOM_NUM_MODES)))
    {  
      if (rfmeas_mc_func_tbl[src_tech] == NULL || rfmeas_mc_func_tbl[target_tech] == NULL)
      {
        result = RFM_MEAS_COMMON_FAILURE;
        MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR,"rfmeas_mc_func_tbl  is NULL! src(0x%x) target(0x%x)", 
              rfmeas_mc_func_tbl[src_tech], rfmeas_mc_func_tbl[target_tech]);
        return result;
      }
    }

    MSG_2(MSG_SSID_RF, MSG_LEGACY_HIGH, "rfmeas_mc_exit(): source_tech= %d, target_tech= %d", 
            src_tech, target_tech);

    if (((RFM_PARKED_MODE < src_tech) && (src_tech < RFCOM_NUM_MODES)) && 
        ((RFM_PARKED_MODE < target_tech) && (target_tech < RFCOM_NUM_MODES)))
    { 
      /* For Half script call exit only for the target tech */
      if (meas_exit_param->exit_params->script_type == RFM_MEAS_PROGRAM_TUNEAWAY_HALF_SCRIPT)
      {
        if (rfmeas_mc_func_tbl[target_tech]->rfmeas_mc_exit_fp != NULL)
        {
          result &= rfmeas_mc_func_tbl[target_tech]->rfmeas_mc_exit_fp(meas_exit_param, FALSE);
        }
        else
        {
          MSG_1(MSG_SSID_RF, MSG_LEGACY_HIGH,"target_tech=%d rfmeas_mc_exit_fp == NULL", target_tech);
        }
      } /*  if (meas_exit_param->exit_params->script_type == RFM_MEAS_PROGRAM_TUNEAWAY_HALF_SCRIPT) */
   
      else
      {
        // tech's module to perform meas exit functionality.
        if (rfmeas_mc_func_tbl[target_tech]->rfmeas_mc_exit_fp != NULL)
        {
          result &= rfmeas_mc_func_tbl[target_tech]->rfmeas_mc_exit_fp(meas_exit_param, FALSE);
        }
        else
        {
          MSG_1(MSG_SSID_RF, MSG_LEGACY_HIGH,"target_tech=%d rfmeas_mc_exit_fp == NULL", target_tech);
        }

        if (rfmeas_mc_func_tbl[src_tech]->rfmeas_mc_exit_fp != NULL)
        {
          result &= rfmeas_mc_func_tbl[src_tech]->rfmeas_mc_exit_fp(meas_exit_param, TRUE);
        }
        else
        {
          MSG_1(MSG_SSID_RF, MSG_LEGACY_HIGH,"src_tech=%d rfmeas_mc_exit_fp == NULL", src_tech);
        }
      }

    }   /*   if (((RFM_PARKED_MODE < src_tech) && (src_tech < RFCOM_NUM_MODES)) && 
        ((RFM_PARKED_MODE < target_tech) && (target_tech < RFCOM_NUM_MODES))) */

  
     MSG_2(MSG_SSID_RF, MSG_LEGACY_HIGH,
        "rfmeas_mc_exit status = %d, Gap abort = %d", 
           result,
           meas_exit_param->is_measurement_gap_aborted);
    
  } /* For IRAT, QxTA and Half scripts */

  rfmeas_mc_common_log_data(RFM_MEAS_COMMON_SCRIPT_EXIT,(void*)meas_exit_param,cb_handler);

  return(result);
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
    This function is used to log the APIs called and their corresponding parameters in rfm meas layer

  @param rfm_meas_common_api_type : the type of API which we want to log
  @param param_ptr                               : the pointer to strcuture of parameters of the corresponding API
  @param rfm_cb_handler_type               : the callback handler of the corresponding API

  @pre
   This is called by rfm meas APIs to log their data. If new API gets added, then this function needs to be updated with corresponding case statement.

  @retval
  none.
  */
void rfmeas_mc_common_log_data(rfm_meas_common_api_type api_type,void *param_ptr,rfm_cb_handler_type cb_handler)
{
 
 switch(api_type)
 {
  case RFM_MEAS_COMMON_GET_IRAT_INFO_PARAM:
  {
   rfmeas_mc_log_buffer.rfm_meas_log[rfmeas_mc_log_buffer.buffer_index].api_type   = api_type;
   rfmeas_mc_log_buffer.rfm_meas_log[rfmeas_mc_log_buffer.buffer_index].cb_handler = cb_handler;
   memscpy(&(rfmeas_mc_log_buffer.rfm_meas_log[rfmeas_mc_log_buffer.buffer_index].rfm_meas_param_log.rfm_meas_log_irat_info),sizeof(rfm_meas_irat_info_type),
 	 	                                                                       param_ptr,sizeof(rfm_meas_irat_info_type));
   rfmeas_mc_log_buffer.buffer_index = ++rfmeas_mc_log_buffer.buffer_index % RFM_MEAS_LOG_BUFFER_SIZE; 
  }
  break;	

  case RFM_MEAS_COMMON_SCRIPT_ENTER:
  {
   rfmeas_mc_log_buffer.rfm_meas_log[rfmeas_mc_log_buffer.buffer_index].api_type   = api_type;
   rfmeas_mc_log_buffer.rfm_meas_log[rfmeas_mc_log_buffer.buffer_index].cb_handler = cb_handler;
   memscpy(&(rfmeas_mc_log_buffer.rfm_meas_log[rfmeas_mc_log_buffer.buffer_index].rfm_meas_param_log.rfm_meas_log_enter_param),sizeof(rfm_meas_enter_param_type),
	 	                                         param_ptr,sizeof(rfm_meas_enter_param_type));
   rfmeas_mc_log_buffer.buffer_index = ++rfmeas_mc_log_buffer.buffer_index % RFM_MEAS_LOG_BUFFER_SIZE; 
  }
  break;

  case RFM_MEAS_COMMON_SCRIPT_BUILD_SCRIPT:
  {
   rfmeas_mc_log_buffer.rfm_meas_log[rfmeas_mc_log_buffer.buffer_index].api_type   = api_type;
   rfmeas_mc_log_buffer.rfm_meas_log[rfmeas_mc_log_buffer.buffer_index].cb_handler = cb_handler;
   memscpy(&(rfmeas_mc_log_buffer.rfm_meas_log[rfmeas_mc_log_buffer.buffer_index].rfm_meas_param_log.rfm_meas_log_setup_param),sizeof(rfm_meas_setup_param_type),
	 	                                         param_ptr,sizeof(rfm_meas_setup_param_type));
   rfmeas_mc_log_buffer.buffer_index = ++rfmeas_mc_log_buffer.buffer_index % RFM_MEAS_LOG_BUFFER_SIZE; 
  }
  break;

  case RFM_MEAS_COMMON_SCRIPT_EXIT:
  {
   rfmeas_mc_log_buffer.rfm_meas_log[rfmeas_mc_log_buffer.buffer_index].api_type   = api_type;
   rfmeas_mc_log_buffer.rfm_meas_log[rfmeas_mc_log_buffer.buffer_index].cb_handler = cb_handler;
   memscpy(&(rfmeas_mc_log_buffer.rfm_meas_log[rfmeas_mc_log_buffer.buffer_index].rfm_meas_param_log.rfm_meas_log_exit_param),sizeof(rfm_meas_exit_param_type),
	 	                                         param_ptr,sizeof(rfm_meas_exit_param_type));
   rfmeas_mc_log_buffer.buffer_index = ++rfmeas_mc_log_buffer.buffer_index % RFM_MEAS_LOG_BUFFER_SIZE; 
  }
  break;

  default:
   RF_MSG_1( RF_ERROR, "rfm_meas_common_log_data: Not supported API type %d",api_type);
   break;

 }

 return;
}
