/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

     G L 1  M E S S A G E  L A Y E R -  A C Q U I S I T I O N

GENERAL DESCRIPTION
   This module contains the procedures used for acquiring synchronization to
   a cell by receiving and decoding the FCCH and SCH.
   The procedures used herein are intended for use by the
   portable layer 1 software.

EXTERNALIZED FUNCTIONS

   gl1_msg_cfg_fcch_sch
   gl1_msg_cfg_ncell_fcch_sch
   gl1_msg_rx_fcch_sch
   fcch_sch_handler
   gl1_msg_correct_timing


INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_msg_acq.c#2 $
$DateTime: 2019/12/30 04:21:22 $ $Author: pwbldsvc $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
24/12/19    sc      CR2592509 FR47558/FR47064 Clock Plan 2.0 Interface changes to pass scenario info to mcpm
24/04/18   nsa      CR2227778 Logging issue - RX Power is not updated properly in 0x5A75 after acquisition complete.
14/04/17   hd       CR2032569 Do not overwrite the SCH time offset with FCCH time offset in case of tone id mismatch
15/11/16   sm       CR1089963 Added state transition events and BCCH abort in W2G stare handler, to reproduce issue CR/1082619 
22/06/16   gmr      CR1030189 FR29075 : SNR value advertised as 0 when SCH decode happens directly 
05/01/16   nk/sp    CR901011 Backout CR814147
01/09/15   hd       CR899291 Do not move to next ARFCN when ACQ is aborted while tone seen, but SCH is not decoded
31/08/15   am       CR890621 Check tone when tone id and sch id dont match
18/08/15   am       CR892241 Resume BCCH when sufficient frames are left
12/05/15   jk       CR883632:SAWless support info needs to be handled in RF start GSM CNF message
02/07/15   am      CR863887 Ensure if ACQ has been completed on a cell then move to next cell
05/05/15   cs        CR832756 Ensure correct start FN always used for acquisition
22/05/15   ws       CR790493 Check BSIC decoded in SCH matches requested BSIC from RR
11/06/15   pg       CR850142 Featurise dedicated parallel SCH clock bump to avoid unnecessary MCPM/NPA calls 
11/05/15   pjr      CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
06/05/15   cs       CR827464 Correct use of SCH reported offset when Conv SCH in the
                    same frame as FCCH
31/03/15   am       CR814147 Don't skip acq abort in find_bcch state
30/01/15   cs       Disable clock speed change for Ncell Dedicated as Parallel SCH not supported
26/01/15   cs       CR786031 Only check valid FB/SB id matches when Conventional SCH detected
23/01/15   cs       CR785357 Ensure correct AFC update when Conventional SCH detected
                    on the same frame as FCCH
12/12/14   cs       CR754853 Add support for Parallel SCH detection handling
24/09/14   mko      CR727938 In ncell FCCH/SCH abort if acq state is ACQ_STOP_WAIT then don't set abort flag
09/09/14   jk       CR697308:GL1 changes to support HLin LLin sawless feature
07/08/14   cja      CR643648 Desense for channel adjacent to spur
27/05/14   sp       CR670592: Update AGC_ptr with Rxlev reported from FCCH results
20/12/13   am       CR588310 Always apply total frequency compensation when re-setting XO for new channel (e.g. cell-reselection, handover)
07/01/14   dv       CR596992: X2G NCELL FB search QTA crash
29/11/13   gk       CR551547:Go to the next arfcn if we see fcch missed and suspended the bplmn search
02/11/12   jj       CR 402579 Support of Tx Diversity in GERAN.
05/09/13   sp       CR538279 - Reset acq_state on re-installing the frame tick handler.
06/09/13   sjw      CR467545 - Pass updated RXLEV to GRR in FCCH_SCH_DECODE_IND
29/08/13   og       CR522009. Handover from GSM850 to PCS fails on SUB 1 while MO call is ON in SUB 2.
27/08/13   sp       CR534075 - Delay Sending of STOP_ACQ_CMD on ABORT req, if start_acq_offset is at end of frame.
17/05/13   cs       TSTS Updates
21/12/12   cja      CR423646 Store initial acq XO value for use by MPH_SELECT_SPECIFIC_BCCH_REQ
01/11/12   cja      CR416594 In Acq abort set stop acqisition offset as 1 timeslot, so not too close to late start acq
08/06/12   pa       GPLT: Replaced FEATUREN_GERAN_RF_SUPPORT with FEATURE_GL1_GPLT_CANNED_RSLT.
10/02/12   ws       CR 336194 Enable TD-SCDMA IRAT for GERAN
31/10/11   cja      CR312694 Force AFC update command after acq
20/09/11   cja      CR307007 Set acq XO value in CM startup.
25/05/11   cja      Force update of cycle swallower after acq
24/02/11   cja      CR 276160. For 9K ensure slam PDM after FCCH if Acq on PDM.
07/02/11   cs       Fix 9K PDM management for LIST_REQ
15/06/11   cja      Correct CR264690 merge error
14/06/11   cja      CR264690 Add multipass for XO
22/11/10   ky       Panic reset mechanism is changed. For all errors gl1 first
                    sees an nv item set or not. if this nv item is set then does
                    error fatal immediately other wise gl1 sends signal to grr for
                    recovery
18/11/10   cs       CR264840 Apply the correct XO correction when re-installing under LIST_REQ
05/10/10   cs       CR258661 Use the new frequency based threshold for multipass
18/05/10   ip       Removed compiler warning ...
27/04/10   cs       CR236460 Latest AFC\XO handling updates
09/04/10   cja      Corrected fine freq store for initial acquistion
12/02/10   ws       Corrected setting of afc_freq when SCH is decoded in fcch_sch_handler()
10/02/10   ws       Merged QDSP6 changes back to mainline
11/02/10   cja      Fix LINT error.
08/02/10  cja       XO Update
27/01/10   cja      Correct checkin and LINT errors. Set XO correctly on initial acq.
25/01/10   cja      CR224507. For XO correct freq error setting for NCell.
09-10-22   tjw      avoid compiler and lint warnings about unused variables when MSG
                    is defined away to nothing
09/07/09   ws       Added  FEATURE_GSM_GPRS_POSEIDON
                    to protect non supported API
16/06/09   ap       Fix Lint/compile warnings
05/06/09   cs       Update to give correct rot adjust log info
26/05/09   cs       CR167848 fix for expanded afc_freq I/F to mDSP
30/03/09   ws       Added FEATURE_GSM_GPRS_QDSP6 and FEATURE_GSM_GPRS_POSEIDON
                    to replace FEATURE_GSM_GPRS_MUSTANG
20/03/09   cs       Fixes to the common 32-bit afc_freq mDSP I/F
13/03/09   og       FEATURE_XO updates
12/03/09   og       FEATURE_XO support
19/03/09   ap       Fix Compiler Warning
02/02/09   cs       Fix the call to gl1_hw_set_vctcxo_pdm_val
19/01/09   og       Retain the results of the first tone when multiple tones are
                    detected in succession for the same arfcn. Resolves CR164113.
15/01/09   nt       CR167218 - changes to restore old rot and pdm values after HO fails
07/11/08   kps      Remove dependency on tcxo_srm if we're using TCXOMGR30
09/09/08   og       Mustang bring-up, apply negative frequency offsets correctly.
19/08/08   og       Mustang bring-up - Do not acquire in PDM mode.
18/07/08   nt       CR151852:- Changes for WPLT builds
03/07/08   nt       Added the parameter TCXOMGR_CLIENT_GSM to the call tcxomgr_reset_rgs_and_temp_table
30/06/08   cs       CR149200 Invert PDM/Rotator sign under FEATURE_UMTS_NEGATE_ALL_TCXOMGR_VALUES
27/06/08   nt       CR147984 - REV2:- Final changes for TCXO reset
26/06/08   nt       Code change to remove compiler warnings and error when TCXO3 is not defined
23/06/08   nt       CR147984: - removed featurisation around "tcxomgr.h" and added extra debug
20/06/08   cs       As part of CR147984 remove the 5.5KHz reset of PDM/Rotator
16/06/08   nt       CR147984:- Changes for TCXO reset.
20/05/08   cs       Fix Klocwork errors
12/07/07   ws       Added afc_adjust_tcxo_rotator prototype to fix compile warning
06/07/07   og       Allow W to inform G about the frequency offset of the G cell
                    in W2G non-blind handover scenarios. CR122524.
18/05/07   cs       Tidy freq est update
10/05/07   cs       TCXO Mgr frequency estimate update
28/11/06   ws       High Lint warning fixes
24/11/06   og       Correct compilation issues when rotator feature is switched
                    off.
09/11/06   og       Fix to CR92174, handle IDLE NCELL SCH / BCCH decode at high frequency
                    error conditions.
18/10/06   agv      Added TCXO MGR 3.0 for 7600.
12/10/06   og       Merge fix to CR101667 / SR860781. Ensure the correct freq err
                    is applied before attenpting to decode BCCH after pscan.
11/07/06   cs       CR104631 - Disable CHIPXN_DIV2 clk regime in GSM TCH mode on 7200
03/29/06   gfr      Add frequency offset to ncell functions to support non
                    spec-compliant networks in WCDMA mode.
11/07/05   gfr      QLint cleanup
09/23/05   gfr      Added frequency error to tone detected message
07/29/05   gfr      DTM support
06/21/05   gfr      Remove calls to tt_suspend and tt_resume
05/24/05   gfr      Use sys_algo_agc_update instead of sneaking in AGC pwr.
04/21/05   yh       Support for panic reset
04/20/05   yh       Remove use of RF fifo
04/07/05   gfr      Treat PDM as signed for target independence
02/05/05   bm       BG PLMN scan optimizations
01/25/04   yh       Set abort flag to true if state is INACTIVE or
                    WAITING_FOR_BLOCK START
12/17/04   gfr      Do not lock interrupts when writing to rotator.
12/07/04   yhong    Lint cleanup
11/01/04   yhong    Changed NCell Abort API to take a parameter
09/23/04   gfr      Suspend time tracking during activity.
08/12/04   gfr      Add more information to the abort message.
06/09/04   gfr      Consolidate static variables for robustness.
06/23/04   gfr      Lint cleanup.
06/22/04   gfr      Delay abort an extra frame.
05/20/04   gfr      Mainlined FEATURE_COMMON_PWR_HANDLER
05/07/04   gfr      Removed broken legacy support for back-to-back operation.
05/06/04   gfr      Combine phase and freq value for acq results.
04/09/04   gfr      Rotator now stored as phase instead of Hz.
03/05/04   gfr      Lint cleanup.
02/17/04   gfr      GSM rotators support.
02/11/04   gfr      Fix aborting, and remove need for pwr measure suspension.
02/09/04   dp       Fixed bugs in abort mechanism for background HPLMN.  Also
                    restore old PDM value if aborting
