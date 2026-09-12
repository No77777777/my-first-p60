/*===========================================================================
  FILE: QoSSecondariesInput.cpp

  OVERVIEW: This file provides implementation of the QoSSecondariesInput class.

  DEPENDENCIES: None

  Copyright (c) 2008 - 2012 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "ds_Utils_DebugMsg.h"
#include "ds_Utils_StdErr.h"
#include "ds_Net_QoSSecondariesInput.h"
#include "ds_Net_QoSSecondary.h"
#include "ds_Utils_CreateInstance.h"

using namespace ds::Error;
using namespace ds::Net;

QoSSecondariesInput::QoSSecondariesInput
(
  void
)
: refCnt (1),
  weakRefCnt (1)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

} /* QoSSecondariesInput::QoSSecondariesInput() */

void QoSSecondariesInput::Destructor
(
  void 
)
throw()
{
    QoSSecondary *   pQoSSecondary = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Release the reference to all the QoSSecondaries objects
  -------------------------------------------------------------------------*/

  for (int i = 0; i < mQoSSecondariesList.Count(); i++)
  {
     pQoSSecondary = static_cast <QoSSecondary *> (mQoSSecondariesList.Get(i));
     if ( NULL != pQoSSecondary)
     {
        Disassociate(pQoSSecondary);
     }
     
  }

} /* QoSSecondariesInput::Destructor() */

QoSSecondariesInput::~QoSSecondariesInput
(
  void
)
throw()
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_1 ("QoSSecondariesInput::~QoSSecondariesInput(): "
                            "Obj 0x%p", this);

  /*-------------------------------------------------------------------------
    NO-OP: used only for freeing memory
  -------------------------------------------------------------------------*/

} /* QoSSecondariesInput::~QoSSecondariesInput() */


/*---------------------------------------------------------------------------
  Inherited functions from IQoSSecondariesInput.
---------------------------------------------------------------------------*/
ds::ErrorType QoSSecondariesInput::Associate 
(
 ::ds::Net::IQoSSecondary* qosSecondary
)
{
   QoSSecondary*      pQoSSecondary;
  ds::ErrorType   result;
   /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == qosSecondary)
   {
    return QDS_EFAULT;
   }

   /*-------------------------------------------------------------------------
   Push the QoSSecondary object to the beginning of the list
   -------------------------------------------------------------------------*/
   pQoSSecondary = static_cast <QoSSecondary *>(qosSecondary);
  result = mQoSSecondariesList.PushFront(pQoSSecondary);

  return result;
} /* QoSSecondariesInput::Associate() */


ds::ErrorType QoSSecondariesInput::Disassociate 
(
 ::ds::Net::IQoSSecondary* qosSecondary
)
{
   QoSSecondary*      pQoSSecondary;
   /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == qosSecondary)
   {
    return QDS_EFAULT;
   }

   /*-------------------------------------------------------------------------
    Remove the QoSSecondary object from the list
   -------------------------------------------------------------------------*/
   pQoSSecondary = static_cast <QoSSecondary *>(qosSecondary);
  mQoSSecondariesList.RemoveItem(pQoSSecondary);

   return AEE_SUCCESS;
} /* QoSSecondariesInput::Disassociate() */

ds::ErrorType QoSSecondariesInput::GetNth 
(
  int index,
  IQoSSecondary** qosSecondary
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_3 ("QoSSecondariesInput::GetNth(): "
                            "Obj 0x%p, qos secondary 0x%p, index %d",
                            this, qosSecondary, index);

  if (0 == qosSecondary)
  {
    return QDS_EFAULT;
  }

  if (0 > index)
  {
    return QDS_EINVAL;
  }

  /*-------------------------------------------------------------------------
  Get the Nth QoSSecondary object from the list
  -------------------------------------------------------------------------*/
  *qosSecondary = static_cast <QoSSecondary *> (mQoSSecondariesList.Get(index));
  if (0 == *qosSecondary)
  {
    LOG_MSG_ERROR_2 ("QoSSecondariesInput::GetNth(): "
                     "NULL item at index %d, obj 0x%p", index, this);
    return AEE_EFAILED;
  }

  (void) (*qosSecondary)->AddRef();

  LOG_MSG_FUNCTION_EXIT_2 ("QoSSecondariesInput::GetNth(): "
                           "Nth QoSSecondary 0x%p, obj 0x%p",
                           *qosSecondary, this);
  return AEE_SUCCESS;
} /* QoSSecondariesInput::GetNth() */


ds::ErrorType QoSSecondariesInput::GetNumElements 
(
  int* numElements
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == numElements)
  {
    return QDS_EFAULT;
  }

  /*-------------------------------------------------------------------------
  Get the number of elements from the list
  -------------------------------------------------------------------------*/
  *numElements = mQoSSecondariesList.Count();

  LOG_MSG_FUNCTION_EXIT_2 ("QoSSecondariesInput::GetNumElements(): "
                           "Num elements %d, obj 0x%p", *numElements, this);
  return AEE_SUCCESS;

} /* GetnumElements() */
