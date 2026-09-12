#ifndef TDSSRCHFS_H
#define TDSSRCHFS_H
/*===========================================================================
                S E A R C H      H E A D E R      F I L E

GENERAL DESCRIPTION:
  This file contains extern declaration for the freq scanning functions used in other states.

EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS  
  

Copyright(c) 2000 to 2010 by Qualcomm Technologies Incorporated.
All Rights Reserved.
      
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdssrchfs.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $


when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/21/10   nd      Added support for Band-BC11 
05/03/10   sv     Fixed compiler warnings.
02/02/09   nd     extend the LFS feature for conventional OOS under FEATURE_LFS_COOS
09/26/07   nd     Added a new database to hold the freq, rssi and if the chnl 
                  is a valid wcdma channel
28/06/07   rc     Defined WCDMA band boundaries used to validate fine scan
                  frequency given by RRC.
03/05/07   sk     Added code for flexible deep sleep and 
                  FEATURE_UMTS_UNIFORM_OOS_HANDLING_P2 features.
09/27/06   nd     updated the prototype declaration for tdssrchfs_fine_scan()
11/08/05   nd/gv  Checkin the code onto mainline.
11/07/05   nd     Created file and added freq scan related prototypes.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "tdscdma_variation.h"
#include "tfw_sw_intf_msg.h"
#include "tdsl1rrcif.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/
/* freq scan call back function */
typedef void (*tdssrchfs_cb)(tdsl1_freq_scan_cnf_type* cnf);

/* Parameters to keep track of fs thresholds,
num retrys etc.  */
typedef struct
{
  int16 raw_scan_thresh;
  int16 fine_scan_thresh;
  int16 list_freq_scan_thresh;
  int16 raw_scan_thresh_bandF;
  int16 fine_scan_thresh_bandF;
  /* for fine scan and raw scan */
  uint16 start_ctr_freq;
  uint16 end_ctr_freq;
  /* for list scan*/
  uint16 num_list_freq;
  uint16 freq_idx;
  uint16 freq_list[TDSL1_MAX_ACQ_CHANNEL_NUM];
  /* general */
  tdssrchfs_cb cb_func;
  tdsl1_freq_scan_step_enum_type step;
  tdsl1_freq_scan_band_enum_type  band;
  boolean power_scan_suspend; /*power scan is not finished because of short of lock time*/
}tdssrchfs_parameters_type;

/* freq scan parameters like raw scan threshold, fine scan threshold etc */
extern tdssrchfs_parameters_type  tdssrchfs;

/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/


/*===========================================================================

FUNCTION        tdssrchfs_raw_scan

DESCRIPTION

  This function performs RAW scan on frequency band
  [start_ctr_freq, end_ctr_freq], and returns scan result to RRC via
  l1_freq_scan_cnf;

  NOTE: If the start_ctr_freq or the end_ctr_freq is invalid this 
  function will ERR_FATAL.

  The Frequency RAW procedure is as follows:

  AGC RSSI of all WCDMA carrier frequencies, starting from the
  (start_ctr_freq + offset) with 2 MHz increment, are read. All above-threshold
  carrier requencies are sorted in decending order and reported to RRC.

  RRC will subsequently request a FINE scan (with 200 KHz imcrement).

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  srchfs_first_ctr_freq and srchfs_last_ctr_freq will be updated.

===========================================================================*/

extern void tdssrchfs_raw_scan (uint16 start_ctr_freq, uint16 end_ctr_freq);

/*========================================================================
FUNCTION        tdssrchfs_send_freq_scan_failure

DESCRIPTION.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

==========================================================================*/
extern void tdssrchfs_send_freq_scan_failure(tdsl1_freq_scan_step_enum_type step, boolean no_trm_lock);

