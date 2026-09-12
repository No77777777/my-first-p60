#ifndef RR_CXM_STATE_H
#define RR_CXM_STATE_H
/*============================================================================
  @file rr_cxm_state.h

  @brief This module contains the declarations of types and external functions for CXM L3 state 
  indication feature.

                Copyright (c) 2015 QUALCOMM Technologies, Inc.
                All Rights Reserved.
                Qualcomm Confidential and Proprietary
============================================================================*/
/* $Header: //components/rel/geran.mpss/7.4.0/grr/src/rr_cxm_state.h#1 $ */

/*----------------------------------------------------------------------------
 * Include Files
 *--------------------------------------------------------------------------*/

#include "geran_variation.h"
#include "customer.h"

#ifdef FEATURE_GSM_CXM_L3_STATE

#include "comdef.h"
#include "wcn_coex_mgr.h"

/*----------------------------------------------------------------------------
 * Type Declarations
 *--------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Externalised Function Definitions
 *---------------------------------------------------------------------------*/
extern void rr_cxm_state_send_l3_state_ind(const cxm_tech_l3_state_e, const gas_id_t);

#endif /* FEATURE_GSM_CXM_L3_STATE */
#endif /* RR_CXM_STATE_H */