12/19/03   gfr      Compile fix for quadband.
12/17/03   gfr      Fix for early sleep after acquisition
11/05/03   gfr      Support for quad-band.
05/13/03   gfr      Common power handler support.
04/16/03   gfr      Pass the function pointer when removing the handler.
04/10/03   mk       Fixed PA Ramp buffer corruption issue via mechanism in
                    Acq handler which ensures that Acq stops before frame
                    tick is slammed.
11/26/02   jc       Changes for PDM for leopard.
10/17/02   JC       Added debug message for AGC override value.
10/17/02   JC       Added acquisition AGC support.
09/25/02   dp       Fixed bug in determining # of frames left for SCH decoding
09/13/02   ATM      Added Combined ACQ logging
08/23/02   dp       Fix to set the stored PDM value at the end of each
                    acquisition to carry over PDM value from last acq during
                    parallel acq
08/13/02   dp       Acquisition changes for parallel acq/decodes
07/18/02   ATM      Added FCCH/SCH decode logging.
07/11/02   gw       Changed reference to power_dBm_x16 in acq report to
                    pwr_dBm_x16.
06/28/02   thh      Featurize this whole module under FEATURE_GSM.  This is
                    required to allow dualmode build to make single mode
                    (WCDMA) executable.
03/22/02   JC       Added power and arfcn metrics to acq report for user.
03/06/02   JC       Added NULL argument to hw_was_sch_decoded().
02/15/02   mk       Changed gl1_msg_correct_timing().
02/07/02   gw       Changes to comments only.
02/04/02   JC       Modified interfaces to accommodate new AGC design.
02/01/02   mk       Updated to support the latest afc/tt algorithms.
01/16/02   mk       Added support for afc/tt.
12/18/01   dp       Add support to abort ncell acquisition
11/08/01   gw       Initial version.
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/


#include "geran_variation.h"
#ifdef CUST_H
#include "customer.h"
#endif

#include "comdef.h"
#include "memory.h"

#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "gl1_hwi.h"
#include "gl1_msg_g.h"
#include "gl1_msgi.h"
#include "gtmrs.h"
#include "gtmrs_g.h"
#include "msg.h"
#include "err.h"
#include "tcxomgr.h"
#include "gl1_hw_tcxo_mgr3.h"

#ifdef FEATURE_GL1_GPLT
#include "gplt_result.h"
#endif /* FEATURE_GL1_GPLT */

#include "geran_dual_sim.h"
#include "geran_multi_sim.h"
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
#include "l1_log_qsh.h"
#include "l1_task.h"
#endif

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
extern boolean qsh_sch_acq_success[NUM_GERAN_DATA_SPACES];
#endif
/*****************************************************

 LOCAL STORAGE

******************************************************/
/* Threshold for XO too far from cal value is 5 ppm */
#define ACQ_FREQ_XO_CONFIDENCE_THRESHOLD  (5 * 1024)

/* The default type of acquisition to use for FCCH/SCH and Ncell acq */
#define GL1_HW_ACQ_AFC_DEFAULT_AFC_TYPE GL1_HW_ACQ_AFC_ON_ROTATOR

/* Type for states in rx handler */
typedef enum
{
  ACQ_INACTIVE,
  ACQ_WAITING_FOR_BLOCK_START,

  /* Acquisition AGC states */
  ACQ_AGC_START,
  ACQ_AGC_WAITING,
  ACQ_AGC_PROCESS_RESULT,

  /* Acquisition states */
  ACQ_START,
  ACQ_WAITING,
  ACQ_CHECKING_FOR_TONE,
  ACQ_GETTING_SCH,
  ACQ_ABORTED,
  ACQ_WAIT_STOP,
  ACQ_STOPPED
} acq_state_type;


/* Structure containing all the acq information */
typedef struct
{
   /* State information */
   acq_state_type      state;
   boolean             aborted;
   acq_decode_rpt      fcch_rpt;
   uint16              fcch_frames_to_go;
   uint16              sch_frames_to_go;
   uint16              sch_attempts_to_go;

   /* Callback information */
   gl1_msg_rx_cb_type  callbacks;
   gl1_msg_decode_rpt *data;

   /* AFC parameters */
   gl1_hw_acq_afc_type do_AFC;
   boolean             check_afc;
   boolean             afc_on_vctcxo;
   int32               init_freq_value;

   /* Configured acq parameters */
   boolean             do_AGC;
   ARFCN_T             ARFCN;
   sys_algo_agc_T     *agc_ptr;
   uint16              start_offset;
   uint8               fcch_frames;
   uint8               sch_frames;
   uint8               sch_attempts;

   gl1_hw_rx_tags_type tags;

   boolean             early_callback;
   boolean             acq_completed;

   /* Used to let handler restore the rotator/pdm values
    * present prior to the NCELL combined acquistion attempt.
    */
   boolean             restore_scell_afc;

   uint8               bsic_expected;

} acq_params_type;

typedef struct
{
  boolean         tcxo_reset_pending;
  int32           gl1_msg_acq_xo_value;
  /* Local storage for parameters */
  acq_params_type acq_params;

  /* Retain the frequency error results of the first fcch
   * attempt.
   */
  int32           fine_freq;
  int32           coarse_freq;

  uint32          gl1_msg_start_acq_FN;
} gl1_msg_acq_data_t;

/* Data space for MultiSim Acq Data */
static gl1_msg_acq_data_t  gl1_msg_acq_data[NUM_GERAN_DATA_SPACES];

static gl1_msg_acq_data_t *gl1_msg_acq_data_ptr[NUM_GERAN_DATA_SPACES] =
                             { INITIAL_VALUE_INDEXED( &gl1_msg_acq_data ) };

/* Macro for printing tone information */
#define MSG_TONE_SEEN()                                       \
   MSG_GERAN_HIGH_3_G( GL1_ACQ_HDR"Tone seen: arfcn=%d snr=%d fe=%dHz", \
             gl1_msg_acq_data_ptr[gas_id]->acq_params.ARFCN.num,        \
             gl1_msg_acq_data_ptr[gas_id]->acq_params.fcch_rpt.snr,     \
             gl1_msg_acq_data_ptr[gas_id]->acq_params.fcch_rpt.fine_freq );


/*****************************************************

 LOCAL FUNCTION PROTOTYPES

******************************************************/
static boolean fcch_sch_rx_handler( boolean start_of_block,
                                    uint32  time_stamp,gas_id_t gas_id);

static void gl1_msg_acq_fch_dec_success(
              acq_params_type *gl1_msg_acq_params_p,
              gas_id_t         gas_id );

static void gl1_msg_acq_sch_dec_success(
              acq_params_type *gl1_sch_acq_params_p,
              gas_id_t         gas_id );

static void gl1_msg_check_acq(
              acq_params_type *gl1_acq_params_p,
              uint8            log_mode,
              gas_id_t         gas_id );

static void gl1_msg_get_acq_params_data(
              acq_params_type *gl1_msg_acq_params_p,
              boolean          copy_fcch_acq_results,
              gas_id_t         gas_id );

static void gl1_msg_set_acq_power_params_data(
              acq_params_type *gl1_msg_acq_params_p,
              gas_id_t         gas_id );

static void gl1_msgi_stop_acq_afc(
              boolean  success,
              int32    afc_result,
              boolean  restore_scell_afc,
              gas_id_t gas_id );

#ifdef GERAN_L1_HLLL_LNASTATE
static void gl1_msg_update_sawless_HLL_metrics(
              acq_params_type *gl1_msg_acq_params_p,
              gas_id_t         gas_id );
#endif /* GERAN_L1_HLLL_LNASTATE */

static int32 gl1_msg_get_acq_xo_value(
               gas_id_t gas_id );

static int32 gl1_msg_acq_afc_update(
               acq_params_type *gl1_msg_acq_params_p,
               gas_id_t         gas_id );

static void gl1_msg_set_start_acq_FN(
               uint32   start_acq_FN,
               gas_id_t gas_id );

uint32 gl1_msg_get_start_acq_FN( gas_id_t gas_id );

/*****************************************************

 GLOBAL FUNCTION PROTOTYPES

******************************************************/

boolean gl1_is_ok_to_increment_sch_index_during_abort( gas_id_t gas_id );

/*===========================================================================

FUNCTION gl1_msgi_init_fcch_sch

DESCRIPTION
  This function is used to initialize the data structures for acquisition.

===========================================================================*/
void gl1_msgi_init_fcch_sch( gas_id_t gas_id )
{
   acq_params_type *gl1_msg_acq_params_p =
     &gl1_msg_acq_data_ptr[gas_id]->acq_params;

   L1_STATE_CHANGE( gl1_msg_acq_params_p->state = ACQ_INACTIVE,
                    "ACQ_INACTIVE" );

   gl1_msg_acq_params_p->aborted              = FALSE;

   gl1_msg_acq_params_p->callbacks.data_cb    = NULL;
   gl1_msg_acq_params_p->callbacks.abort_cb   = NULL;
   gl1_msg_acq_params_p->data                 = NULL;
   gl1_msg_acq_params_p->agc_ptr              = NULL;
   gl1_msg_acq_params_p->early_callback       = FALSE;
   gl1_msg_acq_params_p->acq_completed        = FALSE;
   gl1_msg_acq_params_p->restore_scell_afc    = FALSE;
}

/*===========================================================================

FUNCTION gl1_msg_cfg_rx_fcch_sch

DESCRIPTION
  This function is used to configure the callbacks for acquisition.

  Note that no metrics are returned for acquisition.  The metrics
  callback is ignored.

===========================================================================*/
void gl1_msg_cfg_fcch_sch( gl1_msg_rx_cb_type *cb, gas_id_t gas_id )
{
   acq_params_type *gl1_msg_acq_params_p =
     &gl1_msg_acq_data_ptr[gas_id]->acq_params;

   /* Validate AFC */
   gl1_msg_acq_params_p->check_afc = TRUE;

   /* Do acq AFC using Rotator on XO */
   gl1_msg_acq_params_p->do_AFC = GL1_HW_ACQ_AFC_ON_ROTATOR;

   MSG_GERAN_MED_1_G( GL1_AFC_HDR"AFC on ROT FN:%d",
                      GSTMR_GET_FN_GERAN(gas_id) );

   gl1_msg_acq_params_p->callbacks.data_cb  = cb->data_cb;
   gl1_msg_acq_params_p->callbacks.abort_cb = cb->abort_cb;

   gl1_msg_acq_params_p->restore_scell_afc  = FALSE;
}

