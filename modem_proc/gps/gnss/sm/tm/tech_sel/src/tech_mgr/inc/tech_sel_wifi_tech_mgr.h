/*======================================================================

              Technology Selector Wifi Tech Manager Header File

 GENERAL DESCRIPTION
  This file contains declaration of Wifi Tech Manager Class
  
 EXTERNALIZED FUNCTIONS


 INITIALIZATION AND SEQUENCING REQUIREMENTS

 Copyright (c) 2016 - 2021 Qualcomm Technologies, Inc. 
  Qualcomm Technologies Confidential and Proprietary. All Rights Reserved.
 Export of this technology or software is regulated by the U.S. Government.
 Diversion contrary to U.S. law prohibited.
======================================================================*/

/*=====================================================================

 EDIT HISTORY FOR MODULE

 This section contains comments describing changes made to the module.
 Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/tech_sel/src/tech_mgr/inc/tech_sel_wifi_tech_mgr.h#2 $

 when      who   what, where, why
 ----      ----  --- --------------------------------------------------- 
12/15/21   sai   Added new NV for ODCPI and change in ODCPI back off logic
03/02/16   ss    Initial creation of file.
======================================================================*/

#ifndef TECH_SEL_WIFI_TECH_MGR_H
#define TECH_SEL_WIFI_TECH_MGR_H
#include "tech_sel_tech_mgr.h"
#include "tech_sel_wifi_tech_lowi_wrapper.h"

/*Maximum number of simultaneous CPI request handled*/
#define TECHSEL_MAX_CPI_REQUESTS 10
/*Wifi Assistance NV config is 32 bits long. Following bits are used*/
/*Bit 0: Wifi Assistance Enabled or not*/
#define TECHSEL_WIFI_ASSIST_ENABLE_MASK 1

/*Wifi Technology Manager Class: Singleton Class*/
/*Encapsulates routines and state machine to handle Wifi functionality including turning on and off
Wifi fixes*/
class TechSelWifiTechMgr : public TechSelTechMgr
{
public:
  TechSelWifiTechMgr();      

  /*Public Member Functions*/    
  ~TechSelWifiTechMgr(){};

  /*Event and message handler functions*/
  bool handleWifiAssistNvConfig(uint32 q_WifiAssistConfig);
  bool handleTimerExpiry(techSelTimerIdType timerId);

  /*Interface functions to control Wifi functionality*/
  bool startCpi(const techSel_CpiRequest *pz_CpiRequest,q_TselRequestId *pq_ReqId);
  bool stopCpi(q_TselRequestId q_RedId);

  bool performOnDemandWifiScan( techSel_WifiBandEnumType e_band, 
                                   techSel_WifiScanMethodEnumType e_scanType,
                                   techSel_clientWifiScanCbk* p_wifiScanCbk);    

  bool performWifiRangingScan( techSel_WifiBandEnumType e_Band, 
                                  pdsm_RangingScanReqType *pz_ReqInfo,
                                  techSel_clientWifiRangingScanCbk* p_RangingWifiScanCbk);

  bool registerForUnsolicitedWifiScan( techSel_clientWifiScanCbk* p_wifiScanCbk );

  bool handleWifiScan(t_wiper_ap_set_struct_type* p_wifiScan);    
  bool handleLowiError(void);

  /*Intercace function to request Wifi scans*/

  /*Functions to fetch information regarding Wifi functionality*/
  bool isWifiAssistInSessionEnabled(void);

  bool handleWifiRangingScan(pdsm_RangingScanRespType* p_wifiScan);
  
  bool updateOdcpiTimerConfigFromNv(tech_sel_odcpi_timers_s_type* pz_odcpiTimers);

  tech_sel_odcpi_timers_s_type* getOdcpiTimers(void);

  void setCpiBlockedState(bool v_isCpiBlocked);

  bool getCpiBlockedState(void);
  
private:  
    /*Private Member variables*/      
    TechSelLOWIClientListenerImpl m_LowiClient;
    
    /*Member variables caching GNSS related information*/
    bool mv_isEmergencyCpiRunning;
    bool mv_isCpiRunning;
    bool mv_isWifiAssistInSessionEnabled;
    /*Whether CPI is Blocked */
    bool mv_isCpiBlocked;

    tech_sel_odcpi_timers_s_type mz_odcpiTimers;

    /*CPI fix request table element*/
    typedef struct
    {
      bool v_inUse; /*whether CPI request in progress*/
      bool v_isEmergencyMode; /*Is CPI requested in emergency mode*/
    }cpiRequestTableInfo;

    /*CPI request table*/
    cpiRequestTableInfo cpiRequestTable[TECHSEL_MAX_CPI_REQUESTS];
};

#endif /*TECH_SEL_WIFI_TECH_MGR_H*/

