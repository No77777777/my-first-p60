/*===========================================================================

                     P S _ F T P _ P A R S E R . C

DESCRIPTION
 The Data Services FTP API File. Contains API functions for File Transfer
 Protocol ( FTP )

EXTERNALIZED FUNCTIONS
  dss_ftp_init_options()
    Initializes the FTP configuration structure to default values.
  dss_ftp_connect()
    Connects to remote destination with the user specified FTP config.
  dss_ftp_write()
    Use to write file to a connected FTP server.
  dss_ftp_read()
    Use to read file from a connected FTP server. 
  dss_ftp_stop()
    Stop FTP transfer. Can be used to stop an ongoing FTP session.

Copyright (c) 2014-15 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary.

===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $PVCSPath: $
  $Header: //components/rel/data.mpss/3.5..2.1/protocols/app/src/ps_ftp_server_pb.c#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
06/06/18    fn     Fix Incorrect message in user/pass/login states
02/08/14    fn     Created Module
===========================================================================*/
#include "comdef.h"
#include "target.h"
#include "customer.h"
#include "msg.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ds_Utils_DebugMsg.h"
#include "ps_ftp_server_pb.h"
#include "ps_ftp_server_def.h"
#include "ps_app_logging_util.h"
#include "data_msg.h"

/*===========================================================================

                      INTERNAL FUNCTION DEFINITIONS

===========================================================================*/
#define FTP_MSG_SPACE 0x20
#define FTP_MSG_RETURN 0x0d
#define FTP_MSG_NEWLINE 0x0a

char ftp_server_pb_cmd_strings[FTP_MAX_COMMAND_STRINGS][5] =
{
  "USER",
  "PASS",
  "PASV",
  "RETR",
  "SYST",
  "EPSV",
  "TYPE",
  "PORT",
  "QUIT"
};


/*===========================================================================
STATIC FUNCTION ps_ftp_build_result_code

DESCRIPTION:

PARAMETERS:

RETURN VALUE:
  void

DEPENDENCIES:
  None.
===========================================================================*/
boolean ps_ftp_build_result_code
(
   uint16 code, 
   char* buffer,
   uint16* offset
)
{
  if ( NULL == buffer || NULL == offset )
  {
    PS_APP_ERR_DEBUG_2("Invalid input parameters to build result code"
                       "buffer p=0x%p offset p=0x%p",
                       buffer, offset);
    return FALSE;
  }

  buffer[(*offset)++] = ((code & FTP_MSG_THIRD_DIGIT_MASK) >> 8) + 0x30;
  buffer[(*offset)++] = ((code & FTP_MSG_SECOND_DIGIT_MASK) >> 4) + 0x30;
  buffer[(*offset)++] = (code & FTP_MSG_FIRST_DIGIT_MASK) + 0x30;
  buffer[(*offset)++] = FTP_MSG_SPACE;

  return TRUE;
}

