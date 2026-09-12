
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

         ELNA INTERNAL

GENERAL DESCRIPTION
  Helper functions for the ELNA

EXTERNALIZED FUNCTIONS


  Copyright (c) 2014-2015 Qualcomm Technologies Inc. All Rights Reserved

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
  written permission of Qualcomm Technologies Inc.

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rfdevice_elna.mpss/1.22/src/rfdevice_elna_internal.cpp#1 $
$DateTime: 2021/03/09 03:41:28 $
$Author: pwbldsvc $

when       who   what, where, why
--------   ---   ----------------------------------------------------------
09/02/15   px    Added support for alternate gain map
02/17/15   vv    eLNA sleep/wakeup PM trigger support
12/04/14   vv    Initial version

===========================================================================*/


/*===========================================================================

                         INCLUDE FILES FOR MODULE

===========================================================================*/
#include "rfcommon_msg.h"
#include "rfdevice_elna.h"
#include "stringl.h" /* for memscpy */
#include "rfdevice_msg_log.h"
#include "rf_hal_buffer.h"


extern "C"
{
#include "rf_hal_rffe.h"
}

boolean rfdevice_elna::rfdevice_elna_param_validity
    (
      rfdevice_lna_cfg* lna_cfg_p,
      rf_buffer_intf *buff_obj_ptr,
      rf_device_execution_type dev_action,
      int16 script_timing
    )
{ 
  boolean status = TRUE;

  if (lna_cfg_p->rfm_device > RFM_MAX_DEVICES)
  { 
    RF_MSG_1(RF_ERROR, "rfdevice_elna::rfdevice_elna_get_tech_port_info(): invalid RFM device %d passed", lna_cfg_p->rfm_device);    
    return FALSE;
  }

  if( dev_action == RFDEVICE_CREATE_SCRIPT && buff_obj_ptr == NULL)
  {
    RF_MSG( RF_ERROR, "rfdevice_elna::rfdevice_elna_param_validity() Script not allocated for eLNA request");
    return FALSE; 
  }
  
  switch (lna_cfg_p->mode)
  {
    case RFCOM_WCDMA_MODE:
      /* check bands and bws supported */
      if(lna_cfg_p->band.wcdma_band >= RFCOM_NUM_WCDMA_BANDS)
      {
        RF_MSG_1( RF_ERROR, "Invalid WCDMA band %d", lna_cfg_p->band.wcdma_band);
        return FALSE; 
      }
      break;

    case RFCOM_1X_MODE:
    case RFCOM_1XEVDO_MODE:
      /* check bands and bws supported */
      if(lna_cfg_p->band.cdma_band >= RFM_CDMA_MAX_BAND)
      {
        RF_MSG_1( RF_ERROR, "Invalid CDMA band %d", lna_cfg_p->band.cdma_band);
        return FALSE; 
      }
      break;

    case RFCOM_GSM_MODE:
      /* check bands and bws supported */
      if(lna_cfg_p->band.gsm_band >= RFCOM_NUM_GSM_BANDS)
      {
        RF_MSG_1( RF_ERROR, "Invalid GSM band %d", lna_cfg_p->band.gsm_band);
        return FALSE; 
      }
      break;      


    case RFCOM_LTE_MODE:
      /* check bands and bws supported */
      if(lna_cfg_p->band.lte_band >= RFCOM_NUM_LTE_BANDS)
      {
        RF_MSG_1( RF_ERROR, "Invalid LTE band %d", lna_cfg_p->band.lte_band);
        return FALSE; 
      }
      break;


    case RFCOM_TDSCDMA_MODE:
      /* check bands and bws supported */
      if(lna_cfg_p->band.tdscdma_band >= RFCOM_NUM_TDSCDMA_BANDS)
      {
        RF_MSG_1( RF_ERROR, "Invalid TDSCDMA band %d", lna_cfg_p->band.tdscdma_band);
        return FALSE; 
      }
      break;

    default:
      RF_MSG_1(RF_ERROR, "ELNA request for invalid tech %d", lna_cfg_p->mode);
      status = FALSE;
      break;
  }
  return status;
}

