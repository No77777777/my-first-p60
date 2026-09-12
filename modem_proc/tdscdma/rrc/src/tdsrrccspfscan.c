/*===========================================================================
             R R C    C S P    F R E Q U E N C Y    S C A N
DESCRIPTION

   This file contains the access routines for Frequency Scan.

   It implements the frequency scan algorithm support in RRC.
   The implemented blocks are marked with a "=======" boundary below.

              CSP PLMN SEARCH MECHANISM
              
       -----------   ------------    -------------    ---------
       |         |   | CSP      |    | Acq DB    |    | Acq   |
       |CSP      |   | Initial  |    | Access    |    | DB    |
       |State    |   | And .    |<-->| Functions |<-->|       |
       |Machine  |   | Stored   |    | -PLMN     |    |       |
       |Functions|<->|Selection |    | -Acq type |    |       |
       |         |   |Controller|    -------------    ---------
       |         |   |-PLMN     |    
       |         |   |-PLMN     |    ============
       -----------   | type     |<-->| CSP      |
            |        ------------    | Frequency|
            |           |            | Scan     |
            |           |            | Algorithm| 
       -----------      |            ============            
       |Internal |   ------------       
       |  Data   |   |Selection |
       |         |   |          |
       -----------   |Data      |
                     ------------

EXTERNALIZED FUNCTIONS   
   
  tdsrrccsp_init_freq_scan_data()
    This function initializes the frequency scan data in Current selection 
    data.

  tdsrrccsp_get_next_freq_scan_entry()
    This function is responsible for determining the next frequency scan type
    and returning the frequency.

  tdsrrccsp_update_freq_scan_entry()
    This function is responsible for updating frequency scan
    data in Current selection data. 


INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrccspfscan.c#1 $    $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/10/11   zz     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53

===========================================================================*/

/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/


/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "tdscdma_variation.h"
#include "comdef.h"
#include "customer.h"
#include "msg.h"
#include "err.h"
#include "tdsuecomdef.h"
#include "task.h"

#include "tdsrrcmm_v.h"
#include "tdsrrcintif.h"
#include "tdsrrccmd_v.h"
#include "tdsl1rrcif.h"
#include "tdsl1mtask.h"
#include "tdsrrcnv.h"

#include "tdsrrccspi.h"
#include "tdsrrcscmgr.h"


#include "tdsrrcccm.h"
#include "tdsrrcmeasi.h"
#include "tdsrrcsibdb.h"


#include "tdsrrcmcm.h"
#ifdef TEST_FRAMEWORK
#error code not present
#endif
#include "tdsrrc_dependancy.h"


/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Global Constant Data Declarations
** ----------------------------------------------------------------------- */
extern tdsrrc_csp_band_f_pattern_type  tdsrrccsp_band_f_pattern;
extern tdsrrc_csp_band_a_pattern_type  tdsrrccsp_band_a_pattern;
extern tdsrrc_csp_band_e_pattern_type  tdsrrccsp_band_e_pattern;
/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Local Object Definitions
** ----------------------------------------------------------------------- */

/* =======================================================================
**                            Function Definitions
** ======================================================================= */
/* Not used, disabled */
#if 0
static boolean tdsrrccsp_freq_in_fs_result
(
   tdsl1_freq_scan_cnf_type * fs_ptr,
   uint16 prev_freq
)
{
   boolean freq_incl = FALSE;
   
   if (fs_ptr)
   {
      uint16 i = 0;
      for (i = 0; i < fs_ptr->num_freq; i++)
      {
         if (fs_ptr->freq_result[i].ctr_freq == prev_freq)
         {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Freq %d already in FS list",prev_freq);
            freq_incl = TRUE;
            break;
         }
      }
   }

   return freq_incl;
}
#endif

/*===========================================================================

FUNCTION          tdsrrccsp_pick_next_raw_freq_for_fine_scan

DESCRIPTION     picks the next frequency from raw scan list in current
                  selection data and returns the frequency for fine scan. It also
                  initializes the fine scan list and sets the type of scan to Fine Scan.
DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None
===========================================================================*/
static void  tdsrrccsp_pick_next_raw_freq_for_fine_scan
(
  tdsrrc_freq_type* ret_freq
)
{
  int16  curr_freq_index = 0;

  tdsrrc_csp_curr_select_data.scan_info.raw_scan.curr_freq_index ++;
  curr_freq_index = tdsrrc_csp_curr_select_data.scan_info.raw_scan.curr_freq_index;
  *ret_freq = tdsrrc_csp_curr_select_data.scan_info.raw_scan.freq_result[curr_freq_index].ctr_freq;
  tdsrrc_csp_curr_select_data.scan_info.fine_scan_completed = FALSE;
  tdsrrc_csp_curr_select_data.scan_info.fine_scan.curr_freq_index = 0;
  tdsrrc_csp_curr_select_data.scan_info.fine_scan.num_freq = 0;
  tdsrrc_csp_curr_select_data.scan_info.type_freq_scan = TDSRRC_CSP_FINE_SCAN;
}

/*===========================================================================

FUNCTION          tdsrrccsp_pick_first_raw_freq_for_fine_scan

DESCRIPTION     picks the first frequency from raw scan list in current
                  selection data and returns the frequency for fine scan. It also
                  initializes the fine scan list and sets the type of scan to Fine Scan.
DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None
===========================================================================*/
static void  tdsrrccsp_pick_first_raw_freq_for_fine_scan
(
  tdsrrc_freq_type* ret_freq
)
{
    tdsrrc_csp_curr_select_data.scan_info.raw_scan.curr_freq_index = -1;
    tdsrrccsp_pick_next_raw_freq_for_fine_scan(ret_freq);
}

/*===========================================================================

FUNCTION          tdsrrccsp_pick_next_fine_freq_for_acq

DESCRIPTION     picks the next frequency from fine scan list in current
                  selection data and returns the frequency for acquisition. It also
                  sets the type of scan to Acquisition scan.
DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None
===========================================================================*/
static void  tdsrrccsp_pick_next_fine_freq_for_acq
(
  tdsrrc_freq_type* ret_freq
)
{
  tdsrrc_csp_curr_select_data.scan_info.fine_scan.curr_freq_index ++;
  *ret_freq = tdsrrc_csp_curr_select_data.scan_info.fine_scan.
    freq_result[tdsrrc_csp_curr_select_data.scan_info.fine_scan.curr_freq_index].ctr_freq;
  tdsrrc_csp_curr_select_data.scan_info.type_freq_scan = TDSRRC_CSP_ACQ_SCAN;
}

/*===========================================================================

FUNCTION          tdsrrccsp_pick_first_fine_freq_for_acq

DESCRIPTION       picks the first frequency from fine scan list in current
                  selection data and returns the frequency for acquisition. It also
                  sets the type of scan to Acquisition scan.
DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None
===========================================================================*/

static void  tdsrrccsp_pick_first_fine_freq_for_acq
(
  tdsrrc_freq_type* ret_freq
)
{
  tdsrrc_csp_curr_select_data.scan_info.fine_scan.curr_freq_index = -1;
  tdsrrccsp_pick_next_fine_freq_for_acq(ret_freq);
}

/*===========================================================================

FUNCTION          tdsrrccsp_pick_next_fine_freq_for_acq_lfs

DESCRIPTION       picks the next frequency from fine scan list in current
                  selection data and returns the frequency for acquisition. It also
                  sets the type of scan to LFS scan.
                  It should not be used except only at the time from fine scan to LFS switch.
DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None
===========================================================================*/
static void  tdsrrccsp_pick_next_fine_freq_for_acq_lfs
(
  tdsrrc_freq_type* ret_freq
)
{
  tdsrrc_csp_curr_select_data.scan_info.fine_scan.curr_freq_index ++;
  *ret_freq = tdsrrc_csp_curr_select_data.scan_info.fine_scan.
              freq_result[tdsrrc_csp_curr_select_data.scan_info.fine_scan.curr_freq_index].ctr_freq;
  tdsrrc_csp_curr_select_data.scan_info.type_freq_scan = TDSRRC_CSP_LIST_SCAN;
}

/*===========================================================================

FUNCTION          tdsrrccsp_pick_first_fine_freq_for_acq

DESCRIPTION     picks the first frequency from fine scan list in current
                  selection data and returns the frequency for acquisition. It also
                  sets the type of scan to Acquisition scan.
DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None
===========================================================================*/
/* ZZTDB: due to off-target hack TDSFEATURE_ONE_FREQ_SCAN
 * it keeps going back to the first fine scan frequency.
 * Will undef it when we do frequency scan */

/*static void  tdsrrccsp_pick_first_fine_freq_for_acq
(
  tdsrrc_freq_type* ret_freq
)
{
  tdsrrc_csp_curr_select_data.scan_info.fine_scan.curr_freq_index = -1;
  tdsrrccsp_pick_next_fine_freq_for_acq_lfs(ret_freq);
}*/