/*===========================================================================

FUNCTION        tdssrchfs_fine_scan

DESCRIPTION
  This function performs a fine scan of WCDMA carrier frequencies around
  a specified center frequency (ctr_freq), and returns the scan results to 
  RRC via l1_freq_scan_cnf;

  The fine scan procedure is as follows.

  The mobile measures RX_AGC values on all WCDMA carrier frequencies within
  the (2*TDSSRCH_FREQ_SCAN_FINE_SCAN_RANGE/5) MHz frequency band --
  +(TDSSRCH_FREQ_SCAN_FINE_SCAN_RANGE/5) MHz and
  -(TDSSRCH_FREQ_SCAN_FINE_SCAN_RANGE/5) MHz around the specified center frequency.  I.e. from ctr_freq - 1 MHz to ctr_freq + 1 MHz.

  For non-PCS bands this scan is performed in steps of 200 kHz. For the PCS 
  band the steps are in 100 kHz steps.

  Finally, L1 returns all above-threshold carrier frequencies, sorted with the
  largest RX_AGC value first, to RRC .via "TDSRRC_CPHY_FREQ_SCAN_CNF". In general,
  RRC will subsequently request Search acquisition (via "L1_CPHY_ACQ_REQ") on
  the carrier frequency with the largest RX_AGC.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/

extern void tdssrchfs_fine_scan (uint16 ctr_freq, tdsl1_freq_scan_band_enum_type band);

/*===========================================================================

FUNCTION        tdssrchfs_tune_and_chk_fine_scan

DESCRIPTION
  This function tunes to the specified frequency and checks the RX AGC
  value for a fine scan. If the RX AGC value is above the threshold the 
  value and frequency are stored in the results to be returned to RRC.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern uint16 tdssrchfs_tune_and_chk_fine_scan
(
  uint16                  ctr_freq,
  tdsl1_freq_scan_cnf_type  *freq_scan_cnf,
  uint16                  num_rpt_freq
);


/*===========================================================================

FUNCTION        tdssrchfs_list_scan

DESCRIPTION
  This function performs a list scan of TDSCDMA carrier frequencies provided 
  in a frequency list. This function can be invoked internally by ACQ module.
  If a call back function is provided, it will invoke the callback function to
  send the result to the caller.

DEPENDENCIES

RETURN VALUE
  TRUE - Freq Scan is started succesfully
  FALSE - Freq Scan failed due to error
SIDE EFFECTS

===========================================================================*/

extern boolean tdssrchfs_list_scan (uint16 num_freq, uint16 *freq_list, tdssrchfs_cb cb_func);


/*===========================================================================
FUNCTION     tdssrchfs_proc_power_scan_rsp

DESCRIPTION:
  - handle power_scan_rsp from FW
  

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================*/
void tdssrchfs_proc_power_scan_rsp(tfw_srch_power_scan_rsp_t *ps_result);

/*===========================================================================
FUNCTION     tdssrchfs_proc_state_cfg_rsp

DESCRIPTION:
  - handle state_cfg_rsp from FW
  

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================*/
void tdssrchfs_proc_state_cfg_rsp(tfw_state_t state);

/*===========================================================================

FUNCTION        tdssrchfs_is_valid_band_A_channel

DESCRIPTION
  This function determines if the specified channel is a valid IMT channel 
 
  Valid IMT channels can fall in the range of 10562-10838.

DEPENDENCIES
  RAW scan has been performed prior to this.

RETURN VALUE
  TRUE if it is a valid channel and it falls in the proper range, otherwise
  FALSE

SIDE EFFECTS
  None

===========================================================================*/
boolean tdssrchfs_is_valid_band_A_channel(uint16 ctr_freq);

/*===========================================================================

FUNCTION        tdssrchfs_is_valid_band_E_channel

DESCRIPTION
  This function determines if the specified channel is a valid IMT channel 
 
  Valid IMT channels can fall in the range of 10562-10838.

DEPENDENCIES
  RAW scan has been performed prior to this.

RETURN VALUE
  TRUE if it is a valid channel and it falls in the proper range, otherwise
  FALSE

SIDE EFFECTS
  None

===========================================================================*/
boolean tdssrchfs_is_valid_band_E_channel(uint16 ctr_freq);

/*===========================================================================

FUNCTION        tdssrchfs_is_valid_band_F_channel

DESCRIPTION
  This function determines if the specified channel is a valid IMT channel 
 
  Valid IMT channels can fall in the range of 10562-10838.

DEPENDENCIES
  RAW scan has been performed prior to this.

RETURN VALUE
  TRUE if it is a valid channel and it falls in the proper range, otherwise
  FALSE

SIDE EFFECTS
  None

===========================================================================*/
boolean tdssrchfs_is_valid_band_F_channel(uint16 ctr_freq);

#endif
