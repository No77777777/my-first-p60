/*===========================================================================
                      DS_EDU_MGR_SOCK.H

DESCRIPTION
 socket header file for EDU_MGR.

EXTERNALIZED FUNCTIONS

 Copyright (c) 2014 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  when        who                            what, where, why
--------    -------                ----------------------------------------
10/09/14    Youjunf                          First version of file
===========================================================================*/

#ifndef DS_EDU_MGR_SOCK_H
#define DS_EDU_MGR_SOCK_H

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#include "datamodem_variation.h"
#ifdef FEATURE_DATA_EDU_MGR

#include "ps_sys.h"
#include "dssocket_defs.h"

/*===========================================================================

                           FUNCTION DECLARATIONS

===========================================================================*/
sint15 ds_edu_mgr_sock_create(sint15 nethandle, uint16 *port);
boolean ds_edu_mgr_sock_close(sint15 sock_fd);


boolean ds_edu_mgr_sock_async_select(sint15  sock_fd,
                                     sint31  event_mask);


uint32 ds_edu_mgr_sock_recvfrom(sint15                sock_fd,
                                uint8                 *buf_ptr,
                                uint16                 buf_len,
                                struct ps_sockaddr_in *dest_ptr);

#endif   /*FEATURE_DATA_EDU_MGR*/
#endif /* DS_EDU_MGR_SOCK_H */
