/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

         COUPLER INTERNAL

GENERAL DESCRIPTION
  Helper functions for the COUPLER

EXTERNALIZED FUNCTIONS


  Copyright (c) 2015 - 2016 by QUALCOMM TECHNOLOGIES INCORPORATED.
  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rfdevice_coupler.mpss/1.22/src/rfdevice_rffe_coupler_internal.cpp#1 $
$DateTime: 2021/03/09 03:37:09 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
07/21/16   kpa     Moved the Coupler Config get data messages under tuner log mask
12/04/15   px      Changed append_rffe to use new time reference
07/23/15   px      Added generic config coupler api
07/14/15   px      Initial version

===========================================================================*/


/*===========================================================================

                         INCLUDE FILES FOR MODULE

===========================================================================*/
#include "rfcommon_msg.h"
#include "rfdevice_rffe_coupler.h"
#include "stringl.h" /* for memscpy */
#include "rfdevice_msg_log.h"
#include "rf_hal_buffer.h"
#include "rf_hal_rffe.h"
#include "rfcommon_time_profile.h"

extern "C"
{
#include "fs_lib.h"
#include "fs_public.h"
}

#ifdef FEATURE_RF_HAS_TP_CARDS
#error code not present
#else
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
boolean rfdevice_rffe_coupler::config_coupler_internal
(
  rfdevice_coupler_tech_cfg_type tech_cfg,
  rf_buffer_intf *buff_obj_ptr, 
  rf_device_execution_type dev_action, 
  int16 script_timing
)
{
  rfdevice_rffe_coupler_ag_settings_type ag_settings;
  rfdevice_rffe_coupler_rfc_cfg_type* rfc_cfg_ptr = NULL;
  rf_time_tick_type start_tick = rf_time_get_tick();

  ag_settings.data_tbl=NULL;
  ag_settings.reg_tbl=NULL;

  /*==========================================================================*/
  /* VALIDITY CHECK  */
  /*==========================================================================*/
  if(this->rf_device_type != RFDEVICE_COUPLER)
  {
    RF_MSG(RF_ERROR, "COUPLER driver not constructed. Config coupler failed. Check RFC");    
    return FALSE;
  }  
  if(!rfdevice_rffe_coupler_validate( buff_obj_ptr, dev_action, script_timing))
  {
    RF_MSG_1(RF_ERROR, "COUPLER %d config coupler: Invalid params", this->coupler_device_info.rf_device_id);
    return FALSE;
  }

  /*==========================================================================*/
  /* GET PORT FROM RFC */
  /*==========================================================================*/
  rfdevice_rffe_coupler_get_band_config_map(rfdevice_rffe_coupler_p, tech_cfg.mode, tech_cfg.band, &rfc_cfg_ptr);

  if(rfc_cfg_ptr->port <= this->coupler_settings.num_ports)
  {
    tech_cfg.port = rfc_cfg_ptr->port;
  }
  else
  {
    RF_MSG_1( RF_ERROR, "COUPLER %d config_coupler: port not set, defaulting to port 0",this->coupler_device_info.rf_device_id);
    tech_cfg.port = 0;
  }
 
  /*==========================================================================*/
  /* CHOOSE AG SETTINGS */
  /*==========================================================================*/
  if (rfdevice_rffe_coupler_p->coupler_fn_ptrs.get_config_data != NULL)
  {
    /* Update coupler configuration depending on mode, band, and direction */
    if(!rfdevice_rffe_coupler_update_cfg(rfdevice_rffe_coupler_p, tech_cfg.mode, tech_cfg.band, tech_cfg.direction, 
                                         RFDEVICE_COUPLER_GAIN_INVALID, RFDEVICE_COUPLER_OUTPUT_TYPE_INVALID))
    {
      RF_MSG( RF_ERROR, "config_coupler: COUPLER update config failed");
      return FALSE;
    }

    /* Get port settings from driver ag files */
    // DIRECTION
    if(!rfdevice_rffe_coupler_p->coupler_fn_ptrs.get_config_data(&ag_settings, rfdevice_rffe_coupler_p->coupler_cfg))
    {
      RFDEVICE_LOG_MSG_1(LOG_RFA_DEVICE_TUNER_C, MSG_LEGACY_MED,
                         "COUPLER %d config_coupler: get config data failed",
                         this->coupler_device_info.rf_device_id);
      return FALSE;
    }
  }
  else
  {
    /* Return True as few Couplers have not been enabled yet 
       --TODO - return FALSE when Coupler functionality is stabilized */
    RF_MSG_1( RF_ERROR, "COUPLER %d config_coupler: get config data NULL",this->coupler_device_info.rf_device_id);
    return TRUE;
  }
  
  /*==========================================================================*/
  /* PROCESS SCRIPT  */
  /*==========================================================================*/
  if(ag_settings.reg_tbl == NULL || ag_settings.data_tbl == NULL || ag_settings.script_size == 0)
  {
        RF_MSG_4( RF_ERROR, "COUPLER %d does not have AG settings, data_tbl:%d, reg_tbl: %d, script_size: %d",
            this->coupler_device_info.rf_device_id,
            ag_settings.data_tbl,
            ag_settings.reg_tbl,
            ag_settings.script_size);
    return FALSE;
  }
  else if(!rfdevice_rffe_coupler_process_ag_table(rfdevice_rffe_coupler_p,
                                          buff_obj_ptr,
                                          ag_settings.reg_tbl[tech_cfg.port],
                                          ag_settings.data_tbl[tech_cfg.port],
                                          ag_settings.script_size,
                                          dev_action,
                                          script_timing,
                                          tech_cfg.port))
  {
    RF_MSG_1(RF_ERROR, "COUPLER %d config coupler failed to process settings", this->coupler_device_info.rf_device_id);
    return FALSE;
  }
  
  /*==========================================================================*/ 
  /* Append Trigger for RFDEVICE_EXECUTE_IMMEDIATE */ 
  /*==========================================================================*/ 
  if (RFDEVICE_EXECUTE_IMMEDIATE == dev_action) 
  { 
    (void)trigger_internal(NULL, buff_obj_ptr, dev_action, script_timing); 
  } 

  /*==========================================================================*/
  /* TIME PROFILING  */
  /*==========================================================================*/
  RFDEVICE_LOG_MSG_5(LOG_RFA_DEVICE_TUNER_C, MSG_LEGACY_MED,
          					"COUPLER %d config coupler for port %d script %d timing %d complete in %d us",
          					this->coupler_device_info.rf_device_id,
          					tech_cfg.port, dev_action, script_timing,
          					rf_time_get_elapsed(start_tick,RF_USEC));

  return TRUE;
}
#endif

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Configure COUPLER for specific gain

  @details
  This function is used to configure the coupler for specific gain
      
  @param tech_cfg
  Structure containing tech specific configuration for coupler

  @param buff_obj_ptr
  void pointer to CCS buffer object

  @param dev_action
  indicates if it is immediate write to hw or build scripts    

  @script_timing
  timing in us

