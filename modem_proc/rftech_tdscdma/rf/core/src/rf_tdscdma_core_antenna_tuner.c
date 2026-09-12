/*!
  @file
  rf_tdscdma_core_antenna_tuner.c

  @brief
  Provides TDSCDMA antenna tuner functionality

  @details

*/

/*===========================================================================

Copyright (c) 2012 - 2015 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$DateTime: 2021/03/09 05:09:33 $ $Author: pwbldsvc $
$Header: //components/rel/rftech_tdscdma.mpss/3.11/rf/core/src/rf_tdscdma_core_antenna_tuner.c#1 $

when         who    what, where, why
--------   ---     ----------------------------------------------------------
12/09/15   hx      Add protection for potential race condition when tuner Tx state is not set as expected. 
12/10/15   hm      Pass the frequency to tuner API's
08/26/15   hx      Porting logic from CR672625 by adding protection for concurrent tuner configuration
06/30/15   ndb     Added Tuner IRAT support in RxTx split feature
05/26/15   ndb     Updated to new tuner interface
04/23/15   jak     tuner API interface change and add new function for update tune code
02/28/15   jr      Reduce debug messages from sleep process
10/21/14   yb      Added priority configuration to config Tuners during CRAT scenarios
08/20/14   ndb     Add Tuner_ID as part of the response in "FTM_SET_TUNER_TUNE_CODE_OVERRIDE"
09/10/14   ych     Fix Tuner callflow issues
09/10/14   ych     Disabled rfnv_tdscdma_b40.c and made corresponding compilation/cw/kw changes
06/22/14   ych	   Add tuner_disable for both sleep/exit and Tx mode in IRAT
06/03/14   jz      Add tuner tune code override command
04/24/14   ych     Added dynamic tuner script for ifreq
04/09/14   jyu     Updated TxLM handle for rf_tdscdma_core_antenna_tuner_program_tx_mode()
03/26/14   kg      Added support for CL LM handle 
02/12/14   ms      Disable tuner in X2T iRAT case (porting per CR600367)
07/12/13   jz       Hook CL NV pointer in tuner set tx mode
06/27/13   ndb     Added rfcommon_nv_ant_tuner_cl_ctrl_type in tuner tx mode 
05/02/13   vb      Tuner CA changes 
04/30/13   vb      Fix for wrong argument when device script needs to run in 
                         IMMEDIATE mode
04/23/13   jyu     Changed the interface to program Tx mode
03/21/13   ms      Tuner TDET support
01/07/13   ndb    Added Tech specific Settings support in rx_init
11/19/12   jyu     Initial version (ported from WCDMA)

============================================================================*/

#ifdef FEATURE_RF_HAS_QTUNER

#include "rf_tdscdma_core_antenna_tuner.h"
#include "rfcommon_msg.h"
#include "rfc_card.h"
#include "rf_tdscdma_mc.h"
#include "rfc_card_tdscdma.h"
#include "rfcommon_atuner_intf.h"
#include "rfnv_tdscdma_b34.h"
#include "rfnv_tdscdma_b39.h"
#include "rfnv_tdscdma_b40.h"

#ifdef FEATURE_RF_ASDIV
#include "rfcommon_asdiv_manager.h"
#endif



boolean rf_tdscdma_tuner_rf_more_f3_flag = FALSE;
static boolean rf_tdscdma_QFE_TX_status = TRUE;


LOCAL rfcommon_atuner_device_type rf_tdscdma_core_antenna_tuner_device_type
(
  rfm_device_enum_type device,
  rfdevice_rx_mode_type rfdevice_op_mode    
);

