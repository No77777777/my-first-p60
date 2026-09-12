/*!
  @file rf_device_factory.cpp 

  @brief
  This is the rf device factory which creates all devices.



*/

/*===========================================================================

  Copyright (c) 2009 - 2024 QUALCOMM Technologies Inc. All Rights Reserved

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

===========================================================================*/


/*===========================================================================


                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rfc_habanero.mpss/1.11/target/divar/src/rf_device_factory.cpp#3 $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
03/04/24   spra    Add support for QAT5655 and QAT5616
12/23/19   akm     GPS only featurization
10/22/19   dh      Added support for QSW2820F, QSW8820B, QSW8574, QAT3555, QAT5515 devices
05/27/19   aj      Initial Revision


===========================================================================*/

/*===========================================================================

                            INCLUDE FILES

===========================================================================*/
#include "comdef.h"
#include "rfcommon_atuner_manager_factory.h"
#include "rf_device_factory.h"
#include "rfdevice_elna.h"
#include "rfdevice_test_device.h"
#include "rfdevice_rxtx_common_adapter.h"
//#ifndef FEATURE_GNSS_ONLY_NO_WWAN
#include "rfdevice_pa_common.h"
#include "rfdevice_papm_common.h"
#include "rfdevice_asm_common.h" 
#include "rfdevice_asm_common_xsw.h" 
#include "rfdevice_coupler_common.h" 
#include "qfe3320_physical_device_ag.h"
#include "rfdevice_papm_physical_device.h"
//#endif
#include "rfc_class.h"
#include "DALSys.h" /* DALSYS_BusyWait() */


extern "C"
{
#include "msg.h"
#include "rfcommon_nv_mm.h"

#if defined(FEATURE_CDMA1X) || defined(FEATURE_HDR)
#if (!( defined(FEATURE_JOLOKIA_MODEM) || defined(FEATURE_TABASCO_MODEM)))
#include "wtr2605_cdma_device_intf.h"
#endif
#include "rfdevice_trx_cdma_rx_adapter.h"
#include "rfdevice_trx_cdma_tx_adapter.h"
#endif

#ifdef FEATURE_GSM 
#if (!( defined(FEATURE_JOLOKIA_MODEM) || defined(FEATURE_TABASCO_MODEM)))
#include "wtr2605_gsm_device_intf.h"
#endif

#ifdef FEATURE_RF_HAS_WTR3925
#include "wtr3925_gsm_device_intf.h"
#endif //FEATURE_RF_HAS_WTR3925

#include "rfdevice_trx_gsm_rx_adapter.h"
#include "rfdevice_trx_gsm_tx_adapter.h"
#endif

#ifdef FEATURE_WCDMA
#if (!( defined(FEATURE_JOLOKIA_MODEM) || defined(FEATURE_TABASCO_MODEM)))
#include "wtr2605_wcdma_device_intf.h"
#endif

#ifdef FEATURE_RF_HAS_WTR3925
#include "wtr3925_wcdma_device_intf.h"
#include "wtr3925_common_device_intf.h"
#include "wtr3925_physical_device.h"
#endif  //FEATURE_RF_HAS_WTR3925

#include "rfdevice_trx_wcdma_rx_adapter.h"
#include "rfdevice_trx_wcdma_tx_adapter.h"
#endif

#ifdef FEATURE_TDSCDMA
#if (!( defined(FEATURE_JOLOKIA_MODEM) || defined(FEATURE_TABASCO_MODEM)))
#include "wtr2605_tdscdma_device_intf.h"
#endif


#ifdef FEATURE_RF_HAS_WTR3925
#include "wtr3925_tdscdma_device_intf.h"
#endif /* #ifdef FEATURE_RF_HAS_WTR3925 */


#include "rfdevice_trx_tdscdma_rx_adapter.h"
#include "rfdevice_trx_tdscdma_tx_adapter.h"
#endif

#ifdef FEATURE_CGPS
#if (!( defined(FEATURE_JOLOKIA_MODEM) || defined(FEATURE_TABASCO_MODEM)))
#include "wtr2605_gnss_device_intf.h"
#endif
#endif

#ifdef FEATURE_CDMA
#if defined(FEATURE_CDMA1X) || defined(FEATURE_HDR)
#include "rfdevice_trx_cdma_rx_adapter.h"
#include "rfdevice_trx_cdma_tx_adapter.h"

#ifdef FEATURE_RF_HAS_WTR3925
#include "wtr3925_cdma_device_intf.h"
#endif //FEATURE_RF_HAS_WTR3925

#ifdef FEATURE_RF_HAS_WTR2965
#include "wtr2965_cdma_device_intf.h"
#endif

#endif
#endif 

#ifdef FEATURE_GSM 
#ifdef FEATURE_RF_HAS_WTR2965
#include "wtr2965_gsm_device_intf.h"
#endif

#include "rfdevice_trx_gsm_rx_adapter.h"
#include "rfdevice_trx_gsm_tx_adapter.h"
#endif 

#ifdef FEATURE_WCDMA
#include "rfdevice_trx_wcdma_rx_adapter.h"
#include "rfdevice_trx_wcdma_tx_adapter.h"

#ifdef FEATURE_RF_HAS_WTR2965
#include "wtr2965_wcdma_device_intf.h"
#endif

#endif

#ifdef FEATURE_LTE
#include "rfdevice_trx_lte_rx_adapter.h"
#include "rfdevice_trx_lte_tx_adapter.h"


#ifdef FEATURE_RF_HAS_WTR2965
#include "wtr2965_lte_device_intf.h"
#endif

#ifdef FEATURE_RF_HAS_WTR3925
#include "wtr3925_lte_device_intf.h"
#endif /* #ifdef FEATURE_RF_HAS_WTR3925 */

#endif /* #ifdef FEATURE_LTE */

#ifdef FEATURE_TDSCDMA
#include "rfdevice_trx_tdscdma_rx_adapter.h"
#include "rfdevice_trx_tdscdma_tx_adapter.h"

#ifdef FEATURE_RF_HAS_WTR2965
#include "wtr2965_tdscdma_device_intf.h"
#endif

#endif 
}
#ifdef FEATURE_CGPS
#ifdef FEATURE_RF_HAS_WTR2965
#include "wtr2965_gnss_device_intf.h"
#endif

