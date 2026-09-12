/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                C D M A    P A G I N G    M O D U L E

GENERAL DESCRIPTION
  Contains items associated with the paging channel

EXTERNALIZED FUNCTIONS
  rxc_pc_init - initializes the Paging Channel processing state
  rxc_pc - Paging Channel state processing
  rxc_pc_isr - interrupt handler for the Decoder interrupt used
  when the Receive task is processing the Paging Channel

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 1990 - 2020 Qualcomm Technologies, Inc.
  All Rights Reserved Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc. and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header: //components/rel/1x.mpss/8.0/mux/src/rxcpaging.c#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/22/20   vaa     Implement QSH MDUMP framework
11/29/16   srk     Use new events for paging and sync SW DEMBACK decode.
                   Revert previous changes.
10/13/16   eye     Capture the dec prev frame when SLOT_F/WAKE_F is rxed
08/23/16   eye     Added frame num check to wait if PC_ISR and PN ROLL collide.
07/08/16   nsh     QSH:Enable event diagnostic data to QSH
05/12/16   srk     Use locally stored/passed in RC instead of global variable
                   to avoid race condition between RX Task and TC ISR.
09/11/15   jh      Clean up message level
08/27/15   jh      Changed common SW Demback messages from Error to Medium
06/24/15   srk     1x SW support for 1x FW DEMBACK changes in ATLAS
05/05/15   ab      Soft Demback changes for QTA
04/24/15   srk     Disable SOM detection.
04/23/15   pap     Adding changes to account late SOM detection as bad pages
                   when detected 60ms after start of decode.
04/21/15   srk     DRDSDS and BOLT REVISIT Feature cleanup
02/12/15   jh      Changes for SW Demback
11/23/14   eye     Added support for 0x1877 Power Reporting Log Packet.
07/24/14   eye     Clearing failsafe timer while entering sleep.
03/21/14   vks     No need to send RXC_TC_F command to PC isr for timetransfer
03/05/14   srk     Message macro reduction
03/03/14   srk     Dynamically specify stack size for ISRs.
01/28/14   pap     Aggregating the variables into global structures.
12/07/13   cjb     Removed old/unused DEM1X register writes
12/07/13   cjb     Changes to support DEMBACK_1
11/25/13   cjb     Switched logic for FEATURE_MODEM_1X_BOLT_OBSOLETE feature.
10/28/13   srk     Remove references to obsolete 1xA enum.
10/25/13   cjb     Avoid registering for RX ISR when FW is disabled
10/10/13   cjb     DEMBACK0 vs DEMBACK1: Use DEMBACK ID from FW shared memory
09/27/13   bb      Changes to ignore PC interrupts if RXC is already in SLEEP
09/27/13   vks     Use rxccommon api to configure ch0 demod for sync/paging
09/23/13   vks     Update code beta information for paging
09/13/13   pap     Adding changes for the m1x_stmr module to be integrated.
09/11/13   vks     Use muxmdsp api to update ch0 config
08/28/13   cjb     Mainline FEATURE_MODEM_1X_DIME
08/16/13   vks     Mainline FEATURE_ZZ2_2
07/23/13   srk     Use TCB_PTR MACRO instead of rex_self() or tcb_ptr to make
                   the code compatible with RCINIT.
06/14/13   pap     Aggregating different global variables into structures.
05/31/13   bb      Changes to clear pending decoder interrupts before sending
                   out SLOT report to MC for BC_PAGE/BC_DATA scenarios
05/31/13   srk     Mainlined FEATURE_MODEM_1X_NON_SIG.
05/06/13   cjb     Resolve compiler warnings
02/15/13   srk     Replace mcs hwio with m1x hwio
01/08/13   trc     Remove deint task setup dependency on Rel B feature
12/19/12   bph     Replace memcpy w/ struct assignment or memscpy
10/18/12   srk     Use Mux debug F3s for unnecessary F3s.
09/15/12   ag      Changes for 1xCP memory optimization.
09/14/12   jtm     Feature cleanup.
07/17/12   eye     Notify SRCH of bad page if the SOM is detected
                   while in PC_MSG_STATE
07/16/12   srk     Removed FEATURE_IS2000_REL_A_CC
06/28/12   srk     Reverted previous edit.
06/28/12   srk     Mainlined FEATURE_IS2000_REL_A_CC.
06/21/12   jtm     Mainlined FEATURE_IS2000_REL_A_CC.
06/15/12   srk     Removed FEATURE_IS2000_REL_A_CC_SMMC
06/14/12   srk     Feature Cleanup.
04/16/12   jtm     Added support to vote for 1xA clock resources in 1xA RCs
04/04/12   trc     Convert to m1x_time module
03/16/12   jtm     Return RXC_BADCMD_S for illegal commands from MC.
02/24/12   srk     Mainlined FEATURE_IS2000_P2
02/17/12   srk     Replaced the tramp api with DAL Interrupt Controller.
02/16/12   srk     Replaced FEATURE_LTE_TO_1X by FEATURE_MODEM_1X_IRAT_LTO1X.
01/11/12   srk     Feature Cleanup
11/22/11   srk     Feature clean up: Mainlined and Internalized Features
11/07/11   jtm     Integrate ZZ2_2 support.
10/26/11   vks     Remove references to obsolete sleep* api and the code
                   associated with it.
10/12/11   jtm     Added rxc_get_pc_rate()
09/28/11   srk     Mainlined Always On features and removed Always Off Features
09/02/11   srk     Rebased the code to the NikeL modem package by mainlining
                   FEATURE_MODEM_1X_NIKEL.
09/02/11   jtm/vks Initialize deinterleaver when getting a SLOT_F after a
                   SLEEP_F.
08/31/11   adw     Unify TT and native TX jump to hyperspace.
06/15/11   bb      Changes to wait for DEC_DONE ISR under non critical section
                   code in rxc_pc()
06/29/11   adw     Re-initialize deint/dec upon wakeup.
05/09/11   trc     Remove unnecessary encoder coupling
04/29/11   adw     Remove extraneous enc_set_mode() calls.
04/22/11   jtm     CMI Phase 4 - Final SU API changes.
04/20/11   adw     Removed unnecessary modulator config from SYNC state.
03/31/11   adw     Clear decoder done interrupt status on NikeL.
03/03/11   adw     Initialize deinterleaver and decoder when entering paging.
01/28/11   jtm     Additional FTM decoupling.
01/25/11   jtm     Added Mux Vocoder interface module.
01/19/11   jtm     Eliminate dependency on FTM.
01/19/11   vks     More register featurization for nikel.
01/18/11   jtm     Lint clean up.
12/17/10   jtm     Feature cleanup.
12/15/10   vks     Temporarily featurize register accesses for Nikel.
12/14/10   jtm     Mainlined FEATURE_SRCH_CLKREGIM_RM and cleaned up unneeded
                   legacy clock code.
12/09/10   jtm     Removed FEATURE_IS2000_REL_D support.
12/09/10   jtm     Remove obsolete RXC_* commands.
12/08/10   jtm     Modified page match F3s for clarity.
12/01/10   bb      Changes to support LTE to 1X feature
11/15/10   jtm     Initial SU API split for CP.
11/05/10   jtm     Added more _i.h includes for Modem 1x SU API support.
11/01/10   jtm     Modem 1x SU API changes.
10/30/10   jtm     Modem 1x SU API changes.
10/21/10   jtm     No need to enable TD clks as part of RXC_TC_F.
07/20/10   jtm/adw Call rxc_prepare_rx_to_sleep() when we receive RXC_SLEEP_F
                   from MC to setup RXC for transition slotted sleep.
                   Enable decoder clocks in rxc_pc_init().
06/10/10   jtm     Eliminate dependency on log_dmss.h
05/14/10   vks     Replace MUXMSM_AGC_CTL_MASK_DELAY macro call with function
                   muxmdsp_set_agc_ctl_mask_delay()
02/04/10   jtm     Removed FEATURE_PLT.
09/22/09   jtm     Corrected compiler warnings.
08/22/09   jtm     Lint Fixes.
08/21/09   jtm     Lint Fixes.
08/11/09   jtm     Replaced deprecated ts.h api's with new time api's.
08/05/09   jtm     Lint fixes.
07/29/09   jtm     Moved RXC_PC_F case for FTM to just before default case.
06/12/09   jtm     Added debuging messages to get length and CRC info.
05/20/09   jtm     Added include for rx_v.h for CMI clean up.
04/28/09   vlc     Modified call to mcc_bc_match() so it is called at the right
                   time for large BCSMS messages which span 3 slots.
04/17/09   jtm     Added includes to fix compiler warnings.
04/01/09   jtm     Featurized necessary includes.
03/30/09   jtm     Lint fixes.
03/27/09   jtm     Eliminated implicit includes.
03/20/09   jtm     Cleaned up header file inclusions.
03/17/09   jtm     Replaced INTLOCK/INTFREE, INTLOCK_SAV/INTFREE_SAV, and
                   MSM_LOCK/MSM_UNLOCK with macros that can be mapped correctly
                   based on the compiler and controlled internally to RXC.
03/13/09   vlc     Changed BCSMS SLOT_R logic to safeguard corner cases.
02/02/09   vlc     In function rxc_pc(), do not send RXC_SLOT_R while BCSMS
                   is in progress.
11/19/08   jtm     Split various header files into internal and external
                   declarations for Common Modem Interface.
07/21/08   jtm     Removed call to rxc_ser_accum(). Added additional debug
                   messages for monitoring commands. Replaced block of code in
                   rxc_vote_not_ok_to_sleep() with rxc_configure_ffpc_channel()
                   since it is duplicated elsewhere.
07/01/08   vlc     Merged code changes from old tip archive.
04/10/08   trc     Switch clock mgmt strategy via FEATURE_SRCH_CLKREGIM_RM
04/02/08   trc     Call dec_svd_clk_enable() upon receipt of RXC_SLOT_F
03/05/08   vlc     User clock resource manager to manage RX clocks
                   (FEATURE_CLKREGIM_RM.)
05/03/07   cab     Moved to new sleep voting interface
02/23/07   vlc     Fixed compiler warnings/errors when SCH, Rel A, and P2
                   features are turned off.
10/30/06   bn      Added code to support DO to 1x handoff
06/09/06   trc     Remove T_MSM6700 featurization
05/08/06   bn      Added logic to support the case if receiving slot_mask = 0 in rude wakeup
01/20/06   fh      Added the support for AKA and MI.
03/31/06   bn      Added call to the function to map the deint RAM in rxc_pc_init()
03/22/06   kwo     Added calls into srch to report frame stats
03/22/06   rkc     Fix debug printing for OOS after slot+1.
01/12/06   rkc     OOS Enhancement: In addition to the normal SLOT_R, send MC
                   a SLOT_R after (slot+1) if a good message CRC after a bad
                   CRC in (slot) and (slot+1).
09/27/05   rkc     In rxc_log_pc_msg(), check if log_ptr != NULL before logging.
09/26/05   bn      Modifed the condition to send out decoder error message
09/08/05   ejv     Use new srch_mux interface calls.
09/07/05   rkc     Mainline FEATURE_ZZ2.
08/29/05   bn      Clear the all the decoder status it after installing the isr
08/09/05   bn      Added logic to indicate the page matched event to MC
07/25/05   rkc     Fixed lint errors.
07/05/05   bn      Fixed the page match logic in MUX
05/04/05   kwo     srch_idle_page_match() -> srch_mux_page_match()
04/27/05   vlc     Main lined code in current targets, cleaned out obsolete targets,
                   cleaned out obsolete features.
04/08/05   fc/vlc  Added support for FEATURE_IS2000_REL_D TCI to IDLE state
                   transition (for DCA - Direct Channel Assignment).
03/14/05   bn      Instead of SRCH, MUX need to install the ISR after WAKE_F
02/24/05   vlc     Lock interrupt between clearing DEC_DONE_INT_STATUS and
                   installing RXC ISR.
02/23/05   vlc     Clear the DEC_DONE_INT_STATUS when setting RXC ISR.
01/26/05   vlc     Removed obsolete ds.h inclusion.
06/24/04   sr      Added support for 2nd level dec_done interrupt in ISR
04/20/04   jrp     Fixed lint warnings.
04/14/04   sr      Removed unused ZIF logging code
03/05/04   sr/bkm  Moved initialization of frame offset from MC to RXC_TCI_F
11/24/03   vlc     Fixed compilation warnings for RVCT compiler.
10/21/03   bn      Added support for paging statistic counters
10/20/03   bn      Reset the DCCH,FCH data valid when entering TCI state
09/09/03   dna     Merged porting changes for MSM6500
07/18/03   vlc     Release A changes:  Changed FCH and DCCH
                   channel set up to better indicate when decoder data is
                   available for the appropriate channel(s) for the RXC layer.
