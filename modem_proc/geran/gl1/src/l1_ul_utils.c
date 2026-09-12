/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                              l1_ul_utils.c

GENERAL DESCRIPTION
   This module contains utility routines for the uplink

EXTERNALIZED FUNCTIONS
  L1_store_ta_txlev

  L1_change_ta

  L1_control_half_rate

  L1_terminate_dedicated

  L1_condition_rxqual

  L1_send_access_burst

  L1_more_abs_to_send

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_ul_utils.c#1 $
  $DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who       what, where, why
--------   ---       ---------------------------------------------------------
12/10/15   aga       CR921827 KW errors in void L1_precompute_txlev (...)
11/05/15   pjr       CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
13/03/15   mc        CR773788: COEX Enhancements : FACCH delay, RACH delay, PS monitor reduction and DTX timeline changes
29/10/14   pjr       CR745958 Removed remaining obsolete featurisation FEATURE_RACH_PWR_RED
20/06/14   jj        CR656872 fix critical gl1 kw error 
29/11/13   js        CR.574402 : COEX : VBATT : Accurate prediction of N+1 frame Tx power for CS (TCH and SDCCH)
02/10/13   pjr       CR552448 DDR Frequency plan implementation - l1_arbitration modification
08/11/13   cja       CR507514 Keep RACH power reduction until new Tx Lev scheduled
03/10/13   ws        CR499846 - Bug fix for RACH PWR reduction feature
23/05/13   pg        CR460555: Enable 3GPP Rel10 RACH Power Reduction feature 
15/08/13   sk        CR524039 GBTA bringup changes
29/07/13   sk        CR519663 Partial QBTA bringup changes
21/05/13   cs        TSTS Updates
13/03/13   mc        CR 459487 : COEX Support - GERAN arbitrator framework
01/03/12   ws        CR 458470 - Fix referencing of tx_lev resolves erratic txlev
                     power in dedicated mode
21/07/10   ems       Added support for ignoring SACCH power control
08/06/10   dv        GP-031925/CR235566 - For DTM use SACCH TX power if C
                     value not valid.
05/01/10   cs        CR 218984 GCF 13.3.4.1 failure on high bands TX lev wrongly
                     reported
23/01/07   og        Resolve RVCT2.1 compiler warnings, merge from the
                     gpl1_hummer.10.00.43.01 branch.
05/26/04   sv        Lint Cleanup.
11/05/03   gfr       Support for quad-band.
08/15/03   dp        Moved decrementing of number of access bursts outside
                     L1_more_abs_to_send to fix issues with incorrect number
                     of access bursts
06/05/03   dp        Clipped TXLEV at 5 since we're a class 4 mobile
02/25/03   dp        Moved L1_control_half_rate prototype to l1_isr.h
08/29/02   JC        Obsoleted L1_map_txlev_to_power_index() for multi-band
                     support.
08/27/02   plm       Merged m10511.c, m10515.c and m10517.c. Also added return
                     statement for L1_condition_rxqual() and L1_send_access_burst()
                     to eliminate compiler warnings.
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
/* Alphabetical Order */
#include "geran_variation.h"
#include "comdef.h"
#include "gl1_msg_g.h"
#include "l1i.h"
#include "l1_isr.h"
#include "l1_utils.h"

#ifdef FEATURE_GSM_COEX
#include "gl1_arbitrator_interface.h"
#endif /* FEATURE_GSM_COEX */

#include "l1_task.h"
#include "geran_dual_sim.h"
#include "geran_multi_sim.h"

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/
/*
   Default to TCH reporting period.  Will be sufficient for SDCCH too which is
   102
*/
#define MAX_REP_PERIOD 104
static int16 txlev[NUM_GERAN_DATA_SPACES][MAX_REP_PERIOD];

/*
   Indicates the FNMod104 (TCH) or FNMod102 (SDCCH) on which the
   reporting period ends. The txlev[][] array is aligned to the
   reporting window - which in may or may not be aligned to FNMod104.
   This providea the offset that needs to be applied to an absolute 
   FNMod104 before indexing the txlev[][] array.
*/
static uint16 txlev_reporting_period_end_minus_1[NUM_GERAN_DATA_SPACES];
static uint16 txlev_reporting_period[NUM_GERAN_DATA_SPACES]; /* 104 (TCH) or 102 (SDCCH) */

