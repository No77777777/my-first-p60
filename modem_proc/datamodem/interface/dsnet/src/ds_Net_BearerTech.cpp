/*=========================================================================*/
/*!
  @file
  BearerTech.cpp

  @brief
  This file provides implementation of the ds::Net::BearerTech class.

  Copyright (c) 2008-2013 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
*/
/*=========================================================================*/

/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dsnet/src/ds_Net_BearerTech.cpp#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2011-09-30 kr  Free floating changes
  2009-08-20 hm  Use crit sect interface instead of direct objects.
  2008-04-04 hm  Created module.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include <stringl/stringl.h>
#include "ds_Utils_StdErr.h"
#include "ds_Utils_DebugMsg.h"
#include "ds_Utils_CreateInstance.h"
#include "ds_Net_BearerTech.h"
#include "ds_Net_BearerTech.h"
#include "ds_Net_Utils.h"
#include "ds_Utils_CCritSect.h"
#include "dss_errors_def.h"

using namespace ds::Net;
using namespace ds::Error;
using namespace dss::Error;

/*===========================================================================

                     PRIVATE FUNCTION DEFINITIONS

===========================================================================*/

#define DSNET_UTILS_BEARER_ERROR_FATAL(expression) \
         dsnet_utils_bearer_err_fatal_wrapper(__LINE__, expression)


/*===========================================================================

FUNCTION dsnet_utils_bearer_err_fatal_wrapper()

DESCRIPTION
  Wrapper function for DSNET_UTILS_BEARER_ERROR_FATAL
 
DEPENDENCIES 
  None

RETURN VALUE
  None
 
SIDE EFFECTS

===========================================================================*/
static void dsnet_utils_bearer_err_fatal_wrapper
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
}/* dsnet_utils_bearer_err_fatal_wrapper */


ds::ErrorType BearerTech::GetParams
(
  ParamsType paramName,
  void *     pOut,
  void *     pIn,
  int        len
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (NULL == pOut)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();
  memscpy (pOut, len, pIn, len);
  mpICritSect->Leave();

  LOG_MSG_INFO1_3 ("BearerTech::GetParams(): "
                   "Obj 0x%p, param %d, val %d",
                   this, paramName, *((int *)pOut));

  return AEE_SUCCESS;

} /* GetParams() */

/*===========================================================================

                     PUBLIC FUNCTION DEFINITIONS

===========================================================================*/
/*---------------------------------------------------------------------------
  CONSTRUCTOR/DESTRUCTOR
---------------------------------------------------------------------------*/
BearerTech::BearerTech
(
  ds::Net::IfaceNameType                   networkType,
  uint32                                   cdmaMask,
  uint32                                   cdmaServiceOptionMask,
  uint32                                   umtsMask,
  BearerTechRateType*                pDataBearerRate
)
: mNetworkType (networkType),
  mCdmaTypeMask (cdmaMask),
  mCdmaServiceOptionMask (cdmaServiceOptionMask),
  mUmtsTypeMask (umtsMask),
  mpICritSect(0),
  mBearerRate(),
  refCnt (1)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (AEE_SUCCESS != DS_Utils_CreateInstance (NULL,
                                              AEECLSID_CCritSect,
                                              (void **) &mpICritSect))
  {
    DSNET_UTILS_BEARER_ERROR_FATAL(0);
  }

  if (NULL != pDataBearerRate)
  {
    memscpy (&mBearerRate, sizeof(BearerTechRateType), 
      pDataBearerRate, sizeof(BearerTechRateType));
  }


} /* BearerTech() */

BearerTech::~BearerTech
(
  void
)
throw()
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*lint -save -e1550, -e1551 */
  DS_UTILS_RELEASEIF(mpICritSect);
  /*lint -restore */

} /* ~BearerTech() */

