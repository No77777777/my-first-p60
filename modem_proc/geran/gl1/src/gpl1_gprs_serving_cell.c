/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

              L 1  S E R V I N G  C E L L  M E A S U R E M E N T S

GENERAL DESCRIPTION
    This module contains the processing required to perform serving cell
    measurements in both IDLE and TRANSFER modes. The modules in this file
    are the ISRs and callback functions.


INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/gpl1_gprs_serving_cell.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who       what, where, why
--------   --------  ----------------------------------------------------------
10/05/17   sk        CR2024440 Add an explicit intlock for DivRx meas buffer update
21/04/17   br        CR1041084 FR 36219: GDET – GMSK Modulation Detector
13/05/16   db        CR937805. DL BLER for GPRS and EGPRS DL TBF
04/06/15   ab        CR817146 : DSDS: GBTA - Apply Generic Mutex Level Protection for GBTA concurrency
11/05/15   pjr       CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
30/01/15   pa        CR703985:GSM RxD RxLEV report should be based on maximum of PRx and DRx average RSSIs. 
18/11/14   ab        CR758820 PCHXFER : Correct Featurization and Enable for FR21036 : Own in PCH in Transfer for SS.
12/09/14   nm        CR723570 GL1 changes for FR21036 - Read own PCH in transfer to BO2.0(to be leveraged for TH/JO)
29/10/14   ab        CR745429 - Featurization correction for PCHXFER for SS / CR merges.
12/09/14   nm        CR723570 GL1 changes for FR21036 - Read own PCH in transfer to BO2.0(to be leveraged for TH/JO)
04/09/14   am        CR682760 Addressing KW GL1 P1 errors
12/08/14   zc        CR708666 Disable Interference measurements if WTR hopping started
23/07/14   pjr       CR682764 TP IR Feature - implement IR mem Full handling from GFW to GRLC
06/08/14   df        CR536059 Allow Asynchronous sleep for all idle activities
30/05/14   ws        CR657674 Data + MMS Phase 1
20/09/13   zc        CR546674 Making EGPL1_MEAN_BEP_MOVING_AVERAGE dual dataspaced
15/08/13   sk        CR524039 GBTA bringup changes
24/08/11   kb        CR302696 - Added initialization function to initialize gprs serving cell values
26/03/12   jj        CR344695 rename feature name FEATURE_GSM_GPRS_MSC34 to FEATURE_GSM_GPRS_MSC33
24/11/10   cja       Integrate HMSC
14/06/10   ab        CR: 235571 - 3GPP: Radio blocks for channel quality reporting
24/03/10   ab        CR: 229730 - UE is not reporting Link Quality Measurements as per spec
09-10-28   tjw       Clean up medium Lint errors
17/08/09   ap        CR190902 BER support needed for PS in 2G
06/05/09   ap        Fix Compile Issue
12/03/09   ws        CR 173134 Added Test mode A to L1 interface and calculate
                     RLA_P for test modes from PDCH
26/01/09   og        Ensure rxqual calculations ignore CS4 radio blocks. Resolves
                     CR170214.
24/11/08   og        FEATURE_GSM_GPRS_MSC33 GPL1 changes.
05/11/07   og        Add NV item 1027 support to EGPRS BEP.
24/09/07   og        Handle the error fatal case where EGPRS DL TBF is released
                     but queued nodes still exist.
12/07/07   ws        Resolved Compile warning in gpl1_egprs_create_qual_meas_node()
06/07/07   og        Delay the EPDAN REQ generation so it occurs four
                     tdma frames before transmission. Resolves CR122066.
05/03/07   ws        Fixed filtered lint warnings
23/06/06   og        Add protection against divide by zero when processing egprs
                     quality measurement reporting.
28/02/06   rn        Changed the new_gmac_trans_calc_e_pdan_size interface.
                     size_of_channel_req is now called channel_req_present and
                     is a boolean.
12/08/05   ws        Updated for initial DTM release
01/08/05   og        Correction to ul/dl ptcch booking frame numbers when doing
                     interference measurements.
07/29/05   gfr       Support for NPL1 DTM interface changes
23/06/05   og        Lint error/warnings removal.
20/06/05   og        Changes to the egprs quality reporting as requested by the
                     Systems Group.
17/06/05   og        Fix to the raw bep moving average filter.
17/06/05   og        Add a timeslot validity check in the serv cell data callback.
13/06/05   og        Fix to the bler sliding window including header crc-failed
                     data blocks.
09/06/05   og        Implement a mean bep moving average filter.
09/06/05   og        Change the setting of the interference measurements structure
                     flag in the timeslot link quality measurements IE.
02/06/05   og        Removal of header-header includes.
31/05/05   og        Inform the egprs qual meas module of the broadcast bep period
                     directly.
26/05/05   og        Perform comparisons between previous and current bep values
                     when block error rate is less than %20.
24/05/05   og        Save the quantised mean bep instead of the reported mean bep
                     for future quality report comparisons.
16/05/05   og        Additional mean bep reporting changes.
11/05/05   og        Take TFI of the data block into consideration when
                     sliding the bler window.
10/05/05   og        Reset bler sliding window structures when leaving egprs tbf.
09/05/05   og        Addition of BLER sliding window reference to mean bep.
04/05/05   og        Update the egprs node removal routine so it handles any location
                     in the queue.
20/04/05   og        Addition of filtered bep params to the msg metrics log packet.
19/04/05   og        Changes to egprs bep report generation.
14/04/05   og        Increase Bep quality report debug printout.
14/04/05   og        Bep quality report generation fix.
07/04/05   og        Use npl1 mean/cv bep.
4/4/05     og        More robust egprs bep filter parameter validity checking.
30/3/05    og        Changes to egprs bep report generation.
16/2/05    og        Lint warnings removal.
11/2/05    og        Increase mean bep / cv bep calculations precision.
25/1/05    og        Modification to interference measurement tasking.
17/1/05    og        Lint error removal.
17/1/05    og        Extra interference/bep message debug added.
17/12/04   og        Perform rssi averaging more efficiently.
09/12/04   og        Fix to tx power control by tracking c value on ccch.
03/12/04   og        Corrected/added comments to clarify the fixed point format
                     of variables used in the bep quality meas.
01/12/04   og        Change the reporting of the invalid egprs forgetting factor.
30/11/04   og        Enable interference measurements when cuta is ON.
30/11/04   og        Change to report Mean Bep and CV Bep independent of the link
                     quality measurement mode.
22/11/04   og        Fix to egprs bep filtering reliability decay in the absence
                     of quality parameters.
12/11/04   og        Added code to handle BCCH/PBCCH BEP PERIOD field reserved case.
28/10/04   ws        Lint tidy up
15/10/04   og        Fix to the incrementing of ptcch/idle -frame interference
                     measurements counters.
14/10/04   og        Fixes to the handling of inteference measurements while
                     in egprs packet transfer.
14/10/04   og        Added initialisation structure and its use for edge qual report.
03/09/04   ws        Corrected setting of qual report size with and size without
                     in EGPRS PDAN REQ
27/08/04   ws        Added chan request present to channel qual report generation
30/06/04   og        Updates following integration using simulator.
23/06/04   og        Egprs Qual measurements handling.
05/05/04   kf        Changes for ts slamming.
13/05/04   rm        Updated lint cleanup with the main branch
28/04/04   rm        Lint cleanup and added ta interface to remove direct
                     reference to ta parameters
26/11/03   ws        Lint cleanup
06/11/03   gfr       Support for quad-band.
26/09/03   kf        Fix for power control on pdch
12/09/03   pjr       Lint tidy
17/07/03   ws        Downgraded MSG's as no longer required
09/07/03   pjr       Modified RLA_P count values.
04/07/03   npr       Minor Lint warning cleanup.
19/06/03   pjr       Modified serving cell measurements to use running average
16/06/03   pjr       Modified interference measurement scheduling when an uplink
                     and downlink PTCCH are scheduled.
15/05/03   pjr       Modified count checking for RLA_P average.
14/05/03   pjr       Modified QUALITY REPORT TIME to handle multiple timeslots
02/05/03   pjr       Modified suspension of interference measurements due to reconfigure
30/04/03   pjr       Added suspension of interference measurements due to reconfigure
25/04/03   pjr       Restrict interference measurements to a max of 3 during active
                     PTCCH frames.
14/03/03   pjr       Modified RLA_P average counts to account for different averages
                     in IDLE and TRANSFER
27/02/03   pjr       Added running average count to RLA_P average.
04/02/03   pjr       Modified switch statement in gpl1_gprs_serv_cell_data_cb to
                     allow for scheduling change to data callback.
17/01/03   pjr       Modified ptcch handling to cope with multislot.
16/01/03   pjr       Made ptcch_timeslot an integer and corrected for negative mode.
16/01/03   pjr       Added use of gprs_serv_cell_meas.suspended_tbf to stop interference
                     measurements during TBF suspension.
10/01/03   pjr       Removed l1_sci_abort_sb() and l1_sci_abort_fcb_tch()
08/01/03   pjr       Added intialisation of dual band parameter.
16/12/02   pjr       Added l1_sci_abort_sb() and l1_sci_abort_fcb_tch()
06/12/02   pjr       Removed debug.
26/11/02   pjr       Changed initialisation of power levels.
25/11/02   pjr       General tidy.
22/11/02   pjr       Added initialisation of channel powers
08/11/02   pjr       Made quality reporting time an array to handle pending
                     ack/nacks.
11/01/02   pjr       Created.


===========================================================================*/

#include "geran_variation.h"
#include "customer.h"


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "geran_tasks.h"

#include "l1_isr.h"
#include "gpl1_gprs_isr.h"
#include "l1_task.h"
#include "task.h"
#include "gl1_msg_g.h"
#include "gl1_defs.h"
#include "gl1_defs_g.h"
#include "gl1_sys_params.h"

#include "task.h"
#include "gpl1_gprs_task.h"
#include "gpl1_gprs_utils.h"
#include "gpl1_gprs_ptcch.h"
#include "l1_utils.h"
#include "gpl1_gprs_log.h"
#include "gpl1_gprs_serving_cell.h"
#ifdef GL1_GMSK_MOD_DET
#include "gprs_mem.h"
#endif

#include "grlcl1.h"


#ifdef PL1_FW_SIM

/* set test harness signal */
#include "pl1_test_harness.h"

#endif

#ifdef FEATURE_GSM_WTR_HOP
#include "gl1_msg_wtr_hop.h"
#endif

#include "geran_eng_mode_info.h"

#ifdef FEATURE_GPRS_GBTA
#include "gpl1_dual_sim.h"
#endif /* FEATURE_GPRS_GBTA */

#include "gmutex.h"

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/

/*========================== #defines =====================================*/

#define  DIVIDE_BY_2_SHIFT_VALUE  1
#define  DIVIDE_BY_4_SHIFT_VALUE  2

#define  NUM_CODING_SCHEMES  15    /* CS1-4 + PAB + EPAB + MCS1-9 */
#define  MOD13_FN_COUNTER    13

#define  NUM_RRBP_VALUES  4

#define  RRBP_0_MIN_FRAMES_TO_WAIT  (13)
#define  RRBP_1_MIN_FRAMES_TO_WAIT  (17)
#define  RRBP_2_MIN_FRAMES_TO_WAIT  (21)
#define  RRBP_3_MIN_FRAMES_TO_WAIT  (26)

/* Used to define bep period value mapping array */
#define  NUM_BEP_PERIOD_VALUES   16
#define  NUM_BEP_PERIOD2_VALUES  15

#define  NUM_MEAN_BEP_VALUES  32
#define  NUM_CV_BEP_VALUES    8

/* Used to define mapping array */
#define  NUM_QUAL_MEAS_MODES  (INVALID_MEAS_MODE + 1)

/* Used to test the validity of the broadcast
 * bep period value.
 */
#define  HIGHEST_VALID_BEP_PERIOD_VALUE  (15)

/* bep period2 field value */
#define  GPRS_PL1_BEP_PERIOD2_NORMAL     (15)

/* MPKC */
/* Used for Eng Mode */
static uint8 lq_meas_mode[NUM_GERAN_DATA_SPACES];

/* This is used when the egprs quality report is about to
 * include mean/cv bep for both modulation schemes. The number
 * of filtered blocks has to be equal or larger than this value
 * for it to included in the report.
 * This is used to help achieve smoother GMSK<->8PSK
 * transitions.
 */
#if  EGPL1_OPTIMISE_MEAN_BEP

#define  EGPL1_MIN_FILT_BLKS_TO_REPORT  3U

/* BLER sliding window */
#define  EGPL1_DATA_BLK_WINDOW_SIZE     20U

/* Percentage threshold values in F0.16 format */
#define  EGPL1_20_PERCENT_BLER   13107U
#define  EGPL1_30_PERCENT_BLER   19661U
#define  EGPL1_50_PERCENT_BLER   32768U

/* Defines the number of data blocks in error a particular MCS
 * scheme must at least meet inorder for its BLER value to be
 * taken into account.
 */
#define  EIGHTY_PERCENT_OF_WINDOW_SIZE  (((EGPL1_DATA_BLK_WINDOW_SIZE) * (80) ) / (100))

/* Used to indicate the non validity of mean/cv bep values
 * stored in the moving average filter.
 * This is because either a block has not been received
 * yet or crc failed or tfi mismatched.
 */
#define  INVALID_MEAN_BEP  0xFFFFFFFF
#define  INVALID_CV_BEP    0xFFFFFFFF

/* Used to define the number of elements used
 * for the moving average filter i.e. number
 * of mean bep values stored.
 * Applies for GMSK/8PSK.
 */
#define  EGPL1_MOVING_AVERAGE_SIZE  (4U)

#endif /* EGPL1_OPTIMISE_MEAN_BEP */

#define GL1_MAX_INTERF_MEAS_PER_FRAME (4)
#ifdef GL1_GMSK_MOD_DET
extern uint16 *decStat[NUM_GERAN_DATA_SPACES] ;
#endif
/*========================== Macros =======================================*/

/* Macro used to confirm whether or not the queue is empty of any nodes */
#define  IS_QUEUE_EMPTY( queue_head_ptr )  ( (queue_head_ptr) == NULL ? TRUE: FALSE )

/*========================== Local types/enums ============================*/

typedef enum
{
  NO_FRAME_AVAILABLE,
  PTCCH_FRAME_AVAILABLE,
  IDLE_FRAME_AVAILABLE
} interference_meas_frame_E;

typedef enum
{
  DOWNLINK,
  UPLINK,
  NOT_AVAILABLE
} ptcch_timeslot_e;

#if  EGPL1_OPTIMISE_MEAN_BEP
typedef struct
{
  uint8  num_blks_failed;
  uint8  num_blks_total;
} bler_num_blocks_T;

/* Moving average window element,
 * used for storing mean bep and cv bep.
 */
typedef struct
{
  uint32  mean_bep;
  uint32  cv_bep;
} moving_avg_window_element_T;

/* Defines the per-timeslot structure for storing
 * the moving average mean bep values.
 */
typedef struct
{
  /* moving average mean bep window
   * This always contains the last 4
   * mean/cv beps received.
   */
  moving_avg_window_element_T   moving_avg_window[ EGPL1_MOVING_AVERAGE_SIZE ];

  /* pointer to the last element received */
  moving_avg_window_element_T*  mov_avg_wndw_element_ptr;

} ts_mean_bep_moving_avg_T;

#endif /* EGPL1_OPTIMISE_MEAN_BEP */

/*========================== Local function declarations ==================*/

/* GPRS serving cell measurements initialisation routine. */
static void  gpl1_gprs_serv_cell_init( gprs_pl1_serving_cell_meas_T* gpl1_sc_ptr, gas_id_t gas_id );

/* Used to task npl1 to perform interference measurements */
static boolean  gpl1_interf_measurements( transfer_data_T              *l1_transfer_data_ptr,
                                          gprs_pl1_serving_cell_meas_T *gpl1_sc_ptr,
                                          l1_mode_command_T             mode,
                                          uint32                        FN,
                                          gas_id_t gas_id  );

static void  gpl1_egprs_process_c_value( gprs_pl1_serving_cell_meas_T *gpl1_sc_ptr, gas_id_t gas_id );

/* Used for adding nodes to the tail of the queue */
static void  gpl1_egprs_add_node( gpl1_egprs_qual_meas_queue_T **node_pptr,
                                  gpl1_egprs_qual_meas_queue_T **head_pptr,
                                  gpl1_egprs_qual_meas_queue_T **tail_pptr );

/* Used for removing nodes off the head of the queue */
static void  gpl1_egprs_remove_node( gpl1_egprs_qual_meas_queue_T **node_pptr,
                                     gpl1_egprs_qual_meas_queue_T **head_pptr,
                                     gpl1_egprs_qual_meas_queue_T **tail_pptr );

/* Used to find out the reported measurement value using
 * the log scale provided
 */


static void  gpl1_egprs_serv_cell_ota_data_cb( gl1_defs_tn_type                   timeslot,
                                               const gpl1_egprs_decode_metrics_T *egprs_decode_metrics_ptr,
                                               boolean                            tfi_match,
                                               usf_dl_tfi_info_t*                 usf_dl_tfi_info_ptr,
                                               gas_id_t gas_id );

static void  gpl1_egprs_serv_cell_gcf_data_cb( gl1_defs_tn_type                   timeslot,
                                               const gpl1_egprs_decode_metrics_T *egprs_decode_metrics_ptr,
                                               boolean                            tfi_match,
                                               usf_dl_tfi_info_t*                 usf_dl_tfi_info_ptr,
                                               gas_id_t gas_id );

static void  gpl1_egprs_serv_cell_qual_meas_report_ota( egprs_chn_qual_rpt_t *qual_report_msg_ptr,
                                                        uint8                 dl_timeslot_alloc,
                                                        gas_id_t gas_id );

static void  gpl1_egprs_serv_cell_qual_meas_report_gcf( egprs_chn_qual_rpt_t *qual_report_msg_ptr,
                                                        uint8                 dl_timeslot_alloc,
                                                        gas_id_t gas_id );

typedef void  (*gpl1_egprs_previous_qual_meas_report_type)( egprs_chn_qual_rpt_t*  qual_report_msg_ptr,uint8 dl_timeslot_alloc, gas_id_t gas_id );
gpl1_egprs_previous_qual_meas_report_type gpl1_egprs_previous_qual_meas_report_fptr[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(gpl1_egprs_serv_cell_qual_meas_report_ota) };

/* Use a local static pointer */
typedef void  (*gpl1_egprs_previous_data_cb_type) ( gl1_defs_tn_type  timeslot, const gpl1_egprs_decode_metrics_T*  egprs_decode_metrics_ptr,boolean  tfi_match, usf_dl_tfi_info_t* usf_dl_tfi_info_ptr, gas_id_t  gas_id );
gpl1_egprs_previous_data_cb_type gpl1_egprs_previous_data_cb_fptr[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(gpl1_egprs_serv_cell_ota_data_cb) };


/*======================== Local Data =====================================*/


/* Used for quick extraction of the modulation type from the coding type */
static const gl1_defs_modulation_type GPL1_EGPRS_CODING_2_MODULATION[ NUM_CODING_SCHEMES ]=
{
    GL1_DEFS_MODULATION_GMSK,  /* GL1_DEFS_CS1_CODING        */
    GL1_DEFS_MODULATION_GMSK,  /* GL1_DEFS_CS2_CODING        */
    GL1_DEFS_MODULATION_GMSK,  /* GL1_DEFS_CS3_CODING        */
    GL1_DEFS_MODULATION_GMSK,  /* GL1_DEFS_CS4_CODING        */
    GL1_DEFS_MODULATION_GMSK,  /* GL1_DEFS_ACCESS_CODING     */
    GL1_DEFS_MODULATION_GMSK,  /* GL1_DEFS_EXT_ACCESS_CODING */
    GL1_DEFS_MODULATION_GMSK,  /* GL1_DEFS_MCS1_CODING       */
    GL1_DEFS_MODULATION_GMSK,  /* GL1_DEFS_MCS2_CODING       */
    GL1_DEFS_MODULATION_GMSK,  /* GL1_DEFS_MCS3_CODING       */
    GL1_DEFS_MODULATION_GMSK,  /* GL1_DEFS_MCS4_CODING       */
    GL1_DEFS_MODULATION_8PSK,  /* GL1_DEFS_MCS5_CODING       */
    GL1_DEFS_MODULATION_8PSK,  /* GL1_DEFS_MCS6_CODING       */
    GL1_DEFS_MODULATION_8PSK,  /* GL1_DEFS_MCS7_CODING       */
    GL1_DEFS_MODULATION_8PSK,  /* GL1_DEFS_MCS8_CODING       */
    GL1_DEFS_MODULATION_8PSK   /* GL1_DEFS_MCS9_CODING       */
};

static const uint8 GPL1_EGPRS_RRBP_FN_REDUCTION[ MOD13_FN_COUNTER ]=
{
  5, 6, 7, 8, 4, 5, 6, 7, 4, 5, 6, 7, 4
};

/* Provides a lookup table to test whether/not the value of bep period,
 * broadcast on bcch/ pbcch is reserved or not.
 */
static const boolean  EGPL1_BEP_PERIOD_FIELD_RESERVED[ NUM_BEP_PERIOD_VALUES ]=
{
 /* 0      1      2      3      4      5      6      7      8      9      10   */
    FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
 /* 11     12     13     14     15 */
    TRUE,  TRUE,  TRUE,  TRUE,  TRUE
};

/* Provides a look up table to convert bep period field value into the
 * filter forgetting factor e in F6.10 format.
 * Warning: A check on field value = 15 to be done before passing the value
 * into the array.
 */
static const uint16  GPRS_PL1_BEP_PERIOD_2_E[ NUM_BEP_PERIOD2_VALUES ]=
{
    BEP_PERIOD2_FIELD_VALUE_0,  BEP_PERIOD2_FIELD_VALUE_1,  BEP_PERIOD2_FIELD_VALUE_2,
    BEP_PERIOD2_FIELD_VALUE_3,  BEP_PERIOD2_FIELD_VALUE_4,  BEP_PERIOD2_FIELD_VALUE_5,
    BEP_PERIOD2_FIELD_VALUE_6,  BEP_PERIOD2_FIELD_VALUE_7,  BEP_PERIOD2_FIELD_VALUE_8,
    BEP_PERIOD2_FIELD_VALUE_9,  BEP_PERIOD2_FIELD_VALUE_10, BEP_PERIOD2_FIELD_VALUE_11,
    BEP_PERIOD2_FIELD_VALUE_12, BEP_PERIOD2_FIELD_VALUE_13, BEP_PERIOD2_FIELD_VALUE_14
};

/* Provides a look up table to map mean bep values onto a log scale
 * Used for the GMSK modulation scheme.
 */
const uint32  GPRS_PL1_GMSK_MEAN_BEP_LOG_SCALE[ NUM_MEAN_BEP_VALUES ]=
{
    GMSK_MEAN_BEP_0 , GMSK_MEAN_BEP_1 , GMSK_MEAN_BEP_2 , GMSK_MEAN_BEP_3 ,
    GMSK_MEAN_BEP_4 , GMSK_MEAN_BEP_5 , GMSK_MEAN_BEP_6 , GMSK_MEAN_BEP_7 ,
    GMSK_MEAN_BEP_8 , GMSK_MEAN_BEP_9 , GMSK_MEAN_BEP_10, GMSK_MEAN_BEP_11,
    GMSK_MEAN_BEP_12, GMSK_MEAN_BEP_13, GMSK_MEAN_BEP_14, GMSK_MEAN_BEP_15,
    GMSK_MEAN_BEP_16, GMSK_MEAN_BEP_17, GMSK_MEAN_BEP_18, GMSK_MEAN_BEP_19,
    GMSK_MEAN_BEP_20, GMSK_MEAN_BEP_21, GMSK_MEAN_BEP_22, GMSK_MEAN_BEP_23,
    GMSK_MEAN_BEP_24, GMSK_MEAN_BEP_25, GMSK_MEAN_BEP_26, GMSK_MEAN_BEP_27,
    GMSK_MEAN_BEP_28, GMSK_MEAN_BEP_29, GMSK_MEAN_BEP_30, GMSK_MEAN_BEP_31
};

/* Provides a look up table to map mean bep values onto a log scale
 * Used for the 8PSK modulation scheme.
 */
const uint32  GPRS_PL1_8PSK_MEAN_BEP_LOG_SCALE[ NUM_MEAN_BEP_VALUES ]=
{
    PSK8_MEAN_BEP_0 , PSK8_MEAN_BEP_1 , PSK8_MEAN_BEP_2 , PSK8_MEAN_BEP_3 ,
    PSK8_MEAN_BEP_4 , PSK8_MEAN_BEP_5 , PSK8_MEAN_BEP_6 , PSK8_MEAN_BEP_7 ,
    PSK8_MEAN_BEP_8 , PSK8_MEAN_BEP_9 , PSK8_MEAN_BEP_10, PSK8_MEAN_BEP_11,
    PSK8_MEAN_BEP_12, PSK8_MEAN_BEP_13, PSK8_MEAN_BEP_14, PSK8_MEAN_BEP_15,
    PSK8_MEAN_BEP_16, PSK8_MEAN_BEP_17, PSK8_MEAN_BEP_18, PSK8_MEAN_BEP_19,
    PSK8_MEAN_BEP_20, PSK8_MEAN_BEP_21, PSK8_MEAN_BEP_22, PSK8_MEAN_BEP_23,
    PSK8_MEAN_BEP_24, PSK8_MEAN_BEP_25, PSK8_MEAN_BEP_26, PSK8_MEAN_BEP_27,
    PSK8_MEAN_BEP_28, PSK8_MEAN_BEP_29, PSK8_MEAN_BEP_30, PSK8_MEAN_BEP_31,
};

/* Provides a look up table to map cv bep values onto a scale.
 * Used for both GMSK and 8PSK coding schemes (ETSI 05.08).
 * Values stored in F6.10 format.
 */
static const uint32  GPRS_PL1_CV_BEP_SCALE[ NUM_CV_BEP_VALUES ]=
{
    CV_BEP_0, CV_BEP_1, CV_BEP_2, CV_BEP_3, CV_BEP_4, CV_BEP_5, CV_BEP_6,
    CV_BEP_7
};

static const egprs_chn_qual_rpt_t  qual_report_msg_reset =
{
    FALSE, 0x0, 0x0, FALSE, 0x0, 0x0, 0x0, not_valid, 0x0, 0x0,
    { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 }, not_valid, 0x0,
    { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 }
};

static const egprs_rxqual_calc_T egprs_rxqual_calc_store_reset =
{ {{0,0,0,0,0,0},{0,0,0,0,0,0}},0};

static const boolean  EGPRS_PL1_INTERF_MEAS_MODE[ NUM_QUAL_MEAS_MODES ]=
{
    FALSE,  /* NO_MEASUREMENTS     */
    TRUE ,  /* INTERF_MEASUREMENTS */
    FALSE,  /* BEP_MEASUREMENTS    */
    TRUE ,  /* BOTH_MEASUREMENTS   */
    TRUE    /* INVALID_MEAS_MODE   */
};

static const uint8  EGPRS_PL1_INTERF_MEAS_BITMAP[ NUM_QUAL_MEAS_MODES ]=
{
    0x0,                                       /* NO_MEASUREMENTS     */
    ( TIMESLOTS_0_1_2_3 | TIMESLOTS_4_5_6_7 ), /* INTERF_MEASUREMENTS */
    0x0,                                       /* BEP_MEASUREMENTS    */
    TIMESLOTS_0_1_2_3,                         /* BOTH_MEASUREMENTS   */
    ( TIMESLOTS_0_1_2_3 | TIMESLOTS_4_5_6_7 )  /* INVALID_MEAS_MODE   */
};

#if  EGPL1_OPTIMISE_MEAN_BEP

/* bler sliding window */
static gpl1_egprs_bler_window_element_T  EGPL1_BLER_SLIDING_WINDOW[NUM_GERAN_DATA_SPACES][ EGPL1_DATA_BLK_WINDOW_SIZE ]  = 
{
  {
    { GL1_DEFS_ACCESS_CODING, FALSE }, { GL1_DEFS_ACCESS_CODING, FALSE }, { GL1_DEFS_ACCESS_CODING, FALSE },
    { GL1_DEFS_ACCESS_CODING, FALSE }, { GL1_DEFS_ACCESS_CODING, FALSE }, { GL1_DEFS_ACCESS_CODING, FALSE },
    { GL1_DEFS_ACCESS_CODING, FALSE }, { GL1_DEFS_ACCESS_CODING, FALSE }, { GL1_DEFS_ACCESS_CODING, FALSE },
    { GL1_DEFS_ACCESS_CODING, FALSE }, { GL1_DEFS_ACCESS_CODING, FALSE }, { GL1_DEFS_ACCESS_CODING, FALSE },
    { GL1_DEFS_ACCESS_CODING, FALSE }, { GL1_DEFS_ACCESS_CODING, FALSE }, { GL1_DEFS_ACCESS_CODING, FALSE },
    { GL1_DEFS_ACCESS_CODING, FALSE }, { GL1_DEFS_ACCESS_CODING, FALSE }, { GL1_DEFS_ACCESS_CODING, FALSE },
    { GL1_DEFS_ACCESS_CODING, FALSE }, { GL1_DEFS_ACCESS_CODING, FALSE }
  }
#ifdef FEATURE_DUAL_SIM
  ,{
    { GL1_DEFS_ACCESS_CODING, FALSE }, { GL1_DEFS_ACCESS_CODING, FALSE }, { GL1_DEFS_ACCESS_CODING, FALSE },
    { GL1_DEFS_ACCESS_CODING, FALSE }, { GL1_DEFS_ACCESS_CODING, FALSE }, { GL1_DEFS_ACCESS_CODING, FALSE },
    { GL1_DEFS_ACCESS_CODING, FALSE }, { GL1_DEFS_ACCESS_CODING, FALSE }, { GL1_DEFS_ACCESS_CODING, FALSE },
    { GL1_DEFS_ACCESS_CODING, FALSE }, { GL1_DEFS_ACCESS_CODING, FALSE }, { GL1_DEFS_ACCESS_CODING, FALSE },
    { GL1_DEFS_ACCESS_CODING, FALSE }, { GL1_DEFS_ACCESS_CODING, FALSE }, { GL1_DEFS_ACCESS_CODING, FALSE },
    { GL1_DEFS_ACCESS_CODING, FALSE }, { GL1_DEFS_ACCESS_CODING, FALSE }, { GL1_DEFS_ACCESS_CODING, FALSE },
    { GL1_DEFS_ACCESS_CODING, FALSE }, { GL1_DEFS_ACCESS_CODING, FALSE }
  }
#endif
#ifdef FEATURE_TRIPLE_SIM

 ,{
    { GL1_DEFS_ACCESS_CODING, FALSE }, { GL1_DEFS_ACCESS_CODING, FALSE }, { GL1_DEFS_ACCESS_CODING, FALSE },
    { GL1_DEFS_ACCESS_CODING, FALSE }, { GL1_DEFS_ACCESS_CODING, FALSE }, { GL1_DEFS_ACCESS_CODING, FALSE },
    { GL1_DEFS_ACCESS_CODING, FALSE }, { GL1_DEFS_ACCESS_CODING, FALSE }, { GL1_DEFS_ACCESS_CODING, FALSE },
    { GL1_DEFS_ACCESS_CODING, FALSE }, { GL1_DEFS_ACCESS_CODING, FALSE }, { GL1_DEFS_ACCESS_CODING, FALSE },
    { GL1_DEFS_ACCESS_CODING, FALSE }, { GL1_DEFS_ACCESS_CODING, FALSE }, { GL1_DEFS_ACCESS_CODING, FALSE },
    { GL1_DEFS_ACCESS_CODING, FALSE }, { GL1_DEFS_ACCESS_CODING, FALSE }, { GL1_DEFS_ACCESS_CODING, FALSE },
    { GL1_DEFS_ACCESS_CODING, FALSE }, { GL1_DEFS_ACCESS_CODING, FALSE }
  }
#endif
};