static void tdsrrccsp_fscan_get_freqs_from_fine_scan(void)
{
   int i = 0;
   
   /*Invalidate the curr and scanned indices */
   tdsrrc_csp_curr_select_data.scan_info.list_scan.curr_freq_index = -1;

   tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq = 
      tdsrrc_csp_curr_select_data.scan_info.fine_scan.num_freq;

   for (i = 0; i < tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq; i++)
   {
      tdsrrc_csp_curr_select_data.scan_info.list_scan.freq_result[i] = 
         tdsrrc_csp_curr_select_data.scan_info.fine_scan.freq_result[i];
      tdsrrc_csp_curr_select_data.scan_info.list_scan.source[i]=
                                          Fine_Scan;
      /* ZZTBD: need PLMN type? */
   }
   
   /* Update fine scan index so that 
    * tdsrrccsp_pick_next_fine_freq_for_acq_lfs
    * won't be called for TDD in normal case
    * because LFS takes all fine scan frequencies in one shot
    */
   tdsrrc_csp_curr_select_data.scan_info.fine_scan.curr_freq_index = 
      tdsrrc_csp_curr_select_data.scan_info.fine_scan.num_freq -1;

   tdsrrc_csp_curr_select_data.scan_info.type_freq_scan = TDSRRC_CSP_LIST_SCAN;
}

/*===========================================================================

FUNCTION          RRC_CSP_INIT_FREQ_SCAN_DATA

DESCRIPTION       This function initializes the frequency scan data in
                  Current selection data.
DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None
===========================================================================*/
void  tdsrrccsp_init_freq_scan_data(void)
{
#if !defined(TDSFEATURE_ONE_FREQ_SCAN)
  /* Since we haven't started the scan, initialize it to NO_SCAN */
  tdsrrc_csp_curr_select_data.scan_info.type_freq_scan = TDSRRC_CSP_NO_SCAN;

  /* Set Raw and Fine scan completed booleans to FALSE */
  tdsrrc_csp_curr_select_data.scan_info.raw_scan_completed = FALSE;
  tdsrrc_csp_curr_select_data.scan_info.fine_scan_completed = FALSE;

  /* Current freq. indices in raw and fine scan lists are set to 0 */
  tdsrrc_csp_curr_select_data.scan_info.raw_scan.curr_freq_index = 0;
  tdsrrc_csp_curr_select_data.scan_info.fine_scan.curr_freq_index = 0;

  /* Number of freq in raw and fine scan list are set to 0 */
  tdsrrc_csp_curr_select_data.scan_info.raw_scan.num_freq = 0;
  tdsrrc_csp_curr_select_data.scan_info.fine_scan.num_freq = 0;
  /* If the first band to be scanned has not been scanned yet, 
   * assign it to current band so that a full scan is initiated
   * on it
   */

  if((tdsrrc_csp_curr_select_data.first_band_to_be_scanned & 
       tdsrrc_csp_curr_select_data.next_band_mask)&&
     (tdsrrc_csp_int_data.is_shallow_search_complete == FALSE))
  {
    tdsrrc_csp_curr_select_data.current_band = 
      tdsrrc_csp_curr_select_data.first_band_to_be_scanned;
  }

#else
  /* This is a case for VST build. Initialize it so that only acquisition
   * scan is done without going to Raw and Fine scan.
   */
  tdsrrc_csp_curr_select_data.scan_info.type_freq_scan = TDSRRC_CSP_FINE_SCAN;

  /* Spoof completion of Raw and Fine scans - 
   * Set Raw and Fine scan completed booleans to TRUE
   */
  tdsrrc_csp_curr_select_data.scan_info.raw_scan_completed = TRUE;
  tdsrrc_csp_curr_select_data.scan_info.fine_scan_completed = TRUE;

  /* Current freq. indices in raw and fine scan lists are set to 0 */
  tdsrrc_csp_curr_select_data.scan_info.raw_scan.curr_freq_index = 0;
  tdsrrc_csp_curr_select_data.scan_info.fine_scan.curr_freq_index = 0;

  /* Number of freq in raw and fine scan list are set to 1 */
  tdsrrc_csp_curr_select_data.scan_info.raw_scan.num_freq = 1;
  tdsrrc_csp_curr_select_data.scan_info.fine_scan.num_freq = 1;

  /* Set the raw and fine scan frequencies to default frequencies */
  tdsrrc_csp_curr_select_data.scan_info.raw_scan.
    freq_result[0].ctr_freq = TDSDEFAULT_FREQ;

  tdsrrc_csp_curr_select_data.scan_info.fine_scan.
    freq_result[0].ctr_freq = TDSDEFAULT_FREQ;
#endif

} /* tdsrrccsp_init_freq_scan_data */


/*===========================================================================

FUNCTION          RRCCSP_SHOULD_FREQ_BE_SKIPPED

DESCRIPTION       This function determines based on previous acquired frequencies,
                  whether a new frequency should be scanned or skipped.
                  
DEPENDENCIES
                  None

RETURN VALUE
                  TRUE - if frequency should be skipped
                  FALSE - otherwise

SIDE EFFECTS
                  None
===========================================================================*/
boolean tdsrrccsp_should_freq_be_skipped(tdsrrc_freq_type freq, uint16 resolution)
{
  boolean ret_status = FALSE;
  uint16  i;

  if (TDSCHECK_OPTIONAL_FEATURE_ENABLE_RRC_FREQ_PRUNING)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSRRC freq pruning enabled");
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSRRC freq pruning disabled");
    return ret_status;
  }
  
  /* Don't skip the frequency if the freq passed is the same as last
   * camped frequency and scan type is NOT WTOW BPLMN scan 
   */
  if(   (tdsrrc_get_state() != TDSRRC_STATE_DISCONNECTED) 
     && (freq == tdsrrc_csp_int_data.last_camped_freq_in_connected_mode)
     && (tdsrrc_csp_int_data.curr_scan != RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)
     && (tdsrrc_csp_int_data.curr_scan != RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN))
  {
    return ret_status; 
  }

  for(i=0; i<tdsrrc_csp_curr_select_data.num_available_freq; i++)
  {
    if((freq >= tdsrrc_csp_curr_select_data.available_freq_list[i] - resolution) &&
       (freq <= tdsrrc_csp_curr_select_data.available_freq_list[i] + resolution))
    {
      if(resolution == TDSRRCCSP_RAW_SCAN_SKIP_RANGE)
      {
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"Raw Freq %d skipped,prev_scanned %d",freq,tdsrrc_csp_curr_select_data.available_freq_list[i]);
      }
      else
      {
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"Fine Freq %d skipped,prev_scanned %d",freq,tdsrrc_csp_curr_select_data.available_freq_list[i]);
      }

      return(ret_status = TRUE);
    }
  }
  
#if defined(FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)
  if((tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN) || 
     (tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN) || 
     (tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN) || 
     (tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_MANUAL_SCAN) || 
     (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_PLMN_LIST_SCAN))
  {
    uint16 frequency; /*integral multiple of 100KHz*/
    uint16 idx;
    uint8 lte_resol;
    uint8 gsm_resol;

    frequency = tdsrrccsp_derive_freq_from_uarfcn(freq,tdsrrc_csp_curr_select_data.current_band);

    /* LTETBD: need to verify if TDS freq still follow WCDMA algorithm or need to do this for TDS */
    if(resolution == TDSRRCCSP_RAW_SCAN_SKIP_RANGE)
    {
      lte_resol = TDSRRCCSP_LTE_CHANNEL_RAW_SCAN_SKIP_RANGE;
      gsm_resol = TDSRRCCSP_GSM_CHANNEL_RAW_SCAN_SKIP_RANGE * 2;
    }
    else
    {
      lte_resol = TDSRRCCSP_LTE_CHANNEL_FINE_SCAN_SKIP_RANGE;
      gsm_resol = TDSRRCCSP_GSM_CHANNEL_FINE_SCAN_SKIP_RANGE * 2;
    }
    for(idx = 0;idx<tdsrrc_csp_int_data.detected_frequency_list.num_freqs && idx < LTE_IRAT_MAX_DETECTED_FREQ;idx++)
    {
      if(tdsrrc_csp_int_data.detected_frequency_list.freq[idx].rat == SYS_RAT_LTE_RADIO_ACCESS)
      {
        uint16 bw = tdsrrc_csp_int_data.detected_frequency_list.freq[idx].bandwidth;
        if((frequency >= tdsrrc_csp_int_data.detected_frequency_list.freq[idx].frequency - (bw/2+lte_resol))&&
           (frequency <= tdsrrc_csp_int_data.detected_frequency_list.freq[idx].frequency + (bw/2+lte_resol)))
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Skip TDSCDMA ARFCN %d ,frequency in 100KHz units %d",freq,frequency);
          return (ret_status = TRUE);
        }
      }
      else if(tdsrrc_csp_int_data.detected_frequency_list.freq[idx].rat == SYS_RAT_GSM_RADIO_ACCESS)
      {
        if((frequency >= tdsrrc_csp_int_data.detected_frequency_list.freq[idx].frequency - gsm_resol)&&
           (frequency <= tdsrrc_csp_int_data.detected_frequency_list.freq[idx].frequency + gsm_resol))
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Skip TDSCDNA ARFCN %d ,frequency in 100KHz units %d",freq,frequency);
          return (ret_status = TRUE);
        }
      }
    }
  }