void convert_uint8_to_ascii
(
   uint8* num,
   char * num_str,
   uint16* offset
)
{
  uint8* ptr = num;
  uint16 index = 0;

  PS_APP_DEBUG_2("convert_uint8_to_ascii, offset %d num_str[offset] %c", 
                 *offset, *num_str);

  index = *offset;

  if ( ((uint8)(*ptr/100)) > 0)
  {
    num_str[index++] = (uint8)(*ptr/100) + 0x30; 
    num_str[index++] = (uint8)((*ptr/10)%10) + 0x30;      
  }
  else if( ((uint8)(*ptr/10)) > 0)
  {
    num_str[index++] = (uint8)(*ptr/10) + 0x30;
  }

  num_str[index++] = (uint8)(*ptr%10) + 0x30;

  *offset = index ;

  PS_APP_DEBUG_1("finishing convert_uint8_to_ascii offset %d", *offset);
}
/*===========================================================================
STATIC FUNCTION ps_ftp_build_sockinfo

DESCRIPTION:

PARAMETERS:

RETURN VALUE:
  void

DEPENDENCIES:
  None.
===========================================================================*/
boolean ps_ftp_build_sockinfo
(
   ps_sockaddr_any_type* localaddr, 
   char* buffer,
   uint16* offset
)
{
  uint8 i = 0;
  uint8 *ptr;


  if ( NULL == buffer || NULL == offset || NULL == localaddr )
  {
    PS_APP_ERR_DEBUG_2("Invalid input parameters to build result code"
                       "buffer p=0x%p offset p=0x%p",
                       buffer, offset);
    return FALSE;
  }

  if ( localaddr->type == DSS_AF_INET )
  {
    PS_APP_DEBUG_2("ps_s_addr =0x%lx, ps_sin_port = 0x%lx",
                    localaddr->sockaddr.v4.ps_sin_addr.ps_s_addr,
                    localaddr->sockaddr.v4.ps_sin_port);

    ptr = (uint8*)&localaddr->sockaddr.v4.ps_sin_addr.ps_s_addr;
    for (i=0; i< sizeof(uint32); i++)
    {
      convert_uint8_to_ascii(ptr++, buffer, offset);
      buffer[(*offset)++] = ',';
    }

    PS_APP_DEBUG_0("Add port");
    ptr = (uint8 *)&localaddr->sockaddr.v4.ps_sin_port;
    for (i=0; i< sizeof(uint16); i++)
    {
      convert_uint8_to_ascii(ptr++, buffer, offset);
      buffer[(*offset)++] = ',';
    }

    //remove last comma
    *offset = *offset - 1;
    buffer[*offset] = '\0'; 
  }
  else if ( localaddr->type == DSS_AF_INET6 )
  {
    /*RFC 2428 and rfc */
    PS_APP_DEBUG_0("Add port");
    ptr = (uint8 *)&localaddr->sockaddr.v6.ps_sin6_port;
    for (i=0; i< sizeof(uint16); i++)
    {
      convert_uint8_to_ascii(ptr++, buffer, offset);
      buffer[(*offset)++] = ',';
    }

    //remove last comma
    *offset = *offset - 1;
    buffer[*offset] = '\0';
  }

  return TRUE;
}

/*===========================================================================
STATIC FUNCTION ps_ftp_print_msg_server_ready

DESCRIPTION:

PARAMETERS:

RETURN VALUE:
  void

DEPENDENCIES:
  None.
===========================================================================*/
void ps_ftp_print_msg_server_ready
(
  char* buffer,
  int buffer_size,
  sint15* msg_len
)
{
  uint16 offset = 0;
  uint16 code;
  
  code = FTP_MSG_POS_COMPLETE_CODE | FTP_MSG_CONNECTION_CODE;
  ps_ftp_build_result_code(code, buffer, &offset);
    
  strlcat( &buffer[offset], "FTP Server ready.\r\n", buffer_size );
  *msg_len = strlen(buffer);

  PS_APP_DEBUG_1("Sending FTP Server Ready msg, msg_len = %ld",
                  *msg_len);
}

/*===========================================================================
STATIC FUNCTION ps_ftp_print_msg_server_ready

DESCRIPTION:

PARAMETERS:

RETURN VALUE:
  void

DEPENDENCIES:
  None.
===========================================================================*/
void ps_ftp_print_msg_unknown_cmd
(
  char* buffer,
  int buffer_size,
  sint15* msg_len
)
{
  uint16 offset = 0;
  uint16 code;
  
  code = FTP_MSG_NEG_PERM_COMPLETE_CODE;
  ps_ftp_build_result_code(code, buffer, &offset);
  
  strlcat( &buffer[offset], "Syntax error, command unrecognized.\r\n",buffer_size );
  *msg_len = strlen(buffer);

  PS_APP_DEBUG_1("Sending FTP Server Unknown cmd in msg, msg_len = %ld",
                  *msg_len);
}


