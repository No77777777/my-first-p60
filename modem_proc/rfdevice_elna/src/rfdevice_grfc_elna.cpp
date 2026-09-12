/*!
   @file
   rfdevice_grfc_elna.cpp

   @brief
   GRFC eLNA  device driver

*/

/*===========================================================================

  Copyright (c) 2014-2016 Qualcomm Technologies Inc. All Rights Reserved

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

$Header: //components/rel/rfdevice_elna.mpss/1.22/src/rfdevice_grfc_elna.cpp#1 $

when       who    what, where, why
--------   ---   ---------------------------------------------------------------
09/02/15   px    Added support for alternate gain map
03/24/15   vv    Add GRFC eLNA sleep support
03/11/15   vv    Replace RFM_MAX_DEVICES with RFM_MAX_WAN_DEVICES to reduce memory usage
02/19/15   vv    Initial file

============================================================================*/

#include "comdef.h"
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



/*----------------------------------------------------------------------------*/
/*!
  @brief
  constructor- not valid as only the overloaded constructor will be called

*/
rfdevice_grfc_elna::rfdevice_grfc_elna()
{
  /*Clear out device info */
  memset(&this->grfc_elna_device_info, 0, sizeof(rfc_device_cfg_info_type));

}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Create specific eLNA based on the device_info - constructor for physical device support

*/
rfdevice_grfc_elna::rfdevice_grfc_elna(rfdevice_physical_device* phy_obj_ptr,
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
    RF_MSG( RF_ERROR, "rfdevice_grfc_elna(): eLNA device construction failed. Inavlid parameters passed" );
    init_status = FALSE;
    return;
  }

    /*Call the default constructor*/
    rfdevice_grfc_elna();

    /*copy the rfdevice_grfc_elna object ptr*/
    rfdevice_grfc_elna_p = this;

    /* Update its physical device object ptr */
    rfdevice_grfc_elna::rfdevice_phy_obj_ptr = phy_obj_ptr;

    /* Copy over the config info */
    grfc_elna_device_info.rf_device_id             =       logical_device_info->rf_device_id;
    grfc_elna_device_info.rf_device_type           =       logical_device_info->rf_device_type;
    grfc_elna_device_info.rf_asic_id               =       logical_device_info->rf_asic_id;
    grfc_elna_device_info.rf_device_comm_protocol  =       phy_device_info->rf_device_comm_protocol;
    grfc_elna_device_info.bus[0]                   =       phy_device_info->bus[0];
    grfc_elna_device_info.manufacturer_id          =       phy_device_info->manufacturer_id;
    grfc_elna_device_info.product_id               =       phy_device_info->product_id;
    grfc_elna_device_info.product_rev              =       phy_device_info->product_rev;
    grfc_elna_device_info.default_usid_range_start =       phy_device_info->default_usid_range_start;
    grfc_elna_device_info.default_usid_range_end   =       phy_device_info->default_usid_range_end;
    grfc_elna_device_info.assigned_usid            =       phy_device_info->assigned_usid;
    grfc_elna_device_info.init_required            =       phy_device_info->init_required;
    grfc_elna_device_info.associated_dac           =       phy_device_info->associated_dac;


    /* Init device header */
    this->mfg_id = (uint16)grfc_elna_device_info.manufacturer_id;
    this->prd_id = (uint16)grfc_elna_device_info.product_id;
    this->rev_id = (uint16)grfc_elna_device_info.product_rev;
    this->rf_device_id = grfc_elna_device_info.rf_device_id;
    this->rf_device_type = grfc_elna_device_info.rf_device_type;
    this->instance_num  = logical_device_info->rf_asic_id;

    /* Invalidate all grfc settings */
    memset(&this->band_map_grfc_tbl.cdma,    0xFF, sizeof(rfdevice_elna_grfc_rfc_tbl_type)*RFM_CDMA_MAX_BAND*RFM_MAX_WAN_DEVICES);
    memset(&this->band_map_grfc_tbl.gsm,     0xFF, sizeof(rfdevice_elna_grfc_rfc_tbl_type)*RFCOM_NUM_GSM_BANDS*RFM_MAX_WAN_DEVICES);
    memset(&this->band_map_grfc_tbl.lte,     0xFF, sizeof(rfdevice_elna_grfc_rfc_tbl_type)*RFCOM_NUM_LTE_BANDS*RFM_MAX_WAN_DEVICES);
    memset(&this->band_map_grfc_tbl.wcdma,   0xFF, sizeof(rfdevice_elna_grfc_rfc_tbl_type)*RFCOM_NUM_WCDMA_BANDS*RFM_MAX_WAN_DEVICES);
    memset(&this->band_map_grfc_tbl.tdscdma, 0xFF, sizeof(rfdevice_elna_grfc_rfc_tbl_type)*RFCOM_NUM_TDSCDMA_BANDS*RFM_MAX_WAN_DEVICES);

    //On successfull eLNA device object construction, update the init flag
    init_status = TRUE;

}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  destructor