#endif 

#ifdef FEATURE_RF_HAS_QFE2340
#include "qfe2340_pa.h"
#include "qfe2340_asm.h"
#include "qfe2340_typedef_ag.h"
#include "qfe2340_physical_device_ag.h"
#endif

#include "qfe2320_pa.h"
#include "qfe2320_asm.h"
#include "qfe2320_typedef_ag.h"
#include "qfe2320_physical_device_ag.h"

#if (!( defined(FEATURE_JOLOKIA_MODEM) || defined(FEATURE_TABASCO_MODEM)))
#include "qfe2320ept_pa.h"
#include "qfe2320ept_asm.h"
#include "qfe2320ept_typedef_ag.h"
#endif 

#include "qpa4351_physical_device.h"

#include "rfdevice_rxtx_common_class.h"
#include "rfc_cmn_transceiver_hdet_adapter.h"


#include "qfe2550_physical_device.h"
#include "qat3550_physical_device.h"
#include "qat3514_physical_device.h"
#include "qat3516_physical_device.h"
#include "qat3522_physical_device.h"
#include "qat3518_physical_device.h"
#include "qat3555_physical_device.h"
#include "qat5515_physical_device.h"
#include "qat5616_physical_device.h"
#include "qat5655_physical_device.h"

#ifdef FEATURE_RF_HAS_WTR2965
#include "wtr2965_common_device_intf.h"
#include "wtr2965_physical_device.h"
#endif

#ifdef FEATURE_RF_HAS_WTR3925
#include "wtr3925_gnss_device_intf.h"
#endif //FEATURE_RF_HAS_WTR3925