*/
boolean rfdevice_rffe_coupler::config_coupler_gain_internal
(
  rfdevice_coupler_tech_cfg_type tech_cfg,
  rf_buffer_intf *buff_obj_ptr, 
  rf_device_execution_type dev_action, 
  int16 script_timing
)
{
  rfdevice_rffe_coupler_ag_settings_type ag_settings;

  rf_time_tick_type start_tick = rf_time_get_tick();

  ag_settings.data_tbl=NULL;
  ag_settings.reg_tbl=NULL;

  /*==========================================================================*/
  /* VALIDITY CHECK  */
  /*==========================================================================*/
 
  if(!rfdevice_rffe_coupler_validate( buff_obj_ptr, dev_action, script_timing))
  {
    RF_MSG_1(RF_ERROR, "COUPLER %d config coupler gain: Invalid params", this->coupler_device_info.rf_device_id);
    return FALSE;
  }
 
  /*==========================================================================*/
  /* CHOOSE SETTINGS */
  /*==========================================================================*/

  if(rfdevice_rffe_coupler_p->coupler_fn_ptrs.get_config_data != NULL)
  {
    /* Update coupler configuration depending on mode, band, direction, gain, and output type */
    if(!rfdevice_rffe_coupler_update_cfg(rfdevice_rffe_coupler_p, tech_cfg.mode, tech_cfg.band, tech_cfg.direction, tech_cfg.gain, tech_cfg.output_type))
    {
      RF_MSG( RF_ERROR, "config_coupler_gain: COUPLER update config failed");
      return FALSE;
    }

    /* With updated coupler configuration get port settings from driver ag files*/
    if(!rfdevice_rffe_coupler_p->coupler_fn_ptrs.get_gain_data(&ag_settings, rfdevice_rffe_coupler_p->coupler_cfg))
    {
      RF_MSG_1( RF_ERROR, "COUPLER %d config_coupler_gain: get config data failed",this->coupler_device_info.rf_device_id);
      return FALSE;
    }
  }
  else
  {
    /* Return True as few Couplers have not been enabled yet 
       --TODO - return FALSE when Coupler functionality is stabilized */
    RF_MSG_1( RF_ERROR, "COUPLER %d config_coupler_gain: get config data NULL",this->coupler_device_info.rf_device_id);
    return TRUE;
  }
  
  /*==========================================================================*/
  /* PROCESS SCRIPT  */
  /*==========================================================================*/
  if(ag_settings.reg_tbl == NULL || ag_settings.data_tbl == NULL
      || ag_settings.script_size == 0)
  {
        RF_MSG_4( RF_ERROR, "COUPLER %d does not have AG settings, data_tbl:%d, reg_tbl: %d, script_size: %d",
            this->coupler_device_info.rf_device_id,
            ag_settings.data_tbl,
            ag_settings.reg_tbl,
            ag_settings.script_size);
    return FALSE;
  }
  else if(!rfdevice_rffe_coupler_process_ag_table(rfdevice_rffe_coupler_p,
                                          buff_obj_ptr,
                                          ag_settings.reg_tbl[tech_cfg.port],
                                          ag_settings.data_tbl[tech_cfg.port],
                                          ag_settings.script_size,
                                          dev_action,
                                          script_timing,
                                          tech_cfg.port))
  {
    RF_MSG_1(RF_ERROR, "COUPLER %d config coupler gain failed to process settings", this->coupler_device_info.rf_device_id);
    return FALSE;
  }
  
 

  /*==========================================================================*/
  /* TIME PROFILING  */
  /*==========================================================================*/
  RFDEVICE_LOG_MSG_5(LOG_RFA_DEVICE_TUNER_C, MSG_LEGACY_MED,
          					"COUPLER %d config coupler gain for port %d script %d timing %d complete in %d us",
          					this->coupler_device_info.rf_device_id,
          					tech_cfg.port, dev_action, script_timing,
          					rf_time_get_elapsed(start_tick,RF_USEC));

  return TRUE;
}







