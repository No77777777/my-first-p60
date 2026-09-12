/*!
   @file
   rfdevice_elna.cpp

   @brief
   eLNA  device driver

*/

/*===========================================================================

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

$Header: //components/rel/rfdevice_elna.mpss/1.22/src/rfdevice_elna.cpp#1 $

when       who   what, where, why
--------   ---   ---------------------------------------------------------------
09/02/15   px    Added support for alternate gain map
02/26/15   vv    Add GRFC eLNA support
02/17/15   vv    eLNA sleep/wakeup PM trigger support
12/03/14   vv    Initial file

============================================================================*/

#include "comdef.h"
#include "rfdevice_elna.h"
#include "rfdevice_grfc_elna.h"
#include "rfcommon_msg.h"
#include "rfdevice_msg_log.h"
#include "rfcommon_time_profile.h"
#include "stringl.h" /* for memscpy */
#include "modem_mem.h"


extern "C" {
#include "rf_hal_rffe.h"
#include "modem_mem.h"
}

/* TODO consolidate using helper .h */
#define RFDEVICE_ELNA_MFG_ID_GET(x) (((x) >> 22) & 0x3FF)
#define RFDEVICE_ELNA_PRD_ID_GET(x) (((x) >> 14) & 0xFF)
#define RFDEVICE_ELNA_PORT_NUM_GET(x) ((x) & 0x7F)
#define RFDEVICE_ELNA_RX_TOTAL_GAIN_STATES_GET(x)((x) & 0xF)
#define RFDEVICE_ELNA_GAIN_STATE_GET(x,y)( ( (x) >> y ) & 0xF)
#define RFDEVICE_ELNA_DEFAULT_GAIN_LUT_INDEX 2
#define RFDEVICE_ELNA_ALT_GAIN_LUT_INDEX 3

extern "C"{
boolean rfdevice_elna_choose_settings (
                                       uint16 mfg_id, 
                                       uint8 prd_id, 
                                       uint8 prd_rev, 
                                       rfdevice_elna_settings_type *elna_settings
                                       );
}

/*!
  @brief
  create 3rd party lna devices

  @details
  create 3rd party lna devices and register itself to the parent physical device object passed to it.

  @param rfdevice_physical_ptr : parent physical device object of the apa device
  @param logical_device_cfg : logical device info of the pa device
*/

rfdevice_logical_component* create_gen_elna_object(
                                                  rfdevice_physical_device *rfdevice_physical_ptr,
                                                  rfc_logical_device_info_type *logical_device_cfg
                                                  )
{
  boolean init_status = FALSE;
  rfdevice_class *temp_obj = NULL;
  rfc_phy_device_info_type *phy_device_cfg = NULL;

  /*NULL pointer check*/
  if ( 
       (rfdevice_physical_ptr == NULL)           ||
       (logical_device_cfg == NULL)              ||
       ( ((rfdevice_physical_third_party*)rfdevice_physical_ptr)->third_party_phy_dev_cfg == NULL) 
     )
  {
    RF_MSG( RF_ERROR, "create_gen_elna_object() failed ! NULL Pointer detected");
    return NULL;
  }

  phy_device_cfg=((rfdevice_physical_third_party*)rfdevice_physical_ptr)->third_party_phy_dev_cfg;

 /*Create based on the protocol type*/
  if(phy_device_cfg->rf_device_comm_protocol == RFDEVICE_COMM_PROTO_GRFC)
  { 
    /*GRFC eLNA*/
    temp_obj = (rfdevice_class*)new rfdevice_grfc_elna(rfdevice_physical_ptr,
                                         phy_device_cfg,
                                         logical_device_cfg);
    if (temp_obj != NULL)
    {
      if(((rfdevice_grfc_elna*)temp_obj)->init_status == TRUE)
      { 
        /*store the grfc elna logical device object in its parent physical device object*/
        ((rfdevice_physical_third_party*)rfdevice_physical_ptr)->store_object(temp_obj);
      }
      else
      {
        delete temp_obj; /* destroy the GRFC LNA device object, as it failed to initialize the device correctly. */
        temp_obj = NULL;
      }
    }
  }
  else if(phy_device_cfg->rf_device_comm_protocol == RFDEVICE_COMM_PROTO_RFFE)
  {
    /*RFFE eLNA*/
    temp_obj = (rfdevice_class*)(new rfdevice_elna(rfdevice_physical_ptr ,
                                                  phy_device_cfg,
                                                  logical_device_cfg)); 
    if (temp_obj != NULL)
    {
      if(((rfdevice_elna*)temp_obj)->init_status == TRUE)
      {
        /*store the rffe elna logical device object in its parent physical device object*/
        ((rfdevice_physical_third_party*)rfdevice_physical_ptr)->store_object(temp_obj);
      }
      else
      {
        delete temp_obj; /* destroy the RFFE eLNA device object, as it failed to initialize the device correctly. */
        temp_obj = NULL;
      }
    }
  }
  else
  {
    RF_MSG( RF_ERROR, "create_gen_elna_object() failed ! Invalid commnunication protocal passed. Check RFC ");
  }

  return temp_obj;

}




