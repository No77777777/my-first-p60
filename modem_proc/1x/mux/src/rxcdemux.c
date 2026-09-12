/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

             C D M A    D E -M U L T I P L E X I N G    M O D U L E

GENERAL DESCRIPTION
  Contains mux constant tables and de-multiplexing functions

EXTERNALIZED FUNCTIONS
  rxc_pri - processes primary traffic received on the Forward Traffic Channel

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 1990 - 2016 Qualcomm Technologies, Inc.
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

$PVCSPath: O:/src/asw/COMMON/vcs/rxcdemux.c_v   1.83   15 Oct 2002 18:30:54   donb  $
$Header: //components/rel/1x.mpss/8.0/mux/src/rxcdemux.c#1 $ $DateTime: 2019/06/11 23:51:40 $ $Author: pwbldsvc $

when       who     what, where, why
----------------------------------------------------------------------------
12/13/16   nsh     Fixed KW errors.
04/14/16   srk     Optimized TX Trigger fixes and code cleanup.
02/05/16   eye     1x2GTA Feature changes
12/05/14   srk     Remove the obsolete DS item - tracer.
12/02/14   agh     Support for new customer specific voice service option
10/04/14   pap     Resolving KW errors reported.
03/05/14   srk     Message macro reduction
01/28/14   pap     Aggregating the variables into global structures.
10/08/13   pap     Removing warnings for LLVM.
07/23/13   srk     Use TCB_PTR MACRO instead of rex_self() or tcb_ptr to make
                   the code compatible with RCINIT.
05/31/13   srk     Mainlined FEATURE_MODEM_1X_NON_SIG.
05/21/13   srk     Add guaranteed frame information while reporting frame event
01/15/13   srk     Added checks to prevent overflow while copying RLP payload
01/15/13   srk     Add checks to prevent invalid reads from data pointer.
12/19/12   bph     Replace memcpy w/ struct assignment or memscpy
10/30/12   srk     KW warning fix.
10/22/12   jtm     Suspend power measurement and fade checking while a TT HHO
                   is in progress.
05/31/12   srk     Added check to prevent SCH from having blank RC.
04/04/12   trc     Convert to m1x_time module
03/28/12   srk     Mainlined FEATURE_DS
02/24/12   srk     Mainlined FEATURE_IS2000_SCH and FEATURE_IS2000_P2
02/23/12   jtm     Protect against invalid FCH rates.
01/23/12   jtm     Added check for RLP_BLANK for low speed data.
01/23/12   srk     Feature Cleanup
01/17/12   jtm     Revert REL_A clean up.
01/17/12   ppr     Feature Cleanup.
01/09/12   jtm     Fixed support for low data rate SOs.
01/04/12   srk     Feature Cleanup.
11/22/11   srk     Feature clean up: Mainlined and Internalized Features
09/30/11   srk     Fixed compiler warnings.
09/29/11   srk     Mainlined Always On features and removed Always Off Features
09/29/11   srk     Fixed compiler warnings.
08/15/11   jtm     Klocwork Fixes.
07/07/11   trc     Changed MUX interface to loopback_rx
06/17/11   jtm     Fixed syntax errors.
04/22/11   jtm     CMI Phase 4 - Final SU API changes.
04/04/11   jtm     Remove null rxc_elem_ptr check from rxc_pri_data and rxc_sec_data.
03/28/11   jtm     Fix for MSO RDA Test capability.
03/01/11   jtm     Replace FEATURE_MODEM_1X_SUPPORTS_RLP_SU_API with
                   FEATURE_RLP_API_VERSION_1. Defined in dsrlp_api.h.
02/18/11   jtm     Removed unneeded DS header includes.
02/15/11   jtm     Free the fundicated DSM item during RLP processing if
                   rxc_elem_ptr is Null.
02/01/11   jtm     Replaced direct use of RLP variables with accessor functions.
01/28/11   jtm     Additional FTM decoupling.
01/25/11   jtm     Added Mux Vocoder interface module.
01/21/11   jtm     Klocwork fixes.
12/17/10   jtm     Mainlined FEATURE_MVS_MIGRATE under FEATURE_MODEM_1X_VOICE_SUPPORT.
12/15/10   jtm     Feature cleanup.
12/14/10   jtm     Removed MDR/SCCH support.
12/14/10   jtm     Removed include of voc.h.
12/09/10   jtm     Removed FEATURE_IS2000_REL_D support.
12/02/10   jtm     Added the capability to track the synchronization
                   of MVS and TXC and report errors.
11/15/10   jtm     Initial SU API split for CP.
11/05/10   jtm     Added more _i.h includes for Modem 1x SU API support.
11/01/10   jtm     Private file clean up.
10/30/10   jtm     Modem 1x SU API changes.
10/30/10   jtm     Integrate from rel/1h10.
10/08/10   jtm     Move 1xA ACK counters to rxctraffic.c. Give mar_rx() the
                   correct PCG value for ACK's Tx'd.
10/07/10   jtm     Fix "invalid rate" message when erasures are received.
08/10/10   vks     Cleanup Genesis Revisit feature.
08/06/10   jtm     Replaced fwd_rc_to_rate_set with rxc_get_fwd_link_rate_set().
08/06/10   jtm     Use a common demux routine for fch and dcch for SVD and
                   non-SVD cases. This helps to decouple 1xA from REL_A.
08/05/10   vks     Mainline FEATURE_MODEM_1X_USE_RLP_LOG_TYPES.
07/27/10   jtm     Merge calls to ds_process_rlp_rx_frames() under
                   FEATURE_IS95_RLP_MUX_COMMON_API regardless of whether or
                   not FEATURE_IS95B_MDR is enabled. The number and type
                   of parameters are the same in either case.
07/09/10   jtm     Only send the mux report for DCCH when it is the
                   primary channel.
07/07/10   jtm     Added DCCH demod status logging support.
07/06/10   jtm     Fixed compiler warnings.
06/29/10   jtm     Added support for new DS RLP log types.
06/24/10   jtm     Use CAI_IS_SO_VOICE to check for valid voice service options.
                   Remove featurization around voice SO handling.
06/15/10   jtm     Removed CAI_NULL_RATE processing from switch statement in
                   rxc_demux_fch() since it is invalid for F-FCH. The default
                   case is used instead.
06/15/10   jtm     Provide guaranteed frame indication to fade_check().
                   Renamed valid_frame to is_guaranteed_frame.
                   Provide guaranteed frame indication to fade_check_fch().
                   SO74 and SO75 support added. Bit error checking support added.
                   Count only guaranteed frames for power measurement reporting.
06/10/10   jtm     Eliminate dependency on log_dmss.h
05/05/10   jtm     Added support for new RLP API using FEATURE_IS95_RLP_MUX_COMMON_API
04/12/10   jtm     Added logic for 1x Advanced to prevent fade checking
                   in RC11 when the frame is not guaranteed.
                   Update mar_rx() api to pass the valid_frame flag.
04/02/10   jtm     Added support for new RLP APIs.
02/04/10   jtm     Removed FEATURE_PLT.
02/01/10   jtm     Removed feature TMOBILE.
11/05/09   jtm     Corrected 1xA featurization for MSO.
10/27/09   jtm     Mux support for MSO74 and SO73. FTM support updates.
09/24/09   jtm     Klocwork fixes.
08/26/09   adw     Replaced ts_get() and added necessary time_svc.h include.
08/11/09   jtm     Replaced deprecated ts.h api's with new time api's and
                   removed FEATURE_1X_DISABLE_CMI.
08/05/09   jtm     Lint fixes.
08/04/09   jtm     Replaced calls to deprecated BIO funcs with new DAL API.
06/06/09   vlc     Added support for 1x Advanced (RC11.)
05/20/09   jtm     Added include for rx_v.h for CMI clean up.
05/11/09   jtm     Added missing case to rxc_log_calc_phy_chan_rate when
                   service type is DSRLP_SECONDARY_SRVC.
04/21/09   jtm     Replaced assert.h with amssassert.h. Assert.h is being
                   phased out.
04/17/09   jtm     Added includes to fix compiler warnings.
04/13/09   adw     Replaced tdso.h with tdso_v.h include for CMI.
04/01/09   jtm     Featurized necessary includes.
03/27/09   jtm     Eliminated implicit includes.
11/19/08   jtm     Split various header files into internal and external
                   declarations for Common Modem Interface.
09/12/08   vlc     Added support for FEATURE_MVS_MIGRATE.
07/21/08   jtm     Fixed Klockwork Errors. Removed FEATURE_IS2000_REL_C_DV code.
                   Removed obsolete code.
01/09/08   vlc     Added call to function rxc_check_for_sch_update() to rel
                   0 code as well.
12/10/07   vlc     Fixed Lint errors.
06/28/07   trc     Add srch_mux_report_frame reporting for release 0 only build
05/29/07   vlc     Moved to new FL DSDV RLP API for release 0 only build.
04/30/07   vlc     Fixed RLP logging issue when REL A is turned off.
03/06/07   vlc     Fixed compiler errors when REL A is turned off.
02/23/07   vlc     Fixed compiler warnings/errors when SCH, Rel A, and P2
                   features are turned off.
09/19/06   vlc     Fixed lint errors.
09/08/06   bn      Fixed problem in rxc_pri_data_mdr, rxc_pri_mdr_dpt. For 6800
                   the SCCH decoder buffer is 32-bit, not 16-bits
07/24/06   vlc     Added call to mccdma_decrypt_voice_frame() for authenticaion
                   support.  Provide system time for said function as well.
06/26/06   vlc     Added check for rxc_sent_valid_rpt prior to performing
                   channel supervision under FEATURE_IS2000_REL_D.
04/17/06   vlc     Added debug information in case RXC time line is extended over
                   20ms frame.  Added code to recover if this happens.
09/08/05   ejv     Use new srch_mux interface calls.
07/25/05   rkc     Fixed lint errors.
07/25/05   vlc     If all frames are fill frames, return NULL ptr to RLP in
                   addition to zero frame count.
06/16/05   vlc     Changed code to drop MuxPDU3 or MuxPDU5 fill frames on SCH,
                   without giving these fill frames to RLP as blank frames.  As
                   the code was, the RLP blank count was incremented in RLP
                   statistics debug screen, causing confusion.
06/07/05   vlc     In function rxc_tc_data(), make fch_mux and dcch_mux static
                   variables to facilitate debugging.
05/09/05   vlc     Added check for running out of DSM buffers for rel A branch.
                   If the number of DSM buffers obtained is less than number
                   requested, return all DSM buffers and try again next frame.
                   In function rxc_pri(), only give decoded data on FCH to loopback
                   module, drop DCCH data since loopback does not support DCCH.
04/29/05   vlc     Added 4GV support under FEATURE_VOC_4GV.
04/27/05   vlc     Main lined code in current targets, cleaned out obsolete targets,
                   cleaned out obsolete features.
03/07/05   vlc     In functions rxc_pri() and rxc_sec, added check for
                   Data3G service option.
01/26/05   vlc     Changed #include ds.h (obsolete) to ds707.h.
10/25/04   vlc     Removed calls to srch_flag_erasure() as a clean up effort.
                   This function has been converted to a macro that is a nop.
09/28/04   sr      Merged in changes from 6550 2009 build
07/30/04   vlc     Changed curr_ch to rxc_curr_ch.
07/27/04   vlc     Re-instated call to function rxc_check_for_sch_update().
                   Added debug code for MUX/RLP interface.
07/21/04   vlc     In function rxc_pri_log_mar(), delete F3 error message in default
                   case since this case is valid.
07/01/04   vlc     Put PDCH logging code under feature FEATURE_IS2000_REL_C_DV.
06/18/04   ank     Mainlined FEATURE_GPSONE_DBM.
06/16/04   sr      Merged in changes from 6500 5004 build
06/14/04   vlc     Added support for logging of F-PDCH encapsulated MuxPDUs.
06/09/04   vlc     Added check for running out of DSM buffers.  If the number
                   of DSM buffers obtained is less than number requested, return
                   all DSM buffers and try again next frame.
06/08/04   ejv     Pass address to srch_report_frame rather then entire struct.
06/01/04   vlc     Added missing function headers plus minor modifications to
                   variable and function names.  Changed curr_ch to different
                   type.  Added F-PDCH support.
05/26/04   jrp/rdh Changed calls from srch_flag_erasure() to new interface:
                   srch_report_frame().
05/07/04   vlc     Implemented new RLP interface for EV-DV.  The new interface
                   replaces the current interface for 1x channels as well.
                   Also, modified RLP logging extensively to work with new
                   interface.
04/27/04   vlc     Added call to function rxc_check_for_sch_update() to fix
                   rate change issue for bursts with 1 frame gap.
                   Removed unused rxc_fwd_fr_ptr in function rxc_tc_data().
04/20/04   jrp     Fixed lint warnings.
12/02/03   vlc     Fixed RVCT compiler warnings.  Added comments to describe
                   contents of dec_poss lookup table.
10/21/03   vlc     In function rxc_tc_data_dcch(), if the channel quality is
                   poor, reset MUX category to BLANK.  Also removed voice
                   error check in same function as voice is not carried on
                   DCCH.
08/05/03   wd      Fixed DCCH FERs logging for Release A FTM.
07/25/03   junz    Added FDCH RLP3 frame logging
07/18/03   vlc     Release A changes:  Changed FCH and DCCH
                   channel set up to better indicate when decoder data is
                   available for the appropriate channel(s) for the RXC layer.
07/15/03   ka      Added SER logging support in FTM.
06/27/03   bn      Corrected the RLP logging's time stamp logic when in VP2 and P3
06/25/03   vlc     Added code to set pri_len, sec_len, and sig_len to safe
                   values when received frame is known to be bad.
06/24/02   vlc     Added code to check for possible bad MUX category selection
                   for RS 2 due to FL errors on F-FCH/F-DCCH channel (for
                   release 0.)
05/30/03   vlc     Commented out 2 debug messages.  For SVD, the messages do
                   not apply and can potentially flood debug screen.
05/29/03   vlc     For SVD, created fade_check_fch() and fade_check_dcch() to
                   replace fade_check().
05/22/03   vlc     Fixed compiler warning for MSM6000 target.
05/21/03   vlc     In function rxc_sec_data, added check for DTX mux categories
                   on DCCH.  Give RLP DTX frame status rather than BLANK frame
                   status for DCCH DTX frames.
04/29/03   vlc     Fixed TDSO 1X SCH issue, account for no shifting in HW for
                   release A.  Comment out debug statement occuring every frame.
04/25/03   bn      Added logics to support RLP loging in VP2 and P3
04/18/03   bn      Removed JCDMA Features in FER so that it can apply to DIAG
02/18/03   vlc     Changes only apply to Rel A.  Functions rxc_tc_data_dcch()
                   and rxc_tc_data_fch() now will call primary and secondary
                   clients even when there's no data for those clients so the
                   client data can be properly set.
12/12/02   ph      call rxc_tc_parse() with the channel frame received on
12/04/02   bn      Fixed the compilation error when P2 not defined
11/26/02   bn      Fixed the wrong timing in partial RLP logging when both
                   F-DCCH, F-FCH are on at the same time. And remove the
                   medium message "Rx'ed 3g Data" flooding F3
10/15/02   dlb     Added new power control logging to combine power control
                   and frame type logging.
10/14/02   ak, vlc Modified code to work with MultiMode Data client on MSM6100.
09/18/02   vlc     In function rxc_tc_data(), removed debug info relating to
                   signalling message.  Also, only process SCH and initialize
                   RLP buffer if primary or secondary service is data to fix
                   Release A memory leak bug.
08/27/01   vlc     Changed power measurement report process for VP2.
08/08/02   lh      Fixed dependency on RelA TDSO.
07/26/02   bn      Fixed the RLP log packet length for MUXPDU 5
07/19/02   vlc     Fixed buffer initialization in case RLP data is only carried
                   on SCH (no RLP data on DCCH and FCH.)
                   Made a few SVD variables static.
                   Move variable i in rxc_tdso() inside FEATURE_IS2000_F_SCH.
                   Merged the following VP2 SCH code changes from branch:
           vlc     Created function rxc_data_3g_sch() to handle SCH data in VP2
                   mode.  Now rxc_data_3g() is only called to handle FCH/DCCH
                   data and is only called if there are FCH/DCCH data that
                   frame.
           lh      Forward MuxPDU 5 STDSO clean up.
           vlc     Modified rxc_data_3g() to support VP2.  Moved call to
                   dsrlp_process_rx_frames() to rxc_tc_data() so it is only
                   called once per frame.  Create fch_sig_buf and dcch_sig_buf
                   to store separate signalling data on both channels in case
                   signalling is sent on both channels in a frame.
07/10/02   dlb2    Use RDA_FRAME_INFO_2 log format for MSMHW_EXTENDED_ENERGY_MATRIX
                   feature.
06/25/02   HQ      Added FEATURE_CDSMS.
06/06/02   vlc     Added code not to give DCCH data to voice client.
06/04/02   vlc     Changed VP2 featurization.  Changed parameter list of
                   calls to rxc_fdch_log_frame_info() and rxc_rda_log_frame_info()
                   for VP2.  Moved demux functions back into IRAM until further
                   analysis.
06/04/02   vlc     Added support for VP2 (to service concurrent FCH and DCCH
                   channels.)
                   Added processing for CAI_MUX2_FOR_DCCH_3_ID and
                   CAI_MUX2_FOR_DCCH_4_ID in function rxc_pri_data().
                   Fix Release A 1X SCH PDU5 bug by not calling function
                   rxc_data_3g_check_mm_bit() for PDU5.
                   For SVD builds, move 3 demux functions out of IRAM to
                   save space.
05/31/02   lcc     Corrected channel supervision for DCCH such that bad quality
                   frames during DTX are considered bad frames.
05/16/02   bgc     Added FEATURE_DS_IS2000 around data only code.
05/16/02   bgc     Featurized rxc_sch_frame_status inside FEATURE_IS2000_F_SCH.
05/14/02   snn     Added boundry check for the accessing contents of
                   dsrlp_used_len_table
04/18/02   az      Added support for new voice service option SMV (SO 56)
03/15/02   vlc     Fixed SCH 1X bit shift issue with data calls as secondary
                   traffic.
                   In function rxc_pri_data_3g(), use rxc_sch.sch_rs to
                   determine rate set.
03/06/02   az      Featurized 13K voice support.
03/04/02   bgc     Changed features to IS2000_R_SCH where appropriate.
02/27/02   sy      Fixed logging problem with secondary service.
02/27/02   vlc     Merged the following changes from SVD branch.
           snn     Updated code to use proper PDU types.
                   Modified the check for Sr_id to be done only after testing
                   PDU Type.
           sy      Added logging for SVD.
           lh      Recycle dsm items for both good frame and erasure cases.
           lh      Added recycling of DSM items in rxc_pri_tdso().
           bn      Corrected the function name for logging RDA
           vlc     Added support for FEATURE_IS2000_REL_A_SVD.  Renamed
                   functions which started with rxc_pri (removed the _pri)
                   if they are now called for both primary and secondary
                   traffic.
02/26/02   fc      Changed TDSO frame rate mapping based on frame MUX type
                   rather than frame rate.
02/25/02   hjr     Fixed featurization of FEATURE_IS2000_SCH with
                   FEATURE_IS2000_DS_RS2 on.
02/07/02   bgc     Featurized use of rxc_sch.sch_rs for MSM6000.
01/22/02   vlc     In function rxc_pri_data_3g(), use rxc_sch.sch_rs to determine
                   Mux PDU type.
01/21/02   bgc     Merged data changes for disabling MDR.
12/20/01   bn      Corrected the function name for logging RDA
12/07/01   fc      Fixed the blank-&-burst reporting to TDSO.
11/27/01   hrk, bn Wait for the spurious interrupts to get done before starting
                   FORWARD FRAME TYPE logging.
11/20/01   bn      Added RDA logging logic
11/13/01   bn      Added the enhance DCCH DTX detection and channel supervision logics
11/06/01   lcc     Reinstated usage of rxc_sch_frame_status in rxc_pri_tdso
                   to check for SCH which was overwritten in revision 1.31.
10/31/01   sy      Fixed code such that if dsm_new_buffer returns NULL in rxc
                   then try to get that item the next time around.
10/30/01   bn      Added support for Release A FDCH logging
10/18/01   kk      Fixed fr_rate translation in rxc_pri_tdso for Forward DCCH.
09/20/01   usb     Backed out mixed-rate rate set changes for MDR to fix supplemental
                   channel data corruption problem.
09/20/01   ak      For F-SCH == 1X, do not check SR_ID.
09/14/01   rama    SR_ID fix. Differentiate between SR_ID and RLP instance.
                   Check SR_ID of MUX frame.
09/14/01   bn      Fixed P2 featurization
09/12/01   bn      Fixed the incorrect time stamp for the first RX partial RLP log packet
09/06/01   sr      Moved variable rate_set outside FEATURE_IS2000_P2
                   in rxc_pri_data_3g()
08/22/01   bn      Support DCCH in FTM mode
08/15/01   sr      Fixed REL_A featurization
08/14/01   sy      Added support for DCCH on RS2.
08/13/01   snn     Made changes for supporting MuxPDU5 in data.
08/09/01   kk      Added support for MuxPDU type 5.
08/02/01   na      Fixed a bug that would cause the used field of a dsm
                   item to have a unknown value.
                   Also changed DSM_RLP to PS_RLP - was done as part of dsm
                   upgrades.
08/02/01   jrp     Added support for SO54, SO55.
07/30/01   lcc     Used rxc_sch_frame_status in rxc_pri_tdso to check for SCH
                   frame rather than rxc_sup_dec_int_enable which is false for
                   the last SCH frame in a burst.
07/12/01   snn     Added support for logging and Mixed rate sets.
06/18/01   day     Fix incorrect mainlining of feature. Original definition
                   had !(features).  Missed this, so need to delete unneeded
           code.
06/15/01   day     Mainlined FEATURE_FFPC, FEATURE_IS95B
                   Delete FEATURE_SPECIAL_MDR, FEATURE_MDR_FER_F3MSG
06/13/01   sy      Added DSRLP_RATE_INACTIVE enum to support P2 Mode
                   to show which FCH/DCCH is inactive. It behaves
                   exactly like DSRLP_RATE_BLANK. Corrected logging
                   problem
06/07/01   kk      Added mixed rate support to TDSO interface.
06/07/01   bgc     Initialized local sch_mux_pdu_type in rxc_pri_data_3g().
06/04/01   bgc     Added #ifdefs to turn off data services.
06/04/01   vlc     Changed FEATURE_GPSONE to FEATURE_GPS_MODE.
                   Removed extra q_put in rxc_pri().
06/01/01   snn     RS2 support for Data.
05/29/01   kk      fixed mux pdu type setting for 1x.
05/18/01   kk      RS2 TDSO support. Total length is retrieved from
                   the TDSO table.
05/11/01   fc      Added support for DCCH STDSO.
                   Corrected TDSO eraseure reporting problem.
04/20/01   sr      Merged in from MSM5100 archive
04/19/01   tc      Merged with MSM_MUX1X.01.00.01
           lcc     Corrected a problem with logging received RLP frames.
04/18/01   sr      Merged with MSM_MUX1X.01.00.00
           lcc     Added support for FEATURE_IS2000_P2
03/27/01   vlc     Merged MSM_MUX1X.00.00.17.
           lcc     Merged in support for SO 17 (CAI_SO_VOICE_13K_IS733).
03/20/01   ych     Merged JCDMA Features.
03/02/01   tc      Merged MSM_MUX1X.00.00.15.
02/22/01   ak      File re-arrange of rxc_pri, so that only 3G data
                   functionality in IRAM.
02/16/01   bgc     Changed FEATURE_FACTORY_TESTMODE to enable a runtime
                   switch.
02/05/01   ak      Make SR_ID assignments use #define.  Check for log status
                   on RLP FCH frames.  Also now report correct bps for FCH
                   frames (1/2 rate, 1/4 rate, etc).
02/01/01   tc      Updated to rxc_rifhho_cfs_gps_in_progress.
01/30/01   tc      Merged GPS support.
01/20/01   ks      Sending the TDSO_BLANK and TDSO_ERASURE info correctly
                   to the TDSO module.
01/12/01   tc      Merged MSM_MUX1X.00.00.06.
01/09/00   ak      LTU CRC checking now uses app_field instead of kind fld.
11/23/00   tc      Fixed MSM5100 rxc fch loopback and sch loopback query.
11/29/00   ak      New code to take advantage of LTU CRC checking in ISR.
11/13/00   bgc     Changed access of fch_loopback flags from global variable
                   sharing to accessor fuctions in FTM.
11/13/00   hrk     Moved updating of FFPC setpoints from task context to ISR.
11/03/00   sr      Added support for Full TDSO
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
#include "rxc_i.h"
#include "ffpc_i.h"
#include "mux.h"
#include "rxccommon.h"
#include "rxccommon_i.h"
#include "rxctraffic.h"
#include "rxcrda.h"
#include "rxcdemux.h"
#include "mux1xdiag.h"
#include "txctraffic_i.h"
#include "mux_logtypes.h"
#include "muxvoc.h"

//drivers
#include "enc.h"
#include "dec5000.h"
#include "dec5000_v.h"

//diag
#include "cdma2kdiagi.h"
#include "onex_non_sig.h"

//cp
#include "mar_i.h"
#include "parm_i.h"
#include "cai.h"
#include "cai_v.h"
#include "rx.h"
#include "rx_v.h"
#include "loopback.h"
#include "mccdma_v.h"
#include "mccdma_i.h"
#include "tdso_v.h"

//srch
#include "srch_mux.h"

//other
#include "amssassert.h"
#include "qw.h"
#include "rex.h"
#include "bit.h"
#include "queue.h"
#include "err.h"
#include "log.h"
#include "msg.h"
#include "m1x_diag.h"
#include "m1x_time_i.h"
#include "dsdvrlp_rx.h"
#include "dsm.h"
#include "ds707_defs.h"
#include "dsrlp_api.h"
#include "dsrlp_stats.h"
#include "mdrrlp_api.h"
#include "stringl.h" /* for memscpy */

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/

/*
** Translation Table for CAI_rate type to useful enc_rate_type.
*/
const enc_rate_type mux_cai_to_enc [5] = { (enc_rate_type) 0, ENC_8TH_RATE,
              ENC_QTR_RATE, ENC_HALF_RATE, ENC_FULL_RATE };

#ifdef FEATURE_IS2000_REL_A_SVD
  /* Table used to determine full rate TDSO secondary block size (in bits),
  ** between MuxPDU type 1 categories 11 through 14, inclusive.
  */
  const  word mux_rs1_tdso_sec_len [4] =  { TDSO_RS1_SECONDARY_00,
                                            TDSO_RS1_SECONDARY_01,
                                            TDSO_RS1_SECONDARY_02,
                                            TDSO_RS1_SECONDARY_03
                                          };
  /* Table used to determine full rate TDSO secondary block size (in bits),
  ** between MuxPDU type 2 categories 6 through 9, inclusive.
  */
  const  word mux_rs2_tdso_sec_len [4] =  { TDSO_RS2_SECONDARY_00,
                                            TDSO_RS2_SECONDARY_01,
                                            TDSO_RS2_SECONDARY_02,
                                            TDSO_RS2_SECONDARY_03
                                          };
#endif /* FEATURE_IS2000_REL_A_SVD */

/*-----------------------------------------------------------------*/
/*  A Lookup table of positions and lengths in a frame for the 3
 *  information streams for Multiplex option 2 frames.
 *
Er MMl MMv FMl FMv  pripos prilen  sigpos siglen secpos seclen CRC Tail Tot
1   1   0   0          2      266     268    0      268    0    12   8   288
1   1   1   4    0     6      124     130    138    268    0    12   8   288
1   1   1   4    1     6      54      60     208    268    0    12   8   288
1   1   1   4    2     6      20      26     242    268    0    12   8   288
1   1   1   4    3     6      0       6      262    268    0    12   8   288
1   1   1   4    4     6      124     130    0      130    138  12   8   288
1   1   1   4    5     6      54      60     0      60     208  12   8   288
1   1   1   4    6     6      20      26     0      26     242  12   8   288
1   1   1   4    7     6      0       6      0      6      262  12   8   288
1   1   1   4    8     6      20      26     222    248    20   12   8   288

1   1   0   0          2      124     126    0      126    0    10   8   144
1   1   1   3    0     5      54      59     67     126    0    10   8   144
1   1   1   3    1     5      20      25     101    126    0    10   8   144
1   1   1   3    2     5      0       5      121    126    0    10   8   144
1   1   1   3    3     5      54      59     0      59     67   10   8   144
1   1   1   3    4     5      20      25     0      25     101  10   8   144
1   1   1   3    5     5      0       5      0      5      121  10   8   144
1   1   1   3    6     5      20      25     81     106    20   10   8   144

1   1   0   0          2      54      56     0      56     0    8    8   72
1   1   1   2    0     4      20      24     32     56     0    8    8   72
1   1   1   2    1     4      0       4      52     56     0    8    8   72
1   1   1   2    2     4      20      24     0      24     32   8    8   72
1   1   1   2    3     4      0       4      0      4      52   8    8   72

1   1   1    0         2      20      22     0      22     0    6    8   36
1   1   1    0         2      0       2      0      2      20   6    8   36
*
*/


/* Full Rate Mux 2 position lookup table:
 * we assume MM = 1, and the lookup is a 4 bit FM (frame mode) lookup
 */
const word mux2_full_pri_pos [16] = {   6,   6,   6,   6,   6,   6,   6,   6,
                                 6,   0,   0,   0,   0,   0,   0,   0 };

const word mux2_full_pri_len [16] = { 124,  54,  20,   0, 124,  54,  20,   0,
                                20,   0,   0,   0,   0,   0,   0,   0  };

const word mux2_full_sig_pos [16] = { 130,  60,  26,   6, 130,  60,  26,   6,
                                26,   0,   0,   0,   0,   0,   0,   0  };

const word mux2_full_sig_len [16] = { 138, 208, 242, 262,   0,   0,   0,   0,
                               222,   0,   0,   0,   0,   0,   0,   0  };

const word mux2_full_sec_pos [16] = { 268, 268, 268, 268, 130,  60,  26,   6,
                               248,   0,   0,   0,   0,   0,   0,   0  };

const word mux2_full_sec_len [16] = {   0,   0,   0,   0, 138, 208, 242, 262,
                                20,   0,   0,   0,   0,   0,   0,   0  };

/* Half Rate Mux 2 position lookup table:
 * we assume MM = 1, and the lookup is a 3 bit FM (frame mode) lookup
 */
const word mux2_half_pri_pos [8] = {   5,   5,   5,   5,   5,   5,   5,  0 };

const word mux2_half_pri_len [8] = {  54,  20,   0,  54,  20,   0,  20,  0 };

const word mux2_half_sig_pos [8] = {  59,  25,   5,  59,  25,   5,  25,  0 };

const word mux2_half_sig_len [8] = {  67, 101, 121,   0,   0,   0,  81,  0 };

const word mux2_half_sec_pos [8] = { 126, 126, 126,  59,  25,   5, 106,  0 };

