/*===========================================================================

     IMPLEMENTATION OF GPRS AGC FUNCTIONS

GENERAL DESCRIPTION

These are functions to handle AGC on the GPRS channels (PDTCH/PTCCH/PBCCH/
PCCCH).  They invoke the shared GSM/GPRS AGC functions.


EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS
The following is the desired order in which functions should be called

sys_algo_gprs_agc_cfg
...

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_sys_algo_gprs_agc.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
10/02/16   hd       CR9439080 Do not reset the stored powerl level for PDTCH burst if an invalid metrics info 
                    is received from GFW
29/08/14   cos      CR 716812 - Adding missing changes for imrd framework
15/07/14   cja      CR691580 For large Rx Level always update AGC.
21/05/13   cs       TSTS Updates
03/05/11   pg       CR284720: Use all Rx blocks for AGC in SRB loopback mode
02/12/09   ws       Added FEATURE_GSM_GPRS_AGC_UPDATE for GPRS/EGPRS AGC
                    Improvements
04/26/05   gfr      Handle invalid rx metrics gracefully
04/08/05   gfr      Combined GSM and GPRS channel types
01/28/05   gfr      Fix for using FN instead of time stamp.
07/22/04   gfr      EGPRS support.
09/22/03   gfr      Workaround for Anite boxes not correctly setting P0.
08/13/03   gfr      Pass correct metrics parameter to AGC
02/24/03   gfr      Lint cleanup.
11/19/02   gfr      Changed fifo to single data structure.
11/19/02   gfr      Fixed bug where AGC was performed on 0 valid timeslots.
10/14/02   gfr      Changed process_pdtch interface to be called once per
                    message instead of once per block.
10/10/02   gfr      Do not do any PDTCH processing if this is not the last
                    frame of the message.
09/06/02   gfr      Initial Revision

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
/* definitions */

#include "geran_variation.h"
#ifdef CUST_H
#include "customer.h"
#endif


#include "comdef.h"
#include "gl1_sys_algo_gprs.h"
#include "msg.h"
#include "gtmrs.h"
#include "gtmrs_g.h"  /* if FEATURE_GSM_GPRS_AGC_UPDATE, for GSTMR_GET_FN_GERAN */
#include "gl1_msg_pdch.h"

#include "geran_dual_sim.h"
#include "geran_multi_sim.h"

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/

/* Define this to calculate and use Pr average */
#define AGC_GPRS_USE_PR_AVG

#ifndef AGC_GPRS_USE_PR_AVG
/* If Pr average is not used, use this as an estimate */
#define AGC_GPRS_PR_ESTIMATE (5*16)
#endif

/* Maximum drop in power on PDTCH to allow before assuming P0 is not set
   correctly - used during testing on broken Anite boxes */
#define ANITE_MAX_VALID_PDTCH_PWR_DROP (25*16)

/* If Rx Level greater than threshold (-50dBm), always use for AGC */
#define GL1_GPRS_AGC_ALWAYS_VALID_RXLEV_THRESHOLD ((-50)*16)

/* One frame worth of stored pdtch power data */
typedef struct
{
  dBx16_T pwr_dBm_x16[GL1_DEFS_SLOTS_IN_FRAME];
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
  dBx16_T pwr_dBm_x16_divrx[GL1_DEFS_SLOTS_IN_FRAME];
#endif
  uint32  time_stamp;
  uint8   num_tn;
  uint8   num_tn_valid;
  uint8   num_metrics_valid [GL1_DEFS_SLOTS_IN_FRAME];
  dBx16_T avg_pwr;
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
  dBx16_T avg_pwr_divrx;
#endif
#ifdef AGC_GPRS_USE_PR_AVG
  dBx16_T avg_Pr;
#endif
} sys_algo_agc_stored_pdtch_data_struct;

static sys_algo_agc_stored_pdtch_data_struct gl1_agc_stored_pdtch_data[NUM_GERAN_DATA_SPACES];

static dBx16_T gl1_agc_Pb[NUM_GERAN_DATA_SPACES]  =
                 { INITIAL_VALUE( 0 ) };

static dBx16_T gl1_agc_P0[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( 0 ) };

static dBx16_T gl1_agc_Pr_avg[NUM_GERAN_DATA_SPACES]  =
                 { INITIAL_VALUE( 0 ) };

static boolean gl1_agc_pbcch_exists[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( FALSE ) };