/*----------------------------------------------------------------------------*/
/*!
  @brief
  constructor- not valid as only the overloaded constructor will be called

*/
rfdevice_elna::rfdevice_elna()
{
  /*Clear out device info */
  memset(&this->elna_device_info, 0, sizeof(rfc_device_cfg_info_type));
  /* Clear out settings */
  memset(&this->elna_settings, 0, sizeof(rfdevice_elna_settings_type));

}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Create specific eLNA based on the device_info - constructor for physical device support

*/
rfdevice_elna::rfdevice_elna(rfdevice_physical_device* phy_obj_ptr,
                             rfc_phy_device_info_type* phy_device_info, 
                             rfc_logical_device_info_type* logical_device_info
                             )
{
  init_status = FALSE;

  if( 
      (phy_obj_ptr == NULL) ||
      (phy_device_info == NULL) ||
      (logical_device_info == NULL) ||
      (logical_device_info->rf_device_type != RFDEVICE_LNA)
    )
  {
    RF_MSG( RF_ERROR, "rfdevice_elna::rfdevice_elna() eLNA device construction failed. Inavlid parameters passed" );
    return;
  }

    /*Call the default constructor*/
    rfdevice_elna();

    /*copy the rfdevice_elna object ptr*/
    rfdevice_elna_p = this;

    /* Update its physical device object ptr */
    rfdevice_elna::rfdevice_phy_obj_ptr = phy_obj_ptr;

    /* Copy over the config info */
    elna_device_info.rf_device_id             =       logical_device_info->rf_device_id;
    elna_device_info.rf_device_type           =       logical_device_info->rf_device_type;
    elna_device_info.rf_asic_id               =       logical_device_info->rf_asic_id;
    elna_device_info.rf_device_comm_protocol  =       phy_device_info->rf_device_comm_protocol;
    elna_device_info.bus[0]                   =       phy_device_info->bus[0];
    elna_device_info.manufacturer_id          =       phy_device_info->manufacturer_id;
    elna_device_info.product_id               =       phy_device_info->product_id;
    elna_device_info.product_rev              =       phy_device_info->product_rev;
    elna_device_info.default_usid_range_start =       phy_device_info->default_usid_range_start;
    elna_device_info.default_usid_range_end   =       phy_device_info->default_usid_range_end;
    elna_device_info.assigned_usid            =       phy_device_info->assigned_usid;
    elna_device_info.init_required            =       phy_device_info->init_required;
    elna_device_info.associated_dac           =       phy_device_info->associated_dac;


    /* Init device header */
    this->mfg_id = (uint16)elna_device_info.manufacturer_id;
    this->prd_id = (uint16)elna_device_info.product_id;
    this->rev_id = (uint16)elna_device_info.product_rev;
    this->rf_device_id = elna_device_info.rf_device_id;
    this->rf_device_type = elna_device_info.rf_device_type;
    this->instance_num  = logical_device_info->rf_asic_id;

    /* Invalidate all ports */
    memset(&this->port_info.cdma, 0xFF, sizeof(uint8)*RFM_CDMA_MAX_BAND*RFM_MAX_DEVICES);
    memset(&this->port_info.gsm, 0xFF, sizeof(uint8)*RFCOM_NUM_GSM_BANDS*RFM_MAX_DEVICES);
    memset(&this->port_info.lte, 0xFF, sizeof(uint8)*RFCOM_NUM_LTE_BANDS*RFM_MAX_DEVICES);
    memset(&this->port_info.wcdma, 0xFF, sizeof(uint8)*RFCOM_NUM_WCDMA_BANDS*RFM_MAX_DEVICES);
    memset(&this->port_info.tdscdma, 0xFF, sizeof(uint8)*RFCOM_NUM_TDSCDMA_BANDS*RFM_MAX_DEVICES);

    /* Invalidate all lna gain states*/
    memset(&this->gain_info.cdma, RFDEVICE_LNA_GAIN_INVALID, sizeof(uint8)*RFM_CDMA_MAX_BAND*RFM_MAX_DEVICES*RFDEVICE_MAX_RX_GAIN_STATES);
    memset(&this->gain_info.gsm, RFDEVICE_LNA_GAIN_INVALID, sizeof(uint8)*RFCOM_NUM_GSM_BANDS*RFM_MAX_DEVICES*RFDEVICE_MAX_RX_GAIN_STATES);
    memset(&this->gain_info.lte, RFDEVICE_LNA_GAIN_INVALID, sizeof(uint8)*RFCOM_NUM_LTE_BANDS*RFM_MAX_DEVICES*RFDEVICE_MAX_RX_GAIN_STATES);
    memset(&this->gain_info.wcdma, RFDEVICE_LNA_GAIN_INVALID, sizeof(uint8)*RFCOM_NUM_WCDMA_BANDS*RFM_MAX_DEVICES*RFDEVICE_MAX_RX_GAIN_STATES);
    memset(&this->gain_info.tdscdma, RFDEVICE_LNA_GAIN_INVALID, sizeof(uint8)*RFCOM_NUM_TDSCDMA_BANDS*RFM_MAX_DEVICES*RFDEVICE_MAX_RX_GAIN_STATES);

    memset(&this->gain_info_alt.cdma, RFDEVICE_LNA_GAIN_INVALID, sizeof(uint8)*RFM_CDMA_MAX_BAND*RFM_MAX_DEVICES*RFDEVICE_MAX_RX_GAIN_STATES);
    memset(&this->gain_info_alt.gsm, RFDEVICE_LNA_GAIN_INVALID, sizeof(uint8)*RFCOM_NUM_GSM_BANDS*RFM_MAX_DEVICES*RFDEVICE_MAX_RX_GAIN_STATES);
    memset(&this->gain_info_alt.lte, RFDEVICE_LNA_GAIN_INVALID, sizeof(uint8)*RFCOM_NUM_LTE_BANDS*RFM_MAX_DEVICES*RFDEVICE_MAX_RX_GAIN_STATES);
    memset(&this->gain_info_alt.wcdma, RFDEVICE_LNA_GAIN_INVALID, sizeof(uint8)*RFCOM_NUM_WCDMA_BANDS*RFM_MAX_DEVICES*RFDEVICE_MAX_RX_GAIN_STATES);
    memset(&this->gain_info_alt.tdscdma, RFDEVICE_LNA_GAIN_INVALID, sizeof(uint8)*RFCOM_NUM_TDSCDMA_BANDS*RFM_MAX_DEVICES*RFDEVICE_MAX_RX_GAIN_STATES);

    /*Get elna settings*/
    if ( !rfdevice_elna_choose_settings (
                                         (uint16) this->mfg_id, 
                                         (uint8)  this->prd_id, 
                                         (uint8)  this->rev_id, 
                                         &elna_settings
                                        )
        )
    {
      RF_MSG_1( RF_ERROR, "rfdevice_elna::rfdevice_elna() eLNA %d Unable to get elna settings during device creation",this->rf_device_id );
    }
    else
    {
      //On successfull eLNA device object construction, update the init flag
      init_status = TRUE;
    }

}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  destructor