const word mux2_half_sec_len [8] = {   0,   0,   0,  67, 101, 121,  20,  0 };

/* Quarter Rate Mux 2 position lookup table:
 * we assume MM = 1, and the lookup is a 2 bit FM (frame mode) lookup
 */
const word mux2_qtr_pri_pos [4] =  {   4,   4,   4,   4 };

const word mux2_qtr_pri_len [4] =  {  20,   0,  20,   0 };

const word mux2_qtr_sig_pos [4] =  {  24,   4,  24,   4 };

const word mux2_qtr_sig_len [4] =  {  32,  52,   0,   0 };

const word mux2_qtr_sec_pos [4] =  {  56,  56,  24,   4 };

const word mux2_qtr_sec_len [4] =  {   0,   0,  32,  52 };


/* Table of sig_len for blank frames at the 4 ENC_RATE values */
/* Frame Mode value to use is reverse index which is 3 - rate */

const word mux2_sig_blank_len [4] = { 261, 120, 51, 0 };

/* position taking const into account SOM bit */
const word mux2_sig_blank_pos [4] = { 6, 5, 4, 0 };

/* FM mode bits as they appear in first byte of frame */
const word mux2_sig_blank_fm  [4] = { 0x0C, 0x10, 0x10, 0 };

/* SOM bit as it appears in first byte of frame */
const word mux2_sig_blank_som [4] = { 0x02, 0x04, 0x08, 0 };

/*
** Table of sig_len for dim frames at the 4 ENC_RATE values
** 1st dimension is frame rate, second is primary data rate
** Length of signalling does not include SOM bit.
*/
const word mux2_sig_dim_len [4] [4] = { {0,  137,  207,  241},
                                        {0,    0,   66,  100},
                                        {0,    0,    0,   31},
                                        {0,    0,    0,    0} };

const word mux2_sig_dim_pos [4] [4] = { {0,  130,   60,   26},
                                        {0,    0,   59,   25},
                                        {0,    0,    0,   24},
                                        {0,    0,    0,    0} };

const word mux2_sig_dim_fm  [4] [4] = { {0, 0x00, 0x04, 0x08},
                                        {0, 0x00, 0x00, 0x08},
                                        {0,    0,    0, 0x00},
                                        {0,    0,    0,    0} };

const word mux2_pri_dim_len [4] [4] = { {0,  124,   54,   20},
                                        {0,    0,   54,   20},
                                        {0,    0,    0,   20},
                                        {0,    0,    0,    0} };

const word mux2_pri_dim_pos [4] [4] = { {0,    6,    6,    6},
                                        {0,    0,    5,    5},
                                        {0,    0,    0,    4},
                                        {0,    0,    0,    0} };

const word MUX2_BLANK [4] = { CAI_MUX2_REV_5_ID,  CAI_MUX2_REV_14_ID,
                       CAI_MUX2_REV_21_ID, 0 };

const word MUX2_DIM [4] [4] = {
             {0, CAI_MUX2_REV_2_ID, CAI_MUX2_REV_3_ID,  CAI_MUX2_REV_4_ID },
             {0, 0,                 CAI_MUX2_REV_12_ID, CAI_MUX2_REV_13_ID},
             {0, 0,                 0,                  CAI_MUX2_REV_20_ID},
             {0, 0,                 0,                  0                 }  };

/* Rate Set 1 definitions */
const word mux1_full_pri_pos [8] = {  4,   4,   4,   0,   4,   4,   4,   4 };

const word mux1_full_pri_len [8] = { 80,  40,  16,  0,  80,  40,  16,   0 };

const word mux1_full_sig_pos [8] = { 84,  44,  20,   4,   0,   0,   0,   0 };

const word mux1_full_sig_len [8] = { 88, 128, 152, 168,   0,   0,   0,   0 };

const word mux1_full_sec_pos [8] = {  0,   0,   0,   0,  84,  44,  20,   4 };

const word mux1_full_sec_len [8] = {  0,   0,   0,   0,  88, 128, 152, 168 };


#define FCH_BUF       0   // Index to buffer for FCH in RLP data buffer
#define DCCH_BUF      1 // Index to buffer for DCCH in RLP data buffer
#define SCH_BUF_START 2 // Index to 1st buffer for SCH in RLP data buffer

dsrlp_fwd_frame_block_type  rxc_fwd_fr_blk;   /* block of rxed RLP frames*/

/*-------------------------------------------------------------------------*/
/* Variables for averaged fer with the moving average calculation          */
#define RXC_MOVING_AVE_FACTOR 5
#define RXC_AVE_SAMPLE_FRAMES 25

struct
{
  byte current;    /* Counter of current frames */
  byte erasure;    /* Counter of erasure frames */
  boolean full;    /* Indicator if the circle buffer is full */
  byte index;      /* Index of array to store most recently received value */
  word esd[ RXC_MOVING_AVE_FACTOR ];  /* Array to store erasures */
} rxc_averaged_fer;


#ifdef FEATURE_IS2000_REL_A_SVD
typedef struct
{
  word                 bits_to_copy;        /* byte copy count   */
  rlp_rate_enum_type   ds_rate;             /* FCH frame rate    */
  dsrlp_rate_enum_type rxc_dsrlp_fund_rate; /* FCH frame rate    */

  /* Ptr to 1 RLP element in linked list.  Each element
  ** in linked list contains PDU information plus pointer to PDU payload.
  */
  dsm_item_type *rxc_elem_ptr;

  /* Ptr to first element in SCH channel linked list.  Each element
  ** in linked list contains PDU information plus pointer to PDU payload.
  */
  dsm_item_type *rxc_sch_1st_elem_ptr;

  /* Count of actual number of PDUs given to RLP.  This count does not
  ** include PDUs that are fill PDUs (SR_ID = "111").  Since these PDUs
  ** do not belong to RLP, do not pass them to RLP layer.
  */
  word rxc_num_rlp_pdu;

} rxc_mux_rlp_data_t;

static rxc_mux_rlp_data_t rxc_mux_rlp_data =
{
  .bits_to_copy         = 0,
  .ds_rate              = 0,
  .rxc_dsrlp_fund_rate  = 0,
  .rxc_elem_ptr         = NULL,
  .rxc_sch_1st_elem_ptr = NULL,
  .rxc_num_rlp_pdu      = 0
};
//DSDVRLP
/* Indicates the channel currently being demuxed.
*/
dsdvrlp_fwd_channel_enum_type rxc_curr_ch = DSDVRLP_RX_FCH;

/* RLP needs to know the MUX PDU type.
*/
dsrlp_mux_pdu_enum_type rxc_mux_pdu;

/* Struct used to debug MUX/RLP interface.
*/
dsdvrlp_fwd_fr_info_type rxc_frm_arr [RLP_MAX_FWD_PDU]; /*lint -e552 used for debugging */
#endif /* FEATURE_IS2000_REL_A_SVD */

/* Save timing information of demuxing operation for debugging purpose.
** Used to determine if RXC task time line is OK.
*/
qword  rxc_start_demux_fr_time;
qword  rxc_end_demux_fr_time;

#ifdef FEATURE_MODEM_1X_MSO_RDA_TEST
static byte mar_tx_data [ 265 / 8 + 1 ];   /* tx reference pkt storage */
static mar_rate_type rxc_exp_markov_rate;
#endif /* FEATURE_MODEM_1X_MSO_RDA_TEST */

typedef struct
{
  word dec_pos;
    /* Position to start reading decoder bits for determined rate */
  word pri_pos;
    /* Position of first bit of primary traffic in decoder frame */
  word pri_len;
    /* Number of bits of primary traffic in decoder frame */

  #ifdef FEATURE_IS2000_REL_A_SVD
  word sec_pos;
    /* Position of first bit of secondary traffic in decoder frame */
  #endif /* FEATURE_IS2000_REL_A_SVD */

  word sec_len;
    /* Number of bits of secondary traffic in decoder frame */
  word sig_pos;
    /* Position of first bit of signaling traffic in decoder frame */
  word sig_len;
    /* Number of bits of signaling traffic in decoder frame */
} rxc_demux_info_type;

byte rxc_demux_fundicated_ch
(
  rxc_dec_type *buf_ptr,
  cai_data_rate_type dec_rate,
  rxc_demux_info_type *demux_info,
  boolean is_channel_dcch
);

byte rxc_mar_bits[ENC_14400_FULL_RATE_SIZE];
byte *rxc_mar_bits_ptr = rxc_mar_bits;

/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================
FUNCTION      RXC_INIT_AVERAGED_FER

DESCRIPTION   This function initializes erasure frames for poor FER alarm.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void rxc_init_averaged_fer( void )
{
  /* Initialize poor FER erasure frame counts */
  rxc_averaged_fer.current = 0;
  rxc_averaged_fer.erasure = 0;
  rxc_averaged_fer.full = FALSE;
  rxc_averaged_fer.index = 0;
} /* rxc_init_averaged_fer */

/*===========================================================================
FUNCTION      RXC_CALC_AVERAGED_FER

DESCRIPTION   This function counts erasure frames for poor FER alarm.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
LOCAL void rxc_calc_averaged_fer
(
  word mux                                   /* CAI MUX1/2 frame statistic */
)
{
  if ((mux == CAI_MUX1_FOR_10_ID) || (mux == CAI_MUX2_FOR_26_ID))
  {
    rxc_averaged_fer.erasure++;
  }

  if (++rxc_averaged_fer.current == RXC_AVE_SAMPLE_FRAMES)
  {
    rxc_averaged_fer.esd[rxc_averaged_fer.index++] = rxc_averaged_fer.erasure;

    rxc_averaged_fer.current = 0;
    rxc_averaged_fer.erasure = 0;

    if (rxc_averaged_fer.index >= RXC_MOVING_AVE_FACTOR)
    {
      rxc_averaged_fer.index = 0;

      if (rxc_averaged_fer.full == FALSE)
      {
        rxc_averaged_fer.full = TRUE;
      }
    }
  }
} /* rxc_calc_averaged_fer */

/*===========================================================================
FUNCTION      RXC_GET_AVERAGED_FER

DESCRIPTION   This function gets erasure frames for poor FER alarm.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void rxc_get_averaged_fer
(
  mux1xdiag_fer_s_type *fer     /* pointer to store FER data */
)
{
  uint16 sum;                   /* Total Erasure Frames for moving average */
  int i;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if( rxc_averaged_fer.full )
  {
    sum = 0;
    for (i = 0; i < RXC_MOVING_AVE_FACTOR; i++)
    {
      sum += rxc_averaged_fer.esd[i];
    }
    fer->meas_frames = (RXC_MOVING_AVE_FACTOR * RXC_AVE_SAMPLE_FRAMES);
    fer->err_frames = sum;
  }
  else
  {
    fer->meas_frames = 0;
    fer->err_frames  = 0;
  }
} /* rxc_get_averaged_fer */

/*===========================================================================
FUNCTION      RXC_PRI_LOOPBACK

DESCRIPTION   Procedure does the processing for loopback SO's.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void rxc_pri_loopback
(
  byte bits[],                  /* Address of first byte of primary data   */
  word bit_pos,                 /* Position of first bit of data in buffer */
  word bit_len,                 /* Number of bits of data in buffer        */
  word mux,                     /* CAI MUX1 frame statistic                */
  rxc_dec_type *buf_ptr         /* Ptr to buffer which holds decoder data  */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -- - - - - */

  /* Unused Parameter */
  (void) buf_ptr;

  /* Pass the relevant data by reference to the loopback input function,
     where it will be copied. */
  loopback_rx( bits, bit_pos, bit_len, mux );

} /* rxc_pri_loopback */

/*===========================================================================
FUNCTION      RXC_PRI_MARKOV

DESCRIPTION   Procedure does the processing for markov SO's.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void rxc_pri_markov
(
  byte bits[],                  /* Address of first byte of primary data   */
  word bit_pos,                 /* Position of first bit of data in buffer */
  word bit_len,                 /* Number of bits of data in buffer        */
  word mux,                     /* CAI MUX1 frame statistic                */
  rxc_dec_type *buf_ptr         /* Ptr to buffer which holds decoder data  */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Unused Paramters */
  (void) mux;
  (void) buf_ptr;

  b_copy( bits, bit_pos, rxc_mar_bits_ptr, 0, bit_len );
} /* rxc_pri_markov */

/*===========================================================================
FUNCTION      RXC_PRI_DATA_LSPD

DESCRIPTION   Procedure does the processing for data SO's. Pre-MDR.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void rxc_pri_data_lspd
(
  dsm_item_type       *item_ptr,/* holds fch data                          */
  rxc_dec_type        *buf_ptr, /* Ptr to buffer which holds decoder data  */
  rlp_rate_enum_type   local_ds_rate  /* FCH fr. rate                      */
)
{
  rlp_fwd_frame_block_type rxc_rlp_fwd_frames = {0};      /* struct for fcc+7scc */

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /*-------------------------------------------------------------------------
   Save FCH frame into struct and pass fundamental and supplemental code
   channel data to RLP.  Write fundamental channel rate
  -------------------------------------------------------------------------*/
  rxc_rlp_fwd_frames.fwd_frames[0].rlp_item_ptr = item_ptr;
  rxc_rlp_fwd_frames.fwd_frames[0].frame_rate   = local_ds_rate;

  /*-------------------------------------------------------------------------
    Write number of supps to the RLP frame block.
    Should be 0 as SCCH is no longer supported.
  -------------------------------------------------------------------------*/
  rxc_rlp_fwd_frames.num_sup = 0;

  if ( local_ds_rate != RLP_BLANK)
  {
    /*-------------------------------------------------------------------------
      buf_ptr points to decoder data buffer which has been copied to
      dsm_item. Hence the buf_ptr can be freed without any problems

      free the decoder buffer queue element & call RLP function to process
      the data frame
    -------------------------------------------------------------------------*/
    ds_process_rlp_rx_frames (&rxc_rlp_fwd_frames, local_ds_rate, TRUE, item_ptr);
  }
  else
  {
    /*---------------------------------------------------------------------
      Data stream not yet active, or IS-95 frame doesn't contain
      RLP data - discard item
    ---------------------------------------------------------------------*/
    (void)dsm_free_buffer(item_ptr);
  }

} /* rxc_pri_data_lspd */

/*===========================================================================
FUNCTION      rxc_log_calc_phy_chan_rate

DESCRIPTION   Calculate physical channel rate for RLP logging.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
word rxc_log_calc_phy_chan_rate
(
  dsdvrlp_fwd_fr_info_type *fr_ptr,     /* Ptr to frame info */
  word                ch_rate
)
{
  word                phy_ch_rate = 0;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if(fr_ptr->srvc_type == DSRLP_PRIMARY_SRVC)
  {
    switch (fr_ptr->fr_rate)
    {
      case DSRLP_RATE_1:
        phy_ch_rate = ch_rate;
        break;

      case DSRLP_RATE_1_2:
        phy_ch_rate = ch_rate >> 1;
        break;

      case DSRLP_RATE_1_4:
        phy_ch_rate = ch_rate >> 2;
        break;

      case DSRLP_RATE_1_8:
        phy_ch_rate = ch_rate >> 3;
        break;

      case DSRLP_RATE_BLANK:
      case DSRLP_RATE_INACTIVE:
      case DSRLP_RATE_DCCH_DTX:
      case DSRLP_RATE_ERASURE:
        phy_ch_rate = 0;
        break;

      default:
        M1X_MSG( MUX, LEGACY_ERROR,
          "Invalid Primary mux category %d",
          fr_ptr->fr_rate);
        phy_ch_rate = 0;
        break;
    }
  }
  else if (fr_ptr->srvc_type == DSRLP_SECONDARY_SRVC)
  {
    switch (fr_ptr->fr_rate)
    {
      /* Full Rate */
      case DSRLP_RATE_1:
      case DSRLP_RATE_7_8:
      case DSRLP_RATE_3_4:
      case DSRLP_RATE_1_2:
        phy_ch_rate = ch_rate;
        break;

      /* Half Rate */
      case DSRLP_RATE_7_16:
      case DSRLP_RATE_3_8:
      case DSRLP_RATE_1_4:
        phy_ch_rate = ch_rate >> 1;
        break;

      /* Quarter Rate */
      case DSRLP_RATE_3_16:
      case DSRLP_RATE_1_8:
        phy_ch_rate = ch_rate >> 2;
        break;

      /* Eighth Rate */
      case DSRLP_RATE_1_16:
        phy_ch_rate = ch_rate >> 3;
        break;

      /* Zero Rate */
      case DSRLP_RATE_BLANK:
      case DSRLP_RATE_ERASURE:
      case DSRLP_RATE_INACTIVE:
      case DSRLP_RATE_DCCH_DTX:
        phy_ch_rate = 0;
        break;

      default:
        M1X_MSG( MUX, LEGACY_ERROR,
          "Invalid Secondary:fund_rate= %d, rlp_rate = %d",
          ch_rate,
          fr_ptr->fr_rate);
        phy_ch_rate = 0;
        break;
    }
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "Invalid service type");
    phy_ch_rate = 0;
  }
  return (phy_ch_rate);

} /* rxc_log_calc_phy_chan_rate */

/*===========================================================================
FUNCTION      RXC_LOG_RLP3_PAYLOAD

DESCRIPTION   Logs RLP 3 frames.

DEPENDENCIES  Assumes log mask indicates to do RLP 3 logging.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void rxc_log_rlp3_payload
(
  dsm_item_type *curr_list_elem_ptr         /* Ptr to curr element in list */
)
{
  dsdvrlp_fwd_fr_info_type *fr_info_ptr = NULL;       /* Ptr to frame info */
  ds_log_rlp_pkt_type   *rlp_frame_log_ptr = NULL;    /* Ptr to frame entry of log */
  ds_log_rlp_frames_type *log_rx_rlp_frm_buf = NULL;  /* Ptr to log buffer */

  word                chan_rate = 0;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Get pointer to the next frame packet in the RLP Rx frames log buffer.
  ** Build the packet using this pointer.
  */
  rlp_frame_log_ptr = dsrlp_log_rx_init_frame ();

  /* fr_info_ptr points to source information.*/
  fr_info_ptr = (dsdvrlp_fwd_fr_info_type *) curr_list_elem_ptr->data_ptr;

  /* Pointer to dsrlp_log_rx_rlp_frm_buf structure */
  #ifdef FEATURE_RLP_API_VERSION_1
  log_rx_rlp_frm_buf = dsrlp_get_log_rx_rlp_frm_buf();
  #else
  log_rx_rlp_frm_buf = &dsrlp_log_rx_rlp_frm_buf;
  #endif /* FEATURE_RLP_API_VERSION_1 */

  if ( ( rlp_frame_log_ptr == NULL ) ||
       ( fr_info_ptr == NULL )       ||
       ( log_rx_rlp_frm_buf == NULL ) )
  {
    /* One of our pointer is NULL. Don't log this time around. */
    M1X_MSG( MUX, LEGACY_ERROR,
      "Null RLP pointer, no log generated.");
    return;
  }

  /* Save SR ID for entire RLP frames log record.
  */
  log_rx_rlp_frm_buf->service_ID = fr_info_ptr->sr_id;

  /* time delta should be 1 for the first PDU in every 20ms frame so the time
  ** stamp in the actual log can be properly incremented.  time delta is set
  ** to 0 in subsequent PDUs in the same frame.  There is 1 exception to
  ** this rule, when the first PDU in the 20ms is also the first entry in the
  ** log record.  In this case, time delta for the first PDU in the 20ms
  ** frame is set to 0, since there is no need to increment the time stamp.
  */
  if (rxc_bufs_qs_data.rxc_rlp_logging_chan_info.first_entry == TRUE)
  {
    if (rlp_frame_log_ptr == &log_rx_rlp_frm_buf->var_len_buf.pkt[0])
    {
      rlp_frame_log_ptr->time_rate.time_delta = 0;
    }
    else
    {
      rlp_frame_log_ptr->time_rate.time_delta = 1;
    }
    rxc_bufs_qs_data.rxc_rlp_logging_chan_info.first_entry = FALSE;
  }
  else
  {
    rlp_frame_log_ptr->time_rate.time_delta = 0;
  }

  rlp_frame_log_ptr->mux_chan.mux_pdu = (byte) fr_info_ptr->mux_pdu_type;

  /*-------------------------------------------------------------------------
     Start channel specific processing.
  -------------------------------------------------------------------------*/
  if (
      (fr_info_ptr->chan == DSDVRLP_RX_FCH) ||
      (fr_info_ptr->chan == DSDVRLP_RX_DCCH) ||
      (fr_info_ptr->chan == DSDVRLP_RX_PDCH)
     )
  {
    /* Calculate channel rate, similar calculation for FCH, DCCH, and PDCH.
    */
    if (fr_info_ptr->mux_pdu_type == DSRLP_MUX_PDU_1)
    {
      chan_rate = 96;
    }
    else if (fr_info_ptr->mux_pdu_type == DSRLP_MUX_PDU_2)
    {
      chan_rate = 144;
    }
    else
    {
      chan_rate = 0;
      M1X_MSG( MUX, LEGACY_ERROR,
        "Invalid PDU type %d",
        fr_info_ptr->mux_pdu_type);
    }

    rlp_frame_log_ptr->time_rate.phy_chan_rate =
      rxc_log_calc_phy_chan_rate (fr_info_ptr, chan_rate);
  }

  /*-------------------------------------------------------------------------
     Channel specific processing.  Process FCH log packet
  -------------------------------------------------------------------------*/
  if (fr_info_ptr->chan == DSDVRLP_RX_FCH)
  {
    rlp_frame_log_ptr->mux_chan.phy_chan = (word) DS_LOG_RLP_FCH;

    /* For BLANK and ERASURE, bits_to_copy == 0.  For all others, it is
    ** non-zero.  Since special logging convention for ERASURE is to
    ** 0xFFFF, only check against RATE_ERASURE
    */
    rlp_frame_log_ptr->frame_len =
      (fr_info_ptr->fr_rate == DSRLP_RATE_ERASURE) ?
        0xFFFF : rxc_bufs_qs_data.rxc_rlp_logging_chan_info.log_fch_rlp_bits;
    rlp_frame_log_ptr->payload_len.head =
      (rlp_frame_log_ptr->frame_len + 7) / 8;
  }
  /*-------------------------------------------------------------------------
     Channel specific processing.  Process DCCH log packet
  -------------------------------------------------------------------------*/

  else if (fr_info_ptr->chan == DSDVRLP_RX_DCCH)
  {
    rlp_frame_log_ptr->mux_chan.phy_chan = (word) DS_LOG_RLP_DCCH;

    /* For BLANK and ERASURE, bits_to_copy == 0.  For all others, it is
    ** non-zero.  Since special logging convention for ERASURE is to
    ** 0xFFFF, only check against RATE_ERASURE
    */
    rlp_frame_log_ptr->frame_len =
      (fr_info_ptr->fr_rate == DSRLP_RATE_ERASURE) ?
        0xFFFF : rxc_bufs_qs_data.rxc_rlp_logging_chan_info.log_dcch_rlp_bits;
    rlp_frame_log_ptr->payload_len.head =
      (rlp_frame_log_ptr->frame_len + 7) / 8;
  }

  /*-------------------------------------------------------------------------
     Channel specific processing.  Process SCH log packet
  -------------------------------------------------------------------------*/
  else if (fr_info_ptr->chan == DSDVRLP_RX_SCH)
  {
    rlp_frame_log_ptr->mux_chan.phy_chan = (word) DS_LOG_RLP_SCH_0;

    /* Copy the channel rate based on the forward RC
    ** sch_rate can have values from 0,1,2,3,4. Use this to obtain the
    ** channel rate.
    */
    #ifdef FEATURE_IS2000_1X_ADV
    if ((rxc_sch.rc==DEC_RC3) || (rxc_sch.rc==DEC_RC4) || (rxc_sch.rc==DEC_RC11))
    #else
    if ((rxc_sch.rc==DEC_RC3) || (rxc_sch.rc==DEC_RC4))
    #endif /* FEATURE_IS2000_1X_ADV */
    {
      rlp_frame_log_ptr->time_rate.phy_chan_rate = (1 << (word) rxc_sch.sch_rate) * 96;
    }
    else
    {
      rlp_frame_log_ptr->time_rate.phy_chan_rate = (1 << (word) rxc_sch.sch_rate) * 144;
    }

    /* This next "if" only makes sense if doing Turbo.  Return now as the
    ** entire SDU was bad.  No LTU's could be recovered.
    */
    if (rxc_bufs_qs_data.rxc_rlp_logging_chan_info.log_turbo_sch_erasure == TRUE)
    {
      rlp_frame_log_ptr->frame_len = 0xFFFF;
      rlp_frame_log_ptr->payload_len.head = 0;
      rlp_frame_log_ptr->payload_len.tail = 0;

      rxc_bufs_qs_data.rxc_rlp_logging_chan_info.log_turbo_sch_erasure = FALSE;
      return;
    }

    /* Here for Conv coding.
    ** First check for Blank and Erasure, which require special handling.
    */
    if (fr_info_ptr->fr_rate == DSRLP_RATE_BLANK)
    {
      rlp_frame_log_ptr->frame_len = 0;
      rlp_frame_log_ptr->payload_len.head = 0;
    }
    else if (fr_info_ptr->fr_rate == DSRLP_RATE_ERASURE_SCH)
    {
      rlp_frame_log_ptr->frame_len = 0xFFFF;
      rlp_frame_log_ptr->payload_len.head = 0;
    }
    else
    {
      /* Not blank or erasure frame, need to calculate frame length.
      ** For Rate Set 1, we have 171 bits of information.
      */
      if (fr_info_ptr->mux_pdu_type == DSRLP_MUX_PDU_1)
      {
        rlp_frame_log_ptr->frame_len = 171;
      }

      /* For Rate Set 2, we have 266 bits of information.
      */
      else if (fr_info_ptr->mux_pdu_type == DSRLP_MUX_PDU_2)
      {
        rlp_frame_log_ptr->frame_len = 266;
      }

      else if (fr_info_ptr->mux_pdu_type == DSRLP_MUX_PDU_3)
      {
        /* It is Mux PDU3.
        ** Check the RC to see if we have Rate Set1 or Rate Set 2.
        */
        #ifdef FEATURE_IS2000_1X_ADV
        if ((rxc_sch.rc==DEC_RC3) || (rxc_sch.rc==DEC_RC4) || (rxc_sch.rc==DEC_RC11))
        #else
        if ((rxc_sch.rc==DEC_RC3) || (rxc_sch.rc==DEC_RC4))
        #endif /* FEATURE_IS2000_1X_ADV */
        {
          /* In RC3/4/11 we have 44 bytes of information for double size PDU and
          ** 22 bytes for single size PDU. The first 6 bits are the header of
          ** the MUXPDU3. So remove those 6 bits and log the rest of the data
          ** hence (44 * 8 ) - 6 = 346 bits for double size PDU and
          ** ((22 * 8 ) - 6) = 170 bits for single size MuxPDU.
          */
          if (rxc_sch.mux_pdu_double == TRUE)
          {
            rlp_frame_log_ptr->frame_len = 346;
          }
          else
          {
            rlp_frame_log_ptr->frame_len = 170;
          }
        }
        else if (rxc_sch.rc==DEC_RC5)
        {
          /* In RC5 we have 68 bytes of information for double size PDU and
          ** 34 bytes for single size PDU. The first 6 bits are the header of
          ** the MUXPDU3. So remove those 6 bits and log the rest of the data
          ** hence (68 * 8 ) - 6 = 538 bits for double size PDU and
          ** ((34 * 8 ) - 6) = 266 bits for single size MuxPDU.
          */
          if (rxc_sch.mux_pdu_double == TRUE)
          {
            rlp_frame_log_ptr->frame_len = 538;
          }
          else
          {
            rlp_frame_log_ptr->frame_len = 266;
          }
        }
        else
        {
          M1X_MSG( MUX, LEGACY_HIGH,
            "Unknown RC for logging ");
        } /* end if RC3 or RC4 */
      }

      #ifdef FEATURE_IS2000_REL_A
      else if (fr_info_ptr->mux_pdu_type == DSRLP_MUX_PDU_5)
      {
        /* the total packet length subtracts 6 bits MUX header */
        rlp_frame_log_ptr->frame_len =
            (word) ((8*dsm_length_packet(fr_info_ptr->actual_fr_item_ptr)) - 6);
      }
      #endif /* FEATURE_IS2000_REL_A */
      else
      {
        M1X_MSG( MUX, LEGACY_ERROR,
          "Wrong MUX PDU for logging:  %d",
          fr_info_ptr->mux_pdu_type);
      } /* end check for MUX PDUs */

      rlp_frame_log_ptr->payload_len.head = DSRLP_SCH_PAYLOAD_LOG_LEN;

    } /* end check for Blanks and Erasures */
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "rxc_log_rlp3_payload() excludes ch %d",
      fr_info_ptr->chan);
  } /* end channel check */

  /*-------------------------------------------------------------------------
     End channel specific processing.
  -------------------------------------------------------------------------*/

  rlp_frame_log_ptr->payload_len.tail = 0;         /* No tail!             */

  /*-------------------------------------------------------------------------
    Update length fields.
  -------------------------------------------------------------------------*/
  log_rx_rlp_frm_buf->hdr.len += rlp_frame_log_ptr->payload_len.head;

  /*-------------------------------------------------------------------------
    Copy frame data as head payload.
  -------------------------------------------------------------------------*/
  if ((fr_info_ptr->actual_fr_item_ptr != NULL) &&
      (rlp_frame_log_ptr->payload_len.head > 0) &&
      (rlp_frame_log_ptr->payload_len.head <= DS_LOG_MAX_RLP_PAYLOAD_LEN)
     )
  {
    memscpy((void*) rlp_frame_log_ptr->payload,
            rlp_frame_log_ptr->payload_len.head,
            fr_info_ptr->actual_fr_item_ptr->data_ptr,
            rlp_frame_log_ptr->payload_len.head
           );
  }
} /* rxc_log_rlp3_payload */

#ifdef FEATURE_IS2000_REL_A_SVD