#endif


  return ret_status;

} /* tdsrrccsp_should_freq_be_skipped */
/*===========================================================================

FUNCTION          TDSRRCCSP_PRIMIARY_FREQ

DESCRIPTION       This function determines whether the frequency is in 
                  primiary frequency list
                  
DEPENDENCIES
                  None

RETURN VALUE
                  TRUE - frequency in primiary frequency list
                  FALSE - otherwise

SIDE EFFECTS
                  None
===========================================================================*/
boolean tdsrrccsp_is_primiary_freq(tdsrrc_freq_type freq)
{
   int i = 0;
   for (i = 0; i < tdsrrc_csp_int_data.num_tds_prim_freq; i++)
   {
     if (tdsrrc_csp_int_data.tds_prim_freq_list[i] == freq)
     {
	    TDSRRC_MSG1(MSG_LEGACY_HIGH,"freq %d is in primiary list",freq);
        return TRUE;
     }
   }
   TDSRRC_MSG1(MSG_LEGACY_HIGH,"freq %d is not in primiary list",freq);
   return FALSE;
}

/*===========================================================================

FUNCTION          RRCCSP_IS_FINE_SCAN_REQUIRED

DESCRIPTION       This function determines if a fine scan is required or not.
                  It loops through the fine scan results and skips the result,
                  if it falls within the RAW_SCAN_SKIP_RANGE. 
                  
DEPENDENCIES
                  None

RETURN VALUE
                  TRUE if Fine scan is required,
                  FALSE otherwise

SIDE EFFECTS
                  None
===========================================================================*/
static boolean tdsrrccsp_is_fine_scan_required(tdsrrc_freq_type* ret_freq)
{
  boolean fine_scan_required = TRUE;

  /* There are no more frequencies in fine scan or we've covered all of 
   * them for acquisition - pick the next freq. in raw scan list for fine scan
   */
  if((tdsrrc_csp_curr_select_data.scan_info.raw_scan.num_freq > 0) &&
     (tdsrrc_csp_curr_select_data.scan_info.raw_scan.curr_freq_index <
        tdsrrc_csp_curr_select_data.scan_info.raw_scan.num_freq -1))
  {
    /* There is atleast one more freq in raw scan list that can be tried for
     * fine scan.
     */
    tdsrrccsp_pick_next_raw_freq_for_fine_scan(ret_freq);
  }
  else
  {
    return FALSE;
  }

  while(tdsrrccsp_should_freq_be_skipped(*ret_freq, TDSRRCCSP_RAW_SCAN_SKIP_RANGE))
  {
    if((tdsrrc_csp_curr_select_data.scan_info.raw_scan.curr_freq_index <
       tdsrrc_csp_curr_select_data.scan_info.raw_scan.num_freq -1))
    {
      tdsrrccsp_pick_next_raw_freq_for_fine_scan(ret_freq);
    }
    else
    {
      fine_scan_required = FALSE;
      break;
    }
  }

  return fine_scan_required;

} /* tdsrrccsp_is_fine_scan_required */


/*===========================================================================

FUNCTION          RRCCSP_IS_ACQ_SCAN_REQUIRED

DESCRIPTION       This function determines if an acquisition is required or not.
                  It loops through the fine scan results and skips the result,
                  if it falls within the FINE_SCAN_SKIP_RANGE. 
                  
DEPENDENCIES
                  None

RETURN VALUE
                  TRUE if Acquisition is required,
                  FALSE otherwise

SIDE EFFECTS
                  None
===========================================================================*/
static boolean tdsrrccsp_is_acq_scan_required(tdsrrc_freq_type* ret_freq)
{
  boolean acq_scan_required = TRUE;

  while(tdsrrccsp_should_freq_be_skipped(*ret_freq, TDSRRCCSP_FINE_SCAN_SKIP_RANGE))
  {
    if((tdsrrc_csp_curr_select_data.scan_info.fine_scan.curr_freq_index <
       tdsrrc_csp_curr_select_data.scan_info.fine_scan.num_freq -1))
    {
      
      if((TD2TD_SEARCH_STOP != tdsrrccsp_bplmn_srch_in_progress()) ||
         (tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN) ||
         (tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_MANUAL_SCAN))
      {
        /* Do ACQ scan for BPLMN to make sure it doesn't take too long time to miss paging */
        tdsrrccsp_pick_next_fine_freq_for_acq(ret_freq);
      }
      else
      
      {
        tdsrrccsp_pick_next_fine_freq_for_acq_lfs(ret_freq);
      }
    }
    else
    {
      acq_scan_required = FALSE;
      break;
    }
  }

  return acq_scan_required;

} /* tdsrrccsp_is_acq_scan_required */



