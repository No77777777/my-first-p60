#ifndef PS_FTPSERVERCONFIG_H
#define PS_FTPSERVERCONFIG_H
/*=========================================================================*/
/*!
@file
ps_FTPServerConfig.h

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

$Header: //components/rel/data.mpss/3.5..2.1/protocols/app/inc/ps_FTPServerConfig.h#2 $
$DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $
 
when        who    what, where, why
--------    ---    ----------------------------------------------------------
02/08/14    fn     Created Module   
===========================================================================*/

/*===========================================================================

INCLUDE FILES FOR MODULE 

===========================================================================*/

#include "comdef.h"

/*===========================================================================

INCLUDE FILES FOR MODULE (extern C includes)

===========================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

#include "ps_ftp_server_def.h"

#ifdef __cplusplus
}
#endif  // extern "C"

/*===========================================================================

EXPORTED MACROS

===========================================================================*/


/*===========================================================================

EXPORTED FUNCTIONS

===========================================================================*/
namespace PS_FTP
{

  /**-------------------------------------------------------------------------- 
    *    @class FTPServerConfig
    *    @brief PS FTP server configuration utility
  ---------------------------------------------------------------------------*/
  class FTPServerConfig
  {
  public:

    //---------------------------------------------------------------------------
    // Static methods
    //---------------------------------------------------------------------------
  public:

    //---------------------------------------------------------------------------
    // Public methods
    //---------------------------------------------------------------------------
  public:

    /** @fn FTPServer()
    *   @brief Default constructor
    */
    FTPServerConfig();

    /** @fn  ~FTPServerConfig()
    *   @brief Default destructor.
    */
    ~FTPServerConfig();

    /** @fn  FTPServerInitDefaultConfig()
    *   @brief Initialize to default FTP server configurations
    */
    virtual boolean InitDefaultConfig(
       ps_ftp_server_config_type* ftp_server_config);

    /** @fn  FTPServerInitDefaultConfig()
    *   @brief Initialize to default FTP server configurations
    */
    virtual boolean ReadEFSConfig(
       ps_ftp_server_config_type* ftp_server_config, char *config_efs_file);

    //---------------------------------------------------------------------------
    // Protected methods
    //---------------------------------------------------------------------------
  protected:

    //---------------------------------------------------------------------------
    // Protected data fields
    //---------------------------------------------------------------------------
  protected:

    
  private:
  };  //FTPServerConfig
} //PS_FTP

#endif // PS_FTPSERVERCONFIG_H

