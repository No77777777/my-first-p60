#ifndef RR_GPRS_DSC_H
#define RR_GPRS_DSC_H

/*============================================================================
  @file rr_gprs_dsc.h

  Header file for the RR GPRS Downlink Signalling Counter module.

                Copyright (c) 2002-2015 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Confidential and Proprietary
============================================================================*/
/* $Header: //components/rel/geran.mpss/7.4.0/grr/src/rr_gprs_dsc.h#1 $ */

/*----------------------------------------------------------------------------
 * Include Files
 *--------------------------------------------------------------------------*/

#include "geran_variation.h"
#include "customer.h"
#include "comdef.h"
#include "rr_events.h"
#include "rr_l1.h"
#include "rr_l1_g.h"
#include "geran_multi_sim.h"

/*----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 *--------------------------------------------------------------------------*/
#define RR_DSC_DRX_DURATION_PER_MULTIFRAME 235 /* millisecs */
#define GPRS_DSC_DIVIDEND                   90

/*----------------------------------------------------------------------------
 * Type Declarations
 *--------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Function Declarations and Documentation
 *--------------------------------------------------------------------------*/

/**
  @brief Get the current downlink signalling counter value.

  @return The current downlink signalling counter value.
*/
extern int rr_gprs_return_gprs_dsc_counter_value(const gas_id_t gas_id);

/**
  @brief Apply the specified block quality indication to the downlink
         signalling counter during early camp

  Applies the specified block_quality indication to the current downlink
  signalling counter value.

  @param block_quality   An indication of the block quality, provided by GL1.
  @param l2_channel_type An indication of the channel where decode was attempted.

  @return RR_EV_DSF_DURING_EARLY_CAMP f the downlink signalling counter has reached during early camp
          RR_EV_DSF_DUE_TO_DENIAL_DURING_EARLY_CAMP if denial during early camp
          EV_NO_EVENT otherwise.
*/
extern rr_event_T rr_gprs_check_dsc_in_early_camp(
  const gl1_block_quality_t block_quality,
  const l2_channel_type_T   l2_channel_type,
  const gas_id_t            gas_id);

/**
  @brief Apply the specified block quality indication to the downlink
         signalling counter and determine if reselection is required.

  Applies the specified block_quality indication to the current downlink
  signalling counter value.

  @param block_quality   An indication of the block quality, provided by GL1.
  @param l2_channel_type An indication of the channel where decode was attempted.

  @return EV_DSF_RESELECTION if the downlink signalling counter has reached zero
          RR_EV_START_RESELECTION_POWER_SCAN if RPS is required to resync the scell
          EV_NO_EVENT otherwise.
*/
extern rr_event_T rr_gprs_check_dsc_in_idle(
  const gl1_block_quality_t block_quality,
  const l2_channel_type_T   l2_channel_type,
  const gas_id_t            gas_id);

/**
  @brief Initialise the downlink signalling counter.

  This function will initialise the downlink signalling counter.
*/
extern void rr_gprs_initialise_dsc(const gas_id_t gas_id);
extern int  rr_gprs_get_max_dsc_value(const gas_id_t gas_id);

/**
  @brief Return an indication of whether the downlink signalling counter is
         below it's maximum permitted value.

  @return TRUE if the downlink signalling counter is below it's maximum value,
  FALSE otherwise.
*/
extern boolean rr_gprs_is_dsc_below_max_value(const gas_id_t gas_id);

/**
  @brief This function can be used to get the percentage of current DSC to its maximum value

  @param gas_id

  @return uint8 - 0-100%
*/
extern uint8 rr_gprs_get_dsc_percent(const gas_id_t gas_id);

#endif /* RR_GPRS_DSC_H */

/* EOF */

