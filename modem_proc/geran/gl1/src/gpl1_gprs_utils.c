/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

          G P L 1   G P R S  U T I L S

GENERAL DESCRIPTION
   This module contains the procedures for the following:

   Calculate Paging Data
   Calculate Hopping ARFCNS

   This procedure executes in the L1 ISR context.

EXTERNALIZED FUNCTIONS


  gpl1_gprs_calculate_paging_data
  gpl1_gprs_calculate_arfcns
  gpl1_gprs_get_arfcns


INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/gpl1_gprs_utils.c#1 $

when       who       what, where, why
--------   --------  ----------------------------------------------------------
15/04/15   hd        CR822761: Properly check for maximum no freq in hopping list
30/05/14   ws        CR657674 Data + MMS Phase 1
17/05/12   cgc       CR332796 add support for Tx power info api
17/11/10   cja       Fix Klocwork issues
24/09/10   cja       Fix for Klocwork
08/09/09   cs        Fix for Klocwork bounds checking problems
13/02/09   ap        Lint fixes for high Warning
28/07/98   hv        Added KxMutex support
28/11/06   dv        Lint cleanup
11/11/05   og        Lint 8.0 AMSS_std_critical_errors removal.
04/11/05   og        Adjustements to start time checks, resolves CR 79207.
18/07/05   og        Add handling for missing (ST - T) range.
01/12/04   ws        Corrected shift_out_fbi_e() to produce correct number of
                     shifted out bytes
25/10/04   ws        Corrected bug in shift_out_fbi_e() for dual payload lengths
13/08/04   kf        Modification of start time calculation
09/07/04   ws        Added shift_out_fbi_e function
23/06/04   ws        Corrected bug in previous checkin % FRAMES_IN_HYPERFRAME instead
                     of % 42432
23/06/04   kf        Added % 42432 of startime in gpl1_gprs_adjust_51_starting_time().
20/01/04   dlh       Lint fix
26/11/03   ws        Lint cleanup
06/11/03   gfr       Support for quad-band.
23/10/02   dlh       Removed ext paging gap as no longer calculated here.
14/10/03   npr       Fixed bug in handling of starting time already elapsed condition.
14/10/03   dlh       Lint Cleanup
14/10/03   npr       Fixed bug in handling of starting time already elapsed condition.
02/10/03   ws        Fixed bug in detecting starting time elapsed condition
25/09/03   dlh       Sleep mode fix.
03/09/03   pjr       Modified logic when checking temporary starting times dl_st
                     and ul_st.
29/08/03   dlh       Added gap calculations to packet paging algo.
08/08/03   kf        changed msg_errror to err_fatal.
08/08/03   dlh       Added divide by 0 F3 messages.
24/07/03   pjr       Modified gpl1_gprs_get_starting_time to set start_time_elapsed
                     for 52 type starting times.
11/07/03   gw        Added check for divide by zero in gpl1_gprs_get_ARFCN().
11/07/03   ws        Added  gpl1_gprs_is_frame_num_later()
08/07/03   gw        GRR/L1 SCE cleanup.
27/06/03   pjr       Modified logic when checking temporary starting times dl_st
                     and ul_st.
06/23/03   gfr       Compiler warning cleanup.
20/06/03   pjr       Add check to determine if either of the temporary starting
                     times dl_st and ul_st have elapsed, if so set to invalid
10/06/03   pjr       Added handling of PDA whilst waiting for PUA starting time
                     and PUA whilst waiting for PDA starting time.
29/05/03    kf       ncell pbcch read changse
20/05/03    ws       Modifications to use correct starting time calculation
19/05/03    ws       Fixed bug for PTCCH frequency hoping
02/05/03    ws       Updates to gpl1_gprs_calculate_paging_data()
01/05/03    ag       Updates to ncell pbcch reading.
13/03/03    ws       Added FN boundary checking to gpl1_gprs_chk_blk_bndry()
26/02/03    ws       Modified frequency hopping for PTCCH as it is now called 2 frames
                     before RX/TX  frame
