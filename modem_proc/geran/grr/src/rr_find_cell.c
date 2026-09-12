/*! \file rr_find_cell.c

    \brief "Find Cell" represents the combination of PSCANs + BCCH decodes.
           This module instructs how to proceed with the power-scan, and then what action to take
           next, depending on the results. This could be to perform another power-scan, or to
           perform BCCH decodes on the frequencies of the power-scan results.

    The Find Cell search algorithm is as follows:

    1. Search over cells in the ACQ-DB
       a. PSCAN
       b. BCCH decodes in order of RXLEV, giving priority to the band of the last-camped cell
          - There are multiple BCCH decode requests to GL1, one per band, in DR mode
    2. Search over all bands 
       a. PSCAN
       b. BCCH decodes in order of RXLEV, giving priority to the band of the last-camped cell
          - There are multiple BCCH decode requests to GL1, one per band, in DR mode
    3. Retry BCCH decodes on any denied bands in SR mode
    4. Exit if no valid BCCHs found (caller will check for low priority / forbidden cells)
    5. BCCH decodes in order of RXLEV for any remaining cells from the previous power-scan
       There are multiple BCCH decode requests to GL1, one per band, in DR mode
    6. Retry BCCH decodes on any denied bands in SR mode
    7. Exit if no valid BCCHs found (caller will check for low priority / forbidden cells)
*/

/*=================================================================================================
                          Copyright (c) 2015 Qualcomm Technologies, Inc.
                          All Rights Reserved.
                          Qualcomm Confidential and Proprietary
=================================================================================================*/
/* $Header: //components/rel/geran.mpss/7.4.0/grr/src/rr_find_cell.c#1 $ */
/* $DateTime: 2019/06/06 23:27:31 $$Author: mplcsds1 $ */

/*-------------------------------------------------------------------------------------------------
 * Include Files
 * ----------------------------------------------------------------------------------------------*/

#include "geran_variation.h"
#include "rr_find_cell.h"
#include "sys_type.h"
#include "rr_nv.h"
#include "rr_cell_selection.h"
#include "rr_general.h"
#include "rr_acq_db.h"
#include "rr_gprs_debug.h"
#include "rr_multi_sim.h"

/*-------------------------------------------------------------------------------------------------
 * State Machine Definition
 * ----------------------------------------------------------------------------------------------*/

/**
 * This state machine does not track a power-scan, then BCCH decodes, then SI acquisition, etc. The purpose of
 * this state machine is to track whether a suitable cell is found by the combination of a power-scan over a 
 * certain set of frequencies and the decoding of BCCH on the frequencies returned from that power-scan. If 
 * that combination fails to find a suitable cell, then this module will try another combination, until there 
 * is no more to try. 
 * So each of the states below may represent a combination of a power-scan and subsequent BCCH decodes, or may 
 * be just BCCH decodes on a set of frequencies where power-scan results have already been done. 
 */
typedef enum
{
  RR_FC_ST_NULL,
  RR_FC_ST_ACQ_DB,
  RR_FC_ST_ACQ_DB_DENIED_BANDS,
  RR_FC_ST_ALL_BANDS,
  RR_FC_ST_DENIED_BANDS,
  RR_FC_ST_ALL_BANDS_NEXT_SET,
  RR_FC_NUM_OF_STATES
} rr_find_cell_state_e;

/**
 * The state machine has two funtions per state, which are defined in the rr_find_cell_sm_def table, below. 
 * The 'enter function' (enter_fn) is called during the transition to that state, and returns the action to 
 * take (rr_find_cell_action_e). 
 * The 'next_state function' (next_state_fn) function is called to obtain the next SM state. 
 * The main state machine event handler is rr_find_cell_sm().
 */

typedef rr_find_cell_action_e (*rr_find_cell_enter_fn_t)(const gas_id_t);
typedef rr_find_cell_state_e (*rr_find_cell_next_state_fn_t)(const gas_id_t);

static rr_find_cell_action_e rr_fc_sm_acq_db_enter_fn(const gas_id_t gas_id);
static rr_find_cell_action_e rr_fc_sm_all_bands_enter_fn(const gas_id_t gas_id);
static rr_find_cell_action_e rr_fc_sm_denied_bands_enter_fn(const gas_id_t gas_id);
static rr_find_cell_action_e rr_fc_sm_all_bands_next_set_enter_fn(const gas_id_t gas_id);

