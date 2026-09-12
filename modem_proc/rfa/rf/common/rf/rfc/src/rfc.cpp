/*!
   @file
   rfc_card.cpp

   @brief
   This file contains implementation the RFC container class
   The RFC container card is an abstract class that acts as an interface to
   abstract different RF cards the system may support

   The RFC container class is technology independent can only contains
   attributes of a card.

   The implementation aspects of this file contains only the following:
   1. Constructor
   2. Destructor
   3. Get_Instance for the singleton

*/

/*===========================================================================

  Copyright (c) 2011-2020 Qualcomm Technologies Incorporated. All Rights Reserved

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

$DateTime: 2022/09/02 04:30:25 $ $Author: mplcsds1 $
$Header: //components/rel/rfa.mpss/4.11/rf/common/rf/rfc/src/rfc.cpp#2 $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
09/02/22   tej     FR69305 To add support for new error codes for QMI_DMS_SET_OPERATING_MODE
08/07/20   akm     irat restrictions on aggati qrd card
04/10/20   akm     irat restirctions on habanero qrd card
03/06/20   nik     Heap clade2 changes
08/16/17   Om      Add RFFE V2 support
11/17/15   ndb     Move the Dummy tuner manager creation outside of ASDIV
11/30/15   ry      RFA common support changes for LPM FR
11/06/15   hbl     [Ported]Change IRAT alt path lookup implementation
10/11/15   npi     iRAT restriction band mask support
09/27/15   sr      Added Support for 3rd party grfc xsw and asdiv based ASM devices 
09/25/15   hm      Added Support for mipi tuner, 3rd party cxw and asdiv based ASM devices 
09/03/15   aak     Support for TP Tuner manager
09/02/15   Saul    Stop RFC init if errors with device tables
09/01/15   zhw     Added Dev 4/5 support in RFC Common Dev Param
08/08/15   yb      ACL Feature
07/30/15   hzh     Product_rev msb recover during logical device construction.
07/29/15   hm      GRFC tuner interface support
07/24/15   dyc     Un-paired (dummy) tuner support with update messages.
07/23/15   dbc     Create wlan_rfc while RFFE bus is still enabled
07/09/15   aak     Set XO drive strength only for RF_CLK1 for TP RF cards
07/20/15   Saul    Provide dynamic props only if card init passed.
07/14/15   Saul    Access rfc device properties using ptr.
07/08/15   Saul    RF Device Max Bandwidth Interface
06/25/15   px      Added check to verify logical device creation
06/19/15   yb      Default to AOL manager for legacy CL
06/08/15   Saul    RFC Dynamic Properties
03/03/15   vv      Added rfc support for ADC VCM Cal feature
06/01/15   zhw     Added support for RFC preferred device query
05/29/15   Saul    Call generate_rfcard_dynamic_properties at card creation.
05/22/15   fhuo    Added support for WLAN
05/26/15   Saul    RFC Dynamic Properties API
04/30/15   Saul    CRAT Updates
02/20/15   px      Added rffe scan EFS dump feature for factory debug tool
01/28/15   ndb     Added the case statement in RFDEVICE_ANTENNA_TUNER_OL_PLUS_CL_FBRX_ALGO
01/05/15   ndb     Added Third party Coupler 
01/06/15   vv      Added support for trx to phy_path & adc(modem_chain) mapping 
12/12/14   vv      Added 3rd party external LNA(GEN_LNA) device support
12/10/14   ndb     Added FBRX based CL enums
11/18/14   dyc     Support updating rfc_asdiv_config_info table from RFC info
10/31/14   vr      Featurized the CDMA tech object creation to avoid bootup
                   crash on 1x compiled out build
10/29/14   vb      Wrapper API to get a device object for the device type and
                   instance provided
10/21/14   px      Added RFM Device support for PAPM
09/29/14   Saul    New alt path selection APIs.
09/11/14   aak     Remove featurization for MDM9x45
09/02/14   vv      Wrapper api to create 3rd party physical devices.
                   Update logical device status for missing physical devices
                   Store physical device config info in physical device array
08/15/14   st      LTE SCELL Delete CA Reconf Requirement - Handle Alt Path 0 case
08/13/14   tks     Modified get alt path table function param list
08/12/14   aak     Move PAM calls for LTE light sleep to the vreg manager
08/12/14   sb      Set XO drive strength based on the WTRs present in an RFC
08/04/14   tks     Fixed incorrect return values in update port map function
07/31/14   tsr     GSM Alternate path support
07/22/14   tks     Removed un-used functions
07/17/14   spa     Update cdma alt rx path during updating port mapping
07/14/14   vrb     Support for Alternate Path Selection Table
07/09/14   aak     Changes for API to control MPP for DAC Vref
06/25/14   vv      Enabled physical device creation for all devices during bootup
06/16/14   tks     Added support for tdscdma & wcdma in update port mapping
06/09/14   vv      Enable physical device creation for QFE devices with new physical/logical device config structures
05/19/14   Saul    Bypass device object detection for RFDEVICE_BUFFER
05/09/14   krg     Force offline mode for missing devices
05/02/14   spa     Propagate CDMA RFC failure up to RFM init
04/24/14   tks     Fix klocwork error
04/17/14   kg      Temp change to remove the rfc_init faliure for alternate parts
04/09/14   zhw     Remove seperate code for creating TRX HDET objects.
                   Return NULL for dummy tuner if ASDIV not defined
03/17/14    kg     Port from Dime
02/11/14    vb     Support for Dummy Tuner Manager
02/11/14  sr/as    Re-design asdiv_tuner to support multiple RF configurations
03/12/14   sbm     Bug fix: hdet autocal NV write fix for wtr devices.
01/16/14   ndb     Added Daughter Card support(NON_FATAL_DEVICES_SUPPORT).
01/06/14   adk     Added support for antenna tuners
11/18/13   shb     GNSS fix
11/05/13   adk     Added support for extensions to NV item RFNV_ATUNER_ALGO_TYPE
10/31/13   shb     Switch to C++ TRX common interface
10/03/13   APU     Add support for getting new common Tx LUT.
09/22/13   jr      Added trigger command to update the PAPM device disable sequence
09/16/13   hm      hooking up WCDMA C++ device driver interface for WTR
09/04/13   pl      Fix stack overflow
08/29/13   pl      Create ASD_TUNER_MANAGER if and only if tuner is defined, tuner manager is created
                   and antennna switch diversity is enabled
08/26/13   jr      Added FTM_GET_EFS_VERIFICATION_DATA
07/16/13  ndb      Added "FEATURE_RF_HAS_QTUNER" for the  creation of tuner manager
07/16/13   ndb     NV based & HW based tuner manager creation support
06/29/13   hm      Add new FTM command for tuner self cal: "FTM_DO_TUNER_SELF_CAL"
05/17/13   kai     Add new api to get cmn device properties
05/02/13   aca     Update physical device cal exit condition
05/01/13   shb     Pass efs_file_path string to legacy device factory
04/25/13   shb     Remove special handling of PDET DC cal for WTR1625 as it is
                   managed in EFS memory inside device
04/18/13   bmg     Physical device self-cal load and store.
04/08/13   bmg     Add support for keeping an array of physical devices to
                   represent chips in the card.  Try to create a physical
                   device for a chip before using the component base class
                   factory.  Helper function to find an instance of a physical
                   device for components that refer to "associated" devices.
03/27/13   zhw     1x prep_/exec_wakeup time update
03/22/13   sr      changes to make the interfaces consistent with other devices.
03/21/13   shb     Updated internal device cal function to handle WTR1625
                   PDET DC cal requirements
03/11/13   sar     Updated file for APQ, GNSS only target.
03/14/13   sar     Fixed compiler errors in APQ target.
03/07/13   sr      changed the "pwr_tracker" name to "papm".
03/06/13   vrb     RFC 2.13.132 requirement: populate req enum in RFC API
03/06/13   sr      fixed KW errors.
03/05/13   sr      Added self_test support.
02/22/13   dw      Support for FTM common Cal routines without set mode
02/21/13   aca     Support for WTR query using DAC
02/21/13   zhw     Reverting 1x warm up time to old values
02/21/13   sr      Added support to return WTR device for the tx dac.
02/09/13   sr      free the memory allocated for rfc_data and vreg_mgr to avoid
                   memory leaks in off-target test mode.
02/06/13   sty     Changed prep_ and exec_wakeup times for 1x
02/05/13   vb      Added support for creation of Tuner manager in RFC
02/05/13   pv      Support for Dime Interface and CA for Device manager
01/24/13   sr      corrected mis-leading error message.
01/18/13  shb/vrb  Disable QPOETs after all USIDs have been programmed
01/17/13   sr      changes to assign USID and then create device objects
01/14/13   cd      Updated Rx wakeup settling time to match worst case time for
                   Rx wakeup script
01/11/13   shb     Fixed bug in do_internal_device_calibration to write RC
                   tuner correctly for cards with 2 Transceivers
01/10/13   dbz     KW error fix
01/07/13   shb/dbz Create GNSS device in create_gnss_rfc
                   Set dev_param[RFM_DEVICE_GNSS] based on rfc ag data instead
                   of hardcoding to common device 0
12/21/12   sr      klockwork error fix.
11/27/12   sr      Removed the err_fatal() to avoid crash.
11/26/12   sr      Added more error checking to avoid crashes.
11/19/12   aki     Added more cleanup code to fix OFT memory leaks
10/30/12   sr      warnings fix.
10/30/12   sr      changes create tech rfc object if the tech is supported by
                   the specific rf-card.
10/17/12   kai     Removed qfe_cmn_device
09/06/12   gh      Wrap QFE tuner and hdet operations in feature
09/06/12   gh      Initialize all dev_param members to NULL in constructor
10/08/12   dbz     Created GPS device
10/03/12   jyu     Fixed crash in create_cdma_rfc()
09/24/12   sty     Updated call to create_device_instance() for CDMA
09/24/12   sr      Made changes to remove hand-coded RFC files.
08/03/12   sr      initialized the rf_devices[] ptrs .
07/18/12   sr      Changes to create devices through device factory.
07/09/12   aak     Move Quiet mode API from device to RFC
05/23/12   sr      Moved the generic code to rfc base-class.
05/18/12   sr      Made chanegs to derive all the rfc classes from rfa class.
04/18/12   swb     Override new operator to allocate memory from MODEM RFA
02/28/12   sr      made changes to put all the GPIOs into low power mode during sleep.
01/16/12   pv      RF Common Core Device Manager Implementation.
09/27/11   aro     Added interface to load int device calibration data
09/19/11   aro     Added common RFC interface to perform target specific
                   RC Tuner calibration
09/15/11   sar     Fixed compiler warning.
09/14/11   sar     Fixed KW errors.
09/14/11   aro     Added interface to query the common logical device
                   parameter
08/24/11   aak     Changes to enable NPA voting for resources from RF SW
08/18/11   dw      Added rfc_command_dispatch
08/18/11   zg      Added TDSCDMA RFC support.
08/15/11   aak     New RFC interface for rfc_get_awake_resource_request()
08/05/11   sr/plim Old RFC cleanup.
06/30/11   cd      Added RFC common interface to get devices configuration
                   from card specific instance and return to RFM layer
06/20/11   cd      Delete GNSS instance in RF card interface destructor
06/07/11   shb     Created rfc_init_status flag as private member of rfc_intf
                   base class. Added accessor functions: protected method
                   set_init_status() and public method get_init_status()
03/23/11   dw      Use get_instance() for WCDMA
02/18/11   sty     Renamed getinstance to get_instance
02/18/11   sr      changed to call get_instance() for gsm rfc.
02/17/11   sty     Renamed rfc_cdma.h to rfc_card_cdma.h
02/03/11   pl      Adding a member function to allow gpio configuration of
                   GPIO.
02/02/11   pl      Remove DALTlmm_GpioConfig from constructor. GPIO needs to be
                   configured during enter_mode().
01/20/11   sty     Renamed rfc_cdma_card_interface to rfc_cdma_card_intf
01/19/11   sty     Replaced ptrQ with getinstance
01/15/11   pl      Initial version.

============================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
extern "C" {
#include "rfa_variation.h"
#include "rfc_card.h"
#include "rfcommon_nv_mm.h"
#include "rfcommon_nv.h"
#include "task.h"
#include "fs_lib.h"
}

#include "rfcommon_atuner_manager_factory.h"
#include "rfc_class.h"
#include "pm_clk.h"
#include "rfcommon_msg.h"
#include "modem_mem.h"
#include "rfc_vreg_mgr.h"
#include "rfc_common_data.h"
#include "rfdevice_rxtx_common_class.h"
#include "rf_device_factory.h"
#include "rfcommon_efs.h"
#include "stringl.h" /* memscpy */

/*To create third party PA,ASM devices with physical device interface support*/
#include "rfdevice_pa_common.h"
#include "rfdevice_asm_common.h"

#ifndef FEATURE_GNSS_ONLY_NO_WWAN
#include "rfc_cmn_transceiver_hdet_adapter.h"
#endif /* FEATURE_GNSS_ONLY_NO_WWAN */

#ifdef FEATURE_LTE
#include "rfc_lte.h"
#include "rfc_lte_data.h"
#include "rfdevice_lte_interface.h"
#endif

#ifdef FEATURE_GSM
#include "rfc_gsm.h"
#include "rfc_gsm_data.h"
#include "rfdevice_gsm_type_defs.h"
#include "rfdevice_gsm_intf.h"
#endif

#ifdef FEATURE_WCDMA
#ifdef ATLAS_RF_WCDMA_PRESI
#include "rfc_wcdma_card.h"
#else
#include "rfc_wcdma.h"
#endif
#include "rfc_wcdma_data.h"
#include "rfdevice_wcdma_type_defs.h"
#include "rfdevice_trx_wcdma_rx.h"
#include "rfdevice_wcdma_intf.h"
#endif

#if defined(FEATURE_CDMA1X) || defined(FEATURE_HDR)
#include "rfc_card_cdma.h"
#include "rfc_cdma_data.h"
#include "rfdevice_cdma_interface.h"
#endif

#ifdef FEATURE_CGPS
#include "rfc_gnss.h"
#include "rfc_gnss_data.h"
#endif

#include "rfc_wlan.h"
#include "rfc_wlan_data.h"

#ifdef FEATURE_CLADE_HEAP
#include "clade_heap.h"
#endif

#ifdef FEATURE_TDSCDMA
#include "rfc_tdscdma.h"
#include "rfc_tdscdma_data.h"
#include "rfdevice_tdscdma_type_defs.h"
#include "rfdevice_tdscdma_intf.h"
#endif

#ifdef FEATURE_RF_ASDIV
#include "rfcommon_asdiv_tuner_manager.h"
#endif
#include "rfdevice_antenna_tuner.h"

extern "C" {
#include "DDITlmm.h"
}

/*!
  @brief
  Maximum size of the path buffer used to construct physical device self-
  calibration file names.

  @details
  This path length must be large enough to contain the longest string
  expected to be generated by the @ref RFC_PHYS_DEVICE_SELF_CAL_PATH_FMT_STR
  format specifier string.
*/
#define MAX_RFC_SELFCAL_PATH_LENGTH 40

/*!
  @brief
  Sprintf-style format specifier string to create file paths to chip
  self-cal data.

  @details
  This format specifier string must fit into @ref MAX_RFC_SELFCAL_PATH_LENGTH
  - 1 characters.

  The first parameter is the RF card numerical identifier.

  The second parameter is the device number, starting from 0.
*/
#define RFC_PHYS_DEVICE_SELF_CAL_PATH_FMT_STR "/rfc/%04d/selfcal/dev%d"

/*!
  @brief
  Settings to customize RFFE Scan EFS Dump format and location
*/
#define RFC_RFFE_SCAN_VERSION         2
#define RFC_RFFE_SCAN_HEAD_SIZE       7
#define RFC_RFFE_SCAN_BODY_SIZE       11
#define RFC_RFFE_SCAN_PATH            "rfc/rffe_scan/rffe_scan.dat"

#define RFC_RFFE_SCAN_VERSION_IDX     0
#define RFC_RFFE_SCAN_HEAD_SIZE_IDX   1
#define RFC_RFFE_SCAN_BODY_SIZE_IDX   2
#define RFC_RFFE_SCAN_RFHW_ID_IDX     3
#define RFC_RFFE_SCAN_NUM_DEVICES_IDX 4
#define RFC_RFFE_SCAN_NUM_PRESENT_IDX 5
#define RFC_RFFE_SCAN_ERROR_CODE_IDX  6

#define RFC_RFFE_SCAN_ERROR_RFC_DATA_MISSING 1
#define RFC_RFFE_SCAN_ERROR_DEV_CFG_MISSING 2
#define RFC_RFFE_SCAN_ERROR_RFC_INIT_FAILED 3

extern rfdevice_class* rf_device_factory_create_device(rfc_device_cfg_info_type* cfg);
extern boolean rf_device_factory_program_device_id(rfc_device_cfg_info_type* cfg);
extern rfdevice_logical_component* create_gen_device_object(
                                                            rfdevice_physical_device *rfdevice_physical_third_party_p,
                                                            rfc_logical_device_info_type *logical_device_cfg
                                                            );

/* To Store flag to indicate the rfc init result */
rfm_init_status_type_e_type cmw_rfc_init_status;

typedef struct
{
  uint8 rat_mask_idx;
  uint32 device_mask;
} rfc_rat_restrict_helper_type;

// Keeps track of device masks and mask index location in final array for each instance
rfc_rat_restrict_helper_type rfc_rat_restrict_info_helper[ RFC_MAX_DEVICE_INSTANCES ];

rfc_intf *rfc_intf::rfc_p = (rfc_intf *)NULL;

// Singleton GetInstance method to instantiate Object
rfc_intf *rfc_intf::get_instance(void)
{
  return rfc_p;
}

