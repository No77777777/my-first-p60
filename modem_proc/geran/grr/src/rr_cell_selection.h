#ifndef RR_CELL_SELECTION_H
#define RR_CELL_SELECTION_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                               rr_cell_selection.h

GENERAL DESCRIPTION
   This module contains functions for dealing with cell selection.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/grr/src/rr_cell_selection.h#2 $

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "geran_variation.h"
#include "customer.h"
#include "comdef.h"
#include "rr_task.h"
#include "rr_events.h"
#include "sys.h"
#include "geran_multi_sim.h"
#ifdef FEATURE_LOCK_ARFCN
#include "geran_grr_api.h"
#endif /*FEATURE_LOCK_ARFCN*/

/*===========================================================================

                         DATA DECLARATIONS

===========================================================================*/
/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

extern rr_event_T rr_selection_control(
  rr_event_T cell_select_event,
  rr_cmd_bdy_type *new_message,
  const gas_id_t gas_id
);

extern boolean rr_compare_plmn_select_req_info(
  rr_plmn_select_req_T *rr_plmn_select_req,
  const gas_id_t gas_id
);

extern rr_event_T rr_process_plmn_select_req(
  rr_plmn_select_req_T *rr_plmn_select_req,
  boolean *service_domain_changed_ptr,
  const gas_id_t gas_id
);

extern boolean rr_cell_selection_waiting_for_pscan_results(
  const gas_id_t gas_id
);

extern rr_event_T rr_selection_process_select_specific_bcch_cnf(
  mph_select_specific_bcch_cnf_T * mph_select_specific_bcch_cnf,
  const gas_id_t gas_id
);

extern void rr_store_plmn_id(
  PLMN_id_T *plmn_id,
  const gas_id_t gas_id
);

extern void rr_set_equivalent_plmn(
  PLMN_id_T *plmn_id,
  const gas_id_t gas_id
);

extern boolean rr_equivalent_plmn_is_undefined(
  const gas_id_t gas_id
);

extern boolean rr_cell_selection_get_ue_oos_status(
  const gas_id_t gas_id
);

extern void rr_cell_selection_set_ue_oos_status(
  boolean status,
  const gas_id_t gas_id
);

extern boolean rr_sel_is_scan_scope_acq_db(
  rr_PLMN_request_details_T *rr_PLMN_request_details_ptr
);

extern void rr_update_band_preference_for_rrc(
  sys_band_mask_type band_pref_mask,
  const gas_id_t gas_id
);

extern void rr_update_band_preference_for_tdsrrc(
  sys_band_mask_type band_pref_mask,
  const gas_id_t gas_id
);

/**
  @brief Checks whether the cell that RR is currently camped on is suitable
         or not.

  @return TRUE if the currently camped on cell is suitable
  @return FALSE if the currently camped on cell is not suitable
*/
extern boolean rr_cell_selection_is_camped_cell_suitable(
  const gas_id_t gas_id
);

extern byte rr_cell_selection_get_decode_bcch_retry_status(
  const gas_id_t gas_id
);

extern void rr_cell_selection_clear_decode_bcch_retry_status(
  const gas_id_t gas_id
);

/**
  @brief Returns the current service domain
*/
extern sys_srv_domain_e_type rr_sel_get_service_domain(
  const gas_id_t gas_id
);

/**
  @brief Clears cell selection details data structure
*/
extern void rr_clear_cell_selection_details(
  const gas_id_t gas_id
);

/**
  @brief Sets the search reason
*/
extern void rr_sel_set_search_reason(
  rr_search_reason_T search_reason,
  const gas_id_t gas_id
);

/**
  @brief Gets the search reason
 */
extern rr_search_reason_T rr_sel_get_search_reason(const gas_id_t gas_id);

#ifdef FEATURE_LOCK_ARFCN
/**
  @brief set the locked channel
*/
extern void rr_set_locking_channel(ARFCN_T channel_number, const gas_id_t gas_id);

/**
  @brief Modifys the candidate cell db to have only locked channel
*/
extern rr_event_T rr_modify_candidate_cell_db_to_locked_channel(ARFCN_T channel_to_set, const gas_id_t gas_id);

/**
  @brief resets the locking channel info
*/
extern void rr_reset_locking_channel_info(const gas_id_t gas_id);

/**
  @brief finds whether MS is locked or not
*/
extern boolean  rr_is_ms_locked_to_channel(const gas_id_t gas_id);

/**
  @brief registers a power scan complete call back
*/
extern  boolean rr_register_scan_complete_callback(pscan_completion_cb_fn callback_func_ptr, const gas_id_t gas_id);

#endif /*FEATURE_LOCK_ARFCN*/

extern boolean rr_sel_is_search_cause_service_req_mm_full_rat_acq_db_scan(
  rr_PLMN_request_details_T *rr_PLMN_request_details_ptr
);

#ifdef FEATURE_QSH_MDUMP
QSH_MDUMP_FN_ATTR
extern void grr_mdump_rr_cell_selection(const gas_id_t gas_id);
#endif /* FEATURE_QSH_MDUMP */

#endif /* #ifndef RR_CELL_SELECTION_H */

/* EOF */