/*===========================================================================

FUNCTION gl1_msg_cfg_rx_fcch_sch_list

DESCRIPTION
  This function is used to configure the callbacks for acquisition during
  parallel decodes

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_cfg_fcch_sch_list(
       gl1_msg_rx_cb_type *cb,
       uint32              list_pos,
       gas_id_t            gas_id )
{
   acq_params_type *gl1_msg_acq_params_p =
     &gl1_msg_acq_data_ptr[gas_id]->acq_params;

   /* Validate AFC on the first acq of the list */
   gl1_msg_acq_params_p->check_afc = (list_pos == 0 ? TRUE : FALSE);

   /* Do acq AFC using the rotator if possible.  We do this for parallel
      decodes simply to exercise the code for regression. */
   gl1_msg_acq_params_p->do_AFC = GL1_HW_ACQ_AFC_ON_ROTATOR;

   MSG_GERAN_MED_1_G( GL1_AFC_HDR"list AFC on ROT FN:%d",
                      GSTMR_GET_FN_GERAN( gas_id ) );

   gl1_msg_acq_params_p->callbacks.data_cb  = cb->data_cb;
   gl1_msg_acq_params_p->callbacks.abort_cb = cb->abort_cb;

   gl1_msg_acq_params_p->restore_scell_afc  = FALSE;
}

/*===========================================================================

FUNCTION gl1_msg_cfg_ncell_fcch_sch

DESCRIPTION
  This function is used to configure the callbacks for ncell acquisition
  and the afc mode during IDLE mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void  gl1_msg_cfg_ncell_fcch_sch( gl1_msg_rx_cb_type *cb,
                                  boolean             afc_on,
                                  gas_id_t            gas_id )
{
   acq_params_type *gl1_msg_acq_params_p =
     &gl1_msg_acq_data_ptr[gas_id]->acq_params;

   if ( !afc_on )
   {
     gl1_msg_acq_params_p->restore_scell_afc = FALSE;

     /* Assume AFC is good at this point and disable it */
     gl1_msg_acq_params_p->check_afc = FALSE;
     gl1_msg_acq_params_p->do_AFC    = GL1_HW_ACQ_AFC_DISABLED;
   }
   else
   {
     /* Ensure initial MS values are restored after this
      * attempt.
      */
     gl1_msg_acq_params_p->restore_scell_afc = TRUE;

     /* Validate AFC */
     gl1_msg_acq_params_p->check_afc = TRUE;
     gl1_msg_acq_params_p->do_AFC    = GL1_HW_ACQ_AFC_ON_ROTATOR;
   } /* End of else afc_on is TRUE */

   /* Save the callback pointers */
   gl1_msg_acq_params_p->callbacks.data_cb   = cb->data_cb;
   gl1_msg_acq_params_p->callbacks.abort_cb  = cb->abort_cb;
}

/*===========================================================================

FUNCTION gl1_msg_rx_fcch_sch

DESCRIPTION
  Acquire synchronization to a cell by receiving the FCCH followed by the SCH.

  ARFCN            - the ARFCN to attempt acquisition on
  signal_strength  - the expext rx signal level
  start_offset     - the qbit offset of the start of the acquisition window
  fcch_frames      - the maximum number of frames to search for an FCB
  sch_frames       - the maximum number of frames to try to decode an SB
                     once an FCB has been detected (A value of 0 indicates
                     do FCCH only, however the report does not support this
                     feature very well yet.)
  do_AFC           - indicates whether the results from the FCb should be
                     used to adjust the frequency reference
  do_AGC           - enables/disables AGC during acquisition
  rpt              - reports for passing to callbacks

DEPENDENCIES
  gl1_msg_cfg_fcch_sch() must have been called at some time prior.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_rx_fcch_sch(
       ARFCN_T            ARFCN,
       sys_algo_agc_T    *agc_ptr,
       uint16             start_offset,
       uint8              fcch_frames,
       uint8              sch_frames,
       uint8              sch_attempts,
       boolean            do_AFC,
       boolean            do_AGC,
       boolean            early_callback,
       gl1_msg_rx_rpt    *rpt,
       uint8              bsic_expected,
       gas_id_t           gas_id )
{
   acq_params_type *gl1_msg_acq_params_p =
     &gl1_msg_acq_data_ptr[gas_id]->acq_params;

   /* Make sure we are not already active */
   if ( gl1_msg_acq_params_p->state == ACQ_WAITING_FOR_BLOCK_START )
   {
      MSG_GERAN_ERROR_2_G( "Double acquisition issued (ARFCNs=%d,%d)",
                           gl1_msg_acq_params_p->ARFCN.num, ARFCN.num );

      /* Remove the handler to avoid spurious errors, will be reinstalled below */
      gl1_msgi_remove_ft_handler( fcch_sch_rx_handler, gas_id );
   }
   else if ( gl1_msg_acq_params_p->state != ACQ_INACTIVE )
   {
      MSG_GERAN_ERROR_2_G( "Acquisition collision (state=%d ARFCN=%d)",
                           gl1_msg_acq_params_p->state, ARFCN.num );

      if ( gl1_msg_acq_params_p->state < ACQ_ABORTED )
      {
        /* At this point L1 is probably stuck, but to continue with this
             acquisition could kill the lower layers even worse.  To try and recover
             reinstall the handler in case somehow it got uninstalled and hope
             for the best. */
        gl1_msgi_add_ft_handler( fcch_sch_rx_handler, GL1_MSG_FT_HDLR_ACQ_NCELL, gas_id );

        return;
      }
      else
      {
         /*Acq State is probably not reset correctly during earlier abort*/
         /* Continue to add FT Handler and also reset ACQ state.*/
         MSG_GERAN_ERROR_0_G( "Acquisition state reset" );
      }
   }

   /* Save the parameters */
   gl1_msg_acq_params_p->ARFCN               = ARFCN;
   gl1_msg_acq_params_p->agc_ptr             = agc_ptr;
   gl1_msg_acq_params_p->start_offset        = start_offset;
   gl1_msg_acq_params_p->fcch_frames         = fcch_frames;
   gl1_msg_acq_params_p->sch_frames          = sch_frames;
   gl1_msg_acq_params_p->sch_attempts        = sch_attempts;
   gl1_msg_acq_params_p->do_AGC              = do_AGC;
   gl1_msg_acq_params_p->data                = rpt->data;
   gl1_msg_acq_params_p->early_callback      = early_callback;
   gl1_msg_acq_params_p->bsic_expected       = bsic_expected;
   gl1_msg_acq_params_p->acq_completed       = FALSE;

   /* Store the ARFCN into the AGC structure to ease debugging */
   agc_ptr->arfcn                            = ARFCN;

   /* Let the given afc setting override the default */
   if ( !do_AFC &&
        ( gl1_msg_acq_params_p->do_AFC != GL1_HW_ACQ_AFC_DISABLED ) )
   {
     MSG_GERAN_ERROR_0_G( "Disabling default acq afc" );

     gl1_msg_acq_params_p->do_AFC = GL1_HW_ACQ_AFC_DISABLED;
   }

   /* Acquisition AGC requires slew-rate limiting to be disabled */
   if ( do_AGC && agc_ptr->srl_enabled )
   {
      MSG_GERAN_ERROR_1_G( "Disabling SRL for ACQ AGC ARFCN=%d",
                           ARFCN.num );

      sys_algo_agc_disable_slew_rate_limiting( agc_ptr );
   }

   /* Add the handler and set the initial state */
   gl1_msgi_add_ft_handler( fcch_sch_rx_handler, GL1_MSG_FT_HDLR_ACQ_NCELL, gas_id );

   L1_STATE_CHANGE( gl1_msg_acq_params_p->state = ACQ_WAITING_FOR_BLOCK_START,
                    "ACQ_WAITING_FOR_BLOCK_START" );

   gl1_msg_acq_params_p->aborted = FALSE;
}

/****************************************************************************
 ****************************************************************************
 ****************************************************************************
                            LOCAL   FUNCTIONS
 ****************************************************************************
 ****************************************************************************
 ***************************************************************************/

/*===========================================================================

FUNCTION  gl1_msg_check_acq

DESCRIPTION
  This function is called at the end of acquisition attempt and indicates
  if failure/success and moves state to completed.

DEPENDENCIES
  None.

PARAMETERS
  gl1_acq_params_p - Ptr to acquisition data structure
  log_mode         - Indicate success/fail status for logging

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
static void gl1_msg_check_acq( acq_params_type *gl1_acq_params_p,
                               uint8            log_mode,
                               gas_id_t         gas_id )
{
   /* Only mode that indicates SUCCESS */
   boolean acq_success = ( log_mode == GL1_HW_LOG_ACQ_SUCCESS );

   /* ACQ attempt completed either success/fail */
   gl1_hw_stop_acquisition_geran( 0, gas_id );

   /* Log the result of combined acquisition */
   if(log_mode != GL1_HW_LOG_ACQ_FAIL_FCCH)
   {
     gl1_hw_log_comb_acq( log_mode, gl1_acq_params_p->ARFCN, gl1_acq_params_p->fcch_rpt.pwr_dBm_x16, gas_id );
   }
   else
   {
   gl1_hw_log_comb_acq( log_mode, gl1_acq_params_p->ARFCN, 0, gas_id );
   }
   if ( !acq_success )
   {
     /* Clear out for stop_acq data is reporting these values */
     gl1_acq_params_p->fcch_rpt.afc_freq = 0;

     gl1_acq_params_p->data->acq.snr     =
       gl1_acq_params_p->fcch_rpt.snr;
   }

   gl1_acq_params_p->data->acq.success = acq_success;

   L1_STATE_CHANGE( gl1_acq_params_p->state = ACQ_WAIT_STOP,
                    "ACQ_WAIT_STOP" );
}

/*===========================================================================

FUNCTION gl1_msgi_start_acq_agc

DESCRIPTION
  Starts acquisition AFC, scheduling an appropriate power measure.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void gl1_msgi_start_acq_agc( gas_id_t gas_id )
{
  /* Issue a power measure command to the frame layer */
  (void)gl1_hw_pwr_meas_cmd(
          &gl1_msg_acq_data_ptr[gas_id]->acq_params.ARFCN,
          &gl1_msg_acq_data_ptr[gas_id]->acq_params.agc_ptr,
          1,  /* single power measure */
          &gl1_msg_acq_data_ptr[gas_id]->acq_params.tags,
          gas_id,
          0 );
}

