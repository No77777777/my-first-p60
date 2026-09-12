#ifndef PS_FTP_SERVER_DEF_H
#define PS_FTP_SERVER_DEF_H
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
  $Header: //components/rel/data.mpss/3.5..2.1/protocols/app/inc/ps_ftp_server_def.h#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

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

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/
#define  PS_FTP_SERVER_INST_INVALID -1
#define  PS_FTP_SERVER_INST_MAX 3
/*---------------------------------------------------------------------------
  Event sent to client when FTP server is ready 
---------------------------------------------------------------------------*/
#define  DSS_FTP_SERVER_READY_EVT  0x1

/*---------------------------------------------------------------------------
  Event sent to client when FTP server is accepting a connection
---------------------------------------------------------------------------*/
#define  DSS_FTP_SERVER_ACCEPT_EVT  0x2

/*---------------------------------------------------------------------------
  Event sent to client when FTP server is connected to a client
---------------------------------------------------------------------------*/
#define  DSS_FTP_SERVER_CONNECTED_EVT    0x4

/*---------------------------------------------------------------------------
  Event sent to client when FTP server completed transfer with a client
---------------------------------------------------------------------------*/
#define  DSS_FTP_SERVER_TRANSFER_COMPLETE_EVT    0x8

/*---------------------------------------------------------------------------
  Event sent to client when FTP server is closing a connection
---------------------------------------------------------------------------*/
#define  DSS_FTP_SERVER_CLOSED_EVT       0x10

/*---------------------------------------------------------------------------
  Handle for FTP server instance
---------------------------------------------------------------------------*/
typedef int8 ps_ftp_server_inst_type;

/*---------------------------------------------------------------------------
  This structure is used to hold FTP credentials
---------------------------------------------------------------------------*/
typedef struct
{
  char * user; /*string containing the username*/
  char * pass; /*string containing the password*/
} dss_ftp_server_credentials_type;

/*---------------------------------------------------------------------------
  This structure is used to hold information to find FTP server
---------------------------------------------------------------------------*/
typedef struct
{
  char *server_name;                   /* string containing the DNS server name */
  char *server_addr_ptr;               /* string containing the server IP address */
  ip_addr_enum_type server_addr_type;  /* Indicate server IP addr type */
  uint16 server_control_port;          /* Indicate server control port */
} dss_ftp_server_location_type;

/*---------------------------------------------------------------------------
  This structure is used to indicate the FTP Server options.
---------------------------------------------------------------------------*/
typedef struct
{
  dss_ftp_server_credentials_type client_credential;
  dss_ftp_server_location_type    ftp_server_info;
  char *  file_directory; 
  uint8   conn_retries;         /* Retries before failing FTP connection     */
  uint32  retransmit_time;      /* Interval between each TCP retries, ms     */
  uint8   dl_file_retries;      /* # of tries before erroring out for file dl*/
  uint32  dl_resp_time_out;     /* Time before stopping DL file transfer, ms */
  uint8   ul_file_retries;      /* # of tries before erroring out for file ul*/
  uint32  ul_resp_time_out;     /* Time before stopping UL file transfer, ms */
  uint32  cookie;               /* Internal cookie                           */
  boolean lookup;               /* Indicate to server to lookup netpolicy    */
  boolean bringup;              /* Indicate to server to bringup netpolicy   */
} ps_ftp_server_config_type;

/*---------------------------------------------------------------------------
  This structure is used to indicate the ping statistics. It is used
  during callbacks to the app to notify about the result of each ping.
---------------------------------------------------------------------------*/
typedef struct
{
  boolean reachable;
  uint32 dl_rate;
  uint32 ul_rate;
  boolean ul_or_dl;
  char* file_transferred;
} dss_ftp_server_stats_type;

/*---------------------------------------------------------------------------
  This structure is used to for socket address info to FTP
---------------------------------------------------------------------------*/
typedef struct
{
  dss_protocol_family_enum_type type;     /**< Address type. */
  /** IP address structure. */
  union
  {
    struct ps_sockaddr_in  v4;   /**< IPv4 sock address. */
    struct ps_sockaddr_in6 v6;   /**< IPv6 sock address. */
  } sockaddr;
} ps_sockaddr_any_type;

/*---------------------------------------------------------------------------
  Event callback mechanism to clients to listen to FTP server events
---------------------------------------------------------------------------*/
typedef void (*dss_ftp_server_event_cback_fn_type )
(
  ps_ftp_server_inst_type          ps_ftp_server_inst,
  uint32                           ftp_server_event_mask,
  void *                           ftp_server_event_data,
  uint16                           ftp_server_event_data_len
);

#endif /* PS_FTP_SERVER_DEF_H */

