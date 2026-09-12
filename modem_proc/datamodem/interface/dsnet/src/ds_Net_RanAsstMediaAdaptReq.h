#ifndef DS_NET_RANASSTMEDIAADAPTREQ_H
#define DS_NET_RANASSTMEDIAADAPTREQ_H

/*=========================================================================*/
/*!
  @file
  ds_Net_RanAsstMediaAdaptReq.h

  @brief
  This file defines the class that implements the IRanAsstMediaAdaptReqType interface.

  @details
  The Firewall class (DS::Net::RanAsstMediaAdaptReqType) implements the following
  interfaces:
  1. IQI

  IFirewall instance can be obtained by calling createInstance() method.

  IFirewall object represents the firewall rules set up by the admin.
  The admin can perform firewall related IOCTLs
  like:
  1. GetFirewall(): getting a firewall rule

  Copyright (c) 2018 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary

*/
/*=========================================================================*/
/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dsnet/src/ds_Net_RanAsstMediaAdaptReq.h#1 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2008-05-20 dm  Created module.

===========================================================================*/
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "ps_mem.h"
#include "ds_Utils_CSSupport.h"
#include "ds_Utils_INode.h"
#include "ds_Net_Utils.h"
#include "ds_Utils_ISignalBus.h"
#include "ds_Net_IRanAsstMediaAdaptReq.h"
#include "ds_Errors_Def.h"
#include "ds_Net_IPFilterSpec.h"


/*===========================================================================

                     PUBLIC DATA DECLARATIONS

===========================================================================*/

namespace ds
{
namespace Net
{

/*lint -esym(1510, IRanAsstMediaAdaptReqType) */
/*lint -esym(1510, IQI) */
class RanAsstMediaAdaptReqType: public IRanAsstMediaAdaptReqType
{
private:
  IPFilterSpec  *mFilterSpec;
  uint32         mranAsstHandle;
  ISignalBus *   mpSigBusStateChange;

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
  virtual void Destructor
  (
    void 
  )
  throw();

  /*!
  @brief
  Dummy destructor. Only used to free memory.

  @details
  For objects that support weak references, destruction of the object and
  freeing up of memory can happen at different times. Hence Destructor()
  is the one which performs actual destruction of the object, where as
  ~Object() would perform memory cleanup.
  @params     None.
  @return     None.
  */
  virtual ~RanAsstMediaAdaptReqType
  (
    void
  )
  throw();


public:
  /*!
  @brief
  Constructor.

  @param      None.
  @return     None.
  @see        IQI::Release()
  */
  RanAsstMediaAdaptReqType
  (
    void
  );

  /*-------------------------------------------------------------------------
    Methods from DS::Net::IRanAsstMediaAdaptReqType interface.
  -------------------------------------------------------------------------*/
  /*!
  @brief
  

  @details
  @param[out] pFilterSpec - Filter Spec.

  @return SUCCESS    - if request is received successfully
  @return DSS_EFAULT - Parameters are invalid.
  */
  virtual ds::ErrorType CDECL GetRanAsstMediaAdaptReqFltrSpec
  (
    IIPFilterPriv** ppFilterSpec
  );

  /*!
  @brief
  

  @details
  @param[out] pFilterSpec - Filter Spec.

  @return SUCCESS    - if request is received successfully
  @return DSS_EFAULT - Parameters are invalid.
  */
  virtual ds::ErrorType CDECL SetRanAsstMediaAdaptReqFltrSpec
  (
    IIPFilterPriv*  pFilterSpec
  );

  /*!
   @brief
   
  
   @details
   @param[out] bitrate -  Get bit rate.
  
   @return SUCCESS    - if request is received successfully
   @return DSS_EFAULT - Parameters are invalid.
   */
   virtual ds::ErrorType CDECL GetRanAsstMediaAdaptHandle
   ( 
     uint32* handle
   );

  /*-------------------------------------------------------------------------
    Method from INode interface - NO-OP
  -------------------------------------------------------------------------*/
  virtual boolean Process
  (
    void *pUserData
  );


  /*-------------------------------------------------------------------------
    Methods to overload new/delete operators.
  -------------------------------------------------------------------------*/
  DSNET_OVERLOAD_OPERATORS (PS_MEM_RAN_ASST_MEDIA_ADAPT_REQUEST_TYPE)

  /*-------------------------------------------------------------------------
    Defintions of IQI Methods
  -------------------------------------------------------------------------*/
  DSIQI_IMPL_DEFAULTS(IRanAsstMediaAdaptReqType)

}; /* class RanAsstMediaAdaptReqType */

} /* namespace Net */
} /* namespace DS */

#endif /* DS_NET_QOS_RanAsstMediaAdaptReqType_H */