*/
rfdevice_grfc_elna::~rfdevice_grfc_elna()
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
boolean rfdevice_grfc_elna::set_band_map
(
    rfdevice_lna_cfg* lna_cfg_p,
    int32 *data_ptr, 
    uint8 size
)
{
 
  rfdevice_elna_grfc_rfc_tbl_type rfc_grfc_tbl;
  rfdevice_elna_grfc_rfc_tbl_type* rfc_grfc_tbl_ptr = NULL;
  boolean status = TRUE;
  uint8 grfc_count =0;
  uint8 bitshift =0;
  uint8 rf_gain_index =0;
  
  /*==========================================================================*/
  /* VALIDITY CHECK  */
  /*==========================================================================*/

  /*NULL Pointer*/
  if ((data_ptr == NULL) || (lna_cfg_p == NULL))
  {
    RF_MSG(RF_ERROR, "set_band_map():"
                     "GRFC eLNA %d set_band_config() failed. NULL pointer passed");    
    return FALSE;
  }

  /*reset the local buffer*/
  memset(&rfc_grfc_tbl,0xFF,sizeof(rfdevice_elna_grfc_rfc_tbl_type));
    

  /*==========================================================================*/
  /* EXTRACT THE GRFC/eLNA GAIN INFO FROM RFC DATA  */
  /*==========================================================================*/
  
  /* Add +1 to data_ptr index as first byte is for rev id */
  while ( (data_ptr != NULL) &&
          (RFDEVICE_GRFC_ELNA_VALID_FLAG_GET(data_ptr[grfc_count+1]) != 0) &&
          (grfc_count            < RFDEVICE_GRFC_ELNA_MAX_NUM_GRFCS)
        )
  {
    rfc_grfc_tbl.rfc_grfc_data[grfc_count].grfc_num     = RFDEVICE_GRFC_ELNA_GRFC_NUMBER_GET(data_ptr[grfc_count+1]);
    rfc_grfc_tbl.rfc_grfc_data[grfc_count].valid_flag   = RFDEVICE_GRFC_ELNA_VALID_FLAG_GET(data_ptr[grfc_count+1]);

    /*Capture the sleep  settings*/
    bitshift = RFDEVICE_GRFC_ELNA_SLEEP_BITSHIFT;
    rfc_grfc_tbl.rfc_grfc_data[grfc_count].sleep = RFDEVICE_GRFC_ELNA_SLEEP_GET(data_ptr[grfc_count+1],bitshift);

    /*Capture the GRFC ELNA gain states  per GRFC*/
    bitshift = RFDEVICE_GRFC_ELNA_GAIN0_BITSHIFT;
    for(rf_gain_index=0; rf_gain_index < RFDEVICE_MAX_RX_GAIN_STATES; rf_gain_index++)
    {
      rfc_grfc_tbl.rfc_grfc_data[grfc_count].gain_states[rf_gain_index]=RFDEVICE_GRFC_ELNA_GAIN_STATE_GET(data_ptr[grfc_count+1],bitshift);
      bitshift -= RFDEVICE_BITS_PER_GRFC_ELNA_GAIN_STATE;
    }
    grfc_count++;
  }
  
  rfc_grfc_tbl.num_grfc = grfc_count;

  
  /*==========================================================================*/
  /* SAVE BAND CONFIG  */
  /*==========================================================================*/
  
  if(!rfdevice_grfc_elna_get_band_map_grfc_tbl(lna_cfg_p->rfm_device,
                                               lna_cfg_p->mode,
                                               lna_cfg_p->band, 
                                               &rfc_grfc_tbl_ptr)
     )
  {
    /* use LTE band as LTE has the most bands */
    RF_MSG_3(RF_ERROR, "set_band_map():GRFC elna %d Unable to get band config map info for tech %d band %d", 
                       this->grfc_elna_device_info.rf_device_id, lna_cfg_p->mode, lna_cfg_p->band.lte_band);
    status = FALSE;
  }
  else if(rfc_grfc_tbl_ptr != NULL)
  {
    memscpy(rfc_grfc_tbl_ptr,sizeof(rfdevice_elna_grfc_rfc_tbl_type),
            &rfc_grfc_tbl,sizeof(rfdevice_elna_grfc_rfc_tbl_type));
    
    status = TRUE;
  }
  
  return TRUE; 
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
boolean rfdevice_grfc_elna::set_lna_gain
   (
    rfdevice_lna_cfg* lna_cfg_p,
    rf_buffer_intf *buff_obj_ptr,
    rf_device_execution_type dev_action,
    int16 script_timing
    )
{
   rf_time_tick_type start_tick = rf_time_get_tick();
   boolean status = TRUE;
   rfdevice_elna_grfc_rfc_tbl_type* rfc_grfc_tbl_ptr = NULL;
   rfdevice_elna_req_enum_type grfc_elna_req = RFDEVICE_ELNA_INVALID;

   /* NULL pointer check*/
   if(lna_cfg_p == NULL)
   {
     RF_MSG( RF_ERROR, "set_lna_gain(): NULL pointer detected");
     return FALSE;
   }

   /*=========================================================================*/
   /* VALIDITY CHECK  */
   /*==========================================================================*/

   if((!rfdevice_grfc_elna_param_validity(lna_cfg_p, buff_obj_ptr, dev_action, script_timing)) || 
      (!(lna_cfg_p->lna_gain < RFDEVICE_LNA_GAIN_INVALID))
      )
   {
     RF_MSG_1(RF_ERROR, "set_lna_gain() GRFC eLNA %d Invalid params", this->grfc_elna_device_info.rf_device_id);
     return FALSE;
   }

  /*==========================================================================*/
  /* CHOOSE SETTINGS */
  /*==========================================================================*/
  if(!rfdevice_grfc_elna_get_band_map_grfc_tbl(lna_cfg_p->rfm_device,
                                               lna_cfg_p->mode,
                                               lna_cfg_p->band, 
                                               &rfc_grfc_tbl_ptr)
     )
  {
    /* use LTE band as LTE has the most bands */
    RF_MSG_3(RF_ERROR, "set_lna_gain():GRFC eLNA %d Unable to get band config map info for tech %d band %d", 
                       this->grfc_elna_device_info.rf_device_id, lna_cfg_p->mode, lna_cfg_p->band.lte_band);
    status &= FALSE;
  }
  else if(rfc_grfc_tbl_ptr != NULL)
  {

    /*==========================================================================*/
    /*PROCESS SCRIPT */
    /*==========================================================================*/

    /*Specify the eLNA request type*/
    grfc_elna_req = RFDEVICE_ELNA_GAIN;

    if(!rfdevice_grfc_elna_process_grfc_tbl(grfc_elna_req,
                                            buff_obj_ptr,
                                            dev_action,
                                            script_timing,
                                            rfc_grfc_tbl_ptr,
                                            lna_cfg_p)
      )
    { 
      RF_MSG_1(RF_ERROR, "set_lna_gain(): GRFC eLNA %d failed to process gain settings", this->grfc_elna_device_info.rf_device_id);
      status &= FALSE;
    }

  }
  else
  {
      RF_MSG_1(RF_ERROR, "set_lna_gain(): GRFC eLNA %d rfc_grfc_tbl_ptr returned NULL", this->grfc_elna_device_info.rf_device_id);
      status &= FALSE;
  }


  /*==========================================================================*/
  /* TIME PROFILING  */
  /*==========================================================================*/
   RFDEVICE_LOG_MSG_7(LOG_RFA_DEVICE_TRASCEIVER_C, MSG_LEGACY_MED,"set_lna_gain(): eLNA %d tech %d band %d lna_gain %d script %d timing %d complete in %d us",
                      this->grfc_elna_device_info.rf_device_id,lna_cfg_p->mode,lna_cfg_p->band.lte_band, lna_cfg_p->lna_gain, dev_action, script_timing,
                      rf_time_get_elapsed(start_tick,RF_USEC));

 return status;

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
    Currently NOT supported. Will force crash if TRUE.
   
    @return
    boolean indicating pass or fail
*/
boolean rfdevice_grfc_elna::get_lna_gain_index
   (
    rfdevice_lna_cfg* lna_cfg_p,
    rfdevice_lna_gain_enum_type *lna_gain,
    boolean use_alt_gain
   )
{
   boolean status = TRUE;
   rf_time_tick_type start_tick = rf_time_get_tick();
  
   /* NULL pointer check*/
   if((lna_cfg_p == NULL) || (lna_gain ==  NULL))
   {
     RF_MSG( RF_ERROR, "get_lna_gain_index(): invalid params");
     return FALSE;
   }

   *lna_gain = RFDEVICE_LNA_GAIN_INVALID;

   /*=========================================================================*/
   /* VALIDITY CHECK  */
   /*==========================================================================*/

   if((!rfdevice_grfc_elna_param_validity(lna_cfg_p, NULL, RFDEVICE_EXECUTE_IMMEDIATE, 0)) ||
      (!(lna_cfg_p->system_gain < RFDEVICE_MAX_RX_GAIN_STATES))
       )
   {
     RF_MSG_1(RF_ERROR, "get_lna_gain_index():  GRFC eLNA %d Invalid params", this->grfc_elna_device_info.rf_device_id);
     return FALSE;
   }

  /*==========================================================================*/
  /* CHOOSE SETTINGS */
  /*==========================================================================*/

   /*GRFC eLNAs have one to one system gain to lna gain mapping. Typecast the system gain to lna gain index and return*/
   if(((rfdevice_lna_gain_enum_type)(lna_cfg_p->system_gain)) < RFDEVICE_LNA_GAIN_MAX)
   {
     *lna_gain = (rfdevice_lna_gain_enum_type)(lna_cfg_p->system_gain);
   }
   else
   {
     *lna_gain = RFDEVICE_LNA_GAIN_INVALID;
     status &= FALSE;
   }

  /*==========================================================================*/
  /* TIME PROFILING  */
  /*==========================================================================*/
   RFDEVICE_LOG_MSG_5(LOG_RFA_DEVICE_TRASCEIVER_C, MSG_LEGACY_MED,"get_lna_gain_index():  eLNA %d tech %d band %d lna_gain %d complete in %d us",
                      this->grfc_elna_device_info.rf_device_id,
                      lna_cfg_p->mode,
                      lna_cfg_p->band.lte_band, 
                      *lna_gain, 
                      rf_time_get_elapsed(start_tick,RF_USEC)
                      );

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
boolean rfdevice_grfc_elna::sleep
(
    rfdevice_lna_cfg* lna_cfg_p, 
    rf_buffer_intf *buff_obj_ptr, 
    rf_device_execution_type dev_action, 
    int16 script_timing
)
{
   
  rf_time_tick_type start_tick = rf_time_get_tick();
  boolean status = TRUE;
  rfdevice_elna_grfc_rfc_tbl_type* rfc_grfc_tbl_ptr = NULL;
  rfdevice_elna_req_enum_type grfc_elna_req = RFDEVICE_ELNA_INVALID;

   /* NULL pointer check*/
   if(lna_cfg_p == NULL)
   {
     RF_MSG( RF_ERROR, "sleep(): NULL pointer detected");
     return FALSE;
   }

   /*=========================================================================*/
   /* VALIDITY CHECK  */
   /*==========================================================================*/

   if(!rfdevice_grfc_elna_param_validity(lna_cfg_p, buff_obj_ptr, dev_action, script_timing))
   {
     RF_MSG_1(RF_ERROR, "sleep() GRFC eLNA %d Invalid params", this->grfc_elna_device_info.rf_device_id);
     return FALSE;
   }

  /*==========================================================================*/
  /* CHOOSE SETTINGS */
  /*==========================================================================*/
  if(!rfdevice_grfc_elna_get_band_map_grfc_tbl(lna_cfg_p->rfm_device,
                                               lna_cfg_p->mode,
                                               lna_cfg_p->band, 
                                               &rfc_grfc_tbl_ptr)
     )
  {
    /* use LTE band as LTE has the most bands */
    RF_MSG_3(RF_ERROR, "sleep():GRFC eLNA %d Unable to get band config map info for tech %d band %d", 
                       this->grfc_elna_device_info.rf_device_id, lna_cfg_p->mode, lna_cfg_p->band.lte_band);
    status &= FALSE;
  }
  else if(rfc_grfc_tbl_ptr != NULL)
  {
    /*==========================================================================*/
    /*PROCESS SCRIPT */
    /*==========================================================================*/

    /*Specify the eLNA request type*/
    grfc_elna_req = RFDEVICE_ELNA_SLEEP;

    if(!rfdevice_grfc_elna_process_grfc_tbl(grfc_elna_req,
                                            buff_obj_ptr,
                                            dev_action,
                                            script_timing,
                                            rfc_grfc_tbl_ptr,
                                            lna_cfg_p)
      )
    { 
      RF_MSG_1(RF_ERROR, "sleep(): GRFC eLNA %d failed to process gain settings", this->grfc_elna_device_info.rf_device_id);
      status &= FALSE;
    }

  }
  else
  {
      RF_MSG_1(RF_ERROR, "sleep(): GRFC eLNA %d rfc_grfc_tbl_ptr returned NULL", this->grfc_elna_device_info.rf_device_id);
      status &= FALSE;
  }


  /*==========================================================================*/
  /* TIME PROFILING  */
  /*==========================================================================*/
   RFDEVICE_LOG_MSG_7(LOG_RFA_DEVICE_TRASCEIVER_C, MSG_LEGACY_MED,"sleep(): eLNA %d tech %d band %d lna_gain %d script %d timing %d complete in %d us",
                      this->grfc_elna_device_info.rf_device_id,lna_cfg_p->mode,lna_cfg_p->band.lte_band, lna_cfg_p->lna_gain, dev_action, script_timing,
                      rf_time_get_elapsed(start_tick,RF_USEC));

 return status;

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
boolean rfdevice_grfc_elna::wakeup
   (
    rfdevice_lna_cfg* lna_cfg_p, 
    rf_buffer_intf *buff_obj_ptr, 
    rf_device_execution_type dev_action, 
    int16 script_timing
    )
{
 
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
boolean rfdevice_grfc_elna::trigger
   (
    rfdevice_lna_cfg* lna_cfg_p,
    rf_buffer_intf *buff_obj_ptr,
    rf_device_execution_type dev_action,
    int16 script_timing
    )
{

 return TRUE;


}

