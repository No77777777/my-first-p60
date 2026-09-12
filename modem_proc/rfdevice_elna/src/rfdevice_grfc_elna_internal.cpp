
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

         ELNA INTERNAL

GENERAL DESCRIPTION
  Helper functions for the ELNA

EXTERNALIZED FUNCTIONS


  Copyright (c) 2015, 2018 by Qualcomm Technologies INCORPORATED.
  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rfdevice_elna.mpss/1.22/src/rfdevice_grfc_elna_internal.cpp#1 $
$DateTime: 2021/03/09 03:41:28 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---   ----------------------------------------------------------
02/22/18   vp    Fix KW errors
03/24/15   vv    Add GRFC eLNA sleep support
02/19/15   vv    Initial version

===========================================================================*/


/*===========================================================================

                         INCLUDE FILES FOR MODULE

===========================================================================*/
#include "rfcommon_msg.h"
#include "rfdevice_grfc_elna.h"
#include "stringl.h" /* for memscpy */
#include "rfdevice_msg_log.h"
#include "rf_hal_buffer.h"


extern "C"
{
#include "rf_hal_grfc.h"
}


boolean rfdevice_grfc_elna::rfdevice_grfc_elna_param_validity
    (
      rfdevice_lna_cfg* lna_cfg_p,
      rf_buffer_intf *buff_obj_ptr,
      rf_device_execution_type dev_action,
      int16 script_timing
    )
{ 
  boolean status = TRUE;

  if (lna_cfg_p->rfm_device > RFM_MAX_WAN_DEVICES)
  { 
    RF_MSG_1(RF_ERROR, "rfdevice_grfc_elna_get_tech_port_info(): "
                       "invalid RFM device %d passed", lna_cfg_p->rfm_device);    
    return FALSE;
  }

  if( dev_action == RFDEVICE_CREATE_SCRIPT && buff_obj_ptr == NULL)
  {
    RF_MSG( RF_ERROR, "rfdevice_grfc_elna_param_validity() "
                      "Script not allocated for eLNA request");
    return FALSE; 
  }
  
  switch (lna_cfg_p->mode)
  {
    case RFCOM_WCDMA_MODE:
      /* check bands and bws supported */
      if(lna_cfg_p->band.wcdma_band >= RFCOM_NUM_WCDMA_BANDS)
      {
        RF_MSG_1( RF_ERROR, "rfdevice_grfc_elna_get_tech_port_info():"
                            "Invalid WCDMA band %d", lna_cfg_p->band.wcdma_band);
        return FALSE; 
      }
      break;

    case RFCOM_1X_MODE:
    case RFCOM_1XEVDO_MODE:
      /* check bands and bws supported */
      if(lna_cfg_p->band.cdma_band >= RFM_CDMA_MAX_BAND)
      {
        RF_MSG_1( RF_ERROR, "rfdevice_grfc_elna_get_tech_port_info():"
                            "Invalid CDMA band %d", lna_cfg_p->band.cdma_band);
        return FALSE; 
      }
      break;

    case RFCOM_GSM_MODE:
      /* check bands and bws supported */
      if(lna_cfg_p->band.gsm_band >= RFCOM_NUM_GSM_BANDS)
      {
        RF_MSG_1( RF_ERROR, "rfdevice_grfc_elna_get_tech_port_info():"
                            "Invalid GSM band %d", lna_cfg_p->band.gsm_band);
        return FALSE; 
      }
      break;      


    case RFCOM_LTE_MODE:
      /* check bands and bws supported */
      if(lna_cfg_p->band.lte_band >= RFCOM_NUM_LTE_BANDS)
      {
        RF_MSG_1( RF_ERROR, "rfdevice_grfc_elna_get_tech_port_info():"
                            "Invalid LTE band %d", lna_cfg_p->band.lte_band);
        return FALSE; 
      }
      break;


    case RFCOM_TDSCDMA_MODE:
      /* check bands and bws supported */
      if(lna_cfg_p->band.tdscdma_band >= RFCOM_NUM_TDSCDMA_BANDS)
      {
        RF_MSG_1( RF_ERROR, "rfdevice_grfc_elna_get_tech_port_info():"
                            "Invalid TDSCDMA band %d", lna_cfg_p->band.tdscdma_band);
        return FALSE; 
      }
      break;

    default:
      RF_MSG_1(RF_ERROR, "rfdevice_grfc_elna_get_tech_port_info():"
                         "ELNA request for invalid tech %d", lna_cfg_p->mode);
      status = FALSE;
      break;
  }
  return status;
}