/* Used to inform whether the received coding scheme is dualpayload or not */
static const boolean  EGPL1_IS_MCS_DUAL_PAYLOAD[ NUM_CODING_SCHEMES ]=
{
    FALSE,  /* GL1_DEFS_CS1_CODING        */
    FALSE,  /* GL1_DEFS_CS2_CODING        */
    FALSE,  /* GL1_DEFS_CS3_CODING        */
    FALSE,  /* GL1_DEFS_CS4_CODING        */
    FALSE,  /* GL1_DEFS_ACCESS_CODING     */
    FALSE,  /* GL1_DEFS_EXT_ACCESS_CODING */
    FALSE,  /* GL1_DEFS_MCS1_CODING       */
    FALSE,  /* GL1_DEFS_MCS2_CODING       */
    FALSE,  /* GL1_DEFS_MCS3_CODING       */
    FALSE,  /* GL1_DEFS_MCS4_CODING       */
    FALSE,  /* GL1_DEFS_MCS5_CODING       */
    FALSE,  /* GL1_DEFS_MCS6_CODING       */
    TRUE,   /* GL1_DEFS_MCS7_CODING       */
    TRUE,   /* GL1_DEFS_MCS8_CODING       */
    TRUE    /* GL1_DEFS_MCS9_CODING       */
};