/*===========================================================================
FUNCTION      RXC_DATA_3G_CHECK_MM_BIT

DESCRIPTION   For Data 3G service option, this function removes the 1X hdr
              bits from the data stream.  This function is only called to
              process 1X SCH data.

DEPENDENCIES  This function depends on changes in  the decoder read SCH
              function and the deinterleaver program SCH function.

RETURN VALUE  Service type.

SIDE EFFECTS  None.
===========================================================================*/
dsrlp_srvc_enum_type rxc_data_3g_check_mm_bit (void)
{
  byte i = 0;             /* Index for for loop                            */
  byte mm_bit = 0;        /* Mixed Mode bit                                */
  word bit_count = 0;     /* num bits to shift to get to start of RLP data */
  word bit_lefto = 0;     /* num bits left over after shift operation      */
  byte bit_mask = 0;      /* bit mask as result of shift operation         */
  byte fh_bit_mask = 0;   /* data frame header bit mask                    */
  word frame_len = 0;     /* length of data prior to shifting operation    */
  dsrlp_srvc_enum_type service_type = DSRLP_PRIMARY_SRVC; /* primary or secondary data     */

  if (rxc_sch.sch_rs == DEC_RATE_14400)
  {
    frame_len   =  MUX2_FULL_LEN_BYTES;
    mm_bit = (rxc_log_sch_data.rxc_dsm_arr[0]->data_ptr[0]) & MUX2_MM_BIT;
    if (!mm_bit)
    {
      /* RS2 case and RLP is carried in the primary data stream. */
      bit_count   =  MUX2_PRI_SHIFT_LEN;
      bit_lefto   =  MUX2_PRI_SHIFT_LO_LEN;
      fh_bit_mask =  MUX2_PRI_SHIFT_MASK;
    }
    else
    {
      /* RS2 case and RLP is carried in the secondary data stream. */
      bit_count   =  MUX2_SEC_SHIFT_LEN;
      bit_lefto   =  MUX2_SEC_SHIFT_LO_LEN;
      fh_bit_mask =  MUX2_SEC_SHIFT_MASK;

      /* Need to decrement used by 1 byte.  This accounts for the header bits
      ** that have been shifted out of the data stream.
      */
      rxc_log_sch_data.rxc_dsm_arr[0]->used--;
      service_type = DSRLP_SECONDARY_SRVC;
    }
  }
  else
  {
    frame_len   =  MUX1_FULL_LEN_BYTES;
    mm_bit = (rxc_log_sch_data.rxc_dsm_arr[0]->data_ptr[0]) & MUX1_MM_BIT;
    if (!mm_bit)
    {
      /* RS1 case and RLP is carried in the primary data stream. */
      bit_count   =  MUX1_PRI_SHIFT_LEN;
      bit_lefto   =  MUX1_PRI_SHIFT_LO_LEN;
      fh_bit_mask =  MUX1_PRI_SHIFT_MASK;
    }
    else
    {
      /* RS1 case and RLP is carried in the secondary data stream. */
      bit_count   =  MUX1_SEC_SHIFT_LEN;
      bit_lefto   =  MUX1_SEC_SHIFT_LO_LEN;
      fh_bit_mask =  MUX1_SEC_SHIFT_MASK;

      /* Need to decrement used by 1 byte.  This accounts for the header bits
      ** that have been shifted out of the data stream.
      */
      rxc_log_sch_data.rxc_dsm_arr[0]->used--;
      service_type = DSRLP_SECONDARY_SRVC;

    }
  } /* end if (rxc_sch.sch_rs == DEC_RATE_14400) */

  /* At the end of the for loop, RLP data starts at location 0 of buffer.
  */
  for (i = 0; i < frame_len; i++)
  {
    rxc_log_sch_data.rxc_dsm_arr[0]->data_ptr[i] <<= bit_count;
    bit_mask =
      ((rxc_log_sch_data.rxc_dsm_arr[0]->data_ptr[i + 1]) & fh_bit_mask) >>
          bit_lefto;
    rxc_log_sch_data.rxc_dsm_arr[0]->data_ptr[i] |= bit_mask;

  } /* end for loop */
  return (service_type);

} /* rxc_data_3g_check_mm_bits */

/*===========================================================================
FUNCTION      rxc_data_build_rlp_pdu

DESCRIPTION   This procedure builds 1 3G MuxPDU from F-FCH, F-DCCH or F-PDCH.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void rxc_data_build_rlp_pdu
(
  dsm_item_type       *item_ptr,  /* holds PDU data                        */
  rxc_dec_type        *buf_ptr,   /* Ptr to buff which holds decoder data  */
  dsrlp_rate_enum_type fund_rate,
  word                 local_bits_to_copy
)
{
  dsdvrlp_fwd_fr_info_type *fr_info_ptr = NULL;       /* Ptr to frame info */

  /* Pointer to dsrlp_log_rx_rlp_frm_buf structure */
  #ifdef FEATURE_RLP_API_VERSION_1
  ds_log_rlp_frames_type *log_rx_rlp_frm_buf = dsrlp_get_log_rx_rlp_frm_buf();
  #else
  ds_log_rlp_frames_type *log_rx_rlp_frm_buf = &dsrlp_log_rx_rlp_frm_buf;
  #endif /* FEATURE_RLP_API_VERSION_1 */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Unused Paramter */
  MODEM_1X_UNUSED(buf_ptr);

  if ((rxc_mux_rlp_data.rxc_elem_ptr == NULL) ||
      (rxc_mux_rlp_data.rxc_elem_ptr->data_ptr == NULL) ||
      (item_ptr == NULL))
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "rxc_data_build_rlp_pdu called in error, Null ptr.");
    dsm_free_packet(&item_ptr);
    return;
  }

  /* Set PDU information fields to appropriate values.
  */
  fr_info_ptr =
    (dsdvrlp_fwd_fr_info_type *) rxc_mux_rlp_data.rxc_elem_ptr->data_ptr;

  fr_info_ptr->actual_fr_item_ptr = item_ptr;

  if (rxc_so.sec == CAI_SO_PPP_PKT_DATA_3G)
  {
    fr_info_ptr->srvc_type = DSRLP_SECONDARY_SRVC;
  }
  else
  {
    fr_info_ptr->srvc_type = DSRLP_PRIMARY_SRVC;
  }

  fr_info_ptr->chan  = rxc_curr_ch;
  fr_info_ptr->mux_pdu_type = rxc_mux_pdu;
  fr_info_ptr->fr_rate = fund_rate;

  /* Use default value since SR ID doesn't apply to FCH/DCCH.
  */
  fr_info_ptr->sr_id   = dsrlp_get_sr_id (DSRLP_FIXED_SR_ID_INDEX);

  /* Set used of the dsm item to the frame size of the received frame.
  ** Check for the boundry conditions. If the input is not in the range
  ** make the used field to zero.
  ** Since this is FCH or DCCH the MuxPDUtype can never be greater than 2.
  */
  if (
      (local_bits_to_copy == 0) ||
      (fr_info_ptr->fr_rate >= DSRLP_RATE_ERASURE) ||
      (fr_info_ptr->mux_pdu_type >= DSRLP_MUX_PDU_3)
     )
  {
    fr_info_ptr->actual_fr_item_ptr->used = 0;
  }
  else
  {
    if(rxc_so.sec == CAI_SO_PPP_PKT_DATA_3G)
    {
      #ifdef FEATURE_RLP_API_VERSION_1
      fr_info_ptr->actual_fr_item_ptr->used =
                      dsrlp_get_used_frame_len(fr_info_ptr->fr_rate,
                                               DSRLP_SECONDARY_SRVC,
                                               fr_info_ptr->mux_pdu_type);
      #else
      fr_info_ptr->actual_fr_item_ptr->used =
                      dsrlp_used_len_table [fr_info_ptr->fr_rate]
                                           [DSRLP_SECONDARY_SRVC]
                                           [fr_info_ptr->mux_pdu_type];
      #endif /* FEATURE_RLP_API_VERSION_1 */
    }
    else
    {
      #ifdef FEATURE_RLP_API_VERSION_1
      fr_info_ptr->actual_fr_item_ptr->used =
                      dsrlp_get_used_frame_len(fr_info_ptr->fr_rate,
                                               DSRLP_PRIMARY_SRVC,
                                               fr_info_ptr->mux_pdu_type);
      #else
      fr_info_ptr->actual_fr_item_ptr->used =
                      dsrlp_used_len_table [fr_info_ptr->fr_rate]
                                           [DSRLP_PRIMARY_SRVC]
                                           [fr_info_ptr->mux_pdu_type];
      #endif /* FEATURE_RLP_API_VERSION_1 */
    }
  }

  /* Save fundicated channel info.
  */
  if (rxc_curr_ch == DSDVRLP_RX_FCH)
  {
    rxc_frm_arr[DSDVRLP_RX_FCH] = *(fr_info_ptr);
  }
  else if (rxc_curr_ch == DSDVRLP_RX_DCCH)
  {
    rxc_frm_arr[DSDVRLP_RX_DCCH] = *(fr_info_ptr);
  }

  /* Perform RLP payload logging. */
  if ( log_rx_rlp_frm_buf != NULL )
  {
    if (log_status(LOG_RLP_RX_FRAMES_C))
    {
      log_rx_rlp_frm_buf->hdr.code = LOG_RLP_RX_FRAMES_C;
    }
    else if (log_status(LOG_RLP_RX_FDCH_FRAMES_C))
    {
      log_rx_rlp_frm_buf->hdr.code = LOG_RLP_RX_FDCH_FRAMES_C;
    }
  }
  rxc_log_rlp3_payload (rxc_mux_rlp_data.rxc_elem_ptr);
} /* rxc_data_build_rlp_pdu */

/*===========================================================================
FUNCTION      rxc_data_build_sch

DESCRIPTION   This function builds the SCH data stream for RLP.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void rxc_data_build_sch (void)
{
  dsrlp_mux_pdu_enum_type     sch_mux_pdu_type = DSRLP_MUX_PDU_1;
                                            /* MuxPDU type for SCH         */
  dsrlp_srvc_enum_type service_type = DSRLP_PRIMARY_SRVC;   /* primary or secondary data   */
  dsm_item_type *curr_list_elem_ptr = NULL; /* Ptr to curr element in list */
  dsm_item_type *temp_elem_ptr = NULL;    /* Temp Ptr in case of fill PDUs */
  dsdvrlp_fwd_fr_info_type *fr_info_ptr = NULL;       /* Ptr to frame info */

  byte i, j;

  /* Pointer to dsrlp_log_rx_rlp_frm_buf structure */
  #ifdef FEATURE_RLP_API_VERSION_1
  ds_log_rlp_frames_type *log_rx_rlp_frm_buf = dsrlp_get_log_rx_rlp_frm_buf();
  #else
  ds_log_rlp_frames_type *log_rx_rlp_frm_buf = &dsrlp_log_rx_rlp_frm_buf;
  #endif /* FEATURE_RLP_API_VERSION_1 */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Get MuxPDU type based on the Rate set.
  ** For Release A implementations, RLP/DS depend on the Mux layer to
  ** provide the MuxPDU information. This is to avoid any ambiguity between
  ** MuxPDU3 and MuxPDU5.
  **
  ** Since the PDU types used by decoder and RLP have different enumerated
  ** values for MuxPDU 1 & 2, this translation is done. Ideally one of them
  ** can be modified to be same as other, but its legacy..
  */
  sch_mux_pdu_type = (dsrlp_mux_pdu_enum_type) rxc_sch.mux_pdu_type;
  if (rxc_sch.mux_pdu_type == DEC_MUX_PDU_TYPE_1_2_4)
  {
    if (rxc_sch.sch_rs == DEC_RATE_9600)
    {
      sch_mux_pdu_type = DSRLP_MUX_PDU_1;
    }
    else if (rxc_sch.sch_rs == DEC_RATE_14400)
    {
      sch_mux_pdu_type = DSRLP_MUX_PDU_2;
    }
    else
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "Unknown Rate set");
    }

    if (rxc_sch.sch_rate == DEC_SCH_RATE_1X)
    {
      /* Call function to determine service type and also shift out frame
      ** header bits from data stream.
      */
      service_type = rxc_data_3g_check_mm_bit ();
    }
  } /* end if (rxc_sch.mux_pdu_type... */

  /* While loop to build SCH PDU data link list.
  */
  i = 0;
  j = SCH_BUF_START;

  /* Start by assuming no fill PDUs, thus all PDUs will be given to RLP.
  */
  rxc_mux_rlp_data.rxc_num_rlp_pdu = rxc_log_sch_data.rxc_dsm_arr_cnt;
  curr_list_elem_ptr = rxc_mux_rlp_data.rxc_sch_1st_elem_ptr;

  while (curr_list_elem_ptr != NULL)
  {
    /* Check if this is a fill PDU.
    ** The fill PDU is confirmed if all the following are TRUE:
    **    a) the SR_ID is 111 (i.e. 0xE0)
    **    b) the LTU is not ERASURE (app_field == TRUE)
    **    c) PDU Type is 3 or 5
    */
    if (
        (((rxc_log_sch_data.rxc_dsm_arr[i]->data_ptr [0]) & 0xe0) == 0xe0) &&
        (rxc_log_sch_data.rxc_dsm_arr[i]->app_field == TRUE)               &&
        ((sch_mux_pdu_type == DSRLP_MUX_PDU_3) ||
         (sch_mux_pdu_type == DSRLP_MUX_PDU_5)
        )
       )
    {

      /* Fill PDUs, adjust number of PDUs given to RLP.
      */
      rxc_mux_rlp_data.rxc_num_rlp_pdu = i;

      /* Increment blank counts then free all remaining PDUs.
      */
      while (i < rxc_log_sch_data.rxc_dsm_arr_cnt)
      {
        (void)parm_xtra_inc(CAI_XTRA_SCH0_FWD_BLANK_PDU_ID, 1);
        i++;
      }
      dsm_free_packet (&curr_list_elem_ptr);
      curr_list_elem_ptr = NULL;

      /* Check if all PDUs are filled PDUs.
      ** If all are filled PDUs, then all buffers requested
      ** from RLP have been freed.  Therefore, set
      ** rxc_sch_1st_elem_ptr to NULL.  Note: number of RLP buffer
      ** is already set to 0 above in this case.
      ** If not all are filled PDUs, make sure the last buffer
      ** in the linked list points to NULL.
      */
      if (rxc_mux_rlp_data.rxc_num_rlp_pdu == 0)
      {
        /* All buffers contain filled PDUs */
        rxc_mux_rlp_data.rxc_sch_1st_elem_ptr = NULL;
      }
      else
      {
        /* Should never be NULL here but the check makes lint happy.
        */
        if (temp_elem_ptr != NULL)
        {
          temp_elem_ptr->pkt_ptr = NULL; /* Last element of linked list */
        }
      }
      break;
    }
    else /* else of if blank PDU */
    {
      fr_info_ptr =
        (dsdvrlp_fwd_fr_info_type *) curr_list_elem_ptr->data_ptr;
      fr_info_ptr->mux_pdu_type = sch_mux_pdu_type;

      /* Account for bad LTU-CRC's.  If a bad PDU, then mark it as a
      ** bad PDU.  Still pass DSM item up to the rlp layer, where it
      ** will be handled. Also ensure that the SR_ID for the service
      ** and the frame are the same.
      ** Note that we check for Sr_id after we check for MuxPDU type.
      ** This is correct because sr_id has meaning only for PDU3 or 5
      ** and so it prevents code from checking sr_id for PDU 1 or 2.
      */
      if (
          (rxc_log_sch_data.rxc_dsm_arr[i]->app_field == FALSE)  ||
          (
           ((fr_info_ptr->mux_pdu_type == DSRLP_MUX_PDU_3) ||
            (fr_info_ptr->mux_pdu_type == DSRLP_MUX_PDU_5)) &&
            (dsrlp_check_sr_id ( DSRLP_FIXED_SR_ID_INDEX,
                                 rxc_log_sch_data.rxc_dsm_arr[i]) == FALSE)
          )
         )
      {
        fr_info_ptr->fr_rate = DSRLP_RATE_ERASURE_SCH;
      }
      else
      {
        fr_info_ptr->fr_rate = DSRLP_RATE_1;
      }

      M1X_MSG( MUX, LEGACY_LOW,
        "Mux PDU Type is: %d",
        fr_info_ptr->mux_pdu_type);

      if ( rxc_sch.sch_rate == DEC_SCH_RATE_1X)
      {
        /* Service type is determine by the MM bit.
        */
        fr_info_ptr->srvc_type  = service_type;
      }
      else
      {
        fr_info_ptr->srvc_type  = DSRLP_GENERIC_SRVC;
      }
      fr_info_ptr->chan  = DSDVRLP_RX_SCH;

      /* Use default value for now.
      */
      fr_info_ptr->sr_id   = dsrlp_get_sr_id (DSRLP_FIXED_SR_ID_INDEX);

      /*  Get a buffer from the data services memory pool
      */
      fr_info_ptr->actual_fr_item_ptr =  rxc_log_sch_data.rxc_dsm_arr[i];
      rxc_log_sch_data.rxc_dsm_arr[i] = NULL;

      /* If the incoming data rate is 1x AND we are using MuxPDU5, we
      ** should trim the data by one byte i.e. 21 bytes for RS1 and 33 bytes
      ** for RS2.
      ** This is done because MuxLayer has 172 bits of data (for RS1) and
      ** RLP needs octet aligned data. So, we should discard the last 4 bits
      ** to make it octet aligned, i.e. make it 168 bits. Similarly for RS2
      ** also.
      */
      if (rxc_sch.sch_rate == DEC_SCH_RATE_1X)
      {

        if (fr_info_ptr->mux_pdu_type == DSRLP_MUX_PDU_5)
        {
          if(rxc_sch.sch_rs == DEC_RATE_9600)
          {
            dsm_trim_packet (&(fr_info_ptr->actual_fr_item_ptr), 21);
          }
          else
          {
            dsm_trim_packet (&(fr_info_ptr->actual_fr_item_ptr), 33);
          }
        }
      } /* sch_rate is 1X */

      #ifdef FEATURE_DATA_MM
      fr_info_ptr->actual_fr_item_ptr->kind = DSM_PS_RLP;
      #else
      fr_info_ptr->actual_fr_item_ptr->kind = PS_RLP;
      #endif /* FEATURE_DATA_MM */

      /* Save SCH info.
      */
      if (j < RLP_MAX_FWD_PDU )
      {
        rxc_frm_arr[j++] = *(fr_info_ptr);
      }

      if ( log_rx_rlp_frm_buf != NULL )
      {
        if (log_status(LOG_RLP_RX_FRAMES_C))
        {
          log_rx_rlp_frm_buf->hdr.code = LOG_RLP_RX_FRAMES_C;
          rxc_log_rlp3_payload ( curr_list_elem_ptr );
        }
      }

      /* Increment i, used to access payload buffers.  Also, move to the
      ** next link list element.
      */
      i++;

      /* Save old curr_list ptr, in case PDUs which come after this PDU are
      ** fill PDUs.  In this case, we have to discard the fill PDUs.
      */
      temp_elem_ptr = curr_list_elem_ptr;
      curr_list_elem_ptr = curr_list_elem_ptr->pkt_ptr;

    } /* end if fill PDU */
  } /* end main while loop */
} /* rxc_data_build_sch */

#else /* else of FEATURE_IS2000_REL_A_SVD */

/*===========================================================================
FUNCTION      RXC_DATA_3G

DESCRIPTION   Procedure does the processing for data SO's, when doing IS2000
              service options.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void rxc_data_3g
(
  dsm_item_type       *item_ptr,  /* holds fch data                        */
  rxc_dec_type        *buf_ptr,   /* Ptr to buff which holds decoder data  */
  dsrlp_rate_enum_type fund_rate, /* FCH fr. rate                          */
  word                 bits_to_copy /* bits in fch frame                   */
)
{
  byte                        i;
  dsrlp_mux_pdu_enum_type     sch_mux_pdu_type = DSRLP_MUX_PDU_1;
                                               /* MuxPDU type for SCH     */
  boolean rlp_buffer_error = FALSE;
  word    rlp_partial_buf_cnt = 0;

  dsrlp_fwd_frame_type       *rxc_fwd_fr_ptr;  /* Ptr to one RLP frame    */
  dec_rate_set_type rs;

  /* Ptr to 1st RLP element in linked list as returned by
  ** dsdvrlp_alloc_fwd_frames().  Each element in linked list contains PDU
  ** information plus pointer to PDU payload.
  */
  dsm_item_type *rxc_elem_ptr = NULL;

  /* Temp ptr to frame info to simplify the building of frame data */
  dsdvrlp_fwd_fr_info_type *fr_info_ptr = NULL;

  /* Pointer to dsrlp_log_rx_rlp_frm_buf structure */
  #ifdef FEATURE_RLP_API_VERSION_1
  ds_log_rlp_frames_type *log_rx_rlp_frm_buf = dsrlp_get_log_rx_rlp_frm_buf();
  #else
  ds_log_rlp_frames_type *log_rx_rlp_frm_buf = &dsrlp_log_rx_rlp_frm_buf;
  #endif /* FEATURE_RLP_API_VERSION_1 */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Unused Paramter */
  MODEM_1X_UNUSED(buf_ptr);

  if (item_ptr == NULL)
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "rxc_data_3g called in error, Null ptr.");
    return;
  }

  /* NOTE: Although the variable below is named rxc_fwd_fr_blk.sr_id, it is
  actually the RLP instance (or index in rscb_array) for this service.
  Currently, only one RLP instance is supported. In the future, when multiple
  RLP instances are supported, change this to use the correct one*/
  rxc_fwd_fr_blk.sr_id = DSRLP_FIXED_SR_ID_INDEX;
  rxc_fwd_fr_blk.num_rlp_frames = SCH_BUF_START;

  /*-------------------------------------------------------------------------
    Write FCH and DCCH channel rate in a 2-time loop
  -------------------------------------------------------------------------*/
  for ( i = FCH_BUF; i <= DCCH_BUF; ++i )
  {
    boolean channel_included;

    // Point to proper buffer in rxc_fwd_fr_blk
    rxc_fwd_fr_ptr = &(rxc_fwd_fr_blk.fwd_arr[i]);

    if ( i == FCH_BUF )
    {
      channel_included = rxc_so.for_fch.included;
      rs = rxc_so.rate_set;
    }
    else
    {
      channel_included = rxc_so.for_dcch.included;
      rs = rxc_get_fwd_link_rate_set(rxc_so.for_dcch.rc);
    }

    if ( channel_included )
    {
      rxc_fwd_fr_ptr->fr_item_ptr  = item_ptr;
      rxc_fwd_fr_ptr->fr_rate      = fund_rate;
      rxc_fwd_fr_ptr->srvc_type = DSRLP_PRIMARY_SRVC;
      /*
      Check the rate set and assign the MUX PDU type accordingly. For RS1 we use
      MUX PDU1 and for RS2 we use MUX PDU2.
      */

      if (rs == DEC_RATE_9600)
      {
        rxc_fwd_fr_ptr->mux_pdu_type = DSRLP_MUX_PDU_1;
      }
      else if (rs == DEC_RATE_14400)
      {
        rxc_fwd_fr_ptr->mux_pdu_type = DSRLP_MUX_PDU_2;
      }
      else
      {
        // May be for variable rates.
      }

      /*-------------------------------------------------------------------------
        Set used of the dsm item to the frame size of the received frame.
        Check for the boundry conditions. If the input is not in the range
        make the used field to zero.
        Since this is FCH or DCCH the MuxPDUtype can never be greater than 2.
      -------------------------------------------------------------------------*/
      if( ( bits_to_copy == 0)                             ||
          (rxc_fwd_fr_ptr->fr_rate >= DSRLP_RATE_ERASURE)  ||
          (rxc_fwd_fr_ptr->mux_pdu_type >= DSRLP_MUX_PDU_3)
        )
      {
        rxc_fwd_fr_ptr->fr_item_ptr->used =0;
      }
      else
      {
        #ifdef FEATURE_RLP_API_VERSION_1
        rxc_fwd_fr_ptr->fr_item_ptr->used =
                    dsrlp_get_used_frame_len(rxc_fwd_fr_ptr->fr_rate,
                                             DSRLP_PRIMARY_SRVC,
                                             rxc_fwd_fr_ptr->mux_pdu_type);
        #else
        rxc_fwd_fr_ptr->fr_item_ptr->used =
                    dsrlp_used_len_table[ rxc_fwd_fr_ptr->fr_rate]
                                        [ DSRLP_PRIMARY_SRVC]
                                        [ rxc_fwd_fr_ptr->mux_pdu_type];
        #endif /* FEATURE_RLP_API_VERSION_1 */
      }

      /* Init to NULL before trying to obtain 1 linked list item from RLP to store payload info.
      */
      rxc_elem_ptr = NULL;

      /* rxc_elem_ptr will point to 1 RLP element to give to RLP once payload
      ** has been processed.  Elements are initialized to default values in
      ** dsdvrlp_alloc_fwd_frames().
      */
      rxc_elem_ptr = dsdvrlp_alloc_fwd_frames (1);

      if ((rxc_elem_ptr == NULL) ||
          (rxc_elem_ptr->data_ptr == NULL))
      {
        /* If RLP does not allocate the linked list, drop the OTA
        ** RLP frame data.
        */
        M1X_MSG( MUX, LEGACY_ERROR,
          "Failed to obtain fdch list from RLP, free DSM item");
        dsm_free_packet(&item_ptr);
      }
      else
      {
        /* Set PDU information fields to appropriate values.
        */
        fr_info_ptr =
          (dsdvrlp_fwd_fr_info_type *) rxc_mux_rlp_data.rxc_elem_ptr->data_ptr;

        fr_info_ptr->actual_fr_item_ptr = rxc_fwd_fr_ptr->fr_item_ptr;
        fr_info_ptr->srvc_type = rxc_fwd_fr_ptr->srvc_type;
        if (i == FCH_BUF)
        {
          fr_info_ptr->chan = DSDVRLP_RX_FCH;
        }
        else
        {
          fr_info_ptr->chan = DSDVRLP_RX_DCCH;
        }
        fr_info_ptr->mux_pdu_type = rxc_fwd_fr_ptr->mux_pdu_type;
        fr_info_ptr->fr_rate = rxc_fwd_fr_ptr->fr_rate;
        /* Use default value since SR ID doesn't apply to FCH/DCCH.
        */
        fr_info_ptr->sr_id   = dsrlp_get_sr_id (DSRLP_FIXED_SR_ID_INDEX);
        fr_info_ptr->actual_fr_item_ptr->used = rxc_fwd_fr_ptr->fr_item_ptr->used;
      }

      /* Perform RLP payload logging. */
      if ( log_rx_rlp_frm_buf != NULL )
      {
        if (log_status(LOG_RLP_RX_FRAMES_C))
        {
          log_rx_rlp_frm_buf->hdr.code = LOG_RLP_RX_FRAMES_C;
        }
        else if (log_status(LOG_RLP_RX_FDCH_FRAMES_C))
        {
          log_rx_rlp_frm_buf->hdr.code = LOG_RLP_RX_FDCH_FRAMES_C;
        }
      }

      if ( i == FCH_BUF )
      {
        rxc_bufs_qs_data.rxc_rlp_logging_chan_info.log_fch_rlp_bits =
            bits_to_copy;
      }
      else
      {
        rxc_bufs_qs_data.rxc_rlp_logging_chan_info.log_dcch_rlp_bits =
            bits_to_copy;
      }

      if(rxc_mux_rlp_data.rxc_elem_ptr != NULL)
      {
        rxc_log_rlp3_payload (rxc_mux_rlp_data.rxc_elem_ptr);
      }

      /* Give 1 PDU to RLP.
      */
      dsdvrlp_enqueue_fwd_frames (rxc_mux_rlp_data.rxc_elem_ptr, 1);
    }
    else
    {
      rxc_fwd_fr_ptr->fr_item_ptr  = NULL;
      rxc_fwd_fr_ptr->fr_rate      = DSRLP_RATE_INACTIVE;
      rxc_fwd_fr_ptr->srvc_type    = DSRLP_PRIMARY_SRVC;
      rxc_fwd_fr_ptr->mux_pdu_type = DSRLP_MUX_PDU_1;
    }
  } // Of for...

  /*-------------------------------------------------------------------------
    Make the supplemental frames
  -------------------------------------------------------------------------*/
  if( (rxc_log_sch_data.rxc_dsm_arr_cnt > 0) &&
      (rxc_log_sch_data.rxc_dsm_arr_cnt <= SCH_MAX_FWD_PDU))
  {
    if( (rxc_log_sch_data.rxc_dsm_arr_cnt == 1) &&
        (rxc_sch.mux_pdu_double == FALSE))
    {
      if (rxc_sch.sch_rs == DEC_RATE_9600)
      {
        sch_mux_pdu_type = DSRLP_MUX_PDU_1;
      }
      else if (rxc_sch.sch_rs == DEC_RATE_14400)
      {
        sch_mux_pdu_type = DSRLP_MUX_PDU_2;
      }
      else
      {
          M1X_MSG( MUX, LEGACY_HIGH,
            "Unknown Rate set" );
      }
    }
    else
    {
      sch_mux_pdu_type = DSRLP_MUX_PDU_3;
    }

    for( i=0; i < rxc_log_sch_data.rxc_dsm_arr_cnt; i++)
    {
      /*---------------------------------------------------------------------
        check if this is blank PDU
        The Blank PDU is confirmed if all the following are TRUE
            a) the SR_ID is 111 (i.e. 0xE0)
            b) the LTU is not ERASURE (app_field == TRUE)
            c) PDU Type is 3 or 5
      ---------------------------------------------------------------------*/
      if (  (((rxc_log_sch_data.rxc_dsm_arr[i]->data_ptr[ 0]) & 0xe0) == 0xe0) &&
            (rxc_log_sch_data.rxc_dsm_arr[i]->app_field == TRUE)               &&
            ((sch_mux_pdu_type == DSRLP_MUX_PDU_3))
         )
      {
        /*-------------------------------------------------------------------
           Blank PDU - free all remaining PDUs
        -------------------------------------------------------------------*/
        while( i < rxc_log_sch_data.rxc_dsm_arr_cnt )
        {
          /*-----------------------------------------------------------------
            Store the rate as BLANK.  The increment is because the 0'th
            element is the FCH frame.
          -----------------------------------------------------------------*/
          rxc_fwd_fr_blk.fwd_arr[i + SCH_BUF_START].fr_rate = DSRLP_RATE_BLANK;
          rxc_fwd_fr_blk.fwd_arr[i + SCH_BUF_START].mux_pdu_type = sch_mux_pdu_type;
          rxc_fwd_fr_blk.fwd_arr[i + SCH_BUF_START].fr_item_ptr = NULL;
          i++;
          (void)parm_xtra_inc(CAI_XTRA_SCH0_FWD_BLANK_PDU_ID, 1);
        }
        break;
      }
      else /* valid MuxPDU */
      {
        rxc_fwd_fr_ptr = &(rxc_fwd_fr_blk.fwd_arr[i+SCH_BUF_START]);
        rxc_fwd_fr_ptr->mux_pdu_type = sch_mux_pdu_type;

        /*-------------------------------------------------------------------
          Account for bad LTU-CRC's.  If a bad PDU, then mark it as a
          bad PDU.  Still pass DSM item up to the rlp layer, where it
          will be handled. Also ensure that the SR_ID for the service
          and the frame are the same.
          Note that we check for Sr_id after we check for MuxPDU type.
          This is correct because sr_id has meaning only for PDU3 or 5
          and so it prevents code from checking sr_id for PDU 1 or 2.
        -------------------------------------------------------------------*/
        if ((rxc_log_sch_data.rxc_dsm_arr[i]->app_field == FALSE)||
            (((rxc_fwd_fr_ptr->mux_pdu_type == DSRLP_MUX_PDU_3)
             )
             &&
             (dsrlp_check_sr_id(DSRLP_FIXED_SR_ID_INDEX,
                                rxc_log_sch_data.rxc_dsm_arr[i]) == FALSE)
            )
           )
        {
          rxc_fwd_fr_ptr->fr_rate = DSRLP_RATE_ERASURE_SCH;
        }
        else
        {
          rxc_fwd_fr_ptr->fr_rate = DSRLP_RATE_1;
        }

        M1X_MSG( MUX, LEGACY_LOW,
          "Mux PDU Type is: %d",
          rxc_fwd_fr_ptr->mux_pdu_type);

        if ( rxc_sch.sch_rate == DEC_SCH_RATE_1X)
        {
          rxc_fwd_fr_ptr->srvc_type  = DSRLP_PRIMARY_SRVC;
        }
        else
        {
          rxc_fwd_fr_ptr->srvc_type  = DSRLP_GENERIC_SRVC;
        }

        /*-------------------------------------------------------------------
          Get a buffer from the data services memory pool
        -------------------------------------------------------------------*/
        rxc_fwd_fr_ptr->fr_item_ptr =  rxc_log_sch_data.rxc_dsm_arr[i];

        /* Init to NULL before trying to obtain 1 linked list item from RLP to store payload info.
        */
        rxc_elem_ptr = NULL;

        /* rxc_elem_ptr will point to 1 RLP element to give to RLP once payload
        ** has been processed.  Elements are initialized to default values in
        ** dsdvrlp_alloc_fwd_frames().
        */
        rxc_elem_ptr = dsdvrlp_alloc_fwd_frames (1);

        if ((rxc_elem_ptr == NULL) ||
            (rxc_elem_ptr->data_ptr == NULL))
        {
          /* If RLP does not allocate the linked list, drop the OTA
          ** RLP frame data.  For SCH, don't need to free the buffer since it can be re-used
          ** in the next frame by remaining in rxc_dsm_arr.
          */
          M1X_MSG( MUX, LEGACY_ERROR,
            "Failed to obtain sch list from RLP");
        }
        else
        {
          /* Set PDU information fields to appropriate values.
          */
          fr_info_ptr =
            (dsdvrlp_fwd_fr_info_type *) rxc_elem_ptr->data_ptr;

          fr_info_ptr->mux_pdu_type = rxc_fwd_fr_ptr->mux_pdu_type;
          fr_info_ptr->fr_rate = rxc_fwd_fr_ptr->fr_rate;
          fr_info_ptr->srvc_type = rxc_fwd_fr_ptr->srvc_type;
          fr_info_ptr->chan  = DSDVRLP_RX_SCH;

          /* Use default value for now.
          */
          fr_info_ptr->sr_id   = dsrlp_get_sr_id (DSRLP_FIXED_SR_ID_INDEX);

          /*  Get a buffer from the data services memory pool
          */
          fr_info_ptr->actual_fr_item_ptr = rxc_fwd_fr_ptr->fr_item_ptr;

          /* Perform RLP payload logging. */
          if ( log_rx_rlp_frm_buf != NULL )
          {
            if (log_status(LOG_RLP_RX_FRAMES_C))
            {
              log_rx_rlp_frm_buf->hdr.code = LOG_RLP_RX_FRAMES_C;
              rxc_log_rlp3_payload ( rxc_elem_ptr );
            }
          }

          /* Give 1 PDU to RLP.
          */
          dsdvrlp_enqueue_fwd_frames (rxc_elem_ptr, 1);

          /* Indicate that the DSM item containing RLP data this frame has been
          ** given to RLP layer.
          */
          rxc_log_sch_data.rxc_dsm_arr[i] = NULL;
        }

        #ifdef FEATURE_DATA_MM
        rxc_fwd_fr_ptr->fr_item_ptr->kind = DSM_PS_RLP;
        #else
        rxc_fwd_fr_ptr->fr_item_ptr->kind = PS_RLP;
        #endif /* FEATURE_DATA_MM */

        /*-------------------------------------------------------------------
          Increment number of frames, even if there was bad LTU CRC's.
          The bad frames are passed up to RLP as erasures, and discarded
          there.
        -------------------------------------------------------------------*/
        rxc_fwd_fr_blk.num_rlp_frames++;
      } /* else valid MUX PDU */
    } /* for */
    rxc_log_sch_data.rxc_dsm_arr_cnt = 0;
  } /* if SCHs received */

  /*---------------------------------------------------------------------
    This loop makes sure that all the buffer's were allocated. In case
    if the new buffer returns NULL this time, it will try to allocate
    again the next time.
   ---------------------------------------------------------------------*/
  for (i=0; i< SCH_MAX_FWD_PDU; i++)
  {
    if (rxc_log_sch_data.rxc_dsm_arr[i] == NULL)
    {
      /*-------------------------------------------------------------------
          Re-load the array with a new dsm item.
      -------------------------------------------------------------------*/
      rxc_log_sch_data.rxc_dsm_arr[i] = dsm_new_buffer(DSM_DS_SMALL_ITEM_POOL);

      /* If less than 8 buffers are available, treat as error case.
      */
      if (rxc_log_sch_data.rxc_dsm_arr[i] == NULL)
      {
        rlp_buffer_error = TRUE;
        break;
      }
    }
  }

  /* Error case, return the rest of the buffers, try again next frame.
  */
  if (rlp_buffer_error == TRUE)
  {
    for( i=0; i< SCH_MAX_FWD_PDU; i++)
    {
      if (rxc_log_sch_data.rxc_dsm_arr[i] != NULL)
      {
        dsm_free_packet( &rxc_log_sch_data.rxc_dsm_arr[i]);
        rxc_log_sch_data.rxc_dsm_arr[i] = NULL;
        rlp_partial_buf_cnt++;
      }
    }
    M1X_MSG( MUX, LEGACY_ERROR,
      "Not enough DSM items %d",
      rlp_partial_buf_cnt);

    /* Set array count to max value + 1 so RXC ISR function will not
    ** read decoder output for this frame (because rxc_dsm_arr_cnt
    ** is not 0) and this function will not try to parse this frame
    ** as well (because rxc_dsm_arr_cnt is not within valid range,
    ** setting it to SCH_MAX_FWD_PDU or any positive count lower than
    ** max causes data abort in this function.)
    **
    ** Array count is reset once good DSM items are obtained.
    */
    rxc_log_sch_data.rxc_dsm_arr_cnt = SCH_MAX_FWD_PDU + 1;
  }

  /* Check for SCH update, now that the frame has been assembled
  ** for RLP, it's a good time to update SCH parameters (meant for
  ** future frames, if there have been changes.)  This call is
  ** specifically designed for the burst with 1 frame gap cases.
  ** See comments inside rxc_check_for_sch_update() for timing
  ** information.
  */
  rxc_check_for_sch_update ();

  dsdvrlp_process_fwd_frames ();
} /* rxc_data_3g */
#endif /* FEATURE_IS2000_REL_A_SVD */

