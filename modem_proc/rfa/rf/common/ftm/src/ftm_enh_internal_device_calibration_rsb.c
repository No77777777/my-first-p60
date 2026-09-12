/*!
  @file
  ftm_enh_internal_device_calibration_rsb.c

  @brief

*/
/*==============================================================================

  Copyright (c) 2014 - 2021 Qualcomm Technologies Incorporated. All Rights Reserved

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

  $Header: //components/rel/rfa.mpss/4.11/rf/common/ftm/src/ftm_enh_internal_device_calibration_rsb.c#2 $

when       who     what, where, why
--------   ---     ------------------------------------------------------------- 
04/15/21  mzahra   add maximum check for num_gain_state
06/03/20   ssl     [CR2669493]Passing Sub_id as input argument to WTR APIs
01/20/20   ssl     FR 55657: Replace RF Enter/Exit with RF Sleep/Wakeup for L+L DSDS awake
03/23/16   sk      Use Tx band for feeback query and pass FB device from RFC
02/15/16   tik     Bug fix: Fix Post RxRSB measurements for DRx
11/02/15   xsh     Fix LTE post-rsb IQ capture Save over-ride pre-rsb IQ capture Save 
10/11/15  xshn    Added buffer NULL detection for LTE IQ simutenuously capture
10/11/15  xsh     WCDMA IQ capture update: Added carrier 1 IQ capture case
10/11/15  xsh     Add Error Code for rsb device api return invalid rsb data
10/01/15   xsh     STG identification and IQ capture  for STG support
09/25/15   xsh     clean code to split rxRSB and fbrxRsb API
09/25/15   xsh     LTE IQ simutenuously capture
09/24/15   sd      FR30081 add support for AsDiv alt gain param in device APIs
07/27/15   xsh     kw error fix
07/20/15   xsh     Fix rsb device map on th2.x for tx/rx split implementation, revert stw's modification on 6/24 because rsb ftm api from PC send device enum with old format for exisiting customer api maintence
06/24/15   stw     LTE_U: RSB Cal for LTE able to handle any carrier device mapping
06/17/15   stw     LTE_U : Fix for Band 252 , 255 : Interger Band to RF COM BAND conversion
06/15/15   jak     TDS FTM RSB cal
05/28/15   aro     Support to test ATLAS WCDMA driver in THOR baseline
04/27/15    br     Remove hardcoding while fetching tx_freq and band in WCDMA.
05/08/15   ak     [Rx/Tx Split]Enable 6 Rx Path Cal and include support to Cal upto MAX devices
04/16/15   kg     Support for re-programming ADC settings to compensate Vcm voltage on 4905
03/17/15   ms      TDSCDMA RX RSB cal fix
02/10/15    br     Send tech,band,bandwidth and fbrx gain state as inputs to stg enable and disable apis
11/05/14   xsh     Add scell2 rsb cal support
11/05/14   vs      Fix linker errors on W/T compile out flavor builds
11/03/14   ndb     Fix the compiler error in LTE removed builds
10/08/14    ka     Replace rfc with device api for retrieving device pointer
10/02/14   dps     Changed lna_gain_state to gain_state in rfdevice_rsb_cal_data_type
09/12/14   xsh    Move error log init, tech check before flag check so that flag=3 can reset the error status
09/02/14   dr      Added featurization for Non-W/TDS builds
08/29/14   xsh     Remove RFCal FW state update as it should be done in RF setup
08/29/14   xsh     Added deviceId setup when sending tds IQ capture msg to FW
08/29/14   xsh     Fixed compiler warning  
08/11/14   shb     Fixed offtarget compiler error
08/11/14   xsh     Re-set efs write function pointer
08/11/14   xsh     Revert efs save function pointer set as there is bug in those efs save function
08/11/14   xsh     Set efs write function pointer to what's provided by device driver
08/05/14   xsh     Seperate efs write from normal rsb cal
08/04/14   xsh     Extend wait time after IQMC updated
08/04/14   xsh     Remove Bolt Dependency to support other PLs
07/24/14   xsh     Add rsb cal validation check before update iqmc based on cal data
07/14/14   aa      Fix FBRx DC Cal failures due to incorrect condition buffer ID check
07/09/14   jmf     Perform RSB coeff calc with DC removed
07/08/14   jps     FTM RF Mode Rx1 IQ capture fix
07/07/14   xsh     Clean code
07/07/14   xsh     Add support for multiple gain rsb cal
07/01/14   xsh     Add debug level time profile
06/13/14   kab     Add 3xCA support
05/29/14   brath   Updates to provide TX LO frequency for device cal APIs
5/30/14    xsh     add different gain state rsb cal support
5/29/14    xsh     add lte scell rx rsb cal support
4/15/14    xsh     hooking to fbrx rsb cal to rx rsb cal
4/8/14     xsh     add after rsb cal -recal to get the rsb coef for limit computation
3/6/14     xsh     hooking rsb algorithm to SS
1/8/14     xsh     add rsb hard-code algorithm
11/25/13   xsh     Initial Revision  

==============================================================================*/

#ifndef FEATURE_GNSS_ONLY_NO_WWAN

#include "rfa_variation.h"
#include "comdef.h"
#include "modem_mem.h"
#include "string.h"
#include "stringl.h" /* for memscpy */
#include "fs_estdio.h"
#include "ftm_msg.h"
#include "rfm_mode_types.h"
#include "ftm_common_msm.h"
#include "rfcommon_msm.h"
#include "msm.h"
#include "ftm_common_control.h"
#include "ftm_common_data.h"
#include "ftm_common_enh_internal_device_cal.h"
#include "ftm_enh_internal_device_cal_rsb.h"
#include "ftm_enh_internal_device_cal_rsb_alg.h"
#include "rfcommon_fbrx_api.h"
#include "rfcommon_msm.h"
#include "rfcommon_core_utils.h"
#include "rfdevice_antenna_switch_intf.h"

#include "math.h"

#include "rfcommon_mdsp.h"

#ifdef FEATURE_LTE
#include "rflte_util.h"
#include "rflte_mc.h"
#include "rfdevice_lte_interface.h"
#include "rflte_ftm_mc.h"
#include "rflte_msm_iq_capture.h"
#include "rflte_state.h"
extern rflte_ftm_mc_sm_type rflte_ftm_mc_sm;
extern rflte_msm_iq_capture_state_type lte_iq_capture_state_machine;
#endif

#ifdef FEATURE_WCDMA
#include "rfwcdma_mc.h"
#ifdef ATLAS_RF_WCDMA_PRESI
#include "rfm_wcdma_ftm.h"
#include "rfa_wcdma_utilities.h"
#else
#include "rfwcdma_data.h"
#endif
#include "ftm_hwtc_wcdma_ino.h"
#include "rfdevice_wcdma_intf.h"
#include "rfwcdma_msm.h"
extern ftm_lm_buf_type ftm_wcdma_rxlm_buffer[RFM_MAX_WAN_DEVICES];
extern rfcom_wcdma_band_type ftm_curr_wcdma_mode;
extern ftm_hwtc_wcdma_data_type ftm_hwtc_wcdma_data;
#endif /* FEATURE_WCDMA */

#ifdef FEATURE_TDSCDMA
#include "rfdevice_tdscdma_intf.h"
#include "ftm_tdscdma_ctl.h"
#include "rf_tdscdma_mdsp.h"
#include "rf_tdscdma_core_util.h"
extern ftm_tdscdma_iq_capture_data_type ftm_tds_iq_capture_data;
extern ftm_lm_buf_type ftm_tdscdma_rxlm_buffer[2];
extern void ftm_tdscdma_util_convert_10pcfl_iq_data_format( uint32 *input_buf,
																  word *output_buf,
																  uint32 num_sample);
extern void ftm_tdscdma_util_convert_20pcfl_iq_data_format( uint32 *input_buf,
																  word *output_buf,
																  uint32 num_sample);
#endif

#ifdef FEATURE_CDMA1X
#include "rf_cdma_data.h"
#include "rf_cdma_utils_freq.h"
#endif

//#ifdef FEATURE_BOLT_MODEM
#include "rflm_api_fbrx_fw_hal.h"

//#endif

#ifdef FEATURE_FACTORY_TESTMODE


#define MAX_RSB_CAL_ELEMENT_LTE ((RFCOM_NUM_LTE_BANDS)*(RFM_MAX_WAN_DEVICES)*6)
#define MAX_RSB_CAL_ELEMENT_WCDMA ((RFCOM_NUM_WCDMA_BANDS)*(RFM_MAX_WAN_DEVICES)*4)
#define MAX_RSB_CAL_ELEMENT_TDSCDMA ((RFCOM_NUM_TDSCDMA_BANDS)*(RFM_MAX_WAN_DEVICES)*4)


typedef boolean (*ftm_enh_internal_device_cal_rsb_save_efs_fp)( rfm_device_enum_type rfm_device, 
                                                               uint8 rf_band, 
                                                               rfdevice_rsb_cal_verfication_data_type* rsb_ver_data);

boolean rsb_efs_empty_write( rfm_device_enum_type rfm_device,                                          
								uint8 rf_band, 
								rfdevice_rsb_cal_verfication_data_type* rsb_ver_data)
{
 	if(rsb_ver_data!=NULL)
 	{
		rsb_ver_data->num_calibrated_item =0;
 	}
	return TRUE;
}

static int16 sat_int32_to_int16(int32 in_val, uint8 * overflow_det)
{
  int16 out_val;
  int16 max_val = 0x7FFF;
  int16 min_val = 0x8000;
  *overflow_det = 0;
  if (in_val > (int32)max_val)
  {
    out_val = max_val;
    *overflow_det = 1;
    FTM_MSG_2(FTM_ERROR, "OVERFLOW DETECTED: %d > %d", in_val, max_val);
  }
  if (in_val < (int32)min_val)
  {
    out_val = min_val;
    *overflow_det = 1;
    FTM_MSG_2(FTM_ERROR, "OVERFLOW DETECTED: %d < %d", in_val, min_val);
  }

  if (*overflow_det == 0)
  {
    out_val = (int16)( in_val  & 0xFFFF );
  }
  return out_val;

}


/*----------------------------------------------------------------------------*/
/*!
  @brief: ftm_calv4_convert_band
  
 
  @details: convert band in ftm enum to tech-dependant enum
  
  @param 
	input:	ftm_rf_tech, rf_band
	output: tech dependant band enum
	  
  @return
  Status of initialization
*/

uint16 ftm_calv4_convert_band
(
  ftm_rf_technology_type ftm_rf_tech,
  uint16 rf_band
)
{
  uint16 ret_band = rf_band;
  switch(ftm_rf_tech)
  {
     case FTM_RF_TECH_CDMA:
        // not needed yet, add it later if needed 
		break;
	  
     case FTM_RF_TECH_WCDMA:
	 	 switch(rf_band)
        {
          case WCDMA_FTM_BAND_1:
          ret_band = RFCOM_BAND_IMT;
          break;
      
          case WCDMA_FTM_BAND_2:
          ret_band = RFCOM_BAND_1900;
           break;
      
          case WCDMA_FTM_BAND_3:
          ret_band = RFCOM_BAND_BC3;
          break;
      
          case WCDMA_FTM_BAND_4:
           ret_band = RFCOM_BAND_BC4;
           break;
      
         case WCDMA_FTM_BAND_5:
         ret_band = RFCOM_BAND_800;
          break;
      
         case WCDMA_FTM_BAND_8:
          ret_band = RFCOM_BAND_BC8;
          break;
      
         case WCDMA_FTM_BAND_9:
          ret_band = RFCOM_BAND_BC9;
          break;
      
        case WCDMA_FTM_BAND_11:
          ret_band = RFCOM_BAND_BC11;
          break;
      
        case WCDMA_FTM_BAND_19:
          ret_band = RFCOM_BAND_BC19;
          break;
      
        default:
         ret_band = RFCOM_BAND_INVALID;
         break;
       }
	   
       break;
    
    #ifdef FEATURE_LTE
  case FTM_RF_TECH_LTE:
    
     ret_band =  (uint16)rflte_ftm_mc_convert_integer_band_to_rfcom_band(rf_band);
       break;
    #endif
    
     case FTM_RF_TECH_TDSCDMA:	
	 	switch(rf_band)
       {
          case TDSCDMA_FTM_BAND_B34:
          ret_band = RFCOM_BAND_TDSCDMA_B34;
          break;
      
         case TDSCDMA_FTM_BAND_B39:
          ret_band = RFCOM_BAND_TDSCDMA_B39;
          break;
      
        case TDSCDMA_FTM_BAND_B40:
         ret_band = RFCOM_BAND_TDSCDMA_B40;
         break;
      
        default:
         ret_band = RFCOM_BAND_INVALID;
         break;
        } 
    
	  break;
     case FTM_RF_TECH_GSM:
     default: 
	 // not supported yet
	   break;  	
  }
  return ret_band;
}


/*----------------------------------------------------------------------------*/
/*!
  @brief: ftm_convert_iq_8b_to_16b
  
 
  @details: rx iq capture data format convert: from 8bit iq data to 16 bit iq data 
  
  @param 
	input:
	iq_ptr: 
	reverseOrder: change iq sequence order

	output: iq_ptr
	  
  @return
  Status of initialization
*/
	
boolean ftm_convert_iq_8b_to_16b
( 
  uint32* iq_ptr,  
  boolean reverseOrder, 
  uint16 sample_size
)
{
	boolean api_status = FALSE;
	int8 twoscomplement8_L =0;
	int8 twoscomplement8_H= 0;
	uint32 id =0;	
	if(iq_ptr == NULL)
		return api_status;
	api_status = TRUE;
		
	for( id = 0; id < sample_size; id ++)
	{
		twoscomplement8_L = (int8) (iq_ptr[id] & 0xFF);
		twoscomplement8_H= (int8) (((iq_ptr[id]>>8) & 0xFF));
		if(reverseOrder)
		{
			iq_ptr[id] = (((int16) twoscomplement8_H) &0xFFFF)|(((int16) twoscomplement8_L)<<16) ;		
		}
		else
		{
			iq_ptr[id] = (((int16) twoscomplement8_L) &0xFFFF)|(((int16) twoscomplement8_H)<<16) ;	
		}
	}
  return api_status;

}


/*----------------------------------------------------------------------------*/
/*!
  @brief: ftm_convert_iq_16b_to_16b
  
 
  @details: rx iq capture data format convert: from 8bit iq data to 16 bit iq data 
  
  @param 
	input:
	iq_ptr: 
	reverseOrder: change iq sequence order

	output: iq_ptr
	  
  @return
  Status of initialization
*/
	
boolean ftm_convert_rsb_iq_format
( 
  uint32* iq_ptr,  
  boolean reverseOrder,
  uint8 convert_type,
  uint16 sample_size
)
{
	boolean api_status = FALSE;
	int8 twoscomplement8_L =0;
	int8 twoscomplement8_H= 0;

	uint32 id =0;	
	if(iq_ptr == NULL)
		return api_status;
	api_status = TRUE;

	if(convert_type == FTM_RX_RSB_IQ_8bit_to_16bit )
	{
	  for( id = 0; id < sample_size; id ++)
	  {
		twoscomplement8_L = (int8) (iq_ptr[id] & 0xFF);
		twoscomplement8_H= (int8) (((iq_ptr[id]>>8) & 0xFF));
		if(reverseOrder)
		{
			iq_ptr[id] = (((int16) twoscomplement8_H) &0xFFFF)|(((int16) twoscomplement8_L)<<16) ;		
		}
		else
		{
			iq_ptr[id] = (((int16) twoscomplement8_L) &0xFFFF)|(((int16) twoscomplement8_H)<<16) ;	
		}
	  }
	}  
	else if((convert_type == FTM_RX_RSB_IQ_16bit_to_16bit)&&(reverseOrder))
	{
		for( id = 0; id < sample_size; id ++)
		{	  
			iq_ptr[id] = ( ( (iq_ptr[id] &0xFFFF )<<16 ) &0xFFFF0000) | ((iq_ptr[id]>>16)& 0xFFFF);			  
		}
	}
  return api_status;

}

/*----------------------------------------------------------------------------*/
/*!
  @brief: ftm_enh_internal_device_cal_rx_iq_capture
  
 
  @details: get rx iq data
  
  @param 
  tech: the tech mode that doing rsb cal

  @param
  carrier: the carrier id, for LTE, 

  @param
  rx_path: rx_path: PRX or DRX

  @param
  sample_size: IQ sample size

  @param: output
  iq_dst_ptr: the dst iq pointer to save the iq data

  @return
  Status of iq capture. TRUE: gets iq data; FALSE: not getting iq data
*/

