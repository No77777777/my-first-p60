/*======================================================================

              Technology Selector  sensor Manager Header File

 GENERAL DESCRIPTION
  This file contains declaration of Tech Manager Class
  
 EXTERNALIZED FUNCTIONS


 INITIALIZATION AND SEQUENCING REQUIREMENTS

 Copyright (c) 2017-2019 Qualcomm Technologies, Inc. 
  Qualcomm Technologies Confidential and Proprietary. All Rights Reserved.
 Export of this technology or software is regulated by the U.S. Government.
 Diversion contrary to U.S. law prohibited.
======================================================================*/

/*=====================================================================

 EDIT HISTORY FOR MODULE

 This section contains comments describing changes made to the module.
 Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/tech_sel/src/tech_mgr/inc/tech_sel_sensor_mgr.h#1 $

 when      who   what, where, why
 ----      ----  --- --------------------------------------------------- 
02/15/17   ah    Initial creation of file.
======================================================================*/

#ifndef _TECH_SEL_SENSOR_MGR_H
#define _TECH_SEL_SENSOR_MGR_H

#include "comdef.h"
#include "tech_sel_api.h"
#include "slim_api.h"

#define HECTOPASCAL_TO_PASCAL (100.0)


class TechSelSensorClass
{
public:
  TechSelSensorClass():
    m_zState(),
    m_eService(eSLIM_SENSOR_TYPE_NONE)
  {
  }

  TechSelSensorClass(slimSensorTypeEnumT e_Service):
    m_zState(),
    m_eService(e_Service)
  { }

  virtual size_t GetSize() const
  {
    return sizeof(*this);
  }

  virtual void Init(void) {}

  virtual void ShutDown() { }

  virtual ~TechSelSensorClass() {}

  virtual void ClearInfo() { }

  void SetErrorCause(techSel_sensorErrorEnumT e_ErrCause)
  {
    m_zState.z_LatestData.e_errorCause = e_ErrCause;
  }

  void SetSensorData(const slimSensorDataStructT &cz_Data,
                     uint64 t_IpcInjTimeMsec);

  virtual bool SensorDataRequest(slimClientHandleT &rz_ClientHandle,
                                 uint32 q_Client, boolean v_Enable, 
                                 uint8 u_SampleRateHz=1) 
  {
    return false;
  }

  void ReleaseCachedData(bool v_FreeMem=true);
  slimServiceEnumT MapSensor(const slimSensorTypeEnumT &ce_Service);
  uint64 GetLastSampleTime(const slimSensorDataStructT &rz_SensorData) const;

  struct SlimClientStateType
  {
    SlimClientStateType()
    {
      Init();
    }

    void Init()
    {
      v_ReqValid = false;;
      v_StoreDataToRfs = false;
      v_LatestDataValid = false;
      q_ClientMask = 0; /* upto 32 clients */
      memset(&z_CurrentReq, 0x00, sizeof(z_CurrentReq));
      u_TxId = 0;
      pz_SensorData = NULL;
      w_SensorDataCount = 0;
      z_LatestData.e_errorCause = TECH_SEL_SENSOR_MEAS_NOT_AVAILABLE;
      memset(&z_LatestData.z_SensorData, 0x00, sizeof(z_LatestData.z_SensorData));
      q_FileCount = 0;
    }

    bool                               v_ReqValid;
    bool                               v_StoreDataToRfs;
    bool                               v_LatestDataValid;
    uint32                             q_ClientMask; /* upto 32 clients */
    slimEnableSensorDataRequestStructT z_CurrentReq;
    uint8                              u_TxId;
    slimSensorDataStructT              *pz_SensorData;
    uint16                             w_SensorDataCount;
    techSel_SensorInfoType             z_LatestData;
    uint32                             q_FileCount;
    uint32                             q_MaxDataAgeMsec; 
  };

  SlimClientStateType GetState() const
  {
    return m_zState;
  }

  slimSensorTypeEnumT GetSensorType() const
  {
    return m_eService;
  }

  void ClearSensorData()
  {
    m_zState.pz_SensorData = NULL;
  }

  void WriteDataToRfs();
  void StoreSensorData();
  virtual void UpdateData() { } ;
  void SetSensorStorage(slimSensorTypeEnumT e_Service, bool v_Enable);

  static const uint8           cu_MaxSampleRateHz = 100;
  static const uint8           cu_MaxSilmSampleCount  = 50;
  static const uint16          cw_MaxSampleArraySize  = 1000;
  static const size_t          cMaxRfsFileSize        = 100000;
  static const uint16          cw_MaxFileNameSize     = 250;

protected:
  SlimClientStateType m_zState;
  static char         m_cFileName[cw_MaxFileNameSize];
  static const char   cFilePath[cw_MaxFileNameSize];
  static const uint32 c_qMaxDataAgeMsec = 1000; /* 1 seconds */
  slimSensorTypeEnumT m_eService;