#include "qfe1040_physical_device_ag.h"
#include "qfe3320_physical_device_ag.h"
#include "qfe2082fc_physical_device_ag.h"
#include "qfe4320_physical_device_ag.h"
#include "qfe3340fc_physical_device.h"
#include "qfe4373fc_physical_device_ag.h"
#include "qfe2085fc_physical_device_ag.h"
#include "qpa6560_physical_device.h"
#include "qpa6560_hb_physical_device.h"
#include "qpa8673_physical_device.h"
#include "qpa8675_physical_device.h"
#include "qpa8685_physical_device.h"
#include "qpa8686_physical_device.h"
#include "qpa8687_physical_device.h"
#include "qpa8688_physical_device.h"
#include "qsw2820d_physical_device.h"
#include "qsw2630a_physical_device.h"
#include "qsw8820b_physical_device.h"
#include "qsw2820f_physical_device.h"
#include "qsw8574_physical_device.h"

#ifdef FEATURE_RF_HAS_QPA4340
#include "qpa4340_physical_device.h"
#endif

#include "qpa4360_physical_device.h"
#include "qpa4361_physical_device.h"


#if (!( defined(FEATURE_JOLOKIA_MODEM) || defined(FEATURE_TABASCO_MODEM)))
#include "wtr2605_rxtx_common_class.h"
#endif

#ifdef FEATURE_GNSS_ONLY_NO_WWAN
#ifdef FEATURE_RF_ASDIV
#include "rfcommon_asdiv_tuner_manager.h"
#endif
#else
#ifdef FEATURE_RF_HAS_QTUNER
#ifdef FEATURE_RF_ASDIV
#include "rfcommon_asdiv_tuner_manager.h"
#endif

#ifdef FEATURE_RF_HAS_QFE1550
#include "qfe1550_physical_device.h"
#endif
#endif

extern "C"
{
  #include "rfdevice_antenna_tuner_intf.h"
  #include "rfdevice_hdet_cmn_intf.h"
}
#endif /*FEATURE_RF_HAS_QTUNER */


/*----------------------------------------------------------------------------*/
/*!
  @brief
  Device factory handler for creating WTR2605 instance

  @details
  This is an internal device factory handler for creating WTR1605. The common
  and tech specific instances are created. Common instances will have references
  to tech specific instances. All tech specific instance creation and header
  file includes need to be featurized.
 
  Please note that the device specific factory handlers should be static and
  not be exposed to other modules for direct call.
  
  @param cfg
  Device configuration settings of the device which needs to be created.
  Typically, this configuration should be autogenerated in RF card specific
  autogen file.
 
  @return rfdevice_class*
  Returns an instance of rfdevice_class (super class of the devices)  
*/
#if (!( defined(FEATURE_JOLOKIA_MODEM) || defined(FEATURE_TABASCO_MODEM)))