06/16/03   bn      Fixed the "out of buffer dec_free_q" problem. Also fixed the
                   hard-handoff problem when switching from NCCCH to PCH
05/28/03   vlc     Added support for FEATURE_IS2000_REL_B traffic to idle
                   transition.
05/05/03   bn      Allowed transition from NCCCH to PCH
03/24/03   sr/bn   CHAN_CONFIG_F is now outside the sync channel workaround
                   FEATURE. We ignore it if we get it from SRCH in PC state.
03/05/03   fc      Consolidated f-csh fade timer implementation.
           sr      Added support for Sync Channel workaround.
02/25/03   va      Removed changing MC and SRCH priority by MC.
02/07/03   lcc/sr  Changed flush count from 8 to 4 in case of idle handoff
01/30/03   bt      Changed FAST_SLEEP_CMD for FEATURE_ZZ2.
08/06/02   cr      Added MSM6100 changes.
08/14/02   sr      Removed #define MAX_SKIPPED_FRAMES
08/13/02   sr      Added Fast RAHO support
06/03/02   sr      Changed CAIX_PC to RXC_CDMA_PC
05/31/02   lcc     Corrected programming of reverse power control delay to be
                   like MSM6050.
05/31/02   sr      Call to function rxcidle_first_frame() instead of inline
                   code that checks symbol combiner position to determine if
                   it is the first frame after wake up
05/30/02   vlc     VP2 changes are now featurized under FEATURE_IS2000_REL_A_SVD
                   and FEATURE_IS2000_P2 per request of development team.
                   Implemented comments from VP2 code review.
05/13/02   vlc     Added support for simultaneous DCCH & FCH channel set up
                   (feature FEATURE_IS2000_REL_A_VP2.)
03/05/02   ems     Merged in changes for MSM6050. Fixed shifting for setting
                   of MASK_DELAY in AGC_CTL8.  Added zif logging code.
02/27/02   lad     Merged IS2000 Releas A Common Channels:
                   Replaced inline code with rxc_clear_decoder_queue().
                   Moved rxc ATMR allocation to rxctraffic.c.
                   Moved TCI code to rxc_tci_init().
01/14/02   bgc     Added fixes for power control delay.
01/12/02   sst     Modified to use the new srch_* files that replaced srchdrv.c
                     Includes a function name change:
                       srch_get_vsc_time -> srch_scomb_get_vsc_time
11/16/01   vlc     Added call to dec_clk_select () before switching rxc state.
11/13/01   lh      Removed extra increment of CAI_PAG_4_ID.
11/13/01   bn      Added the enhance DCCH DTX detection and channel supervision logics
10/02/01   ejv     Added new SLEEP macros.
09/20/01   lcc     Moved call to rxc_init_action_item to rxctraffic.c inside
                   rxc_tci_init.
08/22/01   bn      Support DCCH in FTM mode
06/15/01   day     Mainlined FEATURE_FFPC, FEATURE_IS95B, FEATURE_TMSI, FEATURE_IS2000
                   Delete FEATURE_SPECIAL_MDR
06/11/01   fc      Changes to support split of fade reports between idle and
                   system access substates.
04/20/01   sr      Merged in from MSM5100 archive
04/19/01   tc      Merged with MSM_MUX1X.01.00.01
           lcc     Corrected problem initializing to the wrong RC in rxc_pc when
                   RXC_TCI_F is received.
           fc      Renamed mcc_report_event_timer to mclog_report_event_timer.
04/18/01   sr      Merged with MSM_MUX1X.01.00.00
           bgc     Added FEATURE_IS2000_P2 for DCCH.
03/27/01   vlc     Merged MSM_MUX1X.00.00.17.
           fc      Fixed timer ID for T40m.
           fc      Added support of events report: Message received and Timer
                   expired.
03/07/01   ph      Passing the parser encryption status on to MC when a
                   message is received on f-csch (PCH, BCCH, F-CCCH)
03/07/01   va      Added support for populating p_rev_in_use info during TC init.
03/02/01   tc      Merged MSM_MUX1X.00.00.15.
02/21/01   sr      Fixes for paging channel loss problem in rxc_pc_init()
02/16/01   bgc     Changed FEATURE_FACTORY_TESTMODE to enable a runtime switch.
01/18/01   ejv     Modified some combiner macros to match new names.
01/12/01   tc      Merged MSM_MUX1X.00.00.06.
11/23/00   tc      Fixed MSM5100 PLT ftm_pc_msg_sent, ftm_rxc_pc_cmd.
12/07/00   bgc     Commented out some unused PLT code.
11/29/00   jq      Moved initialization of rxc_slot.slot out of BROADCAST_SMS.
11/13/00   bgc     Moved FTM paging flags to rxcpaging.c
11/10/00   jc      This file was part of rxc.c. See rxccommon.c for full
                   change history

===========================================================================*/
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

//common
#include "1x_variation.h"
#include <memory.h>
#include "comdef.h"
#include "customer.h"
#include "modem_1x_defs.h"
#include "modem_1x_utils.h"

//mux
#include "rxc.h"
#include "ffpc_i.h"
#include "muxmdsp_i.h"
#include "muxmsm.h"
#include "rxccommon.h"
#include "rxccommon_i.h"
#include "rxctraffic.h"
#include "rxcsync.h"
#include "rxcpaging.h"
#include "rxcidle.h"
#include "rxc_i.h"
#include "rxcmc_i.h"
#include "mux_logtypes.h"
#include "muxmdsp_i.h"

//drivers
#include "dalint_1x.h"
#include "enc_v.h"
#include "enc_i.h"
#include "is2k_fl_decoder.h"
#include "m1x_hwio_mpss.h"
#include "msm_drv.h"
#include "msm_drv_i.h"
#include "dec5000_v.h"
#include "deint_i.h"
#include "dmod_i.h"

//cp
#include "parm_i.h"
#include "mccrx.h"
#include "cai.h"
#include "cai_v.h"
#include "caix.h"
#include "caix_i.h"
#include "rx.h"
#include "rx_v.h"
#include "mccrxtx.h"
#include "mclog.h"
#include "mclog_v.h"
#include "rxtx_v.h"
#include "mccdma_v.h"
#include "mccdma_i.h"
#include "caii_v.h"
#ifdef FEATURE_1XCP_QSH_SUPPORT
#include "mccqsh_i.h"
#endif /* FEATURE_1XCP_QSH_SUPPORT */

//srch
#include "srch_mux.h"
#include "srchmc_v.h"
#include "srchmc_i.h"
#include "srch_v.h"
#include "srch_i.h"

//diag
#include "onex_non_sig.h"

//other
#include "qw.h"
#include "rex.h"
#include "bit.h"
#include "queue.h"
#include "err.h"
#include "log.h"
#include "crc.h"
#include "cmd.h"
#include "msg.h"
#include "m1x_diag.h"
#include "m1x_time_i.h"
#include "onex_non_sig.h"
#include "stringl.h" /* for memscpy */

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/

rxcpaging_data_t rxcpaging_data=
{
  0,      /*rxc_pc_crc_good*/
  0,      /*rxc_pc_crc_bad*/
  0,      /*rxc_gpm_page_matched_counter*/
  0,      /*rxc_spm_page_matched_counter*/
  0,      /*rxc_srch_rssi_negative_counter*/
  FALSE,  /*rxc_page_increase_page_matched_cnt*/
  FALSE   /*ftm_pc_msg_sent*/
};

/*-------------------------------------------------------------------------*/
/* Paging channel State Variables                                          */
/*-------------------------------------------------------------------------*/

/* States of Paging Channel parsing state machine */
typedef enum
{
  PC_HUNT_STATE,
  PC_LEN_STATE,
  PC_MSG_STATE,
  PC_FLUSH_STATE,
  PC_SKIP_STATE
} rxc_pc_state_type;

#define RX_MIN_SOM_TIME_SCLKS TIMETICK_SCLK_FROM_MS( 60 )

struct
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*         TASK VARIABLES                                                  */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  qword msg_frame;
    /* frame in which current msg was received */
  rxc_state_type msg_state;
    /* state in which current msg was received */
  byte page_frm[ DEC_192_BITS / 8 ];
    /* Paging Channel frame read from Decoder */
  byte page_blk[ DEC_192_BITS / 8 ];
    /* reconstructed over-the-air Paging Channel frame */
  word pc_bits;
    /* Number of bits in a Paging Channel frame */
  word parse_bits;
    /* Number of bits to send to rxc_pc_parse */
  word parse_index;
    /* Bit index of next paging channel sub-frame to parse */
  word parse_start;
    /* parse_index reset value on (waking up after a sleep) */
  word bc1;
  /* Parameter for bcopy procedure used in rxc_pc_data */
  word bc2;
  /* Parameter for bcopy procedure used in rxc_pc_data */
  rxc_pc_state_type state;
  /* State of Paging Channel state machine */
  word  msg_inx;    /* Index into message buffer */
  word  msg_len;    /* total number of bits in message */
  word  err_cnt;    /* Number of consecutive messages with bad CRCs */
  rxtx_rx_msg_type *msg_ptr;
    /* pointer to buffer to place received message for Layer 2 task */
  word flush_cnt;    /* number of frames remaining to be flushed - used for
                        idle handoff */
  byte half_frames;  /* number of half frames which contained part of the
                        current message */
  boolean is_sleeping; /* Flag to keep track of deint initialization */


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*         INTERRUPT HANDLER VARIABLES                                     */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  cai_pch_rate_type pc_rate;
    /* Paging channel data rate */
} rxc_pc_var;

/* Disabling SOM detection algorithm for now */
#if 0
static timetick_type decode_start_time = 0;
#endif

dec_pc_status_type rxc_pc_status = { 0, 0, 0 };

#define RXC_MAX_INT_CNT    3    /* Max count for timing ints after wakeup */
#define RXC_SLOT_INT_CNT   2    /* Count at which our slot will arrive */

word rxc_last_pc_ser = 0;

/* Number of bits in 2 slots at 9600bps
   192 bits per frames * 4 frames * 2 slots = 1536
*/
#define RXC_9600_NUM_BITS_IN_2SLOTS 1536

/* Number of bits in 2 slots at 4800bps
   96 bits per frames * 4 frames * 2 slots = 768
*/
#define RXC_4800_NUM_BITS_IN_2SLOTS 768

/* Declarations for SW demback */
static is2k_fl_decoder_type *pPagingDecoder    = NULL;
static boolean rxc_paging_use_sw_demback       = FALSE;

static void rxc_pc_sw_isr (m1x_stmr_event_type events);
static is2k_fl_frame_type* rxc_pc_sw_decode( void );

#ifdef FEATURE_1XCP_QSH_SUPPORT
static int32 rxc_pc_consec_page_dec_fail_count = 0;
#endif /* FEATURE_1XCP_QSH_SUPPORT */

/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION RXC_PAGING_STMR_INIT

DESCRIPTION
  This function initializes the m1x_stmr timer resource for the paging
  channel state.  Also allocates SW demback resources.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
static void rxc_paging_stmr_init (cai_pch_rate_type pc_rate)
{
  int err = 0;

  /* Configure FW for SW demback. HW configuration is done in
     dec_config_clk_resources and frame boundary ISR. */
  muxmdsp_set_demback_mode( FALSE, TRUE );

  /* Init the SW decoder */
  if ( NULL == pPagingDecoder )
  {
    // Switch between 9600 & 4800
    if (CAI_PC_2_RATE == pc_rate)
    {
      pPagingDecoder = is2k_fl_decoder_new( IS2K_FL_PCH_4800 );
    }
    else /* (CAI_PC_1_RATE == rxc_pc_var.pc_bits) */
    {
      pPagingDecoder = is2k_fl_decoder_new( IS2K_FL_PCH_9600 );
    }
    M1X_MSG( MUX, LEGACY_MED, "Allocated pPagingDecoder" );
  }

  err = m1x_stmr_register( M1X_STMR_CLIENT_SW_DEM, M1X_STMR_DECODE_PCH , rxc_pc_sw_isr );
  if(err != M1X_STMR_SUCCESS && err != M1X_STMR_ERR_REG_DUP)
  {
    ONEX_ERR_FATAL("Unable to schedule the event with err: %d", err, 0, 0);
  }

} /* rxc_paging_stmr_init */

/*===========================================================================

FUNCTION RXC_PAGING_STMR_DEINIT

DESCRIPTION
  This funtion de-initializes the m1x_stmr timer resource for the paging
  channel state.  Also frees the SW demback resources.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
static void rxc_paging_stmr_deinit( void )
{

  /* Free up resources used by decoder */
  if ( pPagingDecoder )
  {
    is2k_fl_decoder_free(pPagingDecoder);
    pPagingDecoder = NULL;
    M1X_MSG( MUX, LEGACY_MED, "De-Allocated pPagingDecoder" );
  }

  /* Clear the ISR */
  m1x_stmr_deregister( M1X_STMR_CLIENT_SW_DEM, M1X_STMR_DECODE_PCH );

} /* rxc_paging_stmr_deinit */

