/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

          L 1   D E D I C A T E D - (U / L) C O D E C   A D A P T A T I O N

GENERAL DESCRIPTION
   This module contains the procedures and global variables required to 
   implement codec mode adaptation on the U/L for GSM AMR Full and Half rate 
   as per GSM 05.09 standard. It also include some general purpose functions
   for AMR implementation.

EXTERNALIZED FUNCTIONS
   gen_codec_mode_request
      This provides an interface to generate Codec Mode Requests during
      AMR call
   filter_c_over_i_fr
      This function implements the SNR filter needed for AMR full rate and
      returns a normalized C/I value that is used to generate the Codec Mode 
      Request (CMR) to be transmitted on the U/L.
   filter_c_over_i_hr
      This function implements the SNR filter needed for AMR half rate and
      returns a normalized C/I value that is used to generate the Codec Mode 
      Request (CMR) to be transmitted on the U/L.
   set_ul_default_icm
      This provides an interface to set the default U/L Initial Codec Mode
      in case an ICM is not provided in the multi-rate IE by the network
   set_dl_default_icm
      This provides an interface to set the default D/L Initial Codec Mode
      in case an ICM is not provided in the multi-rate IE by the network
   set_ul_acs
      This provides an interface to set the default U/L Active Codec Set
   set_dl_acs
      This provides an interface to set the default D/L Active Codec Set
     
INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.
$Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_amr_ca.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
03/01/16   mn       CR1104539:Migrating to new API to expose the value of GPRS_ANITE_GCF in Gl1(rework of1094617)
08/04/15   pjr      CR818535 FR25951 Changes to support Single Voice Session 
21/06/13   cs       TSTS Updates
16/05/13   pjr      CR487402: Changes to improve interaction with Voice Services
03/04/13   pjr      CR470940 included gl1_hw_vs_g.h 
07/03/13   dv       CR461080: Introduction of new GSM VS interface  
06/04/10   dv       CR228666 AHS codec rate fluctuations in link adaptation TCs@Nortel IOT 
06/03/07   ws       Removed unecessary debug from AMR/SAIC optimisations
20/12/06   ws       CR: 106468 - bias c_over_i to take account of SAIC improvement
10/17/06   ws       Protect against filter_input becoming 0. Set filter to default params   
10/13/06   ws       CR 102919 only use AMR C/I optimisation if we have less than 8
                    hopping frequencies in MA list
07/08/06   ws       CR 95398 - Optimization of C/I calculation for frequency hopping 
05/03/05   pp       Fix for 14.10 test case
05/26/04   sv       Lint Cleanup.
03/18/04   gsc      Lint Cleanup
02/18/04   gsc      If thresh/hyst pairs are not defined, U/L CMR is set to
                    the D/L ICM value.
11/05/03   gsc      Merge from the main line into 5.0 branch
10/17/03   gsc      Use burst C/I value to initialize the filter rather than
                    initializing it to 0. This results in faster ramp up. 
10/03/03   gsc      U/L codec rate adaptation related changes
09/02/03   gsc      In set_dl_default_icm changed curr_ul_num_acm to 
                    curr_dl_num_acm
08/11/03   gsc      Removed FEATURE_USE_C_OVER_I and related changes
08/11/03   gsc      C/I filter changes to include the new C/I field in the
                    burst metrics. 
07/14/03   gsc      Fixes based on Ericsson IOT.
07/01/03   gsc      Modified set_ul_default_icm amd set_dl_default_icm.
05/07/03   gsc      First revision.

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "comdef.h"

#ifdef CUST_H
#include "customer.h"
#endif

#ifdef FEATURE_GSM_AMR

#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "l1_amr.h"
#include "msg.h"

#include "geran_dual_sim.h"
#include "mcfg_nv_api.h"

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/

static uint32 take_log2(uint16 data);

/*---------------------------------------------------------------------------
 *                       EXTERNAL DATA
 *-------------------------------------------------------------------------*/