boolean ftm_enh_internal_device_cal_rx_iq_capture
(
  ftm_rf_technology_type ftm_rf_tech,
  uint8 carrier,
  uint8 rx_path,
  uint16 sample_size,
  uint32* iq_dst_ptr
)
{
	ftm_config_iq_log_type iq_config = {0};
	uint32 *iq_buf_ptr = NULL;
	uint32 total_wait_time_limit = 11000; // hard code here , 1ms
	uint32 total_wait_time = 0;
	ftm_iq_capture_error_code_type ret_value= FTM_IQ_SUCCESS;

#ifdef FEATURE_LTE 
	rflte_mc_carrier_type lte_carr_rxpath = RFLTE_MC_CARR_TYPE_PRX;
#endif /*FEATURE_LTE*/ 
	boolean api_status = TRUE;

    switch(ftm_rf_tech)
	{
	    case FTM_RF_TECH_CDMA:
		case FTM_RF_TECH_GSM:
		default: 
            api_status = FALSE;
		  // not supported yet
		  break;  
	   #ifdef FEATURE_WCDMA   
		case FTM_RF_TECH_WCDMA:
            if (rfwcdma_data_get_device_type((rfm_device_enum_type) rx_path) == RFM_WCDMA_PRIMARY_RX_DEVICE)
            {
                switch (carrier)
                {
                    case 0:
                        iq_config.testbus_sel = FTM_HSPA_WCDMA_RXF0_NARROWBAND_C0_IQ_FLOATING_SAMPLES;
                    break;
      
                    case 1:
                        iq_config.testbus_sel = FTM_HSPA_WCDMA_RXF0_NARROWBAND_C1_IQ_FLOATING_SAMPLES;
                    break;
      
                    default:
                        FTM_MSG_2(FTM_MED, "Wrong configuration passed: Device %d Carrier id %d", rx_path, carrier);
                        ret_value = FTM_IQ_GENERAL_FAILURE;
                    break;
                }
            }      
            else if (rfwcdma_data_get_device_type((rfm_device_enum_type) rx_path) == RFM_WCDMA_DIVERSITY_RX_DEVICE)
            {
                switch (carrier)
                {
                    case 0:
                      iq_config.testbus_sel = FTM_HSPA_WCDMA_RXF1_NARROWBAND_C0_IQ_FLOATING_SAMPLES;
                        break;
      
                    case 1:
                      iq_config.testbus_sel = FTM_HSPA_WCDMA_RXF1_NARROWBAND_C1_IQ_FLOATING_SAMPLES;
                      break;
      
                    default:
                        FTM_MSG_2(FTM_MED,"Wrong configuration passed: Device %d Carrier id %d", rx_path, carrier);
                        ret_value = FTM_IQ_GENERAL_FAILURE;
                    break;
                }
            }
            else
            {
                FTM_MSG_1(FTM_MED,"Wrong path passed %d", rx_path);
                ret_value = FTM_IQ_GENERAL_FAILURE;
            }   

            if(ret_value != FTM_IQ_SUCCESS)
                api_status = FALSE;
            else
            {
			    //iq_config.testbus_sel = ftm_hwtc_wcdma_data.iq_data.dlog_sel;			
			    iq_buf_ptr = (uint32 *)rfwcdma_msm_iq_capture(&iq_config);
			    DALSYS_BusyWait(200);
			    if( iq_buf_ptr != NULL )
			    {
			        memscpy(iq_dst_ptr, sample_size*sizeof(int32), iq_buf_ptr, sample_size*sizeof(int32)); // need to convert it to LTE format
			    }
			    // free rfwcdma iq buffer
			    rfwcdma_msm_free_iq_buffer();
			    api_status = TRUE;		  
            }
		    break;
  	   #endif /*FEATURE_WCDMA*/
#ifdef FEATURE_LTE 
		case FTM_RF_TECH_LTE:
			
		    lte_carr_rxpath = ((rx_path ==FTM_ENH_INTERNAL_DEVICE_CAL_RXRSB_RES_PRX1) ||(rx_path ==FTM_ENH_INTERNAL_DEVICE_CAL_RXRSB_RES_PRX2)
				                ||(rx_path ==FTM_ENH_INTERNAL_DEVICE_CAL_RXRSB_RES_PRX3))?
								RFLTE_MC_CARR_TYPE_PRX: RFLTE_MC_CARR_TYPE_DRX;
			rflte_msm_iq_capture_config(carrier,lte_carr_rxpath,sample_size);
			total_wait_time = 0;

			while((lte_iq_capture_state_machine.fw_response_flag == FALSE) &&(total_wait_time < total_wait_time_limit))
			{                
				DALSYS_BusyWait(100);
				total_wait_time += 100;
			}
			
			DALSYS_BusyWait(200);
			if(lte_iq_capture_state_machine.fw_response_flag == TRUE)
			{
			  iq_buf_ptr = (uint32 *) ((lte_carr_rxpath == RFLTE_MC_CARR_TYPE_PRX)? 
				lte_iq_capture_state_machine.prx_data_pointer :lte_iq_capture_state_machine.drx_data_pointer);
			  memscpy(iq_dst_ptr, sample_size*sizeof(int32), iq_buf_ptr, sample_size*sizeof(int32)); 
			  api_status = TRUE;
			}
			else
			{
 				memset(iq_dst_ptr, 0,sample_size*sizeof(int32));
				FTM_MSG(FTM_MED,"lte iq capture from fw flag is FALSE");
				api_status = FALSE;
			}
		  break;
#endif /*FEATURE_LTE*/

		#ifdef FEATURE_TDSCDMA 
		case FTM_RF_TECH_TDSCDMA:
 	
			ftm_tds_iq_capture_data.IqCaptureStatus = FTM_TDSCDMA_IQ_CAPTURE_NO_RSP; //0=clear the flag at first
			ftm_tds_iq_capture_data.MaxWaitCnt =  20;   //todo: change back to 20
			ftm_tds_iq_capture_data.SampleSize = sample_size;
		
            ftm_tdscdma_cmd_get_iq_data_from_fw((rfm_device_enum_type)(rx_path));
			
			/* Waiting for the result from TFW return message */
			while ( (ftm_tds_iq_capture_data.IqCaptureStatus == FTM_TDSCDMA_IQ_CAPTURE_NO_RSP) &&
					(ftm_tds_iq_capture_data.MaxWaitCnt != 0) )
			{
			  DALSYS_BusyWait(500); // 1ms resolution
			  ftm_tds_iq_capture_data.MaxWaitCnt -- ;
			}
			
			if(ftm_tds_iq_capture_data.IqCaptureStatus == FTM_TDSCDMA_IQ_CAPTURE_SUCCEED )
			{
  			  if(rx_path == FTM_ENH_INTERNAL_DEVICE_CAL_RXRSB_RES_PRX1 )
			  {

				 {
				   ftm_tdscdma_util_convert_20pcfl_iq_data_format(ftm_tds_iq_capture_data.startAddrPrx,(word *) iq_dst_ptr, (uint32)(sample_size));
			       //FTM_MSG_2(FTM_HIGH, "rx_path: %d  ftm_tds_iq_capture_data.startAddrPrx: %d", rx_path, ftm_tds_iq_capture_data.startAddrPrx);

				 }
				}
			   else
			   {
				 {
				   ftm_tdscdma_util_convert_20pcfl_iq_data_format(ftm_tds_iq_capture_data.startAddrDrx, (word *) iq_dst_ptr, (uint32)(sample_size));
				   //FTM_MSG_2(FTM_HIGH, "rx_path: %d  ftm_tds_iq_capture_data.startAddrDrx: %d", rx_path, ftm_tds_iq_capture_data.startAddrDrx);
				 }
			   }
			   api_status = TRUE;
			}
			else
			{
			   api_status = FALSE;
			}
			FTM_MSG_1(FTM_MED,"tds do iq capture, status:%d", ftm_tds_iq_capture_data.IqCaptureStatus);
		  break;		  
	#endif /*FEATURE_TDSCDMA*/
	 	}
	 return api_status;	 
}

boolean ftm_enh_internal_device_cal_rx_iq_prx_drx_capture
(
  ftm_rf_technology_type ftm_rf_tech,
  uint8 carrier_idx,
  uint8 rx_chain_mask,
  uint16 sample_size,
  uint32* prx_iq_buf_ptr,
  uint32* drx_iq_buf_ptr
)
{
	uint32 total_wait_time_limit = 5000; // hard code here , 5ms
	uint32 total_wait_time = 0;

	boolean api_status = TRUE;
	//ftm_iq_capture_error_code_type status;

	 switch(ftm_rf_tech)
	 {
		case FTM_RF_TECH_CDMA:
		case FTM_RF_TECH_GSM:
		default: 
            FTM_MSG(FTM_MED,"CDMA/GSM RxRSB rsb not support yet, no need do IQ capture, return False");
            api_status = FALSE;
		  // not supported yet
		  break;  
#ifdef FEATURE_WCDMA   
		case FTM_RF_TECH_WCDMA:
			FTM_MSG(FTM_MED,"WCDMA Prx/Drx Simultanuously IQ capture not supported yet");
            api_status = FALSE;
		  break;
#endif /*FEATURE_WCDMA*/

#ifdef FEATURE_TDSCDMA 
		case FTM_RF_TECH_TDSCDMA:
			FTM_MSG(FTM_MED,"TDS Prx/Drx Simultanuously IQ capture not supported yet");
            api_status = FALSE;
		  break;		  
#endif /*FEATURE_TDSCDMA*/	   
#ifdef FEATURE_LTE 
		case FTM_RF_TECH_LTE:
            
			if( ( (rx_chain_mask & 0x01) && (prx_iq_buf_ptr == NULL) ) 
                || ( (rx_chain_mask & 0x02) && (drx_iq_buf_ptr == NULL) ) )
			{
                FTM_MSG(FTM_MED,"LTE IQ capture failure, either Prx or Drx or both Prx and Drx IQ  capture buffer is NULL");
                api_status = FALSE;
			}
            else
            {
                
			    rflte_msm_prx_drx_iq_capture_config (carrier_idx,rx_chain_mask,sample_size);

			    total_wait_time = 0;

			    while(( lte_iq_capture_state_machine.fw_response_flag == FALSE) &&(total_wait_time < total_wait_time_limit))
			    {                
				    DALSYS_BusyWait(100);
				    total_wait_time += 100;
			    }
			
			    if(lte_iq_capture_state_machine.fw_response_flag == TRUE)
			    {

                    if( (rx_chain_mask & 0x01) && ( lte_iq_capture_state_machine.prx_data_pointer !=NULL))
                    {
                        memscpy(prx_iq_buf_ptr, sample_size*sizeof(int32), lte_iq_capture_state_machine.prx_data_pointer, sample_size*sizeof(int32));
                    }

                    if( (rx_chain_mask & 0x02) && ( lte_iq_capture_state_machine.drx_data_pointer != NULL))
                    {
                        memscpy(drx_iq_buf_ptr, sample_size*sizeof(int32), lte_iq_capture_state_machine.drx_data_pointer, sample_size*sizeof(int32));
                    }
                    if ( (rx_chain_mask & 0x01) && ( lte_iq_capture_state_machine.prx_data_pointer == NULL) )
                    {
                        FTM_MSG(FTM_MED,"LTE IQ capture, NO PRX IQ data returned");
                        api_status = FALSE;
                    }
                    else if ( (rx_chain_mask & 0x02) && ( lte_iq_capture_state_machine.drx_data_pointer == NULL))
                    {
                        FTM_MSG(FTM_MED,"LTE IQ capture, No DRX IQ data returned");
                        api_status = FALSE;
                    }
                    else
                    {
                        api_status = TRUE;
                    }
			    }
			    else
			    {
                    if(prx_iq_buf_ptr !=NULL)
                    {
                        memset(prx_iq_buf_ptr, 0,sample_size*sizeof(int16));
                    }
                    if(drx_iq_buf_ptr != NULL)
                    {
				        memset(drx_iq_buf_ptr,0,sample_size*sizeof(int16));
                    }
				    FTM_MSG(FTM_MED,"lte prx/drx simultanuously iq capture from fw flag is FALSE");
				    api_status = FALSE;
			    }
            }
		  break;
#endif /*FEATURE_LTE*/
	 }
	 return api_status;	 
}			


/*----------------------------------------------------------------------------*/
/*!
  @brief: ftm_enh_internal_device_cal_save_iq
  
 
  @details:iq capture save to efs
  
  @param 

    output: 
  
  @return
 
*/

boolean ftm_enh_internal_device_cal_save_iq
( 
  ftm_rf_technology_type ftm_rf_tech, 
  uint16 band,
  uint8 rxPath, 
  uint32 *iq_data, 
  int16 sampleSize,  
  uint8 is_rsb_updated
)
{
	EFS_EFILE *iq_fp;
	char iq_file_name[128];
	boolean api_status = FTM_IQ_SUCCESS;
	fs_size_t write_result;
	int file_close_res;
	
    memset(iq_file_name,0,sizeof(iq_file_name));
    
	 switch(ftm_rf_tech)
	 {
		case FTM_RF_TECH_CDMA:
		case FTM_RF_TECH_GSM:
		default: 
		  // not supported yet
		  break;  
		   
		case FTM_RF_TECH_WCDMA:
            if(is_rsb_updated == 2)
            {
				snprintf(&iq_file_name[0], 128, "rxIQ_wcdma_band_%d_path_%d_rsbcomped2.bin",band,rxPath );

            }
			else if(is_rsb_updated == 1)
            {
				snprintf(&iq_file_name[0], 128, "rxIQ_wcdma_band_%d_path_%d_rsbcomped.bin",band,rxPath );
            }
			else
			{
				snprintf(&iq_file_name[0], 128, "rxIQ_wcdma_band_%d_path_%d.bin",band,rxPath);
			}	
		  break;
	
		case FTM_RF_TECH_LTE:

		    if(is_rsb_updated==2)
            {
				snprintf(&iq_file_name[0], 128, "rxIQ_lte_band_%d_path_%d_rsbcomped2.bin",band, rxPath);
             }
			else if(is_rsb_updated==1)
            {
				snprintf(&iq_file_name[0], 128, "rxIQ_lte_band_%d_path_%d_rsbcomped.bin", band,rxPath);
             }
			else
			{
				snprintf(&iq_file_name[0], 128, "rxIQ_lte_band_%d_path_%d.bin", band,rxPath);
			}

		  break;
	
		case FTM_RF_TECH_TDSCDMA:
		    if(is_rsb_updated == 2)
            {
				snprintf(&iq_file_name[0], 128, "rxIQ_tdscdma_band_%d_path_%d_rsbcomped2.bin", band,rxPath);
             }
			else if(is_rsb_updated == 1)
            {
				snprintf(&iq_file_name[0], 128, "rxIQ_tdscdma_band_%d_path_%d_rsbcomped.bin", band,rxPath);
             }
			else
			{
				snprintf(&iq_file_name[0], 128, "rxIQ_tdscdma_band_%d_path_%d.bin", band,rxPath);
			}			
		 break;
	}
	iq_fp = efs_efopen( &iq_file_name[0], "w" );
	if(iq_fp !=NULL)
	{
			/* Save samples as they appear in lmem. */
		write_result = efs_efwrite( &iq_data[0], 
									sizeof(uint32),
									sampleSize, iq_fp );	
		if( write_result == 0 )
		{
			api_status = FALSE;
		}
		file_close_res = efs_efclose( iq_fp );
    	if( file_close_res != 0 )
    	{
      		api_status = FALSE;
    	}
	}	
   return api_status;
}


/*----------------------------------------------------------------------------*/
/*!
  @brief: ftm_enh_device_cal_rsb_get_thermistor
  
 
  @details:get thermistor reading
  
  @param 

    output: 
  
  @return
 
*/

boolean ftm_enh_device_cal_rsb_get_thermistor
(
  rfm_device_enum_type device, 
  uint16 band, 
  uint16* enh_therm_val
)
{
	boolean ret_val = FALSE;
	#ifdef  FEATURE_LTE
	rfdevice_lte_script_data_type script_data = {TRUE, NULL};
	 #endif  /*FEATURE_LTE*/
   ftm_rf_technology_type ftm_rf_tech = ftm_get_rf_technology();
  
   switch (ftm_rf_tech)
   {
  #ifdef FEATURE_WCDMA    	
      case FTM_RF_TECH_WCDMA: //wcdma use tx device to do it
		ret_val = rfdevice_wcdma_tx_cmd_dispatch(device,
							 ftm_curr_wcdma_mode,
							 RFDEVICE_GET_THERMISTER_VALUE, 
							 (void *) enh_therm_val);

		break;
  #endif /*FEATURE_WCDMA*/

  #ifdef  FEATURE_LTE
      case FTM_RF_TECH_LTE://lte to use tx device to do it
	  	
		ret_val = rfdevice_lte_tx_cmd_dispatch(device, 
                                               (rfcom_lte_band_type) band, 
                                               RFDEVICE_LTE_GET_THERMISTER_VALUE,
                                               (void *) enh_therm_val,
                                                &script_data);
		break;
  #endif  /*FEATURE_LTE*/

  #ifdef FEATURE_TDSCDMA
	  case FTM_RF_TECH_TDSCDMA: // tds has to use tx device to read it
	  	ret_val = rfdevice_tdscdma_tx_cmd_dispatch(device,
                                     RFCOM_TDSCDMA_MODE,
                                     band,
                                     RFDEVICE_GET_THERMISTER_VALUE,
                                     (void *) enh_therm_val
                                     );
		break;
  #endif /*FEATURE_TDSCDMA*/

	  case FTM_RF_TECH_CDMA: //add it later
	  case FTM_RF_TECH_GSM:// add it later
	  case FTM_RF_TECH_UNKNOWN:
	  default:
	  	enh_therm_val =0;
	    break;
   	}
   return ret_val;

}

/*----------------------------------------------------------------------------*/
/*!
  @brief: get_rxlm_handler
  
 
  @details:get_rxlm_handler as well as rflm_tech_id, which is different from ftm_tech_enum
  
  @param 

    output: 
  
  @return
 
*/
	