  bool SlimSetUpRequest(slimClientHandleT &rz_ClientHandle,
                        uint8 u_SampleRateHz, boolean v_Enable);
};

class TechSelBaroSensorClass : public TechSelSensorClass 
{
public:
  TechSelBaroSensorClass();

  ~TechSelBaroSensorClass() 
  { 
    ShutDown();
  }

  void ClearInfo();

  bool SensorDataRequest(slimClientHandleT &rz_ClientHandle, uint32 q_Client, 
                         boolean v_Enable, uint8 u_SampleRateHz=1);

  size_t GetSize() const
  {
    return sizeof(*this);
  }

  void Init(void);

  void ShutDown()
  {
    ReleaseCachedData();
  }

  void UpdateData();
  
  bool SlimGetBaroMeas(techSel_SensorBaroInfoType &rz_BaroMeas);

  bool SetupFilter(slimClientHandleT &rz_ClientHandle,
                   uint8 u_BaroSampleRate, uint8 u_BaroWeight);

private:

  typedef struct
  {
    uint8  u_WeightFactor;      /* Current sample wieght factor */
    uint8  u_SampleRateHz;      /* Barometer sampling rate  */
    uint32 q_OutlierCountThreshold; /* Number of outlier samples to ignore 
                                       before resetting the filter */
  } SlimSensorBaroFilterCfgType;

  static FLT PressToAlt(const FLT &cf_PressPascal);
  static FLT AltToPress(const FLT &cf_Alt);

  void UpdateFilteredData(const FLT &rf_ThisPressure);

  static const uint8           cu_MaxBaroWeightFactor = 100;
  static const int32           c_RefEncodedPressPascal = 101325;

  static const uint32          cq_MaxAltitudeThreshold = 3; /* Max sudden spike in pressure based altitude in meters 
                                                               that will still be used in filter */
  static const uint32          cq_MaxOutlierThrsholdSec = 5; /* If received samples are outliers for more than
                                                                        this threshold, the filter should be reset  */
  static const uint16          cw_TempAtSeaLevelKelvin = 288;
  static const FLT             cf_Lb; /* lapse value or temperature variation per m, Kelvin/m*/
  static const FLT             cf_R; /* Universal Gas Constant in N.m/Kmol.K */
  static const FLT             cf_g0; /* Gravitational constant ( m/s^2) */
  static const FLT             cf_AirMolarMass; /* air molar mass in g/mole */
  static const FLT             cf_AltExpFactor;

  techSel_SensorBaroInfoType     m_zBaroData;
  SlimSensorBaroFilterCfgType    m_zFilterCfg;
  FLT                            m_fCurrentWeightFactor;

  struct
  {
    unsigned BaroFilterCfgValid : 1;
    unsigned BaroDataValid      : 1;
    unsigned BaroFilterStarted  : 1;
  } m_StatusMask;
};


class TechSelSensorMgrClass 
{
public:
  TechSelSensorMgrClass();

  ~TechSelSensorMgrClass() 
  { 
    ShutDown();
  }

  bool SensorDataRequest(uint32 q_Client, slimSensorTypeEnumT e_Service, 
                         boolean v_Enable, uint8 u_SampleRateHz=1);

  size_t GetSize() const
  {
    return sizeof(*this);
  }

  void Init(void);

  void ShutDown()
  {
    ReleaseCachedData();
  }
  
  void SlimHandleSensorReport(uint64 t_CallbackData,
                              const slimMessageHeaderStructT *pz_MessageHeader,
                              const void *cpz_Message);

  void ProcessSensorReport(const techSel_SensorReportType &cz_SensorMsg );

  bool SlimGetSensorMeas(techSel_SensorBaroInfoType &rz_BaroMeas);

  bool SetupBaroFilter(uint8 u_BaroSampleRate,
                       uint8 u_BaroWeight);
                        
  void SetSensorStorage(slimSensorTypeEnumT e_Service, bool b_Enable);

private:

  TechSelSensorClass * GetSensorHandle(slimSensorTypeEnumT e_Service,
                                        bool v_Create=false);

  void SensorInit(slimSensorTypeEnumT e_Service);
  void ReportData();
  void ReleaseCachedData();

  slimSensorTypeEnumT MapSensorService(const slimServiceEnumT &ce_Service);

  static const uint8           cu_MaxServiceIndex = static_cast<uint8>(eSLIM_SENSOR_TYPE_BAROMETER+1);

  TechSelSensorClass             *m_pzArrSensors[cu_MaxServiceIndex];
  slimClientHandleT              mp_ClientHandle;

  struct
  {
    unsigned MgrInitialized     : 1;
    unsigned SlimOpenRequested  : 1;
  } m_StatusMask;
};

#endif /*_TECH_SEL_SENSOR_MGR_H*/