/*
 * This variable is set during GSM FTM non-signaling test, so that
 * the mobile will not respond to the PCL in the SACCH channel
 * TRUE = allow SACCH PCL to be used to set mobile Tx power
 * FALSE = disable SACCH PCL from being used to set mobile Tx power
 */
static boolean sacch_tx_pwr_control[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( TRUE ) };

static byte L1_process_next_txlev( byte power_level, ARFCN_T arfcn );

/*===========================================================================

FUNCTION  L1_COMPUTE_NEXT_TXLEV

DESCRIPTION
  This function checks the demanded tx power level against the power
  level in use. If different, it moves the power level in use 1 step (2dB)
  closer to the demanded level

DEPENDENCIES
  demanded power level
  power level in use

RETURN VALUE
  Power level in use

SIDE EFFECTS
  None

===========================================================================*/
byte L1_compute_next_txlev( byte    demanded_power_level,
                            byte    power_level_in_use,
                            ARFCN_T arfcn )
{
   /* For high bands apply power level translation as per spec */
   demanded_power_level = L1_process_next_txlev( demanded_power_level, arfcn );

   power_level_in_use   = L1_process_next_txlev( power_level_in_use, arfcn );

   if ( power_level_in_use > demanded_power_level )
   {
      power_level_in_use--;
   }

   if ( power_level_in_use < demanded_power_level )
   {
      power_level_in_use++;
   }

   return ( power_level_in_use );
}

/*===========================================================================

FUNCTION  L1_process_next_txlev

DESCRIPTION
  This function checks the tx power level and depending on the band will
  apply the same logic as per the TX level calculation used.

DEPENDENCIES
  None

RETURN VALUE
  Power level to use

SIDE EFFECTS
  None

===========================================================================*/
static byte L1_process_next_txlev( byte    power_level,
                                   ARFCN_T arfcn )
{
  sys_band_T band;

  band = arfcn.band;

  switch ( band )
  {
    case SYS_BAND_EGSM_900:
    case SYS_BAND_PGSM_900:
    case SYS_BAND_CELL_850:
    /* Nothing to do here */
    break;

    /* For DCS/PCS we need to apply the spec TX level calculation */
    case SYS_BAND_DCS_1800:
    case SYS_BAND_PCS_1900:
      if ( power_level > 28 )
      {
        power_level = 0;
      }
      else if ( power_level >= 16 )
      {
        power_level = 15;
      }
    break;

    default:
      MSG_GERAN_ERROR_1( "Unsupported Band Class",
                 0);
    break;
  }

  return ( power_level );
}

/*===========================================================================

FUNCTION  L1_CHANGE_TA

DESCRIPTION
  This function changes the timing advance to the TA ordered by the base station
DEPENDENCIES
  demanded power level
  power evel in use

RETURN VALUE
  Timing advance in use

SIDE EFFECTS
  None

===========================================================================*/
byte L1_change_ta( byte demanded_ta,
                   byte ta_in_use,
                   gas_id_t gas_id
                  )
{
    if ( demanded_ta != ta_in_use )
    {
        gl1_msg_store_ta(demanded_ta, gas_id);
    }

    return ( demanded_ta );
}

/*===========================================================================

FUNCTION  GL1_SET_SACCH_TX_PWR_CONTROL

DESCRIPTION
  This function will set an indication as to use or ignore the
  SACCH tx power control level.

DEPENDENCIES

RETURN VALUE
  gs_status_T GS_SUCCESS

SIDE EFFECTS
  None

===========================================================================*/
gs_status_T gl1_set_sacch_tx_pwr_control(boolean tx_pwr_control, gas_id_t gas_id)
{
    sacch_tx_pwr_control[gas_id] = tx_pwr_control;

    return ( GS_SUCCESS );
}

