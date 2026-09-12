/*============================================================================
  @file rr_cxm_state.c

  @brief This module implements a state machine that manages RR-CXM state indication feature for 
  multi-SIM and associated interface functions.

   MCS-CxM is kept informed of GERAN states like connected, idle, rach etc.

                Copyright (c) 2015 QUALCOMM Technologies, Inc.
                All Rights Reserved.
                Qualcomm Confidential and Proprietary
============================================================================*/
/* $Header: //components/rel/geran.mpss/7.4.0/grr/src/rr_cxm_state.c#1 $ */
/*----------------------------------------------------------------------------
 * Include Files
 *--------------------------------------------------------------------------*/

#include "geran_variation.h"
#include "customer.h"

#ifdef FEATURE_GSM_CXM_L3_STATE

#include "comdef.h"

#include "rr_task.h"
#include "rr_cxm_state.h"
#include "rr_general.h"
#include "rr_defs.h"
#include "rr_gprs_debug.h"
#include "wcn_coex_mgr.h"
#include "rr_nv.h"
#include "rr_msgr.h"
#ifdef PERLUTF
#error code not present
#endif /* PERLUTF */

/*----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 *--------------------------------------------------------------------------*/
#define RR_CXM_STATE_INIT {CXM_TECH_L3_INACTIVE}

/*----------------------------------------------------------------------------
 * Type Declarations
 *--------------------------------------------------------------------------*/

typedef struct
{
  cxm_tech_l3_state_e       state;
} rr_cxm_data_t;

/*----------------------------------------------------------------------------
 * Global Data Definitions
 *--------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Static Variable Definitions
 *--------------------------------------------------------------------------*/
static rr_cxm_data_t rr_cxm_data[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(RR_CXM_STATE_INIT) };

/*----------------------------------------------------------------------------
 * Static Function Declarations and Definitions
 *--------------------------------------------------------------------------*/

/*!
 * \brief Returns a pointer to the module data.
 *
 * \param gas_id
 *
 * \return rr_cxm_state_t*
 */
static rr_cxm_data_t *rr_cxm_state_get_data_ptr(const gas_id_t gas_id)
{
  return(&(rr_cxm_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)]));
}

/*-----------------------------------------------------------------------------
 * Externalised Function Definitions
 *---------------------------------------------------------------------------*/
/*!
 * \brief Sends rr_cxm_state_send_l3_state_ind to MCS-CXM with current RR-CxM state value
 *
 * \param arfcn
 *        gas_id
 *
 * \return void
 */
void rr_cxm_state_send_l3_state_ind(const cxm_tech_l3_state_e state, const gas_id_t gas_id)
{
  rr_cxm_data_t *rr_cxm_data = rr_cxm_state_get_data_ptr(gas_id);

  /* Validate module-global data */
  RR_NULL_CHECK_RETURN_VOID(rr_cxm_data);

#ifdef PERLUTF
  #error code not present
#endif /* PERLUTF */

#ifdef FEATURE_GSM_DTM
  /* Disable the FR if DTM is enabled */
  if (TRUE == rr_get_nv_dtm_enabled(RR_GAS_ID_TO_AS_ID))
  {
    return;
  }
#endif /* FEATURE_GSM_DTM */

  /* Send state indication only if current state is different from last indicated one */
  if (state != rr_cxm_data->state)
  {
    cxm_coex_tech_l3_state_ind_s state_ind;
    errno_enum_type result;

    memset(&state_ind, 0, sizeof(state_ind));

    msgr_init_hdr_variant(
      &state_ind.msg_hdr,
      MSGR_GERAN_GRR,
      MCS_CXM_COEX_TECH_L3_STATE_IND,
      RR_GAS_ID_TO_AS_ID
    );

#ifdef FEATURE_GERAN_MULTIPLE_CLIENTS_FOR_CXM_STATE
    /* TODO: FR24681 support for multiple GERAN client IDs */
    state_ind.tech     = geran_map_gas_id_to_cxm_id(gas_id);
#else
    state_ind.tech     = CXM_TECH_GSM1;
#endif /* FEATURE_GERAN_MULTIPLE_CLIENTS_FOR_CXM_STATE */
    state_ind.l3.mode  = (CXM_TECH_L3_INACTIVE == state) ? CXM_TECH_INACTIVE : CXM_TECH_ACTIVE;
    state_ind.l3.state = state;

    MSG_GERAN_HIGH_3_G("RR-CxM tech %d state %d mode %d",
                       (int)state_ind.tech, (int)state_ind.l3.state, (int)state_ind.l3.mode);

    result = rr_msgr_send(&state_ind.msg_hdr, sizeof(state_ind), gas_id);
    if (result != E_SUCCESS)
    {
      MSG_GERAN_ERROR_1_G("rr_msgr_send failed: %d", result);
    }
    else
    {
      /* remember the last state indication sent */
      rr_cxm_data->state = state;
    }
  }
  else
  {
    MSG_GERAN_MED_1_G("Filtered out RR-CxM state: %d", (uint8)state);
  }

  return;
}
#endif /* FEATURE_GSM_CXM_L3_STATE */