boolean get_rxlm_handler
(
  ftm_rf_technology_type ftm_rf_tech,
  rflm_handle_rx_t *rxlm_buffer_idx, 
  rflm_tech_id_t *rflm_tech_id
)
{
	boolean api_status = FALSE;
	uint8 lte_carrierId = 0;
	uint8 rxDevice_id = 0;
	switch(ftm_rf_tech)
	{
        #ifdef FEATURE_WCDMA
		case FTM_RF_TECH_WCDMA:
            for( rxDevice_id = 0; rxDevice_id< RFM_MAX_WAN_DEVICES; rxDevice_id ++)
            {                
                rxlm_buffer_idx[rxDevice_id] = ftm_wcdma_rxlm_buffer[rxDevice_id].buf_idx;
            }
            
			*rflm_tech_id = RFLM_TECH_WCDMA;
		
			api_status = TRUE;
			break;
        #endif /*FEATURE_WCDMA*/
		
#ifdef FEATURE_LTE 
		case FTM_RF_TECH_LTE:
		for (lte_carrierId = 0; lte_carrierId< RFA_RF_LTE_MAX_CELL_SUPPORTED; lte_carrierId ++)
		{
			rxlm_buffer_idx[ lte_carrierId*2] = rflte_ftm_mc_sm.carrier[lte_carrierId].rxlm_prx_buf_idx;
			rxlm_buffer_idx[ lte_carrierId*2+1] = rflte_ftm_mc_sm.carrier[lte_carrierId].rxlm_drx_buf_idx;
		}	            
			*rflm_tech_id = RFLM_TECH_LTE;
			api_status = TRUE;
			break;
#endif /*FEATURE_LTE*/ 
	
     #ifdef FEATURE_TDSCDMA
		case FTM_RF_TECH_TDSCDMA:
			rxlm_buffer_idx[0] = ftm_tdscdma_rxlm_buffer[FTM_RECEIVE_CHAIN_0].buf_idx;
			rxlm_buffer_idx[1] = ftm_tdscdma_rxlm_buffer[FTM_RECEIVE_CHAIN_1].buf_idx;
			*rflm_tech_id = RFLM_TECH_TDSCDMA;
			api_status = TRUE;
			break;
     #endif /*FEATURE_TDSCDMA*/	
	
		case FTM_RF_TECH_CDMA: //add it late
		case FTM_RF_TECH_GSM:// add it later
		case FTM_RF_TECH_UNKNOWN:
		default:
			api_status = FALSE;
			rxlm_buffer_idx[0] = 0;
			rxlm_buffer_idx[1] = 0;
			*rflm_tech_id = RFLM_TECH_1X;
			break;

	}
	
	FTM_MSG_4(FTM_MED, "rxlm prx buf:0x%x, drx buf id:0x%x, carrier 1: prx buf: 0x%x, drx buf id: 0x%x", rxlm_buffer_idx[0],rxlm_buffer_idx[1],
		rxlm_buffer_idx[2],rxlm_buffer_idx[3]);
	return api_status;
}


/*----------------------------------------------------------------------------*/
/*!
  @brief: get_fbrx_lm_handler
  
 
  @details:get_rxlm_handler as well as rflm_tech_id, which is different from ftm_tech_enum
  
  @param 

    output: 
  
  @return
 
*/
boolean get_fbrx_lm_handler
(
  ftm_rf_technology_type ftm_rf_tech,
  rflm_handle_rx_t *fbrx_buffer_idx, 
  rflm_tech_id_t *rflm_tech_id
)
{
	boolean api_status = FALSE;

	uint8 sub_id =0;
	switch(ftm_rf_tech)
	{
        #ifdef FEATURE_WCDMA
	case FTM_RF_TECH_WCDMA:
			fbrx_buffer_idx[0] = rfcommon_fbrx_mc_get_tech_handle( RFM_IMT_MODE );
			fbrx_buffer_idx[1] = fbrx_buffer_idx[0];
			*rflm_tech_id = RFLM_TECH_WCDMA;
			api_status = TRUE;
			break;
         #endif
         #ifdef  FEATURE_LTE
		case FTM_RF_TECH_LTE:
			fbrx_buffer_idx[0] = rflte_state_get_fbrxlm_buffer_idx(sub_id, 0 );			
			fbrx_buffer_idx[1] = rflte_state_get_fbrxlm_buffer_idx(sub_id, 1 );			
			*rflm_tech_id = RFLM_TECH_LTE;
			api_status = TRUE;
			break;
         #endif

		case FTM_RF_TECH_TDSCDMA:
			fbrx_buffer_idx[0] = rfcommon_fbrx_mc_get_tech_handle( RFM_TDSCDMA_MODE );
			fbrx_buffer_idx[1] = fbrx_buffer_idx[0];
			*rflm_tech_id = RFLM_TECH_TDSCDMA;
			api_status = TRUE;
			break;
	
		case FTM_RF_TECH_CDMA: //add it late
		case FTM_RF_TECH_GSM:// add it later
		case FTM_RF_TECH_UNKNOWN:
		default:
			api_status = FALSE;
			fbrx_buffer_idx[0] = -1;
			fbrx_buffer_idx[1] = -1;
			*rflm_tech_id = RFLM_TECH_1X;
			break;

	}
	
	FTM_MSG_2(FTM_MED, "fbrx lm buf_id:0x%x, carrier 1 buf id:0x%x", fbrx_buffer_idx[0],fbrx_buffer_idx[1]);
	return api_status;
}

ftm_enh_internal_device_cal_error_code_type ftm_enh_internal_device_cal_get_stg_freq
(
  rfdevice_rxtx_common_class * device_ptr,
  int32 stg_offset_khz,
  uint8 debug_mode_en,
  uint64 * stg_freq,
  rfm_device_enum_type tx_device
)
{
  ftm_enh_internal_device_cal_error_code_type ret_val = FTM_ENH_INTERNAL_DEVICE_CAL_RESULTS_SUCCESS;
  ftm_rf_technology_type ftm_rf_tech  = FTM_RF_TECH_UNKNOWN;
  uint32 tx_freq = 0;
  
  #ifdef FEATURE_CDMA
  const rf_cdma_data_status_type *dev_status_r;
  rfm_cdma_band_class_type band;
  #endif

  /* Get Current FTM RF Tech in order to get current Rx Freq */
  ftm_rf_tech = ftm_get_rf_technology();

  /* check for null pointer and throw error */
  if (device_ptr == NULL)
  {
    FTM_MSG(FTM_ERROR, "rfc_common_get_cmn_device_object(0) returns NULL");
    return FTM_ENH_INTERNAL_DEVICE_CAL_FBRX_DEV_ACTION_FAILED;
  }

  /****************************************************************/
  /* Get STG Frequency to tune to from Tx channel information     */
  /****************************************************************/

  switch ( ftm_rf_tech )
  {
  #if defined(FEATURE_CDMA1X) && defined(FEATURE_CDMA) 
  case FTM_RF_TECH_CDMA:
    dev_status_r = rf_cdma_get_device_status( tx_device ); // what is the current tx device?
    if( dev_status_r != NULL)
    {
      band = rf_cdma_sys_band_type_to_rf_band_type( (sys_band_class_e_type)dev_status_r->curr_band);
      tx_freq = rf_cdma_get_tx_carrier_freq( band, (int)
                                             dev_status_r->lo_tuned_to_chan );
    }
    else
    {
      FTM_MSG(FTM_ERROR, "rfc_common_get_cmn_device_object(0) returns NULL");
      return FTM_ENH_INTERNAL_DEVICE_CAL_GENERAL_FAILURE;
    }
    break;
  #endif

  #ifdef FEATURE_WCDMA
  case FTM_RF_TECH_WCDMA:
    {
      #ifdef ATLAS_RF_WCDMA_PRESI
      rfm_wcdma_ftm_radio_state_data_type radio_state;
      rfm_wcdma_ftm_tx_get_radio_state( tx_device, &radio_state );
      tx_freq = rf_wcdma_utils_get_tx_freq_from_ul_arfcn(radio_state.channel[0]);
      #else
      tx_freq = rfwcdma_core_get_freq_from_ul_arfcn(rfwcdma_data_get_curr_chan(tx_device,RFCOM_SINGLE_CARRIER));
      FTM_MSG_1(FTM_HIGH, "ftm_enh_internal_device_cal_get_stg_rxfreq: curr_rx_chan: %d", tx_freq);
      #endif
    }
    break;
  #endif /* FEATURE_WCDMA */
  #ifdef FEATURE_LTE
  case FTM_RF_TECH_LTE:
    tx_freq = rflte_core_get_tx_freq_from_uarfcn(
                                   rflte_ftm_mc_sm_get_tx_chan(),
                                   rflte_ftm_mc_sm_get_tx_band());
    break;
  #endif
  #ifdef FEATURE_TDSCDMA
  case FTM_RF_TECH_TDSCDMA:
  	tx_freq = rf_tdscdma_core_util_get_freq_from_chan(rf_tdscdma_mc_state.curr_chan_tx);
    break;
  #endif
  default:
    FTM_MSG(FTM_ERROR, "Unsupported Tech Type: Return error handle");
    return FALSE;
    /* Flag Error as Unsupported Tech */
  }
  
  *stg_freq = (uint64) ((tx_freq + stg_offset_khz)*1000);
  return ret_val;

}

ftm_enh_internal_device_cal_error_code_type ftm_enh_internal_device_cal_rsb_stg_on
(
  rfdevice_rxtx_common_class * device_ptr,
  int32 stg_offset_khz,
  uint8 debug_mode_en,
  uint64 * stg_freq_hz,
  rfm_mode_enum_type mode,
  rfcom_band_type_u band,
  rfcom_bw_type_u bw,
  uint8 fbrx_gain_state,
  rfm_device_enum_type tx_device
)
{
	ftm_enh_internal_device_cal_error_code_type ret_val = FTM_ENH_INTERNAL_DEVICE_CAL_RESULTS_SUCCESS;
	boolean api_status = FALSE;
	uint64 stg_freq = 0;
	ret_val = ftm_enh_internal_device_cal_get_stg_freq(device_ptr, stg_offset_khz, debug_mode_en, &stg_freq, tx_device);
	if(ret_val !=  FTM_ENH_INTERNAL_DEVICE_CAL_RESULTS_SUCCESS)
	{
		return ret_val;
	}
	api_status = rfdevice_cmn_enable_stg(
								   device_ptr,
								   stg_freq, 
								   RFDEVICE_EXECUTE_IMMEDIATE,
								   NULL,
                                   mode,
                                   band,
                                   bw,
                                   fbrx_gain_state
								  );
    if (api_status == FALSE)
    {
      FTM_MSG(FTM_ERROR, "rfdevice_cmn_enable_stg returned FALSE ");
      rfcommon_core_xpt_free_all_buffers();  
      return FTM_ENH_INTERNAL_DEVICE_CAL_FBRX_DEV_ACTION_FAILED;
    }

    *stg_freq_hz = stg_freq;
    /* Allow settling time for STG?? */
	return ret_val;
}


ftm_enh_internal_device_cal_error_code_type ftm_enh_internal_device_cal_fbrx_rsb_iq_capture
(
	rfdevice_rxtx_common_class * device_ptr,
	uint8 rsb_cal_alg,
	int32 stg_offset_khz,
	uint16 num_iq_samples,
	uint8 debug_mode_en,
	uint32 * IQSamples,
    uint32 * dcCompVals,
    rfcom_band_type_u band_union,
    rfcom_bw_type_u rfcom_bw,
    uint8 fbrx_gain_state,
    rfm_device_enum_type tx_device
)
{
  ftm_enh_internal_device_cal_error_code_type ret_val = FTM_ENH_INTERNAL_DEVICE_CAL_RESULTS_SUCCESS;
  boolean api_status = TRUE;
  //rfdevice_rxtx_common_class *device_ptr = NULL;

  ftm_rf_technology_type ftm_rf_tech  = FTM_RF_TECH_UNKNOWN;
  rfm_mode_enum_type rfm_mode = RFM_INVALID_MODE;
  uint8 samp_buf_id = 0;
  
  ftm_cal_xpt_instr_payload_iq_capture_type iq_cap_instr;
  char filename[20] = {'\0'};
 
  uint8 iter = 0;
  uint64 stg_freq_hz = 0;
  
  /* Reset Capture Buffers */
  rfcommon_core_xpt_init_buffers(); // must be done before IQ capture code is called - Mark in Documentation for IQ Capture

  /* Get Current FTM RF Tech in order to get current Tx Freq 
     and also to dispatch tech-specific IQ capture command */
  ftm_rf_tech = ftm_get_rf_technology();
  rfm_mode = ftm_common_ftm_tech_to_rfm_mode(ftm_rf_tech);

  if (rsb_cal_alg == FTM_ENH_INTERNAL_DEVICE_CAL_RSB_ALG_STG )
  {
    ftm_enh_internal_device_cal_rsb_stg_on (device_ptr,stg_offset_khz,debug_mode_en,&stg_freq_hz,rfm_mode,band_union,rfcom_bw,fbrx_gain_state,tx_device);
	FTM_MSG(FTM_MED,"fbrx rsb cal, stg turned on");
  }
	 
  // check if there is going to be too many captures to save on the EFS
  iq_cap_instr.primary_device = tx_device; /* What is the tx device, doesn't matter */
  iq_cap_instr.feedback_device = tx_device; /* What is the fbrx device */
  iq_cap_instr.num_samples = num_iq_samples;
  iq_cap_instr.proc_flag = PROC_NONE;
  iq_cap_instr.capture_type = XPT_CAPTURE_FBRX;
  iq_cap_instr.first_trigger = 1;
  iq_cap_instr.last_trigger = 1;


  snprintf(&filename[0],20, "iq_%05d_khz.bin", (int)stg_offset_khz);

    /****************************************************************/
    /*              Setup and Trigger XPT IQ Capture                */
    /****************************************************************/

  ret_val = ftm_enh_internal_device_cal_fbrx_iq_capture(ftm_rf_tech, &iq_cap_instr, &samp_buf_id, debug_mode_en, &filename[0]);
  if(rsb_cal_alg == FTM_ENH_INTERNAL_DEVICE_CAL_RSB_ALG_STG )
  {
	  api_status = rfdevice_cmn_disable_stg(
                                      device_ptr,
                                      RFDEVICE_EXECUTE_IMMEDIATE,
                                      NULL,
                                      stg_freq_hz,
                                      rfm_mode,
                                      band_union,
                                      rfcom_bw,
                                      fbrx_gain_state
                                     );
	  if (api_status == FALSE)
      {
        FTM_MSG_1(FTM_ERROR, "Something went wrong on loop #%d, breaking loop, check FTM Msgs", iter );
        rfcommon_core_xpt_free_all_buffers(); 
        return FTM_ENH_INTERNAL_DEVICE_CAL_FBRX_DEV_ACTION_FAILED;
      }
  }
   	 
  if (ret_val != FTM_ENH_INTERNAL_DEVICE_CAL_RESULTS_SUCCESS)
  {
    FTM_MSG_1(FTM_ERROR, "ftm_enh_internal_device_cal_fbrx_iq_capture failed for tech %d", (int)ftm_rf_tech);
	rfcommon_core_xpt_free_all_buffers();
    return ret_val;
  }

  FTM_MSG_1(FTM_HIGH, "Current Capture Buffer is %d", samp_buf_id);

  ret_val = ftm_enh_internal_device_cal_rsb_fetch_iq( samp_buf_id, num_iq_samples,debug_mode_en, IQSamples, dcCompVals);
  
  if (ret_val != FTM_ENH_INTERNAL_DEVICE_CAL_RESULTS_SUCCESS)
  {
      FTM_MSG_1(FTM_ERROR, "rfcommon_mdsp_fetch_selftest_iq_data failed with status %d", api_status);
      return FTM_ENH_INTERNAL_DEVICE_CAL_FETCH_SAMPLES_FAILED;
  } 

  rfcommon_core_xpt_free_all_buffers();  // must be done after last IQ capture code is called - Mark in Documentation for IQ Capture

  return ret_val;
}


ftm_enh_internal_device_cal_error_code_type 
ftm_enh_internal_device_cal_rsb_fetch_iq
(
   uint8  samp_buffers,
   uint16 num_samps,
   uint8 debug_mode_en,
   uint32 * IQSamples,
   uint32 * dcCompVals
)
{
  ftm_enh_internal_device_cal_data_type *internal_device_cal_data;
  
  uint16 iter = 0;
  boolean ret_val = FALSE;
  uint32 rx_samp_pcfl20;
  uint8 exponent;
  int32 rxI;
  int32 rxQ;
  int16 rxI_FP16 = 0;
  int16 rxQ_FP16 = 0;
  int32 * rx_samps;
  int32 * tx_samps;
  uint8 overflow_det;
  int32 dc_I = 0;
  int32 dc_Q = 0;
  int16 dc_I_FP16 = 0;
  int16 dc_Q_FP16 = 0;



  internal_device_cal_data = ftm_common_data_get_enh_internal_device_cal_ptr();

  debug_mode_en = internal_device_cal_data->internal_device_cal_config.dbg_mode_en;
//  fbrx_bw = internal_device_cal_data->internal_device_cal_config.bw;


  ret_val = rfcommon_mdsp_fetch_selftest_iq_data(samp_buffers, num_samps, &rx_samps, &tx_samps);

  for (iter = 0; iter < num_samps; iter++)
  { 
      if (iter%8 == 0)
      {
        rx_samp_pcfl20 = (((uint32)*rx_samps) >> 0) & 0xFFFFF;
      }
      else if (iter%8 == 1)
      {
        rx_samp_pcfl20 = ( ( ((uint32)*rx_samps) >> 20 )  +  ( ( (uint32)*(rx_samps + 1) ) << 12 )) & 0xFFFFF ;
      }
      else if (iter%8 == 2)
      {
        rx_samp_pcfl20 = ( ((uint32)*(rx_samps+1) ) >> 8) & 0xFFFFF;
      }
      else if (iter%8 == 3)
      {
        rx_samp_pcfl20 = ( ( ( (uint32)*( rx_samps + 1 ) ) >> 28 ) + ( ( (uint32) *( rx_samps + 2 ) ) << 4 ) ) & 0xFFFFF;
      }
      else if (iter%8 == 4)
      {
        rx_samp_pcfl20 = ( ( ( (uint32) *( rx_samps + 2 ) ) >> 16 ) + (( (uint32) *( rx_samps + 3 ) ) << 16 ) ) & 0xFFFFF;
      }
      else if (iter%8 == 5)
      {
        rx_samp_pcfl20 = (((uint32)*(rx_samps+3)) >> 4) & 0xFFFFF;
      }
      else if (iter%8 == 6)
      {
        rx_samp_pcfl20 = ( ( ((uint32) *( rx_samps + 3 ) ) >> 24 ) + ( ( (uint32) *( rx_samps + 4 ) ) << 8 )) & 0xFFFFF;
      }
      else if (iter%8 == 7)
      {
        rx_samp_pcfl20 = (((uint32)*(rx_samps+4)) >> 12) & 0xFFFFF;
        rx_samps = rx_samps + 5;
      }

      exponent = (uint8)((rx_samp_pcfl20>>16) & 0xFF);
      rxI = (int32)(((int8)((rx_samp_pcfl20>>8) & 0xFF)) << (15-exponent)); 
      rxQ = (int32)(((int8)((rx_samp_pcfl20>>0) & 0xFF)) << (15-exponent)); 

      rxI = rxI>>6;
      rxQ = rxQ>>6;

      rxI_FP16 = sat_int32_to_int16(rxI, &overflow_det);
      if (overflow_det == 1)
      {
        return FTM_ENH_INTERNAL_DEVICE_CAL_ARITHMETIC_OVERFLOW;
      }
      rxQ_FP16 = sat_int32_to_int16(rxQ, &overflow_det);
      if (overflow_det == 1)
      {
        return FTM_ENH_INTERNAL_DEVICE_CAL_ARITHMETIC_OVERFLOW;
      }
      dc_I += rxI_FP16;
      dc_Q += rxQ_FP16;

      if ( (iter <32) &  (debug_mode_en != 0))
      {
        FTM_MSG_6(FTM_MED, "samp ind %4d    =  %16d  +  j *  %16d   ,  %16d + j * %16d, overflow_det = %d", iter, rxI, rxQ, rxI_FP16, rxQ_FP16, overflow_det);
      }
      if ( (iter > 9206) &  (debug_mode_en != 0))
      {
        FTM_MSG_6(FTM_MED, "samp ind %4d    =  %16d  +  j *  %16d   ,  %16d + j * %16d, overflow_det = %d", iter, rxI, rxQ, rxI_FP16, rxQ_FP16, overflow_det);
      }
	  IQSamples[iter] = (uint32) ((((uint32)(rxI_FP16<<16)) & 0xFFFF0000) | ((uint32) rxQ_FP16& 0xFFFF));

  }
  dc_I = (int32)(dc_I/(int32)num_samps);
  dc_Q = (int32)(dc_Q/(int32)num_samps);
  dc_I_FP16 = sat_int32_to_int16(dc_I, &overflow_det);
  if (overflow_det == 1)
  {
    return FTM_ENH_INTERNAL_DEVICE_CAL_ARITHMETIC_OVERFLOW;
  }
  dc_Q_FP16 = sat_int32_to_int16(dc_Q, &overflow_det);
  if (overflow_det == 1)
  {
    return FTM_ENH_INTERNAL_DEVICE_CAL_ARITHMETIC_OVERFLOW;
  }
  *dcCompVals = (uint32) ((((uint32)(dc_I_FP16<<16)) & 0xFFFF0000) | ((uint32) dc_Q_FP16& 0xFFFF));

  if(ret_val == TRUE)
  {
     return FTM_ENH_INTERNAL_DEVICE_CAL_RESULTS_SUCCESS;
  }
  else
  {
	return FTM_ENH_INTERNAL_DEVICE_CAL_IQ_CAPTURE_FAILURE;
  }

}