static boolean gl1_agc_pdch_dl_power_control[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( FALSE ) };

/*===========================================================================

FUNCTION sys_algo_gprs_agc_cfg

DESCRIPTION
   Configure AGC parameters

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void sys_algo_gprs_agc_cfg
(
  dBx16_T Pb,
  dBx16_T P0,
  boolean pbcch_exists,
  boolean pdch_dl_power_control,
  gas_id_t gas_id
)
{
  gl1_agc_Pb[gas_id] = Pb;
  gl1_agc_P0[gas_id] = P0;
  gl1_agc_pbcch_exists[gas_id] = pbcch_exists;
  gl1_agc_pdch_dl_power_control[gas_id] = pdch_dl_power_control;

}



/*=============================================================================
FUNCTION sys_algo_agc_gprs_normalize

DESCRIPTION
  Normalize the given RSSI to BCCH power level

DEPENDENCIES
   The message layer must be initialized.

RETURN VALUE
   The normalized rssi value.

SIDE EFFECTS
   None
============================================================================= */
static dBx16_T sys_algo_agc_gprs_normalize
(
  dBx16_T pwr,  /* power in dBmx16 */
  gl1_hw_channel_type  chan_type,
  gas_id_t gas_id
)
{
  dBx16_T pwr_norm = pwr;

  switch (chan_type)
  {
    case GL1_HW_PDTCHD:
      if (gl1_agc_pdch_dl_power_control[gas_id])
      {
        /* Pr will be added later when it is decoded from the message */
        pwr_norm += gl1_agc_P0[gas_id];
      }
      else
      {
        pwr_norm += gl1_agc_Pb[gas_id];
      }
      break;

    case GL1_HW_PTCCHD:
      if (gl1_agc_pbcch_exists[gas_id])
      {
        pwr_norm += gl1_agc_Pb[gas_id];
      }
      break;

    case GL1_HW_PBCCH:
    case GL1_HW_PCCCH:
      pwr_norm += gl1_agc_Pb[gas_id];
      break;

    default:
      MSG_GERAN_ERROR_1_G("Unknown chan type %d for AGC normalize",chan_type);
      break;
  }

  return pwr_norm;
}


/*=============================================================================
FUNCTION sys_algo_agc_gprs_expected_pwr

DESCRIPTION
  Un-normalize the given power to the given channel power level

DEPENDENCIES
   The message layer must be initialized.

RETURN VALUE
   None

SIDE EFFECTS
   None
============================================================================= */
dBx16_T sys_algo_agc_gprs_expected_pwr
(
  dBx16_T pwr_dBm_x16,   /* BCCH normalized power in dBmx16 */
  gl1_hw_channel_type  chan_type,
  gas_id_t gas_id
)
{
  dBx16_T pwr_exp = pwr_dBm_x16;

  switch (chan_type)
  {
    case GL1_HW_PDTCHD:
      if (gl1_agc_pdch_dl_power_control[gas_id])
      {
#ifdef AGC_GPRS_USE_PR_AVG
        pwr_exp -= (gl1_agc_P0[gas_id] + gl1_agc_Pr_avg[gas_id]);
#else
        pwr_exp -= (gl1_agc_P0[gas_id] + AGC_GPRS_PR_ESTIMATE);
#endif
      }
      else
      {
        pwr_exp -= gl1_agc_Pb[gas_id];
      }
      break;

    case GL1_HW_PTCCHD:
      if (gl1_agc_pbcch_exists[gas_id])
      {
        pwr_exp -= gl1_agc_Pb[gas_id];
      }
      break;

    case GL1_HW_PBCCH:
    case GL1_HW_PCCCH:
      pwr_exp -= gl1_agc_Pb[gas_id];
      break;

    default:
      MSG_GERAN_ERROR_1_G("Unknown chan type %d for AGC expected pwr",chan_type);
      break;
  }

  return pwr_exp;
}




