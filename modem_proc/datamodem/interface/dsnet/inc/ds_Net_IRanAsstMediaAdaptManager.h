#ifndef DS_NET_IRANASSTMEDIAADAPTMANAGER_H
#define DS_NET_IRANASSTMEDIAADAPTMANAGER_H
/*============================================================================
               Copyright (c) 2011 - 2012 Qualcomm Technologies Incorporated.
                            All Rights Reserved.
                    Qualcomm Confidential and Proprietary
============================================================================
*/
#include "ds_Utils_Interface.h"
#include "ds_Utils_StdDef.h"
#include "ds_Utils_IQI.h"
#include "ds_Addr_Def.h"
#include "ds_Net_Def.h"
#include "ds_Net_IIPFilterPriv.h"
#include "ds_Errors_Def.h"
#include "ds_Net_IRanAsstMediaAdaptReq.h"
#include "ds_Net_Platform.h"
#include "ds_Utils_ICritSect.h"

using namespace NetPlatform;

namespace ds
{
   namespace Net
   {
      
      const ::AEEIID  AEEIID_IRanAsstMediaAdaptManager = 0x1101F33; 
      
      /** @interface IRanAsstMediaAdaptManager
       * 
       * ds RAN assited media adaptation Request interface.
       * Events that can be registered on this interface via OnStateChange(as part of IEventManager interface):
       * ds::Net::RanAsstEvent::QDS_EV_RAN_ASST_MEDIA_ADAPT_EV. Use GetLastMatchingFlowHandle to fetch matching flow handle.
       */
      struct IRanAsstMediaAdaptManager : public ::IQI
      {
         
         /**
          * This function checks if there is an iface that fulfils RAN assisted request and get the value
          * @param pRAMAReqType  Ran Assist media adapt request type
          * @param pRAMABitRate Bit Rate if matched
          
          * @retval ds::SUCCESS matched successfully
          * @retval Other ds designated error codes might be returned.
          * @see ds_Errors_Def.idl.
          */
     virtual ::AEEResult AEEINTERFACE_CDECL GetRealTimeBitRate(::ds::Net::IRanAsstMediaAdaptReqType* pRAMAReqType, PSRAMAInfoType*            bitRateInfo) = 0;

      /**
       * This function will create media adapt ran asst request based on paramertes provided by application
       * @param pRAMAReqType  Ran Assist media adapt request type
       * @param pRAMABitRate Bit Rate if matched
       
       * @retval ds::SUCCESS matched successfully
       * @retval Other ds designated error codes might be returned.
       * @see ds_Errors_Def.idl.
       */
    virtual ::AEEResult AEEINTERFACE_CDECL UpdateBitRateInfo(::ds::Net::IRanAsstMediaAdaptReqType* pRAMAReqType, PSRAMABitRateInfoType*            bitRateInfo) = 0;

    virtual boolean AEEINTERFACE_CDECL MatchPhyLink(::ds::Net::IRanAsstMediaAdaptReqType* pRAMAReqType, int32                                  physLinkHandle) = 0;


      
      virtual ::AEEResult AEEINTERFACE_CDECL CreateMediaAdaptRanAsstReq(::ds::Net::IIPFilterPriv* pFilter,::ds::Net::IRanAsstMediaAdaptReqType** ppRAMAReqType) = 0;

      virtual void AEEINTERFACE_CDECL GetIfaceId(int32* pIfaceId)= 0;

      virtual void  AEEINTERFACE_CDECL GetCritsect( ICritSect**  pCritSect) = 0;

      };
   };
};
#endif //DS_NET_IRANASSTMEDIAADAPTMANAGER_H