/*---------------------------------------------------------------------------
  Functions inherited from IBearerInfo
---------------------------------------------------------------------------*/
ds::ErrorType BearerTech::GetNetwork
(
  ds::Net::IfaceNameType* pNetworkType
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return GetParams (DS_NET_BEARER_TECH_PARAMS_NETWORK_TYPE,
                    pNetworkType,
                    &mNetworkType,
                    sizeof (ds::Net::IfaceNameType));

} /* GetNetwork() */


ds::ErrorType BearerTech::GetCDMATypeMask
(
  unsigned int* pCDMATypeMask
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return GetParams (DS_NET_BEARER_TECH_PARAMS_CDMA_TYPE_MASK,
                    pCDMATypeMask,
                    &mCdmaTypeMask,
                    sizeof (unsigned int));

} /* GetCDMATypeMask() */

ds::ErrorType BearerTech::GetCDMAServiceOptionsMask
(
  unsigned int* pCDMAServiceOptionsMask
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return GetParams (DS_NET_BEARER_TECH_PARAMS_CDMA_SERVICE_OPTION_MASK,
                    pCDMAServiceOptionsMask,
                    &mCdmaServiceOptionMask,
                    sizeof (unsigned int));

} /* GetCDMAServiceOptionsMask() */


ds::ErrorType BearerTech::GetUMTSTypeMask
(
  unsigned int* pUMTSTypeMask
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return GetParams (DS_NET_BEARER_TECH_PARAMS_UMTS_TYPE_MASK,
                    pUMTSTypeMask,
                    &mUmtsTypeMask,
                    sizeof (unsigned int));

} /* GetUMTSTypeMask() */

ds::ErrorType BearerTech::GetRate
(
  BearerTechRateType* pDataBearerRate
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
   return GetParams (DS_NET_BEARER_TECH_PARAMS_BAERER_RATE,
                     pDataBearerRate,
                     &mBearerRate,
                     sizeof (BearerTechRateType));

} /* GetRate() */

ds::ErrorType BearerTech::GetBearerIsNull
(
  boolean*  BearerIsNull
)
{
  ds::Net::IfaceNameType  networkType;
  ds::ErrorType           dsErrno;
  unsigned int            typeMask;
  dss::Error::dss_error_enum_type  ds_net_error_code = DSS_ERROR_MIN;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_2 ("BearerTech::GetBearerIsNull(): "
                            "Obj %p BearerIsNull 0x%x",
                            this, BearerIsNull);

  if (0 == BearerIsNull)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();

  dsErrno = GetNetwork(&networkType);
  if (AEE_SUCCESS != dsErrno)
  {
    ds_net_error_code = DSS_ERROR_GET_NETWORK_FAILED;
    goto bail;
  }

  switch (networkType)
  {
    case IfaceName::IFACE_CDMA_SN:
    case IfaceName::IFACE_CDMA_AN:
      dsErrno = GetCDMATypeMask(&typeMask);
      if (AEE_SUCCESS != dsErrno)
      {
        ds_net_error_code = DSS_ERROR_GET_CDMA_TYPE_MASK_FAILED;
        goto bail;
      }

      *BearerIsNull = (0 != (typeMask ==
                             BearerTechCDMA::SUBTECH_NULL));
      break;

    case IfaceName::IFACE_UMTS:
      dsErrno = GetUMTSTypeMask(&typeMask);
      if (AEE_SUCCESS != dsErrno)
      {
        ds_net_error_code = DSS_ERROR_GET_UMTS_TYPE_MASK_FAILED;
        goto bail;
      }

      *BearerIsNull = (0 != (typeMask ==
                             BearerTechUMTS::SUBTECH_NULL));
      break;

    default:
      *BearerIsNull = FALSE;
  }

  mpICritSect->Leave();
  return AEE_SUCCESS;

  bail:
  mpICritSect->Leave();
  LOG_MSG_FUNCTION_EXIT_3("BearerTech::GetBearerIsNull(): "
                          "Obj %p BearerIsNull %d error %d",
                          this, *BearerIsNull, ds_net_error_code);
  return dsErrno;

} /* GetBearerIsNull() */