/*=============================================================================
FUNCTION sys_algo_agc_process_ptcch_pbcch_pccch

DESCRIPTION
   Process the given power values immediately.  Called for PCCCH/PBCCH/PTCCH

DEPENDENCIES
   The message layer must be initialized.

RETURN VALUE
   None

SIDE EFFECTS
   None
============================================================================= */
static void sys_algo_agc_process_ptcch_pbcch_pccch
(
  sys_algo_agc_T *gprs_agc,
#if defined (FEATURE_GSM_RX_DIVERSITY_DATA)
  sys_algo_agc_T *gprs_agc_divrx, 			
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA */
  gl1_defs_rx_pkt_metrics_type **metrics_info,
  uint8 num_metrics,
  gl1_hw_channel_type  chan_type,
  uint32 time_stamp,
  gas_id_t gas_id
)
{
  uint8 i;
  dBx16_T pwr;
#if defined (FEATURE_GSM_RX_DIVERSITY_DATA)
  dBx16_T pwr_divrx;
#endif /* FEATURE_GSM_RX_DIVERSITY_DATA */
  uint8 num_valid_metrics;

  /* Average over the timeslots */
  pwr = 0;
#if defined (FEATURE_GSM_RX_DIVERSITY_DATA)
  pwr_divrx = 0;
#endif /* FEATURE_GSM_RX_DIVERSITY_DATA */  
  num_valid_metrics = 0;
  for (i = 0; i < num_metrics; i++)
  {
    if (!metrics_info[i]->valid) continue;

    pwr += metrics_info[i]->pwr_dBm_x16;
#if defined (FEATURE_GSM_RX_DIVERSITY_DATA)
   if(gl1_get_MSRD_ctl_nv(gas_id))
   {
     if((metrics_info[i]->pwr_dBm_x16_divrx != 0) && (gl1_get_rxdiv_agc_start(gas_id)))
     {
       pwr_divrx += metrics_info[i]->pwr_dBm_x16_divrx;
     }
     else 
     {
       metrics_info[i]->pwr_dBm_x16_divrx = metrics_info[i]->pwr_dBm_x16;
       pwr_divrx = pwr;
     }
   }
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA*/
    num_valid_metrics++;
  }

  /* If we had no valid metrics, we are done */
  if (num_valid_metrics == 0) return;

  pwr /= num_valid_metrics;

  /* Normalize */
  pwr = sys_algo_agc_gprs_normalize(pwr, chan_type,gas_id);

  /* Update the AGC table for PRX */
  sys_algo_agc_update
   (
     gprs_agc,
     pwr,
     TRUE,
     time_stamp,
     gas_id
   );

#if defined (FEATURE_GSM_RX_DIVERSITY_DATA)
   if(gl1_get_MSRD_ctl_nv(gas_id))
   {
     pwr_divrx /= num_valid_metrics;

     /* Normalize */
     pwr_divrx = sys_algo_agc_gprs_normalize(pwr_divrx, chan_type,gas_id);

     /* Update the AGC table for DRX */
     sys_algo_agc_update
     (
       gprs_agc_divrx,
       pwr_divrx,
       TRUE,
       time_stamp,
       gas_id
     );
   }
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA */

}



