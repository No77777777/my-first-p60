/*! \file rr_find_cell.h

    \brief This file is the header file for the RR-FIND-CELL module.
*/

/*============================================================================
                Copyright (c) 2015 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Confidential and Proprietary
============================================================================*/
/* $Header: //components/rel/geran.mpss/7.4.0/grr/src/rr_find_cell.h#1 $ */
/* $DateTime: 2019/06/06 23:27:31 $$Author: mplcsds1 $ */

#ifndef RR_FIND_CELL_H
#define RR_FIND_CELL_H

/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/

#include "geran_variation.h"
#include "rr_l1.h"
#include "mm_rr.h"
#include "sys.h"
#include "geran_dual_sim.h"
#include "rr_defs.h"
#include "rr_candidate_cell_db.h"

/*----------------------------------------------------------------------------
 * Type Declarations
 * -------------------------------------------------------------------------*/

typedef enum
{
  RR_FIND_CELL_ACTION_NONE,          // No further action to take
  RR_FIND_CELL_ACTION_PSCAN,         // Perform power-scan using the power-scan parameters
  RR_FIND_CELL_ACTION_BCCH_DECODES,  // Perform BCCH decodes using the BCCH decodes parameters
  RR_FIND_CELL_ACTION_MAX
} rr_find_cell_action_e;

typedef enum
{
  RR_ACQ_DB_PREF_NOT_ALLOWED,  // ACQ-DB search not allowed
  RR_ACQ_DB_PREF_ALLOWED,      // ACQ-DB search allowed
  RR_ACQ_DB_PREF_ONLY,         // ACQ-DB search ONLY
  RR_ACQ_DB_PREF_MAX
} rr_acq_db_pref_e_t;

typedef struct
{
  boolean                              acq_db_search;
  boolean                              filter_acq_db_freqs;
  sys_band_mask_type                   band_mask;
  boolean                              all_candidate_db_cells_tried;
} rr_find_cell_pscan_params_t;

typedef struct
{
  boolean                              acq_db_search;
} rr_find_cell_pscan_results_params_t;

typedef struct
{
  sys_band_mask_type                   band_mask;
  boolean                              high_priority;
  boolean                              search_by_band;
  boolean                              all_candidate_db_cells_tried;
} rr_find_cell_bcch_decodes_params_t;

/*----------------------------------------------------------------------------
 * Function Declarations and Documentation
 * -------------------------------------------------------------------------*/

extern rr_find_cell_pscan_params_t rr_find_cell_new_search(
  rr_acq_db_pref_e_t acq_db_pref,
  sys_band_mask_type band_pref,
  boolean search_by_band_allowed,
  rr_candidate_cell_db_T *candidate_db_ptr,
  const gas_id_t gas_id
);

extern rr_find_cell_pscan_params_t rr_find_cell_get_pscan_params(const gas_id_t gas_id);

extern rr_find_cell_bcch_decodes_params_t rr_find_cell_get_bcch_decodes_params(const gas_id_t gas_id);

extern rr_find_cell_action_e rr_find_cell_no_freq_found(const gas_id_t gas_id);

/*!
 * \brief Called when a power-scan has completed to get the parameters to process the results.
 * 
 * \param gas_id (in)
 * 
 * \return rr_find_cell_pscan_results_params_t 
 */
extern rr_find_cell_pscan_results_params_t rr_find_cell_pscan_complete(const gas_id_t gas_id);

extern rr_find_cell_action_e rr_find_cell_no_suitable_cell_found(
  sys_band_mask_type denied_bands,
  const gas_id_t gas_id
);

extern void rr_find_cell_acquisition_failure(const gas_id_t gas_id);

extern void rr_find_cell_search_complete(const gas_id_t gas_id);

/*!
 * \brief Indicates if the current acquisition should be high priority or not.
 * 
 * \param gas_id (in)
 * 
 * \return boolean - TRUE if the current acquisition should be high priority
 */
boolean rr_find_cell_acq_high_priority(const gas_id_t gas_id);

/*!
 * \brief Initialisation function. Should be called once at RR task start.
 */
extern void rr_find_cell_task_start_init(void);

#endif /* ifndef RR_FIND_CELL_H */

/* EOF */