/*===========================================================================

FUNCTION gl1_msgi_stop_acq_agc

DESCRIPTION
  Stops acquisition AGC and retrieves the results.

===========================================================================*/
static dBx16_T gl1_msgi_stop_acq_agc(
                 gl1_hw_rx_tags_type *tags,
                 gas_id_t             gas_id )
{
  gl1_hw_measurement_T measurement;
  uint32               retrieved_meas;

  /* Just in case Klocwork complains */
  measurement.dBm_x16 = 0;

  (void)gl1_hw_pwr_meas_results(
          &measurement,     /* store the data here */
          1,                /* expect 1 measurement */
          &retrieved_meas,  /* how many measurements were found */
          TRUE,             /* only try once to get the data */
          tags,
          gas_id );

  /* Check that no mdsp errors occured */
  if ( retrieved_meas == 0 )
  {
    measurement.dBm_x16 = 0;
  }

  return ( measurement.dBm_x16 );
}

/*===========================================================================

FUNCTION gl1_msgi_start_acq_afc

DESCRIPTION
  Starts acquisition AFC.

===========================================================================*/
static void gl1_msgi_start_acq_afc( gas_id_t gas_id )
{
  int32            xo_adjust            = 0;
  acq_params_type *gl1_msg_acq_params_p =
    &gl1_msg_acq_data_ptr[gas_id]->acq_params;

  /* Check if acquisition afc is enabled */
  if ( gl1_msg_acq_params_p->do_AFC != GL1_HW_ACQ_AFC_DISABLED )
  {
    /* No PDM updates for XO */
    gl1_msg_acq_params_p->afc_on_vctcxo   = FALSE;

    gl1_msg_acq_params_p->init_freq_value = gl1_hw_get_xo_acc_freq_err( gas_id );

   /* Use XO value already set-up by bcch list request */
    MSG_GERAN_HIGH_2_G( GL1_AFC_HDR"Use Acq XO value:%dXO current value:%dXO",
                        gl1_msg_get_acq_xo_value(gas_id),
                        gl1_hw_get_xo_acc_freq_err( gas_id ) );

    gl1_hw_set_xo_acc_freq_err( gl1_msg_get_acq_xo_value( gas_id ), gas_id );

    /* Ensure rotator is reset to 0 */
    gl1_hw_set_rotator_val( 0, gas_id );

    /* Use this if the XO adjust comes in the rot field from TCXOMgr */
    xo_adjust =
     ( gl1_hw_get_xo_acc_freq_err( gas_id ) - gl1_msg_acq_params_p->init_freq_value );

    gl1_log_afc_adjust( xo_adjust, 0, LOG_AFC_ACQ_ON_XO, gas_id );
  }

  /* F3 the Acquisition parameters for debug. */
  MSG_GERAN_HIGH_2_G( GL1_AFC_HDR"ACQ Res: do_afc:%d DPLL:%dXO",
                      gl1_msg_acq_params_p->do_AFC,
                      gl1_hw_get_xo_acc_freq_err( gas_id ) );
}

/*===========================================================================

FUNCTION gl1_msgi_stop_acq_afc

DESCRIPTION
  Stops acquisition AFC.

===========================================================================*/
static void gl1_msgi_stop_acq_afc(
              boolean  success,
              int32    afc_result,
              boolean  restore_scell_afc,
              gas_id_t gas_id )
{
  int32            xo_adjust            = 0;
  acq_params_type *gl1_msg_acq_params_p =
    &gl1_msg_acq_data_ptr[gas_id]->acq_params;

  /* If we did not do acquisition afc then we have nothing to do */
  if ( gl1_msg_acq_params_p->do_AFC == GL1_HW_ACQ_AFC_DISABLED )
  {
    return;
  }

  if( restore_scell_afc )
  {
    success = FALSE;
  }

  /* Always reset rotator as any frequency error will be moved to XO */
  gl1_hw_set_rotator_val( 0, gas_id );

  /* Update XO if success */
  if ( success )
  {
    /* Set here as per previous designs */
    gl1_hw_set_xo_acc_freq_err( afc_result, gas_id );

    /* Store as acq XO value */
    gl1_msg_set_acq_xo_value( afc_result, gas_id );

    MSG_GERAN_HIGH_1_G( GL1_AFC_HDR"Post FCCH/SCH store ACQ AFC DPLL:%dXO",
                        gl1_hw_get_xo_acc_freq_err( gas_id ) );

    /* Calc the delta from original XO for logging */
    xo_adjust =
      ( gl1_hw_get_xo_acc_freq_err( gas_id ) - gl1_msg_acq_params_p->init_freq_value );
  }
  else
  {
    /* Failed so return values back to stored prior to failed ACQ */
    gl1_hw_set_xo_acc_freq_err( gl1_msg_acq_params_p->init_freq_value, gas_id );

    MSG_GERAN_MED_1_G( GL1_AFC_HDR"ACQ: restoring DPLL:%dXO",
                       gl1_msg_acq_params_p->init_freq_value );
  }

  /* Log any adjustment */
  gl1_log_afc_adjust( xo_adjust, 0, LOG_AFC_ACQ_RESULTS, gas_id );
}

/*===========================================================================

FUNCTION gl1_msgi_reset_acq_afc

DESCRIPTION
  Sets acquisition AFC.

===========================================================================*/
void gl1_msgi_reset_acq_afc( int32 afc_freq, gas_id_t gas_id )
{
  int32 xo_adjust     = 0;
  int32 init_xo_value = gl1_hw_get_xo_acc_freq_err( gas_id );

  MSG_GERAN_HIGH_2_G( GL1_ACQ_HDR"AFC reset: new xo_acc_freq_err:%dXO previous:%dXO",
                      afc_freq, gl1_hw_get_xo_acc_freq_err( gas_id ) );

  gl1_hw_set_xo_acc_freq_err( afc_freq, gas_id );
  gl1_hw_set_rotator_val( 0, gas_id );

  /* How much have we moved XO by */
  xo_adjust = ( gl1_hw_get_xo_acc_freq_err( gas_id ) - init_xo_value );

  /* Log any adjustment */
  gl1_log_afc_adjust( xo_adjust, 0, LOG_AFC_ACQ_RESULTS, gas_id );
}

/* This function is used to allow calling client write access to the
 * values of the rotator or XO/PDM.
 */

void gl1_msg_set_afc_info(
       int32    afc_freq,
       gas_id_t gas_id )
{
  MSG_GERAN_LOW_2_G( GL1_AFC_HDR"ACQ set: new xo_acc_freq_err:%dXO previous:%dXO",
                     afc_freq, gl1_hw_get_xo_acc_freq_err(gas_id) );

  /* Absolute XO acc freq err */
  gl1_hw_set_xo_acc_freq_err( afc_freq, gas_id );
}

/* Provides calling client (most likely GSM L1)
 * with the values of the rotator/pdm/XO.
 */
void gl1_msg_get_afc_info( int32 *afc_val_p, gas_id_t gas_id )
{
  if ( NULL != afc_val_p )
  {
    /* Absolute XO acc freq err */
    *afc_val_p = gl1_hw_get_xo_acc_freq_err(gas_id);
  }
  else
  {
    /* Print a warning message */
    MSG_GERAN_ERROR_0_G( GL1_AFC_HDR"NULL afc_val_p" );
  }

  MSG_GERAN_LOW_1_G( GL1_AFC_HDR"ACQ get: new xo_acc_freq_err:%dXO",
                     *afc_val_p);
}