/*=============================================================================
FUNCTION sys_algo_agc_process_pdtch

DESCRIPTION
   Applies the appropriate measureds from a previous PDTCH block to the
   AGC algorithm.  Must be called by PL1 for every PDTCH message.
   If downlink power control was off when the PDTCH was received, the
   Pr values must be set to 0.

DEPENDENCIES
   The AGC must be initialized.

RETURN VALUE
   None

SIDE EFFECTS
   None
============================================================================= */
void sys_algo_agc_process_pdtch
(
  sys_algo_agc_T *gprs_agc,  /* gprs agc pointer */
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
  sys_algo_agc_T *gprs_agc_divrx,  /* gprs diversity Rx agc pointer */
#endif
  gl1_defs_tn_type tn,       /* timeslot */
  dBx16_T Pr,                /* Pr values in dBx16 for each timeslot */
  boolean valid,             /* number of timeslots / Pr values */
  gas_id_t gas_id
)
{
  /* Sanity check */
  if (gl1_agc_stored_pdtch_data[gas_id].num_tn == 0)
  {
    MSG_GERAN_ERROR_0_G("No AGC data to process");
    return;
  }

  if (gl1_agc_stored_pdtch_data[gas_id].num_metrics_valid[tn] == 0)
  {
    MSG_GERAN_ERROR_2_G(" No valid metrics! tn %d [%d]",tn,GSTMR_GET_FN_GERAN(gas_id));
    return;
  }

  /* average all valid bursts for this timeslot */
  gl1_agc_stored_pdtch_data[gas_id].pwr_dBm_x16[tn] =
            ( gl1_agc_stored_pdtch_data[gas_id].pwr_dBm_x16[tn] / gl1_agc_stored_pdtch_data[gas_id].num_metrics_valid[tn]);
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
  /* average all valid bursts for this timeslot */
  gl1_agc_stored_pdtch_data[gas_id].pwr_dBm_x16_divrx[tn] =
            ( gl1_agc_stored_pdtch_data[gas_id].pwr_dBm_x16_divrx[tn] / gl1_agc_stored_pdtch_data[gas_id].num_metrics_valid[tn]);
#endif
  /* Force valid to be TRUE if Rx level is large */
  if(gl1_agc_stored_pdtch_data[gas_id].pwr_dBm_x16[tn] > GL1_GPRS_AGC_ALWAYS_VALID_RXLEV_THRESHOLD)
  {
     valid = TRUE;
  }

  /* If this timeslot was for us and had valid metrics, we use the power */
  if ((valid || gl1_srb_get_loopback_state(gas_id)) && gl1_agc_stored_pdtch_data[gas_id].pwr_dBm_x16[tn] != 0)
  {
    /* Average power over each timeslot */
    gl1_agc_stored_pdtch_data[gas_id].avg_pwr += gl1_agc_stored_pdtch_data[gas_id].pwr_dBm_x16[tn] + Pr;
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
    if ( gl1_agc_stored_pdtch_data[gas_id].pwr_dBm_x16_divrx[tn] != 0) 
    {
       /* Average power over each timeslot */
       gl1_agc_stored_pdtch_data[gas_id].avg_pwr_divrx += gl1_agc_stored_pdtch_data[gas_id].pwr_dBm_x16_divrx[tn] + Pr;
    }
    else
    {
       /* Average power over each timeslot, but use PRx data as DRx data is not available */
       gl1_agc_stored_pdtch_data[gas_id].avg_pwr_divrx += gl1_agc_stored_pdtch_data[gas_id].pwr_dBm_x16[tn] + Pr;
    }
#endif
    gl1_agc_stored_pdtch_data[gas_id].num_tn_valid++;
#ifdef AGC_GPRS_USE_PR_AVG
    gl1_agc_stored_pdtch_data[gas_id].avg_Pr += Pr;
#endif
  }

  gl1_agc_stored_pdtch_data[gas_id].num_tn--;

  /* If there are timeslots left to process, or none of the processed
     timeslots was for us, or the data is stale, then we are done */
  if (gl1_agc_stored_pdtch_data[gas_id].num_tn != 0 ||
      gl1_agc_stored_pdtch_data[gas_id].num_tn_valid == 0 ||
      gl1_agc_stored_pdtch_data[gas_id].time_stamp <= gprs_agc->time_stamp)
  {
    return;
  }

  /* Process all the data that was for us and is not stale and AGC on it */

  /* Calculate the average pwr */
  gl1_agc_stored_pdtch_data[gas_id].avg_pwr /= gl1_agc_stored_pdtch_data[gas_id].num_tn_valid;

  /* Update the AGC table */
  sys_algo_agc_update
   (
     gprs_agc,
     gl1_agc_stored_pdtch_data[gas_id].avg_pwr,
     TRUE,
     gl1_agc_stored_pdtch_data[gas_id].time_stamp,
     gas_id
   );
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
  /* Calculate the average pwr */
  gl1_agc_stored_pdtch_data[gas_id].avg_pwr_divrx /= gl1_agc_stored_pdtch_data[gas_id].num_tn_valid;

  /* Update the AGC table */
  sys_algo_agc_update
   (
     gprs_agc_divrx,
     gl1_agc_stored_pdtch_data[gas_id].avg_pwr_divrx,
     TRUE,
     gl1_agc_stored_pdtch_data[gas_id].time_stamp,
     gas_id
   );
#endif

#ifdef AGC_GPRS_USE_PR_AVG
  gl1_agc_stored_pdtch_data[gas_id].avg_Pr /= gl1_agc_stored_pdtch_data[gas_id].num_tn_valid;
  gl1_agc_Pr_avg[gas_id] = gl1_agc_stored_pdtch_data[gas_id].avg_Pr;
#endif

}



/*=============================================================================
FUNCTION sys_algo_gprs_agc_reset

DESCRIPTION
   Reset the GPRS AGC.  Called when aborting DL PDTCH processing to
   keep everything in sync.

DEPENDENCIES
   The AGC must be initialized.

RETURN VALUE
   None

SIDE EFFECTS
   None
============================================================================= */
void sys_algo_gprs_agc_reset (gas_id_t gas_id)
{
  /* Clear out the stored data */
  gl1_agc_stored_pdtch_data[gas_id].num_tn = 0;
  gl1_agc_stored_pdtch_data[gas_id].num_tn_valid = 0;
}




