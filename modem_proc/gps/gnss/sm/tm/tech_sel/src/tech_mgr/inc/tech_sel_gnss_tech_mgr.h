/*======================================================================

              Technology Selector Gnss Tech Manager Header File

 GENERAL DESCRIPTION
  This file contains declaration of Gnss Tech Manager Class
  
 EXTERNALIZED FUNCTIONS


 INITIALIZATION AND SEQUENCING REQUIREMENTS

 Copyright (c) 2016 - 2018 Qualcomm Technologies, Inc. 
  Qualcomm Technologies Confidential and Proprietary. All Rights Reserved.
 Export of this technology or software is regulated by the U.S. Government.
 Diversion contrary to U.S. law prohibited.
======================================================================*/

/*=====================================================================

 EDIT HISTORY FOR MODULE

 This section contains comments describing changes made to the module.
 Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/tech_sel/src/tech_mgr/inc/tech_sel_gnss_tech_mgr.h#1 $

 when      who   what, where, why
 ----      ----  --- --------------------------------------------------- 
03/02/16   ss    Initial creation of file.
======================================================================*/

#ifndef TECH_SEL_GNSS_TECH_MGR_H
#define TECH_SEL_GNSS_TECH_MGR_H
#include "tech_sel_tech_mgr.h"
#include "tech_sel_event.h"
#include "tech_sel_sensor_mgr.h"

/*LPPM configuration NV is 32 bits long. Following bits of the NV is used*/

/*1st bit of LPPM configuration is used to enable\disable usage of LPPM*/
#define TECHSEL_LPPM_CONFIG_ENABLE_MASK 1

#define TECHSEL_MAX_GNSS_FIX_CBKS 5

#define TECHSEL_MAX_SENSOR_LE_REQUESTS 5

#define TECHSEL_GNSS_FIX_FAIL_RETRY_MSEC (30 * 1000) /*30 seconds*/

#define TECHSEL_GNSS_LE_FIX_ACQ_POWER_MA (65) /* 65 mA */

typedef struct
{
  techSel_GnssPosReqInfo z_gnssPosReq;
  techSel_clientGnssPosFixCbk *p_posFixCbk;
} techSel_onDemandPosFixReq;

/*CPI fix request table element*/
typedef struct
{
  boolean v_inUse;
  techSel_clientSensorLeStatusCbk *p_statusCbk;
  techSel_clientSensorLeRelDispCbk *p_relDispCbk;
}techSel_sensorLeRequestTableInfo;

/*GNSS Technology Manager Class: Singleton Class*/
/*Encapsulates routines and state machine to handle GNSS functionality including turning on and off
GNSS engine and modifying GNSS engine modes*/
class TechSelGnssTechMgr : public TechSelTechMgr
{
public:
  /*Public Member Functions*/    

  TechSelGnssTechMgr();

  ~TechSelGnssTechMgr(){};

  /*Function to get instance of GNSS Technology Manager Singleton*/
  static TechSelGnssTechMgr* getInstance();

  /*Event and message handler functions*/
  bool handlePedStatusInd(gnss_MgpPedStatusEnumType e_PedStatus);
  bool handleGnssQualInd(gnss_QualIndEnumType  e_GnssQualityInd);
  bool handlePedDevCtxInd(gnss_PedDevCxtEnumType  e_PedDevCtx);
  bool handlePedAlignAvailInd(boolean v_PedAlignSensorAvailable);
  bool handlePedSensorAssistAvailInd(boolean v_PedSensorAssistAvailable);    
  bool handleLppmStatusInd(const sm_LppmStatusStructType *pz_LppmStatus);
  bool handleGnssEngineStateChangeInd(sm_RcvrStateChangeStructType *pz_rcvrState);
  bool handlePosSessionStart(techSel_PosSessInfo *pz_sess_info);
  bool handlePosFixStart();
  void handleE911Start();
  bool handlePosSessionStop(techSel_PosSessInfo *pz_sess_info,
                               tm_sess_stop_reason_e_type    abort_reason);
  bool handleLppmNvConfig(uint32 q_LppmConfig);
  bool handlePremiumSvcConfig(uint32 q_PremiumSvcCfg);
  bool handleTimerExpiry(techSelTimerIdType timerId);
  bool handleRcOnThreshold(uint32 q_rcOnThresholdMsec);
  /*Interface functions to control GNSS functionality*/
  bool startLppm(tm_sess_power_mode_s_type z_PowerMode);
  bool stopLppm(void);
  bool modifyLppm(tm_sess_power_mode_s_type z_PowerMode);
  bool queryLppmStatus(tm_sess_power_mode_s_type z_PowerMode);
  techSel_PosSessInfo* getGnssSessionInfo(void);    
  bool allowSdpSensor(bool v_SdpSensorAllowed);
  bool getBestAvailPos(void);
  bool abortSession(void);
  