static rr_find_cell_state_e rr_fc_sm_null_next_state_fn(const gas_id_t gas_id);
static rr_find_cell_state_e rr_fc_sm_acq_db_next_state_fn(const gas_id_t gas_id);
static rr_find_cell_state_e rr_fc_sm_acq_db_denied_bands_next_state_fn(const gas_id_t gas_id);
static rr_find_cell_state_e rr_fc_sm_all_bands_next_state_fn(const gas_id_t gas_id);
static rr_find_cell_state_e rr_fc_sm_denied_bands_next_state_fn(const gas_id_t gas_id);

typedef struct
{
  rr_find_cell_enter_fn_t       enter_fn;
  rr_find_cell_next_state_fn_t  next_state_fn;
} rr_find_cell_sm_def_t;

const rr_find_cell_sm_def_t rr_find_cell_sm_def[RR_FC_NUM_OF_STATES] =
{
  /* State                 | Enter Function                         | Next State Function                     */
  /* ----------------------|----------------------------------------|---------------------------------------- */
  /* NULL                */  {NULL,                                   rr_fc_sm_null_next_state_fn},
  /* ACQ_DB              */  {rr_fc_sm_acq_db_enter_fn,               rr_fc_sm_acq_db_next_state_fn},
  /* ACQ_DB_DENIED_BANDS */  {rr_fc_sm_denied_bands_enter_fn,         rr_fc_sm_acq_db_denied_bands_next_state_fn},
  /* ALL_BANDS           */  {rr_fc_sm_all_bands_enter_fn,            rr_fc_sm_all_bands_next_state_fn},
  /* DENIED_BANDS        */  {rr_fc_sm_denied_bands_enter_fn,         rr_fc_sm_denied_bands_next_state_fn},
  /* ALL_BANDS_NEXT_SET  */  {rr_fc_sm_all_bands_next_set_enter_fn,   rr_fc_sm_all_bands_next_state_fn}
};

/*-------------------------------------------------------------------------------------------------
 * Type Declarations
 *-----------------------------------------------------------------------------------------------*/

typedef struct
{
  rr_find_cell_pscan_params_t          action_params;
} rr_find_cell_pscan_info_t;

typedef struct
{
  sys_band_mask_type                   denied_bands_mask;
  boolean                              search_by_band_allowed;
  rr_find_cell_bcch_decodes_params_t   action_params;
} rr_find_cell_bcch_decodes_info_t;

typedef struct
{
  rr_acq_db_pref_e_t                   acq_db_pref;
  sys_band_mask_type                   band_pref;
  rr_candidate_cell_db_T *             candidate_db_ptr;
} rr_find_cell_search_info_t;

typedef struct
{
  rr_find_cell_state_e                 state;
  rr_find_cell_pscan_info_t            pscan;
  rr_find_cell_bcch_decodes_info_t     bcch_decodes;
  rr_find_cell_search_info_t           search_info;
  sys_band_T                           last_camped_cell_band;
  uint8                                band_priority_row;
  uint8                                band_priority_index;
} rr_find_cell_data_t;

/*-------------------------------------------------------------------------------------------------
 * Static Variable Definitions
 * ----------------------------------------------------------------------------------------------*/

/**
 * When BCCH decodes per-band is enabled, BCCH decodes are done one band at a time. 
 * This table gives the order that the bands will be tried.
 * The line of the table selected depends on the band of the last-camped cell, or if not available, the band 
 * of the strongest cell of the power-scan results. 
 */
static const sys_band_T band_priority_table[4][4] =
{
  /*  priority   */
  /*     900     */  { SYS_BAND_EGSM_900, SYS_BAND_DCS_1800, SYS_BAND_PCS_1900, SYS_BAND_CELL_850 },  /* 900-1800-1900-850 */
  /*    1800     */  { SYS_BAND_DCS_1800, SYS_BAND_EGSM_900, SYS_BAND_PCS_1900, SYS_BAND_CELL_850 },  /* 1800-900-1900-850 */
  /*     850     */  { SYS_BAND_CELL_850, SYS_BAND_PCS_1900, SYS_BAND_DCS_1800, SYS_BAND_EGSM_900 },  /* 850-1900-1800-900 */
  /*    1900     */  { SYS_BAND_PCS_1900, SYS_BAND_CELL_850, SYS_BAND_DCS_1800, SYS_BAND_EGSM_900 }   /* 1900-850-1800-900 */
};

