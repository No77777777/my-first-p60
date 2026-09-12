#ifndef DOG_HB_REX_H
#define DOG_HB_REX_H
/**
@file dog_hb_rex.h
@brief Dog heartbeat monitoring REX APIs
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
#include "core_pragmas.h"
#include "rex.h"
#include "dog_hb.h"

#if defined(__cplusplus)
extern "C"
{
#endif

/** @addtogroup dog_hb_api */ /** @{ */

/**
API, Rex API for clients to register their task for watchdog heartbeat monitoring
@param[in] sig_m : Client supplied signal mask
@deprecated
Deprecated REX API,
Migrate to dog_hb_register_qurt(qurt_anysignal_t* sig_p, unsigned int sig_m)
@dependencies 
none
@return
dog_report_type : Dog heartbeat handle returned back and should be greater than 0
*/
CORE_WARN_DEPRECATED_MSG("Deprecated REX API","Migrate to dog_hb_register_qurt(qurt_anysignal_t* sig_p, unsigned int sig_m)")
DLL_API_WARN_UNUSED_RESULT
dog_report_type static inline dog_hb_register_rex(rex_sigs_type sig_m)
{
   rex_tcb_type* tcb_p;
   char task_name[REX_TASK_NAME_LEN + 1];

   tcb_p = rex_self();
   rex_get_task_name(tcb_p, task_name, sizeof(task_name), NULL);
   return dog_hb_register((char const*)task_name, DOG_HB_SIG_REXSIGNAL, tcb_p, sig_m);

}

/** @} end_addtogroup dog_hb_api */

#if defined(__cplusplus)
}
#endif

#endif
