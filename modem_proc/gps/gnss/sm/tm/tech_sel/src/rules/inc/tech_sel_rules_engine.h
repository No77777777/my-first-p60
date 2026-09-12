/*======================================================================

 Technology Selector Rules Engine Header File

 GENERAL DESCRIPTION
  This file contains declaration of Rules Engine Class
  
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

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/tech_sel/src/rules/inc/tech_sel_rules_engine.h#2 $

 when      who   what, where, why
 ----      ----  --- --------------------------------------------------- 
12/15/21   sai   Added new NV for ODCPI and change in ODCPI back off logic
03/02/16   ss    Initial creation of file.
======================================================================*/

#ifndef TECH_SEL_RULES_ENGINE_H
#define TECH_SEL_RULES_ENGINE_H


#include "tech_sel_common.h"
#include "tech_sel_rules.h"
#include "tech_sel_event_mgr.h"
#include "tech_sel_mgr.h"

/*Rules Engine Class*/
/*This class is responsible for listening to events and executing Rules. 
    Rules engine is event based and upon an event, goes through the Rules 
    and executes the Rules whose conditions meet*/
class TechSelRulesEngine{
  public:
    /*Public Member Functions*/    
    /*Function to get singleton instance of Rules Engine class*/
    static TechSelRulesEngine* getInstance();

    /*Utility Functions to be used by Rules*/
    /*This is so that Rules talk to Rules Engine only for thier working and not to Technology managers*/
    boolean getBestAvailPos(void);
    boolean abortSession(void);
    boolean startCpi(techSel_CpiRequest *pz_CpiRequest,q_TselRequestId *pq_ReqId);    
    boolean stopCpi(q_TselRequestId q_ReqId);
    boolean startLppm(tm_sess_power_mode_s_type z_PowerMode);
    boolean stopLppm(void);
    boolean modifyLppm(tm_sess_power_mode_s_type z_PowerMode);
    tm_sess_power_mode_e_type getCurrentPowerMode(void);
    uint32 getCurLppmTbm(void);
    boolean isEngineInLppm(void);    
    boolean queryLppmStatus(tm_sess_power_mode_s_type z_PowerMode);
    boolean allowSdpSensor(boolean v_SdpSensorAllowed);
    gnss_MgpPedStatusEnumType getPedStatus(void);
    gnss_QualIndEnumType getGnssQual(void);
    gnss_PedDevCxtEnumType getCurPedDevCtx(void);
    boolean getPedSensorAssistAvail(void);
    boolean isGnssSessionRunning(void);
    sm_RcvrStateChangeStructType* getGnssEngineState(void);
    static techSel_PosSessInfo* getGnssSessionInfo(void);
    uint32 getRcOnThresholdMsec(void);
    boolean isForegroundLppmEnabled(void);
    boolean isPedAlignAvail(void);
    boolean isWifiAssistInSessionEnabled(void);
    static boolean handleEvent(TechSelEvent *pz_event);
    static boolean registerForEvent(techSelEventType e_EventType);
    uint32 getPremiumSvcConfig(void);
    uint8 getLppmTbmProfile(void);
    tech_sel_odcpi_timers_s_type* getOdcpiTimers(void);
    void setCpiBlockedState(bool v_isCpiBlocked);
    boolean getCpiBlockedState(void);
    
  private:  
      /*Private Member variables*/
      /*Private instance of Rules engine. This makes it singleton*/
      static TechSelRulesEngine* m_pInstance;
      static TechSelMgrClass    *m_pTechMgr;
      /*Instance of Event Manager*/      
      static TechSelEventMgr    *m_pEventMgrInstance;      
      /*Rule Table. Stores instances of Rules*/
      static TechSelRule *mz_RulesTable[TECHSEL_RULE_MAX];

      /*Private Member Functions*/
      /*Private constructor which makes the class singleton*/
      TechSelRulesEngine();

      /*This function executes Rules upon receiving an event*/
      static boolean executeRule(TechSelEvent *pz_event);
      
};

#endif
