/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                L 1   G L O B A L   V A R I A B L E S

GENERAL DESCRIPTION
   This module contains the globals used by Layer1.  Refrain from adding
   variables to this file as there is an effort to obsolete this file
   moving forwards !!!!!

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_glbls.c#1 $
  $DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who       what, where, why
--------   --------  ----------------------------------------------------------
20/03/17   mk        CR2020999 Prx Drx power difference needs to be calculated & applied to NCELLs before REP_PERIOD_IND to avoid delays
08/12/16   mk        CR1097591 Enhancement for DRX: DRx and PRx difference will be applied to neighbor cell measurements, if DRx power level is greater than PRx
01/10/13   sk        CR552259 CR sync from 1.0/1.1
20/08/13   nn        CR518344 Use secapi_get_random() for generating random numbers.
06/04/04   sv        Removed all references to XXX_mframe_power_XXX.
11/05/03   gfr       Support for quad-band.
06/23/03   gfr       Compiler warning cleanup.
09/03/02   dp        Cleaned up first_cch_block
05/30/02   dp        Declared l1_tskisr_blk as volatile to suppress compiler
                     optimizations
02/07/02   gw        Removed obsolete SCE variables.
09/27/01   gw/jc     Removed unused variables.
09/25/01   mk        General SCE support related cleanup.
08/31/01   JC        Changed rach_ARFCN to uint16 type, removed unnecessary
                     compile flags, & relocated globals to respective
                     modules.
05/16/01   JC        Initial version.  Changed txcch_ARFCN type

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "comdef.h"
#include "rr_l1.h"
#include "rr_l1_g.h"      /* channel_information_T      */
#include "l1i.h"        /* ISRTIM_CMD_BLK,idle_data_T */
#include "l1_isr.h"     /* cch_state_T                */

#include "geran_dual_sim.h"

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/
typedef struct
{

byte                      current_timeslot;
boolean                   L1_specific_bcch;

/* Globals*/


/* Globals for the Routine L1_read_CCH (m10510.c) */
boolean                   cch_ciphering_flag;
idle_data_T              *idle_data_ptr_1;
idle_data_T              *idle_data_ptr_2;

boolean                   seed_used;

byte                      no_of_messages;

/* Globals for SCE */
boolean                   serv_pwr_meas_permitted;

}l1_glbls_data_t;


/* Fill Frame for the DCCH - the SACCH is 2 bytes shorter -
    see GSM 04.06 section 5.4.2.3 */

const byte fill_frame[N201_MAX] = {

   0x01, 0x03, 0x01, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b,
   0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b,
   0x2b, 0x2b, 0x2b
};

static l1_glbls_data_t  l1_glbls_data[NUM_GERAN_DATA_SPACES];

l1_serving_cell_meas_T    l1_serving_cell_meas[NUM_GERAN_DATA_SPACES];


byte l1_get_current_timeslot( gas_id_t gas_id )
{
  l1_glbls_data_t *l1_glbls_data_ptr = &l1_glbls_data[gas_id];
  return l1_glbls_data_ptr->current_timeslot;
} 
void l1_set_current_timeslot(byte current_timeslot,gas_id_t gas_id )
{
  l1_glbls_data_t *l1_glbls_data_ptr = &l1_glbls_data[gas_id];
  l1_glbls_data_ptr->current_timeslot=current_timeslot;

  {
    eng_mode_curr_l1_ts_t eng_mode_ts;

    eng_mode_ts.timeslot = current_timeslot;
    geran_eng_mode_data_write(ENG_MODE_CURRENT_L1_TIMESLOT,(void*)&eng_mode_ts, TRUE, gas_id);
  }
} 
boolean l1_get_cch_ciphering_flag( gas_id_t gas_id )
{
  l1_glbls_data_t *l1_glbls_data_ptr = &l1_glbls_data[gas_id];
  return l1_glbls_data_ptr->cch_ciphering_flag;
} 
void l1_set_cch_ciphering_flag(boolean cch_ciphering_flag,gas_id_t gas_id )
{
  l1_glbls_data_t *l1_glbls_data_ptr = &l1_glbls_data[gas_id];
  l1_glbls_data_ptr->cch_ciphering_flag=cch_ciphering_flag;
}

boolean l1_get_serv_pwr_meas_permitted( gas_id_t gas_id )
{
  l1_glbls_data_t *l1_glbls_data_ptr = &l1_glbls_data[gas_id];
  return l1_glbls_data_ptr->serv_pwr_meas_permitted;
} 
void l1_set_serv_pwr_meas_permitted(boolean serv_pwr_meas_permitted,gas_id_t gas_id )
{
  l1_glbls_data_t *l1_glbls_data_ptr = &l1_glbls_data[gas_id];
  l1_glbls_data_ptr->serv_pwr_meas_permitted=serv_pwr_meas_permitted;
}
