/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                            GRR Downlink Signalling Counter Module

GENERAL DESCRIPTION
  This module contains the code to set up and operate the downlink signalling
  counter.

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/grr/src/rr_gprs_dsc.c#1 $
$DateTime: 2019/06/06 23:27:31 $
$Author: mplcsds1 $

when        who     what, where, why
--------    ---     ----------------------------------------------------------
09/09/14    sjw     Revised DSC algorithm for multi-SIM and Dual Receive support
dd/mm/yy    who     Sample text for edit history
===========================================================================*/
#include "geran_variation.h"
#include "customer.h"  /* Contains FEATURE_... defines */
#include "comdef.h"
#include "rr_seg_load.h"
#include "rr_gprs_dsc.h"
#include "rr_general.h"
#include "rr_gprs_defs.h"
#include "rr_log.h"
#include "rr_multi_sim.h"
#include "rr_nv.h"
#include "rr_l1_send.h"
#include "rr_pscan.h"
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif //FEATURE_QSH_EVENT_NOTIFY_HANDLER
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
#include "rr_qsh_event.h"
#endif //FEURE_QSH_EVENT_NOTIFY_TO_QSH

/*===================================================================
            DEFINES
=====================================================================*/
#define RR_DSC_THRESHOLD_POWER_SCAN_VALIDITY (25) /* percentage */

typedef struct
{
  int    gprs_dsc_counter; /* Downlink Signalling Counter */
  uint32 denial_period_ms; /* Time in milliseconds since last good/bad decode */
} rr_gprs_dsc_data_t;

typedef enum
{
  DSC_IN_FULL_IDLE, /*DSC during during idle mode*/
  DSC_IN_EARLY_CAMP, /*DSC in early camp state*/
} rr_dsc_mode_t;

/*===================================================================
            LOCAL DATA
=====================================================================*/
static rr_gprs_dsc_data_t rr_gprs_dsc_data[NUM_GERAN_DATA_SPACES];

/*===================================================================
                      FUNCTION DECLARATIONS
=====================================================================*/

/*!
 * \brief Return a pointer to the module data.
 *
 * \param gas_id
 *
 * \return rr_gprs_dsc_data_t*
 */
static rr_gprs_dsc_data_t *rr_gprs_dsc_get_data_ptr(const gas_id_t gas_id)
{
  uint8 as_index = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

  return &rr_gprs_dsc_data[as_index];
}

/****************************************************************************
 *   Function name:  rr_gprs_get_max_dsc_value
 *   --------------
 *
 *   Description:    Returns upper bound for Downlink Signalling Counter
 *   ------------
 *
 *   Parameters:     NONE
 *   -----------
 *
 *
 *   Returns:        Max DSC value for the current serving cell
 *   --------
 *
 *
 ****************************************************************************/
int rr_gprs_get_max_dsc_value(const gas_id_t gas_id)
{
  gprs_scell_info_t *scell_info_ptr = rr_gprs_get_scell_info(gas_id);
  int                BS_PA_MFRMS = 0;

  if (scell_info_ptr && scell_info_ptr->gsm.valid_data_flag)
  {
    BS_PA_MFRMS = (int)(scell_info_ptr->gsm.control_channel_description.BS_PA_MFRMS);
  }

  if (BS_PA_MFRMS == 0)
  {
    /* default to smallest value (corresponding to fastest paging cycle) */
    BS_PA_MFRMS = BS_PA_MFRMS_MIN;
    MSG_GERAN_HIGH_0_G("DSC: default to BS_PA_MFRMS=2");
  }

  return(GPRS_DSC_DIVIDEND / BS_PA_MFRMS);
}


/****************************************************************************
 *   Function name:  rr_gprs_return_gprs_dsc_counter_value
 *   --------------
 *
 *   Description: This function returns the dsc counter value
 *
 *   Parameters: An event and a possible message
 *   -----------
 *
 *   Returns:  gprs_dsc_counter value
 *   --------
 *
 *
 ****************************************************************************/