  /*Functions to fetch information regarding GNSS functionality*/
  bool isEngineInLppm(void);
  tm_sess_power_mode_e_type getCurrentPowerMode(void);
  uint32 getCurLppmTbm(void);
  bool isForegroundLppmEnabled(void);
  bool isPedAlignAvail(void);    
  gnss_MgpPedStatusEnumType getCurPedStatus(void);
  gnss_QualIndEnumType getCurGnssQual(void);
  gnss_PedDevCxtEnumType getCurPedDevCtx(void);
  bool getCurPedSensorAssistAvail(void);
  static bool isGnssSessionRunning(void);
  uint32 getRcOnThresholdMsec(void);
  sm_RcvrStateChangeStructType* getGnssEngineState(void);

  /*Function to request GNSS Pos fixes*/
  bool requestGnssPosFix(techSel_GnssPosReqInfo *pz_PosReq, techSel_clientGnssPosFixCbk *p_posFixCbk);
  bool stopGnssPosFix(techSel_clientGnssPosFixCbk *p_posFixCbk);    
  bool registerForUnsolicitedGnssPosFix(techSel_clientGnssPosFixCbk *p_posFixCbk);
  bool registerForGnssLePosFix(techSel_clientGnssPosFixCbk *p_posFixCbk);
  bool deregisterForGnssLePosFix(techSel_clientGnssPosFixCbk *p_posFixCbk);
  static boolean handleEvent(TechSelEvent* pz_event);
  uint32 getPremiumSvcConfig(void){return mq_PremiumSvcCfg;};

  void setLppmTbmProfile(uint8 u_LppmTbmProfile)
  {
    mu_LppmTbmProfile = u_LppmTbmProfile ;
  }

  uint8 getLppmTbmProfile()
  {
    return (mu_LppmTbmProfile);
  }

  /*Sensor LE Related Functions*/
  
  bool startSensorLe(const mgp_SensorLEConfigStructType  * const pz_SensorLEConfig,
                        techSel_clientSensorLeStatusCbk *p_statusCbk,
                        techSel_clientSensorLeRelDispCbk *p_relDispCbk,
                        q_TselRequestId *pq_ReqId);
  bool stopSensorLe(q_TselRequestId q_ReqId);
  bool getSensorLeRelDisp(uint64 t_ModemTsFromMsec, 
                             uint64 t_ModemTsToMsec,
                             q_TselRequestId q_ReqId);
  bool handleSensorLeStatus(sm_SensorLEStatusStructType *p_SensorLEStatus);
  bool handleSensorLeRelDisp(const sm_SensorLERelDispStructType * const p_RelDisp);

  size_t GetSize() const
  {
    return sizeof(*this);
  }

private:  
      /*Private Member variables*/
      
      static techSel_onDemandPosFixReq m_onDemandGnssPosFixReqs[TECHSEL_MAX_GNSS_FIX_CBKS];
      static techSel_clientGnssPosFixCbk* m_unsolicitedGnssPosFixCbks[TECHSEL_MAX_GNSS_FIX_CBKS];      
      static techSel_clientGnssPosFixCbk* m_GnssLePosFixCbks[TECHSEL_MAX_GNSS_FIX_CBKS];      
      static  uint8 mb_numOnDemandGnssPosFixReqs;
      static  uint8 mb_numUnsolicitedGnssPosFixReqs;
      static  uint8 mb_numGnssLePosFixReqs;
      static  uint8 mb_numSensorLeReqs;

