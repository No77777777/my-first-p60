/*======================================================================

              Technology Selector  Wifi Tech Lowi Wrapper Header File

 GENERAL DESCRIPTION
  This file contains declaration of Wifi Tech Manager Lowi Wrapper Class

 EXTERNALIZED FUNCTIONS


 INITIALIZATION AND SEQUENCING REQUIREMENTS

 Copyright (c) 2017 Qualcomm Technologies, Inc.
  Qualcomm Technologies Confidential and Proprietary. All Rights Reserved.
 Export of this technology or software is regulated by the U.S. Government.
 Diversion contrary to U.S. law prohibited.
======================================================================*/

/*=====================================================================

 EDIT HISTORY FOR MODULE

 This section contains comments describing changes made to the module.
 Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/tech_sel/src/tech_mgr/inc/tech_sel_wifi_tech_lowi_wrapper.h#1 $

 when      who   what, where, why
 ----      ----  --- ---------------------------------------------------
09/27/16   ss    Initial creation of file.
======================================================================*/
#ifndef TECH_SEL_WIFI_TECH_LOWI_WRAPPER_H
#define TECH_SEL_WIFI_TECH_LOWI_WRAPPER_H

#include "tech_sel_ext_intf.h"
#include "tech_sel_api.h"
#include "lowi_client.h"

#include "lowi_request.h"
#include "lowi_response.h"
#include "vector.h"

using namespace qc_loc_fw;
#define TECHSEL_MAX_WIFI_SCAN_CBKS 5

#define TECHSEL_MAX_RANGING_WIFI_SCAN_CBKS 1

#define TECHSEL_WIFI_SCAN_POWER_MA (120) /* 120 mA */

#define TECHSEL_WIFI_SCAN_COLLECT_TIMEOUT_MS 1000 /* 1 second */

class TechSelLOWIClientListenerImpl : public qc_loc_fw::LOWIClientListener
{
private:

  /*Pointer to Lowi Client Instance*/
  qc_loc_fw::LOWIClient* m_pLowiClientInstance;
  t_wiper_ap_set_struct_type mz_wifiScan;

  techSel_clientWifiScanCbk* m_onDemandWifiScanCbks[TECHSEL_MAX_WIFI_SCAN_CBKS];
  techSel_clientWifiScanCbk* m_unsolicitedWifiScanCbks[TECHSEL_MAX_WIFI_SCAN_CBKS];
  techSel_clientWifiRangingScanCbk* m_rangingWifiScanCbks[TECHSEL_MAX_RANGING_WIFI_SCAN_CBKS];

  uint32 mq_lowi_req_id;
  uint8 mb_numOnDemandWifiScanReqs;
  uint8 mb_numUnsolicitedWifiScanReqs;
  uint8 mb_numRangingWifiScanReqs;

  void processAndSendWifiScan (LOWIResponse::eResponseType eRespType,
                               vector <LOWIScanMeasurement*> & scanMeasurements,
                               vector <LOWIChannelInfo> &scannedFreq);

  void processAndSendRangingWifiScan(LOWIResponse* p_Response);

  void formatMacAddr(uint8* pu_dest_mac_addr, uint32 q_mac_addr_length,
                     uint32 q_mac_addr_low24, uint32 q_mac_addr_high24);

  static qc_loc_fw::LOWIDiscoveryScanRequest::eBand convertWifiBandType(techSel_WifiBandEnumType e_band);
  static qc_loc_fw::LOWIDiscoveryScanRequest::eScanType convertWifiScanMethodType(techSel_WifiScanMethodEnumType e_scanMethod);

  boolean registerWithLowiForUnsolicitedWifiScan(void);

  bool collectWifiScan(t_wiper_ap_set_struct_type *p_WifiScan, bool &is_CollectDone);
  void Init();


  /* Index of the AP in the collected wifi scan */
  uint8 mu_wifiScanApIdx;
  /* Extended wifi scan structure to hold the APs in the wifi scan */
  pdsm_wifi_scan_info_ext_s_type mz_wifiScanExt;

  /* Timer for max interval between batch wifi scan collects */
  os_TimerMsgType *mz_BatchWifiScanMaxIntervaTimer;

public:

  bool performOnDemandWifiScan( techSel_WifiBandEnumType e_band,
                                techSel_WifiScanMethodEnumType e_scanType,
                                techSel_clientWifiScanCbk* p_wifiScanCbk);

  bool registerForUnsolicitedWifiScan( techSel_clientWifiScanCbk* p_wifiScanCbk );
  void responseReceived  (qc_loc_fw::LOWIResponse* p_response);
  void errorReceived (qc_loc_fw::LOWIResponse::eScanStatus e_status);

  void handleWifiScan  (t_wiper_ap_set_struct_type* p_wifiScan);
  void handleError (void);

  bool performWifiRangingScan( techSel_WifiBandEnumType e_Band,
                               pdsm_RangingScanReqType *pz_ReqInfo,
                               techSel_clientWifiRangingScanCbk* p_RangingWifiScanCbk);

  void handleWifiRangingScan  (pdsm_RangingScanRespType* p_wifiScan);

  virtual ~TechSelLOWIClientListenerImpl ();

  TechSelLOWIClientListenerImpl();

  void handleWifiScanCollectTimerExpiry();

  size_t GetSize() const
  {
    return sizeof(*this);
  }

};

#endif /*TECH_SEL_WIFI_TECH_LOWI_WRAPPER_H*/
