/*======================================================================

 Technology Selector Event  Header File

 GENERAL DESCRIPTION
  This file contains declaration of Event  Class
  
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

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/tech_sel/src/event/inc/tech_sel_event.h#1 $

 when      who   what, where, why
 ----      ----  --- --------------------------------------------------- 
03/02/16   ss    Initial creation of file.
======================================================================*/

#ifndef TECH_SEL_EVENT_H
#define TECH_SEL_EVENT_H

#include "tech_sel_api.h"
#include "tech_sel_common.h"
#include "tech_sel_tech_mgr.h"

typedef enum
{
  TECHSEL_EVENT_TYPE_NONE,
  /*Timer started by a rule expires*/
  TECHSEL_EVENT_RULES_TIMER_EXPIRY,   
  /*Pedestrian state change indication*/
  TECHSEL_EVENT_TYPE_PED_CHANGE_IND,
  /*LPPM Status report is received from MGP*/
  TECHSEL_EVENT_LPPM_STATUS,
  /*GNSS Engine state change indication*/
  TECHSEL_EVENT_GNSS_ENGINE_STATE_CHANGE_IND,
  /*Position Session is started*/
  TECHSEL_EVENT_POS_SESS_START,
  /*Position Session is stopped or aborted*/
  TECHSEL_EVENT_POS_SESS_STOP,
  /*GNSS Strength is calculated*/
  TECHSEL_EVENT_GNSS_STRENGTH,
  /*Wifi Strength is calculated*/
  TECHSEL_EVENT_WIFI_STRENGTH,
  /*GNSS Quality change indication*/
  TECHSEL_EVENT_GNSS_QUAL_CHANGE,
  /*Ped Device Context Change indication*/
  TECHSEL_EVENT_PED_DEV_CONTEXT_CHANGE,
  /*Pedestrian assistance availibility is reported*/
  TECHSEL_EVENT_PED_ASSIST_AVAIL,
  /*Position Fix Event*/
  TECHSEL_EVENT_POS_FIX,
  TECHSEL_EVENT_LPPM_TBM_CHANGE,
  TECHSEL_EVENT_POS_FIX_START,
  TECHSEL_EVENT_E911_START,
  TECHSEL_EVENT_MAX = 2147483647 /* Force 32bit */
}techSelEventType;

/*Base TechSel Event Class*/
class TechSelEvent{
  public:
    /*Public Member Functions*/    
    TechSelEvent(techSelEventType e_EventType){me_EventType = e_EventType;};
    virtual ~TechSelEvent(){};
    
    techSelEventType getEventType(void){return me_EventType;};
    
  private:  
      /*Private Member variables*/
      techSelEventType me_EventType;
};


/*Rule Timer Expiry*/
class TechSelRulesTimerExpiryEvent : public TechSelEvent {
  
  public:
      TechSelRulesTimerExpiryEvent(techSelTimerIdType e_TimerId):TechSelEvent(TECHSEL_EVENT_RULES_TIMER_EXPIRY)
      {
        me_TimerId = e_TimerId;
      };

      techSelTimerIdType getTimerId(void)
      {
        return me_TimerId;
      };

      size_t GetSize() const
      {
        return sizeof(*this);
      }

  private:
      techSelTimerIdType me_TimerId;    
    
};

/*Pedestrian Statuc Change Event*/
class TechSelPedChangeEvent : public TechSelEvent {
  
  public:
      TechSelPedChangeEvent(gnss_MgpPedStatusEnumType e_PrevPedStatus,
                            gnss_MgpPedStatusEnumType e_CurPedStatus):TechSelEvent(TECHSEL_EVENT_TYPE_PED_CHANGE_IND)
      {
        me_CurPedStatus = e_CurPedStatus;
        me_PrevPedStatus = e_PrevPedStatus;        
      };

      gnss_MgpPedStatusEnumType getCurPedStatus(void)
      {
        return me_CurPedStatus;
      };
      gnss_MgpPedStatusEnumType getPrevPedStatus(void)
      {
        return me_PrevPedStatus;
      };      

      size_t GetSize() const
      {
        return sizeof(*this);
      }

  private:
      gnss_MgpPedStatusEnumType me_PrevPedStatus;    
      gnss_MgpPedStatusEnumType me_CurPedStatus;
    
};

/*LPPM Status Event*/
class TechSelLppmStatusEvent : public TechSelEvent {
  
  public:
      TechSelLppmStatusEvent(const sm_LppmStatusStructType *pz_LppmStatus):TechSelEvent(TECHSEL_EVENT_LPPM_STATUS)
      {
        mpz_LppmStatus = pz_LppmStatus;
      };

