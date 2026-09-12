#ifndef DS_NET_IRANASSTMEDIAADAPTREQ_H
#define DS_NET_IRANASSTMEDIAADAPTREQ_H

/*============================================================================
  Copyright (c) 2012 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
  ============================================================================*/

#include "ds_Utils_Interface.h"
#include "ds_Utils_StdDef.h"
#include "ds_Utils_IQI.h"
#include "ds_Errors_Def.h"
#include "ds_Addr_Def.h"
#include "ds_Net_Def.h"
#include "ds_Net_IIPFilterPriv.h"
#include "ds_Net_IEventManager.h"
#include "ds_Utils_ISignal.h"

namespace ds
{
   namespace Net
   {
      const ::AEEIID AEEIID_IRanAsstMediaAdaptReqType = 0x109e2f6;
      
      struct IRanAsstMediaAdaptReqType : public ::IQI
      {
         
         /**
           * This function retrieves filter spec
           * @param filterSpec Output Retrieved  specs
           * @retval ds::SUCCESS Request received successfully.
           * @retval Other DS designated error codes might be returned.
           * @see ds_Errors_Def.idl.
           */
         virtual ::AEEResult AEEINTERFACE_CDECL GetRanAsstMediaAdaptReqFltrSpec(::ds::Net::IIPFilterPriv** filterSpec) = 0;

         /**
           * This function retrieves filter spec
           * @param filterSpec Output Retrieved  specs
           * @retval ds::SUCCESS Request received successfully.
           * @retval Other DS designated error codes might be returned.
           * @see ds_Errors_Def.idl.
           */
         virtual ::AEEResult AEEINTERFACE_CDECL SetRanAsstMediaAdaptReqFltrSpec(::ds::Net::IIPFilterPriv* filterSpec) = 0;


      /**
        * This function retrieves handle Rate
        * @retval ds::SUCCESS Request received successfully.
        * @retval Other DS designated error codes might be returned.
        * @see ds_Errors_Def.idl.
        */
      virtual ::AEEResult AEEINTERFACE_CDECL GetRanAsstMediaAdaptHandle(uint32* handle) = 0;

      };

    };
  };

#endif //DS_NET_IRANASSTMEDIAADAPTREQ_H