/*===========================================================================

FUNCTION          RRC_CSP_GET_NEXT_FREQ_SCAN_ENTRY

DESCRIPTION       This function is responsible for determining the next
                  frequency scan type and returning the frequency.
                  
DEPENDENCIES
                  None

RETURN VALUE
                  SUCCESS: Able to find a frequency where a Raw or Fine scan
                           or an Acquisition can be tried.
                  FAILURE: All frequencies exhausted.

SIDE EFFECTS
                  None
===========================================================================*/
uecomdef_status_e_type
tdsrrccsp_get_next_freq_scan_entry(tdsrrc_freq_type *ret_freq)
{
  uecomdef_status_e_type  return_value = FAILURE;

#ifdef FEATURE_TDSCDMA_SCAN_SCOPE
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"srvc_req_scan_scope: %d",tdsrrc_csp_int_data.srvc_req_scan_scope);
#endif

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"ue_in_deep_sleep: %d,full_scan_needed: %d",tdsrrc_csp_int_data.ue_in_deep_sleep,tdsrrc_csp_int_data.full_scan_needed);
  
  TDSRRC_MSG3(MSG_LEGACY_HIGH,"full_scan_in_deep_sleep_counter: %d, max_full_scan_in_deep_sleep: %d, freq scan tmr running: %d",
							   tdsrrc_csp_int_data.full_scan_in_deep_sleep_counter, TDSRRC_MAX_FULL_SCANS_IN_DEEP_SLEEP,
  	                           (tdsrrctmr_get_remaining_time(TDSRRCTMR_FREQ_SCAN_TIMER)!=0));

  if(tdsrrc_csp_curr_select_data.acq_db_in_use)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Frequency scan not active");
    return(FAILURE);
  }



  switch(tdsrrc_csp_curr_select_data.scan_info.type_freq_scan)
  {
    case TDSRRC_CSP_NO_SCAN:
    {
      /* This is the begining of frequency scan. Start it with a Raw scan of the
       * entire band. Always start frequency scan for certain scan types even if
       * the frequency scan timer hasn't expired
       */
      if((tdsrrc_csp_int_data.ue_in_deep_sleep) && 
          (!tdsrrc_csp_int_data.full_scan_needed) && 
            (tdsrrc_csp_int_data.curr_scan != TDSRRC_CSP_PLMN_LIST_SCAN) &&
             (tdsrrc_csp_int_data.curr_scan != RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN) &&
              (tdsrrc_csp_int_data.curr_scan != RRC_CSP_G2TD_BPLMN_MANUAL_SCAN) &&
                (((tdsrrc_csp_int_data.deep_sleep_band_scanned_mask & tdsrrc_csp_int_data.tds_band_mask)
                                                              ^ tdsrrc_csp_int_data.tds_band_mask) == 0)
#ifdef FEATURE_WRLF_SYSTEM_SEL
                 && (!TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS())
#endif
                 && (tdsrrc_csp_int_data.curr_scan != RRC_CSP_GSM_TO_TDS_REDIRECTION_SCAN)
        )
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Full scan not needed,Scan type %d",tdsrrc_csp_int_data.curr_scan);
        return_value = FAILURE;
      }
      /* Same timer is used for both automatic and manual searches. So changing the timer value will affect 
        * both the behaviors.
        */
      else if(((tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN) 
               || (tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)) 
              && (!tdsrrc_csp_int_data.plmn_list_freq_scan_needed)
              && (!tdsrrc_csp_int_data.acq_db_empty))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"BPLMN/WTOW full FreqScan not needed,Scan type %d",tdsrrc_csp_int_data.curr_scan);
        return_value = FAILURE;
      }   
      else
      {
        if (tdsrrc_csp_int_data.acq_fs_status == TDSL1_ACQ_FS_NONE)
        {
           TDSRRC_MSG0(MSG_LEGACY_HIGH,"Change acq_fs_status from TDSL1_ACQ_FS_NONE to TDSL1_ACQ_FS_START");
           tdsrrc_csp_int_data.acq_fs_status = TDSL1_ACQ_FS_START;
        }
        tdsrrc_csp_curr_select_data.scan_info.type_freq_scan = TDSRRC_CSP_RAW_SCAN;

        switch (tdsrrc_csp_curr_select_data.current_band)
        {
           case RRC_CSP_TDS_A_BAND:
              if (!(((tdsrrc_csp_int_data.is_shallow_search_complete == FALSE)
                   && (tdsrrccsp_band_a_pattern.band_shallow == TRUE))
                 ||((tdsrrc_csp_int_data.is_shallow_search_complete == TRUE)
                   &&(tdsrrccsp_band_a_pattern.band_deep == TRUE))))
              {
                 if((tdsrrc_csp_curr_select_data.current_band = 
                    tdsrrccsp_get_next_band_to_be_scanned()) == TDSRRC_CSP_BAND_MAX)
                 {
                   return(FAILURE);
                 }
              }
              break;  
           
           case RRC_CSP_TDS_F_UPPER_BAND:
              if (!(((tdsrrc_csp_int_data.is_shallow_search_complete == FALSE)
                   && (tdsrrccsp_band_f_pattern.upper_band_shallow == TRUE))
                 ||((tdsrrc_csp_int_data.is_shallow_search_complete == TRUE)
                   &&(tdsrrccsp_band_f_pattern.upper_band_deep == TRUE))))
              {
                 if((tdsrrc_csp_curr_select_data.current_band = 
                    tdsrrccsp_get_next_band_to_be_scanned()) == TDSRRC_CSP_BAND_MAX)
                 {
                   return(FAILURE);
                 }
              }
              break;          

           case RRC_CSP_TDS_F_LOWER_BAND:
              if (!(((tdsrrc_csp_int_data.is_shallow_search_complete == FALSE)
                   && (tdsrrccsp_band_f_pattern.lower_band_shallow == TRUE))
                 ||((tdsrrc_csp_int_data.is_shallow_search_complete == TRUE)
                   &&(tdsrrccsp_band_f_pattern.lower_band_deep == TRUE))))
              {
                 if((tdsrrc_csp_curr_select_data.current_band = 
                    tdsrrccsp_get_next_band_to_be_scanned()) == TDSRRC_CSP_BAND_MAX)
                 {
                   return(FAILURE);
                 }
              }
              break;
           case RRC_CSP_TDS_E_BAND:
              if (!(((tdsrrc_csp_int_data.is_shallow_search_complete == FALSE)
                   && (tdsrrccsp_band_e_pattern.band_shallow == TRUE))
                 ||((tdsrrc_csp_int_data.is_shallow_search_complete == TRUE)
                   &&(tdsrrccsp_band_e_pattern.band_deep== TRUE))))
              {
                 if((tdsrrc_csp_curr_select_data.current_band = 
                    tdsrrccsp_get_next_band_to_be_scanned()) == TDSRRC_CSP_BAND_MAX)
                 {
                   return(FAILURE);
                 }
              }
              break;  

           default:
              break;        
        }

        if (tdsrrc_csp_int_data.is_shallow_search_complete == FALSE)
        {
           tdsrrc_csp_int_data.acq_mode = TDSL1_ACQ_MODE_SHALLOW;
        }
        else
        {
          tdsrrc_csp_int_data.acq_mode = TDSL1_ACQ_MODE_DEEP;
        }

        /* Init Raw scan data */
        tdsrrc_csp_curr_select_data.scan_info.raw_scan_completed = FALSE;

        /* Restore  prioritized_bands_read_idx when split no trm lock occurred in raw scan */
        if(tdsrrc_csp_int_data.is_prioritized_band_used_for_freq_scan &&
           (tdsrrc_csp_int_data.is_no_trm_lock_for_raw_scan) &&
           (tdsrrc_csp_curr_select_data.prioritized_bands_read_idx < tdsrrc_csp_curr_select_data.num_of_prioritized_bands))
        {
           tdsrrc_csp_curr_select_data.prioritized_bands_read_idx++;
        }

#if !defined(TDSFEATURE_ONE_FREQ_SCAN)

        if(tdsrrc_csp_curr_select_data.current_band == RRC_CSP_TDS_A_BAND)
        {
          *ret_freq = TDSL1_BAND_A_CHAN_MIN;
        }
        else if(tdsrrc_csp_curr_select_data.current_band == RRC_CSP_TDS_F_LOWER_BAND)
        {
          *ret_freq = TDSL1_BAND_F_CHAN_MIN;
        }
        else if(tdsrrc_csp_curr_select_data.current_band == RRC_CSP_TDS_F_UPPER_BAND)
        {
          *ret_freq = TDSL1_BAND_F_CHAN_FS_UPPER_MIN;
        }
        else if(tdsrrc_csp_curr_select_data.current_band == RRC_CSP_TDS_E_BAND)
        {
          *ret_freq = tdsrrccsp_get_band_e_lower_bound();
        }
        else
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid current band 0x%x",tdsrrc_csp_curr_select_data.current_band);
          return(FAILURE);
        }

        /* Use sub band for BPLMN only */
        if((TD2TD_SEARCH_STOP != tdsrrccsp_bplmn_srch_in_progress() &&
           ((tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN) ||
           (tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN))) ||
           (tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN) ||
           (tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_MANUAL_SCAN))
        {
          *ret_freq += tdsrrc_csp_curr_select_data.current_sub_band * TDSRRC_CSP_SUB_BAND_GAP;
        }

        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Setting the start_freq for raw scan to %d",*ret_freq);

#else

        *ret_freq = TDSDEFAULT_FREQ;
        
#endif /* !defined(TDSFEATURE_ONE_FREQ_SCAN) */
        
        return_value = SUCCESS;
      }
    }
    break;

    case TDSRRC_CSP_RAW_SCAN:
    {
      if(tdsrrc_csp_curr_select_data.scan_info.raw_scan_completed == FALSE)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Wrong call to fn: Raw scan not finished");
        return(FAILURE);
      }
      
      /* Raw Scan has been completed. If there are non-zero entries in Raw list,
       * start the fine scan with the strongest raw freq. entry. If there are no 
       * entries in raw scan list, we are pretty much doomed - Return a failure.
       */
      if(tdsrrc_csp_curr_select_data.scan_info.raw_scan.num_freq == 0)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Freq scan - no more frequency");
        return(FAILURE);
      }
      else  /* Raw scan got some frequencies */
      {
        /* We got some good frequencies in raw scan - proceed to fine scan */
        tdsrrccsp_pick_first_raw_freq_for_fine_scan(ret_freq);
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Starting fine scan on UARFCN %d",*ret_freq);
        return_value = SUCCESS;
      }
    }   /* In Raw Scan */
    break;

    case TDSRRC_CSP_FINE_SCAN:
    {
      if(tdsrrc_csp_curr_select_data.scan_info.fine_scan_completed == FALSE)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Wrong call to fn: Fine scan not finished");
        return(FAILURE);
      }
      /* If there are any frequencies in fine scan list that have not been
       * tried for acquisition, get the next fine scan frequency and return it
       * to be tried for acquisition. 
       */
      if(tdsrrc_csp_curr_select_data.scan_info.fine_scan.num_freq > 0)
      {
        
        if((TD2TD_SEARCH_STOP != tdsrrccsp_bplmn_srch_in_progress()) ||
           (tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN) ||
           (tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_MANUAL_SCAN))
        {
          tdsrrccsp_pick_first_fine_freq_for_acq(ret_freq);
          
          if(tdsrrccsp_is_acq_scan_required(ret_freq))
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Starting acq on UARFCN %d",*ret_freq);

            return_value = SUCCESS;
          }
          else
          {
            if(tdsrrccsp_is_fine_scan_required(ret_freq))
            {
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"Starting fine scan on UARFCN %d",*ret_freq);
              return_value = SUCCESS;
            }
            else
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Freq scan - no more frequency");
              return(FAILURE);
            }
          }
        }
        else
        
        {
          tdsrrccsp_fscan_get_freqs_from_fine_scan();
          return_value = SUCCESS;
        }
      }
      else
      {
        /* There are no more frequencies in fine scan or we've covered all of 
         * them for acquisition - pick the next freq. in raw scan list for fine scan
         */
        if(tdsrrccsp_is_fine_scan_required(ret_freq))
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Starting fine scan on UARFCN %d",*ret_freq);
          return_value = SUCCESS;
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Freq scan - no more frequency");
          return(FAILURE);
        }
      }   /* No more freq for acq in fine scan list */
    } /* In Fine Scan */
    break;

    case TDSRRC_CSP_ACQ_SCAN:
    {
      /* Move on to the next frequency in fine scan list and try acquisition there. If 
       * there is no more frequency in fine scan list to be tried, pick the next one 
       * from raw scan list and try a fine scan there. If raw scan does not have a 
       * frequency either, return a failure.
       */

      if((tdsrrc_csp_curr_select_data.scan_info.fine_scan.num_freq > 0) &&
         (tdsrrc_csp_curr_select_data.scan_info.fine_scan.curr_freq_index < 
          tdsrrc_csp_curr_select_data.scan_info.fine_scan.num_freq -1))
      {
        /* Pick the next fine scan frequency and try acquisition there */
        tdsrrccsp_pick_next_fine_freq_for_acq(ret_freq);

        if(tdsrrccsp_is_acq_scan_required(ret_freq))
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Starting acq on UARFCN %d",*ret_freq);


          return_value = SUCCESS;
        }
        else
        {
          if(tdsrrccsp_is_fine_scan_required(ret_freq))
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Starting fine scan on UARFCN %d",*ret_freq);
            return_value = SUCCESS;
          }
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Freq scan - no more frequency");
            return(FAILURE);
          }
        }   /* No more freq for acq in fine scan list */
      }
      else
      {
        if(tdsrrccsp_is_fine_scan_required(ret_freq))
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Starting fine scan on UARFCN %d",*ret_freq);
          return_value = SUCCESS;
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Freq scan - no more frequency");
          return(FAILURE);
        }
      }

    } /* In Acq scan */
    break;

    /* TDD ACQ should come here */
    case TDSRRC_CSP_LIST_SCAN:
      /* ZZTBD: This condition assumes no repeating freq in the LFS freq list
       * Needs further investigation to void repeating freq from fine scan result */
      if((tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq > 0) &&
         (tdsrrc_csp_curr_select_data.scan_info.list_scan.curr_freq_index < 
          tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq -1))
      {
        tdsrrc_csp_curr_select_data.scan_info.list_scan.curr_freq_index++;
        return_value = SUCCESS;

        TDSRRC_MSG2(MSG_LEGACY_HIGH,"LFS curr_freq_index set to %d of total %d freqs",tdsrrc_csp_curr_select_data.scan_info.list_scan.curr_freq_index,tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq);

#if 0 /* Should not apply to TDS because of LFS always */        
        if(tdsrrccsp_is_acq_scan_required(ret_freq))
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Starting acq on UARFCN %d",*ret_freq);


          return_value = SUCCESS;
        }
        else
        {
          if(tdsrrccsp_is_fine_scan_required(ret_freq))
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Starting fine scan on UARFCN %d",*ret_freq);
            return_value = SUCCESS;
          }
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Freq scan - no more frequency");
            return(FAILURE);
          }
        }   /* No more freq for acq in fine scan list */
