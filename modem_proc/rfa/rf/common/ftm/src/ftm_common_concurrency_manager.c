/*!
   @file
   ftm_common_concurrency_manager.c

   @brief
 
*/

/*==============================================================================

  Copyright (c) 2014 - 2015 QUALCOMM Technologies Incorporated. All Rights Reserved

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

                       EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

$Header: 

when       who     what, where, why
--------------------------------------------------------------------------------
10/09/15  vkm     FTM concurrency manager changes for handling device allocationwith Rx/Tx split
10/01/15   kg      Generalize the rx_op enum to account for tx rfm_devices
07/14/15   Saul    Access rfc device properties using ptr.
07/08/15   ck/kg/zhw Avoid checkign against same device that's already assigned
07/10/15   zhw     KW Fix
07/08/15   ck/kg/zhw Provide concurrency check APIs from FTM Con- Manager
07/06/15   zhw     Added support for FTM Device Manager
06/11/15   ck      Hook ftm concurrency manager cleanup to FTM RF exit mode command  
06/10/15   ck      Optimze ftm ccmgr cleanup api
06/01/15   zhw     Bypass FTM concurrency manager until format is fixed
04/30/15   Saul    CRAT Updates
04/10/15   utk     Fix to copy rx_operation_type parameter before calling Concurrency manager
02/21/15   ck      Bypass reconfig request for LTE CA mode and band/tech transition 
02/13/15   ck      Workaround to inform rfc ccmgr still even fails to fix incapability of pcell reconfig 
02/11/15   rmb     Update FTM Conc manager state machine if the api returns success.
11/19/14   ck      Add tech protection for ftm ccmgr cleanup
11/13/14   ck      Bypass concurrency management in cal mode and fix implicit declaration 
11/10/14   kg      Add ftm api for setting concurrency manager debug mode
10/22/14   ck      Add allocation status cleanup api
10/22/14   ck      Fix to update alt_path information 
10/22/14   ck      Wrap parameters into a whole struct for future flexiblity
10/16/14   ck      Correct typo from RFM_WLAN_MODES to RFM_ALL_MODES 
10/10/14   ck      Fix DR band support for RxD 
10/03/14   ck      Fix incomplete search criteria through restriction table and incorrect initialization
09/30/14   ck      Fix warnings
09/30/14   ck      Initial revision
 
==============================================================================*/



#include "ftm_common_control.h"
#include "rfc_common.h"
#include "stringl.h" /* for memscpy */
#include "rfm_device_config_type.h"
#include "rfcommon_core_utils.h"
#include "ftm_common_concurrency_manager.h"
#include "rfm.h"
#include "rfcommon_concurrency_manager.h"
#include "ftm_msg.h"
#include "rfc_card.h"

static rfm_device_allocation_type static_dev_allocation = 
{
  {
	{RFM_INVALID_MODE,RFM_DEVICE_ALLOCATION_INVALID, SYS_BAND_CLASS_NONE,0, RFM_INVALID_OPERATION, RFM_INVALID_SUB_ID},			
	{RFM_INVALID_MODE,RFM_DEVICE_ALLOCATION_INVALID, SYS_BAND_CLASS_NONE,0, RFM_INVALID_OPERATION, RFM_INVALID_SUB_ID},			
	{RFM_INVALID_MODE,RFM_DEVICE_ALLOCATION_INVALID, SYS_BAND_CLASS_NONE,0, RFM_INVALID_OPERATION, RFM_INVALID_SUB_ID},			
	{RFM_INVALID_MODE,RFM_DEVICE_ALLOCATION_INVALID, SYS_BAND_CLASS_NONE,0, RFM_INVALID_OPERATION, RFM_INVALID_SUB_ID},			
	{RFM_INVALID_MODE,RFM_DEVICE_ALLOCATION_INVALID, SYS_BAND_CLASS_NONE,0, RFM_INVALID_OPERATION, RFM_INVALID_SUB_ID},			
	{RFM_INVALID_MODE,RFM_DEVICE_ALLOCATION_INVALID, SYS_BAND_CLASS_NONE,0, RFM_INVALID_OPERATION, RFM_INVALID_SUB_ID},			
	{RFM_INVALID_MODE,RFM_DEVICE_ALLOCATION_INVALID, SYS_BAND_CLASS_NONE,0, RFM_INVALID_OPERATION, RFM_INVALID_SUB_ID},			
	{RFM_INVALID_MODE,RFM_DEVICE_ALLOCATION_INVALID, SYS_BAND_CLASS_NONE,0, RFM_INVALID_OPERATION, RFM_INVALID_SUB_ID},
	{RFM_INVALID_MODE,RFM_DEVICE_ALLOCATION_INVALID, SYS_BAND_CLASS_NONE,0, RFM_INVALID_OPERATION, RFM_INVALID_SUB_ID},	
	{RFM_INVALID_MODE,RFM_DEVICE_ALLOCATION_INVALID, SYS_BAND_CLASS_NONE,0, RFM_INVALID_OPERATION, RFM_INVALID_SUB_ID},
  }
};

