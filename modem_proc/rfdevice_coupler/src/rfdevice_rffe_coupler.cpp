/*!
   @file
   rfdevice_rffe_coupler.cpp

   @brief
   Common Coupler Driver

*/

/*=============================================================================

Copyright (c) 2015-2016 by QUALCOMM Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rfdevice_coupler.mpss/1.22/src/rfdevice_rffe_coupler.cpp#1 $

when       who   what, where, why
--------   ---   ---------------------------------------------------------------
01/22/16   hzh   KW errors fix
08/31/15   hzh   Replace usage of memcpy with memscpy
07/14/15   px    Initial version

=============================================================================*/

#include "comdef.h"
#include "rfdevice_rffe_coupler.h"
#include "rfdevice_rffe_coupler_typedef.h"
#include "rfcommon_msg.h"
#include "rfdevice_msg_log.h"
#include "rfcommon_time_profile.h"
#include "stringl.h" /* for memscpy */
#include "modem_mem.h"

#ifdef TEST_FRAMEWORK
#error code not present
#endif

extern "C"
{
#include "rf_hal_rffe.h"
#include "fs_public.h"
#include "modem_mem.h"
}


/*----------------------------------------------------------------------------*/
/*!
  @brief
  Create specific COUPLER based on the device_info
  
*/
rfdevice_rffe_coupler::rfdevice_rffe_coupler(
                                     rfdevice_physical_device* phy_obj_ptr , 
                                     rfc_phy_device_info_type* phy_device_info , 
                                     rfc_logical_device_info_type* logical_device_info,
                                     rfdevice_rffe_coupler_settings_type *coupler_settings_p,
                                     rfdevice_rffe_coupler_func_tbl_type *coupler_fn_ptrs_p
                                     )
{
  this->init_status = FALSE;
  memset(&(this->coupler_device_info), 0, sizeof(rfc_device_cfg_info_type));
  memset(&(this->coupler_settings), 0, sizeof(rfdevice_rffe_coupler_settings_type));
  memset(&(this->coupler_cfg), 0, sizeof(rfdevice_rffe_coupler_cfg_type));
  memset(&(this->coupler_fn_ptrs), 0, sizeof(rfdevice_rffe_coupler_func_tbl_type));

  /* initialize the coupler ptr */
  rfdevice_rffe_coupler::rfdevice_rffe_coupler_p = this;
  /* Update its physical device object ptr */
  rfdevice_rffe_coupler::rfdevice_phy_obj_ptr = phy_obj_ptr;

  if( 
      (phy_obj_ptr != NULL) && 
      (phy_device_info != NULL) && 
      (logical_device_info != NULL) && 
      (logical_device_info->rf_device_type == RFDEVICE_COUPLER)
    )
  {
    /* Copy over the config info */
    coupler_device_info.rf_device_id             =       logical_device_info->rf_device_id;
    coupler_device_info.rf_device_type           =       logical_device_info->rf_device_type;
    coupler_device_info.rf_asic_id               =       logical_device_info->rf_asic_id;
    coupler_device_info.rf_device_comm_protocol  =       phy_device_info->rf_device_comm_protocol;
    coupler_device_info.bus[0]                   =       phy_device_info->bus[0];
    coupler_device_info.manufacturer_id          =       phy_device_info->manufacturer_id;
    coupler_device_info.product_id               =       phy_device_info->product_id;
    coupler_device_info.product_rev              =       phy_device_info->product_rev;
    coupler_device_info.default_usid_range_start =       phy_device_info->default_usid_range_start;
    coupler_device_info.default_usid_range_end   =       phy_device_info->default_usid_range_end;
    coupler_device_info.assigned_usid            =       phy_device_info->assigned_usid;
    coupler_device_info.init_required            =       phy_device_info->init_required;
    coupler_device_info.associated_dac           =       phy_device_info->associated_dac;

    /* Populate COUPLER settings */
	memscpy(&this->coupler_settings, sizeof(rfdevice_rffe_coupler_settings_type), coupler_settings_p, sizeof(rfdevice_rffe_coupler_settings_type));

    /* Populate COUPLER funtion pointers */
	memscpy(&this->coupler_fn_ptrs, sizeof(rfdevice_rffe_coupler_func_tbl_type), coupler_fn_ptrs_p, sizeof(rfdevice_rffe_coupler_func_tbl_type));

    /* Init device header */
    this->mfg_id = (uint16)coupler_device_info.manufacturer_id;
    this->prd_id = (uint16)coupler_device_info.product_id;
    this->rev_id = (uint16)coupler_device_info.product_rev;
    this->rf_device_id = coupler_device_info.rf_device_id;
    this->rf_device_type = RFDEVICE_COUPLER;  
    this->instance_num  = logical_device_info->rf_asic_id;
    
    /* Invalidate all rfc config mapping */
    memset(&this->coupler_rfc_config_mapping.cdma, 0xFF, sizeof(rfdevice_rffe_coupler_rfc_cfg_type)*RFM_CDMA_MAX_BAND);
    memset(&this->coupler_rfc_config_mapping.gsm, 0xFF, sizeof(rfdevice_rffe_coupler_rfc_cfg_type)*RFCOM_NUM_GSM_BANDS);
    memset(&this->coupler_rfc_config_mapping.lte, 0xFF, sizeof(rfdevice_rffe_coupler_rfc_cfg_type)*RFCOM_NUM_LTE_BANDS);
    memset(&this->coupler_rfc_config_mapping.wcdma, 0xFF, sizeof(rfdevice_rffe_coupler_rfc_cfg_type)*RFCOM_NUM_WCDMA_BANDS);
    memset(&this->coupler_rfc_config_mapping.tdscdma, 0xFF, sizeof(rfdevice_rffe_coupler_rfc_cfg_type)*RFCOM_NUM_TDSCDMA_BANDS);

    /* Put the coupler in a known state at bootup */
    config_coupler_default();

    /* Tech independent init */
    this->init();
    this->init_status = TRUE;

  }
}