      const sm_LppmStatusStructType* getLppmStatus(void)
      {
        return mpz_LppmStatus;
      };
      boolean IsPowerModeEquivalent(tm_sess_power_mode_s_type z_SessionPowerMode)
      {
//        if (mpz_LppmStatus->z_LppmConfig.z_LppmParam.q_TbmMsecs == z_SessionPowerMode.q_TbmMsecs)
        {
          if ((mpz_LppmStatus->z_LppmConfig.e_LppmSubMode == MGP_LPPM_SUBCONFIG_FOREGROUND_FIXED_TIME) &&
            (z_SessionPowerMode.e_PowerMode == TM_SESS_POWER_MODE_NORMAL))
          {
            return TRUE;
          }
          else if ((mpz_LppmStatus->z_LppmConfig.e_LppmSubMode == MGP_LPPM_SUBCONFIG_BACKGROUND_FIXED_POWER) &&
            (z_SessionPowerMode.e_PowerMode == TM_SESS_POWER_MODE_BACKGROUND_DEFINED_POWER))
          {
            return TRUE;
          }
          else if ((mpz_LppmStatus->z_LppmConfig.e_LppmSubMode == MGP_LPPM_SUBCONFIG_BACKGROUND_FIXED_TIME) &&
            (z_SessionPowerMode.e_PowerMode == TM_SESS_POWER_MODE_BACKGROUND_DEFINED_TIME))
          {
            return TRUE;
          }
          else if ((mpz_LppmStatus->z_LppmConfig.e_LppmSubMode == MGP_LPPM_SUBCONFIG_BACKGROUND_KEEP_WARM) &&
            (z_SessionPowerMode.e_PowerMode == TM_SESS_POWER_MODE_BACKGROUND_KEEP_WARM))
          {
            return TRUE;
          }
          else
          {
            TECHSEL_MSG_2(MSG_LEGACY_ERROR, "Mode Mismatch: Sess Mode(TM) %d Event Mode(MGP) %d",
              z_SessionPowerMode.e_PowerMode, mpz_LppmStatus->z_LppmConfig.e_LppmSubMode);
            return FALSE;
          }
        }
        //else
        //{
        //  TECHSEL_MSG_2(MSG_LEGACY_ERROR, "TBM Mismatch: Sess TBM %u Event TBM %u",
        //    z_SessionPowerMode.q_TbmMsecs, mpz_LppmStatus->z_LppmConfig.z_LppmParam.q_TbmMsecs);
        //  return FALSE;
        //}
      }

      size_t GetSize() const
      {
        return sizeof(*this);
      }

  private:
      const sm_LppmStatusStructType *mpz_LppmStatus;
    
};

/*Gnss Engine State Change  Event*/
class TechSelGnssEngineStateChangeEvent : public TechSelEvent {
  
  public:
      TechSelGnssEngineStateChangeEvent(sm_RcvrStateChangeStructType *pz_prevRcvrState,
                                        sm_RcvrStateChangeStructType *pz_curRcvrState):TechSelEvent(TECHSEL_EVENT_GNSS_ENGINE_STATE_CHANGE_IND)
      {
        mpz_PrevGnssEngineState = pz_prevRcvrState;
        mpz_CurGnssEngineState = pz_curRcvrState;
        
      };

      sm_RcvrStateChangeStructType* getPrevGnssEngineState(void)
      {
        return mpz_PrevGnssEngineState;
      };

      sm_RcvrStateChangeStructType* getCurGnssEngineState(void)
      {
        return mpz_CurGnssEngineState;
      };

      size_t GetSize() const
      {
        return sizeof(*this);
      }

  private:
      sm_RcvrStateChangeStructType *mpz_PrevGnssEngineState;      
      sm_RcvrStateChangeStructType *mpz_CurGnssEngineState;
    
};

/*Position Session Start Event*/
class TechSelPosSessionStartEvent : public TechSelEvent {
  
  public:
      TechSelPosSessionStartEvent(techSel_PosSessInfo *pz_sess_info):TechSelEvent(TECHSEL_EVENT_POS_SESS_START)
      {
        mpz_PosSessionIfo = pz_sess_info;
      };

      techSel_PosSessInfo* getPosSessInfo(void)
      {
        return mpz_PosSessionIfo;
      };

      size_t GetSize() const
      {
        return sizeof(*this);
      }

  private:
      techSel_PosSessInfo *mpz_PosSessionIfo;
    
};

/*Position Fix Start Event*/
class TechSelPosFixStartEvent : public TechSelEvent {

public:
  TechSelPosFixStartEvent() :TechSelEvent(TECHSEL_EVENT_POS_FIX_START){}

  size_t GetSize() const
  {
    return sizeof(*this);
  }
};

/*E911 Start Event*/
class TechSelE911StartEvent: public TechSelEvent
{

  public:
  TechSelE911StartEvent() :TechSelEvent(TECHSEL_EVENT_E911_START){}

  size_t GetSize() const
  {
    return sizeof(*this);
  }
};

/*Position Session Stop Event*/
class TechSelPosSessionStopEvent : public TechSelEvent {
  