static rr_find_cell_data_t rr_find_cell_data[NUM_GERAN_DATA_SPACES];

/*-------------------------------------------------------------------------------------------------
 * Static Function Declarations and Definitions
 * ----------------------------------------------------------------------------------------------*/

/*!
 * \brief Returns a pointer to the module data.
 *
 * \param gas_id
 *
 * \return rr_ps_access_data_t*
 */
static rr_find_cell_data_t *rr_find_cell_get_data_ptr(const gas_id_t gas_id)
{
  uint8 as_index = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

  return &rr_find_cell_data[as_index];
}

/*!
 * \brief Returns a text-string representing the given state.
 * 
 * \param state (in)
 * 
 * \return const char* - 
 */
static const char* rr_find_cell_get_state_name(rr_find_cell_state_e state)
{
  switch (state)
  {
    case RR_FC_ST_NULL:                   return "NULL";
    case RR_FC_ST_ACQ_DB:                 return "ACQ_DB";
    case RR_FC_ST_ACQ_DB_DENIED_BANDS:    return "ACQ_DB_DENIED_BANDS";
    case RR_FC_ST_ALL_BANDS:              return "ALL_BANDS";
    case RR_FC_ST_DENIED_BANDS:           return "DENIED_BANDS";
    case RR_FC_ST_ALL_BANDS_NEXT_SET:     return "ALL_BANDS_NEXT_SET";
    default:                              return "?";
  }
}

/*!
 * \brief Sanity-check that the band value is valid.
 * 
 * \param band (in)
 * 
 * \return sys_band_T - checked band returned if valid, SYS_BAND_NONE otherwise
 */
static sys_band_T check_band(sys_band_T band)
{
  switch (band)
  {
    case SYS_BAND_CELL_850:
    case SYS_BAND_PGSM_900:
    case SYS_BAND_EGSM_900:
    case SYS_BAND_DCS_1800:
    case SYS_BAND_PCS_1900: return band;
    default               : ;
  }

  return SYS_BAND_NONE;
}
/*!
 * \brief Returns the row of the band weighting table for a given band.
 * 
 * \param band (in)
 * 
 * \return uint8 - index
 */
static uint8 band_to_band_priority_row(sys_band_T band)
{
  switch (band)
  {
    case SYS_BAND_PGSM_900: return 0;
    case SYS_BAND_EGSM_900: return 0;
    case SYS_BAND_DCS_1800: return 1;
    case SYS_BAND_CELL_850: return 2;
    case SYS_BAND_PCS_1900: return 3;
    default               : ;
  }

  return 0;
}

static sys_band_T get_next_band(const gas_id_t gas_id)
{
  sys_band_T band;

  rr_find_cell_data_t *data_ptr = rr_find_cell_get_data_ptr(gas_id);

  ++data_ptr->band_priority_index;

  if (data_ptr->band_priority_index < ARR_SIZE(band_priority_table[0]))
  {
    band = band_priority_table[data_ptr->band_priority_row][data_ptr->band_priority_index];
  }
  else
  {
    band = SYS_BAND_NONE;
    data_ptr->band_priority_index = 0;
  }

  rr_debug_print_sys_band("get_next_band", band);

  return band;
}

static sys_band_T get_first_band(const gas_id_t gas_id)
{
  sys_band_T band;

  rr_find_cell_data_t *data_ptr = rr_find_cell_get_data_ptr(gas_id);

  data_ptr->band_priority_index = 0;

  band = band_priority_table[data_ptr->band_priority_row][0];

  MSG_GERAN_HIGH_2_G("get_first_band() band_priority_row=%d band=%d",data_ptr->band_priority_row, band);

  rr_debug_print_sys_band("get_first_band", band);

  return band;
}

/* --------------------------  State Machine state functions  ---------------------------------- */

/* --------------------------------------  NULL  ----------------------------------------------- */

static rr_find_cell_state_e rr_fc_sm_null_next_state_fn(const gas_id_t gas_id)
{
  rr_find_cell_state_e new_state;

  rr_find_cell_data_t *data_ptr = rr_find_cell_get_data_ptr(gas_id);

  boolean acq_db_search_allowed = (RR_ACQ_DB_PREF_NOT_ALLOWED != data_ptr->search_info.acq_db_pref);

  if (acq_db_search_allowed)
  {
    new_state = RR_FC_ST_ACQ_DB;
  }
  else
  {
    new_state = RR_FC_ST_ALL_BANDS;
  }

  return new_state;
}