/*! 
  @brief
  FTM interface to concurrency manager and check combination validity
 
  @details
  Function is to offer an interface in FTM to concurrency manager, including
  static and dynamic concurrency validation. Also maintain a static variable
  to track status on all rfm_dev
 
  @param rfm_device_enum_type rfm_dev,
  current rfm_dev to enter
 
  @param rfcom_band_type_u band,
  current band to use
 
  @param rfm_mode_enum_type tech,
  the target tech to enter
 
  @param rfc_rx_path_type prx_drx
  primary or diversity
 
 
  @return
  TRUE indicates validation is passed
*/

boolean ftm_concurrency_manager_validate_operation( ftm_concurrency_manager_params_type *params)
{
  const rfm_devices_configuration_type* dev_cfg;
  boolean status = TRUE;
  rfm_bands_bitmask bands_supported;
  rfm_device_allocation_status_type* allocation_status;
  rfm_device_client_info_type dev_client;
  uint8 mask_index = 0;
  uint8 mask_bit_num = 0;
  rfm_device_enum_type dev_index = 0;
  uint16 res_table_index = 0;
  sys_band_class_e_type sys_band = SYS_BAND_CLASS_NONE;
  uint64 dev_cfg1_band_mask1 = 0;	
  uint64 dev_cfg1_band_mask2 = 0;
  uint32 dev_cfg1_dev_mask = 0;
  uint64 dev_cfg2_band_mask1 = 0;	
  uint64 dev_cfg2_band_mask2 = 0;
  uint32 dev_cfg2_dev_mask = 0;
  rfm_device_enum_type test_dev1 = RFM_INVALID_DEVICE;	
  uint32 test_band1 = 0;	
  rfm_device_enum_type test_dev2 = RFM_INVALID_DEVICE;	
  uint32 test_band2 = 0;

  /* if in calibration, concurrency should be bypassed */ 
  if(rfm_get_calibration_state())
  {    
    MSG(MSG_SSID_FTM, MSG_LEGACY_HIGH, "ftm concurrency management is bypassed in calibration state"); 
    return TRUE;
  }

  /* parameter boundary check */ 
  if(params->rfm_dev >= RFM_MAX_WAN_DEVICES)
  {
    MSG_1(MSG_SSID_FTM, MSG_LEGACY_ERROR, "fail to validate cocurrency due to invalid dev %d", params->rfm_dev); 
    return FALSE;
  }


   /*step0: band type conversion rfcom_band to sys_band in rfcommon_core_utils.c */ 
  sys_band = rfcommon_core_convert_rfcom_band_to_sys_band( params->tech, params->band);

  MSG_3(MSG_SSID_FTM, MSG_LEGACY_MED, "INFO: Enter concurrency manager with tech = %d, sys_band = %d on dev %d", params->tech, sys_band, params->rfm_dev);
														  

  /*Step 1: Verify if the band is supported in the mask array, based on band enum value*/

  dev_cfg = rfc_get_logical_device_properties();
  if (dev_cfg == NULL)
  {
    RF_MSG( RF_ERROR, 
           "ftm_concurrency_manager_validate_operation NULL ptr." );
    return FALSE;
  }

  /*if ( (RFM_DEVICE_2 == params->rfm_dev)  && ((dev_cfg->concurrency_features / RFM_DUAL_RX_DSDS_SUPPORTED)& 0x1) )
  {
    bands_supported = dev_cfg->device_support[params->rfm_dev].dr_bands_supported;
  }
  else if(params->carrier_index != 0)
  {
    bands_supported = dev_cfg->device_support[params->rfm_dev].ca1_bands_supported;
  }
  else*/
  {
    bands_supported = dev_cfg->device_support[params->rfm_dev].bands_supported;
  }

  mask_bit_num = (sizeof(bands_supported.mask[0])*8);
  mask_index = (uint8)sys_band/mask_bit_num;
  if( !( (bands_supported.mask[mask_index]>>(sys_band % mask_bit_num))&0x1 ) )
  {
	  MSG_2(MSG_SSID_FTM, MSG_LEGACY_ERROR, "unsupported band %d on rfm_dev %d", sys_band, params->rfm_dev);
	  status &= FALSE;
  }

  MSG_3(MSG_SSID_FTM, MSG_LEGACY_MED, "INFO: supported_band_mask[%d] = %d; sys_band mod 64 = %d", mask_index, bands_supported.mask[mask_index], sys_band % mask_bit_num);
  
  /*Step 2: Parse whether there are any static sconcurrency restrictions 
    Need to process dev_cfg->concurrency_restrictions */

	test_dev2 = params->rfm_dev;	
  test_band2 = sys_band;
  for(dev_index = 0; dev_index < RFM_MAX_WAN_DEVICES; dev_index++)
  {
    if(test_dev2 == dev_index) continue;
    if(static_dev_allocation.device_allocation[dev_index].allocation_type == RFM_DEVICE_ASSIGNED)
    {
      test_band1 = static_dev_allocation.device_allocation[dev_index].band;
      /* KW Fix, test_band1 cannot be bigger than 191 */
      if ( test_band1 > 191 )
      {
        MSG_1(MSG_SSID_FTM, MSG_LEGACY_ERROR, "INFO: Invalid test_band1 value %d"
                                              "May result in bad memorry access."
                                              "Abort operation",
              test_band1);
        break;
      }
      
      test_dev1 = dev_index; 
      MSG_2(MSG_SSID_FTM, MSG_LEGACY_MED, "INFO: discover assigned device on dev_index = %d, band = %d",
            test_dev1,test_band1);
      
        for (res_table_index = 0; res_table_index < dev_cfg->concurrency_restrictions.num_restrictions; res_table_index++) 
        {
            
        dev_cfg1_band_mask1 = dev_cfg->concurrency_restrictions.restriction_table[res_table_index].band_group1.mask[test_band1/mask_bit_num];

        dev_cfg1_band_mask2 = dev_cfg->concurrency_restrictions.restriction_table[res_table_index].band_group1.mask[test_band2/mask_bit_num];
        dev_cfg1_dev_mask = dev_cfg->concurrency_restrictions.restriction_table[res_table_index].device_group1;

        dev_cfg2_band_mask1 = dev_cfg->concurrency_restrictions.restriction_table[res_table_index].band_group2.mask[test_band1/mask_bit_num];
        dev_cfg2_band_mask2 = dev_cfg->concurrency_restrictions.restriction_table[res_table_index].band_group2.mask[test_band2/mask_bit_num];
        dev_cfg2_dev_mask = dev_cfg->concurrency_restrictions.restriction_table[res_table_index].device_group2;

				
				MSG_5(MSG_SSID_FTM, MSG_LEGACY_MED, "debug: %d %d %d %d %d ",
					res_table_index, dev_cfg1_band_mask1,dev_cfg1_dev_mask,
					dev_cfg2_band_mask2, dev_cfg2_dev_mask);
              
                if (
            (( ( (dev_cfg1_dev_mask >> test_dev1) & 0x1)
                && ((dev_cfg1_band_mask1>>(test_band1 % mask_bit_num))&0x1 ))               
                &&(( (dev_cfg2_dev_mask >> test_dev2) & 0x1 )
                &&( (dev_cfg2_band_mask2>>(test_band2 % mask_bit_num))&0x1 )))

	    || (
	       ( ( (dev_cfg2_dev_mask >> test_dev1) & 0x1)
                && ((dev_cfg2_band_mask1>>(test_band1 % mask_bit_num))&0x1 ))               
                &&(( (dev_cfg1_dev_mask >> test_dev2) & 0x1 )
                &&( (dev_cfg1_band_mask2>>(test_band2 % mask_bit_num))&0x1 ))
                   )

        )
                {
                if( (dev_cfg->concurrency_restrictions.restriction_table[res_table_index].ignore_same_band_restriction)
                && ((test_dev1 != test_dev2) && (test_band1 == test_band2)))
                  {status &= TRUE;}
                else {status &= FALSE;}
          MSG_5(MSG_SSID_FTM, MSG_LEGACY_ERROR, "restriction table index %d blocks the combination:(dev1 = %d, band1 = %d) + (dev2 = %d, band2 = %d) ", 
                res_table_index,  
                test_dev2, 
                test_band2,
                test_dev1,
                test_band1);

                }

        }
    }
  }
  


  /* Step 3: Validate Device concurrency */
  dev_client.band = sys_band;
  dev_client.rfm_dev = params->rfm_dev;
  dev_client.tech = params->tech;
  dev_client.rx_operation = (params->prx_drx == RFC_PRX_PATH)? RFM_PRX_OPERATION : RFM_DRX_OPERATION;
  allocation_status = rfm_verify_rf_concurrency(&dev_client);

  /* Loop through the curr_status of each rfm_dev from allocation_status and return fail if any of the devices need to be reconfigured*/
  for(dev_index = 0; dev_index < RFM_MAX_WAN_DEVICES; dev_index++)
  {

    /* bypass tech and band transition reconfig request on the same device */ 
    if(dev_index != dev_client.rfm_dev)
    {
      if (allocation_status->curr_status[dev_index] == RFM_DEVICE_RECONFIG_REQUIRED ) 
      {
	/* LTE should bypass reconfig request since LTE could handle reconfiguration */ 
	if( (params->tech == RFM_LTE_MODE) && (static_dev_allocation.device_allocation[dev_index].tech == RFM_LTE_MODE))
	{

	  status &= TRUE; 	  	  
          MSG_3(MSG_SSID_FTM, MSG_LEGACY_MED, "Allow rfconfiguration for LTE mode", 
                                               params->tech,sys_band, params->rfm_dev );
	}
        else
        {
          status &= FALSE;
          MSG_3(MSG_SSID_FTM, MSG_LEGACY_ERROR, "tech %d on dev %d for band %d needs reconfiguration",
	      	  static_dev_allocation.device_allocation[dev_index].tech,  
	      	  dev_index, 
	      	  static_dev_allocation.device_allocation[dev_index].band);
          /* notice that alt_path is not maintained in FTM concurrency manager. Only maintained in rfcmn_concurrency_db */ 		        
        }
	  }
		else
		{		  
		  MSG_2(MSG_SSID_FTM, MSG_LEGACY_MED, "INFO: allocation status %d on device %d and does not require reconfiguration", 
				allocation_status->curr_status[dev_index],
				dev_index);
        status &= TRUE;
      }		
    }
    else
    {
      status &= TRUE;
      MSG_1(MSG_SSID_FTM, MSG_LEGACY_MED, "Ignore reconfig on the requested device %d ", dev_index );      
		}		
  }

  
#if 0
  /* Step 4: Inform device allocation */
  if(status)
  {
    static_dev_allocation.device_allocation[params->rfm_dev].tech = params->tech;
    static_dev_allocation.device_allocation[params->rfm_dev].band = sys_band;
    static_dev_allocation.device_allocation[params->rfm_dev].allocation_type = RFM_DEVICE_ASSIGNED;
    static_dev_allocation.device_allocation[params->rfm_dev].rx_operation = (rfm_device_operation_type)params->prx_drx;
	
    /* this alt_path update currently does not take real effect since rfc concurrency manager will recalculate */ 
    static_dev_allocation.device_allocation[params->rfm_dev].alt_path = allocation_status->alt_path[params->rfm_dev];
    if ( !rfm_inform_device_allocation(&static_dev_allocation))
    {
      MSG(MSG_SSID_FTM, MSG_LEGACY_ERROR, "fail to inform device allocation!");          
      static_dev_allocation.device_allocation[params->rfm_dev].allocation_type = RFM_DEVICE_ALLOCATION_INVALID;
      status &= FALSE;
    }
  }
#endif  

  MSG_6(MSG_SSID_FTM, MSG_LEGACY_MED, "INFO: exiting concurrency manager with status %d, updated tech = %d , band = %d, dev = %d, alt_path = %d, and allocation result = %d; ", 
		status,
  	static_dev_allocation.device_allocation[params->rfm_dev].tech,  
  	static_dev_allocation.device_allocation[params->rfm_dev].band,
  	params->rfm_dev,
  	static_dev_allocation.device_allocation[params->rfm_dev].alt_path,
  	static_dev_allocation.device_allocation[params->rfm_dev].allocation_type);

  return status;
}




