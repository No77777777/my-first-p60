#ifndef RR_RESEL_H
#define RR_RESEL_H

/*============================================================================
  @file rr_resel.h

  Signalling interface for the RR reselection module.

                Copyright (c) 2002, 2008-2015 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Confidential and Proprietary
============================================================================*/
/* $Header: //components/rel/geran.mpss/7.4.0/grr/src/rr_resel.h#2 $ */

/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/

#include "geran_variation.h"
#include "customer.h"
#include "comdef.h"
#include "rr_events.h"
#include "rr_task.h"
#include "geran_multi_sim.h"

/*----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Type Declarations
 * -------------------------------------------------------------------------*/

/* reasons for the packet cell change failure */
typedef enum
{
  /* 44.060 11.2.3 defined PCCF causes */
  FREQ_NOT_IMPLEMENTED  = 0,
  NO_RESPONSE_ON_TARGET = 1,
  ASSIGNMENT_REJECT_ON_TARGET = 2,
  ONGOING_CS_CONNECTION = 3,
#ifdef FEATURE_GPRS_PS_HANDOVER
  PSHO_FAILURE_REASON_OTHERS = 4,
#endif /* FEATURE_GPRS_PS_HANDOVER */
  MS_IN_GMM_STANDBY = 5,
  FORCED_TO_STANDBY = 6,

  /* below are no spec defined values - used internally */
  ANONYMOUS_ACCESS  = 13,
  PCCF_STORED_BSIC = 14,
  USE_PRELOADED_CAUSE = 15
} pccf_cause_t;

typedef enum
{
  RR_RESEL_ALGO_UNDEFINED,
  RR_RESEL_ALGO_RANK_BASED,
  RR_RESEL_ALGO_PRIORITY_BASED,
  RR_RESEL_ALGO_MAX
} rr_resel_algo_e;

/*----------------------------------------------------------------------------
 * Function Declarations and Documentation
 * -------------------------------------------------------------------------*/

/**
  @brief Initialise the RR reselection module.
*/
extern void rr_resel_init(
  const gas_id_t gas_id
);

/**
  @brief Sets the PCCF cause value to indicate ASSIGNMENT REJECT on target.
*/
extern void rr_resel_set_pccf_cause_to_assignment_reject_on_target(
  const gas_id_t gas_id
);

/**
  @brief Sets the PCCF cause value to indicate NO RESPONSE on target.
*/
extern void rr_resel_set_pccf_cause_to_no_response_on_target(
  const gas_id_t gas_id
);

/**
  @brief Check whether a PCCO procedure is ongoing.

  For the purposes of this function, a PCCO is only considered ongoing if
  RR is in the process of camping on the target cell, or if RR has camped
  on the cell and T3174 is still running, i.e. if the first cell access
  attempt has not yet been completed.

  @return TRUE if a PCCO process is ongoing, FALSE otherwise.
*/
extern boolean rr_pcco_in_progress(
  const gas_id_t gas_id
);

/**
  @brief Check whether a PCCO failure procedure is ongoing.

  For the purposes of this function a PCCO failure procedure is ongoing if
  RR is in the process of camping on the old serving cell after a failed
  PCCO or if RR has camped on the old serving cell and is attempting to
  send a PCCF (e.g. T3176 is running).

  @return TRUE if a PCCO failure procedure is ongoing, FALSE otherwise.
*/
extern boolean rr_failed_pcco_in_progress(
  const gas_id_t gas_id
);


#ifdef FEATURE_GPRS_PS_HANDOVER

/**
  @brief Initialise the PCCF message from PS Handover message.

  @param psho: Pointer to the PS Handover command to be used to initialise
  the PCCF message.
*/
extern void rr_gprs_load_pccf_from_psho(
  ps_handover_command_t *psho,
  const gas_id_t gas_id
);

#endif /* FEATURE_GPRS_PS_HANDOVER */

/**
  @brief Send a PCCF message with the specified cause value.

  Sends a PCCF message to the network using the specified cause.  All other
  values must have previously been set up.

  @param cause: The cause value to be included in the PCCF message.
*/
extern void rr_gprs_send_pccf(
  pccf_cause_t cause,
  const gas_id_t gas_id
);

/**
  @brief Determine whether GPRS reselection parameters are available.
*/
extern boolean rr_gprs_parameters_available(
  const gas_id_t gas_id
);


#ifdef FEATURE_WCDMA

/**
  @brief Inform that reselection module that a W2G reselection has just been
         performed.

  This function is used to inform the reselection module that a W2G reselection
  has been performed.  This will result in the RESELECTION_HYSTERESIS timer
  being started.
*/
extern void rr_resel_w2g_reselection_performed(
  const gas_id_t gas_id
);

#endif /* FEATURE_WCDMA */

/**
  @brief
*/
extern void rr_gprs_reset_reselection_list(
  const gas_id_t gas_id
);