/*!
  @name: ftm_enh_internal_device_cal_rsb

  @brief: rsb calibration
  
 
  @details
  
  @param
  input: req_instr, payload instruction
  output: res_len:  cal result length
             rsp_payload: output of feedback rsb cal
  
  @return
     success or fail
*/

boolean ftm_enh_internal_device_cal_rsb
(
    void const *req_instr,
    uint16 *res_len,
    void *rsp_payload 
)
{
    boolean ret_val = TRUE;

//  extract rsb cal-parameter
    ftm_enh_internal_device_cal_instr_payload_rsb_type * rsb_instr = (ftm_enh_internal_device_cal_instr_payload_rsb_type *) req_instr;
    uint8 rsb_type = rsb_instr->rsb_type;
    ftm_enh_internal_device_cal_data_type *internal_device_cal_data = ftm_common_data_get_enh_internal_device_cal_ptr();    
    if(rsb_type == FTM_ENH_INTERNAL_DEVICE_CAL_RX_RSB)
    {
        return ftm_enh_internal_device_cal_rx_rsb(req_instr, res_len, rsp_payload);
    }
    else if(rsb_type == FTM_ENH_INTERNAL_DEVICE_CAL_FB_RSB)
    {
        return ftm_enh_internal_device_cal_fbrx_rsb(req_instr, res_len, rsp_payload);
    }
    else
    {
        ftm_enh_internal_device_cal_rsb_result_type * rsb_result = (ftm_enh_internal_device_cal_rsb_result_type *) rsp_payload;
        FTM_MSG(FTM_ERROR, "ftm_enh_internal_device_cal_rsb() failure, requested rsb type not supported!");
        internal_device_cal_data->internal_device_cal_status.error_log = FTM_ENH_INTERNAL_DEVICE_CAL_INVALID_CAL_TYPE ;
        internal_device_cal_data->internal_device_cal_status.error_mask = (1<< ENH_INTERNAL_DEVICE_CAL_RSB_BIT);        
        return FALSE;
    }
}    


/*!
  @name: ftm_enh_internal_device_cal_rx_rsb

  @brief: rx rsb calibration
  
 
  @details
  
  @param
  input: req_instr, payload instruction
  output: res_len:  cal result length
             rsp_payload:  output of rsb cal results 
  @return
     success or fail
*/

