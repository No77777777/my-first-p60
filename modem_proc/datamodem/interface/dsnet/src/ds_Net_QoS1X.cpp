/*===========================================================================
  FILE: QoS1X.cpp

  OVERVIEW: This file provides implementation of the QoS1X class.

  DEPENDENCIES: None

  Copyright (c) 2007 - 2014 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dsnet/src/ds_Net_QoS1X.cpp#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2008-03-25 hm  Created module.

===========================================================================*/
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "ds_Utils_DebugMsg.h"
#include "ds_Utils_StdErr.h"
#include "ds_Net_QoS1X.h"
#include "ds_Net_Platform.h"
#include "ds_Utils_CCritSect.h"
#include "ds_Utils_CreateInstance.h"

using namespace ds::Net;
using namespace ds::Error;
using namespace NetPlatform;

#define DS_NET_QOS1X_ASSERT(expression) \
         dsnet_qos1x_assert_wrapper(__LINE__, expression)

/*===========================================================================

FUNCTION dsnet_init_assert_wrapper()

DESCRIPTION
  Wrapper function for DS_NET_INIT_ASSERT
 
DEPENDENCIES 
  None

RETURN VALUE
  None
 
SIDE EFFECTS

===========================================================================*/
static void dsnet_qos1x_assert_wrapper
(
  unsigned int     line_num,
  int              expression
)
{
  if ( !expression )
  {
    ERR_FATAL("DSNET_FATAL at line:%d ",  
                                   line_num,0,0);  
  }
}/* dsnet_init_assert_wrapper */



/*===========================================================================

                     PUBLIC FUNCTION DEFINITIONS

===========================================================================*/
QoS1X::QoS1X
(
  int32 flowHandle
) 
: mFlowHandle(flowHandle),
  refCnt (1)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_2 ("QoS1X::QoS1X(): Obj 0x%p, flow handle 0x%x",
                            this, flowHandle);
  
  if (AEE_SUCCESS != DS_Utils_CreateInstance (NULL, 
                                              AEECLSID_CCritSect,
                                             (void **) &mpICritSect))
  {
    DS_NET_QOS1X_ASSERT(0);
  }
} /* QoS1X::QoS1X() */

QoS1X::~QoS1X
(
  void
)
throw()
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*lint -save -e1550, -e1551 */
  DS_UTILS_RELEASEIF(mpICritSect);
  /*lint -restore */

} /* ~QoS1X() */


/*---------------------------------------------------------------------------
  Inherited functions from IQoS1X.
---------------------------------------------------------------------------*/
ds::ErrorType QoS1X::GetRMAC3Info
(
  RMAC3InfoType* rmac3Info
)
{
  ds::ErrorType  result;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  mpICritSect->Enter();

  result = FlowIoctlNonNullArg (mFlowHandle,
                              FLOW_IOCTL_HDR_GET_RMAC3_INFO,
                              (void *)rmac3Info);

  mpICritSect->Leave();

  return result;

} /* QoS1X::GetRMAC3Info() */

ds::ErrorType QoS1X::GetTXStatus
(
  boolean* txStatus
)
{
  ds::ErrorType  result;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  mpICritSect->Enter();

  result = FlowIoctlNonNullArg (mFlowHandle,
                              FLOW_IOCTL_707_GET_TX_STATUS,
                              (void *)txStatus);

  mpICritSect->Leave();

  return result;

} /* QoS1X::GetTXStatus() */


ds::ErrorType QoS1X::GetInactivityTimer
(
  int *  inactivityTimerVal
)
{
  ds::ErrorType  result;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  mpICritSect->Enter();

  result = FlowIoctlNonNullArg (mFlowHandle,
                              FLOW_IOCTL_707_GET_INACTIVITY_TIMER,
                                (void *) inactivityTimerVal);

  mpICritSect->Leave();

  return result;

} /* QoS1X::GetInactivityTimer() */


ds::ErrorType QoS1X::SetInactivityTimer
(
  int  inactivityTimerVal
)
{
  ds::ErrorType  result;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  mpICritSect->Enter();

  result = FlowIoctlNonNullArg (mFlowHandle,
                              FLOW_IOCTL_707_SET_INACTIVITY_TIMER,
                                (void *) &inactivityTimerVal);

  LOG_MSG_FUNCTION_EXIT_3 ("QoS1X::SetInactivityTimer(): "
                           "Result 0x%x, inactivity timer val %d, obj 0x%p",
                           result, inactivityTimerVal, this); 

  mpICritSect->Leave();

  return result;

} /* QoS1X:;SetInactivityTimer() */
