#ifndef mncnm_qsh_h
#define mncnm_qsh_h

/*===========================================================================
                     COPYRIGHT INFORMATION

Copyright (c) 2001 Qualcomm Technologies, Incorporated and its licensors.  All Rights 
Reserved.  QUALCOMM Proprietary.  Export of this technology or software 
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                     EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/mn/vcs/mn_cc.h_v   1.23   10 Jul 2002 13:22:34   cdealy  $   
$Header: //components/rel/mmcp.mpss/7.9.0/api/mncnm_qsh.h#1 $    $DateTime: 2021/06/15 22:40:27 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------

===========================================================================*/
/************************************************************************
*                                                                       *
*                       QSH to MN/CMN PRIMITIVES                        *
*                       -------------------                             *
*                                                                       *
*************************************************************************
*/
#include "queue.h"
#include "nas_exp.h"
#include "mn_qsh_ext.h"
#include "cnm_qsh_ext.h"


#define  MN_QSH_MSG_IND   0    /* MN-QSH message id */
#define  CNM_QSH_MSG_IND  1    /* CNM-QSH message id */

/************************************************************************
*                                                                       *
*                       MNQSH_DEBUG_DUMP_REQ_T                          *
*                                                                       *
*************************************************************************
*/
typedef struct 
{
   IMH_T                      msg_header;
   qsh_client_cb_params_s     param;
}mncnm_qsh_msg_T;

#endif /* mncnm_qsh */
