#ifndef PS_FTP_SERVER_PB_H
#define PS_FTP_SERVER_PB_H
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
  $Header: //components/rel/data.mpss/3.5..2.1/protocols/app/src/ps_ftp_server_pb.h#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
02/08/14    fn     Created Module
===========================================================================*/

/*===========================================================================

                      INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/*---------------------------------------------------------------------------
  Third digit of FTP return code 
---------------------------------------------------------------------------*/

#define FTP_MSG_POS_PRELIM_CODE           0x100
#define FTP_MSG_POS_COMPLETE_CODE         0x200
#define FTP_MSG_POS_INTER_CODE            0x300
#define FTP_MSG_NEG_TRANS_COMPLETE_CODE   0x400
#define FTP_MSG_NEG_PERM_COMPLETE_CODE    0x500
#define FTP_MSG_PROTECTED_CODE            0x600

/*---------------------------------------------------------------------------
  Second digit of FTP return code 
---------------------------------------------------------------------------*/
#define FTP_MSG_SYNTAX_CODE              0x000
#define FTP_MSG_INFORMATION_CODE         0x010
#define FTP_MSG_CONNECTION_CODE          0x020
#define FTP_MSG_AUTH_ACC_CODE            0x030
#define FTP_MSG_UNKNOWN_CODE             0x040
#define FTP_MSG_FILE_SYS_CODE            0x050

/*---------------------------------------------------------------------------
 FTP code digit masks
---------------------------------------------------------------------------*/
#define FTP_MSG_FIRST_DIGIT_MASK      0x00F
#define FTP_MSG_SECOND_DIGIT_MASK     0x0F0
#define FTP_MSG_THIRD_DIGIT_MASK      0xF00

typedef enum
{
  FTP_MSG_USER_CMD,
  FTP_MSG_PASS_CMD,
  FTP_MSG_PASV_CMD,
  FTP_MSG_RETR_CMD,
  FTP_MSG_SYST_CMD,
  FTP_MSG_EPSV_CMD,
  FTP_MSG_TYPE_CMD,
  FTP_MSG_PORT_CMD,
  FTP_MSG_QUIT_CMD,
  FTP_MAX_COMMAND_STRINGS,
}ftp_server_pb_cmd_e_type;

typedef enum 
{
 FTP_CLIENT_MSG_INVALID,
 FTP_CLIENT_MSG_CMD,
 FTP_CLIENT_MSG_USER,
 FTP_CLIENT_MSG_PASS,
 FTP_CLIENT_MSG_PASV,
 FTP_CLIENT_MSG_RETR,
 FTP_CLIENT_MSG_EPSV
}ftp_client_msg_e_type;

typedef enum 
{
  FTP_SERVER_MSG_INVALID,
  FTP_SERVER_MSG_SERVER_READY,
  FTP_SERVER_MSG_PASS_REQUIRE,
  FTP_SERVER_MSG_WELCOME,
  FTP_SERVER_MSG_PASV,
  FTP_SERVER_MSG_RETR,
  FTP_SERVER_MSG_TRANSFER_COMPLETE,
  FTP_SERVER_MSG_SYSTEM_INFO,
  FTP_SERVER_MSG_EPSV,
  FTP_SERVER_MSG_TYPE,
  FTP_SERVER_MSG_PORT,
  FTP_SERVER_MSG_START_DATA,
  FTP_SERVER_MSG_QUIT,
  FTP_SERVER_MSG_UNKNOWN_CMD,
}ftp_server_msg_e_type;

typedef struct
{
  char * username;
}ftp_username_resp_type;

typedef struct
{
  ftp_server_pb_cmd_e_type cmd;
  char * cmd_arg;
}ftp_cmd_resp_type;


typedef union
{
  ftp_username_resp_type username_resp;
  ftp_cmd_resp_type      client_cmd_resp;
}ftp_msg_response_type;

/*===========================================================================
STATIC FUNCTION ps_ftp_build_response

DESCRIPTION:


PARAMETERS:



RETURN VALUE:
  void

DEPENDENCIES:
  None.
===========================================================================*/
boolean ps_ftp_build_response
(
  char *buffer,
  int buffer_size,
  sint15 *response_len,
  ftp_server_msg_e_type ftp_msg_type,
  void *user_arg
);

/*===========================================================================
STATIC FUNCTION ps_ftp_build_response

DESCRIPTION:


PARAMETERS:



RETURN VALUE:
  void

DEPENDENCIES:
  None.
===========================================================================*/
boolean ps_ftp_parse_response
(
  char *buffer,
  uint32 response_len,
  ftp_msg_response_type* ftp_response
);

/*===========================================================================
STATIC FUNCTION dss_ftp_parse_port

DESCRIPTION:


PARAMETERS:



RETURN VALUE:
  0 for invalid ports
  >0 if port parsing succeeded

DEPENDENCIES:
  None.
===========================================================================*/
uint16 ps_ftp_parse_port
(
  char* port_cmd_arg,
  uint16 arg_len
);
#endif /* PS_FTP_SERVER_PB_H */

