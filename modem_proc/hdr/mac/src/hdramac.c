/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                 H D R   A C C E S S   C H A N N E L   M A C

                             P R O T O C O L

GENERAL DESCRIPTION
  This module contains the implementation of the HDR Access Channel Media
  Access Control (MAC) Protocol for an HDR access terminal.

EXTERNALIZED FUNCTIONS
  hdramac_init
    Initialize the Access Channel MAC protocol.

  hdramac_activate
    Activate the Access Channel MAC protocol.

  hdramac_deactivate
    Deactivate the Access Channel MAC protocol.

  hdramac_is_active
    Determine if protocol is active.

  hdramac_is_in_attempt
    Determine if protocol is currently in an access attempt.

  hdramac_start_attempt
    Initiate an access attempt.

  hdramac_process_cmd
    Process Access Channel MAC commands from the task command queue.

  hdramac_process_sig
    Process Access Channel MAC signals received at task level.

  hdramac_process_msg
    Process received Access Channel MAC messages.

  hdramac_msg_cb
    Callback function that queues received Access Channel MAC messages onto
    the task message queue.

  hdramac_get_last_tx_pwr
    Return transmit power level used for the most recent access probe.

  hdramac_configure
    Performs any necessary configuration for AT-init phase of negotiation.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  The protocol must be initialized after system startup prior to activating
  the protocol.  The procotol must be activated before an access probe
  sequence can be sent.

  Copyright (c) 2000 - 2015 by Qualcomm Technologies, Inc.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* EJECT */
/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$PVCSPath: O:/src/asw/COMMON/vcs/hdramac.c_v   1.54   30 Jan 2003 15:46:30   kevins  $
$Header: //components/rel/hdr.mpss/6.0/mac/src/hdramac.c#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
05/27/16   vko     QSH framework for events capturing
07/13/15   arm     Mainlined FEATURE_RF_SVDO_API feature.
05/21/15   kss     Added support for modem tx power stats.
05/11/15   wsh     Added support for RxTx Split and TRM grouped API
04/06/15   rmv     Ignore handling duplicate AMAC ISR fired at same frame and
                   hstr
03/27/15   wsh     Fixed processing AMAC ISR after RMAC is active  
01/27/15   arm     Shutdown TX if not completely off during tuneaway.
01/09/15   sat     Changes to add Pilot strength correction in Non RUMI cases only.
10/17/14   wsh     Fixed reporting wrong mode to TRM in 4a->3a transistion
10/14/14   wsh     Changes to support Jolokia bringup
09/29/14   sat     Changes to not skip disable Tx if Acack not received.
07/29/14   tnp     FR22375: Fix Medium compiler warnings in HDR
07/21/14   sat     Mainlined hybrid_during_overhead to always TRUE
05/02/14   rmv     Re-schedule the probe if Access Attempt is stopped because 
                   of tune away 
03/14/14   dsp     FR19338:Reduce call setup time by skipping TX RF shutdown.
01/27/14   sat     Mainlined FEATURE_HYBRID_ACCESS_DURING_OVERHEAD.
12/16/13   sat     Changed memcpy() to assignment or memscpy() (safer version).
11/26/13   rmg     Supported enabling carriers in FW without a frame delay.
10/29/13   sat     Reduced F3 messages (or/and) ASSERTs
10/02/13   wsh     Changes to support BOLT RUMI
08/07/13   kss     Handled possible race condition when tuning away.  
08/01/13   smd     Used new secapi to get random number.
07/15/13   rmv     Added protection around hdrmod APIs while stopping 
                   access attempts as a result of tune away. 
09/03/13   wsh     Replaced FEATURE_DIME/TRITON_BOLT_MODEM with HDR version
06/26/13   wsh     Added Selective TxD feature
06/10/13   kss     Handled possible race condition when disabling AMAC ISR. 
05/28/13   kss     Added display of channel gains during probes. 
05/06/13   mbs     Fixed compiler errors for BOLT build
04/04/13   kss     Changed to always transition to tuneaway state after probe.
02/26/13   kss     Fixed incorrect (out-of-bounds) indexing for log packet.
01/17/13   rmg     Added support for seperating modulator config from PA ctrl.
01/10/13   kss     Don't disable Tx/Mod if they are already disabled. 
01/08/13   dsp     Moved programming of pn_code from hdrhitmsg_process_mod_acc() 
                   to hdramac_process_start_attempt(). 
12/19/12   smd     Added RUMI Tx support.
12/11/12   kss     Ensure modConfig is disabled in FW on access cleanup. 
11/20/12   mbs     Fixed compilation errors for Triton build
09/20/12   srn     Fixed QCHAT API issue. 
07/17/12   kss     Enable FW access mode and modulator before each probe. 
07/06/12   kss     Remove delay between TRANSMISSION_STOPPED_IND & TX_ENDED_IND
04/30/12   dsp     Fixed compiler warnings on Dime. 
04/10/12   kss     Reschedule probe if initial scheduled event is late.
03/19/12   kss     Fixed issue where incorrect max RRI could be read from FW
03/22/12   kss     Fixed QCHAT merge issue. 
03/20/12   cnx     Resolved TCB compiler errors for Dime. 
03/20/12   cnx     Resolved TCB compiler errors for Dime. 
03/19/12   kss     Fixed more Dime compiler errors.
03/19/12   ljl     Fixed Dime compiler errors.
03/09/12   kss     Fixed access_offset!=0 issue causing first probe to fail.
03/06/12   rmg     Cleared RMAC ISR signal when the ISR is uninstalled.
02/14/12   wsh     Added support for SVLTE+SHDR
02/27/12   smd     Added updates of FTM msgr interface changes. 
01/26/12   ljl     Replaced tramp with DAL.
01/16/12   rmg     Added change to turn PA off before disabling DAC.
12/09/11   rmg     Added support for calling tx stop confirmation callback 
                   after processing stop access command for tuneaway.
11/18/11   kss     Default to hybrid access during overhead behavior. 
11/16/11   rmg     Removed workaround for MAPE and fast connect issue.
10/13/11   kss     Increased wait added below to 192 slots.
08/25/11   kss     Added a 64 slot wait before sending of access probes as 
                   workaround for access probe failure issue.
06/21/11   rmg     Added support for updated hdrmod_tx_start/stop().
06/21/11   kss     Cleaned up and reverted bringup changes.
06/16/11   kss     Added check for tuneaway time sanity.
05/02/11   kss     Fixed compiler warning.
04/27/11   kss     Supported FW-controlled Tx enable.
03/24/11   kss     Updated RF SVDO APIs.
03/23/11   kss     Adjusted RF includes for NikeL RF.
03/02/11   kss     Check LMAC state before sending ACMAC_READY ind.
01/21/11   rmg     Supported RF API changes for NikeL.
11/18/10   kss     Added tuneaway support for when LMAC enters access mode.
09/15/10   pxu     Replaced JCDMA #ifdef macros with dynamic function checks. 
06/21/10   cnx     Fixed compiler warnings.
06/09/10   cnx     Fixed Klocwork errors. 
01/25/10   kss     Removed use of RF Tx bring up functions.
12/19/09   kss     Remove BTF and other obsolete bringup flags..
12/08/09   kss     MDM9k support clean up.
12/02/09   rmg     Featurized calls to rfhdr_ functions under 
                   FEATURE_RF_BRINGUP_SUPPORT.
12/01/09   kss     Added debug test control variables for AMAC.
10/23/09   kss     Move ISR processing to task. 
09/30/09   kss     Added support for MSGR.
09/15/09   kss     Use new PACK() macro for Q6 compatibility.
05/20/09   kss     Increase packet buffer to account for CRC bytes.
04/16/09   rkc     Mainlined T_MSM6500.
04/15/09   kss     Fix delay issue when ProbeBackoff is 0 and probe 
                   collides with RLSI.
04/14/09   wsh     Mainlined T_MSM6800 to solve compile issue
04/01/09   wsh     JCDMA passport mode changes
03/23/09   kss     Fix application of probe_time_out_adjust.
12/05/08   kss     Removed/replaced intlocks.
11/12/08   kss     Signal search to disable tuneaway after ack (per standard).
11/05/08   kss     Fix for hybrid access during persistence.
10/18/08   etv     Log ACMAC Capsule after Probe number is updated.
08/29/08   lyl     Used primary chain filtered energy of best ASP.
05/28/08   kss     Only enable carrier 0 in mDSP.
03/08/08   etv     Replaced obsolete RF APIs with new ones
03/13/08   etv     Removed RevB 7500 featurization.
02/15/08   wsh     Fixed rare crash caused by sending back-to-back AC msg in 
                   PSIST_NO_ACCCESS scenario
11/21/07   rmg     Fixed DISABLE_CHANNEL_MC() macro call.
11/20/07   etv     Featurized hdrsrchmac API for 7800 compilation.
11/12/07   kss     7800 integration.
10/05/07   kss     Fix logging issue that occurs if ACAck is received early.
09/21/07   kss     Include file cleanup.
08/28/07   kss     Check whether config is needed before AT prop.
08/23/07   etv     Fixed high lint warnings.
07/31/07   kss     Fix compiler warning.
07/05/07   kss     Fix compiler warning.
06/19/07   cc      Change hdramac_gaup_attrib_verno_offsets[] to uint16.
04/12/07   kss     Initial support for PN Randomization.
04/02/07   hrk     Added Enhanced ACMAC and Access Probe Logging support.
03/28/07   rmg     Added hdramac_override_config().
01/16/07   kss     Disable Tx in deactivate if tune away has not occurred.
11/15/06   hrk     Ignore persistence tests if session not negotiated.
10/12/06   hrk     At end of probe go to TUNEAWAY only if tune away scheduled. 
10/09/06   hrk     Ignore START, TUNEAWAY cmd when attempt not in progress.
09/25/06   hrk     In Hybrid Access disable Tx/PA only if attempt in progress.
09/18/06   hrk     No support for tune away during access attempt in FTM mode.
09/15/06   etv     Initialized gaup_attribs unconditionally in powerup-init.
09/13/06   hrk     Fixed reporting of access attempt duration.
08/23/06   hal     Added support to reset the current access attempt.
06/28/06   jyw     Added support to MPC.
06/16/06   hrk     Fixed featurization error.
06/07/06   hrk     Support for tune away during access.
05/09/06   hrk     Fixed persistence backoff calculation & featurization error.
04/12/06   hal     Fixed compiler/lint warnings
03/20/06   hrk     Support for fixing access probe rate.
03/03/06   hrk     #ifdef cleanup, fixed buffer overflow at higher access rates
02/14/06   hrk     Fixed PA not turning off at frame offset for 4 slot preamble
02/03/06   hrk     Fixed Pilot Strength Correction computation error.
01/13/06   kss     Added hdramac_update_persist_index() (needed for RUIM).
12/13/05   pba     Modified SCMDB calls to use updated API's
12/06/05   etv     Added support for Session Negotiation using SCM.
11/30/05   hrk     Enhanced ACMAC updates
10/06/05   dna     Inform SD if PSIST value makes HDR unusable
10/04/05   pba     Added support to read/write config parameters from SCMDB/EFS
09/08/05   hrk     Addendum changes.
08/01/05   hrk     Debug msg for RETAP-Enhanced ACMAC interface.
07/19/05   hrk     Fixed Rev A. featurization.
07/14/05   hrk     Fixed Rev A. featurization.
07/08/05   hrk     Rev A changes for Enhanced Access Channel MAC.
06/16/05   pba     Fixed ACMAC maximum payload bytes calculation.
03/03/05   etv     Added the member transaction_id back to the struct
                   hdramac_attempt_log_type.
01/25/05   etv     Cleaned lint errors. 
01/12/05   kss     Ignore AC ACK if probe is not in progress.
11/03/04   mpa     Removed dependency on std.h (merged with sys.h)
10/20/04   kss     Use hdrmod interface to enable/disable frame interrupt.
10/14/04   mpa     Update the SLP cb queue before abort in persistence state.
09/09/04   kss     Process abort in persistence state.
08/31/04   kss     Added events for persistence pass/fail.
07/14/04   kss     Fixed typo.
02/23/04   mpa     Added payload to transmission failed indication
01/08/04   kss     Add command ignored event if activate is called when active;
                   include state parameter in indication event.
12/18/03   kss     Store and display ACCOLC value used for access attempt.
11/04/03   kss     Corrected time to wait for ACK in last probe of attempt.
10/21/03   kss     Include rf.h instead of rfhdr.h.
10/14/03   kss     Set RRI to zero at end of probe; print AGC on F3 during
                   probe.
09/16/03   mpa     Converted F3 messages to use new MSG2.0 SSID
09/16/03   kss     Add featurized support for new ACCOLC mapping.
07/30/03   kss     Added MSM6500 support.
05/23/03   kss     Added F3 messaging for ATI, color code & sectorID
10/30/02   kss     Added ACCOLC->HDR Access Persistence index mapping.
10/27/02   kss     Added support for FTM mode.
10/24/02   kss     Fixed delay in starting the 1s timer after receiving AcAck.
10/01/02   kss     Added support for logging the AC MAC channel capsule.
08/30/02   kss     Synchronize deactivate with the hdrmc task to avoid race
                   condition where search and mac layer get out of sync.
03/25/02   kss     Don't count an interrupted access attempt as a failure.
01/07/02   kss     Renamed several variables; corrected problem where first
                   probe could Tx at wrong time (rarely).
12/08/01   kss     Don't fail after max persistence attempts.
12/05/01   kss     Compute probe start frame correctly for cycleDurations
                   less then 16.
12/03/01   kss     Fixed problem where ACMAC would not stop (abort) if it was
                   waiting for Access Parameters.
11/30/01   om      Added security layer support
11/28/01   kss     Removed probe sequence max=0 check.
11/16/01   kss     Fixed sign extention problem in F3 message, slight change
                   in RF interface for tx gain adjust.
11/10/01   kss     Fixed sign extention problem in ap_msg.
11/06/01   kss     Set nominal power in RF; fixed problem where PN was logged
                   incorrectly.
10/29/01   mpa     Modified hdramac_reset_access_stats() to send a log packet
                   after reset is performed on the target.
10/25/01   kss     Added support for silence interval; renamed global var to
                   be consistent with other files; added limit for persist
                   tests per standard; imported declarations from header file.
09/17/01   kss     Moved dsm_free_packet() to hdramac_attempt_cleanup().
09/10/01   kss     Added hdramac_reset_access_stats() function.
09/07/01   kss     Added hdramac_set_silence_params() function.
09/05/01   dna     Added separate handling for in-use vs. in_config messages.
08/24/01   kss     Made config support like other MAC files.
08/22/01   kss     Added config support. Changed hdramac_abort() to
                   hdramac_stop().
08/13/01   kss     Call hdrpcp_get_pkt() after successful probe so SLP call
                   backs function properly.
07/16/01   kss     The AMAC will now wait for the AP parameters to be updated
                   (if not current) rather than aborting right away.
06/21/01   kss     Place DSP in access mode before sending a probe.
06/07/01   kss     Fixed problem where frame offset would not be set
                   after the first probe.
06/06/01   dna     new param for hmp_send_msg()
06/05/01   kss     Set data gain registers in DSP.
05/25/01   kss     Integration changes and other fixes.  Added support for
                   multiple access capsule fragments, fcs computation, among
                   others.
05/07/01   kss     Various functional changes (such as adding the scheduler)
                   for IS-856.  More to come.
04/25/01   dna     Set access_attempt_in_progress to FALSE before giving
                   TxEnded indication. Call hdrpcp_get_pkt() before giving
                   TxAborted or TxStopped indications.
04/23/01   kss     Added hdramac_abort() handling.
04/19/01   kss     Corrected bug where both failure and an abort indication
                   would be sent on failure.
01/26/01   dna     Don't TRACE hdramac_is_active and HDRTX_AMAC_ISR_SIG
12/11/00   kss     Restructured several areas; code is essentially the same
                   but has been moved to separate functions.
                   Hdramac_start_attempt() is now used to start an access
                   attempt.
11/03/00   dna     Added HDR event logging
03/30/00   st      Created.

===========================================================================*/

/* EJECT */

/*===========================================================================

                         INCLUDE FILES FOR MODULE

===========================================================================*/

/* General include files */
#include "hdr_variation.h"
#include <memory.h>
#include "comdef.h"
#include "dsm.h"
#include "err.h"
#include "hdrdebug.h"
#include "ran.h"
#include "rex.h"
#include "rf.h"
#include "rfm_hdr.h"
#include "task.h"

#ifdef FEATURE_MEMCPY_REMOVAL
#include "stringl.h"
#endif /* FEATURE_MEMCPY_REMOVAL */

/* HDR protocol-related include files */
#include "hdrhai.h"
#include "hdrind.h"
#include "hdrhmp.h"
#include "hdrpcp.h"
#include "hdramp.h"
#include "hdrmci.h"
#include "hdrrmac.h"
#include "hdrscp.h"
#include "hdrsec.h"
#include "hdrmc.h"

/* Other HDR-specific include files */
#include "hdrbit.h"
#include "hdrbuf.h"
#include "crc.h"
#include "hdrdsm.h"
#include "hdrenc.h"
#include "hdrlogi.h"
#include "hdrmod.h"
#include "hdrsrch.h"
#include "hdrts.h"
#include "hdrtx.h"
#include "hdrtxi.h"
#include "hdrerrno.h"
#include "hdrtrace.h"
#include "hdrutil.h"
#include "hdramac.h"
#include "hdramaci.h"
#include "hdrmdspmcr.h"
#include "hdrmsg.h"

#include "hdrscmamac.h"
#include "hdrscmdb.h"

#ifdef FEATURE_HDR_REVA_L1
#include "hdrsrchmac.h"
#endif /* FEATURE_HDR_REVA_L1 */

#ifdef FEATURE_FACTORY_TESTMODE
#include "hdrhitmsg.h"
#include "ftm.h"
#endif


#ifdef FEATURE_HDR_QCHAT
#include "hdrqchatupk_opts.h"
#include "hdrqchatupk.h"
#endif /*FEATURE_HDR_QCHAT*/

#include "hdrsrchrf.h"

#ifdef FEATURE_HDR_SELECTIVE_TXD
#include "hdrsrchmac.h"
#endif /* FEATURE_HDR_SELECTIVE_TXD */

#include "hdr_log_qsh.h"
#include "hdrutil.h"
/* EJECT */
/*===========================================================================

                   DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions and declarations for constants, macros,
types, variables and other items needed by this module.

===========================================================================*/

/* Access attempt parameters. */
hdramac_struct_type hdramac;

/* Local packet buffer.  Note that buffer should be at least 
   physical frame size, including CRC/padding fields (hdrenc requirement). */
#ifdef FEATURE_HDR_REVA_L1
LOCAL byte hdramac_pkt[ HDRAMAC_PKT_38K4_FRAME_DATA_LEN + 
                        HDRAMAC_PKT_CRC_TRAILER_LEN];
#else
LOCAL byte hdramac_pkt[ HDRAMAC_PKT_FRAME_DATA_LEN + 
                        HDRAMAC_PKT_CRC_TRAILER_LEN];
#endif /* FEATURE_HDR_REVA_L1 */

/* Statistics for logging */
hdramac_attempt_log_type hdramac_attempt_log;

/* Statistics for logging enhanced access attempt */
LOG_HDR_ENH_ACCESS_ATTEMPT_C_type hdramac_enh_attempt_log;

/* Statistics for logging access probe information */
LOG_HDR_ACCESS_PROBE_C_type hdramac_probe_log;

/* Copy of the Access Parameters message. */
hdrovhd_ap_msg_type hdramac_ap_msg;

/* Used for gain display/logging. Indicates whether it is the
   first data set for a probe. */
boolean hdramac_first_gain_log = FALSE;

/* Access probe I & Q mask permutation array. */
LOCAL const byte hdramac_permutation_array[] =
{
   0, 31, 22, 13,  4, 26, 17,  8, 30, 21, 12,  3, 25, 16,  7, 29,
  20, 11,  2, 24, 15,  6, 28, 19, 10,  1, 23, 14,  5, 27, 18,  9
};

/* Arrays for generating first bit of Q mask */
/* Bit positions for low 32 bits */
LOCAL const byte hdramac_q_lo_xor_array[] =
{
   0, 1, 2, 4, 5, 6, 9, 15, 16, 17, 18, 20, 21, 24, 25, 26, 30
};

/* Bit positions for high 32 bits (relative to 32-bit word boundary) */
LOCAL const byte hdramac_q_hi_xor_array[] =
{
  32-32, 34-32, 41-32
};

/* Binary fraction representation of 2^(-n/4), for n=1..4
   All other values can be derived from these by shifting
   To produce a persistance value, for n=1 to 63, use
   persist_array[(n-1)%4] >> ((n-1)/4)                */
#ifdef FEATURE_HDR_QCHAT
const uint16 hdramac_persist_array[] =
{
  0xd700,  /* n=1 */
  0xb500,  /* n=2 */
  0x9800,  /* n=3 */
  0x8000   /* n=4 */
};
#else
LOCAL const uint16 hdramac_persist_array[] =
{
  0xd700,  /* n=1 */
  0xb500,  /* n=2 */
  0x9800,  /* n=3 */
  0x8000   /* n=4 */
};
#endif /*FEATURE_HDR_QCHAT*/

/* Macro for computing persistence value from index */
#define HDRAMAC_COMPUTE_PERSISTANCE(index)         \
         ((hdramac_persist_array[(index-1)%4] >> ((index-1)/4)) << 16)

/* Table to help compute max persistence tests */
LOCAL const uint16 hdramac_persistence_limits[] =
{
  10000,    /* 2^(0/4) * 10000 */
  11892,    /* 2^(1/4) * 10000 */
  14142,    /* 2^(2/4) * 10000 */
  16818     /* 2^(3/4) * 10000 */
};

/* Array for translating period to frames
   (Note:  We could also use ((0x0800 << period) - 1) )
*/
LOCAL const uint16 hdramac_silence_period[] =
{
  2047,
  4095,
  8191,
  16383
};

#ifdef FEATURE_HDR_REVA_L1

/* Table for PilotStrengthNominal encoding */
LOCAL const int8 hdramac_pilot_str_nom_val[] =
{
  0, -1, -2, -3, -4, 1, 2, 3
};

/* Table for PilotStrengthCorrectionMin encoding */
LOCAL const int8 hdramac_pilot_str_corr_min_val[] =
{
  0, -1, -2, -3, -4, -5
};

/* Table for PilotStrengthCorrectionMax encoding */
LOCAL const int8 hdramac_pilot_str_corr_max_val[] =
{
  0, 1, 2, 3, 4, 5
};


/* GAUPable configuration attributes of AMAC */
LOCAL const hdrscmamac_attrib_id_enum_type 
  hdramac_gaup_attrib_ids[HDRSCMAMAC_NUM_GAUPABLE_ATTRIBS] = 
{
  HDRSCMAMAC_AT_CLASS_OVERRIDE_ATTRIB,      /* Only AN GAUP supported */
  HDRSCMAMAC_TERM_ACCESS_RATE_MAX_ATTRIB,   /* Only AN GAUP supported */
  HDRSCMAMAC_APERSISTENCE_OVERRIDE_ATTRIB   /* Both AT and AN GAUP supported */
};

/* AMAC cares only about the version numbers of the GAUPable attributes to find
   out the attributes changed due to recent GAUP. This table maintains the
   offset of each GAUPable attribute's version number (in the same order as the
   table "hdramac_gaup_attrib_ids") in the master version number array */
LOCAL const uint16
  hdramac_gaup_attrib_verno_offsets[HDRSCMAMAC_NUM_GAUPABLE_ATTRIBS] = 
{
  HDRSCMAMAC_AT_CLASS_OVERRIDE_VERNO_OFFSET,   /* AccessTerminalClassOverride */
  HDRSCMAMAC_TERM_ACCESS_RATE_MAX_VERNO_OFFSET,/* TerminalAccessRateMax */
  HDRSCMAMAC_AP_OVERRIDE_VERNO_OFFSET          /* ApersistenceOverride */
};

/* Wait in slots before scheduling access probe. */
uint32 hdramac_initial_slot_wait = 0;

/* Forward function declaration */
void hdramac_process_access_attempt ( void );

#ifdef FEATURE_HDR_HYBRID_ACCESS

void hdramac_process_tuneaway_stop_access 
(
  hdramac_ta_stop_tx_cb_type tx_stopped_cb
);

void hdramac_tuneaway_resume_access (void);

#endif /* FEATURE_HDR_HYBRID_ACCESS */

/* Logging/displaying of channel gains during access */
void hdramac_process_mdsp_log_event
(  
  hdrlog_mdsp_slot_logging_data_type *sl_params
);

#ifdef FEATURE_FACTORY_TESTMODE
extern rex_tcb_type *hdrmc_tcb_ptr;
#endif /* FEATURE_FACTORY_TESTMODE */

