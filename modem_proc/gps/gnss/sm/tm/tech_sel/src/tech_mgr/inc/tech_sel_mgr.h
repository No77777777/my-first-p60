/*======================================================================

              Technology Selector Manager Header File

 GENERAL DESCRIPTION
  This file contains declaration of Tech Manager Class
  
 EXTERNALIZED FUNCTIONS


 INITIALIZATION AND SEQUENCING REQUIREMENTS

 Copyright (c) 2017 - 2021 Qualcomm Technologies, Inc. 
  Qualcomm Technologies Confidential and Proprietary. All Rights Reserved.
 Export of this technology or software is regulated by the U.S. Government.
 Diversion contrary to U.S. law prohibited.
======================================================================*/

/*=====================================================================

 EDIT HISTORY FOR MODULE

 This section contains comments describing changes made to the module.
 Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/tech_sel/src/tech_mgr/inc/tech_sel_mgr.h#2 $

 when      who   what, where, why
 ----      ----  --- --------------------------------------------------- 
12/15/21   sai   Added new NV for ODCPI and change in ODCPI back off logic
06/16/17   ss    Initial creation of file.
======================================================================*/

#ifndef TECH_SEL_MGR_H
#define TECH_SEL_MGR_H

#include "tech_sel_gnss_tech_mgr.h"
#include "tech_sel_wifi_tech_mgr.h"

/*Technology Manager Class */
class TechSelMgrClass
{
  public:
    static TechSelMgrClass *getInstance();   

    size_t GetSize() const
    {
      return sizeof(*this);
    }

    boolean HandleWifiAssistNvConfig(uint32 q_WifiAssistConfig)
    {
      if (m_WifiMgr.handleWifiAssistNvConfig(q_WifiAssistConfig))
      {
        return TRUE;
      }
      return FALSE; 
    }

    boolean StartCpi(const techSel_CpiRequest *pz_CpiRequest,q_TselRequestId *pq_ReqId)
    {
      if (m_WifiMgr.startCpi(pz_CpiRequest, pq_ReqId))
      {
        return TRUE;
      }
      return FALSE; 
    }

    boolean StopCpi(q_TselRequestId q_RedId)
    {
      if (m_WifiMgr.stopCpi(q_RedId))
      {
        return TRUE;
      }
      return FALSE; 
    }

    boolean PerformOnDemandWifiScan( techSel_WifiBandEnumType e_band, 
                                     techSel_WifiScanMethodEnumType e_scanType,
                                     techSel_clientWifiScanCbk* p_wifiScanCbk)
    {
      if (m_WifiMgr.performOnDemandWifiScan(e_band, e_scanType, p_wifiScanCbk))
      {
        return TRUE;
      }
      return FALSE; 
    }

    boolean PerformWifiRangingScan( techSel_WifiBandEnumType e_Band, 
                                    pdsm_RangingScanReqType *pz_ReqInfo,
                                    techSel_clientWifiRangingScanCbk* p_RangingWifiScanCbk)
    {
      if (m_WifiMgr.performWifiRangingScan(e_Band, pz_ReqInfo, p_RangingWifiScanCbk))
      {
        return TRUE;
      }
      return FALSE; 
    }

    boolean RegisterForUnsolicitedWifiScan( techSel_clientWifiScanCbk *p_wifiScanCbk )
    {
      return m_WifiMgr.registerForUnsolicitedWifiScan(p_wifiScanCbk);
    }

    boolean HandleWifiScan(t_wiper_ap_set_struct_type* p_wifiScan)
    {
      return m_WifiMgr.handleWifiScan(p_wifiScan);
    }

    boolean IsWifiAssistInSessionEnabled(void) 
    {
      return m_WifiMgr.isWifiAssistInSessionEnabled();
    }

    boolean HandleWifiRangingScan(pdsm_RangingScanRespType *p_wifiScan)
    {
      if (m_WifiMgr.handleWifiRangingScan(p_wifiScan))
      {
        return TRUE;
      }
      return FALSE; 
    }

    boolean HandleWifiTimerExpiry(techSelTimerIdType timerId)
    {
      return m_WifiMgr.handleTimerExpiry(timerId);
    }

    boolean HandleLowiError(void)
    {
      if (m_WifiMgr.handleLowiError())
      {
        return TRUE;
      }
      return FALSE; 
    }