/*===========================================================================

FUNCTION fcch_sch_handler

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  TRUE if message reception is complete
  FALSE if messages are still being received

SIDE EFFECTS
  None
===========================================================================*/
static boolean fcch_sch_rx_handler(
                 boolean  start_of_block, /* next frame is start of block (ignored) */
                 uint32   time_stamp,     /* currently not used                     */
                 gas_id_t gas_id )
{
   boolean                tick_sm;

   dBx16_T                acq_agc_pwr = 0;
   sys_modem_as_id_e_type as_id       = geran_map_gas_id_to_nas_id( gas_id );

   acq_params_type *gl1_msg_acq_params_p =
     &gl1_msg_acq_data_ptr[gas_id]->acq_params;

   /* We wrap the state machine in a while loop because we may need to
      tick it twice in a single frame if we are skipping acq AGC */
   do
   {
      /* Assume a single SM tick */
      tick_sm = FALSE;

      /* Tick the state machine */
      switch ( gl1_msg_acq_params_p->state )
      {
         case ACQ_INACTIVE:
            MSG_GERAN_ERROR_0_G( GL1_ACQ_HDR"Acquisition inactive" );
            return TRUE;

         case ACQ_WAITING_FOR_BLOCK_START:
            if ( !start_of_block )
            {
              return FALSE;
            }

            /* Set the next state based on AGC or not, and run the SM again */
            gl1_msg_acq_params_p->state =
              ( gl1_msg_acq_params_p->do_AGC ? ACQ_AGC_START : ACQ_START );

            acq_agc_pwr = 0;

            tick_sm = TRUE;
         break;

         /* --- ACQ AGC STATE MACHINE --- */

         case ACQ_AGC_START:
            /* Start AGC */
            gl1_msgi_start_acq_agc( gas_id );

            L1_STATE_CHANGE( gl1_msg_acq_params_p->state = ACQ_AGC_WAITING,
                             "ACQ_AGC_WAITING" );
         break;

         case ACQ_AGC_WAITING:
            /* Monitors in progress */
            L1_STATE_CHANGE( gl1_msg_acq_params_p->state = ACQ_AGC_PROCESS_RESULT,
                             "ACQ_AGC_PROCESS_RESULT" );
         break;

         case ACQ_AGC_PROCESS_RESULT:
            /* Stop agc now and get the results */
            acq_agc_pwr = gl1_msgi_stop_acq_agc( &gl1_msg_acq_params_p->tags, gas_id );

            /* Check if we aborted */
            if ( gl1_msg_acq_params_p->aborted )
            {
              if ( gl1_msg_acq_params_p->callbacks.abort_cb )
              {
                gl1_msg_acq_params_p->callbacks.abort_cb( gas_id );
              }
              else
              {
                MSG_GERAN_HIGH_0_G( GL1_ACQ_HDR"No Ncell FCCH/SCH abort CB registered" );
              }

              L1_STATE_CHANGE( gl1_msg_acq_params_p->state = ACQ_INACTIVE,
                               "ACQ_INACTIVE" );

              break;
            }

            L1_STATE_CHANGE( gl1_msg_acq_params_p->state = ACQ_START,
                             "ACQ_START" );
            /* fall through */

         /* --- ACQ STATE MACHINE --- */

         case ACQ_START:
            /* Start acquisition afc algorithm */
            gl1_msgi_start_acq_afc( gas_id );

            /* If we have valid acquisition AGC power, use it */
            if ( acq_agc_pwr < 0 )  /*lint !e644*/ /* acq_agc_pwr is initialized */
            {
               /* Believe it or not we need this useless copy or ADS 1.2 crashes */
               ARFCN_T arfcn = gl1_msg_acq_params_p->ARFCN;

               sys_algo_agc_update( gl1_msg_acq_params_p->agc_ptr, acq_agc_pwr,
                                    TRUE, time_stamp, gas_id );

               MSG_GERAN_MED_3_G( GL1_ACQ_HDR"ARFCN=%d: offset=%d (AGC pwr=%d)",
                                  arfcn.num, gl1_msg_acq_params_p->start_offset,
                                  acq_agc_pwr );

               /* avoid compiler and lint warnings about unused variables when MSG
                  is defined away to nothing */
               (void)arfcn;
            }
            else
            {
               MSG_GERAN_MED_3_G( GL1_ACQ_HDR"ARFCN=%d offset=%d FN=%d",
                                  gl1_msg_acq_params_p->ARFCN.num,
                                  gl1_msg_acq_params_p->start_offset,
                                  GSTMR_GET_FN_GERAN( gas_id ) );
            }

            /* Actual frame when start acq is sent to firmware */
            gl1_msg_set_start_acq_FN( GSTMR_GET_FN_GERAN( gas_id ),
                                      gas_id );

            /* Start the acquisition */
            gl1_hw_start_acquisition_geran(
               gl1_msg_acq_params_p->ARFCN,
               gl1_msg_acq_params_p->agc_ptr->pwr_dBm_x16,
               gl1_msg_acq_params_p->start_offset,
               0,                                   /* No frequency offset in GSM mode */
               gl1_hw_get_sch_decode_mode( TRUE ),  /* Do SCH decode */
               gl1_msg_acq_params_p->do_AFC,
               &gl1_msg_acq_params_p->tags,
               (gl1_msg_acq_params_p->bsic_expected != GL1_HW_INVALID_BSIC),
               as_id
             );

            /* Log the start of combined acquisition */
            gl1_hw_log_comb_acq(
               GL1_HW_LOG_ACQ_START,
               gl1_msg_acq_params_p->ARFCN,
               gl1_msg_acq_params_p->agc_ptr->pwr_dBm_x16,
               gas_id
             );

            /* Reset the FCCH SNR to 0 as default */
            gl1_msg_acq_params_p->fcch_rpt.snr = 0;

            /* Keep track of how many frames to go. */
            gl1_msg_acq_params_p->fcch_frames_to_go =
              gl1_msg_acq_params_p->fcch_frames;

            L1_STATE_CHANGE( gl1_msg_acq_params_p->state = ACQ_WAITING,
                             "ACQ_WAITING" );

#if defined ( FEATURE_QTA ) && defined ( FEATURE_CXM_QTA )
            gl1_hw_set_rx_cmd_issued_in_qta( TRUE, gas_id );
#endif
         break;

         case ACQ_WAITING:
           /* Wait for first frame to be received */
           L1_STATE_CHANGE( gl1_msg_acq_params_p->state = ACQ_CHECKING_FOR_TONE,
                            "ACQ_CHECKING_FOR_TONE" );
         break;

         case ACQ_CHECKING_FOR_TONE:
         {
           /* Check to see if we have found a valid gsm cell yet */
           /* or if we have used up all our frames.    */
           gl1_cell_acq_decode_status_t gl1_cell_acq_status =
             gl1_hw_was_gsm_cell_detected( gl1_msg_acq_params_p->ARFCN,
                                           gl1_msg_acq_params_p->bsic_expected,
                                           &gl1_msg_acq_params_p->data->acq,
                                           &gl1_msg_acq_params_p->fcch_rpt,
                                           gas_id );

           /* If we saw any valid SCH above and the CRC passed */
           if ( gl1_cell_acq_status == PARALLEL_SCH_DECODED ||
                gl1_cell_acq_status == CONVENTIONAL_SCH_DECODED )
           {
             /*
              * If SCH has been found then cache off the timing offset here
              * as otherwise it is overwritten with the FCCH report data below
              */
             int32 gl1_stored_sch_acq_time_offset =
                     gl1_msg_acq_params_p->data->acq.time_offset;

             /*
              * Update power params only for Parallel SCH based on rssi in SCH data as
              * FCCH decode has been missed out. For Conventional here then copy
              * over FCCH acq data as will also be populated.
              */
             gl1_msg_get_acq_params_data( gl1_msg_acq_params_p,
                                          ( gl1_cell_acq_status == CONVENTIONAL_SCH_DECODED ),
                                          gas_id );

             /*
              * For conventional in this case FCCH has been performed but sm tick
              * won't have occurred here so we need to ensure AFC/offset are correctly
              * updated, so we need to copy back the saved time-offset from SCH report
              */
             if ( gl1_cell_acq_status == CONVENTIONAL_SCH_DECODED )
             {
               gl1_msg_acq_params_p->data->acq.time_offset =
                 gl1_stored_sch_acq_time_offset;

               (void)gl1_msg_acq_afc_update( gl1_msg_acq_params_p, gas_id );
             }

             gl1_msg_acq_sch_dec_success( gl1_msg_acq_params_p,
                                          gas_id );
           }
           /* We found an FCCH instead so process that */
           else if ( gl1_cell_acq_status == FCCH_FOUND )
           {
             gl1_msg_get_acq_params_data( gl1_msg_acq_params_p,
                                          TRUE,
                                          gas_id );

             MSG_GERAN_MED_2_G( GL1_ACQ_HDR"FCCH Tone Seen: ARFCN=%d RSSI=%ddBm",
                                gl1_msg_acq_params_p->ARFCN.num,
                                ( gl1_msg_acq_params_p->fcch_rpt.pwr_dBm_x16 >> 4 ) );

             MSG_TONE_SEEN();

             /* Try to get SCH data */
             /* SCH frames is the total # of frames from the beginning of
                acquisition, so we need to subtract the frames used up
                for FCCH detection so far */
             gl1_msg_acq_params_p->sch_frames_to_go     = ( gl1_msg_acq_params_p->sch_frames -
               ( gl1_msg_acq_params_p->fcch_frames - gl1_msg_acq_params_p->fcch_frames_to_go ) );
             gl1_msg_acq_params_p->sch_attempts_to_go   = gl1_msg_acq_params_p->sch_attempts;
             gl1_msg_acq_params_p->fcch_frames_to_go    = gl1_msg_acq_params_p->fcch_frames;

             (void)gl1_msg_acq_afc_update( gl1_msg_acq_params_p, gas_id );

             L1_STATE_CHANGE( gl1_msg_acq_params_p->state = ACQ_GETTING_SCH,
                              "ACQ_GETTING_SCH" );
           }
           /* In case of GPLT and no RF, return default/canned result */
           else if ( (--gl1_msg_acq_params_p->fcch_frames_to_go == 0)
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
                     || ( !gplt_fbsb_data.success && gplt_fbsb_data.failure_fcch )
#endif /* (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/
                   )
           {
              /* ACQ has been checked */
              gl1_msg_acq_params_p->acq_completed = TRUE;

              /* We used up all our frames - time to give up */
              gl1_msg_check_acq( gl1_msg_acq_params_p,
                                 GL1_HW_LOG_ACQ_FAIL_FCCH,
                                 gas_id );

              /* No point in showing acq_params as not filled in for failed FCCH */
              MSG_GERAN_MED_1_G( GL1_ACQ_HDR"FCCH missed: ARFCN=%d",
                                 gl1_msg_acq_params_p->ARFCN.num );
           }
         }
         break;

         case ACQ_GETTING_SCH:
         {
            /* Check for parallel/conventional SCH first */
            gl1_cell_acq_decode_status_t gl1_cell_acq_status =
              gl1_hw_was_parallel_sch_detected( gl1_msg_acq_params_p->ARFCN,
                                                gl1_msg_acq_params_p->bsic_expected,
                                                &gl1_msg_acq_params_p->data->acq,
                                                &gl1_msg_acq_params_p->fcch_rpt,
                                                gas_id );

            /* Conventional SCH decode success implying FCCH already found */
            if ( gl1_cell_acq_status == CONVENTIONAL_SCH_DECODED )
            {
              boolean id_matched = TRUE;

              /* Only possible to check for Conventional SCH here*/
              if ( gl1_msg_acq_params_p->fcch_rpt.id !=
                   gl1_msg_acq_params_p->data->acq.id )
              {
                /*
                 * If SCH has been found then cache off the timing offset here
                 * as otherwise it is overwritten with the FCCH report data below
                 */
                int32 gl1_stored_sch_acq_time_offset =
                          gl1_msg_acq_params_p->data->acq.time_offset;

                MSG_GERAN_HIGH_2_G( GL1_ACQ_HDR"FCCH/SCH id mismatch fcch_rpt.id:%d data->acq.id:%d",
                                     gl1_msg_acq_params_p->fcch_rpt.id,
                                     gl1_msg_acq_params_p->data->acq.id );

                /* check tone now incase tone result update from GFW was delayed */
                if ( gl1_hw_gen_was_tone_detected( gl1_msg_acq_params_p->ARFCN,
                                                   &gl1_msg_acq_params_p->fcch_rpt,
                                                   gas_id ) )
                {
                  gl1_msg_acq_fch_dec_success( gl1_msg_acq_params_p, gas_id);

                  /* Copy back the saved time-offset from SCH report */
                  gl1_msg_acq_params_p->data->acq.time_offset =
                    gl1_stored_sch_acq_time_offset;
                }

                id_matched = ( gl1_msg_acq_params_p->fcch_rpt.id ==
                                     gl1_msg_acq_params_p->data->acq.id )? TRUE:FALSE;
              }

              if ( !id_matched )
              {
                MSG_GERAN_ERROR_2_G( GL1_ACQ_HDR"FCCH/SCH id error gl1_msg_acq_params_p->fcch_rpt.id:%d"
                                                "gl1_msg_acq_params_p->data->acq.id:%d",
                                     gl1_msg_acq_params_p->fcch_rpt.id,
                                     gl1_msg_acq_params_p->data->acq.id );

                gl1_msg_check_acq( gl1_msg_acq_params_p,
                                   GL1_HW_LOG_ACQ_FAIL_SCH,
                                   gas_id );

                break;
              }
              else
              {
                gl1_msg_acq_sch_dec_success( gl1_msg_acq_params_p,
                                             gas_id );
              }
            }
            /*
             * Parallel SCH decode success so id's won't match as only filled in by firmware
             * if a Conventional SCH is found
             */
            else if ( gl1_cell_acq_status == PARALLEL_SCH_DECODED )
            {
              gl1_msg_acq_sch_dec_success( gl1_msg_acq_params_p,
                                           gas_id );
            }
            /* In case of GPLT and no RF, return default/canned result */
            else if ( ( --gl1_msg_acq_params_p->sch_frames_to_go == 0 )
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
                      || ( !gplt_fbsb_data.success && gplt_fbsb_data.failure_sch )
#endif /* (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/
                    )
            {
               /* Time to give up */

               gl1_msg_acq_params_p->acq_completed = TRUE;

               gl1_msg_check_acq( gl1_msg_acq_params_p,
                                  GL1_HW_LOG_ACQ_FAIL_SCH,
                                  gas_id );
            }
            else
            {
               /* Check for SCH CRC failures - Give up after 2 CRC failures*/
               if ( gl1_cell_acq_status == SCH_CRC_FAILED )
               {
                  /* Only stop acq if we are not looking for specific BSIC */
                 if (gl1_msg_acq_params_p->bsic_expected == GL1_HW_INVALID_BSIC)
                 {
                    if ( --gl1_msg_acq_params_p->sch_attempts_to_go == 0 )
                    {
                       MSG_GERAN_HIGH_1_G( GL1_ACQ_HDR"SCH END after %d attempts",
                                           gl1_msg_acq_params_p->sch_attempts );

                       gl1_msg_check_acq( gl1_msg_acq_params_p,
                                          GL1_HW_LOG_ACQ_FAIL_SCH,
                                          gas_id );

                       break;
                    }
                 }
               }

               /* Check for other tones */
               if ( gl1_hw_gen_was_tone_detected( gl1_msg_acq_params_p->ARFCN,
                                                  &gl1_msg_acq_params_p->fcch_rpt,
                                                  gas_id ) )
               {
                 gl1_msg_acq_fch_dec_success( gl1_msg_acq_params_p, gas_id);
               }
               else if ( --gl1_msg_acq_params_p->fcch_frames_to_go == 0 )
               {
                  gl1_msg_check_acq( gl1_msg_acq_params_p,
                                     GL1_HW_LOG_ACQ_FAIL_FCCH,
                                     gas_id );
               }
            }
         }
         break;

         case ACQ_ABORTED:
            /* Stop the mdsp */
            if( ( gl1_msg_acq_params_p->start_offset >= (QS_PER_FRAME - QS_PER_SLOT) ) &&
                ( gl1_msg_acq_params_p->start_offset < QS_PER_FRAME ) )
            {
              /* If start_acq offset is between 4375 - 4999qS
               * (start_acq will be sent in fn-2, to run at 0-624 in fn),
               * trigger stop acquisition in same frame but 2 slots later
               */
              gl1_hw_stop_acquisition_geran( ( 2 * QS_PER_SLOT ), gas_id );
            }
            else
            {
              gl1_hw_stop_acquisition_geran( 0, gas_id );
            }

            /* We need an extra frame if we abort because the PL1 issues the abort
               and then ticks us in the same frame */
            L1_STATE_CHANGE( gl1_msg_acq_params_p->state = ACQ_WAIT_STOP,
                             "ACQ_WAIT_STOP" );
         break;

         case ACQ_WAIT_STOP:
            /*  We are now done acq afc */
            gl1_msgi_stop_acq_afc(
               ( !gl1_msg_acq_params_p->aborted &&
                 gl1_msg_acq_params_p->data->acq.success ),
               gl1_msg_acq_params_p->fcch_rpt.afc_freq,
               gl1_msg_acq_params_p->restore_scell_afc,
               gas_id );

            /* Force cycle swallower update */
            gl1_hw_set_active_cycle_swallower_adj( gl1_msg_acq_params_p->ARFCN, gas_id );
            gl1_hw_gfw_config_cycle_swallower( gas_id );

            MSG_GERAN_HIGH_3_G( GL1_ACQ_HDR"exiting: ARFCN=%d fine_freq=%dHz afc_freq=%dXO",
                                gl1_msg_acq_params_p->ARFCN.num,
                                (int32)gl1_msg_acq_params_p->data->acq.fine_freq,
                                (int32)gl1_msg_acq_params_p->fcch_rpt.afc_freq );

            L1_STATE_CHANGE( gl1_msg_acq_params_p->state = ACQ_STOPPED,
                             "ACQ_STOPPED" );

            /* Fall through if jump_acq_wait_stop is not TRUE */
            if ( !gl1_msg_acq_params_p->early_callback )
            {
              break;
            }
            /* fall through */

         case ACQ_STOPPED:
            /* Call the appropriate callback */
            if ( gl1_msg_acq_params_p->aborted )
            {
              if ( gl1_msg_acq_params_p->callbacks.abort_cb )
              {
                gl1_msg_acq_params_p->callbacks.abort_cb( gas_id );
              }
              else
              {
                MSG_GERAN_HIGH_0_G( GL1_ACQ_HDR"No Ncell FCCH/SCH abort CB registered" );
              }
            }
            else
            {
              if ( gl1_msg_acq_params_p->callbacks.data_cb )
              {
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
                // In case of GPLT and no RF, return default/canned result
                MSG_GERAN_HIGH_0_G( "GPLT DBG: fcch_sch_rx_handler: Filling acq data" );

                gl1_msg_acq_params_p->data->acq.success     = gplt_fbsb_data.success;
                gl1_msg_acq_params_p->data->acq.sch_data[0] = ( gplt_fbsb_data.bsic << 2 );
#endif /* (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/

                gl1_msg_acq_params_p->callbacks.data_cb( gl1_msg_acq_params_p->data, gas_id );
              }
              else
              {
                MSG_GERAN_HIGH_0_G( GL1_ACQ_HDR"No FCCH/SCH callback" );
              }
            }

            L1_STATE_CHANGE( gl1_msg_acq_params_p->state = ACQ_INACTIVE,
                             "ACQ_INACTIVE" );
         break;

         default:
            MSG_GERAN_ERROR_1_G( GL1_ACQ_HDR"Error in FCCH/SCH state machine state:%d",
                                 gl1_msg_acq_params_p->state );

            L1_STATE_CHANGE( gl1_msg_acq_params_p->state = ACQ_INACTIVE,
                             "ACQ_INACTIVE" );

            return TRUE;
         break;
      }
   } while ( tick_sm );

   return ( gl1_msg_acq_params_p->state == ACQ_INACTIVE );

/*lint -esym(715,time_stamp)*/
}