/* EJECT */
/*===========================================================================

FUNCTION HDRAMAC_ENABLE_CARRIER_ACCESS_M0DE

DESCRIPTION 
  Sends message to firmware to enanable the carrier enter access mode,
  and set the data channel gains for access data rates.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdramac_enable_carrier_access_mode( void )
{

  hdrfw_msg_u fw_cmd;
    /* Firmware command */

  errno_enum_type estatus;
    /* Error status */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  
  /* Initialize message for firmware RL Gain Offset command -- we need to
     set the gains for RRIs 1,2 and 3. */
  memset ( &fw_cmd, 0, sizeof(hdrfw_msg_u) );
  msgr_init_hdr ( &fw_cmd.hdr, MSGR_HDR_AMAC, 
                  HDR_FW_RMAC0_GAIN_OFFSET_CFG_CMD );

  /* Set data gain ratio for rates 1 (9.6), 2 (19.2), and 3 (38.4) */
  fw_cmd.rmac0_gain_offset_cfg.dataToPilot[REVERSE_RATE_9600] = 
    (HDRAMAC_DATA_NOM_GAIN_9K6 +
      (hdramac.config->power_params.data_offset_nom * 2) +
      hdramac.config->power_params.data_offset_9k6);

  fw_cmd.rmac0_gain_offset_cfg.dataToPilot[REVERSE_RATE_19200] = 
    (HDRAMAC_DATA_NOM_GAIN_19K2 +
      (hdramac.config->power_params.data_offset_nom * 2) +
      hdramac.config->power_params.data_offset_19k2);

  fw_cmd.rmac0_gain_offset_cfg.dataToPilot[REVERSE_RATE_38400] = 
    (HDRAMAC_DATA_NOM_GAIN_38K4 +
      (hdramac.config->power_params.data_offset_nom * 2) +
      hdramac.config->power_params.data_offset_38k4);

  /* Send the message */
  estatus = msgr_send ( &fw_cmd.hdr, sizeof(hdrfw_msg_u) );
  ASSERT ( estatus == E_SUCCESS );


  /* Initialize message for firmware RL Config command -- we need to
     set FW to Access mode. */
  memset ( &fw_cmd, 0, sizeof(hdrfw_msg_u) );
  msgr_init_hdr ( &fw_cmd.hdr, MSGR_HDR_AMAC, HDR_FW_REL0_RL_CFG_CMD );

  /* Inform DSP that we will be sending access probes.
     DSP will set ACK and DRC channel gains to 0, as well as control
     the preamble power appriately. */
  fw_cmd.rel0_rl_cfg.txAgcAccessMode = 1;
  fw_cmd.rel0_rl_cfg.fieldMask.txAgcAccessMode_Valid = 1;

  /* Send the message */
  estatus = msgr_send ( &fw_cmd.hdr, sizeof(hdrfw_msg_u) );
  ASSERT ( estatus == E_SUCCESS );

  /* Initialize message for firmware carrier enable command -- we need to
     enable carrier 0. */
  memset ( &fw_cmd, 0, sizeof(hdrfw_msg_u) );
  msgr_init_hdr ( &fw_cmd.hdr, MSGR_HDR_RMAC, 
    HDR_FW_RMAC_ENABLE_CARRIER_CMD );

  /* Enable carrier 0 in FW. */
  fw_cmd.rmac_enable_carrier.carrierMask = 1;
  fw_cmd.rmac_enable_carrier.modulatorConfig[0].enable = 1;
  fw_cmd.rmac_enable_carrier.modulatorConfig[0].carrierMask = 1;

  /* Send the message */
  estatus = msgr_send ( &fw_cmd.hdr, sizeof(hdrfw_msg_u) );
  ASSERT ( estatus == E_SUCCESS );
}

/*===========================================================================

FUNCTION HDRAMAC_DISABLE_CARRIER_ACCESS_M0DE

DESCRIPTION 
  Sends message to firmware to disable the carrier and exit access mode. 

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdramac_disable_carrier_access_mode( void )
{

  hdrfw_msg_u fw_cmd;
    /* Firmware command */

  errno_enum_type estatus;
    /* Error status */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Initialize message for firmware carrier config command */
  memset ( &fw_cmd, 0, sizeof(hdrfw_msg_u) );
  msgr_init_hdr ( &fw_cmd.hdr, MSGR_HDR_RMAC, 
    HDR_FW_RMAC_ENABLE_CARRIER_CMD );

  /* Disable carrier 0 in FW. */
  fw_cmd.rmac_enable_carrier.carrierMask = 1;
  fw_cmd.rmac_enable_carrier.modulatorConfig[0].enable = 0;

  /* Send the message. */
  estatus = msgr_send ( &fw_cmd.hdr, sizeof(hdrfw_msg_u) );
  ASSERT ( estatus == E_SUCCESS );

  /* Initialize message for firmware RL Config command */
  memset ( &fw_cmd, 0, sizeof(hdrfw_msg_u) );
  msgr_init_hdr ( &fw_cmd.hdr, MSGR_HDR_RMAC, 
    HDR_FW_REL0_RL_CFG_CMD );

  /* Inform DSP that we are no longer sending access probes. */
  fw_cmd.rel0_rl_cfg.txAgcAccessMode = 0;
  fw_cmd.rel0_rl_cfg.fieldMask.txAgcAccessMode_Valid = 1;

  /* Send the message. */
  estatus = msgr_send ( &fw_cmd.hdr, sizeof(hdrfw_msg_u) );
  ASSERT ( estatus == E_SUCCESS );
}
/*===========================================================================

FUNCTION HDRAMAC_RECORD_VERSION_NUMS

DESCRIPTION
  The Configuration attribute version numbers are needed only for GAUPable
  attributes in order to query the SCM DB and find out the updated attribs. 
  This function is used to store the version number of GAUPable attributes
  into static memory.

DEPENDENCIES
  None.

PARAMETERS
  verno - Array of version numbers one for each AMAC config attrib.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
LOCAL void hdramac_record_version_nums
(
  hdrscmamac_verno_type verno
)
{

  uint8 g_attr_ind;
    /* GAUP attribute index in the version number array */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */ 

  /* Store the version numbers of GAUPable attributes */
  for (g_attr_ind=0; g_attr_ind<HDRSCMAMAC_NUM_GAUPABLE_ATTRIBS; ++g_attr_ind)
  {
    hdramac.gaup_attrib_verno[g_attr_ind] = 
      verno[hdramac_gaup_attrib_verno_offsets[g_attr_ind]];
  }

} /* hdramac_record_version_nums */


/* EJECT */
/*===========================================================================

FUNCTION HDRAMAC_PROCESS_ATTRIB_UPDATE

DESCRIPTION
  This function processes the GAUP Update indication by querying SCM to find
  the updated attributes and gets the values of updated attributes and takes
  appropriate action.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdramac_process_attrib_update(void)
{

  boolean is_changed[HDRSCMAMAC_NUM_GAUPABLE_ATTRIBS];
    /* Has the GAUPable attribute changed since last read? */

  uint16 g_attr_ind;
    /* GAUPable attribute index. */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */  

  /* Query SCM DB to find the changed attributes */
  hdrscmdb_find_changed_attribs( HDRHAI_AC_MAC_PROTOCOL,
                                 HDRSCMAMAC_NUM_GAUPABLE_ATTRIBS,
                                 hdramac.gaup_attrib_verno,
                                 hdramac_gaup_attrib_verno_offsets,
                                 is_changed  );

  for (g_attr_ind = 0; g_attr_ind < HDRSCMAMAC_NUM_GAUPABLE_ATTRIBS; ++g_attr_ind)
  {
    if (is_changed[g_attr_ind])
    {
      /* Get the Updated attribute value from the SCM Database */
      hdramac.gaup_attrib_verno[g_attr_ind] = 
      hdrscmdb_get_inuse_attrib( HDRHAI_AC_MAC_PROTOCOL,
                                 hdramac_gaup_attrib_ids[g_attr_ind], 
                                 hdramac.gaup_attribs[g_attr_ind],
                                 HDRSCP_ENH_ACMAC );

      /* TODO Harsha to fill in the action for each GAUP! */
      switch (hdramac_gaup_attrib_ids[g_attr_ind])
      {
        case HDRSCMAMAC_AT_CLASS_OVERRIDE_ATTRIB:
          hdramac.config->access_terminal_class_override = 
            hdramac.scm_cfg.at_class_override;
          break;

        case HDRSCMAMAC_TERM_ACCESS_RATE_MAX_ATTRIB:
          hdramac.config->terminal_access_rate_max = 
            hdramac.scm_cfg.term_acc_rate_max;
          break;

        case HDRSCMAMAC_APERSISTENCE_OVERRIDE_ATTRIB:
          hdramac.config->apersistence_override = 
            hdramac.scm_cfg.ap_override;
          break;

        default:
          HDR_MSG_PROT(MSG_LEGACY_ERROR,
                       "Invalid attrib GAUPed... Ignored");
                         
      }
    }
  }

} /* hdramac_process_attrib_update */

#endif /* FEATURE_HDR_REVA_L1 */


/* EJECT */
/*===========================================================================

FUNCTION HDRAMAC_TRANSLATE_CONFIG_PARAMS

DESCRIPTION

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdramac_translate_config_params(void)
{

  /* TODO unit conversions... */

  if (hdramac.current_subtype == HDRSCP_ENH_ACMAC)
  {
    /* Simple attributes */
    hdramac.config->pn_delay_supported =
      hdramac.scm_cfg.pn_delay_supported;
    hdramac.config->access_terminal_class_override = 
      hdramac.scm_cfg.at_class_override;
    hdramac.config->apersistence_override = 
      hdramac.scm_cfg.ap_override;
    hdramac.config->terminal_access_rate_max = 
      hdramac.scm_cfg.term_acc_rate_max;

    /* Complex attributes */
    hdramac.config->power_params.data_offset_nom = 
      hdramac.scm_cfg.data_offset_nom;
    hdramac.config->power_params.data_offset_9k6 = 
      hdramac.scm_cfg.data_offset_9k6;
    hdramac.config->power_params.data_offset_19k2 = 
      hdramac.scm_cfg.data_offset_19k2;
    hdramac.config->power_params.data_offset_38k4 = 
      hdramac.scm_cfg.data_offset_38k4;
  }
  else /* HDRSCP_DEFAULT_ACMAC */
  {
    /* No simple attributes in Default ACMAC */

    /* Complex attributes */
    /* Power Parameters */
    hdramac.config->power_params.data_offset_nom = 
      hdramac.scm_cfg.power_params.data_offset_nom;
    hdramac.config->power_params.data_offset_9k6 = 
      hdramac.scm_cfg.power_params.data_offset_9k6;
  }

  /* Initial Configuration attributes */
  hdramac.config->initial.probe_backoff = 
    hdramac.scm_cfg.init_config.probe_backoff;
  hdramac.config->initial.probe_sequence_backoff = 
    hdramac.scm_cfg.init_config.probe_seq_backoff;
  hdramac.config->initial.probe_sequence_max = 
    hdramac.scm_cfg.init_config.probe_seq_max;

} /* hdramac_translate_config_params */


/* EJECT */
/*===========================================================================

FUNCTION HDRAMAC_SET_IQ_MASK

DESCRIPTION
  This function computes and sets the IQ mask.

DEPENDENCIES
  Hdramac_schedule() must have been called for the access cycle we are
  going to transmit in.  hdramac.sector_id, .color_code, and _cycle_num
  must be valid.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdramac_set_iq_mask( void )
{

  /* Loop variable */
  uint32 i;

  /* Permuted color code & sector id for creating I mask */
  uint32 permuted_i_mask = 0;

  /* Working regs */
  uint32 xor = 0, i_shift, i_mask;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Low 32 bits of the I mask */
  i_mask = (hdramac.color_code << 24) | hdramac.sector_id;

  /* Permute the ATI value to determine the User Long Code Mask. */
  for( i=0; i < sizeof(hdramac_permutation_array); i++ )
  {
    if( i_mask & (0x01 << hdramac_permutation_array[i]) )
    {
      permuted_i_mask |= (0x80000000UL >> i);
    }
  }

  /* Set the Long Code Mask for the Access Channel
     (based on pilot PN and sector ID) */
  qw_set(hdramac.i_mask,
         HDRAMAC_I_MASK_UPPER_BITS | hdramac.pn_cycle_num,
         permuted_i_mask);

  /* Compute bit 0 of the Q mask, which is based on the XORing of
     several of the I mask bits. */

  /* First do low bits. */
  i_mask = qw_lo(hdramac.i_mask);
  for (i=0;i < sizeof(hdramac_q_lo_xor_array); i++)
  {
    i_shift = hdramac_q_lo_xor_array[i];
    xor ^= ((i_mask >> i_shift) & 0x00000001);
  }

  /* Now do high bits. */
  i_mask = qw_hi(hdramac.i_mask);
  for (i=0;i < sizeof(hdramac_q_hi_xor_array); i++)
  {
    i_shift = hdramac_q_hi_xor_array[i];
    xor ^= ((i_mask >> i_shift) & 0x00000001);
  }

  /* Q mask is based on the I mask, shifted over, and adding the XOR bit */
  qw_set(hdramac.q_mask, qw_hi(hdramac.i_mask),
               qw_lo(hdramac.i_mask));

  qw_shift(hdramac.q_mask, 1);  /* Shift to left */

  /* OR in bit 0 */
  qw_set(hdramac.q_mask, qw_hi(hdramac.q_mask) & 0x000003FF,
      (qw_lo(hdramac.q_mask) | xor));

  /* Load mask into modulator. */
  hdrmod_set_pn_long_code_mask( hdramac.i_mask,
                                hdramac.q_mask
#ifdef FEATURE_HDR_BOLT_MODEM
                                ,1  /* Enable carrier 0 PN generator */
#endif /* FEATURE_HDR_BOLT_MODEM */
    );

  /* Show information used to create PN */
  HDR_MSG_PROT_1( MSG_LEGACY_HIGH, "Probe: Cycle=0x%x",hdramac.pn_cycle_num );
  HDR_MSG_PROT_4( MSG_LEGACY_MED, 
                        " I: 0x%x %x  Q: 0x%x %x",
                          qw_hi(hdramac.i_mask),
                          qw_lo(hdramac.i_mask),
                          qw_hi(hdramac.q_mask),
                          qw_lo(hdramac.q_mask));
}


/* EJECT */
/*===========================================================================

FUNCTION HDRAMAC_COMPUTE_PERSISTENCE_LIMIT

DESCRIPTION
  Computes the maximum number of times the persistence test may be run before
  an automatic pass.

DEPENDENCIES
  None.

PARAMETERS
  n - Persistence value from AP message.

RETURN VALUE
  perist_limit - The max peristence tests allowed by standard.

SIDE EFFECTS
  None.

===========================================================================*/
uint32 hdramac_compute_persistence_limit(
  int n
    /* persistence val */
)
{
  uint32 persist_limit;      /* Persistence test limit */
  uint32 bits;               /* Helper for determining limit */

  /* -----------------------------------------------------------
     Maximum number of persistence tests is:  4/(2^(-n/4))
     Compute using integer math and a table.
     4 / (2^(-n/4)) = 4 * 2^(n/4) = 2^2 * 2^(n/4)

     if x = int(n/4) and y = (n%4) we have

     2^2 * 2^(x+(y/4)) = 2^(2+x) * 2^(y/4) where y is 0,1,2,3

     Use bit shifting to determine 2^(2+x) and a lookup table
     for 2^(y/4).  (Table values are *10000)
  ----------------------------------------------------------- */

  /* Find 2^(2+x) */
  bits = (uint32) ((n/4) + 2);
  persist_limit = 0x01 << bits;

  /* Multiply the result by 2^(y/4). */
  persist_limit *= hdramac_persistence_limits[(n%4)];
  persist_limit /= 10000;

  /* Return max number of tests allowed.  */
  return persist_limit;
}


/* EJECT */
/*===========================================================================

FUNCTION HDRAMAC_COMPUTE_NEXT_SILENCE_INTERVAL

DESCRIPTION
  Computes the next Reverse Silence Interval time

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdramac_compute_silence_time( void )
{
  uint32 frame;
  qword  diff;
  qword  silence_cycles;
  qword  silence_time;
  qword  current_frametime;

  /* Set silence_cycles to current cycle number; slots to number of
     slots into the cycle we are currently at */
  (void) hdrts_get_frame_time( current_frametime );

  /* find current silence cycle number from UTC=0 
     in units of silence period duration */
  (void) qw_div(silence_cycles, current_frametime, (uint16) hdramac.silence_period);

  /* Find frame number corresponding to begining of current cycle from UTC=0 */
  qw_mul( silence_time, silence_cycles, hdramac.silence_period);

  /* Find current frame into current silence cycle */
  qw_sub(diff, current_frametime, silence_time);

  frame = qw_lo(diff);

  qw_equ( hdramac.silence_begin_time, silence_time);

  /* Check if we are currently in a reverse silence cycle */
  if (frame >= (hdramac.silence_duration))
  {
    qw_inc( hdramac.silence_begin_time, hdramac.silence_period );
  }

  HDR_MSG_PROT_3( MSG_LEGACY_LOW, "Next Silence Int: %x %x (%d)",
                  qw_hi(hdramac.silence_begin_time),
                  qw_lo(hdramac.silence_begin_time),
                  frame );

}

/* EJECT */
/*===========================================================================

FUNCTION HDRAMAC_CHECK_REVERSE_SILENCE_WAIT

DESCRIPTION
  Computes whether the ACMAC must wait before sending a probe due
  to a conflict with the next RSI (Reverse Silence Interval).

DEPENDENCIES
  None.

PARAMETERS
  time - The frame time to check.

RETURN VALUE
  wait - If there is a conflict, the number of frames to wait.

SIDE EFFECTS
  None.
===========================================================================*/
uint32 hdramac_check_reverse_silence_wait
(
  qword time
    /* The frametime to check */
)
{
  qword temp;
  qword qdiff;
  int cmp;
  uint32 diff;    /* Working regs for computation */
  uint32 wait = 0;   /* Return value */

  /* If duration is zero, there is no conflict */
  if (hdramac.silence_duration != 0)
  {

    qw_equ(temp, time);

    /* Compensate for setup frames */
    qw_inc(temp, 2);

    /* Determine if there is overlap between probe and RSI */
    cmp = qw_cmp(hdramac.silence_begin_time, temp);

    /* If silence_begin is >= time */
    if (cmp>=0)
    {
      qw_sub(qdiff, hdramac.silence_begin_time, temp);
      diff = qw_lo(qdiff);
      if (diff<hdramac.probe_len)
      {
        wait = diff + hdramac.silence_duration;
      }
    }
    else /* If silence_begin is < time */
    {
      qw_sub(qdiff, temp, hdramac.silence_begin_time);
      diff = qw_lo(qdiff);
      if (diff<hdramac.silence_duration)
      {
        wait = hdramac.silence_duration - diff;
      }
    }
  }

  return wait;
}

/* EJECT */
/*===========================================================================

FUNCTION HDRAMAC_SET_SILENCE_DURATION                               EXTERNAL

DESCRIPTION
  This function sets the AMAC silence duration and offset.

DEPENDENCIES
  The parameter must not be NULL.

PARAMETERS
  silence_duration
  silence_period

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdramac_set_silence_params
(
  uint8 silence_duration,
  uint8 silence_period
)
{

  REX_DISABLE_PREMPTION();  
    /* Protect during update. Ultimately this function should be a 
       command and this could be removed. */
   
  hdramac.silence_duration = silence_duration;
  hdramac.silence_period = hdramac_silence_period[silence_period];
    /* Copy the data */

  REX_ENABLE_PREMPTION();

  HDR_MSG_PROT_3( MSG_LEGACY_MED, "Silence dur, per: %d, %d (%d)",
          silence_duration, silence_period, hdramac.silence_period);

}

/* EJECT */
/*===========================================================================

FUNCTION HDRAMAC_SCHEDULER

DESCRIPTION
  This function schedules a time for the AMAC_ISR signal to be set.  This
  time is given as a number of access cycles + a number of slots from the
  current time.  The event will occur at the first access cycle boudary that
  meets the cycle & slot requirements. The signal can be set a specific
  number of frames early for setup.

DEPENDENCIES
  The hdramac_ap_msg structure must be valid  (cycle duration needed).

PARAMETERS
  min_cycles   - Minimum number of access cycles before event.
  min_slots    - Minimum number of slots before event.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdramac_scheduler
(
  uint32 min_cycles,   /* Minimum number of cycles before event may occur */
  uint32 min_slots    /* Minimum number of slots before event.
                          (Accumulative with cycles.) */
)
{

  uint8  cycle_duration;    /* Local copy of access cycle duration (in slots)*/
  qword  access_cycles;     /* Number of access cycles since time began */
  uint32 slots;             /* Slot number into current access cycle */
  uint32 slots_remaining;   /* Slots until next access cycle */
  uint8 slot_offset;       /* The Tx frame offset needed for boundary */
  qword  scheduled_time;    /* The scheduled time */

  /* Number of frames to wake up early. */
  uint16 setup_frames = HDRAMAC_TX_SETUP_FRAMES;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if ( !hdramac_is_active(HDRHAI_AC_MAC_PROTOCOL) )
  {
    HDR_MSG_PROT( MSG_LEGACY_ERROR, "Ignoring scheduler when AMAC is inactive");
    return;
  }

  HDR_MSG_PROT_2( MSG_LEGACY_LOW, "Scheduler: Cycles %d, Slots %d",
                  min_cycles, min_slots );

  /* Get local copy of the access cycle duration. */
  cycle_duration = hdramac_ap_msg.access_cycle_duration;
  if (cycle_duration == 0) cycle_duration = 16;

  /* If the setup frames require more time than the cycles & slots asked for,
     increase the min_slots value accordingly. We *must* have enough time to
     setup */

  if ((uint32)(setup_frames*16) > ((min_cycles*cycle_duration)+min_slots))
  {
    min_slots += (setup_frames*16) - ((min_cycles*cycle_duration)+min_slots);
  }

  /* Get the number of slots to the next access cycle boundary
     (slots_remaining) and the number of access cycles since time
     began. (access_cycles) */
  hdrts_get_cycle_and_slot( access_cycles, &slots, cycle_duration);
  slots_remaining = cycle_duration - slots;

#ifdef FEATURE_HDR_REVA_L1
  if ((hdramac.current_subtype == HDRAMAC_SUBTYPE_ENHANCED) &&
      hdramac_ap_msg.enhanced_access_parameters_included)
  {
    /* actual probe transmission can start only after AccessOffset slots
       from the next access cyle duration. AccessOffset units = 4 slots */
    slots_remaining += (hdramac_ap_msg.access_offset * 4);
  }
#endif /* FEATURE_HDR_REVA_L1 */

  /* Bump up min_cycles according to the number of slots we still need */
  min_cycles++;
  if (slots_remaining < min_slots)
    min_cycles += ((min_slots-slots_remaining)/cycle_duration)+1;

  HDR_MSG_PROT_1( MSG_LEGACY_LOW, "Cycles to wait: %d", min_cycles );

  /* Add min_cycles to get the access cycle in the future that we are
     aiming for. */
  qw_inc(access_cycles, min_cycles);


  /* Convert cycles into slots from UTC=0
     (scheduled_time = access_cycles * cycle duration) */
  qw_mul( scheduled_time, access_cycles, cycle_duration );

#ifdef FEATURE_HDR_REVA_L1
  /* In Rev A, Access Cycle Number  = (SystemTime - AccessOffset) mod 256. 
     SystemTime  = CDMA time in slots corresponding to start of access cycle 
     duration + access offset slots. So Access cycle number = Starting slot 
     of current access cycle duration mod 256.  This is the value in 
     scheduled_time */
#endif /* FEATURE_HDR_REVA_L1 */

  /* Compute and store AccessCycleNumber (mod 256) for PN mask.
     (access_cycle is just a placeholder here) */
  hdramac.pn_cycle_num = (uint8) qw_div(access_cycles, scheduled_time,
                                 HDRAMAC_PN_CYCLE_NUM_MOD) ;

#ifdef FEATURE_HDR_REVA_L1
  /* Add access_offset to the start of the next access cycle duration to 
     obtain the actual probe transmission begin slot */
  if ((hdramac.current_subtype == HDRAMAC_SUBTYPE_ENHANCED) &&
      hdramac_ap_msg.enhanced_access_parameters_included)
  {
    HDR_MSG_PROT_1( MSG_LEGACY_HIGH, "EACMAC Access Offset = %d slots",
                    (hdramac_ap_msg.access_offset * 4));
    qw_inc(scheduled_time, (hdramac_ap_msg.access_offset * 4));
  }
#endif /* FEATURE_HDR_REVA_L1 */

  /* Convert to frames and slots */
  slot_offset = (uint8) qw_div( scheduled_time, scheduled_time, 16);

  /* Compensate for slot_offsets which would put us in the previous
     frame (TODO: make macro) */
  if (slot_offset > 3) setup_frames--;

  /* Decrement schduled_time by the number of setup frames needed. */
  qw_dec(scheduled_time, setup_frames);

  /* Save scheduled time */
  qw_equ(hdramac.scheduled_time, scheduled_time);

  /* Flag that a probe event is scheduled. */
  hdramac.event_scheduled = TRUE;

  /* Show schedule! */
  HDR_MSG_PROT_3( MSG_LEGACY_HIGH, "Scheduling event: 0x%x %x @ %x",
           qw_hi(scheduled_time), qw_lo(scheduled_time), slot_offset);

  /* Save and set frame slot offset for Tx */

  /* slot when probe data Tx begins */
  hdramac.tx_offset = slot_offset;

#ifdef FEATURE_HDR_REVA_L1
  if ((hdramac.current_subtype == HDRAMAC_SUBTYPE_ENHANCED) &&
      hdramac_ap_msg.enhanced_access_parameters_included)
  {
    if (hdramac_ap_msg.preamble_length_slots == 1)
    {
      HDR_MSG_PROT( MSG_LEGACY_HIGH, "EACMAC Preamble Length = 16 slots");
    }
    else
    {
      HDR_MSG_PROT( MSG_LEGACY_HIGH, "EACMAC Preamble Length = 4 slots");
    }
  }
#endif /* FEATURE_HDR_REVA_L1 */

  /* Store the tx start offset (data frame start) time. */
  hdramac.tx_frame_start_offset = slot_offset;
  hdrmod_set_tx_frame_start_offset( hdramac.tx_frame_start_offset );
  hdrmod_set_tx_frame_interrupt_offset( 
    (slot_offset + HDRAMAC_FRAME_INTERRUPT_ENC_SLOT) % 16 );
}



/*===========================================================================

FUNCTION HDRAMAC_POWERUP_INIT

DESCRIPTION
  This function initializes the protocol prior to use.  This function should
  be called once upon system startup.  The protocol configuration is
  initialized and the protocol state is set to Inactive.

DEPENDENCIES
  THIS FUNCTION IS RUN IN HDRMC CONTEXT!

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Protocol configuration is initialized and state is assigned.

===========================================================================*/
void hdramac_powerup_init ( void )
{

  hdrscmamac_verno_type verno; /* Version numbers of configuration attribs */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdramac.config = &hdramac.cfg;

  /* Get the current subtype of AMAC from SCP */
  hdramac.current_subtype = (hdramac_subtypes_type)
    hdrscp_get_current_subtype(HDRHAI_AC_MAC_PROTOCOL);

  /* Get the current value and the version number for each attribute */
  hdrscmdb_get_inuse_config_and_verno( 
    HDRHAI_AC_MAC_PROTOCOL,
    &hdramac.scm_cfg,
    verno,
    HDRSCMAMAC_NUM_CONFIG_ATTRIBS );

  /* Record the version numbers that we need */
  hdramac_record_version_nums(verno);

  /* Initialize the pointers to GAUPable attributes */
  hdramac.gaup_attribs[0] = &hdramac.scm_cfg.at_class_override;
  hdramac.gaup_attribs[1] = &hdramac.scm_cfg.term_acc_rate_max;
  hdramac.gaup_attribs[2] = &hdramac.scm_cfg.ap_override;

  /* Translate the config attributes from SCM definition to AMAC's defn */
  hdramac_translate_config_params();

  /* Init flags */
  hdramac.abort_pending = FALSE;
  hdramac.attempt_in_progress = FALSE;
  hdramac.tx_enabled = FALSE;
  hdramac.override_data_bytes = FALSE;

  /* Update persistence index based on ACCOLC */
  hdramac_update_persist_index();
}