*/
rfdevice_elna::~rfdevice_elna()
{
}



/*!
   @brief
   populates the band map information from rfc

   @details
    populates the band map information from rfc
 
   @param rfdevice_lna_cfg
   pointer to a lna cfg object which contains lna specific config info

   @param *data_ptr
   band mapping information passed as int32 pointer
    
   @param size
   number of valid words in the *data_ptr
    
   @return
   boolean indicating pass or fail
*/
boolean rfdevice_elna::set_band_map
(
    rfdevice_lna_cfg* lna_cfg_p,
    int32 *data_ptr, 
    uint8 size
)
{
  uint8 index = 0;
  uint32 mfr_id = 0;
  uint32 prd_id = 0;
  uint8 port = 0xFF;
  uint8 rx_gain_states = 0;
  uint8 bitshift = 0;
  boolean status = TRUE;
  uint8* port_data_ptr = NULL;
  uint8* gain_data_ptr = NULL;
  uint8 rx_to_lna_gain_map[RFDEVICE_MAX_RX_GAIN_STATES];
  uint8 alt_gain_lut_index = RFDEVICE_ELNA_ALT_GAIN_LUT_INDEX;

  /* Invalidate all lna gain states*/
  memset(&rx_to_lna_gain_map, RFDEVICE_LNA_GAIN_INVALID, sizeof(uint8)*RFDEVICE_MAX_RX_GAIN_STATES);

  /*==========================================================================*/
  /* VALIDITY CHECK */
  /*==========================================================================*/

  if ((lna_cfg_p == NULL)||(data_ptr == NULL))
  {
    RF_MSG_1(RF_ERROR, "rfdevice_elna::set_band_map() eLNA %d set_band_map invalid params",this->elna_device_info.rf_device_id);
    return FALSE;
  }

  /*==========================================================================*/
  /* SET PORT MAP */
  /*==========================================================================*/
  // Find port for matching alternate part
  for(index = 0; index < size; index++)
  {
    mfr_id = RFDEVICE_ELNA_MFG_ID_GET(data_ptr[index]);
    prd_id = RFDEVICE_ELNA_PRD_ID_GET(data_ptr[index]);
    port   = RFDEVICE_ELNA_PORT_NUM_GET(data_ptr[index]);

    /* Check if this is the part we are storing port info for */
    if (
        this->elna_device_info.manufacturer_id == mfr_id &&
        this->elna_device_info.product_id == prd_id
       )
      break;
  }

  if(index >= size || port >= this->elna_settings.num_ports)
  {
    RF_MSG_6(RF_ERROR, "rfdevice_elna::set_band_map() eLNA %d set_band_map %d invalid device mfr_id 0x%x(0x%x), prd_id 0x%x(0x%x)",
             this->elna_device_info.rf_device_id, port,
             mfr_id, this->elna_device_info.manufacturer_id,
             prd_id, this->elna_device_info.product_id);
    return FALSE;
  }

  // Save found port in device object
  if(!rfdevice_elna_get_tech_port_info(lna_cfg_p->rfm_device, lna_cfg_p->mode, lna_cfg_p->band, &port_data_ptr))
  {
    /* use LTE band as LTE has the most bands */
    RF_MSG_3(RF_ERROR, "eLNA %d Unable to get port info for tech %d band %d",
             this->elna_device_info.rf_device_id, lna_cfg_p->mode, lna_cfg_p->band.lte_band);
    status = FALSE;
  }
  else if(port_data_ptr != NULL)
  {
    port_data_ptr[(uint16)(lna_cfg_p->band.lte_band)] = port;
  }

  /*==========================================================================*/
  /* SET DEFAULT GAIN MAPPING */
  /*==========================================================================*/
  rx_gain_states = RFDEVICE_ELNA_RX_TOTAL_GAIN_STATES_GET(data_ptr[RFDEVICE_ELNA_DEFAULT_GAIN_LUT_INDEX]);

  // First verify valid gain states
  if((rx_gain_states == RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID) || (rx_gain_states > RFDEVICE_MAX_RX_GAIN_STATES))
  {
    RF_MSG_3(RF_ERROR, "rfdevice_elna::set_band_map(): eLNA %d Invalid lna default gain mapping for tech %d band %d",
             this->elna_device_info.rf_device_id, lna_cfg_p->mode, lna_cfg_p->band.lte_band);
    status = FALSE;
  }
  else
  {
    // Parse asic config info for gain mapping
    bitshift = 0;
    for(index=0; index < rx_gain_states; index++)
    {
      bitshift += RFDEVICE_BITS_PER_RX_GAIN_STATE;
      rx_to_lna_gain_map[index] = RFDEVICE_ELNA_GAIN_STATE_GET(data_ptr[RFDEVICE_ELNA_DEFAULT_GAIN_LUT_INDEX], bitshift);
    }

    // Get gain mapping data pointer
    if(!rfdevice_elna_get_elna_gain_info(lna_cfg_p->rfm_device, lna_cfg_p->mode, lna_cfg_p->band, &gain_data_ptr, FALSE))
    {
      RF_MSG_3(RF_ERROR, "eLNA %d Unable to get lna gain info for tech %d band %d",
              this->elna_device_info.rf_device_id, lna_cfg_p->mode, lna_cfg_p->band.lte_band);
      status = FALSE;
    }
    else if(gain_data_ptr != NULL)
    { 
      // Save gain mapping in device object
      for(index=0; index < RFDEVICE_MAX_RX_GAIN_STATES; index++)
      {
	      *(gain_data_ptr + index) = rx_to_lna_gain_map[index];
      }
    }
  }

  /* Invalidate all lna gain states*/
  memset(&rx_to_lna_gain_map, RFDEVICE_LNA_GAIN_INVALID, sizeof(uint8)*RFDEVICE_MAX_RX_GAIN_STATES);

  /*==========================================================================*/
  /* SET ALT GAIN MAPPING */
  /*==========================================================================*/
  rx_gain_states = RFDEVICE_ELNA_RX_TOTAL_GAIN_STATES_GET(data_ptr[RFDEVICE_ELNA_ALT_GAIN_LUT_INDEX]);

  // Check if alt gain mapping exists. If not, use default gain mapping for alt gain.
  if (rx_gain_states == RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID)
  {
    rx_gain_states = RFDEVICE_ELNA_RX_TOTAL_GAIN_STATES_GET(data_ptr[RFDEVICE_ELNA_DEFAULT_GAIN_LUT_INDEX]);
    alt_gain_lut_index = RFDEVICE_ELNA_DEFAULT_GAIN_LUT_INDEX;
  }

  // First verify valid gain states
  if((rx_gain_states == RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID) || (rx_gain_states > RFDEVICE_MAX_RX_GAIN_STATES))
  {
    RF_MSG_3(RF_ERROR, "rfdevice_elna::set_band_map(): eLNA %d Invalid lna asdiv gain mapping for tech %d band %d",
             this->elna_device_info.rf_device_id, lna_cfg_p->mode, lna_cfg_p->band.lte_band);
  }
  else
  {
    // Parse asic config info for gain mapping
    bitshift = 0;
    for(index=0; index < rx_gain_states; index++)
    {
      bitshift += RFDEVICE_BITS_PER_RX_GAIN_STATE;
      rx_to_lna_gain_map[index] = RFDEVICE_ELNA_GAIN_STATE_GET(data_ptr[alt_gain_lut_index], bitshift);
    }

    // Get asdiv gain mapping data pointer
    if(!rfdevice_elna_get_elna_gain_info(lna_cfg_p->rfm_device, lna_cfg_p->mode, lna_cfg_p->band, &gain_data_ptr, TRUE))
    {
      RF_MSG_3(RF_ERROR, "eLNA %d Unable to get lna asdiv gain info for tech %d band %d",
              this->elna_device_info.rf_device_id, lna_cfg_p->mode, lna_cfg_p->band.lte_band);
      status = FALSE;
    }
    else if(gain_data_ptr != NULL)
    { 
      // Save gain mapping in device object
      for(index=0; index < RFDEVICE_MAX_RX_GAIN_STATES; index++)
      {
        *(gain_data_ptr + index) = rx_to_lna_gain_map[index];
      }
    }
  }

  return status;
}