/*===========================================================================

FUNCTION  SACCH_TX_PWR_CONTROL_IS_ACTIVE

DESCRIPTION
  This function will return an indication as to use or ignore the
  SACCH tx power control level.

DEPENDENCIES

RETURN VALUE
  boolean  TRUE use SACCH  FALSE do not use SACCH

SIDE EFFECTS
  None

===========================================================================*/
boolean sacch_tx_pwr_control_is_active( gas_id_t gas_id )
{
    return ( sacch_tx_pwr_control[gas_id] );
}

/*===========================================================================

FUNCTION  L1_STORE_TA_TXLEV

DESCRIPTION
  This function copies the commanded timing and power levels received
  on the SACCH to be applied at the appropriate time

DEPENDENCIES
  pointer to dedicated data
  p

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_store_ta_txlev ( dedicated_data_T *ded_dtr_ptr, byte new_ta, byte new_txlev,
                         gas_id_t gas_id )
{
    /* Store ordered TA and TXLEV */
    ded_dtr_ptr->ordered_timing_advance = new_ta;

    /* Only allow SACCH updates in normal operation not FTM */
    if ( sacch_tx_pwr_control_is_active(gas_id) )
    {
      ded_dtr_ptr->ordered_ms_power_level = new_txlev;
    }
}

/*===========================================================================

FUNCTION  L1_PRECOMPUTE_TXLEV

DESCRIPTION
  This function precomputes and stores all TXLEVs for an entire reporting period

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_precompute_txlev (int16 ordered_txlev, int16 current_txlev,
                          byte rep_period_length, ARFCN_T arfcn,
                          gas_id_t gas_id)
{
    int i;

    current_txlev = gl1_check_pwr_reduction_update(arfcn, ordered_txlev, current_txlev, gas_id);

    if ( rep_period_length >= MAX_REP_PERIOD) 
    {
      rep_period_length = MAX_REP_PERIOD;
      MSG_GERAN_ERROR_0("txlev buffer over flow avoided; rep_period_length exceed  expected size");
    }
    /* Loop through entire reporting period */
    for (i = 0; i < rep_period_length; i++)
    {
       /* Step up/down txlev if needed only every 13 frames as stated in specs */
       if (i % 13)
       {
           txlev[gas_id][i] = current_txlev;
       }
       else
       {
           txlev[gas_id][i] = L1_compute_next_txlev( (byte)ordered_txlev,
                                             (byte)current_txlev,
                                             arfcn );

           current_txlev = txlev[gas_id][i];
       }
    }
}

/*===========================================================================

FUNCTION  L1_RETRIEVE_TXLEV

DESCRIPTION
  This function retrieves the TXLEVs to be used for a block of
  four bursts given the block start and the burst spacing

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_retrieve_txlev ( uint8 block_start, uint8 burst_spacing, int16 block_txlev[],gas_id_t gas_id)
{
   int i, j;

   for (i = block_start, j = 0; j < 4; i += burst_spacing,j++)
   {
     if( i >= 104)
     {
      MSG_GERAN_ERROR_0("txlev buffer over flow");
      i = 103 ;
     }
       block_txlev[j] = txlev[gas_id][i];
   }
}

/*===========================================================================

FUNCTION  L1_TERMINATE_DEDICATED

DESCRIPTION
  This function stops the TCH running

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_terminate_dedicated( void )

{
  /* Add h/w specific code here */
}

/*===========================================================================

FUNCTION  L1_SEND_ACCESS_BURST

DESCRIPTION
  This function send the access on the SDCCH.

DEPENDENCIES
  None

RETURN VALUE
  finished

SIDE EFFECTS
  None

===========================================================================*/
boolean L1_send_access_burst( void )
{
  /* Add h/w specific code here */
  return ( FALSE );
}