/*===========================================================================

FUNCTION HDRAMAC_UPDATE_PERSIST_INDEX

DESCRIPTION
  This function sets up the HDR AC MAC persistence index based on the 1x
  accolc NV value.

DEPENDENCIES
  THIS FUNCTION IS RUN IN HDRMC CONTEXT.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void hdramac_update_persist_index ( void )
{
  nv_accolc_type accolc;       /* For holding NV item */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Init persist_index */
  hdramac.persist_index = HDRAMAC_PSIST_INDEX;

  /* Set nam for NV access */
  accolc.nam = 0;

  /* Retrieve ACCOLC from NV and map to HDR */
  if( hdrutil_read_nv_item( NV_ACCOLC_I,
                    (nv_item_type*)&accolc ) == NV_DONE_S )
  {
    if(!hdrmc_feature_is_enabled(
       HDRMC_FEATURE_HDR_ACCOLC_ALTERNATE_MAPPING))
    {
        /*------------------------------------------------------------
          ACCOLC is a 4-bit value from NV which is translated to
          an HDR Access Persistence index according to the following:
    
                    ACCOLC     HDR persistence index
                    --------------------------------
                    0-9                 0
                    10,11               1
                    12,13               2
                    14,15               3
    
        ------------------------------------------------------------*/
        static uint8 accolc_to_persist[16] =
        {
          0, 0, 0, 0, 0, /* ACCOLC 0, 1, 2, 3, 4 */
          0, 0, 0, 0, 0, /* ACCOLC 5, 6, 7, 8, 9 */
          1, 1,          /* ACCOLC 10,11         */
          2, 2,          /* ACCOLC 12,13         */
          3, 3           /* ACCOLC 14,15         */
        };
            /* Store ACCOLC value for later display on F3 */
        hdramac.accolc = accolc.ACCOLCpClass[NV_CDMA_MIN_INDEX] & 0x0f;

        /* Convert the ACCOLC value [0-15] to a persistence index [0-3] based
           on above table. */
        hdramac.persist_index = accolc_to_persist[ hdramac.accolc ];
    } /* if(!hdrmc_feature_is_enabled(HDRMC_FEATURE_HDR_ACCOLC_ALTERNATE_MAPPING))*/
    else
    {
        /*------------------------------------------------------------
          ACCOLC is a 4-bit value from NV which is translated to
          an HDR Access Persistence index according to the following:
    
          ACCOLC0 to 9 (Used as general purpose)                 0
          ACCOLC11 (Used for mobile station with priority)       1
          ACCOLC10, 12, 13, 15 (Used for maintenance)            2
          ACCOLC14 (Used as backup)                              3
    
        ------------------------------------------------------------*/
        static uint8 accolc_to_persist[16] =
        {
          0, 0, 0, 0, 0, /* ACCOLC 0, 1, 2, 3, 4 */
          0, 0, 0, 0, 0, /* ACCOLC 5, 6, 7, 8, 9 */
          2,             /* ACCOLC 10            */
          1,             /* ACCOLC 11            */
          2,             /* ACCOLC 12            */
          2,             /* ACCOLC 13            */
          3,             /* ACCOLC 14            */
          2              /* ACCOLC 15            */
        };
            /* Store ACCOLC value for later display on F3 */
        hdramac.accolc = accolc.ACCOLCpClass[NV_CDMA_MIN_INDEX] & 0x0f;

        /* Convert the ACCOLC value [0-15] to a persistence index [0-3] based
           on above table. */
        hdramac.persist_index = accolc_to_persist[ hdramac.accolc ];
    } /* else */
  }
}

/* EJECT */
/*===========================================================================

FUNCTION HDRAMAC_IS_ACTIVE                                           EXTERNAL

DESCRIPTION
  This function queries the current state of the protocol.

DEPENDENCIES
  None.

PARAMETERS
  caller - The protocol calling this externalized function (for sw trace)

RETURN VALUE
  TRUE  = protocol is Active.
  FALSE = protocol is Inactive.

SIDE EFFECTS
  None.

===========================================================================*/
boolean hdramac_is_active 
( 
  hdrhai_protocol_name_enum_type caller 
  /*lint -esym(715,caller)
   * ignores the unused argument(caller) lint error #715.*/
)
{

  return( (hdramacsm_get_state() != HDRAMAC_INACTIVE_STATE) ? TRUE : FALSE );
}

/* EJECT */
/*===========================================================================

FUNCTION HDRAMAC_IS_BUSY                                             EXTERNAL

DESCRIPTION
  This function queries the current state of the protocol.

DEPENDENCIES
  None.

PARAMETERS
  caller - The protocol calling this externalized function (for sw trace)

RETURN VALUE
  TRUE  = protocol is busy.
  FALSE = protocol is idle.

SIDE EFFECTS
  None.

===========================================================================*/
boolean hdramac_is_in_attempt 
(
  hdrhai_protocol_name_enum_type caller 
  /*lint -esym(715,caller)
   * ignores the unused argument(caller) lint error #715 */
)
{
  return( hdramac.attempt_in_progress );
}

/* EJECT */
/*===========================================================================

FUNCTION HDRAMAC_PROCESS_ISR_SIG

DESCRIPTION
  Handles the signal triggered by the Access MAC ISR for Access MAC 
  frame processing. Function checks if either a probe is ongoing, or if 
  an action time has been met and triggers probe processing if so.
  
DEPENDENCIES
  HDR system time must be valid.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdramac_process_isr_sig ( void )
{
  qword current_frametime;
    /* Current time */

  int   comp_result;
   /* Result of comparison between the current time and the scheduled time. */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /*-----------------------------------------------------------------------
   Increment the elapsed frame count each frame.
  -----------------------------------------------------------------------*/
  hdramac.elapsed_frames++;

  /* If no event is scheduled, (meaning a probe is already in progress),
     go ahead and set the signal. */
  if (!hdramac.event_scheduled)
  {
    /* Trigger access processing */
    hdramac_process_access_attempt();
  }
  else /* There is an event scheduled... */
  {
    /* Get the time in frames.
       (We are assuming that HDR time is valid here) */
    (void) hdrts_get_frame_time( current_frametime );

    /* Check if the frame time is equal to or greater than the time
       we're looking for. */
    comp_result = qw_cmp(current_frametime, hdramac.scheduled_time);

    /* If equal, we have reached the appointed time */
    if (comp_result == 0 )
    {
      hdramac.event_scheduled = FALSE;

      HDR_MSG_PROT_2( MSG_LEGACY_LOW, "Scheduled event! 0x%x %x",
                      qw_hi(current_frametime),qw_lo(current_frametime) );

      /* Trigger access processing */
      hdramac_process_access_attempt();
    }
    else if (comp_result > 0)  /* Late, so just reschedule */
    {
      uint32  wait;          /* Frames to wait to avoid RLSI */

      HDR_MSG_PROT_2( MSG_LEGACY_HIGH, "Late for event (0x%x %x). Rescheduling",
                      qw_hi(current_frametime),qw_lo(current_frametime) );

      hdramac_compute_silence_time ();
      wait = hdramac_check_reverse_silence_wait (current_frametime);

      /* reschedule for next available cycle */
      hdramac_scheduler (0, wait*16);
    }
  }
} /* hdramac_process_isr_sig */

/*===========================================================================

FUNCTION HDRAMAC_ISR

DESCRIPTION
  This interrupt service routine sets the corresponding signal for the task
  each time the interrupt occurs.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdramac_isr 
( 
  uint32 unused_param
    /* Parameter */
)
{
  qword current_frametime;
  uint16 current_hstr;
  int cmp;
  static qword previous_rec_frametime = {0,0};
    /* frametime last when AMAC ISR was fired */
  static uint16 previous_rec_hstr = 0;
    /* hstr last when AMAC ISR was fired */

  (void) hdrts_get_frame_time( current_frametime );
  current_hstr = HDRHSTR_GET_COUNT();
  // REVISIT: This is a test debug message.
  HDR_MSG_PROT_4( MSG_LEGACY_HIGH,"AMAC ISR: %d slots, pa %d frame hi 0x%x lo %x", 
                  current_hstr, hdrmod_pa_is_on(),
                  qw_hi(current_frametime),qw_lo(current_frametime) );

  cmp = qw_cmp(current_frametime, previous_rec_frametime);
    /* Compare the frame count between current frame and previous time when
       ISR was fired*/

  /* Make sure we are not processing duplicate ISR fired in same frame
     at same hstr by comparing with previously recorded ISR frame and hstr */
  if ( ( cmp == 0) && ( current_hstr == previous_rec_hstr) )
  {
     HDR_MSG_PROT_3( MSG_LEGACY_HIGH,"AMAC ISR: Ignore handling "
                     "duplicate ISR fired at same time "
                     "prev_slot %d, prev_frame 0x%x lo %x ",
                     previous_rec_hstr,
                     qw_hi(previous_rec_frametime),
                     qw_lo(previous_rec_frametime) );
  }
  else
  {
  /* Trigger tx task to process event */
  (void) rex_set_sigs( HDRAMAC_TASK_PTR, HDRTX_AMAC_ISR_SIG );
}

  previous_rec_frametime[0] = current_frametime[0];
  previous_rec_frametime[1] = current_frametime[1];
  previous_rec_hstr = current_hstr;

}

/* EJECT */
#ifdef FEATURE_HDR_REVA_L1
/*===========================================================================

FUNCTION HDRAMAC_LOG_ENH_ACCESS_ATTEMPT                              INTERNAL

DESCRIPTION
  Logs data relating to an access attempt when enhanced access mode.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdramac_log_enh_access_attempt( void )
{
  uint16                            sub_val;

  /* Figure out how much to shrink the log packet by... */
  sub_val = (HDRLOG_AC_MAX_PROBE_CNT
            - hdramac_attempt_log.probe_sequence_count);

  hdramac_enh_attempt_log.enh_acmac_log.version = 0;
  hdramac_enh_attempt_log.enh_acmac_log.enh_access_params_inc = 0;

  if (hdramac_ap_msg.enhanced_access_parameters_included)
  {
    /* copy legacy log attempt information */
#ifdef FEATURE_MEMCPY_REMOVAL
    memscpy((void*)&(hdramac_enh_attempt_log.enh_acmac_log.rev0_log),
             sizeof(hdramac_enh_attempt_log.enh_acmac_log.rev0_log),
            (void*)&hdramac_attempt_log,
             sizeof(hdramac_attempt_log) - sub_val);
#else
    memcpy((void*)&(hdramac_enh_attempt_log.enh_acmac_log.rev0_log),
           (void*)&hdramac_attempt_log,
           sizeof(hdramac_attempt_log) - sub_val);
#endif /* FEATURE_MEMCPY_REMOVAL */

    /* log information about enhanced access parameters */
    hdramac_enh_attempt_log.enh_acmac_log.preamble_length_slots  =
                       hdramac_ap_msg.preamble_length_slots;
    hdramac_enh_attempt_log.enh_acmac_log.enh_access_params_inc  = 
                       hdramac_ap_msg.enhanced_access_parameters_included;
    hdramac_enh_attempt_log.enh_acmac_log.probe_pwr_adjust       =
                       hdramac.probe_init_adjust;
    hdramac_enh_attempt_log.enh_acmac_log.sector_access_max_rate =
                       hdramac_ap_msg.sector_access_max_rate;
    hdramac_enh_attempt_log.enh_acmac_log.access_offset          =
                       hdramac_ap_msg.access_offset;
    hdramac_enh_attempt_log.enh_acmac_log.payload_size  = hdramac.data_bytes;

    hdramac_enh_attempt_log.enh_acmac_log.max_pn_delay =
                       hdramac.max_pn_randomization_delay;
  }

  /* Send the variable length enhanced access log. */
  {
    LOG_HDR_ENH_ACCESS_ATTEMPT_C_type *log_ptr;

    /* Get log record */
    log_ptr = (LOG_HDR_ENH_ACCESS_ATTEMPT_C_type *)
              log_alloc(LOG_HDR_ENH_ACCESS_ATTEMPT_C,
              sizeof(LOG_HDR_ENH_ACCESS_ATTEMPT_C_type)-sub_val);

    if (log_ptr != NULL)
    {
      /* Fill in and send the log record. */
#ifdef FEATURE_MEMCPY_REMOVAL
      memscpy((void*)&(log_ptr->enh_acmac_log),
               sizeof(log_ptr->enh_acmac_log),
              (void*)&(hdramac_enh_attempt_log.enh_acmac_log),
               FSIZ(LOG_HDR_ENH_ACCESS_ATTEMPT_C_type, enh_acmac_log)-sub_val);
#else
      memcpy((void*)&(log_ptr->enh_acmac_log),
             (void*)&(hdramac_enh_attempt_log.enh_acmac_log),
             FSIZ(LOG_HDR_ENH_ACCESS_ATTEMPT_C_type, enh_acmac_log)-sub_val);
#endif /* FEATURE_MEMCPY_REMOVAL */

      HDR_MSG_PROT_6( MSG_LEGACY_HIGH,
           "Access Attempt: PrmblLenSlots %d, PrbPwrAdj %d, EnhAccParamInc %d"
           "AccOffset %d, SectorAccMaxRt %d, PayloadSize %d",
            log_ptr->enh_acmac_log.preamble_length_slots,
            log_ptr->enh_acmac_log.probe_pwr_adjust,
            log_ptr->enh_acmac_log.enh_access_params_inc,
            log_ptr->enh_acmac_log.access_offset,
            log_ptr->enh_acmac_log.sector_access_max_rate,
            log_ptr->enh_acmac_log.payload_size);

      log_commit((log_type*)log_ptr);
    }
  }
}

#endif /* FEATURE_HDR_REVA_L1 */

/* EJECT */
/*===========================================================================

FUNCTION HDRAMAC_LOG_ACCESS_PROBE

DESCRIPTION
  Logs data relating to an access probe.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdramac_log_access_probe( void )
{
  LOG_HDR_ACCESS_PROBE_C_type *log_ptr;
  uint16                      sub_val;
  byte  ii;

  /* Send the variable length access probe log. */
 
  /* Figure out how much to shrink the log packet by... */
  sub_val = (HDRLOG_AC_MAX_FRAME_CNT
              - hdramac_probe_log.probe_log.frame_count);

  /* Get log record */
  log_ptr = (LOG_HDR_ACCESS_PROBE_C_type *)
            log_alloc(LOG_HDR_ACCESS_PROBE_C,
            sizeof(LOG_HDR_ACCESS_PROBE_C_type) -
            sizeof(log_frame_info_type) * sub_val);

  if (log_ptr != NULL)
  {
    /* Fill in and send the log record. */
#ifdef FEATURE_MEMCPY_REMOVAL
    memscpy((void*)&(log_ptr->probe_log), 
              sizeof(log_ptr->probe_log),
             (void*)&(hdramac_probe_log.probe_log),
              FSIZ(LOG_HDR_ACCESS_PROBE_C_type, probe_log)
               - (sizeof(log_frame_info_type) * sub_val));
#else
    memcpy((void*)&(log_ptr->probe_log),
           (void*)&(hdramac_probe_log.probe_log),
           FSIZ(LOG_HDR_ACCESS_PROBE_C_type, probe_log)
            - (sizeof(log_frame_info_type) * sub_val));
#endif /* FEATURE_MEMCPY_REMOVAL */

    /* These F3 msgs are only for initial debugging */
    HDR_MSG_PROT_5( MSG_LEGACY_HIGH,
        "Seq# %d, Prb# %d, Probe Delayed %d, PreamblePwr %d, FrameCnt %d",
         log_ptr->probe_log.probe_seq_num,
         log_ptr->probe_log.probe_num,
         log_ptr->probe_log.probe_delayed,
         log_ptr->probe_log.preamble_pwr,
         log_ptr->probe_log.frame_count);

    for (ii = 0; ii < log_ptr->probe_log.frame_count; ii++)
    {
      HDR_MSG_PROT_5( MSG_LEGACY_HIGH, 
                      "Frame# %d, Size %d, Rate %d, Pilot %d, Total %d", 
                      ii,
                      log_ptr->probe_log.frame_info[ii].frame_data_size,
                      log_ptr->probe_log.frame_info[ii].frame_rate,
                      log_ptr->probe_log.frame_info[ii].pilot_power,
                      log_ptr->probe_log.frame_info[ii].total_power);
    }
    log_commit((log_type*)log_ptr);
  }
  else
  {
    HDR_MSG_PROT( MSG_LEGACY_HIGH, 
                  "Unable to alloc mem for LOG_HDR_ACCESS_PROBE_C" );
  }
}

/* EJECT */
/*===========================================================================

FUNCTION HDRAMAC_LOG_ACCESS_ATTEMPT                                  INTERNAL

DESCRIPTION
  Logs data relating to an access attempt.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdramac_log_access_attempt( void )
{
  uint32 hdramac_attempt_cycles;
    /* Number of page cycles the access attempt took. */
  uint16 hdramac_end_slot_time;
    /* For determining the end time of the access attempt */

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Record the duration of the attempt, in slots */
  hdramac_end_slot_time = hdrts_get_slot_time();
  hdramac_attempt_cycles = hdrts_get_page_cycle()
     - hdramac.start_page_cycle;
  hdramac_attempt_log.duration =
     (uint16) ((hdramac_attempt_cycles * HDRAMAC_SLOTS_PER_PAGE_CYCLE) +
                hdramac_end_slot_time) - hdramac.start_slot_time;

  /* Record number of probe sequences */
  hdramac_attempt_log.probe_sequence_count = (uint8) hdramac.seq_cnt;

  /* Update total attempt counts */
  if (hdramac_attempt_log.result != HDRLOG_AMAC_PROBE_INTERRUPTED)
  {
    if (hdramac_attempt_log.result == HDRLOG_AMAC_ACACK_RECEIVED)
    {
      hdramac_attempt_log.success_count++;
    }
    else
    {
      hdramac_attempt_log.failure_count++;
    }
    hdramac_attempt_log.attempts_count++;
  }

  /* Fill out fields */
  hdramac_attempt_log.color_code = hdramac.color_code;
  hdramac_attempt_log.sector_id_lsw = (hdramac.sector_id & 0xffff);
  hdramac_attempt_log.sector_id_usw = (uint8) (hdramac.sector_id >> 16);

#ifdef FEATURE_HDR_REVA_L1
  /* log enhanced access attempt. Note: Rev0 log packet should be 
     filled in before logging enhanced access information */
  if (hdramac_ap_msg.enhanced_access_parameters_included)
  {
    hdramac_log_enh_access_attempt();
  }
#endif /* FEATURE_HDR_REVA_L1 */

  /* Send the variable length log. */
  {
    LOG_HDR_ACCESS_ATTEMPT_C_type *log_ptr;
    uint16                         sub_val;

    /* Figure out how much to shrink the log packet by... */
    sub_val = (HDRLOG_AC_MAX_PROBE_CNT
              - hdramac_attempt_log.probe_sequence_count);

    /* Get log record */
    log_ptr = (LOG_HDR_ACCESS_ATTEMPT_C_type *)
              log_alloc(LOG_HDR_ACCESS_ATTEMPT_C,
              sizeof(LOG_HDR_ACCESS_ATTEMPT_C_type)-sub_val);

    if (log_ptr != NULL)
    {

      /* Fill in and send the log record. */
#ifdef FEATURE_MEMCPY_REMOVAL
      memscpy((void*)&(log_ptr->acmac_log), 
               sizeof(log_ptr->acmac_log),
              (void*)&hdramac_attempt_log,
               FSIZ(LOG_HDR_ACCESS_ATTEMPT_C_type, acmac_log)
               - sub_val);
#else
      memcpy((void*)&(log_ptr->acmac_log),
             (void*)&hdramac_attempt_log,
             FSIZ(LOG_HDR_ACCESS_ATTEMPT_C_type, acmac_log)
              - sub_val);
#endif /* FEATURE_MEMCPY_REMOVAL */

      log_commit((log_type*)log_ptr);
    }
  }
}


/* EJECT */
/*===========================================================================

FUNCTION HDRAMAC_ATTEMPT_CLEANUP                                     INTERNAL

DESCRIPTION
  General clean-up for ending an access attempt.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdramac_attempt_cleanup( void )
{

  /* Failsafe: make sure firmware logging callback is removed */
  hdrlog_register_slot_logging_cb ( HDRLOG_AMAC_MDSP_SLOT_LOGGING_CLIENT,
                                    NULL );

  /* ISR cleanup.  Un-install the access channel ISR. */
  hdrmod_disable_frame_isr();
  (void) rex_clr_sigs( HDRTX_TASK_PTR, HDRTX_AMAC_ISR_SIG );
  
  if ( hdramac.tx_enabled == TRUE )
  {
#ifdef FEATURE_HDR_PN_DELAY_SUPPORT
    #error code not present
#endif /* FEATURE_HDR_PN_DELAY_SUPPORT */

    /* Turn PA off.
       In most cases PA should be already be off when this function is called. */
    hdrmod_setup_tx_timed_disable( TRUE, 0 );

    /* Modulator cleanup. Disable PA and tell modulator to exit access mode */
    hdrmod_exit_access_mode();

    /* Disable carrier 0 in firmware and exit access mode. */
    hdramac_disable_carrier_access_mode();

    /* Trigger Tx DAC stop */
    hdrmod_tx_stop();

    /* Skip-disable-RF-Tx optimization */ 
    (void) hdrtx_disable_tx( hdramac.skip_tx_disable );

    /* Update tx status. */
    hdramac.tx_enabled = FALSE;

  }

  /* Free dsm item */
  if(hdramac.pkt_ptr)
  {
    dsm_free_packet(&hdramac.pkt_ptr);
  }

  hdramac.abort_pending = FALSE;
  /*-------------------------------------------------------------------------
   Since LMAC is in the same task context as ACMAC, the attempt_in_progress
   flag must be set to FALSE before the TxEnded indication is given, or LMAC
   will still think there is an access in progress when processing it.
  -------------------------------------------------------------------------*/
  hdramac.attempt_in_progress = FALSE;

  HDR_MSG_PROT( MSG_LEGACY_HIGH, "Access attempt cleanup." );

}

/* EJECT */
/*===========================================================================

FUNCTION HDRAMAC_ADD_PKT_FCS                                        INTERNAL

DESCRIPTION
  Computes FCS for an Access Channel packet.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdramac_add_pkt_fcs( void )
{

  /* Length of data over which FCS must be computed */
  uint16 length = hdramac.data_bytes;

  /* Indexes into dsm item for each capsule fragment (encoder packet) */
  uint16    index = 0;

  /* Size of each capsule fragment */
  uint16    packet_size;

  /* Temp buffer for holding the FCS, in big endian format. */
  byte fcs_buf[HDRAMAC_PKT_FCS_LEN];

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Initialize CRC seed */
  hdramac.fcs = CRC_32_SEED;

  /* Iterate for each capsule fragment */
  while (length)
  {

    /* Size of current encoder packet, in bytes **/
    packet_size = MIN(length, HDRAMAC_PKT_FRAME_DATA_LEN);

    /* Extract a packet's worth of data into buffer */
    (void) dsm_extract( hdramac.pkt_ptr, 
                        index, 
                        hdramac_pkt, 
                        packet_size );

    /* Compute FCS */
    hdramac.fcs = crc_32_calc( hdramac_pkt, 
                               (uint16)(packet_size*8),
                                  hdramac.fcs);

    /* Adjust counters */
    length -= packet_size;
    index += packet_size;
  }

  /* Set FCS in buffer, MSB first. */
  HDRAMAC_SET_PKT_FCS( fcs_buf, hdramac.fcs);

  /* Add to dsm item */
  (void) dsm_pushdown_tail( &hdramac.pkt_ptr,
                     fcs_buf,
                     HDRAMAC_PKT_FCS_LEN,
                     DSM_DS_POOL_SIZE( HDRAMAC_PKT_FCS_LEN ));

  /* Add FCS to packet length */
  hdramac.data_bytes += HDRAMAC_PKT_FCS_LEN;

}


/* EJECT */
/*===========================================================================

FUNCTION HDRAMAC_PREPARE_PKT_HEADER                                  INTERNAL

DESCRIPTION
  Prepares and appends packet header.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdramac_prepare_pkt_header (void)
{
  uint16 session_config_token;

  /* Temp buffer for creating the Access Channel header. */
  byte hdramac_pkt_hdr[HDRAMAC_PKT_HDR_LEN];

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Compute the number of encoder packets it will take to send the data. */
  hdramac.num_encoder_pkts = (HDRAMAC_PKT_HDR_LEN
             + hdramac.data_bytes
             + HDRAMAC_PKT_FCS_LEN
             + (HDRAMAC_PKT_FRAME_DATA_LEN-1)) / HDRAMAC_PKT_FRAME_DATA_LEN;

  /*-------------------------------------------------------------------
   Build the Access Channel Packet header, which includes the length
   field, authentication and encryption bits, and the ATI record.
  -------------------------------------------------------------------*/
  HDRAMAC_CLR_PKT_HDR( hdramac_pkt_hdr );

  HDRAMAC_SET_PKT_LEN( hdramac_pkt_hdr, hdramac.data_bytes
                          + HDRAMAC_PKT_HDR_LEN
                          - HDRAMAC_PKT_HDR_LEN_FIELD_LEN );

  session_config_token = 
           hdrscp_get_session_config_token(HDRHAI_AC_MAC_PROTOCOL);
  HDRAMAC_SET_PKT_CONFIG_TOKEN( hdramac_pkt_hdr,
        session_config_token);

  if ( hdramac.is_size_incl ) HDRAMAC_SET_PKT_SIZE_BIT( hdramac_pkt_hdr );
  if ( hdramac.is_secured ) HDRAMAC_SET_PKT_SEC_BIT( hdramac_pkt_hdr );

  HDRAMAC_SET_PKT_ATI_TYPE( hdramac_pkt_hdr, hdramac.ati.ati_type );
  HDRAMAC_SET_PKT_ATI_VAL(  hdramac_pkt_hdr, hdramac.ati.ati_value );

  /* Insert header into beginning of dsm item. */
  (void) dsm_pushdown( &hdramac.pkt_ptr,
                   hdramac_pkt_hdr,
                   HDRAMAC_PKT_HDR_LEN,
                   DSM_DS_POOL_SIZE( HDRAMAC_PKT_HDR_LEN ));

  hdramac.data_bytes += HDRAMAC_PKT_HDR_LEN;
}