17/02/03    ws       Changed MSG_HIGH's to MSG_LOW's  during DL power control
14/02/03    pjr      Added PBCCH case to gpl1_gprs_calculate_arfcns (will be removed later)
13/02/03    ws       Added gpl1_gprs_get_pr_value()
02/05/02    ws       Adjusted starting time min reaction time and frequency hopping
                     for new scheduling mechanism
12/04/02    ws       Changed  FEATURE_GSM_GPRS_L1_PBCCH_PCCCH to FEATURE_GSM_GPRS_PCCCH
11/27/02    DLH      Added FEATURE_GSM_GPRS_L1_PBCCH_PCCCH featurization to GPL1_GPRS_CALCULATE_PAGING_DATA
10/21/02    he       Replaced FN by gp1_get_FN().
16/10/02    pjr      Changed start time for no supplied start time to FN+4
09/09/02    ws       set starting time to mod42432 if received from IA
09/18/02    ws       Added gpl1_gprs_adjust_51_starting_time() to adjust 51 starting
                     times and added call in gpl1_gprs_get_starting_time
09/06/02    pjr      Changed change_value parameter of gpl1_gprs_adjust_mod_num
                     to int32.
09/05/02    pjr      Modified gpl1_gprs_get_starting_time to calculate starting time
                     for 51 access, and changed return to boolean from void.
08/19/02    pjr      Added function gpl1_gprs_adjust_mod_num.
08/08/02    DLH      Corrected Lint errors
07/31/02    DLH      Added extended paging calculation
07/16/02    DLH      Added gpl1_gprs_calculate_paging_data function
07/16/02    DLH      Added gpl1_gprs_calculate_arfcns frequency hopping function

06/27/02    ws       Added support of gpl1_gprs_get_starting_time() and
                     gpl1_gprs_chk_blk_bndry()

06/21/02    DLH      Initial version created for GPRS utility functions

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "customer.h"


#include "comdef.h"

#include "gtmrs.h"
#include "gtmrs_g.h"

#include "l1_task.h"
#include "l1_isr.h"
#include "gpl1_gprs_utils.h"
#include "gpl1_gprs_isr.h"
#include "geran_eng_mode_read_api.h"

#ifdef PL1_FW_SIM
#include "pl1_test_harness.h"
#endif

#include "l1_utils.h" /* for gl1_get_FN() */
#include "l1_sc.h"
#include "l1_sc_int.h"
#include "err.h"

#include "l1_log.h"

extern const byte NBIN[65];
extern const byte RNTABLE[114];

#define paging_blk_array_size 12

/*===========================================================================

  DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.
===========================================================================*/

/* This value is used to determine which of two  */
/* frame numbers is earlier.  A lower frame       */
/* number may be later if the frame number just  */
/* wrapped around.                               */
#define FN_WRAP_THRESHOLD        (FRAMES_IN_HYPERFRAME/2)
#define FN_MAX_PLUS_ONE          FRAMES_IN_HYPERFRAME

typedef struct
{
  /* Local start time and frame number variables */
  uint32  temp_start_time;
}gpl1_gprs_utils_data_t;

static gpl1_gprs_utils_data_t  gpl1_gprs_utils_data[NUM_GERAN_DATA_SPACES];


/*===========================================================================

FUNCTION gpl1_gprs_chk_blk_bndry

DESCRIPTION
  This function checks the supplied frame number lies on a block boundary.
  If NOT on a block boundary returns TRUE.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_chk_blk_bndry(uint32 *st_ptr)
{

  if( NULL != st_ptr )
  {
    switch  ((*st_ptr)%13)
    {
    case 0:
    case 4:
    case 8:
      /*  If already at start do not modify */

      break;

    case 3:
    case 7:
    case 12:
      /*  Move to next boundary by incrementing one frame.  */
      *st_ptr = ADD_FN(*st_ptr,1);

      break;

    case 2:
    case 6:
    case 11:
      /*  Move to next boundary by incrementing two frames.  */
      *st_ptr = ADD_FN(*st_ptr,2);
      break;

    case 1:
    case 5:
    case 10:
      /*  Move to next boundary by incrementing three frames. */
      *st_ptr = ADD_FN(*st_ptr,3);

      break;

    case 9:
      /*  Move to next boundary by incrementing four frames.  */
      *st_ptr = ADD_FN(*st_ptr,4);

      break;
    default:
      break;
    }
  }
  else
  {
    MSG_GERAN_ERROR_0(" NULL st_ptr ");
  }
}