extern uint8 curr_dl_num_acm[];
extern uint8 curr_ul_num_acm[];
extern uint8 curr_ul_icm[];
extern uint8 curr_dl_icm[];
extern uint8 curr_cmr[];
extern uint8 computed_cmr[];
extern gl1_msg_amr_mode_type curr_dl_acs[NUM_GERAN_DATA_SPACES][MAX_CODEC_MODES];
extern gl1_msg_amr_mode_type curr_ul_acs[NUM_GERAN_DATA_SPACES][MAX_CODEC_MODES];
extern uint8 curr_thresh_tbl[NUM_GERAN_DATA_SPACES][MAX_CODEC_MODES - 1];
extern uint8 curr_hyst_tbl[NUM_GERAN_DATA_SPACES][MAX_CODEC_MODES - 1];
extern uint32 curr_norm_c_over_i[];
extern int32 prev_c_over_i_filter_output[];
extern boolean amr_thresh_hyst_defined[];


/*===========================================================================
FUNCTION: filter_c_over_i_fr

DESCRIPTION:
  This function implements the SNR filter needed for AMR full rate and
  returns a normalized C/I value that is used to generate the Codec Mode 
  Request (CMR) to be transmitted on the U/L.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
void filter_c_over_i_fr(uint16 mean_c_over_i, gas_id_t gas_id)
{
  int32 filter_input, filter_output;
  uint32 trunc_mask = 0x4000;
  const uint16 saic_offset_param = 16; /* 32 * 0.5 */
   
  /* Bias mean_c_over_i to take account of improvement in the downlink of SAIC.
   * This will give a more consevative CMR to improve performance of downlink
   * in poor signal conditions
   */
  if( (mean_c_over_i > saic_offset_param) && (mean_c_over_i < 500) && (mcfg_gcf_nv_get_status() == FALSE) )
  {
    
    mean_c_over_i = ( ( mean_c_over_i - saic_offset_param) * 21) >> 5; /* 21 = 32 /1.5 */
  }


  
  
  /* The difference take_log2( mean_c_over_i ) - 327680 must not yield a
   * +ve value greater than 2^31 - 1 otherwise the int32 will overflow
   */
  /* Suppress shift right of signed quantity */ 
  filter_input = ( ( (int32)( take_log2( mean_c_over_i ) - 327680 ) ) >> 6 ) * 193;  /*lint !e704*/

  if ( prev_c_over_i_filter_output[gas_id] == 0 )
  {
     prev_c_over_i_filter_output[gas_id] += filter_input;
  }

  /* Suppress shift right of signed quantity */ 
  filter_output = (115 * prev_c_over_i_filter_output[gas_id] + 13 * filter_input) >> 7;  /*lint !e704*/

  prev_c_over_i_filter_output[gas_id] = filter_output;

  if ( filter_output < 0 )
  {
     filter_output = 0;
  }
  curr_norm_c_over_i[gas_id] = 
    ( (uint32)filter_output + ((uint32)filter_output & trunc_mask ) ) >> 15;

  if ( curr_norm_c_over_i[gas_id] > 63 )
  {
     curr_norm_c_over_i[gas_id] = 63;
  }
}