/* EJECT */
#ifdef FEATURE_HDR_REVA_L1
/*===========================================================================

FUNCTION HDRAMAC_UPDATE_PROBE_NUM_AND_FCS                           INTERNAL

DESCRIPTION
  Updates probe num in packet header and recomputes FCS.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Access packet header contains updated probe number. Updated FCS.

===========================================================================*/
void hdramac_update_probe_num_and_fcs (void)
{
  /* temporary buffer to hold Access Header + capsule data */
  byte  tmp_pkt_ptr[ HDRAMAC_PKT_MAX_DATA_LEN ];

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* extract header + capsule data into temporary buffer */
  (void) dsm_extract (hdramac.pkt_ptr,
                      0,
                      tmp_pkt_ptr,
                      hdramac.data_bytes - HDRAMAC_PKT_FCS_LEN);
  
  /* buffer extracted does not contain FCS. Subtract FCS length from
    total access data size */
  hdramac.data_bytes -= HDRAMAC_PKT_FCS_LEN;
  
  /* add ProbeNumber. Header length is unchanged. */
  b_packb ((byte) hdramac.probe_cnt, tmp_pkt_ptr, 26, 4);

  /* free old access packet */
  dsm_free_packet (&hdramac.pkt_ptr);

  /* prepare new access packet with updated header and capsule data */
  (void) dsm_pushdown (&hdramac.pkt_ptr, tmp_pkt_ptr, 
                hdramac.data_bytes,
                       DSM_DS_POOL_SIZE(hdramac.data_bytes));

  /* Recompute and add FCS to Access Packet. */
  hdramac_add_pkt_fcs();
}

/* EJECT */
#endif /* FEATURE_HDR_REVA_L1 */
/*===========================================================================

FUNCTION HDRAMAC_PREPARE_PKT                                         INTERNAL

DESCRIPTION
  Retrieves a packet from the upper layer (if one is available) and prepares
  it for transmission over the access channel.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  TRUE - a packet was retrieved and is ready to send.
  FALSE - no packet was available.

SIDE EFFECTS
  None.

===========================================================================*/
boolean hdramac_prepare_pkt( void )
{

  /* Maximum number of payload bytes that may be sent over the access
     channel. */
  uint16 max_bytes;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#ifdef FEATURE_FACTORY_TESTMODE
  if(ftm_get_mode() == FTM_MODE)
  {
    /* Create dummy packet */
    if (!hdramac.override_data_bytes)
    {
      hdramac.data_bytes = 10;
    }

    hdramac.num_encoder_pkts=1;
  }
  else
#endif
  {
    max_bytes = hdramac_ap_msg.capsule_length_max * HDRAMAC_PKT_FRAME_DATA_LEN;

   /* Max allowed ACMAC payload is 260 bytes ( 1 byte len field, 255 bytes of
    * payload which includes the last 7 bytes of the header, and 4 byte FCS ) */
    if (hdramac_ap_msg.capsule_length_max >= 9)
    {
      /* There is room for at least 261 (9 * 29) bytes, so the packet size is
       * limited by the Length field being only 8 bits */
      max_bytes = HDRAMAC_PKT_MAX_DATA_LEN - ( HDRAMAC_PKT_HDR_LEN
                                             - HDRAMAC_PKT_HDR_LEN_FIELD_LEN );
    }
    else
    {
      /* The packet is limited by the capsule_length_max */
      max_bytes = max_bytes - ( HDRAMAC_PKT_HDR_LEN
                              + HDRAMAC_PKT_FCS_LEN );
    }

    /*---------------------------------------------------------------------
     Access Channel packet request.  Get the Security Layer Packet payload
     from the Security Layer in the form of a dsm item chain.
    ---------------------------------------------------------------------*/
    if (!hdramac.override_data_bytes)
    {
      hdramac.data_bytes = hdrsec_get_pkt( max_bytes,
                                           HDRHAI_AC,
                                           hdramac.elapsed_frames,
                                           &hdramac.pkt_ptr,
                                           &hdramac.is_size_incl,
                                           &hdramac.is_secured );
    }

    hdramac.elapsed_frames=0;

    /* Return FALSE if there was no data to send. */
    if ( hdramac.data_bytes == 0 )
    {
      return FALSE;
    }

    /* Check if we got more data than we asked for...don't know if this is
       even possible... */
    if ( hdramac.data_bytes > max_bytes )
    {
      ERR_FATAL("Too much data for probe!", 0, 0, 0);
    }

    HDR_MSG_PROT_1( MSG_LEGACY_HIGH, "AC: Got %d bytes from SEC",
                    hdramac.data_bytes );

    if (hdramp_get_transmit_ati( &hdramac.ati, HDRHAI_AC_MAC_PROTOCOL )
        != E_SUCCESS)
    {
      ERR("Could not get ATI for transmit",0,0,0);

      /* Note that the dsm item will get freed in attempt_cleanup() */
      return FALSE;
    }

    HDR_MSG_PROT_2( MSG_LEGACY_HIGH, "Attempt using ATI (%x) 0x%x",
                    hdramac.ati.ati_type,
                    hdramac.ati.ati_value );

    hdramac_prepare_pkt_header();
  }

  /* Compute and add FCS to Access Packet. */
  hdramac_add_pkt_fcs();


  return(TRUE);
}


/* EJECT */
/*===========================================================================

FUNCTION HDRAMAC_PROCESS_ACTIVATE

DESCRIPTION
  This command transitions the protocol to the Active state.  If the protocol
  is already in the Active state then no action is taken.

DEPENDENCIES
  The protocol must be initialized by calling the hdramac_init function prior
  to activation.

PARAMETERS
  caller - The protocol giving this command (only for software trace)

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdramac_process_activate 
( 
  hdrhai_protocol_name_enum_type caller
)
{

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  /* Initialize skip Tx disable optimization to FALSE by default */
  hdramac.skip_tx_disable = FALSE; 

  HDRTRACE_CMD( HDRHAI_AC_MAC_PROTOCOL, HDRAMAC_ACTIVATE_CMD,
    caller, hdramacsm_get_state() );

  /*-----------------------------------------------------------------------
   Since the Access Channel MAC and the Reverse Traffic Channel MAC both use
   the transmitter, both must be inactive before one can be activated.
  -----------------------------------------------------------------------*/
  if ( hdrrmac_is_active(HDRHAI_AC_MAC_PROTOCOL) == FALSE )
  {
    if (hdramac_is_active(HDRHAI_AC_MAC_PROTOCOL) == FALSE)
    {
      hdramacsm_change_state( HDRAMAC_ACTIVE_STATE );
    }
    else
    {
      HDR_MSG_PROT( MSG_LEGACY_HIGH, "ACMAC already in active state" );

      HDRTRACE_CMD_IGNORED_STATE( HDRHAI_AC_MAC_PROTOCOL, HDRAMAC_ACTIVATE_CMD,
        caller, hdramacsm_get_state() );

    }
  }
  else
  {
    ERR( "RTCMAC is active, can't activate ACMAC", 0, 0, 0 );

    HDRTRACE_CMD_IGNORED_STATE( HDRHAI_AC_MAC_PROTOCOL, HDRAMAC_ACTIVATE_CMD,
      caller, hdramacsm_get_state() );
  }

}

/* EJECT */
/*===========================================================================

FUNCTION HDRAMAC_PROCESS_DEACTIVATE

DESCRIPTION
  This command transitions the protocol to the Inactive state and clears the
  supervision timer.  If the protocol is already in the Inactive state then
  no action is taken.

DEPENDENCIES
  None.

PARAMETERS
  caller - The protocol giving this command (only for software trace)
  skip_tx_disable - Specifies if CP wants to skip disabling Tx, 
                    If true, then we skip disabling Tx RF and MCPM. 
RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdramac_process_deactivate
(
  hdrhai_protocol_name_enum_type caller,
  boolean skip_tx_disable
)
{

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

    /* If deactivatation occured before the attempt is complete,
     mark skip tx disable to False */
  if ((hdramac.substate != HDRAMAC_ACC_END2_SUBSTATE) &&
      (hdramac.substate != HDRAMAC_ACC_END_SUBSTATE))
  {
    hdramac.skip_tx_disable = FALSE;
  }
  else
  {
    /* Store the value passed by CP into a global variable */
    hdramac.skip_tx_disable = skip_tx_disable;
  }
  
  HDRTRACE_CMD( HDRHAI_AC_MAC_PROTOCOL, HDRAMAC_DEACTIVATE_CMD,
    caller, hdramacsm_get_state() );

    /* First check if ACMAC is even active. */
  if ( hdramac_is_active(HDRHAI_AC_MAC_PROTOCOL) == TRUE )
  {

    /* Check if we are making an attempt. */
    if (hdramac.attempt_in_progress)
    {
#ifdef FEATURE_HDR_HYBRID_ACCESS
      if ( hdramac.tx_enabled == FALSE )
      {
        /* ISR cleanup.  Un-install the access channel ISR. */
        hdrmod_disable_frame_isr();
        (void) rex_clr_sigs( HDRTX_TASK_PTR, HDRTX_AMAC_ISR_SIG );
        
        /* Free dsm item */
        if(hdramac.pkt_ptr)
        {
          dsm_free_packet(&hdramac.pkt_ptr);
        }

        hdramac.abort_pending = FALSE;

        hdramac.attempt_in_progress = FALSE;
        hdrind_give_ind( HDRIND_ACMAC_TX_ENDED, NULL );

        HDR_MSG_PROT( MSG_LEGACY_HIGH, "Deactivate-Access attempt cleanup." );
      }
      else
#endif /* FEATURE_HDR_HYBRID_ACCESS */
      {
        /* Disable probe transmission and clean up. */
        hdramac_attempt_cleanup();

        /* Signal that Tx has ended */
        hdrind_give_ind( HDRIND_ACMAC_TX_ENDED, NULL );
      }

      /* If deactivatation occured before the attempt is complete,
         send a TransmissionAborted indication. */
      if ((hdramac.substate != HDRAMAC_ACC_END2_SUBSTATE) &&
          (hdramac.substate != HDRAMAC_ACC_END_SUBSTATE))
      {
        hdrind_give_ind( HDRIND_ACMAC_TRANSMISSION_ABORTED, NULL );
        HDRTRACE_AMAC_ACCESS_ABORTED();
      }
    }

    /* Switch to inactive state */
    hdramacsm_change_state( HDRAMAC_INACTIVE_STATE );

  }
  else
  {
    /* Ignore Command */
    HDRTRACE_CMD_IGNORED_STATE( HDRHAI_AC_MAC_PROTOCOL,
      HDRAMAC_DEACTIVATE_CMD, caller, hdramacsm_get_state() );
  }

#ifdef FEATURE_FACTORY_TESTMODE
  if(ftm_get_mode() != FTM_MODE)
#endif
  {
    /* Deactivate must be called from hdrmc task */
    (void) rex_set_sigs(HDRMC_TASK_PTR, HDRMC_MAC_DEACTIVATE_COMPLETE_SIG);
  }

}

/* EJECT */
/*===========================================================================
FUNCTION HDRAMAC_ABORT

DESCRIPTION
  This command causes the ACMAC to finish its current probe as if it were
  the last.  If no response is received ACMAC returns
  HDRIND_ACMAC_TRANSMISSION_STOPPED indicaiton rather  than a failure.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdramac_process_stop ( hdrhai_protocol_name_enum_type caller )
{

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDRTRACE_CMD( HDRHAI_AC_MAC_PROTOCOL, HDRAMAC_STOP_CMD,
    caller, hdramacsm_get_state() );

  /* Check if we are making an attempt. */
  if (hdramac.attempt_in_progress)
  {
    /* Set abort pending flag. */
    hdramac.abort_pending = TRUE;
  }
}

/*===========================================================================
FUNCTION HDRAMAC_PROCESS_RESET_ATTEMPT

DESCRIPTION
  This function processes the reset attempt command and resets the current
  access attempt.

DEPENDENCIES
  None.

PARAMETERS
  caller - The protocol calling this externalized function (for sw trace)

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdramac_process_reset_attempt( hdrhai_protocol_name_enum_type caller )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDRTRACE_CMD( HDRHAI_AC_MAC_PROTOCOL, HDRAMAC_RESET_ATTEMPT_CMD,
    caller, hdramacsm_get_state() );

  if( ( hdramac.attempt_in_progress ) && 
      ( hdramac.substate == HDRAMAC_ACC_END2_SUBSTATE ) )
  {
      hdramac.countdown = 0;
      hdramac_attempt_cleanup();

      /* Signal that Tx has ended */
      hdrind_give_ind( HDRIND_ACMAC_TX_ENDED, NULL );
  }
}

/* EJECT */

#if defined (FEATURE_HDR_IS890A)
/*===========================================================================
FUNCTION HDRAMAC_SET_EACH_RATE

DESCRIPTION
  This function specifies the enhanced access channel rate to be used in
  the next access attempt.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  Rate of EAC frame.

SIDE EFFECTS
  None.
===========================================================================*/
void hdramac_set_each_rate ( uint8 each_rate )
{
  if (each_rate == HDRAMAC_RETAP_EACH_RATE_MODE_DISABLED)
  {
    /* EACH rate is not configured through RETAP */
    hdramac.test_access_rate = 0;

    HDR_MSG_PROT( MSG_LEGACY_HIGH, 
                  "ParamAssgMsg Configured EACH rate mode disabled" );
  }
  else
  {
    hdramac.test_access_rate = 
                  1 + MIN (each_rate, hdramac_ap_msg.sector_access_max_rate);

    HDR_MSG_PROT_3( MSG_LEGACY_HIGH, 
         "ParamAssgMsg Each rate %d, SectorAccessMaxRate %d, Access Rate %d",
          each_rate, hdramac_ap_msg.sector_access_max_rate,
          (hdramac.test_access_rate - 1));
  }
}

/* EJECT */
#endif /* FEATURE_HDR_IS890A */

/*===========================================================================
FUNCTION HDRAMAC_EAC_MAX_ACCESS_RATE

DESCRIPTION
  This function determines the maximum allowable access channel rate 
  based on SectorAccessMaxRate, TerminalAccessRateMax, payload length 
  and available transmit power.

DEPENDENCIES
  This function is called only if Enahnced Access Mode is negotiated and 
  enhanced access parameters are included in Access Parameters Message.

PARAMETERS
  None.

RETURN VALUE
  Maximum access channel frame rate.

SIDE EFFECTS
  None.
===========================================================================*/
hdrmod_rri_enum_type hdramac_eac_max_access_rate (void)
{
  hdrmod_rri_enum_type  highest_pwr_transmit_rate, access_rate_max;
  hdrmod_rri_enum_type  min_payload_frame_rate, access_rate;
  uint8  payload_size;

  /* determine the higher of the 2 rates : 
     SectorAccessMaxRate or TerminalAccessRateMax */
  access_rate_max = (hdrmod_rri_enum_type)
                    (1 + MAX (hdramac_ap_msg.sector_access_max_rate,
                    hdramac.config->terminal_access_rate_max ));

  /* sample maxDataRateIndex register to get the highest data rate that 
     can be accommodated by the available transmit power */
  highest_pwr_transmit_rate = (hdrmod_rri_enum_type) HDRAGC_GET_MAX_RRI();

  if (highest_pwr_transmit_rate == 0)
  {
    highest_pwr_transmit_rate = REVERSE_RATE_9600;
  }

  /* determine size of payload yet to be sent */
  payload_size = hdramac.data_bytes - hdramac.data_sent;

  /* determine smallest rate frame that can accomodate 
     the remaining payload */
  if (payload_size <= HDRAMAC_PKT_9K6_FRAME_DATA_LEN)
  {
    min_payload_frame_rate = REVERSE_RATE_9600;
  }
  else if (payload_size <= HDRAMAC_PKT_19K2_FRAME_DATA_LEN)
  {
    min_payload_frame_rate = REVERSE_RATE_19200;
  }
  else
  {
    min_payload_frame_rate = REVERSE_RATE_38400;
  }

  HDR_MSG_PROT_3( MSG_LEGACY_HIGH,
                  "MaxSectTermRate %d, AvailPwrRate %d, PayloadSizeRate %d",
                  access_rate_max, highest_pwr_transmit_rate, 
                  min_payload_frame_rate );

  /* MIN (access_rate_max, 
          highest_pwr_transmit_rate, 
          min_payload_frame_rate) */
  access_rate = MIN( MIN( access_rate_max, highest_pwr_transmit_rate ),
                     min_payload_frame_rate );

  return access_rate;
}

/* EJECT */
/*===========================================================================
FUNCTION HDRAMAC_SET_FIXED_ACCESS_RATE 

DESCRIPTION
  This function sets the access rate to a fixed rate as specified by the user
  through the AT$QCHDREAMAC AT cmd.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Fixes access channel rate.
===========================================================================*/
boolean hdramac_set_fixed_access_rate
(
 uint8  rate  /* Rate specified by user through AT cmd */
)
{
  hdramac.test_access_rate = rate;

  if (rate == 0x0)
  {
    HDR_MSG_PROT( MSG_LEGACY_HIGH, "Variable Rate Access Attempt");
  }
  else
  {
    HDR_MSG_PROT_1( MSG_LEGACY_HIGH, "Fixing Access rate to %d", rate);
  }

  return TRUE;
}

/* EJECT */
/*===========================================================================
FUNCTION HDRAMAC_EAC_FRAME_RATE

DESCRIPTION
  This function determines the rate of an enhanced access channel frame
   
DEPENDENCIES
  None.

PARAMETERS
  Returns size of data and rate of frame to be transmitted in the next 
  access probe frame.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdramac_eac_frame_rate
(
 uint8                 *payload_size,
 hdrmod_rri_enum_type  *probe_frame_rate
)
{
  if ((hdramac.current_subtype == HDRAMAC_SUBTYPE_ENHANCED) && 
      hdramac_ap_msg.enhanced_access_parameters_included)
  {
    hdrmod_rri_enum_type  max_access_rate;
    uint8                 max_frame_size;

    /* obtain the best access rate and the corresponding frame size */

    if ((hdramac.test_access_rate >= 1) && (hdramac.test_access_rate <= 3))
    {
      max_access_rate = (hdrmod_rri_enum_type) hdramac.test_access_rate;
    }
    else
    {
      max_access_rate = hdramac_eac_max_access_rate();
    }

    if (max_access_rate == REVERSE_RATE_9600)
    {
      max_frame_size = HDRAMAC_PKT_9K6_FRAME_DATA_LEN;
    }
    else if (max_access_rate == REVERSE_RATE_19200)
    {
      max_frame_size = HDRAMAC_PKT_19K2_FRAME_DATA_LEN;
    }
    else /* for return value >= 3 use 38k4 rate */
    {
      max_frame_size = HDRAMAC_PKT_38K4_FRAME_DATA_LEN;
    }

    /* set the actual payload size to be transmitted, 
       max payload size is limited by max frame rate */
    *payload_size = MIN((hdramac.data_bytes - hdramac.data_sent), 
                        max_frame_size);

    /* rate of next probe frame */
    *probe_frame_rate = max_access_rate;

    HDR_MSG_PROT_2( MSG_LEGACY_HIGH,
                    "EACMAC Payload %d bytes, ProbeFrameRate %d",
                    *payload_size, *probe_frame_rate );
  }
  else
  {
    /* Enhanced Access mode but enhanced access parameters not included OR
       Rev 0 mode */
    *payload_size = MIN((hdramac.data_bytes - hdramac.data_sent),
                        HDRAMAC_PKT_FRAME_DATA_LEN);

    *probe_frame_rate = REVERSE_RATE_9600;
  }
}

/* EJECT */
/*===========================================================================
FUNCTION HDRAMAC_ADD_PILOT_STRENGTH_CORR

DESCRIPTION
  This function computes pilot strength correction value and updates
  probe_init_adjust value in hdramac struct.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  Pilot strength correction value.

SIDE EFFECTS
  probe_init_adjust in hdramac struct is updated.
===========================================================================*/
void hdramac_add_pilot_strength_corr (void)
{
  uint16  pilot_eng;
  int8    ap_msg_nom, ap_msg_corr_min, ap_msg_corr_max;
  int8    nom, corr_min, corr_max, pilot_str_corr;
  int     pilot_eng_db;

  if ((hdramac.current_subtype == HDRAMAC_SUBTYPE_ENHANCED) && 
      hdramac_ap_msg.enhanced_access_parameters_included)
  {
    /* Best active sector pilot energy */
    pilot_eng    = 
                  hdrsrchmac_get_basp_pri_chain_eng( HDRMDSP_DEMOD_CARRIER_0 );

    /* pilot strength as ecio */
    pilot_eng_db = hdrsrchmac_eng_to_ecio( pilot_eng ); /* -0.5 dB units */
    pilot_eng_db = -1 * (pilot_eng_db >> 1);  /* converted to 1 dB units */

    ap_msg_nom      = hdramac_ap_msg.pilot_strength_nominal;
    ap_msg_corr_min = hdramac_ap_msg.pilot_strength_correction_min;
    ap_msg_corr_max = hdramac_ap_msg.pilot_strength_correction_max;

    nom      = hdramac_pilot_str_nom_val [ap_msg_nom];
    corr_min = hdramac_pilot_str_corr_min_val [ap_msg_corr_min];
    corr_max = hdramac_pilot_str_corr_max_val [ap_msg_corr_max];

    /* pilot_str_corr is in 1 dB units */
    pilot_str_corr = MIN( corr_max, (MAX( (nom - pilot_eng_db), corr_min )) );

    /* add Pilot strength correction to Probe Init Adjust value */
    hdramac.probe_init_adjust += pilot_str_corr;

    HDR_MSG_PROT_3( MSG_LEGACY_HIGH,
                    "Using PilotStr (dB): Nom %d, CorrMin %d, CorrMax %d",
                    nom, corr_min, corr_max);
    HDR_MSG_PROT_3( MSG_LEGACY_HIGH,
                    "PilotStrDB %d, ProbeInitAdj %d, PilotStrCorr %d",
                    pilot_eng_db, hdramac.probe_init_adjust, pilot_str_corr);
  }
}

/* EJECT */

#ifdef FEATURE_HDR_HYBRID_ACCESS
/*===========================================================================
FUNCTION HDRAMAC_ESTIMATE_PROBE_LENGTH

DESCRIPTION
  This functions estimates the length of the remaining probe in slots

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  remaining probe length value in slots

SIDE EFFECTS
  probe_init_adjust in hdramac struct is updated.
===========================================================================*/
uint32 hdramac_estimate_probe_length (void)
{
  uint32  remaining_probe_length = 0;  /* in slots */

  if ( hdramac.substate == HDRAMAC_ACC_TUNE_AWAY_SUBSTATE )
  {
    remaining_probe_length = (HDRAMAC_TX_SETUP_FRAMES * 16) +
                             hdramac.preamble_len           +
                             (hdramac.num_encoder_pkts * 16);
  }
  else if ( ( hdramac.substate == HDRAMAC_ACC_TX_PRE_SUBSTATE  ) ||
            ( hdramac.substate == HDRAMAC_ACC_TX_DATA_SUBSTATE ) )
  {
    if (hdramac.frame_num <= 0)
    {
      /* preamble TX to start, followed by probe data */

      remaining_probe_length = (hdramac.frame_num * -16)  +
                                hdramac.preamble_len      +
                                (hdramac.num_encoder_pkts * 16);
    }
    else
    {
      /* Rev.A preamble is covered in 1 ISR. Remaining probe length
         estimated at 9.6 kbps. */ 
      remaining_probe_length = (hdramac.num_encoder_pkts  +
                                hdramac.preamble_isr_cnt  -
                                hdramac.frame_num) * 16;
    }
  }
  else
  {
    ERR ("Call to hdramac_estimate_probe_length in HDRAMAC %d state illegal",
          hdramac.substate, 0, 0);
  }

  return remaining_probe_length;
}

/* EJECT */
/*===========================================================================
FUNCTION HDRAMAC_CHECK_TUNEAWAY_OVERLAP

DESCRIPTION
  This functions checks if a probe overlaps with tune away time.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  TRUE  - if probe overlaps with tune away, else FALSE.

SIDE EFFECTS
  probe_init_adjust in hdramac struct is updated.
===========================================================================*/
boolean hdramac_check_tuneaway_overlap
(
  uint32  remaining_probe_length
)
{
  qword   slot_time;
  int     cmp;

  /* get current time in slots */
  (void) hdrts_get_current_time (slot_time, NULL);

  HDR_MSG_PROT_3( MSG_LEGACY_HIGH, 
    "Current time in slots: 0x%x:%x, estimated probe len w/ACK: 0x%x",
     slot_time[1], slot_time[0], 
     (remaining_probe_length + HDRAMAC_ACACK_TIMEOUT_PERIOD));

  /* add remaining probe length + ACACK timeout to current time in slots */
  qw_inc (slot_time, (remaining_probe_length + HDRAMAC_ACACK_TIMEOUT_PERIOD));

  /* compare tuneaway time with end of probe time */
  cmp = qw_cmp ( hdramac.tuneaway_time, slot_time );

  HDR_MSG_PROT_3( MSG_LEGACY_HIGH, 
            "Tune away time: 0x%x:%x, overlap? (<0 True)/(>=0 False) : %d",
            hdramac.tuneaway_time[1], hdramac.tuneaway_time[0], cmp);

  if ( cmp >= 0 )
  {
    /* probe ends before tuneaway period, no overlap */
    return FALSE;
  }
  else
  {
    /* probe overlaps with tuneaway time */
    return TRUE;
  }
}