      /*CPI request table*/
      static  techSel_sensorLeRequestTableInfo sensorLeRequestTable[TECHSEL_MAX_SENSOR_LE_REQUESTS];
      
      /*Member variables caching GNSS related information*/
      gnss_MgpPedStatusEnumType        me_CurPedStatus;
      static gnss_QualIndEnumType      me_CurGnssQuality;
      gnss_PedDevCxtEnumType           me_CurPedDevCtx;
      bool                             mv_CurPedAlignSensorAvailable;
      bool                             mv_CurPedSensorAssistAvailable;
      bool                             mv_IsEngineInLppm;
      tm_sess_power_mode_s_type        mz_CurLppmMode;
      sm_RcvrStateChangeStructType     mz_CurGnssEngState;
      techSel_PosSessInfo              mz_CurPosSessInfo;
      static bool                      mv_IsPosSessInfoValid;
      bool                             mv_IsForegroundLppmEnabled;
      bool                             mv_IsPedAlignAvail;
      uint32                           mq_PremiumSvcCfg;
      uint8                            mu_LppmTbmProfile;
      uint32                           mq_rcOnThresholdMsec;
      bool                             mv_IsSensorLeOn;
      mgp_SensorLEConfigStructType     mz_SensorLeConfig;
      /* GNSS-LE filter config to use */
      static gnss_GnssLeFilterConfigType mz_GnssLeFilterCfg;

      /* Optional parameters for TM core position fix. */
      static pdsm_pd_option_s_type mz_get_pos_options;        /* PD option type */
      static pdsm_pd_qos_type mz_get_pos_qos;                 /* QOS for this pos fix request */

      static uint8 mb_winningPosRequest;
      static bool mv_isOnDemandSessionRunning;
      static bool mb_waitForEngineOffForTryingFix;
      static uint64  mt_fixReqTimeMsec;
      static uint64  mt_powerCalcStartTimeMsec;

      static os_TimerMsgType* mz_GnssPosFixRetryTimer;
      static bool mb_prevFixReqFailed;

      void Init()
      {
      }

      static bool handlePosFix(sm_FixInfoStructType *pz_FixRpt);      
      static bool startGnssPosFix(void);
      static bool calculateFixDurationAndPower(uint32 &l_time_delta_ms, uint32 &q_powerConsumedmA);

      /*Functions to generate and post Events to Event Manager*/
      bool genPedStatusChangeEvent(gnss_MgpPedStatusEnumType e_PrevPedStatus,
                                      gnss_MgpPedStatusEnumType e_CurPedStatus);
      bool genGnssQualChangeEvent(gnss_QualIndEnumType e_PrevGnssQual,
                                     gnss_QualIndEnumType e_CurGnssQual);
      bool genPedDevCtxChangeEvent(gnss_PedDevCxtEnumType e_PrevPedDevCtx,
                                      gnss_PedDevCxtEnumType e_CurPedDevCtx);
      bool genLppmTbmChangeEvent(uint8 u_TbmSecs);
      bool genLppmStatusEvent(const sm_LppmStatusStructType *pz_LppmStatus);      
      bool genGnssEngineStateChangeEvent(sm_RcvrStateChangeStructType *pz_prevRcvrState,
                                            sm_RcvrStateChangeStructType *pz_curRcvrState);
      bool genPosSessionStartEvent(techSel_PosSessInfo *pz_sess_info);
      bool genPosFixStartEvent(void); 
      void genE911StartEvent(void);
      bool genPosSessionStopEvent(techSel_PosSessInfo *pz_sess_info,
                                     tm_sess_stop_reason_e_type    abort_reason);
      bool genPedAssistAvailEvent(bool v_PedAssistAvail);
      
      static bool selectClientForGnssPosFix(uint8 *pb_winningPosRequest);
};

#endif