/*===========================================================================
FUNCTION gl1_msg_acq_fch_dec_success

DESCRIPTION
  This function cleans up when we detect FCH success either Parallel or
  Conventional.

PARAMETERS
  gl1_acq_params_p - Ptr to acquisition data structure

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void gl1_msg_acq_fch_dec_success(
              acq_params_type *gl1_msg_acq_params_p,
              gas_id_t         gas_id )
{
  /* Retain the frequency error results of the first FCCH
  * attempt.
  */
  if ( 1 == gl1_msg_acq_params_p->fcch_rpt.id )
  {
    gl1_msg_acq_data_ptr[gas_id]->coarse_freq =
      gl1_msg_acq_params_p->data->acq.coarse_freq;
    gl1_msg_acq_data_ptr[gas_id]->fine_freq   =
      gl1_msg_acq_params_p->data->acq.fine_freq;
  }

  gl1_msg_get_acq_params_data( gl1_msg_acq_params_p,
                               TRUE,
                               gas_id );

  gl1_msg_acq_params_p->fcch_frames_to_go    =
    gl1_msg_acq_data_ptr[gas_id]->acq_params.fcch_frames;

  MSG_GERAN_MED_2_G( GL1_ACQ_HDR"New FCCH Tone Seen: ARFCN=%d RSSI=%ddBm",
                     gl1_msg_acq_params_p->ARFCN.num,
                     ( gl1_msg_acq_params_p->fcch_rpt.pwr_dBm_x16 >> 4 ) );

  MSG_TONE_SEEN();

  gl1_msg_acq_params_p->fcch_rpt.coarse_freq =
    gl1_msg_acq_data_ptr[gas_id]->coarse_freq;

  /* Use latest afc_freq as fine_freq is saved from first acquire and is old */
  gl1_msg_acq_params_p->fcch_rpt.fine_freq   =
    gl1_msg_acq_afc_update( gl1_msg_acq_params_p, gas_id );

  /* Also update .data struct as this is passed into acq callback */
  gl1_msg_acq_params_p->data->acq.coarse_freq =
    gl1_msg_acq_params_p->fcch_rpt.coarse_freq;
  gl1_msg_acq_params_p->data->acq.fine_freq   =
    gl1_msg_acq_params_p->fcch_rpt.fine_freq;
}

/*===========================================================================
FUNCTION gl1_msg_acq_sch_dec_success

DESCRIPTION
  This function cleans up when we detect SCH success either Parallel or
  Conventional.

PARAMETERS
  gl1_acq_params_p - Ptr to acquisition data structure

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void gl1_msg_acq_sch_dec_success(
              acq_params_type *gl1_sch_acq_params_p,
              gas_id_t         gas_id )
{
  boolean parallel_sch_decoded =
            gl1_sch_acq_params_p->fcch_rpt.was_sch_decoded;

  gl1_sch_acq_params_p->data->acq.coarse_freq =
    gl1_sch_acq_params_p->fcch_rpt.coarse_freq;

  if ( !parallel_sch_decoded )
  {
    /* Fine freq return value is the total frequency error (in Hz), so calculate here */
    gl1_sch_acq_params_p->data->acq.fine_freq =
      gl1_hw_tone_detected_total_freq_offset( gl1_sch_acq_params_p->ARFCN,
                                              gl1_sch_acq_params_p->fcch_rpt.afc_freq,
                                              gas_id );

    /* Only needed if non-parallel SCH */
    gl1_sch_acq_params_p->data->acq.afc_freq  =
      gl1_sch_acq_params_p->fcch_rpt.afc_freq;
  }
  else
  {
    /* Parallel SCH so update AFC data params here */
    (void)gl1_msg_acq_afc_update( gl1_sch_acq_params_p, gas_id );

    /* Parallel SCH found so just copy full error in Hz from coarse_freq */
    gl1_sch_acq_params_p->data->acq.fine_freq =
      gl1_sch_acq_params_p->data->acq.coarse_freq;
  }


  gl1_sch_acq_params_p->data->acq.arfcn       = gl1_sch_acq_params_p->ARFCN;

  gl1_sch_acq_params_p->data->acq.pwr_dBm_x16 = gl1_sch_acq_params_p->fcch_rpt.pwr_dBm_x16;
  gl1_sch_acq_params_p->agc_ptr->pwr_dBm_x16  = gl1_sch_acq_params_p->fcch_rpt.pwr_dBm_x16;

  MSG_GERAN_MED_5_G( GL1_ACQ_HDR"SCH success: ARFCN=%d coarse_freq=%dHz fine_freq=%dHz"
                                " afc_freq=%dXO Parallel SCH:%d",
                     gl1_sch_acq_params_p->data->acq.arfcn.num,
                     gl1_sch_acq_params_p->data->acq.coarse_freq,
                     gl1_sch_acq_params_p->data->acq.fine_freq,
                     gl1_sch_acq_params_p->data->acq.afc_freq,
                     parallel_sch_decoded );

  /* Set acq_completed to TRUE only after SCH decoding successful */

  gl1_sch_acq_params_p->acq_completed = TRUE;

  gl1_msg_check_acq( gl1_sch_acq_params_p,
                     GL1_HW_LOG_ACQ_SUCCESS,
                     gas_id );
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
 if ((l1_tsk_buffer[gas_id].l1_state == L1_WTOG_BPLMN_MODE) && (qsh_sch_acq_success[gas_id] == TRUE))
  {
    gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_SCH_ACQ_SUCCESS);
  }