/*===========================================================================

FUNCTION gpl1_gprs_get_starting_time

DESCRIPTION
  This function checks the supplied starting time and determines adjusts the
  value to lie on a block boundary.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean  gpl1_gprs_get_starting_time( uint32*    start_time_out_ptr,
                                      uint32     start_time_in,
                                      channel_t  st_mode, gas_id_t gas_id )
{
    uint32   FN;

  /* Elapsed flag to be returned to calling client */
  boolean  start_time_elapsed = FALSE;

  /* Ensure that starting time is modulo hyperframe */
  while( start_time_in >= FRAMES_IN_HYPERFRAME )
  {
    start_time_in -= FRAMES_IN_HYPERFRAME;
  }

  /* Make sure the pointer address is valid */
  if( NULL == start_time_out_ptr )
  {
    start_time_out_ptr = &gpl1_gprs_utils_data[gas_id].temp_start_time;
  }

  *start_time_out_ptr = start_time_in;

  /* Check if we received start time from Immed. Assign */
  if(st_mode == CCCH_51)
  {
    /* convert to 42432 format if needed */
    *start_time_out_ptr = (*start_time_out_ptr % PL1_FN_MODULO42432);

    /* Adjust starting time from mod42432 format to absolute FN format */
    start_time_elapsed = gpl1_gprs_adjust_51_starting_time( start_time_out_ptr, gas_id );
  }

  /* May need to check current FN before setting ST = elapsed */
  /* i.e.  has it passed?? */
  l1_transfer_data[gas_id].starting_time_elapsed = FALSE;

  /*  Add check to determine if either of the temporary starting times have elapsed,
  if so, a problem has probably occurred earlier. To allow the MS to recover
  set the offending xx_st to invalid and print out a warning, takes account of
  hyperframe wrap round.  */

  /* Get the current FN value */
  FN = GSTMR_GET_FN_GERAN( gas_id );

  if(   ( l1_transfer_data[gas_id].dl_st != L1_GPRS_INVALID_FN )
        &&( SUB_FN( l1_transfer_data[gas_id].dl_st, FN ) >= PL1_FN_MODULO42432 )
      )
  {
    MSG_GERAN_HIGH_2_G("dl_ST set to invalid: was %d current fn %d",l1_transfer_data[gas_id].dl_st, FN);

    l1_transfer_data[gas_id].dl_st = L1_GPRS_INVALID_FN;
  }

  if(  ( l1_transfer_data[gas_id].ul_st != L1_GPRS_INVALID_FN )
       &&( SUB_FN( l1_transfer_data[gas_id].ul_st, FN ) >= PL1_FN_MODULO42432 )
      )
  {
    MSG_GERAN_HIGH_2_G("ul_ST set to invalid: was %d current fn %d",l1_transfer_data[gas_id].ul_st, FN);

    l1_transfer_data[gas_id].ul_st = L1_GPRS_INVALID_FN;
  }

  /* Align the starting time on a block boundary */
  gpl1_gprs_chk_blk_bndry( start_time_out_ptr );

  if ((l1_transfer_data[gas_id].dl_st == L1_GPRS_INVALID_FN) &&
    (l1_transfer_data[gas_id].ul_st == L1_GPRS_INVALID_FN))
  {
    l1_transfer_data[gas_id].starting_time = *start_time_out_ptr;
  }

  /*  Check that the starting time has not expired  */
  if( SUB_FN( *start_time_out_ptr, FN ) >= PL1_FN_MODULO42432 )
  {
    /*  Set flag and leave any action to calling function */
    start_time_elapsed                   = TRUE;
    l1_transfer_data[gas_id].starting_time_valid = FALSE;

    MSG_GERAN_HIGH_3_G( "STARTTIME %u HAS EXPIRED FN %u",
                  *start_time_out_ptr,
                  FN,
                  0 );
    }
    else
    {
        /* Set starting time present flag */
      l1_transfer_data[gas_id].starting_time_valid = TRUE;

        MSG_GERAN_MED_3_G( "STARTTIME %u has not expired FN %u",
                 *start_time_out_ptr,
                 FN,
                 0 );
    }

    /* Return the value to caller */
    return  start_time_elapsed;
}

