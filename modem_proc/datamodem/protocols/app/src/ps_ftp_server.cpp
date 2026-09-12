/*=========================================================================*/
/*!
@file
ps_ftp_server.cpp

@brief 
FTP server interface  

Copyright (c) 2014-2018 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
*/
/*=========================================================================*/
/*===========================================================================
EDIT HISTORY FOR MODULE

Please notice that the changes are listed in reverse chronological order.

$Header: //components/rel/data.mpss/3.5..2.1/protocols/app/src/ps_ftp_server.cpp#2 $
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

#include "ps_FTPServer.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "ps_ftp_server.h"
#include "ds_Utils_DebugMsg.h"
#include "ps_app_logging_util.h"

#ifdef __cplusplus
}
#endif  // extern "C"

using namespace PS_FTP;

/*===========================================================================

DEFINITIONS

===========================================================================*/

/*===========================================================================

INTERNAL DATA DEFINITIONS

===========================================================================*/


/*===========================================================================

EXPORTED METHODS

===========================================================================*/


/*===========================================================================

Globals

===========================================================================*/
FTPServer *ftpServers[PS_FTP_SERVER_INST_MAX];
static char ftp_server_default_dir[] = {'/','d','a','t','a','/','f','t','p','/',0};

/*===========================================================================

METHODS

===========================================================================*/
/*===========================================================================
FUNCTION PS_FTP_SERVER_CONFIGURE

DESCRIPTION
  This function initializes the FTP server internal structures

DEPENDENCIES
  Should be call only during task bring up and not by every client

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
void ps_ftp_server_init
( 
   void
)
{
  uint8 i =0;

  for(i=0; i< PS_FTP_SERVER_INST_MAX; i++)
  {
    ftpServers[i] = NULL;
  }

}

/*===========================================================================
FUNCTION PS_FTP_SERVER_CONFIGURE

DESCRIPTION
  This function initializes the ping options (e.g, number of times to ping
  the destination) to default values.

DEPENDENCIES
  None.

RETURN VALUE
  On success, return DSS_SUCCESS. On error, return DSS_ERROR.

SIDE EFFECTS
  None.
===========================================================================*/
ps_ftp_server_inst_type ps_ftp_server_configure
( 
   dss_net_policy_info_type*         net_policy_input, 
   ps_ftp_server_config_type*        ftp_server_config_input,
   dss_ftp_server_event_cback_fn_type ftp_server_evt_cback,
   uint32                             reg_ftp_server_event_mask
)
{
  uint8 i=0;
  ps_ftp_server_inst_type server_inst = PS_FTP_SERVER_INST_INVALID;

  if( ftp_server_config_input == NULL )
  {
    PS_APP_ERR_LOG_1(ftp_server_config_input);
    return PS_FTP_SERVER_INST_INVALID;
  }

  if( NULL == ftp_server_config_input->file_directory )
  {
    PS_APP_ERR_DEBUG_0("Setting the default directory to /data/ftp");
    ftp_server_config_input->file_directory = ftp_server_default_dir;
  }

  for(i=0; i<PS_FTP_SERVER_INST_MAX; i++)
  {
    if(NULL == ftpServers[i])
    {
      ftpServers[i] = new FTPServer(i);
      server_inst = (ps_ftp_server_inst_type)i;

      if( FALSE == ftpServers[i]->Init())
      {
        PS_APP_ERR_DEBUG_0("Unable to init FTP server");
        delete ftpServers[i];
        ftpServers[i] = NULL;
        server_inst = PS_FTP_SERVER_INST_INVALID;
        break;
      }
      else if (FALSE == ftpServers[i]->Configure( net_policy_input,
                                                  ftp_server_config_input,
                                                  ftp_server_evt_cback, 
                                                  reg_ftp_server_event_mask ))
      {
        PS_APP_ERR_DEBUG_0("Unable to configure FTP server with input parameters");
        delete ftpServers[i];
        ftpServers[i] = NULL;
        server_inst = PS_FTP_SERVER_INST_INVALID;
      }
      break;
    }
    else
    {
      PS_APP_ERR_DEBUG_1("FTP server[%i] is occupied", i);
    }
  }
  
  return server_inst;
}

/*===========================================================================
FUNCTION PS_FTP_START()

DESCRIPTION
 API to start the FTP server instance that was previously configured

DEPENDENCIES
 FTP Server instance is configured
 
 
 
RETURN VALUE
  On success, return TRUE. If server is unable to start it will return
  FALSE


SIDE EFFECTS
  None.
===========================================================================*/
boolean ps_ftp_server_start
(
  ps_ftp_server_inst_type server_inst,
  ps_sockaddr_any_type* local_addr
)
{
  PS_APP_DEBUG_1("ps_ftp_server_start: starting FTP server[%i]", server_inst);
  if( local_addr == NULL ||
      server_inst <= PS_FTP_SERVER_INST_INVALID ||
      server_inst >= PS_FTP_SERVER_INST_MAX ||
      ftpServers[server_inst] == NULL)
  {
    return FALSE;
  }

  if (FALSE == ftpServers[server_inst]->Start( local_addr ) )
  {
    PS_APP_ERR_DEBUG_0("Unable to start FTP server with input parameters");
    delete ftpServers[server_inst];
    ftpServers[server_inst] = NULL;
    return FALSE; 
  }

  return TRUE;
}

/*===========================================================================
FUNCTION PS_FTP_SERVER_STOP()

DESCRIPTION
 Stop the ftp server

RETURN VALUE

SIDE EFFECTS
  None.
===========================================================================*/
void ps_ftp_server_stop
(
  ps_ftp_server_inst_type server_inst
)
{
  PS_APP_DEBUG_1("ps_ftp_server_stop: stopping FTP server[%i]", server_inst);
  if( server_inst > PS_FTP_SERVER_INST_INVALID &&
      server_inst < PS_FTP_SERVER_INST_MAX &&
      ftpServers[server_inst] != NULL)
  {
    ftpServers[server_inst]->Abort();
    delete ftpServers[server_inst];
    ftpServers[server_inst] = NULL;
  }

  return;
}

/*===========================================================================
FUNCTION PS_FTP_SERVER_CLEANUP()

DESCRIPTION
 Stop the ftp server

RETURN VALUE

SIDE EFFECTS
  None.
===========================================================================*/
void ps_ftp_server_cleanup
(
  ps_ftp_server_inst_type server_inst
)
{
  PS_APP_DEBUG_1("ps_ftp_server_cleanup: cleaning up FTP server[%i]", server_inst);
  if( server_inst > PS_FTP_SERVER_INST_INVALID &&
      server_inst < PS_FTP_SERVER_INST_MAX &&
      ftpServers[server_inst] != NULL)
  {
    ftpServers[server_inst]->Stop();
    delete ftpServers[server_inst];
    ftpServers[server_inst] = NULL;
  }

  return;
}