/**
  @brief Determine whether the RESELECTION_HYSTERESIS is running.

  This function is used by logging functions.
*/
extern boolean get_recent_reselection_flag_value(
  const gas_id_t gas_id
);


/**
  @brief

  @return
*/
extern boolean rr_gprs_nacc_check_pccn_flag(
  const gas_id_t gas_id
);

/**
  @brief

  @return
*/
extern boolean rr_resel_is_in_ccn_reselection_state(
  const gas_id_t gas_id
);

/**
  @brief

  @param target_type
*/
extern void rr_gprs_nacc_prepare_for_wtog_cco(
  byte target_type,
  const gas_id_t gas_id
);


/**
  @brief Determine whether the RR reselection state machine is able to process
         an RR_RESEL_IMSG_ABORT_REQ or if it should be buffered.

  @return TRUE if the reselection state machine is able to process an
  RR_RESEL_IMSG_ABORT_REQ at this time, FALSE if it should be buffered.
*/
extern boolean rr_resel_can_process_rr_resel_abort_req(
  const gas_id_t gas_id
);

#ifndef FEATURE_GERAN_REDUCED_SPRINTF
/**
  @brief Return a string containing the current reselection state machine
         state.

  @return Pointer to a string.
*/
extern const char *rr_resel_current_state_name(
  const gas_id_t gas_id
);
#else
/**
  @brief Return the current state of reselection state machine.

  @return uint32.
*/
extern uint32 rr_resel_current_state(
  const gas_id_t gas_id
);
#endif /* FEATURE_GERAN_REDUCED_SPRINTF */

/**
  @brief Returns an indication of whether a cell reselection is pending.

  A cell reselection is pending if RR has chosen a cell and is performing
  a surround update before reselecting it.

  @return TRUE if a cell reselection is pending, FALSE otherwise.
*/
extern boolean rr_resel_reselection_is_pending(
  const gas_id_t gas_id
);

/**
  @brief The RR reselection state machine.

  @param event: An informational event or a command instructing the reselection
  state machine to take some action.

  @param message: An optional message sent to the reselection state machine or
  one of it's sub-state machines.  Only valid if 'event' is set to 
  EV_INPUT_MESSAGE.

  @return An event to the parent state machine, or EV_NO_EVENT.
*/
extern rr_event_T rr_resel_control(
  rr_event_T        event,
  rr_cmd_bdy_type * message,
  const gas_id_t    gas_id
);

extern boolean resel_type_C2_based(const gas_id_t    gas_id);

/**
  @brief Initialisation function called at task-start
*/
extern void rr_resel_task_start_init(
  void
);

/**
  @brief Used to inhibit rank based reselection to UTRAN.  If 
         all instances of SI2q have not been received then we
         will inhibit rank based reselection to UTRAN.  This is
         to prevent better cell reselection to UTRAN where
         priority based reselection is available in the NW and
         SI2q indicating this have not been received.
 
*/
boolean rr_inhibit_utran_reselection(const gas_id_t gas_id);

/**
  @brief Used to  Cancel all Priority based reselection timers once it is known that
         Priority based Reselection Algorithm cannot be run  
*/
extern  void rr_resel_cancel_all_priority_based_resel_timers(
  const gas_id_t gas_id
);

/**
  @brief Used to  Cancel all Rank based reselection timers once it is known that
         Rank based Reselection Algorithm cannot be run  
*/
extern void rr_resel_cancel_all_rank_based_resel_timers(
  const gas_id_t gas_id
);

/**
  @brief In case of FAST or SLOW SNR, the 
         snr_reselection_triggered flag is set,and RR_SERV_CELL_BAD_SNR_TIMER timer started 
         
*/
extern void rr_gprs_set_old_scell_snr_resel_status(
  ARFCN_T ARFCN,
  boolean snr_reselection_triggered,
  const gas_id_t gas_id
);

/**
 *@brief Sets the ihibit_pri_resel flag which is used to revert  
 * to rank based reselection when set.
 *  
 * Return - TRUE - inhibit has change, FALSE - inhibit is the 
 * same 
 *
*/
extern boolean rr_set_inhibit_pri_resel(const boolean inhibit, const gas_id_t gas_id);

/**
 *@brief Gets the ihibit_pri_resel flag.
 *  
 * Return - inhibit_pri_resel flag
 *
*/
extern boolean rr_get_inhibit_pri_resel(const gas_id_t gas_id);

/**
  @brief Determine the reselection algorithm that RR should use, based on
         system information.

  @return the reselection algorithm that should be used.
*/
extern rr_resel_algo_e rr_resel_algo(const gas_id_t gas_id);

#ifdef FEATURE_QSH_MDUMP
QSH_MDUMP_FN_ATTR
extern void grr_mdump_rr_resel(const gas_id_t gas_id);
#endif /* FEATURE_QSH_MDUMP */

#endif /* #ifndef RR_RESEL_H */

/* EOF */

