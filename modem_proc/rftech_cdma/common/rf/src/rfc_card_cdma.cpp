/*!
  @file
  rfc_cdma.cpp

  @brief
  <b>Card-specific developers should refer to this interface.</b> This file
  defines the base Class for CDMA, needed to implement RFC for specific RF card

  @addtogroup RF_RFC_CDMA_BASE_RFCARD_INTF
  @{
*/

/*==============================================================================

  Copyright (c) 2010 - 2015 Qualcomm Technologies Incorporated. All Rights Reserved

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

  $Header: //components/rel/rftech_cdma.mpss/3.11/common/rf/src/rfc_card_cdma.cpp#1 $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
05/30/19   mk      compilation error fix
02/28/18   am      QAT3522 xsw tech side changes
12/08/16   rs      Passing the correct script size for ASM device config
01/27/16   pk      Assign different path for different devices
01/05/16   rs      fix crash for FTM_GET_RFFE_DEVICE_INFO cmd
12/03/15   rs      F3 message reduction to avoid F3 flood
11/18/15   rs      KW error Fix, corrected valid_nv_path_array index check
11/02/15   dbc     Add configure_asm_xsw_port for antenna switching
10/13/15   Saul    GNSS Blanking
12/23/14   vc      Use Band specific column for default antenna info
01/27/15   vc      Propogating immediate flag for FTM AsDiv support
12/23/14   vc      Use Band specific column for default antenna info
09/14/15   pk      Added tuner manager update in the alt path update.
07/24/15   dyc     Un-paired (dummy) tuner support.
07/23/15   wwl     Fix ASDIV GRFC generation missing rxtx bug
07/22/15   zhw     Added support for dynamic Rx MDSP Chain mapping
07/09/15   zhw     KW Fix
07/08/15   Saul    RF Device Max Bandwidth Interface
07/02/15   zhw     Added support for RFM Device 8/9
06/29/15   Saul    Use Tx device for fbrx-related RFC queries
06/11/15   yb      Added API to retrieve tuner device object
06/15/15   zhw     Handle NULL CDMA RFC in is_nv_path_valid
06/08/15   zhw     Fix compiler error
06/08/15   zhw     CDMA NV path specific RFC construcation with new BC Config
05/29/15   Saul    Call generate_rfcard_dynamic_properties at card creation.
05/27/15   zhw     Map BC COnfig to logical device for CDMA
                   Remove temporary API
03/05/15   vv      API support for rfc to capture modem chain to trx and VCO path mapping 
05/04/15   zhw     Update Tuner object storage
04/21/15   ndb     Remove the CA PORT layer
04/09/15   zhw     Fix RFC CDMA max number of device check
03/30/15   zhw     [Part 1]Remove hard-coded Device to path & chain mapping in RFC
                   Impelement temporary Tx device query API
03/11/15   zhw     OFT compiler fix
03/11/15   zhw     Remove Tuner featurization
03/11/15   zhw     AsDiv fix and KW fix
03/02/15   zhw     Refactor RFC for splittingTx from Rx devices
01/28/15   spa     Initialize eLNA index to -1
01/06/15   vv      API support for rfc to capture the modem chain to trx mapping  
12/17/14   wwl     add get_fb_device_modem_chain function implementation
12/16/14   spa     ELNA object creation support
12/09/14   spa     Use Band specific column for default antenna info
12/05/14   spa     Use antenna swap table from RFC for ASDiv config info
11/28/14   ljz     Set paired_device invalid for RFC doesn't support div
11/24/14   dyc     Support updating rfc_asdiv_config_info table from RFC info
11/12/14   spa     Change tx_nv_path to be per band per logical device
11/11/14   spa     Ignore error if XSW object is NULL since few RFCs might
                    not have a valid object for it
11/06/14   spa     Change rx_nv_path to be per band per logical device
11/05/14   spa     Pass in BC config queried based on the device
11/05/14   spa     Skip creating rx/tx device objects for unsupported bands in
                   BC config
11/04/14   spa     DO NOT return failure if XSW object is not present
10/27/14   vv      Add papm device info api support
10/23/14   spa     Remove XSW related operations in _rx_wakeup_grfc_script API
10/21/14   px      Added RFM Device support for PAPM
10/21/14   vc      Fixing error of tabasco
09/16/14   sn      ASM set band RFM Device support
08/25/14   spa     Add num_grfc pointer as arg to generate_tx_on_off_grfc_script
08/19/14   zhw     Query AsDiv GRFC signal list for GRFC XSW
07/17/14   spa     Added api to update RFC info for the alternate path
06/05/14   spa     Skip creating rx/tx device objects for unsupported bands in
                   BC config
27/02/14   vr      Fix to correct tx device mapping per device.
05/05/14   spa     Not return failure if device ptr for unsupported band is NULL
05/02/14   spa     Propagate CDMA RFC failure up to RFM init
04/08/14   yb      Added support for coupler
04/07/14   APU     Added support for RFM_DEVICES 2 and 3 even when RFM_DEVICES
                   0 & 1 are absent for CDMA.
04/02/14   Saul    RFC. Intermediate check-in for alternate path.
02/04/14   zhw     Support for updating tuner list during runtime
03/25/14   yb      Added support for Tuner linearizer
03/10/13   zhw     Set paired SSMA device of Dev 2 to INVALID_DEVICE
12/02/13   spa     Call 'get_device_objects' APi for device 1 and 3 as well
11/11/13   APU     Removed hardcoding for RFM_DEVICE_2 to be a Tx device.
10/31/13   shb     Converted TRX common device interface to C++
10/02/13   APU     Split ASM logically into Rx and TX ASMs
09/04/13   zhw     Tuner AsDiv support
09/03/13   JJ      Remove func which loads RFC with given dev info and sig config
08/26/13   shb     Updates for new C++ transceiver device interface
08/01/13   JJ      Featurize for bolt rumi
08/01/13   JJ      Added func to load RFC with given dev info and sig config
07/24/13   zhw     Fix F3 levels
07/18/13   zhw     Add AsDiv Support for setting GRFC switches
07/18/13   spa     Added support for multiple ASM/Tuners
07/08/13   yzw     Add ASM support for FTM command FTM_GET_RFFE_DEVICE_INFO
06/30/13   yzw     Add method get_rffe_device_info() to get RFFE device information
06/14/13   spa     Use CDMA wrapper object for antenna tuner
06/10/13   JJ      Remove break to avoid crash on phone which has no asm/qpoet
06/07/13   JJ      Fix a minor error in checking null pointer
06/06/13   JJ      Clean redundant RFC APIs and create API get_device_objects
05/29/13   spa     Added delay param to APIs for getting GRFC scripts
05/06/13   vb      Bug fix for assiging the Tuner port object
05/06/13   vb      Support for Tuner manager factory
05/02/13   vb      KW Fix: Check for Tuner Manager validity
05/02/13   vb      Tuner CA changes
04/15/13   APU     Add sub class support to BC11 for the following sub-classes:
                   RF_SC_8 | RF_SC_9 | RF_SC_10 | RF_SC_11
03/22/13   sr      changes to PAPM device interface.
03/13/13   sn      Added XPT functions
03/13/13   aro     Removed deleting of nonCDMA device object
03/11/13   aro     Added memory cleanup in CDMA RFC class destructor
03/05/13   vrb     Replace RFC accessor param init_req boolean by a req enum
03/05/13   zhw     Remove trx_hdet objection init since common obj is available
02/28/13   spa     Reinit counters for devices at beginning in loop for bands
02/15/13   bmg     Added official tuner interface support.
02/13/13   bmg     Refactored QTuner support for generalized antenna tuner
                   interface.
02/05/13   bmg     Added antenna tuner pointer to CDMA RFC
01/10/13   spa     Support for RFC based PA LUT mapping
01/15/13   cd      Add support for CDMA ASM aggregator functionality
01/14/13   adk     Updated get_hdet_object
01/14/13   Saul    CDMA. 1x EPT/ET call support.
12/27/12   shb     RX device wrapper added for multiple RX devices per band
12/27/12   shb     Updates for different physical rx_device per band
12/27/12   adk     Added support for QFE1510 HDET object on RFM_DEVICE_2
12/21/12   zhw     Compiler warning fix.
12/21/12   zhw     KW error fix. No tuner obj for VZ_SV_QFE1320 fix
12/19/12   zhw     Create HDET object for TRX HDET adatper
12/09/12   zhw     Create GRFC PA object for CDMA RFC
12/11/12   hdz     Enabled timing support in generate_tx_on_off_grfc_script()
12/09/12   zhw     Make GRFC overwritten API take RFC logic instead of boolean
12/05/12   zhw     Check for init column in RFC rx/tx table. It should be empty
12/04/12   zhw     API for checking init columns in RFC for CDMA Rx/Tx
11/30/12   adk     Dime RFC updates to get hdet device instance
11/29/12   zhw     Back off init_rx, init_tx change until init is not path common
11/26/12   adk     Dime RFC updates to get tuner device instance
11/16/12   av/spa      Configuring the init_rx, init_tx grfcs.
11/09/12   zhw     Temp fix for hard coded misreference of LUT table name.
11/06/12   spa     Removed refs to unused enums pa/asm bus type and com type
11/05/12   zhw     Support for PA On/Off GRFC script in RF On/Off event
11/05/12   spa     Populate RF path for band path config, even though unused in
                   CDMA, as a precautionary measure
10/30/12   spa     Use RFM_DEVICE_3 as diversity device on SV path
10/30/12   zhw     Support for getting device info from RFC
10/29/12   spa     Added support to query for PA R0/R1 MSM information
10/23/12   zhw     Support for getting Tx Template ID based on RFC query
10/25/12   zhw     Query for Band/Path specific signals for sleep rx/tx
10/22/12   zhw     Temporarily HACK max rf device to be 5.
                   TODO: Remove hack once GPS doesn't use device 3.
10/16/12   zhw     RF On/Off grfc scripting using overwritten rfc signal
10/02/12   spa     Temporary hard coding of paired device for Second WTR (SV)
10/02/12   spa     Bug fixes in PA/ASM object retrieving APIs
09/27/12   zhw     Remove write_sbi/grfc_via_rfsw flag
09/24/12   spa     Hard coding logic level for tx wakeup and sleep (temporary)
09/24/12   spa     Removed hard coding of max devices
09/19/12   spa     Added API create_device_instance for all RF cards
09/18/12   spa     Added APIs to populate band agnostic device RxTx items
09/14/12   spa     Added generic APIs to set Tx and Rx devices
08/29/12   APU     Fixed compiler warning.
08/28/12   spa     Added implementation to get QPOET object
08/28/12   APU     Added support to get modem_chain from rfc_device_info
08/25/12   spa     Added implementation to get pa/asm object and set port map
08/10/12   vrb     Use updated interfaces to push band port information to dev
08/08/12   spa     Added API to set band port information in base class
07/27/12   spa     updated script generating APIs to have valid logical device
07/25/12   sty     updated get_tx_lut_table_name() to get port info
07/25/12   sty     Added device info - deleted todo comments
07/16/12   APU     Added ASM and PA pointers
07/16/12   zhw     Fix logical_device field not being populated in
                   is_device_tunable()
07/11/12   shb     Updated script_buffer to be of rf_buffer_intf* type
                   instead of void ptr
07/09/12   spa     Renamed grfc_buf_ptr to script_buf_ptr
07/09/12   sr      changes to use the rfc_data object instead of rfc_ag function ptr table.
07/06/12   spa     All generate_grfc_script APIs now accept void pointer to
                   script buffer and no longer have 'is_irat' flag and use
                   rfc_common_generate_grfc_script() API
07/02/12   spa     Compiler warning fix
06/29/12   sr      RFC changes : use rf_hal_buffer to generate scripts
06/07/12   sbo     Added missing SC in BC0 and BC5
05/29/12   zhw     Moved debug flag do_grfc_write to rf_cdma_data
05/23/12   sty     Added get_rf_chip_num
05/18/12   sr      RFC-EFS code clean-up changes.
05/14/12   zhw     Refined added APIs and removed unused APIs
05/14/12   zhw     Added comments
05/09/12   zhw     Added GRFC scripting generation APIs
05/11/12   sty     Added changes to accomodate calling AG init() with device
05/08/12   zhw     Fixed KW Warning
05/05/12   sty     Added support for EFS-RFC
05/03/12   sb      Fixed KW warning
04/26/12   sty     Call rfc_common_config_signal() instead of
                   rfc_common_config_signal_temp()
04/16/12   Saul    QFE. Fix strip/pack errors.
04/12/12   sr      made changes to put all the GPIOs into low power mode during sleep.
04/11/12   Saul    QFE. Initial support.
04/05/12   cd      Fixed get GRFC num functionality
02/18/11   sty     Renamed getinstance to get_instance
02/17/11   sty     Renamed rfc_cdma.h to rfc_card_cdma.h
01/25/11   sty     Added copy_grfc_table() utility
01/20/11   sty     Renamed rfc_cdma_card_interface to rfc_cdma_card_intf
01/19/11   sty     Replaced ptrQ with getinstance
01/07/11   sty     Doxygen update
01/06/11   sty     Minor update to comments
01/06/11   sty     Renamed rfc_cdma to rfc_cdma_card_intf
01/05/11   sty     Doxygen changes
12/20/10   sty     Added Doxygen comments
12/20/10   sty     Initial Revision

==============================================================================*/
#include "comdef.h"
#ifdef FEATURE_CDMA1X
#include "rf_cdma_pa.h"
#include "rfa_variation.h"
#include "rfc_card_cdma.h"
#include "rfcommon_msg.h"
#include "rfm_1x.h"
#include "modem_mem.h"
#include "rxlm_intf.h"
#include "txlm_intf.h"
#include "rf_cdma_data.h"
#include "rfc_cdma_data.h"
#include "rfc_common_data.h"
#include "rfc_class.h"
#include "rfdevice_rxtx_common_class.h"
#include "rfdevice_asm_intf.h"
#include "rfdevice_pa.h"
#include "rfdevice_xsw.h"
#include "rfdevice_asm.h"
#include "rfc_common_grfc_pa.h"
#include "rfc_cdma_device_wrapper.h"
#include "rfcommon_atuner_manager_factory.h"
#include "rfdevice_trx_cdma_rx.h"
#include "rfdevice_coupler.h"
#include "rfdevice_lna_intf.h"
#include "rfcommon_core_utils.h"
#include "rfdevice_xsw_intf.h"
#include "stringl.h"


#ifdef FEATURE_RF_ASDIV
#include "rfcommon_asdiv_manager.h"
#include "rfcommon_asdiv_tuner_manager.h"
#include "rf_cdma_mc.h"
#endif /* FEATURE_RF_ASDIV */

#include "rf_cdma_utils_freq.h"
#include "rfcommon_nv_mm.h"
#include "rf_cdma_utils.h"

/*----------------------------------------------------------------------------*/
/*!
  @name RFC CDMA Class and methods

  @brief The following detail the base class and methods used in RFC for CDMA

*/
/*! @{ */

/*!
  @brief
  Specifies the max number of valid RFM devices (i.e, if the device can do Rx
  and/or TX).
  Currently, the max configuration is to have 2 RF devices on a RF card - hence
  we have 4 valid paths (RFM_DEVICE_0, RFM_DEVICE_1, RFM_DEVICE_2, RFM_DEVICE_3)
  Starting from TH 2.0 RFM_DEVICE_6 & 7 support is added.

  So the max valid devices is 8
*/
#define RFC_CDMA_MAX_VALID_RFM_DEVICES                                         8


/*----------------------------------------------------------------------------*/
/*!
  @brief Pointer that will be used to dereference the RFC class

*/
rfc_cdma_card_intf *rfc_cdma_card_intf::rfc_cdma_ptr
               = (rfc_cdma_card_intf *)NULL;


/*!
  @brief
  Sets the logical device structure to initial values.

  @param logical_dev_param_ptr
  The logical param that needs to be initialized.

  @return void
*/
void
rfc_cdma_init_logical_device
(
  rfc_cdma_logical_device_params_type* logical_dev_param_ptr
)
{
  logical_dev_param_ptr->ftm_rxlm_chain_hdr  =  RXLM_CHAIN_MAX;
  logical_dev_param_ptr->ftm_rxlm_chain_onex =  RXLM_CHAIN_MAX;
  logical_dev_param_ptr->ftm_txlm_chain_hdr  =  TXLM_CHAIN_MAX;
  logical_dev_param_ptr->ftm_txlm_chain_onex =  TXLM_CHAIN_MAX;
  logical_dev_param_ptr->paired_device       =  RFM_INVALID_DEVICE;
  /* RX MDSP PATH is set dynamically during wakeup Rx & enable diversity
  based on the following mapping: Prx - MDSP Chain 0, Drx - MDSP Chain 1*/
  logical_dev_param_ptr->rx_mdsp_path        =  RF_CDMA_MDSP_CHAIN_INVALID;
  logical_dev_param_ptr->rxlm_adc            =  RXLM_ADC_MAX;
  logical_dev_param_ptr->tx_mdsp_path        =  RF_CDMA_MDSP_CHAIN_INVALID;
  logical_dev_param_ptr->txlm_dac            =  TXLM_DAC_MAX;

  /*Initialize pa, asm and qpoet arrays to NULL*/
  for( int band=0 ; band < RFM_CDMA_MAX_BAND ; band++ )
  {
    logical_dev_param_ptr->tx_nv_path[band] = RF_CDMA_NV_PATH_NUM;
    logical_dev_param_ptr->rx_nv_path[band] = RF_CDMA_NV_PATH_NUM;
    logical_dev_param_ptr->rx_device[band] =  NULL;
    logical_dev_param_ptr->tx_device[band]   = NULL ;
    logical_dev_param_ptr->pa[band]  = NULL;
    logical_dev_param_ptr->sm[band]  = NULL;
    logical_dev_param_ptr->qpoet_obj[band]  = NULL;
    #ifdef FEATURE_RF_HAS_QTUNER
    logical_dev_param_ptr->qtuner_obj[band]  = NULL;
    #endif
    logical_dev_param_ptr->ant_tuner[band]  = NULL;
    logical_dev_param_ptr->tx_ant_tuner[band]  = NULL;
    logical_dev_param_ptr->hdet_obj[band]  = NULL;
    logical_dev_param_ptr->coupler_obj[band]  = NULL;    
  } /* For each band (objects that have a band array) */
}


/*!
  @brief
  Helper that sets the BC information initial values.

  @param logical_dev_param_ptr
  The BC information that needs to be initialized.

  @param rf_dev
  The index of RF device that needs to be initialized and can be
  between 0 and RFC_CDMA_MAX_VALID_RFM_DEVICES.

  @return boolean
  TRUE:   Function successful.
  FALSE:  Function has failed.
*/
boolean
rfc_cdma_init_bc_info
               (
  rfc_cdma_sub_band_class_info_type*& per_device_sub_bc_info_ptr ,
  uint8 rf_dev
)
{
  boolean success = TRUE ;
    if ( per_device_sub_bc_info_ptr != NULL )
    {
      /* intialize the data structures with initial values */
        for ( int rf_band = RFM_CDMA_BC0; rf_band < RFM_CDMA_MAX_BAND;
              rf_band++ )
        {
          per_device_sub_bc_info_ptr[rf_dev].sub_classes_supported[rf_band] = 0;
        }
      }
    else
    {
      success = FALSE;
      RF_MSG ( RF_ERROR, "helper_for_rfc_cdma_card_intf: Failed in"
                         "malloc for per_device_sub_bc_info_ptr" );
    }
  return success ;
}


/*!
  @brief
  Helper that sets the Tx LUT initial values.

  @param tx_lut_type_to_use_ptr
  The Tx LUT that needs to be initialized.

  @param rf_dev
  The index of RF device that needs to be initialized and can be
  between 0 and RFC_CDMA_MAX_VALID_RFM_DEVICES.

  @return boolean
  TRUE:   Function successful.
  FALSE:  Function has failed.
*/
boolean
rfc_cdma_init_tx_lut
               (
  rfc_cdma_lut_to_use_for_pa_state_type*& tx_lut_type_to_use_ptr ,
  uint8 rf_dev
)
{
  boolean success = TRUE ;
    if ( tx_lut_type_to_use_ptr != NULL )
    {
      /* intialize the data structure with initial values */
        for ( int rf_band = RFM_CDMA_BC0; rf_band < RFM_CDMA_MAX_BAND;
              rf_band++ )
        {
          tx_lut_type_to_use_ptr[rf_dev].tx_lut_type[rf_band][RFM_PA_GAIN_STATE_0]
                                                       = RFC_CDMA_INVALID_TABLE;
          tx_lut_type_to_use_ptr[rf_dev].tx_lut_type[rf_band][RFM_PA_GAIN_STATE_1]
                                                       = RFC_CDMA_INVALID_TABLE;
          tx_lut_type_to_use_ptr[rf_dev].tx_lut_type[rf_band][RFM_PA_GAIN_STATE_2]
                                                       = RFC_CDMA_INVALID_TABLE;
          tx_lut_type_to_use_ptr[rf_dev].tx_lut_type[rf_band][RFM_PA_GAIN_STATE_3]
                                                       = RFC_CDMA_INVALID_TABLE;
        }
      }
    else
    {
      success = FALSE;
      RF_MSG ( RF_ERROR, "helper_for_rfc_cdma_card_intf: Failed in"
                         "malloc for tx_lut_type_to_use_ptr" );
    }
  return success ;
}