/*===========================================================================

FUNCTION  L1_MORE_ABS_TO_SEND

DESCRIPTION
  This function returns true if there are more ABs to send.

DEPENDENCIES
  None

RETURN VALUE
  finished

SIDE EFFECTS
  None

===========================================================================*/
boolean L1_more_abs_to_send(dedicated_data_T *ded_d_ptr, byte  DCCH_type, gas_id_t gas_id)
{
   if (ded_d_ptr->no_of_ab_to_send == 0xff )
   {
      return ( TRUE );
      /* i.e. go on until a MPH_STOP_HANDOVER_ACCESS_REQ message */
   }
   else if ( ded_d_ptr->no_of_ab_to_send == 0 )
   {
      return ( FALSE );
   }
   /* Send CNF after last access burst is sent out */
   else  if ( ded_d_ptr->no_of_ab_to_send == 1 )
   {
      ded_d_ptr->no_of_ab_to_send = 0;
      L1_send_ISR_HANDOVER_CNF(gas_id);
      L1isr_send_PH_READY_TO_SEND_IND(DCCH_type,gas_id);
      L1isr_send_PH_READY_TO_SEND_IND(SACCH,gas_id);

      return ( FALSE );
   }
   else
   {
      return ( TRUE );
   }
}

/*===========================================================================

FUNCTION  L1_GET_DTX_STATE

DESCRIPTION
  This function gets the current DTX state (ON vs OFF)

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean L1_get_dtx_state(gas_id_t gas_id)
{
   return ( l1_get_tch_amr_dtx_state(gas_id) || l1_get_tch_dtx_state(gas_id) );
}

/*===========================================================================

FUNCTION  L1_SET_REPORTING_PERIOD

DESCRIPTION
  This function sets the absolute FNMod104 (TCH) or FNMod102 (SDCCH) to
  which the reporting period indicated by txlev[][] is aligned. This
  provides the offset that needs to be applied to an absolute FNMod104/102
  to index the txlev[][] array and determine the Tx power on any specified 
  FNMod104/102.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_set_reporting_period(uint16 reporting_period_end_minus_1, uint16 reporting_period, gas_id_t gas_id)
{
   txlev_reporting_period_end_minus_1[gas_id] = reporting_period_end_minus_1;
   txlev_reporting_period[gas_id] = reporting_period;
}

/*===========================================================================

FUNCTION  L1_GET_TXLEV_FOR_FN

DESCRIPTION
  This function gets the TxLev for the specified FN.

DEPENDENCIES
  L1_SET_REPORTING_PERIOD() and L1_PRECOMPUTE_TXLEV() must have been called
  prior to calling L1_GET_TXLEV_FOR_FN()

RETURN VALUE
  TxLev for specified FN

SIDE EFFECTS
  None

===========================================================================*/
int16 L1_get_txlev_for_FN(uint32 fn, gas_id_t gas_id)
{
   volatile ISRTIM_CMD_BLK* l1_tskisr_blk  = &l1_tsk_buffer[gas_id];
   uint8                    fnMod102104    = fn % txlev_reporting_period[gas_id];

  /* The txlev_reporting_period is the length of the reporting period (102 for 
   * SDCCH or 104 for TCH). This is the number of TxLevs that are
   * pre-computed prior to the start of each reporting period.
   *
   * The txlev_reporting_period_end_minus_1 represents the point at which one
   * reporting period ends and the next begins, relative to FNMod102(SDCCH)
   * or FnMod104(TCH).
   *
   * The txlev[][] array is aligned to the reporting period *NOT* the FNMod102
   * or FNMod104, so given the an absolute FNMod102 or 104 we need to modulo
   * add txlev_reporting_period_end_minus_1 to get the required index into the
   * txlev[][] array and retrieve the TxLev that we require.
   *
   * The +2 is to account for the -1 inherent in txlev_reporting_period_end_minus_1
   * +1 to account for the issuing of the command one frame early.
   */

   if(l1_tskisr_blk->l1_state!=L1_DEDICATED_MODE)
   {
      return 0;
   }

   if( (txlev_reporting_period[gas_id]!=104) && (txlev_reporting_period[gas_id]!=102) )
   {
      return 0;
   }

   if( txlev_reporting_period_end_minus_1[gas_id] > 104)
   {
      return 0;
   }

   return txlev[gas_id][( txlev_reporting_period_end_minus_1[gas_id] + fnMod102104 + 2) % txlev_reporting_period[gas_id]];
}