int rr_gprs_return_gprs_dsc_counter_value(const gas_id_t gas_id)
{
  // Obtain a pointer to the module data
  rr_gprs_dsc_data_t *rr_gprs_dsc_data_ptr = rr_gprs_dsc_get_data_ptr(gas_id);
  RR_NULL_CHECK_FATAL(rr_gprs_dsc_data_ptr);

  return rr_gprs_dsc_data_ptr->gprs_dsc_counter;
}

#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)

/****************************************************************************
 *   Function name:  rr_gprs_epd_preference
 *   --------------
 *
 *   Description: This function checks whether Enhanced Page Decoding  pref needs to be set as 
 *                     EPD_ON_PREFERRED/EPD_OFF_PREFERRED for Multi-SIM which is 
 *                     Enumerated value of GRR preference for EPD
 *
 *
 *   Parameters: GAS ID
 *   -----------
 *
 *
 *   Returns: RR_L1_EPD_NO_CHANGE if sys info refresh is allowed else
 *                RR_L1_EPD_ON_PREFERRED  if timer is running, else
 *                RR_L1_EPD_OFF_PREFERRED 
 *   --------
 *
 *
 ****************************************************************************/
static rr_l1_epd_preference_t rr_gprs_epd_preference(const gas_id_t gas_id)
{
  if (rr_timer_has_expired(RR_EPD_TIMER, gas_id))
  {
    /* timer has expired at some point in the past and not been cleared yet */
    (void)rr_timer_cancel(RR_EPD_TIMER, gas_id);
     return(RR_L1_EPD_OFF_PREFERRED);
  }

  if (FALSE == rr_timer_is_running(RR_EPD_TIMER, gas_id))
  {
    if (rr_timer_start(RR_EPD_TIMER, RR_EPD_TIMER_DURATION, gas_id))
    {
      MSG_GERAN_HIGH_1_G("EPD timer started for %dms", RR_EPD_TIMER_DURATION);
    }
  }
    /* EPD duty cycle timer is now running */
  return(RR_L1_EPD_ON_PREFERRED);
}

#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */

/****************************************************************************
 *   Function name:  rr_dsc_evaluation
 *   --------------
 *
 *   Description: This function maintains the Downlink Signalling Counter by
 *   ------------ handling the reported block quality which may modify it.
 *
 *
 *   Parameters: Block quality enumeration, channel type and GAS ID
 *   -----------
 *
 *
 *   Returns:  EV_NO_EVENT, EV_DSF_RESELECTION  or RR_EV_START_RPS_AFTER_DENIAL
 *   --------
 *
 ****************************************************************************/