/*===========================================================================
FUNCTION: gen_codec_mode_request

DESCRIPTION:
  This function uses the current set of thresholds and hysteresis values 
  and the normalized C/I value to adjust the current Codec Mode Request.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
void gen_codec_mode_request(gas_id_t gas_id)
{
  if (!amr_thresh_hyst_defined[gas_id])
  {
    computed_cmr[gas_id] = curr_cmr[gas_id] = curr_dl_icm[gas_id];
  }
  else
  {
    if ( (curr_cmr[gas_id] < curr_dl_num_acm[gas_id] - 1) &&
         (curr_norm_c_over_i[gas_id] >= 
          curr_thresh_tbl[gas_id][curr_cmr[gas_id]] + curr_hyst_tbl[gas_id][curr_cmr[gas_id]] ) )
    {
      computed_cmr[gas_id] = curr_cmr[gas_id]+1;
    }
    else if ( ( curr_cmr[gas_id] > 0 ) && 
              ( curr_norm_c_over_i[gas_id] < curr_thresh_tbl[gas_id][curr_cmr[gas_id]-1] ) )
    {
      computed_cmr[gas_id] = curr_cmr[gas_id]-1;
    }
    else
    {
      computed_cmr[gas_id] = curr_cmr[gas_id];
    }
  }
}

/*===========================================================================
FUNCTION: set_ul_default_icm

DESCRIPTION:
  This function implements the set of rules outlined in GSM 05.09 standard
  to determine the U/L Initial Codec Mode in case it is not specified in
  the multi-rate IE.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
void set_ul_default_icm(gas_id_t gas_id)
{
  switch ( curr_ul_num_acm[gas_id] )
  {
    case 1:
    case 2:
    case 3:
      curr_ul_icm[gas_id] = 0;
      break;
    case 4:
      curr_ul_icm[gas_id] = 1;
      break;

    default:
      MSG_GERAN_ERROR_1("Invalid num of codec modes %d", curr_ul_num_acm[gas_id]);
      curr_ul_icm[gas_id] = 0;
      break;
  }
}	

/*===========================================================================
FUNCTION: set_dl_default_icm

DESCRIPTION:
  This function implements the set of rules outlined in GSM 05.09 standard
  to determine the D/L Initial Codec Mode in case it is not specified in
  the multi-rate IE.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
void set_dl_default_icm(gas_id_t gas_id)
{
  switch ( curr_dl_num_acm[gas_id] )
  {
    case 1:
    case 2:
    case 3:
      curr_dl_icm[gas_id] = 0;
      break;
    case 4:
      curr_dl_icm[gas_id] = 1;
      break;
    default:
      MSG_GERAN_ERROR_1("Invalid num of codec modes %d", curr_dl_num_acm[gas_id]);
      break;
  }
}

/*===========================================================================
FUNCTION: set_ul_acs

DESCRIPTION:
  This function sets the U/L ACS.

DEPENDENCIES:
  None

RETURN VALUE:
  TRUE if successful, otherwise FALSE  

SIDE EFFECTS:
  None
===========================================================================*/
boolean set_ul_acs( gl1_msg_amr_mode_type *acs, uint8 num_modes, gas_id_t gas_id )
{
  uint16 i;

  if (!acs)
  {
     MSG_GERAN_ERROR_2("Rcvd empty ACS table, acs = %d", acs, 0 );
     return FALSE;
  }

  if ( num_modes > MAX_CODEC_MODES )
  {
      MSG_GERAN_ERROR_2("Invalid num of U/L ACM %d", num_modes, 0 );
      return FALSE;
  }

  for ( i = 0; i < num_modes; i++ )
  {
      curr_ul_acs[gas_id][i] = acs[i];
  }

  curr_ul_num_acm[gas_id] = num_modes;
  
  return TRUE;
}

/*===========================================================================
FUNCTION: set_dl_acs

DESCRIPTION:
  This function sets the D/L ACS.

DEPENDENCIES:
  None

RETURN VALUE:
  TRUE if successful, otherwise FALSE

SIDE EFFECTS:
  None
===========================================================================*/
boolean set_dl_acs( gl1_msg_amr_mode_type *acs, uint8 num_modes, gas_id_t gas_id )
{
  uint16 i;

  if (!acs)
  {
     MSG_GERAN_ERROR_2("Rcvd empty ACS table, acs = %d", acs, 0 );
     return FALSE;
  }

  if ( num_modes > MAX_CODEC_MODES )
  {
      MSG_GERAN_ERROR_2("Invalid num of D/L ACM %d", num_modes, 0 );
      return FALSE;
  }

  for ( i = 0; i < num_modes; i++ )
  {
      curr_dl_acs[gas_id][i] = acs[i];
  }

  curr_dl_num_acm[gas_id] = num_modes;
  
  return TRUE;
}

/*===========================================================================
FUNCTION: take_log2

DESCRIPTION:
  Log2 model is from Combination of first order taylor's series and 
  Second Order Curve fitting.  Max error is approx +/- 0.008 or     
  0.025dB. Input is a +ve non zero fixed point number < 65536           

  Output is in Q(16) and > 65536. For Fixed Point result divide by  
  65536. For log10, mult by log10(2)                                  

DEPENDENCIES:                                 
  None

RETURN VALUE:
  Returns a log2 of the number passed in

SIDE EFFECTS:
  None
===========================================================================*/
static uint32 take_log2(uint16 data)
{
  uint16 exp_log2, sto1;
  uint16 data1;
 
  for (exp_log2 = 0; data >= (1 << exp_log2 ); ++exp_log2 )
  {
      ;
  }

  --exp_log2;

  data1 = (data << (16 - exp_log2 ))  & 0xffff ;

  sto1 = 22708 - ((data1 * 22708) >> 16 );

  sto1 = (sto1 * data1) >> 16;

  return( (exp_log2 * 65536) + data1 + sto1  );
}


#endif /* FEATURE_GSM_AMR */