/*===========================================================================
FUNCTION      RXC_PRI_LOG_MAR

DESCRIPTION   Procedure does the logging for MARKOV data statistics.

DEPENDENCIES  Assumes the rxc_vbits already contain the vocoder data.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void rxc_pri_log_mar
(
  byte bits[],                  /* Address of first byte of primary data   */
  word bit_pos,                 /* Position of first bit of data in buffer */
  word bit_len,                 /* Number of bits of data in buffer        */
  word mux,                     /* CAI MUX1 frame statistic                */
  rxc_dec_type *buf_ptr         /* Ptr to buffer which holds decoder data  */
)
{
  /*-------------------------------------------------------------------------
    Default to 1/8th rate voice service option on the following flags and
    change later if necessary
  -------------------------------------------------------------------------*/
  byte                        log_flag = 0xF0;
  byte                        log_mux;
  boolean                     rate_13k = FALSE;

  word                        mux_ber;
  static mar_stat_type        mar;
  cai_data_rate_type          mar_rate;
  word                        so;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Unused Parameters */
  (void) bits;
  (void) bit_pos;
  (void) bit_len;

  if ( rxc_so.rate_set == DEC_RATE_9600 )
  {
    log_mux = (byte) ((mux >= CAI_MUX1_FOR_DCCH_1_ID) ?
                        mux - CAI_MUX1_FOR_DCCH_1_ID : mux - CAI_MUX1_FOR_1_ID);
  }
  else
  {
    log_mux = (byte) ((mux >= CAI_MUX2_FOR_DCCH_1_ID) ?
                        mux - CAI_MUX2_FOR_DCCH_1_ID : mux - CAI_MUX2_FOR_1_ID);
  }

  so = rxc_so.pri;

  #ifdef FEATURE_1X_CUST_VOICE_SO
  /* This workaround has been added to handle NV dependent non-constant
   * SO while ensuring the existing switch construct is not disrupted */
  if (so == mcc_get_cust_so() && so != CAI_SO_NULL)
  {
    so = CAI_GEN_EVRC_CUST_SO;
  }
  #endif /* FEATURE_1X_CUST_VOICE_SO */

  switch (so)
  {
    case CAI_SO_VOICE_13K_IS733:
    case CAI_SO_VOICE_13K:
    case CAI_SO_VOICE_4GV_WB:
      if (log_mux > 15)
      {
        log_mux &= 0x0F;
        log_flag = 0xD0;
      }
      else
      {
        log_flag = 0xC0;
      }
      /*---------------------------------------------------------------------
         Fall-through!
      ---------------------------------------------------------------------*/
    case CAI_SO_VOICE_IS96A:
    case CAI_SO_VOICE_EVRC:
    #ifdef FEATURE_1X_CUST_VOICE_SO
    case CAI_GEN_EVRC_CUST_SO:
    #endif /* FEATURE_1X_CUST_VOICE_SO */
    case CAI_SO_VOICE_EVRC_NW:
    case CAI_SO_VOICE_SMV:
    case CAI_SO_VOICE_4GV_NB:
      /* Log frame rate decision */
      if (log_mux > 15)
      {
        log_mux &= 0x0F;
        log_flag = 0xD0;
      }
      rxc_log_mar ( (byte) (log_flag | log_mux), 0 );
      break;

    case CAI_SO_LOOPBACK_13K:
    case CAI_SO_LOOPBACK_SO55:
    {
      byte for_fch_rc = snm_get_current_config()->for_fch_rc;
      if ( (( rxc_so.pri == CAI_SO_LOOPBACK_SO55)
           && rxc_get_fwd_link_rate_set(for_fch_rc) == ((byte) CAI_RATE_14400))
           || ( rxc_so.pri == CAI_SO_LOOPBACK_13K) )
      {
        if (log_mux > 15)
        {
          log_mux &= 0x0F;
          log_flag = 0xD0;
        }
        else
        {
          log_flag = 0xC0;
        }
      }
    }
      /*---------------------------------------------------------------------
         Fall-through!
      ---------------------------------------------------------------------*/
    case CAI_SO_LOOPBACK:
      /* Log frame rate decision */
      rxc_log_mar ( (byte) (log_flag | log_mux), 0 );
      break;

    case CAI_SO_MARKOV_SO54:
    case CAI_SO_MARKOV_13K:
    case CAI_SO_RS2_MARKOV:
      rate_13k = TRUE;
      if (rxc_so.pri == CAI_SO_MARKOV_SO54)
      {
        byte for_fch_rc = snm_get_current_config()->for_fch_rc;
        rate_13k = (rxc_get_fwd_link_rate_set(for_fch_rc) ==
                         ((byte) CAI_RATE_14400));
      }
      /*---------------------------------------------------------------------
         Fall-through!
      ---------------------------------------------------------------------*/
    case CAI_SO_RS1_MARKOV:
    case CAI_SO_MARKOV:
#ifdef FEATURE_IS2000_1X_ADV
    case CAI_SO_MARKOV_SO74:
#endif /* FEATURE_IS2000_1X_ADV */

      /* Process Markov rate as a function of mux category */
      mar_rate = rxc_mar_rate_sel( mux );

      /* zero out bit error count */
      mux_ber = 0;

      mux_ber = mar_rx (rxc_mar_bits_ptr, (byte) mar_rate,
                        qw_lo (buf_ptr->buf.tc_buf.frame),
                        NULL
                        #ifdef FEATURE_MODEM_1X_MSO_RDA_TEST
                        ,mar_tx_data,
                        (byte) rxc_exp_markov_rate
                         #endif /* FEATURE_MODEM_1X_MSO_RDA_TEST */
                         #ifdef FEATURE_IS2000_1X_ADV
                        /* PCG ACK was transmitted on the R-ACKCH */
                        ,rxc_traffic_status.rxc_rc11.status.ack_transmitted_pcg,
                        /* TRUE = frame is guaranteed, FALSE not guaranteed */
                        rxc_traffic_status.rxc_rc11.status.is_guaranteed_frame
                        #endif /* FEATURE_IS2000_1X_ADV */
                        );

      /* If Markov services are in sync log bit error and frame rate
          decision */
      if( mux_ber != 0xFFFF )
      {
        /* Test if 13k and whether we are in A or B mux category. */
        if (rate_13k)
        {
          if (log_mux > 15)
          {
            log_mux &= 0x0F;
            log_flag = 12 - (mux_ber & 0xFF);
          }
          else
          {
            log_flag = 5 - (mux_ber & 0xFF);
          }
        }
        else
        {
          if ((mux_ber & 0xFF) == 4)
          {
            log_flag = 0;
          }
          else
          {
            log_flag = 8 - (mux_ber & 0xFF);
          }
        }
        rxc_log_mar ( ( byte )(log_flag * 16 + log_mux),
                      ( byte )(mux_ber >> 8 ) );
      }/* mux_ber != 0xFFFF */

      /* zero out symbol error rate and markov statistics if requested */
      if (rxc_zero_count)
      {
        rxc_ser_reset();
        mar_get_stat( &mar );  /* need to read in Markov rate */
        mar_init( (word) mar.m_rate,TRUE,FALSE,cdma.esn, rate_13k );
        rxc_zero_count = FALSE;
      }
      break;

    DS_CASE_DATA
      if( DS_ISDATA_RS2( rxc_so.pri ) )
      {
        log_flag = 0xc0;   /* convention used for logging RS2 */

        if (log_mux > 15)
        {
          log_mux &= 0x0F;
          log_flag = 0xD0;
        }
      }
      else if( DS_ISDATA_RS1( rxc_so.pri ) )
      {
        log_flag = 0xf0;   /* convention used for logging RS1 */

        if (log_mux > 15)
        {
          log_mux &= 0x0F;
          log_flag = 0xD0;
        }
      }
      else
      {
        M1X_MSG( MUX, LEGACY_ERROR,
          " Ouch ! Wrong Data related Service Option ");
      }

      /* Log frame rate decision */
      rxc_log_mar ( (byte) (log_flag | log_mux), 0 );
      break;

    default:
      break;

  }/* switch */
}

#ifdef FEATURE_IS2000_REL_A
/*===========================================================================
FUNCTION      RXC_PREP_TURBO_TDSO_DATA

DESCRIPTION   If Turbo Coding is used, Data is presented in a DSM chain. The
              first entry in DSM array has all the data as a chain. This
              function is not used in case of Convolutional coding because
              data is presented as a DSM array instead of a chain.

              Notice this function is byte oriented rather than bit oriented.
              Byte oriented argorithm is much more efficient and easy to do.
              Therefore, processing of MuxPDU 5 Turbo in TDSO module is also
              specially designed.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void rxc_prep_turbo_tdso_data
(
  tdso_frame_block_type   *tdso_frame_block,
    /* Data structure to copy data to */
  dsm_item_type           *dsm_ptr,
    /* Data structure to copy data from */
  word max_num_bytes
    /* Maximum number of bytes to copy */
)
{
  word dsm_offset = 0;    /* Offset in the dsm chain */
  word bytes_read = 0;
  word blocks_to_copy;
  word total_bytes_read = 0;
  word i, j;         /* loop variables */


  /* SCH frame starts at index 1 in tdso_frame_block */
  #define SCH_OFFSET 1

  if (dsm_ptr == NULL )
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "NULL DSM item received");
    return;
  }

  /* If the total number of bits is greater than those in the DSM item
  ** chain, zero out the difference. This is only required for sTDSO.
  */

  /* How many tdso buffers are needed */
  blocks_to_copy =
        (max_num_bytes % MAX_PDU_LEN) ? (max_num_bytes / MAX_PDU_LEN + 1):
        (max_num_bytes / MAX_PDU_LEN);

  /* Initialize the offset */
  dsm_offset = 0;

  for (i=SCH_OFFSET; i<(blocks_to_copy+SCH_OFFSET) && i < TDSO_MAX_PDU; i++)
  {
    /* read a block */
    bytes_read = dsm_extract(
      dsm_ptr,
      dsm_offset,
      tdso_frame_block->frame_type.fwd_arr[i].fr_item_ptr,
      MAX_PDU_LEN
      );

    total_bytes_read += bytes_read;

    /* If a block is not filled up, pad it with zero, until the
    ** end of the whole frame.
    */
    if ((bytes_read < MAX_PDU_LEN) && (total_bytes_read < max_num_bytes))
    {
      for (j=bytes_read; j<MAX_PDU_LEN; j++)
      {
        tdso_frame_block->frame_type.fwd_arr[i].fr_item_ptr[j] = 0;
      }
    }

    tdso_frame_block->frame_type.fwd_arr[i].valid_frame = TRUE;

    /* We do not officially support TDSO on secondary */
    tdso_frame_block->frame_type.fwd_arr[i].srvc_type =
                                                       TDSO_PRIMARY_SRVC;

    tdso_frame_block->frame_type.fwd_arr[i].sch_rate =
                                   (tdso_sch_rate_type) rxc_sch.sch_rate;

    tdso_frame_block->frame_type.fwd_arr[i].sch_coding =
                               (rxc_sch.code_type == DEC_CONVOLUTIONAL) ?
                               TDSO_CODING_CONV:TDSO_CODING_TURBO;

    tdso_frame_block->frame_type.fwd_arr[i].mux_pdu_type =
                                                           TDSO_MUX_PDU_5;

    tdso_frame_block->frame_type.fwd_arr[i].fr_rate = TDSO_RATE_3;
    tdso_frame_block->frame_type.fwd_arr[i].fr_channel = TDSO_SCH0;
    tdso_frame_block->num_frames++;

    /* Intrement offset */
    dsm_offset += MAX_PDU_LEN;
  }
}  /* rxc_prep_turbo_tdso_data */

#endif /* FEATURE_IS2000_REL_A */

/*===========================================================================
FUNCTION      RXC_TDSO

DESCRIPTION   Procedure does the processing for TDSO SO's.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void rxc_tdso
(
  byte bits[],                  /* Address of first byte of primary data   */
  word bit_pos,                 /* Position of first bit of data in buffer */
  word bit_len,                 /* Number of bits of data in buffer        */
  word mux,                     /* CAI MUX1 frame statistic                */
  rxc_dec_type *buf_ptr         /* Ptr to buffer which holds decoder data  */
)
{
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  word num_sch_bits = 0;
  byte sch_bit_pos = 0;
  byte i;
  cai_radio_config_type cur_rc;
#ifdef FEATURE_IS2000_REL_A
  dsm_item_type *temp_dsm_ptr;
//  uint16 temp_copy_len;
//  uint16 start_shift_pos = 0;
  /* data_block_size indicates the number of bytes for the RC and the given
     SCH Rate */
  word data_block_bytes =  dec_sch_ob_params[DEC_SCH_RC_INDEX(rxc_sch.rc)]
                                           [rxc_sch.sch_rate];
  word bits_to_read = 0;
    /* How much to read from one dsm item */
  word bits_read = 0;
    /* bits already read */
#endif /* FEATURE_IS2000_REL_A */

  // Fill up rxc_tdso_frame_block;
  // FCH / DCCH info
  rxc_tdso_frame_block.num_frames = 1;
  rxc_tdso_frame_block.fr_time = qw_lo (buf_ptr->buf.tc_buf.frame);
  cur_rc = rxc_so.for_dcch.included ? rxc_so.for_dcch.rc : rxc_so.for_fch.rc;
  rxc_tdso_frame_block.fdch_fr_mux = (tdso_mux_type) rxc_get_fwd_link_rate_set(cur_rc);

  cur_rc = ( (cai_radio_config_type) rxc_sch.rc == CAI_INIT_RC ) ?
           CAI_RC_3 : (cai_radio_config_type) rxc_sch.rc;
  rxc_tdso_frame_block.sup_fr_mux = (tdso_mux_type) rxc_get_fwd_link_rate_set(cur_rc);

  /* Default to primary service.
  */
  rxc_tdso_frame_block.frame_type.fwd_arr[0].srvc_type = TDSO_PRIMARY_SRVC;

#ifdef FEATURE_IS2000_REL_A_SVD
  rxc_tdso_frame_block.sec_len = 0;

  /* Change service type and sec_len if TDSO SO is secondary traffic.
  */
  if ((rxc_so.sec == CAI_SO_TDSO) || (rxc_so.sec == CAI_SO_FULL_TDSO))
  {
    rxc_tdso_frame_block.frame_type.fwd_arr[0].srvc_type = TDSO_SECONDARY_SRVC;

    /* Set sec_len to the appropriate value via a lookup
    ** table indexed by an offset of the mux categories.
    */
    if ((mux >= CAI_MUX1_FOR_11_ID) && (mux <= CAI_MUX1_FOR_14_ID))
    {
      rxc_tdso_frame_block.sec_len =
        (word) (mux_rs1_tdso_sec_len [mux - CAI_MUX1_FOR_11_ID]);
    }
    else if ((mux >= CAI_MUX2_FOR_6_ID) && (mux <= CAI_MUX2_FOR_9_ID))
    {
      rxc_tdso_frame_block.sec_len =
        (word) (mux_rs2_tdso_sec_len [mux - CAI_MUX2_FOR_6_ID]);
    }
    else if ((mux >= CAI_MUX1_FOR_DCCH_11_ID) &&
                   (mux <= CAI_MUX1_FOR_DCCH_14_ID))
    {
      rxc_tdso_frame_block.sec_len =
        (word) (mux_rs1_tdso_sec_len [mux - CAI_MUX1_FOR_DCCH_11_ID]);
    }
    else if ((mux >= CAI_MUX2_FOR_DCCH_6_ID) && (mux <= CAI_MUX2_FOR_DCCH_9_ID))
    {
      rxc_tdso_frame_block.sec_len =
        (word) (mux_rs2_tdso_sec_len [mux - CAI_MUX2_FOR_DCCH_6_ID]);
    }
    else
    {
      rxc_tdso_frame_block.sec_len = (dword) TDSO_NON_FULL_FRAME_ON_SEC;
    }
  }
  else if ((rxc_so.pri != CAI_SO_TDSO) && (rxc_so.pri != CAI_SO_FULL_TDSO))
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "Pri and Sec traffic not TDSO, %d, %d",
      rxc_so.pri,
      rxc_so.sec);
  }