boolean ftm_enh_internal_device_cal_rx_rsb
(
    void const *req_instr,
    uint16 *res_len,
    void *rsp_payload 
)
{
    boolean ret_val = TRUE;
    ftm_enh_internal_device_cal_error_code_type api_status = FTM_ENH_INTERNAL_DEVICE_CAL_RESULTS_SUCCESS;
    ftm_rf_technology_type curr_ftm_rf_tech = FTM_RF_TECH_UNKNOWN;

//  extract rsb cal-parameter
    ftm_enh_internal_device_cal_instr_payload_rsb_type * rsb_instr = (ftm_enh_internal_device_cal_instr_payload_rsb_type *) req_instr;
    uint8 rsb_type = rsb_instr->rsb_type;
    uint8 rsb_cal_alg = rsb_instr->rsb_cal_alg;
    uint32 sample_size = rsb_instr->iq_sample_size;
    int32  stg_offset_khz = rsb_instr->offset; 
    uint8 carrier = rsb_instr->carrier;
    uint8 cur_gain_state = rsb_instr->gain_states[0];
   
    ftm_enh_internal_device_cal_rsb_result_type * rsb_result = (ftm_enh_internal_device_cal_rsb_result_type *) rsp_payload;

//  extract common enhanced internal cal info parameters

    ftm_enh_internal_device_cal_data_type *internal_device_cal_data = ftm_common_data_get_enh_internal_device_cal_ptr();
    uint8 * rsp_payload_validation_addr = internal_device_cal_data->internal_device_cal_result.rsb_calver_buff_ptr;
    uint8 tech = internal_device_cal_data->internal_device_cal_config.tech;
    uint16 band = internal_device_cal_data->internal_device_cal_config.band;
    uint8  bw = internal_device_cal_data->internal_device_cal_config.bw;     
    uint16 device_mask = internal_device_cal_data->internal_device_cal_config.device_mask;
    uint8 dbg_mode_en = internal_device_cal_data->internal_device_cal_config.dbg_mode_en;
    ftm_enh_internal_device_rsb_cal_efswrite_flag_type flag = (ftm_enh_internal_device_rsb_cal_efswrite_flag_type) internal_device_cal_data->internal_device_cal_config.flag;   // flag for efs write request 

// local parameters

   // chains for rsb cal
    uint8 chain_mask = 0;    
    uint8 rx_path[2] = {FTM_ENH_INTERNAL_DEVICE_CAL_RXRSB_RES_PRX1,FTM_ENH_INTERNAL_DEVICE_CAL_RXRSB_RES_DRX1};	

    uint8 rx_path_num = 1;
    uint8 rx_path_idx = 0;
    boolean asdiv_alt_gain = FALSE; 
   // rxlm and device parameter 
    rflm_tech_id_t rflm_tech_id; 
    rfcommon_msm_num_carr_bw msm_carr_bw;
    rflm_handle_rx_t rxlm_buffer_idx[RFM_MAX_WAN_DEVICES];
    rfm_device_enum_type tx_device = RFM_DEVICE_0;
    rfm_device_enum_type prx_device = RFM_DEVICE_0;
    rfm_device_enum_type drx_device = RFM_DEVICE_1;
   #ifdef FEATURE_LTE
    rflte_ftm_mc_sm_type *ftm_mc_sm_ptr = NULL;
   #endif
	// buffer for iq capture   
    uint32 *iq_buf_ptr = NULL;
    uint32 *iq_drx_buf_ptr = NULL;

   // rsb data
    int32 rsb_a_coeff, rsb_b_coeff;  
    rfdevice_rsb_cal_data_type rsb_data = {0,0,0,0,0,0,0} ;

    uint16 max_cal_elements = 0;
    ftm_common_msm_iq_cap_info iq_cap_struct_dc_comped;   
    uint32 dcCompVals = 0; // for FBRx RSB computation with DC removal

    uint16 therm_data =0; 
   
    int id =0; //for loop id
    uint8 chain_idx = 0;
    uint8 rsb_res_dev_index = 0;
    uint8 rsb_res_gain_index = 0;
    rfdevice_trx_sub_type sub_id=0;

   // cal time profile
    rf_time_tick_type start_tick,start_tick0;

    *res_len = 0;   

    band = ftm_calv4_convert_band(tech, band);
    start_tick0 = rf_time_get_tick();
    internal_device_cal_data->internal_device_cal_status.error_log  = FTM_ENH_INTERNAL_DEVICE_CAL_RESULTS_SUCCESS;

    // init results as default, no rsb comp
    rsb_result->device_mask = 0;
	if(rsb_instr->num_gain_states > MAX_NUM_RSB_GAIN_STATES)
    {
       FTM_MSG_2(FTM_ERROR,"Maximum num of gain states supported %d, requested num of gain states %d", MAX_NUM_RSB_GAIN_STATES, rsb_instr->num_gain_states);
	   return FALSE;
    }
    rsb_result->num_gain_states = rsb_instr->num_gain_states;
    internal_device_cal_data->internal_device_cal_status.error_mask = 0;   

    msm_carr_bw.bw = (rfcom_lte_bw_type) bw;
    msm_carr_bw.bw_kHz = (uint32) bw;
    msm_carr_bw.num_carr = (rfcom_multi_carrier_hspa_id_type) carrier;
   
    FTM_MSG_7(FTM_MED,"enh_internal_device_cal:tech: %d, device_mask%d, rsb type %d, stg_offset: %d, alg: %d, carrier:%d, sampleSize: %d",tech, device_mask, rsb_type, stg_offset_khz,
		   rsb_cal_alg, carrier,sample_size);

    FTM_MSG_5(FTM_MED,"enh_internal_device_cal: num_gain:%d, gain states: %d, %d, %d, %d",rsb_instr->num_gain_states, rsb_instr->gain_states[0],rsb_instr->gain_states[1],
   		rsb_instr->gain_states[2],rsb_instr->gain_states[3]);

    FTM_MSG_3(FTM_MED," band:%d, pcell band for scell: %d, pcell chanl for scell:%d ", band, rsb_instr->pcell_band_for_scell, rsb_instr->pcell_ref_chan_for_scell);
   

    curr_ftm_rf_tech = ftm_get_rf_technology();

    if ( tech != curr_ftm_rf_tech)
    {
	    FTM_MSG_2(FTM_ERROR, "Current RF Tech %d does not match Tech for RxRsb/FbRsb Cal %d", curr_ftm_rf_tech, tech );
	    internal_device_cal_data->internal_device_cal_status.error_log = FTM_ENH_INTERNAL_DEVICE_CAL_TECH_MISMATCH;
	    internal_device_cal_data->internal_device_cal_status.error_mask = (1<< ENH_INTERNAL_DEVICE_CAL_RSB_BIT);
	    return FALSE;
    }

    if (flag != ENH_DO_RSB_WITHOUT_EFS_WRITE)
    {
        FTM_MSG_1(FTM_ERROR, "New SW Version doesn't support RSB efs write mode, RSB cal data Need to GO to NV, error code:%d", 
            FTM_ENH_INTERNAL_DEVICE_CAL_RXRSB_EFS_CAL_DATA_NOT_SUPPORTED);
        internal_device_cal_data->internal_device_cal_status.error_log = FTM_ENH_INTERNAL_DEVICE_CAL_RXRSB_EFS_CAL_DATA_NOT_SUPPORTED;
        internal_device_cal_data->internal_device_cal_status.error_mask = (1<< ENH_INTERNAL_DEVICE_CAL_RSB_BIT);
	    return FALSE;
    }     

    switch (curr_ftm_rf_tech ) 
    {
  #if defined(FEATURE_CDMA1X) && defined(FEATURE_CDMA) 
       case FTM_RF_TECH_CDMA:
           FTM_MSG(FTM_ERROR, "ftm_enh_internal_device_cal_rsb Unsupported Tech Type"); 
           break;
  #endif
       
  #ifdef FEATURE_WCDMA
       case FTM_RF_TECH_WCDMA:
       {
          #ifdef ATLAS_RF_WCDMA_PRESI
          //rfm_wcdma_ftm_radio_state_data_type radio_state;
          tx_device = rfm_wcdma_ftm_tx_get_rfm_device();
          prx_device = rfm_wcdma_ftm_rx_get_primary_rfm_device(tx_device);
          drx_device = rfc_common_get_preferred_associated_rx_device(prx_device);
          #else
           tx_device = rfwcdma_data_get_tx_device();
           prx_device = rfwcdma_data_get_associated_device_prx_from_tx(tx_device);
           drx_device = rfwcdma_data_get_associated_device_rx(prx_device);
          #endif
          #ifdef FEATURE_RF_ASDIV
            if(rfcommon_asdiv_get_current_position(prx_device)==ASDIV_POSITION_1)
            {
              asdiv_alt_gain = TRUE;
            }
            else
            {
              asdiv_alt_gain = FALSE;
            }
          #endif
	   
          break;
       }
  #endif /* FEATURE_WCDMA */
  #ifdef FEATURE_LTE
         case FTM_RF_TECH_LTE:
            if(carrier >=RFA_RF_LTE_MAX_CELL_SUPPORTED)
            {
                FTM_MSG_2(FTM_ERROR, "Enhanced internal device cal: rsb: not supported carrier, max: %d, requested: %d",RFA_RF_LTE_MAX_CELL_SUPPORTED, carrier );
                internal_device_cal_data->internal_device_cal_status.error_log  = FTM_ENH_INTERNAL_DEVICE_CAL_CARRIER_NOT_SUPPORTED ;
                internal_device_cal_data->internal_device_cal_status.error_mask = (1<< ENH_INTERNAL_DEVICE_CAL_RSB_BIT);
                return FALSE;
            }
            ftm_mc_sm_ptr = rflte_ftm_mc_get_sm();
            prx_device = ftm_mc_sm_ptr->carrier[carrier].prx_device;
            drx_device = ftm_mc_sm_ptr->carrier[carrier].drx_device;
         break;
  #endif
  #ifdef FEATURE_TDSCDMA
       case FTM_RF_TECH_TDSCDMA:
         
            prx_device = rf_tdscdma_mc_get_curr_pri_rx_device();
            drx_device = rfc_common_get_preferred_associated_rx_device(prx_device); // temp solution, waiting for TDS to provide api to get current active rx. 
         
         break;
   #endif
       default:
         FTM_MSG(FTM_ERROR, "ftm_enh_internal_device_cal_rsb Unsupported Tech Type");     
         break;
    }
    FTM_MSG_2(FTM_HIGH,"rx_rsb cal: logical prx device :%d, logical drx device:%d", prx_device, drx_device);    

// initialize rsb cal results first. 
    for (rsb_res_dev_index = 0; rsb_res_dev_index < FTM_ENH_INTERNAL_DEVICE_CAL_RSB_RES_MAX ; rsb_res_dev_index ++)
    {
        for(rsb_res_gain_index =0; rsb_res_gain_index< rsb_instr->num_gain_states; rsb_res_gain_index++)
        {
            rsb_result->gain_states[rsb_res_gain_index ] = rsb_instr->gain_states[rsb_res_gain_index];
      		rsb_result->rsb_res[rsb_res_dev_index][rsb_res_gain_index].nv_active = 0;	
            rsb_result->rsb_res[rsb_res_dev_index][rsb_res_gain_index].sin_theta_final = 0;
            rsb_result->rsb_res[rsb_res_dev_index][rsb_res_gain_index].cos_theta_final = 16384;
            rsb_result->rsb_res[rsb_res_dev_index][rsb_res_gain_index].gain_inv_final = 16384; // check for default value
            rsb_result->rsb_res[rsb_res_dev_index][rsb_res_gain_index].sin_theta_after_cal = 0;
            rsb_result->rsb_res[rsb_res_dev_index][rsb_res_gain_index].gain_inv_after_cal = 16384;	  
            rsb_result->rsb_res[rsb_res_dev_index][rsb_res_gain_index].cos_theta_after_cal = 16384;
            rsb_result->rsb_res[rsb_res_dev_index][rsb_res_gain_index].rsb_before_cal = 0;
            rsb_result->rsb_res[rsb_res_dev_index][rsb_res_gain_index].rsb_after_cal = 99;
         }
    }

   //error check
	if( (rsb_instr->num_gain_states !=1) && (rsb_cal_alg == FTM_ENH_INTERNAL_DEVICE_CAL_RSB_ALG_CALLBOX ) ) // for rxrsb cal, we can only support one gain per request to sync with dl power time line) 
    {
	    FTM_MSG_2(FTM_ERROR, "Current Rx Rsb cal only support number of %d gain states, requested number of %d gain states", 1, rsb_instr->num_gain_states );
	    internal_device_cal_data->internal_device_cal_status.error_log = FTM_ENH_INTERNAL_DEVICE_CAL_NUM_OF_GAIN_STATES_UNSUPPORTED ;
	    internal_device_cal_data->internal_device_cal_status.error_mask = (1<< ENH_INTERNAL_DEVICE_CAL_RSB_BIT);	   
	    return FALSE;
    }
    // lte rx rsb cal, gain state is less than 5, wcdma: gain state is less than 3 
    if(((rsb_instr->gain_states[0] >5) && (tech == FTM_RF_TECH_LTE ) ) ||((rsb_instr->gain_states[0] > 3) &&( tech== FTM_RF_TECH_WCDMA )))
    {
	    FTM_MSG_1(FTM_ERROR, "Requested gain state %d not supported", rsb_instr->gain_states[0] );
	    internal_device_cal_data->internal_device_cal_status.error_log = FTM_ENH_INTERNAL_DEVICE_CAL_GAIN_STATE_UNSUPPORTED ;
	    internal_device_cal_data->internal_device_cal_status.error_mask = (1<< ENH_INTERNAL_DEVICE_CAL_RSB_BIT);
	    return FALSE;
    }
	get_rxlm_handler(tech, rxlm_buffer_idx,&rflm_tech_id);

    // Configure TFW state to TRK mode for IQ capture to work
#ifdef FEATURE_TDSCDMA
    if (tech== FTM_RF_TECH_TDSCDMA && rsb_type == FTM_ENH_INTERNAL_DEVICE_CAL_RX_RSB) 
    {
       FTM_MSG(FTM_HIGH,"ftm_tdscdma_override_tfw_ftm_trk_state called");
       ftm_tdscdma_override_tfw_ftm_trk_state();
    }
#endif /* FEATURE_TDSCDMA */


    rx_path_num = 1;
    if(carrier ==0)
    {
       chain_mask = (device_mask & 0x03);
	   rx_path[0] = FTM_ENH_INTERNAL_DEVICE_CAL_RXRSB_RES_PRX1;
	   rx_path[1] = FTM_ENH_INTERNAL_DEVICE_CAL_RXRSB_RES_DRX1;
	}
	else if(carrier == 1 )
	{
	   chain_mask =  ((device_mask >>2) & 0x03); 
	   if(chain_mask == 0)
	   {
	 	  FTM_MSG(FTM_ERROR, "requested dev cal doesn't match to carrier 1");
	        internal_device_cal_data->internal_device_cal_status.error_log = FTM_ENH_INTERNAL_DEVICE_CAL_CARRIER_NOT_MATCH_DEV_ID;
	        internal_device_cal_data->internal_device_cal_status.error_mask = (1<< ENH_INTERNAL_DEVICE_CAL_RSB_BIT);
            return FALSE;		 
	   }
	   rx_path[0] = FTM_ENH_INTERNAL_DEVICE_CAL_RXRSB_RES_PRX2;
	   rx_path[1] = FTM_ENH_INTERNAL_DEVICE_CAL_RXRSB_RES_DRX2;
	}
	else if(carrier ==2)
	{
        chain_mask = ((device_mask >>4) & 0x03);
	    if(chain_mask == 0 )
		{
		    FTM_MSG(FTM_ERROR, "requested dev cal doesn't match to carrier 2");
		    internal_device_cal_data->internal_device_cal_status.error_log = FTM_ENH_INTERNAL_DEVICE_CAL_CARRIER_NOT_MATCH_DEV_ID;
		    internal_device_cal_data->internal_device_cal_status.error_mask = (1<< ENH_INTERNAL_DEVICE_CAL_RSB_BIT);
            return FALSE;			
		}		
		rx_path[0] = FTM_ENH_INTERNAL_DEVICE_CAL_RXRSB_RES_PRX3;
		rx_path[1] = FTM_ENH_INTERNAL_DEVICE_CAL_RXRSB_RES_DRX3;
	}
	if(chain_mask == 0x03)
	{
	    rx_path_num = 2;
	}
	iq_buf_ptr = (uint32 *) modem_mem_alloc( sample_size*sizeof(int32), MODEM_MEM_CLIENT_RFA );	   
	if(iq_buf_ptr == NULL )
	{
	    FTM_MSG(FTM_ERROR, "Enhanced internal device cal: rsb: not able to allocate memory, returns NULL");
	    internal_device_cal_data->internal_device_cal_status.error_log  = FTM_ENH_INTERNAL_DEVICE_CAL_MEM_ALLOC_FAILURE;
	    internal_device_cal_data->internal_device_cal_status.error_mask = (1<< ENH_INTERNAL_DEVICE_CAL_RSB_BIT);
		return FALSE;
	}
	memset((void *) iq_buf_ptr, 0, sample_size*sizeof(int)); /* reset buffer */
	   
	if( (tech == FTM_RF_TECH_LTE ) && (chain_mask &0x02) )
	{
	    iq_drx_buf_ptr = (uint32 *) modem_mem_alloc( sample_size*sizeof(int32), MODEM_MEM_CLIENT_RFA );
	    if(iq_drx_buf_ptr == NULL )
	    {
	        FTM_MSG(FTM_ERROR, "Enhanced internal device cal: rsb: not able to allocate drx memory, returns NULL");
	        internal_device_cal_data->internal_device_cal_status.error_log  = FTM_ENH_INTERNAL_DEVICE_CAL_MEM_ALLOC_FAILURE;
	        internal_device_cal_data->internal_device_cal_status.error_mask = (1<< ENH_INTERNAL_DEVICE_CAL_RSB_BIT);
	        if(iq_buf_ptr != NULL)
	        {
		        modem_mem_free((void*)iq_buf_ptr, MODEM_MEM_CLIENT_RFA);
	        }
	        return FALSE;
	    }
	    memset((void *) iq_drx_buf_ptr, 0, sample_size*sizeof(int));
	}	            	   

    if(dbg_mode_en>=2)
    {
        FTM_MSG_3(FTM_MED,
	       "rsb cal: type(0: rx rsb; 1: fbrx rsb) %d, tech: %d, first time span<decoding ftm instruction>: %d us",
		   rsb_type, tech, rf_time_get_elapsed(start_tick,RF_USEC));  
           start_tick = rf_time_get_tick();
   	}

// Configure TFW state to TRK mode for IQ capture to work
   #ifdef FEATURE_TDSCDMA
   if (tech== FTM_RF_TECH_TDSCDMA && rsb_type == FTM_ENH_INTERNAL_DEVICE_CAL_RX_RSB) 
   {
      FTM_MSG(FTM_HIGH,"ftm_tdscdma_override_tfw_ftm_trk_state called");
      ftm_tdscdma_override_tfw_ftm_trk_state();
   }
   #endif /* FEATURE_TDSCDMA */
   
   // clear current IQMC data 
   rsb_a_coeff = 0;
   rsb_b_coeff = 0x8000;
                
   if(carrier ==0)
   {
      rfcommon_rx_msm_update_rsb_coeff(rxlm_buffer_idx[0],rsb_a_coeff,rsb_b_coeff, rflm_tech_id,msm_carr_bw,dbg_mode_en); 
      rfcommon_rx_msm_update_rsb_coeff(rxlm_buffer_idx[1],rsb_a_coeff,rsb_b_coeff,rflm_tech_id,msm_carr_bw,dbg_mode_en);
   }
   else if(carrier ==1) 
   { 
      rfcommon_rx_msm_update_rsb_coeff(rxlm_buffer_idx[2],rsb_a_coeff,rsb_b_coeff, rflm_tech_id,msm_carr_bw,dbg_mode_en); 
      rfcommon_rx_msm_update_rsb_coeff(rxlm_buffer_idx[3],rsb_a_coeff,rsb_b_coeff,rflm_tech_id,msm_carr_bw,dbg_mode_en);    
   }
   else if(carrier == 2)
   {
      rfcommon_rx_msm_update_rsb_coeff(rxlm_buffer_idx[4],rsb_a_coeff,rsb_b_coeff, rflm_tech_id,msm_carr_bw,dbg_mode_en); 
      rfcommon_rx_msm_update_rsb_coeff(rxlm_buffer_idx[5],rsb_a_coeff,rsb_b_coeff,rflm_tech_id,msm_carr_bw,dbg_mode_en);   
   }
  
   DALSYS_BusyWait(2500);  // give enough time to let iqmc register reset, clear iqmc done
        
   if(dbg_mode_en>=2)
   {
      FTM_MSG_3(FTM_MED,
               "rsb cal: type(0: rx rsb; 1: fbrx rsb) %d, tech: %d, second time span <clear iqmc>: %d us",
                rsb_type, tech, rf_time_get_elapsed(start_tick,RF_USEC));         
      start_tick = rf_time_get_tick();
   }
           
   for ( rsb_res_gain_index = 0; rsb_res_gain_index < rsb_instr->num_gain_states; rsb_res_gain_index++)
   {	 	       
       if (rsb_res_gain_index != 0)
       {
            // add lna set for different tech here for STG rsb cal support
       }
      if(tech == FTM_RF_TECH_LTE)
      {
#ifdef FEATURE_LTE

         if(rsb_cal_alg == FTM_ENH_INTERNAL_DEVICE_CAL_RSB_ALG_CALLBOX )
         {
	        ret_val = ftm_enh_internal_device_cal_rx_iq_prx_drx_capture(FTM_RF_TECH_LTE, 
																   carrier, 
																   chain_mask,
																   sample_size,
																   (uint32 *) iq_buf_ptr,
																   (uint32 *) iq_drx_buf_ptr);
         }
         else if (rsb_cal_alg == FTM_ENH_INTERNAL_DEVICE_CAL_RSB_ALG_STG )
         {
             if(chain_mask & 0x01)
             {
                // Keep default settings: STG is piped to PRx
                rfdevice_antenna_switch_set_pos(prx_device,0,RFM_LTE_MODE,(uint32)band);
                ret_val = ftm_enh_internal_device_cal_rx_iq_prx_drx_capture(FTM_RF_TECH_LTE, 
                                                                                carrier, 
                                                                                0x01,
                                                                                sample_size,
                                                                                (uint32 *) iq_buf_ptr,
                                                                                (uint32 *) iq_drx_buf_ptr);                                                                                                           
             }
             if(chain_mask & 0x02 )
             {
                 // Route the STG to DRx 
                rfdevice_antenna_switch_set_pos(prx_device,1,RFM_LTE_MODE,(uint32)band);         
                ret_val = ftm_enh_internal_device_cal_rx_iq_prx_drx_capture(FTM_RF_TECH_LTE, 
                                                                                 carrier, 
                                                                                 0x02,
                                                                                 sample_size,
                                                                                 (uint32 *) iq_buf_ptr,
                                                                                 (uint32 *) iq_drx_buf_ptr);                                                                                                  
             }

         }

	     if(ret_val == FALSE)
	     {
	        FTM_MSG_2(FTM_MED,"rsb_cal failed on IQ capture, tech:%d, band:%d",tech, band);
		    internal_device_cal_data->internal_device_cal_status.error_log = FTM_ENH_INTERNAL_DEVICE_CAL_IQ_CAPTURE_FAILURE;
		    internal_device_cal_data->internal_device_cal_status.error_mask = (1<< ENH_INTERNAL_DEVICE_CAL_RSB_BIT);
			rsb_result->device_mask = 0;	  
			if(iq_buf_ptr != NULL)
			{
			   modem_mem_free((void*)iq_buf_ptr, MODEM_MEM_CLIENT_RFA);
			}
			if(iq_drx_buf_ptr != NULL)
			{
			   modem_mem_free((void*)iq_drx_buf_ptr, MODEM_MEM_CLIENT_RFA);
			}
			return FALSE;
		 }
         if( (chain_mask & 0x01) &&( iq_buf_ptr != NULL) )
         {
		     ftm_convert_rsb_iq_format(&iq_buf_ptr[0], 1,FTM_RX_RSB_IQ_16bit_to_16bit,sample_size);
         }
         if( (chain_mask & 0x02) && (iq_drx_buf_ptr != NULL) )
         {
		     ftm_convert_rsb_iq_format(&iq_drx_buf_ptr[0], 1,FTM_RX_RSB_IQ_16bit_to_16bit,sample_size);
         }

		 if( dbg_mode_en >= 2 )
		 {
             FTM_MSG_3(FTM_MED,
                            "rsb cal: type(0: rx rsb; 1: fbrx rsb) %d, tech: %d, time span <IQ capture>: %d us",
                            rsb_type, tech, rf_time_get_elapsed(start_tick,RF_USEC));      
             start_tick = rf_time_get_tick();

             if( (iq_buf_ptr !=NULL) && (chain_mask & 0x01))
		     {
		        ftm_enh_internal_device_cal_save_iq( tech,band,rx_path[0], iq_buf_ptr, sample_size, 0);
			    FTM_MSG_3(FTM_MED,
							   "rsb cal: type(0: rx rsb; 1: fbrx rsb) %d, tech: %d, time span <PRX IQ save>: %d us",
							   rsb_type, tech, rf_time_get_elapsed(start_tick,RF_USEC));	  
			    start_tick = rf_time_get_tick();
		     }
			 if ( (iq_drx_buf_ptr!=NULL) && (chain_mask &0x02))
		     {
			     ftm_enh_internal_device_cal_save_iq( tech,band,rx_path[1], iq_drx_buf_ptr, sample_size, 0);
			     FTM_MSG_3(FTM_MED,
						   "rsb cal: type(0: rx rsb; 1: fbrx rsb) %d, tech: %d, time span <DRX IQ save>: %d us",
						   rsb_type, tech, rf_time_get_elapsed(start_tick,RF_USEC));	  
			     start_tick = rf_time_get_tick();
		     }
		 }
	   
		 for(chain_idx =0;chain_idx <2;chain_idx ++)
		 {
		    dcCompVals = 0;
		    if( (chain_mask & (1<<chain_idx)) )
		    {
                if(chain_idx == 0)
                {
                    iq_cap_struct_dc_comped.access_ptr = (uint32*)iq_buf_ptr;
                }
                else
                {
                    iq_cap_struct_dc_comped.access_ptr = (uint32*)iq_drx_buf_ptr;
                }
			    iq_cap_struct_dc_comped.sample_size = sample_size;
			    iq_cap_struct_dc_comped.dcCompVals = dcCompVals;
			    rfcommon_msm_compute_rsb_coeff_from_iq_dc_comped( 
			        iq_cap_struct_dc_comped,
			        (int16*) &rsb_a_coeff,
				    (int16*) &rsb_a_coeff,
				    &rsb_data.i_sq,
					&rsb_data.q_sq,
					&rsb_data.iq);
				
				if(dbg_mode_en>=2)
				{
				    FTM_MSG_3(FTM_MED,
							   "rsb cal: type(0: rx rsb; 1: fbrx rsb) %d, tech: %d, time span <rsb_coeff_computation>: %d us",
							   rsb_type, tech, rf_time_get_elapsed(start_tick,RF_USEC));	  
					
					start_tick = rf_time_get_tick();		
				}
 
		        ftm_enh_device_cal_rsb_get_thermistor(tx_device, band, &therm_data);
                FTM_MSG_1(FTM_MED,"lte rsb cal: thermistor read %d", therm_data); 
				rsb_data.gain_state = rsb_instr->gain_states[rsb_res_gain_index];
                        
		        if( chain_idx == 0 )
		        {
    			    ret_val = rfdevice_lte_rx_calc_rsb_nv_params( prx_device, (rfcom_lte_band_type) band, /*RFCOM_BW_LTE_10MHz*/ bw,
											  rsb_data.i_sq,rsb_data.q_sq, rsb_data.iq, therm_data,
											  &rsb_data.sin_theta_f,&rsb_data.cos_theta_f,&rsb_data.gain_inv_f,0,rsb_instr->gain_states[rsb_res_gain_index],
											  sub_id); 
		        }
                else if( chain_idx == 1 )
                {
    				ret_val = rfdevice_lte_rx_calc_rsb_nv_params( drx_device, (rfcom_lte_band_type) band, bw,
								  rsb_data.i_sq,rsb_data.q_sq, rsb_data.iq, therm_data,
								  &rsb_data.sin_theta_f,&rsb_data.cos_theta_f,&rsb_data.gain_inv_f,0,rsb_instr->gain_states[rsb_res_gain_index],
								  sub_id);
                }        
				if( (ret_val ==TRUE) && ( rsb_data.cos_theta_f !=0) && (rsb_data.gain_inv_f !=0))
				{
				    rsb_result->rsb_res[rx_path[chain_idx]][rsb_res_gain_index].nv_active = 1;
				    rsb_result->rsb_res[rx_path[chain_idx]][rsb_res_gain_index].rsb_before_cal = (int8) floor(compute_rsb(rsb_data));
					
				    rsb_result->rsb_res[rx_path[chain_idx]][rsb_res_gain_index].sin_theta_final = rsb_data.sin_theta_f;
				    rsb_result->rsb_res[rx_path[chain_idx]][rsb_res_gain_index].cos_theta_final = rsb_data.cos_theta_f;
				    rsb_result->rsb_res[rx_path[chain_idx]][rsb_res_gain_index].gain_inv_final = rsb_data.gain_inv_f; // hardcode to only one gain state for now
				}
				else
				{
				    rsb_result->rsb_res[rx_path[chain_idx]][rsb_res_gain_index].nv_active = 0;
				    rsb_result->rsb_res[rx_path[chain_idx]][rsb_res_gain_index].rsb_before_cal = 0;					
				}
				rsb_result->device_mask |= (1<<rx_path[chain_idx]);
				   		
				if ( (dbg_mode_en >= 1) && (rsb_result->rsb_res[rx_path[chain_idx]][rsb_res_gain_index].nv_active == 1))
				{
				    ftm_common_compute_rsb_coeffs_a_b_params(	rsb_data.sin_theta_f,
							  rsb_data.cos_theta_f,
							  rsb_data.gain_inv_f,
							  0,
							  16384,
							  16384,
							  0, /* rsb_cal_is_on - RSB cal not currently in process */
				    		  &rsb_a_coeff,
							  &rsb_b_coeff );
									
					rfcommon_rx_msm_update_rsb_coeff(rxlm_buffer_idx[chain_idx+carrier*2],-rsb_a_coeff,-rsb_b_coeff, rflm_tech_id,msm_carr_bw,dbg_mode_en); 
				 }
		    }
		 }
		 if ( dbg_mode_en >= 1   )
		 {
			 DALSYS_BusyWait(2500);
             if(rsb_cal_alg == FTM_ENH_INTERNAL_DEVICE_CAL_RSB_ALG_CALLBOX )
             {
			    ret_val = ftm_enh_internal_device_cal_rx_iq_prx_drx_capture(FTM_RF_TECH_LTE, 
																		 carrier, 
																		 chain_mask,
																		 sample_size,
																		 (uint32 *) iq_buf_ptr,
																		 (uint32 *) iq_drx_buf_ptr);
             }             
             else if (rsb_cal_alg == FTM_ENH_INTERNAL_DEVICE_CAL_RSB_ALG_STG )
             {
                 if(chain_mask & 0x01)
                 {
                    // Keep default settings: STG is piped to PRx
                    rfdevice_antenna_switch_set_pos(prx_device,0,RFM_LTE_MODE,(uint32)band);
                    ret_val = ftm_enh_internal_device_cal_rx_iq_prx_drx_capture(FTM_RF_TECH_LTE, 
                                                                                    carrier, 
                                                                                    0x01,
                                                                                    sample_size,
                                                                                    (uint32 *) iq_buf_ptr,
                                                                                    (uint32 *) iq_drx_buf_ptr);
                 }
                 if(chain_mask & 0x02 )
                 {
                    // Route the STG to DRx 
                    rfdevice_antenna_switch_set_pos(prx_device,1,RFM_LTE_MODE,(uint32)band);
                    ret_val = ftm_enh_internal_device_cal_rx_iq_prx_drx_capture(FTM_RF_TECH_LTE, 
                                                                                     carrier, 
                                                                                     0x02,
                                                                                     sample_size,
                                                                                     (uint32 *) iq_buf_ptr,
                                                                                     (uint32 *) iq_drx_buf_ptr);
                 }
             
             }
             
             if(ret_val == FALSE)
             {
                 FTM_MSG_2(FTM_MED,"rsb_cal failed on IQ capture, tech:%d, band:%d",tech, band);
                 internal_device_cal_data->internal_device_cal_status.error_log = FTM_ENH_INTERNAL_DEVICE_CAL_IQ_CAPTURE_FAILURE;
                 internal_device_cal_data->internal_device_cal_status.error_mask = (1<< ENH_INTERNAL_DEVICE_CAL_RSB_BIT);
                 rsb_result->device_mask = 0;      
                 if(iq_buf_ptr != NULL)
                 {
                      modem_mem_free((void*)iq_buf_ptr, MODEM_MEM_CLIENT_RFA);
                 }
                 if(iq_drx_buf_ptr != NULL)
                 {
                      modem_mem_free((void*)iq_drx_buf_ptr, MODEM_MEM_CLIENT_RFA);
                 }
                 return FALSE;
              }
              if( (iq_buf_ptr !=NULL) && (chain_mask & 0x01) )
              {
				  ftm_convert_rsb_iq_format(&iq_buf_ptr[0], 1,FTM_RX_RSB_IQ_16bit_to_16bit,sample_size);
              }
              if( (iq_drx_buf_ptr != NULL) && (chain_mask & 0x02))
              {
				  ftm_convert_rsb_iq_format(&iq_drx_buf_ptr[0], 1,FTM_RX_RSB_IQ_16bit_to_16bit,sample_size);
              }
		   
		      if( dbg_mode_en >= 2 )
		      {
		          if( (iq_buf_ptr !=NULL) && (chain_mask & 0x01))
		          {
			         ftm_enh_internal_device_cal_save_iq( tech,band,rx_path[0], iq_buf_ptr, sample_size, 1);
			         FTM_MSG_3(FTM_MED,
						   "rsb cal: type(0: rx rsb; 1: fbrx rsb) %d, tech: %d, time span <IQ save: after rsb cal>: %d us",
						   rsb_type, tech, rf_time_get_elapsed(start_tick,RF_USEC));	  
			               start_tick = rf_time_get_tick();
		          }
			      if ( (iq_drx_buf_ptr!=NULL) && (chain_mask &0x02))
		          {
				     ftm_enh_internal_device_cal_save_iq( tech,band,rx_path[1], iq_drx_buf_ptr, sample_size, 1);
				     FTM_MSG_3(FTM_MED,
						   "rsb cal: type(0: rx rsb; 1: fbrx rsb) %d, tech: %d, time span <IQ save after rsb cal>: %d us",
						   rsb_type, tech, rf_time_get_elapsed(start_tick,RF_USEC));	  
				     start_tick = rf_time_get_tick();
		          }
		      }		   
			  for(chain_idx =0;chain_idx < 2; chain_idx ++)
			  {
		 		  if( ( (chain_mask & (1<<chain_idx)) ) && ( rsb_result->rsb_res[rx_path[chain_idx]][rsb_res_gain_index].nv_active == 1) )
				  {				    					    
                     if(chain_idx == 0)
                     {
                         iq_cap_struct_dc_comped.access_ptr = (uint32*)iq_buf_ptr;
                     }
                     else
                     {
                         iq_cap_struct_dc_comped.access_ptr = (uint32*)iq_drx_buf_ptr;
                     }                            
					 iq_cap_struct_dc_comped.sample_size = sample_size;
					 iq_cap_struct_dc_comped.dcCompVals = dcCompVals;
					 rfcommon_msm_compute_rsb_coeff_from_iq_dc_comped( 
						  iq_cap_struct_dc_comped,
						  (int16*) &rsb_a_coeff,
						  (int16*) &rsb_a_coeff,
						  &rsb_data.i_sq,
						  &rsb_data.q_sq,
						  &rsb_data.iq);
					  
					 if(dbg_mode_en>=2)
					 {
					      FTM_MSG_3(FTM_MED,
							 "rsb cal: type(0: rx rsb; 1: fbrx rsb) %d, tech: %d, time span <rsb_coeff_computation>: %d us",
							 rsb_type, tech, rf_time_get_elapsed(start_tick,RF_USEC));		
					  
					      start_tick = rf_time_get_tick();		  
					 }
					  
                     therm_data = 0; // approcimate after_cal rsb, no temp/bw compensation, not call device api to avoid device apply post-cal rsb data
                     if(chain_idx == 0)
                     {
                          ret_val = ftm_compute_rsb_nv_params( 
                             prx_device,
                             tech,                                   
                             band,                               
                             rsb_data.i_sq,  rsb_data.q_sq,  rsb_data.iq,  therm_data, 
                                &rsb_data.sin_theta_f,&rsb_data.cos_theta_f,&rsb_data.gain_inv_f,0);
                     }
                     else if(chain_idx == 1 )
                     {
                          ret_val = ftm_compute_rsb_nv_params( 
                                    drx_device,
                                    tech,                                   
                                    band,                               
                                    rsb_data.i_sq,  rsb_data.q_sq,  rsb_data.iq,  therm_data, 
                                    &rsb_data.sin_theta_f,&rsb_data.cos_theta_f,&rsb_data.gain_inv_f,0);

                     }
    
                     if(ret_val == TRUE)
                     {
					    rsb_result->rsb_res[rx_path[chain_idx]][rsb_res_gain_index].sin_theta_after_cal = rsb_data.sin_theta_f;
					    rsb_result->rsb_res[rx_path[chain_idx]][rsb_res_gain_index].cos_theta_after_cal = rsb_data.cos_theta_f;
					    rsb_result->rsb_res[rx_path[chain_idx]][rsb_res_gain_index].gain_inv_after_cal = rsb_data.gain_inv_f; // hardcode to only one gain state for now
					    rsb_result->rsb_res[rx_path[chain_idx]][rsb_res_gain_index].rsb_after_cal = (int8) floor(compute_rsb(rsb_data));	  
                     }
                     else
                     {
                        rsb_result->rsb_res[rx_path[chain_idx]][rsb_res_gain_index].rsb_after_cal = 0;
                     }
				  }
			    }  
			 }
 #else
			 FTM_MSG(FTM_HIGH,"FEATURE_LTE is not enabled in this build");	
 #endif
            }
		    else
		    {
			    for (rx_path_idx = 0; rx_path_idx < rx_path_num; rx_path_idx++)
			    {
			        if(chain_mask == 0x02)
			        {
				        rx_path_idx = 1;
			        }	  
			        if (iq_buf_ptr != NULL)
			        {
				        memset(iq_buf_ptr, 0,sample_size*4); // reset iq data to be zero
			        }
                    if(rx_path_idx == 0)
                    {
 			            ret_val = ftm_enh_internal_device_cal_rx_iq_capture(tech, carrier, prx_device, sample_size,iq_buf_ptr);
                    }
                    else if(rx_path_idx == 1)
                    {
 			            ret_val = ftm_enh_internal_device_cal_rx_iq_capture(tech, carrier, drx_device, sample_size,iq_buf_ptr);
                    }
		            if(ret_val == FALSE)
		            {
				        FTM_MSG_2(FTM_MED,"rsb_cal failed on IQ capture, tech:%d, band:%d",tech, band);
				        internal_device_cal_data->internal_device_cal_status.error_log = FTM_ENH_INTERNAL_DEVICE_CAL_IQ_CAPTURE_FAILURE;
				        internal_device_cal_data->internal_device_cal_status.error_mask = (1<< ENH_INTERNAL_DEVICE_CAL_RSB_BIT);
  	                    rsb_result->device_mask = 0;	
				        if(iq_buf_ptr != NULL)
				        {
				            modem_mem_free((void*)iq_buf_ptr, MODEM_MEM_CLIENT_RFA);
				        }
				        if(iq_drx_buf_ptr != NULL)
				        {
				            modem_mem_free((void*)iq_drx_buf_ptr, MODEM_MEM_CLIENT_RFA);
				        }
				        return FALSE;
		            }
		            if(dbg_mode_en>=2)
        		    {
			            FTM_MSG_3(FTM_MED,
							   "rsb cal: type(0: rx rsb; 1: fbrx rsb) %d, tech: %d, time span <rx IQ capture>: %d us",
							    rsb_type, tech, rf_time_get_elapsed(start_tick,RF_USEC));	  
			            start_tick = rf_time_get_tick();
		            }
		            if(tech == FTM_RF_TECH_WCDMA )
			        {
			            ftm_convert_iq_8b_to_16b( &iq_buf_ptr[0], 0, sample_size);
			        }
			        else if( (tech == FTM_RF_TECH_TDSCDMA ) )
			        {
			            ftm_convert_rsb_iq_format(&iq_buf_ptr[0], 1,FTM_RX_RSB_IQ_16bit_to_16bit,sample_size);		
			        }
		   	
		            if(dbg_mode_en>=2)
		            {
			            FTM_MSG_3(FTM_MED,
							   "rsb cal: type(0: rx rsb; 1: fbrx rsb) %d, tech: %d, time span <convert IQ data format>: %d us",
							   rsb_type, tech, rf_time_get_elapsed(start_tick,RF_USEC));	  
			            start_tick = rf_time_get_tick();
			            ftm_enh_internal_device_cal_save_iq( tech,band,rx_path[rx_path_idx], iq_buf_ptr, sample_size, 0);
			            FTM_MSG_3(FTM_MED,
							   "rsb cal: type(0: rx rsb; 1: fbrx rsb) %d, tech: %d, time span <IQ save>: %d us",
							   rsb_type, tech, rf_time_get_elapsed(start_tick,RF_USEC));	  
			            start_tick = rf_time_get_tick();
		            }
	     		
    	            if(dbg_mode_en >= 4 )
     	            {
	 		            for (id = 0;id < 5;id++)
	  		            {
				            FTM_MSG_4(FTM_MED,"iq_buf_ptr:0x%x, 0x%x, 0x%x, 0x%x", iq_buf_ptr[id*4],iq_buf_ptr[id*4+1],
					        iq_buf_ptr[id*4+2],iq_buf_ptr[id*4+3]);		
	  		            }			
     	            }		
 					iq_cap_struct_dc_comped.access_ptr = (uint32*)iq_buf_ptr;
					iq_cap_struct_dc_comped.sample_size = sample_size;
					iq_cap_struct_dc_comped.dcCompVals = dcCompVals;
					rfcommon_msm_compute_rsb_coeff_from_iq_dc_comped( 
									  iq_cap_struct_dc_comped,
									  (int16*) &rsb_a_coeff,
									  (int16*) &rsb_a_coeff,
									  &rsb_data.i_sq,
									  &rsb_data.q_sq,
									  &rsb_data.iq);
                    
                    ftm_enh_device_cal_rsb_get_thermistor(tx_device, band, &therm_data);
                    rsb_data.gain_state = rsb_instr->gain_states[rsb_res_gain_index];
	                if(tech == FTM_RF_TECH_WCDMA)
	                {
            #ifdef FEATURE_WCDMA
                        if(rx_path_idx == 0 )
                        {
                            ret_val = rfdevice_wcdma_rx_get_rsb_cal_data_temp_comp(prx_device, (rfcom_wcdma_band_type) band, &rsb_data,  asdiv_alt_gain);
                        }
                        else if (rx_path_idx == 1 )
                        {
            			 ret_val = rfdevice_wcdma_rx_get_rsb_cal_data_temp_comp(drx_device, (rfcom_wcdma_band_type) band, &rsb_data, asdiv_alt_gain); 
                        }
	        #else
                        FTM_MSG(FTM_HIGH,"FEATURE_WCDMA is not enabled in this build");	  
            #endif
	                }
	                else if (tech == FTM_RF_TECH_TDSCDMA)
	                {
            #ifdef FEATURE_TDSCDMA
                        if(rx_path_idx == 0)
                        {
                            ret_val = rfdevice_tdscdma_rx_get_rsb_nv_params(prx_device, (rfcom_tdscdma_band_type) band, 
		  								    rsb_data.i_sq,rsb_data.q_sq, rsb_data.iq,therm_data,
		  									&rsb_data.sin_theta_f,&rsb_data.cos_theta_f,&rsb_data.gain_inv_f);
                        }
                        else if(rx_path_idx == 1)
                        {
                            ret_val = rfdevice_tdscdma_rx_get_rsb_nv_params(drx_device, (rfcom_tdscdma_band_type) band, 
		  								    rsb_data.i_sq,rsb_data.q_sq, rsb_data.iq,therm_data,
		  									&rsb_data.sin_theta_f,&rsb_data.cos_theta_f,&rsb_data.gain_inv_f);
                        }
            #else
                        FTM_MSG(FTM_HIGH,"FEATURE_TDSCDMA is not enabled in this build");	  
            #endif
	                }
                    if(dbg_mode_en >=2)
                    {
		                FTM_MSG_3(FTM_MED,
							   "rsb cal: type(0: rx rsb; 1: fbrx rsb) %d, tech: %d, time span < compensation from device driver>: %d us",
							   rsb_type, tech, rf_time_get_elapsed(start_tick,RF_USEC));	  			
		                start_tick = rf_time_get_tick();	
                    }
	                FTM_MSG_3(FTM_MED, "before cal, rsb sin_theta: %d, cos_theta: %d, gain_inv: %d", rsb_data.sin_theta_f,rsb_data.cos_theta_f, rsb_data.gain_inv_f);
	                if( ret_val == TRUE )
	                {
	                    if( ( rsb_data.cos_theta_f !=0) && (rsb_data.gain_inv_f !=0))
	                    {
	                        rsb_result->rsb_res[rx_path[rx_path_idx]][rsb_res_gain_index].nv_active = 1;			  
			                rsb_result->rsb_res[rx_path[rx_path_idx]][rsb_res_gain_index].sin_theta_final = rsb_data.sin_theta_f;
			                rsb_result->rsb_res[rx_path[rx_path_idx]][rsb_res_gain_index].cos_theta_final = rsb_data.cos_theta_f;
			                rsb_result->rsb_res[rx_path[rx_path_idx]][rsb_res_gain_index].gain_inv_final = rsb_data.gain_inv_f; // hardcode to only one gain state for now	
			                rsb_result->rsb_res[rx_path[rx_path_idx]][rsb_res_gain_index].rsb_before_cal = (int8) floor(compute_rsb(rsb_data));
	                    }
			            else
			            {
			                rsb_result->rsb_res[rx_path[rx_path_idx]][rsb_res_gain_index].nv_active = 0;
			                rsb_result->rsb_res[rx_path[rx_path_idx]][rsb_res_gain_index].rsb_before_cal = 0;
                            
			            }	        
			            rsb_result->device_mask |= (1<<rx_path[rx_path_idx]);
		                if (dbg_mode_en >= 1)
		                {								
			                if(rsb_result->rsb_res[rx_path[rx_path_idx]][rsb_res_gain_index].nv_active == 1)
			                {
			                    ftm_common_compute_rsb_coeffs_a_b_params(   rsb_data.sin_theta_f,
														  rsb_data.cos_theta_f,
														  rsb_data.gain_inv_f,
														  0,
														  16384,
														  16384,
														  0, /* rsb_cal_is_on - RSB cal not currently in process */
														  &rsb_a_coeff,
														  &rsb_b_coeff );
	
				                rfcommon_rx_msm_update_rsb_coeff(rxlm_buffer_idx[rx_path_idx+carrier*2],-rsb_a_coeff,-rsb_b_coeff, rflm_tech_id,msm_carr_bw,dbg_mode_en); 			 
			                    DALSYS_BusyWait(2500);  /// give HW enough time to update iqmc register, move here to save cal-time
		                        memset(iq_buf_ptr, 0,sample_size*4);
                                if(rx_path_idx == 0)
                                {
                                    ftm_enh_internal_device_cal_rx_iq_capture(tech, carrier, prx_device, sample_size,iq_buf_ptr);
                                }
                                else if(rx_path_idx == 1)
                                {
		                            ftm_enh_internal_device_cal_rx_iq_capture(tech, carrier, drx_device, sample_size,iq_buf_ptr);
                                }
			
		                        if(tech == FTM_RF_TECH_WCDMA )
		                        {
	 	                            ftm_convert_iq_8b_to_16b( iq_buf_ptr, 1, sample_size);
		                        }
		                        else if( tech == FTM_RF_TECH_TDSCDMA  )
		                        {
			                        ftm_convert_rsb_iq_format(&iq_buf_ptr[0], 0,FTM_RX_RSB_IQ_16bit_to_16bit,sample_size);
		                        }
                                if( dbg_mode_en >= 2)  // re-check if iq is good after dynamic iqmc register update
                                {
                                    ftm_enh_internal_device_cal_save_iq( tech, band,rx_path[rx_path_idx], iq_buf_ptr, sample_size, 1);                                
                                    FTM_MSG_3(FTM_MED,
                                           "rsb cal: type(0: rx rsb; 1: fbrx rsb) %d, tech: %d, time span <IQ save: after cal rsb>: %d us",
                                           rsb_type, tech, rf_time_get_elapsed(start_tick,RF_USEC));      
                                    start_tick = rf_time_get_tick();
                                }           

				                dcCompVals = 0;
				                iq_cap_struct_dc_comped.access_ptr = (uint32*)iq_buf_ptr;
                                iq_cap_struct_dc_comped.sample_size = sample_size;
                                iq_cap_struct_dc_comped.dcCompVals = dcCompVals;
                                rfcommon_msm_compute_rsb_coeff_from_iq_dc_comped( 
                                                                            iq_cap_struct_dc_comped,
                                                                            (int16*) &rsb_a_coeff,
                                                                            (int16*) &rsb_a_coeff,
                                                                            &rsb_data.i_sq,
                                                                            &rsb_data.q_sq,
                                                                            &rsb_data.iq);
	
			                    therm_data = 0; // approcimate after_cal rsb, no temp/bw compensation, not call device api to avoid device apply post-cal rsb data
			                    ret_val = ftm_compute_rsb_nv_params( 
  							            (rfm_device_enum_type)rx_path[rx_path_idx],
  							            tech,
  							            band,   							
  							            rsb_data.i_sq,  rsb_data.q_sq,  rsb_data.iq,  therm_data, 
							            &rsb_data.sin_theta_f,&rsb_data.cos_theta_f,&rsb_data.gain_inv_f,0);
			                    FTM_MSG_3(FTM_MED, "after cal, rsb sin_theta: %d, cos_theta: %d, gain_inv: %d", rsb_data.sin_theta_f,rsb_data.cos_theta_f, rsb_data.gain_inv_f   );
			                    if(ret_val == TRUE)
			                    {
 			                        rsb_result->rsb_res[rx_path[rx_path_idx]][rsb_res_gain_index].cos_theta_after_cal = rsb_data.cos_theta_f;
			                        rsb_result->rsb_res[rx_path[rx_path_idx]][rsb_res_gain_index].sin_theta_after_cal = rsb_data.sin_theta_f;	
			                        rsb_result->rsb_res[rx_path[rx_path_idx]][rsb_res_gain_index].gain_inv_after_cal = rsb_data.gain_inv_f;
			                        rsb_result->rsb_res[rx_path[rx_path_idx]][rsb_res_gain_index].rsb_after_cal = (int8) floor(compute_rsb(rsb_data));  
			                    }
			                    else
			                    {
			                        rsb_result->rsb_res[rx_path[rx_path_idx]][rsb_res_gain_index].rsb_after_cal = 0; 	
			                    }                                
			                } 
			                else
			                {
				                rsb_result->rsb_res[rx_path[rx_path_idx]][rsb_res_gain_index].rsb_after_cal = 0; 
			                } 
		  	            }
	                }
	                else
	                {
		                rsb_result->rsb_res[rx_path[rx_path_idx]][rsb_res_gain_index].rsb_before_cal = 0; 
		                rsb_result->rsb_res[rx_path[rx_path_idx]][rsb_res_gain_index].rsb_after_cal = 0; 
		 //add error mesg here
	                }
	                FTM_MSG_7(FTM_MED,"rsb cal: tech:%d, band:%d, carrier:%d, rx_path:%d, nv_active: %d, rsb_before_cal:%d, rsb_after_cal:%d",
	  	                tech,band,carrier,rx_path[rx_path_idx],rsb_result->rsb_res[rx_path[rx_path_idx]][rsb_res_gain_index].nv_active, rsb_result->rsb_res[rx_path[rx_path_idx]][rsb_res_gain_index].rsb_before_cal, 
    	                rsb_result->rsb_res[rx_path[rx_path_idx]][rsb_res_gain_index].rsb_after_cal);
                    if(rsb_result->rsb_res[rx_path[rx_path_idx]][rsb_res_gain_index].nv_active == 0)
                    {
                        FTM_MSG_2(FTM_MED,"rsb_cal failed on device rsb temp comp, tech:%d, band:%d",tech, band);
                        internal_device_cal_data->internal_device_cal_status.error_log = FTM_ENH_INTERNAL_DEVICE_CAL_RXRSB_DEVICE_COMPENSATION_FAIL;
                        internal_device_cal_data->internal_device_cal_status.error_mask = (1<< ENH_INTERNAL_DEVICE_CAL_RSB_BIT);
                        rsb_result->device_mask = 0;      
                        if(iq_buf_ptr != NULL)
                        {
                            modem_mem_free((void*)iq_buf_ptr, MODEM_MEM_CLIENT_RFA);
                        }
                        if(iq_drx_buf_ptr != NULL)
                        {
                            modem_mem_free((void*)iq_drx_buf_ptr, MODEM_MEM_CLIENT_RFA);
                        }
                        return FALSE;
                    }
	            }
 	        }	
   	    }  	

        if(rsb_result->device_mask != 0)
        {
   	        *res_len = sizeof(ftm_enh_internal_device_cal_rsb_result_type);
        }   
        else
   	        *res_len = 0;    
    if(iq_buf_ptr !=NULL)
   	    modem_mem_free(iq_buf_ptr,MODEM_MEM_CLIENT_RFA);
    if(iq_drx_buf_ptr !=NULL)
        modem_mem_free(iq_drx_buf_ptr,MODEM_MEM_CLIENT_RFA);

    FTM_MSG_3(FTM_MED,
                     "rsb cal: type(0: rx rsb; 1: fbrx rsb) %d, tech: %d, complete in %d us",
                     rsb_type, tech, rf_time_get_elapsed(start_tick0,RF_USEC));   
    return ret_val;  
}