/*! 
  @brief
  FTM interface to clean up allocation status when tech exits
  
  @param rfm_device_enum_type rfm_dev,
  current rfm_dev to enter
 
  @return
  TRUE indicates validation is passed
*/

boolean ftm_concurrency_manager_cleanup_device_allocation( rfm_device_enum_type rfm_dev, rfm_mode_enum_type cur_tech )
{

  /* if in calibration, concurrency should be bypassed */ 
  if(rfm_get_calibration_state())
  {    
    MSG(MSG_SSID_FTM, MSG_LEGACY_HIGH, "ftm concurrency management is bypassed in calibration state"); 
    return TRUE;
  }

  /* do not allow other tech except itself to request cleanup */
  if( ((cur_tech == static_dev_allocation.device_allocation[rfm_dev].tech) 
   || (RFM_INVALID_MODE == static_dev_allocation.device_allocation[rfm_dev].tech))
   || (cur_tech == RFM_ALL_MODES) )
  {
    static_dev_allocation.device_allocation[rfm_dev].tech = RFM_INVALID_MODE;
    static_dev_allocation.device_allocation[rfm_dev].band = SYS_BAND_CLASS_NONE;
    static_dev_allocation.device_allocation[rfm_dev].allocation_type = RFM_DEVICE_ALLOCATION_INVALID;
    static_dev_allocation.device_allocation[rfm_dev].alt_path = 0;
  }
  else
  {
    if (!(cur_tech == static_dev_allocation.device_allocation[rfm_dev].tech))
    {
      MSG_2(MSG_SSID_FTM, MSG_LEGACY_ERROR, "cleanup is not allowed by cur_tech = %d. Only tech %d is permitted", cur_tech, static_dev_allocation.device_allocation[rfm_dev].tech);     
    }
    if (!(RFM_INVALID_MODE == static_dev_allocation.device_allocation[rfm_dev].tech))
    {
      MSG_1(MSG_SSID_FTM, MSG_LEGACY_ERROR, "dev %d is already cleaned", rfm_dev);     
    }

    return FALSE;
  }
  

  if ( rfcmn_concurrency_mgr_update_rx_state(rfm_dev, RFM_INVALID_MODE, 0) !=  RFCMN_CONCURRENCY_MGR_UPDATE_SUCCESS)
  {
    MSG_1(MSG_SSID_FTM, MSG_LEGACY_ERROR, "fail to inform device allocation on dev %d", rfm_dev); 
    return FALSE;
  }
  else
  {
    MSG_2(MSG_SSID_FTM, MSG_LEGACY_MED, "INFO: clean up allocation status on dev %d for tech %d ", rfm_dev, cur_tech);
    return TRUE;
  }  



}

