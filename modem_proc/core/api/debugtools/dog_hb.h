#ifndef DOG_HB_H
#define DOG_HB_H
/**
@file dog_hb.h
@brief WatchDog heartbeat monitoring
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
#include "tms_dll_api.h"

#if defined(__cplusplus)
extern "C"
{
#endif

/** @addtogroup dog_hb_api */ /** @{ */

/**
API, Typedefs and Enumerations
*/

typedef unsigned int dog_report_type;

typedef enum
{
   DOG_HB_SIG_REXSIGNAL = 1,              /* Argument Supplied for REX API */
   DOG_HB_SIG_QURTSIGNAL = 2              /* Argument Supplied for QURT ANYSIGNAL API */

} DOG_HB_SIG;

/**
API, Registers clients for watchdog hb monitoring
Reports to the watchdog whether the monitored task is functioning properly
@param[in] task_name : Client task name
@param[in] sig_t     : Client supplied signal type information. Qurt or Rex signal type
@param[in] sig_p     : Client supplied signal information. Qurt or Rex signal
@param[in] sig_m     : Client supplied mask information
@dependencies 
none
@return
dog_report_type : Dog heartbeat handle returned back and should be greater than 0
*/
DLL_API_WARN_UNUSED_RESULT
dog_report_type dog_hb_register(char const* task_name, DOG_HB_SIG sig_t, void* sig_p, unsigned int sig_m);

/**
API, API for clients to report to watchdog heartbeat 
Reports to the watchdog whether the monitored task is functioning properly
@param[in] ident : Dog handle used to tell the watchdog which task is reporting
@dependencies 
none
@return
none
*/
void dog_hb_report(dog_report_type ident);

/**
API, Dynamically de-registers tasks for watchdog heartbeat services
@param[in] ident : Handle that the watchdog uses to identify the task
@dependencies 
none
@return
none
*/
void dog_hb_deregister(dog_report_type ident);

/** @} end_addtogroup dog_hb_api */

#if defined(__cplusplus)
}
#endif

#endif