#endif /* FEATURE_HDR_HYBRID_ACCESS */

/* EJECT */
/*===========================================================================

FUNCTION HDRAMAC_PROCESS_ACCESS_ATTEMPT

DESCRIPTION
  This function handles the access attempt and is called each frame in
  response to the signal that is received from the interrupt service routine
  which is attached to the modulator transmit frame interrupt.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdramac_process_access_attempt ( void )
{
  hdrind_ind_data_union_type ind_data;
    /* Payload for HDRIND_ACMAC_TRANSMISSION_FAILED indication*/   

#ifdef FEATURE_HDR_PN_DELAY_SUPPORT
#error code not present
#endif /* FEATURE_HDR_PN_DELAY_SUPPORT */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Check that Access is indeed in a probe attempt to avoid
     race condition where this function is run after attempt is
     disabled. Also check for tune away for same reason. */
  if ((!hdramac.attempt_in_progress) || (hdramac.tuneaway_stopped))
  {

     if (hdramac.tuneaway_stopped)
     {
        /* Access Attepmt stopped because of TuneAway. Re-schedule Probe */
        qword current_frametime;
        uint32  wait;          /* Frames to wait to avoid RLSI */

        HDR_MSG_PROT(MSG_LEGACY_HIGH, 
                  "Access Attepmt stopped because of TuneAway. "
                  "Re-schedule Probe!!");

        /* Get the time in frames. */
        (void) hdrts_get_frame_time( current_frametime );

        hdramac_compute_silence_time ();
        wait = hdramac_check_reverse_silence_wait (current_frametime);

        /* reschedule for next available cycle */
        hdramac_scheduler (0, wait*16);
     }

    HDR_MSG_PROT( MSG_LEGACY_HIGH, 
                  "Access no longer in attempt or tuned away. Exit.");
    return;
  }

  /*-----------------------------------------------------------------------
   Access attempt start substate.  Start a new access attempt by building a
   new access probe to send and initializing the access attempt state
   variables.  This substate can begin in an arbitrary frame of the current
   Access Channel Slot.  Upon exiting this substate, the number of delay
   frames required prior to the persistence test is computed.
  -----------------------------------------------------------------------*/
  if ( hdramac.substate == HDRAMAC_ACC_START_SUBSTATE )
  {
    hdramac.substate = HDRAMAC_ACC_AP_MSG_SUBSTATE;
  }

  if ( hdramac.substate == HDRAMAC_ACC_AP_MSG_SUBSTATE )
  {
    if (hdramac.abort_pending)
    {
      /* We got a stop probe command...stop attempt. */
      hdramac_attempt_cleanup();

      /* Signal that Tx has ended */
      hdrind_give_ind( HDRIND_ACMAC_TX_ENDED, NULL );

      hdrind_give_ind( HDRIND_ACMAC_TRANSMISSION_STOPPED, NULL );
      return;
    }

#ifdef FEATURE_FACTORY_TESTMODE
    if(ftm_get_mode() == FTM_MODE)
    {
      hdrhitmsg_ftm_get_ap_msg(&hdramac_ap_msg);
    }
    else
#endif
    {

      /*---------------------------------------------------------------------
       The AccessParameters message for the current sector must be available
       before activating the Access Channel MAC.
      ---------------------------------------------------------------------*/
      if ( hdrovhd_get_ap_msg( &hdramac_ap_msg, HDRHAI_AC_MAC_PROTOCOL) !=
            E_SUCCESS )
      {
        HDR_MSG_PROT( MSG_LEGACY_HIGH,
                      "No AccessParameters message. Waiting..." );
        return;
      }
    }
    HDR_MSG_PROT( MSG_LEGACY_MED, "Retrieved AP msg" );

    /* Copy and sign extend the probe_init_adjust value */
    hdramac.probe_init_adjust = hdramac_ap_msg.probe_init_adjust;

    hdramac.preamble_len = hdramac_ap_msg.preamble_len * 16;

    /* Error check ACD for 0. */
    if (hdramac_ap_msg.access_cycle_duration == 0)
    {
      hdramac_ap_msg.access_cycle_duration = 16;
    }

#ifdef FEATURE_HDR_RUMI
    hdramac.current_subtype = hdramac_ap_msg.enhanced_access_parameters_included ? 
      HDRAMAC_SUBTYPE_ENHANCED : HDRAMAC_SUBTYPE_DEFAULT;
#else
    hdramac.current_subtype = (hdramac_subtypes_type)
      hdrscp_get_current_subtype( HDRHAI_AC_MAC_PROTOCOL );
#endif /* FEATURE_HDR_RUMI */

    HDR_MSG_PROT_1( MSG_LEGACY_HIGH, "Negotiated AC MAC Subtype = %d",
                    hdramac.current_subtype);

    if ((hdramac.current_subtype == HDRAMAC_SUBTYPE_ENHANCED) &&
        hdramac_ap_msg.enhanced_access_parameters_included)
    {
      if (hdramac_ap_msg.preamble_length_slots == 1)
      {
        /* 16 slots preamble */
        hdramac.preamble_len = 16;
      }
      else 
      {
        /* 4 slots preamble */
        hdramac.preamble_len = 4;
      }

      /* Set up the max PN delay that should be used. */
      if ((hdramac.config->pn_delay_supported) &&
          (hdramac_ap_msg.max_pn_randomization_delay_included))
      {
        hdramac.max_pn_randomization_delay = 
          hdramac_ap_msg.max_pn_randomization_delay;
      }
      else
      {
        hdramac.max_pn_randomization_delay = 0;
      }       

    }

    if (hdramac.current_subtype == HDRAMAC_SUBTYPE_ENHANCED)
    {
      HDR_MSG_PROT_3( MSG_LEGACY_HIGH,
                     "EnhAccParamInc %d, SectMaxRate %d, ProbeTimeOutAdj %d",
                      hdramac_ap_msg.enhanced_access_parameters_included,
                      hdramac_ap_msg.sector_access_max_rate,
                      hdramac_ap_msg.probe_time_out_adjust);
      HDR_MSG_PROT_3( MSG_LEGACY_HIGH,
                     "PreLenFrame %d, PreLenSlots %d, AccessOffset %d",
                      hdramac_ap_msg.preamble_len,
                      hdramac_ap_msg.preamble_length_slots,
                      hdramac_ap_msg.access_offset );
      HDR_MSG_PROT_3( MSG_LEGACY_HIGH,
                      "AP msg PilotStr: Nom %d, CorrMin %d, CorrMax %d",
                      hdramac_ap_msg.pilot_strength_nominal,
                      hdramac_ap_msg.pilot_strength_correction_min,
                      hdramac_ap_msg.pilot_strength_correction_max);

      HDR_MSG_PROT_2( MSG_LEGACY_HIGH,
                      "AP msg Max PN: Inc %d, Delay %d",
                      hdramac_ap_msg.max_pn_randomization_delay_included,
                      hdramac_ap_msg.max_pn_randomization_delay);
    }

#ifndef FEATURE_HDR_RUMI_TX_ONLY
    /* add Pilot Strength Correction to probe_init_adjust value */
    hdramac_add_pilot_strength_corr();
#endif /* FEATURE_HDR_RUMI_TX_ONLY */

    /* Set nominal pwr in RF driver */
    rfm_hdr_set_nominal_pwr(hdrsrchrf_get_rf_dev(HDRSRCHRF_TX_CHAIN), 
                            hdramac_ap_msg.nominal_pwr);

    /* Get data to send */
    if (hdramac_prepare_pkt() != TRUE) {
      /* There was no data to send! End attempt. */
      ind_data.access_inhibited = FALSE;
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
      hdr_qsh_event_notify(hdrutil_get_hdr_sub_id(),QSH_CLT_HSRCH, HSRCH_QSH_EVENT_ACCESS_FAILURE);
#endif
      hdrind_give_ind( HDRIND_ACMAC_TRANSMISSION_FAILED, (void*)&ind_data );
      hdramac_attempt_cleanup();

      /* Signal that Tx has ended */
      hdrind_give_ind( HDRIND_ACMAC_TX_ENDED, NULL );

      return;
    }
    hdramac.substate = HDRAMAC_ACC_SCHEDULE_SUBSTATE;
  }

  /* This only occurs if the last probe succeeded. */
  if ( hdramac.substate == HDRAMAC_ACC_END2_SUBSTATE )
  {
    if (hdramac_prepare_pkt() == TRUE)
    {
      HDR_MSG_PROT( MSG_LEGACY_MED, "Retrieved another packet for AC" );
      hdramac.substate = HDRAMAC_ACC_SCHEDULE_SUBSTATE;
    }
  }

  if ( hdramac.substate == HDRAMAC_ACC_SCHEDULE_SUBSTATE )
  {
#ifdef FEATURE_HDR_HYBRID_ACCESS
    uint16  cycle_duration = 0;
#endif /* FEATURE_HDR_HYBRID_ACCESS */

    if (hdramac.current_subtype == HDRAMAC_SUBTYPE_ENHANCED)
    {
      if (hdramac.config->apersistence_override == 0xff)
      {
        if (hdramac.config->access_terminal_class_override == 0xff)
        {
          hdramac.persist_value =
            hdramac_ap_msg.apersistence[ hdramac.persist_index ];
        }
        else
        {
          hdramac.persist_value =
            hdramac_ap_msg.apersistence
            [ hdramac.config->access_terminal_class_override ];
        }
      }
      else if (hdramac.config->apersistence_override ==
               HDRAMAC_PSIST_NO_ACCESS)
      {
        hdramac.persist_value = HDRAMAC_PSIST_NO_ACCESS;
      }
      else if (hdramac.config->apersistence_override <= 0x3e)
      {
        hdramac.persist_value = hdramac.config->apersistence_override;
      }
      else
      {
        /* Do we check for incorrect values? */
      }
    }
    else
    {
      hdramac.persist_value =
        hdramac_ap_msg.apersistence[ hdramac.persist_index ];
    }

    /* Display ACCOLC to persistence index mapping */
    HDR_MSG_PROT_3( MSG_LEGACY_HIGH,
                    "ACCOLC.class = %d, APersistence[%d] = %d",
                    hdramac.accolc,
                    hdramac.persist_index,
                    hdramac.persist_value);

    /* Check if persistence value disallows any network access. Abort if
       necessary. */
    if ( hdramac.persist_value == HDRAMAC_PSIST_NO_ACCESS )
    {
      HDR_MSG_PROT( MSG_LEGACY_HIGH, "Persistence = No Access." );
      ind_data.access_inhibited = TRUE;
      hdrind_give_ind( HDRIND_ACMAC_TRANSMISSION_FAILED, (void*)&ind_data );
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
      hdr_qsh_event_notify(hdrutil_get_hdr_sub_id(),QSH_CLT_HSRCH, HSRCH_QSH_EVENT_ACCESS_FAILURE);
#endif
      hdramac_log_access_attempt();
      hdramac_attempt_cleanup();

      /* Signal that Tx has ended */
      hdrind_give_ind( HDRIND_ACMAC_TX_ENDED, NULL );

      return;
    }

    /* Set up logging record */
    hdramac_attempt_log.probe_num_step = hdramac_ap_msg.probe_num_step;
    hdramac_attempt_log.max_probe_sequence =
                 hdramac.config->initial.probe_sequence_max;
    hdramac_attempt_log.probe_sequence_count = 0;
    hdramac_attempt_log.result = HDRLOG_AMAC_ACACK_NOT_RECEIVED;
    hdramac_attempt_log.pn = 0;
    hdramac_attempt_log.duration = 0;

    hdramac_enh_attempt_log.enh_acmac_log.preamble_length_slots  = 0;
    hdramac_enh_attempt_log.enh_acmac_log.enh_access_params_inc  = FALSE;
    hdramac_enh_attempt_log.enh_acmac_log.sector_access_max_rate = 0;
    hdramac_enh_attempt_log.enh_acmac_log.access_offset          = 0;
    hdramac_enh_attempt_log.enh_acmac_log.payload_size           = 0;
    hdramac_enh_attempt_log.enh_acmac_log.max_pn_delay           = 0;

    /* Record time at which access attempt began. */
    hdramac.start_slot_time = hdrts_get_slot_time();
    hdramac.start_page_cycle = hdrts_get_page_cycle();

    /* Trace the maximum probes we can send and initial power */
    HDRTRACE_AMAC_START_ACCESS( hdramac_ap_msg.probe_num_step,
                                hdramac.config->initial.probe_sequence_max,
                                (int) hdramac.probe_init_adjust);

    /*-----------------------------------------------------------------
     All conditions for activating the protocol have been satisfied.
     Determine the persistence probability and activate the protocol.
    -----------------------------------------------------------------*/
    if ( hdramac.persist_value == HDRAMAC_PSIST_MAX_PROB )
    {
      hdramac.persist_prob = HDRAMAC_PSIST_PROB_ONE_VAL;
    }
    else
    {
      hdramac.persist_prob =
        HDRAMAC_COMPUTE_PERSISTANCE(hdramac.persist_value);
    }

    hdramac.persist_limit =
        hdramac_compute_persistence_limit(hdramac.persist_value);

    /*-------------------------------------------------------------------------
     Access attempt setup. Set the access attempt values that are derived from
     the Access Parameters message fields and Configuration fields, and
     initialize the access attempt substate for a new access attempt.

     The initial Tx power level is set according to the following:

       Tx pwr (dBm) = (- mean Rx pwr) + nominal pwr + init adjust

     and multiplied by 2 to convert to units of 0.5 dB.
    -------------------------------------------------------------------------*/

    if ((hdramac.current_subtype == HDRAMAC_SUBTYPE_ENHANCED) &&
        hdramac_ap_msg.enhanced_access_parameters_included)
    {
      /* For both 4 and 16 slot preamble length 
         one ISR will be needed to cover preamble transmission */
      hdramac.preamble_isr_cnt = 1;
    }
    else
    {
      /* Rev 0 mode */
      hdramac.preamble_isr_cnt = hdramac_ap_msg.preamble_len;
    }

    hdramac.probe_len = hdramac_ap_msg.preamble_len
                            + hdramac.num_encoder_pkts;

#ifdef FEATURE_HDR_HYBRID_ACCESS
    cycle_duration = hdramac_ap_msg.access_cycle_duration;
    if (cycle_duration == 0)
    {
      cycle_duration = 16;
    }

    hdramac.max_inter_probe_int = 
         (T_ACMP_PROBE_TIMEOUT * 16) + T_ACMP_MAX_DELAY_PREV_PROBE +
         (MAX (hdramac.config->initial.probe_sequence_backoff,
               hdramac.config->initial.probe_backoff) * cycle_duration);
#endif /* FEATURE_HDR_HYBRID_ACCESS */

#ifdef FEATURE_FACTORY_TESTMODE
    if(ftm_get_mode() != FTM_MODE)
#endif
    {
      if ( hdrovhd_get_color_code( &hdramac.color_code,
           HDRHAI_AC_MAC_PROTOCOL ) != E_SUCCESS )
      {
        ERR_FATAL("Can't retrieve color code", 0, 0, 0);
      }

      if ( hdrovhd_get_sector_id_24( &hdramac.sector_id,
            HDRHAI_AC_MAC_PROTOCOL ) != E_SUCCESS )
      {
        ERR_FATAL("Can't retrieve sector_id", 0, 0, 0);
      }

      HDR_MSG_PROT_2( MSG_LEGACY_HIGH, "Attempt: Color=0x%x, SectorID=0x%x",
                      hdramac.color_code, hdramac.sector_id );
    }

    /* Schedule probe for next access cycle */
    hdramac_scheduler(0,hdramac_initial_slot_wait);

    /*-------------------------------------------------------------------
     Initialize the access attempt specific fields in the access attempt
     data structure, and proceed to the pre-sequence persistence test
     substate.
    -------------------------------------------------------------------*/
    hdramac.elapsed_frames = 0;
    hdramac.probe_cnt      = 0;
    hdramac.seq_cnt        = 0;
    hdramac.persist_cnt    = 0;

    hdramac.substate = HDRAMAC_ACC_PERSIST_SUBSTATE;
    return;
  }

  /*-----------------------------------------------------------------------
   Pre-sequence persistence test substate.  Perform the pre-sequence
   persistence test to determine whether to begin the access probe sequence
   in the next Access Channel Slot, or to wait until the following Access
   Channel Slot and repeat the persistence test.  The persistence countdown
   expires in the second to last frame of the Access Channel Slot to allow
   time for the transmitter setup operations that must occur prior to the
   beginning of an access probe transmission.
  -----------------------------------------------------------------------*/
  if ( hdramac.substate == HDRAMAC_ACC_PERSIST_SUBSTATE )
  {
    uint32 wait;           /* Frames to wait to avoid RSI */
    qword time_now;     /* Current time */

    if (hdramac.abort_pending)
    {

      /*-------------------------------------------------------------------
       Because acmac and SLP run in the same task, the SLP processing of
       the STOPPED or FAILED indications happens in-line, and the updating
       of the SLP callback queue must happen before the indication.
      -------------------------------------------------------------------*/
      (void) hdrpcp_get_pkt( 0, HDRHAI_AC, hdramac.elapsed_frames,
                      NULL, NULL );

      hdramac.elapsed_frames=0;
      
      /* We got a stop probe command...stop attempt. */
      hdramac_attempt_cleanup();

      hdrind_give_ind( HDRIND_ACMAC_TRANSMISSION_STOPPED, NULL );

      /* Signal that Tx has ended */
      hdrind_give_ind( HDRIND_ACMAC_TX_ENDED, NULL );

      return;
    }

    /* Check that probe will not overlap with Reverse Silence
       Interval */
    (void) hdrts_get_frame_time(time_now);
    hdramac_compute_silence_time();
    wait = hdramac_check_reverse_silence_wait(time_now);

    /* If necessary, reschedule for next available cycle */
    if (wait)
    {
      hdramac_scheduler(0,wait*16);
      HDR_MSG_PROT_1( MSG_LEGACY_MED, "Waiting %d frames (Silence Interval)",
                      wait );
      return;
    }

    if ( hdrmc_feature_is_enabled(HDRMC_FEATURE_JCDMA) && 
         !hdrstream_stream_is_assigned (HDRHAI_APP_PACKET_APP_ANY) )
    {
      /* If a stream is not assigned implies session is not negotiated,
         skip persistence test */

      HDR_MSG_PROT( MSG_LEGACY_HIGH,
                      "Ignoring persistence tests. Proceeding..." );

      hdramac.persist_cnt = 0;
      /*-----------------------------------------------------------------
       Persistence test Ignored.  Clear the probe count and the countdown
       counter, and start the next access sequence.
      -----------------------------------------------------------------*/
      hdramac.probe_cnt = 0;
      hdramac.countdown = 0;
      hdramac.seq_cnt++;

#ifdef FEATURE_HDR_SELECTIVE_TXD
      if ( hdramac.seq_cnt > 1 )
      {
        hdrsrchmac_next_acc_seq();
      }
#endif /* FEATURE_HDR_SELECTIVE_TXD */
      hdramac.substate = HDRAMAC_ACC_TX_SETUP_SUBSTATE;

#ifdef FEATURE_HDR_HYBRID_ACCESS
      /* If Hybrid Access is enabled, enter tune_away substate,
         unless FTM is enabled. */
#ifdef FEATURE_FACTORY_TESTMODE
      if (ftm_get_mode() != FTM_MODE)
#endif /* FEATURE_FACTORY_TESTMODE */
      {
        hdramac.substate = HDRAMAC_ACC_TUNE_AWAY_SUBSTATE;
      }
#endif /* FEATURE_HDR_HYBRID_ACCESS */

    }
    else
    {
      /* Persistance test */
      hdramac.persist_cnt++;

      if (( hdrutil_get_random() < hdramac.persist_prob ) ||
         (hdramac.persist_cnt > hdramac.persist_limit))
      {

        if (hdramac.persist_cnt > hdramac.persist_limit)
        {
          HDR_MSG_PROT( MSG_LEGACY_HIGH,
                        "Max persistence tests. Proceeding..." );
        }
        else
        {
          HDR_MSG_PROT_2( MSG_LEGACY_HIGH, "Persistence passed test %d of %d",
                          hdramac.persist_cnt,hdramac.persist_limit );
        }
        /* Give persistence passed event */
        HDRTRACE_AMAC_PERSISTENCE_PASSED();

        hdramac.persist_cnt = 0;
        /*-----------------------------------------------------------------
         Persistence test passed.  Clear the probe count and the countdown
         counter, and start the next access sequence.
        -----------------------------------------------------------------*/
        hdramac.probe_cnt = 0;
        hdramac.countdown = 0;
        hdramac.seq_cnt++;

#ifdef FEATURE_HDR_SELECTIVE_TXD
        if ( hdramac.seq_cnt > 1 )
        {
          hdrsrchmac_next_acc_seq();
        }
#endif /* FEATURE_HDR_SELECTIVE_TXD */

        hdramac.substate = HDRAMAC_ACC_TX_SETUP_SUBSTATE;

#ifdef FEATURE_HDR_HYBRID_ACCESS
        /* If Hybrid Access is enabled, enter tune_away substate,
           unless FTM is enabled. */
#ifdef FEATURE_FACTORY_TESTMODE
        if (ftm_get_mode() != FTM_MODE)
#endif /* FEATURE_FACTORY_TESTMODE */
        {
          hdramac.substate = HDRAMAC_ACC_TUNE_AWAY_SUBSTATE;
        }
#endif /* FEATURE_HDR_HYBRID_ACCESS */
      }
      else
      {
        /*-----------------------------------------------------------------
         Persistence test failed.  Try the persistence test again in the
         the next Access Channel Slot.
        ----------------------------------------------------------------*/
        HDR_MSG_PROT_2( MSG_LEGACY_HIGH, "Persistence failed test %d of %d",
                        hdramac.persist_cnt,hdramac.persist_limit );

        /* Give persistence failed event */
        HDRTRACE_AMAC_PERSISTENCE_FAILED();

        /* Schedule us to try again next cycle. */
        hdramac_scheduler(0,0);
        return;
      }
    }
  }

#ifdef FEATURE_HDR_HYBRID_ACCESS
  /*-----------------------------------------------------------------------
   Tune away substate. Wait here for a new tune away command. 
  -----------------------------------------------------------------------*/
  if ( hdramac.substate == HDRAMAC_ACC_TUNE_AWAY_SUBSTATE )
  {
    boolean reschedule = FALSE;         /* re-schedule probe? */
    uint32  remaining_probe_length = 0;

    if (hdramac.tuneaway_received == TRUE)
    {
      if ((hdramac.reject_tuneaway == FALSE) &&
          (hdramac.no_tune_away    == FALSE))
      {
        /*------------------------------------------------------------------
          new tune away command received and tune away scheduled. There is
          chance that a probe could be sent before tune away occurs. Check 
          if probe conflicts with tune away.
        -------------------------------------------------------------------*/
        /* get estimate for probe length */
        remaining_probe_length = hdramac_estimate_probe_length (); 

        /* check if estimated probe length overlaps with tuneaway time */
        reschedule = hdramac_check_tuneaway_overlap (remaining_probe_length);
      }
    }
    else
    {
      /* still waiting on tune away command. re-schedule probe. */
      reschedule = TRUE;

      HDR_MSG_PROT (MSG_LEGACY_HIGH,
                    "Waiting in TUNE_AWAY for tune away cmd, RESCHEDULING!");
    }

    if (reschedule)
    {
      /*------------------------------------------------------------------
        waiting on tune away command or probe overlaps with tune away.
        re-schedule probe for next access cycle. Current probe is delayed.
      -------------------------------------------------------------------*/
      uint32  wait;          /* Frames to wait to avoid RSI */
      qword   current_time;  /* Current time */

      (void) hdrts_get_frame_time (current_time);

      hdramac_compute_silence_time ();

      wait = hdramac_check_reverse_silence_wait (current_time);

      /* reschedule for next available cycle */
      hdramac_scheduler (0, wait*16);

      /* processed latest tune away command */
      hdramac.tuneaway_received = FALSE;

      /* probe is delayed, note for logging purposes. */
      hdramac.probe_delayed = TRUE;

      return;
    }
    else
    {
      /* tune away not scheduled OR probe tx and tune away do not overlap OR
         reject all tune away commands, continue with probe */ 

      hdramac.substate = HDRAMAC_ACC_TX_SETUP_SUBSTATE;

      HDR_MSG_PROT (MSG_LEGACY_HIGH, "Proceeding to TX_SETUP from TUNE_AWAY");
    }
  }