/*===========================================================================

FUNCTION gpl1_gprs_adjust_51_starting_time

DESCRIPTION
  This function adjusts the starting time from mod42432 format to the absolute
  FN as specified in (GSM 04.18 10.5.2.38)

DEPENDENCIES
  None

RETURN VALUE
  starting_time_elapsed - true if the starting time has passed
                          false if the starting time is in the future

SIDE EFFECTS
  Locks out the isr while starting time is being calculated


===========================================================================*/
boolean gpl1_gprs_adjust_51_starting_time( uint32*  start_time_ptr, gas_id_t gas_id )
{
    /* local variables */
    uint32   st_minus_fn_mod42432;       /* Used to store the ST - FNmod42432 value */
    boolean  start_time_elapsed = FALSE; /* Used to return ST elapsed decision to caller */

    if( NULL == start_time_ptr )
    {
      MSG_GERAN_ERROR_0_G(" NULL start_time_ptr ");

      return  FALSE;
    }

 GL1_ISR_LOCK(gas_id);

  if( *start_time_ptr < frame_counters[gas_id].FNmod42432 )
  {
        /* Calculate the difference, no need for signed arithmetic */
        st_minus_fn_mod42432 = ( *start_time_ptr + PL1_FN_MODULO42432 ) - frame_counters[gas_id].FNmod42432;

        if( st_minus_fn_mod42432 >= PL1_FN_MODULO42432 )
        {
            st_minus_fn_mod42432 -= PL1_FN_MODULO42432;
        }
  }
  else
  {
        /* Use the modulo operation just incase the start time
         * is not within the 42432 modulo frame range.
         */
        st_minus_fn_mod42432 = ( *start_time_ptr - frame_counters[gas_id].FNmod42432 ) % PL1_FN_MODULO42432;
  }

  if( st_minus_fn_mod42432 <= 31623 )
  {
        /* Set the current frame number */
        uint32  current_fn = GSTMR_GET_FN_GERAN( gas_id );

        /* This covers the range 0 to 31623 (inclusive) */

        /* starting time has NOT elapsed */
        *start_time_ptr = ADD_FN( current_fn, st_minus_fn_mod42432 );
  }
  else
  {
        /* Set the current frame number */
        uint32  current_fn = GSTMR_GET_FN_GERAN( gas_id );

        /* This covers the range 31624 to 32023 (inclusive )
         * and the range 32024 to 42431 ( inclusive ).
         */

        /* Warning: This is valid only when TEMP_DELAY_OS < 2715648 */
        *start_time_ptr = ADD_FN( current_fn, TEMP_DELAY_OS );

        start_time_elapsed = TRUE;
  }

 GL1_ISR_UNLOCK(gas_id);

  /* Return the value to caller */
  return  start_time_elapsed;
}