/*===========================================================================

FUNCTION RXC_EMPTY_GPM_AND_DONE

DESCRIPTION
  This function performs an empty page match routine on General Page Message.
  It is called from the Paging Channel Decoder interrupt handler during slotted
  mode.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE: If it is an empty General Page Message that indicates no more page
        for this mobile in the current slot.
  FALSE: Otherwise.

SIDE EFFECTS
  None.

===========================================================================*/

boolean rxc_empty_gpm_and_done( dec_pc_type bits )
{
  /* Perform empty general page match with 72-bit Decoder delay */
  if ( (bits[9] == 0x84 ) &&
       (bits[10] == 0x88 ) &&
       ( (bits[11] & 0x80) == 0x80 ) &&
           /* The above pattern match ensures that
              1. SOM (SCI) bit is set to 1,
              2. MSG_LEN is 9-bytes for an empty General Page Message, and
              3. MSG_TYPE is CAI_GENERAL_PAGE_MSG, which is 0x11.
            */
       (((bits[11] & 0x7E) >> 1) == mcc_get_cur_msg_seq() ) &&
           /* current CFG_MSG_SEQ */
       ((cdma.assigning_tmsi.info.tmsi_code == INVALID_TMSI_CODE) ||
        (bits[12] & 0x01) == 0x01) &&   /* TMSI not assigned or TMSI_DONE */

       (
          ((cdma.imsi_addr_num != IMSI_CLASS_0_ADDR_NUM) &&
           ((bits[12] & 0x02) == 0x02)) ||
          ((cdma.imsi_addr_num == IMSI_CLASS_0_ADDR_NUM) &&
           ((bits[12] & 0x04) == 0x04))
       )      /* (MS has a class 1 IMSI and CLASS_1_DONE) or
                 (MS has a class 0 IMSI and CLASS_0_DONE)
               */
              /* Removed feature definition for FEATURE_CLASS_1_IMSI to embed the code */
       && ( mcc_bc_sms_enable ? (bits[13] & 0x40) : 1)
                 /* Check for BROADCAST_DONE */
    )
  {
    return (TRUE);
  }
  else
  {
    return (FALSE);
  }
} /* rxc_empty_gpm_and_done */



/*===========================================================================

FUNCTION RXC_EMPTY_SPM_AND_DONE

DESCRIPTION
  This function performs an empty page match routine on Slotted Page Message.
  It is called from the Paging Channel Decoder interrupt handler during slotted
  mode.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE: If it is an empty Slotted Page Message that indicates no more page
        for this mobile in the current slot.
  FALSE: Otherwise.

SIDE EFFECTS
  None.

===========================================================================*/
boolean rxc_empty_spm_and_done( dec_pc_type bits)
{
  /* Perform empty general page match with 72-bit Decoder delay */
  if ( (bits[9] == 0x84 ) &&
       (bits[10] == 0x02 ) &&
       ((bits[11] & 0x80) == 0x80 ) &&
           /* The above pattern match ensures that
              1. SOM (SCI) bit is set to 1,
              2. MSG_LEN is 8-bytes for an empty Slotted Page Message, and
              3. MSG_TYPE is CAI_SLT_PAGE_MSG, which is 0x05.
            */
        (((bits[11] & 0x7E) >> 1) == mcc_get_cur_msg_seq()) &&
                                        /* current CFG_MSG_SEQ */
        ((bits[12] & 0x04) == 0x00)     /* No More Pages */
      )
  {
    return (TRUE);
  }
  else
  {
    return (FALSE);
  }
} /* rxc_empty_spm_and_done */


/*===========================================================================

FUNCTION RXC_PAGING_CHECK_PAGE_MATCH

DESCRIPTION
  Common ISR processing for checking for a page match

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
static void rxc_paging_check_page_match(dec_pc_type bits)
{
  boolean page_match_enabled  = FALSE;
  boolean gpm_page            = FALSE;
  boolean spm_page            = FALSE;
  boolean empty_page_match    = FALSE;
  int2  srch_rssi_value = srch_mux_get_fing_rssi();

  if( rxc_slot.page_match )
  {
    /* OK, we can try to match. Go for it. */
    page_match_enabled  = TRUE;
    gpm_page = rxc_empty_gpm_and_done(bits);
    spm_page = rxc_empty_spm_and_done(bits);

    /* Disable matcher for this wakeup */
    rxc_slot.page_match = FALSE;

    /* Check if the first half frame of the slot is an empty General Page Msg */
    /* Note that this check will work for 9600 Paging Channels only with
       a low probability of false sleeping */

    /* Quick Page Match for a General Page/Slotted Pages */
    if ( gpm_page || spm_page )
    {
      empty_page_match = TRUE;

      if ( gpm_page )
      {
        /* increase the GPM page matched counter */
        rxcpaging_data.rxc_gpm_page_matched_counter++;
      }
      else
      {
        /* increase the SPM page matched counter */
        rxcpaging_data.rxc_spm_page_matched_counter++;
      }

      /* set the page matched flag to indicate to MC later on */
      rxcpaging_data.rxc_page_increase_page_matched_cnt = TRUE;

      /* An empty General Page/Slotted Page Message and no more page
         for this mobile in the current slot */
    }
  } /* page_match */

  M1X_MSG( MUX, LEGACY_HIGH,
           "Page Match enabled: %d, Empty Page match: %d, "
           "GPM: %d, SPM: %d, SER: %d|%d, RSSI: %d",
           page_match_enabled,
           empty_page_match,
           gpm_page,
           spm_page,
           rxc_last_pc_ser,
           (word)rxc_pc_status.ser,
           srch_rssi_value);

  srch_mux_page_match( empty_page_match );

  /* check to see if SRCH RSSI value if low, SRCH out of lock, then increase the counter */
  if (srch_rssi_value < 0 )
  {
    rxcpaging_data.rxc_srch_rssi_negative_counter++;
  }

} /* rxc_paging_check_page_match */

/*===========================================================================

FUNCTION RXC_PC_HANDLE_PENDING_INTERRUPTS

DESCRIPTION
  Common ISR processing for handling interrupts

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
static void rxc_pc_handle_pending_interrupts( void )
{
  rxc_isr_cmd_type *cmd_ptr;
    /* pointer to command received from the task */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* get frame number if interrupts have just settled down */
  m1x_time_get_20ms_frame_time ( rxc_dec_frame );
  /* During ISR of a paging channel (which operates in continuous mode),
   * the frame number being decoded is 2 less than the current system
   * time's frame number.  This is due to the fact that the 20ms_sys_time - 1
   * is containing the end of the frame before that. */
  qw_dec( rxc_dec_frame, 2L );

  /* Check to see if there are pending commands for the interrupt handler */
  while( (cmd_ptr = (rxc_isr_cmd_type *)q_get(&rxc_common_data.rxc_isr_q)) !=
           NULL )
  {
    /* process command */
    switch( cmd_ptr->cmd )
    {
      case RXC_SC_F:
      {
        M1X_MSG ( MUX, LEGACY_MED,
          "PC ISR: RXC_SC_F" );

        rxc_sc_init_isr( rxc_paging_use_sw_demback );

        if( FALSE == rxc_paging_use_sw_demback )
        {
          /* switch to Sync channel */
          deint_set_sc_mode();
        }

        break;
      }

      case RXC_TC_F:
      {
        M1X_MSG ( MUX, LEGACY_MED,
          "PC ISR: RXC_TC_F" );

        rxc_paging_stmr_deinit();

        dec_config_clk_resources( DEC_CLK_VOICE_TRAFFIC );

        /* Update the Demback ID */
        if( muxmdsp_fw_smem_addr )
        {
          dec_set_demback_id( muxmdsp_get_demback_id_index() );
        }
        else
        {
          M1X_MSG ( MUX, LEGACY_FATAL, "demback id not set" );
        }
        deint_init();
        deint_set_sc_mode();
        deint_set_pc_mode( PRAT_9600 );

        #ifdef FEATURE_IS2000_REL_A
        /* Always returns TRUE.  Ignoring return value. */
        (void) rxctraffic_frame_bnd_init ();
        #else /* FEATURE_IS2000_REL_A */
        /* switch to Traffic Channel and use initial rate set */
        #ifdef FEATURE_IS2000_REL_A_SVD
        if( rxc_so.for_dcch.included )
        {
          rxc_set_dcch_mode( (deint_rc_type) rxc_so.for_dcch.rc,
                             ( ffpc_channel == CAI_FPC_PRI_CHAN_DCCH ) );
        }

        if( rxc_so.for_fch.included )
        {
          rxc_set_fch_mode( (deint_rc_type) rxc_so.for_fch.rc,
                            ( ffpc_channel == CAI_FPC_PRI_CHAN_FCH ) );
        }
        #else /* FEATURE_IS2000_REL_A_SVD */
        if( rxc_so.for_dcch.included )
        {
          deint_set_fch_mode( (deint_rc_type) rxc_so.for_dcch.rc );
        }

        if( rxc_so.for_fch.included )
        {
          deint_set_fch_mode( (deint_rc_type) rxc_so.for_fch.rc  );
        }
        #endif /* FEATURE_IS2000_REL_A_SVD */

        /* increment frame count */
        qw_inc( rxc_dec_frame, 1L );

        #ifdef FEATURE_IS2000_1X_ADV
        if( rxc_so.for_fch.included )
        {
          /* Select the appropriate ISR for frame processing depending on the FCH RC. */
          rxc_tc_isr_select();
        }

        /* Check if this is a 1xA call and set clocks accordingly. */
        if( rxc_so.for_fch.rc == CAI_RC_11 )
        {
          dec_config_clk_resources( DEC_CLK_VOICE_TRAFFIC );
        }
        else
        {
          dec_config_clk_resources( DEC_CLK_IDLE );
        }
        #endif /* FEATURE_IS2000_1X_ADV */

        rxc_register_isr( (dalint_1x_isr) rxc_tc_isr, DALINT_1X_STACK_4K );

        /* Clear DEC DONE int status to make sure ISR is properly serviced */
        HWIO_OUT( DEC_CLR_INT_STATUS, HWIO_FMSK( DEC_CLR_INT_STATUS, VD_CLR_STATUS ) );
        #endif /* FEATURE_IS2000_REL_A */
        break;
      }

      case RXC_WAKE_F:
      {
        qword tmp_qw;

        M1X_MSG ( MUX, LEGACY_MED,
          "PC ISR: RXC_WAKE_F");

        qw_equ(tmp_qw, rxc_dec_frame);
        qw_shift(tmp_qw, -2); /* get the cur slot number */

        /* Save the page slot and sci mask for use elsewhere */
        rxc_slot.idle_pgslot = cmd_ptr->idle_pgslot;
        rxc_slot.sci_mask    = cmd_ptr->sci_mask;

        rxc_slot.slot_now = (word)(qw_lo(tmp_qw));

        if (rxc_slot.slot_now > rxc_slot.slot)
        {
          /* slot has wrapped the word boundary, back roll the number by one
           * cycle*/
          rxc_slot.slot -= (cmd_ptr->sci_mask+1);
          rxc_slot.slot_now -= (cmd_ptr->sci_mask+1);
        }

        if ((rxc_slot.slot_mask & PAGE_SLOT) || (rxc_slot.slot_mask == 0))
        {
          /* set 2 slot timer for assigned paging slot */
          rxc_slot.slot_frames =
              4 * (rxc_slot.slot - rxc_slot.slot_now) + 8
              - ( word )(qw_lo( rxc_dec_frame ) & 03);//lint !e734 Loss of precision (assignment) (18 bits to 16 bits)
          if (rxc_slot.slot_mask == 0)
          {
            /* in the case of rule wakeup, MC can send MUX a slot mask = 0, for such case, MUX
            should treat it as a PAGE_SLOT, but without a checking for page match */
            rxc_slot.slot_mask = PAGE_SLOT;
            rxc_slot.valid_msg_mask = rxc_slot.valid_msg_mask | rxc_slot.slot_mask;
            /* Page match is not available */
            rxc_slot.page_match = FALSE;
            M1X_MSG( MUX, LEGACY_HIGH,
              "MUX receives slot mask = 0 from MC, treat it as PC_SLOT");
          }
        }

        if (rxc_slot.slot_mask & BC_PAGE_SLOT)
        {
          /* set 2 slot timer for broadcast paging slot */
          rxc_slot.bcpg_slot_frames =
              4 * (rxc_slot.slot - rxc_slot.slot_now) + 8
              - ( word )(qw_lo( rxc_dec_frame ) & 03);//lint !e734 Loss of precision (assignment) (18 bits to 16 bits)
        }
        if (rxc_slot.slot_mask & BC_DATA_SLOT)
        {
          /* set 3 slot timer for broadcast SMS slot */
          rxc_slot.bcsms_slot_frames =
              4 * (rxc_slot.slot - rxc_slot.slot_now) + 12
              - ( word )(qw_lo( rxc_dec_frame ) & 03);//lint !e734 Loss of precision (assignment) (18 bits to 16 bits)
          /* no idle page match in the schedule broadcast slot */
          M1X_MSG( MUX, LEGACY_MED,
            "Page match disabled in BCSMS slot");
          rxc_slot.page_match = FALSE;
          rxc_slot.bc_started = FALSE;
        }
        RX_DBG_MSG( "SlotFrames: Asgn=%d, BC=%d, SMS=%d",
                 rxc_slot.slot_frames,
                 rxc_slot.bcpg_slot_frames,
                 rxc_slot.bcsms_slot_frames);

        rxc_slot.active = TRUE;

        rxc_slot.after_slot_boundary = FALSE;

        break;
      }

      default:
      {
        /* illegal command */
        ONEX_ERR_FATAL("PC ISR: Illegal command %d", (int) cmd_ptr->cmd, 0, 0 );
      }
    } /* switch */

    /* return the memnory back to the free queue */
    q_put (&rxc_common_data.rxc_isr_free_q, &cmd_ptr->link);

    (void)rex_set_sigs ( RX_TCB_PTR, RXC_ISR_CMD_SIG );
      /* signal task that command has been processed */
  }/* while */

} /* rxc_pc_handle_pending_interrupts */