// Constructor
rfc_intf::rfc_intf(rf_hw_type rfhw, system_clock_enum sysclk)
{
  uint8 temp_rfm_dev_val = RFM_INVALID_DEVICE;
  uint8 dev_type = 0;
  uint8 instance = 0;
  uint8 antenna = 0;
  uint8 txdac = 0;
  rfc_signal_info_type *rfc_info_table = NULL;
  uint32 rfc_signal_num = 0;
  boolean init_status = FALSE;
  rfc_common_data *rfc_data = NULL;
  rfdevice_rxtx_common_class *transceiver_cmn_device = NULL;
  memset(trx_phy_path_adc_mapping, 0xFF, RXLM_CHAIN_MAX*sizeof(trx_phy_path_adc_mapping_type));
  memset(&rfcard_dynamic_properties, 0x0, sizeof(rfcard_dynamic_properties));
  memset(rfc_rat_restrict_info_helper, 0x0, sizeof(rfc_rat_restrict_info_helper));
  rfc_dynamic_properties_valid = TRUE;

#ifdef FEATURE_RF_ASDIV
  rfcommon_asdiv_tuner_manager *asd_tuner_manager_device = NULL;
#endif

  // make sure we are not going to create this rf_card again. This is a singleton class.
  if (rfc_intf::rfc_p != NULL)
  {
    ERR_FATAL("RFC ERROR Re-creating the Common rf_card Object which is not allowed",0,0,0);
  }

  rfc_card_id=rfhw; 
  phys_devices_count=0;
  logical_devices_count=0;
  phys_devices_array=NULL; 
  sys_clk=sysclk;

  /* Call rfc_intf::set_init_status() to set init flag */
  set_init_status(init_status);

  /* initialize the rfc common object ptr before initializing the tech RFCs */
  rfc_intf::rfc_p = this;

  for (temp_rfm_dev_val = RFM_DEVICE_0; temp_rfm_dev_val < RFM_MAX_DEVICES;
       ++temp_rfm_dev_val)
  {
    /* To map the logical Devices to NULL or No phyiscal chip in Constructor
    This gets updated in the RFC later when the derived class is instantiated */

    dev_param[temp_rfm_dev_val].cmn_device = NULL;

#ifdef FEATURE_RF_HAS_QTUNER
    /* Initialize device ptrs to NULL */
    dev_param[temp_rfm_dev_val].qfe_tuner_device = NULL;
    dev_param[temp_rfm_dev_val].qfe_hdet_device = NULL;
#endif
  }

  // initialize the cmn_rf_devices[][]
  for (dev_type = 0; dev_type < RFDEVICE_TYPE_MAX_NUM; dev_type++)
  {
    for (instance = 0; instance < RFC_MAX_DEVICE_INSTANCES; instance++)
    {
      this->cmn_rf_devices[dev_type][instance].device_obj = NULL;
      this->cmn_rf_devices[dev_type][instance].device_status = RFC_DEVICE_INVALID_STATUS;
    }
  }

  /* initialize the trx device ptr to NULL */
  for (txdac = 0; txdac < TXLM_DAC_NUM; txdac++)
  {
    this->txdac_trx_device[txdac] = NULL;
  }

  /* initialize Dummy Tuner Mgr per antenna */
  for (antenna = 0; antenna < RFC_MAX_ANTENNA; antenna++)
  {
    dummy_tuner_mgr[antenna] = NULL;
  }

  /* Create rfc common AG data object */
  rfc_data = rfc_common_data::get_instance();
  if (rfc_data == NULL)
  {
    ERR_FATAL("RFC ERROR Cannot Create RFC COMMON DATA Object",0,0,0);
  }

  // Retrive RF_GRFC/GRFC/GPIO table
  rfc_signal_num = rfc_data->sig_info_table_get(&rfc_info_table);

  // init status depends on RF_GRFC/GRFC/GPIO initialization result;
  // All GPIO's are disabled after initialization; use rfc_common_wakeup
  // to wakeup GPIO's.
  init_status = rfc_common_init(rfc_info_table, rfc_signal_num);

  /* Enable VREGs in autocal mode BEFORE calling WTR1605 constructor so
  that WTR1605 can communicate with the chipset (say for process/revision
  detection) during initialization. This is a requirement of WTR1605
  device constructor to have VREGs and A0 buffer ON for SSBI communication
  with device. In addition to the VREGS, devices also need RFC_SSBI, RFC_RFFE GRFC's
  to be turned on.*/
  rfc_common_enable_autocal_vregs(TRUE);

  /* Create WTR1605 common device. WTR1605 will return a common device
  pointer which will be used for tech specific device creations. If a
  common interface is to be used, this pointer should be saved in RFC
  for future use. For now, it is used only for device configuration */

  // create all the rf device objects based on the device configuration data
  init_status &= create_cmn_rf_devices();
  
  /* create wlan RFC now so that when the WLAN port is pushed to the device
  the ASM switch can be programmed at boot up to the WLAN position */
  create_wlan_rfc();

  /* create ASDiv Tuner Manager */
  init_status &= create_asdiv_tuner_mgr();
  /*Disable vregs after device creation is completed.*/
  rfc_common_enable_autocal_vregs(FALSE);

  /* Updated dev_param[] for common rfc */
  /* @TODO the following is a hack. dev_param cannot be used anymore.
   *       RFM device to common device instance needs to come from RFC
   *       and is dynamic
   */
  if(cmn_rf_devices[RFDEVICE_TRANSCEIVER][0].device_obj != NULL)
  {
    transceiver_cmn_device = (rfdevice_rxtx_common_class*)cmn_rf_devices[RFDEVICE_TRANSCEIVER][0].device_obj;
    dev_param[RFM_DEVICE_0].cmn_device = transceiver_cmn_device;
    dev_param[RFM_DEVICE_1].cmn_device = transceiver_cmn_device;
    dev_param[RFM_DEVICE_3].cmn_device = transceiver_cmn_device;
    dev_param[RFM_DEVICE_GPS].cmn_device = transceiver_cmn_device;
  }
  else
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR, "NULL Common device obj on instance 0",0);
  }

  if(cmn_rf_devices[RFDEVICE_TRANSCEIVER][1].device_obj != NULL)
  {
    transceiver_cmn_device = (rfdevice_rxtx_common_class*)cmn_rf_devices[RFDEVICE_TRANSCEIVER][1].device_obj;
    dev_param[RFM_DEVICE_2].cmn_device = transceiver_cmn_device;
    dev_param[RFM_DEVICE_3].cmn_device = transceiver_cmn_device;
    /* Logical Device 4 & 5 are mapped to the second WTR device. This is hard-coded
    because there is no Device to WTR Instance translation table available yet.*/
    dev_param[RFM_DEVICE_4].cmn_device = transceiver_cmn_device;
    dev_param[RFM_DEVICE_5].cmn_device = transceiver_cmn_device;
  }


  create_lte_rfc();

  create_gsm_rfc();

  create_tdscdma_rfc();

  create_wcdma_rfc();

  #if defined(FEATURE_CDMA1X) || defined (FEATURE_HDR)
  create_cdma_rfc();
  #endif

  create_gnss_rfc();

#ifdef FEATURE_RF_ASDIV
  /* process the asdiv_config_info table after it has been populated
   * with devices after each tech rfc instance
   */
  asd_tuner_manager_device = (rfcommon_asdiv_tuner_manager*)cmn_rf_devices[RFDEVICE_ASD_TUNER_MANAGER][0].device_obj;

  if( asd_tuner_manager_device != NULL )
  {
     if ( asd_tuner_manager_device->process_rfc_asdiv_config_info() == FALSE )
     {
         RF_MSG(RF_HIGH, "ASDiv: rfc_asdiv_config_info ptr is NULL");
     }
  }
#endif /* FEATURE_RF_ASDIV */

  /* All tech specific initializations successful,
     set init status flag to TRUE */
  set_init_status(init_status);

   if (init_status == FALSE)
   	{
	 if(cmw_rfc_init_status == RFM_INIT_FAILURE_NONE)
	   {
		  cmw_rfc_init_status = RFM_INIT_FAILURE_SW;
	   }
   	}
}

// Destructor
// The destructor must destroy all card of all tech
rfc_intf::~rfc_intf()
{
  #ifdef FEATURE_LTE
  delete rfc_lte_intf::Get_Instance();
  delete rfc_lte_data::get_instance();
  #endif

  #ifdef FEATURE_GSM
  delete rfc_gsm::get_instance();
  delete rfc_gsm_data::get_instance();
  #endif

  #ifdef FEATURE_WCDMA
  delete rfc_wcdma::get_instance();
  delete rfc_wcdma_data::get_instance();
  #endif

  #if defined(FEATURE_CDMA1X) || defined (FEATURE_HDR)
  delete rfc_cdma_card_intf::get_instance();
  delete rfc_cdma_data::get_instance();
  #endif

  #ifdef FEATURE_CGPS
  delete rfc_gnss::get_instance();
  delete rfc_gnss_data::get_instance();
  #endif


  delete rfc_wlan::get_instance();
  delete rfc_wlan_data::get_instance();



  #ifdef FEATURE_TDSCDMA
  delete rfc_tdscdma::get_instance();
  delete rfc_tdscdma_data::get_instance();
  #endif

  this->destroy_cmn_rf_devices();
  #ifndef FEATURE_CLADE_HEAP
  modem_mem_free(phys_devices_array, MODEM_MEM_CLIENT_RFA);
  #else
  clade_heap_free(phys_devices_array);      
  #endif
  
  delete rfc_common_data::get_instance();
  delete rfc_vreg_mgr::get_instance();
}

// API Implemenation
system_clock_enum rfc_intf::get_sys_clk_type(void)
{
  return (sys_clk);
}


boolean rfc_intf::get_init_status(void)
{
  return rfc_card_init_success;
}

void rfc_intf::set_init_status(boolean status)
{
  rfc_card_init_success = status;
}

rf_hw_type rfc_intf::get_rfc_hardware_id(void)
{
  return rfc_card_id;
}

/*----------------------------------------------------------------------------*/
/*
 *   @brief
 *     This funtion returns the total number of physical devices
 *     */
int rfc_intf::get_phys_devices_count(void)
{
    return this->phys_devices_count;
}

/*----------------------------------------------------------------------------*/
/*
 *   @brief
 *     This function returns a pointer to the rfc physical devices array
 *     */
rfc_physical_device_struct_type* rfc_intf::get_phys_devices_array(void)
{
    return this->phys_devices_array;
}

/*----------------------------------------------------------------------------*/
/*
  @brief
  This funtion returns the common device object ptr for the specified device type and instance
  number.

  @details
  This funtion returns the common device object ptr for the specified device type and instance
  number.

  @param
  dev_type : type of device
  instance : instance number for the device type.

  @return
  rfdevice_class * : device object ptr.

*/
rfdevice_class *rfc_intf::get_cmn_rf_device_object(rfdevice_type_enum_type dev_type, uint8 instance)
{
  if (dev_type >= RFDEVICE_TYPE_MAX_NUM || instance >= RFC_MAX_DEVICE_INSTANCES)
  {
    MSG_2(MSG_SSID_RF, MSG_LEGACY_HIGH, "rfc_intf::get_cmn_device_objects(%d, %d): Invalid parameters", dev_type, instance);
    return (rfdevice_class *)NULL;
  }

  return cmn_rf_devices[dev_type][instance].device_obj;
}

/*----------------------------------------------------------------------------*/
/*
  @brief
  This funtion returns the common device  status  for the specified device type and instance
  number.

  @details
  This funtion returns the common device status for the specified device type and instance
  number.

  @param
  dev_type : type of device
  instance : instance number for the device type.

  @return
  rfc_device_availability_enum_type : device status

*/
rfc_device_availability_enum_type rfc_intf::get_cmn_rf_device_status(rfdevice_type_enum_type dev_type, uint8 instance)
{
  if (dev_type >= RFDEVICE_TYPE_MAX_NUM || instance >= RFC_MAX_DEVICE_INSTANCES)
  {
    MSG_2(MSG_SSID_RF, MSG_LEGACY_HIGH, "rfc_intf::get_cmn_device_objects(%d, %d): Invalid parameters", dev_type, instance);
    return RFC_DEVICE_INVALID_STATUS;
  }

  return cmn_rf_devices[dev_type][instance].device_status;
}

/*----------------------------------------------------------------------------*/
/*
  @brief
  This funtion returns the common device object ptr for the specified device type and instance
  number.

  @details
  This funtion returns the common device object ptr for the specified device type and instance
  number.

  @param
  txdac : dac chain of interest

  @return
  rfdevice_class * : device object ptr.

*/
rfdevice_class *rfc_intf::get_txdac_trx_device(txlm_dac_type txdac)
{
  if (txdac >= TXLM_DAC_MAX)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_HIGH, "rfc_intf::get_txdac_trx_device(%d): Invalid parameters", txdac);
    return (rfdevice_class *)NULL;
  }

  return txdac_trx_device[txdac];
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Deleta all rf device objects

  @param
  None

  @return
  None
