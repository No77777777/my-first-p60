#ifndef __VS_TIMER_H__
#define __VS_TIMER_H__

/*
  Copyright (C) 2019 QUALCOMM Technologies, Inc.
  All rights reserved.
  Confidential and Proprietary - QUALCOMM Technologies, Inc.

  $Header: //components/rel/avs.mpss/9.0.c51/vsd/utils/inc/protected/vs_timer.h#1 $
  $Author: pwbldsvc $
*/

#include "timer.h"
#include "mmdefs.h"
#include "vs_errcodes.h"
#include "DALSys.h"
#include "msg.h"
#include "mmstd.h"
#include "qurt.h"
#include "qurt_error.h"

typedef void ( *vs_timer_fn_t ) ( void* client_token );

typedef void* vs_timer_t;

 uint32_t vs_timer_create (
  vs_timer_t* ret_timer,
  vs_timer_fn_t timer_cb,
  void* client_token
);

 uint32_t vs_timer_destroy (
  vs_timer_t timer
);

uint32_t vs_timer_start (
  vs_timer_t timer,
  uint64_t time_ns
);

uint32_t vs_timer_stop (
  vs_timer_t timer
);

#endif /* __VS_TIMER_H__ */