/*---------------------------------------------------------------------------*/
/*!
  @brief
  Initial QFE tuner
  @details
  Initial QFE tuner
 
  @param device
  The device path to be enabled.

  @param band
   
  @return
  Flag indicating if the api operation was successful (TRUE) or failed (FALSE)
*/
boolean rf_tdscdma_core_antenna_tuner_init
(
  rfm_device_enum_type device,
  rfcom_tdscdma_band_type band,
  rf_device_execution_type dev_action,
  rf_buffer_intf *script_buffer,
  rfdevice_rx_mode_type rfdevice_op_mode  
)
{
  boolean init_status = TRUE;
  void **qtuner_mgr_obj = NULL;
  uint8 device_idx = 0;
  uint32 script_token =0;
  rfcommon_atuner_config_req_type atuner_config_req;

  /* populating atuner_config_req */
  atuner_config_req.rfm_device = device;
  atuner_config_req.mode = RFCOM_TDSCDMA_MODE;
  atuner_config_req.band.tdscdma_band = band;
  atuner_config_req.chan_num = 10055; // hard-coded chan number as this is init
  atuner_config_req.carrier_id = RFCMN_ATUNER_CARRIER_ID_0;
  atuner_config_req.rx_tx = rf_tdscdma_core_antenna_tuner_device_type(device , rfdevice_op_mode);

  /* Get Power tracker object from RFC */
  qtuner_mgr_obj = (void**)rfc_tdscdma_get_rf_device_obj( device, 
                                                      RFC_CONFIG_RX, 
                                                      band, 
                                                      RFDEVICE_TUNER_MANAGER );


  if (qtuner_mgr_obj != NULL)
  {

#ifdef FEATURE_RF_ASDIV
    /* Lock the ASDiv manager interface to avoid concurrency of ASDiv and Tuner configuration*/
    rfcommon_asdiv_manager_enter_lock();
#endif

    while (qtuner_mgr_obj[device_idx]!= NULL)
    {
      /* Initialize tuner */
      init_status &= rfcommon_atuner_init ( qtuner_mgr_obj[device_idx], 
                                            &atuner_config_req, 
                                            &script_token, 
                                            dev_action,
                                            script_buffer,
                                            0);

      device_idx++; 
      // need to check and break for MAX DEVICE IDX 
    }
#ifdef FEATURE_RF_ASDIV
    /* Release the ASDiv manager interface */
    rfcommon_asdiv_manager_release_lock(); 
#endif
  }
  else
  {
    init_status = FALSE;
    RF_MSG_2(RF_ERROR, "Qtuner_obj is NULL for Device %d and Band %d for Rx", device, band);
  }



  if(init_status && (rf_tdscdma_tuner_rf_more_f3_flag == TRUE))
  {   
    RF_MSG_1(RF_HIGH, "ANT_TUNER: Anntenna tuner init on device %d", device);
  }

  return init_status;
} /* rf_tdscdma_core_antenna_tuner_init */


/*---------------------------------------------------------------------------*/
/*!
  @brief
  Disable the QFE.

  @details
  Disable the QFE.

  @param device
  The device path to be disabled.

  @return
  Flag indicating if the api operation was successful (TRUE) or failed (FALSE)
*/
boolean rf_tdscdma_core_antenna_tuner_disable
(
  rfm_device_enum_type device,
  rfcom_tdscdma_band_type band,
  rf_device_execution_type dev_action,
  rf_buffer_intf *script_buffer,
  int16 script_timing,
  rfdevice_rx_mode_type rfdevice_op_mode  
)
{
  boolean init_status = TRUE;
  void **qtuner_mgr_obj = NULL;
  uint8 device_idx = 0;
  uint32 script_token =0;
  rfcommon_atuner_config_req_type atuner_config_req;  

  /* populating atuner_config_req */
  atuner_config_req.rfm_device = device;
  atuner_config_req.mode = RFCOM_TDSCDMA_MODE;
  atuner_config_req.band.tdscdma_band = band;
  atuner_config_req.chan_num = 10055; // hard-coded chan number as this is init
  atuner_config_req.carrier_id = RFCMN_ATUNER_CARRIER_ID_0;
  atuner_config_req.rx_tx = rf_tdscdma_core_antenna_tuner_device_type(device , rfdevice_op_mode);

  /* Get Qtuner object from RFC */
  qtuner_mgr_obj = (void**)rfc_tdscdma_get_rf_device_obj( device, 
                                                      RFC_CONFIG_RX, 
                                                      band, 
                                                      RFDEVICE_TUNER_MANAGER );
  if( qtuner_mgr_obj != NULL)
  {

#ifdef FEATURE_RF_ASDIV
    /* Lock the ASDiv manager interface to avoid concurrency of ASDiv and Tuner configuration*/
    rfcommon_asdiv_manager_enter_lock();
#endif

    while( qtuner_mgr_obj[device_idx]!= NULL)
    {
      /* Disable tuner */
      init_status &= rfcommon_atuner_disable ( qtuner_mgr_obj[device_idx], 
	                                           &atuner_config_req, 
	                                           &script_token, 
	                                           dev_action,
	                                           script_buffer,
	                                           script_timing);

      device_idx++;
    }

#ifdef FEATURE_RF_ASDIV
    /* Release the ASDiv manager interface */
    rfcommon_asdiv_manager_release_lock(); 
#endif

  }
  else
  {
    init_status = FALSE;
    RF_MSG_2(RF_ERROR, "Qtuner_obj is NULL for Device %d and Band %d for Rx", device, band);
  }

  return init_status;

} /* rf_tdscdma_core_antenna_tuner_disable */