/*!
  @brief
  Method that allocates memory for the logical device.

  @return boolean
  TRUE:   Allocation successful.
  FALSE: Memory allocation has failed.
*/
boolean
rfc_cdma_card_intf::rfc_cdma_allocate_mem_for_device ( )
{
  boolean success = TRUE ;
  uint8 device;
  rfc_cfg_params_type band_path_cfg;
  rfc_sig_cfg_type* mode_rf_signals = NULL;
  rfc_cdma_data *rfc_data = rfc_cdma_data::get_instance();  
  uint8 max_dev;
  uint8 max_rfm_dev_found = 0;
 
  memset ( is_rfm_device_rx_tx , FALSE , ( sizeof(is_rfm_device_rx_tx) ) ) ;
  memset ( rfc_cdma_valid_rfm_devices , FALSE ,
            ( sizeof(rfc_cdma_valid_rfm_devices) ) ) ;

  for ( device = RFM_DEVICE_0 ; device < RFM_MAX_WAN_DEVICES ; device ++ )
  {
    logical_dev_param_ptr[device] = NULL;
  }

  for ( device = RFM_DEVICE_0 ; device < RFM_MAX_WAN_DEVICES ; device ++ )
  {      
    uint8 rxtx_counter ;
    /* Query for Band/Path specific signals. Use req=RFC_REQ_DEFAULT_GET_DATA */ 
    band_path_cfg.logical_device = (rfm_device_enum_type)device ;
    band_path_cfg.req = RFC_REQ_DEFAULT_GET_DATA ;
    band_path_cfg.alternate_path = 0 ;
    band_path_cfg.path = RF_PATH_MAX ;

    //RF_MSG_1(RF_HIGH,"device %d", device);
    /* LOOP to get Tx or Rx device */
    for ( rxtx_counter = RFC_CONFIG_RX ;  rxtx_counter < RFC_CONFIG_RXTX_MAX ;
          rxtx_counter++  )
    {
      uint8 band;
      band_path_cfg.rx_tx = (rfc_rxtx_enum_type)rxtx_counter ;

      //RF_MSG_1(RF_HIGH,"rxtx_counter %d", rxtx_counter);
      /* Find the max RFM_DEVICE_x in CDMA RFC columns */
      for ( band = RFM_CDMA_BC0 ; band < RFM_CDMA_MAX_BAND ; band ++ )
      { 
        boolean rfc_valid = FALSE ;

        //RF_MSG_1(RF_HIGH,"band %d", band);
        /* If A Rx/Tx device has been found in RFC, stop looping for next band*/
        if ( is_rfm_device_rx_tx [device][rxtx_counter] == TRUE )
        {
          break;
        }

        band_path_cfg.band = (rfm_cdma_band_class_type)band;

        /* Attempt to locate RFC Column given device/band/trx*/
        rfc_valid = rfc_data->sig_cfg_data_get ( &band_path_cfg, &mode_rf_signals ) ;

        if ( rfc_valid == TRUE )
        {
          //RF_MSG(RF_HIGH,"RFC column found");
          /* If RFC is found, updat the maximum rfm device number*/
          if ( device > max_rfm_dev_found  )
          {
            max_rfm_dev_found = device;          
          }

          /* Set this RFM device as valid*/
          rfc_cdma_valid_rfm_devices[device] = TRUE;         

          /* Set this RFM device valid for Rx/Tx */
          is_rfm_device_rx_tx [device][rxtx_counter] = TRUE;

          /* Allocate Memory for logical Dev params */
          /* TODO */
          if ( logical_dev_param_ptr[device] == NULL )
          {
            logical_dev_param_ptr[device] = 
                          (rfc_cdma_logical_device_params_type*) modem_mem_alloc( 
                             sizeof( rfc_cdma_logical_device_params_type ), 
                             MODEM_MEM_CLIENT_RFA ) ;

            success &= ( logical_dev_param_ptr[device] != NULL ) ? TRUE : FALSE ;

  if ( TRUE == success )
  {
    /* Set all pointers to 0 to guard against uninitialized heap */
              memset ( logical_dev_param_ptr[device] , 0 , 
                       sizeof(rfc_cdma_logical_device_params_type ) ) ;
            }     
  }
        }/* if ( rfc_valid == TRUE ) */
      }/* for ( band = RFM_CDMA_BC0 ; band < RFM_CDMA_MAX_BAND ; band ++ ) */
    }/* for ( rxtx_counter = 0 ;  rxtx_counter < RFC_CONFIG_RXTX_MAX ;*/    
  } /* RFM_DEVICE_X Loop */  

  /* Determine how much memory to allocate for that's enough to cover max number
  of devices, which is max rfm_device plus 1 */
  rfc_cmda_set_rfm_max_valid_devices(max_rfm_dev_found+1);

  max_dev = rfc_cdma_get_rfm_max_valid_devices();

  /* allocate memory for per_device_sub_bc_info_ptr */
  per_device_sub_bc_info_ptr =
               (rfc_cdma_sub_band_class_info_type*)modem_mem_alloc
               (
                 max_dev*sizeof(rfc_cdma_sub_band_class_info_type),
                 MODEM_MEM_CLIENT_RFA
               ) ;
  success &= ( per_device_sub_bc_info_ptr != NULL ) ? TRUE : FALSE ;


  /* allocate memory for TX LUT table info per band per PA state */
  tx_lut_type_to_use_ptr =
               (rfc_cdma_lut_to_use_for_pa_state_type*)modem_mem_alloc
               (
                 max_dev*sizeof(rfc_cdma_lut_to_use_for_pa_state_type),
                 MODEM_MEM_CLIENT_RFA
               ) ;
  success &= ( tx_lut_type_to_use_ptr != NULL ) ? TRUE : FALSE ;

  /* allocate memory for dev_int_param */
  dev_int_param_ptr =
    (rfc_cdma_logical_device_internal_params_type*)modem_mem_alloc
    (
      max_dev*sizeof(rfc_cdma_logical_device_internal_params_type),
      MODEM_MEM_CLIENT_RFA
    );
  success &= ( tx_lut_type_to_use_ptr != NULL ) ? TRUE : FALSE ;

  return success ;
}/* rfc_cdma_card_intf::rfc_cdma_allocate_mem_for_device ()*/


/*!
  @brief
  Method that populates the logical device params according to
  what the device is.

  @param dev
  The index of RF device that needs to be initialized and can be
  between 0 and RFC_CDMA_MAX_VALID_RFM_DEVICES.

  @param rx_path
  The Rx path index for the device in question. For example if
  this is the 2nd Rx device [Diversity]. The rx_path value will
  be 1 [0 based count]. This can be between 0 and
  RFC_CDMA_MAX_VALID_RFM_DEVICES.

  @param tx_path
  The Tx path index for the device in question. For example if
  this is the 2nd Tx device [Say SV device]. The tx_path value
  will be 1 [0 based count]. This can be between 0 and
  RFC_CDMA_MAX_VALID_RFM_DEVICES.

  @param chip.
  This is the logical transceiver number. Each logical
  Tranceiver can have 2 devices. So this number increments every
  2 RFM_DEVICES. So for the 3rd RFM_DEVICE, this value will be
  1[0 based count]. Similarly for RFM_DEVICE 0 and 1, chip == 0.

  @param path.
  This is the path that decides how the VReg powers up the chip.
  The current design has each Vreg path power 2 RFM_Devices.

  @param bc_config
  The band-chan config NV for the device being initialized

  @return boolean
  TRUE:   Function successful.
  FALSE:  Function has failed.
*/
boolean
rfc_cdma_card_intf::rfc_cdma_populate_logical_device
(
   uint8              dev ,
   uint8&             rx_path ,
   uint8&             tx_path ,
   rfc_rf_chip_number chip ,
   rf_path_enum_type  path 
)
{
  rfc_cfg_params_type band_path_cfg ;
  boolean success = TRUE ;
  rfm_device_enum_type dev_index = GET_RFM_DEVICE(dev) ;

  /* For CDMA alternate path is 0 during bootup. i.e. there is no alt path.*/
  band_path_cfg.alternate_path = 0 ;
  band_path_cfg.logical_device = dev_index ;
  band_path_cfg.path = path ;
        band_path_cfg.req = RFC_REQ_DEFAULT_GET_DATA;
  dev_int_param_ptr[dev_index].vreg_path_num = path ;
  dev_int_param_ptr[dev_index].rf_chip_num   = chip ;

  if ( is_rfm_device_rx ( dev_index ) == TRUE )
  {
    /* Set Rx device */
    band_path_cfg.rx_tx = RFC_CONFIG_RX;
    dev_int_param_ptr[dev_index].rfc_rx_path = (rf_path_enum_type) rx_path ;
    rx_path ++ ;
    /* Populate Rx devices: e.g. ASM, Tuner, ELNA, etc */
    success &= populate_rx_devices ( dev_index ) ;
  }
  else
  {
    dev_int_param_ptr[dev_index].rfc_rx_path   = RF_PATH_MAX ;
  }

  if ( is_rfm_device_tx ( dev_index ) == TRUE )
  {
    /* Set Tx device */
    band_path_cfg.rx_tx = RFC_CONFIG_TX ;
    dev_int_param_ptr[dev_index].rfc_tx_path  = (rf_path_enum_type) tx_path ;
    tx_path ++ ;
    /* Populate Tx devices: e.g. WTR, PA, PAPM, ASM, etc */
    success &= populate_tx_devices ( dev_index ) ;
  }
  else
  {
    dev_int_param_ptr[dev_index].rfc_tx_path   = RF_PATH_MAX;
  }

  return success ;
}


/*----------------------------------------------------------------------------*/
/*!
  @brief
  Method called by constructor - needed this function to prevent
  code duplication in the different constructors

  @details
  Initializes members where applicable

  @retval TRUE on success, FALSE otherwise
*/
boolean
rfc_cdma_card_intf::rfc_cdma_construct_cdma_device ( )
{
  boolean success = TRUE;
  /* Band path configuration to be used for setting transceiver*/
  rfc_cfg_params_type band_path_cfg;

  /* For CDMA alternate path is 0 during bootup. i.e. there is no alt path.*/
  band_path_cfg.alternate_path = 0;
  uint8 max_dev = 0;
  
  success = rfc_cdma_allocate_mem_for_device();

  max_dev = rfc_cdma_get_rfm_max_valid_devices();

  /* populate members only if the max_dev check passes */
  if ( success == TRUE && max_dev <= RFM_MAX_WAN_DEVICES  )
    {
      /* intialize the data structures with initial values */
      for ( int rf_dev = RFM_DEVICE_0; rf_dev < max_dev ; rf_dev++ )
      {
      if ( logical_dev_param_ptr[rf_dev] != NULL )
      {
        rfc_cdma_init_logical_device(logical_dev_param_ptr[rf_dev]); 
      }

        success &= rfc_cdma_init_bc_info ( per_device_sub_bc_info_ptr , rf_dev ) ;
        success &= rfc_cdma_init_tx_lut ( tx_lut_type_to_use_ptr , rf_dev ) ;

        /* Set the current path to 0, also the default path. */
        rfc_cmda_set_current_rx_path_for_device ( rf_dev ) ;
      } /* iterate over valid devices */

    memset(valid_nv_path_array, 0, sizeof( valid_nv_path_array ));

      uint8 rx_path = 0 ;
      uint8 tx_path = 0 ;

      /* intialize the data structures with initial values */
      for ( uint8 rf_dev = RFM_DEVICE_0 ; rf_dev < RFM_MAX_WAN_DEVICES ; rf_dev++ )
      {
        if ( TRUE == ( rfc_cdma_is_rfm_device_used (rf_dev) ) )
      {
          switch ( rf_dev )
          {
            case RFM_DEVICE_0 :
            {
              success &=
                rfc_cdma_populate_logical_device ( rf_dev , rx_path , tx_path ,
                                                 RF_CHIP_0 , RF_PATH_0 ) ;
              break ;
            }

            case RFM_DEVICE_1 :
            {
              success &=
                rfc_cdma_populate_logical_device ( rf_dev , rx_path , tx_path ,
                                                 RF_CHIP_0 , RF_PATH_1 ) ;
              break ;
            }

            case RFM_DEVICE_2 :
            {
              success &=
                rfc_cdma_populate_logical_device ( rf_dev , rx_path , tx_path ,
                                                 RF_CHIP_1 , RF_PATH_2 ) ;
              break ;
            }

            case RFM_DEVICE_3 :
            {
            success &=
              rfc_cdma_populate_logical_device ( rf_dev , rx_path , tx_path ,
                                                 RF_CHIP_1 , RF_PATH_3 ) ;
            break ;
          }
          case RFM_DEVICE_4 :
          {
            success &=
              rfc_cdma_populate_logical_device ( rf_dev , rx_path , tx_path ,
                                                 RF_CHIP_0 , RF_PATH_4 ) ;
            break ;
          }
          case RFM_DEVICE_5 :
          {
            success &=
              rfc_cdma_populate_logical_device ( rf_dev , rx_path , tx_path ,
                                                 RF_CHIP_0 , RF_PATH_5 ) ;
            break ;
          }
          case RFM_DEVICE_6 :
          {
            success &=
              rfc_cdma_populate_logical_device ( rf_dev , rx_path , tx_path ,
                                                 RF_CHIP_0 , RF_PATH_6 ) ;
            break ;
          }
          case RFM_DEVICE_7 :
          {
            success &=
              rfc_cdma_populate_logical_device ( rf_dev , rx_path , tx_path ,
                                                 RF_CHIP_1 , RF_PATH_7 ) ;
            break ;
          }
          case RFM_DEVICE_8 :
          {
            success &=
              rfc_cdma_populate_logical_device ( rf_dev , rx_path , tx_path ,
                                                 RF_CHIP_0 , RF_PATH_8 ) ;
            break ;
          }
          case RFM_DEVICE_9 :
          {
              success &=
                rfc_cdma_populate_logical_device ( rf_dev , rx_path , tx_path ,
                                                 RF_CHIP_1 , RF_PATH_9 ) ;
              break ;
            }
            default:
            {
              success = FALSE ;
              RF_MSG_1 ( RF_ERROR , "rfc_cdma_construct_cdma_devcie(): "
                          "Dont know how to handle: %d " , rf_dev );
              break ;
            }
          } /* switch */
      }/* if ( TRUE == ( rfc_cdma_is_rfm_device_used (rf_dev) ) ) */
    }/* for ( uint8 rf_dev = RFM_DEVICE_0 ; rf_dev < RFM_MAX_WAN_DEVICES ...*/
  } /* if ( max_dev < RFC_CDMA_MAX_VALID_RFM_DEVICES  ) */
  else
  {
    /* if there is a valid case for
       max_dev > RFC_CDMA_MAX_VALID_RFM_DEVICES, then related
       changes need to be made in the "if-condition" above -
       for ex: if max_dev==6 is valid, then dev_int_param[] needs
       to be updated for case where max_dev=6
    */

    dev_int_param_ptr = NULL;
    per_device_sub_bc_info_ptr = NULL;
    tx_lut_type_to_use_ptr = NULL;

    success = FALSE;
    RF_MSG_2 ( RF_ERROR, "helper_for_rfc_cdma_card_intf: Skipping "
               "populating members since max valid devices = %d - cannot be "
               "more than %d", max_dev, RFM_MAX_WAN_DEVICES ) ;
  }

  if ( success == FALSE )
  {
    RF_MSG( RF_ERROR, "helper_for_rfc_cdma_card_intf: RF Card not created "
                      "properly !! ");
  }

  return success;

} /* helper_for_rfc_cdma_card_intf */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Helper function that checks if the GRFC init column for CDMA is empty. In DIME
  CDMA does not use init columns, so it will fail if it sees an not empty
  column. This API only gets called once during wakeup rx grfc script.

  @retval TRUE on empty init column, FALSE otherwise
*/
boolean
rfc_cdma_card_intf::is_init_column_empty
(
)
{
  boolean result = TRUE;
  rfc_cfg_params_type band_path_cfg;
  rfc_sig_cfg_type* init_signals = NULL;
  rfc_cdma_data *rfc_data = rfc_cdma_data::get_instance();

  if ( rfc_data == NULL )
  {
    RF_MSG( RF_ERROR, "rfc_cdma_card_intf::is_init_column_empty: "
                      "rfc_data ptr is null !!");
    result &= FALSE;
  }

  if( result == TRUE )
  {
    /* Get GRFC script for Rx init column, should be empty */
    band_path_cfg.alternate_path = 0;
    band_path_cfg.req = RFC_REQ_INIT;
    band_path_cfg.rx_tx = RFC_CONFIG_RX;

    rfc_data->sig_cfg_data_get(&band_path_cfg, &init_signals );

    if( init_signals != NULL)
    {
      if( init_signals[0].sig_name != (int)RFC_SIG_LIST_END )
      {
        RF_MSG( RF_ERROR, "rfc_cdma_card_intf::is_init_column_empty: "
                          "Rx init column not empty for CDMA");
        result &= FALSE;
      }
    }/* if( init_signals != NULL) */

    /* Get GRFC script for Tx init column, should be empty */
    band_path_cfg.rx_tx = RFC_CONFIG_TX;

    rfc_data->sig_cfg_data_get(&band_path_cfg, &init_signals );

    if( init_signals != NULL)
    {
      if( init_signals[0].sig_name != (int)RFC_SIG_LIST_END )
      {
        RF_MSG( RF_ERROR, "rfc_cdma_card_intf::is_init_column_empty: "
                          "Tx init column not empty for CDMA");
        result &= FALSE;
      }
    }/* if( init_signals != NULL) */

  } /* if( result == TRUE ) */

  return result;

} /* is_init_column_empty */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Constructor for base class rfc_cdma_card_intf

  @details
  Instantiates the base class and initializes members where applicable

  @param prep_time_in_us
  estimate of time that rfm_1x_prep_wakeup_rx() takes to execute

  @param exec_time_in_us
  estimate of time that rfm_1x_exec_wakeup_rx() takes to execute

  @param num_valid_rf_devices
  Num of valid paths that can do either RX/Tx or both

  @param reg_this
  Flag to indicate if the class is to be instantiated.

*/
rfc_cdma_card_intf::rfc_cdma_card_intf
(
  uint32 prep_time_in_us,
  uint32 exec_time_in_us,
  boolean *card_init_status
)
:rf_warmup_prep_time_in_us(prep_time_in_us),
 rf_warmup_exec_time_in_us(exec_time_in_us)
{

  /* Instantiate class */
  register_rfc(this);

  /* Construct and populate the various data structures in the class */
  *card_init_status = rfc_cdma_construct_cdma_device( );

} /* rfc_cdma_card_intf::rfc_cdma_card_intf */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  sets the correct bit-mask for sub-classes supported in member
  per_device_sub_bc_info_ptr

  @param device
  RFM device for which sub-class needs to be set

  @param rf_band
  RF band for which sub-class needs to be set
*/
boolean
rfc_cdma_card_intf::set_sub_bandclass_info
(
  rfm_device_enum_type device,
  rfm_cdma_band_class_type rf_band
)
{
  boolean result; /* API result */

  /* check for valid device */
  if ( FALSE == rfc_cdma_is_rfm_device_used (device) )
  {
    RF_MSG_1 ( RF_ERROR, "rfc_cdma_card_intf::set_sub_bandclass_info: "
                          "Invalid device %d", device );
    result = FALSE;
  }
  else if ( rf_band >= RFM_CDMA_MAX_BAND )
  {
    /* check for valid band */
    RF_MSG_1 ( RF_ERROR, "rfc_cdma_card_intf::set_sub_bandclass_info: "
                         "Invalid band %d", rf_band );
    result = FALSE;
  }
  else
  {
    switch ( rf_band )
    {
    case RFM_CDMA_BC0:
      per_device_sub_bc_info_ptr[device].sub_classes_supported[rf_band] =
                                          RF_SC_0 | RF_SC_1 | RF_SC_2 | RF_SC_3;
      break;

    case RFM_CDMA_BC2:
      per_device_sub_bc_info_ptr[device].sub_classes_supported[rf_band] =
                                                    RF_SC_0 | RF_SC_1 | RF_SC_2;
      break;

    case RFM_CDMA_BC5:
      per_device_sub_bc_info_ptr[device].sub_classes_supported[rf_band] =
      RF_SC_0 | RF_SC_1 | RF_SC_2 | RF_SC_3 | RF_SC_4 |RF_SC_5 | RF_SC_6 | RF_SC_7 | RF_SC_8 | RF_SC_9 | RF_SC_10 | RF_SC_11;
      break;

    case RFM_CDMA_BC10:
      per_device_sub_bc_info_ptr[device].sub_classes_supported[rf_band] =
                               RF_SC_0 | RF_SC_1 | RF_SC_2 | RF_SC_3 | RF_SC_4 ;
      break;

    case RFM_CDMA_BC11:
      per_device_sub_bc_info_ptr[device].sub_classes_supported[rf_band] =
                       RF_SC_0 | RF_SC_1 | RF_SC_2 | RF_SC_3 | RF_SC_4 | RF_SC_5 | RF_SC_8 | RF_SC_9 | RF_SC_10 | RF_SC_11 ;
      break;

    case RFM_CDMA_BC12:
      per_device_sub_bc_info_ptr[device].sub_classes_supported[rf_band] =
                                                              RF_SC_0 | RF_SC_1;
      break;

    default:
      break;

    } /* switch ( rf_band ) */

    result = TRUE;
  } /* valid input params */

  return result;

} /* rfc_cdma_card_intf::set_sub_bandclass_info */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  sets the correct bit-mask for sub-classes supported in member
  per_device_sub_bc_info_ptr

  @param device
  RFM device for which sub-class needs to be set

  @param rf_band
  RF band for which sub-class needs to be set
*/
boolean
rfc_cdma_card_intf::set_lut_type_for_band_pa_state
(
  rfm_device_enum_type device,
  rfm_cdma_band_class_type rf_band,
  rfm_pa_gain_state_type const pa_state,
  rfdevice_cdma_lut_type table_type
)
{
  boolean result; /* API result */

  /* check for valid device */
  if ( FALSE == rfc_cdma_is_rfm_device_used (device) )
  {
    RF_MSG_1 ( RF_ERROR, "rfc_cdma_card_intf::set_lut_type_for_band_pa_state: "
                          "Invalid device %d", device );
    result = FALSE;
  }
  else if ( rf_band >= RFM_CDMA_MAX_BAND )
  {
    /* check for valid band */
    RF_MSG_1 ( RF_ERROR, "rfc_cdma_card_intf::set_lut_type_for_band_pa_state: "
                         "Invalid band %d", rf_band );
    result = FALSE;
  }
  else if (pa_state >= RFM_PA_GAIN_STATE_NUM)
  {
    /* check for valid PA State */
    RF_MSG_1 (RF_ERROR, "rfc_cdma_card_intf::set_lut_type_for_band_pa_state: "
                        "Invalid PA State - %d", pa_state );
    result = FALSE;
  }
  else
  {
    tx_lut_type_to_use_ptr[device].tx_lut_type[rf_band][pa_state] = table_type;
  }

  result = TRUE;

  return result;

} /* rfc_cdma_card_intf::set_lut_type_for_band_pa_state */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Destructor for base class rfc_cdma_card_intf

*/
rfc_cdma_card_intf::~rfc_cdma_card_intf
(
)
{

  rfc_cdma_ptr = NULL;

  uint8 device;
  rfc_cdma_logical_device_params_type* logical_dev;
  uint8 rf_band;

  for ( device = 0; device < RFM_MAX_WAN_DEVICES ; device++ )
  {
    logical_dev = logical_dev_param_ptr[device];

    if ( ( logical_dev != NULL ) &&
         ( TRUE == rfc_cdma_is_rfm_device_used (device) ) )
    {
      /* Initialize logical device ASM for all bands */
      for ( rf_band = 0; rf_band < RFM_CDMA_MAX_BAND; rf_band++ )
      {
        if ( logical_dev->sm[rf_band] != NULL )
        {
          delete logical_dev->sm[rf_band];
        }

        if ( logical_dev->pa[rf_band] != NULL )
        {
          delete logical_dev->pa[rf_band];
        }
      }

      if( logical_dev_param_ptr[device] != NULL )
  {
    /* free up memory that was allocated in the constructor */
        modem_mem_free( logical_dev_param_ptr[device], MODEM_MEM_CLIENT_RFA );
      }

    }
  }

  if ( tx_lut_type_to_use_ptr != NULL )
  {
    modem_mem_free( tx_lut_type_to_use_ptr, MODEM_MEM_CLIENT_RFA );
  }

  if ( per_device_sub_bc_info_ptr != NULL )
  {
    modem_mem_free( per_device_sub_bc_info_ptr, MODEM_MEM_CLIENT_RFA );
  }

  if ( dev_int_param_ptr != NULL )
  {
    modem_mem_free( dev_int_param_ptr, MODEM_MEM_CLIENT_RFA );
  }

} /* rfc_cdma_card_intf::~rfc_cdma_card_intf */