static rfdevice_class* 
rf_device_factory_create_wtr2605_device
( 
  rfc_device_cfg_info_type* cfg 
)
{
  rfdevice_class* instance = NULL;

  /* Get Common NV table Pointer */
  rfcommon_nv_tbl_type *rfnv_common_nv_tbl_ptr = rfcommon_nv_get_tbl_ptr();

  rfdevice_cmn_int_dev_cal_data_type cal_data; /* Cal Data */
  rfdevice_bus_info_type wtr2605_bus_info;
  uint32 rc_process_index = 0;

  rc_process_index = (cfg->rf_asic_id) * 8;
  /* Populate Calibration Data with pointer to NV */
  if(rc_process_index < RFCOMMON_MAX_RC_TUNER_CAL_ITEMS)
  {
    cal_data.rc_tuner.rc_process_errors = 
        &rfnv_common_nv_tbl_ptr->rc_process_error[rc_process_index];
  }
  cal_data.rc_tuner.number_of_bytes = 8;
  
  if( cfg->rf_asic_id == 0 )
  {
    cal_data.hdet_autocal = &rfnv_common_nv_tbl_ptr->hdet_autocal.dev0;

    /*Get this information from device dispatch*/
    cal_data.process = &device_process[0];
  }
  else if( cfg->rf_asic_id == 1 )
  {
    cal_data.hdet_autocal = &rfnv_common_nv_tbl_ptr->hdet_autocal.dev1;

    /*Get this information from device dispatch*/
    cal_data.process = &device_process[1];
  }
  else
  {
    RF_MSG_1( RF_ERROR, "rf_device_factory_create_wtr_device. Invalid"
                        "asic id %d, using hdet autocal from dev0",
              cfg->rf_asic_id );
    cal_data.hdet_autocal = &rfnv_common_nv_tbl_ptr->hdet_autocal.dev0;
  }

  wtr2605_bus_info.phy_rx_bus = cfg->bus[1];
  wtr2605_bus_info.phy_tx_bus = cfg->bus[0];

  //create WTR2605 common device
  rfdevice_rxtx_common_class* legacy_cmn_device =
    new wtr2605_rxtx_common_class(wtr2605_bus_info, cal_data);
  instance = new rfdevice_rxtx_common_adapter( legacy_cmn_device, cfg->rf_asic_id );

  wtr2605_rxtx_common_class* wtr2605_common =\
       (wtr2605_rxtx_common_class*) instance;

  /*! create CDMA device*/
  #if defined(FEATURE_CDMA1X) || defined(FEATURE_HDR)
  wtr2605_cdma_create_device(wtr2605_common->get_instance());
  new rfdevice_trx_cdma_rx_adapter((rfdevice_rxtx_common_class*)instance,
                                   RFDEVICE_TRX_PATH_0);
  new rfdevice_trx_cdma_tx_adapter((rfdevice_rxtx_common_class*)instance, 
                                   RFDEVICE_TRX_PATH_0);
  #endif
  /*! create GSM device*/
  #ifdef FEATURE_GSM 
  wtr2605_gsm_create_device(wtr2605_common->get_instance());
  new rfdevice_trx_gsm_rx_adapter((rfdevice_rxtx_common_class*)instance,
                                  RFDEVICE_TRX_PATH_0);
  new rfdevice_trx_gsm_tx_adapter((rfdevice_rxtx_common_class*)instance, 
                                  RFDEVICE_TRX_PATH_0);
  #endif
  /*! create WCDMA device*/
  #ifdef FEATURE_WCDMA
  wtr2605_wcdma_create_device(wtr2605_common->get_instance());
  new rfdevice_trx_wcdma_rx_adapter((rfdevice_rxtx_common_class*)instance,
                                   RFDEVICE_TRX_PATH_0, cfg->rf_asic_id);
  new rfdevice_trx_wcdma_tx_adapter((rfdevice_rxtx_common_class*)instance, 
                                   RFDEVICE_TRX_PATH_0);
#endif
  /*! create TDSCDMA device*/
  #ifdef FEATURE_TDSCDMA
  wtr2605_tdscdma_create_device(wtr2605_common->get_instance());
  new rfdevice_trx_tdscdma_rx_adapter((rfdevice_rxtx_common_class*)instance,
                                      RFDEVICE_TRX_PATH_0);
  new rfdevice_trx_tdscdma_tx_adapter((rfdevice_rxtx_common_class*)instance, 
                                      RFDEVICE_TRX_PATH_0);
  #endif
  /*! create GNSS device*/
  #ifdef FEATURE_CGPS
  rfdevice_wtr2605_gnss_config(RFGNSS_PATH_0);
  #endif

  return instance;
}
#endif

#ifdef FEATURE_RF_ASDIV
/*----------------------------------------------------------------------------*/
/*!
  @brief
  Device factory handler for creating an ASD Tuner Manager instance
 
  @details
  
  @param cfg
  Device config settings like manafacturer id, product id, slave id, group id
  etc needed to create this device. This information is expected from RFC. 
 
  @return rfdevice_class*
  Returns instance of rfdevice_class
*/
rfdevice_class* 
rf_device_factory_create_asd_tuner_device ( void )
{
  rfdevice_class *instance = NULL;
  
/* Instantiate an ASD Tuner Manager */  
  instance = create_rfdevice_asd_tuner_manager(); 

  return instance; 
} /* rf_device_facotry_create_asd_tuner_device */
#endif