/*=============================================================================
FUNCTION sys_algo_agc_store_pdtch

DESCRIPTION
   Process delayed metrics info.  Save them for use later when we find out
   which timeslots were for us.  Called for PDTCH.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
============================================================================= */
static void sys_algo_agc_store_pdtch
(
  gl1_defs_rx_pkt_metrics_type **metrics_info,
  uint8 num_metrics,
  uint32 time_stamp,
  boolean first_metrics,
gas_id_t gas_id
)
{
  uint8 i;

  /* Changed this to be first metrics FN temp */
  if(first_metrics)
  {
   /* clear out old stored data as this is first metrics of new message */
   memset(&gl1_agc_stored_pdtch_data[gas_id],0,sizeof(gl1_agc_stored_pdtch_data[gas_id]) );
  }

  /* Store the data */
  for (i = 0; i < num_metrics; i++)
  {
    if (metrics_info[i]->valid)
    {
       gl1_agc_stored_pdtch_data[gas_id].pwr_dBm_x16[metrics_info[i]->tn] +=
        sys_algo_agc_gprs_normalize(metrics_info[i]->pwr_dBm_x16, GL1_HW_PDTCHD,gas_id);
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
       if(metrics_info[i]->pwr_dBm_x16_divrx != 0)
       {
          gl1_agc_stored_pdtch_data[gas_id].pwr_dBm_x16_divrx[metrics_info[i]->tn] +=
           sys_algo_agc_gprs_normalize(metrics_info[i]->pwr_dBm_x16_divrx, GL1_HW_PDTCHD,gas_id);
       }
       else
       {
          gl1_agc_stored_pdtch_data[gas_id].pwr_dBm_x16_divrx[metrics_info[i]->tn] +=
           sys_algo_agc_gprs_normalize(metrics_info[i]->pwr_dBm_x16, GL1_HW_PDTCHD,gas_id);
       }
#endif
       /* Increment number of valid metrics for this timeslot */
       gl1_agc_stored_pdtch_data[gas_id].num_metrics_valid[metrics_info[i]->tn]++;
    }
    else
    {
      MSG_GERAN_ERROR_1_G("Invalid metrics info for burst %d, power level not considered for AGC calculation",i);
    }
  }
  gl1_agc_stored_pdtch_data[gas_id].time_stamp = time_stamp;
  gl1_agc_stored_pdtch_data[gas_id].num_tn = num_metrics;
  gl1_agc_stored_pdtch_data[gas_id].num_tn_valid = 0;
  gl1_agc_stored_pdtch_data[gas_id].avg_pwr = 0;
  gl1_agc_stored_pdtch_data[gas_id].avg_Pr = 0;

}


/*=============================================================================
FUNCTION sys_algo_gprs_agc_update

DESCRIPTION
   Process new metrics info for a frame.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
============================================================================= */
void sys_algo_gprs_agc_update
(
  sys_algo_agc_T *gprs_agc,
#if defined (FEATURE_GSM_RX_DIVERSITY_DATA)
  sys_algo_agc_T *gprs_agc_divrx, 			
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA */
  gl1_defs_rx_pkt_metrics_type **metrics_info,
  uint8 num_metrics,
  gl1_hw_channel_type  chan_type,
  uint32 time_stamp,
  boolean msg_complete,
  gas_id_t gas_id
)
{
  switch (chan_type)
  {
    case GL1_HW_PBCCH:
    case GL1_HW_PCCCH:
    case GL1_HW_PTCCHD:
      sys_algo_agc_process_ptcch_pbcch_pccch
       (
         gprs_agc,
#if defined (FEATURE_GSM_RX_DIVERSITY_DATA)
         gprs_agc_divrx, 			
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA*/
         metrics_info,
         num_metrics,
         chan_type,
         time_stamp,
         gas_id
       );
      break;

    case GL1_HW_PDTCHD:
      sys_algo_agc_store_pdtch
       (
         metrics_info,
         num_metrics,
         time_stamp,
         msg_complete,
         gas_id
       );
      break;

    default:
      MSG_GERAN_ERROR_1_G("Invalid chan type %d for AGC metrics",chan_type);
      break;
  }

}