    /* Public methods related to GNSS Tech Selector */
    boolean HandlePedStatusInd(gnss_MgpPedStatusEnumType e_PedStatus)
    {
      if (m_GnssMgr.handlePedStatusInd(e_PedStatus))
      {
        return TRUE;
      }
      return FALSE; 
    }

    boolean HandleGnssQualInd(gnss_QualIndEnumType  e_GnssQualityInd)
    {
      if (m_GnssMgr.handleGnssQualInd(e_GnssQualityInd))
      {
        return TRUE;
      }
      return FALSE; 
    }

    boolean HandlePedDevCtxInd(gnss_PedDevCxtEnumType  e_PedDevCtx)
    {
      if (m_GnssMgr.handlePedDevCtxInd(e_PedDevCtx))
      {
        return TRUE;
      }
      return FALSE; 
    }

    boolean HandlePedAlignAvailInd(boolean v_PedAlignSensorAvailable)
    {
      if (m_GnssMgr.handlePedAlignAvailInd(v_PedAlignSensorAvailable))
      {
        return TRUE;
      }
      return FALSE; 
    }

    boolean HandlePedSensorAssistAvailInd(boolean v_PedSensorAssistAvailable)
    {
      if (m_GnssMgr.handlePedSensorAssistAvailInd(v_PedSensorAssistAvailable))
      {
        return TRUE;
      }
      return FALSE; 
    }

    boolean HandleLppmStatusInd(const sm_LppmStatusStructType *pz_LppmStatus)
    {
      if (m_GnssMgr.handleLppmStatusInd(pz_LppmStatus))
      {
        return TRUE;
      }
      return FALSE; 
    }

    boolean HandleGnssEngineStateChangeInd(sm_RcvrStateChangeStructType *pz_rcvrState)
    {
      if (m_GnssMgr.handleGnssEngineStateChangeInd(pz_rcvrState))
      {
        return TRUE;
      }
      return FALSE; 
    }

    boolean HandlePosSessionStart(techSel_PosSessInfo *pz_sess_info)
    {
      return m_GnssMgr.handlePosSessionStart(pz_sess_info);
    }

    boolean HandlePosFixStart(void)
    {
      return m_GnssMgr.handlePosFixStart();
    }

    void HandleE911Start(void)
    {
      m_GnssMgr.handleE911Start(); 
    }

    boolean HandlePosSessionStop(techSel_PosSessInfo *pz_sess_info,
                                 tm_sess_stop_reason_e_type  abort_reason)
    {
      if (m_GnssMgr.handlePosSessionStop(pz_sess_info, abort_reason))
      {
        return TRUE;
      }
      return FALSE; 
    }

    boolean HandleLppmNvConfig(uint32 q_LppmConfig)
    {
      if (m_GnssMgr.handleLppmNvConfig(q_LppmConfig))
      {
        return TRUE;
      }
      return FALSE; 
    }

    boolean HandleGnssTimerExpiry(techSelTimerIdType timerId)
    {
      if (m_GnssMgr.handleTimerExpiry(timerId))
      {
        return TRUE;
      }
      return FALSE; 
    }

    /*Interface functions to control GNSS functionality*/
    boolean StartLppm(tm_sess_power_mode_s_type z_PowerMode)
    {
      if (m_GnssMgr.startLppm(z_PowerMode))
      {
        return TRUE;
      }
      return FALSE; 
    }

    boolean StopLppm(void)
    {
      if (m_GnssMgr.stopLppm())
      {
        return TRUE;
      }
      return FALSE; 
    }

    boolean ModifyLppm(tm_sess_power_mode_s_type z_PowerMode)
    {
      return m_GnssMgr.modifyLppm(z_PowerMode);
    }

    boolean QueryLppmStatus(tm_sess_power_mode_s_type z_PowerMode)
    {
      if (m_GnssMgr.queryLppmStatus(z_PowerMode))
      {
        return TRUE;
      }
      return FALSE; 
    }

    techSel_PosSessInfo * GetGnssSessionInfo(void)
    {
      return m_GnssMgr.getGnssSessionInfo();
    }

    boolean AllowSdpSensor(boolean v_SdpSensorAllowed)
    {
      if (m_GnssMgr.allowSdpSensor(v_SdpSensorAllowed))
      {
        return TRUE;
      }
      return FALSE; 
    }