/* -------------------------------------  ACQ_DB  ---------------------------------------------- */

static rr_find_cell_action_e rr_fc_sm_acq_db_enter_fn(const gas_id_t gas_id)
{
  rr_find_cell_data_t *data_ptr = rr_find_cell_get_data_ptr(gas_id);

  data_ptr->pscan.action_params.acq_db_search = TRUE;
  data_ptr->pscan.action_params.filter_acq_db_freqs = FALSE;

  data_ptr->pscan.action_params.band_mask = data_ptr->search_info.band_pref;
  data_ptr->bcch_decodes.action_params.band_mask = data_ptr->search_info.band_pref;

  return RR_FIND_CELL_ACTION_PSCAN;
}

static rr_find_cell_state_e rr_fc_sm_acq_db_next_state_fn(const gas_id_t gas_id)
{
  rr_find_cell_state_e new_state;

  rr_find_cell_data_t *data_ptr = rr_find_cell_get_data_ptr(gas_id);

  if (RR_ACQ_DB_PREF_ONLY == data_ptr->search_info.acq_db_pref)
  {
    // Only ACQ-DB scan is allowed, so retry denied bands now (if any), otherwise exit
    if (SYS_BAND_MASK_EMPTY != data_ptr->bcch_decodes.denied_bands_mask)
    {
      // Attempt BCCH decodes over the denied bands
      new_state = RR_FC_ST_ACQ_DB_DENIED_BANDS;
    }
    else
    {
      // No options left so search failed - exit
      new_state = RR_FC_ST_NULL;
    }
  }
  else
  {
    data_ptr->pscan.action_params.filter_acq_db_freqs = TRUE;
    data_ptr->pscan.action_params.all_candidate_db_cells_tried = TRUE;
    data_ptr->bcch_decodes.action_params.all_candidate_db_cells_tried = TRUE;

    // Do a search over all bands
    new_state = RR_FC_ST_ALL_BANDS;
  }

  return new_state;
}

/* -------------------------------  ACQ_DB_DENIED_BANDS  --------------------------------------- */

// Note: rr_fc_sm_denied_bands_enter_fn() is re-used for the enter function

static rr_find_cell_state_e rr_fc_sm_acq_db_denied_bands_next_state_fn(const gas_id_t gas_id)
{
  rr_find_cell_state_e new_state;

  rr_find_cell_data_t *data_ptr = rr_find_cell_get_data_ptr(gas_id);

  if (RR_ACQ_DB_PREF_ONLY == data_ptr->search_info.acq_db_pref)
  {
    // No options left so search failed - exit
    new_state = RR_FC_ST_NULL;
  }
  else
  {
    data_ptr->pscan.action_params.filter_acq_db_freqs = TRUE;
    data_ptr->pscan.action_params.all_candidate_db_cells_tried = TRUE;
    data_ptr->bcch_decodes.action_params.all_candidate_db_cells_tried = FALSE;

    // Do a search over all bands
    new_state = RR_FC_ST_ALL_BANDS;
  }

  return new_state;
}

/* ------------------------------------  ALL_BANDS  -------------------------------------------- */

static rr_find_cell_action_e rr_fc_sm_all_bands_enter_fn(const gas_id_t gas_id)
{
  rr_find_cell_data_t *data_ptr = rr_find_cell_get_data_ptr(gas_id);

  data_ptr->pscan.action_params.acq_db_search = FALSE;
  data_ptr->pscan.action_params.band_mask = data_ptr->search_info.band_pref;
  data_ptr->bcch_decodes.action_params.band_mask = data_ptr->search_info.band_pref;

  return RR_FIND_CELL_ACTION_PSCAN;
}