/*!
  @brief
  interface function for lna sleep

  @details
  rfdevice_lna_sleep()
   
  @param rfdevice_lna_cfg
  pointer to a lna cfg object which contains lna specific config info  
  
  @param buff_obj_ptr
  void pointer to CCS buffer object
 
  @param dev_action
  indicates if it is immediate write to hw or build scripts

  @param script_timing
  timing offset for CCS scripts

  @return
  boolean indicating pass or fail
*/
boolean rfdevice_elna::sleep
(
    rfdevice_lna_cfg* lna_cfg_p, 
    rf_buffer_intf *buff_obj_ptr, 
    rf_device_execution_type dev_action, 
    int16 script_timing
)
{
   rf_time_tick_type start_tick = rf_time_get_tick();

   /*==========================================================================*/
   /* VALIDITY CHECK  */
   /*==========================================================================*/

   /* NULL pointer check*/
   if(lna_cfg_p == NULL)
   {
     RF_MSG( RF_ERROR, "rfdevice_elna::sleep() NULL pointer detected");
     return FALSE;
   }

   if( dev_action == RFDEVICE_CREATE_SCRIPT && buff_obj_ptr == NULL)
   {
     RF_MSG( RF_ERROR, "rfdevice_elna::sleep() Script not allocated for eLNA request");
     return FALSE; 
   }

   /*==========================================================================*/
   /* CHOOSE SETTINGS & PROCESS SCRIPT */
   /*==========================================================================*/

   if(
       (this->elna_settings.common_settings.elna_sleep.addr == NULL)         || 
       (this->elna_settings.common_settings.elna_sleep.data == NULL)         ||
       (this->elna_settings.common_settings.elna_sleep_script_size == 0)
      )
   {
     RF_MSG_1(RF_HIGH, "rfdevice_elna::sleep() eLNA %d has no sleep settings", this->elna_device_info.rf_device_id);
   }
   else if(!rfdevice_elna_process_reg_settings(
                                           buff_obj_ptr,
                                           &(this->elna_settings.common_settings.elna_sleep.addr[0]),
                                           &(this->elna_settings.common_settings.elna_sleep.data[0]),
                                           this->elna_settings.common_settings.elna_sleep_script_size,
                                           dev_action,
                                           script_timing,
                                           lna_cfg_p->trigger_type
                                          ))

   {
     RF_MSG_1(RF_ERROR, "rfdevice_elna::sleep() eLNA %d failed to process sleep settings", this->elna_device_info.rf_device_id);
     return FALSE;
   }

   /*==========================================================================*/
   /* TIME PROFILING  */
   /*==========================================================================*/
   RFDEVICE_LOG_MSG_4(LOG_RFA_DEVICE_TRASCEIVER_C, MSG_LEGACY_MED,"rfdevice_elna::sleep() eLNA %d sleep script %d timing %d complete in %d us",
                      this->elna_device_info.rf_device_id, dev_action, script_timing,
                      rf_time_get_elapsed(start_tick,RF_USEC));
   return TRUE;

}