#endif
}

/*===========================================================================
FUNCTION gl1_msg_acq_afc_update

DESCRIPTION
  This function updates the acquisition AFC data.

PARAMETERS
  gl1_msg_acq_params_p - Ptr to acquisition data structure

DEPENDENCIES
  None

RETURN VALUE
  Total freq error in Hz, may be ignored.

SIDE EFFECTS
  None
===========================================================================*/
static int32 gl1_msg_acq_afc_update(
               acq_params_type *gl1_msg_acq_params_p,
               gas_id_t         gas_id )
{
  int32 afc_freq_hz_val, carr_temp;

  afc_freq_hz_val = carr_temp = 0;

  /* If we have a valid afc_freq then use for the ROT case */
  if ( gl1_msg_acq_params_p->do_AFC != GL1_HW_ACQ_AFC_DISABLED )
  {
    /*
     * Use latest afc_freq to get freq_err in Hz as this has
     * all freq corrections applied need to accommodate ACQ
     * using ROTATOR
     */
    afc_freq_hz_val = gl1_hw_phasetohz( gl1_msg_acq_params_p->fcch_rpt.afc_freq );
  }
  else if ( gl1_msg_acq_params_p->fcch_rpt.snr > GL1_MSG_AFC_VALID_SNR_THRESH )
  {
    /* Use raw fine_freq as no rot afc adjust */
    afc_freq_hz_val = gl1_msg_acq_params_p->fcch_rpt.fine_freq;
  } /* If SNR too low force zero offset onto current rot/XO */

  /* normalize rotator correction to F_err*2^14/(F_carr/2^16) */
  carr_temp =
    gl1_hw_freqtonormxofreq( gl1_msg_acq_params_p->ARFCN, afc_freq_hz_val, gas_id );

  /* If desense ARFCN then don't include current XO setting */
  if ( gl1_msg_is_arfcn_xo_desense( gl1_msg_acq_params_p->ARFCN, gas_id ) != DESENSE_CHAN )
  {
    /* total frequency compensation needed, including existing DPLL compensation */
    carr_temp += gl1_hw_get_xo_acc_freq_err( gas_id );
  }

  /* Final XO correction */
  gl1_msg_acq_params_p->fcch_rpt.afc_freq = carr_temp;

  /* Also update .data struct as this is passed into acq callback */
  gl1_msg_acq_params_p->data->acq.afc_freq =
    gl1_msg_acq_params_p->fcch_rpt.afc_freq;

  MSG_GERAN_MED_4_G( GL1_ACQ_HDR"FCCH/SCH captured: rpt.afc_freq=%dXO acq.fine_freq=%dHz"
                     " acq.afc_freq=%dXO afc_freq=%dHz",
                     gl1_msg_acq_params_p->fcch_rpt.afc_freq,
                     gl1_msg_acq_params_p->data->acq.fine_freq,
                     gl1_msg_acq_params_p->data->acq.afc_freq,
                     afc_freq_hz_val );

  return ( afc_freq_hz_val );
}

/*===========================================================================
FUNCTION gl1_msg_get_acq_params_data

DESCRIPTION
  This function sets the acquisition data based on the fcch report
  data. Copies all results to acq data structure.

PARAMETERS
  gl1_msg_acq_params_p - Ptr to acquisition data structure

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void gl1_msg_get_acq_params_data(
              acq_params_type *gl1_msg_acq_params_p,
              boolean          copy_fcch_acq_results,
              gas_id_t         gas_id )
{
  /* Copy over data from initial acq if FCCH performed first */
  if ( copy_fcch_acq_results )
  {
    gl1_msg_acq_params_p->data->acq.id          = gl1_msg_acq_params_p->fcch_rpt.id;
    gl1_msg_acq_params_p->data->acq.coarse_freq = gl1_msg_acq_params_p->fcch_rpt.coarse_freq;
    gl1_msg_acq_params_p->data->acq.fine_freq   = gl1_msg_acq_params_p->fcch_rpt.fine_freq;
    gl1_msg_acq_params_p->data->acq.afc_freq    = gl1_msg_acq_params_p->fcch_rpt.afc_freq;
    gl1_msg_acq_params_p->data->acq.snr         = gl1_msg_acq_params_p->fcch_rpt.snr;
    gl1_msg_acq_params_p->data->acq.time_offset = gl1_msg_acq_params_p->fcch_rpt.time_offset;
  }

  /* Update power params */
  gl1_msg_set_acq_power_params_data( gl1_msg_acq_params_p,
                                     gas_id );
}

/*===========================================================================
FUNCTION gl1_msg_set_acq_power_params_data

DESCRIPTION
  This function sets the acquisition power data based on the fcch report
  data.

PARAMETERS
  gl1_msg_acq_params_p - Ptr to acquisition data structure

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void gl1_msg_set_acq_power_params_data(
              acq_params_type *gl1_msg_acq_params_p,
              gas_id_t         gas_id )
{
  gl1_msg_compute_rx_power( gl1_msg_acq_params_p->fcch_rpt.rssi,
                            &gl1_msg_acq_params_p->fcch_rpt.pwr_dBm_x16,
                            &gl1_msg_acq_params_p->tags.gain.range,
                            &gl1_msg_acq_params_p->tags.gain );

#ifdef GERAN_L1_HLLL_LNASTATE
  gl1_msg_update_sawless_HLL_metrics( gl1_msg_acq_params_p, gas_id );
#endif /* GERAN_L1_HLLL_LNASTATE */
}

#ifdef GERAN_L1_HLLL_LNASTATE
/*===========================================================================
FUNCTION gl1_msg_update_sawless_HLL_metrics

DESCRIPTION
  This function sets the acquisition Sawless Metrics based on the fcch report
  data.

PARAMETERS
  gl1_msg_acq_params_p - Ptr to acquisition data structure

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void gl1_msg_update_sawless_HLL_metrics(
              acq_params_type *gl1_msg_acq_params_p,
              gas_id_t         gas_id )
{
  gl1_defs_metrics_lnastate gl1_metrics_lnastate;
  uint8                     SawlessHLLLState[MAX_NUMBER_OF_CHANNELS] = { 0 };

  if ( gl1_hw_get_sawless_HLLL_support( gl1_msg_acq_params_p->ARFCN.band, gas_id ) )
  {
    gl1_metrics_lnastate.NoOfChannels              = 1;
    gl1_metrics_lnastate.gsm_metrics[0].arfcn      = gl1_msg_acq_params_p->ARFCN;
    gl1_metrics_lnastate.gsm_metrics[0].rx_lvl_dbm = gl1_msg_acq_params_p->fcch_rpt.pwr_dBm_x16;
    gl1_metrics_lnastate.gsm_metrics[0].snr        = gl1_msg_acq_params_p->fcch_rpt.snr;

    /* If parallel SCH is detected then never set jdet field */
    if ( gl1_msg_acq_params_p->fcch_rpt.was_sch_decoded )
    {
      gl1_metrics_lnastate.gsm_metrics[0].jdet_value = 0;
    }
    else
    {
      gl1_metrics_lnastate.gsm_metrics[0].jdet_value =
        gl1_msg_acq_params_p->fcch_rpt.jdetReading;
    }

    SawlessHLLLState[0]                            = gl1_msg_acq_params_p->tags.sawless_lnastate;

    gl1_hw_rf_burst_metrics( &gl1_metrics_lnastate, gas_id );

    gl1_hw_log_HLin_LLin_metrics( &gl1_metrics_lnastate, SawlessHLLLState, gas_id );
  }
}
#endif /* GERAN_L1_HLLL_LNASTATE */

/*===========================================================================

FUNCTION  gl1_msg_abort_rx_ncell_fcch_sch

DESCRIPTION
  This function aborts the currently ongoing ncell FCCH/SCH rx and invokes the
  registered abort callback when done.
  It may be called multiple times as only the first call will be processed.

DEPENDENCIES
  Must only be called in the context of the same ISR that the message layer
  frame tick notification is called from, i.e. we don't want the ftn running
  when we are part way through this function.

PARAMETERS
  Callback to

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
void gl1_msg_abort_rx_ncell_fcch_sch( gl1_msg_ncell_abort_type type,
                                      gas_id_t                 gas_id )
{
   acq_params_type *gl1_msg_acq_params_p =
     &gl1_msg_acq_data_ptr[gas_id]->acq_params;

   MSG_GERAN_MED_3_G( GL1_ACQ_HDR"abort: state=%d ARFCN=%d FN=%d",
                      gl1_msg_acq_params_p->state, gl1_msg_acq_params_p->ARFCN.num,
                      GSTMR_GET_FN_GERAN(gas_id) );

   switch (type)
   {
      case DELAYED_ABORT:
         /* Check if we have started yet */
         if ( gl1_msg_acq_params_p->state == ACQ_INACTIVE ||
              gl1_msg_acq_params_p->state == ACQ_WAITING_FOR_BLOCK_START )
         {
            /* Let state machine make callback and deactivate handler */
            gl1_msg_acq_params_p->aborted = TRUE;

            L1_STATE_CHANGE( gl1_msg_acq_params_p->state = ACQ_STOPPED,
                             "ACQ_STOPPED" );
         }
         /* Check if we have not already aborted */
         else if ( !gl1_msg_acq_params_p->aborted )
         {
            /* Log the failure of combined acquisition */
            gl1_hw_log_comb_acq( GL1_HW_LOG_ACQ_ABORTED, NULL_ARFCN, 0, gas_id );

            if ( gl1_msg_acq_params_p->state == ACQ_WAIT_STOP )
            {
              /* dont set aborted flag here anyhow we are going to call the data cb
               * with the results when handler runs
               */
              MSG_GERAN_MED_0_G( GL1_ACQ_HDR"In ACQ_WAIT_STOP state don't process abort cb" );
            }
            else
            {
               /* Flag to abort when we are done */
               gl1_msg_acq_params_p->aborted = TRUE;

               /* An acq has been started, stop it */
               if ( ( gl1_msg_acq_params_p->state > ACQ_START ) &&
                    ( gl1_msg_acq_params_p->state < ACQ_ABORTED ) )
               {
                  /* Go to the aborted state */
                  L1_STATE_CHANGE( gl1_msg_acq_params_p->state = ACQ_ABORTED,
                                   "ACQ_ABORTED" );
               }
            }
         }
         break;

      case IMMEDIATE_ABORT:
      default:
         if(geran_get_nv_recovery_restart_enabled(gas_id))
         {
           MSG_GERAN_ERROR_1_G( GL1_ACQ_HDR"Aborting combined acq with invalid type %d",
                                type );

           gl1_hw_handle_panic(gas_id);
         }
         else
         {
           MSG_GERAN_FATAL_1_G( GL1_ACQ_HDR"Aborting combined acq with invalid type %d",
                                type );
         }
         break;
   }
}