static rr_find_cell_state_e rr_fc_sm_all_bands_next_state_fn(const gas_id_t gas_id)
{
  rr_find_cell_state_e new_state;

  rr_find_cell_data_t *data_ptr = rr_find_cell_get_data_ptr(gas_id);

  if (SYS_BAND_MASK_EMPTY != data_ptr->bcch_decodes.denied_bands_mask)
  {
    // Attempt BCCH decodes over the denied bands
    new_state = RR_FC_ST_DENIED_BANDS;
  }
  else
  {
    boolean any_valid_cell_found;

    any_valid_cell_found = rr_candidate_db_search_for_BCCH(data_ptr->search_info.candidate_db_ptr);

#ifdef FEATURE_GSM_DECODE_ALL_FREQS

    if(rr_nv_decode_all_bcch_freqs_enabled(RR_GAS_ID_TO_AS_ID))
    {
      rr_pscan_results_db_T  *pscan_results_db_ptr = rr_get_pscan_results_db_ptr(gas_id);
      if ( rr_bcch_decode_all_freq_required(pscan_results_db_ptr, gas_id) == TRUE) 
      {
        any_valid_cell_found = FALSE;
      }
    }
#endif //FEATURE_GSM_DECODE_ALL_FREQS 

    if (any_valid_cell_found)
    {
      // Some valid cells were found. The caller is expected to check for low priority cells.
      new_state = RR_FC_ST_NULL;
    }
    else
    {
      // If there are any remaining power-scan frequencies not tried, then decode BCCHs of those

      // This will populate the candidate DB with the next set to try
      uint16 num_of_freqs = rr_pscan_process_power_scan_results(gas_id);

      if (num_of_freqs > 0)
      {
        // Try BCCH decodes on the next set of frequencies
        new_state = RR_FC_ST_ALL_BANDS_NEXT_SET;
      }
      else
      {
        // There are no more frequencies to try
        new_state = RR_FC_ST_NULL;
#ifdef FEATURE_GSM_DECODE_ALL_FREQS
        //Add Code copy pend db to Candidate db. 
        rr_copy_pend_db_to_candidate_db(gas_id);
        rr_candidate_db_clear(rr_get_candidate_cell_pend_db_ptr(gas_id));
#endif //FEATURE_GSM_DECODE_ALL_FREQS
      }
    }
  }

  return new_state;
}

/* ------------------------------------  DENIED_BANDS  ----------------------------------------- */

static rr_find_cell_action_e rr_fc_sm_denied_bands_enter_fn(const gas_id_t gas_id)
{
  rr_find_cell_data_t *data_ptr = rr_find_cell_get_data_ptr(gas_id);

  // Reset all the 'DENIED_BAND' candidates back to 'UNKNWON'
  rr_candidate_reset_denied_bands(data_ptr->search_info.candidate_db_ptr);

  // Set the bands for the BCCH decodes equal to the denied bands
  data_ptr->bcch_decodes.action_params.band_mask = data_ptr->bcch_decodes.denied_bands_mask;

  data_ptr->bcch_decodes.action_params.high_priority = TRUE;

  return RR_FIND_CELL_ACTION_BCCH_DECODES;
}

static rr_find_cell_state_e rr_fc_sm_denied_bands_next_state_fn(const gas_id_t gas_id)
{
  rr_find_cell_state_e new_state;
  boolean any_valid_cell_found;

  rr_find_cell_data_t *data_ptr = rr_find_cell_get_data_ptr(gas_id);

  // Denied bands have been tried now - clear in case another set are tried in DR mode
  data_ptr->bcch_decodes.denied_bands_mask = SYS_BAND_MASK_EMPTY;

  any_valid_cell_found = rr_candidate_db_search_for_BCCH(data_ptr->search_info.candidate_db_ptr);

  if (any_valid_cell_found)
  {
    // Some valid cells were found. The caller is expected to check for low priority cells.
    new_state = RR_FC_ST_NULL;
  }
  else
  {
    // If there are any remaining power-scan frequencies not tried, then decode BCCHs of those

    // This will populate the candidate DB with the next set to try
    uint16 num_of_freqs = rr_pscan_process_power_scan_results(gas_id);

    if (num_of_freqs > 0)
    {
      // Try BCCH decodes on the next set of frequencies
      new_state = RR_FC_ST_ALL_BANDS_NEXT_SET;
    }
    else
    {
      // There are no more frequencies to try
      new_state = RR_FC_ST_NULL;
    }
  }

  return new_state;
}

/* --------------------------------  ALL_BANDS_NEXT_SET  --------------------------------------- */

static rr_find_cell_action_e rr_fc_sm_all_bands_next_set_enter_fn(const gas_id_t gas_id)
{
  rr_find_cell_data_t *data_ptr = rr_find_cell_get_data_ptr(gas_id);

  data_ptr->bcch_decodes.action_params.band_mask = data_ptr->search_info.band_pref;

  return RR_FIND_CELL_ACTION_BCCH_DECODES;
}