/*!
    @brief
    interface function for lna wakeup

    @details
    rfdevice_lna_wakeup()
   
    @param rfdevice_lna_cfg
    pointer to a lna cfg object which contains lna specific config info  
   
    @param buff_obj_ptr
    void pointer to CCS buffer object

    @param dev_action
    indicates if it is immediate write to hw or build scripts

    @param script_timing
    timing offset for CCS scripts

    @return
    boolean indicating pass or fail
*/
boolean rfdevice_elna::wakeup
   (
    rfdevice_lna_cfg* lna_cfg_p, 
    rf_buffer_intf *buff_obj_ptr, 
    rf_device_execution_type dev_action, 
    int16 script_timing
    )
{
   rf_time_tick_type start_tick = rf_time_get_tick();

   /*==========================================================================*/
   /* VALIDITY CHECK  */
   /*==========================================================================*/

    /* NULL pointer check*/
   if(lna_cfg_p == NULL)
   {
     RF_MSG( RF_ERROR, "rfdevice_elna::wakeup() NULL pointer detected");
     return FALSE;
   }

   if( dev_action == RFDEVICE_CREATE_SCRIPT && buff_obj_ptr == NULL)
   {
     RF_MSG( RF_ERROR, "rfdevice_elna::wakeup() Script not allocated for eLNA request");
     return FALSE; 
   }

   /*==========================================================================*/
   /* CHOOSE SETTINGS & PROCESS SCRIPT */
   /*==========================================================================*/

   if(
       (this->elna_settings.common_settings.elna_wakeup.addr == NULL)         || 
       (this->elna_settings.common_settings.elna_wakeup.data == NULL)         ||
       (this->elna_settings.common_settings.elna_wakeup_script_size == 0)
      )
   {
     RF_MSG_1(RF_HIGH, "rfdevice_elna::wakeup() eLNA %d has no wakeup settings", this->elna_device_info.rf_device_id);
   }
   else if(!rfdevice_elna_process_reg_settings(
                                           buff_obj_ptr,
                                           &(this->elna_settings.common_settings.elna_wakeup.addr[0]),
                                           &(this->elna_settings.common_settings.elna_wakeup.data[0]),
                                           this->elna_settings.common_settings.elna_wakeup_script_size,
                                           dev_action,
                                           script_timing,
                                           lna_cfg_p->trigger_type
                                          ))
   {
     RF_MSG_1(RF_ERROR, "rfdevice_elna::wakeup() eLNA %d failed to process wakeup settings", this->elna_device_info.rf_device_id);
     return FALSE;
   }

   /*==========================================================================*/
   /* TIME PROFILING  */
   /*==========================================================================*/
   RFDEVICE_LOG_MSG_4(LOG_RFA_DEVICE_TRASCEIVER_C, MSG_LEGACY_MED,"eLNA %d wakeup script %d timing %d complete in %d us",
                      this->elna_device_info.rf_device_id, dev_action, script_timing,
                      rf_time_get_elapsed(start_tick,RF_USEC));

  return TRUE;

}


