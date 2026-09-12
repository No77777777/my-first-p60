/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

        L 1   S Y S T E M S   A L G O R I T H M S  F O R   A G C

GENERAL DESCRIPTION
   This module contains the procedures to control the power predictor
   algorithms required to control the system AGC.

EXTERNALIZED FUNCTIONS
  SYS_ALGO_AGC_INIT
    This function initializes the AGC data structure
  SYS_ALGO_AGC_GET_NEXT_INDEX
  SYS_ALGO_AGC_UPDATE
  SYS_ALGO_AGC_DISABLE_SLEW_RATE_LIMITING
  SYS_ALGO_AGC_ENABLE_SLEW_RATE_LIMITING
  SYS_ALGO_AGC_CONFIGURE_MAX_ATTEMPTS_PER_MP

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/gl1_sys_algo_agc.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who       what, where, why
--------   --------  ---------------------------------------------------------
29/05/15   pa        CR826683: FR27452 GL1 Antenna Sharing Between WLAN and WAN RAT's. Dynamic Voice & Data RxD. 
12/03/15   pa        CR806245:Add support of RxD for Single and Dual Multimode in DSDS (except GBTA/TA).
16/12/14   jk        CR753423:LIF for Idle,voice and data
24/09/14   jk        CR729382:Enable LIF feature by default
10/07/14   jk        CR688587:GL1 changes to support LIF feature
19/12/13   jk        CR503359 - GL1 changes to support Rx diversity
21/08/13   npt       CR531633 - Remove FEATURE_DIME_MODEM dependencies 
20/05/13   cs        TSTS Updates
24/07/09   ws        CR 180476 - Improve performance of Dedicated AGC
23/05/07   agv       CR118309 - Large RSSI variation initial fix debug.
01/10/05   dp        Lint cleanup
06/25/04   gfr       Remove obselete code.
11/05/03   gfr       Support for quad-band.
10/07/03   gfr       Removed cast that was causing erroneous truncation of
                     large numbers.
07/11/03   gfr       Lint cleanup.
10/28/03   JC        Added a static index to keep track of where we are in
                     the list during SYS_ALGO_SCAN_RUN.
10/15/02   JC        Minor comment cleanup.
09/05/02   dp        Fixed compiler warnings
05/21/02   gw        Added parameter to sys_algo_agc_get_next_index
                     to indicate when all ARFCNs are done.
05/10/02   JC        Added support to configure the number of attempts
                     per measurement period.
03/27/02   JC        Moved slew rate limit switch into the agc_T since it
                     is no longer a global switch.  It is applied on an
                     AGC-by-AGC basis.
02/13/02   JC        Increment attempt counter on agc_update instead of
                     return from sys_algo_agc_get_next_index.
01/14/02   JC        Initial version
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "comdef.h"
#include "gl1_sys_algo.h"
#include "gl1_sys_params.h"
#include "sys_type.h"
#include "gtmrs.h"
#include "gtmrs_g.h"
#include "msg.h"
#include "gl1_defs.h"
#include "gl1_defs_g.h"

#include "geran_dual_sim.h"
#include "geran_multi_sim.h"
#include "l1_utils.h"

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/

/* Declare a null arfcn for situations when one is needed */
ARFCN_T NULL_ARFCN = {0};

/* The time stamp is a monotonically increasing unsigned 32 bit number */
#define MAX_TIME_STAMP 0xFFFFFFFF

/* Define maximum and minimum valid powers */
#define MAX_VALID_PWR_dBx16  0
#define MIN_VALID_PWR_dBx16  (-200 * 16)

/* Define the maximum FN delta we care about, based on the maximum power delta
   we can expect */
#define MAX_DELTA_PWR_dBx16  (MAX_VALID_PWR_dBx16 - MIN_VALID_PWR_dBx16)
#define MAX_DELTA_FN (MAX_DELTA_PWR_dBx16 / SYS_PARAM_POSITIVE_AGC_SLEW_RATE_dBx16)

#define MAX_AVG_BURST_LEN 4