/*----------------------------------------------------------------------------*/
/*!
  @brief
  Device factory handler to create TRX HDET adapter instance
 
  @details
 
  @param cfg
  Device config settings like manafacturer id, product id, slave id, group id
  etc needed to create this device. This information is expected from RFC.
 
  @return rfdevice_class*
  Returns instance of rfdevice_class
*/
static rfdevice_class* 
rf_device_factory_create_trx_hdet_adapter_device
( 
  rfc_device_cfg_info_type* cfg 
)
{
  rfdevice_class *instance = NULL;
  rfdevice_class *hdet_device = NULL;
  rfc_intf *rfc_cmn = rfc_intf::get_instance();
  rfdevice_rxtx_common_class* trx_ptr = NULL;

  RF_MSG_1( RF_MED, "rf_device_factory_create_trx_hdet_adapter_device() called:"
                    " dev_instance = rf_asic_id = %d", cfg->rf_asic_id);

  hdet_device = rfc_cmn->get_cmn_rf_device_object( RFDEVICE_HDET,
                                                   cfg->rf_asic_id);
  if( hdet_device != NULL)
  {
    RF_MSG( RF_MED, "TRX HDET adapter obj NOT created since "
                    "RFDEVICE_HDET obj has already been created");
    return NULL;
  }

  trx_ptr = (rfdevice_rxtx_common_class *)rfc_cmn->
      get_cmn_rf_device_object( cfg->associated_rf_device_type,   // RFDEVICE_TRANSCEIVER
                                (uint8)cfg->associated_rf_asic_id /* trx_instance */);

  if( trx_ptr != NULL )
  {

#ifndef T_RUMI_EMULATION
#ifdef FEATURE_RF_HAS_WTR2965
    if ( trx_ptr->rfdevice_id_get() == WTR2965 )
    {
      instance = wtr2965_common_create_hdet_device( trx_ptr );
    }
    else
#endif
    {
#endif
      instance = new rfc_cmn_transceiver_hdet_adapter(
                                      trx_ptr->get_instance());
#ifndef T_RUMI_EMULATION
    }
#endif
    RF_MSG( RF_MED, "TRX HDET adapter obj created since "
                    "RFDEVICE_HDET obj has NOT been created");
  }
  else
  {
    RF_MSG( RF_ERROR, "rf_device_factory_create_trx_hdet_adapter_device:"
                      " NULL trx_ptr. TRX HDET adapter obj NOT created");
  }
  return instance;
}


