#ifndef SYS_M_MESSAGES_H
#define SYS_M_MESSAGES_H
/*===========================================================================

           SYS_M_MESSAGES . H

DESCRIPTION

Copyright (c) 2011-18 by Qualcomm Technologies, Inc.  All Rights Reserved.

===========================================================================
DESCRIPTION
High level system monitor
===========================================================================

                           EDIT HISTORY FOR FILE

$Header: //components/rel/core.mpss/10.0/api/debugtools/sys_m_messages.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------


===========================================================================*/
#if defined(__cplusplus)
extern "C"
{
#endif

/** @addtogroup sys_m_api @{ */

/**
API, Typedefs and Enumerations
*/
typedef char const* SYS_M_NAME;                                                  /**< Name */

#define SYS_M_NULL ((void*)0)                                                    /**< ISO/ANSI-C, Forward reference NULL pointer */

/**
API, SYS_M_NAME
*/
#define SYS_M_SSR_LPASS_BEFORE_SHUTDOWN      "ssr:lpass:before_shutdown"         /**< SYS_M_NAME */
#define SYS_M_SSR_ADSP_BEFORE_SHUTDOWN       "ssr:adsp:before_shutdown"          /**< SYS_M_NAME */
#define SYS_M_SSR_SLPI_BEFORE_SHUTDOWN       "ssr:slpi:before_shutdown"          /**< SYS_M_NAME */
#define SYS_M_SSR_WCNSS_BEFORE_SHUTDOWN      "ssr:wcnss:before_shutdown"         /**< SYS_M_NAME */
#define SYS_M_SSR_DSPS_BEFORE_SHUTDOWN       "ssr:dsps:before_shutdown"          /**< SYS_M_NAME */
#define SYS_M_SSR_MODEM_BEFORE_SHUTDOWN      "ssr:modem:before_shutdown"         /**< SYS_M_NAME */
#define SYS_M_SSR_GSS_BEFORE_SHUTDOWN        "ssr:gnss:before_shutdown"          /**< SYS_M_NAME */
#define SYS_M_SSR_EXT_MODEM_BEFORE_SHUTDOWN  "ssr:esoc0:before_shutdown"         /**< SYS_M_NAME */
#define SYS_M_SSR_CDSP_BEFORE_SHUTDOWN       "ssr:cdsp:before_shutdown"          /**< SYS_M_NAME */


#define SYS_M_SSR_LPASS_BEFORE_POWERUP       "ssr:lpass:before_powerup"          /**< SYS_M_NAME */
#define SYS_M_SSR_ADSP_BEFORE_POWERUP        "ssr:adsp:before_powerup"           /**< SYS_M_NAME */
#define SYS_M_SSR_SLPI_BEFORE_POWERUP        "ssr:slpi:before_powerup"           /**< SYS_M_NAME */
#define SYS_M_SSR_WCNSS_BEFORE_POWERUP       "ssr:wcnss:before_powerup"          /**< SYS_M_NAME */
#define SYS_M_SSR_DSPS_BEFORE_POWERUP        "ssr:dsps:before_powerup"           /**< SYS_M_NAME */
#define SYS_M_SSR_MODEM_BEFORE_POWERUP       "ssr:modem:before_powerup"          /**< SYS_M_NAME */
#define SYS_M_SSR_GSS_BEFORE_POWERUP         "ssr:gnss:before_powerup"           /**< SYS_M_NAME */
#define SYS_M_SSR_EXT_MODEM_BEFORE_POWERUP   "ssr:esoc0:before_powerup"          /**< SYS_M_NAME */
#define SYS_M_SSR_CDSP_BEFORE_POWERUP        "ssr:cdsp:before_powerup"           /**< SYS_M_NAME */

#define SYS_M_SSR_LPASS_AFTER_POWERUP        "ssr:lpass:after_powerup"           /**< SYS_M_NAME */
#define SYS_M_SSR_ADSP_AFTER_POWERUP         "ssr:adsp:after_powerup"            /**< SYS_M_NAME */
#define SYS_M_SSR_SLPI_AFTER_POWERUP         "ssr:slpi:after_powerup"            /**< SYS_M_NAME */
#define SYS_M_SSR_WCNSS_AFTER_POWERUP        "ssr:wcnss:after_powerup"           /**< SYS_M_NAME */
#define SYS_M_SSR_DSPS_AFTER_POWERUP         "ssr:dsps:after_powerup"            /**< SYS_M_NAME */
#define SYS_M_SSR_MODEM_AFTER_POWERUP        "ssr:modem:after_powerup"           /**< SYS_M_NAME */
#define SYS_M_SSR_GSS_AFTER_POWERUP          "ssr:gnss:after_powerup"            /**< SYS_M_NAME */
#define SYS_M_SSR_EXT_MODEM_AFTER_POWERUP    "ssr:esoc0:after_powerup"           /**< SYS_M_NAME */
#define SYS_M_SSR_CDSP_AFTER_POWERUP         "ssr:cdsp:after_powerup"            /**< SYS_M_NAME */

#define SYS_M_SSR_LPASS_AFTER_SHUTDOWN       "ssr:lpass:after_shutdown"          /**< SYS_M_NAME */
#define SYS_M_SSR_ADSP_AFTER_SHUTDOWN        "ssr:adsp:after_shutdown"           /**< SYS_M_NAME */
#define SYS_M_SSR_SLPI_AFTER_SHUTDOWN        "ssr:slpi:after_shutdown"           /**< SYS_M_NAME */
#define SYS_M_SSR_WCNSS_AFTER_SHUTDOWN       "ssr:wcnss:after_shutdown"          /**< SYS_M_NAME */
#define SYS_M_SSR_DSPS_AFTER_SHUTDOWN        "ssr:dsps:after_shutdown"           /**< SYS_M_NAME */
#define SYS_M_SSR_MODEM_AFTER_SHUTDOWN       "ssr:modem:after_shutdown"          /**< SYS_M_NAME */
#define SYS_M_SSR_GSS_AFTER_SHUTDOWN         "ssr:gnss:after_shutdown"           /**< SYS_M_NAME */
#define SYS_M_SSR_EXT_MODEM_AFTER_SHUTDOWN   "ssr:esoc0:after_shutdown"          /**< SYS_M_NAME */
#define SYS_M_SSR_CDSP_AFTER_SHUTDOWN        "ssr:cdsp:after_shutdown"           /**< SYS_M_NAME */

#define SYS_M_SHUTDOWN_STM_CLEANUP           "sys_m:shutdown_stm_cleanup"

/** @} end_addtogroup sys_m_api */

#if defined(__cplusplus)
}
#endif

#endif  /* SYS_M_MESSAGES_H */