/*===========================================================================

FUNCTION  GPL1_GPRS_CALCULATE_ARFCNS

DESCRIPTION
  This function calculates 4 hopping frequencies for any type of GPRS radio block.
  There are 3 principal modes of frequency hopping: Normal, PTCCH and PRACH.
  Hence the use of the Mode parameter.  The returns a pointer to an array of 4 freqs.
  This function does the paging parameter calculations as in GSM 05.02
  section 6.5.3.

DEPENDENCIES
  pointer to frequency_information, mode of gprs hopping.

RETURN VALUE
  next frequency (from mobile frequency allocation list)

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_calculate_arfcns(ARFCN_T arfcns[4], frequency_information_T *channel_data_ptr, uint8 mode,
                                 gas_id_t gas_id)
{

  if( NULL != arfcns )
  {
    uint8 loop, offset, ptcch_multi;
    ARFCN_T store;
    ptcch_multi = 0;

    if(mode == PBCCH)
    {
      offset = 2;
      mode = NORM;
    }
    else
    {
      offset = 1;
    }

    if(mode == NORM)
    {
      for(loop = 0; loop < 4; loop++)
      {
        arfcns[loop] = gpl1_gprs_get_ARFCN(channel_data_ptr, offset, gl1_get_FN(gas_id));
        offset++; /* offset = 2,then 3, then 4, then 5 */
      }
    }
    else if(mode == PRACH)
    {
      store = gpl1_gprs_get_ARFCN(channel_data_ptr, offset, gl1_get_FN(gas_id));
      for(loop = 0; loop < 4; loop++)
      {
        arfcns[loop] = store; /* PRACH requires 4 of the same frequencies */
      }
    }
    else if(mode == PTCCH)
    {
      offset = 2; /* Since we schedule PTCCH 2 frames beforehand */

      for(loop = 0; loop < 4; loop++)
      {
        arfcns[loop] = gpl1_gprs_get_ARFCN(channel_data_ptr, offset, gl1_get_FN(gas_id));  /* first time round offset is set to */
        offset = (uint8)((++ptcch_multi * 26) + 2);                                  /* 2 frames in advance for the first */
      }                                                                              /* PTCCH frame, thereafter offset is */
    }                                                                                /* set to 26, 2 * 26 and 3 * 26 for  */
                                                                                     /* for the following 3 PTCCH frames  */

  }
  else
  {
    MSG_GERAN_ERROR_0_G(" NULL arfcns ptr ");
  }

  /*
  ** CgC New CMAPI feature for Tx power, need to log band used for next Tx
  */
  geran_eng_mode_data_write(ENG_MODE_TX_BAND, (void*)&arfcns[0].band, TRUE, gas_id);

}

/*===========================================================================

FUNCTION  GPL1_GPRS_CALCULATE_NCELL_ARFCNS

DESCRIPTION
  Like gpl1_gprs_calculate_arfcns() but takes a frame number argument to
  allow for ncell-serving cell offset.

  Only works for mode==NORM and mode==PBCCH.

DEPENDENCIES
  None

RETURN VALUE
  next frequency (from mobile frequency allocation list)

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_calculate_ncell_arfcns( ARFCN_T                   arfcns[4],
  frequency_information_T *channel_data_ptr,
  uint8                    mode,
  uint32                   ncell_FN )
{

  if( NULL != arfcns )
  {
    uint8 loop, offset;

    if (mode == PBCCH)
    {
      offset = 2;
      mode   = NORM;
    }
    else
    {
      offset = 1;
    }

    if(mode == NORM)
    {
      for(loop = 0; loop < 4; loop++)
      {
        arfcns[loop] = gpl1_gprs_get_ARFCN(channel_data_ptr, offset, ncell_FN);
        offset++; /* offset = 2,then 3, then 4, then 5 */
      }
    }
    else
    {
      MSG_GERAN_ERROR_1("gpl1_gprs_calculate_ncell_arfcns bad mode %d", mode);
    }
  }
  else
  {
    MSG_GERAN_ERROR_0(" NULL arfcns ptr ");
  }
}