/*===========================================================================
STATIC FUNCTION ps_ftp_print_msg_server_ready

DESCRIPTION:

PARAMETERS:

RETURN VALUE:
  void

DEPENDENCIES:
  None.
===========================================================================*/
void ps_ftp_print_msg_pass_require
(
  char* buffer,
  int buffer_size,
  sint15* msg_len
)
{
  uint16 offset = 0;
  uint16 code;
  
  code = FTP_MSG_POS_INTER_CODE | FTP_MSG_AUTH_ACC_CODE | 1;
  ps_ftp_build_result_code(code, buffer, &offset);
  
  strlcat( &buffer[offset], "User name okay, need password.\r\n",buffer_size);
  *msg_len = strlen(buffer);

  PS_APP_DEBUG_1("Sending FTP Server request password msg, msg_len = %ld",
                  *msg_len);
}

/*===========================================================================
STATIC FUNCTION ps_ftp_print_msg_server_ready

DESCRIPTION:

PARAMETERS:

RETURN VALUE:
  void

DEPENDENCIES:
  None.
===========================================================================*/
void ps_ftp_print_msg_user_logged_in
(
  char* buffer,
  int buffer_size,
  sint15* msg_len
)
{
  uint16 offset = 0;
  uint16 code;
  
  code = FTP_MSG_POS_COMPLETE_CODE | FTP_MSG_AUTH_ACC_CODE;
  ps_ftp_build_result_code(code, buffer, &offset);
  
  strlcat( &buffer[offset], "User logged in, proceed.\r\n",buffer_size );
  *msg_len = strlen(buffer);

  PS_APP_DEBUG_1("Sending FTP Server user logged in msg, msg_len = %ld",
                  *msg_len);
}

/*===========================================================================
STATIC FUNCTION ps_ftp_print_msg_pasv

DESCRIPTION:

PARAMETERS:

RETURN VALUE:
  void

DEPENDENCIES:
  None.
===========================================================================*/
void ps_ftp_print_msg_pasv
(
  char* buffer,
  int buffer_size,
  sint15* msg_len,
  void* user_arg
)
{
  uint16 offset = 0;
  uint16 code;
  ps_sockaddr_any_type* sock_addr;
  char pasv_resp[50] = "Entering Passive Mode (";
  char pasv_ip6_resp[50] = "Entering Passive Mode (127,127,127,127,";
 
  sock_addr = (ps_sockaddr_any_type*)user_arg;
   
  if ( sock_addr->type == DSS_AF_INET )
  {
    code = FTP_MSG_POS_COMPLETE_CODE | FTP_MSG_CONNECTION_CODE | 7;
    ps_ftp_build_result_code(code, buffer, &offset);
    
    strlcat(&buffer[offset],pasv_resp, buffer_size);

    /*subtract one for null terminator*/
    offset += strlen(pasv_resp); 

    /*strlcat( &buffer[offset], "Entering Passive Mode (h1,h2,h3,h4,p1,p2).\r\n" );*/

    ps_ftp_build_sockinfo(sock_addr, buffer, &offset);


    strlcat( &buffer[offset], ").\r\n", buffer_size);

    *msg_len = strlen(buffer);

    PS_APP_SPRINTF_DEBUG_1("FTP response final: %s",buffer );
    PS_APP_DEBUG_1("Sending FTP Server PASV msg, msg_len = %ld",
                    *msg_len);
  }
  else
  {
    code = FTP_MSG_POS_COMPLETE_CODE | FTP_MSG_CONNECTION_CODE | 7;
    ps_ftp_build_result_code(code, buffer, &offset);
    
    strlcat(&buffer[offset],pasv_ip6_resp, buffer_size);

    /*subtract one for null terminator*/
    offset += strlen(pasv_ip6_resp); 

    /*strlcat( &buffer[offset], "Entering Passive Mode (h1,h2,h3,h4,p1,p2).\r\n" );*/

    ps_ftp_build_sockinfo(sock_addr, buffer, &offset);

    strlcat( &buffer[offset], ").\r\n", buffer_size );

    *msg_len = strlen(buffer);

    PS_APP_SPRINTF_DEBUG_1("FTP response final: %s",buffer );
    PS_APP_DEBUG_1("Sending FTP Server PASV msg, msg_len = %ld",
                    *msg_len);
  }
  /*else
  {
    code = FTP_MSG_NEG_TRANS_COMPLETE_CODE | FTP_MSG_CONNECTION_CODE | 5;
    ps_ftp_build_result_code(code, buffer, &offset);
    strlcat(&buffer[offset],"You cannot use PASV for ipv6, please use EPSV. \r\n");
  }*/
}