*/
void rfc_intf::destroy_cmn_rf_devices(void)
{
  for (uint32 deviceType = 0; deviceType<RFDEVICE_TYPE_MAX_NUM; deviceType++)
  {
    for (uint32 deviceInstance = 0; deviceInstance<RFC_MAX_DEVICE_INSTANCES; deviceInstance++)
    {
      if (NULL != this->cmn_rf_devices[deviceType][deviceInstance].device_obj)
      {
        delete this->cmn_rf_devices[deviceType][deviceInstance].device_obj;
        this->cmn_rf_devices[deviceType][deviceInstance].device_obj = NULL;
      }
    }
  }
for (uint8 antenna = 0; antenna < RFC_MAX_ANTENNA; antenna++)
  {
    dummy_tuner_mgr[antenna] = NULL;
  }
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Create rf device objects based on the rf-card specific device configuration info.

  @details
  Create rf device objects based on the rf-card specific device configuration info.

  @param
  None

  @return
  None
*/
boolean rfc_intf::create_cmn_rf_devices(void)
{
  boolean status = TRUE;
  uint8 index = 0;
  boolean is_early_end = FALSE;

  /* Init the cmw init statue with NONE*/
  cmw_rfc_init_status = RFM_INIT_FAILURE_NONE;

  /*rfdevice_class*/
  rfdevice_class *dev_obj = NULL;

  /*Physical device*/
  rfc_phy_device_info_type *phy_devices_cfg = NULL;
  rfdevice_id_enum_type phy_dev_id;
  rfdevice_physical_device* phys_dev=NULL;
  uint8 phy_dev_instance;

  /*Logical device*/
  rfc_logical_device_info_type *logical_devices_cfg = NULL;
  rfdevice_type_enum_type logical_dev_type;
  uint8 logical_dev_instance;
  rfdevice_id_enum_type dev_id = RFDEVICE_INVALID;

  /*Old device cfg structure to support legacy WTRs(WTR2605 & WTR1605) and TRX_HDET device creatoion*/
  rfc_device_cfg_info_type device_cfg;

  /*Get the rfc instance*/
  rfc_common_data *rfc_data = rfc_common_data::get_instance();

  if (NULL == rfc_data)
  {
    RF_MSG( RF_ERROR, "rfc_intf::create_cmn_rf_devices RFC ERROR Common data object is NOT created" );
    return FALSE;
  }

  /*Get the physical device config structure from RFC*/
  phy_devices_cfg = rfc_data->get_phy_device_cfg();

  /*Get the logical device config structure from RFC*/
  logical_devices_cfg = rfc_data->get_logical_device_cfg();

  if ( (NULL == phy_devices_cfg) || (NULL == logical_devices_cfg))
  {
    RF_MSG( RF_ERROR, " rfc_intf::create_cmn_rf_devices RFC ERROR Device Configuration data not available" );
    return FALSE;
  }

  char* file_path; /* Character buffer to hold the file path */
  boolean file_path_status;
  file_path = new char[MAX_RFC_SELFCAL_PATH_LENGTH];
  if ( file_path == NULL )
  {
    RF_MSG(RF_ERROR, "rfc_intf::create_cmn_rf_devices RFC ERROR Unable to allocate memory to form RF device self-cal path");
    return FALSE;
  }


  /*******************************************Scan through the physical device cfg list and program USIDs**********************************************/

  /* Program usid for RFFE v2 devices */
  if(FALSE == rfc_program_phy_device_usid(phy_devices_cfg, TRUE /*is_rffe_v2*/ ))
  {
    RF_MSG( RF_ERROR, "rfc_intf::create_cmn_rf_devices, failed to program usid for RFFE v2 devices ");
    return FALSE;
    }

   /* Program usid for RFFE v1 devices */
  if (FALSE == rfc_program_phy_device_usid(phy_devices_cfg, FALSE /*is_rffe_v2*/ ))
    {
    RF_MSG( RF_ERROR, "rfc_intf::create_cmn_rf_devices, failed to program usid for RFFE v1 devices ");
    return FALSE;
  }

  if (0 == phys_devices_count)
  {
    RF_MSG( RF_ERROR, "rfc_intf::create_cmn_rf_devices, failed to program usid for RFFE devices ");
    return FALSE;
  }

  /**************************************************************************************************************************************************/


  /** The maximum number of devices for this card is now known.  Create an
     array to hold the physical devices, and initialize all pointers to
     NULL and their corresponding rfc_device_availability_enum_type to RFC_DEVICE_INVALID_STATUS **/

  phys_devices_array = 
    #ifndef FEATURE_CLADE_HEAP
    (rfc_physical_device_struct_type*)modem_mem_alloc(sizeof(rfc_physical_device_struct_type)*phys_devices_count,MODEM_MEM_CLIENT_RFA);
    #else
    (rfc_physical_device_struct_type*)clade_heap_alloc(sizeof(rfc_physical_device_struct_type)*phys_devices_count);
    #endif    

  if (phys_devices_array == NULL)
  {
    RF_MSG( RF_ERROR, "rfc_intf::create_cmn_rf_devices RFC ERROR Cannot Allocate memory for physical devices");
    return FALSE;
  }

  memset(phys_devices_array, 0, phys_devices_count*sizeof(rfc_physical_device_struct_type));

  /*Update the physical device status for all the elements in the physical device array*/
  for(index=0 ;index < phys_devices_count; index++)
  {
    phys_devices_array[index].device_status = RFC_DEVICE_INVALID_STATUS;
    phys_devices_array[index].phy_dev_cfg = NULL;
  }

  /**********************************Now loop through the physical devices list and create physical device objects*************************************/
  index = 0;
  while(phy_devices_cfg[index].rf_device_id != RFDEVICE_INVALID)
  {
    phy_dev_id = phy_devices_cfg[index].rf_device_id;
    phy_dev_instance = phy_devices_cfg[index].phy_dev_instance;

    /*Check if the device id is within range*/
    if (phy_dev_id == DEFAULT_RFDEVICE || phy_dev_id >= RFDEVICE_MAX_NUM)
    {
      RF_MSG_2( RF_ERROR, "rfc_intf::create_cmn_rf_devices RFC ERROR Invalid rf device data : phy_dev_id=%d, phy_dev_instance=%d",
                phy_dev_id, phy_dev_instance);
      return FALSE;
    }

    /* Make sure that we don't have two physical device objects with the same physical device instance*/
    if((phys_devices_array[phy_dev_instance].device_obj != NULL))
    {
      RF_MSG_2( RF_ERROR, "rfc_intf::create_cmn_rf_devices RFC Skipping phy dev_id %d instance %d creation as one of this instance is already created",
      phy_dev_instance, phy_dev_id);
      index++;
      continue;
    }

    /*Create the physical device*/
    phys_dev = rf_device_factory_create_phys_device(&phy_devices_cfg[index]);

    /**********************************Adding physical device objects to the physical devices array indexed by the physical device instance****************/

    if ( phys_dev != NULL )
    {
      /*Store the phy obj to the phy devices array*/
      phys_devices_array[phy_dev_instance].device_obj = phys_dev;
      phys_devices_array[phy_dev_instance].device_status = RFC_DEVICE_PRESENT;

      RF_MSG_2( RF_HIGH,
                "rfc_intf::create_cmn_rf_devices RFC created successfully phy dev_id %d instance %d", 
                phy_dev_id, phy_dev_instance );

      file_path_status = get_device_self_cal_path(phy_dev_instance,
                                               file_path,
                                               MAX_RFC_SELFCAL_PATH_LENGTH);
      if ( file_path_status == FALSE )
      {
        RF_MSG( RF_ERROR,
                "rfc_intf::create_cmn_rf_devices RFC ERROR file path exceeded the memory allocated to hold it");
      }
      else
      {
        if ( phys_dev->load_self_cal(file_path) == false )
        {
          RF_MSG_1(RF_ERROR,
                   "rfc_intf::create_cmn_rf_devices RFC ERROR loading self cal data for phy_dev_instance index = %d",
                   phy_dev_instance);
        }
      }
    }
    else
    {
     cmw_rfc_init_status = RFM_INIT_FAILURE_HW;
      phys_devices_array[phy_dev_instance].device_status = RFC_DEVICE_MISSING_FATAL;

      /*Update the missing devices as non-fatal if the rfc indicates it from the product revision or if it is a legacy WTR (wtr2605 or wtr1605)
       which don't have physical device support*/
      if(((phy_devices_cfg[index].product_rev & RFC_NONFATAL_IF_MISSING_BIT_IND) && rfcommon_nv_get_rfc_disable_failure_nonfatal_devices_flag() )||
         ((phy_devices_cfg[index].rf_device_id == WTR2605) ||
         (phy_devices_cfg[index].rf_device_id == WTR1605) ||
         (phy_devices_cfg[index].rf_device_id == TP_BUFFER)) )
      {
        phys_devices_array[phy_dev_instance].device_status = RFC_DEVICE_MISSING_NON_FATAL;
      }
      else
      {
        RF_MSG_2( RF_ERROR, "rfc_intf::create_cmn_rf_devices RFC ERROR Physical dev_id %d instance %d not available. Cannot use RFC.",
                  phy_dev_id, phy_dev_instance );
      }
    }

    /*Store the phy device config info to the phy devices array*/
    phys_devices_array[phy_dev_instance].phy_dev_cfg = &phy_devices_cfg[index];

    /*Move to the next physical device listed in the RFC*/
    index++;
  } /* while(phy_devices_cfg[index]) */

  /*******************************************************End of physical device creation*****************************************************************/



  /**********************************Create logical components listed in the RFC for each physical device*************************************************/

  rfc_device_availability_enum_type logical_device_status = RFC_DEVICE_INVALID_STATUS;
  uint32 txdac = 0;

  index=0;
  is_early_end = FALSE;
  while( logical_devices_cfg[index].rf_device_type != RFDEVICE_TYPE_INVALID )
  {
    /*Extract the required device info from rfc*/
    dev_id = logical_devices_cfg[index].rf_device_id;
    logical_dev_type = logical_devices_cfg[index].rf_device_type;
    logical_dev_instance = logical_devices_cfg[index].rf_asic_id;
    phy_dev_instance = logical_devices_cfg[index].associated_phy_dev_instance;
    phys_devices_array[phy_dev_instance].phy_dev_cfg->product_rev &= ~RFC_SKIP_RFFE_DETECT_BIT_IND;
    txdac = phys_devices_array[phy_dev_instance].phy_dev_cfg->associated_dac; /* save the trx device associated with the Tx DAC */

    if ((dev_id == DEFAULT_RFDEVICE) || (dev_id >= RFDEVICE_MAX_NUM))
    {
      is_early_end = TRUE;
      break;
    }

    /*Check if the device instances are within range*/
    if (logical_dev_instance >= RFC_MAX_DEVICE_INSTANCES)
    {
      RF_MSG_2( RF_HIGH, "rfc_intf::create_cmn_rf_devices RFC ERROR. Detected logical_dev_instance >= RFC_MAX_DEVICE_INSTANCES .  logical_dev_type=%d, logical_dev_instance=%d",
                logical_dev_type, logical_dev_instance);
      return FALSE;
    }

    /*For legacy WTRs (WTR2605 & WTR1605) and TRX_HDET devices, fallback to the old device creation */
    if ( (logical_devices_cfg[index].rf_device_id == WTR2605) ||
         (logical_devices_cfg[index].rf_device_id == WTR1605) ||
         (logical_devices_cfg[index].rf_device_id == TRX_HDET)
       )
    {

      /*populate the old device config structure*/
      device_cfg.associated_rf_device_type    = RFDEVICE_TRANSCEIVER;
      device_cfg.associated_rf_asic_id        = phy_dev_instance;
      device_cfg.rf_device_type               = logical_devices_cfg[index].rf_device_type;
      device_cfg.rf_device_id                 = logical_devices_cfg[index].rf_device_id;
      device_cfg.rf_asic_id                   = logical_devices_cfg[index].rf_asic_id;
      device_cfg.rf_device_comm_protocol      = phys_devices_array[phy_dev_instance].phy_dev_cfg->rf_device_comm_protocol;
      device_cfg.bus[0]                       = phys_devices_array[phy_dev_instance].phy_dev_cfg->bus[0];
      device_cfg.bus[1]                       = phys_devices_array[phy_dev_instance].phy_dev_cfg->bus[1];
      device_cfg.manufacturer_id              = phys_devices_array[phy_dev_instance].phy_dev_cfg->manufacturer_id;
      device_cfg.product_id                   = phys_devices_array[phy_dev_instance].phy_dev_cfg->product_id;
      device_cfg.product_rev                  = phys_devices_array[phy_dev_instance].phy_dev_cfg->product_rev;
      device_cfg.default_usid_range_start     = phys_devices_array[phy_dev_instance].phy_dev_cfg->default_usid_range_start;
      device_cfg.default_usid_range_end       = phys_devices_array[phy_dev_instance].phy_dev_cfg->default_usid_range_end;
      device_cfg.assigned_usid                = phys_devices_array[phy_dev_instance].phy_dev_cfg->assigned_usid;
      device_cfg.group_id                     = phys_devices_array[phy_dev_instance].phy_dev_cfg->group_id;
      device_cfg.init_required                = phys_devices_array[phy_dev_instance].phy_dev_cfg->init_required;
      device_cfg.associated_dac               = phys_devices_array[phy_dev_instance].phy_dev_cfg->associated_dac;


      dev_obj = rf_device_factory_create_device(&device_cfg);

    }
    else
    {
      if (phys_devices_array[phy_dev_instance].device_obj == NULL)
      {
        RF_MSG_2( RF_HIGH, "rfc_intf::create_cmn_rf_devices Physical device id %d instance = %d NOT available. Move to next logical device ",
                  phys_devices_array[phy_dev_instance].phy_dev_cfg->rf_device_id, phy_dev_instance);
        index++;
        /*Update logical device status */
        this->cmn_rf_devices[logical_dev_type][logical_dev_instance].device_status = phys_devices_array[phy_dev_instance].device_status ;
        continue;

      }
      /*If a third party device. Call the device factory for 3rd party devices */
      /*Once created successfully, it registers itself as a child of the physical device object passed to it*/
      if ( (logical_devices_cfg[index].rf_device_id == GEN_ASM) || 
            (logical_devices_cfg[index].rf_device_id == GEN_PA)  || 
            (logical_devices_cfg[index].rf_device_id == GEN_LNA) || 
      	    (logical_devices_cfg[index].rf_device_id == GEN_COUPLER) || 
            (logical_devices_cfg[index].rf_device_id == GEN_ASM_GRFC_TUNER) ||
           (logical_devices_cfg[index].rf_device_id == GEN_ASM_ASDIV) ||
           (logical_devices_cfg[index].rf_device_id == GEN_ASM_XSW) ||
           (logical_devices_cfg[index].rf_device_id == GEN_ASM_RFFE_TUNER) ||
           (logical_devices_cfg[index].rf_device_id == GEN_ASM_GRFC_ASDIV) ||
           (logical_devices_cfg[index].rf_device_id == GEN_ASM_GRFC_XSW))
      {
        dev_obj = create_gen_device_object(phys_devices_array[phy_dev_instance].device_obj,(&logical_devices_cfg[index]));
      }
      else
      {
        /*For rest of the devices, call get_component to obtain the requested logical component from the physical device object*/
        dev_obj = phys_devices_array[phy_dev_instance].device_obj->get_component( (&logical_devices_cfg[index]) );
      }

    }

    /* Update the logical device status*/
    if (NULL == dev_obj)
    {
      logical_device_status = RFC_DEVICE_MISSING_FATAL;
      RF_MSG_3( RF_HIGH, "rf device NOT available : dev_type=%d, dev_id=%d,  logical device instance=%d. May be a third party alternate part which is already created.",
                logical_devices_cfg[index].rf_device_type,  logical_devices_cfg[index].rf_device_id, logical_devices_cfg[index].rf_asic_id );

      if( ( ((phys_devices_array[phy_dev_instance].phy_dev_cfg->product_rev) & RFC_NONFATAL_IF_MISSING_BIT_IND) &&
            rfcommon_nv_get_rfc_disable_failure_nonfatal_devices_flag() ) ||
          (logical_devices_cfg[index].rf_device_type == RFDEVICE_BUFFER) )
      {
        logical_device_status = RFC_DEVICE_MISSING_NON_FATAL;
        RF_MSG_2( RF_HIGH,
                  "rfc_intf::create_cmn_rf_devices RFC missing non fatal logical dev_id %d instance %d", 
                  logical_dev_type, logical_dev_instance );
      }
      else
      {
        RF_MSG_3( RF_ERROR, "rfc_intf::create_cmn_rf_devices RFC ERROR MISSING FATAL Logical[dev_type %d/instance %d] for physical instance %d not available. Cannot use RFC."
                  " Check logical vs physical device mappings in RFC.",
                  logical_dev_type, logical_dev_instance, phy_dev_instance );
      }
    }
    else
    {
      RF_MSG_2( RF_HIGH,
                "rfc_intf::create_cmn_rf_devices RFC created successfully logical dev_id %d instance %d", 
                logical_dev_type, logical_dev_instance );

      logical_device_status = RFC_DEVICE_PRESENT;
      /* Increment the logical device list as we find one*/
      logical_devices_count++;
    }

    /* Make sure that we don't have two device objects of the same device type and instance number.
    This is a critical error in the device configuration information from the rf-card. */
    if (this->cmn_rf_devices[logical_dev_type][logical_dev_instance].device_obj == NULL)
    {
      this->cmn_rf_devices[logical_dev_type][logical_dev_instance].device_obj = dev_obj;

      /* save the trx device associated with the Tx DAC */
      if (txdac < TXLM_DAC_NUM && (logical_dev_type == RFDEVICE_TRANSCEIVER || logical_dev_type == RFDEVICE_TRANSMITTER))
      {
        this->txdac_trx_device[txdac] = dev_obj;
      }
      this->cmn_rf_devices[logical_dev_type][logical_dev_instance].device_status = logical_device_status;
    }
    else
    {
      if (dev_obj != NULL ) // free-up the dynamically allocated dev_obj if we don't need it.
      {
        if (phys_devices_array[phy_dev_instance].phy_dev_cfg->alternate_part_idx == RFC_NO_ALTERNATE_PART)
        {
           delete dev_obj;
           RF_MSG_3( RF_ERROR, "RFC ERROR Conflicting rf_device objects for the same device_type and instance : logical_dev_type=%d, logical_device_asic_id=%d, logical_dev_type=%d",
                     logical_dev_type,  logical_devices_cfg[index].rf_asic_id, logical_dev_type);
           status = FALSE;
        }
      }
    }

    /* enable the WTR LPM SM */
    if (logical_dev_type == RFDEVICE_TRANSCEIVER)
    {
      rfdevice_rxtx_common_class *common_device = 
      (rfdevice_rxtx_common_class *)(this->cmn_rf_devices[logical_dev_type][logical_dev_instance].device_obj);
	  
      if (common_device != NULL)
      {
        rfdevice_cmn_enable_wtr_lpm_state_machine(common_device, rfcommon_nv_get_lpm_feature_enabled());
      }
    }

    /*Move to the next logical device*/
    index++;

  }

  if ( is_early_end || (logical_devices_count == 0) )
  {
    // Check logical dev table contents in crash dump make sure they match actual source code. If they dont match, then it is due to mem reclamation error.
    // Check that value of dev idx (index) reported matches the num of items in log dev table as well as another indicator of bad mem
    RF_MSG_3( RF_ERROR, "RFC ERROR with logical dev table. Log dev idx %d id %d num valid devices found %d. Check table validity.", 
              index, dev_id, logical_devices_count );
    return FALSE;
  }

 /*******************************************************End of logical device creation*****************************************************************/


  /*Temporary code to be supports until RFC AG incorporates the following */
  index = 0;
  for (index = 0; index < RFC_MAX_DEVICE_INSTANCES; index++)
  {
#ifndef FEATURE_GNSS_ONLY_NO_WWAN
    /*Disable all the PWR tracker devices*/
    dev_obj = this->cmn_rf_devices[RFDEVICE_PAPM][index].device_obj;
    if (NULL != dev_obj)
    {
      ((rfdevice_papm *)dev_obj)->disable(RFM_DEVICE_0, RFM_ALL_MODES, NULL, RFDEVICE_EXECUTE_IMMEDIATE);
      ((rfdevice_papm *)dev_obj)->trigger(NULL, RFM_DEVICE_0, RFDEVICE_EXECUTE_IMMEDIATE, 0);
    }

    /*Create Tuner Manager objects for each of the Tuner devices found*/
    dev_obj = this->cmn_rf_devices[RFDEVICE_TUNER][index].device_obj;
    if ( (this->cmn_rf_devices[RFDEVICE_TUNER_MANAGER][index].device_obj == NULL)
         && (NULL != dev_obj) )
    {
      /*! Valid Tuner device found, create a Tuner manager.
          For now there will be one-to-one mapping between tuner manger and a
          tuner device */

      /*Create the tuner manager based on the type of the algorithm requested
      and the actual HW support*/
      if(TP_TUNER == dev_obj->rfdevice_id_get())
      {
        this->cmn_rf_devices[RFDEVICE_TUNER_MANAGER][index].device_obj =
           rf_device_factory_create_tuner_manager( dev_obj, ATUNER_TP_MANAGER);
        MSG_1(MSG_SSID_RF, MSG_LEGACY_HIGH,"TP Tuner Manager Object created", 0);
      }
      else
      {

   #ifdef FEATURE_RF_HAS_QTUNER
      rfcommon_atuner_manager_factory_type manager_type = ATUNER_AOL_MANAGER;

      switch((rfdevice_atuner_tuner_device_algo_type)rfcommon_nv_tbl.rf_atuner_algo_type)
      {

      case RFDEVICE_ANTENNA_TUNER_OL_ONLY_ALGO:
      case RFDEVICE_ANTENNA_TUNER_AOL_ONLY_ALGO:
      case RFDEVICE_ANTENNA_TUNER_AOL_PLUS_CL_ALGO:
      case RFDEVICE_ANTENNA_TUNER_OL_PLUS_CL_ALGO:
           manager_type = ATUNER_AOL_MANAGER;
           break;

      /*FBRX based CL did not depend on the tuner device.So always create a CL manager*/
      case RFDEVICE_ANTENNA_TUNER_AOL_PLUS_CL_FBRX_ALGO:
      case RFDEVICE_ANTENNA_TUNER_OL_PLUS_CL_FBRX_ALGO:
           manager_type = ATUNER_CL_MANAGER;
           break;
#ifdef FEATURE_RFA_ACL
      case RFDEVICE_ANTENNA_TUNER_AOL_PLUS_ACL_FBRX_ALGO:
      case RFDEVICE_ANTENNA_TUNER_OL_PLUS_ACL_FBRX_ALGO:
           manager_type = ATUNER_ACL_MANAGER;
           break;
#endif /* FEATURE_RFA_ACL */
      default:
           manager_type = ATUNER_AOL_MANAGER;
           break;
      }

         this->cmn_rf_devices[RFDEVICE_TUNER_MANAGER][index].device_obj =
                   rf_device_factory_create_tuner_manager( dev_obj, manager_type);
      #endif
      }
    }

    /*Update the tuner device  status as tuner manager status*/
    this->cmn_rf_devices[RFDEVICE_TUNER_MANAGER][index].device_status =
          this->cmn_rf_devices[RFDEVICE_TUNER][index].device_status;
#endif
  }


  /* Verify PHYSICAL rf device creation */
  if (status != FALSE) // if all the devices created sucessfully
  {
    index = 0;
    while(phy_devices_cfg[index].rf_device_id != RFDEVICE_INVALID)
    {
      /* Extract expected physical devices from RFC */
      phy_dev_id = phy_devices_cfg[index].rf_device_id;
      phy_dev_instance = phy_devices_cfg[index].phy_dev_instance;

      /* Verify creation */
      if ((NULL == this->phys_devices_array[phy_dev_instance].device_obj) &&
          (RFC_DEVICE_MISSING_FATAL == this->phys_devices_array[phy_dev_instance].device_status))
      {
        RF_MSG_2(RF_ERROR, "Physical rf device creation failed. Missing phy_dev_id=%d, phy_dev_instance=%d",
                  phy_dev_id, phy_dev_instance );
        status = FALSE;
      }

      index++;
    }
  }

  /* Verify LOGICAL rf device creation */
  if (status != FALSE) // if all the devices created sucessfully
  {
    index = 0;
    while(logical_devices_cfg[index].rf_device_type != RFDEVICE_TYPE_INVALID)
    {
      /* Extract expected logical devices from RFC */
      logical_dev_type = logical_devices_cfg[index].rf_device_type;
      logical_dev_instance = logical_devices_cfg[index].rf_asic_id;

      /* Verify creation */
      if ((NULL == this->cmn_rf_devices[logical_dev_type][logical_dev_instance].device_obj) &&
          (RFC_DEVICE_MISSING_FATAL == this->cmn_rf_devices[logical_dev_type][logical_dev_instance].device_status))
      {
        RF_MSG_2(RF_ERROR, "Logical rf device creation failed. Missing logical_dev_type=%d, logical_dev_instance=%d",
                 logical_dev_type, logical_dev_instance);
        status = FALSE;
      }

      index++;
    }
  }

  delete [] file_path;
  return status;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  API to program usid for all physical devices in RFC spreadsheet

  @details
  API to progeam usid for both RFFE v1/v2 devices found in given RFC spreadsheet

  @param phy_devices_cfg
  Physical device list pointer to all devices in RFC spreadsheet
  
  @param is_rffe_v2 
  Boolean flag to indicate to program RFFE v2 devices or v1 devices 

  @return
  TRUE means SUCESS. FALSE otherwise 
*/

boolean rfc_intf::rfc_program_phy_device_usid(rfc_phy_device_info_type *phy_devices_cfg, boolean is_rffe_v2)
{
  uint8 index = 0; /*Index to loop through all physical devices in the list*/
  boolean is_early_end = FALSE;
  boolean status = FALSE;
  rfdevice_id_enum_type phy_dev_id;

  while(phy_devices_cfg[index].rf_device_id != RFDEVICE_INVALID)
  {
    /* Skip RFFE v2 devices during v1 device programming */
    if ( (phy_devices_cfg[index].proto_version == RFDEVICE_COMM_PROTO_VERSION_2) && (FALSE == is_rffe_v2) ) 
    {
      index++;
      continue;
    }
    
    /* Skip RFFE v1 devices during v2 device programming */
    if ( (phy_devices_cfg[index].proto_version == RFDEVICE_COMM_PROTO_VERSION_DEFAULT) && (TRUE == is_rffe_v2) )
    {
      index++;
      continue;
    }

    phy_dev_id = phy_devices_cfg[index].rf_device_id;
    if ((phy_dev_id == DEFAULT_RFDEVICE) || (phy_dev_id >= RFDEVICE_MAX_NUM))
    {
      is_early_end = TRUE;
      break;
    }

    status=rf_device_factory_program_device_id( &(phy_devices_cfg[index]) );

    if (status)
    {
      /*Increment the physical device size as and when we detect one*/
      phys_devices_count++;
    }
    index++;
  }/*End of looping through all devices */

  if (TRUE == is_early_end)
  {
     RF_MSG_3( RF_ERROR, "RFC ERROR with phy dev table. Phy dev idx %d id %d num valid devices found %d. Check table validity.", 
              index, phy_dev_id, phys_devices_count );
     return FALSE;
  }
  else
  {
    return TRUE;
  }

} /*End of rfc_intf::rfc_program_phy_device_usid*/
/*----------------------------------------------------------------------------*/
/*!
  @brief
  Populates a client provided character string buffer with the path to the
  RF card device self-cal data file.

  @details
  The function insures that the character buffer is not overflowed, and that
  the string is always NULL terminated.  If insufficient space exists in
  the buffer, the function returns a false value so the caller can catch
  the incorrect string.

  @param device_index
  The 0-based index of the physical device in the devices auto-gen list

  @param buffer
  A pointer to the character buffer where the string will be created.

  @param size
  The maximum length of the character buffer.  Up to size-1 characters will
  be put into the buffer, guaranteeing there buffer can not be overflowed.

  @return
  True if the buffer contains the full and correct file path to the device
  self-cal data.  False if for any reason creating the file path string
  failed, including running out of buffer space.
*/
bool
rfc_intf::get_device_self_cal_path
(
  int device_index,
  char* buffer,
  int size
)
{
  if ( buffer != NULL )
  {
    int file_path_length;

#ifdef _MSC_VER
    /* MSVC doesn't support the C99 standard snprintf() function */
    file_path_length =
      _snprintf_s(buffer, size, _TRUNCATE,
                  RFC_PHYS_DEVICE_SELF_CAL_PATH_FMT_STR,
                  rfc_card_id, device_index);
    if ( file_path_length > 0 )
    {
      return true;
    }
#else
    file_path_length =
      snprintf(buffer, size,
               RFC_PHYS_DEVICE_SELF_CAL_PATH_FMT_STR,
               rfc_card_id, device_index);
    if ( file_path_length < MAX_RFC_SELFCAL_PATH_LENGTH )
    {
      return true;
    }
#endif /* _MSC_VER */
  }

  return false;
} /* rfc_intf::get_device_self_cal_path() */


/*----------------------------------------------------------------------------*/
/*
  @brief

  @details

  @param

  @return

*/

#ifndef FEATURE_GNSS_ONLY_NO_WWAN
boolean rfc_intf::do_self_test(rfc_self_test_info_type *self_test_info)
{
  rfc_common_data *rfc_data = rfc_common_data::get_instance();
  rfdevice_type_enum_type dev_type = RFDEVICE_TYPE_INVALID;
  uint32 dev_instance = 0;
  rfdevice_class *dev_obj = NULL;
  uint8 index = 0;
  boolean status = FALSE;
  rfc_phy_device_info_type *phy_devices_cfg = NULL;
  rfc_logical_device_info_type *logical_devices_cfg = NULL;
  rfc_device_cfg_info_type device_cfg;
  rfdevice_id_enum_type phy_dev_id;
  uint8 phy_dev_instance;
  rfdevice_type_enum_type logical_dev_type;
  uint8 logical_dev_instance;

  if (NULL == self_test_info)
  {
    RF_MSG( RF_ERROR, " RFC incorrect parameter for do_self_test() : self_test_info == NULL!" );
    return FALSE;
  }

  if (NULL == rfc_data)
  {
    RF_MSG( RF_ERROR, " RFC Common data object is NOT created : rfc_common_data == NULL!" );
    return FALSE;
  }

  phy_devices_cfg = rfc_data->get_phy_device_cfg();
  logical_devices_cfg = rfc_data->get_logical_device_cfg();
  if ( (NULL == phy_devices_cfg) || (NULL == logical_devices_cfg))
  {
    RF_MSG( RF_ERROR, " RFC rf device Configuration data not available : devices_cfg == NULL!" );
    return FALSE;
  }

  index = 0;

  while(logical_devices_cfg[index].rf_device_type != RFDEVICE_TYPE_INVALID)
  {
    status = TRUE;
    logical_dev_type = logical_devices_cfg[index].rf_device_type;
    logical_dev_instance = logical_devices_cfg[index].rf_asic_id;
    phy_dev_instance = logical_devices_cfg[index].associated_phy_dev_instance;

    if (logical_dev_type >= RFDEVICE_TYPE_MAX_NUM ||
        logical_dev_instance >= RFC_MAX_DEVICE_INSTANCES)
    {
      RF_MSG_2( RF_HIGH, "Invalid rf device data : logical_dev_type=%d, logical_dev_instance=%d",
                logical_dev_type, logical_dev_instance);
      status = FALSE;
      break;
    }

      /*Get the desired logical component from the physical device object*/
      dev_obj = phys_devices_array[phy_dev_instance].device_obj->get_component( (&logical_devices_cfg[index]) );

    if (index < RFC_MAX_DEVICE_NUM)
    {
      self_test_info->device[index].device_info = &logical_devices_cfg[index];
      self_test_info->device[index].result = FALSE;
      if (NULL != dev_obj)
      {
        status = dev_obj->self_test(phys_devices_array[phy_dev_instance].phy_dev_cfg->manufacturer_id, phys_devices_array[phy_dev_instance].phy_dev_cfg->product_id,
                                    phys_devices_array[phy_dev_instance].phy_dev_cfg->product_rev);
        self_test_info->device[index].result = status;
      }
      else
      {
        RF_MSG_3( RF_ERROR, "Missing the required rf_device : logical_dev_type=%d, rf_device_id =%d, logical_dev_instance=%d",
                  logical_dev_type,  logical_devices_cfg[index].rf_device_id, logical_dev_instance);
      }


    }
    else
    {
      RF_MSG_1( RF_ERROR, "RFC self_test buffer overflow! :index = %d",index);
    }

    index++;
  }

  self_test_info->num_devices = index;
  return TRUE;
}

#endif /* FEATURE_GNSS_ONLY_NO_WWAN */

/*----------------------------------------------------------------------------*/
/*
  @brief

  @details

  @param

  @return

*/
void rfc_intf::get_awake_resource_request(rfm_resource_info* dev_cfg)
{
  rfc_vreg_mgr *vreg_mgr_p = rfc_vreg_mgr::get_instance();


  if ( dev_cfg != NULL )
  {
    if (vreg_mgr_p != NULL)
    {
      vreg_mgr_p->get_awake_resource_request(dev_cfg);
    }
    else
    {
      ERR_FATAL("Cannot Create VREG_MGR",0,0,0);
    }
  }
  else
  {
    RF_MSG( RF_ERROR, "rfc_get_resource_request: Invalid Container");
  }
}

/*----------------------------------------------------------------------------*/
/*
  @brief

  @details

  @param

  @return

*/
void rfc_intf::set_quiet_mode(rfm_mode_enum_type mode, boolean on_off)
{
  rfc_vreg_mgr *vreg_mgr_p = rfc_vreg_mgr::get_instance();

  if (vreg_mgr_p != NULL)
  {
    vreg_mgr_p->set_quiet_mode(mode, on_off);
  }
  else
  {
    ERR_FATAL("Cannot Create VREG_MGR",0,0,0);
  }
}

/*----------------------------------------------------------------------------*/
/*
  @brief

  @details

  @param

  @return

*/
void rfc_intf::common_enable_autocal_vregs(boolean on_off)
{
  rfc_vreg_mgr *vreg_mgr_p = rfc_vreg_mgr::get_instance();
  if (vreg_mgr_p != NULL)
  {
    vreg_mgr_p->enable_autocal_vregs(on_off);
  }
  else
  {
    ERR_FATAL("Cannot Create VREG_MGR",0,0,0);
  }

}

/*----------------------------------------------------------------------------*/
/*
  @brief

  @details

  @param

  @return

*/
void rfc_intf::common_enable_mpp_dac_vref(boolean on_off)
{
  rfc_vreg_mgr *vreg_mgr_p = rfc_vreg_mgr::get_instance();
  if (vreg_mgr_p != NULL)
  {
    vreg_mgr_p->enable_mpp_dac_vref(on_off);
  }
  else
  {
    ERR_FATAL("Cannot Create VREG_MGR",0,0,0);
  }

}

/*----------------------------------------------------------------------------*/
/*
  @brief

  @details

  @param

  @return

*/
void rfc_intf::lte_light_sleep_vregs(void)
{
  rfc_vreg_mgr *vreg_mgr_p = rfc_vreg_mgr::get_instance();
  if (vreg_mgr_p != NULL)
  {
    vreg_mgr_p->lte_light_sleep_vregs();
  }
  else
  {
    ERR_FATAL("Cannot Create VREG_MGR",0,0,0);
  }

}

/*----------------------------------------------------------------------------*/
/*
  @brief

  @details

  @param

  @return

*/
void rfc_intf::lte_light_sleep_wakeup_vregs(void)
{
  rfc_vreg_mgr *vreg_mgr_p = rfc_vreg_mgr::get_instance();
  if (vreg_mgr_p != NULL)
  {
    vreg_mgr_p->lte_light_sleep_wakeup_vregs();
  }
  else
  {
    ERR_FATAL("Cannot Create VREG_MGR",0,0,0);
  }

}

/*----------------------------------------------------------------------------*/
/*
  @brief
  Function is used to set/unset VREGS as per necessity.

  @details

  @param

*/
void rfc_intf::manage_vregs(rfm_path_enum_type path, rfc_vregs_id_type vreg_id, rfc_vregs_cmd_type vreg_cmd)
{
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Load Internal Device Calibration Data

  @details
  Nothing to do in this function for most rf-cards.

  @return
  Status indicating that loading of internal device calibration.
*/
const boolean rfc_intf::load_internal_device_calibration(void)
{
  /* Return */
  return TRUE;
}


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
const rfc_common_logical_device_params_type*
rfc_intf::get_logical_dev_param
(
  rfm_device_enum_type rfm_device
)
{
  /* Perform check on individual dev Param to be for its physical Mapped RF Device. */
  if (dev_param[rfm_device].cmn_device != NULL)
  {
    /* Pointer to Device Param */
    return &dev_param[rfm_device];
  }
  else
  {
    RF_MSG( RF_ERROR, " RF Card get_logical_dev_param : Bad Device" );
    /* Invalid Pointer */
    return NULL;
  }

} /* get_logical_dev_param */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Create GSM tech specific rf-card.

  @details
  This function will create GSM tech specific rf-card related data structures and
  initializes the rf-card specific parameters.

  @return
  None
*/
void rfc_intf::create_gsm_rfc()
{
#ifdef FEATURE_GSM

  rfc_gsm_data *rfc_gsm_data = rfc_gsm_data::get_instance();

  if (rfc_gsm_data == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_MED,"GSM NOT Supported in the rf_card hwid=%d", rfc_card_id);
    return;
  }

  // create the GSM RFC Object
  rfc_gsm *rfc_gsm = rfc_gsm::create_instance();
  if (rfc_gsm == NULL)
  {
    ERR_FATAL("Cannot Create GSM RFC Object",0,0,0);
  }
#endif /* FEATURE_GSM */
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Create LTE tech specific rf-card.

  @details
  This function will create LTE tech specific rf-card related data structures and
  initializes the rf-card specific parameters.

  @return
  None
*/
void rfc_intf::create_lte_rfc( )
{
#ifdef FEATURE_LTE
  rfc_lte_data *rfc_lte_data = rfc_lte_data::get_instance();

  if (rfc_lte_data == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_MED,"LTE NOT Supported in the rf_card hwid=%d", rfc_card_id);
    return;
  }

  // create the LTE RFC Object
  rfc_lte_intf *rfc_lte = rfc_lte_intf::create_instance();
  if (rfc_lte == NULL)
  {
    ERR_FATAL("Cannot Create LTE RFC Object",0,0,0);
  }
#endif /* FEATURE_GSM */
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Create WCDMA tech specific rf-card.

  @details
  This function will create WCDMA tech specific rf-card related data structures and
  initializes the rf-card specific parameters.

  @return
  None
*/
void rfc_intf::create_wcdma_rfc()
{
#ifdef FEATURE_WCDMA
//  rfdevice_cmn_type *common_device = NULL;
//  rfdevice_wcdma_txrx_device_type *tranceiver_wcdma_device = NULL;

  rfc_wcdma_data *rfc_wcdma_data = rfc_wcdma_data::get_instance();

  if (rfc_wcdma_data == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_MED,"WCDMA NOT Supported in the rf_card hwid=%d", rfc_card_id);
    return;
  }

  /* This is temporary, until all the device interface change to query the RFC for device object */
/* rfwcdma_rx_device is not used...delete it slowly
  if(cmn_rf_devices[RFDEVICE_TRANSCEIVER][0] != NULL)
  {
    common_device = ((rfdevice_rxtx_common_class*)cmn_rf_devices[RFDEVICE_TRANSCEIVER][0])->get_instance();
    // get pointers to its RX0, RX1 and TX devices
    tranceiver_wcdma_device =(rfdevice_wcdma_txrx_device_type *)(common_device->tech_instance[RFM_IMT_MODE]);

    // Connect up the device
    rfwcdma_rx_device[RF_PATH_0] = tranceiver_wcdma_device->rx0_device;
    rfwcdma_rx_device[RF_PATH_1] = tranceiver_wcdma_device->rx1_device;
    rfwcdma_tx_device[RF_PATH_0] = tranceiver_wcdma_device->tx_device;
  }
*/
  // create the WCDMA RFC Object
  rfc_wcdma *rfc_wcdma = rfc_wcdma::create_instance();
  if (rfc_wcdma == NULL)
  {
    ERR_FATAL("Cannot Create WCDMA RFC Object",0,0,0);
  }
#endif /* FEATURE_WCDMA */
}


/*----------------------------------------------------------------------------*/
/*!
  @brief
  Create GNSS tech specific rf-card.

  @details
  This function will create GNSS tech specific rf-card related data structures and
  initializes the rf-card specific parameters.

  @return
  None
*/
void rfc_intf::create_gnss_rfc()
{
#ifdef FEATURE_CGPS
  rfc_gnss_data *rfc_gnss_data = rfc_gnss_data::get_instance();
  rfc_cfg_params_type cfg;
  rfc_device_info_type *device_info = NULL;
  rfdevice_type_enum_type dev_type;
  uint32 instance_id;

  if (rfc_gnss_data == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_MED,"GNSS NOT Supported in the rf_card hwid=%d", rfc_card_id);
    return;
  }

  cfg.alternate_path = 0;
  cfg.req = RFC_REQ_DEFAULT_GET_DATA;
  cfg.rx_tx = RFC_CONFIG_RX;
  rfc_gnss_data->devices_cfg_data_get(&cfg, &device_info);

  if (device_info == NULL)
  {
    RF_MSG( RF_ERROR, "rfc_intf::create_gnss_rfc. Null device data !");
    return;
  }

  if (rf_device_factory_create_gnss_device(device_info) == FALSE)
  {
    RF_MSG( RF_ERROR, "rfc_intf::create_gnss_rfc. Device creation failed!" );
  }

  dev_type = device_info->rf_asic_info[0].device_type;
  instance_id = device_info->rf_asic_info[0].instance;

  if ( (dev_type < RFDEVICE_TYPE_MAX_NUM) &&
       (instance_id < RFC_MAX_DEVICE_INSTANCES) )
  {
    dev_param[RFM_DEVICE_GPS].cmn_device =
      (rfdevice_rxtx_common_class*)cmn_rf_devices[dev_type][instance_id].device_obj;
  }
  else
  {
    RF_MSG_2( RF_ERROR, "rfc_intf::create_gnss_rfc. Invalid GNSS device type %d"
                        " or instance %d",
              dev_type, instance_id );
  }

  // create the GNSS RFC Object
  rfc_gnss *rfc_gnss = rfc_gnss::create_instance();
  if (rfc_gnss == NULL)
  {
    ERR_FATAL("Cannot Create GNSS RFC Object",0,0,0);
  }
#endif /* FEATURE_CGPS */
}


/*----------------------------------------------------------------------------*/
/*!
  @brief
  Create WLAN tech specific rf-card.

  @details
  This function will create WLAN tech specific rf-card related data structures and
  initializes the rf-card specific parameters.

  @return
  None
*/
void rfc_intf::create_wlan_rfc(void)
{
  rfc_wlan_data *rfc_wlan_data = rfc_wlan_data::get_instance();
  if (rfc_wlan_data == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_MED,"WLAN NOT Supported in the rf_card hwid=%d", rfc_card_id);
    return;
  }

  // create the WLAN RFC Object
  rfc_wlan *rfc_wlan = rfc_wlan::create_instance();
  if (rfc_wlan == NULL)
  {
    ERR_FATAL("Cannot Create WLAN RFC Object",0,0,0);
  }
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Create CDMA tech specific rf-card.

  @details
  This function will create CDMA tech specific rf-card related data structures and
  initializes the rf-card specific parameters.

  @return
  None
*/
boolean rfc_intf::create_cdma_rfc()
{
  boolean card_init_status = TRUE;
#if defined(FEATURE_CDMA1X) || defined(FEATURE_HDR)
  rfc_cdma_data *rfc_cdma_data = rfc_cdma_data::get_instance();

  if (rfc_cdma_data == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_MED,"CDMA NOT Supported in the rf_card hwid=%d", rfc_card_id);
    return FALSE;
  }

  /* create the CDMA RFC Object */
  rfc_cdma_card_intf *rfc_cdma = rfc_cdma_card_intf::create_device_instance
                                                                 ( 200,
                                                            /*prep time in us*/
                                                                   4500,
                                                            /*exec time in us*/
                                                                   &card_init_status
                                                                   );
  /* Null pointer can get returned if BC Config has disabled CDMA RFC creation.
    In that case dont crash since the card_init_status will still be TRUE */
  if ( (rfc_cdma == NULL) && ( card_init_status == FALSE ) )
  {
    ERR_FATAL("Cannot Create CDMA RFC Object",0,0,0);
  }

  if ( card_init_status == FALSE )
  {
    RF_MSG(RF_ERROR, "rfc_intf::create_cdma_rfc : Failed to create CDMA "
                     "object");
  }

#endif /* FEATURE_CDMA */
  return card_init_status;

}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Create TDSCDMA tech specific rf-card.

  @details
  This function will create TDSCDMA tech specific rf-card related data structures and
  initializes the rf-card specific parameters.

  @return
  None