/*===========================================================================

FUNCTION  GPL1_GPRS_GET_ARFCN

DESCRIPTION
  This function calculates the next frequency when frequency hopping. The
  algorithm is given in GSM 05.02 section 6.2.3.

DEPENDENCIES
  pointer to channel data, offset

RETURN VALUE
  next_ARFCN (from mobile frequency allocation list)

SIDE EFFECTS
  None

===========================================================================*/
ARFCN_T gpl1_gprs_get_ARFCN(frequency_information_T *channel_data, uint8 offset, uint32 input_FN)
{
  /* Locals */
  ARFCN_T next_ARFCN;
  uint32    N; /* number of mobile allocated frequencies */
  uint32    M; /* intermediate integer defined */
  uint32    Mprime;
  uint32    MAI; /* index to the mobile allocated frequency list */
  uint32    T1R; /* reduced T1 */
  uint32    Tprime;
  uint32    S;
  uint32    i;
  uint32    twotoNBIN;
  uint32    T3;
  uint32    fn;
  uint8     fn_mod_102;
  uint8     fn_mod_104;

  fn         = ADD_FN (input_FN, offset);

  if( NULL != channel_data )
  {
    fn_mod_102 = fn%102;
    fn_mod_104 = fn%104;

    N = channel_data->frequency_list.no_of_items;

    /* If the Number of frequencies in hopping list is 0 or greater than maximum */
    if ( ( N == 0 ) || 
         ( N >= ( sizeof( NBIN ) / sizeof( NBIN[0] ) ) )
       )
    {
      MSG_GERAN_ERROR_1( "none or too many items in freq. description %d",
                 N);

      return NULL_ARFCN;
    }

    if(channel_data->hopping_flag)
    {
      if(channel_data->hsn == 0)
      {
        /* Cyclic hopping */
        MAI = (fn + channel_data->maio) % N;
      }
      else
      {
        /* Pseudo-random hopping */

        T1R = (fn / 1326) % 64;

        /* here, T3 = FN mod 51 */

        T3 = fn_mod_102 % 51;
        i = (channel_data->hsn ^ T1R) + T3;
        M = RNTABLE[i];
        M += (fn_mod_104 % 26);

        twotoNBIN = NBIN[N];

        if (twotoNBIN==0)
        {
          MSG_GERAN_ERROR_0("twotoNBIN == 0");
          return NULL_ARFCN;
        }

        Mprime = M % twotoNBIN;
        Tprime = T3 % twotoNBIN;

        if(Mprime < N)
        {
          S = Mprime;
        }
        else
        {
          S = (Mprime + Tprime) % N ;
        }
        MAI = (S + channel_data->maio) % N;

      }
    }
    else
    {
      /* No hopping */
      MAI = 0;
    }

    next_ARFCN = channel_data->frequency_list.channel[MAI];
  }
  else
  {
    /* Keep LINT happy and assign values */
    next_ARFCN.band = INVALID_BAND;
    next_ARFCN.num = 0xFF;
    MSG_GERAN_ERROR_0( " NULL channel_data ptr - ARFCN.band set to INVALID, ARFCN.num = 256");
  }

  return  next_ARFCN;
}


/*===========================================================================

FUNCTION  GPL1_GPRS_ADJUST_MOD_NUM

DESCRIPTION
  This function takes the input value and applies the change value, a check is
  then made against the modulo number to ensure the result is still in the
  correct range.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
uint32 gpl1_gprs_adjust_mod_num(uint32 input_value, int32 change_value, int32 adjust_mod)
{
  int32 result;

  result = (int32)((int32)input_value + change_value);

  /*  As change_value can be positive or negative check for under flow. */
  if (result < 0)
    result += adjust_mod;

  /*  Determine if still within mod range */
  while( result >= adjust_mod )
  {
    result -= adjust_mod;
  };

  return ((uint32)result);


}

/*===========================================================================

FUNCTION  GPL1_GPRS_GET_PR_VALUE

DESCRIPTION
  This function retrieves the PR value from either an RLC/MAC control block
  or an RLC data block. The retrieved bits are then passed through the
  l1_compute_pr value which converts the value to dBx16_T

DEPENDENCIES

RETURN VALUE
  Pr_value in dBx16_T

SIDE EFFECTS
  None

===========================================================================*/