    boolean GetBestAvailPos(void)
    {
      if (m_GnssMgr.getBestAvailPos()) 
      {
        return TRUE;
      }
      return FALSE; 
    }

    boolean AbortSession(void)
    {
      if (m_GnssMgr.abortSession())
      {
        return TRUE;
      }
      return FALSE;
    }

    /*Functions to fetch information regarding GNSS functionality*/
    boolean IsEngineInLppm(void)
    {
      return m_GnssMgr.isEngineInLppm();
    }

    tm_sess_power_mode_e_type getCurrentPowerMode(void)
    {
      return m_GnssMgr.getCurrentPowerMode();
    }

    uint32 GetCurLppmTbm(void)
    {
      return m_GnssMgr.getCurLppmTbm();
    }

    boolean IsForegroundLppmEnabled(void)
    {
      if (m_GnssMgr.isForegroundLppmEnabled()) 
      {
        return TRUE;
      }
      return FALSE; 
    }

    boolean IsPedAlignAvail(void)
    {
      if (m_GnssMgr.isPedAlignAvail()) 
      {
        return TRUE;
      }
      return FALSE; 
    }

    gnss_MgpPedStatusEnumType GetCurPedStatus(void)
    {
      return m_GnssMgr.getCurPedStatus();
    }

    gnss_QualIndEnumType GetCurGnssQual(void)
    {
      return m_GnssMgr.getCurGnssQual();
    }

    gnss_PedDevCxtEnumType GetCurPedDevCtx(void)
    {
      return m_GnssMgr.getCurPedDevCtx();
    }

    boolean GetCurPedSensorAssistAvail(void)
    {
      if (m_GnssMgr.getCurPedSensorAssistAvail()) 
      {
        return TRUE;
      }
      return FALSE; 
    }

    boolean IsGnssSessionRunning(void)
    {
      if (m_GnssMgr.isGnssSessionRunning()) 
      {
        return TRUE;
      }
      return FALSE; 
    }

    sm_RcvrStateChangeStructType * GetGnssEngineState(void)
    {
      return m_GnssMgr.getGnssEngineState();
    }

    /*Function to request GNSS Pos fixes*/
    boolean RequestGnssPosFix(techSel_GnssPosReqInfo *pz_PosReq, techSel_clientGnssPosFixCbk *p_posFixCbk)
    {
      return m_GnssMgr.requestGnssPosFix(pz_PosReq, p_posFixCbk);
    }

    boolean StopGnssPosFix(techSel_clientGnssPosFixCbk *p_posFixCbk)
    {
      if (m_GnssMgr.stopGnssPosFix(p_posFixCbk)) 
      {
        return TRUE;
      }
      return FALSE; 
    }

    boolean RegisterForUnsolicitedGnssPosFix(techSel_clientGnssPosFixCbk *p_posFixCbk)
    {
      if (m_GnssMgr.registerForUnsolicitedGnssPosFix(p_posFixCbk)) 
      {
        return TRUE;
      }
      return FALSE; 
    }

    boolean RegisterForGnssLePosFix(techSel_clientGnssPosFixCbk *p_posFixCbk)
    {
      if (m_GnssMgr.registerForGnssLePosFix(p_posFixCbk)) 
      {
        return TRUE;
      }
      return FALSE; 
    }

    boolean DeregisterForGnssLePosFix(techSel_clientGnssPosFixCbk *p_posFixCbk)
    {
      return m_GnssMgr.deregisterForGnssLePosFix(p_posFixCbk);
    }

    boolean HandleEvent(TechSelEvent *pz_event)
    {
      return m_GnssMgr.handleEvent(pz_event);
    }

    void ProcessSensorReport(const techSel_SensorReportType &cz_SensorMsg )
    {
      m_SensorMgr.ProcessSensorReport(cz_SensorMsg);
    }

    void HandleSensorNvConfig(uint8 u_BaroSampleRate,
                              uint8 u_BaroWeight,
                              bool b_EnableFileStorage)
    {
      m_SensorMgr.SetupBaroFilter(u_BaroSampleRate, u_BaroWeight);
      m_SensorMgr.SetSensorStorage(eSLIM_SENSOR_TYPE_BAROMETER, b_EnableFileStorage);
    }

    bool GetSensorBaroMeas(techSel_SensorBaroInfoType *pz_BaroMeas)
    {
      if (NULL != pz_BaroMeas)
      {
        return m_SensorMgr.SlimGetSensorMeas(*pz_BaroMeas);
      }

      return false;
    }