boolean rfdevice_elna::rfdevice_elna_get_tech_port_info
(
  rfm_device_enum_type rfm_device,
  rfcom_mode_enum_type mode, 
  rfcom_band_type_u band,
  uint8** port_data_ptr 
)
{
  boolean status = TRUE;

  /*==========================================================================*/
  /* Validity check  */
  /*==========================================================================*/

  if (rfm_device > RFM_MAX_DEVICES)
  { 
    RF_MSG_1(RF_ERROR, "rfdevice_elna::rfdevice_elna_get_tech_port_info(): invalid RFM device %d passed", rfm_device);    
    return FALSE;
  }

  /*==========================================================================*/
  /* Extract tech settings check  */
  /*==========================================================================*/
  
  switch (mode)
  {

    case RFCOM_WCDMA_MODE:
      if(band.wcdma_band < RFCOM_NUM_WCDMA_BANDS)
      {
        *port_data_ptr = &(rfdevice_elna_p->port_info.wcdma[rfm_device][0]);
      }
      else
      {
        RF_MSG_1(RF_ERROR, "rfdevice_elna::rfdevice_elna_get_tech_port_info(): invalid WCDMA band %d", band.wcdma_band);    
        status = FALSE;
      }
      break;

    case RFCOM_1X_MODE:
    case RFCOM_1XEVDO_MODE:
      if (band.cdma_band < RFM_CDMA_MAX_BAND)
      {
        *port_data_ptr = &(rfdevice_elna_p->port_info.cdma[rfm_device][0]);      
      }
      else
      {
        RF_MSG_1(RF_ERROR, "rfdevice_elna::rfdevice_elna_get_tech_port_info():invalid CDMA band %d", band.cdma_band);  
        status = FALSE;
      }
      break;

    case RFCOM_GSM_MODE:
      if (band.gsm_band < RFCOM_NUM_GSM_BANDS)
      {
        *port_data_ptr = &(rfdevice_elna_p->port_info.gsm[rfm_device][0]);
      }
      else
      {
        RF_MSG_1(RF_ERROR, "rfdevice_elna::rfdevice_elna_get_tech_port_info():invalid GSM band %d", band.gsm_band); 
        status = FALSE;
      }
      break;      

    case RFCOM_LTE_MODE:
      if (band.lte_band < RFCOM_NUM_LTE_BANDS)
      {
        *port_data_ptr = &(rfdevice_elna_p->port_info.lte[rfm_device][0]);
      }
      else
      {
        RF_MSG_1(RF_ERROR, "rfdevice_elna::rfdevice_elna_get_tech_port_info(): invalid LTE band %d", band.lte_band); 
        status = FALSE;
      }
      break;

    case RFCOM_TDSCDMA_MODE:
      if (band.tdscdma_band < RFCOM_NUM_TDSCDMA_BANDS)
      {
        *port_data_ptr = &(rfdevice_elna_p->port_info.tdscdma[rfm_device][0]);
      }
      else
      {
        RF_MSG_1(RF_ERROR, "rfdevice_elna::rfdevice_elna_get_tech_port_info():  invalid TDSCDMA band %d", band.tdscdma_band);    
        status = FALSE;
      }
      break;

    default:
      RF_MSG_1(RF_ERROR, "rfdevice_elna::rfdevice_elna_get_tech_port_info():  invalid tech %d", mode);
      status = FALSE;
      break;
  }
  return status;
}


