#ifndef PS_FTPCMDRESP_H
#define PS_FTPCMDRESP_H
/*=========================================================================*/
/*!
@file
FTPCmdResp.h

@brief
Application that provides FTP server service 
 
Copyright (c) 2014-15 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
*/
/*=========================================================================*/
/*===========================================================================
EDIT HISTORY FOR MODULE

Please notice that the changes are listed in reverse chronological order.

$Header: //components/rel/data.mpss/3.5..2.1/protocols/app/src/ps_FTPCmdResp.h#2 $
$DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $ 
 
when        who    what, where, why
--------    ---    ----------------------------------------------------------
02/08/14    fn     Created Module
===========================================================================*/

/*===========================================================================

INCLUDE FILES FOR MODULE 

===========================================================================*/

#include "comdef.h"
#include "ps_FTPServer.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "ps_ftp_server_def.h"
#include "ps_ftp_server_pb.h"
#ifdef __cplusplus
}
#endif  // extern "C"

/*===========================================================================

CLASS DEFINITION

===========================================================================*/
namespace PS_FTP
{
  class FTPSocket;

  /**-------------------------------------------------------------------------- 
    *    @class FTPCmdResp
    *    @brief PS FTP server core
  ---------------------------------------------------------------------------*/
  class FTPCmdResp
  {
  public:

    /** @fn FTPCmdResp ()
    *   @brief constructor
    */
    FTPCmdResp();

    /** @fn  ~FTPCmdResp()
    *   @brief Default destructor.
    */
    ~FTPCmdResp();

    /** @fn  SendCmd
    *   @brief Sends a FTP server response
    */
    boolean ParseCmd(ftp_msg_response_type* cmd_request,
                     FTPSocket *mpSock, char* buffer );

    /** @fn  SendCmd
    *   @brief Sends a FTP server response
    */
    boolean SendCmd( ftp_server_msg_e_type msg_type,
        FTPSocket *mpSock, void* msg_arg_data );

  protected:
  private:

  };// FTPCmdResp
} //PS_FTP

#endif // ps_FTPCMDRESP_H