// Note: rr_fc_sm_all_bands_next_state_fn() is re-used for the next-state function

/* ----------------------------  State Machine function  --------------------------------------- */

/*!
 * \brief The Find Cell state machine function. When this is called, it advances the state machine
 *        and returns the next action required.
 * 
 * \param gas_id (in)
 * 
 * \return rr_find_cell_action_e - next action
 */
static rr_find_cell_action_e rr_find_cell_sm(const gas_id_t gas_id)
{
  rr_find_cell_data_t *data_ptr = rr_find_cell_get_data_ptr(gas_id);

  // Default action is NONE
  rr_find_cell_action_e action = RR_FIND_CELL_ACTION_NONE;

  // Get the current SM state
  rr_find_cell_state_e state = data_ptr->state;

  // Check the relevant next state function is defined
  if (rr_find_cell_sm_def[state].next_state_fn != NULL)
  {
    // Call the relevant next state function
    rr_find_cell_state_e new_state = (*rr_find_cell_sm_def[state].next_state_fn)(gas_id);

    /**
     * Note: In this SM implementation, it is expected that every time the state machine is advanced, a new 
     * state is given and the enter function for that state is called. However, it is valid that the same 
     * state is returned - this allows a loop through that same state functionality again. This means that the
     * state enter function will be called again, even though the state hasn't changed. 
     */

    // Check the relevant state entry function is defined
    if (rr_find_cell_sm_def[new_state].enter_fn != NULL)
    {
      // Call the relevant state entry function
      action = (*rr_find_cell_sm_def[new_state].enter_fn)(gas_id);
    }

    RR_LOG_STATE_TRANSITION(
      "rr_find_cell_sm",
      state,
      new_state,
      rr_find_cell_get_state_name,
      gas_id
    );

    // Update the SM state
    data_ptr->state = new_state;
  }

  return action;
}

/*!
 * \brief Reset function for the state machine. Calls the NULL enter function (if there is one).
 * 
 * \param gas_id (in)
 */
static void rr_find_cell_sm_reset(const gas_id_t gas_id)
{
  // Check if there is a NULL state entry function defined
  if (rr_find_cell_sm_def[RR_FC_ST_NULL].enter_fn != NULL)
  {
    // Call the NULL state entry function
    (void) (*rr_find_cell_sm_def[RR_FC_ST_NULL].enter_fn)(gas_id);
  }

  rr_find_cell_get_data_ptr(gas_id)->state = RR_FC_ST_NULL;

  return;
}

/*!
 * \brief Indicates if the current search is base on the ACQ-DB or not.
 * 
 * \param gas_id (in)
 * 
 * \return boolean - TRUE is ACQ-DB search, FALSE otherwise
 */
static boolean rr_find_cell_search_is_acq_db(const gas_id_t gas_id)
{
  rr_find_cell_data_t *data_ptr = rr_find_cell_get_data_ptr(gas_id);

  if ((RR_FC_ST_ACQ_DB == data_ptr->state) ||
      (RR_FC_ST_ACQ_DB_DENIED_BANDS == data_ptr->state))
  {
    return TRUE;
  }

  return FALSE;
}

/*-------------------------------------------------------------------------------------------------
* Externalized Function Definitions
* -----------------------------------------------------------------------------------------------*/

/*!
 * \brief Called to indicate a new search is to be done. Returns the pscan parameters to use.
 * 
 * \param acq_db_pref (in)
 * \param band_pref (in)
 * \param candidate_db_ptr (in)
 * \param gas_id (in)
 * 
 * \return rr_find_cell_pscan_params_t - parameters for the subsequent pscan
 */