*/
void rfc_intf::create_tdscdma_rfc()
{
#ifdef FEATURE_TDSCDMA

  rfc_tdscdma_data *rfc_tdscdma_data = rfc_tdscdma_data::get_instance();

  if (rfc_tdscdma_data == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_MED,"TDSCDMA NOT Supported in the rf_card hwid=%d", rfc_card_id);
    return;
  }

  /* This is temporary, until all the device interface change to query the RFC for device object */

  // create the GNSS RFC Object
  rfc_tdscdma *rfc_tdscdma = rfc_tdscdma::create_instance();
  if (rfc_tdscdma == NULL)
  {
    ERR_FATAL("Cannot Create TDSCDMA RFC Object",0,0,0);
  }
#endif /* FEATURE_TDSCDMA */

}


/*----------------------------------------------------------------------------*/
/*!
  @brief
  Perform Internal Device Calibration for the entire RF card

  @details
  Does internal device cal for all physical devices.


  @return
  Status indicating that status of card internal device calibration.
*/
const boolean
rfc_intf::do_internal_device_calibration_top
(
  void
)
{
  int i;
  boolean cal_status = TRUE;
  rfc_common_data *rfc_data = NULL;
  rfc_phy_device_info_type *phy_devices_cfg = NULL;
  rfc_logical_device_info_type *logical_devices_cfg = NULL;

  /*Get the rfc instance*/
  rfc_data = rfc_common_data::get_instance();

  if (NULL == rfc_data)
  {
    RF_MSG( RF_ERROR, " rfc_intf::create_cmn_rf_devices RFC Common data object is NOT created : rfc_common_data == NULL!" );
    return FALSE;
  }

  /*Get the physical device config structure from RFC*/
  phy_devices_cfg = rfc_data->get_phy_device_cfg();

  /*Get the logical device config structure from RFC*/
  logical_devices_cfg = rfc_data->get_logical_device_cfg();

  if (phy_devices_cfg == NULL ||
       logical_devices_cfg == NULL)
  {
    RF_MSG( RF_ERROR, "rfc_intf::do_internal_device_calibration_top RFC rf device Configuration data not available" );
    return FALSE;
  }

  /*Loop through the entire physical device array and try to calibrate each physical device using the perform_self_cal API*/
  for (i = 0; i < phys_devices_count; i++)
  {
   if ( phys_devices_array[i].device_status == RFC_DEVICE_PRESENT &&
        phys_devices_array[i].device_obj != NULL )
    {
      char* file_path = new char[MAX_RFC_SELFCAL_PATH_LENGTH];
      if ( file_path == NULL )
      {
        RF_MSG(RF_ERROR, "rfc_intf::do_internal_device_calibration_top  Unable to allocate memory to form RF device self-cal path");
        break;
      }
      if ( get_device_self_cal_path(i,
                                    file_path,
                                    MAX_RFC_SELFCAL_PATH_LENGTH) )
      {
        if ( phys_devices_array[i].device_obj->perform_self_cal(file_path) == false )
        {
          RF_MSG_1(RF_ERROR,
                   "rfc_intf::do_internal_device_calibration_top Failure self-calibrating physical device %d", i);
          cal_status = false;
        }
      }
      else
      {
        RF_MSG(RF_ERROR,
               "rfc_intf::do_internal_device_calibration_top RFC file path exceeded the memory allocated to hold it");
        cal_status = false;
      }

      delete [] file_path;
    }
    else
    {
      RF_MSG_1( RF_ERROR, "rfc_intf::do_internal_device_calibration_top Phyiscal device NOT available for Self cal: phy_device_index=%d",i);


    }
  }

   /*self cal for legacy WTRs (WTR2605 & WTR1605 in DPM2.0) which dont have physical device interface*/
  for (i = 0; i < logical_devices_count; i++)
  {
    if (
        ( logical_devices_cfg[i].rf_device_type == RFDEVICE_TRANSCEIVER ) &&
        ( (logical_devices_cfg[i].rf_device_id == WTR2605)|| (logical_devices_cfg[i].rf_device_id == WTR1605) )
        )
      {
        rfdevice_class *common_device = NULL;
        common_device = get_cmn_rf_device_object(RFDEVICE_TRANSCEIVER,
                                                 logical_devices_cfg[i].rf_asic_id);
        if ( common_device != NULL)
        {
          if ( !do_internal_device_calibration(logical_devices_cfg[i].rf_asic_id) )
          {
            RF_MSG_2(RF_ERROR,
                     "Failure self-calibrating transceiver device %d with logical device module instance (%d)",
                     logical_devices_cfg[i].rf_asic_id, i);
            cal_status = FALSE;
          }
        }

     break;

      }

    }

  return cal_status;

}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Perform Tuner Self Calibration Only

  @details
  Tuner Self Cal only for NOK

  @return
  Status indicating that status of card internal device calibration.
