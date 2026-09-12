#ifndef RR_QSH_H
#define RR_QSH_H

/*!
 * \file rr_qsh.h 
 *  
 * This module contains functionality to interface to QSH. 
 *  
 *              Copyright (c) 2015 Qualcomm Technologies, Inc.
 *              All Rights Reserved.
 *              Qualcomm Confidential and Proprietary
 */
/* $Header: //components/rel/geran.mpss/7.4.0/grr/src/rr_qsh.h#2 $ */
/* $DateTime: 2020/01/27 04:29:28 $$Author: pwbldsvc $ */

/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/

#include "geran_variation.h"

#if defined (FEATURE_QSH_DUMP) || defined (FEATURE_QSH_EVENT_METRIC) || defined(FEATURE_QSH_EVENT_NOTIFY_TO_QSH) || defined(FEATURE_QSH_EVENT_NOTIFY_HANDLER) || defined(FEATURE_QSH_MDUMP)
#include "rr_if.h"

/*----------------------------------------------------------------------------
 * Function Declarations and Documentation
 * -------------------------------------------------------------------------*/
#ifdef FEATURE_QSH_EVENT_METRIC
/*!
 * \brief Process function for RR_IMSG_METRIC_CONFIG_IND
 * 
 * \param msg_ptr (in)
 */
__attribute__((section(".uncompressible.text")))
extern void rr_qsh_process_metric_config_ind(rr_imsg_qsh_metric_config_ind_t *msg_ptr);

/*!
 * \brief Process function for RR_IMSG_METRIC_TIMER_EXPIRY_IND
 * 
 * \param msg_ptr (in)
 */
__attribute__((section(".uncompressible.text")))
extern void rr_qsh_process_metric_timer_expiry_ind(rr_imsg_qsh_metric_timer_expiry_ind_t *msg_ptr);
#endif // FEATURE_QSH_EVENT_METRIC

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
/*!
 * \brief Process function for RR_IMSG_EVENT_CONFIG_IND
 * 
 * \param msg_ptr (in)
 */
__attribute__((section(".uncompressible.text")))
extern void rr_qsh_process_event_config_ind(rr_imsg_qsh_event_config_ind_t *msg_ptr);
#endif // FEATURE_QSH_EVENT_NOTIFY_TO_QSH

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif // FEATURE_QSH_EVENT_NOTIFY_HANDLER

/*!
 * \brief Initialisation function called once at RR task start.
 */
__attribute__((section(".uncompressible.text")))
extern void rr_qsh_task_start_init(void);

#endif /* FEATURE_QSH_DUMP || FEATURE_QSH_EVENT_METRIC || FEATURE_QSH_EVENT_NOTIFY_TO_QSH || FEATURE_QSH_EVENT_NOTIFY_HANDLER || FEATURE_QSH_MDUMP */

#endif /* #ifndef RR_MSC_H */