#endif /* FEATURE_HDR_HYBRID_ACCESS */

  /*-----------------------------------------------------------------------
   Transmitter setup substate.  Setup the AGC power level and modulator
   hardware for the upcoming access probe.  The operations of this substate
   are performed in the second to last frame before the beginning of the
   next Access Channel Slot to provide the two frames that are required
   for the modulator hardware setup operations prior to the beginning of
   the access probe transmission.
  -----------------------------------------------------------------------*/
  if ( hdramac.substate == HDRAMAC_ACC_TX_SETUP_SUBSTATE )
  {

#ifdef FEATURE_HDR_PN_DELAY_SUPPORT
    #error code not present
#endif /* FEATURE_HDR_PN_DELAY_SUPPORT */

    /*---------------------------------------------------------------------
     Initialize the frame number such that the frame number will be equal
     to one for the first frame of the next Access Channel Slot.
    ---------------------------------------------------------------------*/
    hdramac.frame_num = 1 - HDRAMAC_TX_SETUP_FRAMES;

    /*---------------------------------------------------------------------
     Determine the access probe transmit power level based on the initial
     AGC transmit power and the number of probes that have already been
     sent.
    ---------------------------------------------------------------------*/

    /* Pwr_step is in .5 dB */
    hdramac.tx_pwr = (int16) ( (hdramac.probe_init_adjust * 2)
                 + (hdramac.probe_cnt * hdramac_ap_msg.power_step) );

    /* Set Tx power, in .5 dB. */
    if( rfm_hdr_set_tx_power_adjust( hdrsrchrf_get_rf_dev(HDRSRCHRF_TX_CHAIN), 
                                     hdramac.tx_pwr ) 
        == TRUE)
    {
      HDR_MSG_PROT_1( MSG_LEGACY_HIGH, "Adjusting Tx power by %d dB",
                      hdramac.tx_pwr / 2 );
    }
    else
    {
      HDR_MSG_PROT_1( MSG_LEGACY_HIGH, "Probe@max power (tried %d dB)",
                      hdramac.tx_pwr /2 );
    }

    if (hdramac.current_subtype == HDRAMAC_SUBTYPE_ENHANCED)
    {
      hdramac_update_probe_num_and_fcs();
    }

    if (log_status(LOG_HDR_AC_MAC_CAPSULE_C))
    {
      LOG_HDR_AC_MAC_CAPSULE_C_type *log_ptr;

      /* Get log record */
      log_ptr = (LOG_HDR_AC_MAC_CAPSULE_C_type *)
                log_alloc(LOG_HDR_AC_MAC_CAPSULE_C,
                sizeof(LOG_HDR_AC_MAC_CAPSULE_C_type)+hdramac.data_bytes-1);

      if (log_ptr != NULL)
      {
        log_ptr->pkt_length = hdramac.data_bytes;

        /* Extract into buffer */
        (void) dsm_extract(hdramac.pkt_ptr, 0, (void*)&log_ptr->data[0],
                    hdramac.data_bytes);

        log_commit((log_type*)log_ptr);
      }
    }

#ifdef FEATURE_FACTORY_TESTMODE
    if(ftm_get_mode() == FTM_MODE)
    {
        hdrhitmsg_set_pn_long_code_mask();
    }
    else
#endif
    {
      hdrenc_init();
      hdramac_set_iq_mask();
    }

    hdramac.substate = HDRAMAC_ACC_TX_PRE_SUBSTATE;

    hdramac.data_sent = 0;

  }

  /*-----------------------------------------------------------------------
   Access probe substates.  The access probe is transmitted during these
   substates.  The following operations begin in the last frame of the
   previous Access Channel Slot and finish in the last frame of the Access
   Channel Slot in which the probe is transmitted.
  -----------------------------------------------------------------------*/
  if ( ( hdramac.substate == HDRAMAC_ACC_TX_PRE_SUBSTATE  ) ||
       ( hdramac.substate == HDRAMAC_ACC_TX_DATA_SUBSTATE ) ) 
  {
    uint8  slot_length  = 16; /* preamble length in slots */
    int packet_num = 0;
    int32 pilot_power, total_power;
    uint8  payload_size = 0; /* max payload_size = 1000/8 bytes */
    qword current_frametime; /* Current time */
#ifdef FEATURE_HDR_RUMI
    uint8 i;
#endif /* FEATURE_HDR_RUMI */
    /*---------------------------------------------------------------------
     Last frame of the previous Access Channel Slot.  Program the RRI and
     enable the modulator to begin access probe transmission. The modulator
     hardware is updated on the next frame boundary.
    ---------------------------------------------------------------------*/
    if ( hdramac.frame_num == 0 )
    {
      if ((hdramac.current_subtype == HDRAMAC_SUBTYPE_ENHANCED) &&
          hdramac_ap_msg.enhanced_access_parameters_included)
      {
        if (hdramac_ap_msg.preamble_length_slots == 1)
        {
          hdrmod_set_tx_frame_interrupt_offset( 
            (hdramac.tx_offset + HDRAMAC_FRAME_INTERRUPT_ENC_SLOT) % 16 );
        }
        else
        {
          /* --------------------------------------------------------------
             schedule next interrupt to trigger in the slot where TX frame 
             offset is currently set to begin. This slot coincides with the 
             start of preamble and 4 slots prior to probe data TX begins.
           ----------------------------------------------------------------*/
          slot_length = 4;
          hdrmod_set_tx_frame_interrupt_offset ( hdramac.tx_offset % 16 );
        }
      }
      else
      {
        hdrmod_set_tx_frame_interrupt_offset( 
          (hdramac.tx_offset + HDRAMAC_FRAME_INTERRUPT_ENC_SLOT) % 16 );
      }

      /* Enable FW mod in access mode */
      hdramac_enable_carrier_access_mode();

      /* Install firmware logging callback. */
      hdramac_first_gain_log = TRUE;
      hdrlog_register_slot_logging_cb ( HDRLOG_AMAC_MDSP_SLOT_LOGGING_CLIENT,
                                        hdramac_process_mdsp_log_event );

      hdrmod_setup_tx_timed_enable( FALSE, hdramac.tx_frame_start_offset );

      /* Increment probe count as soon as new probe begins. */
      hdramac.probe_cnt++;

      HDR_MSG_PROT_3( MSG_LEGACY_HIGH,
                      "Probe start: %d of seq %d (PA on), Preamble=%d slots",
                      hdramac.probe_cnt, hdramac.seq_cnt, slot_length );


      HDR_MSG_PROT_4( MSG_LEGACY_HIGH,
                      "CycleDur: %d, CycleNum %x. SchedFrame hi 0x%x, lo 0x%x",
                      hdramac_ap_msg.access_cycle_duration, hdramac.pn_cycle_num, qw_hi(hdramac.scheduled_time), qw_lo(hdramac.scheduled_time));

      (void) hdrts_get_frame_time( current_frametime );

      HDR_MSG_PROT_2( MSG_LEGACY_HIGH,
                      "Acc probe info: Now: 0x%x, 0x%x",
                      qw_hi(current_frametime),qw_lo(current_frametime) );

      /* log probe number and the sequence number this probe is part of */
      hdramac_probe_log.probe_log.version       = 0;
      hdramac_probe_log.probe_log.reserved      = 0;
      hdramac_probe_log.probe_log.probe_num     = hdramac.probe_cnt;
      hdramac_probe_log.probe_log.probe_seq_num = hdramac.seq_cnt;

    }

    if ( hdramac.frame_num == 1 )
    {
      if ((hdramac.current_subtype == HDRAMAC_SUBTYPE_ENHANCED) &&
          hdramac_ap_msg.enhanced_access_parameters_included &&
          (hdramac_ap_msg.preamble_length_slots == 0))
      {
        /* shift frame offset by 4 slots for 4 slots preamble probe */
        hdramac.tx_frame_start_offset = ( hdramac.tx_offset + 4 ) % 16;
        hdrmod_set_tx_frame_start_offset( hdramac.tx_frame_start_offset );

        /* Log power duration and level for short preamble. */
        hdrtx_update_tx_power_stats_with_duration( 4 );
      }
      else
      {
         /* Log power duration and level. */
        hdrtx_update_tx_power_stats_with_duration( 16 );
      }
    } 
    else if ( hdramac.frame_num > 1 ) 
    {
      /* Log power duration and level. */
      hdrtx_update_tx_power_stats_with_duration( 16 );
    }

    /*---------------------------------------------------------------------
     Last preamble frame of the probe.  Load the encoder and set the RRI.
     Transmission of data begins on the next frame boundary.
    ---------------------------------------------------------------------*/
    if (( hdramac.frame_num >= hdramac.preamble_isr_cnt )
        && ( hdramac.data_sent < hdramac.data_bytes ))
    {
      hdrmod_rri_enum_type  probe_frame_rate = REVERSE_RATE_9600;

      /* Break Access Capsule into packets to send to encoder */
      {
        packet_num = hdramac.frame_num - hdramac.preamble_isr_cnt;
 
#ifndef FEATURE_HDR_RUMI
        hdramac_eac_frame_rate (&payload_size, &probe_frame_rate);

        (void) dsm_extract( hdramac.pkt_ptr,
                       hdramac.data_sent,
                       hdramac_pkt,
                       payload_size);

        HDR_MSG_PROT_3( MSG_LEGACY_HIGH,
                        "Wrote pkt #%d to encoder, size %d bytes, rate %d",
                        packet_num, payload_size, probe_frame_rate );

        hdrenc_send_frame( hdramac_pkt, payload_size, 0, probe_frame_rate);
#else /* FEATURE_HDR_RUMI */

        if ( hdramac_ap_msg.enhanced_access_parameters_included && 
             ( hdramac_ap_msg.sector_access_max_rate != 0 ) )
        {
          probe_frame_rate = hdramac_ap_msg.sector_access_max_rate + 1;
        }

        switch ( probe_frame_rate )
        {
         case REVERSE_RATE_19200:
           hdramac.data_bytes = HDRAMAC_PKT_19K2_FRAME_DATA_LEN;
           break;

         case REVERSE_RATE_38400:
           hdramac.data_bytes = HDRAMAC_PKT_38K4_FRAME_DATA_LEN;
           break;

         case REVERSE_RATE_9600: 
           HDR_MSG_PROT_1( MSG_LEGACY_ERROR,
                           "RUMI: Invalid access rate:%d, force to 9.6K",
                           probe_frame_rate );
           /* Intentional fall thru */

         default:
           hdramac.data_bytes = HDRAMAC_PKT_9K6_FRAME_DATA_LEN;
           break;
        }

        payload_size = hdramac.data_bytes;

        for ( i=0; i<hdramac.data_bytes; i++)
        {
          hdramac_pkt[i]= hdrhitmsg_get_data_pkt_at_pos(i);
        }

        //hdramac_eac_frame_rate (&payload_size, &probe_frame_rate);

        HDR_MSG_PROT_3( MSG_LEGACY_HIGH,
                        "RUMI TX only: Wrote pkt #%d to encoder, size %d bytes, rate %d",
                        packet_num, payload_size, probe_frame_rate );
        
        hdrenc_send_frame( hdramac_pkt, payload_size, 0xc0, probe_frame_rate);
#endif /* FEATURE_HDR_RUMI_TX_ONLY */

        /*----------------------------------------------------------------
                 update logging information for this access probe. 
        ----------------------------------------------------------------*/

        pilot_power = rfm_hdr_get_tx_pilot_power( hdrsrchrf_get_rf_dev(HDRSRCHRF_TX_CHAIN), 
                                                  RFM_HDR_CARRIER_ID__0, 
                                                  RFM_HDR_AGC_FORMAT__DBM256 );

        pilot_power += (pilot_power<0)?-128:128;

        total_power = rfm_hdr_get_tx_total_power( hdrsrchrf_get_rf_dev(HDRSRCHRF_TX_CHAIN), 
                                                  RFM_HDR_CARRIER_ID__0, 
                                                  RFM_HDR_AGC_FORMAT__DBM256 );

        total_power += (total_power<0)?-128:128;

        if (hdramac.frame_num == hdramac.preamble_isr_cnt)
        {
           /* last preamble frame, log preamble power */
          hdramac_probe_log.probe_log.preamble_pwr = total_power/256;
        }
        else
        {
          /*----------------------------------------------------------------
            current pilot and total power measurements are for the frame 
            built in the PREVIOUS frame, hence filling information for 
            (packet_num - 1)
          ----------------------------------------------------------------*/

          hdramac_probe_log.probe_log.frame_info[packet_num - 1].pilot_power =
                                                pilot_power/256;

          hdramac_probe_log.probe_log.frame_info[packet_num - 1].total_power =
                                                total_power/256;
        }

        /*------------------------------------------------------------------
          PA headroom, payload size, probe rate information is for the frame 
          to be transmitted in the NEXT frame, hence filling information for
          (packet_num)
         ------------------------------------------------------------------*/
        hdramac_probe_log.probe_log.frame_info[packet_num].pa_headroom = 
                                                HDRAGC_GET_MAX_RRI();
        hdramac_probe_log.probe_log.frame_info[packet_num].frame_data_size =
                                                payload_size;
        hdramac_probe_log.probe_log.frame_info[packet_num].frame_rate =
                                                probe_frame_rate;

        /* packet number starts with 0 */
        hdramac_probe_log.probe_log.frame_count = packet_num + 1;
      }

      hdramac.substate = HDRAMAC_ACC_TX_DATA_SUBSTATE;

      HDRTRACE_AMAC_SENDING_PROBE( (int) (hdramac.tx_pwr / 2) );
    }

    /*---------------------------------------------------------------------
     Last data frame of the probe.  Turn off the transmitter.  This
     operation turns off the transmitter on the next frame boundary.
    ---------------------------------------------------------------------*/
    if ( hdramac.data_sent == hdramac.data_bytes )
    {
      /* last frame in the probe */
      uint8  last_frame = hdramac_probe_log.probe_log.frame_count - 1;

      /* Set RRI back to zero */
      hdrmod_set_rri(0);

#ifndef FEATURE_HDR_UNIFIED_TX
#ifndef FEATURE_HDR_BOLT_MODEM
      /* Ensure DSP gets rri update by faking encode. */
      HWIO_REVMOD_ENC_RATE_OUT(1);

      /* Set end-of-frame bit to trigger encode. */
      HWIO_REVMOD_EDI_CONTROL_OUT(2);
#endif /* FEATURE_HDR_BOLT_MODEM */
#endif /* FEATURE_HDR_UNIFIED_TX */

      /* Disable PA */
      hdrmod_setup_tx_timed_disable( FALSE, hdramac.tx_frame_start_offset);

      HDR_MSG_PROT( MSG_LEGACY_HIGH, "Disabling PA" );

      hdramac.last_pwr = hdramac.tx_pwr;
      
      /*---------------------------------------------------------------------
        delay the inter-probe, inter-sequence scheduling until PA is turned
        off which happens at frame offset. For probes with 4 slots preamble
        length if scheduling is not delayed until PA is off, frame offset 
        changes and PA remains on for longer than necessary period.
       --------------------------------------------------------------------*/
      if ((hdramac.current_subtype == HDRAMAC_SUBTYPE_ENHANCED) &&
          hdramac_ap_msg.enhanced_access_parameters_included)
      {
        if (hdramac_ap_msg.preamble_length_slots == 1)
        {
          /* frame offset and tx_offset identical */
          hdrmod_set_tx_frame_interrupt_offset( hdramac.tx_offset % 16 );
        }
        else
        {
          /* 4 slots preamble case: frame offset is 4 slots after tx_offset */
          hdrmod_set_tx_frame_interrupt_offset ( (hdramac.tx_offset + 4) % 16 );
        }
      }
      else
      {
        /* frame offset and tx_offset identical */
        hdrmod_set_tx_frame_interrupt_offset( hdramac.tx_offset % 16 );
      }

      /*----------------------------------------------------------------
         log the pilot and total power for the FINAL frame
       ----------------------------------------------------------------*/

      pilot_power = rfm_hdr_get_tx_pilot_power( hdrsrchrf_get_rf_dev(HDRSRCHRF_TX_CHAIN), 
                                                RFM_HDR_CARRIER_ID__0, 
                                                RFM_HDR_AGC_FORMAT__DBM256 );

      pilot_power += (pilot_power<0)?-128:128;
      hdramac_probe_log.probe_log.frame_info[last_frame].pilot_power =
                  pilot_power/256;

      total_power = rfm_hdr_get_tx_total_power( hdrsrchrf_get_rf_dev(HDRSRCHRF_TX_CHAIN), 
                                                RFM_HDR_CARRIER_ID__0, 
                                                RFM_HDR_AGC_FORMAT__DBM256 );

      total_power += (total_power<0)?-128:128;
      hdramac_probe_log.probe_log.frame_info[last_frame].total_power =
                  total_power/256;

      /* fill in whether probe was delayed or not */
      hdramac_probe_log.probe_log.probe_delayed = hdramac.probe_delayed;

      /* send access probe log */
      hdramac_log_access_probe();

      /* Logging information... */
      hdramac_attempt_log.probe_count[hdramac.seq_cnt-1]
        = (uint8) hdramac.probe_cnt;

      hdramac.substate = HDRAMAC_ACC_TX_DONE_SUBSTATE;
    }

    /* Print Tx AGC on F3 messages - remove in future */
    if ((hdramac.frame_num > 0) && (hdramac.data_sent <= hdramac.data_bytes))
    {
      hdrtx_report_agc( 0 );
    }

    /*---------------------------------------------------------------------
     Increment the access probe frame number each time through any of the
     access probe transmission substates.
    ---------------------------------------------------------------------*/
    hdramac.frame_num++;

    /* update number of bytes sent */
    hdramac.data_sent += payload_size;

    /* done with probe, reset probe delayed information. */
    hdramac.probe_delayed = FALSE;

    return;
  }

  /*-------------------------------------------------------------------------
   PA is now off, proceed with inter-probe/inter-sequence backoff
  -------------------------------------------------------------------------*/
  if ( hdramac.substate == HDRAMAC_ACC_TX_DONE_SUBSTATE )
  {

    /* Remove firmware logging callback, probe is complete. */
    hdrlog_register_slot_logging_cb ( HDRLOG_AMAC_MDSP_SLOT_LOGGING_CLIENT,
                                      NULL );

    /*-------------------------------------------------------------------
     Proceed with the required backoff operation.  If there are more
     probes in the sequence, then proceed with the inter-probe backoff.
     Otherwise, proceed with the inter-sequence backoff.
    -------------------------------------------------------------------*/
    if (( hdramac.probe_cnt < hdramac_ap_msg.probe_num_step ) &&
          (!hdramac.abort_pending))
    {
      hdramac.substate = HDRAMAC_ACC_PRB_BKOFF_SUBSTATE;
    }
    else
    {
      hdramac.substate = HDRAMAC_ACC_SEQ_BKOFF_SUBSTATE;
    }
  }

  /*-----------------------------------------------------------------------
   Inter-probe backoff setup substate.  Setup the number of backoff frames
   to wait before beginning the next probe.  Proceed to the inter-probe
   backoff countdown substate to begin the countdown in the current frame.

   Determine the inter-probe backoff countdown, in frames, according to
   the following:

     Backoff = ( RAN * Config.ProbeBackoff ) * AccessCycleDuration
               + T_ProbeTimeout

     where RAN is an evenly distributed random number between 0 and 1.

   The number of frames required for transmitter setup is subtracted from
   the total frame count to allow the final two frames of the last Access
   Channel Slot to be used for transmitter setup for the next probe.
  -----------------------------------------------------------------------*/
  if ( hdramac.substate == HDRAMAC_ACC_PRB_BKOFF_SUBSTATE )
  {
    int loop_count = 0;
      /* Loop count for RLSI conflict avoidance. */

    uint32  min_slots = T_ACMP_PROBE_TIMEOUT * 16;
      /* Adjusted PROBE_TIMEOUT, per standard */

    uint32  reverse_silence_wait = 0;
      /* Frames to wait to avoid RLSI */  

    /* --- */

    if ((hdramac.current_subtype == HDRAMAC_SUBTYPE_ENHANCED) && 
        hdramac_ap_msg.enhanced_access_parameters_included)
    {
      /*-------------------------------------------------------------- 
         Reduce inter-probe backoff interval by adjustment specified 
         through probe timeout adjust parameter. Currently the maximum
         permissible value for probe timeout adjust is 112 slots and 
         T_ACMP_PROBE_TIMEOUT is defined to be 128 slots.
         So min_slots would not get assigned a negative value.
         Nevertheless, a sanity check...
      ---------------------------------------------------------------*/

      if (min_slots > (uint32)(hdramac_ap_msg.probe_time_out_adjust * 16))
      {
        /* probe timeout adjust values are in units of 16 slots. */
        min_slots -= (hdramac_ap_msg.probe_time_out_adjust * 16);
      }
      else
      {
        HDR_MSG_PROT_1( MSG_LEGACY_ERROR,
                        "Bogus value in AP:ProbeTimeOutAdjust: %d",
                        hdramac_ap_msg.probe_time_out_adjust * 16);
      }
      
    }

    /* Check next RSI time */
    hdramac_compute_silence_time();

    hdramac.backoff_cycles = 0;

    /* Loop until probe does not interfere with RSI */
    do {

      loop_count++;

      hdramac.backoff_cycles += ran_dist( hdrutil_get_random(), 0,
         hdramac.config->initial.probe_backoff+1);

      /* Schedule next probe */ 
      hdramac_scheduler( hdramac.backoff_cycles, min_slots);

      /* Check for conflict with RLSI */
      reverse_silence_wait = 
        hdramac_check_reverse_silence_wait(hdramac.scheduled_time);

      /* Special case: If ProbeBackoff = 0, the same backoff value will
         be chosen every time through the loop. For this case, we have
         to push things along by adding reverse_silence_wait */
      if ((hdramac.config->initial.probe_backoff == 0) &&
          (reverse_silence_wait != 0))
      {
        /* Add wait time to get out of RLSI. Round up. */
        hdramac.backoff_cycles += (
             ((reverse_silence_wait * 16) + 
               (hdramac_ap_msg.access_cycle_duration - 1)) /
             hdramac_ap_msg.access_cycle_duration);

        HDR_MSG_PROT_1( MSG_LEGACY_HIGH, 
                        "RLSI conflict, Probe Backoff config'd to 0: add %d",
                        reverse_silence_wait );
      }
    } while (reverse_silence_wait != 0  );

    HDR_MSG_PROT_2( MSG_LEGACY_HIGH, "Probe Backoff: %d (%d tries)",
                    hdramac.backoff_cycles,
                    loop_count );

    hdramac.substate = HDRAMAC_ACC_TX_SETUP_SUBSTATE;

#ifdef FEATURE_HDR_HYBRID_ACCESS
    /* If Hybrid Access is enabled, enter tune_away substate,
       unless FTM is enabled. */
#ifdef FEATURE_FACTORY_TESTMODE
    if (ftm_get_mode() != FTM_MODE)
#endif /* FEATURE_FACTORY_TESTMODE */
    {
      /* Transition to TUNE_WAY state. If no tuneaway is scheduled, it
         will drop through to TX_SETUP state. */
      hdramac.substate = HDRAMAC_ACC_TUNE_AWAY_SUBSTATE;
    }
#endif /* FEATURE_HDR_HYBRID_ACCESS */

    return;
  }

  /*-----------------------------------------------------------------------
   Inter-sequence backoff setup substate.  Setup the number of backoff
   frames to wait before proceeding with the next probe sequence.  If the
   maximum number of probe sequences have been transmitted, then wait for
   the ACAck message.  Proceed to the inter-sequence backoff countdown
   substate, or the final probe response substate, to begin the countdown
   in the current frame.

   Determine the inter-sequence backoff countdown, in frames, according to
   the following

     Backoff = ( RAN * Config.ProbeSequenceBackoff ) * AccessCycleDuration
               + T_ProbeTimeout

     where RAN is an evenly distributed random number between 0 and 1.

   The number of frames required for transmitter setup is subtracted from
   the total frame count to allow the final two frames of the last Access
   Channel Slot to be used for transmitter setup for the next probe.
  -----------------------------------------------------------------------*/
  if ( hdramac.substate == HDRAMAC_ACC_SEQ_BKOFF_SUBSTATE )
  {
    if (( hdramac.seq_cnt < hdramac.config->initial.probe_sequence_max )
        && (!hdramac.abort_pending))
    {

      hdramac.backoff_cycles = ran_dist( hdrutil_get_random(), 0,
         hdramac.config->initial.probe_sequence_backoff+1);

      HDR_MSG_PROT_1( MSG_LEGACY_HIGH, "Sequence Backoff: %d",
                      hdramac.backoff_cycles );

      hdramac_scheduler( hdramac.backoff_cycles,
                         T_ACMP_PROBE_TIMEOUT*16);

      hdramac.substate = HDRAMAC_ACC_PERSIST_SUBSTATE;
      return;
    }
    else
    {
      hdramac.substate = HDRAMAC_ACC_RX_ACK_SUBSTATE;
    }
  }

  /*-----------------------------------------------------------------------
   ACAck timeout setup substate.  Setup the number of frames to wait for
   the ACAck message after sending the last access probe of the last access
   sequence.  Proceed to the ACAck timeout countdown substate to begin the
   countdown in the current frame.
  -----------------------------------------------------------------------*/
  if ( hdramac.substate == HDRAMAC_ACC_RX_ACK_SUBSTATE )
  {
    hdramac.countdown = T_ACMP_CYCLE_LEN + T_ACMP_PROBE_TIMEOUT;
    hdramac.substate = HDRAMAC_ACC_RX_ACK2_SUBSTATE;
  }

  /*-----------------------------------------------------------------------
   ACAck timeout countdown substate.  Countdown the number of frames
   determined in the ACAck timeout setup substate before returning the
   TxFailed indication.  If an ACAck message is received before the
   countdown reaches zero, then the substate of the access attempt will be
   changed as part of the ACAck message processing to prevent the TxFailed
   indication from being returned.
  -----------------------------------------------------------------------*/
  if ( hdramac.substate == HDRAMAC_ACC_RX_ACK2_SUBSTATE )
  {
    if ( hdramac.countdown-- == 0 )
    {
      hdramac.countdown = 0;

      /*-------------------------------------------------------------------
       Because acmac and SLP run in the same task, the SLP processing of
       the STOPPED or FAILED indications happens in-line, and the updating
       of the SLP callback queue must happen before the indication.
      -------------------------------------------------------------------*/
      (void) hdrpcp_get_pkt( 0, HDRHAI_AC, hdramac.elapsed_frames,
                      NULL, NULL );

      hdramac.elapsed_frames=0;

      if (!hdramac.abort_pending)
      {
        /* Disable probe transmission and clean up. 
           The function clears abort_pending flag and hence cannot be 
           shifted above this conditional. */
        hdramac_attempt_cleanup();

        ind_data.access_inhibited = FALSE;
        hdrind_give_ind( HDRIND_ACMAC_TRANSMISSION_FAILED, (void*)&ind_data );
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
        hdr_qsh_event_notify(hdrutil_get_hdr_sub_id(),QSH_CLT_HSRCH, HSRCH_QSH_EVENT_ACCESS_FAILURE);
#endif
        hdramac_attempt_log.result = HDRLOG_AMAC_ACACK_NOT_RECEIVED;
        HDRTRACE_AMAC_MAX_PROBES();
      }
      else
      {
        /* Disable probe transmission and clean up. */
        hdramac_attempt_cleanup();

        hdrind_give_ind( HDRIND_ACMAC_TRANSMISSION_STOPPED, NULL );
        hdramac_attempt_log.result = HDRLOG_AMAC_PROBE_INTERRUPTED;
        HDRTRACE_AMAC_PROBING_STOPPED();
      }

      hdramac_log_access_attempt();

      /* Signal that Tx has ended */
      hdrind_give_ind( HDRIND_ACMAC_TX_ENDED, NULL );
    }
  }

  /*-----------------------------------------------------------------------
   Access attempt finish setup substate.  Setup the number of frames to
   countdown before ending the access attempt after receiving the ACAck
   message.  Proceed to the inter-probe backoff countdown substate to begin
   the countdown in the current frame.
  -----------------------------------------------------------------------*/
  if ( hdramac.substate == HDRAMAC_ACC_END_SUBSTATE )
  {
    hdramac.countdown = T_ACMP_TRANSACTION;
    hdramac.substate  = HDRAMAC_ACC_END2_SUBSTATE;
  }

  /*-----------------------------------------------------------------------
   Access attempt finish countdown substate.  Countdown the number of frames
   determined in the finish setup substate before returning to the Start
   substate to begin the next access attempt.
  -----------------------------------------------------------------------*/
  if ( hdramac.substate == HDRAMAC_ACC_END2_SUBSTATE )
  {
    if ( hdramac.countdown-- == 0 )
    {
      hdramac.countdown = 0;
      hdramac_attempt_cleanup();

      /* Signal that Tx has ended */
      hdrind_give_ind( HDRIND_ACMAC_TX_ENDED, NULL );

    }
  }
}