/*----------------------------------------------------------------------------*/
/*!
  @brief
  Store band specific configuration 

  @details
  Band specific coupler direction  information which is pushed from RFC 
  is stored in the device using this API

  @param mode
  enum indicating the radio access technology

  @param band
  enum indicating the band number

  @param data_ptr
  void pointer to pass or receive desired information   
*/
boolean rfdevice_rffe_coupler::set_band_config_map_internal
(
  rfcom_mode_enum_type mode,
  rfcom_band_type_u band,
  int32 *data_ptr
)
{
  rfdevice_rffe_coupler_rfc_cfg_type rfc_cfg = {RFDEVICE_COUPLER_GAIN_INVALID, RFDEVICE_COUPLER_OUTPUT_TYPE_INVALID, RFDEVICE_COUPLER_DIRECTION_INVALID, 0};
  boolean status = TRUE;
  
  /*==========================================================================*/
  /* VALIDITY CHECK  */
  /*==========================================================================*/
  if(this->rf_device_type != RFDEVICE_COUPLER)
  {
    RF_MSG(RF_ERROR, "COUPLER driver not constructed. set_band_config_map failed. Check RFC");    
    return FALSE;
  }

  if (data_ptr == NULL)
  {
    RF_MSG_3(RF_ERROR, "COUPLER %d set_band_config_map invalid param for mode %d, band %d",
		                this->coupler_device_info.rf_device_id, mode, band.lte_band);    
    return FALSE;
  }


  /* Extract default direction, gain, and output type from RFC data */
  rfc_cfg.port = (uint8)RFDEVICE_COUPLER_PORT_NUM_GET(data_ptr[RFDEVICE_COUPLER_RFC_PORT_NUM_INDEX]);
  rfc_cfg.default_gain = (rfdevice_coupling_gain_type)data_ptr[RFDEVICE_COUPLER_RFC_DEFAULT_GAIN_INDEX];
  rfc_cfg.default_output_type = (rfdevice_coupler_output_type)data_ptr[RFDEVICE_COUPLER_RFC_DEFAULT_OUTPUT_TYPE_INDEX];
  rfc_cfg.default_direction = (rfdevice_coupling_direction_type)data_ptr[RFDEVICE_COUPLER_RFC_DEFAULT_FWD_DIRECTION_INDEX];

  /*==========================================================================*/
  /* SAVE BAND CONFIG  */
  /*==========================================================================*/
  switch (mode)
  {
    case RFCOM_LTE_MODE:
      if (band.lte_band < RFCOM_NUM_LTE_BANDS)
      {
        rfdevice_rffe_coupler_p->coupler_rfc_config_mapping.lte[band.lte_band] = rfc_cfg;
      }
      else
      {
        RF_MSG_1(RF_ERROR, "COUPLER rfdevice_rffe_coupler_set_band_config_map invalid LTE band %d", band.lte_band); 
        status = FALSE;
      }
      break;
    case RFCOM_WCDMA_MODE:
      if(band.wcdma_band < RFCOM_NUM_WCDMA_BANDS)
      {
        this->coupler_rfc_config_mapping.wcdma[band.wcdma_band] = rfc_cfg;
      }
      else
      {
        RF_MSG_1(RF_ERROR, "COUPLER rfdevice_rffe_coupler_set_band_config_map invalid WCDMA band %d", band.wcdma_band);    
        status = FALSE;
      }
      break;
    case RFCOM_1X_MODE:
    case RFCOM_1XEVDO_MODE:
      if (band.cdma_band < RFM_CDMA_MAX_BAND)
      {
        this->coupler_rfc_config_mapping.cdma[band.cdma_band] = rfc_cfg;
      }
      else
      {
        RF_MSG_1(RF_ERROR, "COUPLER rfdevice_rffe_coupler_set_band_config_map invalid CDMA band %d", band.cdma_band);  
        status = FALSE;
      }
      break;
    case RFCOM_GSM_MODE:
      if (band.gsm_band < RFCOM_NUM_GSM_BANDS)
      {
        this->coupler_rfc_config_mapping.gsm[band.gsm_band] = rfc_cfg;
      }
      else
      {
        RF_MSG_1(RF_ERROR, "COUPLER rfdevice_rffe_coupler_set_band_config_map invalid GSM band %d", band.gsm_band); 
        status = FALSE;
      }
      break;      
    case RFCOM_TDSCDMA_MODE:
      if (band.tdscdma_band < RFCOM_NUM_TDSCDMA_BANDS)
      {
        rfdevice_rffe_coupler_p->coupler_rfc_config_mapping.tdscdma[band.tdscdma_band] = rfc_cfg;      
      }
      else
      {
        RF_MSG_1(RF_ERROR, "COUPLER rfdevice_rffe_coupler_set_band_config_map invalid TDSCDMA band %d", band.tdscdma_band);    
        status = FALSE;
      }
      break;
    default:
      RF_MSG_1(RF_ERROR, "COUPLER rfdevice_rffe_coupler_set_band_config_map invalid tech %d", mode);
      status = FALSE;
  }

  return status; 
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
boolean rfdevice_rffe_coupler::trigger_internal
(
   int32* rfc_data,
   rf_buffer_intf *buff_obj_ptr, 
   rf_device_execution_type dev_action, 
   int16 script_timing
)
{
  rfdevice_rffe_coupler_ag_settings_type ag_settings;
  rf_time_tick_type start_tick = rf_time_get_tick();
  
  /*==========================================================================*/
  /* VALIDITY CHECK  */
  /*==========================================================================*/
  if(this->rf_device_type != RFDEVICE_COUPLER)
  {
    RF_MSG(RF_ERROR, "COUPLER driver not constructed. Trigger failed. Check RFC");    
    return FALSE;
  }    

  if(!rfdevice_rffe_coupler_validate(buff_obj_ptr, dev_action, script_timing))
  {
    RF_MSG_1(RF_ERROR, "COUPLER %d trigger: Invalid params", this->coupler_device_info.rf_device_id);    
    return FALSE;
  }
  /*==========================================================================*/
  /* CHOOSE SETTINGS */
  /*==========================================================================*/
  
  ag_settings = this->coupler_settings.common_settings_ag.coupler_trigger;
  
  /* No settings available - may not be a failure TODO */
  if(ag_settings.reg_tbl == NULL || ag_settings.data_tbl == NULL
	 || this->coupler_settings.common_settings_ag.coupler_trigger_script_size == 0)
  {
    RF_MSG_1(RF_HIGH, "COUPLER %d has no trigger settings", this->coupler_device_info.rf_device_id);    
  }
   
  /*==========================================================================*/
  /* PROCESS SCRIPT  */
  /*==========================================================================*/
  else if(!rfdevice_rffe_coupler_process_ag_table(rfdevice_rffe_coupler_p,
                                          buff_obj_ptr,
                                          ag_settings.reg_tbl[0],
                                          ag_settings.data_tbl[0],
                                          this->coupler_settings.common_settings_ag.coupler_trigger_script_size,
                                          dev_action,
                                          script_timing))

  {
    RF_MSG_1(RF_ERROR, "COUPLER %d trigger failed to process settings", this->coupler_device_info.rf_device_id);    
    return FALSE;
  }  
  /*==========================================================================*/
  /* TIME PROFILING  */
  /*==========================================================================*/
  RFDEVICE_LOG_MSG_4(LOG_RFA_DEVICE_TUNER_C, MSG_LEGACY_MED,"COUPLER %d trigger script %d timing %d complete in %d us",
                                                this->coupler_device_info.rf_device_id, dev_action, script_timing,
					                                      rf_time_get_elapsed(start_tick,RF_USEC));

  return TRUE;

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
boolean rfdevice_rffe_coupler::wakeup_internal
(
   int32* rfc_data,
   rf_buffer_intf *buff_obj_ptr, 
   rf_device_execution_type dev_action,
   int16 script_timing
)
{
   rfdevice_rffe_coupler_ag_settings_type ag_settings;
  rf_time_tick_type start_tick = rf_time_get_tick();
  
  /*==========================================================================*/
  /* VALIDITY CHECK  */
  /*==========================================================================*/
  if(this->rf_device_type != RFDEVICE_COUPLER)
  {
    RF_MSG(RF_ERROR, "COUPLER driver not constructed. Wakeup failed. Check RFC");    
    return FALSE;
  }    

  if(!rfdevice_rffe_coupler_validate(buff_obj_ptr, dev_action, script_timing))
  {
    RF_MSG_1(RF_ERROR, "COUPLER %d wakeup: Invalid params", this->coupler_device_info.rf_device_id);    
    return FALSE;
  }
  /*==========================================================================*/
  /* CHOOSE SETTINGS */
  /*==========================================================================*/
  
  ag_settings = this->coupler_settings.common_settings_ag.coupler_wakeup;
  
  /* No settings available - may not be a failure TODO */
  if(ag_settings.reg_tbl == NULL || ag_settings.data_tbl == NULL
	 || this->coupler_settings.common_settings_ag.coupler_wakeup_script_size == 0)
  {
    RF_MSG_1(RF_HIGH, "COUPLER %d has no wakeup settings", this->coupler_device_info.rf_device_id);    
  }
   
  /*==========================================================================*/
  /* PROCESS SCRIPT  */
  /*==========================================================================*/
  else if(!rfdevice_rffe_coupler_process_ag_table(rfdevice_rffe_coupler_p,
                                          buff_obj_ptr,
                                          ag_settings.reg_tbl[0],
                                          ag_settings.data_tbl[0],
                                          this->coupler_settings.common_settings_ag.coupler_wakeup_script_size,
                                          dev_action,
                                          script_timing))

  {
    RF_MSG_1(RF_ERROR, "COUPLER %d wakeup failed to process settings", this->coupler_device_info.rf_device_id);    
    return FALSE;
  }  
  /*==========================================================================*/
  /* TIME PROFILING  */
  /*==========================================================================*/
  RFDEVICE_LOG_MSG_4(LOG_RFA_DEVICE_TUNER_C, MSG_LEGACY_MED,"COUPLER %d wakeup script %d timing %d complete in %d us",
                                                this->coupler_device_info.rf_device_id, dev_action, script_timing,
					                                      rf_time_get_elapsed(start_tick,RF_USEC));

  return TRUE;

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
boolean rfdevice_rffe_coupler::sleep_internal
(
   int32* rfc_data,
   rf_buffer_intf *buff_obj_ptr, 
   rf_device_execution_type dev_action,
   int16 script_timing
)
{
  rfdevice_rffe_coupler_ag_settings_type ag_settings;
 rf_time_tick_type start_tick = rf_time_get_tick();

 /*==========================================================================*/
 /* VALIDITY CHECK  */
 /*==========================================================================*/
 if(this->rf_device_type != RFDEVICE_COUPLER)
 {
   RF_MSG(RF_ERROR, "COUPLER driver not constructed. sleep failed. Check RFC");    
   return FALSE;
 }    

 if(!rfdevice_rffe_coupler_validate(buff_obj_ptr, dev_action, script_timing))
 {
   RF_MSG_1(RF_ERROR, "COUPLER %d sleep: Invalid params", this->coupler_device_info.rf_device_id);    
   return FALSE;
 }
 /*==========================================================================*/
 /* CHOOSE SETTINGS */
 /*==========================================================================*/

 ag_settings = this->coupler_settings.common_settings_ag.coupler_sleep;

 /* No settings available - may not be a failure TODO */
 if(ag_settings.reg_tbl == NULL || ag_settings.data_tbl == NULL
  || this->coupler_settings.common_settings_ag.coupler_sleep_script_size == 0)
 {
   RF_MSG_1(RF_HIGH, "COUPLER %d has no sleep settings", this->coupler_device_info.rf_device_id);    
 }

 /*==========================================================================*/
 /* PROCESS SCRIPT  */
 /*==========================================================================*/
 else if(!rfdevice_rffe_coupler_process_ag_table(rfdevice_rffe_coupler_p,
                                         buff_obj_ptr,
                                         ag_settings.reg_tbl[0],
                                         ag_settings.data_tbl[0],
                                         this->coupler_settings.common_settings_ag.coupler_sleep_script_size,
                                         dev_action,
                                         script_timing))

 {
   RF_MSG_1(RF_ERROR, "COUPLER %d sleep failed to process settings", this->coupler_device_info.rf_device_id);    
   return FALSE;
 }  
 /*==========================================================================*/
 /* TIME PROFILING  */
 /*==========================================================================*/
 RFDEVICE_LOG_MSG_4(LOG_RFA_DEVICE_TUNER_C, MSG_LEGACY_MED,"COUPLER %d sleep script %d timing %d complete in %d us",
                                               this->coupler_device_info.rf_device_id, dev_action, script_timing,
                                               rf_time_get_elapsed(start_tick,RF_USEC));

 return TRUE;

}


/*!
  @brief
  The function is used to process the settings script table and do the following operation

  1. Do Immediate write if the dev action is RFDEVICE_EXECUTE_IMMEDIATE
  2. Update the CCS buffer object  if the dev action is RFDEVICE_CREATE_SCRIPT
  
  @details
  rfdevice_rffe_coupler_process_ag_table()

  @param coupler_device
  pointer to coupler device

  @param buff_obj_ptr
  void pointer to CCS buffer object
  
  @param reg_table
  pointer to register address array

  @param data_table
  pointer to register data array

  @param write_size
  Size of the settings script table

  @param dev_action
  indicates if it is immediate write to hw or build scripts    

  @script_timing
  timing in us

  @return
  boolean indicating pass or fail
*/
boolean rfdevice_rffe_coupler::rfdevice_rffe_coupler_process_ag_table
(
  rfdevice_rffe_coupler* coupler_device,
  rf_buffer_intf *buff_obj_ptr,
  const uint16* reg_table,
  const int16* data_table,
  uint8 write_size,
  rf_device_execution_type dev_action,
  int16 script_timing,
  uint8 port_index
)
{
  boolean status = TRUE;
  uint8 counter = 0;
  uint8 write_count = 0;

  /*==========================================================================*/
  /* VALIDITY  */
  /*==========================================================================*/
  if(coupler_device == NULL)
  {
    RF_MSG( RF_ERROR, "process_script: Invalid COUPLER device pointer");
    return FALSE;
  }
  /* CHeck if data was provided */
  if(reg_table == NULL || data_table == NULL)
  {
    RF_MSG( RF_ERROR, "process_script: Input tables missing");
    return FALSE;
  }

  /* Check if there are any entries to write */
  if(write_size == 0)
  {
    RF_MSG( RF_ERROR, "process_script failed: No entries to write");
    return FALSE; 
  }
  /* Check if the provided buffer is valid */
  if( dev_action == RFDEVICE_CREATE_SCRIPT && buff_obj_ptr == NULL)
  {
    RF_MSG( RF_ERROR, "process_script failed: Script not allocated");
    return FALSE; 
  }
  /* If script wasnt provided, create one */
  else if(dev_action == RFDEVICE_EXECUTE_IMMEDIATE)
  {
     buff_obj_ptr = rf_buffer_create(0, COUPLER_MAX_SCRIPT_SIZE, 0);
      
     if(buff_obj_ptr == NULL)
     {
       RF_MSG_1(RF_ERROR,"coupler_process_script failed: Unable to get memory for %d entries",COUPLER_MAX_SCRIPT_SIZE);
       return FALSE;
     }    
  }
  
  /*==========================================================================*/
  /* PROCESSING  */
  /*==========================================================================*/
  for(counter = 0; counter < write_size && status; counter++)
  {
    if((int16)data_table[counter] != RFDEVICE_COUPLER_REG_INVALID)
    {
      write_count++;


        status &= buff_obj_ptr->append_rffe( (uint8)coupler_device->coupler_device_info.assigned_usid,
                                        (uint8)coupler_device->coupler_device_info.bus[0],
                                        (uint16)reg_table[counter],
                                        (uint8)data_table[counter],
                                        RF_BUFFER_WRITE,
                                        script_timing,
                                        coupler_device->coupler_settings.coupler_rfcom_bus_cfg.extended_cmd,
                                        coupler_device->coupler_settings.coupler_rfcom_bus_cfg.settings.half_rate,
                                        coupler_device->coupler_settings.coupler_rfcom_bus_cfg.settings.rd_delay,
                                        TRUE);
 
      
    }
  }
  
  /* Execute immediately and process reads */
  if(status && dev_action == RFDEVICE_EXECUTE_IMMEDIATE)
  {
    
    if( RF_HAL_BUS_SUCCESS != rfhal_rffe_execute_rf_buf(buff_obj_ptr, 
                                                        NULL, 
                                                        0, 
                                                        &(coupler_device->coupler_settings.coupler_rfcom_bus_cfg.settings),
                                                        0))
      status = FALSE;
  }
  /* Log # actual transactions */
  
  RFDEVICE_LOG_MSG_3(LOG_RFA_DEVICE_TUNER_C, MSG_LEGACY_MED,"COUPLER %d driver generated %d writes. Buffer contains %d writes total",
                                                  coupler_device->coupler_device_info.rf_device_id, write_count,
                                                  buff_obj_ptr->get_num_trans(RF_BUFFER_RFFE));
  /*==========================================================================*/
  /* CLEAN UP  */
  /*==========================================================================*/

  /* Get rid of the buffer we created */
  if(dev_action == RFDEVICE_EXECUTE_IMMEDIATE)
  {
     rf_buffer_delete(buff_obj_ptr);        
  }

  return status;
}


/*----------------------------------------------------------------------------*/
/*!
  @brief
  The function is used to validate all the necessary paramters and the depedencies need to
  read/write the Qcoupler.
  
  @details
  rfdevice_rffe_coupler_validate()

  @param buff_obj_ptr
  void pointer to CCS buffer object

  @param dev_action
  indicates if it is immediate write to hw or build scripts    

  @script_timing
  timing in us

  @return
  boolean indicating pass or fail
*/

boolean rfdevice_rffe_coupler::rfdevice_rffe_coupler_validate
(
  rf_buffer_intf *buff_obj_ptr, 
  rf_device_execution_type dev_action,
  int16 script_timing   
)
{ 
  boolean status = TRUE;

  /* Dev action check */
  if( dev_action != RFDEVICE_CREATE_SCRIPT && dev_action != RFDEVICE_EXECUTE_IMMEDIATE)
  {
    RF_MSG( RF_ERROR, "Invalid Script for COUPLER request");
    return FALSE; 
  }
  
  /* Null pointer check */
  if( dev_action == RFDEVICE_CREATE_SCRIPT && buff_obj_ptr == NULL)
  {
    RF_MSG( RF_ERROR, "Script not allocated for COUPLER request");
    return FALSE; 
  }

  return status;
}


/*!
  @brief
  The function is used to update direction config of coupler
  
  @details
  rfdevice_rffe_coupler_update_cfg()

  @param coupler_device
  pointer to coupler device

  @param mode
  enum indicating the radio access technology

  @param band
  enum indicating the band number

  @param direction
  Enum indicating the required state of the coupler

  @param gain
  enum indicating the gain (attenuation)

  @param output_type
  enum indicating the output type

  @return
  boolean indicating pass or fail
*/
boolean rfdevice_rffe_coupler::rfdevice_rffe_coupler_update_cfg
(
  rfdevice_rffe_coupler* coupler_device,
  rfcom_mode_enum_type mode,
  rfcom_band_type_u band,
  rfdevice_coupling_direction_type direction,
  rfdevice_coupling_gain_type gain,
  rfdevice_coupler_output_type output_type
)
{
  /* Update Direction, mode and band */
  coupler_device->coupler_cfg.direction = direction;
  coupler_device->coupler_cfg.mode = mode;
  coupler_device->coupler_cfg.band = band;
  coupler_device->coupler_cfg.gain = gain;
  coupler_device->coupler_cfg.output_type = output_type;

  return TRUE;
}


/*----------------------------------------------------------------------------*/
/*!
  @brief
  The function returns the pointer to tech specific coupler rfc config.

  @details
  rfdevice_rffe_coupler_get_band_config_map()

  @param coupler_device
  pointer to coupler device

  @param mode
  enum indicating the radio access technology

  @param band
  enum indicating the band number

  @param rfc_cfg
  pointer to coupler rfc config type

  @return
  boolean indicating pass or fail
*/
boolean rfdevice_rffe_coupler::rfdevice_rffe_coupler_get_band_config_map
(
  rfdevice_rffe_coupler* coupler_device,
  rfcom_mode_enum_type mode,
  rfcom_band_type_u band,
  rfdevice_rffe_coupler_rfc_cfg_type **rfc_cfg
)
{
  boolean status = TRUE;

  switch (mode)
  {
    case RFCOM_LTE_MODE:
      if (band.lte_band < RFCOM_NUM_LTE_BANDS)
      {
        *rfc_cfg = &(coupler_device->coupler_rfc_config_mapping.lte[band.lte_band]);
      }
      else
      {
        RF_MSG_1(RF_ERROR, "COUPLER rfdevice_rffe_coupler_get_band_config_map invalid LTE band %d", band.lte_band); 
        status = FALSE;
      }
      break;
    case RFCOM_WCDMA_MODE:
      if(band.wcdma_band < RFCOM_NUM_WCDMA_BANDS)
      {
        *rfc_cfg = &(coupler_device->coupler_rfc_config_mapping.wcdma[band.wcdma_band]);
      }
      else
      {
        RF_MSG_1(RF_ERROR, "COUPLER rfdevice_rffe_coupler_get_band_config_map invalid WCDMA band %d", band.wcdma_band);    
        status = FALSE;
      }
      break;
    case RFCOM_1X_MODE:
    case RFCOM_1XEVDO_MODE:
      if (band.cdma_band < RFM_CDMA_MAX_BAND)
      {
        *rfc_cfg = &(coupler_device->coupler_rfc_config_mapping.cdma[band.cdma_band]);
      }
      else
      {
        RF_MSG_1(RF_ERROR, "COUPLER rfdevice_rffe_coupler_get_band_config_map invalid CDMA band %d", band.cdma_band);  
        status = FALSE;
      }
      break;
    case RFCOM_GSM_MODE:
      if (band.gsm_band < RFCOM_NUM_GSM_BANDS)
      {
        *rfc_cfg = &(coupler_device->coupler_rfc_config_mapping.gsm[band.gsm_band]);
      }
      else
      {
        RF_MSG_1(RF_ERROR, "COUPLER rfdevice_rffe_coupler_get_band_config_map invalid GSM band %d", band.gsm_band); 
        status = FALSE;
      }
      break;      
    case RFCOM_TDSCDMA_MODE:
      if (band.tdscdma_band < RFCOM_NUM_TDSCDMA_BANDS)
      {
        *rfc_cfg = &(coupler_device->coupler_rfc_config_mapping.tdscdma[band.tdscdma_band]);      
      }
      else
      {
        RF_MSG_1(RF_ERROR, "COUPLER rfdevice_rffe_coupler_get_band_config_map invalid TDSCDMA band %d", band.tdscdma_band);    
        status = FALSE;
      }
      break;
    default:
      RF_MSG_1(RF_ERROR, "COUPLER rfdevice_rffe_coupler_get_band_config_map invalid tech %d", mode);
      status = FALSE;
  }

  return status;
}