/*----------------------------------------------------------------------------*/
/*!
  @brief
  Enable the QFE RX.

  @details
  Programs QFE tuner initialization sequence, and set up RX path.

  @param device
  The device path to be enabled.

  @param band

  @return
  Flag indicating if the api operation was successful (TRUE) or failed (FALSE)
*/
boolean rf_tdscdma_core_antenna_tuner_program_rx_mode 
( 
  rfm_device_enum_type rfm_device,
  rfcom_tdscdma_band_type band,
  uint16 chan_num,
  rf_device_execution_type rf_tdscdma_exec_type,
  rf_buffer_intf* buffer_ptr,
  int16 script_timing,
  rfdevice_rx_mode_type rfdevice_op_mode
)
{
  boolean success = TRUE;
  rfcom_band_type_u band_u;
  void **qtuner_mgr_obj = NULL;
  uint8 device_idx = 0;
  uint32 script_token =0;
  rfcom_antenna_tuner_priority_cfg tuner_priority_cfg;
  rfcommon_atuner_config_req_type atuner_config_req;  

  /* populating atuner_config_req */
  atuner_config_req.rfm_device = rfm_device;
  atuner_config_req.mode = RFCOM_TDSCDMA_MODE;
  atuner_config_req.band.tdscdma_band = band;
  atuner_config_req.chan_num = chan_num;
  atuner_config_req.carrier_id = RFCMN_ATUNER_CARRIER_ID_0;
  atuner_config_req.rx_tx = rf_tdscdma_core_antenna_tuner_device_type(rfm_device , rfdevice_op_mode);  
  atuner_config_req.freq = rf_tdscdma_core_util_get_freq_from_uarfcn(chan_num,band);

  /* Get Power tracker object from RFC */
  qtuner_mgr_obj = (void**)rfc_tdscdma_get_rf_device_obj( rfm_device, 
                                                      RFC_CONFIG_RX, 
                                                      band, 
                                                      RFDEVICE_TUNER_MANAGER );

  band_u.tdscdma_band = band;
  tuner_priority_cfg.rfm_device = rfm_device;

  if (qtuner_mgr_obj)
  {
#ifdef FEATURE_RF_ASDIV
    /* Lock the ASDiv manager interface to avoid concurrency of ASDiv and Tuner configuration*/
    rfcommon_asdiv_manager_enter_lock();
#endif

    while (qtuner_mgr_obj[device_idx]!= NULL)
    {
      /* Enable tuner manager's rx_mode */
      success &= rfcommon_atuner_set_rx_mode ( qtuner_mgr_obj[device_idx], 
                                               &atuner_config_req, 
                                               &script_token, 
                                               rf_tdscdma_exec_type,
                                               buffer_ptr,
                                               script_timing);

/* There is no way for RF to know when it gets executed by FW.
TFW just polls for common FW to check if update is available and executes it when appropriate time is available. 
Since an update occurs every 200 ms, FW should definitely find time to execute it before the next update*/

        if (success)
        {
          /*Ack if the script construction was successful */
          rfcommon_atuner_acknowledge_script ( qtuner_mgr_obj[device_idx], 
                                                       script_token );
        }
      device_idx++; 
       // need to check and break for MAX DEVICE IDX
    }

#ifdef FEATURE_RF_ASDIV
    /* Release the ASDiv manager interface */
    rfcommon_asdiv_manager_release_lock(); 
#endif

  }
  else
  {
    success = FALSE;
    RF_MSG_2(RF_ERROR, "Qtuner_obj is NULL for Device %d and Band %d for Rx", rfm_device, band);
  }

  if(success)
  {
  	rf_tdscdma_QFE_TX_status = FALSE;
  }

  if(success && (rf_tdscdma_tuner_rf_more_f3_flag == TRUE))
  {         
    RF_MSG_1(RF_HIGH, "ANT_TUNER: Programmed in Rx mode on device %d", rfm_device);   
  }

  return success;
} /* rf_tdscdma_core_antenna_tuner_program_rx_mode */