/* EJECT */
/*===========================================================================

FUNCTION HDRAMAC_PROCESS_START_ATTEMPT

DESCRIPTION
  Initiate an access attempt.

DEPENDENCIES
  The ACMAC must be in the active state.

PARAMETERS
  caller - The protocol calling this externalized function (for sw trace)

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdramac_process_start_attempt( hdrhai_protocol_name_enum_type caller )
{

  /* Check that ACMAC is in the active state */
  if (hdramac_is_active(HDRHAI_AC_MAC_PROTOCOL))
  {
#ifdef FEATURE_HDR_HYBRID_ACCESS
#ifdef FEATURE_FACTORY_TESTMODE
    if (ftm_get_mode() != FTM_MODE)
#endif
    {
      if (!hdrlmac_is_in_access_state())
      {
        HDR_MSG_PROT( MSG_LEGACY_HIGH, "Abort AC MSG because lmac is idle");
        hdrind_give_ind( HDRIND_ACMAC_TRANSMISSION_ABORTED, NULL );
        HDRTRACE_AMAC_ACCESS_ABORTED();
        return;
      }
    }
#endif /* FEATURE_HDR_HYBRID_ACCESS */

    /* Are we already sending a message over the access channel?
    ** If so, the protocol will check for more messages once the
    ** current message is complete, so nothing needs to be done.
    */
    if (!hdramac.attempt_in_progress)
    {
      /* Set states and flags */
      hdramac.attempt_in_progress = TRUE;
      hdramac.abort_pending = FALSE;
      hdramac.substate = HDRAMAC_ACC_START_SUBSTATE;

      /*-------------------------------------------------------------------
      ** TxStarted indication:
      ** The TxStarted and TxEnded indications are given in a slightly
      ** different way than is defined in the protocol.  When several
      ** access attempts are made in succession, only one TxStarted
      ** indication occurs before the first attempt, and one TxEnded
      ** indication occurs when there are no more access messages to be
      ** sent.  The effect is the same, and it relieves the upper protocols
      ** from having to count how many of each indication it receives.
      */
      hdrind_give_ind( HDRIND_ACMAC_TX_STARTED, NULL );

      /* Only enable RF and init modulator if we haven't tuned away */
      if (hdramac.tuneaway_stopped == FALSE)
      {
        /* Enable RF TX -- No callback needed because we will always have
           at least 2 frames (52.33 ms) before probe transmission which is
           more than enough time for RF to warm up. */
        (void) hdrtx_enable_tx(NULL);
      
#ifdef FEATURE_HDR_BOLT_MODEM
        /* Send PN and carrier enable information to FW. PN information can be
           updated at a later point when probes are sent. The purpose of this
           call is to convey to FW the carrier to enable in microkernel. This 
           information should be available with FW before DAC start. */
        hdramac_set_iq_mask();
#endif /* FEATURE_HDR_BOLT_MODEM */

        /* Prepare modulator to start tx */
        hdrmod_tx_start();
              
        /* Update tx status. */
        hdramac.tx_enabled = TRUE;

        /*-------------------------------------------------------------------
         Modulator setup. Program the modulator to enter the access mode.
        -------------------------------------------------------------------*/
        hdrmod_enter_access_mode();

        /* Enable access mode and set gains in firmware */
        hdramac_enable_carrier_access_mode();

        HDR_MSG_PROT( MSG_LEGACY_HIGH, "ACMAC: Tx enabled");
      }

      HDR_MSG_PROT( MSG_LEGACY_HIGH, "HDRAMAC attempt starting..." );

      /* Start access probe processing */
#ifdef FEATURE_HDR_QCHAT
      if (hdrqchatupk_oftcs_enabled() || hdrqchatupk_tftcs_enabled())
      {
        hdrqchatupk_amac_process_access_attempt();
      }
      else
      {
        /* Start access probe processing */
        hdramac_process_access_attempt();
      }
#else
      /* Start access probe processing */
      hdramac_process_access_attempt();
#endif /* FEATURE_HDR_QCHAT */

      /* Install the access channel ISR if we haven't aborted for some
         reason. */
      if ((hdramac.tuneaway_stopped == FALSE) && 
          (hdramac.attempt_in_progress))
      {
        hdrmod_enable_frame_isr( hdramac_isr );
      }

      /* initialize probe delayed for logging */
      hdramac.probe_delayed = FALSE;
    }
  }
  else
  {
    ERR("Access attempt while ACMAC inactive",0,0,0);
  }

}

/* EJECT */

#ifdef FEATURE_HDR_HYBRID_ACCESS
/*===========================================================================

FUNCTION HDRAMAC_PROCESS_TUNEAWAY_CMD

DESCRIPTION
  This function processes tune away time cmd. AMAC is expected to be active
  and access attempt must be in progress.

DEPENDENCIES
  None.

PARAMETERS
  no_tune_away  - whether a tune away is scheduled or not
  tuneaway_time - time of tune away, if scheduled.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdramac_process_tuneaway_time
(
  boolean no_tune_away,
  qword   tuneaway_time
)
{
  qword  delta, curr_time;
  qword  buff_curr_time; 
    /* Current time + buffer, for checking tuneaway time */

   HDR_MSG_PROT( MSG_LEGACY_HIGH, "ACMAC: Tuneaway time cmd");

  /* process TUNEAWAY_TIME_CMD if AMAC is active */
  if (hdramac_is_active (HDRHAI_AC_MAC_PROTOCOL))
  {
    /*-----------------------------------------------------------------------
       Access attempt in progress. AMAC is either waiting in TUNE_AWAY_SUBSTATE
       for a tune away command to proceed with the next access probe or is in 
       one of the states during transmission of access probe. If AMAC is 
       waiting in TUNE_AWAY_SUBSTATE the tune away time is unconditionally 
       accepted. If AMAC is in the middle of probe transmission, check if tune 
       away occurs prior to estimated probe completion. If tune away time occurs 
       prior to probe completion, tune away is rejected. If not, tune away is 
       accepted. 
       -----------------------------------------------------------------------*/

    (void) hdrts_get_current_time (curr_time, NULL);

    HDR_MSG_PROT_2( MSG_LEGACY_HIGH, "Current Time 0x %x:%x",
                    curr_time[1], curr_time[0]);

    if (hdramac.no_tune_away == FALSE)
    {
      /* tune away occured, tune back */
      hdramac_tuneaway_resume_access ();
    }

    /* Only do the following if an attempt has been kicked off */
    if (hdramac.attempt_in_progress)
    {  
      if ((hdramac.substate == HDRAMAC_ACC_TUNE_AWAY_SUBSTATE) &&
          (hdramac.no_tune_away == FALSE)                      &&
          ((hdramac.probe_cnt > 0) || (hdramac.seq_cnt > 1)))
      {
        /*---------------------------------------------------------------------
          Access attempt has begun and one or more probes in a sequence have
          already been transmitted. AMAC had tuned away after the previous 
          probe. AMAC has received a new tune away command. Compute time 
          delta between last tune away and time when new tune away command is 
          received (current time)
        ---------------------------------------------------------------------*/

        HDR_MSG_PROT_2( MSG_LEGACY_HIGH, "last tuneaway time 0x %x:%x",
                        hdramac.tuneaway_time[1], hdramac.tuneaway_time[0]);

        qw_set (delta, 0, 0);

        if (qw_cmp (curr_time, hdramac.tuneaway_time) >= 0)
        {
          /* current time is greater than last tuneaway time */
          qw_sub (delta, curr_time, hdramac.tuneaway_time);
        }

        if (qw_hi (delta) || (qw_lo (delta) > hdramac.max_inter_probe_int))
        {
          /*-------------------------------------------------------------------
           The new tune away command is received after inter-probe timeout,
           Reject all tune away commands until probe succeeds or max probe
           failure occurs. Restart AMAC.
          -------------------------------------------------------------------*/
          hdramac.reject_tuneaway = TRUE;

          /* start over again */
          hdramac.substate = HDRAMAC_ACC_SCHEDULE_SUBSTATE;

          HDR_MSG_PROT( MSG_LEGACY_HIGH, 
            "TUNE AWAY rcvd after inter-probe timeout period, HDRAMAC Restarting");

          HDR_MSG_PROT_3( MSG_LEGACY_HIGH, 
                          "Delta 0x %x:%x, inter probe timeout 0x %x",
                             delta[1], delta[0], hdramac.max_inter_probe_int);
        }
        else
        {
          /* inter-probe timeout has not occured, AMAC can continue with 
             current access attempt */
        }
      }
    }  
    else
    {
      HDR_MSG_PROT( MSG_LEGACY_HIGH, "ACMAC: No attempt in progress. Skip probe check");
    }

    /* Locking is not needed since all accesses to the following variables
       (tuneaway_received, no_tune_away, tuneaway_time_ok, tuneaway_time)
       are contained within hdrtx task context. */
    hdramac.tuneaway_received = TRUE;
    hdramac.no_tune_away      = no_tune_away;
    hdramac.tuneaway_time_ok  = FALSE;

    /* update new tuneaway time */
    qw_equ( hdramac.tuneaway_time, tuneaway_time );


    if ( no_tune_away == TRUE )
    {
      /* no tune away scheduled */

      hdramac.tuneaway_time[0] = 0xFFFFFFFF;
      hdramac.tuneaway_time[1] = 0xFFFFFFFF;

      HDR_MSG_PROT( MSG_LEGACY_HIGH, 
                    "TUNE_AWAY cmd received. No tune away scheduled");

      hdramac.tuneaway_time_ok = TRUE;
    }
    else if ( hdramac.reject_tuneaway == TRUE )
    {
      /* reject all tune away commands until probe succeeds or 
         max probe failure occurs */
      hdramac.tuneaway_time_ok = FALSE;
    }
    else
    {
      HDR_MSG_PROT_3 (MSG_LEGACY_HIGH,
                      "TUNE_AWAY recd in %d state. Tune away time 0x %x:%x",
                       hdramac.substate, tuneaway_time[1], tuneaway_time[0]);

      /* Check sanity of tuneaway time. Should not be in past, or within
         specified time in future (as there would not be enough time to 
         setup). */
      qw_equ( buff_curr_time, curr_time );
      qw_inc( buff_curr_time, HDRAMAC_TUNEAWAY_TIME_BUFFER_IN_SLOTS );

      if ( qw_cmp ( buff_curr_time, tuneaway_time ) > 0)
      {
        HDR_MSG_PROT_2( MSG_LEGACY_HIGH, "tuneaway time too close or in the past (0x %x:%x)",
                        tuneaway_time[1], tuneaway_time[0]);

        /* current time has past the new tuneaway time! it's too late! */
        hdramac.tuneaway_time_ok = FALSE;

        /* done with tune away processing */
        hdramac.tuneaway_received = FALSE;
      }

      /* Check if a probe is in progress */
      else if ( ( hdramac.attempt_in_progress ) &&
           ( ( hdramac.substate == HDRAMAC_ACC_TX_PRE_SUBSTATE ) ||
             ( hdramac.substate == HDRAMAC_ACC_TX_DATA_SUBSTATE ) ) )
      {
        uint32  remaining_probe_length = 0;
        boolean overlap;

        /*-------------------------------------------------------------------
         AMAC is about to sent a probe, but there is a new tune away cmd.
         Does the new tune away time occur before AMAC can get done with
         transmitting the probe? Get estimate for remaining probe length 
         in slots.
         -------------------------------------------------------------------*/

        remaining_probe_length = hdramac_estimate_probe_length ();

        /* check if estimated probe length conflicts with tuneaway time */
        overlap = hdramac_check_tuneaway_overlap (remaining_probe_length);

        if (overlap == TRUE)
        {
          /* The new tune away time occurs before AMAC can finish sending the
             probe, transmitting the probe takes priority, so no tune away. */
          hdramac.tuneaway_time_ok = FALSE;

          /* done with tune away processing */
          hdramac.tuneaway_received = FALSE;
        }
        else
        {
          /* probe tx and tune away do not overlap, probe can go through 
             before start of tune away time. Accept tune away */
          hdramac.tuneaway_time_ok = TRUE;
        }
      }
      else
      {
        /*-------------------------------------------------------------------
         Access attempt in progress but there is time before the next probe
         is scheduled for Tx. Accept tune away. Consequently the next probe 
         MAY be delayed due to tune away. Whether or not the probe is delayed 
         is processed in TUNE_AWAY_SUBSTATE as there may be time to send the
         probe before tune away occurs. 
        -------------------------------------------------------------------*/
        hdramac.tuneaway_time_ok = TRUE;
      }
    }

    /* send back indication only if access attempt in progress */
    hdrind_give_ind ( HDRIND_ACMAC_TUNE_AWAY_TIME_OK,
                      &hdramac.tuneaway_time_ok );
  
    HDR_MSG_PROT_1( MSG_LEGACY_HIGH,
                    "Tune away accepted (1) / rejected (0) : %d", 
                    hdramac.tuneaway_time_ok);
  }
  else
  {
    ERR ("TUNEAWAY_TIME_CMD rcvd in AMAC inactive state", 0, 0, 0);
  }
}
#endif /* FEATURE_HDR_HYBRID_ACCESS */

/* EJECT */
/*===========================================================================

FUNCTION HDRAMAC_PROCESS_CMD

DESCRIPTION
  This function processes the commands for this protocol that were queued
  on the task command queue from a different task context.

DEPENDENCIES
  None.

PARAMETERS
  cmd_ptr - address of command structure.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdramac_process_cmd
(
  hdramac_cmd_union_type *cmd_ptr
)
{
  switch ( cmd_ptr->hdr.cmd )
  {
    case HDRAMAC_ACTIVATE_CMD:
    {
      hdramac_process_activate(cmd_ptr->hdr.sender);
      break;
    }

    case HDRAMAC_DEACTIVATE_CMD:
    {
      hdramac_process_deactivate(cmd_ptr->hdr.sender,
                                 cmd_ptr->hdr.skip_tx_disable);
      break;
    }

    case HDRAMAC_START_ATTEMPT_CMD:
    {
      hdramac_process_start_attempt(cmd_ptr->hdr.sender);
      break;
    }

    case HDRAMAC_STOP_CMD:
    {
      hdramac_process_stop(cmd_ptr->hdr.sender);
      break;
    }

    case HDRAMAC_RESET_ATTEMPT_CMD:
    {
      hdramac_process_reset_attempt(cmd_ptr->hdr.sender);
      break;
    }


#ifdef FEATURE_HDR_HYBRID_ACCESS
    case HDRAMAC_TUNE_AWAY_TIME_CMD:
    {
      hdramac_process_tuneaway_time (cmd_ptr->ta_time.no_tune_away,
                                     cmd_ptr->ta_time.ta_time_slot);

      break;
    }

    case HDRAMAC_TUNE_AWAY_STOP_TX_CMD:
    {
      hdramac_process_tuneaway_stop_access (cmd_ptr->stop_tx.cb);

      break;
    }
#endif /* FEATURE_HDR_HYBRID_ACCESS */

    default:
    {
      ERR("Invalid command! = %x", cmd_ptr->hdr.cmd, 0, 0);
    }
  }
}


/* EJECT */
/*===========================================================================

FUNCTION HDRAMAC_PROCESS_SIG

DESCRIPTION
  This function handles the task signals assigned to this protocol.

DEPENDENCIES
  None.

PARAMETERS
  signal - task signal to be processed.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdramac_process_sig
(
  rex_sigs_type signal
)
{
  switch ( signal )
  {
    case HDRTX_AMAC_ISR_SIG:
    {
#ifdef FEATURE_HDR_QCHAT
      if (hdrqchatupk_oftcs_enabled() || hdrqchatupk_tftcs_enabled())
      {
        hdrqchatupk_amac_process_isr_sig();
      }
      else
      {
        hdramac_process_isr_sig();
      }
#else 
      hdramac_process_isr_sig();
#endif /*FEATURE_HDR_QCHAT*/
      break;
    }

    default:
    {
      ERR("Invalid signal! = %x", signal, 0, 0);
    }
  }
}

/* EJECT */
/*===========================================================================

FUNCTION HDRAMAC_PROCESS_ACACK_MSG

DESCRIPTION
  This function processes the ACAck message.  The transmitter is disabled and
  the TransmissionSuccessful indication is returned followed by the TxEnded
  indication after the required delay.  This message is only handled if the
  protocol is Active since this message is received in response to an access
  probe.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdramac_process_acack_msg
(
  dsm_item_type *msg_ptr
)
{
  
  int32 pilot_power, total_power;
    /* Working vars for logging pilot and total power if needed. */

  int packet_num;
    /* Last packet -- needed if logging needs to be completed */

#ifdef FEATURE_HDR_HYBRID_ACCESS
  boolean disable_tuneaway = TRUE;
    /*  Whether tuneaway should be disabled based on this AcAck. */

  qword   slot_time;
  int     cmp;
    /* Used to check how much time there is before the next tuneaway. */
#endif /* FEATURE_HDR_HYBRID_ACCESS */

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  packet_num = MAX(0, hdramac.frame_num - hdramac.preamble_isr_cnt - 1);
    /* Last packet number. Used if still logging needs to be completed */

  /* Make sure we're still active */
  if (( hdramac_is_active(HDRHAI_AC_MAC_PROTOCOL) == TRUE ) &&
      (hdramac.attempt_in_progress == TRUE))
  {
    /* Disable PA - now */
    hdrmod_setup_tx_timed_disable( TRUE, 0 );

    /* If we are still in DATA substate, log was not sent for the probe. 
       Send it now. */
    if (hdramac.substate == HDRAMAC_ACC_TX_DATA_SUBSTATE)
    {

      /* Remove firmware logging callback - AcAck received. */
      hdrlog_register_slot_logging_cb ( HDRLOG_AMAC_MDSP_SLOT_LOGGING_CLIENT,
                                        NULL );

      /* Log pilot power and total power for the current frame
         before finishing up. */
      pilot_power = rfm_hdr_get_tx_pilot_power( hdrsrchrf_get_rf_dev(HDRSRCHRF_TX_CHAIN), 
                                                RFM_HDR_CARRIER_ID__0, 
                                                RFM_HDR_AGC_FORMAT__DBM256 );

      pilot_power += (pilot_power<0)?-128:128;
      hdramac_probe_log.probe_log.frame_info[packet_num].pilot_power =
                  pilot_power/256;

      total_power = rfm_hdr_get_tx_total_power( hdrsrchrf_get_rf_dev(HDRSRCHRF_TX_CHAIN), 
                                                RFM_HDR_CARRIER_ID__0, 
                                                RFM_HDR_AGC_FORMAT__DBM256 );

      total_power += (total_power<0)?-128:128;
      hdramac_probe_log.probe_log.frame_info[packet_num].total_power =
                  total_power/256;

      /* fill in whether probe was delayed or not */
      hdramac_probe_log.probe_log.probe_delayed = hdramac.probe_delayed;

      /* send access probe log */
      hdramac_log_access_probe();
    }

    /* Force SLP to call the callbacks for messages in this probe with success
     * Must occur before indication so SLP will process them correctly.
     */
    (void) hdrpcp_get_pkt( 0, HDRHAI_AC, hdramac.elapsed_frames,
                    NULL, NULL );

    hdramac.elapsed_frames=0;

    hdrind_give_ind( HDRIND_ACMAC_TRANSMISSION_SUCCESSFUL, NULL );
    hdramac.substate = HDRAMAC_ACC_END_SUBSTATE;

    /* Cause state processing to happen at the next frame boundary, rather
       than at the next scheduled probe time */
    hdramac.event_scheduled = FALSE;

    HDRTRACE_AMAC_ACCESS_COMPLETE();

#ifdef FEATURE_HDR_HYBRID_ACCESS
    /* We need to disable tuneaways for 1 sec since we have received an Ack.
       However, if there the Ack was recevied too close to a scheduled tuneaway
       we may not be able to disable in time.  In this case, tuneaway will not
       be disabled. Note that this can only occur if the AC Ack arrives much
       later than expected. */  

    /*  Check if tuneaway is scheduled */
    if( (hdramac.tuneaway_received == TRUE ) && 
        (hdramac.reject_tuneaway == FALSE) &&
        (hdramac.no_tune_away    == FALSE))
    {
      /* Get current time in slots */
      (void) hdrts_get_current_time (slot_time, NULL);

      /* Add buffer to current time in slots */
      qw_inc (slot_time, HDRAMAC_TUNEAWAY_BUFFER_MS);

      /* compare with tuneaway time */
      cmp = qw_cmp ( hdramac.tuneaway_time, slot_time );

      /* If tuneaway is scheduled in the buffer, don't disable tuneaway. */
      if (cmp < 0)
      {
        disable_tuneaway = FALSE;

        HDR_MSG_PROT( MSG_LEGACY_HIGH, "AC MAC: 1x Tuneaway too close to disable");
      }
    }

    /* Disable tuneaway during the transaction time if it is still
       appropriate to do so. */
    if (disable_tuneaway)
    {
      hdrlmac_acmac_disable_ta( HDRAMAC_T_ACMP_TRANSACTION_MS
                              + HDRAMAC_TUNEAWAY_BUFFER_MS );
    }
#endif /* FEATURE_HDR_HYBRID_ACCESS */


    /* Log successful attempt */
    hdramac_attempt_log.result = HDRLOG_AMAC_ACACK_RECEIVED;
    hdramac_attempt_log.probe_count[hdramac.seq_cnt-1] =
       (uint8) hdramac.probe_cnt;

    /* Map link_id -> pn for logging */
    {
      sys_link_type link_info;

      (void) hdrsrch_get_link_info(HDRDSM_GET_LINK_ID(msg_ptr), &link_info);

      hdramac_attempt_log.pn = link_info.pilot_pn;
    }

    /* Post log */
    hdramac_log_access_attempt();

    /* Free dsm packet */
    dsm_free_packet(&hdramac.pkt_ptr);
  }
  else
  {
    HDRTRACE_RXMSG_IGNORED_STATE( HDRHAI_AC_MAC_PROTOCOL,
      HDRAMAC_ACACK_MSG_ID, hdramacsm_get_state() );
  }
}


/* EJECT */
/*===========================================================================

FUNCTION HDRAMAC_PROCESS_MSG

DESCRIPTION
  This function processes incoming messages for this protocol.  The message
  ID field is extracted from the message and if successful the corresponding
  message processing function is dispatched.

DEPENDENCIES
  None.

PARAMETERS
  msg_ptr - address of dsm item chain containing message.
  protocol_instance - Instance of the protocol the message is for

RETURN VALUE
  None.

SIDE EFFECTS
  The message buffer memory is freed after the message has been processed.

===========================================================================*/
void hdramac_process_msg
(
  dsm_item_type *msg_ptr,
  hdrhai_protocol_instance_enum_type protocol_instance
)
{
  /* Message ID field buffer. */
  hdramac_msg_id_type msg_id;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  ASSERT((protocol_instance == HDRHAI_IN_USE) ||
         (protocol_instance == HDRHAI_IN_CONFIG));

  /*-------------------------------------------------------------------------
   Extract the Message ID field.
  -------------------------------------------------------------------------*/
  if ( HDRAMAC_GET_MSG_ID( msg_ptr, msg_id ) != E_SUCCESS )
  {
    ERR_FATAL("Message ID extraction failed.", 0, 0, 0);
  }

  HDRTRACE_MSG( HDRHAI_AC_MAC_PROTOCOL, msg_id,
    hdramacsm_get_state() );
  /*-------------------------------------------------------------------------
   Perform the corresponding message processing.
  -------------------------------------------------------------------------*/
  if (protocol_instance == HDRHAI_IN_USE)
  {
    switch( msg_id )
    {
      case HDRAMAC_ACACK_MSG_ID:
      {
        HDR_MSG_PROT( MSG_LEGACY_HIGH, "ACACK message received" );
          hdramac_process_acack_msg( msg_ptr );
        break;
      }

      default:
      {
        ERR("Invalid in use message ID %d", msg_id, 0, 0);
      }
    }
  }
  else /* (protocol_instance == HDRHAI_IN_CONFIG) */
  {
    /* Config messages are handled by SCM */
    ERR("Unexpected in config message ID %d", msg_id, 0, 0);
  }
  /*-------------------------------------------------------------------------
   Free the message buffer memory.
  -------------------------------------------------------------------------*/
  dsm_free_packet( &msg_ptr );
}

/* EJECT */
/*===========================================================================

FUNCTION HDRAMAC_IND_CB

DESCRIPTION
  This callback function queues incoming indications for this protocol.

DEPENDENCIES
  None.

PARAMETERS
  ind_name - Indication to process, is a global enum for all
                    possible indications in HDR signaling
  ind_data - Optional data pointer for indication specific data

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdramac_ind_cb
(
  hdrind_ind_name_enum_type ind_name,
  void * ind_data
)
{
  HDR_MSG_PROT_1( MSG_LEGACY_LOW, "hdramac_ind_cb %d", ind_name );

  if (rex_self () == HDRAMAC_TASK_PTR)
  {
    /* Indication is from another protocol in the same task, so consider it
     * an internal indication
     */
    hdramac_process_ind (ind_name, (hdrind_ind_data_union_type *)ind_data);
  }
  else
  {
    hdrtx_queue_ind (HDRHAI_AC_MAC_PROTOCOL, ind_name, ind_data);
  }

} /* hdramac_ind_cb */