/*===========================================================================

FUNCTION RXC_PC_SKIP_FRAME

DESCRIPTION
  ISR common processing for Skip Frame check

DEPENDENCIES
  None.

RETURN VALUE
  Boolean if frame is to be skipped.

SIDE EFFECTS
  None.

===========================================================================*/
static boolean rxc_pc_skip_frame(boolean* first_frame)
{
  boolean skip_frame = FALSE;

  if(NULL == first_frame)
  {
    return FALSE;
  }

  if ( rxc_slot.skipping == TRUE )
  {
    if( rxcidle_first_frame() == TRUE )
    {
      *first_frame = TRUE;
        /* This is the first frame of the slot, after a WAKEUP. */

      rxc_slot.skipping = FALSE;
        /* Don't skip frames anymore. */
    }
  }

    /* Capture and update the modem power stats in paging state */
    rxc_get_pwr_stats( CAI_INIT_RC, &rxc_srch_mux_pwr_stats );
    rxc_tx_update_flush_pwr_log();

  if( rxc_slot.skipping )
  {
     /* This is the interrupt for a frame
        we do not care about. Just exit,
        discarding it outright.  */

    rxc_slot.frame_skip_cnt++;
      /* Count each frame while skipping is TRUE */

    /* Once we have waited for an excessive number of frames to
       go by without recognizing the first frame of the slot, assume
       that the symbol combiner phase/offset is incorrect and start
       decoding.  This will likely cause a lost Paging Channel, but
       is better than hanging indefinitely (probably forever) waiting
       for the phase to be correct. */
    if( rxc_slot.frame_skip_cnt >= MAX_SKIPPED_FRAMES )
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "Skipped %d frames - SC must be wrong",
        rxc_slot.frame_skip_cnt);

      rxc_slot.skipping = FALSE;
        /* Don't skip frames anymore. */

      *first_frame = FALSE;
        /* This is definitely not the first frame of the slot, after a WAKEUP. */

      if( FALSE == rxc_paging_use_sw_demback )
      {
        RXC_ENTER_ATOMIC_SECTION();

        DECODER_INITIAL_STATE_NORMAL( );
          /* Ensure decoder is not reseting the Viterbi Decoder
             state metrics at the start of future frames. */

        RXC_LEAVE_ATOMIC_SECTION();
      }

      srch_mux_page_match( FALSE );
        /* Definitely no page matching happening at this point */
    }

    M1X_MSG( MUX, LEGACY_MED,
        "Decint: fr = %lx, slt = %d, PC: ser = %lx, PC_Energy = %lx",
        qw_lo(rxc_dec_frame),
        ( (word)(qw_lo( rxc_dec_frame ) >> 2) & rxc_slot.sci_mask ),
        rxc_pc_status.ser,
        rxc_pc_status.energy );

     rxc_last_pc_ser = rxc_pc_status.ser;
     skip_frame = TRUE;

  }

  return skip_frame;
} /* rxc_pc_skip_frame */


/*===========================================================================

FUNCTION RXC_PC_ISR

DESCRIPTION
  This function is the interrupt handler for the Decoder interrupt used
  when the Receive task is processing the Paging Channel.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void  rxc_pc_isr ( void )
{
  rxc_dec_type *buf_ptr;
    /* pointer to buffer to be placed on rxc_dec_q */
  boolean first_frame = FALSE;
    /* Set to true after wakeup from sleep on the first frame of the slot */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -- - - - -*/

  uint32 dec_int_status = 0;

  dec_int_status = GET_DEMBACK_REG( DEC_DONE_INT_STATUS );

  /* Clear DEC DONE int status to make sure ISR is properly serviced */
  SET_DEMBACK_REG( DEC_CLR_INT_STATUS,
                   HWIO_FMSK( DEC_CLR_INT_STATUS, VD_CLR_STATUS ) );

  if(dec_int_status != 0x2)
  {
    /* when MUX got an dec int, the status bits will be none-zero. If the status bit
     is zero, then this must be the very first time MUX configs the decoder. We won't flag this
     as an error. We will flag error when we expect SVB bit, but we got none-SVB bit
    */
    if (dec_int_status > 0)
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "DEC Done Int, but not SVD! %d",
        dec_int_status);
    }
    return;
  }

  rxc_pc_handle_pending_interrupts();

/*-------------------------------------------------------------------------*/
  /* perform normal interrupt processing */

  /* read in symbol error rates */
  dec_read_pc_status(&rxc_pc_status);

  srch_mux_idle_frame_stats(CAI_PCH, rxc_pc_status.ser, rxc_pc_status.energy,
                              FALSE);

  if (TRUE == rxc_pc_skip_frame(&first_frame))
  {
    return;
  }

  if ( (buf_ptr = (rxc_dec_type *)q_get( &rxc_common_data.rxc_dec_free_q))
           == NULL )
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "No buffers on rxc_dec_free_q" );
  }
  else
  {
    /* Get decoder data */
    dec_read_pc_data( buf_ptr->buf.pc_buf.bits );

    if( first_frame )
    {
      RXC_ENTER_ATOMIC_SECTION();

      DECODER_INITIAL_STATE_NORMAL( );
      /* Ensure decoder is not reseting the Viterbi Decoder
         state metrics at the start of future frames. */

      RXC_LEAVE_ATOMIC_SECTION();

      /* This is the interrupt for the first frame from our paging slot */
      M1X_MSG( MUX, LEGACY_MED,
         "  Decint: fr=%lx, slt=%d",
         qw_lo(rxc_dec_frame),
         ( (word)(qw_lo( rxc_dec_frame ) >> 2) & rxc_slot.sci_mask ) );

       /* Check to see if page matching was enabled this wakeup */
       rxc_paging_check_page_match(buf_ptr->buf.pc_buf.bits);

    } /* first_frame */

    qw_equ (buf_ptr->buf.pc_buf.frame, rxc_dec_frame );
      /* store frame number with data */
    buf_ptr->state = RXC_PC_STATE;
      /* indicate data from Pagig Channel */
    q_put( &rxc_common_data.rxc_dec_q, &buf_ptr->link );
      /* put item on queue for task */
    (void)rex_set_sigs ( RX_TCB_PTR, RXC_INT_SIG );
      /* set signal to indicate item is on queue */
  }

  #ifndef FEATURE_IS2000_REL_B
  /* For release B traffic to idle transition scenario, call deint_set_pc_mode()
  ** in rxc_pc_init() and do not call it again here.  Since the deint task list
  ** is deleted when exiting traffic state, it's necessary to set up the task
  ** list for Paging mode in order to get RXC interrupts.
  **
  ** Prior to release B implementation, the rxc_pc_isr() will run with Sync mode
  ** deint task list until the following condition is met and the deint task list
  ** is switched to Paging mode parameters.  This synchronization is deemed
  ** un-necessary since some Demod parameters have been program for Paging mode
  ** in rxc_pc_init(), so might as well program deint parameters in rxc_pc_init()
  ** also.
  */

  /* Switch decoder back to continuous mode if we are in Paging Channel state */
  if ( srch_mux_start_pc_state( FALSE ) )
  {

    /* REVISIT - Not sure how we handle traffic->idle transition but pc init
       should cover it. */
    if(rxc_paging_use_sw_demback == FALSE)
    {
      deint_set_pc_mode((deint_pc_rate_type)rxc_pc_var.pc_rate);
    }
  }
  #endif /* !FEATURE_IS2000_REL_B */

  /* perform AGC/power control logging */
  srch_mux_log_agc_pctl();

} /* rxc_pc_isr */



/*===========================================================================

FUNCTION RXC_LOG_PC_MSG

DESCRIPTION
  This function logs messages received on the Paging Channel.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void  rxc_log_pc_msg
(
  cai_pc_ext_type *msg_ptr   /* pointer to received message */
)
{
  mux_log_pc_msg_type  *log_ptr = NULL; /* Pointer to log */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  if (msg_ptr && msg_ptr->length != 0)
  {
    log_ptr = (mux_log_pc_msg_type *) log_alloc(LOG_PC_MSG_C,
                                            FSIZ(mux_log_pc_msg_type, hdr) +
                                            msg_ptr->length);
  }
  else
    log_ptr = NULL;

  if (log_ptr == NULL)
  {
    rxc_log_msg_drop++;
  }
  else
  {
    /* copy message into buffer */
    (void)memscpy( (void *) log_ptr->msg,
                   msg_ptr->length,
                   (void *) &msg_ptr->length,  //length includes both the byte for intself, as well as body
                   msg_ptr->length );//lint !e613 Possible use of null pointer (msg_ptr). The above has checked msg_ptr non-null

    log_commit(log_ptr);
  }

  /* Event report: Message received */
  if ((msg_ptr) && (log_ptr != NULL))
  {
    mclog_report_event_msg_rxed(CAI_PCH, msg_ptr->body);
  }
} /* rxc_log_pc_msg */




