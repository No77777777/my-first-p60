/*!
   @file
   rfdevice_grfc_coupler.cpp

   @brief
   GRFC Coupler Driver

*/

/*=============================================================================

Copyright (c) 2014-2016 by QUALCOMM Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rfdevice_coupler.mpss/1.22/src/rfdevice_grfc_coupler.cpp#1 $

when       who   what, where, why
--------   ---   ---------------------------------------------------------------
01/22/16   hzh    KW errors fix
11/10/15   aak    Change to check for GRFC 0
10/09/15   aak    Change F3 message to a device log packet
02/11/15   yb     Fixed band map index for GRFC Coupler
01/26/15   yb     Fixed bit shift masks
01/22/15   yb     Added bitmasks and bishifts for RFC data
12/26/14   ndb    Initial file

=============================================================================*/

#include "comdef.h"
#include "rfdevice_grfc_coupler.h"
#include "rfcommon_msg.h"
#include "rfdevice_msg_log.h"
#include "rfcommon_time_profile.h"
#include "modem_mem.h"
#include "stringl.h" /* for memscpy */


#ifdef TEST_FRAMEWORK
#error code not present
#endif

extern "C"
{
#include "rf_hal_rffe.h"
#include "fs_public.h"
#include "modem_mem.h"
}

extern int32 rfc_common_get_signal_num(int32 rf_sig_name, rfc_signal_type sig_type);

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Create specific grfc coupler based on the device_info
  
*/
rfdevice_grfc_coupler::rfdevice_grfc_coupler
(
  rfdevice_physical_device* phy_obj_ptr , 
  rfc_phy_device_info_type* phy_device_info , 
  rfc_logical_device_info_type* logical_device_info
)
{
  this->init_status = FALSE;
  memset(&(this->grfc_coupler_device_info), 0, sizeof(rfc_device_cfg_info_type));
  /* initialize the grfc coupler ptr */
  rfdevice_grfc_coupler::rfdevice_grfc_coupler_p = this;
  /* Update its physical device object ptr */
  rfdevice_grfc_coupler::rfdevice_phy_obj_ptr = phy_obj_ptr;

  if( 
      (phy_obj_ptr != NULL) && 
      (phy_device_info != NULL) && 
      (logical_device_info != NULL) && 
      (logical_device_info->rf_device_type == RFDEVICE_COUPLER)
    )
  {
    /* Copy over the config info */
    grfc_coupler_device_info.rf_device_id             =       logical_device_info->rf_device_id;
    grfc_coupler_device_info.rf_device_type           =       logical_device_info->rf_device_type;
    grfc_coupler_device_info.rf_asic_id               =       logical_device_info->rf_asic_id;
    grfc_coupler_device_info.rf_device_comm_protocol  =       phy_device_info->rf_device_comm_protocol;
    grfc_coupler_device_info.bus[0]                   =       phy_device_info->bus[0];
    grfc_coupler_device_info.manufacturer_id          =       phy_device_info->manufacturer_id;
    grfc_coupler_device_info.product_id               =       phy_device_info->product_id;
    grfc_coupler_device_info.product_rev              =       phy_device_info->product_rev;
    grfc_coupler_device_info.default_usid_range_start =       phy_device_info->default_usid_range_start;
    grfc_coupler_device_info.default_usid_range_end   =       phy_device_info->default_usid_range_end;
    grfc_coupler_device_info.assigned_usid            =       phy_device_info->assigned_usid;
    grfc_coupler_device_info.init_required            =       phy_device_info->init_required;
    grfc_coupler_device_info.associated_dac           =       phy_device_info->associated_dac;

    /* Init device header */
    this->mfg_id = (uint16)grfc_coupler_device_info.manufacturer_id;
    this->prd_id = (uint16)grfc_coupler_device_info.product_id;
    this->rev_id = (uint16)grfc_coupler_device_info.product_rev;
    this->rf_device_id = grfc_coupler_device_info.rf_device_id;
    this->rf_device_type = RFDEVICE_COUPLER;  
    this->instance_num  = logical_device_info->rf_asic_id;
    this->init_status = TRUE;
  }
}


/*----------------------------------------------------------------------------*/
/*!
  @brief
  constructor- not valid as only the overloaded constructor will be called
  
*/
rfdevice_grfc_coupler::rfdevice_grfc_coupler()
{
  /*Clear out device info */ 
  memset(&this->grfc_coupler_device_info, 0, sizeof(rfc_device_cfg_info_type)); 

  /*Clear out the GRFC band map tbl*/
  memset(&this->band_map_grfc_tbl, 0, sizeof(band_map_grfc_tbl)); 
}