/*! 
  @brief
  FTM interface to set the debug flags for concurrency manager module
  
  @param *ftm_req_data
  FTM Request Data to configure CDMA Debug mode

  @return
  Response packet for the command
*/
ftm_rsp_pkt_type
ftm_concurrency_manager_configure_debug_mode
(
ftm_common_pkt_type *ftm_req_data
)
{
	ftm_concurrency_mgr_debug_mode_request_packet_type *header =
		(ftm_concurrency_mgr_debug_mode_request_packet_type *)ftm_req_data;

	ftm_rsp_pkt_type ftm_rsp_data;
	ftm_concurrency_mgr_debug_mode_response_packet_type
		*ftm_concurrency_mgr_rsp;

  boolean debug_en;
	boolean ret_val;
  uint32 debug_mask;
	MSG(MSG_SSID_FTM, MSG_LEGACY_HIGH, "ftm_concurrency_manager_configure_debug_mode called");    

  debug_en = (boolean)header->debug_en;
  debug_mask = header->debug_mask;

	ret_val = rfcmn_concurrency_mgr_set_debug_mode(debug_mask, debug_en);
	MSG(MSG_SSID_FTM, MSG_LEGACY_ERROR, "back from debug api");    

	ftm_rsp_data = ftmdiag_create_new_pkt( sizeof (ftm_concurrency_mgr_debug_mode_response_packet_type) );  

	if( ftm_rsp_data.pkt_payload != NULL )
  {
    ftm_concurrency_mgr_rsp = ftm_rsp_data.pkt_payload;
		// Copy data from request to response
		memscpy(ftm_concurrency_mgr_rsp, sizeof(ftm_composite_cmd_header_type), 
						ftm_req_data, sizeof(ftm_composite_cmd_header_type));
		ftm_concurrency_mgr_rsp->ftm_concurrency_mgr_status = (uint32)ret_val;
  }
  else
  {
		MSG(MSG_SSID_FTM, MSG_LEGACY_HIGH, "ftm_concurrency_manager_configure_debug_mode : ftm_rsp_data is NULL");    
  }

	return ftm_rsp_data;

}

