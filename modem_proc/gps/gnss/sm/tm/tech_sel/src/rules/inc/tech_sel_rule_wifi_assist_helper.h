/*======================================================================

 Technology Selector Wifi assistance Rule Helper

 GENERAL DESCRIPTION
  This file contains declaration of Wifi assistance Rule Helper Class
  
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

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/tech_sel/src/rules/inc/tech_sel_rule_wifi_assist_helper.h#2 $

 when      who   what, where, why
 ----      ----  --- --------------------------------------------------- 
12/15/21   sai   Added new NV for ODCPI and change in ODCPI back off logic
03/02/16   ss    Initial creation of file.
======================================================================*/

#ifndef TECH_SEL_RULE_WIFI_ASSIST_HELPER_H
#define TECH_SEL_RULE_WIFI_ASSIST_HELPER_H

#include "tech_sel_rules.h"
#include "tech_sel_rules_engine.h"

#define TECHSEL_MIN_SV_FOR_SINGLE_CONST_FOR_GNSS_STRONG 5 /*Minimum 5 SVs required for GNSS Strong*/
#define TECHSEL_MIN_PUNC_METERS_FOR_GNSS_STRONG  10000  /*Min punc of 10km required for GNSS Strong*/

#define TECHSEL_GNSS_STRENGTH_HYST_MSECS (10 * 1000) /* 10 seconds */

#define TECHSEL_MIN_PUNC_METERS_FOR_WIFI_STRONG  500 /*Min punc of 500 meters required for Wifi Strong*/


/*Wifi Assistance Helper Rule*/
/*This class helps Wifi assiatance Rules by monitoring GNSS and Wifi strengths and generating
corresponding strength events. It also exposes interfaces to start and stop cpi for Wifi assistance Rules*/
class TechSelRuleWifiAssistHelper{
  public:      
      /*Public Member Functions*/    
      /*Public interface to get singleton instance of this class*/
      static TechSelRuleWifiAssistHelper* getInstance();

      /*CPI related interfaces*/
      static boolean startCpi(void);
      static boolean stopCpi(void);
      static boolean isCpiRunning(void);

      /*Callback function to handle registered events*/
      static boolean handleEvent(TechSelEvent* pz_event);

  private:      
      /*Private Member variables*/
      /*Private Instance variable which makes this class Singleton*/
      static TechSelRuleWifiAssistHelper* m_pInstance;
      /*CPI related state variables*/
      /*Whether CPI is running*/
      static boolean m_vIsCpiRunning;
      
      /*Request ID for ongoing CPI request*/
      static q_TselRequestId  m_qCpiReqId;

      /*Current GNSS strength*/
      static techSelGnssStrength       me_gnssStrength;
      /*Current Wifi strength*/      
      static techSelWifiStrength       me_WifiStrength;

      /*Gnss Strength Hysteresis Timer is started as soon as Gnss strength changes. At expiry
          of the timer, Gnss strength event is generated.*/
      static os_TimerMsgType*          mz_GnssStrengthHystTimer;
    
      /*Wifi Weak Hysteresis Timer is started as soon as Wifi weak is detected. After the timer expiry,
        Wifi Weak event is generated.*/
      static os_TimerMsgType*          mz_WifiWeakHystTimer;
      
      /*Wifi unknown hysteresis timer is started when wifi fix request fails and Wifi weak state is detected(no wifi fix is injected).
        After expiry of the timer, wifi strength is considered Unknown*/
      static os_TimerMsgType*          mz_WifiUnknownHystTimer;      
      /*Wifi position wait timer is started right after requesting a Wifi position fix. After the timer expiry,
        Wifi weak is detected*/
      static os_TimerMsgType*          mz_WifiPosWaitTimer;
      /*ODCPI Back Off timer would be running for the duration CPI requests are not allowed by tech sel core logic*/
      static os_TimerMsgType*          mz_OdcpiBackOffTimer;
      
      /*Private Member Functions*/        
      /*Private constructor which makes this class Singleton*/
      TechSelRuleWifiAssistHelper();

      /*Function to handle position stop event*/
      static boolean handlePosSessStop(tm_sess_stop_reason_e_type e_AbortReason);
      /*Function to handle timer expiry event event*/
      static boolean handleTimerExpiry(techSelTimerIdType timerId);

      /*Function to handle position fix*/
      static boolean handlePosFix(sm_FixInfoStructType *pz_FixRpt);
      /*Function to handle Gnss position fix*/
      static boolean handleGnssPosFix(sm_FixInfoStructType *pz_FixRpt);
      /*Function to handle Wifi position fix*/
      static boolean handleWifiPosFix(sm_FixInfoStructType *pz_FixRpt);

      /*Function to start stop amnd handle Gnss Strength Hysteresis Timer*/
      static boolean startGnssStrengthHystTimer(void);
      static boolean stopGnssStrengthHystTimer(void);
      static boolean handleGnssStrengthHystTimerExpiry(void);

      /*Function to start stop amnd handle Wifi weak Hysteresis Timer*/
      static boolean startWifiWeakHystTimer(void);
      static boolean stopWifiWeakHystTimer(void);
      static boolean handleWifiWeakHystTimerExpiry(void);

      /*Function to start stop amnd handle Wifi unknown Timer*/
      static boolean startWifiUnknownHystTimer(void);
      static boolean stopWifiUnknownHystTimer(void);
      static boolean handleWifiUnknownHystTimerExpiry(void);      

      /*Function to start stop amnd handle Wifi position fix wait Timer*/
      static boolean startWifiPosWaitTimer(void);
      static boolean stopWifiPosWaitTimer(void);
      static boolean handleWifiPosWaitTimerExpiry(void);

      static boolean blockCpiIfNeeded(void);
      static boolean handleOdcpiBackOffTimerExpiry(void);
    
      /*Utility function to calculate Gnss strength from position fix*/
      static techSelGnssStrength calculateGnssStrengthFromFix(sm_FixInfoStructType *pz_FixRpt);
      /*Utility function to calculate Gnss strength from Gnss State: Punc and Tunc*/
      static techSelGnssStrength calculateGnssStrengthFromPuncTunc(uint32 q_PuncMeters, uint32 q_TuncMsec);

      /*Function to generate GNSS strength event*/
      static boolean genGnssStrengthEvent(techSelGnssStrength e_GnssStrength);
      /*Function to generate Wifi strength event*/
      static boolean genWifiStrengthEvent(techSelWifiStrength e_WifiStrength);

};


#endif /*TECH_SEL_RULE_PDR_H*/