/*===========================================================================

FUNCTION RXC_PC_PARSE

DESCRIPTION
  This function collects the bits of a Paging Channel frame by parsing
  Paging Channel frames.  When it determines that an entire Paging Channel
  message has been received, and the message CRC checks, it will place the
  message on a queue for the Layer 2 task and signal the Layer 2 task.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/


void rxc_pc_parse
(
  qword  frame_num,   /* frame number */
  byte   sframe[],    /* Address of 1st byte of subframe. */
  word   sframe_len   /* Number of bits in subframe */
)
{
  word  sframe_inx;         /* index into subframe */
  word  num_bits;           /* Number of bits to be copied to the message
                               buffer */
  word parm_stat;           /* status returned by parm_inc */
  boolean frame_cnted = FALSE;
    /* indicator of whether subframe has been counted as one with a part of
        a message with a CRC that checks */
  mccrxtx_cmd_type *msg_ptr;
    /* pointer to buffer to place translated message in */
  word status;
    /* status returned by caix_ext_to_int */
  word msg_length;
    /* message length to pass to caix_ext_to_int */
  static qword  frame_stamp;
    /* timestamp frame number */
  word num_bits_in_2slots;
    /* number of bits in 2 slots of BCSMS */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* increment number of half frames received */
  if( ( parm_stat = parm_inc ( CAI_PAG_4_ID, 1 )) != PARM_DONE_S )
  {
    M1X_MSG( MUX, LEGACY_HIGH,
      "Invalid parameter increment: status %d",
      parm_stat );
  }

  if ( rxc_slot.active  &&  rxc_slot.after_slot_boundary == FALSE )
  {
    if ((word)(qw_lo( frame_num ) >> 2) == rxc_slot.slot)
    {
      rxc_slot.after_slot_boundary = TRUE;
    }
  }

  if ( ( sframe[0] & 0x80 ) && ( rxc_pc_var.flush_cnt == 0 ) )
  {
    if(rxc_pc_var.state == PC_MSG_STATE)
    {
      srch_mux_notify_page_decode(FALSE);
      M1X_MSG( MUX, LEGACY_MED,
        "Parser reset detected.");
    }

    /* start of message bit found - start parsing a new message regardless
       of message parsing state */
    rxc_pc_var.state = PC_HUNT_STATE;
    rxc_pc_var.half_frames = 0;
      /* zero out number of half frames counted for last message */
  }

  sframe_inx = 1;  /* Skip over 1st bit (SOM). */

  while ( sframe_inx < sframe_len )  /* While still more bits. */
  {
    switch ( rxc_pc_var.state )
    {
      case  PC_HUNT_STATE:
      {
        /* Search for start of message bit */
        if( sframe[0] & 0x80 )
        {
          /* Disabling SOM detection algorithm for now */
          #if 0
          /* check how long it has been since the decoders were started */
          if( decode_start_time != 0 )
          {
            int32 time_diff = 0;

            /* based on how far the time is we decide on marking bad pages */
            time_diff = (int32)( timetick_get() - decode_start_time );

            if( time_diff > RX_MIN_SOM_TIME_SCLKS )
            {
              int bad_page_count = (2*time_diff)/TIMETICK_SCLK_FROM_MS(20);

              /* we need to ascertain how much extra time was required */
              while( bad_page_count )
              {
                /* call setting of bad pages */
                srch_mux_notify_page_decode( FALSE );
                bad_page_count--;
              }
            }

            M1X_MSG( MUX, LEGACY_HIGH,
              "IDLE DIV: SOM detected, time_diff %d, start_time %d, min_time %d",
              time_diff, decode_start_time, RX_MIN_SOM_TIME_SCLKS);
            decode_start_time = 0;
          }
          #endif
          rxc_pc_var.msg_inx = 0;

          rxc_pc_var.state = PC_LEN_STATE;

          M1X_MSG( MUX, LEGACY_MED,
            "Received start of new page message");
        }

        else
        {
          rxc_pc_var.state = PC_FLUSH_STATE;
        }

        break;
      }

      case  PC_LEN_STATE:
      {
        rxc_pc_var.msg_len = (word)(b_unpackw ( sframe, sframe_inx, 8 ) * 8);

        M1X_MSG( MUX, LEGACY_MED,
          "Page Length In Bits = %d",
          rxc_pc_var.msg_len);

        if ( ( (sframe_inx + 8) <= sframe_len ) &&
             ( rxc_pc_var.msg_len > 0 ) &&
               ( rxc_pc_var.msg_len <= ( CAI_PC_MAX_MSG_BYTES * 8 ) ) )
        {

          if( (rxc_pc_var.msg_ptr == NULL ) &&
              ( (rxc_pc_var.msg_ptr =
#ifdef FEATURE_1X_CP_MEM_OPT
                   (rxtx_rx_msg_type *)rxtx_alloc_queue_buf( RXTX_RX_Q ) )
                     == NULL ) )
#else /* !FEATURE_1X_CP_MEM_OPT */
                   (rxtx_rx_msg_type *)q_get( &rxtx_rx_free_q) ) == NULL ) )
#endif /* FEATURE_1X_CP_MEM_OPT */
          {
            M1X_MSG( MUX, LEGACY_ERROR,
              "No buffers on rxtx_rx_free_q" );
            /* out of buffers -- go to skip state */
            rxc_pc_var.state = PC_SKIP_STATE;
          }
          else
          {
            qw_equ (frame_stamp, frame_num);

            b_copy ( sframe, sframe_inx,
                     &rxc_pc_var.msg_ptr->msg.pc.length,
                     rxc_pc_var.msg_inx, 8 );
                       /* copy length byte to message buffer */

            rxc_pc_var.msg_inx += 8; /* increment index into message buffer */

            sframe_inx += 8;    /* increment index into subframe */

            if( !frame_cnted )
            {
              rxc_pc_var.half_frames++;
                /* count this half frame as part of a message */
              frame_cnted = TRUE;
            }

            rxc_pc_var.state = PC_MSG_STATE; /* go to next state */

            if( ( parm_stat = parm_inc ( CAI_PAG_1_ID, 1 ))
                  != PARM_DONE_S )
            {
              M1X_MSG( MUX, LEGACY_HIGH,
                "Invalid parameter increment: status %d",
                parm_stat );
            }
          }
        }

        else
        { /* message length = 0 or less than 8 bits left in subframe */
          rxc_pc_var.state = PC_FLUSH_STATE;
        }

        break;
      }

      case PC_MSG_STATE:
      {
        num_bits = MIN ( (rxc_pc_var.msg_len - rxc_pc_var.msg_inx),
                         (sframe_len - sframe_inx) );
          /* calculate number of bits to copy into message buffer */
        b_copy ( sframe, sframe_inx, &rxc_pc_var.msg_ptr->msg.pc.length,
                   rxc_pc_var.msg_inx, num_bits );

        sframe_inx += num_bits;
          /* increment index into subframe */

        rxc_pc_var.msg_inx += num_bits;
          /* increment index into message buffer */

        /* Count this half frame as having a part of a message */
        if( !frame_cnted )
        {
          rxc_pc_var.half_frames++;
            /* count this half frame as part of a message */
          frame_cnted = TRUE;
        }

        num_bits_in_2slots = ((rxc_pc_var.pc_rate == CAI_PC_2_RATE) ?
          RXC_4800_NUM_BITS_IN_2SLOTS : RXC_9600_NUM_BITS_IN_2SLOTS);

        if ((rxc_slot.slot_mask & BC_DATA_SLOT) &&
            (rxc_pc_var.msg_inx > (CAII_BC_HDR_LEN+CAI_PC_LGTH_SIZE)) &&
            (rxc_pc_var.msg_inx < num_bits_in_2slots) &&
            (rxc_slot.bcsms_slot_frames > RXC_BC_PEEK_BOUNDRY) &&
            (rxc_slot.bc_started == FALSE))
        {
          /* if the current slot is a BC SMS slot, we have acquired enough
           * header bits, the number of bits we received is less than 2 slots,
           * and the beginning of these bits started in the first
           * slot of the scheduled slots, we can try to determine whether the
           * expected BC SMS has arrived. */
          if (mcc_bc_match(rxc_pc_var.msg_ptr->msg.pc.body))
          {
            /* we have found the expected BC */
            rxc_slot.bc_started = TRUE;
          }
        }

        if ( rxc_pc_var.msg_inx >= rxc_pc_var.msg_len )
        {
          /* all bits of message collected */
          if ( (crc_30_calc (&rxc_pc_var.msg_ptr->msg.pc.length,
                                       rxc_pc_var.msg_len )) == CRC_30_OK )
          {

            rxcpaging_data.rxc_pc_crc_good++;

            M1X_MSG( MUX, LEGACY_MED,
              "Good CRC: cnt=%d",
              rxcpaging_data.rxc_pc_crc_good);

            /* No consecutive page decode fail, count set to zero */
            #if defined(FEATURE_1XCP_QSH_SUPPORT) &&  \
                defined(FEATURE_QSH_EVENT_NOTIFY_TO_QSH)
              rxc_pc_consec_page_dec_fail_count = 0;
            #endif /* FEATURE_1XCP_QSH_SUPPORT && 
                      FEATURE_QSH_EVENT_NOTIFY_TO_QSH */
        
            srch_mux_notify_page_decode(TRUE);

            /* process message. */
            rxc_pc_var.err_cnt = 0;

            if( ( parm_stat = parm_inc ( CAI_PAG_5_ID, rxc_pc_var.half_frames))
                  != PARM_DONE_S )
            {
              M1X_MSG( MUX, LEGACY_HIGH,
                "Invalid parameter increment: status %d",
                parm_stat );
            }

            /* Reset the fade timer since we got a valid message */
            rxcidle_set_fade_timer(RXCIDLE_CURR_FADE);

            if ( rxc_slot.active  &&  !rxc_slot.valid_msg  &&
                 rxc_slot.after_slot_boundary )
            {
              rxc_slot.valid_msg = TRUE;
            }

            if (rxc_pc_var.msg_len > 40 )
            {
              /* log message */
              rxc_log_pc_msg( &rxc_pc_var.msg_ptr->msg.pc );

              /* translate every message but null message and send to
                  Main Control task */

              if((msg_ptr =
#ifdef FEATURE_1X_CP_MEM_OPT
                     (mccrxtx_cmd_type *)mc_alloc_queue_buf( MC_MCC_RXTX_Q ))
                       == NULL)
#else /* !FEATURE_1X_CP_MEM_OPT */
                     (mccrxtx_cmd_type *)q_get(&mcc_rxtx_free_q)) == NULL)
#endif /* FEATURE_1X_CP_MEM_OPT */
              {
#ifdef FEATURE_1X_CP_MEM_OPT
                M1X_MSG( MUX, LEGACY_ERROR,
                  "No buffers on modem heap for MC_MCC_RXTX_Q" );
#else /* !FEATURE_1X_CP_MEM_OPT */
                M1X_MSG( MUX, LEGACY_ERROR,
                  "No buffers on mcc_rxtx_free_q" );
#endif /* FEATURE_1X_CP_MEM_OPT */
              }
              else
              {
                msg_length = rxc_pc_var.msg_len - CAI_PC_CRC_SIZE -
                             CAI_PC_LGTH_SIZE ;
                status = caix_ext_to_int( msg_length, CAIX_PC,
                          &rxc_pc_var.msg_ptr->msg,
                          &msg_ptr->msg.msg );

                if ( (status != CAIX_DONE_S)
#ifdef FEATURE_IS2000_REL_A
                /* If Ext. Encryption failure occurs, we want to send the message to
                MC anyway so it can take appropriate action by examining the
                Ext. Encryption fields. */

                     &&
                     (status != CAIX_INV_ENC_MODE_NOT_SUP_S)
#endif
                   )
                {
                  M1X_MSG( MUX, LEGACY_MED,
                    "Bad PC translation: status %d",
                    status );
                  /* Put item back on to free q */
#ifdef FEATURE_1X_CP_MEM_OPT
                  mc_free_queue_buf( msg_ptr, MC_MCC_RXTX_Q );
#else /* !FEATURE_1X_CP_MEM_OPT */
                  q_put(&mcc_rxtx_free_q, &msg_ptr->msg.hdr.cmd_hdr.link );
#endif /* FEATURE_1X_CP_MEM_OPT */
                }
                else
                {
                  if ( (!onex_nonsig_is_ftm_mode()) ||
                       (onex_nonsig_wait_for_pc_msg() &&
                        !rxcpaging_data.ftm_pc_msg_sent ) )
                  {
                     /* User waits for a PC message */
#ifdef FEATURE_IS2000_REL_A
                    if (status == CAIX_INV_ENC_MODE_NOT_SUP_S)
                    {
                      /* Ext. Encryption failure at parser, log and pass to MC. */
                      M1X_MSG( MUX, LEGACY_HIGH,
                        "rxcpaging: Ext. Encryption failure, pass to MC");
                      msg_ptr->msg.parser_decryption_failure = TRUE;
                    }
                    else
                    {
                      msg_ptr->msg.parser_decryption_failure = FALSE;
                    }
#endif
                    msg_ptr->msg.hdr.command = MCCRXTX_MSG_F;
                    msg_ptr->msg.chn = CAIX_PC;
                    /* indicate to MC if there is a page matched */
                    if (rxcpaging_data.rxc_page_increase_page_matched_cnt)
                    {
                      msg_ptr->msg.page_match_counter_incremented = TRUE;
                      rxcpaging_data.rxc_page_increase_page_matched_cnt = FALSE;
                    }
                    else
                    {
                      msg_ptr->msg.page_match_counter_incremented = FALSE;
                    }
                    /* message is from Paging Channel */
                    qw_equ (msg_ptr->msg.frm.frame, frame_stamp);

#ifndef FEATURE_1X_CP_MEM_OPT
                    msg_ptr->msg.hdr.cmd_hdr.done_q_ptr = &mcc_rxtx_free_q;
                    msg_ptr->msg.hdr.cmd_hdr.task_ptr = NULL;
#endif /* !FEATURE_1X_CP_MEM_OPT */

                    /* Put message on queue for MC */
                    mccrxtx_cmd( msg_ptr);
                     rxcpaging_data.ftm_pc_msg_sent = TRUE;
                  }
                  else
                  {
                     /* Put item back on to free q */
#ifdef FEATURE_1X_CP_MEM_OPT
                     mc_free_queue_buf( msg_ptr, MC_MCC_RXTX_Q );
#else /* !FEATURE_1X_CP_MEM_OPT */
                     q_put( &mcc_rxtx_free_q, &msg_ptr->msg.hdr.cmd_hdr.link );
#endif /* FEATURE_1X_CP_MEM_OPT */
                  }
                }
              }

            }

            rxc_pc_var.state = PC_LEN_STATE;
          }

          else
          {
            /* CRC did not check */
            rxcpaging_data.rxc_pc_crc_bad++;

            M1X_MSG( MUX, LEGACY_MED,
              "Bad CRC, cnt=%d",
              rxcpaging_data.rxc_pc_crc_bad);

            /* Check for 3 consecutive page decode fail */
            #if defined(FEATURE_1XCP_QSH_SUPPORT) &&  \
                defined(FEATURE_QSH_EVENT_NOTIFY_TO_QSH)
            rxc_pc_consec_page_dec_fail_count++;
            if( rxc_pc_consec_page_dec_fail_count == RXC_CONSEC_PAGE_DEC_FAIL )
            {
              QSH_LOG(QSH_CLT_ONEXL3, QSH_CAT_EVENT, QSH_MSG_TYPE_HIGH,
                "QSH:Consec 3 page dec fail");
              mc_send_qsh_event(ONEXL3_QSH_EVENT_L2_PAGE_DEC_FAIL);
              rxc_pc_consec_page_dec_fail_count = 0;
            }
            #endif /* FEATURE_1XCP_QSH_SUPPORT && 
                      FEATURE_QSH_EVENT_NOTIFY_TO_QSH */

            if( ( parm_stat = parm_inc ( CAI_PAG_2_ID, 1 )) != PARM_DONE_S )
            {
              M1X_MSG( MUX, LEGACY_HIGH,
                "Invalid parameter increment: status %d",
                parm_stat );
            }

            srch_mux_notify_page_decode(FALSE);

            /* log message of 0 length to indicate bad CRC*/
            rxc_pc_var.msg_ptr->msg.pc.length = 0;
            rxc_log_pc_msg( &rxc_pc_var.msg_ptr->msg.pc );

            if ( ++rxc_pc_var.err_cnt > 2 )
            {
              /* need to resynchronize to paging channel */
              rxc_pc_var.err_cnt = 0;
              rxc_pc_var.state = PC_FLUSH_STATE;
            }
            else
            {
              rxc_pc_var.state = PC_LEN_STATE;
            }

          }
          rxc_pc_var.msg_inx = 0;  /* reset message index */
          /* regardless of whether CRC checked or not zero out count of
              half-frames which contributed to message */
          rxc_pc_var.half_frames = 0;
        }

        break;
      }

      case PC_FLUSH_STATE:
      {
        sframe_inx = sframe_len; /* Indicate all bits used up. */

        /* only go back to hunt state if you are not flushing frames from
           idle handoff */
        if( rxc_pc_var.flush_cnt > 0 )
        {
          rxc_pc_var.flush_cnt--;
        }
        if( rxc_pc_var.flush_cnt == 0 )
        {
          rxc_pc_var.state = PC_HUNT_STATE;
        }

        break;
      }

      case PC_SKIP_STATE:
      {
        /* Here we are keeping track of the bits of the message that
           have been skipped with msg_inx even though we are not
           collecting the bits in a buffer */

        num_bits = MIN ( (rxc_pc_var.msg_len - rxc_pc_var.msg_inx),
                         (sframe_len - sframe_inx) );

        sframe_inx += num_bits;
          /* increment index into subframe */
        rxc_pc_var.msg_inx += num_bits;
          /* increment index into message buffer */

        if ( rxc_pc_var.msg_inx >= rxc_pc_var.msg_len )
        {
          rxc_pc_var.state = PC_HUNT_STATE;
        }
        break;
      }

      default:
      {
        ONEX_ERR_FATAL("Invalid PC parse substate %d", (int) rxc_pc_var.state,0,0);
      }
    } /* switch */

  } /* while */

} /* rxc_pc_parse */