/*===========================================================================
STATIC FUNCTION ps_ftp_print_msg_retr

DESCRIPTION:

PARAMETERS:

RETURN VALUE:
  void

DEPENDENCIES:
  None.
===========================================================================*/
void ps_ftp_print_msg_retr
(
  char* buffer,
  int buffer_size,
  sint15* msg_len,
  void * user_arg
)
{
  uint16 offset = 0;
  uint16 code;
  boolean file_readable;


  file_readable = *((boolean*) user_arg);

  if ( file_readable )
  {
    code = FTP_MSG_POS_PRELIM_CODE | FTP_MSG_FILE_SYS_CODE;
    ps_ftp_build_result_code(code, buffer, &offset);
    strlcat(&buffer[offset], "Openning data connection in Ascii mode.\r\n",buffer_size);
  }
  else
  {
    code = FTP_MSG_NEG_PERM_COMPLETE_CODE | FTP_MSG_FILE_SYS_CODE;
    ps_ftp_build_result_code(code, buffer, &offset);
    strlcat(&buffer[offset], "File not accessable.\r\n",buffer_size);
  }

  *msg_len = strlen(buffer);

  PS_APP_DEBUG_1("Sending FTP Server RETR msg, msg_len = %ld",
                 *msg_len);
}

/*===========================================================================
STATIC FUNCTION ps_ftp_print_msg_server_ready

DESCRIPTION:

PARAMETERS:

RETURN VALUE:
  void

DEPENDENCIES:
  None.
===========================================================================*/
void ps_ftp_print_msg_transfer_complete
(
  char* buffer,
  int buffer_size,
  sint15* msg_len,
  void *  user_arg
)
{
  uint16 offset = 0;
  uint16 code;
  boolean transfer_successful;

  if ( NULL == buffer || NULL == user_arg )
  {
    PS_APP_ERR_DEBUG_3("Invalid input parameters 0x%p, 0x%p, 0x%p",
                       buffer, msg_len, user_arg);
    return;
  }

  transfer_successful = *((boolean *)user_arg);
  
  if ( TRUE == transfer_successful )
  {
    code = FTP_MSG_POS_COMPLETE_CODE | FTP_MSG_CONNECTION_CODE | 6;
    ps_ftp_build_result_code(code, buffer, &offset);

    strlcat(&buffer[offset], "Transfer Complete.\r\n",buffer_size);
    *msg_len = strlen(buffer);
  }
  else
  {
    //FN todo expand on failure cases
    code = FTP_MSG_NEG_TRANS_COMPLETE_CODE | FTP_MSG_FILE_SYS_CODE;
    ps_ftp_build_result_code(code, buffer, &offset);

    strlcat(&buffer[offset], "Transfer Failed.\r\n",buffer_size);
    *msg_len = strlen(buffer);
  }
}

/*===========================================================================
STATIC FUNCTION ps_ftp_print_msg_server_ready

DESCRIPTION:

PARAMETERS:

RETURN VALUE:
  void

DEPENDENCIES:
  None.
===========================================================================*/
void ps_ftp_print_msg_system_info
(
  char* buffer,
  int buffer_size,
  sint15* msg_len
)
{
  uint16 offset = 0;
  uint16 code;

  code = FTP_MSG_POS_COMPLETE_CODE | FTP_MSG_INFORMATION_CODE | 5; 
  ps_ftp_build_result_code(code, buffer, &offset);
  
  strlcat( &buffer[offset], "QCOM System Type: L8.\r\n",buffer_size );
  *msg_len = strlen(buffer);

  PS_APP_DEBUG_1("Sending FTP Sys infomation, msg_len = %ld",
                  *msg_len);
}