/* const structure used to reset the moving average structure contents */
static const ts_mean_bep_moving_avg_T  EGPL1_MOVING_AVERAGE_RESET =
{
  {
    { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
    { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP }
  },
  NULL
};

/* mean bep moving average structure */
static ts_mean_bep_moving_avg_T  EGPL1_MEAN_BEP_MOVING_AVERAGE[ NUM_GERAN_DATA_SPACES ][ NUM_MODULATION_SCHEMES ][ GL1_DEFS_SLOTS_IN_FRAME ]=
{
    {{ { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 0 */
      { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 1 */
      { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 2 */
      { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 3 */
      { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 4 */
      { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 5 */
      { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 6 */
      { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 7 */
    }, /* GMSK */

    { { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 0 */
      { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 1 */
      { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 2 */
      { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 3 */
      { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 4 */
      { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 5 */
      { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 6 */
      { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 7 */
    } } /* 8PSK */
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   ,{{ { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 0 */
      { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 1 */
      { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 2 */
      { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 3 */
      { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 4 */
      { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 5 */
      { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 6 */
      { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 7 */
    }, /* GMSK */

    { { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 0 */
      { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 1 */
      { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 2 */
      { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 3 */
      { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 4 */
      { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 5 */
      { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 6 */
      { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 7 */
    } } /* 8PSK */
#ifdef FEATURE_TRIPLE_SIM
  ,{{ { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 0 */
      { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 1 */
      { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 2 */
      { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 3 */
      { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 4 */
      { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 5 */
      { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 6 */
      { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 7 */
    }, /* GMSK */

    { { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 0 */
      { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 1 */
      { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 2 */
      { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 3 */
      { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 4 */
      { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 5 */
      { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 6 */
      { { { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP },
          { INVALID_MEAN_BEP, INVALID_CV_BEP }, { INVALID_MEAN_BEP, INVALID_CV_BEP } }, NULL
      }, /* TS 7 */
    } } /* 8PSK */
#endif /* FEATURE_TRIPLE_SIM */
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
};

#endif /* EGPL1_OPTIMISE_MEAN_BEP */


typedef struct
{
  /*======================== Local Data =====================================*/

  /* Running measurement average buffer */
  dBx16_T                       gprs_serv_cell_meas_buf[L1_GPRS_SERV_CELL_MEAS_BUF_LEN];


  /* Head and tail of queue */
  gpl1_egprs_qual_meas_queue_T  *head_ptr ;
  gpl1_egprs_qual_meas_queue_T  *tail_ptr ;


  /* Data callback function pointer, this is setup according to the
   * Anite GCF flag and effectively decides whether the OTA or GCF
   * routines are executed.
   * Assume the OTA routine is the default one.
   */
  void  (*gpl1_egprs_data_cb_fptr)

  ( gl1_defs_tn_type                    timeslot,
    const gpl1_egprs_decode_metrics_T*  egprs_decode_metrics_ptr,
    boolean                             tfi_match,
    usf_dl_tfi_info_t*                  usf_dl_tfi_info_ptr,
    gas_id_t gas_id
  ) ;

  /* Quality Measurement report function pointer, this is setup according to the
   * Anite GCF flag and effectively decides whether the OTA or GCF
   * routines are executed.
   * Assume the OTA routine is the default one.
   */
  void  (*gpl1_egprs_qual_meas_report_fptr)
  ( egprs_chn_qual_rpt_t*  qual_report_msg_ptr,
    uint8                  dl_timeslot_alloc,
    gas_id_t gas_id
  ) ;


#if EGPL1_OPTIMISE_MEAN_BEP
  gpl1_egprs_bler_window_element_T*  window_element_ptr ;

  /* This variable is used to implement a very simple
   * mean bep tracking loop when reporting egprs quality
   * measurements in MCS 9.
   */
  uint8  previous_mean_bep;
#endif /* EGPL1_OPTIMISE_MEAN_BEP */

  sys_algo_agc_T  interference_meas_gain;
  /* static Array to avoid dynamically allocating memory */
  gpl1_egprs_qual_meas_queue_T  qual_meas_node[ GPL1_EGPRS_QUAL_MEAS_QUEUE_SIZE ];
  uint8                         element ;
  /* static Array to avoid dynamically allocating memory */
  egprs_chn_qual_rpt_t  qual_report[ GPL1_EGPRS_QUAL_MEAS_QUEUE_SIZE ];
  uint8                 qual_report_index ;

  /* Size with/without variables */
  uint16  size_with;
  uint16  size_without;

  /* Structure used to store a copy of the quality report
   * msg during multislot polling.
   */
  egprs_chn_qual_rpt_t  copy_of_qual_report_msg;
  ARFCN_T        gprs_arfcns[4];

#if defined(FEATURE_GSM_RX_DIVERSITY_IDLE) || defined (FEATURE_GSM_RX_DIVERSITY_DATA)
  /* Running measurement average buffer */
  dBx16_T                       gprs_serv_cell_divrx_meas_buf[L1_GPRS_SERV_CELL_MEAS_BUF_LEN];
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA || FEATURE_GSM_RX_DIVERSITY_DATA*/
}gpl1_gprs_serving_cell_data_t;


static gpl1_gprs_serving_cell_data_t  gpl1_gprs_serving_cell_data[NUM_GERAN_DATA_SPACES];
// static gpl1_gprs_serving_cell_data_t *gpl1_gprs_serving_cell_data_ptr = gpl1_gprs_serving_cell_data;

/* The serving cell measurement structure */
gprs_pl1_serving_cell_meas_T  gprs_serv_cell_meas[NUM_GERAN_DATA_SPACES];
// gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=gprs_serv_cell_meas;

/*===========================================================================

FUNCTION init_gpl1_gprs_serving_cell_data

DESCRIPTION
             Initializes gprs serving cell values.

PARAMS
             None.

DEPENDENCIES

RETURN VALUE None

===========================================================================*/
void init_gpl1_gprs_serving_cell_data( gas_id_t gas_id )
{
  memset ( &gpl1_gprs_serving_cell_data[gas_id], 0 , sizeof ( gpl1_gprs_serving_cell_data_t) );
  gpl1_gprs_serving_cell_data[gas_id].head_ptr = NULL;
  gpl1_gprs_serving_cell_data[gas_id].tail_ptr = NULL;
  gpl1_gprs_serving_cell_data[gas_id].gpl1_egprs_data_cb_fptr = gpl1_egprs_serv_cell_ota_data_cb;
  gpl1_gprs_serving_cell_data[gas_id].gpl1_egprs_qual_meas_report_fptr = gpl1_egprs_serv_cell_qual_meas_report_ota;
#ifdef EGPL1_OPTIMISE_MEAN_BEP
  gpl1_gprs_serving_cell_data[gas_id].window_element_ptr = &( EGPL1_BLER_SLIDING_WINDOW[gas_id][ 0 ] );
#endif
  gpl1_gprs_serving_cell_data[gas_id].copy_of_qual_report_msg.gmsk_mean_bep_present = FALSE;
  gpl1_gprs_serving_cell_data[gas_id].copy_of_qual_report_msg.psk_mean_bep_present = FALSE;
  gpl1_gprs_serving_cell_data[gas_id].copy_of_qual_report_msg.bep_meas_rpt_flg = not_valid;
  gpl1_gprs_serving_cell_data[gas_id].copy_of_qual_report_msg.int_meas_rpt_flg = not_valid;
}


dBx16_T*   gpl1_gprs_get_serv_cell_meas_buf( gl1_power_meas_buffer_type prx_drx_buffer, 
                                             gas_id_t gas_id )
{
  dBx16_T *buffer = NULL;
  gpl1_gprs_serving_cell_data_t *gpl1_gprs_serving_cell_data_ptr = &gpl1_gprs_serving_cell_data[gas_id];

  switch (prx_drx_buffer) 
  {
    case PRX_MEAS_BUFFER:
      buffer = gpl1_gprs_serving_cell_data_ptr->gprs_serv_cell_meas_buf;
      break;

    case DIVRX_MEAS_BUFFER:
#if defined(FEATURE_GSM_RX_DIVERSITY_IDLE) || defined (FEATURE_GSM_RX_DIVERSITY_DATA)
      buffer = gpl1_gprs_serving_cell_data_ptr->gprs_serv_cell_divrx_meas_buf;
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA || FEATURE_GSM_RX_DIVERSITY_DATA*/
      break;

    default:
      break;
  }

  if (!buffer) 
  {
    MSG_GERAN_ERROR_1_G("Wrong buffer request %d", prx_drx_buffer);
  }
  return buffer; 
}

gpl1_egprs_qual_meas_queue_T*   gpl1_gprs_get_serv_cell_head_ptr( gas_id_t gas_id )
{
  gpl1_gprs_serving_cell_data_t *gpl1_gprs_serving_cell_data_ptr = &gpl1_gprs_serving_cell_data[gas_id];

  return gpl1_gprs_serving_cell_data_ptr->head_ptr;

}


/*========================= Local function Prototypes =====================*/

/*===========================================================================

FUNCTION  gpl1_gprs_serv_cell_init( )

DESCRIPTION
  Local serving cell measurements initialisation routine.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void  gpl1_gprs_serv_cell_init( gprs_pl1_serving_cell_meas_T* gpl1_sc_ptr, gas_id_t gas_id )
{
    /* Local loop variables and pointers */
    uint8   pch_value;
    uint32  i;
    uint32 *qual_report_time_ptr;
    uint32 *pch_ptr;

    gpl1_sc_ptr->cn                  = 0;
    gpl1_sc_ptr->c_value_burst_count = 0;
    gpl1_sc_ptr->task_event          = 0;
    gpl1_sc_ptr->rssi_buffer_ptr     = &( gpl1_sc_ptr->block_rssi[ BUFFER_1 ] );

    i = MAX_NUM_QUAL_RPRTS;
    qual_report_time_ptr = &( gpl1_sc_ptr->quality_report_time
                                           [ (MAX_NUM_QUAL_RPRTS - 1) ] );
    while( i-- > 0 )
    {
        *( qual_report_time_ptr-- ) = L1_GPRS_INVALID_FN;
    }
    qual_report_time_ptr++;

    gpl1_sc_ptr->rla_p_reported_value      = 0;
    gpl1_sc_ptr->c_index                   = 1;
    gpl1_sc_ptr->idle_int_freq_array_valid = FALSE;
    gpl1_sc_ptr->reported_rxlev            = GPL1_INVALID_RXLEV;

    /*  Convert the pmax value and use to initialise the power values */
    if( gpl1_sc_ptr->pmax < 5 )
    {
        pch_value = 5;
    }
    else
    {
        pch_value = gpl1_sc_ptr->pmax;
    }

    i = GL1_DEFS_SLOTS_IN_FRAME;
    pch_ptr = &( gpl1_sc_ptr->pch[ (GL1_DEFS_SLOTS_IN_FRAME-1) ] );

    while( i-- > 0 )
    {
        *( pch_ptr-- ) = (uint32) pch_value;
    }
    pch_ptr++;

    /*  Initialise the gamma_gsm to the 900 band value. This will be set later
        to the correct band, but by setting it here allows for the case when the
        serving cell task runs before this value has been set correctly.  */
    gpl1_sc_ptr->gamma_gsm = GAMMA_GSM900;

    /* Initialise the running average structure */
    l1_running_avg_init( &( gpl1_sc_ptr->rla_p_buffer ),
                         gpl1_gprs_get_serv_cell_meas_buf(PRX_MEAS_BUFFER, gas_id),
#if defined(FEATURE_GSM_RX_DIVERSITY_IDLE) || defined (FEATURE_GSM_RX_DIVERSITY_DATA)
                         gpl1_gprs_get_serv_cell_meas_buf(DIVRX_MEAS_BUFFER, gas_id),
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA || FEATURE_GSM_RX_DIVERSITY_DATA*/
                         (L1_GPRS_SERV_CELL_MEAS_BUF_LEN - 1),
                         gas_id);

    l1_running_avg_set_avg_len( &( gpl1_sc_ptr->rla_p_buffer ),
                                (L1_GPRS_SERV_CELL_MEAS_BUF_LEN - 2), gas_id );

    /* Initialise the BEP quality measurement module parameters */
    gpl1_egprs_serv_cell_qual_meas_reset( gas_id );
}

/*===========================================================================

FUNCTION  gpl1_interf_measurements( )

DESCRIPTION
  Local interfrence measurements handling routine.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static boolean  gpl1_interf_measurements( transfer_data_T              *l1_transfer_data_ptr,
                                          gprs_pl1_serving_cell_meas_T *gpl1_sc_ptr,
                                          l1_mode_command_T             mode,
                                          uint32                        FN,
                                          gas_id_t gas_id  )
{
    idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
    volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    gpl1_gprs_serving_cell_data_t *gpl1_gprs_serving_cell_data_ptr = &gpl1_gprs_serving_cell_data[gas_id];
    boolean  set_int_dllink_start = FALSE;
    uint8    tx_timeslot;
    boolean *tx_ts_ptr = &( gpl1_sc_ptr->tx_in_progress
                              [ (GL1_DEFS_SLOTS_IN_FRAME - 1) ] );
#ifdef FEATURE_GPRS_GBTA    
   gas_id_t idle_gas_id = GERAN_ACCESS_STRATUM_ID_1; 
#endif /* FEATURE_GPRS_GBTA*/
#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
   gpl1_g2x_ta_priority_t pri_ongoing;
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */

    if( gpl1_sc_ptr->ta_meas_in_prog == UL_DL_PTCCH )
    {

#if  DEBUG_GPL1_INTERF_MEAS
        #error code not present
#endif

      switch( frame_counters[gas_id].FNmod52 )
      {
      case  11:
      case  37:
          if( ( L1_TRANSFER_MODE == l1_tskisr_blk->l1_state )
            &&( L1_SINGLE_BLOCK_ALLOC != l1_transfer_data_ptr->current_alloc_type ) )
          {
              if( frame_counters[gas_id].FNmod104 == 89 )
              {
                /* If this is the final frame for reading
                 * the dl ptcch then reset the progress flag.
                 */
                gpl1_sc_ptr->ta_meas_in_prog = NO_PTCCH;
              }
              else
              {
                gpl1_sc_ptr->ta_meas_in_prog = DL_PTCCH;
              }
          }
          break;

      case  24:
      case  50:
          if( gpl1_sc_ptr->idle_frame_free )
          {
            /*  Clear idle_slot_free flag now it has been used. */
            gpl1_sc_ptr->idle_frame_free = FALSE;
          }
          break;

      default:
          break;
      }
    }
    else if(  ( FALSE != gpl1_sc_ptr->lowest_tn.valid )
            &&( L1_SINGLE_BLOCK_ALLOC != l1_transfer_data_ptr->current_alloc_type ) )
    {
        interference_meas_frame_E  int_frame_available = NO_FRAME_AVAILABLE;
        uint8                      ptcch_timeslot      = 0xff; /* Initialise to invalid */
        ptcch_timeslot_e           ptcch_ts_direction  = NOT_AVAILABLE;

        switch( frame_counters[gas_id].FNmod52 )
        {
        case  11:
        case  37:
            int_frame_available = PTCCH_FRAME_AVAILABLE;

            if( L1_TRANSFER_MODE == l1_tskisr_blk->l1_state )
            {
              switch( gpl1_sc_ptr->ta_meas_in_prog )
              {
              case UL_PTCCH:
                /* Determine ptcch ul physical timeslot */
                ptcch_timeslot     = gpl1_gprs_get_global_ta_timeslot(gas_id );
                ptcch_ts_direction = UPLINK;

                gpl1_sc_ptr->ta_meas_in_prog = NO_PTCCH;
                break;

              case DL_PTCCH:
                /* Determine ptcch dl physical timeslot */
                ptcch_timeslot     = gpl1_gprs_get_global_ta_timeslot(gas_id );
                ptcch_ts_direction = DOWNLINK;

                if( frame_counters[gas_id].FNmod104 == 89 )
                {
                  /* If this is the final frame for reading
                   * the dl ptcch then reset the progress flag.
                   */
                  gpl1_sc_ptr->ta_meas_in_prog = NO_PTCCH;
                }
                break;

              default:
                gpl1_sc_ptr->ta_meas_in_prog = NO_PTCCH;
                break;
              }

              /* if we are in EGPRS transfer mode then
               * obey the interf meas bitmap setup by the quality
               * measurement module.
               */
              if( TBF_MODE_EGPRS == l1_transfer_data_ptr->egprs_or_gprs_tbf )
              {
                if( FALSE == gpl1_sc_ptr->egprs_interf_meas_required )
                {
                  int_frame_available = NO_FRAME_AVAILABLE;

#if  DEBUG_GPL1_INTERF_MEAS
                  #error code not present
#endif

                }
              }
#if defined (FEATURE_GSM_GPRS_READ_CCCH_IN_XFER) || defined (FEATURE_GPRS_GBTA)
              /* Do not schedule Intf meas when GBTA Events are active */
              if( (int_frame_available != NO_FRAME_AVAILABLE) &&
                  (
#ifdef FEATURE_GPRS_GBTA
                     ( gl1_check_other_subs_idle_event_status(GBTA_STATUS_PWR_MON_SCHEDULED, gas_id,&idle_gas_id) || 
                      IS_GBTA_ACTIVE()
                     )
#endif /* FEATURE_GPRS_GBTA */

#if defined (FEATURE_GSM_GPRS_READ_CCCH_IN_XFER) && defined (FEATURE_GPRS_GBTA)
                    ||
#endif
#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER 
                    (gpl1_g2x_ta_is_activty_running(&pri_ongoing,gas_id))
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */
                  )
                )
              {
#ifdef FEATURE_GPRS_GBTA
                if(idle_gas_id < NUM_GERAN_DATA_SPACES)
                {
                  MSG_GERAN_MED_3_G("GBTA Intf meas not allowed gbta_active %d idle_ev_state %d GBTA state %d",
                             IS_GBTA_ACTIVE(),
                             IS_IDLE_EV_PWR_MON_SCHEDULED(idle_gas_id),GET_GBTA_STATE());
                }
                else
#endif /*FEATURE_GPRS_GBTA */
                {
                  MSG_GERAN_ERROR_0_G("GBTA Intf meas not allowed - no sub in idle mode or ccch in prog");
                }

                int_frame_available = NO_FRAME_AVAILABLE;
              }
#endif  /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER || defined (FEATURE_GPRS_GBTA) */
            }
            else if( FALSE == gpl1_sc_ptr->idle_int_freq_array_valid )
            {
              /* PTCCH frame in IDLE or ACCESS modes.
               * Measurements are only taken in IDLE/ACCESS modes if on 52-idle and
               * the idle_int_freq_array_valid flag is set indicating a valid array
               * of frequencies to be measured exists
               */
              /*  Idle 51 - no measurements performed.  */
              int_frame_available = NO_FRAME_AVAILABLE;
            }
            break;

        case  24:
        case  50:
            if( gpl1_sc_ptr->idle_frame_free )
            {
              /*  Clear idle_slot_free flag now it has been used. */
              gpl1_sc_ptr->idle_frame_free = FALSE;

              if( l1_tskisr_blk->l1_state == L1_TRANSFER_MODE )
              {
				/*  Frame is idle and available for interference measurements. */
				int_frame_available = IDLE_FRAME_AVAILABLE;
				
#ifdef FEATURE_GSM_WTR_HOP
		/*WTR hopping starts on FNmod26 = 23, which will collide with the interference measurements
		Can't do interference measurements when switching devices*/
		if(gl1_msg_wtr_hop_get_status() != WTR_HOP_INACTIVE)
		{
                  MSG_GERAN_MED_0_G("WTR Hopping Active, interference measurements disabled");
                  int_frame_available = NO_FRAME_AVAILABLE;
                }
#endif

                /* if we are in EGPRS transfer mode then
                 * obey the interf meas bitmap setup by the quality
                 * measurement module.
                 */
                if( TBF_MODE_EGPRS == l1_transfer_data_ptr->egprs_or_gprs_tbf )
                {
                  if( FALSE == gpl1_sc_ptr->egprs_interf_meas_required )
                  {
                    int_frame_available = NO_FRAME_AVAILABLE;

#if  DEBUG_GPL1_INTERF_MEAS
                    #error code not present
#endif

                  }
                }
              }
              else if( FALSE != gpl1_sc_ptr->idle_int_freq_array_valid )
              {
                /* Measurements are only taken in IDLE/ACCESS modes if on 52-idle and
                 * the idle_int_freq_array_valid flag is set indicating a valid array
                 * of frequencies to be measured exists.
                 */
                int_frame_available = IDLE_FRAME_AVAILABLE;
              }

#if defined (FEATURE_GSM_GPRS_READ_CCCH_IN_XFER) || defined (FEATURE_GPRS_GBTA)
            /* Do not schedule Intf meas when GBTA Events are active */
            if( (int_frame_available != NO_FRAME_AVAILABLE) &&
                (
#ifdef FEATURE_GPRS_GBTA
                  ( gl1_check_other_subs_idle_event_status(GBTA_STATUS_PWR_MON_SCHEDULED, gas_id,&idle_gas_id) || 
                    IS_GBTA_ACTIVE()
                  )
#endif /* FEATURE_GPRS_GBTA */

#if defined (FEATURE_GSM_GPRS_READ_CCCH_IN_XFER) && defined (FEATURE_GPRS_GBTA)
                  ||
#endif
#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER 
                  (gpl1_g2x_ta_is_activty_running(&pri_ongoing,gas_id))
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */
                )
              )
             {
 #ifdef FEATURE_GPRS_GBTA
               if(idle_gas_id < NUM_GERAN_DATA_SPACES)
               {
                 MSG_GERAN_MED_3_G("GBTA Intf meas not allowed gbta_active %d idle_ev_state %d GBTA state %d",
                            IS_GBTA_ACTIVE(),
                            IS_IDLE_EV_PWR_MON_SCHEDULED(idle_gas_id),GET_GBTA_STATE());
               }
               else
 #endif /*FEATURE_GPRS_GBTA */
               {
                 MSG_GERAN_ERROR_0_G("GBTA Intf meas not allowed - no sub in idle mode or ccch in prog");
               }
 
               int_frame_available = NO_FRAME_AVAILABLE;
             }
#endif  /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER || defined (FEATURE_GPRS_GBTA) */

            } /* End of if idle slot is free */
#if  DEBUG_GPL1_INTERF_MEAS
            #error code not present
#endif
            break;

        default:
            break;
        } /* End of switch statement on fn mod 52 */

      /*  Now schedule the measurement if a valid frame has been found  */
      if( int_frame_available != NO_FRAME_AVAILABLE )
      {
        /* Dont program the hw incase there is a pending assignment
         * with the start time 3 frames ahead since this is gonna cause
         * the hw to be re-programmed on the idle/ptcch frame
         * thus resulting in problems.
         */
        if( ( L1_MODE_RUN == mode )
          &&( !( ( l1_transfer_data_ptr->starting_time_valid )
               &&( 2 == SUB_FN( l1_transfer_data_ptr->starting_time, FN ) ) ) ) )
        {
            /* Only schedule interference measurements if not about to leave Transfer  */
            /*  If the TBF is suspended or a reconfigure is in progress then
                suspend interference measurements.  */
            if( FALSE == gpl1_sc_ptr->suspended_tbf )
            {
                /* Setup the interference measurements bitmap
                 * Initialise to allow measurements on all timeslots.
                 * TS: 0  1  2  3  4  5  6  7
                 *     1  1  1  1  1  1  1  1
                 * Note: These are the physical downlink timeslots.
                 */
                uint8 interf_meas_bitmap = 0xFF;

                /* Setup the maximum number of interference measurements
                 * per TDMA frame to the maximum number of allowed
                 * timeslots for this MS multislot class
                 */
                uint32  max_interf_meas_per_frame = GL1_MAX_INTERF_MEAS_PER_FRAME;

                uint8  timeslot;

                switch( l1_get_current_timeslot(gas_id) )
                {
                case  0:
                    /* if the lowest dl timeslot is TS0 then dont do an
                     * interference measurements on TS7 to allow receiver
                     * HW enough settling time for following TDMA frame.
                     */
                    interf_meas_bitmap &= 0xFE;
                case  1:  /*lint !e616 : no break statement for avoiding repitition */
                case  2:
                case  3:
                    /* Are there any transmits from the previous
                     * TDMA frame happening in TS0, TS1 and TS2 of this
                     * frame due to usf/rrbp ? Remove any
                     * interf measurements happening on these timeslots.
                     */
                    tx_timeslot = GL1_DEFS_SLOTS_IN_FRAME;
                    while( tx_timeslot-- > 5 )
                    {
                        /* ptr initialised to point at ts7 in beginning
                         * of function.
                         */
                        if( FALSE != ( *tx_ts_ptr ) )
                        {
                            uint8  tx_ts_mask = (uint8) ( 0x0080 >> (tx_timeslot - 5) );

                            interf_meas_bitmap &= ~( tx_ts_mask );
                            interf_meas_bitmap &= ~( tx_ts_mask>>1 );
                            interf_meas_bitmap &= ~( (uint8)(tx_ts_mask<<1) );
                        }
                        tx_ts_ptr--;
                    }
                    if( FALSE != ( *tx_ts_ptr ) )
                    {
                        /* If there is a tx on UL TS4 in the previous frame
                         * then remove any meas on TS0.
                         */
                        interf_meas_bitmap &= 0x7F;
                    }
                     /* reset pointer to ts7 location */
                    tx_ts_ptr =
                        &( gpl1_sc_ptr->tx_in_progress[ (GL1_DEFS_SLOTS_IN_FRAME - 1) ] );

                    /* Transmissions from the previous tdma frame are affected
                     * by the curent TA advance value, this means the 1TS guard
                     * period we introduced here is effectively reduced. If this
                     * causes the time between the transmit and the interference
                     * meas to be reduced below the hw required settling time
                     * then we need to start taking TA value into consideration.
                     */
                    break;

                case  7:
                    /* If the lowest dl timeslot is TS7 then remove interf
                     * measurements on TS0 to make sure there is 1 TS gap
                     * between receive in last frame and meas in this frame.
                     * Frame slamming would ensure we are protected from this
                     * case anyways.
                     */
                    interf_meas_bitmap &= 0x7F;
                    break;

                default:
                    break;
                }

                /* Due to frame interrupt being synced to lowest dl timeslot
                 * eliminiate any timeslots < current timeslot since
                 * MS cannot command npl1 to meas interf on these.
                 * Warning: This also ensures we meet the minimum inter meas
                 * requirements regarding TSmin -> TSmax. (ETSI 05.08).
                 */
                timeslot = l1_get_current_timeslot(gas_id);
                while( timeslot-- > 0 )
                {
                    interf_meas_bitmap &= (uint8) (~( 0x0080 >> timeslot ));
                }

                /* Remove any interference measurements happening
                 * on ptcch timeslot and its neighbouring timslots
                 * (prev and after) for receiver HW settling purposes.
                 * check whether this is the uplink or downlink slot.
                 */
                if( 0xFF != ptcch_timeslot )
                {
                    uint8  ts_bitmap        = (uint8) (0x0080 >> ptcch_timeslot);
                    uint8  ts_before_bitmap = (uint8) (ts_bitmap << 1);
                    uint8  ts_after_bitmap  = (uint8) (ts_bitmap >> 1);

                    switch( ptcch_ts_direction )
                    {
                    case DOWNLINK:
                        interf_meas_bitmap &=
                            ~( ts_before_bitmap | ts_bitmap | ts_after_bitmap );
                        break;

                    case UPLINK:
                        if( 0 == ptcch_timeslot )
                        {
                            /* Remove any measurements on TS2 since
                             * the 8bit bitmap mechanism wont be able
                             * to detect it.
                             */
                            interf_meas_bitmap &= ~(0x20);
                        }
                        else if( 5 == ptcch_timeslot )
                        {
                            interf_meas_bitmap &= 0xFE;
                        }
                        interf_meas_bitmap &=
                            ~( (uint8) ((ts_before_bitmap | ts_bitmap | ts_after_bitmap)>>3) );

                        /* A transmission is about to occur on the UL ptcch 2 tdma
                         * frames ahead. Due to SBI (serial bus i/f) hw limitations
                         * the maxmimum number of interference measurements needs to
                         * limited to 3 so as to ensure neither the measurements or
                         * ul ptcch transmission is corrupted.
                         */
                        max_interf_meas_per_frame = (GL1_MAX_INTERF_MEAS_PER_FRAME - 1);
                        break;

                    default:
                        break;
                    }
                }

                /* Now fill the tn array necessary to task the NPL1 layer
                 * Save some looping time, dont continue if bitmap is zero.
                 */
                if( 0 != interf_meas_bitmap )
                {
                  /* IDLE / ACCESS mode, determine what should be measured */
                  /* Set-up frequency for channels to be measured  */
                  gpl1_gprs_calculate_arfcns( l1_get_gprs_arfcns(gas_id),
                                              l1_transfer_data_ptr->frequency_info_ptr,
                                              PTCCH,gas_id );

                  /* No measurements shall be taken on the BCCH  carrier of the
                   * serving cell since the BTS transmits with constant power on
                   * this carrier. (ETSI 05.08).
                   */
                  if( l1_get_gprs_arfcns_value(0,gas_id).num != l1_idle_data->campedon_cell_ARFCN.num )
                  {


                      gl1_msg_meas_signal_type  signal_info;

                      gl1_defs_tn_type          tn[ GL1_MAX_INTERF_MEAS_PER_FRAME ] =
                          { GL1_DEFS_TN_0, GL1_DEFS_TN_0, GL1_DEFS_TN_0, GL1_DEFS_TN_0};

                      /* Pointer used to fill tn array values during for loop */
                      gl1_defs_tn_type *tn_ptr = &( tn[0] );

                      uint8  interf_meas_ts = 0;
                      uint8  num_tn         = 0;


                      /* Fill in the Gain settings for the measurements
                       * Treat as a totally new power measurement with medium
                       * receiver gain level.
                       */
                      gpl1_gprs_serving_cell_data_ptr->interference_meas_gain.pwr_dBm_x16 = SYS_PARAM_INIT_EXP_PWR_RX_LVL_dBm_x16;

                      /* Used when gain update/debug purposes.
                       * Does not matter in this case since interf measurements
                       * are not fed back.
                       * Filled out for completeness.
                       */
                      gpl1_gprs_serving_cell_data_ptr->interference_meas_gain.srl_enabled = TRUE;
                      gpl1_gprs_serving_cell_data_ptr->interference_meas_gain.time_stamp  = FN;

                      gpl1_gprs_serving_cell_data_ptr->interference_meas_gain.first_visit = TRUE;
                      gpl1_gprs_serving_cell_data_ptr->interference_meas_gain.valid       = TRUE;

                      gpl1_gprs_serving_cell_data_ptr->interference_meas_gain.FN          = FN;
                      gpl1_gprs_serving_cell_data_ptr->interference_meas_gain.arfcn       = l1_get_gprs_arfcns_value(0,gas_id);

                      /* Fill the signal info parameter */
                      signal_info.arfcn   = l1_get_gprs_arfcns_value(0,gas_id);

                      signal_info.agc_ptr = &(gpl1_gprs_serving_cell_data_ptr->interference_meas_gain);

                      while( interf_meas_ts < GL1_DEFS_SLOTS_IN_FRAME )
                      {
                        if( 0 != ( interf_meas_bitmap & ((uint8)(0x0080 >> interf_meas_ts ))))
                        {
                          if( ++num_tn > max_interf_meas_per_frame )
                          {
                            num_tn = (uint8) max_interf_meas_per_frame;
                            break;
                          }

                          /* Fill the local tn array in order
                           * Task npl1 in terms of relative timeslot.
                           */
                          *(tn_ptr++) = (gl1_defs_tn_type)
                              gpl1_gprs_ts_conv( interf_meas_ts,gas_id );
                        }
                        interf_meas_ts++;
                      } /* End of while loop */
#ifdef GL1_GMSK_MOD_DET
                       if(decStat[gas_id] == NULL)
					   {
					    decStat[gas_id] = (uint16 *)GPRS_MEM_CALLOC(4*(100+1));
		               }
#endif
                      /* Set up interference callback */
                      gl1_msg_cfg_int_meas( gpl1_gprs_serv_cell_int_cb, gas_id );
#ifdef GL1_GMSK_MOD_DET					  
					  gpl1_sc_ptr->int_results_info.decStat = decStat[gas_id];
#endif
                      /* Program npl1 to do interference measurements */
                      gl1_msg_do_int_meas( signal_info,
                                           &( tn[0] ),
                                           num_tn,
                                           &( gpl1_sc_ptr->int_results_info ), gas_id );

                      /* Debug output */
#if  DEBUG_GPL1_INTERF_MEAS
                      #error code not present
#else
                      MSG_GERAN_LOW_3_G("INT_MEAS %d, %d %d",
                              l1_get_current_timeslot(gas_id), num_tn, frame_counters[gas_id].FNmod52);
#endif
                      /*  Set flag so downlink start set next frame.  */
                      set_int_dllink_start = TRUE;
                  } /* End of if arfcn is not equal to BCCH arfcn */

#if  DEBUG_GPL1_INTERF_MEAS
                  #error code not present
#endif

                } /* End of if there are any timeslots to use for interf meas */

#if  DEBUG_GPL1_INTERF_MEAS
                #error code not present
#endif

            }   /* End of if tbf is not suspended */

#if  DEBUG_GPL1_INTERF_MEAS
            #error code not present
#endif

        }   /* End of if not leaving transfer && no assignment is approaching in 3 frames time */

#if  DEBUG_GPL1_INTERF_MEAS
        #error code not present
#endif

      }     /* End of if int_meas frame is !(NO_FRAME_AVAILABLE) */
    }       /* End of if DL_UL_PTCCH in process && lowest tn is valid
             * && current alloc is not single block.
             */

    tx_timeslot = GL1_DEFS_SLOTS_IN_FRAME;
    while( tx_timeslot-- > 0 )
    {
        *( tx_ts_ptr-- ) = FALSE;
    }
    tx_ts_ptr++;

    return  set_int_dllink_start;
} /* End of local function for tasking interference measurements */


/*===========================================================================

FUNCTION  gpl1_egprs_meas_scale( )

DESCRIPTION
  Local function used to scale a measurement according to the supplied
  array pointer.
  It is required for the EGPRS BER measurements.

DEPENDENCIES
  None

RETURN VALUE
  Scaled value.

SIDE EFFECTS
  None

===========================================================================*/
 uint8  gpl1_egprs_meas_scale( const uint32 *scale_ptr,
                                     uint32        target_measurement )
{
    uint8  reported_measurement = 0;

    while( (*scale_ptr) > target_measurement )
    {
        scale_ptr++;
        reported_measurement++;
    }
    return  reported_measurement;
}

/*===========================================================================

FUNCTION  gpl1_egprs_add_node( )

DESCRIPTION
  Local function used to add a node onto the quality measurement queue tail.
  It is required for the EGPRS BER measurements.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gpl1_egprs_add_node( gpl1_egprs_qual_meas_queue_T **node_pptr,
                                 gpl1_egprs_qual_meas_queue_T **head_pptr,
                                 gpl1_egprs_qual_meas_queue_T **tail_pptr )
{
    if( NULL == ( (*head_pptr)->next_ptr ) )
    {
        /* If there is one element in the queue
         * then extra care must be taken when inserting
         * the second node.
         */
        (*head_pptr)->next_ptr = *node_pptr;
    }

    (*node_pptr)->next_ptr = NULL;
    (*tail_pptr)->next_ptr = *node_pptr;
    (*tail_pptr)           = *node_pptr;
}

/*===========================================================================

FUNCTION  gpl1_egprs_remove_node( )

DESCRIPTION
  Local function used to remove a node off the quality measurement queue head.
  It is required for the EGPRS BER measurements.

  This function ensures that the node ptr ends up pointing to the node
  next to the one that was removed.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gpl1_egprs_remove_node( gpl1_egprs_qual_meas_queue_T **node_pptr,
                                    gpl1_egprs_qual_meas_queue_T **head_pptr,
                                    gpl1_egprs_qual_meas_queue_T **tail_pptr )
{
    if( (*node_pptr) == (*head_pptr) )
    {
        /* The node is on the head of the queue. */

        /* Point to the next element in the queue */
        (*node_pptr) = (*node_pptr)->next_ptr;

        /* Re position the head ptr */
        (*head_pptr) = (*node_pptr);

        if( NULL == (*node_pptr) )
        {
            /* If this is the only node in the queue
             * then re position the tail ptr.
             */
            (*tail_pptr) = NULL;
        }
    }
    else
    {
        /* The node is on the tail of the queue or
         * in between.
         */
        gpl1_egprs_qual_meas_queue_T* node_before_ptr =
          (*head_pptr);

        while( node_before_ptr->next_ptr != (*node_pptr) )
        {
            /* Search the queue for the node which is before
             * the one we wish to remove.
             * This search starts at the head of the queue and
             * ends at the tail.
             */
            node_before_ptr = node_before_ptr->next_ptr;
        }

        /* Remove the node by connecting the one before it
         * to the next element in the queue.
         */
        node_before_ptr->next_ptr = (*node_pptr)->next_ptr;

        /* Unlink the node */
        (*node_pptr)->next_ptr = NULL;

        if( (*node_pptr) == (*tail_pptr) )
        {
            /* If the removed node was on the tail
             * then re position the tail ptr.
             */
            (*tail_pptr) = node_before_ptr;

            /* Point to the next element in the queue */
            (*node_pptr) = NULL;
        }
        else
        {
            /* Move the node ptr to the following
             * node in the queue.
             */
            (*node_pptr) = node_before_ptr->next_ptr;
        }
    }
}

/*===========================================================================

FUNCTION  gpl1_egprs_process_c_value( )

DESCRIPTION
  Local function for running C value processing in a TDMA frame
  synchronous fashion.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void  gpl1_egprs_process_c_value( gprs_pl1_serving_cell_meas_T *gpl1_sc_ptr, gas_id_t gas_id )
{
    gprs_l1_idle_data_t *gprs_l1_idle_data = gl1_ms_switch_gprs_idle_data(gas_id);
    /* Local variables */
    uint16  forgetting_factor;
    int16   cblockn;

    /* Setup a local pointer to the power control parameters */
    pccch_power_control_params_t  *gpl1_pc_ptr =
        &( gprs_l1_idle_data->gprs_campedon_cell_data.power_control );

    /* Ssblockn or SSn has been calculated and the block has decoded correctly
       so now calculate the c_value. */
    /* Determine which measurement and calculations need to be performed. This
       decision is based on the value of pc_meas_chan and whether the state is
       IDLE or TRANSFER. */

    /*  Value passed in for t_avg_t is the 'k' in 2^(k/2) / 6.
        For k = 0, .., 25  */
    /*  Range check k */
    if( gpl1_pc_ptr->t_avg_t >= 25 )
    {
        gpl1_pc_ptr->t_avg_t = 25;
    }

    /*  Transfer state, now determine value of pc_meas_chan */
    if( FALSE != gpl1_pc_ptr->pc_meas_chan )
    {
        /* Power measurements made on PDTCH in transfer mode so use forgetting
           factor c, (See 05.08 sect. 10.2.3).
           The value cblockn is determined by adding the Pb value to the
           Ssblockn value. If frequency hopping is in progress the value
           of Pb is reduced by 25% for every burst received on the BCCH */
        cblockn = (int16)(gpl1_sc_ptr->ssblockn + gpl1_sc_ptr->weighted_pb);

        /*  So forgetting factor = 12 * t_avg_t equates to */
        forgetting_factor = (uint16) (1 << ((gpl1_pc_ptr->t_avg_t >> 1) + 1));
    }
    else
    {
        /*  Power measurements made on BCCH in transfer mode so use forgetting
            factor b  */
        cblockn = gpl1_sc_ptr->power_cntrl_avg;

        /*  So forgetting factor = 6 * t_avg_t equates to */
        forgetting_factor = (uint16) (1 << (gpl1_pc_ptr->t_avg_t >> 1));
    }
    /*  Increment sample count  */
    gpl1_sc_ptr->c_index++;

    /*  If the previous value of cn, cn_1 is zero then prime with current
        cn value    */
    if( 0 == gpl1_sc_ptr->cn_1 )
    {
        gpl1_sc_ptr->cn_1 = cblockn << 8; /*lint !e701 sign not lost during shift */
    }

    /* Obtain value of Cn. Note:- within the function a greater precision
       record of cn is kept to be used as Cn-1 next time round.
     */
    gpl1_sc_ptr->cn = gpl1_gprs_divide_ff( cblockn,
                                           &( gpl1_sc_ptr->cn_1 ),
                                           forgetting_factor );

    /* Use the cn value to calculate the RF output power. */
    gpl1_gprs_calc_rf_pwr( gas_id);
}

/*===========================================================================

FUNCTION   gpl1_egprs_serv_cell_gcf_data_cb( )

DESCRIPTION
  This function is basically the ota data cb but with EGPL1_OPTIMISE_MEAN_BEP
  switched off.

DEPENDENCIES
  1- Used only during EGPRS.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void  gpl1_egprs_serv_cell_gcf_data_cb( gl1_defs_tn_type                   timeslot,
                                               const gpl1_egprs_decode_metrics_T *egprs_decode_metrics_ptr,
                                               boolean                            tfi_match,
                                               usf_dl_tfi_info_t*                 usf_dl_tfi_info_ptr,
                                               gas_id_t gas_id )
{
    volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    gprs_l1_idle_data_t *gprs_l1_idle_data = gl1_ms_switch_gprs_idle_data(gas_id);
    /* Pointer to update the serving cell measurement
     * structure
     */
    gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
    gprs_pl1_serving_cell_meas_T *gprs_pl1_scell_meas_ptr = gprs_serv_cell_meas_ptr;
        /* Pointer to update the rx qual measurement */
    egprs_mean_bep_store_T  *egprs_rxqual_calc_ptr =
       &gprs_serv_cell_meas_ptr->egprs_rxqual_calc.egprs_mean_bep_store
        [gprs_serv_cell_meas_ptr->egprs_rxqual_calc.active_buffer];

    /* Setup a pointer to the current transfer data parameters */
    transfer_data_T*  transfer_data_ptr =
      l1_tskisr_blk->current_params.L1Data.pTransfer_data;

    /* local rssi pointer */
    block_rssi_t* rssi_buffer_ptr = gprs_pl1_scell_meas_ptr->rssi_buffer_ptr;

    /* Set the modulation type for this block.
     * work it out by mapping the coding scheme used.
     */
    const gl1_defs_modulation_type  modulation =
        GPL1_EGPRS_CODING_2_MODULATION[ egprs_decode_metrics_ptr->coding_type ];

    const boolean  decode_error      = egprs_decode_metrics_ptr->decode_error;
    boolean        filter_this_block = FALSE;

    /* Perform a final check on the forgetting factor e to
     * make sure it is valid for filtering.
     */
    if(  ( GPRS_PL1_FORGET_FACTOR_INVALID == gprs_pl1_scell_meas_ptr->e )
       ||( gprs_pl1_scell_meas_ptr->e > BEP_PERIOD2_FIELD_VALUE_0 )
       ||( gprs_pl1_scell_meas_ptr->e < BEP_PERIOD2_FIELD_VALUE_14 )
      )
    {
        if( ( GPRS_PL1_FORGET_FACTOR_INVALID != gprs_pl1_scell_meas_ptr->broadcast_e )
          &&( gprs_pl1_scell_meas_ptr->broadcast_e <= BEP_PERIOD2_FIELD_VALUE_0 )
          &&( gprs_pl1_scell_meas_ptr->broadcast_e >= BEP_PERIOD2_FIELD_VALUE_10 )
          )
        {
            /* Set the forgetting factor in use to a valid value using
             * broadcast bep period.
             */
            gprs_pl1_scell_meas_ptr->e = gprs_pl1_scell_meas_ptr->broadcast_e;

            /* Printout an F3 debug message */
            MSG_GERAN_MED_3_G(" qual meas use broadcast f6.10 ff %u",
                    gprs_pl1_scell_meas_ptr->e,
                    0,
                    0 );
        }
        else
        {
            gprs_pl1_scell_meas_ptr->e           = GPRS_PL1_FORGET_FACTOR_INVALID;
            gprs_pl1_scell_meas_ptr->broadcast_e = GPRS_PL1_FORGET_FACTOR_INVALID;

            MSG_GERAN_MED_0_G(" qual meas reset pacch and broadcast e to invalid ");
        }
    }

    /* Fill in the filtered quality params part of the message
     * metrics log packet.
     * This is done per timeslot.
     */
    usf_dl_tfi_info_ptr->mean_bep_tn = 0xffff;
    usf_dl_tfi_info_ptr->cv_bep_tn   = 0xffff;
    usf_dl_tfi_info_ptr->r           = 0xffff;

    if(  ( FALSE == decode_error )
       &&( FALSE != tfi_match ) )
    {
        filter_this_block = TRUE;
    }

    /* Should this data block be filtered ? */
    if( FALSE != filter_this_block )
    {
        if( ( GPRS_PL1_FORGET_FACTOR_INVALID != gprs_pl1_scell_meas_ptr->e )
          &&( gprs_pl1_scell_meas_ptr->e <= BEP_PERIOD2_FIELD_VALUE_0 )
          &&( gprs_pl1_scell_meas_ptr->e >= BEP_PERIOD2_FIELD_VALUE_14 )
          )
        {
            /* Local variables and pointers */
            gpl1_egprs_qual_meas_T *ts_qual_meas_ptr =
                &( gprs_pl1_scell_meas_ptr->qual_meas[ modulation ][ timeslot ] );

            /* Used to work out the average mean bep */
            uint32  mean_bep_per_blk  = 0;  /* F6.10 format */
            uint32  cv_bep_per_blk    = 0;  /* F6.10 format */

            uint16  e_div_r           = 0;  /* F6.10 format */
            uint16  one_minus_e_div_r;      /* F6.10 format */

            /* Set filter paramter r, maintain F6.10 format by shifting
             * multiplication result which is in F12.20 format.
             * i.e. divide by 1024 ---> >> 10.
             */
            ts_qual_meas_ptr->r = (uint16)( ( ( ((uint16)( ONE_F6_10 - gprs_pl1_scell_meas_ptr->e ) )
                                             * ts_qual_meas_ptr->r ) >> F6_10_SHIFT_VALUE )
                                           + gprs_pl1_scell_meas_ptr->e );

            /* Update the e/r accordingly, maintain F6.10 format by shifting
             * e i.e multiply by 1024 ---> << 10.
             */
            if( 0 != ts_qual_meas_ptr->r )
            {
              e_div_r           = (uint16) ( ( gprs_pl1_scell_meas_ptr->e << F6_10_SHIFT_VALUE )
                                            / ts_qual_meas_ptr->r );
            }
            else
            {
              MSG_GERAN_ERROR_3_G(" div by zero r %u e %u FN %u ",
                        ts_qual_meas_ptr->r,
                        gprs_pl1_scell_meas_ptr->e,
                        GSTMR_GET_FN_GERAN( gas_id )
                       );
            }

            one_minus_e_div_r = (uint16) ( ONE_F6_10 - e_div_r );

            /* Obtain the mean/cv bep for this block in F0.16 format */
            gl1_msg_process_bep( &( egprs_decode_metrics_ptr->bep[ 0 ] ),
                                 &( mean_bep_per_blk ),
                                 &( cv_bep_per_blk ) );

            /* Reduce resolution of cv bep from F0.16 -> F6.10
             * before feeding into filter.
             */
            cv_bep_per_blk >>= 6;

            /* Update the serving cell measurement structure
             * We are working F6.10 format and hence shift the multiplication
             * results which are in F12.20 format.
             * i.e. divide by 1024 ---> >> 10.
             */

            /* The mean_bep_tn, stored in the scell structure, is in 1/65535 units
             * Not F6.10.
             */
            ts_qual_meas_ptr->mean_bep_tn = (uint16)
                ( ( ( one_minus_e_div_r * ts_qual_meas_ptr->mean_bep_tn )
                   +( e_div_r * mean_bep_per_blk ) ) >> F6_10_SHIFT_VALUE );

            /* The cv_bep_tn, stored in the scell structure, is in F6.10 format */
            ts_qual_meas_ptr->cv_bep_tn = (uint16)
                ( ( ( one_minus_e_div_r * ts_qual_meas_ptr->cv_bep_tn )
                   +( e_div_r * ((uint16) cv_bep_per_blk ) ) ) >> F6_10_SHIFT_VALUE );

            /* Increment the iteration index for that timeslot */
            ts_qual_meas_ptr->num_blocks++;

            /* Fill in the filtered quality params part of the message
             * metrics log packet.
             * This is done per timeslot.
             */
            usf_dl_tfi_info_ptr->mean_bep_tn = ts_qual_meas_ptr->mean_bep_tn;
            usf_dl_tfi_info_ptr->cv_bep_tn   = ts_qual_meas_ptr->cv_bep_tn;
            usf_dl_tfi_info_ptr->r           = ts_qual_meas_ptr->r;

            if (modulation==GL1_DEFS_MODULATION_GMSK)
            { /* Accumulate filtered gmsk blocks */
              uint32  r_gmsk = (uint32) ts_qual_meas_ptr->r;
              egprs_rxqual_calc_ptr->acc_gmsk_r += r_gmsk;
              /* Warning: Accumulated values are in F6.10 format */
              egprs_rxqual_calc_ptr->acc_mean_gmsk_bep_tn +=
                  ( r_gmsk * ( (uint32) ts_qual_meas_ptr->mean_bep_tn ) );
              /* Increment the per-modulation scheme counter */
              egprs_rxqual_calc_ptr->num_gmsk_blocks++;
            }
            else if (modulation==GL1_DEFS_MODULATION_8PSK)
            {
              uint32  r_8psk = (uint32) ts_qual_meas_ptr->r;
              egprs_rxqual_calc_ptr->acc_8psk_r += r_8psk;
              /* Warning: Accumulated values are in F6.10 format */
              egprs_rxqual_calc_ptr->acc_mean_8psk_bep_tn +=
                  ( r_8psk * ( (uint32) ts_qual_meas_ptr->mean_bep_tn ) );
              /* Increment the per-modulation scheme counter */
              egprs_rxqual_calc_ptr->num_8psk_blocks++;
            }


        }     /* End of if forgetting factor is valid */
        else
        {
            /* BCCH/PBCCH bep period field value is reserved
             * and no valid bep period2 was supplied. This
             * means the bep filtering algorithm cannot run
             * Flag an error.
             */
            MSG_GERAN_ERROR_3_G(" forget factor invalid for meas mode %u ",
                       gprs_pl1_scell_meas_ptr->link_qual_meas_mode,
                       0,
                       0 );
        } /* End of if the forgetting is invalid */
    }     /* End of if filter_this_block is TRUE */
    else if( GPRS_PL1_FORGET_FACTOR_INVALID != gprs_pl1_scell_meas_ptr->e )
    {
        gpl1_egprs_qual_meas_T *ts_qual_meas_ptr =
            &( gprs_pl1_scell_meas_ptr->qual_meas[ GPL1_EGPRS_CODING_2_MODULATION[ egprs_decode_metrics_ptr->coding_type ] ]
                                                 [ timeslot ] );

        /* Set filter paramter r, maintain F6.10 format by shifting
         * multiplication result which is in F12.20 format.
         * i.e. divide by 1024 ---> >> 10.
         */
        ts_qual_meas_ptr->r = (uint16)( ( ( ((uint16)( ONE_F6_10 - gprs_pl1_scell_meas_ptr->e ) )
                                         * ts_qual_meas_ptr->r ) >> F6_10_SHIFT_VALUE ) );

        /* Fill in the filtered quality params part of the message
         * metrics log packet.
         * This is done per timeslot.
         */
        usf_dl_tfi_info_ptr->r = ts_qual_meas_ptr->r;
    } /* End of else bad crc or good crc and tfi mismatch and forgetting factor is valid */

    /* Do the non-egprs specific C value measurements and buffer management */
    if( FALSE == decode_error )
    {
        if( timeslot == (gl1_defs_tn_type) l1_get_current_timeslot(gas_id) )
        {
          gl1_power_meas_type pwr_meas_type = PRX_MEAS;
          int16 ssblockn_divrx = GL1_INVALID_POWER_LEVEL;
#if defined(FEATURE_GSM_RX_DIVERSITY_IDLE) || defined(FEATURE_GSM_RX_DIVERSITY_DATA) 
          /* MSRD is enabled, use MSRD divrx power*/
          /*Idle RxD is enabled then to keep buffer consistent use PRX power*/
          if (gl1_get_MSRD_ctl_nv(gas_id)) 
          {
            gprs_pl1_scell_meas_ptr->ssblockn_divrx = 
              (rssi_buffer_ptr->accum_block_rssi_divrx) >> DIVIDE_BY_4_SHIFT_VALUE;
            pwr_meas_type = PRX_DIVRX_MEAS;
          }
          else if (gl1_get_IMRD_ctl_nv(gas_id)) 
          {
            gprs_pl1_scell_meas_ptr->ssblockn_divrx = 
              (rssi_buffer_ptr->accum_block_rssi) >> DIVIDE_BY_4_SHIFT_VALUE;
            pwr_meas_type = PRX_DIVRX_MEAS;
          }
          ssblockn_divrx = gprs_pl1_scell_meas_ptr->ssblockn_divrx;
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE || FEATURE_GSM_RX_DIVERSITY_DATA*/

          /* Average the rssi only once */
            gprs_pl1_scell_meas_ptr->ssblockn =
                   (rssi_buffer_ptr->accum_block_rssi) >> DIVIDE_BY_4_SHIFT_VALUE;

            if(transfer_data_ptr->l1_test_mode != L1_TEST_MODE_OFF)
            {

             /* Add P0 if downlink Power control is enabled */
              if(transfer_data_ptr->dl_power_control.params_valid == TRUE)
              {
                gprs_pl1_scell_meas_ptr->ssblockn += (transfer_data_ptr->dl_power_control.po * 32);
              }

              MSG_GERAN_LOW_1_G("ssblckn %d",gprs_pl1_scell_meas_ptr->ssblockn);
              l1_running_avg_add_meas( &(gprs_serv_cell_meas_ptr->rla_p_buffer), 
                                       gprs_pl1_scell_meas_ptr->ssblockn, 
                                       ssblockn_divrx,
                                       pwr_meas_type,
                                       gas_id);
              gprs_serv_cell_meas_ptr->rla_p_count++;
            }


            if( PC_MEAS_CHAN_ON_PDCH == gprs_l1_idle_data->gprs_campedon_cell_data
                                                          .power_control.pc_meas_chan )
            {
              /* Run the C value processing */
              gpl1_egprs_process_c_value( gprs_pl1_scell_meas_ptr, gas_id );
            }
        }

    } /* End if decode error is FALSE */

    /* Reset the buffer rssi content */
    rssi_buffer_ptr->accum_block_rssi = 0;

#if defined(FEATURE_GSM_RX_DIVERSITY_IDLE) || defined(FEATURE_GSM_RX_DIVERSITY_DATA) 
    /* Reset the buffer rssi content */
    rssi_buffer_ptr->accum_block_rssi_divrx = 0;
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE || FEATURE_GSM_RX_DIVERSITY_DATA*/
}

/*===========================================================================

FUNCTION   gpl1_egprs_serv_cell_ota_data_cb( )

DESCRIPTION
  This function is basically the ota data cb but with EGPL1_OPTIMISE_MEAN_BEP
  switched ON.

DEPENDENCIES
  1- Used only during EGPRS.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void  gpl1_egprs_serv_cell_ota_data_cb( gl1_defs_tn_type                   timeslot,
                                               const gpl1_egprs_decode_metrics_T *egprs_decode_metrics_ptr,
                                               boolean                            tfi_match,
                                               usf_dl_tfi_info_t*                 usf_dl_tfi_info_ptr
                                               , gas_id_t gas_id )
{
    gprs_l1_idle_data_t *gprs_l1_idle_data = gl1_ms_switch_gprs_idle_data(gas_id);
    volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
    gpl1_gprs_serving_cell_data_t *gpl1_gprs_serving_cell_data_ptr = &gpl1_gprs_serving_cell_data[gas_id];
    /* Pointer to update the serving cell measurement
     * structure
     */
    gprs_pl1_serving_cell_meas_T *gprs_pl1_scell_meas_ptr = gprs_serv_cell_meas_ptr;

        /* Pointer to update the rx qual measurement */
    egprs_mean_bep_store_T  *egprs_rxqual_calc_ptr =
       &gprs_serv_cell_meas_ptr->egprs_rxqual_calc.egprs_mean_bep_store
        [gprs_serv_cell_meas_ptr->egprs_rxqual_calc.active_buffer];
    /* Setup a pointer to the current transfer data parameters */
    transfer_data_T*  transfer_data_ptr =
      l1_tskisr_blk->current_params.L1Data.pTransfer_data;

    /* local rssi pointer */
    block_rssi_t* rssi_buffer_ptr = gprs_pl1_scell_meas_ptr->rssi_buffer_ptr;

    /* Set the modulation type for this block.
     * work it out by mapping the coding scheme used.
     */
    const gl1_defs_modulation_type  modulation =
        GPL1_EGPRS_CODING_2_MODULATION[ egprs_decode_metrics_ptr->coding_type ];

    const boolean  decode_error      = egprs_decode_metrics_ptr->decode_error;
    boolean        filter_this_block = FALSE;

#if  EGPL1_OPTIMISE_MEAN_BEP

    /* Used to work out the average mean bep */
    uint32  mean_bep_per_blk  = 0;  /* F6.10 format */
    uint32  cv_bep_per_blk    = 0;  /* F6.10 format */

#endif

    /* Perform a final check on the forgetting factor e to
     * make sure it is valid for filtering.
     */
    if(  ( GPRS_PL1_FORGET_FACTOR_INVALID == gprs_pl1_scell_meas_ptr->e )
       ||( gprs_pl1_scell_meas_ptr->e > BEP_PERIOD2_FIELD_VALUE_0 )
       ||( gprs_pl1_scell_meas_ptr->e < BEP_PERIOD2_FIELD_VALUE_14 )
      )
    {
        if( ( GPRS_PL1_FORGET_FACTOR_INVALID != gprs_pl1_scell_meas_ptr->broadcast_e )
          &&( gprs_pl1_scell_meas_ptr->broadcast_e <= BEP_PERIOD2_FIELD_VALUE_0 )
          &&( gprs_pl1_scell_meas_ptr->broadcast_e >= BEP_PERIOD2_FIELD_VALUE_10 )
          )
        {
            /* Set the forgetting factor in use to a valid value using
             * broadcast bep period.
             */
            gprs_pl1_scell_meas_ptr->e = gprs_pl1_scell_meas_ptr->broadcast_e;

            /* Printout an F3 debug message */
            MSG_GERAN_MED_3_G(" qual meas use broadcast f6.10 ff %u",
                    gprs_pl1_scell_meas_ptr->e,
                    0,
                    0 );
        }
        else
        {
            gprs_pl1_scell_meas_ptr->e           = GPRS_PL1_FORGET_FACTOR_INVALID;
            gprs_pl1_scell_meas_ptr->broadcast_e = GPRS_PL1_FORGET_FACTOR_INVALID;

            MSG_GERAN_MED_0_G(" qual meas reset pacch and broadcast e to invalid ");
        }
    }

    /* Fill in the filtered quality params part of the message
     * metrics log packet.
     * This is done per timeslot.
     */
    usf_dl_tfi_info_ptr->mean_bep_tn = 0xffff;
    usf_dl_tfi_info_ptr->cv_bep_tn   = 0xffff;
    usf_dl_tfi_info_ptr->r           = 0xffff;

#if  EGPL1_OPTIMISE_MEAN_BEP

    /* Make sure this block is not a control block before
     * including it in the moving average filter.
     * @@TODO Is it better to check payload type ?
     */
    /* CR: 235571 - 3GPP: Radio blocks for channel quality reporting
     *  As part of this we should consider CS 1 block for which DL TFI matches.
     */
    {
        /* Loop counter */
        uint32   mov_avg_element = EGPL1_MOVING_AVERAGE_SIZE;

        /* Used to accumulate the current and previous
         * mean/cv bep values.
         */
        uint32  accum_mean_bep = 0;
        uint32  accum_cv_bep   = 0;

        uint32  num_mean_bep   = 0;

        /* Moving average pointer */
        const moving_avg_window_element_T*  mov_avg_ptr;

        /* Set a pointer to the moving average structure
         * based on the modulation scheme and timeslot.
         */
        ts_mean_bep_moving_avg_T*  ts_mean_bep_mov_avg_ptr =
            &( EGPL1_MEAN_BEP_MOVING_AVERAGE[ gas_id ][ modulation ][ timeslot ] );

        /* Check the decode error and tfi match flags
         * Make sure the pointer for that timeslot is
         * not NULL.
         */
        if( NULL == (ts_mean_bep_mov_avg_ptr->mov_avg_wndw_element_ptr) )
        {
            /* Point to the first element */
            ts_mean_bep_mov_avg_ptr->mov_avg_wndw_element_ptr =
                &( ts_mean_bep_mov_avg_ptr->moving_avg_window[ 0 ] );
        }

        /* Check if the crc check failed for this block
         * or the tfi did not match that of the downlink
         * assignment.
         */
        if(  ( FALSE != decode_error )
           ||( FALSE == tfi_match ) )
        {
            /* This block is invalid hence mark
             * mean bep for it as such.
             * This makes sure it is included in the
             * moving average.
             */
            ts_mean_bep_mov_avg_ptr->mov_avg_wndw_element_ptr->mean_bep = INVALID_MEAN_BEP;
            ts_mean_bep_mov_avg_ptr->mov_avg_wndw_element_ptr->cv_bep   = INVALID_CV_BEP;
        }
        else
        {
            /* This block passed the crc check and
             * is meant for this MS.
             * Work out the mean bep.
             */

            /* Obtain the mean/cv bep for this block in F0.16 format */
            gl1_msg_process_bep( &( egprs_decode_metrics_ptr->bep[ 0 ] ),                          /* IN  */
                                 &( ts_mean_bep_mov_avg_ptr->mov_avg_wndw_element_ptr->mean_bep ), /* OUT */
                                 &( ts_mean_bep_mov_avg_ptr->mov_avg_wndw_element_ptr->cv_bep )    /* OUT */
                               );
        }

        /* Increment the moving avg pointer for this timeslot
         * \ modulation scheme.
         * Protect against the wrap around condition.
         * This pointer is not used the moving avg calculation,
         * it is just for averaging purposes.
         */
        if( (ts_mean_bep_mov_avg_ptr->mov_avg_wndw_element_ptr)
                == &( ts_mean_bep_mov_avg_ptr->moving_avg_window[ (EGPL1_MOVING_AVERAGE_SIZE-1) ] ) )
        {
            /* Point to the first element */
            ts_mean_bep_mov_avg_ptr->mov_avg_wndw_element_ptr =
                &( ts_mean_bep_mov_avg_ptr->moving_avg_window[ 0 ] );
        }
        else
        {
            /* Point to the next element */
            (ts_mean_bep_mov_avg_ptr->mov_avg_wndw_element_ptr)++;
        }

        /* Initialise the pointer which is used to only read values
         * of the last recorded EGPL1_MOVING_AVERAGE_SIZE mean beps.
         */
        mov_avg_ptr = &( ts_mean_bep_mov_avg_ptr->moving_avg_window[ (EGPL1_MOVING_AVERAGE_SIZE-1) ] );

        /* Search through the moving average window
         * and accumulate valid mean bep values.
         */
        while( mov_avg_element-- != 0 )
        {
            if( INVALID_MEAN_BEP != (mov_avg_ptr->mean_bep) )
            {
                /* Accumulate mean bep */
                accum_mean_bep += (mov_avg_ptr->mean_bep);

                /* Accumulate cv bep */
                accum_cv_bep += (mov_avg_ptr->cv_bep);

                /* Increment the count */
                num_mean_bep++;
            }

            /* Decrement the pointer */
            mov_avg_ptr--;
        }

        /* Check there is at least one valid value
         * accumulated.
         * Protect against a divide by zero.
         */
        if( 0 != num_mean_bep )
        {
            /* The module managed to obtain a valid mean bep
             * value.
             * Set the filtering flag to TRUE.
             */
            filter_this_block = TRUE;

            /* Obtain the moving average */
            switch( num_mean_bep )
            {
            case  4:
              mean_bep_per_blk = accum_mean_bep >> 2; /* Divide by 4 */
              cv_bep_per_blk   = accum_cv_bep   >> 2;
              break;

            case  3:
              mean_bep_per_blk = accum_mean_bep / 3;
              cv_bep_per_blk   = accum_cv_bep   / 3;
              break;

            case  2:
              mean_bep_per_blk = accum_mean_bep >> 1; /* Divide by 2 */
              cv_bep_per_blk   = accum_cv_bep   >> 1;
              break;

            case  1:
              /* Do not do anything here since we are
               * we are dividing by 1.
               * Just set the value.
               */
              mean_bep_per_blk = accum_mean_bep;
              cv_bep_per_blk   = accum_cv_bep;
              break;

            default:
              /* Although this effectively an error condition,
               * attempt to generate a valid value.
               */
              mean_bep_per_blk = (accum_mean_bep / num_mean_bep);
              cv_bep_per_blk   = (accum_cv_bep / num_mean_bep);
              break;
            } /* End of switch on num_mean_bep */
        }     /* End of if num_mean_bep != 0 */
    }         /* End of this block is not a CS1 */

#else /* EGPL1_OPTIMISE_MEAN_BEP */

    if(  ( FALSE == decode_error )
       &&( FALSE != tfi_match ) )
    {
        filter_this_block = TRUE;
    }
#endif

    /* Should this data block be filtered ? */
    if( FALSE != filter_this_block )
    {
        if( ( GPRS_PL1_FORGET_FACTOR_INVALID != gprs_pl1_scell_meas_ptr->e )
          &&( gprs_pl1_scell_meas_ptr->e <= BEP_PERIOD2_FIELD_VALUE_0 )
          &&( gprs_pl1_scell_meas_ptr->e >= BEP_PERIOD2_FIELD_VALUE_14 )
          )
        {
            /* Local variables and pointers */
            gpl1_egprs_qual_meas_T *ts_qual_meas_ptr =
                &( gprs_pl1_scell_meas_ptr->qual_meas[ modulation ][ timeslot ] );

#if !( EGPL1_OPTIMISE_MEAN_BEP )

            /* Used to work out the average mean bep */
            uint32  mean_bep_per_blk  = 0;  /* F6.10 format */
            uint32  cv_bep_per_blk    = 0;  /* F6.10 format */

#endif
            uint16  e_div_r           = 0;  /* F6.10 format */
            uint16  one_minus_e_div_r;      /* F6.10 format */

            /* Set filter paramter r, maintain F6.10 format by shifting
             * multiplication result which is in F12.20 format.
             * i.e. divide by 1024 ---> >> 10.
             */
            ts_qual_meas_ptr->r = (uint16)( ( ( ((uint16)( ONE_F6_10 - gprs_pl1_scell_meas_ptr->e ) )
                                             * ts_qual_meas_ptr->r ) >> F6_10_SHIFT_VALUE )
                                           + gprs_pl1_scell_meas_ptr->e );

            /* Update the e/r accordingly, maintain F6.10 format by shifting
             * e i.e multiply by 1024 ---> << 10.
             */
            if( 0 != ts_qual_meas_ptr->r )
            {
              e_div_r           = (uint16) ( ( gprs_pl1_scell_meas_ptr->e << F6_10_SHIFT_VALUE )
                                            / ts_qual_meas_ptr->r );
            }
            else
            {
              MSG_GERAN_ERROR_3_G(" div by zero r %u e %u FN %u ",
                        ts_qual_meas_ptr->r,
                        gprs_pl1_scell_meas_ptr->e,
                        GSTMR_GET_FN_GERAN( gas_id )
                       );
            }

            one_minus_e_div_r = (uint16) ( ONE_F6_10 - e_div_r );

#if !( EGPL1_OPTIMISE_MEAN_BEP )

            /* Obtain the mean/cv bep for this block in F0.16 format */
            gl1_msg_process_bep( &( egprs_decode_metrics_ptr->bep[ 0 ] ),
                                 &( mean_bep_per_blk ),
                                 &( cv_bep_per_blk )
                               );

#endif
            /* Reduce resolution of cv bep from F0.16 -> F6.10
             * before feeding into filter.
             */
            cv_bep_per_blk >>= 6;

            /* Update the serving cell measurement structure
             * We are working F6.10 format and hence shift the multiplication
             * results which are in F12.20 format.
             * i.e. divide by 1024 ---> >> 10.
             */

            /* The mean_bep_tn, stored in the scell structure, is in 1/65535 units
             * Not F6.10.
             */
            ts_qual_meas_ptr->mean_bep_tn = (uint16)
                ( ( ( one_minus_e_div_r * ts_qual_meas_ptr->mean_bep_tn )
                   +( e_div_r * mean_bep_per_blk ) ) >> F6_10_SHIFT_VALUE );

            /* The cv_bep_tn, stored in the scell structure, is in F6.10 format */
            ts_qual_meas_ptr->cv_bep_tn = (uint16)
                ( ( ( one_minus_e_div_r * ts_qual_meas_ptr->cv_bep_tn )
                   +( e_div_r * ((uint16) cv_bep_per_blk ) ) ) >> F6_10_SHIFT_VALUE );

            /* Increment the iteration index for that timeslot */
            ts_qual_meas_ptr->num_blocks++;

            /* Fill in the filtered quality params part of the message
             * metrics log packet.
             * This is done per timeslot.
             */
            usf_dl_tfi_info_ptr->mean_bep_tn = ts_qual_meas_ptr->mean_bep_tn;
            usf_dl_tfi_info_ptr->cv_bep_tn   = ts_qual_meas_ptr->cv_bep_tn;
            usf_dl_tfi_info_ptr->r           = ts_qual_meas_ptr->r;

            if (modulation==GL1_DEFS_MODULATION_GMSK)
            { /* Accumulate filtered gmsk blocks */
              uint32  r_gmsk = (uint32) ts_qual_meas_ptr->r;
              egprs_rxqual_calc_ptr->acc_gmsk_r += r_gmsk;
              /* Warning: Accumulated values are in F6.10 format */
              egprs_rxqual_calc_ptr->acc_mean_gmsk_bep_tn +=
                  ( r_gmsk * ( (uint32) ts_qual_meas_ptr->mean_bep_tn ) );
              /* Increment the per-modulation scheme counter */
              egprs_rxqual_calc_ptr->num_gmsk_blocks++;
            }
            else if (modulation==GL1_DEFS_MODULATION_8PSK)
            {
              uint32  r_8psk = (uint32) ts_qual_meas_ptr->r;
              egprs_rxqual_calc_ptr->acc_8psk_r += r_8psk;
              /* Warning: Accumulated values are in F6.10 format */
              egprs_rxqual_calc_ptr->acc_mean_8psk_bep_tn +=
                  ( r_8psk * ( (uint32) ts_qual_meas_ptr->mean_bep_tn ) );
              /* Increment the per-modulation scheme counter */
              egprs_rxqual_calc_ptr->num_8psk_blocks++;
            }
        }     /* End of if forgetting factor is valid */
        else
        {
            /* BCCH/PBCCH bep period field value is reserved
             * and no valid bep period2 was supplied. This
             * means the bep filtering algorithm cannot run
             * Flag an error.
             */
            MSG_GERAN_ERROR_3_G(" forget factor invalid for meas mode %u ",
                       gprs_pl1_scell_meas_ptr->link_qual_meas_mode,
                       0,
                       0 );
        } /* End of if the forgetting is invalid */
    }     /* End of if filter_this_block is TRUE */
    else if( GPRS_PL1_FORGET_FACTOR_INVALID != gprs_pl1_scell_meas_ptr->e )
    {
        gpl1_egprs_qual_meas_T *ts_qual_meas_ptr =
            &( gprs_pl1_scell_meas_ptr->qual_meas[ GPL1_EGPRS_CODING_2_MODULATION[ egprs_decode_metrics_ptr->coding_type ] ]
                                                 [ timeslot ] );

        /* Set filter paramter r, maintain F6.10 format by shifting
         * multiplication result which is in F12.20 format.
         * i.e. divide by 1024 ---> >> 10.
         */
        ts_qual_meas_ptr->r = (uint16)( ( ( ((uint16)( ONE_F6_10 - gprs_pl1_scell_meas_ptr->e ) )
                                         * ts_qual_meas_ptr->r ) >> F6_10_SHIFT_VALUE ) );

        /* Fill in the filtered quality params part of the message
         * metrics log packet.
         * This is done per timeslot.
         */
        usf_dl_tfi_info_ptr->r = ts_qual_meas_ptr->r;
    } /* End of else bad crc or good crc and tfi mismatch and forgetting factor is valid */

    /* Do the non-egprs specific C value measurements and buffer management */
    if( FALSE == decode_error )
    {
        if( timeslot == (gl1_defs_tn_type) l1_get_current_timeslot(gas_id) )
        {
          gl1_power_meas_type pwr_meas_type = PRX_MEAS;
          int16 ssblockn_divrx = GL1_INVALID_POWER_LEVEL;
#if defined(FEATURE_GSM_RX_DIVERSITY_IDLE) || defined(FEATURE_GSM_RX_DIVERSITY_DATA) 
          /* MSRD is enabled, use MSRD divrx power*/
          /*Idle RxD is enabled then to keep buffer consistent use PRX power*/
          if (gl1_get_MSRD_ctl_nv(gas_id)) 
          {
            pwr_meas_type = PRX_DIVRX_MEAS;
            gprs_pl1_scell_meas_ptr->ssblockn_divrx = 
              (rssi_buffer_ptr->accum_block_rssi_divrx) >> DIVIDE_BY_4_SHIFT_VALUE;
          }
          else if (gl1_get_IMRD_ctl_nv(gas_id)) 
          {
            gprs_pl1_scell_meas_ptr->ssblockn_divrx = 
              (rssi_buffer_ptr->accum_block_rssi) >> DIVIDE_BY_4_SHIFT_VALUE;
            pwr_meas_type = PRX_DIVRX_MEAS;
          }
          ssblockn_divrx = gprs_pl1_scell_meas_ptr->ssblockn_divrx;
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA*/
            /* Average the rssi only once */
            gprs_pl1_scell_meas_ptr->ssblockn =
                    (rssi_buffer_ptr->accum_block_rssi) >> DIVIDE_BY_4_SHIFT_VALUE;

            if(transfer_data_ptr->l1_test_mode != L1_TEST_MODE_OFF)
            {
              /* Add P0 if downlink Power control is enabled */
              if(transfer_data_ptr->dl_power_control.params_valid == TRUE)
              {
                gprs_pl1_scell_meas_ptr->ssblockn += (transfer_data_ptr->dl_power_control.po * 32);
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
                if (PRX_DIVRX_MEAS == pwr_meas_type)
                {
                  gprs_pl1_scell_meas_ptr->ssblockn_divrx += (transfer_data_ptr->dl_power_control.po * 32);
                  ssblockn_divrx = gprs_pl1_scell_meas_ptr->ssblockn_divrx;
                }
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA*/
              }

              MSG_GERAN_LOW_1_G("ssblckn %d",gprs_pl1_scell_meas_ptr->ssblockn);
              l1_running_avg_add_meas( &(gprs_serv_cell_meas_ptr->rla_p_buffer), 
                                       gprs_pl1_scell_meas_ptr->ssblockn, 
                                       ssblockn_divrx,
                                       pwr_meas_type,
                                       gas_id);
              gprs_serv_cell_meas_ptr->rla_p_count++;
            }

            if( PC_MEAS_CHAN_ON_PDCH == gprs_l1_idle_data->gprs_campedon_cell_data
                                                          .power_control.pc_meas_chan )
            {
                /* Run the C value processing */
                gpl1_egprs_process_c_value( gprs_pl1_scell_meas_ptr, gas_id );
            }
        }

#if  EGPL1_OPTIMISE_MEAN_BEP
        if( ( FALSE != tfi_match )
            && (GL1_DEFS_CS1_CODING != egprs_decode_metrics_ptr->coding_type )
            && ( GPRS_PL1_FORGET_FACTOR_INVALID != gprs_pl1_scell_meas_ptr->e ) )
        {
            /* Update the bler sliding window */
            gpl1_gprs_serving_cell_data_ptr->window_element_ptr->data_crc_fail =
                egprs_decode_metrics_ptr->first_payload_error;

            gpl1_gprs_serving_cell_data_ptr->window_element_ptr->coding_scheme = egprs_decode_metrics_ptr->coding_type;

            /* Move to next element, take care of the wrap around case. */
            if( &( EGPL1_BLER_SLIDING_WINDOW[gas_id][ (EGPL1_DATA_BLK_WINDOW_SIZE - 1) ] ) == gpl1_gprs_serving_cell_data_ptr->window_element_ptr )
            {
                gpl1_gprs_serving_cell_data_ptr->window_element_ptr = &( EGPL1_BLER_SLIDING_WINDOW[gas_id][ 0 ] );
            }
            else
            {
                ++gpl1_gprs_serving_cell_data_ptr->window_element_ptr;
            }

            if( FALSE != EGPL1_IS_MCS_DUAL_PAYLOAD[ egprs_decode_metrics_ptr->coding_type ] )
            {
                gpl1_gprs_serving_cell_data_ptr->window_element_ptr->data_crc_fail =
                    egprs_decode_metrics_ptr->second_payload_error;

                gpl1_gprs_serving_cell_data_ptr->window_element_ptr->coding_scheme = egprs_decode_metrics_ptr->coding_type;

                /* Move to next element, take care of the wrap around case. */
               if( &( EGPL1_BLER_SLIDING_WINDOW[gas_id][ (EGPL1_DATA_BLK_WINDOW_SIZE - 1) ] ) == gpl1_gprs_serving_cell_data_ptr->window_element_ptr )
               {
                   gpl1_gprs_serving_cell_data_ptr->window_element_ptr = &( EGPL1_BLER_SLIDING_WINDOW[gas_id][ 0 ] );
               }
               else
               {
                   ++gpl1_gprs_serving_cell_data_ptr->window_element_ptr;
               }
            } /* End of if this is a dual payload data block */
        }     /* End of if tfi match is TRUE && forget factor is valid */
#endif
    } /* End if decode error is FALSE */

    /* Reset the buffer rssi content */
    rssi_buffer_ptr->accum_block_rssi = 0;

#if defined(FEATURE_GSM_RX_DIVERSITY_IDLE) || defined(FEATURE_GSM_RX_DIVERSITY_DATA) 
    /* Reset the buffer rssi content */
    rssi_buffer_ptr->accum_block_rssi_divrx = 0;
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE || FEATURE_GSM_RX_DIVERSITY_DATA*/
}

/*===========================================================================

FUNCTION   gpl1_egprs_serv_cell_qual_meas_report_gcf( )

DESCRIPTION
  This function is basically the ota meas report but with EGPL1_OPTIMISE_MEAN_BEP
  switched OFF.

DEPENDENCIES
  1- Used only during EGPRS.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  gpl1_egprs_serv_cell_qual_meas_report_gcf( egprs_chn_qual_rpt_t *qual_report_msg_ptr,
                                                 uint8                 dl_timeslot_alloc,
                                                 gas_id_t gas_id )
{
    /* Pointers to read the serving cell measurement
     * structure
     */
    gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
    gprs_pl1_serving_cell_meas_T *gprs_pl1_scell_meas_ptr = gprs_serv_cell_meas_ptr;

    uint32  timeslot; /* Loop variable */

    uint32  interf_meas_ts_bitmap = 0xFFFFFFFF;

    /* GPRS only specific reporting
     * C-VALUE
     * Convert C-value into rxlev 6-bit values
     */
    dBx16_T  reported_rxlev =
        L1_map_dBm_x16_to_rxlev( (dBx16_T) gprs_pl1_scell_meas_ptr->cn );

    gprs_pl1_scell_meas_ptr->reported_rxlev = reported_rxlev;
    qual_report_msg_ptr->c_value            = (uint8) reported_rxlev;

    /* Initialise the appropriate flags in the message */
    qual_report_msg_ptr->bep_meas_rpt_flg = not_valid;
    qual_report_msg_ptr->int_meas_rpt_flg = not_valid;

    if( GPRS_PL1_FORGET_FACTOR_INVALID != gprs_pl1_scell_meas_ptr->e )
    {
        /* Report the per-modulation scheme Mean Bep / CV Bep
         * This is independent of the value of the link quality
         * measurement mode and is a replacement for sign var / rxqual
         */

        /* Pointers to read the serving cell measurement
         * structure
         */
        gpl1_egprs_qual_meas_T  *ts_gmsk_qual_meas_ptr =
            &( gprs_pl1_scell_meas_ptr->qual_meas[ GL1_DEFS_MODULATION_GMSK ]
                                                 [ GL1_DEFS_SLOTS_IN_FRAME - 1 ] );

        gpl1_egprs_qual_meas_T  *ts_8psk_qual_meas_ptr =
            &( gprs_pl1_scell_meas_ptr->qual_meas[ GL1_DEFS_MODULATION_8PSK ]
                                                 [ GL1_DEFS_SLOTS_IN_FRAME - 1 ] );

        /* Local variables.
         * 2 instances: one for gmsk and other for 8psk.
         */
        uint32  acc_mean_gmsk_bep_tn = 0;
        uint32  acc_mean_8psk_bep_tn = 0;

        uint32  acc_cv_gmsk_bep_tn   = 0;
        uint32  acc_cv_8psk_bep_tn   = 0;

        uint32  acc_gmsk_r        = 0; /* F6.10 format */
        uint32  acc_8psk_r        = 0; /* f6.10 format */

        /* Counters to help in choosing which modulation scheme
         * report to the network
         */
        uint32  num_gmsk_blocks = 0;
        uint32  num_8psk_blocks = 0;

        timeslot = GL1_DEFS_SLOTS_IN_FRAME;

        /* Calculate the mean/cv bep */
        while( timeslot-- != 0 )
        {
            if( 0 != (uint8) ( ( dl_timeslot_alloc >> timeslot ) & 0x01 ) )
            {
                if( 0 != ts_gmsk_qual_meas_ptr->num_blocks )
                {
                    /* Accumulate filtered gmsk blocks */
                    uint32  r_gmsk = (uint32) ts_gmsk_qual_meas_ptr->r;
                    acc_gmsk_r += r_gmsk;

                    /* Warning: Accumulated values are in F6.10 format */
                    acc_mean_gmsk_bep_tn += ( r_gmsk * ( (uint32) ts_gmsk_qual_meas_ptr->mean_bep_tn ) );

                    /* Warning: Accumulated values are in F12.20 format */
                    acc_cv_gmsk_bep_tn   += ( r_gmsk * ( (uint32) ts_gmsk_qual_meas_ptr->cv_bep_tn ) );

                    /* Increment the per-modulation scheme counter */
                    num_gmsk_blocks += (uint32) ts_gmsk_qual_meas_ptr->num_blocks;
                }

                if( 0 != ts_8psk_qual_meas_ptr->num_blocks )
                {
                    /* Accumulate filtered 8psk blocks */
                    uint32  r_8psk = (uint32) ts_8psk_qual_meas_ptr->r;
                    acc_8psk_r += r_8psk;

                    /* Warning: Accumulated values are in F6.10 format */
                    acc_mean_8psk_bep_tn += ( r_8psk * ( (uint32) ts_8psk_qual_meas_ptr->mean_bep_tn ) );

                    /* Warning: Accumulated values are in F12.20 format */
                    acc_cv_8psk_bep_tn   += ( r_8psk * ( (uint32) ts_8psk_qual_meas_ptr->cv_bep_tn ) );

                    /* Increment the per-modulation scheme counter */
                    num_8psk_blocks += (uint32) ts_8psk_qual_meas_ptr->num_blocks;
                }
            }
            ts_gmsk_qual_meas_ptr--;
            ts_8psk_qual_meas_ptr--;
        }
        /* Reset ts_qual_meas_ptr so its does not point outside of array
         * and set it up to point to TS7 meas info for use in loop.
         */
        ts_gmsk_qual_meas_ptr =
            &( gprs_pl1_scell_meas_ptr->qual_meas[ GL1_DEFS_MODULATION_GMSK ]
                                                 [ GL1_DEFS_SLOTS_IN_FRAME - 1 ] );

        ts_8psk_qual_meas_ptr =
            &( gprs_pl1_scell_meas_ptr->qual_meas[ GL1_DEFS_MODULATION_8PSK ]
                                                 [ GL1_DEFS_SLOTS_IN_FRAME - 1 ] );

        /* Reset the flags used to declare the presence of the
         * per-modulation scheme Mean Bep and CV Bep.
         */
        qual_report_msg_ptr->gmsk_mean_bep_present = FALSE;
        qual_report_msg_ptr->psk_mean_bep_present  = FALSE;

        /* Division results in F6.10 format
         * Only divide if accumulated r > 0.
         */
        if( 0 != acc_gmsk_r )
        {
            /* Fill in the gmsk mean/cv bep for the
             * received blocks since last measurement
             * report.
             */
            if( 0 != num_gmsk_blocks )
            {
                const uint16  mean_gmsk_bep =
                    (uint16) ( (acc_mean_gmsk_bep_tn) / acc_gmsk_r ); /* 1/65535 units */

                const uint16  cv_gmsk_bep =
                    (uint16) ( (acc_cv_gmsk_bep_tn) / acc_gmsk_r );   /* F6.10 format */

                /* Convert the mean gmsk bep inorder to pass it to the 32-bit quantization
                 * table.
                 * Multiply by (2^32)-1 then divide by ((2^16) - 1 ).
                 * This is the same as multiplying by 65537, optimise to not use multiplication.
                 */
                const uint32  high_res_mean_gmsk_bep = (mean_gmsk_bep << 16) + mean_gmsk_bep;

#if  DEBUG_GPL1_EGPRS_BEP
                MSG_GERAN_MED_3_G(" acc gmsk bep mean %u acc r %u ",
                        acc_mean_gmsk_bep_tn,
                        acc_gmsk_r,
                        0 );
#endif
                /* BEP reporting */
                qual_report_msg_ptr->gmsk_m_bep =
                    (uint8) gpl1_egprs_meas_scale( GPRS_PL1_GMSK_MEAN_BEP_LOG_SCALE,
                                                   high_res_mean_gmsk_bep );

                /* Fill in the cv bep into the report */
                qual_report_msg_ptr->gmsk_cv_bep =
                    (uint8) gpl1_egprs_meas_scale( GPRS_PL1_CV_BEP_SCALE,
                                                   (uint32) cv_gmsk_bep );

                qual_report_msg_ptr->gmsk_mean_bep_present = TRUE;

#if  DEBUG_GPL1_EGPRS_BEP
                MSG_GERAN_MED_3_G(" gmsk bep mean %u cv %u quantised bep %u ",
                        mean_gmsk_bep,
                        cv_gmsk_bep,
                        qual_report_msg_ptr->gmsk_m_bep );
#endif
            }     /* End of if num gmsk blocks is not equal to zero */
        }         /* End of if gmsk reliability factor is not equal to zero */

        if( 0 != acc_8psk_r )
        {
            /* Fill in the 8psk mean/cv bep for the
             * received blocks since last measurement
             * report.
             */
            if( 0 != num_8psk_blocks )
            {
                const uint16  mean_8psk_bep =
                    (uint16) ( (acc_mean_8psk_bep_tn) / acc_8psk_r ); /* 1/65535 units */

                const uint16  cv_8psk_bep =
                    (uint16) ( (acc_cv_8psk_bep_tn) / acc_8psk_r );    /* F6.10 format */

                /* Convert the mean gmsk bep inorder to pass it to the 32-bit quantization
                 * table.
                 * Multiply by (2^32)-1 then divide by ((2^16) - 1 ).
                 * This is the same as multiplying by 65537, optimise to not use multiplication.
                 */
                const uint32  high_res_mean_8psk_bep = (mean_8psk_bep << 16) + mean_8psk_bep;

#if  DEBUG_GPL1_EGPRS_BEP
                MSG_GERAN_MED_3_G(" acc 8psk bep mean %u acc r %u ",
                        acc_mean_8psk_bep_tn,
                        acc_8psk_r,
                        0 );
#endif
                /* BEP reporting */
                qual_report_msg_ptr->psk_m_bep =
                    (uint8) gpl1_egprs_meas_scale( GPRS_PL1_8PSK_MEAN_BEP_LOG_SCALE,
                                                   high_res_mean_8psk_bep );

                /* Fill in the cv bep into the report */
                qual_report_msg_ptr->psk_cv_bep =
                    (uint8) gpl1_egprs_meas_scale( GPRS_PL1_CV_BEP_SCALE,
                                                         (uint32) cv_8psk_bep );
                qual_report_msg_ptr->psk_mean_bep_present = TRUE;

#if  DEBUG_GPL1_EGPRS_BEP
                MSG_GERAN_MED_3_G(" 8psk bep mean %u cv %u quantised bep %u",
                        mean_8psk_bep,
                        cv_8psk_bep,
                        qual_report_msg_ptr->psk_m_bep );
#endif
            }     /* End of if num 8psk blocks is not equal to zero */
        }         /* End of if 8psk reliability factor is not equal to zero */

        switch( gprs_pl1_scell_meas_ptr->link_qual_meas_mode )
        {
            case BOTH_MEASUREMENTS:
            case BEP_MEASUREMENTS:
            {
                if( ( 0 != num_gmsk_blocks )
                  ||( 0 != num_8psk_blocks ) )
                {
                    /* Used for filling the individual ts measurements in the msg */
                    uint8  *tn_bitmap_ptr = &( qual_report_msg_ptr->tn_bitmap );

                    /* Report the per-timeslot Bep.
                     * This is dependent of the value of the link quality
                     * measurement mode.
                     */
                    uint8  *mean_bep_ptr = &( qual_report_msg_ptr->mean_bep[(GL1_DEFS_SLOTS_IN_FRAME - 1)] );

                    /* Should report whatever the network asks for( i.e. dl assignment ).
                     * The shift to the right is for reporting 4 MSBs of the 5.
                     */
                    qual_report_msg_ptr->bep_meas_rpt_flg = valid;

                    /* Initialise to 0 for the benefit of any per-timeslot gmsk
                     * reporting.
                     */
                    qual_report_msg_ptr->gmsk_or_8psk_bitmap = 0;

                    timeslot = GL1_DEFS_SLOTS_IN_FRAME;
                    while( timeslot-- != 0 )
                    {
                        if( 0 != (uint8) ( ( dl_timeslot_alloc >> timeslot ) & 0x01 ) )
                        {
                            if( ( 0 != ts_gmsk_qual_meas_ptr->num_blocks )
                              &&( (ts_gmsk_qual_meas_ptr->num_blocks) >= (ts_8psk_qual_meas_ptr->num_blocks) ) )
                            {
                                /* Convert the mean gmsk bep inorder to pass it to the 32-bit quantization
                                 * table.
                                 * Multiply by (2^32)-1 then divide by ((2^16) - 1 ).
                                 * This is the same as multiplying by 65537, optimise to not use multiplication.
                                 */
                                const uint32  high_res_gmsk_bep =
                                    (ts_gmsk_qual_meas_ptr->mean_bep_tn << 16) + ts_gmsk_qual_meas_ptr->mean_bep_tn;

                                (*tn_bitmap_ptr) |= (uint8) (0x0100 >> (8-timeslot) );

                                /* Mark the bep measurement type for this timeslot
                                 * as a gmsk one by not filling anything in the
                                 * gmsk_or_8psk_bitmap.
                                 */

                                /* Shift by 1 inorder to report 4 most significant bits. ETSI 04.60 */
                                *mean_bep_ptr = (uint8) ( gpl1_egprs_meas_scale( GPRS_PL1_GMSK_MEAN_BEP_LOG_SCALE,
                                                                                 high_res_gmsk_bep ) >> 1 );
                            }
                            else if( 0 != ts_8psk_qual_meas_ptr->num_blocks )
                            {
                                /* Convert the mean gmsk bep inorder to pass it to the 32-bit quantization
                                 * table.
                                 * Multiply by (2^32)-1 then divide by ((2^16) - 1 ).
                                 * This is the same as multiplying by 65537, optimise to not use multiplication.
                                 */
                                const uint32  high_res_8psk_bep =
                                    (ts_8psk_qual_meas_ptr->mean_bep_tn << 16) + ts_8psk_qual_meas_ptr->mean_bep_tn;

                                (*tn_bitmap_ptr) |= (uint8) (0x0100 >> (8-timeslot) );

                                /* Mark the bep measurement type for this timeslot
                                 * as a 8psk one
                                 */
                                (qual_report_msg_ptr->gmsk_or_8psk_bitmap) |=
                                    (uint8) (0x0100 >> (8-timeslot) );

                                /* Shift by 1 inorder to report 4 most significant bits. ETSI 04.60 */
                                *mean_bep_ptr = (uint8) ( gpl1_egprs_meas_scale( GPRS_PL1_8PSK_MEAN_BEP_LOG_SCALE,
                                                                                 high_res_8psk_bep ) >> 1 );
                            }
                        }

                        /* Reset the serving cell qual measurements used for this averaging
                         * period. ETSI 05.08 does not specifically ask for a reset in the
                         * EGPRS case but does for GPRS.
                         */

                        /* Warning: Be ware of multislot polling since the following
                         * reports generated in this frame for the rest of the dl slots
                         * will be meaningless.
                         */
                        ts_gmsk_qual_meas_ptr->num_blocks  = 0;
                        ts_gmsk_qual_meas_ptr--;

                        ts_8psk_qual_meas_ptr->num_blocks  = 0;
                        ts_8psk_qual_meas_ptr--;

                        /* decrement the msg ptr inorder to stay in step */
                        mean_bep_ptr--;
                    }  /* End of while loop */

                    /* Reset ts_qual_meas_ptr so its does not point outside of array */
                    ts_gmsk_qual_meas_ptr =
                        &( gprs_pl1_scell_meas_ptr->qual_meas[ GL1_DEFS_MODULATION_GMSK ]
                                                             [ GL1_DEFS_SLOTS_IN_FRAME - 1 ] );

                    ts_8psk_qual_meas_ptr =
                        &( gprs_pl1_scell_meas_ptr->qual_meas[ GL1_DEFS_MODULATION_8PSK ]
                                                             [ GL1_DEFS_SLOTS_IN_FRAME - 1 ] );
                } /* End of if either gmsk or 8psk blocks have been filtered */
                else
                {
                    qual_report_msg_ptr->tn_bitmap = 0x0;
                }
            }     /* End of case both || bep measurements */
            break;

            default:
            {
                timeslot = GL1_DEFS_SLOTS_IN_FRAME;
                while( timeslot-- != 0 )
                {
                    /* Reset the serving cell qual measurements used for this averaging
                     * period. ETSI 05.08 does not specifically ask for a reset in the
                     * EGPRS case but does for GPRS.
                     */

                    /* Warning: Be ware of multislot polling since the following
                     * reports generated in this frame for the rest of the dl slots
                     * will be meaningless.
                     */
                    ts_gmsk_qual_meas_ptr->num_blocks  = 0;
                    ts_gmsk_qual_meas_ptr--;

                    ts_8psk_qual_meas_ptr->num_blocks  = 0;
                    ts_8psk_qual_meas_ptr--;

                }  /* End of while loop */

                /* Reset ts_qual_meas_ptr so its does not point outside of array */
                ts_gmsk_qual_meas_ptr =
                    &( gprs_pl1_scell_meas_ptr->qual_meas[ GL1_DEFS_MODULATION_GMSK ]
                                                         [ GL1_DEFS_SLOTS_IN_FRAME - 1 ] );

                ts_8psk_qual_meas_ptr =
                    &( gprs_pl1_scell_meas_ptr->qual_meas[ GL1_DEFS_MODULATION_8PSK ]
                                                         [ GL1_DEFS_SLOTS_IN_FRAME - 1 ] );
            }
            break;
        } /* End of switch statement on link quality measurement mode */
    }     /* End of if forgetting factor is valid */

    switch( gprs_pl1_scell_meas_ptr->link_qual_meas_mode )
    {
    case BOTH_MEASUREMENTS:
    {
        /* Local variable used to filter out the reported
         * interference measurement timeslots in the
         * alternating fashion.
         */
        interf_meas_ts_bitmap = (uint32) gprs_pl1_scell_meas_ptr->interf_meas_ts_bitmap;

        /* Alternate the interference measurements timeslot
         * bitmap:
         * TIMESLOTS_0_1_2_3 ---> TIMESLOTS_4_5_6_7
         * TIMESLOTS_4_5_6_7 ---> TIMESLOTS_0_1_2_3
         */
        gprs_pl1_scell_meas_ptr->interf_meas_ts_bitmap = (uint8)
            ~(gprs_pl1_scell_meas_ptr->interf_meas_ts_bitmap);
    }

    case INTERF_MEASUREMENTS: /*lint !e616 : no break statement for avoiding repetition */
    {
        /* Setup the source, destination interference measurement pointers
         * used for filling the interference measurements
         */
        uint8*  i_level_bitmap_ptr = &( qual_report_msg_ptr->i_level_bitmap );
        uint8*  i_level_ptr        = &( qual_report_msg_ptr->i_level[(GL1_DEFS_SLOTS_IN_FRAME - 1)] );

        interference_T* tn_interf_ptr = &( gprs_pl1_scell_meas_ptr->int_array[(GL1_DEFS_SLOTS_IN_FRAME - 1)] );

        timeslot = GL1_DEFS_SLOTS_IN_FRAME;

        /* Fill in the interference measurements
         * into the report message.
         */
        while( timeslot-- != 0 )
        {
            /* Mark measurement as not present and over write as neccessary */
            /* *i_level_bitmap_ptr */

            /* Determine which channels have had at least one measurement on
             * Currently only add interference measurements when in transfer mode.
             */
            if( FALSE != tn_interf_ptr->valid_ind )
            {
                if( 0 != ( interf_meas_ts_bitmap & (0x0080 >> timeslot ) ) ) /* @@TODO */
                {
                    /*  Convert the measured C-value into the report value. */
                    dBx16_T int_dbm = L1_map_dBm_x16_to_rxlev
                              ( (dBx16_T) ( (tn_interf_ptr->meas_1) >> 8) );

                    /* Reported value is a 4 bit representation of the relative value
                     * of the intereference measurement and the C-value
                     */
                    int8 rxlev_diff = (int8) ( reported_rxlev - int_dbm );

                    if( rxlev_diff >= 0 )
                    {
                        /*  Divide by two and add 1 to give reported value  */
                        rxlev_diff >>= 1;
                        rxlev_diff += 1;

                        /* Range check, max. value 15  */
                        if( rxlev_diff > 15 )
                        {
                            rxlev_diff = 15;
                        }
                    }
                    else
                    {
                        /*  Interference is greater than C - value  */
                        rxlev_diff = 0;
                    }
                    *i_level_ptr = (uint8) rxlev_diff;

                    (*i_level_bitmap_ptr) |= (uint8) (0x0100 >> (8-timeslot) );

#if  DEBUG_GPL1_INTERF_MEAS
                    #error code not present
#endif

                } /* End of if statement */
#if  DEBUG_GPL1_INTERF_MEAS
                #error code not present
#endif
                /* Clear the measurement values and count to reset after reporting */
                /*
                ** CR: 229730 - UE is not reporting Link Quality Measurements as per spec
                ** LINK_QUALITY_MEASUREMENT_MODE
                ** MS shall substitute least-recently-reported and available interference
                ** measurements for time slots not already included in the report.
                **
                tn_interf_ptr->meas_1            = 0;
                tn_interf_ptr->valid_ind         = FALSE;
                */
                tn_interf_ptr->measurement_count = 0;
            } /* End of if valid meas ind for this timeslot */
            i_level_ptr--;
            tn_interf_ptr--;
        } /* End of while loop */

        /* Check the interference measurements timeslot bitmap */
        if( 0 != (*i_level_bitmap_ptr) )
        {
            /* Set the appropriate flags in the message */
            qual_report_msg_ptr->int_meas_rpt_flg = valid;
        }

    } /* End of case BOTH_MEASUREMENTS or INTERF_MEASUREMENTS */
        break;

    default:
        break;
    }  /* End of Switch statement on link quality measurement mode */
}

/*===========================================================================

FUNCTION   gpl1_egprs_serv_cell_qual_meas_report_ota( )

DESCRIPTION
  This function is basically the ota meas report but with EGPL1_OPTIMISE_MEAN_BEP
  switched ON.

DEPENDENCIES
  1- Used only during EGPRS.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void  gpl1_egprs_serv_cell_qual_meas_report_ota( egprs_chn_qual_rpt_t *qual_report_msg_ptr,
                                                        uint8                 dl_timeslot_alloc,
                                                        gas_id_t gas_id )
{
    /* Pointers to read the serving cell measurement
     * structure
     */
    gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
    gpl1_gprs_serving_cell_data_t *gpl1_gprs_serving_cell_data_ptr = &gpl1_gprs_serving_cell_data[gas_id];
    gprs_pl1_serving_cell_meas_T *gprs_pl1_scell_meas_ptr =gprs_serv_cell_meas_ptr;

    uint32  timeslot; /* Loop variable */

    uint32  interf_meas_ts_bitmap = 0xFFFFFFFF;

    /* GPRS only specific reporting
     * C-VALUE
     * Convert C-value into rxlev 6-bit values
     */
    dBx16_T  reported_rxlev =
        L1_map_dBm_x16_to_rxlev( (dBx16_T) gprs_pl1_scell_meas_ptr->cn );

    gprs_pl1_scell_meas_ptr->reported_rxlev = reported_rxlev;
    qual_report_msg_ptr->c_value            = (uint8) reported_rxlev;

    /* Initialise the appropriate flags in the message */
    qual_report_msg_ptr->bep_meas_rpt_flg = not_valid;
    qual_report_msg_ptr->int_meas_rpt_flg = not_valid;

    if( GPRS_PL1_FORGET_FACTOR_INVALID != gprs_pl1_scell_meas_ptr->e )
    {
        /* Report the per-modulation scheme Mean Bep / CV Bep
         * This is independent of the value of the link quality
         * measurement mode and is a replacement for sign var / rxqual
         */

        /* Pointers to read the serving cell measurement
         * structure
         */
        gpl1_egprs_qual_meas_T  *ts_gmsk_qual_meas_ptr =
            &( gprs_pl1_scell_meas_ptr->qual_meas[ GL1_DEFS_MODULATION_GMSK ]
                                                 [ GL1_DEFS_SLOTS_IN_FRAME - 1 ] );

        gpl1_egprs_qual_meas_T  *ts_8psk_qual_meas_ptr =
            &( gprs_pl1_scell_meas_ptr->qual_meas[ GL1_DEFS_MODULATION_8PSK ]
                                                 [ GL1_DEFS_SLOTS_IN_FRAME - 1 ] );

        /* Local variables.
         * 2 instances: one for gmsk and other for 8psk.
         */
        uint32  acc_mean_gmsk_bep_tn = 0;
        uint32  acc_mean_8psk_bep_tn = 0;

        uint32  acc_cv_gmsk_bep_tn   = 0;
        uint32  acc_cv_8psk_bep_tn   = 0;

        uint32  acc_gmsk_r        = 0; /* F6.10 format */
        uint32  acc_8psk_r        = 0; /* f6.10 format */

        /* Counters to help in choosing which modulation scheme
         * report to the network
         */
        uint32  num_gmsk_blocks = 0;
        uint32  num_8psk_blocks = 0;

#if  EGPL1_OPTIMISE_MEAN_BEP
        /* Bler biasing of results */
        boolean  bias_gmsk_mean_bep_report = FALSE;
        boolean  bias_8psk_mean_bep_report = FALSE;

        boolean  comp_with_prev_bep = FALSE;
        uint8    mean_bep_bias      = 0;
#endif
        timeslot = GL1_DEFS_SLOTS_IN_FRAME;

        /* Calculate the mean/cv bep */
        while( timeslot-- != 0 )
        {
            if( 0 != (uint8) ( ( dl_timeslot_alloc >> timeslot ) & 0x01 ) )
            {
                if( 0 != ts_gmsk_qual_meas_ptr->num_blocks )
                {
                    /* Accumulate filtered gmsk blocks */
                    uint32  r_gmsk = (uint32) ts_gmsk_qual_meas_ptr->r;
                    acc_gmsk_r += r_gmsk;

                    /* Warning: Accumulated values are in F6.10 format */
                    acc_mean_gmsk_bep_tn += ( r_gmsk * ( (uint32) ts_gmsk_qual_meas_ptr->mean_bep_tn ) );

                    /* Warning: Accumulated values are in F12.20 format */
                    acc_cv_gmsk_bep_tn   += ( r_gmsk * ( (uint32) ts_gmsk_qual_meas_ptr->cv_bep_tn ) );

                    /* Increment the per-modulation scheme counter */
                    num_gmsk_blocks += (uint32) ts_gmsk_qual_meas_ptr->num_blocks;
                }

                if( 0 != ts_8psk_qual_meas_ptr->num_blocks )
                {
                    /* Accumulate filtered 8psk blocks */
                    uint32  r_8psk = (uint32) ts_8psk_qual_meas_ptr->r;
                    acc_8psk_r += r_8psk;

                    /* Warning: Accumulated values are in F6.10 format */
                    acc_mean_8psk_bep_tn += ( r_8psk * ( (uint32) ts_8psk_qual_meas_ptr->mean_bep_tn ) );

                    /* Warning: Accumulated values are in F12.20 format */
                    acc_cv_8psk_bep_tn   += ( r_8psk * ( (uint32) ts_8psk_qual_meas_ptr->cv_bep_tn ) );

                    /* Increment the per-modulation scheme counter */
                    num_8psk_blocks += (uint32) ts_8psk_qual_meas_ptr->num_blocks;
                }
            }
            ts_gmsk_qual_meas_ptr--;
            ts_8psk_qual_meas_ptr--;
        }
        /* Reset ts_qual_meas_ptr so its does not point outside of array
         * and set it up to point to TS7 meas info for use in loop.
         */
        ts_gmsk_qual_meas_ptr =
            &( gprs_pl1_scell_meas_ptr->qual_meas[ GL1_DEFS_MODULATION_GMSK ]
                                                 [ GL1_DEFS_SLOTS_IN_FRAME - 1 ] );

        ts_8psk_qual_meas_ptr =
            &( gprs_pl1_scell_meas_ptr->qual_meas[ GL1_DEFS_MODULATION_8PSK ]
                                                 [ GL1_DEFS_SLOTS_IN_FRAME - 1 ] );

        /* Reset the flags used to declare the presence of the
         * per-modulation scheme Mean Bep and CV Bep.
         */
        qual_report_msg_ptr->gmsk_mean_bep_present = FALSE;
        qual_report_msg_ptr->psk_mean_bep_present  = FALSE;

#if  EGPL1_OPTIMISE_MEAN_BEP
        /* Check the mcs scheme with the highest number of received blocks
         * and calculate the BLER for it.
         */
        if( ( 0 != num_gmsk_blocks )
          ||( 0 != num_8psk_blocks ) )
        {
            bler_num_blocks_T  EGPL1_NUM_BLER_BLKS[ NUM_CODING_SCHEMES ] =
            {
                {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
                {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}
            };

            gl1_defs_coding_type  coding_type = GL1_DEFS_MCS1_CODING;

            /* Accumulate the number of blocks for the different
             * MCS schemes.
             */
            const gpl1_egprs_bler_window_element_T*  window_search_ptr =
            &( EGPL1_BLER_SLIDING_WINDOW[gas_id][ 0 ] );

            uint32  element = 0;

            /* Build up the statistical library */
            while( element < EGPL1_DATA_BLK_WINDOW_SIZE )
            {
                bler_num_blocks_T*  bler_num_blks_ptr =
                    &( EGPL1_NUM_BLER_BLKS[ window_search_ptr->coding_scheme ] );

                if( window_search_ptr->coding_scheme != GL1_DEFS_ACCESS_CODING)
                {

                  bler_num_blks_ptr->num_blks_total++;

                  if( FALSE != window_search_ptr->data_crc_fail )
                  {
                    bler_num_blks_ptr->num_blks_failed++;
                  }
                }
                ++window_search_ptr;
                ++element;
            }

            /* Search for the modulation scheme with at least
             * 80% of the total blocks.
             * ( crc header pass, tfi match ).
             */
            while( coding_type <= GL1_DEFS_MCS9_CODING )
            {
                if( EGPL1_NUM_BLER_BLKS[ coding_type ].num_blks_total >= EIGHTY_PERCENT_OF_WINDOW_SIZE )
                {
                    /* Calculate bler in F0.16 format */
                    uint32  bler =
                        (uint32) ( (EGPL1_NUM_BLER_BLKS[ coding_type ].num_blks_failed) << 16)
                        / (uint32) (EGPL1_NUM_BLER_BLKS[ coding_type ].num_blks_total);

                    /* Work out the MEAN BEP biasing value from
                     * the calculated bler.
                     */
                    uint8  bias_value = 0;

                    /* Continuously compare bler with
                     * threshold values.
                     */
                    if( bler >= EGPL1_30_PERCENT_BLER )
                    {
                      bias_value = 1;

                      if( bler >= EGPL1_50_PERCENT_BLER )
                      {
                          bias_value = 2;
                      }

                      if( GL1_DEFS_MODULATION_GMSK == GPL1_EGPRS_CODING_2_MODULATION[ coding_type ] )
                      {
                          bias_gmsk_mean_bep_report = TRUE;

#if  DEBUG_GPL1_EGPRS_BEP
                          MSG_GERAN_MED_3_G(" Bias gmsk mean bep mcs %u f0.16 bler %u ",
                                  ((uint8) coding_type - (uint8) GL1_DEFS_EXT_ACCESS_CODING),
                                  bler,
                                  0 );
#endif
                      }
                      else
                      {
                          bias_8psk_mean_bep_report = TRUE;

#if  DEBUG_GPL1_EGPRS_BEP
                          MSG_GERAN_MED_3_G(" Bias 8psk mean bep mcs %u f0.16 bler %u ",
                                  ((uint8) coding_type - (uint8) GL1_DEFS_EXT_ACCESS_CODING),
                                  bler,
                                  0 );
#endif
                      }

                      /* Set the biasing value */
                      mean_bep_bias = bias_value;

                    } /* End of if bler is higher than %30 */
                    else if( bler < EGPL1_20_PERCENT_BLER )
                    {
                        comp_with_prev_bep = TRUE;

#if  DEBUG_GPL1_EGPRS_BEP
                        MSG_GERAN_MED_3_G(" mcs %u f0.16 bler %u ",
                                ((uint8)  coding_type - (uint8) GL1_DEFS_EXT_ACCESS_CODING),
                                bler,
                                0 );
#endif
                    }

                    /* Since we are looking for 80% of
                     * of the total number of blocks
                     * break out of the while loop.
                     */
                    break;

                } /* End of if total num blocks for a particular mcs
                   * is higher than or equal to %80.
                   */

                /* Look for the next coding scheme */
                coding_type++;

            } /* End of while mcs scheme is less or equal to mcs 9 */
        }     /* End of if num gmsk or 8psk blocks is higher than zero */

#endif /* EGPL1_OPTIMISE_MEAN_BEP */

        /* Division results in F6.10 format
         * Only divide if accumulated r > 0.
         */
        if( 0 != acc_gmsk_r )
        {
            /* Fill in the gmsk mean/cv bep for the
             * received blocks since last measurement
             * report.
             */
            if( 0 != num_gmsk_blocks )
            {
#if EGPL1_OPTIMISE_MEAN_BEP
                if( !( ( num_8psk_blocks >= EGPL1_MIN_FILT_BLKS_TO_REPORT )
                     &&( num_gmsk_blocks < EGPL1_MIN_FILT_BLKS_TO_REPORT ) ) )
                {
                    uint8  quantized_gmsk_mean_bep;
#endif
                    const uint16  mean_gmsk_bep =
                        (uint16) ( (acc_mean_gmsk_bep_tn) / acc_gmsk_r ); /* 1/65535 units */

                    const uint16  cv_gmsk_bep =
                        (uint16) ( (acc_cv_gmsk_bep_tn) / acc_gmsk_r );   /* F6.10 format */

                    /* Convert the mean gmsk bep inorder to pass it to the 32-bit quantization
                     * table.
                     * Multiply by (2^32)-1 then divide by ((2^16) - 1 ).
                     * This is the same as multiplying by 65537, optimise to not use multiplication.
                     */
                    const uint32  high_res_mean_gmsk_bep = (mean_gmsk_bep << 16) + mean_gmsk_bep;

#if  DEBUG_GPL1_EGPRS_BEP
                    MSG_GERAN_MED_3_G(" acc gmsk bep mean %u acc r %u ",
                            acc_mean_gmsk_bep_tn,
                            acc_gmsk_r,
                            0 );
#endif
                    /* BEP reporting */
                    qual_report_msg_ptr->gmsk_m_bep =
                        (uint8) gpl1_egprs_meas_scale( GPRS_PL1_GMSK_MEAN_BEP_LOG_SCALE,
                                                       high_res_mean_gmsk_bep );

#if  EGPL1_OPTIMISE_MEAN_BEP
                    /* Save the quantized mean bep */
                    quantized_gmsk_mean_bep = qual_report_msg_ptr->gmsk_m_bep;

                    if( quantized_gmsk_mean_bep <= 6 )
                    {
                        /* Dont bias the mean bep value
                         * if it is low.
                         */
                        bias_gmsk_mean_bep_report = FALSE;
                    }

                    if( FALSE != bias_gmsk_mean_bep_report )
                    {
                        /* Need to negatively bias the value
                         * due to high bler.
                         */
                        if( qual_report_msg_ptr->gmsk_m_bep >= mean_bep_bias )
                        {
                            qual_report_msg_ptr->gmsk_m_bep -= mean_bep_bias;
                        }
                    }

                    /* CV BEP reporting */
                    switch( qual_report_msg_ptr->gmsk_m_bep )
                    {
                    case  31:
                      /* Fix cv bep to 7 */
                      qual_report_msg_ptr->gmsk_cv_bep = 7;
                      break;

                    case  30:
                    case  29:
                    case  28:
                    case  27:
                    case  26:

                      /* Quantize cv bep, if it is less than 6 then
                       * set it to 6.
                       * Value provided by Systems Group.
                       */
                      qual_report_msg_ptr->gmsk_cv_bep =
                          (uint8) gpl1_egprs_meas_scale( GPRS_PL1_CV_BEP_SCALE,
                                                         (uint32) cv_gmsk_bep );

                      /* Make sure that a large BEP variance does not
                       * cause network to switch down the MCS when the
                       * MEAN BEP value is high.
                       */
                      if( qual_report_msg_ptr->gmsk_cv_bep < 6 )
                      {
                          qual_report_msg_ptr->gmsk_cv_bep = 6;
                      }
                      break;

                    case  25:
                    case  24:
                    case  23:
                    case  22:

                      if( FALSE != comp_with_prev_bep )
                      {
                          if( gpl1_gprs_serving_cell_data_ptr->previous_mean_bep > 25 )
                          {
                            /* Force the current report to
                             * mean bep 28, cv bep 6.
                             */
                            qual_report_msg_ptr->gmsk_m_bep  = gpl1_gprs_serving_cell_data_ptr->previous_mean_bep;
                            qual_report_msg_ptr->gmsk_cv_bep = 6;
                            break;
                          }
                      }
                    default: /*lint !e616 */

#endif /* EGPL1_OPTIMISE_MEAN_BEP */

                      /* Fill in the cv bep into the report */
                      qual_report_msg_ptr->gmsk_cv_bep =
                          (uint8) gpl1_egprs_meas_scale( GPRS_PL1_CV_BEP_SCALE,
                                                         (uint32) cv_gmsk_bep );
#if  EGPL1_OPTIMISE_MEAN_BEP
                      break;
                    } /* End of switch statement on reported mean bep */

                    if(  ( num_gmsk_blocks >= EGPL1_MIN_FILT_BLKS_TO_REPORT )
                       &&( num_8psk_blocks < EGPL1_MIN_FILT_BLKS_TO_REPORT  ) )
                    {
                      /* Save the quantized gmsk mean bep value
                       * only when 8psk mean bep is not included in this
                       * report to prevent early over-write of previous
                       * mean bep value.
                       */
                      gpl1_gprs_serving_cell_data_ptr->previous_mean_bep = quantized_gmsk_mean_bep;
                    }

#endif /* EGPL1_OPTIMISE_MEAN_BEP */

                    qual_report_msg_ptr->gmsk_mean_bep_present = TRUE;

#if  DEBUG_GPL1_EGPRS_BEP
                    MSG_GERAN_MED_3_G(" gmsk bep mean %u cv %u quantised bep %u ",
                            mean_gmsk_bep,
                            cv_gmsk_bep,
                            qual_report_msg_ptr->gmsk_m_bep );
#endif

#if EGPL1_OPTIMISE_MEAN_BEP
                } /* End of if !(num 8psk blks >= MIN && num gmsk blks < MIN) */
#endif
            }     /* End of if num gmsk blocks is not equal to zero */
        }         /* End of if gmsk reliability factor is not equal to zero */

        if( 0 != acc_8psk_r )
        {
            /* Fill in the 8psk mean/cv bep for the
             * received blocks since last measurement
             * report.
             */
            if( 0 != num_8psk_blocks )
            {

#if  EGPL1_OPTIMISE_MEAN_BEP
                if( !(  ( num_gmsk_blocks >= EGPL1_MIN_FILT_BLKS_TO_REPORT )
                      &&( num_8psk_blocks < EGPL1_MIN_FILT_BLKS_TO_REPORT ) ) )
                {
                    uint8  quantized_8psk_mean_bep;
#endif
                    const uint16  mean_8psk_bep =
                        (uint16) ( (acc_mean_8psk_bep_tn) / acc_8psk_r ); /* 1/65535 units */

                    const uint16  cv_8psk_bep =
                        (uint16) ( (acc_cv_8psk_bep_tn) / acc_8psk_r );    /* F6.10 format */

                    /* Convert the mean gmsk bep inorder to pass it to the 32-bit quantization
                     * table.
                     * Multiply by (2^32)-1 then divide by ((2^16) - 1 ).
                     * This is the same as multiplying by 65537, optimise to not use multiplication.
                     */
                    const uint32  high_res_mean_8psk_bep = (mean_8psk_bep << 16) + mean_8psk_bep;

#if  DEBUG_GPL1_EGPRS_BEP
                    MSG_GERAN_MED_3_G(" acc 8psk bep mean %u acc r %u ",
                            acc_mean_8psk_bep_tn,
                            acc_8psk_r,
                            0 );
#endif
                    /* BEP reporting */
                    qual_report_msg_ptr->psk_m_bep =
                        (uint8) gpl1_egprs_meas_scale( GPRS_PL1_8PSK_MEAN_BEP_LOG_SCALE,
                                                       high_res_mean_8psk_bep );

#if  EGPL1_OPTIMISE_MEAN_BEP
                    /* Save the quantized mean bep */
                    quantized_8psk_mean_bep = qual_report_msg_ptr->psk_m_bep;

                    if( quantized_8psk_mean_bep <= 6 )
                    {
                        /* Dont bias the mean bep value
                         * if it is low.
                         */
                        bias_8psk_mean_bep_report = FALSE;
                    }

                    if( FALSE != bias_8psk_mean_bep_report )
                    {
                        /* Need to negatively bias the value
                         * due to high bler.
                         */
                        if( qual_report_msg_ptr->psk_m_bep >= mean_bep_bias )
                        {
                            qual_report_msg_ptr->psk_m_bep -= mean_bep_bias;
                        }
                    }

                    /* CV BEP reporting */
                    switch( qual_report_msg_ptr->psk_m_bep )
                    {
                    case  31:
                      /* Fix cv bep to 7 */
                      qual_report_msg_ptr->psk_cv_bep = 7;
                      break;

                    case  30:
                    case  29:
                    case  28:
                    case  27:
                    case  26:

                      /* Quantize cv bep, if it is less than 6 then
                       * set it to 6.
                       * Value provided by Systems Group.
                       */
                      qual_report_msg_ptr->psk_cv_bep =
                          (uint8) gpl1_egprs_meas_scale( GPRS_PL1_CV_BEP_SCALE,
                                                         (uint32) cv_8psk_bep );

                      /* Make sure that a large BEP variance does not
                       * cause network to switch down the MCS when the
                       * MEAN BEP value is high.
                       */
                      if( qual_report_msg_ptr->psk_cv_bep < 6 )
                      {
                          qual_report_msg_ptr->psk_cv_bep = 6;
                      }
                      break;

                    case  25:
                    case  24:
                    case  23:
                    case  22:

                      if( FALSE != comp_with_prev_bep )
                      {
                          if( gpl1_gprs_serving_cell_data_ptr->previous_mean_bep > 25 )
                          {
                            /* Force the current report to
                             * mean bep 28, cv bep 6.
                             */
                            qual_report_msg_ptr->psk_m_bep  = gpl1_gprs_serving_cell_data_ptr->previous_mean_bep;
                            qual_report_msg_ptr->psk_cv_bep = 6;
                            break;
                          }
                      }
                    default: /*lint !e616 */

#endif /* EGPL1_OPTIMISE_MEAN_BEP */

                      /* Fill in the cv bep into the report */
                      qual_report_msg_ptr->psk_cv_bep =
                          (uint8) gpl1_egprs_meas_scale( GPRS_PL1_CV_BEP_SCALE,
                                                         (uint32) cv_8psk_bep );
#if  EGPL1_OPTIMISE_MEAN_BEP
                      break;
                    } /* End of switch statement on reported mean bep */

                    /* Save the quantized 8psk mean bep value */
                    gpl1_gprs_serving_cell_data_ptr->previous_mean_bep = quantized_8psk_mean_bep;

#endif /* EGPL1_OPTIMISE_MEAN_BEP */

                    qual_report_msg_ptr->psk_mean_bep_present = TRUE;

#if  DEBUG_GPL1_EGPRS_BEP
                    MSG_GERAN_MED_3_G(" 8psk bep mean %u cv %u quantised bep %u",
                            mean_8psk_bep,
                            cv_8psk_bep,
                            qual_report_msg_ptr->psk_m_bep );
#endif

#if  EGPL1_OPTIMISE_MEAN_BEP
                } /* End of if !(num gmsk blks >= MIN && num 8psk blks < MIN) */
#endif
            }     /* End of if num 8psk blocks is not equal to zero */
        }         /* End of if 8psk reliability factor is not equal to zero */

        switch( gprs_pl1_scell_meas_ptr->link_qual_meas_mode )
        {
            case BOTH_MEASUREMENTS:
            case BEP_MEASUREMENTS:
            {
                if( ( 0 != num_gmsk_blocks )
                  ||( 0 != num_8psk_blocks ) )
                {
                    /* Used for filling the individual ts measurements in the msg */
                    uint8  *tn_bitmap_ptr = &( qual_report_msg_ptr->tn_bitmap );

                    /* Report the per-timeslot Bep.
                     * This is dependent of the value of the link quality
                     * measurement mode.
                     */
                    uint8  *mean_bep_ptr = &( qual_report_msg_ptr->mean_bep[(GL1_DEFS_SLOTS_IN_FRAME - 1)] );

                    /* Should report whatever the network asks for( i.e. dl assignment ).
                     * The shift to the right is for reporting 4 MSBs of the 5.
                     */
                    qual_report_msg_ptr->bep_meas_rpt_flg = valid;

                    /* Initialise to 0 for the benefit of any per-timeslot gmsk
                     * reporting.
                     */
                    qual_report_msg_ptr->gmsk_or_8psk_bitmap = 0;

                    timeslot = GL1_DEFS_SLOTS_IN_FRAME;
                    while( timeslot-- != 0 )
                    {
                        if( 0 != (uint8) ( ( dl_timeslot_alloc >> timeslot ) & 0x01 ) )
                        {
                            if( ( 0 != ts_gmsk_qual_meas_ptr->num_blocks )
                              &&( (ts_gmsk_qual_meas_ptr->num_blocks) >= (ts_8psk_qual_meas_ptr->num_blocks) ) )
                            {
                                /* Convert the mean gmsk bep inorder to pass it to the 32-bit quantization
                                 * table.
                                 * Multiply by (2^32)-1 then divide by ((2^16) - 1 ).
                                 * This is the same as multiplying by 65537, optimise to not use multiplication.
                                 */
                                const uint32  high_res_gmsk_bep =
                                    (ts_gmsk_qual_meas_ptr->mean_bep_tn << 16) + ts_gmsk_qual_meas_ptr->mean_bep_tn;

                                (*tn_bitmap_ptr) |= (uint8) (0x0100 >> (8-timeslot) );

                                /* Mark the bep measurement type for this timeslot
                                 * as a gmsk one by not filling anything in the
                                 * gmsk_or_8psk_bitmap.
                                 */

                                /* Shift by 1 inorder to report 4 most significant bits. ETSI 04.60 */
                                *mean_bep_ptr = (uint8) ( gpl1_egprs_meas_scale( GPRS_PL1_GMSK_MEAN_BEP_LOG_SCALE,
                                                                                 high_res_gmsk_bep ) >> 1 );
                            }
                            else if( 0 != ts_8psk_qual_meas_ptr->num_blocks )
                            {
                                /* Convert the mean gmsk bep inorder to pass it to the 32-bit quantization
                                 * table.
                                 * Multiply by (2^32)-1 then divide by ((2^16) - 1 ).
                                 * This is the same as multiplying by 65537, optimise to not use multiplication.
                                 */
                                const uint32  high_res_8psk_bep =
                                    (ts_8psk_qual_meas_ptr->mean_bep_tn << 16) + ts_8psk_qual_meas_ptr->mean_bep_tn;

                                (*tn_bitmap_ptr) |= (uint8) (0x0100 >> (8-timeslot) );

                                /* Mark the bep measurement type for this timeslot
                                 * as a 8psk one
                                 */
                                (qual_report_msg_ptr->gmsk_or_8psk_bitmap) |=
                                    (uint8) (0x0100 >> (8-timeslot) );

                                /* Shift by 1 inorder to report 4 most significant bits. ETSI 04.60 */
                                *mean_bep_ptr = (uint8) ( gpl1_egprs_meas_scale( GPRS_PL1_8PSK_MEAN_BEP_LOG_SCALE,
                                                                                 high_res_8psk_bep ) >> 1 );
                            }
                        }

                        /* Reset the serving cell qual measurements used for this averaging
                         * period. ETSI 05.08 does not specifically ask for a reset in the
                         * EGPRS case but does for GPRS.
                         */

                        /* Warning: Be ware of multislot polling since the following
                         * reports generated in this frame for the rest of the dl slots
                         * will be meaningless.
                         */
                        ts_gmsk_qual_meas_ptr->num_blocks  = 0;
                        ts_gmsk_qual_meas_ptr--;

                        ts_8psk_qual_meas_ptr->num_blocks  = 0;
                        ts_8psk_qual_meas_ptr--;

                        /* decrement the msg ptr inorder to stay in step */
                        mean_bep_ptr--;
                    }  /* End of while loop */

                    /* Reset ts_qual_meas_ptr so its does not point outside of array */
                    ts_gmsk_qual_meas_ptr =
                        &( gprs_pl1_scell_meas_ptr->qual_meas[ GL1_DEFS_MODULATION_GMSK ]
                                                             [ GL1_DEFS_SLOTS_IN_FRAME - 1 ] );

                    ts_8psk_qual_meas_ptr =
                        &( gprs_pl1_scell_meas_ptr->qual_meas[ GL1_DEFS_MODULATION_8PSK ]
                                                             [ GL1_DEFS_SLOTS_IN_FRAME - 1 ] );
                } /* End of if either gmsk or 8psk blocks have been filtered */
                else
                {
                    qual_report_msg_ptr->tn_bitmap = 0x0;
                }
            }     /* End of case both || bep measurements */
            break;

            default:
            {
                timeslot = GL1_DEFS_SLOTS_IN_FRAME;
                while( timeslot-- != 0 )
                {
                    /* Reset the serving cell qual measurements used for this averaging
                     * period. ETSI 05.08 does not specifically ask for a reset in the
                     * EGPRS case but does for GPRS.
                     */

                    /* Warning: Be ware of multislot polling since the following
                     * reports generated in this frame for the rest of the dl slots
                     * will be meaningless.
                     */
                    ts_gmsk_qual_meas_ptr->num_blocks  = 0;
                    ts_gmsk_qual_meas_ptr--;

                    ts_8psk_qual_meas_ptr->num_blocks  = 0;
                    ts_8psk_qual_meas_ptr--;

                }  /* End of while loop */

                /* Reset ts_qual_meas_ptr so its does not point outside of array */
                ts_gmsk_qual_meas_ptr =
                    &( gprs_pl1_scell_meas_ptr->qual_meas[ GL1_DEFS_MODULATION_GMSK ]
                                                         [ GL1_DEFS_SLOTS_IN_FRAME - 1 ] );

                ts_8psk_qual_meas_ptr =
                    &( gprs_pl1_scell_meas_ptr->qual_meas[ GL1_DEFS_MODULATION_8PSK ]
                                                         [ GL1_DEFS_SLOTS_IN_FRAME - 1 ] );
            }
            break;
        } /* End of switch statement on link quality measurement mode */
    }     /* End of if forgetting factor is valid */

    switch( gprs_pl1_scell_meas_ptr->link_qual_meas_mode )
    {
    case BOTH_MEASUREMENTS:
    {
        /* Local variable used to filter out the reported
         * interference measurement timeslots in the
         * alternating fashion.
         */
        interf_meas_ts_bitmap = (uint32) gprs_pl1_scell_meas_ptr->interf_meas_ts_bitmap;

        /* Alternate the interference measurements timeslot
         * bitmap:
         * TIMESLOTS_0_1_2_3 ---> TIMESLOTS_4_5_6_7
         * TIMESLOTS_4_5_6_7 ---> TIMESLOTS_0_1_2_3
         */
        gprs_pl1_scell_meas_ptr->interf_meas_ts_bitmap = (uint8)
            ~(gprs_pl1_scell_meas_ptr->interf_meas_ts_bitmap);
    }

    case INTERF_MEASUREMENTS: /*lint !e616 : no break statement for avoiding repetition */
    {
        /* Setup the source, destination interference measurement pointers
         * used for filling the interference measurements
         */
        uint8*  i_level_bitmap_ptr = &( qual_report_msg_ptr->i_level_bitmap );
        uint8*  i_level_ptr        = &( qual_report_msg_ptr->i_level[(GL1_DEFS_SLOTS_IN_FRAME - 1)] );

        interference_T* tn_interf_ptr = &( gprs_pl1_scell_meas_ptr->int_array[(GL1_DEFS_SLOTS_IN_FRAME - 1)] );

        timeslot = GL1_DEFS_SLOTS_IN_FRAME;

        /* Fill in the interference measurements
         * into the report message.
         */
        while( timeslot-- != 0 )
        {
            /* Mark measurement as not present and over write as neccessary */
            /* *i_level_bitmap_ptr */

            /* Determine which channels have had at least one measurement on
             * Currently only add interference measurements when in transfer mode.
             */
            if( FALSE != tn_interf_ptr->valid_ind )
            {
                if( 0 != ( interf_meas_ts_bitmap & (0x0080 >> timeslot ) ) ) /* @@TODO */
                {
                    /*  Convert the measured C-value into the report value. */
                    dBx16_T int_dbm = L1_map_dBm_x16_to_rxlev
                              ( (dBx16_T) ( (tn_interf_ptr->meas_1) >> 8) );

                    /* Reported value is a 4 bit representation of the relative value
                     * of the intereference measurement and the C-value
                     */
                    int8 rxlev_diff = (int8) ( reported_rxlev - int_dbm );

                    if( rxlev_diff >= 0 )
                    {
                        /*  Divide by two and add 1 to give reported value  */
                        rxlev_diff >>= 1;
                        rxlev_diff += 1;

                        /* Range check, max. value 15  */
                        if( rxlev_diff > 15 )
                        {
                            rxlev_diff = 15;
                        }
                    }
                    else
                    {
                        /*  Interference is greater than C - value  */
                        rxlev_diff = 0;
                    }
                    *i_level_ptr = (uint8) rxlev_diff;

                    (*i_level_bitmap_ptr) |= (uint8) (0x0100 >> (8-timeslot) );

#if  DEBUG_GPL1_INTERF_MEAS
                    #error code not present
#endif

                } /* End of if statement */
#if  DEBUG_GPL1_INTERF_MEAS
                #error code not present
#endif
                /* Clear the measurement values and count to reset after reporting */
                /*
                ** CR: 229730 - UE is not reporting Link Quality Measurements as per spec
                ** LINK_QUALITY_MEASUREMENT_MODE
                ** MS shall substitute least-recently-reported and available interference
                ** measurements for time slots not already included in the report.
                **
                tn_interf_ptr->meas_1            = 0;
                tn_interf_ptr->valid_ind         = FALSE;
                */
                tn_interf_ptr->measurement_count = 0;
            } /* End of if valid meas ind for this timeslot */
            i_level_ptr--;
            tn_interf_ptr--;
        } /* End of while loop */

        /* Check the interference measurements timeslot bitmap */
        if( 0 != (*i_level_bitmap_ptr) )
        {
            /* Set the appropriate flags in the message */
            qual_report_msg_ptr->int_meas_rpt_flg = valid;
        }

    } /* End of case BOTH_MEASUREMENTS or INTERF_MEASUREMENTS */
        break;

    default:
        break;
    }  /* End of Switch statement on link quality measurement mode */
}


/*========================= External function Prototypes =====================*/

/*===========================================================================

FUNCTION   gpl1_egprs_scell_qual_meas_broadcast_config( )

DESCRIPTION
  This function is called by the PL1 and used to configure the serving
  cell bep quality measurements with the broadcast bep period off the
  pbcch or bcch.

DEPENDENCIES
  1- Used only during EGPRS.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  gpl1_egprs_scell_qual_meas_broadcast_config( uint8  bep_period, gas_id_t gas_id )
{
    /* Pointer to read the serving cell measurement
     * structure
     */
    gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
    gprs_pl1_serving_cell_meas_T*  gprs_pl1_scell_meas_ptr =gprs_serv_cell_meas_ptr;

    /* Perform a validity check on the parameter
     * before using it in the filtering.
     */
    if( bep_period <= HIGHEST_VALID_BEP_PERIOD_VALUE )
    {
        if( FALSE == EGPL1_BEP_PERIOD_FIELD_RESERVED[ bep_period ] )
        {
            /* If the broadcast bep period value is not reserved
             * then use it to set forgetting factor.
             * The check for reserved value 15 ensures array
             * boundaries are not exceeded.
             */
            if(bep_period < HIGHEST_VALID_BEP_PERIOD_VALUE)
            {
              const uint16  forget_factor = GPRS_PL1_BEP_PERIOD_2_E[ bep_period ]; /* nolint !e661 out of bounds ptr */

              /* Set the broadcast forgetting factor e and the one in use */
              gprs_pl1_scell_meas_ptr->broadcast_e = forget_factor;
              gprs_pl1_scell_meas_ptr->e           = forget_factor;

#if  DEBUG_GPL1_EGPRS_BEP
              MSG_GERAN_MED_3_G(" egprs config bep period %u f6.10 ff %u",
                    bep_period,
                    forget_factor,
                    0 );
#endif
            }
            else
            {
              /* Set the broadcast forgetting factor e and the one in use */
              gprs_pl1_scell_meas_ptr->broadcast_e = GPRS_PL1_FORGET_FACTOR_INVALID;
              gprs_pl1_scell_meas_ptr->e           = GPRS_PL1_FORGET_FACTOR_INVALID;

              /* Print out a debug msg warning */
              MSG_GERAN_ERROR_3_G(" egprs config bep period out of range %u invalid",
                          bep_period,
                          0,
                          0 );

            }
            /* MPKC */
            {
              eng_mode_edge_meas_t edge_meas = {0};
              edge_meas.bep_period = bep_period;
              edge_meas.lq_meas_mode = lq_meas_mode[gas_id];
              geran_eng_mode_data_write(ENG_MODE_EDGE_MEAS, (void *)&edge_meas, TRUE, gas_id);
            }

        } /* End of if the bep period is reserved */
        else
        {
            /* Set the broadcast forgetting factor e and the one in use */
            gprs_pl1_scell_meas_ptr->broadcast_e = GPRS_PL1_FORGET_FACTOR_INVALID;
            gprs_pl1_scell_meas_ptr->e           = GPRS_PL1_FORGET_FACTOR_INVALID;

        }

    } /* End of if bep period is less than or equal to 15 */
    else
    {
        /* Set the broadcast forgetting factor e and the one in use */
        gprs_pl1_scell_meas_ptr->broadcast_e = GPRS_PL1_FORGET_FACTOR_INVALID;
        gprs_pl1_scell_meas_ptr->e           = GPRS_PL1_FORGET_FACTOR_INVALID;

        /* Print out a debug msg warning */
        MSG_GERAN_ERROR_3_G(" egprs config bep period %u invalid",
                  bep_period,
                  0,
                  0 );
    } /* End of else bep period is invalid */
}

/*===========================================================================

FUNCTION   gpl1_egprs_scell_qual_meas_pacch_config( )

DESCRIPTION
  This function is called by the PL1 and used to configure the serving
  cell bep quality measurements with the relevant assignment information.

DEPENDENCIES
  1- Used only during EGPRS.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  gpl1_egprs_scell_qual_meas_pacch_config( uint8  link_qual_meas_mode,
                                               uint8  new_dl_timeslot_alloc,
                                               uint8  bep_period2, gas_id_t gas_id )
{
    /* Pointer to read the serving cell measurement
     * structure
     */
    gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
    gprs_pl1_serving_cell_meas_T *gprs_pl1_scell_meas_ptr =gprs_serv_cell_meas_ptr;

    /* If the quality measurement module is configured as a result
     * of an uplink assignment then dont reset the bitmap since
     * no link quality meas mode is supplied and hence we could
     * incorrectly turn off interference measurements as a result.
     */
    if( 0 != new_dl_timeslot_alloc )
    {
        /* Pointer to update the serving cell measurement
         * structure
         */
        gpl1_egprs_qual_meas_T *ts_gmsk_qual_meas_ptr =
            &( gprs_pl1_scell_meas_ptr->qual_meas[ GL1_DEFS_MODULATION_GMSK ]
                                                 [ GL1_DEFS_SLOTS_IN_FRAME - 1 ] );

        gpl1_egprs_qual_meas_T *ts_8psk_qual_meas_ptr =
            &( gprs_pl1_scell_meas_ptr->qual_meas[ GL1_DEFS_MODULATION_8PSK ]
                                                 [ GL1_DEFS_SLOTS_IN_FRAME - 1 ] );

#if  EGPL1_OPTIMISE_MEAN_BEP

    /* Pointers used to reset the moving average structure
     * contents for GMSK and 8PSK.
     */
    const ts_mean_bep_moving_avg_T*  moving_average_reset_ptr =
        &( EGPL1_MOVING_AVERAGE_RESET );

    ts_mean_bep_moving_avg_T*  ts_moving_avg_gmsk_ptr =
        &( EGPL1_MEAN_BEP_MOVING_AVERAGE[ gas_id ][ GL1_DEFS_MODULATION_GMSK ]
                                        [ (GL1_DEFS_SLOTS_IN_FRAME - 1) ] );

    ts_mean_bep_moving_avg_T*  ts_moving_avg_8psk_ptr =
        &( EGPL1_MEAN_BEP_MOVING_AVERAGE[ gas_id ][ GL1_DEFS_MODULATION_8PSK ]
                                        [ (GL1_DEFS_SLOTS_IN_FRAME - 1) ] );

#endif
        /* Initialise quality measurements for all the timeslots
         * No harm in doing that for the moment, can be optimised
         * to do the maximum no of allowed timeslots for this
         * mutlislot class.
         */
        uint32  timeslot = GL1_DEFS_SLOTS_IN_FRAME; /* Local loop variable */
        while( timeslot-- != 0 )
        {
            if( 0 != ( ( new_dl_timeslot_alloc >> timeslot ) & 0x01 ) )
            {
                ts_gmsk_qual_meas_ptr->mean_bep_tn = 0;
                ts_gmsk_qual_meas_ptr->cv_bep_tn   = 0;
                ts_gmsk_qual_meas_ptr->num_blocks  = 0;
                ts_gmsk_qual_meas_ptr->r           = 0;

                ts_8psk_qual_meas_ptr->mean_bep_tn = 0;
                ts_8psk_qual_meas_ptr->cv_bep_tn   = 0;
                ts_8psk_qual_meas_ptr->num_blocks  = 0;
                ts_8psk_qual_meas_ptr->r           = 0;

#if  EGPL1_OPTIMISE_MEAN_BEP
                /* Use a struct->struct copy to reset the contents
                 * of the moving average structure for GMSK/8PSK.
                 */
                *ts_moving_avg_gmsk_ptr = *moving_average_reset_ptr;
                *ts_moving_avg_8psk_ptr = *moving_average_reset_ptr;
#endif
            }
            ts_gmsk_qual_meas_ptr--;
            ts_8psk_qual_meas_ptr--;

#if  EGPL1_OPTIMISE_MEAN_BEP
            ts_moving_avg_gmsk_ptr--;
            ts_moving_avg_8psk_ptr--;
#endif
        }

        /* Reset the ts_qual_meas_ptrs so it does not point
         * outside of the array
         */
        ts_gmsk_qual_meas_ptr++;
        ts_8psk_qual_meas_ptr++;

#if  EGPL1_OPTIMISE_MEAN_BEP
        ts_moving_avg_gmsk_ptr++;
        ts_moving_avg_8psk_ptr++;
#endif

    } /* End of if there are new dl assignment timeslots
       * allocated.
       */

    if( INVALID_MEAS_MODE != link_qual_meas_mode )
    {
        /* Perform a validity check on the parameter
         * before using it in the filtering.
         */
        if( link_qual_meas_mode < INVALID_MEAS_MODE )
        {
            /* Initialise the interference measurements timeslot
             * bitmap.
             */
            gprs_pl1_scell_meas_ptr->interf_meas_ts_bitmap =
                EGPRS_PL1_INTERF_MEAS_BITMAP[ link_qual_meas_mode ];

            /* Store the link quality measurement mode */
            gprs_pl1_scell_meas_ptr->link_qual_meas_mode =
              link_qual_meas_mode;

#if  DEBUG_GPL1_EGPRS_BEP
            MSG_GERAN_MED_3_G(" egprs config meas mode %u ",
                    link_qual_meas_mode,
                    0,
                    0 );
#endif
            /* MPKC */
            {
              eng_mode_edge_meas_t edge_meas = {0};
              edge_meas.bep_period = bep_period2;
              edge_meas.lq_meas_mode = link_qual_meas_mode;
              lq_meas_mode[gas_id] = link_qual_meas_mode;
              geran_eng_mode_data_write(ENG_MODE_EDGE_MEAS, (void *)&edge_meas, TRUE, gas_id);
            }

        } /* End of if( ) meas mode < invalid value */
        else
        {
            /* Print out a debug msg warning */
            MSG_GERAN_ERROR_1_G(" egprs config meas mode %u invalid ",link_qual_meas_mode);
        } /* End of else meas mode is invalid */
    }
    else
    {
        /* This is an uplink assignment which has no meas mode,
         * in which case we only need to update the bep period.
         * Or a PDCH timeslot release in which case the current
         * configuration of the quality measurement module
         * should not be altered.
         */
    }

    /* Test the bep period2 value inorder to determine
     * the forgetting factor.
     */
    switch( bep_period2 )
    {
    case GPRS_PL1_BEP_PERIOD2_NORMAL:
    case GPRS_PL1_BEP_PERIOD2_INVALID:

        /* If not present or normal, bep period2 received in a previous message
         * of the same tbf session shall be used instead of bep period.
         */
        if( GPRS_PL1_FORGET_FACTOR_INVALID == gprs_pl1_scell_meas_ptr->e )
        {
            /* If no valid bep period2 was received during this tbf then
             * use the forgetting factor resulting from the broadcast
             * bep period.
             */
            if( ( GPRS_PL1_FORGET_FACTOR_INVALID != gprs_pl1_scell_meas_ptr->broadcast_e )
              &&( gprs_pl1_scell_meas_ptr->broadcast_e <= BEP_PERIOD2_FIELD_VALUE_0 )
              &&( gprs_pl1_scell_meas_ptr->broadcast_e >= BEP_PERIOD2_FIELD_VALUE_10 )
              )
            {
                /* Set the forgetting factor e */
                gprs_pl1_scell_meas_ptr->e = gprs_pl1_scell_meas_ptr->broadcast_e;

#if  DEBUG_GPL1_EGPRS_BEP
            MSG_GERAN_MED_3_G(" egprs config broadcast f6.10 ff %u",
                    gprs_pl1_scell_meas_ptr->e,
                    0,
                    0 );
#endif
            }
        }
        break;

    default:

        /* Perform a validity check on the parameter
         * before using it in the filtering.
         */
        if( bep_period2 < HIGHEST_VALID_BEP_PERIOD_VALUE )
        {
            /* Set the forgetting factor e
             * The check for normal/invalid values using the switch
             * statement ensures array boundaries are not exceeded.
             */
            gprs_pl1_scell_meas_ptr->e = GPRS_PL1_BEP_PERIOD_2_E[ bep_period2 ];

#if  DEBUG_GPL1_EGPRS_BEP
            MSG_GERAN_MED_3_G(" egprs config bep period2 %u f6.10 ff %u",
                    bep_period2,
                    gprs_pl1_scell_meas_ptr->e,
                    0 );
#endif
        } /* End of if( ) statement */
        else
        {
            /* Print out a debug msg warning */
            MSG_GERAN_ERROR_1_G(" egprs config bep period2 %u invalid",bep_period2);

            if( GPRS_PL1_FORGET_FACTOR_INVALID == gprs_pl1_scell_meas_ptr->e )
            {
                if( ( GPRS_PL1_FORGET_FACTOR_INVALID != gprs_pl1_scell_meas_ptr->broadcast_e )
                  &&( gprs_pl1_scell_meas_ptr->broadcast_e <= BEP_PERIOD2_FIELD_VALUE_0 )
                  &&( gprs_pl1_scell_meas_ptr->broadcast_e >= BEP_PERIOD2_FIELD_VALUE_10 )
                  )
                {
                    /* Set the forgetting factor e */
                    gprs_pl1_scell_meas_ptr->e = gprs_pl1_scell_meas_ptr->broadcast_e;

#if  DEBUG_GPL1_EGPRS_BEP
                    MSG_GERAN_MED_3_G(" egprs config broadcast f6.10 ff %u",
                            gprs_pl1_scell_meas_ptr->e,
                            0,
                            0 );
#endif
                }
            }

        } /* End of else bep period is invalid */

        break;

    } /* End of switch on bep period2 */
}

/*===========================================================================

FUNCTION   gpl1_egprs_scell_qual_meas_nv_item_config( )

DESCRIPTION
  This function is called by the PL1 and used to configure the serving
  cell bep quality measurements with the relevant nv item information.

DEPENDENCIES
  1- Used only for EGPRS quality measurements.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  gpl1_egprs_scell_qual_meas_nv_item_config( boolean  gcf_item_set, gas_id_t gas_id )
{
  gpl1_gprs_serving_cell_data_t *gpl1_gprs_serving_cell_data_ptr = &gpl1_gprs_serving_cell_data[gas_id];
  /* Test the flag and setup the data callback function pointer */
  if( FALSE == gcf_item_set )
  {
    /* Use the ota routines */
    gpl1_gprs_serving_cell_data_ptr->gpl1_egprs_data_cb_fptr          = gpl1_egprs_serv_cell_ota_data_cb;
    gpl1_gprs_serving_cell_data_ptr->gpl1_egprs_qual_meas_report_fptr = gpl1_egprs_serv_cell_qual_meas_report_ota;
  }
  else
  {
    /* Use the GCF routines */
    gpl1_gprs_serving_cell_data_ptr->gpl1_egprs_data_cb_fptr          = gpl1_egprs_serv_cell_gcf_data_cb;
    gpl1_gprs_serving_cell_data_ptr->gpl1_egprs_qual_meas_report_fptr = gpl1_egprs_serv_cell_qual_meas_report_gcf;
  }

  /* Print an F3 message, this is an important event
   * and we need to make sure this does not happen in
   * the middle of a DL TBF.
   */
  MSG_GERAN_MED_3_G(" EGPRS fptrs set %u dcb 0x%p mrpt 0x%p",
          gcf_item_set,
          gpl1_gprs_serving_cell_data_ptr->gpl1_egprs_data_cb_fptr,
          gpl1_gprs_serving_cell_data_ptr->gpl1_egprs_qual_meas_report_fptr );
}

/*===========================================================================

FUNCTION  egpl1_calc_block_statistical_params( )

DESCRIPTION
  This function supplies the client with the following statistical
  parameters: Mean and Coefficient of variation.

  The format of the CV bep is F6.10 which is the same used
  by the egprs bep quality measurement module.

DEPENDENCIES
  1- Used only during EGPRS.

RETURN VALUE
  Coefficient of variation of bep in F6.10 format.

SIDE EFFECTS
  None

===========================================================================*/
uint16  egpl1_calc_block_statistical_params( const uint16  bep[ GL1_DEFS_FRAMES_IN_BLK_PERIOD ],
                                             uint16       *mean_bep_ptr,
                                             gas_id_t      gas_id)
{
    /* F6.10 format */
    uint32  cv_bep_per_blk;
    uint32  mean_bep_per_blk;

    gl1_msg_process_bep( &( bep[ 0 ] ),
                         &( mean_bep_per_blk ),
                         &( cv_bep_per_blk )
                         );

    /* Fill in the mean bep in F0.16 format */
    *mean_bep_ptr = (uint16) mean_bep_per_blk;

    /* Reduce resolution of cv bep from F0.16 -> F6.10
     * Return to client.
     */
    return  (uint16) ( cv_bep_per_blk >> 6 );
}

/*===========================================================================

FUNCTION  gpl1_calc_gprs_egprs_dl_bler( )

DESCRIPTION
  This function calculates the dl bler based on the total number of payload
  crc passes and failures across all DL timeslots.

  The format of the bler is F0.16

DEPENDENCIES
  1- Used during EGPRS timer based bler printing.

  2- Used in GPRS to print bler when the measurement report is built and 
     also based on the bler timer.

  In both cases, it will be called from task context so mutex protection
  is required.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  gpl1_calc_gprs_egprs_dl_bler( tbf_mode_T  egprs_or_gprs_tbf,
                                    gas_id_t    gas_id
                                  )
{
  bler_num_blocks_T  EGPL1_NUM_BLER_BLKS[ NUM_CODING_SCHEMES ] =
    {
      {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
      {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}
    };

  gl1_defs_coding_type  coding_type = GL1_DEFS_CS1_CODING;

  /* Accumulate the number of blocks for the different
   * MCS schemes.
   */
  const gpl1_egprs_bler_window_element_T*  window_search_ptr =
    &( EGPL1_BLER_SLIDING_WINDOW[gas_id][ 0 ] );

  uint32  element = 0;

  GL1_ISR_LOCK(gas_id);

  switch( egprs_or_gprs_tbf )
  {
  case TBF_MODE_GPRS:

    /* Build up the statistical library */
    while( element < EGPL1_DATA_BLK_WINDOW_SIZE )
    {
      bler_num_blocks_T*  bler_num_blks_ptr =
        &( EGPL1_NUM_BLER_BLKS[ window_search_ptr->coding_scheme ] );

      /* Needed because the sliding window might be partially filled in the beginning */
      if( coding_type != GL1_DEFS_ACCESS_CODING)
      {
        bler_num_blks_ptr->num_blks_total++;

        if( FALSE != window_search_ptr->data_crc_fail )
        {
          bler_num_blks_ptr->num_blks_failed++;
        }
      }

      ++window_search_ptr;
      ++element;
    }

    coding_type = GL1_DEFS_CS1_CODING;

    while( coding_type <= GL1_DEFS_CS4_CODING )
    {
      if( coding_type != GL1_DEFS_ACCESS_CODING )
      {
        if( EGPL1_NUM_BLER_BLKS[ coding_type ].num_blks_total >= EIGHTY_PERCENT_OF_WINDOW_SIZE )
        {
          /* Calculate bler in F0.16 format */
          uint32  bler =
            (uint32) ( (EGPL1_NUM_BLER_BLKS[ coding_type ].num_blks_failed) << 16)
              / (uint32) (EGPL1_NUM_BLER_BLKS[ coding_type ].num_blks_total);

          MSG_GERAN_MED_3_G(" num failed %u total %u ",
                            EGPL1_NUM_BLER_BLKS[ coding_type ].num_blks_failed,
                            EGPL1_NUM_BLER_BLKS[ coding_type ].num_blks_total,
                            0 );

          MSG_GERAN_MED_3_G(" cs %u f0.16 bler %u ",
                            ((uint8)(coding_type+1)),
                            bler,
                            0 );
        }
      }
      coding_type++;
    }

  break;

  case TBF_MODE_EGPRS:

    /* Build up the statistical library */
    while( element < EGPL1_DATA_BLK_WINDOW_SIZE )
    {
      bler_num_blocks_T*  bler_num_blks_ptr =
        &( EGPL1_NUM_BLER_BLKS[ window_search_ptr->coding_scheme ] );

      /* Needed because the sliding window might be partially filled in the beginning */
      if( coding_type != GL1_DEFS_ACCESS_CODING)
      {
        bler_num_blks_ptr->num_blks_total++;

        if( FALSE != window_search_ptr->data_crc_fail )
        {
          bler_num_blks_ptr->num_blks_failed++;
        }
      }

      ++window_search_ptr;
      ++element;
    }

    coding_type = GL1_DEFS_MCS1_CODING;

    while( coding_type <= GL1_DEFS_MCS9_CODING )
    {
      if( coding_type != GL1_DEFS_ACCESS_CODING)
      {
        if( EGPL1_NUM_BLER_BLKS[ coding_type ].num_blks_total >= EIGHTY_PERCENT_OF_WINDOW_SIZE )
        {
          /* Calculate bler in F0.16 format */
          uint32  bler =
            (uint32) ( (EGPL1_NUM_BLER_BLKS[ coding_type ].num_blks_failed) << 16)
              / (uint32) (EGPL1_NUM_BLER_BLKS[ coding_type ].num_blks_total);

          MSG_GERAN_MED_3_G(" num failed %u total %u ",
                            EGPL1_NUM_BLER_BLKS[ coding_type ].num_blks_failed,
                            EGPL1_NUM_BLER_BLKS[ coding_type ].num_blks_total,
                            0 );

          MSG_GERAN_MED_3_G(" mcs %u f0.16 bler %u ",
                            ((uint8)  coding_type - (uint8) GL1_DEFS_EXT_ACCESS_CODING),
                            bler,
                            0 );
        }
      }
      coding_type++;
    }


  break;

  default:

    MSG_GERAN_ERROR_1_G(" invalid tbf mode %d ",egprs_or_gprs_tbf );

  break;

  }   /* switch( egprs_or_gprs_tbf ) */


  GL1_ISR_UNLOCK(gas_id);
}

/*===========================================================================

FUNCTION  gpl1_egprs_serv_cell_data_cb( )

DESCRIPTION
  This function is called by the NPL1 data call back and it calculates and
  accumulates mean/cv bep measurements into the serving cell measurements structure.

  All calculations here are based on F6.10 format ( 3 decimal places ).

DEPENDENCIES
  1- Used only during EGPRS.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  gpl1_egprs_serv_cell_data_cb( gl1_defs_tn_type                   timeslot,
                                    const gpl1_egprs_decode_metrics_T *egprs_decode_metrics_ptr,
                                    boolean                            tfi_match,
                                    usf_dl_tfi_info_t*                 usf_dl_tfi_info_ptr,
                                    gas_id_t                           gas_id )
{
  gpl1_gprs_serving_cell_data_t *gpl1_gprs_serving_cell_data_ptr = &gpl1_gprs_serving_cell_data[gas_id];
  /* Make sure the function pointer is valid first */
  if(  (  ( gpl1_egprs_serv_cell_ota_data_cb == gpl1_gprs_serving_cell_data_ptr->gpl1_egprs_data_cb_fptr )
        ||( gpl1_egprs_serv_cell_gcf_data_cb == gpl1_gprs_serving_cell_data_ptr->gpl1_egprs_data_cb_fptr )
       )
    )
  {

    /* Check if the fptr has changed since the last data cb */
      if( gpl1_gprs_serving_cell_data_ptr->gpl1_egprs_data_cb_fptr != gpl1_egprs_previous_data_cb_fptr[gas_id] )
    {
      /* Print a warning message if it has changed.
       * This is not always a problem.
       */
      MSG_GERAN_HIGH_3_G(" dcb ptr has changed prev 0x%p curr 0x%p",
                 gpl1_egprs_previous_data_cb_fptr[gas_id],
               gpl1_gprs_serving_cell_data_ptr->gpl1_egprs_data_cb_fptr,
               0 );
    }

    /* Set the prev fptr now */
      gpl1_egprs_previous_data_cb_fptr[gas_id] = (gpl1_egprs_previous_data_cb_type) gpl1_gprs_serving_cell_data_ptr->gpl1_egprs_data_cb_fptr;
    

    /* De-reference and pass the necessary arguments */
    gpl1_gprs_serving_cell_data_ptr->gpl1_egprs_data_cb_fptr( timeslot,
                             egprs_decode_metrics_ptr,
                             tfi_match,
                             usf_dl_tfi_info_ptr,
                             gas_id );
  }
  else
  {
    /* Print an error message since the fptr is
     * either NULL or has a rogue address.
     */
    MSG_GERAN_ERROR_3_G(" Invalid egprs data cb fptr 0x%p ",
              gpl1_gprs_serving_cell_data_ptr->gpl1_egprs_data_cb_fptr,
              0,
              0 );
  }
}

/*===========================================================================

FUNCTION  gpl1_egprs_serv_cell_qual_meas_report( )

DESCRIPTION
  This function is called by the PL1 and is used to fill in the EGPRS quality
  measurement report.
DEPENDENCIES
  1- Used only during EGPRS.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  gpl1_egprs_serv_cell_qual_meas_report( egprs_chn_qual_rpt_t *qual_report_msg_ptr,
                                             uint8                 dl_timeslot_alloc, gas_id_t gas_id )
{
  gpl1_gprs_serving_cell_data_t *gpl1_gprs_serving_cell_data_ptr = &gpl1_gprs_serving_cell_data[gas_id];
  /* Sanity check the fptr */
  if(  (  ( gpl1_egprs_serv_cell_qual_meas_report_ota == gpl1_gprs_serving_cell_data_ptr->gpl1_egprs_qual_meas_report_fptr )
        ||( gpl1_egprs_serv_cell_qual_meas_report_gcf == gpl1_gprs_serving_cell_data_ptr->gpl1_egprs_qual_meas_report_fptr )
       )
    )
  {

    /* Check if the fptr has changed since the last data cb */
    if( gpl1_gprs_serving_cell_data_ptr->gpl1_egprs_qual_meas_report_fptr != gpl1_egprs_previous_qual_meas_report_fptr[gas_id] )
    {
      /* Print a warning message if it has changed.
       * This is not always a problem.
       */
      MSG_GERAN_HIGH_3_G(" qm rpt ptr has changed prev 0x%p curr 0x%p",
               gpl1_egprs_previous_qual_meas_report_fptr[gas_id],
               gpl1_gprs_serving_cell_data_ptr->gpl1_egprs_qual_meas_report_fptr,
               0 );
    }

    /* Set the prev fptr now */
    gpl1_egprs_previous_qual_meas_report_fptr[gas_id] = (gpl1_egprs_previous_qual_meas_report_type) gpl1_gprs_serving_cell_data_ptr->gpl1_egprs_qual_meas_report_fptr;

    if(qual_report_msg_ptr->gmsk_mean_bep_present == TRUE)
    {
      geran_eng_mode_data_write(ENG_MODE_GMSK_CV_VALUE, &(qual_report_msg_ptr->gmsk_cv_bep), TRUE, gas_id);
    }

    if(qual_report_msg_ptr->psk_mean_bep_present== TRUE)
    {
      geran_eng_mode_data_write(ENG_MODE_PSK_CV_VALUE, &(qual_report_msg_ptr->psk_cv_bep), TRUE, gas_id);
    }

    /* De-reference the fptr and pass the required arguments */
    gpl1_gprs_serving_cell_data_ptr->gpl1_egprs_qual_meas_report_fptr( qual_report_msg_ptr,
                                      dl_timeslot_alloc,gas_id );
  }
  else
  {
    /* Report an error since the fptr has an invalid address */
    MSG_GERAN_ERROR_3_G(" Invalid qual meas fptr 0x%p ",
              gpl1_gprs_serving_cell_data_ptr->gpl1_egprs_qual_meas_report_fptr,
              0,
              0 );
  }
}

/*===========================================================================

FUNCTION  gpl1_egprs_serv_cell_qual_meas_reset( )

DESCRIPTION
  This function is called by the PL1 and is used to reset the EGPRS quality
  measurement module on leaving Packet Transfer.
DEPENDENCIES
  1- Used only during EGPRS.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  gpl1_egprs_serv_cell_qual_meas_reset( gas_id_t gas_id )
{
    /* Pointers to read the serving cell measurement
     * structure
     */
    gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
    gpl1_gprs_serving_cell_data_t *gpl1_gprs_serving_cell_data_ptr = &gpl1_gprs_serving_cell_data[gas_id];
    gprs_pl1_serving_cell_meas_T *gprs_pl1_scell_meas_ptr =gprs_serv_cell_meas_ptr;

#if  EGPL1_OPTIMISE_MEAN_BEP
    /* Pointer to the sliding window elements to allow
     * resetting of its contents.
     */
    gpl1_egprs_bler_window_element_T*  element_ptr =
        &( EGPL1_BLER_SLIDING_WINDOW[gas_id][ (EGPL1_DATA_BLK_WINDOW_SIZE - 1) ] );

    /* Pointers used to reset the moving average structure
     * contents for GMSK and 8PSK.
     */
    const ts_mean_bep_moving_avg_T*  moving_average_reset_ptr =
        &( EGPL1_MOVING_AVERAGE_RESET );

    ts_mean_bep_moving_avg_T*  ts_moving_avg_gmsk_ptr =
        &( EGPL1_MEAN_BEP_MOVING_AVERAGE[ gas_id ][ GL1_DEFS_MODULATION_GMSK ]
                                        [ (GL1_DEFS_SLOTS_IN_FRAME - 1) ] );

    ts_mean_bep_moving_avg_T*  ts_moving_avg_8psk_ptr =
        &( EGPL1_MEAN_BEP_MOVING_AVERAGE[ gas_id ][ GL1_DEFS_MODULATION_8PSK ]
                                        [ (GL1_DEFS_SLOTS_IN_FRAME - 1) ] );

    /* Loop variables */
    uint32  element = EGPL1_DATA_BLK_WINDOW_SIZE;

    uint32  timeslot = GL1_DEFS_SLOTS_IN_FRAME;

    /* Reset the sliding window elements */
    while( 0 != element-- )
    {
        /* Reset the content */
        element_ptr->coding_scheme = GL1_DEFS_ACCESS_CODING;
        element_ptr->data_crc_fail = FALSE;

        /* Move to the next element in the array
         * Decrease the element count.
         */
        element_ptr--;
        element--;

        /* Repeat pattern 3 times more to speed up loop
         * Warning: The number of times is dependent on
         * EGPL1_DATA_BLK_WINDOW_SIZE.
         */
        element_ptr->coding_scheme = GL1_DEFS_ACCESS_CODING;
        element_ptr->data_crc_fail = FALSE;

        element_ptr--;
        element--;

        element_ptr->coding_scheme = GL1_DEFS_ACCESS_CODING;
        element_ptr->data_crc_fail = FALSE;

        element_ptr--;
        element--;

        element_ptr->coding_scheme = GL1_DEFS_ACCESS_CODING;
        element_ptr->data_crc_fail = FALSE;

        element_ptr--;
    }

    /* Reset the mean bep tracking variables */
    gpl1_gprs_serving_cell_data_ptr->previous_mean_bep = 0;

    /* Reset the moving average structure contents
     * for GMSK and 8PSK.
     * This is valid for EGPL1_MOVING_AVERAGE_SIZE 3.
     */
    while( timeslot-- != 0 )
    {
        /* Reset the structure, use a struct to struct copy */
        *ts_moving_avg_gmsk_ptr = *moving_average_reset_ptr;
        *ts_moving_avg_8psk_ptr = *moving_average_reset_ptr;

        /* Move to the next timeslot */
        ts_moving_avg_gmsk_ptr--;
        ts_moving_avg_8psk_ptr--;
    }
#endif

    gprs_pl1_scell_meas_ptr->link_qual_meas_mode   = INVALID_MEAS_MODE;
    gprs_pl1_scell_meas_ptr->e                     = GPRS_PL1_FORGET_FACTOR_INVALID;
    gprs_pl1_scell_meas_ptr->interf_meas_ts_bitmap = 0x0;

    gprs_pl1_scell_meas_ptr->egprs_interf_meas_required = TRUE;

    gprs_pl1_scell_meas_ptr->egprs_rxqual_calc = egprs_rxqual_calc_store_reset;
}

/*===========================================================================

FUNCTION  gpl1_egprs_create_qual_meas_node( )

DESCRIPTION
  This function is called by the NPL1 data call back.
  It adds a qual measurement node to the tail of the fifo queue.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  gpl1_egprs_create_qual_meas_node( gl1_defs_tn_type  timeslot,
                                        uint8             rrbp,
                                        uint8             es_p,
                                        uint16            bsn1, gas_id_t gas_id )
{

   gpl1_gprs_serving_cell_data_t *gpl1_gprs_serving_cell_data_ptr = &gpl1_gprs_serving_cell_data[gas_id];

    /* OG changes to generate EPDAN REQ as late as possible */
    uint32  tx_fn = L1_GPRS_INVALID_FN;

    uint32  current_fn       = gl1_get_FN(gas_id );
    uint8   current_fn_mod13 = (uint8) frame_counters[gas_id].FNmod13;

    /* Used to choose the correct pipelined array element */
    gpl1_egprs_qual_meas_queue_T *node_ptr = &( gpl1_gprs_serving_cell_data_ptr->qual_meas_node[ gpl1_gprs_serving_cell_data_ptr->element ] );
    gpl1_egprs_qual_meas_data_T  *data_ptr = &( node_ptr->data );

    if( ++gpl1_gprs_serving_cell_data_ptr->element >= GPL1_EGPRS_QUAL_MEAS_QUEUE_SIZE )
    {
        gpl1_gprs_serving_cell_data_ptr->element -= GPL1_EGPRS_QUAL_MEAS_QUEUE_SIZE;
    }

    /* Calculation assumes that a decoding of
     * block N is always reported after block N
     * and during block N + 1.
     */
    data_ptr->timeslot = timeslot;

    /* calculate polled blks first abs fn (N) */
    data_ptr->rrbp_fn  = SUB_FN( current_fn,
                                 GPL1_EGPRS_RRBP_FN_REDUCTION[ current_fn_mod13 ] );

    /* Calculate the tx fn based on the decoded RRBP
     * value from the MAC header.
     */
    switch( rrbp )
    {
    case  0:

      tx_fn = ADD_FN( data_ptr->rrbp_fn, RRBP_0_MIN_FRAMES_TO_WAIT );

      break;

    case  1:

      tx_fn = ADD_FN( data_ptr->rrbp_fn, RRBP_1_MIN_FRAMES_TO_WAIT );

      /* Align on radio block boundary if needs be */
      switch( tx_fn % MOD13_FN_COUNTER )
      {
      case 3:
      case 7:
      case 12:
        tx_fn = ADD_FN( tx_fn, 1 );
        break;

      default:
        break;
      }


      break;

    case  2:

      tx_fn = ADD_FN( data_ptr->rrbp_fn, RRBP_2_MIN_FRAMES_TO_WAIT );

      /* Align on radio block boundary if needs be */
      switch( tx_fn % MOD13_FN_COUNTER )
      {
      case 3:
      case 7:
      case 12:
        tx_fn = ADD_FN( tx_fn, 1 );
        break;

      default:
        break;
      }

      break;

    case  3:

      tx_fn = ADD_FN( data_ptr->rrbp_fn, RRBP_3_MIN_FRAMES_TO_WAIT );

      break;

    default:
      MSG_GERAN_ERROR_1_G("invalid rrbp value %d ", rrbp);
      break;
    }

    if(tx_fn == L1_GPRS_INVALID_FN)
    {
      MSG_GERAN_ERROR_0_G("RRBP Invalid tx_fn is not setup");
      return;
    }

    /* This is effectively the number of frames the EPDAN request
     * will be generated relative to the actual RRBP transmission.
     */
    tx_fn = SUB_FN( tx_fn, 4 );

    /* OG Changes to generate the EPDAN REQ as late as possible */
    node_ptr->fn_countdown = (uint8)SUB_FN( tx_fn, current_fn );

    data_ptr->rrbp     = rrbp;
    data_ptr->es_p     = es_p;
    data_ptr->bsn1     = bsn1;

    /* Attach node to the tail of the queue */
    if( FALSE != IS_QUEUE_EMPTY( gpl1_gprs_serving_cell_data_ptr->head_ptr ) )
    {
        /* Attach a node to the empty queue,
         * special handling for head and tail
         * pointers required.
         */
        gpl1_gprs_serving_cell_data_ptr->head_ptr = node_ptr;
        gpl1_gprs_serving_cell_data_ptr->tail_ptr = node_ptr;
        node_ptr->next_ptr = NULL;
    }
    else
    {
        gpl1_egprs_add_node( &node_ptr, &gpl1_gprs_serving_cell_data_ptr->head_ptr, &gpl1_gprs_serving_cell_data_ptr->tail_ptr);
    }
}

/*===========================================================================

FUNCTION  gpl1_egprs_search_qual_meas_queue( )

DESCRIPTION
  This function is called every TDMA frame during EGPRS.
  It searches the queue for nodes due to be reported.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  gpl1_egprs_search_qual_meas_queue( uint8  dl_timeslot_alloc, gas_id_t gas_id )
{
  gpl1_gprs_serving_cell_data_t *gpl1_gprs_serving_cell_data_ptr = &gpl1_gprs_serving_cell_data[gas_id];
    /* start at head of queue */
    gpl1_egprs_qual_meas_queue_T *node_ptr         = gpl1_gprs_serving_cell_data_ptr->head_ptr;
    boolean                       multislot_report = FALSE;

    /* Search the queue for any nodes which are
     * due now to be converted to quality reports.
     */
    while( node_ptr != NULL )
    {
        if( UNLINK_NODE_FN == node_ptr->fn_countdown )
        {


            /* Allocate memory for the rlc msg, assign pointer */
            pl1_rlc_msg_t *rlc_msg_ptr = grlc_get_cmd_buf_pl1(gas_id);

            /* Allocate memory for Quality report msg, assign pointer */
            egprs_chn_qual_rpt_t *qual_report_msg_ptr = &( gpl1_gprs_serving_cell_data_ptr->qual_report[ gpl1_gprs_serving_cell_data_ptr->qual_report_index ] );

            /* Initialise the memory handed in to the report function
             * to avoid unpredictable long term periodic behaviour
             */
            *qual_report_msg_ptr = qual_report_msg_reset;

            if( ++gpl1_gprs_serving_cell_data_ptr->qual_report_index >= GPL1_EGPRS_QUAL_MEAS_QUEUE_SIZE )
            {
                gpl1_gprs_serving_cell_data_ptr->qual_report_index -= GPL1_EGPRS_QUAL_MEAS_QUEUE_SIZE;
            }

            if( FALSE == multislot_report )
            {
                /* Channel request variable to be filled by MAC */
                boolean channel_req_present;

                /* Generate the report only once for this frame */

                /* Fill in the quality report message */
                gpl1_egprs_serv_cell_qual_meas_report( qual_report_msg_ptr,
                                                         dl_timeslot_alloc, gas_id );

                /* Size query the MAC layer */
                new_gmac_trans_calc_e_pdan_size( qual_report_msg_ptr,
                                                 &( gpl1_gprs_serving_cell_data_ptr->size_with ),
                                                 &( gpl1_gprs_serving_cell_data_ptr->size_without ),
                                                 &( channel_req_present ),
                                                 gas_id );

                rlc_msg_ptr->avail_len_no_qrep = gpl1_gprs_serving_cell_data_ptr->size_without;
                rlc_msg_ptr->avail_len_qrep    = gpl1_gprs_serving_cell_data_ptr->size_with;

                /* Setup channel request present flag */
                rlc_msg_ptr->include_chan_req = channel_req_present;

                /* Take a copy of the generated quality report */
                gpl1_gprs_serving_cell_data_ptr->copy_of_qual_report_msg = *qual_report_msg_ptr;

                /* Set the flag to TRUE so that qual report
                 * is not un-necessarly generated from 1st
                 * principles again during this TDMA frame.
                 */
                multislot_report = TRUE;
            }
            else
            {
                /* Fill in the allocated dynamic memory with the stored
                 * copy of the quality report generated for previous
                 * nodes in this TDMA frame.
                 * Warning: This is a copy operation and not a pointer
                 * assignment.
                 */
                *qual_report_msg_ptr = gpl1_gprs_serving_cell_data_ptr->copy_of_qual_report_msg;

                rlc_msg_ptr->avail_len_no_qrep = gpl1_gprs_serving_cell_data_ptr->size_without;
                rlc_msg_ptr->avail_len_qrep    = gpl1_gprs_serving_cell_data_ptr->size_with;

                /* Fill in the chan request as FALSE */
                rlc_msg_ptr->include_chan_req = FALSE;
            }

            /* Fill out the rlc message */

            /* The link parameter inside this msg is for rlc
             * internal queue management and hence need not
             * be filled in by L1.
             */
            rlc_msg_ptr->sig  = PR_DL_EPDAN_REQ;

            rlc_msg_ptr->frame_no = node_ptr->data.rrbp_fn;
            rlc_msg_ptr->timeslot = (uint8) (node_ptr->data.timeslot);
            rlc_msg_ptr->rrbp     = node_ptr->data.rrbp;
            rlc_msg_ptr->esp      = node_ptr->data.es_p;
            rlc_msg_ptr->bsn      = node_ptr->data.bsn1;
            rlc_msg_ptr->ms_out_of_mem = gpl1_get_egprs_ir_mem_status(gas_id);

            rlc_msg_ptr->qrep_ptr = qual_report_msg_ptr;

            /* Send msg to the rlc layer */
            grlc_put_cmd_pl1( gas_id, rlc_msg_ptr );

            MSG_GERAN_HIGH_3_G("DL_EPDAN_REQ sent rb fn %d out of mem %d [%d] ",rlc_msg_ptr->frame_no,rlc_msg_ptr->ms_out_of_mem,gl1_get_FN(gas_id));

            /* Remove the node off the head of the queue
             * The node removal routine makes sure the
             * node ptr is re positioned to point to the
             * next node.
             * i.e. no need to do
             * node_ptr = node_ptr->next_ptr
             */
            gpl1_egprs_remove_node( &node_ptr,
                                    &gpl1_gprs_serving_cell_data_ptr->head_ptr,
                                    &gpl1_gprs_serving_cell_data_ptr->tail_ptr );
        }
        else
        {
            (node_ptr->fn_countdown)--;

            /* Look in the next node */
            node_ptr = node_ptr->next_ptr;
        }
    } /* End of while node_ptr != NULL */
}

/*===========================================================================

FUNCTION  gpl1_egprs_empty_qual_meas_queue( )

DESCRIPTION
  This function is used to perform a clean up on the queued quality
  measurement nodes. It resets the head and tail of the queue.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  gpl1_egprs_empty_qual_meas_queue( gas_id_t gas_id )
{
  gpl1_gprs_serving_cell_data_t *gpl1_gprs_serving_cell_data_ptr = &gpl1_gprs_serving_cell_data[gas_id];
    /* Dont check whether or not the pointers are set
     * It is quicker to just reset them.
     */
    gpl1_gprs_serving_cell_data_ptr->head_ptr = NULL;
    gpl1_gprs_serving_cell_data_ptr->tail_ptr = NULL;
}


/*===========================================================================

FUNCTION  gpl1_gprs_serving_cell

DESCRIPTION
  This function manages the serving cell measurements. It will be run from the
  isr and will run every frame tick in gprs 52 idle and transfer.

DEPENDENCIES
  mode

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean gpl1_gprs_serving_cell_meas(l1_mode_command_T  mode, gas_id_t gas_id)
{
  gprs_l1_idle_data_t *gprs_l1_idle_data = gl1_ms_switch_gprs_idle_data(gas_id);
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  int8              i;
  boolean           set_int_dllink_start = FALSE;
  uint32            qrt;

  /*  Get frame number  */
  uint32 FN = gl1_get_FN(gas_id);

  /*  Setup a local pointer to the serving cell measurements  */
  gprs_pl1_serving_cell_meas_T *gpl1_sc = gprs_serv_cell_meas_ptr;

  /*  Setup a local pointer to the idle data parameters.  */
  pccch_power_control_params_t  *gpl1_pc = &gprs_l1_idle_data->gprs_campedon_cell_data.power_control;

  /*  Assign pointer  */
  transfer_data_T *l1_transfer_data_ptr = &l1_transfer_data[gas_id];

  switch (gpl1_sc->state)
  {
    case  SCM_INIT:
    {
      /*  Initialize serving cell measurement parameters  */
      gpl1_gprs_serv_cell_init( gpl1_sc, gas_id );

      /*  Change state to Running */
      gpl1_sc->state = SCM_RUNNING;

#ifdef FEATURE_GSM_DTM
      if ( l1_tskisr_blk->l1_state == L1_DTM_MODE )
      {
        gpl1_sc->state = SCM_DTM_RUNNING;
      }
#endif

    }
    break;

    case  SCM_RUNNING:
    {

#ifdef FEATURE_GSM_DTM
      if ( l1_tskisr_blk->l1_state == L1_DTM_MODE )
      {
        /* If the ISR is in the DTM state then move to the DTM scell meas handler */
        gpl1_sc->state = SCM_DTM_RUNNING;
        gpl1_sc->c_value_burst_count = 0;
      }
#endif
      switch( l1_tskisr_blk->l1_state )
      {
        case L1_TRANSFER_MODE:
        case L1_DTM_MODE:
  
        /* Generate the bler printout once every 480 ms */
        if( frame_counters[gas_id].FNmod104 == 0 )
        {
          gpl1_calc_gprs_egprs_dl_bler( l1_transfer_data_ptr->egprs_or_gprs_tbf, gas_id );
        }
          break;
        default:
          break;
      }

      /*  Serving cell measurement requirements for both Idle and Transfer mode are
        almost the same, one exception being the power control measurements */

      /*  RUNNING AVERAGE MEASUREMENTS  */
      /*  ****************************************************************** */
      /*  Determine if the RLA_P reporting period has been reached, and if so send
          information to GRR. */
      /*  Send signal to task to initiate sending of MPH_SERVING_MEAS_IND
          message to GRR, parameters required in message have been collected
          by the metrics callback */
      if( ( !(  ( l1_tskisr_blk->l1_state == L1_TRANSFER_MODE )
              ||( l1_tskisr_blk->l1_state == L1_RANDOM_ACCESS_MODE )
              ||( l1_tskisr_blk->l1_state == L1_PACKET_ACCESS_MODE )
             )
           &&( gpl1_sc->rla_p_count >= gpl1_sc->rla_p_count_idle )
          )
        ||(  ( l1_tskisr_blk->l1_state == L1_TRANSFER_MODE )
           &&( gpl1_sc->rla_p_count >= gpl1_sc->rla_p_count_transfer )
          )
        )
      {
          /*  Average the rla_p measurements  */
        gpl1_sc->rla_p_isr_avg = l1_running_avg_get_avg(&gprs_serv_cell_meas_ptr->rla_p_buffer);
#if defined(FEATURE_GSM_RX_DIVERSITY_IDLE) || defined(FEATURE_GSM_RX_DIVERSITY_DATA) 

        if (gl1_get_IMRD_ctl_nv(gas_id) 
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
            || gl1_get_MSRD_ctl_nv(gas_id)
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA*/
            ) 
        {
          gpl1_sc->rla_p_isr_avg_divrx = l1_running_avg_get_divrx_avg(&gprs_serv_cell_meas_ptr->rla_p_buffer);
        }

#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE || FEATURE_GSM_RX_DIVERSITY_DATA*/
        MSG_GERAN_LOW_3_G("RLA_P_AVG %d %d %d",gpl1_sc->rla_p_isr_avg, FN, l1_tskisr_blk->l1_state);

        /*  Reset rla_p counter */
        gpl1_sc->rla_p_count = 0;

        /*  Set idle count to report every paging block after the initial ramp up of the
            filter averaging at idle start  */
        gpl1_sc->rla_p_count_idle = GPRS_SC_RLA_P_COUNT_IDLE_CONT;

        /*  Set transfer count to report every paging block after the initial ramp up of the
            filter averaging at transfer start  */
        gpl1_sc->rla_p_count_transfer = GPRS_SC_RLA_P_COUNT_TRANSFER_CONT;

        gpl1_sc->task_event |= RLA_P_REPORT_TIME;
      }

      /*  POWER CONTROL MEASUREMENTS  */
      /*  ****************************************************************** */
      /*  Determine whether the mode is TRANSFER and PC CHAN MEAS indicates BCCH  */
      if( ( L1_TRANSFER_MODE == l1_tskisr_blk->l1_state )
        &&( PC_MEAS_CHAN_ON_BCCH == gpl1_pc->pc_meas_chan ) )
      {
        /*  Transfer mode and power control measurements on BCCH. So required
            to schedule at least 6 serving cell BCCH measurements every 52 TDMA
            multiframes. This requirement is met by the neighbour cell monitoring
            function which will ensure 2 out of every 11 monitors are on the
            serving cell. The counter checked here is incremented everytime a
            monitor is added. */

        /*  Check the count and signal task if necessary. */
        if (((gpl1_sc->c_value_burst_count)++ >= BCCH_PWR_CNTRL_TIMER) &&
                  (gpl1_sc->power_cntrl_count))
        {
          /*  Sufficient samples have been taken to calculate a Cn value */
          /*  Protect from divide by zero */
          gpl1_sc->power_cntrl_avg = (int16)(gpl1_sc->power_cntrl_samples /
                                                      gpl1_sc->power_cntrl_count);
          /*  Reset intermediate values */
          gpl1_sc->power_cntrl_samples = 0;
          gpl1_sc->power_cntrl_count = 0;

          if( TBF_MODE_EGPRS == l1_transfer_data_ptr->egprs_or_gprs_tbf )
          {
              gpl1_egprs_process_c_value( gpl1_sc, gas_id );
          }
          /* Now signal task to trigger sending of c_value to upper layers  */
          /*  Add appropriate bit to task event register  */
          gpl1_sc->task_event |= PROCESS_C_VALUE;
        }
      }

      /*  QUALITY REPORT TIME */
      /*  ****************************************************************** */
      /*  If frame number matches the report quality measurement time to send to
          MAC then setup task event control word. As more than one quality report
          can be pending, search the whole array for possible matches.  */
      for (i=0; i<MAX_NUM_QUAL_RPRTS; i++)
      {
        qrt = gpl1_sc->quality_report_time[i];

        if (qrt != L1_GPRS_INVALID_FN)
        {
          if (gpl1_gprs_adjust_mod_num(qrt, -(int32)FN, FRAMES_IN_HYPERFRAME) < TWO_BLOCKS_BEFORE_QR)
          {
            /*  Reset report time to INVALID so no spurious reports triggered.  */
            gpl1_sc->qrt = qrt;
            gpl1_sc->quality_report_time[i] = L1_GPRS_INVALID_FN;
            gpl1_sc->quality_report_count++;

            MSG_GERAN_LOW_2_G("QRC %d %d",gpl1_sc->quality_report_count, FN);

            /*  Time to send quality report to MAC, let task know.  */
            gpl1_sc->task_event |= QUALITY_REPORT_TIME;

          }
        }
      }

      /* Interference Measurements */
      switch( frame_counters[gas_id].FNmod13 )
      {
      case  11:
          set_int_dllink_start = gpl1_interf_measurements( l1_transfer_data_ptr,
                                                           gpl1_sc,
                                                           mode,
                                                           FN, gas_id );
          break;
      default:
          break;
      }

      if( ( L1_TRANSFER_MODE == l1_tskisr_blk->l1_state )
        &&( TBF_MODE_EGPRS == l1_transfer_data_ptr->egprs_or_gprs_tbf ) )
      {
        /* Due to potential quality report time constraints while in EGPRS,
         * the report is generated from the isr.
         * We dont want it to happen in the Task.
         */
        gpl1_sc->task_event &= (~QUALITY_REPORT_TIME);

        /* rxqual and sign var are replaced by mean bep and cv bep
         * Do not bother calculating them in the task.
         */
        gpl1_sc->task_event &= (~PROCESS_RXQUAL_SIGN_VAR);

        /* C value processing is done synchronously in egprs */
        gpl1_sc->task_event &= (~PROCESS_C_VALUE);
      }

      /*  If any task events have been set signal the task  */
      if (gpl1_sc->task_event)
      {
        /*  Now signal task, when task receives this signal it will look in the
            task_event to determine what actions to take  */
#ifdef PL1_FW_SIM
        /* set test harness signal */
        (void)rex_set_sigs(&tcb_pl1, SERV_MEAS_ISR_SIG);
#else
        (void)rex_set_sigs(l1_task_tcb_read(gas_id), SERV_MEAS_ISR_SIG);
#endif

      }
    } /*  End of SCM_RUNNING case */
    break;


#ifdef FEATURE_GSM_DTM
    case SCM_DTM_RUNNING:
      {

        if ( l1_tskisr_blk->l1_state != L1_DTM_MODE )
        {
          /* If the ISR has left the DTM state then move back to the INIT state */
          gpl1_sc->state = SCM_INIT;
        }

        switch( l1_tskisr_blk->l1_state )
        {
          case L1_DTM_MODE:
          /* Generate the bler printout once every 480 ms */
          if( frame_counters[gas_id].FNmod104 == 0 )
          {
            gpl1_calc_gprs_egprs_dl_bler( l1_transfer_data_ptr->egprs_or_gprs_tbf, gas_id );
          }
            break;
          default:
            break;
        }


        /*  POWER CONTROL MEASUREMENTS  */
        /*  ****************************************************************** */
        /*  Determine whether the mode is TRANSFER and PC CHAN MEAS indicates BCCH  */
        if( PC_MEAS_CHAN_ON_BCCH == gpl1_pc->pc_meas_chan )
        {

          /***************************************************************/
          /* 05.08 - 10.1.1.2                                            */
          /* At least 5 rx signal level measurements are required for a  */
          /* valid RLA_P value. Use the c_value_burst_count as a flag to */
          /* indicate that this criterion has been met. This will then   */
          /* stay set until the mode changes.                            */
          /***************************************************************/
          if (gpl1_sc->power_cntrl_count >= BCCH_PWR_CNTRL_TIMER)
          {
            gpl1_sc->c_value_burst_count = BCCH_PWR_CNTRL_TIMER;
          }

          /* Check the count and signal task if necessary. */
          if ((gpl1_sc->c_value_burst_count >= BCCH_PWR_CNTRL_TIMER) &&
              (gpl1_sc->power_cntrl_count))
          {
            /*  Sufficient samples have been taken to calculate a Cn value */
            /*  Protect from divide by zero */
            gpl1_sc->power_cntrl_avg =
              (int16)(gpl1_sc->power_cntrl_samples / gpl1_sc->power_cntrl_count);

            /*  Reset intermediate values */
            gpl1_sc->power_cntrl_samples = 0;
            gpl1_sc->power_cntrl_count = 0;

            if( TBF_MODE_EGPRS == l1_transfer_data_ptr->egprs_or_gprs_tbf )
            {
              gpl1_egprs_process_c_value( gpl1_sc, gas_id );
            }
            /* Now signal task to trigger sending of c_value to upper layers  */
            /*  Add appropriate bit to task event register  */
            gpl1_sc->task_event |= PROCESS_C_VALUE;
          }
        }

        /*  QUALITY REPORT TIME */
        /*  ****************************************************************** */
        /*  If frame number matches the report quality measurement time to send to
            MAC then setup task event control word. As more than one quality report
            can be pending, search the whole array for possible matches.  */
        for (i=0; i<MAX_NUM_QUAL_RPRTS; i++)
        {
          qrt = gpl1_sc->quality_report_time[i];

          if (qrt != L1_GPRS_INVALID_FN)
          {
            if (gpl1_gprs_adjust_mod_num(qrt, -(int32)FN, FRAMES_IN_HYPERFRAME) < TWO_BLOCKS_BEFORE_QR)
            {
              /*  Reset report time to INVALID so no spurious reports triggered.  */
              gpl1_sc->qrt = qrt;
              gpl1_sc->quality_report_time[i] = L1_GPRS_INVALID_FN;
              gpl1_sc->quality_report_count++;

              MSG_GERAN_LOW_2_G("QRC %d %d",gpl1_sc->quality_report_count, FN);

              /*  Time to send quality report to MAC, let task know.  */
              gpl1_sc->task_event |= QUALITY_REPORT_TIME;

            }
          }
        }

        /*  If any task events have been set signal the task  */
        if (gpl1_sc->task_event)
        {
          /*  Now signal task, when task receives this signal it will look in the
              task_event to determine what actions to take  */
#ifdef PL1_FW_SIM
          /* set test harness signal */
          (void)rex_set_sigs(&tcb_pl1, SERV_MEAS_ISR_SIG);
#else
          (void)rex_set_sigs(l1_task_tcb_read(gas_id), SERV_MEAS_ISR_SIG);
#endif

        }
      }
    break;
#endif /* FEATURE_GSM_DTM */

    default:
    break;
  } /*  End of SCM state switch */

  return  set_int_dllink_start;

}

/*===========================================================================

FUNCTION  gpl1_scell_cs_idle_metrics_cb

DESCRIPTION
  This function allows burst metrics to be passed to serving cell
  measurements module.

  It used in circuit switched Idle/access Modes on the ccch logical channel.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  gpl1_scell_cs_idle_metrics_cb( uint8    burst_num,
                                     dBx16_T  rssi, 
#if defined(FEATURE_GSM_RX_DIVERSITY_IDLE) || defined(FEATURE_GSM_RX_DIVERSITY_DATA)
                                     dBx16_T  divrx_rssi,
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE || FEATURE_GSM_RX_DIVERSITY_DATA*/
                                     gl1_power_meas_type pwr_meas_type,
                                     gas_id_t gas_id )
{
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
  /* local pointer to the serving cell measurements */
  gprs_pl1_serving_cell_meas_T *scell_meas_ptr = gprs_serv_cell_meas_ptr;

  if( NULL != scell_meas_ptr->rssi_buffer_ptr )
  {
      if( L1_BURST_1 == burst_num )
      {
          /* Move to the next buffer. */
          if( &(scell_meas_ptr->block_rssi[(NUM_RSSI_BUFFERS - 1)])
              == scell_meas_ptr->rssi_buffer_ptr )
          {
              scell_meas_ptr->rssi_buffer_ptr =
                  &(scell_meas_ptr->block_rssi[BUFFER_1]);
          }
          else
          {
              scell_meas_ptr->rssi_buffer_ptr++;
          }

          /* Reset the rssi buffer content */
          scell_meas_ptr->rssi_buffer_ptr->num_on_bcch      = 0;
          scell_meas_ptr->rssi_buffer_ptr->accum_block_rssi = 0;
          scell_meas_ptr->rssi_buffer_ptr->num_bursts       = 0;
#if defined(FEATURE_GSM_RX_DIVERSITY_IDLE) || defined(FEATURE_GSM_RX_DIVERSITY_DATA) 
          scell_meas_ptr->rssi_buffer_ptr->accum_block_rssi_divrx = 0;
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE || FEATURE_GSM_RX_DIVERSITY_DATA*/
      }

      switch (pwr_meas_type) 
      {
        case PRX_DIVRX_MEAS:
#if defined(FEATURE_GSM_RX_DIVERSITY_IDLE) || defined(FEATURE_GSM_RX_DIVERSITY_DATA) 
        /* Accumulate the rssi measurement and increment the measurements count */
          scell_meas_ptr->rssi_buffer_ptr->accum_block_rssi_divrx += divrx_rssi;

          /* Store burst signal strength */
          scell_meas_ptr->rssi_buffer_ptr->burst_ss_divrx[ burst_num ] = divrx_rssi;
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE || FEATURE_GSM_RX_DIVERSITY_DATA*/

        case PRX_MEAS:
          /* Accumulate the rssi measurement and increment the measurements count */
          scell_meas_ptr->rssi_buffer_ptr->accum_block_rssi += rssi;

          /* Store burst signal strength */
          scell_meas_ptr->rssi_buffer_ptr->burst_ss[ burst_num ] = rssi;
          break;

        default:
          MSG_GERAN_ERROR_1_G("Incorrect measurement type %d", pwr_meas_type);
      }

      /* Update the last burst received id */
      scell_meas_ptr->rssi_buffer_ptr->last_burst_received = burst_num;

      /* Increment the number of bursts received */
      scell_meas_ptr->rssi_buffer_ptr->num_bursts++;
  } /* End of if rssi_buffer_ptr is initialised */
  else
  {
      MSG_GERAN_ERROR_0_G(" de-referencing an un-initialised rssi_buffer_ptr ");
  } /* End of else rssi_buffer_ptr is not initialised */
}

/*===========================================================================

FUNCTION  gpl1_scell_pkt_idle_metrics_cb

DESCRIPTION
  This function allows burst metrics to be passed to serving cell
  measurements module.

  It used in Packet Idle Mode.
  Redundant function. need to be removed during the cleanup.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  gpl1_scell_pkt_idle_metrics_cb( uint8    burst_num,
                                      dBx16_T  rssi, gas_id_t gas_id )
{
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
  /* local pointer to the serving cell measurements */
  gprs_pl1_serving_cell_meas_T *scell_meas_ptr = gprs_serv_cell_meas_ptr;

  if( L1_BURST_1 == burst_num )
  {
      /* Move to the next buffer. */
      if( &(scell_meas_ptr->block_rssi[(NUM_RSSI_BUFFERS - 1)])
          == scell_meas_ptr->rssi_buffer_ptr )
      {
          scell_meas_ptr->rssi_buffer_ptr =
              &(scell_meas_ptr->block_rssi[BUFFER_1]);
      }
      else
      {
          scell_meas_ptr->rssi_buffer_ptr++;
      }

      /* Reset the rssi buffer content */
      scell_meas_ptr->rssi_buffer_ptr->num_on_bcch      = 0;
      scell_meas_ptr->rssi_buffer_ptr->accum_block_rssi = 0;
  }

  /* Accumulate the rssi measurement and increment the measurements count */
  scell_meas_ptr->rssi_buffer_ptr->accum_block_rssi += rssi;

  /* Store burst signal strength */
  scell_meas_ptr->rssi_buffer_ptr->burst_ss[ burst_num ] = rssi;

  /* Update the last burst received id */
  scell_meas_ptr->rssi_buffer_ptr->last_burst_received = burst_num;
}

/*===========================================================================

FUNCTION  gpl1_scell_pkt_access_metrics_cb

DESCRIPTION
  This function allows burst metrics to be passed to serving cell
  measurements module.

  It used in Packet Access Mode.
  Redundant function. need to be removed during the cleanup.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  gpl1_scell_pkt_access_metrics_cb( uint8    burst_num,
                                        dBx16_T  rssi, gas_id_t gas_id )
{
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
  /* local pointer to the serving cell measurements */
  gprs_pl1_serving_cell_meas_T *scell_meas_ptr = gprs_serv_cell_meas_ptr;

  if( L1_BURST_1 == burst_num )
  {
      /* Move to the next buffer. */
      if( &(scell_meas_ptr->block_rssi[(NUM_RSSI_BUFFERS - 1)])
          == scell_meas_ptr->rssi_buffer_ptr )
      {
          scell_meas_ptr->rssi_buffer_ptr =
              &(scell_meas_ptr->block_rssi[BUFFER_1]);
      }
      else
      {
          scell_meas_ptr->rssi_buffer_ptr++;
      }

      /* Reset the rssi buffer content */
      scell_meas_ptr->rssi_buffer_ptr->num_on_bcch      = 0;
      scell_meas_ptr->rssi_buffer_ptr->accum_block_rssi = 0;
  }

  /* Accumulate the rssi measurement and increment the measurements count */
  scell_meas_ptr->rssi_buffer_ptr->accum_block_rssi += rssi;

  /*  Store burst signal strength */
  scell_meas_ptr->rssi_buffer_ptr->burst_ss[ burst_num ] = rssi;

  /* Update the last burst received id */
  scell_meas_ptr->rssi_buffer_ptr->last_burst_received = burst_num;
}

/*===========================================================================

FUNCTION  gpl1_scell_pkt_transfer_metrics_cb

DESCRIPTION
  This function allows burst metrics to be passed to serving cell
  measurements module ( 1 PDCH only ).

  It used in Packet Transfer Mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  gpl1_scell_pkt_transfer_metrics_cb( uint16   arfcn_number,
                                          uint8    burst_num,
                                          dBx16_T  rssi,
#if defined(FEATURE_GSM_RX_DIVERSITY_IDLE) || defined(FEATURE_GSM_RX_DIVERSITY_DATA) 
                                          dBx16_T  divrx_rssi,
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE || FEATURE_GSM_RX_DIVERSITY_DATA*/
                                          gl1_power_meas_type pwr_meas_type,
                                          gas_id_t gas_id )
{
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
  /* local pointer to the serving cell measurements */
  gprs_pl1_serving_cell_meas_T *scell_meas_ptr = gprs_serv_cell_meas_ptr;

  if( L1_BURST_1 == burst_num )
  {
      /* Move to the next buffer. */
      if( &(scell_meas_ptr->block_rssi[(NUM_RSSI_BUFFERS - 1)])
          == scell_meas_ptr->rssi_buffer_ptr )
      {
          scell_meas_ptr->rssi_buffer_ptr =
              &(scell_meas_ptr->block_rssi[BUFFER_1]);
      }
      else
      {
          scell_meas_ptr->rssi_buffer_ptr++;
      }

      /* Reset the rssi buffer content */
      scell_meas_ptr->rssi_buffer_ptr->num_on_bcch      = 0;
      scell_meas_ptr->rssi_buffer_ptr->accum_block_rssi = 0;
#if defined(FEATURE_GSM_RX_DIVERSITY_IDLE) || defined(FEATURE_GSM_RX_DIVERSITY_DATA) 
      scell_meas_ptr->rssi_buffer_ptr->accum_block_rssi_divrx = 0;
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE || FEATURE_GSM_RX_DIVERSITY_DATA*/
  }

  switch (pwr_meas_type) 
  {
    case PRX_DIVRX_MEAS:
#if defined(FEATURE_GSM_RX_DIVERSITY_IDLE) || defined(FEATURE_GSM_RX_DIVERSITY_DATA) 
      /* Accumulate the rssi measurement and increment the measurements count */
      scell_meas_ptr->rssi_buffer_ptr->accum_block_rssi_divrx += divrx_rssi;

      /* Store burst signal strength */
      scell_meas_ptr->rssi_buffer_ptr->burst_ss_divrx[ burst_num ] = divrx_rssi;
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE || FEATURE_GSM_RX_DIVERSITY_DATA*/

    case PRX_MEAS:
      /* Accumulate the rssi measurement and increment the measurements count */
      scell_meas_ptr->rssi_buffer_ptr->accum_block_rssi += rssi;

      /*  Store burst signal strength */
      scell_meas_ptr->rssi_buffer_ptr->burst_ss[ burst_num ] = rssi;
      break;

    default:
      MSG_GERAN_ERROR_1_G("Incorrect measurement type %d", pwr_meas_type);
  }


  /* Flag the burst if it was received on the Beacon arfcn */
  if( arfcn_number == l1_idle_data->campedon_cell_ARFCN.num )
  {
    scell_meas_ptr->rssi_buffer_ptr->
                    burst_on_bcch[ burst_num ] = TRUE;

    scell_meas_ptr->rssi_buffer_ptr->num_on_bcch++;
  }
  else
  {
    scell_meas_ptr->rssi_buffer_ptr->
                    burst_on_bcch[ burst_num ] = FALSE;
  }

  /* Update the last burst received id */
  scell_meas_ptr->rssi_buffer_ptr->last_burst_received = burst_num;
}


/*===========================================================================

FUNCTION  gpl1_egprs_scell_interf_meas_config( )

DESCRIPTION
  This function is called to inform the interference measurements module
  of the egprs link quality measurement mode so that the decision of whether
  to measure or not is taken.

DEPENDENCIES
  Used only during EDGE.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  gpl1_egprs_scell_interf_meas_config( uint8  link_qual_meas_mode, gas_id_t gas_id )
{
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
    /* Configure the interference measurements module
     * depending on the value of the link quality meas
     * mode:
     * 0  NO_MEASUREMENTS
     * 1  INTERF_MEASUREMENTS
     * 2  BEP_MEASUREMENTS
     * 3  BOTH_MEASUREMENTS
     * 4  INVALID_MEAS_MODE
     */
    gprs_serv_cell_meas_ptr->egprs_interf_meas_required =
        EGPRS_PL1_INTERF_MEAS_MODE[ link_qual_meas_mode ];
}
/*===========================================================================

FUNCTION  gpl1_gprs_scell_disable_int_meas( )

DESCRIPTION
  This function is called to inform serving cell interference measurements
  tasking that a certain timeslot is being used for uplink so it can avoid
  it.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  gpl1_gprs_scell_disable_int_meas( gl1_defs_tn_type  ul_timeslot, gas_id_t gas_id )
{
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
  /* Mark this timeslot as being used for
   * transmission so we can avoid it
   * while doing interf meas.
   */
  gprs_serv_cell_meas_ptr->tx_in_progress[ ul_timeslot ] = TRUE;
}

/*===========================================================================

FUNCTION  gpl1_gprs_serv_cell_int_cb( )

DESCRIPTION
  This function is called by NPL1 when scheduled interference measurements
  have been completed.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  gpl1_gprs_serv_cell_int_cb( gl1_defs_int_meas_result_type *int_meas_results, gas_id_t gas_id )
{
  gprs_l1_idle_data_t *gprs_l1_idle_data = gl1_ms_switch_gprs_idle_data(gas_id);
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
  uint8   i;
  int32   n_avg_i;

  pccch_power_control_params_t *gpl1_pc_ptr =
      &( gprs_l1_idle_data->gprs_campedon_cell_data.power_control );

  gprs_pl1_serving_cell_meas_T* gpl1_sc_ptr =gprs_serv_cell_meas_ptr;

  if( ( L1_TRANSFER_MODE == l1_tskisr_blk->l1_state )
    &&( TBF_MODE_EGPRS == l1_transfer_data[gas_id].egprs_or_gprs_tbf ) )
  {
      if( FALSE == gpl1_sc_ptr->egprs_interf_meas_required )
      {

#if  DEBUG_GPL1_INTERF_MEAS
          #error code not present
#endif

          return;
      }
  }

  /*  N_avg_i parameter is calculated from the 'k' parameter passed to PL1.
      N_avg_i = 2^(k/2), for k = 0, .. 15 */
  /*  Range check k */
  if( gpl1_pc_ptr->n_avg_i >= 15 )
  {
      gpl1_pc_ptr->n_avg_i = 15;
  }

  n_avg_i = (1 << (gpl1_pc_ptr->n_avg_i >> 1));

#if  DEBUG_GPL1_INTERF_MEAS
  #error code not present
#endif

  /*  Process interference measurements */
  for( i=0; i < int_meas_results->num_tn; i++ )
  {
      /*  Define working pointer  */
      interference_T  *index_ptr =
          &( gpl1_sc_ptr->int_array
                          [ gpl1_gprs_ts_conv_unslam( (uint8) int_meas_results->tn[i] ,gas_id) ] );

      /*  Include this sample in the average measurement  */
      index_ptr->ss_chn += int_meas_results->measured_pwr_dBm_x16[i];

      /* A measurement must be done on both idle and ptcch
       * frames before the signal strength filters come into
       * play hence keep a count. This is for the benefit
       * of the interf meas callback function.
       */
      switch( frame_counters[gas_id].FNmod52 )
      {
      case  (12 + 2):
      case  (38 + 2):  /* PTCCH frame */
          index_ptr->ptcch_count++;
          break;

      case  (25 + 2):
      case  (1):  /* ADD_MOD(51,2,52) - IDLE frame */
          index_ptr->idle_count++;
          break;

      default:
          break;
      }

      /*  If enough measurements have been taken, (at least one on the PTCCH
          and one on the IDLE frame) then apply filter.  */
      if(  ( index_ptr->ptcch_count > 0 )
         &&( index_ptr->idle_count  > 0 ) )
      {
        uint16  forgetting_factor;
        int16   temp_pwr;

        /*  Increment sample count  */
        index_ptr->measurement_count++;

        /*  At least one measurement has been taken on each type of frame.
            now average sample before applying filter.  */
        index_ptr->ss_chn /= ((index_ptr->ptcch_count) + (index_ptr->idle_count));

        forgetting_factor = (uint16) ( MIN( (index_ptr->measurement_count),
                                            n_avg_i ) );

        /* Pass through filter */
        temp_pwr = gpl1_gprs_divide_ff( (int16) ( index_ptr->ss_chn ),
                                        &( index_ptr->meas_1 ),
                                        forgetting_factor );

        /* Average all timeslot measurements into single result */
        if( gpl1_sc_ptr->serv_cell_int )
        {
          /* Sum and average */
          gpl1_sc_ptr->serv_cell_int += temp_pwr;
          gpl1_sc_ptr->serv_cell_int >>= 1;
        }
        else
        {
          /* If this is the first measurement then do not average. */
          gpl1_sc_ptr->serv_cell_int += temp_pwr;
        }

        /* Set valid flag to indicate at n_avg_i samples have been passed through
           the filter on this channel and therefore the interference measurement
           is valid for this channel. */
        if( (index_ptr->measurement_count) >= gpl1_pc_ptr->n_avg_i )
        {
          index_ptr->valid_ind = TRUE;
        }

        /* Clear the ptcch and idle frame counts and the frame measurement ss_chn */
        index_ptr->ss_chn      = 0;
        index_ptr->ptcch_count = 0;
        index_ptr->idle_count  = 0;

      } /* End of if idle_count && ptcch are higher than zero */
  }   /* End of for loop */

}
/*===========================================================================

FUNCTION  gpl1_scell_calc_ssblockn

DESCRIPTION
  This function calculates block rssi based upon number of bursts.

DEPENDENCIES
  None

RETURN VALUE
  Block power level.

SIDE EFFECTS
  None

===========================================================================*/
int16     gpl1_scell_calc_ssblockn( int16 accum_block_rssi, 
                                    uint8 num_bursts, 
                                    gas_id_t gas_id )
{
  int16 ssblockn = 0;
  switch(num_bursts )
  {
    case  GL1_FOUR_BURSTS:
      ssblockn = (accum_block_rssi >> DIVIDE_BY_4_SHIFT_VALUE);
      break; 

    case  GL1_THREE_BURSTS:
      ssblockn = (accum_block_rssi /3);
      break;

    case  GL1_TWO_BURSTS:
      ssblockn = (accum_block_rssi >> DIVIDE_BY_2_SHIFT_VALUE);
      break;

    case  GL1_ONE_BURSTS:
      ssblockn = accum_block_rssi;
      break;

    default:
      MSG_GERAN_ERROR_1_G(" invalid num rssi bursts %d", num_bursts);
      break;
  }
  return ssblockn;
}
/*===========================================================================

FUNCTION  gpl1_scell_cs_idle_data_cb

DESCRIPTION
  This function is called from the appropriate block receive data callback.

  It used in circuit switched Idle/access Modes on the ccch logical channel.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  gpl1_scell_cs_idle_data_cb( boolean  decode_ok, gas_id_t gas_id  )
{
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];

  /* local pointer to the scell meas structure */
  gprs_pl1_serving_cell_meas_T *scell_meas_ptr =gprs_serv_cell_meas_ptr;

  boolean rxd_on = FALSE;
#if defined(FEATURE_GSM_RX_DIVERSITY_IDLE) || defined(FEATURE_GSM_RX_DIVERSITY_DATA)
  rxd_on = (gl1_get_IMRD_ctl_nv(gas_id) || gl1_get_MSRD_ctl_nv(gas_id));
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE || FEATURE_GSM_RX_DIVERSITY_DATA*/

  if( NULL != scell_meas_ptr->rssi_buffer_ptr )
  {
    /* local rssi pointer */
    block_rssi_t* rssi_buffer_ptr = scell_meas_ptr->rssi_buffer_ptr;

    if( FALSE != decode_ok )
    {
      scell_meas_ptr->ssblockn = gpl1_scell_calc_ssblockn(rssi_buffer_ptr->accum_block_rssi,
                                                          rssi_buffer_ptr->num_bursts, 
                                                          gas_id);
      if (rxd_on) 
      {
#if defined(FEATURE_GSM_RX_DIVERSITY_IDLE) || defined(FEATURE_GSM_RX_DIVERSITY_DATA)
        scell_meas_ptr->ssblockn_divrx = gpl1_scell_calc_ssblockn(rssi_buffer_ptr->accum_block_rssi_divrx,
                                                                  rssi_buffer_ptr->num_bursts, 
                                                                  gas_id);
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE || FEATURE_GSM_RX_DIVERSITY_DATA*/
      }

      if ((rssi_buffer_ptr->num_bursts < GL1_ONE_BURSTS) 
          || (rssi_buffer_ptr->num_bursts > GL1_FOUR_BURSTS)) 
      {
        /* Reset the buffer rssi content and exit */
        rssi_buffer_ptr->accum_block_rssi = 0;
#if defined(FEATURE_GSM_RX_DIVERSITY_IDLE) || defined(FEATURE_GSM_RX_DIVERSITY_DATA)
        rssi_buffer_ptr->accum_block_rssi_divrx = 0;
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE || FEATURE_GSM_RX_DIVERSITY_DATA*/
      }

      /* Raise flag so that ssblockn is passed into the
       * averaging filter.*/
      if( 0 == scell_meas_ptr->task_event )
      {
        /* Ask the OS for a ISR -> L1 Task context switch
         * inorder to process the C value and Sign Var
         * calculations.
         */
#ifdef PL1_FW_SIM
        (void)rex_set_sigs(&tcb_pl1, SERV_MEAS_ISR_SIG);
#else
        (void)rex_set_sigs(l1_task_tcb_read(gas_id), SERV_MEAS_ISR_SIG);
#endif
      }

      /* If the synchronous serv cell meas function has
         has asked for a context switch already then dont
         signal again. Just OR in the required events.
       */
      scell_meas_ptr->task_event |= PROCESS_C_VALUE;
    } /* End of if block is decoded correctly */

    /* Reset the buffer rssi content */
    rssi_buffer_ptr->accum_block_rssi = 0;

#if defined(FEATURE_GSM_RX_DIVERSITY_IDLE) || defined(FEATURE_GSM_RX_DIVERSITY_DATA)
    rssi_buffer_ptr->accum_block_rssi_divrx = 0;
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE || FEATURE_GSM_RX_DIVERSITY_DATA*/

  } /* End of if rssi_buffer_ptr is initialised */
  else
  {
    MSG_GERAN_ERROR_0_G(" de-referencing an un-initialised rssi_buffer_ptr ");
  } /* End of else rssi_buffer_ptr is not initialised */
}
/*===========================================================================

FUNCTION  gpl1_scell_pkt_idle_data_cb

DESCRIPTION
  This function is called from the appropriate block receive data callback.
  Redundant function. need to be removed during the cleanup.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  gpl1_scell_pkt_idle_data_cb( boolean  decode_ok, gas_id_t gas_id )
{
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
    /* local pointer to the scell meas structure */
    gprs_pl1_serving_cell_meas_T *scell_meas_ptr =gprs_serv_cell_meas_ptr;

    /* local rssi pointer */
    block_rssi_t* rssi_buffer_ptr = scell_meas_ptr->rssi_buffer_ptr;

    if( FALSE != decode_ok )
    {
        scell_meas_ptr->ssblockn =
            (rssi_buffer_ptr->accum_block_rssi) >> DIVIDE_BY_4_SHIFT_VALUE;

        /* Raise flag so that ssblockn is passed into the
         * averaging filter.
         */
        if( 0 == scell_meas_ptr->task_event )
        {
            /* Ask the OS for a ISR -> L1 Task context switch
             * inorder to process the C value and Sign Var
             * calculations.
             */
#ifdef PL1_FW_SIM
            (void)rex_set_sigs(&tcb_pl1, SERV_MEAS_ISR_SIG);
#else
            (void)rex_set_sigs(l1_task_tcb_read(gas_id), SERV_MEAS_ISR_SIG);
#endif
        }

        /* If the synchronous serv cell meas function has
         * has asked for a context switch already then dont
         * signal again. Just OR in the required events.
         */
        scell_meas_ptr->task_event |= PROCESS_C_VALUE;
    }

    /* Reset the buffer rssi content */
    rssi_buffer_ptr->accum_block_rssi = 0;
}

/*===========================================================================

FUNCTION  gpl1_scell_pkt_access_data_cb

DESCRIPTION
  This function is called from the appropriate block receive data callback.
  Redundant function. need to be removed during the cleanup.
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  gpl1_scell_pkt_access_data_cb( boolean  decode_ok, gas_id_t gas_id )
{
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
    /* local pointer to the scell meas structure */
    gprs_pl1_serving_cell_meas_T *scell_meas_ptr =gprs_serv_cell_meas_ptr;

    /* local rssi pointer */
    block_rssi_t* rssi_buffer_ptr = scell_meas_ptr->rssi_buffer_ptr;

    if( FALSE != decode_ok )
    {
        scell_meas_ptr->ssblockn =
            (rssi_buffer_ptr->accum_block_rssi) >> DIVIDE_BY_4_SHIFT_VALUE;

        /* Raise flag so that ssblockn is passed into the
         * averaging filter.
         */
        if( 0 == scell_meas_ptr->task_event )
        {
            /* Ask the OS for a ISR -> L1 Task context switch
             * inorder to process the C value and Sign Var
             * calculations.
             */
#ifdef PL1_FW_SIM
            (void)rex_set_sigs(&tcb_pl1, SERV_MEAS_ISR_SIG);
#else
            (void)rex_set_sigs(l1_task_tcb_read(gas_id), SERV_MEAS_ISR_SIG);
#endif
        }

        /* If the synchronous serv cell meas function has
         * has asked for a context switch already then dont
         * signal again. Just OR in the required events.
         */
        scell_meas_ptr->task_event |= PROCESS_C_VALUE;
    }

    /* Reset the buffer rssi content */
    rssi_buffer_ptr->accum_block_rssi = 0;
}

/*===========================================================================

FUNCTION  gpl1_scell_pkt_transfer_data_cb

DESCRIPTION
  This function is called from the appropriate block receive data callback.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  gpl1_scell_pkt_transfer_data_cb( gl1_defs_tn_type  timeslot,
                                                    const gpl1_gprs_decode_metrics_T*  gprs_decode_metrics_ptr,
                                                    gas_id_t gas_id )
{
    gprs_l1_idle_data_t *gprs_l1_idle_data = gl1_ms_switch_gprs_idle_data(gas_id);
    gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
    volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    /* local pointer to the scell meas structure */
    gprs_pl1_serving_cell_meas_T *scell_meas_ptr =gprs_serv_cell_meas_ptr;

    /* Setup a pointer to the current transfer data parameters */
    transfer_data_T*  transfer_data_ptr =
      l1_tskisr_blk->current_params.L1Data.pTransfer_data;

    /* local rssi pointer */
    block_rssi_t* rssi_buffer_ptr = scell_meas_ptr->rssi_buffer_ptr;

    if( FALSE != gprs_decode_metrics_ptr->header_decode_ok )
    {
        rx_qual_T *scell_rxqual_ptr = &(scell_meas_ptr->rx_qual);

        if( timeslot == (gl1_defs_tn_type) l1_get_current_timeslot(gas_id) )
        {
          gl1_power_meas_type pwr_meas_type = PRX_MEAS;
          int16 ssblockn_divrx = GL1_INVALID_POWER_LEVEL;

#if defined(FEATURE_GSM_RX_DIVERSITY_IDLE) || defined(FEATURE_GSM_RX_DIVERSITY_DATA) 
          /* MSRD is enabled, use MSRD divrx power*/
          /*Idle RxD is enabled then to keep buffer consistent use PRX power*/
          if (gl1_get_MSRD_ctl_nv(gas_id)) 
          {
            scell_meas_ptr->ssblockn_divrx = 
              (rssi_buffer_ptr->accum_block_rssi_divrx) >> DIVIDE_BY_4_SHIFT_VALUE;
            pwr_meas_type = PRX_DIVRX_MEAS;
          }
          else if (gl1_get_IMRD_ctl_nv(gas_id)) 
          {
            scell_meas_ptr->ssblockn_divrx = 
              (rssi_buffer_ptr->accum_block_rssi) >> DIVIDE_BY_4_SHIFT_VALUE;
            pwr_meas_type = PRX_DIVRX_MEAS;
          }
          ssblockn_divrx = scell_meas_ptr->ssblockn_divrx;
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE || FEATURE_GSM_RX_DIVERSITY_DATA*/

            /* Average the rssi only once */
            scell_meas_ptr->ssblockn =
                (rssi_buffer_ptr->accum_block_rssi) >> DIVIDE_BY_4_SHIFT_VALUE;

            if(transfer_data_ptr->l1_test_mode != L1_TEST_MODE_OFF)
            {

             /* Add P0 if downlink Power control is enabled */
              if(transfer_data_ptr->dl_power_control.params_valid == TRUE)
              {
                scell_meas_ptr->ssblockn += (transfer_data_ptr->dl_power_control.po * 32);
#if defined(FEATURE_GSM_RX_DIVERSITY_IDLE) || defined(FEATURE_GSM_RX_DIVERSITY_DATA) 
                if (PRX_DIVRX_MEAS == pwr_meas_type)
                {
                  scell_meas_ptr->ssblockn_divrx += (transfer_data_ptr->dl_power_control.po * 32);
                  ssblockn_divrx = scell_meas_ptr->ssblockn_divrx;
                }
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE || FEATURE_GSM_RX_DIVERSITY_DATA*/
              }

              MSG_GERAN_LOW_1_G("ssblckn %d",scell_meas_ptr->ssblockn);
              l1_running_avg_add_meas( &(gprs_serv_cell_meas_ptr->rla_p_buffer), 
                                       scell_meas_ptr->ssblockn, 
                                       ssblockn_divrx,
                                       pwr_meas_type,
                                       gas_id);
              gprs_serv_cell_meas_ptr->rla_p_count++;
            }

            /* Raise flag so that ssblockn is passed into the
             * averaging filter.
             */
            if( 0 == scell_meas_ptr->task_event )
            {
                /* Ask the OS for a ISR -> L1 Task context switch
                 * inorder to process the C value and Sign Var
                 * calculations.
                 */
#ifdef PL1_FW_SIM
                (void)rex_set_sigs(&tcb_pl1, SERV_MEAS_ISR_SIG);
#else
                (void)rex_set_sigs(l1_task_tcb_read(gas_id), SERV_MEAS_ISR_SIG);
#endif
            }

            /* If the synchronous serv cell meas function has
             * has asked for a context switch already then dont
             * signal again. Just OR in the required events.
             */
            scell_meas_ptr->task_event |= PROCESS_RXQUAL_SIGN_VAR;

            if( PC_MEAS_CHAN_ON_PDCH == gprs_l1_idle_data->gprs_campedon_cell_data
                                                          .power_control.pc_meas_chan )
            {
                scell_meas_ptr->task_event |= PROCESS_C_VALUE;
            }
        }

        if( GL1_DEFS_CS4_CODING != gprs_decode_metrics_ptr->coding_type )
        {
          /* Include rx_qual in running total */
          scell_rxqual_ptr->value += gprs_decode_metrics_ptr->rxqual;

          (scell_rxqual_ptr->count)++;
        }
    }

    /* Update the bler sliding window */
    gpl1_gprs_serving_cell_data[gas_id].window_element_ptr->data_crc_fail =
    gprs_decode_metrics_ptr->data_payload_error;

    gpl1_gprs_serving_cell_data[gas_id].window_element_ptr->coding_scheme = gprs_decode_metrics_ptr->coding_type;

    /* Move to next element, take care of the wrap around case. */
    if( &( EGPL1_BLER_SLIDING_WINDOW[gas_id][ (EGPL1_DATA_BLK_WINDOW_SIZE - 1) ] ) == gpl1_gprs_serving_cell_data[gas_id].window_element_ptr )
    {
      gpl1_gprs_serving_cell_data[gas_id].window_element_ptr = &( EGPL1_BLER_SLIDING_WINDOW[gas_id][ 0 ] );
    }
    else
    {
      ++gpl1_gprs_serving_cell_data[gas_id].window_element_ptr;
    }

    /* Reset the buffer rssi content */
    rssi_buffer_ptr->accum_block_rssi = 0;
#if defined(FEATURE_GSM_RX_DIVERSITY_IDLE) || defined(FEATURE_GSM_RX_DIVERSITY_DATA) 
    rssi_buffer_ptr->accum_block_rssi_divrx = 0;
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE || FEATURE_GSM_RX_DIVERSITY_DATA*/
}

ARFCN_T* l1_get_gprs_arfcns ( gas_id_t gas_id )
{
  gpl1_gprs_serving_cell_data_t *gpl1_gprs_serving_cell_data_ptr = &gpl1_gprs_serving_cell_data[gas_id];
  return gpl1_gprs_serving_cell_data_ptr->gprs_arfcns ;
}

ARFCN_T  l1_get_gprs_arfcns_value ( uint8 index , gas_id_t gas_id)
{
  gpl1_gprs_serving_cell_data_t *gpl1_gprs_serving_cell_data_ptr = &gpl1_gprs_serving_cell_data[gas_id];
  return gpl1_gprs_serving_cell_data_ptr->gprs_arfcns[index] ;
}