typedef struct
{
uint16 current_avg_burst_len;
/* Declare agc running average buffer */
dBx16_T sys_algo_agc_avg[MAX_AVG_BURST_LEN];

/* declare running average input index */
uint16 sys_algo_agc_avg_index;

#ifdef FEATURE_GSM_RX_DIVERSITY   
/* Declare agc running average buffer for diversity Rx */
dBx16_T sys_algo_agc_avg_divrx[MAX_AVG_BURST_LEN];
/* declare running average input index for diversity Rx*/
uint16 sys_algo_agc_avg_index_divrx;
#endif

}gl1_sys_algo_agc_data_t;

static gl1_sys_algo_agc_data_t  gl1_sys_algo_agc_data[NUM_GERAN_DATA_SPACES];
static gl1_sys_algo_agc_data_t *gl1_sys_algo_agc_data_ptr[NUM_GERAN_DATA_SPACES] =
                                  { INITIAL_VALUE_INDEXED( &gl1_sys_algo_agc_data ) };

/*===========================================================================

FUNCTION  SYS_ALGO_AGC_AVG_INIT

DESCRIPTION
  This function initializes the AGC running average data structure.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void sys_algo_agc_avg_init(uint16 avg_len, gas_id_t gas_id )
{
 int16 i;

  MSG_GERAN_MED_1_G("Init AGC running average buf avg_len %d",avg_len);

  /* Preset AGC buffer to mid range */
  for(i=0;i<avg_len;i++)
  {
    gl1_sys_algo_agc_data_ptr[gas_id]->sys_algo_agc_avg[i]= SYS_PARAM_INIT_EXP_PWR_RX_LVL_dBm_x16;
#ifdef FEATURE_GSM_RX_DIVERSITY 
   gl1_sys_algo_agc_data_ptr[gas_id]->sys_algo_agc_avg_divrx[i]= SYS_PARAM_INIT_EXP_PWR_RX_LVL_dBm_x16;	
#endif
  }

  gl1_sys_algo_agc_data_ptr[gas_id]->sys_algo_agc_avg_index = 0;
#ifdef FEATURE_GSM_RX_DIVERSITY 
  gl1_sys_algo_agc_data_ptr[gas_id]->sys_algo_agc_avg_index_divrx = 0;
#endif
  gl1_sys_algo_agc_data_ptr[gas_id]->current_avg_burst_len = avg_len;

}

