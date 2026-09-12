/*==============================================================================

                            ds_appsrv_msg.h

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
#ifndef DS_APPSRV_MSG_H
#define DS_APPSRV_MSG_H

#include "comdef.h"
#include "data_msg.h"
#include <stringl/stringl.h>


#ifdef __cplusplus
extern "C" {
#endif

extern const char DS_APPSRV_ERR_LOG_EMPTY_FCN[];
extern const char DS_APPSRV_ERR_LOG_PRINT_FORMAT[];

#ifdef __cplusplus
}
#endif

/*==============================================================================
                              Common
==============================================================================*/
#define DS_APPSRV_MSG_ERR_PRINT(appsrv_msg, fcn_name) \
  do {                                                \
    const char* fcn_name_ptr = fcn_name;              \
    if(NULL == appsrv_msg.output_msg)                 \
    {                                                 \
      break;                                          \
    }                                                 \
    if(NULL == fcn_name_ptr)                          \
    {                                                 \
      fcn_name_ptr = DS_APPSRV_ERR_LOG_EMPTY_FCN;     \
    }                                                 \
    DATA_APPSRV_MSG_SPRINTF_2(                        \
                              MSG_LEGACY_ERROR,       \
                              DS_APPSRV_ERR_LOG_PRINT_FORMAT, \
                              fcn_name_ptr,           \
                              appsrv_msg.output_msg   \
                              );                      \
  } while(0)




/*==============================================================================
                              Err msg utils for C
==============================================================================*/
typedef struct
{
  char* output_msg;
} ds_appsrv_err_msg_s;


#ifdef __cplusplus
extern "C" {
#endif

void ds_appsrv_msg_init(ds_appsrv_err_msg_s* appsrv_msg);
void ds_appsrv_msg_alloc(ds_appsrv_err_msg_s* appsrv_msg);
void ds_appsrv_msg_clean(ds_appsrv_err_msg_s* appsrv_msg);
void ds_appsrv_msg_set(ds_appsrv_err_msg_s* appsrv_msg, const char* fmtstr, ...);

#ifdef __cplusplus
}
#endif



#ifndef __cplusplus


#define DS_APPSRV_MSG_ERR_INIT(appsrv_msg) ds_appsrv_msg_init(&appsrv_msg)

#define DS_APPSRV_MSG_ERR_FLUSH(appsrv_msg, fcn_name) \
{                                                \
  DS_APPSRV_MSG_ERR_PRINT(appsrv_msg, fcn_name); \
  ds_appsrv_msg_clean(&appsrv_msg);              \
}

/* If msg is set calling DS_APPSRV_MSG_ERR_FLUSH() is REQUIRED to free allocated memory */
#define DS_APPSRV_MSG_ERR_SET0(appsrv_msg, fmtstr) \
  {                                           \
    ds_appsrv_msg_alloc(&appsrv_msg);         \
    ds_appsrv_msg_set( &appsrv_msg, fmtstr ); \
  }

#define DS_APPSRV_MSG_ERR_SET1(appsrv_msg, fmtstr, arg1) \
  {                                         \
    ds_appsrv_msg_alloc(&appsrv_msg);       \
    ds_appsrv_msg_set( &appsrv_msg, fmtstr, \
                       arg1 );              \
  }

#define DS_APPSRV_MSG_ERR_SET2(appsrv_msg, fmtstr, arg1, arg2) \
  {                                         \
    ds_appsrv_msg_alloc(&appsrv_msg);       \
    ds_appsrv_msg_set( &appsrv_msg, fmtstr, \
                       arg1, arg2 );        \
  }

#define DS_APPSRV_MSG_ERR_SET3(appsrv_msg, fmtstr, arg1, arg2, arg3) \
  {                                         \
    ds_appsrv_msg_alloc(&appsrv_msg);       \
    ds_appsrv_msg_set( &appsrv_msg, fmtstr, \
                       arg1, arg2, arg3 );  \
  }

#define DS_APPSRV_MSG_ERR_SET4(appsrv_msg, fmtstr, arg1, arg2, arg3, arg4) \
  {                                         \
    ds_appsrv_msg_alloc(&appsrv_msg);       \
    ds_appsrv_msg_set( &appsrv_msg, fmtstr, \
                       arg1, arg2,          \
                       arg3, arg4 );        \
  }

#define DS_APPSRV_MSG_ERR_SET5(appsrv_msg, fmtstr, arg1, arg2, arg3, arg4, arg5) \
  {                                         \
    ds_appsrv_msg_alloc(&appsrv_msg);       \
    ds_appsrv_msg_set( &appsrv_msg, fmtstr, \
                       arg1, arg2, arg3,    \
                       arg4, arg5 );        \
  }

#define DS_APPSRV_MSG_ERR_SET6(appsrv_msg, fmtstr, arg1, arg2, arg3, arg4, arg5, arg6) \
  {                                         \
    ds_appsrv_msg_alloc(&appsrv_msg);       \
    ds_appsrv_msg_set( &appsrv_msg, fmtstr, \
                       arg1, arg2, arg3,    \
                       arg4, arg5, arg6 );  \
  }

#endif /* __cplusplus */


/*==============================================================================
                              Err msg utils for C++
==============================================================================*/
#ifdef __cplusplus

#define DS_APPSRV_MSG_ERR_FLUSH(appsrv_msg, fcn_name) DS_APPSRV_MSG_ERR_PRINT(appsrv_msg, fcn_name)



class AppsrvErrMsg
{
public:
  AppsrvErrMsg();
  ~AppsrvErrMsg();

  void init();

  const char* get_msg() const;
  void set_msg(const char* fmtstr, ...);


public:
  char* output_msg;
};



#endif /* __cplusplus */

#endif /* DS_APPSRV_MSG_H */