#endif /* FEATURE_IS2000_REL_A_SVD */

  if (rxc_tdso_frame_block.fdch_fr_mux == TDSO_RATE_SET_1)
  {
    rxc_tdso_frame_block.frame_type.fwd_arr[0].mux_pdu_type = TDSO_MUX_PDU_1;
  }
  else
  {
    rxc_tdso_frame_block.frame_type.fwd_arr[ 0 ].mux_pdu_type = TDSO_MUX_PDU_2;
  }

  rxc_tdso_frame_block.frame_type.fwd_arr[0].fr_channel =
    rxc_so.for_dcch.included ? TDSO_DCCH : TDSO_FCH;

  /* Primary traffic */
  if (rxc_so.pri == CAI_SO_TDSO || rxc_so.pri == CAI_SO_FULL_TDSO)
  {
    /* Figure out the rate of the incoming frame */
    switch (mux)
    {
      /* Full rate */
      case CAI_MUX1_FOR_1_ID:
      case CAI_MUX2_FOR_1_ID:
      case CAI_MUX1_FOR_DCCH_1_ID:
      case CAI_MUX2_FOR_DCCH_1_ID:
          rxc_tdso_frame_block.frame_type.fwd_arr[0].fr_rate = TDSO_RATE_1;
          break;

      /* Dim-and-Burst */
      case CAI_MUX1_FOR_2_ID:
      case CAI_MUX1_FOR_3_ID:
      case CAI_MUX1_FOR_4_ID:
      case CAI_MUX1_FOR_11_ID:
      case CAI_MUX1_FOR_12_ID:
      case CAI_MUX1_FOR_13_ID:
      case CAI_MUX2_FOR_2_ID:
      case CAI_MUX2_FOR_3_ID:
      case CAI_MUX2_FOR_4_ID:
      case CAI_MUX2_FOR_6_ID:
      case CAI_MUX2_FOR_7_ID:
      case CAI_MUX2_FOR_8_ID:
      case CAI_MUX2_FOR_10_ID:
      case CAI_MUX2_FOR_12_ID:
      case CAI_MUX2_FOR_13_ID:
      case CAI_MUX2_FOR_15_ID:
      case CAI_MUX2_FOR_16_ID:
      case CAI_MUX2_FOR_18_ID:
      case CAI_MUX2_FOR_20_ID:
      case CAI_MUX2_FOR_22_ID:
      case CAI_MUX1_FOR_DCCH_2_ID:
      case CAI_MUX1_FOR_DCCH_3_ID:
      case CAI_MUX1_FOR_DCCH_4_ID:
      case CAI_MUX1_FOR_DCCH_11_ID:
      case CAI_MUX1_FOR_DCCH_12_ID:
      case CAI_MUX1_FOR_DCCH_13_ID:
      case CAI_MUX2_FOR_DCCH_2_ID:
      case CAI_MUX2_FOR_DCCH_3_ID:
      case CAI_MUX2_FOR_DCCH_4_ID:
      case CAI_MUX2_FOR_DCCH_6_ID:
      case CAI_MUX2_FOR_DCCH_7_ID:
      case CAI_MUX2_FOR_DCCH_8_ID:
        rxc_tdso_frame_block.frame_type.fwd_arr[0].fr_rate = TDSO_DIM_BURST;
          break;

      /* Blank-and-Burst */
      case CAI_MUX1_FOR_5_ID:
      case CAI_MUX1_FOR_14_ID:
      case CAI_MUX2_FOR_5_ID:
      case CAI_MUX2_FOR_9_ID:
      case CAI_MUX2_FOR_14_ID:
      case CAI_MUX2_FOR_17_ID:
      case CAI_MUX2_FOR_21_ID:
      case CAI_MUX2_FOR_23_ID:
      case CAI_MUX2_FOR_25_ID:
      case CAI_MUX1_FOR_DCCH_5_ID:
      case CAI_MUX1_FOR_DCCH_14_ID:
      case CAI_MUX2_FOR_DCCH_5_ID:
      case CAI_MUX2_FOR_DCCH_9_ID:
        rxc_tdso_frame_block.frame_type.fwd_arr[0].fr_rate = TDSO_BLANK;
        break;

      /* Erasure */
      case CAI_MUX1_FOR_10_ID:
      case CAI_MUX2_FOR_26_ID:
      case CAI_MUX1_FOR_DCCH_10_ID:
      case CAI_MUX2_FOR_DCCH_26_ID:
        rxc_tdso_frame_block.frame_type.fwd_arr[0].valid_frame = FALSE;
        rxc_tdso_frame_block.frame_type.fwd_arr[0].fr_rate = TDSO_ERASURE;
        break;

      /* 1/2 rate */
      case CAI_MUX1_FOR_6_ID:
      case CAI_MUX2_FOR_11_ID:
          rxc_tdso_frame_block.frame_type.fwd_arr[0].fr_rate = TDSO_RATE_HALF;
          break  ;

      /* 1/4 rate */
      case CAI_MUX1_FOR_7_ID:
      case CAI_MUX2_FOR_19_ID:
          rxc_tdso_frame_block.frame_type.fwd_arr[0].fr_rate = TDSO_RATE_QUARTER;
          break;

      /* 1/8 rate */
      case CAI_MUX1_FOR_8_ID:
      case CAI_MUX2_FOR_24_ID:
      case CAI_MUX2_FOR_DCCH_10_ID:
            rxc_tdso_frame_block.frame_type.fwd_arr[0].fr_rate = TDSO_RATE_EIGHTH;
            break;

      /* Insufficient frame quality */
      case CAI_MUX1_FOR_9_ID:
        rxc_tdso_frame_block.frame_type.fwd_arr[0].fr_rate =
                TDSO_INSUFFICIENT_FRAME;
        break;

      /* Null */
      case CAI_MUX1_FOR_DCCH_15_ID:
      case CAI_MUX2_FOR_DCCH_27_ID:
        rxc_tdso_frame_block.frame_type.fwd_arr[0].fr_rate = TDSO_NULL;
        break;

      default:
        rxc_tdso_frame_block.frame_type.fwd_arr[0].valid_frame = FALSE;
        rxc_tdso_frame_block.frame_type.fwd_arr[0].fr_rate = TDSO_ERASURE;
        break;
    }
  }
  /* Secondary traffic */
  else
  {
    /* Figure out the rate of the incoming frame */
    switch (mux)
    {
      case CAI_MUX1_FOR_14_ID:
      case CAI_MUX2_FOR_9_ID:
      case CAI_MUX1_FOR_DCCH_14_ID:
      case CAI_MUX2_FOR_DCCH_9_ID:
        rxc_tdso_frame_block.frame_type.fwd_arr[0].fr_rate = TDSO_RATE_1;
        break;

      case CAI_MUX1_FOR_11_ID:
      case CAI_MUX1_FOR_12_ID:
      case CAI_MUX1_FOR_13_ID:
      case CAI_MUX2_FOR_6_ID:
      case CAI_MUX2_FOR_7_ID:
      case CAI_MUX2_FOR_8_ID:
      case CAI_MUX2_FOR_10_ID:
      case CAI_MUX2_FOR_15_ID:
      case CAI_MUX2_FOR_16_ID:
      case CAI_MUX2_FOR_18_ID:
      case CAI_MUX2_FOR_22_ID:
      case CAI_MUX1_FOR_DCCH_11_ID:
      case CAI_MUX1_FOR_DCCH_12_ID:
      case CAI_MUX1_FOR_DCCH_13_ID:
      case CAI_MUX2_FOR_DCCH_6_ID:
      case CAI_MUX2_FOR_DCCH_7_ID:
      case CAI_MUX2_FOR_DCCH_8_ID:
      case CAI_MUX2_FOR_DCCH_10_ID:
        rxc_tdso_frame_block.frame_type.fwd_arr[0].fr_rate = TDSO_DIM_BURST;
        break;

      case CAI_MUX1_FOR_1_ID:
      case CAI_MUX1_FOR_2_ID:
      case CAI_MUX1_FOR_3_ID:
      case CAI_MUX1_FOR_4_ID:
      case CAI_MUX1_FOR_5_ID:
      case CAI_MUX1_FOR_6_ID:
      case CAI_MUX1_FOR_7_ID:
      case CAI_MUX1_FOR_8_ID:
      case CAI_MUX2_FOR_1_ID:
      case CAI_MUX2_FOR_2_ID:
      case CAI_MUX2_FOR_3_ID:
      case CAI_MUX2_FOR_4_ID:
      case CAI_MUX2_FOR_5_ID:
      case CAI_MUX2_FOR_11_ID:
      case CAI_MUX2_FOR_12_ID:
      case CAI_MUX2_FOR_13_ID:
      case CAI_MUX2_FOR_14_ID:
      case CAI_MUX2_FOR_19_ID:
      case CAI_MUX2_FOR_20_ID:
      case CAI_MUX2_FOR_21_ID:
      case CAI_MUX2_FOR_24_ID:
      case CAI_MUX1_FOR_DCCH_1_ID:
      case CAI_MUX1_FOR_DCCH_2_ID:
      case CAI_MUX1_FOR_DCCH_3_ID:
      case CAI_MUX1_FOR_DCCH_4_ID:
      case CAI_MUX1_FOR_DCCH_5_ID:
      case CAI_MUX2_FOR_DCCH_1_ID:
      case CAI_MUX2_FOR_DCCH_2_ID:
      case CAI_MUX2_FOR_DCCH_3_ID:
      case CAI_MUX2_FOR_DCCH_4_ID:
      case CAI_MUX2_FOR_DCCH_5_ID:
        rxc_tdso_frame_block.frame_type.fwd_arr[0].fr_rate = TDSO_BLANK;
        break;

      case CAI_MUX1_FOR_9_ID:
        rxc_tdso_frame_block.frame_type.fwd_arr[0].fr_rate = TDSO_INSUFFICIENT_FRAME;
        break;

      case CAI_MUX1_FOR_10_ID:
      case CAI_MUX2_FOR_26_ID:
      case CAI_MUX1_FOR_DCCH_10_ID:
      case CAI_MUX2_FOR_DCCH_26_ID:
        rxc_tdso_frame_block.frame_type.fwd_arr[0].valid_frame = FALSE;
        rxc_tdso_frame_block.frame_type.fwd_arr[0].fr_rate = TDSO_ERASURE;
        break;

      /* Null */
      case CAI_MUX1_FOR_DCCH_15_ID:
      case CAI_MUX2_FOR_DCCH_27_ID:
        rxc_tdso_frame_block.frame_type.fwd_arr[0].fr_rate = TDSO_NULL;
        break;

      /* Other */
      case CAI_MUX2_FOR_17_ID:
      case CAI_MUX2_FOR_23_ID:
      case CAI_MUX2_FOR_25_ID:
        rxc_tdso_frame_block.frame_type.fwd_arr[0].fr_rate = TDSO_OTHER;
        break;

        default:
        rxc_tdso_frame_block.frame_type.fwd_arr[0].valid_frame = FALSE;
          rxc_tdso_frame_block.frame_type.fwd_arr[0].fr_rate = TDSO_ERASURE;
          break;
      }
  }

  b_copy
    (
      bits,
      bit_pos,
      rxc_tdso_frame_block.frame_type.fwd_arr[0].fr_item_ptr,
      0,
      bit_len
    );

  // SCH info
  if ( rxc_frame_data.rxc_sch_frame_status != RXC_NO_FRAME )
  {
    if (rxc_frame_data.rxc_sch_frame_status == RXC_ERASURE_FRAME)
    {
      rxc_tdso_frame_block.frame_type.fwd_arr[1].valid_frame = FALSE;
      rxc_tdso_frame_block.frame_type.fwd_arr[1].fr_rate = TDSO_ERASURE;
      rxc_tdso_frame_block.frame_type.fwd_arr[1].sch_rate =
        (tdso_sch_rate_type) rxc_sch.sch_rate;
#ifdef FEATURE_IS2000_REL_A
      rxc_tdso_frame_block.frame_type.fwd_arr[1].sch_coding =
        (rxc_sch.code_type == DEC_CONVOLUTIONAL) ?
                TDSO_CODING_CONV:TDSO_CODING_TURBO;
#endif /* FEATURE_IS2000_REL_A */
      rxc_tdso_frame_block.frame_type.fwd_arr[1].srvc_type =
        TDSO_PRIMARY_SRVC;
      rxc_tdso_frame_block.frame_type.fwd_arr[1].fr_channel = TDSO_SCH0;
      rxc_tdso_frame_block.num_frames++;
    }
    else if (rxc_frame_data.rxc_sch_frame_status == RXC_DTX_FRAME)
    {
      rxc_tdso_frame_block.frame_type.fwd_arr[1].valid_frame = FALSE;
      rxc_tdso_frame_block.frame_type.fwd_arr[1].fr_rate = TDSO_BLANK;
      rxc_tdso_frame_block.frame_type.fwd_arr[1].sch_rate =
        (tdso_sch_rate_type) rxc_sch.sch_rate;
#ifdef FEATURE_IS2000_REL_A
      rxc_tdso_frame_block.frame_type.fwd_arr[1].sch_coding =
        (rxc_sch.code_type == DEC_CONVOLUTIONAL) ?
                TDSO_CODING_CONV:TDSO_CODING_TURBO;
#endif /* FEATURE_IS2000_REL_A */
      rxc_tdso_frame_block.frame_type.fwd_arr[1].srvc_type = TDSO_PRIMARY_SRVC;
      rxc_tdso_frame_block.frame_type.fwd_arr[1].fr_channel = TDSO_SCH0;
      rxc_tdso_frame_block.num_frames++;
    }
    else
    {
#ifdef FEATURE_IS2000_REL_A
      /* For release A, let's calculate data block size first */
      if (rxc_sch.num_ltu == 0)
      {
        /* LTU = 0 case covers turbo and convolutional 1X & 2X case */
        if (rxc_sch.sch_rate == DEC_SCH_RATE_1X)
        {
          /* For 1X case, there are 12 bits frame CRC and 8 bits tail */
          data_block_bytes -= (DEC_SCH_SIZ_1X_CRC + DEC_SCH_SIZ_TAIL )/ 8;
        }
        else
        {
          /* For 2X and up, there are 16 bits frame CRC and 8 bits tail */
          data_block_bytes -= (DEC_SCH_SIZ_2X_CRC + DEC_SCH_SIZ_TAIL )/ 8;
        }

        /* Convert bytes into bits and deduct 6 bits MuxPDU header */
        num_sch_bits = data_block_bytes*8 - 6;//lint !e734 Loss of precision (assignment) (19 bits to 16 bits)
      }
      else
      {
        /* If there is LTU, then each data block =
        ** LTU_LEN - CRC_LEN - MUXPDU HEADER LEN
        */
        num_sch_bits = rxc_sch.ltu_len - 16 - 6;
      }

      /* If Turbo coding is used the first DSM item has all
         the data */
      if ((rxc_sch.mux_pdu_type == DEC_MUX_PDU_TYPE_5) &&
          (rxc_sch.code_type == DEC_TURBO))

      {
        /* Fill out the data from the first DSM item which includes the chain  */
        /* For mux PDU 5, the header is copied too, so adjust num_sch_bits by 6*/
        rxc_prep_turbo_tdso_data ( &rxc_tdso_frame_block ,
                                   rxc_log_sch_data.rxc_dsm_arr[0],
                                   data_block_bytes );
      }
      else
#endif /* FEATURE_IS2000_REL_A */
      {
        for (i = 0; (i < rxc_log_sch_data.rxc_dsm_arr_cnt) &&
              (rxc_log_sch_data.rxc_dsm_arr_cnt <= SCH_MAX_FWD_PDU); i++)
        {
          rxc_tdso_frame_block.frame_type.fwd_arr[i + 1].valid_frame = TRUE;
          rxc_tdso_frame_block.frame_type.fwd_arr[i + 1].srvc_type =
            TDSO_PRIMARY_SRVC;
          rxc_tdso_frame_block.frame_type.fwd_arr[i + 1].sch_rate =
            (tdso_sch_rate_type) rxc_sch.sch_rate;

#ifdef FEATURE_IS2000_REL_A
          rxc_tdso_frame_block.frame_type.fwd_arr[i + 1].sch_coding =
            (rxc_sch.code_type == DEC_CONVOLUTIONAL) ?
                      TDSO_CODING_CONV:TDSO_CODING_TURBO;

          if (rxc_sch.mux_pdu_type == DEC_MUX_PDU_TYPE_5)
          {
            rxc_tdso_frame_block.frame_type.fwd_arr[ i + 1 ].mux_pdu_type
                                                            = TDSO_MUX_PDU_5;

            rxc_tdso_frame_block.frame_type.fwd_arr[ i + 1 ].fr_rate =
                                                  TDSO_RATE_3;

            sch_bit_pos = 6; // 6 bits of mux pdu type 5 header.
          }
          else    /* not MUX PDU type 5 */
#endif /* FEATURE_IS2000_REL_A */
          {
            if (rxc_sch.sch_rate > DEC_SCH_RATE_1X)
            {
              rxc_tdso_frame_block.frame_type.fwd_arr[i + 1].mux_pdu_type =
                TDSO_MUX_PDU_3;
              if (rxc_sch.mux_pdu_double)
              {
                rxc_tdso_frame_block.frame_type.fwd_arr[i + 1].fr_rate =
                  TDSO_RATE_2;
                num_sch_bits = tdso_data_len [ TDSO_RATE_2]
                  [RXC_UPPER_BOUNDS_CHECK(rxc_tdso_frame_block.sup_fr_mux,
                                          TDSO_MAX_RATE_SET)];
                /* was 346    */
              }
              else
              {
                rxc_tdso_frame_block.frame_type.fwd_arr[i + 1].fr_rate =
                  TDSO_RATE_1;
                num_sch_bits = tdso_data_len [ TDSO_RATE_1]
                   [RXC_UPPER_BOUNDS_CHECK(rxc_tdso_frame_block.sup_fr_mux,
                                           TDSO_MAX_RATE_SET)] - 1;
                /* was 170 */
              }
              sch_bit_pos = 6; // 6 bits of PDU (type 3) header
            }
            else
            {
              if( rxc_tdso_frame_block.sup_fr_mux == TDSO_RATE_SET_1 )
              {
                rxc_tdso_frame_block.frame_type.fwd_arr[ i + 1 ].mux_pdu_type =
                                                                TDSO_MUX_PDU_1;
#ifdef FEATURE_IS2000_REL_A_SVD
                sch_bit_pos = 1; // For Rel A, need to remove Mixed Mode bit
#endif /* FEATURE_IS2000_REL_A_SVD */
              }
              else
              {
                rxc_tdso_frame_block.frame_type.fwd_arr[ i + 1 ].mux_pdu_type =
                                                                TDSO_MUX_PDU_2;
#ifdef FEATURE_IS2000_REL_A_SVD
                sch_bit_pos = 2; // For Rel A, need to remove Erasure and MM bits
#endif /* !FEATURE_IS2000_REL_A_SVD */
              }

              rxc_tdso_frame_block.frame_type.fwd_arr[i + 1].fr_rate =
                TDSO_RATE_1;

              num_sch_bits = tdso_data_len [ TDSO_RATE_1]
                [RXC_UPPER_BOUNDS_CHECK(rxc_tdso_frame_block.sup_fr_mux,
                                        TDSO_MAX_RATE_SET)] ;
              /* was 171 */
#ifndef FEATURE_IS2000_REL_A_SVD
              sch_bit_pos = 0; // Mixed mode bit consumed by decoder driver
#endif /* !FEATURE_IS2000_REL_A_SVD */
            }
          }

          rxc_tdso_frame_block.frame_type.fwd_arr[i + 1].fr_channel = TDSO_SCH0;

#ifdef FEATURE_IS2000_REL_A
          /* now, the data may be spanning across more that one dsm items
             Note that this case applies to 32x only */
          if (rxc_log_sch_data.rxc_dsm_arr[ i ] != NULL)
          {
            /* Clear read counter */
            bits_read = 0;

            if (rxc_log_sch_data.rxc_dsm_arr[ i ]->used*8 >=
                  (sch_bit_pos + num_sch_bits))
            {
              /* One copy should be good enough. Do it now */
              bits_to_read = num_sch_bits;
            }
            else
            {
              /* Need more reading */
              //lint !e734 Loss of precision (assignment) (19 bits to 16 bits)
              bits_to_read = rxc_log_sch_data.rxc_dsm_arr[ i ]->used*8 -
                               sch_bit_pos;
            }

            /* perform the first read */
            b_copy
            (
                rxc_log_sch_data.rxc_dsm_arr[i]->data_ptr,
                sch_bit_pos,
                rxc_tdso_frame_block.frame_type.fwd_arr[i + 1].fr_item_ptr,
                0,
                bits_to_read
            );

            temp_dsm_ptr = rxc_log_sch_data.rxc_dsm_arr[i];
            bits_read += bits_to_read;

            while (temp_dsm_ptr->pkt_ptr != NULL && (bits_read < num_sch_bits))
            {
              /* Move the dsm pointer to the next item on the dsm item chain */
              temp_dsm_ptr = temp_dsm_ptr->pkt_ptr;

              /* If still bits to read */
              bits_to_read = MIN(num_sch_bits - bits_read, temp_dsm_ptr->used*8);//lint !e734 Loss of precision (assignment) (19 bits to 16 bits)

              b_copy
              (
                  temp_dsm_ptr->data_ptr,
                  0,
                  rxc_tdso_frame_block.frame_type.fwd_arr[i + 1].fr_item_ptr,
                  bits_read,
                  bits_to_read
              );

              bits_read += bits_to_read;
            }
          }
          else
          {
            M1X_MSG( MUX, LEGACY_ERROR,
              "NULL rxc_dsm_arr[%d]!",
              i);
          }
#else
          b_copy
          (
            rxc_log_sch_data.rxc_dsm_arr[i]->data_ptr,
            sch_bit_pos,
            rxc_tdso_frame_block.frame_type.fwd_arr[i + 1].fr_item_ptr,
            0,
            num_sch_bits
          );
#endif /* FEATURE_IS2000_REL_A */

          rxc_tdso_frame_block.num_frames++;
          /* num_frames is actually number of PDUs */

        } /* for loop */
      } /* else pdu 5 & turbo coding */
    }

    for (i=0; i<rxc_log_sch_data.rxc_dsm_arr_cnt; i++)
    {
      /*
      ** Free the dsm item first and allocate a new one.
      ** So that data_ptr will be reset. Otherwise, data abort
      ** might happen.
      */

      dsm_free_packet( &rxc_log_sch_data.rxc_dsm_arr[i] );
      rxc_log_sch_data.rxc_dsm_arr[i] = dsm_new_buffer( DSM_DS_SMALL_ITEM_POOL );

    }

    rxc_log_sch_data.rxc_dsm_arr_cnt = 0;
  }

  /* Return value ignored.  Error cases handled within function by printing ERR */
  (void) tdso_process_rx_frames( &rxc_tdso_frame_block );
} /* rxc_tdso() */

#ifdef FEATURE_IS2000_REL_A_SVD
/*===========================================================================
FUNCTION      RXC_DATA

DESCRIPTION   Common processing between primary and secondary data traffic
              for all data SO's.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void rxc_data
(
  byte bits[],                  /* Address of first byte of primary data   */
  word bit_pos,                 /* Position of first bit of data in buffer */
  word bit_len,                 /* Number of bits of data in buffer        */
  word mux,                     /* CAI MUX1 frame statistic                */
  rxc_dec_type *buf_ptr         /* Ptr to buffer which holds decoder data  */
)
{
  dsm_item_type         *item_ptr;                      /* dsm item        */

  /* Unused Parameters */
  (void) mux;
  (void) bit_len;

  /*-------------------------------------------------------------------------
    Copy the FCH data into a dsm item.
  -------------------------------------------------------------------------*/
  item_ptr = dsm_new_buffer( DSM_DS_SMALL_ITEM_POOL);
  if (item_ptr == NULL)
  {
    /*----------------------------------------------------------------------
      Not enough dsm items.  report error, drop mux frame on floor,
      and get out.  This may affect MARKOV stats, and RLP timing.
    ----------------------------------------------------------------------*/
    M1X_MSG( MUX, LEGACY_ERROR,
      "RXC has no DSM items!");
    return;
  }

  #ifdef FEATURE_DATA_MM
  item_ptr->kind = DSM_PS_RLP;
  #else
  item_ptr->kind = PS_RLP;
  #endif /* FEAUTRE_DATA_MM */

  if( rxc_mux_rlp_data.bits_to_copy != 0 )
  {
    b_copy( bits,
            bit_pos,
            item_ptr->data_ptr,
            0,
            rxc_mux_rlp_data.bits_to_copy);
  }

  if( (rxc_so.pri != CAI_SO_PPP_PKT_DATA_3G) &&
      (rxc_so.sec != CAI_SO_PPP_PKT_DATA_3G))
  {
    rxc_pri_data_lspd(item_ptr, buf_ptr, rxc_mux_rlp_data.ds_rate);
  }
  else
  {
    rxc_data_build_rlp_pdu (item_ptr, buf_ptr,
                            rxc_mux_rlp_data.rxc_dsrlp_fund_rate,
                            rxc_mux_rlp_data.bits_to_copy);
  }

} /* end of rxc_data() */
#endif /* FEATURE_IS2000_REL_A_SVD */

/*===========================================================================
FUNCTION      RXC_PRI_DATA

DESCRIPTION   Services primary traffic stream for the FCH and DCCH for all data
              SO's.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void rxc_pri_data
(
  byte bits[],                  /* Address of first byte of primary data   */
  word bit_pos,                 /* Position of first bit of data in buffer */
  word bit_len,                 /* Number of bits of data in buffer        */
  word mux,                     /* CAI MUX1 frame statistic                */
  rxc_dec_type *buf_ptr         /* Ptr to buffer which holds decoder data  */
)
{
  #ifndef FEATURE_IS2000_REL_A_SVD
  word                   bits_to_copy = 0;                  /* byte copy count */
  rlp_rate_enum_type     ds_rate;                       /* FCH fr. rate    */

  dsrlp_rate_enum_type   rxc_dsrlp_fund_rate;           /* FCH fr. rate    */
  dsm_item_type         *item_ptr;                      /* dsm item        */
  #endif /* !FEATURE_IS2000_REL_A_SVD */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /*-------------------------------------------------------------------------
    Figure out the rate of the incoming frame.
  -------------------------------------------------------------------------*/
  switch (mux)
  {
    case CAI_MUX1_FOR_1_ID:
    case CAI_MUX2_FOR_1_ID:
    case CAI_MUX1_FOR_DCCH_1_ID:
    case CAI_MUX2_FOR_DCCH_1_ID:

      rxc_mux_rlp_data.rxc_dsrlp_fund_rate   = DSRLP_RATE_1;
      rxc_mux_rlp_data.ds_rate      = RLP_FULL_RATE;
      rxc_mux_rlp_data.bits_to_copy = bit_len;
      break;

    case CAI_MUX1_FOR_2_ID:  case CAI_MUX1_FOR_6_ID:
    case CAI_MUX1_FOR_11_ID: case CAI_MUX2_FOR_6_ID:
    case CAI_MUX2_FOR_11_ID: case CAI_MUX2_FOR_2_ID:
    case CAI_MUX1_FOR_DCCH_2_ID:
    case CAI_MUX1_FOR_DCCH_11_ID:
    case CAI_MUX2_FOR_DCCH_2_ID:
    case CAI_MUX2_FOR_DCCH_6_ID:
      rxc_mux_rlp_data.rxc_dsrlp_fund_rate   = DSRLP_RATE_1_2;
      rxc_mux_rlp_data.ds_rate      = RLP_HALF_RATE;
      rxc_mux_rlp_data.bits_to_copy = bit_len;
      break;

    case CAI_MUX1_FOR_3_ID:  case CAI_MUX1_FOR_7_ID:
    case CAI_MUX1_FOR_12_ID: case CAI_MUX2_FOR_3_ID:
    case CAI_MUX2_FOR_7_ID:  case CAI_MUX2_FOR_12_ID:
    case CAI_MUX2_FOR_15_ID: case CAI_MUX2_FOR_19_ID:
    case CAI_MUX1_FOR_DCCH_3_ID:
    case CAI_MUX1_FOR_DCCH_12_ID:
    case CAI_MUX2_FOR_DCCH_3_ID:
    case CAI_MUX2_FOR_DCCH_7_ID:
      rxc_mux_rlp_data.rxc_dsrlp_fund_rate   = DSRLP_RATE_1_4;
      rxc_mux_rlp_data.ds_rate      = RLP_QUARTER_RATE;
      rxc_mux_rlp_data.bits_to_copy = bit_len;
      break;

    case CAI_MUX1_FOR_4_ID:  case CAI_MUX1_FOR_8_ID:
    case CAI_MUX1_FOR_13_ID: case CAI_MUX2_FOR_4_ID:
    case CAI_MUX2_FOR_8_ID:  case CAI_MUX2_FOR_10_ID:
    case CAI_MUX2_FOR_13_ID: case CAI_MUX2_FOR_16_ID:
    case CAI_MUX2_FOR_18_ID: case CAI_MUX2_FOR_20_ID:
    case CAI_MUX2_FOR_22_ID: case CAI_MUX2_FOR_24_ID:
    case CAI_MUX1_FOR_DCCH_4_ID:
    case CAI_MUX1_FOR_DCCH_13_ID:
    case CAI_MUX2_FOR_DCCH_4_ID:
    case CAI_MUX2_FOR_DCCH_8_ID:
    case CAI_MUX2_FOR_DCCH_10_ID:
      rxc_mux_rlp_data.rxc_dsrlp_fund_rate   = DSRLP_RATE_1_8;
      rxc_mux_rlp_data.ds_rate      = RLP_EIGHTH_RATE;
      rxc_mux_rlp_data.bits_to_copy = bit_len;
      break;

    case CAI_MUX1_FOR_9_ID:  case CAI_MUX1_FOR_10_ID:
    case CAI_MUX2_FOR_26_ID:
    case CAI_MUX1_FOR_DCCH_10_ID:
    case CAI_MUX2_FOR_DCCH_26_ID:
      rxc_mux_rlp_data.rxc_dsrlp_fund_rate   = DSRLP_RATE_ERASURE;
      rxc_mux_rlp_data.ds_rate      = RLP_ERASURE;
      rxc_mux_rlp_data.bits_to_copy = bit_len;
      break;

    case CAI_MUX1_FOR_5_ID:  case CAI_MUX1_FOR_14_ID:
    case CAI_MUX2_FOR_5_ID:  case CAI_MUX2_FOR_9_ID:
    case CAI_MUX2_FOR_14_ID: case CAI_MUX2_FOR_17_ID:
    case CAI_MUX2_FOR_21_ID: case CAI_MUX2_FOR_23_ID:
    case CAI_MUX2_FOR_25_ID:
    case CAI_MUX1_FOR_DCCH_5_ID:    // Signalling
    case CAI_MUX1_FOR_DCCH_14_ID:   // Secondary
    case CAI_MUX2_FOR_DCCH_5_ID:    // Signalling
    case CAI_MUX2_FOR_DCCH_9_ID:    // Secondary
      rxc_mux_rlp_data.rxc_dsrlp_fund_rate   = DSRLP_RATE_BLANK;
      rxc_mux_rlp_data.ds_rate      = RLP_BLANK;
      rxc_mux_rlp_data.bits_to_copy = 0;
      break;

    case CAI_MUX1_FOR_DCCH_15_ID:   // DTX
    case CAI_MUX2_FOR_DCCH_27_ID:   // DTX
      rxc_mux_rlp_data.rxc_dsrlp_fund_rate   = DSRLP_RATE_DCCH_DTX;
      rxc_mux_rlp_data.ds_rate      = RLP_BLANK;
      rxc_mux_rlp_data.bits_to_copy = 0;
      break;

  default:
      #ifndef FEATURE_IS2000_REL_A_SVD
      M1X_MSG( MUX, LEGACY_ERROR,
        "Invalid mux category %d",
        mux );
      #endif /* !FEATURE_IS2000_REL_A_SVD */
      rxc_mux_rlp_data.rxc_dsrlp_fund_rate   = DSRLP_RATE_BLANK;
      rxc_mux_rlp_data.ds_rate      = RLP_BLANK;
      rxc_mux_rlp_data.bits_to_copy = 0;
      break;
  } /* switch */


  #ifdef FEATURE_IS2000_REL_A_SVD
  /* Moved common processing between primary and secondary data
  ** to function rxc_data() to support secondary data as well.
  */
  rxc_data(bits, bit_pos, bit_len, mux, buf_ptr);
  #else
  /*-------------------------------------------------------------------------
    Copy the FCH data into a dsm item.
  -------------------------------------------------------------------------*/
  item_ptr = dsm_new_buffer( DSM_DS_SMALL_ITEM_POOL);
  if (item_ptr == NULL)
  {
    /*----------------------------------------------------------------------
      Not enough dsm items.  report error, drop mux frame on floor,
      and get out.  This may affect MARKOV stats, and RLP timing.
    ----------------------------------------------------------------------*/
    M1X_MSG( MUX, LEGACY_ERROR,
      "RXC has no DSM items!");
    return;
  }

  #ifdef FEATURE_DATA_MM
  item_ptr->kind = DSM_PS_RLP;
  #else
  item_ptr->kind = PS_RLP;
  #endif /* FEATURE_DATA_MM */

  bits_to_copy = bit_len;

  if( bits_to_copy != 0 )
  {
    b_copy( bits,
            bit_pos,
            item_ptr->data_ptr,
            0,
            bits_to_copy);
  }

  if( rxc_so.pri != CAI_SO_PPP_PKT_DATA_3G)
  {
    rxc_pri_data_lspd(item_ptr, buf_ptr, rxc_mux_rlp_data.ds_rate);
  }
  else
  {
    rxc_data_3g(item_ptr, buf_ptr, rxc_mux_rlp_data.rxc_dsrlp_fund_rate,
                bits_to_copy);
  }
#endif /* FEATURE_IS2000_REL_A_SVD */

} /* rxc_pri_data() */

/*===========================================================================
FUNCTION       RXC_PRI

DESCRIPTION    This function processes the primary traffic received on the
               Forward Traffic Channel.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.
===========================================================================*/
void rxc_pri
(
  byte bits[],               /* Address of first byte of primary data      */
  word bit_pos,              /* Position of first bit of data in buffer    */
  word bit_len,              /* Number of bits of data in buffer           */
  word mux,                  /* CAI MUX1 frame statistic                   */
  rxc_dec_type *buf_ptr      /* Pointer to buffer which holds decoder data */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -- - - - - */
  word so ;
  if(!onex_nonsig_is_ftm_mode())
  {
    /*-----------------------------------------------------------------------
      Get vocoder rate for frame regardless of service option so that DM can
      display it
    -----------------------------------------------------------------------*/
    rxc_frame_rate = rxc_voc_rate_sel( mux );

    so = rxc_so.pri;

    #ifdef FEATURE_1X_CUST_VOICE_SO
    /* This workaround has been added to handle NV dependent non-constant
     * SO while ensuring the existing switch construct is not disrupted */
    if (so == mcc_get_cust_so() && so != CAI_SO_NULL)
    {
      so = CAI_GEN_EVRC_CUST_SO;
    }
    #endif /* FEATURE_1X_CUST_VOICE_SO */

    switch (so)
    {
      /*-----------------------------------------------------------------------
         DS_CASE_DATA is a macro containing several cases.
      -----------------------------------------------------------------------*/
      DS_CASE_DATA
        #ifdef FEATURE_IS2000_REL_A_SVD
        if (rxc_so.pri == CAI_SO_PPP_PKT_DATA_3G)
        {
          /* Obtain 1 linked list item from RLP to store payload info */
          rxc_tc_data_get_rlp_list ();
        }
        #endif /* FEATURE_IS2000_REL_A_SVD */
        rxc_pri_data(bits, bit_pos, bit_len, mux, buf_ptr);
        #ifdef FEATURE_IS2000_REL_A_SVD
        if (rxc_so.pri == CAI_SO_PPP_PKT_DATA_3G)
        {
          /* Give 1 PDU to RLP, if any.
          */
          rxc_tc_data_give_rlp_list ();
        }
        #endif /* FEATURE_IS2000_REL_A_SVD */
        rxc_calc_averaged_fer( mux );
        break;

      #ifdef FEATURE_CDSMS
      case CAI_SO_SMS:
      case CAI_SO_RS2_SMS:
        break;
      #endif /* FEATURE_CDSMS */

      case CAI_SO_RS1_OTAPA:
      case CAI_SO_RS2_OTAPA:
        break;

      case CAI_SO_VOICE_13K_IS733:
      case CAI_SO_VOICE_13K:
      case CAI_SO_VOICE_IS96A:
      case CAI_SO_VOICE_EVRC:
      #ifdef FEATURE_1X_CUST_VOICE_SO
      case CAI_GEN_EVRC_CUST_SO:
      #endif /* FEATURE_1X_CUST_VOICE_SO */
      case CAI_SO_VOICE_EVRC_NW:
      case CAI_SO_VOICE_SMV:
      case CAI_SO_VOICE_4GV_NB:
      case CAI_SO_VOICE_4GV_WB:
        muxvoc_rxc_pri_voice(bits, bit_pos, bit_len, rxc_frame_rate);
        rxc_calc_averaged_fer( mux );
        break;

      #ifdef FEATURE_IS2000_1X_ADV
      case CAI_SO_LOOPBACK_SO75:
      #endif /* FEATURE_IS2000_1X_ADV */
      case CAI_SO_LOOPBACK:
      case CAI_SO_LOOPBACK_13K:
      case CAI_SO_LOOPBACK_SO55:
        #ifdef FEATURE_IS2000_REL_A_SVD
        /* Loopback is only applicable on FCH.
        */
        if (rxc_curr_ch == DSDVRLP_RX_FCH)
        {
          rxc_pri_loopback(bits, bit_pos, bit_len, mux, buf_ptr);
        }
        #else
        rxc_pri_loopback(bits, bit_pos, bit_len, mux, buf_ptr);
        #endif /* FEATURE_IS2000_REL_A_SVD */
        break;

      #ifdef FEATURE_IS2000_1X_ADV
      case CAI_SO_MARKOV_SO74:
      #endif /* FEATURE_IS2000_1X_ADV */
      case CAI_SO_MARKOV:
      case CAI_SO_MARKOV_13K:
      case CAI_SO_RS1_MARKOV:
      case CAI_SO_RS2_MARKOV:
      case CAI_SO_MARKOV_SO54:
        rxc_pri_markov(bits, bit_pos, bit_len, mux, buf_ptr);
        break;
      case CAI_SO_TDSO:
      case CAI_SO_FULL_TDSO:
        rxc_tdso(bits, bit_pos, bit_len, mux, buf_ptr);
        break;

      #ifdef FEATURE_GPSONE
      case CAI_SO_RS1_PD:
      case CAI_SO_RS2_PD:
        break;
      #endif /* FEATURE_GPSONE */

      default:
        M1X_MSG( MUX, LEGACY_ERROR,
          "Invalid service option %d",
          rxc_so.pri );
    } /* end of switch */

    /*-------------------------------------------------------------------------
      Log MAR data. Do this last, as some MAR calculations require rxc_vbits
      to be filled in.
    -------------------------------------------------------------------------*/
    rxc_pri_log_mar(bits, bit_pos, bit_len, mux, buf_ptr);

    #ifndef FEATURE_IS2000_REL_A_SVD
    /*-------------------------------------------------------------------------
      finally release the buf_ptr, as all done.
    -------------------------------------------------------------------------*/
    q_put(&rxc_common_data.rxc_dec_free_q, &buf_ptr->link );
    #endif /* !FEATURE_IS2000_REL_A_SVD */
  }

} /* rxc_pri() */

