/*=========================================================================*/
/*!
@file
ps_FTPServerConfig.cpp

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

$Header: //components/rel/data.mpss/3.5..2.1/protocols/app/src/ps_FTPServerConfig.cpp#2 $
$DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $
 
when        who    what, where, why
--------    ---    ----------------------------------------------------------
02/08/14    fn     Created Module 
===========================================================================*/

/*===========================================================================

INCLUDE FILES FOR MODULE

===========================================================================*/

#include <stringl/stringl.h>
#include "comdef.h"

#include "ps_FTPServerConfig.h"

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

DEFINITIONS

===========================================================================*/
#define PS_FTP_MODEM_DEFAULT_CTRL_PORT 0

/*===========================================================================

INTERNAL DATA DEFINITIONS

===========================================================================*/
char ftp_server_name[] = {'M', 'o', 'd', 'e', 'm','_','P','S','_','F','T','P',0};

/*===========================================================================

EXPORTED METHODS

===========================================================================*/


/*===========================================================================

Globals

===========================================================================*/


/*===========================================================================

METHODS

===========================================================================*/
/**-------------------------------------------------------------------------- 
*   @fn FTPServerConfig()
*   @brief Standard constructor
---------------------------------------------------------------------------*/
FTPServerConfig::FTPServerConfig()
{


}

/**-------------------------------------------------------------------------- 
*   @fn  ~FTPServerConfig()
*   @brief Default destructor.
---------------------------------------------------------------------------*/
FTPServerConfig::~FTPServerConfig()
{

}


/**--------------------------------------------------------------------------  
*   @fn  InitDefaultConfig()
*   @brief Initialize to default FTP server configurations
-------------------------------------------------------------------------- */
boolean FTPServerConfig::InitDefaultConfig
(
  ps_ftp_server_config_type* ftp_server_config
)
{
  if( NULL == ftp_server_config )
  {
    PS_APP_ERR_LOG_0();
    return FALSE;
  }

  memset(ftp_server_config,0,sizeof(ps_ftp_server_config_type));

  ftp_server_config->ftp_server_info.server_name = ftp_server_name; 
  ftp_server_config->ftp_server_info.server_control_port = 
    PS_FTP_MODEM_DEFAULT_CTRL_PORT;
  ftp_server_config->ftp_server_info.server_addr_type = IP_ANY_ADDR;
  return TRUE;

}

/**--------------------------------------------------------------------------  
*   @fn  FTPServerInitDefaultConfig()
*   @brief Initialize to default FTP server configurations
-------------------------------------------------------------------------- */
boolean FTPServerConfig::ReadEFSConfig
(
  ps_ftp_server_config_type* ftp_server_config,
  char * config_efs_file
)
{
  /*FN to do add ability to read from EFS file*/
  return FALSE;
}