boolean rfdevice_elna::rfdevice_elna_get_elna_gain_info
(
  rfm_device_enum_type rfm_device,
  rfcom_mode_enum_type mode, 
  rfcom_band_type_u band,
  uint8** gain_data_ptr,
  boolean use_alt_gain
)
{
  boolean status = TRUE;
  rfdevice_elna_gain_info_type* gain_info;
  
  /*==========================================================================*/
  /* Validity check */
  /*==========================================================================*/
  if (rfm_device > RFM_MAX_DEVICES)
  { 
    RF_MSG_1(RF_ERROR, "rfdevice_elna::rfdevice_elna_get_elna_gain_info(): invalid RFM device %d passed", rfm_device);    
    return FALSE;
  }

  /*==========================================================================*/
  /* Select default vs. alt gain mapping */
  /*==========================================================================*/
  if(use_alt_gain)
  {
    gain_info = &(this->gain_info_alt);
  }
  else
  {
    gain_info = &(this->gain_info);
  }

  /*==========================================================================*/
  /* Extract tech settings check */
  /*==========================================================================*/
  switch (mode)
  {
    case RFCOM_WCDMA_MODE:
      if(band.wcdma_band < RFCOM_NUM_WCDMA_BANDS)
      {
        *gain_data_ptr = (uint8*)(&(gain_info->wcdma[rfm_device][band.wcdma_band]));
      }
      else
      {
        RF_MSG_1(RF_ERROR, "rfdevice_elna::rfdevice_elna_get_tech_port_info(): invalid WCDMA band %d", band.wcdma_band);    
        status = FALSE;
      }
      break;

    case RFCOM_1X_MODE:
    case RFCOM_1XEVDO_MODE:
      if (band.cdma_band < RFM_CDMA_MAX_BAND)
      {
        *gain_data_ptr = (uint8*)(&(gain_info->cdma[rfm_device][band.cdma_band]));      
      }
      else
      {
        RF_MSG_1(RF_ERROR, "rfdevice_elna::rfdevice_elna_get_tech_port_info():invalid CDMA band %d", band.cdma_band);  
        status = FALSE;
      }
      break;

    case RFCOM_GSM_MODE:
      if (band.gsm_band < RFCOM_NUM_GSM_BANDS)
      {
        *gain_data_ptr = (uint8*)(&(gain_info->gsm[rfm_device][band.gsm_band]));
      }
      else
      {
        RF_MSG_1(RF_ERROR, "rfdevice_elna::rfdevice_elna_get_tech_port_info():invalid GSM band %d", band.gsm_band); 
        status = FALSE;
      }
      break;      

    case RFCOM_LTE_MODE:
      if (band.lte_band < RFCOM_NUM_LTE_BANDS)
      {
        *gain_data_ptr = (uint8*)(&(gain_info->lte[rfm_device][band.lte_band]));
      }
      else
      {
        RF_MSG_1(RF_ERROR, "rfdevice_elna::rfdevice_elna_get_tech_port_info(): invalid LTE band %d", band.lte_band); 
        status = FALSE;
      }
      break;

    case RFCOM_TDSCDMA_MODE:
      if (band.tdscdma_band < RFCOM_NUM_TDSCDMA_BANDS)
      {
        *gain_data_ptr = (uint8*)(&(gain_info->tdscdma[rfm_device][band.tdscdma_band]));
      }
      else
      {
        RF_MSG_1(RF_ERROR, "rfdevice_elna::rfdevice_elna_get_tech_port_info():  invalid TDSCDMA band %d", band.tdscdma_band);    
        status = FALSE;
      }
      break;

    default:
      RF_MSG_1(RF_ERROR, "rfdevice_elna::rfdevice_elna_get_tech_port_info():  invalid tech %d", mode);
      status = FALSE;
      break;
  }
  return status;
}

