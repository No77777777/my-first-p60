#ifndef DSS_RANASSTMEDIAADAPTREQ_H
#define DSS_RANASSTMEDIAADAPTREQ_H

/*======================================================

FILE:  DSS_RanAsstMediaAdaptReq.h

SERVICES:


GENERAL DESCRIPTION:

=====================================================

Copyright (c)  2018 Qualcomm Technologies Incorporated. 
All Rights Reserved.
Qualcomm Confidential and Proprietary

=====================================================*/
/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dss/src/DSS_RanAsstMediaAdaptReq.h#1 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------


===========================================================================*/


//===================================================================
//   Includes and Public Data Declarations
//===================================================================

//-------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------
#include "comdef.h"
#include "customer.h"
#include "dssocket.h"

#include "ds_Utils_StdErr.h"
#include "ds_Utils_ISignal.h"
#include "ds_Utils_ISignalCtl.h"
#include "dssocket_defs.h"

#include "ds_Net_IRanAsstMediaAdaptReq.h"
#include "ds_Utils_IWeakRef.h"
#include "ds_Utils_IWeakRefSupport.h"

#include "ds_Utils_ICritSect.h"
#include "DSS_PhysLinkRanAsstMediaAdaptHandler.h"
#include "DSS_NetMonitored.h"
//-------------------------------------------------------------------
// Constant / Define Declarations
//-------------------------------------------------------------------

//-------------------------------------------------------------------
// Type Declarations (typedef, struct, enum, etc.)
//-------------------------------------------------------------------

//-------------------------------------------------------------------
// Global Constant Data Declarations
//-------------------------------------------------------------------

//-------------------------------------------------------------------
// Global Data Declarations
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Forward Declarations
//-------------------------------------------------------------------

//===================================================================
//              Macro Definitions
//===================================================================


//===================================================================
//              Class Definitions
//===================================================================

//===================================================================
//  CLASS:      DSSRanAsstMediaAdaptReq
//
//  DESCRIPTION:
//
//  HOW TO USE:
//
//===================================================================

// Forward declarations to prevent circular inclusion of DSS_RanAsstReq.
class DSSEventHandler;
class DSSNetActive;

using ds::Utils::IWeakRef;

class DSSRanAsstMediaAdaptReq : public IWeakRef
{
//-------------------------------------------------------------------
//  Constructors/Desctructors
//-------------------------------------------------------------------
  public:

    DSSRanAsstMediaAdaptReq(ds::Net::IRanAsstMediaAdaptReqType* pNetRanAsstReq, dss_iface_id_type ifaceId);
    virtual ~DSSRanAsstMediaAdaptReq() throw() {}
    void InsertToList(DSSRanAsstMediaAdaptReq* DSSRanAsstReq);
    virtual void Destructor() throw();    

    /*-------------------------------------------------------------------------
    Defintions of IQI and IWeakRef Methods
    -------------------------------------------------------------------------*/
    DS_UTILS_IWEAKREF_IMPL_DEFAULTS()

//-------------------------------------------------------------------
//  Interface members
//-------------------------------------------------------------------
  public:


//-------------------------------------------------------------------
//  Get Net Ran Asst Media Adapt request type 
//-------------------------------------------------------------------
   inline AEEResult GetIDSNetIRanAsstMediaAdaptReq(
                           ds::Net::IRanAsstMediaAdaptReqType** ppNetRAMAReq)
   {
      if (NULL == mpNetRanAsstReq) {
         return AEE_SUCCESS;
      }
      *ppNetRAMAReq = mpNetRanAsstReq;
      (void)mpNetRanAsstReq->AddRef();
      return AEE_SUCCESS;
   }

    //-------------------------------------------------------------------
    //  Get next DSS Ran Asst Req
    //-------------------------------------------------------------------
    AEEResult GetNext(DSSRanAsstMediaAdaptReq** ppDSSRAMAReq) throw()
    {
       *ppDSSRAMAReq = mNext;
       return AEE_SUCCESS;
    }
    //-------------------------------------------------------------------
    //  Get Net Active Object
    //-------------------------------------------------------------------
    AEEResult GetDSSNetActive(DSSNetActive** ppDSSNetActive);
    //-------------------------------------------------------------------
    //  Set Net Active Object
    //-------------------------------------------------------------------
    void SetDSSNetActive(DSSNetActive* pDSSNetActive);

    //-------------------------------------------------------------------
    //  Get Net Monitor Object
    //-------------------------------------------------------------------
    AEEResult GetDSSNetMonitored(DSSNetMonitored** ppDSSNetMonitored);
    //-------------------------------------------------------------------
    //  Set Net Monitor Object
    //-------------------------------------------------------------------
    void SetDSSNetMonitored(DSSNetMonitored* pDSSNetMonitored);

    
    //-------------------------------------------------------------------
    //  Register Event call back
    //-------------------------------------------------------------------
    AEEResult RegEventCB(dss_iface_ioctl_ev_cb_type* pEvArg,
                             dss_iface_id_type ifaceId);

    void GetIfaceId(dss_iface_id_type* pIfaceId);

//-------------------------------------------------------------------
//  Protected members
//-------------------------------------------------------------------
  protected:
     template<typename HandlerType>
     AEEResult FetchHandler(HandlerType** pHandler, DSSEventHandler** ppEventHandler, bool bInit);

     AEEResult GetEventHandler(dss_iface_ioctl_event_enum_type event, DSSEventHandler** ppEventHandler, bool bInit);

//-------------------------------------------------------------------
//  Data members
//-------------------------------------------------------------------
  protected:
     ds::Net::IRanAsstMediaAdaptReqType*    mpNetRanAsstReq;
     DSSRanAsstMediaAdaptReq*               mNext;
     DSSPhysLinkRanAsstMediaAdaptHandler*   mpRanAsstReqEventHandler;
     DSSNetActive*                          mParentNetActive;
     DSSNetMonitored*                       mParentDSSNetMonitor;
     dss_iface_id_type                      mIfaceId;
     
  public :

     void * operator new (
        unsigned int numBytes
     )  throw();

     void operator delete (
        void *  bufPtr
     );

};

//===================================================================

#endif // DSS_RANASSTMEDIAADAPTREQ_H