boolean
ftm_concurrency_manager_check_band_conflict
(
   rfm_device_enum_type dev_1 , 
   sys_band_class_e_type dev_1_sys_band, 
   rfm_device_enum_type dev_2 , 
   sys_band_class_e_type dev_2_sys_band
) 
{
  const rfm_devices_configuration_type* dev_cfg;
  boolean api_status = TRUE;
  boolean restriction_found = FALSE;

  uint8 dev_1_band_mask_group = 0;
  uint8 dev_2_band_mask_group = 0;

  dev_cfg = rfc_get_logical_device_properties();
  if (dev_cfg == NULL)
  {
    RF_MSG( RF_ERROR, 
           "ftm_concurrency_manager_check_band_conflict NULL ptr." );
    return FALSE;
  }

  if ((dev_1_sys_band >= 0) && (dev_1_sys_band <= 63)) 
  {
    dev_1_band_mask_group = 0;
  }
  else if ( ( dev_1_sys_band > 63 ) && (dev_1_sys_band <= 127) )
  {
    dev_1_band_mask_group = 1;
  }
  else if ( ( dev_1_sys_band > 127 ) && (dev_1_sys_band <= 191) )
  {
    dev_1_band_mask_group = 2;
  }
  else
  {
    api_status = FALSE;
  }

  if ((dev_2_sys_band >= 0) && (dev_2_sys_band <= 63)) 
  {
    dev_2_band_mask_group = 0;
  }
  else if ( ( dev_2_sys_band > 63 ) && (dev_2_sys_band <= 127) )
  {
    dev_2_band_mask_group = 1;
  }
  else if ( ( dev_2_sys_band > 127 ) && (dev_2_sys_band <= 191) )
  {
    dev_2_band_mask_group = 2;
  }
  else
  {
    api_status = FALSE;
  }

  if ( api_status == TRUE )
  {
    uint8 res_table_index;

    for ( res_table_index = 0; 
          res_table_index < dev_cfg->concurrency_restrictions.num_restrictions; 
          res_table_index++ ) 
    {
      const rfm_concurrency_restriction_type* res_tbl_ptr = 
        &(dev_cfg->concurrency_restrictions.restriction_table[res_table_index]);

      FTM_MSG_5(FTM_HIGH,"ftm_concurrency: checking restriction idx %d "
                         "for device_1 %d sys_band %d, "
                         "with device_2 %d sys_band %d ",
                          res_table_index, dev_1, dev_1_sys_band, 
                          dev_2, dev_2_sys_band );

      if ( ( ( (res_tbl_ptr->device_group1 & ((uint32)1 << dev_1 ) )!= 0 ) &&
          ( (res_tbl_ptr->device_group2 & ((uint32)1 << dev_2) ) != 0 ) &&
          ( (res_tbl_ptr->band_group1.mask[dev_1_band_mask_group] & 
            ( (uint64)1 << ( dev_1_sys_band - dev_1_band_mask_group * 64 ) ) ) != 0 )  &&
          ( (res_tbl_ptr->band_group2.mask[dev_2_band_mask_group] & 
            ( (uint64)1 << ( dev_2_sys_band - dev_2_band_mask_group * 64 ) ) ) != 0  ) ) ||         

        ( ( (res_tbl_ptr->device_group1 & ((uint32)1 << dev_2 ) )!= 0 ) &&
            ( (res_tbl_ptr->device_group2 & ((uint32)1 << dev_1) ) != 0 ) &&
            ( (res_tbl_ptr->band_group1.mask[dev_2_band_mask_group] & 
              ( (uint64)1 << ( dev_2_sys_band - dev_2_band_mask_group * 64 ) ) ) != 0 )  &&
            ( (res_tbl_ptr->band_group2.mask[dev_1_band_mask_group] & 
              ( (uint64)1 << ( dev_1_sys_band - dev_1_band_mask_group * 64 ) ) ) != 0  ) ) )
      {
        restriction_found = TRUE;
        FTM_MSG_5(FTM_HIGH,"ftm_concurrency: Restriction found in idx %d "
                           "for device_1 %d sys_band %d, "
                           "with device_2 %d sys_band %d ",
                           res_table_index, dev_1, dev_1_sys_band, 
                           dev_2, dev_2_sys_band );
        break;
      }
    }
  }
  else
  {
    FTM_MSG(FTM_ERROR,"ftm_concurrency: error in restriction check" );
  }

  return restriction_found;
}