#ifdef FEATURE_IS2000_REL_A_SVD
/*===========================================================================
FUNCTION      RXC_SEC_DATA

DESCRIPTION   Top level processing for all data SO's.  Eventually calls the
              SO-specific procedure further in.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void rxc_sec_data
(
  byte bits[],                  /* Address of first byte of data           */
  word bit_pos,                 /* Position of first bit of data in buffer */
  word bit_len,                 /* Number of bits of data in buffer        */
  word mux,                     /* CAI MUX frame statistic                 */
  rxc_dec_type *buf_ptr         /* Ptr to buffer which holds decoder data  */
)
{
  /*  Figure out the rate of the incoming frame.
  */
  switch (mux)
  {
    case CAI_MUX1_FOR_11_ID:
    case CAI_MUX1_FOR_DCCH_11_ID:
      rxc_mux_rlp_data.rxc_dsrlp_fund_rate   = DSRLP_RATE_1_2;
      rxc_mux_rlp_data.ds_rate      = RLP_FULL_RATE;
      rxc_mux_rlp_data.bits_to_copy = bit_len;
      break;

    case CAI_MUX1_FOR_12_ID:
    case CAI_MUX1_FOR_DCCH_12_ID:
      rxc_mux_rlp_data.rxc_dsrlp_fund_rate   = DSRLP_RATE_3_4;
      rxc_mux_rlp_data.ds_rate      = RLP_FULL_RATE;
      rxc_mux_rlp_data.bits_to_copy = bit_len;
      break;

    case CAI_MUX1_FOR_13_ID:
    case CAI_MUX1_FOR_DCCH_13_ID:
      rxc_mux_rlp_data.rxc_dsrlp_fund_rate   = DSRLP_RATE_7_8;
      rxc_mux_rlp_data.ds_rate      = RLP_FULL_RATE;
      rxc_mux_rlp_data.bits_to_copy = bit_len;
      break;

    case CAI_MUX1_FOR_14_ID:
    case CAI_MUX1_FOR_DCCH_14_ID:
      rxc_mux_rlp_data.rxc_dsrlp_fund_rate   = DSRLP_RATE_1;
      rxc_mux_rlp_data.ds_rate      = RLP_FULL_RATE;
      rxc_mux_rlp_data.bits_to_copy = bit_len;
      break;

    case CAI_MUX2_FOR_6_ID:
    case CAI_MUX2_FOR_DCCH_6_ID:
      rxc_mux_rlp_data.rxc_dsrlp_fund_rate   = DSRLP_RATE_1_2;
      rxc_mux_rlp_data.ds_rate      = RLP_FULL_RATE;
      rxc_mux_rlp_data.bits_to_copy = bit_len;
      break;

    case CAI_MUX2_FOR_7_ID:
    case CAI_MUX2_FOR_DCCH_7_ID:
      rxc_mux_rlp_data.rxc_dsrlp_fund_rate   = DSRLP_RATE_3_4;
      rxc_mux_rlp_data.ds_rate      = RLP_FULL_RATE;
      rxc_mux_rlp_data.bits_to_copy = bit_len;
      break;

    case CAI_MUX2_FOR_8_ID:
    case CAI_MUX2_FOR_DCCH_8_ID:
      rxc_mux_rlp_data.rxc_dsrlp_fund_rate   = DSRLP_RATE_7_8;
      rxc_mux_rlp_data.ds_rate      = RLP_FULL_RATE;
      rxc_mux_rlp_data.bits_to_copy = bit_len;
      break;

    case CAI_MUX2_FOR_9_ID:
    case CAI_MUX2_FOR_DCCH_9_ID:
      rxc_mux_rlp_data.rxc_dsrlp_fund_rate   = DSRLP_RATE_1;
      rxc_mux_rlp_data.ds_rate      = RLP_FULL_RATE;
      rxc_mux_rlp_data.bits_to_copy = bit_len;
      break;

    case CAI_MUX2_FOR_10_ID:
    case CAI_MUX2_FOR_DCCH_10_ID:
      rxc_mux_rlp_data.rxc_dsrlp_fund_rate   = DSRLP_RATE_1_16;
      rxc_mux_rlp_data.ds_rate      = RLP_FULL_RATE;
      rxc_mux_rlp_data.bits_to_copy = bit_len;
      break;

    case CAI_MUX2_FOR_15_ID:
      rxc_mux_rlp_data.rxc_dsrlp_fund_rate   = DSRLP_RATE_1_4;
      rxc_mux_rlp_data.ds_rate      = RLP_FULL_RATE;
      rxc_mux_rlp_data.bits_to_copy = bit_len;
      break;

    case CAI_MUX2_FOR_16_ID:
      rxc_mux_rlp_data.rxc_dsrlp_fund_rate   = DSRLP_RATE_3_8;
      rxc_mux_rlp_data.ds_rate      = RLP_FULL_RATE;
      rxc_mux_rlp_data.bits_to_copy = bit_len;
      break;

    case CAI_MUX2_FOR_17_ID:
      rxc_mux_rlp_data.rxc_dsrlp_fund_rate   = DSRLP_RATE_7_16;
      rxc_mux_rlp_data.ds_rate      = RLP_FULL_RATE;
      rxc_mux_rlp_data.bits_to_copy = bit_len;
      break;

    case CAI_MUX2_FOR_18_ID:
      rxc_mux_rlp_data.rxc_dsrlp_fund_rate   = DSRLP_RATE_1_16;
      rxc_mux_rlp_data.ds_rate      = RLP_FULL_RATE;
      rxc_mux_rlp_data.bits_to_copy = bit_len;
      break;

    case CAI_MUX2_FOR_22_ID:
      rxc_mux_rlp_data.rxc_dsrlp_fund_rate   = DSRLP_RATE_1_8;
      rxc_mux_rlp_data.ds_rate      = RLP_FULL_RATE;
      rxc_mux_rlp_data.bits_to_copy = bit_len;
      break;

    case CAI_MUX2_FOR_23_ID:
      rxc_mux_rlp_data.rxc_dsrlp_fund_rate   = DSRLP_RATE_3_16;
      rxc_mux_rlp_data.ds_rate      = RLP_FULL_RATE;
      rxc_mux_rlp_data.bits_to_copy = bit_len;
      break;

    case CAI_MUX2_FOR_25_ID:
      rxc_mux_rlp_data.rxc_dsrlp_fund_rate   = DSRLP_RATE_1_16;
      rxc_mux_rlp_data.ds_rate      = RLP_FULL_RATE;
      rxc_mux_rlp_data.bits_to_copy = bit_len;
      break;

    case CAI_MUX1_FOR_9_ID:
    case CAI_MUX1_FOR_10_ID:
    case CAI_MUX2_FOR_26_ID:
    case CAI_MUX1_FOR_DCCH_10_ID:
    case CAI_MUX2_FOR_DCCH_26_ID:
      rxc_mux_rlp_data.rxc_dsrlp_fund_rate   = DSRLP_RATE_ERASURE;
      rxc_mux_rlp_data.ds_rate      = RLP_ERASURE;
      rxc_mux_rlp_data.bits_to_copy = bit_len;
      break;

    case CAI_MUX1_FOR_DCCH_15_ID:   // DTX
    case CAI_MUX2_FOR_DCCH_27_ID:   // DTX
      rxc_mux_rlp_data.rxc_dsrlp_fund_rate   = DSRLP_RATE_DCCH_DTX;
      rxc_mux_rlp_data.ds_rate      = RLP_BLANK;
      rxc_mux_rlp_data.bits_to_copy = 0;
      break;

    default:
      rxc_mux_rlp_data.rxc_dsrlp_fund_rate   = DSRLP_RATE_BLANK;
      rxc_mux_rlp_data.ds_rate      = RLP_BLANK;
      rxc_mux_rlp_data.bits_to_copy = 0;
      break;

  } /* switch */

  /* Common processing between primary and secondary data is done
  ** in function rxc_data().
  */
  rxc_data(bits, bit_pos, bit_len, mux, buf_ptr);

} /* end rxc_sec_data () */

/*===========================================================================
FUNCTION       RXC_SEC

DESCRIPTION    This function processes the secondary traffic stream received on
               the Forward Traffic Channel.

               Currently, only data SOs are supported in the secondary traffic
               stream.  It may be necessary to add other SOs as needed in the
               future.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.
===========================================================================*/
void rxc_sec
(
  byte bits[],               /* Address of first byte of secondary data    */
  word bit_pos,              /* Position of first bit of data in buffer    */
  word bit_len,              /* Number of bits of data in buffer           */
  word mux,                  /* CAI MUX1 frame statistic                   */
  rxc_dec_type *buf_ptr      /* Pointer to buffer which holds decoder data */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -- - - - - */
  if (!onex_nonsig_is_ftm_mode())
  {
    switch (rxc_so.sec)
    {
      /* DS_CASE_DATA is a macro containing several cases. It is defined in
      */
      DS_CASE_DATA
      if (rxc_so.sec == CAI_SO_PPP_PKT_DATA_3G)
      {
        /* Obtain 1 linked list item from RLP to store payload info.
        */
        rxc_tc_data_get_rlp_list ();
      }
      rxc_sec_data(bits, bit_pos, bit_len, mux, buf_ptr);

      if (rxc_so.sec == CAI_SO_PPP_PKT_DATA_3G)
      {
        /* Give 1 PDU to RLP, if any.
        */
        rxc_tc_data_give_rlp_list ();
      }
      break;

      case CAI_SO_TDSO:
      case CAI_SO_FULL_TDSO:
        rxc_tdso(bits, bit_pos, bit_len, mux, buf_ptr);
        break;

      default:
        M1X_MSG( MUX, LEGACY_ERROR,
          "SO %d not supported as secondary traffic",
          rxc_so.sec );
    } /* end of switch */
  }
} /* end rxc_sec () */
#endif /* FEATURE_IS2000_REL_A_SVD */

/*===========================================================================

FUNCTION RXC_TC_DATA_FTM

DESCRIPTION
  This procedure processes the data for FEATURE_FACTORY_TESTMODE.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_tc_data_ftm (
  rxc_dec_type *buf_ptr
  )
{
  /* Keep a total of all types of frames seen for logging */
  static dword fch_total_frames = 0;    /* total count FCH frames */
  static dword fch_fer = 0;             /* total FCH frame error count */
  /* Support FEATURE_FTM_DCCH */
  static dword dcch_total_frames = 0;   /* total count of DCCH frames */
  static dword dcch_fer = 0;            /* total DCCH frame error count */
  static dword dcch_num_dtx = 0;        /* total DCCH DTX count */
  /* End of FEATURE_FTM_DCCH */

  static dword sch_total_frames = 0;    /* total count of SCH frames */
  static dword sch_fer = 0;             /* total SCH frame error count */
  static dword sch_num_dtx = 0;         /* total SCH DTX count */

  /* A flag to indicate that it is time to send a packet */
  boolean send_log_pkt = FALSE;
  /* A flag indicating that we should flush the counters */
  boolean zero_counters = FALSE;

  size_t i; /* a counter */

  if( log_status( LOG_FTM_C ) )
  {
    /* check for FCH */
    if (rxc_so.for_fch.included)
    {
      fch_total_frames++;
      if(buf_ptr->dec_rate == CAI_BLANK_RATE)
      {
        fch_fer++;
      }
    }
    /* Support FEATURE_FTM_DCCH */
    /* check for DCCH */
    if (rxc_so.for_dcch.included)
    {
      dcch_total_frames++;
  #ifdef FEATURE_IS2000_REL_A_SVD
      if(buf_ptr->dcch_dec_rate == CAI_BLANK_RATE)
      {
        dcch_fer++;
      }
      else if(buf_ptr->dcch_dec_rate == CAI_NULL_RATE)
      {
        dcch_num_dtx++;
      }
  #else
      if(buf_ptr->dec_rate == CAI_BLANK_RATE)
      {
        dcch_fer++;
      }
      else if(buf_ptr->dec_rate == CAI_NULL_RATE)
      {
        dcch_num_dtx++;
      }
  #endif /* FEATURE_IS2000_REL_A_SVD */
    }
    /* End of FEATURE_FTM_DCCH */

    if(rxc_sup_dec_int_enable)
    {
      sch_total_frames++;
      switch (rxc_frame_data.rxc_sch_frame_status)
      {
        case RXC_DTX_FRAME:
          sch_num_dtx++;
          break;
        case RXC_ERASURE_FRAME:
          sch_fer++;
          break;
        case RXC_GOOD_FRAME:
        case RXC_NO_FRAME:
          /* Do nothing for these results */
          break;
      }
    }
    if (rxc_so.for_fch.included)
    {
      if(fch_total_frames % 100 == 0)
      {
        send_log_pkt = TRUE;
      }
    }
    /* Support FEATURE_FTM_DCCH */
    if (rxc_so.for_dcch.included)
    {
      if(dcch_total_frames % 100 == 0)
      {
        send_log_pkt = TRUE;
      }
    }
    /* End of FEATURE_FTM_DCCH */
  }
  else
  {
    /* No logging is occuring, so zero the counters.  I would
       flush the residual numbers, but logging is off at this time
       so it has no where to go. */
    if (rxc_so.for_fch.included)
    {
      if(fch_total_frames > 0)
      {
        zero_counters = TRUE;
      }
    }
    /* Support FEATURE_FTM_DCCH */
    if (rxc_so.for_dcch.included)
    {
      if(dcch_total_frames > 0)
      {
        zero_counters = TRUE;
      }
    }
    /* End of FEATURE_FTM_DCCH */
  }

  if(send_log_pkt)
  {
    mux_log_ftm_type *ftm_log_pkt = (mux_log_ftm_type *) log_alloc( LOG_FTM_C,
                                              sizeof(mux_log_ftm_type));
    if(ftm_log_pkt)
    {
      ftm_log_pkt->fch_total_frames = fch_total_frames;
      ftm_log_pkt->fch_fer          = fch_fer;
      /* Support FEATURE_FTM_DCCH */
      ftm_log_pkt->dcch_total_frames = dcch_total_frames;
      ftm_log_pkt->dcch_fer          = dcch_fer;
      ftm_log_pkt->dcch_num_dtx      = dcch_num_dtx;
      /* End of FEATURE_FTM_DCCH */

      ftm_log_pkt->sch_total_frames = sch_total_frames;
      ftm_log_pkt->sch_fer          = sch_fer;
      ftm_log_pkt->sch_num_dtx      = sch_num_dtx;

      log_commit( ftm_log_pkt );
    }
  }

  if(zero_counters)
  {
    fch_total_frames = fch_fer = 0;
    sch_total_frames = 0;
    sch_fer = sch_num_dtx = 0;

    /* Support FEATURE_FTM_DCCH */
    dcch_total_frames = dcch_fer = dcch_num_dtx = 0;
    /* End of FEATURE_FTM_DCCH */
  }

  if (rxc_fch_loopback_query())
  {

    /* Change RX rate (4=full, 3=half..) to TX rate (0=full, 1=half,)*/
    switch(buf_ptr->dec_rate)
    {

      case CAI_BLANK_RATE:
        txc_ftm_rfch_rate = ENC_FULL_RATE;
        memset((void *)txc_ftm_rfch_data.data, 0, DEC_FCH_MAX_SIZ_FULL);
        break;
      case CAI_FULL_RATE:
        txc_ftm_rfch_rate = ENC_FULL_RATE;
        memscpy(txc_ftm_rfch_data.data,
                sizeof(txc_ftm_rfch_data.data),
                buf_ptr->buf.tc_buf.bits.buf.bits1,
                DEC_FCH_MAX_SIZ_FULL);
        break;
      case CAI_HALF_RATE:
        txc_ftm_rfch_rate = ENC_HALF_RATE;
        memscpy(txc_ftm_rfch_data.data,
                sizeof(txc_ftm_rfch_data.data),
                buf_ptr->buf.tc_buf.bits.buf.bits2,
                DEC_FCH_MAX_SIZ_HALF);
        break;
      case CAI_QUARTER_RATE:
        txc_ftm_rfch_rate = ENC_QTR_RATE;
        memscpy(txc_ftm_rfch_data.data,
                sizeof(txc_ftm_rfch_data.data),
                buf_ptr->buf.tc_buf.bits.buf.bits4,
                DEC_FCH_MAX_SIZ_QTR);
        break;
      case CAI_EIGHTH_RATE:
        txc_ftm_rfch_rate = ENC_8TH_RATE;
        memscpy(txc_ftm_rfch_data.data,
                sizeof(txc_ftm_rfch_data.data),
                buf_ptr->buf.tc_buf.bits.buf.bits8,
                DEC_FCH_MAX_SIZ_8TH);
        break;

      case CAI_NULL_RATE:
      default:
        /* Do nothing */
        break;
    }
  }
  /* Support FEATURE_FTM_DCCH */
  if (rxc_dcch_loopback_query())
  {
    switch(buf_ptr->dec_rate)
    {
      case CAI_BLANK_RATE:
        txc_ftm_rdcch_rate = ENC_FULL_RATE;
        memset((void *)txc_ftm_rdcch_data.data, 0, DEC_FCH_MAX_SIZ_FULL);
        break;
      case CAI_FULL_RATE:
        txc_ftm_rdcch_rate = ENC_FULL_RATE;
        memscpy(txc_ftm_rdcch_data.data,
                sizeof(txc_ftm_rdcch_data.data),
                buf_ptr->buf.tc_buf.bits.buf.bits1,
                DEC_FCH_MAX_SIZ_FULL);
        break;
      case CAI_NULL_RATE: /* DCCH DTX */
         txc_ftm_dtx_dcch = TRUE;
        break;

      case CAI_EIGHTH_RATE:
      case CAI_QUARTER_RATE:
      case CAI_HALF_RATE:
      default:
        /* Do nothing */
        break;
    }
  }
  /* End of FEATURE_FTM_DCCH */

  if (rxc_sch_loopback_query())
  {
    word copied=0;

    for ( i = 0;
          ( ( i < rxc_log_sch_data.rxc_dsm_arr_cnt ) &&
            ( i < SCH_MAX_FWD_PDU ) &&
            ( rxc_log_sch_data.rxc_dsm_arr[i]->used <=
                rxc_log_sch_data.rxc_dsm_arr[i]->size ) );
          i++ )
    {
      if (rxc_log_sch_data.rxc_dsm_arr[i]->used >
            sizeof(txc_ftm_rsch_data.data) - copied)
      {
        M1X_MSG( MUX, LEGACY_ERROR,
          "Size of PDU (%d) exceeds the limit (%d), hence dropped",
          (copied + rxc_log_sch_data.rxc_dsm_arr[i]->used),
          sizeof(txc_ftm_rsch_data.data) );
        break;
      }
      copied += memscpy(&(txc_ftm_rsch_data.data[copied]),
                        sizeof(txc_ftm_rsch_data.data) - copied,
                        rxc_log_sch_data.rxc_dsm_arr[i]->data_ptr,
                        rxc_log_sch_data.rxc_dsm_arr[i]->used);
    }
    txc_ftm_rsch_data.size = copied;
  }

  if( ( rxc_log_sch_data.rxc_dsm_arr_cnt != RXC_DSM_ARR_NOT_ALLOC ) &&
      ( rxc_log_sch_data.rxc_dsm_arr_cnt < SCH_MAX_FWD_PDU ) )
  {
    for (i=0; i<rxc_log_sch_data.rxc_dsm_arr_cnt; i++)
    {
      dsm_free_packet( &rxc_log_sch_data.rxc_dsm_arr[i] );
      rxc_log_sch_data.rxc_dsm_arr[i] = dsm_new_buffer( DSM_DS_SMALL_ITEM_POOL);
    }
    rxc_log_sch_data.rxc_dsm_arr_cnt = 0;
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "Size of rxc_dsm_arr_cnt (%d) exceeds the limit (%d)",
      rxc_log_sch_data.rxc_dsm_arr_cnt, 
      SCH_MAX_FWD_PDU );
  }
  q_put(&rxc_common_data.rxc_dec_free_q, &buf_ptr->link );
}

#ifdef FEATURE_IS2000_REL_A_SVD
/*===========================================================================

FUNCTION RXC_TC_DATA_DCCH

DESCRIPTION
  This procedure processes data received from the decoder while the Receive
  task is processing the Traffic Channel.  This procedure handles the F_DCCH
  only.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
byte rxc_tc_data_dcch
(
  rxc_dec_type *buf_ptr
  /* Pointer to buffer received on rxc_dec_q */
)
{
  rxc_demux_info_type demux_info = {0};
  byte mux = 0;                 /* MUX option determined for the frame */

  mccrx_rpt_type *rpt_ptr;
    /* pointer to report to send to Main Control task */

  static byte dcch_sig_buf[DEC_FCH_MAX_SIZ_FULL];
    /* Buffer for signaling data */

  static boolean tt_fade_msg = FALSE;

  boolean rxc_ta_is_enabled = rxc_is_ta_enabled();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  mux = rxc_demux_fundicated_ch ( buf_ptr, buf_ptr->dcch_dec_rate,
                                  &demux_info, TRUE);

  /*--------------------Update values for Power Measurement Report----------*/

  if (
      (mux == CAI_MUX1_FOR_DCCH_10_ID) ||
      (mux == CAI_MUX2_FOR_DCCH_26_ID) ||
      (buf_ptr->dcch_chan_quality == RXC_POOR_CHANNEL)
     )
  {
    if (rxc_pwr.num_delay_frames == 0)
    {
      /* If Return-If-Fail HHO or CFS is in progress, suspend counting
       * BAD_FRAMES per section 6.6.6.2.8.2 of IS-95B.
       */
      if ( ! rxc_rifhho_cfs_gps_in_progress )
      {
        rxc_pwr.dcch_num_bad_frames++;
      }
    }
  }

  /*--------------------Power control channel fade check--------------------*/

  if (ffpc_channel == CAI_FPC_PRI_CHAN_DCCH)
  {
    if ( mux == CAI_MUX1_FOR_DCCH_10_ID || mux == CAI_MUX2_FOR_DCCH_26_ID ||
         (rxc_so.for_dcch.included &&
          buf_ptr->dcch_chan_quality == RXC_POOR_CHANNEL ) )
    {
      rxc_ta_fade_check();

      srch_mux_report_frame ( TRUE, TRUE, buf_ptr->dcch_dec_rate,
                              (word)rxc_so.for_dcch.rc, &rxc_dcch_status );

    }
    else
    {
      /* Reset T5m timer, if fade check is inactive and 1x is out of TA GAP */
      if( !rxc_fade_data.rxc_fade && !rxc_ta_is_enabled )
      {
        rxc_fade_data.rxc_t5m_bad_frames = 0;
      }

      srch_mux_report_frame ( FALSE, TRUE, buf_ptr->dcch_dec_rate,
                              (word)rxc_so.for_dcch.rc, &rxc_dcch_status );
    }

    if( rxc_rifhho_cfs_gps_in_progress && !rxc_ta_is_enabled )
    {
      /* If Return-If-Fail HHO or CFS is in progress, start counting
       * lost frames as per section 6.6.6.2.8.2 of IS-95B.
       * LOST_FRAMES is used to measure the interval between the time
       * that the mobile station disables its transmitter and the time
       * that it resumes using the SF in the case of failed HHO with
       * Return on Failure.  Under some circumstances, a good frame can
       * be received on the TF/CF, but it is still a lost frame.
       */
      rxc_hho_data.rxc_lost_frames++;

      if (rxc_hho_data.rxc_lost_frames == rxc_hho_data.rxc_rif_hho_timeout_frames)
      {
        /* rxc_ignore_good_frame is set to TRUE here to ignore any good
         * frames that come after the handoff has timed out.  This
         * prevents us from sending RXC_TX_ON_R after RXC_HHO_TIMEOUT_R.
         */
        rxc_ignore_good_frame = TRUE;

        /* send report of handoff timeout to Main Control task */
        if ( (rpt_ptr = (mccrx_rpt_type *)q_get(&mcc_rx_free_q) ) != NULL )
        {
          rpt_ptr->hdr.rpt = RXC_HHO_TIMEOUT_R;
          rxc_mc_rpt(rpt_ptr );
        }
        else
        {
          M1X_MSG( MUX, LEGACY_ERROR,
            "No buffers on mcc_rx_free_q" );
          rxc_hho_data.rxc_rif_hho_timeout_frames++; /* Try again next frame */
        }
      }
    }

    if (rxc_hho_data.rxc_hard_handoff_in_progress)
    {
      /* Allow fade checking to resume for the normal HHO begin process.*/
      rxc_fade_data.rxc_pre_tt_ho = FALSE;
      if(!tt_fade_msg)
      {
        M1X_MSG( MUX, LEGACY_MED,
          "Resume fade checking post TT");
        tt_fade_msg = TRUE;
      }
    }
    else
    {
      tt_fade_msg = FALSE;
    }

    /* If fade has started perform reception watchdog processing.
    ** Rel D fast call set up feature also requires this processing
    ** to send 2 valid frame report in TC state.
    */

    /* In time transfer cases we want to prevent RX from fade checking
       and issuing any reports back to MC until the HHO_BEGIN action is
       processed. */

    if ((rxc_fade_data.rxc_fade)&&(rxc_fade_data.rxc_ho_cnt == 0)&&
        (!rxc_fade_data.rxc_pre_tt_ho))
    {
      fade_check_dcch (buf_ptr->dcch_chan_quality);
    }
  }

  /* increment relevant parameter */
  ( void )parm_inc( mux, 1 );

/*--------------------Process Signaling traffic---------------------------*/

  /* Process Signaling traffic */
  if( demux_info.sig_len > 0 )
  {
    b_copy( &buf_ptr->buf.tc_buf.bits.bits[demux_info.dec_pos], demux_info.sig_pos,
            dcch_sig_buf, 0, demux_info.sig_len );
    M1X_MSG( MUX, LEGACY_MED,
      "Parsing signalling msg on DCCH");
    /* If the signalling portion of the frame is bad, mark the
    ** entire frame as bad.
    */
    if (!rxc_tc_parse( buf_ptr->buf.tc_buf.frame, dcch_sig_buf, demux_info.sig_len,
                       RXC_FOR_DCCH ))
      mux = CAI_MUX2_FOR_26_ID;
  }

/*--------------------Process Primary traffic-----------------------------*/

  /* Process Primary traffic.
  */
  if (
      (rxc_so.pri != CAI_SO_NULL) && (!CAI_IS_SO_VOICE(rxc_so.pri))
     )
  {
    if (DS_ISDATA_RS1(rxc_so.pri) || DS_ISDATA_RS2(rxc_so.pri))
    {
      /* record RLP bit length for logging */
      rxc_bufs_qs_data.rxc_rlp_logging_chan_info.log_dcch_rlp_bits =
        demux_info.pri_len;
    }

    /* Primary traffic service option active - process primary traffic
    */
    rxc_pri( &buf_ptr->buf.tc_buf.bits.bits[demux_info.dec_pos],
             demux_info.pri_pos, demux_info.pri_len, mux, buf_ptr );
  }

/*--------------------Process Secondary traffic---------------------------*/

  /* Process Secondary traffic.
  */
  if (rxc_so.sec != CAI_SO_NULL)
  {
    if (DS_ISDATA_RS1(rxc_so.sec) || DS_ISDATA_RS2(rxc_so.sec))
    {
      /* record RLP bit length for logging */
      rxc_bufs_qs_data.rxc_rlp_logging_chan_info.log_dcch_rlp_bits =
        demux_info.sec_len;
    }

    /* Secondary traffic service option active - process secondary traffic
    */
    rxc_sec( &buf_ptr->buf.tc_buf.bits.bits[demux_info.dec_pos],
             demux_info.sec_pos, demux_info.sec_len, mux, buf_ptr );
  }

  /* Return mux value for logging purpose */
  return (mux);

}/* rxc_tc_data_dcch */
#endif /* FEATURE_IS2000_REL_A_SVD */