*/
const boolean
rfc_intf::do_tuner_device_calibration_top
(
  void
)
{
  uint8 index, count_tuner_device = 0;
  rfc_logical_device_info_type* logical_devices_cfg;
  boolean cal_status = TRUE;
  uint8 phy_dev_instance;
  rfdevice_class *tuner_device = NULL;

  if ( rfc_common_data::get_instance() == NULL ||
       rfc_common_data::get_instance()->get_logical_device_cfg() == NULL )
  {
    RF_MSG( RF_ERROR, "RFC rf device Configuration data not available" );
    return FALSE;
  }


  /*Get the logical device config structure from RFC*/
  logical_devices_cfg = rfc_common_data::get_instance()->get_logical_device_cfg();

  /*NULL pointer check*/
  if  (NULL == logical_devices_cfg)
  {
    RF_MSG( RF_ERROR, " RFC rf device Configuration data not available!" );
    return FALSE;
  }

 /*Search for tuner devices in the logical devices list and perform self cal as and when you find one*/
  index=0;
  while(logical_devices_cfg[index].rf_device_type != RFDEVICE_TYPE_INVALID)
  {
     if ( logical_devices_cfg[index].rf_device_type == RFDEVICE_TUNER )
    {
        /*Obtain the physical device instance of the logical component*/
         phy_dev_instance = logical_devices_cfg[index].associated_phy_dev_instance;

         if ( phys_devices_array[phy_dev_instance].device_status == RFC_DEVICE_PRESENT &&
              phys_devices_array[phy_dev_instance].device_obj != NULL )
      {
        char* file_path = new char[MAX_RFC_SELFCAL_PATH_LENGTH];

        if ( file_path == NULL )
        {
          RF_MSG(RF_ERROR, "Unable to allocate memory to form RF device self-cal path");
          return FALSE;
        }
            if ( get_device_self_cal_path(phy_dev_instance,
                                      file_path,
                                      MAX_RFC_SELFCAL_PATH_LENGTH) )
        {
                if ( phys_devices_array[phy_dev_instance].device_obj->perform_self_cal(file_path) == false )
          {
            RF_MSG_1(RF_ERROR,
                             "Failure self-calibrating physical device %d", phy_dev_instance);
            cal_status = false;
          }
        }
        else
        {
          RF_MSG(RF_ERROR,
                "RFC file path exceeded the memory allocated to hold it");
          cal_status = false;
        }

        delete [] file_path;
        count_tuner_device++;
      }
      else
      {
           RF_MSG_2( RF_ERROR, "Tuner phyiscal device NOT available for Tuner Self cal:  dev_id=%d,"
                     ",phy_device_index=%d",logical_devices_cfg[index].rf_device_id,phy_dev_instance);
        cal_status = false;
      }
    }

 }

  if (count_tuner_device == 0)
  {
    /*We didnot attempt calibration on any Tuner device*/
    RF_MSG(RF_ERROR,
           "Tuner not defined in RFC");
    cal_status = false;
  }

 return cal_status;
} /*do_tuner_device_calibration_top*/


/*----------------------------------------------------------------------------*/
/*!
  @brief
  get RFC data for the entire RF card

  @details
  get_rfc_data

  @return
  Status indicating that status of get rfc data.
*/
const boolean
rfc_intf::get_rfc_data
(
  void
)
{
  boolean cal_status = TRUE;

  /* collecting comm RFC data */
  if ( rfc_common_data::get_instance() == NULL ||
       rfc_common_data::get_instance()->get_rfcard_data() != TRUE )
  {
    RF_MSG( RF_ERROR, "Failed to collect RFC common data" );
    cal_status &= FALSE;
  }


  #ifdef FEATURE_GSM
  /* collectinf GSM RFC data */
  if ( rfc_gsm_data::get_instance() == NULL ||
       rfc_gsm_data::get_instance()->get_rfcard_data() != TRUE )
  {
     RF_MSG( RF_ERROR, "Failed to collect GSM RFC data" );
     cal_status &= FALSE;
  }
  #endif

   #ifdef FEATURE_WCDMA
   /* collecting WCDMA RFC data */
   if ( rfc_wcdma_data::get_instance() == NULL ||
    rfc_wcdma_data::get_instance()->get_rfcard_data() != TRUE )
   {
     RF_MSG( RF_ERROR, "Failed to collect RFC common data" );
     cal_status &= FALSE;
   }
   #endif

   #ifdef FEATURE_CDMA1X
   /* collecting CDMA RFC data */
   if ( rfc_cdma_data::get_instance() == NULL ||
    rfc_cdma_data::get_instance()->get_rfcard_data() != TRUE )
   {
      RF_MSG( RF_ERROR, "Failed to collect RFC common data" );
      cal_status &= FALSE;
   }
   #endif

   #ifdef FEATURE_TDSCDMA
   /* collecting TDSCDMA RFC data */
   if ( rfc_tdscdma_data::get_instance() == NULL ||
    rfc_tdscdma_data::get_instance()->get_rfcard_data() != TRUE )
   {
      RF_MSG( RF_ERROR, "Failed to collect RFC common data" );
      cal_status &= FALSE;
   }
   #endif /*FEATURE_TDSCDMA*/

   /* collecting GNSS RFC data */
   if ( rfc_gnss_data::get_instance() == NULL ||
    rfc_gnss_data::get_instance()->get_rfcard_data() != TRUE )
   {
     RF_MSG( RF_ERROR, "Failed to collect RFC common data" );
     cal_status &= FALSE;
   }

   #ifdef FEATURE_LTE
   if ( rfc_lte_data::get_instance() == NULL ||
    rfc_lte_data::get_instance()->get_rfcard_data() != TRUE )
   {
     RF_MSG( RF_ERROR, "Failed to collect RFC common data" );
     cal_status &= FALSE;
   }
   #endif  /* FEATURE_LTE */
   return cal_status;

}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Perform Internal Device Calibration

  @details
  This function will perform internal device calibration for all the RTRs
  present in this RF card. This function will send the current
  internal device calibration data from NV to the driver. Based on the device
  calibration status, the new calibration data will be written back to
  NV. NV will be written only if the calibration is completed successfully.

  @return
  Status indicating that status of internal device calibration.
*/
const boolean rfc_intf::do_internal_device_calibration(uint8 instance_num)
{
  rfdevice_rxtx_common_class *common_device = NULL;
  rfdevice_cmn_int_dev_cal_data_type cal_data; /* Cal Data for Device */
  boolean ret_val = TRUE; /* Return Value */
  rfcommon_nv_hdet_autocal_type hdet_autocal_result;
  nv_stat_enum_type nv_status = NV_FAIL_S;

  /* Get Common NV table Pointer */
  rfcommon_nv_tbl_type *rfnv_common_nv_tbl_ptr = rfcommon_nv_get_tbl_ptr();

  if (instance_num < 2) //max 2 instances are supported by NV for now.

  {

    /* Populate Calibration Data with pointer to NV */
    cal_data.rc_tuner.rc_process_errors = &rfnv_common_nv_tbl_ptr->rc_process_error[instance_num * 8];
    cal_data.rc_tuner.number_of_bytes = 8;

    hdet_autocal_result.dev0 = rfnv_common_nv_tbl_ptr->hdet_autocal.dev0;
    hdet_autocal_result.dev1 = rfnv_common_nv_tbl_ptr->hdet_autocal.dev1;

    switch (instance_num)
    {
    case 0:
      cal_data.hdet_autocal = &rfnv_common_nv_tbl_ptr->hdet_autocal.dev0;
      hdet_autocal_result.dev0 = *(cal_data.hdet_autocal);
      break;
    case 1:
      cal_data.hdet_autocal = &rfnv_common_nv_tbl_ptr->hdet_autocal.dev1;
      hdet_autocal_result.dev1 = *(cal_data.hdet_autocal);
      break;

    default:
      RF_MSG_1(RF_ERROR, "RFC do_internal_device_calibration: Failed, Device Instance num (%d) NOT supported",
               instance_num);
      break;
    }

#ifndef FEATURE_GNSS_ONLY_NO_WWAN
    /* This is temporary, until all the device interface change to query the RFC for device object */
    if (cmn_rf_devices[RFDEVICE_TRANSCEIVER][instance_num].device_obj != NULL)
    {
      common_device = ((rfdevice_rxtx_common_class *)cmn_rf_devices[RFDEVICE_TRANSCEIVER][instance_num].device_obj);
      /* Perform Internal Device calibration on Device */
      ret_val = rfdevice_cmn_do_internal_device_cal(common_device, &cal_data);

      /* Populate return value based on calibration status */
      RF_MSG_4(RF_HIGH, "RFC do_internal_device_calibration: Device %d "
               "overall status: %d [RC tuner %d] [HDET Autocal %d]",
               instance_num, ret_val, cal_data.rc_tuner_cal_status,
               cal_data.hdet_autocal_status);
  }
  else
    {
      /* Invalid Common device pointer; so flag failure */
      ret_val = FALSE;
      RF_MSG(RF_ERROR, "RFC do_internal_device_calibration: "
             "Invalid Device Pointer");
    } /* if(cmn_rf_devices[RFDEVICE_TRANSCEIVER][instance_num].device_obj != NULL) */
#endif

    /* Write the Calibration Data back to NV, if all calibration is successful */
    if (ret_val == TRUE)
    {
      /* Write calibration data to NV */
      rfcommon_nv_set_item(NV_RC_PROCESS_ERRORS_I,
                           (nv_item_type *)rfnv_common_nv_tbl_ptr->rc_process_error,
                           rex_self(), FS_OP_COMPLETE_SIG,
                           rfc_common_write_nv_cb);

      /* 8-bit PDET autocal performed */
      switch (instance_num)
      {
      case 0:
        hdet_autocal_result.dev0 = *(cal_data.hdet_autocal);
        break;

      case 1:
        hdet_autocal_result.dev1 = *(cal_data.hdet_autocal);
        break;

      default:
          RF_MSG_1(RF_ERROR, "RFC do_internal_device_calibration: Device Instance num "
                             "(%d) NOT supported - NV write failed",
                 instance_num);
        break;
      }

      nv_status = rfcommon_rfnv_set_item(RFNV_RFDEVICE_HDET_AUTOCAL_I,
                             (rfnv_item_type *)(&hdet_autocal_result),
                             sizeof(rfcommon_nv_hdet_autocal_type),
                             NULL,
                             (rex_sigs_type)0,
                             NULL);
      if (nv_status != NV_DONE_S)
      {
         RF_MSG_1(RF_ERROR, "RFC do_internal_device_calibration: Fail NV write status %d",nv_status);
         ret_val = FALSE;
      }
      else
      {
         RF_MSG_1(RF_MED, "RFC do_internal_device_calibration: Successful",ret_val);
      }
  }
  else
    {
      RF_MSG_1(RF_ERROR, "RFC do_internal_device_calibration: Failed",
               ret_val);
    } /* if ( ret_val == TRUE ) */
  }
  else
  {
    ret_val = FALSE;
    RF_MSG_1(RF_ERROR, "Invalid Instance number (%d)", instance_num);
  }
  /* Return */
  return ret_val;

} /* do_internal_device_calibration */


/*----------------------------------------------------------------------------*/
/*
  @brief
  Provides card specific implementation to retrieve device specific
  configuration

  @details
  This function will return all the device configuration that is supported for
  the target. The configuration includes the techs and antenna supported in each
  RF device. Along with this it will include flag indicating whether features
  like SV is supported.

  @param dev_cfg
  The container to populate the Device configuration configuraion which includes
  SV flag, tech supported, and antenna supported.

  @return
  Flag indicating if the RF Device configuration is valid or not
*/
boolean rfc_intf::get_devices_configuration(rfm_devices_configuration_type* dev_cfg)
{
  boolean status = FALSE;
  rfc_common_data *rfc_data = rfc_common_data::get_instance();
  if (NULL == rfc_data)
  {
    RF_MSG( RF_ERROR, " RFC Common data object is NOT created : rfc_common_data == NULL!" );
    return status;
  }

  status = rfc_data->get_logical_path_config(dev_cfg);
  return status;
}