/*------------------------------------------------------------------------------*/
/*! 
  @brief
  FTM command Api to register the second coming tech beforehand 
  force 1st tech to apply correct alt_path to support drds reconfiguration
  
  @return
  ftm response packet
*/

ftm_rsp_pkt_type ftm_common_drds_reconfig_enable(ftm_common_pkt_type *ftm_req_data)
{
  ftm_drds_reconfig_enable_pkt_type* ftm_drds_reconfig_enable_data = (ftm_drds_reconfig_enable_pkt_type*)(&(ftm_req_data->ftm_extended_params));
  ftm_rsp_pkt_type rsp = {FTM_RSP_DO_LEGACY, 0, ftm_req_data, FALSE};
  rfcom_band_type_u rfcom_band_dev2;    
  rfcom_band_type_u rfcom_band_dev0;  
  sys_band_class_e_type sys_band_dev2 = SYS_BAND_CLASS_NONE;
  rfm_mode_enum_type mode_dev2 = RFM_NUM_MODES;
  rfm_mode_enum_type mode_dev0 = RFM_NUM_MODES;
  rfm_device_client_info_type dev_client;
  rfm_device_allocation_status_type* allocation_status;
  ftm_mode_id_type ftm_band_dev2 = (ftm_mode_id_type)ftm_drds_reconfig_enable_data->band2;  

  //set default error status to 0  
  ftm_drds_reconfig_enable_data->status = 0;
  
  MSG_5(MSG_SSID_FTM, MSG_LEGACY_MED,"Concurrency combination: tech1 = %d, band1 = %d, tech2 = %d, band2 =%d, status = %d",
  	ftm_drds_reconfig_enable_data->tech1,
  	ftm_drds_reconfig_enable_data->band1,
  	ftm_drds_reconfig_enable_data->tech2,
  	ftm_drds_reconfig_enable_data->band2,
  	ftm_drds_reconfig_enable_data->status);

  /* set response status to 1 */
    ftm_drds_reconfig_enable_data->status = 1;

  
  return rsp;
}

