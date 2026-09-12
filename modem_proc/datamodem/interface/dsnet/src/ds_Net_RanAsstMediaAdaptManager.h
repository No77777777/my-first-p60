#ifndef DS_NET_RANASSTMEDIAADAPTMANAGER_H
#define DS_NET_RANASSTMEDIAADAPTMANAGER_H
/*==========================================================================*/
/*!
  @file
  ds_Net_RanAsstMediaAdaptManager.h

  @brief
  This file defines the class that implements the IRanAsstMediaAdaptManager
  interface.

  @details
  The RanAsstMediaAdaptManager class (DS::Net::RanAsstMediaAdaptManager) implements the following
  interfaces:
  IQI
  IRanAsstMediaAdaptManager



  Copyright (c) 2018 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
*/
/*=========================================================================*/
/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dsnet/src/ds_Net_RanAsstMediaAdaptManager.h#1 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2008-05-20 dm  Created module.

===========================================================================*/
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "ds_Errors_Def.h"
#include "ds_Utils_CSSupport.h"
#include "ds_Utils_Factory.h"
#include "ds_Utils_ICritSect.h"
#include "ds_Net_Utils.h"
#include "ds_Net_MemManager.h"
#include "ds_Net_IRanAsstMediaAdaptManager.h"



/*===========================================================================

                     PUBLIC DATA DECLARATIONS

===========================================================================*/
namespace ds
{
  namespace Net
  {
  /*lint -esym(1510, IRanAsstMediaAdaptManager) */
  /*lint -esym(1510, IQI) */
    class RanAsstMediaAdaptManager : public IRanAsstMediaAdaptManager
    {
    
    private:
       ICritSect* mpCritSect;
       int32      mIfaceHandle;
      /*!
      @brief
      Private destructor.
    
      @details
      The destructor of this object is private. Use Release() method of the
      IQI interface to free the object.
    
      @param      None.
      @return     None.
      @see        IQI::Release()
      */
      virtual ~RanAsstMediaAdaptManager
      (
        void
      )
      throw();
    
    
    public:

      RanAsstMediaAdaptManager( int32 ifaceHandle );
      /*-------------------------------------------------------------------------
        Inherited functions from IRanAsstMediaAdaptManager.
      -------------------------------------------------------------------------*/

      /*!
      @function
      CreateMediaAdaptRanAsstReq()
    
      @brief
      This function  create media adapt ran asst request based on paramertes provided by application
    
    
      @param[in]pRAMAReqType  Ran Assist media adapt request type
    
      @return     SUCCESS - on success
      @return     DSS_EFAULT - Invalid arguments
      */
      virtual ds::ErrorType CDECL CreateMediaAdaptRanAsstReq
      (
        IIPFilterPriv*              pFltrSpec,
        IRanAsstMediaAdaptReqType** ppRAMAReqType
      );


      
      /*!
      @function
      GetRealTimeBitRate()
    
      @brief
      This function  get current bit rate if request match
    
    
      @param[in]pRAMAReqType  Ran Assist media adapt request type
    
      @return     SUCCESS - on success
      @return     DSS_EFAULT - Invalid arguments
      */
      virtual ds::ErrorType CDECL GetRealTimeBitRate
      (
        IRanAsstMediaAdaptReqType* pRAMAReqType,
        PSRAMAInfoType*            bitRateInfo
      );

     virtual ds::ErrorType CDECL UpdateBitRateInfo
     (
     IRanAsstMediaAdaptReqType*        pRAMAReqType,
     PSRAMABitRateInfoType*            bitRateInfo
     );


     virtual boolean CDECL  MatchPhyLink
     (
       IRanAsstMediaAdaptReqType* pRAMAReqType,
       int32                      physLinkHandle
     );



     virtual void CDECL GetIfaceId(int32* pIfaceId);

     virtual void  CDECL GetCritsect( ICritSect**  pCritSect);
      /*-------------------------------------------------------------------------
        IQI interface Methods
      -------------------------------------------------------------------------*/
      DSIQI_IMPL_DEFAULTS(IRanAsstMediaAdaptManager)
    
      /*-------------------------------------------------------------------------
        Overload new/delete operators.
      -------------------------------------------------------------------------*/
      DSNET_OVERLOAD_OPERATORS(PS_MEM_RAN_ASST_MEDIA_ADAPT_MANAGER_TYPE)
    
    };/* class RanAsstMediaAdaptManager */
  } /* namespace Net */
} /* namespace DS */

#endif /* DS_NET_RANASSTMEDIAADAPTMANAGER_H */