/*---------------------------------------------------------------------------*/
/*!
  @brief
  Enable the QFE TX.

  @details
  Programs QFE tuner to set up TX path.

  @param device
  The device path to be enabled.

  @param band
 
  @return
  Flag indicating if the api operation was successful (TRUE) or failed (FALSE)
*/
boolean rf_tdscdma_core_antenna_tuner_program_tx_mode 
(
  rfm_device_enum_type device,
  rfcom_tdscdma_band_type band, 
  uint16 chan_num,
  rf_device_execution_type rf_tdscdma_exec_type,
  rf_buffer_intf* buffer_ptr,
  int16 script_timing  
)
{
  uint32 tx_freq = 0;
  boolean success = TRUE;
  void **qtuner_mgr_obj = NULL;
  uint8 device_idx = 0;
  rfcom_band_type_u band_u;
  uint32 script_token =0;
  rfcommon_atuner_cl_ctrl_info_type cl_ctrl_info;
  rfcommon_atuner_config_req_type atuner_config_req;  

  /* populating atuner_config_req */
  atuner_config_req.rfm_device = device;
  atuner_config_req.mode = RFCOM_TDSCDMA_MODE;
  atuner_config_req.band.tdscdma_band = band;
  atuner_config_req.chan_num = chan_num;
  atuner_config_req.carrier_id = RFCMN_ATUNER_CARRIER_ID_0;
  atuner_config_req.rx_tx = rf_tdscdma_core_antenna_tuner_device_type(device, RFDEVICE_PRX_OPER_MODE);   

  // it is expecting the tx handle  - note there is issue for BHO, as this api is not called for BHO
  //cl_ctrl_info.lm_handle = rf_tdscdma_mc_lm_bufs.rx_buf_wrk[RF_PATH_0].buf_idx; 
  cl_ctrl_info.lm_handle = rf_tdscdma_mc_lm_bufs.tx_buf_wrk.buf_idx;

  /* Get Power tracker object from RFC */
  qtuner_mgr_obj = (void**)rfc_tdscdma_get_rf_device_obj(device, 
                                                         RFC_CONFIG_TX, 
                                                         band, 
                                                         RFDEVICE_TUNER_MANAGER );

  tx_freq = rf_tdscdma_core_util_get_freq_from_chan(chan_num);
  band_u.tdscdma_band=band;
     
  /*Need to convert the Tx freq in KHz to Hz */
  tx_freq = tx_freq * 1000;
     
  switch (band)
  {
    case RFCOM_BAND_TDSCDMA_B34:
      cl_ctrl_info.cl_nv_ctrl_info = &rfnv_tdscdma_b34_tx_tbl.ant_tuner_cl;
      break;

    case RFCOM_BAND_TDSCDMA_B39:
      cl_ctrl_info.cl_nv_ctrl_info = &rfnv_tdscdma_b39_tx_tbl.ant_tuner_cl;
      break;

    case RFCOM_BAND_TDSCDMA_B40:
      //cl_ctrl_info.cl_nv_ctrl_info = &rfnv_tdscdma_b40_tx_tbl.ant_tuner_cl;
      break;

    default:
      cl_ctrl_info.cl_nv_ctrl_info = NULL;
      RF_MSG_1(RF_ERROR, "In rf_tdscdma_core_antenna_tuner_program_tx_mode(), Invalid Band %d, NV pointer is NULL", band);
      break;
  }
	 
  if (qtuner_mgr_obj)
  {

#ifdef FEATURE_RF_ASDIV
    /* Lock the ASDiv manager interface to avoid concurrency of ASDiv and Tuner configuration*/
    rfcommon_asdiv_manager_enter_lock();
#endif

    while (qtuner_mgr_obj[device_idx]!= NULL)
    {


      // set tuner tx mode */
      success &= rfcommon_atuner_set_tx_mode ( qtuner_mgr_obj[device_idx], 
                                               &atuner_config_req,
                                               &cl_ctrl_info,
                                               &script_token, 
                                               rf_tdscdma_exec_type,
                                               buffer_ptr,
                                               script_timing);	  

      /* There is no way for RF to know when it gets executed by FW.
         TFW just polls for common FW to check if update is available and executes it when appropriate time is available. 
         Since an update occurs every 200 ms, FW should definitely find time to execute it before the next update*/

      if (success)
      {
        /*Ack if the script construction was successful */
        rfcommon_atuner_acknowledge_script ( qtuner_mgr_obj[device_idx], 
                                                     script_token );
      }

      device_idx++;
    }
#ifdef FEATURE_RF_ASDIV
    /* Release the ASDiv manager interface */
    rfcommon_asdiv_manager_release_lock(); 
#endif

  }
  else
  {
    success = FALSE;
    RF_MSG_2(RF_ERROR, "Qtuner_obj is NULL for Device %d and Band %d for Tx", device, band);
  }

  if(success)
  {   
    rf_tdscdma_QFE_TX_status = TRUE;
    RF_MSG_2(RF_HIGH, "ANT_TUNER: Programmed in Tx mode on device %d, handle %d", device, cl_ctrl_info.lm_handle); 
  }
  else
  {
    RF_MSG_2(RF_HIGH, "ANT_TUNER: Programmed FAILED in Tx mode on device %d, handle %d", device, cl_ctrl_info.lm_handle); 
  }

  return success;
} /* rf_tdscdma_core_antenna_tuner_program_tx_mode */