rr_find_cell_pscan_params_t rr_find_cell_new_search(
  rr_acq_db_pref_e_t acq_db_pref,
  sys_band_mask_type band_pref,
  boolean search_by_band_allowed,
  rr_candidate_cell_db_T *candidate_db_ptr,
  const gas_id_t gas_id
)
{
  rr_find_cell_data_t *data_ptr = rr_find_cell_get_data_ptr(gas_id);

  // Reset the state machine ready for a new search
  rr_find_cell_sm_reset(gas_id);

  MSG_GERAN_HIGH_2_G("New search: acq_db_pref=%d search_by_band_allowed=%d",
                     (int)acq_db_pref, (int)search_by_band_allowed);

  rr_debug_print_sys_band_mask("search_band_pref", band_pref);

  data_ptr->search_info.acq_db_pref = acq_db_pref;
  data_ptr->search_info.band_pref = band_pref;
  data_ptr->search_info.candidate_db_ptr = candidate_db_ptr;

  data_ptr->last_camped_cell_band = SYS_BAND_NONE;
  data_ptr->bcch_decodes.denied_bands_mask = SYS_BAND_MASK_EMPTY;
  data_ptr->bcch_decodes.search_by_band_allowed = FALSE;

  data_ptr->pscan.action_params.all_candidate_db_cells_tried = FALSE;
  data_ptr->bcch_decodes.action_params.all_candidate_db_cells_tried = FALSE;
  data_ptr->bcch_decodes.action_params.high_priority = FALSE;

  if (search_by_band_allowed && rr_nv_bcch_decodes_per_band_is_enabled(RR_GAS_ID_TO_AS_ID) && (rr_is_csfb_call_in_progress(gas_id) != TRUE))
  {
    MSG_GERAN_HIGH_0_G("Search-by-band allowed for this procedure");

#ifdef FEATURE_GSM_DECODE_ALL_FREQS
    data_ptr->last_camped_cell_band = check_band(rr_nv_get_last_camped_cell_band(gas_id));
    data_ptr->bcch_decodes.search_by_band_allowed = TRUE;
#else
    if (rr_ms_drdsds_enabled(gas_id))
    {
      MSG_GERAN_HIGH_0_G("and DR-DSDS is enabled");

      data_ptr->last_camped_cell_band = check_band(rr_nv_get_last_camped_cell_band(gas_id));
      data_ptr->bcch_decodes.search_by_band_allowed = TRUE;
    }
    else
    {
      MSG_GERAN_HIGH_0_G("but DR-DSDS is not enabled");
    }
#endif //FEATURE_GSM_DECODE_ALL_FREQS
  }
  else
  {
    MSG_GERAN_HIGH_0_G("Search-by-band not allowed for this procedure");
  }

  /**
   * Advance the state machine. 
   * This will return RR_FIND_CELL_ACTION_PSCAN and populate pscan.action_params
   */
  (void) rr_find_cell_sm(gas_id);

  return data_ptr->pscan.action_params;
}

/*!
 * \brief Updates the result of the pscan just done. Returns the next action.
 * 
 * \param gas_id (in)
 * 
 * \return rr_find_cell_action_e - next action required
 */
rr_find_cell_action_e rr_find_cell_no_freq_found(const gas_id_t gas_id)
{
  rr_find_cell_data_t *data_ptr = rr_find_cell_get_data_ptr(gas_id);

  data_ptr->pscan.action_params.all_candidate_db_cells_tried = FALSE;

  // Advance the state machine to get the next action
  return rr_find_cell_sm(gas_id);
}

/*!
 * \brief Updates the result of the BCCH decodes just done. Returns the next action.
 * 
 * \param denied_bands (in)
 * \param gas_id (in)
 * 
 * \return rr_find_cell_action_e - 
 */
rr_find_cell_action_e rr_find_cell_no_suitable_cell_found(sys_band_mask_type denied_bands, const gas_id_t gas_id)
{
  rr_find_cell_action_e action = RR_FIND_CELL_ACTION_NONE;

  rr_find_cell_data_t *data_ptr = rr_find_cell_get_data_ptr(gas_id);

  // Setup the next action if RR-FIND-CELL is active
  if (data_ptr->state != RR_FC_ST_NULL)
  {
    sys_band_T next_band = SYS_BAND_NONE;

    data_ptr->bcch_decodes.denied_bands_mask |= denied_bands;

    if (data_ptr->bcch_decodes.action_params.search_by_band)
    {
      // If there are any bands left to try, decode the candidates in the next band
      next_band = get_next_band(gas_id);
    }

    if (SYS_BAND_NONE != next_band)
    {
      data_ptr->bcch_decodes.action_params.band_mask = rr_convert_band_info(next_band);
      data_ptr->bcch_decodes.action_params.all_candidate_db_cells_tried = FALSE;

      action = RR_FIND_CELL_ACTION_BCCH_DECODES;
    }
    else
    {
      // Advance the state machine to get the next action
      action = rr_find_cell_sm(gas_id);
    }
  }

  return action;
}