/*----------------------------------------------------------------------------*/
/*!
  @brief
  Programs the device's USID based on the device configuration.

  @details
  This is the external API to the RFC for programming device ID. This only applies
  to RFFE devices. Before creating the device objects, program each device's USID.
 
  @param cfg
  Device configuration settings of the device which needs to be created. Typically, this
  configuration should be autogenerated in RF card specific autogen file.
 
  @return TRUE if device id programming is successful else FALSE.
*/
boolean rf_device_factory_program_device_id(rfc_phy_device_info_type* cfg)
{
  boolean status = TRUE;  


  /*NULL pointer check*/
  if (cfg == NULL)
  {
    RF_MSG( RF_ERROR, "rf_device_factory_program_device_id  failed !  cfg structure is NULL");
    return FALSE;
  }

 /*Program the device USIDs for each physical device as per the phy cfg structure supplied*/
  switch(cfg->rf_device_id) 
  {

  case GEN_PA:
    status = rfdevice_pa_program_device_id_phy(cfg);
    break;

  case GEN_DEVICE:
  case GEN_ASM:
  case GEN_ASM_XSW:
  case GEN_LNA:
  case GEN_COUPLER:
  case QFE2550:
  #ifdef FEATURE_RF_HAS_QFE1550
  case QFE1550:
  #endif
  case TP_TUNER:
  case QAT3550:
  case QAT3514:
  case QAT3555:
  case QFE1101:
  case QFE1100:
  case QFE2101:
  case QET4101:
  case QET5102:
    status = rffe_program_assigned_usid(cfg);
    break;

  case QFE2720:
  case QFE2340:
  case QFE2320:
  case QFE2320EPT:
  case QFE3320:
  case QFE4320:
  case QFE4373FC:
  case QFE2085FC:
  case QFE1040:
  case QPA4351:
  case QPA4360:
  case QPA4361:
  case QAT3522:
    status = rffe_program_assigned_usid_ext(cfg);
    break;
  case QFE2082FC:
    status = rffe_program_assigned_usid_ext(cfg);
    break;
  case QSW8574:
    status = rffe_program_assigned_usid_ext(cfg);
    break;
  case QFE3340FC:
  case QPA4340:
  case QPA6560:
  case QPA6560_HB:
    status = rffe_program_assigned_usid_ext_product_id(cfg);
    break;
  case QAT3516:
  case QAT3518:
  case QAT5515:
  case QAT5616:
  case QAT5655:
  case QSW2630A:
  case QSW2820D:
  case QSW2820F:
  case QSW8820B:
  case QPA8673:
  case QPA8675:
  case QPA8685:
  case QPA8686:
  case QPA8687:
  case QPA8688:
    status = rffe_program_assigned_usid_burst(cfg);
    break;
  default:
    break;
  }

  return status;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Creates tuner manager based on the tuner instance provided

  @details
  This is a temporary function which would create a tuner manager based on the
  tuner device provided. This API needs to be removed when the tuner manager is
  part RFC AG.

  
  @param tuner_obj
  Instance of the tuner object
 
  @return rfdevice_class*
  Returns an instance of rfdevice_class (super class of the devices) which is to be stored
  by RFC.
*/
rfdevice_class* 
rf_device_factory_create_tuner_manager
(
  rfdevice_class* device_obj,
   rfcommon_atuner_manager_factory_type manager_type
)
{
  rfcommon_atuner_manager_factory *tuner_mgr_factory = NULL;
  rfdevice_class *tuner_mgr_obj = NULL;
  
  if(device_obj == NULL)
  {
    RF_MSG( RF_ERROR, "rf_device_factory_create_tuner_manager() failed as"
                      " NULL tuner device provided!");
    return tuner_mgr_obj;
  }

  /* Create the instance for Tuner mananger factory */
  tuner_mgr_factory = new rfcommon_atuner_manager_factory (  );

  /* Check if tuner_mgr_factory is valid*/
  if(tuner_mgr_factory != NULL)
  {
    /*Using the tuner manager factory, we can create the tuner 
      manager of interest*/
    /*!Note: By default creating AOL tuner manager for now*/
    tuner_mgr_obj = tuner_mgr_factory->create_atuner_manager( manager_type, 
                                                                 device_obj);
  }
  else
  {
    RF_MSG( RF_ERROR, "rf_device_factory_create_tuner_manager() failed as"
                      "Tuner manager factory instance cannot be created!");
  }

  /*Tuner manager factory*/
  delete tuner_mgr_factory;

  /*Return Tuner manager created */
  return tuner_mgr_obj;
  
} /*rf_device_factory_create_tuner_manager*/

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Creates device instance based on the device configuration provided.

  @details
  This is the external API to the RFC for creating device instances. The device
  factory instantiates the common and tech specific instances for a specific device
  based on the device configuration parameter passed in. Common instances will have
  references to tech specific instances. Every device type (ex: WTR1605,
  WTR2605) is allowed one handle. All tech specific instance creation and header file
  includes need to be featurized.
 
  This API will only dispatch the call to device specific create handler. All logic for
  creating device needs to be maintained in the device specific handler.
  
  @param cfg
  Device configuration settings of the device which needs to be created. Typically, this
  configuration should be autogenerated in RF card specific autogen file.
 
  @return
  Returns an instance of rfdevice_class (super class of the devices) which is to be stored
  by RFC.
*/
rfdevice_class* rf_device_factory_create_device
(
  rfc_device_cfg_info_type* cfg
)
{
  rfdevice_class* instance = NULL;
//*NULL Pointer Checks*/
  if (cfg == NULL)
  {
	 
	/* crashing phone as critical params missing */   
	ERR_FATAL("rf_device_factory_create_device creation failed. cfg == NULL", 0, 0, 0);
    
  }

  switch(cfg->rf_device_id) 
  {

  case WTR2605:
#if (!( defined(FEATURE_JOLOKIA_MODEM) || defined(FEATURE_TABASCO_MODEM)))
    instance = rf_device_factory_create_wtr2605_device( cfg );
#endif
    break;

    case TRX_HDET:
      instance = rf_device_factory_create_trx_hdet_adapter_device (cfg);
      break;

   default:
      break;
  }

  return instance;
}


/*!
  @brief
  Factory method to create instances of 3rd party logical device instances

  @details
  Third party devices could have multiple logical components.
  This API creates the logical components  for 3rd party devices
  and associates it with its parent physical device.

  @param rfdevice_physical_third_party_p
  Pointer to the parent phsyical device 
 
  @param logical_device_cfg
  Pointer to the logical device cfg of the 3rd party device
 
  @return
  Pointer to the logical device object of the 3rd party device, or NULL if no device could
  be created.

  @retval NULL
  The requested device could not be created successfully.
*/
rfdevice_logical_component* create_gen_device_object(
                                                        rfdevice_physical_device     *rfdevice_physical_third_party_p,
                                                        rfc_logical_device_info_type      *logical_device_cfg
                                                    )
{

  if ( (NULL == rfdevice_physical_third_party_p) || (NULL == logical_device_cfg))
  {
     RF_MSG( RF_ERROR, " create_gen_device_object() -  NULL pointer detected!" );
     return NULL;
  }

  switch(logical_device_cfg->rf_device_id) 
  {
  
   case GEN_ASM:
     return (create_gen_asm_object( rfdevice_physical_third_party_p , logical_device_cfg ) );

   case GEN_PA:
     return (create_gen_pa_object( rfdevice_physical_third_party_p , logical_device_cfg ) );
 
   case GEN_LNA:
     return (create_gen_elna_object( rfdevice_physical_third_party_p , logical_device_cfg ) );

   case GEN_COUPLER:
     return (create_gen_coupler_object( rfdevice_physical_third_party_p , logical_device_cfg ) );
     
   case GEN_ASM_XSW:
     return (create_gen_asm_common_xsw_object( rfdevice_physical_third_party_p , logical_device_cfg ) );

   default:
     return NULL;
	
  }

}

/*!
  @brief
  Factory method to create instances of physical device interfaces.

  @details
  The physical device object represents the hardware device in the system.
  This method will construct a software object that represents the physical
  device, and returns a pointer to the physical device abstraction.

  @param cfg
  Pointer to a device configuration structure that describes what type of
  device to try and create.

  @return
  Pointer to the physical device that was created, or NULL if no device could
  be created.

  @retval NULL
  The requested device could not be created successfully.
*/
rfdevice_physical_device*
rf_device_factory_create_phys_device
(
  rfc_phy_device_info_type* cfg
)
{
  /*NULL Pointer Checks*/
 if (cfg == NULL)
 {
		 
	/* crashing phone as critical params missing */  
	ERR_FATAL("rf_device_factory_create_phys_device creation failed. cfg == NULL", 0, 0, 0);

 }

 if ( cfg->rf_device_comm_protocol == RFDEVICE_COMM_PROTO_RFFE) 
 {
 if ( !rffe_scan(cfg) )
 {
   RF_MSG_1( RF_ERROR, "rf_device_factory_create_phys_device: Device %d not found",
                                                               cfg->rf_device_id );
   return NULL;
 }
 }
  switch(cfg->rf_device_id) 
  {
  case TEST_DEVICE:
    return new test_device_physical_device(cfg);

  #ifdef FEATURE_RF_HAS_WTR3925
  case WTR3925:
    return new wtr3925_physical_device(cfg);
  #endif

#ifndef T_RUMI_EMULATION
  #ifdef FEATURE_RF_HAS_WTR2965
  case WTR2965:
    return new wtr2965_physical_device(cfg);
  #endif
  #endif

#ifdef FEATURE_RF_HAS_QTUNER

  case QFE2550:
    return new qfe2550_physical_device(cfg, FALSE);

#ifdef FEATURE_RF_HAS_QFE1550
  case QFE1550:
    return new qfe1550_physical_device(cfg,FALSE);
#endif
   case QAT3550:
    return new qat3550_physical_device(cfg,FALSE);

   case QAT3555:
    return new qat3555_physical_device(cfg,FALSE);
    
   case QAT3514:
    return new qat3514_physical_device(cfg,FALSE);

   case QAT3516:
    return new qat3516_physical_device(cfg,FALSE);
	
   case QAT3518:
    return new qat3518_physical_device(cfg,FALSE);

   case QAT5515:
    return new qat5515_physical_device(cfg,FALSE);	
   
   case QAT5616:
    return new qat5616_physical_device(cfg,FALSE);

   case QAT5655:
    return new qat5655_physical_device(cfg,FALSE);
#endif /* FEATURE_RF_HAS_QTUNER */

  case QAT3522:
    return new qat3522_physical_device(cfg);

  case QFE1100:
  case QFE1101:
  case QFE2101:
  case QET4101:
  case QET5102:
    return new papm_physical_device(cfg);



  case QFE2320:
  case QFE2320EPT:
    return new qfe2320_physical_device(cfg);

  case QFE2340:
    return new qfe2340_physical_device(cfg);

  case QFE1040:
    return new qfe1040_physical_device(cfg);
  case QFE2082FC:
    return new qfe2082fc_physical_device(cfg);
  case QFE3320:
    return new qfe3320_physical_device(cfg);
  case QFE3340FC:
    return new qfe3340fc_physical_device(cfg);
  case QFE4320:
    return new qfe4320_physical_device(cfg);
#ifdef FEATURE_RF_HAS_QPA4340
  case QPA4340:
    return new qpa4340_physical_device(cfg);
#endif  
  case QPA4360:
    return new qpa4360_physical_device(cfg);  
  case QPA4361:
    return new qpa4361_physical_device(cfg);  
  case QPA6560:
    return new qpa6560_physical_device(cfg);  
  case QPA6560_HB:
    return new qpa6560_hb_physical_device(cfg);  
  case QPA8673:
    return new qpa8673_physical_device(cfg);
  case QPA8675:
    return new qpa8675_physical_device(cfg);
  case QPA8685:
    return new qpa8685_physical_device(cfg);
  case QPA8686:
    return new qpa8686_physical_device(cfg);
  case QPA8687:
    return new qpa8687_physical_device(cfg);
  case QPA8688:
    return new qpa8688_physical_device(cfg);
  case QSW2630A:
    return new qsw2630a_physical_device(cfg);
  case QSW2820D:
    return new qsw2820d_physical_device(cfg);
  case QSW2820F:
    return new qsw2820f_physical_device(cfg);
  case QSW8574:
    return new qsw8574_physical_device(cfg);  
  case QSW8820B:
    return new qsw8820b_physical_device(cfg);    
  case QFE4373FC:
    return new qfe4373fc_physical_device(cfg);
  case QFE2085FC:
    return new qfe2085fc_physical_device(cfg);

  case QPA4351:
    return new qpa4351_physical_device(cfg);
  case GEN_DEVICE:
  case GEN_PA:
  case GEN_ASM:
  case GEN_ASM_XSW:
  case GEN_LNA:
  case GEN_COUPLER:
    return new rfdevice_physical_third_party(cfg);

  default:
    return NULL;
  }
}


/*!
  @brief

  @details
*/
boolean
rf_device_factory_create_gnss_device
( 
  rfc_device_info_type* cfg
)
{
  boolean status = TRUE;
  /*NULL Pointer Checks*/
  if (cfg == NULL)
  {
    /* crashing phone as critical params missing */   
    ERR_FATAL("rf_device_factory_create_gnss_device creation failed. cfg == NULL", 
              0, 0, 0);
    
  }
#ifdef FEATURE_CGPS
  switch( cfg->rf_asic_info[0].device_id )
  {
   #ifdef FEATURE_RF_HAS_WTR3925
   case WTR3925:
   status &= 
     rfdevice_wtr3925_gnss_set_instance_id( cfg->rf_asic_info[0].instance );
   break;
   #endif
 
   #ifndef T_RUMI_EMULATION
#ifdef FEATURE_RF_HAS_WTR2965
   case WTR2965:
    status &= 
      rfdevice_wtr2965_gnss_set_instance_id( cfg->rf_asic_info[0].instance );
    break;
#endif
   #endif

  default:
    RF_MSG_1( RF_ERROR, "rf_device_factory_create_gnss_device. Unsupported device_id %d ",
              cfg->rf_asic_info[0].device_id );\
    status = FALSE;
    break;

  } /* switch( cfg->rf_asic_info[0].device_id ) */

  if(status == FALSE)
  {
    RF_MSG_2( RF_ERROR, "rf_device_factory_create_gnss_device Failed!"
                        "device_id %d, instance %d ",
              cfg->rf_asic_info[0].device_id,
              cfg->rf_asic_info[0].instance );
  }
#endif
  return status;
} /* rf_device_factory_create_gnss_device */