/* EJECT */
/*===========================================================================

FUNCTION HDRAMAC_MSG_CB

DESCRIPTION
  This callback function queues incoming messages for this protocol.  The
  AccessParameters message is forwarded to the Overhead Messages Protocol to
  be managed along with the other overhead messages.

DEPENDENCIES
  None.

PARAMETERS
  msg_ptr - address of dsm item chain containing message.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdramac_msg_cb
(
  dsm_item_type *msg_ptr
)
{
  /* Message ID field buffer. */
  hdramac_msg_id_type msg_id;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_PROT( MSG_LEGACY_LOW, "hdramac_msg_cb" );

  if ( HDRAMAC_GET_MSG_ID( msg_ptr, msg_id ) != E_SUCCESS )
  {
    ERR("Message ID extraction failed.", 0, 0, 0);
    dsm_free_packet( &msg_ptr );
  }
  else
  {
    switch ( msg_id )
    {
      case HDRAMAC_ACC_PARAM_MSG_ID:
      {
        hdrovhd_msg_cb( msg_ptr );
        break;
      }

      default:
      {
        hdrtx_queue_msg( HDRHAI_AC_MAC_PROTOCOL, HDRHAI_IN_USE, msg_ptr );
      }
    }
  }
}

/* EJECT */
/*===========================================================================

FUNCTION HDRAMAC_CONFIG_MSG_CB

DESCRIPTION
  This callback function queues incoming messages for the in config instance
  of this protocol.  The AccessParameters message is forwarded to the
  Overhead Messages Protocol to be managed along with the other overhead
  messages.

DEPENDENCIES
  None.

PARAMETERS
  msg_ptr - address of dsm item chain containing message.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdramac_config_msg_cb
(
  dsm_item_type *msg_ptr
)
{
  hdrtx_queue_msg( HDRHAI_AC_MAC_PROTOCOL, HDRHAI_IN_CONFIG, msg_ptr );
}

/* EJECT */
/*===========================================================================

FUNCTION HDRAMAC_GET_LAST_TX_PWR

DESCRIPTION
  This function returns the last power level that was used to send a complete
  access probe.

DEPENDENCIES
  At lease one access probe must have been sent before this function is
  called.

PARAMETERS
  caller - The protocol calling this externalized function (for sw trace)

RETURN VALUE
  Power level in 1/2 dB units.

SIDE EFFECTS
  None.

===========================================================================*/
uint16 hdramac_get_last_tx_pwr ( hdrhai_protocol_name_enum_type caller )
{
  HDRTRACE_AMAC_GET_LAST_TX_PWR( caller, hdramacsm_get_state() );

  return( (uint16) hdramac.last_pwr );
}


/* EJECT */
/*===========================================================================

FUNCTION HDRAMAC_ACTIVATE

DESCRIPTION
  This command transitions the protocol to the Active state.  If the protocol
  is already in the Active state then no action is taken.

DEPENDENCIES
  The protocol must be initialized by calling the hdramac_init function prior
  to activation.

PARAMETERS
  caller - The protocol giving this command (only for software trace)

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdramac_activate ( hdrhai_protocol_name_enum_type caller )
{
  /* Temporary command buffer. */
  hdramac_cmd_union_type cmd_buf;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cmd_buf.hdr.cmd = HDRAMAC_ACTIVATE_CMD;
  cmd_buf.hdr.sender = caller;

  /* Queue the command for execution in the home task. */
  hdrtx_queue_cmd( HDRHAI_AC_MAC_PROTOCOL, &cmd_buf, sizeof( cmd_buf ) );

}

/* EJECT */
/*===========================================================================

FUNCTION HDRAMAC_DEACTIVATE

DESCRIPTION
  This command transitions the protocol to the Inactive state and clears the
  supervision timer.  If the protocol is already in the Inactive state then
  no action is taken.

DEPENDENCIES
  Unless in FTM mode, must be called in hdrmc task context.

PARAMETERS
  caller - The protocol giving this command (only for software trace)
  skip_tx_disable - Specifies if CP wants to skip disabling Tx,
                    If true, then we skip disabling Tx RF and MCPM. 

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void  hdramac_deactivate 
( 
  hdrhai_protocol_name_enum_type caller,
  boolean skip_tx_disable 
)
{
  /* Temporary command buffer. */
  hdramac_cmd_union_type cmd_buf;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cmd_buf.hdr.cmd = HDRAMAC_DEACTIVATE_CMD;
  cmd_buf.hdr.sender = caller;
  cmd_buf.hdr.skip_tx_disable = skip_tx_disable;

  /* Queue the command for execution in the home task. */
  hdrtx_queue_cmd( HDRHAI_AC_MAC_PROTOCOL, &cmd_buf, sizeof( cmd_buf ) );

#ifdef FEATURE_FACTORY_TESTMODE
  if ( rex_self() == hdrmc_tcb_ptr )
#endif
  {
    (void) hdrmc_wait(HDRMC_MAC_DEACTIVATE_COMPLETE_SIG);
    (void) rex_clr_sigs(HDRMC_TASK_PTR, HDRMC_MAC_DEACTIVATE_COMPLETE_SIG);
  }
}

/* EJECT */
/*===========================================================================
FUNCTION HDRAMAC_STOP

DESCRIPTION
  This command causes the ACMAC to finish its current probe as if it were
  the last.  If no response is received ACMAC returns
  HDRIND_ACMAC_TRANSMISSION_STOPPED indication rather  than a failure.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdramac_stop ( hdrhai_protocol_name_enum_type caller )
{
  /* Temporary command buffer. */
  hdramac_cmd_union_type cmd_buf;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cmd_buf.hdr.cmd = HDRAMAC_STOP_CMD;
  cmd_buf.hdr.sender = caller;

  /* Queue the command for execution in the home task. */
  hdrtx_queue_cmd( HDRHAI_AC_MAC_PROTOCOL, &cmd_buf, sizeof( cmd_buf ) );
}

/*===========================================================================
FUNCTION HDRAMAC_RESET_ATTEMPT

DESCRIPTION
  This function queues a command for AMAC to reset the current access attempt.

DEPENDENCIES
  None.

PARAMETERS
  caller - The protocol calling this externalized function (for sw trace)

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdramac_reset_attempt( hdrhai_protocol_name_enum_type caller )
{
  /* Temporary command buffer. */
  hdramac_cmd_union_type cmd_buf;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cmd_buf.hdr.cmd = HDRAMAC_RESET_ATTEMPT_CMD;
  cmd_buf.hdr.sender = caller;

  /* Queue the command for execution in the home task. */
  hdrtx_queue_cmd( HDRHAI_AC_MAC_PROTOCOL, &cmd_buf, sizeof( cmd_buf ) );
}

/* EJECT */
/*===========================================================================

FUNCTION HDRAMAC_START_ATTEMPT

DESCRIPTION
  Initiate an access attempt.

DEPENDENCIES
  The ACMAC must be in the active state.

PARAMETERS
  caller - The protocol calling this externalized function (for sw trace)

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdramac_start_attempt( hdrhai_protocol_name_enum_type caller )
{
  /* Temporary command buffer. */
  hdramac_cmd_union_type cmd_buf;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cmd_buf.hdr.cmd = HDRAMAC_START_ATTEMPT_CMD;
  cmd_buf.hdr.sender = caller;

  hdrtx_queue_cmd( HDRHAI_AC_MAC_PROTOCOL, &cmd_buf, sizeof( cmd_buf ) );
}

/* EJECT */

#ifdef FEATURE_HDR_HYBRID_ACCESS
/*===========================================================================

FUNCTION HDRAMAC_TA_TIME

DESCRIPTION
  Notifty ACMAC tune away time.

DEPENDENCIES
  The ACMAC must be in the active state.

PARAMETERS
  caller - The protocol calling this externalized function (for sw trace)

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdramac_ta_time
(
  hdrhai_protocol_name_enum_type caller,
  boolean  no_tune_away,
  qword    ta_time_slot
)
{
  /* Temporary command buffer. */
  hdramac_cmd_union_type cmd_buf;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cmd_buf.ta_time.hdr.cmd      = HDRAMAC_TUNE_AWAY_TIME_CMD;
  cmd_buf.ta_time.hdr.sender   = caller;
  cmd_buf.ta_time.no_tune_away = no_tune_away;

  qw_equ( cmd_buf.ta_time.ta_time_slot, ta_time_slot );

  hdrtx_queue_cmd( HDRHAI_AC_MAC_PROTOCOL, &cmd_buf, sizeof( cmd_buf ) );
}

/* EJECT */
/*===========================================================================

FUNCTION HDRAMAC_TA_STOP_ACCESS_CMD

DESCRIPTION
  Notify AMAC to stop access attempt due to tuneaway.

DEPENDENCIES
  The ACMAC must be in the active state.

PARAMETERS
  caller - The protocol calling this externalized function (for sw trace)

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdramac_ta_stop_access_cmd 
(
  hdrhai_protocol_name_enum_type caller,
  hdramac_ta_stop_tx_cb_type     cb
)
{
  /* Temporary command buffer. */
  hdramac_cmd_union_type cmd_buf;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cmd_buf.stop_tx.hdr.cmd      = HDRAMAC_TUNE_AWAY_STOP_TX_CMD;
  cmd_buf.stop_tx.hdr.sender   = caller;
  cmd_buf.stop_tx.cb           = cb;

  hdrtx_queue_cmd( HDRHAI_AC_MAC_PROTOCOL, &cmd_buf, sizeof( cmd_buf ) );
}
#endif /* FEATURE_HDR_HYBRID_ACCESS */

/* EJECT */
/*===========================================================================

FUNCTION HDRAMAC_PROCESS_IND

DESCRIPTION
  This function processes indications given to AMAC.

DEPENDENCIES
  None.

PARAMETERS
  ind_name - Name of the indication for AMAC to process
  ind_data_ptr - pointer to data associated with the indication

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdramac_process_ind
(
  hdrind_ind_name_enum_type ind_name,
  hdrind_ind_data_union_type * ind_data_ptr
)
{

  hdrscmamac_verno_type verno;
    /* uint8 Array to hold the version numbers of AMAC config attribs */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  
  HDRTRACE_IND(HDRHAI_AC_MAC_PROTOCOL, ind_name, hdramacsm_get_state() );

  switch (ind_name)
  {
    /* AMAC's Configuration has changed */
    case HDRIND_SCM_SESSION_CHANGED:

      /* Get the newly negotiated subtype */
      hdramac.current_subtype = (hdramac_subtypes_type)
        hdrscp_get_current_subtype(HDRHAI_AC_MAC_PROTOCOL);

      if (hdramac.current_subtype == HDRSCP_ENH_ACMAC)
      {
        /* Get the current value and the version number for each attribute */
        hdrscmdb_get_inuse_config_and_verno( 
          HDRHAI_AC_MAC_PROTOCOL,
          &hdramac.scm_cfg,
          verno,
          HDRSCMAMAC_NUM_CONFIG_ATTRIBS );

        /* Record the version numbers that we need */
        hdramac_record_version_nums(verno);

      }
      else
      {
        /* Get the current value and the version number for each attribute */
        hdrscmdb_get_inuse_config( 
          HDRHAI_AC_MAC_PROTOCOL,
          &hdramac.scm_cfg);
      }

      /* Translate the config attributes from SCM definition to AMAC's defn */
      hdramac_translate_config_params();

      break;

    /* AN has GAUPed an AMAC configuration attribute */
    case HDRIND_SCM_AMAC_GAUP:
      hdramac_process_attrib_update();
      break;

    case HDRIND_LMAC_ACCESS_MODE_ENTERED:

      /* - - - Init tuneaway status - - - */
      if (hdrlmac_is_in_access_state())
      {
        if (!hdramac.attempt_in_progress)
        {
          /* TUNE_AWAY_TIME_CMD has not been received yet. */
          hdramac.tuneaway_received = FALSE;
  
          /* process all tune away commands */
          hdramac.reject_tuneaway = FALSE;
  
          /* Init tuneaway flag to false. */
          hdramac.tuneaway_stopped = FALSE;
  
          /* Send indication to LMAC that AMAC is ready */
          hdrind_give_ind( HDRIND_ACMAC_READY, NULL );
  
          HDR_MSG_PROT( MSG_LEGACY_HIGH, "Sent indication HDRIND_ACMAC_READY");
        }
      }
      else
      {
        HDR_MSG_PROT( MSG_LEGACY_HIGH, "ACCESS mode entered with LMAC != ACCESS");
      }

      break;

    default:
      ERR_FATAL( "Unknown indication", 0, 0, 0 );
      break;

  } /* switch */

} /* hdramac_process_ind */

/* EJECT */
/*===========================================================================

FUNCTION HDRAMAC_RESET_ACCESS_STATS

DESCRIPTION
  This function resets access attempt counts

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdramac_reset_access_stats( void )
{
  hdramac_attempt_log.success_count = 0;
  hdramac_attempt_log.failure_count = 0;
  hdramac_attempt_log.attempts_count = 0;

  /* Send the variable length log */
  {
    LOG_HDR_ACCESS_ATTEMPT_C_type *log_ptr;
    uint16                         sub_val;

    /* Figure out how much to shrink the log packet by... */
    sub_val = (HDRLOG_AC_MAX_PROBE_CNT
              - hdramac_attempt_log.probe_sequence_count);

    /* Get log record */
    log_ptr = (LOG_HDR_ACCESS_ATTEMPT_C_type *)
              log_alloc_ex(LOG_HDR_ACCESS_ATTEMPT_C,
              sizeof(LOG_HDR_ACCESS_ATTEMPT_C_type)-sub_val);

    if (log_ptr != NULL)
    {

      /* Fill in and send the log record. */
#ifdef FEATURE_MEMCPY_REMOVAL
      memscpy((void*)&(log_ptr->acmac_log), 
               sizeof(log_ptr->acmac_log),
              (void*)&hdramac_attempt_log,
               FSIZ(LOG_HDR_ACCESS_ATTEMPT_C_type, acmac_log)
               - sub_val);
#else
      memcpy((void*)&(log_ptr->acmac_log),
             (void*)&hdramac_attempt_log,
             FSIZ(LOG_HDR_ACCESS_ATTEMPT_C_type, acmac_log)
              - sub_val);
#endif /* FEATURE_MEMCPY_REMOVAL */

      log_commit((log_type*)log_ptr);
    }
  }
}

/* EJECT */
/*===========================================================================

FUNCTION HDRAMAC_GET_PERSIST_INDEX

DESCRIPTION
  This function returns the current index in the apersistence array of the 
  AccessParameters message to use for calculating persistence backoff.

DEPENDENCIES
  None.

RETURN VALUE
  apersistence index, based on 1x access overload class

SIDE EFFECTS
  None.

===========================================================================*/
uint8 hdramac_get_persist_index( void )
{
  return hdramac.persist_index;
}

/* EJECT */
#ifdef FEATURE_HDR_HYBRID_ACCESS
/*===========================================================================
FUNCTION HDRAMAC_PROCESS_TUNEAWAY_STOP_ACCESS

DESCRIPTION
  This function stops access attempts as result of tune away

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void hdramac_process_tuneaway_stop_access 
(
  hdramac_ta_stop_tx_cb_type tx_stopped_cb
)
{
  if ( hdramac.attempt_in_progress == TRUE )
  {
    /* Disable ISR if in persistence state to keep persistence tests
       from running during tuneaway. */
    if ( hdramac.substate == HDRAMAC_ACC_PERSIST_SUBSTATE )
    {
      hdrmod_disable_frame_isr();
      (void) rex_clr_sigs( HDRTX_TASK_PTR, HDRTX_AMAC_ISR_SIG );
    }
    
    if ( hdramac.tx_enabled == TRUE )
    {
      /* Turn PA off. */
      hdrmod_setup_tx_timed_disable( TRUE, 0 );

      /* Modulator cleanup. Disable PA and tell modulator to exit access mode */
      hdrmod_exit_access_mode();

      /* Disable carrier 0 in firmware and exit access mode. */
      hdramac_disable_carrier_access_mode();

#ifdef FEATURE_HDR_PN_DELAY_SUPPORT
      #error code not present
#endif /* FEATURE_HDR_PN_DELAY_SUPPORT */

      /* Trigger Tx DAC stop */
      hdrmod_tx_stop();
    }
  }

  /* If TX is in RF_ON_TX_OFF state, make sure it is shutdown before access */
  /* Disable tx */
  (void) hdrtx_disable_tx( FALSE );

  /* Update tx status. */
  hdramac.tx_enabled = FALSE;

  /* Note that we've been stopped for tuneaway. */
  hdramac.tuneaway_stopped = TRUE;

  HDR_MSG_PROT (MSG_LEGACY_HIGH, "HDRAMAC tuning away ... (or deactivated)");

  if( tx_stopped_cb != NULL )
  {
    /* Call tx stopped callback */
    tx_stopped_cb();
  }

}

/* EJECT */
/*===========================================================================
FUNCTION HDRAMAC_TUNEAWAY_RESUME_ACCESS

DESCRIPTION
  This function resumes access attempts after tune away.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void hdramac_tuneaway_resume_access (void)
{

  /* Only enable Tx and modulator if an attempt was in progress */
  if (hdramac.attempt_in_progress )
  {
    /* Enable TX */
    (void) hdrtx_enable_tx( NULL );
  
#ifdef FEATURE_HDR_BOLT_MODEM
    /* Send PN and carrier enable information to FW. PN information can be
       updated at a later point when probes are sent. The purpose of this
       call is to convey to FW the carrier to enable in microkernel. This 
       information should be available with FW before DAC start. */
    hdramac_set_iq_mask();
#endif /* FEATURE_HDR_BOLT_MODEM */

    /* Prepare modulator to start tx */
    hdrmod_tx_start();
  
    HDR_MSG_PROT (MSG_LEGACY_HIGH, 
                  "HDRAMAC tuning back, resuming access attempt ...");
  
    /* program the modulator to enter the access mode.  */
    hdrmod_enter_access_mode();
  
   /* Enable access mode and set gains in firmware */
    hdramac_enable_carrier_access_mode();
  
    /* Update tx status. */
    hdramac.tx_enabled = TRUE;
  
    /* Re-install the access channel ISR. */
    hdrmod_enable_frame_isr( hdramac_isr );
  
  }

  /* Note that we've returned from tuneaway. */
  hdramac.tuneaway_stopped = FALSE;

}
#endif /* FEATURE_HDR_HYBRID_ACCESS */

/*===========================================================================
FUNCTION HDRAMAC_OVERRIDE_CONFIG

DESCRIPTION
  This function overrides amac configuration with the passed values

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None

SIDE EFFECTS
  Values stored in hdramac_ap_msg and hdramac are modified
===========================================================================*/
void hdramac_override_config
(
  hdramac_override_config_struct_type * param_ptr
)
{
  hdramac.data_bytes = param_ptr->data_bytes;

  if (hdramac.data_bytes > 0)
  {
    hdramac.override_data_bytes = TRUE;
  }
  else
  {
    hdramac.override_data_bytes = FALSE;
  }
  
  hdramac.test_access_rate = param_ptr->test_access_rate;

  hdramac_ap_msg = param_ptr->ap_msg_override;

  hdramac_set_silence_params( 0, 0 );

}


/*===========================================================================
FUNCTION HDRAMAC_CONFIGURE                                   EXTERNAL

DESCRIPTION
  This function does the AT-init configuration of Enh. AMAC attribute. 

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  TRUE - If the configuration was needed.
  FALSE - Otherwise.

SIDE EFFECTS
  None.
===========================================================================*/
boolean hdramac_configure
(
  hdrhai_protocol_name_enum_type sender
)
{
  boolean ret_val = FALSE;
    /* Return Value */

  uint16 subtype;
    /* AMAC subtype */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  subtype = hdrscp_get_config_subtype(HDRHAI_AC_MAC_PROTOCOL);

#ifdef FEATURE_HDR_PN_DELAY_SUPPORT
  #error code not present
#endif /* FEATURE_HDR_PN_DELAY_SUPPORT */
  {
    HDR_MSG_PROT_1( MSG_LEGACY_HIGH, 
                  "No ACMAC config needed %d", subtype);
  }

  return ret_val;

} /* hdramac_configure */


/*===========================================================================
FUNCTION HDRAMAC_PROCESS_MDSP_LOG_EVENT

DESCRIPTION
  This function will be registered as callback to HDRLOG module and collects 
  MAC logging information from MDSP. It is executed four times per frame.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdramac_process_mdsp_log_event
(  
  hdrlog_mdsp_slot_logging_data_type *sl_params
)
{
  uint8 subfrm_idx = 0;
    /* Subframe that corresponds to the log time */

  uint16 drc_channel_gain, ack_channel_gain, 
         data_channel_gain, pilot_channel_gain;
    /* Data gain values during probe Tx */

  int16 max_tx_pwr_rri, rx_agc, tx_total, tx_pilot, tx_adjust;
    /* AGC values during probe Tx */

  int16 turnaround, nominal_pwr = 0;          /* For converting AGC values */
  int16 turnaround_db256, nominal_pwr_db256;  /* to dB */

  boolean display_log = FALSE;
    /* Whether to print data in F3 */
    /* Data only printed at start of preamble and data frames to avoid
       excessing F3 messages */
  
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  rfm_hdr_get_turnaround_info( hdrsrchrf_get_rf_dev(HDRSRCHRF_TX_CHAIN),
                               &turnaround,
                               &nominal_pwr );

  turnaround_db256 = turnaround * 256;
  nominal_pwr_db256 = nominal_pwr * 256;

  /* Get the subframe index from the HSTR */
  subfrm_idx = (((sl_params->slot_time - hdramac.tx_offset) % 16) / 4);

  drc_channel_gain = HDRLOG_GET_DRC_DIGITAL_GAIN_LOG_MC( 
          sl_params->tx_log_buff_idx, 0, 0 );
  ack_channel_gain = HDRLOG_GET_ACK_DIGITAL_GAIN_LOG_MC( 
          sl_params->tx_log_buff_idx, 0, 0 );
  data_channel_gain = HDRLOG_GET_DATA0_DIGITAL_GAIN_LOG_MC( 
          sl_params->tx_log_buff_idx, 0, 0 );
  pilot_channel_gain = HDRLOG_GET_PILOT_DIGITAL_GAIN_LOG_MC( 
          sl_params->tx_log_buff_idx, 0, 0 );

  /* Fill in AGC values */
  rx_agc = 
      (int2)(((HDRLOG_GET_OPEN_LOOP_POWER_LOG_MC(sl_params->tx_log_buff_idx, 
                   HDRMDSP_DEMOD_CARRIER_0) * 256)
               / HDRDSP_AGC_COUNTS_PER_DB)
               + HDRDSP_AGC_RX_OFFSET_256);

  tx_total = 
      (int2)((-(((HDRLOG_GET_TOTAL_POWER_LOG(sl_params->tx_log_buff_idx) 
                  * 256)
             / HDRDSP_AGC_COUNTS_PER_DB)
             + (HDRDSP_AGC_RX_OFFSET_256 - turnaround_db256))));

  tx_pilot = 
      (int2)((-(((HDRLOG_GET_PILOT_POWER_LOG_MC(sl_params->tx_log_buff_idx, 
                    HDRMDSP_DEMOD_CARRIER_0) * 256)
             / HDRDSP_AGC_COUNTS_PER_DB)
             + (HDRDSP_AGC_RX_OFFSET_256 - turnaround_db256))));

  tx_adjust = 
      (int2)((HDRLOG_GET_CLOSED_LOOP_POWER_LOG_MC(sl_params->tx_log_buff_idx, 
                    HDRMDSP_DEMOD_CARRIER_0) / 2 )
       - (nominal_pwr_db256 - turnaround_db256));

  max_tx_pwr_rri = HDRLOG_GET_MAX_RRI_LOG( sl_params->tx_log_buff_idx);

  /* Display gain data that correlates to the first slots of each frame (subfrm_idx = 0).
     The four-slot preamble in enhanced access adds a complication handled below. */
  if ((hdramac.current_subtype == HDRAMAC_SUBTYPE_ENHANCED) &&
            hdramac_ap_msg.enhanced_access_parameters_included &&
           (hdramac_ap_msg.preamble_length_slots == 0))
  {
    /* For 4-slot Rev A preamble, we need to log at probe start (at which time
       subfrm_idx is 0 and first log is TRUE; and also when each data frame
       starts, which is after the first log (first log is FALSE) and at
       subfrm_idx 1 (offset is due to the 4 slot preamble). */
    if (((subfrm_idx==0) && (hdramac_first_gain_log)) || ((subfrm_idx==1) && (!hdramac_first_gain_log)))
    {
      display_log = TRUE;
    }
  }
  else
  {
    /* For Rev 0 preamble or 16-slot Rev A preamble, we can simply log at the
       beginning of each frame. */
    if (subfrm_idx==0)
    {
      display_log = TRUE;
    }
  }

  if (display_log)
  {

    /* Display info */
    HDR_MSG_PROT_4( MSG_LEGACY_HIGH, 
                    "HDR Access--Gains: Pilot: %x, Data: %x, DRC: %x, Ack: %x", 
                    pilot_channel_gain, data_channel_gain, drc_channel_gain, 
                    ack_channel_gain);

    HDR_MSG_PROT_4( MSG_LEGACY_HIGH, 
                    "HDR Access--RxAGC: %d, Tx Pilot: %d, Tx Total: %d, Tx Adj: %d", 
                    (rx_agc >> 8), (tx_pilot >> 8), (tx_total >> 8), (tx_adjust >> 8));

    HDR_MSG_PROT_3( MSG_LEGACY_HIGH,
                    "HDR Access--Max RRI: %d, Tx slot: %d, Slot: %d",
                    max_tx_pwr_rri, subfrm_idx * 4, sl_params->slot_time);

      hdramac_first_gain_log = FALSE;
  }

} /* hdramac_process_mdsp_log_event */