/*===========================================================================
STATIC FUNCTION ps_ftp_print_msg_type

DESCRIPTION:

PARAMETERS:

RETURN VALUE:
  void

DEPENDENCIES:
  None.
===========================================================================*/
void ps_ftp_print_msg_type
(
  char* buffer,
  int buffer_size,
  sint15* msg_len,
  void *  user_arg
)
{
  uint16 offset = 0;
  uint16 code;
  char transfer_mode_type;

  transfer_mode_type = *((char *)user_arg);
  
  if ( 'I' == transfer_mode_type ||
       'i' == transfer_mode_type )
  {

    code = FTP_MSG_POS_COMPLETE_CODE;
    ps_ftp_build_result_code(code, buffer, &offset);

    strlcat(&buffer[offset], " Type set to I.\r\n",buffer_size);
    *msg_len = strlen(buffer);
  }
  else
  {
    PS_APP_ERR_DEBUG_1("FTP server currently only supports I/i, client type = %c",
                       transfer_mode_type);
    code = FTP_MSG_POS_COMPLETE_CODE;
    /*should send code 500*/
    //code = FTP_MSG_NEG_PERM_COMPLETE_CODE  | 2
    ps_ftp_build_result_code(code, buffer, &offset);

    strlcat(&buffer[offset], " Type set to I.\r\n",buffer_size);
    //strlcat(&buffer[offset], " Type Not supported.\r\n");
    *msg_len = strlen(buffer);

  }
}

/*===========================================================================
STATIC FUNCTION ps_ftp_print_msg_server_ready

DESCRIPTION:

PARAMETERS:

RETURN VALUE:
  void

DEPENDENCIES:
  None.
===========================================================================*/
void ps_ftp_print_msg_port
(
  char* buffer,
  int buffer_size,
  sint15* msg_len,
  void *  user_arg
)
{
  uint16 offset = 0;
  uint16 code;
  boolean port_success;

  port_success = *((boolean *)user_arg);
  
  if ( TRUE == port_success )
  {

    code = FTP_MSG_POS_COMPLETE_CODE;
    ps_ftp_build_result_code(code, buffer, &offset);

    strlcat(&buffer[offset], "PORT command successful.\r\n", buffer_size);
    *msg_len = strlen(buffer);
  }
  else
  {
    code = FTP_MSG_NEG_PERM_COMPLETE_CODE;
    ps_ftp_build_result_code(code, buffer, &offset);

    strlcat(&buffer[offset], "PORT command FAILED.\r\n", buffer_size);
    *msg_len = strlen(buffer);
  }
}

/*===========================================================================
STATIC FUNCTION ps_ftp_print_msg_quit

DESCRIPTION:

PARAMETERS:

RETURN VALUE:
  void

DEPENDENCIES:
  None.
===========================================================================*/
void ps_ftp_print_msg_quit
(
  char* buffer,
  int buffer_size,
  sint15* msg_len
)
{
  uint16 offset = 0;
  uint16 code;

  code = FTP_MSG_POS_COMPLETE_CODE | FTP_MSG_CONNECTION_CODE | 1; 
  ps_ftp_build_result_code(code, buffer, &offset);
  
  strlcat( &buffer[offset], " Goodbye.\r\n", buffer_size );
  *msg_len = strlen(buffer);
}

