#ifndef RR_QSH_EVENT_H
#define RR_QSH_EVENT_H

/*!
 * \file rr_qsh_event.h 
 *  
 * This module contains functionality to interface to QSH via EVENTs. 
 *  
 *              Copyright (c) 2016 Qualcomm Technologies, Inc.
 *              All Rights Reserved.
 *              Qualcomm Confidential and Proprietary
 */
/* $Header: //components/rel/geran.mpss/7.4.0/grr/src/rr_qsh_event.h#1 $ */
/* $DateTime: 2019/06/06 23:27:31 $$Author: mplcsds1 $ */

/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/

#include "geran_variation.h"

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH

#include "qsh.h"
#include "geran_dual_sim.h"

/*----------------------------------------------------------------------------
 * Type Declarations
 * -------------------------------------------------------------------------*/

/*Enums for event notifications*/
typedef enum
{
  RR_QSH_EVENT_RESEL                         = 0,
  RR_QSH_EVENT_ASSIGNMENT_FAILURE            = 1,
  RR_QSH_EVENT_HANDOVER_FAILURE              = 2,
  RR_QSH_EVENT_DTM_ASSIGNMENT_FAILURE        = 3,
  RR_QSH_EVENT_G2W_HO_FAILURE                = 4,
  RR_QSH_EVENT_RESEL_FAILURE                 = 5,
  RR_QSH_EVENT_CON_REL_L2_RESET              = 6,
  RR_QSH_EVENT_PCCO                          = 7,
  RR_QSH_EVENT_RACH_FAILURE                  = 8,
  RR_QSH_EVENT_MPLMN_TIMEOUT                 = 9,
  RR_QSH_EVENT_IA_RECEIVED                   = 10,
  RR_QSH_EVENT_G2X_REDIR                     = 11,
  RR_QSH_EVENT_G2X_RESEL_FAILED              = 12,  
  RR_QSH_EVENT_G2W_HO_STARTED                = 13,  
  RR_QSH_EVENT_GSM_SYS_INFO_TIMER_EXPIRY     = 14,
  RR_QSH_EVENT_GPRS_SYS_INFO_TIMER_EXPIRY    = 15,
  RR_QSH_EVENT_RLF                           = 16, 
  RR_QSH_EVENT_IA_PART1_RCVD                 = 17,
  RR_QSH_EVENT_IA_PART2_RCVD                 = 18,
  RR_QSH_EVENT_PS_SIGNALLING                 = 19,
  RR_QSH_EVENT_PS_DATA                       = 20,
  RR_QSH_EVENT_G2G_HO_STARTED                = 21,
  RR_QSH_EVENT_FIELD_DEBUG_ANY_FAILURE       = 0xFE,
  RR_QSH_EVENT_INVALID  = 0xff
}rr_qsh_event_id_t;

/*----------------------------------------------------------------------------
 * Function Declarations and Documentation
 * -------------------------------------------------------------------------*/

/*!
 * \brief Performs the EVENT CONFIG call-back action
 * 
 * \param cb_params_ptr (in)
 */
__attribute__((section(".uncompressible.text")))
extern void rr_qsh_event_config_perform_cb_action(qsh_client_cb_params_s *cb_params_ptr);

/*!
 * \brief Notifies an event to QSH if the event notification was enabled by QSH
 *
 * \param event_id(in), gas_id (in)
 */
__attribute__((section(".uncompressible.text")))
extern void rr_qsh_event_notify(rr_qsh_event_id_t event_id, const gas_id_t gas_id);

/*!
 * \brief Maps OTA failure message to QSH event and notifies QSH the same
 *
 * \param msg_type(in), gas_id (in)
 */
__attribute__((section(".uncompressible.text")))
extern void rr_qsh_event_notify_from_ota_failure_msg(uint8 msg_type, const gas_id_t gas_id);

/*!
 * \brief Initialisation function called from RR-QSH when event notification functionality is required.
 */
__attribute__((section(".uncompressible.text")))
extern void rr_qsh_event_init(void);

#endif // FEATURE_QSH_EVENT_NOTIFY_TO_QSH

#endif /* #ifndef RR_QSH_EVENT_H */
/* EOF */