/*! 
  @brief
  FTM interface to concurrency manager to update device allocation
 
  @details
  Function will update the concurrency manager device database with the input details per device
 
  @param rfm_device_allocation_type *dev_allocation,
  current device allocation to update
 
  @return
  TRUE indicates updation was successful
*/

boolean ftm_concurrency_manager_update_device_allocation( rfm_device_allocation_type *dev_allocation )
{  
  boolean status = TRUE;
  rfm_device_enum_type dev_index = 0;
  
  /* if in calibration, concurrency should be bypassed */ 
  if(rfm_get_calibration_state())
  {    
    MSG(MSG_SSID_FTM, MSG_LEGACY_HIGH, "ftm concurrency management is bypassed in calibration state"); 
    return TRUE;
  }
  
  if (dev_allocation == NULL)
  {
    RF_MSG(RF_ERROR, "ftm_concurrency_manager_update_device_allocation: Invalid container"); 
    return FALSE; 
  }

  /* Update the global static dev allocation with the new request details */

  for (dev_index = 0; dev_index < RFM_MAX_WAN_DEVICES; dev_index++)
  {
    if ( dev_allocation->device_allocation[dev_index].allocation_type == RFM_DEVICE_ASSIGNED )
    {
      static_dev_allocation.device_allocation[dev_index].tech = dev_allocation->device_allocation[dev_index].tech;
      static_dev_allocation.device_allocation[dev_index].band = dev_allocation->device_allocation[dev_index].band;
      static_dev_allocation.device_allocation[dev_index].allocation_type = 
       (status == FALSE)?RFM_DEVICE_ALLOCATION_INVALID:dev_allocation->device_allocation[dev_index].allocation_type;
      /* this alt_path update currently does not take real effect since rfc concurrency manager will recalculate */ 
      static_dev_allocation.device_allocation[dev_index].alt_path = dev_allocation->device_allocation[dev_index].alt_path;
      static_dev_allocation.device_allocation[dev_index].rx_operation = dev_allocation->device_allocation[dev_index].rx_operation;
    }
  }
  
  if ( !rfm_inform_device_allocation(&static_dev_allocation) )
  {
    MSG(MSG_SSID_FTM, MSG_LEGACY_ERROR, "fail to inform device allocation!");          
    //static_dev_allocation.device_allocation[params->rfm_dev].allocation_type = RFM_DEVICE_ALLOCATION_INVALID;
    status &= FALSE;
  }

  return (status);
}