static rr_event_T rr_dsc_evaluation(  gl1_block_quality_t block_quality, const l2_channel_type_T l2_channel_type, const rr_dsc_mode_t dsc_mode, const gas_id_t gas_id)                                     
{
  rr_gprs_dsc_data_t     *rr_gprs_dsc_data_ptr;
  rr_event_T             output_event;
  int                    maximum_value;
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
  boolean                resync_serving_cell = FALSE;
  rr_l1_epd_preference_t epd_pref = RR_L1_EPD_NO_CHANGE; 
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */

  /* Obtain a pointer to the module data */
  rr_gprs_dsc_data_ptr = rr_gprs_dsc_get_data_ptr(gas_id);
  RR_NULL_CHECK_FATAL(rr_gprs_dsc_data_ptr);

  /* Retrieve the maximum value of the counter for the serving cell */
  maximum_value = rr_gprs_get_max_dsc_value(gas_id);

  /* Check that the DSC is not greater than the maximum permitted value */
  /* In other words, ensure that the DSC has been initialised correctly */
  if (rr_gprs_dsc_data_ptr->gprs_dsc_counter > maximum_value)
  {
    MSG_GERAN_HIGH_2_G("DSC %d is greater than max %d, reinitialise now", rr_gprs_dsc_data_ptr->gprs_dsc_counter, maximum_value);
    rr_gprs_dsc_data_ptr->gprs_dsc_counter = maximum_value;
  }
  
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
      #error code not present
#endif //FEATURE_QSH_EVENT_NOTIFY_HANDLER

  /* Now process the incoming block quality value received from Layer 1 */
  switch(block_quality)
  {
    case GL1_BLOCK_QUALITY_GOOD:
    {
      /* increment the DSC by one for each good block */
      rr_gprs_dsc_data_ptr->gprs_dsc_counter = rr_gprs_dsc_data_ptr->gprs_dsc_counter + 1;

      /* cap the DSC at the maximum value for the current cell */
      if (rr_gprs_dsc_data_ptr->gprs_dsc_counter > maximum_value)
      {
        rr_gprs_dsc_data_ptr->gprs_dsc_counter = maximum_value;
      }

#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
      /* on reception of a good block, reset the missed page time stamp */
      rr_ms_clear_missed_page(gas_id);
      epd_pref = rr_gprs_epd_preference(gas_id);
      /* reset the denial duration since a good block means access was granted */
      rr_gprs_dsc_data_ptr->denial_period_ms = 0;
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */
    }
    break;

    case GL1_BLOCK_QUALITY_BAD:
    {
      if (BCCH == l2_channel_type)
      {      

        MSG_GERAN_HIGH_0_G("Decrement TC 0_4_5_read count");      
        rr_decrement_read_count_for_optional_SI2bis_SI2ter_SI13(gas_id);
      }
      /* decrement the DSC by four for each bad block (xCCH failure) */
      rr_gprs_dsc_data_ptr->gprs_dsc_counter = rr_gprs_dsc_data_ptr->gprs_dsc_counter - 4;

      /* floor the value at zero */
      if (rr_gprs_dsc_data_ptr->gprs_dsc_counter < 0)
      {
        rr_gprs_dsc_data_ptr->gprs_dsc_counter = 0;
      }

      /* report the DSC and the denial period */
      MSG_GERAN_HIGH_2_G("DSC-- = %d (denied for %dms)",
                         (int)(rr_gprs_dsc_data_ptr->gprs_dsc_counter),
                         (int)(rr_gprs_dsc_data_ptr->denial_period_ms));


    }
    break;

#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
    case GL1_BLOCK_QUALITY_UNKNOWN:
    {
      /* increment the denial period by the DRX cycle length i.e. (235ms x BS_PA_MFRMS) */
      rr_gprs_dsc_data_ptr->denial_period_ms += RR_DSC_DRX_DURATION_PER_MULTIFRAME * (GPRS_DSC_DIVIDEND / maximum_value);

      /* avoid triggering DSC threshold indications below */
      /* because the real DSC is not changing as a result */
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
     /* check whether the denial period exceeds the value loaded from EFS at power-on */
    if (rr_gprs_dsc_data_ptr->denial_period_ms > rr_nv_get_scell_resync_time_millisecs(RR_GAS_ID_TO_AS_ID))
    {
      /* set flag to generate the appropriate output event below */
      resync_serving_cell = TRUE;
      MSG_GERAN_HIGH_3_G("Resource denied for %dms max denial period %dms, Resync allwd= %d", 
                         (int)(rr_gprs_dsc_data_ptr->denial_period_ms),
                         (int)rr_nv_get_scell_resync_time_millisecs(RR_GAS_ID_TO_AS_ID),
                         resync_serving_cell);
      /* reset the denial duration since a bad block means access was granted */
      rr_gprs_dsc_data_ptr->denial_period_ms = 0;

    }

#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */
      maximum_value = 0;
    }
    break;
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */

   default:
     MSG_GERAN_ERROR_2_G("Unexpected BQ value %d, channel %d", (int)block_quality, (int)l2_channel_type);
     break;
  }

  /* always log the current value of the DSC */
  rr_log_dsc_counter(rr_gprs_dsc_data_ptr->gprs_dsc_counter, gas_id);

  /* compare the current DSC against 0 as documented in the 3GPP spec */
  if (rr_gprs_dsc_data_ptr->gprs_dsc_counter <= 0)
  {
    /* Declare a Downlink Signalling Failure since DSC has reached zero */
    output_event = EV_DSF_RESELECTION;
    MSG_GERAN_HIGH_1_G("DSF - Force Reselection DSC mode =%d",dsc_mode);

  }
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
  else if ((resync_serving_cell) && (DSC_IN_FULL_IDLE == dsc_mode))
  {
    /* trigger a Reselection Power Scan for the serving cell and the BA list */
    /* to determine whether the serving cell continues to be a viable target */
	/* But this needs to be done only during idle mode and not early camping*/
    output_event = RR_EV_START_RPS_AFTER_DENIAL;
    MSG_GERAN_HIGH_0_G("Serving cell resync required, Resource denial detected");
  }
  
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */
  else
  {
    output_event = EV_NO_EVENT;
  }

#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
  if ((RR_EV_START_RPS_AFTER_DENIAL == output_event) ||
      (EV_DSF_RESELECTION           == output_event))
  {

    /* and reset the denial duration */
    rr_gprs_dsc_data_ptr->denial_period_ms = 0;

    if (DSC_IN_FULL_IDLE == dsc_mode)
    {
      /* enable priority inversion for service recovery */
      if (EV_DSF_RESELECTION == output_event)
      {
        rr_ms_service_recovery_started(RR_SERVICE_RECOVERY_DOWNLINK_SIGNALLING, gas_id);
      }
      else
      {
        rr_ms_service_recovery_started(RR_SERVICE_RECOVERY_TRANSCEIVER_DENIALS, gas_id);
      }
    }
  }
#ifndef PERLUTF
  else if ((maximum_value) && (DSC_IN_FULL_IDLE == dsc_mode))
  {
    int percent_dsc;

    /* calculate the percentage of maximum DSC */
    percent_dsc = (rr_gprs_dsc_data_ptr->gprs_dsc_counter * 100) / maximum_value;
    if (percent_dsc <= RR_DSC_THRESHOLD_POWER_SCAN_VALIDITY)
    {
      /* invalidate any power scan results since this sub is close to a DSF */
      /* and it is not desirable to share or reuse the data from this point */
      /* since it may cause us to return to this same cell, and start again */
      rr_invalidate_power_scan_results(gas_id);
    }

    /* report the current percentage to Layer 1 */
    rr_send_mph_dsc_threshold_ind(percent_dsc, l2_channel_type, epd_pref, gas_id);
  }
#endif /* !PERLUTF */
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */

  return(output_event);
}