/*!
    @brief
    interface function for lna trigger, to latch the pending settings.

    @details
    rfdevice_lna_trigger()
   
    @param rfdevice_lna_cfg
    pointer to a lna cfg object which contains lna specific config info
   
    @param buff_obj_ptr
    void pointer to CCS buffer object

    @param dev_action
    indicates if it is immediate write to hw or build scripts

    @param script_timing
    time offset for the script where a negative value refers to the time BEFORE the event

    @return
    boolean indicating pass or fail
*/
boolean rfdevice_elna::trigger
   (
    rfdevice_lna_cfg* lna_cfg_p,
    rf_buffer_intf *buff_obj_ptr,
    rf_device_execution_type dev_action,
    int16 script_timing
    )
{
  rf_time_tick_type start_tick = rf_time_get_tick();
  uint8 counter = 0;
  uint8 write_count = 0;
  boolean status = TRUE;

  /*==========================================================================*/
  /* VALIDITY CHECK  */
  /*==========================================================================*/

   /* NULL pointer check*/
  if(lna_cfg_p == NULL)
  {
    RF_MSG( RF_ERROR, "rfdevice_elna::trigger() NULL pointer detected");
    return FALSE;
  }

  if( dev_action == RFDEVICE_CREATE_SCRIPT && buff_obj_ptr == NULL)
  {
    RF_MSG( RF_ERROR, "rfdevice_elna::trigger() Script not allocated for eLNA request");
    return FALSE; 
  }

  if(
      (this->elna_settings.common_settings.elna_trigger.addr == NULL)         || 
      (this->elna_settings.common_settings.elna_trigger.data == NULL)         ||
      (this->elna_settings.common_settings.elna_trigger_script_size == 0)
     )
  {
    RF_MSG_1(RF_HIGH, "rfdevice_elna::trigger() eLNA %d has no trigger settings", this->elna_device_info.rf_device_id);
    return TRUE;
  }

  /* If script wasnt provided, create one */
  if(dev_action == RFDEVICE_EXECUTE_IMMEDIATE)
  {
     buff_obj_ptr = rf_buffer_create(0, ELNA_MAX_SCRIPT_SIZE, 0);
	 
	 /* Force timing to 0 */
     script_timing = 0;
      
     if(buff_obj_ptr == NULL)
     {
       RF_MSG_1(RF_ERROR,"rfdevice_elna::trigger failed: Unable to get memory for %d entries",ELNA_MAX_SCRIPT_SIZE);
       return FALSE;
     }    
  }

  /*==========================================================================*/
  /* CHOOSE SETTINGS & PROCESS SCRIPT */
  /*==========================================================================*/

  if(lna_cfg_p->trigger_type != RFDEVICE_LNA_TRIGGER_NONE)
  {
    if (!(((lna_cfg_p->trigger_type == RFDEVICE_LNA_TRIGGER_WAKEUP) || (lna_cfg_p->trigger_type == RFDEVICE_LNA_TRIGGER_SLEEP)) &&
           (this->elna_settings.lpm_pm_trig_flag))
        )
    {
      for(counter = 0; counter < rfdevice_elna_p->elna_settings.common_settings.elna_trigger_script_size; counter++)
      {
        if((int16)rfdevice_elna_p->elna_settings.common_settings.elna_trigger.data[counter] != RFDEVICE_ELNA_REG_DATA_INVALID)
        {
          write_count++;
      
          /* Export trigger script to rf_buffer */
          status &= buff_obj_ptr->append_rffe( (uint8)rfdevice_elna_p->elna_device_info.assigned_usid,
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

  if( status && ( dev_action == RFDEVICE_EXECUTE_IMMEDIATE ) 
      && ( buff_obj_ptr->get_num_trans(RF_BUFFER_RFFE)== write_count ) )
  {
    if( RF_HAL_BUS_SUCCESS != rfhal_rffe_execute_rf_buf(buff_obj_ptr, 
                                                        NULL, 
                                                        0, 
                                                        &(rfdevice_elna_p->elna_settings.elna_device_bus_cfg.settings),
                                                        0))
      status = FALSE;
  }


  }

  /*==========================================================================*/
  /* CLEAN UP  */
  /*==========================================================================*/

  /* Get rid of the buffer we created */
  if(dev_action == RFDEVICE_EXECUTE_IMMEDIATE)
  {
     rf_buffer_delete(buff_obj_ptr);        
  }


  /*==========================================================================*/
  /* TIME PROFILING  */
  /*==========================================================================*/
  RFDEVICE_LOG_MSG_4(LOG_RFA_DEVICE_TRASCEIVER_C, MSG_LEGACY_MED,"eLNA %d trigger script %d timing %d complete in %d us",
                     this->elna_device_info.rf_device_id, dev_action, script_timing,
                     rf_time_get_elapsed(start_tick,RF_USEC));

 return status;


}


/*!
    @brief
    returns the lna gain settings 

    @details
    returns the lna gain settings 

    @param rfdevice_lna_cfg
    pointer to a lna cfg object which contains lna specific config info

    @param buff_obj_ptr
    void pointer to CCS buffer object

    @param dev_action
    indicates if it is immediate write to hw or build scripts

    @param script_timing
    timing offset for CCS scripts

    @return
    boolean indicating pass or fail
  */
boolean rfdevice_elna::set_lna_gain
   (
    rfdevice_lna_cfg* lna_cfg_p,
    rf_buffer_intf *buff_obj_ptr,
    rf_device_execution_type dev_action,
    int16 script_timing
    )
{
   uint8* port_data_ptr = NULL;
   uint8* gain_data_ptr = NULL;
   const int16* data_tbl = NULL;
   uint8 port_index = 0xFF;
   rf_time_tick_type start_tick = rf_time_get_tick();

   /* NULL pointer check*/
   if(lna_cfg_p == NULL)
   {
     RF_MSG( RF_ERROR, "rfdevice_elna::set_lna_gain() NULL pointer detected");
     return FALSE;
   }

   /*=========================================================================*/
   /* VALIDITY CHECK  */
   /*==========================================================================*/

   if((!rfdevice_elna_param_validity(lna_cfg_p, buff_obj_ptr, dev_action, script_timing)) || 
      (!(lna_cfg_p->lna_gain < RFDEVICE_LNA_GAIN_INVALID))
      )
   {
     RF_MSG_1(RF_ERROR, "rfdevice_elna::set_lna_gain() eLNA %d Invalid params", this->elna_device_info.rf_device_id);
     return FALSE;
   }

  /*==========================================================================*/
  /* CHOOSE SETTINGS */
  /*==========================================================================*/
   if(!rfdevice_elna_get_tech_port_info( lna_cfg_p->rfm_device, lna_cfg_p->mode, lna_cfg_p->band,&port_data_ptr))
  {
    /* use LTE band as LTE has the most bands */
    RF_MSG_2(RF_ERROR, "rfdevice_elna::set_lna_gain() Unable to get port info for tech %d band %d", lna_cfg_p->mode, lna_cfg_p->band.lte_band);
    return FALSE;
  }
  else if(port_data_ptr != NULL)
  {
    port_index = port_data_ptr[(uint16)(lna_cfg_p->band.lte_band)];
  }
  /* check if thats a valid port */
  if(port_index >= rfdevice_elna_p->elna_settings.num_ports)
  {
    RF_MSG_2(RF_ERROR, "rfdevice_elna::set_lna_gain() Incorrect port %d for tech %d", port_index, lna_cfg_p->mode);
    return FALSE;
  }

  /*==========================================================================*/
  /*PROCESS SCRIPT */
  /*==========================================================================*/

  if(
      (this->elna_settings.port_settings.elna_gain.addr == NULL)         || 
      (this->elna_settings.port_settings.elna_gain.data == NULL)         ||
      (this->elna_settings.port_settings.elna_gain_script_size == 0)     ||
      (this->elna_settings.port_settings.elna_num_gain_states == 0)
     )
  {
    RF_MSG_1(RF_HIGH, "rfdevice_elna::set_lna_gain eLNA %d has no gain settings", this->elna_device_info.rf_device_id);
  }
  else
  {
          /*extract the eLNA gain settings for the required tech,band and lna gain*/
           data_tbl = ((this->elna_settings.port_settings.elna_gain.data) + 
                      (port_index)*((this->elna_settings.port_settings.elna_num_gain_states)*(this->elna_settings.port_settings.elna_gain_script_size)) + 
                      (lna_cfg_p->lna_gain)*(this->elna_settings.port_settings.elna_gain_script_size));

         if(!rfdevice_elna_process_reg_settings(
                                                 buff_obj_ptr,
                                                 &(this->elna_settings.port_settings.elna_gain.addr[0]),
                                                 data_tbl,
                                                 this->elna_settings.port_settings.elna_gain_script_size ,
                                                 dev_action,
                                                 script_timing,
                                                 lna_cfg_p->trigger_type
                                             ))
        {

          RF_MSG_1(RF_ERROR, "rfdevice_elna::set_lna_gain() eLNA %d failed to process trigger settings", this->elna_device_info.rf_device_id);
          return FALSE;
        }
  }

  /*==========================================================================*/
  /* TIME PROFILING  */
  /*==========================================================================*/
   RFDEVICE_LOG_MSG_7(LOG_RFA_DEVICE_TRASCEIVER_C, MSG_LEGACY_MED,"rfdevice_elna::set_lna_gain() eLNA %d tech %d band %d lna_gain %d script %d timing %d complete in %d us",
                      this->elna_device_info.rf_device_id,lna_cfg_p->mode,lna_cfg_p->band.lte_band, lna_cfg_p->lna_gain, dev_action, script_timing,
                      rf_time_get_elapsed(start_tick,RF_USEC));

 return TRUE;

}

/*!
    @brief
    returns the lna gain for the requested system gain

    @details
    the techs would call this api to get the lna gain to system gain mapping for a particular tech and band.

    @param rfdevice_lna_cfg
    pointer to a lna cfg object which contains lna specific config info

    @param *lna_gain
    returns an enum indicating the lna gain

    @param use_alt_gain
    set to TRUE to use alternate gain mapping (such as for asdiv)

    @return
    boolean indicating pass or fail
*/
boolean rfdevice_elna::get_lna_gain_index
   (
    rfdevice_lna_cfg* lna_cfg_p,
    rfdevice_lna_gain_enum_type *lna_gain,
    boolean use_alt_gain
   )
{
   uint8* gain_data_ptr=NULL;
   rf_time_tick_type start_tick = rf_time_get_tick();
  

   /* NULL pointer check*/
   if((lna_cfg_p == NULL) || (lna_gain ==  NULL))
   {
     RF_MSG( RF_ERROR, "rfdevice_elna::get_lna_gain() invalid params");
     return FALSE;
   }

   *lna_gain = RFDEVICE_LNA_GAIN_INVALID;

   /*=========================================================================*/
   /* VALIDITY CHECK  */
   /*==========================================================================*/

   if((!rfdevice_elna_param_validity(lna_cfg_p, NULL, RFDEVICE_EXECUTE_IMMEDIATE, 0)) ||
      (!(lna_cfg_p->system_gain < RFDEVICE_MAX_RX_GAIN_STATES))
       )
   {
     RF_MSG_1(RF_ERROR, "rfdevice_elna::get_lna_gain() eLNA %d Invalid params", this->elna_device_info.rf_device_id);
     return FALSE;
   }

  /*==========================================================================*/
  /* CHOOSE SETTINGS */
  /*==========================================================================*/
   if(!rfdevice_elna_get_elna_gain_info(lna_cfg_p->rfm_device, lna_cfg_p->mode, lna_cfg_p->band, &gain_data_ptr, use_alt_gain))
   {
     /* use LTE band as LTE has the most bands */
     RF_MSG_2(RF_ERROR, "rfdevice_elna::get_lna_gain() Unable to get port info for tech %d band %d", lna_cfg_p->mode, lna_cfg_p->band.lte_band);
     return FALSE;
   }
   else if(gain_data_ptr != NULL)
   {
     *lna_gain  = (rfdevice_lna_gain_enum_type)gain_data_ptr[(uint8)(lna_cfg_p->system_gain)];
   }

   if(*lna_gain == RFDEVICE_LNA_GAIN_INVALID )
   { 
      RF_MSG_2(RF_ERROR, "rfdevice_elna::get_lna_gain() invalid LNA gain info for tech %d band %d", lna_cfg_p->mode, lna_cfg_p->band.lte_band);
      return FALSE;
   }
 
  /*==========================================================================*/
  /* TIME PROFILING  */
  /*==========================================================================*/
   RFDEVICE_LOG_MSG_6(LOG_RFA_DEVICE_TRASCEIVER_C, MSG_LEGACY_MED, "rfdevice_elna::get_lna_gain() eLNA %d tech %d band %d lna_gain %d alt_gain %d complete in %d us",
                                                                   this->elna_device_info.rf_device_id,
                                                                   lna_cfg_p->mode,
                                                                   lna_cfg_p->band.lte_band, 
                                                                   *lna_gain, 
                                                                   use_alt_gain,
                                                                   rf_time_get_elapsed(start_tick,RF_USEC));

   return TRUE;
}