#endif /* #if 0 */
      }
      else
      {
        /* Pick the next one from raw scan list and try a fine scan there. 
         * If raw scan does not have a frequency either, return a failure.
         */
        if(tdsrrccsp_is_fine_scan_required(ret_freq))
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Starting fine scan on UARFCN %d",*ret_freq);
          return_value = SUCCESS;
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Freq scan - no more frequency");
          return(FAILURE);
        }  
      }
      break;
      
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Incorrect scan type %d",tdsrrc_csp_curr_select_data.scan_info.type_freq_scan);
      return(FAILURE);

  } /* switch type_freq_scan */

  return(return_value);

} /* tdsrrccsp_get_next_freq_scan_entry */


/*===========================================================================

FUNCTION          RRC_CSP_UPDATE_FREQ_SCAN_ENTRY

DESCRIPTION       This function is responsible for updating frequency scan
                  data in current selection data and list frequency scan
                  data. Note that only the raw frequency scan list, fine frequency 
                  scan list and list frequency scan list need to be updated.

DEPENDENCIES
                  The L1 command data has to be validated before calling
                  this function.

RETURN VALUE
                  SUCCESS: If received scan type matches with the expected
                           scan type.
                  FAILURE: Discrepency between received and expected scan 
                           types.
SIDE EFFECTS
                  None
===========================================================================*/
uecomdef_status_e_type
tdsrrccsp_update_freq_scan_entry(tdsl1_freq_scan_cnf_type *freq_scan_cnf,tdsrrc_csp_freq_scan_e_type type_freq_scan)
{
  uint32 i;
  uint16 num_freq = 0;
  //tdsrrc_state_e_type rrc_state = tdsrrc_get_state();
  tdsrrc_state_e_type rrc_current_state;
  /* comment out until these variables are implemented */
  /*uint32    p,q,r;
  uint32    list_db_ix = 0;*/

  /* comment out until these variables are implemented */
  /*uint16  j,k;
  uint8 temp_plmn;
  uint16 temp_freq;
  rrc_plmn_identity_type temp_plmn_id;
  int spec_plmn_count;*/

  if((type_freq_scan == TDSRRC_CSP_RAW_SCAN) && 
     (freq_scan_cnf->step == TDSL1_FREQ_RAW_SCAN))
  {
    /* Update the Raw Scan list */
    tdsrrc_csp_curr_select_data.scan_info.raw_scan_completed = TRUE;
    if(TDSL1_FAILED_NO_TRM_LOCK == freq_scan_cnf->fail_type)
    { 
      /* avoid fine scan later, continue to do raw scan */
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"ignore num_freq %d in RScan Cnf",freq_scan_cnf->num_freq); 
      tdsrrc_csp_curr_select_data.scan_info.raw_scan.num_freq = 0;
    }
    else
    {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Num freq in RScanCnf %d",freq_scan_cnf->num_freq);

    /*lint -e506*/
    if(freq_scan_cnf->num_freq > TDSL1_MAX_RAW_SCAN_FREQ)
    /*lint +e506*/
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid num_freq %d in RScan Cnf",freq_scan_cnf->num_freq); 
      tdsrrc_csp_curr_select_data.scan_info.raw_scan.num_freq = 0;
    }
    else
    {
      /*lint -e506*/
      tdsrrc_csp_curr_select_data.scan_info.raw_scan.num_freq = 
        MIN(freq_scan_cnf->num_freq, TDSL1_MAX_RAW_SCAN_FREQ);
      /*lint +e506*/

      for(i=0; i < tdsrrc_csp_curr_select_data.scan_info.raw_scan.num_freq; i++)        
      {
        tdsrrc_csp_curr_select_data.scan_info.raw_scan.freq_result[i] = freq_scan_cnf->freq_result[i];
          TDSRRC_MSG3(MSG_LEGACY_MED,"RAW [%d], freq = %d, rssi = %d",i,freq_scan_cnf->freq_result[i].ctr_freq,
                  freq_scan_cnf->freq_result[i].rssi);
      } /* for (...) */
      }
    }
    return(SUCCESS);
  }

  if((type_freq_scan == TDSRRC_CSP_FINE_SCAN) && 
     /*lint -e506*/
     (freq_scan_cnf->step == TDSL1_FREQ_FINE_SCAN))
     /*lint +e506*/
  {
    /* Update the Fine Scan list */
    tdsrrc_csp_curr_select_data.scan_info.fine_scan_completed = TRUE;
    rrc_current_state = tdsrrc_get_state();
    
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Num_freq %d in FnScan Cnf",freq_scan_cnf->num_freq); 

    /* If fine scan result is not zero, Shallow acquisition should be done for this band,
     * then set deep_acq_needed to do Deep acquisition later in case no result in this
     * Shallow acquisition */
    if((tdsrrc_csp_int_data.acq_mode == TDSL1_ACQ_MODE_SHALLOW) &&
       (tdsrrc_csp_int_data.deep_acq_needed == FALSE) &&
       (freq_scan_cnf->num_freq > 0))
    {
      tdsrrc_csp_int_data.deep_acq_needed = TRUE;
    }

    /* TDS limits fine scan result to maximum 10 for next ACQ */
    /* No need to do this because we get num_freq under control */
    /*
    if (freq_scan_cnf->num_freq >= TDSL1_MAX_ACQ_CHANNEL_NUM)
    {
      freq_scan_cnf->num_freq = TDSL1_MAX_ACQ_CHANNEL_NUM;
    }
    */

    #if 0
    /* ZZTBD: JIRA TDS-261 need to include previous camped cell if it is reject redirect */

    /* In Connected mode, UE needs to do additional acquisitions on
     * the frequency it was camped on last, so we are adding it at 
     * the end of the fine scan list.
     */
    if(    (rrc_state != TDSRRC_STATE_DISCONNECTED)
        && (tdsrrc_csp_int_data.curr_scan != RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)
        && (tdsrrc_csp_int_data.curr_scan != RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN)
        && (tdsrrccsp_freq_in_fs_result(freq_scan_cnf, tdsrrc_csp_int_data.last_camped_freq_in_connected_mode) == FALSE))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Add freq %d,rssi -146 to fine scan list",tdsrrc_csp_int_data.last_camped_freq_in_connected_mode);
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Curr_scan %d,RRC state %d",tdsrrc_csp_int_data.curr_scan,rrc_state);

      if (freq_scan_cnf->num_freq < TDSL1_MAX_ACQ_CHANNEL_NUM)
         freq_scan_cnf->num_freq++;
      /* Otherwise replace the last freq in the list */
      
      tdsrrc_csp_curr_select_data.scan_info.fine_scan.num_freq =
          freq_scan_cnf->num_freq;
      freq_scan_cnf->freq_result[freq_scan_cnf->num_freq - 1].ctr_freq =
        tdsrrc_csp_int_data.last_camped_freq_in_connected_mode;
      freq_scan_cnf->freq_result[freq_scan_cnf->num_freq - 1].rssi =  -146;
    }
    /* Add last camped frequency to every fine scan list if RAW scan
     * is NOT due to Manual or Automatic PLMN List Search; and also
     * NOT due to Inter Frequency Redirection */
    else if(   (tdsrrc_csp_int_data.curr_scan != TDSRRC_CSP_PLMN_LIST_SCAN)
            && (tdsrrc_csp_int_data.curr_scan != RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN)
            && (tdsrrc_csp_int_data.curr_scan != RRC_CSP_G2TD_BPLMN_MANUAL_SCAN)
            && (tdsrrc_csp_int_data.curr_scan != RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)
            && (tdsrrc_csp_int_data.curr_scan != RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN)
            && (tdsrrc_csp_int_data.curr_scan != TDSRRC_CSP_INTER_FREQ_REDIRECTION_SCAN)
            && (SUCCESS == tdsrrc_validate_dl_arfcn(tdsrrc_csp_int_data.last_camped_freq_in_idle_mode
                                             , FALSE
                                            )
               )
            && (tdsrrccsp_freq_in_fs_result(freq_scan_cnf, tdsrrc_csp_int_data.last_camped_freq_in_idle_mode) == FALSE)
           )
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Idle mode: Add freq %d,rssi -146 to fine scan list",tdsrrc_csp_int_data.last_camped_freq_in_idle_mode);
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Curr_scan %d,RRC state %d",tdsrrc_csp_int_data.curr_scan,rrc_state);

      if (freq_scan_cnf->num_freq < TDSL1_MAX_ACQ_CHANNEL_NUM)
         freq_scan_cnf->num_freq++;
      /* Otherwise replace the last freq in the list */
      
      tdsrrc_csp_curr_select_data.scan_info.fine_scan.num_freq = freq_scan_cnf->num_freq;
      freq_scan_cnf->freq_result[freq_scan_cnf->num_freq - 1].ctr_freq =
                                       tdsrrc_csp_int_data.last_camped_freq_in_idle_mode;
      freq_scan_cnf->freq_result[freq_scan_cnf->num_freq - 1].rssi = -146;
    }
    #endif

    /*lint +e506*/

    for(i = 0; (i < freq_scan_cnf->num_freq)&&(num_freq < TDSL1_MAX_FINE_SCAN_FREQ); i++)      
    {
      if(!tdsrrccsp_should_freq_be_skipped(freq_scan_cnf->freq_result[i].ctr_freq, TDSRRCCSP_FINE_SCAN_SKIP_RANGE))
      {
        /* When inter frequency redirection scan is in progress, 
        * UE should not attempt acquisition on the last camped frequency till
        * the expiry of the wait timer. 
        */
        if((rrc_current_state == TDSRRC_STATE_CONNECTING) 
           && (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_INTER_FREQ_REDIRECTION_SCAN)
           && (tdsrrc_csp_int_data.start_acq_on_last_camped_freq == FALSE) 
           && (tdsrrc_csp_int_data.last_camped_freq == freq_scan_cnf->freq_result[i].ctr_freq))
        {
           TDSRRC_MSG1(MSG_LEGACY_HIGH,"freq redirect: Do not attempt acq on last camped freq %d during freq scan",tdsrrc_csp_int_data.last_camped_freq);
           continue;
        }
        
        if (((tdsrrc_csp_int_data.curr_scan != RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)
             && (tdsrrc_csp_int_data.curr_scan != RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN))
           || (((tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)
               || (tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN))
                 && (tdsrrccsp_is_primiary_freq(freq_scan_cnf->freq_result[i].ctr_freq) == TRUE)))
        {
          tdsrrc_csp_curr_select_data.scan_info.fine_scan.freq_result[num_freq] = freq_scan_cnf->freq_result[i];
          TDSRRC_MSG4(MSG_LEGACY_HIGH,"Fine [%d], freq = %d, rssi = %d, cnf idx = %d",
                 num_freq,
                 freq_scan_cnf->freq_result[i].ctr_freq,
                 freq_scan_cnf->freq_result[i].rssi,
                 i);

          num_freq++;
        }
	  }
    } /* for (...) */
    
    tdsrrc_csp_curr_select_data.scan_info.fine_scan.num_freq = num_freq;


    return(SUCCESS);
  }

  TDSRRC_MSG2(MSG_LEGACY_ERROR,"Invalid type curr scan %d or L1 scan %d",type_freq_scan,freq_scan_cnf->step); 
  return(FAILURE);
}   /* tdsrrccsp_update_freq_scan_entry */