/*!
 * \brief Called when cell selection has failed.
 * 
 * \param gas_id (in)
 */
void rr_find_cell_acquisition_failure(const gas_id_t gas_id)
{
  rr_find_cell_data_t *data_ptr = rr_find_cell_get_data_ptr(gas_id);

  MSG_GERAN_HIGH_0_G("Acquisition failed");

  data_ptr->pscan.action_params.all_candidate_db_cells_tried = FALSE;
  data_ptr->bcch_decodes.action_params.all_candidate_db_cells_tried = FALSE;

  return;
}

/*!
 * \brief Called when the calling procedure has completed.
 * 
 * \param gas_id (in)
 */
void rr_find_cell_search_complete(const gas_id_t gas_id)
{
  // Reset the state machine
  rr_find_cell_sm_reset(gas_id);

  return;
}

/*!
 * \brief Returns the params for the current pscan.
 * 
 * \param gas_id (in)
 * 
 * \return rr_find_cell_pscan_params_t - pscan params
 */
rr_find_cell_pscan_params_t rr_find_cell_get_pscan_params(const gas_id_t gas_id)
{
  return rr_find_cell_get_data_ptr(gas_id)->pscan.action_params;
}

/*!
 * \brief Returns the params for the current BCCH decodes.
 * 
 * \param gas_id (in)
 * 
 * \return rr_find_cell_bcch_decodes_params_t - BCCH decode params
 */
rr_find_cell_bcch_decodes_params_t rr_find_cell_get_bcch_decodes_params(const gas_id_t gas_id)
{
  return rr_find_cell_get_data_ptr(gas_id)->bcch_decodes.action_params;
}

/*!
 * \brief Indicates if the current acquisition should be high priority or not.
 * 
 * \param gas_id (in)
 * 
 * \return boolean - TRUE if the current acquisition should be high priority
 */
boolean rr_find_cell_acq_high_priority(const gas_id_t gas_id)
{
  return rr_find_cell_get_data_ptr(gas_id)->bcch_decodes.action_params.high_priority;
}

/*!
 * \brief Called when a power-scan has completed to get the parameters to process the results.
 * 
 * \param gas_id (in)
 * 
 * \return rr_find_cell_pscan_results_params_t 
 */
rr_find_cell_pscan_results_params_t rr_find_cell_pscan_complete(const gas_id_t gas_id)
{
  rr_find_cell_pscan_results_params_t pscan_results_params;

  rr_find_cell_data_t *data_ptr = rr_find_cell_get_data_ptr(gas_id);

  // Setup default values

  pscan_results_params.acq_db_search = rr_find_cell_search_is_acq_db(gas_id);
  data_ptr->bcch_decodes.action_params.search_by_band = FALSE;
  data_ptr->bcch_decodes.action_params.high_priority = FALSE;

  // Overwrite defaults as appropriate if RR-FIND-CELL is active
  if (data_ptr->state != RR_FC_ST_NULL)
  {
#ifdef FEATURE_GSM_DECODE_ALL_FREQS
    if (data_ptr->bcch_decodes.search_by_band_allowed)
#else
    if (data_ptr->bcch_decodes.search_by_band_allowed && rr_ms_other_stack_in_traffic(gas_id))
#endif //FEATURE_GSM_DECODE_ALL_FREQS
    {
      if (data_ptr->last_camped_cell_band != SYS_BAND_NONE)
      {
        data_ptr->band_priority_row = band_to_band_priority_row(data_ptr->last_camped_cell_band);
      }
      else
      {
        data_ptr->band_priority_row = band_to_band_priority_row(rr_pscan_get_strongest(gas_id).band);
      }

      data_ptr->bcch_decodes.action_params.band_mask = rr_convert_band_info(get_first_band(gas_id));
      data_ptr->bcch_decodes.action_params.search_by_band = TRUE;
    }
    else
    {
      data_ptr->bcch_decodes.action_params.band_mask = data_ptr->search_info.band_pref;
    }
  }
  else
  {
    data_ptr->bcch_decodes.action_params.band_mask = rr_band_pref(gas_id);
  }

  return pscan_results_params;
}

/*!
 * \brief Initialisation function. Should be called once at RR task start.
 */
void rr_find_cell_task_start_init(void)
{
  // Zero the control/data structure(s)
  memset(rr_find_cell_data, 0, sizeof(rr_find_cell_data));
}

/* EOF */