/*----------------------------------------------------------------------------*/
/*!
  @brief
  destructor
  
*/
rfdevice_grfc_coupler::~rfdevice_grfc_coupler()
{
 
}

/*----------------------------------------------------------------------------*/
 /*!
   @brief
   Initialize Coupler for operation
 
   @details
   This function can be used to initialize the coupler for operation
       
 */
void rfdevice_grfc_coupler::init()
{
  RF_MSG_1(RF_HIGH, "GRFC Coupler %d doesn't support Init API()", 
                    this->grfc_coupler_device_info.rf_device_id);    
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
boolean rfdevice_grfc_coupler::init 
(
  int32* rfc_data
)
{
  RF_MSG_1(RF_HIGH, "GRFC Coupler %d doesn't support Init API() with RFC_data",
                    this->grfc_coupler_device_info.rf_device_id);    
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
boolean rfdevice_grfc_coupler::trigger
(
   int32* rfc_data,
   rf_buffer_intf *buff_obj_ptr, 
   rf_device_execution_type dev_action, 
   int16 script_timing
)
{
   RFDEVICE_LOG_MSG_1(LOG_RFA_DEVICE_TUNER_C, MSG_LEGACY_MED,
                      "GRFC Coupler %d doesn't support Trigger API()",
                      this->grfc_coupler_device_info.rf_device_id);    
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
boolean rfdevice_grfc_coupler::wakeup
(
   int32* rfc_data,
   rf_buffer_intf *buff_obj_ptr, 
   rf_device_execution_type dev_action,
   int16 script_timing
)
{
  RF_MSG_1(RF_HIGH, "GRFC Coupler %d doesn't support Wakeup API()",
                     this->grfc_coupler_device_info.rf_device_id);    
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
boolean rfdevice_grfc_coupler::sleep
(
   int32* rfc_data,
   rf_buffer_intf *buff_obj_ptr, 
   rf_device_execution_type dev_action,
   int16 script_timing
)
{
  RF_MSG_1(RF_HIGH, "GRFC Coupler %d doesn't support sleep API()",
                    this->grfc_coupler_device_info.rf_device_id);    
  return TRUE;
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
boolean rfdevice_grfc_coupler::config_coupler
(
  rfdevice_coupler_tech_cfg_type tech_cfg,
  rf_buffer_intf *buff_obj_ptr, 
  rf_device_execution_type dev_action, 
  int16 script_timing
)
{
  rf_time_tick_type start_tick = rf_time_get_tick();
  rfdevice_grfc_coupler_rfc_grfc_tbl_type* rfc_grfc_tbl_ptr = NULL;
  boolean status = TRUE;
   

  /*==========================================================================*/
  /* VALIDITY CHECK  */
  /*==========================================================================*/
  if(this->rf_device_type != RFDEVICE_COUPLER)
  {
    RF_MSG(RF_ERROR, "GRFC Coupler driver not constructed. Config coupler failed. Check RFC");    
    return FALSE;
  }  
  if(!rfdevice_grfc_coupler_validate( buff_obj_ptr, 
                                      dev_action, 
                                      script_timing))
  {
    RF_MSG_1(RF_ERROR,"GRFC Coupler %d config coupler: Invalid params",this->grfc_coupler_device_info.rf_device_id);
    return FALSE;
  }

  /*==========================================================================*/
  /*Get GRFC tbl info from the mapped data*/
  /*==========================================================================*/
  if(!rfdevice_grfc_coupler_get_band_map_grfc_tbl(tech_cfg.mode,
                                                  tech_cfg.band, 
                                                  &rfc_grfc_tbl_ptr))
  {
    /* use LTE band as LTE has the most bands */
    RF_MSG_3(RF_ERROR,"GRFC Coupler %d Unable to get GRFC table info for tech %d band %d", 
                      this->grfc_coupler_device_info.rf_device_id, tech_cfg.mode,tech_cfg.band.lte_band);
    return FALSE;
  }

  /*==========================================================================*/
  /* PROCESS GRFC Coupler Table   */
  /*==========================================================================*/
  else if(rfc_grfc_tbl_ptr != NULL)
  {
     
    status &= rfdevice_grfc_coupler_process_grfc_tbl(buff_obj_ptr,
                                                     dev_action,
                                                     script_timing,
                                                     rfc_grfc_tbl_ptr,
                                                     tech_cfg.direction);
  }
   
  /*==========================================================================*/
  /* TIME PROFILING  */
  /*==========================================================================*/
  RFDEVICE_LOG_MSG_4(LOG_RFA_DEVICE_TUNER_C, MSG_LEGACY_MED,
                     "GRFC Coupler %d config coupler for script %d timing %d complete in %d us",
                     this->grfc_coupler_device_info.rf_device_id,
                     dev_action, script_timing,
                     rf_time_get_elapsed(start_tick,RF_USEC));

  return status;
  
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Store band specific GRFC configuration 

  @details
  Band specific coupler GRFC number and its values based on the direction which is pushed from RFC 
  is stored in the device using this API

  @param mode
  enum indicating the radio access technology

  @param band
  enum indicating the band number

  @param data_ptr
  void pointer to pass or receive desired information   
*/
boolean rfdevice_grfc_coupler::set_band_config_map
(
  rfcom_mode_enum_type mode,
  rfcom_band_type_u band,
  int32 *data_ptr
)
{

  rfdevice_grfc_coupler_rfc_grfc_tbl_type rfc_grfc_tbl;
  rfdevice_grfc_coupler_rfc_grfc_tbl_type* rfc_grfc_tbl_ptr = NULL;
  boolean status = TRUE;
  uint8 grfc_count =0;
  
  /*==========================================================================*/
  /* VALIDITY CHECK  */
  /*==========================================================================*/
  if(this->rf_device_type != RFDEVICE_COUPLER)
  {
    RF_MSG(RF_ERROR, "GRFC Coupler driver not constructed. set_band_config_map failed. Check RFC");    
    return FALSE;
  }

  if (data_ptr == NULL)
  {
    RF_MSG_3(RF_ERROR, "GRFC Coupler %d set_band_config_map invalid param for mode %d, band %d",
                       this->grfc_coupler_device_info.rf_device_id, mode, band.lte_band);    
    return FALSE;
  }

  /*reset the local buffer*/
  memset(&rfc_grfc_tbl,0,sizeof(rfc_grfc_tbl));
    

  /*==========================================================================*/
  /* EXTRACT THE GRFC/LOGIC_DIRECTION INFO FROM RFC DATA  */
  /*==========================================================================*/
  
  /* Add +1 to data_ptr index as first byte is for rev id */
  while ( (data_ptr != NULL) &&
          (RFDEVICE_GRFC_COUPLER_VALID_FLAG_GET(data_ptr[grfc_count+1]) != 0) &&
          (grfc_count            != RFDEVICE_GRFC_COUPLER_MAX_NUM_GRFCS)
        )
  {
    rfc_grfc_tbl.rfc_grfc_data[grfc_count].grfc_num                  = RFDEVICE_GRFC_COUPLER_GRFC_NUMBER_GET(data_ptr[grfc_count+1]);
    rfc_grfc_tbl.rfc_grfc_data[grfc_count].logic_dir_data.valid_flag = RFDEVICE_GRFC_COUPLER_VALID_FLAG_GET(data_ptr[grfc_count+1]);
    rfc_grfc_tbl.rfc_grfc_data[grfc_count].logic_dir_data.open_logic = RFDEVICE_GRFC_COUPLER_OPEN_LOGIC_GET(data_ptr[grfc_count+1]);
    rfc_grfc_tbl.rfc_grfc_data[grfc_count].logic_dir_data.fwd_logic  = RFDEVICE_GRFC_COUPLER_FWD_LOGIC_GET(data_ptr[grfc_count+1]);
    rfc_grfc_tbl.rfc_grfc_data[grfc_count].logic_dir_data.rev_logic  = RFDEVICE_GRFC_COUPLER_REV_LOGIC_GET(data_ptr[grfc_count+1]);
    grfc_count++;
  }
  
  rfc_grfc_tbl.num_grfc = grfc_count;

  
  /*==========================================================================*/
  /* SAVE BAND CONFIG  */
  /*==========================================================================*/
  
  if(!rfdevice_grfc_coupler_get_band_map_grfc_tbl(mode,
                                                band, 
                                                &rfc_grfc_tbl_ptr))
  {
    /* use LTE band as LTE has the most bands */
    RF_MSG_3(RF_ERROR, "GRFC Coupler %d Unable to get band config map info for tech %d band %d", 
                       this->grfc_coupler_device_info.rf_device_id, mode, band.lte_band);
    status = FALSE;
  }
  else if(rfc_grfc_tbl_ptr != NULL)
  {
    memscpy(rfc_grfc_tbl_ptr,sizeof(rfdevice_grfc_coupler_rfc_grfc_tbl_type),
            &rfc_grfc_tbl,sizeof(rfc_grfc_tbl));
    
    status = TRUE;
  }
  
  return TRUE; 
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  The function returns the pointer to tech specific coupler grfc rfc table.

  @details
  rfdevice_grfc_coupler_get_band_map_grfc_tbl()

  @param qcoupler_device
  pointer to coupler device

  @param mode
  enum indicating the radio access technology

  @param band
  enum indicating the band number

  @param rfc_grfc_tbl
  pointer to the coupler grfc table

  @return
  boolean indicating pass or fail
*/
boolean rfdevice_grfc_coupler::rfdevice_grfc_coupler_get_band_map_grfc_tbl
(
  rfcom_mode_enum_type mode,
  rfcom_band_type_u band,
  rfdevice_grfc_coupler_rfc_grfc_tbl_type **rfc_grfc_tbl
)
{

  boolean status = TRUE;

  switch (mode)
  {
#ifdef FEATURE_WCDMA
    case RFCOM_WCDMA_MODE:
      if(band.wcdma_band < RFCOM_NUM_WCDMA_BANDS)
      {
        *rfc_grfc_tbl = &(this->band_map_grfc_tbl.wcdma[band.wcdma_band]);
      }
      else
      {
        RF_MSG_1(RF_ERROR, "GRFC Coupler rfdevice_grfc_coupler_get_band_map_grfc_tbl invalid WCDMA band %d",
                           band.wcdma_band);    
        status = FALSE;
      }
      break;
#endif /*ifdef FEATURE_WCDMA*/
#ifdef FEATURE_CDMA1X
    case RFCOM_1X_MODE:
    case RFCOM_1XEVDO_MODE:
      if (band.cdma_band < RFM_CDMA_MAX_BAND)
      {
        *rfc_grfc_tbl = &(this->band_map_grfc_tbl.cdma[band.cdma_band]);
      }
      else
      {
        RF_MSG_1(RF_ERROR, "GRFC Coupler rfdevice_grfc_coupler_get_band_map_grfc_tbl invalid CDMA band %d",
                           band.cdma_band);  
        status = FALSE;
      }
      break;
#endif /*ifdef FEATURE_CDMA1X*/
#ifdef FEATURE_GSM
    case RFCOM_GSM_MODE:
      if (band.gsm_band < RFCOM_NUM_GSM_BANDS)
      {
        *rfc_grfc_tbl = &(this->band_map_grfc_tbl.gsm[band.gsm_band]);
      }
      else
      {
        RF_MSG_1(RF_ERROR, "GRFC Coupler rfdevice_grfc_coupler_get_band_map_grfc_tbl invalid GSM band %d",
                           band.gsm_band); 
        status = FALSE;
      }
      break;      
#endif  /*ifdef FEATURE_GSM*/
#ifdef FEATURE_LTE
    case RFCOM_LTE_MODE:
      if (band.lte_band < RFCOM_NUM_LTE_BANDS)
      {
        *rfc_grfc_tbl = &(this->band_map_grfc_tbl.lte[band.lte_band]);
      }
      else
      {
        RF_MSG_1(RF_ERROR, "GRFC Coupler rfdevice_grfc_coupler_get_band_map_grfc_tbl invalid LTE band %d", 
                           band.lte_band); 
        status = FALSE;
      }
      break;
#endif /*ifdef FEATURE_LTE*/
#ifdef FEATURE_TDSCDMA
    case RFCOM_TDSCDMA_MODE:
      if (band.tdscdma_band < RFCOM_NUM_TDSCDMA_BANDS)
      {
        *rfc_grfc_tbl = &(this->band_map_grfc_tbl.tdscdma[band.tdscdma_band]);      }
      else
      {
        RF_MSG_1(RF_ERROR, "GRFC Coupler rfdevice_grfc_coupler_get_band_map_grfc_tbl invalid TDSCDMA band %d",
                           band.tdscdma_band);    
        status = FALSE;
      }
      break;
#endif /*ifdef FEATURE_TDSCDMA*/
    default:
      RF_MSG_1(RF_ERROR, "GRFC Coupler rfdevice_grfc_coupler_get_band_map_grfc_tbl invalid tech %d",mode);
      status = FALSE;
      break;
  }
  return status;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  The function is used to validate all the necessary paramters and the depedencies need to
  read/write the GRFC Coupler.
  
  @details
  rfdevice_qcoupler_validate()

  @param buff_obj_ptr
  void pointer to CCS buffer object

  @param dev_action
  indicates if it is immediate write to hw or build scripts    

  @script_timing
  timing in us

  @return
  boolean indicating pass or fail
*/

boolean rfdevice_grfc_coupler::rfdevice_grfc_coupler_validate
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
    RF_MSG( RF_ERROR, "Invalid Script for GRFC Coupler request");
    return FALSE; 
  }
  
  /* Null pointer check */
  if( dev_action == RFDEVICE_CREATE_SCRIPT && buff_obj_ptr == NULL)
  {
    RF_MSG( RF_ERROR, "Script not allocated for GRFC Coupler request");
    return FALSE; 
  }

  return status;
}
/*!
  @brief
  The function is used to process the GRFC table based on the direction
  
  @details
  rfdevice_grfc_coupler_process_grfc_tbl()

  @param buff_obj_ptr
  void pointer to CCS buffer object

  @param dev_action
  indicates if it is immediate write to hw or build scripts    

  @script_timing
  timing in us

  @param rfc_grfc_tbl
  pointer to the coupler grfc table

  @param direction
  Enum indicating the required state of the coupler

  @return
  boolean indicating pass or fail
*/
boolean rfdevice_grfc_coupler::rfdevice_grfc_coupler_process_grfc_tbl
( 
  rf_buffer_intf *buff_obj_ptr, 
  rf_device_execution_type dev_action, 
  int16 script_timing,
  rfdevice_grfc_coupler_rfc_grfc_tbl_type *rfc_grfc_tbl_ptr,
  rfdevice_coupling_direction_type direction
  
)
{
  boolean status = TRUE;
  uint8 grfc_count = 0;
  rfc_logic_type dir_logic;
  int32 signal_num = -1;

  if( rfc_grfc_tbl_ptr != NULL )
  {
    RFDEVICE_LOG_MSG_1(LOG_RFA_DEVICE_TUNER_C, MSG_LEGACY_MED,
                       "rfc_grfc_tbl_ptr->num_grfc: %d",
                       rfc_grfc_tbl_ptr->num_grfc);

    for (grfc_count = 0; grfc_count < rfc_grfc_tbl_ptr->num_grfc; grfc_count++)
    {
    
      signal_num = rfc_common_get_grfc_num(rfc_grfc_tbl_ptr->rfc_grfc_data[grfc_count].grfc_num);

      if(signal_num >= 0)
      {
        switch(direction)
        {
          case RFDEVICE_COUPLER_DIRECTION_OPEN:
            dir_logic = (rfc_logic_type)rfc_grfc_tbl_ptr->rfc_grfc_data[grfc_count].logic_dir_data.open_logic;
            break;
 
          case RFDEVICE_COUPLER_DIRECTION_FWD:
            dir_logic = (rfc_logic_type)rfc_grfc_tbl_ptr->rfc_grfc_data[grfc_count].logic_dir_data.fwd_logic;
            break;
      
          case RFDEVICE_COUPLER_DIRECTION_REV:
            dir_logic = (rfc_logic_type)rfc_grfc_tbl_ptr->rfc_grfc_data[grfc_count].logic_dir_data.rev_logic;
            break;

          default:
            dir_logic = (rfc_logic_type)rfc_grfc_tbl_ptr->rfc_grfc_data[grfc_count].logic_dir_data.fwd_logic;
            break;
        }

        if ( dev_action == RFDEVICE_EXECUTE_IMMEDIATE)
        {
          status &= rfc_common_set_grfc(signal_num,
                                        dir_logic);
        }
        else if ((dev_action == RFDEVICE_CREATE_SCRIPT) && ( buff_obj_ptr != NULL ))
        {
          status &= buff_obj_ptr->append_grfc(signal_num,
                                              dir_logic,
                                              script_timing,
                                              TRUE);
        }
        else
        {
          RF_MSG_1(RF_ERROR,"buff_obj_ptr is NULL in dev action: %d",dev_action);  
          status &= FALSE;
        }
      }
      else
      {
        RF_MSG_3(RF_ERROR,"signal_num:%d for grfc_num:%d is invalid in dev action: %d",
                   signal_num, 
                   rfc_grfc_tbl_ptr->rfc_grfc_data[grfc_count].grfc_num,
                   dev_action);

        status &= FALSE;
      }
    }
  }
  else
  {
    RF_MSG(RF_ERROR,"rfc_grfc_tbl_ptr is NULL in rfdevice_grfc_coupler_process_grfc_tbl API ()");  
    status &= FALSE;
  }

  return status;
}