/*===========================================================================

FUNCTION          TDSRRCCSP_GET_BAND_E_LOWER_BOUND

DESCRIPTION       This function get the floor limit of band E considering
                  NV setting.
                  Three NV bits to control the range of band E to search:
                  1. If TDSCHECK_OPTIONAL_FEATURE_SUBBAND_E1_SET is set,
                     then the lower bound of Band E is 2320MHz;
                  2. else if TDSCHECK_OPTIONAL_FEATURE_SUBBAND_E2_SET is set,
                     then the lower bound of Band E is 2330MHz;
                  3. else if TDSCHECK_OPTIONAL_FEATURE_SUBBAND_E3_SET is set,
                     then the lower bound of Band E is 2350MHz;
                  4. else search whole band, so start from 2300MHz;

RETURN VALUE
                  floor limit frequency
SIDE EFFECTS
                  None
===========================================================================*/
uint16 tdsrrccsp_get_band_e_lower_bound(void)
{
  uint16 lower_bound;

  if(TDSCHECK_OPTIONAL_FEATURE_SUBBAND_E1_SET)
  {
    lower_bound = 11604; /* 2320MHz */
  }
  else if(TDSCHECK_OPTIONAL_FEATURE_SUBBAND_E2_SET)
  {
    lower_bound = 11654; /* 2330MHz */
  }
  else if(TDSCHECK_OPTIONAL_FEATURE_SUBBAND_E3_SET)
  {
    lower_bound = 11754; /* 2350MHz */
  }
  else
  {
    lower_bound = 11504; /* 2300MHz */
  }

  return lower_bound;
}

/*===========================================================================

FUNCTION          TDSRRCCSP_GET_BAND_E_UPPER_BOUND

DESCRIPTION       This function get the upper limit of band E considering
                  NV setting.
                  Three NV bits to control the range of band E to search:
                  1. If TDSCHECK_OPTIONAL_FEATURE_SUBBAND_E3_SET is set,
                     then the upper bound of Band E is 2370MHz;
                  2. else if TDSCHECK_OPTIONAL_FEATURE_SUBBAND_E2_SET is set,
                     then the upper bound of Band E is 2350MHz;
                  3. else if TDSCHECK_OPTIONAL_FEATURE_SUBBAND_E1_SET is set,
                     then the upper bound of Band E is 2330MHz;
                  4. else scan whole band, so end at 2400MHz;

RETURN VALUE
                  upper limit frequency
SIDE EFFECTS
                  None
===========================================================================*/
uint16 tdsrrccsp_get_band_e_upper_bound(void)
{
  uint16 upper_bound;

  if(TDSCHECK_OPTIONAL_FEATURE_SUBBAND_E3_SET)
  {
    upper_bound = 11846; /* 2370MHz */
  }
  else if(TDSCHECK_OPTIONAL_FEATURE_SUBBAND_E2_SET)
  {
    upper_bound = 11746; /* 2350MHz */
  }
  else if(TDSCHECK_OPTIONAL_FEATURE_SUBBAND_E1_SET)
  {
    upper_bound = 11646; /* 2330MHz */
  }
  else
  {
    upper_bound = 11996; /* 2400MHz */
  }

  return upper_bound;
}