/*!
  @name: ftm_enh_internal_device_cal_fbrx_rsb

  @brief: rsb calibration
  
 
  @details
  
  @param
  input: req_instr, payload instruction
  output: res_len:  cal result length
             rsp_payload: output of fbrx rsb cal results
  
  @return
     success or fail
*/

boolean ftm_enh_internal_device_cal_fbrx_rsb
(
  void const *req_instr,
  uint16 *res_len,
  void *rsp_payload 
)
{
    boolean ret_val = TRUE;
    ftm_enh_internal_device_cal_error_code_type api_status = FTM_ENH_INTERNAL_DEVICE_CAL_RESULTS_SUCCESS;
    ftm_rf_technology_type curr_ftm_rf_tech = FTM_RF_TECH_UNKNOWN;

//  extract rsb cal-parameter
    ftm_enh_internal_device_cal_instr_payload_rsb_type * rsb_instr = (ftm_enh_internal_device_cal_instr_payload_rsb_type *) req_instr;
    uint8 rsb_type = rsb_instr->rsb_type;
    uint8 rsb_cal_alg = rsb_instr->rsb_cal_alg;
    uint32 sample_size = rsb_instr->iq_sample_size;
    int32  stg_offset_khz = rsb_instr->offset; 
    uint8 carrier = rsb_instr->carrier;
    uint8 cur_gain_state = rsb_instr->gain_states[0];
   
    ftm_enh_internal_device_cal_rsb_result_type * rsb_result = (ftm_enh_internal_device_cal_rsb_result_type *) rsp_payload;

// extract common enhanced internal cal info parameters

    ftm_enh_internal_device_cal_data_type *internal_device_cal_data = ftm_common_data_get_enh_internal_device_cal_ptr();
    uint8 tech = internal_device_cal_data->internal_device_cal_config.tech;
    uint16 band = internal_device_cal_data->internal_device_cal_config.band;
    uint8  bw = internal_device_cal_data->internal_device_cal_config.bw;     
    uint16 device_mask = internal_device_cal_data->internal_device_cal_config.device_mask;
    uint8 dbg_mode_en = internal_device_cal_data->internal_device_cal_config.dbg_mode_en;

// local parameters

    uint8 rx_path[2] = {FTM_ENH_INTERNAL_DEVICE_CAL_RSB_RES_FBRX,FTM_ENH_INTERNAL_DEVICE_CAL_RSB_RES_FBRX_ULCA};	

   // rxlm and device parameter 
    rflm_handle_tx_t fbrx_buffer_idx[4]={0,0,0,0};
    rflm_tech_id_t rflm_tech_id; 
    rfdevice_rxtx_common_class* device_ptr = NULL;
    rfcom_band_type_u band_union;
    rfcom_bw_type_u rfcom_bw;
    rfm_device_enum_type tx_device = RFM_DEVICE_0;
#ifdef FEATURE_LTE
    rfcom_bw.lte_bw = 3; 
#endif
#ifdef FEATURE_CDMA
	const rf_cdma_data_status_type *dev_status_r;
#endif

	// buffer for iq capture   
    uint32 *iq_buf_ptr = NULL;
    
    // rsb data
    int32 rsb_a_coeff, rsb_b_coeff;  
    rfdevice_rsb_cal_data_type rsb_data = {0,0,0,0,0,0,0} ;
   
    ftm_common_msm_iq_cap_info iq_cap_struct_dc_comped;   
    uint32 dcCompVals = 0; // for FBRx RSB computation with DC removal

    uint16 therm_data =0; 
   
    int id =0; //for loop id
    uint8 rsb_res_dev_index = 0;
    uint8 rsb_res_gain_index = 0;

    // cal time profile
    rf_time_tick_type start_tick,start_tick0;

    *res_len = 0;   
    carrier = 0; // force to 0 now, but can be extended for future ULCA support
    
    band = ftm_calv4_convert_band(tech, band);

    start_tick0 = rf_time_get_tick();

    internal_device_cal_data->internal_device_cal_status.error_log  = FTM_ENH_INTERNAL_DEVICE_CAL_RESULTS_SUCCESS;
    // init results as default, no rsb comp
    rsb_result->device_mask = 0;
	if(rsb_instr->num_gain_states > MAX_NUM_RSB_GAIN_STATES)
    {
       FTM_MSG_2(FTM_ERROR,"Maximum num of gain states supported %d, requested num of gain states %d", MAX_NUM_RSB_GAIN_STATES, rsb_instr->num_gain_states);
	   return FALSE;
    }
    rsb_result->num_gain_states = rsb_instr->num_gain_states;
    internal_device_cal_data->internal_device_cal_status.error_mask = 0;   

   
    FTM_MSG_7(FTM_MED,"enh_internal_device_cal:tech: %d, device_mask%d, rsb type %d, stg_offset: %d, alg: %d, carrier:%d, sampleSize: %d",tech, device_mask, rsb_type, stg_offset_khz,
		   rsb_cal_alg, carrier,sample_size);

    FTM_MSG_5(FTM_MED,"enh_internal_device_cal: num_gain:%d, gain states: %d, %d, %d, %d",rsb_instr->num_gain_states, rsb_instr->gain_states[0],rsb_instr->gain_states[1],
   		rsb_instr->gain_states[2],rsb_instr->gain_states[3]);

    curr_ftm_rf_tech = ftm_get_rf_technology();

    if ( tech != curr_ftm_rf_tech)
    {
	    FTM_MSG_2(FTM_ERROR, "Current RF Tech %d does not match Tech for RxRsb/FbRsb Cal %d", curr_ftm_rf_tech, tech );
	    internal_device_cal_data->internal_device_cal_status.error_log = FTM_ENH_INTERNAL_DEVICE_CAL_TECH_MISMATCH;
	    internal_device_cal_data->internal_device_cal_status.error_mask = (1<< ENH_INTERNAL_DEVICE_CAL_RSB_BIT);
	    return FALSE;
    }

    if(dbg_mode_en >=2 )
    {
        start_tick = rf_time_get_tick();
    }
    // initialize rsb cal results first. 
    for (rsb_res_dev_index = 0; rsb_res_dev_index < FTM_ENH_INTERNAL_DEVICE_CAL_RSB_RES_MAX ; rsb_res_dev_index ++)
    {
        for(rsb_res_gain_index =0; rsb_res_gain_index< rsb_instr->num_gain_states; rsb_res_gain_index++)
        {
            rsb_result->gain_states[rsb_res_gain_index ] = rsb_instr->gain_states[rsb_res_gain_index];
		    rsb_result->rsb_res[rsb_res_dev_index][rsb_res_gain_index].nv_active = 0;	
	        rsb_result->rsb_res[rsb_res_dev_index][rsb_res_gain_index].sin_theta_final = 0;
	        rsb_result->rsb_res[rsb_res_dev_index][rsb_res_gain_index].cos_theta_final = 16384;
	        rsb_result->rsb_res[rsb_res_dev_index][rsb_res_gain_index].gain_inv_final = 16384; // check for default value
	        rsb_result->rsb_res[rsb_res_dev_index][rsb_res_gain_index].sin_theta_after_cal = 0;
	        rsb_result->rsb_res[rsb_res_dev_index][rsb_res_gain_index].gain_inv_after_cal = 16384;	  
	        rsb_result->rsb_res[rsb_res_dev_index][rsb_res_gain_index].cos_theta_after_cal = 16384;
	        rsb_result->rsb_res[rsb_res_dev_index][rsb_res_gain_index].rsb_before_cal = 0;
	        rsb_result->rsb_res[rsb_res_dev_index][rsb_res_gain_index].rsb_after_cal = 99;
        }
    }

    if(rsb_instr->num_gain_states >4) // this is to check fbrx_cal, max number of fbrx gain_state to check is 4 
    {
        FTM_MSG_2(FTM_ERROR, "Current Fbrx Rsb cal only support number of %d gain states, requested number of %d gain states", 4, rsb_instr->num_gain_states );
        internal_device_cal_data->internal_device_cal_status.error_log = FTM_ENH_INTERNAL_DEVICE_CAL_NUM_OF_GAIN_STATES_UNSUPPORTED ;
	    internal_device_cal_data->internal_device_cal_status.error_mask = (1<< ENH_INTERNAL_DEVICE_CAL_RSB_BIT);
	    return FALSE;
    }

    switch (curr_ftm_rf_tech ) 
    {
       #if defined(FEATURE_CDMA1X) && defined(FEATURE_CDMA) 
        case FTM_RF_TECH_CDMA:
            tx_device = rf_cdma_data_get_curr_tx_device();
            dev_status_r = rf_cdma_get_device_status( tx_device ); // what is the current tx device?
            if( dev_status_r != NULL)
            {
                band_union.cdma_band = dev_status_r->curr_band;
		        device_ptr = rfcommon_core_get_tx_device_ptr(tx_device, RFCOM_1X_MODE, band_union);
            }
            else
            {
                FTM_MSG(FTM_ERROR, "rfcommon_core_get_tx_device_ptr(0) returns NULL");
            }
            break;
       #endif

       #ifdef FEATURE_WCDMA
        case FTM_RF_TECH_WCDMA:
        {
          #ifdef ATLAS_RF_WCDMA_PRESI
            rfm_wcdma_ftm_radio_state_data_type radio_state;
            tx_device = rfm_wcdma_ftm_tx_get_rfm_device();
            if( tx_device < RFM_MAX_WAN_DEVICES )
            {
                rfm_wcdma_ftm_tx_get_radio_state( tx_device, &radio_state );
                band_union.wcdma_band = (rfcom_wcdma_band_type)radio_state.rf_band;
                device_ptr = rfcommon_core_get_tx_device_ptr(tx_device, RFCOM_WCDMA_MODE, band_union);
            }
            else
            {
                device_ptr = NULL;
            }
          #else
            band_union.wcdma_band = band_union.wcdma_band = rfwcdma_data_get_curr_band(tx_device);
            tx_device = rfwcdma_data_get_tx_device();
            device_ptr = rfcommon_core_get_tx_device_ptr(tx_device, RFCOM_WCDMA_MODE, band_union);
          #endif
         }
         break;
       #endif /* FEATURE_WCDMA */
       #ifdef FEATURE_LTE
       case FTM_RF_TECH_LTE:
            band_union.lte_band = rflte_ftm_mc_sm_get_tx_band();
            tx_device = rflte_ftm_mc_sm_get_tx_device(); 
            rfcom_bw.lte_bw = rflte_ftm_mc_sm_get_tx_bw();
            device_ptr = rfcommon_core_get_tx_device_ptr(tx_device, RFCOM_LTE_MODE, band_union);
         break;
       #endif
       #ifdef FEATURE_TDSCDMA
       case FTM_RF_TECH_TDSCDMA:
            band_union.tdscdma_band = rf_tdscdma_mc_state.curr_band_tx;
            tx_device = rf_tdscdma_mc_get_curr_tx_device();
            device_ptr = rfcommon_core_get_tx_device_ptr(tx_device, RFCOM_TDSCDMA_MODE, band_union);
         break;
       #endif
       default:
            FTM_MSG(FTM_ERROR, "ftm_enh_internal_device_cal_rsb Unsupported Tech Type");
     }

	   /* check for null pointer and throw error */
     if (device_ptr == NULL) 
     {
	    FTM_MSG(FTM_ERROR, "rfc_common_get_cmn_device_object(0) returns NULL");
	    internal_device_cal_data->internal_device_cal_status.error_log = FTM_ENH_INTERNAL_DEVICE_CAL_FBRX_DEV_ACTION_FAILED;
	    internal_device_cal_data->internal_device_cal_status.error_mask = (1<< ENH_INTERNAL_DEVICE_CAL_RSB_BIT);
	    return FALSE;
     }
     if(dbg_mode_en>=2)
     {
        FTM_MSG_3(FTM_MED,
              "rsb cal: type(0: rx rsb; 1: fbrx rsb) %d, tech: %d, first time span<decoding ftm instruction>: %d us",
              rsb_type, tech, rf_time_get_elapsed(start_tick,RF_USEC));  
        start_tick = rf_time_get_tick();
     }
     

     iq_buf_ptr = (uint32 *) modem_mem_alloc( sample_size*sizeof(int32), MODEM_MEM_CLIENT_RFA );
     if(iq_buf_ptr == NULL )
     {
         FTM_MSG(FTM_ERROR, "Enhanced internal device cal: rsb: not able to allocate memory, returns NULL");
         internal_device_cal_data->internal_device_cal_status.error_log  = FTM_ENH_INTERNAL_DEVICE_CAL_MEM_ALLOC_FAILURE;
         internal_device_cal_data->internal_device_cal_status.error_mask = (1<< ENH_INTERNAL_DEVICE_CAL_RSB_BIT);
         return FALSE;
     }
     memset((void *) iq_buf_ptr, 0, sample_size*4    ); /* reset buffer */

    // clear current IQMC data 
     get_fbrx_lm_handler(tech,fbrx_buffer_idx,&rflm_tech_id);      
     rsb_a_coeff = 0;
     rsb_b_coeff = 0x8000;
           
    for ( rsb_res_gain_index = 0; rsb_res_gain_index < rsb_instr->num_gain_states; rsb_res_gain_index++)
    {
        cur_gain_state = rsb_instr->gain_states[rsb_res_gain_index];
        ret_val = rfcommon_fbrx_mc_enable_rf_dev(device_ptr, cur_gain_state );
        if (ret_val == FALSE)
        {
            FTM_MSG_1(FTM_ERROR, "Failed to Enable FBRx Device in Gain State %d " , cur_gain_state);
            internal_device_cal_data->internal_device_cal_status.error_log = FTM_ENH_INTERNAL_DEVICE_CAL_FBRX_DEV_ACTION_FAILED;
            internal_device_cal_data->internal_device_cal_status.error_mask = (1<< ENH_INTERNAL_DEVICE_CAL_RSB_BIT);
            if(iq_buf_ptr != NULL)
            {
                modem_mem_free((void*)iq_buf_ptr, MODEM_MEM_CLIENT_RFA);
            }
            return FALSE;
        }
        FTM_MSG(FTM_MED,"enabled fbrx_dev");
        api_status = ftm_enh_internal_device_cal_fbrx_iq_cap_config(tech, bw, band_union,tx_device);                                                                
        if (api_status != FTM_ENH_INTERNAL_DEVICE_CAL_RESULTS_SUCCESS)
        {
           FTM_MSG_1(FTM_ERROR, "ftm_enh_internal_device_cal_fbrx_iq_cap_config failed with error code %d", api_status );       
           if(iq_buf_ptr != NULL)
           {
               modem_mem_free((void*)iq_buf_ptr, MODEM_MEM_CLIENT_RFA);
           }
           rfcommon_fbrx_mc_disable_rf_dev(device_ptr);
           ftm_enh_internal_device_cal_fbrx_iq_cap_deconfig(tech,tx_device);
           internal_device_cal_data->internal_device_cal_status.error_log = api_status;
           internal_device_cal_data->internal_device_cal_status.error_mask = (1<< ENH_INTERNAL_DEVICE_CAL_RSB_BIT);
           return FALSE;
        }
	    FTM_MSG(FTM_MED,"fbrx_iq_cap_configured");
    	if(dbg_mode_en>=2)
		{
		    FTM_MSG_3(FTM_MED,
				   "rsb cal: type(0: rx rsb; 1: fbrx rsb) %d, gain_state: %d, time span <turn on fbrx dev>: %d us",
				   rsb_type, cur_gain_state, rf_time_get_elapsed(start_tick,RF_USEC));    
            start_tick = rf_time_get_tick();
		}        
        
		rfcommon_fbrx_msm_update_rsb_coeff( fbrx_buffer_idx[0], rsb_a_coeff,rsb_b_coeff, rflm_tech_id, 0);          

        DALSYS_BusyWait(2500);	// give enough time to let iqmc register reset, clear iqmc done

        if(dbg_mode_en>=2)
		{
		    FTM_MSG_3(FTM_MED,
				   "rsb cal: type(0: rx rsb; 1: fbrx rsb) %d, gain_state: %d, time span <clear iqmc>: %d us",
				   rsb_type, cur_gain_state, rf_time_get_elapsed(start_tick,RF_USEC));    
            start_tick = rf_time_get_tick();
		}        
	            
        memset(iq_buf_ptr, 0,sample_size*4);
        api_status = ftm_enh_internal_device_cal_fbrx_rsb_iq_capture(   device_ptr,
                                                                        rsb_cal_alg,
                                                                        stg_offset_khz,
                                                                        (uint16) sample_size,
                                                                        dbg_mode_en,
                                                                        iq_buf_ptr,
                                                                        &dcCompVals,
                                                                        band_union,
                                                                        rfcom_bw,
                                                                        cur_gain_state,
                                                                        tx_device
                                                                    );
        
        if (api_status != FTM_ENH_INTERNAL_DEVICE_CAL_RESULTS_SUCCESS)
        {
            FTM_MSG_1(FTM_ERROR, "ftm_enh_internal_device_cal_fbrx_iq_capture failed for tech %d", (int)tech);
            internal_device_cal_data->internal_device_cal_status.error_log = api_status;
            if(iq_buf_ptr != NULL)
            {
                modem_mem_free((void*)iq_buf_ptr, MODEM_MEM_CLIENT_RFA);
            }
            return FALSE;
        }

        if(dbg_mode_en>=2)
		{
		    FTM_MSG_3(FTM_MED,
					   "rsb cal: type(0: rx rsb; 1: fbrx rsb) %d, tech: %d, time span <fbrx IQ capture>: %d us",
					   rsb_type, tech, rf_time_get_elapsed(start_tick,RF_USEC));	  
			start_tick = rf_time_get_tick();
			ftm_enh_internal_device_cal_save_iq( tech,band,rx_path[carrier], iq_buf_ptr, sample_size, 0);
			FTM_MSG_3(FTM_MED,
					   "rsb cal: type(0: rx rsb; 1: fbrx rsb) %d, tech: %d, time span <IQ save>: %d us",
					   rsb_type, tech, rf_time_get_elapsed(start_tick,RF_USEC));	  
			start_tick = rf_time_get_tick();			
	    }			

		
    	if(dbg_mode_en >= 4 )
     	{
	 	    for (id = 0;id < 5;id++)
	  		{
				FTM_MSG_4(FTM_MED,"iq_buf_ptr:0x%x, 0x%x, 0x%x, 0x%x", iq_buf_ptr[id*4],iq_buf_ptr[id*4+1],
				    iq_buf_ptr[id*4+2],iq_buf_ptr[id*4+3]);
	  		}			
     	}
	    iq_cap_struct_dc_comped.access_ptr = (uint32*)iq_buf_ptr;
        iq_cap_struct_dc_comped.sample_size = sample_size;
        iq_cap_struct_dc_comped.dcCompVals = dcCompVals;
        rfcommon_msm_compute_rsb_coeff_from_iq_dc_comped( 
                                                        iq_cap_struct_dc_comped,
                                                        (int16*) &rsb_a_coeff,
                                                        (int16*) &rsb_a_coeff,
                                                        &rsb_data.i_sq,
                                                        &rsb_data.q_sq,
                                                        &rsb_data.iq
                                                       );  
        rsb_data.gain_state = rsb_instr->gain_states[rsb_res_gain_index];
		if (tech == FTM_RF_TECH_LTE)
		{
#ifdef FEATURE_LTE    
            ret_val = rfdevice_lte_fbrx_calc_rsb_nv_params(tx_device, (rfcom_lte_band_type) band, bw,/*rsb_instr->gain_states[rsb_res_gain_index],*/ &rsb_data);

#else
            FTM_MSG(FTM_HIGH,"FEATURE_LTE is not enabled in this build");
#endif
		}            
	    else if(tech == FTM_RF_TECH_WCDMA)
	    {
#ifdef FEATURE_WCDMA	
		    ret_val = rfdevice_wcdma_get_fbrx_rsb_cal(tx_device, (rfcom_wcdma_band_type) band, /*RFDEV_WCDMA_BW_1X*/ bw, &rsb_data);
#else
            FTM_MSG(FTM_HIGH,"FEATURE_WCDMA is not enabled in this build");	  
#endif
	    }
	    else if (tech == FTM_RF_TECH_TDSCDMA)
	    {
#ifdef FEATURE_TDSCDMA
		    ret_val = rfdevice_tdscdma_fbrx_get_rsb_nv_params(tx_device, (rfcom_tdscdma_band_type) band, bw, /*rsb_instr->gain_states[rsb_res_gain_index], */&rsb_data);
#else
            FTM_MSG(FTM_HIGH,"FEATURE_TDSCDMA is not enabled in this build");	  
#endif
	    }
        FTM_MSG_3(FTM_MED, "before cal, rsb sin_theta: %d, cos_theta: %d, gain_inv: %d", rsb_data.sin_theta_f,rsb_data.cos_theta_f, rsb_data.gain_inv_f);
        
        if(dbg_mode_en >=2)
        {
		    FTM_MSG_3(FTM_MED,
					   "rsb cal: type(0: rx rsb; 1: fbrx rsb) %d, tech: %d, time span <fbrx rsb compute>: %d us",
					   rsb_type, tech, rf_time_get_elapsed(start_tick,RF_USEC));	  
			start_tick = rf_time_get_tick();	
        }

	    if( ret_val == TRUE )
	    {
	        if( ( rsb_data.cos_theta_f !=0) && (rsb_data.gain_inv_f !=0))
	        {
	            rsb_result->rsb_res[rx_path[carrier]][rsb_res_gain_index].nv_active = 1;
                rsb_result->rsb_res[rx_path[carrier]][rsb_res_gain_index].rsb_before_cal = (int8) floor(compute_rsb(rsb_data));
	        }
			else
			{
			    rsb_result->rsb_res[rx_path[carrier]][rsb_res_gain_index].nv_active = 0;
                rsb_result->rsb_res[rx_path[carrier]][rsb_res_gain_index].rsb_before_cal = 0;
			}
		    rsb_result->rsb_res[rx_path[carrier]][rsb_res_gain_index].sin_theta_final = rsb_data.sin_theta_f;
		    rsb_result->rsb_res[rx_path[carrier]][rsb_res_gain_index].cos_theta_final = rsb_data.cos_theta_f;
		    rsb_result->rsb_res[rx_path[carrier]][rsb_res_gain_index].gain_inv_final = rsb_data.gain_inv_f; // hardcode to only one gain state for now
		    rsb_result->device_mask |= (1<<rx_path[carrier]);	        

            if (dbg_mode_en >= 1)
		    {								
			    if(rsb_result->rsb_res[rx_path[carrier]][rsb_res_gain_index].nv_active == 1)
			    {
			        ftm_common_compute_rsb_coeffs_a_b_params(   rsb_data.sin_theta_f,
														  rsb_data.cos_theta_f,
														  rsb_data.gain_inv_f,
														  0,
														  16384,
														  16384,
														  0, /* rsb_cal_is_on - RSB cal not currently in process */
														  &rsb_a_coeff,
														  &rsb_b_coeff );

			        rfcommon_fbrx_msm_update_rsb_coeff( fbrx_buffer_idx[0], -rsb_a_coeff,-rsb_b_coeff, rflm_tech_id, ( rfcommon_fbrx_tx_cfg_type) bw);
			        DALSYS_BusyWait(2500);  /// give HW enough time to update iqmc register, move here to save cal-time
		            memset(iq_buf_ptr, 0,sample_size*4);
		            {
			            api_status = ftm_enh_internal_device_cal_fbrx_rsb_iq_capture(   device_ptr,
                                                                              rsb_cal_alg,
                                                                              stg_offset_khz,
                                                                              sample_size,
                                                                              dbg_mode_en,
                                                                              iq_buf_ptr,
                                                                              &dcCompVals,
                                                                              band_union,
                                                                              rfcom_bw,
                                                                              cur_gain_state,
                                                                              tx_device
                                                                          );

			            if (api_status != FTM_ENH_INTERNAL_DEVICE_CAL_RESULTS_SUCCESS)
			            {
        				    FTM_MSG_1(FTM_ERROR, "ftm_enh_internal_device_cal_fbrx_iq_capture failed for tech %d", (int)tech);
				            internal_device_cal_data->internal_device_cal_status.error_log = api_status;
				            if(iq_buf_ptr != NULL)
				            {
					            modem_mem_free((void*)iq_buf_ptr, MODEM_MEM_CLIENT_RFA);
				            }
				            return FALSE;
			            }
                        
                        if( dbg_mode_en >= 2)  // re-check if iq is good after dynamic iqmc register update
                        {
                            ftm_enh_internal_device_cal_save_iq( tech, band,rx_path[carrier], iq_buf_ptr, sample_size, 1);                                
                            FTM_MSG_3(FTM_MED,
                                   "rsb cal: type(0: rx rsb; 1: fbrx rsb) %d, tech: %d, time span <IQ save: after cal rsb>: %d us",
                                   rsb_type, tech, rf_time_get_elapsed(start_tick,RF_USEC));      
                            start_tick = rf_time_get_tick();
                        }           
		      	    }
				    iq_cap_struct_dc_comped.access_ptr = (uint32*)iq_buf_ptr;
                    iq_cap_struct_dc_comped.sample_size = sample_size;
                    iq_cap_struct_dc_comped.dcCompVals = dcCompVals;
                    rfcommon_msm_compute_rsb_coeff_from_iq_dc_comped( 
                                                                    iq_cap_struct_dc_comped,
                                                                    (int16*) &rsb_a_coeff,
                                                                    (int16*) &rsb_a_coeff,
                                                                    &rsb_data.i_sq,
                                                                    &rsb_data.q_sq,
                                                                    &rsb_data.iq
                                                                    );
 			  /* Read one therm value */
			        ret_val = ftm_compute_rsb_nv_params( 
  							tx_device,
  							tech,
  							band, 
  							rsb_data.i_sq,  rsb_data.q_sq,  rsb_data.iq,  therm_data, 
							&rsb_data.sin_theta_f,&rsb_data.cos_theta_f,&rsb_data.gain_inv_f,0);
			        FTM_MSG_3(FTM_MED, "after cal, rsb sin_theta: %d, cos_theta: %d, gain_inv: %d", rsb_data.sin_theta_f,rsb_data.cos_theta_f, rsb_data.gain_inv_f   );
			        if(ret_val == TRUE)
			        {
 			            rsb_result->rsb_res[rx_path[carrier]][rsb_res_gain_index].cos_theta_after_cal = rsb_data.cos_theta_f;
			            rsb_result->rsb_res[rx_path[carrier]][rsb_res_gain_index].sin_theta_after_cal = rsb_data.sin_theta_f;	
			            rsb_result->rsb_res[rx_path[carrier]][rsb_res_gain_index].gain_inv_after_cal = rsb_data.gain_inv_f;
			            rsb_result->rsb_res[rx_path[carrier]][rsb_res_gain_index].rsb_after_cal = (int8) floor(compute_rsb(rsb_data));  
			        }
			        else
			        {
			            rsb_result->rsb_res[rx_path[carrier]][rsb_res_gain_index].rsb_after_cal = 0; 	
			        }
			    } 
			    else
			    {
				    rsb_result->rsb_res[rx_path[carrier]][rsb_res_gain_index].rsb_after_cal = 0; 
			    } 
		  	}
	    }
	    else
	    {
		  rsb_result->rsb_res[rx_path[rsb_res_gain_index]][0].rsb_before_cal = 0; 
		  rsb_result->rsb_res[rx_path[rsb_res_gain_index]][0].rsb_after_cal = 0; 
		 //add error mesg here
	    }
	    FTM_MSG_6(FTM_MED,"rsb cal: tech:%d, band:%d, carrier:%d, rx_path:%d, rsb_before_cal:%d, rsb_after_cal:%d",
	  	tech,band,carrier,rx_path[rsb_res_gain_index],rsb_result->rsb_res[rx_path[0]][0].rsb_before_cal, 
    	rsb_result->rsb_res[rx_path[rsb_res_gain_index]][0].rsb_after_cal);	
        rfcommon_fbrx_mc_disable_rf_dev(device_ptr);
        FTM_MSG(FTM_HIGH,"Disabled FBRx setup ");
        if(rsb_result->device_mask != 0)
        {
            *res_len = sizeof(ftm_enh_internal_device_cal_rsb_result_type);
        
            *res_len -= sizeof(ftm_rsb_result_data_type)*MAX_NUM_RSB_GAIN_STATES*(FTM_ENH_INTERNAL_DEVICE_CAL_RSB_RES_MAX-(FTM_ENH_INTERNAL_DEVICE_CAL_RSB_RES_FBRX+1));
         }   
         else
           *res_len = 0;     
    }
    
    if(iq_buf_ptr !=NULL)
        modem_mem_free(iq_buf_ptr,MODEM_MEM_CLIENT_RFA);
    
    
    FTM_MSG_3(FTM_MED,
                      "rsb cal: type(0: rx rsb; 1: fbrx rsb) %d, tech: %d, complete in %d us",
                      rsb_type, tech, rf_time_get_elapsed(start_tick0,RF_USEC));   
    return ret_val;  
}	



#endif /* FEATURE_FACTORY_TESTMODE */

#endif /* FEATURE_GNSS_ONLY_NO_WWAN */

/*! @} */