/*===========================================================================

FUNCTION RXC_DEMUX_FUNDICATED_CH

DESCRIPTION
  Extract position and number of bits to be demultiplexed
  into traffic streams based on Current Rate Set, returned
  dec_rate from the rate determination algorithm and the Mixed
  Mode Bit and Frame Mode bits in the frame.

DEPENDENCIES
  None.

RETURN VALUE
  Mux ID

SIDE EFFECTS
  None.

===========================================================================*/
byte rxc_demux_fundicated_ch
(
  rxc_dec_type *buf_ptr,
  cai_data_rate_type dec_rate,
  rxc_demux_info_type *demux_info,
  boolean is_channel_dcch
)
{
  byte *dec_bits = NULL;         /* pointer to raw data buffer */
  byte frame_mode = 0;           /* Frame Mode demultiplexing bits */
  byte mux = CAI_MUX1_FOR_10_ID; /* MUX option determined for the frame */
  rxc_demux_info_type temp_demux_info = {0};
  cai_radio_config_type cur_rc;
  boolean rate_error_detected = FALSE;
  boolean invalid_mux_category = FALSE;
  boolean bad_data_detected = FALSE;

  /* Position of bits to be read from the decoder based on rate.
   * Values are based on the position of the buffers in struct
   * dec_fch_type.
   */
  const word dec_position [5] =
    {   0,
       (DEC_FCH_MAX_SIZ_FULL+
        DEC_FCH_MAX_SIZ_HALF+
        DEC_FCH_MAX_SIZ_QTR),
       (DEC_FCH_MAX_SIZ_FULL+
        DEC_FCH_MAX_SIZ_HALF),
       (DEC_FCH_MAX_SIZ_FULL),
        0
    };

/*------------------------------------------------------------------------*/

  if(is_channel_dcch)
  {
    cur_rc = rxc_so.for_dcch.rc;
  }
  else
  {
    cur_rc = rxc_so.for_fch.rc;
  }

  /*--------------------Try rate set 2--------------------------------------*/

  if ( rxc_get_fwd_link_rate_set(cur_rc) == DEC_RATE_14400 )
  {
    #ifdef FEATURE_IS2000_REL_A_SVD
    /* RLP needs to know the MUX PDU type. */
    rxc_mux_pdu = DSRLP_MUX_PDU_2;
    #endif /* FEATURE_IS2000_REL_A_SVD */

    switch (dec_rate)
    {
      case CAI_BLANK_RATE:
        if(is_channel_dcch)
        {
          mux = CAI_MUX2_FOR_DCCH_26_ID;
        }
        else
        {
          mux = CAI_MUX2_FOR_26_ID;
        }
        break;

      case CAI_FULL_RATE:

        if(is_channel_dcch)
        {
          #ifdef FEATURE_IS2000_REL_A_SVD
          dec_bits = buf_ptr->buf.tc_buf.bits.buf.bits1dcch;
          #else
          dec_bits = buf_ptr->buf.tc_buf.bits.buf.bits1;
          #endif /* FEATURE_IS2000_REL_A_SVD */
          mux = CAI_MUX2_FOR_DCCH_1_ID;
        }
        else
        {
          dec_bits = buf_ptr->buf.tc_buf.bits.buf.bits1;
          mux = CAI_MUX2_FOR_1_ID;
        }

        if ( (dec_bits[0] & MUX2_MM_BIT) )
        {
          frame_mode = ( dec_bits[0] & MUX2_FULL_FM ) >> MUX2_FULL_SHIFT;
          temp_demux_info.pri_pos  = mux2_full_pri_pos  [frame_mode];
          temp_demux_info.pri_len  = mux2_full_pri_len  [frame_mode];
          temp_demux_info.sig_pos  = mux2_full_sig_pos  [frame_mode];
          temp_demux_info.sig_len  = mux2_full_sig_len  [frame_mode];
          temp_demux_info.sec_len  = mux2_full_sec_len  [frame_mode];

          #ifdef FEATURE_IS2000_REL_A_SVD
          temp_demux_info.sec_pos  = mux2_full_sec_pos  [frame_mode];
          #endif /* FEATURE_IS2000_REL_A_SVD */

          mux += frame_mode + 1;

          /* Throw out clearly invalid mux categories for full rate.
          */
          if (frame_mode > MUX2_MAX_FULL_FM)
          {
            invalid_mux_category = TRUE;
          }
        }
        else
        {
          /* full rate primary traffic only */
          temp_demux_info.pri_pos = MUX2_PRI_POS;
          temp_demux_info.pri_len = MUX2_FULL_PRI_LEN;
        }
        break;

      case CAI_HALF_RATE:

        if(is_channel_dcch)
        {
          rate_error_detected = TRUE;
          break;
        }

        dec_bits = buf_ptr->buf.tc_buf.bits.buf.bits2;
        if ( (dec_bits[0] & MUX2_MM_BIT) )
        {
          frame_mode = ( dec_bits[0] & MUX2_HALF_FM ) >> MUX2_HALF_SHIFT;
          temp_demux_info.pri_pos = mux2_half_pri_pos [frame_mode];
          temp_demux_info.pri_len = mux2_half_pri_len [frame_mode];
          temp_demux_info.sig_pos = mux2_half_sig_pos [frame_mode];
          temp_demux_info.sig_len = mux2_half_sig_len [frame_mode];
          temp_demux_info.sec_len = mux2_half_sec_len [frame_mode];

          #ifdef FEATURE_IS2000_REL_A_SVD
          temp_demux_info.sec_pos = mux2_half_sec_pos [frame_mode];
          #endif /* FEATURE_IS2000_REL_A_SVD */

          mux = CAI_MUX2_FOR_11_ID + frame_mode + 1;

          /* Throw out clearly invalid mux categories for half rate.
          */
          if (frame_mode > MUX2_MAX_HALF_FM)
          {
            invalid_mux_category = TRUE;
          }
        }
        else
        {
          /* half rate primary traffic only */
          temp_demux_info.pri_pos = MUX2_PRI_POS;
          temp_demux_info.pri_len = MUX2_HALF_PRI_LEN;
          mux = CAI_MUX2_FOR_11_ID;
        }
        break;

      case CAI_QUARTER_RATE:

        if(is_channel_dcch)
        {
          rate_error_detected = TRUE;
          break;
        }

        dec_bits = buf_ptr->buf.tc_buf.bits.buf.bits4;
        if ( (dec_bits[0] & MUX2_MM_BIT) )
        {
          frame_mode = ( dec_bits[0] & MUX2_QTR_FM ) >> MUX2_QTR_SHIFT;
          temp_demux_info.pri_pos = mux2_qtr_pri_pos [frame_mode];
          temp_demux_info.pri_len = mux2_qtr_pri_len [frame_mode];
          temp_demux_info.sig_pos = mux2_qtr_sig_pos [frame_mode];
          temp_demux_info.sig_len = mux2_qtr_sig_len [frame_mode];
          temp_demux_info.sec_len = mux2_qtr_sec_len [frame_mode];

          #ifdef FEATURE_IS2000_REL_A_SVD
          temp_demux_info.sec_pos = mux2_qtr_sec_pos [frame_mode];
          #endif /* FEATURE_IS2000_REL_A_SVD */

          mux = CAI_MUX2_FOR_19_ID + frame_mode + 1;
        }
        else
        {
          /* quarter rate primary traffic only */
          temp_demux_info.pri_pos = MUX2_PRI_POS;
          temp_demux_info.pri_len = MUX2_QTR_PRI_LEN;
          mux = CAI_MUX2_FOR_19_ID;
        }
        break;

      case CAI_EIGHTH_RATE:

        if(is_channel_dcch)
        {
          rate_error_detected = TRUE;
          break;
        }

        dec_bits = buf_ptr->buf.tc_buf.bits.buf.bits8;

        if ( (dec_bits[0] & MUX2_MM_BIT) )
        {
          /* eighth rate secondary traffic only */
          #ifdef FEATURE_IS2000_REL_A_SVD
          temp_demux_info.sec_pos = MUX2_PRI_POS;
          #endif /* FEATURE_IS2000_REL_A_SVD */

          temp_demux_info.sec_len = MUX2_EIGHTH_PRI_LEN;
          mux = CAI_MUX2_FOR_25_ID;
        }
        else
        {
          /* eighth rate primary traffic only */
          temp_demux_info.pri_pos = MUX2_PRI_POS;
          temp_demux_info.pri_len = MUX2_EIGHTH_PRI_LEN;
          mux = CAI_MUX2_FOR_24_ID;
        }
        break;

      case CAI_NULL_RATE:

        if(is_channel_dcch)
        {
          mux = CAI_MUX2_FOR_DCCH_27_ID;
        }
        else
        {
          rate_error_detected = TRUE;
        }
        break;

      default:
        /* If rate is invalid, we may be missing decoder interrupts.
        ** For production code, drop the frame as
        ** a bad frame to recover gracefully.
        */
        rate_error_detected = TRUE;
        break;
    }

    #ifndef FEATURE_IS2000_REL_A_SVD
    if (rxc_so.sec == CAI_SO_NULL)
    {
      /* Throw out clearly invalid mux categories if we haven't
      ** negotiated a secondary traffic service option at this
      ** point in time.
      */

      switch (mux)
      {
        case CAI_MUX2_FOR_1_ID:
        case CAI_MUX2_FOR_2_ID:
        case CAI_MUX2_FOR_3_ID:
        case CAI_MUX2_FOR_4_ID:
        case CAI_MUX2_FOR_5_ID:
        case CAI_MUX2_FOR_11_ID:
        case CAI_MUX2_FOR_12_ID:
        case CAI_MUX2_FOR_13_ID:
        case CAI_MUX2_FOR_14_ID:
        case CAI_MUX2_FOR_19_ID:
        case CAI_MUX2_FOR_20_ID:
        case CAI_MUX2_FOR_21_ID:
        case CAI_MUX2_FOR_24_ID:
        case CAI_MUX2_FOR_DCCH_1_ID:
        case CAI_MUX2_FOR_DCCH_2_ID:
        case CAI_MUX2_FOR_DCCH_3_ID:
        case CAI_MUX2_FOR_DCCH_4_ID:
        case CAI_MUX2_FOR_DCCH_5_ID:
        case CAI_MUX2_FOR_DCCH_27_ID:
          break;

        default:
          invalid_mux_category = TRUE;
          break;
      }
    }
    #endif /* !FEATURE_IS2000_REL_A_SVD */

    if(!is_channel_dcch)
    {
      /* Throw out frame with data in the frame that is clearly bad.
      ** for 13K Markov reserved bits which should be zero.
      */
      if (rxc_so.pri == CAI_SO_RS2_MARKOV)
      {
        if ((mux == CAI_MUX2_FOR_1_ID) &&
            ((buf_ptr->buf.tc_buf.bits.buf.bits1[33] & 0x30) != 0))
        {
          bad_data_detected = TRUE;
        }
      }

      /* Throw out frame with data in the frame that is clearly bad.
      ** for 13K Voice reserved bits which should be zero.
      */
      if ( rxc_so.pri == CAI_SO_VOICE_13K ||
           rxc_so.pri == CAI_SO_VOICE_13K_IS733 )
      {
        if ((mux == CAI_MUX2_FOR_1_ID) &&
            ((buf_ptr->buf.tc_buf.bits.buf.bits1[32] & 0x30) != 0))
        {
          bad_data_detected = TRUE;
        }
      }
    }
  }

  /*--------------------Try rate set 1--------------------------------------*/

  else /* if ( rxc_get_fwd_link_rate_set(cur_rc) == DEC_RATE_9600 ) */
  {
    #ifdef FEATURE_IS2000_REL_A_SVD
    /* RLP needs to know the MUX PDU type. */
    rxc_mux_pdu = DSRLP_MUX_PDU_1;
    #endif /* FEATURE_IS2000_REL_A_SVD */

    switch (dec_rate)
    {
      case CAI_BLANK_RATE:
        if(is_channel_dcch)
        {
          mux = CAI_MUX1_FOR_DCCH_10_ID;
        }
        else
        {
          mux = CAI_MUX1_FOR_10_ID;
        }
        break;

      case CAI_FULL_RATE:

        if(is_channel_dcch)
        {
          #ifdef FEATURE_IS2000_REL_A_SVD
          dec_bits = buf_ptr->buf.tc_buf.bits.buf.bits1dcch;
          #else
          dec_bits = buf_ptr->buf.tc_buf.bits.buf.bits1;
          #endif /* FEATURE_IS2000_REL_A_SVD */
          mux = CAI_MUX1_FOR_DCCH_1_ID;
        }
        else
        {
          dec_bits = buf_ptr->buf.tc_buf.bits.buf.bits1;
          mux = CAI_MUX1_FOR_1_ID;
        }

        if ( (dec_bits[0] & MUX1_MM_BIT) )
        {
          frame_mode = ( dec_bits[0] & MUX1_FULL_FM ) >> MUX1_FULL_SHIFT;
          temp_demux_info.pri_pos  = mux1_full_pri_pos  [frame_mode];
          temp_demux_info.pri_len  = mux1_full_pri_len  [frame_mode];
          temp_demux_info.sig_pos  = mux1_full_sig_pos  [frame_mode];
          temp_demux_info.sig_len  = mux1_full_sig_len  [frame_mode];
          temp_demux_info.sec_len  = mux1_full_sec_len  [frame_mode];

          #ifdef FEATURE_IS2000_REL_A_SVD
          temp_demux_info.sec_pos  = mux1_full_sec_pos  [frame_mode];
          #endif /* FEATURE_IS2000_REL_A_SVD */

          mux += frame_mode + 1;

          #ifdef FEATURE_IS2000_REL_A_SVD
          /* If frame mode indicates full rate with secondary data, skip
          ** the mux categories between full rate without secondary
          ** data and full rate with secondary data.
          */
          if (frame_mode >= MUX1_MIN_FM_WITH_SEC)
          {
            mux += MUX1_SKIP_TO_SEC;
          }
          #endif /* FEATURE_IS2000_REL_A_SVD */
        }
        else
        {
          /* full rate primary traffic only */
          temp_demux_info.pri_pos = MUX1_FULL_PRI_POS;
          temp_demux_info.pri_len = MUX1_FULL_PRI_LEN;
        }
        break;

      case CAI_HALF_RATE:

        if(is_channel_dcch)
        {
          rate_error_detected = TRUE;
          break;
        }

        /* half rate primary traffic only */
        temp_demux_info.pri_pos = MUX1_OTHER_PRI_POS;
        temp_demux_info.pri_len = MUX1_HALF_PRI_LEN;
        mux = CAI_MUX1_FOR_6_ID;
        break;

      case CAI_QUARTER_RATE:

        if(is_channel_dcch)
        {
          rate_error_detected = TRUE;
          break;
        }

        /* quarter rate primary traffic only */
        temp_demux_info.pri_pos = MUX1_OTHER_PRI_POS;
        temp_demux_info.pri_len = MUX1_QTR_PRI_LEN;
        mux = CAI_MUX1_FOR_7_ID;
        break;

      case CAI_EIGHTH_RATE:

        if(is_channel_dcch)
        {
          rate_error_detected = TRUE;
          break;
        }

        /* eighth rate primary traffic only */
        temp_demux_info.pri_pos = MUX1_OTHER_PRI_POS;
        temp_demux_info.pri_len = MUX1_EIGHTH_PRI_LEN;
        mux = CAI_MUX1_FOR_8_ID;
        break;

      case CAI_NULL_RATE:

        if(is_channel_dcch)
        {
          mux = CAI_MUX1_FOR_DCCH_15_ID;
        }
        else
        {
          rate_error_detected = TRUE;
        }
        break;

      default:
        /* If rate is invalid, we may be missing decoder interrupts.
        ** For production code, drop the frame as
        ** a bad frame to recover gracefully.
        */
        rate_error_detected = TRUE;
        break;
    }

    #ifndef FEATURE_IS2000_REL_A_SVD
    /* Throw out clearly invalid mux categories since we could have
    ** received bogus frames and we don't do secondary data at this
    ** point in time.  Note! This should be modified if and when we
    ** start to process secondary data.
    */
    if ( !(mux >= CAI_MUX1_FOR_1_ID && mux <= CAI_MUX1_FOR_10_ID)
        && !((mux >= CAI_MUX1_FOR_DCCH_1_ID && mux <= CAI_MUX1_FOR_DCCH_5_ID) ||
             mux == CAI_MUX1_FOR_DCCH_15_ID)
       )

    {
      invalid_mux_category = TRUE;
    }
    #endif /* !FEATURE_IS2000_REL_A_SVD */
  }

  /*--------------------Error Handling--------------------------------------*/

  if( rate_error_detected || invalid_mux_category ||  bad_data_detected )
  {
    if(rate_error_detected)
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "Invalid Rate=0x%x for this channel. (is_channel_dcch=%d)",
        dec_rate,
        is_channel_dcch);
      dec_rate = CAI_BLANK_RATE;
    }

    if(invalid_mux_category)
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "Invalid Mux Category =0x%x",
        mux);
    }

    if(bad_data_detected)
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "Detected non-zero Reserved Bits.");
    }

    if ( rxc_get_fwd_link_rate_set(cur_rc) == DEC_RATE_14400 )
    {
      if(is_channel_dcch)
      {
        mux = CAI_MUX2_FOR_DCCH_26_ID;
      }
      else
      {
        mux = CAI_MUX2_FOR_26_ID;
      }
    }
    else
    {
      if(is_channel_dcch)
      {
        mux = CAI_MUX1_FOR_DCCH_10_ID;
      }
      else
      {
        mux = CAI_MUX1_FOR_10_ID;
      }
    }
  }

  #ifdef FEATURE_IS2000_REL_A_SVD
  if( (is_channel_dcch) &&
      (buf_ptr->dcch_chan_quality == RXC_POOR_CHANNEL) )
  {
    /* Throw out frames with poor channel quality. */
    if (dec_rate == CAI_FULL_RATE)
    {
      M1X_MSG( MUX, LEGACY_MED,
        "FullRate,PoorCh,CRC=%d,EN=%d,SER=%d",
        rxc_dcch_status.status[DEC_RATE_FULL-1],
        rxc_dcch_status.energy[DEC_RATE_FULL-1],
        rxc_dcch_status.ser[DEC_RATE_FULL-1] );
    }

    if ( rxc_get_fwd_link_rate_set(cur_rc) == DEC_RATE_14400 )
    {
      mux = CAI_MUX2_FOR_DCCH_26_ID;
    }
    else
    {
      mux = CAI_MUX1_FOR_DCCH_10_ID;
    }
  }
  #endif /* FEATURE_IS2000_REL_A_SVD */

  /*--------------------Save Demux Info--------------------------------------*/

  #ifdef FEATURE_IS2000_REL_A_SVD
  if(is_channel_dcch)
  {
    temp_demux_info.dec_pos = (word) offsetof(dec_fch_type,buf.bits1dcch);
  }
  else
  #endif /* FEATURE_IS2000_REL_A_SVD */
  {
    temp_demux_info.dec_pos = 0;

    if(dec_rate < ARR_SIZE(dec_position))
    {
      temp_demux_info.dec_pos = dec_position[dec_rate];
    }
  }

  /* Set lengths to 0 for frames with insufficient quality. */
  if( (mux == CAI_MUX1_FOR_10_ID) || (mux == CAI_MUX1_FOR_DCCH_10_ID) ||
      (mux == CAI_MUX2_FOR_26_ID) || (mux == CAI_MUX2_FOR_DCCH_26_ID) )
  {
    temp_demux_info.pri_len = 0;
    temp_demux_info.sig_len = 0;
    temp_demux_info.sec_len = 0;
  }

  if(demux_info != NULL)
  {
    *demux_info = temp_demux_info;
  }

  return mux;
}

#ifdef FEATURE_IS2000_REL_A_SVD
/*===========================================================================

FUNCTION RXC_TC_DATA_FCH

DESCRIPTION
  This procedure processes data received from the decoder while the Receive
  task is processing the Traffic Channel.  This procedure handles the F-FCH
  only.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
byte rxc_tc_data_fch
(
  rxc_dec_type *buf_ptr
  /* Pointer to buffer received on rxc_dec_q */
)
{
  rxc_demux_info_type demux_info = {0};
  cai_data_rate_type dec_rate;  /* Frame rate determined from RDA */
  byte mux = 0;                 /* MUX option determined for the frame */
  boolean is_guaranteed = TRUE; /* All frames are guaranteed by default */

  static byte fch_sig_buf[DEC_FCH_MAX_SIZ_FULL];
    /* Buffer for signaling data */

  mccrx_rpt_type *rpt_ptr;
    /* pointer to report to send to Main Control task */

  #ifdef FEATURE_MODEM_1X_MSO_RDA_TEST
  rxc_demux_info_type mso_demux_info = {0};
  cai_data_rate_type mar_to_cai_rate = CAI_BLANK_RATE; /* Mar converted to CAI rate */
  boolean is_markov_so = (
    #ifdef FEATURE_IS2000_1X_ADV
    (rxc_so.pri == CAI_SO_MARKOV_SO74)  ||
    #endif /* FEATURE_IS2000_1X_ADV */
    (rxc_so.pri == CAI_SO_MARKOV)       ||
    (rxc_so.pri == CAI_SO_MARKOV_13K)   ||
    (rxc_so.pri == CAI_SO_RS1_MARKOV)   ||
    (rxc_so.pri == CAI_SO_RS2_MARKOV)   ||
    (rxc_so.pri == CAI_SO_MARKOV_SO54) );
  #endif /* FEATURE_MODEM_1X_MSO_RDA_TEST */

  boolean rxc_ta_is_enabled = rxc_is_ta_enabled();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if( buf_ptr->dec_rate <= CAI_FULL_RATE )
  {
    dec_rate = buf_ptr->dec_rate;
  }
  else
  {
    dec_rate = CAI_BLANK_RATE;
    M1X_MSG( MUX, LEGACY_ERROR,
      "Invalid decoder rate (%d). Defaulting to BLANK_RATE",
      (int) buf_ptr->dec_rate);
  }

  /* -----------------------------------------------------------------
  ** Determine position and number of bits to be read from the decoder
  ** based on rate
  ** ----------------------------------------------------------------- */
  #ifdef FEATURE_MODEM_1X_MSO_RDA_TEST
  if(is_markov_so && rxc_is_mso_rda_test_enabled() )
  {
    /*MARKOV ONLY SO - use the expected rate to demux and not the
      result of the RDA. */
    rxc_exp_markov_rate = (mar_rate_type)
      mar_tx ( mar_tx_data, qw_lo (buf_ptr->buf.tc_buf.frame), TRUE );

    switch (rxc_exp_markov_rate)
    {
      case MAR_NC_8_RATE:
        #ifdef FEATURE_IS2000_1X_ADV
        if ( (rxc_so.for_fch.rc == (byte)CAI_RC_11) &&
             (!rxc_traffic_status.rxc_rc11.status.is_guaranteed_frame) )
        {
          rxc_exp_markov_rate = MAR_0_RATE;
          mar_to_cai_rate = CAI_BLANK_RATE;
        }
        else
        #endif /* FEATURE_IS2000_1X_ADV */
        {
          rxc_exp_markov_rate = MAR_8_RATE;
          mar_to_cai_rate = CAI_EIGHTH_RATE;
        }
        break;

      case MAR_8_RATE:
        mar_to_cai_rate = CAI_EIGHTH_RATE;
        break;

      case MAR_4_RATE:
        mar_to_cai_rate = CAI_QUARTER_RATE;
        break;

      case MAR_2_RATE:
        mar_to_cai_rate = CAI_HALF_RATE;
        break;

      case MAR_1_RATE:
        mar_to_cai_rate = CAI_FULL_RATE;
        break;

      default:
        M1X_MSG( MUX, LEGACY_ERROR,
          "Invalid Markov rate (%d). Defaulting to MAR_0_RATE",
          (int) rxc_exp_markov_rate);
        rxc_exp_markov_rate = MAR_0_RATE;
        mar_to_cai_rate = CAI_BLANK_RATE;
        break;
    }
  }
  #endif /* FEATURE_MODEM_1X_MSO_RDA_TEST */

  /* Demux the received frame using the rate from RDA. */
  mux = rxc_demux_fundicated_ch ( buf_ptr, dec_rate, &demux_info, FALSE);

  /*--------------------Update values for Power Measurement Report----------*/

  if (( mux == CAI_MUX1_FOR_10_ID ) || ( mux == CAI_MUX2_FOR_26_ID ))
  {
    if (rxc_pwr.num_delay_frames == 0)
    {
      /* If Return-If-Fail HHO or CFS is in progress, suspend counting
       * BAD_FRAMES per section 6.6.6.2.8.2 of IS-95B.
       */
      if ( ! rxc_rifhho_cfs_gps_in_progress )
      {
        #ifdef FEATURE_IS2000_1X_ADV
        /*For 1x Advanced RCs (RC11) check if this is a guaranteed F-FCH
          frame based on the forward blanking duty cycle. We only want to
          use guaranteed frames (indicated by valid_frame == TRUE) when
          counting bad frames because the mobile will not know the
          difference between a genuine erasure or a blanked frame from
          the base station.*/
        if ( ( rxc_so.for_fch.rc == CAI_RC_11 )  &&
             ( rxc_traffic_status.rxc_rc11.status.is_guaranteed_frame == FALSE )
           )
        {
          M1X_MSG( MUX, LEGACY_LOW,
            "Not a guaranteed frame for pwr meas rpt.");
        }
        else
        #endif /* FEATURE_IS2000_1X_ADV */
        {
          rxc_ta_bad_frame_count_inc(&(rxc_pwr.num_bad_frames));
        }
      }
    }
  }

  /*--------------------Power control channel fade check--------------------*/

  if ( rxc_so.for_fch.rc == CAI_RC_11 )
  {
    is_guaranteed = rxc_traffic_status.rxc_rc11.status.is_guaranteed_frame;
  }

  if (ffpc_channel == CAI_FPC_PRI_CHAN_FCH)
  {
    if (( mux == CAI_MUX1_FOR_10_ID ) || ( mux == CAI_MUX2_FOR_26_ID ))
    {
      rxc_ta_fade_check();
      srch_mux_report_frame ( TRUE, is_guaranteed, (uint16) dec_rate,
                              (word)rxc_so.for_fch.rc, &rxc_fch_status );
    }
    else
    {
      /* Reset T5m timer, if fade check is inactive and 1x is out of TA GAP */
      if( !rxc_fade_data.rxc_fade && !rxc_ta_is_enabled )
      {
        rxc_fade_data.rxc_t5m_bad_frames = 0;
      }

      srch_mux_report_frame ( FALSE, is_guaranteed, (uint16) dec_rate,
                              (word)rxc_so.for_fch.rc, &rxc_fch_status );
    }

    if( rxc_rifhho_cfs_gps_in_progress && !rxc_ta_is_enabled )
    {
      /* If Return-If-Fail HHO or CFS is in progress, start counting
       * lost frames as per section 6.6.6.2.8.2 of IS-95B.
       * LOST_FRAMES is used to measure the interval between the time
       * that the mobile station disables its transmitter and the time
       * that it resumes using the SF in the case of failed HHO with
       * Return on Failure.  Under some circumstances, a good frame can
       * be received on the TF/CF, but it is still a lost frame.
       */
      rxc_hho_data.rxc_lost_frames++;

      if (rxc_hho_data.rxc_lost_frames ==
              rxc_hho_data.rxc_rif_hho_timeout_frames)
      {
        /* rxc_ignore_good_frame is set to TRUE here to ignore any good
         * frames that come after the handoff has timed out.  This
         * prevents us from sending RXC_TX_ON_R after RXC_HHO_TIMEOUT_R.
         */
        rxc_ignore_good_frame = TRUE;

        /* send report of handoff timeout to Main Control task */
        if ( (rpt_ptr = (mccrx_rpt_type *)q_get(&mcc_rx_free_q) ) != NULL )
        {
          rpt_ptr->hdr.rpt = RXC_HHO_TIMEOUT_R;
          rxc_mc_rpt(rpt_ptr );
        }
        else
        {
          M1X_MSG( MUX, LEGACY_ERROR,
            "No buffers on mcc_rx_free_q" );
          rxc_hho_data.rxc_rif_hho_timeout_frames++; /* Try again next frame */
        }
      }
    }

    if (rxc_hho_data.rxc_hard_handoff_in_progress)
    {
      /* Allow fade checking to resume for the normal HHO begin process.*/
      rxc_fade_data.rxc_pre_tt_ho = FALSE;
    }

    /* If fade has started perform reception watchdog processing.
    ** Rel D fast call set up feature also requires this processing
    ** to send 2 valid frame report in TC state.
    */

    /* In time transfer cases we want to prevent RX from fade checking
       and issuing any reports back to MC until the HHO_BEGIN action is
       processed. */

    if (( rxc_fade_data.rxc_fade ) && ( rxc_fade_data.rxc_ho_cnt == 0 ) &&
        (!rxc_fade_data.rxc_pre_tt_ho))
    {
#ifdef FEATURE_IS2000_1X_ADV
    /*For 1x Advanced RCs (RC11) check if this is a guaranteed F-FCH
      frame based on the forward blanking duty cycle. We only want to
      use guaranteed frames (indicated by valid_frame == TRUE) when
      performing N2M checking because the mobile will not know the
      difference between a genuine erasure or a blanked frame from
      the base station.*/
      if ( rxc_so.for_fch.rc == CAI_RC_11 )
      {
        fade_check_fch (mux,
                        rxc_traffic_status.rxc_rc11.status.is_guaranteed_frame);
      }
      else
#endif /* FEATURE_IS2000_1X_ADV */
      {
        fade_check_fch (mux, TRUE);
      }
    }
  }

  /* increment relevant parameter */
  ( void )parm_inc( mux, 1 );

/*--------------------Process Signaling traffic---------------------------*/

  /* Process Signaling traffic */
  if( demux_info.sig_len > 0 )
  {
    b_copy( &buf_ptr->buf.tc_buf.bits.bits[demux_info.dec_pos], demux_info.sig_pos,
            fch_sig_buf, 0, demux_info.sig_len );
    M1X_MSG( MUX, LEGACY_MED,
      "Parsing signalling msg on FCH");
    /* If the signalling portion of the frame is bad, mark the
    ** entire frame as bad.
    */
    if (!rxc_tc_parse( buf_ptr->buf.tc_buf.frame, fch_sig_buf, demux_info.sig_len,
                       RXC_FOR_FCH ))
      mux = CAI_MUX2_FOR_26_ID;
  }
/*--------------------Process Primary traffic-----------------------------*/

  /* Process Primary traffic.
  */
  if (rxc_so.pri != CAI_SO_NULL)
  {
    if (DS_ISDATA_RS1(rxc_so.pri) || DS_ISDATA_RS2(rxc_so.pri))
    {
      /* record RLP bit length for logging */
      rxc_bufs_qs_data.rxc_rlp_logging_chan_info.log_fch_rlp_bits =
        demux_info.pri_len;
    }

#ifdef FEATURE_MODEM_1X_MSO_RDA_TEST
    if(is_markov_so && rxc_is_mso_rda_test_enabled() )
    {
      /*extract all the position and mux information using the expected rate.*/
      (void) rxc_demux_fundicated_ch ( buf_ptr, mar_to_cai_rate, &mso_demux_info,
                                       FALSE);

      /* Provide the demux info for the expected RDA rate for bit comparision. */
      rxc_pri( &buf_ptr->buf.tc_buf.bits.bits[mso_demux_info.dec_pos],
               mso_demux_info.pri_pos, mso_demux_info.pri_len, mux, buf_ptr );
    }
    else
#endif /* FEATURE_MODEM_1X_MSO_RDA_TEST */
    {
      /* Primary traffic service option active - process primary traffic
      */
      rxc_pri( &buf_ptr->buf.tc_buf.bits.bits[demux_info.dec_pos],
               demux_info.pri_pos, demux_info.pri_len, mux, buf_ptr );
    }
  }


/*--------------------Process Secondary traffic---------------------------*/

  /* Process secondary traffic.
  */
  if (rxc_so.sec != CAI_SO_NULL)
  {
    if (DS_ISDATA_RS1(rxc_so.sec) || DS_ISDATA_RS2(rxc_so.sec))
    {
      /* record RLP bit length for logging */
      rxc_bufs_qs_data.rxc_rlp_logging_chan_info.log_fch_rlp_bits =
        demux_info.sec_len;
    }

    /* Secondary traffic service option active - process secondary traffic
    */
    rxc_sec( &buf_ptr->buf.tc_buf.bits.bits[demux_info.dec_pos],
             demux_info.sec_pos, demux_info.sec_len,  mux, buf_ptr );
  }

  /* Return mux value for logging purpose */
  return (mux);

}/* end rxc_tc_data_fch */

/*===========================================================================

FUNCTION rxc_tc_data_get_rlp_list

DESCRIPTION
  This function gets 1 linked list element from RLP client to store
  information of 1 PDU payload.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void rxc_tc_data_get_rlp_list (void)
{
  rxc_mux_rlp_data.rxc_elem_ptr = NULL;

  /* rxc_elem_ptr will point to 1 RLP element to give to RLP once payload
  ** has been processed.  Elements are initialized to default values in
  ** dsdvrlp_alloc_fwd_frames().
  */
  rxc_mux_rlp_data.rxc_elem_ptr = dsdvrlp_alloc_fwd_frames (1);

  if (rxc_mux_rlp_data.rxc_elem_ptr == NULL)
  {
    /* If RLP does not allocate the linked list, drop the OTA
    ** RLP frame data.
    */
    M1X_MSG( MUX, LEGACY_ERROR,
      "Failed to obtain fdch list from RLP");
  }
} /* end rxc_tc_data_get_rlp_list */

/*===========================================================================

FUNCTION rxc_tc_data_give_rlp_list

DESCRIPTION
  This function gives 1 PDU worth of channel data to RLP client.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void rxc_tc_data_give_rlp_list (void)
{
  /* Give channel data to RLP, if any.
  */
  if (rxc_mux_rlp_data.rxc_elem_ptr != NULL)
  {
    dsdvrlp_enqueue_fwd_frames (rxc_mux_rlp_data.rxc_elem_ptr, 1);
    dsdvrlp_process_fwd_frames ();
  }

} /* end rxc_tc_data_give_rlp_list */