boolean rfdevice_elna::rfdevice_elna_process_reg_settings
(
  rf_buffer_intf *rf_buff,
  const uint16* reg_table,
  const int16* data_table,
  uint8 write_size,
  rf_device_execution_type dev_action,
  int16 script_timing,
  rfdevice_lna_trigger_enum_type  trigger_type
)
{
  boolean status = TRUE;
  uint8 counter = 0;
  uint8 write_count = 0;
  boolean append_trigger = TRUE;
  /*==========================================================================*/
  /* VALIDITY  */
  /*==========================================================================*/

  /* Check if data was provided */
  if(reg_table == NULL || data_table == NULL)
  {
    RF_MSG( RF_ERROR, "rfdevice_elna::rfdevice_elna_process_reg_settings: Input tables missing");
    return FALSE;
  }

  /* Check if there are any entries to write */
  if(write_size == 0)
  {
    RF_MSG( RF_ERROR, "rfdevice_elna::rfdevice_elna_process_reg_settings: No entries to write");
    return FALSE; 
  }

  /* If script wasnt provided, create one */
  else if(dev_action == RFDEVICE_EXECUTE_IMMEDIATE)
  {
     rf_buff = rf_buffer_create(0, ELNA_MAX_SCRIPT_SIZE, 0);
	 
	 /* Force timing to 0 */
     script_timing = 0;
      
     if(rf_buff == NULL)
     {
       RF_MSG_1(RF_ERROR,"rfdevice_elna::rfdevice_elna_process_reg_settings failed: Unable to get memory for %d entries",ELNA_MAX_SCRIPT_SIZE);
       return FALSE;
     }    
  }
  
  /*==========================================================================*/
  /* PROCESSING  */
  /*==========================================================================*/
  for(counter = 0; counter < write_size && status; counter++)
  {
    if((int16)data_table[counter] != RFDEVICE_ELNA_REG_DATA_INVALID)
    {
      write_count++;

      /* Export local script to rf_buffer */
      status &= rf_buff->append_rffe( (uint8)rfdevice_elna_p->elna_device_info.assigned_usid,
                                      (uint8)rfdevice_elna_p->elna_device_info.bus[0],
                                      (uint16)reg_table[counter],
                                      (uint8)data_table[counter],
                                      RF_BUFFER_WRITE,
                                      script_timing,
                                      rfdevice_elna_p->elna_settings.elna_device_bus_cfg.extended_cmd,
                                      rfdevice_elna_p->elna_settings.elna_device_bus_cfg.settings.half_rate,
                                      rfdevice_elna_p->elna_settings.elna_device_bus_cfg.settings.rd_delay,
                                      TRUE);  

    }
    else
    { 
      append_trigger = FALSE;
    }
  }
  
  /*==========================================================================*/
  /* PROCESSING (ADD TRIGGER) */
  /*==========================================================================*/
  if( (trigger_type!=RFDEVICE_LNA_TRIGGER_NONE) && append_trigger)
  {
    if (!(((trigger_type == RFDEVICE_LNA_TRIGGER_WAKEUP) || (trigger_type == RFDEVICE_LNA_TRIGGER_SLEEP)) &&
           (rfdevice_elna_p->elna_settings.lpm_pm_trig_flag))
        )
    {
      for(counter = 0; counter < rfdevice_elna_p->elna_settings.common_settings.elna_trigger_script_size && status; counter++)
      {
        if((int16)rfdevice_elna_p->elna_settings.common_settings.elna_trigger.data[counter] != RFDEVICE_ELNA_REG_DATA_INVALID)
        {
          write_count++;
      
          /* Export trigger script to rf_buffer */
          status &= rf_buff->append_rffe( (uint8)rfdevice_elna_p->elna_device_info.assigned_usid,
                                          (uint8)rfdevice_elna_p->elna_device_info.bus[0],
                                          (uint16)rfdevice_elna_p->elna_settings.common_settings.elna_trigger.addr[counter],
                                          (uint8)rfdevice_elna_p->elna_settings.common_settings.elna_trigger.data[counter],
                                          RF_BUFFER_WRITE,
                                          script_timing,
                                          rfdevice_elna_p->elna_settings.elna_device_bus_cfg.extended_cmd,
                                          rfdevice_elna_p->elna_settings.elna_device_bus_cfg.settings.half_rate,
                                          rfdevice_elna_p->elna_settings.elna_device_bus_cfg.settings.rd_delay,
                                          TRUE);
        }
      }
    }

    if (status)
    {
      /* log to show trigger append status */
      RFDEVICE_LOG_MSG_2(LOG_RFA_DEVICE_TRASCEIVER_C, MSG_LEGACY_MED, "eLNA %d Trigger appended. Buffer contains %d writes total", 
                                                              rfdevice_elna_p->elna_device_info.rf_device_id,
                                                              rf_buff->get_num_trans(RF_BUFFER_RFFE));
    }
    else
    {

      RFDEVICE_LOG_MSG_2(LOG_RFA_DEVICE_TRASCEIVER_C, MSG_LEGACY_MED, "eLNA %d Failed to append Trigger. Buffer contains %d writes total", 
                                                              rfdevice_elna_p->elna_device_info.rf_device_id,
                                                               rf_buff->get_num_trans(RF_BUFFER_RFFE));
    }

  }
  
  /* Execute immediately and process reads */
  if( status && ( dev_action == RFDEVICE_EXECUTE_IMMEDIATE ) 
      && ( rf_buff->get_num_trans(RF_BUFFER_RFFE)== write_count ) )
  {
    if( RF_HAL_BUS_SUCCESS != rfhal_rffe_execute_rf_buf(rf_buff, 
                                                        NULL, 
                                                        0, 
                                                        &(rfdevice_elna_p->elna_settings.elna_device_bus_cfg.settings),
                                                        0))
      status = FALSE;
  }

  /* Log # actual transactions */
  RFDEVICE_LOG_MSG_3(LOG_RFA_DEVICE_TRASCEIVER_C, MSG_LEGACY_MED,"eLNA %d driver generated %d writes. Buffer contains %d writes total",
                                                  rfdevice_elna_p->elna_device_info.rf_device_id, write_count,
                                                  rf_buff->get_num_trans(RF_BUFFER_RFFE));
  
  /*==========================================================================*/
  /* CLEAN UP  */
  /*==========================================================================*/

  /* Get rid of the buffer we created */
  if(dev_action == RFDEVICE_EXECUTE_IMMEDIATE)
  {
     rf_buffer_delete(rf_buff);        
  }

  return status;
}
