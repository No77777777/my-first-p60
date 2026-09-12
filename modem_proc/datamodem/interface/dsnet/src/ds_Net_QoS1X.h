#ifndef DS_NET_QOS_1X_H
#define DS_NET_QOS_1X_H
/*===========================================================================
  @file QoS1X.h

  This file defines the class that implements the IQoS1X 
  interface.

  TODO: Detailed explaination about the class here.

  Copyright (c) 2008 - 2014 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
===========================================================================*/
/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dsnet/src/ds_Net_QoS1X.h#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2008-03-25 hm  Created module.

===========================================================================*/
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "ds_Utils_CSSupport.h"
#include "ds_Utils_ICritSect.h"
#include "ds_Net_IQoS1x.h"
#include "ds_Net_Platform.h"
#include "ds_Net_Utils.h"
#include "ds_Net_MemManager.h"

/*===========================================================================
                     FORWARD DECLERATION
===========================================================================*/

/*===========================================================================

                     PUBLIC DATA DECLARATIONS

===========================================================================*/
namespace ds
{
namespace Net
{
/*lint -esym(1510, IQoS1x) */
/*lint -esym(1510, IQI) */

class QoS1X: public IQoS1x
{
private:

  int32                     mFlowHandle;
  ICritSect*                mpICritSect;

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
  virtual ~QoS1X
  (
    void 
  ) 
  throw();

public:
  QoS1X
  (
    int32      flowHandle
  );

  /* Inherited functions from IQoS1X */
  virtual ds::ErrorType CDECL GetRMAC3Info (RMAC3InfoType* RMAC3Info);
  virtual ds::ErrorType CDECL GetTXStatus (boolean* TXStatus);
  virtual ds::ErrorType CDECL GetInactivityTimer (int* InactivityTimer);
  virtual ds::ErrorType CDECL SetInactivityTimer (int InactivityTimer);

  /*-------------------------------------------------------------------------
    Methods to overload new/delete operators.
  -------------------------------------------------------------------------*/
  DSNET_OVERLOAD_OPERATORS (PS_MEM_DS_NET_QOS_1X)

  /*-------------------------------------------------------------------------
    Definitions of IQI Methods
  -------------------------------------------------------------------------*/
  DSIQI_IMPL_DEFAULTS(IQoS1x)

};
} /* namespace Net */
} /* namespace ds */

#endif /* DS_NET_QOS_1X_H */