/****************************************************************************
 *   Function name:  rr_gprs_check_dsc_in_early_camp
 *   --------------
 *
 *   Description This function returns the event triggred through DSC evaluation during 
 *               early camp state .
 *
 *   Parameters: Block quality enumeration, channel type and GAS ID
 *
 *   Returns:    EV_NO_EVENT,RR_EV_DSF_DURING_EARLY_CAMP, 
 *               RR_EV_DSF_DUE_TO_DENIAL_DURING_EARLY_CAMP 
 *
 ****************************************************************************/

rr_event_T rr_gprs_check_dsc_in_early_camp(const gl1_block_quality_t block_quality,const l2_channel_type_T l2_channel_type,const gas_id_t gas_id)
{
  rr_event_T dsc_event;
  
  dsc_event = rr_dsc_evaluation(block_quality, l2_channel_type, DSC_IN_EARLY_CAMP, gas_id);
  // map the output event to one that is for early camp
  if (EV_DSF_RESELECTION == dsc_event)
  {
    dsc_event = RR_EV_DSF_DURING_EARLY_CAMP;
  }
  else if (RR_EV_START_RPS_AFTER_DENIAL == dsc_event)
  {
    dsc_event = RR_EV_DSF_DUE_TO_DENIAL_DURING_EARLY_CAMP;
  }

  return(dsc_event);
}

/****************************************************************************
 *   Function name:  rr_gprs_check_dsc_in_idle
 *   --------------
 *
 *   Description: This function returns the event triggered through DSC evaluation during 
 *              ilde mode  state .
 *
 *   Parameters: Block quality enumeration, channel type and GAS ID
 *   -----------
 **
 *   Returns:  EV_NO_EVENT, EV_DSF_RESELECTION, RR_EV_START_RPS_AFTER_DENIAL 
 *   --------
 *****************************************************************************/