/*!
  @brief
  The function is used to process the GRFC table based on the direction
  
  @details
  rfdevice_grfc_elna_process_grfc_tbl()

  @param buff_obj_ptr
  void pointer to CCS buffer object

  @param dev_action
  indicates if it is immediate write to hw or build scripts    

  @script_timing
  timing in us

  @param rfc_grfc_tbl
  pointer to the elna grfc table

  @param rfdevice_lna_cfg
  pointer to a lna cfg object which contains lna specific config info

  @return
  boolean indicating pass or fail
*/
boolean rfdevice_grfc_elna::rfdevice_grfc_elna_process_grfc_tbl
( 
  rfdevice_elna_req_enum_type grfc_elna_req,
  rf_buffer_intf *buff_obj_ptr, 
  rf_device_execution_type dev_action, 
  int16 script_timing,
  rfdevice_elna_grfc_rfc_tbl_type *rfc_grfc_tbl_ptr,
  rfdevice_lna_cfg* lna_cfg_p
  
)
{
  boolean status = TRUE;
  uint8 grfc_count = 0;
  int32 signal_num = -1;
  rfc_logic_type dir_logic = RFC_LOGIC_INVALID;

  if(( rfc_grfc_tbl_ptr == NULL ) || (grfc_elna_req == RFDEVICE_ELNA_INVALID))
  {
    RF_MSG(RF_HIGH,"rfdevice_grfc_elna_process_grfc_tbl(): Invalid parameters passed"
                   "returning FALSE");
    return FALSE;
  }

  for (grfc_count = 0; grfc_count < rfc_grfc_tbl_ptr->num_grfc; grfc_count++)
  {  
    signal_num = rfc_common_get_grfc_num(rfc_grfc_tbl_ptr->rfc_grfc_data[grfc_count].grfc_num);

    if(signal_num >= 0)
    {
      if (rfc_grfc_tbl_ptr->rfc_grfc_data[grfc_count].valid_flag == TRUE)
      {
        if (grfc_elna_req == RFDEVICE_ELNA_GAIN)
        {
          dir_logic = (rfc_logic_type)rfc_grfc_tbl_ptr->rfc_grfc_data[grfc_count].gain_states[(uint8)lna_cfg_p->lna_gain];
        }
        else if (grfc_elna_req == RFDEVICE_ELNA_SLEEP)
        {
          dir_logic = (rfc_logic_type)rfc_grfc_tbl_ptr->rfc_grfc_data[grfc_count].sleep;
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
          RF_MSG_1(RF_ERROR,"rfdevice_grfc_elna_process_grfc_tbl(): "
                            "buff_obj_ptr is NULL in dev action: %d",dev_action);  
          status &= FALSE;
        }
      }
      else
      {
        RF_MSG_2(RF_HIGH,"rfdevice_grfc_elna_process_grfc_tbl():"
                         "Invalid rfc data for grfc_count %d out of total GRFCs %d"
                        ,grfc_count,rfc_grfc_tbl_ptr->num_grfc);
        status &= FALSE;
      }
  
    }
    else
    {
      RF_MSG_3(RF_ERROR,"rfdevice_grfc_elna_process_grfc_tbl():"
                        "signal_num:%d for grfc_num:%d is invalid with dev action: %d",
                 signal_num, 
                 rfc_grfc_tbl_ptr->rfc_grfc_data[grfc_count].grfc_num,
                 dev_action);
      status &= FALSE;
    }
  }

  return status;
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
boolean rfdevice_grfc_elna::rfdevice_grfc_elna_get_band_map_grfc_tbl
(
  rfm_device_enum_type rfm_device,
  rfcom_mode_enum_type mode,
  rfcom_band_type_u band,
  rfdevice_elna_grfc_rfc_tbl_type **rfc_grfc_tbl
)
{

  boolean status = TRUE;

  if (rfm_device > RFM_MAX_WAN_DEVICES)
  { 
    RF_MSG_1(RF_ERROR, "rfdevice_grfc_elna_get_band_map_grfc_tbl(): "
                       "invalid RFM device %d passed", rfm_device);    
    return FALSE;
  }

  switch (mode)
  {

    case RFCOM_WCDMA_MODE:
      if(band.wcdma_band < RFCOM_NUM_WCDMA_BANDS)
      {
        *rfc_grfc_tbl = &(this->band_map_grfc_tbl.wcdma[rfm_device][band.wcdma_band]);
      }
      else
      {
        RF_MSG_1(RF_ERROR, "rfdevice_grfc_elna_get_band_map_grfc_tbl(): "
                           "GRFC elna rfdevice_grfc_elna_get_band_map_grfc_tbl invalid WCDMA band %d",
                           band.wcdma_band);    
        status = FALSE;
      }
      break;


    case RFCOM_1X_MODE:
    case RFCOM_1XEVDO_MODE:
      if (band.cdma_band < RFM_CDMA_MAX_BAND)
      {
        *rfc_grfc_tbl = &(this->band_map_grfc_tbl.cdma[rfm_device][band.cdma_band]);
      }
      else
      {
        RF_MSG_1(RF_ERROR, "rfdevice_grfc_elna_get_band_map_grfc_tbl():"
                           "GRFC elna rfdevice_grfc_elna_get_band_map_grfc_tbl invalid CDMA band %d",
                           band.cdma_band);  
        status = FALSE;
      }
      break;


    case RFCOM_GSM_MODE:
      if (band.gsm_band < RFCOM_NUM_GSM_BANDS)
      {
        *rfc_grfc_tbl = &(this->band_map_grfc_tbl.gsm[rfm_device][band.gsm_band]);
      }
      else
      {
        RF_MSG_1(RF_ERROR, "rfdevice_grfc_elna_get_band_map_grfc_tbl():"
                           "GRFC elna rfdevice_grfc_elna_get_band_map_grfc_tbl invalid GSM band %d",
                           band.gsm_band); 
        status = FALSE;
      }
      break;      

    case RFCOM_LTE_MODE:
      if (band.lte_band < RFCOM_NUM_LTE_BANDS)
      {
        *rfc_grfc_tbl = &(this->band_map_grfc_tbl.lte[rfm_device][band.lte_band]);
      }
      else
      {
        RF_MSG_1(RF_ERROR, "rfdevice_grfc_elna_get_band_map_grfc_tbl():"
                           "GRFC elna rfdevice_grfc_elna_get_band_map_grfc_tbl invalid LTE band %d", 
                           band.lte_band); 
        status = FALSE;
      }
      break;

    case RFCOM_TDSCDMA_MODE:
      if (band.tdscdma_band < RFCOM_NUM_TDSCDMA_BANDS)
      {
        *rfc_grfc_tbl = &(this->band_map_grfc_tbl.tdscdma[rfm_device][band.tdscdma_band]);      }
      else
      {
        RF_MSG_1(RF_ERROR, "rfdevice_grfc_elna_get_band_map_grfc_tbl():"
                           "GRFC elna rfdevice_grfc_elna_get_band_map_grfc_tbl invalid TDSCDMA band %d",
                           band.tdscdma_band);    
        status = FALSE;
      }
      break;

    default:
      RF_MSG_1(RF_ERROR, "rfdevice_grfc_elna_get_band_map_grfc_tbl():"
                         "GRFC elna rfdevice_grfc_elna_get_band_map_grfc_tbl invalid tech %d",mode);
      status = FALSE;
      break;
  }
  return status;
}