/*----------------------------------------------------------------------------*/
/*
  @brief
  Provides rf-card specific ASDiv configuration information.

  @details
  This function will return rf-card specific ASDiv configuration information.

  @param config_num : Asdiv configuration ID.
  @param config_info : Asdiv configuration info. ptr.

  @return
  Flag indicating if the RF Device configuration is valid or not
*/
boolean rfc_intf::get_asdiv_config_info(int32 config_num, rfc_asdiv_config_info_type **config_info)
{
  boolean status = FALSE;
  rfc_asd_cfg_params_type cfg = {0};

  rfc_common_data *rfc_data = rfc_common_data::get_instance();
  if (NULL == rfc_data)
  {
    RF_MSG( RF_ERROR, " RFC Common data object is NOT created : rfc_common_data == NULL!" );
    return status;
  }

  cfg.asd_cfg = config_num;
  status = rfc_data->get_asd_device_info( &cfg, config_info);
  return status;
}
/*----------------------------------------------------------------------------*/
/*
  @brief
  Provides card common properties

  @details
  This function will return all the card common properties, like the feedback
  path attenuation state.

  @param ptr
  The container to populate the card common properties info.

  @return
  Flag indicating if the RF Device configuration is valid or not
*/
boolean rfc_intf::get_cmn_device_properties(rfc_cmn_properties_type **ptr)
{
  boolean status = FALSE;
  rfc_common_data *rfc_data = rfc_common_data::get_instance();
  if (NULL == rfc_data)
  {
    RF_MSG( RF_ERROR, " RFC Common data object is NOT created : rfc_common_data == NULL!" );
    return status;
  }

  status = rfc_data->get_cmn_properties(ptr);
  return status;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Interface function used to get alternate path selection
  information from specific rf card

  @param tbl_size
  Number of valid alternate path selection entries

  @retval pointer to the rfc alt path selection table
*/
rfc_alt_path_sel_type*
rfc_intf::get_alt_path_selection_tbl
(
  uint32 *tbl_size
)
{
  rfc_common_data *rfc_data = rfc_common_data::get_instance();
  if (NULL == rfc_data)
  {
    RF_MSG( RF_ERROR,
            " RFC Common data object is NOT created : rfc_common_data == NULL!" );
    return NULL;
  }
  else
  {
    return rfc_data->get_alt_path_selection_tbl(tbl_size);
  }
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Interface function used to get alternate path selection information from specific
  rf card, to be used for IRAT measurements

  @param tbl_size
  Number of valid alternate path selection entires

  @retval pointer to the rfc IRAT Alt Path selection table
*/
rfc_alt_path_sel_type*
rfc_intf::get_irat_alt_path_selection_tbl
(
  uint32 *tbl_size,
  uint32 *num_band_groups
)
{
  rfc_common_data *rfc_data = rfc_common_data::get_instance();
  if (NULL == rfc_data)
  {
    RF_MSG( RF_ERROR,
            " RFC Common data object is NOT created : rfc_common_data == NULL!" );
    return NULL;
  }
  else
  {
    return rfc_data->get_irat_alt_path_selection_tbl(tbl_size, num_band_groups);
  }
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Interface function used to update the band port mapping based on new
  alternate path to use

  @param rfm_dev
  current logical device for which path swap is requested

  @param curr_mode
  current technology of operation

  @param curr_band
  current band used in the specifc tech

  @param alt_path
  new alternate path index to use

  @retval status
  TRUE is port map update is successful else returns FALSE

*/
boolean rfc_intf::update_port_mapping
(
  rfm_device_enum_type rfm_dev,
  rfcom_mode_enum_type curr_mode,
  rf_card_band_type curr_band,
  uint8 alt_path
)
{
  boolean status = FALSE;

#ifdef FEATURE_LTE
  if (curr_mode == RFM_LTE_MODE)
  {
    rfc_lte_intf *rfc_lte = rfc_lte_intf::Get_Instance();

    if (rfc_lte == NULL)
    {
      RF_MSG( RF_ERROR, " RFC LTE INTF object is NOT created "
                        ": rfc_lte_intf == NULL!" );
      return FALSE;
    }
    else
    {
      status = rfc_lte->get_updated_rf_port(rfm_dev, curr_band, alt_path);
    }
  }
#endif

#ifdef FEATURE_WCDMA
  if (curr_mode == RFCOM_WCDMA_MODE)
  {
    rfc_wcdma *wcdma_intf = rfc_wcdma::get_instance();

    if (wcdma_intf == NULL)
    {
      RF_MSG(RF_ERROR,"RFC WCMDA INTF object is NOT created:rfc_wcdma == NULL!");
      return FALSE;
    }
    else
    {
      status = wcdma_intf->get_updated_rf_port(rfm_dev, curr_band, alt_path);
    }
  }
#endif

#ifdef FEATURE_TDSCDMA
  if (curr_mode == RFCOM_TDSCDMA_MODE)
  {
    rfc_tdscdma *tds_intf = rfc_tdscdma::get_instance();

    if (tds_intf == NULL)
    {
      RF_MSG(RF_ERROR,"RFC TDS object is NOT created:rfc_tdscdma == NULL!");
      return FALSE;
    }
    else
  {
      status = tds_intf->get_updated_rf_port(rfm_dev, curr_band, alt_path);
    }
  }
#endif

#if defined(FEATURE_CDMA1X) || defined (FEATURE_HDR)
  if( curr_mode == RFM_1X_MODE || curr_mode == RFM_1XEVDO_MODE)
  {
    status = rfc_cdma_update_alt_rx_path( rfm_dev, curr_band, alt_path );
  }
#endif

#ifdef FEATURE_GSM
  if (curr_mode == RFM_EGSM_MODE)
  {
    rfc_gsm *rfc_obj = rfc_gsm::get_instance();

    if ( rfc_obj == NULL )
    {
      MSG_1(MSG_SSID_RF, MSG_LEGACY_FATAL,"GSM RFC Obj is NOT created. ",0);
      return FALSE;
    }
    else
    {
      status = rfc_obj->get_updated_rf_port(rfm_dev, curr_band, alt_path);
    }
  }
#endif

  return status;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Interface function to get the delta devices between two alternate paths
  for the same logical device

  @param rfm_dev
  current logical device in use

  @param curr_mode
  current mode of operation

  @param curr_band
  current band in use

  @param prev_alt_path
  current alt path index used by the band for a specific logical device

  @param next_alt_path
  new alt path index to be used for the band on a specific logical device

  @retval status
  returns TRUE if two successful alt path index are found and delta devices
  have been determined, else returns FALSE

*/
boolean rfc_intf::get_delta_dev
(
  rfm_device_enum_type rfm_dev,
  rfcom_mode_enum_type curr_mode,
  rf_card_band_type curr_band,
  uint8 prev_alt_path,
  uint8 next_alt_path
)
{
  boolean status = FALSE;

#ifdef FEATURE_LTE
  if (curr_mode == RFM_LTE_MODE)
  {
    rfc_lte_intf *rfc_lte = rfc_lte_intf::Get_Instance();
    if (rfc_lte == NULL)
    {
      RF_MSG( RF_ERROR, " RFC LTE INTF object is NOT created "
                        ": rfc_lte_intf == NULL!" );
      return FALSE;
    }
    else
    {
      status = rfc_lte->get_delta_dev( rfm_dev, curr_band,
                                       prev_alt_path, next_alt_path );
    }
  }
  else
  {
    status = FALSE;
  }
#endif

  return status;
}

/*----------------------------------------------------------------------------*/
/*
  @brief
  This funtion ASDiv Tuner Mgr.

  @details
  This funtion ASDiv Tuner Mgr.

  @param
  None

  @return
  TRUE if successful, FALSE otherwise.

*/
boolean rfc_intf::create_asdiv_tuner_mgr( void )
{
  boolean status = TRUE;
#ifndef FEATURE_GNSS_ONLY_NO_WWAN
#ifdef FEATURE_RF_ASDIV
    /* Create an AsDiv tuner manager if
     * 1. Tuner manager exists
     * 2. Antenna switch is enabled
     * 3. ASD_TUNER_MANAGER is not created yet
     *
     * NOTE: Only 1 AsDiv Tuner manager is needed, assuming there
     * is only 1 antenna switch
     */
    rfm_devices_configuration_type *device_config = NULL;

    device_config = 
	#ifndef FEATURE_CLADE_HEAP
	(rfm_devices_configuration_type *)modem_mem_alloc(sizeof(rfm_devices_configuration_type), MODEM_MEM_CLIENT_RFA);
    #else
    (rfm_devices_configuration_type *)clade_heap_alloc(sizeof(rfm_devices_configuration_type));
    #endif
		
    if (device_config != NULL)
    {
      memset(device_config, 0, sizeof(rfm_devices_configuration_type));
      status = this->get_devices_configuration(device_config);
      if (status == TRUE)
      {
        if( (this->cmn_rf_devices[RFDEVICE_TUNER_MANAGER][0].device_obj!= NULL) &&
            (device_config->antenna_swap_supported == TRUE) &&
            (this->cmn_rf_devices[RFDEVICE_ASD_TUNER_MANAGER][0].device_obj == NULL))
        {
          this->cmn_rf_devices[RFDEVICE_ASD_TUNER_MANAGER][0].device_obj= rf_device_factory_create_asd_tuner_device();
        }
      }
	  
	  #ifndef FEATURE_CLADE_HEAP
      modem_mem_free(device_config, MODEM_MEM_CLIENT_RFA);
	  #else
      clade_heap_free(device_config);
      #endif  
	  
    }
    else
    {
      RF_MSG( RF_ERROR, "Cannot Allocation memory for ASD configuration");
      status = FALSE;
    }
#endif /* FEATURE_RF_ASDIV */
#endif
    return status;
}

/*----------------------------------------------------------------------------*/
/*
  @brief
  This funtion returns ptr to Dummy Tuner Mgr for the specified Antenna.

  @details
  This funtion returns ptr to Dummy Tuner Mgr for the specified Antenna.
  if the Dummy Tuner Mgr object is NOT present, create a new one.

  @param
  antenna : Antenna number for which Dummy Tuner Mgr to be returned.

  @return
  rfdevice_class * : ptr to device object for specified antenna.

*/
rfdevice_class *rfc_intf::get_dummy_tuner_mgr(uint32 antenna)
{
  uint8 counter = 0;
  rfdevice_class *tuner_dev = NULL;
  rfcommon_atuner_manager_intf *tuner_mgr_obj = NULL;

  rfc_intf *temp_rfc_p = rfc_intf::get_instance();

  if (antenna == RFC_INVALID_PARAM) // Dummy tuner is NOT needed
  {
    return (rfdevice_class *)NULL;
  }

  if (antenna >= RFC_MAX_ANTENNA)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_HIGH, "get_dummy_tuner_mgr(%d): Invalid Parameters",antenna);
    return (rfdevice_class *)NULL;
  }

      // if there is at-least one Tuner device is present and no dummy Tuner Mgr, create one.
      if (cmn_rf_devices[RFDEVICE_TUNER][0].device_obj!= NULL && dummy_tuner_mgr[antenna] == NULL)
      {
        /* Loop thru to find the next available spot in TUNER_MANAGER array */
        while ( counter < RFC_MAX_DEVICE_INSTANCES)
        {
          if ( cmn_rf_devices[RFDEVICE_TUNER_MANAGER][counter].device_obj!=NULL )
          {
            counter++;
          }
          else
          {
            /*Create a Tuner factory object to create dummy tuner manager */
            rfcommon_atuner_manager_factory *tuner_mgr_factory = NULL;

            /* Create the instance for Tuner mananger factory */
            tuner_mgr_factory = new rfcommon_atuner_manager_factory (  );

            /* Check if tuner_mgr_factory is valid*/
            if(tuner_mgr_factory != NULL)
            {
              /*Using the tuner manager factory, we can create the Dummy tuner */
              /*** Non-Null spot found, fill in the dummy tuner manager ***/
              cmn_rf_devices[RFDEVICE_TUNER_MANAGER][counter].device_obj =
                              tuner_mgr_factory->create_atuner_manager( ATUNER_DUMMY_MANAGER,
                                                                        NULL, /*No valid device object */
                                                                        counter); /*Instance number of Dummy Manager */

            /* Update the Dummy_Tuner_Manager array */
            dummy_tuner_mgr[antenna] = cmn_rf_devices[RFDEVICE_TUNER_MANAGER][counter].device_obj;

          MSG_2(MSG_SSID_RF, MSG_LEGACY_MED,"Create dummy tuner for antenna=%d, obj=0x%x",
                                                 antenna, dummy_tuner_mgr[antenna]);
          delete tuner_mgr_factory;
        }/*if(tuner_mgr_factory != NULL)*/
            else
            {
          RF_MSG( RF_ERROR,"get_dummy_tuner_mgr() failed as Tuner manager factory instance cannot be created!");
            }
            break;
      }/*else if ( cmn_rf_devices[RFDEVICE_TUNER_MANAGER][counter].device_obj!=NULL )*/
    }/*while ( counter < RFC_MAX_DEVICE_INSTANCES)*/
  }/*if (cmn_rf_devices[RFDEVICE_TUNER][0].device_obj!= NULL && dummy_tuner_mgr[antenna] == NULL)*/

  MSG_2(MSG_SSID_RF_DEBUG, MSG_LEGACY_HIGH,"Return dummy_tuner_mgr[antenna=%d] = 0x%x", antenna,
                                      dummy_tuner_mgr[antenna]);
  return dummy_tuner_mgr[antenna];
}

/*----------------------------------------------------------------------------*/
/*
  @brief
  Provides command dispatch interface to retrieve certain card specific
  details.

  @details

  @param

*/
void rfc_intf::command_dispatch( rfm_path_enum_type path, rfcs_cmd_type cmd, void *data )
{
  switch (cmd)
  {
    case RFC_GET_RTR_TX_PHYSICAL_BUS:
      if (data != NULL)
      {
        *((uint32 *)data) = SSBI_BUS_1;
      }
      break;

    case RFC_GET_RTR_RX_PHYSICAL_BUS:
      if (data != NULL)
      {
        *((uint32 *)data) = SSBI_BUS_2;
      }
      break;

    case RFC_GET_APT_CONFIG:
      if (data != NULL)
      {
        switch (((rfc_xpt_supported_type*)data)->band)
        {
          case RF_BCIX_BAND:
            ((rfc_xpt_supported_type*)data)->apt_config = RFC_APT_TX_PDM;
            break;

          default:
            ((rfc_xpt_supported_type*)data)->apt_config = RFC_APT_NOT_SUPPORTED;
            break;
        }
      }
      break;

    default:
      MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR, "Command %d, Not support in %d rf_card", cmd, rfc_card_id);
      break;
  }
}

boolean rfc_get_devices_configuration(rfm_devices_configuration_type *dev_cfg)
{
  boolean dev_cfg_valid = FALSE;

  rfc_intf *temp_rfc_p = rfc_intf::get_instance();

  if(temp_rfc_p != NULL)
  {
    dev_cfg_valid = temp_rfc_p->get_devices_configuration(dev_cfg);
  }
  return dev_cfg_valid;
}

const rfm_devices_configuration_type* rfc_get_logical_device_properties( void )
{
  rfc_common_data * cmn_data_ptr = rfc_common_data::get_instance();
  if (cmn_data_ptr == NULL)
  {
    MSG( MSG_SSID_RF, MSG_LEGACY_ERROR, 
         "rfc_get_logical_device_properties - NULL cmn data ptr." );
    return NULL;
  }

  return cmn_data_ptr->get_logical_device_properties();
}

void rfc_get_awake_resource_request(rfm_resource_info *dev_cfg)
{
  rfc_intf *temp_rfc_p = rfc_intf::get_instance();

  if(temp_rfc_p != NULL)
  {
    temp_rfc_p->get_awake_resource_request(dev_cfg);
  }
}

void rfc_set_quiet_mode(rfm_mode_enum_type mode, boolean on_off)
{
  rfc_intf *temp_rfc_p = rfc_intf::get_instance();

  if(temp_rfc_p != NULL)
  {
    temp_rfc_p->set_quiet_mode(mode, on_off);
  }
}

void rfc_common_enable_autocal_vregs(boolean on_off)
{
  rfc_intf *temp_rfc_p = rfc_intf::get_instance();

  if(temp_rfc_p != NULL)
  {
    if (on_off == TRUE)
    {
      /* Enable vregs before config GPIO for common wakeup*/
      temp_rfc_p->common_enable_autocal_vregs(on_off);
      /* enable the rf signals, if the signals are in low power mode */
      rfc_common_wakeup(RF_PATH_0, RFM_PARKED_MODE);
    }
    else
    {
      /* put the rf signals inot low power mode */
      rfc_common_sleep(RF_PATH_0, RFM_PARKED_MODE);
      /* Disable vreg after config GPIO for common sleep*/
      temp_rfc_p->common_enable_autocal_vregs(on_off);
    }
  }
}

void rfc_common_enable_mpp_dac_vref(boolean on_off)
{
  rfc_intf *temp_rfc_p = rfc_intf::get_instance();

  if(temp_rfc_p != NULL)
  {
    if (on_off == TRUE)
    {
      /* Turn on MPP for DAC Vref */
      temp_rfc_p->common_enable_mpp_dac_vref(on_off);
    }
    else
    {
      /* Turn off MPP for DAC Vref */
      temp_rfc_p->common_enable_mpp_dac_vref(on_off);
    }
  }
}

void rfc_lte_light_sleep_wakeup_manage_vregs(void)
{
  rfc_intf *temp_rfc_p = rfc_intf::get_instance();

  if(temp_rfc_p != NULL)
  {
     temp_rfc_p->lte_light_sleep_wakeup_vregs();
  }
}

void rfc_lte_light_sleep_manage_vregs(void)
{
  rfc_intf *temp_rfc_p = rfc_intf::get_instance();

  if(temp_rfc_p != NULL)
  {
     temp_rfc_p->lte_light_sleep_vregs();
  }
}


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
const rfc_common_logical_device_params_type*
rfc_common_get_logical_device_params
(
  rfm_device_enum_type device
)
{

  const rfc_common_logical_device_params_type *dev_param;

  /* Get RFC Object */
  rfc_intf *temp_rfc_p = rfc_intf::get_instance();

  if ( temp_rfc_p == NULL )
  {
    RF_MSG( RF_ERROR, "rfc_common_get_logical_device_params: Obj Not created");
    dev_param = NULL;
  }
  else
  {
    /* Get Device Parameter */
    dev_param = temp_rfc_p->get_logical_dev_param(device);
  }

  return dev_param;

} /* rfc_common_get_logical_device_params */
/*----------------------------------------------------------------------------*/
/*!
  @brief
  This funtion returns the common device object ptr for the specified device type and instance
  number.

  @details
  This function will return all of the platform-dependent parameters to
  configure the proper hardware devices.

  @param txdac
  Chain being queried

  @return
  Pointer to the Logical Device parameter structure
*/
rfdevice_rxtx_common_class*
rfc_common_get_transceiver_for_chain
(
  txlm_dac_type txdac
)
{

  rfdevice_rxtx_common_class* transceiver_device = NULL;
  rfdevice_class* device_obj = NULL;
  rfdevice_type_enum_type device_type = RFDEVICE_TYPE_INVALID;

  /* Get RFC Object */
  rfc_intf *temp_rfc_p = rfc_intf::get_instance();

  if ( temp_rfc_p == NULL )
  {
    RF_MSG( RF_ERROR, "rfc_common_get_transceiver_for_chain: Obj Not created");
    transceiver_device = NULL;
  }
  else if(txdac >= TXLM_DAC_MAX)
  {
    RF_MSG_1( RF_ERROR, "rfc_common_get_transceiver_for_chain: Requested DAC %d out of bounds", txdac);
    transceiver_device = NULL;
  }
  else
  {
    /* Get Device Parameter */
    device_obj = temp_rfc_p->get_txdac_trx_device(txdac);
    if(device_obj != NULL)
    {
      device_type = device_obj->rfdevice_type_get();
      if(device_type == RFDEVICE_TRANSCEIVER || device_type == RFDEVICE_TRANSMITTER)
      {
        transceiver_device = (rfdevice_rxtx_common_class*)device_obj;
      }
      else
      {
        RF_MSG_1( RF_ERROR, "rfc_common_get_transceiver_for_chain: Incorrect object returned for DAC %d",txdac);
        transceiver_device = NULL;
      }
    }
    else
    {
      RF_MSG_1( RF_ERROR, "rfc_common_get_transceiver_for_chain: WTR obj not found for DAC %d", txdac);
      transceiver_device = NULL;
    }
  }

  return transceiver_device;

} /* rfc_common_get_logical_device_params */


/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get Phyisical Device Ptrs

  @details
  This function will return the pointer to the RFC device
  queried.

  @param dev_type
  dev_type is a type of the RF device in RFC.

 @param instance
  Instance is a counter.

  @return
  Pointer to the Logical Device parameter structure
*/
rfdevice_rxtx_common_class*
rfc_common_get_cmn_device_object
(
  uint8 instance
)
{
  rfdevice_rxtx_common_class *temp_rfdevice_class = NULL;

  /* Get RFC Object */
  rfc_intf *temp_rfc_p = rfc_intf::get_instance();

  /* Perform check on individual INPUT PARAMS */
  if(( temp_rfc_p == NULL ) ||
     (instance >= RFC_MAX_DEVICE_INSTANCES))
  {
    RF_MSG_1( RF_ERROR, "rfc_common_get_cmn_device_param: Obj Not created"
                      " or Invalid parameters: (Instance: %d):",
                      instance ) ;
  }
  else
  {
    temp_rfdevice_class = (rfdevice_rxtx_common_class*)
                          temp_rfc_p->get_cmn_rf_device_object( RFDEVICE_TRANSCEIVER, instance);
  }

  return temp_rfdevice_class ;

} /* rfc_common_get_cmn_device_param */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This funtion returns the common device object ptr for the specified device
  type and instance number.

  @details
  This is a wrapper C function to the CPP implementation of the RFC API
  which does the same job.
  This funtion returns the common device object ptr for the specified device
  type and instance number.

  @param
  dev_type : type of device
  instance : instance number for the device type.

  @return
  rfdevice_class * : device object ptr.

*/
rfdevice_class*
rfc_common_get_cmn_rf_device_object
(
  rfdevice_type_enum_type dev_type,
  uint8 instance
)
{
  /* Get RFC Object */
  rfc_intf *temp_rfc_p = rfc_intf::get_instance();

  /* Perform check on individual INPUT PARAMS */
  if(( temp_rfc_p == NULL ) ||
     (instance >= RFC_MAX_DEVICE_INSTANCES))
  {
    RF_MSG_1( RF_ERROR, "rfc_common_get_cmn_device_param: Obj Not created"
                      " or Invalid parameters: (Instance: %d):",
                      instance ) ;
    return NULL;
  }
  else
  {
    return temp_rfc_p->get_cmn_rf_device_object( dev_type, instance);
  }

} /* rfc_common_get_cmn_rf_device_object */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get Phyisical Device Ptrs

  @details
  This function will return all of the platform-dependent parameters to
  configure the proper hardware devices.

  @param dev_type
  dev_type is a type of the RF device in RFC.

  @param instance
  Instance is a counter

  @return
  Pointer to the Logical Device parameter structure
*/
rfdevice_rxtx_common_class*
rfc_common_get_cmn_device_param
(
  rfdevice_type_enum_type dev_type,
  uint8 instance
)
{

  rfdevice_rxtx_common_class *temp_rfdevice_class = NULL;

  /* Get RFC Object */
  rfc_intf *temp_rfc_p = rfc_intf::get_instance();

  /* Perform check on individual INPUT PARAMS */
  if(( temp_rfc_p == NULL )||
    (dev_type >= RFDEVICE_TYPE_MAX_NUM) ||
     (instance >= RFC_MAX_DEVICE_INSTANCES))
  {
    RF_MSG_2( RF_ERROR, "rfc_common_get_cmn_device_param: Obj Not created"
                      " or Invalid parameters: (%d, %d):",
            dev_type, instance);
  }
  else
  {
    temp_rfdevice_class = (rfdevice_rxtx_common_class*)
                          temp_rfc_p->get_cmn_rf_device_object(dev_type, instance);

  }

  return temp_rfdevice_class;

} /* rfc_common_get_cmn_device_param */


/*----------------------------------------------------------------------------*/
/*!
  @brief
  Perform Internal Device Calibration

  @details
  This function will perform internal device calibration for all the RTRs
  present in this RF card. This function will send the current
  internal device calibration data from NV to the driver. Based on the device
  calibration status, the new calibration data will be written back to
  NV. NV will be written only if the calibration is completed successfully.

  @return
  Status indicating that status of internal device calibration.
*/
const boolean
rfc_common_do_internal_device_calibration
(
  void
)
{

  boolean ret_val = FALSE;
  /* Get RFC Object */
  rfc_intf *temp_rfc_p = rfc_intf::get_instance();

  if ( temp_rfc_p == NULL )
  {
    RF_MSG( RF_ERROR, "rfc_common_do_internal_device_calibration: "
                      "Obj Not created");
    ret_val = FALSE;
  }
  else
  {
    ret_val = temp_rfc_p->do_internal_device_calibration_top();
  }

  return ret_val;

} /* rfc_common_do_internal_device_calibration */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get RFC crad complete data

  @details
  This function will get all data structures data in this RF card.

  @return
  Status indicating that status of RF card data.
*/
const boolean rfc_common_get_rfc_data
(
  void
)
{

  boolean ret_val = FALSE;
  /* Get RFC Object */
  rfc_intf *temp_rfc_p = rfc_intf::get_instance();

  if ( temp_rfc_p == NULL )
  {
    RF_MSG( RF_ERROR, "rfc_common_get_rfc_data: "
                      "Obj Not created");
    ret_val = FALSE;
  }
  else
  {
    ret_val = temp_rfc_p->get_rfc_data();
  }

  return ret_val;

} /* rfc_common_get_rfc_data */


/*----------------------------------------------------------------------------*/
/*!
  @brief
  Perform Tuner Device Calibration Only

  @details
  This function will perform Tuner device calibration only for Nok

  @return
  Status indicating that status of internal device calibration.
*/
const boolean
rfc_common_do_tuner_device_calibration
(
  void
)
{

  boolean ret_val = FALSE;
  /* Get RFC Object */
  rfc_intf *temp_rfc_p = rfc_intf::get_instance();

  if ( temp_rfc_p == NULL )
  {
    RF_MSG( RF_ERROR, "rfc_common_do_tuner_device_calibration: "
                      "Obj Not created");
    ret_val = FALSE;
  }
  else
  {
    ret_val = temp_rfc_p->do_tuner_device_calibration_top();
  }

  return ret_val;

} /* rfc_common_do_tuner_device_calibration */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Load Internal Device Calibration Data

  @details
  This function will load internal device calibration data for all the RTRs
  present in this RF card.

  @return
  Status indicating that loading of internal device calibration.
*/
const boolean
rfc_load_internal_device_calibration
(
  void
)
{

  boolean ret_val = FALSE;

  /* Get RFC Object */
  rfc_intf *temp_rfc_p = rfc_intf::get_instance();

  if ( temp_rfc_p == NULL )
  {
    RF_MSG( RF_ERROR, "rfc_common_load_internal_device_cal_data: "
                      "Obj Not created");
    ret_val = FALSE;
  }
  else
  {
    /* Get Device Parameter */
    ret_val = temp_rfc_p->load_internal_device_calibration();
  }

  return ret_val;

} /* rfc_load_internal_device_calibration */

/*----------------------------------------------------------------------------*/
system_clock_enum rfc_card_get_sys_clk_type(void)
{
  rfc_intf *temp_rfc_p = rfc_intf::get_instance();

  if(temp_rfc_p != NULL)
  {
    return temp_rfc_p->get_sys_clk_type();
  }
  return SYS_CLK_XO;
}

/*----------------------------------------------------------------------------*/
int32 rfc_command_dispatch( rfm_path_enum_type path, int32 cmd, void *data )
{

  rfc_intf *temp_rfc_p = rfc_intf::get_instance();
  if (temp_rfc_p != NULL )
  {
    temp_rfc_p->command_dispatch(path, (rfcs_cmd_type)cmd, data);
    return 1;
  }
  return 0;
}

/*----------------------------------------------------------------------------*/
boolean rfc_get_cmn_device_properties(rfc_cmn_properties_type **ptr)
{
  boolean dev_cfg_valid = FALSE;

  rfc_intf *temp_rfc_p = rfc_intf::get_instance();

  if(temp_rfc_p != NULL)
  {
    dev_cfg_valid = temp_rfc_p->get_cmn_device_properties(ptr);
  }
  return dev_cfg_valid;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Function to retrieve alternate path selection info specific to a rf card

  @details
  Function returns a pointer to the alt path selection table as well as the count
  for total no of entries and size per entry. This is used by concurrency
  manager to detect the device mapping and allocate alternate path accordingly

  @param *tbl_size
  pointer to get the table size or total no of entries

  @param *list_size
  pointer to get the size of each path selection entry

  @retval rfc_alt_path_sel_type
  pointer to the path selection table specific to a rf card

*/
rfc_alt_path_sel_type* rfc_get_alt_path_tbl
(
  uint32 *tbl_size
)
{
  rfc_intf *temp_rfc_p = rfc_intf::get_instance();

  if (temp_rfc_p != NULL)
  {
    return temp_rfc_p->get_alt_path_selection_tbl(tbl_size);
  }
  else
  {
    return NULL;
  }
}



/*----------------------------------------------------------------------------*/
/*!
  @brief
  Function to retrieve IRAT alternate path selection info specific to a rf card

  @details
  Function returns a pointer to the IRAT measurement specific alt path selection
  table as well as the count for total no of entries and size per entry. This is
  used by concurrency manager to detect the device mapping and allocate
  alternate path accordingly

  @param *tbl_size
  pointer to get the table size or total no of entries

  @param *list_size
  pointer to get the size of each path selection entry

  @retval rfc_alt_path_sel_type
  pointer to the path selection table specific to a rf card

*/
rfc_alt_path_sel_type* rfc_get_irat_alt_path_tbl
(
  uint32 *tbl_size,
  uint32 *num_band_groups
)
{
  rfc_intf *temp_rfc_p = rfc_intf::get_instance();

  if (temp_rfc_p != NULL)
  {
    return temp_rfc_p->get_irat_alt_path_selection_tbl(tbl_size, num_band_groups); 
  }
  else
  {
    return NULL;
  }
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Function to update alternate path for all rf devices

  @details
  Function is used to update band port mapping for all rf devices which require
  a band port map update to set up a new rf path

  @param rfm_dev
  current logical device for which path update is requested

  @param curr_mode
  current techology which is requesting the path update

  @param rf_band
  current tech specific band which needs alt path update

  @retval status
  TRUE indicates band port update is successful, else return FALSE

*/
boolean rfc_update_alt_port_mapping
(
  rfm_device_enum_type rfm_dev,
  rfcom_mode_enum_type curr_mode,
  rf_card_band_type rf_band,
  uint8 alt_path
)
{
  boolean dev_cfg_valid = FALSE;
  rfc_intf *temp_rfc_p = rfc_intf::get_instance();
  if (temp_rfc_p != NULL)
  {
    dev_cfg_valid = temp_rfc_p->update_port_mapping(rfm_dev, curr_mode,
                                                    rf_band, alt_path);
  }
  return dev_cfg_valid;
}


/*----------------------------------------------------------------------------*/
/*!
  @brief
  Function to apply the Xo drive strength to appropriate PMIC XO clocks assigned to RF.

  @details
  In the API create_cmn_rf_devices, we loop through different logical devices and
  if the device is a tranceiver, the required XO drive strength for each WTR is set.
  The required XO clock index is also set for each WTR.
  XO clock index indicates which PMIC clock needs to be set for a given WTR.

  All the aforementioned information is populated in a private variable 'rf_xo_clk_info'
  in the RFC common base class.

  if the XO clock index is RF_XO_MAX_CLKS, it means that for the given WTR, all the XO
  clocks assigned to RF must be set to desired XO drive strength.

  In future, for portability, the XO clock index and XO drive strength for a given WTR
  needs to be specified in RFC SS.

  @param none

  @retval status
  TRUE indicates XO drive strength is set successfully.

*/
boolean rfc_set_xo_drive_strength(void)
{
  int8 clk_index;
  rfc_xo_settings_type* xo_info_ptr = NULL;
  pm_clk_type clk;
  uint8 xo_set_flag;
  boolean ret_status = TRUE;

  uint8 xo_drive_strength;
  uint8 xo_clk_index = RF_XO_CLK_INVALID;
  uint8 start_clk_count = 0;
  uint8 end_clk_count = 0;
  uint8 clk_count;
  uint8 index;

  rfdevice_type_enum_type logical_dev_type;
  uint8 logical_dev_instance;

  uint8 num_trx = 0;

#ifdef FEATURE_THOR_MODEM
  rf_hw_type rf_hw_id;
#endif  

  /*Logical device*/
  rfc_logical_device_info_type *logical_devices_cfg = NULL;

 /*Get the rfc instance*/
  rfc_common_data *rfc_data = rfc_common_data::get_instance();

  rfc_intf *rfc_p = rfc_intf::get_instance();

  if (NULL == rfc_data)
  {
    RF_MSG( RF_ERROR, " RFC Common data object is NOT created : rfc_common_data == NULL!" );
    return FALSE;
  }

  /*Get the logical device config structure from RFC*/
  logical_devices_cfg = rfc_data->get_logical_device_cfg();

  if(NULL == logical_devices_cfg)
  {
    RF_MSG( RF_ERROR, " RFC rf device Configuration data not available!" );
    return FALSE;
  }

  if (rfc_p == NULL)
  {
    return FALSE;
  }

  xo_info_ptr = rfc_p->get_rfc_xo_drive_strength_info();

  if (xo_info_ptr == NULL)
  {
    return FALSE;
  }

  index=0;


  memset(xo_info_ptr, 0, RF_XO_MAX_CLKS * sizeof(rfc_xo_settings_type));

  while(logical_devices_cfg[index].rf_device_type != RFDEVICE_TYPE_INVALID)
  {
      logical_dev_type = logical_devices_cfg[index].rf_device_type;
      logical_dev_instance = logical_devices_cfg[index].rf_asic_id;


      /* ------------------Select XO drive strength based on the WTR  --------------------*/
      if (logical_dev_type == RFDEVICE_TRANSCEIVER)
      {

              /*Check if the device instances are within range*/
        if (logical_dev_instance >= RFC_MAX_DEVICE_INSTANCES)
        {
          RF_MSG_1( RF_HIGH, "Detected logical_dev_instance >= RFC_MAX_DEVICE_INSTANCES for RFDEVICE_TRANSCEIVER, logical_dev_instance=%d",
                  logical_dev_instance);
          return FALSE;
        }

        num_trx++;

        /* To do: get the RHS info from RFC*/
        switch (logical_devices_cfg[index].rf_device_id )
        {
        case WTR2605:
           xo_drive_strength = PM_CLK_BUFF_OUT_DRV__3X;
           xo_clk_index = RF_XO_MAX_CLKS;
            break;

        default:
           xo_drive_strength = PM_CLK_BUFF_OUT_DRV__1X;
           xo_clk_index = RF_XO_MAX_CLKS;
            break;
        }

        if (xo_clk_index <= RF_XO_MAX_CLKS)
        {
          /*
           * Unless all the XO clks need to be set to same xo drive strength, Set only
           * the XO clock for corresponding clock index to desired xo drive strength.
           * Else, it will loop through all XO clocks to set desired xo drive strength
           */
          start_clk_count = (xo_clk_index ==  RF_XO_MAX_CLKS)? 0 : xo_clk_index;
          end_clk_count = (xo_clk_index ==  RF_XO_MAX_CLKS)? RF_XO_MAX_CLKS : start_clk_count + 1;

          for (clk_count = start_clk_count; clk_count < end_clk_count; clk_count++)
          {
            xo_info_ptr[clk_count].active = TRUE;

            xo_info_ptr[clk_count].trx_count++;

            /* Store max drive strength each XO clock needs to be set to*/
            if (xo_drive_strength > xo_info_ptr[clk_count].drive_strength)
            {
              xo_info_ptr[clk_count].drive_strength = xo_drive_strength;
            }
          }
        }
      }
      /* --------------------------------------------------------------------------------*/

    index++;
  }

#ifdef FEATURE_THOR_MODEM
  //Find out the RF card id used
  rf_hw_id = rfcommon_nv_get_rf_config();
#endif

  /*Set the XO drive strength*/

  for (clk_index = 0; clk_index < RF_XO_MAX_CLKS; clk_index++)
  {
    /*Check if XO clock is indeed needed*/
    if (TRUE == xo_info_ptr[clk_index].active)
    {
      /* RF XO clk enum to PMIC XO clk enum mapping*/
      switch ((rfc_xo_clk_type)clk_index)
      {
      case RF_XO_CLK_1:
        clk = PM_RF_CLK1;
        break;

      case RF_XO_CLK_2:
        clk = PM_RF_CLK2;
        break;

      default:
        clk = PM_RF_CLK1;
        break;
      }

#ifdef FEATURE_BOLT_MODEM
      /*Set the resultant XO drive strength*/
//      xo_set_flag = pm_dev_clk_buff_set_output_drive_strength( 0, clk, (pm_clk_drv_strength_type)xo_info_ptr[clk_index].drive_strength);
      xo_set_flag = PM_ERR_FLAG__SUCCESS;
#else
//      xo_set_flag = pm_dev_clk_buff_set_output_drive_strength( 0, clk, (pm_clk_drv_strength_type)xo_info_ptr[clk_index].drive_strength);
      xo_set_flag = PM_ERR_FLAG__SUCCESS;
#endif

      if(PM_ERR_FLAG__SUCCESS != (pm_err_flag_type)xo_set_flag)
      {
        RF_MSG_3( RF_ERROR, "Set XO Drive strength:%d for PMIC clk: %d failed. Error type:%d",xo_info_ptr[clk_index].drive_strength, clk, xo_set_flag);
        ret_status = FALSE;
      }
      else
      {
        /* Set the indicator flag if XO drive strength was set successfully or not*/
        xo_info_ptr[clk_index].xo_set = TRUE;
      }

#ifdef FEATURE_THOR_MODEM
    if ((rf_hw_id == RF_HW_WTR3925_TP160_0) ||(rf_hw_id == RF_HW_WTR3925_TP160_1) || 
            (rf_hw_id == RF_HW_WTR3925_TP160_2) || (rf_hw_id == RF_HW_WTR3925_TP160_01) || 
            (rf_hw_id == RF_HW_WTR3925_TP160_02) || (rf_hw_id == RF_HW_WTR3925_3DLCA_NAEU3100V2_PW) ||
            (rf_hw_id == RF_HW_WTR3925_TP160_3) )
{
   break;
}
#endif

#if ((defined (FEATURE_BOLT_MODEM)) && (!defined(FEATURE_BOLT_PLUS_MODEM)))

      /* For Bolt and other relevant modems, only program PM_RF_CLK1 if only single TRX present*/
      if (num_trx == 1)
      {
        break;
      }
#endif
    }
  }

  return ret_status;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Function to return pointer to the private variable 'rf_xo_clk_info'
  in the RFC common base class.

  @param none

  @retval rfc_xo_settings_type*
*/
rfc_xo_settings_type* rfc_intf::get_rfc_xo_drive_strength_info( )
{
  return rf_xo_clk_info;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Dumps EFS device information to EFS. 
  Includes physical device info and device availability

  @param phy_devices_cfg 
  physical device info from RFC
 
  @retval TRUE if dump completed successfully
*/ 
boolean rfc_intf::rfc_rffe_bus_scan(void)
{
  uint8   phy_dev_instance;
  boolean is_unique_device;
  uint32  error_code          = 0;
  uint8   unique_device_count = 0;
  uint8   cfg_index           = 0;
  uint8   cfg_index_2         = 0;
  uint32  num_present         = 0;
  uint32  efs_buffer_index    = RFC_RFFE_SCAN_HEAD_SIZE;
  boolean dump_status         = TRUE;

  rfc_common_data *rfc_data = rfc_common_data::get_instance();
  rfc_phy_device_info_type *phy_devices_cfg = NULL;
  boolean skip_body = FALSE;

  // Set to false to disable this feature
  if(!RFC_ENABLE_RFFE_SCAN_EFS_DUMP)
  {
    return TRUE;
  }
  
  // First delete the existing dump file
  efs_deltree(RFC_RFFE_SCAN_PATH);

  if(this->get_init_status() == FALSE)
  {
    RF_MSG( RF_ERROR, "rfc_rffe_bus_scan: rfc_intf object creation failed" );
    error_code = RFC_RFFE_SCAN_ERROR_RFC_INIT_FAILED;
  }
  
  // ====================================================================
  //                       Validity check
  // ====================================================================
  if (NULL == rfc_data)
  {
    RF_MSG( RF_ERROR, "rfc_rffe_bus_scan: RFC common data object is NOT created" );
    skip_body = TRUE;
    error_code = RFC_RFFE_SCAN_ERROR_RFC_DATA_MISSING;
  }
  else
  {
    // Get the physical device config structure from RFC
    phy_devices_cfg = rfc_data->get_phy_device_cfg();
    
    if (NULL == phy_devices_cfg)
    {
      RF_MSG( RF_ERROR, "rfc_rffe_bus_scan: RFC rf device configuration data not available!" );
      skip_body = TRUE;
      error_code = RFC_RFFE_SCAN_ERROR_DEV_CFG_MISSING;
    }
  }

  // ====================================================================
  //                  Allocate memory for buffer
  // ====================================================================
  uint32* efs_buffer = new uint32[RFC_RFFE_SCAN_HEAD_SIZE + (RFC_RFFE_SCAN_BODY_SIZE * (phys_devices_count))];

  if (NULL == efs_buffer)
  {
    RF_MSG(RF_ERROR, "rfc_rffe_bus_scan: Unable to allocate memory to form device dump");
    return FALSE;
  }

  // ====================================================================
  //                     Populate header info
  // ====================================================================
  efs_buffer[RFC_RFFE_SCAN_VERSION_IDX]     = RFC_RFFE_SCAN_VERSION;
  efs_buffer[RFC_RFFE_SCAN_HEAD_SIZE_IDX]   = RFC_RFFE_SCAN_HEAD_SIZE;
  efs_buffer[RFC_RFFE_SCAN_BODY_SIZE_IDX]   = RFC_RFFE_SCAN_BODY_SIZE;
  efs_buffer[RFC_RFFE_SCAN_RFHW_ID_IDX]     = rfc_card_id;
  efs_buffer[RFC_RFFE_SCAN_NUM_DEVICES_IDX] = 0;
  efs_buffer[RFC_RFFE_SCAN_NUM_PRESENT_IDX] = 0;
  efs_buffer[RFC_RFFE_SCAN_ERROR_CODE_IDX]  = error_code;

  // ====================================================================
  //                Populate body with device info
  // ====================================================================
  while(skip_body == FALSE && phy_devices_cfg[cfg_index].rf_device_id != RFDEVICE_INVALID)
  {
    phy_dev_instance = phy_devices_cfg[cfg_index].phy_dev_instance;

    // Skip duplicate physical devices
    is_unique_device = TRUE;
    cfg_index_2 = 0;

    while((cfg_index_2 < cfg_index) && is_unique_device)
    {
      if(phy_devices_cfg[cfg_index_2++].phy_dev_instance == phy_dev_instance)
      {
        is_unique_device = FALSE;
      }
    }

    if(is_unique_device)
    {
      unique_device_count++;

      // List of device info to be dumped. (Remember to update RFC_EFS_DEV_DUMP_BODY_SIZE and RFC_RFFE_SCAN_VERSION_IDX)
      efs_buffer[efs_buffer_index++] = this->phys_devices_array[phy_dev_instance].phy_dev_cfg->rf_device_id;
      efs_buffer[efs_buffer_index++] = this->phys_devices_array[phy_dev_instance].phy_dev_cfg->phy_dev_instance;
      efs_buffer[efs_buffer_index++] = this->phys_devices_array[phy_dev_instance].phy_dev_cfg->alternate_part_idx;
      efs_buffer[efs_buffer_index++] = this->phys_devices_array[phy_dev_instance].phy_dev_cfg->bus[0];
      efs_buffer[efs_buffer_index++] = this->phys_devices_array[phy_dev_instance].phy_dev_cfg->manufacturer_id;
      efs_buffer[efs_buffer_index++] = this->phys_devices_array[phy_dev_instance].phy_dev_cfg->product_id;
      efs_buffer[efs_buffer_index++] = this->phys_devices_array[phy_dev_instance].phy_dev_cfg->product_rev;
      efs_buffer[efs_buffer_index++] = this->phys_devices_array[phy_dev_instance].phy_dev_cfg->default_usid_range_start;
      efs_buffer[efs_buffer_index++] = this->phys_devices_array[phy_dev_instance].phy_dev_cfg->default_usid_range_end;
      efs_buffer[efs_buffer_index++] = this->phys_devices_array[phy_dev_instance].phy_dev_cfg->assigned_usid;
      efs_buffer[efs_buffer_index++] = this->phys_devices_array[phy_dev_instance].device_status;

      // Count the number of devices present
      if(this->phys_devices_array[phy_dev_instance].device_status == RFC_DEVICE_PRESENT)
      {
        num_present++;
      }
    }

    cfg_index++;
  }

  // Go back and finish header
  efs_buffer[RFC_RFFE_SCAN_NUM_DEVICES_IDX] = unique_device_count;
  efs_buffer[RFC_RFFE_SCAN_NUM_PRESENT_IDX] = num_present;

  // ====================================================================
  //                     Send buffer to EFS
  // ====================================================================
  if(rfcommon_efs_put(RFC_RFFE_SCAN_PATH, efs_buffer, efs_buffer_index * sizeof(uint32), FALSE))
  {
    RF_MSG(RF_MED, "rfc_rffe_bus_scan: Data stored to EFS");
  }
  else
  {
    /* Error writing EFS */    
    RF_MSG(RF_ERROR, "rfc_rffe_bus_scan: Unable to store data to EFS");
    dump_status = FALSE;
  }

  delete [] efs_buffer;
  return dump_status;
}

/*API to set the transciever to ADC mapping 
 
  @param trx_logical_instance
  logical instance of the transciever
 
  @param phy_path
  phyiscal path
 
  @param modem_chain
  modme chain to indicate the ADC
 
  @retval
  boolean indicating pass or fail

*/
boolean rfc_intf::set_trx_phy_path_adc_mapping(                                            
                                                   uint32 rx_modem_chain,       /*modem chain to map the ADC*/
                                                   uint32 phy_path_num,         /*physical path*/
                                                   uint32 trx_logical_instance, /* Transciever device type instance*/
                                                   rfcom_mode_enum_type mode,  
                                                   rfcom_band_type_u band,
                                                   rfdevice_class* trx_device,
                                                   int32* data_array,
                                                   uint8 array_size
                                                 )
{
  boolean status = TRUE;
  rfdevice_rx_vco_path_enum_type vco_path = WTR_RX_INVALID_VCO_PATH;

  /*parameter validity check*/
  if ( (trx_logical_instance >= RFC_MAX_DEVICE_INSTANCES) ||
       (rx_modem_chain >= RXLM_CHAIN_MAX)                 ||
       (mode == RFM_INVALID_MODE)                         ||
       (array_size > RFC_ASIC_INFO_DATA_SIZE)             ||
       (trx_device == NULL)                               ||
       (data_array == NULL)
      )
  {
    RF_MSG_5(RF_ERROR, "set_trx_phy_path_adc_mapping(): invalid parameters passed. "
                       "trx_logical_instance= %d, phy_path_num=%d, rx_modem_chain=%d,mode=%d,array_size=%d", 
                        trx_logical_instance, phy_path_num, rx_modem_chain,mode,array_size);
    return FALSE;  
  }

  switch (mode)
  {
 #ifdef FEATURE_WCDMA
    case RFCOM_WCDMA_MODE:

      if(band.wcdma_band > RFCOM_NUM_WCDMA_BANDS)
      {
        RF_MSG_1( RF_ERROR, "Invalid WCDMA band %d", band.wcdma_band);
        return FALSE;
      }

      vco_path = rfdevice_wcdma_rx_get_vco_path(
                                               (rfdevice_trx_wcdma_rx*)trx_device,
                                               data_array,
                                               array_size
                                              );
 
    break;
#endif /*ifdef FEATURE_WCDMA*/
#ifdef FEATURE_CDMA1X
    case RFCOM_1X_MODE:
    case RFCOM_1XEVDO_MODE:

      if(band.cdma_band> RFM_CDMA_MAX_BAND)
      {
        RF_MSG_1( RF_ERROR, "Invalid CDMA band %d", band.cdma_band);
        return FALSE;
      }

      vco_path = rfdevice_cdma_rx_get_vco_path(
                                               (rfdevice_trx_cdma_rx*)trx_device,
                                               data_array,
                                               array_size
                                              );

      break;
#endif /*ifdef FEATURE_CDMA1X*/
#ifdef FEATURE_GSM
    case RFCOM_GSM_MODE:

      if(band.gsm_band> RFCOM_NUM_GSM_BANDS)
      {
        RF_MSG_1( RF_ERROR, "Invalid GSM band %d", band.gsm_band);
        return FALSE;
      }

      vco_path = rfdevice_gsm_rx_get_vco_path(
                                               (rfdevice_trx_gsm_rx*)trx_device,
                                               data_array,
                                               array_size
                                              );

      break;
#endif /*ifdef FEATURE_GSM*/
#ifdef FEATURE_LTE
    case RFCOM_LTE_MODE:

      if(band.lte_band> RFCOM_NUM_LTE_BANDS)
      {
        RF_MSG_1( RF_ERROR, "Invalid LTE band %d", band.lte_band);
       return FALSE;
      }

      vco_path = rfdevice_lte_rx_get_vco_path(
                                               (rfdevice_trx_lte_rx*)trx_device,
                                               band.lte_band,
                                               data_array,
                                               array_size
                                              );
      break;
#endif /*ifdef FEATURE_LTE*/
#ifdef FEATURE_TDSCDMA
    case RFCOM_TDSCDMA_MODE:

      if(band.tdscdma_band > RFCOM_NUM_TDSCDMA_BANDS)
      {
        RF_MSG_1( RF_ERROR, "Invalid TDSCDMA band %d", band.tdscdma_band);
        return FALSE;
      }

      vco_path = rfdevice_tdscdma_rx_get_vco_path(
                                                  (rfdevice_trx_tdscdma_rx*)trx_device,
                                                  data_array,
                                                  array_size
                                                 );
 
      break;
#endif /*ifdef FEATURE_TDSCDMA*/
    default:
      RF_MSG_1(RF_ERROR, "request for invalid tech %d", mode);
      break;
  }

if (vco_path == WTR_RX_INVALID_VCO_PATH)
{
  RF_MSG_4(RF_ERROR, "Invalid VCO path %d, trx_logical_instance= %d, phy_path_num=%d, rx_modem_chain=%d",
                     vco_path,trx_logical_instance, phy_path_num, rx_modem_chain);
    return FALSE;  
  }

  trx_phy_path_adc_mapping[rx_modem_chain].trx_logical_instance = trx_logical_instance; /*store the transciever logical device instance*/
  trx_phy_path_adc_mapping[rx_modem_chain].phy_path             = phy_path_num;         /*store the physical path*/
  trx_phy_path_adc_mapping[rx_modem_chain].modem_chain          = rx_modem_chain;       /*store rx modem chain(ADC)*/
  trx_phy_path_adc_mapping[rx_modem_chain].vco_path             = vco_path;            /*store VCO path information*/


  return TRUE;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  API to get the transciever to ADC mapping 
 
  @param trx_logical_instance
  logical instance of the transciever
 
  @param phy_path
  phyiscal path
 
  @param modem_chain
  modme chain to indicate the ADC
 
  @retval
  boolean indicating pass or fail

*/
boolean rfc_intf::get_trx_phy_path_adc_mapping(                                            
                                                   uint32 rx_modem_chain,        /*modem chain to map the ADC*/
                                                   uint32 *phy_path_num,         /*physical path*/
                                                   uint32 *trx_logical_instance,            /* Transciever device type instance*/
                                                   rfdevice_rx_vco_path_enum_type *vco_path /*VCO mapped to the modem chain*/
                                                 )
{
  /*parameter validity check*/
  if (
      (rx_modem_chain >= RXLM_CHAIN_MAX) ||
      (phy_path_num == NULL)             ||
      (trx_logical_instance == NULL)     ||
      (vco_path == NULL)                 
      )
  {
    RF_MSG_1(RF_ERROR, "get_trx_phy_path_adc_mapping(): invalid parameters passed. modem_chain=%d",rx_modem_chain);
    return FALSE;  
  }

  *trx_logical_instance = trx_phy_path_adc_mapping[rx_modem_chain].trx_logical_instance; /*return the transciever logical device instance*/
  *phy_path_num         = trx_phy_path_adc_mapping[rx_modem_chain].phy_path;             /*return the physical path*/
  *vco_path              = trx_phy_path_adc_mapping[rx_modem_chain].vco_path;             /*return VCO path information */


  return TRUE;
}

#if (defined (FEATURE_THOR_MODEM))

 /*! 
   API to set the transciever to ADC mapping for FBRx
 
  @param trx_logical_instance
  logical instance of the transciever
 
  @param phy_path
  phyiscal path
 
  @param modem_chain
  modme chain to indicate the ADC
 
  @retval
  boolean indicating pass or fail

*/
boolean rfc_intf::set_fbrx_phy_path_adc_mapping(       
                                                   uint32 modem_chain,          /*modem chain to map the ADC*/
                                                   uint32 phy_path_num,         /*physical path*/
                                                   uint32 trx_logical_instance  /* Transciever device type instance*/
                                                  )
{


  /*parameter validity check*/
  if ( (trx_logical_instance >= RFC_MAX_DEVICE_INSTANCES) ||
       (modem_chain >= RXLM_CHAIN_MAX)                 
      )
  {
    RF_MSG_3(RF_ERROR, "rfc_intf::set_fbrx_phy_path_adc_mapping(): invalid parameters passed. trx_logical_instance= %d, "
                       "phy_path_num=%d, rx_modem_chain=%d", 
                       trx_logical_instance, phy_path_num,modem_chain);
    return FALSE;  
  }

  trx_phy_path_adc_mapping[modem_chain].trx_logical_instance = trx_logical_instance; /*store the transciever logical device instance*/
  trx_phy_path_adc_mapping[modem_chain].phy_path             = phy_path_num;         /*store the physical path*/
  trx_phy_path_adc_mapping[modem_chain].modem_chain          = modem_chain;       /*store rx modem chain(ADC)*/

  return TRUE;
}

#endif

/*----------------------------------------------------------------------------*/
/*!
  @brief
  API to get valid ADC list. 
 
  @param trx_logical_instance
  logical instance of the transciever
 
  @param phy_path
  phyiscal path
 
  @param modem_chain
  modme chain to indicate the ADC
 
  @retval
  boolean indicating pass or fail

*/
boolean rfc_intf::get_valid_adc_list(                                            
                                      uint32* adc_list
                                    )
{
  uint8 idx=0;
  /*parameter validity check*/
  if (adc_list == NULL)
  {
    RF_MSG(RF_ERROR, "get_valid_adc_list(): NULL pointer passed");
    return FALSE;  
  }

  for (idx=0; idx < RXLM_CHAIN_MAX; idx++)
  {
    if(trx_phy_path_adc_mapping[idx].modem_chain < RXLM_CHAIN_MAX)
    {
      adc_list[idx] = trx_phy_path_adc_mapping[idx].modem_chain;
    }
  }

  return TRUE;
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function returns the preferred associated rx or tx device for the given device.

  @details
  If an invalid input is provided, it returns RFM_INVALID_DEVICE.
 
  @param device
  The device for which to obtain preferred associated device
 
  @param rx_tx
  Indicates whether to get the rx or tx preferred assocaited device

  @retval
  Associated device.
*/
rfm_device_enum_type
rfc_intf::rfc_get_preferred_associated_device
(
  rfm_device_enum_type device,
  rfc_rxtx_enum_type rx_tx
)
{
  rfc_common_data *rfc_data = rfc_common_data::get_instance();

  if (NULL == rfc_data)
  {
    RF_MSG( RF_ERROR, "rfc_get_preferred_associated_device - NULL RFC common data object." );
    return RFM_INVALID_DEVICE;
  }

  return rfc_data->rfc_get_preferred_associated_device( device, rx_tx );
} /* rfc_get_preferred_associated_device() */




/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function returns the preferred rx or tx device for the given sys_band.

  @details
  If an invalid input is provided, it returns RFM_INVALID_DEVICE.
 
  @param sys_band
  Sys Band enum for a given tech/band
 
  @param rx_tx
  Indicates whether to get the rx or tx preferred assocaited device

  @retval
  Preferred device 
*/
rfm_device_enum_type
rfc_intf::rfc_get_preferred_device
(
  sys_band_class_e_type sys_band,
  rfc_rxtx_enum_type rx_tx
)
{
  rfc_common_data *rfc_data = rfc_common_data::get_instance();

  if (NULL == rfc_data)
  {
    RF_MSG( RF_ERROR, "rfc_get_preferred_device - NULL RFC common data object." );
    return RFM_INVALID_DEVICE;
  }

  return rfc_data->rfc_get_preferred_device( sys_band, rx_tx );
} /* rfc_get_preferred_device() */


/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function checks whether a sys_band is listed in the band_supported
  fields in RFC

  @details
  If an invalid input is provided, it returns FALSE.
 
  @param sys_band
  Sys Band enum for a given tech/band
 
  @param device
  RFM Device to check against

  @retval
  TRUE if supported. FALSE if otherwise
*/
boolean
rfc_intf::rfc_check_band_supported
(
  sys_band_class_e_type sys_band,
  rfm_device_enum_type device
)
{
  rfc_common_data *rfc_data = rfc_common_data::get_instance();

  if (NULL == rfc_data)
  {
    RF_MSG( RF_ERROR, "rfc_check_band_supported - NULL RFC common data object." );
    return FALSE;
  }

  return rfc_data->rfc_check_band_supported( sys_band, device );
} /* rfc_check_band_supported() */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function remaps the old device represenations in ftm to the new physical device scheme.

  @details
  This API is to be used only in FTM to get the equivalent device mapping from
  the new physical device mappings.
 
  @param source_device_info
  Contains the old device information for which to get equivalent new re-mapping
 
  @param remapped_device_info
  Contains the re-mapped device information

  @retval
  TRUE means provided remapped info should be deemed usable/valid.
  FALSE means an internal API failure. Provided remapped info should be deemed invalid.
*/
boolean
rfc_intf::rfc_get_remapped_device_info
(
   rfc_cal_device_remap_info_type *source_device_info,
   rfc_cal_device_remap_info_type *remapped_device_info
)
{
  boolean api_status = TRUE;
  rfc_common_data *rfc_data = rfc_common_data::get_instance();

  if (NULL == rfc_data)
  {
    RF_MSG( RF_ERROR, "rfc_get_remapped_device_info - NULL RFC common data object." );
    return FALSE;
  }

  /* Get remapping info */
  api_status &= rfc_data->rfc_get_remapped_device_info( source_device_info, remapped_device_info );

  return api_status;
} /* rfc_get_remapped_device_info() */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function retuns the capability(RX or TX) of the rfm device passed

  @details
  This function retuns the capability(RX or TX) of the rfm device passed
 
  @param device
  Contains the rfm device whose capability needs to be queried

  @retval
  RFM_DEVICE_INVALID_SUPPORT(0x00) for Invalid support(No Rx/Tx) on that device
  RFM_DEVICE_RX_SUPPORTED(0x01) for RX device
  RFM_DEVICE_TX_SUPPORTED(0x02) for TX device
*/
rfm_device_capability_type
rfc_intf::rfc_get_device_capability
(
   rfm_device_enum_type device
)
{
  rfc_common_data *rfc_data = rfc_common_data::get_instance();
    
  if (NULL == rfc_data)
  {
    RF_MSG( RF_ERROR, "rfc_get_device_capability - NULL RFC common data object." );
    return RFM_DEVICE_INVALID_SUPPORT;
  }
  
  return rfc_data->rfc_get_device_capability(device);
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  API to generate RFC dynamic properties
 
  @details
  To be called by each tech one-time RFC initialization

  @param rfc_dynamic_params
  Params needed to generate RFC dynamic properties

  @return
  TRUE means data is usable. FALSE otherwise
*/
boolean
rfc_intf::generate_rfcard_dynamic_properties
(
  rfc_dynamic_properties_params_type *rfc_dynamic_params
)
{
  boolean ant_item_exists = FALSE;
  uint16 ant_map_idx = 0;
  uint8 num_band_masks = 0;
  uint8 band_mask_idx = 0;
  uint16 num_bits_in_band_mask = 0;
  uint8 rat_mask_idx = 0;
  boolean bw_item_exists = FALSE;
  uint8 max_bw_idx = 0;

  // Local ptr to work with antenna association info
  rfm_device_antenna_type* antenna_info = &rfcard_dynamic_properties.antenna_device_association;
  // Local ptr to work with rat restriction info
  rfm_device_groups_type* rat_restrict_info = &rfcard_dynamic_properties.single_rat_restriction_groups;
  // Local ptr to work with rat restriction info helper
  rfc_rat_restrict_helper_type* device_instance_helper = NULL;
  // Local ptr to work with max bw info
  rfm_device_max_bw_groups_type* max_bw_info = &rfcard_dynamic_properties.device_max_bw_groups;

  /* Check if rfc_dynamic_params is NULL */
  if ( rfc_dynamic_params == NULL )
  {
    RF_MSG( RF_ERROR, "generate_rfcard_dynamic_properties: NULL param provided. Cannot generate RFC dynamic properties.");
    return rfc_dynamic_properties_valid = FALSE;
  }

  if (rfc_dynamic_params->device >= RFM_MAX_WAN_DEVICES)
  {
    RF_MSG_2( RF_ERROR, 
              "generate_rfcard_dynamic_properties: RFC Error. Device %d >= %d. RFC Dynamic properties not usable.",
              rfc_dynamic_params->device, RFM_MAX_WAN_DEVICES );
    return rfc_dynamic_properties_valid = FALSE;
  }

  if (rfc_dynamic_params->dev_instance >= RFC_MAX_DEVICE_INSTANCES)
  {
    RF_MSG_2( RF_ERROR, 
              "generate_rfcard_dynamic_properties: Device instance %d >= %d. Cannot generate RFC dynamic properties.",
              rfc_dynamic_params->dev_instance, RFC_MAX_DEVICE_INSTANCES );
    return rfc_dynamic_properties_valid = FALSE;
  }
  // Get info for specific device instance
  device_instance_helper = &rfc_rat_restrict_info_helper[ rfc_dynamic_params->dev_instance ];

  /////////////////// GENERAL SECTION ///////////////////
  
  // Find the total number of mask items available.
  num_band_masks = sizeof( antenna_info->antenna_mapping[0].band_group ) / sizeof( antenna_info->antenna_mapping[0].band_group.mask[0] );
  num_bits_in_band_mask = 8 * sizeof( antenna_info->antenna_mapping[0].band_group.mask[0] );
  // Find the mask bin
  for (band_mask_idx = 0; band_mask_idx < num_band_masks; band_mask_idx++)
  {
    if(rfc_dynamic_params->sys_band < ((band_mask_idx+1)*num_bits_in_band_mask))
    {
      break;
    }
  }
  if (band_mask_idx >= num_band_masks)
  {
    RF_MSG_2( RF_ERROR, 
              "generate_rfcard_dynamic_properties: RFC Error. Band mask num bits needed %d > max available num bits %d. RFC Dynamic properties not usable.",
              rfc_dynamic_params->sys_band, (num_band_masks*num_bits_in_band_mask));
    return rfc_dynamic_properties_valid = FALSE;
  }

  /////////////////// ANTENNA MAPPING SECTION ///////////////////

  // Find if an antenna_mapping bin already contains device and antenna items
  for ( ant_map_idx = 0; 
        ( (ant_map_idx < antenna_info->num_antenna_mapping_entries) && (ant_map_idx < MAX_RFM_ANTENNA_MAPPING_ENTRIES) ); 
        ant_map_idx++ 
      )
  {
    if( ( antenna_info->antenna_mapping[ ant_map_idx ].rfm_device == rfc_dynamic_params->device) &&
        ( antenna_info->antenna_mapping[ ant_map_idx ].antenna_num == rfc_dynamic_params->antenna) )
    {
      ant_item_exists = TRUE;
      break;
    }
  }

  if (ant_map_idx >= MAX_RFM_ANTENNA_MAPPING_ENTRIES)
  {
    RF_MSG_2( RF_ERROR, 
              "generate_rfcard_dynamic_properties: RFC Error. Antenna mapping idx %d >= %d. Cannot generate RFC dynamic properties.",
              ant_map_idx, MAX_RFM_ANTENNA_MAPPING_ENTRIES);
    return rfc_dynamic_properties_valid = FALSE;
  }

  // If item was not found above, then new item is added...increase count
  if (!ant_item_exists)
  {
    // Populate antenna items in corresponding bin
    antenna_info->antenna_mapping[ ant_map_idx ].rfm_device = rfc_dynamic_params->device;
    antenna_info->antenna_mapping[ ant_map_idx ].antenna_num = rfc_dynamic_params->antenna;

    antenna_info->num_antenna_mapping_entries++;
  }
  // Always populate band mask for antenna item in corresponding bin
  antenna_info->antenna_mapping[ ant_map_idx ].band_group.mask[ band_mask_idx ] |= ((uint64)1<<(rfc_dynamic_params->sys_band - (band_mask_idx*num_bits_in_band_mask)));

  // irat restirctions on habanero qrd card 
  if( rfc_card_id == RF_HW_WTR3925_QRD_CA_QPA8673 || rfc_card_id == RF_HW_WTR3925_QRD2250_CA_QPA8673 )
  {
    rfc_dynamic_params->restrict_irat_on_non_preferred_dev = TRUE ;
  }

  // Update if there are any restrictions to be applied for iRAT on non-preferred device
  if ( rfc_dynamic_params->restrict_irat_on_non_preferred_dev == TRUE )
  {
    rfcard_dynamic_properties.irat_restriction_band_group.mask[ band_mask_idx ] |= ((uint64)1<<(rfc_dynamic_params->sys_band - (band_mask_idx*num_bits_in_band_mask)));
  }

  /////////////////// DEVICE RAT RESTRICTIONS SECTION ///////////////////

  // if a device does not support multi-rat, it needs to be restricted
  if (rfc_dynamic_params->supports_multi_rat == FALSE)
  {
    // If this device instance does not yet have this specific device recorded...it means it is a new device
    if( ( (device_instance_helper->device_mask >> rfc_dynamic_params->device) & 0x1) == 0 )
    {
      // If this instance does not yet have ANY devices recorded... It means this is the first device in the mask, therefor a new mask is added.
      if (device_instance_helper->device_mask == 0)
      {
        device_instance_helper->rat_mask_idx = rat_restrict_info->num_rfm_device_group_entries;
        rat_restrict_info->num_rfm_device_group_entries++;
      }
      
      rat_mask_idx = device_instance_helper->rat_mask_idx;
      rat_restrict_info->rfm_device_group_bitmask[ rat_mask_idx ] = (device_instance_helper->device_mask |= (1<<rfc_dynamic_params->device));
    }
  }

  /////////////////// DEVICE MAX RX BANDWIDTH SECTION ///////////////////

  // Only if there is an actual bandwidth ...
  if (rfc_dynamic_params->max_bw_khz != 0)
  {
    // Find if an entry already exists for bandwidth
    bw_item_exists = FALSE;
    for ( max_bw_idx = 0; 
          ( (max_bw_idx < max_bw_info->num_max_bw_info_entries) && (max_bw_idx < RFM_MAX_DEVICE_BW_GROUPS) );
          max_bw_idx++ )
    {
      if (max_bw_info->device_max_bw_info[ max_bw_idx ].max_bw_khz == rfc_dynamic_params->max_bw_khz)
      {
        bw_item_exists = TRUE;
        break;
      }
    }

    if (max_bw_idx >= RFM_MAX_DEVICE_BW_GROUPS)
    {
      RF_MSG_2( RF_ERROR, 
                "generate_rfcard_dynamic_properties: RFC Error. Rx Max BW idx %d >= %d array size available. Cannot generate RFC dynamic properties.",
                max_bw_idx, RFM_MAX_DEVICE_BW_GROUPS);
      return rfc_dynamic_properties_valid = FALSE;
    }

    // If item was not found above, then new item is added...increase count
    if (!bw_item_exists)
    {
      // Populate bandwidth items in corresponding bin
      max_bw_info->device_max_bw_info[ max_bw_idx ].max_bw_khz = rfc_dynamic_params->max_bw_khz;
      // Account for new item
      max_bw_info->num_max_bw_info_entries++;
    }

    // Always populate device/band mask in corresponding bin
    max_bw_info->device_max_bw_info[ max_bw_idx ].device_mask |= (1<<rfc_dynamic_params->device);
    max_bw_info->device_max_bw_info[ max_bw_idx ].band_group.mask[ band_mask_idx ] |= ((uint64)1<<(rfc_dynamic_params->sys_band - (band_mask_idx*num_bits_in_band_mask)));
  }

  return TRUE;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  API to obtain RFC dynamic properties

  @param rfc_cfg
  RFC dynamic properties

  @return
  TRUE means data is usable. FALSE otherwise
*/
boolean
rfc_intf::get_rfcard_dynamic_properties
(
  rfm_rfcard_dynamic_properties_type *rfc_cfg
)
{
  /* Check if dev_cfg is NULL */
  if ( rfc_cfg == NULL )
  {
    RF_MSG( RF_ERROR, "get_rfcard_dynamic_properties: NULL param provided. Cannot provide RFC dynamic properties.");
    return FALSE;
  }

  if (rfc_card_init_success == FALSE)
  {
    RF_MSG( RF_ERROR, "get_rfcard_dynamic_properties: RFC CARD INIT FAILED. Cannot provide RFC dynamic properties.");
  }

  if (rfc_dynamic_properties_valid == FALSE)
  {
    RF_MSG( RF_ERROR, "get_rfcard_dynamic_properties: RFC invalid dynamic properties. Cannot provide RFC dynamic properties.");
  }

  // If card init failed in the first place, do not provide dynamic properties
  if ( (rfc_card_init_success == TRUE) && (rfc_dynamic_properties_valid == TRUE) )
  {
    (void) memscpy( rfc_cfg,
                    sizeof( rfm_rfcard_dynamic_properties_type ),
                    &rfcard_dynamic_properties,
                    sizeof( rfm_rfcard_dynamic_properties_type ) );
  }
  else
  {
    (void) memset( rfc_cfg, 0x0, sizeof( rfm_rfcard_dynamic_properties_type ) );
    return FALSE;
  }

  return TRUE;
}