/*===========================================================================

FUNCTION RXC_PC_DATA

DESCRIPTION
  This procedure processes data received from the decoder while the Receive
  task is processing the Paging Channel.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_pc_data ( void )
{
  rxc_dec_type *buf_ptr;              /* pointer to decoder data buffer */
  is2k_fl_frame_type* pOutFrame;      /* pointer to output frame */
  boolean first_frame = FALSE;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  (void)rex_clr_sigs( RX_TCB_PTR, RXC_INT_SIG );


  while ( (buf_ptr = (rxc_dec_type *)q_get( &rxc_common_data.rxc_dec_q ) ) !=
           NULL )
  {
    if( buf_ptr->state == RXC_PC_STATE_SW )
    {

      pOutFrame = rxc_pc_sw_decode();

      /* Mimic HW ISR processing */
      if (pOutFrame)
      {
        memscpy (rxc_pc_var.page_frm,
                 sizeof(rxc_pc_var.page_frm),
                 pOutFrame->pOutBuffer,
                 pOutFrame->num_bits / 8);
        rxc_pc_var.msg_state = buf_ptr->state;
        rxc_pc_status.energy = (int4)pOutFrame->symbol_energy;
        rxc_pc_status.ser    = (word)pOutFrame->symbol_errors;

        qw_equ (rxc_pc_var.msg_frame, buf_ptr->buf.pc_buf.frame);

        /* Free up the buffer for the ISR */
        q_put( &rxc_common_data.rxc_dec_free_q, &buf_ptr->link );

        if (TRUE == rxc_pc_skip_frame(&first_frame))
        {
          /* The HW ISR does a return here, since we're in a
             while loop in SW setup we should continue to the next item.
             Also keep the SVD metrics at initial state, since we need
             those initial conditions at the slot boundary for IS2K */
          is2k_fl_decoder_reset( pPagingDecoder );
          continue;
        }

        srch_mux_idle_frame_stats(CAI_PCH, rxc_pc_status.ser, rxc_pc_status.energy,
                                    FALSE);

        M1X_MSG( MUX, LEGACY_MED,
            "Decint: fr = %lx, slt = %d, PC: ser = %lx, PC_Energy = %lx",
            qw_lo(rxc_dec_frame),
            ( (word)(qw_lo( rxc_dec_frame ) >> 2) & rxc_slot.sci_mask ),
            rxc_pc_status.ser,
            rxc_pc_status.energy );

        if( first_frame )
        {
          rxc_paging_check_page_match((byte*)pOutFrame->pOutBuffer);
        }
      }
    }
    else
    {
      /* copy the data we need from the decoder buffer */
      memscpy (rxc_pc_var.page_frm,
               sizeof(rxc_pc_var.page_frm),
               buf_ptr->buf.pc_buf.bits,
               sizeof(buf_ptr->buf.pc_buf.bits));
      rxc_pc_var.msg_state = buf_ptr->state;
      qw_equ (rxc_pc_var.msg_frame, buf_ptr->buf.pc_buf.frame);

        /* Free up the buffer for the ISR */
      q_put( &rxc_common_data.rxc_dec_free_q, &buf_ptr->link );

    }

    if( rxc_pc_var.msg_state == RXC_PC_STATE ||
        rxc_pc_var.msg_state == RXC_PC_STATE_SW )
    {
      /* Note that we must take into account 71 bit decoder delay on
         Paging Channel but since number of bits received per frame
         varies with the data rate we have set up the parameters for
         the b_copy routine in rxc_pc_init */

      b_copy ( rxc_pc_var.page_frm, 0, rxc_pc_var.page_blk,
               rxc_pc_var.bc1, rxc_pc_var.bc2 );
        /* Copy remaining portion of current frame to buffer. */

      while (rxc_pc_var.parse_index < rxc_pc_var.pc_bits)
      {
        /* Send bits to Paging Channel message processor */
        rxc_pc_parse ( rxc_pc_var.msg_frame,
                      &rxc_pc_var.page_blk[rxc_pc_var.parse_index/8],
                      rxc_pc_var.parse_bits );
        rxc_pc_var.parse_index += rxc_pc_var.parse_bits;
      }

      b_copy ( rxc_pc_var.page_frm, rxc_pc_var.bc2, rxc_pc_var.page_blk, 0,
               rxc_pc_var.bc1 );
        /* Copy first piece of next frame to buffer. */

      rxc_pc_var.parse_index = 0;

      /* If first part of new buffer is large enough to contain
         parsable sections, we can parse them. */
      if (rxc_pc_var.parse_bits < rxc_pc_var.bc1)
      {
        /* This message is part of the next frame; increase frame number */
        qw_inc( rxc_pc_var.msg_frame, 1L);

        while (rxc_pc_var.parse_index + rxc_pc_var.parse_bits <
               rxc_pc_var.pc_bits)
        {
          rxc_pc_parse( rxc_pc_var.msg_frame,
                       &rxc_pc_var.page_blk[rxc_pc_var.parse_index/8],
                       rxc_pc_var.parse_bits );
          rxc_pc_var.parse_index += rxc_pc_var.parse_bits;
        }
      }
    } /* end if RXC_PC_STATE || RXC_PC_STATE_SW */
  }
} /* rxc_pc_data */

/*===========================================================================

FUNCTION RXC_PC_INIT

DESCRIPTION
  This funtion initializes the Paging Channel processing state.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_pc_init
(
  rxc_state_type curr_state,  /* Current RXC state machine state */
  cai_pch_rate_type pc_rate   /* Rate of Paging Channel */
)
{
  /*-----------------------------------------------------------------------*/

  #if 0
  decode_start_time = timetick_get();
  M1X_MSG( MUX, LEGACY_MED,
    "decoder time noted in pc_init %d",
     decode_start_time );
  #endif
  dec_config_clk_resources(DEC_CLK_IDLE);

  rxc_paging_use_sw_demback = rxc_use_sw_demback();

  if (rxc_paging_use_sw_demback == FALSE)
  {
    /* Configure FW for HW demback. SW configuration is done in
       the rxc_paging_stmr_init method. */
    muxmdsp_set_demback_mode( TRUE, TRUE );

    /* Initialize the CDMA deinterleaver post sleep.
       Decoder will also be initialized. */
    deint_init();
  }

  /* Update demod channel 0 information for paging */
  rxc_config_demod_for_paging();

  switch( curr_state )
  {
    case RXC_SC_STATE:
    #if defined(FEATURE_VOIP_1X_HANDOFF) || defined(FEATURE_MODEM_1X_IRAT_LTO1X)
    case RXC_CDMA_STATE:
    #endif /* (FEATURE_VOIP_1X_HANDOFF || FEATURE_MODEM_1X_IRAT_LTO1X)  */
    #ifdef FEATURE_IS2000_REL_B
    case RXC_TC_STATE:
    #endif /* FEATURE_IS2000_REL_B */
    {

      /* Program the deint HW for F-PCH decode */
      if (FALSE == rxc_paging_use_sw_demback)
      {
        deint_set_pc_mode ((deint_pc_rate_type)pc_rate);
      }

      /* Set up variables for State machine */
      if( pc_rate == CAI_PC_1_RATE )
      {
        rxc_pc_var.pc_bits = 192;
      }
      else if ( pc_rate == CAI_PC_2_RATE )
      {
        rxc_pc_var.pc_bits = 96;
      }
      else
      {
        /* Set error value for 1/2 rate */
        rxc_pc_var.pc_bits = 96;
        M1X_MSG( MUX, LEGACY_HIGH,
          "Invalid PC rate %d",
          pc_rate );
      }

      /* set interrupt handler for Paging channel */
      rxc_set_pc_isr(rxc_paging_use_sw_demback, pc_rate);

      /* SVD has a 72 bit delay in continous mode */
      rxc_pc_var.bc2 = 72;
      rxc_pc_var.bc1 = rxc_pc_var.pc_bits - rxc_pc_var.bc2;

      /* We parse 2 subframes per frame */
      rxc_pc_var.parse_bits = rxc_pc_var.pc_bits / 2;

      /* Determine where in frame to start decoding sub-frames after wakeup.
         The first 71 bits out of the decoder may or may not be large
         enough to contain subframe(s) */
      rxc_pc_var.parse_start = (word)( rxc_pc_var.pc_bits -
         rxc_pc_var.parse_bits * (rxc_pc_var.bc2 / rxc_pc_var.parse_bits) );
      rxc_pc_var.parse_index = rxc_pc_var.parse_start;

      /* initialize ser totals */
      rxc_ser_reset();

      rxcpaging_data.rxc_pc_crc_good = 0;
      rxcpaging_data.rxc_pc_crc_bad = 0;

      /* to appease the Lint Gods.*/
      MODEM_1X_UNUSED(rxcpaging_data.rxc_gpm_page_matched_counter);
      MODEM_1X_UNUSED(rxcpaging_data.rxc_spm_page_matched_counter);
      MODEM_1X_UNUSED(rxcpaging_data.rxc_srch_rssi_negative_counter);

      rxc_pc_var.pc_rate = pc_rate;  /* Used in rxc_dec_pc_init */

      rxc_pc_var.err_cnt = 0;  /* initialize parsing errors */
      rxc_pc_var.state = PC_HUNT_STATE;
        /* initialize state of Paging Channel state machine */
      rxc_pc_var.half_frames = 0;
        /* no half frames have been part of a message yet */
      rxc_pc_var.flush_cnt = 0;  /*no frames to flush */

      rxc_slot.active = FALSE;  /* not in slotted mode yet */
      /* Make sure page matching is completely disabled */
      rxc_slot.page_match = FALSE;

      rxc_pc_var.is_sleeping = FALSE;
      break;
    } /* RXC_SC_STATE */

    default:
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "Bad state %d",
        curr_state );
    }
  } /* switch */

} /* rxc_pc_init */