rr_event_T rr_gprs_check_dsc_in_idle(const gl1_block_quality_t block_quality,const l2_channel_type_T l2_channel_type,const gas_id_t gas_id)
{
  return(rr_dsc_evaluation(block_quality, l2_channel_type, DSC_IN_FULL_IDLE, gas_id));
}

/****************************************************************************
 *   Function name:  rr_gprs_initialise_dsc
 *   --------------
 *
 *   Description: This function initialises the Downlink Signalling Counter
 *   ------------
 *
 *   Parameters: NONE
 *   -----------
 *
 *
 *   Returns:  NONE
 *   --------
 *
 *
 ****************************************************************************/
void rr_gprs_initialise_dsc(const gas_id_t gas_id)
{
  gprs_scell_info_t *scell_info = rr_gprs_get_scell_info(gas_id);
  int                BS_PA_MFRMS;
  rr_gprs_dsc_data_t *rr_gprs_dsc_data_ptr;

  // Obtain a pointer to the module data
  rr_gprs_dsc_data_ptr = rr_gprs_dsc_get_data_ptr(gas_id);
  RR_NULL_CHECK_FATAL(rr_gprs_dsc_data_ptr);

  if (scell_info != NULL)
  {
    BS_PA_MFRMS = scell_info->gsm.control_channel_description.BS_PA_MFRMS;
  }
  else
  {
    BS_PA_MFRMS = 0;
  }

  if (BS_PA_MFRMS == 0)
  {
    BS_PA_MFRMS = BS_PA_MFRMS_MIN;
  }

  rr_gprs_dsc_data_ptr->gprs_dsc_counter = GPRS_DSC_DIVIDEND / BS_PA_MFRMS;
  rr_gprs_dsc_data_ptr->denial_period_ms = 0;

#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
  /* reset the missed page time stamp */
  rr_ms_clear_missed_page(gas_id);
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */

  return;
}


/****************************************************************************
 *   Function name:  rr_gprs_is_dsc_below_max_value
 *   --------------
 *
 *   Description: This function can be used to detect if the dsc has fallen
 *   ------------ below its maximum value
 *
 *   Parameters: NONE
 *   -----------
 *
 *
 *   Returns:  TRUE/FALSE
 *   --------
 *
 *
 ****************************************************************************/
boolean rr_gprs_is_dsc_below_max_value(const gas_id_t gas_id)
{
  gprs_scell_info_t *scell_info = rr_gprs_get_scell_info(gas_id);
  int                BS_PA_MFRMS;
  rr_gprs_dsc_data_t *rr_gprs_dsc_data_ptr;

  // Obtain a pointer to the module data
  rr_gprs_dsc_data_ptr = rr_gprs_dsc_get_data_ptr(gas_id);
  RR_NULL_CHECK_FATAL(rr_gprs_dsc_data_ptr);

  if (scell_info != NULL)
  {
    BS_PA_MFRMS = scell_info->gsm.control_channel_description.BS_PA_MFRMS;
  }
  else
  {
    BS_PA_MFRMS = 0;
  }

  if (BS_PA_MFRMS == 0)
  {
    BS_PA_MFRMS = BS_PA_MFRMS_MIN;
  }

  return(rr_gprs_dsc_data_ptr->gprs_dsc_counter < (GPRS_DSC_DIVIDEND / BS_PA_MFRMS));
}

/**
  @brief This function can be used to get the percentage of current DSC to its maximum value

  @param gas_id

  @return uint8 - 0-100%
*/
uint8 rr_gprs_get_dsc_percent(const gas_id_t gas_id)
{
  gprs_scell_info_t *scell_info = rr_gprs_get_scell_info(gas_id);
  uint8              dsc_percent;
  rr_gprs_dsc_data_t *rr_gprs_dsc_data_ptr;
 
  // Obtain a pointer to the module data
  rr_gprs_dsc_data_ptr = rr_gprs_dsc_get_data_ptr(gas_id);
  RR_NULL_CHECK_FATAL(rr_gprs_dsc_data_ptr);

  dsc_percent = (rr_gprs_dsc_data_ptr->gprs_dsc_counter * 100) / rr_gprs_get_max_dsc_value(gas_id);
 
  return dsc_percent;
}

/* EOF */

