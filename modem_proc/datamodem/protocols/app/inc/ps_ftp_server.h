#ifndef PS_FTP_SERVER_H
#define PS_FTP_SERVER_H
/*===========================================================================

         D A T A   S E R V I C E S   F T P   H E A D E R  F I L E

DESCRIPTION

 This header file contains shared variables declarations for functions
 related to the FTP API.


Copyright (c) 2014-15 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary.

===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $PVCSPath: $
  $Header: //components/rel/data.mpss/3.5..2.1/protocols/app/inc/ps_ftp_server.h#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
02/08/14    fn     Created Module  
===========================================================================*/

/*===========================================================================

                      INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "dss_netpolicy.h"
#include "ps_in.h"
#include "dssocket_defs.h"
#include "ps_ftp_server_def.h"

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

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
);

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
   dss_net_policy_info_type*          net_policy_input, 
   ps_ftp_server_config_type*         ftp_server_config_input,
   dss_ftp_server_event_cback_fn_type ftp_server_evt_cback,
   uint32                             reg_ftp_server_event_mask
);

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
);

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
);

/*===========================================================================
FUNCTION PS_FTP_SERVER_CLEANUP()

DESCRIPTION
 called internally by FTP server

RETURN VALUE

SIDE EFFECTS
  None.
===========================================================================*/
void ps_ftp_server_cleanup
(
  ps_ftp_server_inst_type server_inst
);
#endif /* PS_FTP_SERVER_H */