/*===========================================================================

FUNCTION RXC_PC_EXIT

DESCRIPTION
  This funtion performs processing necessary to end the Paging channel state.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_pc_exit( void )
{

  rxcpaging_data.ftm_pc_msg_sent = FALSE;
    /* Reset the flag */

  /* make sure all items are placed back on the rxtx_rx_free_q */
  if (rxc_pc_var.msg_ptr != NULL )
  {
#ifdef FEATURE_1X_CP_MEM_OPT
    RXTX_FREE_QUEUE_BUF( rxc_pc_var.msg_ptr, RXTX_RX_Q );
#else /* !FEATURE_1X_CP_MEM_OPT */
    q_put(&rxtx_rx_free_q, &rxc_pc_var.msg_ptr->link );
    rxc_pc_var.msg_ptr = NULL;
#endif /* FEATURE_1X_CP_MEM_OPT */
  }

  rxc_paging_stmr_deinit();

}/* rxc_pc_exit */

/*===========================================================================

FUNCTION       RXC_PREPARE_RX_TO_SLEEP

DESCRIPTION    This function performs the necessary preparations for placing
               MUX into sleep state.

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   Decoder clocks will be disabled.

===========================================================================*/
void rxc_prepare_rx_to_sleep( void )
{
  RX_DBG_MSG( "rxc_cmd_rx_to_sleep called" );

  /* Disable RX ISR as we are done demodulating frames
  */
  rxc_clear_isr();

  /* Clean up SW Demback ISR */
  rxc_paging_stmr_deinit();

  /* Go back to HUNT state */
  rxc_pc_var.state = PC_HUNT_STATE;

  /* stop running slot timer also */
  rxc_slot.active = FALSE;

  /* reset all valid message reporting counters */
  rxc_reset_frame_count();

  rxcidle_clr_fade_timer();

  /* Clear buffers from decoder queue. */
  rxc_clear_decoder_queue ();

  dec_config_clk_resources(DEC_CLK_DISABLED);

  rxc_pc_var.is_sleeping = TRUE;

  M1X_MSG( MUX, LEGACY_HIGH,
    "Clearing failsafe timer" );
  (void) rex_clr_timer( &rxc_failsafe_timer );
  (void) rex_clr_sigs (RX_TCB_PTR, RXC_TIMER_SIG);

} /* rxc_prepare_rx_to_sleep */


/*===========================================================================

FUNCTION RXC_PC

DESCRIPTION
  Paging Channel state processing.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  Next state to be processed.

===========================================================================*/

rxc_state_type rxc_pc ( void )
{

  rxc_state_type next_state;
    /* next state to be processed */
  rxc_cmd_msg_type *cmd_ptr;
    /* pointer to command buffer received on rxc_cmd_q */
  mccrx_rpt_type *rpt_ptr;
    /* pointer to report to send to Main Control task */
  rex_sigs_type sigs;       /* signals returned by rx_wait */
  rex_sigs_type sa_wait_mask;
    /* This mask is used to set various signals on which Mobile waits */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -- - - - -*/

  next_state = RXC_PC_STATE;
  while( next_state == RXC_PC_STATE )
  {
    /* There might be a race condition between
       PC ISR and processing sleep command. Ingore all pending
       PC interrupts processing if RXC is sleeping,  */
    if ( rxc_pc_var.is_sleeping == FALSE  )
    {
    /* check for received interrupts */
    rxc_pc_data();
    }
    else
    {
      /* Clear buffers from decoder queue. */
      rxc_clear_decoder_queue ();
    }

    (void)rex_clr_sigs( RX_TCB_PTR, RXC_CMD_Q_SIG );
    if( (cmd_ptr = (rxc_cmd_msg_type *)q_get( &rxc_common_data.rxc_cmd_q ) ) !=
          NULL )
    {
      switch( cmd_ptr->hdr.command )
      {
        case RXC_CDMA_F:
          M1X_MSG( MUX, LEGACY_MED,
            "Got RXC_CDMA_F");

          /* --------------------------------------------------
          ** Transition back to CDMA init state without exiting
          ** rxc_subtask()
          ** -------------------------------------------------- */

          /* Clean up SW Demback ISR */
          rxc_paging_stmr_deinit();

          /* --------------------------------------
          ** Clean up before switching to CDMA mode
          ** -------------------------------------- */
          rxc_exit();

          cmd_ptr->hdr.status = RXC_DONE_S;
          next_state = RXC_CDMA_STATE;
          break;

        case RXC_EXIT_F:
        {
          M1X_MSG( MUX, LEGACY_MED,
            "Got RXC_EXIT_F");

          /* Clean up SW Demback ISR */
          rxc_paging_stmr_deinit();

          cmd_ptr->hdr.status = RXC_DONE_S;
          next_state = RXC_EXIT_STATE;
          break;
        }

        case RXC_SC_F:
        {
          M1X_MSG( MUX, LEGACY_MED,
            "Got RXC_SC_F");

          /* Clean up SW Demback ISR */
          rxc_paging_stmr_deinit();

          /* initialize Sync channel state */
          rxc_sc_init( RXC_PC_STATE);
          cmd_ptr->hdr.status = RXC_DONE_S;
          next_state = RXC_SC_STATE;
          break;
        }/* RXC_SC_F */

        case RXC_TCI_F:
        {
          M1X_MSG( MUX, LEGACY_MED,
            "Got RXC_TCI_F");
          next_state = rxc_tci_init (RXC_PC_STATE, cmd_ptr, FALSE);
          break;
        } /* RXC_TCI_F */

        case RXC_WAKE_F:
        {
          M1X_MSG( MUX, LEGACY_MED,
            "Got RXC_WAKE_F");

          #if 0
          decode_start_time = timetick_get();
          M1X_MSG( MUX, LEGACY_MED,
             "decoder time noted in rxc_pc(): %d",
             decode_start_time);
          #endif
          rxc_paging_use_sw_demback = rxc_use_sw_demback();

          dec_config_clk_resources(DEC_CLK_IDLE);

          if (FALSE == rxc_paging_use_sw_demback)
          {
            /* Configure FW for HW demback. SW configuration is done in
               the rxc_paging_stmr_init method. */
            muxmdsp_set_demback_mode( TRUE, TRUE );

            /* Initialize the CDMA deinterleaver post sleep.
               Decoder will also be initialized. */
            deint_init();

            /* Setup the SVD for paging channel */
            deint_set_pc_mode( (deint_pc_rate_type)rxc_pc_var.pc_rate );
          }


          /* call the function to install the corresponding decoder ISR */
          rxc_set_pc_isr (rxc_paging_use_sw_demback, rxc_pc_var.pc_rate);

          /* Reset parser to begin proper message parsing. */
          rxc_pc_var.parse_index = rxc_pc_var.parse_start;

          /* Reset flush count to 0 in case an Idle HO occured before
           * sleep
           */
          rxc_pc_var.flush_cnt = 0;

          RXC_ENTER_ATOMIC_SECTION();

          /* Update demod channel information for paging */
          rxc_config_demod_for_paging();

          /* Reinitialize slot counter */
          rxc_slot.skipping = TRUE;

          /* Reset the frame count used during skipping */
          rxc_slot.frame_skip_cnt = 0;

          /* Clear out path clear control flag */
          rxc_slot.blank = FALSE;

          if( rxc_pc_var.pc_rate == CAI_PC_1_RATE )
          {
             /* Optional page match feature only available
                for 9600-baud paging channel right now */
             rxc_slot.page_match = TRUE;

          }
          else
          {
             /* Page match feature not available */
             rxc_slot.page_match = FALSE;
          }

          rxc_pc_var.is_sleeping = FALSE;

          RXC_LEAVE_ATOMIC_SECTION();

          rxc_slot.slot_mask = cmd_ptr->wake.slot_mask;
          rxc_slot.valid_msg_mask |= cmd_ptr->wake.slot_mask;
          rxc_slot.slot = cmd_ptr->wake.slot;
          /* Send a command to the interrupt handler to reset frame count */
          rxc_common_data.rxc_isr_cmd_buf_ptr =
             (rxc_isr_cmd_type *) q_get (&rxc_common_data.rxc_isr_free_q);
          if (rxc_common_data.rxc_isr_cmd_buf_ptr != NULL)
          {
            rxc_common_data.rxc_isr_cmd_buf_ptr->cmd = RXC_WAKE_F;
            rxc_common_data.rxc_isr_cmd_buf_ptr->sci_mask =
                cmd_ptr->wake.sci_mask;
            rxc_common_data.rxc_isr_cmd_buf_ptr->idle_pgslot =
                cmd_ptr->wake.idle_pgslot;
            rxc_isr_cmd(rxc_common_data.rxc_isr_cmd_buf_ptr);
          }
          else
          {
            M1X_MSG( MUX, LEGACY_ERROR,
              "No buffers on rxc_isr_free_q" );
          }

          /* begin timing 2 slots and checking for a valid message in the
             2 slots */
          rxc_slot.valid_msg = FALSE;

          /* Start Idle fade timer */
          rxcidle_set_fade_timer(RXCIDLE_IDLE_FADE);

          cmd_ptr->hdr.status = RXC_DONE_S;
          break;
        }

        case RXC_SLOT_F:
        {
          M1X_MSG( MUX, LEGACY_MED,
            "Got RXC_SLOT_F");

          /* Reset parser to begin proper message parsing. */
          // rxc_pc_var.parse_index = rxc_pc_var.parse_start;
          /* it's commented out because if RXC_SLOT_F is received while a cross
           * slot message is being received, it would be discarded. it has been
           * seen happening with large SMS messages */

          dec_config_clk_resources(DEC_CLK_IDLE);

          if ( rxc_pc_var.is_sleeping &&
               FALSE == rxc_paging_use_sw_demback)
          {
            /* Configure FW for HW demback. SW configuration is done in
               the rxc_paging_stmr_init method. */
            muxmdsp_set_demback_mode( TRUE, TRUE );

            /* Initialize the CDMA deinterleaver post sleep.
               Decoder will also be initialized. */
            deint_init();

            /* Setup the SVD for paging channel */
            deint_set_pc_mode( (deint_pc_rate_type)rxc_pc_var.pc_rate );

            /* Update demod channel information for paging */
            rxc_config_demod_for_paging();
          }

          RXC_ENTER_ATOMIC_SECTION();

         /* Make sure page matching is completely disabled */
          rxc_slot.skipping   = FALSE;
          rxc_slot.page_match = FALSE;
          rxc_slot.blank      = FALSE;

          /* set interrupts on for Paging channel */
          rxc_set_pc_isr(rxc_paging_use_sw_demback, rxc_pc_var.pc_rate);

          rxc_pc_var.is_sleeping = FALSE;

          RXC_LEAVE_ATOMIC_SECTION();

          rxc_slot.slot_mask = cmd_ptr->wake.slot_mask;
          rxc_slot.valid_msg_mask |= cmd_ptr->wake.slot_mask;
          rxc_slot.slot = cmd_ptr->wake.slot;
          /* Send a command to the interrupt handler to reset frame count */
          rxc_common_data.rxc_isr_cmd_buf_ptr = (rxc_isr_cmd_type *)
               q_get (&rxc_common_data.rxc_isr_free_q);
          if (rxc_common_data.rxc_isr_cmd_buf_ptr != NULL)
          {
            rxc_common_data.rxc_isr_cmd_buf_ptr->cmd = RXC_WAKE_F;
            rxc_common_data.rxc_isr_cmd_buf_ptr->sci_mask =
              cmd_ptr->wake.sci_mask;
            rxc_common_data.rxc_isr_cmd_buf_ptr->idle_pgslot =
              cmd_ptr->wake.idle_pgslot;
            rxc_isr_cmd(rxc_common_data.rxc_isr_cmd_buf_ptr);
          }
          else
          {
            M1X_MSG( MUX, LEGACY_ERROR,
              "No buffers on rxc_isr_free_q" );
          }

          /* begin timing 2 slots and checking for a valid message in the
             2 slots */
          rxc_slot.valid_msg = FALSE;

          cmd_ptr->hdr.status = RXC_DONE_S;
          break;
        }

        case RXC_SLEEP_F:
        {
          M1X_MSG( MUX, LEGACY_MED,
            "Got RXC_SLEEP_F");

          rxc_prepare_rx_to_sleep();

          cmd_ptr->hdr.status = RXC_DONE_S;
          break;
        }

        case RXC_ACC_FADE_F:
        {
          M1X_MSG( MUX, LEGACY_MED,
            "Got RXC_ACC_FADE_F");

          rxcidle_set_fade_timer(RXCIDLE_ACCESS_FADE);

          rxc_slot.active = FALSE;  /* not in slotted mode anymore */

          cmd_ptr->acc_fade.hdr.status = RXC_DONE_S;
          break;
        }

        case RXC_IDL_FADE_F:
        {
          M1X_MSG( MUX, LEGACY_MED,
            "Got RXC_IDL_FADE_F");

          /* start using Idle fade timer (CAI_T30M ) */
          rxcidle_set_fade_timer(RXCIDLE_IDLE_FADE);



          cmd_ptr->idl_fade.hdr.status = RXC_DONE_S;
          break;
        }

        case RXC_IDLE_HO_F:
        {
          M1X_MSG( MUX, LEGACY_MED,
            "Got RXC_IDLE_HO_F");

          /* perform idle handoff processing */

          /* reset message parsing state machine */
          if( rxc_pc_var.msg_ptr != NULL )
          {
#ifdef FEATURE_1X_CP_MEM_OPT
           RXTX_FREE_QUEUE_BUF( rxc_pc_var.msg_ptr, RXTX_RX_Q );
#else /* !FEATURE_1X_CP_MEM_OPT */
            q_put( &rxtx_rx_free_q, &rxc_pc_var.msg_ptr->link );
            rxc_pc_var.msg_ptr = NULL;
#endif /* FEATURE_1X_CP_MEM_OPT */
          }

          rxc_pc_var.msg_inx = 0;
          rxc_pc_var.msg_len = 0;
          rxc_pc_var.err_cnt = 0;
          rxc_pc_var.half_frames = 0;
          rxc_pc_var.state = PC_FLUSH_STATE;
          rxc_pc_var.flush_cnt = 4;  /* flush next 4 half-frames */

          cmd_ptr->idle_ho.hdr.status = RXC_DONE_S;
          break;
        }

        default:
        {
          M1X_MSG( MUX, LEGACY_ERROR,
            "Illegal RX command %d",
            cmd_ptr->hdr.command);
          cmd_ptr->hdr.status = RXC_BADCMD_S;
        }
      } /* switch */

      /* return item to queue if necessary */
      cmd_done( &cmd_ptr->hdr.cmd_hdr );

      (void)rex_clr_sigs( RX_TCB_PTR, RXC_CMD_Q_SIG );
    }
    else
    {
      sa_wait_mask = RXC_CMD_Q_SIG | RXC_INT_SIG | rxcidle_wait_mask();

      sigs = rxc_wait(sa_wait_mask);

      /* Check if any fade timer has expired */
      if (rxcidle_process_sigs(sigs))
      {
        if ( sigs & RXC_INT_SIG )
        {
          /* do frame counting */
          if( rxc_slot.active)
          {
            /* counting slots */
            if( rxc_slot.slot_frames > 0 )
            {
              rxc_slot.slot_frames--;
            }
            if( rxc_slot.bcpg_slot_frames > 0 )
            {
              rxc_slot.bcpg_slot_frames--;
            }
            if( rxc_slot.bcsms_slot_frames > 0 )
            {
              rxc_slot.bcsms_slot_frames--;
            }
            M1X_MSG( MUX, LEGACY_LOW,
              "SlotFrames: Asgn=%d, BC=%d, SMS=%d",
              rxc_slot.slot_frames,
              rxc_slot.bcpg_slot_frames,
              rxc_slot.bcsms_slot_frames);

            /* the expected BC didn't start in the slot, we no longer need to
             * monitor for BC data anymore */
            if ((rxc_slot.slot_mask & BC_DATA_SLOT) &&
                (rxc_slot.bcsms_slot_frames <= RXC_BC_PEEK_BOUNDRY) &&
                !rxc_slot.bc_started)
            {
              rxc_slot.bcsms_slot_frames = 0;
            }

            M1X_MSG( MUX, LEGACY_LOW,
              "valid_mask=%d, valid_msg=%d",
              rxc_slot.valid_msg_mask,
              rxc_slot.valid_msg);

            /*
             * OOS Enhancement: Send MC a SLOT_R after (slot+1) if a good message CRC
             * after a bad CRC in (slot) and (slot+1).  Still send normal SLOT_R
             * if CRC passes in (slot) and (slot+1)
             */
            {
              byte rpt_slot_mask=0; /* the mask to be passed in SLOT_R */
              static int after_slot_1_cnt=0;   /* > 1 if after (slot+1) && no valid msg yet */

              if( rxc_slot.slot_frames > 0 )
              {
                after_slot_1_cnt = 0;  /* Countdown is not after (slot+1) */
              }
              if ((rxc_slot.slot_frames==0)&&(rxc_slot.valid_msg_mask&PAGE_SLOT))
              {
                rpt_slot_mask |= PAGE_SLOT;
                after_slot_1_cnt++;   /* Count one slot early since it only takes affect if msg CRC fails */
                if (after_slot_1_cnt == 2)
                {
                    M1X_MSG( MUX, LEGACY_HIGH,
                      "After slot+1, waiting for good CRC to send SLOT_R");
                }
              }
              if ((rxc_slot.bcpg_slot_frames==0) &&
                  (rxc_slot.valid_msg_mask & BC_PAGE_SLOT))
              {
                rpt_slot_mask |= BC_PAGE_SLOT;
              }
              if ((rxc_slot.bcsms_slot_frames == 0) &&
                  (rxc_slot.valid_msg_mask & BC_DATA_SLOT))
              {
                rpt_slot_mask |= BC_DATA_SLOT;
              }

              if ( rxc_slot.valid_msg )
              {
                /* Send SLOT_R if this is our assigned slot (normal case) or
                 * if this is after (slot+1) and no previous valid msg (OOS enhancement).
                 * Do not send SLOT_R if BCSMS is still in progress.
                 */
                if (
                    ( (rpt_slot_mask != 0)
                    && ((rxc_slot.slot_frames==0) || (after_slot_1_cnt > 1)) )
                    && (rxc_slot.bcpg_slot_frames == 0)
                    &&  (rxc_slot.bcsms_slot_frames == 0)
                   )
                {
                  /* Race condition might happen between complete reception of
                     DBM message and sending out SLOT_R report to MC. So, before
                     sending out SLOT report to MC, clear all pending decoder interrupts and
                     process messages */
                  if ( ( ( rpt_slot_mask & BC_DATA_SLOT ) != 0 ) ||
                       ( ( rpt_slot_mask & BC_PAGE_SLOT ) != 0 ) )
                  {
                    rxc_pc_data();
                    M1X_MSG( MUX, LEGACY_MED,
                      "Clear pending Decoder interrupts");
                  }

                  if ((rpt_ptr = (mccrx_rpt_type *)q_get(&mcc_rx_free_q) ) != NULL)
                  {
                    rpt_ptr->hdr.rpt = RXC_SLOT_R;
                    m1x_time_get_20ms_frame_time( rpt_ptr->slot.rpt_time );
                    rpt_ptr->slot.slot_mask = rpt_slot_mask;
                    /* we don't stop counting slot frames like before, the decision
                     * is left for CP to wake. rxc_slot.active cleared when
                     * RXC_SLEEP_F is rx'd */

                    if (after_slot_1_cnt > 1)
                    {
                      /* We got here after (slot+1) which means (slot) and (slot+1) failed CRC. */
                      M1X_MSG( MUX, LEGACY_HIGH,
                        "Sending SLOT_R after missed slot (OOS)" );
                    }
                    else
                    {
                      M1X_MSG( MUX, LEGACY_HIGH,
                        "Sending SLOT_R (normal case)" );
                    }
                    /* Clear flag so we do not keep sending SLOT_R. */
                    after_slot_1_cnt = 0;

                    rxc_mc_rpt(rpt_ptr );
                    /* clear the bits that triggered the valid msg report, so we
                     * don't send the same report more than once */
                    rxc_slot.valid_msg_mask &= ~rpt_slot_mask;
                  }
                  else
                  {
                    M1X_MSG( MUX, LEGACY_ERROR,
                      "No buffers on mcc_rx_free_q" );
                  }
                }
              }
            }
          }
        } /* end of frame counting */
      } /* if (rxcidle_process_sigs(sigs, RXC_PCH_STATE)) */
    } /* if( (cmd_ptr = (rxc_cmd_msg_type *)q_get(&rxc_cmd_q) ) != NULL ) */


    M1X_MSG( MUX, LEGACY_LOW,
      "RXC slot_frame=%d",
      rxc_slot.slot_frames);
  } /* while */

  /* Perform exit processing */
  rxc_pc_exit();

  return( next_state );

} /* rxc_pc */