/*===========================================================================

FUNCTION  SYS_ALGO_AGC_INIT

DESCRIPTION
  This function initializes the AGC data structure.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void sys_algo_agc_init (sys_algo_agc_T  *agc_ptr, dBx16_T init_value)
{
  agc_ptr->pwr_dBm_x16  = init_value;
  agc_ptr->first_visit  = TRUE;
  agc_ptr->valid        = FALSE;
  agc_ptr->srl_enabled  = TRUE;
  agc_ptr->arfcn        = NULL_ARFCN;
  agc_ptr->time_stamp   = 0;
  agc_ptr->FN           = 0;
}



/*===========================================================================

FUNCTION  SYS_ALGO_AGC_UPDATE

DESCRIPTION
  This function provides update information to the power predictor control
  algorithm.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void sys_algo_agc_update
(
   sys_algo_agc_T     *agc_ptr,
   dBx16_T            pwr_dBm_x16,
   boolean            in_range,
   uint32             time_stamp,
   gas_id_t           gas_id
)
{
   uint32 delta_FN;
   dBx16_T delta_pwr, max_pos_delta_pwr, max_neg_delta_pwr;

   /* Sanity check incoming power */
   if (pwr_dBm_x16 < MIN_VALID_PWR_dBx16 || pwr_dBm_x16 > MAX_VALID_PWR_dBx16)
   {
      MSG_GERAN_ERROR_2_G("Invalid AGC pwr %d, FN=%d", pwr_dBm_x16, GSTMR_GET_FN_GERAN(gas_id));
      return;
   }

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE   
   if(NULL != agc_ptr)
   {
#endif // #ifdef FEATURE_GSM_RX_DIVERSITY_IDLE	

   if (agc_ptr->first_visit)
   {
      agc_ptr->pwr_dBm_x16 = pwr_dBm_x16;
      agc_ptr->first_visit = FALSE;
   }
   else
   {
      /* Do slew-rate limiting if necessary */
      if (agc_ptr->srl_enabled)
      {
         /* Calculate the time stamp delta in frames */
         if (time_stamp >= agc_ptr->time_stamp)
         {
            delta_FN = time_stamp - agc_ptr->time_stamp;
         }
         else
         {
            delta_FN = time_stamp + (MAX_TIME_STAMP - agc_ptr->time_stamp);
         }

         /* If delta FN is too large the pwr calculations could overflow */
         if (delta_FN > MAX_DELTA_FN) delta_FN = MAX_DELTA_FN;

         /* Calculate the power change and SRL max power change */
         delta_pwr = pwr_dBm_x16 - agc_ptr->pwr_dBm_x16;
         max_pos_delta_pwr = (dBx16_T)(SYS_PARAM_POSITIVE_AGC_SLEW_RATE_dBx16 * delta_FN);
         max_neg_delta_pwr = (dBx16_T)(SYS_PARAM_NEGATIVE_AGC_SLEW_RATE_dBx16 * (dBx16_T)delta_FN);

#ifdef FEATURE_GERAN_REDUCED_F3
         MSG_GERAN_MED_3_G("AGC: delta_pwr %d dB max_pos_delta %d dB max_neg_delta %d dB",delta_pwr/16,max_pos_delta_pwr/16,max_neg_delta_pwr/16);
#endif
         /* Cap the power change */
         if (delta_pwr > max_pos_delta_pwr)
         {
            agc_ptr->pwr_dBm_x16 += max_pos_delta_pwr;
         }
         else if (delta_pwr < max_neg_delta_pwr)
         {
            agc_ptr->pwr_dBm_x16 += max_neg_delta_pwr;
         }
         else
         {
            agc_ptr->pwr_dBm_x16 = pwr_dBm_x16;
#ifdef FEATURE_GERAN_REDUCED_F3
            MSG_GERAN_MED_2_G("AGC: slew not applied delta_pwr %d dB delata_FN %d",delta_pwr/16,delta_FN);
#endif
         }
      }
      else
      {
         agc_ptr->pwr_dBm_x16 = pwr_dBm_x16;
      }

   }

   /* Sanity check outgoing power */
   if (agc_ptr->pwr_dBm_x16 < MIN_VALID_PWR_dBx16)
   {
      MSG_GERAN_ERROR_3_G("Invalid new AGC pwr %d (given=%d), FN=%d", agc_ptr->pwr_dBm_x16, pwr_dBm_x16, GSTMR_GET_FN_GERAN( gas_id));
      agc_ptr->pwr_dBm_x16 = MIN_VALID_PWR_dBx16;
   }
   else if (agc_ptr->pwr_dBm_x16 > MAX_VALID_PWR_dBx16)
   {
      MSG_GERAN_ERROR_3_G("Invalid new AGC pwr %d (given=%d), FN=%d", agc_ptr->pwr_dBm_x16, pwr_dBm_x16, GSTMR_GET_FN_GERAN( gas_id));
      agc_ptr->pwr_dBm_x16 = MAX_VALID_PWR_dBx16;
   }


   /* AGC doesn't care whether the power is in range or out of range  */
   /* It just copies this status into the AGC structure as a courtesy */
   /* to GSM L1                                                       */
   agc_ptr->valid = in_range;
   agc_ptr->FN = GSTMR_GET_FN_GERAN( gas_id);
   agc_ptr->time_stamp  = time_stamp;

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
   }
   else
   {
      MSG_GERAN_ERROR_0_G(GL1_RXD"agc_ptr is NULL! not updating AGC!");
   }
#endif /*ifdef FEATURE_GSM_RX_DIVERSITY_IDLE*/

}



/*===========================================================================

FUNCTION  SYS_ALGO_AGC_DISABLE_SLEW_RATE_LIMITING

DESCRIPTION
  This function disables the slew rate limiting function of the power
  predictor algorithm.

===========================================================================*/
void sys_algo_agc_disable_slew_rate_limiting(sys_algo_agc_T *agc_ptr)
{
   agc_ptr->srl_enabled = FALSE;
}