/*===========================================================================

                      EXTERNAL FUNCTION DEFINITIONS

===========================================================================*/
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
   char* port_arg,
   uint16 arg_len
)
{
   char port_char[5] = "";
   uint16 port = 0;
   char *ind1 = NULL, *ind2 = NULL, *ind3 = NULL, 
        *ind_end = NULL, *ind_last_comma = NULL;
   uint8 i = 0;
  /*-------------------------------------------------------------------------
   (PASV does not support v6 but EPSV does RFC 2428. Currently EPSV is not
   support by this function (FN TODO for ipv6 FTP support)
  -------------------------------------------------------------------------*/
   if ( NULL == port_arg)
   {
     return -1;
   }

  PS_APP_SPRINTF_DEBUG_1("Parsing port from FTP client: %s",port_arg );
  /*-------------------------------------------------------------------------
   RFC 959: PASV response structure :
     A->C : 227 Entering Passive Mode. A1,A2,A3,A4,a1,a2
     227 Entering Passive Mode (10,226,46,24,129,108)
     or
     PORT request structure
       PORT fe80::7dcc:efae:cbd5:5bd4%rmnet7,147,207
  -------------------------------------------------------------------------*/
  ind1 = port_arg;
  /*-------------------------------------------------------------------------
    Find the end of PASV resp which is indicated by closing paranteless ")"
  -------------------------------------------------------------------------*/
  ind_end = strchr(ind1, ')');

  if ( ind_end == NULL )
  {
    ind_end = strchr(ind1, '\r');
    PS_APP_DEBUG_3("strlen = %i, ind_end = 0x%p, ind1 = %c ",
                  strlen(ind1),ind_end, *ind1);
  }

  if (ind_end == NULL )
  {
    ind_end = ind1 + strlen(ind1);
    PS_APP_DEBUG_3("strlen = %i, ind_end = %c, ind1 = %c ",
                  strlen(ind1),*ind_end, *ind1);
  }

  if ( ind_end > (ind1 + arg_len) )
  {
    PS_APP_ERR_DEBUG_3("end index 0x%p exceeds arg 0x%p len %d ",
                       ind_end, ind1, arg_len);
    return port;
  }

  /*-------------------------------------------------------------------------
  Find last two comma argument to parse the ports (variable length 1-3) 
  port = a1 * 256 + a2;
   
   Example response 
   227 Entering Passive Mode (ipv6/ipv4/int, port/256, port%256)
   
   small optimization can be added
     ,255,255) the last comma is at most 8 away from ind_end
     ,1,1) but is not alway 8 away. minimum is 4. 
  -------------------------------------------------------------------------*/
  //ind1 = ind_end - 8;

  do
  {
    ind2 = strchr(ind1, ',');

    if ( NULL != ind2 )
    {
      ind3 = ind_last_comma;
      ind_last_comma = ind2;
      ind1 = ind2 + 1;
      continue;
    }
    i++;
  }while(ind2 != NULL && i < 10);

  if ( ind3 == NULL || ind_last_comma == NULL )
  {
    PS_APP_ERR_DEBUG_3("Unexpected null ind1 0x%p ind2 %p i=%d ",
                       ind3,ind_last_comma,i);
    return 0;
  }

  strlcpy(port_char, ind3+1, ind_last_comma -ind3 );
  port_char[ind_last_comma - ind3 -1] = '\0';
  port = atoi((port_char)) << 8;
  strlcpy(port_char, ind_last_comma+1, ind_end - ind_last_comma );
  port_char[ind_end - ind_last_comma - 1] = '\0';
  port |= atoi((port_char));
  PS_APP_DEBUG_1("calculated port 0x%x",port);

  return port;
}