/*---------------------------------------------------------------------------*/
/*!
  @brief
  Disable antenna tuner in tx mode

  @details
  Disable antenna tuner in tx mode
  
  @param device
  The device path to be disabled.  

  @return
  Flag indicating if the api operation was successful (TRUE) or failed (FALSE)
*/
boolean rf_tdscdma_core_antenna_tuner_tx_mode_disable 
(
  rfm_device_enum_type device,
  rfcom_tdscdma_band_type band, 
  uint16 chan_num,
  rf_device_execution_type rf_tdscdma_exec_type,
  rf_buffer_intf* buffer_ptr,
  int16 script_timing
)
{
	uint32 tx_freq = 0;
  boolean success = TRUE;
  void **qtuner_mgr_obj = NULL;
  uint8 device_idx = 0;
  rfcom_band_type_u band_u;
  uint32 script_token =0;
  rfcommon_atuner_config_req_type atuner_config_req;  

  /* populating atuner_config_req */
  atuner_config_req.rfm_device = device;
  atuner_config_req.mode = RFCOM_TDSCDMA_MODE;
  atuner_config_req.band.tdscdma_band = band;
  atuner_config_req.chan_num = chan_num;
  atuner_config_req.carrier_id = RFCMN_ATUNER_CARRIER_ID_0;
  atuner_config_req.rx_tx = rf_tdscdma_core_antenna_tuner_device_type(device, RFDEVICE_PRX_OPER_MODE);  

  /* Get Power tracker object from RFC */
  qtuner_mgr_obj = (void**)rfc_tdscdma_get_rf_device_obj( device, 
                                                      RFC_CONFIG_TX, 
                                                      rf_tdscdma_mc_state.curr_band_tx, 
                                                      RFDEVICE_TUNER_MANAGER );
                                                      
  tx_freq = rf_tdscdma_core_util_get_freq_from_chan(chan_num);
  band_u.tdscdma_band=band;                                                      

  /*Need to convert the Tx freq in KHz to Hz */
  tx_freq = tx_freq * 1000;

  if (qtuner_mgr_obj)
  {
        /*Remove the check for SNUM exceeds crash due to potential race condition*/
  	//if (rf_tdscdma_QFE_TX_status)
  	//{	
#ifdef FEATURE_RF_ASDIV
    /* Lock the ASDiv manager interface to avoid concurrency of ASDiv and Tuner configuration*/
    rfcommon_asdiv_manager_enter_lock();
#endif

      while (qtuner_mgr_obj[device_idx]!= NULL)
      {
        /* set tuner tx mode */
		success &= rfcommon_atuner_tx_disable ( qtuner_mgr_obj[device_idx], 
												&atuner_config_req, 
												&script_token, 
												rf_tdscdma_exec_type,
												buffer_ptr,
												script_timing);

        device_idx++;
      }
#ifdef FEATURE_RF_ASDIV
    /* Release the ASDiv manager interface */
    rfcommon_asdiv_manager_release_lock(); 
#endif
    //} //if (rf_tdscdma_QFE_TX_status)
  }
  else
  {
    success = FALSE;
    RF_MSG_2(RF_ERROR, "Qtuner_obj is NULL for Device %d and Band %d for Tx", device, rf_tdscdma_mc_state.curr_band_tx);
  }

  if(success)
  {      
    RF_MSG_1(RF_HIGH,"ANT_TUNER: Tx mode disabled on device %d", device);    
  }

  return success;
} /* rf_tdscdma_core_antenna_tuner_tx_mode_disable */