/*===========================================================================

FUNCTION RXC_PC_SW_DECODE

DESCRIPTION
  This function performs a SW decode on the paging channel

DEPENDENCIES
  None.

RETURN VALUE
  Pointer to frame data.

SIDE EFFECTS
  None.

===========================================================================*/
static is2k_fl_frame_type*  rxc_pc_sw_decode( void )
{
  const uint32 num_syms = 384;  /* TODO - decoder should provide API for number
                                   of symbols */
  is2k_fl_symbol_type pch_syms[num_syms];
  is2k_fl_frame_type *pOutFrame = NULL;

  if ( pPagingDecoder != NULL )
  {
    /* Read the symbols */
    (void)muxmdsp_extract_ch0_syms( TRUE, TRUE, pch_syms, num_syms );
    /* Process the extracted symbols */
    pOutFrame = is2k_fl_decode_frame( pPagingDecoder, pch_syms, num_syms );
    (void)is2k_fl_decode_perfdump(pPagingDecoder);

    if ( pOutFrame != NULL )
    {
      uint32 *pOut = (uint32 *)pOutFrame->pOutBuffer;
      M1X_MSG( DEC, LEGACY_MED,
               "DEBUG: PCH = [0x%X] [0x%X] [0x%X] [0x%X] [0x%X] [0x%X]",
               pOut[0],pOut[1],pOut[2], pOut[3],pOut[4],pOut[5] );
    }
  }

  return pOutFrame;

}

/*===========================================================================

FUNCTION RXC_PC_SW_ISR

DESCRIPTION
  ISR for SW demback mode of operation

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS


===========================================================================*/
static void rxc_pc_sw_isr (m1x_stmr_event_type events)
{
  rxc_dec_type *buf_ptr;

  M1X_MSG( MUX, LEGACY_MED,
           "rxc_pc_sw_isr FIRED");
   
  /* Check for Rx AGC delta */
  #if defined(FEATURE_1XCP_QSH_SUPPORT) &&  \
      defined(FEATURE_QSH_EVENT_NOTIFY_TO_QSH)
  if( srch_mux_is_rx_div_enabled() )
  {
    rxc_check_rx_agc_delta();
  }
  #endif /* FEATURE_1XCP_QSH_SUPPORT && 
            FEATURE_QSH_EVENT_NOTIFY_TO_QSH */

  rxc_pc_handle_pending_interrupts();

  if ( (buf_ptr = (rxc_dec_type *)q_get( &rxc_common_data.rxc_dec_free_q))
           == NULL )
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "No buffers on rxc_dec_free_q" );
  }
  else
  {

    qw_equ (buf_ptr->buf.pc_buf.frame, rxc_dec_frame );
    buf_ptr->state = RXC_PC_STATE_SW;

    /* indicate data from Paging Channel */
    q_put( &rxc_common_data.rxc_dec_q, &buf_ptr->link );

      /* put item on queue for task */
    (void)rex_set_sigs ( RX_TCB_PTR, RXC_INT_SIG );
  }

} /* end rxc_pc_sw_isr() */

/*===========================================================================

FUNCTION RXC_SET_PC_ISR

DESCRIPTION
  This function sets up the paging channel isr for DEC_INT.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_set_pc_isr
(
  boolean use_sw_demback,
  cai_pch_rate_type pc_rate
)
{
  RXC_ENTER_ATOMIC_SECTION();

  if (use_sw_demback)
  {
    /* Clean up HW ISR (in case of switching modes) */
    rxc_clear_isr();

    /* Init SW ISR */
    rxc_paging_stmr_init(pc_rate);

  }
  else
  {
    /* Clean up SW ISR (in case of switching modes) */
    rxc_paging_stmr_deinit();

    /* Init HW ISR */
    rxc_register_isr( (dalint_1x_isr) rxc_pc_isr, DALINT_1X_STACK_4K );

    /* Clear DEC DONE int status to make sure ISR is properly serviced. */
    SET_DEMBACK_REG( DEC_CLR_INT_STATUS,
                     HWIO_FMSK( DEC_CLR_INT_STATUS, VD_CLR_STATUS ) );
  }

  RXC_LEAVE_ATOMIC_SECTION();
} /* rxc_set_pc_isr */

/*===========================================================================

FUNCTION RXC_GET_PC_RATE

DESCRIPTION
  This function returns the current pc_rate for PCH.

DEPENDENCIES
  None.

RETURN VALUE
  cai_pch_rate_type of either 1 = 4800bps or 0 = 9600bps

SIDE EFFECTS
  None.

===========================================================================*/

cai_pch_rate_type rxc_get_pc_rate(void)
{
  return rxc_pc_var.pc_rate;
}