dBx16_T gpl1_gprs_get_pr_value(uint8 *data_block)
{
  dBx16_T  pr_value = 0;

  if( NULL != data_block )
  {

    /*
    * first check the payload type to work out which radio block we
    * are extracting PR for
    */
    if( ((*data_block & 0xC0)>>6) == 2)
    {
      /* This is an RLC/MAC downlink control block, payload type = 2*/

      /* Check if AC bit (LSB) is present to ascertain if PR value is available in
      * next octet
      */
      if(*(data_block + 1) & 0x1 )
      {
        /* extract PR value */
        pr_value = l1_compute_Pr((*(data_block+2) & 0xC0) >> 6);
        MSG_GERAN_LOW_1("PR rcvd RLC/MAC %d",pr_value);
      }
    }
    else if( !((*data_block & 0xC0)>>6))
    {
      /* This is a RLC downlink data block, payload type = 0 */

      /* extract PR value */
      pr_value = l1_compute_Pr((*(data_block+1) & 0xC0) >> 6);
      MSG_GERAN_LOW_1("PR rcvd RLC %d",pr_value);
    }
    else
    {
      /* PR value not present */
      pr_value = 0;
    }
  }
  else
  {
    MSG_GERAN_ERROR_0(" NULL data_block ptr ");
  }

  return  pr_value;
}

/*===========================================================================

FUNCTION gpl1_gprs_is_frame_num_later

DESCRIPTION
  Determines if one frame number is later than another taking into account
  wrapping of the frame number.


When a > b
  |--------------------------b----------a----------------------|
  |                  |                         |               |
  |                  |<---FN_WRAP_THRESHOLD--->|               |
 a is later than b

  |---------b------------------------------------------a-------|
  |                  |                         |               |
  |                  |<---FN_WRAP_THRESHOLD--->|               |
 a is not later than b


When b > a
  |---------------------a----------b---------------------------|
  |       |                                       |            |
  |       |<-FN_MAX_PLUS_ONE - FN_WRAP_THRESHOLD->|            |
 a is not later than b

  |-----a----------------------------------------------b-------|
  |       |                                       |            |
  |       |<-FN_MAX_PLUS_ONE - FN_WRAP_THRESHOLD->|            |
 a is later than b

  |                                                            |
  |                                                            |
  |                                                            |
  |<---------------------FN_MAX_PLUS_ONE---------------------->|

DEPENDENCIES
  None

RETURN VALUE
  TRUE       fn_a is later than fn_b
  FALSE      fn_a is not later than fn_b

SIDE EFFECTS
  None
===========================================================================*/
boolean gpl1_gprs_is_frame_num_later( uint32 fn_a, uint32 fn_b)
{
  if (fn_a > fn_b)  {
    return (  (fn_a - fn_b) < FN_WRAP_THRESHOLD );
  }
  else {
    return ( (fn_b - fn_a) > (FN_MAX_PLUS_ONE - FN_WRAP_THRESHOLD) );
  }
}


/*===========================================================================

FUNCTION  GPL1_GPRS_SHIFT_OUT_FBI_E

DESCRIPTION
  This function shifts out the FBI and E bits out of a received RLC data block
  and byte alligns the remaining octets in the payload accordingly.

  The

DEPENDENCIES

RETURN VALUE
  void

SIDE EFFECTS
  None

===========================================================================*/

void gpl1_gprs_shift_out_fbi_e(uint8 *data_ptr,uint8 len)
{


  register uint16 count;
  register uint8 *p = (uint8 *) data_ptr;

  if( NULL != data_ptr )
  {
    /* subtract length by 1 octet as msg length does not include E and FBI bits */
    if(len)
    {
      len--;
    }else
    {
      MSG_GERAN_ERROR_1("msg length wrong %d",len);
      return;
    }


    for(count=0;count < len; count++)
    {
      p[count] = ((p[count]>>2) | ( (p[count+1] & 0x03) << 6 ) );
    }
  }
  else
  {
    MSG_GERAN_ERROR_0(" NULL data_ptr ");
  }
}