    bool ReqSensorSamples(uint32 q_SensorClient, slimSensorTypeEnumT e_Service, uint8 u_SampleRateHz,
                          boolean v_Enable)
    {
      return m_SensorMgr.SensorDataRequest(q_SensorClient, e_Service, v_Enable, u_SampleRateHz );
    }

    /*Sensor LE Related Functions*/

    boolean StartSensorLe(const mgp_SensorLEConfigStructType  * const pz_SensorLEConfig,
                          techSel_clientSensorLeStatusCbk *p_statusCbk,
                          techSel_clientSensorLeRelDispCbk *p_relDispCbk,
                          q_TselRequestId *pq_ReqId)
    {
      if (m_GnssMgr.startSensorLe(pz_SensorLEConfig, p_statusCbk, p_relDispCbk, pq_ReqId)) 
      {
        return TRUE;
      }
      return FALSE; 
    }

    boolean StopSensorLe(q_TselRequestId q_ReqId)
    {
      if (m_GnssMgr.stopSensorLe(q_ReqId)) 
      {
        return TRUE;
      }
      return FALSE; 
    }

    boolean GetSensorLeRelDisp(uint64 t_ModemTsFromMsec, 
                               uint64 t_ModemTsToMsec,
                               q_TselRequestId q_ReqId)
    {
      if (m_GnssMgr.getSensorLeRelDisp(t_ModemTsFromMsec, t_ModemTsToMsec, q_ReqId)) 
      {
        return TRUE;
      }
      return FALSE; 
    }

    boolean HandleSensorLeStatus(sm_SensorLEStatusStructType *p_SensorLEStatus)
    {
      if (m_GnssMgr.handleSensorLeStatus(p_SensorLEStatus)) 
      {
        return TRUE;
      }
      return FALSE; 
    }

    boolean HandleSensorLeRelDisp(const sm_SensorLERelDispStructType * const p_RelDisp)
    {
      if (m_GnssMgr.handleSensorLeRelDisp(p_RelDisp)) 
      {
        return TRUE;
      }
      return FALSE; 
    }
    
    uint32 getPremiumSvcConfig(void)
    {
      return m_GnssMgr.getPremiumSvcConfig();
    }
    
    uint8 getLppmTbmProfile(void)
    {
      return m_GnssMgr.getLppmTbmProfile();
    }
    
    uint32 getRcOnThresholdMsec(void)
    {
      return m_GnssMgr.getRcOnThresholdMsec();
    }
    
    void setLppmTbmProfile(uint8 u_LppmTbmProfile)
    {
      m_GnssMgr.setLppmTbmProfile(u_LppmTbmProfile);
    }

    bool handlePremiumSvcConfig(uint32 q_PremiumSvcCfg)
    {
      return m_GnssMgr.handlePremiumSvcConfig(q_PremiumSvcCfg);
    }

    bool handleRcOnThreshold(uint32 q_rcOnThresholdMsec)
    {
      return m_GnssMgr.handleRcOnThreshold(q_rcOnThresholdMsec);
    }

    boolean updateOdcpiTimerConfigFromNv(tech_sel_odcpi_timers_s_type* pz_odcpiTimers)
    {
      if (m_WifiMgr.updateOdcpiTimerConfigFromNv(pz_odcpiTimers))
      {
        return TRUE;
      }
      return FALSE; 
    }

    tech_sel_odcpi_timers_s_type* getOdcpiTimers(void) 
    {
      return (m_WifiMgr.getOdcpiTimers());
    }

    void setCpiBlockedState(bool v_isCpiBlocked)
    {
      return (m_WifiMgr.setCpiBlockedState(v_isCpiBlocked));
    }

    bool getCpiBlockedState(void)
    {
      return (m_WifiMgr.getCpiBlockedState());
    }

  private:
    TechSelWifiTechMgr     m_WifiMgr;
    TechSelGnssTechMgr     m_GnssMgr;
    TechSelSensorMgrClass  m_SensorMgr;

    /*Private Member variables*/
    /*Pointer to Tech Manager Instance. This makes the class singleton*/
    static TechSelMgrClass *m_pzInstance;

    TechSelMgrClass()
    {
      Init();
    }

    void Init()
    {
      m_SensorMgr.Init();
    }
};

#endif /*TECH_SEL_MGR_H*/
