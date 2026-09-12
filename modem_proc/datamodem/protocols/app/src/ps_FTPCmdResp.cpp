/*=========================================================================*/
/*!
@file
ps_FTPCmdResp.cpp

@brief
Provides helper routines for mock TCP application off-target test execution (QTF)

Copyright (c) 2014-15 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
*/
/*=========================================================================*/
/*===========================================================================
EDIT HISTORY FOR MODULE

Please notice that the changes are listed in reverse chronological order.

$Header: //components/rel/data.mpss/3.5..2.1/protocols/app/src/ps_FTPCmdResp.cpp#2 $
$DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $ 
 
when        who    what, where, why
--------    ---    ----------------------------------------------------------
02/08/14    fn     Created Module
===========================================================================*/

/*===========================================================================

INCLUDE FILES FOR MODULE

===========================================================================*/


#include "comdef.h"
#include "ps_FTPSocket.h"
#include "ps_FTPCmdResp.h"
#include <stringl/stringl.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "ps_system_heap.h"
#include "ds_Utils_DebugMsg.h"
#include "ps_app_logging_util.h"
#ifdef __cplusplus
}
#endif  // extern "C"

using namespace PS_FTP;

/*===========================================================================

METHODS

===========================================================================*/
/**-------------------------------------------------------------------------- 
*   @fn FTPCmdResp()
*   @brief Standard constructor
---------------------------------------------------------------------------*/
FTPCmdResp::FTPCmdResp
(

)
{

}

/**-------------------------------------------------------------------------- 
*   @fn  ~FTPCmdResp()
*   @brief Default destructor.
---------------------------------------------------------------------------*/
FTPCmdResp::~FTPCmdResp()
{

}


/**-------------------------------------------------------------------------- 
*   @fn Start()
*   @brief Starts FTP Server socket and waits for connections
---------------------------------------------------------------------------*/
boolean FTPCmdResp::SendCmd
( 
  ftp_server_msg_e_type msg_type,
  FTPSocket *mpSock,
  void* msg_arg_data
)
{
  sint15 write_len = 0;

  PS_APP_DEBUG_2("Sending msg type %d, msg_arg_data 0x%xp",
                 msg_type, msg_arg_data);

  //FN debug add way to check for overflow
  if (FALSE == ps_ftp_build_response ((char*)mpSock->WriteBuffer(),
                                      mpSock->WriteBufferSize(),
                                      &write_len,
                                      msg_type,
                                      msg_arg_data ))
  {
    return FALSE;
  }

  if (FALSE == mpSock->WriteSocket(write_len))
  {
    return FALSE;
  }

  return TRUE;
}

/**-------------------------------------------------------------------------- 
*   @fn Start()
*   @brief Starts FTP Server socket and waits for connections
---------------------------------------------------------------------------*/
boolean FTPCmdResp::ParseCmd
( 
  ftp_msg_response_type* cmd_request,
  FTPSocket *mpSock,
  char* buffer 
)
{  
  sint15 ftp_cmd_request_len = 0;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if( NULL == cmd_request || NULL == mpSock || NULL == buffer)
  {
    PS_APP_ERR_DEBUG_3("Invalid input params 0x%p 0x%p 0x%p",cmd_request, mpSock, buffer);
    return FALSE;
  }

  if( FALSE == mpSock->ReadSocket(&ftp_cmd_request_len) || 
      ftp_cmd_request_len == 0 ||
      NULL == mpSock->ReadBuffer() )
  {
    PS_APP_ERR_DEBUG_2("Unable to allocate memory for buffer len %d 0x%p", 
                       ftp_cmd_request_len, buffer);
    return FALSE;
  }

  memscpy(buffer, ftp_cmd_request_len, 
          mpSock->ReadBuffer(), ftp_cmd_request_len);

  if( FALSE ==  ps_ftp_parse_response(buffer,
                                      ftp_cmd_request_len,
                                      cmd_request) )
  {
    PS_APP_ERR_DEBUG_2("Unable toparse ftp cmd from user", 
                       ftp_cmd_request_len, buffer);
    return FALSE;
  }

  return TRUE;
}
