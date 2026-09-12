#ifndef DOG_HB_QURT_H
#define DOG_HB_QURT_H
/**
@file dog_hb_qurt.h
@brief Dog heartbeat monitoring QuRT APIs
*/
/*=============================================================================
NOTE: The @brief description above does not appear in the PDF.
The tms_mainpage.dox file contains the group/module descriptions that
are displayed in the output PDF generated using Doxygen and LaTeX. To
edit or update any of the group/module text in the PDF, edit the
tms_mainpage.dox file or contact Tech Pubs.
===============================================================================*/
/*=============================================================================
Copyright (c) 2015-2018 Qualcomm Technologies, Inc.
All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.
===============================================================================*/

#include "stdint.h"
#include "qurt.h"
#include "dog_hb.h"
#include "tms_dll_api.h"

#if defined(__cplusplus)
extern "C"
{
#endif

/** @addtogroup dog_hb_api */ /** @{ */

/**
API, QuRT API for clients to register their task for watchdog heartbeat monitoring
@param[in] sig_p : Client supplied qurt signal 
@param[in] sig_m : Client supplied signal mask
@dependencies 
none
@return
dog_report_type : Dog heartbeat handle returned back and should be greater than 0
*/
DLL_API_WARN_UNUSED_RESULT
dog_report_type static inline dog_hb_register_qurt(qurt_anysignal_t* sig_p, unsigned int sig_m)
{
   char task_name[QURT_MAX_NAME_LEN];

   qurt_thread_get_name(task_name, sizeof(task_name));

   return(dog_report_type)dog_hb_register((char const*)task_name, DOG_HB_SIG_QURTSIGNAL, sig_p, sig_m);
}

/** @} end_addtogroup dog_hb_api */

#if defined(__cplusplus)
}
#endif

#endif
