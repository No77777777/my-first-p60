#ifndef MSG_DIAG_SERVICE_H
#define MSG_DIAG_SERVICE_H

/*
Copyright (c) 2014 Qualcomm Technologies, Incorporated. 
All Rights Reserved.
Qualcomm Confidential and Proprietary
*/

#if defined(DIAG_USERPD_TEMP)
  /* Temporarily map msg macros to qshrink 2 functions for audio bringup. */
  #include "msg_diag_service_lsm_tmp.h"
#else
  /* This was the original msg_diag_service.h file */
  #include "msg_diag_service_os_tmp.h"
#endif

#endif /* MSG_DIAG_SERVICE_H */