/*===========================================================================

FUNCTION           RRCCSP_ADD_NBR_FREQ_BANDS_FROM_SIB11_TO_BAND_LIST

DESCRIPTION        Extracts inter frequency bands from SIB11/12 and adds them
                  to the band_list provided starting from num_of_bands_already_present.

DEPENDENCIES       num_of_bands_already_present should be initialized properly before calling
                   this function.

RETURN VALUE       number of bands present in the band list at the end.

SIDE EFFECTS       None
===========================================================================*/
static uint8 
tdsrrccsp_add_nbr_freq_bands_from_sib11_to_band_list
(
  tdsrrc_csp_band_class_type*    band_list,
  uint8 num_of_bands_already_present
)
{
  tdsrrc_SysInfoType11*    sib11_ptr;
  tdsrrc_SysInfoType12*    sib12_ptr;
  tdsrrc_MeasurementControlSysInfo*    measurementControlSysInfo_ptr;
  boolean    is_hcs_used = FALSE;
  boolean    is_rscp_used = FALSE;
  tdsrrc_freq_type    inter_freq_list[TDSMAX_NO_OF_CELLS] = {0};
  uint8    num_of_inter_freqs = 0;
  uint8    i;
  uint32 idx=0;
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();
  rrc_plmn_identity_type plmn_id = tdsrrcccm_get_curr_camped_plmn();
  tdsrrc_int_cell_id_type cell_id = tdsrrcccm_get_curr_camped_cell_id();

  union interFreqMeasurementSysInfo
  {
    tdsrrc_InterFreqMeasurementSysInfo_RSCP interFreqMeasurementSysInfo_rscp;
    tdsrrc_InterFreqMeasurementSysInfo_ECN0 interFreqMeasurementSysInfo_ecno;
    tdsrrc_InterFreqMeasurementSysInfo_HCS_RSCP interFreqMeasurementSysInfo_hcs_rscp;
    tdsrrc_InterFreqMeasurementSysInfo_HCS_ECN0 interFreqMeasurementSysInfo_hcs_ecno;
  }measurementSysInfo;

  (void)tdsrrcccm_get_curr_camped_common_plmn(&plmn_id);

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"WBP: Adding neighbor frequencies to the prioritized list");

  sib11_ptr = (tdsrrc_SysInfoType11 *) tdsrrc_sibdb_return_sib_for_cell(
                                            tdsrrc_SIB11,
                                            plmn_id, 
                                            cell_id);

  if( sib11_ptr == NULL )
  {
    /* SIB3 for this PLMN, CEll ID combination not present */
    TDSRRC_MSG3(MSG_LEGACY_ERROR,"WBP: ERR: SIB11 for MCC %d, MNC %d, Cell ID %d not present",
          TDSRRC_CSP_GET_INT_MCC(plmn_id),
          TDSRRC_CSP_GET_INT_MNC(plmn_id),
          cell_id);

    return num_of_bands_already_present;
  } /* SIB3 not present */

  if((rrc_state != TDSRRC_STATE_DISCONNECTED) && (rrc_state != TDSRRC_STATE_CONNECTING))
  {
    if(sib11_ptr->sib12indicator == TRUE)
    {
      /* Read SIB4. If it's not there mark and error and continue with SIB3 */
      sib12_ptr = (tdsrrc_SysInfoType12 *) tdsrrc_sibdb_return_sib_for_cell(
                                tdsrrc_SIB12, 
                                plmn_id,
                                cell_id);
      if( sib12_ptr == NULL )
      {
        /* SIB12 not present */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"WBP: SIB12 indicated but not present,using SIB11");
        measurementControlSysInfo_ptr = &(sib11_ptr->measurementControlSysInfo);
      }
      else
      {
        measurementControlSysInfo_ptr = &(sib12_ptr->measurementControlSysInfo);
      }
    }
    else
    {
      measurementControlSysInfo_ptr = &(sib11_ptr->measurementControlSysInfo);
    }
  }
  else
  {
    measurementControlSysInfo_ptr = &(sib11_ptr->measurementControlSysInfo);
  }

  if(TDSRRC_CHECK_COMMON_MSG_TYPE(measurementControlSysInfo_ptr->use_of_HCS,
     rrc_MeasurementControlSysInfo_use_of_HCS_hcs_not_used))
  {
    is_hcs_used = FALSE;
    if(measurementControlSysInfo_ptr->use_of_HCS.u.hcs_not_used->cellSelectQualityMeasure.t ==
        T_tdsrrc_MeasurementControlSysInfo_cellSelectQualityMeasure_cpich_RSCP)
    {
      is_rscp_used = TRUE;
      measurementSysInfo.interFreqMeasurementSysInfo_rscp = measurementControlSysInfo_ptr->use_of_HCS.u.hcs_not_used->cellSelectQualityMeasure.u.cpich_RSCP->interFreqMeasurementSysInfo;
    }
    else
    {
      is_rscp_used = FALSE;
      measurementSysInfo.interFreqMeasurementSysInfo_ecno = measurementControlSysInfo_ptr->use_of_HCS.u.hcs_not_used->cellSelectQualityMeasure.u.cpich_Ec_N0->interFreqMeasurementSysInfo;
    }
  }
  else
  {
    is_hcs_used = TRUE;
    if(measurementControlSysInfo_ptr->use_of_HCS.u.hcs_used->cellSelectQualityMeasure.t ==  
       (uint32) T_tdsrrc_MeasurementControlSysInfo_cellSelectQualityMeasure_1_cpich_RSCP)
    {
      is_rscp_used = TRUE;
      measurementSysInfo.interFreqMeasurementSysInfo_hcs_rscp = measurementControlSysInfo_ptr->use_of_HCS.u.hcs_used->cellSelectQualityMeasure.u.cpich_RSCP->interFreqMeasurementSysInfo;
    }
    else
    {
      is_rscp_used = FALSE;
      measurementSysInfo.interFreqMeasurementSysInfo_hcs_ecno = measurementControlSysInfo_ptr->use_of_HCS.u.hcs_used->cellSelectQualityMeasure.u.cpich_Ec_N0->interFreqMeasurementSysInfo;
    }
  }

  if(is_hcs_used)
  {
    if(is_rscp_used)
    {
      if((TDSRRC_MSG_COMMON_BITMASK_IE(measurementSysInfo.interFreqMeasurementSysInfo_hcs_rscp,
        tdsrrc_InterFreqMeasurementSysInfo_HCS_RSCP,interFreqCellInfoSI_List)) &&
         (TDSRRC_MSG_COMMON_BITMASK_IE(measurementSysInfo.interFreqMeasurementSysInfo_hcs_rscp.interFreqCellInfoSI_List,
           tdsrrc_InterFreqCellInfoSI_List_HCS_RSCP,newInterFreqCellList)))
      {
        tdsrrc_NewInterFreqCellSI_List_HCS_RSCP *newInterFreqCellList;
        idx=0;
        newInterFreqCellList = &measurementSysInfo.interFreqMeasurementSysInfo_hcs_rscp.interFreqCellInfoSI_List.newInterFreqCellList;
        while((newInterFreqCellList->n > idx) && (num_of_inter_freqs < TDSMAX_NO_OF_CELLS))
        {
          if((TDSRRC_MSG_LIST_BITMASK_IE(newInterFreqCellList,
            tdsrrc_NewInterFreqCellSI_HCS_RSCP,frequencyInfo)) &&
			  (TDSRRC_CHECK_COMMON_MSG_TYPE(newInterFreqCellList->elem[idx].frequencyInfo.modeSpecificInfo,
             rrc_FrequencyInfo_modeSpecificInfo_fdd)))
          {
            inter_freq_list[num_of_inter_freqs++] = newInterFreqCellList->elem[idx].frequencyInfo.modeSpecificInfo.u.fdd->uarfcn_DL;
          }
          idx++;
        }
      }
    }
    else
    {
      if((TDSRRC_MSG_COMMON_BITMASK_IE(measurementSysInfo.interFreqMeasurementSysInfo_hcs_ecno,
        tdsrrc_InterFreqMeasurementSysInfo_HCS_ECN0,interFreqCellInfoSI_List)) &&
         (TDSRRC_MSG_COMMON_BITMASK_IE(measurementSysInfo.interFreqMeasurementSysInfo_hcs_ecno.interFreqCellInfoSI_List,
          (uint32) tdsrrc_InterFreqCellInfoSI_List_HCS_ECN0,newInterFreqCellList)))
      {
        tdsrrc_NewInterFreqCellSI_List_HCS_ECN0 *newInterFreqCellList;
        idx=0;
        newInterFreqCellList = &measurementSysInfo.interFreqMeasurementSysInfo_hcs_ecno.interFreqCellInfoSI_List.newInterFreqCellList;
        while((newInterFreqCellList->n > idx) && (num_of_inter_freqs < TDSMAX_NO_OF_CELLS))
        {
          if((TDSRRC_MSG_LIST_BITMASK_IE(newInterFreqCellList,
            tdsrrc_NewInterFreqCellSI_HCS_ECN0,frequencyInfo)) &&
             (TDSRRC_CHECK_COMMON_MSG_TYPE(newInterFreqCellList->elem[idx].frequencyInfo.modeSpecificInfo,
             rrc_FrequencyInfo_modeSpecificInfo_fdd)))
          {
            inter_freq_list[num_of_inter_freqs++] = newInterFreqCellList->elem[idx].frequencyInfo.modeSpecificInfo.u.fdd->uarfcn_DL;
          }
          idx++;
        }
      }
    }
  }
  else
  {
    if(is_rscp_used)
    {
      if((TDSRRC_MSG_COMMON_BITMASK_IE(measurementSysInfo.interFreqMeasurementSysInfo_rscp,
        tdsrrc_InterFreqMeasurementSysInfo_RSCP,interFreqCellInfoSI_List)) &&
         (TDSRRC_MSG_COMMON_BITMASK_IE(measurementSysInfo.interFreqMeasurementSysInfo_rscp.interFreqCellInfoSI_List,
         tdsrrc_InterFreqCellInfoSI_List_RSCP,newInterFreqCellList)))
      {
         tdsrrc_NewInterFreqCellSI_List_RSCP *newInterFreqCellList;
         idx=0;
        newInterFreqCellList = &measurementSysInfo.interFreqMeasurementSysInfo_rscp.interFreqCellInfoSI_List.newInterFreqCellList;
        while((newInterFreqCellList->n > idx) && (num_of_inter_freqs < TDSMAX_NO_OF_CELLS))
        {
          if((TDSRRC_MSG_LIST_BITMASK_IE(newInterFreqCellList,
            tdsrrc_NewInterFreqCellSI_RSCP,frequencyInfo)) &&
             (TDSRRC_CHECK_COMMON_MSG_TYPE(newInterFreqCellList->elem[idx].frequencyInfo.modeSpecificInfo,
             rrc_FrequencyInfo_modeSpecificInfo_fdd)))
          {
            inter_freq_list[num_of_inter_freqs++] = newInterFreqCellList->elem[idx].frequencyInfo.modeSpecificInfo.u.fdd->uarfcn_DL;
          }
          idx++;
        }
      }
    }
    else
    {
      if((TDSRRC_MSG_COMMON_BITMASK_IE(measurementSysInfo.interFreqMeasurementSysInfo_ecno,
        tdsrrc_InterFreqMeasurementSysInfo_ECN0,interFreqCellInfoSI_List)) &&
         (TDSRRC_MSG_COMMON_BITMASK_IE(measurementSysInfo.interFreqMeasurementSysInfo_ecno.interFreqCellInfoSI_List,
         tdsrrc_InterFreqCellInfoSI_List_ECN0,newInterFreqCellList)))
      {
         tdsrrc_NewInterFreqCellSI_List_ECN0 *newInterFreqCellList;
        idx=0;
        newInterFreqCellList = &measurementSysInfo.interFreqMeasurementSysInfo_ecno.interFreqCellInfoSI_List.newInterFreqCellList;
        while((newInterFreqCellList->n > idx) && (num_of_inter_freqs < TDSMAX_NO_OF_CELLS))
        {
          if((TDSRRC_MSG_LIST_BITMASK_IE(newInterFreqCellList,
            tdsrrc_NewInterFreqCellSI_ECN0,frequencyInfo)) &&
             (TDSRRC_CHECK_COMMON_MSG_TYPE(newInterFreqCellList->elem[idx].frequencyInfo.modeSpecificInfo,
             rrc_FrequencyInfo_modeSpecificInfo_fdd)))
          {
            inter_freq_list[num_of_inter_freqs++] = newInterFreqCellList->elem[idx].frequencyInfo.modeSpecificInfo.u.fdd->uarfcn_DL;
          }
          idx++;
        }
      }
    }
  }

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"WBP: is_hcs_used: %d,is_rscp_used: %d",is_hcs_used,is_rscp_used);

  for(i = 0; ((i < num_of_inter_freqs) && (num_of_bands_already_present < MAX_NUM_OF_TDS_BANDS)); i++)
  {
    tdsrrc_csp_band_class_type nbr_freq_band;
    nbr_freq_band = tdsrrc_get_frequency_band(inter_freq_list[i]);
    if((TDSRRC_CSP_BAND_MAX != nbr_freq_band) && 
       (tdsrrccsp_is_supported_band(nbr_freq_band)))
    {
      if(!tdsrrccsp_is_band_already_added_to_prioritized_list(nbr_freq_band, num_of_bands_already_present))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"WBP: Adding neighbor band 0x%x to prioritized band list",nbr_freq_band);
        band_list[num_of_bands_already_present++] = nbr_freq_band;
      }
    }
  }

  return num_of_bands_already_present;
}