/*----------------------------------------------------------------------------*/
/*!
  @brief
    rfwcdma_core_antenna_tuner_tune_code_override.
    This function is used to override the antenna tuner settings

  @param 

  @details
*/
uint32 rf_tdscdma_core_antenna_tuner_tune_code_override( rfm_device_enum_type device,
                                                         rfcom_tdscdma_band_type band,
                                                         uint8 override_flag,
                                                         void *data,
                                                         uint8* tuner_nv_ptr,
                                                         uint8* tuner_id_ptr)
{
  uint32 status = RFCMN_ATUNER_ERROR;
  uint8 device_idx = 0;
  void **qtuner_obj = NULL;
  rfcommon_atuner_intf* tuner_mgr = NULL;
  rfcommon_atuner_config_req_type atuner_config_req;  

  /* populating atuner_config_req */
  atuner_config_req.rfm_device = device;
  atuner_config_req.mode = RFCOM_TDSCDMA_MODE;
  atuner_config_req.band.tdscdma_band = band;
  atuner_config_req.chan_num = 10055; // hard-coded chan number as this is init
  atuner_config_req.carrier_id = RFCMN_ATUNER_CARRIER_ID_0;
  atuner_config_req.rx_tx = rf_tdscdma_core_antenna_tuner_device_type(device, RFDEVICE_PRX_OPER_MODE);  

  /* Get Tuner device object from RFC */
  qtuner_obj = (void**)rfc_tdscdma_get_rf_device_obj( device, 
                                                      RFC_CONFIG_RX, 
                                                      band, 
                                                      RFDEVICE_TUNER_MANAGER );
  
  if(qtuner_obj == NULL)
  {
    RF_MSG_1(RF_MED,"ANT_TUNER: No Anntenna tuner manager found from RFC for path %d", device);
    return(RFDEVICE_ANTENNA_TUNER_OVERRIDE_TUNER_NOT_PRESENT);
  }

  /*We don't touch rfcommon_asdiv_manager lock in FTM mode unless necessary*/

  while (qtuner_obj[device_idx]!= NULL)
  {
    tuner_mgr = (rfcommon_atuner_intf*)qtuner_obj[device_idx];

    /* Override the tuner on the path of interest */
    status = rfcommon_atuner_tunecode_override( tuner_mgr,
                                                override_flag,
                                                data,
                                                tuner_nv_ptr,
                                                tuner_id_ptr);

    device_idx++;
  }

  RF_MSG_3(RF_HIGH,"ANT_TUNER: Antennat tuners overriden for the device path# %d, status: %d, tuner_num: %d", device, status, device_idx);

  return status;

} /*rf_tdscdma_core_antenna_tuner_tune_code_override*/

/*---------------------------------------------------------------------------*/
/*!
  @brief
  determine the device type based on the devicd ID ( prx , drx , tx )
  
  @details
  determine the device type based on the devicd ID ( prx , drx , tx )
 
  @param device
  device
  rfdevice_op_mode
   
  @return
  prx / drx / tx ( in rfcommon_atuner_device_type )
*/
rfcommon_atuner_device_type rf_tdscdma_core_antenna_tuner_device_type
(
  rfm_device_enum_type device,
  rfdevice_rx_mode_type rfdevice_op_mode    
)
{
  rfm_device_capability_type device_capability;
  rfcommon_atuner_device_type device_type = RFCMN_ATUNER_INVALID_DEVICE;

  device_capability = rfc_common_get_device_capability( device );
  if ( device_capability == RFM_DEVICE_TX_SUPPORTED )
  {
    device_type = RFCMN_ATUNER_TX_DEVICE;
  }
  else if ( device_capability == RFM_DEVICE_RX_SUPPORTED )
  {
    /* if the device is Rx capability , then we decide it is PRX or DRX based on device0/2 or 1/3 */
    if ( rfdevice_op_mode == RFDEVICE_PRX_OPER_MODE )
    {
      device_type = RFCMN_ATUNER_PRX_DEVICE;
    }

    else if ( rfdevice_op_mode == RFDEVICE_DRX_OPER_MODE )
    {
      device_type = RFCMN_ATUNER_DRX_DEVICE;
    }
  }
  else 
  {
    RF_MSG_2(RF_HIGH,"ANT_TUNER: invalid rfm device capability[%d] for device %d ",device_capability, device);    
  }

  //RF_MSG_2(RF_HIGH,"ANT_TUNER: rfm device capability[%d] for device %d ",device_capability, device);

  return device_type;
  
}