/*===========================================================================

FUNCTION rxc_tc_data_service_sch

DESCRIPTION
  This function services the Supplemental channel.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void rxc_tc_data_service_sch (void)
{
  byte i;
  boolean rlp_buffer_error = FALSE;
  word    rlp_partial_buf_cnt = 0;

  /* Process number of PDUs read from decoder this frame.
  */
  if ((rxc_log_sch_data.rxc_dsm_arr_cnt > 0) &&
      (rxc_log_sch_data.rxc_dsm_arr_cnt <= SCH_MAX_FWD_PDU))
  {
    /* Request the exact number of decoded PDUs from RLP.
    ** Elements are initialized to default values in
    ** dsdvrlp_alloc_fwd_frames().
    */
    rxc_mux_rlp_data.rxc_sch_1st_elem_ptr =
      dsdvrlp_alloc_fwd_frames (rxc_log_sch_data.rxc_dsm_arr_cnt);

    if (rxc_mux_rlp_data.rxc_sch_1st_elem_ptr != NULL)
    {
      /* Demultiplex the SCH channel and build SCH PDUs for RLP.
      */
      rxc_data_build_sch ();

      /* Give SCH data to RLP.  Do not give RLP PDUs which has SR_ID of "111".
      ** Thus, rxc_num_rlp_pdu should always be <= rxc_dsm_arr_cnt.
      ** rxc_num_rlp_pdu is set in rxc_data_build_sch (), which drops
      ** fill frames (SR_ID of "111") and returns unused dsm buffers.
      */
      dsdvrlp_enqueue_fwd_frames (rxc_mux_rlp_data.rxc_sch_1st_elem_ptr,
                                  (uint8) rxc_mux_rlp_data.rxc_num_rlp_pdu);
      dsdvrlp_process_fwd_frames ();

      /* Clear the count, as data has been given to RLP.
      */
      rxc_log_sch_data.rxc_dsm_arr_cnt = 0;
    }
    else
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "Failed to obtain SCH list from RLP");
    }
  } /* end if ((rxc_dsm_arr_cnt > 0)... */

  /* This loop makes sure that all the buffers were allocated. In case
  ** if the new buffer returns NULL this time, it will try to allocate
  ** again the next time.  This array contains the PDU payload.
  */
  for (i = 0; i < SCH_MAX_FWD_PDU; i++)
  {
    if (rxc_log_sch_data.rxc_dsm_arr[i] == NULL)
    {
      /* Re-load the array with a new dsm item.
      */
      rxc_log_sch_data.rxc_dsm_arr[i] = dsm_new_buffer(DSM_DS_SMALL_ITEM_POOL);

      /* If less than 8 buffers are available, treat as error case.
      */
      if (rxc_log_sch_data.rxc_dsm_arr[i] == NULL)
      {
        rlp_buffer_error = TRUE;
        break;
      }
    }
  }
  /* Error case, return the rest of the buffers, try again next frame.
  */
  if (rlp_buffer_error == TRUE)
  {
    for( i=0; i< SCH_MAX_FWD_PDU; i++)
    {
      if (rxc_log_sch_data.rxc_dsm_arr[i] != NULL)
      {
        dsm_free_packet( &rxc_log_sch_data.rxc_dsm_arr[i]);
        rxc_log_sch_data.rxc_dsm_arr[i] = NULL;
        rlp_partial_buf_cnt++;
      }
    }
    M1X_MSG( MUX, LEGACY_ERROR,
      "Not enough DSM items %d",
      rlp_partial_buf_cnt);

    /* Set array count to RXC_DSM_ARR_NOT_ALLOC so RXC ISR function will not
    ** read decoder output for this frame (because rxc_dsm_arr_cnt
    ** is not 0) and this function will not try to parse this frame
    ** as well (because rxc_dsm_arr_cnt is not within valid range).
    ** Setting it to SCH_MAX_FWD_PDU or any positive count lower than
    ** max causes data abort in this function.)
    **
    ** Array count is reset once good DSM items are obtained.
    */
    rxc_log_sch_data.rxc_dsm_arr_cnt = RXC_DSM_ARR_NOT_ALLOC;
  }

} /* end rxc_tc_data_service_sch */

/*===========================================================================

FUNCTION rxc_tc_data_init_rlp_buffer

DESCRIPTION
  Initialize RLP buffer used to debug MUX/RLP interface.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void rxc_tc_data_init_rlp_buffer (void)
{
  byte i;
  for ( i = 0; i < RLP_MAX_FWD_PDU; i++ )
  {
    rxc_frm_arr[i].this_dsm_item_ptr  = NULL;
    rxc_frm_arr[i].actual_fr_item_ptr = NULL;
    rxc_frm_arr[i].srvc_type          = DSRLP_GENERIC_SRVC;
    rxc_frm_arr[i].mux_pdu_type       = DSRLP_MUX_PDU_MAX;
    rxc_frm_arr[i].fr_rate            = DSRLP_RATE_INACTIVE;
    rxc_frm_arr[i].sr_id              = 1;
    rxc_frm_arr[i].chan               = DSDVRLP_RX_FCH;
  }
} /* end rxc_tc_data_init_rlp_buffer */

/*===========================================================================

FUNCTION RXC_TC_DATA

DESCRIPTION
  This procedure processes data received from the decoder while the Receive
  task is processing the Traffic Channel.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_tc_data( void )
{
  rxc_dec_type *buf_ptr;
    /* Pointer to buffer received on rxc_dec_q */

  static byte fch_mux = 0;
    /* MUX option determined for the frame on FCH channel */

  static byte dcch_mux = 0;
    /* MUX option determined for the frame on DCCH channel */

  boolean is_guaranteed = TRUE;
    /* All frames are guaranteed by default */

#ifdef FEATURE_IS2000_REL_A
  /* flush the FDCH buffer */
  static boolean flush_fdch_buffer = FALSE;
#endif /* End of FEATURE_IS2000_REL_A */

  /* flag to indicate the RDA log packet should be flused upon  closing the log */
  static boolean rxc_flush_rda_log_packet = FALSE;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  (void) rex_clr_sigs( RX_TCB_PTR, RXC_INT_SIG );

  if ( rxc_so.for_fch.included && rxc_so.for_fch.rc == CAI_RC_11 )
  {
    is_guaranteed = rxc_traffic_status.rxc_rc11.status.is_guaranteed_frame;
  }

  /* Get a time stamp at the start of the demuxing operation for debugging purpose.
  */
  m1x_time_get_20ms_frame_offset_time (rxc_start_demux_fr_time);

  while ((buf_ptr = (rxc_dec_type *)q_get( &rxc_common_data.rxc_dec_q )) !=
         NULL)
  {
    if(!onex_nonsig_is_ftm_mode())
    {
      if (buf_ptr->state == RXC_TC_STATE)
      {

/*--------------------Process Demux function------------------------------*/

        if( (rxc_so.pri == CAI_SO_PPP_PKT_DATA_3G) ||
            (rxc_so.sec == CAI_SO_PPP_PKT_DATA_3G))
        {
          /* Also need to initialize RLP logging struct before
          ** constructing this frame's RLP 3 payload data for logging.
          ** first_entry will be cleared after the 1st entry is logged, so
          ** set it now since this is the start of the 20ms frame demux.
          */
          rxc_bufs_qs_data.rxc_rlp_logging_chan_info.log_fch_rlp_bits = 0;
          rxc_bufs_qs_data.rxc_rlp_logging_chan_info.log_dcch_rlp_bits = 0;
          rxc_bufs_qs_data.rxc_rlp_logging_chan_info.first_entry = TRUE;
          rxc_tc_data_init_rlp_buffer(); // init rlp buffer for debugging

          if( (rxc_so.pri == CAI_SO_PPP_PKT_DATA_3G) &&
              (rxc_so.sec == CAI_SO_PPP_PKT_DATA_3G))
          {
            M1X_MSG( MUX, LEGACY_ERROR,
              "Pri and Sec are both RLP");
          }
        } /* end if( (rxc_so.pri == CAI_SO_PPP_PKT_DATA_3G) ... */

        /* Demultiplex the fundicated channels.
        */
        if ((rxc_so.for_fch.included) && (rxc_so.for_fch.data_valid))
        {
          rxc_curr_ch = DSDVRLP_RX_FCH;         // set current ch to FCH
          fch_mux = rxc_tc_data_fch (buf_ptr);    // service FCH channel
        }
        if ((rxc_so.for_dcch.included) && (rxc_so.for_dcch.data_valid))
        {
          rxc_curr_ch = DSDVRLP_RX_DCCH;        // set current ch to DCCH
          dcch_mux = rxc_tc_data_dcch (buf_ptr);  // service DCCH channel
        }

        if( (rxc_so.pri == CAI_SO_PPP_PKT_DATA_3G) ||
            (rxc_so.sec == CAI_SO_PPP_PKT_DATA_3G))
        {
          /* Service the supplemental channel for RLP.
          */
          rxc_tc_data_service_sch ();              // service SCH channel
        }

        /* Check for SCH update, now that the frame has been assembled
        ** for RLP, it's a good time to update SCH parameters (meant for
        ** future frames, if there have been changes.)  This call is
        ** specifically designed for the burst with 1 frame gap cases.
        ** See comments inside rxc_check_for_sch_update() for timing
        ** information.
        */
        rxc_check_for_sch_update ();

        if( (rxc_so.pri == CAI_SO_PPP_PKT_DATA_3G) ||
            (rxc_so.sec == CAI_SO_PPP_PKT_DATA_3G))
        {
          /* Every frame, RLP layer needs to be called per RLP API,
          ** regardless if there is any new data.
          */
          dsdvrlp_process_20ms_tick (0,0);

          /* Call RLP function to perform RLP statistics logging.
          */
          dsrlp_log_rlp_statistics( DSRLP_FIXED_SR_ID_INDEX, FALSE );
            /* Using the RLP instance here,
            instead of the sr_id since that is how this function is
            structured right now. So, we will end up logging the
            statistics against the RLP instance instead of sr_id number
            Duplicate this function into 2 functions - one with sr_id
            and one with RLP instance if it is desired to log the stats
            against sr_id and pass rxc_fwd_fr_blk.sr_id */

        } /* end if( (rxc_so.pri == CAI_SO_PPP_PKT_DATA_3G) ... */

/*--------------------Perform RDA logging--------------------------------*/

        /* execute the new log function for release A */
        if (log_status(LOG_FDCH_FRAME_INFO_C))
        {
          rxc_fdch_log_frame_info(fch_mux, dcch_mux);
          flush_fdch_buffer = TRUE;
        }
        else if (flush_fdch_buffer)
        {
          /* flush the buffer */
          rxc_fdch_frame_info_send_log();
          flush_fdch_buffer = FALSE;
        }
        /* call the function to log the RDA packet */
        if (log_status(LOG_RDA_FRAME_INFO_2_C))
        {
          rxc_rda_log_frame_info (buf_ptr->dec_rate,buf_ptr->dcch_dec_rate);
          rxc_flush_rda_log_packet = TRUE;
        }
        else if (rxc_flush_rda_log_packet)
        {
          rxc_rda_frame_info_send_log();
          rxc_flush_rda_log_packet = FALSE;
        }

/*--------------------Check set point threshold---------------------------*/

        /* verify if SETPT_THRESH_DELTA has exceeded */
        ffpc_check_setpt_thresh ();

      } /* end if (buf_ptr->state == RXC_TC_STATE) */

      /* Process Power measurement reports */
      // Still needs work for VP2
      pwr_msr_check();

      /* Free the decoder buffer queue element so that it can be used in the
      ** next traffic channel frame.
      */
      q_put(&rxc_common_data.rxc_dec_free_q, &buf_ptr->link );
    }
    else
    {

      srch_mux_report_frame ( ((buf_ptr->dec_rate == CAI_BLANK_RATE) ?
                                      TRUE : FALSE),
                              is_guaranteed,
                              (uint16) buf_ptr->dec_rate,
                              (word)((rxc_so.for_fch.included) ?
                                         rxc_so.for_fch.rc : rxc_so.for_dcch.rc),
                              ((rxc_so.for_fch.included) ?
                                         &rxc_fch_status : &rxc_dcch_status) );

      if (log_status(LOG_RDA_FRAME_INFO_2_C))
      {
        rxc_rda_log_frame_info (buf_ptr->dec_rate,buf_ptr->dcch_dec_rate);
        rxc_flush_rda_log_packet = TRUE;
      }

      rxc_tc_data_ftm ( buf_ptr );
    }
  } /* end while loop */

  /* Get a time stamp at the end of the demuxing operation for debugging purpose.
  */
  m1x_time_get_20ms_frame_offset_time (rxc_end_demux_fr_time);

} /* end rxc_tc_data */

#else  /* FEATURE_IS2000_REL_A_SVD */

/*===========================================================================

FUNCTION RXC_TC_DATA

DESCRIPTION
  This procedure processes data received from the decoder while the Receive
  task is processing the Traffic Channel.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_tc_data( void )
{
  rxc_dec_type *buf_ptr;
    /* Pointer to buffer received on rxc_dec_q */
  rxc_demux_info_type demux_info = {0};
  cai_data_rate_type dec_rate;  /* Frame rate determined from RDA */
  byte mux = 0;                 /* MUX1 option determined for the frame */
  boolean is_guaranteed = TRUE; /* All frames are guaranteed by default */

  static byte sig_buf[DEC_FCH_MAX_SIZ_FULL];
    /* Buffer for signaling data */

  mccrx_rpt_type *rpt_ptr;
    /* pointer to report to send to Main Control task */

  #ifdef FEATURE_MODEM_1X_MSO_RDA_TEST
  rxc_demux_info_type mso_demux_info = {0};
  cai_data_rate_type mar_to_cai_rate; /* Mar converted to CAI rate */
  boolean is_markov_so = (
    #ifdef FEATURE_IS2000_1X_ADV
    (rxc_so.pri == CAI_SO_MARKOV_SO74)  ||
    #endif /* FEATURE_IS2000_1X_ADV */
    (rxc_so.pri == CAI_SO_MARKOV)       ||
    (rxc_so.pri == CAI_SO_MARKOV_13K)   ||
    (rxc_so.pri == CAI_SO_RS1_MARKOV)   ||
    (rxc_so.pri == CAI_SO_RS2_MARKOV)   ||
    (rxc_so.pri == CAI_SO_MARKOV_SO54) );
    #endif /* FEATURE_MODEM_1X_MSO_RDA_TEST */

  // Holds RC of FCH/DCCH
  cai_radio_config_type cur_rc;

  #ifdef FEATURE_IS2000_REL_A
  /* flush the FDCH buffer */
  static boolean flush_fdch_buffer = FALSE;
  #endif /* End of FEATURE_IS2000_REL_A */

  /* flag to indicate the RDA log packet should be flushed
     upon closing the log */
  static boolean rxc_flush_rda_log_packet = FALSE;

  boolean rxc_ta_is_enabled;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( rxc_so.for_fch.included && rxc_so.for_fch.rc == CAI_RC_11 )
  {
    is_guaranteed = rxc_traffic_status.rxc_rc11.status.is_guaranteed_frame;
  }

  (void) rex_clr_sigs( RX_TCB_PTR, RXC_INT_SIG );

  while( ( buf_ptr = (rxc_dec_type *)q_get( &rxc_common_data.rxc_dec_q ) ) !=
          NULL )
  {
    rxc_ta_is_enabled = rxc_is_ta_enabled();

    if( !onex_nonsig_is_ftm_mode() )
    {
      if( buf_ptr->state == RXC_TC_STATE )
      {
        // Note: this only works if one of FCH/DCCH included
        cur_rc = ( rxc_so.for_dcch.included ?
                   rxc_so.for_dcch.rc : rxc_so.for_fch.rc );

        if( buf_ptr->dec_rate <= CAI_FULL_RATE )
        {
          dec_rate = buf_ptr->dec_rate;
        }
        else
        {
          dec_rate = CAI_BLANK_RATE;
          M1X_MSG( MUX, LEGACY_ERROR,
            "Invalid decoder rate (%d). Defaulting to BLANK_RATE",
            (int) buf_ptr->dec_rate);
        }

        /* -----------------------------------------------------------------
        ** Determine position and number of bits to be read from the decoder
        ** based on rate
        ** ----------------------------------------------------------------- */
        #ifdef FEATURE_MODEM_1X_MSO_RDA_TEST
        if(is_markov_so && rxc_so.for_fch.included &&
           rxc_is_mso_rda_test_enabled() )
        {
          /*MARKOV ONLY SO - use the expected rate to demux and not the
            result of the RDA. */
          rxc_exp_markov_rate = (mar_rate_type)
            mar_tx ( mar_tx_data, qw_lo (buf_ptr->buf.tc_buf.frame), TRUE );

          switch (rxc_exp_markov_rate)
          {
            case MAR_NC_8_RATE:
              #ifdef FEATURE_IS2000_1X_ADV
              if ( (rxc_so.for_fch.rc == (byte)CAI_RC_11) &&
                   (!rxc_traffic_status.rxc_rc11.status.is_guaranteed_frame) )
              {
                rxc_exp_markov_rate = MAR_0_RATE;
                mar_to_cai_rate = CAI_BLANK_RATE;
              }
              else
              #endif /* FEATURE_IS2000_1X_ADV */
              {
                rxc_exp_markov_rate = MAR_8_RATE;
                mar_to_cai_rate = CAI_EIGHTH_RATE;
              }
              break;

            case MAR_8_RATE:
              mar_to_cai_rate = CAI_EIGHTH_RATE;
              break;

            case MAR_4_RATE:
              mar_to_cai_rate = CAI_QUARTER_RATE;
              break;

            case MAR_2_RATE:
              mar_to_cai_rate = CAI_HALF_RATE;
              break;

            case MAR_1_RATE:
              mar_to_cai_rate = CAI_FULL_RATE;
              break;

            default:
              M1X_MSG( MUX, LEGACY_ERROR,
                "Invalid Markov rate (%d). Defaulting to MAR_0_RATE",
                (int) rxc_exp_markov_rate);
              rxc_exp_markov_rate = MAR_0_RATE;
              mar_to_cai_rate = CAI_BLANK_RATE;
              break;
          }
        }
        #endif /* FEATURE_MODEM_1X_MSO_RDA_TEST */

        /*NON-MARKOV SOs - Use the RDA result.*/
        mux = rxc_demux_fundicated_ch ( buf_ptr, dec_rate, &demux_info,
                                        rxc_so.for_dcch.included);

        /*-------------------------------------------------------------------*/

        // Note: following assume only 1 of FCH/DCCH supported
        if (mux == CAI_MUX1_FOR_DCCH_10_ID || mux == CAI_MUX2_FOR_DCCH_26_ID ||
            mux == CAI_MUX1_FOR_10_ID || mux == CAI_MUX2_FOR_26_ID ||
            (rxc_so.for_dcch.included &&
            buf_ptr->dcch_chan_quality == RXC_POOR_CHANNEL))
        {
          if (rxc_pwr.num_delay_frames == 0)
          {
            /* If Return-If-Fail HHO or CFS is in progress, suspend counting
             * BAD_FRAMES per section 6.6.6.2.8.2 of IS-95B.
             */
            if ( ! rxc_rifhho_cfs_gps_in_progress )
            {
              #ifdef FEATURE_IS2000_1X_ADV
              /*For 1x Advanced RCs (RC11) check if this is a guaranteed F-FCH
                frame based on the forward blanking duty cycle. We only want to
                use guaranteed frames (indicated by valid_frame == TRUE) when
                counting bad frames because the mobile will not know the
                difference between a genuine erasure or a blanked frame from
                the base station.*/
              if ( ( rxc_so.for_fch.rc == CAI_RC_11 )  &&
                   ( rxc_traffic_status.rxc_rc11.status.is_guaranteed_frame ==
                         FALSE ) )
              {
                M1X_MSG( MUX, LEGACY_LOW,
                  "Not a guaranteed frame for pwr meas rpt.");
              }
              else
              #endif /* FEATURE_IS2000_1X_ADV */
              {
                rxc_ta_bad_frame_count_inc(&(rxc_pwr.num_bad_frames));
              }
            }

          }
          rxc_ta_fade_check();

          srch_mux_report_frame ( TRUE, is_guaranteed, (uint16) dec_rate,
                                  (word)cur_rc, &rxc_fch_status );
        }
        else
        {
          /* Reset T5m timer, if fade check is inactive and 1x is out of TA GAP */
          if( !rxc_fade_data.rxc_fade && !rxc_ta_is_enabled )
          {
            rxc_fade_data.rxc_t5m_bad_frames = 0;
          }

          srch_mux_report_frame ( FALSE, is_guaranteed, (uint16) dec_rate,
                                  (word)cur_rc, &rxc_fch_status );
        }

        if( rxc_rifhho_cfs_gps_in_progress && !rxc_ta_is_enabled )
        {
          /* If Return-If-Fail HHO or CFS is in progress, start counting
           * lost frames as per section 6.6.6.2.8.2 of IS-95B.
           * LOST_FRAMES is used to measure the interval between the time
           * that the mobile station disables its transmitter and the time
           * that it resumes using the SF in the case of failed HHO with
           * Return on Failure.  Under some circumstances, a good frame can
           * be received on the TF/CF, but it is still a lost frame.
           */
          rxc_hho_data.rxc_lost_frames++;

          if (rxc_hho_data.rxc_lost_frames ==
               rxc_hho_data.rxc_rif_hho_timeout_frames)
          {
            /* rxc_ignore_good_frame is set to TRUE here to ignore any good
             * frames that come after the handoff has timed out.  This
             * prevents us from sending RXC_TX_ON_R after RXC_HHO_TIMEOUT_R.
             */
            rxc_ignore_good_frame = TRUE;

            /* send report of handoff timeout to Main Control task */
            if ( (rpt_ptr = (mccrx_rpt_type *)q_get(&mcc_rx_free_q) ) != NULL )
            {
              rpt_ptr->hdr.rpt = RXC_HHO_TIMEOUT_R;
              rxc_mc_rpt(rpt_ptr );
            }
            else
            {
              M1X_MSG( MUX, LEGACY_ERROR,
                "No buffers on mcc_rx_free_q" );
              rxc_hho_data.rxc_rif_hho_timeout_frames++; /* Try again next frame */
            }
          }
        }

        /* Process Power measurement reports */
        pwr_msr_check();

        if (rxc_hho_data.rxc_hard_handoff_in_progress)
        {
          /* Allow fade checking to resume for the normal HHO begin process.*/
          rxc_fade_data.rxc_pre_tt_ho = FALSE;
        }

        /* If fade has started perform reception watchdog processing.
        ** Rel D fast call set up feature also requires this processing
        ** to send 2 valid frame report in TC state.
        */

        /* In time transfer cases we want to prevent RX from fade checking
           and issuing any reports back to MC until the HHO_BEGIN action is
           processed. */

        if (( rxc_fade_data.rxc_fade ) && ( rxc_fade_data.rxc_ho_cnt == 0 ) &&
            (!rxc_fade_data.rxc_pre_tt_ho))
        {
          #ifdef FEATURE_IS2000_1X_ADV
          /*For 1x Advanced RCs (RC11) check if this is a guaranteed F-FCH
            frame based on the forward blanking duty cycle. We only want to
            use guaranteed frames (indicated by valid_frame == TRUE) when
            performing N2M checking because the mobile will not know the
            difference between a genuine erasure or a blanked frame from
            the base station */
          if ( rxc_so.for_fch.rc == CAI_RC_11 )
          {
            fade_check (mux, rxc_traffic_status.rxc_rc11.status.is_guaranteed_frame
                        ,buf_ptr->dcch_chan_quality);
          }
          else
          #endif /* FEATURE_IS2000_1X_ADV */
          {
            fade_check (mux, TRUE, buf_ptr->dcch_chan_quality);
          }
        }

        /* increment relevant parameter */
        ( void )parm_inc( mux, 1 );

        /*-------------------------------------------------------------------*/
        #ifndef FEATURE_IS2000_REL_A
        if (log_status(LOG_FWD_FRAME_TYPES_C))
        {
          mux_log_fch_type_enum_type fch_type = MUX_LOG_FCH_TYPE_PAYLOAD_ONLY;

          /* Determine FCH frame type */
          if (demux_info.sig_len > 0)
          {
            if (demux_info.pri_len > 0)
            {
              fch_type = MUX_LOG_FCH_TYPE_DIM_BURST;
            }
            else
            {
              fch_type = MUX_LOG_FCH_TYPE_BLANK_BURST;
            }
          }

          /* wait for the spurious interrupts to be over
          */
          if (rxc_frame_offset_countdown == 0)
          {
            /* Now log the types for this frame. */
            rxc_log_frame_type( buf_ptr->dec_rate, fch_type,
                     (mux_log_sch_rate_enum_type)rxc_frame_data.rxc_sch_frame_status,
                     /* right now only one channel is supported, only the
                        last two bits are important */
                     (rxc_so.for_dcch.included ?
                      CAI_CHIND_DCCH_ONLY : CAI_CHIND_FCH_ONLY),
                     mux );
          }
        }
        #else
        /* execute the new log function for release A */
        if (log_status(LOG_FDCH_FRAME_INFO_C))
        {
          rxc_fdch_log_frame_info(mux);
          flush_fdch_buffer = TRUE;
        }
        else if (flush_fdch_buffer)
        {
          /* flush the buffer */
          rxc_fdch_frame_info_send_log();
          flush_fdch_buffer = FALSE;
        }
        #endif /* FEATURE_IS2000_REL_A */

        /* call the function to log the RDA packet */
        if (log_status(LOG_RDA_FRAME_INFO_2_C))
        {
          rxc_rda_log_frame_info(buf_ptr->dec_rate);
          rxc_flush_rda_log_packet = TRUE;
        }
        else if (rxc_flush_rda_log_packet)
        {
          rxc_rda_frame_info_send_log();
          rxc_flush_rda_log_packet = FALSE;
        }

        /* Process Signaling traffic */
        if( demux_info.sig_len > 0 )
        {
          b_copy( &buf_ptr->buf.tc_buf.bits.bits[demux_info.dec_pos],
                  demux_info.sig_pos, sig_buf, 0, demux_info.sig_len );

          /* If the signalling portion of the frame is bad, mark the
          ** entire frame as bad.
          */
          if (!rxc_tc_parse( buf_ptr->buf.tc_buf.frame,
                             sig_buf, demux_info.sig_len,
                             ( rxc_so.for_dcch.included ?
                               RXC_FOR_DCCH : RXC_FOR_FCH )
                           )
             )
          {
            mux = CAI_MUX2_FOR_26_ID;
          }
        }
        /*-------------------------------------------------------------------*/

        /* Also need to initialize RLP logging struct before
        ** constructing this frame's RLP 3 payload data for logging.
        ** first_entry will be cleared after the 1st entry is logged, so
        ** set it now since this is the start of the 20ms frame demux.
        */
        rxc_bufs_qs_data.rxc_rlp_logging_chan_info.log_fch_rlp_bits = 0;
        rxc_bufs_qs_data.rxc_rlp_logging_chan_info.log_dcch_rlp_bits = 0;
        rxc_bufs_qs_data.rxc_rlp_logging_chan_info.first_entry = TRUE;

        /* Process Primary traffic */
        if( (rxc_so.pri != CAI_SO_NULL) )
        {
          /* Decoder buffer queue element buf_ptr is freed inside the
             rxc_pri() after the data is either processed or copied to other
             items. This was added to free the buffer element early to avoid
             "no free buffer on dec queue" problem observed occasionally */

          #ifdef FEATURE_MODEM_1X_MSO_RDA_TEST
          if(is_markov_so && rxc_is_mso_rda_test_enabled() )
          {
            /*extract all the position and mux information using the
              expected rate.*/
            (void) rxc_demux_fundicated_ch ( buf_ptr, mar_to_cai_rate,
                                             &mso_demux_info, FALSE);

            /* Provide the demux info for the expected RDA rate for
               bit comparision. */
            rxc_pri( &buf_ptr->buf.tc_buf.bits.bits[mso_demux_info.dec_pos],
                     mso_demux_info.pri_pos, mso_demux_info.pri_len,
                     mux,  buf_ptr );
          }
          else
          #endif /* FEATURE_MODEM_1X_MSO_RDA_TEST */
          {
            /* Primary traffic SO active - process primary traffic */
            rxc_pri( &buf_ptr->buf.tc_buf.bits.bits[demux_info.dec_pos],
                     demux_info.pri_pos, demux_info.pri_len, mux,  buf_ptr );
          }
        }
        else
        {
          /* Free the decoder buffer queue element as rxc_pri() was not
             executed. This part of the code will be executed for first few
             frames after the call is terminated because Service Option will
             be null for those */

          q_put(&rxc_common_data.rxc_dec_free_q, &buf_ptr->link );
        }

        /*-------------------------------------------------------------------*/
        /* Process Secondary traffic */
        if( demux_info.sec_len > 0 )
        {
          /* Note: Decoder buffer "buf_ptr" is freed inside the rxc_pri()
             called above. Hence so it is not avaible here. If we decide to
             process secondary data in future, proper care should be taken to
             get the data here */

          /* For now we don't do anything with secondary data */
          M1X_MSG( MUX, LEGACY_LOW,
            "%d bits Secondary data dropped",
            demux_info.sec_len );
        }

        if (rxc_so.pri == CAI_SO_PPP_PKT_DATA_3G)
        {
          /* Every frame, RLP layer needs to be called per RLP API,
          ** regardless if there is any new data.
          */
          dsdvrlp_process_20ms_tick (0,0);

          /* Call RLP function to perform RLP statistics logging */
          dsrlp_log_rlp_statistics( DSRLP_FIXED_SR_ID_INDEX, FALSE );
            /* Using the RLP instance here, instead of the sr_id since that
               is how this function is structured right now. So, we will end
               up logging the statistics against the RLP instance instead of
               sr_id number. Duplicate this function into 2 functions - one
               with sr_id and one with RLP instance if it is desired to log
               the stats against sr_id and pass rxc_fwd_fr_blk.sr_id */

        } /* end if (rxc_so.pri == CAI_SO_PPP_PKT_DATA_3G) */

        /*-------------------------------------------------------------------*/
        /* verify if SETPT_THRESH_DELTA has exceeded */
        ffpc_check_setpt_thresh ();

      }/* if state == RXC_TC_STATE */

      else
      {
        /* If the state is not TC then we need to free the decoder buffer queue
           element so that it can be used in the next traffic channel frame */

        /* Free the decoder free buffer queue element */
        q_put(&rxc_common_data.rxc_dec_free_q, &buf_ptr->link );
      }
    }
    else
    {
      srch_mux_report_frame ( ((buf_ptr->dec_rate == CAI_BLANK_RATE) ?
                               TRUE : FALSE),
                              is_guaranteed,
                              (uint16) buf_ptr->dec_rate,
                              (word)rxc_so.for_fch.rc,
                              &rxc_fch_status );

      if (log_status(LOG_RDA_FRAME_INFO_2_C))
      {
        rxc_rda_log_frame_info(buf_ptr->dec_rate);
        rxc_flush_rda_log_packet = TRUE;
      }

      rxc_tc_data_ftm ( buf_ptr );
    }
  }
}/* rxc_tc_data */
#endif /* FEATURE_IS2000_REL_A_SVD */