/*----------------------------------------------------------------------------*/
/*!
  @brief
  destructor
  
*/
rfdevice_rffe_coupler::~rfdevice_rffe_coupler()
{
 
}

/*----------------------------------------------------------------------------*/
 /*!
   @brief
   Initialize Coupler for operation
 
   @details
   This function can be used to initialize the coupler for operation
       
 */
void rfdevice_rffe_coupler::init()
{

  rfdevice_rffe_coupler_ag_settings_type ag_settings;

  rf_time_tick_type start_tick = rf_time_get_tick();
  
  /*==========================================================================*/
  /* VALIDITY CHECK  */
  /*==========================================================================*/
  if(this->rf_device_type != RFDEVICE_COUPLER)
  {
    RF_MSG(RF_ERROR, "COUPLER driver not constructed. Trigger failed. Check RFC");    
  }    

  /*==========================================================================*/
  /* CHOOSE SETTINGS */
  /*==========================================================================*/
  
  ag_settings = this->coupler_settings.common_settings_ag.coupler_init;
  
  /* No settings available - may not be a failure TODO */
  if(ag_settings.reg_tbl == NULL || ag_settings.data_tbl == NULL
	 || this->coupler_settings.common_settings_ag.coupler_init_script_size == 0)
  {
    RF_MSG_1(RF_HIGH, "COUPLER %d has no init settings", this->coupler_device_info.rf_device_id);    
  }
   
  /*==========================================================================*/
  /* PROCESS SCRIPT  */
  /*==========================================================================*/
  else if(!rfdevice_rffe_coupler_process_ag_table(rfdevice_rffe_coupler_p,
                                          NULL,
                                          ag_settings.reg_tbl[0],
                                          ag_settings.data_tbl[0],
                                          this->coupler_settings.common_settings_ag.coupler_init_script_size,
                                          RFDEVICE_EXECUTE_IMMEDIATE,
                                          0))

  {
    RF_MSG_1(RF_ERROR, "COUPLER %d init failed to process settings", this->coupler_device_info.rf_device_id);    
  } 
  
  /*==========================================================================*/
  /* TIME PROFILING  */
  /*==========================================================================*/
  RFDEVICE_LOG_MSG_2(LOG_RFA_DEVICE_TUNER_C, MSG_LEGACY_MED,"COUPLER %d init complete in %d us",
                                                this->coupler_device_info.rf_device_id,
					                                      rf_time_get_elapsed(start_tick,RF_USEC));


}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  interface function for coupler init

  @details
  API used to config the coupler for use

  @param rfc_data
  ASIC data capturing port info and default functionality provided by RFC

  @return
  boolean indicating pass or fail   