/*---------------------------------------------------------------------------*/
/*!
  @brief
  update tune code 

  @details
  update tune code
  
  @param device

  @return
  Flag indicating if the api operation was successful (TRUE) or failed (FALSE)
*/
boolean rf_tdscdma_core_antenna_tuner_update_tune_code
(
  rfm_device_enum_type device,
  rfcom_tdscdma_band_type band, 
  uint16 chan_num,
  rf_device_execution_type rf_tdscdma_exec_type,
  rf_buffer_intf* buffer_ptr,
  int16 script_timing, 
  uint32* script_token_ptr,
  rfdevice_rx_mode_type rfdevice_op_mode  
)
{
  boolean success = TRUE;
  void **qtuner_mgr_obj = NULL;
  uint8 device_idx = 0;
  rfcommon_atuner_config_req_type atuner_config_req;  
  rfcommon_atuner_device_type device_type;
  rfc_rxtx_enum_type rfc_rxtx = RFC_CONFIG_RXTX_INVALID;
  
  /* query for device_type of the device */
  device_type = rf_tdscdma_core_antenna_tuner_device_type(device , rfdevice_op_mode);

  /* then we need to decide this is RFC rx or tx for query RFC for tuner obj */
  if ( device_type == RFCMN_ATUNER_PRX_DEVICE || device_type == RFCMN_ATUNER_DRX_DEVICE )
  {
    rfc_rxtx = RFC_CONFIG_RX;
  }
  else if ( device_type == RFCMN_ATUNER_TX_DEVICE )
  {
    rfc_rxtx = RFC_CONFIG_TX;
  }
  else 
  {
    RF_MSG_2(RF_ERROR,"ANT_TUNER: invalid device type %d for tune code update on device %d", device_type, device);    
  }

  /* populating atuner_config_req */
  atuner_config_req.rfm_device = device;
  atuner_config_req.mode = RFCOM_TDSCDMA_MODE;
  atuner_config_req.band.tdscdma_band = band;
  atuner_config_req.chan_num = chan_num;
  atuner_config_req.carrier_id = RFCMN_ATUNER_CARRIER_ID_0;
  atuner_config_req.rx_tx = device_type;

  /* Get Power tracker object from RFC */
  qtuner_mgr_obj = (void**)rfc_tdscdma_get_rf_device_obj( device, 
                                                      rfc_rxtx, 
                                                      band,
                                                      RFDEVICE_TUNER_MANAGER );

  if (qtuner_mgr_obj)
  {

#ifdef FEATURE_RF_ASDIV
    /* Lock the ASDiv manager interface to avoid concurrency of ASDiv and Tuner configuration*/
    rfcommon_asdiv_manager_enter_lock();
#endif

    while (qtuner_mgr_obj[device_idx]!= NULL)
    {
      /* set tuner tx mode */
	    success &= rfcommon_atuner_update_tune_code ( qtuner_mgr_obj[device_idx], 
												  &atuner_config_req, 
												  (script_token_type*)script_token_ptr, 
												  rf_tdscdma_exec_type,
												  buffer_ptr,
												  script_timing);

    device_idx++;
    }
#ifdef FEATURE_RF_ASDIV
    /* Release the ASDiv manager interface */
    rfcommon_asdiv_manager_release_lock(); 
#endif
  }
  else
  {
    success = FALSE;
    RF_MSG_2(RF_ERROR, "Qtuner_obj is NULL for Device %d and Band %d for Tx", device, band);
  }

  if(success && (rf_tdscdma_tuner_rf_more_f3_flag == TRUE))
  {         
    RF_MSG_1(RF_HIGH,"ANT_TUNER: tune code update on device %d", device);    
  }  

  return success;
} /* rf_tdscdma_core_antenna_tuner_update_tune_code */


/*----------------------------------------------------------------------------*/
/*!
  @brief


  @details
  Programs the tuners in tune-away

  @return
  Flag indicating if the api operation was successful (TRUE) or failed (FALSE)
*/
boolean rf_tdscdma_core_antenna_tuner_tune_away
( 
  rfm_device_enum_type rfm_device,
  rfcom_tdscdma_band_type band,
  uint16 chan_num,
  rf_device_execution_type rf_tdscdma_exec_type,
  rf_buffer_intf* buffer_ptr,
  int16 script_timing,
  rfdevice_rx_mode_type rfdevice_op_mode
)
{
  boolean success = TRUE;
  void **qtuner_mgr_obj = NULL;
  uint8 device_idx = 0;
  uint32 script_token =0;
  rfcommon_atuner_config_req_type atuner_config_req;  

  /* populating atuner_config_req */
  atuner_config_req.rfm_device = rfm_device;
  atuner_config_req.mode = RFCOM_TDSCDMA_MODE;
  atuner_config_req.band.tdscdma_band = band;
  atuner_config_req.chan_num = chan_num;
  atuner_config_req.carrier_id = RFCMN_ATUNER_CARRIER_ID_0;
  atuner_config_req.rx_tx = rf_tdscdma_core_antenna_tuner_device_type(rfm_device , rfdevice_op_mode);  

  /* Get Power tracker object from RFC */
  qtuner_mgr_obj = (void**)rfc_tdscdma_get_rf_device_obj( rfm_device, 
                                                          RFC_CONFIG_RX, 
                                                          band, 
                                                          RFDEVICE_TUNER_MANAGER );
  if (qtuner_mgr_obj)
  {

#ifdef FEATURE_RF_ASDIV
    /* Lock the ASDiv manager interface to avoid concurrency of ASDiv and Tuner configuration*/
    rfcommon_asdiv_manager_enter_lock();
#endif

    while (qtuner_mgr_obj[device_idx]!= NULL)
    {
      success &= rfcommon_atuner_tune_away ( qtuner_mgr_obj[device_idx], 
                                             &atuner_config_req, 
                                             &script_token, 
                                             rf_tdscdma_exec_type,
                                             buffer_ptr,
                                             script_timing);

      if (success)
      {
        /*Ack if the script construction was successful */
        rfcommon_atuner_acknowledge_script ( qtuner_mgr_obj[device_idx], 
                                             script_token );
      }
      device_idx++; 
    }
#ifdef FEATURE_RF_ASDIV
    /* Release the ASDiv manager interface */
    rfcommon_asdiv_manager_release_lock(); 
#endif

  }
  else
  {
    success = FALSE;
    RF_MSG_2(RF_ERROR, "Qtuner_obj is NULL for Device %d and Band %d for Rx", rfm_device, band);
  }

  if(success && (rf_tdscdma_tuner_rf_more_f3_flag == TRUE))
  {         
    RF_MSG_1(RF_HIGH, "ANT_TUNER: Programmed in Tune-away mode on device %d", rfm_device);   
  }

  return success;
} /* rf_tdscdma_core_antenna_tuner_tune_away */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Program the tuners in tuneback mode

  @details
  
  @return
  Flag indicating if the api operation was successful (TRUE) or failed (FALSE)
