/*==============================================================================

                              ds_appsrv_msg.cpp

GENERAL DESCRIPTION



  Copyright (c) 2016 by Qualcomm Technologies Incorporated. All Rights Reserved.
==============================================================================*/

/*==============================================================================
                           EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when        who    what, where, why
--------    ---    ----------------------------------------------------------
03/09/16    ml     Created file/Initial version.
==============================================================================*/
#include "ds_appsrv_msg.h"

#include "ds_appsrv_mem.h"
#include <stringl/stringl.h>
#include <cstdarg>
#include <cstdio>


/* Global variables */
extern "C"
{
  const char DS_APPSRV_ERR_LOG_EMPTY_FCN[]    = "?";
  const char DS_APPSRV_ERR_LOG_PRINT_FORMAT[] = "%s - %s";
}


/* Internal variables */
static const uint32 DS_APPSRV_MSG_MAX_LEN = 200;


/*==============================================================================
                              Err msg utils for C++
==============================================================================*/
AppsrvErrMsg::AppsrvErrMsg()
: output_msg(NULL)
{ }


AppsrvErrMsg::~AppsrvErrMsg()
{
  if(NULL != output_msg)
  {
    ds_appsrv_free(output_msg);
  }
}




void AppsrvErrMsg::init()
{
  if(NULL == output_msg)
  {
    output_msg = (char*)ds_appsrv_alloc(sizeof(char), DS_APPSRV_MSG_MAX_LEN+1);
  }
  else
  {
    memset(output_msg, 0, DS_APPSRV_MSG_MAX_LEN+1);
  }
}

void AppsrvErrMsg::set_msg(const char* fmtstr, ...)
{
  if(NULL == fmtstr || 0 == strlen(fmtstr))
    return;

  init();
  if(NULL == output_msg)
    return;

  va_list arg_list;
  va_start(arg_list, fmtstr);
  int result = vsnprintf(output_msg, DS_APPSRV_MSG_MAX_LEN, fmtstr, arg_list);

  // Check if encoding failed
  if(result < 0 || result >= DS_APPSRV_MSG_MAX_LEN)
  {
    memset(output_msg, 0, DS_APPSRV_MSG_MAX_LEN);
  }
  va_end(arg_list);
}


const char* AppsrvErrMsg::get_msg() const
{
  return output_msg;
}



/*==============================================================================
                              Err msg utils for C
==============================================================================*/
void ds_appsrv_msg_init(ds_appsrv_err_msg_s* appsrv_msg)
{
  if(NULL == appsrv_msg)
    return;

  appsrv_msg->output_msg = NULL;
}


void ds_appsrv_msg_alloc(ds_appsrv_err_msg_s* appsrv_msg)
{
  if(NULL == appsrv_msg)
    return;

  if(appsrv_msg->output_msg == NULL)
  {
    appsrv_msg->output_msg = (char*)ds_appsrv_alloc(sizeof(char), DS_APPSRV_MSG_MAX_LEN+1);
  }
  else
  {
    memset(appsrv_msg->output_msg, 0, DS_APPSRV_MSG_MAX_LEN+1);
  }
}


void ds_appsrv_msg_clean(ds_appsrv_err_msg_s* appsrv_msg)
{
  if(NULL != appsrv_msg && appsrv_msg->output_msg != NULL)
  {
    ds_appsrv_free(appsrv_msg->output_msg);
    appsrv_msg->output_msg = NULL;
  }
}


void ds_appsrv_msg_set(ds_appsrv_err_msg_s* appsrv_msg, const char* fmtstr, ...)
{
  if(NULL == appsrv_msg || NULL == appsrv_msg->output_msg || NULL == fmtstr)
    return;

  va_list arg_list;
  va_start(arg_list, fmtstr);
  int result = vsnprintf(appsrv_msg->output_msg, DS_APPSRV_MSG_MAX_LEN, fmtstr, arg_list);

  // Check if encoding failed
  if(result < 0 || result >= DS_APPSRV_MSG_MAX_LEN)
  {
    memset(appsrv_msg->output_msg, 0, DS_APPSRV_MSG_MAX_LEN);
  }
  va_end(arg_list);
}
