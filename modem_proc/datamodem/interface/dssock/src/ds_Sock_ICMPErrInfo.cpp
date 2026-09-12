/*===========================================================================
  FILE: ds_Sock_ICMPErrInfo.cpp

  OVERVIEW: This file provides implementation of the ICMPErrInfo class.

  DEPENDENCIES: None

  Copyright (c) 2009-2010 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dssock/src/ds_Sock_ICMPErrInfo.cpp#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2009-03-02 msr Created module

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "customer.h"
#include "target.h"
#include "ds_Utils_StdErr.h"

#include "ds_Sock_ICMPErrInfo.h"
#include "ds_Utils_DebugMsg.h"
#include "ps_mem.h"

#include "stringl.h"

using namespace ds::Sock;
using namespace ds::Error;


#define DS_SOCK_ICMPERR_ASSERT(expression) \
         ds_sock_icmperr_assert_wrapper(__LINE__, expression)

/*===========================================================================

FUNCTION ds_sock_eventmgr_assert_wrapper()

DESCRIPTION
  Wrapper function for DS_SOCK_EVENTMGR_ASSERT
 
DEPENDENCIES 
  None

RETURN VALUE
  None
 
SIDE EFFECTS

===========================================================================*/
static void ds_sock_icmperr_assert_wrapper
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
}/* ds_sock_eventmgr_assert_wrapper */



/*===========================================================================

                     PUBLIC MEMBER FUNCTIONS

===========================================================================*/
void * ICMPErrInfo::operator new
(
  unsigned int numBytes
)
throw()
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return ps_mem_get_buf( PS_MEM_ICMP_ERR_INFO_TYPE);

} /* ICMPErrInfo::operator new() */


void ICMPErrInfo::operator delete
(
  void *  bufPtr
)
throw()
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == bufPtr)
  {
    DS_SOCK_ICMPERR_ASSERT(0);
    return;
  }

  PS_MEM_FREE( bufPtr);
  return;

} /* ICMPErrInfo::operator delete() */


ICMPErrInfo::ICMPErrInfo
(
  struct ps_sock_extended_err  psExtendedErrInfo,
  ds::SockAddrInternalType*    _addr
) :
    addr(*_addr)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  extendedErrInfo.error_number = psExtendedErrInfo.ee_errno;
  extendedErrInfo.origin       = psExtendedErrInfo.ee_origin;
  extendedErrInfo.type         = psExtendedErrInfo.ee_type;
  extendedErrInfo.code         = psExtendedErrInfo.ee_code;
  extendedErrInfo.info         = psExtendedErrInfo.ee_info;

  /*-------------------------------------------------------------------------
    Using explicit scope to shut up lint. It complains because AddRef() is
    declared as virtual to shut up compiler
  -------------------------------------------------------------------------*/
  (void) ICMPErrInfo::AddRef();
  return;

} /* ICMPErrInfo::ICMPErrInfo() */


ds::ErrorType CDECL ICMPErrInfo::GetAncID
(
  ds::Sock::AncDataIDType *  ancIDPtr
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == ancIDPtr)
  {
    LOG_MSG_INVALID_INPUT_0("ICMPErrInfo::GetAncID(): "
                            "NULL arg");
    return QDS_EFAULT;
  }

  *ancIDPtr = AncData::ICMP_ERROR_INFO;
  return AEE_SUCCESS;

} /* ICMPErrInfo::GetAncID() */


ds::ErrorType CDECL ICMPErrInfo::SetAncID
(
  ds::Sock::AncDataIDType  ancID
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return AEE_SUCCESS;

} /* ICMPErrInfo::SetAncID() */


ds::ErrorType CDECL ICMPErrInfo::GetExtendedErr
(
  ExtendedErrType *  extendedErrInfoPtr
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  LOG_MSG_FUNCTION_ENTRY_2("ICMPErrInfo::GetExtendedErr(): "
                           "obj 0x%x, extendedErrInfoPrt 0x%x",
                           this, extendedErrInfoPtr);

  if (0 == extendedErrInfoPtr)
  {
    return QDS_EFAULT;
  }

  *extendedErrInfoPtr = extendedErrInfo;

  return AEE_SUCCESS;

} /* ICMPErrInfo::GetExtendedErr() */


ds::ErrorType CDECL ICMPErrInfo::GetAddr
(
  ds::SockAddrStorageType _addr
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  LOG_MSG_FUNCTION_ENTRY_2("ICMPErrInfo::GetAddr(): "
                           "obj 0x%x, addr 0x%s", this, _addr);

  if (0 == _addr)
  {
    return QDS_EFAULT;
  }

  memscpy( _addr, sizeof(ds::SockAddrStorageType),
           &addr, sizeof(ds::SockAddrStorageType));

  return AEE_SUCCESS;

} /* ICMPErrInfo::GetAddr() */


ds::ErrorType CDECL ICMPErrInfo::QueryInterface
(
  AEEIID   iid,
  void **  objPtrPtr
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  LOG_MSG_FUNCTION_ENTRY_3("ICMPErrInfo::QueryInterface(): "
                          "obj 0x%x  iid %d, objPtrPtr 0x%x", this, iid, 
                          objPtrPtr);

  if (0 == objPtrPtr)
  {
    return QDS_EFAULT;
  }

  switch (iid)
  {
    case AEEIID_IAncDataPriv:
    {
      *objPtrPtr = static_cast <IAncDataPriv *> ( this);
      (void) AddRef();
      break;
    }

    case AEEIID_IICMPErrInfoPriv:
    {
      *objPtrPtr = this;
      (void) AddRef();
      break;
    }

    case AEEIID_IQI:
    {
      *objPtrPtr = static_cast <IQI *> ( this);
      (void) AddRef();
      break;
    }

    default:
    {
      *objPtrPtr = 0;
      return AEE_ECLASSNOTSUPPORT;
    }
  } /* switch */

  return AEE_SUCCESS;

} /* ICMPErrInfo::QueryInterface() */

