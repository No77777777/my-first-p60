#ifndef RCINIT_WORKLOOP_H
#define RCINIT_WORKLOOP_H
/** vi: tw=128 ts=3 sw=3 et :
@file rcinit_init.c
@brief This file contains the API for the Run Control Framework, API 3.0
*/
/*=============================================================================
NOTE: The @brief description above does not appear in the PDF.
The tms_mainpage.dox file contains the group/module descriptions that
are displayed in the output PDF generated using Doxygen and LaTeX. To
edit or update any of the group/module text in the PDF, edit the
tms_mainpage.dox file or contact Tech Pubs.
===============================================================================*/
/*=============================================================================
Copyright (c) 2015-2016 Qualcomm Technologies, Inc.
All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.
=============================================================================*/
/*=============================================================================
Edit History
$Header: //components/rel/core.mpss/10.0/api/debugtools/rcinit_workloop.h#1 $
$DateTime: 2019/04/24 00:03:26 $
$Change: 18983508 $
$Author: pwbldsvc $
===============================================================================*/

#include "rcevt.h"
#include "rcecb.h"
#include "rcesn.h"
#include "qurt.h"

typedef enum
{
   INIT_PROCESS_TABLE,
   TERM_PROCESS_TABLE
} rcinit_workloop_method_t;

typedef struct
{
   struct rcinit_internal_s * client_internal;
   RCECB_HANDLE             * rcecb_handle;
   RCESN_HANDLE             * rcesn_handle;
   RCEVT_HANDLE             * rcevt_handle;
   rcinit_workloop_method_t method;
} rcinit_workloop_handle_t;


typedef enum
{
  E_SUCCESS = 0,
  E_FAIL    = 1
} rcinit_err_t;
/**
API, Service Initialization
@param
None
@return
None
*/
rcinit_err_t rcinit_handle_init(rcinit_workloop_handle_t * handle, char * rcecb_str, char * rcesn_str, char * rcevt_str, rcinit_workloop_method_t method, rcinit_autogen_internal_t * autogen_internal);
rcinit_err_t rcinit_workloop_update_rcinit_internal( rcinit_workloop_handle_t * handle );
rcinit_err_t rcinit_handle_deinit(rcinit_workloop_handle_t * handle );

void rcinit_onetime_init(void);

rcinit_err_t rcinit_process_autogen_table( rcinit_workloop_handle_t * handle );


#endif 