/*----------------------------------------------------------------------------*/
/*!
  @brief
  C helper function to get all the supported CDMA band masks.

  @retval
  pointer to the RFC class
*/
uint64
rfc_cdma_card_helper_get_all_cdma_masks ( void )
{
  uint64 cdma_bc_mask = 0 ;
  for ( rfm_cdma_band_class_type band = RFM_CDMA_BC0 ;
        band < RFM_CDMA_MAX_BAND ;
        band = rfm_cdma_band_class_type(band + 1) )
  {
    cdma_bc_mask += rf_cdma_get_sys_band_mask ( band ) ;
  }
  return cdma_bc_mask ;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Method to find out if the NVs are enabled for CDMA card to
  be constructed. If CDMA BC config is set, TRUE is returned.

  @retval
  TRUE:  If any BC Config masks for CDMA is enabled.
  FALSE: If all CDMA bands over all paths\chains are disabled.
*/
boolean
rfc_cdma_card_intf::rfc_cdma_card_is_cdma_enabled_by_nv (  )
{
  rf_path_enum_type path = RF_PATH_0 ;
  bool is_cdma_supported_by_bc_config_nvs = FALSE ;
  uint64 all_cdma_masks = rfc_cdma_card_helper_get_all_cdma_masks() ;
  for ( ; path <= RF_CDMA_MAX_PATH_SUPPORTED ;
        path = rf_path_enum_type(path + 1) )
  {
    uint64 bc_config = rfcommon_nv_get_rf_bc_config( path ) ;
    if ( bc_config & all_cdma_masks )
    {
      /* Found atleast 1 CDMA band. */
      is_cdma_supported_by_bc_config_nvs = TRUE ;
    }
  }
  return is_cdma_supported_by_bc_config_nvs ;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Method that creates the RFC class

  @retval
  pointer to the RFC class
*/
rfc_cdma_card_intf * rfc_cdma_card_intf::create_device_instance
(
  uint32 prep_time_in_us,
  uint32 exec_time_in_us,
  boolean *card_init_status
)
{
  if (rfc_cdma_ptr == NULL )
  {
    /* Atleast 1 CDMA band is good. Go through the CDMA RFC creation. */
    if ( TRUE == rfc_cdma_card_is_cdma_enabled_by_nv() )
    {
    rfc_cdma_ptr = (rfc_cdma_card_intf *)new
                                       rfc_cdma_card_intf( prep_time_in_us,
                                                           exec_time_in_us,
                                                           card_init_status );
  }
    else
    {
      *card_init_status = TRUE ;
    }
  }

  return rfc_cdma_ptr;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Method that returns a pointer to the RFC class

  @retval
  pointer to the RFC class
*/
rfc_cdma_card_intf * rfc_cdma_card_intf::get_instance
(
  void
)
{
  return rfc_cdma_ptr;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Method that instantiates the RFC class

*/
void
rfc_cdma_card_intf::register_rfc
(
  rfc_cdma_card_intf *rfc_ptr
)
{
  rfc_cdma_ptr = rfc_ptr;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function generates the Rx Wakeup GRFCs script for the requested device
  and band.

  @param device
  RF device for which to generate GRFC script

  @param band
  Band for which to generate GRFC script

  @param grfc_buf_ptr
  Output - The GRFC script

  @return
  True on success and FALSE on failure
*/
boolean
rfc_cdma_card_intf::generate_rx_wakeup_grfc_script
(
  rfm_device_enum_type device,
  rfm_cdma_band_class_type band,
  rf_buffer_intf *grfc_buf_ptr,
  int16 delay
)
{
  boolean status = TRUE;
  rfc_cfg_params_type band_path_cfg;
  rfc_sig_cfg_type* mode_rf_signals = NULL;
  const rfc_cdma_logical_device_internal_params_type *dev_i_param;
  rfc_cdma_data *rfc_data = rfc_cdma_data::get_instance();

  if ( rfc_data == NULL )
  {
    RF_MSG( RF_ERROR, "rfc_cdma_card_intf::generate_rx_wakeup_grfc_script: "
                      "rfc_data ptr is null !!");
    return FALSE;
  }

  /* Get Device Parameter */
  dev_i_param = get_logical_dev_internal_param(device);
  if( dev_i_param == NULL )
  {
    RF_MSG( RF_ERROR, "rfc_cdma_card_intf::generate_rx_wakeup_grfc_script: "
                      "could not get valid device internal pointer ");
    return FALSE;
  }
  /* Query for Band/Path specific signals. Use req = RFC_REQ_DEFAULT_GET_DATA*/
  /* For CDMA alternate path is 0 because GRFC isn't supported for now */
  band_path_cfg.alternate_path = 0;
  band_path_cfg.band = band;
  band_path_cfg.logical_device = device;
  band_path_cfg.rx_tx = RFC_CONFIG_RX;
  band_path_cfg.req = RFC_REQ_DEFAULT_GET_DATA;

  rfc_data->sig_cfg_data_get(&band_path_cfg, &mode_rf_signals );

  /* Generate GRFC script for signals obtained*/
  status &= rfc_common_generate_grfc_script( mode_rf_signals,
                                             grfc_buf_ptr,
                                             delay );

  /* Check for init column in RFC rx/tx table. It should be empty */
  status &= is_init_column_empty();

  return status;
} /* rfc_cdma_card_intf::generate_rx_wakeup_grfc_script */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function generates the Rx Sleep GRFCs script for the requested device
  and band.

  @param device
  RF device for which to generate GRFC script

  @param band
  Band for which to generate GRFC script

  @param grfc_buf_ptr
  Output - The GRFC script

  @param is_irat
  Indicates if the API is being called during IRAT operation

  @return
  True on success and FALSE on failure
*/
boolean
rfc_cdma_card_intf::generate_rx_sleep_grfc_script
(
  rfm_device_enum_type device,
  rfm_cdma_band_class_type band,
  rf_buffer_intf *grfc_buf_ptr,
  int16 delay
)
{
  boolean status = TRUE;
  rfc_cfg_params_type band_path_cfg;
  rfc_sig_cfg_type* mode_rf_signals = NULL;
  const rfc_cdma_logical_device_internal_params_type *dev_i_param;
  rfc_cdma_data *rfc_data = rfc_cdma_data::get_instance();

  if ( rfc_data == NULL )
  {
    RF_MSG( RF_ERROR, "rfc_cdma_card_intf::generate_rx_grfc_script: "
                      "rfc_data ptr is null !!");
    return FALSE;
  }

  /* Get Device Parameter */
  dev_i_param = get_logical_dev_internal_param(device);
  if( dev_i_param == NULL )
  {
    RF_MSG( RF_ERROR, "rfc_cdma_card_intf::generate_rx_grfc_script: "
                      "could not get valid device internal pointer ");
    return FALSE;
  }

  /* Query for Band/Path specific signals. Use req = RFC_REQ_DEFAULT_GET_DATA*/
  band_path_cfg.alternate_path = 0;
  band_path_cfg.logical_device = device;
  band_path_cfg.band = band;
  band_path_cfg.path = dev_i_param->rfc_rx_path;
  band_path_cfg.rx_tx = RFC_CONFIG_RX;
  band_path_cfg.req = RFC_REQ_DEFAULT_GET_DATA;

  rfc_data->sig_cfg_data_get(&band_path_cfg, &mode_rf_signals );

  /* Return GRFC script with signal logic at init state*/
  status &= rfc_common_generate_init_grfc_script( mode_rf_signals,
                                                  grfc_buf_ptr,
                                                  delay );

  return status;
} /* rfc_cdma_card_intf::generate_rx_sleep_grfc_script */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function generates the Tx Wakeup GRFCs script for the requested device
  and band.

  @param device
  RF device for which to generate GRFC script

  @param band
  Band for which to generate GRFC script

  @param grfc_buf_ptr
  Output - The GRFC script

  @param is_irat
  Indicates if the API is being called during IRAT operation

  @return
  True on success and FALSE on failure
*/
boolean
rfc_cdma_card_intf::generate_tx_wakeup_grfc_script
(
  rfm_device_enum_type device,
  rfm_cdma_band_class_type band,
  rf_buffer_intf *grfc_buf_ptr,
  int16 delay
)
{
  boolean status = TRUE;
  rfc_cfg_params_type band_path_cfg;
  rfc_sig_cfg_type* mode_rf_signals = NULL;
  const rfc_cdma_logical_device_internal_params_type *dev_i_param;
  rfc_cdma_data *rfc_data = rfc_cdma_data::get_instance();

  if ( rfc_data == NULL )
  {
    RF_MSG( RF_ERROR, "rfc_cdma_card_intf::generate_tx_wakeup_grfc_script: "
                      "rfc_data ptr is null !!");
    return FALSE;
  }

  /* Get Device Parameter */
  dev_i_param = get_logical_dev_internal_param(device);
  if ( dev_i_param == NULL )
  {
    RF_MSG( RF_ERROR, "rfc_cdma_card_intf::generate_tx_wakeup_grfc_script: "
                      "could not get valid device internal pointer ");
    return FALSE;
  }

  /* Query for Band/Path specific signals. Use req = RFC_REQ_DEFAULT_GET_DATA*/
  band_path_cfg.alternate_path = 0;
  band_path_cfg.logical_device = device;
  band_path_cfg.band = band;
  band_path_cfg.path = dev_i_param->rfc_tx_path;
  band_path_cfg.rx_tx = RFC_CONFIG_TX;
  band_path_cfg.req = RFC_REQ_DEFAULT_GET_DATA;

  rfc_data->sig_cfg_data_get(&band_path_cfg, &mode_rf_signals );

  /* Generate GRFC script for signals obtained*/
  status &= rfc_common_generate_grfc_script( mode_rf_signals,
                                             grfc_buf_ptr,
                                             delay );

  return status;
} /* rfc_cdma_card_intf::generate_tx_grfc_script */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function generates the Tx Sleep GRFC script for the requested device
  and band.

  @param device
  RF device for which to generate GRFC script

  @param band
  Band for which to generate GRFC script

  @param grfc_buf_ptr
  Output - The GRFC script

  @param is_irat
  Indicates if the API is being called during IRAT operation

  @return
  True on success and FALSE on failure
*/
boolean
rfc_cdma_card_intf::generate_tx_sleep_grfc_script
(
  rfm_device_enum_type device,
  rfm_cdma_band_class_type band,
  rf_buffer_intf *grfc_buf_ptr,
  int16 delay
)
{
  boolean status = TRUE;
  rfc_cfg_params_type band_path_cfg;
  rfc_sig_cfg_type* mode_rf_signals;
  const rfc_cdma_logical_device_internal_params_type *dev_i_param;
  rfc_cdma_data *rfc_data = rfc_cdma_data::get_instance();

  if ( rfc_data == NULL )
  {
    RF_MSG( RF_ERROR, "rfc_cdma_card_intf::generate_tx_sleep_grfc_script: "
                      "rfc_data ptr is null !!");
    return FALSE;
  }

  /* Get Device Parameter */
  dev_i_param = get_logical_dev_internal_param(device);
  if ( dev_i_param == NULL )
  {
    RF_MSG( RF_ERROR, "rfc_cdma_card_intf::generate_tx_sleep_grfc_script: "
                      "could not get valid device internal pointer ");
    return FALSE;
  }

  /* Query for Band/Path specific signals. Use req = RFC_REQ_DEFAULT_GET_DATA*/
  band_path_cfg.alternate_path = 0;
  band_path_cfg.logical_device = device;
  band_path_cfg.band = band;
  band_path_cfg.path = dev_i_param->rfc_tx_path;
  band_path_cfg.rx_tx = RFC_CONFIG_TX;
  band_path_cfg.req = RFC_REQ_DEFAULT_GET_DATA;

  rfc_data->sig_cfg_data_get(&band_path_cfg, &mode_rf_signals );

  /* Return GRFC script with signal logic at init state*/
  status &= rfc_common_generate_init_grfc_script( mode_rf_signals,
                                                  grfc_buf_ptr,
                                                  delay );

  return status;
} /* rfc_cdma_card_intf::generate_tx_sleep_grfc_script */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function generates the TX ON/OFF GRFCs script for the requested device
  and band.

  @param device
  RF device for which to generate GRFC script

  @param band
  Band for which to generate GRFC script

  @param tx_on
  True for Tx On. False for Tx Off

  @param grfc_buf_ptr
  Output - The GRFC script

  @return
  True on success and FALSE on failure
*/
boolean
rfc_cdma_card_intf::generate_tx_on_off_grfc_script
(
  rfm_device_enum_type device,
  rfm_cdma_band_class_type band,
  boolean tx_on,
  rf_buffer_intf *grfc_buf_ptr,
  int16 delay,
  int32 *num_grfc
)
{
  rfc_cfg_params_type band_path_cfg;
  rfc_sig_cfg_type* mode_rf_signals;
  const rfc_cdma_logical_device_internal_params_type *dev_i_param;
  rfc_cdma_data *rfc_data = rfc_cdma_data::get_instance();
  int32 num_sigs = 0;

  if ( rfc_data == NULL )
  {
    RF_MSG( RF_ERROR, "rfc_cdma_card_intf::generate_tx_on_off_grfc_script: "
                      "rfc_data ptr is null !!");
    return FALSE;
  }

  /* Get Device Parameter */
  dev_i_param = get_logical_dev_internal_param(device);
  if ( dev_i_param == NULL )
  {
    RF_MSG( RF_ERROR, "rfc_cdma_card_intf::generate_tx_on_off_grfc_script: "
                      "could not get valid device internal pointer ");
    return FALSE;
  }

  /* Query for Band/Path specific signals. Use req = RFC_REQ_DEFAULT_GET_DATA*/
  band_path_cfg.alternate_path = 0;
  band_path_cfg.logical_device = device;
  band_path_cfg.band = band;
  band_path_cfg.path = dev_i_param->rfc_tx_path;
  band_path_cfg.rx_tx = RFC_CONFIG_TX;
  band_path_cfg.req = RFC_REQ_DEFAULT_GET_DATA;

  /* RFC_TX_ON_AG is currently used for RF_ON. This signal may subject
     to change */
  rfc_data->sig_cfg_data_get(&band_path_cfg, &mode_rf_signals );

  /* Populate Tx On/Off setting */
  if( tx_on == TRUE )
  {
    *num_grfc = rfc_common_get_grfc_overwritten_buffer( mode_rf_signals,
                                                       RFC_TX_ON_AG, RFC_HIGH,
                                                       grfc_buf_ptr, delay );
  }
  else
  {
    *num_grfc = rfc_common_get_grfc_overwritten_buffer( mode_rf_signals,
                                                       RFC_TX_ON_AG, RFC_LOW,
                                                       grfc_buf_ptr, delay );
  }

  if ( *num_grfc == 0 )
  {
    RF_MSG( RF_HIGH, "rfc_cdma_card_intf::generate_tx_on_off_grfc_script: "
                      "No TX ON/OFF signal available ");
  }

  return TRUE;

} /* rfc_cdma_card_intf::generate_tx_on_off_grfc_script */


/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function generates the TX ON/OFF GRFCs script for the requested device
  and band.

  @param device
  RF device for which to generate GRFC script

  @param band
  Band for which to generate GRFC script

  @param pa_on
  True for Tx On. False for Tx Off

  @param grfc_buf_ptr
  Output - The GRFC script

  @return
  True on success and FALSE on failure
*/
boolean
rfc_cdma_card_intf::generate_pa_on_off_grfc_script
(
  rfm_device_enum_type device,
  rfm_cdma_band_class_type band,
  boolean pa_on,
  rf_buffer_intf *grfc_buf_ptr,
  int16 delay
)
{
  rfc_cfg_params_type band_path_cfg;
  rfc_sig_cfg_type* mode_rf_signals;
  const rfc_cdma_logical_device_internal_params_type *dev_i_param;
  int32 num_grfc;
  rfc_cdma_data *rfc_data = rfc_cdma_data::get_instance();

  if ( rfc_data == NULL )
  {
    RF_MSG( RF_ERROR, "rfc_cdma_card_intf::generate_pa_on_off_grfc_script: "
                      "rfc_data ptr is null !!");
    return FALSE;
  }

  /* Get Device Parameter */
  dev_i_param = get_logical_dev_internal_param(device);
  if ( dev_i_param == NULL )
  {
    RF_MSG( RF_ERROR, "rfc_cdma_card_intf::generate_pa_on_off_grfc_script: "
                      "could not get valid device internal pointer ");
    return FALSE;
  }

  /* Query for Band/Path specific signals. Use req = RFC_REQ_DEFAULT_GET_DATA*/
  band_path_cfg.alternate_path = 0;
  band_path_cfg.logical_device = device;
  band_path_cfg.band = band;
  band_path_cfg.path = dev_i_param->rfc_tx_path;
  band_path_cfg.rx_tx = RFC_CONFIG_TX;
  band_path_cfg.req = RFC_REQ_DEFAULT_GET_DATA;

  num_grfc = 0;

  /* RFC_PA_ON is currently used for PA ON. */
  rfc_data->sig_cfg_data_get(&band_path_cfg, &mode_rf_signals );

  /* Populate PA On/Off setting */
  if( pa_on == TRUE )
  {
  num_grfc = rfc_common_get_grfc_overwritten_buffer( mode_rf_signals,
                                                       RFC_PA_ON, RFC_HIGH,
                                                       grfc_buf_ptr, delay );
  }
  else
  {
    num_grfc = rfc_common_get_grfc_overwritten_buffer( mode_rf_signals,
                                                       RFC_PA_ON, RFC_LOW,
                                                       grfc_buf_ptr, delay );
  }
  if ( num_grfc == 0 )
  {
    RF_MSG( RF_HIGH, "rfc_cdma_card_intf::generate_pa_on_off_grfc_script: "
                     "No PA ON/OFF signal available ");
  }

  return TRUE;

} /* rfc_cdma_card_intf::generate_pa_on_off_grfc_script */

#ifdef FEATURE_RF_ASDIV
/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function generates the AsDiv GRFCs script for the requested device
  and band.

  @param position
  RF device for which to generate GRFC script

  @param cb_data
  Call back data to pass in additional info for extendability

  @param asdiv_script_ptr
  Output - The GRFC script

  @return
  True on success and FALSE on failure
*/
boolean
rfc_cdma_card_intf::generate_asdiv_xsw_script
(
  rfcommon_asdiv_position_type position,
  rf_buffer_intf *asdiv_script_ptr,
  void *cb_data
)
{
  boolean status = TRUE;
  rfc_cfg_params_type band_path_cfg;
  rfc_sig_cfg_type *rf_asd_signals;
  rfc_device_info_type* device_info_ptr = NULL;
  rf_cdma_asdiv_xsw_script_data *xsw_script_data;
  rfc_cdma_data *rfc_data = rfc_cdma_data::get_instance();
  rfc_intf *rfc_cmn = rfc_intf::get_instance();
  /*Device type used to extract tech specific object*/
  rfdevice_type_enum_type cmn_dev_type = RFDEVICE_TYPE_INVALID;
  /*Device instance used to extract tech specific object*/
  uint32 instance = 0;
  /* Pointer to device class to hold common object */
  rfdevice_class *device_obj = NULL;
  boolean is_scripted = TRUE ;
  uint8 ctr = 0;
  rfdevice_xsw_config_type xsw_config;
  rfcom_band_type_u rf_band;

  if ( rfc_data == NULL )
  {
    RF_MSG( RF_ERROR, "rfc_cdma_card_intf::generate_asdiv_xsw_script: "
                      "rfc_data ptr is null !!");
    return FALSE;
  }

  /* Get the ASM script data */
  xsw_script_data = (rf_cdma_asdiv_xsw_script_data*)cb_data;

  /* NULL pointer check */
  if ( xsw_script_data == NULL )
  {
    RF_MSG( RF_ERROR, "rfc_cdma_card_intf::generate_asdiv_xsw_script:"
                      "NULL callback data" );
    return FALSE;
  } /* if ( asm_script_data == NULL ) */

  /* Populate params
    xsw_script_data->immediate param from RF dev driver tells if we use
    scripted or immediate writes 
    If xsw_script_data->immediate is TRUE then switch happens immediate else scripted*/
  is_scripted = xsw_script_data->immediate?FALSE:TRUE;

  band_path_cfg.alternate_path = 0;
  band_path_cfg.logical_device = xsw_script_data->device; /* Not required*/
  band_path_cfg.band = xsw_script_data->band;
  band_path_cfg.rx_tx = xsw_script_data->rxtx;

  xsw_config.band.cdma_band = xsw_script_data->band;
  xsw_config.mode = RFM_1X_MODE;

  if ( position )
  {
    band_path_cfg.req = RFC_REQ_ENABLE_ANT_SWAP_1;
    xsw_config.xsmode = RFDEVICE_XSW_PRX_DRX_PATH_SWAP;
  }
  else
  {
    band_path_cfg.req = RFC_REQ_DISABLE_ANT_SWAP_1;
    xsw_config.xsmode = RFDEVICE_XSW_PRX_DRX_PATH_DEFAULT;
  }

  RF_MSG_1( RF_MED, "rfc_cdma_card_intf::generate_asdiv_grfc_script: "
                     "position %d", position);

  /* update XSW port to use (if needed) */
  status = configure_asm_xsw_port( xsw_script_data->device, xsw_script_data->band );

   /* Get device info pointer */
   rfc_data->devices_cfg_data_get(&band_path_cfg, &device_info_ptr );

   if ( device_info_ptr != NULL )
   {
     /* loop through the device info ptr to find the index of each device object*/
     while (ctr < device_info_ptr->num_devices)
     {
       /*Get device type and device instance to retrieve device object*/
       cmn_dev_type = device_info_ptr->rf_asic_info[ctr].device_type;
       switch (cmn_dev_type)
       {
         case RFDEVICE_XSW:
           instance = device_info_ptr->rf_asic_info[ctr].instance;
           device_obj = rfc_cmn->get_cmn_rf_device_object(RFDEVICE_XSW,(uint8)instance);
           if( device_obj != NULL )
           {
             xsw_config.trigger = RFDEVICE_XSW_TRIGGER_WAKEUP; 
             status = rfdevice_xsw_wakeup(&xsw_config,(void*)device_obj,asdiv_script_ptr,(rf_device_execution_type)is_scripted,0);   
             xsw_config.trigger = RFDEVICE_XSW_TRIGGER_NONE; 
             status = rfdevice_xsw_set_cross_switch_config(&xsw_config,(void*)device_obj,asdiv_script_ptr,(rf_device_execution_type)is_scripted,0);   
             xsw_config.trigger =  RFDEVICE_XSW_TRIGGER_ALL;
             status = rfdevice_xsw_trigger(&xsw_config,(void*)device_obj,asdiv_script_ptr,(rf_device_execution_type)is_scripted,0);  
             if ( status == FALSE )
             {
              RF_MSG( RF_HIGH, "rfc_cdma_card_intf::generate_asdiv_xsw_script: "
                                 "XSW device build script failed! ");
             }
           }/* if( device_obj != NULL ) */
           else
           {
             RF_MSG( RF_HIGH, "rfc_cdma_card_intf::generate_asdiv_xsw_script: "
                               "XSW device object is NULL ");
           }/* if( device_obj == NULL ) */
          break;

          default:
            break;
         }
         ctr++;
     }/*while (ctr < device_info_ptr->num_devices)*/

   }/* if ( device_info_ptr != NULL ) */
   else
   {
     RF_MSG( RF_HIGH, "rfc_cdma_card_intf::generate_asdiv_xsw_script: "
                         "XSW device object not present in RFC ");
   }/* if ( device_info_ptr == NULL ) */

  /* Generate GRFC XSW scripts */
  rfc_data->sig_cfg_data_get(&band_path_cfg, &rf_asd_signals);

  /* CHeck for NULL ptr */
  if ( rf_asd_signals != NULL )
  {
  /* Create Script */
  if ( is_scripted == TRUE )
  {
    status &= rfc_common_generate_grfc_script( rf_asd_signals,
                                               asdiv_script_ptr,
                                               0 );
    if ( status == FALSE )
    {
      RF_MSG_1( RF_ERROR, "rfc_cdma_card_intf::generate_asdiv_xsw_script: "
                          "SwitchPos %d - Script Gen failed", position);

    }/* if ( status == FALSE ) */
  } /* if ( immediate == FALSE ) */
  else
  {
    /* Perform Immediate execution */
    status &= rfc_common_config_signal( rf_asd_signals, TRUE /* gnss_blanking_enabled */ );
    if ( status == FALSE )
    {
      RF_MSG_1( RF_ERROR, "rfc_cdma_card_intf::generate_asdiv_xsw_script: "
                          "SwitchPos %d - IMM failed", position);
    }/* if ( status == FALSE ) */
  } /* if ! ( immediate == FALSE ) */
  }
  else
  {
    RF_MSG( RF_HIGH, "rfc_cdma_card_intf::generate_asdiv_xsw_script: "
                        "No GRFC XSW support");
  }

  return status;

} /* rfc_cdma_card_intf::generate_asdiv_grfc_script */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function updates the XSW port in the ASM device containing the XSW based on
  device and band.

  @param device
  RF device for which to update XSW port

  @param band
  Band to update XSW port for

  @return
  True on success and FALSE on failure
*/
boolean
rfc_cdma_card_intf::configure_asm_xsw_port 
( 
  rfm_device_enum_type device,
  rfm_cdma_band_class_type band
)
{
  boolean status = TRUE;
  rfc_cfg_params_type band_path_cfg;
  rfc_device_info_type* device_info_ptr = NULL;
  rfc_cdma_data *rfc_data = rfc_cdma_data::get_instance();
  rfc_intf *rfc_cmn = rfc_intf::get_instance();
  /*Device type used to extract tech specific object*/
  rfdevice_type_enum_type cmn_dev_type = RFDEVICE_TYPE_INVALID;
  /*Device instance used to extract tech specific object*/
  uint32 instance = 0;
  /* Pointer to device class to hold common object */
  rfdevice_class *device_obj = NULL;
  uint8 ctr = 0;
  rfcom_band_type_u rf_band;

  if ( rfc_data == NULL )
  {
    RF_MSG( RF_ERROR, "rfc_cdma_card_intf::generate_asdiv_grfc_script: "
                      "rfc_data ptr is null !!");
    return FALSE;
  }

  /* Populate params */
  band_path_cfg.alternate_path = 0;
  band_path_cfg.logical_device = device;
  band_path_cfg.band           = band;
  band_path_cfg.rx_tx          = RFC_CONFIG_RX;
  band_path_cfg.req            = RFC_REQ_DISABLE_ANT_SWAP_1; /* functionality is not position dependent */

  rf_band.cdma_band = band;

  /* Get device info pointer */
  rfc_data->devices_cfg_data_get(&band_path_cfg, &device_info_ptr );

  if ( device_info_ptr != NULL )
  {
   /* loop through the device info ptr to find the index of each device object*/
   while (ctr < device_info_ptr->num_devices)
   {
     /*Get device type and device instance to retrieve device object*/
     cmn_dev_type = device_info_ptr->rf_asic_info[ctr].device_type;

     switch (cmn_dev_type) 
     {           
       case RFDEVICE_ASM:
         instance = device_info_ptr->rf_asic_info[ctr].instance;
         device_obj = rfc_cmn->get_cmn_rf_device_object(RFDEVICE_ASM,(uint8)instance);

         if( device_obj != NULL )
         {
           status = ((rfdevice_asm *)device_obj)->set_asm_xsw_port_config( device, RFM_1X_MODE, rf_band );
         }
         else
         {
           status = FALSE;
         }

         break;

        default:
         break;    
     } 
     ctr++;
   }/*while (ctr < device_info_ptr->num_devices)*/

  }/* if ( device_info_ptr != NULL ) */

  return status;

}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function generates the AsDiv RFFE XSW script for the requested device
  and band.

  @param position
  RF device for which to generate GRFC script

  @param cb_data
  Call back data to pass in additional info for extendability

  @param asdiv_script_ptr
  Output - The GRFC script

  @return
  True on success and FALSE on failure
*/
boolean
rfc_cdma_card_intf::generate_xsw_sleep_script
(
  rfcommon_asdiv_position_type position,
  rf_buffer_intf *asdiv_script_ptr,
  void *cb_data
)
{
  boolean status = TRUE;
  rfc_cfg_params_type band_path_cfg;
  rfc_sig_cfg_type *rf_asd_signals;
  rfc_device_info_type* device_info_ptr = NULL;
  rf_cdma_asdiv_xsw_script_data *xsw_script_data;
  rfc_cdma_data *rfc_data = rfc_cdma_data::get_instance();
  rfc_intf *rfc_cmn = rfc_intf::get_instance();
  /*Device type used to extract tech specific object*/
  rfdevice_type_enum_type cmn_dev_type = RFDEVICE_TYPE_INVALID;
  /*Device instance used to extract tech specific object*/
  uint32 instance = 0;
  /* Pointer to device class to hold common object */
  rfdevice_class *device_obj = NULL;
  boolean is_scripted = TRUE ;
  uint8 ctr = 0;
  rfdevice_xsw_config_type xsw_config;
  rfcom_band_type_u rf_band;
  
  /* Get the XSW script data */
  xsw_script_data = (rf_cdma_asdiv_xsw_script_data*)cb_data;

  /* NULL pointer check */
  if ( xsw_script_data == NULL )
  {
    RF_MSG( RF_ERROR, "rfc_cdma_card_intf::generate_asdiv_xsw_script:"
                      "NULL callback data" );
    return FALSE;
  } /* if ( xsw_script_data == NULL ) */

  xsw_config.band.cdma_band = xsw_script_data->band;
  xsw_config.mode = RFM_1X_MODE;
  
  if ( rfc_data == NULL )
  {
    RF_MSG( RF_ERROR, "rfc_cdma_card_intf::generate_asdiv_xsw_script: "
                      "rfc_data ptr is null !!");
    return FALSE;
  }

  /* Populate params
    xsw_script_data->immediate param from RF dev driver tells if we use
    scripted or immediate writes 
    If xsw_script_data->immediate is TRUE then switch happens immediate else scripted*/
  is_scripted = xsw_script_data->immediate?FALSE:TRUE;

  band_path_cfg.alternate_path = 0;
  band_path_cfg.logical_device = xsw_script_data->device; /* Not required*/
  band_path_cfg.band = xsw_script_data->band;
  band_path_cfg.rx_tx = xsw_script_data->rxtx;

  if ( position )
  {
    band_path_cfg.req = RFC_REQ_ENABLE_ANT_SWAP_1;
    xsw_config.xsmode = RFDEVICE_XSW_PRX_DRX_PATH_SWAP;
  }
  else
  {
    band_path_cfg.req = RFC_REQ_DISABLE_ANT_SWAP_1;
    xsw_config.xsmode = RFDEVICE_XSW_PRX_DRX_PATH_DEFAULT;
  }

  RF_MSG_1( RF_MED, "rfc_cdma_card_intf::generate_asdiv_grfc_script: "
                     "position %d", position);

  /* update XSW port to use (if needed) */
  status = configure_asm_xsw_port( xsw_script_data->device, xsw_script_data->band );

  /* Get device info pointer */
  rfc_data->devices_cfg_data_get(&band_path_cfg, &device_info_ptr );	

  if ( device_info_ptr != NULL )
  {
    /* loop through the device info ptr to find the index of each device object*/
    while (ctr < device_info_ptr->num_devices)
    {
      /*Get device type and device instance to retrieve device object*/
      cmn_dev_type = device_info_ptr->rf_asic_info[ctr].device_type;
      switch (cmn_dev_type)
     {
        case RFDEVICE_XSW:
          instance = device_info_ptr->rf_asic_info[ctr].instance;
          device_obj = rfc_cmn->get_cmn_rf_device_object(RFDEVICE_XSW,(uint8)instance);
          if( device_obj != NULL )
          {
            xsw_config.trigger = RFDEVICE_XSW_TRIGGER_SLEEP; 
            status = rfdevice_xsw_sleep(&xsw_config,(void*)device_obj,asdiv_script_ptr,(rf_device_execution_type)is_scripted,0);
            if ( status == FALSE )
            {
             RF_MSG( RF_HIGH, "rfc_cdma_card_intf::generate_xsw_sleep_script: "
                                "XSW device build script failed! ");
            }
          }/* if( device_obj != NULL ) */
          else
          {
            RF_MSG( RF_HIGH, "rfc_cdma_card_intf::generate_xsw_sleep_script: "
                              "XSW device object is NULL ");
          }/* if( device_obj == NULL ) */
          break;
          default:
          break;
     }
         ctr++;
    }/*while (ctr < device_info_ptr->num_devices)*/
  }
  else
  {
    RF_MSG( RF_HIGH, "rfc_cdma_card_intf::generate_asdiv_xsw_script: "
                        "XSW device object not present in RFC ");
  }/* if ( device_info_ptr == NULL ) */

  return status;
}
#endif /* FEATURE_RF_ASDIV */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function returns the GRFC number for a specific signal type based on
  device and band information.

  @details
  Based on the RFC signal type provided, this API queries the RFC for the
  list of signals related to that signal type (Rx/Tx/Init). Common RFC
  functionality is then used to map the RFC signal back to the modem
  GRFC information.

  @param device
  RF device for which GRFC information is requested

  @param band
  Band for which GRFC information is requested

  @param sig_type
  Indicates which RFC signal type is being reverse mapped to the modem GRFC

  @return
  The GRFC number pertaining to the signal type requested
*/
int32
rfc_cdma_card_intf::get_grfc_engine_num
(
  rfm_device_enum_type device,
  rfm_cdma_band_class_type band,
  rfc_signal_type sig_type
)
{
  int32 grfc_engine_num = -1;
  rfc_cfg_params_type band_path_cfg;
  rfc_sig_cfg_type* mode_rf_signals;
  const rfc_cdma_logical_device_internal_params_type *dev_i_param;
  rfc_cdma_data *rfc_data = rfc_cdma_data::get_instance();

  if ( rfc_data == NULL )
  {
    RF_MSG( RF_ERROR, "rfc_cdma_card_intf::get_grfc_engine_num : "
                      "rfc_data ptr is null !!");
    return grfc_engine_num;
  }

  /* Get Device Parameter */
  dev_i_param = get_logical_dev_internal_param(device);
  if ( dev_i_param == NULL )
  {
    RF_MSG( RF_ERROR, "rfc_cdma_card_intf::get_grfc_engine_num : "
                      "Device ptr NULL ");
    return grfc_engine_num;
  }

  band_path_cfg.alternate_path = 0;

  switch ( sig_type )
  {
    case RFC_PA_ON:
    case RFC_TX_ON_AG:
    case RFC_PDM:
    case RFC_TX_GTR_THRESH:
    case RFC_PA_RANGE0:
    case RFC_PA_RANGE1:
      band_path_cfg.band = band;
      band_path_cfg.path = dev_i_param->rfc_tx_path;
      band_path_cfg.rx_tx = RFC_CONFIG_TX;
      band_path_cfg.logical_device = device;

      /* Obtain the RF Signal information for mode */
      band_path_cfg.req = RFC_REQ_DEFAULT_GET_DATA; /* set to FALSE since we need to access
                                          the band-specific-table */
      rfc_data->sig_cfg_data_get(&band_path_cfg, &mode_rf_signals );
        grfc_engine_num =
          rfc_common_get_grfc_engine_num( mode_rf_signals, sig_type );

      /* If the signal wasn't found in the band-specific list, check the
      init list */
      if ( grfc_engine_num == -1 )
      {
        band_path_cfg.req = RFC_REQ_INIT; /* set to TRUE since we need to access
                                           the init-table */
        rfc_data->sig_cfg_data_get(&band_path_cfg, &mode_rf_signals );

          grfc_engine_num =
          rfc_common_get_grfc_engine_num( mode_rf_signals, sig_type );
      }
      break;

    default:
      RF_MSG(RF_ERROR, "rfc_cdma_card_intf::get_grfc_engine_num: "
                       "Unsupported RFC Signal Type");
      break;
  }

  if (grfc_engine_num == -1)
  {
    RF_MSG(RF_HIGH, "rfc_cdma_card_intf::get_grfc_engine_num: "
                    "RFC Signal GRFC Info not found");
  }

  return grfc_engine_num;
} /* rfc_cdma_card_intf::get_grfc_engine_num */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Query the RF card to get the RF Warmup (RF WU)time needed for the SW and HW
  to be configured

  @details
  The current implementation assumes that the RF WU times for all devices is
  the same

  @param device
  The RF device to be configured

  @return
  The RF Warmup time in micro-secs

*/
int32
rfc_cdma_card_intf::get_warmup_time
(
  rfm_device_enum_type device
)
{
  return ( rf_warmup_prep_time_in_us + rf_warmup_exec_time_in_us );
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Query the RF card to get the RF Warmup time needed for the SW and HW to be
  configured

  @param device
  RFM Device on which RF Warmup time is queried

  @param rf_mode
  RFM Mode for which RF warmup time is queried

  @param rf_wu_data
  Pointer to the Structure containing the Rx warmup time breakup. Calling
  function will be responsible to pass a valid pointer to a variable as
  RF will not be allocating memory.

  @return
  Flag indicating the validity of Warmup time

  @retval TRUE
  Flag to indicate that the warmup time time filled in the container is valid

  @retval FALSE
  Flag to indicate that the warmup time time filled in the container is
  not valid
*/
boolean
rfc_cdma_card_intf::get_warmup_time
(
  rfm_device_enum_type device,
  rfm_mode_enum_type rf_mode,
  void * const rf_wu_data
)
{
  boolean ret_val = FALSE;

  /* Return RF WU time based on RF Code */
  if ( rf_mode == RFM_1X_MODE )
  {
    rfm_1x_rx_warmup_time_type *onex_wu =
                           (rfm_1x_rx_warmup_time_type*)rf_wu_data;

    /* NULL Pointer check */
    if ( onex_wu != NULL )
    {
      onex_wu->prep_time_us = rf_warmup_prep_time_in_us;
      onex_wu->exec_time_us = rf_warmup_exec_time_in_us;
      ret_val = TRUE;
    }
    else
    {
      RF_MSG_2( RF_ERROR, "rfc_cdma_card_intf:get_warmup_time - Invalid "
                          "Container : Dev %d, RF Mode %d", device, rf_mode );
      ret_val = FALSE;
    }
  }
  else /* Unsupported mode */
  {
    RF_MSG_2( RF_ERROR, "rfc_cdma_card_intf:get_warmup_time - Invalid Mode : "
                        "Dev %d, RF Mode %d", device, rf_mode );
    ret_val = FALSE;
  }

  return ret_val;

} /* rfc_cdma_card_intf::get_warmup_time */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Retrieve the LUT table name (enum) that for a given band and PA state

  @param mode
  RF mode for which table name is to be retrieved - this parm is not used
  currently, but may be used in future - for instance, if LUT to be used is
  different for 1x and HDR, for the same band

  @param device
  RF device on which operation is to be performed

  @param band
  RF Band based on which LUT table name is to be returned

  @param pa_state
  PA state based on which LUT table name is to be returned

  @param table_name_ptr
  Pointer to variable that holds the table_name

  @return
  TRUE on success, and NULL on failure

*/
boolean
rfc_cdma_card_intf::get_tx_lut_table_name
(
  rfm_mode_enum_type const mode,
  rfm_device_enum_type const device,
  rfm_cdma_band_class_type const band,
  rfm_pa_gain_state_type const pa_state,
  int* table_name_ptr,
  rfdevice_cdma_lut_type* table_type
)
{
  boolean res; /* API result */
  rfc_cdma_logical_device_params_type* logic_dev;

  /* Ensure device is valid for this RFC */
  if ( FALSE == rfc_cdma_is_rfm_device_used (device) )
  {
    RF_MSG_1 ( RF_ERROR, "rfc_cdma_card_intf: get_tx_lut_table_name: "
                         "Invalid device %d", device );
    return FALSE;
  }

  /* check if device is a valid one - i.e, if the internal data structures have
     been initialized for the given device */
  if ( ( logic_dev = get_logical_dev_param(device) ) == NULL )
  {
    return FALSE;
  }

  /* check for valid band */
  if (band >= RFM_CDMA_MAX_BAND)
  {
    RF_MSG_1 ( RF_ERROR, "rfc_cdma_card_intf::get_tx_lut_table_name: "
                         "Invalid band - %d", band );
    return FALSE;
  }

  /* check for valid PA State */
  if (pa_state >= RFM_PA_GAIN_STATE_NUM)
  {
    RF_MSG_1 (RF_ERROR, "rfc_cdma_card_intf::get_tx_lut_table_name: "
                        "Invalid PA State - %d", pa_state );
    return FALSE;
  }

  /* check if Tx is valid for this device */
  if ( logic_dev->tx_device[band] == NULL )
  {
     RF_MSG_1 ( RF_ERROR, "rfc_cdma_card_intf::get_tx_lut_table_name: "
                          "Tx NOT supported for this Device - %d", device );
     return FALSE;
  }

  /*----------- all checks done.... proceed to return a valid value ----------*/

  rfc_cfg_params_type band_path_cfg;
  rfc_device_info_type* device_info_ptr = NULL;
  rfc_cdma_data *rfc_data = rfc_cdma_data::get_instance();

  if ( rfc_data == NULL )
  {
    RF_MSG( RF_ERROR, "rfc_cdma_card_intf::get_tx_lut_table_name: "
                      "rfc_data ptr is null !!");
    return FALSE;
  }

  /* Accessor function to obtain the RF Signal information for
    requested band path configuration */
  band_path_cfg.alternate_path = 0;
  band_path_cfg.logical_device = device;
  band_path_cfg.path = dev_int_param_ptr[device].rfc_tx_path;
  band_path_cfg.rx_tx = RFC_CONFIG_TX;
  /* set to FALSE since we need to access the band-specific-table */
  band_path_cfg.req = RFC_REQ_DEFAULT_GET_DATA;

  band_path_cfg.band = band;
  rfc_data->devices_cfg_data_get(&band_path_cfg, &device_info_ptr );

  /* check if Autogen lookup-passed */
  if ( device_info_ptr != NULL )
  {
    // Dime RFC Changes: need to figure-out how to access the port information.
    // **** temporary fix ****
    // assuming rf_asic_info[0] is WTR and data[1] is port info
    // final fix needs device and RFC changes
    *table_name_ptr = device_info_ptr->rf_asic_info[0].data[1];
    *table_type = tx_lut_type_to_use_ptr[device].tx_lut_type[band][pa_state];
    res = TRUE;
  }
  else
  {
    RF_MSG_4 ( RF_ERROR,
               "rfc_cdma_card_intf::get_tx_lut_table_name: Autogen lookup "
               "failed for Mode %d, device %d, Band %d, Pa_State %d",
               mode, device, band, pa_state );
    res = FALSE;
  }

  return res;

} /* rfc_cdma_card_intf::get_tx_lut_table_name */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get Logical Device Internal Parameter

  @details
  This function will return all of the platform-dependent parameters to
  configure the proper hardware devices. The parameters will be used internal to
  RFC only.

  @param device
  RF Device for which the logical parameter are queried

  @return
  Pointer to the Logical Device internal parameter structure
*/
rfc_cdma_logical_device_internal_params_type*
rfc_cdma_card_intf::get_logical_dev_internal_param
(
  rfm_device_enum_type device
)
{
  /* Perform check on device */
  if ( TRUE == rfc_cdma_is_rfm_device_used (device) )
  {
    /* Pointer to Device Param */
    return &dev_int_param_ptr[device];
  }
  else
  {
    /* Invalid Pointer */
    RF_MSG_1( RF_ERROR, "rfc_cdma_card_intf::get_logical_dev_internal_param: "
              "failed due to bad device = %d", device );
    return NULL;
  }

} /* rfc_cdma_card_intf::get_logical_dev_internal_param */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get RF chip being used for the device (needed by VREG operations)

  @details
  This function returns the RF chip to be used for VREG operations

  @param device
  RF Device for which the logical parameter are queried

  @return
  Valid RF chip number on success and RF_CHIP_MAX on failure
*/
rfc_rf_chip_number
rfc_cdma_card_intf::get_rf_chip_num
(
  rfm_device_enum_type device
)
{
  /* Perform check on device */
  if ( ( TRUE == rfc_cdma_is_rfm_device_used (device) ) )
  {
    /* Pointer to Device Param */
    return dev_int_param_ptr[device].rf_chip_num;
  }
  else
  {
    /* Invalid Pointer */
    RF_MSG_1( RF_ERROR, "rfc_cdma_card_intf::get_rf_chip_num: "
              "failed due to bad device = %d", device );
    return RF_CHIP_MAX;
  }

} /* rfc_cdma_card_intf::get_rf_chip_num */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get path that is needed by VREG

  @details
  This function returns the path to be used for VREG operations

  @param device
  RF Device for which the logical parameter are queried

  @return
  Valid path on success and RF_PATH_MAX on failure
*/
rf_path_enum_type
rfc_cdma_card_intf::get_vreg_path
(
  rfm_device_enum_type device
)
{
  /* Perform check on device */
  if ( ( TRUE == rfc_cdma_is_rfm_device_used (device) ) )
  {
    /* Pointer to Device Param */
    return dev_int_param_ptr[device].vreg_path_num;
  }
  else
  {
    /* Invalid Pointer */
    RF_MSG_1( RF_ERROR, "rfc_cdma_card_intf::get_vreg_path: "
              "failed due to bad device = %d", device );
    return RF_PATH_MAX;
  }

} /* rfc_cdma_card_intf::get_vreg_path */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get Logical Device Parameter

  @details
  This function will return all of the platform-dependent parameters to
  configure the proper hardware devices.

  @param device
  RF Device for which the logical parameter are queried

  @return
  Pointer to the Logical Device parameter structure
*/
rfc_cdma_logical_device_params_type*
rfc_cdma_card_intf::get_logical_dev_param
(
  rfm_device_enum_type device
)
{
  /* Check for valid device*/
  if ( ( TRUE == rfc_cdma_is_rfm_device_used (device) ) )
  {
    return logical_dev_param_ptr[device];
  }
  else
  {
    return NULL;
  }
} /* rfc_cdma_card_intf::get_logical_dev_param */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Returns sub-classes supported by the RF card for a given band

  @param device
  the RF device that needs to be queried

  @param band
  RF band for which sub-bc info is needed

  @return
  32-bit info that holds info about sub-classes supported

*/
uint32
rfc_cdma_card_intf::get_sub_class_for_band
(
  rfm_device_enum_type device,
  rfm_cdma_band_class_type rf_band
)
{
  uint32 result; /* API result */

  /* check for valid device */
  if ( ( FALSE == rfc_cdma_is_rfm_device_used (device) ) )
  {
    RF_MSG_1 ( RF_ERROR, "rfc_cdma_card_intf::get_sub_class_for_band: "
                          "Invalid device %d", device );
    result = 0;
  }
  else if ( rf_band >= RFM_CDMA_MAX_BAND )
  {
    /* check for valid band */
    RF_MSG_1 ( RF_ERROR, "rfc_cdma_card_intf::get_sub_class_for_band: "
                         "Invalid band %d", rf_band );
    result = 0;
  }
  else
  {
    result = ( boolean )
              per_device_sub_bc_info_ptr[device].sub_classes_supported[rf_band];
  }

  return result;

} /* rfc_cdma_card_intf::get_sub_class_for_band */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Returns whether the specified device is tunable or not for a given band

  @param device the RF device that needs to be tuned

  @param band RF band to be checked for tuning

  @retval TRUE if the device can be tuned to the givenband, FALSE otherwise

*/
boolean
rfc_cdma_card_intf::is_device_tuneable
(
  rfm_device_enum_type device,
  rfm_cdma_band_class_type rf_band
)
{
  boolean result; /* API result */
  rfc_cdma_logical_device_internal_params_type* dev_params;

  /* check for valid device */
  if ( FALSE == rfc_cdma_is_rfm_device_used (device) )
  {
    RF_MSG_1 ( RF_ERROR, "rfc_cdma_card_intf::is_device_tuneable: "
                          "Invalid device %d", device );
    result = FALSE;
  }
  else if ( rf_band >= RFM_CDMA_MAX_BAND )
  {
    /* check for valid band */
    RF_MSG_1 ( RF_ERROR, "rfc_cdma_card_intf::is_device_tuneable: "
                         "Invalid band %d", rf_band );
    result = FALSE;
  }
  else if ( ( dev_params = get_logical_dev_internal_param(device) ) == NULL )
  {
    RF_MSG_1 ( RF_ERROR, "rfc_cdma_card_intf::is_device_tuneable: failed to "
               "get valid internal device params for device %d ", device);
    result = FALSE;
  }
  else
  {
    /* params need to do a query on Autogen*/
    rfc_cfg_params_type band_path_cfg;
    rfc_sig_cfg_type* band_path_rf_signals = NULL;
    rfc_cdma_data *rfc_data = rfc_cdma_data::get_instance();

    if ( rfc_data == NULL )
    {
      RF_MSG( RF_ERROR, "rfc_cdma_card_intf::is_device_tuneable: "
                        "rfc_data ptr is null !!");
      return FALSE;
    }

    /* populate band_path_cfg with required info */
    band_path_cfg.alternate_path = 0;
    /* Returns 1 for Tx*/
    if ( is_rfm_device_tx( device ) == TRUE )
    {
      band_path_cfg.rx_tx = RFC_CONFIG_TX ;
    }
    else
    {
      band_path_cfg.rx_tx = RFC_CONFIG_RX ;
    }
    band_path_cfg.logical_device = device;

    /* set to FALSE since we need to access the band-specific-table */
    band_path_cfg.req = RFC_REQ_DEFAULT_GET_DATA;
    band_path_cfg.path = dev_params->rfc_rx_path;
    band_path_cfg.band = rf_band;

    /* Accessor function to obtain check if band is supported */
    result = rfc_data->sig_cfg_data_get(&band_path_cfg, &band_path_rf_signals );
  }

  return result;

} /* rfc_cdma_card_intf::is_device_tuneable */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Configures the XSW for the given card. Since the configuration
  is not needed for every device the functions returns if the
  configuration is retried for the enxt device.

  @param device
  The RF device is stored and checked against so we dont
  configure the XSW twice.

  @param band
  RF band for XSW port configuration

  @retval boolean
  TRUE if the XSW config is successful for a given band,
  FALSE otherwise

*/
boolean
rfc_cdma_card_intf::rfc_cdma_set_xsw_band_ports
(
  rfm_device_enum_type   device ,
  rfcom_band_type_u&       band_u ,
  rfc_cfg_params_type&   band_path_cfg_orig
)
{
  rfm_device_enum_type first_dev = device ;
  rfc_device_info_type*   device_info_ptr_en  = NULL ;
  rfc_device_info_type*   device_info_ptr_dis = NULL ;
  boolean success = TRUE ;
    rfc_cdma_data* rfc_data = rfc_cdma_data::get_instance() ;
  uint32 instance = 0 ;
  rfdevice_class* device_obj = NULL ;
  rfc_intf* rfc_i = rfc_intf::get_instance() ;

  rfdevice_xsw_config_type xsw_cfg; 
  xsw_cfg.mode = RFM_INVALID_MODE ; 

  if ( first_dev == device )
  {
    rfc_cfg_params_type bad_path_cfg_local ;
    memscpy ( &bad_path_cfg_local , sizeof(rfc_cfg_params_type) ,
              &band_path_cfg_orig , sizeof(rfc_cfg_params_type) ) ;

    bad_path_cfg_local.alternate_path = 0 ;
    bad_path_cfg_local.req = RFC_REQ_ENABLE_ANT_SWAP_1;
    rfc_data->devices_cfg_data_get ( &bad_path_cfg_local , &device_info_ptr_en ) ;


    bad_path_cfg_local.req = RFC_REQ_DISABLE_ANT_SWAP_1;
    rfc_data->devices_cfg_data_get ( &bad_path_cfg_local, &device_info_ptr_dis ) ;

    rfdevice_type_enum_type cmn_dev_type = RFDEVICE_TYPE_INVALID ;

    if ( ( device_info_ptr_en != NULL ) && ( device_info_ptr_dis != NULL ) )
    {
      uint32 dev_ctr = 0 ;
      while ( dev_ctr < device_info_ptr_en->num_devices )
      {
        cmn_dev_type = device_info_ptr_en->rf_asic_info[dev_ctr].device_type ;
        if ( RFDEVICE_XSW == cmn_dev_type )
        {

          instance = device_info_ptr_en->rf_asic_info[dev_ctr].instance ;
          device_obj =
            rfc_i->get_cmn_rf_device_object ( RFDEVICE_XSW , (uint8)instance ) ;

          if ( device_obj != NULL )
          {
            /* Update the XSW port map by invoking the API below */
            xsw_cfg.mode = RFCOM_1X_MODE;
            xsw_cfg.band = band_u;
            success = rfdevice_xsw_set_band_map_swap(    
                                               &xsw_cfg,
                                               device_obj,
                                               device_info_ptr_en->rf_asic_info[dev_ctr].data,
                                               RFC_ASIC_INFO_DATA_SIZE
                                             ) ;
          }
        }
        dev_ctr++;

        if ( success == FALSE )
        {

          RF_MSG_2 ( RF_FATAL ,
                    "set_band_map_swap()Failure||Dev:%d|Band:%d||" ,
                     device , band_u.cdma_band ) ;
        }
      } /* while ( dev_ctr < device_info_ptr->num_devices) */


      dev_ctr = 0 ;
      while ( dev_ctr < device_info_ptr_dis->num_devices )
      {

        cmn_dev_type = device_info_ptr_dis->rf_asic_info[dev_ctr].device_type ;
        if ( RFDEVICE_XSW == cmn_dev_type )
        {
          instance = device_info_ptr_dis->rf_asic_info[dev_ctr].instance ;
          device_obj = rfc_i->get_cmn_rf_device_object (
                              RFDEVICE_XSW , (uint8)instance ) ;

          if ( device_obj != NULL )
          {
            /* Update the XSW port map by invoking the API below */
            xsw_cfg.mode = RFCOM_1X_MODE;
            xsw_cfg.band = band_u;
            success = rfdevice_xsw_set_band_map_default(   
                                               &xsw_cfg,
                                               device_obj,
                            device_info_ptr_dis->rf_asic_info[dev_ctr].data ,
                                                      RFC_ASIC_INFO_DATA_SIZE
                                                    );
          }
        }
        dev_ctr++;

        if ( success == FALSE )
        {
          RF_MSG_2 ( RF_FATAL ,
                    "set_band_map_default()Failure||Dev:%d|Band:%d||" ,
                     device , band_u.cdma_band ) ;
        }
      } /* while ( dev_ctr < device_info_ptr->num_devices) */
    } /* if ( ( device_info_ptr_en != NULL ) && ( device_info_ptr_dis != NULL ) ) */
    else
    {
      success = FALSE ;
    }
  } /* if ( firt_dev == device ) */

  return success ;
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  Given a RFC device column, acquire Tx WTR device, initialize its properties,
  and store it to rfc_cdma_logical_dev[rf_band]
*/
boolean
rfc_cdma_card_intf::populate_tx_transceiver
(
  rfc_cdma_logical_device_params_type* logical_dev_ptr,
  rfm_cdma_band_class_type rf_band,
  rfc_device_info_type *device_info_ptr, 
  int16 device_index,
  rfm_device_enum_type device
)
{
  boolean api_status = TRUE;
  rfc_intf *rfc_cmn = rfc_intf::get_instance();
  rfc_cdma_data *rfc_data = rfc_cdma_data::get_instance();    
 
  rfdevice_class *trx_common_device = NULL; /* Pointer to device class to hold common object */
  rfdevice_trx_cdma_tx *tx_device = NULL;  
  rfdevice_cdma_lut_type cdma_tx_pa_lut[RFDEVICE_MAX_PA_STATES];/* Lookup table type for PA states */
  uint32 phy_path;
  uint8 rfm_device=0;

  uint8 instance;
  rfcom_band_type_u band_u;
  band_u.cdma_band = rf_band;
    
  if (rfc_data == NULL)
  {
    RF_MSG(RF_ERROR," set_cdma_tx_device: RFC CDMA Data Object is NOT"
                    " created ");
    api_status = FALSE;
  }
  else /*Proceed only if rfc_data obj was created succesfully*/
  {
    /*Get device type and device instance to retrieve device object*/              
    instance = (uint8)device_info_ptr->rf_asic_info[device_index].instance;
    phy_path = device_info_ptr->rf_asic_info[device_index].phy_path_num;

    /*Retrieve device object*/
    trx_common_device = rfc_cmn->get_cmn_rf_device_object(RFDEVICE_TRANSCEIVER, instance);

    if( trx_common_device != NULL )
    {
      /* extract the cdma device object from the cmn_device object and store it */
      tx_device =
        (rfdevice_trx_cdma_tx*)\
        (((rfdevice_rxtx_common_class *)trx_common_device)->get_tech_instance(RFM_1X_MODE,
                                                                        RFDEVICE_TX,
                                                                        (rfdevice_trx_phy_path_enum_type)phy_path));
      rfdevice_cdma_tx_set_band_data( tx_device, rf_band,
                                      device_info_ptr->rf_asic_info[device_index].data,
                                      RFC_ASIC_INFO_DATA_SIZE);

      /* Populate the array containing LUT info*/
      rfdevice_cdma_tx_get_lut_data( tx_device, rf_band, 
                                     device_info_ptr->rf_asic_info[device_index].data,
                                     RFC_ASIC_INFO_DATA_SIZE, 
                                     cdma_tx_pa_lut);

      /* Update LUT table types */
      set_lut_type_for_band_pa_state( device, rf_band, RFM_PA_GAIN_STATE_0, 
                                      cdma_tx_pa_lut[RFM_PA_GAIN_STATE_0]);

      set_lut_type_for_band_pa_state( device, rf_band, RFM_PA_GAIN_STATE_1,
                                      cdma_tx_pa_lut[RFM_PA_GAIN_STATE_1]);

      set_lut_type_for_band_pa_state( device, rf_band, RFM_PA_GAIN_STATE_2,
                                      cdma_tx_pa_lut[RFM_PA_GAIN_STATE_2]);

      set_lut_type_for_band_pa_state( device, rf_band, RFM_PA_GAIN_STATE_3, 
                                      cdma_tx_pa_lut[RFM_PA_GAIN_STATE_3]);

      /* set Tx device */
      logical_dev_ptr->tx_device[rf_band] = tx_device ;
    } /* if( cmn_dev_obj != NULL ) */
  }/* rfc_data != NULL */

#if (defined (FEATURE_THOR_MODEM))
  if ( api_status == TRUE )
  {
    rfc_cfg_params_type band_path_cfg;

    /*!FBRx loop*/
    for(rfm_device=0; rfm_device < RFM_MAX_DEVICES; rfm_device++)
    {
      band_path_cfg.logical_device = (rfm_device_enum_type)rfm_device;
      band_path_cfg.rx_tx = RFC_CONFIG_TX;
      band_path_cfg.req = RFC_REQ_ENABLE_FBRX;

      for (rf_band = RFM_CDMA_BC0; rf_band < RFM_CDMA_MAX_BAND; rf_band++ )
      {
         band_path_cfg.band = rf_band;
         if(rfc_data->devices_cfg_data_get(&band_path_cfg, &device_info_ptr)== TRUE)
         {
           if(!rfc_set_fbrx_mapping(       
                                      &band_path_cfg, 
                                      device_info_ptr
                                   )
             )
           {
                RF_MSG_3( RF_ERROR, " rfc_set_fbrx_mapping() failed for rfm device %d. "
                                    "May not be supported for this tech %d band %d. check rfc",
                                    rfm_device,RFM_1X_MODE,rf_band);
           } 
         }
      }
    }
  }
#endif
  /*Throw error message if result is FALSE*/
  if ( api_status == FALSE )
  {
    RF_MSG_1 ( RF_ERROR , "populate_tx_transceiver: API failed for device: %d ",
                          device ) ;
  }

  return api_status;
} /* rfc_cdma_card_intf::populate_tx_transceiver */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Given a RFC device column, acquire Rx WTR device, initialize its properties,
  and store it to rfc_cdma_logical_dev[rf_band]
*/
boolean
rfc_cdma_card_intf::populate_rx_transceiver
(
  rfc_cdma_logical_device_params_type* logical_dev_ptr,
  rfm_cdma_band_class_type rf_band,
  rfc_device_info_type *device_info_ptr, 
  int16 device_index,
  uint8 tranceiver_count,
  rfm_device_enum_type device
)
{
  rfc_cdma_data *rfc_data = rfc_cdma_data::get_instance();
  rfc_intf *rfc_cmn = rfc_intf::get_instance();

  /*Pointers to rx and tx devices*/
  rfdevice_trx_cdma_rx *rx_device = NULL;

  /* Pointer to device class to hold common object */
  rfdevice_class *trx_common_device = NULL;

  /*API status*/
  boolean api_status = TRUE;

  /* Used to loop thru all devices for a band */
  uint32 phy_path;
  uint8 instance;
  rfcom_band_type_u band;

  if (rfc_data == NULL)
  {
    RF_MSG(RF_ERROR," populate_rx_transceiver: RFC CDMA Data Object is NOT"
                    " created ");
    api_status = FALSE;
  }
  else /*Proceed only if rfc_data obj was created succesfully*/
  {
    instance = (uint8)device_info_ptr->rf_asic_info[device_index].instance;
    phy_path = device_info_ptr->rf_asic_info[device_index].phy_path_num;

    /*Retrieve device object*/
    trx_common_device = rfc_cmn->get_cmn_rf_device_object(RFDEVICE_TRANSCEIVER, instance);

    if( trx_common_device != NULL )
  {
      /* extract the cdma device object from the cmn_device object and store it */
        rx_device =
        (rfdevice_trx_cdma_rx*)\
      (((rfdevice_rxtx_common_class *)trx_common_device)->get_tech_instance(RFM_1X_MODE,
                                                                      RFDEVICE_RX,
                                                                      (rfdevice_trx_phy_path_enum_type)phy_path));

      rfdevice_cdma_rx_set_band_data( rx_device, rf_band,
                                      device_info_ptr->rf_asic_info[device_index].data,
                                      RFC_ASIC_INFO_DATA_SIZE);
      band.cdma_band = (rfm_cdma_band_class_type)rf_band;

      /*call the below api to capture the transciever->phy path->modem chain(ADC) mapping to perform adc dc cal.*/
      if(!rfc_cmn->set_trx_phy_path_adc_mapping(
                                    device_info_ptr->modem_chain,                               /*modem chain to map the ADC*/
                                    device_info_ptr->rf_asic_info[device_index].phy_path_num,   /*physical path*/
                                    device_info_ptr->rf_asic_info[device_index].instance,       /*Transciever device type instance*/
                                    RFM_1X_MODE,                                                /*tech*/
                                    band,                                                       /*band*/
                                    (rfdevice_class*)rx_device,                                 /*rx device pointer*/
                                    device_info_ptr->rf_asic_info[device_index].data,           /*port info*/
                                    RFC_ASIC_INFO_DATA_SIZE                                     /*data size*/
                                    )
      )
    {
        RF_MSG_3( RF_ERROR, "rfc_cmn->set_trx_phy_path_adc_mapping() failed. check rfc"
                            "modem_chain=%d,phy_path_num=%d, trx logical instance=%d  ",
                            device_info_ptr->modem_chain, 
                            device_info_ptr->rf_asic_info[device_index].phy_path_num,
                            device_info_ptr->rf_asic_info[device_index].instance
        );
        api_status = FALSE;
    }


      if ( tranceiver_count == 1 )
    {
        /* Only 1 device - no wrapper needed */
        logical_dev_ptr->rx_device[rf_band] = rx_device ;
      }
      else if ( tranceiver_count == 2 )
      {
        /* Band uses 2 receive devices. Need wrapper object */
        logical_dev_ptr->rx_device[rf_band] = new rfc_cdma_device_wrapper( 
                                          logical_dev_ptr->rx_device[rf_band],
                                          rx_device );

        }
        else
        {
        RF_MSG_3( RF_ERROR, "populate_rx_transceiver: Invalid number of"
                            "transceiver devices %d of %d devices for "
                            "band %d",
                            tranceiver_count,
                            device_info_ptr->num_devices,
                            rf_band );

        api_status = FALSE;
      }      
    }
    else
          {
      RF_MSG_2( RF_ERROR, "populate_rx_transceiver. trx_cmn_device NULL"
                          "for dev_type %d, instance %d",
                          RFDEVICE_TRANSCEIVER, instance );
      api_status = FALSE;
          }

    set_sub_bandclass_info( device, rf_band);

  }

  /*Throw error message if result is FALSE*/
  if ( api_status == FALSE )
             {
    RF_MSG_1 ( RF_ERROR , "set_cdma_rx_device: API failed for device: %d ",
               device) ;
                 }

  return api_status;

} /* rfc_cdma_card_intf::populate_rx_transceiver */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Given a RFC device column, acquire Tx ASM device, initialize its properties,
  and store it to rfc_cdma_logical_dev[rf_band].
*/
boolean
rfc_cdma_card_intf::populate_tx_asm
(
 rfc_cdma_logical_device_params_type* logical_dev_ptr,
 rfm_cdma_band_class_type rf_band,
 rfc_device_info_type *device_info_ptr, 
 int16 device_index,
 rfm_device_enum_type device
)
{ 
  rfc_intf *rfc_cmn = rfc_intf::get_instance();
  rfdevice_class *device_obj = NULL;
  rfcom_band_type_u band_u;
  uint8 instance;
  boolean result = TRUE;

  band_u.cdma_band= rf_band;
                               
  if ( logical_dev_ptr->sm[rf_band] == NULL  )
                  {
    logical_dev_ptr->sm[rf_band] = new rf_cdma_asm( device );
                  }

  /*Retrieve device object*/
  instance = (uint8)device_info_ptr->rf_asic_info[device_index].instance;
  device_obj = rfc_cmn->get_cmn_rf_device_object(RFDEVICE_ASM, instance);

  /* Set the Tx band map for this ASM device for selected band */
  /* Also pass the size of the array where data is being stored */
  result &= rfdevice_asm_set_tx_band_map(
             (void*)device_obj,
             device,
             RFM_1X_MODE,
             band_u,
             device_info_ptr->rf_asic_info[device_index].data,
             RFC_ASIC_INFO_DATA_SIZE );

  /* Add the ASM device to the wrapper ASM object */
  result &= logical_dev_ptr->sm[rf_band]->add_device_instance((void*)device_obj, 
                                                               TX_ASM );
  return result;
}/* populate_tx_asm */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Given a RFC device column, acquire Rx ASM device, initialize its properties,
  and store it to rfc_cdma_logical_dev[rf_band]
*/
boolean
rfc_cdma_card_intf::populate_rx_asm
(
 rfc_cdma_logical_device_params_type* logical_dev_ptr,
 rfm_cdma_band_class_type rf_band,
 rfc_device_info_type *device_info_ptr, 
 int16 device_index,
 rfm_device_enum_type device
)
{
  boolean result = TRUE; 
  rfc_intf *rfc_cmn = rfc_intf::get_instance();
  rfdevice_class *device_obj = NULL;
  rfcom_band_type_u band_u;
  uint8 instance;

  band_u.cdma_band= rf_band;
                                
  if ( logical_dev_ptr->sm[rf_band] == NULL  )
                   {
    logical_dev_ptr->sm[rf_band] = new rf_cdma_asm( device );
                   }

  /*Retrieve device object*/
  instance = (uint8)device_info_ptr->rf_asic_info[device_index].instance;
  device_obj = rfc_cmn->get_cmn_rf_device_object(RFDEVICE_ASM, instance);

  /* Set the Rx band map for this ASM device for selected band */
  /* Also pass the size of the array where data is being stored */
  result &= rfdevice_asm_set_rx_band_map(
             (void*)device_obj,
             device,
             RFM_1X_MODE,
             band_u,
             device_info_ptr->rf_asic_info[device_index].data,
             RFC_ASIC_INFO_DATA_SIZE );

  /* Add the ASM device to the wrapper ASM object */
  result &= logical_dev_ptr->sm[rf_band]->add_device_instance((void*)device_obj, 
                                                               RX_ASM );
  return result;
}/* populate_rx_asm */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Given a RFC device column, acquire PAPM device, initialize its properties,
  and store it to rfc_cdma_logical_dev[rf_band]
*/
boolean
rfc_cdma_card_intf::populate_pa
(
 rfc_cdma_logical_device_params_type* logical_dev_ptr,
 rfm_cdma_band_class_type rf_band,
 rfc_device_info_type *device_info_ptr, 
 int16 device_index,
 rfm_device_enum_type device,
 rfc_cfg_params_type* band_path_cfg
)
{ 
  rfc_intf *rfc_cmn = rfc_intf::get_instance();
  rfc_cdma_data *rfc_data = rfc_cdma_data::get_instance();
  rfdevice_class *device_obj = NULL;
  uint8 instance;
  rfcom_band_type_u band_u;

  band_u.cdma_band = rf_band;

               if ( logical_dev_ptr->pa[rf_band] == NULL )
               {
    logical_dev_ptr->pa[rf_band] = new cdma_pa( device ) ;
  }

  if (device_index == -1)
               {
    /* GRFC PA has no objects in RFC */
                 rfc_sig_cfg_type* mode_rf_signals = NULL;
                 device_obj = new grfc_pa();
    rfc_data->sig_cfg_data_get(band_path_cfg, &mode_rf_signals );
    /* Set PA Band Port Mapping */
    if ( device_obj != NULL )
                 {
                   ((rfdevice_pa *)device_obj)->set_band_map ( RFM_1X_MODE,
                                                              band_u,
                                                              (int32*) mode_rf_signals,
                                                              0 /* Don't care */ );
    logical_dev_ptr->pa[rf_band]->set_misc_device_descriptor ((void*)device_obj ) ;
                 }
   }/*if( device_index == -1 )*/
                 else
                 {
     /* RFFE PA*/    
     instance = device_info_ptr->rf_asic_info[device_index].instance;
                  device_obj = rfc_cmn->get_cmn_rf_device_object(RFDEVICE_PA,
                                                    instance);
                  if ( device_obj != NULL )
                  {
                    /* Set PA Band Port Mapping */
                    ((rfdevice_pa *)device_obj)->set_band_map (
                               RFM_1X_MODE,
                               band_u,
                            device_info_ptr->rf_asic_info[device_index].data,
                               RFC_ASIC_INFO_DATA_SIZE);

       logical_dev_ptr->pa[rf_band]->set_misc_device_descriptor ((void*)device_obj ) ;
     }
                 else
                 {
       RF_MSG_1(RF_ERROR,"Unexpected NULL PA object for band",rf_band);
     }
   }/*if( device_index != -1 )*/

  return TRUE;
}/* populate_pa */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Given a RFC device column, acquire Tx WTR device, initialize its properties,
  and store it to rfc_cdma_logical_dev[rf_band]
*/
boolean
rfc_cdma_card_intf::populate_papm
(
 rfc_cdma_logical_device_params_type* logical_dev_ptr,
 rfm_cdma_band_class_type rf_band,
 rfc_device_info_type *device_info_ptr, 
 int16 device_index,
 rfm_device_enum_type device
)
{ 
  rfc_intf *rfc_cmn = rfc_intf::get_instance();
  rfdevice_class *device_obj = NULL;
  uint8 instance;
  rfcom_band_type_u band_u;

  band_u.cdma_band= rf_band;

                 /*Retrieve device object*/
  instance = (uint8)device_info_ptr->rf_asic_info[device_index].instance;
  device_obj = rfc_cmn->get_cmn_rf_device_object(RFDEVICE_PAPM, instance);

                 if( device_obj != NULL )
                 {
    /* Set PAPM Band Port Mapping */
                   ((rfdevice_papm *)device_obj)->set_band_map( device,
                                                                RFM_1X_MODE,
                                                                band_u,
                                                 device_info_ptr->rf_asic_info[device_index].data,
                                                                RFC_ASIC_INFO_DATA_SIZE);

                   logical_dev_ptr->qpoet_obj[rf_band] = (void *)device_obj;
                 }/* if ( device_obj != NULL)*/

  return TRUE;
}/* populate_papm */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Given a RFC device column, acquire hdet device, initialize its properties,
  and store it to rfc_cdma_logical_dev[rf_band]
*/
boolean
rfc_cdma_card_intf::populate_hdet
(
 rfc_cdma_logical_device_params_type* logical_dev_ptr,
 rfm_cdma_band_class_type rf_band,
 rfc_device_info_type *device_info_ptr, 
 int16 device_index
)
{ 
  rfc_intf *rfc_cmn = rfc_intf::get_instance();
  rfdevice_class *device_obj = NULL;
  uint8 instance;
  rfcom_band_type_u band_u;

  band_u.cdma_band= rf_band;

                 /*Retrieve device object*/
  instance = (uint8)device_info_ptr->rf_asic_info[device_index].instance;
  device_obj = rfc_cmn->get_cmn_rf_device_object(RFDEVICE_HDET, instance);

                   logical_dev_ptr->hdet_obj[rf_band] = (void *)device_obj;

  return TRUE;
}/* populate_hdet */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Given a RFC device column, acquire Coupler device, initialize its properties,
  and store it to rfc_cdma_logical_dev[rf_band]
*/
boolean
rfc_cdma_card_intf::populate_coupler
(
 rfc_cdma_logical_device_params_type* logical_dev_ptr,
 rfm_cdma_band_class_type rf_band,
 rfc_device_info_type *device_info_ptr, 
 int16 device_index
)
{ 
  rfc_intf *rfc_cmn = rfc_intf::get_instance();
  rfdevice_class *device_obj = NULL;
  uint8 instance;
  rfcom_band_type_u band_u;
  bool result = TRUE ;

  band_u.cdma_band= rf_band;

  if ( logical_dev_ptr->coupler_obj[rf_band] == NULL  )
  {
    logical_dev_ptr->coupler_obj[rf_band] = new rf_cdma_coupler( device_index ) ;
  }

                   /*Retrieve device object*/
  instance = (uint8)device_info_ptr->rf_asic_info[device_index].instance;
  device_obj = rfc_cmn->get_cmn_rf_device_object(RFDEVICE_COUPLER, instance);

                   if( device_obj != NULL )
                   {
                     /* Set PA Band Port Mapping */
                     ((rfdevice_coupler *)device_obj)->set_band_config_map (
                                    RFM_1X_MODE,
                                    band_u,
                             device_info_ptr->rf_asic_info[device_index].data);

   /* Add the ASM device to the wrapper ASM object */
   result &= logical_dev_ptr->coupler_obj[rf_band]->
     add_device_instance((void*)device_obj ) ;
                   }/* if ( device_obj != NULL)*/

  return TRUE;
}/* rfc_cdma_card_intf::populate_coupler */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Given a RFC device column, acquire Tuner device, initialize its properties,
  and store it to rfc_cdma_logical_dev[rf_band]
*/
boolean
rfc_cdma_card_intf::populate_tuner_manager
(
 void** qtuner_obj_ptr,
 rf_cdma_atuner** ant_tuner_ptr,
 rfm_cdma_band_class_type rf_band,
 rfc_device_info_type *device_info_ptr, 
 int16 device_index,
 rfm_device_enum_type device
)
{ 
  rfc_intf *rfc_cmn = rfc_intf::get_instance();

  /* *** Get Tuner manager *** */
  /* Pointer to Tuner manager class to hold common object */
  rfdevice_class *atuner_mgr = NULL;

  rfcom_band_type_u band_u;

  band_u.cdma_band= rf_band;
  
  /* Get default antenna from the device info pointer and use it to
  for the constructor for tuner objects. The default antenna is per
  band and per rx/tx column. The default antenna is used to query
  the tuner driver for the updated tuner list every time there is
  an antenna switch. The default antenna needs to be unique for
  each Rx chain.*/

  if ( *ant_tuner_ptr == NULL )
                   {
    /* Create ATUNER wrapper object for each device and band */
    *ant_tuner_ptr = new rf_cdma_atuner( device, device_info_ptr->ant_num );;
  }

  if ( device_index != -1 )
             {
    uint8 instance;
    rfdevice_class *device_obj;

    instance = (uint8)device_info_ptr->rf_asic_info[device_index].instance; 
    device_obj = rfc_cmn->get_cmn_rf_device_object(RFDEVICE_TUNER,instance);
    if( device_obj != NULL )
          {
      /* Set Tuner Band Distortion Config Mapping */
      ((rfdevice_antenna_tuner *)device_obj)->set_band_distortion_config_map (
                            RFM_1X_MODE,
                            band_u,
                            device_info_ptr->rf_asic_info[device_index].data);

      *qtuner_obj_ptr = (void *)device_obj;

    }/* if ( device_obj != NULL)*/

    /*Grab the associated Tuner manager instance, for now there is one
      to one mapping between a tuner device and tuner manager*/
    atuner_mgr = static_cast<rfcommon_atuner_intf*>(
        rfc_cmn->get_cmn_rf_device_object(RFDEVICE_TUNER_MANAGER, instance));

      /* Make sure there is a valid Antenna tuner manager */
    if (atuner_mgr != NULL)
          {
      (*ant_tuner_ptr)->
      add_default_tuner_instance((rfcommon_atuner_intf*)atuner_mgr, device_obj );

     }
  }
  else
            {
    /* If tuner is not present for this device and this band, try to get
    the dummy Tuner object if needed for a given antenna path.
    Common RFC will return a dummy tuner manager to configure if it knows
    there are tuners on other antenna paths, NULL if no tuner at all */
    rfdevice_class *atuner_mgr =
      static_cast<rfcommon_atuner_intf*>(
         rfc_cmn->get_dummy_tuner_mgr( device_info_ptr->ant_num ) );

    if (atuner_mgr != NULL)
              {
      /* No Physical Tuner for Dummy Tuner hence Tuner Device is NULL */
      (*ant_tuner_ptr)->
      add_default_tuner_instance((rfcommon_atuner_intf*)atuner_mgr, NULL );
    }
              }

  return TRUE;
}/* rfc_cdma_card_intf::populate_tuner_manager */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Given a RFC device column, acquire AsDiv device, initialize its properties.
*/
#ifdef FEATURE_RF_ASDIV
boolean
rfc_cdma_card_intf::populate_asdiv
(
 rfm_cdma_band_class_type rf_band,
 rfc_device_info_type *device_info_ptr, 
 rfm_device_enum_type device
)
{ 
  rfc_intf *rfc_cmn = rfc_intf::get_instance();
  rfc_cdma_data *rfc_data = rfc_cdma_data::get_instance();

  /* Pointer to ASD Tuner Manager */
  rfdevice_class *asd_tuner_mgr = NULL;
  rfc_cfg_params_type asdiv_tuner_band_path_cfg;
  rfc_device_info_type *asdiv_tuner_device_info_ptr = NULL;

            /* Update the rfc_asdiv_config info table with devices on Tx path
            *   (i.e. Tuners and Couplers that will be swapped).
            *   Only require Tuner manager instance 0 to establish device type, actual instance
            *   passed via instance parameter.
                 */
                 asd_tuner_mgr = rfc_cmn->get_cmn_rf_device_object(RFDEVICE_ASD_TUNER_MANAGER, 0);

                 if ( asd_tuner_mgr != NULL )
                 {
                  uint8 dev_count = 0;

                  /* Disable Antenna Swap Port Mapping */
                  asdiv_tuner_band_path_cfg.logical_device = device;
                  asdiv_tuner_band_path_cfg.alternate_path = 0;
                  asdiv_tuner_band_path_cfg.band = rf_band;
                  asdiv_tuner_band_path_cfg.req = RFC_REQ_DISABLE_ANT_SWAP_1;

                  if(rfc_data->devices_cfg_data_get(&asdiv_tuner_band_path_cfg, &asdiv_tuner_device_info_ptr )==TRUE)
                  {
                  for (dev_count =0; dev_count < asdiv_tuner_device_info_ptr->num_devices; dev_count++)
                  {
                    (void)((rfcommon_asdiv_tuner_manager*)asd_tuner_mgr)->\
               update_rfc_asdiv_config_info(&asdiv_tuner_band_path_cfg,    \
                                            asdiv_tuner_device_info_ptr);

                    RF_MSG_5(RF_ERROR, "Debug ASDIV: ant_num: %d device_type: %d instnace: %d device: %d rf_band: %d", asdiv_tuner_device_info_ptr->ant_num, 
                      asdiv_tuner_device_info_ptr->rf_asic_info[dev_count].device_type, asdiv_tuner_device_info_ptr->rf_asic_info[dev_count].instance,
                      asdiv_tuner_band_path_cfg.logical_device, asdiv_tuner_band_path_cfg.band);
                  }
                  }
                  else
                  {
                    RF_MSG_2(RF_ERROR, "Debug ASDIV: asdiv_tuner_device_info_ptr returns NULL ptr device: %d rf_band: %d",                  
                      device, rf_band);
                  }

                  /* Enable Antenna Swap Port Mapping */
                  asdiv_tuner_band_path_cfg.req = RFC_REQ_ENABLE_ANT_SWAP_1;

                  if(rfc_data->devices_cfg_data_get(&asdiv_tuner_band_path_cfg, &asdiv_tuner_device_info_ptr )==TRUE)
                  {
                                
                   for (dev_count =0; dev_count < asdiv_tuner_device_info_ptr->num_devices; dev_count++)
                   {
                   (void)((rfcommon_asdiv_tuner_manager*)asd_tuner_mgr)->\
             update_rfc_asdiv_config_info(&asdiv_tuner_band_path_cfg,    \
                                          asdiv_tuner_device_info_ptr);

                    RF_MSG_5(RF_ERROR, "Debug ASDIV: ant_num: %d device_type: %d instnace: %d device: %d rf_band: %d", asdiv_tuner_device_info_ptr->ant_num, 
                      asdiv_tuner_device_info_ptr->rf_asic_info[dev_count].device_type, asdiv_tuner_device_info_ptr->rf_asic_info[dev_count].instance,
                      asdiv_tuner_band_path_cfg.logical_device, asdiv_tuner_band_path_cfg.band);
                   }
                  }
                  else
                  {
     RF_MSG_2(RF_ERROR, "Debug ASDIV: asdiv_tuner_device_info_ptr "
                        "returns NULL ptr device: %d rf_band: %d",
                      device, rf_band);
                  }
                 }
                 else
                 {
                    RF_MSG(RF_ERROR, "ASDIV: asd_tuner_mgr returns NULL ptr");
                 }

  return TRUE;
}/* rfc_cdma_card_intf::populate_asdiv */
#endif

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Given a RFC device column, acquire ELNA device, initialize its properties,
  and store it to rfc_cdma_logical_dev[rf_band]
*/
boolean
rfc_cdma_card_intf::populate_elna
(
 rfc_cdma_logical_device_params_type* logical_dev_ptr,
 rfm_cdma_band_class_type rf_band,
 rfc_device_info_type *device_info_ptr, 
 int16 device_index,
 rfm_device_enum_type device
)
{ 
  rfc_intf *rfc_cmn = rfc_intf::get_instance();
  rfdevice_class *device_obj = NULL;
  uint8 instance;
  rfcom_band_type_u rf_band_u;
  boolean result = TRUE;

  rf_band_u.cdma_band= rf_band;

                  /*Retrieve device object*/
  instance = (uint8)device_info_ptr->rf_asic_info[device_index].instance; 
  device_obj = rfc_cmn->get_cmn_rf_device_object( RFDEVICE_LNA, instance );

                  /* Perform NULL pointer check & band path config check */
                  if ( device_obj != NULL )
                  {
    /* Populate LNA config params */
    rfdevice_lna_config_type lna_cfg;
    lna_cfg.rfm_device = device;
    lna_cfg.band = rf_band_u;
    lna_cfg.mode = RFM_1X_MODE;/* same for 1x/HDR */
    /* The rest of the fields are dont care in this case*/
    lna_cfg.trigger_type = RFDEVICE_LNA_TRIGGER_NONE;
    lna_cfg.lna_gain = RFDEVICE_LNA_GAIN_INVALID;
    lna_cfg.system_gain = 0;
    /* Set the Rx band map for this ELNA device for selected 
    band */ 
    result &= rfdevice_lna_set_band_map(
                               (void*)device_obj,
         &lna_cfg,
         device_info_ptr->rf_asic_info[device_index].data,
         sizeof(device_info_ptr->rf_asic_info[device_index].data) );
    /* Add ELNA to device list */
    logical_dev_ptr->elna_obj[rf_band] = (void *)device_obj;                 
  }
                
  return result;
}/* rfc_cdma_card_intf::populate_elna */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Creates and sets up all Tx devices for a device & band.

  @param device
  RFM device for RFC device configuration

  @param logical_dev_ptr
  Pointer to the logical device

  @param alt_path
  Alternate Rx path, used if there is a remapping required based on concurrency
  manager recommendation

  @param bc_config
  The band-chan config NV for the device being initialized

  @Return
  TRUE on success, and FALSE on failure

*/
boolean
rfc_cdma_card_intf::populate_tx_devices
(
  rfm_device_enum_type device
)
{
  boolean result = TRUE ;
  
  rfc_device_info_type *device_info_ptr = NULL;
  rfc_cdma_data *rfc_data = rfc_cdma_data::get_instance();
  rfc_cdma_logical_device_params_type* logical_dev_ptr = 
                                              logical_dev_param_ptr[device];
  
  rfc_cfg_params_type band_path_cfg;
  boolean tmp_status = FALSE;
  rfc_dynamic_properties_params_type rfc_dynamic_params;
  sys_band_class_e_type sys_band = SYS_BAND_CLASS_MAX;
  rfdevice_rxtx_common_class* cmn_device_ptr = NULL;
  rfc_intf *rfc_cmn_ptr = rfc_intf::get_instance();

  /* Accessor function to obtain the RF Signal information for
  requested band path configuration */
   /* Send in alt path as zero in this case since there is no remapping */
  band_path_cfg.alternate_path = 0;
  band_path_cfg.logical_device = device;
  band_path_cfg.rx_tx = RFC_CONFIG_TX;
  band_path_cfg.req = RFC_REQ_DEFAULT_GET_DATA;
    /* Path is not used by CDMA, but populated for RFC */
        band_path_cfg.path = RF_PATH_0;

  memset( &rfc_dynamic_params, 0x0, sizeof(rfc_dynamic_params) );

  if ( NULL != logical_dev_ptr )
  {
    uint8 rf_band;

    boolean set_band_agnostic_info = TRUE;

    for ( rf_band = RFM_CDMA_BC0; rf_band < RFM_CDMA_MAX_BAND; rf_band++ )
    {    
      if ( rf_cdma_is_band_supported_on_any_device( (rfm_cdma_band_class_type) rf_band ) )
      {      
        rfcom_band_type_u band_u;       
        band_u.cdma_band = (rfm_cdma_band_class_type)rf_band;
        band_path_cfg.band = rf_band;/* populate band path config struct */
        sys_band = rfcommon_core_convert_rfcom_band_to_sys_band( RFM_1X_MODE, band_u );

        /* Call this once for a card for every band to configure the xsw */
        rfc_cdma_set_xsw_band_ports ( device , band_u , band_path_cfg ) ;

        /* Get device info pointer */
        rfc_data->devices_cfg_data_get(&band_path_cfg, &device_info_ptr );

        if ( device_info_ptr != NULL )
                  {
          /* Initialize the counters , need to do this after each iteration
          for a band, to avoid issues that might be cause due to different
          indexing of device objects */
          uint8 ctr = 0;
          boolean pa_found = FALSE;
          boolean tuner_found = FALSE;
          /*Device type used to extract tech specific object*/
          rfdevice_type_enum_type cmn_dev_type = RFDEVICE_TYPE_INVALID;

          /* Get Tx device info ptr (used to access modem chain and nv path)*/
          logical_dev_ptr->rfc_tx_device_info.rfc_device_info[rf_band] = 
                                                          device_info_ptr;
            
           /* Set Tx NV path for this device and band */
          logical_dev_ptr->tx_nv_path[rf_band] = 
                      (rf_cdma_nv_path_type)device_info_ptr->nv_container;

          if ( device_info_ptr->nv_container < RF_CDMA_NV_PATH_NUM )
            {
            valid_nv_path_array[device_info_ptr->nv_container][rf_band]= TRUE; 
            }

          /*Check if band agnostic info for this logical device has been set,
          Set the info if not, skip this step otherwise */
          if ( set_band_agnostic_info == TRUE )
          {
            result &= set_tx_band_agnostic_info( device ,device_info_ptr );
            set_band_agnostic_info = FALSE;
          }/*if ( set_band_agnostic_info == TRUE )*/

           /* loop through the device info ptr to find the index of each device object*/
          while (ctr < device_info_ptr->num_devices)
          {
            /*Get device type and device instance to retrieve device object*/
            cmn_dev_type = device_info_ptr->rf_asic_info[ctr].device_type;
            switch (cmn_dev_type)
            {
            case RFDEVICE_TRANSCEIVER:
              result &= populate_tx_transceiver( 
                                     logical_dev_ptr, (rfm_cdma_band_class_type)rf_band, 
                                     device_info_ptr, ctr, device );

              cmn_device_ptr = (rfdevice_rxtx_common_class*) rfc_cmn_ptr->get_cmn_rf_device_object( RFDEVICE_TRANSCEIVER,
                                                                                                    device_info_ptr->rf_asic_info[ctr].instance );

              /////////////////// START RFC DYNAMIC PROPERTIES SECTION ///////////////////

              // Get tranceiver indication of restriction to single tech
              tmp_status = TRUE;
              if( !rfdevice_cmn_get_rxtx_simultaneous_rat_support( cmn_device_ptr, 
                                                                           &rfc_dynamic_params.supports_multi_rat ) )
              {
                tmp_status = FALSE;
                RF_MSG( RF_ERROR, "RFC ERROR. CMN RF device rat support API returned false. Cannot generate RFC dynamic properties." );
              }

              if (tmp_status == TRUE)
               {
                // Generate RFC dynamic properties
                rfc_dynamic_params.device = device;
                rfc_dynamic_params.dev_instance = (uint8) device_info_ptr->rf_asic_info[ctr].instance;
                rfc_dynamic_params.sys_band = sys_band;
                rfc_dynamic_params.antenna = device_info_ptr->ant_num;
                (void) rfc_cmn_ptr->generate_rfcard_dynamic_properties( &rfc_dynamic_params );
              }
               else
               {
                RF_MSG( RF_ERROR, "RFC detected ERROR. See preceeding errors. Cannot generate RFC dynamic properties." );
              }

              /////////////////// END RFC DYNAMIC PROPERTIES SECTION ///////////////////

              break;
            case RFDEVICE_PA:
              result &= populate_pa( logical_dev_ptr, (rfm_cdma_band_class_type)rf_band, 
                                     device_info_ptr, ctr, device,
                                     &band_path_cfg );
              pa_found = TRUE;
              break;
            case RFDEVICE_ASM:
              result &= populate_tx_asm( logical_dev_ptr, (rfm_cdma_band_class_type)rf_band, 
                                         device_info_ptr, ctr, device );                    
              break;
            case RFDEVICE_PAPM:
              result &= populate_papm( logical_dev_ptr, (rfm_cdma_band_class_type)rf_band, 
                                       device_info_ptr, ctr, device );
              break;
            case RFDEVICE_HDET:
              result &= populate_hdet( logical_dev_ptr, (rfm_cdma_band_class_type)rf_band, 
                                       device_info_ptr, ctr );
              break;
            case RFDEVICE_COUPLER:
              result &= populate_coupler( logical_dev_ptr, (rfm_cdma_band_class_type)rf_band, 
                                          device_info_ptr, ctr );
              break;
            case RFDEVICE_TUNER:
              result &= populate_tuner_manager( &(logical_dev_ptr->qtuner_obj[rf_band]),
                                                &(logical_dev_ptr->ant_tuner[rf_band]),
                                                (rfm_cdma_band_class_type)rf_band, 
                                                device_info_ptr, ctr, device );                
              tuner_found = TRUE;
              break;
            default:
              break;
            }
            ctr++; 
          }/*while (ctr < device_info_ptr->num_devices)*/

          /* Special handling for legacy GRFC PA*/
          if ( pa_found == FALSE )
          {
            result &= populate_pa( logical_dev_ptr, (rfm_cdma_band_class_type)rf_band, 
                                   device_info_ptr, -1, device,
                                   &band_path_cfg );
          }

          /* Special handling for dummy Tuner */
          if ( tuner_found == FALSE )
                 {
            result &= populate_tuner_manager( &(logical_dev_ptr->qtuner_obj[rf_band]),
                                              &(logical_dev_ptr->ant_tuner[rf_band]),
                                              (rfm_cdma_band_class_type)rf_band, 
                                              device_info_ptr, -1, device );                
          }

  #ifdef FEATURE_RF_ASDIV
          result &= populate_asdiv ((rfm_cdma_band_class_type)rf_band, 
                                    device_info_ptr, device );
  #endif   
        }/*if ( device_info_ptr != NULL )*/
                 else
                  {
          RF_MSG_1(RF_HIGH,"get_device_objects: NULL pointer returned "
                        "for device_info_ptr for band %d", rf_band);
        }/*if ( device_info_ptr == NULL )*/           
      }/* if ( rf_cdma_is_band_supported_on_any_device( rf_band ) )*/
    } /* for ( rf_band = RFM_CDMA_BC0; rf_band < RFM_CDMA_MAX_BAND; .... */
  } /* logical_dev_ptr != NULL */

  return result;
} /* populate_tx_devices */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Creates and sets up all Rx devices for a device & band.

  @param device
  RFM device for RFC device configuration

  @param logical_dev_ptr
  Pointer to the logical device

  @param bc_config
  The band-chan config NV for the device being initialized

  @Return
  TRUE on success, and FALSE on failure

*/
boolean
rfc_cdma_card_intf::populate_rx_devices
(
  rfm_device_enum_type device
)
{
  boolean result = TRUE ;
  rfc_cfg_params_type band_path_cfg;

  /*Device type used to extract tech specific object*/
  rfdevice_type_enum_type cmn_dev_type = RFDEVICE_TYPE_INVALID;

  rfc_device_info_type *device_info_ptr = NULL;
  rfc_cdma_data *rfc_data = rfc_cdma_data::get_instance();
  rfc_cdma_logical_device_params_type* logical_dev_ptr = 
                                            logical_dev_param_ptr[device];

  /* Accessor function to obtain the RF Signal information for
  requested band path configuration */
  /* Send in alt path as zero in this case since there is no remapping */
  band_path_cfg.alternate_path = 0;
  band_path_cfg.logical_device = device;
  band_path_cfg.rx_tx = RFC_CONFIG_RX;
  band_path_cfg.req = RFC_REQ_DEFAULT_GET_DATA;
  /* Path is not used by CDMA, but populated for RFC */
  band_path_cfg.path = RF_PATH_0;
  boolean tmp_status = FALSE;
  rfc_dynamic_properties_params_type rfc_dynamic_params;
  sys_band_class_e_type sys_band = SYS_BAND_CLASS_MAX;
  rfdevice_rxtx_common_class* cmn_device_ptr = NULL;
  rfc_intf *rfc_cmn_ptr = rfc_intf::get_instance();

  memset( &rfc_dynamic_params, 0x0, sizeof(rfc_dynamic_params) );

  if ( NULL != logical_dev_ptr )
                 {
    uint8 rf_band;
    boolean set_band_agnostic_info = TRUE;

    for ( rf_band = RFM_CDMA_BC0; rf_band < RFM_CDMA_MAX_BAND; rf_band++ )
    {
      if ( rf_cdma_is_band_supported(device,(rfm_cdma_band_class_type)rf_band) )
                     {
        /* Band union, used for tech bands */
        rfcom_band_type_u band_u;
        band_u.cdma_band = (rfm_cdma_band_class_type)rf_band;                       
        band_path_cfg.band = (rfm_cdma_band_class_type)rf_band;
        sys_band = rfcommon_core_convert_rfcom_band_to_sys_band( RFM_1X_MODE, band_u );
        
        /* Call this once for a card for every band to configure the xsw */
        rfc_cdma_set_xsw_band_ports ( device , band_u , band_path_cfg ) ;

        /* Get device info pointer */
        rfc_data->devices_cfg_data_get(&band_path_cfg, &device_info_ptr );

        if ( device_info_ptr != NULL )
                     {
          uint8 ctr = 0; /*counter for looping*/
          uint8 tranceiver_count = 0;
          boolean tuner_found = FALSE;
        
          /* Get device info ptr (used to access modem chain and nv path)*/
          logical_dev_ptr->rfc_dev_info.rfc_device_info[rf_band] = device_info_ptr;

          logical_dev_ptr->rx_nv_path[rf_band] = 
                        ( rf_cdma_nv_path_type )device_info_ptr->nv_container;

          if ( device_info_ptr->nv_container < RF_CDMA_NV_PATH_NUM )
                     {
            valid_nv_path_array[device_info_ptr->nv_container][rf_band] = TRUE; 
                     }

          /*Check if band agnostic info for this logical device has been set,
            Set the info if not, skip this step otherwise */
          if ( set_band_agnostic_info == TRUE )
          {
            result &= set_rx_band_agnostic_info( device ,device_info_ptr );
            set_band_agnostic_info = FALSE;
          }/*if ( set_band_agnostic_info == TRUE )*/

           /* loop through the device info ptr to find the index of each device object*/
          /* TODO */
          while (ctr < device_info_ptr->num_devices)
            {
            /*Get device type and device instance to retrieve device object*/
            cmn_dev_type = device_info_ptr->rf_asic_info[ctr].device_type;

            switch (cmn_dev_type)
                     {
            case RFDEVICE_TRANSCEIVER:
              tranceiver_count++;
              result &= populate_rx_transceiver( 
                                     logical_dev_ptr, (rfm_cdma_band_class_type)rf_band, 
                                     device_info_ptr, ctr, tranceiver_count,
                                     device ); 
              
              cmn_device_ptr = (rfdevice_rxtx_common_class*) rfc_cmn_ptr->get_cmn_rf_device_object( RFDEVICE_TRANSCEIVER,
                                                                                                    device_info_ptr->rf_asic_info[ctr].instance );

              /////////////////// START RFC DYNAMIC PROPERTIES SECTION ///////////////////

              // Get tranceiver indication of restriction to single tech
              tmp_status = TRUE;
              if( !rfdevice_cmn_get_rxtx_simultaneous_rat_support( cmn_device_ptr, 
                                                                           &rfc_dynamic_params.supports_multi_rat ) )
                {
                tmp_status = FALSE;
                RF_MSG( RF_ERROR, "RFC ERROR. CMN RF device rat support API returned false. Cannot generate RFC dynamic properties." );
                     }

              if (tmp_status == TRUE)
                {
                // Generate RFC dynamic properties
                rfc_dynamic_params.device = device;
                rfc_dynamic_params.dev_instance = (uint8) device_info_ptr->rf_asic_info[ctr].instance;
                rfc_dynamic_params.sys_band = sys_band;
                rfc_dynamic_params.antenna = device_info_ptr->ant_num;
                (void) rfc_cmn_ptr->generate_rfcard_dynamic_properties( &rfc_dynamic_params );           
                }
                else
                {
                RF_MSG( RF_ERROR, "RFC detected ERROR. See preceeding errors. Cannot generate RFC dynamic properties." );
                }

              /////////////////// END RFC DYNAMIC PROPERTIES SECTION ///////////////////
                   
              break;
            case RFDEVICE_ASM:
              result &= populate_rx_asm( logical_dev_ptr, (rfm_cdma_band_class_type)rf_band, 
                                         device_info_ptr, ctr, device );
              break;
            case RFDEVICE_TUNER:                  
               result &= populate_tuner_manager( &(logical_dev_ptr->qtuner_obj[rf_band]),
                                                 &(logical_dev_ptr->ant_tuner[rf_band]),
                                                 (rfm_cdma_band_class_type)rf_band, 
                                                 device_info_ptr, ctr, device );                
               tuner_found = TRUE;
               break;
            case RFDEVICE_LNA:                
               result &= populate_elna( logical_dev_ptr, (rfm_cdma_band_class_type)rf_band, 
                                        device_info_ptr, ctr, device );                                 
               break;
            default:
              break;
              }
            ctr++;
          }/*while (ctr < device_info_ptr->num_devices)*/

          /* Special handling for dummy Tuner */
          if ( tuner_found == FALSE )
          {
            result &= populate_tuner_manager( &(logical_dev_ptr->qtuner_obj[rf_band]),
                                              &(logical_dev_ptr->ant_tuner[rf_band]),
                                              (rfm_cdma_band_class_type)rf_band, 
                                              device_info_ptr, -1, device );                
          }

  #ifdef FEATURE_RF_ASDIV
          result &= populate_asdiv ((rfm_cdma_band_class_type)rf_band, 
                                    device_info_ptr, device );
  #endif
        }/* if ( device_info_ptr != NULL ) */
      }
      } /* for ( rf_band = RFM_CDMA_BC0; rf_band < RFM_CDMA_MAX_BAND; .... */
  } /* logical_dev_ptr != NULL */
  return result;
} /* populate_rx_devices */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Creates the RX chain for a given device with new alt path.

  @details
  Creates and sets up devices including ASM, TUNER and WTR for a
  RX device and sets up WTR, ASM, TUNER for a given band.

  @param device
  RFM device for RFC device configuration

  @params band_path_cfg
  THis is required to query the RFC for Alt path Rx devices
  information.

  @param alt_path
  Alternate Rx path, used if there is a remapping required based on concurrency
  manager recommendation

  @Return
  TRUE on success, and FALSE on failure

*/

boolean
rfc_cdma_card_intf::recreate_rx_chain_with_updated_alt_path
(
  rfm_device_enum_type  device        ,
  rfc_cfg_params_type&  band_path_cfg ,
  uint8                 alt_path
)
{
  boolean result = TRUE ;
  boolean tuner_found = FALSE;

  /* Iterator to loop through the Rx devices for a given Rx path */
  uint8 ctr = 0 ;
  rfc_device_info_type* device_info_ptr = NULL ;
  rfc_cdma_data* rfc_data = rfc_cdma_data::get_instance();
  rfc_intf *rfc_cmn = rfc_intf::get_instance();

  rf_card_band_type curr_band = (rf_card_band_type)band_path_cfg.band ;

  RF_MSG_5 ( RF_MED , "recreate_rx_chain_with_updated_alt_path(): "
            "||Dev: %d|CP: %d|AP: %d|Band: %d|req: %d||",
            device, (rfc_cdma_get_current_rx_path ( device ,
            (rfm_cdma_band_class_type)curr_band )),
            alt_path , curr_band , band_path_cfg.req ) ;

  /* Update the new path with cuurent path. */
  rfc_cmda_set_current_rx_path_for_device ( device ,
                                            (rfm_cdma_band_class_type)curr_band ,
                                            alt_path ) ;

  if ( ( rfc_data != NULL ) && ( ((rfm_cdma_band_class_type)curr_band) < RFM_CDMA_MAX_BAND ) )
  { /* Get device info pointer */
    if  ( TRUE == rfc_data->devices_cfg_data_get ( &band_path_cfg , &device_info_ptr ) )
    {
      if ( device_info_ptr != NULL )
      {
        rfc_cdma_logical_device_params_type* logical_dev_ptr =
          logical_dev_param_ptr[device] ;

        rfcom_band_type_u band_u;
        band_u.cdma_band = (rfm_cdma_band_class_type)curr_band ;

        /* Call this once for a card for every band to configure the xsw */
        rfc_cdma_set_xsw_band_ports ( device , band_u , band_path_cfg ) ;

        /* loop through the device info ptr to find the index of each device object*/
        RF_MSG_1 ( RF_LOW , "recreate_rx_chain_with_updated_alt_path(): "
                       "Looping through %d devices." , device_info_ptr->num_devices ) ;
        
        /* Clear the tuner\tuner manager lists before updating the new tuner\tuner manager 
        list based on the alt path */
        if (logical_dev_ptr->ant_tuner[curr_band] != NULL )
        {
          (logical_dev_ptr->ant_tuner[curr_band])->
          clear_default_tuner_instance();          
        } 

        while ( ctr < device_info_ptr->num_devices )
        {
          uint32 instance = device_info_ptr->rf_asic_info[ctr].instance ;
          rfdevice_type_enum_type dev_type =
                  device_info_ptr->rf_asic_info[ctr].device_type ;
          rfdevice_class* device_obj =
               rfc_cmn->get_cmn_rf_device_object ( dev_type , (uint8)instance ) ;

          uint8 asm_ctr = 0 ;

          /*Get device type and device instance to retrieve device object*/
          switch ( dev_type )
          {
            case RFDEVICE_TRANSCEIVER :
            {
              /* Perform NULL pointer check & band path config check */
              if( device_obj != NULL )
              {
                RF_MSG ( RF_LOW ,
                         "recreate_rx_chain_with_updated_alt_path(): "
                         "Updating WTR ports " ) ;
                rfdevice_trx_phy_path_enum_type phy_path =
                        (rfdevice_trx_phy_path_enum_type)
                        device_info_ptr->rf_asic_info[ctr].phy_path_num ;
                /* extract the cdma device object from the cmn_device object and store it */
                rfdevice_trx_cdma_rx* rx_device =
                  (rfdevice_trx_cdma_rx*)\
                (((rfdevice_rxtx_common_class *)device_obj)->get_tech_instance(RFM_1X_MODE,
                                                                                RFDEVICE_RX,
                                                                                (rfdevice_trx_phy_path_enum_type)phy_path));

                result &= rfdevice_cdma_rx_set_band_data ( rx_device ,
                                                band_u.cdma_band ,
                                                device_info_ptr->rf_asic_info[ctr].data,
                                                RFC_ASIC_INFO_DATA_SIZE) ;

              } /* if( device_obj != NULL ) */
            } /* case RFDEVICE_TRANSCEIVER :  */
            break ;


            case RFDEVICE_ASM:
            {
               /* Perform NULL pointer check & band path config check */
              if ( device_obj != NULL )
              {
                RF_MSG ( RF_LOW ,
                         "recreate_rx_chain_with_updated_alt_path(): "
                         "Updating ASM ports " ) ;
                /* Set the Rx band map for this ASM device for selected band */
                /* Also pass the size of the array where data is being stored */
                result &= rfdevice_asm_set_rx_band_map (
                           (void*)device_obj,
                           device,
                           RFM_1X_MODE,
                           band_u,
                           device_info_ptr->rf_asic_info[ctr].data,
                           RFC_ASIC_INFO_DATA_SIZE ) ;


                /* Add the ASM device to the wrapper ASM object */
                /* result &=
                logical_dev_ptr->sm[curr_band]->update_device_instance(
                   (void*)device_obj , RX_ASM  , asm_ctr ) ;  */

                asm_ctr++ ;
              }/* if ( device_obj != NULL)*/
              else
              {
                  RF_MSG_1 ( RF_ERROR , "rfc_cdma_card_intf::recreate_rx_chain_with_updated_alt_path: "
                             "Failed to get ASM device obj for dev: %d", device ) ;
              }/* if( device_obj == NULL )*/
            }
            break ;

            case RFDEVICE_TUNER:
            {
              if( device_obj != NULL )
              {
                RF_MSG ( RF_LOW ,
                         "recreate_rx_chain_with_updated_alt_path(): "
                         "Updating TUNER ports " ) ;

                #ifdef FEATURE_RF_HAS_QTUNER
                logical_dev_ptr->qtuner_obj[curr_band] = (void *)device_obj;
                #endif

              }/* if ( device_obj != NULL)*/
              else
              {
                RF_MSG_1 ( RF_ERROR , "rfc_cdma_card_intf::recreate_rx_chain_with_updated_alt_path: "
                           "Failed to get TUNER device obj for dev: %d" , device ) ;
              }/* if ( device_obj == NULL)*/

              /* *** Get Tuner manager *** */
              /* Pointer to Tuner manager class to hold common object */
              rfdevice_class *atuner_mgr = NULL;
              /*Grab the associated Tuner manager instance, for now there is one
               to one mapping between a tuner device and tuner manager*/
              atuner_mgr =
               static_cast<rfcommon_atuner_intf*>(
                 rfc_cmn->get_cmn_rf_device_object(RFDEVICE_TUNER_MANAGER,
                                                   (uint8)instance));
          /* Create the tuner wrapper object if we it is not created for this logical device 
              if the object is already created then just update the antenna number that corresponds 
              to the new alt path*/       
              if (logical_dev_ptr->ant_tuner[curr_band] == NULL )
              {
                /* Create ATUNER wrapper object for each device and band */
                logical_dev_ptr->ant_tuner[curr_band] = new rf_cdma_atuner( device, device_info_ptr->ant_num );
                }
          else
                {
            (logical_dev_ptr->ant_tuner[curr_band])->
        set_default_ant_num (device_info_ptr->ant_num );
                }
              /* Make sure there is a valid Antenna tuner manager */
              if (atuner_mgr != NULL)
                {
                (logical_dev_ptr->ant_tuner[curr_band])->
                add_default_tuner_instance((rfcommon_atuner_intf*)atuner_mgr, device_obj );
              } /*else if (atuner_mgr != NULL) */
              tuner_found = TRUE; 
            } /*  case RFDEVICE_TUNER: */
            break ;

            default:
            break;

          } /* SWITCH-CASE */
          ctr++ ;
        }/*while (ctr < device_info_ptr->num_devices)*/
        if (tuner_found == FALSE)
    {
      /* If tuner is not present for this device and this band, try to get
      the dummy Tuner object if needed for a given antenna path.
      Common RFC will return a dummy tuner manager to configure if it knows
          there are tuners on other antenna paths, NULL if no tuner at all */
          rfdevice_class *atuner_mgr =
              static_cast<rfcommon_atuner_intf*>(
              rfc_cmn->get_dummy_tuner_mgr( device_info_ptr->ant_num ));

          if (atuner_mgr != NULL)
          {
            /* No Physical Tuner for Dummy Tuner hence Tuner Device is NULL */
            (logical_dev_ptr->ant_tuner[curr_band])->
                add_default_tuner_instance((rfcommon_atuner_intf*)atuner_mgr, NULL );
          }
        }

        RF_MSG_2 ( RF_MED , "recreate_rx_chain_with_updated_alt_path(): "
                   "||dev: %d|OldModemChain: %d||" ,
                   device ,
                   logical_dev_ptr->rfc_dev_info.
                   rfc_device_info[curr_band]->modem_chain ) ;
        /* Also update the modem chain while configuring the new alt path */
        logical_dev_ptr->rfc_dev_info.rfc_device_info[curr_band] =
          device_info_ptr ;
        RF_MSG_2 ( RF_MED , "recreate_rx_chain_with_updated_alt_path(): "
                   "||dev: %d|NewModemChain: %d||" ,
                   device ,
                   logical_dev_ptr->rfc_dev_info.
                   rfc_device_info[curr_band]->modem_chain ) ;

      } /* if ( device_info_ptr != NULL ) */
      else
      {
        RF_MSG ( RF_ERROR , "recreate_rx_chain_with_updated_alt_path(): "
                 "devices_cfg_data_get() returned [device_info_ptr == NULL] " ) ;
      }
    } /* if  ( TRUE == rfc_data->devices_cfg_data_get ( &band_path_cfg, &device_info_ptr ) ) */
    else
    {
      RF_MSG ( RF_ERROR , "recreate_rx_chain_with_updated_alt_path(): "
                      "rfc_data->devices_cfg_data_get() returned FALSE " ) ;
    }
  } /* If rfc_data != NULL */
  else
  {
    result = FALSE ;
    RF_MSG_2 ( RF_ERROR , "recreate_rx_chain_with_updated_alt_path(): "
               "rfc_data() NULL[0x%P] or Invalid Band[%d] " ,
               rfc_data , curr_band ) ;

  }
  return result ;
} /* recreate_rx_chain_with_updated_alt_path */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Creates and sets band agnostic Rx information for a given band path
  configuration and the corresponding device information
  (NV path/ FTM chain/ RX LM ADC)

  @param logical_device
  The logical device for which this info needs to be set

  @param device_info_ptr
  The pointer to the device information for this band-path config (nv_container,
  modem_chain, asic_info etc.)
*/
boolean
rfc_cdma_card_intf::set_rx_band_agnostic_info
(
  rfm_device_enum_type logical_device,
  rfc_device_info_type *device_info_ptr
)
{
  boolean api_status = TRUE;

  if ( logical_dev_param_ptr[logical_device] == NULL )
  {
    return FALSE;
  }

  /* Set RXLM chain for 1x*/
  logical_dev_param_ptr[logical_device]->ftm_rxlm_chain_onex =
                                ( rxlm_chain_type )device_info_ptr->modem_chain;

  /* Set RXLM chain for HDR*/
  logical_dev_param_ptr[logical_device]->ftm_rxlm_chain_hdr =
                                ( rxlm_chain_type )device_info_ptr->modem_chain;

  /* Set RXLM ADC chain */
  logical_dev_param_ptr[logical_device]->rxlm_adc =
                                  ( rxlm_adc_type )device_info_ptr->modem_chain;

  /* Set paired devices and Rx MDSP paths */
  if ( logical_device == RFM_DEVICE_0 )
  {
    if(NULL != rfc_cdma_get_logical_device_params(RFM_DEVICE_1))
    {
      /*Paired device for logical device zero is one and vice versa*/
      logical_dev_param_ptr[logical_device]->paired_device = RFM_DEVICE_1;
    }
    else
    {
      logical_dev_param_ptr[logical_device]->paired_device = RFM_INVALID_DEVICE;
    }
  }
  else if ( logical_device == RFM_DEVICE_1 )
  {
    if(NULL != rfc_cdma_get_logical_device_params(RFM_DEVICE_0))
    {
      /*Paired device for logical device zero is one and vice versa*/
      logical_dev_param_ptr[logical_device]->paired_device = RFM_DEVICE_0;
    }
    else
    {
      logical_dev_param_ptr[logical_device]->paired_device = RFM_INVALID_DEVICE;
    }
  }
  else if ( logical_device == RFM_DEVICE_2 )
  {
    /* The physical pairing between Dev 2 and Dev 3 is no longer valid due
    to change in system design. The SSMA module which requires the paired
    device will be reconsidered. For now invalidate Dev2's paired_device */
    logical_dev_param_ptr[logical_device]->paired_device = RFM_INVALID_DEVICE;
  }
  else if ( logical_device == RFM_DEVICE_3 )
    {
    /* The physical pairing between Dev 2 and Dev 3 is no longer valid due
    to change in system design. The SSMA module which requires the paired
    device will be reconsidered. For now invalidate Dev3's paired_device */
      logical_dev_param_ptr[logical_device]->paired_device = RFM_INVALID_DEVICE;
  }
  else if ( logical_device == RFM_DEVICE_4 )
  {
    /* The physical pairing between Dev 2 and Dev 3 is no longer valid due
    to change in system design. The SSMA module which requires the paired
    device will be reconsidered. For now invalidate Dev2's paired_device */
    logical_dev_param_ptr[logical_device]->paired_device = RFM_INVALID_DEVICE;
  }
  else if ( logical_device == RFM_DEVICE_5 )
  {
    /* The physical pairing between Dev 2 and Dev 3 is no longer valid due
    to change in system design. The SSMA module which requires the paired
    device will be reconsidered. For now invalidate Dev3's paired_device */
      logical_dev_param_ptr[logical_device]->paired_device = RFM_INVALID_DEVICE;
  }
  else
  {
    RF_MSG_1 ( RF_ERROR , "set_rx_band_agnostic_info: Invalid logical device:%d ",
               logical_device ) ;
    api_status = FALSE;
  }

  return api_status;

} /* rfc_cdma_card_intf::set_rx_band_agnostic_info */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Creates and sets band agnostic Tx information for a given band path
  configuration and the corresponding device information
  (Tx NV path/Rx LM DAC/ FTM chain/ MDSP chain)

  @param logical_device
  The logical device for which this info needs to be set

  @param device_info_ptr
  The pointer to the device information for this band-path config (nv_container,
  modem_chain, asic_info etc.)
*/
boolean
rfc_cdma_card_intf::set_tx_band_agnostic_info
(
  rfm_device_enum_type logical_device,
  rfc_device_info_type *device_info_ptr
)
{
  boolean api_status = TRUE;

  if ( logical_dev_param_ptr[logical_device] == NULL )
  {
    return FALSE;
  }

  /* Set TXLM chain for 1x*/
  logical_dev_param_ptr[logical_device]->ftm_txlm_chain_onex =
                                ( txlm_chain_type )device_info_ptr->modem_chain;

  /* Set TXLM chain for HDR*/
  logical_dev_param_ptr[logical_device]->ftm_txlm_chain_hdr =
                                ( txlm_chain_type )device_info_ptr->modem_chain;

  /* Set TXLM DAC chain */
  logical_dev_param_ptr[logical_device]->txlm_dac =
                                  ( txlm_dac_type )device_info_ptr->modem_chain;

  /* Always use MDSP_CHAIN_0 for FW path. mdsp_chain was used to identify 
  modem chain/path for FW. Its functioanlity has been replaced by txlm_handle,
  so always hard-code a 0 to indicate a valid mdsp path for compatibility.
  once tx_mdsp_path is cleaned up, it can be removed  */
    logical_dev_param_ptr[logical_device]->tx_mdsp_path = RF_CDMA_MDSP_CHAIN_0;

  return api_status;

} /* rfc_cdma_card_intf::set_tx_band_agnostic_info */

/*! @} */

/*============================================================================*/
/*!
  @name XPT configure feedback for IQ capture

  @brief
  This section XPT calibration related functions that sets up feedback
  for IQ capture
*/
/*! \{ */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  queries RFC ag to setup feedback path signals

  @details
  queries RFC ag to setup feedback path signals for IQ capture during XPT cal

  @param device_fb
  feedback RF device to which the path is set up

  @param band
  Enum to indicate the cdma band

  @param et_enable
  boolean enable or disable ET

  @return
  boolean status
*/
/*----------------------------------------------------------------------------*/
boolean rfc_cdma_card_intf::setup_fb_path
(
  rfm_device_enum_type device_fb,
  rfm_cdma_band_class_type band,
  boolean et_enable
)
{
  rfc_cfg_params_type cfg;
  rfc_sig_cfg_type       *signal_list_ptr = NULL;
  rfc_cdma_data *rfc_cdma_data_ptr = NULL;

  cfg.alternate_path = 0;
  cfg.band = (int)band;
  cfg.rx_tx = RFC_CONFIG_TX;
  cfg.logical_device = device_fb;
  cfg.req = et_enable?RFC_REQ_ENABLE_XPT_CAPTURE:RFC_REQ_DISABLE_XPT_CAPTURE;

  rfc_cdma_data_ptr = rfc_cdma_data::get_instance();

  rfc_cdma_data_ptr->sig_cfg_data_get(&cfg, &signal_list_ptr);

  rfc_common_config_signal(signal_list_ptr, TRUE /* gnss_blanking_enabled */ );

  return TRUE;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  queries RFC ag function to setup feedback device

  @details
  queries RFC ag function to setup feedback device for IQ capture during XPT cal

  @param device
  Transmitting RF device that sends IQ samples

  @param device_fb
  feedback RF device to which the path is set up

  @param band
  Enum to indicate the cdma band

  @param tx_chan
  ARFCN channel number

  @param fb_device_ptr
  rfcmn_device_type common device pointer for feedback device

  @return
  boolean status
*/
/*----------------------------------------------------------------------------*/
boolean rfc_cdma_card_intf::get_fb_tranceivers
(
  rfm_device_enum_type device,
  rfm_device_enum_type device_fb,
  rfm_cdma_band_class_type band,
  rfdevice_rxtx_common_class**  tx_device_ptr,
  rfdevice_rxtx_common_class**  fb_device_ptr
)
{
  rfc_cfg_params_type cfg;
  rfc_intf *rfc_cmn_ptr = rfc_intf::get_instance();
  rfdevice_class *trx_device = NULL;
  rfc_device_info_type *tx_device_info_ptr = NULL;
  rfc_device_info_type *fb_device_info_ptr = NULL;
  rfc_cdma_data *rfc_cdma_data_ptr = rfc_cdma_data::get_instance();

  cfg.alternate_path = 0;
  cfg.band = (int)band;
  cfg.rx_tx = RFC_CONFIG_TX;
  cfg.logical_device = device;
  cfg.req = RFC_REQ_DEFAULT_GET_DATA;

  if (rfc_cdma_data_ptr == NULL || rfc_cmn_ptr == NULL)
  {
    RF_MSG_1(RF_ERROR,"NULL RFC Pointer device %d ", device);
    return FALSE;
  }

  rfc_cdma_data_ptr->devices_cfg_data_get(&cfg, &tx_device_info_ptr);

  if (tx_device_info_ptr == NULL)
  {
    RF_MSG_1(RF_ERROR,"Invalid device: %d RFC device info not found ", device);
    return(FALSE);
  }

  *tx_device_ptr = (rfdevice_rxtx_common_class*)rfc_cmn_ptr->get_cmn_rf_device_object(RFDEVICE_TRANSCEIVER,
                   tx_device_info_ptr->rf_asic_info[0].instance);

  cfg.band = (int)band;
  cfg.rx_tx = RFC_CONFIG_TX;
  cfg.logical_device = device;
  cfg.req = RFC_REQ_ENABLE_XPT_CAPTURE;

  rfc_cdma_data_ptr->devices_cfg_data_get(&cfg, &fb_device_info_ptr);

  if (fb_device_info_ptr == NULL)
  {
    RF_MSG_1(RF_ERROR,"Invalid FB device: %d RFC device info not found ", 
             device_fb);
    return FALSE;
  }

  *fb_device_ptr = (rfdevice_rxtx_common_class*)rfc_cmn_ptr->get_cmn_rf_device_object(RFDEVICE_TRANSCEIVER,
                   fb_device_info_ptr->rf_asic_info[0].instance);


      return TRUE;
    }


/*----------------------------------------------------------------------------*/
/*!
  @brief
  This API gets the RFFE device information based on rfm device and device type for CDMA bands

  @params device
  RFM device

  @params band
  RFM CDMA band

  @params rffe_device_type
  RFFE device type

  @params mfg_id
  Passing in parameter to store manufacture id

  @params prd_id
  Passing in parameter to store product id

  @params rev_id
  Passing in parameter to store revision id

  @return
  TRUE on success and FALSE on failure

*/
boolean rfc_cdma_card_intf::get_rffe_device_info
(
    rfm_device_enum_type rfm_dev,
    rfm_cdma_band_class_type band,
    rfdevice_type_enum_type rffe_device_type,
    uint16 *mfg_id,
    uint16 *prd_id,
    uint16 *rev_id
)
{
    boolean  status = TRUE ;
    rfc_cdma_logical_device_params_type* logic_dev;
    rfc_cdma_card_intf *rfc_obj = rfc_cdma_card_intf::get_instance();
    /* PA device */
    cdma_pa * pa_device;
    /*ASM device*/
    rf_cdma_asm * asm_device;
    /*PAPM device*/
    rfdevice_class *papm_obj;

    rfdevice_class *device_obj;

    /*temporary vairable for IDs*/
    uint16 mfg_tmp;
    uint16 prd_tmp;
    uint16 rev_tmp;

    /*check the pass in paramters*/
    if ( (rfm_dev < RFM_DEVICE_0) || (rfm_dev >= RFM_INVALID_DEVICE) )
    {
      RF_MSG_1( RF_ERROR, "rfc_cdma_card_intf::get_rffe_device_info : Invalid device %d", rfm_dev);
      status &= FALSE;
    }

    if ( (band < RFM_CDMA_BC0) || (band >= RFM_CDMA_MAX_BAND) )
    {
      RF_MSG_1( RF_ERROR, "rfc_cdma_card_intf::get_rffe_device_info : Invalid band %d", band);
      status &= FALSE;
    }

    if ( (rffe_device_type < RFDEVICE_TRANSCEIVER) || (rffe_device_type >= RFDEVICE_TYPE_MAX_NUM) )
    {
      RF_MSG_1( RF_ERROR, "rfc_cdma_card_intf::get_rffe_device_info : Invalid device type %d", rffe_device_type);
      status &= FALSE;
    }

    if ((rffe_device_type == RFDEVICE_PA) && (is_rfm_device_tx ( rfm_dev ) != TRUE)) 
    { 
      RF_MSG_1( RF_ERROR, "rfc_cdma_card_intf::is_rfm_device_tx : It is NOT Tx device %d", rfm_dev); 
      status &= FALSE; 
    }

    /*get logic device*/
    if((logic_dev = rfc_obj->get_logical_dev_param(rfm_dev)) == NULL)
    {
        RF_MSG( RF_ERROR, "rfc_cdma_card_intf::get_rffe_device_info, get logic device failed !!");
        status &= FALSE;
    }

    /*if it pass the basic sanity check*/
    if(status == TRUE)
    {
        switch(rffe_device_type)
        {
            case RFDEVICE_PA:
            {
                /*get PA device object*/
                pa_device = logic_dev->pa[band];
                device_obj = (rfdevice_class*)(pa_device->get_misc_device_descriptor());

                if(device_obj != NULL)
                {
                    /*get the PA IDs*/
                    if(device_obj->get_device_info(&mfg_tmp, &prd_tmp, &rev_tmp) == FALSE)
                    {
                        RF_MSG(RF_ERROR,"rfc_cdma_card_intf::get_rffe_device_info, get PA IDs failed");
                        status &= FALSE;
                    }
                    else
                    {
                        /*fill up the passing in ID arrays*/
                        mfg_id[0] = mfg_tmp;
                        prd_id[0] = prd_tmp;
                        rev_id[0] = rev_tmp;
                    }
                }
                else
                {
                    RF_MSG( RF_ERROR, "rfc_cdma_card_intf::get_rffe_device_info, get PA device failed !!");
                    status &= FALSE;
                }
            }

            break;

            case RFDEVICE_ASM:
            {
                /*get ASM device object*/
                asm_device = logic_dev->sm[band];

                if(asm_device != NULL)
                {
                    status &= asm_device->get_asm_device_info(mfg_id,prd_id,rev_id);
                }
                else
                {
                    RF_MSG( RF_ERROR, "rfc_cdma_card_intf::get_rffe_device_info, get ASM device failed !!");
                    status &= FALSE;
                }
            }

            break;


       case RFDEVICE_PAPM:
       {
         if((rfdevice_class*)logic_dev->qpoet_obj[band] != NULL)
         {
         /*get PAPM device object*/
         papm_obj = (rfdevice_class*)logic_dev->qpoet_obj[band];

         if(papm_obj != NULL)
         {
           status &= papm_obj->get_device_info(mfg_id,prd_id,rev_id);
         }
         else
         {
           RF_MSG( RF_ERROR, "rfc_cdma_card_intf::get_rffe_device_info, get PAPM device failed !!");
           status &= FALSE;
         }
        }

       }
       break;

            default:
                RF_MSG(RF_ERROR,"rfc_cdma_card_intf::get_rffe_device_info, invalid rffe_device_type");
                return FALSE;
        }
    }

    return status;

}


/*----------------------------------------------------------------------------*/
/*!
  @brief
  This API queries to see if the given RFM device is Rx.

  @params device
  RFM device in question.

  @return
  TRUE on if RFM device is Rx and FALSE otherwise

*/
boolean
rfc_cdma_card_intf::is_rfm_device_rx ( rfm_device_enum_type device )
{
  boolean is_rx = FALSE ;
  if ( device < RFM_MAX_WAN_DEVICES )
  {
    is_rx = is_rfm_device_rx_tx [device][RFC_CONFIG_RX] ;
  }
  return is_rx ;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This API queries to see if the given RFM device is Tx.

  @params device
  RFM device in question.

  @return
  TRUE on if RFM device is Tx and FALSE otherwise

*/
boolean
rfc_cdma_card_intf::is_rfm_device_tx ( rfm_device_enum_type device )

{
  boolean is_tx = FALSE ;
  if ( device < RFM_MAX_WAN_DEVICES )
  {
    is_tx = is_rfm_device_rx_tx [device][RFC_CONFIG_TX] ;
  }
  return is_tx ;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  API to update RFC information for the alternate path

  @param device
  device which needs update

  @param band
  The current band

  @param alt_path
  the alternate path recommended by concurrency manager

  @return
  TRUE: success
  FALSE: failure

*/
boolean
rfc_cdma_card_intf::update_rx_path
(
  rfm_device_enum_type device,
  rf_card_band_type curr_band,
  uint8 alt_path
)
{
  rfc_cfg_params_type band_path_cfg ;
  boolean success = TRUE ;

  /* For CDMA alternate path is 0. THis may need to be changed later */
  band_path_cfg.alternate_path = alt_path ;
  band_path_cfg.logical_device = device ;
  band_path_cfg.req = RFC_REQ_DEFAULT_GET_DATA;

  /* Set Rx device */
  band_path_cfg.rx_tx = RFC_CONFIG_RX;
  band_path_cfg.band =  (int)curr_band ;

  /* Get WTR, ASM, Tuner, RX related FE elements */
  success &=
    recreate_rx_chain_with_updated_alt_path ( device ,
                                              band_path_cfg ,
                                              alt_path ) ;

  if ( success == FALSE )
  {
    RF_MSG ( RF_ERROR, "rfc_cdma_card_intf::update_rx_path "
                       " API failed" );
  }

  return success ;
}/* rfc_cdma_card_intf::update_rx_path */



/*----------------------------------------------------------------------------*/
/*!
  @brief
  API to retrieve the Rx path for the given device.

  @param device
  device which needs update

  @return uint8
  Returns the path stored in the CDMA data structure in turn
  queried by the RF concurrency manager.

*/
uint8
rfc_cdma_card_intf::get_current_rx_path
(
  rfm_device_enum_type      device ,
  rfm_cdma_band_class_type  band
)
{
  uint8 curr_path = ((uint8)~0) ;
  if ( ( device < RFM_MAX_WAN_DEVICES ) &&
       ( band < RFM_CDMA_MAX_BAND ) )
  {
    curr_path = current_path[device][band] ;
  }
  else
  {
    RF_MSG_3 ( RF_ERROR, "rfc_cdma_card_intf::get_current_rx_path() "
             "Bad Device[%d] or Bad Band[%d], Returning %d as current_path " ,
             device , band , curr_path ) ;
  }

  return curr_path ;
}

/*! Default Setter method for current path per RFM device for
  all bands */
void
rfc_cdma_card_intf::rfc_cmda_set_current_rx_path_for_device
(
  uint8 rfm_device
)
{
  int32 band = 0 ;

  for ( band = (int32)RFM_CDMA_BC0 ;
        band < (int32)RFM_CDMA_MAX_BAND ; band++ )
  {
    current_path[rfm_device][band] = 0 ;
  }
}

/*! Setter method for current path per RFM device */
void
rfc_cdma_card_intf::rfc_cmda_set_current_rx_path_for_device
(
  uint8 rfm_device ,
  rfm_cdma_band_class_type new_band ,
  uint8 curr_path
)
{
   current_path[rfm_device][new_band] = curr_path ;
}

/*! get method for feedback RX modem chain */
boolean 
rfc_cdma_card_intf::get_fb_device_modem_chain
(
  rfm_device_enum_type device_fb,
  rfm_cdma_band_class_type band, 
  uint32 *fbrx_chain
)
{
  boolean retval = FALSE;
  rfc_cfg_params_type cfg;
  rfc_device_info_type *device_info_ptr = NULL;
  rfc_cdma_data *rfc_cdma_data_ptr = NULL;

  /* Init the modem chain to a default value in case this 
   * function fails 
   * */
  *fbrx_chain = (uint32) (~0) ;

  rfc_cdma_data_ptr = rfc_cdma_data::get_instance();

  cfg.logical_device = device_fb;
  cfg.alternate_path = RFC_CDMA_DEFAULT_PATH;
  cfg.band = (int)band;
  cfg.rx_tx = RFC_CONFIG_TX;
  cfg.req = RFC_REQ_ENABLE_XPT_CAPTURE;

  rfc_cdma_data_ptr->devices_cfg_data_get(&cfg, &device_info_ptr);

  if (device_info_ptr == NULL)
  {
    RF_MSG_1(RF_ERROR,"Invalid device: %d device object not found ", device_fb);
  }
  else
  {
    *fbrx_chain = device_info_ptr->modem_chain;
    retval = TRUE;
  }
  return retval;
}

/*! Return whether a given nv path is valid for a given band */
boolean
rfc_cdma_card_intf::is_nv_path_valid
(
   rf_cdma_nv_path_type nv_path,
   rfm_cdma_band_class_type rf_band
)
{
  boolean path_valid = FALSE;

  if ( valid_nv_path_array != NULL )
  {
    if ( nv_path < RF_CDMA_NV_PATH_NUM && rf_band < RFM_CDMA_MAX_BAND ) 
    {
      path_valid = valid_nv_path_array[nv_path][rf_band];
    }
  }
  else
  {
    RF_MSG(RF_HIGH,"is_nv_path_valid: NULL RFC ptr");
  }

  return path_valid;
} /* rfc_cdma_card_intf::is_nv_path_valid */


/*! Default Setter method for current rx_mdsp_path per RFM device for
  all bands */
boolean
rfc_cdma_card_intf::set_rx_mdsp_path
(
  rfm_device_enum_type device,
  rf_cdma_mdsp_chain_enum_type rx_mdsp_path
)
{
  boolean api_status = TRUE;

  if ( device < RFM_MAX_WAN_DEVICES )
  {
    if ( ( logical_dev_param_ptr[device]->rx_mdsp_path == RF_CDMA_MDSP_CHAIN_INVALID && 
           rx_mdsp_path != RF_CDMA_MDSP_CHAIN_INVALID ) ||
         ( logical_dev_param_ptr[device]->rx_mdsp_path != RF_CDMA_MDSP_CHAIN_INVALID && 
           rx_mdsp_path == RF_CDMA_MDSP_CHAIN_INVALID ) )
    {
      logical_dev_param_ptr[device]->rx_mdsp_path = rx_mdsp_path;
    }
    else
    {
      RF_MSG_3(RF_ERROR,"rfc_cdma_card_intf::set_rx_mdsp_path: "
                        "device %d attempts to override valid rx_mdsp_path %d with"
                        "%d. Invalid call flow.", device, 
               logical_dev_param_ptr[device]->rx_mdsp_path, rx_mdsp_path );
      api_status = FALSE;
    }

  }
  else
  {
    RF_MSG_1(RF_ERROR,"rfc_cdma_card_intf::set_rx_mdsp_path: invalid device %d", device );
    api_status = FALSE;
  }

  return api_status;
}


#endif /* FEATURE_CDMA1X */

/*! @} */