*/
boolean rf_tdscdma_core_antenna_tuner_tune_back
( 
  rfm_device_enum_type rfm_device,
  rfcom_tdscdma_band_type band,
  uint16 chan_num,
  rf_device_execution_type rf_tdscdma_exec_type,
  rfc_rxtx_enum_type rx_tx_config,
  rf_buffer_intf* buffer_ptr,
  int16 script_timing,
  rfdevice_rx_mode_type rfdevice_op_mode
)
{
  boolean success = TRUE;
  void **qtuner_mgr_obj = NULL;
  uint8 device_idx = 0;
  uint32 script_token =0;
  rfcommon_atuner_config_req_type atuner_config_req;  

  /* populating atuner_config_req */
  atuner_config_req.rfm_device = rfm_device;
  atuner_config_req.mode = RFCOM_TDSCDMA_MODE;
  atuner_config_req.band.tdscdma_band = band;
  atuner_config_req.chan_num = chan_num;
  atuner_config_req.carrier_id = RFCMN_ATUNER_CARRIER_ID_0;
  atuner_config_req.rx_tx = rf_tdscdma_core_antenna_tuner_device_type(rfm_device , rfdevice_op_mode);  

  /* Get Power tracker object from RFC */
  qtuner_mgr_obj = (void**)rfc_tdscdma_get_rf_device_obj( rfm_device, 
                                                          rx_tx_config, 
                                                          band, 
                                                          RFDEVICE_TUNER_MANAGER );
  if (qtuner_mgr_obj)
  {
#ifdef FEATURE_RF_ASDIV
    /* Lock the ASDiv manager interface to avoid concurrency of ASDiv and Tuner configuration*/
    rfcommon_asdiv_manager_enter_lock();
#endif

    while (qtuner_mgr_obj[device_idx]!= NULL)
    {
      success &= rfcommon_atuner_tune_back ( qtuner_mgr_obj[device_idx], 
                                             &atuner_config_req, 
                                             &script_token, 
                                             rf_tdscdma_exec_type,
                                             buffer_ptr,
                                             script_timing);
      if (success)
      {
        /*Ack if the script construction was successful */
        rfcommon_atuner_acknowledge_script ( qtuner_mgr_obj[device_idx], 
                                             script_token );
      }
      device_idx++; 
    }
#ifdef FEATURE_RF_ASDIV
    /* Release the ASDiv manager interface */
    rfcommon_asdiv_manager_release_lock(); 
#endif

  }
  else
  {
    success = FALSE;
    RF_MSG_2(RF_ERROR, "Qtuner_obj is NULL for Device %d and Band %d for Rx", rfm_device, band);
  }

  if(success && (rf_tdscdma_tuner_rf_more_f3_flag == TRUE))
  {         
    RF_MSG_1(RF_HIGH, "ANT_TUNER: Programmed in Tuneback on device %d", rfm_device);   
  }

  return success;
} /* rf_tdscdma_core_antenna_tuner_tune_back */
#endif /* FEATURE_RF_HAS_QTUNER */