/*===========================================================================

FUNCTION           tdsrrccsp_prioritize_tds_bands_for_full_scan

DESCRIPTION       Constructs the order in which the TDS bands need to be searched while doing
                  full frequency scan.

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
void 
tdsrrccsp_prioritize_tds_bands_for_full_scan(void)
{
  /*
   * The priority order of scanning the TDS bands by the UE is: 
   * (i) Band on which UE last had service, followed by 
   * (ii) Band(s) to which the neighbor TDS frequencies belong to (extracted from System Information Blocks (SIB) 11 & 12), followed by 
   * (iii) Band(s) to which the frequencies stored in Acquisition Database belong to (LIFO), followed by 
   * (iv) Default band priority order specified by the carrier/OEM for any remaining bands. (this is the band mask values defined in tdsrrccspdb.h).
   */

  tdsrrc_csp_band_class_type    last_camped_band = TDSRRC_CSP_BAND_MAX;
  tdsrrc_freq_type    last_camped_freq = 0;
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();
  int i = 0;

  tdsrrc_csp_curr_select_data.prioritized_bands_read_idx = 0;
  tdsrrc_csp_curr_select_data.num_of_prioritized_bands = 0;

  if((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_SELECTION_SCAN) ||
     (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN) ||
     (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_DCH_TO_CELL_FACH_SCAN) ||
     (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_DCH_TO_CELL_PCH_SCAN) ||
     (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_DCH_TO_URA_PCH_SCAN) ||
     (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_FACH_TO_CELL_FACH_SCAN) ||
     (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_FACH_TO_CELL_PCH_SCAN) ||
     (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_FACH_TO_URA_PCH_SCAN))
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"WBP: Prioritizing the TDS bands used for full frequency scan,curr_scan is: %d.",tdsrrc_csp_int_data.curr_scan);
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"WBP: Not Prioritizing the TDS bands ,curr_scan is: %d.",tdsrrc_csp_int_data.curr_scan);
    return;
  }
  /*
    Step (i) Band on which UE last had service
  */

  if(rrc_state != TDSRRC_STATE_DISCONNECTED)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"LCF in Connected Mode = %d",tdsrrc_csp_int_data.last_camped_freq_in_connected_mode);
    last_camped_freq = tdsrrc_csp_int_data.last_camped_freq_in_connected_mode;
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"LCF in Idle Mode = %d",tdsrrc_csp_int_data.last_camped_freq_in_idle_mode);
    last_camped_freq = tdsrrc_csp_int_data.last_camped_freq_in_idle_mode;
  }

  last_camped_band = tdsrrc_get_frequency_band(last_camped_freq);

  if((TDSRRC_CSP_BAND_MAX != last_camped_band) &&
     (tdsrrccsp_is_supported_band(last_camped_band)) &&
      !tdsrrccsp_is_band_already_added_to_prioritized_list(last_camped_band, tdsrrc_csp_curr_select_data.num_of_prioritized_bands))
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"WBP: Adding last camped band 0x%x to prioritized band list",last_camped_band);
    tdsrrc_csp_curr_select_data.prioritized_bands[tdsrrc_csp_curr_select_data.num_of_prioritized_bands++] = last_camped_band;
  }

  /*
    Step (ii) Band(s) to which the neighbor TDS frequencies belong to (extracted from System Information Blocks (SIB) 11 & 12)
  */
  tdsrrc_csp_curr_select_data.num_of_prioritized_bands = tdsrrccsp_add_nbr_freq_bands_from_sib11_to_band_list(
                                                            tdsrrc_csp_curr_select_data.prioritized_bands,
                                                            tdsrrc_csp_curr_select_data.num_of_prioritized_bands);

  /*
    Step (iii) Band(s) to which the frequencies stored in Acquisition Database belong to (LIFO)
  */
  tdsrrc_csp_curr_select_data.num_of_prioritized_bands = tdsrrccsp_add_freq_bands_from_acq_db_to_band_list(
                                                            tdsrrc_csp_curr_select_data.prioritized_bands,
                                                            tdsrrc_csp_curr_select_data.num_of_prioritized_bands);

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"WBP: Total no of prioritized bands: %d ",tdsrrc_csp_curr_select_data.num_of_prioritized_bands);

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"WBP: List of Prioritized bands ");
  for(i = 0; ((i< tdsrrc_csp_curr_select_data.num_of_prioritized_bands) && (i < MAX_NUM_OF_TDS_BANDS)); i++)
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"WBP: Band :%d is 0x%x ",i+1,tdsrrc_csp_curr_select_data.prioritized_bands[i]);
  }

}