  public:
      TechSelPosSessionStopEvent(techSel_PosSessInfo *pz_sess_info,
         tm_sess_stop_reason_e_type    abort_reason):TechSelEvent(TECHSEL_EVENT_POS_SESS_STOP)
      {
        mpz_PosSessionIfo = pz_sess_info;
        me_AbortReason = abort_reason;
      };

      techSel_PosSessInfo* getPosSessInfo(void)
      {
        return mpz_PosSessionIfo;
      };

      tm_sess_stop_reason_e_type getAbortReason(void)
      {
        return me_AbortReason;
      };

      size_t GetSize() const
      {
        return sizeof(*this);
      }
      
  private:
      techSel_PosSessInfo *mpz_PosSessionIfo;
      tm_sess_stop_reason_e_type  me_AbortReason;
    
};

/*GNSS Strength Event*/
class TechSelGnssStrengthEvent : public TechSelEvent {
  
  public:
      TechSelGnssStrengthEvent(techSelGnssStrength e_GnssStrength):TechSelEvent(TECHSEL_EVENT_GNSS_STRENGTH)
      {
        me_GnssStrength = e_GnssStrength;
      };

      techSelGnssStrength getGnssStrength(void)
      {
        return me_GnssStrength;
      };      
  private:
      techSelGnssStrength  me_GnssStrength;
    
};

/*Wifi Strength Event*/
class TechSelWifiStrengthEvent : public TechSelEvent {
  
  public:
      TechSelWifiStrengthEvent(techSelWifiStrength e_WifiStrength):TechSelEvent(TECHSEL_EVENT_WIFI_STRENGTH)
      {
        me_WifiStrength = e_WifiStrength;
      };

      techSelWifiStrength getWifiStrength(void)
      {
        return me_WifiStrength;
      };      
  private:
      techSelWifiStrength  me_WifiStrength;
    
};

/*GNSS Qual Change Event*/
class TechSelGnssQualChangeEvent : public TechSelEvent {
  
  public:
      TechSelGnssQualChangeEvent(gnss_QualIndEnumType e_PrevGnssQual,
                                 gnss_QualIndEnumType e_CurGnssQual):TechSelEvent(TECHSEL_EVENT_GNSS_QUAL_CHANGE)
      {
        me_CurGnssQual = e_CurGnssQual;
        me_PrevGnssQual = e_PrevGnssQual;        
      };

      gnss_QualIndEnumType getCurGnssQual(void)
      {
        return me_CurGnssQual;
      };
      gnss_QualIndEnumType getPrevGnssQual(void)
      {
        return me_PrevGnssQual;
      };  
      
      size_t GetSize() const
      {
        return sizeof(*this);
      }

  private:
    gnss_QualIndEnumType me_PrevGnssQual;
    gnss_QualIndEnumType me_CurGnssQual;
    
};


/*Ped Device Context Change Event*/
class TechSelPedDevContextChangeEvent : public TechSelEvent {
  
  public:
      TechSelPedDevContextChangeEvent(gnss_PedDevCxtEnumType e_PrevPedDevCtx,
                                      gnss_PedDevCxtEnumType e_CurPedDevCtx):TechSelEvent(TECHSEL_EVENT_PED_DEV_CONTEXT_CHANGE)
      {
        me_CurPedDevCtx = e_CurPedDevCtx;
        me_PrevPedDevCtx = e_PrevPedDevCtx;
      };

      gnss_PedDevCxtEnumType getCurPedDevCtx(void)
      {
        return me_CurPedDevCtx;
      };
      gnss_PedDevCxtEnumType getPrevPedDevCtx(void)
      {
        return me_PrevPedDevCtx;
      };      

      size_t GetSize() const
      {
        return sizeof(*this);
      }

  private:
    gnss_PedDevCxtEnumType me_PrevPedDevCtx;
    gnss_PedDevCxtEnumType me_CurPedDevCtx;
};

/*Pedestrian Assistance Availibility Event*/
class TechSelPedAssistAvailEvent : public TechSelEvent {
  
  public:
      TechSelPedAssistAvailEvent(bool v_PedAssistAvail):TechSelEvent(TECHSEL_EVENT_PED_ASSIST_AVAIL)
      {
        mv_PedAssistAvail = v_PedAssistAvail;
      };

      bool getPedAssistAvail(void)
      {
        return mv_PedAssistAvail;
      };

      size_t GetSize() const
      {
        return sizeof(*this);
      }

  private:
    bool  mv_PedAssistAvail;
};

/*Position Fix Event*/
class TechSelPositionFixEvent : public TechSelEvent {
  
  public:
      TechSelPositionFixEvent(sm_FixInfoStructType *pz_FixRpt):TechSelEvent(TECHSEL_EVENT_POS_FIX)
      {
        mpz_FixRpt = pz_FixRpt;
      };

      sm_FixInfoStructType* getPosFix(void)
      {
        return mpz_FixRpt;
      };

      size_t GetSize() const
      {
        return sizeof(*this);
      }

  private:
    sm_FixInfoStructType*  mpz_FixRpt;
};

#endif