/* fine freq */
void gl1_msg_reset_pdm( int16     frequency_offset,
                        ARFCN_T   arfcn,
                        gas_id_t  gas_id )
{
  int32  curr_xo_val = gl1_hw_get_xo_acc_freq_err(gas_id);

  int32  carr_temp   = gl1_hw_freqtonormxofreq( arfcn,
                                                frequency_offset,
                                                gas_id );

  MSG_GERAN_MED_2_G( GL1_AFC_HDR"setting rotator to correct %dHz freq error for ARFCN=%d",
                     frequency_offset,
                     arfcn.num );

  /* total frequency compensation needed, including existing DPLL compensation */
  carr_temp += gl1_hw_get_xo_acc_freq_err( gas_id );

  gl1_hw_set_rotator_val( 0, gas_id );

  /* Set here as per previous designs */
  gl1_hw_set_xo_acc_freq_err( carr_temp, gas_id );

  /* Store as acq XO value */
  gl1_msg_set_acq_xo_value( carr_temp, gas_id );

  gl1_log_afc_adjust( ( gl1_hw_get_xo_acc_freq_err(gas_id) - curr_xo_val ),
                      0,                            /* No rot adjust applied yet */
                      LOG_AFC_TRACKING,
                      gas_id );
}

/*===========================================================================

FUNCTION  gl1_msg_check_list_req_pass_num

DESCRIPTION
  This function looks at the retry_with_reset passed in from RR. This is the provided
  by RR as an indication that bcch_decode_list_req has been re-requested as L1 has
  indicated low confidence on the last pass.

DEPENDENCIES

PARAMETERS
  boolean retry_with_reset
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/
void gl1_msg_check_list_req_pass_num( boolean retry_with_reset, gas_id_t gas_id )
{
  int32                 xo_curr_temp;
  tcxomgr_vco_info_type vco_info;

  if ( retry_with_reset )
  {
     xo_curr_temp = tcxomgr_ft_curr();

#if defined  FEATURE_TCXOMGR_NEGATE_FREQ_ERROR
     xo_curr_temp *= ( -1 );
#endif /* FEATURE_TCXOMGR_NEGATE_FREQ_ERROR | FEATURE_UMTS_NEGATE_ALL_TCXOMGR_VALUES */

    gl1_msg_set_acq_xo_value( xo_curr_temp, gas_id );
  }
  else
  {
     /* Set XO to TCXO Manager stored value - normal operation */
     vco_info = tcxomgr_get_stored_vco();

#if defined  FEATURE_TCXOMGR_NEGATE_FREQ_ERROR
     vco_info.rot_value *= ( -1 );
#endif /* FEATURE_TCXOMGR_NEGATE_FREQ_ERROR | FEATURE_UMTS_NEGATE_ALL_TCXOMGR_VALUES */

     gl1_msg_set_acq_xo_value( vco_info.rot_value, gas_id );
  }

  MSG_GERAN_HIGH_3_G( GL1_ACQ_HDR"Set Acq XO to %dXO for multipass %d FN:%d",
                      gl1_msg_get_acq_xo_value( gas_id ), retry_with_reset,
                      GSTMR_GET_FN_GERAN( gas_id ) );
}

/*===========================================================================
FUNCTION gl1_msg_set_acq_xo_value

DESCRIPTION
  This function sets the XO value to be used for initial acquisition.
===========================================================================*/
void gl1_msg_set_acq_xo_value( int32 xo_value, gas_id_t gas_id )
{
  gl1_msg_acq_data_ptr[gas_id]->gl1_msg_acq_xo_value = xo_value;
}

/*===========================================================================
FUNCTION gl1_msg_get_acq_xo_value

DESCRIPTION
  This function sets the XO value to be used for initial acquisition.
===========================================================================*/
static int32 gl1_msg_get_acq_xo_value( gas_id_t gas_id )
{
  return ( gl1_msg_acq_data_ptr[gas_id]->gl1_msg_acq_xo_value );
}

/*===========================================================================

FUNCTION  gl1_msg_get_pdm_confidence_factor

DESCRIPTION
  This function looks at the value of the currently used PDM value to decide if the value looks
  like a normal value. If it is outside the CONFIDENCE_THRESHOLD's then this
  returns FALSE, or confidence is low verdict.

DEPENDENCIES

PARAMETERS
  None
RETURN VALUE
  boolean FALSE=confidence is low
          TRUE=confidence is high
SIDE EFFECTS
  None

===========================================================================*/
boolean gl1_msg_get_pdm_confidence_factor( gas_id_t gas_id )
{
  int32 xo_diff_from_cal, xo_curr_temp;

  boolean confidence = TRUE;
  int32   xo_value   = gl1_hw_get_xo_acc_freq_err(gas_id);

  xo_curr_temp = tcxomgr_ft_curr();

#if defined  FEATURE_TCXOMGR_NEGATE_FREQ_ERROR
  xo_curr_temp *= ( -1 );
#endif /* FEATURE_TCXOMGR_NEGATE_FREQ_ERROR | FEATURE_UMTS_NEGATE_ALL_TCXOMGR_VALUES */

  xo_diff_from_cal = ( xo_curr_temp - xo_value );

  if ( abs(xo_diff_from_cal) > ACQ_FREQ_XO_CONFIDENCE_THRESHOLD )
  {
     MSG_GERAN_ERROR_3_G( GL1_ACQ_HDR"XO over limit XO=%d Cal=%d Diff=%d",
                          xo_value, xo_curr_temp, xo_diff_from_cal );

     confidence = FALSE;
  }

  return ( confidence );
}

/*=============================================================================
FUNCTION gl1_msg_get_afc_params

DESCRIPTION
   Gets the current values loaded up in the rotator and the PDM, and returns
   them in the pointers passed into the function.

============================================================================= */
void gl1_msg_get_afc_params( int32 *rotator_value_store,
                             int32 *freq_value_store, gas_id_t gas_id )
{
  *rotator_value_store = gl1_hw_get_rotator_val(gas_id);

  *freq_value_store    = gl1_hw_get_xo_acc_freq_err(gas_id);

  MSG_GERAN_LOW_2_G( GL1_AFC_HDR"Get rotator of %d and the XO of %dXO",
                     *rotator_value_store, *freq_value_store );
}

/*=============================================================================
FUNCTION gl1_msg_set_afc_params

DESCRIPTION
   Sets up the PDM and the rotator to the values passed in.

============================================================================= */
void gl1_msg_set_afc_params( int32 rotator_value_store,
                             int32 freq_value_store, gas_id_t gas_id )
{
  MSG_GERAN_LOW_2_G( GL1_AFC_HDR"Slam the rotator to %d and the XO to %dXO",
                     rotator_value_store, freq_value_store );

  /* For XO just put back what we had before */
  gl1_hw_set_xo_acc_freq_err( freq_value_store, gas_id );

  /* Write the rotator setting */
  gl1_hw_set_rotator_val( rotator_value_store, gas_id );
}

boolean gl1_is_ok_to_increment_sch_index_during_abort( gas_id_t gas_id )
{

  return ( gl1_msg_acq_data_ptr[gas_id]->acq_params.acq_completed
           && ( ( gl1_msg_acq_data_ptr[gas_id]->acq_params.state == ACQ_WAIT_STOP )
                || ( gl1_msg_acq_data_ptr[gas_id]->acq_params.state == ACQ_STOPPED ) )
         );
}

/*=============================================================================
FUNCTION gl1_msg_getting_sch

DESCRIPTION
   Returns TRUE if ACQ has find a tone and moved to SCH detection

============================================================================= */
boolean gl1_msg_getting_sch( gas_id_t gas_id )
{
  return ( gl1_msg_acq_data_ptr[gas_id]->acq_params.state == ACQ_GETTING_SCH );
}

#ifdef FEATURE_GSM_PARALLEL_SCH_DEDICATED
/*===========================================================================

FUNCTION  l1_par_sch_mcpm_change

DESCRIPTION
  This function requests MCPM change at start/end of Ncell reads
  in dedicated so that clock speed is increased during Ncell FCCH/SCH
  to cope with increased Parallel SCH firmware processing load.

===========================================================================*/
void gl1_par_sch_mcpm_change(
        boolean  par_acq_start,
        gas_id_t gas_id )
{
  if ( gl1_hw_is_parallel_sch_enabled() ||
       gl1_hw_is_x2g_parallel_sch_enabled() )
  {
    /* Ensure higher clock speed for Parallel ACQ */
    MSG_GERAN_LOW_1_G( "MCPM change for Parallel ACQ or Ncell FCCH alone  %d",
                       par_acq_start );

    /* Fix to always disable clock change as no Parallel FCCH support in Dedicated */
    par_acq_start = FALSE;

    gl1_clkdata_speed( par_acq_start, L1_PAR_ACQ_SPEEDUP, 0, L1_PAR_ACQ, gas_id );
  }
}
#endif /* FEATURE_GSM_PARALLEL_SCH_DEDICATED */

/*===========================================================================

FUNCTION  gl1_msg_set/get_start_acq_FN

DESCRIPTION
  This function is used to cache away the acquisition start FN when the
  firmware acquisition command is actually going to be processed by firmware.
  That is why there is a frame offset. This is then used when the SCH is
  received to correctly ascertain the FN offset to synchronise to the new
  cell info.

DEPENDENCIES
  None

PARAMETERS
  uint32 start_acq_FN - Actual FN when acquisition command is issued by the
  GL1 message layer acquisition state machine.

RETURN VALUE
  The cached off start frame number when the firmware starts the start
  acquisition attempt.

SIDE EFFECTS
  None

===========================================================================*/
static void gl1_msg_set_start_acq_FN(
              uint32   start_acq_FN,
              gas_id_t gas_id )
{
  /* Increment by 1 here as actual f/w acquisition starts in the next frame */
  gl1_msg_acq_data[gas_id].gl1_msg_start_acq_FN = ADD_FN( start_acq_FN, 1 );
}

uint32 gl1_msg_get_start_acq_FN( gas_id_t gas_id )
{
  return ( gl1_msg_acq_data[gas_id].gl1_msg_start_acq_FN );
}