/*===========================================================================

FUNCTION  SYS_ALGO_AGC_ENABLE_SLEW_RATE_LIMITING

DESCRIPTION
  This function enables the slew rate limiting function of the power
  predictor algorithm.

===========================================================================*/
void sys_algo_agc_enable_slew_rate_limiting(sys_algo_agc_T *agc_ptr)
{
   agc_ptr->srl_enabled = TRUE;
}

/*===========================================================================

FUNCTION  SYS_ALGO_AGC_AVERAGE_OUTPUT

DESCRIPTION
  This function averages the output of the AGC over MAX_AVG_BURST_LEN bursts

DEPENDENCIES
  None

RETURN VALUE
  int16 rxlev

SIDE EFFECTS
  None

===========================================================================*/
dBx16_T sys_algo_agc_average_output(dBx16_T agc_output, gas_id_t gas_id )
{
  dBx16_T agc_average,agc_total=0;
  int16 i;

  /* load up latest output from AGC into running average */
  gl1_sys_algo_agc_data_ptr[gas_id]->sys_algo_agc_avg[gl1_sys_algo_agc_data_ptr[gas_id]->sys_algo_agc_avg_index++] = agc_output;

  /* wrap around index if it's reached the end */
  if(gl1_sys_algo_agc_data_ptr[gas_id]->sys_algo_agc_avg_index >= gl1_sys_algo_agc_data_ptr[gas_id]->current_avg_burst_len)
  {
    gl1_sys_algo_agc_data_ptr[gas_id]->sys_algo_agc_avg_index = 0;
  }

  /* calculate new AGC average */
  for(i=0;i<gl1_sys_algo_agc_data_ptr[gas_id]->current_avg_burst_len;i++)
  {
    agc_total += gl1_sys_algo_agc_data_ptr[gas_id]->sys_algo_agc_avg[i];
  }

  agc_average = (agc_total/gl1_sys_algo_agc_data_ptr[gas_id]->current_avg_burst_len);
  MSG_GERAN_LOW_3_G("AGC: Burst run avg %d  burst i/p %d index %d",agc_average/16,agc_output/16,gl1_sys_algo_agc_data_ptr[gas_id]->sys_algo_agc_avg_index);

  return(agc_average);
}

#ifdef FEATURE_GSM_RX_DIVERSITY
/*===========================================================================

FUNCTION  sys_algo_agc_average_output_divrx

DESCRIPTION
  This function averages the output of the AGC over MAX_AVG_BURST_LEN bursts for diversity RX

DEPENDENCIES
  None

RETURN VALUE
  int16 rxlev

SIDE EFFECTS
  None

===========================================================================*/
dBx16_T sys_algo_agc_average_output_divrx(dBx16_T agc_output,gas_id_t gas_id)
{
  dBx16_T agc_average,agc_total=0;
  int16 i;

  /* load up latest output from AGC into running average */
  gl1_sys_algo_agc_data_ptr[gas_id]->sys_algo_agc_avg_divrx[gl1_sys_algo_agc_data_ptr[gas_id]->sys_algo_agc_avg_index_divrx++] = agc_output;

  /* wrap around index if it's reached the end */
  if(gl1_sys_algo_agc_data_ptr[gas_id]->sys_algo_agc_avg_index_divrx >= gl1_sys_algo_agc_data_ptr[gas_id]->current_avg_burst_len)
  {
    gl1_sys_algo_agc_data_ptr[gas_id]->sys_algo_agc_avg_index_divrx = 0;
  }

  /* calculate new AGC average */
  for(i=0;i<gl1_sys_algo_agc_data_ptr[gas_id]->current_avg_burst_len;i++)
  {
    agc_total += gl1_sys_algo_agc_data_ptr[gas_id]->sys_algo_agc_avg_divrx[i];
  }

  agc_average = (agc_total/gl1_sys_algo_agc_data_ptr[gas_id]->current_avg_burst_len);
  MSG_GERAN_LOW_3_G(GL1_RXD"AGC DivRx: Burst run avg %d  burst i/p %d index %d",
                    agc_average/16,
                    agc_output/16,
                    gl1_sys_algo_agc_data_ptr[gas_id]->sys_algo_agc_avg_index_divrx);

  return(agc_average);
}
#endif /*FEATURE_GSM_RX_DIVERSITY*/