/*===========================================================================
STATIC FUNCTION ps_ftp_send_response

DESCRIPTION:


PARAMETERS:



RETURN VALUE:
  void

DEPENDENCIES:
  None.
===========================================================================*/
boolean ps_ftp_build_response
(
  char                  *buffer,
  int                   buffer_size,
  sint15                *response_len,
  ftp_server_msg_e_type ftp_msg_type,
  void                  *user_arg
)
{

  if ( NULL == buffer || NULL == response_len )
  {
    PS_APP_ERR_DEBUG_2("Invalid input parameters to build server response"
                       "buffer p=0x%p, response_len = 0x%p",
                       buffer, response_len);
    return FALSE;
  }  

  switch ( ftp_msg_type )
  {
     case FTP_SERVER_MSG_SERVER_READY:
       ps_ftp_print_msg_server_ready(buffer, buffer_size, response_len);
       break;
     case FTP_SERVER_MSG_PASS_REQUIRE:
       ps_ftp_print_msg_pass_require(buffer, buffer_size, response_len);
       break;
     case FTP_SERVER_MSG_WELCOME:
       ps_ftp_print_msg_user_logged_in(buffer, buffer_size, response_len);
       break;
     case FTP_SERVER_MSG_PASV:
       ps_ftp_print_msg_pasv(buffer,buffer_size, response_len, user_arg);
       break;
     case FTP_SERVER_MSG_RETR:
       ps_ftp_print_msg_retr(buffer,buffer_size,response_len, user_arg);
       break;
     case FTP_SERVER_MSG_TRANSFER_COMPLETE:
       ps_ftp_print_msg_transfer_complete(buffer, buffer_size, 
                                          response_len, user_arg);
       break;
     case FTP_SERVER_MSG_SYSTEM_INFO:
       ps_ftp_print_msg_system_info(buffer, buffer_size, 
                                    response_len);
       break;
     case FTP_SERVER_MSG_TYPE:
       ps_ftp_print_msg_type(buffer,buffer_size,
                             response_len, user_arg);
       break;
     case FTP_SERVER_MSG_PORT:
       ps_ftp_print_msg_port(buffer, buffer_size,
                              response_len, user_arg);
       break;
     case FTP_SERVER_MSG_QUIT:
       ps_ftp_print_msg_quit(buffer,buffer_size,response_len);
       break;
     case FTP_SERVER_MSG_UNKNOWN_CMD:
     default:
       PS_APP_ERR_DEBUG_1("Unsupported FTP response? %d", ftp_msg_type);
       ps_ftp_print_msg_unknown_cmd(buffer,buffer_size,response_len);
       break;
  }

  return TRUE;
}

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
)
{
  uint8 i = 0; 
  int len_of_cmd = 0;
  char *space_ptr = NULL;
  ftp_cmd_resp_type* ftp_cmd_resp;
  boolean space = FALSE;

  /*-------------------------------------------------------------------------
   Look for the first space in the request to the server
  -------------------------------------------------------------------------*/
  if ( NULL == ftp_response || NULL == buffer )
  {
    PS_APP_ERR_DEBUG_2("Invalid inputs response 0x%p buffer 0x%p",
                       ftp_response, buffer);
    return FALSE;
  }

  ftp_cmd_resp = &ftp_response->client_cmd_resp;
  ftp_cmd_resp->cmd_arg = NULL;
  space_ptr = buffer;

  for (i=0; i< response_len; i++)
  {
    if( (*space_ptr) == ' ')
    {
      space = TRUE;
      len_of_cmd = space_ptr - buffer;
      ftp_cmd_resp->cmd_arg = space_ptr + 1;
    }
    else if ((*space_ptr) == '\r')
    {
      /*replace \r with null terminator for string operation */
      *space_ptr = '\0';
      break;
    }
    space_ptr++;
  }

  PS_APP_SPRINTF_DEBUG_1("Parsing cmd from FTP client: %s",buffer );

  if ( FALSE == space )
  {
    len_of_cmd = space_ptr - buffer;
  }

  PS_APP_DEBUG_3("space located at 0x%p, buffer located at 0x%p, len_of_cmd = %i ",
                  space_ptr, buffer,len_of_cmd);

  for ( i = 0; i < FTP_MAX_COMMAND_STRINGS; i++)
  {

    if (0 == strncasecmp(buffer, ftp_server_pb_cmd_strings[i], len_of_cmd) )
    {
      ftp_cmd_resp->cmd = i;
      if ( TRUE == space )
      {
        PS_APP_SPRINTF_DEBUG_1("FTP cmd %s", ftp_server_pb_cmd_strings[i]);
        PS_APP_SPRINTF_DEBUG_1("FTP argument %s",ftp_cmd_resp->cmd_arg);
      }
      return TRUE;
    }
  }

  PS_APP_ERR_DEBUG_0("Unable to find FTP cmd string on server, cmd not supported");
  ftp_cmd_resp->cmd = FTP_MAX_COMMAND_STRINGS;
  ftp_cmd_resp->cmd_arg = NULL;

  return FALSE;
}