*/
boolean rfdevice_rffe_coupler::init 
(
  int32* rfc_data
)
{
  /* To Do - Implement port specific init */
  return TRUE;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Interface function for coupler trigger, to latch the pending settings.

  @details
  trigger script
  
  @param rfc_data
  ASIC data capturing port info and default functionality provided by RFC

  @param buff_obj_ptr
  void pointer to CCS buffer object

  @param dev_action
  indicates if it is immediate write to hw or build scripts  
    
  @param script_timing
  time offset for the script where a negative value refers to the time BEFORE the event

  @return
  boolean indicating pass or fail  
*/
boolean rfdevice_rffe_coupler::trigger
(
   int32* rfc_data,
   rf_buffer_intf *buff_obj_ptr, 
   rf_device_execution_type dev_action, 
   int16 script_timing
)
{
  boolean status = TRUE;

  /* Acquire lock before entering device critical section,so that the device call is thread safe */
  rf_common_enter_critical_section( &rfdevice_critical_sect );

  /* Get Trigger script or Trigger immediately */
  status = trigger_internal( rfc_data, buff_obj_ptr, dev_action, script_timing );

  /* Release lock for others to use this device. */
  rf_common_leave_critical_section( &rfdevice_critical_sect );

  return status;
}


/*----------------------------------------------------------------------------*/
/*!
  @brief
  placeholder

  @details
  placeholder

  @param buff_obj_ptr
  void pointer used to coupler's address of CCS buffer  

  @param dev_action
  indicates if it is immediate write to hw or build scripts  
*/
boolean rfdevice_rffe_coupler::wakeup
(
   int32* rfc_data,
   rf_buffer_intf *buff_obj_ptr, 
   rf_device_execution_type dev_action,
   int16 script_timing
)
{
  boolean status = TRUE;

  /* Acquire lock before entering device critical section,so that the device call is thread safe */
  rf_common_enter_critical_section( &rfdevice_critical_sect );

  /* Wakeup */
  status = wakeup_internal( rfc_data, buff_obj_ptr, dev_action, script_timing );

  /* Release lock for others to use this device. */
  rf_common_leave_critical_section( &rfdevice_critical_sect );

  return status;
}


/*----------------------------------------------------------------------------*/
/*!
  @brief
  placeholder

  @details
  placeholder

  @param buff_obj_ptr
  void pointer used to coupler's address of CCS buffer  

  @param dev_action
  indicates if it is immediate write to hw or build scripts  
*/
boolean rfdevice_rffe_coupler::sleep
(
   int32* rfc_data,
   rf_buffer_intf *buff_obj_ptr, 
   rf_device_execution_type dev_action,
   int16 script_timing
)
{
  boolean status = TRUE;

  /* Acquire lock before entering device critical section,so that the device call is thread safe */
  rf_common_enter_critical_section( &rfdevice_critical_sect );

  /* Sleep */
  status = sleep_internal( rfc_data, buff_obj_ptr, dev_action, script_timing );

  /* Release lock for others to use this device. */
  rf_common_leave_critical_section( &rfdevice_critical_sect );

  return status;
}


/*----------------------------------------------------------------------------*/
/*!
  @brief
  Configure COUPLER for specific mode of operation

  @details
  This function is used to configure the coupler for the specific coupler
  direction and corresponding band of operation
      
  @param tech_cfg
  Structure containing tech specific configuration for coupler

  @param buff_obj_ptr
  void pointer to CCS buffer object

  @param dev_action
  indicates if it is immediate write to hw or build scripts    

  @script_timing
  timing in us

*/
boolean rfdevice_rffe_coupler::config_coupler
(
  rfdevice_coupler_tech_cfg_type tech_cfg,
  rf_buffer_intf *buff_obj_ptr, 
  rf_device_execution_type dev_action, 
  int16 script_timing
)
{
  boolean status = TRUE;

  /* Acquire lock before entering device critical section,so that the device call is thread safe */
  rf_common_enter_critical_section( &rfdevice_critical_sect );

  /* Configure the Coupler */
  status = config_coupler_internal( tech_cfg, buff_obj_ptr, dev_action, script_timing );

  /* Release lock for others to use this device. */
  rf_common_leave_critical_section( &rfdevice_critical_sect );

  return status;
}


/*----------------------------------------------------------------------------*/
/*!
  @brief
  Configure COUPLER for specific gain

  @details
  This function is used to configure the coupler for the specific gain
      
  @param tech_cfg
  Structure containing tech specific configuration for coupler

  @param buff_obj_ptr
  void pointer to CCS buffer object

  @param dev_action
  indicates if it is immediate write to hw or build scripts    

  @script_timing
  timing in us

*/
boolean rfdevice_rffe_coupler::config_coupler_gain
(
  rfdevice_coupler_tech_cfg_type tech_cfg,
  rf_buffer_intf *buff_obj_ptr, 
  rf_device_execution_type dev_action, 
  int16 script_timing
)
{
  boolean status = TRUE;

#ifndef FEATURE_RF_HAS_TP_CARDS
  /* Acquire lock before entering device critical section,so that the device call is thread safe */
  rf_common_enter_critical_section( &rfdevice_critical_sect );

  /* Configure the Coupler */
  status = config_coupler_gain_internal( tech_cfg, buff_obj_ptr, dev_action, script_timing );

  /* Release lock for others to use this device. */
  rf_common_leave_critical_section( &rfdevice_critical_sect );
#endif

  return status;
}

/*!
  @brief
  interface function to configure the coupler in default FWD state

  @param void
  
  @return
  boolean indicating pass or fail  
*/	
boolean rfdevice_rffe_coupler::config_coupler_default()
{
  rfdevice_coupler_tech_cfg_type tech_cfg;
  boolean status = TRUE;
  /*==========================================================================*/
  /* VALIDITY CHECK  */
  /*==========================================================================*/
  if(this->rf_device_type != RFDEVICE_COUPLER)
  {
    RF_MSG(RF_ERROR, "COUPLER driver not constructed. Config coupler failed. Check RFC");    
    return FALSE;
  }  

  /*==========================================================================*/
  /* CHOOSE SETTINGS */
  /*==========================================================================*/
  tech_cfg.mode = RFCOM_LTE_MODE;
  tech_cfg.band.lte_band = RFCOM_BAND_LTE_B1;
  tech_cfg.direction = RFDEVICE_COUPLER_DIRECTION_FWD;
  tech_cfg.gain = RFDEVICE_COUPLER_GAIN_0;
  tech_cfg.output_type = RFDEVICE_COUPLER_OUTPUT_TYPE_0;

  /*==========================================================================*/
  /* CONFIG COUPLER */
  /*==========================================================================*/
  if(!config_coupler(tech_cfg,
                     NULL,
                     RFDEVICE_EXECUTE_IMMEDIATE,
                     0))
  {
    return FALSE;
  }

  return status;
}


/*----------------------------------------------------------------------------*/
/*!
  @brief
  Store band specific configuration 

  @details
  Band specific coupler direction and attenuation  information which is pushed from RFC 
  is stored in the device using this API

  @param mode
  enum indicating the radio access technology

  @param band
  enum indicating the band number

  @param data_ptr
  void pointer to pass or receive desired information   
*/
boolean rfdevice_rffe_coupler::set_band_config_map
(
  rfcom_mode_enum_type mode,
  rfcom_band_type_u band,
  int32 *data_ptr
)
{

  boolean status = TRUE;

  /* Acquire lock before entering device critical section,so that the device call is thread safe */
  rf_common_enter_critical_section( &rfdevice_critical_sect );

  /* Configure the band map */
  status = set_band_config_map_internal( mode, band, data_ptr );

  /* Release lock for others to use this device. */
  rf_common_leave_critical_section( &rfdevice_critical_sect );

  return status;
}


